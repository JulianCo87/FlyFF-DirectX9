// GuildCombat1to1.h: interface for the CGuildCombat1to1 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDCOMBAT1TO1_H__A65C69E2_6A0F_4E25_AD16_B4004BB3A844__INCLUDED_)
#define AFX_GUILDCOMBAT1TO1_H__A65C69E2_6A0F_4E25_AD16_B4004BB3A844__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//sun: 11, �ϴ��� ��� ����

#include "LuaBase.h"

//////////////////////////////////////////////////////////////////////
//// CGuildCombat1To1Mng /////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#ifdef __CLIENT
typedef struct __GC1TO1_PLAYER_INFO
{
	u_long m_uidPlayer;
	int m_nState;
}__GC1TO1_PLAYER_INFO;
#endif //__CLIENT

class CGuildCombat1to1;
class CGuildCombat1to1Mng
{
public:
	enum { GC1TO1_CLOSE, GC1TO1_OPEN, GC1TO1_ENTRANCE, GC1TO1_WAR };
#ifdef __CLIENT
	enum {	GC1TO1WAR_CLOSE=100, GC1TO1WAR_WAIT, GC1TO1WAR_WAR,	GC1TO1WAR_FINISH, GC1TO1WAR_CLOSEWAIT };
#endif // __CLIENT	
	enum { GC1TO1_PLAYER_READY=200, GC1TO1_PLAYER_WAR, GC1TO1_PLAYER_WIN, GC1TO1_PLAYER_LOSE, GC1TO1_PLAYER_DRAW };

	
	CGuildCombat1to1Mng();
	virtual ~CGuildCombat1to1Mng();

	int m_nState;				// ���� state

	int	m_nJoinPenya;			// �ּ� ���� ���
	int	m_nMaxJoinPlayer;		// �ִ� ������ ��
	int	m_nMinJoinPlayer;		// �ּ� ������ ��
	int	m_nMinJoinPlayerLevel;	// �ּ� ���� ����

	int m_nWaitTime;
	DWORD m_dwTime;

	void	ClearTime();
#if defined(__WORLDSERVER) || defined(__DBSERVER)
	CLuaBase	m_Lua;
	BOOL	LoadScript();
	
	int	m_nGuildLevel;			// �ּ� ���� ��� ����
	int	m_nPlayerLife;			// ���� ������
	int	m_nMaxJoinGuild;		// �ִ� ���� ��� ��
	int	m_nMinJoinGuild;		// �ּ� ���� ��� ��
	int	m_nCancelReturnRate;	// ���� ��ҽ� ��ȯ��(%)
	int	m_nFailReturnRate;		// ���� ���н� ��ȯ��(%)

	struct __GC1TO1TENDER
	{
		u_long	ulGuildId;
		int		nPenya;
		int		nStageId;
		int		nPosition;
		DWORD	dwWorldId;
		int		nWinCount;
		int		nNowPlayerLife;
		int		nLastWinState;
		vector<u_long> vecMemberId;
		__GC1TO1TENDER()
		{
			nNowPlayerLife = 0;
			nWinCount = 0;
			nPosition = 0;
			nLastWinState = GC1TO1_PLAYER_DRAW;
			ulGuildId = NULL_ID;
			nPenya = 0;
			nStageId = NULL_ID;
			dwWorldId = NULL_ID;
		}
	};

//////////////////////////////////////////////////////////////////////////
// 20120716_MIRCHANG - ����� ���� ������ �ӽ� ���� �߰�
private:
	DWORD	m_dwRewardItemID;

	void	SetRewardItemID( const DWORD dwRewardItemID )	{	m_dwRewardItemID	=	dwRewardItemID;		}

public:
	DWORD	GetRewardItemID() const		{	return m_dwRewardItemID;	}
//////////////////////////////////////////////////////////////////////////
#endif // __WORLDSERVER || __DBSERVER

#ifdef __DBSERVER
	int		CheckOpenTIme();
	int		m_nCombatId;
#endif // __DBSERVER

#ifdef __CLIENT
	vector < __GC1TO1_PLAYER_INFO > m_vecGuildCombat1to1_Players;		// �ϴ��� �������� �����ϴ� ���� ����Ʈ
	u_long m_nGuildCombat1to1Guild;										// �ϴ��� �������� ��� ���
	int m_nMyGuildCount;
	int m_nVsGuildCount;

