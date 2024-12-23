#include "StdAfx.h"
#include "dpcoresrvr.h"
#include "dpdatabaseclient.h"
#include "dploginsrvr.h"
extern	CDPLoginSrvr	g_dpLoginSrvr;
#include "dpcachesrvr.h"
#include <msghdr.h>
#include <logging/FLMyTrace.h>
#include "player.h"
#include <misc.h>
#include <party.h>
#include <definetext.h>

extern	CPlayerMng	g_PlayerMng;
extern	CDPCoreSrvr	g_dpCoreSrvr;
extern	CDPDatabaseClient	g_dpDatabaseClient;
extern	FLMyTrace	g_MyTrace;
extern	CPartyMng	g_PartyMng;

#include <guild.h>
#include <guildwar.h>
extern	CGuildMng	g_GuildMng;
extern	CGuildWarMng	g_GuildWarMng;

#include <lang.h>

// %Y%m%d%H%M%S

CDPCacheSrvr::CDPCacheSrvr()
:m_dwClientSocketId( NULL_ID )
{
	ON_MSG( PACKETTYPE_JOIN, OnAddPlayer );
	ON_MSG( PACKETTYPE_DESTROY_PLAYER, OnQueryRemovePlayer );
//	ON_MSG( PACKETTYPE_ADDPARTYMEMBER, OnAddPartyMember );
	ON_MSG( PACKETTYPE_REMOVEPARTYMEMBER, OnRemovePartyMember );
	ON_MSG( PACKETTYPE_CHANGETROUP, OnPartyChangeTroup );
	ON_MSG( PACKETTYPE_CHANPARTYNAME, OnPartyChangeName );
	ON_MSG( PACKETTYPE_PARTYCHANGEITEMMODE, OnPartyChangeItemMode );
	ON_MSG( PACKETTYPE_PARTYCHANGEEXPMODE, OnPartyChangeExpMode );
	ON_MSG( PACKETTYPE_ADDFRIEND, OnAddFriend );
	ON_MSG( PACKETTYPE_GETFRIENDSTATE, OnGetFriendState );
	ON_MSG( PACKETTYPE_SETFRIENDSTATE, OnSetFrinedState );
	ON_MSG( PACKETTYPE_FRIENDINTERCEPTSTATE, OnFriendInterceptState );
	ON_MSG( PACKETTYPE_REMOVEFRIEND, OnRemoveFriend );
	ON_MSG( PACKETTYPE_GUILD_SETNAME, OnGuildSetName );
	ON_MSG( PACKETTYPE_GUILD_PENYA, OnGuildPenya );
	ON_MSG( PACKETTYPE_GUILD_AUTHORITY, OnGuildAuthority );
	ON_MSG( PACKETTYPE_DESTROY_GUILD, OnDestroyGuild );
	ON_MSG( PACKETTYPE_ADD_GUILD_MEMBER, OnAddGuildMember );
	ON_MSG( PACKETTYPE_REMOVE_GUILD_MEMBER, OnRemoveGuildMember );
	ON_MSG( PACKETTYPE_GUILD_MEMBER_LEVEL, OnGuildMemberLv );
	ON_MSG( PACKETTYPE_GUILD_CLASS, OnGuildClass );
	ON_MSG( PACKETTYPE_GUILD_NICKNAME, OnGuildNickName );
	ON_MSG( PACKETTYPE_DECL_GUILD_WAR, OnDeclWar );
	ON_MSG( PACKETTYPE_ACPT_GUILD_WAR, OnAcptWar );
	ON_MSG( PACKETTYPE_SURRENDER, OnSurrender );
	ON_MSG( PACKETTYPE_QUERY_TRUCE, OnQueryTruce );
	ON_MSG( PACKETTYPE_ACPT_TRUCE, OnAcptTruce );
	ON_MSG( PACKETTYPE_NC_ADDVOTE, OnAddVote );
	ON_MSG( PACKETTYPE_NC_REMOVEVOTE, OnRemoveVote );
	ON_MSG( PACKETTYPE_NC_CLOSEVOTE, OnCloseVote );
	ON_MSG( PACKETTYPE_NC_CASTVOTE, OnCastVote );
	ON_MSG( PACKETTYPE_PARTYCHANGELEADER, OnPartyChangeLeader );
	ON_MSG( PACKETTYPE_SENDTAG, OnSendTag );
	ON_MSG( PACKETTYPE_CHG_MASTER, OnChgMaster );
//	ON_MSG( PACKETTYPE_BUYING_INFO, OnBuyingInfo );
}

CDPCacheSrvr::~CDPCacheSrvr()
{

}

void CDPCacheSrvr::SysMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD /*dwMsgSize*/, DPID /*idFrom*/ )
{
	switch( lpMsg->dwType )
	{
		case DPSYS_CREATEPLAYERORGROUP:
			{
				LPDPMSG_CREATEPLAYERORGROUP lpCreatePlayer	= (LPDPMSG_CREATEPLAYERORGROUP)lpMsg;
				OnAddConnection( lpCreatePlayer->dpId );
				break;
			}
		case DPSYS_DESTROYPLAYERORGROUP:
			{
				LPDPMSG_DESTROYPLAYERORGROUP lpDestroyPlayer	= (LPDPMSG_DESTROYPLAYERORGROUP)lpMsg;
				OnRemoveConnection( lpDestroyPlayer->dpId );
				break;
			}
	}
}

void CDPCacheSrvr::UserMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom )
{
	CAr ar( (LPBYTE)lpMsg + sizeof(DPID), dwMsgSize - sizeof(DPID) );
	GETTYPE( ar );
	PACKET_HANDLER_FUNC pfn	=	GetHandler( dw );
	
	if( pfn ) {
		( this->*( pfn ) )( ar, idFrom, *(UNALIGNED LPDPID)lpMsg, dwMsgSize - sizeof(DPID) - sizeof(DWORD) );
	}
	else {
		FLTRACE_LOG( PROGRAM_NAME, _T( "Handler not found(%08x)" ), lpMsg->dwType );
	}
}

bool	CDPCacheSrvr::IsEmptyConnectedClient()
{
	if( m_apServer.empty() == false )
	{
		return false;
	}

	return true;
}

void CDPCacheSrvr::SendProcServerList( DPID dpid )
{
	BEFORESENDSOLE( ar, PACKETTYPE_PROCSERVER_LIST, DPID_UNKNOWN );

	ar << (short)g_dpCoreSrvr.m_apServer.size();
	for( CServerDescArray::iterator i = g_dpCoreSrvr.m_apServer.begin(); i != g_dpCoreSrvr.m_apServer.end(); ++i )
		i->second->Serialize( ar );

	SEND( ar, this, dpid );
}

void CDPCacheSrvr::OnAddConnection( DPID dpid )
{
	if( m_dwClientSocketId == NULL_ID )
	{
		m_dwClientSocketId = dpid;

		SendProcServerList( dpid );
		
		CServerDesc* pServer	= new CServerDesc;
		GetPlayerAddr( dpid, pServer->m_szAddr, _countof( pServer->m_szAddr ) );
		bool bResult	= m_apServer.insert( CServerDescArray::value_type( dpid, pServer ) ).second;
		FLASSERT( bResult );
		g_MyTrace.Add( FLMyTrace::Key( pServer->m_szAddr ), FALSE, "%s", pServer->m_szAddr );
		g_PlayerMng.AddCache( dpid );
	}
	else
	{
		TCHAR szInvalidCacheServer[16] = { 0, };
		GetPlayerAddr( dpid, szInvalidCacheServer, _countof( szInvalidCacheServer ) );
		FLERROR_LOG( PROGRAM_NAME, _T( "Other Cache Connection - IP : %s" ), szInvalidCacheServer );
	}
}

void CDPCacheSrvr::OnRemoveConnection( DPID dpid )
{
	if( m_dwClientSocketId == dpid )
	{
		m_dwClientSocketId	= NULL_ID;
		CServerDesc* pServer	= m_apServer.GetAt( dpid );
		m_apServer.erase( dpid );
		if( pServer )
			g_MyTrace.Add( FLMyTrace::Key( pServer->m_szAddr ), TRUE, "%s", pServer->m_szAddr );
		SAFE_DELETE( pServer );
		g_PlayerMng.RemoveCache( dpid );
	}
}

void CDPCacheSrvr::OnAddPlayer( CAr & ar, DPID dpidCache, DPID dpidUser, u_long /*uBufSize*/ )
{
	char lpszPlayer[MAX_PLAYER];
	DWORD dwAuthKey;
	u_long idPlayer, dpidSocket;

	ar >> dwAuthKey;
	ar >> idPlayer;
	ar >> dpidSocket;

	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	CPlayer* pPlayer	= g_PlayerMng.GetPlayer( idPlayer );	// m_ulong2
	if( pPlayer )
	{
		// �ش� ���Ϲ�ȣ�� ���� �÷��̾ ã�Ƽ� ���ش�.
		// �� �÷��̾�� CACHE������ LOGIN������ ���� ������ ���� �ʴ� ����
		CPlayer* p;
		p = g_PlayerMng.GetPlayerBySocket( dpidSocket );	// m_players
		if( p )
			g_PlayerMng.UnregisterPlayerInfo( p, FALSE );
		
		pPlayer->m_dwSerial = dpidUser;
		pPlayer->dpidCache	= dpidCache;
		pPlayer->dpidUser	= dpidSocket;
		ar >> pPlayer->m_uPartyId;

		g_GuildMng.m_AddRemoveLock.Enter();
		ar >> pPlayer->m_idGuild;
		if( NULL == g_GuildMng.GetGuild( pPlayer->m_idGuild ) )
			pPlayer->m_idGuild	= 0;

		ar >> pPlayer->m_idWar;
		if( NULL == g_GuildWarMng.GetWar( pPlayer->m_idWar ) )
			pPlayer->m_idWar	= 0;

		g_GuildMng.m_AddRemoveLock.Leave();

		ar >> pPlayer->m_uIdofMulti;
		pPlayer->m_RTMessenger.Serialize( ar );
		ar.ReadString( lpszPlayer, _countof( lpszPlayer ) );
		ar.ReadString( pPlayer->lpAddr, _countof( pPlayer->lpAddr ) );

		g_PlayerMng.RegisterPlayerInfo( pPlayer );

		BEFORESENDSOLE( out, PACKETTYPE_JOIN, pPlayer->dpidUser );	// result
		out << pPlayer->m_dwSerial ;	// serial key�� �����ϰ� �Ѵ�.
		out << (BYTE)1;					// �׻� ���� 
		SEND( out, this, dpidCache );
	}
	else
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "pPlayer is NULL, %d" ), idPlayer );
	}
}

void CDPCacheSrvr::OnQueryRemovePlayer( CAr & /*ar*/, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	CPlayer* pPlayer	= g_PlayerMng.GetPlayerBySerial( dpidUser );
	if( pPlayer )
	{
		if( *pPlayer->lpszAccount != '\0' )
			g_dpLoginSrvr.SendQueryRemovePlayer( pPlayer->lpszAccount );
		else
			FLERROR_LOG( PROGRAM_NAME, _T( "lpszAccount == '\0'" ) );

		g_PlayerMng.RemovePlayer( pPlayer );
	}
}

void CDPCacheSrvr::SendWhisper( const CHAR* sPlayerFrom, const CHAR* sPlayerTo, u_long idFrom, u_long idTo, const CHAR* lpString, CPlayer* pTo, int nSearch )
{
	if( !pTo )
		return;
//	FLASSERT( pTo );

	BEFORESENDSOLE( ar, PACKETTYPE_WHISPER, pTo->dpidUser );
	ar.WriteString( sPlayerFrom );
	ar.WriteString( sPlayerTo );
	ar.WriteString( lpString );
	ar << idFrom << idTo;
	ar << nSearch;
	SEND( ar, this, pTo->dpidCache );
}

void CDPCacheSrvr::SendSay( const CHAR* sPlayerFrom, const CHAR* sPlayerTo, u_long idFrom, u_long idTo, const CHAR* lpString, CPlayer* pTo, int nSearch )
{
	if( !pTo )
		return;
//	FLASSERT( pTo );

	BEFORESENDSOLE( ar, PACKETTYPE_SAY, pTo->dpidUser );
	ar.WriteString( sPlayerFrom );
	ar.WriteString( sPlayerTo );
	ar.WriteString( lpString );
	ar << idFrom << idTo;
	ar << nSearch;
	SEND( ar, this, pTo->dpidCache );
}

void CDPCacheSrvr::SendGMSay( const CHAR* sPlayerFrom, const CHAR* sPlayerTo, const CHAR* lpString, CPlayer* pTo )
{
	if( !pTo )
		return;
//	FLASSERT( pTo );
	
	BEFORESENDSOLE( ar, PACKETTYPE_GMSAY, pTo->dpidUser );
	ar.WriteString( sPlayerFrom );
	ar.WriteString( sPlayerTo );
	ar.WriteString( lpString );
	SEND( ar, this, pTo->dpidCache );
}

