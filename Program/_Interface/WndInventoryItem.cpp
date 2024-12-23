

#include "StdAfx.h"
#include "WndInventoryItem.h"
#include "WndVendor.h"

/// ���̵����� �����ִ� �κ��丮 ������ �� �ݰ� �������� �� �˰���? �Ƹ� �ڵ��Ǿ� ������ ���� �ϴٸ�...

namespace WndInventoryItem
{
	static const BYTE	MAXITEMCOUNT_OF_ONE_PAGE( 42 );
	static const BYTE	HORIZON_ITEMCOUNT( 6 );
	static const BYTE	VERTICAL_ITEMCOUNT( MAXITEMCOUNT_OF_ONE_PAGE / HORIZON_ITEMCOUNT );	
};

using namespace WndInventoryItem;


//---------------------------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------------------------
CWndInventoryItem::CWndInventoryItem()
:	m_eCurrentItemType( eItemType_Normal )
,	m_eCurrentItemSlot( eItemSlot_1 )
,	m_pButtonItemArrange( NULL )
,	m_pWndConfirmBuy( NULL )
{
	ZeroMemory( m_pButtonItemType,	sizeof( CWndButton* ) * eItemType_Max );
	ZeroMemory( m_pButtonItemSlot,	sizeof( CWndButton* ) * eItemSlot_Max );

	m_vecItemInfo.reserve( MAXITEMCOUNT_OF_ONE_PAGE );
}


//---------------------------------------------------------------------------------------------
// �ı���
//---------------------------------------------------------------------------------------------
CWndInventoryItem::~CWndInventoryItem()
{
	SAFE_DELETE( m_pWndConfirmBuy );
}


//---------------------------------------------------------------------------------------------
// �ʱ�ȭ
// param	: �θ� ������, ���� ������ ���̵�
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndInventoryItem::Initialize( CWndBase* pWndParent, DWORD dwWndId )
{
	FLERROR_ASSERT_LOG_RETURN( pWndParent,	FALSE, _T("[������ �κ��丮(������)] �θ� ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
	FLERROR_ASSERT_LOG_RETURN( dwWndId,		FALSE, _T("[������ �κ��丮(������)] ���� ������ ���̵� ��ȿ���� �ʽ��ϴ�.") );

	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), dwWndId, 0, CPoint( 0, 0 ), pWndParent );
}


//---------------------------------------------------------------------------------------------
// ������ ��Ʈ�� ���� ������
// param	: ����
// return	: void
//---------------------------------------------------------------------------------------------
CRect CWndInventoryItem::GetWndCtrlRect( DWORD dwWndId )
{
	CRect rtWndCtrl( 0, 0, 0 ,0 );

	LPWNDCTRL pWndCtrl( GetWndCtrl( dwWndId ) );
	FLERROR_ASSERT_LOG_RETURN( pWndCtrl, rtWndCtrl, _T("[������ �κ��丮(������)] ������ ��Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�. ID - [ %u ]"), dwWndId ); 

	return pWndCtrl->rect;
}


//---------------------------------------------------------------------------------------------
// ������Ʈ �ʱ�ȭ
// param	: ����
// return	: void
//---------------------------------------------------------------------------------------------
void CWndInventoryItem::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 

	m_pButtonItemType[eItemType_Normal]		= static_cast<CWndButton*>( GetDlgItem(WIDC_INV_ITEM_ICON) );
	m_pButtonItemType[eItemType_Pet]		= static_cast<CWndButton*>( GetDlgItem(WIDC_INV_PET_ICON) );
	m_pButtonItemType[eItemType_Costume]	= static_cast<CWndButton*>( GetDlgItem(WIDC_INV_COSTUME_ICON) );
	m_pButtonItemType[eItemType_Quest]		= static_cast<CWndButton*>( GetDlgItem(WIDC_INV_QUEST_ICON) );

	m_pButtonItemSlot[eItemSlot_1]			= static_cast<CWndButton*>( GetDlgItem(WIDC_BUTT_INV_SLOT1) );
	m_pButtonItemSlot[eItemSlot_2]			= static_cast<CWndButton*>( GetDlgItem(WIDC_BUTT_INV_SLOT2) );
	m_pButtonItemSlot[eItemSlot_3]			= static_cast<CWndButton*>( GetDlgItem(WIDC_BUTT_INV_SLOT3) );
	m_pButtonItemSlot[eItemSlot_4]			= static_cast<CWndButton*>( GetDlgItem(WIDC_BUTT_INV_SLOT4) );

	m_pButtonItemArrange					= static_cast<CWndButton*>( GetDlgItem(WIDC_INV_SORT_ICON) );

	SetSelectedButtonFromItemType();
	SetSelectedButtonFromItemSlot();

	CalculationItemRect();
}  


