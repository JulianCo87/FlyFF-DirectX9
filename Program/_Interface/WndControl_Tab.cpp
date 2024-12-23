
#include "StdAfx.h"
#include "WndControl_Tab.h"

//---------------------------------------------------------------------------------------------
// ������ 
//---------------------------------------------------------------------------------------------
CWndTabCtrl::CWndTabCtrl()
:	m_nCurSelect( NULL )
,	m_nTabButtonLength( NULL )
,	m_eTabTitleAlign( ALIGN_LEFT )
{
	m_aTab.reserve( nReserveTabControlItem );
}


//---------------------------------------------------------------------------------------------
// �ı��� 
//---------------------------------------------------------------------------------------------
CWndTabCtrl::~CWndTabCtrl()
{
	for( UINT nIndex = 0; nIndex < m_aTab.size(); nIndex++ )
		SAFE_DELETE( m_aTab[nIndex] );
		
	m_aTab.clear();
}


//---------------------------------------------------------------------------------------------
// �����ϱ�
// param	: ������ ��Ÿ��, ����, �θ� ������, ������ ���̵�
// return	: BOOL 
//---------------------------------------------------------------------------------------------
BOOL CWndTabCtrl::Create(DWORD dwStyle, const RECT& rect, CWndBase* pParentWnd, UINT nID)
{
	return CWndBase::Create( dwStyle | WBS_CHILD | WBS_NOFRAME, rect, pParentWnd, nID );
}


//---------------------------------------------------------------------------------------------
// �Ϲ� �� ��Ʈ�� �ؽ��� �ε�
// param	: void
// return	: void 
//---------------------------------------------------------------------------------------------
void CWndTabCtrl::LoadTabTexture()
{
	// �� �ؽ��� ���
	//"WndTabTile00.bmp"
	//"WndTabTile01.bmp"
	//"WndTabTile02.bmp"
	//"WndTabTile03.bmp"
	//"WndTabTile04.bmp"
	//"WndTabTile05.bmp"
	
	CString strTexture( _T("") );

	for( UINT nIndex = 0; nIndex < nTabTextureCount; nIndex++ )
	{
		strTexture.Format( _T("WndTabTile%02d.bmp"), nIndex );
		BOOL bResult = m_TextureTab[nIndex].LoadTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(),strTexture ), dwTextureKeyColor, TRUE );
		FLERROR_ASSERT_LOG_DO( bResult,	continue, _T("[������ �� ��Ʈ��] �� �ؽ��� ���� ���� - [ %s ]."), strTexture );	

		strTexture = _T("");
	}

	// ���� �� �ؽ��� ���
	//"WndTabTile10.bmp"
	//"WndTabTile11.bmp"
	//"WndTabTile12.bmp"
	//"WndTabTile13.bmp"
	//"WndTabTile14.bmp"
	//"WndTabTile15.bmp"


	for( UINT nIndex = 0; nIndex < nTabTextureCount; nIndex++ )
	{
		strTexture.Format( _T("WndTabTile%d.bmp"), nIndex + 10 );
		BOOL bResult = m_TextureTabSelect[nIndex].LoadTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(),strTexture ), dwTextureKeyColor, TRUE );
		FLERROR_ASSERT_LOG_DO( bResult,	continue, _T("[������ �� ��Ʈ��] ���� �� �ؽ��� ���� ���� - [ %s ]."), strTexture );	

		strTexture = _T("");
	}
}


//---------------------------------------------------------------------------------------------
// �κ��丮 �� ��Ʈ�� �ؽ��� �ε�
// param	: void
// return	: void 
//---------------------------------------------------------------------------------------------
void CWndTabCtrl::LoadInventoryTabTexture()
{
	// �ؽ��� ���
	//"WndInventab01.tga"
	//"WndInventab02.tga"
	
	m_aInventoryTexture[0].LoadTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), "WndInventab02.tga" ), dwTextureKeyColor, TRUE );	
	m_aInventoryTexture[1].LoadTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), "WndInventab01.tga" ), dwTextureKeyColor, TRUE );	
}


