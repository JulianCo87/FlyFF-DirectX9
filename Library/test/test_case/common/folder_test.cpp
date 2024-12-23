
#include <gtest/gtest.h>

#include <common/FLFileUtil.h>
#include <common/FLTextFile.h>


TEST( CreateFolder, Folder  )
{
	const TCHAR* szTestFolder = _T( "./test_temp/" );
	if( FLIsFolder( szTestFolder ) == false )
	{
		EXPECT_TRUE( FLCreateFolder( szTestFolder ) );
	}
}

TEST( DeleteFolder, Folder )
{
	const TCHAR* szTestEmptyFolder = _T( "./test_temp/empty/" );
	const TCHAR* szTestNotEmptyFolder = _T( "./test_temp/notempty/" );

	EXPECT_TRUE( FLCreateFolder( szTestEmptyFolder ) );
	EXPECT_TRUE( FLCreateFolder( szTestNotEmptyFolder ) );
	EXPECT_TRUE( FLIsFolder( szTestEmptyFolder ) );
	EXPECT_TRUE( FLIsFolder( szTestNotEmptyFolder ) );

	const TCHAR* szTempFile = _T( "./test_temp/notempty/test_temp_file.txt" );

	FLTextFile cTempFile;
	EXPECT_TRUE( cTempFile.Create( szTempFile ) );
	cTempFile.WriteText( szTestNotEmptyFolder, ::_tcslen( szTestNotEmptyFolder ) );
	EXPECT_TRUE( cTempFile.Size() > 0 );
	cTempFile.Close();

	FLTextFile cValid;
	EXPECT_TRUE( cValid.Open( szTempFile ) );
	EXPECT_TRUE( cValid.Size() > 0 );
	cValid.Close();

	EXPECT_TRUE( FLDeleteFolder( _T( "./test_temp/" ) ) );
	EXPECT_FALSE( FLIsFolder( szTestEmptyFolder ) );
	EXPECT_FALSE( FLIsFolder( szTestNotEmptyFolder ) );

	// 존재하지 않는 폴더 지우기
	EXPECT_FALSE( FLDeleteFolder( _T( "./test_temp/" ) ) );
}

TEST( DeleteFile, File )
{
	const TCHAR* szTempFile = _T( "./delete_file_test.txt" );

	FLTextFile cTempFile;
	EXPECT_TRUE( cTempFile.Create( szTempFile ) );
	cTempFile.Close();

	EXPECT_TRUE( FLDeleteFile( szTempFile ) );

	FLTextFile cReadOnlyFile;
	EXPECT_TRUE( cReadOnlyFile.Create( szTempFile, GENERIC_WRITE, FILE_SHARE_READ, FILE_ATTRIBUTE_READONLY ) );
	cReadOnlyFile.Close();

	EXPECT_TRUE( FLDeleteFile( szTempFile ) );
}
