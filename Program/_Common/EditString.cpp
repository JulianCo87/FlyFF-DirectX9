

#include "StdAfx.h"

#define CODE_PAGE(wCodePage)	( wCodePage ? wCodePage : g_xFlyffConfig->GetCodePage() )

namespace nsEditString
{

#define DEFAULT_WIDTH			100
#define DEFAULT_STYLE			0x00000000
#define DEFAULT_PSTYLE			0x00000001
#define DEFAULT_CODEPAGE		0

#define GET_DEFAULT_FONTCOLOR	g_WndFontColorManager->GetEditString()->m_stDefault.GetFontColor()

};

using namespace nsEditString;

#pragma region Contruction

//---------------------------------------------------------------------------------------------
// ������ 
//---------------------------------------------------------------------------------------------
CEditString::CEditString()
:	CString()
,	m_pFont( NULL )
,	m_bWordAlign( TRUE )
,	m_nWidth( DEFAULT_WIDTH )
{
#ifdef __IMPROVE_MAP_SYSTEM

	PFONTCOLOR_EDITSTRING pFontColorEditString = g_WndFontColorManager->GetEditString();
	SetColor( pFontColorEditString->m_stDefault.GetFontColor() );
	SetStyle( 0 );
	SetCodePage( 0 );

	m_sizeFont.cx = 12;
	m_sizeFont.cy = 12;

#endif 
}


//---------------------------------------------------------------------------------------------
// ������ 
//---------------------------------------------------------------------------------------------
CEditString::CEditString( const CString& stringSrc )
:	CString( stringSrc )
,	m_pFont( NULL )
,	m_bWordAlign( TRUE )
,	m_nWidth( DEFAULT_WIDTH )

{
#ifdef __IMPROVE_MAP_SYSTEM
	
	PFONTCOLOR_EDITSTRING pFontColorEditString = g_WndFontColorManager->GetEditString();
	SetColor( pFontColorEditString->m_stDefault.GetFontColor() );
	SetStyle( 0 );
	SetCodePage( 0 );

	m_sizeFont.cx = 12;
	m_sizeFont.cy = 12;

#endif 
}


//---------------------------------------------------------------------------------------------
// ������ 
//---------------------------------------------------------------------------------------------
CEditString::CEditString( const CEditString& stringSrc )
:	CString( stringSrc )
,	m_pFont( stringSrc.m_pFont )
,	m_sizeFont( stringSrc.m_sizeFont )
,	m_bWordAlign( stringSrc.m_bWordAlign )
,	m_nWidth( stringSrc.m_nWidth )
{
	m_adwColor.Append( stringSrc.m_adwColor );
	m_abyStyle.Append( stringSrc.m_abyStyle );
	m_awCodePage.Append( stringSrc.m_awCodePage );
}


//---------------------------------------------------------------------------------------------
// ������ 
//---------------------------------------------------------------------------------------------
CEditString::CEditString( TCHAR ch, int nRepeat )
:	CString( ch, nRepeat )
,	m_pFont( NULL )
,	m_bWordAlign( TRUE )
,	m_nWidth( DEFAULT_WIDTH )
{
	PFONTCOLOR_EDITSTRING pFontColorEditString = g_WndFontColorManager->GetEditString();
	SetColor( pFontColorEditString->m_stDefault.GetFontColor() );
	SetStyle( 0 );
	SetCodePage( 0 );

	m_sizeFont.cx = 12;
	m_sizeFont.cy = 12;
}


//---------------------------------------------------------------------------------------------
// ������ 
//---------------------------------------------------------------------------------------------
CEditString::CEditString( LPCTSTR lpch, int nLength ) 
:	CString( lpch, nLength )
,	m_pFont( NULL )
,	m_bWordAlign( TRUE )
,	m_nWidth( DEFAULT_WIDTH )
{
	PFONTCOLOR_EDITSTRING pFontColorEditString = g_WndFontColorManager->GetEditString();
	SetColor( pFontColorEditString->m_stDefault.GetFontColor() );
	SetStyle( 0 );
	SetCodePage( 0 );

	m_sizeFont.cx = 12;
	m_sizeFont.cy = 12;
}


//---------------------------------------------------------------------------------------------
// ������ 
//---------------------------------------------------------------------------------------------
CEditString::CEditString( const unsigned char* psz ) 
:	CString( psz )
,	m_pFont( NULL )
,	m_bWordAlign( TRUE )
,	m_nWidth( DEFAULT_WIDTH )
{
	PFONTCOLOR_EDITSTRING pFontColorEditString = g_WndFontColorManager->GetEditString();
	SetColor( pFontColorEditString->m_stDefault.GetFontColor() );
	SetStyle( 0 );
	SetCodePage( 0 );

	m_sizeFont.cx = 12;
	m_sizeFont.cy = 12;
}


//---------------------------------------------------------------------------------------------
// ������ 
//---------------------------------------------------------------------------------------------
CEditString::CEditString( LPCWSTR lpsz )
:	CString( lpsz )
,	m_pFont( NULL )
,	m_bWordAlign( TRUE )
,	m_nWidth( DEFAULT_WIDTH )
{
	PFONTCOLOR_EDITSTRING pFontColorEditString = g_WndFontColorManager->GetEditString();
	SetColor( pFontColorEditString->m_stDefault.GetFontColor() );
	SetStyle( 0 );
	SetCodePage( 0 );

	m_sizeFont.cx = 12;
	m_sizeFont.cy = 12;
}


//---------------------------------------------------------------------------------------------
// ������ 
//---------------------------------------------------------------------------------------------
CEditString::CEditString( LPCSTR lpsz ) 
:	CString( lpsz )
,	m_pFont( NULL )
,	m_bWordAlign( TRUE )
,	m_nWidth( DEFAULT_WIDTH )
{
	PFONTCOLOR_EDITSTRING pFontColorEditString = g_WndFontColorManager->GetEditString();
	SetColor( pFontColorEditString->m_stDefault.GetFontColor() );
	SetStyle( 0 );
	SetCodePage( 0 );

	m_sizeFont.cx = 12;
	m_sizeFont.cy = 12;
}

#pragma endregion Contruction


//---------------------------------------------------------------------------------------------
// �ı���
//---------------------------------------------------------------------------------------------
CEditString::~CEditString()
{
}