//---------------------------------------------------------------------------------------------
// ������ �簢�� ������ ����Ѵ�.
// param	: ����
// return	: void
//---------------------------------------------------------------------------------------------
void CWndInventoryItem::CalculationItemRect()
{
	CRect	rtItemTotalSpace( GetWndCtrlRect( WIDC_ITEM_TOTAL_SPACE ) );  
	CRect	rtItemDrawSpace( GetWndCtrlRect( WIDC_ITEM_DRAW_SPACE ) );  
	CRect	rtItemOrgDrawSpace( rtItemDrawSpace );  

	WORD	wOffsetVertical( rtItemTotalSpace.Width() - rtItemDrawSpace.Width() );
	WORD	wOffsetHorizon( rtItemTotalSpace.Height() - rtItemDrawSpace.Height() );

	WORD	wCount( NULL );

	for( BYTE byVerticalCount = 0; byVerticalCount < VERTICAL_ITEMCOUNT; byVerticalCount++ )
	{
		rtItemDrawSpace = rtItemOrgDrawSpace;
		rtItemDrawSpace.OffsetRect( 0, (rtItemDrawSpace.Height() + wOffsetVertical) * byVerticalCount );

		for( BYTE byHorizonCount = 0; byHorizonCount < HORIZON_ITEMCOUNT; byHorizonCount++ )
		{
			ITEMINFO stItemInfo;
			stItemInfo.rtDrawItemSpace = rtItemDrawSpace;

			m_vecItemInfo.push_back( stItemInfo );
			
			rtItemDrawSpace.OffsetRect( rtItemDrawSpace.Width() + wOffsetHorizon, 0 );
			wCount++;
		}
	}

	if( m_vecItemInfo.size() > MAXITEMCOUNT_OF_ONE_PAGE )
		FLERROR_ASSERT_LOG_RETURN_VOID( NULL, _T("[������ �κ��丮(������)] �� ���������� ������ �� �ִ� ������ ���� ���� ���� �� ������ �� �����ϴ�.") );
}


//---------------------------------------------------------------------------------------------
// ���� ������ Ÿ�Կ� ���� ��ư�� �����Ѵ�.
// param	: ����
// return	: void
//---------------------------------------------------------------------------------------------
void CWndInventoryItem::SetSelectedButtonFromItemType()
{
	for( BYTE byCount = eItemType_Normal; byCount < eItemType_Max; byCount++ )
	{
		FLERROR_ASSERT_LOG_DO( m_pButtonItemType[byCount], continue, _T("[������ �κ��丮(������)] ������ Ÿ�� ��ư �����Ͱ� ��ȿ���� �ʽ��ϴ� : [ %d ]"), byCount );
		m_pButtonItemType[byCount]->SetCheck( NULL );

		if( byCount == m_eCurrentItemType )
			m_pButtonItemType[byCount]->SetCheck( 2 );
	}
}



//---------------------------------------------------------------------------------------------
// ���� ������ ���� ��ȣ�� ���� ��ư�� �����Ѵ�.
// param	: ����
// return	: void
//---------------------------------------------------------------------------------------------
void CWndInventoryItem::SetSelectedButtonFromItemSlot()
{
	for( BYTE byCount = eItemSlot_1; byCount < eItemSlot_Max; byCount++ )
	{
		FLERROR_ASSERT_LOG_DO( m_pButtonItemSlot[byCount], continue, _T("[������ �κ��丮(������)] ���� ��ư �����Ͱ� ��ȿ���� �ʽ��ϴ� : [ %d ]"), byCount );
		m_pButtonItemSlot[byCount]->SetCheck( NULL );

		if( byCount == m_eCurrentItemSlot )
			m_pButtonItemSlot[byCount]->SetCheck( 2 );
	}
}


