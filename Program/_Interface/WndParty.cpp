#include "stdafx.h"
#include "WndParty.h"
#include "WndManager.h"
#include "Party.h"
#include "playerdata.h"		//sun: 11, ĳ���� ���� ����

/****************************************************
  WndId : APP_PARTY - �ش�(Party)
  CtrlId : WIDC_TABCTRL1 - TabCtrl
  CtrlId : WIDC_LEAVE - Ż��
  CtrlId : WIDC_CHANGE - �ش���ȯ
****************************************************/

CWndParty::CWndParty()

{ 
	m_WndPartyChangeTroup = NULL;
	m_pWndPartyQuick = NULL;			//sun: 8�� ��Ƽâ ���� ���� Neuz
	FLINFO_LOG( PROGRAM_NAME, _T( "CWndParty::CWndParty()" ) );
} 
CWndParty::~CWndParty() 
{ 
	SAFE_DELETE( m_WndPartyChangeTroup );

	DeleteDeviceObjects();
} 
void CWndParty::SerializeRegInfo( CAr& ar, DWORD& dwVersion )
{
	FLINFO_LOG( PROGRAM_NAME, _T( "IN" ) );

	CWndNeuz::SerializeRegInfo( ar, dwVersion );
	CWndTabCtrl* lpTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
	if( ar.IsLoading() )
	{
		if( dwVersion == 0 )
		{
		}
		else
		if( dwVersion == 1 )
		{
			int nCurSel;
			ar >> nCurSel;
			lpTabCtrl->SetCurSel( nCurSel );
		}
	}
	else
	{
		dwVersion = 1;
		ar << lpTabCtrl->GetCurSel();
	}

	FLINFO_LOG( PROGRAM_NAME, _T( "OUT" ) );
}
void CWndParty::OnDraw( C2DRender* p2DRender ) 
{ 
	if( g_Party.GetSizeofMember() < 2 )
	{
		m_pWndLeave->EnableWindow( FALSE );
		m_pWndTransfer->EnableWindow( FALSE );
		m_pBtnPartyQuick->EnableWindow( FALSE );	//sun: 8�� ��Ƽâ ���� ���� Neuz
	}
	else
	{
		m_pWndLeave->EnableWindow( TRUE );
		m_pWndTransfer->EnableWindow( TRUE );
		m_pBtnPartyQuick->EnableWindow( TRUE );		//sun: 8�� ��Ƽâ ���� ���� Neuz
	}

	if( g_Party.GetSizeofMember() < 2 && g_Party.m_nLevel > 1 )
	{
		m_pWndLeave->EnableWindow( TRUE );
	}

	if( g_Party.GetLevel() >= 10 && g_Party.IsLeader( g_pPlayer->m_idPlayer ) )
		m_pWndChange->EnableWindow( TRUE );
	else
		m_pWndChange->EnableWindow( FALSE );

	// ��Ƽ ���� ���
	CWndStatic* pWndStatic;
	CString strTemp;
	if( g_Party.GetKind()== PARTY_KIND_BASE )
	{
		strTemp.Format("%s",prj.GetText(TID_GAME_PARTY1));
	}
	else 
	{
		if( 0 == strlen( g_Party.m_sParty ) )
			strTemp.Format("%s",prj.GetText(TID_GAME_PARTY2));
		else
			strTemp.Format("%s",g_Party.m_sParty);
	}
	pWndStatic = (CWndStatic*)GetDlgItem( WIDC_NAME );
	pWndStatic->SetTitle(strTemp);
	strTemp.Format("%d",g_Party.m_nLevel);
	pWndStatic = (CWndStatic*)GetDlgItem( WIDC_LEVEL );
	pWndStatic->SetTitle(strTemp);
	
	if( g_Party.m_nLevel >= 10 )
	{
		strTemp.Format("%.2f %%", (float)g_Party.m_nExp * 100 / (float)( ( ( 50 + g_Party.GetLevel() ) * g_Party.GetLevel() / 13 ) * 10 ));
	}
	else
	{
		strTemp.Format("%.2f %%", (float)g_Party.m_nExp * 100 / (float)prj.m_aExpParty[g_Party.m_nLevel].Exp );
	}
	pWndStatic = (CWndStatic*)GetDlgItem( WIDC_EXP );
	pWndStatic->SetTitle(strTemp);

	//gmpbigsun( 20120327 ) : ������ ǥ��
	if( g_Party.IsActivatedEffect( PARTY_EFFECT_POINT_LIMITLESS ) )
		strTemp = CString( prj.GetText( TID_MMI_PARTYPOINTINFINITE ) );
	else
		strTemp.Format("%d",g_Party.m_nPoint);

	pWndStatic = (CWndStatic*)GetDlgItem( WIDC_POINT );
	pWndStatic->SetTitle(strTemp);

	// ������ ����ġ �й��� ���� ����
	CWndButton* pWndButton;
	switch( g_Party.m_nTroupsShareExp) 
	{
	case 0:
		{
			pWndButton = (CWndButton*)GetDlgItem( WIDC_EXP_SHARE );
			pWndButton->SetCheck(TRUE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO6 );
			pWndButton->SetCheck(FALSE);
//			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO7 );
//			pWndButton->SetCheck(FALSE);
		}
		break;
	case 1:
		{
			pWndButton = (CWndButton*)GetDlgItem( WIDC_EXP_SHARE );
			pWndButton->SetCheck(FALSE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO6 );
			pWndButton->SetCheck(TRUE);
//			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO7 );
//			pWndButton->SetCheck(FALSE);
		}
		break;
	case 2:
		{
			pWndButton = (CWndButton*)GetDlgItem( WIDC_EXP_SHARE );
			pWndButton->SetCheck(FALSE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO6 );
			pWndButton->SetCheck(FALSE);
//			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO7 );
//			pWndButton->SetCheck(TRUE);
		}
		break;
	}
	switch(g_Party.m_nTroupeShareItem) 
	{
	case 0:
		{
			pWndButton = (CWndButton*)GetDlgItem( WIDC_ITEM_SHARE );
			pWndButton->SetCheck(TRUE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO2 );
			pWndButton->SetCheck(FALSE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO3 );
			pWndButton->SetCheck(FALSE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO4 );
			pWndButton->SetCheck(FALSE);
		}
		break;
	case 1:
		{
			pWndButton = (CWndButton*)GetDlgItem( WIDC_ITEM_SHARE );
			pWndButton->SetCheck(FALSE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO2 );
			pWndButton->SetCheck(TRUE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO3 );
			pWndButton->SetCheck(FALSE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO4 );
			pWndButton->SetCheck(FALSE);
		}
		break;
	case 2:
		{
			pWndButton = (CWndButton*)GetDlgItem( WIDC_ITEM_SHARE );
			pWndButton->SetCheck(FALSE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO2 );
			pWndButton->SetCheck(FALSE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO3 );
			pWndButton->SetCheck(TRUE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO4 );
			pWndButton->SetCheck(FALSE);
		}
		break;
	case 3:
		{
			pWndButton = (CWndButton*)GetDlgItem( WIDC_ITEM_SHARE );
			pWndButton->SetCheck(FALSE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO2 );
			pWndButton->SetCheck(FALSE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO3 );
			pWndButton->SetCheck(FALSE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO4 );
			pWndButton->SetCheck(TRUE);
		}
		break;
	}
} 
void CWndParty::OnInitialUpdate() 
{ 
	FLINFO_LOG( PROGRAM_NAME, _T( "IN" ) );

	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	
	m_pWndLeave = (CWndButton*)GetDlgItem( WIDC_LEAVE );
	m_pWndChange = (CWndButton*)GetDlgItem( WIDC_CHANGE );
	m_pWndTransfer = (CWndButton*)GetDlgItem( WIDC_BUTTON1 );
	m_pBtnPartyQuick = (CWndButton*)GetDlgItem( WIDC_BUTTON2 );		//sun: 8�� ��Ƽâ ���� ���� Neuz

	//COMMENT BEGIN ( ver<8 )
	//m_pBtnPartyQuick = (CWndButton*)GetDlgItem( WIDC_BUTTON2 );
	//m_pBtnPartyQuick->EnableWindow(FALSE);
	//m_pBtnPartyQuick->SetVisible(FALSE);
	//COMMENT END

	if( g_Party.GetSizeofMember() < 2 )
	{
		m_pWndTransfer->EnableWindow( FALSE );
		m_pWndLeave->EnableWindow( FALSE );
		m_pBtnPartyQuick->EnableWindow( FALSE );		//sun: 8�� ��Ƽâ ���� ���� Neuz
	}
	m_pWndChange->EnableWindow( FALSE );
	
	CWndTabCtrl* pWndTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );

	if(pWndTabCtrl)
	{
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
			pWndTabCtrl->SetTabTitleAlign( CWndTabCtrl::ALIGN_CENTER );
	}

	CRect rect = GetClientRect();
	rect.left = 5;
	rect.top = 15;

	//gmpbigsun( 100416 ) : Error ���� 
	//FLINFO_LOG( PROGRAM_NAME, _T( "m_wndPartyInfo.Create - IN" ) );
	m_wndPartyInfo.Create( WBS_CHILD | WBS_NOFRAME | WBS_NODRAWFRAME, rect, pWndTabCtrl, APP_PARTY_INFO );
	//FLINFO_LOG( PROGRAM_NAME, _T( "m_wndPartyInfo.Create - OUT" ) );

	//FLINFO_LOG( PROGRAM_NAME, _T( "m_wndPartySkill.Create - IN" ) );
	m_wndPartySkill.Create( WBS_CHILD | WBS_NOFRAME | WBS_NODRAWFRAME , rect, pWndTabCtrl, APP_PARTY_SKILL );
	//FLINFO_LOG( PROGRAM_NAME, _T( "m_wndPartySkill.Create - OUT" ) );

	WTCITEM tabTabItem;
	
	tabTabItem.mask = WTCIF_TEXT | WTCIF_PARAM;
	tabTabItem.pszText = prj.GetText(TID_APP_INFOMATION);//"����";
	tabTabItem.pWndBase = &m_wndPartyInfo;
	pWndTabCtrl->InsertItem( 0, &tabTabItem );
	
	tabTabItem.pszText = prj.GetText(TID_APP_SKILL);//"��ų";
	tabTabItem.pWndBase = &m_wndPartySkill;
	pWndTabCtrl->InsertItem( 1, &tabTabItem );

	MoveParentCenter();

	FLINFO_LOG( PROGRAM_NAME, _T( "OUT" ) );
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndParty::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	FLINFO_LOG( PROGRAM_NAME, _T( "IN" ) );
	BOOL bFlag = CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_PARTY, 0, CPoint( 0, 0 ), pWndParent );
	FLINFO_LOG( PROGRAM_NAME, _T( "OUT" ) );

	// Daisy���� ������ ���ҽ��� ������ ����.
	return bFlag;
} 
/*
  ���� ������ ���� ��� 
BOOL CWndParty::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndParty::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndParty::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndParty::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndParty::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndParty::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_LEAVE ) 
	{ // Ż�� ���ȴ�...
		if( g_pPlayer->m_nDuel == 2 )		// �شܵ�����̸� Ż�� �ȵ�.
		{
			DWORD dwText = TID_GAME_PPVP_LEAVE;
			g_WndMng.PutString( prj.GetText( dwText ), NULL, prj.GetTextColor( dwText ) );
		} 
		else
		{
			if(m_wndPartyInfo.m_nSelected!=-1) 
			{
				u_long  nLeaveMember = g_Party.m_aMember[m_wndPartyInfo.m_nSelected].m_uPlayerId; //Ż���ų�� ID
				// ����ٰ� Ż��
				if( g_Party.m_nSizeofMember != 0 )
				{
					CWndPartyLeaveConfirm* pWndPartyLeaveConfirm = (CWndPartyLeaveConfirm*)g_WndMng.GetWndBase( APP_PARTYLEAVE_CONFIRM );
					if( pWndPartyLeaveConfirm == NULL )
					{
						g_WndMng.m_pWndPartyLeaveConfirm = new CWndPartyLeaveConfirm;
						g_WndMng.m_pWndPartyLeaveConfirm->Initialize( NULL, APP_PARTYLEAVE_CONFIRM );
						if( g_Party.m_aMember[0].m_uPlayerId == g_pPlayer->m_idPlayer )
							g_WndMng.m_pWndPartyLeaveConfirm->SetLeaveId( nLeaveMember );
						else
							g_WndMng.m_pWndPartyLeaveConfirm->SetLeaveId( g_pPlayer->m_idPlayer );
					}
				}
				else
				{
					// �ش��� ������ �ȵǾ�����
				}
			}
			else
			{
				CWndPartyLeaveConfirm* pWndPartyLeaveConfirm = (CWndPartyLeaveConfirm*)g_WndMng.GetWndBase( APP_PARTYLEAVE_CONFIRM );
				if( pWndPartyLeaveConfirm == NULL )
				{
					g_WndMng.m_pWndPartyLeaveConfirm = new CWndPartyLeaveConfirm;
					g_WndMng.m_pWndPartyLeaveConfirm->Initialize( NULL, APP_PARTYLEAVE_CONFIRM );
					if( g_Party.m_nSizeofMember != 0 )
						g_WndMng.m_pWndPartyLeaveConfirm->SetLeaveId( g_pPlayer->m_idPlayer );
				}
				// �� �ڽ��� �شܿ��� Ż��
			}
			m_wndPartyInfo.m_nSelected=-1;
		}
	}
	// �ش��� �ΰ�.
	else if( nID==WIDC_BUTTON1 )
	{
		if( g_Party.m_nSizeofMember != 0 )
		{
			if( g_Party.IsLeader( g_pPlayer->m_idPlayer ) && m_wndPartyInfo.m_nSelected != -1 )
			{
				if( g_Party.m_aMember[m_wndPartyInfo.m_nSelected].m_bRemove == FALSE )
				{
					u_long  nLeadMember = g_Party.m_aMember[m_wndPartyInfo.m_nSelected].m_uPlayerId;
					if( g_Party.IsLeader( nLeadMember ) == FALSE )
					{
						if( g_pPlayer->m_nDuel == 2 )	// �ش� ������϶��� �ش����� �ٲܼ� ����
							g_WndMng.PutString( prj.GetText( TID_PK_NOCHANGE_PARTYLEADER ), NULL, prj.GetTextColor( TID_PK_NOCHANGE_PARTYLEADER ) );
						else
							g_DPlay.SendPartyChangeLeader( g_pPlayer->m_idPlayer, nLeadMember );						
					}
					else
					{
						g_WndMng.PutString( prj.GetText( TID_GAME_PARTY_ALREADYMASTER ), NULL, prj.GetTextColor( TID_GAME_PARTY_ALREADYMASTER ) ); // "�̹� ������ �ϰ� �Դϴ�"
					}
				}
				else
				{
					g_WndMng.PutString( prj.GetText( TID_GAME_MASTER_AWAY ), NULL, prj.GetTextColor( TID_GAME_MASTER_AWAY ) ); // "������ �ƴϰų� �ش�â���� �ܿ��� ������ �ؾ� �մϴ�"					
				}
			}
			else
			{
				g_WndMng.PutString( prj.GetText( TID_GAME_PARTY_ISNOTMASTER ), NULL, prj.GetTextColor( TID_GAME_PARTY_ISNOTMASTER ) ); // "������ �ƴϰų� �ش�â���� �ܿ��� ������ �ؾ� �մϴ�"
			}
		}
	}
	else if( nID==WIDC_CHANGE )
	{
		if( g_Party.GetKind() == PARTY_KIND_BASE /* && g_Party.m_nLevel == 10 */ )
		{
			// ��ȸ�ش����� �ٲٱ�~
			SAFE_DELETE( m_WndPartyChangeTroup );
			m_WndPartyChangeTroup = new CWndPartyChangeTroup;
			m_WndPartyChangeTroup->Initialize( this );
		}
	}
	//sun: 8�� ��Ƽâ ���� ���� Neuz
	else if(nID == WIDC_BUTTON2)
	{
		if(g_Party.GetSizeofMember() >= 2)
		{
			m_pWndPartyQuick = (CWndPartyQuick*)g_WndMng.GetApplet( APP_PARTY_QUICK );
			if(m_pWndPartyQuick == NULL)
			{
				m_pWndPartyQuick = (CWndPartyQuick*)g_WndMng.CreateApplet( APP_PARTY_QUICK );
				m_pWndPartyQuick->SetActiveMember(g_Party.GetSizeofMember());
			}
			else
			{
				m_pWndPartyQuick->Destroy();
				m_pWndPartyQuick = NULL;
			}
		}
	}
	// ��ȸ�ش��϶��� ������ ����ġ �й��� ����
	else
	if( g_Party.IsLeader(g_pPlayer->m_idPlayer) )
	{
		if( g_Party.GetKind() == PARTY_KIND_EXTEND )
		{
			// ����ġ �й� ����� ��ȸ�ش� ��
			int nTroupsShareExp = g_Party.m_nTroupsShareExp;
			
			if(nID==WIDC_EXP_SHARE) nTroupsShareExp   = 0;
			else if(nID==WIDC_RADIO6) nTroupsShareExp = 1;
	//		else if(nID==WIDC_RADIO7) nTroupsShareExp = 2;
			
			if( nTroupsShareExp == 2 )
			{
				g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0004) ) );
				//			g_WndMng.OpenMessageBox( _T("����ġ �й��� ���Ϻй�� ���� ������ �ȵ˴ϴ�.") );
			}
			else
			{
				if( nTroupsShareExp != g_Party.m_nTroupsShareExp )
				{
					// Send
					g_DPlay.SendChangeShareExp( nTroupsShareExp );
				}
			}
		}
		// ������ �й� ����� �ܸ��ش� ��
		int nTroupeShareItem = g_Party.m_nTroupeShareItem;

		if(nID==WIDC_ITEM_SHARE) nTroupeShareItem  = 0;
		else if(nID==WIDC_RADIO2) nTroupeShareItem = 1;
		else if(nID==WIDC_RADIO3) nTroupeShareItem = 2;
		else if(nID==WIDC_RADIO4) nTroupeShareItem = 3;		

		if( nTroupeShareItem != g_Party.m_nTroupeShareItem )
		{
			// Send
			g_DPlay.SendChangeShareItem( nTroupeShareItem );
			
		}
	}
	else
	{
		if( nID != WTBID_CLOSE && nID != WTBID_HELP )
			g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0008) ) );
