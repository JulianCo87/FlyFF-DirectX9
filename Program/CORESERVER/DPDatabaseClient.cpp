#include "stdafx.h"
#include "player.h"
#include "dpcachesrvr.h"
#include "DPCoreSrvr.h"
#include "dpdatabaseclient.h"


#include <party.h>
#include <guild.h>
#include <guildwar.h>

#include "WantedList.h"

extern	CPartyMng		g_PartyMng;
extern	CGuildMng		g_GuildMng;
extern	CGuildWarMng	g_GuildWarMng;

extern	CPlayerMng		g_PlayerMng;
extern	CDPCacheSrvr	g_DPCacheSrvr;
extern  CDPCoreSrvr		g_dpCoreSrvr;

CDPDatabaseClient::CDPDatabaseClient()
{
	ON_MSG( PACKETTYPE_PARTYNAME, OnPartyName );
	ON_MSG( PACKETTYPE_GLOBAL_DATA, OnGlobalData );
	ON_MSG( PACKETTYPE_INSERTTAG_RESULT, OnInsertTagResult );
	ON_MSG( PACKETTYPE_DC_ADDVOTERESULT, OnAddVoteResult );
	ON_MSG( PACKETTYPE_UPDATE_GUILD_RANKING_END, OnUpdateGuildRankFinish );
	ON_MSG( PACKETTYPE_DEL_PLAYER, OnDelPlayer );
	ON_MSG( PACKETTYPE_GUILD_MEMBERTIME, OnGuildMemberTime );
	ON_MSG( PACKETTYPE_BUYING_INFO, OnBuyingInfo );
	ON_MSG( PACKETTYPE_TC_LIST, OnTCList );
	ON_MSG( PACKETTYPE_QUERYSETPLAYERNAME, OnSetPlayerName );

	ON_MSG( PACKETTYPE_UPDATE_PLAYER_DATA, OnUpdatePlayerData );

	ON_MSG( PACKETTYPE_EVENTLUA_NOTICE, OnEventNotice );

	ON_MSG( PACKETTYPE_REMOVEFRIEND, OnRemovePlayerFriend );

	ON_MSG( PACKETTYPE_CLOSE_EXISTING_CONNECTION, OnCloseExistingConnection );
#ifdef __M_USER_BOTS_DB_BASE
	ON_MSG(PACKETTYPE_REMOVE_BOT, onRemoveBot);
	ON_MSG(PACKETTYPE_ADD_BOT, onAddBot);
#endif
}

CDPDatabaseClient::~CDPDatabaseClient()
{

}

void CDPDatabaseClient::SysMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD /*dwMsgSize*/, DPID /*dpId*/ )
{
	switch( lpMsg->dwType )
	{
		case DPSYS_CREATEPLAYERORGROUP:
			#if !defined(__INTERNALSERVER )
				SendCTWanted( CT_WANTED_REQ_LIST, 0, 0, time(NULL), "" );  // 리스트 요청 
			#endif 
			break;
		case DPSYS_DESTROYPLAYERORGROUP:
			break;
	}
}

void CDPDatabaseClient::UserMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID /*dpId*/ )
{
	CAr ar( (LPBYTE)lpMsg, dwMsgSize );

	GETTYPE( ar );
	
	PACKET_HANDLER_FUNC pfn	=	GetHandler( dw );

	if( pfn ) {
		( this->*( pfn ) )( ar );
	}
//	else {
//		switch( dw ) {
//			default:
//				FLTRACE_LOG( PROGRAM_NAME, _T( "Handler not found(%08x)" ), dw );
//				FLASSERT( 0 );
//				break;
//		}
//	}
}

void CDPDatabaseClient::OnPartyName( CAr & ar )
{
	u_long uSize;
	u_long uidPlayer;
	char szPartyName[36];
	
	ar >> uSize;
	for( DWORD i = 0 ; i < uSize ; i++ )
	{
		ar >> uidPlayer;
		ar.ReadString( szPartyName, _countof( szPartyName ) );	
		g_PartyMng.AddPartyName( uidPlayer, szPartyName );
	}
}

