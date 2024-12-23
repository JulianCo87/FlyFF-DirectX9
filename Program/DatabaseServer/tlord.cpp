#include "stdafx.h"
#include "tlord.h"
#include "lordskillexecutable.h"
#include <cmnhdr.h>
#include "dptrans.h"
#include <dbmanager.h>
#include <definetext.h>
#include <playerdata.h>

extern	AppInfo		g_appInfo;


////////////////////////////////////////////////////////////////////////////////
// Ʈ���� ������ ����
CTLord::CTLord()
: CLord(),
m_pController( 0 )
{
}

CTLord::~CTLord()
{
}

void CTLord::CreateColleagues( void )
{	// ���� ��ü�� ���� �� �ʱ�ȭ
	// ����
	m_pController	= new CLController( this );
	m_pController->CreateDbHandler( MIN( 1 ) );

	CTElection * pElection	= NULL;
	CLEvent* pEvent			= NULL;
	CLordSkill* pSkills		= NULL;

	try
	{
		pElection	= new CTElection( this );
		pEvent		= new CLEvent( this );
		pSkills		= new CTLordSkill( this );
		// ��ũ��Ʈ

		const TCHAR szFileName[]	= _T( "election.inc" );

		TCHAR szFullPath[ MAX_PATH ]	= { 0, };
		g_pScriptFileManager->GetScriptFileFullPath( szFullPath, _countof( szFullPath ), szFileName );

		pElection->Initialize( szFullPath );
		pEvent->Initialize( "lordevent.inc" );
		pSkills->Initialize( "lordskill.txt" );
		// ����
		m_pElection		= pElection;
		m_pEvent	= pEvent;
		m_pSkills	= pSkills;
	}
	catch( ... )
	{
		SAFE_DELETE( pElection );
		SAFE_DELETE( pEvent );
		SAFE_DELETE( pSkills );
	}
	// �����ͺ��̽��κ��� ���� ���� ����
	if( !RestoreAll() )
	{
		FLTRACE_LOG( PROGRAM_NAME, _T( "CTLord.CreateColleagues" ) );
		exit( -1 );
	}
}

void CTLord::DestroyColleagues( void )
{	// ���� ��ü�� ����
	if( m_pController )
		m_pController->CloseDbHandler();

	SAFE_DELETE( m_pElection );
	SAFE_DELETE( m_pEvent );
	SAFE_DELETE( m_pSkills );
	SAFE_DELETE( m_pController );
}

CTLord* CTLord::Instance( void )
{
	static	CTLord	sLord;
	return &sLord;
}

BOOL CTLord::PostRequest( int nQuery, BYTE* lpBuf, int nBufSize, ULONG_PTR dwCompletionKey )
{	// ���� �����忡 �۾��� ��û
	return m_pController->PostRequest( nQuery, lpBuf, nBufSize, dwCompletionKey );
}

void CTLord::Handler( LPDB_OVERLAPPED_PLUS pov, ULONG_PTR dwCompletionKey )
{	// ���� �����尡 ������ ��û�� ó���Ѵ�
	switch( pov->nQueryMode )
	{
		case eBeginCandidacy:	// ���ĺ� ����
			GetElection()->BeginCandidacy();
			break;
		case eBeginVote:	// ��ǥ ����
			GetElection()->BeginVote( 0 );
			break;
		case eEndVote:	// ��ǥ ����
			GetElection()->EndVote( 0 );
			break;
		//--------------------------------------------------------------------------------
		case eAddDeposit:	// ���� ����
			{
				u_long idPlayer;
				__int64 iDeposit;
				CAr ar( pov->lpBuf, pov->uBufSize );
				ar >> idPlayer >> iDeposit;
				GetElection()->AddDeposit( idPlayer, iDeposit, time( NULL ) );
				break;
			}
		case eSetPledge:	// ���� ����
			{
				u_long idPlayer;
				char szPledge[CCandidate::nMaxPledgeLen]	= {0, };
				CAr ar( pov->lpBuf, pov->uBufSize );
				ar >> idPlayer;
				ar.ReadString( szPledge, _countof( szPledge ) );
				GetElection()->SetPledge( idPlayer, szPledge );
				break;
			}
		case eIncVote:	// ��ǥ
			{
				u_long idPlayer, idElector;
				CAr ar( pov->lpBuf, pov->uBufSize );
				ar >> idPlayer >> idElector;
				GetElection()->IncVote( idPlayer, idElector );
				break;
			}
		//--------------------------------------------------------------------------------
		case eInit:		// ���� ���� ����
								// dwCompletionKey	���� ���� ���� dpid
			CDPTrans::GetInstance()->SendLord( dwCompletionKey );
			break;
		//--------------------------------------------------------------------------------
		case eLEventCreate:		// ���� �̺�Ʈ ����
			{
				u_long idPlayer;
				int iEEvent, iIEvent;
				CAr ar( pov->lpBuf, pov->uBufSize );
				ar >> idPlayer >> iEEvent >> iIEvent;
				GetEvent()->AddComponent( idPlayer, iEEvent, iIEvent );
				break;
			}
		case eLEventInitialize:		// ���� �̺�Ʈ �ʱ�ȭ
			{
				GetEvent()->Initialize();
				break;
			}
		case eLordSkillUse:		// ���� ��ų ���
			{
				u_long idPlayer, idTarget;
				int nSkill;
				CAr ar( pov->lpBuf, pov->uBufSize );
				ar >> idPlayer >> idTarget >>  nSkill;
				CLordSkillComponentExecutable* pComponent	= GetSkills()->GetSkill( nSkill );
				if( pComponent )	// �� Ŭ������ CLordSkillComponentODBC
					pComponent->Execute( idPlayer, idTarget, static_cast<LPVOID>( GetController() ) );
				break;
			}
	}
}