//			g_WndMng.OpenMessageBox( "������ �ƴϰų� ��ȸ�ش��� �ƴϹǷ� �����Ҽ� �����ϴ�." );
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
}


#pragma region WndPartyInfo


/****************************************************
  WndId		: APP_PARTY_INFO - ���κй�
  CtrlId 	: WIDC_GROUPBOX1 - �ش�����
  CtrlId 	: WIDC_GROUPBOX3 - ����ġ�й�
  CtrlId 	: WIDC_GROUPBOX4 - �����ۺй�
  CtrlId 	: WIDC_ITEM_SHARE - ���κй�
  CtrlId 	: WIDC_RADIO2 - �����й�
  CtrlId 	: WIDC_RADIO3 - �����й�
  CtrlId 	: WIDC_RADIO4 - �����й�
  CtrlId 	: WIDC_EXP_SHARE - ���Ϻй�
  CtrlId 	: WIDC_RADIO6 - �⿩���й�
  CtrlId 	: WIDC_RADIO7 - ���غй�
  CtrlId 	: WIDC_STATIC1 - �شܸ�Ī :
  CtrlId 	: WIDC_STATIC2 - �شܷ��� :
  CtrlId 	: WIDC_STATIC3 - �شܰ���ġ :
  CtrlId 	: WIDC_STATIC4 - �ش�����Ʈ :
  CtrlId 	: WIDC_NAME - ����Ŀ���̵�
  CtrlId 	: WIDC_LEVEL - 1
  CtrlId 	: WIDC_EXP - 90
  CtrlId 	: WIDC_POINT - 0
****************************************************/