//---------------------------------------------------------------------------------------------
// �ʱ�ȭ
//---------------------------------------------------------------------------------------------
void CEditString::Init( int nWidth, SIZE sizeFont )
{
	m_nWidth	= nWidth;
	m_sizeFont	= sizeFont;
}


//---------------------------------------------------------------------------------------------
// �ʱ�ȭ
//---------------------------------------------------------------------------------------------
void CEditString::Init( CD3DFont* pFont, CRect* pRect )
{
	TCHAR str[3]	= _T( "��" );
	m_sizeFont		= pFont->GetTextExtent(str);

	BOOL bAlign		= FALSE;

	if( m_nWidth != pRect->Width() ||
		m_pFont != pFont )	// ������� ��Ʈ ���߿� �ϳ��� �����Ͱ� �ٸ��ٸ� ������ ���־���Ѵ�.
		bAlign = TRUE;

	m_pFont		= pFont;
	m_nWidth	= pRect->Width();

	if( bAlign )
		Align( m_pFont, NULL );
}


#pragma region AddString

//---------------------------------------------------------------------------------------------
// Add String ( ���ڿ� �߰� )
// param	: ...
// return	: void 
//---------------------------------------------------------------------------------------------
void CEditString::AddString( LPCTSTR lpsz )
{ 
	int nLine	= GetLineCount() - 1;
	int nBegin	= GetLength();
	int nEnd	= nBegin + _tcslen( lpsz );

	*((CString*)this) += lpsz; 

	for( int i = nBegin; i < nEnd; i++ )
		m_adwColor.SetAtGrow( i, GET_DEFAULT_FONTCOLOR ); 

	for( int i = nBegin; i < nEnd; i++ )
		m_abyStyle.SetAtGrow( i, static_cast<BYTE>( DEFAULT_STYLE ) ); 

	for( int i = nBegin; i < nEnd; i++ )
		m_awCodePage.SetAtGrow( i, CODE_PAGE( DEFAULT_CODEPAGE ) ); 

	Align( m_pFont, nLine );
}


//---------------------------------------------------------------------------------------------
// Add String ( ���ڿ� �߰� )
// param	: ...
// return	: void 
//---------------------------------------------------------------------------------------------
void CEditString::AddString( LPCTSTR lpsz, DWORD dwColor )
{ 
	int nLine	= GetLineCount() - 1;
	int nBegin	= GetLength();
	int nEnd	= nBegin + _tcslen( lpsz );

	*((CString*)this) += lpsz; 

	for( int i = nBegin; i < nEnd; i++ )
		m_adwColor.SetAtGrow( i, dwColor ); 

	for( int i = nBegin; i < nEnd; i++ )
		m_abyStyle.SetAtGrow( i, static_cast<BYTE>( DEFAULT_STYLE ) ); 

	for( int i = nBegin; i < nEnd; i++ )
		m_awCodePage.SetAtGrow( i, CODE_PAGE( DEFAULT_CODEPAGE ) ); 

	Align( m_pFont, nLine );
}


//---------------------------------------------------------------------------------------------
// Add String ( ���ڿ� �߰� )
// param	: ...
// return	: void 
//---------------------------------------------------------------------------------------------
void CEditString::AddString( LPCTSTR lpsz, DWORD dwColor, DWORD dwStyle )
{ 
	int nLine	= GetLineCount() - 1;
	int nBegin	= GetLength();
	int nEnd	= nBegin + _tcslen( lpsz );

	*((CString*)this) += lpsz; 

	for( int i = nBegin; i < nEnd; i++ )
		m_adwColor.SetAtGrow( i, dwColor ); 

	for( int i = nBegin; i < nEnd; i++ )
		m_abyStyle.SetAtGrow( i, (BYTE)( dwStyle ) ); 

	for( int i = nBegin; i < nEnd; i++ )
		m_awCodePage.SetAtGrow( i, CODE_PAGE( DEFAULT_CODEPAGE ) ); 

	Align( m_pFont, nLine );
}


//---------------------------------------------------------------------------------------------
// Add String ( ���ڿ� �߰� )
// param	: ...
// return	: void 
//---------------------------------------------------------------------------------------------
void CEditString::AddString( LPCTSTR lpsz, DWORD dwColor, DWORD dwStyle, WORD wCodePage )
{ 
	int nLine	= GetLineCount() - 1;
	int nBegin	= GetLength();
	int nEnd	= nBegin + _tcslen( lpsz );

	*((CString*)this) += lpsz; 

	for( int i = nBegin; i < nEnd; i++ )
		m_adwColor.SetAtGrow( i, dwColor ); 

	for( int i = nBegin; i < nEnd; i++ )
		m_abyStyle.SetAtGrow( i, (BYTE)( dwStyle ) ); 

	for( int i = nBegin; i < nEnd; i++ )
		m_awCodePage.SetAtGrow( i, CODE_PAGE( DEFAULT_CODEPAGE ) ); 

	Align( m_pFont, nLine );
}


//---------------------------------------------------------------------------------------------
// Add String ( ���ڿ� �߰� )
// param	: ...
// return	: void 
//---------------------------------------------------------------------------------------------
void CEditString::AddString( char ch )
{ 
	int nLine	= GetLineCount() - 1;

	*((CString*)this) += ch; 

	m_adwColor.Add( GET_DEFAULT_FONTCOLOR ); 
	m_abyStyle.Add( (BYTE)( static_cast<BYTE>( DEFAULT_STYLE ) ) ); 
	m_awCodePage.Add( CODE_PAGE( DEFAULT_CODEPAGE ) );

	Align( m_pFont, nLine );
}


//---------------------------------------------------------------------------------------------
// Add String ( ���ڿ� �߰� )
// param	: ...
// return	: void 
//---------------------------------------------------------------------------------------------
void CEditString::AddString( char ch,	DWORD dwColor, DWORD dwStyle )
{ 
	int nLine	= GetLineCount() - 1;

	*((CString*)this) += ch; 

	m_adwColor.Add( dwColor ); 
	m_abyStyle.Add( (BYTE)( static_cast<BYTE>( dwStyle ) ) ); 
	m_awCodePage.Add( CODE_PAGE( DEFAULT_CODEPAGE ) );

	Align( m_pFont, nLine );
}


