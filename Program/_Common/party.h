#ifndef __PARTY_H__
#define	__PARTY_H__

#ifdef __CORESERVER
#include <network/FLAr.h>
#include "projectcmn.h"
#endif // __CORESERVER

//#ifdef __WORLDSERVER
//#include "User.h"
//#endif // worldserver

#include <memory/FLMemPooler.h>
#include <map>

// 모드(데미지 증가, 유니크, 휴식, 아이템)
// 1, 2, 4, 8, 10, 12
// #define PARTY_LINKATTACK_MODE		0	// 링크어텍 상태	: 데미지 증가
// #define PARTY_FORTUNECIRCLE_MODE	1	// 포춘서클 상태	: 유니크 드롭확률
// #define PARTY_STRETCHING_MODE		2	// 스트레칭 상태	: 휴식 2배 : 단장 어시스트 3배
// #define PARTY_GIFTBOX_MODE			3	// 기프트박스 상태  : 아이템 양 2배
// 
// //sun: 12, 파스킬 아이템 수정
// #define PARTY_PARSKILL_MODE			4	// parskill 상태  :단장에게서 멀리떨어져도 사용가능
// #define MAX_PARTYMODE				5

#define	PP_REMOVE	0		// ???????????? 삭제하자...

const size_t	MAX_PTMEMBER_SIZE			= 8;
const size_t	MIN_PTMEMBER_SIZE			= 2;

const size_t	PARTY_MAP_SEC				= 2;
const size_t	PARTY_MAP_LENGTH			= 10;

const size_t	MAX_PARTY_BASE_LEVEL		= 10;

const size_t	MIN_PARSKILL_MODE_LEVEL		= 40;

enum PARTY_KIND {	PARTY_KIND_BASE = 0, PARTY_KIND_EXTEND, PARTY_KIND_MAX	};		// 단막극단, 순회극단

// 극단 스킬 모드
enum PARTY_SKILL_MODE
{
	PARTY_SKILL_NONE = 0
	, PARTY_LINKATTACK_MODE
	, PARTY_STRETCHING_MODE
	, PARTY_FORTUNECIRCLE_MODE
	, PARTY_GIFTBOX_MODE
	, MAX_PARTYMODE
};

// 극단 효과
enum PARTY_EFFECT_MODE
{
	PARTY_EFFECT_NONE = 0
	, PARTY_EFFECT_PARSKILLFULL
	, PARTY_EFFECT_POINT_LIMITLESS
	, PARTY_EFFECT_MAX
};


class CParty;
extern	CParty		g_Party;

typedef	struct	_PartyMember	// 플레이어 아이디만 가지고 있음
{
	u_long	m_uPlayerId;
	CTime	m_tTime;
	BOOL	m_bRemove;
#if defined( __WORLDSERVER ) || defined( __CLIENT )
	D3DXVECTOR3	m_vPos;
#endif // defined( __WORLDSERVER ) || defined( __CLIENT )
	_PartyMember()
	{
		m_uPlayerId	= NULL_PLAYER_ID;
		m_tTime = CTime::GetCurrentTime();
		m_bRemove = FALSE;

#if defined( __WORLDSERVER ) || defined( __CLIENT )
		m_vPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
#endif // #if defined( __WORLDSERVER ) || defined( __CLIENT )
	}
}	PartyMember,	*PPartyMember;

class CParty
{
private:
	bool	m_arrPartyEffect[ PARTY_EFFECT_MAX ];

	DWORD	m_dwKind;							// 극단 종류 : 단막극단, 순회극단

public:
	u_long	m_uPartyId;								// 극단 ID
	TCHAR	m_sParty[33];							// 극단 명칭( 단막극단 : NO, 순회극단 : YES )
	PartyMember	m_aMember[MAX_PTMEMBER_SIZE];		// 한개의 극단의 극단원 정보
	int		m_nSizeofMember;						// 극단원 숫자	( 2 ~ 8 )
	LONG	m_nLevel, m_nExp, m_nPoint;				// 극단 레벨, 경험치, 포인트
	int		m_nTroupsShareExp, m_nTroupeShareItem;	// 경험치 분배방식, 아이템 분배방식
	
	int		m_nReferens;							// 극단에 포함되어 있는 상태일때 게임에 나갔을경우 10분후에 탈퇴 검색할 파티
	DWORD	m_dwModeTime[MAX_PARTYMODE];				// 모드 시간
	u_long	m_nGetItemPlayerId;						// 아이템 얻은 캐릭터
	u_long	m_idDuelParty;							// 파티 듀얼중이면 상대방 파티의 ID, 아니면 0

#ifdef __WORLDSERVER
	DWORD	m_dwWorldId;
#endif // __WORLDSERVER

public:
#if !defined(__WORLDSERVER) && !defined(__CLIENT)
	CMclCritSec	m_AddRemoveLock;
#endif

public:
//	Constructions
	CParty();
	~CParty();

//	Operations
	BOOL	IsGuildParty();


