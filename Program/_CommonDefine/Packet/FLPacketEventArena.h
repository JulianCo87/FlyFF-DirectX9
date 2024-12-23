
#ifndef	__FLPACKETEVENTARENA_H__
#define	__FLPACKETEVENTARENA_H__


#include "FLPacket.h"


enum
{
	EVENT_ARENA_STATE_CONFIG,		// ���� �ܰ�
	EVENT_ARENA_STATE_CALL_WAIT,	// ���� ��ȯ ���
	EVENT_ARENA_STATE_CALL,			// ���� ��ȯ
	EVENT_ARENA_STATE_CALLED,		// ���� ��ȯ ��
	EVENT_ARENA_STATE_JOIN,			// ���� ����
	EVENT_ARENA_STATE_JOINED,		// ���� ���� ��
	EVENT_ARENA_STATE_COUNTDOWN,	// ī��Ʈ �ٿ�
	EVENT_ARENA_STATE_PLAYING,		// ���� ��
	EVENT_ARENA_STATE_RESULT,		// ���� ���
	EVENT_ARENA_STATE_END,			// ����
};


enum
{
	MAX_EVENT_ARENA_WIN_COUNT		= 7,
	MAX_EVENT_ARENA_TEAM_NAME		= 64,
	MAX_EVENT_ARENA_TONEMENT_NAME	= 128,
	MAX_EVENT_ARENA_TONEMENT_DESC	= 128,
	MAX_EVENT_ARENA_GAME_RESULT_CODE	= 16,
	MAX_EVENT_ARENA_GAME_RESULT_REASON	= 16,
};


enum
{
	EVENT_ARENA_TEAM_MEMBER_STATE_LOGOUT,		// �α׾ƿ� ����
	EVENT_ARENA_TEAM_MEMBER_STATE_LOGIN,		// �α��� ����(��������)
	EVENT_ARENA_TEAM_MEMBER_STATE_BATTLE_WAIT,	// ���� ���
	EVENT_ARENA_TEAM_MEMBER_STATE_BATTLE,		// ���� ����
};

enum
{
	EVENT_ARENA_GAME_STATE_NONE,	// 
	EVENT_ARENA_GAME_STATE_CALL,	// ���� ��ȯ ��
	EVENT_ARENA_GAME_STATE_JOIN,	// ���� ���� ��
	EVENT_ARENA_GAME_STATE_PLAYING,	// ��� ��
};

#pragma pack( 1 )

struct	T_EVENT_ARENA_TEAM_INFO
{
	T_EVENT_ARENA_TEAM_INFO() : dwID( 0 )	{ szName[ 0 ] = '\0'; }

	DWORD	dwID;
	TCHAR	szName[MAX_EVENT_ARENA_TEAM_NAME];
};


struct	T_EVENT_ARENA_TEAM_MEMBER_INFO
{
	OBJID	dwObjID;				// ĳ���� ������Ʈ ���̵�
	BYTE	byPartyNumber;			// ��Ƽ �ѹ�
	TCHAR	szCharName[MAX_NAME];	// ĳ���� �̸�

	int		nState;					// ����

	int		nCurrentHP;				// ���� HP
	int		nMaxHP;					// �ִ� HP
};

// ���� ����
struct	T_EVENT_ARENA_GAME_INFO
{
	int		nState;				// ���� ����

	DWORD	dwTonementID;		// ��ʸ�Ʈ ���̵�
	DWORD	dwRedTeamID;		// ������ ���̵�
	DWORD	dwBlueTeamID;		// ����� ���̵�

	BYTE	byRedWinFlag[MAX_EVENT_ARENA_WIN_COUNT];	// ������ �¸� �÷���
	BYTE	byBlueWinFlag[MAX_EVENT_ARENA_WIN_COUNT];	// ����� �¸� �÷���

	DWORD	dwRemainBattleTime;	// �ܿ� �����ð�(1000�� 1��)
};