//---------------------------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------------------------
CWndPartyInfo::CWndPartyInfo() 
:	m_nSelected( -1 )
,	m_pVBGauge( NULL )
,	m_pGauEmptyNormal( NULL ) 
,	m_pGauFillNormal( NULL )
{ 
} 


//---------------------------------------------------------------------------------------------
// �ı���
//---------------------------------------------------------------------------------------------
CWndPartyInfo::~CWndPartyInfo() 
{ 
	DeleteDeviceObjects();
} 

//---------------------------------------------------------------------------------------------
// ��ġ ������Ʈ ȸ���ϱ�
// param	: void
// return	: void 
//---------------------------------------------------------------------------------------------
HRESULT CWndPartyInfo::RestoreDeviceObjects()
{
	CWndNeuz::RestoreDeviceObjects();

	if( m_pVBGauge == NULL )
		return m_pApp->m_pd3dDevice->CreateVertexBuffer( sizeof( TEXTUREVERTEX2 ) * 3 * 6, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_TEXTUREVERTEX2, D3DPOOL_DEFAULT, &m_pVBGauge, NULL );

	return S_OK;
}


//---------------------------------------------------------------------------------------------
// ��ġ ������Ʈ ��ȿȭ
// param	: void
// return	: void 
//---------------------------------------------------------------------------------------------
HRESULT CWndPartyInfo::InvalidateDeviceObjects()
{
	CWndNeuz::InvalidateDeviceObjects();
	SAFE_RELEASE( m_pVBGauge );

	return S_OK;
}


