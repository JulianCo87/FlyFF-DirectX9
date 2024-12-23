#include "stdafx.h"
#include "WndExtra.h"

#if 0
CWndExtraCapture::CWndExtraCapture() 
{ 
} 
CWndExtraCapture::~CWndExtraCapture() 
{ 
} 
void CWndExtraCapture::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndExtraCapture::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndExtraCapture::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_EXTRA_CAPTURE, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  ���� ������ ���� ��� 
BOOL CWndExtraCapture::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndExtraCapture::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndExtraCapture::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndExtraCapture::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndExtraCapture::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndExtraCapture::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

#endif // if 0


//--------------------------------------------------------------------------------------------------
//CWndCoupon100722

CWndCoupon100722::CWndCoupon100722( ) :
m_pFont( NULL )
{
}

CWndCoupon100722::~CWndCoupon100722( )
{
}

void CWndCoupon100722::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndCoupon100722::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();

	CWndEdit* pWnd1	= (CWndEdit*)GetDlgItem( WIDC_EDIT1 );
	CWndEdit* pWnd2	= (CWndEdit*)GetDlgItem( WIDC_EDIT2 );
	CWndEdit* pWnd3 = (CWndEdit*)GetDlgItem( WIDC_EDIT3 );
	CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT1 );

	if( !( pWnd1 && pWnd2 && pWnd3 && pWndText ) )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "pWnd1 && pWnd2 && pWnd3 && pWndText" ) );
		Destroy();
	}

//	AddWndStyle( WBS_MODAL );
	AddWndStyle( WBS_TOPMOST );

	pWnd1->SetMaxStringNumber( 4 );
	pWnd1->SetTabStop( TRUE );
	pWnd1->AddWndStyle( WSS_ALIGNHCENTER );
	pWnd2->SetMaxStringNumber( 4 );
	pWnd2->SetTabStop( TRUE );
	pWnd2->AddWndStyle( WSS_ALIGNHCENTER );
    pWnd3->SetMaxStringNumber( 4 );
	pWnd3->SetTabStop( TRUE );
	pWnd3->AddWndStyle( WSS_ALIGNHCENTER );

	pWnd1->SetFocus();
	
	CD3DFont* pFont = m_Theme.GetFont( _T("gulim13" ) );
	if( pFont )
	{
		pWnd1->SetFont( pFont );
		pWnd2->SetFont( pFont );
		pWnd3->SetFont( pFont );
	}
	
	pWndText->SetString( prj.GetText( TID_MMI_EVENT_AROTTO_KOR_TEXT ) ); //"���� ã�ƿ��ٴ� [���ڿ� ID�ʿ�]" );
} 

// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndCoupon100722::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_COUPON_100722, WBS_KEY, CPoint( 0, 0 ), pWndParent );
} 

