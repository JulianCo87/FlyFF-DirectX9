
#include "StdAfx.h"
#include "FLFlyffConfig.h"
#include "FLFileEncryption.h"
#include <common/FLStdFile.h>
#include "lang.h"
#ifdef __CLIENT
#include <lang.h>
#include <algorithm>
#endif // __CLIENT

FLFlyffConfig::FLFlyffConfig()
:	m_language( NULL_ID )
,	m_code_page( 0 )
,	m_use_bank_password_at_login( false )
,	m_port_cache_server( 0 )
,	m_connect_account_db( false )
,	m_connect_log_db( false )
,	m_encrypt_password( false )
,	m_limit_shout_tick( 0 )
,	m_limit_shout_count( 0 )
,	m_allow_multi_execution( false )
{
}

FLFlyffConfig::~FLFlyffConfig()
{
}

FLFlyffConfig*	FLFlyffConfig::GetInstance()
{
	static FLFlyffConfig xFlyffConfig;
	return & xFlyffConfig;
}

bool	FLFlyffConfig::LoadFlyffConfig( const TCHAR* )
{
#if 0
	FILE* pFile = FLStdFileOpen( pFileName, "rt" );
	if( pFile == NULL ) {
		FLERROR_LOG( PROGRAM_NAME, _T( "[ FAILED FILE OPEN : \"%s\" ]" ), pFileName );
		FLASSERT( 0 );
		return false;
	}

	TCHAR szBuffer[512] = { 0 };
	if( _fgetts( szBuffer, _countof( szBuffer ), pFile ) == NULL ) {
		FLERROR_LOG( PROGRAM_NAME, _T( "[ EMPTY CONFIG FILE ]" ) );
		FLASSERT( 0 );
		fclose( pFile );
		return false;
	}

	do {
		TCHAR szOut[512] = { 0 };
		FLFileEncryption::GetInstance()->Decrypt( szBuffer, szOut );

		TCHAR szToken[2][512] = { 0 };
#if	defined(_MSC_VER) && (_MSC_VER >= 1400)
		_stscanf_s( szOut, "%s %s", szToken[0], _countof( szToken[0] ), szToken[1], _countof( szToken[1] ) );
#else
		_stscanf( szOut, "%s %s", szToken[0], szToken[1] );
#endif
		if( _tcscmp( szToken[0], "LANG" ) == 0 ) {
			m_language = _tstoi( szToken[1] );
			FLINFO_LOG( PROGRAM_NAME, _T( "[ LANGUAGE : %d ]" ), m_language );
		}
		else if( _tcscmp( szToken[0], "CODEPAGE" ) == 0 ) {
			m_code_page	= static_cast<WORD>( _tstoi( szToken[1] ) );
		}
		else if( _tcscmp( szToken[0], "2ND_PASSWORD" ) == 0 ) {
			m_use_bank_password_at_login	= _tstoi( szToken[1] ) != 0;
		}
		else if( _tcscmp( szToken[0], "CERTIFIER_IP" ) == 0 ) {
			m_main_server_certifier_ip	= szToken[1];
			m_certifier_ip	= szToken[1];
		}
		else if( _tcscmp( szToken[0], "CERTIFIER_IP_TEST" ) == 0 ) {
			m_test_server_certifier_ip.push_back( szToken[1] );
		}
		else if( _tcscmp( szToken[0], "NPROTECT_FILE_NAME" ) == 0 ) {
			m_main_server_nprotect_config_file_name	= szToken[1];
			m_nprotect_config_file_name	= szToken[1];
		}
		else if( _tcscmp( szToken[0], "NPROTECT_FILE_NAME_TEST" ) == 0 ) {
			m_test_server_nprotect_config_file_name	= szToken[1];
		}
		else if( _tcscmp( szToken[0], "ENCRYPTPWD" ) == 0 ) {
			m_encrypt_password	= _tstoi( szToken[1] ) != 0;
		}
		else if( _tcscmp( szToken[0], "CACHE_PORT" ) == 0 ) {
			m_port_cache_server	= static_cast<u_short>( _tstoi( szToken[1] ) );
		}
		else if( _tcscmp( szToken[0], "SHOUTLIMIT_SECOND" ) == 0 ) {
			m_limit_shout_tick	= static_cast<DWORD>( _tstoi( szToken[1] ) );
		}
		else if( _tcscmp( szToken[0], "SHOUTLIMIT_COUNT" ) == 0 ) {
			m_limit_shout_count	= _tstoi( szToken[1] );
		}
		else if( _tcscmp( szToken[0], "CONNECT_ACCOUNTDB" ) == 0 ) {
			m_connect_account_db	= _tstoi( szToken[1] ) != 0;
		}
		else if( _tcscmp( szToken[0], "CONNECT_LOGDB" ) == 0 ) {
			m_connect_log_db	= _tstoi( szToken[1] ) != 0;
		}
		else if( _tcscmp( szToken[0], "MULTI_EXECUTION" ) == 0 ) {
			m_allow_multi_execution	= _tstoi( szToken[1] ) != 0;
		}
		else {
			__noop;
		}

		memset( szBuffer, 0, sizeof( TCHAR ) * 512 );

	} while ( _fgetts( szBuffer, _countof( szBuffer ), pFile ) != NULL );

	fclose( pFile );

	/************************************************************************/
	/*
	QA 테스트 서버 IP 목록. QA팀은 config.ini 파일을 바꿔가며 한 장비 안에서 전국가 테스트를 하기 때문에
	QA 테스트 서버 IP 정보는 config.ini 에 넣지 않고 하드코딩 함.
	*/
	m_test_server_certifier_ip.push_back( _T( "172.16.100.16" ) );
	m_test_server_certifier_ip.push_back( _T( "172.16.100.17" ) );
	m_test_server_certifier_ip.push_back( _T( "172.16.100.20" ) );
	m_test_server_certifier_ip.push_back( _T( "172.16.100.21" ) );
	/************************************************************************/
#else
	this->SetServerIP("152.89.239.201", 15400);
#endif
	return true;
}

