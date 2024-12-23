// Ctrl.cpp: implementation of the CCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResData.h"
#ifdef __WORLDSERVER
#include "CtrlObjs.h"
#endif
#include "Ship.h"

#ifdef __CLIENT
#include "../Neuz/DPClient.h"
extern	CDPClient	g_DPlay;
#endif

#ifdef __WORLDSERVER

#include "../WorldServer/user.h"
#include "Party.h"

extern	CPartyMng			g_PartyMng;

#endif // Worldserver

#include "commonctrl.h"

#include "FLSkillSystem.h"

extern map< string, DWORD > g_MapStrToObjId;



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCtrl::CCtrl()
{
	m_dwType = OT_CTRL;
	m_objid	 = NULL_ID;

#ifdef __WORLDSERVER
	m_lRespawn	= -1;
	m_nRespawnType = 0;

	//memset( m_nOldCenter, 0, sizeof(m_nOldCenter) );
#endif	// __WORLDSERVER

	m_pIAObjLink = NULL;
	D3DXMatrixIdentity( &m_mInvTM );
}

CCtrl::~CCtrl()
{
#ifdef __WORLDSERVER
	if( IsVirtual() )
		return;

	//RemoveItFromView();
	
#endif	// __WORLDSERVER

	RemoveItFromGlobalId();
}

void CCtrl::Process()
{
	CObj::Process();
#ifdef __WORLDSERVER
	ProcessDeleteRespawn();
#endif
}

void CCtrl::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	CObj::Render( pd3dDevice );
}

BOOL CCtrl::Read( CFileIO* pFile )
{
	return CObj::Read( pFile );
}
// this�� pIA�� ��ũ�Ǿ���.
// pIA�� NULL�̸� ��ũ����.
void CCtrl::SetIAObjLink( CShip *pIA )
{
	if( pIA != NULL )	// IA�� ��ũ���Ѿ� �Ҷ�...
	{
		if( m_pIAObjLink != pIA )				// ������ ��ũ�� �ٸ���ũ�� �ɶ���...
			if( pIA->FindCtrl( GetId() ) == NULL )			// pIA�� �̹� this�� ��ϵǾ����� ����...
				pIA->AddCtrl( GetId() );			// IA������Ʈ�� this�� ��Ͻ�Ŵ.
	} else
	{
		if( m_pIAObjLink )
			m_pIAObjLink->RemoveCtrl( GetId() );
	}

	m_pIAObjLink = pIA;
}

void CCtrl::AddItToGlobalId()
{
	prj.m_objmap.Add( this );

	if( OT_MOVER == GetType() && ( (CMover*)this )->IsPlayer() )
	{
		bool bResult = prj.m_idPlayerToUserPtr.insert( make_pair( ( (CMover*)this )->m_idPlayer, (CMover*)this ) ).second;
		if (bResult == false) {
#ifndef __M_USER_BOTS_UPDATE_GLOBAL_ID
			FLERROR_LOG( PROGRAM_NAME, _T( "id:%d duplicated." ), ((CMover*)this)->m_idPlayer );
#else
			// is bot?
			if (dynamic_cast<CMover*>(this)->m_idPlayer >= INT_MAX) {
				auto it = prj.m_idPlayerToUserPtr.find(dynamic_cast<CMover*>(this)->m_idPlayer);
				if (it != prj.m_idPlayerToUserPtr.end()) { // set new bot to this ID
					it->second = dynamic_cast<CMover*>(this);
				}
			}
			else {
				FLERROR_LOG(PROGRAM_NAME, "AddItToGlobalId id:%d duplicated.", dynamic_cast<CMover*>(this)->m_idPlayer);
			}
#endif
		}
	}

#ifdef __WORLDSERVER	
	if( GetType() == OT_CTRL )
	{
		CCommonCtrl* pCommonCtrl = (CCommonCtrl*)this;
		if( pCommonCtrl->m_CtrlElem.m_strCtrlKey[0] != '\0' )
			g_MapStrToObjId.insert( map< string, DWORD >::value_type(pCommonCtrl->m_CtrlElem.m_strCtrlKey, pCommonCtrl->GetId() ) );
	}
#ifdef __AGGRO16
	FLAggro * pAggro			= static_cast< CMover * >( this	)->GetAggroMng();
	if( pAggro != NULL )
		pAggro->Initialize( GetId() );
#endif //__AGGRO16

	if( GetWorld() != NULL )
	{
		
	}

#endif // __WORLDSERVER
}

void CCtrl::RemoveItFromGlobalId()
{
	if( m_objid == NULL_ID )	
	{
// 		for( map<DWORD, CCtrl*>::iterator it = prj.m_objmap.m_map.begin(); it != prj.m_objmap.m_map.end(); ++it )
// 		{
// 			if( it->second == this )
// 				return;
// 		}

		return;
	}

	prj.m_objmap.RemoveKey( m_objid );

	if( OT_MOVER == GetType() && ( (CMover*)this )->IsPlayer() && ( (CMover*)this )->m_idPlayer != (u_long)0xffffffff )
		prj.m_idPlayerToUserPtr.erase( ( (CMover*)this )->m_idPlayer );

#ifdef __WORLDSERVER	
	if( GetType() == OT_CTRL )
	{
		CCommonCtrl* pCommonCtrl = (CCommonCtrl*)this;
		if( pCommonCtrl->m_CtrlElem.m_strCtrlKey[0] != '\0' )
			g_MapStrToObjId.erase( pCommonCtrl->m_CtrlElem.m_strCtrlKey );
	}
#endif // __WORLDSERVER
}