//---------------------------------------------------------------------------------------------
// ��ġ ������Ʈ ����
// param	: void
// return	: void 
//---------------------------------------------------------------------------------------------
HRESULT CWndPartyInfo::DeleteDeviceObjects()
{
	CWndNeuz::DeleteDeviceObjects();
	InvalidateDeviceObjects();

	return S_OK;
}


//---------------------------------------------------------------------------------------------
// �ʱ�ȭ
// param	: �θ� ������, ������ ���̵�
// return	: void 
//---------------------------------------------------------------------------------------------
BOOL CWndPartyInfo::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_PARTY_INFO, 0, CPoint( 0, 0 ), pWndParent );
} 


//---------------------------------------------------------------------------------------------
// ������Ʈ �ʱ�ȭ
// param	: void
// return	: void 
//---------------------------------------------------------------------------------------------
void CWndPartyInfo::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 

	RestoreDeviceObjects();

	if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
	{
		m_texGauEmptyNormal.LoadTexture(	m_pApp->m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), _T("GauEmptyNormal.bmp") ), WNDCOLOR_DEFAULT_KEY, TRUE );
		m_texGauFillNormal.LoadTexture(		m_pApp->m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), _T("GauEmptyNormal.bmp") ), WNDCOLOR_DEFAULT_KEY, TRUE );
	}
	if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
	{
		m_pGauEmptyNormal	= m_textureMng.AddTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), _T("BarBg.tga") ), WNDCOLOR_DEFAULT_KEY, TRUE );
		m_pGauFillNormal	= m_textureMng.AddTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), _T("BarRed.tga") ), WNDCOLOR_DEFAULT_KEY, TRUE );
	}

	CRect	rectRoot( m_pWndRoot->GetLayoutRect() );
	CRect	rectWindow( GetWindowRect() );
	CPoint	point( rectRoot.right - rectWindow.Width(), 110 );

	Move( point );
	MoveParentCenter();
} 