struct	T_EVENT_ARENA_TONEMENT
{
	//T_EVENT_ARENA_TONEMENT() : dwID( 0 ), nNeedWinCount( 0 ) {		szName[ 0 ] = '\0';		szDesc[ 0 ] = '\0'; }

	DWORD	dwID;									// ��ʸ�Ʈ ���̵�
	TCHAR	szName[MAX_EVENT_ARENA_TONEMENT_NAME];	// ��ʸ�Ʈ �̸�
	int		nNeedWinCount;							// �ʿ� �¼�
	TCHAR	szDesc[MAX_EVENT_ARENA_TONEMENT_DESC];	// ����
};

struct	T_BATTLE_RESULT_LOG
{
	int			nBattleNumber;								// ���� �ѹ�
	int			nWinTeam;									// �¸��� �ε���(Red, Blue ��)
	TCHAR		szWinTeamName[MAX_EVENT_ARENA_TEAM_NAME];	// �¸��� �ε���
	ULONGLONG	u64BattleTime;								// �����ð�
	int			nRedSurvivalMembers;						// ������ ������ ��
	int			nBlueSurvivalMembers;						// ����� ������ ��
};

struct	T_GAME_RESULT_INFO
{
	T_EVENT_ARENA_TONEMENT	tTonement;				// ��ʸ�Ʈ ����
	T_EVENT_ARENA_TEAM_INFO	tRedTeam;				// ������ ����
	T_EVENT_ARENA_TEAM_INFO	tBlueTeam;				// ����� ����

	TCHAR					szCode[MAX_EVENT_ARENA_GAME_RESULT_CODE];			// �α� �ڵ�(��� ����(GameEnd), ��� �ߴ�(GameStop))
	TCHAR					szStopReason[MAX_EVENT_ARENA_GAME_RESULT_REASON];	// ��� �ߴ� �߰� ����(��ɾ� �ߴ�(ChatCommand, UICommand), ��� �α׾ƿ�(Logout))

	SYSTEMTIME				tGameStart;				// ���� ���� �ð�
	SYSTEMTIME				tGameEnd;				// ���� ���� �ð�(�α� �߻� �ð�)

	int						nWinTeam;				// �¸���
	TCHAR					szWinTeam[MAX_EVENT_ARENA_TEAM_NAME];				// �¸��� �̸�

	int						nRedWinCount;			// ������ �¼�
	int						nBlueWinCount;			// ����� �¼�
};

#pragma pack()


class	FLEventArenaTeamStream
{
public:

	FLEventArenaTeamStream();

	T_EVENT_ARENA_TEAM_INFO							tTeamInfo;		// �� ����
	std::vector< T_EVENT_ARENA_TEAM_MEMBER_INFO >	vecMembers;		// �ɹ� ����Ʈ

	bool	Serialize( CAr& kAr )	const;
	bool	Deserialize( CAr& kAr );

};

class	FLEventArenaBattleResultInfo
{
public:

	FLEventArenaBattleResultInfo();

	T_EVENT_ARENA_TONEMENT	tTonement;				// ��ʸ�Ʈ ����
	T_EVENT_ARENA_TEAM_INFO	tRedTeam;				// ������ ����
	T_EVENT_ARENA_TEAM_INFO	tBlueTeam;				// ����� ����

	SYSTEMTIME				tBattleStart;			// ���� ���� �ð�
	SYSTEMTIME				tBattleEnd;				// ���� ���� �ð�(�α� �߻� �ð�)

	ULONGLONG				u64BattleTime;			// ���� �ð�

	int						nWinTeam;				// �¸���
	tstring					strWinTeam;				// �¸��� �̸�

	int						nRedWinCount;			// ������ �¼�
	int						nBlueWinCount;			// ����� �¼�

