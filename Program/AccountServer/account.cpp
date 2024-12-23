#include "stdafx.h"
#include "account.h"
#include "dpsrvr.h"
#include "dpdbsrvr.h"
#include <MsgHdr.h>
#include "FLASDBManager.h"
#include "../_Common/FLServerDescription.h"

extern	CDPSrvr		g_dpSrvr;
extern	CDPDBSrvr	g_dpDbSrvr;

CAccountMng			g_AccountMng;
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
MEMPOOLER_IMPLEMENT( CAccount, 1024 );

CAccount::CAccount( LPCTSTR lpszAccount, DWORD dpid1, DWORD dpid2, BYTE cbAccountFlag, int fCheck, const CTime & tmBirthDate, const TCHAR* pAddress )
: m_tmBirthDate( tmBirthDate )
{
	FLStrcpy( m_lpszAccount, _countof( m_lpszAccount ), lpszAccount );
	m_dpid1				= dpid1;
	m_dpid2				= dpid2;
	m_cbAccountFlag		= cbAccountFlag;
	m_fRoute			= FALSE;
	m_dwIdofServer		= 0;
	m_dwPing			= timeGetTime();
	m_uIdofMulti		= NULL_ID;
	m_dwAuthKey			= 0;
	m_cbRef				= 0;

	m_dwBillingClass	= 0;
	m_TimeOverDays		= 0;
	m_nStatus			= ACCOUNT_STATUS_INITIAL;
	m_fCheck			= fCheck;
	m_cbLastOneLogon	= 'N';

	// BEGIN110520
	m_bCertify			= FALSE;
	// END110520

	FLStrcpy( m_address, _countof( m_address ), pAddress );
}

CAccount::~CAccount()
{
}

BOOL CAccount::IsAdult()
{
	return ( m_cbAccountFlag & ACCOUNT_FLAG_18 );
}
//
//void CAccount::SendBillingResult( BILLING_INFO* pResult )
//{
//	BYTE cbResult;
//	switch( pResult->lResult )
//	{
//	case SUCCESS:
//		cbResult = ACCOUNT_CHECK_OK;
//		m_TimeOverDays	= *(pResult->pTimeOverDays);
//		m_cbLastOneLogon = pResult->cbLastOneLogon;
//		break;
//	case BILLING_INFO_FAILED:
//		cbResult = ACCOUNT_BILLING_INFO_FAILED;
//		break;
//	case DATABASE_ERROR:
//		cbResult = ACCOUNT_BILLING_DATABASE_ERROR;
//		break;
//	case TIME_OVER:
//		cbResult = ACCOUNT_BILLING_TIME_OVER;
//		break;
//	case OTHER_ERROR:
//	default:
//		cbResult = ACCOUNT_BILLING_OTHER_ERROR;
//		break;
//	}
//
//	if( m_nStatus == ACCOUNT_STATUS_SECONDQUERY )
//	{
//		if( pResult->lResult == SUCCESS )
//		{
//			m_nStatus = ACCOUNT_STATUS_INITIAL;								// GOTO BEGIN
//			FLTRACE_LOG( PROGRAM_NAME, _T( "%s ACCOUNT_STATUS_SECONDQUERY -> ACCOUNT_STATUS_INITIAL" ), pResult->szAccount );
//		}
//		else
//			g_dpSrvr.CloseExistingConnection( pResult->szAccount, ERROR_BILLING_TIME_OVER );	// END
//	}
//	else  // ACCOUNT_STATUS_INITIAL or ACCOUNT_STATUS_NOTIFIED
//	{
//		time_t lTimeSpan = 0;
//		if( pResult->lResult == SUCCESS )
//		{
//			// ���簡 1�� �̸� �������, ������ �α��� �Ҷ� '��ð� ��� ���ҽ��ϴ�' �޼���ó�� 
//			time_t tm = (time_t)( m_TimeOverDays.GetTime() );
//			time_t cur = time( NULL );
//			if( tm > cur )
//			{
//				time_t diff = tm - cur;
//				CTimeSpan span( diff );
//				if( span.GetTotalHours() < 24 )
//					lTimeSpan = diff;
//			}
//		}
//		
//		g_dpSrvr.OnAfterChecking( cbResult, pResult->szAccount, m_dpid1, m_dpid2, 
//								  m_dwAuthKey, m_cbAccountFlag, lTimeSpan, "", "" );
//	}
//}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------