//---------------------------------------------------------------------------------------------
// Add String ( ���ڿ� �߰� )
// param	: ...
// return	: void 
//---------------------------------------------------------------------------------------------
void CEditString::AddString( char ch, DWORD dwColor, DWORD dwStyle, WORD wCodePage )
{ 
	int nLine	= GetLineCount() - 1;

	*((CString*)this) += ch; 

	m_adwColor.Add( dwColor ); 
	m_abyStyle.Add( (BYTE)( dwStyle ) ); 
	m_awCodePage.Add( CODE_PAGE(wCodePage) );

	Align( m_pFont, nLine );
}


//---------------------------------------------------------------------------------------------
// Add EditString ( ���� ���ڿ� �߰� )
// param	: string
// return	: void 
//---------------------------------------------------------------------------------------------
void CEditString::AddEditString( const CEditString& string )
{
	int nLine	= GetLineCount() - 1;
	int nBegin	= GetLength();
	int nEnd	= nBegin + string.GetLength();

	*((CString*)this) += string; 

	m_adwColor.Append( string.m_adwColor );
	m_abyStyle.Append( string.m_abyStyle );
	m_awCodePage.Append( string.m_awCodePage );

	Align( m_pFont, nLine );
}


//---------------------------------------------------------------------------------------------
// Add ParseString ( �м� ���ڿ� �߰� )
// param	: lpsz
// return	: void 
//---------------------------------------------------------------------------------------------
void CEditString::AddParsingString( LPCTSTR lpsz )
{ 
 	int nLine = GetLineCount() - 1;

	ParsingString( lpsz, GET_DEFAULT_FONTCOLOR, DEFAULT_STYLE, CODE_PAGE( DEFAULT_CODEPAGE ),(CString&)*this, m_adwColor, m_abyStyle, m_awCodePage, DEFAULT_PSTYLE );
	Align( m_pFont, nLine );
}


//---------------------------------------------------------------------------------------------
// Add ParseString ( �м� ���ڿ� �߰� )
// param	: lpsz
// return	: void 
//---------------------------------------------------------------------------------------------
void CEditString::AddParsingString( LPCTSTR lpsz, DWORD dwColor )
{ 
 	int nLine = GetLineCount() - 1;

	ParsingString( lpsz, dwColor, DEFAULT_STYLE, CODE_PAGE( DEFAULT_CODEPAGE ),(CString&)*this, m_adwColor, m_abyStyle, m_awCodePage, DEFAULT_PSTYLE );
	Align( m_pFont, nLine );
}


//---------------------------------------------------------------------------------------------
// Add ParseString ( �м� ���ڿ� �߰� )
// param	: ...
// return	: void 
//---------------------------------------------------------------------------------------------
void CEditString::AddParsingString( LPCTSTR lpsz, DWORD dwColor, DWORD dwStyle )
{ 
 	int nLine = GetLineCount() - 1;

	ParsingString( lpsz, dwColor, dwStyle, CODE_PAGE( DEFAULT_CODEPAGE ),(CString&)*this, m_adwColor, m_abyStyle, m_awCodePage, DEFAULT_PSTYLE );
	Align( m_pFont, nLine );
}


//---------------------------------------------------------------------------------------------
// Add ParseString ( �м� ���ڿ� �߰� )
// param	: ...
// return	: void 
//---------------------------------------------------------------------------------------------
void CEditString::AddParsingString( LPCTSTR lpsz, DWORD dwColor, DWORD dwStyle, WORD wCodePage, DWORD dwPStyle )
{ 
 	int nLine = GetLineCount() - 1;

	ParsingString( lpsz, dwColor, dwStyle, CODE_PAGE(wCodePage),(CString&)*this, m_adwColor, m_abyStyle, m_awCodePage, dwPStyle );
	Align( m_pFont, nLine );
}


#pragma endregion AddString


#pragma region SetString


//---------------------------------------------------------------------------------------------
// Set String ( ���ڿ� ���� )
// param	: string
// return	: void 
//---------------------------------------------------------------------------------------------
void CEditString::SetString( LPCTSTR lpsz )
{ 
	Empty();

	*((CString*)this) = lpsz; 

	SetColor( GET_DEFAULT_FONTCOLOR );
	SetStyle( DEFAULT_STYLE );
	SetCodePage( CODE_PAGE( DEFAULT_CODEPAGE ) );

	Align( m_pFont, 0 );
}


//---------------------------------------------------------------------------------------------
// Set String ( ���ڿ� ���� )
// param	: string, dwColor
// return	: void 
//---------------------------------------------------------------------------------------------
void CEditString::SetString( LPCTSTR lpsz, DWORD dwColor )
{ 
	Empty();

	*((CString*)this) = lpsz; 

	SetColor( dwColor );
	SetStyle( DEFAULT_STYLE );
	SetCodePage( CODE_PAGE( DEFAULT_CODEPAGE ) );

	Align( m_pFont, 0 );
}


//---------------------------------------------------------------------------------------------
// Set String ( ���ڿ� ���� )
// param	: string, dwColor
// return	: void 
//---------------------------------------------------------------------------------------------
void CEditString::SetString( LPCTSTR lpsz, DWORD dwColor, DWORD dwStyle )
{ 
	Empty();

	*((CString*)this) = lpsz; 

	SetColor( dwColor );
	SetStyle( dwStyle );
	SetCodePage( CODE_PAGE( DEFAULT_CODEPAGE ) );

	Align( m_pFont, 0 );
}


//---------------------------------------------------------------------------------------------
// Set String ( ���ڿ� ���� )
// param	: string
// return	: void 
//---------------------------------------------------------------------------------------------
void CEditString::SetString( LPCTSTR lpsz, DWORD dwColor, DWORD dwStyle, WORD wCodePage )
{ 
	Empty();

	*((CString*)this) = lpsz; 

	SetColor( dwColor );
	SetStyle( dwStyle );
	SetCodePage( CODE_PAGE( wCodePage ) );

	Align( m_pFont, 0 );
}


//---------------------------------------------------------------------------------------------
// Set String ( ���ڿ� ���� )
// param	: lpsz
// return	: void 
//---------------------------------------------------------------------------------------------
void CEditString::SetString( char* lpsz )
{
	Empty();

	*((CString*)this) = lpsz; 

	SetColor( GET_DEFAULT_FONTCOLOR );
	SetStyle( DEFAULT_STYLE );
	SetCodePage( CODE_PAGE( DEFAULT_CODEPAGE ) );

	Align( m_pFont, 0 );
}


