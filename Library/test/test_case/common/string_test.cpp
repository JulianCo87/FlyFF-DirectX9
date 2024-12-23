

#include <gtest/gtest.h>
#include <common/FLStringFunction.h>


TEST( TrimLeft, C_String )
{
	char test[100] = "   a s d f  ";
	FLTrimLeft( test, _countof( test ) );
	EXPECT_TRUE( ::_stricmp( test, "a s d f  " ) == 0 );
}

TEST( TrimRight, C_String )
{
	char test[100] = "   a s d f  ";
	FLTrimRight( test, _countof( test ) );
	EXPECT_TRUE( ::_stricmp( test, "   a s d f" ) == 0 );
}

TEST( Trim, C_String )
{
	char test[100] = "   a s d f  ";
	FLTrim( test, _countof( test ) );
	EXPECT_TRUE( ::_stricmp( test, "a s d f" ) == 0 );
}

TEST( TrimLeft, W_String )
{
	wchar_t test[100] = L"   a s d f  ";
	FLTrimLeft( test, _countof( test ) );
	EXPECT_TRUE( ::_wcsicmp( test, L"a s d f  " ) == 0 );
}

TEST( TrimRight, W_String )
{
	wchar_t test[100] = L"   a s d f  ";
	FLTrimRight( test, _countof( test ) );
	EXPECT_TRUE( ::_wcsicmp( test, L"   a s d f" ) == 0 );
}

TEST( Trim, W_String )
{
	wchar_t test[100] = L"   a s d f  ";
	FLTrim( test, _countof( test ) );
	EXPECT_TRUE( ::_wcsicmp( test, L"a s d f" ) == 0 );
}

TEST( TrimLeft, String )
{
	std::string test = "   a s d f  ";
	std::string result = FLTrimLeft( test );
	EXPECT_TRUE( result == std::string( "a s d f  " ) );
}

TEST( TrimRight, String )
{
	std::string test = "   a s d f  ";
	std::string result = FLTrimRight( test );
	EXPECT_TRUE( result == std::string( "   a s d f" ) );
}

TEST( Trim, String )
{
	std::string test = "   a s d f  ";
	std::string result = FLTrim( test );
	EXPECT_TRUE( result == std::string( "a s d f" ) );
}

TEST( TrimLeft, WString )
{
	std::wstring test = L"   a s d f  ";
	std::wstring result = FLTrimLeft( test );
	EXPECT_TRUE( result == std::wstring( L"a s d f  " ) );
}

TEST( TrimRight, WString )
{
	std::wstring test = L"   a s d f  ";
	std::wstring result = FLTrimRight( test );
	EXPECT_TRUE( result == std::wstring( L"   a s d f" ) );
}

TEST( Trim, WString )
{
	std::wstring test = L"   a s d f  ";
	std::wstring result = FLTrim( test );
	EXPECT_TRUE( result == std::wstring( L"a s d f" ) );
}
