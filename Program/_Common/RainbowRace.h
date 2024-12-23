#pragma once

#include "MiniGameBase.h"

enum {	RMG_GAWIBAWIBO = 0, RMG_DICEPLAY, RMG_ARITHMATIC, RMG_STOPWATCH,
		RMG_TYPING, RMG_PAIRGAME, RMG_LADDER,
		RMG_MAX };

#ifdef __WORLDSERVER
#include "FLRainbowRaceData.h"
#endif // __WORLDSERVER

//////////////////////////////////////////////////////////////////
// CRainbowRace //////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
class CRainbowRace
{
public:
	CRainbowRace();
	~CRainbowRace();
	void	DestroyGame();

	void	Serialize( CAr & ar );
	BOOL	IsCompletedGame( int nGameNum );	// nGameNum(RMG_...) ������ Ŭ���� �ߴ°�?
	BOOL	IsAllCompleted() { return ( m_wGameState == (0x7fff >> (15 - RMG_MAX)) ); } // ��� �̴ϰ����� Ŭ���� �ߴ°�?
	
	WORD	m_wGameState;		// �ش� �̴ϰ����� �ϷῩ��(�� ��Ʈ)
	WORD	m_wNowGame;			// ���� �������� �̴ϰ���(�ش� ��Ʈ)
#ifdef __CLIENT
	static CRainbowRace* GetInstance();
	int		GetGameKey();
	WORD	GetGameKeyToNowGame( int nGameKey ) {	return (0x0001 << nGameKey); }
	void	SendMinigamePacket( int nGameKey = RMG_MAX, int nState = MP_NONE, int nParam1 = 0, int nParam2 = 0 );
	void	SendMinigameExtPacket( vector<string> &vecszData, int nGameKey = RMG_MAX, int nState = MP_NONE, int nParam1 = 0, int nParam2 = 0 );

	DWORD	m_dwRemainTime;
	BOOL	m_bRRFinishMsg;
	BOOL	m_bCheckEnd;
#endif // __CLIENT
#ifdef __WORLDSERVER
	void	SetNowGameComplete( FLWSUser* pUser );	// �̹� ������ �Ϸ���·� ����� ���� ������ �����Ѵ�.
	void	SetNextMiniGame();	// ���� ���� ����...
	void	OnMiniGamePacket( FLWSUser* pUser, __MINIGAME_PACKET* pMiniGamePacket );

	void	SetFinish()	{ m_bFinished = TRUE; m_wGameState = 0x0000; }		// ���� ����
	BOOL	IsFinished() { return m_bFinished; }	// �̹� ������ ���ΰ�..
private:
	int		GetCompletedGameNum();		// ������� �Ϸ��� �̴ϰ��� ����
	void	SetMiniGame( int nGameNum );
	CMiniGameBase*	m_pMiniGame;		// ���� �������� �̴ϰ��� ������

	BOOL	m_bFinished;		// ���κ��� ���̽� �����ߴ°�..
#endif // __WORLDSERVER
};


//////////////////////////////////////////////////////////////////
// CRainbowRaceMng ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
#ifdef __WORLDSERVER
typedef map<DWORD, CRainbowRace*> MAPRR;

class CRainbowRaceMng
{
public:
	enum {	RR_CLOSED = 0, RR_APPLICATION, RR_OPEN, RR_WAIT, RR_READY, RR_PROGRESS };

	CRainbowRaceMng(void);
	~CRainbowRaceMng(void);

	BOOL Init();
	void Clear();
	
	static CRainbowRaceMng* GetInstance();
	
//	BOOL	LoadScript();
	int		GetMiniGameSize()			{ return m_vecMiniGame.size(); }	// �̴ϰ��� ����
	void	SetState( int nState )	{ m_nState = nState; }				// State ����
	int		GetState()				{ return m_nState; }				// ���� State

	void	Process();
	void	SetNextTime( DWORD dwTick )		{ m_dwNextTime = dwTick; }			// ���� �ܰ� �ð� ����
	CMiniGameBase* GetMiniGame( int nGameNum ) { return m_vecMiniGame[nGameNum]; }

