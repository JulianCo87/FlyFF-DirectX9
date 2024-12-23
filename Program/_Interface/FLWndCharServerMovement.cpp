#include "StdAfx.h"
#include "FLWndCharServerMovement.h"
#include "../Neuz/dploginclient.h"
#include "../_Common/FLServerDescription.h"
extern	CDPLoginClient	g_dpLoginClient;


//--------------------------------------------------------------------------------------------------------
//					 ĳ���� ���� ���� [���ǻ���]
//						2011/08/22
//--------------------------------------------------------------------------------------------------------
CFLWndCharServerMovement_Warning::CFLWndCharServerMovement_Warning()
{

}

//---------------------------------------------------------------------------------------------------------
CFLWndCharServerMovement_Warning::~CFLWndCharServerMovement_Warning()
{

}
//---------------------------------------------------------------------------------------------------------
void CFLWndCharServerMovement_Warning::SetItemID(OBJID dwObjId,DWORD dwItemId)
{
	m_dwObjId = dwObjId;
	m_dwItemId = dwItemId;
}

//---------------------------------------------------------------------------------------------------------
void CFLWndCharServerMovement_Warning::OnInitialUpdate()
{
	CWndNeuz::OnInitialUpdate(); 
	CWndText* pWndText = NULL;

	pWndText = (CWndText*)GetDlgItem( WIDC_TEXT1 );
	
	if(	!pWndText )
	{	
		FLERROR_LOG( PROGRAM_NAME, _T( "CFLWndCharServerMovement_Warning::OnInitialUpdate()" ) );
		return;
	}

	CScript scanner;
	if( scanner.Load( "servermoveinfo.inc" ) == FALSE )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "NoFile : [servermoveinfo.inc]" ) );
		return;
	}
	
	pWndText->m_string.AddParsingString( scanner.m_pProg );
	pWndText->ResetString();	

	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
}
//---------------------------------------------------------------------------------------------------------
BOOL CFLWndCharServerMovement_Warning::Initialize(CWndBase* pWndParent, DWORD dwWndId)
{
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), dwWndId, WBS_MODAL, CPoint( 0, 0 ), pWndParent );
}
//---------------------------------------------------------------------------------------------------------
BOOL CFLWndCharServerMovement_Warning::OnChildNotify( UINT message,UINT nID,LRESULT* pLResult )
{
	switch( nID )
	{
	case WIDC_BT_OK:
		g_DPlay.SendDoUseItem( MAKELONG( ITYPE_ITEM, m_dwObjId ), g_pPlayer->GetId());
		Destroy();
		break;

	case WIDC_BT_CANCEL:
		Destroy();
		break;
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
}
//---------------------------------------------------------------------------------------------------------
void CFLWndCharServerMovement_Warning::OnDraw( C2DRender* p2DRender )
{

}



//--------------------------------------------------------------------------------------------------------
//					 ĳ���� ���� �̵� [�̵��� ���� ����]
//						2011/08/22
//--------------------------------------------------------------------------------------------------------
CFLWndCharServerMovement_Select::CFLWndCharServerMovement_Select()
{

}
//---------------------------------------------------------------------------------------------------------
CFLWndCharServerMovement_Select::~CFLWndCharServerMovement_Select()
{

}

//---------------------------------------------------------------------------------------------------------
void CFLWndCharServerMovement_Select::OnInitialUpdate()
{
	CWndNeuz::OnInitialUpdate(); 

	m_pWndListServer		= (CWndListBox*)GetDlgItem( WIDC_LB_WORLD );//�����
	m_pWndListMove			= (CWndText*)GetDlgItem( WIDC_TEXT1 );//�̵�����

	
	if( !( m_pWndListServer&& 
			m_pWndListMove) )
	{	
		FLERROR_LOG( PROGRAM_NAME, _T( "CFLWndCharServerMovement_Select::OnInitialUpdate()" ) );
		return;
	}
	CString szWorldName;
	CString szMoveCharacter;

	g_pServerDescription->GetCanConnectionServerDescription( &m_vecServerDesc );

	for( ServerDescCItr kCItr = m_vecServerDesc.begin(); kCItr != m_vecServerDesc.end(); ++kCItr )
	{
		char lpString[MAX_PATH]	= { 0, };
		char lpStrtmp[32]	= { 0, };
		long lCount	= 0;
		long lMax	= 0;
		
		szWorldName.Format("%s",kCItr->szName);

		if( kCItr->dwMyID != g_Neuz.m_dwSys )
		{
			if( kCItr->lCanConnection != 0L )
			{
				if(kCItr->bTransformCharacter == true )
				{
					szMoveCharacter.Format("  %s\n",prj.GetText( TID_MMI_SMELTING01_TEXT ) );
				}
				else 
				{
					szMoveCharacter.Format("  %s\n",prj.GetText( TID_MMI_SMELTING02_TEXT ) );
				}
			}
			
		}
		else 
		{
			szMoveCharacter.Format("  -\n");
		}
	
		int nIndex	= m_pWndListServer->AddString( szWorldName );
		m_pWndListServer->SetItemData( nIndex, (DWORD)&( *kCItr ) );
		m_pWndListMove->AddString( szMoveCharacter );
	
		m_pWndListServer->SetOnSelectColor(D3DCOLOR_ARGB( 255, 45, 107, 180 ));
		m_pWndListServer->SetOnMouseColor(D3DCOLOR_ARGB( 255, 45, 107, 180 ));
		m_pWndListServer->SetCurSel(0);
		m_pWndListServer->SetStyleState(TRUE);
	}
	
	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
}
//---------------------------------------------------------------------------------------------------------
BOOL CFLWndCharServerMovement_Select::Initialize(CWndBase* pWndParent, DWORD dwWndId)
{
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), dwWndId, WBS_MODAL, CPoint( 0, 0 ), pWndParent );
}
//---------------------------------------------------------------------------------------------------------
BOOL CFLWndCharServerMovement_Select::OnChildNotify( UINT message,UINT nID,LRESULT* pLResult )
{
	switch( nID )
	{
	case WIDC_BT_OK:
        SelectWorld();        
		break;	
	case WIDC_BT_CANCEL:
		Destroy();
		break;
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
}
//---------------------------------------------------------------------------------------------------------
void CFLWndCharServerMovement_Select::OnLButtonUp(UINT nFlags, CPoint point)
{
}

//---------------------------------------------------------------------------------------------------------
void CFLWndCharServerMovement_Select::OnDraw( C2DRender* p2DRender )
{

}
//---------------------------------------------------------------------------------------------------------
void CFLWndCharServerMovement_Select::SelectWorld()
{
	if(m_vecServerDesc.empty()) return;

	if(!m_vecServerDesc.at( m_pWndListServer->GetCurSel() ).bTransformCharacter || m_vecServerDesc.at( m_pWndListServer->GetCurSel() ).dwMyID == g_Neuz.m_dwSys )
	{
		g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_WORLDCHANGEERROR_TEXT06) );
		return;
	}
	else 
	{
		DWORD dwWorldID = m_vecServerDesc.at( m_pWndListServer->GetCurSel() ).dwMyID;
		FLPacketCharacterServerTransformReq pkPacket;
		pkPacket.dwItemObjID = m_dwItemObjID;
		pkPacket.dwTransformServerIndex = dwWorldID;
		g_DPlay.SendPacket(&pkPacket);
		
		//���� ��Ŷ ������ ��ư�� �� ��Ȱ��
		CWndButton* pBtnOK  = ( CWndButton* )GetDlgItem( WIDC_BT_OK );
		if( pBtnOK )
			pBtnOK->EnableWindow( FALSE );
		CWndButton* pBtnCANCEL  = ( CWndButton* )GetDlgItem( WIDC_BT_CANCEL );
		if( pBtnCANCEL )
			pBtnCANCEL->EnableWindow( FALSE );
	}

}

//---------------------------------------------------------------------------------------------------------
void CFLWndCharServerMovement_Select::ReceivedPacket( FLSnapshotCharacterServerTransformNoti* pkcharServer)
{
	if(pkcharServer->dwItemObjID != NULL_ID)
	{
		m_dwItemObjID = pkcharServer->dwItemObjID;
		Initialize( &g_WndMng, APP_SERVERMOVE_SELECT );
	}
}

//---------------------------------------------------------------------------------------------------------
void CFLWndCharServerMovement_Select::ReceivedPacket_LogOut( FLPacketCharacterServerTransformAck *pkPacketLogOut )
{
	g_dpLoginClient.RemoteShowDisconnectMsg( FALSE );
	//Ȯ�� �޼����� �Բ� �α��� ȭ������ �� 
	g_WndMng.OpenCustomBox( NULL, new CWndLogOut(pkPacketLogOut->szMessage,CWndLogOut::LOGOUT_TYPE2) );

}

