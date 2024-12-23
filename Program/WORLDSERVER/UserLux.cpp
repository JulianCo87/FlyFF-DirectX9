#include "stdafx.h"
#include "User.h"
#include <WorldMng.h>

#include "UserMacro.h"

extern	CWorldMng	g_WorldMng;


#include "FLMapGrid.h"
#include "LinkMap.h"

/*  
//
// �ڿ� ä�� ����
//
BOOL FLWSUser::DoCollect( CMover *pTarget )
{
	BOOL bAble = TRUE;

	if( pTarget->m_idCollecter != NULL_ID )	// pTarget�� ������ �ִ� ���̰� 
	{
		CMover *pCollecter = prj.GetMover( pTarget->m_idCollecter );
		if( IsValidObj(pCollecter) )
		{
			if( m_idparty )		// ä���Ϸ��� ����� ��Ƽ�� �ִ°�?
			{
				if( m_idparty != pCollecter->m_idparty )		// �� ���ΰ� ���� ��Ƽ�� �ƴϴ�.
					if( pTarget->m_nCollectOwnCnt > 0 )			// ���� �ð��� ��Ǯ�ȴ°�?
						bAble = FALSE;		// ������.
			} else
			// ��Ƽ�� ������
			{
				if( pTarget->m_idCollecter != GetId() )			// �� ������ ���� �ƴϳ�?
					if( pTarget->m_nCollectOwnCnt > 0 )			// ���� �ð��� ��Ǯ�ȴ°�?
						bAble = FALSE;		// ������.
			}
		}
//		else
			// ������ ��������� �� ���� �� ����.
//
//		if( pTarget->m_idCollecter != GetId() )	// �� ������ ���� �ƴϰ�
//		{
//			CMover *pCollecter = prj.GetMover( pTarget->m_idCollecter );
//			if( IsValidObj( pCollecter ) )
//			{
//				if( m_idparty && m_idparty != pCollecter->m_idparty )	// ���� ��Ƽ���� �ƴϰ�
//				{
//					if( pTarget->m_nCollectOwnCnt > 0 ) // ���� �ð��� �� Ǯ������
//					{
//						AddDefinedText( TID_COLLECT_ERROROTHER );
//						return FALSE;				// pTarget�� this�� �� �Դ´�.
//					}
//				}
//			}
//		}

	}

	// ä���� ������ �ʴ� ��Ȳ�̸�
	if( bAble == FALSE )
	{
		AddDefinedText( TID_COLLECT_ERROROTHER );		// �����޽��� ����ϰ�
		return FALSE;				// pTarget�� this�� �� �Դ´�.
	}
	

	SkillProp *pHandProp = GetActiveHandItemProp();
	if( pHandProp->dwItemKind3 != IK3_COLLECTER )
		return FALSE;
				
	if( CMover::DoCollect( pTarget ) == TRUE )
	{
		m_idTargetCollect = pTarget->GetId();	// pTarget�� ä�����̶�°��� ǥ����.
//		pTarget->m_idCollecter = GetId();		// Ÿ�ٿ��� ������� ǥ����.
		m_nCollect = 0;
		m_tmCollect = g_tmCurrent;
	} else
		return FALSE;
	
	return TRUE;
}
*/