//---------------------------------------------------------------------------------------------
// ������Ʈ �� �ʱ�ȭ
// param	: void
// return	: void 
//---------------------------------------------------------------------------------------------
void CWndTabCtrl::OnInitialUpdate()
{
	CWndBase* pWndBase = this->GetParentWnd();
	FLERROR_ASSERT_LOG_RETURN_VOID( pWndBase,	_T("[������ �� ��Ʈ��] �θ� ������ �����Ͱ� ��ȿ���� ����.") );	

	int nParent( pWndBase->GetWndId() );
	
	if(nParent == APP_INVENTORY)
	{
		switch(_GetContentState(CT_NEW_INVENTORY))
		{
		case CS_VER1:
			LoadTabTexture();
			return;
		case CS_VER2:
			LoadInventoryTabTexture();	
			return;
		default:
			FLASSERT(0);
			return;
		}
    }
	else
		LoadTabTexture();
}


//---------------------------------------------------------------------------------------------
// ������ ����
// param	: nItem, �� ��Ʈ�� ������
// return	: BOOL 
//---------------------------------------------------------------------------------------------
BOOL CWndTabCtrl::InsertItem( int nItem, WTCITEM* pTabCtrlItem )
{
	if( nItem >= (int)( m_aTab.size() ) )
		m_aTab.resize( nItem + 1 );

	LPWTCITEM pItem = new WTCITEM;
	FLERROR_ASSERT_LOG_RETURN( pItem, FALSE,	_T("[������ �� ��Ʈ��] ������ �� ��Ʈ�� ������ ���� ����.") );	

	memcpy( pItem, pTabCtrlItem, sizeof( WTCITEM ) );
	m_aTab[ nItem ] = pItem;

	if( pTabCtrlItem->pWndBase )
	{
		if( nItem == m_nCurSelect )
			pTabCtrlItem->pWndBase->SetVisible( TRUE );
		else
			pTabCtrlItem->pWndBase->SetVisible( FALSE );

		CRect rect = GetClientRect( TRUE );
		rect.bottom -= 18;
		rect.OffsetRect( -rect.TopLeft() );

		if( IsWndStyle( WBS_VSCROLL ) )
			rect.right -= 15;

		pItem->pWndBase->SetWndRect( rect );
		pItem->pWndBase->AddWndStyle( WBS_CHILDFRAME );

		if( m_aTab[ m_nCurSelect ] )
			m_aTab[ m_nCurSelect ]->pWndBase->SetFocus();
	}

	m_nTabButtonLength = GetClientRect().Width() / m_aTab.size();
	
	return TRUE;
}


//---------------------------------------------------------------------------------------------
// ���� �簢���� ���ϱ�
// param	: ������
// return	: BOOL 
//---------------------------------------------------------------------------------------------
CRect CWndTabCtrl::GetWndTabCtrlWindowRect()
{
	CRect rtRect( 0, 0 ,0 ,0 );
	
	rtRect = GetWindowRect();
	rtRect.right -= 15;	// ��ũ�� �� ������ �������� �ΰ� ���� �Ѥ� �̷� ����...

	return rtRect;
}


