

#include "StdAfx.h"
#include "WndCommonItemExchangeConfirm.h"

//---------------------------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------------------------
CWndCommonItemExchangeConfirm::CWndCommonItemExchangeConfirm()
:	m_pWndEditCount( NULL )
,	m_dwMMIDefineIndex( -1 )
,	m_nSelectedIndex( -1 )
,	m_nPossibleMaxExchangeCount( NULL )
{
}


//---------------------------------------------------------------------------------------------
// �ı���
//---------------------------------------------------------------------------------------------
CWndCommonItemExchangeConfirm::~CWndCommonItemExchangeConfirm()
{
}


//---------------------------------------------------------------------------------------------
// �ʱ�ȭ
// param	: �θ� ������, ���� ������ ���̵�
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndCommonItemExchangeConfirm::Initialize( CWndBase* pWndParent, DWORD dwWndId )
{
	FLERROR_ASSERT_LOG_RETURN( pWndParent,	FALSE, _T("[���� ������ ��ȯ â(������ ��ȯ ���� �Է�)] �θ� ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_TRADE_GOLD, WBS_MODAL, 0, pWndParent );
}


//---------------------------------------------------------------------------------------------
// ������Ʈ �ʱ�ȭ
// param	: ����
// return	: void
//---------------------------------------------------------------------------------------------
void CWndCommonItemExchangeConfirm::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 

	CWndButton* pWndButtonOk( static_cast<CWndButton*>( GetDlgItem( WIDC_OK ) ) );
	FLERROR_ASSERT_LOG_RETURN_VOID( pWndButtonOk, _T("[���� ������ ��ȯ â(������ ��ȯ ���� �Է�)] ����Ʈ ��Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

	pWndButtonOk->SetDefault( TRUE );

	m_pWndEditCount = ( static_cast<CWndEdit*>( GetDlgItem( WIDC_EDIT ) ) );
	FLERROR_ASSERT_LOG_RETURN_VOID( m_pWndEditCount, _T("[���� ������ ��ȯ â(������ ��ȯ ���� �Է�)] ����Ʈ ��Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

	m_pWndEditCount->SetString( _T("0") );	
	m_pWndEditCount->SetFocus();
	
	MoveParentCenter();
}  


//---------------------------------------------------------------------------------------------
// ������ ��ȯ ���� ����
// param	: MMI ���� �ε���
// return	: �� / �ƴϿ�
//---------------------------------------------------------------------------------------------
bool CWndCommonItemExchangeConfirm::SetItemExchangeInfo(const DWORD dwMMIDefineIndex, const int nSelectedIndex, const int PossibleMaxExchangeCount)
{
	if( -1 == dwMMIDefineIndex )
		FLERROR_ASSERT_LOG_RETURN( NULL, false, _T("[���� ������ ��ȯ â(������ ��ȯ ���� �Է�)] MMI �ε��� ������ �ùٸ��� �ʽ��ϴ�.") );

	if( -1 == nSelectedIndex )
		FLERROR_ASSERT_LOG_RETURN( NULL, false, _T("[���� ������ ��ȯ â(������ ��ȯ ���� �Է�)] ���� �ε��� ������ �ùٸ��� �ʽ��ϴ�.") );

	if( 1 > PossibleMaxExchangeCount )
		FLERROR_ASSERT_LOG_RETURN( NULL, false, _T("[���� ������ ��ȯ â(������ ��ȯ ���� �Է�)] ��ȯ ���� ������ �ùٸ��� �ʽ��ϴ�.") );

	m_dwMMIDefineIndex			= dwMMIDefineIndex;
	m_nSelectedIndex			= nSelectedIndex;
	m_nPossibleMaxExchangeCount = PossibleMaxExchangeCount;
	
	SetTitle( prj.GetText( TID_MMI_EXCHANGEAMOUNT_TITLE ) );

	CWndStatic* pStaticDesc( (CWndStatic*)GetDlgItem( WIDC_STATIC ) );
	if( pStaticDesc )
		pStaticDesc->m_strTitle = prj.GetText( TID_MMI_EXCHANGEAMOUNT_TEXT );

	CWndStatic* pStaticCount( (CWndStatic*)GetDlgItem( WIDC_CONTROL1 ) );
	if( pStaticCount )
		pStaticCount->m_strTitle = prj.GetText( TID_GAME_NUMCOUNT );

	return true;
}


//---------------------------------------------------------------------------------------------
// ������ ���� �Է� ó��
// param	: void
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndCommonItemExchangeConfirm::ProcessInputCount()
{
	CString strCount( m_pWndEditCount->GetString() );
	int nCount = _ttoi( strCount );

	if( nCount > m_nPossibleMaxExchangeCount )
		nCount = m_nPossibleMaxExchangeCount;

	strCount.Format( _T("%d"), nCount );
	m_pWndEditCount->SetString( strCount );

	return TRUE;
}


//---------------------------------------------------------------------------------------------
// �ڽ����� ���� �޽��� ����
// param	: �޽���, ���̵�, ���
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndCommonItemExchangeConfirm::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( NULL == m_pWndEditCount )
		return CWndNeuz::OnChildNotify( message, nID, pLResult );
	
	if( nID == WIDC_OK || message == EN_RETURN )
	{
		if( m_pParentWnd &&
			APP_DIALOG_EVENT == m_pParentWnd->GetWndId() )
		{
			CString strCount( m_pWndEditCount->GetString() );
			int nCount = _ttoi( strCount );

			if( NULL < nCount )
			{
				FLPacketItemExchangeReq	kPacket;

				kPacket.dwMenuID		= m_dwMMIDefineIndex;
				kPacket.dwListNth		= m_nSelectedIndex;
				kPacket.nExchangeCount	= nCount;

				g_DPlay.SendPacket( &kPacket );

				CWndCommonItemExchange* pWndCommonItemExchange( static_cast<CWndCommonItemExchange*>( m_pParentWnd ) );
				if( pWndCommonItemExchange )
					pWndCommonItemExchange->SetSendPacket();		
			}
		}
		
		Destroy();
	}

	if( WIDC_EDIT == nID )
	{
		ProcessInputCount();
	}

	if( WIDC_CANCEL == nID )
		Destroy();

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 