void CTLord::OnTimer()
{	// ƽ	// �ʴ� 1ȸ		// m_pController�� ȣ��
	FLTRACE_LOG( PROGRAM_NAME, _T( "%d" ), time( NULL ) );
	CTElection* pElection	= static_cast<CTElection*>( GetElection() );
	pElection->OnTimer();
	CLEvent* pEvent	= static_cast<CLEvent*>( GetEvent() );
	pEvent->OnTimer();
	CTLordSkill* pSkills	= static_cast<CTLordSkill*>( GetSkills() );
	pSkills->OnTimer();
}

BOOL CTLord::RestoreAll( void )
{	// �����ͺ��̽��κ��� ��� ���� ����
	return GetController()->Restore();
};

BOOL CTLord::Restore( CQuery* pQuery )
{	// ���� ����
	if( !pQuery->Execute( "uspRestoreLord %d", g_appInfo.dwSys ) )
		return FALSE;
	if( pQuery->Fetch() )
		Set( pQuery->GetInt( "idLord" ) );
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
// Ʈ���� ������ ���� Ŭ����
CTElection::CTElection( CLord* pLord )
:
IElection( pLord ),
#ifdef __INTERNALSERVER
m_bRun( FALSE )	// ���� ������ ���� ���μ��� ��Ȱ��
#else	// __INTERNALSERVER
m_bRun( TRUE )
#endif	// __INTERNALSERVER
{
}

CTElection::~CTElection()
{
}

BOOL CTElection::Initialize( const char* szFile )
{	// �ʱ�ȭ
	if( !ReadIni( szFile ) )	// ��ũ��Ʈ �ε�
	{
		FLTRACE_LOG( PROGRAM_NAME, _T( "LoadScript(): file not found: %s" ), szFile );
		return FALSE;
	}
	return TRUE;
}

BOOL CTElection::ReadIni( const char* szFile )
{	// ��ũ��Ʈ �ε�
	CScript s;
	if( s.Load( szFile ) == FALSE )
		return FALSE;

	s.GetToken();
	while( s.tok != FINISHED )
	{
		if( s.Token == _T( "property" ) )
		{
			s.GetToken();	// {
			s.GetToken();
			while( *s.token != '}' )
			{
				if( s.Token == _T( "nDayOfWeek" ) )
					property.nDayOfWeek		= s.GetNumber();
				else if( s.Token == _T( "nHour" ) )
					property.nHour	= s.GetNumber();
				else if( s.Token == _T( "tCandidacy" ) )
					property.tCandidacy	= s.GetNumber();
				else if( s.Token == _T( "tVote" ) )
					property.tVote	= s.GetNumber();
				else if( s.Token == _T( "nDays" ) )
					property.nDays	= s.GetNumber();
				s.GetToken();
			}
		}
		else if( s.Token == _T( "rate" ) )
		{
			s.GetToken();	// {
			float fRate	= s.GetFloat();
			while( *s.token != '}' )
			{
				property.m_vReturnDepositRates.push_back( fRate );
				fRate	= s.GetFloat();
			}
		}
		else if( s.Token == _T( "fRequirementFactor" ) )
		{
			property.fRequirementFactor	= s.GetFloat();
		}
		else if( s.Token == _T( "item" ) )
		{
			int iIndex	= s.GetNumber();
			s.GetToken();	// {
			int nItem		= s.GetNumber();
			while( *s.token != '}' )
			{
				property.m_vItems[iIndex].push_back( nItem );
				nItem	= s.GetNumber();
			}
		}
		s.GetToken();
	}
	return TRUE;
}

CString	CTElection::ttos( time_t t )
{
	CTime tm( t );
	return tm.Format( "%Y%m%d%H%M%S" );
}

time_t CTElection::stot( const char* sz )
{
	if( strlen( sz ) != 14 )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s" ), sz );
	}

	char szYear[5]	= { 0,};
	char szMonth[3]	= { 0,};
	char szDay[3]	= { 0,};
	char szHour[3]	= { 0,};
	char szMin[3]	= { 0,};
	char szSec[3]	= { 0, };

	FLStrncpy( szYear, _countof( szYear ), sz, 4 );
	FLStrncpy( szMonth, _countof( szMonth ), sz+4, 2 );
	FLStrncpy( szDay, _countof( szDay ), sz+6, 2 );
	FLStrncpy( szHour, _countof( szHour ), sz+8, 2 );
	FLStrncpy( szMin, _countof( szMin ), sz+10, 2 );
	FLStrncpy( szSec, _countof( szSec ), sz+12, 2 );



	time_t	tReturnTime	= 0;
	::time( &tReturnTime);

	//////////////////////////////////////////////////////////////////////////
	// mirchang_20100706
	int nYear, nMonth, nDay, nHour, nMin, nSec;
	nYear = nMonth = nDay = nHour = nMin = nSec = 0;

	nYear	= atoi( szYear );
	nMonth	= atoi( szMonth );
	nDay	= atoi( szDay );
	nHour	= atoi( szHour );
	nMin	= atoi( szMin );
	nSec	= atoi( szSec );

	if( g_pCommonCheck->CheckTime( nYear, nMonth, nDay, nHour, nMin, nSec ) == true )
	{
		CTime	tm( nYear, nMonth, nDay, nHour, nMin, nSec );

		tReturnTime	= (time_t)( tm.GetTime() );
	}

	return	tReturnTime;
}