	void ClearGuildCombat1to1Players() { m_vecGuildCombat1to1_Players.clear(); }
	void SetGuildCombat1to1Guild(u_long nGuildId);
	void AddGuildCombat1to1Player(u_long nPlayerId);
	void SetGuildCombat1to1State(int nState, u_long nPlayer, int nMyGuildPoint=0, int nVsGuildPoint=0);
	BOOL IsPossibleMover( CMover* pMover );
#endif //__CLIENT

#ifdef __WORLDSERVER
	int		GetTenderGuildIndexByUser( FLWSUser* pUser );
	int		GetTargetTenderGuildIndexByUser( FLWSUser* pUser );
	BOOL	IsGuildMaster( FLWSUser* pUser );
	BOOL	IsGCTenderGuild( FLWSUser* pUser );
	BOOL	IsLineUpMember( FLWSUser* pUser );
	BOOL	IsPossibleUser( FLWSUser* pUser );
		
	void	Process();
	void	SendNowState( FLWSUser* pUser );

	void	GuildCombat1to1Open( BOOL bGMText = FALSE );
	void	GuildCombat1to1AllClose();
	void	CreateGuildCombat1to1();

	void	SendTenderGuildOpenWnd( FLWSUser* pUser );
	void	SendTenderGuildView( FLWSUser* pUser );
	void	SetTenderGuild( FLWSUser* pUser, int nPenya );
	void	SetCancelTenderGuild( FLWSUser* pUser );
	void	SetFailedTenderGuild( FLWSUser* pUser );

	void	SendMemberLineUpOpenWnd( FLWSUser* pUser );
	void	SetMemberLineUp( FLWSUser* pUser, vector<u_long>& vecMemberId );

	void	GuildCombat1to1EntranceWait();
	void	GuildCombat1to1War();
	void	SetTeleportToNPC( FLWSUser* pUser );
	void	SetTeleportToStage( FLWSUser* pUser );
	
	vector<CGuildCombat1to1>	m_vecGuilCombat1to1;	
	vector<__GC1TO1TENDER>		m_vecTenderGuild;		// ���� ���
	vector<__GC1TO1TENDER>		m_vecTenderFailGuild;	// ���� ���� ���

#endif // __WORLDSERVER
};




//////////////////////////////////////////////////////////////////////
//// CGuildCombat1To1Mng /////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#ifdef __WORLDSERVER
class CGuildCombat1to1  
{
public:
	// ����� CGuildCombat1to1Mng Class�� ���� ���� �����Ͽ��� �Ѵ�.
	enum {	GC1TO1WAR_CLOSE=100, GC1TO1WAR_WAIT, GC1TO1WAR_WAR,	GC1TO1WAR_FINISH, GC1TO1WAR_CLOSEWAIT };

	CGuildCombat1to1( vector<CGuildCombat1to1Mng::__GC1TO1TENDER>& vecTenderGuild, int nStageId );
	virtual ~CGuildCombat1to1();

	void	ClearTime();
	void	Process();
	void	SendNowState( FLWSUser* pUser );
	void	SendNowStateAllPlayer();
	void	SendWarResultAllPlayer();
	BOOL	IsClosed() { return m_nState == GC1TO1WAR_CLOSE; }
	BOOL	IsPossibleUser( FLWSUser* pUser );
	int		GetChipNum();
	void	SetWinChip( FLWSUser* pUserWin, FLWSUser* pUserLost, BOOL bDraw = FALSE, BOOL bDiag = TRUE );

	void	GuildCombat1to1CloseWait();
	void	GuildCombat1to1Close();
	void	GuildCombat1to1WarWait();
	void	GuildCombat1to1War();
	void	GuildCombat1to1WarResultCheck( BOOL bLogOut=FALSE, FLWSUser* pUserLogOut=NULL );
	void	SetLost( FLWSUser* pUserDef );
	void	SetLastWinState( int nIndex );
	
	void	SetTeleportToWaitStage( FLWSUser* pUser );
	void	SetTeleportToWarStage( FLWSUser* pUser );
	void	SetTeleportToWaitStageNowPlayers();

	vector<CGuildCombat1to1Mng::__GC1TO1TENDER>	m_vecTenderGuild;

	int		m_nState;
	int		m_nWaitTime;
	int		m_dwTime;
	int		m_nProgWarCount;
	int		m_nTotalWarCount;
//	int		m_nRealWarCount;
	int		m_nIndex[2];
};
#endif // __WORLDSERVER

#endif // !defined(AFX_GUILDCOMBAT1TO1_H__A65C69E2_6A0F_4E25_AD16_B4004BB3A844__INCLUDED_)