//---------------------------------------------------------------------------------------------
// ���� üũ
// param	: void
// return	: BOOL 
//---------------------------------------------------------------------------------------------
BOOL CWndPartyInfo::CheckSummary()
{
	BOOL bSummary( FALSE );
	int nLang = g_xFlyffConfig->GetMainLanguage();

	switch( nLang )
	{
		case LANG_GER:
		case LANG_FRE:
			bSummary = TRUE;
			break;
	}

	return bSummary;
}


//---------------------------------------------------------------------------------------------
// ���¿� ���� ���� ���
// param	: ���, ��� �ε���
// return	: void 
//---------------------------------------------------------------------------------------------
DWORD CWndPartyInfo::GetFillColorofStatus( CMover* pMember, UINT nMemberIndex )
{
	PFLCOLOR_WNDPARTYINFO	pColorPartyInfo( g_FLColorManager->GetWndPartyInfo() ); 
	DWORD					dwFillColor( pColorPartyInfo->m_stStatusBoxFill_Normal.GetColor() );	// �⺻

	if( IsValidObj( pMember ) )
	{
		if( pMember->GetHitPoint() == 0 ) 
			dwFillColor = pColorPartyInfo->m_stStatusBoxFill_Die.GetColor();						// ����
		else 
		{
			if( pMember->GetMaxHitPoint() > 0 && ( (FLOAT)pMember->GetHitPoint() ) / ( (FLOAT)pMember->GetMaxHitPoint() ) <.1f ) 
				dwFillColor = pColorPartyInfo->m_stStatusBoxFill_LowHP.GetColor();					// HP ����
		}
	}
	else
	{
		dwFillColor = pColorPartyInfo->m_stStatusBoxFill_NotFoundFromAround.GetColor();				// ������ ����

		if( g_Party.m_aMember[nMemberIndex].m_bRemove ) 
			dwFillColor = pColorPartyInfo->m_stStatusBoxFill_Disconnect.GetColor();
	}

	return dwFillColor;
}


//---------------------------------------------------------------------------------------------
// ���� �ڽ� �׸���
// param	: ������, ���, ��� �ε���, ���� ������ġ
// return	: void 
//---------------------------------------------------------------------------------------------
void CWndPartyInfo::DrawStatusBox( C2DRender* p2DRender, CMover* pMember, UINT nMemberIndex, UINT nPosY )
{
	BYTE					byBoxPosX( 10 );
	BYTE					byBoxSideSize( 10 );

	CRect					rtBox( byBoxPosX, nPosY, byBoxPosX + byBoxSideSize, nPosY + byBoxSideSize );

	// �ƿ� ���� �׸���
	PFLCOLOR_WNDPARTYINFO	pColorPartyInfo( g_FLColorManager->GetWndPartyInfo() ); 
	p2DRender->RenderFillRect( rtBox, pColorPartyInfo->m_stStatusBoxOutLine.GetColor() );	

	rtBox.left		= rtBox.left	+ 1;
	rtBox.top		= rtBox.top		+ 1;
	rtBox.right		= rtBox.right	- 1;
	rtBox.bottom	= rtBox.bottom	- 1;

	// ���� üũ
	DWORD dwFillColor = GetFillColorofStatus( pMember, nMemberIndex );
	p2DRender->RenderFillRect( rtBox, dwFillColor );
}