BOOL CTElection::Restore( CQuery* pQuery )
{	// �����ͺ��̽��κ��� ���� ���� ����
	if( !pQuery->Execute( "uspRestoreElection %d", g_appInfo.dwSys ) )
		return FALSE;
	if( pQuery->Fetch() )
	{
		m_idElection	= pQuery->GetInt( "idElection" );
		ELECTION_STATE eState	= static_cast<ELECTION_STATE>( pQuery->GetInt( "eState" ) );
		SetState( eState );
		SetRequirement( (int)( pQuery->GetInt( "nRequirement" ) * property.fRequirementFactor ) );
		char szBegin[17]	= { 0,};
		pQuery->GetStr( "szBegin", szBegin, _countof( szBegin ) );
		SetBegin( CTElection::stot( szBegin ) );
	}
	if( !pQuery->Execute( "uspRestoreCandidates %d, %d", g_appInfo.dwSys, GetId() ) )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "couldn't execute uspRestoreCandidates" ) );
		return FALSE;
	}
	while( pQuery->Fetch() )
	{
		u_long idPlayer		= static_cast<u_long>( pQuery->GetInt( "idPlayer" ) );
		__int64 iDeposit	= pQuery->GetInt64( "iDeposit" );
		char szPledge[CCandidate::nMaxPledgeLen]	= { 0,};
		pQuery->GetStr( "szPledge", szPledge, _countof( szPledge ) );
		int nVote	= pQuery->GetInt( "nVote" );
		time_t tCreate	= pQuery->GetInt( "tCreate" );
		AddCandidate( idPlayer, iDeposit, szPledge, nVote, tCreate );
	}
	if( GetState() == eVote )	// ��ǥ ���¸�
		SortVote();		// ��ǥ���� ����
	else	// �� ���� ���¶��
		SortDeposit();	// ���������� ����
	
	if( !GetBegin() || GetState() == eExpired )	// ���� �����̰ų� ����� ���Ŷ��
	{
		PrepareNext();
		if( !pQuery->Execute( "uspElectionInitialize %d, %d, %s", g_appInfo.dwSys, GetId(), CTElection::ttos( GetBegin() ) ) )
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "couldn't execute uspElectionInitialize" ) );
			return FALSE;
		}
	}
	return TRUE;
}