//---------------------------------------------------------------------------------------------
// ������ ����Ʈ�� �����Ѵ� ( �Ϲ� )
// param	: void
// return	: ���� / ����
// ����		: �������� ���� ������ �迭�� �����Ǿ��ִ�... ����.. �׷��� �´� �迭�� �ε��� �����Ͽ�
//			  �������� �����ͼ� �־�� �Ѵ�.
//---------------------------------------------------------------------------------------------
bool CWndInventoryItem::UpdateItemList_Normal()
{
	if( eItemType_Normal != m_eCurrentItemType )
		return false;

	WORD wStartCount( MAX_HUMAN_PARTS );
	WORD wEndCount( MAX_INVENINDEX_GENERAL );

	WORD nCurrentCount( NULL );

	for( WORD wCount = wStartCount; wCount < wEndCount; wCount++ )
	{
		FLItemElem* pItemElem( g_pPlayer->m_Inventory.GetAt( wCount ) );
		if( pItemElem )
			m_vecItemInfo[nCurrentCount].pItemElem = pItemElem;

		m_vecItemInfo[nCurrentCount].dwOrgArrayIndex = wCount;
		nCurrentCount++;
	}

	return true;
}


//---------------------------------------------------------------------------------------------
// ������ ����Ʈ�� �����Ѵ� ( �� )
// param	: void
// return	: ���� / ����
// ����		: �������� ���� ������ �迭�� �����Ǿ��ִ�... ����.. �׷��� �´� �迭�� �ε��� �����Ͽ�
//			  �������� �����ͼ� �־�� �Ѵ�.
//---------------------------------------------------------------------------------------------
bool CWndInventoryItem::UpdateItemList_Pet()
{
	if( eItemType_Pet != m_eCurrentItemType )
		return false;

	WORD wStartCount( MAX_INVENINDEX_GENERAL );
	WORD wEndCount( MAX_INVENINDEX_PET );

	WORD nCurrentCount( NULL );

	for( WORD wCount = wStartCount; wCount < wEndCount; wCount++ )
	{
		FLItemElem* pItemElem( g_pPlayer->m_Inventory.GetAt( wCount ) );
		if( pItemElem )
			m_vecItemInfo[nCurrentCount].pItemElem = pItemElem;

		m_vecItemInfo[nCurrentCount].dwOrgArrayIndex = wCount;
		nCurrentCount++;
	}

	return true;
}


//---------------------------------------------------------------------------------------------
// ������ ����Ʈ�� �����Ѵ� ( �ڽ�Ƭ )
// param	: void
// return	: ���� / ����
// ����		: �������� ���� ������ �迭�� �����Ǿ��ִ�... ����.. �׷��� �´� �迭�� �ε��� �����Ͽ�
//			  �������� �����ͼ� �־�� �Ѵ�.
//---------------------------------------------------------------------------------------------
bool CWndInventoryItem::UpdateItemList_Costume()
{
	if( eItemType_Costume != m_eCurrentItemType )
		return false;

	WORD wStartCount( MAX_INVENINDEX_PET );
	WORD wEndCount( MAX_INVENINDEX_COSTUME );

	WORD nCurrentCount( NULL );

	for( WORD wCount = wStartCount; wCount < wEndCount; wCount++ )
	{
		FLItemElem* pItemElem( g_pPlayer->m_Inventory.GetAt( wCount ) );
		if( pItemElem )
			m_vecItemInfo[nCurrentCount].pItemElem = pItemElem;

		m_vecItemInfo[nCurrentCount].dwOrgArrayIndex = wCount;
		nCurrentCount++;
	}

	return true;
}


//---------------------------------------------------------------------------------------------
// ������ ����Ʈ�� �����Ѵ� ( ����Ʈ )
// param	: void
// return	: ���� / ����
// ����		: �������� ���� ������ �迭�� �����Ǿ��ִ�... ����.. �׷��� �´� �迭�� �ε��� �����Ͽ�
//			  �������� �����ͼ� �־�� �Ѵ�.
//---------------------------------------------------------------------------------------------
bool CWndInventoryItem::UpdateItemList_Quest()
{
	if( eItemType_Quest != m_eCurrentItemType )
		return false;

	WORD wStartCount( MAX_INVENINDEX_COSTUME );
	WORD wEndCount( MAX_INVENINDEX_QUEST );

	WORD nCurrentCount( NULL );

	for( WORD wCount = wStartCount; wCount < wEndCount; wCount++ )
	{
		FLItemElem* pItemElem( g_pPlayer->m_Inventory.GetAt( wCount ) );
		if( pItemElem )
			m_vecItemInfo[nCurrentCount].pItemElem = pItemElem;

		m_vecItemInfo[nCurrentCount].dwOrgArrayIndex = wCount;
		nCurrentCount++;
	}

	return true;
}


//---------------------------------------------------------------------------------------------
// ������ ����Ʈ�� �ʱ�ȭ �Ѵ�.
// param	: void
// return	: ���� / ����
// ����		: ������ ������ �ٲ��� �����Ƿ� ������ �����͸� �ʱ�ȭ ��Ų��.
//---------------------------------------------------------------------------------------------
void CWndInventoryItem::ClearItemList()
{
	for( UINT nCount = 0; nCount < m_vecItemInfo.size(); nCount++ )
		m_vecItemInfo[nCount].pItemElem = NULL;
}


