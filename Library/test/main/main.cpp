
#include <gtest/gtest.h>


// gtest
#ifdef	_DLL
	#ifdef	_DEBUG
		#pragma comment( lib, "gtestDlld.lib" )
	#else
		#pragma comment( lib, "gtestDll.lib" )
	#endif
#else
	#ifdef	_DEBUG
		#pragma comment( lib, "gtestd.lib" )
	#else
		#pragma comment( lib, "gtest.lib" )
	#endif
#endif

// library
#include <FLIncludeAllLibrary.h>

#include <dump/FLDumpInstaller.h>


int	main( int argc, char ** argv )
{
	FLDumpInstaller::Install();
//	FLDumpInstaller::InstallFLAssertHooker();

//	FLASSERT( false );

	testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}

