#include "StdAfx.h"
#include "Player.h"
#include <msghdr.h>
#include "dpcachesrvr.h"
#include "dploginsrvr.h"
#include <party.h>
#include <guild.h>
#include "dpdatabaseclient.h"
#include "dpcoresrvr.h"

extern	CDPCacheSrvr		g_DPCacheSrvr;
extern	CDPLoginSrvr		g_dpLoginSrvr;
extern	CGuildMng			g_GuildMng;
extern	CPartyMng			g_PartyMng;
extern	CDPDatabaseClient	g_dpDatabaseClient;
extern CDPCoreSrvr			g_dpCoreSrvr;
CPlayerMng	g_PlayerMng;

MEMPOOLER_IMPLEMENT( CPlayer, 512 );

CPlayer::CPlayer( u_long idPlayer, const CHAR* pszPlayer, const CHAR* pszAccount 
#ifdef __M_USER_BOTS
	, const u_long uSrvrKey
#endif
)
{
	m_dwSerial = 0;
	dpid = dpidCache = dpidUser	= DPID_UNKNOWN;
	uKey = idPlayer;
	FLStrcpy( lpszPlayer, _countof( lpszPlayer ), pszPlayer );
	FLStrcpy( lpszAccount, _countof( lpszAccount ), pszAccount );
	*lpAddr	= '\0';
	m_uPartyId = 0;
	m_idGuild = 0;
	m_idWar	= 0;
	m_uIdofMulti = 0;
	m_tGuildMember = CTime::GetCurrentTime();
#ifdef __M_USER_BOTS
	this->uSrvrKey = uSrvrKey;
#endif
	m_idSnoop = 0;
}

CPlayer::~CPlayer()
{
}

