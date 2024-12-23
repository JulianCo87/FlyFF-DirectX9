#include "stdafx.h"
#include "WndMessenger.h"
#include "WndFriendCtrl.h"
#include "WndFriendConFirm.h"
#include "../Neuz/dpcertified.h"
extern	CDPCertified	g_dpCertified;

#include "WndManager.h"
#include "../_Common/FLServerDescription.h"

/****************************************************
  WndId : APP_MESSENGER - Messenger
  CtrlId : WIDC_TABCTRL1 - TabCtrl
  CtrlId : WIDC_BUTTON1 - Button
****************************************************/


CWndMessenger::CWndMessenger() 
{ 
} 
CWndMessenger::~CWndMessenger() 
{ 
	m_TexMail.DeleteDeviceObjects();
} 	
void CWndMessenger::SerializeRegInfo( CAr& ar, DWORD& dwVersion )
{
	CWndNeuz::SerializeRegInfo( ar, dwVersion );
	CWndTabCtrl* lpTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
	if( ar.IsLoading() )
	{
		int nCurSel;
		ar >> nCurSel;
		lpTabCtrl->SetCurSel( nCurSel );
	}
	else
	{
		ar << lpTabCtrl->GetCurSel();
	}
}

void CWndMessenger::OnMouseWndSurface( CPoint point )
{
	// 직업 아이콘 위치에 커서를 가져가면 접속한 서버의 정보(서버이름과 멀티서버 번호)를 보여준다.
	CRect rectHittest( 2, 2, 35, 27 );	
	if( rectHittest.PtInRect( point ) )
	{
		CPoint point2 = point;
		ClientToScreen( &point2 );
		ClientToScreen( &rectHittest );
		g_toolTip.PutToolTip( (DWORD)this, m_strTooltip, rectHittest, point2, 0 );
	}
}

void CWndMessenger::OnDraw( C2DRender* p2DRender ) 
{ 
	CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );

	DWORD dwMyState;
	if( g_WndMng.m_RTMessenger.GetState() == FRS_AUTOABSENT )
		dwMyState	= FRS_ABSENT;
	else
		dwMyState	=  g_WndMng.m_RTMessenger.GetState();
	
	TEXTUREVERTEX2 vertex[ 6 * 2 ];
	TEXTUREVERTEX2* pVertices = vertex;
	
	if( dwMyState == FRS_OFFLINE )
	{
		if( g_pPlayer->IsPro() )
		{
			pWndWorld->m_texMsgIcon.MakeVertex( p2DRender, CPoint( 5, 0 ),  ( 54 + g_pPlayer->GetJob() - 6 ) + ( 8 * g_pPlayer->GetSex() ), &pVertices, 0xffff6464 );
		}
//sun: 10차 전승시스템	Neuz, World, Trans
		else if( g_pPlayer->IsMaster() )
		{
			pWndWorld->m_texMsgIcon.MakeVertex( p2DRender, CPoint( 5, 0 ),  ( 54 + g_pPlayer->GetJob() - 16 ) + ( 8 * g_pPlayer->GetSex() ), &pVertices, 0xffff6464 );
		}
		else if( g_pPlayer->IsHero() )
		{
			pWndWorld->m_texMsgIcon.MakeVertex( p2DRender, CPoint( 5, 0 ),  ( 54 + g_pPlayer->GetJob() - 24 ) + ( 8 * g_pPlayer->GetSex() ), &pVertices, 0xffff6464 );
		}
		else
		{
			pWndWorld->m_texMsgIcon.MakeVertex( p2DRender, CPoint( 5, 0 ),  g_pPlayer->GetJob() + ( 6 * g_pPlayer->GetSex() ), &pVertices, 0xffff6464 );
		}
	}
	else
	{
		if( g_pPlayer->IsPro() )
		{
			pWndWorld->m_texMsgIcon.MakeVertex( p2DRender, CPoint( 5, 0 ),  ( 54 + g_pPlayer->GetJob() - 6 ) + ( 8 * g_pPlayer->GetSex() ), &pVertices, 0xffffffff );
		}
//sun: 10차 전승시스템	Neuz, World, Trans
		else if( g_pPlayer->IsMaster() )
		{
			pWndWorld->m_texMsgIcon.MakeVertex( p2DRender, CPoint( 5, 0 ),  ( 54 + g_pPlayer->GetJob() - 16 ) + ( 8 * g_pPlayer->GetSex() ), &pVertices, 0xffffffff );
		}
		else if( g_pPlayer->IsHero() )
		{
			pWndWorld->m_texMsgIcon.MakeVertex( p2DRender, CPoint( 5, 0 ),  ( 54 + g_pPlayer->GetJob() - 24 ) + ( 8 * g_pPlayer->GetSex() ), &pVertices, 0xffffffff );
		}
		else
		{
			pWndWorld->m_texMsgIcon.MakeVertex( p2DRender, CPoint( 5, 0 ),  g_pPlayer->GetJob() + ( 6 * g_pPlayer->GetSex() ), &pVertices, 0xffffffff );
		}

		if( dwMyState != FRS_ONLINE )
			pWndWorld->m_texMsgIcon.MakeVertex( p2DRender, CPoint( 5 + 18, 0 + 16 ), 27 + ( dwMyState - 2 ), &pVertices, 0xffffffff );
	}	
	
	pWndWorld->m_texMsgIcon.Render( m_pApp->m_pd3dDevice, vertex, ( (int) pVertices - (int) vertex ) / sizeof( TEXTUREVERTEX2 ) );
	
	CString strState;
	strState.Format( "(%s)", prj.GetText( dwMyState + TID_FRS_ONLINE ) );
	
	LPCTSTR pszName = g_pPlayer->GetName( TRUE );
	p2DRender->TextOut( 70,  2, 1, 1, pszName, 0xff606060 );
	p2DRender->TextOut( 71,  2, 1, 1, pszName, 0xff606060 );
	p2DRender->TextOut( 70, 18, 1, 1, strState, 0xff606060 );

	if( g_pPlayer && g_pPlayer->IsMode(MODE_MAILBOX) )
	{
		LPWNDCTRL lpMail = GetWndCtrl( WIDC_CUSTOM1 );
		p2DRender->RenderTexture( lpMail->rect.TopLeft(), &m_TexMail, m_nFlashCounter );
	}
} 