//---------------------------------------------------------------------------------------------
// ��� ���� �׸���
// param	: ������, ���, ���� ������ġ
// return	: void 
//---------------------------------------------------------------------------------------------
void CWndPartyInfo::DrawMemberInfo( C2DRender* p2DRender, CMover* pMember, UINT nMemberIndex, UINT nPosY )
{
	PFLCOLOR_WNDPARTYINFO		pColorPartyInfo( g_FLColorManager->GetWndPartyInfo() ); 
	PFONTCOLOR_WNDPARTYINFO		pFontColorWndPartyInfo = g_WndFontColorManager->GetWndPartyInfo();

	CString						strName( _T("") );
	CString						strTemp( _T("") );
	CString						strClass( _T("") );

	DWORD						dwTextColor( m_dwColor );	
	BOOL						bSummary( CheckSummary() );

	static const int			nString1st_PosX( 30 );				
	static const int			nString2nd_PosX( 80 );
	static const int			nString3rd_PosX( 120 );
	static const int			nString4th_PosX( 210 );

	static const BYTE			byPlayerNameMaxCount( 8 );

	// ���� üũ
	if( g_Party.IsLeader( g_Party.m_aMember[nMemberIndex].m_uPlayerId ) ) 
	{
		strClass = prj.GetText( TID_GAME_LEADER );	
		dwTextColor = pFontColorWndPartyInfo->m_stLeader.GetFontColor();
	}
	else
		strClass = prj.GetText( TID_GAME_MEMBER );	

	if( IsValidObj(pMember) )
	{
		if( pMember->GetHitPoint() == 0 ) 
			dwTextColor = pColorPartyInfo->m_stStatusBoxFill_Die.GetColor();						// ����
		else 
		{
			if( pMember->GetMaxHitPoint() > 0 && ( (FLOAT)pMember->GetHitPoint() ) / ( (FLOAT)pMember->GetMaxHitPoint() ) <.1f ) 
				dwTextColor = pColorPartyInfo->m_stStatusBoxFill_LowHP.GetColor();					// HP ����
		}

		if( MAX_PROFESSIONAL <= pMember->GetJob() &&
			pMember->GetJob() < MAX_MASTER )
			strTemp.Format( "%d%s", pMember->GetLevel(), prj.GetText( TID_GAME_TOOLTIP_MARK_MASTER ) );
		else if( MAX_MASTER <= pMember->GetJob() )
			strTemp.Format( "%d%s", pMember->GetLevel(), prj.GetText( TID_GAME_TOOLTIP_MARK_HERO ) );
		else 
			strTemp.Format( "%d", pMember->GetLevel() );

		p2DRender->TextOut( nString1st_PosX, nPosY, 1.0f, 1.0f, strTemp, dwTextColor );				// ����, ���

		if( !bSummary )
		{
			strTemp.Format( "%s", strClass );
			p2DRender->TextOut( nString2nd_PosX, nPosY, 1.0f, 1.0f, strTemp, dwTextColor );			// ����, ���
			strTemp.Format( "%s", pMember->GetJobString() );
			p2DRender->TextOut( nString3rd_PosX, nPosY, 1.0f, 1.0f, strTemp, dwTextColor );			// ����
		}
		else
		{
			strTemp.Format( "%s", pMember->GetJobString() );
			p2DRender->TextOut( nString2nd_PosX, nPosY, 1.0f, 1.0f, strTemp, dwTextColor );			// ����
		}

		strName = pMember->GetName();
	}
	else
	{
		PlayerData* pPlayerData	= CPlayerDataCenter::GetInstance()->GetPlayerData( g_Party.m_aMember[nMemberIndex].m_uPlayerId );
		FLERROR_ASSERT_LOG_RETURN_VOID( pPlayerData, _T("[������ ��Ƽ ����(�ش�)] �÷��̾� ������ ã�� �� �����ϴ� - ID : [ %u]"), g_Party.m_aMember[nMemberIndex].m_uPlayerId );

		dwTextColor = pColorPartyInfo->m_stStatusBoxFill_NotFoundFromAround.GetColor();				// ������ ����

		if( g_Party.m_aMember[nMemberIndex].m_bRemove ) 
			dwTextColor = pColorPartyInfo->m_stStatusBoxFill_Disconnect.GetColor();

		UINT nJob( pPlayerData->data.nJob );
		UINT nLevel( pPlayerData->data.nLevel );

		if( MAX_PROFESSIONAL <= nJob && nJob < MAX_MASTER )
			strTemp.Format( "%d%s", nLevel, prj.GetText( TID_GAME_TOOLTIP_MARK_MASTER ) );
		else if( MAX_MASTER <= nJob )
			strTemp.Format( "%d%s", nLevel, prj.GetText( TID_GAME_TOOLTIP_MARK_HERO ) );
		else 
			strTemp.Format( "%d", nLevel );
		
		p2DRender->TextOut( nString1st_PosX, nPosY, 1.0f, 1.0f, strTemp, dwTextColor );
		
		if( !bSummary )
		{
			strTemp.Format( "%s", strClass );
			p2DRender->TextOut( nString2nd_PosX, nPosY, 1.0f, 1.0f, strTemp, dwTextColor);
			strTemp.Format( "%s",prj.m_aJob[nJob].szName);
			p2DRender->TextOut( nString3rd_PosX, nPosY, 1.0f, 1.0f, strTemp, dwTextColor);
		}
		else
		{
			strTemp.Format( "%s", prj.m_aJob[nJob].szName );
			p2DRender->TextOut( nString2nd_PosX, nPosY, 1.0f, 1.0f, strTemp, dwTextColor );
		}
		strName	= pPlayerData->szPlayer;
	}

	// �� �̸� ó��
	if( strName.GetLength() > byPlayerNameMaxCount ) 
	{
		int	nReduceCount( NULL );

		for( nReduceCount = 0; nReduceCount < byPlayerNameMaxCount; )
		{
			if( IsDBCSLeadByte( strName[ nReduceCount ] ) )
				nReduceCount+=2;
			else
				nReduceCount++;
		}

		strName = strName.Left( nReduceCount );		// 10����Ʈ°���� �ѱ��� �ε������Ű����� 9����Ʈ���� ¥����
		strName+="...";								// ... �߰�
	}

	strTemp.Format("%s",strName);
	p2DRender->TextOut( nString4th_PosX, nPosY, 1.0f, 1.0f, strTemp, dwTextColor );
}


//---------------------------------------------------------------------------------------------
// ������ �׸���
// param	: ������, ���, ���� ������ġ
// return	: void 
//---------------------------------------------------------------------------------------------
void CWndPartyInfo::DrawGauge(C2DRender* p2DRender, CMover* pMember, UINT nPosY)
{
	CRect					rtGauge( 0, 0, 0, 0 );
	CRect					rtGaugeBg( 0, 0, 0, 0 );
	BOOL					bSummary( CheckSummary() );
	static const WORD		wGaugeWidth( 95 );

	// HP ������
	if( !bSummary )
		rtGauge.SetRect( 310, nPosY - 2, 310 + wGaugeWidth, nPosY + 12 );
	else
		rtGauge.SetRect( 310, nPosY - 2, 310 + wGaugeWidth, nPosY + 12 );	// ���������� 30 �̵�

	rtGaugeBg = rtGauge; 

	if( IsValidObj( pMember ) )
	{
		int		nOffsetRight( NULL );
		int		nCurrentHitPoint( pMember->GetHitPoint() );
		int		nMaxHitPoint( pMember->GetMaxHitPoint() );

		if( nCurrentHitPoint > nMaxHitPoint )
			nCurrentHitPoint = nMaxHitPoint;

		nOffsetRight = nCurrentHitPoint * wGaugeWidth / nMaxHitPoint;
		rtGauge.right = rtGauge.left + nOffsetRight;
	}

	if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
	{
		m_pTheme->RenderGauge( p2DRender, &rtGaugeBg, WNDCOLOR_WHITE, m_pVBGauge, m_pGauEmptyNormal );
		m_pTheme->RenderGauge( p2DRender, &rtGauge, 0x64ff0000, m_pVBGauge, m_pGauFillNormal );
	}			

	if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
	{
		if( m_pGauEmptyNormal )
			m_pTheme->RenderGauge( p2DRender, &rtGaugeBg, WNDCOLOR_WHITE, m_pVBGauge, m_pGauEmptyNormal );

		if( IsValidObj(pMember) )
		{
			if( m_pGauFillNormal )
				m_pTheme->RenderGauge( p2DRender, &rtGauge, WNDCOLOR_WHITE, m_pVBGauge, m_pGauFillNormal );
		}
	}
}


