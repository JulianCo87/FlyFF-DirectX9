

#include <gtest/gtest.h>

#include <common/FLTextFile.h>


TEST( ReadText, TextFile )
{
	FLTextFile cWriteFile;
	FLTextFile cReadFile;

	EXPECT_TRUE( cWriteFile.Create( "read_test.txt" ) );
	EXPECT_TRUE( cWriteFile.WriteText( "asdfghjkl\r\n", 11 ) == 11 );

	EXPECT_TRUE( cWriteFile.Size() == 11 );

	EXPECT_TRUE( cReadFile.Open( "read_test.txt" ) );
	EXPECT_TRUE( cReadFile.Size() == cWriteFile.Size() );

	TCHAR szText[4] = { 0, };
	EXPECT_TRUE( cReadFile.ReadText( szText, _countof( szText ) ) == 3 );
	EXPECT_TRUE( ::strcmp( szText, "asd" ) == 0 );

	cWriteFile.Close();
	cReadFile.Close();
}

TEST( ReadLine, TextFile )
{
	FLTextFile cWriteFile;
	FLTextFile cReadFile;

	EXPECT_TRUE( cWriteFile.Create( "read_line_test.txt" ) );
	EXPECT_TRUE( cWriteFile.WriteText( "asdfghjkl\r\naaa\r\n\r\n", 18 ) == 18 );

	EXPECT_TRUE( cReadFile.Open( "read_line_test.txt" ) );

	TCHAR szText[20];
	EXPECT_TRUE( cReadFile.ReadLine( szText, _countof( szText ) ) == 9 );
	EXPECT_TRUE( ::strcmp( szText, "asdfghjkl" ) == 0 );


	EXPECT_TRUE( cReadFile.ReadLine( szText, _countof( szText ) ) == 3 );
	EXPECT_TRUE( ::strcmp( szText, "aaa" ) == 0 );

	// 아무 글자도 없는 라인
	EXPECT_TRUE( cReadFile.ReadLine( szText, _countof( szText ) ) == 0 );

	// EOF. 공백 라인과 구분이 안된다.
	EXPECT_TRUE( cReadFile.ReadLine( szText, _countof( szText ) ) == 0 );

	cWriteFile.Close();
	cReadFile.Close();
}
