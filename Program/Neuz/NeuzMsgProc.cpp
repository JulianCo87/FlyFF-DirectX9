
#include "stdafx.h"

#include "Webbox.h"
#include <network/FLDPMng.h>
#include "WorldMap.h"
#include "msghdr.h"

#include "xUtil.h"

HANDLE g_hMutex = 0;
static HHOOK	g_hHook;

extern	LPTSTR WEB_ADDRESS_DEFAULT;
extern	LPTSTR WEB_POSTDATA;

#ifdef	_DEBUG
#include <logging/FLLogPubLoggingDialog.h>
FLLogPubLoggingDialog	g_cLogPubDialog;
#endif


/*
//////////////////////////////////////////////////////////////////////////
//	BEGINTEST100111	FFL_DUMP
#include <dump/FLDumpInstaller.h>
//	ENDTEST100111	FFL_DUMP
//////////////////////////////////////////////////////////////////////////
*/

#include <FLIncludeAllLibrary.h>

//X-Trap
#ifdef _USE_XTRAP
#include "XTrapLib.h"
#endif


LRESULT CALLBACK LowLevelKeyboardProc (INT nCode, WPARAM wParam, LPARAM lParam)
{
    // By returning a non-zero value from the hook procedure, the
    // message does not get passed to the target window
    KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *) lParam;
    BOOL bControlKeyDown = 0;
	
    switch (nCode)
    {
	case HC_ACTION:
        {
            // Check to see if the CTRL key is pressed
            bControlKeyDown = GetAsyncKeyState (VK_CONTROL) >> ((sizeof(SHORT) * 8) - 1);
            
            // Disable CTRL+ESC
            if (pkbhs->vkCode == VK_ESCAPE && bControlKeyDown)
                return 1;

			// Disable ALT+ESC
            if (pkbhs->vkCode == VK_ESCAPE && pkbhs->flags & LLKHF_ALTDOWN)
                return 1;
			if( pkbhs->vkCode == VK_LWIN || pkbhs->vkCode == VK_RWIN )
				return 1;
			
            break;
        }
		
	default:
		break;
    }
    return CallNextHookEx (g_hHook, nCode, wParam, lParam);
}

void InitGlobalVars( HINSTANCE hInstance )
{
	g_Neuz.m_bGPotatoAuth	= ( g_xFlyffConfig->GetMainLanguage() == LANG_GER || g_xFlyffConfig->GetMainLanguage() == LANG_FRE || g_xFlyffConfig->GetMainLanguage() == LANG_USA );

	InitWebGlobalVar();					// WEB_ADDRESS_DEFAULT, WEB_POSTDATA설정 
}

void DisableTaskSwitching( HINSTANCE hInstance )
{
#ifndef _DEBUG	
	OSVERSIONINFO versionInformation;
	versionInformation.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if( GetVersionEx( &versionInformation ) )
	{
		if( versionInformation.dwPlatformId == VER_PLATFORM_WIN32_NT )		// NT계열이면 키보드를 후킹하고
		{
			g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)::LowLevelKeyboardProc, hInstance, 0);
		} 
		else
		{
			// Disables task switching
			UINT nPreviousState;
			::SystemParametersInfo(SPI_SCREENSAVERRUNNING, 1, &nPreviousState, 0);		// 95, 98, Me면 API를 쓴다.
		}
	}
#endif	
}

