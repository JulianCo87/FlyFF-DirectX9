#include "stdafx.h"
#include "Ship.h"
#include "ModelObject.h"
#include "CreateObj.h"
//#include "ModelGlobal.h"


#ifdef __CLIENT
	#include "AppDefine.h"
	#include "defineSound.h"
	#include "Sfx.h"
	#include "../Neuz/DPClient.h"
	#include "..\_Common\ParticleMng.h"
#endif	// __CLIENT
#ifdef	__WORLDSERVER
	#include "../WorldServer/User.h"
	#include "WorldMng.h"
	#include "..\_AIInterface\AIInterface.h"
	#include "Party.h"
#endif	// __WORLDSERVER

#ifdef __EVENT_MONSTER
	#include "EventMonster.h"
#endif // __EVENT_MONSTER

#include "FLSkillSystem.h"


#ifdef __CLIENT
	extern	CDPClient			g_DPlay;
#else	// __CLIENT
	extern	CWorldMng			g_WorldMng;
	extern	CPartyMng			g_PartyMng;
#endif	// not __CLIENT

BOOL IsLootSendText( PT_ITEM_SPEC pItemProp )
{
	if( pItemProp == NULL )
		return FALSE;

	if( pItemProp->dwItemKind1 == IK1_GOLD    ||
		pItemProp->dwItemKind1 == IK1_GENERAL ||
		pItemProp->dwItemKind1 == IK1_SYSTEM  ||
		pItemProp->dwItemKind1 == IK1_RIDE        )
		return FALSE;

	return TRUE;
}

/*--------------------------------------------------------------------------------------------------------


											Action Event Handler
										   ----------------------
						CActionMover������ ���ۿ� ���õ� �̺�Ʈ�� �߻��ϸ� ȣ��Ǵ� �Լ���


----------------------------------------------------------------------------------------------------------*/


// RT_TIME�� ���� ��ų���� ��ų �ð��� ���.
void	CMover::SubReferTime( DWORD *pTime1, DWORD *pTime2, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp )
{
	DWORD dwTime1 = 0, dwTime2 = 0;

	if( pSkillProp->dwReferTarget1 == RT_TIME )
	{
		switch( pSkillProp->dwReferStat1 )
		{
		case DST_STA:	dwTime1 = GetSta();		break;
		case DST_DEX:	dwTime1 = GetDex();		break;
		case DST_INT:	dwTime1 = GetInt();		break;
		}
		
		dwTime1 = (DWORD)( ((pSkillProp->dwReferValue1/10.0f)*dwTime1) + (pAddSkillProp->dwSkillLvl * FLOAT(dwTime1/50.0f)) );
	}
	
	if( pSkillProp->dwReferTarget2 == RT_TIME )
	{
		switch( pSkillProp->dwReferStat2 )
		{
		case DST_STA:	dwTime2 = GetSta();		break;
		case DST_DEX:	dwTime2 = GetDex();		break;
		case DST_INT:	dwTime2 = GetInt();		break;
		}
		
		dwTime2 = (DWORD)( ((pSkillProp->dwReferValue2/10.0f)*dwTime2) + (pAddSkillProp->dwSkillLvl * FLOAT(dwTime2/50.0f)) );
	}

	*pTime1 = dwTime1;
	*pTime2 = dwTime2;
}

BOOL	CMover::GetSkillProp( SkillProp **ppSkillProp, AddSkillProp **ppAddSkillProp, int nSkill, DWORD dwLevel, LPCTSTR szErr )
{
	char szStr[256] = {0, };
	FLSPrintf( szStr, _countof( szStr ), "%s-%s", m_szName, szErr );

	return CCtrl::GetSkillProp( ppSkillProp, ppAddSkillProp, nSkill, dwLevel, szStr );
}


// ��Ÿ�� ����!
//sun: 9, ���ο� AddSkillProp �÷� �߰� �� ����
void	CMover::SetCoolTime( AddSkillProp* pAddSkillProp, LPCTSTR /*szCall*/ )
{
	int nIdx	= GetSkillIdx( pAddSkillProp->dwSkillPropID );		// ��ų����Ʈ �ε����� ã��.
	
	if( nIdx < 0 || nIdx >= MAX_SKILL_JOB )
		FLERROR_LOG( PROGRAM_NAME, _T( "szCall SetCoolTime : %d %d��ų�� ã�� �� ���� %s" ), nIdx, pAddSkillProp->dwSkillPropID, GetName() );
	else
		m_tmReUseDelay[ nIdx ] = pAddSkillProp->dwCooldown + timeGetTime();		// 1/1000����
}


//
//	��Ÿ����Ⱑ �߻�Ǵ� ������ �߻��ϴ� �̺�Ʈ
//
BOOL	CMover::OnAttackRange()
{
//  	if( GetAct() != OBJACT_RANGE_ATTACK ) {
//  		return FALSE;
//  	}

	int		idTarget = GetActParam( 0 );
	DWORD	dwItemID = GetActParam( 1 );		// ����� ������ ���̵� (player�̸� ��������)
	int		idSfxHit = GetActParam( 2 );		// ���Ͱ� ����Ѱ��̸� ���⿡ �����κ��� �������� ����ִ�.
	UNREFERENCED_PARAMETER( idSfxHit );

	BOOL	bControl = FALSE;					//sun: 8, // __S8_PK

	CMover* pHit = prj.GetMover( idTarget );	// Ÿ���� �����͸� ��.
	if( IsInvalidObj(pHit) )	
		return FALSE;							// Ŭ��������ô� �־����� �߻�Ǳ����� �����Ǿ���.
	
	PT_ITEM_SPEC pItemProp = NULL;
	if( IsPlayer() )
		pItemProp = GetActiveHandItemProp();		// �̷����� �� �տ� �� ����(Ȥ�� �⺻������)
	else 
	{
		if( dwItemID == NULL_ID || dwItemID == 0 )	
			pItemProp = GetActiveHandItemProp();		// �̷����� �� �տ� �� ����(Ȥ�� �⺻������)
		else
			pItemProp = g_xSpecManager->GetSpecItem( dwItemID );	// ���� �������� ������ ���� �װɷ� ������(���Ͱ� �ַ� �����)
	}

	if( pItemProp == NULL )
		return FALSE;

#ifdef __WORLDSERVER
//sun: 12, // __MONSTER_SKILL
	if( IsNPC() )
		CMonsterSkill::GetInstance()->ApplySkill( this, pHit, ATK_RANGE );
#endif // __WORLDSERVER

	DWORD dwSfxObj = pItemProp->dwSfxObj2;
	
	D3DXVECTOR3 vPosDest = pHit->GetPos() + D3DXVECTOR3( 0.0f, 1.0f, 0.0f ); // �߻� ������ ���Ƿ� �÷��ش�. ���� 
	D3DXVECTOR3 vPosSrc  = GetPos() + D3DXVECTOR3( 0.0f, 1.0f, 0.0f ); // �߻� ������ ���Ƿ� �÷��ش�. ���� 

#ifdef __CLIENT		
	CSfx* pSfx = NULL;
#endif
	// ��ũ�ι��� ����Ʈ ����� �׻� �޼տ�...
	if( IsPlayer() )		
	{
		PT_ITEM_SPEC pItemElem = GetActiveHandItemProp( PARTS_BULLET );  // ȭ��������� ������ ȭ������Ʈ ����
		if( pItemElem && pItemElem->dwSfxObj2 != NULL_ID )
		{
		#ifdef __CLIENT
			if( IsActiveMover() )
				pSfx = CreateSfxArrow( pItemElem->dwSfxObj2, pItemProp->dwSfxObj3, vPosDest, idTarget );
		#endif //__CLIENT

		#ifdef __WORLDSERVER
			g_xWSUserManager->AddCreateSfxAllow( this, pItemElem->dwSfxObj2, pItemProp->dwSfxObj3, vPosDest, idTarget );
		#endif //__WORLDSERVER
		}
	}
	else
	{
		if( dwSfxObj == NULL_ID )	
			return FALSE;
	#ifdef __CLIENT		
		pSfx = CreateSfx( D3DDEVICE, dwSfxObj, vPosSrc,GetId(), vPosDest ,idTarget );
	#endif
	}

	DWORD dwAtkFlags = GetAct() == OBJACT_RANGE_ATTACK ? GetActParam( 3 ) : AF_MISS;
	if( IsPlayer() )
		dwAtkFlags |= AF_RANGE;

#ifdef __WORLDSERVER
	OnAttackSFX( pHit->GetId(), ( IsPlayer()? dwItemID: 0 ), NULL_ID, 0, 0, 0, dwAtkFlags, bControl );

#endif	// __WORLDSERVER

#ifdef __CLIENT
	if( pSfx )
	{
		if( IsActiveMover() )		
		{
			idSfxHit = ++m_idSfxHit; 	// Ŭ�󿡼� �߻�ü�� ������ ��� ID�� ����� �װ��� ������ ����.
			g_DPlay.SendSfxID( idTarget, idSfxHit, dwAtkFlags );
			pSfx->m_idSfxHit	= idSfxHit;
			pSfx->m_nMagicPower = IsPlayer() ? dwItemID : 0;		// ���������� �־��ش�.
		}
		// ����� g_pPlayer�ų� / ����� ���̰� �´³��� g_pPlayer��
		if( IsActiveMover() || (IsNPC() && pHit->IsActiveMover()) )
			pSfx->m_idSfxHit	= idSfxHit;		// id�� �����
	}
#endif //__CLIENT
	return TRUE;
}