void CDPCacheSrvr::SendFriendState( CPlayer* pTo )
{
	vector< u_long > vecIdFriend;
	vector< u_long > vecIdBlock;

	DWORD dwState;
	u_long uIdofMulti;
	BEFORESENDSOLE( ar, PACKETTYPE_GETFRIENDSTATE, pTo->dpidUser );
	
	pTo->Lock();
	for( map<u_long, Friend>::iterator i = pTo->m_RTMessenger.begin(); i != pTo->m_RTMessenger.end(); ++i )
	{
		Friend* pFriend		= &i->second;
		if( !pFriend->bBlock )
			vecIdFriend.push_back( i->first );
		else
			vecIdBlock.push_back( i->first );
	}
	pTo->Unlock();

	ar << static_cast<int>( vecIdFriend.size() );
	ar << static_cast<int>( vecIdBlock.size() );

	for( DWORD j = 0 ; j < vecIdFriend.size() ; j++ )
	{
		CPlayer* pPlayer	= g_PlayerMng.GetPlayer( vecIdFriend[j] );
		if( pPlayer )
		{
			pPlayer->Lock();
			Friend* pFriend		= pPlayer->m_RTMessenger.GetFriend( pTo->uKey );
			if( pFriend )
			{
				if( pFriend->bBlock )
					dwState		= FRS_OFFLINE;
				else
					dwState		= pPlayer->m_RTMessenger.GetState();
			}
			pPlayer->Unlock();
		}
		else
			dwState = FRS_OFFLINE;

		ar << vecIdFriend[j] << dwState;
		uIdofMulti	= ( pPlayer? pPlayer->m_uIdofMulti : 100 );
		ar << uIdofMulti;
	}

	for( DWORD k = 0 ; k < vecIdBlock.size() ; k++ )
	{
		CPlayer* pPlayer	= g_PlayerMng.GetPlayer( vecIdBlock[k] );
		if( pPlayer )
		{
			pPlayer->Lock();
			Friend* pFriend		= pPlayer->m_RTMessenger.GetFriend( pTo->uKey );
			if( pFriend )
			{
				if( pFriend->bBlock )
					dwState		= FRS_OFFLINE;
				else
					dwState		= pPlayer->m_RTMessenger.GetState();
			}
			else
			{
				dwState		= FRS_OFFLINE;
			}
			pPlayer->Unlock();
		}
		else
		{
			dwState		= FRS_OFFLINE;
		}
		ar << vecIdBlock[k] << dwState;
		uIdofMulti	= ( pPlayer? pPlayer->m_uIdofMulti : 100 );
		ar << uIdofMulti;
	}
	
	SEND( ar, this, pTo->dpidCache );
}

void CDPCacheSrvr::SendSetFriendState( CPlayer* pTo )
{
	vector< u_long > vecIdFriend;

	pTo->Lock();
	u_long idPlayer = pTo->uKey;
	DWORD  dwState	= pTo->m_RTMessenger.GetState();
	for( map<u_long, Friend>::iterator i = pTo->m_RTMessenger.begin(); i != pTo->m_RTMessenger.end(); ++i )
	{
		Friend* pFriend		= &i->second;
		if( !pFriend->bBlock )
			vecIdFriend.push_back( i->first );
	}

	{
		BEFORESENDSOLE( ar, PACKETTYPE_SETFRIENDSTATE, pTo->dpidUser );
		ar << idPlayer;
		ar << dwState;
		SEND( ar, this, pTo->dpidCache );
	}
	
	pTo->Unlock();

	for( DWORD j = 0 ; j < vecIdFriend.size() ; j++ )
	{
		CPlayer* pPlayer = g_PlayerMng.GetPlayer( vecIdFriend[j] );
		if( pPlayer )
		{
			pPlayer->Lock();
			Friend* pFriend		= pPlayer->m_RTMessenger.GetFriend( idPlayer );
			if( pFriend )
			{
				pFriend->dwState	= dwState;
				BEFORESENDSOLE( arSend, PACKETTYPE_SETFRIENDSTATE, pPlayer->dpidUser );
				arSend << idPlayer; 
				arSend << dwState;
				SEND( arSend, this, pPlayer->dpidCache );
			}
			pPlayer->Unlock();
		}
	}
}

void CDPCacheSrvr::SendFriendJoin( CPlayer* pTo, CPlayer* pFriend )
{
	BEFORESENDSOLE( ar, PACKETTYPE_ADDFRIENDJOIN, pTo->dpidUser );

	ar << pFriend->uKey;
	ar << pFriend->m_RTMessenger.GetState();
	ar << pFriend->m_uIdofMulti;

	SEND( ar, this, pTo->dpidCache );
}

void CDPCacheSrvr::SendFriendLogOut( CPlayer* pTo, u_long uidPlayer )
{
	BEFORESENDSOLE( ar, PACKETTYPE_ADDFRIENDLOGOUT, pTo->dpidUser );
	ar << uidPlayer;
	SEND( ar, this, pTo->dpidCache );
}

void CDPCacheSrvr::SendFriendNoIntercept( CPlayer* pTo, CPlayer* pFriend, int state )
{
	BEFORESENDSOLE( ar, PACKETTYPE_FRIENDNOINTERCEPT, pTo->dpidUser );
	ar << pFriend->uKey;
	ar << state;
	SEND( ar, this, pTo->dpidCache );

	SendFriendJoin( pFriend, pTo );
}

void CDPCacheSrvr::SendFriendNoIntercept( CPlayer* pTo, u_long uFriendid, int state )
{
	BEFORESENDSOLE( ar, PACKETTYPE_FRIENDNOINTERCEPT, pTo->dpidUser );
	ar << uFriendid;
	ar << state;
	SEND( ar, this, pTo->dpidCache );
}

void CDPCacheSrvr::SendFriendIntercept( CPlayer* pPlayer, CPlayer* pFriend )
{
	{
		BEFORESENDSOLE( ar, PACKETTYPE_FRIENDINTERCEPTSTATE, pPlayer->dpidUser );
		ar << pPlayer->uKey << pFriend->uKey;
		SEND( ar, this, pPlayer->dpidCache );
	}
	{
		BEFORESENDSOLE( ar, PACKETTYPE_FRIENDINTERCEPTSTATE, pFriend->dpidUser );
		ar << pPlayer->uKey << pFriend->uKey;
		SEND( ar, this, pFriend->dpidCache );
	}
}

void CDPCacheSrvr::SendFriendIntercept( CPlayer* pPlayer, u_long uFriendid )
{
	BEFORESENDSOLE( ar, PACKETTYPE_FRIENDINTERCEPTSTATE, pPlayer->dpidUser );
	ar << pPlayer->uKey << uFriendid;
	SEND( ar, this, pPlayer->dpidCache );
}

void CDPCacheSrvr::SendOneFriendState( CPlayer* pTo, u_long uidPlayer, DWORD dwState )
{
	BEFORESENDSOLE( ar, PACKETTYPE_ONEFRIEMDSTATE, pTo->dpidUser );
	ar << uidPlayer << dwState;
	SEND( ar, this, pTo->dpidCache );		
}

void CDPCacheSrvr::SendModifyMode( DWORD dwMode, BYTE f, u_long idFrom, CPlayer* pTo )
{
	if( !pTo )
		return;
//	FLASSERT( pTo );

	BEFORESENDSOLE( ar, PACKETTYPE_MODIFYMODE, pTo->dpidUser );
	ar << dwMode << f << idFrom;
	ar << pTo->dpidUser;
	SEND( ar, this, pTo->dpidCache );
}

// #ifdef __LAYER_1015
// void CDPCacheSrvr::SendSummonPlayer( u_long idOperator, u_long uIdofMulti, DWORD dwWorldID, const D3DXVECTOR3 & vPos, CPlayer* pPlayer, int nLayer )
// #else	// __LAYER_1015
// void CDPCacheSrvr::SendSummonPlayer( u_long idOperator, u_long uIdofMulti, DWORD dwWorldID, const D3DXVECTOR3 & vPos, CPlayer* pPlayer )
// #endif	// __LAYER_1015
// {
// 	FLASSERT( pPlayer );
// 
// 	if( pPlayer == NULL )
// 	{
// 		return;
// 	}
// 
// 	BEFORESENDSOLE( ar, PACKETTYPE_SUMMONPLAYER, pPlayer->dpidUser );
// 	ar << idOperator;
// 	ar << dwWorldID;
// 	ar << vPos;
// 	ar << uIdofMulti;
// 	ar << pPlayer->dpidUser;
// #ifdef __LAYER_1015
// 	ar << nLayer;
// #endif	// __LAYER_1015
// 	SEND( ar, this, pPlayer->dpidCache );
// }

// void CDPCacheSrvr::SendTeleportPlayer( u_long idOperator, CPlayer* pPlayer )
// {
// 	BEFORESENDSOLE( ar, PACKETTYPE_TELEPORTPLAYER, pPlayer->dpidUser );
// 	ar << idOperator;
// 	ar << pPlayer->dpidUser;
// 	SEND( ar, this, pPlayer->dpidCache );
// }

void CDPCacheSrvr::SendKillPlayer( CPlayer* pPlayer )
{
	SendHdr( PACKETTYPE_KILLPLAYER, pPlayer->dpidCache, pPlayer->dpidUser );
}

void CDPCacheSrvr::SendGetPlayerAddr( const CHAR* lpszPlayer, const CHAR* lpAddr, CPlayer* pOperator )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GETPLAYERADDR, pOperator->dpidUser );
	ar.WriteString( lpszPlayer );
	ar.WriteString( lpAddr );
	SEND( ar, this, pOperator->dpidCache );
}

void CDPCacheSrvr::SendGetPlayerCount( u_short uCount, CPlayer* pOperator )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GETPLAYERCOUNT, pOperator->dpidUser );
	ar << uCount;
	SEND( ar, this, pOperator->dpidCache );
}

void CDPCacheSrvr::SendGetCorePlayer( CPlayer* pOperator )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GETCOREPLAYER, pOperator->dpidUser );
	g_PlayerMng.PackName( ar );
	SEND( ar, this, pOperator->dpidCache );
}

void CDPCacheSrvr::SendSystem( const CHAR* lpString )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SYSTEM, DPID_ALLPLAYERS );
	ar.WriteString( lpString );
	SEND( ar, this, DPID_ALLPLAYERS );
}

void CDPCacheSrvr::SendCaption( const CHAR* lpString, DWORD dwWorldId, BOOL bSmall, DWORD dwPartyId )
{
	BEFORESENDSOLE( ar, PACKETTYPE_CAPTION, DPID_ALLPLAYERS );
	ar << bSmall;
	ar << dwWorldId;
	ar << dwPartyId;
	ar.WriteString( lpString );
	SEND( ar, this, DPID_ALLPLAYERS );
}


void CDPCacheSrvr::SendGameRate( FLOAT fRate, BYTE nFlag )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GAMERATE, DPID_ALLPLAYERS );
	ar << fRate;
	ar << nFlag;
	SEND( ar, this, DPID_ALLPLAYERS );
}

void CDPCacheSrvr::SendDefinedText( int dwText, DPID dpidCache, DPID dpidUser, LPCSTR lpszFormat, ... )
{
	va_list args;
	va_start( args, lpszFormat );
	TCHAR szBuffer[128];
	FLVSPrintf( szBuffer, _countof( szBuffer ), lpszFormat, args );
	va_end( args );

	BEFORESENDSOLE( ar, PACKETTYPE_DEFINEDTEXT, dpidUser );
	ar << dwText;
	ar.WriteString( szBuffer );
	SEND( ar, this, dpidCache );
}

void CDPCacheSrvr::OnSendTag( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long idTo;
	char szString[256];

	ar >> idTo;						// ������ ���� ĳ���� 
	ar.ReadString( szString, _countof( szString ) );		// �������� 
	szString[255] = '\0';

	FLTRACE_LOG( PROGRAM_NAME, _T( "%d %s" ), idTo, szString );
	
	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );

	CPlayer* pFrom	=  g_PlayerMng.GetPlayerBySerial( dpidUser );
	if( pFrom == NULL)
		return;		// �������� ������ ����

	pFrom->Lock();

	TAG_RESULT result = pFrom->IsTagSendable( idTo );  // db�� ������ �ִ´�.
	switch( result )
	{
	case TAG_BLOCKED:
		SendTagResult( pFrom, 0 );		// ������ blocked��Ų �����̸� full(0�� �ǹ�)�Ǿ ������ ���ߴٰ� �˷��ش�.
		break;

	case TAG_OK:
		if( g_PlayerMng.GetPlayer( idTo ) )
		{
			pFrom->Unlock();
			return;					// �޴��� online�̸� ���� 
		}

		g_dpDatabaseClient.SendTag( pFrom->uKey, idTo, szString );
		break;
	}

	pFrom->Unlock();
}