BOOL CWndMessenger::Process()
{
	LPWNDCTRL lpMail = GetWndCtrl( WIDC_CUSTOM1 );

	if( g_pPlayer && g_pPlayer->IsMode(MODE_MAILBOX) )
	{
		lpMail->m_bDisabled = FALSE;

		m_nFlashCounter += (m_nSwitch*8);
		
		if( m_nFlashCounter >= 255 )
		{
			m_nFlashCounter = 255;
			m_nSwitch = -1;
		}
		
		if( m_nFlashCounter <= 50 )
		{
			m_nFlashCounter = 50;
			m_nSwitch = 1;
		}

		CRect  rect    = lpMail->rect;
		CPoint point	= GetMousePoint();
		if( PtInRect( lpMail->rect, point ) )
		{
			ClientToScreen( &point );
			ClientToScreen( &rect );
			g_toolTip.PutToolTip( (DWORD)this, prj.GetText(TID_MAIL_RECEIVE), rect, point );
		}		
	}
	return CWndBase::Process();
}

void CWndMessenger::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// 여기에 코딩하세요

	LPCTSTR lpszFormat = prj.GetText(TID_GAME_TOOLTIP_MESS); //접속 서버 -  %s / 멀티 %d 서버 -

	const T_SERVER_DESC* pServerDesc = g_pServerDescription->FindByNth( NULL_ID, g_Option.m_nSer );
	if( pServerDesc != NULL )
	{
		m_strTooltip.Format( lpszFormat, pServerDesc->szName, g_Option.m_nMSer+1 );
	}

	CWndTabCtrl* pWndTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
	
	//m_wndAdminBalanceExp.Create( WBS_CHILD | WBS_NODRAWFRAME, rect, pWndTabCtrl, APP_ADMIN_BALANCE_EXP );
	//m_wndPartySkill.Create( WBS_CHILD | WBS_NOFRAME | WBS_NODRAWFRAME , rect, pWndTabCtrl, APP_PARTY_SKILL );
	WTCITEM tabTabItem;

	m_wndFriend.Create( WLVS_ICON, CRect( 0, 0, 250, 250 ), pWndTabCtrl, 11 );
	m_wndFriend.AddWndStyle( WBS_NODRAWFRAME );
//	m_wndFriend.InitItem( g_pPlayer->m_nJob, g_pPlayer->m_nJob, g_pPlayer->m_aJobSkill );
	if( g_WndMng.m_RTMessenger.size() )
	{
		g_DPlay.SendGetFriendState();
	}
	m_wndParty.Create( WLVS_ICON, CRect( 0, 0, 250, 250 ), pWndTabCtrl, 12 );
	m_wndParty.AddWndStyle( WBS_NODRAWFRAME );