//
//	�ϵ�(����)������ �߻�Ǵ� ������ �߻��ϴ� �̺�Ʈ.
//	
BOOL	CMover::OnAttackMagic()
{
//  	if( GetAct() != OBJACT_MAGIC_ATTACK ) {
//  		return FALSE;
//  	}

	int		idTarget = GetActParam( 0 );
	int		nMagicPower = GetActParam( 1 );

	BOOL	bControl = GetActParam( 4 );		//sun: 8, // __S8_PK

	CMover* pHit = prj.GetMover( idTarget );	// Ÿ���� �����͸� ��.

	if( IsInvalidObj(pHit) )	
		return FALSE;		// Ŭ��������ô� �־����� �߻�Ǳ����� �����Ǿ���.

#ifdef __WORLDSERVER
	const DWORD attackFlag = GetAct() == OBJACT_MAGIC_ATTACK ? GetActParam( 3 ) : AF_MISS;
	OnAttackSFX( pHit->GetId(), nMagicPower, NULL_ID, 0, 0, 0, attackFlag, bControl );

#else	// __WORLDSERVER
	DWORD dwWeaponId;

	if( IsActiveMover() ) 
	{
		FLItemElem* pWeapon = GetWeaponItem();
		if( pWeapon == NULL )	return FALSE;
		dwWeaponId	= pWeapon->m_dwItemId;
	}
	else 
	{
		dwWeaponId	= m_aEquipInfo[PARTS_RWEAPON].dwItemID;
		if( dwWeaponId == 0 )	return FALSE;
	}

	D3DXVECTOR3 vPosDest = pHit->GetPos(); 
	// �߻� ��ǥ������ ����� �߾�.
	float fHeight = fabs(pHit->m_pModel->m_vMax.y) / 2.0f;	// ���̹�Ʈ�������� ���߿� ���ֱⶫ�� ���� ������ Max���� ���ߴ�.
	fHeight *= pHit->GetScale().x;
	vPosDest.y += fHeight;	

	D3DXVECTOR3 vPosSrc = GetPos() + D3DXVECTOR3( 0.0f, 1.0f, 0.0f ); // �߻� ������ ���Ƿ� �÷��ش�. ���� 

	PT_ITEM_SPEC pHandItemProp = GetActiveHandItemProp( PARTS_RWEAPON );
	CSfx* pSfx = NULL;
	if( m_pActMover->IsFly() )
	{
		pSfx = CreateSfx( D3DDEVICE, XI_INDEX( 28, XI_FIR_WAND02 ), vPosSrc, GetId(), vPosDest, idTarget );
		if( pSfx )
			pSfx->ShootSfx( GetAngle(), -GetAngleX(), 0.75f );		 
	} 
	else
	{
		DWORD dwSfxObj2 = XI_INDEX( 27, XI_FIR_WAND01 );
		if( pHandItemProp )
			dwSfxObj2 = pHandItemProp->dwSfxObj2;

		pSfx = CreateSfx( D3DDEVICE, dwSfxObj2, vPosSrc,GetId(), vPosDest ,idTarget );
		if( pSfx )
		{
			switch(nMagicPower) 
			{
			case 1:		pSfx->SetScale( D3DXVECTOR3( 1.2f, 1.2f, 1.2f ) );	break;
			case 2:		pSfx->SetScale( D3DXVECTOR3( 1.7f, 1.7f, 1.7f ) );	break;
			case 3:
			case 4:		pSfx->SetScale( D3DXVECTOR3( 2.5f, 2.5f, 2.5f ) );	break;
			}
			((CSfxItemWandAtk1 *)pSfx)->m_fCenter = fHeight;		
		}
	}

	if( pSfx )
	{
		if( IsActiveMover() )		// g_pPlayer�� �Ʒ����� �ϰ� �Ѵ�.
		{
			int idSfxHit = ++m_idSfxHit;			// Ŭ�󿡼� �߻�ü�� ������ ��� ID�� ����� �װ��� ������ ����.
			g_DPlay.SendSfxID( idTarget, idSfxHit, AF_MAGIC );
			pSfx->m_idSfxHit	= idSfxHit;
			pSfx->m_nMagicPower = nMagicPower;		// ���������� �־��ش�.
		}
	}


	CModelObject* pModel = (CModelObject*)m_pModel;
	if( pModel )
	{
		pModel->m_SparkInfo.m_bUsed  = TRUE;
		pModel->m_SparkInfo.m_v3Color.x = 2.0f;
		pModel->m_SparkInfo.m_v3Color.y = 2.0f;
		pModel->m_SparkInfo.m_v3Color.z = 2.0f;
		pModel->m_SparkInfo.m_nCnt = 0;
		pModel->m_SparkInfo.m_fLerp= 1.0f;
		
		AngleToVectorXZ( &pModel->m_SparkInfo.m_v3SparkDir, GetAngle(), -1.0f );
	}
#endif // __WORLDSERVER
	return TRUE;
}

void	CMover::OnAttackMelee_ClockWorks( DWORD dwState, CMover *pHitObj )
{
	int		nItemID = GetActParam( 1 );	// ����� ������ ���̵�

#ifdef __WORLDSERVER
	UpdateMatrix();		// ��ü�� Ư���������� ���� �������� ó���ϱ� ���ؼ� ���������� ��Ʈ������ �����ؾ��Ѵ�.
	CModelObject *pModel = (CModelObject *)m_pModel;
	pModel->UpdateMatrixBone();		// ���������� ���ִϸ��̼��� ���� �����Ƿ� GetEventPos()�� ����� ���� ������ ������Ʈ�� ����� �Ѵ�.
#endif // WorldServer
	
	switch( dwState )
	{
	case OBJSTA_ATK1:		// Į�� ����ġ��
		{
			D3DXVECTOR3 vPos;
			CModelObject *pModel = (CModelObject *)m_pModel;
			pModel->GetEventPos( &vPos, 0 );
			D3DXVec3TransformCoord( &vPos, &vPos, GetMatrixWorldPtr() );

			OnDamage( nItemID, dwState, NULL, &vPos );		// ����Ʈ�� �߻��ϴ� ��ġ�� �������� �ְ�
#ifdef __CLIENT
			GetWorld()->m_pCamera->SetQuake( 15, 0.5f );
			CSfx *pSfx = CreateSfx( D3DDEVICE, XI_INDEX( 1615, XI_NPCSP1DIRFIRESP ), vPos );	// ����Ʈ�� �߻� - �������� ���� �ȵ�.
			if( pSfx )
				pSfx->SetAngle( -GetAngle() );
#endif 
		}
		break;
	case OBJSTA_ATK2:		// �����
		{
			D3DXVECTOR3 vPos;
			CModelObject *pModel = (CModelObject *)m_pModel;
			pModel->GetEventPos( &vPos, 1 );	// �޼� ���Թ� ��ġ.
			D3DXVec3TransformCoord( &vPos, &vPos, GetMatrixWorldPtr() );
#ifdef __CLIENT
			CSfx *pSfx = CreateSfx( D3DDEVICE, XI_INDEX( 1613, XI_NPCSP1DIRCANNON ), vPos, GetId() );	// �߻�����Ʈ.
			if( pSfx )
			{
				pSfx->SetAngle( -GetAngle() );
				pSfx->SetScale( D3DXVECTOR3(4.0f, 4.0f, 4.0f) );
			}
#endif
			CMover *pTarget = pHitObj;
			if( IsValidObj(pTarget) )
			{
				OnDamage( nItemID, dwState, NULL, &pTarget->GetPos() );		// Ÿ���� ��ǥ�� �߽����� �������� ��.
#ifdef __CLIENT
				pSfx = CreateSfx( D3DDEVICE, XI_INDEX( 1616, XI_NPCSP1RANBALL ), vPos, GetId(), pTarget->GetPos(), pTarget->GetId(), -1 );		// ��ź
				if( pSfx )
				{
					pSfx->SetScale( D3DXVECTOR3(4.0f, 4.0f, 4.0f) );
					{
						FLOAT fAngXZ, fAngH;
						D3DXVECTOR3 vDist = pTarget->GetPos() - vPos;
						xGetDegree( &fAngXZ, &fAngH, vDist );		// ��ǥ���� ��������.
						((CSfxAtkStraight *)pSfx)->ShootSfx( fAngXZ, -fAngH, 2.0f, XI_INDEX( 1617, XI_NPCSP1RANBOOM ) );		// ���ư��� ���� ����.
					}
				}
#endif
			}
		}
		break;
	case OBJSTA_ATK3:
#ifdef __CLIENT
		GetWorld()->m_pCamera->SetQuake( 30, 0.7f );
#endif
		break;
	case OBJSTA_ATK4:	// �߷� ���.
		{
			D3DXVECTOR3 vPos;

			vPos = GetPos();
			DWORD dwFlag = AF_GENERIC;
			if( xRandom(100) < 15 )
				dwFlag |= AF_FLYING;
			OnDamage( nItemID, dwState, NULL, &vPos, dwFlag );		// �������� ���߽��� �������� �Ѵ�.

			CModelObject *pModel = (CModelObject *)m_pModel;
			pModel->GetEventPos( &vPos, 3 );
			D3DXVec3TransformCoord( &vPos, &vPos, GetMatrixWorldPtr() );

#ifdef __CLIENT
			GetWorld()->m_pCamera->SetQuake( 60, 1.5f );

			CSfx *pSfx = CreateSfx( D3DDEVICE, XI_INDEX( 1614, XI_NPCSP1DIRCIRCLE ), vPos );
			if( pSfx )
				pSfx->SetScale( D3DXVECTOR3(2.0f, 2.0f, 2.0f) );
			pModel->GetEventPos( &vPos, 4 );
			D3DXVec3TransformCoord( &vPos, &vPos, GetMatrixWorldPtr() );

			pSfx = CreateSfx( D3DDEVICE, XI_INDEX( 1614, XI_NPCSP1DIRCIRCLE ), vPos );
			if( pSfx )
				pSfx->SetScale( D3DXVECTOR3(2.0f, 2.0f, 2.0f) );
#endif
		}
		break;
	}
}