//---------------------------------------------------------------------------------------------
// Set String ( ���ڿ� ���� )
// param	: ...
// return	: void 
//---------------------------------------------------------------------------------------------
void CEditString::SetString( char* lpsz, DWORD dwColor , DWORD dwStyle, WORD wCodePage )
{
	Empty();

	*((CString*)this) = lpsz; 

	SetColor( dwColor );
	SetStyle( dwStyle );
	SetCodePage( CODE_PAGE(wCodePage) );

	Align( m_pFont, 0 );
}


//---------------------------------------------------------------------------------------------
// Set ParseString ( �м� ���ڿ� ���� )
// param	: ...
// return	: void 
//---------------------------------------------------------------------------------------------
void CEditString::SetParsingString( LPCTSTR lpsz )
{ 
	Empty();

	ParsingString( lpsz, GET_DEFAULT_FONTCOLOR, DEFAULT_STYLE, CODE_PAGE( DEFAULT_CODEPAGE ), (CString&)*this, m_adwColor, m_abyStyle, m_awCodePage, DEFAULT_PSTYLE );

	Align( m_pFont, 0 );
}


//---------------------------------------------------------------------------------------------
// Set ParseString ( �м� ���ڿ� ���� )
// param	: ...
// return	: void 
//---------------------------------------------------------------------------------------------
void CEditString::SetParsingString( LPCTSTR lpsz, DWORD dwColor )
{ 
	Empty();

	ParsingString( lpsz, dwColor, DEFAULT_STYLE, CODE_PAGE( DEFAULT_CODEPAGE ), (CString&)*this, m_adwColor, m_abyStyle, m_awCodePage, DEFAULT_PSTYLE );

	Align( m_pFont, 0 );
}


//---------------------------------------------------------------------------------------------
// Set ParseString ( �м� ���ڿ� ���� )
// param	: ...
// return	: void 
//---------------------------------------------------------------------------------------------
void CEditString::SetParsingString( LPCTSTR lpsz, DWORD dwColor, DWORD dwStyle, WORD wCodePage, DWORD dwPStyle )
{ 
	Empty();

	ParsingString( lpsz, dwColor, dwStyle, CODE_PAGE(wCodePage), (CString&)*this, m_adwColor, m_abyStyle, m_awCodePage, dwPStyle );

	Align( m_pFont, 0 );
}



//---------------------------------------------------------------------------------------------
// Set ParseString ( �м� ���ڿ� ���� )
// param	: ...
// return	: void 
//---------------------------------------------------------------------------------------------
void CEditString::SetParsingString( LPCTSTR lpsz, DWORD dwColor, DWORD dwStyle, WORD wCodePage, DWORD dwPStyle, BOOL bNoInitAlign )
{ 
	Empty();

	ParsingString( lpsz, dwColor, dwStyle, CODE_PAGE(wCodePage), (CString&)*this, m_adwColor, m_abyStyle, m_awCodePage, dwPStyle );

	if( bNoInitAlign == FALSE )
		Align( m_pFont, 0 );
}


//---------------------------------------------------------------------------------------------
// Set Edit String ( ���� ���ڿ� ���� )
// param	: string
// return	: void 
//---------------------------------------------------------------------------------------------
void CEditString::SetEditString( const CEditString& string )
{
	Empty();

	*((CString*)this) = string; 

	m_adwColor.RemoveAll();
	m_adwColor.Append( string.m_adwColor );
	m_abyStyle.RemoveAll();
	m_abyStyle.Append( string.m_abyStyle );
	m_awCodePage.RemoveAll();
	m_awCodePage.Append( string.m_awCodePage );

	Align( m_pFont, 0 );
}

#pragma endregion SetString







void CEditString::SetAlpha( DWORD dwAlpha )
{
	for( int i = 0; i < GetLength(); i++)
		m_adwColor.SetAt( i, ( m_adwColor.GetAt( i ) & 0x00ffffff ) | ( dwAlpha << 24 ) );
}


void CEditString::SetColor( DWORD dwColor )
{
	m_adwColor.SetSize( GetLength() );
	for( int i = 0; i < GetLength(); i++)
		m_adwColor.SetAt( i , dwColor );
}
void CEditString::SetStyle( DWORD dwStyle )
{
	m_abyStyle.SetSize( GetLength() );
	for( int i = 0; i < GetLength(); i++)
		m_abyStyle.SetAt( i , (BYTE)( dwStyle ) );
}
void CEditString::SetCodePage( WORD wCodePage )
{
	m_awCodePage.SetSize( GetLength() );
	for( int i = 0; i < GetLength(); i++)
		m_awCodePage.SetAt( i , CODE_PAGE(wCodePage) );
}
void CEditString::SetColor( int nPos, int nLength, DWORD dwColor )
{
	for( int i = nPos; i < nPos + nLength; i++)
		m_adwColor.SetAt( i, dwColor );
}
void CEditString::SetStyle( int nPos, int nLength, DWORD dwStyle )
{
	for( int i = nPos; i < nPos + nLength; i++)
		m_abyStyle.SetAt( i, (BYTE)( m_abyStyle.GetAt( i ) | dwStyle ) );
}
#ifdef __IMPROVE_MAP_SYSTEM
void CEditString::ClearColor( DWORD dwColor )
{
	m_adwColor.SetSize( GetLength() );
	for( int i = 0; i < GetLength(); ++i )
	{
		m_adwColor.SetAt( i , (BYTE)( m_adwColor.GetAt( i ) & ~dwColor ) );
	}
}
void CEditString::ClearColor( int nPos, int nLength, DWORD dwColor )
{
	for( int i = nPos; i < nPos + nLength; ++i )
	{
		m_adwColor.SetAt( i, (BYTE)( m_adwColor.GetAt( i ) & ~dwColor ) );
	}
}
#endif // __IMPROVE_MAP_SYSTEM
void CEditString::ClearStyle( int nPos, int nLength, DWORD dwStyle )
{
	for( int i = nPos; i < nPos + nLength; i++)
		m_abyStyle.SetAt( i , (BYTE)( m_abyStyle.GetAt( i ) & ~dwStyle ) );
}

void CEditString::ClearStyle( DWORD dwStyle )
{
	m_abyStyle.SetSize( GetLength() );
	for( int i = 0; i < GetLength(); i++)
		m_abyStyle.SetAt( i , (BYTE)( m_abyStyle.GetAt( i ) & ~dwStyle ) );
}

