#include "stdafx.h"

#include "rangda.h"
#include <worldmng.h>
#include <lord.h>
extern	CWorldMng	g_WorldMng;
#ifndef __M_USER_BOTS
#ifdef __M_RANGDA_EX
#include <sstream>
#include <iomanip>
namespace mu {
	__time64_t str2time(const char* szInput, const char* szFormat) {
		const auto tCurrentTime = ::time(nullptr);
		errno_t err = 0;
		tm tm;
		// converting ::time(nullptr) value to "tm"
		if ((err = localtime_s(&tm, &tCurrentTime)) != 0) {
			Error("Error while converting time %s. Format should be the same as follwing: %s",
				szInput, szFormat);
			return 0LL;
		}
		// set sec, min and hour to 0.
		tm.tm_sec = tm.tm_min = tm.tm_hour = 0;
		// stringstream
		std::basic_istringstream<char, ::std::char_traits<char>, ::std::allocator<char>> ss(szInput);
		ss >> std::get_time(&tm, szFormat);

		_set_errno(0);
		// mktime
		return std::mktime(&tm);
	}
};
#endif
#endif

CRangda::CRangda( DWORD dwMonster )
:
m_dwMonster( dwMonster ),
m_nInterval( 0 ),
m_nReplace( 0 ),
m_bActiveAttack( FALSE ),
m_nGenerateCountdown( 0 ),
m_nReplaceCountdown( 0 ),
m_bReplaceable( TRUE ),
m_nOldPos( -1 ),
m_objid( NULL_ID )
{
}

CRangda::CRangda( DWORD dwMonster, int nInterval, int nReplace, BOOL bActiveAttack )
:
m_dwMonster( dwMonster ),
m_nInterval( nInterval ),
m_nReplace( nReplace ),
m_bActiveAttack( bActiveAttack ),
m_nGenerateCountdown( 0 ),
m_nReplaceCountdown( 0 ),
m_bReplaceable( TRUE ),
m_nOldPos( -1 ),
m_objid( NULL_ID )
{
}

CRangda::~CRangda()
{
}

void CRangda::AddPos( const RANGDA_POS & vPos )
{	// 좌표 추가
	m_vvPos.push_back( vPos );
}

RANGDA_POS CRangda::GetRandomPos( void )
{	// 추가된 좌표 중 하나를 임의로 반환
	FLASSERT( !m_vvPos.empty() );
	
	// 추가된 좌표가 하나라면 아래의 규칙을
	// 따를 수 없으므로 그대로 반환
	if( m_vvPos.size() == 1 )
		return m_vvPos[0];
	
	int nPos;
	do	{	// 출력 좌표는 기존 좌표와 달라야 한다
		nPos	= xRandom( 0, m_vvPos.size() );
	}	while( nPos == m_nOldPos );

	m_nOldPos	= nPos;
	return m_vvPos[nPos];
}

void CRangda::SetObj( OBJID objid )
{	// 현재 생성된 실 객체를 설정
	m_objid		= objid;
	m_nGenerateCountdown	= m_nInterval;
	m_nReplaceCountdown		= m_nReplace;
	m_bReplaceable	= TRUE;
}

void CRangda::OnTimer( void )
{
	CMover* pMonster	= GetMonster();
	if( IsValidObj( pMonster ) )	// 객체가 존재하면
		ProcessReplace( pMonster );		// 조건을 만족하면 임의의 좌표로 이동시킨다
	else	// 객체가 존재하지 않으면 재생
		ProcessGenerate();
}

CMover* CRangda::GetMonster( void )
{	// 실 객체의 포인터를 반환
	if( m_objid != NULL_ID )
	{
		CMover* pMonster	= prj.GetMover( m_objid );
		if( IsValidObj( pMonster ) )
			return pMonster;
		m_objid		= NULL_ID;
	}
	return NULL;
}

void CRangda::ProcessReplace( CMover* pMonster )
{	// 임의의 좌표로 이동
	if( m_bReplaceable )
	{
		if( IsDamaged( pMonster ) )		// 상처 입었으면
			m_bReplaceable	= FALSE;	// 이동시키지 않는다
		else if( HavetoReplace() )	// 이동할 조건을 만족하면
		{
			FLTRACE_LOG( PROGRAM_NAME, _T( "CRangda.ProcessReplace:" ) );
			pMonster->Delete();		// 제거하고
			CreateMonster();	// 새로 생성한다
		}
	}
}

