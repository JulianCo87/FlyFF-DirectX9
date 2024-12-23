#include "stdafx.h"

#include "WndGuildTabInfo.h"
#include "WndGuildTabApp.h"
#include "WndGuildTabWar.h"
#include "WndGuild.h"

#include "guild.h"
extern	CGuildMng	g_GuildMng;

/****************************************************
  WndId : APP_GUILD - ���â
  CtrlId : WIDC_TABCTRL1 - TabCtrl
****************************************************/
CWndGuild::CWndGuild() 
{ 
	FLINFO_LOG( PROGRAM_NAME, _T( "CWndGuild::CWndGuild()" ) );
} 
CWndGuild::~CWndGuild() 
{ 
} 
void CWndGuild::OnDraw( C2DRender* p2DRender ) 
{ 
} 

void CWndGuild::UpdateDataAll()
{
	m_WndGuildTabInfo.UpdateData();
	m_WndGuildTabApp.UpdateData();
	m_WndGuildTabMember.UpdateData();
	m_WndGuildTabWar.UpdateData();

	m_WndGuildTabPower.UpdateData( );
}

void CWndGuild::OnInitialUpdate() 
{ 
	FLINFO_LOG( PROGRAM_NAME, _T( "IN" ) );
	FLINFO_LOG( PROGRAM_NAME, _T( "(CWndNeuz::OnInitialUpdate()) - IN" ) );
	CWndNeuz::OnInitialUpdate(); 
	FLINFO_LOG( PROGRAM_NAME, _T( "(CWndNeuz::OnInitialUpdate()) - OUT" ) );
	// ���⿡ �ڵ��ϼ���
	
	CWndTabCtrl* pWndTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );

	CRect rect = GetClientRect();
	rect.left = 5;
	rect.top = 0;
	//pWndTabCtrl->Create( WBS_NOFRAME, rect, this, 12 );
	
	m_WndGuildTabInfo.Create( WBS_CHILD | WBS_NODRAWFRAME, rect, pWndTabCtrl, APP_GUILD_TABINFO );
	m_WndGuildTabApp.Create( WBS_CHILD | WBS_NODRAWFRAME, rect, pWndTabCtrl, APP_GUILD_TABAPPELLATION );
//sun: 12, ��� â�� ��� �ܿ� �׸� ����
	m_WndGuildTabMember.Create( WBS_CHILD | WBS_NODRAWFRAME, rect, pWndTabCtrl, APP_GUILD_TABMEMBER_EX );

	m_WndGuildTabWar.Create( WBS_CHILD | WBS_NODRAWFRAME, rect, pWndTabCtrl, APP_GUILD_TABGUILDWAR );

	m_WndGuildTabPower.Create( WBS_CHILD | WBS_NODRAWFRAME, rect, pWndTabCtrl, APP_GUILD_TAPPOWER );
	
	WTCITEM tabTabItem;
	
	tabTabItem.mask = WTCIF_TEXT | WTCIF_PARAM;
	tabTabItem.pszText = prj.GetText(TID_GAME_TOOLTIP_GUILDINFO);
	tabTabItem.pWndBase = &m_WndGuildTabInfo;
	pWndTabCtrl->InsertItem( 0, &tabTabItem );


	tabTabItem.pszText = prj.GetText(TID_GAME_TOOLTIP_GUILDMEMBER);
	tabTabItem.pWndBase = &m_WndGuildTabMember;
	pWndTabCtrl->InsertItem( 1, &tabTabItem );
	
	tabTabItem.pszText = prj.GetText(TID_GAME_TOOLTIP_APPELLATION);
	tabTabItem.pWndBase = &m_WndGuildTabApp;
	pWndTabCtrl->InsertItem( 2, &tabTabItem );

	tabTabItem.pszText = prj.GetText(TID_GAME_TOOLTIP_GUILDWAR);
	tabTabItem.pWndBase = &m_WndGuildTabWar;
	pWndTabCtrl->InsertItem( 3, &tabTabItem );

	tabTabItem.pszText = prj.GetText(TID_GAME_TOOLTIP_AGIT);
	tabTabItem.pWndBase = &m_WndGuildTabPower;
	pWndTabCtrl->InsertItem( 4, &tabTabItem );


	if( g_WndMng.m_bAllAction == FALSE )
	{
		Destroy();
		return;
	}
	
	// ������ �߾����� �ű�� �κ�.
	MoveParentCenter();

	{
		CRect r = GetWindowRect( TRUE );
		m_WndGuildTabMember.m_rectParent = r;
	}	

	FLINFO_LOG( PROGRAM_NAME, _T( "OUT" ) );
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndGuild::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	if( g_eLocal.GetState( EVE_SCHOOL ) )
		return FALSE;

	// Daisy���� ������ ���ҽ��� ������ ����.
	FLINFO_LOG( PROGRAM_NAME, _T( "IN" ) );
	BOOL bFlag = CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_GUILD, 0, CPoint( 0, 0 ), pWndParent );
	FLINFO_LOG( PROGRAM_NAME, _T( "OUT" ) );

	return bFlag;
} 
/*
  ���� ������ ���� ��� 
BOOL CWndGuild::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndGuild::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndGuild::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndGuild::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndGuild::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndGuild::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 
void CWndGuild::OnMouseMove(UINT nFlags, CPoint point )
{
#ifdef _DEBUG
	m_WndGuildTabInfo.m_nMx = point.x;
	m_WndGuildTabInfo.m_nMy = point.y;
#endif
}

void CWndGuild::SetCurTab( int index )
{
	CWndTabCtrl* pWndTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
	FLASSERT( pWndTabCtrl );

	pWndTabCtrl->SetCurSel( index );
}







//---------------------------------------------------------------------------------------------------------------------------
// ��� ��û Ȯ��â
//---------------------------------------------------------------------------------------------------------------------------
CWndGuildConfirm::CWndGuildConfirm() 
{ 
	m_idMaster = 0;
//	m_idPlayer = 0;
} 
CWndGuildConfirm::~CWndGuildConfirm() 
{ 
} 
void CWndGuildConfirm::SetGuildName( char* pGuildName )
{
	strGuildName.Format( "%s", pGuildName );
}
void CWndGuildConfirm::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndGuildConfirm::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	CWndEdit* pEdit = (CWndEdit*)GetDlgItem( WIDC_STATIC3 );
	pEdit->SetTitle( strGuildName );
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( ( rectRoot.right - rectWindow.Width() ) / 2, 70 );
	Move( point );		
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndGuildConfirm::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_GUILD_INVATE, 0, CPoint( 0, 0 ), pWndParent );
} 
BOOL CWndGuildConfirm::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndGuildConfirm::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndGuildConfirm::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndGuildConfirm::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndGuildConfirm::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_OK || message == EN_RETURN )
	{
		if( g_pPlayer )
		{
//			g_DPlay.SendAddGuildMember( m_idMaster, m_idPlayer );
			GUILD_MEMBER_INFO	info;
			info.idPlayer	= g_pPlayer->m_idPlayer;
			g_DPlay.SendAddGuildMember( m_idMaster, info, g_pPlayer->IsAuthorization( AUTH_GAMEMASTER ) );
		}
		Destroy();
	}
	else if( nID == WIDC_NO )
	{
		Destroy();
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 