float CTElection::GetRetRate( u_long idPlayer )
{	// ������ ��ȯ��
	int nOrder	= GetOrder( idPlayer );
	nOrder	= min( nOrder, IElection::nMaxCandidates );
	if( nOrder < 0 )
		return 0.0F;
	return property.m_vReturnDepositRates.at( nOrder );
}

BOOL CTElection::DoTestBeginCandidacy( void )
{	// ���ĺ� ���� ������ ���� �� ���� ���� ����
	// IElection::BeginCandidacy
	FLTRACE_LOG( PROGRAM_NAME, _T( "CTElection.DoTestBeginCandidacy" ) );
	CLController* pController	= m_pLord->GetController();
	BOOL bResult	= pController->BeginCandidacy();
	if( bResult )
		CDPTrans::GetInstance()->SendElectionBeginCandidacy();
	return bResult;
}

void PostDeposit::operator ()( const SPC & lhs )	const
{	// ������ ���� ��ȯ �Լ���
	const u_long idReceiver	= lhs->GetIdPlayer();
	CTElection* pElection	= static_cast<CTElection*>( CTLord::Instance()->GetElection() );

	// �ĺ��� ������ ���� ��ȯ�� ���ϱ�
	float fRate		= pElection->GetRetRate( idReceiver );
	if( fRate == 0.0F )
		return;
	// 2008/10/31 - ���е� ������ �߻��Ͽ� ���� - ˬ
//	__int64 iDeposit	= lhs->GetDeposit() * fRate;
	__int64 iDeposit	= (__int64)( lhs->GetDeposit() / 100 * ( fRate * 100 ) );

	// �ĺ����� ������ ���� ���� ������ �ۼ�
	TCHAR szMailTitle[MAX_MAILTITLE]	= { 0, };
	TCHAR szMailText[MAX_MAILTEXT]		= { 0, };
	FLItemElem kItemElem;

	const int nOrder	= pElection->GetOrder( idReceiver );

	if( nOrder < IElection::nMaxCandidates )
	{
		FLStrcpy( szMailTitle, _countof( szMailTitle ), prj.GetText( TID_GAME_ELECTION_RETURN_DEPOSIT_MAIL_TITLE_02 ) );	// title
		FLSPrintf( szMailText, _countof( szMailText ), prj.GetText( TID_GAME_ELECTION_RETURN_DEPOSIT_MAIL_TEXT_02 ),
			static_cast<__int64>( lhs->GetDeposit() ),
			nOrder + 1,
			static_cast<float>( fRate * 100.0F ),
			static_cast<__int64>( iDeposit ) );
	}
	else
	{
		FLStrcpy( szMailTitle, _countof( szMailTitle ), prj.GetText( TID_GAME_ELECTION_RETURN_DEPOSIT_MAIL_TITLE_01 ) );	// title
		FLSPrintf( szMailText, _countof( szMailText ), prj.GetText( TID_GAME_ELECTION_RETURN_DEPOSIT_MAIL_TEXT_01 ),
			static_cast<__int64>( lhs->GetDeposit() ),
			static_cast<float>( fRate * 100.0F ),
			static_cast<__int64>( iDeposit ) );

		if( !m_pQuery->Execute( "uspElectionLeaveOut %d, %d, %d", g_appInfo.dwSys, pElection->GetId(), idReceiver ) )
			FLERROR_LOG( PROGRAM_NAME, _T( "couldn't execute uspElectionLeaveOut" ) );
	}

	// ��ȯ���� �丰�� ��ķ� �и��Ͽ� ÷��
	const int nPerin	= max( static_cast<int>( iDeposit / static_cast<__int64>(PERIN_VALUE ) ), 0 );
	const int nPenya	= max( static_cast<int>( iDeposit - static_cast<__int64>( PERIN_VALUE ) * static_cast<__int64>( nPerin ) ), 0 );

	if( nPerin > 0 )
	{
		kItemElem.m_dwItemId	= ITEM_INDEX( 26456, II_SYS_SYS_SCR_PERIN );
		kItemElem.m_nItemNum	= nPerin;
		kItemElem.SetSerialNumber();
	}

	CDbManager::PostMail( *m_pQuery, idReceiver, 0, kItemElem, nPenya, szMailTitle, szMailText );
}