BOOL ParseCmdLine( LPCTSTR lpCmdLine )
{
	TCHAR szArg1[64] = {0, }; 
	TCHAR szArg2[64] = {0, }; 
	TCHAR szArg3[64] = {0, }; 

	//////////////////////////////////////////////////////////////////////////
	// mirchang_100727 일본 한게임

	TCHAR szArg4[MAX_ENCRYPT_PWD] = {0, }; 

	// mirchang_100727 일본 한게임
	//////////////////////////////////////////////////////////////////////////


	TCHAR szArg5[64] = {0, };
#if	defined(_MSC_VER) && (_MSC_VER >= 1400)
	_stscanf_s( lpCmdLine, "%s %s %s %s %s",
		szArg1, _countof( szArg1 ),
		szArg2, _countof( szArg2 ),
		szArg3, _countof( szArg3 ),
		szArg4, _countof( szArg4 ),
		szArg5, _countof( szArg5 ) );
#else
	_stscanf( lpCmdLine, "%s %s %s %s %s", szArg1, szArg2, szArg3, szArg4, szArg5 );
#endif
	if (strlen(szArg1) > 0 && strcmp(szArg1, "sunkist") != 0) {
		g_xFlyffConfig->SetServerIP(szArg1, 15400);
	}

#ifdef __MAINSERVER
#if 0
	if( _strcmpi( szArg1, "DeleteResourceFile" ) )
	{
		return FALSE;
	}
#endif
// 	if( g_xFlyffConfig->GetMainLanguage() == LANG_KOR )
// 	{
// 		if( strcmpi( szArg2, "localhost" ) == 0 )
// 		{
// 			g_Neuz.m_bEncryptPWD	= FALSE;
// 		}
// 		else
// 		{
// 			g_Neuz.m_bEncryptPWD	= TRUE;
// 		}
// 	}
#endif	// __MAINSERVER
	char	szPortal[LANG_MAX][3][100]	=
		{
			"localhost",	"",	"",					// LANG_KOR
			"",	"",	"",								// LANG_USA
			"hangame.co.jp",	"",	"",				// LANG_JAP
			"",	"",	"",								// LANG_CHI
			"flyffonline.ini3.co.th",	"",	"",		// LANG_THA
			"www.omg.com.tw/fff/", "", "",			// LANG_TWN
			"",	"",	"",								// LANG_GER
			"http://www.zoomby.net/",	"",	"",		// LANG_SPA
			"",	"",	"",								// LANG_FRE
			"",	"",	"",								// LANG_HK
			"",	"",	"",								// LANG_POR
			"",	"",	"",								// LANG_VTN
			"", "", "",								// LANG_RUS
			"", "", "",								// LANG_ID
			"", "", "",								// LANG_PHP
		};
	//	BEGINTODO::::::
	if( lstrlen( szPortal[g_xFlyffConfig->GetMainLanguage()][0] ) > 0
		&& lstrcmpi( szArg2, szPortal[g_xFlyffConfig->GetMainLanguage()][0] ) == 0 )
	{
		g_Neuz.m_bThroughPortal		= TRUE;

		g_xFlyffConfig->DeactivatePasswordEncryption( szArg2 );

		g_Neuz.SetAccountInfo( szArg3, szArg4, szArg5 );
	}

	return TRUE;
}

void EnableFloatException()
{
#if	defined(_MSC_VER) && (_MSC_VER >= 1400)
	unsigned int dwCurrent = 0;
	::_controlfp_s( &dwCurrent, 0, 0 );
	unsigned int dwNew = dwCurrent & ~(EM_OVERFLOW|EM_UNDERFLOW|EM_ZERODIVIDE|EM_DENORMAL);
	::_controlfp_s( NULL, dwNew, MCW_EM );
#else
	int cw = _controlfp( 0, 0 );
	cw &= ~(EM_OVERFLOW|EM_UNDERFLOW|EM_ZERODIVIDE|EM_DENORMAL);
	_controlfp( cw, MCW_EM );
#endif
}

BOOL InitApp()
{
	// Get Os Version
	OSVERSIONINFO versionInfo;
	versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	BOOL bGetOsVr = GetVersionEx( &versionInfo );

	// Get VGA Identifier
	LPDIRECT3D9 pD3D;
	D3DADAPTER_IDENTIFIER9 Identifier; 
	pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	HRESULT hres = pD3D->GetAdapterIdentifier( D3DADAPTER_DEFAULT, 0, &Identifier ); 
	
//	CString temp;
//	temp.Format("Os Ver : %d, VGA Vendor Id : %d", versionInfo.dwMajorVersion, Identifier.VendorId);
//	AfxMessageBox(temp, MB_OK);

	//ATI계열 카드의 Vista드라이버에서 Floating Point Exception이 발생하여 Vista & ATI가 아닐 경우만 Enable함
	if( bGetOsVr && hres == S_OK && versionInfo.dwMajorVersion != 6 && Identifier.VendorId != 4098 )
		EnableFloatException();

	SAFE_RELEASE(pD3D);

	// Set up the highest resolution timer we can manage
	TIMECAPS tc; 
	UINT uPeriod = (TIMERR_NOERROR == timeGetDevCaps(&tc, sizeof(tc))) ? tc.wPeriodMin : 1;  
	timeBeginPeriod( uPeriod ); 

	xSRand( timeGetTime() );	// 속도를 요하는 랜덤은 xRandom()을 쓰도록...
	srand( timeGetTime() );		// c내장 rand를 쓰고 싶다면 random()을 쓰도록...
 
	InitUPS();					// 애니메이션 타이머 초기화

	FLLOG_ADD_NETWORK( NETWORK_LOG_NAME );
	if( InitializeNetLib() == FALSE )
		return FALSE;

	LPCTSTR szCPU = GetCPUInfo();

	#ifndef _DEBUG
	{
		InitEH();
		FLERROR_LOG( PROGRAM_NAME, szCPU );
	}
	#endif

//gmpbigsun: 위치옮김
//	g_Neuz.LoadOption();		// Neuz.ini를 로딩 	

//	g_Neuz.BeginLoadThread();	// prj.OpenProject()
	return TRUE;
}