// cbResult -  ���: 0 - ����(20�� �ʰ��� ���), 1 - ���� 
void CDPCacheSrvr::SendTagResult( CPlayer* pPlayer, BYTE cbResult )
{
	BEFORESENDSOLE( ar, PACKETTYPE_INSERTTAG_RESULT, pPlayer->dpidUser );
	ar << cbResult;		
	SEND( ar, this, pPlayer->dpidCache );
}

void CDPCacheSrvr::OnPartyChangeLeader( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long _idLeader, idChangeLeader;
	ar >> _idLeader >> idChangeLeader;
	
	//CPlayer* pChangeLeader;
	
	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	
	CPlayer* pLeader = g_PlayerMng.GetPlayerBySerial( dpidUser );
	//pChangeLeader = g_PlayerMng.GetPlayer( idChangeLeader );
	
	if( !pLeader )
	{
		//		player not found
		return;
	}
	
	CParty* pParty;
	CMclAutoLock	Lock2( g_PartyMng.m_AddRemoveLock );
	
	pParty	= g_PartyMng.GetParty( pLeader->m_uPartyId );
	if( !pParty || !pParty->IsMember( pLeader->uKey ) )
	{
		// leader's party not found
		// OR is not party member
		pLeader->m_uPartyId		= 0;
		return;
	}
	if( pLeader->uKey != idChangeLeader )
	{
		if( FALSE == pParty->IsLeader( pLeader->uKey ) )
		{
			// is not leader
			return;
		}
		if( !pParty->IsMember( idChangeLeader ) )
		{
			// pMember is not pLeader's party member
			SendErrorParty( ERROR_NOTMAMBER, pLeader );
			return;
		}
	}
	else
	{
		SendErrorParty( ERROR_SAMLEADER, pLeader );
		return;
	}

	int nFind = pParty->FindMember( idChangeLeader );
	if( nFind == -1 || pParty->m_aMember[nFind].m_bRemove )
		return;

	//12�� �Ľ�ų ������ ����  world,core,neuz
	for( int j = 0 ; j < MAX_PARTYMODE ; j++ )
	{
		if( pParty->m_dwModeTime[j] )
		{
// 			if( j == PARTY_PARSKILL_MODE)
// 				continue;
			pParty->m_dwModeTime[j] = 0;
			g_dpCoreSrvr.SendSetPartyMode( pParty->m_uPartyId, j, FALSE );
		}
	}

	pParty->ChangeLeader( idChangeLeader );
	g_dpCoreSrvr.SendChangeLeader( pParty->m_uPartyId, idChangeLeader );
}

// void CDPCacheSrvr::OnAddPartyMember( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
// {
// 	u_long idLeader, _idMember;
// // 	LONG nLeaderLevel, nMemberLevel, nLeaderJob, nMemberJob;
// // 	DWORD dwLSex, dwMSex;
// 	ar >> idLeader;		// >> nLeaderLevel >> nLeaderJob >> dwLSex;
// 	ar >> _idMember;	// >> nMemberLevel >> nMemberJob >> dwMSex;
// 	CPlayer* pLeader;
// 	CPlayer* pMember;
// 
// 	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
// 	
// 	pLeader	= g_PlayerMng.GetPlayer( idLeader );
// 	pMember = g_PlayerMng.GetPlayerBySerial( dpidUser );
// 
// 	if( !pLeader || !pMember )
// 		return;
// 
// 	CMclAutoLock	Lock1( g_PartyMng.m_AddRemoveLock );
// 
// 	CParty* pParty	= g_PartyMng.GetParty( pMember->m_uPartyId );	// member's party
// 	if( pParty && pParty->IsMember( pMember->uKey ) )
// 	{
// 		SendErrorParty( ERROR_DIFFRENTPARTY, pLeader );
// 		return;
// 	}
// 
// 	pMember->m_uPartyId	= 0;
// 	pParty	= g_PartyMng.GetParty( pLeader->m_uPartyId );	// leader's party
// 	if( pParty && pParty->IsMember( pLeader->uKey ) )	// add
// 	{
// 		if( FALSE == pParty->IsLeader( pLeader->uKey ) )	// is not leader
// 		{
// 			SendErrorParty( ERROR_NOLEADER, pLeader );
// 			return;
// 		}
// 		if( pParty->m_nSizeofMember >= MAX_PTMEMBER_SIZE )
// 		{
// 			SendErrorParty( ERROR_FULLPARTY, pLeader );
// 			return;
// 		}
// 		if( pParty->m_idDuelParty > 0 )
// 		{
// 			return;
// 		}
// 
// 		if( FALSE == pParty->NewMember( pMember->uKey ) )
// 		{
// 			// error
// 		}
// 		else	// ok
// 		{
// 			pMember->m_uPartyId		= pParty->m_uPartyId;
// 			g_dpCoreSrvr.SendAddPartyMember( pParty->m_uPartyId, pLeader->uKey, pMember->uKey );
// // 			// __PARSKILL1001 090917 mirchang - �Ľ�ų ������ ����
// // 			g_dpCoreSrvr.SendSetPartyMode( pParty->m_uPartyId, PARTY_PARSKILL_MODE, FALSE );
// 		}
// 	}
// 	else	// new
// 	{
// 		u_long uPartyId		=
// 			g_PartyMng.NewParty( pLeader->uKey, pMember->uKey );
// 		if( uPartyId != 0 )
// 		{
// 			pLeader->m_uPartyId		= pMember->m_uPartyId	= uPartyId;
// 			g_dpCoreSrvr.SendAddPartyMember( uPartyId, pLeader->uKey, pMember->uKey );
// 		}
// 		else
// 		{
// //			error
// 		}
// 	}
// }

void CDPCacheSrvr::OnRemovePartyMember( CAr & ar, DPID /*dpidCache*/, DPID /*dpidUser*/, u_long /*uBufSize*/ )
{
	u_long _idLeader, idMember;
	ar >> _idLeader >> idMember;

	CPlayer* pLeader;
	CPlayer* pMember;

	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );

	//pLeader	= g_PlayerMng.GetPlayerBySerial( dpidUser );
	pLeader	= g_PlayerMng.GetPlayer( _idLeader );
	pMember = g_PlayerMng.GetPlayer( idMember );

	if( !pLeader )
		return;
	
	CMclAutoLock	Lock2( g_PartyMng.m_AddRemoveLock );
	
	CParty* pParty	= g_PartyMng.GetParty( pLeader->m_uPartyId );
	if( !pParty || !pParty->IsMember( pLeader->uKey ) )
	{
		// leader's party not found
		// OR is not party member
		FLERROR_LOG( PROGRAM_NAME, _T( "Party Not Found OR IsMember Not Found" ) );
		pLeader->m_uPartyId		= 0;
		return;
	}

	if( pParty->m_idDuelParty > 0 )
		return;


	if( pLeader->uKey != idMember )
	{
		if( FALSE == pParty->IsLeader( pLeader->uKey ) )
		{
			// is not leader
			FLERROR_LOG( PROGRAM_NAME, _T( "Not IsLeader" ) );
			return;
		}
		if( !pParty->IsMember( idMember ) )
		{
			// pMember is not pLeader's party member
			FLERROR_LOG( PROGRAM_NAME, _T( "Not IsMember" ) );
			SendErrorParty( ERROR_DIFFRENTPARTY, pLeader );
			return;
		}
	}
	BOOL bLeader = pParty->IsLeader( idMember );
	if( pParty->DeleteMember( idMember ) )
	{
		if( pMember )
			pMember->m_uPartyId		= 0;
		g_dpCoreSrvr.SendRemoveParty( pParty->m_uPartyId, pLeader->uKey, idMember );
		if( pParty->GetSizeofMember() < 2 )
		{
			CPlayer* pLeadertmp	= g_PlayerMng.GetPlayer( pParty->GetPlayerId( 0 ) );
			if( pLeadertmp )
				pLeadertmp->m_uPartyId	= 0;
			g_PartyMng.DeleteParty( pParty->m_uPartyId );
		}
		else
		{
			if( bLeader )
			{
				for( int j = 0 ; j < MAX_PARTYMODE ; j++ )
				{
					if( pParty->m_dwModeTime[j] )
					{
// 						if( j == PARTY_PARSKILL_MODE)
// 							continue;
						pParty->m_dwModeTime[j] = 0;
						g_dpCoreSrvr.SendSetPartyMode( pParty->m_uPartyId, j, FALSE );
					}
				}
				u_long idChangeLeader =  pParty->GetPlayerId(0);
				g_dpCoreSrvr.SendChangeLeader( pParty->m_uPartyId, idChangeLeader );
			}
// 			// 090917 mirchang - �Ľ�ų ������ ����
// 			if( pParty->m_nModeTime[PARTY_PARSKILL_MODE] )
// 				g_dpCoreSrvr.SendSetPartyMode( pParty->m_uPartyId, PARTY_PARSKILL_MODE, FALSE );
		}
	}
	else
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "Not DeleteMember" ) );
	}
}

void CDPCacheSrvr::OnPartyChangeName( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long uBufSize )
{
	if( uBufSize > 40 )	return;		// 4 + 4 + 32

	u_long _uidPlayer;
	TCHAR	sParty[33];	// 32
	ar >> _uidPlayer;	// 4
	ar.ReadString( sParty, _countof( sParty ) );
	CPlayer* pPlayer;
		
	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	
	pPlayer	= g_PlayerMng.GetPlayerBySerial( dpidUser );
	
	if( !pPlayer )
		return;
	
	CParty* pParty;
	CMclAutoLock	Lock2( g_PartyMng.m_AddRemoveLock );
	
	pParty	= g_PartyMng.GetParty( pPlayer->m_uPartyId );
	if( !pParty || FALSE == pParty->IsMember( pPlayer->uKey ) )
	{
		// party not found
		SendErrorParty( ERROR_NOPARTY, pPlayer );
		return;
	}
	
	if( pParty->IsLeader( pPlayer->uKey ) )
	{	
		if( pParty->GetKind() == PARTY_KIND_EXTEND )
		{
			if( g_PartyMng.IsPartyName( sParty ) == FALSE )
			{
				FLStrcpy( pParty->m_sParty, _countof( pParty->m_sParty ), sParty );
				g_PartyMng.AddPartyName( pPlayer->uKey, sParty );
				g_dpDatabaseClient.SendAddPartyName( pPlayer->uKey, sParty );
				g_dpCoreSrvr.SendPartyChangeName( pParty->m_uPartyId, sParty );
			}
			else
			{
				// �̹� �׷� �̸��� �ִ�~ �ٸ������ �������
				SendErrorParty( ERROR_DIFFERNTUSERNAME, pPlayer );
			}
			
		}
		else
		{
			// �̹� ��ȸ�ش��� �ƴϹǷ� �̸��� �����Ҽ� ����~
		}
	}
	else
	{
		// ������ �ƴϴ� �׷��Ƿ� �Ҽ��� ����~
	}
}



void CDPCacheSrvr::OnPartyChangeItemMode( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long _uidPlayer;
	int nItemMode;
	ar >> _uidPlayer >> nItemMode;

	CPlayer* pPlayer;
	
	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	
	pPlayer	= g_PlayerMng.GetPlayerBySerial( dpidUser );
	
	if( !pPlayer )
		return;
	
	CParty* pParty;
	CMclAutoLock	Lock2( g_PartyMng.m_AddRemoveLock );
	
	pParty	= g_PartyMng.GetParty( pPlayer->m_uPartyId );
	if( NULL == pParty )
	{
		// ��Ƽ ã�� ����
		SendErrorParty( ERROR_NOPARTY, pPlayer );
		return;
	}

	if( pParty->IsLeader( pPlayer->uKey ) )
	{	
		pParty->m_nTroupeShareItem = nItemMode;
		g_dpCoreSrvr.SendPartyChangeItemMode( pPlayer->m_uPartyId, nItemMode );
	}
	else
	{
		// ������ �ƴϴ� �׷��Ƿ� �Ҽ��� ����~
	}
}

