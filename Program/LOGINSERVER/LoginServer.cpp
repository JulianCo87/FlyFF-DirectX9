// LoginServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#ifdef IDC_STATIC
#undef IDC_STATIC
#endif
#include "resource.h"
#include "dploginsrvr.h"
#include "dpdatabaseclient.h"
#include "dpcoreclient.h"

#include <logging/FLMyTrace.h>
#include <ui/FLUIMessageLoop.h>

#include "user.h"


//#include <vld/vld.h>
#if 0
#include "ggsrv.h"

#ifdef	_DLL
	#pragma comment( lib, "ggsrv30lib_x86_MD.lib" )
#else
	#pragma comment( lib, "ggsrv30lib_x86_MT.lib" )
#endif
#endif


extern	CDPLoginSrvr		g_dpLoginSrvr;
extern	CDPDatabaseClient	g_dpDBClient;
extern	CDPCoreClient		g_dpCoreClient;
extern	CUserMng			g_UserMng;


#define MAX_LOADSTRING 100

FLMyTrace	g_MyTrace;
HINSTANCE	hInst;	// current instance
TCHAR		szTitle[MAX_LOADSTRING];	// The title bar text
TCHAR		szWindowClass[MAX_LOADSTRING];	// The title bar text
HWND		hMainWnd;


ATOM	MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL	InitInstance(HINSTANCE, int);
void	ExitInstance( void );
BOOL	Script( LPCSTR lpszFileName );

//////////
#define NPROTECT_UPDATE_TIMER	1234
#define XTRAP_UPDATE_TIMER		1235

// void __stdcall CS3LogCallback( int nLogType, char *szLog )
// {
// 	FLERROR_LOG( _T( "NPROTECT" ), _T( "Type:(%d), Message:(%s)" ), nLogType, szLog );
// 	// 게임가드 프로토콜 버전, 내부버전의업데이트가발생했을때호출됩니다. (호출빈도 낮음)
// 	if( nLogType == LOG_UPDATE ) {
// 		FLERROR_LOG( _T( "NPROTECT" ), _T( "LOG_UPDATE : %s" ), szLog );
// 	}
// 	// 초기화상황, 일반적인로그, 에러발생등의로그가발생했을때호출됩니다. (호출빈도 보통)
// 	else if( nLogType == LOG_NORMAL ) {
// 		FLERROR_LOG( _T( "NPROTECT" ), _T( "LOG_NORMAL : %s" ), szLog );
// 	}
// 	// 모든 유저의 패킷 정보가 남게 됩니다. (호출빈도,양 매우 높음)
// 	else if( nLogType == LOG_DEBUG ) {
// 		FLERROR_LOG( _T( "NPROTECT" ), _T( "LOG_DEBUG : %s" ), szLog );
// 	}
// }
// 
// void __stdcall CS3UpdateInfoCallback( int nUpdateType, int nBefore, int nAfter )
// {
// 	if( nUpdateType == UPDATE_PROTOCOL ) {
// 		FLERROR_LOG( _T( "NPROTECT_UPDATE" ), _T( "UPDATE_PROTOCOL : [%ld] -> [%ld]" ), nBefore, nAfter ); 
// 	}
// 	else if( nUpdateType == UPDATE_GGVERSION ) {
// 		FLERROR_LOG( _T( "NPROTECT_UPDATE" ), _T( "UPDATE_GGVERSION : [%ld] -> [%ld]" ), nBefore, nAfter ); 
// 	}
// }

// GGAUTHS_API void NpLog(int mode, char* msg)
// {
// 	if(mode & (NPLOG_DEBUG | NPLOG_ERROR)) //Select log mode. 
// 	{
// 		FLERROR_LOG( _T( "NPROTECT" ), msg );
// 	}
// };
// 
// GGAUTHS_API void GGAuthUpdateCallback(PGG_UPREPORT report)
// {
// 	FLERROR_LOG( _T( "NPROTECT" ), _T( "GGAuth version update [%s] : [%ld] -> [%ld]" ),
// 		report->nType==1?"GameGuard Ver":"Protocol Num", 
// 		report->dwBefore, report->dwNext); 
// };
//////////




//////////////////////////////////////////////////////////////////////////
//	BEGINTEST100111	FFL_DUMP
#include <dump/FLDumpInstaller.h>
#include "../_CommonDefine/FLCommonDumper.h"

//#include <bugtrap/FLBugTrapper.h>

#include <FLIncludeAllLibrary.h>
//	ENDTEST100111	FFL_DUMP
//////////////////////////////////////////////////////////////////////////



#include "../_Common/FLXTrap_Server.h"