void CEditString::SetCodePage( int nPos, int nLength, WORD wCodePage )
{
	for( int i = nPos; i < nPos + nLength; i++)
		m_awCodePage.SetAt( i, CODE_PAGE(wCodePage) );
}
void CEditString::Empty( )
{
	CString::Empty();
	m_adwLineOffset.RemoveAll();
	m_adwColor.RemoveAll();
	m_abyStyle.RemoveAll();
	m_awCodePage.RemoveAll();
	m_adwLineOffset.Add( 0 );
	//m_adwColor.Add( 0 );
	//m_abyStyle.Add( 0 );
	//m_adwLineOffset.Add( 0 );
}

void CEditString::GetTextFormat( CString& str )
{
	//gmpbigsun_WKG
	int nLen = CString::GetLength();

	if( nLen )
	{
		int   nColorMarkCnt = 0;
		int   nSMarkCnt = 0;
		int   nBMarkCnt = 0;
		int   nUMarkCnt = 0;
#ifdef __HYPERLINK_ITEM16
		int   nHIMarkCnt = 0;
#endif // __HYPERLINK_ITEM16
		DWORD dwColor;
		DWORD dwStyle;
		DWORD dwStyleS;
		DWORD dwStyleB;
		DWORD dwStyleU;
#ifdef __HYPERLINK_ITEM16
		DWORD dwStyleHI;
#endif // __HYPERLINK_ITEM16
		PFONTCOLOR_EDITSTRING pFontColorEditString = g_WndFontColorManager->GetEditString();
		DWORD dwOldColor	= pFontColorEditString->m_stDefault.GetFontColor();
		DWORD dwOldStyleS	= 0;
		DWORD dwOldStyleB	= 0;
		DWORD dwOldStyleU	= 0;
#ifdef __HYPERLINK_ITEM16
		DWORD dwOldStyleHI = 0;
#endif // __HYPERLINK_ITEM16
		str.Empty();

		for( int i=0; i<nLen; i++ )
		{
			dwColor = m_adwColor.GetAt(i);
			dwStyle = m_abyStyle.GetAt(i);

			dwStyleU = dwStyle & ESSTY_UNDERLINE;
			dwStyleB = dwStyle & ESSTY_BOLD;
			dwStyleS = dwStyle & ESSTY_STRIKETHROUGH;
#ifdef __HYPERLINK_ITEM16
			dwStyleHI = dwStyle & ESSTY_HYPERLINK_ITEM;
#endif // __HYPERLINK_ITEM16

#ifdef __HYPERLINK_ITEM16
			if( dwColor != dwOldColor && !( dwStyleHI & ESSTY_HYPERLINK_ITEM ) )
#else // __HYPERLINK_ITEM16
			if( dwColor != dwOldColor )
#endif // __HYPERLINK_ITEM16
			{
				if( nColorMarkCnt )
				{
					str += "#nc";
					nColorMarkCnt--;
				}

				if( dwColor != pFontColorEditString->m_stDefault.GetFontColor() )
				{
					CString strColor;
					strColor.Format( "#c%x", dwColor );
					
					str += strColor;	
					nColorMarkCnt++;
				}

			}

#ifdef __HYPERLINK_ITEM16
			//gmpbigsun: �Ʒ��ڵ�� �� �ҷ��� �Ѱɱ�?? ������ ��Ÿ���� �ٸ��� Į�� �ٸ��� ������ �����۸�ũ�ΰ�?
			//if( dwStyleHI != dwOldStyleHI || dwColor != dwOldColor )
			//{
			//	if( nHIMarkCnt )
			//	{
			//		str += "$ni";
			//		nHIMarkCnt--;
			//	}

			//	if( dwColor != 0xff000000 )
			//	{
			//		CString strColor;
			//		strColor.Format( "$i%x", dwColor );
			//		str += strColor;	
			//		nHIMarkCnt++;
			//	}
			//}
#endif // __HYPERLINK_ITEM16

			if( dwStyleU != dwOldStyleU )
			{
				if( nUMarkCnt )
				{
					str += "#nu";
					nUMarkCnt--;
				}

				if( dwStyleU & ESSTY_UNDERLINE )
				{
					str += "#u";
					nUMarkCnt++;
				}
			}

			if( dwStyleB != dwOldStyleB )
			{
				if( nBMarkCnt )
				{
					str += "#nb";
					nBMarkCnt--;
				}
				
				if( dwStyleB & ESSTY_BOLD )
				{
					str += "#b";
					nBMarkCnt++;
				}
			}
			
			if( dwStyleS != dwOldStyleS )
			{
				if( nSMarkCnt )
				{
					str += "#ns";
					nSMarkCnt--;
				}
				
				if( dwStyleS & ESSTY_STRIKETHROUGH )
				{
					str += "#s";
					nSMarkCnt++;
				}
			}
			
			str += CString::GetAt(i);

			dwOldColor = dwColor;

			dwOldStyleS = dwStyleS;
			dwOldStyleB = dwStyleB;
			dwOldStyleU = dwStyleU;
#ifdef __HYPERLINK_ITEM16
			dwOldStyleHI = dwStyleHI;
#endif // __HYPERLINK_ITEM16
		}

		if( nColorMarkCnt )
			str += "#nc";

		if( nSMarkCnt )
			str += "#ns";

		if( nBMarkCnt )
			str += "#nb";

		if( nUMarkCnt )
			str += "#nu";	
#ifdef __HYPERLINK_ITEM16
		if( nHIMarkCnt )
			str += "$ni";
#endif // __HYPERLINK_ITEM16
	}
}

