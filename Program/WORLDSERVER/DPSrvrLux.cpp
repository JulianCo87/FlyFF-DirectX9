#include "stdafx.h"
#include "dpsrvr.h"
#include "user.h"
#include <worldmng.h>
#include <misc.h>

#include <guild.h>
extern	CGuildMng	g_GuildMng;


#include "..\_Common\Ship.h"


#include "..\_aiinterface\AIPet.h"

#include <Party.h>
extern	CPartyMng			g_PartyMng;
extern	CWorldMng			g_WorldMng;

#include "..\_Network\ErrorCode.h"


void CDPSrvr::OnUseSkill( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE /*lpBuf*/, u_long /*uBufSize*/ )
{
	WORD wType		= 0;			// ������ų�̳�, ���̼�����ų�̳� �����ϴ� ���� - 2005.10.04 �ǹ̾��� 
	WORD wId		= 0;
	OBJID objid		= 0;
	int	 nUseType	= 0;
	BOOL bControl	= FALSE;

	ar >> wType >> wId >> objid >> nUseType >> bControl;
	wType = 0;


	FLWSUser* pUser	= g_xWSUserManager->GetUser( dpidCache, dpidUser );
	if( IsValidObj( (CObj*)pUser ) == FALSE )
	{
		return;
	}

	if( pUser->m_vtInfo.VendorIsVendor() )
	{
		pUser->AddHdr( GETID( pUser ), SNAPSHOTTYPE_CLEAR_USESKILL );
		return;
	}

	if( pUser->m_bAllAction == FALSE )
	{
		pUser->AddHdr( GETID( pUser ), SNAPSHOTTYPE_CLEAR_USESKILL );
		return;
	}

	const int nIdx = wId;
	if( nIdx < 0 || nIdx >= MAX_SKILL_JOB )
	{
		pUser->AddHdr( GETID( pUser ), SNAPSHOTTYPE_CLEAR_USESKILL );
		return;
	}

	if( _GetContentState( CT_NEWUI_19 ) == CS_VER1 )	// 18�� ����..
	{
		const DWORD dwResult	= pUser->DoUseSkill( wType, nIdx, objid, (SKILLUSETYPE)nUseType, bControl );
		if( dwResult == FSC_SKILLUSE_SUCCESS )	// ��ų��뿡 �����߰�
		{
			if( nUseType == SUT_QUEUESTART )	// ��ųť�� �����϶�� �Ѱſ���.
			{
				pUser->m_playTaskBar.m_nUsedSkillQueue = SKILL_QUEUE_USE;		// ��ųť �������� ǥ�� ����.
			}
		}
		else
		{
			pUser->AddHdr( GETID( pUser ), SNAPSHOTTYPE_CLEAR_USESKILL );
			return;
		}
	}
	else		// 19�� �̻�..
	{
		/*
		�׼ǽ������� ��ų ��� �� ù ��ų�� ��� ���а� �Ǹ� �׼� ���� ��� ��ü�� �ȵǹǷ�
		ù ��ų�� ��Ÿ�� ������ ��� �Ұ����϶� �׼� ������ ����Ͽ��ٰ� �Ǵ��ϰ� ��Ÿ�� ���� ��ų���� ��ŵ�� �Ѵ�.
		��� ��ų�� ��Ÿ���� ���������ÿ� ��� �Ұ�..
		*/
		if( nUseType == SUT_QUEUESTART ) {
			if( pUser->m_playTaskBar.CanUseSkillQueue( pUser ) == false || pUser->m_playTaskBar.ExistCanUseSkillByActionSlot( pUser ) == false ) {
				pUser->AddHdr( GETID( pUser ), SNAPSHOTTYPE_CLEAR_USESKILL );
				return;
			}

			const DWORD dwResult = pUser->DoUseSkill( wType, nIdx, objid, (SKILLUSETYPE)nUseType, bControl );

			if( dwResult == FSC_SKILLUSE_SUCCESS ) {
				pUser->m_playTaskBar.m_nUsedSkillQueue = SKILL_QUEUE_USE;		// ��ųť �������� ǥ�� ����.
				pUser->m_playTaskBar.SetUseSkillQueue();
			}
			else if( dwResult == FSC_SKILLUSE_SKIP ) {
				pUser->m_playTaskBar.m_nUsedSkillQueue = SKILL_QUEUE_USE;		// ��ųť �������� ǥ�� ����.
				pUser->AddActionSlotSkillSkip_HardCording( SUT_QUEUESTART );
				if( pUser->m_playTaskBar.SetNextSkill( pUser ) == true ) {
					pUser->m_playTaskBar.SetUseSkillQueue();
				}
				else {
					pUser->AddHdr( GETID( pUser ), SNAPSHOTTYPE_USE_ACTION_SLOT_NOTI );
				}
			}
			else {
				pUser->AddHdr( GETID( pUser ), SNAPSHOTTYPE_CLEAR_USESKILL );
				return;
			}
		}
		else {
			const DWORD dwResult	= pUser->DoUseSkill( wType, nIdx, objid, (SKILLUSETYPE)nUseType, bControl );
			if( dwResult != FSC_SKILLUSE_SUCCESS ) {
				pUser->AddHdr( GETID( pUser ), SNAPSHOTTYPE_CLEAR_USESKILL );
				return;
			}
		}
	}
}
/*
void CDPSrvr::OnDoCollect( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
}
*/
// Ŭ���̾�Ʈ�� ���� ���� �����ڵ带 �α׷� �����.
void CDPSrvr::OnError( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE /*lpBuf*/, u_long /*uBufSize*/)
{
	FLWSUser* pUser	= g_xWSUserManager->GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		int		nCode, nData;
		ar >> nCode >> nData;
		switch( nCode )
		{
		case FE_GENERAL:
			break;
		case FE_INVALIDATTACKER:	// Ŭ���̾�Ʈ���� ����Ŀ�� invalid�Ѱ��(���ɸ� ����)
			{
#ifndef _DEBUG
				OBJID idAttacker = (OBJID)nData;
				CMover *pAttacker = prj.GetMover( idAttacker );
				if( IsValidObj(pAttacker) )
				{
					FLERROR_LOG( PROGRAM_NAME, _T( "FE_INVALIDATTACKER ������:%s(%f,%f,%f), ������:%s(%f,%f,%f)" ), pUser->GetName(), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z,
						pAttacker->GetName(), pAttacker->GetPos().x, pAttacker->GetPos().y, pAttacker->GetPos().z );
					pUser->AddCorrReq( pAttacker );	// ��û�� Ŭ�󿡰� �ι븮���� ����Ŀ�� �ٽ� ������.
				}
				else
					FLERROR_LOG( PROGRAM_NAME, _T( "FE_INVALIDATTACKER ������:%s(%f,%f,%f) �̷����� ���������� pAttacker�� Invalid��. 0x%08x" ),
					pUser->GetName(), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z,
					nData );
				
#endif // not debug
				//		case NEXT:
				
			}
			break;
		}
	} else
		FLERROR_LOG( PROGRAM_NAME, _T( "pUser - Invalid %d %d" ), dpidCache, dpidUser );
}

