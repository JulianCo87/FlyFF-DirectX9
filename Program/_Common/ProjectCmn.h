#ifndef __PROJECTCMN_H__
#define	__PROJECTCMN_H__

#define	MAX_OBJARRAY			8
#define	MAX_QUICKSLOT			21
#define	MAX_EQUIPMENT			18

#define	MAX_INVENTORY_OLD		42
#ifdef INVENTORY_GENERAL54
#define MAX_INVENTORY_GENERAL	54
#else 
#define MAX_INVENTORY_GENERAL	42
#endif	// INVENTORY_GENERAL54
#ifdef INVENTORY_PET_COSTUME42
#define MAX_INVENTORY_PET		42
#define MAX_INVENTORY_COSTUME	42
#else
#define MAX_INVENTORY_PET		24
#define MAX_INVENTORY_COSTUME	24
#endif
#define MAX_INVENTORY_QUEST		24

#define MAX_INVENINDEX_GENERAL		( MAX_HUMAN_PARTS + MAX_INVENTORY_GENERAL )
#define MAX_INVENINDEX_PET			( MAX_HUMAN_PARTS + MAX_INVENTORY_GENERAL + MAX_INVENTORY_PET )
#define MAX_INVENINDEX_COSTUME		( MAX_HUMAN_PARTS + MAX_INVENTORY_GENERAL + MAX_INVENTORY_PET + MAX_INVENTORY_COSTUME )
#define MAX_INVENINDEX_QUEST		( MAX_HUMAN_PARTS + MAX_INVENTORY_GENERAL + MAX_INVENTORY_PET + MAX_INVENTORY_COSTUME + MAX_INVENTORY_QUEST )

#if __VER >= 17

#define	MAX_INVENTORY			( MAX_INVENTORY_GENERAL + MAX_INVENTORY_PET + MAX_INVENTORY_COSTUME + MAX_INVENTORY_QUEST )

#else // __VER >= 17

#define	MAX_INVENTORY			MAX_INVENTORY_OLD

#endif // __VER >= 17


#define MAX_HUMAN_PARTS_OLD		31	// ���� ���� ������..
#define MAX_HUMAN_PARTS_ADD		19	// �κ��丮 Ȯ���� �Ǹ鼭 ���� �ٽ� Ȯ���� �����ϰ� �ϱ����� ��������� ����..

#define	MAX_HUMAN_PARTS         ( MAX_HUMAN_PARTS_OLD + MAX_HUMAN_PARTS_ADD )



#define MAX_GUILDBANK			42
#define	MAX_BANK				42
#define	MAX_DEFAULT_POCKET		6
#define	MAX_EXTENSION_POCKET	24
#define	MAX_REPAIR				16
//#define	MAX_TRADE				25
const	BYTE MAX_TRADE			=	25;
#define	MAX_VENDITEM			30
#define	MAX_REPAIRINGITEM		25
#define	MAX_VENDORNAME			48
#define	MAX_STUFF				( MAX_INVENTORY + MAX_QUICKSLOT )
#define	MAX_EXPLPPOINT			41
#define	MAX_EXPSKILL			21
#define	MAX_EXPCHARACTER		200
#define	MAX_FXPCHARACTER		200
#define	MAX_VENDOR_INVENTORY	100
#define	MAX_VENDOR_INVENTORY_TAB 4

//#define	MAX_QUEST				100
//#define	MAX_COMPLETE_QUEST		300 
//#define 	MAX_CHECKED_QUEST		5

const size_t	MAX_QUEST				= 100;
const size_t	MAX_DOING_QUEST			= 60;
const size_t	MAX_COMPLETE_QUEST		= 500;
const size_t	MAX_CHECKED_QUEST		= 5;
const size_t	MAX_QUEST_INDEX			= 9999;

#define MAX_VENDOR_NEW_INVENTAB 4


#define MAX_PARTYLEVEL			10
#define MAX_ADDEXPPARTY			16
#define MAX_RENEWEVENT			8000
#define MAX_REALITEM			8			// �̺�Ʈ ��ǰ ������
#define MAX_EVENTREALITEM		4096
#define MAX_EXPJOBLEVEL			90
#define	ITEM_KIND_MAX			100

//#define	MAX_SKILL_PARAM			2
//#define	MAX_ITEM_PARAM			3

const int	MAX_PROPMOVER =		2000;		// MoverProp�迭�� �ִ밹�� 

#define	MAX_HONOR_TITLE		150	// ����Ÿ��Ʋ ����

#define MAX_VENDOR_REVISION		20			//sun: 8, // __S8_VENDOR_REVISION
#define MAX_SHOP_BASESLOT		24

// #define dwDestParam1	dwDestParam[0]
// #define dwDestParam2	dwDestParam[1]
// #define nAdjParamVal1	nAdjParamVal[0]
// #define nAdjParamVal2	nAdjParamVal[1]
// #define dwChgParamVal1	dwChgParamVal[0]
// #define dwChgParamVal2	dwChgParamVal[1]
// 
// //sun: 10, __PROP_0827
// #define dwDestParam3	dwDestParam[2]
// #define nAdjParamVal3	nAdjParamVal[2]
// #define dwChgParamVal3	dwChgParamVal[2]

#include <defineitemkind.h>		//sun: 9,

struct tagColorText
{
	DWORD dwColor;
	TCHAR * lpszData;
}; 

struct ObjProp
{
	DWORD	dwID;			// ������	
	TCHAR	szName[64];	// �ѱ۸�Ī 
	DWORD	dwType; 
	DWORD	dwAI;			// AIInterface
	DWORD	dwHP;
	ObjProp()
	{
		szName[0]	= '\0';
		dwID	= dwType	= dwAI	= dwHP	= 0;
	}
};

struct CtrlProp : ObjProp
{
	DWORD   dwCtrlKind1;
	DWORD   dwCtrlKind2;
	DWORD   dwCtrlKind3;
	DWORD   dwSfxCtrl;
	DWORD   dwSndDamage;
	CtrlProp() : ObjProp()
	{
		dwCtrlKind1	= dwCtrlKind2	= dwCtrlKind3	= dwSfxCtrl	= dwSndDamage	= 0;
	}

	BOOL IsGuildHousingObj( )	{ return CK1_GUILD_HOUSE == dwCtrlKind1; } //��� �Ͽ�¡ ���� ������Ʈ �ΰ�?
	BOOL IsHousingObj( )		{ return CK1_HOUSING == dwCtrlKind1; }		//���� �Ͽ�¡ ���� ������Ʈ 

};