//	m_wndParty.InitItem( g_pPlayer->m_nJob, g_pPlayer->m_nJob, g_pPlayer->m_aJobSkill );

	m_wndGuild.Create( WLVS_ICON, CRect( 0, 0, 250, 250 ), pWndTabCtrl, 13 );
	m_wndGuild.AddWndStyle( WBS_NODRAWFRAME );
	
	tabTabItem.mask = WTCIF_TEXT | WTCIF_PARAM;
	tabTabItem.pszText = prj.GetText(TID_APP_COMMUNITY_FRIEND);//"친구";
	tabTabItem.pWndBase = &m_wndFriend;
	pWndTabCtrl->InsertItem( 0, &tabTabItem );
	
	tabTabItem.mask = WTCIF_TEXT | WTCIF_PARAM;
	tabTabItem.pszText = prj.GetText(TID_APP_PARTY);//"극단";
	tabTabItem.pWndBase = &m_wndParty;
	pWndTabCtrl->InsertItem( 1, &tabTabItem );

	tabTabItem.mask = WTCIF_TEXT | WTCIF_PARAM;
	tabTabItem.pszText = prj.GetText(TID_APP_COMPANY);//"극단";;	// GetText 추가해야함
	tabTabItem.pWndBase = &m_wndGuild;
	pWndTabCtrl->InsertItem( 2, &tabTabItem );

	m_menuState.CreateMenu( this );	
	m_menuState.AppendMenu( 0, FRS_ONLINE   , prj.GetText( TID_FRS_ONLINE   ) );
	m_menuState.AppendMenu( 0, FRS_OFFLINE  , prj.GetText( TID_FRS_OFFLINE  ) );
	m_menuState.AppendMenu( 0, FRS_ABSENT   , prj.GetText( TID_FRS_ABSENT   ) );
	m_menuState.AppendMenu( 0, FRS_HARDPLAY , prj.GetText( TID_FRS_HARDPLAY ) );
	m_menuState.AppendMenu( 0, FRS_EAT      , prj.GetText( TID_FRS_EAT      ) );
	m_menuState.AppendMenu( 0, FRS_REST     , prj.GetText( TID_FRS_REST     ) );
	m_menuState.AppendMenu( 0, FRS_MOVE     , prj.GetText( TID_FRS_MOVE     ) );
	
	//flyingjin : Default 폴더를 없애고 국가별 폴더로 넣어준다
	m_TexMail.LoadTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage() , "WndMail.dds" ), 0xffff00ff );
	m_nFlashCounter = 0;
	m_nSwitch       = 1;

	// 윈도를 중앙으로 옮기는 부분.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), rectRoot.bottom - rectWindow.Height() );
	Move( point );
} 
// 처음 이 함수를 부르면 윈도가 열린다.
BOOL CWndMessenger::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	if( g_eLocal.GetState( EVE_SCHOOL ) )
		return FALSE;
	// Daisy에서 설정한 리소스로 윈도를 연다.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_MESSENGER_, WBS_THICKFRAME, CPoint( 0, 0 ), pWndParent );
} 
/*
  직접 윈도를 열때 사용 
BOOL CWndMessenger::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndMessenger::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndMessenger::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndMessenger::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndMessenger::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_ADD )
	{
//		if( g_WndMng.GetWndBase( APP_ADD_FRIEND ) == FALSE )
		if( !g_WndMng.m_pWndAddFriend )
		{
			g_WndMng.m_pWndAddFriend = new CWndAddFriend;
			g_WndMng.m_pWndAddFriend->Initialize();
			
/*			g_DPlay.SendAddFriendReqest( pFocusMover->m_idPlayer );
			char szMessage[256];
			sprintf( szMessage, "%s님에게 메신저 요청을 하였습니다", pFocusMover->GetName());
			g_WndMng.PutString( szMessage, NULL, 0xffffff00 );
*/
		}
	}
	else
	if( nID == WIDC_FIND )
	{
//		SAFE_DELETE( g_WndMng.m_pWndMessengerSearch );
//		g_WndMng.m_pWndMessengerSearch = new CWndMessengerSearch;
//		g_WndMng.m_pWndMessengerSearch->Initialize( &g_WndMng, APP_MESSENGER_SEARCH );
		g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0027) ) );
	}
	else
	if( nID == WIDC_BUTTON1 )
	{
		CWndButton* pWndButton = (CWndButton*)GetDlgItem( WIDC_BUTTON1 );
		CRect rect = pWndButton->GetWindowRect( TRUE );
		ClientToScreen( &rect );
		rect.OffsetRect( CPoint( 0, rect.Height() ) );
		m_menuState.Move( rect.TopLeft() );
		m_menuState.SetVisible( TRUE );
		m_menuState.SetFocus();
	}
	else
	if( nID == WIDC_BUTTON2 )
	{
		if( !g_WndMng.m_pWndMessengerNote )
		{
			g_WndMng.m_pWndMessengerNote = new CWndMessengerNote;
			g_WndMng.m_pWndMessengerNote->Initialize();  
		}
	}
	else
	{
		if( FRS_ONLINE <= nID && nID < MAX_FRIENDSTAT )
		{
			// 내 상태가 바뀌었따~ 코어로 보내어 모두 알려주자~
			g_DPlay.SendSetState( nID );
		}
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 
void CWndMessenger::SetWndRect( CRect rectWnd, BOOL bOnSize )
{
	AdjustMinRect( &rectWnd, 16 * 11, 16 * 10 );
//	AdjustMaxRect( &rectWnd, 16 * 12, 16 * 12 );
	CWndNeuz::SetWndRect( rectWnd, bOnSize );
}
/*
void CWndNavigator::OnSize(UINT nType, int cx, int cy)
{
	m_wndPlace.Move( CPoint( 0, 0 ) );
	m_wndZoomIn.Move( CPoint(0, cy - 32 ) );
	m_wndZoomOut.Move( CPoint(0, cy - 16 ) );
	
	CWndNeuz::OnSize(nType,cx,cy);
}
*/
void CWndMessenger::OnSize(UINT nType, int cx, int cy)
{
	CRect rect = GetClientRect();//GetWndRect();
	CWndTabCtrl* pTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
	CWndButton* pAdd = (CWndButton*)GetDlgItem( WIDC_ADD );
	CWndButton* pFind = (CWndButton*)GetDlgItem( WIDC_FIND );
	CWndButton* pTag = (CWndButton*)GetDlgItem( WIDC_BUTTON2 );

	rect.top += 32;
	rect.left += 4;
	rect.bottom -= 20;
	pTabCtrl->SetWndRect( rect );
	pAdd->Move( rect.left + 5, rect.bottom + 2 );
	pFind->Move( rect.left + 25, rect.bottom + 2 );
	pTag->Move( rect.left + 45, rect.bottom + 2 );

	m_wndFriend.ScrollBarPos( 0 );
	m_wndParty.ScrollBarPos( 0 );
	m_wndGuild.ScrollBarPos( 0 );

	CWndNeuz::OnSize( nType, cx, cy );
}

//sun: 11, 메신저창 개선
//////////////////////////////////////////////////////////////////////////
// New Messenger Window
//////////////////////////////////////////////////////////////////////////

CWndMessengerEx::CWndMessengerEx() 
{ 
}

CWndMessengerEx::~CWndMessengerEx() 
{ 
	m_TexMail.DeleteDeviceObjects();
}

void CWndMessengerEx::SerializeRegInfo( CAr& ar, DWORD& dwVersion )
{
	CWndNeuz::SerializeRegInfo( ar, dwVersion );
	CWndTabCtrl* lpTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
	if( ar.IsLoading() )
	{
		int nCurSel;
		ar >> nCurSel;
		lpTabCtrl->SetCurSel( nCurSel );
	}
	else
	{
		ar << lpTabCtrl->GetCurSel();
	}
}

void CWndMessengerEx::OnDraw( C2DRender* p2DRender ) 
{ 
	CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );

	PFONTCOLOR_WNDMESSENGEREX pFontColorWndMessengerEx = g_WndFontColorManager->GetWndMessengerEx();

	// Draw Status Icon
	DWORD dwMyState;
	if( g_WndMng.m_RTMessenger.GetState() == FRS_AUTOABSENT )
		dwMyState	= FRS_ABSENT;
	else if( g_WndMng.m_RTMessenger.GetState() == FRS_ONLINE )
		dwMyState	= 2;
	else if( g_WndMng.m_RTMessenger.GetState() == FRS_OFFLINE )
		dwMyState	= 8;
	else
		dwMyState	= g_WndMng.m_RTMessenger.GetState();
	
	TEXTUREVERTEX2 vertex[ 6 ];
	TEXTUREVERTEX2* pVertices = vertex;
	
	pWndWorld->m_texPlayerDataIcon.MakeVertex( p2DRender, CPoint( 8, 8 ), dwMyState - 2, &pVertices, 0xffffffff );	
	pWndWorld->m_texPlayerDataIcon.Render( m_pApp->m_pd3dDevice, vertex, ( (int) pVertices - (int) vertex ) / sizeof( TEXTUREVERTEX2 ) );
	
	// Draw Name & Status
	CString strState;
	switch( g_WndMng.m_RTMessenger.GetState() )
	{
		case FRS_ONLINE:
			strState.Format( "(%s)", prj.GetText( TID_FRS_ONLINE_STATUS ) );
			break;
		case FRS_OFFLINE:
			strState.Format( "(%s)", prj.GetText( TID_FRS_OFFLINE_STATUS ) );
			break;
		case FRS_ABSENT:
			strState.Format( "(%s)", prj.GetText( TID_FRS_ABSENT ) );
			break;
		case FRS_HARDPLAY:
			strState.Format( "(%s)", prj.GetText( TID_FRS_HARDPLAY ) );
			break;
		case FRS_EAT:
			strState.Format( "(%s)", prj.GetText( TID_FRS_EAT ) );
			break;
		case FRS_REST:
			strState.Format( "(%s)", prj.GetText( TID_FRS_REST ) );
			break;
		case FRS_MOVE:
			strState.Format( "(%s)", prj.GetText( TID_FRS_MOVE ) );
			break;
	}
	
	LPCTSTR pszName = g_pPlayer->GetName( TRUE );

	if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		p2DRender->TextOut( 49, 11, 1, 1, pszName, pFontColorWndMessengerEx->m_stNameShadow.GetFontColor()  );

	p2DRender->TextOut( 48, 10, 1, 1, pszName, pFontColorWndMessengerEx->m_stName.GetFontColor() );
	p2DRender->TextOut( 46, 27, 1, 1, strState, pFontColorWndMessengerEx->m_stState.GetFontColor() );

	if( g_pPlayer && g_pPlayer->IsMode(MODE_MAILBOX) )
	{
		LPWNDCTRL lpMail = GetWndCtrl( WIDC_CUSTOM1 );
		p2DRender->RenderTexture( CPoint(lpMail->rect.left, lpMail->rect.top), &m_TexMail, m_nFlashCounter );
	}

	if(g_Neuz.m_nTagCount > 0 && g_Neuz.m_bCheckTag == FALSE)
	{
		LPWNDCTRL lpTagButton = GetWndCtrl( WIDC_BUTTON2 );
		p2DRender->RenderTexture( CPoint(lpTagButton->rect.left + 2, lpTagButton->rect.top + 2), &m_TexMail, m_nFlashCounter );
	}

	CWndTabCtrl* pWndTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
	if( pWndTabCtrl == NULL )
		return;
	if( pWndTabCtrl->GetCurSel() == 2 )
	{
		CString strCampusPoint = _T( "" );
		strCampusPoint.Format( prj.GetText( TID_GAME_CAMPUS_POINTS ), g_pPlayer->GetCampusPoint() );

		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
			p2DRender->TextOut( 190, 20, 1, 1, strCampusPoint, pFontColorWndMessengerEx->m_stCampusPoint.GetFontColor() );
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
			p2DRender->TextOut( 190, 16, 1, 1, strCampusPoint, pFontColorWndMessengerEx->m_stCampusPoint.GetFontColor() );
	}
	else
	{
		CString strServerName = _T( "" );

		const T_SERVER_DESC* pServerDesc = g_pServerDescription->FindByNth( NULL_ID, g_Option.m_nSer );
		if( pServerDesc != NULL )
		{
			strServerName.Format( "%s", pServerDesc->szName );
			if( strServerName.GetLength() > 18 ) 
			{
				int	nReduceCount = 0;

				for( nReduceCount = 0; nReduceCount < 18; )
				{
					if( IsDBCSLeadByte( strServerName[ nReduceCount ] ) )
						nReduceCount += 2;
					else
						++nReduceCount;
				}
				strServerName = strServerName.Left( nReduceCount );
				strServerName += "...";
			}
			
			if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
				p2DRender->TextOut( 170, 12, 1, 1, strServerName, pFontColorWndMessengerEx->m_stServer.GetFontColor() );
			if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
				p2DRender->TextOut( 170, 11, 1, 1, strServerName, pFontColorWndMessengerEx->m_stServer.GetFontColor() );

			// 채널 정보
			const T_SERVER_DESC* pChannelDesc = g_pServerDescription->FindByNth( pServerDesc->dwMyID, g_Option.m_nMSer );
			if( pChannelDesc != NULL )
			{
				strServerName.Format( "%s", pChannelDesc->szName );
				if( strServerName.GetLength() > 18 ) 
				{
					int	nReduceCount = 0;

					for( nReduceCount = 0; nReduceCount < 18; )
					{
						if( IsDBCSLeadByte( strServerName[ nReduceCount ] ) )
							nReduceCount += 2;
						else
							++nReduceCount;
					}
					strServerName = strServerName.Left( nReduceCount );
					strServerName += "...";
				}

				if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
					p2DRender->TextOut( 170, 27, 1, 1, strServerName, pFontColorWndMessengerEx->m_stChannel.GetFontColor() );
				if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
					p2DRender->TextOut( 170, 26, 1, 1, strServerName, pFontColorWndMessengerEx->m_stChannel.GetFontColor() );
			}
		}
	}
} 

