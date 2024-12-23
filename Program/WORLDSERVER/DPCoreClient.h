#ifndef __DPCORECLIENT_H__
#define __DPCORECLIENT_H__

#include <network/FLDPMng.h>
#include <MsgHdr.h>
#include "WorldServer.h"
#include <misc.h>

#include <guild.h>

#include <InstanceDungeonBase.h>

#define BEFOREPASS( ar, dw, wWorldSrvr, wCacheSrvr, dpid, objid )	\
	BEFORESENDDUAL( ar, PACKETTYPE_PASSAGE, MAKELONG( wWorldSrvr, wCacheSrvr ), dpid );	\
	ar << objid << dw;

#define PASS( ar )	\
	SEND( ar, this, DPID_SERVERPLAYER );

#define	BEFOREBROADCAST( ar, dw )	\
	BEFORESENDDUAL( ar, PACKETTYPE_BROADCAST, DPID_UNKNOWN, DPID_UNKNOWN );	\
	ar << dw;

#define	BROADCAST( ar )	\
	SEND( ar, this, DPID_SERVERPLAYER );


class CDPCoreClient : public CDPMng
{
private:
	WSAEVENT	m_hWait;
	u_long		m_uRecharge;
	volatile BOOL		m_bAlive;
public:
	BOOL	GetAlive() {	return m_bAlive;	}
private:
	void	MyRegister( u_long uKey );
	void	Recharge( u_long uBlockSize );
	BOOL	Contribute( FLWSUser* pUser, DWORD dwPxpCount, DWORD dwPenya );

public:

public:
//	Constructions
	CDPCoreClient();
	virtual	~CDPCoreClient();
//	Override
	virtual	void	SysMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom );
	virtual	void	UserMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom );
//	Operations
	BOOL	Run( LPSTR lpszAddr, USHORT uPort, u_long uKey );
	BOOL	CheckIdStack( void );

	void	SendJoin( u_long idPlayer, const char* szPlayer, BOOL bOperator );
	void	SendSystemMessage( LPCTSTR lpsz );
//	Operator commands
	void	SendWhisper( u_long idFrom, u_long idTo, const CHAR* lpString );

	void	SendSay( u_long idFrom, u_long idTo, const CHAR* lpString );
	void	SendModifyMode( DWORD dwMode, BYTE fAdd, u_long idFrom, u_long idTo );
	void	SendShout( FLWSUser* pUser, const CHAR* lpString );
	void	SendPartyChat( FLWSUser* pUser, const CHAR* lpString );

	void	SendForceSetPartySkillTime( u_long uLeaderID, int nMode, DWORD dwSkillTime );
	void	SendUserPartySkill( u_long uidPlayer, int nMode, DWORD dwSkillTime, int nRemovePoint , int nCachMode );

	void	SendGMSay( u_long idPlayer, DWORD dwWorldID, const CHAR* lpString );
	void	SendPlayMusic( DWORD dwWorldID, u_long idmusic );
	void	SendPlaySound( DWORD dwWorldID, u_long idsound );
