
#pragma once 

#include "WndCommonItemExchangeConfirm.h"

namespace WndCommonItemExchange
{
	typedef struct stIconDrawInfo
	{
		DWORD	m_dwItemID;
		CRect	m_rtItemIconDraw;		
		DWORD	m_dwMinGeneralEnchant;
		DWORD	m_dwMaxGeneralEnchant;
		BYTE	m_bind_flag;
	
		stIconDrawInfo()
		:	m_dwItemID( NULL_ID )
		,	m_rtItemIconDraw( 0, 0 ,0 ,0 )
		,	m_dwMinGeneralEnchant( NULL )
		,	m_dwMaxGeneralEnchant( NULL )
		,	m_bind_flag( 0 )
		{
		}
	}ICONDRAWINFO, *PICONDRAWINFO;
};

using namespace WndCommonItemExchange;

//---------------------------------------------------------------------------------------------
// ���� ������ ��ȯ ������
//---------------------------------------------------------------------------------------------
class CWndCommonItemExchange	:	public CWndNeuz
{

private:

	CWndText*									m_pWndDescription;
	CWndListBox*								m_pWndExchangeList;

	CTexture*									m_pTexExchangeArrow;

	DWORD										m_dwMMIDefineIndex;
	DWORD										m_dwCurrentDescPageIndex;
	UINT										m_nTotalDescPageCount;

	ExchangeItemListMap							m_mapExchangeItem;

	typedef vector< ICONDRAWINFO >				vecIconDrawRect;	
	vecIconDrawRect								m_vecIconDrawRect;					// ������ �׸��� ���� ����Ʈ

	BYTE										m_byConditionItemMaxCount;			// �ʿ� ������ �� ���� ���� ������ �����Ѵ�. ( �� ���� �׸��� ���� )
	BYTE										m_byPaymentItemMaxCount;			// ���� ������ �� ���� ���� ������ �����Ѵ�. ( �� ���� �׸��� ���� )

	bool										m_bSendPacket;		

	BYTE										m_byPaymentItemColorChangeCount;	// ���� ������ ������ ������ ���� ī��Ʈ
	CTimer										m_PaymentItemColorChangeTimer;		// ���� ������ ���� ����� Ÿ�̸�

	ICONDRAWINFO								m_stCurrentOnMouseItemDrawInfo;		// ���� ���콺�� �ö� �ִ� �������� �׸��� ����

	CWndCommonItemExchangeConfirm*				m_pWndCommonItemExchangeConfirm;	// ��ȯ ���� �Է� â
	DWORD										m_dwPossibleMaxExchangeCount;		// ��ȯ ���� �� �ִ� ����


public:

												CWndCommonItemExchange( void );
	virtual										~CWndCommonItemExchange( void );

	virtual BOOL 								Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);

	virtual	void 								OnInitialUpdate( void );
	virtual void 								OnDraw( C2DRender* p2DRender );
	virtual BOOL								OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 

	bool										SetItemExchangeInfo( const DWORD dwMMIDefineIndex );
	void										SetSendPacket( void );
	void										SetReceivedPacket( void );

	ICONDRAWINFO								GetCurrentOnMouseItemDrawInfo( void );


private:


	bool										SetDescriptionString( void );
	bool										CreateExchangeItemList( void );

	void										UpdatePaymentItemColorChange( void );

	void										SetViewPort( C2DRender* const p2DRender );	
	bool										CheckExchangeConditionItemHaveCount( const T_EXCHANGE_ITEM& stData );

	bool										DrawExchangeItemList( C2DRender* const p2DRender );
	bool										DrawExchangeItem_ConditionItemIcon( C2DRender* const p2DRender, const ExchangeItemMap& mapConditionItem, CPoint ptItem, const CRect rtItem );
	bool										DrawExchangeItem_ExchangeArrow( C2DRender* const p2DRender, CPoint ptItem, const CRect rtItem );
	bool										DrawExchangeItem_PaymentItemIcon( C2DRender* const p2DRender, const ExchangeItemMap& mapPaymentItem, CPoint ptItem, const CRect rtItem );
	bool										DrawExchangeItem_WidthLine( C2DRender* p2DRender, const UINT& nCount, const CRect& rtItem );
	bool										DrawExchangeItem_SelectBox( C2DRender* p2DRender, const UINT& nCount, CPoint ptItem, const CRect& rtItem );

	bool										DrawToolTip( void );


	bool										CheckPossibleMaxExchangeCount( void );



};