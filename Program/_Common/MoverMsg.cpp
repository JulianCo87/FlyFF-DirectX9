#include "stdafx.h"
#include "defineText.h"
#ifdef __CLIENT
#include "AppDefine.h"
#endif

#include "ModelObject.h"

#ifdef __CLIENT
#include "../Neuz/DPClient.h"
extern	CDPClient	g_DPlay;
#else	// __CLIENT
#ifdef __WORLDSERVER
#include "../WorldServer/User.h"
#include "WorldMng.h"
extern	CWorldMng	g_WorldMng;
#endif	// __WORLDSERVER
#endif	// __CLIENT

#include "ship.h"


/*--------------------------------------------------------------------------------------------------------


											Mover act command


----------------------------------------------------------------------------------------------------------*/

void	CMover::SetCmd( OBJACT cmd, int nParam1, int nParam2, int nParam3 )
{
	m_oaCmd = cmd;
	m_nCParam[0] = nParam1;		// ��� �Ķ����.
	m_nCParam[1] = nParam2;
	m_nCParam[2] = nParam3;
}

//
// fRange = ���޹���.  ����Ʈ�� 0.0f�� ������ �ݰ���� �ٴ°Ŵ�.
void CMover::SetDestObj( OBJID objid, float fRange, BOOL bSend )
{
#ifndef __WORLDSERVER
	if( bSend && IsActiveMover() && m_idDest != objid )
	{
		CMover* pTarget = prj.GetMover( m_idDest );
		if( pTarget )
		{
			if( pTarget->IsPlayer( ) )
				g_DPlay.SendPlayerBehavior();	// ���ο� Obj�� �̵��Ұ�� ����ȭ �ѹ� �����ְ�....
		}
		g_DPlay.SendPlayerDestObj( objid, fRange );
	}
#endif	// not __WORLDSERVER

#ifdef __WORLDSERVER
	// �������� SetDestObj�� �����ϸ� Ŭ��鿡�� �����ִ°� ��� �̰� ����.
	if( bSend )
	{
		if( m_idDest != objid || m_fArrivalRange != fRange )
			g_xWSUserManager->AddMoverSetDestObj( this, objid, fRange, FALSE );
	}
#endif	// __WORLDSERVER

	if( m_idDest != objid )
		m_idDest	    = objid;
	m_fArrivalRange = fRange;
	ClearDestPos();



#ifdef __CLIENT
	m_oaCmd = OBJACT_NONE;		// CMD_ �ø�� ��ġ�� �ʰ� �̰͸� ȣ��Ǵ� ���� Cmd�� ������Ű��.
#endif // Client

//	if( IsPlayer() )
//		FLTRACE_LOG( PROGRAM_NAME, _T( "SetDestObj %08x" ), objid );
}

//
//
void CMover::SetDestPos( const D3DXVECTOR3 & vDestPos, bool bForward, BOOL fTransfer )
{
#ifndef __CLIENT
	UNREFERENCED_PARAMETER( fTransfer );
#endif

	m_vDestPos	= vDestPos;

#ifdef __CLIENT	//mover�� ����ȭ ����
	m_bForward	= true;				//sun
#else //__CLIENT
	m_bForward	= bForward;
#endif //__CLIENT

	D3DXVECTOR3	vDir = GetPos() - m_vDestPos;

	m_bPositiveX = ( vDir.x > 0.0f );
	m_bPositiveZ = ( vDir.z > 0.0f );

	ClearDestObj();

#ifdef __CLIENT
	if( IsActiveMover() && fTransfer ) 
	{
		g_DPlay.PutPlayerDestPos( vDestPos, bForward );
	}
#endif
}

// IA������Ʈ ���� �̵�Ŭ���� ������....
#ifdef __IAOBJ0622
void CMover::SetDestPos( CShip *pIAObj, const D3DXVECTOR3 &vDestPos )
{
	if( NULL == GetIAObjLink() )
		return;

	m_pIADestPos = pIAObj;
	
	D3DXMATRIX mTrans, mInvIA, mLocal;
	D3DXMatrixTranslation( &mTrans, vDestPos.x, vDestPos.y, vDestPos.z );		// ������ǥ�� ��Ʈ������ ��ȯ
	D3DXMatrixInverse( &mInvIA, NULL, pIAObj->GetMatrixWorldPtr() );			// IAObj�� �����.
	D3DXMatrixMultiply( &mLocal, &mTrans, GetIAObjLink()->GetInvTM() );	// IA������Ʈ�κ����� ���� ���.

	D3DXVECTOR3 vLocal;
	vLocal.x = mLocal._41;
	vLocal.y = mLocal._42;
	vLocal.z = mLocal._43;
	
	SetDestPos( vLocal, true, FALSE );	// do not transfer

#ifdef __CLIENT
	if( IsActiveMover() )
	{
		g_DPlay.PutPlayerDestPos( vLocal, true, 0, pIAObj->GetId() );
	}
#endif	// __CLIENT
}
#endif	// __IAOBJ0622