// TODO SkillLevelProp�� �̸��� �ٲٴ� ���� ���� �Ͱ���.
struct AddSkillProp
{
	// TODO ���� �ɷ�ġ�� �ʿ��Ѱ�?
	// ���� �� �� ������ ��������.
	DWORD	dwID;							// ID
	DWORD	dwSkillPropID;					// ��ųProp ID

	DWORD	dwSkillLvl;						// ��ų����

	DWORD	dwAbilityMin;					// �ּҴɷ�
	DWORD	dwAbilityMax;					// �ִ�ɷ�

	DWORD	dwDmgShift;						// Ÿ�ݽ� �ݵ�
	DWORD	dwProbability;					// �ߵ� Ȯ��

	DWORD	dwDestParam[MAX_SKILL_PARAM];	// ������1
	DWORD	dwAdjParamVal[MAX_SKILL_PARAM];	// ���밪1;
	DWORD	dwChgParamVal[MAX_SKILL_PARAM];	// ���뺯ȭ��1
	int		arrAddSkillDestNumData1[3];			// ���뵥��Ÿ3��, DestParam[0]�� �ش�.TODO �̰� �� 3��?


	DWORD	dwActiveSkill;					// �ߵ� ����(���Ȳ���� �̰��� �����ϴ��� ��ȣ�ϴ�.?)
	DWORD	dwActiveSkillRate;				// �ߵ� ���� Ȯ��.
	DWORD	dwActiveSkillRatePVP;			// �ߵ� ���� Ȯ��(����)

	int		nReqMp;							// �ʿ�MP
	int		nReqFp;							// �ʿ�FP

	DWORD	dwCooldown;						// ��ٿ�
	DWORD	dwCastingTime;					// ��� �غ�ð�
	DWORD	dwSkillRange;					// ��� �����Ÿ�	

	DWORD	dwCircleTime;					// �����ð�(������ �� ����.)
	DWORD   dwPainTime;						// ���� ���ؽð�
	DWORD	dwSkillTime;					// ���ӽð�

	int		nSkillCount;					// �߻�ü �߻� ����. ������ݷ� = �Ѱ��ݷ� / nSkillCount;

	DWORD	dwAbilityMinPVP;				// �ּҴɷ�(����)
	DWORD	dwAbilityMaxPVP;				// �ִ�ɷ�(����)
	DWORD	dwProbabilityPVP;				// �ߵ� Ȯ��(����)

	DWORD	dwAttackSpeed_;					// ���ݼӵ�	// ������
	DWORD	dwTaunt_;						// Ÿ��Ʈ	// ������
	DWORD   dwSkillExp_;						// ��ų ����ġ	// ������
	DWORD	dwExp__;							// �������ġ	// ������
	DWORD	dwComboSkillTime_;				// �޺���ųŸ��	// ������


	AddSkillProp()
	{
		dwID = 0;				// ������	
		dwSkillPropID = 0;		// ��ųProp ID
		dwSkillLvl = 0;			// ��ų����
		dwAbilityMin = 0;		// �ּҴɷ�
		dwAbilityMax = 0;		// �ִ�ɷ�
		dwDmgShift = 0;			// Ÿ�ݽ� �ݵ�
		dwProbability = 0;		// �ߵ� Ȯ��

		memset( dwDestParam, 0, sizeof( dwDestParam ) );		// ������1
		memset( dwAdjParamVal, 0, sizeof( dwAdjParamVal ) );	// ���밪1;
		memset( dwChgParamVal, 0, sizeof( dwChgParamVal ) );	//���뺯ȭ��1
		memset( arrAddSkillDestNumData1, 0, sizeof( arrAddSkillDestNumData1 ) );		// ���뵥��Ÿ3��, DestParam[0]�� �ش�.

		dwActiveSkill = 0;		// �ߵ� ����
		dwActiveSkillRate = 0;	// �ߵ� ���� Ȯ��.

		nReqMp = 0;				//�ʿ�MP
		nReqFp = 0;				//�ʿ�FP

		dwCooldown = 0;			// ��ٿ�
		dwCastingTime = 0;		//��� �غ�ð�

		dwSkillRange = 0;		// ��� �����Ÿ�	
		dwCircleTime = 0;		//�����ð�
		dwPainTime = 0;         // ���� ���ؽð�
		dwSkillTime = 0;		//���ӽð�
		nSkillCount = 0;		// �߻�ü �߻� ����. ������ݷ� = �Ѱ��ݷ� / nSkillCount;

		dwActiveSkillRatePVP = 0;	// �ߵ� ���� Ȯ��(����)
		dwAbilityMinPVP = 0;	// �ּҴɷ�(����)
		dwAbilityMaxPVP = 0;	// �ִ�ɷ�(����)
		dwProbabilityPVP = 0;	// �ߵ� Ȯ��(����)

		dwAttackSpeed_ = 0;		// ���ݼӵ�
		dwTaunt_ = 0;			// Ÿ��Ʈ
		dwSkillExp_ = 0;         //��ų ����ġ
		dwExp__ = 0;				//�������ġ
		dwComboSkillTime_ = 0;	//�޺���ųŸ��
	}
};

enum 
{	
	FILE_FILTER	= 0, 
	FILE_INVALID = 1, 
	FILE_NOTICE = 2,	
	FILE_GUILDCOMBAT_TEXT_1 = 3, 
	FILE_GUILDCOMBAT_TEXT_2 = 4, 
	FILE_GUILDCOMBAT_TEXT_3 = 5, 
	FILE_GUILDCOMBAT_TEXT_4 = 6, 
	FILE_GUILDCOMBAT_TEXT_5 = 7,
	FILE_GUILDCOMBAT_TEXT_6 = 8,
	FILE_ALLOWED_LETTER		= 9		//sun: 10, __RULE_0615

//sun: 11, �ϴ��� ��� ����
	,
	FILE_GUILDCOMBAT_1TO1_TEXT_1 = 10,
	FILE_GUILDCOMBAT_1TO1_TEXT_2 = 11,
	FILE_GUILDCOMBAT_1TO1_TEXT_3 = 12,
	FILE_GUILDCOMBAT_1TO1_TEXT_4 = 13,
	FILE_GUILDCOMBAT_1TO1_TEXT_5 = 14

#ifdef __VENDOR_1106
	,FILE_ALLOWED_LETTER2	= 15
#endif	// __VENDOR_1106
	,FILE_SERVERMOVEINFO	= 16
};

// Item Property Type
enum IP_TYPE 
{
	IP_FLAG_NONE	= 0x00,				// �÷��� ���� 
//	IP_FLAG_BINDS	= 0x01,				// 1 - �ͼ� ������			// item property�� �ͼ� ���� �÷� ���� ���� �ͼ� �Ǵ���. �ش� ���� ������� ����.
	IP_FLAG_UNDESTRUCTABLE	= 0x02	//	2 - ���� �Ұ� ������
//	,IP_FLAG_EQUIP_BIND	= 0x04			// ���� �� �ͼ�				// item property�� �ͼ� ���� �÷� ���� ���� �ͼ� �Ǵ���. �ش� ���� ������� ����.
	, IP_FLAG_PERIOD = 0x08		// ������ ���� �� ��� �Ⱓ ����..
};