BOOL CWndCoupon100722::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( EN_CHANGE == message )
	{
		AutoFocus( );
	}

	if( EN_RETURN == message )
	{
		if( SendPacket() )
			Destroy();

		//return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
	}


	switch( nID )
	{
	case WIDC_BUTTONOK:
		if( SendPacket() )
			Destroy();
		break;

	case WIDC_BUTTONCANCEL:
		if(IsTextSize() == 0)
		{
			Destroy();
		}
		else 
        {
			g_WndMng.OpenMessageBox( _T( prj.GetText(TID_MMI_EVENT_AROTTO_KOR_COND_CLOSE) ) ,MB_OKCANCEL, this);	
		}
		break;
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

BOOL CWndCoupon100722::Process()
{
	CWndEdit* pWnd1	= (CWndEdit*)GetDlgItem( WIDC_EDIT1 );
	CWndEdit* pWnd2	= (CWndEdit*)GetDlgItem( WIDC_EDIT2 );
	CWndEdit* pWnd3	= (CWndEdit*)GetDlgItem( WIDC_EDIT3 );
	CWndButton* pBtnOK = (CWndButton*)GetDlgItem( WIDC_BUTTONOK );
	pBtnOK->EnableWindow( FALSE );

	// ��� ���� �Է¶��� ��� �ԷµǾ����� Ȯ�ι�ư Ȱ��ȭ 
	LPCTSTR pStr = pWnd1->GetString( );
	if( strlen( pStr ) == 4 )
	{
		pStr = pWnd2->GetString( );
		if( strlen( pStr ) == 4 )
		{
			pStr = pWnd3->GetString( );
			if( strlen( pStr ) == 4 )
				pBtnOK->EnableWindow( TRUE );
		}
	}

	//
	//LPCTSTR pStr = pWnd1->GetString();

	//if( strlen( pStr ) > 3 )
	//{
	//	pStr = pWnd2->GetString();
	//	if( strlen( pStr ) > 3 )
	//	{
	//		pStr = pWnd3->GetString();
	//		if( strlen( pStr ) > 3 )
	//		{
	//			CWndButton* pBtnOK = ( CWndButton* )GetDlgItem( WIDC_BUTTONOK );
	//			pBtnOK->SetFocus( );
	//		}
	//		else 
	//			pWnd3->SetFocus();
	//	}
	//	else
	//		pWnd2->SetFocus();
	//}
	//else 
	//	pWnd1->SetFocus();

	return TRUE;

}

void CWndCoupon100722::AutoFocus( )
{
	CWndEdit* pWnd1	= (CWndEdit*)GetDlgItem( WIDC_EDIT1 );
	CWndEdit* pWnd2	= (CWndEdit*)GetDlgItem( WIDC_EDIT2 );
	CWndEdit* pWnd3 = (CWndEdit*)GetDlgItem( WIDC_EDIT3 );

	LPCTSTR pStr = pWnd2->GetString();
	size_t len1 = strlen( pWnd1->GetString() );
	size_t len2 = strlen( pWnd2->GetString() );

	if( len2 == 4 )
	{
		if( len1 == 4 )
			pWnd3->SetFocus( );
	}
	else
	{
		if( len1 == 4 )
			pWnd2->SetFocus( );
	}
}

extern	CDPClient	g_DPlay;
int CWndCoupon100722::IsTextSize()
{
	CWndEdit* pWnd1	= (CWndEdit*)GetDlgItem( WIDC_EDIT1 );
	CWndEdit* pWnd2	= (CWndEdit*)GetDlgItem( WIDC_EDIT2 );
	CWndEdit* pWnd3 = (CWndEdit*)GetDlgItem( WIDC_EDIT3 );

	CString strCoupon = CString( pWnd1->GetString( ) ) + CString( pWnd2->GetString( ) ) + CString( pWnd3->GetString( ) );

	int nStrLength = strCoupon.GetLength();

	return nStrLength;
}
BOOL CWndCoupon100722::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase )
{
	switch(nID)
	{
	case IDOK:
		Destroy();
		break;
	case IDCANCEL:
		break;
	}

	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
}
BOOL CWndCoupon100722::SendPacket( )
{
	//���⼭ ������ȣ�� ������ ����
	CWndEdit* pWnd1	= (CWndEdit*)GetDlgItem( WIDC_EDIT1 );
	CWndEdit* pWnd2	= (CWndEdit*)GetDlgItem( WIDC_EDIT2 );
	CWndEdit* pWnd3 = (CWndEdit*)GetDlgItem( WIDC_EDIT3 );

	CString strCoupon = CString( pWnd1->GetString( ) ) + CString( pWnd2->GetString( ) ) + CString( pWnd3->GetString( ) );

	//���ڿ� �˻� 
	int nMax = strCoupon.GetLength();
	for( int i = 0; i < nMax; ++i )
	{
		TCHAR c = strCoupon.GetAt( i );
		if( FALSE == IsAlphaNum( c ) )
		{
			g_WndMng.OpenMessageBox( prj.GetText( TID_MMI_EVENT_AROTTO_KOR_COND ) ); //"���ڿ� ���ĺ����� �Է��ؾ� �մϴ�. �ٽ� �Է��ϼ���ID�ʿ�" );
			pWnd1->Empty();
			pWnd2->Empty();
			pWnd3->Empty();
			pWnd1->SetFocus();
			return FALSE;
		}
	}
	
	//'-'�߰�
	strCoupon = CString( pWnd1->GetString( ) ) + CString("-") + CString( pWnd2->GetString( ) ) + CString("-") + CString( pWnd3->GetString( ) );

	char* szCoupon = LPSTR(LPCTSTR(strCoupon));

	if( strlen( szCoupon ) != _MAX_COUPON_NUMBER_LEN )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "strlen( szCoupon ) != _MAX_COUPON_NUMBER_LEN" ) );
		return FALSE;
	}
    	
	//ok... let's send packet
	T_PACKET_COUPON_NUMBER kData = { 0 };
	//mem_set( &kData, 0, sizeof( kData ) );
	kData.dwPlayerId = g_pPlayer->m_idPlayer;
	FLStrcpy( kData.szCouponNumber, _countof( kData.szCouponNumber ), szCoupon );
	g_DPlay.SendCouponNumber( &kData );
	return TRUE;
}