CAccountMng::CAccountMng()
{
	m_nSizeof	= 0;
	m_nCount	= 0;
	m_nOldHour  = 0;

	CTime tTime = CTime::GetCurrentTime();
	m_nYear = tTime.GetYear() - 1;
	m_nMonth = tTime.GetMonth();
	m_nDay = tTime.GetDay();
	m_nHour = tTime.GetHour();
	m_nMinute = tTime.GetMinute();
}

CAccountMng::~CAccountMng()
{
	Clear();
}

void CAccountMng::Clear( void )
{
	CMclAutoLock	Lock( m_AddRemoveLock );

	for( map<string, CAccount*>::iterator i	= m_stringToPAccount.begin(); i != m_stringToPAccount.end(); ++i )
		SAFE_DELETE( i->second );
	m_stringToPAccount.clear();

	for( int j = 0; j < m_nSizeof; j++ )
		m_adpidToPAccount[j].clear();
	m_dpidToIndex.clear();
}

int CAccountMng::GetIndex( DWORD dpid1 )
{
// locked
	map<DWORD, int>::iterator i		= m_dpidToIndex.find( dpid1 );
	if( i  != m_dpidToIndex.end() )
		return i->second;
	return( -1 );
}

int CAccountMng::GetIdofServer( DWORD dpid )
{
// locked
	map<DWORD, DWORD>::iterator i	= m_2IdofServer.find( dpid );
	if( i != m_2IdofServer.end() )
		return i->second;
	return( -1 );
}

BYTE CAccountMng::AddAccount( LPCTSTR lpszAccount, DWORD dpid1, DWORD dpid2, DWORD* pdwAuthKey, BYTE cbAccountFlag, int fCheck, const CTime & tmBirthDate, const TCHAR* pAddress )
{
	CMclAutoLock	Lock( m_AddRemoveLock );

	map<string, CAccount*>::iterator i1	= m_stringToPAccount.find( lpszAccount );
	if( i1 == m_stringToPAccount.end() )
	{
		int nIndex	= GetIndex( dpid1 );
		if( nIndex >= 0 && nIndex < MAX_CERTIFIER )
		{
			map<DWORD, CAccount*>::iterator i2	= m_adpidToPAccount[nIndex].find( dpid2 );
			if( i2 == m_adpidToPAccount[nIndex].end() )
			{
				CAccount* pAccount	= new CAccount( lpszAccount, dpid1, dpid2, cbAccountFlag, fCheck, tmBirthDate, pAddress );
				m_stringToPAccount.insert( map<string, CAccount*>::value_type( lpszAccount, pAccount ) );
				m_adpidToPAccount[nIndex].insert( map<DWORD, CAccount*>::value_type( dpid2, pAccount ) );
				m_nCount++;
				*pdwAuthKey	= pAccount->m_dwAuthKey		= xRandom( 0x00000001UL, ULONG_MAX );

				// BEGIN110520
				pAccount->m_bCertify					= TRUE;
				// END110520
				return ACCOUNT_CHECK_OK;
			}
			return ACCOUNT_DUPLIACTE;
		}
		return ACCOUNT_DUPLIACTE;
	}
	else
	{
		CAccount* pAccount	= GetAccount( lpszAccount );
		if( pAccount && FALSE == pAccount->m_fRoute )
		{
			if( timeGetTime() - pAccount->m_dwPing > MIN( 2 ) )
			{
				g_dpSrvr.DestroyPlayer( pAccount->m_dpid1, pAccount->m_dpid2 );
				RemoveAccount( pAccount->m_dpid1, pAccount->m_dpid2 );
				return AddAccount( lpszAccount, dpid1, dpid2, pdwAuthKey, cbAccountFlag, fCheck, tmBirthDate, pAddress );
			}
		}
		return ACCOUNT_DUPLIACTE;
	}
}

// ������������ ������ �������� �˸��� ���� �±��� ��츸 �׷��ϴ�.
// �̴� ������ü(���̷���)�� ���Ǽ��� ���ֱ� ���ؼ��̴�.
// ���Ӽ����� �ɵ������� ���� �÷��� ���ɽð��� �������� �ʱ� ������, 
// �ֱ��� �ð� ������ �Կ��� �ұ��ϰ�, �α׾ƿ���  ������������ �̸� �˷���� �ϴ� ��찡 �����. 
//

// ��: 
// step1: ����A�� �α��� ( 2�� �÷��� �ð��� ���� ���� )
// step2: ������������ ���� �ð��� ��� 2������ ���� 
// step3: ���� �÷��� 
// step4: ���� �ð� ����, ���
// step5: ������������ A������ �÷��� �ϴ� �� ��� 
// step6: �÷��������� �������� ���� -> ���������� ���� A�� ���ؼ� �ð��� �������� ���� -> �÷��� �ð��� �״�� 2�� 
// step7: ����A�� �α��� -> �α��� ���ɻ��� 

