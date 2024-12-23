

#include "StdAfx.h"
#include "WndCommonInputCount.h"

//---------------------------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------------------------
CWndCommonInputCount::CWndCommonInputCount()
:	m_pWndEditCount( NULL )
,	m_bySlot( -1 )
,	m_nCount( NULL )
{
}


//---------------------------------------------------------------------------------------------
// �ı���
//---------------------------------------------------------------------------------------------
CWndCommonInputCount::~CWndCommonInputCount()
{
}


//---------------------------------------------------------------------------------------------
// �ʱ�ȭ
// param	: �θ� ������, ���� ������ ���̵�
// return	: BOOL
// ����		: �� ��ȯâ�� ���̽��� ����. ���� UI�� ����⿡ ���ҽ� ���� �� ������ �̷��� ���� �ִ���... (��_ ��)a
//---------------------------------------------------------------------------------------------
BOOL CWndCommonInputCount::Initialize( CWndBase* pWndParent, DWORD dwWndId )
{
	FLERROR_ASSERT_LOG_RETURN( pWndParent,	FALSE, _T("[���� ���� �Է� â] �θ� ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_TRADE_GOLD, WBS_MODAL, 0, pWndParent );
}


//---------------------------------------------------------------------------------------------
// ������Ʈ �ʱ�ȭ
// param	: ����
// return	: void
//---------------------------------------------------------------------------------------------
void CWndCommonInputCount::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 

	CWndButton* pWndButtonOk( static_cast<CWndButton*>( GetDlgItem( WIDC_OK ) ) );
	FLERROR_ASSERT_LOG_RETURN_VOID( pWndButtonOk, _T("[���� ���� �Է� â] ����Ʈ ��Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

	pWndButtonOk->SetDefault( TRUE );

	m_pWndEditCount = ( static_cast<CWndEdit*>( GetDlgItem( WIDC_EDIT ) ) );
	FLERROR_ASSERT_LOG_RETURN_VOID( m_pWndEditCount, _T("[���� ���� �Է� â] ����Ʈ ��Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

	m_pWndEditCount->SetString( _T("0") );	
	m_pWndEditCount->SetFocus();
	
	MoveParentCenter();
}  


//---------------------------------------------------------------------------------------------
// ��ȿ�� ���� ������ �����ΰ� ?
// param	: ���� ������ ����
// return	: �� / �ƴϿ�
//---------------------------------------------------------------------------------------------
bool CWndCommonInputCount::IsValidShortCutInfo()
{
	if( SHORTCUT_NONE == m_stShortcut.m_dwShortcut )
		return false;

	if( NULL == m_stShortcut.m_pFromWnd )
		return false;

	CWndBase* pWndFromWnd( m_stShortcut.m_pFromWnd->GetFrameWnd() );
	if( NULL == pWndFromWnd )
		return false;

	return true;
}


//---------------------------------------------------------------------------------------------
// ó�� ( �������� ���� ���� )
// param	: �޽���, ���̵�, ���
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndCommonInputCount::ProcessPrivateBank()
{
	if( false == IsValidShortCutInfo() )
		return FALSE;

	if( APP_COMMON_BANK != m_stShortcut.m_pFromWnd->GetFrameWnd()->GetWndId() )
		return FALSE;

	if( APP_INVENTORY_ITEM != m_pParentWnd->GetWndId() )
		return FALSE;

	CString strCount( m_pWndEditCount->GetString() );
	m_nCount = _ttoi( strCount );

	if( m_stShortcut.m_dwData == 0 ) // ��
	{
		if( m_nCount > static_cast<int>( g_pPlayer->m_dwGoldBank[m_bySlot] ) ) 
			m_nCount = g_pPlayer->m_dwGoldBank[m_bySlot];
	
		strCount.Format( _T("%d"), m_nCount );
		m_pWndEditCount->SetString( strCount );
	}
	else
	{
		FLItemBase* pItemBase( g_pPlayer->GetItemBankId( m_bySlot, m_stShortcut.m_dwId ) );
		if( NULL == pItemBase )
			return FALSE;
		
		if( m_nCount > ( (FLItemElem*)pItemBase )->m_nItemNum )
			m_nCount = ( (FLItemElem*)pItemBase )->m_nItemNum;

		strCount.Format( _T("%d"), m_nCount );
		m_pWndEditCount->SetString( strCount );
	}

	return TRUE;
}


//---------------------------------------------------------------------------------------------
// ó�� ( ��� �������� ���� ���� )
// param	: �޽���, ���̵�, ���
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndCommonInputCount::ProcessGuildBank()
{
	if( false == IsValidShortCutInfo() )
		return FALSE;

	if( APP_GUILD_BANK != m_stShortcut.m_pFromWnd->GetFrameWnd()->GetWndId() )
		return FALSE;

	if( APP_INVENTORY_ITEM != m_pParentWnd->GetWndId() )
		return FALSE;

	CString strCount( m_pWndEditCount->GetString() );
	m_nCount = _ttoi( strCount );

	if( m_stShortcut.m_dwData == 0 ) // ��
	{
		if( m_nCount > static_cast<int>( g_pPlayer->GetGuild()->m_nGoldGuild ) )
			m_nCount = g_pPlayer->GetGuild()->m_nGoldGuild;
	
		strCount.Format( _T("%d"), m_nCount );
		m_pWndEditCount->SetString( strCount );
	}
	else
	{
		FLItemBase* pItemBase( g_pPlayer->GetGuild()->m_GuildBank.GetAtId( m_stShortcut.m_dwId ) );
		if( NULL == pItemBase )
			return FALSE;
		
		if( m_nCount > ( (FLItemElem*)pItemBase )->m_nItemNum )
			m_nCount = ( (FLItemElem*)pItemBase )->m_nItemNum;

		strCount.Format( _T("%d"), m_nCount );
		m_pWndEditCount->SetString( strCount );
	}

	return TRUE;
}


//---------------------------------------------------------------------------------------------
// ó�� ( �޴� �������� ���� ���� )
// param	: �޽���, ���̵�, ���
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndCommonInputCount::ProcessMobileBag()
{
	if( false == IsValidShortCutInfo() )
		return FALSE;

	if( APP_GUILD_BANK != m_stShortcut.m_pFromWnd->GetFrameWnd()->GetWndId() )
		return FALSE;

	if( APP_INVENTORY_ITEM != m_pParentWnd->GetWndId() )
		return FALSE;

	CString strCount( m_pWndEditCount->GetString() );
	m_nCount = _ttoi( strCount );

	if( m_stShortcut.m_dwData == 0 ) // ��
		return FALSE;

	FLItemBase* pItemBase( g_pPlayer->GetGuild()->m_GuildBank.GetAtId( m_stShortcut.m_dwId ) );
	if( NULL == pItemBase )
		return FALSE;
	
	if( m_nCount > ( (FLItemElem*)pItemBase )->m_nItemNum )
		m_nCount = ( (FLItemElem*)pItemBase )->m_nItemNum;

	strCount.Format( _T("%d"), m_nCount );
	m_pWndEditCount->SetString( strCount );

	return TRUE;
}


//---------------------------------------------------------------------------------------------
// �޽��� ó�� ( �������� ���� ���� )
// param	: �޽���, ���̵�, ���
// return	: BOOL
//---------------------------------------------------------------------------------------------
bool CWndCommonInputCount::ProcessChildNotifyFromPrivateBank()
{
	if( false == IsValidShortCutInfo() )
		return false;

	if( -1 == m_bySlot )
		FLERROR_ASSERT_LOG_RETURN( NULL, false, _T("[���� ���� �Է� â] ���� �ε��� ������ ��ȿ���� �ʽ��ϴ�.") );

	if( APP_COMMON_BANK != m_stShortcut.m_pFromWnd->GetFrameWnd()->GetWndId() )
		return false;

	if( APP_INVENTORY_ITEM != m_pParentWnd->GetWndId() )
		return false;

	if( m_stShortcut.m_dwData == 0 )	// ��
	{
		if( m_nCount > 0 )
			g_DPlay.SendGetGoldBank( m_bySlot, m_nCount );
	}
	else								// ������
	{
		if( ITYPE_ITEM != m_stShortcut.m_dwType )
			FLERROR_ASSERT_LOG_RETURN( NULL, false, _T("[���� ���� �Է� â] ���� ������ ������ ������ Ÿ�� ������ ��ȿ���� �ʽ��ϴ�.") );

		g_DPlay.SendGetItemBank( m_bySlot, (BYTE)( m_stShortcut.m_dwId ), m_nCount );
	}

	return true;
}


//---------------------------------------------------------------------------------------------
// �޽��� ó�� ( ��� �������� ���� ���� )
// param	: �޽���, ���̵�, ���
// return	: BOOL
//---------------------------------------------------------------------------------------------
bool CWndCommonInputCount::ProcessChildNotifyFromGuildBank()
{
	if( false == IsValidShortCutInfo() )
		return false;

	if( APP_GUILD_BANK != m_stShortcut.m_pFromWnd->GetFrameWnd()->GetWndId() )
		return false;

	if( APP_INVENTORY_ITEM != m_pParentWnd->GetWndId() )
		return false;

	if( m_stShortcut.m_dwData == 0 )	// ��
	{
		if( m_nCount > 0 )
			g_DPlay.SendGetItemGuildBank( static_cast<BYTE>( m_stShortcut.m_dwId ), m_nCount, NULL );
	}
	else								// ������
	{
		if( ITYPE_ITEM != m_stShortcut.m_dwType )
			FLERROR_ASSERT_LOG_RETURN( NULL, false, _T("[���� ���� �Է� â] ���� ������ ������ ������ Ÿ�� ������ ��ȿ���� �ʽ��ϴ�.") );

		g_DPlay.SendGetItemGuildBank( static_cast<BYTE>( m_stShortcut.m_dwId ), m_nCount, 1 ); 
	}

	return true;
}


//---------------------------------------------------------------------------------------------
// �޽��� ó�� ( �޴� �������� ���� ���� )
// param	: �޽���, ���̵�, ���
// return	: BOOL
//---------------------------------------------------------------------------------------------
bool CWndCommonInputCount::ProcessChildNotifyFromMobileBag()
{
	if( false == IsValidShortCutInfo() )
		return false;

	if( APP_BAG_EX != m_stShortcut.m_pFromWnd->GetFrameWnd()->GetWndId() )
		return false;

	if( APP_INVENTORY_ITEM != m_pParentWnd->GetWndId() )
		return false;

	if( m_stShortcut.m_dwData == 0 )
		return false;

	if( ITYPE_ITEM != m_stShortcut.m_dwType )
		FLERROR_ASSERT_LOG_RETURN( NULL, false, _T("[���� ���� �Է� â] ���� ������ ������ ������ Ÿ�� ������ ��ȿ���� �ʽ��ϴ�.") );

		g_DPlay.SendMoveItem_Pocket( m_bySlot, m_stShortcut.m_dwId, m_nCount , -1 );

	return true;
}


//---------------------------------------------------------------------------------------------
// �ڽ����� ���� �޽��� ����
// param	: �޽���, ���̵�, ���
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndCommonInputCount::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( NULL == m_pWndEditCount )
		return CWndNeuz::OnChildNotify( message, nID, pLResult );
	
	if( false == IsValidShortCutInfo() )
		return CWndNeuz::OnChildNotify( message, nID, pLResult );

	if( nID == WIDC_OK || message == EN_RETURN )
	{
		FLERROR_ASSERT_LOG_RETURN( m_stShortcut.m_pFromWnd, FALSE, _T("[���� ���� �Է� â] ���� ������ �������� From �������� �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
		FLERROR_ASSERT_LOG_RETURN( m_stShortcut.m_pFromWnd->GetFrameWnd(), FALSE, _T("[���� ���� �Է� â] ���� ������ �������� From �������� �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
	
		ProcessChildNotifyFromPrivateBank();
		ProcessChildNotifyFromGuildBank();
		ProcessChildNotifyFromMobileBag();

		m_stShortcut.Empty();
		Destroy();
	}

	if( WIDC_EDIT == nID )
	{
		ProcessPrivateBank();
		ProcessGuildBank();
		ProcessMobileBag();
	}

	if( WIDC_CANCEL == nID )
		Destroy();

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 


//---------------------------------------------------------------------------------------------
// ��� �� ���� �����ϱ�
// param	: void
// return	: ���� / ����
//---------------------------------------------------------------------------------------------
void CWndCommonInputCount::SetInputCountInfo( const SHORTCUT* const pShortcut, const BYTE bySlot /*= 0*/ )
{
	FLERROR_ASSERT_LOG_RETURN_VOID( pShortcut, _T("[���� ���� �Է� â] ���� ������ ���� �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

	m_stShortcut	= (*pShortcut);
	m_bySlot		= bySlot;

	if( m_stShortcut.m_dwData == 0 ) // ��
	{
		SetTitle( prj.GetText( TID_MMI_BANK_SAVEMONEY ) );

		CWndStatic* pStaticDesc( (CWndStatic*)GetDlgItem( WIDC_STATIC ) );
		if( pStaticDesc )
			pStaticDesc->m_strTitle = prj.GetText( TID_GAME_MOVEPENYA );

		CWndStatic* pStaticCount( (CWndStatic*)GetDlgItem( WIDC_CONTROL1 ) );
		if( pStaticCount )
			pStaticCount->m_strTitle = prj.GetText( TID_GAME_PENYACOUNT );
	}
	else
	{
		SetTitle( prj.GetText( TID_MMI_BANK_SAVEITEM ) );

		CWndStatic* pStaticDesc( (CWndStatic*)GetDlgItem( WIDC_STATIC ) );
		if( pStaticDesc )
			pStaticDesc->m_strTitle = prj.GetText( TID_GAME_MOVECOUNT );

		CWndStatic* pStaticCount( (CWndStatic*)GetDlgItem( WIDC_CONTROL1 ) );
		if( pStaticCount )
			pStaticCount->m_strTitle = prj.GetText( TID_GAME_NUMCOUNT );
	}
}