struct SkillProp : CtrlProp
{
	DWORD	dwMotion_;			// ����		// ���� �Ǿ� ����
	DWORD	dwNum_;				// �⺻��������
	DWORD	dwPackMax_;			// �ִ��ħ����

	BOOL	bPermanence_;		// �Һ�Ұ�
	DWORD	dwUseable_;			// ������
	DWORD	dwItemSex_;			// ��뼺��
	DWORD	dwCost_;				// ����	
	DWORD	dwEndurance_;		// ������

	int		nAbrasion_;			// ������
	int		nMaxRepair_;			// ����Ƚ��	// nHardness

	DWORD	dwFlag_;				// �ٸ��� �÷���	// dwHeelH
	DWORD	dwParts_;			// ������ġ	
	DWORD	dwPartsub_;			// ������ġ	
	DWORD	bPartsFile_;			// �������� ��뿩�� 
	DWORD	dwExclusive_;		// ���ŵ���ġ	
	DWORD	dwBasePartsIgnore_;
	DWORD	dwItemLV_;			// �����۷���	

	DWORD   dwShopAble_;
	int		nShellQuantity_;		// �ִ���������	- IK�� ä�������϶��� �ִ� ä������ �ȴ�.(���� ����)
	DWORD   dwFuelRe_;			// �Ѵ翬��������
	DWORD	dwAFuelReMax_;		// ���ӿ���������

	BOOL	bCharged_;			//

	short	wItemEatk_;			// �Ӽ� ������( �� �Ӽ� Ÿ������ ���ݷ��� �����Ѵ�. )
	DWORD   dwParry_;			// ȸ���� 
	DWORD   dwblockRating_;		// �� ��ġ 
	int		nAddSkillMin_;		// �ּ� �߰� ��ų
	int		nAddSkillMax_;		// �ִ� �߰� ��ų.

	DWORD	dwWeaponType_;		// �������� 
	DWORD	dwItemAtkOrder1_;	// ������1�����ݼ���
	DWORD	dwItemAtkOrder2_;	// ������2�����ݼ���
	DWORD	dwItemAtkOrder3_;	// ������3�����ݼ���
	DWORD	dwItemAtkOrder4_;	// ������4�����ݼ���

	DWORD	dwRecoil_;			// �ݵ�	
	DWORD	dwLoadingTime_;		// �����ð�	- IK�� ä�������϶��� ä���ӵ�(�ɷ�)�� �ȴ�.
	LONG	nAdjHitRate_;		// �߰����ݼ�����	
	FLOAT	fAttackSpeed_;		// ���ݼӵ�	

	DWORD	dwSkillReadyType_;

	DWORD	dwSfxElemental_;

	DWORD	dwCircleTime_;		// �����ð� 

	int		nItemResistElecricity_;
	int		nItemResistDark_;	// ���� �Ǿ� ����
	int		nItemResistFire_;
	int		nItemResistWind_;
	int		nItemResistWater_;
	int		nItemResistEarth_;

	FLOAT	fFlightSpeed_;		// ����������(�ӵ�)
	FLOAT	fFlightLRAngle_;		// �¿� �� ����.
	FLOAT	fFlightTBAngle_;		// ���� �� ����.
	DWORD	dwFlightLimit_;		// �������ѷ���
	DWORD	dwFFuelReMax_;		// ���࿬��������
	DWORD	dwLimitLevel1_;		// ���ѷ���1
	int		nReflect_;			// ���÷��� �ɼ�.

	DWORD	dwSndAttack2_;		// ȿ���� : ���� 2
	DWORD	dwQuestId_;
	TCHAR	szTextFileName_[64];	// item�� GM command�� �ִ� �Ϳ� ���

	DWORD	dwSkillRare_;		// ��ͼ�(TODO �Է°�����, ��������, �׽�Ʈ �ʿ�)
	int		nSkillLog_;			// �α�(�Է°�����, ��������)
	DWORD	dwSkillKind3_;		// 3����ų����, �������� �������� �����ص� �ɵ��ϴ�.
	DWORD	dwExpertLV_;			// (���� �� �ľ��� �ȵǳ�)
	DWORD	dwSkillJob_;			// ��ų�����Ҽ�(TODO DatabaseServer���� ��� ���� ���� ���� �𸣰ڴ�. skillKine2 �� �ߺ��Ǵ� �� ����. �����ص� �ɵ�.)


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	// ���� ����?
	DWORD	dwReqMp_;			// �ʿ�MP	- �ش� ��ų ���� ǥ�ÿ��� �����. ��ũ��Ʈ�� �ش� �÷��� �����ʹ� ����.
	DWORD	dwReqFp_;			// �ʿ�FP	- �ش� ��ų ���� ǥ�ÿ��� �����. ��ũ��Ʈ�� �ش� �÷��� �����ʹ� ����.
	


	// ���� Ư��?
	DWORD	dwSkillKind1;		// ���� ������?
	DWORD	dwSkillKind2;		// ����

	DWORD	dwSkillType;		// ����Ӽ�(���������� ���δ�. KT_SKILL... BUFF_SKILL...)

	LONG	nEvildoing;			// ����

	DWORD	dwSubDefine;		// AddSkillProp �ε���?
	DWORD	dwComboStyle;

	
	DWORD	dwExpertMax;		// �ִ���÷���

	// ���� ����
	DWORD	dwReqDisLV;			// �ʿ� ����
	DWORD   dwReqSkill1;		// ���� ��ų1
	DWORD   dwReqSkillLevel1;	// ���� ��ų1 �ʿ� ����
	DWORD   dwReqSkill2;		// ���� ��ų2
	DWORD   dwReqSkillLevel2;	// ���� ��ų2 �ʿ� ����

	// ��� ����
	DWORD	dwNeedItemHanded;	// �������
	DWORD	dwNeedItemKind3;	// �ʿ� ����������??
	DWORD	dwNeedItemBulletKind2;	// �߻�ü? �Ҹ���� ������

	DWORD	dwExeTarget;		// �ߵ� ���(�� �ָ��ϴ�, ������ε� ���ǰ� ��ų ���·ε� ���ȴ�.)
	DWORD	dwUseChance;		// ���ñ�?

	DWORD	dwSpellRegion;		// ���� ����


	DWORD	dwSkillReady;		// ��� �غ�ð�(��Ÿ��)
	DWORD	dwSkillRange;		// ��� �����Ÿ�

	