// #ifdef __LAYER_1015
// 	void	SendSummonPlayer( u_long idOperator, DWORD dwWorldID, const D3DXVECTOR3 & vPos, u_long idPlayer, int nLayer );
// #else	// __LAYER_1015
// 	void	SendSummonPlayer( u_long idOperator, DWORD dwWorldID, const D3DXVECTOR3 & vPos, u_long idPlayer );
// #endif	// __LAYER_1015
//	void	SendTeleportPlayer( u_long idOperator, u_long idPlayer );
	void	SendKillPlayer( u_long idOperator, u_long idPlayer );
	void	SendGetPlayerAddr( u_long idOperator, u_long idPlayer );
	void	SendGetPlayerCount( u_long idOperator );
	void	SendGetCorePlayer( u_long idOperator );
	void	SendSystem( const CHAR* lpString );
	void	SendToServer( LPBYTE lpBuffer, u_long uBufSize, DPID dpidCache, DPID dpidUser );
	
	void	SendCaption( const CHAR* lpString, DWORD dwWorldId = 0, BOOL bSmall = FALSE, DWORD dwPartyId = 0 );
		
	void	SendEventRealItem( u_long uIdPlayer, int nRealItemIndex, int nRealItemCount );

	void	SendPartyLevel( FLWSUser* pUser, DWORD dwLevel, DWORD dwPoint, DWORD dwExp );
	void	SendPartyMemberFlightLevel( FLWSUser* pUser );
	void	SendAddPartyExp( u_long uPartyId, int nMonLv, BOOL bSuperLeader , BOOL bLeaderSMExpUp );

	void	SendRemovePartyPoint( u_long uPartyId, int nRemovePoint );
	void	SendGameRate( FLOAT fRate, BYTE nFlag );
	void	SendLoadConstant();
	void	SendSetMonsterRespawn( u_long uidPlayer, DWORD dwMonsterID, DWORD dwRespawnNum, DWORD dwAttackNum, DWORD dwRect, DWORD dwRespawnTime, BOOL bFlying );

	void	SendSeasonEffectByGM( const bool effect );

//	void	SendSetPointParam( CMover* pMover, int nParam, int nVal );
	void	SendGuildCombatState( int nState );
	void	SendGCRemoveParty( u_long uidPartyid, u_long uidPlayer );
// 	void	SendGCAddParty( u_long idLeader, LONG nLeaderLevel, LONG nLeaderJob, DWORD dwLSex, 
// 							u_long idMember, LONG nMemberLevel, LONG nMemberJob, DWORD dwMSex );
	void	OnCWWantedList( CAr & ar, DPID, DPID, OBJID );
	void	OnCWWantedReward( CAr & ar, DPID, DPID, OBJID );
	void	SendSetPartyDuel( u_long idParty1, u_long idParty2, BOOL bDuel );
	void	SendCreateGuild( GUILD_MEMBER_INFO* info, int nSize, const char* szGuild );
	void	SendGuildChat( FLWSUser* pUser, const char* sChat );
	BOOL	SendGuildStat(  FLWSUser* pUser, GUILD_STAT stat, DWORD data );
	void	SendGuildGetPay( u_long uGuildId, DWORD nGoldGuild );
	void	SendGuildMsgControl_Bank_Item( FLWSUser* pUser, FLItemElem* pItemElem, BYTE p_Mode );
	void	SendGuildMsgControl_Bank_Penya( FLWSUser* pUser, DWORD p_Penya, BYTE p_Mode, BYTE cbCloak );
	void	SendWarDead( u_long idPlayer );
	void	SendWarMasterAbsent( u_long idWar, BOOL bDecl );
	void	SendWarTimeout( u_long idWar );
	void	OnWarDead( CAr & ar, DPID, DPID, OBJID );
	void	OnWarEnd( CAr & ar, DPID, DPID, OBJID );
	void	SendAddFriendNameReqest( u_long uLeaderid, LONG nLeaderJob, BYTE nLeaderSex, u_long uMember, const char * szLeaderName, const char * szMemberName );
	void	SendBlock( BYTE nGu, u_long uidPlayerTo, char *szNameTo, u_long uidPlayerFrom );
	void	SendWCWantedGold( LPCTSTR szPlayer, u_long idPlayer, int nGold, LPCTSTR szMsg );
#ifdef __LAYER_1015
	void	SendWCWantedReward( u_long idPlayer, u_long idAttacker, LPCTSTR szFormat, DWORD dwWorldID, const D3DXVECTOR3& vPos, int nLayer );
#else	// __LAYER_1015
	void	SendWCWantedReward( u_long idPlayer, u_long idAttacker, LPCTSTR szFormat, DWORD dwWorldID, const D3DXVECTOR3& vPos );