//---------------------------------------------------------------------------------------------
// ó��
// param	: void
// return	: ���� / ����
//---------------------------------------------------------------------------------------------
BOOL CWndInventoryItem::Process()
{
	if( NULL == g_pPlayer )
		return FALSE;

	ClearItemList();
	UpdateItemList_Normal();
	UpdateItemList_Pet();
	UpdateItemList_Costume();
	UpdateItemList_Quest();

	return TRUE;
}


//---------------------------------------------------------------------------------------------
// ����� ���� ���� �̺�Ʈ ó��
// param	: ������ ���̵�, �޽���, �⺻ ������
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndInventoryItem::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase )
{
	return CWndBase::OnCommand( nID, dwMessage, pWndBase );
}
 

//---------------------------------------------------------------------------------------------
// �ڽ����� ���� �޽��� ����
// param	: �޽���, ���̵�, ���
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndInventoryItem::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	switch( nID )
	{
		case WIDC_INV_ITEM_ICON:	
			m_eCurrentItemType = eItemType_Normal;
			break;

		case WIDC_INV_PET_ICON:	
			m_eCurrentItemType = eItemType_Pet;
			break;

		case WIDC_INV_COSTUME_ICON:	
			m_eCurrentItemType = eItemType_Costume;
			break;

		case WIDC_INV_QUEST_ICON:	
			m_eCurrentItemType = eItemType_Quest;
			break;

		case WIDC_BUTT_INV_SLOT1:	
			m_eCurrentItemSlot = eItemSlot_1;
			break;

		case WIDC_BUTT_INV_SLOT2:	
			m_eCurrentItemSlot = eItemSlot_2;
			break;

		case WIDC_BUTT_INV_SLOT3:	
			m_eCurrentItemSlot = eItemSlot_3;
			break;

		case WIDC_BUTT_INV_SLOT4:	
			m_eCurrentItemSlot = eItemSlot_4;
			break;

		default:
			break;
	}

	SetSelectedButtonFromItemType();
	SetSelectedButtonFromItemSlot();

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 


//---------------------------------------------------------------------------------------------
// ������ ������ �׸���
// param	: ������
// return	: void
//---------------------------------------------------------------------------------------------
bool CWndInventoryItem::DrawItemIcon( C2DRender* const p2DRender )
{
	CPoint	ptMouse( GetMousePoint() );

	for( UINT nCount = 0; nCount < m_vecItemInfo.size(); nCount++ )
	{
		ITEMINFO	stItemInfo( m_vecItemInfo[nCount] );
		bool		bChangeTextureSize( false );
		
		FLItemElem* pItemElem( stItemInfo.pItemElem );
		if( NULL == pItemElem )
			continue;

		PT_ITEM_SPEC pItemSpec( pItemElem->GetProp() );
		if( NULL == pItemSpec )
			continue;

		CTexture* pTexture = m_textureMng.AddTexture( p2DRender->m_pd3dDevice, MakePath( DIR_ITEM, pItemSpec->szIcon ), WNDCOLOR_DEFAULT_KEY );
		if( NULL == pTexture )
			continue;

		if( stItemInfo.rtDrawItemSpace.PtInRect( ptMouse ) )
		{
			pTexture->m_size.cx += 4;
			pTexture->m_size.cy += 4;
			bChangeTextureSize = true;
		}
		
		CPoint	ptTexture( 0, 0 );
		CRect	rtOrgItemDrawSpace( GetWndCtrlRect( WIDC_ITEM_DRAW_SPACE ) );  

		long nOffsetX = static_cast<long>( ( rtOrgItemDrawSpace.Width() - pTexture->m_size.cx ) * 0.5f );
		long nOffsetY = static_cast<long>( ( rtOrgItemDrawSpace.Width() - pTexture->m_size.cx ) * 0.5f );

		ptTexture.x = stItemInfo.rtDrawItemSpace.left + nOffsetX;
		ptTexture.y = stItemInfo.rtDrawItemSpace.top + nOffsetY;

		if( pItemElem->IsFlag( FLItemElem::expired ) )
			pTexture->Render2( p2DRender, ptTexture, D3DCOLOR_ARGB( 255, 255, 100, 100 ) );
		else
			pTexture->Render( p2DRender, ptTexture );

		if( bChangeTextureSize )
		{
			pTexture->m_size.cx -= 4;
			pTexture->m_size.cy -= 4;
		}
	}

	return true;
}