void	CMover::OnAttackMelee_BigMuscle( DWORD dwState, CMover *pHitObj )
{
	int		nItemID = GetActParam( 1 );	// ����� ������ ���̵�
	CModelObject *pModel = (CModelObject *)m_pModel;
	
#ifdef __WORLDSERVER
	UpdateMatrix();		// ��ü�� Ư���������� ���� �������� ó���ϱ� ���ؼ� ���������� ��Ʈ������ �����ؾ��Ѵ�.
	pModel->UpdateMatrixBone();		// ���������� ���ִϸ��̼��� ���� �����Ƿ� GetEventPos()�� ����� ���� ������ ������Ʈ�� ����� �Ѵ�.
#endif // WorldServer
	
	switch( dwState )
	{
	case OBJSTA_ATK1:	
	case OBJSTA_ATK2:
		{
			if( IsValidObj(pHitObj) )
			{
				OnDamage( nItemID, dwState, pHitObj );		
			}
		}
		break;
	case OBJSTA_ATK3:		// ��ġ�� ��ų - �μ�
		{
			D3DXMATRIX mMatrix;
			
			const D3DXMATRIX mWorld1 = GetMatrixWorld();
			D3DXMatrixMultiply( &mMatrix, (pModel->GetMatrixBone(12)), &mWorld1/*&(GetMatrixWorld())*/ );
			
			D3DXVECTOR3 v2;
			v2.x = mMatrix._41;
			v2.y = mMatrix._42;
			v2.z = mMatrix._43;
			v2.y -= 5.0f;
			
		#ifdef __CLIENT
			// ��ġ�� ������ ����Ʈ �޼�
			CSfx *pSfx = CreateSfx( D3DDEVICE, XI_INDEX( 1743, XI_SKILL_CIRCLE_DUST ), v2 );			
			if( pSfx )
				pSfx->SetScale( D3DXVECTOR3( 12.0f, 12.0f, 12.0f ) );

			PLAYSND( SND_PC_SKILLD_HITOFPENYA );//, &v2 );
		#endif //__CLIENT 			
			
			const D3DXMATRIX mWorld2 = GetMatrixWorld();
			D3DXMatrixMultiply( &mMatrix, (pModel->GetMatrixBone(16)), &mWorld2/*&(GetMatrixWorld())*/ );
			D3DXVECTOR3 v3;
			v3.x = mMatrix._41;
			v3.y = mMatrix._42;
			v3.z = mMatrix._43;						
			v3.y -= 5.0f;
			
		#ifdef __CLIENT
			// ��ġ�� ������ ����Ʈ ������
			pSfx = CreateSfx( D3DDEVICE, XI_INDEX( 1743, XI_INDEX( 1743, XI_SKILL_CIRCLE_DUST ) ), v3 );			
			if( pSfx )
				pSfx->SetScale( D3DXVECTOR3( 12.0f, 12.0f, 12.0f ) );
		#endif //__CLIENT 			
		
			if( m_pActMover->m_nMotionHitCount == 1 )
			{				
	#ifdef __WORLDSERVER
				g_xApplyItemEffect->SendDamageAroundPos( &GetPos(), (AF_GENERIC | AF_CRITICAL) , this, OBJTYPE_PLAYER, ITEM_INDEX( 9262, II_WEA_MOB_MONSTER4_ATK2 ), 16.0f );					

				SkillProp *pSkillProp;
				AddSkillProp *pAddSkillProp;
				BOOL bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SKILL_INDEX( 228, SI_GEN_BLEEDING ), 10, "OnAttackMelee_BigMuscle" );
				if( bSuccess )
					g_cSkillSystem->ApplySkillAround( this, this, OBJTYPE_PLAYER, pSkillProp, pAddSkillProp , false );							
	#endif //__WORLDSERVER
				
			}
			else
			{
	#ifdef __CLIENT
				PLAYSND( SND_PC_BOSS_STUN );
	#endif //__CLIENT
	#ifdef __WORLDSERVER				
				SkillProp *pSkillProp;
				AddSkillProp *pAddSkillProp;
				BOOL bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SKILL_INDEX( 300, SI_BIGMUSCLE_STUNGROUP ), 1, "OnAttackMelee_BigMuscle" );
				if( bSuccess )
					g_cSkillSystem->ApplySkillAround( this, this, OBJTYPE_PLAYER, pSkillProp, pAddSkillProp , false );							
	#endif //__WORLDSERVER
			}
		}
		break;
	case OBJSTA_ATK4:   // ��ġ�� ��ų - �Ѽ�
		{
			static D3DXVECTOR3 v3OldPos;

			if( m_pActMover->m_nMotionHitCount == 0 )
			{
				v3OldPos = pHitObj->GetPos();			
			}
			else
			if( m_pActMover->m_nMotionHitCount == 1 )
			{
				D3DXMATRIX mMatrix;
				const D3DXMATRIX mWorld = GetMatrixWorld();
				
				D3DXMatrixMultiply( &mMatrix, (pModel->GetMatrixBone(12)), &mWorld/*&(GetMatrixWorld())*/ );
				
				D3DXVECTOR3 v2;
				v2.x = mMatrix._41;
				v2.y = mMatrix._42;
				v2.z = mMatrix._43;
				v2.y -= 5.0f;
				
			#ifdef __CLIENT
				// ��ġ�� ������ ����Ʈ �޼�
				CSfx *pSfx = CreateSfx( D3DDEVICE, XI_INDEX( 1743, XI_SKILL_CIRCLE_DUST ), v2 );
				if( pSfx )
					pSfx->SetScale( D3DXVECTOR3( 9.0f, 9.0f, 9.0f ) );
				
				pSfx = CreateSfx( D3DDEVICE, XI_INDEX( 246, XI_SKILL_MAG_EARTH_ROCKCRASH01 ), v3OldPos );
				if( pSfx )
					pSfx->SetScale( D3DXVECTOR3( 2.0f, 2.0f, 2.0f ) );
				
				PLAYSND( SND_PC_SKILLD_HITOFPENYA );
			#endif //__CLIENT

			#ifdef __WORLDSERVER
				SkillProp *pSkillProp;
				AddSkillProp *pAddSkillProp;
				BOOL bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SKILL_INDEX( 228, SI_GEN_BLEEDING ), 10, "OnAttackMelee_BigMuscle" );
				if( bSuccess )
					g_cSkillSystem->DoApplySkill( this, pHitObj, pSkillProp, pAddSkillProp );	
				
				g_xApplyItemEffect->SendDamageAroundPos( &(v3OldPos), AF_GENERIC | AF_CRITICAL , this, OBJTYPE_PLAYER, ITEM_INDEX( 9262, II_WEA_MOB_MONSTER4_ATK2 ), 4.0f );	
			#endif //__WORLDSERVER
				
					
			}
		}
		
		break;
	}
}

void	CMover::OnAttackMelee_Krrr( DWORD dwState, CMover *pHitObj )
{
	int		nItemID = GetActParam( 1 );	// ����� ������ ���̵�

	CModelObject *pModel = (CModelObject *)m_pModel;
	
#ifdef __WORLDSERVER
	UpdateMatrix();		// ��ü�� Ư���������� ���� �������� ó���ϱ� ���ؼ� ���������� ��Ʈ������ �����ؾ��Ѵ�.
	pModel->UpdateMatrixBone();		// ���������� ���ִϸ��̼��� ���� �����Ƿ� GetEventPos()�� ����� ���� ������ ������Ʈ�� ����� �Ѵ�.
#endif // WorldServer
	
	switch( dwState )
	{
	case OBJSTA_ATK1:	
		{
	#ifdef __WORLDSERVER			
			OnDamage( nItemID, dwState, pHitObj );		
	#endif //__WORLDSERVER			
		}
		break;
	case OBJSTA_ATK2:	
		{
	#ifdef __WORLDSERVER
			int nRandom = xRandom(100);
			if( nRandom <= 50 )
			{
				SkillProp *pSkillProp;
				AddSkillProp *pAddSkillProp;
				BOOL bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SKILL_INDEX( 301, SI_KRRR_POISON ), 1, "OnSfxHit" );
				if( bSuccess )
				{
					g_cSkillSystem->ApplySkill( this, pHitObj, pSkillProp, pAddSkillProp );
				}
			}
			else
			{
				OnDamage( nItemID, dwState, pHitObj );		
			}
	#endif // WorldServer
		}
		break;
	case OBJSTA_ATK3:		// �μ����� ���� ����ġ��
		{
		#ifdef __CLIENT
			CSfx *pSfx = CreateSfx( D3DDEVICE, XI_INDEX( 1615, XI_NPCSP1DIRFIRESP ), pHitObj->GetPos() );	// ����Ʈ�� �߻� - �������� ���� �ȵ�.
			if( pSfx )
				pSfx->SetAngle( -GetAngle() );

			PLAYSND( SND_PC_BOSS_CRASH, &pHitObj->GetPos() );
		#endif //__CLIENT

		#ifdef __WORLDSERVER
			SkillProp *pSkillProp;
			AddSkillProp *pAddSkillProp;
			BOOL bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SKILL_INDEX( 224, SI_GEN_KNOCK_BACK ), 4, "OnAttackMelee_BigMuscle" );
			if( bSuccess )
				g_cSkillSystem->ApplySkillAround( this, this, OBJTYPE_PLAYER, pSkillProp, pAddSkillProp , false );							

			g_xApplyItemEffect->SendDamageAroundPos( &(pHitObj->GetPos()), AF_GENERIC, this, OBJTYPE_PLAYER, ITEM_INDEX( 9261, II_WEA_MOB_MONSTER4_ATK1 ), 7.0f );	
		#endif //__WORLDSERVER
		}
		break;
	case OBJSTA_ATK4:		// �ֹ��� �ܿ�� �÷��̾�� �Ѱ��� ������ ���������� ��
		{
			static D3DXVECTOR3	  vLocal;
			if( m_pActMover->m_nMotionHitCount == 0 )
			{
		#ifdef __CLIENT
				PLAYSND( SND_PC_BOSS_SUMMONS );
		#endif //__CLIENT
				
				AngleToVectorXZ( &vLocal, GetAngle(), 5.0f );	// �����¹��� 5���;�.
				vLocal += GetPos();

				FLOAT fHeight = GetWorld()->GetLandHeight( vLocal.x, vLocal.z );
				vLocal.y = fHeight;
				
		#ifdef __WORLDSERVER
				CPartyQuestProcessor* pProc	= CPartyQuestProcessor::GetInstance();
				CWorld* pWorld	= GetWorld();
				if( pWorld )
				{
					int nId	= pProc->PtInQuestRect( pWorld->GetID(), GetPos() );
					if( nId > -1 )
					{
						PPARTYQUESTELEM pElem	= pProc->GetPartyQuest( nId );
						PPARTYQUESTRECT pRect	= pProc->GetPartyQuestRect( nId );
						if( pElem && pRect )
#ifdef __LAYER_1015
							g_xWSUserManager->ReplaceWorldArea( pElem->idParty, pRect->dwWorldId, pRect->dwWorldId, vLocal.x, vLocal.z, 0.5f, TRUE, GetLayer() );
#else	// __LAYER_1015
							g_xWSUserManager->ReplaceWorldArea( pElem->idParty, pRect->dwWorldId, pRect->dwWorldId, vLocal.x, vLocal.z, 0.5f, TRUE );
#endif	// __LAYER_1015
					}
				}
		#endif //__WORLDSERVER
			}
			else
			if( m_pActMover->m_nMotionHitCount == 1 )		// �� 5���Ŀ� ����
			{	
		#ifdef __CLIENT
				PLAYSND( SND_PC_BOSS_STUN );
		#endif //__CLIENT
#ifdef __WORLDSERVER
				SkillProp *pSkillProp;
				AddSkillProp *pAddSkillProp;
				BOOL bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SKILL_INDEX( 302, SI_KRRR_STUNGROUP ), 1, "OnAttackMelee_Krrr" );
				if( bSuccess )
					g_cSkillSystem->ApplySkillAround( this, this, OBJTYPE_PLAYER, pSkillProp, pAddSkillProp , false );	
#endif //__WORLDSERVER
				
			}
			else
			if( m_pActMover->m_nMotionHitCount == 2 )
			{	
#ifdef __CLIENT
				D3DXVECTOR3 v3Pos = vLocal;
				CSfx *pSfx = CreateSfx( D3DDEVICE, XI_INDEX( 434, XI_SKILL_ELE_MULTI_METEOSHOWER02 ), v3Pos );	// ����Ʈ�� �߻� - �������� ���� �ȵ�.
				if( pSfx )
					pSfx->SetScale( D3DXVECTOR3( 0.8f, 0.8f, 0.8f ) );
				
				v3Pos.x += ( xRandomF( 12.0f ) + -6.0f );
				pSfx = CreateSfx( D3DDEVICE, XI_INDEX( 434, XI_SKILL_ELE_MULTI_METEOSHOWER02 ), v3Pos );	// ����Ʈ�� �߻� - �������� ���� �ȵ�.
				if( pSfx )
					pSfx->SetScale( D3DXVECTOR3( 0.8f, 0.8f, 0.8f ) );
				
				v3Pos = vLocal;
				v3Pos.z += ( xRandomF( 12.0f ) + -6.0f );
				pSfx = CreateSfx( D3DDEVICE, XI_INDEX( 434, XI_SKILL_ELE_MULTI_METEOSHOWER02 ), v3Pos );	// ����Ʈ�� �߻� - �������� ���� �ȵ�.
				if( pSfx )
					pSfx->SetScale( D3DXVECTOR3( 0.8f, 0.8f, 0.8f ) );
				
#endif //__CLIENT
				
			}
			else
			if( m_pActMover->m_nMotionHitCount == 3 )
			{	
		#ifdef __WORLDSERVER
				g_xApplyItemEffect->SendDamageAroundPos( &(pHitObj->GetPos()), AF_GENERIC, this, OBJTYPE_PLAYER, ITEM_INDEX( 9262, II_WEA_MOB_MONSTER4_ATK2 ), 5.0f );	
		#endif //__WORLDSERVER
				
			}
		}
		break;
	}
}