/*
//
//
// this�� pTarget�� ä���Ѵ�.
int		FLWSUser::OnActCollecting( void )
{
	CMover *pTarget = prj.GetMover( m_idTargetCollect );		// ä������� ������ ����.
	if( IsInvalidObj( pTarget ) )	// ä���� �ð��� �ٵǼ� ������Ʈ�� ������ų� �Žñ��� ������Ʈ�� �� ����.
		return TRUE;		// ä�� ���� �����϶�.

	// ä�� ����� ����Ǵµ��� ��� ���´�.
	if( g_tmCurrent > m_tmCollect + 1000.0f )
	{
		m_tmCollect = g_tmCurrent;
		SkillProp *pHandProp = GetActiveHandItemProp();
		if( pHandProp == NULL )
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "pHandProp==NULL" ) );
			return TRUE;
		}
		if( pHandProp->dwItemKind3 != IK3_COLLECTER )		// ä���� ������ ���ָ� ä�� ���.
			return TRUE;
		
		m_nCollect += (int)pHandProp->dwLoadingTime;				// �������� ä���ɷ¿� ���� ä������ ����.
		pTarget->m_nResource -= (int)pHandProp->dwLoadingTime;		// ä���� ��ŭ Ÿ���� ���ҽ����� ��.
		if( pTarget->m_nResource < 0 )
			pTarget->m_nResource = 0;
		if( m_nCollect >= pHandProp->nShellQuantity )	// ä������ �ƽ�ġ�� �Ѿ��.
		{
			//������ 1�� ����;
			BYTE bID;
			FLItemElem itemElem;
			itemElem.m_dwItemId	= pTarget->GetProp()->dwSourceMaterial;		// �ڿ� ����.
			itemElem.m_nItemNum		= 1;
			if( itemElem.m_dwItemId != NULL_ID )
			{
				BOOL bRet = CreateItem( &itemElem, &bID );
				if( bRet == TRUE )
				{
					LogItemInfo aLogItem;
					aLogItem.Action = "C";
					aLogItem.SendName = pTarget->GetName();
					aLogItem.RecvName = GetName();
					aLogItem.WorldId = GetWorld()->GetID();
					aLogItem.Gold = GetGold();
					aLogItem.Gold2 = GetGold();
					aLogItem.ItemNo = itemElem.GetSerialNumber();
					aLogItem.ItemName = itemElem.GetProp()->szName;
					aLogItem.itemNumber = 1;
					g_DPSrvr.OnLogItem( aLogItem );
				}
			} else
				FLERROR_LOG( PROGRAM_NAME, _T( "%s�� dwSourceMaterial�� -1" ), pTarget->GetName() );

			FLOAT fResRatio = (float)pTarget->m_nResource / pTarget->GetProp()->dwMaterialAmount;		// ���ۼ�Ʈ ���Ҵ��� ...
			FLOAT fScale = pTarget->m_pModel->m_pModelElem->m_fScale;		// mdlDyna�� �ִ� scale���� 100%
			if( fResRatio > 1.0f )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "%f %d %d" ), fResRatio, pTarget->m_nResource, (int)pTarget->GetProp()->dwMaterialAmount );
				fResRatio = 1.0f;
			}
			fScale *= fResRatio;
			D3DXVECTOR3 vScale = D3DXVECTOR3( fScale, fScale, fScale );
			pTarget->SetScale( vScale );
			if( fScale > 10.0f )
				FLERROR_LOG( PROGRAM_NAME, _T( "%s %f %f %d %d" ), GetName(), fScale, 
														pTarget->m_pModel->m_pModelElem->m_fScale, pTarget->m_nResource, pTarget->GetProp()->dwMaterialAmount );
			
			g_xWSUserManager->AddSetScale( pTarget, fResRatio );
			

			m_nCollect -= pHandProp->nShellQuantity;	// �����Ȱ� ������.
		}
		if( pTarget->m_nResource <= 0 )							// Ÿ���� ���ҽ��� �ٶ���������.
		{
			// ä����
			pTarget->m_nResource = 0;							// 0���θ� �صΰ� Ÿ���� �������°� ���ʿ��� �˾Ƽ� �Ѵ�.
			AddDefinedText( TID_COLLECT_EMPTY );	// ä���� �������ϴ�
			return TRUE;	// ä������ �����϶�.
		}			
	}
	return 0;
}
*/

// tmMaxEscape : Ż���� ��Ÿ��
void FLWSUser::AddEscape( DWORD tmMaxEscape )
{
	if( IsDelete() )	return;
	
	m_Snapshot.cb++;
	m_Snapshot.ar << NULL_ID;
	m_Snapshot.ar << SNAPSHOTTYPE_DO_ESCAPE;
	m_Snapshot.ar << tmMaxEscape;
}

// �׼�����Ʈ ����.
void FLWSUser::AddSetActionPoint( int nAP )
{
	if( IsDelete() )	return;
	
	m_Snapshot.cb++;
	m_Snapshot.ar << NULL_ID;
	m_Snapshot.ar << SNAPSHOTTYPE_SETACTIONPOINT;
	m_Snapshot.ar << nAP;
}

void FLWSUser::AddSetTarget( OBJID idTarget )
{
	if( IsDelete() )	return;
	
	m_Snapshot.cb++;
	m_Snapshot.ar << NULL_ID;
	m_Snapshot.ar << SNAPSHOTTYPE_SETTARGET;
	m_Snapshot.ar << idTarget;
}