//---------------------------------------------------------------------------------------------
// �׸���
// param	: ������
// return	: void
//---------------------------------------------------------------------------------------------
void CWndInventoryItem::OnDraw( C2DRender* p2DRender )
{
	if( NULL == p2DRender )
		return ;

	DrawItemIcon( p2DRender );
}


//---------------------------------------------------------------------------------------------
// ���콺�� ������ ���� �ö��� �� 
// param	: ���콺 ��ġ
// return	: void
//---------------------------------------------------------------------------------------------
void CWndInventoryItem::OnMouseWndSurface(  CPoint point  )
{
	if( m_vecItemInfo.empty() )
		return;

	for( UINT nCount = 0; nCount < m_vecItemInfo.size(); nCount++ )
	{
		ITEMINFO stItemInfo( m_vecItemInfo[nCount] );
		
		if( stItemInfo.rtDrawItemSpace.PtInRect( point ) )
		{
			FLItemBase* pItemBase( stItemInfo.pItemElem );
			if( NULL == pItemBase )
				return ;

			CPoint	ptConvert( point );
			CRect	rtConvert( stItemInfo.rtDrawItemSpace );

			ClientToScreen( &ptConvert );
			ClientToScreen( &rtConvert );
		
			g_toolTip.SetSubToolTipNumber( 0 );
			g_WndMng.PutToolTip_Item( pItemBase, ptConvert, &rtConvert, APP_INVENTORY );

			return;
		}
	}
}

//---------------------------------------------------------------------------------------------
// ���콺 ���� ��ư ������ ��
// param	: �÷���, ���콺 ��ġ
// return	: void
//---------------------------------------------------------------------------------------------
void CWndInventoryItem::OnLButtonDown( UINT nFlags, CPoint point )
{
}


//---------------------------------------------------------------------------------------------
// ���콺 ���� ��ư ���� Ŭ�� ���� �� ( ������ ���� )
// param	: �÷���, ���콺 ��ġ
// return	: void
//---------------------------------------------------------------------------------------------
void CWndInventoryItem::OnLButtonDblClk( UINT nFlags, CPoint point )
{
}


//---------------------------------------------------------------------------------------------
// �������� ���´�.
// param	: �÷���, ���콺 ��ġ
// return	: void
//---------------------------------------------------------------------------------------------
void CWndInventoryItem::PickItem( const CPoint point )
{
	if( false == m_bLButtonDown )
		return ;

	if( FALSE == m_GlobalShortcut.IsEmpty() )
		return ;

	for( UINT nCount = 0; nCount < m_vecItemInfo.size(); nCount++ )
	{
		ITEMINFO stItemInfo( m_vecItemInfo[nCount] );
		
		if( stItemInfo.rtDrawItemSpace.PtInRect( point ) )
		{
			FLItemElem* pItemElem( stItemInfo.pItemElem ); 
			
			if( NULL == pItemElem )
				return ;
			
			if( FALSE == IsUsableItem( pItemElem ) )
				return ;
			
			///& m_useDieFlag �̰� ����? ������ ���� ���õǾ� �ִٴµ� �������� ����� �� �ִ� ��Ȳ�� üũ �ϴ°� ����.
			/// !pWndInventory->m_bRemoveJewel �̰� �� ����....
			/// if( ((pItemElem != NULL && m_useDieFlag) || IsUsableItem( pItemElem )) && !pWndInventory->m_bRemoveJewel) ( �����ڵ� )

			m_GlobalShortcut.Empty();
			m_GlobalShortcut.m_pFromWnd   = this;
			m_GlobalShortcut.m_dwShortcut = SHORTCUT_ITEM;
			m_GlobalShortcut.m_dwType     = ITYPE_ITEM;
			m_GlobalShortcut.m_dwId       = pItemElem->m_dwObjId;
			m_GlobalShortcut.m_dwIndex    = stItemInfo.dwOrgArrayIndex;
			m_GlobalShortcut.m_pTexture   = pItemElem->GetTexture();
			m_GlobalShortcut.m_dwData     = (DWORD)pItemElem;
				
			FLStrcpy( m_GlobalShortcut.m_szString, _countof( m_GlobalShortcut.m_szString ), pItemElem->GetName() );
		}
	}
}



//---------------------------------------------------------------------------------------------
// ���콺 �������� ��
// param	: �÷���, ���콺 ��ġ
// return	: void
//---------------------------------------------------------------------------------------------
void CWndInventoryItem::OnMouseMove( UINT nFlags, CPoint point )
{
	PickItem( point );
}