//sun: ���׿���Ŀ ���̾� �߻�!!!
void	CMover::OnAttackMelee_Meteonyker( DWORD dwState, CMover *pHitObj )
{
	int		nItemID = GetActParam( 1 );	// ����� ������ ���̵�

	CModelObject *pModel = (CModelObject *)m_pModel;
	
#ifdef __WORLDSERVER
	UpdateMatrix();		// ��ü�� Ư���������� ���� �������� ó���ϱ� ���ؼ� ���������� ��Ʈ������ �����ؾ��Ѵ�.
	pModel->UpdateMatrixBone();		// ���������� ���ִϸ��̼��� ���� �����Ƿ� GetEventPos()�� ����� ���� ������ ������Ʈ�� ����� �Ѵ�.
#endif // WorldServer
	
	switch( dwState )
	{
	case OBJSTA_ATK1:
		{
			if( IsValidObj(pHitObj) )
			{
				OnDamage( nItemID, dwState, pHitObj );		
			}
		}
		break;
	case OBJSTA_ATK3:		// ���̾ �߻�
		{
			if( m_pActMover->m_nMotionHitCount == 0 )
			{
				CMover *pTarget = pHitObj;

				D3DXVECTOR3 vPos;
				CModelObject *pModel = (CModelObject *)m_pModel;
				pModel->GetEventPos( &vPos, 0 );	// ���� �� ��ǥ.
				D3DXVec3TransformCoord( &vPos, &vPos, GetMatrixWorldPtr() );

				if( IsValidObj(pTarget) )
				{
					OnDamage( nItemID, dwState, pTarget, &pTarget->GetPos() );		// Ÿ���� ��ǥ�� �߽����� �������� ��.
					SkillProp *pSkillProp;
					AddSkillProp *pAddSkillProp;
					BOOL bFind	= GetSkillProp( &pSkillProp, &pAddSkillProp, SKILL_INDEX( 189, SI_GEN_STUNSINGLE ), 5, "SI_GEN_STUNSINGLE" );
					if( bFind )
					{
						if( xRand() % 2 )
							g_cSkillSystem->DoApplySkill( this, pTarget, pSkillProp, pAddSkillProp , false );
					}

	#ifdef __CLIENT
					CSfx *pSfx = CreateSfx( D3DDEVICE, XI_INDEX( 1616, XI_NPCSP1RANBALL ), vPos, GetId(), pTarget->GetPos(), pTarget->GetId(), -1 );		// ��ź
					if( pSfx )
					{
						pSfx->SetScale( D3DXVECTOR3(4.0f, 4.0f, 4.0f) );
						{
							FLOAT fAngXZ, fAngH;
							D3DXVECTOR3 vDist = pTarget->GetPos() - vPos;
							xGetDegree( &fAngXZ, &fAngH, vDist );		// ��ǥ���� ��������.
							((CSfxAtkStraight *)pSfx)->ShootSfx( fAngXZ, -fAngH, 2.0f, XI_INDEX( 1617, XI_NPCSP1RANBOOM ) );		// ���ư��� ���� ����.
						}
					}
	#endif	// __CLIENT
				}
			}
		}
		break;
	case OBJSTA_ATK4:   // ���� �����ٰ� ��ġ��
		{
			if( m_pActMover->m_nMotionHitCount == 0 )
			{
				// ���� ȿ��
				SkillProp *pSkillProp;
				AddSkillProp *pAddSkillProp;
				BOOL bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SKILL_INDEX( 308, SI_GEN_BURN ), 1, "OnAttackMelee_Meteonyker" );
				if( bSuccess )
				{
					D3DXVECTOR3 vPos	= GetPos();
					g_cSkillSystem->ApplySkillAround( this, this, OBJTYPE_PLAYER, pSkillProp, pAddSkillProp, false, FALSE, 10.0F );
					MoverProp* pProp	= GetProp();
					OnDamage( pProp->dwAtk4, OBJSTA_ATK4, NULL, &vPos );
#ifdef __CLIENT
					CSfx *pSfx = CreateSfx( D3DDEVICE, XI_INDEX( 1863, XI_NPCMETEONYKER ), vPos );
#endif	// __CLIENT
				}
			}
		}
		break;
	}
}

void	CMover::OnAttackMelee_Bear( DWORD dwState, CMover *pHitObj )
{
	int		nItemID = GetActParam( 1 );	// ����� ������ ���̵�

	CModelObject *pModel = (CModelObject *)m_pModel;
	
#ifdef __WORLDSERVER
	UpdateMatrix();		// ��ü�� Ư���������� ���� �������� ó���ϱ� ���ؼ� ���������� ��Ʈ������ �����ؾ��Ѵ�.
	pModel->UpdateMatrixBone();		// ���������� ���ִϸ��̼��� ���� �����Ƿ� GetEventPos()�� ����� ���� ������ ������Ʈ�� ����� �Ѵ�.
#endif // WorldServer
	
	switch( dwState )
	{
	case OBJSTA_ATK1:	
		{
	#ifdef __WORLDSERVER
			SkillProp *pSkillProp;
			AddSkillProp *pAddSkillProp;
			BOOL bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SKILL_INDEX( 234, SI_GEN_ATKSPELL_RATEGROUP ), 5, "OnAttackMelee_BigMuscle" );
			if( bSuccess )
				g_cSkillSystem->ApplySkillAround( this, this, OBJTYPE_PLAYER, pSkillProp, pAddSkillProp , false );			
			
			OnDamage( nItemID, dwState, pHitObj );					
	#endif // WorldServer
		}
		break;
	case OBJSTA_ATK2:	
		{
	#ifdef __WORLDSERVER
			OnDamage( nItemID, dwState, pHitObj );					
	#endif // WorldServer
		}
		break;
	case OBJSTA_ATK3:		
		{
			if( m_pActMover->m_nMotionHitCount == 0 )
			{				
	#ifdef __CLIENT
				CSfx *pSfx = CreateSfx( D3DDEVICE, XI_INDEX( 1743, XI_SKILL_CIRCLE_DUST ), GetPos() );			
				if( pSfx )
					pSfx->SetScale( D3DXVECTOR3( 12.0f, 12.0f, 12.0f ) );
				
				PLAYSND( SND_PC_SKILLD_HITOFPENYA );//, &v2 );
	#endif //__CLIENT 			
				
	#ifdef __WORLDSERVER
				SkillProp *pSkillProp;
				AddSkillProp *pAddSkillProp;
				BOOL bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SKILL_INDEX( 224, SI_GEN_KNOCK_BACK ), 4, "OnAttackMelee_BigMuscle" );
				if( bSuccess )
					g_cSkillSystem->ApplySkillAround( this, this, OBJTYPE_PLAYER, pSkillProp, pAddSkillProp , false );
	#endif //__WORLDSERVER			
			}
			else
			if( m_pActMover->m_nMotionHitCount == 1 )
			{
	#ifdef __CLIENT
				PLAYSND( SND_PC_BOSS_STUN );
	#endif //__CLIENT
				
	#ifdef __WORLDSERVER
				SkillProp *pSkillProp;
				AddSkillProp *pAddSkillProp;
				BOOL bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SKILL_INDEX( 190, SI_GEN_STUNGROUP ), 5, "OnAttackMelee_Bear" );
				if( bSuccess )
					g_cSkillSystem->ApplySkillAround( this, this, OBJTYPE_PLAYER, pSkillProp, pAddSkillProp , false );				
	#endif //__WORLDSERVER			
			}
		}
		break;
	case OBJSTA_ATK4:	
		{	
#ifdef __WORLDSERVER
			SkillProp *pSkillProp;
			AddSkillProp *pAddSkillProp;
			BOOL bSuccess;
	
			// �ڽ� ����
			bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SKILL_INDEX( 44, SI_ASS_HEAL_HEALING ), 1, "OnAttackMelee_BigMuscle" );
			if( bSuccess )
				g_cSkillSystem->DoApplySkill( this, this, pSkillProp, pAddSkillProp , false );

			//����ũ
			bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SKILL_INDEX( 303, SI_MUSHMOOT_BERSERK ), 1, "OnAttackMelee_Bear" );
			if( bSuccess )
				g_cSkillSystem->DoApplySkill( this, this, pSkillProp, pAddSkillProp , false );

			// ��罺Ų 
			bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SKILL_INDEX( 304, SI_MUSHMOOT_WOODENSKIN ), 1, "OnAttackMelee_Bear" );
			if( bSuccess )
				g_cSkillSystem->DoApplySkill( this, this, pSkillProp, pAddSkillProp , false );
			
			//// ����
			//bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SKILL_INDEX( 49, SI_ASS_CHEER_HEAPUP ), 1, "OnAttackMelee_BigMuscle" );
			//if( bSuccess )
			//	g_cSkillSystem->DoApplySkill( this, this, pSkillProp, pAddSkillProp , false );

			////ĳ��
			//bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SKILL_INDEX( 50, SI_ASS_CHEER_CANNONBALL ), 1, "OnAttackMelee_BigMuscle" );
			//if( bSuccess )
			//	g_cSkillSystem->DoApplySkill( this, this, pSkillProp, pAddSkillProp , false );

			////��Ż����
			//bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SKILL_INDEX( 52, SI_ASS_CHEER_MENTALSIGN ), 1, "OnAttackMelee_BigMuscle" );
			//if( bSuccess )
			//	g_cSkillSystem->DoApplySkill( this, this, pSkillProp, pAddSkillProp , false );

			////������
			//bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SKILL_INDEX( 53, SI_ASS_CHEER_BEEFUP ), 1, "OnAttackMelee_BigMuscle" );
			//if( bSuccess )
			//	g_cSkillSystem->DoApplySkill( this, this, pSkillProp, pAddSkillProp , false );
			//
			//
			////ĳ���÷���
			//bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SKILL_INDEX( 115, SI_ASS_CHEER_CATSREFLEX ), 1, "OnAttackMelee_BigMuscle" );
			//if( bSuccess )
			//	g_cSkillSystem->DoApplySkill( this, this, pSkillProp, pAddSkillProp , false );

			////��ť����
			//bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SKILL_INDEX( 116, SI_ASS_CHEER_ACCURACY ), 1, "OnAttackMelee_BigMuscle" );
			//if( bSuccess )
			//	g_cSkillSystem->DoApplySkill( this, this, pSkillProp, pAddSkillProp , false );

			////���̽�Ʈ
			//bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SKILL_INDEX( 20, SI_ASS_CHEER_HASTE ), 1, "OnAttackMelee_BigMuscle" );
			//if( bSuccess )
			//	g_cSkillSystem->DoApplySkill( this, this, pSkillProp, pAddSkillProp , false );
			

			// �������Ͱ� ���� Ŀ����.
			D3DXVECTOR3 vScale = GetScale();

			vScale *= 1.3f;

			if( vScale.x < 10.0f && m_dwIndex != OBJECT_INDEX( 1565, MI_GIANTTREEK1 ))
			{
				SetScale( vScale );
				g_xWSUserManager->AddSetScale( this, (vScale.x / m_pModel->m_pModelElem->m_fScale)  );
			}
			
