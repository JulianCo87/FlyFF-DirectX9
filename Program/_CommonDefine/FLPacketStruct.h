#ifndef __FLPACKETSTRUCT_H__
#define __FLPACKETSTRUCT_H__


#include "../_CommonDefine/FLTm.h"
#include "../_CommonDefine/FlyffCommonDefine.h"
#include "../_CommonDefine/FLCommonStruct.h"


#pragma pack( 1 )

typedef	struct	_T_PACKET_COUPON_NUMBER
{
	TCHAR	szCouponNumber[ _MAX_COUPON_NUMBER_LEN + 1 ];
	TCHAR	szAccount[42];
	DWORD	dwPlayerId;
	DWORD	dwItemId;
	int		nError;
	TCHAR	szError[128];

} T_PACKET_COUPON_NUMBER, *PT_PACKET_COUPON_NUMBER;


typedef struct	_T_PACKET_ITEM_LINK
{
	DWORD	dwPlayerId;		// �������� ������ �ִ� �÷��̾� ���̵�
	DWORD	dwObjId;		// ������ ObjId
	DWORD	dwItemId;		// ������ ���̵�
	DWORD	dwStringId;		// ��Ʈ�� ���̵�

} T_PACKET_ITEM_LINK, *PT_PACKET_ITEM_LINK;



typedef	struct	_T_PACKET_SERIALNUMBER_REQ
{
	DWORD	dwCount;		// ��û ����

} T_PACKET_SERIALNUMBER_REQ, * PT_PACKET_SERIALNUMBER_REQ;


typedef	struct	_T_PACKET_SERIALNUMBER_ACK
{
	SERIALNUMBER	iStartNumber;		// ���� �ѹ�
	SERIALNUMBER	iEndNumber;		// �� �ѹ�

} T_PACKET_SERIALNUMBER_ACK, * PT_PACKET_SERIALNUMBER_ACK;


typedef struct	_T_PACKET_OPER_EXTRACT
{
	DWORD	dwMaterialObjid;	// ���۸� ������ ������
	bool	bResult;

} T_PACKET_OPER_EXTRACT, *PT_PACKET_OPER_EXTRACT;


typedef struct	_T_PACKET_OPER_CREATE
{
	DWORD	dwOperPieceObjid;			// ���� ����ü
	DWORD	dwOperPieceCombineObjid;	// ���� ������
	bool	bResult;

} T_PACKET_OPER_CREATE, *PT_PACKET_OPER_CREATE;


typedef struct	_T_PACKET_CID_CREATE
{
	DWORD	dwCidObjid;				// �ֻ�� �õ�
	DWORD	dwCidCombineObjid;		// �ֻ�� �õ� ������
	bool	bResult;

} T_PACKET_CID_CREATE, *PT_PACKET_CID_CREATE;


typedef struct	_T_PACKET_CIDPIECE_UPGRADE
{
	DWORD	dwCidPieceObjid;			// �õ� ����ü
	DWORD	dwCidPieceCombineObjid;		// �õ� ������
	bool	bResult;

} T_PACKET_CIDPIECE_UPGRADE, *PT_PACKET_CIDPIECE_UPGRADE;


typedef struct	_T_PACKET_OPERCID_CREATE
{
	DWORD	dwOperObjid;			// ����
	DWORD	dwCidObjid;				// �ֻ�� �õ�
	DWORD	dwBaryummealObjid;		// �ٷ��� ����
	bool	bResult;

} T_PACKET_OPERCID_CREATE, *PT_PACKET_OPERCID_CREATE;


typedef struct _T_PACKET_GUILDHOUSE_TENDER_STATE
{
	DWORD	dwGHType;
	int		nState;

} T_PACKET_GUILDHOUSE_TENDER_STATE, *PT_PACKET_GUILDHOUSE_TENDER_STATE;


typedef struct _T_PACKET_GUILDHOUSE_INFO
{
	DWORD	dwGuildHouseNpcIndex;		// ����Ͽ콺 NPC �ε���
	DWORD	dwGuildId;					// ���� ��� ���̵�
	DWORD	dwGuildHouseTitleIndex;		// ����Ͽ콺 �̸�
	TCHAR	szGuildHouseComment[64];	// ����Ͽ콺 ����

} T_PACKET_GUILDHOUSE_INFO, *PT_PACKET_GUILDHOUSE_INFO;



//////////////////////////////////////////////////////////////////////////
// shopping basket
//////////////////////////////////////////////////////////////////////////
typedef struct _T_BUY_ITEM_INFO
{
	int		nShopTabId;
	DWORD	dwObjId;
	DWORD	dwItemId;
	int		nItemNum;

} T_BUY_ITEM_INFO, *PT_BUY_ITEM_INFO;