void CEditString::ParsingString( LPCTSTR lpsz, DWORD dwColor, DWORD dwStyle, WORD wCodePage, CString& string, CDWordArray& adwColor, CByteArray& abyStyle, CWordArray& awCodePage, DWORD dwPStyle )
{
	FLASSERT( wCodePage );
	// #cffffffff // Į�� 
	// #u // underline
	// #b // bold
	DWORD dwCurColor = dwColor;
	DWORD dwCurStyle = dwStyle;
	DWORD wCurCodePage = wCodePage;
	CString strTemp;
	TCHAR szColor[9];
	int nLen = strlen( lpsz );
	
	for( int i = 0; i < nLen; i++ )
	{
		if( lpsz[ i ] == '#' ) // �ν� �ڵ�
		{
			if( ++i >= nLen )
				break;
			switch( lpsz[ i ] )
			{
			case 'c':
				{
					if( ++i >= nLen )
						break;

					if( dwPStyle & PS_USE_MACRO )
					{
						memcpy( szColor, &lpsz[ i ], 8 );
						szColor[ 8 ] = 0;
						DWORDLONG dwlNumber = 0;
						DWORD dwMulCnt = 0;
						CHAR cVal;
						for( int j = 7; j >= 0; j--)
						{
							cVal = szColor[ j ];
							if( cVal >= 'a' )
								cVal = ( cVal - 'a' ) + 10;
							else cVal -= '0';
							dwlNumber |= (DWORDLONG) cVal << dwMulCnt;
							dwMulCnt += 4;
						}
						dwCurColor = (DWORD)dwlNumber;
					}

					i += 7;
				}
				break;
			case 'u':
				if( dwPStyle & PS_USE_MACRO )
					dwCurStyle |= ESSTY_UNDERLINE;
				break;
			case 'b':
				if( dwPStyle & PS_USE_MACRO )
					dwCurStyle |= ESSTY_BOLD;
				break;
			case 's':
				if( dwPStyle & PS_USE_MACRO )
					dwCurStyle |= ESSTY_STRIKETHROUGH;
				break;

			case 'l':
				{
					if(++i >= nLen)
						break;

					if( dwPStyle & PS_USE_MACRO )
					{						
						memcpy( szColor, &lpsz[ i ], 4 );
						szColor[ 4 ] = 0;
						wCurCodePage = CODE_PAGE( atoi( szColor ) );
					}
					i += 3;
				}				
				break;
			case 'n':
				if( ++i >= nLen )
					break;

				if( dwPStyle & PS_USE_MACRO )
				{					
					switch( lpsz[ i ] )
					{
					case 'c':
						dwCurColor = dwColor;
						break;
					case 'b':
						dwCurStyle &= ~ESSTY_BOLD; 
						break;
					case 'u':
						dwCurStyle &= ~ESSTY_UNDERLINE; 
						break;
					case 's':
						dwCurStyle &= ~ESSTY_STRIKETHROUGH; 
						break;
					case 'l':
						wCurCodePage = wCodePage;
						break;
					}
				}
				break;
			default: // ����ڵ带 �߰� ������ ��� 
				if( dwPStyle & PS_USE_MACRO )
				{
					// #�ڵ带 �־��ش�
					strTemp += lpsz[ i - 1 ];
					adwColor.Add( dwCurColor );
					abyStyle.Add( (BYTE)( dwCurStyle ) );			
					awCodePage.Add( (WORD)( wCurCodePage ) );
					// ���� ���ڸ� �־��ش�.
					strTemp += lpsz[ i ];
					adwColor.Add( dwCurColor );
					abyStyle.Add( (BYTE)( dwCurStyle ) );
					awCodePage.Add( (WORD)( wCurCodePage ) );
				}
				break;
			}
		}
#ifdef __HYPERLINK_ITEM16
		else if( lpsz[ i ] == '$' )
		{
			if( ++i >= nLen )
			{
				break;
			}

			switch( lpsz[ i ] )
			{
			case 'i':
				{
					if( ++i >= nLen )
						break;

					if( dwPStyle & PS_USE_MACRO )
					{
						//gmpbigsun_WKG : ���⼭ ��ũ ������������ �м��ؼ� Data�� ���ܾ� �Ѵ�.
						//masterid, itemOid, linenum, seq = 0( ���ٴ� �Ѱ��� ��ȿ�ϴٰ� �����Ѵ� )

						char szMasterID[9];
						char szItemID[5];

						memcpy( szColor, &lpsz[ i ], 8 );
						memcpy( szMasterID, &lpsz[ i + 8 ], 8 );
						memcpy( szItemID, &lpsz[ i + 16 ], 4 );
						szColor[ 8 ] = 0;
						szMasterID[ 8 ] = 0;
						szItemID[ 4 ] = 0;
						

						if( dwPStyle & PS_REALADDING_CHATWND )
						{
							//������ ä�����â�� ���� �༮�� �����͸� �����.
							CWndChat* pWndChat = ( CWndChat* )g_WndMng.GetApplet( 1014 ); //APP_COMMUNICATION_CHAT
							if( !pWndChat )
								break;

							//�ϴ� �ӽ�����ҿ� ����
							HLITEM_DATA& kReceData = pWndChat->m_kTempHLData_Receive;
							kReceData._masterID = (DWORD)_atoi64( szMasterID );
							kReceData._itemID = atoi( szItemID );
							kReceData._wLine = pWndChat->GetLineCount( );

							DWORD tmp = kReceData._wLine;
							DWORD key = ( tmp << 16 ) | 0; // �Ѷ��δ� �Ѱ��� �ϴ� ( ���ι�ȣ | ���� )
							pWndChat->m_cHyperLinkItems.insert( HyperLinkItemContainer::value_type( key, kReceData ) );
						}
			
						DWORDLONG dwlNumber = 0;
						DWORD dwMulCnt = 0;
						CHAR cVal;
						for( int j = 7; j >= 0; j--)
						{
							cVal = szColor[ j ];
							if( cVal >= 'a' )
								cVal = ( cVal - 'a' ) + 10;
							else cVal -= '0';
							dwlNumber |= (DWORDLONG) cVal << dwMulCnt;
							dwMulCnt += 4;
						}
						dwCurColor = (DWORD)dwlNumber;
						dwCurStyle |= ESSTY_BOLD;
						dwCurStyle |= ESSTY_HYPERLINK_ITEM;
					}

					//i += 7;
					i += ( 7 + 8 + 4 );
					break;
				}
			case 'n':
				{
					if( ++i >= nLen )
					{
						break;
					}

					if( dwPStyle & PS_USE_MACRO )
					{					
						switch( lpsz[ i ] )
						{
						case 'i':
							{
								dwCurColor = dwColor;
								dwCurStyle &= ~ESSTY_BOLD;
								dwCurStyle &= ~ESSTY_HYPERLINK_ITEM; 
								break;
							}
						}
					}
					break;
				}
			default:
				{
					//if( dwPStyle & PS_USE_MACRO )
					//{
					//	strTemp += lpsz[ i - 1 ];
					//	adwColor.Add( dwCurColor );
					//	abyStyle.Add( (BYTE)( dwCurStyle ) );
					//	awCodePage.Add( (WORD)( wCurCodePage ) );
					//	strTemp += lpsz[ i ];
					//	adwColor.Add( dwCurColor );
					//	abyStyle.Add( (BYTE)( dwCurStyle ) );
					//	awCodePage.Add( (WORD)( wCurCodePage ) );
					//}

					break;
				}

			}
		}
#endif // __HYPERLINK_ITEM16
		else 
		{
			if( lpsz[ i ] == '\\' && lpsz[ i+1 ] == 'n' )
			{
				strTemp += '\n';
				adwColor.Add( dwCurColor );
				abyStyle.Add( (BYTE)( dwCurStyle ) );
				awCodePage.Add( (WORD)( wCurCodePage ) );

				i+=1;
			}
			else
			{
				strTemp += lpsz[ i ];
				int nlength = strTemp.GetLength();
				adwColor.Add( dwCurColor );
				abyStyle.Add( (BYTE)( dwCurStyle ) );
				awCodePage.Add( (WORD)( wCurCodePage ) );
			}
		}
	}
	string += strTemp;
	//*((CString*)this) = string; 
	//m_adwColor.RemoveAll();
	//m_adwColor.Append( adwColor );
}