//---------------------------------------------------------------------------------------------
// ��Ƽ ���� �׸��� ( �ش� )
// param	: ������
// return	: void 
//---------------------------------------------------------------------------------------------
void CWndPartyInfo::OnDraw( C2DRender* p2DRender ) 
{
	PFLCOLOR_WNDPARTYINFO	pColorPartyInfo( g_FLColorManager->GetWndPartyInfo() ); 

	// ���� ���õ� �� ǥ��
	if( m_nSelected != -1 ) 
	{
		CRect rect( 5, 8 + m_nSelected * 15, 410, 22 + m_nSelected * 15 );
		p2DRender->RenderFillRect( rect, pColorPartyInfo->m_stSelectMember.GetColor() );
	}

	// ��Ƽ�� ���� ���
	if( g_Party.GetSizeofMember() < m_nSelected + 1 ) 
		m_nSelected = -1;

	int nStartPosY( 10 );
	for( int i = 0; i < g_Party.m_nSizeofMember; i++ ) 
	{		
		CMover*	pObjMember( prj.GetUserByID( g_Party.m_aMember[ i ].m_uPlayerId ) );

		DrawStatusBox( p2DRender, pObjMember, i, nStartPosY );
		DrawMemberInfo( p2DRender, pObjMember, i, nStartPosY );	
		DrawGauge( p2DRender, pObjMember, nStartPosY );
	
		nStartPosY += 15; // ���� ��
	}
} 




void CWndPartyInfo::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
//sun: 11, Ÿ�� ǥ�� ��� ����
	CWndTaskBar* pTaskBar = g_WndMng.m_pWndTaskBar;
	if(((CWndWorld*)g_WndMng.m_pWndWorld)->m_bAutoAttack || pTaskBar->m_nExecute != 0)
		return;
	
	// ��Ƽâ���� ��� �������� ����
	CRect rect( 10, 10, 365, 10 + g_Party.GetSizeofMember() * 15 );
	if( rect.PtInRect( point ) ) 
		m_nSelected = ( point.y - 10 ) / 15;
	else 
		m_nSelected = -1;
	if( m_nSelected != -1 ) 
	{ // ���õȳ� ������
		// �׳��� Ÿ������

		//sun: 8�� ��Ƽâ ���� ���� Neuz
		((CWndWorld*)g_WndMng.m_pWndWorld)->m_pSelectRenderObj = NULL; //���콺�� �ɷ� �׷������� ����� Ÿ���� ��ƾ� �Ȱ�ģ��..

		CMover* pObjMember = prj.GetUserByID( g_Party.m_aMember[m_nSelected].m_uPlayerId );
		if(g_pPlayer!=pObjMember) 
		{
			if(IsValidObj(pObjMember)) 
			{ // ȭ�鿡 ���³��� �н�...
				g_WorldMng()->SetObjFocus(pObjMember);
				CWndWorld* pWndWorld = g_WndMng.m_pWndWorld;
				if(pWndWorld)
					pWndWorld->m_pRenderTargetObj = NULL;
			}
		}
		else
			g_WorldMng()->SetObjFocus( NULL );	//sun: 8�� ��Ƽâ ���� ���� Neuz
	}
	else 
	{
		// ������ Ÿ�� �����
		g_WorldMng()->SetObjFocus(NULL);
	}
} 


#pragma endregion WndPartyInfo



/****************************************************
  WndId : APP_PARTY_SKILL - �شܽ�ų
****************************************************/