void CDPCacheSrvr::OnPartyChangeExpMode( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long _uidPlayer;
	int nExpMode;
	ar >> _uidPlayer >> nExpMode;

	CPlayer* pPlayer;
	
	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	
	pPlayer	= g_PlayerMng.GetPlayerBySerial( dpidUser );
	
	if( !pPlayer )
		return;
	
	CParty* pParty;
	CMclAutoLock	Lock2( g_PartyMng.m_AddRemoveLock );
	
	pParty	= g_PartyMng.GetParty( pPlayer->m_uPartyId );
	if( NULL == pParty )
	{
		// ��Ƽ ã�� ����
		SendErrorParty( ERROR_NOPARTY, pPlayer );
		return;
	}

	if( pParty->IsLeader( pPlayer->uKey ) )
	{	
		if( pParty->GetKind() == PARTY_KIND_EXTEND )
		{
			pParty->m_nTroupsShareExp = nExpMode;
			g_dpCoreSrvr.SendPartyChangeExpMode( pPlayer->m_uPartyId, nExpMode );
		}
		else
		{
			// �̹� ��ȸ�ش��� �ƴϹǷ� �̸��� �����Ҽ� ����~
		}
	}
	else
	{
		// ������ �ƴϴ� �׷��Ƿ� �Ҽ��� ����~
	}
}

void CDPCacheSrvr::OnPartyChangeTroup( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long _uidPlayer;
	BOOL   bSendName;
	char szPartyName[33];
	ar >> _uidPlayer;
	ar >> bSendName;
	if( bSendName )
		ar.ReadString( szPartyName, _countof( szPartyName ) );

	CPlayer* pPlayer;
		
	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	
	pPlayer	= g_PlayerMng.GetPlayerBySerial( dpidUser );
	
	if( !pPlayer )
		return;
	
	CParty* pParty;
	CMclAutoLock	Lock2( g_PartyMng.m_AddRemoveLock );
	
	pParty	= g_PartyMng.GetParty( pPlayer->m_uPartyId );
	if( NULL == pParty )
	{
		// ��Ƽ ã�� ����
		SendErrorParty( ERROR_NOPARTY, pPlayer );
		return;
	}
	
	if( pParty->IsLeader( pPlayer->uKey ) )
	{	
		if( pParty->GetKind() == PARTY_KIND_BASE && pParty->GetLevel() >= MAX_PARTY_BASE_LEVEL )
		{
			LPCSTR lpszPlayer = g_PartyMng.GetPartyString( pPlayer->uKey );
			if( lpszPlayer != NULL && 0 == strcmp( lpszPlayer, szPartyName ) )
			{
				FLStrcpy( pParty->m_sParty, _countof( pParty->m_sParty ), lpszPlayer );
				pParty->SetKind( PARTY_KIND_EXTEND );
				g_dpCoreSrvr.SendPartyChangeTroup( pParty->m_uPartyId, pParty->m_sParty );
//				g_dpCoreSrvr.SendSetPartyMode( pParty->m_uPartyId, PARTY_PARSKILL_MODE, FALSE );
			}
			else
			{
				if( g_PartyMng.IsPartyName( szPartyName ) == FALSE )		
				{
					pParty->SetKind( PARTY_KIND_EXTEND );
					g_dpCoreSrvr.SendPartyChangeTroup( pParty->m_uPartyId, pParty->m_sParty );						
//					g_dpCoreSrvr.SendSetPartyMode( pParty->m_uPartyId, PARTY_PARSKILL_MODE, FALSE );
					
					if( lpszPlayer != NULL )
						g_PartyMng.RemovePartyName( pPlayer->uKey, lpszPlayer );

					g_PartyMng.AddPartyName( pPlayer->uKey, szPartyName );
					g_dpDatabaseClient.SendAddPartyName( pPlayer->uKey, szPartyName );
					g_dpCoreSrvr.SendPartyChangeName( pParty->m_uPartyId, szPartyName );
					FLStrcpy( pParty->m_sParty, _countof( pParty->m_sParty ), szPartyName );
				}
				else
				{
					// �̹� �׷� �̸��� �ִ�~ �ٸ������ �������
					SendErrorParty( ERROR_DIFFERNTUSERNAME, pPlayer );
				}
			}
		}
		else
		{
			// �̹� ��ȸ�ش��̴�
		}
	}
	else
	{
		// ������ �ƴϴ� �׷��Ƿ� �Ҽ��� ����~
	}
}