	DWORD	dwReqPartyPoint;	// �������ġ(��Ƽ��ų�϶� ������ ����Ʈ)


	// ����
	DWORD	dwSkillSpellType;		// �����Ӽ�(������ ����?)(������ų?)
	SAI79::ePropType	eSkillType;	// dwSkillSpellType�� �ߺ��Ǵ� �� �ϸ鼭�� �ٸ���. �ָ��ϴ�.

	BYTE	byBuffTickType;		// ���� ȿ�� ���� Ÿ��(���۰� ��ƽ, ��ƽ��)

	DWORD	dwAtkStyle;			// ���� ��Ÿ�� (dwAtkStyle == AS_BACK ���⿡���� ���ȴ�. ����.)

	DWORD   dwReferStat1;
	DWORD   dwReferStat2;
	DWORD   dwReferTarget1;
	DWORD   dwReferTarget2;
	DWORD   dwReferValue1;
	DWORD   dwReferValue2;


	DWORD	dwAbilityMin;		// �ּҴɷ�ġ - ���ݷ�, ����, ���� ��Ÿ��� AddSkillProp�� ���� ��ų�� �⺻��?
	DWORD	dwAbilityMax;		// �ִ�ɷ�ġ - ���ݷ�, ����, ���� ��Ÿ��� AddSkillProp�� ���� ��ų�� �⺻��?

    DWORD	tmContinuousPain;	// ���� ���� 

	DWORD	dwDmgShift;			// Ÿ�ݽ� �ݵ�	
	DWORD	dwAttackRange;		// ���ݹ���(�ѱ��븸 �����)
	int		nProbability;		// ����Ȯ��

	DWORD	dwDestParam[3];		// ������1
	LONG	nAdjParamVal[3];	// ���밪1
	DWORD	dwChgParamVal[3];	// ���뺯ȭ��1
	int		nDestData1[3];		// ���뵥��Ÿ�� 3��, destParam1���� �ش��.

	DWORD	dwSkillTime;		// ���ӽð�


	DWORD	dwActiveSkill;		// �ߵ� ����
	DWORD	dwActiveSkillLv;	// �ߵ� ���� ���� (���������� ����ؾ� �ҵ�.)
	DWORD	dwActiveSkillRate;	// �ߵ� ���� Ȯ��.

	DWORD	dwSfxObj;			// ������ ȿ�� ������Ʈ 
	DWORD	dwSfxObj2;			// ������ ȿ�� ������Ʈ 
	DWORD	dwSfxObj3;			// �ߵ�ȿ��, Ÿ��
	DWORD	dwSfxObj4;			// ���ӵǴ� ȿ�� ����Ʈ.
	DWORD	dwSfxObj5;			// �ߵ�ȿ��, ������ 
	DWORD	dwUseMotion;		// ��� ���

	DWORD	dwSndAttack1;		// ȿ���� : ���� 1 //client�� ���

#ifdef __CLIENT
    TCHAR	szIcon[64];			// dds���� �̸� 
	TCHAR	szCommand[256];		// ���� 
#endif
	int		nVer;

	DWORD	dwAdjMonsterGrade;		// ���� ���� ���� ���

	// ���� ����
	DWORD	dwEquipItemKeepSkill;	// ���� ��ų ������ �ʿ��� ������

	bool	bCanUseActionSlot;		// �׼� ���Կ� ����� �� �ִ� ��ų�ΰ�


//////////////////////////////////////////////////////////////////////////
	
	BOOL	IsDebuff()
	{
		if( nEvildoing < 0 )
		{
			return TRUE;
		}

		return FALSE;
	}
};


// ������ ���� factor ENUM
enum JOB_PROP_TYPE
{
	JOB_PROP_SWD,
	JOB_PROP_AXE,	
	JOB_PROP_STAFF,
	JOB_PROP_STICK,
	JOB_PROP_KNUCKLE,
	JOB_PROP_WAND,
	JOB_PROP_BLOCKING,
	JOB_PROP_YOYO,
	JOB_PROP_CRITICAL,
};

// ������ ���� ������Ƽ ( propJob.inc���� �о���� )
struct JobProp
{
	float	fAttackSpeed;			//���� 
	float	fFactorMaxHP;			//�ִ� HP ��꿡 ���Ǵ� factor
	float	fFactorMaxMP;			//�ִ� MP ��꿡 ���Ǵ� factor
	float	fFactorMaxFP;			//�ִ� FP ��꿡 ���Ǵ� factor
	float   fFactorDef;				//���� ���� ��꿡 ���Ǵ� factor
	float	fFactorHPRecovery;		//HPȸ�� factor
	float	fFactorMPRecovery;		//MPȸ�� factor
	float	fFactorFPRecovery;		//FPȸ�� factor
	float	fMeleeSWD;				//WT_MELEE_SWD�� ATK factor
	float	fMeleeAXE;				//WT_MELEE_AXE�� ATK factor
	float   fMeleeSTAFF; 			//WT_MELEE_STAFF�� ATK factor
	float   fMeleeSTICK;			//WT_MELEE_STICK�� ATK factor
	float   fMeleeKNUCKLE;			//WT_MELEE_KNUCKLE�� ATK factor
	float   fMagicWAND;				//WT_MAGIC_WAND�� ATK factor 
	float   fBlocking;				//���ŷ factor
	float	fMeleeYOYO;				//����� ATK factor 
	float   fCritical;				//ũ��Ƽ�� ó��
	TCHAR	szMarkFileName[ 32 ];		//��ũ���ϸ�
};

typedef	struct	tagRANDOM_ITEM
{
	BYTE	m_lpQuestFlagArray[ITEM_KIND_MAX / 8 + 1];

	void	SetItemKind( int nItemKind,BOOL bBool );
	BOOL	IsItemKind( int nItemKind );

	BYTE	m_nTotalNum;	// �߻� ������ ���� 
	BYTE	m_nUniqueMax;	// ����ũ ������ �ִ� 
	BYTE	m_nUniqueMin;	// ����ũ ������ �ּ�  
	WORD	m_nGoldMax;		// ��� �ƽ� 
	WORD	m_nAmmoMax;		// �Ѿ� �ƽ� 
	WORD	m_nDartMax;		// ǥâ �ƽ� 
	WORD	m_nGoldMin;		// ��� �ƽ� 
	WORD	m_nAmmoMin;	// �Ѿ� �ƽ� 
	WORD	m_nDartMin;	// ǥâ �ƽ� 
	tagRANDOM_ITEM()
	{
		::memset( m_lpQuestFlagArray, 0, sizeof( m_lpQuestFlagArray ) );

		m_nTotalNum		= 0;
		m_nUniqueMax	= 0;
		m_nUniqueMin	= 0;
		m_nGoldMax		= 0;
		m_nAmmoMax		= 0;
		m_nDartMax		= 0;
		m_nGoldMin		= 0;
		m_nAmmoMin		= 0;
		m_nDartMin		= 0;
	}
}
RANDOM_ITEM,* LPRANDOM_ITEM;