int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE /*hPrevInstance*/,
                     LPSTR     /*lpCmdLine*/,
                     int       nCmdShow)
{
	
	//FLXTrap_Server::GetInstance();


	//////////////////////////////////////////////////////////////////////////
	//	BEGINTEST100111	FFL_DUMP
 	FLDumpInstaller::Install( FLCommonDumper::GetInstance() );

#if	defined(FLASSERT_ENABLE) && defined(FLASSERT_HOOKER_ENABLE)
	FLDumpInstaller::InstallFLAssertHooker();
#endif

// #ifdef __MAINSERVER
// 	FLBugTrapper::Install( PROGRAM_NAME );
// #else
// 	FLBugTrapper::Install( PROGRAM_NAME, true );
// #endif // __MAINSERVER
	//	ENDTEST100111	FFL_DUMP
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// mirchang_100914 load config file

	if( g_xFlyffConfig->LoadFlyffConfig( "config.ini" ) == false )
	{
		return FALSE;
	}

	// mirchang_100914 load config file
	//////////////////////////////////////////////////////////////////////////

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LOGINSERVER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	int nRet = 0;
	if( InitInstance( hInstance, nCmdShow ) == TRUE )
	{
		FLUIMessageLoop kMessageLoop;
		nRet = kMessageLoop.Run();
	}

	ExitInstance();

#if	defined(FLASSERT_ENABLE) && defined(FLASSERT_HOOKER_ENABLE)
	FLDumpInstaller::UninstallFLAssertHooker();
#endif

	FLDumpInstaller::Uninstall();
	FLLOG_FLUSH();

	return nRet;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_LOGINSERVER);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_LOGINSERVER;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	if( !Script( "LoginServer.ini" ) )
		return FALSE;

	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hMainWnd	= hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if( !hWnd )
	{
		return FALSE;
	}

	SetTimer( hWnd, 0, 60 * 1000, NULL );

	int x = 880, y = 606;
	SetWindowPos( hWnd, NULL, x, y, 400, 416, SWP_SHOWWINDOW );

	g_MyTrace.Initialize( hWnd, "SquireD", RGB( 0x00, 0x00, 0x00 ), RGB( 0xff, 0xff, 0xff ) );

	CTime time	= CTime::GetCurrentTime();
	g_MyTrace.Add( 0, FALSE, "%s", time.Format( "%Y/%m/%d %H:%M:%S" ) );
	g_MyTrace.AddLine( '\0' );

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	FLLOG_ADD_NETWORK( NETWORK_LOG_NAME );
	if( InitializeNetLib() == FALSE )
		return FALSE;

	if( !g_dpDBClient.ConnectToServer( g_lpDBAddr, PN_DBSRVR_0, FLStreamProtocol::GetInstance() ) )
	{
		return FALSE;
	}

	if( !g_dpCoreClient.ConnectToServer( g_lpCoreAddr, PN_CORESRVR + 2, FLStreamProtocol::GetInstance() ) )
	{
		return FALSE;
	}
	g_dpCoreClient.QueryTickCount();	// block,

	if( !g_dpLoginSrvr.StartServer( g_szClientBindIP, PN_LOGINSRVR, FLXORProtocol::GetInstance(), FALSE ) )
		return FALSE;

	//////////////////////////////////////////////////////////////////////////
	//	BEGIN100616
#if 0
	if( g_dpLoginSrvr.IsUseGameGuard() == true )
	{
//		DWORD dw = InitGameguardAuth( NULL, 5, true, 0x3 );
//		DWORD dw = InitGameguardAuth( NULL, 1000, true, 0 );
//		DWORD dw = InitGameguardAuth( NULL, 1000, true, 0x03 );
		const UINT32 dw = InitCSAuth3( "./" );  
		if( dw != ERROR_SUCCESS  )
		{
			char s[128];
			FLSPrintf( s, _countof( s ), "InitGameguardAuth//%d - check ggauth*.dll", dw );
			MessageBox( hWnd, s, "", MB_OK );
			return FALSE;
		}
		SetTimer( hWnd, 1, SEC( 1 ), NULL );
		SetTimer( hWnd, NPROTECT_UPDATE_TIMER, MIN( 5 ), NULL );	//5분 주기 권장
	}
	//	END100616
	//////////////////////////////////////////////////////////////////////////

	if( FLXTrap_Server::GetInstance().IsActive() == TRUE )
	{
		SetTimer( hWnd, XTRAP_UPDATE_TIMER, FLXTrap_Server::XTRAP_UPDATE_INTERVAL, NULL );
	}
#endif


	return TRUE;
}