FLOAT	CMover::GetAttackRange( DWORD dwAttackRange )
{
	float fAttRange = 0.0f;
	switch( dwAttackRange )
	{
	case AR_SHORT:	fAttRange = 2.0f;	break;
	case AR_LONG:	fAttRange = 3.0f;		break;
	case AR_FAR:	fAttRange = 4.0f;		break;
	case AR_RANGE:	fAttRange = 10.0f;		break;
	case AR_WAND:	fAttRange = 15.0f;		break;
	case AR_HRANGE: fAttRange = 6.0f;	break;
	case AR_HWAND:	fAttRange = 18.0f;		break;	//sun: 10�� ���½ý���	Neuz, World, Trans
	default:		fAttRange = 0.0f;		break;
	}
//sun: 10�� ���½ý���	Neuz, World, Trans
	int  nTmpATR = GetParam( DST_HAWKEYE_RATE, 0 );
	if( nTmpATR > 0 )
	{
		nTmpATR += 100;
		fAttRange =  fAttRange * nTmpATR / 100;
	}

	return fAttRange;
}

//
// �ܺο��� UseSkill�� ����Ҷ� �̰����� ȣ������.
// sutType : ��ų�� ����Ҷ� ��ųť���� ��Ÿ�� ����Ѱǰ� �Ϲ����� ����� �Ѱǰ�.c
//
int	CMover::CMD_SetUseSkill( OBJID idTarget, int nSkillIdx, SKILLUSETYPE sutType )
{
	m_oaCmd = OBJACT_NONE;
	//FLTRACE_LOG( PROGRAM_NAME, _T( "CMD_SetUseSkill( " ) );
	if( m_pActMover->IsFly() )	return 0;				// �����߿� ��ų��� ����.
	if( m_pActMover->IsActAttack() )	return 0;
	if( m_pActMover->IsActJump() )	return 0;			// �����߿� ������.
	if( m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL )		return 0;	// ������ �ö����߿� ��ų������.
	if( IsDie() )	return 0;		// �׾����� ������.

	LPSKILL pSkill	= GetSkill( 0, nSkillIdx );		// this�� ���� ��ų�� nIdx�� �ش��ϴ� ��ų�� ������.
	if( pSkill == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s skill(%d) not found" ), m_szName, nSkillIdx );
		return 0;	// 
	}

	SkillProp* pSkillProp = pSkill->GetProp();
	if( pSkillProp == NULL )		// JobSkill ����Ʈ���� ���� ��ų�� ������Ƽ�� ����.
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s. skill(%d) property not found" ), m_szName, pSkill->dwSkill );
		return 0;	// 
	}
	
	if( IsPlayer() && IsStateMode( STATE_BASEMOTION_MODE ) )	// ������(�غ�ð�)�϶� ������.
	{
	#ifdef __CLIENT
		g_DPlay.SendStateModeCancel( STATE_BASEMOTION_MODE, STATEMODE_BASEMOTION_CANCEL );
	#endif 
		return 0;		
	}

	// ���޹��� - �󸶳� ������ �����ؾ��ϴ°�. ���ʹ���
	float	fArrivalRange = 0.0f;		
	fArrivalRange = GetAttackRange( pSkillProp->dwAttackRange );
	
	switch( pSkillProp->dwUseChance )
	{
	case WUI_NOW:	// Ÿ���ð� ������� �ڱ��ڽſ��� ���� ���.
		idTarget = GetId();
		break;
	case WUI_TARGETINGOBJ:	// ����Ʈ �Ǿ� �ִ� Ÿ�ٿ��� ���.
		{
#ifdef __CLIENT
		CObj *pFocusObj = GetWorld()->GetObjFocus();
		if( pFocusObj && pFocusObj->GetType() == OT_MOVER )
			idTarget = ((CMover*)pFocusObj)->GetId();
#else
		if( IsPlayer() )
			idTarget = ((FLWSUser *)this)->m_idSetTarget;
#endif // __CLIENT
		}

		break;
#ifdef __CLIENT
	case WUI_TARGETCURSORPTZ:
		{
			idTarget = GetId();
			
			CRect rect;
			D3DXVECTOR3 vPos;
			CWndWorld* pWndWorld;
			
			pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );

			rect = pWndWorld->GetClientRect();

			if( GetWorld()->ClientPointToVector( NULL, rect, pWndWorld->GetMousePoint(), &GetWorld()->m_matProj, &GetWorld()->GetCamera()->m_matView, &vPos, TRUE ) )
			{
#ifdef __SKILL0517
				AddSkillProp* pAddSkillProp = prj.GetAddSkillProp( pSkillProp->dwSubDefine, GetSkillLevel( pSkill ) );		// UseSkill���� ����� ��ų�� ������Ƽ ����
#else	// __SKILL0517
				AddSkillProp* pAddSkillProp = prj.GetAddSkillProp( pSkillProp->dwSubDefine, pSkill->dwLevel );		// UseSkill���� ����� ��ų�� ������Ƽ ����
#endif	// __SKILL0517
				if( pAddSkillProp == NULL )
				{
					FLERROR_LOG( PROGRAM_NAME, _T( "%s. add��ų(%d)�� ������Ƽ�� ����." ), m_szName, nSkillIdx );
					return 0;	// property not found
				}
				
				FLOAT	fDist;
				FLOAT	fMaxDistSq;
				D3DXVECTOR3	vDist;
				fMaxDistSq  = (float)pAddSkillProp->dwSkillRange;
				fMaxDistSq *= fMaxDistSq;
				vDist = vPos - GetPos();
				fDist = D3DXVec3LengthSq( &vDist );	

				SetAngle( GetDegree(vPos, GetPos()) );		// ��ǥ������ ���� ����.
				
				// �ڷ���Ʈ �� ��ġ�� �ְ�� ���� ��ų�� �ش��ϴ� �Ÿ��� �ٲ��ش�
				if( fDist > fMaxDistSq )
				{
					FLOAT fLength;
					D3DXVECTOR3 vDirNor;
					D3DXVec3Normalize( &vDirNor, &vDist );
					fLength = (float)pAddSkillProp->dwSkillRange;
					float y = vPos.y;
					vPos = GetPos() + (vDirNor * fLength);
					vPos.y	= y;

					// ��ų�� �ش��ϴ� �Ÿ��� �ٲ۰��� ������ �����̶�� ���� �ִ� ������ �˻��Ѵ�.
					int nAttr = GetWorld()->GetHeightAttribute( vPos.x, vPos.z );

					if( nAttr != HATTR_NONE ) 
					{
						while( nAttr != HATTR_NONE )
						{
							if( nAttr == HATTR_NOFLY )
								break;
							
							fLength -= 1.0f; // 1���;� �ٿ����� ����Ѵ�.
							vPos  = GetPos() + (vDirNor * fLength);
							nAttr = GetWorld()->GetHeightAttribute( vPos.x, vPos.z );

							// ĳ������ �� �ڷ� �̵��Ұ� �� ��� ������ �̵��Ұ� ���� �ɶ� ���� ��� ����Ͽ� �̵���Ŵ
							// �׷��Ƿ� �ڷ���Ʈ ��ų ������ �Ѿ ��� ���� �ڸ��� �ڷ���Ʈ �ϵ��� ó��
							D3DXVECTOR3 vTemp = vPos - GetPos();
							float fTemp = D3DXVec3LengthSq( &vTemp );
							if(fTemp > fMaxDistSq)
							{
								vPos = GetPos();
								break;
							}
						}
						
						// �ѹ��� �ٿ���
						fLength -= 1.0f; 
						vPos = GetPos() + (vDirNor * fLength);

						// ���� ���� �̵��Ұ� ������ �� �ִ�.
						nAttr = GetWorld()->GetHeightAttribute( vPos.x, vPos.z );
						if( nAttr != HATTR_NONE )
						{
							vPos = GetPos();
						}
					}
				}
				else // �ڷ���Ʈ �� ��ġ�� �ش罺ų �Ÿ����� �������
				{
					int nAttr = GetWorld()->GetHeightAttribute( vPos.x, vPos.z );

					FLOAT fLength;
					D3DXVECTOR3 vDirNor;
					D3DXVec3Normalize( &vDirNor, &vDist );
					fLength = 0.0f;

					while( nAttr != HATTR_NONE )
					{
						if( nAttr == HATTR_NOFLY )
							break;

						fLength -= 1.0f;
						vPos = GetPos() + (vDirNor * fLength);
						
						nAttr = GetWorld()->GetHeightAttribute( vPos.x, vPos.z );

						// ĳ������ �� �ڷ� �̵��Ұ� �� ��� ������ �̵��Ұ� ���� �ɶ� ���� ��� ����Ͽ� �̵���Ŵ
						// �׷��Ƿ� �ڷ���Ʈ ��ų ������ �Ѿ ��� ���� �ڸ��� �ڷ���Ʈ �ϵ��� ó��
						D3DXVECTOR3 vTemp = vPos - GetPos();
						float fTemp = D3DXVec3LengthSq( &vTemp );
						if(fTemp > fMaxDistSq)
						{
							vPos = GetPos();
							break;
						}
					}
				}

				if( IsActiveMover() && g_eLocal.GetState( EVE_SCHOOL ) )	// �б��̺����̸�.
				{
					D3DXVECTOR3 v1, v2;
					v1 = GetPos();	v1.y += 0.1f;
					v2 = vPos;		v2.y += 0.1f;
					if( GetWorld()->IntersectObjLine( NULL, v1, v2, FALSE, FALSE ) )	// �ڷ���Ʈ�� ����üũ��.
					{
						g_WndMng.PutString( prj.GetText(TID_GAME_NOMOVING), NULL, prj.GetTextColor(TID_GAME_NOMOVING) );
						g_WndMng.m_pWndWorld->SetNextSkill( NEXTSKILL_NONE );
						return 0;
					}
				}

				pWndWorld->m_vTelePos = vPos;
			}
			else
			{
				g_WndMng.m_pWndWorld->SetNextSkill( NEXTSKILL_NONE );
				g_WndMng.PutString( prj.GetText(TID_GAME_NOMOVING), NULL, prj.GetTextColor(TID_GAME_NOMOVING) );
				return 0;
			}
		}
		break;
