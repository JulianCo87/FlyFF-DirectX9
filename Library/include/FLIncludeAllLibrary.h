
#ifndef	__FLINCLUDEALLLIBRARY_H__
#define	__FLINCLUDEALLLIBRARY_H__

#if	!defined( _MT )
	#error	Please use the /MT(d) or /MD(d) switch
#endif

#ifdef	_DLL
	#ifdef	_DEBUG
		//#pragma comment( lib, "FLBugtrapDlld.lib" )
		#pragma comment( lib, "FLCommonDlld.lib" )
		#pragma comment( lib, "FLCryptoDlld.lib" )
		#pragma comment( lib, "FLDumpDlld.lib" )
		#pragma comment( lib, "FLLoggingDlld.lib" )
		#pragma comment( lib, "FLMemoryDlld.lib" )
		#pragma comment( lib, "FLNetworkDlld.lib" )
		#pragma comment( lib, "FLUIDlld.lib" )
	#else
	//	#pragma comment( lib, "FLBugtrapDll.lib" )
		#pragma comment( lib, "FLCommonDll.lib" )
		#pragma comment( lib, "FLCryptoDll.lib" )
		#pragma comment( lib, "FLDumpDll.lib" )
		#pragma comment( lib, "FLLoggingDll.lib" )
		#pragma comment( lib, "FLMemoryDll.lib" )
		#pragma comment( lib, "FLNetworkDll.lib" )
		#pragma comment( lib, "FLUIDll.lib" )
	#endif
#else
	#ifdef	_DEBUG
	//	#pragma comment( lib, "FLBugtrapd.lib" )
		#pragma comment( lib, "FLCommond.lib" )
		#pragma comment( lib, "FLCryptod.lib" )
		#pragma comment( lib, "FLDumpd.lib" )
		#pragma comment( lib, "FLLoggingd.lib" )
		#pragma comment( lib, "FLMemoryd.lib" )
		#pragma comment( lib, "FLNetworkd.lib" )
		#pragma comment( lib, "FLUId.lib" )
	#else
		//#pragma comment( lib, "FLBugtrap.lib" )
		#pragma comment( lib, "FLCommon.lib" )
		#pragma comment( lib, "FLCrypto.lib" )
		#pragma comment( lib, "FLDump.lib" )
		#pragma comment( lib, "FLLogging.lib" )
		#pragma comment( lib, "FLMemory.lib" )
		#pragma comment( lib, "FLNetwork.lib" )
		#pragma comment( lib, "FLUI.lib" )
	#endif
#endif


#endif