BOOL CWndMessengerEx::Process()
{
	if( (g_pPlayer && g_pPlayer->IsMode(MODE_MAILBOX)) || (g_Neuz.m_nTagCount > 0 && g_Neuz.m_bCheckTag == FALSE) )
	{
		m_nFlashCounter += (m_nSwitch*8);
		
		if( m_nFlashCounter >= 255 )
		{
			m_nFlashCounter = 255;
			m_nSwitch = -1;
		}
		
		if( m_nFlashCounter <= 50 )
		{
			m_nFlashCounter = 50;
			m_nSwitch = 1;
		}
	}

	if( g_pPlayer && g_pPlayer->IsMode(MODE_MAILBOX) )
	{
		LPWNDCTRL lpMail = GetWndCtrl( WIDC_CUSTOM1 );
		lpMail->m_bDisabled = FALSE;

		CRect  rect    = lpMail->rect;
		CPoint point	= GetMousePoint();
		if( PtInRect( lpMail->rect, point ) )
		{
			ClientToScreen( &point );
			ClientToScreen( &rect );
			g_toolTip.PutToolTip( (DWORD)this, prj.GetText(TID_MAIL_RECEIVE), rect, point );
		}		
	}
	return CWndBase::Process();
}

void CWndMessengerEx::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// 여기에 코딩하세요

	CWndTabCtrl* pWndTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );	

	if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		pWndTabCtrl->SetTabTitleAlign( CWndTabCtrl::ALIGN_CENTER );

	WTCITEM tabTabItem;

	m_wndFriend.Create( CRect( 0, 0, 250, 250 ), pWndTabCtrl, 11 );
	m_wndFriend.AddWndStyle( WBS_NODRAWFRAME );
	if( g_WndMng.m_RTMessenger.size() )
	{
		g_DPlay.SendGetFriendState();
	}
	m_wndGuild.Create( CRect( 0, 0, 250, 250 ), pWndTabCtrl, 13 );
	m_wndGuild.AddWndStyle( WBS_NODRAWFRAME );

	m_WndCampus.Create( WBS_CHILD | WBS_NODRAWFRAME, GetClientRect(), pWndTabCtrl, APP_MESSENGER_TAB_CAMPUS );
	
	tabTabItem.mask = WTCIF_TEXT | WTCIF_PARAM;
	tabTabItem.pszText = prj.GetText(TID_APP_COMMUNITY_FRIEND); //"친구"
	tabTabItem.pWndBase = &m_wndFriend;
	pWndTabCtrl->InsertItem( 0, &tabTabItem );
	
	tabTabItem.mask = WTCIF_TEXT | WTCIF_PARAM;
	tabTabItem.pszText = prj.GetText(TID_APP_COMPANY); //"극단"
	tabTabItem.pWndBase = &m_wndGuild;
	pWndTabCtrl->InsertItem( 1, &tabTabItem );

	tabTabItem.mask = WTCIF_TEXT | WTCIF_PARAM;
	tabTabItem.pszText = prj.GetText(TID_APP_COMMUNITY_CAMPUS); //"사제"
	tabTabItem.pWndBase = &m_WndCampus;
	pWndTabCtrl->InsertItem( 2, &tabTabItem );

	m_wndFriend.ScrollBarPos( 0 );
	m_wndGuild.ScrollBarPos( 0 );

	m_menuState.CreateMenu( this );	
	m_menuState.AppendMenu( 0, FRS_ONLINE   , prj.GetText( TID_FRS_ONLINE_STATUS   ) );
	m_menuState.AppendMenu( 0, FRS_ABSENT   , prj.GetText( TID_FRS_ABSENT   ) );
	m_menuState.AppendMenu( 0, FRS_HARDPLAY , prj.GetText( TID_FRS_HARDPLAY ) );
	m_menuState.AppendMenu( 0, FRS_EAT      , prj.GetText( TID_FRS_EAT      ) );
	m_menuState.AppendMenu( 0, FRS_REST     , prj.GetText( TID_FRS_REST     ) );
	m_menuState.AppendMenu( 0, FRS_MOVE     , prj.GetText( TID_FRS_MOVE     ) );
	m_menuState.AppendMenu( 0, FRS_OFFLINE  , prj.GetText( TID_FRS_OFFLINE_STATUS  ) );
	
	//flyingjin : Default 폴더를 없애고 국가별 폴더로 넣어준다

	m_TexMail.LoadTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), "WndMail.dds" ), 0xffff00ff );
	m_nFlashCounter = 0;
	m_nSwitch       = 1;

	if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
	{
		// 버튼 이미지 세팅
		CWndButton* pWndButton = (CWndButton*)GetDlgItem(WIDC_CHANNEL);
		if(pWndButton)
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), "ButtMessengerChannel.bmp" ), 0xffff00ff );

		pWndButton = (CWndButton*)GetDlgItem(WIDC_STATE);
		if(pWndButton)
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), "ButtMessengerState.bmp" ), 0xffff00ff );

		pWndButton = (CWndButton*)GetDlgItem(WIDC_LEVEL);
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), "ButtMessengerLevel.bmp" ), 0xffff00ff );

		pWndButton = (CWndButton*)GetDlgItem(WIDC_JOB);
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), "ButtMessengerJob.bmp" ), 0xffff00ff );

		pWndButton = (CWndButton*)GetDlgItem(WIDC_NAME);
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), "ButtMessengerChar.bmp" ), 0xffff00ff );

	}
	
	// 윈도를 중앙으로 옮기는 부분.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), rectRoot.bottom - rectWindow.Height() );
	Move( point );
}