//---------------------------------------------------------------------------------------------
// ��ȿ�� ���� ������ �����ΰ� ?
// param	: ���� ������ ����
// return	: �� / �ƴϿ�
//---------------------------------------------------------------------------------------------
bool CWndInventoryItem::IsValidShortCutInfo( LPSHORTCUT pShortcut )
{
	FLERROR_ASSERT_LOG_RETURN( pShortcut, false, _T("[������ �κ��丮(������)] ���� �� ������ ������ ��ȿ���� �ʽ��ϴ�.") );

	if( SHORTCUT_ITEM != pShortcut->m_dwShortcut )
		return false;

	if( ITYPE_ITEM != pShortcut->m_dwType )	/// ī�� , �� , ť�� �� ���� ���� �ʳ�?
		return false;

	if( NULL == pShortcut->m_pFromWnd )
		return false;

	CWndBase* pWndFromWnd( pShortcut->m_pFromWnd->GetFrameWnd() );
	if( NULL == pWndFromWnd )
		return false;

	return true;
}


//---------------------------------------------------------------------------------------------
// ������ �÷����� ��
// param	: ���� ������ ����, ���콺 ��ġ
// return	: void
//---------------------------------------------------------------------------------------------
BOOL CWndInventoryItem::OnDropIcon( LPSHORTCUT pShortcut, CPoint point )
{
	/// �ŷ��� �Ǵ� ���� �ŷ� �� �̷��� ���ƾ� �ϳ� ������ ����

	if( false == IsValidShortCutInfo( pShortcut ) )
		FLERROR_ASSERT_LOG_RETURN( NULL, FALSE, _T("[������ �κ��丮(������)] ���� �� ������ ������ ��ȿ���� �ʽ��ϴ�.") );

	ProcessItemMove_FromInventoryItem( pShortcut, point );	
	ProcessItemMove_FromPrivateBank( pShortcut, point );
	ProcessItemMove_FromGuildBank( pShortcut, point );
	ProcessItemMove_FromMobileBag( pShortcut, point );
	ProcessItemMove_FromVendor( pShortcut, point );
	ProcessItemMove_FromPost( pShortcut, point );

	return CWndBase::OnDropIcon( pShortcut, point );
}



//---------------------------------------------------------------------------------------------
// ������ �̵� ó�� ( ������ �κ��丮�� ���� ���� �� [�ڱ� �ڽ�])
// param	: ���� ������ ����, ���콺 ��ġ
// return	: void
//---------------------------------------------------------------------------------------------
bool CWndInventoryItem::ProcessItemMove_FromInventoryItem( const SHORTCUT* const pShortcut, const CPoint point )
{
	CWndBase* pWndFrame( pShortcut->m_pFromWnd->GetFrameWnd() );
	if( NULL == pWndFrame )
		return false;

	if( APP_INVENTORY_ITEM != pWndFrame->GetWndId() )
		return false;

	/// ������ �ƴ��� �Ǵ��ؼ� ó���ؾ� ���� ������?

	for( UINT nCount = 0; nCount < m_vecItemInfo.size(); nCount++ )
	{
		ITEMINFO stItemInfo( m_vecItemInfo[nCount] );
		
		if( stItemInfo.rtDrawItemSpace.PtInRect( point ) )
		{
			FLItemElem* pItemElem( g_pPlayer->m_Inventory.GetAt( pShortcut->m_dwIndex ) );
			
			if( NULL == pItemElem )
				return false;
			
			if( FALSE == IsUsableItem( pItemElem ) )
				return false;

			if( IsUsingItem( pItemElem ) )
				return false;
			
			g_DPlay.SendMoveItem( pShortcut->m_dwIndex, stItemInfo.dwOrgArrayIndex );
			return true;
		}
	}

	return false;
}