	std::vector< tstring >	vecRedSurvivalMembers;	// ������ ������ ���
	std::vector< tstring >	vecBlueSurvivalMembers;	// ����� ������ ���

	bool	Serialize( CAr& kAr )	const;
	bool	Deserialize( CAr& kAr );

};


//////////////////////////////////////////////////////////////////////////
// Server -> Client
//////////////////////////////////////////////////////////////////////////

// =========================== ���� ���� ============================ //

// SNAPSHOTTYPE_EVENT_ARENA_TEAM_INFO_NOTI			// �� ���� �뺸
class	FLSnapshotEventArenaTeamInfoNoti : public FLPacket
{
public:

	FLSnapshotEventArenaTeamInfoNoti();

	FLEventArenaTeamStream	kTeamInfo;	// �� ����

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};


// SNAPSHOTTYPE_EVENT_ARENA_TEAM_MEMBER_UPDATE_NOTI	// ���� ���� ����(����� ������)
class	FLSnapshotEventArenaTeamMemberUpdateNoti : public FLPacket
{
public:

	FLSnapshotEventArenaTeamMemberUpdateNoti();

	std::vector< T_EVENT_ARENA_TEAM_MEMBER_INFO >	vecChangedMembers;		// ����� �ɹ����� ������

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};


// SNAPSHOTTYPE_EVENT_ARENA_CALL_PLAYER_NOTI		// ������ȯ��� �뺸
class	FLSnapshotEventArenaCallPlayerNoti : public FLPacket
{
public:

	FLSnapshotEventArenaCallPlayerNoti();

	DWORD	dwWaitTimeout;		// ��� �ð�(���� ��)

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};


// =========================== ���� ============================ //

// SNAPSHOTTYPE_EVENT_ARENA_GAME_JOIN_TEAM_INFO_NOTI	// ���ӿ� �����ϴ� ����, ����� ����
class	FLSnapshotEventArenaGameJoinTeamInfoNoti : public FLPacket
{
public:

	FLSnapshotEventArenaGameJoinTeamInfoNoti();

	std::vector< tstring >	vecRedTeamMembers;
	std::vector< tstring >	vecBlueTeamMembers;

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// SNAPSHOTTYPE_EVENT_ARENA_BATTLE_RESULT_NOTI		// ���� ��� �뺸
class	FLSnapshotEventArenaBattleResultNoti : public FLPacket
{
public:

	FLSnapshotEventArenaBattleResultNoti();

	int		nBattleWinTeam;							// �¸���

	TCHAR	szRedName[MAX_EVENT_ARENA_TEAM_NAME];	// ������ �̸�
	int		nRedWinCount;							// ������ �¼�

	TCHAR	szBlueName[MAX_EVENT_ARENA_TEAM_NAME];	// ����� �̸�
	int		nBlueWinCount;							// ����� �¼�

	DWORD	dwWinnerDisplayTime;					// ����ǥ�� �ð�(���� ��)

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// SNAPSHOTTYPE_EVENT_ARENA_GAME_RESULT_NOTI			// ���� ���(���� �¸�) �뺸
class	FLSnapshotEventArenaGameResultNoti : public FLPacket
{
public:

	FLSnapshotEventArenaGameResultNoti();

	int		nGameWinTeam;							// ���� �¸���

	TCHAR	szRedName[MAX_EVENT_ARENA_TEAM_NAME];	// ������ �̸�
	int		nRedWinCount;							// ������ �¼�

	TCHAR	szBlueName[MAX_EVENT_ARENA_TEAM_NAME];	// ����� �̸�
	int		nBlueWinCount;							// ����� �¼�

	DWORD	dwWinnerDisplayTime;					// ����ǥ�� �ð�(���� ��)

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};


//SNAPSHOTTYPE_EVENT_ARENA_GAME_SCOREBOARD_NOTI		// ���� ������ �뺸
class	FLSnapshotEventArenaGameScoreboardNoti : public FLPacket
{
public:

	FLSnapshotEventArenaGameScoreboardNoti();

	TCHAR	szTonementName[MAX_EVENT_ARENA_TONEMENT_NAME];	// ��ʸ�Ʈ �̸�
	TCHAR	szTonementDesc[MAX_EVENT_ARENA_TONEMENT_DESC];	// ����

	BYTE	byGameState;									// ����

	TCHAR	szRedTeamName[MAX_EVENT_ARENA_TEAM_NAME];		// ������ �̸�
	int		nRedWinCount;									// ������ �¼�
	int		nRedSurvivalMembers;							// ������ ������ ��

	TCHAR	szBlueTeamName[MAX_EVENT_ARENA_TEAM_NAME];		// ����� �̸�
	int		nBlueWinCount;									// ����� �¼�
	int		nBlueSurvivalMembers;							// ����� ������ ��

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};


// SNAPSHOTTYPE_EVENT_ARENA_BATTLE_RESULT_LOG_NOTI	// ���� ��� �α� �뺸
class	FLSnapshotEventArenaBattleResultLogNoti : public FLPacket
{
public:

	FLSnapshotEventArenaBattleResultLogNoti();

	std::vector< T_BATTLE_RESULT_LOG >	vecBattleResultLog;

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};


//////////////////////////////////////////////////////////////////////////
// Client -> Server
//////////////////////////////////////////////////////////////////////////

// =========================== ���� ���� ============================ //

// PACKETTYPE_EVENT_ARENA_MOVE_BATTLE_WAIT_NOTI	// NPC �̿� ������� ���� �̵� ��û
class	FLPacketEventArenaMoveBattleWaitNoti : public FLPacket
{
public:

	FLPacketEventArenaMoveBattleWaitNoti();

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};



// =========================== ������ ============================ //

//////////////////////////////////////////////////////////////////////////
// Server -> Client
//////////////////////////////////////////////////////////////////////////

// SNAPSHOTTYPE_EVENT_ARENA_MANAGE_WINDOW_OPEN_NOTI	// ����â ���� �뺸
class	FLSnapshotEventArenaManageWindowNoti : public FLPacket
{
public:

	FLSnapshotEventArenaManageWindowNoti();

	// ������ ������
	T_EVENT_ARENA_GAME_INFO					tGameInfo;		// ���� ���� ����

	// ����
	std::vector< T_EVENT_ARENA_TONEMENT >	vecTonements;	// ��ʸ�Ʈ ����Ʈ
	std::vector< T_EVENT_ARENA_TEAM_INFO >	vecTeams;		// �� ����Ʈ

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// SNAPSHOTTYPE_EVENT_ARENA_MANAGE_WINDOW_CLOSE_NOTI			// ����â �ݱ� �뺸
class	FLSnapshotEventArenaManageWindowCloseNoti : public FLPacket
{
public:

	FLSnapshotEventArenaManageWindowCloseNoti();

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};


// SNAPSHOTTYPE_EVENT_ARENA_MANAGE_GAME_INFO_SYNC_NOTI		// ���� ���� ���� ��û
class	FLSnapshotEventArenaManageGameInfoSyncNoti : public FLPacket
{
public:

	FLSnapshotEventArenaManageGameInfoSyncNoti();

	T_EVENT_ARENA_GAME_INFO	tGameInfo;		// ���� ���� ����

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};


// SNAPSHOTTYPE_EVENT_ARENA_MANAGE_WAIT_CALL_PLAYERS_NOTI		// ������ȯ ��� �뺸
class	FLSnapshotEventArenaManageWaitCallPlayersNoti : public FLPacket
{
public:

	FLSnapshotEventArenaManageWaitCallPlayersNoti();

	DWORD	dwWaitTimeout;	// ��� �ð�(���� ��)

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};


// SNAPSHOTTYPE_EVENT_ARENA_MANAGE_RED_BLUE_TEAM_INFO_NOTI	// ����, ��� �� ���� �뺸(�ֱ������� ����)
class	FLSnapshotEventArenaManageRedBlueTeamInfoNoti : public FLPacket
{
public:

	FLSnapshotEventArenaManageRedBlueTeamInfoNoti();

	FLEventArenaTeamStream	kRedTeamInfo;
	FLEventArenaTeamStream	kBlueTeamInfo;

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};


// SNAPSHOTTYPE_EVENT_ARENA_MANAGE_RED_BLUE_TEAM_INFO_CLOSE_NOTI	// ����, ��� �� ���� â �ݱ� �뺸
class	FLSnapshotEventArenaManageRedBlueTeamInfoCloseNoti : public FLPacket
{
public:

	FLSnapshotEventArenaManageRedBlueTeamInfoCloseNoti();

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};


// SNAPSHOTTYPE_EVENT_ARENA_MANAGE_WAIT_JOIN_PLAYERS_NOTI		// �������� ��� �뺸
class	FLSnapshotEventArenaManageWaitJoinPlayersNoti : public FLPacket
{
public:

	FLSnapshotEventArenaManageWaitJoinPlayersNoti();

	DWORD	dwWaitTimeout;	// ��� �ð�(���� ��)

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};


//////////////////////////////////////////////////////////////////////////
// Client -> Server
//////////////////////////////////////////////////////////////////////////

// PACKETTYPE_EVENT_ARENA_MANAGE_SET_TONEMENT_NOTI		// ��ʸ�Ʈ ���� ����
class	FLPacketEventArenaManageSetTonementNoti : public FLPacket
{
public:

	FLPacketEventArenaManageSetTonementNoti();

	DWORD	dwTonementID;		// ��ʸ�Ʈ ���̵�

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// PACKETTYPE_EVENT_ARENA_MANAGE_SET_RED_TEAM_NOTI		// ������ ���� ����
class	FLPacketEventArenaManageSetRedTeamNoti : public FLPacket
{
public:

	FLPacketEventArenaManageSetRedTeamNoti();

	DWORD	dwRedTeamID;		// ������ ���̵�

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// PACKETTYPE_EVENT_ARENA_MANAGE_SET_BLUE_TEAM_NOTI		// ����� ���� ����
class	FLPacketEventArenaManageSetBlueTeamNoti : public FLPacket
{
public:

	FLPacketEventArenaManageSetBlueTeamNoti();

	DWORD	dwBlueTeamID;		// ����� ���̵�

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// PACKETTYPE_EVENT_ARENA_MANAGE_CALL_PLAYERS_NOTI		// ������ȯ ����
class	FLPacketEventArenaManageCallPlayersNoti : public FLPacket
{
public:

	FLPacketEventArenaManageCallPlayersNoti();

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// PACKETTYPE_EVENT_ARENA_MANAGE_JOIN_PLAYERS_NOTI		// �������� ����
class	FLPacketEventArenaManageJoinPlayersNoti : public FLPacket
{
public:

	FLPacketEventArenaManageJoinPlayersNoti();

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// PACKETTYPE_EVENT_ARENA_MANAGE_BATTLE_START_NOTI		// ���� ���� ����
class	FLPacketEventArenaManageBattleStartNoti : public FLPacket
{
public:

	FLPacketEventArenaManageBattleStartNoti();

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// PACKETTYPE_EVENT_ARENA_MANAGE_GAME_END_NOTI			// ���� ���� ����
class	FLPacketEventArenaManageGameEndNoti : public FLPacket
{
public:

	FLPacketEventArenaManageGameEndNoti();

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};


// PACKETTYPE_EVENT_ARENA_MANAGE_GAME_STOP_NOTI			// ���� ���� ����
class	FLPacketEventArenaManageGameStopNoti : public FLPacket
{
public:

	FLPacketEventArenaManageGameStopNoti();

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};




#endif