// 처음 이 함수를 부르면 윈도가 열린다.
BOOL CWndMessengerEx::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	if( g_eLocal.GetState( EVE_SCHOOL ) )
		return FALSE;
	// Daisy에서 설정한 리소스로 윈도를 연다.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_MESSENGER_EX, 0, CPoint( 0, 0 ), pWndParent );
} 

BOOL CWndMessengerEx::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
}

void CWndMessengerEx::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
}

void CWndMessengerEx::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
}

BOOL CWndMessengerEx::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_ADD )
	{
		if( !g_WndMng.m_pWndAddFriend )
		{
			g_WndMng.m_pWndAddFriend = new CWndAddFriend;
			g_WndMng.m_pWndAddFriend->Initialize();
		}
	}
	else if( nID == WIDC_BUTTON1 )
	{
		CWndButton* pWndButton = (CWndButton*)GetDlgItem( WIDC_BUTTON1 );
		CRect rect = pWndButton->GetWindowRect( TRUE );
		ClientToScreen( &rect );
		rect.OffsetRect( CPoint( 0, rect.Height() ) );
		m_menuState.Move( rect.TopLeft() );
		m_menuState.SetVisible( TRUE );
		m_menuState.SetFocus();
	}
	else if( nID == WIDC_BUTTON2 )
	{
		if( !g_WndMng.m_pWndMessengerNote )
		{
			g_WndMng.m_pWndMessengerNote = new CWndMessengerNote;
			g_WndMng.m_pWndMessengerNote->Initialize();
			g_Neuz.m_bCheckTag = TRUE;
		}
	}
	else if( nID == WIDC_TABCTRL1 )
	{
		CWndTabCtrl* pTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
		CWndBase* pChild = pTabCtrl->GetFocusChild();
		if(pChild == &m_wndFriend)
			m_wndFriend.UpdatePlayerList();
		else if(pChild == &m_wndGuild)
			m_wndGuild.UpdatePlayerList();
		else if( pChild == &m_WndCampus )
			m_WndCampus.UpdatePlayerList();
	}
	else
	{
		if( FRS_ONLINE <= nID && nID < MAX_FRIENDSTAT )
		{
			// 내 상태가 바뀌었따~ 코어로 보내어 모두 알려주자~
			g_DPlay.SendSetState( nID );
		}
	}

	CWndTabCtrl* pTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
	CWndBase* pChild = pTabCtrl->GetFocusChild();
	int nFocusChild = 0;
	if(pChild == &m_wndFriend)
		nFocusChild = 1;
	else if(pChild == &m_wndGuild)
		nFocusChild = 2;
	else if( pChild == &m_WndCampus )
		nFocusChild = 3;

	switch(nID)
	{
		case WIDC_CHANNEL:
			{
				if(nFocusChild == 1)
					m_wndFriend.SortbyChannel();
				else if(nFocusChild == 2)
					m_wndGuild.SortbyChannel();
				else if( nFocusChild == 3 )
					m_WndCampus.SortbyChannel();
			}
			break;
		case WIDC_STATE:
			{
				if(nFocusChild == 1)
					m_wndFriend.SortbyStatus();
				else if(nFocusChild == 2)
					m_wndGuild.SortbyStatus();
				else if( nFocusChild == 3 )
					m_WndCampus.SortbyStatus();
			}
			break;
		case WIDC_LEVEL:
			{
				if(nFocusChild == 1)
					m_wndFriend.SortbyLevel();
				else if(nFocusChild == 2)
					m_wndGuild.SortbyLevel();
				else if( nFocusChild == 3 )
					m_WndCampus.SortbyLevel();
			}
			break;
		case WIDC_JOB:
			{
				if(nFocusChild == 1)
					m_wndFriend.SortbyJob();
				else if(nFocusChild == 2)
					m_wndGuild.SortbyJob();
				else if( nFocusChild == 3 )
					m_WndCampus.SortbyJob();
			}
			break;
		case WIDC_NAME:
			{
				if(nFocusChild == 1)
					m_wndFriend.SortbyName();
				else if(nFocusChild == 2)
					m_wndGuild.SortbyName();
				else if( nFocusChild == 3 )
					m_WndCampus.SortbyName();
			}
			break;
	}
	
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
}
/*
void CWndMessengerEx::SetWndRect( CRect rectWnd, BOOL bOnSize )
{
	AdjustMinRect( &rectWnd, 352, 368 );
	CWndNeuz::SetWndRect( rectWnd, bOnSize );
}

void CWndMessengerEx::OnSize(UINT nType, int cx, int cy)
{
	CRect rect = GetClientRect();
	CWndTabCtrl* pTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
	CWndButton* pAdd = (CWndButton*)GetDlgItem( WIDC_ADD );
	CWndButton* pTag = (CWndButton*)GetDlgItem( WIDC_BUTTON2 );
	LPWNDCTRL wndCtrl = GetWndCtrl( WIDC_CUSTOM1 );

	rect.top += 78;
	rect.left += 8;
	rect.bottom -= 22;
	rect.right -= 8;
	
	pTabCtrl->SetWndRect( rect );
	pAdd->Move( rect.left, rect.bottom + 3 );
	pTag->Move( rect.left + 22, rect.bottom + 3 );
	wndCtrl->rect.left = rect.left + 49;
	wndCtrl->rect.right = wndCtrl->rect.left + 12;
	wndCtrl->rect.top = rect.bottom + 3;
	wndCtrl->rect.bottom = wndCtrl->rect.top + 12;

	m_wndFriend.ScrollBarPos( 0 );
	m_wndGuild.ScrollBarPos( 0 );

	CWndNeuz::OnSize( nType, cx, cy );
}
*/
void CWndMessengerEx::UpdateFriendList()
{
	CWndTabCtrl* pTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
	CWndBase* pChild = pTabCtrl->GetFocusChild();
	if(pChild == &m_wndFriend)
		m_wndFriend.UpdatePlayerList();
}