void CTElection::DoReturnDeposit( void )
{	// ��� �ĺ��� ���� ������ ���� ������ ��ȯ
	// IElection::BeginVote
	FLTRACE_LOG( PROGRAM_NAME, _T( "CTElection.DoReturnDeposit" ) );
	CLController* pController	= m_pLord->GetController();

	for_each( m_vCandidates.begin() + IElection::nMaxCandidates, m_vCandidates.end(), PostDeposit( pController->GetQueryObject() ) );
}

BOOL CTElection::DoTestBeginVote( int & nRequirement )
{	// ��ǥ ���� ������ ���� �� ����
	FLTRACE_LOG( PROGRAM_NAME, _T( "CTElection.DoTestBeginVote" ) );
	CLController* pController	= m_pLord->GetController();
	int nTotal	= pController->BeginVote();
	if( nTotal >= 0 )
	{
		nRequirement	= static_cast<int>( nTotal * property.fRequirementFactor );
		CDPTrans::GetInstance()->SendElectionBeginVote( nRequirement );
		return TRUE;
	}
	return FALSE;
}

BOOL CTElection::DoTestEndVote( u_long idPlayer )
{	// ��ǥ ���� ���� ó���� ����
	// IElection::EndVote
	FLTRACE_LOG( PROGRAM_NAME, _T( "CTElection.DoTestEndVote" ) );
	CLController* pController	= m_pLord->GetController();
	if( pController->EndVote( idPlayer ) )	// ���� ����
	{
		CDPTrans::GetInstance()->SendElectionEndVote( idPlayer );		// ����
		PostDepositToAllCandidate();	// ������ ��ȯ
		PostLordItems( idPlayer );		// ���� ������ ����
		return TRUE;
	}
	return FALSE;
}

void CTElection::PostDepositToAllCandidate( void )
{	// ��� �ĺ��� ������ ��ȯ
	CLController* pController	= m_pLord->GetController();
	for_each( m_vCandidates.begin(), m_vCandidates.end(), PostDeposit( pController->GetQueryObject() ) );
}

BOOL CTElection::DoTestAddDeposit( u_long idPlayer, __int64 iDeposit, time_t tCreate )
{	// ���� ���� ���� �� ��� ����
	// IElection::AddDeposit
	FLTRACE_LOG( PROGRAM_NAME, _T( "CTElection.DoTestAddDeposit: %07d, %I64d, %d" ), idPlayer, iDeposit, tCreate );
	CLController* pController	= m_pLord->GetController();
	BOOL bRet	= ( GetState() == eCandidacy ) && pController->AddDeposit( idPlayer, iDeposit, tCreate );
	CDPTrans::GetInstance()->SendElectionAddDeposit( idPlayer, iDeposit, tCreate, bRet );
	return bRet;
}

void CTElection::DoAddDepositComplete( u_long /*idPlayer*/, __int64 /*iDeposit*/, time_t /*tCreate*/ )
{
}

BOOL CTElection::DoTestSetPledge( u_long idPlayer, const char* szPledge )
{	// ���� ���� ���� �� ��� ����
	// IElection::SetPledge
	BOOL bRet	= FALSE;
	FLTRACE_LOG( PROGRAM_NAME, _T( "CTElection.DoTestSetPledge: %07d, %s" ), idPlayer, szPledge );
	if( GetState() == eVote )
	{
		CLController* pController	= m_pLord->GetController();
		bRet	= pController->SetPledge( idPlayer, szPledge );
		CDPTrans::GetInstance()->SendElectionSetPledge( idPlayer, szPledge, bRet );
	}
	return bRet;
}

BOOL CTElection::DoTestIncVote( u_long idPlayer, u_long idElector )
{	// ��ǥ ���� �� ��� ����
	// IElection::IncVote
	FLTRACE_LOG( PROGRAM_NAME, _T( "CTElection.DoTestIncVote: %07d, %07d" ), idPlayer, idElector );
	CLController* pController	= m_pLord->GetController();
	BOOL bRet	= pController->IncVote( idPlayer, idElector );
	CDPTrans::GetInstance()->SendElectionIncVote( idPlayer, idElector, bRet );
	return bRet;
}