void ExitInstance( void )
{
	//////////////////////////////////////////////////////////////////////////
	//	BEGIN100616
#if 0
	if( g_dpLoginSrvr.IsUseGameGuard() == true )
	{
		CloseCSAuth3();
		//CleanupGameguardAuth();
	}
#endif
	//	END100616
	//////////////////////////////////////////////////////////////////////////

	g_dpLoginSrvr.DeleteDPObject();
	g_dpCoreClient.DeleteDPObject();
	g_dpDBClient.DeleteDPObject();
	g_UserMng.Free();
	MEMPOOLER_DESTROY( CUser );

	UninitializeNetLib();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId;//, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;


	//////////////////////////////////////////////////////////////////////////
	//	BEGIN100616
	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			//wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				case IDM_DO_UPDATE_GAMEGUARD_AUTH_DLL:
					{
						//AddAuthProtocol( "./ggauth01.dll" );
						break;
					}
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_TIMER:
			{
				const WPARAM wTimerID	= wParam;
				switch( wTimerID )
				{
					case	0:
						{
							g_UserMng.DestroyAbnormalPlayer();
						}
						break;
					case	1:
						{
#if 0
							if( g_dpLoginSrvr.IsUseGameGuard() == true )
							{
								g_UserMng.CheckAll();
							}
#endif
						}
						break;
#if 0
					case	NPROTECT_UPDATE_TIMER:
						{
							//GGAuthUpdateTimer();
						}
						break;
					case	XTRAP_UPDATE_TIMER:
						{
							const DWORD	dwCurrTick		= ::GetTickCount();
							if( FLXTrap_Server::GetInstance().IsActive() == TRUE && FLXTrap_Server::GetInstance().IsUpdateTime( dwCurrTick ) == TRUE )
								g_UserMng.XTrapUpdate();
						}	
						break;
#endif
				}
				break;
			}
		case WM_PAINT:
			{
				hdc = BeginPaint(hWnd, &ps);
				g_MyTrace.Paint( hdc );
				EndPaint(hWnd, &ps);
			}
			break;
		case WM_DESTROY:
			{
				KillTimer( hMainWnd, 0 );

				if( g_dpLoginSrvr.IsUseGameGuard() == true )
				{
					KillTimer( hMainWnd, 1 );
				}

				PostQuitMessage(0);
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   //	END100616
   //////////////////////////////////////////////////////////////////////////

   return 0;
}

BOOL Script( LPCSTR lpszFileName )
{
	CScanner s;

	if( s.Load( lpszFileName ) )
	{
		s.GetToken();
		while( s.tok != FINISHED )
		{
			if( s.Token == ";" )
			{
				s.GetToken();
				continue;
			}
			else if( s.Token == "CLIENT_BIND_IP" )
			{
				s.GetToken();
				FLStrcpy( g_szClientBindIP, _countof( g_szClientBindIP ), s.Token );
			}
			else if( s.Token == "DB" )
			{
				s.GetToken();
				FLStrcpy( g_lpDBAddr, _countof( g_lpDBAddr ), s.Token );
			}
			else if( s.Token == "Core" )
			{
				s.GetToken();
				FLStrcpy( g_lpCoreAddr, _countof( g_lpCoreAddr ), s.Token );
			}
			else if( s.Token == "AddCache" )
			{
				s.GetToken();	// (
				s.GetToken();	// ip
				g_dpLoginSrvr.AddCahce( s.Token );
				s.GetToken();	// )
			}
#if 0
			else if( s.Token == "NPROTECT" )
			{
				if( FLXTrap_Server::GetInstance().IsActive() == TRUE )
				{
					FLERROR_LOG( PROGRAM_NAME, _T( "[ NPROTECT : XTRAP LOADED ]" ) );
					return FALSE;
				}

//				s.GetToken();
				//////////////////////////////////////////////////////////////////////////
				//	BEGIN100616
				g_dpLoginSrvr.SetUseGameGuard();
				//	END100616
				//////////////////////////////////////////////////////////////////////////
			}
			else if( s.Token == "XTRAP" )
			{
				if( g_dpLoginSrvr.IsUseGameGuard() == TRUE )
				{
					FLERROR_LOG( PROGRAM_NAME, _T( "[ XTRAP : NPROTECT LOADED ]" ) );
					return FALSE;
				}
#ifndef _DEBUG
				FLXTrap_Server::GetInstance().SetActive( TRUE );
				const BOOL bRet		= FLXTrap_Server::GetInstance().Start();
				if( bRet == FALSE )
				{
					FLERROR_LOG( PROGRAM_NAME, _T( "[ FAIL - XTRAP START ]" ) );
					return FALSE;
				}
#endif //_DEBUG
			}
#endif
			s.GetToken();
		}

		return TRUE;
	}

	FLERROR_LOG( PROGRAM_NAME, _T( "Can't open file, %s." ), lpszFileName );
	return FALSE;
};