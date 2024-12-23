

#include "StdAfx.h"
#include "WndCommonItemExchange.h"

namespace WndCommonItemExchange
{
	static const BYTE		HEIGHT_OF_ONE_EXCHANGE_ITEM( 36 );

	static const BYTE		EXCHANGE_ITEM_ICON_DRAWOFFSET_X( 8 );
	static const BYTE		DRAWITEMCOUNT_OFFSET_X( 2 ); 

	static const char		EXCHANGE_ITEM_VIEWPORT_OFFSET_TOP( 4 );
	static const char		EXCHANGE_ITEM_VIEWPORT_OFFSET_LEFT( 1 );
	static const char		EXCHANGE_ITEM_VIEWPORT_OFFSET_RIGHT( 6 );
	static const char		EXCHANGE_ITEM_VIEWPORT_OFFSET_BOTTOM( 3 );

	static const BYTE		EXCHANGE_ARROW_COUNT( 1 );
};


//---------------------------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------------------------
CWndCommonItemExchange::CWndCommonItemExchange()
:	m_pWndDescription( NULL )
,	m_pWndExchangeList( NULL )
,	m_pTexExchangeArrow( NULL )
,	m_dwMMIDefineIndex( -1 )
,	m_dwCurrentDescPageIndex( NULL )
,	m_nTotalDescPageCount( NULL )
,	m_byConditionItemMaxCount( NULL )
,	m_byPaymentItemMaxCount( NULL )
,	m_bSendPacket( false )
,	m_byPaymentItemColorChangeCount( NULL )
,	m_pWndCommonItemExchangeConfirm( NULL )
,	m_dwPossibleMaxExchangeCount( NULL )
{
}


//---------------------------------------------------------------------------------------------
// �ı���
//---------------------------------------------------------------------------------------------
CWndCommonItemExchange::~CWndCommonItemExchange()
{
	m_mapExchangeItem.clear();
	m_vecIconDrawRect.clear();
}


//---------------------------------------------------------------------------------------------
// �ʱ�ȭ
// param	: �θ� ������, ���� ������ ���̵�
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndCommonItemExchange::Initialize( CWndBase* pWndParent, DWORD dwWndId )
{
	FLERROR_ASSERT_LOG_RETURN( dwWndId,	FALSE, _T("[���� ������ ��ȯ â] ������ ���̵� ��ȿ���� �ʽ��ϴ�.") );
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), dwWndId, 0, CPoint( 0, 0 ), pWndParent );
}


//---------------------------------------------------------------------------------------------
// ������Ʈ �ʱ�ȭ
// param	: ����
// return	: void
//---------------------------------------------------------------------------------------------
void CWndCommonItemExchange::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 

	m_pWndDescription = ( static_cast<CWndText*>( GetDlgItem( WIDC_TEXT1 ) ) );
	FLERROR_ASSERT_LOG_RETURN_VOID( m_pWndDescription, _T("[���� ������ ��ȯ â] �ؽ�Ʈ ��Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

	m_pWndExchangeList = ( static_cast<CWndListBox*>( GetDlgItem( WIDC_LISTBOX1 ) ) );
	FLERROR_ASSERT_LOG_RETURN_VOID( m_pWndExchangeList, _T("[���� ������ ��ȯ â] ����Ʈ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
	
	if( m_pWndExchangeList )
	{
		m_pWndExchangeList->m_bOnItem = TRUE;
		m_pWndExchangeList->m_nLineSpace = (int)( ( HEIGHT_OF_ONE_EXCHANGE_ITEM - m_pWndExchangeList->GetFontHeight( ) ) * 0.5f );
	}

	CString strFullPath( _T("") );

	strFullPath += DIR_THEME;
	strFullPath += DIR_DEFAULT;

	m_pTexExchangeArrow = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), _T("ExchangeArrow.tga") ), 0, TRUE );

	m_PaymentItemColorChangeTimer.Set( SEC( 2 ) );

	MoveParentCenter();
}  


//---------------------------------------------------------------------------------------------
// ������ ��ȯ ���� ����
// param	: MMI ���� �ε���
// return	: �� / �ƴϿ�
//---------------------------------------------------------------------------------------------
bool CWndCommonItemExchange::SetItemExchangeInfo(const DWORD dwMMIDefineIndex)
{
	if( -1 == dwMMIDefineIndex )
		FLERROR_ASSERT_LOG_RETURN( NULL, false, _T("[���� ������ ��ȯ â] MMI �ε��� ������ �ùٸ��� �ʽ��ϴ�.") );
	
	m_dwMMIDefineIndex = dwMMIDefineIndex;

	if( false == SetDescriptionString() )
		return false;
	
	if( false == CreateExchangeItemList() )
		return false;

	return true;
}


