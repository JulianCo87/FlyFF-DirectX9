#if !defined(AFX_MOVER_H__4B7B21D7_A2D3_4115_946C_68DC9045A845__INCLUDED_)
#define AFX_MOVER_H__4B7B21D7_A2D3_4115_946C_68DC9045A845__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "pocket.h"		//sun: 11, �ָӴ�
#include "buff.h"

#include "Ctrl.h"
#include "..\_AIInterface\ActionMover.h"
#include "SkillInfluence.h"
#include "CooltimeMgr.h"
#include <memory/FLMemPooler.h>

#ifdef __WORLDSERVER
	#include "NpcProperty.h"
	#include "SfxHitArray.h"
	#include "..\_AIInterface\AIInterface.h"
	#include "FLAggro.h"
	#ifdef __ENCHANT_BARUNA16
	#include "NPCSummoner.h"
	#endif //__ENCHANT_BARUNA16
#else
	#include "CooltimeMgr.h"
#endif

#include "Synchronizer.h"
#include "Stat.h"


//#define	MAX_NAME				64


#define	COLOR_PLAYER			0xffffffff
#define	COLOR_NPC				0xffa0a0ff  
#define	COLOR_MONSTER			0xffffffa0
#define	COLOR_PARTY				0xff70f070
#define COLOR_GUILDCOMBAT		0xff00ff00

/// SetMotion() option
#define	MOP_NONE				0x00000000
#define	MOP_SWDFORCE			0x00000001		/// �˱� �����ϰ�
#define	MOP_FIXED				0x00000002		/// dwMotion���� �״�� �ε��ϰ� ( +100 ���ϰ�.. )
#define	MOP_NO_TRANS			0x00000004		/// ��� Ʈ������ ��������.
#define	MOP_HITSLOW				0x00000008		/// Ÿ�����ĺ��� ���ο�.

#define	REQ_USESKILL			0x00000001		/// �����û �÷��� - ��ų��� ����
#define	REQ_MOTION				0x00000002		/// �����û �÷��� - ��� ����

#define	OBJTYPE_PLAYER			0x00000001		
#define	OBJTYPE_MONSTER			0x00000002		
#define OBJTYPE_CTRL			0x00000004		
#define	OBJTYPE_FLYING			0x00010000		/// m_dwTypeFlag���� ���.

#define	CRITICAL_PROBABILITY	3
#define	CRITICAL_BERSERK_HP		30
#define	CRITICAL_BERSERK_PROB	22
#define	CRITICAL_AFTER_DEATH	22

// UPDATEITEM
#define	UI_NUM							0
#define	UI_HP							1
#define	UI_RN							2
#define	UI_AO							3
#define	UI_RAO							4
#define	UI_IR							5
#define	UI_PIERCING_SIZE				6
#define	UI_PIERCING						7
#define UI_COOLTIME						8
#define	UI_FLAG							9
#define	UI_RANDOMOPTITEMID				10
#define	UI_KEEPTIME						11
#define UI_REPAIR_NUM					12
#define UI_ULTIMATE_PIERCING_SIZE		13
#define UI_ULTIMATE_PIERCING			14
#define UI_PETVIS_SIZE					15
#define UI_PETVIS_ITEM					16
#define UI_PETVIS_ITEMSWAP				17
#define UI_TRANSFORM_VISPET				18
#define UI_EQUIP_LEVEL					19
#define UI_LOOKS_CHANGE					20
// UPDATEITEM


const int PETVIS_DEFAULT_VIS_SLOT_SZIE	=	2;

#define RESTATE_LOW				50


//sun: 10�� ���½ý���	Neuz, World, Trans

#define MAX_SKILL_JOB	( MAX_JOB_SKILL + MAX_EXPERT_SKILL + MAX_PRO_SKILL + MAX_MASTER_SKILL + MAX_HERO_SKILL + MAX_LEGEND_HERO_SKILL )


#ifdef __INTERNALSERVER
#define	TICK_CHEERPOINT	MIN( 1 )
#else	// __INTERNALSERVER
#define	TICK_CHEERPOINT	MIN( 60 )
#endif	// __INTERNALSERVER

#define	MAX_CHEERPOINT	3
#define	NULL_CHGPARAM	0x7FFFFFFF

const DWORD NEXT_TICK_DUEL          = SEC(1);
const DWORD NEXT_TICK_ENDDUEL       = MIN(3);
const DWORD NEXT_TICK_RECOVERY      = SEC(2);
const DWORD NEXT_TICK_RECOVERYSTAND = SEC(3);


//sun: 10�� ���½ý���	Neuz, World, Trans

#define	LEGEND_CLASS_NORMAL			0
#define	LEGEND_CLASS_MASTER			1
#define	LEGEND_CLASS_HERO			2
#define	LEGEND_CLASS_LEGEND_HERO	3


/// Replace�Լ����� ����ϴ� Ÿ�� 	
enum REPLACE_TYPE
{
	REPLACE_NORMAL,				/// ����ü Ÿ�� �κ��� ���� ���� 
	REPLACE_FORCE,				/// ������ �̵� 
};

/// ��ų ��� Ÿ��
enum SKILLUSETYPE
{
	SUT_NORMAL,			/// ����Ű�� �ְ� ����ϴ� �Ϲ����� ���.
	SUT_QUEUESTART,		/// ��ųť�� ó�� �����ϴ� ��ų�̴�.(ĳ������ ���Ƽ���)
	SUT_QUEUEING,		/// ��ųť�� ���ӹ߻��� ��ų�̴�.(ĳ���õ��۾��� �ٷ� �߻�)
};

////////////////////////////////////////////////////////////////////////////////
// PVP ���� 
////////////////////////////////////////////////////////////////////////////////

/// Player vs Player�� ��� 
enum PVP_MODE
{
	PVP_MODE_NONE,				/// PVP�Ұ� 	
	PVP_MODE_GUILDWAR,			/// ���� 
	PVP_MODE_PK,				/// PK
	PVP_MODE_DUEL				/// ��� 
};

/// �ŷ� Ȯ�� Ÿ�� 
enum TRADE_CONFIRM_TYPE
{
	TRADE_CONFIRM_ERROR,		/// �κ��� ������ ���ų�, GOLD�� OVERFLOW���� ��� 
	TRADE_CONFIRM_OK,			/// ��뵵 ok(���� �ŷ�)
};

/// �ŷ����� 
enum TRADE_STATE
{
	TRADE_STEP_ITEM,			/// ������ �ܰ� - �������� �ø��� ������  
	TRADE_STEP_OK,			    /// OK���� �ܰ� - ok��ư�� ���ȴ�.        
	TRADE_STEP_CONFIRM,			/// CONFIRM�ܰ� - ����Ȯ�δܰ�            
};

////////////////////////////////////////////////////////////////////////////////
// struct
////////////////////////////////////////////////////////////////////////////////

/// ���� ���� ����ü 
typedef	struct	_EQUIP_INFO
{
	DWORD	dwItemID;
	int		nOption;
	BYTE	byFlag;

	DWORD	dwLooksChangeItemID;
	bool	bIsCombined;

	_EQUIP_INFO()
	{
		Init();
	}

	void	Copy( FLItemElem & kItemElem )
	{
		dwItemID			= kItemElem.m_dwItemId;
		nOption				= kItemElem.GetAttrOption();
		byFlag				= kItemElem.m_byFlag;
		dwLooksChangeItemID	= kItemElem.GetLooksChangeItemID();
		bIsCombined			= kItemElem.IsSetCombinedOption();
	}

	void	Init()
	{
		dwItemID			= 0;
		nOption				= 0;
		byFlag				= 0;
		dwLooksChangeItemID	= 0;
		bIsCombined			= false;
	}

}	EQUIP_INFO,	*PEQUIP_INFO;

/// ���� �߰� ���� ����ü 
// typedef	struct	_EQUIP_INFO_ADD
// {
// 	BYTE				byItemResist;
// 	int					nResistAbilityOption;
// 	WORD				wRandomOptionOriginId;
// 	int					nLevelDown;
// 
// 	DWORD				dwCombinedAddDamage;
// 	WORD				wCombinedRandomOptionOriginID;
// 
// 	FLPiercingOption	kPiercingOption;
// 	FLRandomOption		kRandomOption;
// 
// #ifdef __CLIENT
// 	CTexture*			pTexture;
// #endif	// __CLIENT
// 	_EQUIP_INFO_ADD()
// 	{
// 		byItemResist					= SAI79::NO_PROP;
// 		nResistAbilityOption			= 0;
// 		wRandomOptionOriginId			= 0;
// 		nLevelDown						= 0;
// 
// 		dwCombinedAddDamage				= 0;
// 		wCombinedRandomOptionOriginID	= 0;
// 
// 		kPiercingOption.Clear();
// 		kRandomOption.Clear();
// 
// #ifdef __CLIENT
// 		pTexture	= NULL;
// #endif	// __CLIENT
// 	}
// 
// 	void	CopyItemOption( FLItemElem & kItemElem )
// 	{
// 		byItemResist					= kItemElem.GetItemResist();
// 		nResistAbilityOption			= kItemElem.GetResistAbilityOption();
// 		wRandomOptionOriginId			= kItemElem.GetRandomOptionOriginID();
// 		nLevelDown						= kItemElem.GetLevelDown();
// 
// 		dwCombinedAddDamage				= kItemElem.GetCombinedAddDamage();
// 		wCombinedRandomOptionOriginID	= kItemElem.GetCombinedRandomOptionOriginID();
// 
// 		kItemElem.GetPiercingOption( kPiercingOption );
// 		kItemElem.GetRandomOption( kRandomOption );
// 	}
// 
// 	void	Serialize( CAr & ar )
// 	{
// 		ar << byItemResist;
// 		ar << nResistAbilityOption;
// 		ar << wRandomOptionOriginId;
// 		ar << nLevelDown;
// 
// 		ar << dwCombinedAddDamage;
// 		ar << wCombinedRandomOptionOriginID;
// 
// 		kPiercingOption.SerializePiercingOption( ar );
// 		kPiercingOption.SerializeCombinedPiercingOption( ar );
// 		kRandomOption.SerializeRandomOptionExtension( ar );
// 		kRandomOption.SerializeCombinedRandomOptionExtension( ar );
// 	}
// 
// 	void	Deserialize( CAr & ar )
// 	{
// 		ar >> byItemResist;
// 		ar >> nResistAbilityOption;
// 		ar >> wRandomOptionOriginId;
// 		ar >> nLevelDown;
// 
// 		ar >> dwCombinedAddDamage;
// 		ar >> wCombinedRandomOptionOriginID;
// 
// 		kPiercingOption.DeserializePiercingOption( ar );
// 		kPiercingOption.DeserializeCombinedPiercingOption( ar );
// 		kRandomOption.DeserializeRandomOptionExtension( ar );
// 		kRandomOption.DeserializeCombinedRandomOptionExtension( ar );
// 	}
// 
// }	EQUIP_INFO_ADD, *PEQUIP_INFO_ADD;

/// �÷��̾� �ܾ� ����ü 
typedef struct tagPLAYER
{
	EQUIP_INFO	m_aEquipInfo[MAX_HUMAN_PARTS];
	char		m_byCostume;
	char		m_bySkinSet;
	char		m_byFace;
	char		m_byHairMesh;
	char		m_byHairColor;
	char		m_byHeadMesh;
	char		m_bySex;
	char		m_byJob;
	u_short		m_uSlot;
} PLAYER,* LPPLAYER;

/// ����Ʈ ����ü 
typedef struct tagQuest
{
	BYTE		m_nState;
	WORD		m_wTime;
	WORD		m_wId;
//	BYTE		m_nKillNPCNum[ 2 ];	// chipi_091015 - NPC Kill Quest ���� Ȯ��( BYTE -> WORD )
	WORD		m_nKillNPCNum[ MAX_QUEST_KILL_NPC_COUNT ];	// chipi_091015 - NPC Kill Quest ���� Ȯ��( BYTE -> WORD )
	BYTE		m_bPatrol   : 1;
	BYTE		m_bDialog   : 1;
	BYTE		m_bReserve3 : 1;
	BYTE		m_bReserve4 : 1;
	BYTE		m_bReserve5 : 1;
	BYTE		m_bReserve6 : 1;
	BYTE		m_bReserve7 : 1;
	BYTE		m_bReserve8 : 1;

	QuestProp* GetProp() { return prj.m_aPropQuest.GetAt( m_wId ); }

} QUEST,* LPQUEST;

/// �ǰ� ���� ����ü 
struct HIT_INFO
{
	int		nHit;		/// ���� ������ 
	DWORD	dwTick;		/// ���ݴ��� �ð� 
};

typedef map<OBJID, HIT_INFO> SET_OBJID;		

/// ������ Ÿ�� 
enum HITTYPE
{
	HITTYPE_FAIL	= 0,
	HITTYPE_GENERIC,			/// Player vs Monster
	HITTYPE_PVP,				/// Player vs Player
	HITTYPE_WAR,				/// ����� 
	HITTYPE_SCHOOL,				/// �б� ���� �̺�Ʈ Ÿ�� 
	HITTYPE_PK,					/// Player Killing
	HITTYPE_GUILDCOMBAT,		/// ������ 
	HITTYPE_ARENA,				// �Ʒ���
	HITTYPE_EVENTARENA,			// �̺�Ʈ �Ʒ���
};

