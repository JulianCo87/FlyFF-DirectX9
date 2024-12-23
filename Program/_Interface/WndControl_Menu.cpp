
#include "stdafx.h"
#include "WndControl_Menu.h"
#include "WndControl_Button.h"

#include "GuildHouse.h"

namespace WndMenu
{
	static const BYTE		BUTTON_HEIGHT_SIZE( WndButton::ICONIMAGE_SIZE );					// ��ư ����
	static const BYTE		BUTTON_BETWEEN_SIZE( 2 );											// ��ư ���� �Ÿ�

	static const BYTE		BUTTON_TOP_OFFSET( 10 );											// ��ư �� ������
	static const BYTE		BUTTON_BOTTOM_OFFSET( BUTTON_TOP_OFFSET + BUTTON_BETWEEN_SIZE );	// ��ư �Ʒ� ������
	static const BYTE		BUTTON_LEFT_OFFSET( 10 );											// ��ư ���� ������

	static const BYTE		BUTTONWIDTH_ADDSIZE( 10 );											// ��ư ���� �߰� ����
	static const BYTE		WNDWIDTH_ADDSIZE( 8 );												// ��ư ���� �߰� ����
}


using namespace WndMenu;
using namespace WndButton;


//---------------------------------------------------------------------------------------------
// ������
// ##	�Ŵ��� ������ m_wndAray�� ������ ȭ�� ��ǥ�� ���� ��ǥ��. 
//		���� m_rectWindow, m_rectClient�� ȭ�� �󿡼��� ��ǥ�̹Ƿ� �ٷ� ����ص� �ȴ�.
//---------------------------------------------------------------------------------------------
CWndMenu::CWndMenu()
:	m_nLargeWidth( NULL )
,	m_wButtonWidth( NULL )
{
	m_byWndType = WTYPE_MENU;
	ZeroMemory( m_pTextureBackground, sizeof( CTexture* ) * WndMenu::MAX_BACKGROUND_TEXTRUE_COUNT );
}


//---------------------------------------------------------------------------------------------
// �ı���
//---------------------------------------------------------------------------------------------
CWndMenu::~CWndMenu()
{
	for( int i = 0; i < m_awndMenuItem.GetSize(); i++)
		SAFE_DELETE_CONST( GetMenuItem( i ) );
}


//---------------------------------------------------------------------------------------------
// �޴� �����ϱ�
// param	: �θ� ������
// return	: BOOL 
//---------------------------------------------------------------------------------------------
BOOL CWndMenu::CreateMenu( CWndBase* pWndParent )
{
	BOOL bResult = CWndBase::Create( WBS_POPUP, CRect( 0, 0, 0, 0 ), pWndParent, 10 );
	SetVisible( FALSE );

	return bResult;
}


//---------------------------------------------------------------------------------------------
// �޴� �����ϱ�
// param	: nPosition, nFlags
// return	: BOOL 
//---------------------------------------------------------------------------------------------
BOOL CWndMenu::DeleteMenu(UINT nPosition, UINT nFlags)
{	
	SAFE_DELETE_CONST( GetMenuItem( nPosition ) );

	m_awndMenuItem.RemoveAt( nPosition );

	for( int i = nPosition; i < m_awndMenuItem.GetSize(); i++)
	{
		CWndButton* pWndMenuItem( GetMenuItem( i ) );
		FLERROR_ASSERT_LOG_DO( pWndMenuItem, continue, _T("[������ �޴�] ������ �޴� �����Ͱ� ��ȿ���� �ʽ��ϴ�."))

		CRect rtRect = pWndMenuItem->GetWindowRect( TRUE );
		rtRect.OffsetRect( 0, -ICONIMAGE_SIZE);
		pWndMenuItem->SetWndRect( rtRect );  
	}

	CRect rtRect( GetWindowRect(TRUE) );
	rtRect.OffsetRect( 0, ICONIMAGE_SIZE);
	rtRect.bottom -= ICONIMAGE_SIZE;
	SetWndRect( rtRect );  

	return TRUE;
}


