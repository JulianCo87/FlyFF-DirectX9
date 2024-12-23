#include "stdafx.h"
#include "dbmanager.h"

#include <afxinet.h>

#define	s_sUrl	"spe.gpotato.eu"

#include "dpcertifier.h"
#include "dpaccountclient.h"
#include "AccountMgr.h"
#include <..\Resource\Lang.h>

extern	CDPCertifier		g_dpCertifier;
extern	CDPAccountClient	g_dpAccountClient;

extern	CDbManager	g_DbManager;
extern	HANDLE		s_hHandle;


u_int __stdcall GPotatoAuthWorker( LPVOID pParam )
{
	CAccountMgr mgr;
	CQuery query;

	if( FALSE == query.Connect( 3, g_DbManager.GetDBConnectString( LOGIN_DSN_NAME )
		, g_DbManager.GetDBConnectString( LOGIN_DB_ADMIN_ID )
		, g_DbManager.GetDBConnectString( LOGIN_PASSWORD ) ) )
	{
		AfxMessageBox( "can't connect to database : login" );
	}

	SetEvent( s_hHandle );

	HANDLE hIOCP	= g_DbManager.GetIOCPHandle( (int)pParam );
	DWORD dwBytesTransferred	= 0;
	LPDWORD lpCompletionKey		= NULL;
	LPDB_OVERLAPPED_PLUS pov	= NULL;

	while( 1 )
	{
		BOOL fOk = GetQueuedCompletionStatus( hIOCP,
										&dwBytesTransferred,
										(LPDWORD)&lpCompletionKey,
										(LPOVERLAPPED*)&pov,
										INFINITE );

		if( fOk == FALSE ) 
		{	
			FLASSERT( 0 );				// fatal error
			continue;
		}

		if( dwBytesTransferred == 0 )	// terminate
			return( 0 );

		switch( pov->nQueryMode )
		{
			case CERTIFY:
				g_DbManager.Certify( query, pov, mgr );
				break;
			case CLOSE_EXISTING_CONNECTION:
				g_DbManager.CloseExistingConnection( query, pov );
				break;
			case ACCOUNT_LOGOUT:
				break;
			default:
				{
					FLERROR_LOG( PROGRAM_NAME, _T( "pData->nQueryMode Invalid : [%d]" ), pov->nQueryMode );
				}
				break;
		}

		if( pov != NULL )
		{
			g_DbManager.m_pDbIOData->Free( pov );
			pov = NULL;
		}
	}
	return( 0 );
}