/// �������� ����Ÿ ����ü 
typedef struct RESURRECTION_DATA
{
	u_long		  dwPlayerID;
	BOOL		  bUseing;
	SkillProp	 *pSkillProp;
	AddSkillProp *pAddSkillProp;
	int			  nDamage;
} RESURRECTION_DATA;

/// �ŷ��� ��� ����ü 
struct VENDOR_SELL_RESULT
{
	FLItemElem	item;			/// �ȸ� ������ 
	int			nRemain;		/// �ȸ��� ���� ���� 
	int			nErrorCode;		/// �����ڵ� 
};

/// ���λ���(Vendor)�� �ŷ�(Trade) Ŭ���� 
class CVTInfo
{
public:
	CVTInfo();

private:
	DWORD					m_dwTradeGold;					/// �ŷ����� �� 
	FLItemBase*				m_apItem_VT[MAX_VENDITEM];		/// vendor and trader share pointer array
	OBJID					m_objId;						/// ���� id
	CMover*					m_pOwner;						/// Ŭ���� ������ 
	TRADE_STATE				m_state;						/// �ŷ����� 

	string					m_strTitle;						/// ���λ����� Ÿ��Ʋ 

public:
	void					Clear();


	OBJID					GetOtherID() { return m_objId; }
	CMover*					GetOther() const;
	void					SetOther( CMover* pMover );
	void					Init( CMover* pMover );
	FLItemBase*				GetItem( BYTE byNth );
	void					SetItem( BYTE byNth, FLItemBase* pItemBase );
	LPCTSTR					GetTitle();
	void					SetTitle( LPCTSTR szTitle );
	BOOL					IsVendorOpen();

	void					TradeClear();
	void					TradeSetItem( DWORD dwItemObjID, BYTE byNth, int nItemNum );
	BOOL					TradeClearItem( BYTE byNth );
	void					TradeSetGold( DWORD dwGold );
	int						TradeGetGold();
	BOOL					TradeConsent( CAr & ar );
	DWORD					TradeSetItem2( DWORD dwItemObjID, BYTE byNth, int & nItemNum );	// Return: dwText 
	TRADE_CONFIRM_TYPE		TradeLastConfirm( CAr& ownerar, CAr& traderar, CAr & logar );
	TRADE_STATE				TradeGetState();
	void					TradeSetState( TRADE_STATE state );

	void					VendorClose( BOOL bClearTitle = TRUE );	// ���� ���� �ݱ�
	void					VendorCopyItems( FLItemBase** ppItemVd );
	void					VendorItemNum( BYTE byNth, int nNum );
	void					VendorSetItem( DWORD dwItemObjID, BYTE byNth, int nNum, int nCost );
	BOOL					VendorClearItem( BYTE byNth );
	BOOL					VendorSellItem( CMover* pBuyer, BYTE byNth, DWORD dwItemId, int nNum, VENDOR_SELL_RESULT& result );
	BOOL					VendorIsVendor(); 
	BOOL				IsTrading( FLItemElem* pItemElem );
};

/// �������� SFX��ŷ ������ ���� �ڷ��� 
struct SFXHIT_INFO
{
	OBJID	idTarget;
	int		nMagicPower;
	DWORD	dwSkill;
	int		nDmgCnt;
	float	fDmgAngle;
	float	fDmgPower;
	DWORD	dwAtkFlags;

	BOOL	bControl;		//sun: 8, // __S8_PK

	DWORD	dwTickCount;
};

struct	SFXHIT_COUNT
{
	DWORD	dwSkill;
	DWORD	dwTickCount;
};

class	CGuild;
class	CGuildWar;
class	CNpcProperty;
struct	ATTACK_INFO;
struct	REGIONELEM;
class	CParty;
class	CActionMover;

#ifdef __LAYER_1015
#define	REPLACE( uMulti, dwWorld, vPos, type, nLayer )	Replace( (uMulti), (dwWorld), (vPos), (type), (nLayer) )
#else	// __LAYER_1015
#define	REPLACE( uMulti, dwWorld, vPos, type )			Replace( (uMulti), (dwWorld), (vPos), (type) )
#endif	// __LAYER_1015

#ifdef __CLIENT
class CClientPet
{
public:
	CClientPet();
	virtual	~CClientPet();
public:
	void	SetObj( CMover* pObj )	{	m_pObj	= pObj;	}
	CMover*	GetObj()	{	return m_pObj;		}
	void	SetLevelup( BYTE nLevelup )		{	m_nLevelup	= nLevelup;		}
	BYTE	GetLevelup()	{	return m_nLevelup;	}
//sun: ?, __PET_1024
	void	SetName( char* szName );
	char*	GetName()	{	return m_szName;	}
	BOOL	HasName()	{	return strlen( m_szName ) > 0;	}

private:
	CMover*	m_pObj;
	BYTE	m_nLevelup;		// PLU_LEVEL_UP �̸�, �� ���� �� �� ������ ȿ�� �����ϰ� 0���� �ʱ�ȭ
	
//sun: ?, __PET_1024
	char	m_szName[MAX_PET_NAME];

};
#endif	// __CLIENT

typedef	struct	_T_MOVE_CHECK_DATA
{
	DWORD	dwPrevMoveTick;
	DWORD	dwLastMoveTick;

} T_MOVE_CHECK_DATA, *PT_MOVE_CHECK_DATA;

namespace nsRevival
{
	struct DECREASE_EXP_DATA
	{
		float	decreaseExpRate;
		bool	downLevel;

		DECREASE_EXP_DATA() : decreaseExpRate( 0.0f ), downLevel( false )	{}
	};
}

/// �÷��̾�� NPC
class CMover : public CCtrl
{	
public:	
	CMover();
	virtual ~CMover();

	friend			CActionMover;
	enum			{ PLAYER, NPC };			/// CMover�� Ÿ�� 

public:
	//DWORD			m_dwUpdateDestTick;

	T_MOVE_CHECK_DATA	m_tMoveCheckData;

	T_CONNECTION_POINT	m_kMadrigalGiftPoint;

#ifdef __WORLDSERVER
	int					m_nNearPCCount;
#endif 

	BOOL			m_bPlayer;					/// Player�ΰ�, �ƴ϶�� NPC
	u_long			m_idPlayer;					/// player�� db��ȣ, NPC�� 0xffffffff
	CActionMover*	m_pActMover;				/// ������ �����̴� ��ü. ���⼭ ������ �������� ��Ʈ���Ѵ�.
	DWORD			m_dwTypeFlag;				/// OBJTYPE_FLYING �� ��� flag
	DWORD			m_dwMode;					/// ����, ����, ���� ��� flag
	DWORD			m_dwFlag;					/// MVRF_ ���� flag �ø���
	DWORD			m_dwStateMode;				/// PK, PVP, ���� ���� ... flag
	float			m_fAniSpeed;				/// �ִϸ��̼� �ӵ� ���. 1.0�� �⺻. 2.0�̸� �ι������. 2.0�� �ƽ���.
	float			m_fArrivalRange;			/// ��ǥ�� �󸶸�ŭ �����ؾ��ϴ°�. ���ʹ���. ����Ʈ 0
	OBJID			m_idDest;					/// ��ǥ ��ü ���̵�
	D3DXVECTOR3		m_vDestPos;					/// ��ǥ ���� 
	CShip*          m_pIADestPos;				/// ���⿡ �����Ͱ� �ִٸ� m_vDestPos�� ��ǥ�� m_pIADestPos�������� ���� �����ǥ�� �ȴ�.
	BOOL			m_fWaitQueryGetPos;			/// ����ȭ ���� 
	bool			m_bForward;					/// ������?
	bool			m_bPositiveX;				/// GetPos().x - m_vDestPos.x > 0.0f
	bool			m_bPositiveZ;				/// GetPos().z - m_vDestPos.z > 0.0f
	DWORD			m_dwRegionAttr;				/// ���� ��ǥ�� region �Ӽ� 
	DWORD			m_dwOldRegionAttr;			/// ���� ��ǥ�� region �Ӽ� 
	DWORD			m_dwMotion;					/// ��� ��ȣ 
	DWORD			m_dwMotionOption;			/// ��� �߰� �ɼ�
	OBJMSG			m_dwMotionArrive;			/// �̵� �Ϸ� �� ���

	DWORD			m_dwTickRecovery;			/// ���� ������ hp, mp ���� timer
	DWORD			m_dwTickRecoveryStand;		/// stand ������ hp, mp ���� timer

#if defined __WORLDSERVER && defined __AGGRO16
	FLAggro::MapAggroOwner		m_AggroOwnerList;	/// ���� ��׷θ� ���� ��� ����Ʈ (��ȣ����)
#endif // __WORLDSERVER

#ifdef __WORLDSERVER
	BOOL			m_IsReturnToBegin;

	BOOL			TryDelete_IntoIdleMode();
	void			SetDelete_IntoIdleMode( const BOOL bDelete );
private:
    BOOL			m_bDelete_IntoIdleMode;		///AI���� IDLE ���� ���ƿý� ��ü ����( ������ ������ �����ؾ� �ϴ� ��� )
#endif // __WORLDSERVER

public:
	int				m_nAccountPlayTime;		//ms���� ���� �÷��� �ð�

	BOOL	HasBuff( WORD wType, WORD wId );
	void	RemoveBuff( WORD wType, WORD wId );
	BOOL	HasBuffByIk3( DWORD dwIk3 );

	//////////////////////////////////////////////////////////////////////////
	// mirchang_101101
	bool	HasUseLimitGroupItemBuff( int nUseLimitGroup );
	int		GetCountBuffByItemGroup( int nItemGroup );
	int		GetCountBuffByItemId( DWORD dwId );
	int		GetAdjValueDuplicationBuff( DWORD dwItemKind3 );
	int		GetRemainCountBuff( const DWORD dwItemKind3 );

	IBuff*	GetBuffByIk3( DWORD dwItemKind3 );
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	void	DoApplyHitActiveSkill( CMover* pDest, const DWORD dwProb );
	void	DoApplyDamageActiveSkill( CMover* pDest, const DWORD dwProb );
	//////////////////////////////////////////////////////////////////////////

#ifdef __WORLDSERVER
	BOOL	HasPet();
	void	RemovePet();
#endif	// __WORLDSERVER
	DWORD	GetDisguise();
	BOOL	AddBuff( WORD wType, WORD wId, DWORD dwLevel, DWORD tmTime, DWORD tmPain, BYTE byBuffTickType, OBJID oiAttacker = NULL_ID );
	void	RemoveCommonBuffs();
	void	RemoveChrStateBuffs( DWORD dwChrState );
	void	RemoveIk3Buffs( DWORD dwIk3 );
	void	RemoveAllBuff();
	void	RemoveAttackerBuffs( OBJID oiAttacker );
	void	RemoveDstParamBuffs( DWORD dwDstParam );
	void	RemoveDebufBuffs();
	void	ProcessBuff();

	void	RemoveDebuffOnDamaged();
#ifdef __CLIENT
	void	ClearBuffInst();
	int		GetCurrentMaxSkillPoint();
#endif	// __CLIENT

	CBuffMgr		m_buffs;

	DWORD			m_dwSMTime[SM_MAX];			/// ���� ������ �ð� ���� ������ ����

	SKILL			m_aJobSkill[ MAX_SKILL_JOB ];		/// ��ų �迭 
	DWORD			m_tmReUseDelay[ MAX_SKILL_JOB ];	/// ��ų ����ð�
	
	Stat			m_Stat;
	LONG			m_nLevel;					/// ���� 
	LONG			m_nDeathLevel;				/// �׾��� �� ����
	EXPINTEGER		m_nExp1, m_nDeathExp;		/// m_nExp1 - ����ġ, DeathExp - �׾��� �� ����ġ

private:
	int		m_nTutorialState;	//sun: 12, Ʃ�丮�� ����

public:
//sun: 12, Ʃ�丮�� ����
	LONG	GetFlightLv( void )	{	return ( GetLevel() >= 20? 1: 0 );		}
	void	SetFlightLv( LONG /*nFlightLv*/ )	{}
	int		GetTutorialState( void )	{	return m_nTutorialState;	}
	void	SetTutorialState( int nTutorialState )	{	m_nTutorialState	= nTutorialState;	}

	LONG			m_nFxp;		/// ���෹��, 
	LONG			m_nHitPoint;				/// HP
	LONG			m_nManaPoint;				/// MP
	LONG			m_nFatiguePoint;			/// FP( ������ )
	LONG			m_nDefenseMin, m_nDefenseMax;	/// ���� min, ���� max
	int				m_nAdjHitRate, m_nAdjParry;		/// ����ġ 
	LONG			m_nJob;						/// ����

	BYTE			m_bySex;						/// ���� 
	DWORD			m_dwSkinSet, m_dwFace, m_dwHairMesh, m_dwHairColor, m_dwHeadMesh;	/// �ܾ� 
	FLOAT			m_fHairColorR, m_fHairColorG, m_fHairColorB;						/// �Ӹ��� 

	DWORD			m_dwAuthorization;			/// ���� ���� (�Ϲݿ��� �ְ� �����ڱ��� )