int CEditString::Insert( int nIndex, TCHAR ch, DWORD dwColor, DWORD dwStyle, WORD wCodePage )
{
	int nLine = GetLineCount() - 1;
	CString::Insert( nIndex, ch );
	m_adwColor.InsertAt( nIndex, dwColor );
	m_abyStyle.InsertAt( nIndex, (BYTE)( dwStyle ) );
	m_awCodePage.InsertAt( nIndex, CODE_PAGE(wCodePage) );
	Align( m_pFont, 0 );//nLine );
	return TRUE;
}
int CEditString::Insert( int nIndex, LPCTSTR pstr, DWORD dwColor, DWORD dwStyle, WORD wCodePage )
{
	int nLine = GetLineCount() - 1;
	CString::Insert( nIndex, pstr );
	m_adwColor.InsertAt( nIndex, dwColor, _tcslen( pstr) );
	m_abyStyle.InsertAt( nIndex, (BYTE)( dwStyle ), _tcslen( pstr) );
	m_awCodePage.InsertAt( nIndex, CODE_PAGE(wCodePage), _tcslen( pstr) );
	Align( m_pFont, 0 );//nLine );
	return TRUE;
}
int CEditString::Delete( int nIndex, int nCount )
{
	int nLine = GetLineCount() - 1;
	CString::Delete( nIndex, nCount );
	m_adwColor.RemoveAt( nIndex, nCount );
	m_abyStyle.RemoveAt( nIndex, nCount );
	m_awCodePage.RemoveAt( nIndex, nCount );
	Align( m_pFont, 0 );//nLine );
	return TRUE;
}
int CEditString::DeleteLine( int nBeginLine, int nCount )
{
	int nMax = nBeginLine + nCount;
	int nBeginOffset, nEndOffset;
	int nLengthCount = 0;
	if( nMax > m_adwLineOffset.GetSize() )
	{
		nCount -= nMax - m_adwLineOffset.GetSize();
		nMax = m_adwLineOffset.GetSize();
	}
	for( int i = nBeginLine; i < nMax; i++ )
	{
		nBeginOffset = GetLineOffset( i );
		nEndOffset = GetLineOffset( i + 1 );
		nLengthCount += nEndOffset - nBeginOffset;
	}
	nBeginOffset = GetLineOffset( nBeginLine );
	CString::Delete( nBeginOffset, nLengthCount );
	m_adwColor.RemoveAt( nBeginOffset, nLengthCount );
	m_abyStyle.RemoveAt( nBeginOffset, nLengthCount );
	m_awCodePage.RemoveAt( nBeginOffset, nLengthCount );
	m_adwLineOffset.RemoveAt( nBeginLine, nCount ); 

	for( int j = nBeginLine; j < m_adwLineOffset.GetSize(); j++ )
	{
		int nNewOffset = m_adwLineOffset.GetAt( j ) - nLengthCount;
		m_adwLineOffset.SetAt( j, nNewOffset );
	}

	//ign( m_pFont, 0 );//nLine );
	return TRUE;
}
TCHAR CEditString::GetAt( int nIndex ) const
{
	return CString::GetAt( nIndex );
}

int CEditString::Find( LPCTSTR pstr, int nStart ) const
{
	return CString::Find( pstr, nStart );
}


void CEditString::SetAt( int nIndex, TCHAR ch, DWORD dwColor, DWORD dwStyle, WORD wCodePage )
{
	int nLine = GetLineCount() - 1;
	CString::SetAt( nIndex, ch );
	m_adwColor.SetAt( nIndex, dwColor );
	m_abyStyle.SetAt( nIndex, (BYTE)( dwStyle ) );
	m_awCodePage.SetAt( nIndex, CODE_PAGE(wCodePage) );
	Align( m_pFont, 0 );//nLine );
}


void CEditString::Adjust( int nWidth, SIZE sizeFont )
{
	m_nWidth = nWidth;
	m_sizeFont = sizeFont;
}
void CEditString::Adjust( CD3DFont* pFont, CRect* pRect )
{
	TCHAR str[ 3 ] = _T( "��" );
	m_sizeFont = pFont->GetTextExtent(str);
	BOOL bAlign = FALSE;
	// ������� ��Ʈ ���߿� �ϳ��� �����Ͱ� �ٸ��ٸ� ������ ���־���Ѵ�.
	if( m_nWidth != pRect->Width() || m_pFont != pFont )
		bAlign = TRUE;
	m_nWidth = pRect->Width();
	m_pFont = pFont;
	if( bAlign )
		Align( m_pFont, 0 );//nLine );
}