//---------------------------------------------------------------------------------------------
// �κ��丮 �� �׸���
// param	: ������
// return	: BOOL 
//---------------------------------------------------------------------------------------------
void CWndTabCtrl::DrawTabInventory( C2DRender* p2DRender )
{
	CWndBase* pWndBase = this->GetParentWnd();
	FLERROR_ASSERT_LOG_RETURN_VOID( pWndBase,	_T("[������ �� ��Ʈ��] �θ� ������ �����Ͱ� ��ȿ���� ����.") );	

	int		nParent( pWndBase->GetWndId() );
	int		nTabSize( m_aTab.size() );

	if(nParent != APP_INVENTORY)	// �κ��丮�� �ƴ϶��
		return;

	PFONTCOLOR_WNDTABCTRL pFontColorWndTabCtrl = g_WndFontColorManager->GetWndTabCtrl();

	DWORD dwDefaultFontColor( pFontColorWndTabCtrl->m_stDefault.GetFontColor() );
	DWORD dwSelectFontColor( pFontColorWndTabCtrl->m_stSelectTab.GetFontColor() );

	CRect rectClient	= GetWindowRect();
	CRect rect			= GetWindowRect();
	int y				= 0;


	switch(_GetContentState(CT_NEW_INVENTORY))
	{	
	case CS_VER1:
		{
			if( m_nCurSelect >= 0 )
			{
				rect.right -= 15;
				int nHeight = ( rect.Height() / EDIT_HEIGHT ) - 2;
				rect.top += ( nHeight * EDIT_HEIGHT );

				if( nTabSize == 0 )
					nTabSize = 1;
				int nWidth = rect.Width() / 8;
				int nSize = ( nWidth / nTabSize );
				int nCurPos = nSize * m_nCurSelect;

				static const int TEXTURE_TAB_SIZE_X = m_TextureTabSelect[ 1 ].m_size.cx;
				static const int TEXTURE_TAB_SIZE_Y = m_TextureTabSelect[ 1 ].m_size.cy;

				CPoint pointStart = rect.TopLeft();
				pointStart.x += ( TEXTURE_TAB_SIZE_X * nCurPos );

				int nFirstTabTextureIndex = ( m_nCurSelect == 0 ) ? 0 : 1;
				m_TextureTabSelect[ nFirstTabTextureIndex ].Render( p2DRender, pointStart, CWndBase::m_nAlpha );
				m_TextureTabSelect[ 1 ].Render( p2DRender, 
					CPoint( pointStart.x + TEXTURE_TAB_SIZE_X, pointStart.y ), 
					CPoint( TEXTURE_TAB_SIZE_X, TEXTURE_TAB_SIZE_Y ), 
					CWndBase::m_nAlpha, 
					static_cast<FLOAT>( nSize - 3 ));
				m_TextureTabSelect[ 2 ].Render( p2DRender, CPoint( pointStart.x + ( TEXTURE_TAB_SIZE_X * ( nSize - 2 ) ), pointStart.y ), CWndBase::m_nAlpha );
				if( m_nCurSelect == nTabSize - 1 )
					m_TextureTabSelect[ 5 ].Render( p2DRender, CPoint( pointStart.x + ( TEXTURE_TAB_SIZE_X * ( nSize - 1 ) ), pointStart.y ), CWndBase::m_nAlpha );
				else
				{
					m_TextureTabSelect[ 3 ].Render( p2DRender, CPoint( pointStart.x + ( TEXTURE_TAB_SIZE_X * ( nSize - 1 ) ), pointStart.y ), CWndBase::m_nAlpha );
					m_TextureTabSelect[ 4 ].Render( p2DRender, CPoint( pointStart.x + ( TEXTURE_TAB_SIZE_X * ( nSize ) ), pointStart.y ), CWndBase::m_nAlpha );
				}
			}

			rectClient.right -= 15;
			y = rectClient.bottom - 17;
		}
		break;
	case CS_VER2:
		{
			if( nTabSize == 0 )
				nTabSize = 1;

			int nTabX = rect.left+1;
			int nTabY = rect.bottom-20;
			int nTabX_Under = rect.left+1;
			int nTabY_Under = rect.bottom-20;

			switch(m_nCurSelect)
			{
			case 0:
				break;
			case 1:
				{
					int nX = nTabX + m_aInventoryTexture[ 1 ].m_size.cx;
					nTabX = nX;
				}
				break;
			case 2:
				{
					int nX = nTabX + m_aInventoryTexture[ 1 ].m_size.cx*2;
					nTabX = nX;
				}
				break;
			case 3:
				{
					int nX = nTabX + m_aInventoryTexture[ 1 ].m_size.cx*3;
					nTabX = nX;
				}
				break;
			}
			m_aInventoryTexture[ 0 ].Render(  p2DRender, CPoint(nTabX_Under,nTabY_Under),CWndBase::m_nAlpha);
			m_aInventoryTexture[ 1 ].Render(  p2DRender, CPoint(nTabX,nTabY),CWndBase::m_nAlpha);

			rectClient.right -= 15;
			y = rectClient.bottom - 17;
		}
		break;
	default:
		FLASSERT(0);
		return;
	}

	if( nTabSize > 0 )
	{
		int nLength = rectClient.Width() / nTabSize;
		for( int i = 0; i < nTabSize; ++i )
		{
			LPWTCITEM pItem = m_aTab[ i ];
			if( pItem )
			{
				switch( m_eTabTitleAlign )
				{
				case ALIGN_LEFT:
					{
						DWORD dwTextColor = ( i != m_nCurSelect ) ? dwDefaultFontColor : dwSelectFontColor;
						p2DRender->TextOut( ( i * nLength ) + 2, y, pItem->pszText, dwTextColor );
						break;
					}
				case ALIGN_RIGHT:
					{
						DWORD dwTextColor = ( i != m_nCurSelect ) ? dwDefaultFontColor : dwSelectFontColor;
						CSize size = p2DRender->m_pFont->GetTextExtent( pItem->pszText );
						if( size.cx < nLength )
						{
							int nRenderingPositionX = nLength - size.cx;
							p2DRender->TextOut( ( i * nLength ) + nRenderingPositionX, y, pItem->pszText, dwTextColor );
						}
						else
							p2DRender->TextOut( ( i * nLength ) + 2, y, pItem->pszText, dwTextColor );
						break;
					}
				case ALIGN_CENTER:
					{
						DWORD dwTextColor = ( i != m_nCurSelect ) ? dwDefaultFontColor : dwSelectFontColor;
						CSize size = p2DRender->m_pFont->GetTextExtent( pItem->pszText );
						if( size.cx < nLength )
						{
							int nRenderingPositionX = ( nLength - size.cx ) / 2;
							p2DRender->TextOut( ( i * nLength ) + nRenderingPositionX, y, pItem->pszText, dwTextColor );
						}
						else
							p2DRender->TextOut( ( i * nLength ) + 2, y, pItem->pszText, dwTextColor );
						break;
					}
				default:
					{
						DWORD dwTextColor = ( i != m_nCurSelect ) ? dwDefaultFontColor : dwSelectFontColor;
						p2DRender->TextOut( ( i * nLength ) + 2, y, pItem->pszText, dwTextColor );
					}
				}
			}
		}
	}

}


