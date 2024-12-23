

#include <logging/FLLogPubLoggingDialog.h>
#include <ui/FLUIMessageLoop.h>
#include <dump/FLDumpInstaller.h>

#include <shlwapi.h>

#include <common/FLStringFunction.h>

#pragma comment( lib, "shlwapi.lib" )


// library
#include <FLIncludeAllLibrary.h>


int	main( int argc, char** argv )
{
	(argc);
	(argv);
	FLLogPubLoggingDialog cLogPubDialog;

	FLDumpInstaller::Install();

	TCHAR szResDllPath[MAX_PATH] = { 0, };
	::GetModuleFileName( NULL, szResDllPath, _countof( szResDllPath ) );
	::PathRemoveFileSpec( szResDllPath );
	::PathAppend( szResDllPath, _T( "\\FLResCommon.dll" ) );

	if( cLogPubDialog.Create( _T( "test" ), NULL, szResDllPath ) == false )
	{
		TCHAR szMessage[1024] = { 0, };
		FLSPrintf( szMessage, _countof( szMessage ), _T( "Resource Dll load failed.\r\n%s" ), szResDllPath );
		::MessageBox( NULL, szMessage, _T( "Error" ), MB_OK );
		return 0;
	}

	cLogPubDialog.Show();

	FLUIMessageLoop cMessageLoop;
	return cMessageLoop.Run();
}