//---------------------------------------------------------------------------------------------
// ��� �޴� �����ϱ�
// param	: void
// return	: void 
//---------------------------------------------------------------------------------------------
void CWndMenu::DeleteAllMenu()
{	
	for( int i = 0; i < m_awndMenuItem.GetSize(); i++)
		SAFE_DELETE_CONST( GetMenuItem( i ) );

	m_awndMenuItem.RemoveAll();

	m_wButtonWidth = NULL;
}


//---------------------------------------------------------------------------------------------
// ���� �޴� ���̱� �����ϱ�
// param	: ���̱�
// return	: void 
//---------------------------------------------------------------------------------------------
void CWndMenu::SetVisibleSub( BOOL bVisible )
{
	CWndBase::SetVisible( bVisible );
	for( int i = 0; i < m_awndMenuItem.GetSize(); i++)
	{
		if( GetMenuItem( i )->m_pWndMenu )
			GetMenuItem( i )->m_pWndMenu->SetVisibleSub( bVisible );
	}
}


//---------------------------------------------------------------------------------------------
// ��� ���� �޴� ���̱� �����ϱ�
// param	: ���̱�
// return	: void 
//---------------------------------------------------------------------------------------------
void CWndMenu::SetVisibleAllMenu( BOOL bVisible )
{
	CWndMenu* pMenu = this;
	CWndMenu* pMenu2 = this;
	while( pMenu2 && pMenu2->IsWndStyle( WBS_POPUP ) )
	{
		pMenu = pMenu2;
		pMenu2 = (CWndMenu*) pMenu->GetParentWnd();
	}
	if( pMenu )
	{
		pMenu->CWndBase::SetVisible( bVisible );
		for( int i = 0; i < pMenu->m_awndMenuItem.GetSize(); i++)
		{
			if( pMenu->GetMenuItem( i )->m_pWndMenu )
				pMenu->GetMenuItem( i )->m_pWndMenu->SetVisibleSub( bVisible );
		}
	}
}


