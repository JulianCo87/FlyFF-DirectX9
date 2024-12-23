
#pragma once 

namespace WndFlyffPieceSystem
{
	//---------------------------------------------------------------------------------------------
	// Ʈ�� ������ ������
	//---------------------------------------------------------------------------------------------
	typedef struct stTREEDATA
	{
		DWORD		_dwKey;
		DWORD		_dwParentKey;

		CString		_strTitle;
		DWORD		_dwType[2];

		stTREEDATA()
		:	_dwKey( NULL_ID )
		,	_dwParentKey( NULL_ID )
		,	_strTitle( _T("") )
		{  
			ZeroMemory( _dwType, sizeof( DWORD ) * 2 );
		}
	}TREEDATA, *LPTREEDATA;
}

using namespace WndFlyffPieceSystem;

//---------------------------------------------------------------------------------------------
// ������ ���� �ý���
//---------------------------------------------------------------------------------------------
class CWndFlyffPieceSystem		:	public CWndNeuz
{

private:

	
	CWndTreeCtrl*								m_pExchangeItemKindList;
	CWndListBox*								m_pExchangeItemList;
	CWndButton*									m_pExchangeButton;

	LPWNDCTRL									m_pExchangeItemKindCtrl;
	LPWNDCTRL									m_pExchangeItemCtrl;
	LPWNDCTRL									m_pButtonItem;
	LPWNDCTRL									m_pButtonLevel;
	LPWNDCTRL									m_pButtonCoupon;

	CRect										m_rtExchangeItemKindList;
	CRect										m_rtExchangeItemList;
	CRect										m_rtButtonItem;
	CRect										m_rtButtonLevel;
	CRect										m_rtButtonCoupon;

	CTexture*									m_pTexGuildImage;						// �ȳ� ��� �̹���		
	CTexture*									m_pTexLoadingImage;						// �ε� ��� �̹���	

	CWndListBox::LPLISTITEM						m_pFocusItem;							// ���� ���� �� ������

	bool										m_bClickedExchangeItemKind_Type1;		// ������ ���� Ʈ�� ����Ʈ �߿��� ��з��� Ŭ���Ͽ��°� ?

	bool										m_bSendItemListPacket;					// ��Ŷ Send ������ true, Ack �޾Ҵٸ� false ( ������ ����Ʈ ��û )
	bool										m_bSendItemExchangePacket;				// ��Ŷ Send ������ true, Ack �޾Ҵٸ� false ( ������ ��ȯ ��û )

	BOOL										m_bAscendingOrderByName;				// �������� �����ΰ� (������ �̸�)
	BOOL										m_bAscendingOrderByLevel;				// �������� �����ΰ� (�ʿ� ����)
	BOOL										m_bAscendingOrderByNeedCouponCound;		// �������� �����ΰ� (�ʿ� ���� ����)

	typedef map< DWORD, TREEDATA* >				mapTreeDataContainer;
	typedef mapTreeDataContainer::iterator		mapTreeDataIter;						

	mapTreeDataContainer						m_mapTreeData;							// ��ũ��Ʈ���� �о�帰 ������ ���� ����Ʈ

	typedef vector< T_EXCHANGE_ITEM_DATA >		vecExchangeItem;					
	vecExchangeItem								m_vecExchangeItem;						// ��ȯ ������ ����Ʈ

	typedef vector< CRect >						vecIconRect;	
	vecIconRect									m_vecIconRect;							// ������ ���� ����Ʈ

public:


												CWndFlyffPieceSystem( void );
	virtual										~CWndFlyffPieceSystem( void );

	virtual BOOL 								Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);

	virtual BOOL								Process( void );
	virtual	void 								OnInitialUpdate( void );
	virtual void 								OnDraw( C2DRender* p2DRender );
	virtual BOOL								OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual BOOL								OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 

	bool										SendPacket_ItemListReq( DWORD dwTreeKey );
	void										SetListData( const vecExchangeItem vecItemData );

	void										SetSendItemExchangePacket( bool bSend )		{	m_bSendItemExchangePacket = bSend;		}			


private:


	bool										ParsingScript(  const char* szFileName );

	bool										CreateExchangeItemKindList( void );
	bool										CreateExchangeItemList( void );

	void										DrawGuideImage( C2DRender* p2DRender );
	void										DrawGuideText( C2DRender* p2DRender );

	void										DrawLoadingImage( C2DRender* p2DRender );
	void										DrawLoadingText( C2DRender* p2DRender );

	void										DrawExchangeItem( C2DRender* p2DRender );
	void										DrawItemListEmptyText( C2DRender* p2DRender );
	void										DrawTooltip( void );

	void										SetViewPort( C2DRender* p2DRender );
	void										DrawExchangeItem_Icon( C2DRender* p2DRender, const PT_ITEM_SPEC pProp, CPoint ptItem, const CRect rtItem );
	void										DrawExchangeItem_Name( C2DRender* p2DRender, const PT_ITEM_SPEC pProp, CPoint ptItem, const CRect rtItem );
	void										DrawExchangeItem_Level( C2DRender* p2DRender, const PT_ITEM_SPEC pProp, CPoint ptItem, const CRect rtItem );
	void										DrawExchangeItem_CouponIcon( C2DRender* p2DRender, const PT_ITEM_SPEC pProp, CPoint ptItem, const CRect rtItem );
	void										DrawExchangeItem_CouponCount( C2DRender* p2DRender, const UINT& nCount, CPoint ptItem, const CRect rtItem );
	void										DrawExchangeItem_SelectBox( C2DRender* p2DRender, DWORD dwExchangeItemDataIndex, CPoint ptItem, const CRect rtItem );

	void										DrawExchangeItem_WidthLine( C2DRender* p2DRender, const UINT& nCount, const CRect& rtItem );
	void										DrawExchangeItem_HeightLine( C2DRender* p2DRender );

	void										SendExchangeItemPacket( DWORD dwItemIndex );

	CString										GetExchangeItemMessageBoxText( void );

	BOOL										Process_ExchangeButtonClicked( UINT nID );
	BOOL										Process_CancelButtonClicked( UINT nID );
	BOOL										Process_MessageBoxButtonClicked( UINT nID, CWndBase* pWndBase );
	BOOL										Process_ItemButtonClicked( UINT nID );
	BOOL										Process_LevelButtonClicked( UINT nID );
	BOOL										Process_NeedCouponCountButtonClicked( UINT nID );

	static bool									SortItemListByName_Ascending( const T_EXCHANGE_ITEM_DATA& rLeft, const T_EXCHANGE_ITEM_DATA& rRight );									
	static bool									SortItemListByName_Descending( const T_EXCHANGE_ITEM_DATA& rLeft, const T_EXCHANGE_ITEM_DATA& rRight );										

	static bool									SortItemListByLevel_Ascending( const T_EXCHANGE_ITEM_DATA& rLeft, const T_EXCHANGE_ITEM_DATA&  rRight );										
	static bool									SortItemListByLevel_Descending( const T_EXCHANGE_ITEM_DATA& rLeft, const T_EXCHANGE_ITEM_DATA&  rRight );										
	
	static bool									SortItemListByNeedCouponCount_Ascending( const T_EXCHANGE_ITEM_DATA& rLeft, const T_EXCHANGE_ITEM_DATA&  rRight );										
	static bool									SortItemListByNeedCouponCount_Descending( const T_EXCHANGE_ITEM_DATA& rLeft, const T_EXCHANGE_ITEM_DATA&  rRight );										

};