	TCHAR			m_szCharacterKey[32];		/// NPC key���ڿ� 
	DWORD			m_dwBelligerence;			/// ȣ���� ���� 
	BOOL			m_bActiveAttack;			/// �������ΰ�?
	DWORD			m_dwVirtItem;				/// npc�� ���(����)�� ������. �Ǵ� �÷��̾ ���� �������� ����� �⺻ ������. 
	DWORD			m_dwVirtType;				/// m_dwVirtItem�� ���������� ��ų���� ���� VT_ITEM, VT_SKILL
//private:
	OBJID	m_oiEatPet;
public:
	int				m_nFuel;					/// ����ü ���� ����.
	int				m_tmAccFuel;				/// ���ӿ���(�ð�����)
	BOOL			m_bItemFind;				/// ĳ���� �Ŀ� �������� �ִ��� �˻��� ���ΰ�? 
	int				m_nReadyTime;				/// ����ü Ÿ�� ĳ���� �ð�
	OBJID			m_dwUseItemId;				/// ����ü Ÿ�� ������ ���̵� 

	DWORD			m_dwCtrlReadyTime;			/// box open ĳ���� Ÿ��  
	OBJID			m_dwCtrlReadyId;			/// ĳ���� ��� box ���̵�  

	OBJID			m_idAttacker;				/// this�� ���� �����ѳ�
	OBJID			m_idTargeter;				/// this�� Ÿ����� �����ϰ� �ִ³�.
	OBJID			m_idTracking;				/// ������ �ڵ������ϰ� �ִ� Ÿ��.
	OBJID			m_idLastHitMover;			/// this�� ���������� �ƴ� �������̵�
	float			m_fExpRatio;				/// exp ���޷�. this�� �׿����� this�� exp�� ���ۼ�Ʈ�� �ٰ��ΰ�. ����Ʈ�� 1.0�̴�. �̰��� ��Ƽ���� ������� �ʴ´�.
	int				m_nAtkCnt;					/// �����ϸ� ī���Ͱ� 0�̵Ǹ鼭 ��� �����Ѵ�.
	int				m_nReflexDmg;				/// ���÷��� ������ ���� (������)
	LONG			m_nPlusMaxHitPoint;			/// �߰� MAX HP
	DWORD			m_tmPoisonUnit;				/// �� ���� �ð�
	DWORD			m_tmBleedingUnit;			/// ���ʸ��� �ѹ����ΰ�?
	OBJID			m_idPoisonAttacker;			/// �� ������
	OBJID			m_idBleedingAttacker;		/// ���� ������
	short			m_wStunCnt;					/// ���ϻ��� �ð� 
	short			m_wPoisonCnt;				/// ������ �ð� 
	short			m_wPoisonDamage;			/// ������ ���� ������
	short			m_wDarkCnt;					/// ���� ���ӽð�
	short			m_wDarkVal;					/// ���浿�� ���� ���߷� ����ġ
	short			m_wBleedingCnt;				/// ���� ���� �ð� 
	short			m_wBleedingDamage;			/// ������ ���� ������
	short			m_wDarkCover;				/// �Ⱥ��̰� ����

	short			m_nHealCnt;					//sun: 9, 9-10�� �� //	0�� ��츸 �ڵ� ġ��	

//sun: 9, //__AI_0509
	FLOAT	m_fSpeedFactor;
	void	SetSpeedFactor( FLOAT fSpeedFactor );
	FLOAT	GetSpeedFactor( void )	{	return m_fSpeedFactor;	}
#ifdef __WORLDSERVER
	BOOL	IsReturnToBegin( void );
#endif	// __WORLDSERVER

//sun: 10, ���׿���Ŀ AI ����
	BOOL	IsRank( DWORD dwClass )	
		{
			MoverProp* pProp	= GetProp();
			return( pProp && pProp->dwClass == dwClass );
		}

	BYTE			m_nAttackResistLeft;		/// �ڵ� ���������� ���ӳ� ������� ���� ( �޼� �Ӽ� ���ݷ� �߰� ) 
	BYTE			m_nAttackResistRight;		/// �ڵ� ���������� ���ӳ� ������� ���� ( ������ �Ӽ� ���ݷ� �߰� ) 
	BYTE			m_nDefenseResist;			/// �ڵ� ���������� ���ӳ� ������� ���� ( �Ӽ� ���� ) 
	
	u_long			m_idparty;					/// ��Ƽ ��ȣ (Ŭ�󿡼� Ư���� ��찡 �ƴϸ� ����ؼ� �ȵ�  g_Party.m_uPartyId�� ��� )
	u_long			m_idGuild;					/// ��� ��ȣ 
	BOOL			m_bGuildBank;				/// ��� ��ũ�� �̿������� Ȯ��
	DWORD			m_idGuildCloak;				/// ������ ����ȣ�� ������ ����
	u_long			m_idWar;					/// ����� ���� ��ȣ 

	DWORD			m_tmActionPoint;			/// �׼�����Ʈ �ڵ���� Ÿ�̸�(3��) TODO : 19�������� ��� ����.
	DWORD			m_dwTickCheer;				/// ���� Ÿ�̸� 
	int				m_nCheerPoint;				/// Ÿ�ο��� ���� ���� ��  

	DWORD			m_dwTickDuel;				/// ��� ���� Ÿ�̸� 
	DWORD			m_dwTickEndDuel;			/// ��� ���� Ÿ�̸� 
	int				m_nDuelState;				/// ������ 
	int				m_nDuel;					/// ����� ���� ( 0:���� 1:���ε���� 2:��Ƽ����� )
	OBJID			m_idDuelOther;				/// ��� ����� ���̵�
	u_long			m_idDuelParty;				/// ��Ƽ������̸� ���� ��Ƽ�� ���̵� 
	
	int				m_nFame;					/// ��ġ 
	u_long			m_idMurderer;				/// �� ���������� �׿����� �÷��̾� ���̵�
//sun: 8, // __S8_PK
	DWORD			m_dwPKTime;					/// ��ũ ���� �ð�
	int				m_nPKValue;					/// PK ��ġ
	DWORD			m_dwPKPropensity;			/// PK ����
	DWORD			m_dwPKExp;					/// PK ���� ���� ����ġ		
	
	EXPINTEGER		m_nAngelExp;				/// ���� ����ġ
	LONG			m_nAngelLevel;				/// ���� Level

	BYTE			m_nLegend;					//sun: 10�� ���½ý���	Neuz, World, Trans

//sun, 11, Ȯ����ų ȿ������ world,neuz
	DWORD			dwRemoveSfxObj[MAX_SKILLBUFF_COUNT];			// ������ ȿ�� ������Ʈ 
	DWORD			GetRemoveSfxObj(DWORD	dwRemoveSfxObjID);
	void			SetRemoveSfxObj(DWORD	dwRemoveSfxObjID);

//sun: 13, ����
	int				m_nHonor;					// ���μ��� 
	DWORD			m_dwHonorCheckTime;			//���� �ð�üũ
	int				m_aHonorTitle[MAX_HONOR_TITLE];			// ���μ�ġ
	int				GetHonorTitle(int nIdx)	{	return m_aHonorTitle[nIdx];	}
	void			SetHonorCount(int nIdx , int nCount );
	void			CheckHonorStat();
	void			CheckHonorTime();
	void			SetHonorAdd(int nSmallIdx,int nLargeGroup,int nDefault = 0);
	CString			m_strTitle;
	LPCTSTR			GetTitle();
	void			SetTitle(LPCTSTR	pTitle);

#ifdef __CLIENT
	CSfx*			m_pSfxWing;
	DWORD			m_dwWingTime;

	CSfx*			m_pSfxBuffPet;
	void			SetSfxBuffPet( const DWORD idEffect );
#endif //__CLIENT

	DWORD		m_dwMute;

	RESURRECTION_DATA				m_Resurrection_Data;		/// �������� ��ų ���� 
	CItemContainer< FLItemElem  >*	m_ShopInventory[ MAX_VENDOR_INVENTORY_TAB ];	/// ����NPC�� ��ǰ ���� 
	CItemContainer< FLItemElem  >	m_Inventory;				/// �κ��丮

	CPocketController	m_Pocket;	//sun: 11, �ָӴ�

	EQUIP_INFO		m_aEquipInfo[MAX_HUMAN_PARTS];				/// �������� 	
	CVTInfo			m_vtInfo;					/// �ŷ��� ���λ��� ��ȣ 

	BYTE			m_nDBDataSlot;					/// �α��� ȭ���� 3�� ĳ���� ���� ��ȣ 
	BOOL			m_bBank;					/// ���� �̿� ��?
	u_long			m_idPlayerBank[MAX_CHARACTER_SLOT];			/// 3�� ĳ���� idPlayer
	DWORD			m_dwGoldBank[MAX_CHARACTER_SLOT];			/// 3�� ĳ���� ��嵷 
	CItemContainer< FLItemElem >	m_Bank[MAX_CHARACTER_SLOT];		/// 3�� ĳ���� ���� ������ 

	BYTE			m_nQuestKeeping;			/// Ŭ����ũ ��� ����Ʈ ��ȣ  
	BYTE			m_nPartyQuestKeeping;		/// �Ŵ� ���� ����Ʈ ��ȣ 
	size_t			m_nQuestSize;				/// ����Ʈ ���� 
	LPQUEST			m_aQuest;					/// ���� �������� ����Ʈ �迭 
	size_t			m_nCompleteQuestSize;		/// �Ϸ�� ����Ʈ ���� 
	LPWORD   		m_aCompleteQuest;			/// �Ϸ�� ����Ʈ �迭 	
	size_t			m_nCheckedQuestSize;
	LPWORD			m_aCheckedQuest;

	int				m_nDead;					/// ���� �� 5�ʰ��� ����
	int				m_nGuildCombatState;		/// ��� ���� ���� 
	D3DXVECTOR3		m_vMarkingPos;				/// �ε����Ʈ���� ��ŷ�� ��ǥ 
	OBJID			m_idMarkingWorld;			/// �ε����Ʈ���� ��ŷ�� ���� ���̵� 

#ifdef __CLIENT
	static BOOL		m_bQuestEmoticonAdd;		/// ����Ʈ ������ Ȯ�� ���μ��� 
	static FLOAT	m_fQuestEmoticonScale;
	int				m_nQuestEmoticonIndex;
	bool			m_bShowQuestEmoticon;

	NaviPoint		m_nvPoint;
	V_NaviPoint		m_vOtherPoint;
	int				m_idSfxHit;
	BYTE			m_nWaterCircleCount;
	DWORD			m_dwReqFlag;				/// �����û �÷���.
	char			m_szPartyName[32];
	float			m_fDestScale;				/// �ɱ׶�鶧 �ε巴�� �ϱ�����..
	float			m_fDestScaleSlerp;			/// 1.0f ~ 0.0���� ��ȭ��.
	CCooltimeMgr	m_cooltimeMgr;	
	LPDIRECT3DTEXTURE9		m_pCloakTexture;	/// �����ؽ���.

	//sun: �����ڰŸ���... Neuz
	static LPDIRECT3DTEXTURE9		m_pTextureEye[2][MAX_HEAD];
	static LPDIRECT3DTEXTURE9		m_pTextureEyeFlash[2][MAX_HEAD];

#endif // __CLIENT

#ifdef __WORLDSERVER
	CNpcProperty*	m_pNpcProperty;				/// NPC ���� ����( ���, ����Ʈ, �����̸� ) ����� ���� NPC ��ȭó����
	CTimer          m_timerQuestLimitTime;		/// ����Ʈ ���� �ð� Ÿ�̸� 
	DWORD	        m_dwPKTargetLimit;			/// PK�������ѽð�
	DWORD			m_dwTickCreated;			/// ���� �ð� 
//	BOOL			m_bLastPK;					/// ���������� �׾����� �÷��̾��(�������) �����ǰ� �׿��� ���� �����ǰ�.
//	BOOL			m_bLastDuelParty;			/// ��Ƽ���� ���?
	CTime			m_tGuildMember;				/// ��� Ż�� �ð� 
//	BOOL			m_bGuildCombat;				/// ��� ���� ��?
//	OBJID			m_idCollecter;				/// this�� ä��������.
//	int				m_nResource;				/// �ڿ� ������. - ���������� ���.
//	int				m_nCollectOwnCnt;			/// > 0 �λ�Ȳ�� this�� m_idCollecter���̴�.
	CSfxHitArray	m_sfxHitArray;				/// sfx hit ���� �迭 
	map< OBJID, queue< SFXHIT_INFO> >	m_mapSFXInfo;		/// sfx ��ŷ ������ ....	
//	map< OBJID, int>					m_mapSFXCount;		/// sfx�� �߻�Ǹ� ++ 
	map< OBJID, queue< SFXHIT_COUNT > >	m_mapSFXCount;
#endif	// __WORLDSERVER

#ifdef __WORLDSERVER
//private:
	void	DoUseEatPet( FLItemElem* PitemElem );
	void	DoUseSystemPet( FLItemElem* pItemElem );
	void	ActivateEatPet( FLItemElem* pItemElem );
	void	ActivateSystemPet( FLItemElem* pItemElem );
public:
	void	InactivateEatPet( void );
	void	InactivateSystemPet( FLItemElem* pItemElem );

	void	SetPetVisDST( FLItemElem* pItemElem );
	void	ResetPetVisDST( FLItemElem* pItemElem );
	void	ProcessVisPet();

#if defined __WORLDSERVER && defined __ENCHANT_BARUNA16		
public:
	void					SetNPCOwner( const OBJID OwnerOBJID );
	OBJID					GetNPCOwner() const;

	void					DoUseSummonNPC( FLItemElem & ItemElem );
	FLNPCSummoner &			GetBarunaNPCSummoner();

protected:	
	FLNPCSummoner			m_BarunaNPCSummoner;			//NPC ��ȯ��ü ( User��ü�� )