void SkipPatch()
{
	//gmpbigsun(20100701) : 투 클라이언트 막음 ( Window class id )
 	HANDLE hNeuz = FindWindow( _T("saida"), NULL );
	if( hNeuz )
	{
		if( g_xFlyffConfig->IsAbleMultiExecution() == false )
		{
			AfxMessageBox( "Already running Flyff client!!" );
			::PostMessage( g_Neuz.GetSafeHwnd(), WM_CLOSE, TRUE, 0 );
		}
	}

	//CreateMutex 버젼
	//g_hMutex = CreateMutex( NULL, TRUE, "Neuz" );
	//if( GetLastError( ) == ERROR_ALREADY_EXISTS )
	//{
	//	CloseHandle( g_hMutex );
	//	AfxMessageBox( "Already running Flyff client!!" );
	//	ExitProcess( 0 );
	//}

	//OpenMutex 버젼
	g_hMutex = OpenMutex( MUTEX_ALL_ACCESS, TRUE, "neuz" );
	if( g_hMutex == NULL )
	{
		g_hMutex = CreateMutex( NULL, TRUE, "neuz" );
		if( g_hMutex )
			ReleaseMutex( g_hMutex );
	}
	else
	{
		if( g_xFlyffConfig->IsAbleMultiExecution() == false )
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "Double Execution Error!" ) );
			::PostMessage( g_Neuz.GetSafeHwnd(), WM_CLOSE, TRUE, 0 );
		}
	}

	return;
}

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	//////////////////////////////////////////////////////////////////////////
	// mirchang_100914 load config file

	if( g_xFlyffConfig->LoadFlyffConfig( "config.ini" ) == false )
	{
		return FALSE;
	}

	// mirchang_100914 load config file
	//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
	afxTraceEnabled = TRUE;
#endif

	g_Option.Init();			//_SUN_DEFAULT_WINDOW
	g_Neuz.LoadOption();		// Neuz.ini를 로딩 	

#ifdef _USE_XTRAP
	FLERROR_LOG( PROGRAM_NAME, "LoadOption OK" );
#endif	

	SkipPatch();
#ifdef _USE_XTRAP
	FLERROR_LOG( PROGRAM_NAME, "SkipPatch OK" );
#endif	

	InitGlobalVars( hInstance );
#ifdef _USE_XTRAP
	FLERROR_LOG( PROGRAM_NAME, "InitGlobalVars OK" );
#endif	

	DisableTaskSwitching( hInstance );	
#ifdef _USE_XTRAP
	FLERROR_LOG( PROGRAM_NAME, "DisableTaskSwitching OK" );
#endif	

	if( ParseCmdLine( lpCmdLine ) == FALSE )
		return 0;

#ifdef _USE_XTRAP
	FLERROR_LOG( PROGRAM_NAME, "ParseCmdLine OK" );
#endif	

	InitApp();

#ifdef _USE_XTRAP
	FLERROR_LOG( PROGRAM_NAME, "InitAPP() OK" );
#endif

	if( FAILED( g_Neuz.Create( hInstance ) ) )
		return 0;

#ifdef _USE_XTRAP
	FLERROR_LOG( PROGRAM_NAME, "g_Neuz.Create() OK" );
#endif

#ifdef _USE_XTRAP
	XTrap_C_KeepAlive( );
#endif //_USE_XTRAP

#ifdef _USE_XTRAP
	FLERROR_LOG( PROGRAM_NAME, _T( "XTrap_C_KeepAlive OK" ) );
#endif

#ifdef	_DEBUG
	if( g_cLogPubDialog.Create( PROGRAM_NAME, g_Neuz.GetSafeHwnd(), _T( "FLResCommon.dll" ) ) == true )
	{
		FLLOG_ADD_PUBLISHER( PROGRAM_NAME, &g_cLogPubDialog );
// 		g_cLogPubDialog.Show();
// 		SetFocus( g_Neuz.GetSafeHwnd( ) );	//gmpbigsun
	}
#endif

#ifdef _USE_XTRAP
	FLERROR_LOG( PROGRAM_NAME, _T( "XTrap OK , here is g_neuz.Run() before" ) );
#endif

	INT nResult = g_Neuz.Run();

#ifdef	_DEBUG
	g_cLogPubDialog.Destroy();
	FLLOG_DEL_PUBLISHER( PROGRAM_NAME, &g_cLogPubDialog );
#endif

	FLLOG_FLUSH();

	return nResult;
}