	CRainbowRace*	GetRainbowRacerPtr( DWORD dwPlayerId );	// �ش� �÷��̾��� ���̽� ������ ���� �����͸� ��´�.
	BOOL			IsEntry( DWORD dwPlayerId );	// �ش� �÷��̾ �������� �����ΰ�?(��ũ��, ��ų, ������ ��������...)
	BOOL			IsApplicant( DWORD dwPlayerId ) { return GetRainbowRacerPtr( dwPlayerId ) ? TRUE : FALSE; }		// ��û�� �����ΰ�?
	int				GetApplicantNum() { return m_mapRainbowRace.size(); }	// ������ �ο� ��
	void			SetApplicationUser( FLWSUser* pUser );		// �÷��̾ ��û�� ���..
	void			SetApplication( DWORD dwPlayerId );		// ��û ���
	BOOL			SetDropOut( DWORD dwPlayerId );			// Ż��
	DWORD			GetNextTime()				{ return m_dwNextTime; }			// ���� �ܰ�� �Ѿ�� �ð�
	void			OnMiniGamePacket( FLWSUser* pUser, __MINIGAME_PACKET* pMiniGamePacket );	// �̴ϰ��� ��Ŷ�� ������ ó��
	void			SetRanking( FLWSUser* pUser );	// ���̽��� ������ ĳ���͸� ������ ���
	void			SetPrevRanking( vector<DWORD> & vecPrevRanking );	// ���� ���� ���
	vector<DWORD>	GetPrevRanking();
	
private:
	struct MINIGMAME_PRIZE_LIST
	{
		DWORD dwPlayerId;	int nCompletedNum;	DWORD dwCompletedTick;
		MINIGMAME_PRIZE_LIST( DWORD dwPI, int nCN, DWORD dwCT )
		: dwPlayerId( dwPI ), nCompletedNum( nCN ), dwCompletedTick( dwCT )	{}
	};
	vector<MINIGMAME_PRIZE_LIST>	m_vecMiniGamePrizeList;
	void	SetMiniGamePrize( DWORD dwTick );	// �̴ϰ��ӿϷ� ��ǰ���޿���� ��ǰ�� ����
public:
	void	SetMiniGamePrizeList( DWORD dwPlayerId, int nCompletedNum, DWORD dwCompletedTick )// �̴ϰ��ӿϷ� ��ǰ���� ����
	{ m_vecMiniGamePrizeList.push_back( MINIGMAME_PRIZE_LIST( dwPlayerId, nCompletedNum, dwCompletedTick ) ); }
	

private:
	BOOL	IsApplicationTime();		// ���κ��� ���̽� ��û �ð��ΰ�?
	BOOL	IsOpenTime();				// ���κ��� ���̽� ���� �ð��ΰ�?
	void	BeginRainbowRace();			// ���κ��� ���̽� ������ Ŭ�� �˸�(�������ڴ� Ż��)
	bool	SetNPC();					// ���� NPC�� �����.
	void	RemoveNPC();				// ���̽��� �Ϸ�Ǹ� ���� NPC�� �����Ѵ�.
	void	SetPrize();					// ��ǰ�� �������� �����Ѵ�.
	
	MAPRR					m_mapRainbowRace;	// �� �÷��̾ ���� ���� ���
	vector<CMiniGameBase*>	m_vecMiniGame;		// �̴ϰ��� ���
	CLuaBase				m_Lua;

	int			m_nState;					// ���� ����
	DWORD		m_dwNextTime;				// ���� ���·� �Ѿ�� �ð�
	vector<OBJID> m_vecNPCId;				// ���� NPC�� Id
	vector<DWORD> m_vecdwRankingId;			// ������ PlayerId
	vector<DWORD> m_vecPrevRanking;			// ���� 5������ ����



public:
	bool	Load();

	bool	IsActivate() const;

	bool	SetCurrentDay( const int _Day );

	int		DayStringToDefine( std::string & _strDay ) const;

private:
	bool	LoadScript_Time( const TCHAR* _pFileName );
	bool	LoadScript_NPC( const TCHAR* _pFileName );
	bool	LoadScript_Prize( const TCHAR* _pFileName );
	bool	LoadScript_MiniGamePrize( const TCHAR* _pFileName );

	bool	InsertApplicationData( const int _Day, const nsRainbowRaceData::T_RACE_APPLICATION_DATA & _ApplicationData );
	bool	InsertNPCData( const nsRainbowRaceData::T_RACE_NPC_DATA & _NPCData );
	bool	InsertPrizeData( const int _Day, const nsRainbowRaceData::T_RACE_PRIZE_DATA & _PrizeData );
	bool	InsertMiniGamePrizeData( const int _Day, const nsRainbowRaceData::T_RACE_MINIGAME_PRIZE_DATA & _MiniGamePrizeData );

	void	InitializeApplicationData();
	void	InitializePrizeData();
	void	InitializeMiniGamePrizeData();

	bool	CheckLoadScript() const;

	void	SetActivate( const bool _Activation );

	int		CalcCurrentDay() const;
	int		GetCurrentDay() const;

	int		GetApplicationPenya( const int _Day ) const;

	DWORD	GetWaitingTick( const int _Day ) const;
	DWORD	GetReadyTick( const int _Day ) const;
	DWORD	GetProgressTick( const int _Day ) const;

	const nsRainbowRaceData::T_RACE_APPLICATION_DATA*		GetApplicationData( const int _Day ) const;
	const nsRainbowRaceData::T_RACE_MINIGAME_PRIZE_DATA*	GetMiniGamePrizeData( const int _Day, const DWORD _CompleteCount ) const;

public:
	enum Day		{	DAY_NONE = 0, SUN, MON, TUE, WED, THU, FRI, SAT, DAY_MAX	};

private:
	enum ReadState	{	None, MyLanguage, Common	};

	int						m_Day;

	bool					m_Activation;

	nsRainbowRaceData::ApplicationDataMap		m_ApplicationData;
	nsRainbowRaceData::NPCDataVec				m_NPCData;
	nsRainbowRaceData::PrizeDataMap				m_PrizeData;
	nsRainbowRaceData::MiniGamePrizeDataMap		m_MiniGamePrizeData;
};

#endif __WORLDSERVER