void CDbManager::Certify_GPotato( CQuery & query, LPDB_OVERLAPPED_PLUS pov, CAccountMgr & mgr )
{

	if( pov->AuthInfo.dwIP && pov->AuthInfo.AccountInfo.szAccount[0] != '\0' )
	{
		ACCOUNT_CHECK result	= CHECK_OK;
		result	= mgr.Check( pov->AuthInfo );
		switch( result )
		{
		case CHECK_1TIMES_ERROR:
			g_dpCertifier.SendError( ERROR_15SEC_PREVENT, pov->AuthInfo.dpId );
			return;
		case CHECK_3TIMES_ERROR:
			g_dpCertifier.SendError( ERROR_15MIN_PREVENT, pov->AuthInfo.dpId );
			return;
		}
	}

	char sAddr[16]	= { 0,};
	g_dpCertifier.GetPlayerAddr( pov->AuthInfo.dpId, sAddr, _countof( sAddr ) );

	//GPAUTH_RESULT r;
	//GetGPAuthResult( s_sUrl, 1, ( g_xFlyffConfig->GetMainLanguage() == LANG_GER? 1: 2 ), pov->AuthInfo.AccountInfo.szAccount, pov->AuthInfo.AccountInfo.szPassword, sAddr, r );

	//FLTRACE_LOG( PROGRAM_NAME, _T( "%d, %s, %s, %d, %d, %s" ), r.nResult, r.szResult, r.szGPotatoID, r.nGPotatoNo, r.bGM, r.szCheck );
	//if( r.nResult== 0 )
	{
		if( pov->AuthInfo.dwIP )
			mgr.ResetErrorCount();

		FLStrcpy( pov->AuthInfo.AccountInfo.szBak, _countof( pov->AuthInfo.AccountInfo.szBak ), pov->AuthInfo.AccountInfo.szAccount );
		//FLSPrintf( pov->AuthInfo.AccountInfo.szAccount, _countof(pov->AuthInfo.AccountInfo.szAccount), pov->AuthInfo.AccountInfo.szAccount);

		OnCertifyQueryOK( query, pov, pov->AuthInfo.AccountInfo.szAccount);

		SQLAddAccount( query, pov->AuthInfo.AccountInfo.szAccount, pov->AuthInfo.AccountInfo.szPassword, FALSE );
	}
	/*else if( r.nResult == WEBZEN_ACCOUNT_ID_CONVERTING_FAIL ) {
		g_dpCertifier.SendErrorCodeString( ERROR_WEBZEN_ACCOUNT_ID_CONVERTING_FAIL, r.szResult, pov->AuthInfo.dpId );
	}
	else if( r.nResult == WEBZEN_GDPR_AGREEMENT ) {
		g_dpCertifier.SendErrorCodeString( ERROR_WEBZEN_GDPR_AGREEMENT, r.szResult, pov->AuthInfo.dpId );
	}
	else if( r.nResult == WEBZEN_TERMS_AGREEMENT ) {
		g_dpCertifier.SendErrorCodeString( ERROR_WEBZEN_TERMS_AGREEMENT, r.szResult, pov->AuthInfo.dpId );
	}
	else if( r.nResult == WEBZEN_TRY_CONNECT_BLOCKING_AREA ) {
		g_dpCertifier.SendErrorCodeString( ERROR_WEBZEN_TRY_CONNECT_BLOCKING_AREA, r.szResult, pov->AuthInfo.dpId );
	}
	else
	{
		g_dpCertifier.SendErrorString( r.szResult, pov->AuthInfo.dpId );
	}*/
}

void CDbManager::CloseExistingConnection_GPotato( CQuery & query, LPDB_OVERLAPPED_PLUS pov )
{
	GPAUTH_RESULT r;

	char sAddr[16]	= { 0,};
	g_dpCertifier.GetPlayerAddr( pov->AuthInfo.dpId, sAddr, _countof( sAddr ) );

	GetGPAuthResult( s_sUrl, 1, ( g_xFlyffConfig->GetMainLanguage() == LANG_GER? 1: 2 ), pov->AuthInfo.AccountInfo.szAccount, pov->AuthInfo.AccountInfo.szPassword, sAddr, r );

	if( r.nResult == 0 )
	{
		FLSPrintf( pov->AuthInfo.AccountInfo.szAccount, _countof( pov->AuthInfo.AccountInfo.szAccount ), "%d", r.nGPotatoNo );

		g_dpAccountClient.SendCloseExistingConnection( pov->AuthInfo.AccountInfo.szAccount );

		SQLAddAccount( query, pov->AuthInfo.AccountInfo.szAccount, pov->AuthInfo.AccountInfo.szPassword, r.bGM );
	}
}