	OBJID					m_NPCOwnerOBJID;					//�� ��ü(NPC)�� ����. (NPC ��ü��)
#endif //__ENCHANT_BARUNA16		

#endif	// __WORLDSERVER

public:
	BOOL	IsUsingEatPet( FLItemElem* pItemElem ) const;	//sun: 12, �� ���� // ������� �����ΰ�?

	BOOL	HasActivatedEatPet( void ) const		{	return m_oiEatPet != NULL_ID;	}
	BOOL	HasActivatedSystemPet( void )	{	return m_dwPetId != NULL_ID;	}

	OBJID	GetEatPetId( void ) const		{	return m_oiEatPet;		}
	void	SetEatPetId( OBJID oiEatPet )		{	m_oiEatPet	= oiEatPet;	}
	DWORD	GetPetId( void ) const	{	return m_dwPetId;	}
	void	SetPetId( DWORD dwPetId )		{	m_dwPetId	= dwPetId;	}
//private:
	DWORD	m_dwPetId;	// ��ȯ���� ���� �κ��丮 ��ġ(�), ��ȯ���� �� �ε���(��)

	int		DoUseItemPetTonic( FLItemElem* pItemElem );	//sun: 12, �� ���� // �� ������ ���	
private:
	BOOL	SetValidNeedVis( FLItemElem* pItemElem, int nPos, vector<BYTE> & vecValid );
public:
	vector<BYTE>	GetValidVisTable( FLItemElem* pItemElem );
	BYTE		IsSatisfyNeedVis( FLItemElem* pItemElemVisPet, PT_ITEM_SPEC ptItemSpecVis );			
	FLItemElem*	GetVisPetItem( void )	{ return m_Inventory.GetAtId( m_objIdVisPet ); }
	void		SetVisPetItem( OBJID objId )	{ m_objIdVisPet = objId; }
	BOOL		HasActivatedVisPet()	{ return m_objIdVisPet != NULL_ID; }
	DWORD		m_dwMoverSfxId;	// ������ �ٴ� ����Ʈ
private:
	OBJID		m_objIdVisPet;	// ��ȯ���� ������ �κ��丮 ��ġ

public:
//sun: 9, 9-10�� ��
	CPet*	GetPet( void );
	FLItemElem*	GetPetItem( void );
	void	PetLevelup( void );
	void	PetRelease( void );

#ifdef __CLIENT
	CClientPet	m_pet;
	CModelObject*	GetPetModel( void )		{	return (CModelObject*)( m_pet.GetObj()? m_pet.GetObj()->m_pModel: NULL );	}
	void	CreatePetSfx( void );

	BOOL	IsOptionRenderMask()	{	return !IsMode( MODE_OPTION_DONT_RENDER_MASK );	}	//sun: 12, �ɼ�â ����
	BOOL	IsOptionRenderCostume(int nCostumeIdx);

#else	// __CLIENT	// __WORLDSERVER
	void	ProcessPetAvail( void );
	void	ProcessPetEnergy( void );
	void	ProcessPetExp( void );
	int		DoUseItemFeedPocket( FLItemElem* pItemElem );
#endif	// __CLIENT

//sun: 11, �ָӴ�
	FLItemElem*	GetItemId2( int nPocket, DWORD dwItemObjID, BOOL bExpiration = TRUE );
	BOOL	CreateItem2( FLItemElem* pItem, int nPocket );
	void	RemoveItem2( DWORD dwItemObjID, int nNum, int nPocket, BOOL bExpiration = TRUE );

//sun: 11, ä�� �ý���
	int						m_nMaxCltTime;
	int						m_nCltTime;
	BOOL	IsCollecting( void )	{	return	m_pActMover->GetActionState() == OBJSTA_COLLECT;	}
	virtual	void	ProcessCollecting( void );
	virtual	void	StartCollecting( void );
	virtual	void	StopCollecting( void );
	FLItemElem*	GetCollector( void );

	int				m_nSkillLevel;				/// ���ݱ��� �ø� ��ų����
	int				m_nSkillPoint;				/// ��ų ���� ����Ʈ ��ġ 

	CTimer			m_SkillTimer;
	BOOL			m_SkillTimerStop;
	
	//sun: 8�� ���� ��ȯ Neuz, World, Trans
	BOOL			m_pAngelFlag;				/// Angel Buff Exist?
	CModelObject*	m_pAngel;					/// Angel Model Object.
	D3DXVECTOR3		m_AngelPos;					/// Angel Position.
	D3DXMATRIX		m_AngelWorldM;				/// Angel Matrix.
	DWORD			m_dwAngelKind;				/// Angel Kind.
	
//sun:10, __EVE_BALLOON
	BOOL			m_pBalloonFlag;				/// Balloon Buff Exist?
	CModelObject*	m_pBalloon;					/// Balloon Model Object.
	D3DXVECTOR3		m_BalloonPos;				/// Balloon Position.
	D3DXMATRIX		m_BalloonWorldM;			/// Balloon Matrix.
	DWORD			m_dwBalloonKind;			/// Balloon Kind.
	float			m_fBalloonYPos;				/// Balloon Y Position
	BOOL			m_bBalloon;

private:
	SET_OBJID		m_idEnemies;				/// ������ ���ݹ��� ���� 

	OBJACT			m_oaCmd;					/// ������Ʈ ���  
	int				m_nCParam[3];				/// ���� ��� �Ķ����
	OBJACT			m_oaAct;					/// ������Ʈ �ൿ����

	int				m_nAParam[5];				/// ���� �ൿ �Ķ����

	LONG			m_adjParamAry[ MAX_ADJPARAMARY ];		/// ���� �Ķ����( ��: hp + ����hp )
	LONG			m_chgParamAry[ MAX_ADJPARAMARY ];		/// ��ü �Ķ����( ��: ��ü hp )

	BOOL			m_bRegenItem;				/// NPC ������ �� ������ ������ �ξ��°� flag  
	int				m_nMovePattern;				/// NPC ���� �̵� Ÿ��
	int				m_nMoveEvent;				/// NPC ���� �̵��� ���̴� ���º�ȭ 99�� ���³�
	int				m_nMoveEventCnt;			/// NPC �����̵��� ���̴� ���º�ȭī����

#ifdef	__CLIENT
	DWORD			m_dwLadolfFlag;
	CModelObject*   m_pLadolf;					/// ������
	int				m_nDmgCnt;					/// ������ ī��Ʈ
	enum WingStateFlag
	{
		FLOATING, FLYING, TURNING_L, TURNING_R
	};

	WingStateFlag	m_eWingStateFlag;		/// �ִϸ��̼� �÷��� ����ġ
	float			m_fOldLengthSq;			/// ���� �ִϸ��̼� �ӵ� ��ȭ���� �������� �����ϱ� ���� ����
	CTimer			m_tmEye[2];				//sun: �����ڰŸ���... Neuz
public:
	BOOL			m_bExclusiveLowBody;
	
	// ������ ���信 �����ҷ��� �õ��� �ڵ�		2016.07.11
	//CModelObject*   m_pCloakWing;					/// ���� ��ġ�� ���� ����
#endif // __CLIENT

private:
	u_long	m_idCampus;			// ���� ���̵�
	int		m_nCampusPoint;		// ���� ����Ʈ
public:
	u_long	GetCampusId()		{	return m_idCampus;	}
	void	SetCampusId( u_long idCampus )		{	m_idCampus = idCampus;	}
	int		GetCampusPoint()	{	return m_nCampusPoint;	}
	void	SetCampusPoint( int nMPPoint )		{	m_nCampusPoint = nMPPoint;	}

#ifdef __PROFESSION
public:
	int		m_nProfessionLevel;
	int		m_nProfessionExp;
#endif // __PROFESSION
//protected:
	int				m_nCount;					/// ������ �������� ���� ������ ī����. ������ �ܿ� 0���� �ʱ�ȭ ���� ����.
	DWORD			m_dwGold;					/// ��� 
	DWORD			m_dwRideItemIdx;			/// ����ü�� ������ �ε���
	CModelObject*	m_pRide;					/// ����ü ��ü ������ 
	TCHAR			m_szName[MAX_NAME];			/// �̸� 
	
public:
	static CMover*  GetActiveMover() { return (CMover*)m_pObjActive; }	// ���ΰ� ��ü ��� 	
	static	int		GetHairCost( CMover* pMover, BYTE nR, BYTE nG, BYTE nB, BYTE nHair );
	static void		UpdateParts( int nSex, int nSkinSet, int nFace, int nHairMesh, int nHeadMesh, PEQUIP_INFO pEquipInfo, CModelObject* pModel, CItemContainer< FLItemElem  >* pInventory, BOOL bIfParts = TRUE, CMover* pMover = NULL );
	static BOOL		DoEquip( int nSex, int nSkinSet, FLItemElem* pItemElem, int nPart, const EQUIP_INFO & rEquipInfo, CItemContainer< FLItemElem  >* pInventory, PEQUIP_INFO pEquipeInfo, CModelObject* pModel, BOOL bEquip, CMover *pMover );
	static	float	GetItemEnduranceInfluence( int nEndurance );	
	static	int		GetItemEnduranceWeight( int nEndurance );	

	virtual	BOOL	SetIndex( LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwIndex, BOOL bInitProp = FALSE, BOOL bDestParam = TRUE );
	virtual BOOL	Read( CFileIO* pFile );
	virtual void	Process();
	virtual	void	Serialize( CAr & ar, int nMethod ); // �ø�������� ; ��Ʈ�� ���¿��� ������ Ŭ���̾�Ʈ, Ŭ���̾�Ʈ ������ �ְ���� ��Ŷ ���� 
	virtual	CModel* LoadModel( LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwType, DWORD dwIndex );
	virtual void	InitProp( BOOL bInitAI = TRUE );		// ��ü�� ������Ƽ �������� �ʱ�ȭ 	
//	virtual int		OnActCollecting();				// User�� ���Ǵ� ���̹Ƿ� FLWSUser���� ã����.
	virtual int		SendDamage( DWORD dwAtkFlag, OBJID idAttacker, int nParam = 0, BOOL bTarget = TRUE ) { return m_pActMover->SendDamage( dwAtkFlag, idAttacker, nParam, bTarget );  }
	virtual int		SendDamageForce( DWORD dwAtkFlag, OBJID idAttacker, int nParam = 0, BOOL bTarget = TRUE ) { return m_pActMover->SendDamageForce( dwAtkFlag, idAttacker, nParam, bTarget ); }	// ������

	void			Init();										// ��ü �ʱ�ȭ 
	void			InitLevel( int nJob, LONG nLevel, BOOL bGamma = TRUE );			// ��ü�� nLevel�� �ٲ���

	void			SetDefaultLowerBody(CModelObject* pModel);

	//////////////////////////////////////////////////////////////////////////
// #ifdef __WORLDSERVER
// public:
// 	void			CorrectErrorsSkillPoint();
// 	int				GetUseSkillPoint() const;
// #endif // __WORLDSERVER
	//////////////////////////////////////////////////////////////////////////

	void			ProcessAniSpeed();
	void			AllocShopInventory( LPCHARACTER pCharacter );
	BOOL			IsVendorNPC();
	void			CheckTickCheer();
	void			SetCheerParam( int nCheerPoint, DWORD dwTickCount, DWORD dwRest );
	void			ClearEquipInfo();
	int				GetMaxPoint(int nDest);
	DWORD			GetReuseDelay( int nIndex );
	BOOL			InitSkillExp();
	void			InitCharacter( LPCHARACTER lpCharacter );	// �̸� �ʱ�ȭ LoadDialogȣ�� 
	LPCHARACTER		GetCharacter();								// 
	void			InitNPCProperty();							// NPC���� ��ũ��Ʈ �ʱ�ȭ 	
	BOOL			LoadDialog();								// dialog ��ũ��Ʈ �ε� 
	void			ProcessRecovery();							// HP, MP, FPȸ���� ó���Ѵ�.
	BOOL			IsActiveMover() { return m_pObjActive == this; }	// ���� ���ΰ� ��ü�ΰ�?
	int				IsSteal( CMover *pTarget );		// pTarget�� ��ƿ�Ϸ� �ϴ°�.
	int				IsSteal( OBJID idTaget );		// id�� �˻��ϴ� ����.
	u_long			GetPartyId() { return m_idparty; }
	BOOL			IsMode( DWORD dwMode ); 
	void			SetMode( DWORD dwMode )		{ m_dwMode |= dwMode; }	// �������� �V��
	void			SetNotMode( DWORD dwMode )	{ m_dwMode &= (~dwMode); } // �������� ���V
	BOOL			SetDarkCover( BOOL bApply, DWORD tmMaxTime = 0 );
	BOOL			SetStun( BOOL bApply, DWORD tmMaxTime = 0 );
	BOOL			SetPoison( BOOL bApply, OBJID idAttacker = NULL_ID, DWORD tmMaxTime = 0, DWORD tmUnit = 0, short wDamage = 0 );	// this�� ���� �ɸ��� �Ѵ�.
	BOOL			SetDark( BOOL bApply, DWORD tmMaxTime = 0, int nAdjHitRate = 0 );	// this�� ������¿� ������ �Ѵ� .
	BOOL			SetBleeding( BOOL bApply, OBJID idAttacker = NULL_ID, DWORD tmMaxTime = 0, DWORD tmUnit = 0, short wDamage = 0 );	// this�� ������Ų��.
	void			RemoveDebuff( DWORD dwState );		// ����� ����
	void			RemoveBuffOne( DWORD dwSkill = 0 );		// ���� �ϳ� ����
	void			RemoveBuffAll();				// ���� ��� ����.
	BOOL			IsStateMode( DWORD dwMode ); 
	void			SetStateMode( DWORD dwMode, BYTE nFlag );		// �������� �V��
	void			SetStateNotMode( DWORD dwMode, BYTE nFlag );	// �������� ���V
	BOOL			IsUseItemReadyTime( PT_ITEM_SPEC pItemProp, OBJID dwObjItemId );
	BOOL			IsMonster();		// NPC & MONSTER
	BOOL			IsCitizen();		// NPC & !MONSTER
	BOOL			IsNPC()				{ return !m_bPlayer; }
	BOOL			IsPlayer() const			{ return m_bPlayer; }
	BOOL			IsEquipableNPC()	{	return( GetCharacter() && GetCharacter()->m_nEquipNum > 0 );	}
	BOOL			IsFlyingNPC()		{ return (m_dwTypeFlag & OBJTYPE_FLYING) ? TRUE : FALSE; }		// ������ ���ΰ�.
	BOOL			IsFly();
	MoverProp*		GetProp() { return prj.GetMoverProp( m_dwIndex ); }	// ��ü�� ������Ƽ ��� 	
	PT_ITEM_SPEC	GetActiveHandItemProp( int nParts = PARTS_RWEAPON );							// �տ� ����� �������� ������Ƽ�� ��� 
	PT_ITEM_SPEC	GetTransyItem( PT_ITEM_SPEC pItemProp, BOOL bCheck = FALSE, LPCTSTR lpszFileName = NULL );
	OBJID			GetDestId()			{ return m_idDest; }
	CCtrl*			GetDestObj()		{ return prj.GetCtrl( m_idDest ); }
	void			SetDestObj( CCtrl* pObj, float fRange = 0.0f );
	D3DXVECTOR3		GetDestPos()		{ return m_vDestPos; }
	void			SetDestPos( CONST D3DXVECTOR3& vDestPos, bool bForward = true, BOOL fTransfer = TRUE );
	void			SetDestPos( CShip *pIAObj, const D3DXVECTOR3 &vDestPos );
	
