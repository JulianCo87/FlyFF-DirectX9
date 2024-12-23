
#include "StdAfx.h"

#include "playerdata.h"

///////////////////////////////////////////////////////////////////////////////////////////////
//GlobalTickManager
GlobalTickManager::GlobalTickManager( ) :
_dwLastTime( g_tmCurrent ),
_dwDelta( 0 )
{
}

GlobalTickManager::~GlobalTickManager( ) 
{
}

void GlobalTickManager::Update( )
{
	_dwDelta = g_tmCurrent - _dwLastTime;
	_dwLastTime = g_tmCurrent;
}

GlobalTickManager* GlobalTickManager::GetThisPtr()
{
	static GlobalTickManager stGTM;
	return &stGTM;
}

void GlobalTickManager::GetHMSbySec( int sec, OUT int& h, OUT int& m, OUT int& s )
{
	//�� �ð�(��)�� �ð�, ��, �ʷ� �и��Ѵ�.
	h = sec / 3600;
	sec = sec - h * 3600;
	m = sec / 60;
	sec = sec - m * 60;
	s = sec;
}

void GlobalTickManager::GetDHMbySec( int sec, OUT int& d, OUT int& h, OUT int& m )
{
	//�� �ð�(��)�� ��, �ð�, �� ���� �и��Ѵ�.
	d = sec / ( 3600 * 24 );
	sec = sec - d * ( 3600 * 24 );
	h = sec / 3600;
	sec = sec - h * 3600;
	m = sec / 60;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//EAPlayers
EAPlayers* EAPlayers::GetThisPtr( )
{
	static EAPlayers stThis;
	return &stThis;
}

int EAPlayers::GetTeam( const std::string& strName )
{
	if( TRUE == IsRedTeam( strName ) )
		return EA_REDTEAM;
	else if( TRUE == IsBlueTeam( strName ) )
		return EA_BLUETEAM;

	return EA_NOTEAM;
}

BOOL EAPlayers::IsRedTeam( const std::string& strName )
{
	for( StringIter iter = m_vecRedTeamMembers.begin(); iter != m_vecRedTeamMembers.end(); ++iter )
	{
		std::string& strDataName = *iter;
		if( strDataName == strName )
			return TRUE;
	}

	return FALSE;

	//StringIter iter = std::find( m_vecRedTeamMembers.begin(), m_vecRedTeamMembers.end(), strName );
	//if( iter != m_vecRedTeamMembers.end() )
	//	return TRUE;

	//return FALSE;
}

BOOL EAPlayers::IsBlueTeam( const std::string& strName )
{
	for( StringIter iter = m_vecBlueTeamMembers.begin(); iter != m_vecBlueTeamMembers.end(); ++iter )
	{
		std::string& strDataName = *iter;
		if( strDataName == strName )
			return TRUE;
	}

	return FALSE;

	//StringIter iter = std::find( m_vecBlueTeamMembers.begin(), m_vecBlueTeamMembers.end(), strName );
	//if( iter != m_vecBlueTeamMembers.end() )
	//	return TRUE;

	//return FALSE;
}

void EAPlayers::Update( )
{
	BOOL bReset = FALSE;
	if( m_vecBlueTeamMembers.empty() && m_vecRedTeamMembers.empty() )
		bReset = TRUE;

	//��� �÷��̾ ���Ͽ� EA team���� ������Ʈ
	CMover* pMover = NULL;
	map<string, u_long> mapData = CPlayerDataCenter::GetInstance()->GetContainer_PlayerStringToId( );
	for( map<string, u_long>::iterator iter = mapData.begin(); iter != mapData.end(); ++iter )
	{
		OBJID dwObjID = iter->second;
		pMover = prj.GetUserByID( dwObjID );
		if( pMover && pMover->GetType() == OT_MOVER )
		{
			const std::string& strName = iter->first;
			if( bReset )
				pMover->m_nEATeam = EA_NOTEAM;
			else 
				pMover->m_nEATeam = GetTeam( strName );
		}
		else
		{
			//FLASSERT( 0 );
		}
	}
}

void EAPlayers::UpdatePlayer( CMover* pMover )
{
	//���� ������� �������� ���Ͽ��� ������Ʈ
	if( NULL == pMover )
		return;
	
	std::string strName = pMover->GetName();
	int nTeam = EAPlayers::GetTeam( strName );
	pMover->m_nEATeam = nTeam;
}

BOOL EAPlayers::IsEnemy( const int nTeam, CMover* pMover )
{
	//nteam ���� ��
	//pMover : �˻��� ����
	switch( nTeam )
	{
	case EA_REDTEAM:
		return IsBlueTeam( pMover->GetName() );

	case EA_BLUETEAM:
		return IsRedTeam( pMover->GetName() );

	default :
		return FALSE;
	}

	return FALSE;

}