// �ذ�åA: �ֱ������� ��� �� ���� �ð��� �޾Ƽ� ���. 
//		����: �ֱⰡ 20���̶�� 18�е��� ���� �÷��̸� �� �� ����  
// �ذ�åB: ���Ӽ����� �ð��� �����ϰ�, ���������� �̸� �޾Ƽ� ���常 �Ѵ�.
//		����: ?

// ���̷��ʹ� �ذ�å A�� �ַ� ����߱⿡ �̸� �����ؼ� �α׾ƿ��� ���������� �˷���. 

// ��... ���� ���ۿ��� ���Ӱ����� Ƽ���� ���ٰ� ��û�� ���µ� �״����� �ٷ� ���� ���� ��û�� ���ŵ��.
// ��.. ��, �׶��� �Ⱥ��� �ֽǼ��� ��������? ^^; ������� ������ �ϴ� ���������� �ѹ��� Ȯ���� �ؾ� �ϴϱ��.. 

void CAccountMng::RemoveAccount( LPCTSTR lpszAccount )
{
	CMclAutoLock	Lock( m_AddRemoveLock );

#ifdef __INTERNALSERVER
	FLINFO_LOG( PROGRAM_NAME, _T( "CAccountMng::RemoveAccount(%s)" ), lpszAccount );
#endif

	map<string, CAccount*>::iterator i	= m_stringToPAccount.find( lpszAccount );
	if( i != m_stringToPAccount.end() )
	{
		CAccount* pAccount = i->second;

		if( pAccount->m_fRoute && pAccount->m_cbRef == 2 )
		{
			long lConnectionCount = g_pServerDescription->DecreaseConnectionCount( pAccount->m_dwIdofServer, pAccount->m_uIdofMulti );

			g_dpSrvr.SendPlayerCount( pAccount->m_dwIdofServer, pAccount->m_uIdofMulti, lConnectionCount );
		}

		g_xASDBManager->UpdateTracking( FALSE, lpszAccount );	// ������ logoff������ ��� ����.
		g_dpSrvr.SendAccountLogout( lpszAccount );				// ������ logoff������ Certifier�� �˸���.

		SAFE_DELETE( pAccount );
		m_stringToPAccount.erase( i );
		m_nCount--;
	}
}

void CAccountMng::RemoveAccount( DWORD dpid1, DWORD dpid2 )
{
	CAccount* pAccount;
	int nIndex;
	CMclAutoLock	Lock( m_AddRemoveLock );

	nIndex	= GetIndex( dpid1 );
	if( nIndex >= 0 && nIndex < m_nSizeof )
	{
		map<DWORD, CAccount*>::iterator i	= m_adpidToPAccount[nIndex].find( dpid2 );
		if( i != m_adpidToPAccount[nIndex].end() )
		{
			pAccount	= i->second;
			m_adpidToPAccount[nIndex].erase( i );

			if( FALSE == pAccount->m_fRoute )
			{
				RemoveAccount( pAccount->m_lpszAccount );	// erase & delete
			}
		}
	}
}

CAccount* CAccountMng::GetAccount( LPCTSTR lpszAccount )
{
// locked
	map<string, CAccount*>::iterator i	= m_stringToPAccount.find( lpszAccount );
	if( i != m_stringToPAccount.end() )
		return i->second;
	return NULL;
}

CAccount* CAccountMng::GetAccount( DWORD dpid1, DWORD dpid2 )
{
// locked
	int nIndex	= GetIndex( dpid1 );
	if( nIndex >= 0 && nIndex < m_nSizeof )
	{
		map<DWORD, CAccount*>::iterator i	= m_adpidToPAccount[nIndex].find( dpid2 );
		if( i != m_adpidToPAccount[nIndex].end() )
		{
			return i->second;
		}
	}
	return NULL;
}

void CAccountMng::AddConnection( DWORD dpid1 )
{
	CMclAutoLock	Lock( m_AddRemoveLock );

	map<DWORD, int>::iterator i	= m_dpidToIndex.find( dpid1 );
	if( i == m_dpidToIndex.end() )
	{
		m_dpidToIndex.insert( map<DWORD, int>::value_type( dpid1, m_nSizeof ) );
		m_nSizeof++;
	}
//	else
//	{
//	}
}