	void			ClearDestObj();
	void			ClearDestPos()		{ m_vDestPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ); }		
	void			ClearDest();
	void			SetStop()			{ SendActMsg( OBJMSG_STAND ); ClearDestObj(); } // ���� 

	BOOL			IsEmptyDestObj()	{ return m_idDest == NULL_ID; }
	BOOL			IsEmptyDestPos()	{ return m_vDestPos.x == 0.0f && m_vDestPos.z == 0.0f; }
	BOOL			IsEmptyDest()		{ return IsEmptyDestPos() && IsEmptyDestObj(); }
	BOOL			IsRegionAttr( DWORD dwAttribite ) { return ( m_dwRegionAttr & dwAttribite ) == dwAttribite ? TRUE : FALSE; }
	REGIONELEM*		UpdateRegionAttr();
	//REGIONELEM*		UpdateRegion_ToIndun( const DWORD dwWorldID );
	


	DWORD			GetRideItemIdx()	{ return m_dwRideItemIdx; }
	void			SetRide( CModel *pModel, int nRideItemIdx = 0 ) { m_dwRideItemIdx = nRideItemIdx; m_pRide = (CModelObject*)pModel; 	};
	void			ClearDuel();
	void			ClearDuelParty();		
	int				SendActMsg( OBJMSG dwMsg, int nParam1 = 0, int nParam2 = 0, int nParam3 = 0, int nParam4 = 0 ); 		
	void			SendAIMsg( DWORD dwMsg, DWORD dwParam1 = 0, DWORD dwParam2 = 0 );
	void			PostAIMsg( DWORD dwMsg, DWORD dwParam1 = 0, DWORD dwParam2 = 0 );
	BOOL			SetMotion(DWORD dwMotion, int nLoop = ANILOOP_LOOP, DWORD dwOption = 0 );	// MOP_SWDFORCE, MOP_FIXED, MOP_NO_TRANS, MOP_HITSLOW
	BOOL			InitMotion(DWORD dwMotion);
	void			SetDamagedMotion( CMover* pAttacker, DWORD dwAtkFlags );
	FLItemElem*		GetWeaponItem( int nParts = PARTS_RWEAPON );					// ������ ���� ��� 
	FLItemElem*		GetLWeaponItem();					// �޼տ� ������ ���� ���.
	FLItemElem*		GetEquipItem( int nParts );			// ������ ������ ��� 
	PT_ITEM_SPEC	GetEquipItemProp( CItemContainer<FLItemElem>* pInventory, PEQUIP_INFO pEquipInfo, int nParts );
	BOOL			IsDualWeapon();
	void			RedoEquip( BOOL fFakeParts, BOOL bDestParam = TRUE );	
	void			UpdateParts( BOOL bFakeParts  = FALSE ); // normal or fake
	int				InvalidEquipOff( BOOL fFakeparts );		// �Žñ��� ��� ����.
	BOOL			DoEquip( FLItemElem* pItemElem, BOOL bEquip, int nPart = -1 ); // for normal

	BOOL			IsEquipAble( FLItemElem* pItem,BOOL bIgnoreLevel = FALSE );						//sun: 11, ����, �ູ // ���������Ѱ�?	

	BOOL			IsUnEquipAble( PT_ITEM_SPEC pItemProp );					// ���°� �����Ѱ�?
	void			SetEquipDstParam();
	void			SumEquipAdjValue( int* pnAdjHitRate, int* pnAdjParray );
	int				SumEquipDefenseAbility( LONG* pnMin, LONG* pnMax );
	void			SetDestParamEquip( PT_ITEM_SPEC pItemProp, FLItemElem* pItemElem, BOOL bIgnoreSetItem = FALSE );
	void			ResetDestParamEquip( PT_ITEM_SPEC pItemProp, FLItemElem* pItemElem );
	LPQUEST			FindQuest( int nQuestId );
	LPQUEST			GetQuest( int nQuestId );
	BOOL			RemoveQuest( int nQuestId );
	BOOL			IsCompleteQuest( int nQuestId );
	BOOL			MakeCompleteQuest( int nQuestId, LPQUEST lpQuest );
	BOOL			SetQuest( int nQuestId, int nState, LPQUEST lpReturnQuest );
	BOOL			SetQuest( LPQUEST lpQuest );
	void			RemoveAllQuest();
	void			RemoveCompleteQuest();

#ifdef	__WORLDSERVER
	DWORD			m_dwDisguiseID;
#endif
	BOOL            IsDisguise();
	BOOL			NoDisguise( LPDIRECT3DDEVICE9 pd3dDevice = NULL );
	BOOL			Disguise( LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwMoverIndex );
	BOOL			IsAuthorization( DWORD dwAuthorization ) { return dwAuthorization == m_dwAuthorization; }
	BOOL			IsAuthHigher( DWORD dwAuthorization ) { return dwAuthorization <= m_dwAuthorization; }
	void			UpdateParam();		
	int				GetParam( int nDestParameter, int nParam = 0 );
	void			SetDestParam( int nDstParameter, int nAdjParameterValue, int nChgParameterValue, BOOL fSend = TRUE );
	void			SetDestParam( int nItemIdx2, BOOL fSend = TRUE );
	void			SetDestParam( int nIdx, SkillProp* pProp, BOOL bSend = TRUE );
	
	void			ResetDestParam( int nDstParameter, int nAdjParameterValue, BOOL fSend = TRUE );

	void			ResetDestParamSync( int nDstParameter,int nAdjParameterValue, int nParameterValue, BOOL fSend = TRUE );

	void			ResetDestParam( int nIdx, SkillProp* pProp, BOOL bSend = TRUE );
	
	int				GetAdjParam( int nDestParameter );
	int				GetChgParam( int nDestParameter );
	int				GetPointParam( int nDstParameter );
	void			SetPointParam( int nDstParameter, int nValue, BOOL bTrans = FALSE ); // bTrans�� TRUE�̸� ��������
	void			OnApplySM();
	LPSKILL			GetSkill( int nType, int nIdx );
	LPSKILL			GetSkill( DWORD dwSkill );
	int				GetSkillIdx( DWORD dwSkill );
	BOOL			CheckSkill( DWORD dwSkill );
	bool			isLearnSkill( const int job_skill_array_index ) const;
	void			SetHair( int nHair );
	void			SetHairColor( DWORD dwHairColor );
	void			SetHairColor( FLOAT r, FLOAT g, FLOAT b );
	void			SetHead( int nHead );
	void			SetSkinSet( int nSkinSet );
	void			SetName( const char* lpszName )		{	FLStrcpy( m_szName, _countof( m_szName ), lpszName );	}
	LPCTSTR			GetName( BOOL bNickname = FALSE );	// ��ü�� �̸� ���
	BYTE			GetSex() { return m_bySex; }
	void			SetSex( BYTE bySex ) { m_bySex = bySex; }
	BOOL			IsPeaceful() { return m_dwBelligerence == BELLI_PEACEFUL; }
	BOOL			IsBaseJob();
	BOOL			IsExpert();
	BOOL			IsPro();
	BOOL			IsInteriorityJob( int nJob );
	BOOL			SetExpert( int nExpert );
	BOOL			AddChangeJob( int nJob );
	int				GetJob();
	int				GetExpPercent();
	int				SetLevel( int nSetLevel );
	int				AddGPPoint( int nAddGPPoint );
	void			SetJobLevel( int nLevel, int nJob );
	BOOL			IsJobType( DWORD dwJobType ); 
	int				GetLevel() const { return m_nLevel; }
	int				GetDeathLevel() const { return m_nDeathLevel; }
	int				GetFxp() { return m_nFxp; }
	int				GetTxp() { return m_nFxp; }
	EXPINTEGER		GetExp1()	{	return m_nExp1;	}
	EXPINTEGER		GetMaxExp1()	{	return prj.m_aExpCharacter[m_nLevel+1].nExp1;	}
	EXPINTEGER		GetMaxExp2()	{	return prj.m_aExpCharacter[m_nLevel].nExp2;	}
	int				GetRemainGP(); // growth
	void			IncHitPoint( int nVal) ;
	void			IncManaPoint(int nVal) ;
	void			IncFatiguePoint(int nVal) ;
	void			SetHitPoint( int nVal) ;
	void			SetManaPoint( int nVal ) ;
	void			SetFatiguePoint( int nVal ) ;
#ifdef __WORLDSERVER
	float			GetExpFactor();
#endif // __WORLDSERVER
	float			GetItemDropRateFactor( CMover* pAttacker );
	float			GetPieceItemDropRateFactor( CMover* pAttacker );
	BOOL			AddExperience( EXPINTEGER nExp, BOOL bFirstCall = TRUE, BOOL bMultiply = TRUE, BOOL bMonster = FALSE );	// bMultiply : ���ȭ ������ ����? // bMonster : ���͸� �׿��� �� ����ġ �ΰ�?
//	BOOL			DecExperience( EXPINTEGER nExp, BOOL bExp2Clear, BOOL bLvDown );	// ����ġ�� ��´�.
//	BOOL			DecExperiencePercent( float fPercent, BOOL bExp2Clear, BOOL bLvDown  );	// ����ġ�� �ۼ�Ʈ�� ��´�.
	BOOL			AddFxp( int nFxp );
	BOOL			SetFxp( int nFxp, int nFlightLv );

	BOOL			IsPVPTarget( CMover* pMover );
	BOOL			IsWarTarget( CMover* pMover );
	BOOL			IsSchoolTarget( CMover* pMover );
	HITTYPE			GetHitType( CMover* pMover, BOOL bTarget, int nReflect );

	BOOL	IsArenaTarget( CMover* pMover );
	BOOL			IsEventArenaTarget( CMover* pMover );

//sun: 8, // __S8_PK
	HITTYPE			GetHitType2( CMover* pMover, BOOL bTarget, BOOL bGood );

	void			ReSetDuelTime( CMover* pAttacker , CMover* pDefender);
	int				GetHR();
	int				GetStr();
	int				GetDex();
	int				GetInt();
	int				GetSta();
	FLOAT			GetSpeed(FLOAT fSrcSpeed);    
	int				GetGold();

	bool			HasZeroSpeedStateParam_hardcoding();


	//////////////////////////////////////////////////////////////////////////
	// mirchang_20100705
	bool			CheckUserGold( int nGold, bool bAdd );
	//////////////////////////////////////////////////////////////////////////


//sun: 12, ����
	int		GetPerinNum( void );
	__int64		GetTotalGold( void );
#ifdef __WORLDSERVER
	int		RemoveTotalGold( __int64 iGold );	// ���ŵ� �丰 ����
	int		RemovePerin( int nPerin );