void CTElection::PostLordItems( u_long idLord )
{	// ���� ������ ���� ����
	if( idLord != NULL_ID )
	{
		BYTE iIndex		= SEX_MALE;
		{
			CMclAutoLock	Lock( CPlayerDataCenter::GetInstance()->m_Access );
			PlayerData* pData	= CPlayerDataCenter::GetInstance()->GetPlayerData( idLord );
			if( pData )
				iIndex	= pData->data.nSex;
			FLASSERT( iIndex >= 0 && iIndex < 2 );
			if( iIndex < 0 || iIndex > 2 )
				iIndex	= 0;
		}
		for( vector<int>::iterator i = property.m_vItems[iIndex].begin(); i != property.m_vItems[iIndex].end(); ++i )
		{
			PostItemStruct pi;
			pi.nItem	= *i;
			pi.dwKeepTime	= time( NULL ) + 1209600;	// 14��
			PostItem( idLord, pi );
		}
	}
}

void CTElection::PostItem( u_long idPlayer, PostItemStruct & pi )
{	// ������ ���� ������ ����
	// �Ϲ�ȭ ���

	TCHAR szMailTitle[MAX_MAILTITLE]	= { 0, };
	TCHAR szMailText[MAX_MAILTEXT]		= { 0, };
	FLItemElem kItemElem;

	FLSPrintf( szMailTitle, _countof( szMailTitle ), prj.GetText( TID_GAME_ELECTION_SET_LORD_MAIL_TITLE ) );
	FLSPrintf( szMailText, _countof( szMailText ), prj.GetText( TID_GAME_ELECTION_SET_LORD_MAIL_TEXT ) );
	kItemElem.m_dwItemId	= pi.nItem;
	kItemElem.m_nItemNum	= 1;
	kItemElem.m_dwKeepTime	= pi.dwKeepTime;
	kItemElem.SetSerialNumber();
	CLController* pController	= m_pLord->GetController();

	CDbManager::PostMail( *pController->GetQueryObject(), idPlayer, 0, kItemElem, 0, szMailTitle, szMailText );
}

void CTElection::OnTimer( void )
{	// ���� ƽ
	if( !IsRunable() )		// ���� ���μ����� �������� �ƴϸ�
		return;
	
	// ������ ���� ���¿�
	// ���� �ð� ���� ���� ���°� �ٸ���
	// ���� �ܰ�� ���� ��Ų��
	if( GetState() != GetPropertyState() )
	{
		if( GetState() == eReady )
			BeginCandidacy();
		else if( GetState() == eCandidacy )
			BeginVote( 0 );
		else if( GetState() == eVote )
			EndVote( 0 );
	}
}

////////////////////////////////////////////////////////////////////////////////
// Ʈ���� ������ ���� �̺�Ʈ
CLEvent::CLEvent( CLord* pLord )
:
ILordEvent( pLord )
{
}

CLEvent::~CLEvent()
{
}

BOOL CLEvent::Restore( CQuery* pQuery )
{	// �����ͺ��̽��κ��� ����
	if( !pQuery->Execute( "uspRestoreLEvent %d", g_appInfo.dwSys ) )
		return FALSE;
	while( pQuery->Fetch() )
	{
		int nTick	= pQuery->GetInt( "nTick" );
		u_long idPlayer		= pQuery->GetInt( "idPlayer" );
		float fEFactor	= pQuery->GetFloat( "fEFactor" );
		float fIFactor	= pQuery->GetFloat( "fIFactor" );
		CLEComponent* pComponent	= new CLEComponent( nTick, idPlayer, fEFactor, fIFactor );
		AddComponent( pComponent, FALSE );
	}
	return TRUE;
}

void CLEvent::OnTimer( void )
{	// ƽ
	// ��� ���� �̺�Ʈ�� �ð��� ���ҽ�Ų��
	BOOL bExpired	= DecrementAllComponentTick();
	CDPTrans::GetInstance()->SendLEventTick( this );
	// �ð� ����� �̺�Ʈ�� �ִٸ� �����Ѵ�
	if( bExpired )	
		EraseExpiredComponents();
}

BOOL CLEvent::DecrementAllComponentTick( void )
{	// ��� ���� �̺�Ʈ�� ���� �ð��� ���ҽ�Ű�� ������ ��
	// ���� �ð��� 0�� �̺�Ʈ�� �ִٸ� TRUE�� ��ȯ�Ѵ�
	BOOL bExpired	= FALSE;
	for( VLEC::iterator i = m_vComponents.begin(); i != m_vComponents.end(); ++i )
	{
		if( ( *i )->Decrement() == 0 )
			bExpired	= TRUE;
		m_pLord->GetController()->UpdateLordEventTick( *i );
	}
	return bExpired;
}