#endif //__WORLDSERVER			
		}
		break;
	}
}
// �������� �����ϴ� �κ�. - ���ݹ�Ŀ� ���� Ÿ���ѹ��� ��� ȣ��� ���� �ִ�.
// pHitObj : �������� ���� ���. NULL�̸� vDamagePos�� ��ǥ�� �߽����� �������� �ش�.
BOOL	CMover::OnDamage( int nItemID, DWORD dwState, CMover *pHitObj, const D3DXVECTOR3 *pvDamagePos, DWORD dwAtkFlag )
{
	int nParam = 0x01;		//sun

	if( IsPlayer() && IsDualWeapon() )		// ��Į����?
	{
		if( dwAtkFlag & AF_GENERIC )	// ��Ÿ��
		{
			switch( dwState )
			{
			case OBJSTA_ATK1:	nParam |= 0x01;		break; // ������
			case OBJSTA_ATK2:	nParam |= 0x02;		break; // �޼�
			case OBJSTA_ATK3:	nParam |= 0x01;		break; // ������
			case OBJSTA_ATK4:	nParam |= 0x03;		break; // ���
			case OBJSTA_ATK5:	nParam |= 0x03;		break; // ���
			}
		}
	}


#if defined(__CLIENT)
	if( IsValidObj(pHitObj) )
	{
		CModelObject* pHitModel = (CModelObject*)pHitObj->m_pModel;
		if( pHitModel )
		{
			D3DXVECTOR3 vDir;
			pHitModel->m_SparkInfo.m_bUsed = TRUE;
			pHitModel->m_SparkInfo.m_v3Color.x = 2.0f;
			pHitModel->m_SparkInfo.m_v3Color.y = 2.0f;
			pHitModel->m_SparkInfo.m_v3Color.z = 2.0f;
			pHitModel->m_SparkInfo.m_nCnt = 0;
			pHitModel->m_SparkInfo.m_fLerp= 1.0f;

			vDir = pHitObj->GetPos() - GetPos();
			D3DXVec3Normalize( &pHitModel->m_SparkInfo.m_v3SparkDir, &vDir );
		}
	}
#endif //defined(__CLIENT) 
	
	int	nDamage = 0;
	if( IsPlayer() )
	{
		// �÷��̾�
		nDamage = pHitObj->m_pActMover->SendDamage( dwAtkFlag, GetId(), nParam );
	}
	else
	{
		// ����
		PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( nItemID );
		if( pItemProp )
		{
			if( pItemProp->dwSpellRegion == SRO_AROUND )
			{
				int nType = OBJTYPE_PLAYER;		// �÷��̾�鸸 ������.
				if( pHitObj )
				{
					if( dwAtkFlag == AF_MAGICSKILL )
					{
						pHitObj->SendDamageAround( dwAtkFlag, this, nType, nItemID, (float)pItemProp->_dwSkillRange, 1.0f, 1.0f );
					}
					else
					{
#ifdef __WORLDSERVER
						g_xApplyItemEffect->SendDamageAroundTarget( dwAtkFlag, this, pHitObj, nType, nItemID, (float)pItemProp->_dwSkillRange, 1.0f );
#endif // __WORLDSERVER
					}
				}
				else
				{
					if( dwAtkFlag == AF_MAGICSKILL )
					{
						GetWorld()->SendDamageAround( pvDamagePos, dwAtkFlag, this, nType, nItemID, (float)pItemProp->_dwSkillRange );
					}
					else
					{
#ifdef __WORLDSERVER
						g_xApplyItemEffect->SendDamageAroundPos( pvDamagePos, dwAtkFlag, this, nType, nItemID, (float)pItemProp->_dwSkillRange );
#endif // __WORLDSERVER
					}
				}
			} else
			{
				if( pHitObj )
					nDamage = pHitObj->m_pActMover->SendDamage( dwAtkFlag, GetId(), 1 );
			}
		} else
		{
			if( pHitObj )
				nDamage = pHitObj->m_pActMover->SendDamage( dwAtkFlag, GetId(), 1 );
		}
	}


 #ifdef __WORLDSERVER
	if( nDamage > 0 )		// THIS�� �����ߴµ� ������ ���̴µ� �����ߴ�
	{
		if( pHitObj->m_pActMover->GetState() & OBJSTA_ATK_MELEESKILL )	// �´³��� ��ų ������̾���.
		{
			DWORD dwSkill = pHitObj->GetActParam(0);		// ���� ����ϰ� �ִ� ��ų. dwSkill
			if( dwSkill == SKILL_INDEX( 14, SI_MER_ONE_REFLEXHIT ) )			// ���÷���Ʈ ��ų�� ���� �ִ� �߿� �¾Ҵ�.
			{
				pHitObj->m_nReflexDmg += nDamage;			// ���÷��������� ����.
			}
		}

		////< ��׷� �߰� >
		////���Ͱ� PC�� ó�� �������
		//if( IsMonster() == TRUE && m_nFirstHitObjID == NULL_ID )	
		//{
		//	FLAggro::SumFirstTargetAggro( *this, pHitObj->GetId() );	
		//	m_nFirstHitObjID			= pHitObj->GetId();			
		//}
		////PC�� Monster ���� ���
		//else if( IsPlayer() == TRUE && pHitObj->IsMonster() )					
		//{
		//	if( m_nFirstAttackerObjID == NULL_ID )						//PC�� Monster�� ���ʷ� ���� ���
		//	{
		//		FLAggro::SumFirstAttackerAggro( *pHitObj, GetId() );
		//		m_nFirstHitObjID		= pHitObj->GetId();					
		//	}
		//	else														
		//	{
		//		FLAggro::SumAttackAggro( *pHitObj, this->GetId(), nDamage );
		//	}
		//}
	}
 #endif
	return nDamage > 0;
}

//
// �Ϲ� ������ Ÿ���� ȣ��Ǵ� �ڵ鷯.
BOOL	CMover::OnAttackMelee( DWORD dwState, CMover *pHitObj )
{
//  	if( GetAct() != OBJACT_MELEE_ATTACK ) {
//  		return FALSE;
//  	}

//	CModelObject	*pModel = (CModelObject *)m_pModel;
	int		nItemID = GetActParam( 1 );	// ����� ������ ���̵�

#ifdef __WORLDSERVER
	PostAIMsg( AIMSG_ATTACK_MELEE );
#endif
	
 	if( m_dwIndex == OBJECT_INDEX( 164, MI_CLOCKWORK1 ) )	// Ŭ����ũ�� ��� ������ ����Ʈ.
	{
		OnAttackMelee_ClockWorks( dwState, pHitObj );
		return TRUE;
	}
	
	if( m_dwIndex == OBJECT_INDEX( 165, MI_BIGMUSCLE ) )	// ��ӽ��� ��� ������ ����Ʈ.
	{
		OnAttackMelee_BigMuscle( dwState, pHitObj );
		m_pActMover->m_nMotionHitCount++;		
		return TRUE;
	}

	if( m_dwIndex == OBJECT_INDEX( 166, MI_KRRR ) || m_dwIndex == OBJECT_INDEX( 1554, MI_KRRR2 ) )	// ũ������ ��� ������ ����Ʈ.
	{
		OnAttackMelee_Krrr( dwState, pHitObj );
		m_pActMover->m_nMotionHitCount++;		
		return TRUE;
	}	

	if( m_dwIndex == OBJECT_INDEX( 167, MI_MUSHMOOT ) || m_dwIndex == OBJECT_INDEX( 1552, MI_MUSHMOOT2 ) || m_dwIndex == OBJECT_INDEX( 1565, MI_GIANTTREEK1 ))	// �ӽ���Ʈ�� ��� ������ ����Ʈ.
	{
		OnAttackMelee_Bear( dwState, pHitObj );
		m_pActMover->m_nMotionHitCount++;		
		return TRUE;
	}	

//sun: ���׿���Ŀ ���̾� �߻�!!!
	if( m_dwIndex == OBJECT_INDEX( 715, MI_DU_METEONYKER ) || m_dwIndex == OBJECT_INDEX( 753, MI_DU_METEONYKER2 ) || m_dwIndex == OBJECT_INDEX( 754, MI_DU_METEONYKER3 ) ||
		m_dwIndex == OBJECT_INDEX( 755, MI_DU_METEONYKER4 ) || m_dwIndex == OBJECT_INDEX( 1548, MI_DU_METEONYKER5 ) )
	{
		OnAttackMelee_Meteonyker( dwState, pHitObj );
		m_pActMover->m_nMotionHitCount++;		
		return TRUE;
	}

#ifdef __WORLDSERVER
//sun: 12, // __MONSTER_SKILL
	if( IsNPC() && CMonsterSkill::GetInstance()->ApplySkill( this, pHitObj, ATK_MELEE ) )
	{
		//m_pActMover->m_nMotionHitCount++;
		return TRUE;
	}
#endif // __WORLDSERVER

	BOOL bSuccess = FALSE;
	BOOL bDamage = TRUE;

#ifdef __WORLDSERVER
	if( IsNPC() && pHitObj->IsPlayer() && pHitObj->IsRangeObj( this, 10.0f ) == FALSE )		// �ι��״� ��춫�� �־�ô�. ��->���� 10���� �̻� �������������� �ȸ°�.
		bDamage = FALSE;
#endif
	if( bDamage )
	{
		const DWORD attackFlag = GetAct() == OBJACT_MELEE_ATTACK ? GetActParam( 3 ) : AF_MISS;
		bSuccess = OnDamage( nItemID, dwState, pHitObj, NULL, attackFlag );	// �������� �ش�.
	}

#ifdef __WORLDSERVER
	if( _GetContentState( CT_NEWUI_19 ) == CS_VER1 && IsPlayer() == TRUE && bSuccess == TRUE )
	{
		if( dwState == OBJSTA_ATK1 && ((FLWSUser*)this)->m_playTaskBar.m_nActionPoint < 100 )	// 2Ÿ° ġ�°� �׼�����Ʈ �ö�.
		{
			((FLWSUser*)this)->m_playTaskBar.m_nActionPoint++;
		}
	}
#endif	// worldserver

#ifdef __CLIENT
	CModelObject	*pModel = (CModelObject *)m_pModel;
	if( bSuccess )		// Ÿ�ݿ� �����ߴٸ� Ÿ������ �÷���.
	{
		if( _GetContentState( CT_NEWUI_19 ) == CS_VER1 && CMover::GetActiveMover() == this )
		{
			if( dwState == OBJSTA_ATK1 && g_WndMng.m_pWndTaskBar->m_nActionPoint < 100 )	// 2Ÿ° ġ�°� �׼�����Ʈ �ö�.
			{
				g_WndMng.m_pWndTaskBar->m_nActionPoint++;
			}
		}
		//  �÷��̾�� ��� �ִ� ���⿡ ���� Ÿ�� ���� �ٸ���.
		PT_ITEM_SPEC pItemProp = GetActiveHandItemProp();
		if( pItemProp )
		{
			PLAYSND( pItemProp->dwSndAttack1, &GetPos() );
		}
		
		if( IsPlayer() )
		{
			pModel->m_nPause = 5;	// frame ����
		}
		else
		{
			pModel->m_nPause = 0;	// ���ʹ� ������ ����
			pHitObj->m_pModel->m_nPause = 0;
		}

#if defined(__CLIENT)
		{
			if( IsValidObj( pHitObj ) )
			{
				pModel->m_SparkInfo.m_bUsed  = TRUE;
				pModel->m_SparkInfo.m_v3Color.x = 2.0f;
				pModel->m_SparkInfo.m_v3Color.y = 2.0f;
				pModel->m_SparkInfo.m_v3Color.z = 2.0f;
				pModel->m_SparkInfo.m_nCnt = 0;
				pModel->m_SparkInfo.m_fLerp= 1.0f;
				
				D3DXVECTOR3 vDir = GetPos() - pHitObj->GetPos();
				D3DXVec3Normalize( &pModel->m_SparkInfo.m_v3SparkDir, &vDir );
			}
		}
#endif //defined(__CLIENT)
		
		
	}
#endif // client
	
	
	return TRUE;
}