void CDPDatabaseClient::SendCreateGuild( GUILD_MEMBER_INFO* info, int nSize, u_long idGuild, const char* szGuild )
{
	BEFORESEND( ar, PACKETTYPE_CREATE_GUILD );
	ar << nSize;
	ar.Write( info, sizeof(GUILD_MEMBER_INFO)*nSize );
	ar << idGuild;
	ar.WriteString( szGuild );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::SendDestroyGuild( u_long idGuild, u_long idMaster )
{
	BEFORESEND( ar, PACKETTYPE_DESTROY_GUILD );
	ar << idGuild;
	ar << idMaster;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::SendAddGuildMember( u_long idPlayer, u_long idGuild, u_long DoidPlayer )
{
	BEFORESEND( ar, PACKETTYPE_ADD_GUILD_MEMBER );
	ar << idPlayer << idGuild << DoidPlayer;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::SendRemoveGuildMember( u_long idPlayer, u_long idGuild, u_long DoidPlayer )
{
	BEFORESEND( ar, PACKETTYPE_REMOVE_GUILD_MEMBER );
	ar << idPlayer << idGuild << DoidPlayer;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::SendGuildMemberLv( u_long idPlayer, int nMemberLv )
{
	BEFORESEND( ar, PACKETTYPE_GUILD_MEMBER_LEVEL );
	ar << idPlayer << nMemberLv;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::SendGuildClass( u_long idPlayer, int nClass )
{
	BEFORESEND( ar, PACKETTYPE_GUILD_CLASS );
	ar << idPlayer << nClass;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::SendGuildNickName( u_long idPlayer, LPCTSTR strNickName )
{
	BEFORESEND( ar, PACKETTYPE_GUILD_NICKNAME );
	ar << idPlayer;
	ar.WriteString( strNickName );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::SendChgMaster( u_long idPlayer, u_long idPlayer2 )
{
	BEFORESEND( ar, PACKETTYPE_CHG_MASTER );
	ar << idPlayer << idPlayer2;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::SendAcptWar( u_long idWar, u_long idDecl, u_long idAcpt )
{
	BEFORESEND( ar, PACKETTYPE_ACPT_GUILD_WAR );
	ar << idWar << idDecl << idAcpt;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::SendWarEnd( u_long idWar, u_long idDecl, u_long idAcpt, int nWptDecl, int nWptAcpt, int nType, CTime Time, int nWinPointDecl, int nWinPointAcpt, int nGetPointDecl, int nGetPointAcpt )
{
	char GuildWarStartTime[15] = {0,};
	
	FLSPrintf( GuildWarStartTime, _countof( GuildWarStartTime ), "%s", Time.Format( "%Y%m%d%H%M%S" ) );

	BEFORESEND( ar, PACKETTYPE_WAR_END );
	ar << idWar << idDecl << idAcpt << nWptDecl << nWptAcpt << nType;
	ar.WriteString( GuildWarStartTime );
	ar << nWinPointDecl << nWinPointAcpt << nGetPointDecl << nGetPointAcpt;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::SendSurrender( u_long idWar, u_long idPlayer, u_long idGuild )
{
	BEFORESEND( ar, PACKETTYPE_SURRENDER );
	ar << idWar << idPlayer << idGuild;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::SendWarDead( u_long idWar, u_long idGuild )
{
	BEFORESEND( ar, PACKETTYPE_WAR_DEAD );
	ar << idWar << idGuild;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::SendWarMasterAbsent( u_long idWar, u_long idGuild )
{
	BEFORESEND( ar, PACKETTYPE_WAR_MASTER_ABSENT );
	ar << idWar << idGuild;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::SendGuildLogo( u_long idGuild, DWORD dwLogo )
{
	BEFORESEND( ar, PACKETTYPE_CD_GUILD_LOGO );
	ar << idGuild << dwLogo;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::SendGuildNotice( u_long idGuild, LPCTSTR szNotice )
{
	BEFORESEND( ar, PACKETTYPE_CD_GUILD_NOTICE );
	ar << idGuild;
	ar.WriteString( szNotice );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::SendGuildAuthority( u_long idGuild, DWORD adwAuthority[] )
{
	BEFORESEND( ar, PACKETTYPE_GUILD_AUTHORITY );
	ar << idGuild;
	ar.Write( adwAuthority, sizeof(DWORD) * MAX_GM_LEVEL );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::SendGuildPenya( u_long idGuild, DWORD adwPenya[] )
{
	BEFORESEND( ar, PACKETTYPE_GUILD_PENYA );
	ar << idGuild;
	ar.Write( adwPenya, sizeof(DWORD) * MAX_GM_LEVEL );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::SendGuildSetName( u_long uidGuild, char * szName )
{
	BEFORESEND( ar, PACKETTYPE_GUILD_SETNAME );
	ar << uidGuild;
	ar.WriteString( szName );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::SendAddPartyName( u_long uidPlayer, const char * sParty )
{
	BEFORESEND( ar, PACKETTYPE_ADDPARTYNAME );
	ar << uidPlayer;
	ar.WriteString( sParty );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::OnGlobalData( CAr & ar )
{
	g_GuildMng.Serialize( ar, FALSE );
	CGuildTable::GetInstance().Serialize( ar );
	g_GuildWarMng.Serialize( ar );
	ar.Read( (void*)g_PartyMng.m_aExpParty, sizeof(g_PartyMng.m_aExpParty) );
	
#if defined(__INTERNALSERVER )	
	OnTCList( ar );		//현상금 리스트를 얻는다.
#endif // __INTERNALSERVER
	
	extern DWORD   g_dwId;
	NotifyProcessStart( g_dwId );	
}


void CDPDatabaseClient::SendTag( u_long idFrom, u_long idTo, LPCTSTR lpszString )
{
	BEFORESEND( ar, PACKETTYPE_TAG );
	
	ar << idFrom << idTo;
	ar.WriteString( lpszString );
	
	SEND( ar, this, DPID_SERVERPLAYER );
}


void CDPDatabaseClient::OnInsertTagResult( CAr & ar )
{
	u_long idFrom;
	BYTE cbResult;

	ar >> idFrom;
	ar >> cbResult;

	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	CPlayer* pFrom	= g_PlayerMng.GetPlayer( idFrom );

	if( pFrom == NULL)
		return;

	g_DPCacheSrvr.SendTagResult( pFrom, cbResult);
}


void CDPDatabaseClient::OnAddVoteResult( CAr & ar )
{
	VOTE_INSERTED_INFO	info;
	ar >> info;						

	if( info.idVote )
	{
		CMclAutoLock	Lock( g_GuildMng.m_AddRemoveLock );	
		
		CGuild* pGuild = g_GuildMng.GetGuild( info.idGuild );
		if( pGuild )
			pGuild->AddVote( info );
	}

	g_dpCoreSrvr.SendAddVoteResult( info );		// 모든 월드서버에 알린다.
}

void CDPDatabaseClient::SendAddVote( u_long idGuild, const char* szTitle, const char* szQuestion, char szSelections[4][MAX_BYTE_VOTESELECT] )
{
	BEFORESEND( ar, PACKETTYPE_CD_ADDVOTE );
	
	ar << idGuild;
	ar.WriteString( szTitle );
	ar.WriteString( szQuestion );
	for( int i=0; i<4; ++i)
		ar.WriteString( szSelections[i] );
	
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::SendRemoveVote( u_long idVote )
{
	BEFORESEND( ar, PACKETTYPE_CD_REMOVEVOTE );
	ar << idVote;
	
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::SendCloseVote( u_long idVote )
{
	BEFORESEND( ar, PACKETTYPE_CD_CLOSEVOTE );
	ar << idVote;
	
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::SendCastVote( u_long idVote, BYTE cbSelection )
{
	BEFORESEND( ar, PACKETTYPE_CD_CASTVOTE );
	ar << idVote;
	ar << cbSelection;	

	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::OnUpdateGuildRankFinish( CAr & ar )
{
	// 길드 랭크 정보를 시리얼라이즈 한다.
	CGuildRank* pGuildRank = CGuildRank::Instance();
	pGuildRank->Serialize( ar );

	g_DPCacheSrvr.SendUpdateGuildRank();
}

void CDPDatabaseClient::OnDelPlayer( CAr & ar )
{
	u_long idGuild, idPlayer;
	ar >> idPlayer >> idGuild;

	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	CMclAutoLock	Lock2( g_GuildMng.m_AddRemoveLock );
	
	CGuild* pGuild	= g_GuildMng.GetGuild( idGuild );
	
	if( NULL == pGuild )
		return;
	if( FALSE == pGuild->IsMember( idPlayer ) )
		return;
	
	if( pGuild->IsMaster( idPlayer ) )	// master
	{
		CGuildMember* pMember;
		CPlayer* pPlayer;
		for( map<u_long, CGuildMember*>::iterator i = pGuild->m_mapPMember.begin();
			i != pGuild->m_mapPMember.end(); ++i )
		{
			pMember		= i->second;
			pPlayer	= g_PlayerMng.GetPlayer( pMember->m_idPlayer );
			if( pPlayer )
			{
				pPlayer->m_tGuildMember = CTime::GetCurrentTime();
				pPlayer->m_tGuildMember += CTimeSpan( 7, 0, 0, 0 );
				pPlayer->m_idGuild	= 0;
			}
		}
		g_GuildMng.RemoveGuild( idGuild );
		SendDestroyGuild( idGuild, idPlayer );	// databaseserver
		g_dpCoreSrvr.SendDestroyGuild( idGuild );	// worldserver
	}
	else	// member
	{
		if( pGuild->RemoveMember( idPlayer ) )
		{
			CPlayer* pPlayer;
			for( map<u_long, CGuildMember*>::iterator i = pGuild->m_mapPMember.begin();
				i != pGuild->m_mapPMember.end(); ++i )
			{
				pPlayer	= g_PlayerMng.GetPlayer( i->second->m_idPlayer );
				if( pPlayer )
					g_DPCacheSrvr.SendRemoveGuildMember( idPlayer, pGuild->m_idGuild, pPlayer );
			}
			g_dpCoreSrvr.SendRemoveGuildMember( idPlayer, idGuild );	// worldserver
			SendRemoveGuildMember( idPlayer, idGuild, pGuild->m_idMaster );		// databaseserver
		}
	}
}

void CDPDatabaseClient::OnGuildMemberTime( CAr & ar )
{
	u_long idPlayer;
	char GuildMemberTime[15] = {0,};
	ar >> idPlayer;
	ar.ReadString( GuildMemberTime, _countof( GuildMemberTime ) );		


	//////////////////////////////////////////////////////////////////////////
	// mirchang_20100706
	if( _tcslen( GuildMemberTime ) == 0 )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "GuildMemberTime Is Null. User:[%07d]" ), idPlayer );
		return;
	}
	//////////////////////////////////////////////////////////////////////////


	char cYear[5] = {0,};
	char cMonth[3] = {0,};
	char cDay[3] = {0,};
	char cHour[3] = {0,};
	char cMin[3] = {0,};
	
	FLStrncpy(cYear, _countof( cYear ), GuildMemberTime, 4);
	FLStrncpy(cMonth, _countof( cMonth ), GuildMemberTime + 4, 2 );
	FLStrncpy(cDay, _countof( cDay ), GuildMemberTime + 6, 2 );
	FLStrncpy(cHour, _countof( cHour ), GuildMemberTime + 8, 2 );
	FLStrncpy(cMin, _countof( cMin ), GuildMemberTime + 10, 2 );



	//////////////////////////////////////////////////////////////////////////
	// mirchang_20100706
	int nYear, nMonth, nDay, nHour, nMin;
	nYear = nMonth = nDay = nHour = nMin = 0;

	nYear = atoi( cYear );
	nMonth = atoi( cMonth );
	nDay = atoi( cDay );
	nHour = atoi( cHour );
	nMin = atoi( cMin );

	struct tm atm;
	atm.tm_sec = 0;
	atm.tm_min = nMin;
	atm.tm_hour = nHour;
	atm.tm_mday = nDay;
	atm.tm_mon = nMonth - 1;
	atm.tm_year = nYear - 1900;
	atm.tm_isdst = -1;
	__time64_t tTimeTmp = _mktime64(&atm);

	if( tTimeTmp == -1 )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "CTime Is Invalid. szTime:[%s], Y:[%04d], M:[%02d], D:[%02d], H:[%02d], M:[%02d]" ), GuildMemberTime, nYear, nMonth, nDay, nHour, nMin );
		return;
	}
	//////////////////////////////////////////////////////////////////////////


	
	CTime tGuildMember( nYear, nMonth, nDay, nHour, nMin, 0 );

	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );

	CPlayer* pPlayer	= g_PlayerMng.GetPlayer( idPlayer );
	if( pPlayer )
	{
		pPlayer->m_tGuildMember = tGuildMember;
	}	
}

void CDPDatabaseClient::OnBuyingInfo( CAr & ar )
{
	FLTRACE_LOG( PROGRAM_NAME, _T( "CORESERVER.EXE\t// recv from trans" ) );

	BUYING_INFO2 bi2;
	ar.Read( (void*)&bi2, sizeof(BUYING_INFO2) );

	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	CPlayer* pPlayer	= g_PlayerMng.GetPlayer( bi2.dwPlayerId );
// 	if( pPlayer != NULL && g_DPCacheSrvr.IsEmptyConnectedClient() == false )
// 	{
// 		g_DPCacheSrvr.SendBuyingInfo( &bi2, pPlayer );
// 	}

	if( pPlayer != NULL )
	{
		DPID WorldServerDPID	= g_dpCoreSrvr.GetWorldSrvrDPIDByMultiID( pPlayer->m_uIdofMulti );
		if( WorldServerDPID != DPID_UNKNOWN)
		{
			g_dpCoreSrvr.SendBuyingInfo( &bi2, WorldServerDPID );
		}
		else
		{
			SendBuyingInfo( &bi2 );
		}
	}
	else
	{
		// 해당 캐릭터가 접속되어있지 않다.. 트랜스로 결과 전송
		SendBuyingInfo( &bi2 );
	}
}

// 현상금시스템 관련 요청 
void CDPDatabaseClient::SendCTWanted( BYTE byReqType, u_long idPlayer, int nGold, long nEnd, LPCTSTR szMsg )
{
	BEFORESEND( ar, PACKETTYPE_CT_WANTED );
	ar << byReqType;
	ar << idPlayer;
	ar << nGold;
	ar << nEnd;
	ar.WriteString( szMsg );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::SendBuyingInfo( PBUYING_INFO2 pbi2 )
{
	BEFORESEND( ar, PACKETTYPE_BUYING_INFO );
	ar.Write( (void*)pbi2, sizeof(BUYING_INFO2) );
	SEND( ar, this, DPID_SERVERPLAYER );
}

// 현상금 리스트 
void CDPDatabaseClient::OnTCList( CAr & ar )
{
	CWantedList& wantedList = CWantedList::GetInstance();
	wantedList.Read( ar );
}

void CDPDatabaseClient::OnSetPlayerName( CAr & ar )
{
	FLTRACE_LOG( PROGRAM_NAME, _T( "CORESERVER.EXE\tPACKETTYPE_QUERYSETPLAYERNAME" ) );
	u_long idPlayer;
	char lpszPlayer[MAX_PLAYER]	= { 0, };
	DWORD dwData;
	BOOL f;
	ar >> idPlayer;
	ar.ReadString( lpszPlayer, _countof( lpszPlayer ) );
	ar >>dwData >> f;
	if( f )
	{
		CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
		CPlayer* pPlayer	= g_PlayerMng.GetPlayer( idPlayer );
		if( pPlayer )
		{
//			g_PlayerMng.m_str2.erase( pPlayer->lpszPlayer );
			FLStrcpy( pPlayer->lpszPlayer, _countof( pPlayer->lpszPlayer ), lpszPlayer );
//			g_PlayerMng.m_str2.insert( map<string, CPlayer*>::value_type( pPlayer->lpszPlayer, pPlayer ) );
		}
		CWantedList::GetInstance().Rename( idPlayer, lpszPlayer );
		g_DPCacheSrvr.SendSetPlayerName( idPlayer, lpszPlayer );
	}
	g_dpCoreSrvr.SendSetPlayerName( idPlayer, lpszPlayer, dwData, f );
}

void CDPDatabaseClient::SendSnoopGuild( u_long idGuild, u_long idPlayer, const char* lpszChat )
{
	BEFORESEND( ar, PACKETTYPE_SETSNOOPGUILD );
	ar  << idGuild << idPlayer;
	ar.WriteString( lpszChat );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::OnUpdatePlayerData( CAr & ar )
{
	u_long idPlayer;
	PlayerData pd;
	ar >> idPlayer;
	ar.Read( &pd.data, sizeof(sPlayerData) );
	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	CPlayer* pPlayer	= g_PlayerMng.GetPlayer( idPlayer );
	map<u_long, int>	mapIdPlayer;
	if( pPlayer )
	{
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
				g_DPCacheSrvr.SendUpdatePlayerData( idPlayer, &pd, pTo );
		}
	}
}

void CDPDatabaseClient::QueryAddMessenger( u_long idPlayer, u_long idFriend )
{
	BEFORESEND( ar, PACKETTYPE_ADD_MESSENGER );
	ar << idPlayer << idFriend;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::QueryDeleteMessenger( u_long idPlayer, u_long idFriend )
{
	BEFORESEND( ar, PACKETTYPE_DELETE_MESSENGER );
	ar << idPlayer << idFriend;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::QueryUpdateMessenger( u_long idPlayer, u_long idFriend, BOOL bBlock )
{
	BEFORESEND( ar, PACKETTYPE_UPDATE_MESSENGER );
	ar << idPlayer << idFriend << bBlock;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPDatabaseClient::OnRemovePlayerFriend( CAr & ar )
{
	u_long uPlayerId, uFriendId;
	ar >> uPlayerId >> uFriendId;

	CPlayer* pFriendUser;
	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	
	pFriendUser = g_PlayerMng.GetPlayer( uFriendId );
	if( pFriendUser )
	{
		pFriendUser->Lock();
		pFriendUser->m_RTMessenger.RemoveFriend( uPlayerId );
		pFriendUser->Unlock();
		g_dpCoreSrvr.SendRemoveFriend( pFriendUser->uKey, uPlayerId );
	}
}

void CDPDatabaseClient::OnEventNotice( CAr & ar )
{
	int nSize = 0;
	char szTemp[255] = {0,};
	vector<string> vecstrMessage;
	ar >> nSize;
	for( int i=0; i<nSize; i++ )
	{
		ar.ReadString( szTemp, _countof( szTemp ) );
		g_DPCacheSrvr.SendSystem( szTemp );
	}
}

void	CDPDatabaseClient::OnCloseExistingConnection( CAr & ar )
{
	char lpszAccount[MAX_ACCOUNT]	= { 0, };
	LONG lError = 0;

	ar.ReadString( lpszAccount, _countof( lpszAccount ) );
	ar >> lError;

	g_DPCacheSrvr.SendCloseExistingConnection( lpszAccount, lError );
}

#ifdef __M_USER_BOTS_DB_BASE
void	CDPDatabaseClient::sendServerConnected(const u_long uKey) {
	BEFORESEND(ar, PACKETTYPE_ADD_WSERV);
	ar << uKey;
	SEND(ar, this, DPID_ALLPLAYERS);
}
void	CDPDatabaseClient::sendServerDisconnected(const u_long uKey) {
	BEFORESEND(ar, PACKETTYPE_REMOVE_WSERV);
	ar << uKey;
	SEND(ar, this, DPID_ALLPLAYERS);
}

void CDPDatabaseClient::onAddBot(CAr& ar) {
	u_long uKey = 0;
	u_long idBot = 0;
	std::size_t nBotType = 0;
	std::string strName;
	ar >> uKey >> idBot;
	ar.ReadString(strName);
	ar >> nBotType;
	g_dpCoreSrvr.sendAddBot(idBot, strName, uKey, nBotType);
}
void CDPDatabaseClient::onRemoveBot(CAr& ar) {
	u_long uKey = 0;
	u_long idBot = 0;
	ar >> uKey >> idBot;
	g_dpCoreSrvr.sendRemoveBot(idBot, uKey);
}
#endif


CDPDatabaseClient	g_dpDatabaseClient;