	void	InitParty();
	BOOL	IsMember( u_long uPlayerId )		{	return( FindMember( uPlayerId ) >= 0 );	}
	int		IsLeader( u_long uPlayerId ) const		{	return( m_aMember[0].m_uPlayerId == uPlayerId );	}
	int		GetSizeofMember()					{	return m_nSizeofMember;		}
	int		GetLevel() const							{	return m_nLevel;			}
	int		GetExp()							{	return m_nExp;			}
	int		GetPoint()							{	return m_nPoint;		}
	u_long	GetPlayerId( int i )	{	return m_aMember[i].m_uPlayerId;	}
#if defined( __WORLDSERVER ) || defined( __CLIENT )
	D3DXVECTOR3	GetPos( int i )		{	return m_aMember[i].m_vPos;			}
	void	SetPos( int i, D3DXVECTOR3 vPos );
#endif // #if defined( __WORLDSERVER ) || defined( __CLIENT )
#ifndef __CORESERVER
	CMover* GetLeader( void ) const;
#endif // __CORESERVER
//	FLWSUser	*GetMember( int nIdx ) 
//	{ 
//		return g_xWSUserManager->GetUserByPlayerID( m_aMember[nIdx].m_uPlayerId );
//	}
		
	void	SetPartyId( u_long uPartyId )		{	 m_uPartyId = uPartyId ;	}
//sun: 11, 캐릭터 정보 통합
	BOOL	NewMember( u_long uPlayerId );

	BOOL	DeleteMember( u_long uPlayerId );

#if !defined(__WORLDSERVER) && !defined(__CLIENT)
	void	Lock( void )	{	m_AddRemoveLock.Enter();	}
	void	Unlock( void )	{	m_AddRemoveLock.Leave();	}
#endif

	void	ChangeLeader( u_long uLeaderId );

	void	Serialize( CAr & ar );

	bool	SwapPartyMember( int first, int Second );

//sun: 12, 극단유료아이템
	DWORD	GetPartyModeTime( int nMode );
	void	SetPartyMode( int nMode, DWORD dwSkillTime ,int nCachMode );

	void	DoUsePartySkill( u_long uPartyId, u_long nLeaderid, int nSkill );
#ifdef __WORLDSERVER
	void	SetPartyLevel( FLWSUser* pUser, DWORD dwLevel, DWORD dwPoint, DWORD dwExp );
	void	DoDuelPartyStart( CParty *pDst );
	void	DoDuelResult( CParty *pParty, BOOL bWin, int nAddFame, float fSubFameRatio );
	void	DoUsePartyReCall( u_long uPartyId, u_long nLeaderid, int nSkill );
	void	Replace( DWORD dwWorldId, D3DXVECTOR3 & vPos, BOOL bMasterAround );
	void	Replace( DWORD dwWorldId, LPCTSTR sKey );
	BOOL	ReplaceChkLv( int Lv );
	BOOL	ReplaceChkLowLv( int Lv );
	void	ReplaceLodestar( const CRect &rect );
#endif

	void	DoDuelPartyCancel( CParty* pDuelParty );
	void	GetPoint( int nTotalLevel, int nMemberSize, int nDeadLeavel );
//	Attributes
	int		GetSize( void );
	int		FindMember( u_long uPlayerId );



	//////////////////////////////////////////////////////////////////////////
	DWORD	GetKind() const;
	void	SetKind( const DWORD dwPartyKind );
	//////////////////////////////////////////////////////////////////////////




	//////////////////////////////////////////////////////////////////////////
	int		GetMinLevel( const DWORD dwPartyKind ) const;
	bool	IsActivatedEffect( const DWORD dwEffectMode ) const;
	bool	SetEffectActivation( const DWORD dwEffectMode, const bool bActivate );
	bool	SetDisconnectedMember( const u_long uPlayerID );
	bool	ForceTurnOverLeader();
	void	InitializeSkillMode();
//	void	InitializeEffectMode();

	bool	CanChangePartyKind( const DWORD dwPartyType ) const;

#ifdef __WORLDSERVER
	int		GetReqPointByUseSkill( const int nSkillID );
	bool	CanApplySkillMode( const u_long uPlayerID, const int nSkillMode ) const;
#endif // __WORLDSERVER
	//////////////////////////////////////////////////////////////////////////

private:


public:
	MEMPOOLER_DECLARE( CParty );
};

using	namespace	std;
typedef	map< u_long, CParty*>	C2PartyPtr;
typedef map<u_long, string>	ULONG2STRING;
typedef map<string, u_long>	STRING2ULONG;

class CPlayer;

class CPartyMng
{
private:
	u_long		m_id;	// 새로 생성되는 파티에 순차적으로 아이디를 할당하기 위한 변수다.
//	CMapParty	m_2Party;
	C2PartyPtr	m_2PartyPtr;
#ifdef __WORLDSERVER
	//int			m_nSecCount;
	DWORD		m_dwPartyMapInfoLastTick;
#endif // __WORLDSERVER
public:
#if !defined(__WORLDSERVER) && !defined(__CLIENT)
	CMclCritSec	m_AddRemoveLock;
#endif
#ifdef __CORESERVER
	EXPPARTY	m_aExpParty[MAX_PARTYLEVEL];
#endif	// __CORESERVER