typedef struct _T_PACKET_SHOPPING_BASKET
{
	DWORD	dwVendorId;					// �Ǹ����� �÷��̾��� �� �÷��̾� ���̵�
	int		nVendorKind;				// PC or NPC
	int		nVendorNPCType;				// Penya or Chip
	bool	bResult;					// ���� ���� ����

	T_BUY_ITEM_INFO tBuyItemInfo[_MAX_SHOPPING_BASKET];

} T_PACKET_SHOPPING_BASKET, *PT_PACKET_SHOPPING_BASKET;


//////////////////////////////////////////////////////////////////////////
// reset property bind
//////////////////////////////////////////////////////////////////////////
typedef struct _T_PACKET_RESET_BIND
{
	DWORD	dwSrcObjid;			// �ͼ� ���� ��ũ��
	DWORD	dwDestObjid;		// �ͼ� ���� �� ������
	bool	bResult;			// �ͼ� ���� ���� ����

} T_PACKET_RESET_BIND, *PT_PACKET_RESET_BIND;


//------------------------------------------------------------------------------------------------------------------------------------------//
//----------------------------------- ���� ��� start --------------------------------------------//


struct T_PACKET_COOPERATIVE_CONTRIBUTIONS_GET_INFO_REQ
{
	DWORD					dwObjidNPC;
};

struct T_PACKET_COOPERATIVE_CONTRIBUTIONS_GET_INFO_HEADER_ACK
{
	// ��� Text
	__time64_t				tRemainSecond;

	DWORD					dwDescription_TextID;		//��ǥ 
	DWORD					dwEndCond_TextID;			//��� ����
	size_t					nEndConditionCount;
	//body
	//���� ���� : [ ConditionCount | ConditionKind ( | IndexOfKind ) | ConditionCountMax | ContributedCount ]
	//���� ���� : [ RewardCount | RewardTextID1 | RewardTestID2 ... ]
};



struct T_PACKET_COOPERATIVE_CONTRIBUTIONS_CONTRIBUTE_REQ
{
	DWORD					dwObjidNPC;
};


struct T_PACKET_COOPERATIVE_CONTRIBUTIONS_CONTRIBUTE_ACK
{
	enum E_RESULT
	{
		RESULT_SUCCESS				= 0,
		INVALID_BEGIN_CONDITION,
		ALREADY_COMPLETE,
		INVALID_PERIOD,						//�̺�Ʈ �Ⱓ�� �ƴϴ�
		NOT_ENOUGH,							//�������� ������̰ų� ���ڶ��ϴ�.
	};

	E_RESULT				eResult;
};

struct T_PACKET_COOPERATIVE_CONTRIBUTIONS_GET_RANKING_REQ
{
	DWORD					dwObjidNPC;
};

struct T_PACKET_COOPERATIVE_CONTRIBUTIONS_GET_RANKING_ACK
{
	enum		{	RANKING_MAX = 3		};

	enum E_RESULT
	{
		RESULT_SUCCESS				= 0,
		INVALID_PERIOD,						//�̺�Ʈ �Ⱓ�� �ƴմϴ�.
	};

	TCHAR					arrNameString[ RANKING_MAX ][ MAX_NAME ];

	E_RESULT				eResult;
};




struct T_W2DB_PACKET_COOPERATIVE_CONTRIBUTIONS_GET_INFO_REQ
{
	DWORD					dwContributionID;
	FLTm					tmStart;
	FLTm					tmEnd;
};

struct T_W2DB_PACKET_COOPERATIVE_CONTRIBUTIONS_GET_INFO_ACK
{
	DWORD					dwContributionID;
	FLTm					tmStart;
	FLTm					tmEnd;

	DWORD					dwContiributionCount;
};

struct T_W2DB_PACKET_CONTRIBUTION_GET_RANKING_REQ
{
	u_long					idPlayer;
	DWORD					dwContributionID;
	FLTm					tmStart;
	FLTm					tmEnd;
};

struct T_W2DB_PACKET_CONTRIBUTION_GET_RANKING_ACK
{
	enum		{	RANKING_MAX = 3		};

	DWORD					dwContributionID;

	u_long					idPlayer;
	TCHAR					arrNameString[ RANKING_MAX ][ MAX_NAME ];
};

struct T_W2DB_PACKET_CONTRIBUTE_REQ
{
	u_long					idPlayer;
	TCHAR					szName[ MAX_NAME ];

	DWORD					dwContributionID;
	FLTm					tmStart;
	FLTm					tmEnd;