void CEditString::Align( CD3DFont* pFont, int nBeginLine )
{
	if( pFont == NULL )
		return;
	int nWidth  = m_nWidth;

	if( nBeginLine < 0 ) nBeginLine = 0;
	CSize sizeString = m_sizeFont;
	int nLength = GetLength();
	if( nLength == 0 || nBeginLine == 0 )
	{
		m_adwLineOffset.RemoveAll();
		if( nLength == 0 )
			return;
	}
	CString strTemp;
	int nOffset = 0;
	if( GetLineCount() )
	{
		nOffset = GetLineOffset( nBeginLine );
		//  ���οɼ� ���� �̻� ���̶�� 
		if( nOffset < 0 || nOffset >= nLength ) 
			FLERROR_LOG( PROGRAM_NAME, _T( "pos = %c, LineCount = %d, Length = %d" ), nOffset, GetLineCount(), nLength );
	}
	char chr = 0; 
	int nLineHeight = sizeString.cy;
	LPCTSTR lpszString = *this;
	CSize sizeTemp;	
	CString strAdd;
	int nSpace = 0;
	int nOffAdd = 0;

	BOOL bWordAlign		= FALSE;
	// g_xFlyffConfig->GetMainLanguage()�� constructor���� ȣ�� �� �� ���⿡ ���⼭ �Ѵ�.
	int nLang	= g_xFlyffConfig->GetMainLanguage();
	switch( nLang )
	{
		case LANG_USA:
		case LANG_ID:
		case LANG_PHP:
		case LANG_GER:
		case LANG_SPA:
		case LANG_POR:
		case LANG_FRE:
		case LANG_VTN:
		case LANG_RUS:
			bWordAlign	= m_bWordAlign;
			break;
	}

	do
	{
		m_adwLineOffset.SetAtGrow( nBeginLine++,  nOffset );
		strTemp = _T("");
		sizeTemp = CSize( 0, 0 );
		strAdd.Empty();
		nSpace = 0;
		do 
		{
			nOffAdd = 0;
			chr = GetAt( nOffset + nOffAdd );
			nOffAdd++;
			if( strchr( " +-*^/%=;(),':{}.", chr ) ) // chr == ' ' )
				nSpace = nOffset + 1;
			if( chr == '\n' )
			{
				nOffset += nOffAdd;
				break;
			}
			if( chr == '\r' )
				nOffset += nOffAdd;
			else
			{
				if( nOffset + nOffAdd < nLength && IsDBCSLeadByte( chr ) )
				{
					strAdd += chr;
					chr = GetAt( nOffset + nOffAdd );
					nOffAdd++;
				}
				strAdd += chr;
				CSize sizeAdd;
				pFont->GetTextExtent( strAdd, &sizeAdd );
				sizeTemp.cx += sizeAdd.cx;
				if( strAdd[ 0 ] != ' ' && sizeTemp.cx > nWidth ) 
				{
					if( bWordAlign && nSpace )
						nOffset = nSpace;
					break;
				}		
				else
				{
					strTemp += strAdd;
					nOffset += nOffAdd;
				}
				strAdd.Empty();
			}
		} while( nOffset < nLength && chr != '\0' && chr != '\n' );

		sizeString.cy += nLineHeight;
	} 
	while( chr != '\0' && nOffset < nLength );
}


DWORD CEditString::GetLineCount() 
{
	return (DWORD) m_adwLineOffset.GetSize();//- 1;
}
DWORD CEditString::GetLineOffset( DWORD dwLine ) 
{
	if ( dwLine >= (DWORD)m_adwLineOffset.GetSize() ) 
		return GetLength();
	if ( dwLine < 0 ) return 0;
	return m_adwLineOffset[ dwLine ];
}
void CEditString::GetLine( DWORD dwLine, CHAR* szStr )
{
	CString retval = _T("");

	if( dwLine >= 0 && dwLine < GetLineCount() ) 
	{
		LONG begin = GetLineOffset( dwLine );
		LONG size;
		if( dwLine < GetLineCount() ) 
			size = GetLineOffset( dwLine + 1 ) - begin ;//- 1;
		else 
			size = GetLength() - begin;

		PCXSTR pData = GetString();
		memcpy( ( VOID* )szStr, ( VOID* )&pData[ begin ], size );

		szStr[ size ] = 0;
	}
}

CString CEditString::GetLine(DWORD dwLine)
{
	CString retval = _T("");

	if( dwLine >= 0 && dwLine < GetLineCount() ) 
	{
		LONG begin = GetLineOffset( dwLine );
		LONG size;
		if( dwLine < GetLineCount() ) 
			size = GetLineOffset( dwLine + 1 ) - begin ;//- 1;
		else 
			size = GetLength() - begin;
		retval = Mid( begin, size ); 
		//retval.Remove('\r');
		//retval.Remove('\n');
	}
	return retval;
}
#ifdef __IMPROVE_MAP_SYSTEM
CString CEditString::GetLine( DWORD dwLine, int& rnLineStartPosition )
{
	CString retval = _T("");

	if( dwLine >= 0 && dwLine < GetLineCount() )
	{
		LONG begin = GetLineOffset( dwLine );
		LONG size;
		if( dwLine < GetLineCount() )
			size = GetLineOffset( dwLine + 1 ) - begin ;
		else
			size = GetLength() - begin;
		retval = Mid( begin, size );
		rnLineStartPosition = static_cast< int >( begin );
	}
	return retval;
}
#endif // __IMPROVE_MAP_SYSTEM
CEditString CEditString::GetEditString( DWORD dwLine )
{
	CEditString retval = _T("");
	
	if( dwLine >= 0 && dwLine < GetLineCount() ) 
	{
		LONG begin = GetLineOffset( dwLine );
		LONG size;
		if( dwLine < GetLineCount() ) 
			size = GetLineOffset( dwLine + 1 ) - begin;
		else 
			size = GetLength() - begin;
		retval = Mid( begin, size ); 
		for( int i = 0; i < size; i++ )
		{
			retval.m_adwColor[ i ] = m_adwColor[ begin + i ];
			retval.m_abyStyle[ i ] = m_abyStyle[ begin + i ];
			retval.m_awCodePage[ i ] = m_awCodePage[ begin + i ];
		}
	}
	return retval;
}
DWORD CEditString::OffsetToLine(DWORD dwOffset)
{
	CString retval = _T("");

	DWORD dwLineCount = GetLineCount();

	for(DWORD i = 0; i < dwLineCount; i++) 
	{
		DWORD dwBegin = GetLineOffset( i );
		DWORD dwNext = GetLineOffset( i + 1 );

		if(dwOffset >= dwBegin && dwOffset < dwNext)
			return i;
	}
	if( dwOffset == (DWORD)GetLength() )
	{
		if( dwOffset > 0 && GetAt( dwOffset - 1 ) == '\n')
			return dwLineCount;
		return dwLineCount - 1;
	}
	return 0;
}

const CEditString& CEditString::operator=( const CEditString& stringSrc )
{
	m_bWordAlign = stringSrc.m_bWordAlign;
	m_pFont = stringSrc.m_pFont;
	Init( stringSrc.m_nWidth, stringSrc.m_sizeFont );
	SetEditString( stringSrc ); return *this;
}


void CEditString::InitWordAlignment()
{
	m_bWordAlign = TRUE;
}