void CWndMessengerEx::UpdateGuildMemberList()
{
	CWndTabCtrl* pTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
	CWndBase* pChild = pTabCtrl->GetFocusChild();
	if(pChild == &m_wndGuild)
		m_wndGuild.UpdatePlayerList();
}

void CWndMessengerEx::UpdateCampusMemberList()
{
	CWndTabCtrl* pTabCtrl = ( CWndTabCtrl* )GetDlgItem( WIDC_TABCTRL1 );
	CWndBase* pChild = pTabCtrl->GetFocusChild();
	if( pChild == &m_WndCampus )
		m_WndCampus.UpdatePlayerList();
}

CWndInstantMsg::CWndInstantMsg() 
{ 
} 
CWndInstantMsg::~CWndInstantMsg() 
{ 
} 
void CWndInstantMsg::OnDraw( C2DRender* p2DRender ) 
{ 
	if( m_timer.IsTimeOut() )
	{
		Destroy();
	}
} 
void CWndInstantMsg::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	
	m_timer.Set( SEC( 10 ) ); // 열리거나, 매시지를 받은 이후 10초 후에 사라진다.

	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), rectRoot.bottom  - rectWindow.Height() );
	Move( point );
	//MoveParentCenter();
} 
// 처음 이 함수를 부르면 윈도가 열린다.
BOOL CWndInstantMsg::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy에서 설정한 리소스로 윈도를 연다.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_INSTANTMSG, WBS_NOFOCUS, CPoint( 0, 0 ), pWndParent );
} 
/*
  직접 윈도를 열때 사용 
BOOL CWndInstantMsg::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndInstantMsg::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndInstantMsg::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndInstantMsg::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
	if( m_strPlayer == "Mail" || m_strPlayer == "Event" )
		return;

	CWndMessage* pWndMessage = g_WndMng.OpenMessage( m_strPlayer );
	pWndMessage->AddMessage( m_strPlayer, m_strMessage );
	Destroy();
} 
void CWndInstantMsg::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndInstantMsg::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID != WTBID_CLOSE )
	{
		if( m_strPlayer == _T("Event") )
		{
			Destroy();
		}
		else
		if( m_strPlayer != _T("Mail") )
		{
			CWndMessage* pWndMessage = g_WndMng.OpenMessage( m_strPlayer );
			pWndMessage->AddMessage( m_strPlayer, m_strMessage );
			Destroy();
		}
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 


//---------------------------------------------------------------------------------------------
// 메시지 추가 ( "님의 말 : " )
// param	: 없음
// return	: void
//---------------------------------------------------------------------------------------------
void CWndInstantMsg::AddMessage( LPCTSTR lpszFrom, LPCTSTR lpszMessage )
{
	PFONTCOLOR_WNDINSTANTMSG pFontColorWndInstatnMsg = g_WndFontColorManager->GetWndInstantMsg();

	PLAYSND( SND_INF_MESSENGERRING );
	
	CWndText* pWndText( (CWndText*)GetDlgItem( WIDC_TEXT ) );
	FLERROR_ASSERT_LOG_RETURN_VOID( pWndText, _T("[인스턴스 메시지] 텍스트 컨트롤 포인터가 유효하지 않습니다.") );

	m_strMessage = lpszMessage;

	CString strMessage( _T("") );
	strMessage.Format( "%s%s : \n %s\n", lpszFrom, prj.GetText(TID_GAME_FROM3), lpszMessage );

	pWndText->AddString( strMessage, pFontColorWndInstatnMsg->m_stMessage.GetFontColor() );
	m_timer.Reset();
}


//---------------------------------------------------------------------------------------------
// 접속 메시지 추가 ( "님이 로그인하였습니다" )
// param	: 없음
// return	: void
//---------------------------------------------------------------------------------------------
void CWndInstantMsg::AddMessageJoin( LPCTSTR lpszJoinName )
{
	PFONTCOLOR_WNDINSTANTMSG pFontColorWndInstatnMsg = g_WndFontColorManager->GetWndInstantMsg();

	PLAYSND( SND_INF_MESSENGERRING );

	CWndText* pWndText( (CWndText*)GetDlgItem( WIDC_TEXT ) );
	FLERROR_ASSERT_LOG_RETURN_VOID( pWndText, _T("[인스턴스 메시지] 텍스트 컨트롤 포인터가 유효하지 않습니다.") );
	
	CString strMessage( _T("") );
	strMessage.Format( _T("%s%s\n"), lpszJoinName, prj.GetText(TID_GAME_LOGIN) );

	pWndText->AddString( strMessage, pFontColorWndInstatnMsg->m_stMessage.GetFontColor() );
	m_timer.Reset();
}


//---------------------------------------------------------------------------------------------
// 우편 메시지
// param	: 없음
// return	: void
//---------------------------------------------------------------------------------------------
void CWndInstantMsg::AddPostMessage( LPCTSTR lpszSendName )
{
	PFONTCOLOR_WNDINSTANTMSG pFontColorWndInstatnMsg = g_WndFontColorManager->GetWndInstantMsg();

	PLAYSND( SND_INF_MESSENGERRING );
	
	CWndText* pWndText( (CWndText*)GetDlgItem( WIDC_TEXT ) );
	FLERROR_ASSERT_LOG_RETURN_VOID( pWndText, _T("[인스턴스 메시지] 텍스트 컨트롤 포인터가 유효하지 않습니다.") );

	pWndText->AddString( lpszSendName, pFontColorWndInstatnMsg->m_stMessage.GetFontColor() );
	m_timer.Reset();
}


CWndMessage::CWndMessage() 
{ 
} 
CWndMessage::~CWndMessage() 
{ 
} 
void CWndMessage::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndMessage::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 

	CWndEdit* pWndEdit = (CWndEdit*)GetDlgItem( WIDC_EDIT );
	pWndEdit->AddWndStyle( EBS_AUTOVSCROLL );
	pWndEdit->SetNativeMode();

	pWndEdit->SetFocus();

	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
void CWndMessage::InitSize( void )
{
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
}
// 처음 이 함수를 부르면 윈도가 열린다.
BOOL CWndMessage::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy에서 설정한 리소스로 윈도를 연다.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_MESSAGE, WBS_THICKFRAME, CPoint( 0, 0 ), pWndParent );
} 
/*
  직접 윈도를 열때 사용 
BOOL CWndMessage::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndMessage::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndMessage::SetWndRect( CRect rectWnd, BOOL bOnSize )
{
	AdjustMinRect( &rectWnd, 16 * 10, 16 * 10 );
//	AdjustMaxRect( &rectWnd, 16 * 12, 16 * 12 );
	CWndNeuz::SetWndRect( rectWnd, bOnSize );
}
void CWndMessage::OnSize( UINT nType, int cx, int cy ) \
{ 
	CRect rect = GetClientRect();//GetWndRect();
	
	CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT );
	CWndEdit* pWndEdit = (CWndEdit*)GetDlgItem( WIDC_EDIT );
	CWndButton* pWndSend = (CWndButton*)GetDlgItem( WIDC_SEND );

	rect.top += 4;
	rect.left += 4;
	rect.right -= 4;
	rect.bottom -= 50;
	pWndText->SetWndRect( rect );

	rect = GetClientRect();//GetWndRect();
	rect.top = rect.bottom - 45;
	rect.left += 4;
	rect.right -= 80;
	rect.bottom -= 4;
	pWndEdit->SetWndRect( rect );
	
	rect = GetClientRect();//GetWndRect();
	rect.top = rect.bottom - 45;
	rect.left = rect.right - 74;
	rect.right -= 4;
	rect.bottom -= 4;
	//pWndSend->SetWndRect( rect );
	pWndSend->Move( rect.TopLeft() );//rect.right + 4, rect.top );

	//pAdd->Move( rect.left + 5, rect.bottom + 2 );
	//pFind->Move( rect.left + 25, rect.bottom + 2 );
	
	//m_wndPlace.Move( CPoint( 0, 0 ) );
	//m_wndZoomIn.Move( CPoint(0, cy - 32 ) );
	//m_wndZoomOut.Move( CPoint(0, cy - 16 ) );
	
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndMessage::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndMessage::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndMessage::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	CWndEdit* pWndText = (CWndEdit*)GetDlgItem( WIDC_EDIT );
	switch(nID)
	{
		case WIDC_EDIT: // 본문 
			if( message == EN_RETURN)
			{
				if( pWndText->m_string.IsEmpty() == FALSE )
				{
					CString strFormat = pWndText->m_string;					
					pWndText->m_string.GetTextFormat(strFormat);
					CString string;
					string.Format( "/say \"%s\" %s", m_strPlayer, strFormat );
					
					g_DPlay.SendChat( string );
					pWndText->Empty();
				}
				//				pWndText->ResetString();
			}
			break;
		case WIDC_SEND: // 본문 
			{
				CString str = pWndText->m_string;
				if( str.IsEmpty() == FALSE )
				{
					CString strFormat = pWndText->m_string;

					pWndText->m_string.GetTextFormat(strFormat);

					CString string;
					string.Format( "/say \"%s\" %s", m_strPlayer, strFormat );					
					
					g_DPlay.SendChat( string );
					pWndText->Empty();
	//				pWndText->ResetString();
				}
			}
			break;

	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 
void CWndMessage::AddMessage( LPCTSTR lpszFrom, LPCTSTR lpszMessage )
{
	CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT );
	CString strMessage;

	if( !strcmp( lpszFrom, g_pPlayer->GetName() ) )
		strMessage.Format( "#cffff0000%s%s :#nc\n  %s\n", lpszFrom, prj.GetText(TID_GAME_FROM3), lpszMessage );
	//  	strMessage.Format( "#cffff0000%s님의 말 :#nc\n  %s\n", lpszFrom, lpszMessage );
	else
		strMessage.Format( "#cff0000ff%s%s :#nc\n  %s\n", lpszFrom, prj.GetText(TID_GAME_FROM3), lpszMessage );
	//  	strMessage.Format( "#cff0000ff%s님의 말 :#nc\n  %s\n", lpszFrom, lpszMessage );
	
	pWndText->AddString( strMessage );
	pWndText->m_wndScrollBar.SetMaxScrollPos();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		쪽지창
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************
WndId : APP_MESSAGE_NOTE - 쪽지보내기창
CtrlId : WIDC_EDIT1 - 
CtrlId : WIDC_BUTTON1 - Button
CtrlId : WIDC_TEXT1 - 
****************************************************/

