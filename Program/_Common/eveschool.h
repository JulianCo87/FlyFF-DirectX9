#ifndef __EVE_SCHOOL_H__
#define	__EVE_SCHOOL_H__

#include "guild.h"

#define	MAX_SCHOOL	10

enum
{
	SBS_END, SBS_READY, SBS_START, SBS_START2,
};

typedef	struct	_SCHOOL_ENTRY
{
	u_long	id;
	char	lpName[MAX_G_NAME];
	int		nSurvivor;
	int		nSize;
	int		nLevel;
	int		nDead;
	_SCHOOL_ENTRY()
		{	
			id	= 0;
			lpName[0]	= '\0';
			nSurvivor	= 0;
			nSize	= 0;
			nLevel = 0;
			nDead	= 0;
		};
}
SCHOOL_ENTRY, *PSCHOOL_ENTRY;

class CGuildMng;

class	CEveSchool
{
public:
	CGuildMng*	m_pSchoolMng;
	D3DXVECTOR3	m_avPos[MAX_SCHOOL];
	D3DXVECTOR3	m_vDefault;
	DWORD	m_dwsbstart2;
public:
	CEveSchool();
	~CEveSchool()	{}

	BOOL	Ready( void );
	BOOL	Start( void );
	BOOL	Start2( void );
	BOOL	End( void );
	BOOL	Report( void );

	BOOL	LoadPos( LPCSTR lpszFileName );
	D3DXVECTOR3		GetPos( u_long id );
	D3DXVECTOR3		GetPos( void )	{	return m_vDefault;	}

	static	CEveSchool* GetInstance();
};


typedef struct __AUTO_OPEN
{
	BOOL bUseing;
	BYTE nHour;	
	BYTE nMinute;		
} __AUTO_OPEN;

class CGuildCombat
{
#ifdef __WORLDSERVER 	
	__AUTO_OPEN		__AutoOpen[7];
#endif // __WORLDSERVER
	
public:
	enum { OPEN_STATE, CLOSE_STATE, WAR_STATE, COMPLET_CLOSE_STATE, GM_COLSE_STATE };
	enum { NOTENTER_STATE = 100, NOTENTER_COUNT_STATE, ENTER_STATE, MAINTENANCE_STATE, WAR_WAR_STATE, WAR_CLOSE_STATE, WAR_CLOSE_WAIT_STATE, WAR_TELEPORT_STATE };
	enum { ALLMSG = 1000, GUILDMSG, JOINMSG, WORLDMSG, STATE, WARSTATE, WAIT };
	struct __REQUESTGUILD
	{
		u_long uidGuild;
		DWORD dwPenya;
	};
	struct __JOINPLAYER
	{
		u_long	uidPlayer;		// ĳ���� ���̵�
		int		nlife;			// ���� ����
//		BOOL	bEntry;			// ���� ��/��
		int		nPoint;			// ����Ʈ
		int		uKillidGuild;	// ���� ���� ���
		int		nMap;			// ���� �ڷ���Ʈ ��
		DWORD	dwTelTime;		// ���� �ڷ���Ʈ �ð�
		__JOINPLAYER()
		{
			uidPlayer	= 0;
			nlife	= 0;
			nPoint	= 0;
			uKillidGuild	= 0;
			nMap	= 0;
			dwTelTime	= 0;
		}
	};
	struct __GCSENDITEM
	{
		int nWinCount;
		DWORD dwItemId;
		int nItemNum;
		__GCSENDITEM()
		{
			nWinCount = 0;
			dwItemId = 0;
			nItemNum = 0;
		}
	};
	struct __GuildCombatMember
	{
		u_long	uGuildId;		//sun: 10, __S_BUG_GC

		vector<__JOINPLAYER*> vecGCSelectMember;	// ������ ���õ� ��� ����
//		set<u_long> GuildCombatJoinMember;		// ������� �����Ѹ��
//		set<u_long> GuildCombatOutMember;		// �����ߴµ� �׾����� ���� ���
//		FLOAT fAvgLevel;		// ��� ����
		DWORD dwPenya;			// ������
		BOOL  bRequest;			// ���� ��/��
		u_long	m_uidDefender;	// �����
		int	  nJoinCount;		// ���� ī����
		int	  nWarCount;		// ���� �ο���
		int   nGuildPoint;		// ��� ����Ʈ
		list<__JOINPLAYER*>	lspFifo;

