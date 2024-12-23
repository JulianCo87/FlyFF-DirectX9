
#include "StdAfx.h"
#include "langman.h"


//---------------------------------------------------------------------------------------------
// ������ 
//---------------------------------------------------------------------------------------------
CLangMan::CLangMan()
{
}


//---------------------------------------------------------------------------------------------
// �ı��� 
//---------------------------------------------------------------------------------------------
CLangMan::~CLangMan()
{
	for( ItorLanguageData Itor = m_mapLanguageData.begin(); Itor != m_mapLanguageData.end(); ++Itor )
	{
		PLANG_DATA pLangData = Itor->second;
		FLERROR_ASSERT_LOG_DO( pLangData, continue, _T("[��� �Ŵ���] ��� ���� �����Ͱ� ��ȿ���� �ʽ��ϴ�. ���� ����") );	
		
		SAFE_DELETE( pLangData );
	}

	m_mapLanguageData.clear();
}


//---------------------------------------------------------------------------------------------
// Load ( �ҷ����� )
// param	: lpszFileName ( ���� : proplang.txt )
// return	: BOOL 
//---------------------------------------------------------------------------------------------
BOOL CLangMan::Load( LPCTSTR lpszFileName )
{
	FLERROR_ASSERT_LOG_RETURN( lpszFileName, FALSE, _T("[��� �Ŵ���] ������ �ҷ� �� �� �����ϴ�.") );	

	CScanner s;
	if( FALSE == s.Load( lpszFileName, TRUE ) ) 
		FLERROR_ASSERT_LOG_RETURN( lpszFileName, FALSE, _T("[��� �Ŵ���] ��ĳ�ʷ� �о���� ����.") );	

	DWORD dwLang( s.GetNumber() );	// dwLang

	while( s.tok != FINISHED )
	{
		PLANG_DATA	pData = new LANG_DATA(); 
		FLERROR_ASSERT_LOG_DO( pData, continue, _T("[��� �Ŵ���] ��� ������ ���� ���� - ����ȣ : [%u]."), dwLang );	
		
		s.GetToken();
		s.GetToken();	// szPath
		FLStrcpy( pData->szPath, _countof( pData->szPath ), s.token );
		s.GetToken();

		s.GetToken();	//	szFileName
		FLStrcpy( pData->szFileName, _countof( pData->szFileName ), s.token );

		s.GetToken();	//	szTitle
		FLStrcpy( pData->szTitle, _countof( pData->szTitle ), s.token );

		s.GetToken();
		s.GetToken();	//	szFontFirst
		FLStrcpy( pData->stFont.lfCaption.szFontFirst, _countof( pData->stFont.lfCaption.szFontFirst ), s.token );
		s.GetToken();

		s.GetToken();
		s.GetToken();	//	szFontSecond
		FLStrcpy( pData->stFont.lfCaption.szFontSecond, _countof( pData->stFont.lfCaption.szFontSecond ), s.token );
		s.GetToken();

		pData->stFont.lfCaption.nDivTitle	= static_cast< CHAR >( s.GetNumber() );				// nDivTitle
		pData->stFont.lfCaption.nDivCaption	= static_cast< CHAR >( s.GetNumber() );				// nDivCaption
		pData->stFont.fdLang.charset		= s.GetNumber();									// charset
		pData->stFont.fdLang.wCodePage		= static_cast< WORD >( s.GetNumber() );				// wCodePage

		s.GetToken();
		s.GetToken();
		FLStrcpy( pData->stFont.fdLang.faceNT, _countof( pData->stFont.fdLang.faceNT ), s.token );	// faceNT
		s.GetToken();

		s.GetToken();
		s.GetToken();
		FLStrcpy( pData->stFont.fdLang.face9x, _countof( pData->stFont.fdLang.face9x ), s.token );	// face9x
		s.GetToken();


		UINT uiFontCount( s.GetNumber() );

		for( UINT nCount = 0; nCount < uiFontCount; nCount++ )
		{
			FONT_INFO	stFontInfo;

			DWORD		dwColorA( NULL ),
						dwColorR( NULL ),
						dwColorG( NULL ),
						dwColorB( NULL );

			s.GetToken();
			s.GetToken();									// szFont
			FLStrcpy( stFontInfo.szFont, _countof( stFontInfo.szFont ), s.token );
			s.GetToken();

			stFontInfo.dwFontSize	= s.GetNumber();		// dwFontSize

			dwColorA = s.GetNumber();						// dwColor_A
			dwColorR = s.GetNumber();						// dwColor_R
			dwColorG = s.GetNumber();						// dwColor_G
			dwColorB = s.GetNumber();						// dwColor_B
			stFontInfo.dwColor = D3DCOLOR_ARGB( dwColorA, dwColorR, dwColorG, dwColorB );

			dwColorA = s.GetNumber();						// dwBgColor6_A
			dwColorR = s.GetNumber();						// dwBgColor6_R
			dwColorG = s.GetNumber();						// dwBgColor6_G
			dwColorB = s.GetNumber();						// dwBgColor6_B
			stFontInfo.dwBgColor = D3DCOLOR_ARGB( dwColorA, dwColorR, dwColorG, dwColorB );

			stFontInfo.nOutLine		= s.GetNumber();		// nOutLine
			stFontInfo.dwFlags		= s.GetNumber();		// dwFlags

			pData->stFont.vecFontInfo.push_back( stFontInfo );
		}

		m_mapLanguageData.insert( pair<DWORD, PLANG_DATA>( dwLang, pData ) );

		dwLang	= s.GetNumber();
	}

	return TRUE;
}


//---------------------------------------------------------------------------------------------
// Get Lang Data ( ��� ������ ��� )
// param	: dwLang
// return	: BOOL 
//---------------------------------------------------------------------------------------------
LANG_DATA const * CLangMan::GetLangData( DWORD dwLang )
{
	ItorLanguageData Itor = m_mapLanguageData.find( dwLang );

	if( Itor == m_mapLanguageData.end() )
		FLERROR_ASSERT_LOG_RETURN( NULL, NULL, _T("[��� �Ŵ���] ��� ������ ã�� �� �����ϴ� - [ %u ]."),  dwLang );	
		

	PLANG_DATA pLangData = Itor->second;
	FLERROR_ASSERT_LOG_RETURN( pLangData, NULL, _T("[��� �Ŵ���] ��� ���� �����Ͱ� ��ȿ���� �ʽ��ϴ� - [ %u ]."),  dwLang );	

	return pLangData;
}


//---------------------------------------------------------------------------------------------
// Get Instance ( �ν��Ͻ� ��� )
// param	: void
// return	: CLangMan* 
//---------------------------------------------------------------------------------------------
CLangMan* CLangMan::GetInstance( void )
{
	static CLangMan	sLocal;
	return &sLocal;
}