//---------------------------------------------------------------------------------------------
// ���� �� �� �׸���
// param	: ������
// return	: BOOL 
//---------------------------------------------------------------------------------------------
void CWndTabCtrl::DrawTabSelect( C2DRender* p2DRender )
{
	CWndBase* pWndBase = this->GetParentWnd();
	FLERROR_ASSERT_LOG_RETURN_VOID( pWndBase,	_T("[������ �� ��Ʈ��] �θ� ������ �����Ͱ� ��ȿ���� ����.") );	

	int		nParent( pWndBase->GetWndId() );
	int		nTabSize( m_aTab.size() );

	if(nParent == APP_INVENTORY)
		return;

	CRect rtTabCtrlRect = GetWndTabCtrlWindowRect();

	CTexture* pTexture = &m_TextureTabSelect[0];
	FLERROR_ASSERT_LOG_RETURN_VOID( pWndBase, _T("[������ �� ��Ʈ��] �� �ؽ��� �����Ͱ� ��ȿ���� ����.") );	

	int nWidth( rtTabCtrlRect.Width() / pTexture->m_size.cx );
	int nHeight( rtTabCtrlRect.Height() / pTexture->m_size.cy );

	int nHeightOffset( rtTabCtrlRect.Height() % pTexture->m_size.cy );

	CRect rtTabRect = rtTabCtrlRect;
	rtTabRect.top += ( (pTexture->m_size.cy * (nHeight - 1)) + nHeightOffset );

	if( m_aTab.empty() )
		return ;
	
	UINT nOneTabBlockCount( nWidth / m_aTab.size() );

	CPoint ptLeftTop( rtTabRect.left, rtTabRect.top );
	for( UINT nCount = 0; nCount < m_aTab.size(); nCount++ )
	{
		if( m_nCurSelect != nCount )
		{
			ptLeftTop.x += ( pTexture->m_size.cx * nOneTabBlockCount );	
			continue;
		}

		for( UINT nBlockCount = 0; nBlockCount < nOneTabBlockCount; nBlockCount++ )
		{
			if( 0 == nBlockCount )								// ���� ��
				m_TextureTabSelect[ 0 ].Render( p2DRender, ptLeftTop, CWndBase::m_nAlpha );
			else if( (nOneTabBlockCount - 1) == nBlockCount )	// ������ ��
				m_TextureTabSelect[ 5 ].Render( p2DRender, ptLeftTop, CWndBase::m_nAlpha );
			else												// ���µ� �ؽ���
				m_TextureTabSelect[ 1 ].Render( p2DRender, ptLeftTop, CWndBase::m_nAlpha );

			ptLeftTop.x += pTexture->m_size.cx ;	
		}
	}


	PFONTCOLOR_WNDTABCTRL pFontColorWndTabCtrl = g_WndFontColorManager->GetWndTabCtrl();

	DWORD dwDefaultFontColor( pFontColorWndTabCtrl->m_stDefault.GetFontColor() );
	DWORD dwSelectFontColor( pFontColorWndTabCtrl->m_stSelectTab.GetFontColor() );

	CRect rectClient = GetWindowRect();
	rectClient.right -= 15;
	int y = rectClient.bottom - 17;

	if( nTabSize > 0 )
	{
		int nLength = rectClient.Width() / nTabSize;
		for( int i = 0; i < nTabSize; ++i )
		{
			LPWTCITEM pItem = m_aTab[ i ];
			if( pItem )
			{
				switch( m_eTabTitleAlign )
				{
				case ALIGN_LEFT:
					{
						DWORD dwTextColor = ( i != m_nCurSelect ) ? dwDefaultFontColor : dwSelectFontColor;
						p2DRender->TextOut( ( i * nLength ) + 2, y, pItem->pszText, dwTextColor );
						break;
					}
				case ALIGN_RIGHT:
					{
						DWORD dwTextColor = ( i != m_nCurSelect ) ? dwDefaultFontColor : dwSelectFontColor;
						CSize size = p2DRender->m_pFont->GetTextExtent( pItem->pszText );
						if( size.cx < nLength )
						{
							int nRenderingPositionX = nLength - size.cx;
							p2DRender->TextOut( ( i * nLength ) + nRenderingPositionX, y, pItem->pszText, dwTextColor );
						}
						else
							p2DRender->TextOut( ( i * nLength ) + 2, y, pItem->pszText, dwTextColor );
						break;
					}
				case ALIGN_CENTER:
					{
						DWORD dwTextColor = ( i != m_nCurSelect ) ? dwDefaultFontColor : dwSelectFontColor;
						CSize size = p2DRender->m_pFont->GetTextExtent( pItem->pszText );
						if( size.cx < nLength )
						{
							int nRenderingPositionX = ( nLength - size.cx ) / 2;
							p2DRender->TextOut( ( i * nLength ) + nRenderingPositionX, y, pItem->pszText, dwTextColor );
						}
						else
							p2DRender->TextOut( ( i * nLength ) + 2, y, pItem->pszText, dwTextColor );
						break;
					}
				default:
					{
						DWORD dwTextColor = ( i != m_nCurSelect ) ? dwDefaultFontColor : dwSelectFontColor;
						p2DRender->TextOut( ( i * nLength ) + 2, y, pItem->pszText, dwTextColor );
					}
				}
			}
		}
	}
}