// ----------------------------------------------------------------------------------
//
//   FLWSUserManager Lux
//
// ----------------------------------------------------------------------------------
/*
void	FLWSUserManager::AddDoCollect( FLWSUser* pUser, OBJID idTarget )
{
	CAr ar;
	ar << GETID( pUser ) << SNAPSHOTTYPE_DO_COLLECT;
	ar << idTarget;
	GETBLOCK( ar, lpBuf, nBufSize );
	
	FOR_VISIBILITYRANGE( pUser )
		USERPTR->AddBlock( lpBuf, nBufSize );
	NEXT_VISIBILITYRANGE( pUser )
}
*/

void	FLWSUserManager::AddCreateSkillEffect( CMover *pAttacker, OBJID idTarget, DWORD dwSkill, DWORD dwLevel )
{
	if( pAttacker == NULL )
	{
		return;
	}

	CAr ar;
	ar << GETID( pAttacker ) << SNAPSHOTTYPE_ACTIVESKILL;
	ar << idTarget << dwSkill << dwLevel;
	GETBLOCK( ar, lpBuf, nBufSize );
	
	if( pAttacker->IsPlayer() == TRUE )
	{
		static_cast<FLWSUser*>( pAttacker )->AddBlock( lpBuf, nBufSize );
	}

	FOR_VISIBILITYRANGE( pAttacker )
		if( pAttacker->m_idPlayer != USERPTR->m_idPlayer )
		{
			USERPTR->AddBlock( lpBuf, nBufSize );
		}
	NEXT_VISIBILITYRANGE( pAttacker )
}

// pMover�� ���� �Ȱ��� ���� ����Ŭ�� �˸�.
void	FLWSUserManager::AddSetStun( CMover *pMover, int nMaxTime )
{
	if( pMover == NULL )
	{
		return;
	}

	CAr ar;
	ar << GETID( pMover ) << SNAPSHOTTYPE_SETSTUN;
	ar << nMaxTime;
	GETBLOCK( ar, lpBuf, nBufSize );
	
	if( pMover->IsPlayer() == TRUE )
	{
		static_cast<FLWSUser*>( pMover )->AddBlock( lpBuf, nBufSize );
	}

	FOR_VISIBILITYRANGE( pMover )
		if( pMover->m_idPlayer != USERPTR->m_idPlayer )
		{
			USERPTR->AddBlock( lpBuf, nBufSize );
		}
	NEXT_VISIBILITYRANGE( pMover )
}

void	FLWSUserManager::AddDoApplySkill( CCtrl *pCtrl, OBJID idTarget, DWORD dwSkill, DWORD dwLevel )
{
	if( pCtrl == NULL )
	{
		return;
	}

	CAr ar;
	ar << GETID( pCtrl ) << SNAPSHOTTYPE_DOAPPLYUSESKILL;
	ar << idTarget << dwSkill << dwLevel;
	GETBLOCK( ar, lpBuf, nBufSize );
	
	FOR_VISIBILITYRANGE( pCtrl )
		USERPTR->AddBlock( lpBuf, nBufSize );
	NEXT_VISIBILITYRANGE( pCtrl )
}

// SendActMsg�� ���� �����鿡�� ���� .
void	FLWSUserManager::AddSendActMsg( CMover *pMover, OBJMSG dwMsg, int nParam1, int nParam2, int nParam3 )
{
	if( pMover == NULL )
	{
		return;
	}

	CAr ar;
	ar << GETID( pMover ) << SNAPSHOTTYPE_SENDACTMSG;
	ar << (int)dwMsg;
	ar << pMover->IsFly();
	ar << nParam1 << nParam2 << nParam3;
	GETBLOCK( ar, lpBuf, nBufSize );
	
	if( pMover->IsPlayer() == TRUE )
	{
		static_cast<FLWSUser*>( pMover )->AddBlock( lpBuf, nBufSize );
	}

	FOR_VISIBILITYRANGE( pMover )
		if( pMover->m_idPlayer != USERPTR->m_idPlayer )
		{
			USERPTR->AddBlock( lpBuf, nBufSize );
		}
	NEXT_VISIBILITYRANGE( pMover )
}