void	GetGPAuthResult( const char* szUrl, int nMode, int nGameMode, const char* sAccount, const char* sPassword, const char* sAddr, GPAUTH_RESULT & result )
{
	CInternetSession	session( _T( "GPAuth" ) );
	CHttpConnection*	pServer		= NULL;

	CHttpFile*	pFile	= NULL;
	char pszSendMsg[255]	= { 0,};
	//	char pszHeader[255]		= { 0,};
	CString strHeaders;
	int nReadSize	= 0;



	FLSPrintf( pszSendMsg, _countof( pszSendMsg ), "MODE=%c&GAMECODE=G00%d&GPOTATOID=%s&PASSWORD=%s&USERIP=%s", ( nMode == 1 ? 'L': 'O' ), nGameMode, sAccount, sPassword, sAddr );
	strHeaders	= "Content-Type: application/x-www-form-urlencoded";


	//////////////////////////////////////////////////////////////////////////
	//	BEGIN100519
	DWORD	dwStartTick	= 0;
	DWORD	dwEndTick	= 0;

	dwStartTick	= ::GetTickCount();
	//	END100519
	//////////////////////////////////////////////////////////////////////////

	try
	{
		pServer		= session.GetHttpConnection( szUrl );
		if( !pServer )
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "\"%s\" Connection Failed!" ), szUrl );
			goto DONE;
		}
		pFile	= pServer->OpenRequest( CHttpConnection::HTTP_VERB_POST, "/Account/Authentication.Aspx" );
		if( !pFile  )
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "OpenRequest Failed!" ) );
			goto DONE;
		}
		if( !( pFile->SendRequest( strHeaders, (LPVOID)pszSendMsg, lstrlen( pszSendMsg ) ) ) )
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "SendRequest Failed! -> Msg : %s" ), pszSendMsg ); 	
			goto DONE;
		}
	}
	catch( CInternetException* e )
	{
		TCHAR	szError[255]	= { 0,};
		e->GetErrorMessage( szError, 255 );

		FLERROR_LOG( PROGRAM_NAME, _T( "InternetException : %s" ), szError );

		goto DONE;
	}

	//	mulcom	BEGIN100524	HTTP Read ReadHuge
	//	nReadSize	= (int)pFile->ReadHuge( strResult.GetBuffer( (int)( pFile->GetLength() ) ), (unsigned int)( pFile->GetLength() ) );
	//mem_set( szResult, 0, sizeof( szResult ) );
	char szResult[4096] = {0, };
	nReadSize	= (int)pFile->Read( szResult, sizeof( szResult ) );