#endif	// __LAYER_1015
	void	SendQuerySetGuildName( u_long idPlayer, u_long idGuild, const char* lpszGuild, BYTE nId );
	void	SendSetSnoop( u_long idPlayer, u_long idSnoop, BOOL bRelease  );
	void	OnSetSnoop( CAr & ar, DPID, DPID, OBJID );
	void	SendSetSnoopGuild( u_long idGuild, BOOL bRelease );
	void	SendChat( u_long idPlayer1, u_long idPlayer2, const char* lpszChat );
	void	SendPing( void );
	void	OnDestroyPlayer( CAr & ar, DPID, DPID, OBJID );
private:
	void	OnInstanceDungeonAllInfo( CAr & ar, DPID, DPID, OBJID );
	void	OnInstanceDungeonCreate( CAr & ar, DPID, DPID, OBJID );
	void	OnInstanceDungeonDestroy( CAr & ar, DPID, DPID, OBJID );
	void	OnInstanceDungeonSetCoolTimeInfo( CAr & ar, DPID, DPID, OBJID );
	void	OnInstanceDungeonResetCoolTimeInfo( CAr & ar, DPID, DPID, OBJID );
	void	OnInstanceDungeonDeleteCoolTimeInfo( CAr & ar, DPID, DPID, OBJID );
public:
	void	SendInstanceDungeonCreate( int nType, DWORD dwDungeonId, INDUN_INFO & ID_Info, const DWORD dwPlayerID );
	void	SendInstanceDungeonDestroy( int nType, DWORD dwDungeonId, INDUN_INFO & ID_Info );
	void	SendInstanceDungeonSetCoolTimeInfo( int nType, DWORD dwPlayerId, COOLTIME_INFO & CT_Info );
	void	SendInstanceDungeonResetCoolTimeInfo( const int nType, const DWORD dwWorldId, const DWORD dwDungeonId );

public:
	void	SendQuizSystemMessage( int nDefinedTextId, BOOL bAll = FALSE, int nChannel = 0, int nTime = 0 );
	void	SendRemovePartyMember( u_long LeaderId, u_long MemberId );

	void	SendPacket( const FLPacket* pPacket );

private:
	void	OnBuyingInfo( CAr & ar, DPID, DPID, OBJID );

	void	OnSetPartyEffectModeNoti( CAr & ar, DPID, DPID, OBJID );
	void	OnRefreshPartyEffectModeNoti( CAr & ar, DPID, DPID, OBJID );
	
	void	OnStartFlyffPieceBurningMode( CAr & ar, DPID, DPID, OBJID );
	void	OnEndFlyffPieceBurningMode( CAr & ar, DPID, DPID, OBJID );

