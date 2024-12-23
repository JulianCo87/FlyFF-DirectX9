
#include "StdAfx.h"

#include "WndFlyffPieceSystem.h"

// 18�� ������ ��¿ �� ���� ���� ��Ų��.
#include "../Neuz/DPClient.h"
#include "../_CommonDefine/Packet/FLPacketFlyffPieceEvent.h"
extern CDPClient		g_DPlay;


namespace WndFlyffPieceSystem
{
	static const BYTE		HEIGHT_OF_ONE_EXCHANGE_ITEM( 36 );
	static const BYTE		DRAW_WIDTHLINE_OFFSET_X( 1 );

	static const BYTE		EXCHANGE_ITEM_NAME_LIMIT_WIDTH( 150 );

	static const BYTE		EXCHANGE_ITEM_ICON_DRAWOFFSET_X( 2 );
	static const BYTE		EXCHANGE_ITEM_NAME_DRAWOFFSET_X( 10 );

	static const char		EXCHANGE_ITEM_VIEWPORT_OFFSET_TOP( 1 );
	static const char		EXCHANGE_ITEM_VIEWPORT_OFFSET_LEFT( 1 );
	static const char		EXCHANGE_ITEM_VIEWPORT_OFFSET_RIGHT( 1 );
	static const char		EXCHANGE_ITEM_VIEWPORT_OFFSET_BOTTOM( 3 );
};


//---------------------------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------------------------
CWndFlyffPieceSystem::CWndFlyffPieceSystem()
:	m_pExchangeItemKindList( NULL )
,	m_pExchangeItemList( NULL )
,	m_pExchangeButton( NULL )
,	m_pExchangeItemKindCtrl( NULL )
,	m_pExchangeItemCtrl( NULL )
,	m_pButtonItem( NULL )
,	m_pButtonLevel( NULL )
,	m_pButtonCoupon( NULL )
,	m_rtExchangeItemKindList( 0, 0, 0, 0 )
,	m_rtExchangeItemList( 0, 0, 0, 0 )
,	m_rtButtonItem( 0, 0, 0, 0 )
,	m_rtButtonLevel( 0, 0, 0, 0 )
,	m_rtButtonCoupon( 0, 0, 0, 0 )
,	m_pTexGuildImage( NULL )
,	m_pTexLoadingImage( NULL )
,	m_pFocusItem( NULL )
,	m_bClickedExchangeItemKind_Type1( true )
,	m_bSendItemListPacket( false )
,	m_bSendItemExchangePacket( false )
,	m_bAscendingOrderByName( FALSE )
,	m_bAscendingOrderByLevel( FALSE )
,	m_bAscendingOrderByNeedCouponCound( FALSE )
{
}


//---------------------------------------------------------------------------------------------
// �ı���
//---------------------------------------------------------------------------------------------
CWndFlyffPieceSystem::~CWndFlyffPieceSystem()
{
	AssDeleter< mapTreeDataContainer >() ( m_mapTreeData );

	m_vecExchangeItem.clear();
	m_vecIconRect.clear();
}


//---------------------------------------------------------------------------------------------
// �ʱ�ȭ
// param	: �θ� ������, ���� ������ ���̵�
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndFlyffPieceSystem::Initialize( CWndBase* pWndParent, DWORD dwWndId )
{
	FLERROR_ASSERT_LOG_RETURN( pWndParent,	FALSE, _T("[������ ���� �ý���] �θ� ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
	FLERROR_ASSERT_LOG_RETURN( dwWndId,		FALSE, _T("[������ ���� �ý���] ���� ������ ���̵� ��ȿ���� �ʽ��ϴ�.") );

	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), dwWndId, 0, CPoint( 0, 0 ), pWndParent );
}


//---------------------------------------------------------------------------------------------
// ������Ʈ �ʱ�ȭ
// param	: ����
// return	: void
//---------------------------------------------------------------------------------------------
void CWndFlyffPieceSystem::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 

	m_pExchangeItemKindCtrl = GetWndCtrl( WIDC_TREECTRL1 );
	FLERROR_ASSERT_LOG_RETURN_VOID( m_pExchangeItemKindCtrl, _T("[������ ���� �ý���] ��ȯ ������ ���� ��Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

	m_pExchangeItemCtrl = GetWndCtrl( WIDC_LISTBOX2 );
	FLERROR_ASSERT_LOG_RETURN_VOID( m_pExchangeItemCtrl, _T("[������ ���� �ý���] ��ȯ ������ ����Ʈ ��Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�."))

	m_pButtonItem = GetWndCtrl( WIDC_BUTTON1 );
	FLERROR_ASSERT_LOG_RETURN_VOID( m_pButtonItem, _T("[������ ���� �ý���] ������ ��ư ��Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�."))

	m_pButtonLevel = GetWndCtrl( WIDC_BUTTON2 );
	FLERROR_ASSERT_LOG_RETURN_VOID( m_pButtonLevel, _T("[������ ���� �ý���] ���� ��ư ��Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�."))

	m_pButtonCoupon = GetWndCtrl( WIDC_BUTTON3 );
	FLERROR_ASSERT_LOG_RETURN_VOID( m_pButtonCoupon, _T("[������ ���� �ý���] ���� ��ư ��Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�."))

	m_pExchangeItemKindList	= (CWndTreeCtrl*)GetDlgItem( WIDC_TREECTRL1 );
	FLERROR_ASSERT_LOG_RETURN_VOID( m_pExchangeItemKindList, _T("[������ ���� �ý���] ��ȯ ������ ���� ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�."))

	m_pExchangeItemList = (CWndListBox*)GetDlgItem( WIDC_LISTBOX2 );
	FLERROR_ASSERT_LOG_RETURN_VOID( m_pExchangeItemList, _T("[������ ���� �ý���] ��ȯ ������ ����Ʈ ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�."))

	m_pExchangeButton = (CWndButton*)GetDlgItem( WIDC_BUTTON6 );
	FLERROR_ASSERT_LOG_RETURN_VOID( m_pExchangeButton, _T("[������ ���� �ý���] ��ȯ ������ ����Ʈ ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�."))

	m_pExchangeButton->EnableWindow( FALSE );

	m_rtExchangeItemKindList	= m_pExchangeItemKindCtrl->rect;
	m_rtExchangeItemList		= m_pExchangeItemCtrl->rect;
	m_rtButtonItem				= m_pButtonItem->rect;
	m_rtButtonLevel				= m_pButtonLevel->rect;
	m_rtButtonCoupon			= m_pButtonCoupon->rect;

	m_pTexGuildImage			= m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), _T("WndFlyffCouponGuideImage.tga") ), 0, TRUE );
	m_pTexLoadingImage			= m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), _T("WndFlyffBox.tga") ), 0, TRUE );

	if( m_pExchangeItemList )
	{
		m_pExchangeItemList->m_bOnItem = TRUE;
		m_pExchangeItemList->m_nLineSpace = (int)( ( HEIGHT_OF_ONE_EXCHANGE_ITEM - m_pExchangeItemList->GetFontHeight( ) ) * 0.5f );
	}

	CreateExchangeItemKindList();
	MoveParentCenter();
}  