DONE:
	if( pFile )
	{
		pFile->Close();
		SAFE_DELETE( pFile );
		pFile	= NULL;
	}
	if( pServer )
	{
		pServer->Close();
		SAFE_DELETE( pServer );
		pServer		= NULL;
	}
	session.Close();

	if( nReadSize > 0 )
	{
		CString strResult = szResult;
		// 1. nFirst > 0
		// 2. len <= max
		// 3. if nResult	= 0, szResult = SUCCESS
		int nFirst	= strResult.Find( "\r\n", 0 );
		if( nFirst < 0 )
			nFirst	= strResult.Find( "\n\r", 0 );
		VALID_CRLF( nFirst, result, strResult )

			result.nResult	= atoi( (LPCSTR)strResult.Left( nFirst ) );
		nFirst	+= 2;	// \r\n
		int nLast	= strResult.Find( "\r\n", nFirst );
		if( nLast < 0 )
			nLast	= strResult.Find( "\n\r", nFirst );
		VALID_CRLF( nLast, result, strResult )
			VALID_STR( nLast - nFirst, 255, result, strResult )

		//FLStrcpy( result.szResult, _countof( result.szResult ), (LPCSTR)strResult.Mid( nFirst, nLast - nFirst ) );

		//  [2/13/2015 ChangSeop.Kim]
		WCHAR szUnicode[256] = { 0 };
		TCHAR szAnsi[256] = { 0 };
		::MultiByteToWideChar( CP_UTF8, 0, strResult.Mid( nFirst, nLast - nFirst ), -1, szUnicode, _countof( szUnicode ) );
		::WideCharToMultiByte( CP_ACP, 0, szUnicode, -1, szAnsi, sizeof( szAnsi ), NULL, NULL );
		FLStrcpy( result.szResult, _countof( result.szResult ), szAnsi );
		//  [2/13/2015 ChangSeop.Kim]

		if( result.nResult == 0 && lstrcmp( result.szResult, "SUCCESS" ) )
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "Login Failed. : Account(%s), nResult(%d), szResult(%s), szGPotatoID(%s), nGPotatoNo(%d), szNickName(%s), bGM(%d), szCheck(%s)" )
				, sAccount, result.nResult, result.szResult, result.szGPotatoID, result.nGPotatoNo, result.szNickName, result.bGM, result.szCheck );

			result.nResult	= -1;
			*result.szResult	= '\0';
			//			FLTRACE_LOG( PROGRAM_NAME, (LPCTSTR)strResult );

			return;
		}

		nFirst	= nLast + 2;	// \r\n
		nLast	= strResult.Find( "\r\n", nFirst );
		if( nLast < 0 )
			nLast	= strResult.Find( "\n\r", nFirst );
		VALID_CRLF( nLast, result, strResult )
			VALID_STR( nLast - nFirst, 20, result, strResult )

			FLStrcpy( result.szGPotatoID, _countof( result.szGPotatoID ), (LPCSTR)strResult.Mid( nFirst, nLast - nFirst ) );
		nFirst	= nLast + 2;	// \r\n
		nLast	= strResult.Find( "\r\n", nFirst );
		if( nLast < 0 )
			nLast	= strResult.Find( "\n\r", nFirst );
		VALID_CRLF( nLast, result, strResult )

			result.nGPotatoNo	= atoi( (LPCSTR)strResult.Mid( nFirst, nLast - nFirst ) );
		nFirst	= nLast + 2;	// \r\n
		nLast	= strResult.Find( "\r\n", nFirst );
		if( nLast < 0 )
			nLast	= strResult.Find( "\n\r", nFirst );
		VALID_CRLF( nLast, result, strResult )
			VALID_STR( nLast - nFirst, 20, result, strResult )

			FLStrcpy( result.szNickName, _countof( result.szNickName ), (LPCSTR)strResult.Mid( nFirst, nLast - nFirst ) );

		nFirst	= nLast + 2;	// \r\n
		nLast	= strResult.Find( "\r\n", nFirst );
		if( nLast < 0 )
			nLast	= strResult.Find( "\n\r", nFirst );
		VALID_CRLF( nLast, result, strResult )

			result.bGM	= ( strResult.Mid( nFirst, nLast - nFirst ) == "Y" );
		nFirst	= nLast + 2;	// \r\n
		VALID_STR( strResult.GetLength() - nFirst, 254, result, strResult )

			FLStrcpy( result.szCheck, _countof( result.szCheck ), (LPCSTR)strResult.Right( strResult.GetLength() - nFirst ) );
	}
	else
	{
		result.nResult	= -1;
		FLStrcpy( result.szResult, _countof( result.szResult ), "Could not establish a connection with the server..." );
	}
	switch( result.nResult )
	{
	case 1000:
		FLStrcpy( result.szResult, _countof( result.szResult ), "Unexpected system error occurred." );
		break;
	case 2000:
		FLStrcpy( result.szResult, _countof( result.szResult ), "GPotato is running maintenance from 10 am t0 10 pm." );
		break;
	case 3000:
		FLStrcpy( result.szResult, _countof( result.szResult ), "Your account was blocked by GPOTATO" );
		break;
	}


	//////////////////////////////////////////////////////////////////////////
	//	BEGIN100519

	FLINFO_LOG( PROGRAM_NAME, _T( "Login Result : Account(%s), nResult(%d), szResult(%s), szGPotatoID(%s), nGPotatoNo(%d), szNickName(%s), bGM(%d), szCheck(%s)" )
		, sAccount, result.nResult, result.szResult, result.szGPotatoID, result.nGPotatoNo, result.szNickName, result.bGM, result.szCheck );

	dwEndTick	= ::GetTickCount();

	if( dwEndTick >= dwStartTick + SEC(10) )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "GetGPAuthResult Time Over 10 Sec, Tick[%d], Account[%s], Password[%s]" ), (int)( dwEndTick - dwStartTick ), sAccount, sPassword );
	}
	//	END100519
	//////////////////////////////////////////////////////////////////////////


	return;
}