TAG_RESULT CPlayer::IsTagSendable( u_long idTo )
{
	Friend* pFriend		= m_RTMessenger.GetFriend( idTo );
	if( !pFriend )
		return TAG_NOTFRIEND;
	if( pFriend->bBlock )
		return TAG_BLOCKED;
	return TAG_OK;
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/
CPlayerMng::CPlayerMng()
{
	m_uCount	= 0;
}

CPlayerMng::~CPlayerMng()
{
	Free();
}

void CPlayerMng::Free()
{
	CMclAutoLock Lock( m_AddRemoveLock );

	map<DWORD, CPlayer*>::iterator it;
	for( it = m_players.begin(); it != m_players.end(); ++it )
	{
		SAFE_DELETE( it->second );
	}
	m_players.clear();

	m_uCount	= 0;
	m_ulong2.clear();
}

BOOL CPlayerMng::AddCache( DPID /*dpidCache*/ )
{
	return TRUE;
}

BOOL CPlayerMng::RemoveCache( DPID /*dpidCache*/ )
{
	CMclAutoLock Lock( m_AddRemoveLock );

	map<DWORD, CPlayer*>::iterator it;
	for( it = m_players.begin(); it != m_players.end(); ++it )
	{
		CPlayer* pPlayer = it->second;
		if( *pPlayer->lpszAccount != '\0' )
			g_dpLoginSrvr.SendQueryRemovePlayer( pPlayer->lpszAccount );
		else
			FLERROR_LOG( PROGRAM_NAME, _T( "*pPlayer->lpszAccount == '\0'" ) );
	}

	m_uCount = 0;
	return TRUE;
}

BOOL CPlayerMng::AddPlayer( u_long idPlayer, const CHAR* lpszPlayer, const CHAR* lpszAccount 
#ifdef __M_USER_BOTS
	, const u_long uSrvrKey
#endif
)
{
	CPlayer* pPlayer = GetPlayer( idPlayer );
	if( pPlayer )
		return FALSE;
 
	pPlayer		= new CPlayer( idPlayer, lpszPlayer, lpszAccount 
#ifdef __M_USER_BOTS
		, uSrvrKey
#endif
	);	
	m_ulong2.insert( map<u_long, CPlayer*>::value_type( idPlayer, pPlayer ) );
	return TRUE;
}


BOOL CPlayerMng::UnregisterPlayerInfo( CPlayer* pPlayer, BOOL bNotify )
{
	Logout( pPlayer );

	g_PartyMng.RemoveConnection( pPlayer );
	g_GuildMng.RemoveConnection( pPlayer );

	pPlayer->Lock();
	
	// messenger
	pPlayer->m_RTMessenger.SetState( FRS_OFFLINE );
	for( map<u_long, Friend>::iterator i = pPlayer->m_RTMessenger.begin(); i != pPlayer->m_RTMessenger.end(); ++i )
	{
		CPlayer* pPlayertmp		= GetPlayer( i->first );
		if( pPlayertmp )
			g_DPCacheSrvr.SendFriendLogOut( pPlayertmp, pPlayer->uKey );
	}

	if( pPlayer == GetPlayerBySerial( pPlayer->m_dwSerial ) )
	{
		m_players.erase( pPlayer->m_dwSerial );
		m_uCount--;
		if( bNotify )
		{
			BEFORESENDSOLE( ar, PACKETTYPE_LEAVE, pPlayer->dpidUser );
			ar << pPlayer->m_dwSerial;
			SEND( ar, &g_DPCacheSrvr, pPlayer->dpidCache );
		}
	}
	pPlayer->dpidCache = pPlayer->dpidUser	= DPID_UNKNOWN;	

	pPlayer->Unlock();
	return TRUE;
}

BOOL CPlayerMng::RegisterPlayerInfo( CPlayer* pPlayer )
{
	m_players.insert( make_pair( pPlayer->m_dwSerial, pPlayer ) );
	m_uCount++;
	g_PartyMng.AddConnection( pPlayer );
	g_GuildMng.AddConnection( pPlayer );

	vector< u_long > vecIdFriend;
	// 여기 루틴은 나를 등록한 사람에게 내가 들어왔다는 메세지를 날려주는 것임
	pPlayer->Lock();
	
	for( map<u_long, Friend>::iterator i = pPlayer->m_RTMessenger.begin(); i != pPlayer->m_RTMessenger.end(); ++i )
	{
		Friend* pFriend		= &i->second;
		if( !pFriend->bBlock )
			vecIdFriend.push_back( i->first );
	}

	pPlayer->Unlock();

	for( DWORD j = 0 ; j < vecIdFriend.size() ; j++ )
	{
		CPlayer *pFriendPlayer = GetPlayer( vecIdFriend[j] );
		if( pFriendPlayer )
		{
			g_DPCacheSrvr.SendFriendJoin( pFriendPlayer, pPlayer );
		}
	}
	
	return TRUE;
}

// bNotify - CACHE서버에게 플레이러 제거를 알릴 것인가?
void CPlayerMng::RemovePlayer( CPlayer* pPlayer, BOOL bNotify )
{
	UnregisterPlayerInfo( pPlayer, bNotify );				// m_players에서 제거 

	m_ulong2.erase( pPlayer->uKey );
	g_dpCoreSrvr.SendRemoveUser( pPlayer->m_dwSerial );		// 월드 서버에서도 제거하게 한다.

	m_set.erase( pPlayer->uKey );				
	SAFE_DELETE( pPlayer );
}

CPlayer* CPlayerMng::GetPlayerBySerial( DWORD dwSerial )
{
	map< DWORD, CPlayer* >::iterator it = m_players.find( dwSerial );
	if( it != m_players.end() )
		return it->second;

	return NULL;
}

CPlayer* CPlayerMng::GetPlayerBySocket( DPID dpidSocket )
{
	map< DWORD, CPlayer* >::iterator it;
	for( it = m_players.begin(); it != m_players.end(); ++it )
	{
		CPlayer* p = it->second;
		if( p->dpidUser == dpidSocket )
			return p;
	}
	return NULL;
}

CPlayer* CPlayerMng::GetPlayer( u_long uKey )
{
	ULONG2PTR::iterator i	= m_ulong2.find( uKey );
	if( i != m_ulong2.end() )
		return i->second;
	return NULL;
}

void CPlayerMng::PackName( CAr & ar )
{
	CMclAutoLock Lock( m_AddRemoveLock );

	ar << m_uCount;
	map< DWORD, CPlayer* >::iterator it;

	for( it = m_players.begin(); it != m_players.end(); ++ it )
	{
		CPlayer* pPlayer = it->second;
		ar.WriteString( pPlayer->lpszPlayer );
	}
}

void CPlayerMng::AddItToSetofOperator( u_long uPlayerId )
{
//	CMclAutoLock	Lock( m_AddRemoveLock );
	m_set.insert( uPlayerId );
}

BOOL CPlayerMng::IsOperator( u_long idPlayer )
{
		CMclAutoLock	Lock( m_AddRemoveLock );
		set<u_long>::iterator i	= m_set.find( idPlayer );
		return (BOOL)( i != m_set.end() );
}

u_long CPlayerMng::GetCount( void )
{	
	return m_uCount;	
}

void	CPlayerMng::Logout( CPlayer * pPlayer )
{
	CMclAutoLock	Lock( m_AddRemoveLock );
	map<u_long, int>	mapIdPlayer;
	if( pPlayer )
	{
		u_long idPlayer	= pPlayer->uKey;
		//
		{
			CMclAutoLock	Lock2( g_GuildMng.m_AddRemoveLock );
			CGuild* pGuild	= g_GuildMng.GetGuild( pPlayer->m_idGuild );
			if( pGuild && pGuild->IsMember( idPlayer ) )
			{
				for( map<u_long, CGuildMember*>::iterator i = pGuild->m_mapPMember.begin(); i != pGuild->m_mapPMember.end(); ++i )
					mapIdPlayer.insert( map<u_long, u_long>::value_type( i->second->m_idPlayer, 0 ) );
			}
		}
		//
		{
			CMclAutoLock	Lock3( g_PartyMng.m_AddRemoveLock );
			CParty* pParty	= g_PartyMng.GetParty( pPlayer->m_uPartyId );
			if( pParty && pParty->IsMember( idPlayer ) )
			{
				for( int i = 0; i < pParty->m_nSizeofMember; i++ )
					mapIdPlayer.insert( map<u_long, u_long>::value_type( pParty->m_aMember[i].m_uPlayerId, 0 ) );
			}
		}
		//
		{
			pPlayer->Lock();
			for( map<u_long, Friend>::iterator i = pPlayer->m_RTMessenger.begin(); i != pPlayer->m_RTMessenger.end(); ++i )
				mapIdPlayer.insert( map<u_long, u_long>::value_type( i->first, 0 ) );
			pPlayer->Unlock();
		}
		for( map<u_long, int>::iterator i = mapIdPlayer.begin(); i != mapIdPlayer.end(); ++i )
		{
			CPlayer* pTo	= g_PlayerMng.GetPlayer( i->first );
			if( pTo )
				g_DPCacheSrvr.SendLogout( idPlayer, pTo );
		}
	}


}
#ifdef __M_USER_BOTS_TERMINATE_FIX_WS
void	CPlayerMng::removeAllServerPlayer(const u_long uKey, const bool bOnlyBot) {
	for (auto it = m_players.begin(); it != m_players.end();) {
		auto pPlayer = (*it).second;
		if (!pPlayer || pPlayer && bOnlyBot && !pPlayer->isBot()) {
			it++; continue;
		}
		if (uKey == pPlayer->uSrvrKey) {
			UnregisterPlayerInfo(pPlayer, FALSE);
			m_set.erase(pPlayer->uKey);
			g_dpCoreSrvr.SendRemoveUser(pPlayer->m_dwSerial);
			SAFE_DELETE(pPlayer);
			it = m_players.erase(it);

		}
		else {
			it++;
		}
	}
	for (auto it = m_ulong2.begin(); it != m_ulong2.end();) {
		if (!it->second || it->second->uSrvrKey == uKey) {
			if (it->second && !it->second->isBot()) {
				it++;
				continue;
			}
			it = m_ulong2.erase(it);
		}
		else {
			it++;
		}
	}
}
#endif