#endif // __CLIENT
	}

	// Ÿ�ο��� ���°�쿡�� �˻�...
	if( idTarget != GetId() )		
	{
		CMover *pTarget = prj.GetMover( idTarget );
		if( IsValidObj(pTarget) )
		{
			if( pSkillProp->IsDebuff() == TRUE )	// ���� ��ų�� 
				if( IsAttackAble(pTarget) == FALSE )	// ��������� ���� ������ ����� �� ����.
					return 0;
				
			if( pSkill->dwSkill == SKILL_INDEX( 45, SI_ASS_HEAL_RESURRECTION ) )	// ��Ȱ�� ���������
			{
				if( pTarget->IsNPC() || pTarget->IsDie() == FALSE )		// ��밡 NPC�ų� ��밡 �׾����� �ʴٸ� ���
					return 0;
			} else
			{
				if( pTarget->IsDie() )		// ��Ȱ�� �ƴ� ��ų�� ��������� ��밡 �׾������� ���ȵ�.
					return 0;
			}
		}
	}

	// Ÿ�� ���� ���.
	bool bSetDestObj = false;
	switch( pSkillProp->dwExeTarget )
	{
	case EXT_SELFCHGPARAMET:	// ������ �ڽſ��� ����ϴ� ����
		idTarget = GetId();		// Ÿ���� �ڱ��ڽ����� ����.
		//SetDestObj( idTarget, fArrivalRange, TRUE );		// �̵��� ��ǥ���� idTarget���� ����. << �̰� �� �ʿ�����? ��.��?
		bSetDestObj = true;
		break;
	case EXT_MAGICATKSHOT:
	case EXT_MAGICATK:			// ���Ÿ����� �������� Ÿ���� ����
	case EXT_MAGICSHOT:
		if( idTarget == NULL_ID )	return 0;			// Ÿ���� ���ų� ��ȿ���� ������ ����.
		//SetDestObj( idTarget, fArrivalRange, TRUE );		// �̵��� ��ǥ���� idTarget���� ����. �ٰ����� ������ fArrivalRange������..
		bSetDestObj = true;
		break;
	case EXT_ANOTHERWITH:		// �� Ȥ�� �ٸ�������� ����
		if( idTarget == NULL_ID )	// Ÿ���� �������� ������
			idTarget = GetId();		// �ڽ��� Ÿ������ ����
		//SetDestObj( idTarget, fArrivalRange, TRUE );		// �̵��� ��ǥ���� idTarget���� ����.
		bSetDestObj = true;
		break;
	case EXT_AROUNDATK:			// �� ���������� ���.
		idTarget = GetId();		// Ÿ���� �ڱ��ڽ����� ����.
		//SetDestObj( idTarget, fArrivalRange, TRUE );		// �̵��� ��ǥ���� idTarget���� ����.
		bSetDestObj = true;
		break;
	case EXT_OBJCHGPARAMET:		// Ÿ�ο��� ���
	default:	// �׿ܴ� ��� ��������.
		if( idTarget == NULL_ID )	return 0;			// Ÿ���� ���ų� ��ȿ���� ������ ����.
		//SetDestObj( idTarget, fArrivalRange, TRUE );		// �̵��� ��ǥ���� idTarget���� ����.
		bSetDestObj = true;
		break;
	}

	if( bSetDestObj ) {
#ifdef __WORLDSERVER
		SetDestObj( idTarget, fArrivalRange, FALSE );		// �̵��� ��ǥ���� idTarget���� ����.
		g_xWSUserManager->AddMoverSetDestObj( this, idTarget, fArrivalRange, TRUE );
#else // __WORLDSERVER
		SetDestObj( idTarget, fArrivalRange, TRUE );		// �̵��� ��ǥ���� idTarget���� ����.
#endif // __WORLDSERVER
	}
	
	ClearActParam();
	SetCmd( OBJACT_USESKILL, nSkillIdx, idTarget, sutType );		// �����Ÿ��� �Ǿ����� ������ ��� ����.
	FLTRACE_LOG( PROGRAM_NAME, _T( ") CMD_SetUseSkill" ) );

	return 1;
}