void CDPCacheSrvr::OnDestroyGuild( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long _idMaster;
	ar >> _idMaster;

	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	CPlayer* pMaster = g_PlayerMng.GetPlayerBySerial( dpidUser );
	if( !pMaster )
	{
		// player not found
		return;
	}

	CGuild* pGuild;
	CMclAutoLock	Lock2( g_GuildMng.m_AddRemoveLock );
	pGuild	= g_GuildMng.GetGuild( pMaster->m_idGuild );
	if( !pGuild || FALSE == pGuild->IsMember( pMaster->uKey ) )
	{
		// guild not found
		// OR is not member
		SendDefinedText( TID_GAME_COMNOHAVECOM, pMaster->dpidCache, pMaster->dpidUser, "" );
		pMaster->m_idGuild	= 0;
		return;
	}

	if( FALSE == pGuild->IsMaster( pMaster->uKey ) )
	{
		// is not leader
		SendDefinedText( TID_GAME_COMDELNOTKINGPIN, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}

	if( pGuild->GetWar() )
	{
		SendDefinedText( TID_GAME_GUILDWARNODISMISS, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}

	CGuildMember* pMember;
	CPlayer* pPlayer;
	for( map<u_long, CGuildMember*>::iterator i = pGuild->m_mapPMember.begin();
		i != pGuild->m_mapPMember.end(); ++i )
	{
		pMember		= i->second;
		pPlayer	= g_PlayerMng.GetPlayer( pMember->m_idPlayer );
		if( pPlayer )
		{
			pPlayer->m_idGuild	= 0;
			pPlayer->m_tGuildMember = CTime::GetCurrentTime();
			pPlayer->m_tGuildMember += CTimeSpan( 7, 0, 0, 0 );
		}
		
	}
	u_long idGuild	= pGuild->m_idGuild;
	g_GuildMng.RemoveGuild( idGuild );
	g_dpDatabaseClient.SendDestroyGuild( idGuild, pMaster->uKey );	// databaseserver
	g_dpCoreSrvr.SendDestroyGuild( idGuild );	// worldserver
}

// fixme - raiders
void CDPCacheSrvr::OnAddGuildMember( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long idMaster;//, idPlayer;
	GUILD_MEMBER_INFO	info;
	ar >> idMaster;// >> idPlayer;
	ar.Read( &info, sizeof(GUILD_MEMBER_INFO) );
	CPlayer* pMaster, *pPlayer;

	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );

	pMaster	= g_PlayerMng.GetPlayer( idMaster );
	pPlayer	= g_PlayerMng.GetPlayerBySerial( dpidUser );

	if( !pPlayer )
		return;
	if( pPlayer->uKey != info.idPlayer )
		return;
	if( !pMaster )
	{
		SendDefinedText( TID_GAME_GUILDCHROFFLINE, pPlayer->dpidCache, pPlayer->dpidUser, "" );
		return;
	}

	CTime tCurrent = CTime::GetCurrentTime();

	if( !g_PlayerMng.IsOperator( info.idPlayer ) && tCurrent < pPlayer->m_tGuildMember )
	{
		if( g_xFlyffConfig->GetMainLanguage() == LANG_FRE || g_xFlyffConfig->GetMainLanguage() == LANG_GER )	// ���� - ������ �� 2�ٷ� ���� �ؽ�Ʈ...
		{
			SendDefinedText( TID_GAME_GUILDNOTINCLUDE_01, pMaster->dpidCache, pMaster->dpidUser, "" );
			SendDefinedText( TID_GAME_GUILDNOTINCLUDE_01, pMaster->dpidCache, pMaster->dpidUser, "" );
			SendDefinedText( TID_GAME_GUILDNOTINCLUDE_02, pPlayer->dpidCache, pPlayer->dpidUser, "" );
			SendDefinedText( TID_GAME_GUILDNOTINCLUDE_02, pPlayer->dpidCache, pPlayer->dpidUser, "" );
		}
		else
		{
			SendDefinedText( TID_GAME_GUILDNOTINCLUDE, pMaster->dpidCache, pMaster->dpidUser, "" );
			SendDefinedText( TID_GAME_GUILDNOTINCLUDE, pPlayer->dpidCache, pPlayer->dpidUser, "" );
		}
		return;
	}
		

	CMclAutoLock	Lock2( g_GuildMng.m_AddRemoveLock );
//	1
	CGuild* pGuild	= g_GuildMng.GetGuild( pMaster->m_idGuild );
	if( !pGuild || !pGuild->IsMember( pMaster->uKey ) )
	{
		// guild not found
		SendDefinedText( TID_GAME_COMNOEXCOM, pPlayer->dpidCache, pPlayer->dpidUser, "" );
		return;
	}

	if( pGuild->GetWar() )
	{
		SendDefinedText( TID_GAME_GUILDWARNOINVATE, pPlayer->dpidCache, pPlayer->dpidUser, "" );
		return;
	}

//	2
	CGuild* pGuildtmp	= g_GuildMng.GetGuild( pPlayer->m_idGuild );
	if( pGuildtmp && pGuildtmp->IsMember( pPlayer->uKey ) )
	{
		// guild exists
		SendDefinedText( TID_GAME_COMHAVECOM, pPlayer->dpidCache, pPlayer->dpidUser, "" );
		return;
	}
	pPlayer->m_idGuild	= 0;
//	3
//	if( pGuild->GetSize() >= MAX_GM_SIZE )
	if( pGuild->GetSize() >= pGuild->GetMaxMemberSize() )
	{
		// is full
		SendDefinedText( TID_GAME_COMOVERMEMBER, pPlayer->dpidCache, pPlayer->dpidUser, "" );
		return;
	}
//
	CGuildMember* pMember	= new CGuildMember;
	pMember->m_idPlayer	= pPlayer->uKey;
	pMember->m_nMemberLv	= GUD_ROOKIE;
	if( pGuild->AddMember( pMember ) )
	{
		pPlayer->m_idGuild	= pGuild->m_idGuild;
		g_dpCoreSrvr.SendAddGuildMember( info, pGuild->m_idGuild );
		g_dpDatabaseClient.SendAddGuildMember( pPlayer->uKey, pGuild->m_idGuild, pMaster->uKey );
		CPlayer* pPlayertmp;
		for( map<u_long, CGuildMember*>::iterator i = pGuild->m_mapPMember.begin();
			i != pGuild->m_mapPMember.end(); ++i )
		{
			pPlayertmp	= g_PlayerMng.GetPlayer( i->second->m_idPlayer );
			if( pPlayertmp )
			{
				if( pPlayertmp == pPlayer )
					SendGuild( pGuild, pPlayer );
				else
					SendAddGuildMember( info, pPlayer->lpszPlayer, pGuild->m_idGuild, pPlayertmp );
			}
		}
	}
	else
	{
		SAFE_DELETE( pMember );
	}
}

void CDPCacheSrvr::OnRemoveGuildMember( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long _idMaster, idPlayer;
	ar >> _idMaster >> idPlayer;

	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );

	CPlayer* pMaster	= g_PlayerMng.GetPlayerBySerial( dpidUser );
	CPlayer* pPlayer	= g_PlayerMng.GetPlayer( idPlayer );

	if( !pMaster )
		return;

	CGuild* pGuild;
	CMclAutoLock	Lock2( g_GuildMng.m_AddRemoveLock );
//	1
	pGuild	= g_GuildMng.GetGuild( pMaster->m_idGuild );
	if( !pGuild || FALSE == pGuild->IsMember( pMaster->uKey ) )
	{
		// guild not found
		// OR is not member
		SendDefinedText( TID_GAME_COMLEAVENOCOM, pMaster->dpidCache, pMaster->dpidUser, "" );
		pMaster->m_idGuild		= 0;
		return;
	}

	if( pGuild->GetWar() )
	{
		SendDefinedText( TID_GAME_GUILDWARNOMEMBER, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}
	
	if( pMaster->uKey != idPlayer )
	{
//		2
		if( FALSE == pGuild->IsMember( idPlayer ) )
		{
			// is not member
			SendDefinedText( TID_GAME_GUILDNOTMEMBER, pMaster->dpidCache, pMaster->dpidUser, "%s", pGuild->m_szGuild );
			return;
		}
//		3
		if( FALSE == pGuild->IsMaster( pMaster->uKey ) )
		{
			// is not leader
			SendDefinedText( TID_GAME_COMLEAVENOKINGPIN, pMaster->dpidCache, pMaster->dpidUser, "" );
			return;
		}
	}
	else
	{
//		4
		if( pGuild->IsMaster( pMaster->uKey ) )
		{
			// do not remove leader
			SendDefinedText( TID_GAME_COMLEAVEKINGPIN, pMaster->dpidCache, pMaster->dpidUser, "" );
			return;
		}
	}
//
	if( pGuild->RemoveMember( idPlayer ) )
	{
		if( pPlayer )
		{
			pPlayer->m_tGuildMember = CTime::GetCurrentTime();
			pPlayer->m_tGuildMember += CTimeSpan( 7, 0, 0, 0 );
			pPlayer->m_idGuild	= 0;
			SendRemoveGuildMember( idPlayer, pGuild->m_idGuild, pPlayer );
		}
//		
		CPlayer* pPlayertmp;
		for( map<u_long, CGuildMember*>::iterator i = pGuild->m_mapPMember.begin();
			i != pGuild->m_mapPMember.end(); ++i )
		{
			pPlayertmp	= g_PlayerMng.GetPlayer( i->second->m_idPlayer );
			if( pPlayertmp )
				SendRemoveGuildMember( idPlayer, pGuild->m_idGuild, pPlayertmp );
		}
//
		g_dpCoreSrvr.SendRemoveGuildMember( idPlayer, pGuild->m_idGuild );
		g_dpDatabaseClient.SendRemoveGuildMember( idPlayer, pGuild->m_idGuild, pMaster->uKey );
	}
}

void CDPCacheSrvr::OnGuildMemberLv( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long _idMaster, idPlayer;
	int nMemberLv;
	ar >> _idMaster >> idPlayer >> nMemberLv;

	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );

	CPlayer* pMaster	= g_PlayerMng.GetPlayerBySerial( dpidUser );
	//CPlayer* pPlayer	= g_PlayerMng.GetPlayer( idPlayer );

	if( !pMaster )
		return;

	CMclAutoLock	Lock2( g_GuildMng.m_AddRemoveLock );
	
	CGuild* pGuild	= g_GuildMng.GetGuild( pMaster->m_idGuild );
	if( !pGuild || FALSE == pGuild->IsMember( pMaster->uKey ) )
	{
		// guild not found
		// OR is not member
		SendDefinedText( TID_GAME_COMLEAVENOCOM, pMaster->dpidCache, pMaster->dpidUser, "" );
		pMaster->m_idGuild		= 0;
		return;
	}

	if( pGuild->GetWar() )
	{
		SendDefinedText( TID_GAME_GUILDWARNOMEMBER, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}

	if( FALSE == pGuild->IsMember( idPlayer ) )
	{
		// is not member
		SendDefinedText( TID_GAME_GUILDNOTMEMBER, pMaster->dpidCache, pMaster->dpidUser, "%s", pGuild->m_szGuild );
		return;
	}
	CGuildMember* pMember1	= pGuild->GetMember( pMaster->uKey );
	CGuildMember* pMember2	= pGuild->GetMember( idPlayer );
	if( pMember1->m_nMemberLv >= pMember2->m_nMemberLv )
	{
		SendDefinedText( TID_GAME_GUILDAPPOVER, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}
	if( pMember1->m_nMemberLv >= nMemberLv )
	{
		SendDefinedText( TID_GAME_GUILDWARRANTREGOVER, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}
	if( !pGuild->IsCmdCap( pMember1->m_nMemberLv, PF_MEMBERLEVEL ) )
	{
		SendDefinedText( TID_GAME_GUILDAPPNOTWARRANT, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}

	if( nMemberLv < 0 || nMemberLv >= 5 )
	{
		SendDefinedText( TID_GAME_GUILDWARRANTREGOVER, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}

	if( ( pGuild->GetMemberLvSize( nMemberLv ) + 1 ) > pGuild->GetMaxMemberLvSize( nMemberLv ) )
	{
		SendDefinedText( TID_GAME_GUILDAPPNUMOVER, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}

	pMember2->m_nMemberLv	= static_cast< BYTE >( nMemberLv );
	pMember2->m_nClass = 0;

	g_dpCoreSrvr.SendGuildMemberLv( idPlayer, nMemberLv, pGuild->m_idGuild );
	g_dpDatabaseClient.SendGuildMemberLv( idPlayer, nMemberLv );

	CGuildMember* pMember;
	CPlayer* pPlayertmp;
	for( map<u_long, CGuildMember*>::iterator i = pGuild->m_mapPMember.begin();
		i != pGuild->m_mapPMember.end(); ++i )
	{
		pMember		= i->second;
		pPlayertmp	= g_PlayerMng.GetPlayer( pMember->m_idPlayer );
		if( pPlayertmp )
			SendGuildMemberLv( idPlayer, nMemberLv, pPlayertmp );
	}
}

void CDPCacheSrvr::OnGuildAuthority( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long _uidPlayer, _uGuildId;
	DWORD dwAuthority[MAX_GM_LEVEL];
	
	ar >> _uidPlayer >> _uGuildId;
	
	ar.Read( dwAuthority, sizeof(dwAuthority) );
	
	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	CMclAutoLock	Lock2( g_GuildMng.m_AddRemoveLock );	

	CPlayer* pPlayer = g_PlayerMng.GetPlayerBySerial( dpidUser );	
	if( pPlayer == NULL )
		return;

	CGuild* pGuild = g_GuildMng.GetGuild( pPlayer->m_idGuild );
	if( pGuild && pGuild->IsMaster( pPlayer->uKey ) )
	{
		if( pGuild->GetWar() )
		{
			SendDefinedText( TID_GAME_GUILDWARNOMEMBER, pPlayer->dpidCache, pPlayer->dpidUser, "" );
			return;
		}
		memcpy( pGuild->m_adwPower, dwAuthority, sizeof(dwAuthority) );
		g_dpCoreSrvr.SendGuildAuthority( pPlayer->m_idGuild, dwAuthority );

		// GUILD DB AUTHORITY UPDATE
		g_dpDatabaseClient.SendGuildAuthority( pPlayer->m_idGuild, pGuild->m_adwPower );
	}
}

void CDPCacheSrvr::OnGuildSetName( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long _uidPlayer, _uGuildId;
	char szName[MAX_G_NAME];
	
	ar >> _uidPlayer >> _uGuildId;
	ar.ReadString( szName, _countof( szName ) );

	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	CMclAutoLock	Lock2( g_GuildMng.m_AddRemoveLock );	

	CPlayer* pPlayer = g_PlayerMng.GetPlayerBySerial( dpidUser );	
	if( pPlayer == NULL )
		return;

	CGuild* pGuild = g_GuildMng.GetGuild( pPlayer->m_idGuild );
	if( pGuild && pGuild->IsMaster( pPlayer->uKey ) )
	{
		CString str1, str2;
		str1 = pGuild->m_szGuild;
		str2	= "";
//		str2 = pPlayer->lpszPlayer;
		if( str1 == str2 )
		{
			if( g_GuildMng.SetName( pGuild, szName ) )
			{
				g_dpCoreSrvr.SendGuildSetName( pPlayer->m_idGuild, pGuild->m_szGuild );
				g_dpDatabaseClient.SendGuildSetName( pPlayer->m_idGuild, pGuild->m_szGuild );
				SendGuildSetName( pPlayer->m_idGuild, pGuild->m_szGuild );
			}
			else
			{
				// duplicated
				SendGuildError( pPlayer, 1 );
			}
		}
		else
		{
			//
		}
	}
	else
	{
		// is not master
		SendDefinedText( TID_GAME_COMDELNOTKINGPIN, pPlayer->dpidCache, pPlayer->dpidUser, "" );
	}
}

void CDPCacheSrvr::OnGuildPenya( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long _uidPlayer, _uGuildId;
	DWORD dwType, dwPenya;

	ar >> _uidPlayer >> _uGuildId;
	ar >> dwType >> dwPenya;

	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	CMclAutoLock	Lock2( g_GuildMng.m_AddRemoveLock );	

	CPlayer* pPlayer = g_PlayerMng.GetPlayerBySerial( dpidUser );	
	if( pPlayer == NULL )
		return;

	CGuild* pGuild = g_GuildMng.GetGuild( pPlayer->m_idGuild );
	if( pGuild && pGuild->IsMaster( pPlayer->uKey ) )
	{
		if( 0 <= dwPenya && dwPenya < 1000000 )
		{
			pGuild->m_adwPenya[dwType] = dwPenya;
			g_dpCoreSrvr.SendGuildPenya( pPlayer->m_idGuild, dwType, dwPenya );
			
			// GUILD DB AUTHORITY UPDATE
			g_dpDatabaseClient.SendGuildPenya( pPlayer->m_idGuild, pGuild->m_adwPenya );
		}
		else
		{
			SendGuildError( pPlayer, 2 );
		}
	}
}

void CDPCacheSrvr::OnGuildClass( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	BYTE nFlag;
	u_long _idMaster, idPlayer;
	ar >> nFlag >> _idMaster >> idPlayer;

	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	
	CPlayer* pMaster	= g_PlayerMng.GetPlayerBySerial( dpidUser );
//	CPlayer* pPlayer	= g_PlayerMng.GetPlayer( idPlayer );
	
	if( !pMaster )
		return;

	CMclAutoLock	Lock2( g_GuildMng.m_AddRemoveLock );
	
	CGuild* pGuild	= g_GuildMng.GetGuild( pMaster->m_idGuild );
	if( !pGuild || FALSE == pGuild->IsMember( pMaster->uKey ) )
	{
		// guild not found
		// OR is not member
		SendDefinedText( TID_GAME_COMLEAVENOCOM, pMaster->dpidCache, pMaster->dpidUser, "" );
		pMaster->m_idGuild		= 0;
		return;
	}
	
	if( pGuild->GetWar() )
	{
		SendDefinedText( TID_GAME_GUILDWARNOMEMBER, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}
	
	if( FALSE == pGuild->IsMember( idPlayer ) )
	{
		// is not member
		SendDefinedText( TID_GAME_GUILDNOTMEMBER, pMaster->dpidCache, pMaster->dpidUser, "%s", pGuild->m_szGuild );
		return;
	}
	
	CGuildMember* pMember1	= pGuild->GetMember( pMaster->uKey );
	CGuildMember* pMember2	= pGuild->GetMember( idPlayer );

	if( !pGuild->IsCmdCap( pMember1->m_nMemberLv, PF_LEVEL ) )	
	{
		SendDefinedText( TID_GAME_GUILDAPPNOTWARRANT, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}
	int nClass = pMember2->m_nClass;
	if( nFlag == 1 ) // UP
	{
		++nClass;
	}
	else	// DOWN
	{
		--nClass;
	}
	
	if( nClass < 0 || nClass > 2 )
	{
		return;
	}
	
	pMember2->m_nClass	= nClass;
	
	g_dpCoreSrvr.SendGuildClass( idPlayer, nClass, pGuild->m_idGuild );
	g_dpDatabaseClient.SendGuildClass( idPlayer, nClass );
	
	CGuildMember* pMember;
	CPlayer* pPlayertmp;
	for( map<u_long, CGuildMember*>::iterator i = pGuild->m_mapPMember.begin();
	i != pGuild->m_mapPMember.end(); ++i )
	{
		pMember		= i->second;
		pPlayertmp	= g_PlayerMng.GetPlayer( pMember->m_idPlayer );
		if( pPlayertmp )
			SendGuildClass( idPlayer, nClass, pPlayertmp );
	}
}

void CDPCacheSrvr::OnChgMaster( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long _idPlayer, idPlayer2;
	ar >> _idPlayer >> idPlayer2;

	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );

	CPlayer* pPlayer = g_PlayerMng.GetPlayerBySerial( dpidUser );	
	CPlayer* pPlayer2	= g_PlayerMng.GetPlayer( idPlayer2 );
	if( !pPlayer || !pPlayer2 )
		return;
	if( pPlayer == pPlayer2 )
		return;

	CMclAutoLock	Lock2( g_GuildMng.m_AddRemoveLock );
	
	CGuild* pGuild	= g_GuildMng.GetGuild( pPlayer->m_idGuild );
	if( !pGuild || FALSE == pGuild->IsMember( pPlayer->uKey ) || FALSE == pGuild->IsMember( pPlayer2->uKey ) )
	{
		// guild not found
		// OR is not member
		SendDefinedText( TID_GAME_COMLEAVENOCOM, pPlayer->dpidCache, pPlayer->dpidUser, "" );
		pPlayer->m_idGuild		= 0;
		return;
	}
	if( pGuild->GetWar() )
	{
		SendDefinedText( TID_GAME_GUILDWARNOMEMBER, pPlayer->dpidCache, pPlayer->dpidUser, "" );
		return;
	}

	if( pGuild->IsMaster( pPlayer->uKey ) )
	{
		pGuild->m_idMaster	= idPlayer2;	// set master to guild

		g_dpCoreSrvr.SendChgMaster( pPlayer->uKey, idPlayer2, pGuild->m_idGuild );
		g_dpDatabaseClient.SendChgMaster( pPlayer->uKey, idPlayer2 );

		CGuildMember* pMember;
		pMember	= pGuild->GetMember( pPlayer->uKey );
		if( pMember )
		{
			pMember->m_nMemberLv	= GUD_ROOKIE;
			pMember->m_nClass = 0;
		}
		pMember	= pGuild->GetMember( idPlayer2 );
		if( pMember )
		{
			pMember->m_nMemberLv	= GUD_MASTER;
			pMember->m_nClass = 0;
		}

		CPlayer* pPlayertmp;
		for( map<u_long, CGuildMember*>::iterator i = pGuild->m_mapPMember.begin();
		i != pGuild->m_mapPMember.end(); ++i )
		{
			pMember		= i->second;
			pPlayertmp	= g_PlayerMng.GetPlayer( pMember->m_idPlayer );
			if( pPlayertmp )
				SendChgMaster( pPlayer->uKey, idPlayer2, pPlayertmp );
		}
	}
}

void CDPCacheSrvr::OnGuildNickName( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long _idMaster, idPlayer;
	char strNickName[MAX_GM_ALIAS] = {0,};
	ar >> _idMaster >> idPlayer;
	ar.ReadString( strNickName, _countof( strNickName ) );

	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	
	CPlayer* pMaster = g_PlayerMng.GetPlayerBySerial( dpidUser );	
//	CPlayer* pPlayer	= g_PlayerMng.GetPlayer( idPlayer );
	
	if( !pMaster )
		return;
	
	CMclAutoLock	Lock2( g_GuildMng.m_AddRemoveLock );
	
	CGuild* pGuild	= g_GuildMng.GetGuild( pMaster->m_idGuild );
	if( !pGuild || FALSE == pGuild->IsMember( pMaster->uKey ) )
	{
		// guild not found
		// OR is not member
		SendDefinedText( TID_GAME_COMLEAVENOCOM, pMaster->dpidCache, pMaster->dpidUser, "" );
		pMaster->m_idGuild		= 0;
		return;
	}
	
	if( pGuild->m_nLevel < 10 )
	{
		SendDefinedText( TID_GAME_GUILDNOTLEVEL, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}
	
	if( pGuild->GetWar() )
	{
		SendDefinedText( TID_GAME_GUILDWARNOMEMBER, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}

	if( pGuild->IsMaster( pMaster->uKey ) )
	{
//		int nLength = GetStrLen( strNickName );
		int nLen	= lstrlen( strNickName );

		if( nLen < 2 || nLen > 12 )
		{
			SendDefinedText( TID_DIAG_0011_01, pMaster->dpidCache, pMaster->dpidUser, "" );
			//				g_WndMng.OpenMessageBox( _T( "��Ī�� �ѱ� 1���� �̻�, 6���� ���Ϸ� �Է� �Է��Ͻʽÿ�." ) );
			return;
		}
		
		CGuildMember* pMember2	= pGuild->GetMember( idPlayer );
		if( !pMember2 )		// �븸 - �ھ� ���� ũ����	// 2007/01/21
			return;

		FLStrcpy( pMember2->m_szAlias, _countof( pMember2->m_szAlias ), strNickName );
		
		g_dpCoreSrvr.SendGuildNickName( idPlayer, pMember2->m_szAlias, pGuild->m_idGuild );
		g_dpDatabaseClient.SendGuildNickName( idPlayer, pMember2->m_szAlias );
		
		CGuildMember* pMember;
		CPlayer* pPlayertmp;
		for( map<u_long, CGuildMember*>::iterator i = pGuild->m_mapPMember.begin();
		i != pGuild->m_mapPMember.end(); ++i )
		{
			pMember		= i->second;
			pPlayertmp	= g_PlayerMng.GetPlayer( pMember->m_idPlayer );
			if( pPlayertmp )
				SendGuildNickName( idPlayer, pMember2->m_szAlias, pPlayertmp );
		}
	}
}

void CDPCacheSrvr::SendGuild( CGuild* pGuild, CPlayer* pPlayer )
{
	if( !pPlayer )
		return;
	BEFORESENDSOLE( ar, PACKETTYPE_GUILD, pPlayer->dpidUser );
	ar << pGuild->m_idGuild;
	pGuild->Serialize( ar, FALSE );
	SEND( ar, this, pPlayer->dpidCache );
}

void CDPCacheSrvr::SendAddGuildMember( const GUILD_MEMBER_INFO & info, const char* lpszPlayer, u_long idGuild, CPlayer* pPlayer )
{
	if( !pPlayer )
		return;

	BEFORESENDSOLE( ar, PACKETTYPE_ADD_GUILD_MEMBER, pPlayer->dpidUser );
//	ar << idPlayer << idGuild;
	ar.Write( &info, sizeof(GUILD_MEMBER_INFO) );
	ar << idGuild;
	ar.WriteString( lpszPlayer );
	SEND( ar, this, pPlayer->dpidCache );
}

void CDPCacheSrvr::SendRemoveGuildMember( u_long idPlayer, u_long idGuild, CPlayer* pPlayer )
{
	if( !pPlayer )
		return;

	BEFORESENDSOLE( ar, PACKETTYPE_REMOVE_GUILD_MEMBER, pPlayer->dpidUser );
	ar << idPlayer << idGuild;
	SEND( ar, this, pPlayer->dpidCache );
}

void CDPCacheSrvr::SendGuildMemberLv( u_long idPlayer, int nMemberLv, CPlayer* pPlayer )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GUILD_MEMBER_LEVEL, pPlayer->dpidUser );
	ar << idPlayer << nMemberLv;
	SEND( ar, this, pPlayer->dpidCache );
}


void CDPCacheSrvr::SendGuildClass( u_long idPlayer, int nClass, CPlayer* pPlayer )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GUILD_CLASS, pPlayer->dpidUser );
	ar << idPlayer << nClass;
	SEND( ar, this, pPlayer->dpidCache );
}

void CDPCacheSrvr::SendGuildNickName( u_long idPlayer, LPCTSTR strNickName, CPlayer* pPlayer )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GUILD_NICKNAME, pPlayer->dpidUser );
	ar << idPlayer;
	ar.WriteString( strNickName );
	SEND( ar, this, pPlayer->dpidCache );
}

void CDPCacheSrvr::SendChgMaster( u_long idPlayer, u_long idPlayer2, CPlayer* pPlayer )
{
	BEFORESENDSOLE( ar, PACKETTYPE_CHG_MASTER, pPlayer->dpidUser );
	ar << idPlayer << idPlayer2;
	SEND( ar, this, pPlayer->dpidCache );
}

void CDPCacheSrvr::SendGuildSetName( u_long idGuild, const char* szName )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GUILD_SETNAME, DPID_ALLPLAYERS );
	ar << idGuild;
	ar.WriteString( szName );
	SEND( ar, this, DPID_ALLPLAYERS );
}

void CDPCacheSrvr::SendGuildMsgControl( GUILD_MSG_HEADER* pHeader, DWORD pPenya, BYTE cbCloak )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GUILD_MSG_CONTROL, DPID_ALLPLAYERS );

	ar.Write( pHeader, sizeof(GUILD_MSG_HEADER) );
	ar << pPenya;
	ar << cbCloak;
	SEND( ar, this, DPID_ALLPLAYERS );
}

