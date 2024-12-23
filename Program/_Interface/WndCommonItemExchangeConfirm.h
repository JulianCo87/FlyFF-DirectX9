
#pragma once 

#include "../_CommonDefine/Packet/FLPacketItemExchange.h"

//---------------------------------------------------------------------------------------------
// ���� ������ ��ȯ ������ ������ ��ȯ ���� �Է� â
//---------------------------------------------------------------------------------------------
class CWndCommonItemExchangeConfirm	:	public CWndNeuz
{

public:

												CWndCommonItemExchangeConfirm( void );
	virtual										~CWndCommonItemExchangeConfirm( void );

	virtual BOOL 								Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);

	virtual	void 								OnInitialUpdate( void );
	virtual BOOL								OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 

	bool										SetItemExchangeInfo( const DWORD dwMMIDefineIndex, const int nSelectedIndex, const int PossibleMaxExchangeCount );


private:


	BOOL										ProcessInputCount( void );


private:


	CWndEdit*									m_pWndEditCount;

	DWORD										m_dwMMIDefineIndex;
	int											m_nSelectedIndex;
	int											m_nPossibleMaxExchangeCount;

};
