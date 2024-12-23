// AccountMgr.cpp: implementation of the CAccountMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AccountMgr.h"
#include <..\Resource\lang.h>

//////////////////////////////////////////////////////////////////////

UINT _HashKey( const char* key )
{
	UINT nHash = 0;
	while( *key )
		nHash = (nHash<<5) + nHash + *key++;
	return nHash;
}

//////////////////////////////////////////////////////////////////////

CAccountMgr::CAccountMgr()
{
	//////////////////////////////////////////////////////////////////////////
	//	BEGIN100616	Data Initialize
	m_cache.clear();
	//	END100616	Data Initialize
	//////////////////////////////////////////////////////////////////////////
}

CAccountMgr::~CAccountMgr()
{
	list< ACCOUNT_CACHE* >::iterator it;
	for( it = m_cache.begin(); it != m_cache.end(); ++it )
		SAFE_DELETE( *it );
}

//////////////////////////////////////////////////////////////////////

ACCOUNT_CHECK CAccountMgr::Check( const AUTH_INFO & kAuthInfo )
{
	time_t	tmCur = time( NULL );

	// ĳ������ ã�´�.
	list< ACCOUNT_CACHE* >::iterator it;
	for( it = m_cache.begin(); it != m_cache.end(); ++it )
	{
		ACCOUNT_CACHE* pInfo = *it;
		if( pInfo->m_dwIP == kAuthInfo.dwIP && _tcscmp( pInfo->szAccount, kAuthInfo.AccountInfo.szAccount ) == 0 )
		{
			ACCOUNT_CHECK result = CHECK_OK;

			if( pInfo->m_nError >= 3 )
			{
				time_t nSec = tmCur - pInfo->m_tmError;
				if( nSec <= (15 * 60) )
					result = CHECK_3TIMES_ERROR;
			}
			else if( pInfo->m_nError >= 1 && g_xFlyffConfig->GetMainLanguage() != LANG_KOR )
			{
				time_t nSec = tmCur - pInfo->m_tmError;
				if( nSec <= 15 )
					result = CHECK_1TIMES_ERROR;
			}

			m_cache.erase( it );
			m_cache.push_front( pInfo );		// ���� ���� ������ 
			return result;
		}
	}
	
	// MAX���� ũ�� - LRU�� ���� 
	//     or       - new�� �ִ´�. 
	ACCOUNT_CACHE* pInfo = NULL;
	if( m_cache.size() < 3 )
		pInfo = new ACCOUNT_CACHE;
	else
	{
		//////////////////////////////////////////////////////////////////////////
		// 15���� ������ ���� ĳ�ø� ������ ���ɼ��� �ִ�.
		// �䱸������ ��Ȯ�ϰ� �������� �ʴ´�. �ǵ������� �߸𸣰ڴ�.
		pInfo = m_cache.back();					// ���� �� ���� ���� �� �ڿ� ���� �ִ�.
		m_cache.pop_back();
	}

	pInfo->m_nError  = 0;
	pInfo->m_dwIP    = kAuthInfo.dwIP;
	pInfo->m_tmError = tmCur;
	FLStrcpy( pInfo->szAccount, _countof( pInfo->szAccount ), kAuthInfo.AccountInfo.szAccount );

	m_cache.push_front( pInfo );

	return CHECK_OK;
}

void	CAccountMgr::IncErrorCount()
{
	if( g_xFlyffConfig->IsFWCCertifierIP() == true )
	{
		return;
	}
	// ������ �� Ŭ���̾�Ʈ�� m_cache�� �� ó���� �ִٴ� ������ ����ִ�.
	// ������� �Ѱ��� AccountMgr�� �����ϱ� ������ �̻����.
	if( m_cache.empty() == false )
	{
		ACCOUNT_CACHE* pInfo = m_cache.front();		// MRU

		++pInfo->m_nError;
		pInfo->m_tmError = time( NULL );
	}
}

void	CAccountMgr::ResetErrorCount()
{
	// ������ �� Ŭ���̾�Ʈ�� m_cache�� �� ó���� �ִٴ� ������ ����ִ�.
	// ������� �Ѱ��� AccountMgr�� �����ϱ� ������ �̻����.
	if( m_cache.empty() == false )
	{
		ACCOUNT_CACHE* pInfo = m_cache.front();		// MRU

		pInfo->m_nError = 0; 
		pInfo->m_tmError = time( NULL );
	}
}