#endif	// __WORLDSERVER

	void			SetGold( int nGold );
	BOOL			AddGold( int nGold, BOOL bSend = TRUE );
	int				GetHitPointPercent( int nPercent = 100 );
	int				GetManaPointPercent( int nPercent = 100 );
	int				GetFatiguePointPercent( int nPercent = 100 );
	int				GetHitPoint();
	int				GetManaPoint();
	int				GetFatiguePoint();
	int				GetMaxHitPoint();
	int				GetMaxManaPoint();
	int				GetMaxFatiguePoint();
	int				GetMaxOriginHitPoint( BOOL bOrinal = TRUE );
	int				GetMaxOriginManaPoint( BOOL bOrinal = TRUE );
	int				GetMaxOriginFatiguePoint( BOOL bOrinal = TRUE );
	int				GetHPRecovery();
	int				GetMPRecovery();
	int				GetFPRecovery();
	int				GetNaturalArmor();
 	float			GetAttackSpeed();
	float			GetCastingAniSpeed();
	int				GetCastingTime( int nCastingTime );

	//////////////////////////////////////////////////////////////////////////
	// ��� ����
	float			GetDamageMultiplier( ATTACK_INFO* pInfo );
	int				PostCalcMagicSkill( int nATK, ATTACK_INFO* pInfo );
	int				PostCalcGeneric( int nATK, ATTACK_INFO* pInfo );
	float			GetMagicSkillFactor( CMover* pDefender, SAI79::ePropType skillType );
	float			GetATKMultiplier( CMover* pDefender, DWORD dwAtkFlags );
	float			GetDEFMultiplier( ATTACK_INFO* pInfo );
	float			GetBlockFactor( CMover* pAttacker, ATTACK_INFO* pInfo );
	int				GetWeaponATK( DWORD dwWeaponType );
	int				GetPlusWeaponATK( DWORD dwWeaponType );
	int				GetWeaponPlusDamage( int nDamage, BOOL bRandom = TRUE );
	void			GetDamagePropertyFactor( CMover* pDefender, int* pnATKFactor, int* pnDEFFactor, int nParts );
	int				GetPropATKPlus( int nParts );
	int				GetPropDEFPlus();
	void			GetDamageRange( int& nMin, int& nMax );
	BOOL			IsBlocking( CMover* pAttacker );
	BOOL			CanFlyByAttack();
	BOOL			IsCriticalAttack( CMover* pDefense, DWORD dwAtkFlags );
	int				GetCriticalProb();

	int				GetReqMp( int nReqMp );
	int				GetReqFp( int nReqFp );
	bool			isUnconditionedHit( CMover* pDefender ) const;
	int				calculateHitRate( CMover* pDefender );
	BOOL			GetAttackResult( CMover* pHitObj );
	int				GetAdjHitRate();
	int				GetHitPower( ATTACK_INFO* pInfo );
	int				GetRangeHitPower( ATTACK_INFO* pInfo );
	int				GetParrying();
	int				GetDefenseByItem( BOOL bRandom = TRUE );
	int				GetShowDefense( BOOL bRandom );
	int				GetResistMagic();
	int				GetResistSpell( int nDestParam );
	int				GetMeleeSkillPower( ATTACK_INFO* pInfo );	// �������� ��ų������
	int				GetMagicHitPower( int nMagicPower );			// �ϵ���� ������
	int				GetItemAbility( int nItem );
	int				GetItemAbilityMin( int nItem );
	int				GetItemAbilityMax( int nItem );
	void			GetHitMinMax( int* pnMin, int* pnMax, ATTACK_INFO *pInfo = NULL );
	BOOL			IsAfterDeath();
	BOOL			IsDie() const { return m_pActMover->IsDie() || m_nHitPoint == 0; }
	BOOL			IsLive() { return m_pActMover->IsDie() == FALSE || m_nHitPoint > 0; }		// && ��  ||�� �ٲ���.  !=�� >�� �ٲ�
	int				GetCount() { return m_nCount; }
//sun: 8, // __S8_PK
	void			SetPKPink( DWORD dwTime ) { if( dwTime == 0 || m_dwPKTime < dwTime ) m_dwPKTime = dwTime; }
	DWORD			GetPKPink( void ) { return m_dwPKTime; }
	BOOL			IsPKPink( void )	{ return m_dwPKTime > 0; }			/// PK ��ũ �������
	BOOL			IsChaotic( void ) { return m_dwPKPropensity > 0; }	/// ī������
	void			SetPKValue( int nValue );
	int				GetPKValue( void ) { return m_nPKValue; }
	void			SetPKPropensity( DWORD dwValue );
	DWORD			GetPKPropensity( void ) { return m_dwPKPropensity; }
	DWORD			NextPKPropensity( int nPKValue );

	float			GetResist(SAI79::ePropType p_PropType);
	int				GetSetItemParts(DWORD dwParts);
	int				GetSetItemClient();
	int				GetSetItem( FLItemElem* pItemElem = NULL );
	BOOL			IsSetItemPart( DWORD dwParts );
	void			SetSetItemAvail( int nAbilityOption );
	void			ResetSetItemAvail( int nAbilityOption );
	void			DestParamPiercingAvail( FLItemElem* pItemElem, BOOL bSET = TRUE );
#ifdef COSTUME_UPGRADE_ENHANCEMENT_GEM
	void			DestParamGemAvail( FLItemElem* pItemElem, BOOL bSET );
#endif
	BOOL			Pierce( FLItemElem *pItemElem, const DWORD dwPierceItemID );
	void			SetDestParamSetItem( FLItemElem* pItemElem );
	void			ResetDestParamSetItem( FLItemElem* pItemElem );
	int				GetEquipedSetItemNumber( CSetItem* pSetItem, FLItemElem* pUnequipItem = NULL );
	void			GetEquipedSetItem( CSetItem* pSetItem, int* pbEquipedCount,  int* pnTotal, int* pnEquip );
	void			SetDestParamRandomOpt( FLItemElem* pItemElem );
	void			ResetDestParamRandomOpt( FLItemElem* pItemElem );
	
	void	SetDestParamRandomOptOrigin( FLItemElem* pItemElem );
	void	SetDestParamRandomOptExtension( FLItemElem* pItemElem );
	void	ResetDestParamRandomOptOrigin( FLItemElem* pItemElem );
	void	ResetDestParamRandomOptExtension( FLItemElem* pItemElem );

	void			GetItemATKPower( int *pnMin, int *pnMax, PT_ITEM_SPEC pItemProp, FLItemElem *pWeapon ); 
	float			GetItemMultiplier( FLItemElem* pItemElem );
	BOOL			SubLootDropNotMob( CItem *pItem );
	BOOL			SubLootDropMobSingle( CItem *pItem );
	BOOL			SubLootDropMobParty( CItem *pItem, CParty *pParty );
	BOOL			SubLootDropMob( CItem *pItem );
	BOOL			DoLoot( CItem *pItem );		// �ٴڿ� ������ pItem�� �ݴ´�.
	void			PickupGoldCore( int nGold );
	void			PickupGold( int nGold, BOOL bDropMob );
	BOOL			IsDropable( FLItemElem* pItemElem, BOOL bPK );
 	BOOL			IsDropableState( BOOL bPK );
	CItem*			DropGold( DWORD dwGold, const D3DXVECTOR3& vPos, BOOL bPK = FALSE );

	void			UpdateItem( DWORD dwId, CHAR cParam, DWORD dwValue, DWORD dwTime = 0 );
//	void			UpdateItemEx( unsigned char id, char cParam, __int64 iValue );		//sun: 11, ����, �ູ

	CItem*			_DropItemNPC( DWORD dwItemType, DWORD dwID, int nDropNum, const D3DXVECTOR3 &vPos );
	CItem*			DropItem( DWORD dwID, int nDropNum, const D3DXVECTOR3 &vPos, BOOL bPlayer = FALSE );
	int				DoDropItemRandom( BOOL bExcludeEquip, CMover* pAttacker, BOOL bOnlyEquip = FALSE );
	int				GetItemNum( DWORD dwItemId );
#ifdef __CLIENT
	int				GetItemNumForClient( DWORD dwItemId ); // Client������ ����ϴ� ������ ���� ���ϱ�(Null check �̿��� Usable check����)
#endif //__CLIENT
	int				RemoveAllItem( DWORD dwItemId );
	BOOL			AddItem( FLItemBase* pItemBase );
	FLItemBase*		GetItemId( DWORD dwId );
	void			RemoveItemId( DWORD dwId  );
	void			SetKeeptimeInven( DWORD dwItemId, DWORD dwTime );
	void			SetKeeptimeBank( DWORD dwItemId, DWORD dwTime );
	void			SetKeeptimeItem( FLItemElem* pItemElem, DWORD dwTime );
	void			OnTradeRemoveUser();
	BOOL			AddItemBank( int nSlot, FLItemElem* pItemElem );
	void			UpdateItemBank( int nSlot, DWORD dwItemObjID, CHAR cParam, DWORD dwValue );
	FLItemBase*		GetItemBankId( int nSlot, DWORD dwId );
	void			RemoveItemBankId( int nSlot, DWORD dwId );
	void			GenerateVendorItem( T_ITEM_SPEC** pItemProp, int* pcbSize, int nMax, LPVENDOR_ITEM pVendor );
	BOOL			DropItemByDied( CMover* pAttacker );		// �׾ ����Ʈ���� ���.
	BOOL			OnDie( CMover & kAttacker );

	void			SetPKTargetLimit( int nSec );
	void			ClearCmd(); 
	OBJACT			GetCmd() { return m_oaCmd; }
	OBJACT			GetAct() { return m_oaAct; }
	int				GetCmdParam( int nIdx ) { return m_nCParam[nIdx]; }	
	void			ClearActParam(); 
	int				GetEnemyCount();
	OBJID			GetMaxEnemyHitID();
	DWORD			AddEnemy( OBJID objid, int nHit );
	int				GetEnemyHit( OBJID objid, DWORD* pdwTick = NULL );
	void			RemoveEnemy( OBJID objid  );
	void			RemoveAllEnemies();
	BOOL			IsAttackAble( CObj *pObj );
	FLOAT			GetAttackRange( DWORD dwAttackRange );
	BOOL			IsAttackAbleNPC( CMover* pNPC );
	BOOL			IsPKInspection( CMover* pMover );
	int				IsPKPVPInspectionBase( DWORD dwRegionAttr, BOOL bPVP = TRUE );
//sun: 8,     // 8�� ������� ������� PVP�����ϰ���   Neuz, World
	int				IsPVPInspectionBase( DWORD dwRegionAttr,DWORD dwWorldID, BOOL bPVP = TRUE );//8�� ������� ������� PVP�����ϰ���   Neuz, World

	BOOL			SubPKPVPInspectionBase( CMover* pMover, CMover* pMover1, DWORD dwPKAttr, int nFlag );
	void			PrintString( CMover* pMover, DWORD dwId );
	int				CMD_SetUseSkill( OBJID idTarget, int nSkillIdx, SKILLUSETYPE sutType = SUT_NORMAL );
	void			CMD_SetMeleeAttack( OBJID idTarget, FLOAT fRange = 0.0f );
	void			CMD_SetMagicAttack( OBJID idTarget, int nMagicPower );
	void			CMD_SetRangeAttack( OBJID idTarget, int nPower );
	void			CMD_SetMoveToPos( const D3DXVECTOR3 &vPos );
	void			CMD_SetMoveToObj( OBJID idObj );
	void			CMD_SetUseItem( CCtrl *pCtrl );
//	void			CMD_SetCollect( CObj *pObj );
	void			OnAfterUseItem( const PT_ITEM_SPEC pItemProp );
	void			UnequipRide();		
	void			EquipItem( FLItemElem *pItemElem, BOOL bEquip, int nPart );
	BOOL			DoUseItemVirtual( DWORD dwItemId, BOOL bEffectSkip );

