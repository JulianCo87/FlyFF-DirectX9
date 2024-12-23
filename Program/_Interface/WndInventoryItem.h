
#pragma once 

namespace WndInventoryItem
{
	enum eItemType : BYTE
	{
		eItemType_Normal = 0,
		eItemType_Pet,
		eItemType_Costume,
		eItemType_Quest,

		eItemType_Max,
	};

	enum eItemSlot : BYTE
	{
		eItemSlot_1 = 0,
		eItemSlot_2,
		eItemSlot_3,
		eItemSlot_4,

		eItemSlot_Max,
	};

	typedef struct stItemInfo
	{
		DWORD			dwOrgArrayIndex;	// ������ ������ �迭������ �ε���
		FLItemElem*		pItemElem;			// ������ ������
		CRect			rtDrawItemSpace;	// �κ��丮�� �׷����� �簢�� ���� ����

		stItemInfo()
		:	dwOrgArrayIndex( NULL_ID )
		,	pItemElem( NULL )
		,	rtDrawItemSpace( 0, 0, 0, 0 )
		{
		}
	}ITEMINFO, *PITEMINFO;
}

using namespace WndInventoryItem;

//---------------------------------------------------------------------------------------------
// �κ��丮 ������ ������ 
//---------------------------------------------------------------------------------------------
class CWndInventoryItem		:	public CWndNeuz
{

public:


												CWndInventoryItem( void );
	virtual										~CWndInventoryItem( void );

	virtual BOOL 								Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);

	virtual BOOL								Process( void );
	virtual	void 								OnInitialUpdate( void );
	virtual void 								OnDraw( C2DRender* p2DRender );
	virtual BOOL								OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual BOOL								OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual BOOL								OnDropIcon( LPSHORTCUT pShortcut, CPoint point );
	virtual void								OnMouseMove( UINT nFlags, CPoint point );
	virtual void								OnLButtonDown( UINT nFlags, CPoint point );
	virtual void								OnLButtonDblClk( UINT nFlags, CPoint point );
	virtual	void								OnMouseWndSurface( CPoint point );


private:


	CRect										GetWndCtrlRect( DWORD dwWndId );

	void										SetSelectedButtonFromItemType( void );
	void										SetSelectedButtonFromItemSlot( void );

	void										CalculationItemRect( void );

	void										ClearItemList( void );
	bool										UpdateItemList_Normal( void );
	bool										UpdateItemList_Pet( void );
	bool										UpdateItemList_Costume( void );
	bool										UpdateItemList_Quest( void );

	bool										DrawItemIcon( C2DRender* const p2DRender );

	void										PickItem( const CPoint point );

	bool										IsValidShortCutInfo( LPSHORTCUT pShortcut );
	bool										ProcessItemMove_FromInventoryItem( const SHORTCUT* const pShortcut, const CPoint point );
	bool										ProcessItemMove_FromPrivateBank( const SHORTCUT* const pShortcut, const CPoint point );
	bool										ProcessItemMove_FromGuildBank( const SHORTCUT* const pShortcut, const CPoint point );
	bool										ProcessItemMove_FromMobileBag( const SHORTCUT* const pShortcut, const CPoint point );
	bool										ProcessItemMove_FromVendor( const SHORTCUT* const pShortcut, const CPoint point );
	bool										ProcessItemMove_FromPost( const SHORTCUT* const pShortcut, const CPoint point );


private:


	eItemType									m_eCurrentItemType;
	eItemSlot									m_eCurrentItemSlot;

	CWndButton*									m_pButtonItemArrange;									
	CWndButton*									m_pButtonItemType[ eItemType_Max ];
	CWndButton*									m_pButtonItemSlot[ eItemSlot_Max ];

	vector<ITEMINFO>							m_vecItemInfo;

	CWndConfirmBuy*								m_pWndConfirmBuy;
};