void CRangda::ProcessGenerate( void )
{
	if( HavetoGenerate() )	// 재생 조건을 만족하면
	{
		CreateMonster();	// 새로 생성한다
	}
}

BOOL CRangda::IsDamaged( CMover* pMonster )
{	// 상처 입었나?
	return ( pMonster->GetHitPoint() < pMonster->GetMaxHitPoint() );
}

BOOL CRangda::HavetoReplace( void )
{	// 좌표를 바꿀 시간이 되었나?
	if( m_nReplaceCountdown > 0 )
		m_nReplaceCountdown--;
	return ( m_nReplaceCountdown == 0 );
}

BOOL CRangda::HavetoGenerate( void )
{	// 재생 시간이 되었나?
	if( m_nGenerateCountdown > 0 )
		m_nGenerateCountdown--;
	return ( m_nGenerateCountdown == 0 );
}

void CRangda::CreateMonster( void )
{	// 몬스터 생성
	RANGDA_POS pos	= GetRandomPos();
	CWorld* pWorld	= g_WorldMng.GetWorld( pos.dwWorldId );
	if( !pWorld )
		return;
	CMover* pMonster	= static_cast<CMover*>( ::CreateObj( D3DDEVICE, OT_MOVER, m_dwMonster ) );
	if( pMonster )
	{
		pMonster->m_bActiveAttack	= m_bActiveAttack;
		pMonster->SetPos( pos.vPos );
		pMonster->InitMotion( MTI_STAND );
		pMonster->UpdateLocalMatrix();
		pMonster->AddItToGlobalId();
		if( pWorld->ADDOBJ( pMonster, FALSE, nDefaultLayer ) == TRUE )
		{
			SetObj( pMonster->GetId() );
			FLTRACE_LOG( PROGRAM_NAME, _T( "CRangda.CreateMonster: monster = %d, x = %0.2f, y = %0.2f, z = %0.2f" ),
				pMonster->GetIndex(), pMonster->GetPos().x, pMonster->GetPos().y, pMonster->GetPos().z );
		}
		else
		{
			SAFE_DELETE( pMonster );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// 랜덤 이벤트 몬스터 제어 클래스
CRangdaController::CRangdaController()
{
}

CRangdaController::~CRangdaController()
{
	for( VR::iterator i = m_vRangda.begin(); i != m_vRangda.end(); ++i )
		SAFE_DELETE( *i );
	m_vRangda.clear();
}

CRangdaController* CRangdaController::Instance( void )
{
	static CRangdaController sRangdaController;
	return &sRangdaController;
}

void CRangdaController::AddRangda( CRangda* pRangda )
{
	m_vRangda.push_back( pRangda );
}

void CRangdaController::OnTimer( void )
{
	for( VR::iterator i = m_vRangda.begin(); i != m_vRangda.end(); ++i )
		( *i )->OnTimer();
}

BOOL CRangdaController::LoadScript( const char* szFile )
{
	CScript s;

	TCHAR szFullPath[ MAX_PATH ]	= { 0, };
	g_pScriptFileManager->GetScriptFileFullPath( szFullPath, _countof( szFullPath ), szFile );

	if( s.Load( szFullPath ) == FALSE )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "[ FAILED LOAD SCRIPT. FILE_NAME:(%s) ]" ), szFullPath );
		return FALSE;
	}

	DWORD dwMonster		= s.GetNumber();		// subject or FINISHED
	while( s.tok != FINISHED )
	{
		CRangda* pRangda	= new CRangda( dwMonster );
		AddRangda( pRangda );
		s.GetToken();	// {
		s.GetToken();	// subject or '}'
		
		while( *s.token != '}' )
		{
			if( s.Token == _T( "nInterval" ) )
				pRangda->SetInterval( s.GetNumber() );
			else if( s.Token == _T( "nReplace" ) )
				pRangda->SetReplace( s.GetNumber() );
			else if( s.Token == _T( "bActiveAttack" ) )
				pRangda->SetActiveAttack( static_cast<BOOL>( s.GetNumber() ) );
			else if( s.Token == _T( "vRangda" ) )
			{
				RANGDA_POS pos;
				s.GetToken();	// {
				pos.dwWorldId	= s.GetNumber();
				while( *s.token != '}' )
				{
					pos.vPos.x	= s.GetFloat();
					pos.vPos.y	= s.GetFloat();
					pos.vPos.z	= s.GetFloat();
					pRangda->AddPos( pos );
					pos.dwWorldId	= s.GetNumber();
				}
			}
			s.GetToken();
		}
		dwMonster	= s.GetNumber();
	}
	return TRUE;
}