//---------------------------------------------------------------------------------------------
// �� �׸���
// param	: ������
// return	: BOOL 
//---------------------------------------------------------------------------------------------
void CWndTabCtrl::DrawTab( C2DRender* p2DRender )
{
	CWndBase* pWndBase = this->GetParentWnd();
	FLERROR_ASSERT_LOG_RETURN_VOID( pWndBase,	_T("[������ �� ��Ʈ��] �θ� ������ �����Ͱ� ��ȿ���� ����.") );	

	int		nParent( pWndBase->GetWndId() );
	int		nTabSize( m_aTab.size() );

	if(nParent == APP_INVENTORY)
		return;

	CRect rtTabCtrlRect = GetWndTabCtrlWindowRect();

	CTexture* pTexture = &m_TextureTab[0];
	FLERROR_ASSERT_LOG_RETURN_VOID( pWndBase, _T("[������ �� ��Ʈ��] �� �ؽ��� �����Ͱ� ��ȿ���� ����.") );	

	int nWidth( rtTabCtrlRect.Width() / pTexture->m_size.cx );
	int nHeight( rtTabCtrlRect.Height() / pTexture->m_size.cy );

	int nHeightOffset( rtTabCtrlRect.Height() % pTexture->m_size.cy );

	CRect rtTabRect = rtTabCtrlRect;
	rtTabRect.top += ( (pTexture->m_size.cy * (nHeight - 1)) + nHeightOffset );

	if( m_aTab.empty() )
		return ;
	
	UINT nOneTabBlockCount( nWidth / m_aTab.size() );

	CPoint ptLeftTop( rtTabRect.left, rtTabRect.top );
	for( UINT nCount = 0; nCount < m_aTab.size(); nCount++ )
	{
		for( UINT nBlockCount = 0; nBlockCount < nOneTabBlockCount; nBlockCount++ )
		{
			if( 0 == nBlockCount )								// ���� ��
				m_TextureTab[ 0 ].Render( p2DRender, ptLeftTop, CWndBase::m_nAlpha );
			else if( (nOneTabBlockCount - 1) == nBlockCount )	// ������ ��
				m_TextureTab[ 5 ].Render( p2DRender, ptLeftTop, CWndBase::m_nAlpha );
			else												// ���µ� �ؽ���
				m_TextureTab[ 1 ].Render( p2DRender, ptLeftTop, CWndBase::m_nAlpha );

			ptLeftTop.x += pTexture->m_size.cx; 
		}
	}
}