		void Clear()
		{
			uGuildId = 0;		//sun: 10, __S_BUG_GC

			SelectMemberClear();
			dwPenya = 0;
			bRequest = FALSE;
			m_uidDefender = 0;
			nJoinCount = 0;
			nWarCount = 0;
			nGuildPoint = 0;
		};
		void SelectMemberClear()
		{
			for( int veci = 0 ; veci < (int)( vecGCSelectMember.size() ) ; ++veci )
			{
				__JOINPLAYER* pJoinPlayer = vecGCSelectMember.at( veci );
				SAFE_DELETE( pJoinPlayer );
			}
			vecGCSelectMember.clear();
			lspFifo.clear();
		}
	};
	struct __GuildCombatProcess
	{
		int		nState;
		DWORD	dwTime;
		DWORD	dwCommand;
		DWORD	dwParam;
	};
	struct __GCRESULTVALUEGUILD
	{
		int	nCombatID;			// ������ ���̵�
		u_long uidGuild;		// ��� ���̵�
		__int64 nReturnCombatFee;	// �������� ������
		__int64 nReward;				// �����
	};
	struct __GCRESULTVALUEPLAYER
	{
		int nCombatID;			// ������ ���̵�
		u_long uidGuild;		// ��� ���̵�
		u_long uidPlayer;		// �÷��̾� ���̵�
		__int64 nReward;			// �����
	};
	struct __GCGETPOINT
	{
		u_long uidGuildAttack;
		u_long uidGuildDefence;
		u_long uidPlayerAttack;
		u_long uidPlayerDefence;
		int nPoint;
		BOOL bKillDiffernceGuild;
		BOOL bMaster;
		BOOL bDefender;
		BOOL bLastLife;

		__GCGETPOINT()
		{
			uidGuildAttack = uidGuildDefence = uidPlayerAttack = uidPlayerDefence = nPoint = 0;
			bKillDiffernceGuild = bMaster = bDefender = bLastLife = FALSE;
		}
	};
	struct __GCPLAYERPOINT
	{
		u_long	uidPlayer;
		int		nJob;
		int		nPoint;

		__GCPLAYERPOINT()
		{
			uidPlayer = nJob = nPoint = 0;
		}
	};
	int		m_nGuildCombatIndex;
	u_long	m_uWinGuildId;
	int		m_nWinGuildCount;
	u_long	m_uBestPlayer;
	vector<__GCGETPOINT> m_vecGCGetPoint;
	vector<__GCPLAYERPOINT> m_vecGCPlayerPoint;
#ifdef __WORLDSERVER 

	vector<__GuildCombatMember*> m_vecGuildCombatMem;	//sun: 10, __S_BUG_GC

	DWORD	m_dwTime;
	int		m_nProcessGo;
	int	 	m_nProcessCount[ 25 ];
	__GuildCombatProcess GuildCombatProcess[250];
	int		m_nStopWar;	// 1�̸� �߰��� ����, 2�̸� ��ڰ� ����

	int		m_nJoinPanya;		// ������ �����Ҽ� �ִ� �⺻ Penya
	int		m_nGuildLevel;		// ������ �����Ҽ� �ִ� �ּ� ��巹��
//sun: 8, // __GUILDCOMBAT_85
	int		m_nMinGuild;		// �ּ� ������ �Ҽ� �ִ� ��� ����(�ּ� ���� ��� ������ �Ǿ� ��� ������ ������)
	int		m_nMaxGCSendItem;

	int		m_nMaxGuild;		// ������ �����Ҽ� �ִ� ���
	int		m_nMaxJoinMember;	// ������ �����Ҽ� �ִ� �ִ� ����
	int		m_nMaxPlayerLife;	// ������ ������ ������ �ִ� ����
	int		m_nMaxWarPlayer;	// �ִ� ���ߴ� ����
	int		m_nMaxMapTime;		// ���� ��ġ ���� �ð�
	int		m_nMaxGuildPercent;	// ������� ��� �ۼ�Ʈ
	int		m_nMaxPlayerPercent;// ����Ʈ �÷��̾� �ۼ�Ʈ 
	int		m_nRequestCanclePercent;	// ������û�� ����� ��忡�� ������ �ۼ�Ʈ
	int		m_nNotRequestPercent;		// ������ ������ �ȵȱ�忡�� ������ �ۼ�Ʈ
	int		m_nItemPenya;				// ���� ��ǰ ������ ����(�����?)
	BOOL    m_bMutex;					// ������ ���� �ѹ���...
	BOOL    m_bMutexMsg;					// ������ ���� �ѹ���...
	CTimer   m_ctrMutexOut;
//sun: 8, // __GUILDCOMBAT_85
	vector< CString > m_vecstrGuildMsg;
	vector<__GCSENDITEM>	vecGCSendItem;
	