//---------------------------------------------------------------------------------------------
// ������ ���� Ʈ�� �����
// param	: void
// return	: ���� / ����
//---------------------------------------------------------------------------------------------
bool CWndFlyffPieceSystem::CreateExchangeItemKindList()
{
	FLERROR_ASSERT_LOG_RETURN( m_pExchangeItemKindList, false, _T("[������ ���� �ý���] Ʈ�� ��Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

	m_pExchangeItemKindList->DeleteAllItems( );

	if( FALSE == ParsingScript( _T("propFlyffpiece.inc") ) )
		FLERROR_ASSERT_LOG_RETURN( FALSE, false, _T("[������ ���� �ý���] ��ũ��Ʈ �б� ����.") );

	// �θ� ���� ������Ʈ�� �ɷ�����
	mapTreeDataContainer mapCopyTreeData = m_mapTreeData;	

	for( mapTreeDataIter iter = mapCopyTreeData.begin(); iter != mapCopyTreeData.end();  /*none*/ )
	{
		TREEDATA* pData = iter->second;
		FLERROR_ASSERT_LOG_DO( pData, continue, _T("[������ ���� �ý���] Ʈ�� ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

		if( NULL_ID == pData->_dwParentKey ) //������Ʈ
		{
			m_pExchangeItemKindList->InsertItem( NULL, pData->_strTitle, pData->_dwKey );
			mapCopyTreeData.erase( iter++ );
			continue;
		}

		++iter;
	}

	// ���ϵ� ���
	BOOL bRecursive = TRUE;
	while( bRecursive )
	{
		int nSafeWhile = 0;
		for( mapTreeDataIter iter = mapCopyTreeData.begin(); iter != mapCopyTreeData.end();  /*none*/ )
		{
			TREEDATA* pData = iter->second;
			FLERROR_ASSERT_LOG_DO( pData, continue, _T("[������ ���� �ý���] Ʈ�� ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

			if( NULL_ID == pData->_dwParentKey ) //��Ʈ�� �̹� ���ŵǾ��־�� �Ѵ�.
			{
				FLASSERT( 0 );
				continue;
			}

			LPTREEELEM pElem = m_pExchangeItemKindList->FindTreeElem( pData->_dwParentKey );	//�θ� ã�Ƽ� ����
			if( pElem )
			{
				m_pExchangeItemKindList->InsertItem( pElem, pData->_strTitle, pData->_dwKey );
				mapCopyTreeData.erase( iter++ );
				continue;
			}

			++iter;
		}

		++nSafeWhile;

		if( nSafeWhile > 10 || mapCopyTreeData.empty() )		// 10ȸ�̻� ��ȯ�ϰų� �� ������ ����
			bRecursive = FALSE;
	}

	FLASSERT( mapCopyTreeData.empty() );
	return true;
}


//---------------------------------------------------------------------------------------------
// ��ũ��Ʈ ���� �м�
// param	: void
// return	: ���� / ����
//---------------------------------------------------------------------------------------------
bool CWndFlyffPieceSystem::ParsingScript( const char* szFileName )
{
	CScript script;
	if( FALSE == script.Load( _T( szFileName ) ) )
		FLERROR_ASSERT_LOG_RETURN( FALSE, false, _T("[������ ���� �ý���] ��ũ��Ʈ ���� �б� ���� - [ %s ]."), szFileName );

	if( !m_mapTreeData.empty( ) )
		AssDeleter< mapTreeDataContainer >()( m_mapTreeData );
	
	DWORD dwKey = script.GetNumber(); // ID;

	UINT nCount( 0 );	
	while( script.tok != FINISHED )
	{
		TREEDATA* pData = new WndFlyffPieceSystem::TREEDATA;
		FLERROR_ASSERT_LOG_DO( pData, continue, _T("[������ ���� �ý���] Ʈ�� ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

		pData->_dwKey = dwKey; 
		FLERROR_ASSERT_LOG_DO( pData->_dwKey, continue, _T("[������ ���� �ý���] Ű ���� 0 �Դϴ�!!") );
	
		int nBlock( 0 );
	
		do
		{
			script.GetToken();
			if( script.Token == _T( "{" ) )
			{
				++nBlock;
			}
			else if( script.Token == _T( "}" ) )
			{
				--nBlock;
			}
			else if( script.Token == _T( "parent" ) )
			{	
				script.GetToken(); // (
				pData->_dwParentKey = script.GetNumber( );
				script.GetToken(); // )
				script.GetToken(); // ;
			}
			else if( script.Token == _T( "title" ) )
			{
				script.GetToken(); // (
				CString strTemp = prj.GetLangScript( script );
				BOOL bOK = script.GetStringByStringKey( std::string( strTemp.GetString() ), strTemp );
				if( bOK )
					pData->_strTitle = strTemp;
			}
			else if( script.Token == _T( "type1" ) )
			{
				script.GetToken(); // (
				pData->_dwType[0] = script.GetNumber( );
				script.GetToken(); // )
				script.GetToken(); // ;
			}
			else if( script.Token == _T( "type2" ) )
			{
				script.GetToken(); // (
				pData->_dwType[1] = script.GetNumber( );
				script.GetToken(); // )
				script.GetToken(); // ;
			}
		}while( nBlock > 0 && script.tok != FINISHED );

		if( pData->_dwType[0] && pData->_dwType[1] )
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "[������ ���� �ý���] ��ũ��Ʈ�� Ʈ�� �����Ϳ� �ϳ� �̻��� Ÿ���� �����մϴ�.\n Ÿ���� �Ѱ��� ������ �� �ֽ��ϴ�." ) );
			FLERROR_LOG( PROGRAM_NAME, _T( "[������ ���� �ý���] [%s] ��ũ��Ʈ�� Ȯ���ϼ���.\n Title - [%s]." ), szFileName, pData->_strTitle );
			SAFE_DELETE( pData );
			FLASSERT( 0 );
		}

		if( NULL == pData->_dwType[0] && NULL == pData->_dwType[1] )
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "[������ ���� �ý���] ��ũ��Ʈ�� Ʈ�� �����Ϳ� Ÿ���� �������� �ʽ��ϴ�." ) );
			FLERROR_LOG( PROGRAM_NAME, _T( "[������ ���� �ý���] [%s] ��ũ��Ʈ�� Ȯ���ϼ���.\n Title - [%s]." ), szFileName, pData->_strTitle );
			SAFE_DELETE( pData );
			FLASSERT( 0 );
		}

		if( pData )
		{
			pair< mapTreeDataIter, bool > rst = m_mapTreeData.insert( mapTreeDataContainer::value_type( pData->_dwKey, pData ) );
			if( false == rst.second )
			{
				FLERROR_LOG( PROGRAM_NAME, "[������ ���� �ý���] ���̵� �ߺ��Դϴ�. ���� ����! ID - [%u], Title - [%s]", pData->_dwKey, pData->_strTitle );
				FLASSERT( 0 );
				SAFE_DELETE( pData );
			}
		}

		dwKey = script.GetNumber(); // ID;
	}

	return TRUE;
}


//---------------------------------------------------------------------------------------------
// ó��
// param	: void
// return	: ���� / ����
//---------------------------------------------------------------------------------------------
BOOL CWndFlyffPieceSystem::Process()
{
	return TRUE;
}


//---------------------------------------------------------------------------------------------
// �޽��� �ڽ��� ��� �� ���ڿ��� ���´�.
// param	: void
// return	: ��� �޽���
//---------------------------------------------------------------------------------------------
CString CWndFlyffPieceSystem::GetExchangeItemMessageBoxText()
{
	CString strMessage( _T("") );

	T_EXCHANGE_ITEM_DATA stData;

	BOOL bIsFind( FALSE );
	for( UINT nCount = 0; nCount < m_vecExchangeItem.size(); nCount++ )
	{
		if( m_vecExchangeItem[nCount].dwIndex == m_pFocusItem->m_dwData )
		{
			stData	= m_vecExchangeItem[nCount];
			bIsFind	= TRUE;
			break;
		}
	}
		
	FLERROR_ASSERT_LOG_RETURN( bIsFind, strMessage, _T("[������ ���� �ý���] �������� ã�� �� �����ϴ�.") );

	PT_ITEM_SPEC pPayItemProp = g_xSpecManager->GetSpecItem( stData.kPayItem.dwItemID );
	FLERROR_ASSERT_LOG_RETURN( pPayItemProp, strMessage, _T("[������ ���� �ý���] ������ �Ӽ��� ��� �� �� �����ϴ�. INDEX - [%u]"), stData.dwIndex );

	PT_ITEM_SPEC pCondItemProp = g_xSpecManager->GetSpecItem( stData.kCondItem.dwItemID );
	FLERROR_ASSERT_LOG_RETURN( pCondItemProp, strMessage, _T("[������ ���� �ý���] ��ȯ �ʿ� ������ �Ӽ��� ��� �� �� �����ϴ�. INDEX - [%u]"), stData.dwIndex );

	strMessage.Format( prj.GetText( TID_MMI_FLYFFPIECE_TEXT01 ), pPayItemProp->szName, pCondItemProp->szName, stData.kCondItem.nQuantity );

	return strMessage;
}

//---------------------------------------------------------------------------------------------
// ����� ���� ���� �̺�Ʈ ó�� ( ��ȯ ��ư )
// param	: ������ ���̵�
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndFlyffPieceSystem::Process_ExchangeButtonClicked( UINT nID )
{
	if( m_vecExchangeItem.empty() ||
		NULL == m_pExchangeItemList->GetCount() ||
		NULL == m_pFocusItem )
		return FALSE;

	if( WIDC_BUTTON6 == nID )
	{
		CString strMessage( GetExchangeItemMessageBoxText() );

		if( FALSE == g_WndMng.OpenMessageBox( strMessage, MB_OKCANCEL, this ) )
		{
			FLERROR_ASSERT_LOG_RETURN( NULL, FALSE, _T("[������ ������ ��Ʈ��] ������ ��ȯ ���� Ȯ�� �޽��� �ڽ� ���� ����.") );
		}
		else
		{
			g_WndMng.GetMessageBox()->AddWndStyle( WBS_MODAL );
			g_WndMng.GetMessageBox()->AddWndStyle( WBS_TOPMOST );
		}
	}
	
	return TRUE;
}


//---------------------------------------------------------------------------------------------
// ����� ���� ���� �̺�Ʈ ó�� ( ��� ��ư )
// param	: ������ ���̵�
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndFlyffPieceSystem::Process_CancelButtonClicked( UINT nID )
{
	if( WIDC_BUTTON7 == nID )
	{
		Destroy();
		return TRUE;
	}

	return FALSE;
}


//---------------------------------------------------------------------------------------------
// ����� ���� ���� �̺�Ʈ ó�� ( �޽��� �ڽ� ��ư )
// param	: ������ ���̵�, ������ ������
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndFlyffPieceSystem::Process_MessageBoxButtonClicked( UINT nID, CWndBase* pWndBase )
{
	if( m_vecExchangeItem.empty() ||
		NULL == m_pExchangeItemList->GetCount() ||
		NULL == m_pFocusItem )
		return FALSE;

	if( pWndBase )
	{
		if( pWndBase == g_WndMng.GetMessageBox() )
		{
			if( IDOK == nID )
				SendExchangeItemPacket( m_pFocusItem->m_dwData );
		}
	}

	return TRUE;
}


//---------------------------------------------------------------------------------------------
// ����� ���� ���� �̺�Ʈ ó�� ( ������ ��ư )
// param	: ������ ���̵�
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndFlyffPieceSystem::Process_ItemButtonClicked( UINT nID )
{
	if( m_vecExchangeItem.empty() ||
		NULL == m_pExchangeItemList->GetCount() )
		return FALSE;

	if( WIDC_BUTTON1 == nID )
	{
		m_bAscendingOrderByLevel			= FALSE;
		m_bAscendingOrderByNeedCouponCound	= FALSE;

		m_pFocusItem						= NULL;
	
		if( m_pExchangeItemList )
			m_pExchangeItemList->EnableWindow( FALSE );

		m_bAscendingOrderByName = m_bAscendingOrderByName ? 0 : 1;

		if( m_bAscendingOrderByName )
			std::sort( m_vecExchangeItem.begin(), m_vecExchangeItem.end(), SortItemListByName_Ascending );
		else
			std::sort( m_vecExchangeItem.begin(), m_vecExchangeItem.end(), SortItemListByName_Descending );

		CreateExchangeItemList();

		if( m_pExchangeItemList )
			m_pExchangeItemList->EnableWindow( TRUE );

	}

	return TRUE;
}


//---------------------------------------------------------------------------------------------
// ����� ���� ���� �̺�Ʈ ó�� ( ���� ��ư )
// param	: ������ ���̵�
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndFlyffPieceSystem::Process_LevelButtonClicked( UINT nID )
{
	if( m_vecExchangeItem.empty() ||
		NULL == m_pExchangeItemList->GetCount() )
		return FALSE;

	if( WIDC_BUTTON2 == nID )
	{
		m_bAscendingOrderByName				= FALSE;
		m_bAscendingOrderByNeedCouponCound	= FALSE;
		
		m_pFocusItem						= NULL;
	
		if( m_pExchangeItemList )
			m_pExchangeItemList->EnableWindow( FALSE );

		m_bAscendingOrderByLevel = m_bAscendingOrderByLevel ? 0 : 1;

		if( m_bAscendingOrderByLevel )
			std::sort( m_vecExchangeItem.begin(), m_vecExchangeItem.end(), SortItemListByLevel_Ascending );
		else
			std::sort( m_vecExchangeItem.begin(), m_vecExchangeItem.end(), SortItemListByLevel_Descending );

		CreateExchangeItemList();

		if( m_pExchangeItemList )
			m_pExchangeItemList->EnableWindow( TRUE );

	}

	return TRUE;
}

//---------------------------------------------------------------------------------------------
// ����� ���� ���� �̺�Ʈ ó�� ( �ʿ� ���� ���� ��ư )
// param	: ������ ���̵�
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndFlyffPieceSystem::Process_NeedCouponCountButtonClicked( UINT nID )
{
	if( m_vecExchangeItem.empty() ||
		NULL == m_pExchangeItemList->GetCount() )
		return FALSE;

	if( WIDC_BUTTON4 == nID )
	{
		m_bAscendingOrderByName		= FALSE;
		m_bAscendingOrderByLevel	= FALSE;

		m_pFocusItem				= NULL;
	
		if( m_pExchangeItemList )
			m_pExchangeItemList->EnableWindow( FALSE );

		m_bAscendingOrderByNeedCouponCound = m_bAscendingOrderByNeedCouponCound ? 0 : 1;

		if( m_bAscendingOrderByNeedCouponCound )
			std::sort( m_vecExchangeItem.begin(), m_vecExchangeItem.end(), SortItemListByNeedCouponCount_Ascending );
		else
			std::sort( m_vecExchangeItem.begin(), m_vecExchangeItem.end(), SortItemListByNeedCouponCount_Descending );

		CreateExchangeItemList();

		if( m_pExchangeItemList )
			m_pExchangeItemList->EnableWindow( TRUE );

	}

	return TRUE;
}

//---------------------------------------------------------------------------------------------
// ����� ���� ���� �̺�Ʈ ó��
// param	: ������ ���̵�, �޽���, �⺻ ������
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndFlyffPieceSystem::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase )
{
	if( Process_CancelButtonClicked( nID ) )
		return TRUE;

	Process_ExchangeButtonClicked( nID );
	Process_MessageBoxButtonClicked( nID, pWndBase );
	Process_ItemButtonClicked( nID );
	Process_LevelButtonClicked( nID );
	Process_NeedCouponCountButtonClicked( nID );

	return CWndBase::OnCommand( nID, dwMessage, pWndBase );
}
 

//---------------------------------------------------------------------------------------------
// �ڽ����� ���� �޽��� ����
// param	: �޽���, ���̵�, ���
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndFlyffPieceSystem::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( WIDC_TREECTRL1 == nID )
	{
		if( message == WNM_CLICKED )
		{
			FLERROR_ASSERT_LOG_RETURN( pLResult, FALSE, _T("[������ ���� �ý���] ��� �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

			tagScriptElem* pFocusElem = (tagScriptElem*)( pLResult );
			FLERROR_ASSERT_LOG_RETURN( pFocusElem, FALSE, _T("[������ ���� �ý���] ���� �� Ʈ�� ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

			SendPacket_ItemListReq( pFocusElem->m_dwData );
		}
	}

	if( WIDC_LISTBOX2 == nID )
	{
		if( message == WNM_SELCHANGE )
		{
			FLERROR_ASSERT_LOG_RETURN( pLResult, FALSE, _T("[������ ���� �ý���] ��� �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

			m_pFocusItem = (CWndListBox::LPLISTITEM)( pLResult );
			FLERROR_ASSERT_LOG_RETURN( m_pFocusItem, FALSE, _T("[������ ���� �ý���] ���� �� ����Ʈ �ڽ� ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
		}
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 


//---------------------------------------------------------------------------------------------
// ������ ��ϵ� ��Ŷ �����ϱ�
// param	: Ʈ�� ������ Ű ��
// return	: ���� / ����
//---------------------------------------------------------------------------------------------
bool CWndFlyffPieceSystem::SendPacket_ItemListReq( DWORD dwTreeKey )
{
	mapTreeDataIter Iter = m_mapTreeData.find( dwTreeKey );
	if( Iter != m_mapTreeData.end() )
	{
		TREEDATA* pData = Iter->second;
		FLERROR_ASSERT_LOG_RETURN( pData, false, _T("[������ ���� �ý���] Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
		
		if( 0 != pData->_dwType[0] )		// �� �з�( Type1 )�� Ŭ���ߴ�.
		{
			m_bClickedExchangeItemKind_Type1 = true;	
			return true;
		}
		else								// �� �з�( Type2 )�� Ŭ���ߴ�.
			m_bClickedExchangeItemKind_Type1 = false;	

		if( false == m_bSendItemListPacket )
		{
			FLPacketFlyffPieceExchangeListReq	kPacket;

			kPacket.dwMainCategory	= pData->_dwType[0];
			kPacket.dwSubCategory	= pData->_dwType[1];

			g_DPlay.SendPacket( &kPacket );

			m_bSendItemListPacket = true;

			if( m_pExchangeItemKindList )
				m_pExchangeItemKindList->EnableWindow( FALSE );
		}
	}
	else
		FLERROR_ASSERT_LOG_RETURN( NULL, false, _T("[������ ���� �ý���] Ʈ���� �����Ͱ� �������� �ʽ��ϴ�. ID - [ %u ]"), dwTreeKey );

	return true;
}


//---------------------------------------------------------------------------------------------
// ������ ����Ʈ �����ϱ�
// param	: ������ ����Ʈ
// return	: void
//---------------------------------------------------------------------------------------------
void CWndFlyffPieceSystem::SetListData( const vecExchangeItem vecItemData )
{
	m_pFocusItem = NULL;

	m_vecExchangeItem.clear();
	m_vecExchangeItem = vecItemData;

	CreateExchangeItemList();

	m_bSendItemListPacket = false;

	if( m_pExchangeItemKindList )
		m_pExchangeItemKindList->EnableWindow( TRUE );

	m_bAscendingOrderByName				= false;
	m_bAscendingOrderByLevel			= false;
	m_bAscendingOrderByNeedCouponCound	= false;
}


//---------------------------------------------------------------------------------------------
// ������ ����Ʈ �����ϱ�
// param	: ������ ����Ʈ
// return	: void
//---------------------------------------------------------------------------------------------
bool CWndFlyffPieceSystem::CreateExchangeItemList()
{
	FLERROR_ASSERT_LOG_RETURN( m_pExchangeItemList, false, _T("[������ ���� �ý���] ��ȯ ������ ����Ʈ ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

	m_pExchangeItemList->ResetContent();
	
	for( UINT nCount = 0; nCount < 	m_vecExchangeItem.size(); nCount++ )
	{
		m_pExchangeItemList->AddString( _T("") );
		m_pExchangeItemList->SetItemData( nCount, m_vecExchangeItem[nCount].dwIndex );
	}
	return true;
}


//---------------------------------------------------------------------------------------------
// �׸���
// param	: ������
// return	: void
//---------------------------------------------------------------------------------------------
void CWndFlyffPieceSystem::OnDraw( C2DRender* p2DRender )
{
	if( NULL == p2DRender )
		return ;
	
	if( m_bClickedExchangeItemKind_Type1 )
	{
		DrawGuideImage( p2DRender );
		DrawGuideText( p2DRender );
		return ;
	}	

	if( m_bSendItemListPacket )
	{
		DrawLoadingImage( p2DRender );
		DrawLoadingText( p2DRender );
		return ;
	}

	DrawExchangeItem( p2DRender );
	DrawTooltip();
}


//---------------------------------------------------------------------------------------------
// ���� �̹��� �׸���
// param	: ������
// return	: void
//---------------------------------------------------------------------------------------------
void CWndFlyffPieceSystem::DrawGuideImage( C2DRender* p2DRender )
{
	if( NULL == m_pTexGuildImage )
		return ;

	CPoint ptImageLeftTop( m_rtExchangeItemList.TopLeft() );

	ptImageLeftTop.x += static_cast<long>( ( m_rtExchangeItemList.Width() - m_pExchangeItemList->GetScrollWidth() - m_pTexGuildImage->m_size.cx ) * 0.5f );
	ptImageLeftTop.y += static_cast<long>( ( m_rtExchangeItemList.Height() - m_pTexGuildImage->m_size.cy ) * 0.5f );
	
	p2DRender->RenderTexture( ptImageLeftTop, m_pTexGuildImage );
}


//---------------------------------------------------------------------------------------------
// ���� �۾� �׸���
// param	: ������
// return	: void
// �߰�����	: �۾��� ���ٷ� �ϸ� Parsing �Ͽ� ������ ���̸� �˾Ƴ� �� ���⿡ ��ȹ����
//			: �����Ͽ� �������� ���ڿ��� ����ϱ�� �Ͽ����ϴ�.
//---------------------------------------------------------------------------------------------
void CWndFlyffPieceSystem::DrawGuideText( C2DRender* p2DRender )
{
	if( NULL == m_pFont )
		return ;

	PFONTCOLOR_WNDFLYFFPIECESYSTEM pFontColorWndFlyffPieceSystem = g_WndFontColorManager->GetWndFlyffPieceSystem();
	DWORD dwTextColor = pFontColorWndFlyffPieceSystem->m_stGuildText.GetFontColor();
	
	SIZE TextSize_1st = m_pFont->GetTextExtent( prj.GetText( TID_MMI_FLYFFCOUPONSHOP_TEXT01 ) );
	SIZE TextSize_2nd = m_pFont->GetTextExtent( prj.GetText( TID_MMI_FLYFFCOUPONSHOP_TEXT02 ) );
	SIZE TextSize_3rd = m_pFont->GetTextExtent( prj.GetText( TID_MMI_FLYFFCOUPONSHOP_TEXT03 ) );

	CPoint ptTextLeftTop_1st( m_rtExchangeItemList.TopLeft() );

	ptTextLeftTop_1st.x += static_cast<long>( ( m_rtExchangeItemList.Width() - m_pExchangeItemList->GetScrollWidth() - TextSize_1st.cx ) * 0.5f );
	ptTextLeftTop_1st.y += static_cast<long>( ( m_rtExchangeItemList.Height() - TextSize_1st.cy ) * 0.5f );
	ptTextLeftTop_1st.y -= TextSize_1st.cy;

	p2DRender->TextOut( ptTextLeftTop_1st.x, ptTextLeftTop_1st.y, prj.GetText( TID_MMI_FLYFFCOUPONSHOP_TEXT01 ), dwTextColor );


	CPoint ptTextLeftTop_2nd( m_rtExchangeItemList.TopLeft() );

	ptTextLeftTop_2nd.x += static_cast<long>( ( m_rtExchangeItemList.Width() - m_pExchangeItemList->GetScrollWidth() - TextSize_2nd.cx ) * 0.5f );
	ptTextLeftTop_2nd.y = ptTextLeftTop_1st.y + TextSize_2nd.cy; 

	p2DRender->TextOut( ptTextLeftTop_2nd.x, ptTextLeftTop_2nd.y, prj.GetText( TID_MMI_FLYFFCOUPONSHOP_TEXT02 ), dwTextColor );


	CPoint ptTextLeftTop_3rd( m_rtExchangeItemList.TopLeft() );

	ptTextLeftTop_3rd.x += static_cast<long>( ( m_rtExchangeItemList.Width() - m_pExchangeItemList->GetScrollWidth() - TextSize_3rd.cx ) * 0.5f );
	ptTextLeftTop_3rd.y = ptTextLeftTop_2nd.y + TextSize_3rd.cy; 

	p2DRender->TextOut( ptTextLeftTop_3rd.x, ptTextLeftTop_3rd.y, prj.GetText( TID_MMI_FLYFFCOUPONSHOP_TEXT03 ), dwTextColor );
}


//---------------------------------------------------------------------------------------------
// �ε� �̹��� �׸���
// param	: ������
// return	: void
//---------------------------------------------------------------------------------------------
void CWndFlyffPieceSystem::DrawLoadingImage( C2DRender* p2DRender )
{
	if( NULL == m_pTexLoadingImage )
		return ;

	CPoint ptImageLeftTop( m_rtExchangeItemList.TopLeft() );

	ptImageLeftTop.x += static_cast<long>( ( m_rtExchangeItemList.Width() - m_pExchangeItemList->GetScrollWidth() - m_pTexLoadingImage->m_size.cx ) * 0.5f );
	ptImageLeftTop.y += static_cast<long>( ( m_rtExchangeItemList.Height() - m_pTexLoadingImage->m_size.cy ) * 0.5f );
	
	p2DRender->RenderTexture( ptImageLeftTop, m_pTexLoadingImage );
}


//---------------------------------------------------------------------------------------------
// �ε� �۾� �׸���
// param	: ������
// return	: void
//---------------------------------------------------------------------------------------------
void CWndFlyffPieceSystem::DrawLoadingText( C2DRender* p2DRender )
{
	if( NULL == m_pFont )
		return ;

	PFONTCOLOR_WNDFLYFFPIECESYSTEM pFontColorWndFlyffPieceSystem = g_WndFontColorManager->GetWndFlyffPieceSystem();
	DWORD dwTextColor = pFontColorWndFlyffPieceSystem->m_stLoadingText.GetFontColor();
	
	SIZE TextSize = m_pFont->GetTextExtent( prj.GetText( TID_MMI_FLYFFCOUPONSHOP_LOADING ) );

	CPoint ptImageLeftTop( m_rtExchangeItemList.TopLeft() );

	ptImageLeftTop.x += static_cast<long>( ( m_rtExchangeItemList.Width() - m_pExchangeItemList->GetScrollWidth() - m_pTexLoadingImage->m_size.cx ) * 0.5f );
	ptImageLeftTop.y += static_cast<long>( ( m_rtExchangeItemList.Height() - m_pTexLoadingImage->m_size.cy ) * 0.5f );
	
	CPoint ptTextLeftTop( ptImageLeftTop );

	ptTextLeftTop.x += static_cast<long>( ( m_pTexLoadingImage->m_size.cx - TextSize.cx ) * 0.5f );
	ptTextLeftTop.y += static_cast<long>( ( m_pTexLoadingImage->m_size.cy - TextSize.cy ) * 0.5f );

	p2DRender->TextOut( ptTextLeftTop.x, ptTextLeftTop.y, prj.GetText( TID_MMI_FLYFFCOUPONSHOP_LOADING ), dwTextColor );
}



//---------------------------------------------------------------------------------------------
// �� ��Ʈ ����
// param	: ������
// return	: void
// �߰�����	: ����Ʈ �ڽ� �ȿ��� �׸��� �������� �׸��� ���ؼ� �� ��Ʈ�� Ŭ���� ������ ���� �����Ѵ�.
//---------------------------------------------------------------------------------------------
void CWndFlyffPieceSystem::SetViewPort( C2DRender* p2DRender )
{
	CRect	rtViewport( m_pExchangeItemList->m_rectWindow );

	rtViewport.left		+= EXCHANGE_ITEM_VIEWPORT_OFFSET_LEFT;
	rtViewport.right	-= m_pExchangeItemList->GetScrollWidth() + EXCHANGE_ITEM_VIEWPORT_OFFSET_RIGHT;
	rtViewport.top		+= EXCHANGE_ITEM_VIEWPORT_OFFSET_TOP;
	rtViewport.bottom	-= EXCHANGE_ITEM_VIEWPORT_OFFSET_BOTTOM;

	ClientToScreen( &rtViewport );

	p2DRender->SetViewport( rtViewport );
}


//---------------------------------------------------------------------------------------------
// ������ �׸���
// param	: ������
// return	: void
//---------------------------------------------------------------------------------------------
void CWndFlyffPieceSystem::DrawExchangeItem( C2DRender* p2DRender )
{
	if( NULL == m_pExchangeItemList )
		return ;

	if(	m_vecExchangeItem.empty() )
	{
		DrawGuideImage( p2DRender );
		DrawItemListEmptyText( p2DRender );
		return ;
	}

	m_vecIconRect.clear();

	SetViewPort( p2DRender );

	CRect	rtRect( m_pExchangeItemList->m_rectWindow );
	int		nSelected( m_pExchangeItemList->GetCurSel() );

	for( UINT nCount = 0; nCount < m_vecExchangeItem.size(); nCount++ )
	{
		T_EXCHANGE_ITEM_DATA stData = m_vecExchangeItem[nCount];
		
		CRect	rtItem( m_pExchangeItemList->GetItemRect( nCount ) );
		CPoint	ptItem( rtItem.TopLeft() + m_pExchangeItemList->m_rectWindow.TopLeft() );

		PT_ITEM_SPEC pPayItemProp = g_xSpecManager->GetSpecItem( stData.kPayItem.dwItemID );
		FLERROR_ASSERT_LOG_DO( pPayItemProp, continue, _T("[������ ���� �ý���] ��ȯ ������ �Ӽ��� ��� �� �� �����ϴ�. COUNT - [%u], INDEX - [%u]"), nCount, stData.dwIndex );

		PT_ITEM_SPEC pCondItemProp = g_xSpecManager->GetSpecItem( stData.kCondItem.dwItemID );
		FLERROR_ASSERT_LOG_DO( pCondItemProp, continue, _T("[������ ���� �ý���] ��ȯ �ʿ� ������ �Ӽ��� ��� �� �� �����ϴ�. COUNT - [%u], INDEX - [%u]"), nCount, stData.dwIndex );

		DrawExchangeItem_Icon( p2DRender, pPayItemProp, ptItem, rtItem );
		DrawExchangeItem_Name( p2DRender, pPayItemProp, ptItem, rtItem );
		DrawExchangeItem_Level( p2DRender, pPayItemProp, ptItem, rtItem );
		DrawExchangeItem_CouponIcon( p2DRender, pCondItemProp, ptItem, rtItem );
		DrawExchangeItem_CouponCount( p2DRender, nCount, ptItem, rtItem );
		DrawExchangeItem_WidthLine( p2DRender, nCount, rtItem );
		DrawExchangeItem_SelectBox( p2DRender, stData.dwIndex, ptItem, rtItem );
	}

	DrawExchangeItem_HeightLine( p2DRender );
}


//---------------------------------------------------------------------------------------------
// ���� �׸���
// param	: ������
// return	: void
//---------------------------------------------------------------------------------------------
void CWndFlyffPieceSystem::DrawTooltip()
{
	if( m_vecExchangeItem.empty() )
		return;

	if( m_pWndOnMouseMove != m_pExchangeItemList )
		return;

	CPoint	ptMouse( GetMousePoint() );
	CRect	rtListBox( m_pExchangeItemList->GetClientRect( TRUE ) );
	CRect	rtIcon( 0, 0, 0, 0 );

	if( FALSE == rtListBox.PtInRect( ptMouse ) )
		return ;

	DWORD dwIndex( NULL );

	for( UINT nCount = 0; nCount < m_vecIconRect.size(); nCount++ )
	{
		if( m_vecIconRect[nCount].PtInRect( ptMouse ) ) 
		{
			dwIndex = m_vecExchangeItem[nCount].kPayItem.dwItemID;
			rtIcon	= m_vecIconRect[nCount];
			break;
		}
	}

	if( NULL == dwIndex )
		return ;

	FLItemElem ItemElem;	
	ItemElem.m_dwItemId = dwIndex;

	CPoint	ptTemp = ptMouse;
	CRect	rtTemp = rtIcon;

	ClientToScreen( &ptTemp );
	ClientToScreen( &rtTemp );

	g_WndMng.PutToolTip_Item( &ItemElem, ptTemp, &rtTemp, -1 );

}


//---------------------------------------------------------------------------------------------
// ������ ����Ʈ�� ��� �ִٴ� �۾� �׸���
// param	: ������
// return	: void
// �߰�����	: �۾��� ���ٷ� �ϸ� Parsing �Ͽ� ������ ���̸� �˾Ƴ� �� ���⿡ ��ȹ����
//			: �����Ͽ� �������� ���ڿ��� ����ϱ�� �Ͽ����ϴ�.
//---------------------------------------------------------------------------------------------
void CWndFlyffPieceSystem::DrawItemListEmptyText( C2DRender* p2DRender )
{
	if( NULL == m_pFont )
		return ;

	PFONTCOLOR_WNDFLYFFPIECESYSTEM pFontColorWndFlyffPieceSystem = g_WndFontColorManager->GetWndFlyffPieceSystem();
	DWORD dwTextColor = pFontColorWndFlyffPieceSystem->m_stItemListEmptyText.GetFontColor();
	
	SIZE TextSize_1st = m_pFont->GetTextExtent( prj.GetText( TID_MMI_FLYFFCOUPONSHOP_TEXT04 ) );
	SIZE TextSize_2nd = m_pFont->GetTextExtent( prj.GetText( TID_MMI_FLYFFCOUPONSHOP_TEXT05 ) );

	CPoint ptTextLeftTop_1st( m_rtExchangeItemList.TopLeft() );

	ptTextLeftTop_1st.x += static_cast<long>( ( m_rtExchangeItemList.Width() - m_pExchangeItemList->GetScrollWidth() - TextSize_1st.cx ) * 0.5f );
	ptTextLeftTop_1st.y += static_cast<long>( ( m_rtExchangeItemList.Height() - TextSize_1st.cy ) * 0.5f );
	ptTextLeftTop_1st.y -= TextSize_1st.cy;

	p2DRender->TextOut( ptTextLeftTop_1st.x, ptTextLeftTop_1st.y, prj.GetText( TID_MMI_FLYFFCOUPONSHOP_TEXT04 ), dwTextColor );

	CPoint ptTextLeftTop_2nd( m_rtExchangeItemList.TopLeft() );

	ptTextLeftTop_2nd.x += static_cast<long>( ( m_rtExchangeItemList.Width() - m_pExchangeItemList->GetScrollWidth() - TextSize_2nd.cx ) * 0.5f );
	ptTextLeftTop_2nd.y = ptTextLeftTop_1st.y + ( TextSize_2nd.cy * 2 ); 

	p2DRender->TextOut( ptTextLeftTop_2nd.x, ptTextLeftTop_2nd.y, prj.GetText( TID_MMI_FLYFFCOUPONSHOP_TEXT05 ), dwTextColor );
}


//---------------------------------------------------------------------------------------------
// ������ �׸��� (������)
// param	: ������
// return	: void
//---------------------------------------------------------------------------------------------
void CWndFlyffPieceSystem::DrawExchangeItem_Icon( C2DRender* p2DRender, const PT_ITEM_SPEC pProp, CPoint ptItem, const CRect rtItem )
{
	CTexture* pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, pProp->szIcon ), WNDCOLOR_DEFAULT_KEY );
	if( pTexture )
	{
		ptItem.x += EXCHANGE_ITEM_ICON_DRAWOFFSET_X;
		ptItem.y += static_cast<long>( (rtItem.Height() - pTexture->m_size.cy) * 0.5f );

		p2DRender->RenderTexture( ptItem, pTexture );

		CRect rtIcon( ptItem.x, ptItem.y, ptItem.x + pTexture->m_size.cx, ptItem.y + pTexture->m_size.cy ); 

		m_vecIconRect.push_back( rtIcon );
	}
}


//---------------------------------------------------------------------------------------------
// ������ �׸��� (�̸�)
// param	: ������
// return	: void
//---------------------------------------------------------------------------------------------
void CWndFlyffPieceSystem::DrawExchangeItem_Name( C2DRender* p2DRender, const PT_ITEM_SPEC pProp, CPoint ptItem, const CRect rtItem )
{
	CTexture* pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, pProp->szIcon ), WNDCOLOR_DEFAULT_KEY );

	if( NULL == pTexture )
		return ;

	if( NULL == m_pFont )
		return ;

	CString strName( _T("") );
	strName.Format( "%s", pProp->szName );

	SIZE	TextSize( m_pFont->GetTextExtent( strName.GetString() ) );

	int	nCalculationLeftPos =	ptItem.x + 
								EXCHANGE_ITEM_ICON_DRAWOFFSET_X + 
								pTexture->m_size.cx + 
								EXCHANGE_ITEM_NAME_DRAWOFFSET_X - 
								WndListBox::TEXTOUT_OFFSET;

	bool	bIsCutString( false );
	int		nTextWidth( TextSize.cx );
	
	while( nTextWidth > EXCHANGE_ITEM_NAME_LIMIT_WIDTH )
	{
		bIsCutString = true;

		if( IsDBCSLeadByte( strName[ strName.GetLength() - 2 ] ) )
		{
			strName = strName.Left( strName.GetLength() - 2 );
			nTextWidth = m_pFont->GetTextExtent( strName.GetString() ).cx;
		}
		else
		{
			strName = strName.Left( strName.GetLength() - 1 );
			nTextWidth = m_pFont->GetTextExtent( strName.GetString() ).cx;
		}

	}

	if( bIsCutString )
		strName.Append( _T("...") );

	CPoint	ptLeftTop( nCalculationLeftPos, ptItem.y );
	ptLeftTop.y += static_cast<long>( ( rtItem.Height() - TextSize.cy ) * 0.5f );

	p2DRender->TextOut( ptLeftTop.x, ptLeftTop.y, strName.GetString() );
}


//---------------------------------------------------------------------------------------------
// ������ �׸��� (����)
// param	: ������
// return	: void
//---------------------------------------------------------------------------------------------
void CWndFlyffPieceSystem::DrawExchangeItem_Level( C2DRender* p2DRender, const PT_ITEM_SPEC pProp, CPoint ptItem, const CRect rtItem )
{
	if( NULL == m_pFont )
		return ;

	CString strLevel( _T("") );
	strLevel.Format( "%d", pProp->dwLimitLevel1 );

	SIZE	TextSize( m_pFont->GetTextExtent( strLevel.GetString() ) );
	CPoint	ptLeftTop( ptItem.x + m_rtButtonItem.Width() - WndListBox::TEXTOUT_OFFSET + DRAW_WIDTHLINE_OFFSET_X, ptItem.y );

	ptLeftTop.x += static_cast<long>( ( m_rtButtonLevel.Width() - TextSize.cx ) * 0.5f );
	ptLeftTop.y += static_cast<long>( ( rtItem.Height() - TextSize.cy ) * 0.5f );

	p2DRender->TextOut( ptLeftTop.x, ptLeftTop.y, strLevel.GetString(), 0xFFFFFFFF );
}


//---------------------------------------------------------------------------------------------
// ������ �׸��� (����)
// param	: ������
// return	: void
//---------------------------------------------------------------------------------------------
void CWndFlyffPieceSystem::DrawExchangeItem_CouponIcon( C2DRender* p2DRender, const PT_ITEM_SPEC pProp, CPoint ptItem, const CRect rtItem )
{
	CTexture* pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, pProp->szIcon ), WNDCOLOR_DEFAULT_KEY );
	if( pTexture )
	{
		int	nCalculationLeftPos =	ptItem.x + 
									m_rtButtonItem.Width() + 
									m_rtButtonLevel.Width() +
									DRAW_WIDTHLINE_OFFSET_X + 
									DRAW_WIDTHLINE_OFFSET_X -
									WndListBox::TEXTOUT_OFFSET;
		
		CPoint ptLeftTop( nCalculationLeftPos, ptItem.y );

		ptLeftTop.x += static_cast<long>( ( m_rtButtonCoupon.Width() - pTexture->m_size.cx ) * 0.5f );
		ptLeftTop.y += static_cast<long>( ( rtItem.Height() - pTexture->m_size.cy ) * 0.5f );

		p2DRender->RenderTexture( ptLeftTop, pTexture );
	}
}


//---------------------------------------------------------------------------------------------
// ������ �׸��� (���� �ʿ䰳��)
// param	: ������
// return	: void
//---------------------------------------------------------------------------------------------
void CWndFlyffPieceSystem::DrawExchangeItem_CouponCount( C2DRender* p2DRender, const UINT& nCount, CPoint ptItem, const CRect rtItem )
{
	if( NULL == m_pFont )
		return ;

	T_EXCHANGE_ITEM_DATA stData = m_vecExchangeItem[nCount];

	CString	strCouponCount( _T("") );
	strCouponCount.Format( _T( "%d" ), stData.kCondItem.nQuantity );

	SIZE	TextSize( m_pFont->GetTextExtent( strCouponCount.GetString() ) );
	int		nCalculationLeftPos =	ptItem.x + 
									m_rtButtonItem.Width() + 
									m_rtButtonLevel.Width() + DRAW_WIDTHLINE_OFFSET_X + 
									m_rtButtonCoupon.Width() + DRAW_WIDTHLINE_OFFSET_X -
									WndListBox::TEXTOUT_OFFSET;

	CPoint	ptLeftTop( nCalculationLeftPos, ptItem.y );

	ptLeftTop.x += static_cast<long>( ( m_rtButtonCoupon.Width() - TextSize.cx ) * 0.5f );
	ptLeftTop.y += static_cast<long>( ( rtItem.Height() - TextSize.cy ) * 0.5f );

	p2DRender->TextOut( ptLeftTop.x, ptLeftTop.y, strCouponCount.GetString(), 0xFFFFFFFF );
}


//---------------------------------------------------------------------------------------------
// ������ �׸��� (����)
// param	: ������
// return	: void
//---------------------------------------------------------------------------------------------
void CWndFlyffPieceSystem::DrawExchangeItem_SelectBox( C2DRender* p2DRender, DWORD dwExchangeItemDataIndex, CPoint ptItem, const CRect rtItem )
{
	if( NULL == m_pFocusItem )
	{
		if( m_pExchangeButton )
			m_pExchangeButton->EnableWindow( FALSE );

		return ;
	}

	if( m_pFocusItem->m_dwData != dwExchangeItemDataIndex )
		return ;

	if( m_pExchangeButton )
		m_pExchangeButton->EnableWindow( TRUE );

	PFLCOLOR_WNDFLYFFPIECESYSTEM pFLColorWndFlyffPieceSystem = g_FLColorManager->GetWndFlyffPieceSystem();

	DWORD dwRectColor( pFLColorWndFlyffPieceSystem->m_stListBoxSelectRect.GetColor() );
	DWORD dwFillRectColor( pFLColorWndFlyffPieceSystem->m_stListBoxSelectFillRect.GetColor() );

	CRect rtRound( ptItem.x - WndListBox::TEXTOUT_OFFSET, ptItem.y, ptItem.x - WndListBox::TEXTOUT_OFFSET + rtItem.Width(), ptItem.y + rtItem.Height() + 1 );	// �� 1�ȼ��̸������� �Ѥ�;;
	rtRound.DeflateRect( 2, 2 );

	p2DRender->RenderRect( rtRound, dwRectColor );

	rtRound.DeflateRect( 1, 1 );
	p2DRender->RenderFillRect( rtRound, dwFillRectColor );
}


//---------------------------------------------------------------------------------------------
// ������ �׸��� (���� ��)
// param	: ������
// return	: void
//---------------------------------------------------------------------------------------------
void CWndFlyffPieceSystem::DrawExchangeItem_WidthLine( C2DRender* p2DRender, const UINT& nCount, const CRect& rtItem )
{
	PFLCOLOR_WNDFLYFFPIECESYSTEM pFLColorWndFlyffPieceSystem = g_FLColorManager->GetWndFlyffPieceSystem();
	DWORD dwLineColor = pFLColorWndFlyffPieceSystem->m_stListBoxLineColor.GetColor();

	CPoint ptItemLeftTop = m_pExchangeItemList->m_rectWindow.TopLeft(); 
	ptItemLeftTop.y += ( nCount * rtItem.Height() ) + WndListBox::TEXTOUT_OFFSET;

	p2DRender->RenderLine(	CPoint( ptItemLeftTop.x, ptItemLeftTop.y + rtItem.Height() ), 
							CPoint( ptItemLeftTop.x + rtItem.Width(), ptItemLeftTop.y + rtItem.Height() ),
							dwLineColor, dwLineColor );
}


//---------------------------------------------------------------------------------------------
// ������ �׸��� (���� ��)
// param	: ������
// return	: void
//---------------------------------------------------------------------------------------------
void CWndFlyffPieceSystem::DrawExchangeItem_HeightLine( C2DRender* p2DRender )
{
	PFLCOLOR_WNDFLYFFPIECESYSTEM pFLColorWndFlyffPieceSystem = g_FLColorManager->GetWndFlyffPieceSystem();
	DWORD dwLineColor = pFLColorWndFlyffPieceSystem->m_stListBoxLineColor.GetColor();

	CPoint ptListBoxLeftTop = m_pExchangeItemList->m_rectWindow.TopLeft(); 

	CPoint ptButtonItemTop( ptListBoxLeftTop.x + m_rtButtonItem.Width(), ptListBoxLeftTop.y );
	CPoint ptButtonItemBottom( ptListBoxLeftTop.x + m_rtButtonItem.Width(), ptListBoxLeftTop.y + m_pExchangeItemList->m_rectWindow.Height() );

	p2DRender->RenderLine(	ptButtonItemTop, ptButtonItemBottom, dwLineColor, dwLineColor );

	CPoint ptButtonLevelTop( ptButtonItemTop.x + m_rtButtonLevel.Width() + DRAW_WIDTHLINE_OFFSET_X, ptListBoxLeftTop.y );
	CPoint ptButtonLevelBottom( ptButtonItemTop.x + m_rtButtonLevel.Width() + DRAW_WIDTHLINE_OFFSET_X, ptListBoxLeftTop.y + m_pExchangeItemList->m_rectWindow.Height() );

	p2DRender->RenderLine(	ptButtonLevelTop, ptButtonLevelBottom, dwLineColor, dwLineColor );

	CPoint ptButtonCouponTop( ptButtonLevelTop.x + m_rtButtonCoupon.Width() + DRAW_WIDTHLINE_OFFSET_X, ptListBoxLeftTop.y );
	CPoint ptButtonCouponBottom( ptButtonLevelTop.x + m_rtButtonCoupon.Width() + DRAW_WIDTHLINE_OFFSET_X, ptListBoxLeftTop.y + m_pExchangeItemList->m_rectWindow.Height() );

	p2DRender->RenderLine(	ptButtonCouponTop, ptButtonCouponBottom, dwLineColor, dwLineColor );
}


//---------------------------------------------------------------------------------------------
// ������ ��ȯ ��Ŷ �����ϱ�
// param	: ������ ����Ʈ �� �ε���
// return	: void
//---------------------------------------------------------------------------------------------
void CWndFlyffPieceSystem::SendExchangeItemPacket( const DWORD dwItemIndex )
{
	if( false == m_bSendItemExchangePacket )
	{
		FLPacketExchangeFlyffPieceItemReq	kPacket;

		kPacket.dwIndex = dwItemIndex;

		g_DPlay.SendPacket( &kPacket );

		m_bSendItemExchangePacket = true;
	}
}


//---------------------------------------------------------------------------------------------
// ������ ����Ʈ ���� : �̸� (���� ����)
// param	: �� ������ ���� ����, ������
// return	: bool
//---------------------------------------------------------------------------------------------
bool CWndFlyffPieceSystem::SortItemListByName_Ascending( const T_EXCHANGE_ITEM_DATA& rLeft, const T_EXCHANGE_ITEM_DATA& rRight )
{
	PT_ITEM_SPEC pLeftItemProp	= g_xSpecManager->GetSpecItem( rLeft.kPayItem.dwItemID );
	PT_ITEM_SPEC pRightItemProp = g_xSpecManager->GetSpecItem( rRight.kPayItem.dwItemID );

	FLERROR_ASSERT_LOG_RETURN( pLeftItemProp, false, _T("[������ ���� �ý���] ������ �Ӽ� ��� ���� (����)") );
	FLERROR_ASSERT_LOG_RETURN( pRightItemProp, false, _T("[������ ���� �ý���] ������ �Ӽ� ��� ���� (������)") );

	CString strLeft( pLeftItemProp->szName );
	CString strRight( pRightItemProp->szName );

	if( strLeft < strRight )
		return true;

	return false;
}


//---------------------------------------------------------------------------------------------
// ������ ����Ʈ ���� : �̸� (���� ����)
// param	: �� ������ ���� ����, ������
// return	: bool
//---------------------------------------------------------------------------------------------
bool CWndFlyffPieceSystem::SortItemListByName_Descending( const T_EXCHANGE_ITEM_DATA& rLeft, const T_EXCHANGE_ITEM_DATA& rRight )
{
	PT_ITEM_SPEC pLeftItemProp	= g_xSpecManager->GetSpecItem( rLeft.kPayItem.dwItemID );
	PT_ITEM_SPEC pRightItemProp = g_xSpecManager->GetSpecItem( rRight.kPayItem.dwItemID );

	FLERROR_ASSERT_LOG_RETURN( pLeftItemProp, false, _T("[������ ���� �ý���] ������ �Ӽ� ��� ���� (����)") );
	FLERROR_ASSERT_LOG_RETURN( pRightItemProp, false, _T("[������ ���� �ý���] ������ �Ӽ� ��� ���� (������)") );

	CString strLeft( pLeftItemProp->szName );
	CString strRight( pRightItemProp->szName );

	if( strLeft > strRight )
		return true;

	return false;
}


//---------------------------------------------------------------------------------------------
// ������ ����Ʈ ���� : �ʿ� ���� (���� ����)
// param	: �� ������ ���� ����, ������
// return	: bool
//---------------------------------------------------------------------------------------------
bool CWndFlyffPieceSystem::SortItemListByLevel_Ascending( const T_EXCHANGE_ITEM_DATA& rLeft, const T_EXCHANGE_ITEM_DATA& rRight )
{
	PT_ITEM_SPEC pLeftItemProp	= g_xSpecManager->GetSpecItem( rLeft.kPayItem.dwItemID );
	PT_ITEM_SPEC pRightItemProp = g_xSpecManager->GetSpecItem( rRight.kPayItem.dwItemID );

	FLERROR_ASSERT_LOG_RETURN( pLeftItemProp, false, _T("[������ ���� �ý���] ������ �Ӽ� ��� ���� (����)") );
	FLERROR_ASSERT_LOG_RETURN( pRightItemProp, false, _T("[������ ���� �ý���] ������ �Ӽ� ��� ���� (������)") );

	if( pLeftItemProp->dwLimitLevel1 < pRightItemProp->dwLimitLevel1 )
		return true;

	return false;
}


//---------------------------------------------------------------------------------------------
// ������ ����Ʈ ���� : �ʿ� ���� (���� ����)
// param	: �� ������ ���� ����, ������
// return	: bool
//---------------------------------------------------------------------------------------------
bool CWndFlyffPieceSystem::SortItemListByLevel_Descending( const T_EXCHANGE_ITEM_DATA& rLeft, const T_EXCHANGE_ITEM_DATA& rRight )
{
	PT_ITEM_SPEC pLeftItemProp	= g_xSpecManager->GetSpecItem( rLeft.kPayItem.dwItemID );
	PT_ITEM_SPEC pRightItemProp = g_xSpecManager->GetSpecItem( rRight.kPayItem.dwItemID );

	FLERROR_ASSERT_LOG_RETURN( pLeftItemProp, false, _T("[������ ���� �ý���] ������ �Ӽ� ��� ���� (����)") );
	FLERROR_ASSERT_LOG_RETURN( pRightItemProp, false, _T("[������ ���� �ý���] ������ �Ӽ� ��� ���� (������)") );

	if( pLeftItemProp->dwLimitLevel1 > pRightItemProp->dwLimitLevel1 )
		return true;

	return false;
}



//---------------------------------------------------------------------------------------------
// ������ ����Ʈ ���� : �ʿ� ���� ���� (���� ����)
// param	: �� ������ ���� ����, ������
// return	: bool
//---------------------------------------------------------------------------------------------
bool CWndFlyffPieceSystem::SortItemListByNeedCouponCount_Ascending( const T_EXCHANGE_ITEM_DATA& rLeft, const T_EXCHANGE_ITEM_DATA& rRight )
{
	if( rLeft.kCondItem.nQuantity < rRight.kCondItem.nQuantity )
		return true;

	return false;
}


//---------------------------------------------------------------------------------------------
// ������ ����Ʈ ���� : �ʿ� ���� ���� (���� ����)
// param	: �� ������ ���� ����, ������
// return	: bool
//---------------------------------------------------------------------------------------------
bool CWndFlyffPieceSystem::SortItemListByNeedCouponCount_Descending( const T_EXCHANGE_ITEM_DATA& rLeft, const T_EXCHANGE_ITEM_DATA& rRight )
{
	if( rLeft.kCondItem.nQuantity > rRight.kCondItem.nQuantity )
		return true;

	return false;
}