	E_CONTRIBUTION_KIND		eContributionKind;
	DWORD					dwIndexOfKind;
	DWORD					dwContiributionCount;
	DWORD					dwContiributionMax;
};

struct T_W2DB_PACKET_TRY_WRITE_ITEM_REWARD_INFO_REQ
{
	DWORD					dwContributionID;
	FLTm					tmStart;
	FLTm					tmEnd;

	DWORD					dwItemID;
	short					shItemCount;
};

struct T_W2DB_PACKET_TRY_WRITE_ITEM_REWARD_INFO_ACK
{
	enum E_RESULT
	{
		RST_FAIL			= 0,	
		RST_SUCCESS,
		RST_ALREADY_WRITE, 
	};


	DWORD					dwContributionID;
	FLTm					tmStart;
	FLTm					tmEnd;

	DWORD					dwItemID;
	short					shItemCount;

	E_RESULT				eResult;
};


struct T_W2DB_PACKET_CONTRIBUTE_ACK
{
	u_long					idPlayer;

	DWORD					dwContributionID;
	DWORD					dwContiributionCount;
};

struct T_W2DB_PACKET_TRY_REWARD_ITEM_REQ
{
	DWORD					dwContributionID;

	FLTm					tmStart;
	FLTm					tmEnd;
};

//struct T_W2DB_PACKET_TRY_REWARD_ITEM_ACK
//{
//	enum E_RESULT
//	{
//		RST_FAIL		= 0,
//		RST_SUCCESS,
//		RST_ALREADY_REWARD,
//	};
//
//
//	DWORD					dwContributionID;
//	E_RESULT				eResult;
//};


//----------------------------------- ���� ��� end --------------------------------------------//


//************************************ �ݷμ��� start *****************************************//

struct T_PACKET_COLOSSEUM_ENTER_REQ				//���� ����
{
//	DWORD	dwWorldID;
	DWORD	dwDungeonLevel;
	BOOL	bGuild;
};


struct T_PACKET_COLOSSEUM_ENTER_ACK				//���� ����
{
	enum E_RESULT
	{
		RST_SUCESS		= 0,	
		RST_FAIL,
	};

	E_RESULT		eResult;
};


struct T_PACKET_COLOSSEUM_RETRY_ACK
{
	enum E_RESULT
	{
		RST_SUCESS		= 0,
		RST_FAIL,
	};

	E_RESULT		eResult;
};




//struct T_PACKET_COLOSSEUM_AUTO_INVITE_REQ	//���� �ʴ�
//{
//	TCHAR	szInvitePlayerName[ _MAX_LOGSTR_PLAYERNAME_LEN ];
//};

struct T_PACKET_COLOSSEUM_AUTO_INVITE_ACK	//���� �ʴ뿡 ���� ����
{
	DWORD	dwWorldID;
	BOOL	bYes;
};


struct T_PACKET_COLOSSEUM_STARTED_STAGE			//�������� ���۵�
{
	int				nStage;
	DWORD			dwTimeLimitRemainTick;
};
struct T_PACKET_COLOSSEUM_CLEARED_STAGE				//�������� �Ϸ��
{
	int				nStage;
};
struct T_PACKET_COLOSSEUM_STAGE_TIME_LIMIT			//�������� �ð� ����
{
	int				nStage;
	DWORD			dwRetryRemainTick;
};

typedef		T_PACKET_COLOSSEUM_STAGE_TIME_LIMIT			T_PACKET_COLOSSEUM_ALL_DIE;		//��� ��Ƽ�� ���

//�̺�Ʈ ����
struct FLRewardItem
{
	DWORD		dwItemID;
	short		nCount;
};

struct T_PACKET_COLOSSEUM_ALL_CLEAR_HEADER												//���� �Ϸ� ����
{
	DWORD			dwRemainTick;						//���� �ð�
	DWORD			dwCompleteTimeTick;					//�ð� ���

	bool			bDisplayRank;						// ��ŷ ���� ǥ�� ����
	int				nWeeklyRank;						// �ְ� ��ŷ

	T_PACKET_COLOSSEUM_ALL_CLEAR_HEADER() : dwRemainTick( 0 ), dwCompleteTimeTick( 0 ), bDisplayRank( false ), nWeeklyRank( 0 )
	{
	}

	T_PACKET_COLOSSEUM_ALL_CLEAR_HEADER( const DWORD RemainTick, const DWORD CompleteTimeTick, const bool DisplayRank, const int WeeklyRank )
		: dwRemainTick( RemainTick ), dwCompleteTimeTick( CompleteTimeTick ), bDisplayRank( DisplayRank ), nWeeklyRank( WeeklyRank )
	{
	}
};