//---------------------------------------------------------------------------------------------
// ������ �̵� ó�� ( ���� �������� ���� ���� �� )
// param	: ���� ������ ����, ���콺 ��ġ
// return	: void
//---------------------------------------------------------------------------------------------
bool CWndInventoryItem::ProcessItemMove_FromPrivateBank( const SHORTCUT* const pShortcut, const CPoint point )
{
	CWndBase* pWndFrame( pShortcut->m_pFromWnd->GetFrameWnd() );
	if( NULL == pWndFrame )
		return false;

	if( APP_COMMON_BANK != pWndFrame->GetWndId() )
		return false;

	FLERROR_ASSERT_LOG_RETURN( g_WndMng.m_pWndBank, false, _T("[������ �κ��丮(������)] ���� �����찡 �����ϴ�. ������ �̵��� �����մϴ�.") )

	BYTE byBankItemSlotIndex( g_WndMng.m_pWndBank->GetCurrentItemSlotIndex() );
	BYTE byBankMoneySlotIndex( g_WndMng.m_pWndBank->GetCurrentMoneySlotIndex() );

	DWORD dwItemElem( pShortcut->m_dwData );
	FLItemElem* pItemElem = (FLItemElem*)(dwItemElem );
	if( pItemElem )	// ������ 
	{
		if( pItemElem->m_nItemNum > 1 )
		{
			CWndCommonInputCount* pWndCommonInputCount( g_WndMng.GetWndCommonInputCount( this ) );
			FLERROR_ASSERT_LOG_RETURN( pWndCommonInputCount, false, _T("[������ �κ��丮(������)] ���� ���� �Է� ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
				
			pWndCommonInputCount->SetInputCountInfo( pShortcut, byBankItemSlotIndex );
		}
		else
			g_DPlay.SendGetItemBank( byBankItemSlotIndex, static_cast<BYTE>( pShortcut->m_dwId ), pItemElem->m_nItemNum );

		return true;
	
	}
	else // ��
	{
		CWndCommonInputCount* pWndCommonInputCount( g_WndMng.GetWndCommonInputCount( this ) );
		FLERROR_ASSERT_LOG_RETURN( pWndCommonInputCount, false, _T("[������ �κ��丮(������)] ���� ���� �Է� ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
			
		pWndCommonInputCount->SetInputCountInfo( pShortcut, byBankMoneySlotIndex );
		return true;
	}

	return false;
}


//---------------------------------------------------------------------------------------------
// ������ �̵� ó�� ( ��� �������� ���� ���� �� )
// param	: ���� ������ ����, ���콺 ��ġ
// return	: void
//---------------------------------------------------------------------------------------------
bool CWndInventoryItem::ProcessItemMove_FromGuildBank( const SHORTCUT* const pShortcut, const CPoint point )
{
	CWndBase* pWndFrame( pShortcut->m_pFromWnd->GetFrameWnd() );
	if( NULL == pWndFrame )
		return false;

	if( APP_GUILD_BANK != pWndFrame->GetWndId() )
		return false;

	FLERROR_ASSERT_LOG_RETURN( g_WndMng.m_pWndGuildBank, false, _T("[������ �κ��丮(������)] ������� �����찡 �����ϴ�. ������ �̵��� �����մϴ�.") )

	CGuild* pGuild( g_pPlayer->GetGuild() );
	FLERROR_ASSERT_LOG_RETURN( pGuild, false, _T("[������ �κ��丮(������)] ĳ���Ͱ� ��忡 �������� �ʽ��ϴ�.") )

	if( FALSE == pGuild->IsGetItem( g_pPlayer->m_idPlayer ) )
		return false;

	DWORD dwItemElem( pShortcut->m_dwData );
	FLItemElem* pItemElem = (FLItemElem*)(dwItemElem );
	if( pItemElem )	// ������ 
	{
		if( pItemElem->m_nItemNum > 1 )
		{
			CWndCommonInputCount* pWndCommonInputCount( g_WndMng.GetWndCommonInputCount( this ) );
			FLERROR_ASSERT_LOG_RETURN( pWndCommonInputCount, false, _T("[������ �κ��丮(������)] ���� ���� �Է� ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
				
			pWndCommonInputCount->SetInputCountInfo( pShortcut );
		}
		else
			g_DPlay.SendGetItemGuildBank( static_cast<BYTE>( pShortcut->m_dwId ), 1, 1 );

		return true;
	
	}
	else // ��
	{
		CWndCommonInputCount* pWndCommonInputCount( g_WndMng.GetWndCommonInputCount( this ) );
		FLERROR_ASSERT_LOG_RETURN( pWndCommonInputCount, false, _T("[������ �κ��丮(������)] ���� ���� �Է� ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
			
		pWndCommonInputCount->SetInputCountInfo( pShortcut );
		return true;
	}

	return false;
}


//---------------------------------------------------------------------------------------------
// ������ �̵� ó�� ( �������� ���� ���� �� )
// param	: ���� ������ ����, ���콺 ��ġ
// return	: void
//---------------------------------------------------------------------------------------------
bool CWndInventoryItem::ProcessItemMove_FromPost( const SHORTCUT* const pShortcut, const CPoint point )
{
	CWndBase* pWndFrame( pShortcut->m_pFromWnd->GetFrameWnd() );
	if( NULL == pWndFrame )
		return false;

	if( APP_POST_READ != pWndFrame->GetWndId() )
		return false;

	CWndPostRead* pWndPostRead = static_cast<CWndPostRead*>( pWndFrame );
	FLERROR_ASSERT_LOG_RETURN( pWndPostRead, false, _T("[������ �κ��丮(������)] ���� ���� �����찡 �����ϴ�. ������ �̵��� �����մϴ�.") )

	if( pShortcut->m_dwData != 0 )
		pWndPostRead->MailReceiveItem();
	else
		pWndPostRead->MailReceiveGold();
					
	return true;
}


//---------------------------------------------------------------------------------------------
// ������ �̵� ó�� ( �޴밡������ ���� ���� �� )
// param	: ���� ������ ����, ���콺 ��ġ
// return	: void
//---------------------------------------------------------------------------------------------
bool CWndInventoryItem::ProcessItemMove_FromMobileBag( const SHORTCUT* const pShortcut, const CPoint point )
{
	CWndBase* pWndFrame( pShortcut->m_pFromWnd->GetFrameWnd() );
	if( NULL == pWndFrame )
		return false;

	if( APP_BAG_EX != pWndFrame->GetWndId() )
		return false;

	if( pShortcut->m_dwData == 0 )
		return false;

	DWORD dwItemElem( pShortcut->m_dwData );
	FLItemElem* pItemElem = (FLItemElem*)(dwItemElem );
	FLERROR_ASSERT_LOG_RETURN( pItemElem, false, _T("[������ �κ��丮(������)] ���� ������ ���� ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

	BYTE		bySlot( NULL );
	CWndBase*	pWndSlot( pWndFrame->m_pCurFocus );
	
	if( pWndSlot->m_pParentWnd->GetWndId() == WIDC_BASIC )
	{
		bySlot = 0;
	}
	else if( pWndSlot->m_pParentWnd->GetWndId() == WIDC_EXBAG1 )
	{
		if( FALSE == g_pPlayer->m_Pocket.IsAvailable(1) )
			return false;
		else		
			bySlot = 1;
	}
	else
	{
		if( FALSE == g_pPlayer->m_Pocket.IsAvailable(2) )
			return false;
		else		
			bySlot = 2;
	}

	if( pItemElem->m_nItemNum > 1 )
	{
		CWndCommonInputCount* pWndCommonInputCount( g_WndMng.GetWndCommonInputCount( this ) );
		FLERROR_ASSERT_LOG_RETURN( pWndCommonInputCount, false, _T("[������ �κ��丮(������)] ���� ���� �Է� ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
			
		pWndCommonInputCount->SetInputCountInfo( pShortcut, bySlot );
	}
	else
		g_DPlay.SendMoveItem_Pocket( bySlot, pShortcut->m_dwId, 1 , -1 );

	return true;
}


//---------------------------------------------------------------------------------------------
// ������ �̵� ó�� ( ���λ������� ���� ���� �� )
// param	: ���� ������ ����, ���콺 ��ġ
// return	: void
//---------------------------------------------------------------------------------------------
bool CWndInventoryItem::ProcessItemMove_FromVendor( const SHORTCUT* const pShortcut, const CPoint point )
{
	CWndBase* pWndFrame( pShortcut->m_pFromWnd->GetFrameWnd() );
	if( NULL == pWndFrame )
		return false;

	if( APP_VENDOR_REVISION != pWndFrame->GetWndId() )
		return false;

	if( pShortcut->m_dwData == 0 )
		return false;

	CWndVendor* pWndVendor( static_cast<CWndVendor*>( pWndFrame ) ); 
	FLERROR_ASSERT_LOG_RETURN( pWndVendor, false, _T("[������ �κ��丮(������)] ���λ��� ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

	if( pWndVendor->m_pVendor == g_pPlayer )
		g_DPlay.SendUnregisterPVendorItem( static_cast<BYTE>( pShortcut->m_dwIndex ) );
	else
	{
		SAFE_DELETE( pWndVendor->m_pWndVendorBuy );
		pWndVendor->m_pWndVendorBuy	= new CWndVendorBuy( (FLItemBase*)pShortcut->m_dwData, pShortcut->m_dwIndex );
		FLERROR_ASSERT_LOG_RETURN( pWndVendor, false, _T("[������ �κ��丮(������)] ���λ��� ���� â ���� ����.") );
		pWndVendor->m_pWndVendorBuy->Initialize( pWndVendor, APP_VENDOR_BUY );
	}

	return true;
}