class CWndWorld;

void	CMover::CMD_SetMeleeAttack( OBJID idTarget, FLOAT fRange )
{
	PT_ITEM_SPEC pProp = GetActiveHandItemProp();
	if( pProp->dwItemKind2 == IK2_TOOLS )			// ���� ��� ���ݸ��Ѵ�,.
		return;

	if( m_pActMover->IsActAttack() )	
		return;
	if( IsDie() )	
		return;										// �׾����� ������.

	// ���� �и������̱� �ϳ� �ణ �Ÿ��� �ΰ� �����Ѵ�.( ������Ƽ ������ )
	if( pProp->dwItemKind3 == IK3_YOYO )
		fRange = GetAttackRange( pProp->dwAttackRange );
	
#ifdef __CLIENT
	ClearActParam();
	SetDestObj( idTarget, fRange );		// �̵��� ��ǥ���� idTarget���� ����.
	SetCmd( OBJACT_MELEE_ATTACK, idTarget );		// �����Ÿ��� �Ǿ����� ������ ��� ����.
#elif defined(__WORLDSERVER)
	if( GetDestId() == idTarget )	// �ߺ� ��Ŷ ���� ����
		return;

	SetDestObj( idTarget, fRange, TRUE );		// �̵��� ��ǥ���� idTarget���� ����.
//	g_xWSUserManager->AddMoverSetDestObj( this, idTarget, fRange );
#endif // WORLDSERVER
}

