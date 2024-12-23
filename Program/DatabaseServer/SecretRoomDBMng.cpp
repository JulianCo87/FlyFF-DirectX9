#include "stdafx.h"
#include "secretroomdbmng.h"
#include "dptrans.h"
extern AppInfo g_appInfo;
////////////////////////////////////////////////////////////////
/////	CSecretRoomDBMng ///////////////////////////////////////
////////////////////////////////////////////////////////////////
CSecretRoomDBMng::CSecretRoomDBMng(void)
{
	if( !m_SRDbController.CreateDbHandler() )
		FLERROR_LOG( PROGRAM_NAME, _T( "m_SRDbController.CreateDbHandler()" ) );
}

CSecretRoomDBMng::~CSecretRoomDBMng(void)
{
	m_SRDbController.CloseDbHandler();
}

CSecretRoomDBMng* CSecretRoomDBMng::GetInstance( void )
{
	static CSecretRoomDBMng sSecretRoomDBMng;
	return &sSecretRoomDBMng;
}


BOOL CSecretRoomDBMng::PostRequest( int nQuery, BYTE* lpBuf, int nBufSize, ULONG_PTR dwCompletionKey )
{
	return m_SRDbController.PostRequest( nQuery, lpBuf, nBufSize, dwCompletionKey );
}


////////////////////////////////////////////////////////////////
/////	CSecretRoomDBMng ///////////////////////////////////////
////////////////////////////////////////////////////////////////
CSecretRoomDbController::CSecretRoomDbController(void)
:
m_nTimes( 0 )
{
}

CSecretRoomDbController::~CSecretRoomDbController(void)
{
}

void CSecretRoomDbController::Handler( LPDB_OVERLAPPED_PLUS pov, ULONG_PTR dwCompletionKey )
{
	switch( pov->nQueryMode )
	{
		case QUERY_SECRETROOM_LOAD:
			LoadSecretRoom( dwCompletionKey );
			break;

		case QUERY_SECRETROOM_TENDER_INSERT:
			{
				CAr ar( pov->lpBuf, pov->uBufSize );
				BYTE nContinent; DWORD dwGuildId; int nPenya;
				ar >> nContinent >> dwGuildId >> nPenya;
				
				InsertTenderToDB( nContinent, dwGuildId, nPenya );
			}
			break;

		case QUERY_SECRETROOM_TENDER_UPDATE:
			{
				CAr ar( pov->lpBuf, pov->uBufSize );
				BYTE nContinent; DWORD dwGuildId; int nPenya; char chState;
				DWORD dwWorldId = 0; int nWarState = 0, nKillCount = 0;
				ar >> nContinent >> dwGuildId >> nPenya >> chState;
				if( chState == 'W' || chState == 'L' )
					ar >> dwWorldId >> nWarState >> nKillCount;

				UpdateTenderToDB( nContinent, dwGuildId, nPenya, chState, dwWorldId, nWarState, nKillCount );
			}
			break;

		case QUERY_SECRETROOM_LINEUP_INSERT:
			{
				CAr ar( pov->lpBuf, pov->uBufSize );
				BYTE nContinent; DWORD dwGuildId, dwPlayerId; int nSize; 
				vector<DWORD> vecMemberId;
				ar >> nContinent >> dwGuildId;
				ar >> nSize;
				for( int i=0; i<nSize; i++ )
				{
					ar >> dwPlayerId;
					vecMemberId.push_back( dwPlayerId );
				}
				
				InsertLineUpMemberToDB( nContinent, dwGuildId, vecMemberId );
			}
			break;

		case QUERY_SECRETROOM_CLOSED:
			{
				m_nTimes++;
			}
			break;
	}
}