#ifdef __WORLDSERVER

BOOL CCtrl::ProcessDeleteRespawn()
{
	// �������� Remove�̸� ���� ��� 
#ifdef __LAYER_1021
	CRespawnInfo* pRespawnInfo = GetWorld()->m_respawner.GetRespawnInfo( GetRespawn(), m_nRespawnType, GetLayer() );
#else	// __LAYER_1021
	CRespawnInfo* pRespawnInfo = GetWorld()->m_respawner.GetRespawnInfo( GetRespawn(), m_nRespawnType );
#endif	// __LAYER_1021
	if( pRespawnInfo && pRespawnInfo->m_bRemove )
	{
		Delete();
		return TRUE;
	}
	return FALSE;
}

//void CCtrl::RemoveItFromView( BOOL bRemoveall )
//{
//	if( !GetWorld() )	
//		return;
//
//	FLWSUser* pUser;
//	if( GetType() == OT_MOVER && ( (CMover*)this )->IsPlayer() )
//	{
//		map<DWORD, FLWSUser* >::iterator it = m_2pc.begin();
//		while( it != m_2pc.end() )
//		{
//			pUser = it->second;
//			if( pUser != this && pUser->PCRemoveKey( GetId() ) )
//			{
//				//		pUser->AddRemoveObj( GetId() );
//			}
//		
//			++it;
//		}
//
//		if( bRemoveall )
//			m_2pc.clear();
//	}
//	else
//	{
//		map<DWORD, FLWSUser* >::iterator it = m_2pc.begin();
//		while( it != m_2pc.end() )
//		{
//			pUser = it->second;
//			if( pUser->NPCRemoveKey( GetId() ) )
//			{
//		//		pUser->AddRemoveObj( GetId() );
//			}
//			++it;
//		}
//
//		if( bRemoveall )
//			m_2pc.clear();
//	}
//
//}

BOOL CCtrl::IsNearPC( FLWSUser* pUser )
{
	const float fRange		= 90.0f;		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	const D3DXVECTOR3 vDistant = GetPos() - pUser->GetPos();
	D3DXVECTOR3 vDist = vDistant;
	vDist.y	= 0.0f;
	const float fDistSq = D3DXVec3LengthSq( &vDist );	
	if( fDistSq > fRange * fRange )	
		return FALSE;
	return TRUE;
}
//
//BOOL CCtrl::IsNearPC( OBJID objid )
//{
//	const float fRange		= 90.0f;		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//
//	FLWSUser * pUser		= prj.Get
//
//	const D3DXVECTOR3 vDistant = GetPos() - pUser;
//	D3DXVECTOR3 vDist = vDistant;
//	vDist.y	= 0.0f;
//	const float fDistSq = D3DXVec3LengthSq( &vDist );	
//	if( fDistSq > fRange * fRange )	
//		return FALSE;
//	return TRUE;
//
//	//map<DWORD, FLWSUser* >::iterator it = m_2pc.find( objid );
//	//return ( it != m_2pc.end() ) ;
//}

#endif	// __WORLDSERVER

BOOL	CCtrl::GetSkillProp( SkillProp **ppSkillProp, AddSkillProp **ppAddSkillProp, int nSkill, DWORD dwLevel, LPCTSTR szErr )
{
	SkillProp* pSkillProp = prj.GetSkillProp( nSkill );	// UseSkill���� ����� ��ų�� ������Ƽ ����
	if( pSkillProp == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s : %d. ��ų(%d)�� ������Ƽ�� ����." ), szErr, GetIndex(), nSkill );
		return FALSE;
	}
	
	
	AddSkillProp *pAddSkillProp	= prj.GetAddSkillProp( pSkillProp->dwSubDefine, dwLevel );
	if( pAddSkillProp == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s : %d. Add��ų(%d) Lv%d�� ������Ƽ�� ����." ), szErr, GetIndex(), nSkill, dwLevel );
		return FALSE;
	}

	if( pAddSkillProp->dwSkillPropID != pSkillProp->dwID )
		FLERROR_LOG( PROGRAM_NAME, _T( "%s : %d. Add��ų(%d) AddSkill->dwName�� Skill->dwID�� �ٸ���." ), szErr, GetIndex(), nSkill );

	*ppSkillProp = pSkillProp;
	*ppAddSkillProp = pAddSkillProp;	
	return TRUE;
}




// ��������� Ÿ���� ������ ���� �ֺ� Ÿ���� ������ ������.
int		CCtrl::TargetSelecter( CCtrl *pTarget )
{
	int nApplyType = 0;
	CCtrl *pSrc = this;

	if( pTarget->GetType() == OT_MOVER )	// Ÿ���� �����϶�
	{
		nApplyType = OBJTYPE_PLAYER | OBJTYPE_MONSTER;	// ���� �÷��̾��/���Ϳ��� ����
	}
	else
	{
		// Ÿ���� ��Ʈ���϶�
		if( pSrc->GetType() == OT_MOVER )	// �����ڰ� ������
		{
			if( ((CMover *)pSrc)->IsPlayer() )		// �����ڰ� �÷��̾��
				nApplyType = OBJTYPE_PLAYER | OBJTYPE_CTRL | OBJTYPE_MONSTER;	// ���� ���Ϳ� ��Ʈ�ѿ��� ����.
		}
		else
		{	// �����ڰ� ��Ʈ���̳�
			nApplyType = OBJTYPE_PLAYER;	// �÷��̾�� ����
		}
	}

	return nApplyType;
}