//
//
// Ÿ�� ���ĺ��� ���������� ��� ȣ��.
BOOL	CMover::OnAttackMeleeContinue( DWORD dwState )
{
	int		nItemID = GetActParam( 1 );	// ����� ������ ���̵�

	if( m_dwIndex == OBJECT_INDEX( 164, MI_CLOCKWORK1 ) )	// Ŭ����ũ�� ��� ������ ����Ʈ.
	{
	#ifdef __WORLDSERVER
		UpdateMatrix();		// ��ü�� Ư���������� ���� �������� ó���ϱ� ���ؼ� ���������� ��Ʈ������ �����ؾ��Ѵ�.
		CModelObject *pModel = (CModelObject *)m_pModel;
		pModel->UpdateMatrixBone();		// ���������� ���ִϸ��̼��� ���� �����Ƿ� GetEventPos()�� ����� ���� ������ ������Ʈ�� ����� �Ѵ�.
	#endif // WorldServer

		switch( dwState )
		{
		case OBJSTA_ATK3:
			{
				D3DXVECTOR3 v1, v2;
				CModelObject *pModel = (CModelObject *)m_pModel;
				pModel->GetEventPos( &v1, 2 );		// ������ ������ �κ�.
				D3DXVec3TransformCoord( &v1, &v1, GetMatrixWorldPtr() );	// ����������� ��ȯ.
				
				v2 = D3DXVECTOR3( 0, 0, 40.0f );
				D3DXVec3TransformCoord( &v2, &v2, pModel->GetMatrixBone(6) );	// 6�� �Ӹ�.  �Ӹ����� v2������ ��ġ�� ���.
				D3DXVec3TransformCoord( &v2, &v2, GetMatrixWorldPtr() );	// ���� �������� ��ȯ.
				
		#ifdef __CLIENT
				extern CPartsLaser g_Laser;
				g_Laser.SetPos( v1, v2, 0.5f );
		#endif

		#ifdef __CLIENT
				if( (m_pActMover->m_nCount & 1) == 0 )
		#else				
				if( (m_pActMover->m_nCount & 3) == 0 )
		#endif
				{
					CWorld *pWorld = GetWorld();
					D3DXVECTOR3 vOut;
					if( pWorld->IntersectObjLine( &vOut, v1, v2, FALSE, TRUE, FALSE ) )
					{
						OnDamage( nItemID, dwState, NULL, &vOut );		// ����Ʈ�� ������ ��ġ�� �������� ��.

					#ifdef __CLIENT
						CSfx *pSfx = CreateSfx( D3DDEVICE, XI_INDEX( 1618, XI_NPCSP1RANSPARK ), vOut );
						if( pSfx )
							pSfx->SetScale( D3DXVECTOR3( 2.0f, 2.0f, 2.0f ) );
					#endif // __CLIENT
					}
						
				}
			}
			break;
		}
	}
//sun: ���׿���Ŀ ���̾� �߻�!!!
	else
	if( m_dwIndex == OBJECT_INDEX( 715, MI_DU_METEONYKER ) || m_dwIndex == OBJECT_INDEX( 753, MI_DU_METEONYKER2 ) || m_dwIndex == OBJECT_INDEX( 754, MI_DU_METEONYKER3 ) ||
		m_dwIndex == OBJECT_INDEX( 755, MI_DU_METEONYKER4 ) || m_dwIndex == OBJECT_INDEX( 1548, MI_DU_METEONYKER5 ) )
	{
	#ifdef __WORLDSERVER
		UpdateMatrix();		// ��ü�� Ư���������� ���� �������� ó���ϱ� ���ؼ� ���������� ��Ʈ������ �����ؾ��Ѵ�.
		CModelObject *pModel = (CModelObject *)m_pModel;
		pModel->UpdateMatrixBone();		// ���������� ���ִϸ��̼��� ���� �����Ƿ� GetEventPos()�� ����� ���� ������ ������Ʈ�� ����� �Ѵ�.
	#endif // WorldServer

		switch( dwState )
		{
			case OBJSTA_ATK2:
			{
				if( m_pActMover->m_nMotionHitCount == 1 )
				{
					D3DXVECTOR3 v1, v2, v3;
					CModelObject *pModel = (CModelObject *)m_pModel;
					pModel->GetEventPos( &v1, 0 );		// ������ ������ �κ�.
					pModel->GetEventPos( &v3, 1 );		// ������ ������ �κ�.
					D3DXVec3TransformCoord( &v1, &v1, GetMatrixWorldPtr() );	// ����������� ��ȯ.
					D3DXVec3TransformCoord( &v3, &v3, GetMatrixWorldPtr() );	// ����������� ��ȯ.

					v2 = D3DXVECTOR3( 0, 0, 40.0f );
					D3DXVec3TransformCoord( &v2, &v2, pModel->GetMatrixBone(6) );	// 6�� �Ӹ�.  �Ӹ����� v2������ ��ġ�� ���.
					D3DXVec3TransformCoord( &v2, &v2, GetMatrixWorldPtr() );	// ���� �������� ��ȯ.
					
			#ifdef __CLIENT
					D3DXVECTOR3 vDir = v3 - v1;
					D3DXVec3Normalize( &vDir, &vDir );
					D3DXVECTOR3 vTemp;
					D3DXVECTOR3 vScal = D3DXVECTOR3( 0.3f, 0.3f, 0.3f );
					D3DXVECTOR3 vModelScal = GetScale();

					vScal.x *= vModelScal.x;
					vScal.y *= vModelScal.y;
					vScal.z *= vModelScal.z;

					for( int i = 0; i < 2; i ++ )
					{
						vTemp = vDir * (xRandomF( 0.2f ) + 0.1f );
						vTemp *= 0.8f;
						vTemp.y -= (xRandomF( 0.05f ) + 0.01f );
						
						extern CPartsFireDragon	g_FireDragon;
						g_FireDragon.Create( D3DDEVICE, v1, XI_INDEX( 1753, XI_NAT_FIRE01_ADV ), vScal, vTemp );
					}

			#endif

			#ifdef __CLIENT
					if( (m_pActMover->m_nCount & 1) == 0 )
			#else				
					if( (m_pActMover->m_nCount & 3) == 0 )
			#endif
					{
//						CWorld *pWorld = GetWorld();
						D3DXVECTOR3 vOut	= GetPos();
//						if( pWorld->IntersectObjLine( &vOut, v1, v2, FALSE, TRUE, FALSE ) )
						{
							OnDamage( nItemID, dwState, NULL, &vOut );		// ����Ʈ�� ������ ��ġ�� �������� ��.
	/*
						#ifdef __CLIENT
							CSfx *pSfx = CreateSfx( D3DDEVICE,  XI_NPCSP1RANSPARK, vOut );
							if( pSfx )
								pSfx->SetScale( D3DXVECTOR3( 2.0f, 2.0f, 2.0f ) );
						#endif // __CLIENT
	*/
						}
							
					}
				}
				break;
			}
		}
	}

	return TRUE;
}

// ������ Ư������ Ÿ�ݺκп� ȣ��
BOOL	CMover::OnAttackSP()
{
	int		idTarget = GetActParam( 0 );		
	int		nItemID = GetActParam( 1 );	// ����� ������ ���̵�
	
	CMover* pHit = prj.GetMover( idTarget );	// Ÿ���� �����͸� ��.
	if( IsInvalidObj(pHit) )	return FALSE;		// Ŭ��������ô� �־����� �߻�Ǳ����� �����Ǿ���.

	PT_ITEM_SPEC pItemProp;
	if( nItemID == NULL_ID || nItemID == 0 )	
		pItemProp = GetActiveHandItemProp();	// �̷����� �� �տ� �� ����(Ȥ�� �⺻������)
	else
		pItemProp = g_xSpecManager->GetSpecItem( nItemID );	// ���� �������� ������ ���� �װɷ� ������(���Ͱ� �ַ� �����)

	if( pItemProp == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s�� itemProp�� ����. %d" ), m_szName, nItemID );
		return FALSE;
	}

	if( pItemProp->dwSpellRegion == SRO_AROUND )
	{
#ifdef __WORLDSERVER
		g_xApplyItemEffect->SendDamageAroundTarget( AF_GENERIC, this, this, OBJTYPE_PLAYER, nItemID, (float)( pItemProp->_dwSkillRange ), 0.0f );
#endif // __WORLDSERVER
	}
	else
	{
#ifdef __CLIENT
		if( pHit->IsLive() )
			pHit->m_pActMover->SendDamage( AF_MONSTER_SP_CLIENT, GetId(), (nItemID << 16) );	// 09_12_21 ���� ����� ����
#else
		if( pHit->IsLive() )
			pHit->m_pActMover->SendDamage( AF_GENERIC, GetId(), (nItemID << 16) );	// ���� ���� ������.
#endif
	}

	return TRUE;
}                                                         

//
//	ĳ���Ͱ� ���������� ������ ������ ȣ��
//
void	CMover::OnActDrop()
{
#ifdef __WORLDSERVER
	if( m_pActMover->GetDmgState() & OBJSTA_DMG_FLY_ALL )		// ������ �ö��̶� �̰� ���ϴ°� ����.
		return;

	if( FALSE == IsPlayer() )
	{
		if( GetSpeedFactor() < 1.9F	//sun: 9,	//__AI_0509
			&& m_dwAIInterface != AII_PET
			)
		{
			ClearDestPos();
			SendActMsg( OBJMSG_STAND );
			g_xWSUserManager->AddMoverBehavior( this );
		}
	}
#else	// __WORLDSERVER
	/*
	if( IsActiveMover() )
	{
		ClearDest();
		SendActMsg( OBJMSG_STOP );
		g_DPlay.SendPlayerBehavior();
	}
	*/
#endif	// not __WORLDSERVER
}