void CSecretRoomDbController::LoadSecretRoom( DPID dpId )
{
	CDPTrans::GetInstance()->SendSecretRoomInfoClear( dpId );
	
	CQuery* pQuery = GetQueryObject();
	char szQuery[MAX_QUERY_SIZE] = {0,};

//	BYTE nContinent;
	vector<BYTE> vec_nContinent;
	vector<DWORD> vec_dwGuildId;
	vector<int>	vec_nPenya;

   	// m_nTimes �������� ������ 0
	FLSPrintf( szQuery, _countof( szQuery ), "SECRET_ROOM_STR 'S1', '%02d', 0, 0, '0', 0, 'T', 0, 0, 0", g_appInfo.dwSys );
	if( pQuery->Exec( szQuery ) == FALSE )
	{ FLERROR_LOG( PROGRAM_NAME, _T( "%s" ), szQuery ); return; }

	if( pQuery->Fetch() )
		m_nTimes = pQuery->GetInt( "nTimes" );
	if( m_nTimes == 0 )
	{	
		m_nTimes = 1;
		return;
	}
		
	// ���� ���� ��������('T')
	FLSPrintf( szQuery, _countof( szQuery ), "SECRET_ROOM_STR 'S2', '%02d', %d, 0, '0', 0, 'T', 0, 0, 0", g_appInfo.dwSys, m_nTimes );
	if( pQuery->Exec( szQuery ) == FALSE )
	{ FLERROR_LOG( PROGRAM_NAME, _T( "%s" ), szQuery ); return; }

	int nCount = 0;
	while( pQuery->Fetch() )
	{
		vec_nContinent.push_back( static_cast<BYTE>( pQuery->GetInt( "nContinent" ) ) );
		vec_dwGuildId.push_back( static_cast<DWORD>( pQuery->GetInt( "m_idGuild" ) ) );
		vec_nPenya.push_back( static_cast<int>( pQuery->GetInt( "nPenya" ) ) );
		nCount++;
	}
	if( nCount == 0 )
		m_nTimes++;

	// ������ ��� �����ͼ� WorldServer�� ����
	for( DWORD i=0; i<vec_nContinent.size(); i++ )
	{
		vector<DWORD> vec_dwMemberId;

		FLSPrintf( szQuery, _countof( szQuery ), "SECRET_ROOM_MEMBER_STR 'S2', '%02d', %d, %d, '%06d', '0'",
							g_appInfo.dwSys, m_nTimes, vec_nContinent[i], vec_dwGuildId[i] );
		if( pQuery->Exec( szQuery ) == FALSE )
		{ FLERROR_LOG( PROGRAM_NAME, _T( "%s" ), szQuery ); return; }
		while( pQuery->Fetch() )
			vec_dwMemberId.push_back( pQuery->GetInt( "m_idPlayer" ) );
		
		CDPTrans::GetInstance()->SendSecretRoomTenderInfo( vec_nContinent[i], vec_dwGuildId[i], vec_nPenya[i], vec_dwMemberId, dpId );
	}
}

void CSecretRoomDbController::InsertTenderToDB( BYTE nContinent, DWORD dwGuildId, int nPenya )
{
	CQuery* pQuery = GetQueryObject();
	char szQuery[MAX_QUERY_SIZE] = {0,};

	// ���� ���� Insert
	FLSPrintf( szQuery, _countof( szQuery ), "SECRET_ROOM_STR 'I1', '%02d', %d, %d, '%06d', %d, 'T', 0, 0, 0",
						g_appInfo.dwSys, m_nTimes, nContinent, dwGuildId, nPenya );
	if( pQuery->Exec( szQuery ) == FALSE )
	{ FLERROR_LOG( PROGRAM_NAME, _T( "%s" ), szQuery ); return; }
}

void CSecretRoomDbController::UpdateTenderToDB( BYTE nContinent, DWORD dwGuildId, int nPenya, char chState, DWORD dwWorldId, int nWarState, int nKillCount )
{
	CQuery* pQuery = GetQueryObject();
	char szQuery[MAX_QUERY_SIZE] = {0,};

	// ���� ���� Update
	if( chState == 'W' || chState == 'L' )		// �¸�, �й� ����
		FLSPrintf( szQuery, _countof( szQuery ), "SECRET_ROOM_STR 'U2', '%02d', %d, %d, '%06d', %d, '%c', %d, %d, %d",
				g_appInfo.dwSys, m_nTimes, nContinent, dwGuildId, nPenya, chState, dwWorldId, nWarState, nKillCount );
	else	// �Ϲ����� Update( ��û�� �߰� �� ��� )
		FLSPrintf( szQuery, _countof( szQuery ), "SECRET_ROOM_STR 'U1', '%02d', %d, %d, '%06d', %d, '%c', 0, 0, 0",
				g_appInfo.dwSys, m_nTimes, nContinent, dwGuildId, nPenya, chState );
	
	if( pQuery->Exec( szQuery ) == FALSE )
	{ FLERROR_LOG( PROGRAM_NAME, _T( "%s" ), szQuery ); return; }
}

void CSecretRoomDbController::InsertLineUpMemberToDB( BYTE nContinent, DWORD dwGuildId, vector<DWORD>& vecMemberId )
{
	CQuery* pQuery = GetQueryObject();
	char szQuery[MAX_QUERY_SIZE] = {0,};

	// �켱 ���� ������ ��� �����ϰ�...
	FLSPrintf( szQuery, _countof( szQuery ), "SECRET_ROOM_MEMBER_STR 'D1', '%02d', %d, %d, '%06d', '0'",
		g_appInfo.dwSys, m_nTimes, nContinent, dwGuildId );
	if( pQuery->Exec( szQuery ) == FALSE )
	{ FLERROR_LOG( PROGRAM_NAME, _T( "%s" ), szQuery ); return; }

	// �� ������ ��� Insert
	for( DWORD i=0; i<vecMemberId.size(); i++ )
	{
		FLSPrintf( szQuery, _countof( szQuery ), "SECRET_ROOM_MEMBER_STR 'I1', '%02d', %d, %d, '%06d', '%07d'",
			g_appInfo.dwSys, m_nTimes, nContinent, dwGuildId, vecMemberId.at( i ) );
		if( pQuery->Exec( szQuery ) == FALSE )
		{ FLERROR_LOG( PROGRAM_NAME, _T( "%s" ), szQuery ); return; }
	}
}