CWndPartySkill::CWndPartySkill() 
{ 
	ZeroMemory( m_atexSkill, sizeof( m_atexSkill ) );
	m_nSkillSelect = -1;
} 
CWndPartySkill::~CWndPartySkill() 
{ 
} 
void CWndPartySkill::OnDraw( C2DRender* p2DRender ) 
{ 
//	if( g_Party.m_nKindTroup )
	{
		CRect rect = GetClientRect();
		int nWidth = rect.Width() / 3;
		int nHeight = rect.Height() / 3;
		int nCount = 0;
		
		PFONTCOLOR_WNDPARTYSKILL pFontColorWndPartySkill = g_WndFontColorManager->GetWndPartySkill();
		
		for( int i = 0; i < 3; i++ )
		{
			for( int j = 0; j < 3; j++, nCount++ )
			{
				SkillProp* pSkillProp =  prj.GetPartySkill( nCount + 1 );
				if( pSkillProp ) //&& g_Party.m_nSizeofMember >= 2 )
				{
					m_atexSkill[ i * 3 + j ] = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, pSkillProp->szIcon/*pItemBase->GetProp()->szIcon*/), 0xffff00ff, FALSE );
					CPoint point1 = CPoint( j * nWidth + 35 , i * nHeight + 6 );
					CPoint point2 = CPoint( j * nWidth + 3, i * nHeight + 3 );
					
					if( g_Party.GetKind() == PARTY_KIND_EXTEND && int(g_Party.GetLevel() - pSkillProp->dwReqDisLV) >= 0 )
					{
						if( (g_Party.GetPoint() - pSkillProp->dwReqPartyPoint) >= 0 )
						{
							// ��밡���� ���
							p2DRender->TextOut( point1.x, point1.y, pSkillProp->szName, m_dwColor );
							//m_aSlotQueue[nIndex].m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, lpSkill->GetProp()->szIcon/*pItemBase->GetProp()->szIcon*/), 0xffff00ff, FALSE );
							p2DRender->RenderTexture( point2, m_atexSkill[ i * 3 + j ] );
							
						}
						else
						{
							// ����� �������� ����Ʈ�� ���ڶ� ����� ����
							p2DRender->TextOut( point1.x, point1.y, pSkillProp->szName, pFontColorWndPartySkill->m_stlessPoint.GetFontColor() );
							p2DRender->RenderTexture( point2, m_atexSkill[ i * 3 + j ] );
							
						}
					}
					else
					{
						// ����� ��������
						p2DRender->TextOut( point1.x, point1.y, pSkillProp->szName, pFontColorWndPartySkill->m_stNoLearnSkill.GetFontColor() );
						p2DRender->RenderTexture( point2, m_atexSkill[ i * 3 + j ], 54 );		
					}
				}
			}
		}
	}
} 
void CWndPartySkill::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
	CRect rect = GetClientRect();
	int nWidth = rect.Width() / 3;
	int nHeight = rect.Height() / 3;
	point.x /= nWidth; 
	point.y /= nHeight;
	int nSkill = point.y * 3 + point.x;
	
	SkillProp* pSkillProp =  prj.GetPartySkill( nSkill + 1 );
	if( pSkillProp )
	{
		m_nSkillSelect = nSkill;
		return;
	}
	m_nSkillSelect = -1;
} 
void CWndPartySkill::OnMouseWndSurface( CPoint point )
{
	CRect rect = GetClientRect();
	int nWidth = rect.Width() / 3;
	int nHeight = rect.Height() / 3;
	int nCount = 0;
	for( int i = 0; i < 3; i++ )
	{
		for( int j = 0; j < 3; j++, nCount++ )
		{
			SkillProp* pSkillProp =  prj.GetPartySkill( nCount + 1 );
			if( pSkillProp ) //&& g_Party.m_nSizeofMember >= 2 )
			{
				CRect rect( j * nWidth + 3, i * nHeight + 3, j * nWidth + 3 + nWidth, i * nHeight + 3 + nHeight);
				if( rect.PtInRect( point ) )
				{
					CPoint point2 = point;
					ClientToScreen( &point2 );
					ClientToScreen( &rect );
					
					g_WndMng.PutToolTip_Troupe( i * 3 + j + 1, point2, &rect );
				}
			}
		}
	}
}	
void CWndPartySkill::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
	m_nSkillSelect = -1;
}

void CWndPartySkill::OnMouseMove(UINT nFlags, CPoint point)
{
	if( m_nSkillSelect == -1 || IsPush() == FALSE )
		return;
	//	FLTRACE_LOG( PROGRAM_NAME, _T( " hello " ) );
	CPoint pt( 3, 3 );
	CRect rect;
	//if( m_rect.PtInRect( point ) )
	//{
	//DWORD dwSkill = m_apJobSkill[ m_nCurSelect ].dwSkill;
	SkillProp* pSkillProp =  prj.GetPartySkill( m_nSkillSelect + 1 );
	if( pSkillProp ) //&& g_Party.m_nSizeofMember >= 2 )
	{
		if( int(g_Party.GetLevel() - pSkillProp->dwReqDisLV) >= 0 )
		{
			//FLItemElem* pItemElem = &m_pItemContainer->m_apItem[ m_pItemContainer->m_apIndex[ m_nCurSelect ] ] ;
			m_GlobalShortcut.m_dwShortcut  = SHORTCUT_SKILL;
			m_GlobalShortcut.m_dwType  = 2;//m_nJob;//(DWORD)pItemElem;//->m_dwItemId;
			m_GlobalShortcut.m_dwIndex = m_nSkillSelect + 1;//m_nSkillSelect;//m_nCurSelect;//(DWORD)pItemElem;//->m_dwItemId;
			m_GlobalShortcut.m_dwData  = 0;//pItemElem->m_dwObjId;//(DWORD)pItemElem;
			m_GlobalShortcut.m_dwId     = m_nSkillSelect + 1; // �÷�Ʈ ����Ʈ�� �� ID�� ����������.
			m_GlobalShortcut.m_pTexture = m_atexSkill[ m_nSkillSelect ];//L;//pItemElem->m_pTexture;
			FLStrcpy( m_GlobalShortcut.m_szString, _countof( m_GlobalShortcut.m_szString ), pSkillProp->szName);
			//}
		}
	}
}

void CWndPartySkill::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndPartySkill::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_PARTY_SKILL, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  ���� ������ ���� ��� 
BOOL CWndPartySkill::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndPartySkill::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndPartySkill::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndPartySkill::OnLButtonDblClk( UINT nFlags, CPoint point)
{
//	if( g_Party.m_nSizeofMember >= 2 && g_Party.m_nKindTroup == PARTY_KIND_EXTEND )
	{
		CRect rect = GetClientRect();
		int nWidth = rect.Width() / 3;
		int nHeight = rect.Height() / 3;
		point.x /= nWidth; 
		point.y /= nHeight;
		int nSkill = point.y * 3 + point.x;
		
		SkillProp* pSkillProp =  prj.GetPartySkill( nSkill + 1 );
		if( pSkillProp )
		{
			g_WndMng.ObjectExecutor( SHORTCUT_SKILL, nSkill + 1, 2 );
		}
	}
}
BOOL CWndPartySkill::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 