protected:
	// Handlers
	USES_PFNENTRIES( CDPCoreClient, ( CAr & ar, DPID, DPID, OBJID objid ) );

	void	OnLoadWorld( CAr & ar, DPID, DPID, OBJID );
	void	OnQueryTickCount( CAr & ar, DPID, DPID, OBJID );
	void	OnRecharge( CAr & ar, DPID, DPID, OBJID );
	void	OnModifyMode( CAr & ar, DPID, DPID, OBJID objid );

	void	OnSetPartyExp( CAr & ar, DPID, DPID, OBJID objid );
	void	OnRemovePartyPoint( CAr & ar, DPID, DPID, OBJID objid );
	void	OnPartyChangeTroup( CAr & ar, DPID, DPID, OBJID );
	void	OnPartyChangeName( CAr & ar, DPID, DPID, OBJID );
	void	OnAddFriend( CAr & ar, DPID, DPID, OBJID );
	void	OnRemovefriend( CAr & ar, DPID, DPID, OBJID );
	void	OnShout( CAr & ar, DPID, DPID, OBJID );
	void	OnPlayMusic( CAr & ar, DPID, DPID, OBJID );
	void	OnPlaySound( CAr & ar, DPID, DPID, OBJID );
	void	OnErrorParty( CAr & ar, DPID, DPID, OBJID );
	void	OnAddPartyMember( CAr & ar, DPID, DPID, OBJID );
	void	OnRemovePartyMember( CAr & ar, DPID, DPID, OBJID );
	void	OnAddPlayerParty( CAr & ar, DPID, DPID, OBJID );
	void	OnRemovePlayerParty( CAr & ar, DPID, DPID, OBJID );
	void	OnGuildMemberLv( CAr & ar, DPID, DPID, OBJID );
	void	OnSetPartyMode( CAr & ar, DPID, DPID, OBJID );
	void	OnPartyChangeItemMode( CAr & ar, DPID, DPID, OBJID );
	void	OnPartyChangeExpMode( CAr & ar, DPID, DPID, OBJID );

	void	OnSeasonEffect( CAr & ar, DPID, DPID, OBJID );

	void	OnPartyChat( CAr & ar , DPID, DPID, OBJID );
	void	OnCreateGuild( CAr & ar, DPID, DPID, OBJID );
	void	OnDestroyGuild( CAr & ar, DPID, DPID, OBJID );
	void	OnAddGuildMember( CAr & ar, DPID, DPID, OBJID );
	void	OnRemoveGuildMember( CAr & ar, DPID, DPID, OBJID );
	void	OnGuildClass( CAr & ar, DPID, DPID, OBJID );
	void	OnGuildNickName( CAr & ar, DPID, DPID, OBJID );
	void	OnChgMaster( CAr & ar, DPID, DPID, OBJID );
	void	OnGuildMemberLogOut( CAr & ar, DPID, DPID, OBJID );
	void	OnGuildLogoACK( CAr & ar, DPID, DPID, OBJID );
	void	OnGuildContributionACK( CAr & ar, DPID, DPID, OBJID );
	void	OnGuildNoticeACk( CAr & ar, DPID, DPID, OBJID );
	void	OnAddVoteResultACk( CAr & ar, DPID, DPID, OBJID );
	void	OnModifyVote( CAr & ar, DPID, DPID, OBJID );

	void	OnGuildAuthority( CAr & ar, DPID, DPID, OBJID );
	void	OnGuildPenya( CAr & ar, DPID, DPID, OBJID );
	void	OnGuildRealPenya( CAr & ar, DPID, DPID, OBJID );
	void	OnGuildSetName( CAr & ar, DPID, DPID, OBJID );
	void	OnGuildMsgControl( CAr & ar, DPID, DPID, OBJID );
	void	OnAcptWar( CAr & ar, DPID, DPID, OBJID );
	void	OnSurrender( CAr & ar, DPID, DPID, OBJID );

	void	OnSetPointParam( CAr & ar, DPID, DPID, OBJID );
	void	OnFriendInterceptState( CAr & ar, DPID, DPID, OBJID );
	void	OnSetFriendState( CAr & ar, DPID, DPID, OBJID );
	void	OnPartyChangeLeader( CAr & ar, DPID, DPID, OBJID );

	void	OnLoadConstant( CAr & ar, DPID, DPID, OBJID );
	void	OnGameRate( CAr & ar, DPID, DPID, OBJID );
	void	OnSetMonsterRespawn( CAr & ar, DPID, DPID, OBJID );
	void	OnSetPlayerName( CAr & ar, DPID, DPID, OBJID );

	void	OnEvent( CAr & ar, DPID, DPID, OBJID );
	void	OnGuildCombatState( CAr & ar, DPID, DPID, OBJID );
	void	OnRemoveUserFromCORE( CAr & ar, DPID, DPID, OBJID );
	void	OnPing( CAr & ar, DPID, DPID, OBJID );

//private:
	//void	onStart_PeriodRepetitionAlarm();

#ifdef __M_USER_BOTS
	void	OnAddBot(CAr& ar, DPID, DPID, OBJID);
	void	OnRemoveBot(CAr& ar, DPID, DPID, OBJID);
#ifndef __M_USER_BOTS_DB_BASE
	void	SendAddBot(const u_long idBot, std::string& strName);
	void	SendRemoveBot(const u_long idBot, std::string& strName);
#endif
#endif
};

#endif	// __DPCORECLIENT_H__