//---------------------------------------------------------------------------------------------
// �޴� �߰��ϱ�
// param	: ���̱�
// return	: void 
//---------------------------------------------------------------------------------------------
CWndButton* CWndMenu::AppendMenu(UINT nFlags, UINT nIDNewItem,	LPCTSTR lpszNewItem )
{
	FLERROR_ASSERT_LOG_RETURN( lpszNewItem, NULL, _T("[������ �޴�] ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

	CWndButton* pWndButton = new CWndButton;
	FLERROR_ASSERT_LOG_RETURN( pWndButton, NULL, _T("[������ �޴�] ������ ��ư ���� ����") );

	if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
	{
		int nCount = m_awndMenuItem.GetSize();
		CSize size = m_pFont->GetTextExtent( lpszNewItem );
	
		if( size.cx + 60 > m_nLargeWidth )
			m_nLargeWidth = size.cx + 60;

		pWndButton->Create(lpszNewItem, WBS_MENUITEM | WBS_HIGHLIGHT/* | WBS_NOMENUICON*/, CRect( 2, 2 + ( nCount * 22 ), m_nLargeWidth, 2 + ( nCount * 22 ) + 20 ), this, nIDNewItem );
		pWndButton->DelWndStyle( WBS_NODRAWFRAME );
		m_awndMenuItem.Add( pWndButton );

		for( int i = 0; i < m_awndMenuItem.GetSize(); i++ )
		{
			pWndButton = (CWndButton*)m_awndMenuItem.GetAt( i );
			CRect rect = pWndButton->GetWindowRect( TRUE );
			rect.right = rect.left + m_nLargeWidth; 
			pWndButton->SetWndRect( rect );
		}

		SetWndRect( CRect( 0, 0, m_nLargeWidth, 5 + 5 + ( ( nCount + 1 ) * 22 ) ) );
	}
	
	if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
	{
		CSize	StringSize( m_pFont->GetTextExtent( lpszNewItem ) );
		WORD	wCurrentMenuItemCount( m_awndMenuItem.GetSize() );
		WORD	wButtonWidth( static_cast<WORD>( ICONIMAGE_SIZE + StringSize.cx + HOTKEY_OFFSET + BUTTONWIDTH_ADDSIZE ) );

		// ��ư ���α��� üũ
		if( wButtonWidth > m_wButtonWidth )	
			m_wButtonWidth = wButtonWidth;

		// �ٸ� ��ư�� �ٲ� ���α��� ����
		for( int i = 0; i < m_awndMenuItem.GetSize(); i++ )
		{
			CWndButton* pWndButton = (CWndButton*)m_awndMenuItem.GetAt( i );
			FLERROR_ASSERT_LOG_DO( pWndButton, continue, _T("[������ �޴�] �޴� ��ư ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

			CRect rtRect = pWndButton->GetWindowRect( TRUE );
			rtRect.right = rtRect.left + m_wButtonWidth; 
			pWndButton->SetWndRect( rtRect );
		}

		WORD	wTotalButtonHeightSize( wCurrentMenuItemCount * BUTTON_HEIGHT_SIZE );
		WORD	wTotalButtonBetweenSize( ((wCurrentMenuItemCount * 2) + 1) * BUTTON_BETWEEN_SIZE );

		WORD	wTop( BUTTON_TOP_OFFSET + wTotalButtonBetweenSize + wTotalButtonHeightSize);
		WORD	wLeft( BUTTON_LEFT_OFFSET ); 
		WORD	wRight( BUTTON_LEFT_OFFSET + m_wButtonWidth );
		WORD	wBottton( wTop + BUTTON_HEIGHT_SIZE );

		CRect	rtButtonRect( wLeft, wTop, wRight, wBottton );

		// ��ư ����
		pWndButton->Create(lpszNewItem, WBS_MENUITEM | WBS_HIGHLIGHT, rtButtonRect, this, nIDNewItem );
		pWndButton->DelWndStyle( WBS_NODRAWFRAME );
		m_awndMenuItem.Add( pWndButton );

		// ������ ����
		// ��� �ؽ��� ���̰� ������ ���̿� ������ �� ���������� Ȯ��
		WORD	wWndAddSize( NULL );
		WORD	wWndWidth( BUTTON_LEFT_OFFSET + m_wButtonWidth + WNDWIDTH_ADDSIZE );

		CTexture* pTexture = m_pTextureBackground[0];
		if( pTexture )
		{
			WORD wRemainWidth( static_cast<WORD>( wWndWidth % pTexture->m_size.cy ) );

			if( NULL != wRemainWidth )
				wWndAddSize = static_cast<WORD>( pTexture->m_size.cy - wRemainWidth );
		}

		WORD	nWndRight( wWndWidth + wWndAddSize );
		WORD	nWndBottom( BUTTON_TOP_OFFSET + BUTTON_BOTTOM_OFFSET + wTotalButtonBetweenSize + wTotalButtonHeightSize + BUTTON_HEIGHT_SIZE ); 

		SetWndRect( CRect( 0, 0, nWndRight, nWndBottom ) );
	}

	return pWndButton;
}


//---------------------------------------------------------------------------------------------
// �޴� �����ϱ�
// param	: ���̱�
// return	: void 
//---------------------------------------------------------------------------------------------
BOOL CWndMenu::InsertMenu(UINT nPosition, UINT nFlags, UINT nIDNewItem,	LPCTSTR lpszNewItem )
{
	if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
	{
		CWndButton* pWndButton = new CWndButton;
		pWndButton->Create(lpszNewItem, WBS_MENUITEM | WBS_HIGHLIGHT, CRect( 5, 50 + 5 + ( nPosition * 22 ), 175, 50 + 5 + ( nPosition * 22 ) + 20 ), this, nIDNewItem );
		pWndButton->DelWndStyle( WBS_NODRAWFRAME );
		
		m_awndMenuItem.InsertAt( nPosition, pWndButton );
		
		for( int i = nPosition + 1; i < m_awndMenuItem.GetSize(); i++)
		{
			CWndButton* pWndMenuItem = GetMenuItem( i );
			CRect rect = pWndMenuItem->GetWindowRect(TRUE);
			rect.OffsetRect( 0, 22);
			pWndMenuItem->SetWndRect( rect );  
		}
		CRect rect = GetWindowRect(TRUE);
		rect.OffsetRect( 0, -22);
		rect.bottom += 22;
		SetWndRect( rect );  
	}
		
	if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
	{
		CSize	StringSize( m_pFont->GetTextExtent( lpszNewItem ) );
		WORD	wButtonWidth( static_cast<WORD>( StringSize.cx + HOTKEY_OFFSET + ICONIMAGE_SIZE + BUTTON_LEFT_OFFSET + BUTTONWIDTH_ADDSIZE ) );

		if( wButtonWidth > m_wButtonWidth )
			m_wButtonWidth = wButtonWidth;

		for( int i = 0; i < m_awndMenuItem.GetSize(); i++ )
		{
			CWndButton* pWndButton = (CWndButton*)m_awndMenuItem.GetAt( i );
			FLERROR_ASSERT_LOG_DO( pWndButton, continue, _T("[������ �޴�] �޴� ��ư ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

			CRect rtRect = pWndButton->GetWindowRect( TRUE );
			rtRect.right = rtRect.left + m_wButtonWidth; 
			pWndButton->SetWndRect( rtRect );
		}

		CWndButton* pWndButton = new CWndButton;
		FLERROR_ASSERT_LOG_RETURN( pWndButton, FALSE, _T("[������ �޴�] ������ ��ư �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
		
		CWndButton* pWndMenuItem = GetMenuItem( nPosition );
		FLERROR_ASSERT_LOG_RETURN( pWndMenuItem, FALSE, _T("[������ �޴�] ã�� �޴� �������� �������� �ʽ��ϴ� - ��ġ [ %u ]"), nPosition );

		pWndButton->Create(lpszNewItem, WBS_MENUITEM | WBS_HIGHLIGHT, pWndMenuItem->GetClientRect( TRUE ), this, nIDNewItem );
		pWndButton->DelWndStyle( WBS_NODRAWFRAME );

		m_awndMenuItem.InsertAt( nPosition, pWndButton );

		for( int nIndex = nPosition + 1; nIndex < m_awndMenuItem.GetSize(); nIndex++ )
		{
			CWndButton* pWndMenuItem = GetMenuItem( nIndex );
			FLERROR_ASSERT_LOG_DO( pWndButton, continue, _T("[������ �޴�] �޴� ��ư ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
		
			CRect rtRect = pWndMenuItem->GetWindowRect( TRUE );
			rtRect.OffsetRect( 0, BUTTON_HEIGHT_SIZE + ( BUTTON_BETWEEN_SIZE * 2 ) );
			pWndMenuItem->SetWndRect( rtRect );  
		}

		// ������ ����
		WORD	wCurrentMenuItemCount( m_awndMenuItem.GetSize() );
		WORD	wTotalButtonHeightSize( wCurrentMenuItemCount * BUTTON_HEIGHT_SIZE );
		WORD	wTotalButtonBetweenSize( ((wCurrentMenuItemCount * 2) + 1) * BUTTON_BETWEEN_SIZE );

		// ��� �ؽ��� ���̰� ������ ���̿� ������ �� ���������� Ȯ��
		WORD	wWndAddSize( NULL );
		WORD	wWndWidth( BUTTON_LEFT_OFFSET + m_wButtonWidth + WNDWIDTH_ADDSIZE  );

		CTexture* pTexture = m_pTextureBackground[0];
		if( pTexture )
		{
			WORD wRemainWidth( static_cast<WORD>( wWndWidth % pTexture->m_size.cy ) );

			if( NULL != wRemainWidth )
				wWndAddSize = static_cast<WORD>( pTexture->m_size.cy - wRemainWidth );
		}


		WORD	nWndRight( wWndWidth + wWndAddSize );
		WORD	nWndBottom( BUTTON_TOP_OFFSET + BUTTON_BOTTOM_OFFSET + wTotalButtonBetweenSize + wTotalButtonHeightSize + BUTTON_HEIGHT_SIZE ); 

		SetWndRect( CRect( 0, 0, nWndRight, nWndBottom ) );
	}

	return TRUE;
}


//---------------------------------------------------------------------------------------------
// �ʱ�ȭ
// param	: void
// return	: void 
//---------------------------------------------------------------------------------------------
void CWndMenu::OnInitialUpdate()
{
	LoadBackgroundTexture();
	CWndBase::OnInitialUpdate();
}


//---------------------------------------------------------------------------------------------
// ��� �ؽ��� �ε��ϱ�
// param	: void
// return	: void 
//---------------------------------------------------------------------------------------------
void CWndMenu::LoadBackgroundTexture()
{
	CString strTexture( _T("") );

	for( UINT nIndex = 0; nIndex < WndMenu::MAX_BACKGROUND_TEXTRUE_COUNT; nIndex++ )
	{
		strTexture.Format( _T("WndTile%02d.tga"), nIndex );
		m_pTextureBackground[nIndex] = m_textureMng.AddTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(),strTexture ), WNDCOLOR_DEFAULT_KEY, TRUE );
		FLERROR_ASSERT_LOG_DO( m_pTextureBackground[nIndex],	continue, _T("[������ �޺��ڽ�] ��� �ؽ��� ���� ���� - [ %s ]."), strTexture );	

		strTexture = _T("");
	}	
}


//---------------------------------------------------------------------------------------------
// �׸���
// param	: ������
// return	: void 
//---------------------------------------------------------------------------------------------
void CWndMenu::PaintFrame( C2DRender* p2DRender )
{
	if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		p2DRender->RenderFillRect( GetWindowRect(), D3DCOLOR_ARGB( 255, 226,198,181 ) );

	if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		DrawBackground( p2DRender );
}


//---------------------------------------------------------------------------------------------
// ��� ���� ��
// param	: ������
// return	: void 
//---------------------------------------------------------------------------------------------
BOOL CWndMenu::OnEraseBkgnd( C2DRender* p2DRender )
{
	if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		p2DRender->RenderFillRect( GetClientRect(), D3DCOLOR_ARGB( 255, 255, 0, 0 ) );

	return TRUE;
}


//---------------------------------------------------------------------------------------------
// ��� �׸��� 
// param	: ������
// return	: void 
//---------------------------------------------------------------------------------------------
void CWndMenu::DrawBackground( C2DRender* p2DRender )
{
	if( FALSE == m_bVisible )
		return ;

	CTexture* pTexture = m_pTextureBackground[0];
	FLERROR_ASSERT_LOG_RETURN_VOID( pTexture, _T("[������ �޺��ڽ�] ��� �ؽ��� �����Ͱ� ��ȿ���� ����.") );	

	CRect rtRect = GetWindowRect();
	
	int nWidthCount( rtRect.Width() / pTexture->m_size.cx );
	int nHeightCount( (rtRect.Height() / pTexture->m_size.cy) + 1);
	int nRemainHeight( pTexture->m_size.cy - ( rtRect.Height() % pTexture->m_size.cy ) );

	CPoint ptLeftTop( rtRect.left, rtRect.top );
	for( int nHeight = 0; nHeight < nHeightCount; nHeight++ )
	{
		for( int nWidth = 0; nWidth < nWidthCount; nWidth++ )
		{
			if( nHeight == 0 )							// �� ���� �� �׸���
			{
				if( nWidth == 0 )
					m_pTextureBackground[ 0 ]->Render( p2DRender, ptLeftTop, CWndBase::m_nAlpha );
				else if( nWidth == (nWidthCount - 1) )
					m_pTextureBackground[ 2 ]->Render( p2DRender, ptLeftTop, CWndBase::m_nAlpha );
				else
					m_pTextureBackground[ 1 ]->Render( p2DRender, ptLeftTop, CWndBase::m_nAlpha );
			
			}
			else if( nHeight == (nHeightCount - 1) )	// ������ �� �׸���
			{
				if( nWidth == 0 )
					m_pTextureBackground[ 9 ]->Render( p2DRender, ptLeftTop, CWndBase::m_nAlpha );
				
				else if( nWidth == (nWidthCount - 1) )
					m_pTextureBackground[ 11 ]->Render( p2DRender, ptLeftTop, CWndBase::m_nAlpha );
				else
					m_pTextureBackground[ 10 ]->Render( p2DRender, ptLeftTop, CWndBase::m_nAlpha );
			}
			else if( nHeight == (nHeightCount - 2) )	// ������ �ٷ� ���� �� �׸���	
			{
				// ������ ����� ��� �ؽ��� ������� ����� �������� ���� ��� 
				// �� �ؿ��ٰ� �� �ٷ� �������� ���� �÷� ��� �ؽ����� ����� �����
				if( nWidth == 0 )
				{
					p2DRender->RenderTextureUVEx(	ptLeftTop,
													CPoint( m_pTextureBackground[ 6 ]->m_size.cx, m_pTextureBackground[ 6 ]->m_size.cy - nRemainHeight ), 
													m_pTextureBackground[ 6 ],
													0.0f,
													0.0f,
													1.0f,
													1.0f
												);
				}
				else if( nWidth == (nWidthCount - 1) )
				{
					p2DRender->RenderTextureUVEx(	ptLeftTop,
													CPoint( m_pTextureBackground[ 8 ]->m_size.cx, m_pTextureBackground[ 8 ]->m_size.cy - nRemainHeight ), 
													m_pTextureBackground[ 8 ],
													0.0f,
													0.0f,
													1.0f,
													1.0f
												);
				}
				else
				{
					p2DRender->RenderTextureUVEx(	ptLeftTop,
													CPoint( m_pTextureBackground[ 7 ]->m_size.cx, m_pTextureBackground[ 7 ]->m_size.cy - nRemainHeight ), 
													m_pTextureBackground[ 7 ],
													0.0f,
													0.0f,
													1.0f,
													1.0f
												);
				}
			}
			else
			{
				if( nWidth == 0 )
					m_pTextureBackground[ 6 ]->Render( p2DRender, ptLeftTop, CWndBase::m_nAlpha );
				else if( nWidth == (nWidthCount - 1) )
					m_pTextureBackground[ 8 ]->Render( p2DRender, ptLeftTop, CWndBase::m_nAlpha );
				else
					m_pTextureBackground[ 7 ]->Render( p2DRender, ptLeftTop, CWndBase::m_nAlpha );
			}
			ptLeftTop.x += pTexture->m_size.cx;	
		}
		ptLeftTop.x = rtRect.left;
		ptLeftTop.y += pTexture->m_size.cy;

		if( nHeight == (nHeightCount - 2) )
			ptLeftTop.y = ptLeftTop.y - nRemainHeight;
	}
}


UINT CWndMenu::CheckMenuItem(UINT nIDCheckItem, UINT nCheck)
{
	CWndButton* pWndButton = (CWndButton*)m_awndMenuItem.GetAt( nIDCheckItem );
	pWndButton->SetCheck( nCheck );
	return TRUE;
}


UINT CWndMenu::GetMenuState(UINT nID, UINT nFlags) const
{
	CWndButton* pWndButton = (CWndButton*)m_awndMenuItem.GetAt( nID );
	return pWndButton->GetCheck();//( nCheck );
}

CWndButton* CWndMenu::GetMenuItem( int nPos )
{
	return (CWndButton*) m_awndMenuItem.GetAt( nPos );
}

void CWndMenu::OnKillFocus(CWndBase* pNewWnd)
{
	if( pNewWnd == NULL )
		SetVisibleAllMenu( FALSE );
	else
	if( pNewWnd->IsWndStyle( WBS_POPUP ) == FALSE )
		SetVisibleAllMenu( FALSE );
}

BOOL CWndMenu::OnChildNotify(UINT message,UINT nID,LRESULT* pLResult)
{
	BOOL bResult = FALSE;
	if( m_pParentWnd )
	{
		bResult = m_pParentWnd->OnChildNotify( message, nID, pLResult );
		m_pParentWnd->SetFocus();
	}

	return bResult;
}
BOOL CWndMenu::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase )
{	
	//gmpbigsun: ����Ͽ�¡ ��Ʈ�� ������Ʈ�� ���� �޼������ĸ� ����, ���ο������� �۵��ؾ� �ϱ⶧����...
	if( MMI_GHOUSE_INFO == nID )
	{
		// sgook : ��尡��â�� �����ָ� ��ٰ� ��. 2009_11_18
		CWndGuildHousing* pWnd = (CWndGuildHousing*)g_WndMng.GetApplet( APP_GH_FURNITURE_STORAGE );
		if( !pWnd )
		{
			g_WndMng.CreateApplet( APP_GH_FURNITURE_STORAGE ); 
			pWnd = (CWndGuildHousing*)g_WndMng.GetApplet( APP_GH_FURNITURE_STORAGE );
			FLASSERT( pWnd );

			pWnd->InitBySection( CWndGuildHousing::GS_FURNITURE );
		}

	}
	else if( MMI_GHOUSE_REINSTALL == nID )		//�缳ġ
	{
		GuildHouse->Reset( );
		return TRUE;
	}
	else if( MMI_GHOUSE_RECALL == nID )		//ȸ�� 
	{
		GuildHouse->Remove( );
		return TRUE;
	}

	BOOL bResult = FALSE;
	if( m_pParentWnd )
	{
		bResult = m_pParentWnd->OnCommand( nID, dwMessage, this );
		if(bResult != -1)
			m_pParentWnd->SetFocus();
	}
	return bResult;
}

void CWndMenu::SetWndRect( CRect rectWnd, BOOL bOnSize )
{	
	//gmpbigsun( 20110322 ) --BEGIN
	//#0012525: [����_����_17��] ����(PC)���� ��Ŭ�� ������ �ߴ� UI(�ŷ��ϱ�,�ش���û ���..) �� ©���� ������ ����.
	CWndBase* pWndWorld = g_WndMng.GetWndBase( APP_WORLD );
	if( pWndWorld )
	{
		CRect rectWorld = pWndWorld->GetWndRect( );

		if( rectWnd.right > rectWorld.right )	//�������� �Ѿ���
		{
			CRect rectCorrect = rectWnd;
			rectCorrect.left = rectWnd.left - rectWnd.Width();
			rectCorrect.right = rectCorrect.left + rectWnd.Width();

			rectWnd = rectCorrect;
		}
		else if( rectWnd.left < rectWorld.left )	//�������� �Ѿ ���
		{
			CRect rectCorrect = rectWnd;
			rectCorrect.left = rectWnd.left + rectWnd.Width();
			rectCorrect.right = rectCorrect.left + rectWnd.Width();

			rectWnd = rectCorrect;
		}
	}
	// --END

	CRect rectOld = m_rectClient;
	m_rectWindow = rectWnd;
	m_rectClient = m_rectWindow;
	/// �� Ȯ��
	//m_rectClient.DeflateRect( 3, 3 );
	m_rectLayout = m_rectClient;
	if( bOnSize && ( rectOld.Width() != m_rectClient.Width() || rectOld.Height() != m_rectClient.Height() ) )
		OnSize( 0, m_rectClient.Width(), m_rectClient.Height() );
	
	MakeVertexBuffer();
}