void	CMover::OnActCollision()
{
#ifdef __WORLDSERVER
	if( FALSE == IsPlayer() )
	{
		//sun: 9, //__AI_0509 
		if( GetSpeedFactor() < 1.9F && m_dwAIInterface != AII_PET )
		{
			ClearDest();
			SendActMsg( OBJMSG_STAND );
			g_xWSUserManager->AddMoverBehavior( this );
			PostAIMsg( AIMSG_COLLISION );
		}
	}
#else	// __WORLDSERVER
	if( TRUE == IsActiveMover() )
	{
		//ClearDest();
		//SendActMsg( OBJMSG_STAND );
		CWndWorld* pWndWorld	= (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
		if( pWndWorld )
		{
			pWndWorld->m_bLButtonDown	= FALSE;
		}

		//gmpbigsun( 20120116 ) : ��ֹ��� �ɷ������ cancel action slot
		/*
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			if( g_WndMng.m_pWndTaskBar )
				g_WndMng.m_pWndTaskBar->OnCancelSkill();
		}
		*/
	}
#endif	// __WORLDSERVER
}

// ������ų ���� ��������.
void	CMover::OnActEndMeleeSkill()
{
	ClearActParam();			// �ൿ���� Ŭ����
	m_dwFlag &= (~MVRF_SKILL);
#ifdef __WORLDSERVER
	if( IsPlayer() )
	{
		if( ((FLWSUser *)this)->m_playTaskBar.m_nUsedSkillQueue != SKILL_QUEUE_NONE )
		{
			((FLWSUser *)this)->m_playTaskBar.SetNextSkill( (FLWSUser *)this );
		}
	}
#endif 

}

// ������ų ���� ��������.
void	CMover::OnActEndMagicSkill()
{
	ClearActParam();			// �ൿ���� Ŭ����
	m_dwFlag &= (~MVRF_SKILL);
 #ifdef __WORLDSERVER
	if( IsPlayer() )
	{
		if( ((FLWSUser *)this)->m_playTaskBar.m_nUsedSkillQueue != SKILL_QUEUE_NONE )
		{
			((FLWSUser *)this)->m_playTaskBar.SetNextSkill( (FLWSUser *)this );
		}
	}
 #endif 
}

// ����(JUMP4)�÷��װ� ������� ����.
void	CMover::OnActEndJump4()
{
	if( FALSE == IsEmptyDestPos() )
		SetDestPos( GetDestPos(), m_bForward, FALSE );

	if( m_dwIndex == OBJECT_INDEX( 164, MI_CLOCKWORK1 ) )
	{
		SendActMsg( OBJMSG_APPEAR );
	}
}

// ���� �ϴ� ������ �߻�.
void	CMover::OnActLanding()
{
#ifdef __CLIENT	
	if( m_dwIndex == OBJECT_INDEX( 164, MI_CLOCKWORK1 ) )
	{
		GetWorld()->m_pCamera->SetQuake( 60, 1.0f );		// ������ �������� ���� ��鸲.
	}
#endif
}

// ���� �ϴ� ������ �߻�.
void	CMover::OnActFMove()
{
#ifdef __WORLDSERVER
	PostAIMsg( AIMSG_BEGINMOVE );
#endif
}

// Interact ������Ʈ�� ���� ������.
// pIAObj : � IA������Ʈ�� �����ߴ°�.
// vPos : ������ this�� ��ǥ.
void	CMover::OnActIALanding( CObj *pIAObj, const D3DXVECTOR3 &/*vPos*/ )
{
	if( pIAObj->GetType() != OT_SHIP )	
		return;
	CShip *pIA = (CShip *)pIAObj;

	CObj *pOldIA = GetIAObjLink();
	SetIAObjLink( pIA );
	
	if( IsActiveMover() )
	{
		if( pIA != pOldIA )	// �ɾ�ٴҶ��� ��� ���ŵǱⶫ�� �Ź� �����ʿ�� ����.
		{
#ifdef __CLIENT			
			D3DXVECTOR3 vLocal = GetPos() - pIA->GetPos();		// IA������Ʈ�κ����� ��� ��ǥ�� ����.
			g_DPlay.SendLocalPosFromIA( vLocal, pIA->GetId() );				// �����ǥ ������ ����
			FLTRACE_LOG( PROGRAM_NAME, _T( "SendLocalPosFromIA" ) );
#endif //__CLIENT
		}
	}

}

/*
int		CMover::OnActCollecting() 
{ 
#ifdef __CLIENT
	if( (m_dwFlag & MVRF_COLLECT) == 0 )
	{
		SkillProp *pHandProp = GetActiveHandItemProp();
		if( pHandProp->dwSfxObj2 != NULL_ID )
		{
			D3DXVECTOR3 vSrc, vLocal = D3DXVECTOR3(0, 0.5f, 0);
			((CModelObject *)m_pModel)->GetForcePos( &vLocal, 0, PARTS_RWEAPON, GetMatrixWorld() );
			vSrc = vLocal;
			CSfx *pSfx = CreateSfx( D3DDEVICE, pHandProp->dwSfxObj2, vSrc, GetId(), D3DXVECTOR3(0,0,0), NULL_ID, -1 );
			if( pSfx )
			{
				pSfx->SetAngle( -GetAngle() + 90.0f );
				m_dwFlag |= MVRF_COLLECT;		// SFX�� ��������.
			}
		}
	}
#endif // Client
	return 0; 
}				
*/

// ������ �� ����������(�Ϲ�/��ų/�ϵ� ��)
void	CMover::OnAttackStart( CMover * /*pTarget*/, OBJMSG /*dwMsg*/ )
{
}

// this�� ����ȭ ���� �ɷ������� ������Ŵ.
void	CMover::RemoveInvisible()
{
#ifdef __WORLDSERVER
	if( GetAdjParam( DST_CHRSTATE ) & CHS_INVISIBILITY )		// ����ȭ ���´�.
	{
		ResetDestParam( DST_CHRSTATE, CHS_INVISIBILITY, TRUE );
	}

	RemoveBuff( BUFF_SKILL, SKILL_INDEX( 193, SI_ACR_SUP_DARKILLUSION ) );		// ����.
#endif // __WORLDSERVER
}


// pItem�� ���ð����Ѱ��� �Ǻ�.
#ifdef __WORLDSERVER
BOOL CMover::IsLoot( CItem *pItem, BOOL bPet )
{
	BOOL bTake	= FALSE;
	
	if( pItem->IsDelete() || ( pItem->m_idHolder != 0 && pItem->m_idHolder != m_idPlayer ) )
	{
		return bTake;
	}

	if( pItem->m_idOwn != NULL_ID )		// �����ۿ� ������ ������
	{
		// �Ϲ������� ������ ������.
		CMover *pOther = prj.GetMover( pItem->m_idOwn );		// pItem�� ���� ������ �ִ»��.(����...��Ÿ)
		if( IsValidObj(pOther) )
		{
			if( m_idparty && (pOther->m_idparty == m_idparty) )		// ��Ƽ�� �ְ� ���� ���� ��Ƽ���̸� ������ �־��~~~ ������
				bTake = TRUE;
		} else
			bTake = TRUE;		// idOwn�� Invalid���¸� �ƹ��� ���� �� ����.
		
		if( pItem->m_idOwn == GetId() )		// �������� �������ڸ� ���� �� �ִ�.
			bTake = TRUE;
		
	} else
		bTake = TRUE;	// ������ ���°Ÿ� �ƹ��� ������ �ִ�.

	DWORD dwTime	= g_tmCurrent;
#ifdef __EVENT_MONSTER
	DWORD dwMonsterId = pItem->m_IdEventMonster;
	if( dwMonsterId != NULL_ID && CEventMonster::GetInstance()->SetEventMonster( dwMonsterId ) )
	{
		DWORD dwLootTime = CEventMonster::GetInstance()->GetLootTime();
		if( (dwTime - pItem->m_dwDropTime) >= (DWORD)( SEC(dwLootTime) ) )
			bTake = TRUE;
		
		if( bPet && !CEventMonster::GetInstance()->IsPetAble() )
			bTake = FALSE;
	}
	else
#endif // __EVENT_MONSTER
//sun: 9�� �߰���
	if( (dwTime - pItem->m_dwDropTime) >= SEC(7) )	// 7�ʰ� ������ �ƹ��� ���� �� �ִ�.
		bTake = TRUE;

	// ���� �������� �������� ������ ����.
	if( bPet && bTake )
	{
		if( 0 < ((CCtrl*)pItem)->m_lRespawn )
			bTake = FALSE;
		// ����ǰ�� ���� ���� �ش� �������� ȹ���� �� ����
		FLItemElem* pItemElem	= (FLItemElem*)pItem->m_pItemBase;
		PT_ITEM_SPEC pItemProp		= pItemElem->GetProp();
		if( pItemProp->dwItemKind3 != IK3_GOLD && m_Inventory.IsFull( pItemElem, pItemElem->m_nItemNum ) )
			bTake	= FALSE;
	}

	return bTake;
}
#endif // WorldServer

// DoLoot�� ���� - ���� ����Ʈ�� �������� �ַ����϶� ������ �Ҷ� ó����
BOOL CMover::SubLootDropMobSingle( CItem *pItem )
{
	BOOL bSuccess = TRUE;
#ifdef __WORLDSERVER	
	FLItemBase* pItemBase = pItem->m_pItemBase;
	bSuccess	= CreateItem( pItemBase );
	
	if( IsPlayer() && pItemBase->GetProp() != NULL )
	{
		if( bSuccess )
		{
			((FLWSUser*)this)->AddDefinedText( TID_GAME_REAPITEM, "\"%s\"", pItemBase->GetProp()->szName );

			//if( IsLootSendText( pItemBase->GetProp() ) )
			//	((FLWSUser*)this)->AddChatText( TID_GAME_REAPITEM, "\"%s\"", pItemBase->GetProp()->szName );									
			
		}
		else
			((FLWSUser*)this)->AddDefinedText( TID_GAME_LACKSPACE );
	}
	
	if( ( (FLItemElem*)pItemBase )->IsLogable() && bSuccess )
	{
		switch( pItemBase->GetProp()->dwItemKind2 ) 
		{
		case IK2_FOOD:
		case IK2_MAGIC:
		case IK2_POTION:
		case IK2_REFRESHER:
			{
				if( ( (FLItemElem*)pItemBase )->m_nItemNum > 9 )
				{
					LogItemInfo aLogItem;
					//aLogItem.Action = "R";
					//aLogItem.SendName = "GROUND";
					//aLogItem.RecvName = GetName();
					FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "R" );
					FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), "GROUND" );
					FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), GetName() );
					aLogItem.WorldId = GetWorld()->GetID();
					aLogItem.Gold = aLogItem.Gold2 = GetGold();
					g_DPSrvr.OnLogItem( aLogItem, ( (FLItemElem*)pItemBase ), ( (FLItemElem*)pItemBase )->m_nItemNum );
				}
				break;
			}
		default:
			{
				LogItemInfo aLogItem;
				//aLogItem.Action = "R";
				//aLogItem.SendName = "GROUND";
				//aLogItem.RecvName = GetName();

				FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "R" );
				FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), "GROUND" );
				FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), GetName() );

				aLogItem.WorldId = GetWorld()->GetID();
				aLogItem.Gold = aLogItem.Gold2 = GetGold();
				g_DPSrvr.OnLogItem( aLogItem, ( (FLItemElem*)pItemBase ), ( (FLItemElem*)pItemBase )->m_nItemNum );
				break;
			}
		}
	}

#endif // WorldServer
	return bSuccess;
} // SubLootDropMobSingle