// Mover�� ����������� �и��� �ϴ�.
// pMover : �и��� ����
// vPos : �и��� ���������� vPos - ��Ȯ�� ����ȭ������ ��ǥ�� �������� �ʿ��ϴ�.
// fAngle : �и��� ���������� Angle - 
// fPushAngle : �̴� ����.
// fPower : �̴� ��.
void	FLWSUserManager::AddPushPower( CMover *pMover, D3DXVECTOR3 vPos, FLOAT fAngle, FLOAT fPushAngle, FLOAT fPower )
{
	if( pMover == NULL )
	{
		return;
	}

	if( (pMover->m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL) || pMover->m_pActMover->GetState() & OBJSTA_STUN ) // ������ �ö������̸� ����
		return;

	CAr ar;
	ar << GETID( pMover ) << SNAPSHOTTYPE_PUSHPOWER;
	ar << vPos;
	ar << fAngle;
	ar << fPushAngle << fPower;
	GETBLOCK( ar, lpBuf, nBufSize );
	
	if( pMover->IsPlayer() == TRUE )
	{
		static_cast<FLWSUser*>( pMover )->AddBlock( lpBuf, nBufSize );
	}

	FOR_VISIBILITYRANGE( pMover )
		if( pMover->m_idPlayer != USERPTR->m_idPlayer )
		{
			USERPTR->AddBlock( lpBuf, nBufSize );
		}
	NEXT_VISIBILITYRANGE( pMover )
}

void	FLWSUserManager::AddRemoveSkillInfluence( CMover *pMover, WORD wType, WORD wID )
{
	if( pMover == NULL )
	{
		return;
	}

	CAr ar;
	ar << GETID( pMover ) << SNAPSHOTTYPE_REMOVESKILLINFULENCE;
	ar << wType;
	ar << wID;
	GETBLOCK( ar, lpBuf, nBufSize );
	
	if( pMover->IsPlayer() == TRUE )
	{
		static_cast<FLWSUser*>( pMover )->AddBlock( lpBuf, nBufSize );
	}

	FOR_VISIBILITYRANGE( pMover )
		if( pMover->m_idPlayer != USERPTR->m_idPlayer )
		{
			USERPTR->AddBlock( lpBuf, nBufSize );
		}
	NEXT_VISIBILITYRANGE( pMover )
}

void FLWSUserManager::AddSetPosAngle( CCtrl* pCtrl, const D3DXVECTOR3 &vPos, FLOAT fAngle )
{
	if( pCtrl == NULL )
	{
		return;
	}

	CAr ar;
	
	ar << GETID( pCtrl ) << SNAPSHOTTYPE_SETPOSANGLE;
	ar << vPos << fAngle;
	
	GETBLOCK( ar, lpBuf, nBufSize );
	
	FOR_VISIBILITYRANGE( pCtrl )
		USERPTR->AddBlock( lpBuf, nBufSize );
	NEXT_VISIBILITYRANGE( pCtrl )
}

/*
void FLWSUser::AddRemoveSkillInfluence( WORD wType, WORD wID )
{
	if( IsDelete() )	return;
	
	m_Snapshot.Lock();
	m_Snapshot.cb++;
	m_Snapshot.ar << GetId();
	m_Snapshot.ar << SNAPSHOTTYPE_REMOVESKILLINFULENCE;
	m_Snapshot.ar << wType;
	m_Snapshot.ar << wID;
	
	m_Snapshot.Unlock();
}
*/

void	FLWSUserManager::AddSetSleeping( CMover *pMover, DWORD dwDuration )
{
	if( pMover == NULL )
	{
		return;
	}

	CAr ar;
	ar << GETID( pMover ) << SNAPSHOTTYPE_SETSLEEPING;
	ar << dwDuration;
	GETBLOCK( ar, lpBuf, nBufSize );

	if( pMover->IsPlayer() == TRUE )
	{
		static_cast<FLWSUser*>( pMover )->AddBlock( lpBuf, nBufSize );
	}

	FOR_VISIBILITYRANGE( pMover )
		if( pMover->m_idPlayer != USERPTR->m_idPlayer )
		{
			USERPTR->AddBlock( lpBuf, nBufSize );
		}
	NEXT_VISIBILITYRANGE( pMover )
}

void	FLWSUserManager::AddSeasonEffect()
{
	for( std::map<DWORD, FLWSUser*>::iterator pos = m_users.begin(); pos != m_users.end(); ++pos ) {
		if( IsValidObj( static_cast<CObj*>( pos->second ) ) == TRUE ) {
			pos->second->AddSeasonEffect();
		}
	}
}