//	int				GetCountApplyESScroll();
//	bool			HasBuffESScroll();

	void			DoPickupItemAround();
	int				DoAttackMagic( CObj *pTargetObj, int nMagicPower,int idSfxHit = 0 );
	int				DoAttackRange( CObj *pTargetObj, DWORD dwItemID, int idSfxHit );
	int				DoAttackSP( CObj *pTargetObj, DWORD dwItemID );
	int				DoAttackMelee( CMover *pTarget, OBJMSG dwMsg = OBJMSG_ATK1, DWORD dwItemID = 0 );
	int				DoAttackMelee( OBJID idTarget, OBJMSG dwMsg = OBJMSG_ATK1, DWORD dwItemID = 0 );
	int				DoDie( CCtrl *pAttackCtrl, DWORD dwMsg = 0 );	// ������.	
	void			ChangeFame( CMover *pLose );
	int				DuelResult( CMover *pLose );
	PVP_MODE		GetPVPCase( CMover *pAttacker );
	void			SubPVP( CMover *pAttacker, int nReflect );	// ����� ����� �ο򿡼� �׾�����.
	int				SubDuel( CMover *pAttacker );	// ����߿� �׾����� 
	int				SubPK( CMover *pAttacker, int nReflect );		// PK������ �׾�����.
	int				SubWar( CMover *pAttacker );	// ���ﶧ �׾���
	int				SubExperience( CMover *pDead );
	void			AddKillRecovery();
	BOOL			IsValidArea( CMover* pMover, float fLength );
	void			SetJJim( CMover *pJJimer );		// this�� pJJimer�� ���ߴٴ°� ǥ��.
	void			SubReferTime( DWORD *pTime1, DWORD *pTime2, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp );

	BOOL			GetSkillProp( SkillProp **ppSkillProp, AddSkillProp **ppAddSkillProp, int nSkill, DWORD dwLevel, LPCTSTR szErr );
	void			RemoveInvisible();		// ����ȭ ���°ɷ������� ������.
	void			SendDamageAround( int nDmgType, CMover *pAttacker, int nApplyType, int nAttackID, float fRange, float fTargetRatio, float fAroundRatio );	// this�� �߽����� �ֺ��� �������� ����.
	void			SendDamageAroundByItem( int nDmgType, CMover *pAttacker, int nApplyType, int nAttackID, float fRange, float fTargetRatio, float fAroundRatio );
	void			ChangeExpRatio( CMover* pAttacker, CMover* pDefender );
	void			OnAttacked( CMover* pAttacker, int nDamage, BOOL bTarget, int nReflect );
	BOOL			OnDamage( int nItemID, DWORD dwState, CMover *pHitObj, const D3DXVECTOR3 *pvDamagePos = NULL, DWORD dwAtkFlag = AF_GENERIC );

	void			SetCoolTime( AddSkillProp* pAddSkillProp, LPCTSTR szCall );	//sun: 9, ���ο� AddSkillProp �÷� �߰� �� ����

	BOOL			OnAttackRange();		// ��Ÿ����Ⱑ �߻�Ǵ� ������ �߻��ϴ� �̺�Ʈ
	BOOL			OnAttackMelee( DWORD dwState, CMover *pHitObj );		// �ٰŸ����ݽ� Ÿ���� �Ǿ����� �߻��ϴ� �̺�Ʈ.
	BOOL			OnAttackMeleeContinue( DWORD dwState );		// ù��° Ÿ�����ĺ��� �ִϳ��������� ���.
	void			OnAttackMelee_ClockWorks( DWORD dwState, CMover *pHitObj );
	void			OnAttackMelee_BigMuscle( DWORD dwState, CMover *pHitObj );	
	void			OnAttackMelee_Krrr( DWORD dwState, CMover *pHitObj );	
	void			OnAttackMelee_Bear( DWORD dwState, CMover *pHitObj );	
	void			OnAttackMelee_Meteonyker( DWORD dwState, CMover *pHitObj );	//sun: ���׿���Ŀ ���̾� �߻�!!!
	BOOL			OnAttackMagic();		// �ϵ������ �߻�Ǵ� ������ �߻��ϴ� �̺�Ʈ.
	BOOL			OnAttackSP();			// Ư������.
	void			OnActDrop();			// ĳ���Ͱ� ���������� �������� �����ϴ� ������ �߻�.
	void			OnActCollision();		//
	void			OnActEndMeleeSkill();	// ������ų ������ ������ ������ �߻�.
	void			OnActEndMagicSkill();	// ������ų ������ ������ ������ �߻�.
	void			OnEndMeleeAttack( DWORD dwState );		// �Ϲݵ��� ���� ������ �߻�(����).
	void			OnActEndJump4();		// ���� �÷��װ� ������� ����.
	void			OnActLanding();		// ���� �ϴ� ����.
	void			OnActFMove();
	void			OnActIALanding( CObj *pIAObj, const D3DXVECTOR3 &vPos );		// Interact������Ʈ�� ����������.
	void			OnAttackStart( CMover *pTarget, OBJMSG dwMsg );					// ������ �� ����������(�Ϲ�/��ų/�ϵ� ��)

	BOOL			IsBullet( PT_ITEM_SPEC pItemProp );
	BOOL			IsBullet( SkillProp* pSkillProp );
	void			ProcessMovePattern();
	int				GetMovePattern() { return m_nMovePattern; }
	int				GetMoveEvent() { return m_nMoveEvent; }
	void			SetMovePattern( int nPattern );		// �̵����� ����.
//	BOOL			DoCollect( CMover *pTarget );
	CGuild*			GetGuild();
	CGuildWar*		GetWar();
	BOOL			IsSMMode( int nType ) { return ( m_dwSMTime[nType] > 0 ) ? TRUE : FALSE; }
	void			ReState();
	void			ReStateOne( int nKind );	//sun: 10, __S_ADD_RESTATE

	void			ReStateOneLow( int nKind );

	float			GetJobPropFactor( JOB_PROP_TYPE type );
#ifdef ADD_CHARACTER_INFO_DISPLAY
	int				CalcDefense( ATTACK_INFO* pInfo, BOOL bRandom = TRUE, BOOL bItemDivision = TRUE );
	int				CalcDefenseCore( CMover* pAttacker, DWORD dwAtkFlags, BOOL bRandom = TRUE, BOOL bItemDivision = TRUE);	
#else
	int				CalcDefense( ATTACK_INFO* pInfo, BOOL bRandom = TRUE );
	int				CalcDefenseCore( CMover* pAttacker, DWORD dwAtkFlags, BOOL bRandom = TRUE );
#endif
	int				CalcDefensePlayer( CMover* pAttacker, DWORD dwAtkFlags );
	int				CalcDefenseNPC( CMover* pAttacker, DWORD dwAtkFlags );
	void			PutLvUpSkillName_1( DWORD dwLevel );
	void			PutLvUpSkillName_2( DWORD dwSkill );
	BOOL			IsRegionMove( DWORD dwOlgRegionAttr, DWORD dwRegionAttr );
	DWORD			GetPKPVPRegionAttr();
	FLItemBase*		GetVendorItem();		
	BOOL			IsAttackMode();
	void			AddSkillPoint( int nPoint);
	
//sun: 8,     // 8�� ������� ������� PVP�����ϰ���   Neuz, World
	void			DoPVPEnd( CCtrl *pAttackCtrl, bool bWinner , DWORD dwMsg = 0 );	// ��󳡳��� ó��
	void			EndPVP(int	nPVPHP);	// ��󳡳��� ó��
	BOOL			m_bPVPEnd;			//����� �����°�

//sun: 10�� ���½ý���	Neuz, World, Trans
	BYTE			GetLegendChar()	;
	void			SetLegendChar(	BYTE	nLegend	)	{	m_nLegend	=	nLegend;	}
	BOOL			IsMaster();
	BOOL			IsHero();
	BOOL			IsLegendClass();	// ����� �̻��̳�..

	BOOL			IsLegendHero();

	void			SetMasterSkillPointUp();


	DWORD			GetJobType( int nJob = NULL_ID )	{	if( nJob == NULL_ID ) nJob = m_nJob;	return prj.m_aJob[ nJob ].dwJobType;	}	// ���� Ÿ��

	void			AngelMoveProcess();		//sun: 8�� ���� ��ȯ Neuz, World, Trans
	void			BalloonMoveProcess();	//sun:10, __EVE_BALLOON

//sun: 8, // __S8_PK
	void			OnAttackSFX( OBJID	idTarget, int nMagicPower, DWORD dwSkill, int nDmgCnt, float	fDmgAngle, float fDmgPower,  DWORD dwAtkFlags, BOOL bControl );

	BOOL			IsGuildCombatTarget( CMover* pMover );
#ifdef __WORLDSERVER
	void	ProcessSFXDamage( void );
	void	ProcessSFXExpire( void );
#endif	// __WORLDSERVER

	void	ProcessPet( void );		//sun: 9, 9-10�� ��

#ifdef __WORLDSERVER
	void			SetDestObj( OBJID idObj, float fRange = 0.0f, BOOL bSend = FALSE );
#else
	void			SetDestObj( OBJID idObj, float fRange = 0.0f, BOOL bSend = TRUE );
#endif

#if defined(__WORLDSERVER)
	BOOL			IsPVPInspection( CMover* pMover, int nFlag = 0 );
	void			AddExperienceKillMember( CMover *pDead, EXPFLOAT fExpValue, MoverProp* pMoverProp, float fFxpValue );
	void			AddExperienceSolo( EXPFLOAT fExpValue, MoverProp* pMoverProp, float fFxpValue, BOOL bParty );
	void			AddExperienceParty( CMover *pDead, EXPFLOAT fExpValue, MoverProp* pMoverProp, float fFxpValue, CParty* pParty, FLWSUser* apMember[], int* nTotalLevel, int* nMaxLevel10, int* nMaxLevel, int* nMemberSize );
	void			AddExperiencePartyContribution( CMover *pDead, FLWSUser* apMember[], CParty* pParty, EXPFLOAT fExpValue, float fFxpValue, int nMemberSize, int nMaxLevel10 );
	void			AddExperiencePartyLevel( FLWSUser* apMember[], CParty* pParty, EXPFLOAT fExpValue, float fFxpValue, int nMemberSize, int nMaxLevel10 );
	BOOL			GetPartyMemberFind( CParty* pParty, FLWSUser* apMember[], int* nTotalLevel, int* nMaxLevel10, int* nMaxLevel, int* nMemberSize );
	float			GetExperienceReduceFactor( int nLevel, int nMaxLevel );
//	BOOL			IsResourceMonster() { return m_nResource != -1; }		// �ڿ����ͳ�? -1�̸� �ڿ����Ͱ� �ƴϴ�. 0 ~ �ڿ����Ͷ�� ��.
	void			ArrowDown( int nCount );
	int				GetQueueCastingTime();

	DWORD			DoUseSkill( const int nType, const int nIdx, const OBJID idFocusObj, const SKILLUSETYPE sutType, const BOOL bControl );
//	float			SubDieDecExp( BOOL bTransfer = TRUE, DWORD dwDestParam = 0, BOOL bResurrection = FALSE  );	// �׾����� ��ġ ��� �κ�.

	void			SubAroundExp( CMover *pAttacker, float fRange );		// this�� �߽����� fRange�����ȿ� �ִ� �������� ����ġ�� ����Ѵ�.
	void			AddPartyMemberExperience( FLWSUser * pUser, EXPINTEGER nExp, int nFxp );
//	void			GetDieDecExp( int nLevel, FLOAT& fRate, FLOAT& fDecExp, BOOL& bPxpClear, BOOL& bLvDown );

//	void			GetDieDecExpRate( FLOAT& fDecExp, DWORD dwDestParam, BOOL bResurrection );

	BOOL			CreateItem( FLItemBase* pItemBase, DWORD* pItemObjID = NULL, int* pQuantity = NULL, DWORD* pCount = NULL );
	void			RemoveItem( DWORD dwItemObjID, int nNum );
	int				RemoveItemByItemID( const DWORD dwItemID, const int nTotalQuantity, const TCHAR* pszLogAction = NULL, const TCHAR* pszLogContext = NULL );
	int				RemoveItemA( DWORD dwItemId, int nNum );
	void			RemoveVendorItem( CHAR chTab, DWORD dwItemObjID, int nNum );
	void			RemoveItemIK3( DWORD dwItemKind3 );
	BOOL			DoUseItemSexChange( int nFace );
	BOOL			ReplaceInspection( REGIONELEM* pPortkey );
#ifdef __LAYER_1015
	BOOL			Replace( u_long uIdofMulti, DWORD dwWorldID, const D3DXVECTOR3& vPos, REPLACE_TYPE type, int nLayer );
#else	// __LAYER_1015
	BOOL			Replace( u_long uIdofMulti, DWORD dwWorldID, const D3DXVECTOR3& vPos, REPLACE_TYPE type );
#endif	// __LAYER_1015

	BOOL			IsLoot( CItem *pItem, BOOL bPet = FALSE );
	void			ProcInstantBanker();
	void			ProcInstantGC();
	BOOL			IsItemRedyTime( PT_ITEM_SPEC pItemProp, OBJID dwObjid, BOOL bItemFind );
	int				ApplyDPC( int ATK, ATTACK_INFO* pInfo );
	int				CalcLinkAttackDamage( int nDamage ); // ��Ƽ��ų:��ũ������ �߰� ���̹̰�� 	
	int				CalcPropDamage( CMover* pDefender, DWORD dwAtkFlags );
	int				CalcGenericDamage( CMover* pDefender, DWORD& dwAtkFlags );
	int				GetMagicSkillPower( ATTACK_INFO* pInfo );
	void			SubSMMode();
	void			ClearAllSMMode();
	BOOL			SetSMMode( int nType, DWORD dwTime );
	void			Abrade( CMover* pAttacker, int nParts = PARTS_RWEAPON );
	DWORD			GetRandomPartsAbraded();
	void			SetMarkingPos();
	void			RemoveSFX( OBJID idTarget, int id, BOOL bForce, DWORD dwSkill );
	int				GetSFXCount( OBJID idTarget );
	void			IncSFXCount( OBJID idTarget, DWORD dwSkill );
	void			AddSFXInfo( OBJID idTarget, SFXHIT_INFO& info );
	void			ClearSFX( OBJID idTarget );
	int				GetSummonState();
	BOOL			IsDoUseBuff( PT_ITEM_SPEC pItemProp );
#ifdef __AGGRO16
	FLAggro *		GetAggroMng();
#endif //__AGGRO16

	void			ClearAbnormalState();

	
#endif // __WORLDSERVER