/*----------------------------------------------------------------------------------------------------*/
enum DROPTYPE
{
	DROPTYPE_NORMAL,
	DROPTYPE_SEED,
};

typedef struct	tagDROPITEM
{
	DROPTYPE	dtType;
	DWORD	dwIndex;
	DWORD	dwProbability;
	DWORD	dwLevel;
	DWORD	dwNumber;
	DWORD	dwNumber2;	// Min, Max�� Max�� ��.
}
DROPITEM,	*LPDROPITEM;

typedef	struct	tagDROPKIND
{
	DWORD	dwIK3;
	short	nMinUniq;
	short	nMaxUniq;
}
DROPKIND,	*LPDROPKIND;

typedef	struct	tagQUESTITEM
{
	DWORD	dwQuest;
	DWORD   dwState;
	DWORD	dwIndex;
	DWORD	dwProbability;
	DWORD	dwNumber; 
}
QUESTITEM,	*PQUESTITEM;

typedef struct tagEVENTITEM
{
	DWORD	dwIndex;
	DWORD	dwPrabability;
}
EVENTITEM,	*PEVENTITEM;

#define MAX_QUESTITEM	16
#define	MAX_DROPKIND	80

class CDropItemGenerator
{
private:
	vector<DROPITEM>	m_dropItems;

public:
	DWORD				m_dwMax;

public:
//	Contructions
	CDropItemGenerator() { m_dwMax = 0; }
	virtual	~CDropItemGenerator()	{	Clear();	}

//	Operations
	void		AddTail( CONST DROPITEM & rDropItem, const char* s );
	void	Clear( void )	{	m_dropItems.clear();	}
	DWORD		GetSize( void )	{	return m_dropItems.size();	}
	DROPITEM*	GetAt( int nIndex, BOOL bUniqueMode, float fProbability = 0.0f );
};

class CDropKindGenerator
{
private:
	int		m_nSize;
	DROPKIND	m_aDropKind[MAX_DROPKIND];
public:
//	Contructions
	CDropKindGenerator()
		{	
			m_nSize	= 0;	
			memset( m_aDropKind, 0, sizeof( m_aDropKind ) );
		}
	virtual	~CDropKindGenerator()	{}
//	Operations
	void	AddTail( const DROPKIND & rDropKind );	// memcpy
	int		GetSize( void )		{	return m_nSize;	}
	LPDROPKIND	GetAt( int nIndex );
};

class CQuestItemGenerator
{
private:
	u_long	m_uSize;
	QUESTITEM	m_pQuestItem[MAX_QUESTITEM];
public:
	CQuestItemGenerator()		{	m_uSize	= 0;	::memset( m_pQuestItem, 0, sizeof( m_pQuestItem ) );	}
	~CQuestItemGenerator()	{}
	void	AddTail( const QUESTITEM & rQuestItem );
	u_long	GetSize( void )	{	return m_uSize;	}
	QUESTITEM*	GetAt( int nIndex );
};

struct MonsterTransform
{
	float fHPRate;
	DWORD dwMonsterId;
	MonsterTransform() : fHPRate( 0.0f ), dwMonsterId( NULL_ID ) {}
};

/*----------------------------------------------------------------------------------------------------*/

struct MoverProp : CtrlProp
{
#ifdef __WORLDSERVER
	struct FLDropItemAttr
	{
		FLDropItemAttr() : bDropOwnerShipOff( FALSE ), bLevelDropPanaltyOff( FALSE ) {}
		BOOL			bDropOwnerShipOff;
		BOOL			bLevelDropPanaltyOff;
	};

	FLDropItemAttr		m_kDropItemAttr;
#endif //__WORLDSERVER

	DWORD	dwStr;	// ��,                  
	DWORD	dwSta;	// ü��,
	DWORD	dwDex;	// ��ø,
	DWORD	dwInt;	// ����,
	DWORD	dwHR;
	DWORD   dwER;
	DWORD	dwRace;		// ����,
	DWORD	dwBelligerence;		// ȣ����,
	DWORD	dwGender;	// ����,
	DWORD	dwLevel;	// ����,
	DWORD	dwFlightLevel;	// ���෹��
	DWORD	dwSize;		// ũ��,
	DWORD   dwClass;
	BOOL	bIfParts;	// ������?
	int		nChaotic;	// ���۳� ���̳ʽ�/ ������ �÷���
#ifdef __S1108_BACK_END_SYSTEM
	DWORD	dwUseable;	// ��� ĳ����,
#else // __S1108_BACK_END_SYSTEM
	DWORD	dwDefExtent;	// ��� ĳ����,
#endif // __S1108_BACK_END_SYSTEM
	DWORD	dwActionRadius;		// �����ൿ����,
	DWORD	dwAtkMin;	// �ּ�Ÿ��ġ,
	DWORD	dwAtkMax;	// �ִ�Ÿ��ġ,
	DWORD	dwAtk1;
	DWORD	dwAtk2;
	DWORD	dwAtk3;
	DWORD	dwAtk4;		// dwHorizontalRate�� �̰ɷ� �ٲ�.
//sun: 9,	//__AI_0509
	FLOAT	fFrame;	// �̵� �� ������ ����ġ
	DWORD	dwOrthograde;

	DWORD	dwThrustRate;	// ������,

	DWORD	dwChestRate;
	DWORD	dwHeadRate;  
	DWORD	dwArmRate;
	DWORD	dwLegRate;

	DWORD	dwAttackSpeed;	// ���ݼӵ�,
	DWORD	dwReAttackDelay;
	DWORD	dwAddHp;		// ,
	DWORD	dwAddMp;		// ,
	DWORD	dwNaturalArmor;	// �ڿ����� 
	int		nAbrasion;	// ����
	int		nHardness;	// �浵
	DWORD	dwAdjAtkDelay;	// �߰����ݼӵ�����,

	SAI79::ePropType	eElementType;
	short				wElementAtk;		// �Ӽ� ������( �� �Ӽ� Ÿ������ ���ݷ��� �����Ѵ�. )

	DWORD	dwHideLevel;	// ���� �Ⱥ��̴³��̳�..
	FLOAT	fSpeed;	// �̵��ӵ�,
	DWORD	dwShelter;	// ��������,
	DWORD	dwFlying;	// ��������,
	DWORD	dwJumpIng;	// ���̶ٱ� 
	DWORD	dwAirJump;	// �ָ��ٱ�
	DWORD	bTaming;	// �������� 
	DWORD	dwResisMgic;	//�������� 