void CDPCacheSrvr::SendGuildChat( const char* lpszPlayer, const char* sChat, CPlayer* pPlayer, OBJID objid )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GUILD_CHAT, pPlayer->dpidUser );
	ar << objid;
	ar.WriteString( lpszPlayer );
	ar.WriteString( sChat );
	SEND( ar, this, pPlayer->dpidCache );
}

void CDPCacheSrvr::SendGuildMemberLogin( CPlayer* pTo, BYTE nLogin, u_long uPlayerId, u_long uMultiNo )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GUILD_GAMELOGIN, pTo->dpidUser );
	ar << nLogin << uPlayerId << uMultiNo;
	SEND( ar, this, pTo->dpidCache );
}

void CDPCacheSrvr::SendGuildMemberGameJoin( CPlayer * pTo, int nMaxLogin, u_long uLoginPlayerId[], u_long uLoginGuildMulti[] )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GUILD_GAMEJOIN, pTo->dpidUser );
	ar << nMaxLogin;
	ar.Write( uLoginPlayerId, sizeof( u_long ) * nMaxLogin );
	ar.Write( uLoginGuildMulti, sizeof( u_long) * nMaxLogin );
	SEND( ar, this, pTo->dpidCache );
}

void CDPCacheSrvr::SendGuildError( CPlayer * pTo, int nError )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GUILD_ERROR, pTo->dpidUser );
	ar << nError;
	SEND( ar, this, pTo->dpidCache );
}

// fixme - raiders
void CDPCacheSrvr::OnAddFriend( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long uidSend, _uidFriend;
	BYTE nSendSex, nFriendSex;
	LONG nSendJob, nFriendJob;
	ar >> uidSend >> _uidFriend;
	ar >> nSendSex >> nFriendSex;
	ar >> nSendJob >> nFriendJob;

	CPlayer* pSender;
	CPlayer* pFriend;
	
	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	
	pSender = g_PlayerMng.GetPlayer( uidSend );
	pFriend	= g_PlayerMng.GetPlayerBySerial( dpidUser );

	if( !pFriend )
		return;
	if( !pSender )
	{
		// #
		return;
	}
	pSender->Lock();
	BOOL bFullA	= pSender->m_RTMessenger.size() >= MAX_FRIEND;
	pSender->Unlock();
	pFriend->Lock();
	BOOL bFullB	= pFriend->m_RTMessenger.size() >= MAX_FRIEND;
	pFriend->Unlock();
	if( bFullA )
	{
		// #
		return;
	}
	if( bFullB )
	{
		// #
		return;
	}
	pSender->Lock();
	pSender->m_RTMessenger.SetFriend( pFriend->uKey, NULL );
	pSender->Unlock();
	pFriend->Lock();
	pFriend->m_RTMessenger.SetFriend( uidSend, NULL );
	pFriend->Unlock();
	g_dpDatabaseClient.QueryAddMessenger( uidSend, pFriend->uKey );
	g_dpCoreSrvr.SendAddFriend( uidSend, pFriend->uKey, nSendSex, nFriendSex, nSendJob, nFriendJob );
}

void CDPCacheSrvr::OnGetFriendState( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long _uidPlayer;
	ar >> _uidPlayer;

	CPlayer* pPlayer;
	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	pPlayer	= g_PlayerMng.GetPlayerBySerial( dpidUser );

	if( pPlayer )
		SendFriendState( pPlayer );
}

void CDPCacheSrvr::OnSetFrinedState( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long _uidPlayer;
	ar >> _uidPlayer;

	CPlayer* pPlayer;
	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	pPlayer	= g_PlayerMng.GetPlayerBySerial( dpidUser );
	
	if( pPlayer )
	{
		int state;
		ar >> state;
		pPlayer->m_RTMessenger.SetState( state );
		SendSetFriendState( pPlayer );
		if( pPlayer->m_RTMessenger.GetState() != FRS_AUTOABSENT )
			g_dpCoreSrvr.SendSetFriendState( pPlayer->uKey, pPlayer->m_RTMessenger.GetState() );
	}
}

void CDPCacheSrvr::OnFriendInterceptState( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long _uidPlayer, uidFriend;
	ar >> _uidPlayer >> uidFriend;

	CPlayer* pPlayer, *pFriendUser;
	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );

	pPlayer	= g_PlayerMng.GetPlayerBySerial( dpidUser );
	pFriendUser		= g_PlayerMng.GetPlayer( uidFriend );

	if( !pPlayer )
		return;
	pPlayer->Lock();
	Friend* pFriend		= pPlayer->m_RTMessenger.GetFriend( uidFriend );
	if( pFriend )
	{
		if( pFriend->bBlock )	// release block
		{
			pFriend->bBlock		= FALSE;
			pFriend->dwState	= pFriendUser? pFriendUser->m_RTMessenger.GetState(): FRS_OFFLINE;
			if( pFriendUser )
				SendFriendNoIntercept( pPlayer, pFriendUser, pFriend->dwState );
			else
				SendFriendNoIntercept( pPlayer, uidFriend, pFriend->dwState );
			g_dpDatabaseClient.QueryUpdateMessenger( pPlayer->uKey, uidFriend, FALSE );
		}
		else	// set block
		{
			pFriend->bBlock		= TRUE;
			if( pFriendUser )
				SendFriendIntercept( pPlayer, pFriendUser );	
			else
				SendFriendIntercept( pPlayer, uidFriend );
			g_dpDatabaseClient.QueryUpdateMessenger( pPlayer->uKey, uidFriend, TRUE );
		}
	}

	pPlayer->Unlock();
	g_dpCoreSrvr.SendFriendInterceptState( pPlayer->uKey, uidFriend );
}