//---------------------------------------------------------------------------------------------
// �׸���
// param	: ������
// return	: BOOL 
//---------------------------------------------------------------------------------------------
void CWndTabCtrl::OnDraw( C2DRender* p2DRender )
{
	FLERROR_ASSERT_LOG_RETURN_VOID( p2DRender,	_T("[������ �� ��Ʈ��] ������ ������ ��ȿ���� ����.") );	

	DrawTabInventory( p2DRender );
	DrawTab( p2DRender );
	DrawTabSelect( p2DRender );
}


//---------------------------------------------------------------------------------------------
// ��� �̹��� �߰��ϱ�
// param	: ������ �� ���� ...
// return	: void
//---------------------------------------------------------------------------------------------
void CWndTabCtrl::AdditionalSkinTexture_Background( CRect rtRect, LPWORD pDest, CSize sizeSurface, D3DFORMAT d3dFormat )
{
	FLERROR_ASSERT_LOG_RETURN_VOID( pDest,	_T("[������ �� ��Ʈ��] ��� ������ ��ȿ���� ����.") );	

	const UINT			nImageCount = 9;
	LPIMAGE				lpImage[nImageCount] = { NULL, };

	CString				strFileName( _T("") );
	int					nImageWidth( NULL );
	int					nImageHeight( NULL );

	for( UINT nIndex = 0; nIndex < nImageCount; nIndex++ )
	{
		strFileName.Format( "WndEditTile%02d.tga", nIndex );
		if( m_strWndTileMap.Lookup( strFileName, (void*&)lpImage[nIndex] ) == FALSE )
		{
			lpImage[nIndex] = new IMAGE;
			FLERROR_ASSERT_LOG_DO( lpImage[nIndex], continue, _T("[������ �� ��Ʈ��] �̹��� ���� ���� [ %s ]."), strFileName );	
			
			if( LoadImage( MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(),strFileName ), lpImage[nIndex] ) == FALSE )
				FLERROR_ASSERT_LOG_DO( FALSE, continue, _T("[������ �� ��Ʈ��] �̹��� �ҷ����� ���� [ %s ]."), strFileName );	

			m_strWndTileMap.SetAt( strFileName, lpImage[nIndex] );
		}

		if( NULL == nIndex)	// ��� �̹����� ũ�Ⱑ ���ٴ� ����
		{
			nImageWidth		= lpImage[nIndex]->size.cx;
			nImageHeight	= lpImage[nIndex]->size.cy;
		}

		strFileName = _T("");
	}


	int nWidth( rtRect.Width() / nImageWidth );
	int nHeight( rtRect.Height() / nImageHeight );

	CPoint point( 0, 0 );
	for( int i = 0; i < nHeight; i++ )
	{
		for( int j = 0; j < nWidth; j++ )
		{
			point = CPoint( j * 8, i * nImageHeight ) + rtRect.TopLeft();
			if( i == 0 )
			{
				if( j == 0 )
					PaintTexture( pDest, lpImage[ 0 ], point, sizeSurface, d3dFormat );
				else
					if( j == nWidth - 1 )
						PaintTexture( pDest, lpImage[ 2 ], point, sizeSurface, d3dFormat );
					else
						PaintTexture( pDest, lpImage[ 1 ], point, sizeSurface, d3dFormat );
			}
			else
			if( i == nHeight - 1 )
			{
				if( j == 0 )
					PaintTexture( pDest, lpImage[ 6 ], point, sizeSurface, d3dFormat );
				else
					if( j == nWidth - 1 )
						PaintTexture( pDest, lpImage[ 8 ], point, sizeSurface, d3dFormat );
					else
						PaintTexture( pDest, lpImage[ 7 ], point, sizeSurface, d3dFormat );					
			}
			else
			{
				if( j == 0 )
					PaintTexture( pDest, lpImage[ 3 ], point, sizeSurface, d3dFormat );
				else
					if( j == nWidth - 1 )
						PaintTexture( pDest, lpImage[ 5 ], point, sizeSurface, d3dFormat );
					else
						PaintTexture( pDest, lpImage[ 4 ], point, sizeSurface, d3dFormat );				
			}
		}
	}
}