struct T_PACKET_COLOSSEUM_REWARD_ITEM_LIST
{
	size_t			nItemCount;
};


struct T_COLOSSEUM_RANKING
{
	enum		{ RANKING_MAX = 5 };

	char		szMyGuild[ MAX_G_NAME ];	
	DWORD		dwClearTick;

	char		szArrGuild[ RANKING_MAX ][ MAX_G_NAME ];		
	DWORD		dwArrClearTick[ RANKING_MAX ];
};

struct T_COLOSSEUM_RANKING_PACK
{
	T_COLOSSEUM_RANKING		WeekRanking;
	T_COLOSSEUM_RANKING		MonthRanking;
	T_COLOSSEUM_RANKING		TotalRanking;
};

struct T_PACKET_COLOSSEUM_GET_RANKING_INFO
{
	enum						{ DUNGEON_LEVEL_NORMAL = 0, DUNGEON_LEVEL_HARD, DUNGEON_LEVEL_MAX };

	T_COLOSSEUM_RANKING_PACK	arrRanking[ DUNGEON_LEVEL_MAX ];
};


//�ð� ����
//struct T_PACKET_REMAIN_TICK			//���� �ð� ����ü
//{
//	DWORD	dwRemainTime;
//};
//
//struct T_PACKET_COLOSSEUM_AUTO_START_WAITING_TICK : public T_PACKET_REMAIN_TICK	{};		//���� �ڵ� ���� ���� �ð�
//struct T_PACKET_COLOSSEUM_START_STAGE_WAITING_TICK : public T_PACKET_REMAIN_TICK{};		//���� ���� ��� ���� �ð� 
//struct T_PACKET_COLOSSEUM_RETRY_STAGE_WAITING_TICK : public T_PACKET_REMAIN_TICK{};		//���� ��õ� �ð� ���� �ð� ( time limit�� ��� )
//

//
//struct T_PACKET_COLOSSEUM_RETRIED
//{
//	DWORD	dwRetryStageWaitingTick;//STAGE_WAITING_TICK
//};


//------------------------------------ �ݷμ��� end -------------------------------------------//

//- �ݷμ���

typedef struct _T_W2DB_PACKET_REWARD_ITEM_NOTI
{
	u_long					idReceiver;

	DWORD					dwMailTitleTID;
	DWORD					dwMailTextTID;

	DWORD					dwItemID;
	short					nItemCount;
} T_W2DB_PACKET_REWARD_ITEM_NOTI, * PT_W2DB_PACKET_REWARD_ITEM_NOTI;

//struct T_PACKET_COLOSSEUM_ALL_DIE
//struct T_PACKET_COLOSSEUM_STARTED_STAGE
//struct T_PACKET_COLOSSEUM_RETRIED
//struct T_PACKET_COLOSSEUM_FORCE_START



struct T_W2DB_PACKET_RECORD_DUNGEON_PLAY_INFO
{
	DWORD				dwGuildID;
	DWORD				dwWorldID;
	int					eDungeonLevel;
	DWORD				dwCompleteTimeTick;

	FLTm				tmStartedDate;
	FLTm				tmFinishedDate;
	BOOL				bSuccess;

	DWORD				dwPartyID;
	int					nWeeklyRank;
};

//struct T_W2DB_PACKET_RECORD_DUNGEON_BEST_COMPLETE_TIME
//{
//	DWORD				dwGuildID;
//	DWORD				dwWorldID;
//	int					eDungeonLevel;
//	DWORD				dwCompleteTimeTick;
//};



struct T_W2DB_PACKET_COLOSSEUM_GET_RANKING_INFO
{
	u_long				idPlayer;
	//DWORD				dwGuildID;
	DWORD				dwWorldID;
	//int					eDungeonLevel;
	//DWORD				dwCompleteTimeTick;
	//@m_idGuild		char(6),
	//	@dwWorldID		int,
	//	@tPlayTime		int
};

typedef struct _T_PACKET_ITEM_MERGE_INFO
{
	DWORD	dwObjIdSrc;
	DWORD	dwObjIdDest;

} T_PACKET_ITEM_MERGE_INFO, *PT_PACKET_ITEM_MERGE_INFO;

typedef struct _T_PACKET_ITEM_MERGE
{
	bool	bResult;
	DWORD	dwMergeCount;
	DWORD	dwMergeType;

	T_PACKET_ITEM_MERGE_INFO tItemMergeInfo[MAX_ITEM_MERGE_COUNT];

} T_PACKET_ITEM_MERGE, *PT_PACKET_ITEM_MERGE;





#pragma pack()


#endif	//__FLPACKETSTRUCT_H__