void CDPCacheSrvr::OnRemoveFriend( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long _uidPlayer, uidFriend;
	ar >> _uidPlayer >> uidFriend;

	CPlayer* pPlayer, *pFriendUser;
	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	
	pPlayer = g_PlayerMng.GetPlayerBySerial( dpidUser );

	if( pPlayer )
	{
		pPlayer->Lock();
		pPlayer->m_RTMessenger.RemoveFriend( uidFriend );
		pPlayer->Unlock();
		g_dpCoreSrvr.SendRemoveFriend( pPlayer->uKey, uidFriend );
		g_dpDatabaseClient.QueryDeleteMessenger( pPlayer->uKey, uidFriend );

		pFriendUser	= g_PlayerMng.GetPlayer( uidFriend );
		if( !pFriendUser )
			return;
		pFriendUser->Lock();
		pFriendUser->m_RTMessenger.RemoveFriend( pPlayer->uKey );
		pFriendUser->Unlock();
		BEFORESENDSOLE( arSend, PACKETTYPE_REMOVEFRIENDSTATE, pFriendUser->dpidUser );
		arSend << pPlayer->uKey;
		SEND( arSend, this, pFriendUser->dpidCache );
	}
}

void CDPCacheSrvr::SendErrorParty( DWORD dwError, CPlayer* pPlayer )
{
	BEFORESENDSOLE( ar, PACKETTYPE_ERRORPARTY, pPlayer->dpidUser );
	ar << dwError;
	SEND( ar, this, pPlayer->dpidCache );
}

void CDPCacheSrvr::SendAddFriend( u_long uLeader, u_long uMember, LONG nLeaderJob, BYTE nLeaderSex, char * szLeaderName, CPlayer* pMember )
{
	BEFORESENDSOLE( ar, PACKETTYPE_ADDFRIENDNAMEREQEST, pMember->dpidUser );
	ar << uLeader << uMember;
	ar << nLeaderJob << nLeaderSex;
	ar.WriteString( szLeaderName );
	SEND( ar, this, pMember->dpidCache );
}

void CDPCacheSrvr::SendAddFriendNotFound( char * szMemberName, CPlayer* pLeader )
{
	BEFORESENDSOLE( ar, PACKETTYPE_ADDFRIENDNAMENOTFOUND, pLeader->dpidUser );
	ar.WriteString( szMemberName );
	SEND( ar, this, pLeader->dpidCache );
}

void CDPCacheSrvr::SendBlock( BYTE nGu, char *szName, CPlayer* pTo )
{
	BEFORESENDSOLE( ar, PACKETTYPE_BLOCK, pTo->dpidUser );
	ar << nGu;
	ar.WriteString( szName );
	SEND( ar, this, pTo->dpidCache );
}

void CDPCacheSrvr::OnSurrender( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long _idPlayer;
	ar >> _idPlayer;

	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	CMclAutoLock	Lock2( g_GuildMng.m_AddRemoveLock );

	CPlayer* pPlayer = g_PlayerMng.GetPlayerBySerial( dpidUser );	
	if( !pPlayer )
		return;

	CGuildWar* pWar	= g_GuildWarMng.GetWar( pPlayer->m_idWar );
	if( !pWar )
	{
//		not in war
		SendDefinedText( TID_GAME_GUILDWARNOETC, pPlayer->dpidCache, pPlayer->dpidUser, "" );
		return;
	}
	
	CGuild* pDecl	= g_GuildMng.GetGuild( pWar->m_Decl.idGuild );
	CGuild* pAcpt	= g_GuildMng.GetGuild( pWar->m_Acpt.idGuild );
	if( !pDecl || !pAcpt )
		return;

	if( pDecl->m_idGuild == pPlayer->m_idGuild )
	{
		CGuildMember* pMember	= pDecl->GetMember( pPlayer->uKey );
		if( pMember )
		{
			pMember->m_nSurrender++;
			pWar->m_Decl.nSurrender++;

			g_dpCoreSrvr.SendSurrender( pWar->m_idWar, pPlayer->uKey, TRUE/*bDecl*/ );	// worldserver
			g_dpDatabaseClient.SendSurrender( pWar->m_idWar, pPlayer->uKey, pDecl->m_idGuild );	// trans
			SendSurrender( pWar->m_idWar, pDecl, pAcpt, pPlayer->uKey, pPlayer->lpszPlayer, TRUE/*bDecl*/ );	// neuz

			if( pDecl->IsMaster( pPlayer->uKey ) || ( ( pWar->m_Decl.nSurrender * 100 ) / pWar->m_Decl.nSize ) > 70 )
				g_GuildWarMng.Result( pWar, pDecl, pAcpt, WR_ACPT_SR );
		}
	}
	else if( pAcpt->m_idGuild == pPlayer->m_idGuild )
	{
		CGuildMember* pMember	= pAcpt->GetMember( pPlayer->uKey );
		if( pMember )
		{
			pMember->m_nSurrender++;
			pWar->m_Acpt.nSurrender++;

			g_dpCoreSrvr.SendSurrender( pWar->m_idWar, pPlayer->uKey, FALSE/*bDecl*/ );	// world
			g_dpDatabaseClient.SendSurrender( pWar->m_idWar, pPlayer->uKey, pAcpt->m_idGuild );	// trans
			SendSurrender( pWar->m_idWar, pDecl, pAcpt, pPlayer->uKey, pPlayer->lpszPlayer, FALSE/*bDecl*/ );	// neuz

			if( pAcpt->IsMaster( pPlayer->uKey ) ||	( ( pWar->m_Acpt.nSurrender * 100 ) / pWar->m_Acpt.nSize ) > 70 )
				g_GuildWarMng.Result( pWar, pDecl, pAcpt, WR_DECL_SR );
		}
	}
	pPlayer->m_idWar	= 0;
}

void CDPCacheSrvr::SendSurrender( u_long idWar, CGuild* pDecl, CGuild* pAcpt, u_long idPlayer, const char* sPlayer, BOOL bDecl )
{
	for( map<u_long, CGuildMember*>::iterator i = pDecl->m_mapPMember.begin();	// neuz
		i != pDecl->m_mapPMember.end(); ++i )
	{
		CPlayer* pPlayer	= g_PlayerMng.GetPlayer( i->second->m_idPlayer );
		if( pPlayer )
			SendSurrender( idWar, idPlayer, sPlayer, bDecl, pPlayer );
	}
	for( map<u_long, CGuildMember*>::iterator i	= pAcpt->m_mapPMember.begin(); i != pAcpt->m_mapPMember.end(); ++i )
	{
		CPlayer* pPlayer	= g_PlayerMng.GetPlayer( i->second->m_idPlayer );
		if( pPlayer )
			SendSurrender( idWar, idPlayer, sPlayer, bDecl, pPlayer );
	}
}

void CDPCacheSrvr::SendSurrender( u_long idWar, u_long idPlayer, const char* sPlayer, BOOL bDecl, CPlayer* pPlayer )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SURRENDER, pPlayer->dpidUser );
	ar << idWar << idPlayer;
	ar.WriteString( sPlayer );
	ar << bDecl;
	SEND( ar, this, pPlayer->dpidCache );
}

void CDPCacheSrvr::OnQueryTruce( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long _idPlayer;
	ar >> _idPlayer;

	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	CMclAutoLock	Lock2( g_GuildMng.m_AddRemoveLock );

	CPlayer* pPlayer = g_PlayerMng.GetPlayerBySerial( dpidUser );	
	if( !pPlayer )
		return;

	CGuildWar* pWar	= g_GuildWarMng.GetWar( pPlayer->m_idWar );
	if( !pWar )
		return;

	CGuild* pGuild	= g_GuildMng.GetGuild( pPlayer->m_idGuild );
	if( !pGuild )
		return;

	if( !pGuild->IsMaster( pPlayer->uKey ) )
	{
		// is not master
		return;
	}

	CGuild* pGuild2	= NULL;
	if( pWar->IsDecl( pGuild->m_idGuild ) )
		pGuild2		= g_GuildMng.GetGuild( pWar->m_Acpt.idGuild );
	else
		pGuild2		= g_GuildMng.GetGuild( pWar->m_Decl.idGuild );
	if( !pGuild2 )
		return;
	CPlayer* pMaster	= g_PlayerMng.GetPlayer( pGuild2->m_idMaster );
	if( !pMaster )
	{
		// master not found
		return;
	}
	SendQueryTruce( pMaster );
}

void CDPCacheSrvr::OnAcptTruce( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long _idPlayer;
	ar >> _idPlayer;

	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	CMclAutoLock	Lock2( g_GuildMng.m_AddRemoveLock );

	CPlayer* pPlayer	= g_PlayerMng.GetPlayerBySerial( dpidUser );
	if( !pPlayer )
		return;

	CGuildWar* pWar	= g_GuildWarMng.GetWar( pPlayer->m_idWar );
	if( !pWar )
		return;

	CGuild* pDecl	= g_GuildMng.GetGuild( pWar->m_Decl.idGuild );
	CGuild* pAcpt	= g_GuildMng.GetGuild( pWar->m_Acpt.idGuild );
	if( !pDecl || !pAcpt )
		return;

	g_GuildWarMng.Result( pWar, pDecl, pAcpt, WR_TRUCE );
}