/*
void	CMover::CMD_SetCollect( CObj *pObj )
{
	if( IsInvalidObj( pObj ) )	return;
	if( pObj->GetType() != OT_MOVER )	return;
	if( m_pActMover->IsActAttack() )	return;
	if( m_pActMover->IsActJump() )		return;
	if( m_pActMover->IsActDamage() )		return;
	if( IsDie() )	return;		// �׾����� ������.
	
	CMover *pTarget = (CMover *)pObj;
	ClearActParam();
	SetDestObj( pTarget->GetId(), 3.0f );		// �̵��� ��ǥ���� idTarget���� ����.
	SetCmd( OBJACT_COLLECT, pTarget->GetId() );		// �����Ÿ��� �Ǿ����� ������ ��� ����.
}
*/
//
//
//
void	CMover::CMD_SetRangeAttack( OBJID idTarget, int nPower )
{
	FLTRACE_LOG( PROGRAM_NAME, _T( "CMD_SetRangeAttack(%d, %d)" ), idTarget, nPower );

	if( m_pActMover->IsActAttack() )		// �̹� �������̸� ���.
		return;

	if( IsDie() )	
		return;								// �׾����� ������.
	
	PT_ITEM_SPEC pItemProp = GetActiveHandItemProp();
	if( pItemProp == NULL )		
		return;
	
	if( pItemProp->dwItemKind3 != IK3_BOW )	// ����ִ� ���Ⱑ ���찡 �ƴϸ� ����
#ifdef __ADDWEAPON_CROSSBOW16
		if( pItemProp->dwItemKind3 != IK3_CROSSBOW )	// ũ�μ� ���쵵 �ƴϸ� ���� 
#endif //__ADDWEAPON_CROSSBOW16
		return;
	
	if( IsBullet( pItemProp ) == FALSE )
		return;
	
	ClearActParam();
	SetDestObj( idTarget, GetAttackRange( pItemProp->dwAttackRange ), TRUE );		// �̵��� ��ǥ��(idTarget)�� �� �Ÿ��� ���� 
	SetCmd( OBJACT_RANGE_ATTACK, idTarget, nPower, 0 );		// �����Ÿ��� �Ǿ����� ������ ��� ����.
}