	int		nResistElecricity;
	int		nResistDark;
	int		nResistFire;
	int		nResistWind;
	int		nResistWater;
	int		nResistEarth;
	
	DWORD	dwCash;		// �����ݾ�
	DWORD	dwSourceMaterial;	// �������
	DWORD	dwMaterialAmount;	// ����
	DWORD	dwCohesion;	// ���������
	DWORD	dwHoldingTime;	// ��ü�����ð�
	DWORD	dwCorrectionValue;	// �����ۻ���������
	EXPINTEGER	nExpValue;
	int		nFxpValue;		// �������ġ.
	DWORD	nBodyState;		// ������,
	DWORD	dwAddAbility;	// �߰��ɷ�,
	DWORD	bKillable;	// ��������,

	DWORD	dwVirtItem[3];
	DWORD	bVirtType[3]; 

	DWORD   dwSndAtk1  ;
	DWORD   dwSndAtk2  ;
	DWORD   dwSndDie1  ;
	DWORD   dwSndDie2  ;
	DWORD   dwSndDmg1  ;
	DWORD   dwSndDmg2  ;
	DWORD   dwSndDmg3  ;
	DWORD   dwSndIdle1 ;
	DWORD   dwSndIdle2 ;

	//gmpbigsun( 20111226 ) �߰���
	DWORD	dwAreaColor;
	TCHAR	szMarkFileName[32];

	int		nMadrigalGiftPoint;

	short   m_nEvasionHP;
	short	m_nEvasionSec;
	short   m_nRunawayHP          ; // HP�� 10 ���ϸ� ���� 
	short   m_nCallHelperMax       ; // �� ���� 
	short   m_nCallHP              ; // �����û�ϱ� ���� HP
	short   m_nCallHelperIdx  [ 5 ]; // �����û�ϱ� Id
	short   m_nCallHelperNum  [ 5 ]; // �����û�ϱ� Id
	short   m_bCallHelperParty[ 5 ]; // �����û�ϱ� Id

	short   m_dwAttackMoveDelay;
	short   m_dwRunawayDelay;
	short   m_bPartyAttack;
	short   m_nHelperNum;

#if !defined(__CORESERVER) 
	int		m_nScanJob;		// Ÿ���� �˻��Ҷ� Ư�� �������� �˻��ϴ°�. 0 �̸� ALL
	// �������ǵ��� ��Ʈ���� �������� �����Ǿ� ������ ����.
	int		m_nHPCond;		// Ÿ���� hp% �����϶� ������ ��.
	int		m_nLvCond;		// Ÿ�ٰ� �������Ͽ� ������ ������ ��.
	int		m_nRecvCondMe;	// ȸ�� ����.  ��hp�� ��%���Ϸ� ����������?
	int		m_nRecvCondHow;	// ȸ���Ҷ� ȸ���� �� %����
	int		m_nRecvCondMP;	// ȸ���Ҷ� MP�Ҹ� %����
	BYTE	m_bMeleeAttack;	// ���� ���� AI�� �ִ°�?
	BYTE	m_bRecvCondWho;	// ������ ġ���Ҳ���. 0:�����ȵ� 1:�ٸ��� 2:�� 3:���.
	BYTE	m_bRecvCond;	// ġ���ϳ�? 0:ġ����� 1:�����߿��� ġ���� 2:����/������ ��� ġ��
	BYTE	m_bHelpWho;		// �����û�� - 0:�θ������� 1:�ƹ���  2:����������.
	BYTE	m_bRangeAttack[ MAX_JOB ];		// �� ������ ���Ÿ� ���� �Ÿ�.
	int		m_nSummProb;	// ��ȯ Ȯ�� : 0�̸� ��ȯ�ɷ� ����.
	int		m_nSummNum;		// �ѹ��� ����� ��ȯ�ϳ�.
	int		m_nSummID;		// � ����?
	int		m_nHelpRangeMul;	// �����û �Ÿ�. �þ��� ���
	DWORD	m_tmUnitHelp;			// ���� Ÿ�̸�.
	int		m_nBerserkHP;		// ����Ŀ�� �Ǳ� ���� HP%
	float	m_fBerserkDmgMul;	// ����Ŀ�� �Ǿ����� ������ ���.
	int		m_nLoot;			// ���ø��ΰ�.
	int		m_nLootProb;		// ���� Ȯ��
	DWORD   m_dwScanQuestId; 
	DWORD   m_dwScanItemIdx; 
	int		m_nScanChao;		// ī��, ��ī�� �˻�
#endif // !__CORESERVER
	
#ifdef __S1108_BACK_END_SYSTEM
	float	m_fHitPoint_Rate;		// ���� �ִ� HP�� // dwAddHp * m_nHitPoint_Rate
	float	m_fAttackPower_Rate;	// ���� �ִ� ���ݷ� // dwAtkMin * m_nAttackPower_Rate
	float	m_fDefence_Rate;		// ���� �ִ� ���� // dwAddHp * m_nDefence_Rate
	float	m_fExp_Rate;			// ���� �ִ� ����ġ�� // dwAddHp * m_nExp_Rate
	float	m_fItemDrop_Rate;		// ���� �ִ� ������ ��ӷ� // dwAddHp * m_nItemDrop_Rate
	float	m_fPenya_Rate;			// ���� �ִ� ��ķ� // dwAddHp * m_nPenya_Rate
	BOOL	m_bRate;
#endif // __S1108_BACK_END_SYSTEM

	
	short	m_nAttackItemNear;
	short	m_nAttackItemFar;
	short	m_nAttackItem1;
	short	m_nAttackItem2;
	short	m_nAttackItem3;
	short	m_nAttackItem4;
	short	m_nAttackItemSec;
	short	m_nMagicReflection;
	short	m_nImmortality;
	BOOL 	m_bBlow;
	short	m_nChangeTargetRand;

	short   m_nAttackFirstRange;
	RANDOM_ITEM		m_randomItem  ;
	CDropItemGenerator	m_DropItemGenerator;
	CQuestItemGenerator		m_QuestItemGenerator;
	CDropKindGenerator	m_DropKindGenerator;
	MonsterTransform	m_MonsterTransform;


// ��Ƽ ����
	struct	FLMonsterKey		//�ߺ��� MoverIndex�� ����Ҽ� �ֵ����ϴ� ��ü
	{
		DWORD				dwMoverID;
		DWORD				dwSequence;
		bool operator < ( const FLMonsterKey & l ) const	{	return ( dwMoverID < l.dwMoverID ) || ( dwMoverID == l.dwMoverID && dwSequence < l.dwSequence ); }
		bool operator == ( const FLMonsterKey & l ) const	{	return ( dwMoverID == l.dwMoverID && dwSequence == l.dwSequence ); }
	};
	typedef					std::map< FLMonsterKey, DWORD /* MoverIndex */ >	SummonMonstersMap;
	SummonMonstersMap		m_mapSummonMonsters;
// ��Ƽ ����
	