void CAccountMng::RemoveConnection( DWORD dpid1 )
{
	CAccount* pAccount;
	int nIndex;

	CMclAutoLock	Lock( m_AddRemoveLock );
	nIndex	= GetIndex( dpid1 );
	if( nIndex >= 0 && nIndex < m_nSizeof )
	{
		map<DWORD, CAccount*>::iterator	i1	= m_adpidToPAccount[nIndex].begin();
		while( i1 != m_adpidToPAccount[nIndex].end() )
		{
			pAccount	= i1->second;
			map<DWORD, CAccount*>::iterator	i2	= i1;
			++i1;
			m_adpidToPAccount[nIndex].erase( i2 );
			RemoveAccount( pAccount->m_lpszAccount );	// erase & delete
		}
	}
}

void CAccountMng::AddIdofServer( DWORD dpid, DWORD dwIdofServer )
{
	int nIdofServer;
	CMclAutoLock	Lock( m_AddRemoveLock );

	nIdofServer		= GetIdofServer( dpid );
	if( nIdofServer < 0 )
		m_2IdofServer.insert( map<DPID, DWORD>::value_type( dpid, dwIdofServer ) );
}

void CAccountMng::RemoveIdofServer( DWORD dpid, BOOL bRemoveConnection )
{
	CAccount* pAccount;
	CMclAutoLock	Lock( m_AddRemoveLock );
	DWORD dwIdofServer	= GetIdofServer( dpid );

	map<string, CAccount*>::iterator i	= m_stringToPAccount.begin();
	while( i != m_stringToPAccount.end() )
	{
		pAccount	= i->second;
		++i;
		if( pAccount->m_dwIdofServer == dwIdofServer )
		{
			if( pAccount->m_fRoute )
				RemoveAccount( pAccount->m_lpszAccount );
			else
				RemoveAccount( pAccount->m_dpid1, pAccount->m_dpid2 );
		}
	}
	if( bRemoveConnection )
		m_2IdofServer.erase( dpid );
}



// ���� 10�ÿ� �̼����ڿ� �̵�� ���ε��� ������ �����Ų��.
void CAccountMng::PreventExcess()
{
	CTime cur = CTime::GetCurrentTime();

	if( cur.GetHour() == 22 && m_nOldHour != 22 )
	{	
		CAccount* pAccount;
		CMclAutoLock	Lock( m_AddRemoveLock );
		map<string, CAccount*>::iterator i	= m_stringToPAccount.begin();
		while( i != m_stringToPAccount.end() )
		{
			pAccount	= i->second;
			++i;

			if( pAccount->IsAdult() )	// �����̸� skip
				continue;

			g_dpSrvr.CloseExistingConnection( pAccount->m_lpszAccount, 0 );
		}	
	}

	m_nOldHour = cur.GetHour();
}

CAccount* CAccountMng::GetAccount( LPCTSTR lpszAccount, DWORD dwKey )
{
	CMclAutoLock	Lock( m_AddRemoveLock );

	CAccount* pAccount = GetAccount( lpszAccount );
	if( pAccount == NULL )
		return NULL;		// may be logout

	if( dwKey && pAccount->m_dwAuthKey != dwKey )		// dwKey�� 0�̸� �˻����� �ʴ´�.
		return NULL;		// may be different account
	
	return pAccount;
}

//// szAccount���� ����üũ ���(lResult)�� ���� 
//void CAccountMng::SendBillingResult( BILLING_INFO* pResult )
//{
//	CMclAutoLock	Lock( m_AddRemoveLock );
//
//	CAccount* pAccount = GetAccount( pResult->szAccount, pResult->dwKey );
//	if( pAccount == NULL )
//		return;		// may be logout
//
//	pAccount->SendBillingResult( pResult );
//}

// ��¥�� üũ�ؼ� �������� Ǯ���� �˻�
BOOL CAccountMng::IsTimeCheckAddr()
{
	CTime tTime = CTime::GetCurrentTime();
	if( m_nYear == tTime.GetYear() && m_nMonth == tTime.GetMonth() && m_nDay == tTime.GetDay() &&
		m_nHour == tTime.GetHour() && m_nMinute == tTime.GetMinute() )
	{
		return TRUE;
	}
	return FALSE;
}

map<string, CAccount*> CAccountMng::GetMapAccount()
{
	return m_stringToPAccount;
}

DWORD CAccountMng::PopPCBangPlayer( DWORD dwAuthKey )
{
	map<DWORD, DWORD>::iterator it = m_mapPCBang.find( dwAuthKey );
	if( it != m_mapPCBang.end() )
	{
		DWORD dwReturn = it->second;
		m_mapPCBang.erase( it );
		return dwReturn;
	}

	return 0;
}