	ULONG2STRING	m_2PartyNameLongPtr;
	STRING2ULONG	m_2PartyNameStringPtr;

public:
//	Constructions
	CPartyMng();
	~CPartyMng();
	void	Clear( void );
//	Operations
//	u_long	NewParty( u_long uLeaderId, LONG nLeaderLevel, LONG nLeaderJob, BYTE nLeaderSex, LPSTR szLeaderName, u_long uMemberId, LONG nMemberLevel, LONG nMemberJob, BYTE nMemberSex, LPSTR szMembername, u_long uPartyId = 0 );

	u_long	NewParty( const u_long uLeaderId, const u_long uMemberId, const u_long uPartyId = 0 );

	BOOL	DeleteParty( u_long uPartyId );
	CParty*	GetParty( u_long uPartyId ) const;
#if !defined(__WORLDSERVER) && !defined(__CLIENT)
	void	Lock( void )	{	m_AddRemoveLock.Enter();	}
	void	Unlock( void )	{	m_AddRemoveLock.Leave();	}
#endif

	void	Serialize( CAr & ar );

//	Attributs
	int		GetSize( void );

#ifdef __CORESERVER
public:
	HANDLE	m_hWorker;
	HANDLE	m_hCloseWorker;

public:
	BOOL	IsPartyNameId( u_long uidPlayer );
	BOOL	IsPartyName( const char* szPartyName );
	LPCSTR  GetPartyString( u_long uidPlayer );
	u_long  GetPartyID( const char* szPartyName );
	void	AddPartyName( u_long uidPlayer, const char* szPartyName );
	BOOL	CreateWorkers( void );
	void	CloseWorkers( void );
	static	UINT	_Worker( LPVOID pParam );
	void	Worker( void );
	void	RemovePartyName( u_long uidPlayer, const char* szPartyName );

	void	AddConnection( CPlayer* pPlayer );
	void	RemoveConnection( CPlayer* pPlayer );
#endif // __CORESERVERE

#ifdef __WORLDSERVER
	void	PartyMapInfo( void );
#endif // __WORLDSERVER






	//////////////////////////////////////////////////////////////////////////
	bool	SetPartyEffectMode( const u_long uPlayerID, const u_long uPartyID, const DWORD dwEffectMode, const bool bActivate );

	bool	RemovePartyMember( const u_long uPartyID, const u_long uLeaderID, const u_long uMemberID );

	bool	DisconnectPartyMember( const u_long uPartyID, const u_long uPlayerID );


#ifdef __CORESERVER
	DWORD	CanInviteParty( const CPlayer & kSrcPlayer, const CPlayer & kDestPlayer );
	void	OnInviteParty( const u_long uSrcPlayerID, const u_long uDestPlayerID );
	void	RefreshPartyEffectMode( const u_long uPlayerID, const u_long uPartyID, const DWORD dwEffectMode );
#endif // __CORESERVER

#ifdef __WORLDSERVER
	DWORD	CanInviteParty( const u_long uLeaderPlayerID, const u_long uMemberPlayerID );
	void	RefreshPartyEffectMode( const u_long uPartyID, const DWORD dwEffectMode ) const;
	void	CheckAddMemberPartyEffect( const u_long uPlayerID, const u_long uPartyID ) const;
	void	CheckRemoveMemberPartyEffect( const u_long uPlayerID, const u_long uPartyID ) const;

	DWORD	GetPartySkillID_BySkillMode( const DWORD dwMode ) const;
	DWORD	GetDefaultSkillTime( const u_long uLeaderID, const DWORD dwMode ) const;
#endif // __WORLDSERVER
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
#ifdef __CORESERVER
	void	NotiRefreshPartyEffectModeToWS( const u_long uPartyID, const DWORD dwEffectMode ) const;
	void	NotiSetPartyEffectModeToWS( const u_long uPlayerID, const u_long uPartyID, const DWORD dwEffectMode, const bool bActivate ) const;
#endif // __CORESERVER

#ifdef __WORLDSERVER
	void	NotiRefreshPartyEffectModeToCS( const u_long uPlayerID, const u_long uPartyID, const DWORD dwEffectMode ) const;
	void	NotiSetPartyEffectModeToCS( const u_long uPlayerID, const u_long uPartyID, const DWORD dwEffectMode, const bool bActivate ) const;
	void	RequestAddPartyMemberToCS( const u_long uLeaderPlayerID, const u_long uMemberPlayerID ) const;

	void	NotiRefreshPartyEffectModeToClient( const u_long uPartyID ) const;
	void	NotiSetPartyEffectModeToClient( const u_long uPartyID, const DWORD dwEffectMode, const bool bActivate ) const;
#endif // __WORLDSERVER
	//////////////////////////////////////////////////////////////////////////
};

#endif	//	_PARTY_H