	MoverProp()
	{
		dwStr	= dwSta	= dwDex	= dwInt	= dwHR	= dwER	= dwRace	= dwBelligerence	=dwGender
		= dwLevel	= dwFlightLevel	= dwSize	= dwClass	= bIfParts	= nChaotic
		#ifdef __S1108_BACK_END_SYSTEM
		= dwUseable
		#else // __S1108_BACK_END_SYSTEM
		= dwDefExtent
		#endif // __S1108_BACK_END_SYSTEM
		= dwActionRadius	= dwAtkMin	= dwAtkMax	= dwAtk1	= dwAtk2	= dwAtk3	= dwAtk4	= 0;
		//sun: 9,	//__AI_0509
			fFrame	= 1.0F;
			dwOrthograde	= 0;
		
		//!	dwVerticalRate	= dwDiagonalRate	= 0;
		
		dwThrustRate		= 0;
		dwChestRate			= 0;
		dwHeadRate			= 0;
		dwArmRate			= 0;
		dwLegRate			= 0;
		dwAttackSpeed		= 0;
		dwReAttackDelay		= 0;
		dwAddHp				= 0;
		dwAddMp				= 0;
		dwNaturalArmor		= 0;
		nAbrasion			= 0;
		nHardness			= 0;
		dwAdjAtkDelay		= 0;
		dwHideLevel			= 0;
		eElementType		= SAI79::NO_PROP;
		wElementAtk			= 0;
		fSpeed				= 0.0F;
		dwShelter			= 0;
		dwFlying			= 0;
		dwJumpIng			= 0;
		dwAirJump			= 0;
		bTaming				= 0;
		dwResisMgic			= 0;
		nResistElecricity	= 0;
		nResistDark			= 0;
		nResistFire			= 0;
		nResistWind			= 0;
		nResistWater		= 0;
		nResistEarth		= 0;
		dwCash				= 0;
		dwSourceMaterial	= 0;
		dwMaterialAmount	= 0;
		dwCohesion			= 0;
		dwHoldingTime		= 0;
		dwCorrectionValue	= 0;
		nExpValue			= 0;
		nFxpValue			= 0;
		nBodyState			= 0;
		dwAddAbility		= 0;
		bKillable			= 0;
		
		memset( dwVirtItem, 0, sizeof(dwVirtItem) );
		memset( bVirtType, 0, sizeof(bVirtType) );

		dwSndAtk1			= 0;
		dwSndAtk2			= 0;
		dwSndDie1			= 0;
		dwSndDie2			= 0;
		dwSndDmg1			= 0;
		dwSndDmg2			= 0;
		dwSndDmg3			= 0;
		dwSndIdle1			= 0;
		dwSndIdle2			= 0;
		m_nEvasionHP		= 0;
		m_nEvasionSec		= 0;
		m_nRunawayHP		= 0;
		m_nCallHelperMax	= 0;
		m_nCallHP			= 0;
		
		memset( m_nCallHelperIdx, 0, sizeof(m_nCallHelperIdx) );
		memset( m_nCallHelperNum, 0, sizeof(m_nCallHelperNum) );
		memset( m_bCallHelperParty, 0, sizeof(m_bCallHelperParty) );
		
		m_dwAttackMoveDelay	= 0;
		m_dwRunawayDelay	= 0;
		m_bPartyAttack		= 0;
		m_nHelperNum		= 0;

#if !defined(__CORESERVER) 
		m_nScanJob			= 0;
		m_nHPCond			= 0;
		m_nLvCond			= 0;
		m_nRecvCondMe		= 0;
		m_nRecvCondHow		= 0;
		m_nRecvCondMP		= 0;
		m_bMeleeAttack		= 0;
		m_bRecvCondWho		= 0;
		m_bRecvCond			= 0;
		m_bHelpWho			= 0;
		memset( m_bRangeAttack, 0, sizeof(m_bRangeAttack) );
		m_nSummProb			= 0;
		m_nSummNum			= 0;
		m_nSummID			= 0;
		m_nHelpRangeMul		= 0;
		m_tmUnitHelp		= 0;
		m_nBerserkHP		= 0;
		m_fBerserkDmgMul	= 0;
		m_nLoot				= 0;
		m_nLootProb			= 0;
		m_dwScanQuestId		= 0;
		m_dwScanItemIdx		= 0;
		m_nScanChao			= 0;
#endif // !__CORESERVER

#ifdef __S1108_BACK_END_SYSTEM
		m_fHitPoint_Rate	=
		m_fAttackPower_Rate	= 
		m_fDefence_Rate	=
		m_fExp_Rate	= 
		m_fItemDrop_Rate	= 
		m_fPenya_Rate	= 0.0F;
		m_bRate	=		FALSE;
#endif // __S1108_BACK_END_SYSTEM
		m_nAttackItemNear	=
		m_nAttackItemFar	=
		m_nAttackItem1	=
		m_nAttackItem2	=
		m_nAttackItem3	=
		m_nAttackItem4	=
		m_nAttackItemSec	=
		m_nMagicReflection	=
		m_nImmortality	=	0;
		m_bBlow	=		FALSE;
		m_nChangeTargetRand	=
		m_nAttackFirstRange	= 0;
	}
};

typedef	struct	tagBeginItem
{
	DWORD	dwItemType;
	DWORD	dwItemId;
	DWORD	dwNum;
}
BEGINITEM, *LPBEGINITEM; 

#define MAX_BEGINEQUIP  10
#define MAX_BEGINITEM   42 //20 
#define MAX_JOBITEM     10 

typedef struct tagJOBITEM
{
	DWORD adwMale[ MAX_BEGINEQUIP ][ 2 ];
	BEGINITEM beginItem[ MAX_BEGINITEM ];

} JOBITEM,* LPJOBITEM;

#define	MAX_BEGINPOS	16

typedef	struct tagBEGINPOSARR
{
	u_long	uSize;
	D3DXVECTOR3 avPos[MAX_BEGINPOS];
}
BEGINPOSARR, *LPBEGINPOSARR;

#define TASKBAR_TOP    0
#define TASKBAR_BOTTOM 1
#define TASKBAR_LEFT   2
#define TASKBAR_RIGHT  3

// short cut
#if __VER < 19

