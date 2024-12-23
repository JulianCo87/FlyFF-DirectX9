#ifndef __DPDATABASECLIENT_H__
#define	__DPDATABASECLIENT_H__

#include <network/FLDPMng.h>
#include <msghdr.h>
#include <misc.h>
#include <guild.h>


class CDPDatabaseClient : public CDPMng
{
public:
//	Constructions
	CDPDatabaseClient();
	virtual	~CDPDatabaseClient();
//	Operations
	virtual void	SysMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID dpId );
	virtual void	UserMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID dpId );

	void	SendCreateGuild( GUILD_MEMBER_INFO* info, int nSize, u_long idGuild, const char* szGuild );
	void	SendDestroyGuild( u_long idGuild, u_long idMaster );
	void	SendAddGuildMember( u_long idPlayer, u_long idGuild, u_long DoidPlayer );
	void	SendRemoveGuildMember( u_long idPlayer, u_long idGuild, u_long DoidPlayer );
	void	SendGuildMemberLv( u_long idPlayer, int nMemberLv );
	void	SendGuildClass( u_long idPlayer, int nClass );
	void	SendGuildNickName( u_long idPlayer, LPCTSTR strNickName );
	void	SendChgMaster( u_long idPlayer, u_long idPlayer2 );
	void	SendGuildLogo( u_long idGuild, DWORD dwLogo );
	void	SendGuildContribution( CONTRIBUTION_CHANGED_INFO & info );
	void	SendGuildNotice( u_long idGuild, LPCTSTR szNotice );
	void	SendGuildAuthority( u_long idGuild, DWORD adwAuthority[] );
	void	SendGuildPenya( u_long idGuild, DWORD adwPenya[] );
	void	SendGuildSetName( u_long uidGuild, char * szName );

	void	OnAddVoteResult( CAr & ar );

	void	SendAddVote( u_long idGuild, const char* szTitle, 
		                 const char* szQuestion, char szSelections[4][MAX_BYTE_VOTESELECT] );
	void	SendRemoveVote( u_long idVote );
	void	SendCloseVote( u_long idVote );
	void	SendCastVote( u_long idVote, BYTE cbSelection );

	void	SendAcptWar( u_long idWar, u_long idDecl, u_long idAcpt );
	void	SendWarEnd( u_long idWar, u_long idDecl, u_long idAcpt, int nWptDecl, int nWptAcpt, int nType, CTime Time, int nWinPointDecl, int nWinPointAcpt, int nGetPointDecl, int nGetPointAcpt );
	void	SendSurrender( u_long idWar, u_long idPlayer, u_long idGuild );
	void	SendWarDead( u_long idWar, u_long idGuild );
	void	SendWarMasterAbsent( u_long idWar, u_long idGuild );
	void	SendSnoopGuild( u_long idGuild, u_long idPlayer, const char* lpszChat );
	void	OnUpdateGuildRankFinish( CAr & ar );

	void	QueryAddMessenger( u_long idPlayer, u_long idFriend );
	void	QueryDeleteMessenger( u_long idPlayer, u_long idFriend );
	void	QueryUpdateMessenger( u_long idPlayer, u_long idFriend, BOOL bBlock );
	void	OnRemovePlayerFriend( CAr & ar );

	void	OnCloseExistingConnection( CAr & ar );

	void	SendAddPartyName( u_long uidPlayer, const char * sParty );
	void	SendTag( u_long idFrom, u_long idTo, LPCTSTR lpszString );
	void	SendCTWanted( BYTE byReqType, u_long idPlayer, int nGold, long nEnd, LPCTSTR szMsg );

	void	SendBuyingInfo( PBUYING_INFO2 pbi2 );

	USES_PFNENTRIES( CDPDatabaseClient, ( CAr & ar ) );
	void	OnPartyName( CAr & ar );
	void	OnGlobalData( CAr & ar );
	void	OnInsertTagResult( CAr & ar );
	void	OnDelPlayer( CAr & ar );
	void	OnGuildMemberTime( CAr & ar );
	void	OnBuyingInfo( CAr & ar );
	void	OnTCList( CAr & ar );
	void	OnSetPlayerName( CAr & ar );
	void	OnUpdatePlayerData( CAr & ar );

	void	OnEventNotice( CAr & ar );
#ifdef __M_USER_BOTS_DB_BASE
	void onAddBot(CAr& ar);
	void onRemoveBot(CAr& ar);
public:
	void	sendServerConnected(const u_long uKey);
	void	sendServerDisconnected(const u_long uKey);
#endif
};


#endif	// __DPDATABASECLIENT_H__