bool	FLFlyffConfig::IsFWCCertifierIP() const
{
	// 추후 FWC 개최될 시 해당 국가의 IP 수정 후 적용 하시기 바랍니다.
// 	if( _tcscmp( m_certifier_ip.c_str(), "60.199.180.11" ) == 0				// 대만
// 		|| _tcscmp( m_certifier_ip.c_str(), "195.59.138.28" ) == 0			// 유럽
// 		|| _tcscmp( m_certifier_ip.c_str(), "173.245.92.227" ) == 0			// 미국
// 		|| _tcscmp( m_certifier_ip.c_str(), "173.245.92.230" ) == 0			// 스패니쉬
// 		|| _tcscmp( m_certifier_ip.c_str(), "203.195.98.206" ) == 0			// 태국
// 		|| _tcscmp( m_certifier_ip.c_str(), "202.59.255.8" ) == 0			// 인도네시아
// 		|| _tcscmp( m_certifier_ip.c_str(), "203.216.218.74" ) == 0			// 일본
// 		|| _tcscmp( m_certifier_ip.c_str(), "200.98.210.242" ) == 0			// 브라질
// 		|| _tcscmp( m_certifier_ip.c_str(), "119.207.77.37" ) == 0			// 한국
// 		)
// 	{
// 		return true;
// 	}

	return false;
}

/************************************************************************/
#ifdef __CLIENT

void	FLFlyffConfig::DeactivatePasswordEncryption( const TCHAR* argument )
{
	if( GetMainLanguage() == LANG_KOR && _tcsicmp( argument, "localhost" ) == 0 ) {
		m_encrypt_password = false;
		return;
	}

	if( GetMainLanguage() == LANG_JAP && _tcsicmp( argument, "hangame.co.jp" ) == 0 ) {
		m_encrypt_password = false;
		return;
	}
}

bool	FLFlyffConfig::ModifyCertifierIP( const std::string & certifier_ip )
{
#ifndef __INTERNALSERVER
	if( !IsTestServerCertifierIP( certifier_ip ) ) {
		return false;
	}
#endif // !__INTERNALSERVER

	m_certifier_ip				= certifier_ip;
	m_nprotect_config_file_name	= m_test_server_nprotect_config_file_name;

	return true;
}

bool	FLFlyffConfig::IsTestServerCertifierIP( const std::string & certifier_ip ) const
{
	return find( m_test_server_certifier_ip.begin(), m_test_server_certifier_ip.end(), certifier_ip ) != m_test_server_certifier_ip.end();
}
#endif // __CLIENT
/************************************************************************/


void	FLFlyffConfig::SetServerIP(const char* szIp, const u_short nPort) {
	m_language = LANG_FRE;
	m_code_page = 1252;
	m_use_bank_password_at_login = 0;
	m_main_server_certifier_ip = szIp;
	m_certifier_ip = szIp;
	m_nprotect_config_file_name = "FlyffEU";
	m_test_server_nprotect_config_file_name = "FlyffEUTest";
	m_encrypt_password = true;
	m_port_cache_server = nPort;
	m_limit_shout_tick = 60000;
	m_limit_shout_count = 5;
	m_connect_account_db = true;
	m_connect_log_db = true;
	m_allow_multi_execution = true;
	m_test_server_certifier_ip.push_back(_T(szIp));
}