	vector<__REQUESTGUILD>	vecRequestRanking;	// ���� ����
	vector<__GCRESULTVALUEGUILD>			m_GCResultValueGuild;		// ������ �����
	vector<__GCRESULTVALUEPLAYER>			m_GCResultValuePlayer;		// ������ �����
#endif // __WORLDSERVER
	int		m_nState;		// ���� ����
	int		m_nGCState;	// ���� ���� ���� ����
#ifdef __CLIENT
	BOOL	m_bRequest;	// ��û ����
	BOOL	IsRequest( void ) { return m_bRequest; };
#endif // __CLIENT
public:
	//	Constructions
	CGuildCombat();
	virtual ~CGuildCombat();
	void	GuildCombatClear( int Clear = 1 );
	void	GuildCombatGameClear();
	void	SelectPlayerClear( u_long uidGuild );
	void	AddvecGCGetPoint( u_long uidGuildAttack, u_long uidGuildDefence, u_long uidPlayerAttack, u_long uidPlayerDefence, int nPoint,
							BOOL bKillDiffernceGuild, BOOL bMaster, BOOL bDefender, BOOL bLastLife );
	void	AddvecGCPlayerPoint( u_long uidPlayer, int nJob, int nPoint );
#ifdef __WORLDSERVER
	BOOL	LoadScript( LPCSTR lpszFileName );
	void	JoinGuildCombat( u_long idGuild, DWORD dwPenya, BOOL bRequest );
	void	AddSelectPlayer( u_long idGuild, u_long uidPlayer );
	void	GetSelectPlayer( u_long idGuild, vector<__JOINPLAYER> &vecSelectPlayer );
	void	OutGuildCombat( u_long idGuild );
	void	SetMaintenance();
	void	SetEnter();
	void	SetGuildCombatStart();
	void	SetGuildCombatClose( BOOL bGM = FALSE );
	void	SetGuildCombatCloseWait( BOOL bGM = FALSE );
	void	GuildCombatCloseTeleport();

	void	SetNpc( void );
	void	SetRequestRanking( void );
	void	SetDefender( u_long uidGuild, u_long uidDefender );
	void	SetPlayerChange( FLWSUser* pUser, FLWSUser* pLeader );
	u_long	GetDefender( u_long uidGuild );
	u_long	GetBestPlayer( u_long* dwGetGuildId, int* nGetPoint );
	DWORD	GetRequstPenya( u_long uidGuild );
	void	GetPoint( FLWSUser* pAttacker, FLWSUser* pDefender );
	__int64	GetPrizePenya( int nFlag );
	BOOL	IsRequestWarGuild( u_long uidGuild, BOOL bAll );
	BOOL	IsSelectPlayer( FLWSUser* pUser );
	
	void	JoinWar( FLWSUser* pUser, int nMap = 99, BOOL bWar = TRUE );
	void	OutWar( FLWSUser* pUser, FLWSUser* pLeader, BOOL bLogOut = FALSE );
	void	JoinObserver( FLWSUser* pUser );
	void	GuildCombatRequest( FLWSUser* pUser, DWORD dwPenya );
	void	GuildCombatCancel( FLWSUser* pUser );
	void	GuildCombatOpen( void );
	void	GuildCombatEnter( FLWSUser* pUser );
	void	SetSelectMap( FLWSUser* pUser, int nMap );
	void	UserOutGuildCombatResult( FLWSUser* pUser );
	void	GuildCombatResult( BOOL nResult = FALSE, u_long idGuildWin = 0 );
	void	Process();
	void	ProcessCommand();
	void	ProcessJoinWar();
	void	SendJoinMsg( LPCTSTR lpszString );
	void	SendGuildCombatEnterTime( void );
	void	SendGCLog( void );
	void	SerializeGCWarPlayerList( CAr & ar );

	void	GuildCombatResultRanking();		//sun: 11, ������ Ĩ���� �� Ĩ�� ���� ���� �̿�

	CTime	GetNextGuildCobmatTime(void);
	int		m_nDay;

	__GuildCombatMember* FindGuildCombatMember( u_long GuildId );	//sun: 10, __S_BUG_GC

	bool	DeleteGuildCombatTender( const u_long idGuild );

//////////////////////////////////////////////////////////////////////////
// 20120716_MIRCHANG - ����� ���� ������ �ӽ� ���� �߰�
private:

	struct __GCREWARDITEM
	{
		int		m_dwRank;
		int		m_nWinCount;
		DWORD	m_dwRewardItemID;
		int		m_nItemNum;

		__GCREWARDITEM()
		{
			m_dwRank = 0;
			m_nWinCount = 0;
			m_dwRewardItemID = 0;
			m_nItemNum = -0;
		}
	};
public:
	vector<__GCREWARDITEM>	m_vecRewardItem;

	DWORD	m_dwRewardItemID;

	bool	CheckScriptData() const;
	void	SetRewardItemID( const DWORD dwRewardItemID )		{	m_dwRewardItemID	= dwRewardItemID;	}

public:
	DWORD	GetRewardItemID() const		{	return m_dwRewardItemID;	}
//////////////////////////////////////////////////////////////////////////

#endif // __WORLDSERVER
};

#endif	// __EVE_SCHOOL_H__