void CDPSrvr::OnShipActMsg( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE /*lpBuf*/, u_long /*uBufSize*/ )
{
	DWORD dwMsg;
	OBJID idShip;
	int nParam1, nParam2;
	ar >> dwMsg >> nParam1 >> nParam2 >> idShip;
	
	FLWSUser* pUser	= g_xWSUserManager->GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( IsInvalidObj( pUser->GetIAObjLink() ) )		return;
		if( idShip != pUser->GetIAObjLink()->GetId() )
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "Ŭ�󿡼� ������ ���̵�(%d)�� ���������� ���̵�(%d)�� �ٸ���" ), idShip, pUser->GetIAObjLink()->GetId() );
			return;
		}
		
		CShip *pShip = prj.GetShip( idShip );
		if( IsValidObj( pShip ) )
		{
			pShip->SendActMsg( (OBJMSG)dwMsg, nParam1, nParam2, 0 );
			//			g_DPCoreClient.SendShipActMsg( pUser, dwMsg, nParam1, nParam2 );
			g_xWSUserManager->AddShipActMsg( pUser, pShip, dwMsg, nParam1, nParam2 );
		}
	}
}

void CDPSrvr::OnLocalPosFromIA( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE /*lpBuf*/, u_long /*uBufSize*/ )
{
	D3DXVECTOR3 vLocal;
	OBJID		idIA;
	ar >> vLocal >> idIA;
	
	FLWSUser* pUser	= g_xWSUserManager->GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		// Ŭ���̾�Ʈ�κ��� ������������ �����ǥ�� �޾Ҵ�.
		// �� ��ǥ�� �������� ����ȭ ����
		CShip *pIA = prj.GetShip( idIA );
		if( IsInvalidObj( pIA ) )	return;

		
		D3DXVECTOR3 vPos = pIA->GetPos() + vLocal;		// �����󿡼��� IA������Ʈ�� Ŭ�󿡼� ���� ������ǥ�� ���ļ� ���ο� ��ǥ����
		pUser->SetPos( vPos );
		pUser->SetIAObjLink( pIA );
	}
}