//---------------------------------------------------------------------------------------------
// �� ��ư �̹��� �߰��ϱ�
// param	: ������ �� ���� ...
// return	: void
//---------------------------------------------------------------------------------------------
void CWndTabCtrl::AdditionalSkinTexture_TabImage( CRect rtRect, LPWORD pDest, CSize sizeSurface, D3DFORMAT d3dFormat )
{
	FLERROR_ASSERT_LOG_RETURN_VOID( pDest,	_T("[������ �� ��Ʈ��] ��� ������ ��ȿ���� ����.") );	

	const UINT			nTabButtonImageCount = 6;
	LPIMAGE				lpImage[nTabButtonImageCount] = { NULL, };

	CString				strFileName( _T("") );
	int					nImageWidth( NULL );
	int					nImageHeight( NULL );

	for( UINT nIndex = 0; nIndex < nTabButtonImageCount; nIndex++ )
	{
		strFileName.Format( "WndTabTile%02d.bmp", nIndex );
		if( m_strWndTileMap.Lookup( strFileName, (void*&)lpImage[nIndex] ) == FALSE )
		{
			lpImage[nIndex] = new IMAGE;
			FLERROR_ASSERT_LOG_DO( lpImage[nIndex], continue, _T("[������ �� ��Ʈ��] �� ��ư �̹��� ���� ���� [ %s ]."), strFileName );	
			
			if( LoadImage( MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(),strFileName ), lpImage[nIndex] ) == FALSE )
				FLERROR_ASSERT_LOG_DO( FALSE, continue, _T("[������ �� ��Ʈ��] �� ��ư �̹��� �ҷ����� ���� [ %s ]."), strFileName );	

			m_strWndTileMap.SetAt( strFileName, lpImage[nIndex] );
		}

		if( NULL == nIndex)	
		{
			nImageWidth		= lpImage[nIndex]->size.cx;
			nImageHeight	= lpImage[nIndex]->size.cy;
		}

		strFileName = _T("");
	}

	int nWidth = rtRect.Width() / nImageWidth;
	int nHeight = ( rtRect.Height() / nImageHeight );

	CRect rtTabRect = rtRect;
	rtTabRect.top +=  (nImageHeight * (nHeight - 1));

	if( m_aTab.empty() )
		return ;
	
	UINT nOneTabBlockCount( nWidth / m_aTab.size() );

	CPoint ptLeftTop( rtTabRect.left, rtTabRect.top );
	for( UINT nCount = 0; nCount < m_aTab.size(); nCount++ )
	{
		for( UINT nBlockCount = 0; nBlockCount < nOneTabBlockCount; nBlockCount++ )
		{
			if( 0 == nBlockCount )	// ���� ��
				PaintTexture( pDest, lpImage[ 0 ], ptLeftTop, sizeSurface, d3dFormat );
			else if( (nOneTabBlockCount - 1) == nBlockCount )	// ������ ��
				PaintTexture( pDest, lpImage[ 5 ], ptLeftTop, sizeSurface, d3dFormat );
			else 
				PaintTexture( pDest, lpImage[ 1 ], ptLeftTop, sizeSurface, d3dFormat );

			ptLeftTop.x += nImageWidth;	
		}
	}
}


//---------------------------------------------------------------------------------------------
// ��Ų �ؽ��� �߰�
// param	: ������ �� ���� ...
// return	: void
//---------------------------------------------------------------------------------------------
void CWndTabCtrl::AdditionalSkinTexture( LPWORD pDest, CSize sizeSurface, D3DFORMAT d3dFormat )
{
	CRect rtRect = GetWindowRect( TRUE );
	rtRect += ( m_pParentWnd->GetClientRect( TRUE ).TopLeft() - m_pParentWnd->GetWindowRect( TRUE ).TopLeft() );
	rtRect.right -= 15;	// ��ũ�� �� ������ �������� �ΰ� ���� �Ѥ� �̷� ����...

	AdditionalSkinTexture_Background( rtRect, pDest, sizeSurface, d3dFormat );
}


void CWndTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if( m_aTab.size() == 0 ) return;

	CRect rectClient = GetClientRect();
	CRect rect;
	CRect rect2 = rectClient;
	rect2.bottom -= 18;
	int y = rectClient.bottom - 18;

	LPWTCITEM pItem;
	int nSelect = -1;
	if( m_aTab.size() )
	{
		for( int i = 0; i < (int)( m_aTab.size() ); i++ )
		{
			pItem = m_aTab[ i ];
			if( pItem )
			{
				rect.SetRect( i * m_nTabButtonLength, y, m_nTabButtonLength + i * m_nTabButtonLength, rectClient.bottom );
				if( rect.PtInRect( point ) )
					nSelect = i;
			}
		}
	}
	if( nSelect != -1 )
	{
		SetCurSel( nSelect );
		CWndBase* pParent = (CWndBase*)GetParentWnd();
		pParent->OnChildNotify( WNM_SELCHANGE, GetWndId(), (LRESULT*) this ); 
	}
}


BOOL CWndTabCtrl::OnChildNotify(UINT message,UINT nID,LRESULT* pLResult)
{
	CWndBase* pParent = (CWndBase*)GetParentWnd();
	return pParent->OnChildNotify( message,nID,pLResult); 
}


int CWndTabCtrl::SetCurSel( int nItem )
{
	int nOldSelect = m_nCurSelect;
	LPWTCITEM pItem;
	CRect rect;
	m_nCurSelect = nItem;
	if( m_aTab[ m_nCurSelect ]->pWndBase )
	{
		pItem = m_aTab[ nOldSelect ];
		pItem->pWndBase->SetVisible( FALSE );
		
		if( m_nCurSelect >= (int)( m_aTab.size() ) )
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "nItem�� ������ �Ѿ : nItem(%d) m_aTabSize(%d)" ), m_nCurSelect, m_aTab.size() );
		}

		pItem = m_aTab[ m_nCurSelect ];
		pItem->pWndBase->SetVisible( TRUE );
		rect = GetWindowRect( TRUE );
		if( IsWndStyle( WBS_VSCROLL ) )
			rect.right -= 15;
		rect.bottom -= 18;
		for( int i = 0; i < (int)( m_aTab.size() ); i++ )
		{
			LPWTCITEM pItem = m_aTab[ i ];
			if( pItem && pItem->pWndBase )
			{
				rect.OffsetRect( -rect.TopLeft() );
				pItem->pWndBase->SetWndRect( rect );
			}
		}
		GetFrameWnd()->AdjustWndBase();
		m_aTab[ m_nCurSelect ]->pWndBase->SetFocus();
	}
	else
		m_nCurSelect = nOldSelect;
	return nItem;
}


BOOL CWndTabCtrl::GetItem(int nItem, WTCITEM* pTabCtrlItem) const
{
	if( nItem >= (int)( m_aTab.size() ) )
		return FALSE;
	memcpy( pTabCtrlItem, m_aTab[ nItem ], sizeof( WTCITEM ) );
	return TRUE;
}


void CWndTabCtrl::OnSize(UINT nType, int cx, int cy)
{
	CRect rect = GetClientRect( TRUE );
	rect.bottom -= 18;

	for( int i = 0; i < (int)( m_aTab.size() ); i++ )
	{
		LPWTCITEM pItem = m_aTab[ i ];
		if( pItem && pItem->pWndBase )
		{
			rect.OffsetRect( -rect.TopLeft() );
			pItem->pWndBase->SetWndRect( rect );
		}
	}
	if(  m_aTab.size() )
		m_nTabButtonLength = GetClientRect().Width() / m_aTab.size();
	
	CWndBase::OnSize(nType,cx,cy);
}


LPWTCITEM CWndTabCtrl::GetTabItem( int nItemNumber ) const
{
	if( nItemNumber < 0 || nItemNumber >= (int)( m_aTab.size() ) )
		return NULL;
	return m_aTab[ nItemNumber ];
}


LPWTCITEM CWndTabCtrl::GetSelectedTab( void ) const
{
	return m_aTab[ m_nCurSelect ];
}


void CWndTabCtrl::SetTabTitleAlign( const CWndTabCtrl::TabTitleAlign eTabTitleAlign )
{
	m_eTabTitleAlign = eTabTitleAlign;
}


const CWndTabCtrl::TabTitleAlign CWndTabCtrl::GetTabTitleAlign( void ) const
{
	return m_eTabTitleAlign;
}


int CWndTabCtrl::GetCurSel() const
{
	return m_nCurSelect;
}

void CWndTabCtrl::PaintFrame(C2DRender* p2DRender)
{
	return ;
}