BOOL CLEvent::DoTestAddComponent( CLEComponent* pComponent )
{	// �߰��� ���� �̺�Ʈ�� �����ϰ� �����Ѵ�
	// ILordEvent::AddComponent
	CLController* pController	= m_pLord->GetController();
	BOOL bResult	= pController->AddLEComponent( pComponent );
	CDPTrans::GetInstance()->SendLEventCreate( pComponent, bResult );
	return bResult;
}

BOOL CLEvent::DoTestInitialize( void )
{	// ���� �̺�Ʈ�� �ʱ�ȭ�ϰ� ����, ����
	// ILordEvent::Initialize
	CLController* pController	= m_pLord->GetController();
	if( !pController->InitializeLEvent() )
		return FALSE;
	CDPTrans::GetInstance()->SendLEventInitialize();
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
// Ʈ������������ ���ֿ� ���õ� ��� ó���� ����ȭ�Ѵ�(���� ������ ����)
CLController::CLController( CTLord* pLord ) 
:
m_pLord( pLord ), 
CDbController()
{
}

CLController::~CLController()
{
}

void CLController::Handler( LPDB_OVERLAPPED_PLUS pov, ULONG_PTR dwCompletionKey )
{	// ��� ��û�� m_pLord�� �����Ѵ�
	m_pLord->Handler( pov, dwCompletionKey );
}

void CLController::OnTimer( void )
{	// ƽ�� �߻��ϸ� m_pLord�� �˷��ش�
	m_pLord->OnTimer();
}

BOOL CLController::Restore( void )
{	// ��� ���� �ý��� ���� ��ü���� ����
	CQuery* pQuery	= CreateQuery();
	if( !pQuery )
		return FALSE;
	CTElection* pElection	= static_cast<CTElection*>( m_pLord->GetElection() );
	CLEvent* pEvent		= static_cast<CLEvent*>( m_pLord->GetEvent() );
	CTLordSkill* pSkills	= static_cast<CTLordSkill*>( m_pLord->GetSkills() );
	
	if( m_pLord->Restore( pQuery ) && pElection->Restore( pQuery )
		&& pEvent->Restore( pQuery ) && pSkills->Restore( pQuery ) )
	{
		SAFE_DELETE( pQuery );
		return TRUE;
	}
	SAFE_DELETE( pQuery );	
	return FALSE;
}

BOOL CLController::BeginCandidacy( void )
{	// ���ĺ� ���� ���� ����
	char szQuery[255]	= { 0,};
	IElection* pElection	= m_pLord->GetElection();
	FLSPrintf( szQuery, _countof( szQuery ), "uspElectionBeginCandidacy %d, %d", g_appInfo.dwSys, pElection->GetId() );
	return GetQueryObject()->Exec( szQuery );
}

int CLController::BeginVote( void )
{	// ��ǥ ���� ���� ����
	int nRequirement	= -1;
	char szQuery[255]	= { 0,};
	IElection* pElection	= m_pLord->GetElection();
	FLSPrintf( szQuery, _countof( szQuery ), "uspElectionBeginVote %d, %d", g_appInfo.dwSys, pElection->GetId() );
	if( GetQueryObject()->Exec( szQuery ) && GetQueryObject()->Fetch() )
		nRequirement	= GetQueryObject()->GetInt( "nRequirement" );
	return nRequirement;
}

BOOL CLController::EndVote( u_long idPlayer )
{	// ��ǥ ���� ���� ����
	IElection* pElection	= m_pLord->GetElection();
	return GetQueryObject()->Execute( "uspElectionEndVote %d, %d, %d, %s", g_appInfo.dwSys, pElection->GetId(), idPlayer,
		(LPCSTR)CTElection::ttos( pElection->GetNextBegin() ) );
}

BOOL CLController::AddDeposit( u_long idPlayer, __int64 iDeposit, time_t tCreate )
{	// ���� ���� ����
	char szQuery[255]	= { 0,};
	IElection* pElection	= m_pLord->GetElection();
	FLSPrintf( szQuery, _countof( szQuery ), "uspElectionAddDeposit %d, %d, %d, %I64d, %d", g_appInfo.dwSys, pElection->GetId(), idPlayer, iDeposit, tCreate );
	return GetQueryObject()->Exec( szQuery );
}

BOOL CLController::SetPledge( u_long idPlayer, const char* szPledge )
{	// ���� ���� ����
	char szQuery[255]	= { 0,};
	IElection* pElection	= m_pLord->GetElection();
	FLSPrintf( szQuery, _countof( szQuery ), "{call uspElectionSetPledge( %d, %d, %d, ? )}", g_appInfo.dwSys, pElection->GetId(), idPlayer );

	if( !GetQueryObject()->BindParameterVarChar( SQL_PARAM_INPUT, CCandidate::nMaxPledgeLen, const_cast<char*>( szPledge ) ) )
		return FALSE;
	return GetQueryObject()->Exec( szQuery );
}

BOOL CLController::IncVote( u_long idPlayer, u_long idElector )
{	// ��ǥ ����
	// �̹� ��ǥ�� ����ڸ� FALSE ��ȯ
	char szQuery[255]	= { 0,};
	IElection* pElection	= m_pLord->GetElection();
	FLSPrintf( szQuery, _countof( szQuery ), "uspElectionIncVote %d, %d, %d, %d", g_appInfo.dwSys, pElection->GetId(), idPlayer, idElector );
	if( !GetQueryObject()->Exec( szQuery ) )
		return FALSE;
	if( !GetQueryObject()->Fetch() )
		return FALSE;
	return static_cast<BOOL>( GetQueryObject()->GetInt( "bResult" ) );
}

BOOL CLController::AddLEComponent( CLEComponent* pComponent )
{	// ���� �̺�Ʈ ���� ����
	if( GetQueryObject()->Execute( "uspAddLEComponent %d, %d, %d, %3.2f, %3.2f", g_appInfo.dwSys, pComponent->GetIdPlayer(), pComponent->GetTick(), pComponent->GetEFactor(), pComponent->GetIFactor() ) 
		&& GetQueryObject()->Fetch() )
		return static_cast<BOOL>( GetQueryObject()->GetInt( "bResult" ) );
	return FALSE;
}

BOOL CLController::InitializeLEvent( void )
{	// ���� �̺�Ʈ �ʱ�ȭ ����
	return GetQueryObject()->Execute( "uspInitializeLEvent %d", g_appInfo.dwSys );
}

BOOL CLController::UpdateLordSkillTick( CLordSkillComponent* pSkill, int nTick )
{	// ���� ��ų ���� ��� �ð� ����
	return GetQueryObject()->Execute( "uspLordSkillTick %d, %d, %d", g_appInfo.dwSys, pSkill->GetId(), nTick );
}

BOOL CLController::UpdateLordEventTick( CLEComponent* pComponent )
{	// ���� �̺�Ʈ ���� �ð� ����
	return GetQueryObject()->Execute( "uspLordEventTick %d, %d, %d", g_appInfo.dwSys, pComponent->GetIdPlayer(), pComponent->GetTick() );
}
////////////////////////////////////////////////////////////////////////////////
// Ʈ���� ������ ���� ��ų ���� Ŭ����
CTLordSkill::CTLordSkill( CLord* pLord )
: CLordSkill( pLord )
{
}

CTLordSkill::~CTLordSkill()
{
}

CLordSkillComponentExecutable* CTLordSkill::CreateSkillComponent( int /*nType*/ )
{	// Ʈ���� ���������� ��� ���� ��ų��
	// CLordSkillComponentODBC
	return new CLordSkillComponentODBC;
}

BOOL CTLordSkill::Restore( CQuery* pQuery )
{	// �����ͺ��̽��κ��� ���� ��ų ����
	if( !pQuery->Execute( "uspRestoreLordSkill %d", g_appInfo.dwSys ) )
		return FALSE;
	while( pQuery->Fetch() )
	{
			CLordSkillComponent* pComponent	= GetSkill( pQuery->GetInt( "nSkill" ) );
			pComponent->SetTick( pQuery->GetInt( "nTick" ) );
	}
	return TRUE;
}

void CTLordSkill::OnTimer( void )
{	// ƽ
	CLController* pController	= m_pLord->GetController();
	for( VLSC::iterator i = m_vComponents.begin(); i != m_vComponents.end(); ++i )
	{
		CLordSkillComponentExecutable* pSkill	= *i ;
		if( pSkill->GetTick() > 0 )
		{
			// ���� ��� �ð��� ���ҽ�Ų��
			pSkill->SetTick( pSkill->GetTick() - 1 );
			// ����
			pController->UpdateLordSkillTick( pSkill, pSkill->GetTick() );
		}
	}
	CDPTrans::GetInstance()->SendLordSkillTick( this );
}
////////////////////////////////////////////////////////////////////////////////