#define MAX_SLOT_APPLET			18
#define MAX_SLOT_ITEM			9
#define MAX_SLOT_QUEUE			5
#define	MAX_SLOT_ITEM_COUNT		8

#else

const size_t	MAX_SLOT_APPLET			= 18;
const size_t	MAX_SLOT_ITEM_COUNT		= 5;
const size_t	MAX_SLOT_ITEM			= 10;
const size_t	MAX_SLOT_QUEUE			= 6;

#endif // __VER < 19

#define	SHORTCUT_NONE    0
#define	SHORTCUT_ETC     1
#define	SHORTCUT_APPLET  2
#define	SHORTCUT_MOTION  3
#define	SHORTCUT_SCRIPT  4
#define	SHORTCUT_ITEM    5
//#define	SHORTCUT_CARD    5
//#define	SHORTCUT_JACKBOX 6
#define	SHORTCUT_SKILL   6
#define SHORTCUT_OBJECT  7
#define SHORTCUT_CHAT    8
#define SHORTCUT_SKILLFUN 9
#define SHORTCUT_EMOTICON 10
#define SHORTCUT_LORDSKILL	 11


#if defined ( __CLIENT )
class CWndBase;
#endif	// __CLIENT



//---------------------------------------------------------------------------------------------
// ���� ������ ���� - lenahyang �ʱ�ȭ ����
//---------------------------------------------------------------------------------------------
typedef struct tagSHORTCUT
{
// �����ڵ� 
//#if defined ( __CLIENT )
//	CWndBase* m_pFromWnd   ;
//	CTexture* m_pTexture   ;
//	DWORD	m_dwItemId;			//sun: 8�� �۾�
//	int m_nExtraFrame;			//v19 quick slot bar
//
//	tagSHORTCUT::tagSHORTCUT( void ) : m_pFromWnd( NULL ), m_pTexture( NULL ), m_nExtraFrame ( 0 ) {}
//#endif	// __CLIENT
//	DWORD     m_dwShortcut ; 
//	DWORD     m_dwId       ; 
//	DWORD     m_dwType     ; 
//	DWORD     m_dwIndex    ; 
//	DWORD     m_dwUserId   ; 
//	DWORD     m_dwData     ; 
//	TCHAR     m_szString[MAX_SHORTCUT_STRING]; // SHORTCUT_CHAT�� ��� ����.
//	BOOL IsEmpty() { return m_dwShortcut == SHORTCUT_NONE; }
//	void Empty() { m_dwShortcut = SHORTCUT_NONE; }


#if defined ( __CLIENT )

	CWndBase*		m_pFromWnd;
	CTexture*		m_pTexture;
	DWORD			m_dwItemId;							//sun: 8�� �۾�
	int				m_nExtraFrame;						//v19 quick slot bar
	DWORD			m_dwShortcut; 
	DWORD     		m_dwId; 
	DWORD     		m_dwType; 
	DWORD     		m_dwIndex; 
	DWORD     		m_dwUserId; 
	DWORD     		m_dwData; 
	TCHAR     		m_szString[MAX_SHORTCUT_STRING];	// SHORTCUT_CHAT�� ��� ����.

	tagSHORTCUT::tagSHORTCUT( void )
	:	m_pFromWnd( NULL ) 
	,	m_pTexture( NULL )
	,	m_dwItemId( NULL )
	,	m_nExtraFrame( NULL )
	,	m_dwShortcut( SHORTCUT_NONE )
	,	m_dwId( NULL ) 
	,	m_dwType( NULL )
	,	m_dwIndex( NULL )
	,	m_dwUserId( NULL )
	,	m_dwData( NULL ) 
	{
		ZeroMemory( m_szString, sizeof(TCHAR) * MAX_SHORTCUT_STRING );	
	}

	void	Empty( void )
	{
		m_pFromWnd		= NULL; 
		m_pTexture		= NULL;
		m_dwItemId		= NULL;
		m_nExtraFrame	= NULL;
		m_dwShortcut	= SHORTCUT_NONE;
		m_dwId			= NULL; 
		m_dwType		= NULL;
		m_dwIndex		= NULL;
		m_dwUserId		= NULL;
		m_dwData		= NULL; 

		ZeroMemory( m_szString, sizeof(TCHAR) * MAX_SHORTCUT_STRING );	
	}

#else

	DWORD			m_dwShortcut; 
	DWORD     		m_dwId; 
	DWORD     		m_dwType; 
	DWORD     		m_dwIndex; 
	DWORD     		m_dwUserId; 
	DWORD     		m_dwData; 
	TCHAR     		m_szString[MAX_SHORTCUT_STRING];	// SHORTCUT_CHAT�� ��� ����.

	tagSHORTCUT::tagSHORTCUT( void )
	:	m_dwShortcut( SHORTCUT_NONE )
	,	m_dwId( NULL ) 
	,	m_dwType( NULL )
	,	m_dwIndex( NULL )
	,	m_dwUserId( NULL )
	,	m_dwData( NULL ) 
	{
		ZeroMemory( m_szString, sizeof(TCHAR) * MAX_SHORTCUT_STRING );	
	}

	void	Empty( void )
	{
		m_dwShortcut	= SHORTCUT_NONE;
		m_dwId			= NULL; 
		m_dwType		= NULL;
		m_dwIndex		= NULL;
		m_dwUserId		= NULL;
		m_dwData		= NULL; 

		ZeroMemory( m_szString, sizeof(TCHAR) * MAX_SHORTCUT_STRING );	
	}

#endif

	BOOL	IsEmpty( void ) const		{ return m_dwShortcut == SHORTCUT_NONE; }

} SHORTCUT,* LPSHORTCUT;


typedef struct tagEXPPARTY
{
	DWORD	Exp;
	DWORD	Point;
}
EXPPARTY, *LPEXPPARTY;

typedef struct tagRENEWEVENT
{
	int		nLevel;
	DWORD	dwItemId;
	TCHAR	strItemName[64];
	float	fPercent;
}
RENEWEVENT, *LPRENEWEVENT;

typedef struct tagRENEWEVENTITEM
{
	TCHAR strTime[32];
	int nLevel;
	int	nRealitem[ MAX_REALITEM ];
	int nRealitemCount[ MAX_REALITEM ];
	int nSendRealitemCount[ MAX_REALITEM ];
	int nPercent[ MAX_REALITEM ];
	int nNextIndex;
}
RENEWEVENTITEM, *LPRENEWEVENTITEM;


typedef struct tagExpCharacter
{
	EXPINTEGER	nExp1;
	EXPINTEGER	nExp2;
	DWORD dwLPPoint;
	EXPINTEGER	nLimitExp;
}
EXPCHARACTER, *LPEXPCHARACTER;

#endif	// __PROJECTCMN_H__