// DoLoot�� ���� - ���� ����Ʈ�� �������� ��Ƽ�� ������ ������ �Ҷ� ó����
BOOL CMover::SubLootDropMobParty( CItem *pItem, CParty *pParty )
{
	BOOL bSuccess = TRUE;;
#ifdef __WORLDSERVER
	
	int	nMaxListMember = 0;
	FLWSUser* pMember	= NULL;
	FLWSUser* pGetUser = NULL;
//	float fDist;
	D3DXVECTOR3	vDist;
	FLItemBase* pItemBase = pItem->m_pItemBase;
	
	// �������ݴ»�� �ݰ�ȿ� ��� ����鸸 �߷�����.
	//mem_set( pListMember, 0, sizeof(pListMember) );
	FLWSUser *pListMember[ 8 ] = { 0 };
	for( int i = 0; i < pParty->m_nSizeofMember; i++ )
	{
		pMember		= g_xWSUserManager->GetUserByPlayerID( pParty->m_aMember[i].m_uPlayerId );
//		if( IsValidObj( (CObj*)pMember ) )
		// 12-3 ˬ: ������ ȹ���� ���� �� ���� �ݰ濡 ���� ����ڷ� �����ؾ��Ѵ�.
		/*  
		if( IsValidObj( (CObj*)pMember ) && GetWorld() == pMember->GetWorld() )
		{
			vDist = pMember->GetPos() - GetPos();
			fDist = D3DXVec3LengthSq( &vDist );		// ������ �ݴ»���� ������� �Ÿ�.
			if( fDist < 32.0f * 32.0f )		// xx���ͺ��� ������
			{
				pListMember[ nMaxListMember++ ] = pMember;
			}
		}
		*/ //2009.01.21 // �ݰ� �˻翡 ���̾� �߰�
		if( IsValidArea( pMember, 32.0f ) )
			pListMember[ nMaxListMember++ ] = pMember;
	}

	if( nMaxListMember == 0 )
	{
		pGetUser	= (FLWSUser*)this;
	}
	else
	{
		switch( pParty->m_nTroupeShareItem )
		{
		case 1 :	// �����й�

			{
				for( int i = 0 ; i < nMaxListMember ; i++ )
				{
					if( pParty->m_nGetItemPlayerId == pListMember[i]->m_idPlayer )
					{
						if( i + 1 >= nMaxListMember )
						{
							pGetUser = pListMember[0];
							break;												
						}
						else
						{
							pGetUser = pListMember[ i+1 ];
							break;
						}
					}
				}
				
				if( pGetUser == NULL )
				{
					pGetUser = pListMember[0];
				}
			}
			break;
		case 2 :	// �����й� : ���忡�� �� ��
			{
				if( pParty->IsLeader( pListMember[0]->m_idPlayer ) )
				{
					pGetUser = pListMember[0];
				}
				else
				{
					pGetUser = (FLWSUser*)this;
				}
			}
			break;
		case 3 :	// �����й� : �����ϰ� ��
			{
				DWORD dwRand = xRandom( nMaxListMember );
				pGetUser = pListMember[dwRand];
			}
			break;
		default:	// �⺻ ���Ӻй�

			{
				pGetUser = (FLWSUser*)this; 
			}
			break;
		}	// switch
	}	
	pParty->m_nGetItemPlayerId = pGetUser->m_idPlayer;

	bSuccess	= pGetUser->CreateItem( pItemBase );

	for( int i = 0; i < nMaxListMember; i++ )
	{
		pMember = pListMember[i];
		if( TRUE == pMember->IsPlayer() && pItemBase->GetProp() != NULL )
		{
			if( bSuccess )
			{
				if( pMember->m_idPlayer != pGetUser->m_idPlayer )
				{
					if( g_xFlyffConfig->GetMainLanguage() == LANG_TWN || g_xFlyffConfig->GetMainLanguage() == LANG_JAP || g_xFlyffConfig->GetMainLanguage() == LANG_HK )
						( (FLWSUser*)pMember )->AddDefinedText( TID_GAME_TROUPEREAPITEM, "\"%s\" \"%s\"", pGetUser->GetName(), pItemBase->GetProp()->szName );
					else
					{
						if( g_xFlyffConfig->GetMainLanguage() == LANG_THA )
							( (FLWSUser*)pMember )->AddDefinedText( TID_GAME_REAPITEM_THA, "\"%s\" \"%s\"", pGetUser->GetName(), pItemBase->GetProp()->szName );
						else
							( (FLWSUser*)pMember )->AddDefinedText( TID_GAME_REAPITEM, "\"%s%s %s\"", pGetUser->GetName(), prj.GetText(TID_GAME_FROM2), pItemBase->GetProp()->szName );
					}
				}
				else
				{
					( (FLWSUser*)pGetUser )->AddDefinedText( TID_GAME_REAPITEM, "\"%s\"", pItemBase->GetProp()->szName );
				}
			}
			else
			{
				// CreateItem�� ������ Full
				if( pGetUser == pMember )
					( (FLWSUser*)pGetUser )->AddDefinedText( TID_GAME_LACKSPACE );
			}
		}									
	}
	
	if( ( (FLItemElem*)pItemBase )->IsLogable() && bSuccess )
	{
		switch( pItemBase->GetProp()->dwItemKind2 ) 
		{
		case IK2_FOOD:
		case IK2_MAGIC:
		case IK2_POTION:
		case IK2_REFRESHER:
			{
				if( ( ( (FLItemElem*)pItemBase )->m_nItemNum ) > 9 )
				{
					LogItemInfo aLogItem;
					//aLogItem.Action = "R";
					//aLogItem.SendName = "GROUND";
					//aLogItem.RecvName = pGetUser->GetName();
					FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "R" );
					FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), "GROUND" );
					FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), pGetUser->GetName() );
					aLogItem.WorldId = pGetUser->GetWorld()->GetID();
					aLogItem.Gold = aLogItem.Gold2 = pGetUser->GetGold();
					g_DPSrvr.OnLogItem( aLogItem, ( (FLItemElem*)pItemBase ), ( (FLItemElem*)pItemBase )->m_nItemNum );
				}
				break;
			}
		default:
			{
				LogItemInfo aLogItem;
				//aLogItem.Action = "R";
				//aLogItem.SendName = "GROUND";
				//aLogItem.RecvName = pGetUser->GetName();
				FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "R" );
				FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), "GROUND" );
				FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), pGetUser->GetName() );
				aLogItem.WorldId = pGetUser->GetWorld()->GetID();
				aLogItem.Gold = aLogItem.Gold2 = pGetUser->GetGold();
				g_DPSrvr.OnLogItem( aLogItem, ( (FLItemElem*)pItemBase ), ( (FLItemElem*)pItemBase )->m_nItemNum );
				break;
			}
		}
	}
#endif // __WORLDSERVER
	return bSuccess;
} // SubLootDropMobParty

//
//
// DoLoot�� ���� - ���� ����Ʈ�� ������ ���ú�...
BOOL CMover::SubLootDropMob( CItem *pItem )
{
	BOOL bSuccess = TRUE;
#ifdef __WORLDSERVER
	CParty *pParty	= g_PartyMng.GetParty( m_idparty );
	if( pParty && pParty->IsMember( m_idPlayer )  )	// ��Ƽ�� �����Ƿ� ����ġ �й��� �ؾ���
	{
		// ��Ƽ�� ������� ���� ó��.
		bSuccess = SubLootDropMobParty( pItem, pParty );
	}
	else // ��Ƽ�� ������~
	{
		bSuccess = SubLootDropMobSingle( pItem );
	}
#endif	// __WORLDSERVER

	return bSuccess;
}

// DoLoot�� ���� - ���� ����Ʈ���� �ƴ�! �����۷��ú�...
BOOL CMover::SubLootDropNotMob( CItem *pItem )
{
	BOOL bSuccess = TRUE;
#ifdef __WORLDSERVER
	FLItemBase* pItemBase = pItem->m_pItemBase;
	
	bSuccess	= CreateItem( pItemBase );
	if( TRUE == IsPlayer() && pItemBase->GetProp() != NULL )
	{
		if( bSuccess )
		{
			((FLWSUser*)this)->AddDefinedText( TID_GAME_REAPITEM, "\"%s\"", pItemBase->GetProp()->szName );						

			//if( IsLootSendText( pItemBase->GetProp() ) )
			//	((FLWSUser*)this)->AddChatText( TID_GAME_REAPITEM, "\"%s\"", pItemBase->GetProp()->szName );									
		}
		else
			((FLWSUser*)this)->AddDefinedText( TID_GAME_LACKSPACE );
	}
	
	if( ( (FLItemElem*)pItemBase )->IsLogable() && bSuccess )
	{
		switch( pItemBase->GetProp()->dwItemKind2 ) 
		{
		case IK2_FOOD:
		case IK2_MAGIC:
		case IK2_POTION:
		case IK2_REFRESHER:
			{
				if( ( (FLItemElem*)pItemBase )->m_nItemNum > 9 )
				{
					LogItemInfo aLogItem;
					//aLogItem.Action = "R";
					//aLogItem.SendName = "GROUND";
					//aLogItem.RecvName = GetName();
					FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "R" );
					FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), "GROUND" );
					FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), GetName() );
					aLogItem.WorldId = GetWorld()->GetID();
					aLogItem.Gold = aLogItem.Gold2 = GetGold();
					g_DPSrvr.OnLogItem( aLogItem, ( (FLItemElem*)pItemBase ), ( (FLItemElem*)pItemBase )->m_nItemNum );
				}
				break;
			}
		default:
			{
				LogItemInfo aLogItem;
				//aLogItem.Action = "R";
				//aLogItem.SendName = "GROUND";
				//aLogItem.RecvName = GetName();
				FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "R" );
				FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), "GROUND" );
				FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), GetName() );
				aLogItem.WorldId = GetWorld()->GetID();
				aLogItem.Gold = aLogItem.Gold2 = GetGold();
				g_DPSrvr.OnLogItem( aLogItem, ( (FLItemElem*)pItemBase ), ( (FLItemElem*)pItemBase )->m_nItemNum );
				break;
			}
		}
	}
#endif // __WORLDSERVER
	
	return bSuccess;
} // SubLootDropNotMob

//////////////////////////////////////////////////////////////////////////
//
// �ٴڿ� ������ pItem�� ���´�.
//
//////////////////////////////////////////////////////////////////////////
BOOL CMover::DoLoot( CItem *pItem )
{
	if( IsPlayer() == FALSE )	return FALSE;

	FLItemBase* pItemBase = pItem->m_pItemBase;
	BOOL bSuccess = TRUE;

#ifdef __CLIENT	
	PLAYSND( SND_INF_GROUNDPICKUP );
#endif
	
	PT_ITEM_SPEC pItemProp = pItem->GetProp();
	if( pItemProp->dwItemKind1 == IK1_GOLD ) 
	{
		PickupGold( ((FLItemElem*)pItemBase)->GetGold(), pItem->m_bDropMob );
	}
	else 
	{
		// <������ �α� - ȹ��>
#ifdef __WORLDSERVER
		if( pItem->m_bDropMob == TRUE ) // ���Ͱ� ����߸��͸�
		{
			// ���� ����Ʈ�� �����ۿ� ���� ���� ó��
			bSuccess = SubLootDropMob( pItem );
		}
		else	// ������� ����߸����� �������� ����~
		{
			// ���� ����Ʈ���� �ƴ� �������� ���ÿ� ���� ó��
			bSuccess = SubLootDropNotMob( pItem );

		}
#endif	// __WORLDSERVER
	}

	if( TRUE == bSuccess )
	{
		pItem->Delete();
#ifdef __WORLDSERVER
//		g_xWSUserManager->AddMotion( this, OBJMSG_PICKUP );
		RemoveInvisible();		// ������ ������ ���� ����.
#endif	// __WORLDSERVER
	}

	return bSuccess;
}