void CDPCacheSrvr::OnDeclWar( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long _idMaster;
	char szGuild[MAX_G_NAME];
	ar >> _idMaster;
	ar.ReadString( szGuild, _countof( szGuild ) );

	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	
	CPlayer* pMaster = g_PlayerMng.GetPlayerBySerial( dpidUser );	
	if( !pMaster )
		return;
	
	CMclAutoLock	Lock2( g_GuildMng.m_AddRemoveLock );
	CGuild* pDecl	= g_GuildMng.GetGuild( pMaster->m_idGuild );
	if( !pDecl )
	{
		SendDefinedText( TID_GAME_COMNOHAVECOM, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}
	if( !pDecl->IsMaster( pMaster->uKey ) )
	{
		SendDefinedText( TID_GAME_COMDELNOTKINGPIN, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}
#ifndef __INTERNALSERVER
	if( pDecl->m_nLevel < 6 )
	{
		SendDefinedText( TID_GAME_GUILDWARREQLV6, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}
#endif	// __INTERNALSERVER

	if( pDecl->GetWar() )
	{
		SendDefinedText( TID_GAME_GUILDWARSTILLNOWAR, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}

	CGuild* pAcpt	= g_GuildMng.GetGuild( szGuild );
	if( !pAcpt )
	{
		SendDefinedText( TID_GAME_GUILDWARNOTHINGGUILD, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}
#ifndef __INTERNALSERVER
	if( pAcpt->m_nLevel < 6 )
	{
		SendDefinedText( TID_GAME_GUILDWAROHTERLV6, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}
#endif	// __INTERNALSERVER
	CPlayer* pPlayer	= g_PlayerMng.GetPlayer( pAcpt->m_idMaster );
	if( !pPlayer )
	{
		SendDefinedText( TID_GAME_GUILDWARMASTEROFF, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}
#ifndef __INTERNALSERVER
	if( pAcpt->GetSize() < 10 )
	{
		SendDefinedText( TID_GAME_GUILDWARMEMBER10, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}
#endif	// __INTERNALSERVER
	if( pAcpt->GetWar() )
	{
		SendDefinedText( TID_GAME_GUILDWAROTHERWAR, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}
	if( pDecl == pAcpt )
		return;

	SendDeclWar( pDecl->m_idGuild, pMaster->lpszPlayer, pPlayer );
}

// fixme	- raiders
void CDPCacheSrvr::OnAcptWar( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long _idMaster, idDecl;
	ar >> _idMaster >> idDecl;

	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	
	CPlayer* pMaster = g_PlayerMng.GetPlayerBySerial( dpidUser );	
	if( !pMaster )
		return;
	
	CMclAutoLock	Lock2( g_GuildMng.m_AddRemoveLock );
	CGuild* pAcpt	= g_GuildMng.GetGuild( pMaster->m_idGuild );
	if( !pAcpt )
	{
		SendDefinedText( TID_GAME_COMNOHAVECOM, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}
	if( !pAcpt->IsMaster( pMaster->uKey ) )
	{
		SendDefinedText( TID_GAME_COMDELNOTKINGPIN, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}
	if( pAcpt->GetWar() )
	{
		SendDefinedText( TID_GAME_GUILDWARNOREQUEST, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}

	CGuild* pDecl	= g_GuildMng.GetGuild( idDecl );
	if( !pDecl )
	{
		SendDefinedText( TID_GAME_GUILDWARNOFINDGUILD, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}

	if( pDecl == pAcpt )
	{
		return;
	}

	CPlayer* pPlayer	= g_PlayerMng.GetPlayer( pDecl->m_idMaster );
	if( !pPlayer )
	{
		// master is absent
		SendDefinedText( TID_GAME_GUILDWARMASTEROFF, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}
	if( pDecl->GetWar() )
	{
		SendDefinedText( TID_GAME_GUILDWAROTHERWAR, pMaster->dpidCache, pMaster->dpidUser, "" );
		return;
	}
	
	CGuildWar* pWar		= new CGuildWar;
	pWar->m_Decl.idGuild	= pDecl->m_idGuild;
	pWar->m_Decl.nSize	= pDecl->GetSize();
	pWar->m_Acpt.idGuild	= pAcpt->m_idGuild;
	pWar->m_Acpt.nSize	= pAcpt->GetSize();
	pWar->m_nFlag	= WF_WARTIME;
	pWar->m_time	= CTime::GetCurrentTime();

	u_long idWar	= g_GuildWarMng.AddWar( pWar );
	if( idWar > 0 )
	{
		pDecl->m_idWar	= idWar;
		pDecl->m_idEnemyGuild	= pAcpt->m_idGuild;
		pAcpt->m_idWar	= idWar;
		pAcpt->m_idEnemyGuild	= pDecl->m_idGuild;

		for( map<u_long, CGuildMember*>::iterator i = pDecl->m_mapPMember.begin(); i != pDecl->m_mapPMember.end(); ++i )
		{
			CPlayer* pPlayerDecl	= g_PlayerMng.GetPlayer( i->second->m_idPlayer );
			if( pPlayerDecl )
				pPlayerDecl->m_idWar	= idWar;
		}
		for( map<u_long, CGuildMember*>::iterator i	= pAcpt->m_mapPMember.begin(); i != pAcpt->m_mapPMember.end(); ++i )
		{
			CPlayer* pPlayerAcpt	= g_PlayerMng.GetPlayer( i->second->m_idPlayer );
			if( pPlayerAcpt )
				pPlayerAcpt->m_idWar	= idWar;
		}
		g_dpDatabaseClient.SendAcptWar( idWar, idDecl, pAcpt->m_idGuild );
		g_dpCoreSrvr.SendAcptWar( idWar, idDecl, pAcpt->m_idGuild );
		SendAcptWar( idWar, idDecl, pAcpt->m_idGuild );
	}
	else
	{
		SAFE_DELETE( pWar );
	}
}

void CDPCacheSrvr::SendDeclWar( u_long idDecl, const char* pszMaster, CPlayer* pPlayer )
{
	BEFORESENDSOLE( ar, PACKETTYPE_DECL_GUILD_WAR, pPlayer->dpidUser );
	ar << idDecl;
	ar.WriteString( pszMaster );
	SEND( ar, this, pPlayer->dpidCache );
}

void CDPCacheSrvr::SendAcptWar( u_long idWar, u_long idDecl, u_long idAcpt )
{
	BEFORESENDSOLE( ar, PACKETTYPE_ACPT_GUILD_WAR, DPID_ALLPLAYERS );
	ar << idWar << idDecl << idAcpt;
	SEND( ar, this, DPID_ALLPLAYERS );
}

void CDPCacheSrvr::SendWarEnd( u_long idWar, int nWptDecl, int nWptAcpt, int nType )
{
	BEFORESENDSOLE( ar, PACKETTYPE_WAR_END, DPID_ALLPLAYERS );
	ar << idWar << nWptDecl << nWptAcpt << nType;
	SEND( ar, this, DPID_ALLPLAYERS );
}

void CDPCacheSrvr::SendWarDead( u_long idWar, const char* lpszPlayer, BOOL bDecl, CPlayer* pPlayer )
{
	BEFORESENDSOLE( ar, PACKETTYPE_WAR_DEAD, pPlayer->dpidUser );
	ar << idWar;
	ar.WriteString( lpszPlayer );
	ar << bDecl;
	SEND( ar, this, pPlayer->dpidCache );
}

void CDPCacheSrvr::SendQueryTruce( CPlayer* pPlayer )
{
	BEFORESENDSOLE( ar, PACKETTYPE_QUERY_TRUCE, pPlayer->dpidUser );
	SEND( ar, this, pPlayer->dpidCache );
}

void CDPCacheSrvr::OnAddVote( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	char szTitle[MAX_BYTE_VOTETITLE];
	char szQuestion[MAX_BYTE_VOTEQUESTION];
	char szSelections[4][MAX_BYTE_VOTESELECT];

	u_long _idGuild, _idPlayer;

	ar >> _idGuild;
	ar >> _idPlayer;
	ar.ReadString( szTitle, _countof( szTitle ) );
	ar.ReadString( szQuestion, _countof( szQuestion ) );
	for( int i=0; i<4; ++i )
	{
		ar.ReadString( szSelections[i], _countof( szSelections[i] ) );
	}

	////////////////////////////////////////////////////////////////////
	CPlayer* pPlayer = g_PlayerMng.GetPlayerBySerial( dpidUser );	
	if( !pPlayer )
		return;

	CMclAutoLock	Lock2( g_GuildMng.m_AddRemoveLock );	
	
	CGuild* pGuild = g_GuildMng.GetGuild( pPlayer->m_idGuild );
	if( pGuild && pGuild->IsMaster( pPlayer->uKey ) )
	{
		g_dpDatabaseClient.SendAddVote( pPlayer->m_idGuild, szTitle, szQuestion, szSelections );
		// ����� ������ �޾Ƽ� ó���Ѵ�. CDPDatabaseClient::OnAddVoteResult()
	}	
}

void CDPCacheSrvr::OnRemoveVote( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long idVote;
	u_long _idGuild, _idPlayer;

	ar >> _idGuild;
	ar >> _idPlayer;
	ar >> idVote;
	
	////////////////////////////////////////////////////////////////////
	CPlayer* pPlayer = g_PlayerMng.GetPlayerBySerial( dpidUser );	
	if( !pPlayer )
		return;

	CMclAutoLock	Lock2( g_GuildMng.m_AddRemoveLock );	
	
	CGuild* pGuild = g_GuildMng.GetGuild( pPlayer->m_idGuild );
	if( pGuild && pGuild->IsMaster( pPlayer->uKey ) && pGuild->FindVote( idVote ) )
	{
		g_dpDatabaseClient.SendRemoveVote( idVote );
		pGuild->ModifyVote( idVote, REMOVE_VOTE, 0 );
		
		g_dpCoreSrvr.SendAddRemoveVote( pPlayer->m_idGuild, idVote );		// ��� ���弭���� �˸���.
	}	
}

void CDPCacheSrvr::OnCloseVote( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long _idGuild, _idPlayer;
	u_long idVote;

	ar >> _idGuild;
	ar >> _idPlayer;
	ar >> idVote;

	////////////////////////////////////////////////////////////////////
	CPlayer* pPlayer = g_PlayerMng.GetPlayerBySerial( dpidUser );	
	if( !pPlayer )
		return;

	CMclAutoLock	Lock2( g_GuildMng.m_AddRemoveLock );	
	
	CGuild* pGuild = g_GuildMng.GetGuild( pPlayer->m_idGuild );
	CGuildVote* pVote = pGuild->FindVote( idVote );
	if( pGuild && pGuild->IsMaster( pPlayer->uKey ) && pVote )
	{
		if( pVote->IsCompleted() )	// �Ϸ�� �����ΰ�?
			return;

		g_dpDatabaseClient.SendCloseVote( idVote );
		pVote->SetComplete();

		g_dpCoreSrvr.SendAddCloseVote( pPlayer->m_idGuild, idVote );		// ��� ���弭���� �˸���.
	}	
}

void CDPCacheSrvr::OnCastVote( CAr & ar, DPID /*dpidCache*/, DPID dpidUser, u_long /*uBufSize*/ )
{
	u_long _idGuild, _idPlayer;
	u_long idVote;
	BYTE cbSelection;

	ar >> _idGuild;
	ar >> _idPlayer;
	ar >> idVote;
	ar >> cbSelection;

	if( cbSelection > 3 )
		return;

	////////////////////////////////////////////////////////////////////

	CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
	CMclAutoLock	Lock2( g_GuildMng.m_AddRemoveLock );	
	
	CPlayer* pPlayer	= g_PlayerMng.GetPlayerBySerial( dpidUser );
	if( !pPlayer )
		return;

	CGuild* pGuild = g_GuildMng.GetGuild( pPlayer->m_idGuild );
	CGuildVote* pVote = pGuild->FindVote( idVote );
	CGuildMember* pMember = pGuild->GetMember( pPlayer->uKey );

	if( pGuild && pMember && pVote )
	{
		if( pVote->IsCompleted() )					// �Ϸ�� �����ΰ�?
			return;

		if( pMember->m_idSelectedVote == idVote )	// �̹� ��ǥ�ߴ���?
			return;

		GUILD_VOTE_SELECT& select = pVote->GetVoteSelect( cbSelection );
		if( select.szString == '\0' )				// ���ڿ��� ��ǥ�Ұ� 		
			return;

		g_dpDatabaseClient.SendCastVote( idVote, cbSelection );
		
		pMember->m_idSelectedVote = idVote;

		g_dpCoreSrvr.SendAddCastVote( pPlayer->m_idGuild, idVote, cbSelection );		// ��� ���弭���� �˸���.
	}	
}

void CDPCacheSrvr::SendUpdateGuildRank()
{
	BEFORESENDSOLE( ar, PACKETTYPE_UPDATE_GUILD_RANKING, DPID_ALLPLAYERS );

	CGuildRank::Instance()->Serialize( ar );

	SEND( ar, this, DPID_ALLPLAYERS );
}

// void CDPCacheSrvr::SendBuyingInfo( PBUYING_INFO2 pbi2, CPlayer* pPlayer )
// {
// 	BEFORESENDSOLE( ar, PACKETTYPE_BUYING_INFO, pPlayer->dpidUser );
// 	ar.Write( (void*)pbi2, sizeof(BUYING_INFO2) );
// 	SEND( ar, this, pPlayer->dpidCache );
// }

void CDPCacheSrvr::SendSetPlayerName( u_long idPlayer, const char* lpszPlayer )
{
	BEFORESENDSOLE( ar, PACKETTYPE_QUERYSETPLAYERNAME, DPID_ALLPLAYERS );
	ar << idPlayer;
	ar.WriteString( lpszPlayer );
	SEND( ar, this, DPID_ALLPLAYERS );
}

void CDPCacheSrvr::SendSnoop( const char* lpszString, CPlayer* pSnoop )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SNOOP, pSnoop->dpidUser );
	ar.WriteString( lpszString );
	SEND( ar, this, pSnoop->dpidCache );
}

void CDPCacheSrvr::SendUpdatePlayerData( u_long idPlayer, PlayerData* pPlayerData, CPlayer* pTo )
{
	BEFORESENDSOLE( ar, PACKETTYPE_UPDATE_PLAYER_DATA, pTo->dpidUser );
	ar << idPlayer;
	ar.Write( &pPlayerData->data, sizeof(sPlayerData) );
	SEND( ar, this, pTo->dpidCache );
}

void CDPCacheSrvr::SendLogout( u_long idPlayer, CPlayer* pTo )
{
	BEFORESENDSOLE( ar, PACKETTYPE_LOGOUT, pTo->dpidUser );
	ar << idPlayer;
	SEND( ar, this, pTo->dpidCache );
}

void CDPCacheSrvr::SendQuizSystemMessage( int nDefinedTextId, BOOL bAll, int nChannel, int nTime )
{
	BEFORESENDSOLE( ar, PACKETTYPE_QUIZ_NOTICE, DPID_ALLPLAYERS );
	ar << nDefinedTextId << bAll << nChannel << nTime;
	SEND( ar, this, DPID_ALLPLAYERS );
}

void	CDPCacheSrvr::SendCloseExistingConnection( const char* lpszAccount, LONG lError )
{
	BEFORESENDSOLE( ar, PACKETTYPE_CLOSE_EXISTING_CONNECTION, DPID_ALLPLAYERS );
	ar.WriteString( lpszAccount );
	ar << lError;
	SEND( ar, this, DPID_ALLPLAYERS );
}

void	CDPCacheSrvr::SendPacket( DWORD dwDPIDUser, DWORD dwDPIDCache, const FLPacket* pPacket )
{
	if( pPacket == NULL )
	{
		return;
	}

	CAr ar;
	u_long nBufSize = 0;
	ar << dwDPIDUser;

	if( pPacket->Serialize( ar ) == false )
	{
		return;
	}

	LPBYTE lpBuf	= ar.GetBuffer( &nBufSize );
	Send( lpBuf, nBufSize, dwDPIDCache );
}

// void	CDPCacheSrvr::OnBuyingInfo( CAr & ar, DPID /*dpidCache*/, DPID /*dpidUser*/, u_long /*uBufSize*/ )
// {
// 	BUYING_INFO2 bi2;
// 	ar.Read( (void*)&bi2, sizeof(BUYING_INFO2) );
// 	g_dpDatabaseClient.SendBuyingInfo( &bi2 );
// }

CDPCacheSrvr	g_DPCacheSrvr;