CWndMessageNote::CWndMessageNote() 
{ 
} 
CWndMessageNote::~CWndMessageNote() 
{ 
} 
void CWndMessageNote::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndMessageNote::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// 여기에 코딩하세요
//	m_pWndText = (CWndText*)GetDlgItem( WIDC_TEXT1 );
	m_pEdit = (CWndEdit*)GetDlgItem( WIDC_EDIT1 );
	m_pEdit->AddWndStyle( EBS_WANTRETURN );//| WBS_VSCROLL  );
	m_pEdit->SetNativeMode();
	m_pEdit->SetFocus();
	CString strTitle;
	strTitle.Format( "%s - %s", GetTitle(), m_szName );
	SetTitle( strTitle );
	// 윈도를 중앙으로 옮기는 부분.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// 처음 이 함수를 부르면 윈도가 열린다.
BOOL CWndMessageNote::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy에서 설정한 리소스로 윈도를 연다.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_MESSAGE_NOTE, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
직접 윈도를 열때 사용 
BOOL CWndMessageNote::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
CRect rectWindow = m_pWndRoot->GetWindowRect(); 
CRect rect( 50 ,50, 300, 300 ); 
SetTitle( _T( "title" ) ); 
return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndMessageNote::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndMessageNote::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndMessageNote::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndMessageNote::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndMessageNote::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_BUTTON1 )
	{
		LPSTR lpSendMessage = (LPSTR)m_pEdit->GetString();
		int adf = strlen( lpSendMessage );
		if( strlen( lpSendMessage ) <= 255 )
		{
			g_DPlay.SendMessageNote( m_dwUserId, (LPSTR)m_pEdit->GetString() );
		}
		else
		{
			// 에러 메세지 
			g_WndMng.PutString( prj.GetText( TID_GAME_MESSFULLMSG ), NULL, prj.GetTextColor( TID_GAME_MESSFULLMSG ) );
		}
		Destroy();
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

/****************************************************
WndId : APP_MESSENGER_NOTE - 쪽지창
CtrlId : WIDC_TEXT1 - 
****************************************************/

CWndMessengerNote::CWndMessengerNote() 
{ 
} 
CWndMessengerNote::~CWndMessengerNote() 
{ 
} 
void CWndMessengerNote::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndMessengerNote::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// 여기에 코딩하세요
	
	for( int i = 0 ; i < g_Neuz.m_nTagCount ; i++ )
	{
		CWndText * pWndText = (CWndText*)GetDlgItem( WIDC_TEXT1 );
		pWndText->AddWndStyle( EBS_WANTRETURN );
		CString strMessage;
		strMessage.Format( "%s - %d / %02d / %02d", g_Neuz.m_strTagName[ i ], g_Neuz.m_dwtegDate[ i ] / 10000, ( g_Neuz.m_dwtegDate[ i ] % 10000 ) / 100,  ( g_Neuz.m_dwtegDate[ i ] % 10000 ) % 100 );
		pWndText->AddString( strMessage, 0xff804000 );
		pWndText->AddString( "\n  " );
		pWndText->AddString( g_Neuz.m_strTagMessage[ i ] );
		pWndText->AddString( "\n\n" );
		
//		g_Neuz.m_dwtegDate[ i ]
//		g_Neuz.m_strTagMessage[ i ]
	}
	
	// 윈도를 중앙으로 옮기는 부분.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// 처음 이 함수를 부르면 윈도가 열린다.
BOOL CWndMessengerNote::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy에서 설정한 리소스로 윈도를 연다.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_MESSENGER_NOTE, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
직접 윈도를 열때 사용 
BOOL CWndMessengerNote::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
CRect rectWindow = m_pWndRoot->GetWindowRect(); 
CRect rect( 50 ,50, 300, 300 ); 
SetTitle( _T( "title" ) ); 
return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndMessengerNote::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndMessengerNote::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndMessengerNote::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndMessengerNote::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndMessengerNote::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 