void	CMover::CMD_SetMagicAttack( OBJID idTarget, int nMagicPower )
{
	FLTRACE_LOG( PROGRAM_NAME, _T( "CMD_SetMagicAttack(%d, %d)" ), idTarget, nMagicPower );

	if( m_pActMover->IsActAttack() )		// �̹� �������̸� ���.
		return;
	if( IsDie() )	return;		// �׾����� ������.
	
	// �տ� ��� �ִ� �ϵ��� ������Ƽ ����
	// �����Ÿ� �����
	PT_ITEM_SPEC pItemProp = GetActiveHandItemProp();
	if( pItemProp == NULL )		return;

	if( pItemProp->dwItemKind3 != IK3_WAND /*&& pItemProp->dwItemKind3 != IK3_STAFF*/ )	//Staff->ata2k	// ����ִ� ���Ⱑ �ϵ尡 �ƴϸ� ���
		return;
	
	// ���޹��� - �󸶳� ������ �����ؾ��ϴ°�. ���ʹ���
	float	fArrivalRange = GetAttackRange( AR_WAND );
	
	ClearActParam();
	SetDestObj( idTarget, fArrivalRange );		// �̵��� ��ǥ���� idTarget���� ����.
	SetCmd( OBJACT_MAGIC_ATTACK, idTarget, nMagicPower );		// �����Ÿ��� �Ǿ����� ������ ��� ����.
}

// vPos��ġ�� �̵����
void	CMover::CMD_SetMoveToPos( const D3DXVECTOR3 &vPos )
{
	SetDestPos( vPos );
}

// idObj��ġ�� �̵�
void	CMover::CMD_SetMoveToObj( OBJID idObj )
{
	SetDestObj( idObj );
}

void	CMover::CMD_SetUseItem( CCtrl *pCtrl )
{
	if( m_pActMover->IsFly() ) {
		return;
	}
	
	ClearActParam();
	SetDestObj( pCtrl );
	SetCmd( OBJACT_USEITEM );		// �����Ÿ��� �Ǿ����� ������ ��� ����.
}



/*--------------------------------------------------------------------------------------------------------


											Mover act parameter
											

----------------------------------------------------------------------------------------------------------*/

void	CMover::SetActParam( OBJACT act, int nParam1, int nParam2, int nParam3, int nParam4, int nParam5 )
{
	m_oaAct = act;				
	m_nAParam[0] = nParam1;		// ��� �Ķ����.
	m_nAParam[1] = nParam2;
	m_nAParam[2] = nParam3;
	m_nAParam[3] = nParam4;		// ��ų�� �ƴѰ�� ���� ����( dwAtkFlag(AF_GENERIC, AF_MAGIC, AF_MISS, ... ) : CActionMover::m_dwAtkFlags ��ü
	m_nAParam[4] = nParam5;
}