#ifdef __CLIENT
	virtual void	Render( LPDIRECT3DDEVICE9 pd3dDevice );

	BOOL			IsStateDbuff();
	BOOL			EndMotion();
	BOOL			IsPKAttackAble( CMover* pMover );
	BOOL			IsPVPAttackAble( CMover* pMover );
	CSfx*			CreateSfxArrow( DWORD dwSfxObjArrow, DWORD dwSfxObjHit, D3DXVECTOR3 vPosDest, int idTarget );
	void			SetDmgCnt( int nDmgCnt ) { m_nDmgCnt = nDmgCnt; }

	void			GetItemSFXIndex( IN const int nAbilityOption, OUT DWORD& dwSFXIndex, OUT DWORD& dwSFXHandsIndex );
	void			CreateAbilityOption_SetItemSFX( int nAbilityOption );
	BOOL			GetEquipFlag( int nParts, BYTE* pbyFlag );
	void			OverCoatItemRenderCheck(CModelObject* pModel);
	void			PlayCombatMusic();
	BOOL			IsLoot( CItem *pItem ) { return TRUE; }
	LPCTSTR			GetFameName();						// �� �̸� ���

	LPCTSTR			GetJobString();						// ���� �̸� ��� 
	void			DialogOut( LPCTSTR lpszText );		// ��ǳ���� ���� ��� ���
	BOOL			DoFakeEquip( const EQUIP_INFO & rEquipInfo, BOOL bEquip, int nPart, CModelObject* pModel = NULL ); // for Fake
	void			RenderGauge( LPDIRECT3DDEVICE9 pd3dDevice, int nValue );
	void			RenderTurboGauge( LPDIRECT3DDEVICE9 pd3dDevice, DWORD nColor, int nValue, int nMaxValue );
	void			SetRenderPartsEffect( int nParts );
	void			RenderPartsEffect( LPDIRECT3DDEVICE9 pd3dDevice );
	void			RenderName( LPDIRECT3DDEVICE9 pd3dDevice, CD3DFont* pFont, DWORD dwColor = 0xffffffff );
	void			RenderHP(LPDIRECT3DDEVICE9 pd3dDevice);

	void			RenderCltGauge(LPDIRECT3DDEVICE9 pd3dDevice);	//sun: 11, ä�� �ý���

	void			RenderChrState(LPDIRECT3DDEVICE9 pd3dDevice);
	void			RenderFlag( int nType );
	void			RenderCasting( LPDIRECT3DDEVICE9 pd3dDevice );
	void			RenderCtrlCasting( LPDIRECT3DDEVICE9 pd3dDevice );
	void			RenderSkillCasting( LPDIRECT3DDEVICE9 pd3dDevice );		//sun: 10�� ���½ý���	Neuz, World, Trans

	void			RenderPVPCount( LPDIRECT3DDEVICE9 pd3dDevice );
	void			RenderQuestEmoticon( LPDIRECT3DDEVICE9 pd3dDevice );
	void			RenderGuildNameLogo( LPDIRECT3DDEVICE9 pd3dDevice, CD3DFont* pFont, DWORD dwColor );
	void			RenderNameBox( LPDIRECT3DDEVICE9 pd3dDevice, const CRect& rect );
//sun: 8, // __S8_PK
	DWORD			DoUseSkill( const int nType, const int nIdx, const OBJID idFocusObj, const SKILLUSETYPE sutType, const BOOL bControl, const int nCastingTime, DWORD dwSkill = 0, DWORD dwLevel = 0 );

	void			RenderAngelStatus(LPDIRECT3DDEVICE9 pd3dDevice);		//sun: 8�� ���� ��ȯ Neuz, World, Trans

#endif	// __CLIENT

	int				GetSkillLevel( SKILL* pSkill );
	BOOL			SetExperience( EXPINTEGER nExp1, int nLevel );

	BOOL	IsUsing( FLItemElem* pItemElem );

	BOOL	IsShoutFull( void );
	BOOL	IsCommBank( void );

	//////////////////////////////////////////////////////////////////////////
	void			SetActParam( OBJACT act, int nParam1 = 0, int nParam2 = 0, int nParam3 = 0, int nParam4 = 0, int nParam5 = 0 );
	int				GetActParam( int nIdx ) { return m_nAParam[nIdx]; }
	//////////////////////////////////////////////////////////////////////////

private:
	void			ProcessMove();
	void			ProcessMoveArrival( CCtrl *pObj );
	void			ProcessRegenItem();
	void			ProcessRegion();
	void			ProcessQuest();
	void			ProcessIAObjLink( D3DXVECTOR3& vPos);
	void			ProcessActionPoint();
#ifdef __CLIENT
	void			ProcessScaleSlerp();
	void			ProcessWaterCircle( const D3DXVECTOR3& vPosTemp, const D3DXVECTOR3& vPos );
	void			ProcessWalkSound();
	void			ProcessDustSFX();
	void			ProcessETC();
#else
	void			ProcessScript();
	void			ProcessAbnormalState();
	void			ProcessTarget();
#endif // __CLIENT

#ifdef __WORLDSERVER
	bool			IsForceItemDropMonster( CMover* pAttacker );
#endif // __WORLDSERVER

	BOOL			DropItem( CMover* pAttacker );		// �׾ ����Ʈ���� ���.

	BOOL			DropItem( CMover & kAttacker );

	void			OnArriveAtPos();
	void			OnArrive( DWORD dwParam1, DWORD dwParam2 );

	void			SetCmd( OBJACT cmd, int nParam1 = 0, int nParam2 = 0, int nParam3 = 0 );
	void			SetCmdParam( int nIdx, int nValue ) { m_nCParam[nIdx] = nValue; }
	BOOL			__SetQuest( LPQUEST lpQuest, LPQUEST lpNewQuest );

public:
#ifdef __CLIENT

	void			CreateAngelParticle(D3DXVECTOR3 vPos);		//sun: 8�� ���� ��ȯ Neuz, World, Trans
	void			CreatePetParticle(D3DXVECTOR3 vPos);		//sun: 9, 9-10�� ��

	void			ProcessEyeFlash();
	void			WingMotionSetting( const CModelObject* pModel );
	float			GetRideFrameSpeed( void );

#ifdef __BS_EFFECT_LUA
	const char*		GetNameO3D( );
	BOOL			SetDataMTE( const char* fname1, const char* fname2 );
#endif

	void			SetAngle( FLOAT fAngle );

#ifdef _DEBUG
	//������ test
	BOOL			CreateEnchantEffect_NEW( const int nKind, const TCHAR* fname );
	CModelObject*	DeleteEnchantEffect_NEW( const int nKind );
#endif //_DEBUG

#endif //__CLIENT

private:	

	OBJID			m_dwRegisterElldinsJarID;	


public:		

	void			SetRegisterElldinsJarID( OBJID dwID )	{	m_dwRegisterElldinsJarID = dwID;	}
	OBJID			GetRegisterElldinsJarID( void )			{	return m_dwRegisterElldinsJarID;	}


private:
	DWORD	m_dwSleepingDuration;

public:
	bool	SetSleeping( bool bApply, DWORD dwDuration = 0 );

	void	SetSleepingDuration( DWORD dwSleepingDuration )	{	m_dwSleepingDuration = dwSleepingDuration;	}

	bool	IsNoActionState();

	int		GetUsableItemQuantityByItemID( const DWORD dwItemID ) const;

	DWORD	CanSellItemToNPC( const DWORD dwItemObjID, const int nSellQuantity );

	bool	GetUnitCostSellItemToNPC( int & nSellCost, const DWORD dwItemObjID );

#ifdef __CLIENT
	//Event Arena
	int		m_nEATeam;		// ������, ������, ����� : �̺�Ʈ�Ʒ��� ���忡�� �̰��� �����ؼ� �Ǿƽĺ� �������� ����
#endif //__CLIENT

	bool	IsAbnormalState( const int nCHS );

	bool	CheckAttackDistance( CObj* pTarget, const DWORD AttackRangeType );

	MEMPOOLER_DECLARE( CMover );


#ifdef __CLIENT
	int		GetSetItemSFX_EffectValue( void );
#endif
private:
	AutoSynchronizer	m_autoSynchronizer;
public:
	AutoSynchronizer*	AutoSynchronizer()	{	return &m_autoSynchronizer;	}
	virtual D3DXVECTOR3		GetScrPos()		{	return AutoSynchronizer()->GetSyncPos();	}
	float	GetScrAngle()	{	return AutoSynchronizer()->GetSyncAngle();	}
	float	GetScrAngleX()	{	return AutoSynchronizer()->GetSyncAngleX();	}
#ifdef __WORLDSERVER
	void	ProcessMoveArrivalBySynchronizer();
#endif // __WORLDSERVER


	/************************************************************************/
#ifdef __WORLDSERVER
	private:
		virtual float	GetRateRecoveryRevival()	{	return 0.0f;	}
		virtual bool	GetDecreaseExperienceRevival( nsRevival::DECREASE_EXP_DATA& /*decreaseExpData*/ ) const	{	return false;	}
		virtual void	DoRecoveryRevival( const float /*recoveryRate*/ ) {	return;	}
		virtual bool	DoDecreaseExperience( const nsRevival::DECREASE_EXP_DATA& /*decreaseExpData*/, bool& /*downLevel*/ )	{	return false;	}

		void			DecreaseExperience( const bool Transfer, const bool Resurrection );
		void			RecoveryRevival();

		bool			m_decreaseExperience;

	public:
		void			Revival( const bool bTransfer = true, const bool bResurrection = false );
		bool			CanRevival() const;
		
		void			SetDecreaseExperienceRevival( const bool decreaseExperience )	{	m_decreaseExperience = decreaseExperience;	}
		bool			CanDecreaseExperienceRevival() const {	return m_decreaseExperience;	}

		bool			pushedByAnotherPower( const float angle, const float power );
#endif // __WORLDSERVER
	/************************************************************************/

public:
#ifdef PAT_LOOTOPTION
	int m_nPetLootType;		// �ݱ����� �ֿ� Ÿ���� ����.
#endif // PAT_LOOTOPTION
#ifdef SCRIPT_ENTER_INSTANCEDUNGEON
	bool				EnterInstanceDungeon( DWORD dwTeleWorld, int nX, int nY, int nZ );
#endif	// SCRIPT_ENTER_INSTANCEDUNGEON
};

// �������� �˾ƺ�
inline	BOOL CMover::IsMode( DWORD dwMode ) 
{ 
	switch( dwMode )
	{
		case MATCHLESS_MODE:		// ���� ����
		case ONEKILL_MODE:		// ���� ����
		case MATCHLESS2_MODE:	// ���� ����2
			if( m_dwAuthorization == AUTH_GENERAL )
				return FALSE;
			break;
	}
	if( (dwMode & TRANSPARENT_MODE) )	// ����ȭ�� �˻��Ϸ� ��������
		if( GetAdjParam(DST_CHRSTATE) & CHS_INVISIBILITY )	// ��������ȭ�� �ɷȴ°�?
			return TRUE;
	return ( ( m_dwMode & dwMode ) == dwMode ) ? TRUE : FALSE; 
}	

inline	BOOL CMover::IsStateMode( DWORD dwMode ) 
{ 
	return ( ( m_dwStateMode & dwMode ) == dwMode ) ? TRUE : FALSE; 
}	

inline BOOL CMover::IsFly() 
{ 
	if( IsPlayer() )
	{
		return m_pActMover->IsFly();
	}
	else
	{
		return IsFlyingNPC();
	}
}

inline int	CMover::DoAttackMelee( OBJID idTarget, OBJMSG dwMsg, DWORD dwItemID )
{
	CMover *pTarget = (CMover *)prj.GetMover( idTarget );
	if( IsInvalidObj(pTarget) )
		return 0;

	return DoAttackMelee( pTarget, dwMsg, dwItemID );
}

#if !defined(__WORLDSERVER)
inline int CMover::IsSteal( OBJID idTaget )		// id�� �˻��ϴ� ����.
{
	CMover *pTarget = prj.GetMover( idTaget );
	if( IsInvalidObj(pTarget) )		
		return 0;
	else
		return 	IsSteal( pTarget );
}
#endif	// __WORLDSERVER

// �׼� �Ž����� ���� ; ��ü�� �������� �Ž����� ������ 
inline int CMover::SendActMsg( OBJMSG dwMsg, int nParam1, int nParam2, int nParam3, int nParam4 ) 	
{	
	if( m_pActMover )	
		return m_pActMover->SendActMsg( dwMsg, nParam1, nParam2, nParam3, nParam4 );	
	else
		return 0;
}

//raiders.2006.11.28	 trade���� ��꿡 �����ϴ� ���� ����
inline int CMover::GetGold()
{
	int nGold = m_dwGold;
//	nGold -= m_vtInfo.TradeGetGold();
	if( nGold < 0 || nGold > INT_MAX )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "Invalid. UserId:[%07d], Gold:[%d]" ), m_idPlayer, m_dwGold );
	}
	return nGold;
}

#ifndef __CLIENT //_SUN_CHECKDATA_
inline void CMover::SetGold( int nGold )
{
	if( nGold < 0 || nGold > INT_MAX )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "Invalid. UserId:[%07d], Gold:[%d]" ), m_idPlayer, nGold );
		return;
	}
	m_dwGold = (DWORD)nGold;
}
#endif 

inline void CMover::ClearCmd() 
{ 
//	FLTRACE_LOG( PROGRAM_NAME, _T( "ClearCmd %d->0" ), m_oaCmd );
	m_oaCmd = OBJACT_NONE; 
//	memset( m_nCParam, 0xcd, sizeof(m_nCParam) );	// 0xcd??
	memset( m_nCParam, 0, sizeof(m_nCParam) );
}

inline void CMover::ClearActParam() 
{ 
	m_oaAct = OBJACT_NONE; 
//	memset( m_nAParam, 0xcd, sizeof(m_nAParam) );	// 0xcd??
	memset( m_nAParam, 0, sizeof(m_nAParam) );
}


extern int GetWeaponPlusDamage( int nDamage, BOOL bRandom, PT_ITEM_SPEC pItemProp , int nOption );
extern bool __IsEndQuestCondition( CMover* pMover, int nQuestId );

extern bool __IsBeginQuestCondition( CMover* pMover, int nQuestId );

extern bool __IsNextLevelQuest( CMover* pMover, int nQuestId );
extern	BOOL AttackBySFX( CMover* pAttacker, SFXHIT_INFO & info );

#endif // !defined(AFX_MOVER_H__4B7B21D7_A2D3_4115_946C_68DC9045A845__INCLUDED_)