//---------------------------------------------------------------------------------------------
// ���� ���ڿ����� �����Ѵ�.
// param	: void 
// return	: �� / �ƴϿ�
//---------------------------------------------------------------------------------------------
bool CWndCommonItemExchange::SetDescriptionString()
{
	FLERROR_ASSERT_LOG_RETURN( g_pExchangeEvent, false, _T("[���� ������ ��ȯ â] ��ȯ ��ũ��Ʈ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

	TextIDVec vecDescTextID;

	if( false == g_pExchangeEvent->GetDescriptionTextID( m_dwMMIDefineIndex, &vecDescTextID ) )
		FLERROR_ASSERT_LOG_RETURN( NULL, false, _T("[���� ������ ��ȯ â] ��ȯ ���� ����Ʈ ��⸦ �����Ͽ����ϴ�.  MMI ID - [ %u ]"), m_dwMMIDefineIndex );

	m_nTotalDescPageCount = vecDescTextID.size();
	
	// ����Ʈ�� ������ 1����� ���� ������ ��ư�� �� Ȱ��ȭ �Ѵ�.
	if( 1 == m_nTotalDescPageCount )
	{
		CWndButton* pNextPage( static_cast<CWndButton*>( GetDlgItem( WIDC_BUTTON1 ) ) );

		if( pNextPage )
		{
			pNextPage->SetVisible( FALSE );
			pNextPage->EnableWindow( FALSE );
		}
		else
		{
			FLERROR_LOG( PROGRAM_NAME, _T("[���� ������ ��ȯ â] ���� ���� ������ ��ư ��Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );		
			FLASSERT( NULL );
		}
	}

	m_pWndDescription->m_string.AddParsingString( prj.GetText( vecDescTextID[m_dwCurrentDescPageIndex] ) );
	m_pWndDescription->ResetString();	

	return true;
}


//---------------------------------------------------------------------------------------------
// ������ ����Ʈ �����ϱ�
// param	: ������ ����Ʈ
// return	: void
// ����		: ��ȯ �������� ������ OnDraw���� �׸����̰� �� Ŭ������ ��� �������� ��ũ�� �� ������ ListBox�� 
//			: ���� �����͸� �����Ѵ�.
//---------------------------------------------------------------------------------------------
bool CWndCommonItemExchange::CreateExchangeItemList()
{
	FLERROR_ASSERT_LOG_RETURN( m_pWndExchangeList, false, _T("[���� ������ ��ȯ â] ��ȯ ������ ����Ʈ ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

	m_mapExchangeItem.clear();

	if( false == g_pExchangeEvent->GetExchangeItemList( m_dwMMIDefineIndex, m_mapExchangeItem ) )
		FLERROR_ASSERT_LOG_RETURN( NULL, false, _T("[���� ������ ��ȯ â] ��ȯ ������ ����Ʈ ������ ��� �� �� �����ϴ�.") );

	m_pWndExchangeList->ResetContent();
	
	int nCount( NULL );
	for( ExchangeItemListItr Iter = m_mapExchangeItem.begin(); Iter != m_mapExchangeItem.end(); ++Iter )
	{
		T_EXCHANGE_ITEM_LIST stData( Iter->second );

		if( stData.mapConditionItem.empty() )
			FLERROR_ASSERT_LOG_DO( NULL, continue, _T("[���� ������ ��ȯ â] �ʿ� ������ ��� ��� �ֽ��ϴ�!! ID - [%u]."), Iter->first );
			
		if( stData.mapPaymentItem.empty() )
			FLERROR_ASSERT_LOG_DO( NULL, continue, _T("[���� ������ ��ȯ â] ���� ������ ��� ��� �ֽ��ϴ�!! ID - [%u]."), Iter->first );

		if( stData.mapConditionItem.size() > m_byConditionItemMaxCount )
			m_byConditionItemMaxCount = stData.mapConditionItem.size();

		if( stData.mapPaymentItem.size() > m_byPaymentItemMaxCount )
			m_byPaymentItemMaxCount = stData.mapPaymentItem.size();
		
		m_pWndExchangeList->AddString( _T("") );
		m_pWndExchangeList->SetItemData( nCount, NULL );

		nCount++;
	}

	return true;
}


//---------------------------------------------------------------------------------------------
// �� ��Ʈ ����
// param	: ������
// return	: void
// �߰�����	: ����Ʈ �ڽ� �ȿ��� �׸��� �������� �׸��� ���ؼ� �� ��Ʈ�� Ŭ���� ������ ���� �����Ѵ�.
//---------------------------------------------------------------------------------------------
void CWndCommonItemExchange::SetViewPort( C2DRender* const p2DRender )
{
	CRect	rtViewport( m_pWndExchangeList->m_rectWindow );

	rtViewport.left		+= EXCHANGE_ITEM_VIEWPORT_OFFSET_LEFT;
	rtViewport.right	-= m_pWndExchangeList->GetScrollWidth() + EXCHANGE_ITEM_VIEWPORT_OFFSET_RIGHT;
	rtViewport.top		+= EXCHANGE_ITEM_VIEWPORT_OFFSET_TOP;
	rtViewport.bottom	-= EXCHANGE_ITEM_VIEWPORT_OFFSET_BOTTOM;

	ClientToScreen( &rtViewport );

	p2DRender->SetViewport( rtViewport );
}


//---------------------------------------------------------------------------------------------
// ���� ������ ���� �ٲٱ� ����
// param	: void
// return	: void
//---------------------------------------------------------------------------------------------
void CWndCommonItemExchange::UpdatePaymentItemColorChange()
{
	if( m_PaymentItemColorChangeTimer.TimeOut() )
	{
		++m_byPaymentItemColorChangeCount;
		m_PaymentItemColorChangeTimer.Reset();
		m_PaymentItemColorChangeTimer.Set( SEC(2) );

		if( m_byPaymentItemColorChangeCount >= m_byPaymentItemMaxCount )
			m_byPaymentItemColorChangeCount = NULL;
	}
}


//---------------------------------------------------------------------------------------------
// �׸���
// param	: ������
// return	: void
//---------------------------------------------------------------------------------------------
void CWndCommonItemExchange::OnDraw( C2DRender* p2DRender )
{
	if( NULL == p2DRender )
		return ;

	UpdatePaymentItemColorChange();
	DrawExchangeItemList( p2DRender );
}


//---------------------------------------------------------------------------------------------
// ��ȯ ������ ��� �׸���
// param	: ������
// return	: void
//---------------------------------------------------------------------------------------------
bool CWndCommonItemExchange::DrawExchangeItemList( C2DRender* const p2DRender )
{
	if( m_mapExchangeItem.empty() )
		return false;

	SetViewPort( p2DRender );

	m_vecIconDrawRect.clear();

	CRect	rtRect( m_pWndExchangeList->m_rectWindow );
	int		nSelected( m_pWndExchangeList->GetCurSel() );

	int nCount( NULL );
	for( ExchangeItemListItr Iter = m_mapExchangeItem.begin(); Iter != m_mapExchangeItem.end(); ++Iter )
	{
		T_EXCHANGE_ITEM_LIST stData( Iter->second );

		CRect	rtItem( m_pWndExchangeList->GetItemRect( nCount ) );
		CPoint	ptItem( rtItem.TopLeft() + m_pWndExchangeList->m_rectWindow.TopLeft() );

		DrawExchangeItem_ConditionItemIcon( p2DRender, stData.mapConditionItem, ptItem, rtItem );
		DrawExchangeItem_ExchangeArrow( p2DRender, ptItem, rtItem );
		DrawExchangeItem_PaymentItemIcon( p2DRender, stData.mapPaymentItem, ptItem, rtItem );
		DrawExchangeItem_WidthLine( p2DRender, nCount, rtItem );
		DrawExchangeItem_SelectBox( p2DRender, nCount, ptItem, rtItem );

		nCount++;
	}

	DrawToolTip();

	return true;
}


//---------------------------------------------------------------------------------------------
// ��ȯ�� ���� �ʿ��� �������� ��� ������ �ִ��� üũ�Ѵ�
// param	: ��ȯ ������ ����
// return	: �������� �����մϴ� ( true ) / �������� �������� �ʽ��ϴ� ( false )
//---------------------------------------------------------------------------------------------
bool CWndCommonItemExchange::CheckExchangeConditionItemHaveCount( const T_EXCHANGE_ITEM& stData )
{
	bool bNotEnoughItemCount( false );

	if( NULL != stData.dwMinGeneralEnchant &&
		NULL != stData.dwMaxGeneralEnchant )
	{
		int		nItemCount( NULL );
		WORD	wStartCount( MAX_HUMAN_PARTS );
		WORD	wEndCount( MAX_INVENINDEX_QUEST );

		for( WORD wCount = wStartCount; wCount < wEndCount; wCount++ )
		{
			FLItemElem* pItemElem( g_pPlayer->m_Inventory.GetAt( wCount ) );
			if( NULL == pItemElem )
				continue;

			if( stData.dwItemID == pItemElem->m_dwItemId )	
			{
				DWORD dwGeneralEnchantLevel( pItemElem->GetAbilityOption() );
			
				if( dwGeneralEnchantLevel >= stData.dwMinGeneralEnchant &&		// �Ϲ� ���� ��ġ�� ��ũ��Ʈ ��ġ �ȿ� �����Ѵٸ�
					dwGeneralEnchantLevel <= stData.dwMaxGeneralEnchant )
				{
					nItemCount += pItemElem->m_nItemNum;
				}
			}
		}

		if( stData.nItemQuantity > nItemCount )
			bNotEnoughItemCount = true;
	}
	else
	{
		if( stData.nItemQuantity > g_pPlayer->m_Inventory.GetItemNumByItemId( stData.dwItemID ) )
			bNotEnoughItemCount = true;
	}

	return bNotEnoughItemCount;
}


//---------------------------------------------------------------------------------------------
// ������ �׸��� (�ʿ� ������)
// param	: ������, ������ ����Ʈ, ��ġ, ����
// return	: void
//---------------------------------------------------------------------------------------------
bool CWndCommonItemExchange::DrawExchangeItem_ConditionItemIcon( C2DRender* const p2DRender, const ExchangeItemMap& mapConditionItem, CPoint ptItem, const CRect rtItem )
{
	if( NULL == g_pPlayer )
		return false;

	if( mapConditionItem.empty() )
		return false;

	UINT nCount( NULL );

	for( ExchangeItemCItr pos = mapConditionItem.begin(); pos != mapConditionItem.end(); ++pos )
	{
		bool	bNotEnoughItemCount( false );
		const	T_EXCHANGE_ITEM stData( pos->second );

		const PT_ITEM_SPEC pCondItemProp = g_xSpecManager->GetSpecItem( stData.dwItemID );
		FLERROR_ASSERT_LOG_DO( pCondItemProp, continue, _T("[���� ������ ��ȯ â] ��ȯ �ʿ� ������ �Ӽ��� ��� �� �� �����ϴ�. COUNT - [%u], INDEX - [%u]"), nCount, stData.dwItemID );

		CTexture* pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, pCondItemProp->szIcon ), WNDCOLOR_DEFAULT_KEY );
		if( pTexture )
		{
			CPoint ptIcon( ptItem );
			ptIcon.x += EXCHANGE_ITEM_ICON_DRAWOFFSET_X;	// ó���� OFFSET ��ŭ ��� �׸���

			ptIcon.x += (nCount * EXCHANGE_ITEM_ICON_DRAWOFFSET_X) + (nCount * pTexture->m_size.cx);
			ptIcon.y += static_cast<long>( ( rtItem.Height() - pTexture->m_size.cy) * 0.5f );

			bNotEnoughItemCount = CheckExchangeConditionItemHaveCount( stData );

			if( bNotEnoughItemCount )
				p2DRender->RenderTexture( ptIcon, pTexture, 64 );
			else
				p2DRender->RenderTexture( ptIcon, pTexture );

			// �׸��� ���� ���� ����
			CRect rtIcon( ptIcon.x, ptIcon.y, ptIcon.x + pTexture->m_size.cx, ptIcon.y + pTexture->m_size.cy ); 
			
			ICONDRAWINFO stIconDrawRect;
			stIconDrawRect.m_dwItemID				= stData.dwItemID;
			stIconDrawRect.m_rtItemIconDraw			= rtIcon; 
			stIconDrawRect.m_dwMinGeneralEnchant	= stData.dwMinGeneralEnchant;
			stIconDrawRect.m_dwMaxGeneralEnchant	= stData.dwMaxGeneralEnchant;

			m_vecIconDrawRect.push_back( stIconDrawRect );
			++nCount;

			if( NULL == stData.nItemQuantity )
				continue;

			// ������ ���� �׸���
			PFONTCOLOR_WNDCOMMONITEMEXCHANGE pFontColorWndItemExchange = g_WndFontColorManager->GetWndCommonItemExchange();

			TCHAR szCount[ 32 ] = { NULL, };
			FLSPrintf( szCount, _countof( szCount ), _T("%d"), stData.nItemQuantity );

			CPoint ptItemCount( rtIcon.BottomRight() );

			CSize StringSize( m_p2DRender->m_pFont->GetTextExtent( szCount ) );

			m_p2DRender->TextOut(	ptItemCount.x - StringSize.cx - DRAWITEMCOUNT_OFFSET_X,
									ptItemCount.y - StringSize.cy , 
									szCount, 
									pFontColorWndItemExchange->m_stItemCount.GetFontColor(),
									pFontColorWndItemExchange->m_stItemCountShadow.GetFontColor() );
		}
	}
	
	return true;
}


//---------------------------------------------------------------------------------------------
// ������ �׸��� (ȭ��ǥ)
// param	: ������, ��ġ ����
// return	: void
//---------------------------------------------------------------------------------------------
bool CWndCommonItemExchange::DrawExchangeItem_ExchangeArrow( C2DRender* const p2DRender, CPoint ptItem, const CRect rtItem )
{
	if( NULL == m_pTexExchangeArrow )
		return false;

	CPoint ptIcon( ptItem );
	ptIcon.x += EXCHANGE_ITEM_ICON_DRAWOFFSET_X;	// ó���� OFFSET ��ŭ ��� �׸���

	ptIcon.x += ( m_byConditionItemMaxCount * EXCHANGE_ITEM_ICON_DRAWOFFSET_X ) +
				( m_byConditionItemMaxCount * m_pTexExchangeArrow->m_size.cx );

	ptIcon.y += static_cast<long>( ( rtItem.Height() - m_pTexExchangeArrow->m_size.cy) * 0.5f );

	p2DRender->RenderTexture( ptIcon, m_pTexExchangeArrow );

	return true;
}


//---------------------------------------------------------------------------------------------
// ������ �׸��� (���� ������)
// param	: ������, ������ ����Ʈ, ��ġ, ����
// return	: void
//---------------------------------------------------------------------------------------------
bool CWndCommonItemExchange::DrawExchangeItem_PaymentItemIcon( C2DRender* const p2DRender, const ExchangeItemMap& mapPaymentItem, CPoint ptItem, const CRect rtItem )
{
	if( mapPaymentItem.empty() )
		return false;

	UINT nDrawItemCount( mapPaymentItem.size() );
	UINT nCount( NULL );

	for( ExchangeItemCItr pos = mapPaymentItem.begin(); pos != mapPaymentItem.end(); ++pos )
	{
		const T_EXCHANGE_ITEM	stData( pos->second );
		const PT_ITEM_SPEC		pCondItemProp( g_xSpecManager->GetSpecItem( stData.dwItemID ) );

		FLERROR_ASSERT_LOG_DO( pCondItemProp, continue, _T("[���� ������ ��ȯ â] ��ȯ �ʿ� ������ �Ӽ��� ��� �� �� �����ϴ�. COUNT - [%u], INDEX - [%u]"), nCount, stData.dwItemID );

		CTexture* pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, pCondItemProp->szIcon ), WNDCOLOR_DEFAULT_KEY );
		if( pTexture )
		{
			CPoint ptIcon( ptItem );
			ptIcon.x += EXCHANGE_ITEM_ICON_DRAWOFFSET_X;	// ó���� OFFSET ��ŭ ��� �׸���

			ptIcon.x += ( ( nCount + m_byConditionItemMaxCount + EXCHANGE_ARROW_COUNT ) * EXCHANGE_ITEM_ICON_DRAWOFFSET_X ) +
						( ( nCount + m_byConditionItemMaxCount + EXCHANGE_ARROW_COUNT ) * pTexture->m_size.cx );
			
			ptIcon.y += static_cast<long>( ( rtItem.Height() - pTexture->m_size.cy) * 0.5f );

			// ���� �������� �������� ��� �� �� �ϳ��� ��ȯ�� �� ������ ǥ�����ֱ�
			
			if( nDrawItemCount > 1 )
			{
				BYTE byAlphaDrawIndex( NULL );

				if( m_byPaymentItemMaxCount > nDrawItemCount )
				{
					if( m_byPaymentItemColorChangeCount > nDrawItemCount )
						byAlphaDrawIndex = m_byPaymentItemColorChangeCount - nDrawItemCount;
					else
						byAlphaDrawIndex = m_byPaymentItemColorChangeCount;
				}
				else
					byAlphaDrawIndex = m_byPaymentItemColorChangeCount;
				
				if( byAlphaDrawIndex == nCount )
					p2DRender->RenderTexture( ptIcon, pTexture, 255 );
				else
					p2DRender->RenderTexture( ptIcon, pTexture, 64 );
			}
			else
				p2DRender->RenderTexture( ptIcon, pTexture );

			// ������ �׸��� ���� ���� ����
			CRect rtIcon( ptIcon.x, ptIcon.y, ptIcon.x + pTexture->m_size.cx, ptIcon.y + pTexture->m_size.cy ); 
			
			ICONDRAWINFO stIconDrawRect;
			stIconDrawRect.m_dwItemID		= stData.dwItemID;
			stIconDrawRect.m_rtItemIconDraw	= rtIcon; 
			stIconDrawRect.m_bind_flag		= stData.byItemFlag;

			m_vecIconDrawRect.push_back( stIconDrawRect );
			++nCount;

			if( NULL == stData.nItemQuantity )
				continue;

			// ������ ���� �׸���
			PFONTCOLOR_WNDCOMMONITEMEXCHANGE pFontColorWndItemExchange = g_WndFontColorManager->GetWndCommonItemExchange();

			TCHAR szCount[ 32 ] = { NULL, };
			FLSPrintf( szCount, _countof( szCount ), _T("%d"), stData.nItemQuantity );

			CPoint ptItemCount( rtIcon.BottomRight() );

			CSize StringSize( m_p2DRender->m_pFont->GetTextExtent( szCount ) );
			m_p2DRender->TextOut(	ptItemCount.x - StringSize.cx - DRAWITEMCOUNT_OFFSET_X,
									ptItemCount.y - StringSize.cy , 
									szCount, 
									pFontColorWndItemExchange->m_stItemCount.GetFontColor(),
									pFontColorWndItemExchange->m_stItemCountShadow.GetFontColor() );
		}
	}
	
	return true;
}



//---------------------------------------------------------------------------------------------
// ������ �׸��� (���� ��)
// param	: ������
// return	: ���� / ����
//---------------------------------------------------------------------------------------------
bool CWndCommonItemExchange::DrawExchangeItem_WidthLine( C2DRender* p2DRender, const UINT& nCount, const CRect& rtItem )
{
	PFLCOLOR_WNDCOMMONITEMEXCHANGE pFLColorWndCommonItemExchange = g_FLColorManager->GetWndCommonItemExchange();
	DWORD dwLineColor = pFLColorWndCommonItemExchange->m_stListBoxLineColor.GetColor();

	CPoint ptItemLeftTop = m_pWndExchangeList->m_rectWindow.TopLeft(); 
	ptItemLeftTop.y += ( nCount * rtItem.Height() ) + WndListBox::TEXTOUT_OFFSET;

	p2DRender->RenderLine(	CPoint( ptItemLeftTop.x, ptItemLeftTop.y + rtItem.Height() ), 
							CPoint( ptItemLeftTop.x + rtItem.Width(), ptItemLeftTop.y + rtItem.Height() ),
							dwLineColor, dwLineColor );

	return true;
}


//---------------------------------------------------------------------------------------------
// ������ �׸��� (���ùڽ�)
// param	: ������
// return	: ���� / ����
//---------------------------------------------------------------------------------------------
bool CWndCommonItemExchange::DrawExchangeItem_SelectBox(C2DRender *p2DRender, const UINT &nCount, CPoint ptItem, const CRect &rtItem)
{
	if( m_mapExchangeItem.empty() )
		return false;

	int	nSelected( m_pWndExchangeList->GetCurSel() );

	if( nCount != nSelected )
		return false;

	PFLCOLOR_WNDCOMMONITEMEXCHANGE pFLColorWndCommonItemExchange = g_FLColorManager->GetWndCommonItemExchange();

	DWORD dwRectColor( pFLColorWndCommonItemExchange->m_stListBoxSelectRect.GetColor() );
	DWORD dwFillRectColor( pFLColorWndCommonItemExchange->m_stListBoxSelectFillRect.GetColor() );

	CRect rtRound( ptItem.x - WndListBox::TEXTOUT_OFFSET, ptItem.y, ptItem.x - WndListBox::TEXTOUT_OFFSET + rtItem.Width() - EXCHANGE_ITEM_VIEWPORT_OFFSET_RIGHT + 2, ptItem.y + rtItem.Height()+ 1 );	// �� 1�ȼ�, 2�ȼ��� �������� �Ѥ�;;
	rtRound.DeflateRect( 2, 2 );

	p2DRender->RenderRect( rtRound, dwRectColor );

	rtRound.DeflateRect( 1, 1 );
	p2DRender->RenderFillRect( rtRound, dwFillRectColor );

	return true;
}


//---------------------------------------------------------------------------------------------
// ���� ���콺�� �÷��� �ִ� �������� �׸��� ������ ���´�.
// param	: �޽���, ���̵�, ���
// return	: BOOL
//---------------------------------------------------------------------------------------------
ICONDRAWINFO CWndCommonItemExchange::GetCurrentOnMouseItemDrawInfo()
{
	return m_stCurrentOnMouseItemDrawInfo;
}


//---------------------------------------------------------------------------------------------
// ���� �׸���
// param	: �޽���, ���̵�, ���
// return	: BOOL
//---------------------------------------------------------------------------------------------
bool CWndCommonItemExchange::DrawToolTip()
{
	if( m_vecIconDrawRect.empty() )
		return false;

	if( m_pWndOnMouseMove != m_pWndExchangeList )
		return false;

	CPoint	ptMouse( GetMousePoint() );
	CRect	rtListBox( m_pWndExchangeList->GetClientRect( TRUE ) );

	if( FALSE == rtListBox.PtInRect( ptMouse ) )
		return false;

	ZeroMemory( &m_stCurrentOnMouseItemDrawInfo, sizeof( ICONDRAWINFO ) );

	for( UINT nCount = 0; nCount < m_vecIconDrawRect.size(); nCount++ )
	{
		if( m_vecIconDrawRect[nCount].m_rtItemIconDraw.PtInRect( ptMouse ) ) 
		{
			m_stCurrentOnMouseItemDrawInfo = m_vecIconDrawRect[nCount];
			break;
		}
	}

	if( NULL == m_stCurrentOnMouseItemDrawInfo.m_dwItemID )
		return false;

	FLItemElem ItemElem;	
	ItemElem.m_dwItemId = m_stCurrentOnMouseItemDrawInfo.m_dwItemID;
	ItemElem.m_byFlag	= m_stCurrentOnMouseItemDrawInfo.m_bind_flag;

	CPoint	ptTemp = ptMouse;
	CRect	rtTemp = m_stCurrentOnMouseItemDrawInfo.m_rtItemIconDraw;

	ClientToScreen( &ptTemp );
	ClientToScreen( &rtTemp );

	g_WndMng.PutToolTip_Item( &ItemElem, ptTemp, &rtTemp, APP_DIALOG_EVENT );

	return true;
}


//---------------------------------------------------------------------------------------------
// �������� ��Ŷ ����
// param	: �޽���, ���̵�, ���
// return	: BOOL
//---------------------------------------------------------------------------------------------
void CWndCommonItemExchange::SetSendPacket()
{
	m_bSendPacket = true;
}


//---------------------------------------------------------------------------------------------
// ������ ���� ��Ŷ ���� ��� �ޱ�
// param	: �޽���, ���̵�, ���
// return	: BOOL
// ����		: �������� ������� �޾Ƽ� Ŭ���̾�Ʈ���� ��� �޽����� ����Ͽ����� ������ 
//			  �������� ��� �޽����� ����ϰ� ����.
//---------------------------------------------------------------------------------------------
void CWndCommonItemExchange::SetReceivedPacket()
{
	m_bSendPacket = false;
}


//---------------------------------------------------------------------------------------------
// �ڽ����� ���� �޽��� ����
// param	: �޽���, ���̵�, ���
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndCommonItemExchange::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( WIDC_OK == nID )		// Ȯ��
	{
		if( m_bSendPacket )
			return FALSE;

		if( false == CheckPossibleMaxExchangeCount() )
		{
			SAFE_DELETE( m_pWndCommonItemExchangeConfirm );
			m_dwPossibleMaxExchangeCount = NULL;

			g_WndMng.OpenMessageBox( prj.GetText( TID_MMI_EXCHANGEAMOUNT_MESSAGE ) );
		}
	}

	if( WIDC_BUTTON1 == nID )	// ���� ���� ������ ��ư
	{
		if( m_dwCurrentDescPageIndex < m_nTotalDescPageCount - 1 )
		{
			m_dwCurrentDescPageIndex += 1;
			
			SetDescriptionString();

			if( m_dwCurrentDescPageIndex == m_nTotalDescPageCount - 1 )
			{
				CWndButton* pNextPage( static_cast<CWndButton*>( GetDlgItem( WIDC_BUTTON1 ) ) );

				if( pNextPage )
				{
					pNextPage->SetVisible( FALSE );
					pNextPage->EnableWindow( FALSE );
				}
				else
				{
					FLERROR_LOG( PROGRAM_NAME, _T("[���� ������ ��ȯ â] ���� ���� ������ ��ư ��Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );		
					FLASSERT( NULL );
				}
			}

		}
	}

	if( WIDC_CANCEL == nID )	// ���
		Destroy();

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
}


//---------------------------------------------------------------------------------------------
// ��ȯ ���� �� �ִ� ������ ������ �� �� ���� üũ �� ����
// param	: void
// return	: ���� / ����
// ����		: �Լ�ȭ ���Ѿ� ������.. ���� �� ���� �»��̶�.. ���߿� �ٲ���
//---------------------------------------------------------------------------------------------
bool CWndCommonItemExchange::CheckPossibleMaxExchangeCount()
{
	m_dwPossibleMaxExchangeCount = NULL;

	if( m_mapExchangeItem.empty() )
		return false;

	if( NULL == m_pWndExchangeList )
		return false;

	int	nSelected( m_pWndExchangeList->GetCurSel() );
	if( -1 == nSelected )
		return false;

	if( (int)m_mapExchangeItem.size() <= nSelected )
		return false;

	const T_EXCHANGE_ITEM_LIST stDataList( m_mapExchangeItem[nSelected] );

	if( stDataList.mapConditionItem.empty() )
		return false;
	
	for( ExchangeItemCItr pos = stDataList.mapConditionItem.begin(); pos != stDataList.mapConditionItem.end(); ++pos )
	{
		const T_EXCHANGE_ITEM stData( pos->second );

		// �Ϲ� ���� ��ġ ��ȯ ��ũ��Ʈ �� ��
		if( NULL != stData.dwMinGeneralEnchant &&
			NULL != stData.dwMaxGeneralEnchant )
		{
			int		nItemCount( NULL );
			WORD	wStartCount( MAX_HUMAN_PARTS );
			WORD	wEndCount( MAX_INVENINDEX_QUEST );

			for( WORD wCount = wStartCount; wCount < wEndCount; wCount++ )
			{
				FLItemElem* pItemElem( g_pPlayer->m_Inventory.GetAt( wCount ) );
				if( NULL == pItemElem )
					continue;

				if( stData.dwItemID == pItemElem->m_dwItemId )	
				{
					DWORD dwGeneralEnchantLevel( pItemElem->GetAbilityOption() );
				
					// �Ϲ� ���� ��ġ�� ��ũ��Ʈ ��ġ �ȿ� �����Ѵٸ�
					if( dwGeneralEnchantLevel >= stData.dwMinGeneralEnchant &&		
						dwGeneralEnchantLevel <= stData.dwMaxGeneralEnchant )
					{
						nItemCount += pItemElem->m_nItemNum;
					}
				}
			}

			if( stData.nItemQuantity > nItemCount )
				return false;
			else
			{
				// �Ҽ��� ������ ���ر� ���ؼ� ó������ �� ������ ���� ����Ѵ�.
				int nRemainCount( nItemCount % stData.nItemQuantity );
				int nPossibleExchangeCount( ( nItemCount - nRemainCount ) / stData.nItemQuantity );	
		
				if( NULL == m_dwPossibleMaxExchangeCount )
					m_dwPossibleMaxExchangeCount = nPossibleExchangeCount;
				else
				{
					if( nPossibleExchangeCount < (int)m_dwPossibleMaxExchangeCount )
						m_dwPossibleMaxExchangeCount = nPossibleExchangeCount;
				}
			}
		}
		else	// �Ϲ� ��ȯ ��ũ��Ʈ �� ��
		{
			int nItemCountFromInventory( g_pPlayer->m_Inventory.GetItemNumByItemId( stData.dwItemID ) );
			
			if( stData.nItemQuantity > nItemCountFromInventory )
				return false;
			else
			{
				// �Ҽ��� ������ ���ر� ���ؼ� ó������ �� ������ ���� ����Ѵ�.
				int nRemainCount( nItemCountFromInventory % stData.nItemQuantity );
				int nPossibleExchangeCount( ( nItemCountFromInventory - nRemainCount ) / stData.nItemQuantity );	
		
				if( NULL == m_dwPossibleMaxExchangeCount )
					m_dwPossibleMaxExchangeCount = nPossibleExchangeCount;
				else
				{
					if( nPossibleExchangeCount < (int)m_dwPossibleMaxExchangeCount )
						m_dwPossibleMaxExchangeCount = nPossibleExchangeCount;
				}
			}
		}
	}

	if( 1 == m_dwPossibleMaxExchangeCount )
	{
		FLPacketItemExchangeReq	kPacket;

		kPacket.dwMenuID		= m_dwMMIDefineIndex;
		kPacket.dwListNth		= nSelected;
		kPacket.nExchangeCount	= m_dwPossibleMaxExchangeCount;

		g_DPlay.SendPacket( &kPacket );

		SetSendPacket();
	}
	else
	{
		SAFE_DELETE( m_pWndCommonItemExchangeConfirm );
		
		m_pWndCommonItemExchangeConfirm = new CWndCommonItemExchangeConfirm();
		if( m_pWndCommonItemExchangeConfirm )
		{
			m_pWndCommonItemExchangeConfirm->Initialize( this );
			m_pWndCommonItemExchangeConfirm->SetItemExchangeInfo( m_dwMMIDefineIndex, nSelected, m_dwPossibleMaxExchangeCount );
		}
	}
	
	return true;
}