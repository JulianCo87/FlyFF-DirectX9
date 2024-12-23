#ifndef __FLSPECSTRUCT_H__
#define __FLSPECSTRUCT_H__


#include "../../_Common/data.h"


typedef struct _T_ITEM_SPEC
{
	int		nVer;							// ����
	DWORD	dwID;							// ������	
	TCHAR	szName[ ITEM_NAME_BUFFER_LEN ];	// �ѱ۸�Ī 
	DWORD	dwNum;							// �⺻��������	
	DWORD	dwPackMax;						// �ִ��ħ����	
	
	DWORD	dwItemKind1;					// 1������������, Skill: ���� ��ų ����?(�����, 1,2,3�� ������ų)
	DWORD	dwItemKind2;					// 2������������	
	DWORD	dwItemKind3;					// 3������������	
	DWORD	dwItemJob;						// �����������Ҽ� 
	BOOL	bPermanence;					// �Һ�Ұ�	
	
	DWORD	dwUseable;						// ������	
	DWORD	dwItemSex;						// ��뼺��	
	DWORD	dwCost;							// ����	 
	DWORD	dwEndurance;					// ������	
	int		nLog;							// �α�
	
	int		nAbrasion;						// ������
	int		nMaxRepair;						// ����Ƚ��
	DWORD	dwHanded;						// �������	
	DWORD	dwFlag;							// �ٸ��� �÷���	
	DWORD	dwParts;						// ������ġ	
	
	DWORD	dwPartsub;						// ������ġ	
	BOOL	bPartsFile;						// �������� ��뿩�� 
	DWORD	dwExclusive;					// ���ŵ���ġ	
	DWORD	dwBasePartsIgnore;
	DWORD	dwItemLV;						// �����۷���	
	
	DWORD	dwItemRare;						// ��ͼ�	
	DWORD   dwShopAble;
	int		nShellQuantity;					// �ִ���������	- IK�� ä�������϶��� �ִ� ä������ �ȴ�.(���� ����)
	DWORD	dwActiveSkillLv;				// �ߵ� ���� ����
	DWORD   dwFuelRe;						// �Ѵ翬��������
	
	DWORD	dwAFuelReMax;					// ���ӿ���������
	DWORD	dwSpellType;					// �����Ӽ�	
	DWORD	dwLinkKindBullet;				// �ʿ� �Ҹ���� ������	
	DWORD	dwLinkKind;						// �ʿ� ����������
	DWORD	dwAbilityMin;					// �ּҴɷ�ġ - ���ݷ�, ����, ���� ��Ÿ��� 
	
	DWORD	dwAbilityMax;					// �ִ�ɷ�ġ - ���ݷ�, ����, ���� ��Ÿ��� 
	BOOL	bCharged;		
	SAI79::ePropType	eItemType;
	short	wItemEatk;						// �Ӽ� ������( �� �Ӽ� Ÿ������ ���ݷ��� �����Ѵ�. )
	DWORD   dwParry;						// ȸ���� 
	
	DWORD   dwblockRating;					// �� ��ġ 
	int		nAddSkillMin;					// �ּ� �߰� ��ų
	int		nAddSkillMax;					// �ִ� �߰� ��ų.
	DWORD	dwAtkStyle;						// ���� ��Ÿ�� 
	DWORD	dwWeaponType;					// �������� 
	
	DWORD	dwItemAtkOrder1;				// ������1�����ݼ���
	DWORD	dwItemAtkOrder2;				// ������2�����ݼ���
	DWORD	dwItemAtkOrder3;				// ������3�����ݼ���
	DWORD	dwItemAtkOrder4;				// ������4�����ݼ���
	DWORD	tmContinuousPain;				// ���� ���� 
	
	DWORD	dwRecoil;						// �ݵ�	
	DWORD	dwLoadingTime;					// �����ð�	- IK�� ä�������϶��� ä���ӵ�(�ɷ�)�� �ȴ�.
	LONG	nAdjHitRate;					// �߰����ݼ�����	
	FLOAT	fAttackSpeed;					// ���ݼӵ�	
	DWORD	dwDmgShift;						// Ÿ�ݽ� �ݵ�	
	
	DWORD	dwAttackRange;					// ���ݹ���	
	int		nProbability;					// ����Ȯ��

	DWORD	dwDestParam[ MAX_ITEM_PARAM ];					// ������1		// TODO : 
	LONG	nAdjParamVal[ MAX_ITEM_PARAM ];				// ���밪1	
	DWORD	dwChgParamVal[ MAX_ITEM_PARAM ];				// ���뺯ȭ��1	
	int		nDestData1[ MAX_ITEM_PARAM ];					// ���뵥��Ÿ�� 3��, destParam1���� �ش��.
	
	DWORD	dwActiveSkill;					// �ߵ� ����
	DWORD	dwActiveSkillRate;				// �ߵ� ���� Ȯ��.
	DWORD	dwReqMp;						// �ʿ�MP	
	DWORD	dwReqFp;						// �ʿ�FP
	
	DWORD	dwReqDisLV;						// �ʿ� ����
	DWORD   dwReSkill1;						// ���� ��ų1
	DWORD   dwReSkillLevel1;				// ���� ��ų1 �ʿ� ����
	DWORD   dwReSkill2;						// ���� ��ų2
	DWORD   dwReSkillLevel2;				// ���� ��ų2 �ʿ� ����
	
	DWORD	dwSkillReadyType;
	DWORD	dwSkillReady;					// ��� �غ�ð�	
	DWORD	_dwSkillRange;					// ��� �����Ÿ�	
	DWORD	dwSfxElemental;
	DWORD	dwSfxObj;						// ������ ȿ�� ������Ʈ 
	
	DWORD	dwSfxObj2;						// ������ ȿ�� ������Ʈ 
	DWORD	dwSfxObj3;						// �ߵ�ȿ��, Ÿ��
	DWORD	dwSfxObj4;						// ���ӵǴ� ȿ�� ����Ʈ.
	DWORD	dwSfxObj5;						// �ߵ�ȿ��, ������ 
	DWORD	dwUseMotion;					// ��� ���
	
	DWORD	dwCircleTime;					// �����ð� 
	DWORD	dwSkillTime;					// ���ӽð�	
	DWORD	dwExeTarget;					// �ߵ� ���
	DWORD	dwUseChance;					// ���ñ�?
	DWORD	dwSpellRegion;					// ���� ���� 
	
	DWORD   dwReferStat1;
	DWORD   dwReferStat2;
	DWORD   dwReferTarget1;					// ��ȯ�� ���⼳��
	DWORD   dwReferTarget2;					// ��ȯ�� Ȯ�� ����		// ��ȯ�Ҷ� �Һ�Ǵ� ���
	DWORD   dwReferValue1;					// ����Ĩ
	
	DWORD   dwReferValue2;					// �����ġ
	DWORD	dwSkillType;					// ����Ӽ�	
	int		nItemResistElecricity;
	int		nItemResistFire;
	
	int		nItemResistWind;
	int		nItemResistWater;
	int		nItemResistEarth;
	LONG	nEvildoing;						// ����	
	DWORD	dwExpertLV;	
	
	DWORD	dwExpertMax;					// �ִ���÷���
	DWORD	dwSubDefine;
	DWORD	dwExp;							// �������ġ	
	DWORD	dwComboStyle;
	FLOAT	fFlightSpeed;					// ����������(�ӵ�)
	
	FLOAT	fFlightLRAngle;					// �¿� �� ����.
	FLOAT	fFlightTBAngle;					// ���� �� ����.
	DWORD	dwFlightLimit;					// �������ѷ���
	DWORD	dwFFuelReMax;					// ���࿬��������
	DWORD	dwLimitLevel1;					// ���ѷ���1		<< ������� ����
	
	int		nReflect;						// ���÷��� �ɼ�.
	DWORD	dwSndAttack1;					// ȿ���� : ���� 1
	DWORD	dwSndAttack2;					// ȿ���� : ���� 2
	DWORD	dwQuestId;
	TCHAR	szTextFileName[64];				// item�� GM command�� �ִ� �Ϳ� ���

	TCHAR	szIcon[64];						// dds���� �̸� 
	TCHAR	szCommand[256];					// ���� 

//////////////////////////////////////////////////////////////////////////
// �߰� �÷�...
//////////////////////////////////////////////////////////////////////////

	int		nMinLimitLevel;					// �ּ� ��� ����(<=)
	int		nMaxLimitLevel;					// �ִ� ��� ����(<=)


	int		nItemGroup;						// ������ �׷�
	int		nUseLimitGroup;					// ��� ���� �׷�

	int		nMaxDuplication;				// ��ø ��� ����
	int		nEffectValue;					// ȿ�� ���� ��ġ
	int		nTargetMinEnchant;				// ��� ��� �ּ� ���� ��ġ(<=)
	int		nTargetMaxEnchant;				// ��� ��� �ִ� ���� ��ġ(<=)

	BOOL	bResetBind;						// �ͼ� ���� ���� ����
	int		nBindCondition;					// ���� �ͼ� ����
	int		nResetBindCondition;			// �ͼ� ���� ��� �� �ͼ� ����

	DWORD	dwHitActiveSkillId;				// Ÿ�� �� �ߵ��� ��ų �ε���
	DWORD	dwHitActiveSkillLv;				// Ÿ�� �� �ߵ��� ��ų ����
	DWORD	dwHitActiveSkillProb;			// Ÿ�� �� �ߵ��� ��ų Ȯ��
	DWORD	dwHitActiveSkillTarget;			// Ÿ�� �� �ߵ��� ��ų ���

	DWORD	dwDamageActiveSkillId;			// ������ �Ծ��� �� �ߵ��� ��ų �ε���
	DWORD	dwDamageActiveSkillLv;			// ������ �Ծ��� �� �ߵ��� ��ų ����
	DWORD	dwDamageActiveSkillProb;		// ������ �Ծ��� �� �ߵ��� ��ų Ȯ��
	DWORD	dwDamageActiveSkillTarget;		// ������ �Ծ��� �� �ߵ��� ��ų ���

	DWORD	dwEquipActiveSkillId;			// ���� �� �ߵ��� ��ų �ε���
	DWORD	dwEquipActiveSkillLv;			// ���� �� �ߵ��� ��ų ����

	DWORD	dwGeneralEnchantMode;			// �Ϲ� ���� ���� ����( 1 : ����, 0 : �Ұ���, = : ������ ����..tooltip ������..?)
	DWORD	dwAttributeEnchantMode;			// �Ӽ� ���� ���� ����
	DWORD	dwGemEnchantMode;				// ���� ���� ���� ����?
	DWORD	dwPiercingMode;					// �Ǿ�� ���� ����
	DWORD	dwRandomOptionMode;				// ���� ���� ����

	BOOL	bAbsoluteTime;					// �ð��� ������ ����ð� ��� ����

	DWORD	dwItemGrade;					// ������ ���
	BOOL	bCanTrade;						// �ŷ� ���� ����
	DWORD	dwMainCategory;					// ������ �з� 1��
	DWORD	dwSubCategory;					// ������ �з� 2��

	BOOL	bCanHaveServerTransform;		// ������ �̵� �� ������ �� �ִ� �������ΰ�

	BOOL	bCanSavePotion;					// ���� �Ҽ� �ִ� ��������.

	BOOL	bCanLooksChange;				// ���� ���� ������ �������ΰ�
	BOOL	bIsLooksChangeMaterial;			// ���� ���濡 ���� �� �ִ� ��� �ΰ�

	BOOL	bCanSellNPC;					// NPC ������ �Ǹ� �����Ѱ�


	//////////////////////////////////////////////////////////////////////////
	DWORD	GetCoolTime() const;
	BOOL	IsUltimate() const;
	BOOL	IsBaruna() const;
	BOOL	IsBarunaWeapon() const;
	BOOL	IsBarunaArmor() const;
#ifdef BARUNA_ULTIMATE_UPDATE
	BOOL	IsBarunaUltimate() const;
#endif
	BOOL	IsVisPet() const;
	BOOL	IsVis() const;
	BOOL	IsVisKey() const;
	BOOL	IsPickupToBuff() const;
	BOOL	IsGold() const;
	int		GetInventoryType() const;
	BOOL	IsUpgradeAble( int nUpgradeType ) const;
	BOOL	IsAbsoluteTime() const;
	BOOL	IsDebuff() const;

	BOOL	IsAccessory() const;
	BOOL	IsCollector() const;
	BOOL	IsWeaponParts() const;
	BOOL	IsArmorParts() const;
	BOOL	IsCostumeParts() const;
#ifdef COSTUME_UPGRADE_ENHANCEMENT_GEM
	BOOL	IsCostumeEnhanceParts() const;
#endif
	BOOL	IsSystemPet() const;
	BOOL	IsEatPet() const;
	//////////////////////////////////////////////////////////////////////////

} T_ITEM_SPEC, *PT_ITEM_SPEC;



//////////////////////////////////////////////////////////////////////////
// �ڷ���Ʈ ����
enum
{
	TELEPORT_MAP_TYPE_SPEC,	// ���忡 ��ϵ� ����
	TELEPORT_MAP_TYPE_USER,	// ���� ��� ����
};


#pragma pack( 1 )


typedef	struct	_T_WORLD_POSITION
{
	DWORD		dwWorldID;		// ���� ID
	D3DXVECTOR3	tPos;			// ��ġ
	int			layer;

	_T_WORLD_POSITION() : dwWorldID( 0 ), tPos( 0.0F, 0.0F, 0.0F ), layer( 0 )
	{
	}

} T_WORLD_POSITION, * PT_WORLD_POSITION;


typedef	struct	_T_TELEPORTMAP_POINT
{
	DWORD				dwIndex;		// �ε���
	BYTE				byType;			// �ڷ���Ʈ Ÿ��

	T_WORLD_POSITION	tPosition;		// ��ġ

	TCHAR				szName[128];	// �̸�(����) // ���� MAX_TELEPORT_MAP_USER_POINT_NAME

} T_TELEPORTMAP_POINT, * PT_TELEPORTMAP_POINT;

#pragma pack()

typedef struct _T_WORLD_EXPAND_LAYER
{
	DWORD			dwWorldID;			// ���� �ε���
	BYTE			byExpandLayerCount;	// Ȯ���� ���̾� ��

} T_WORLD_EXPAND_LAYER, *PT_WORLD_EXPAND_LAYER;

typedef struct _T_CHARGE_ZONE_WORLD
{
	DWORD			dwIndex;			// �ε���
	DWORD			dwWorldID;			// ���� �ε���
	BYTE			byChannelCount;		// ä�� ��
	D3DXVECTOR3		kStartingPos;		// ���� ��ġ
	TCHAR			szWorldName[128+1];	// �����

} T_CHARGE_ZONE_WORLD, *PT_CHARGE_ZONE_WORLD;

typedef struct	_T_CHARGE_ZONE_TICKET
{
	DWORD			dwItemID;			// ���� ���� ����� ������ �ε���
	DWORD			dwPointID;			// ���� ���� ���� ������ �ε���

} T_CHARGE_ZONE_TICKET, *PT_CHARGE_ZONE_TICKET;


enum
{
	TREASURE_KEY_LOW_LEVEL = 0
	, TREASURE_KEY_MEDIUM_LEVEL
	, TREASURE_KEY_SUPERIOR_LEVEL
	, TREASURE_KEY_MAX
};

typedef struct	_T_TREASURE_CHEST
{
	DWORD		dwItemID;							// ���� ���� ������ �ε���
	DWORD		arrKeyItemID[ TREASURE_KEY_MAX ];	// ���� ���� ���� ������ �ε���

} T_TREASURE_CHEST, *PT_TREASURE_CHEST;

typedef struct	_T_TREASURE_KEY
{
	DWORD		dwItemID;					// ���� ���� ���� ������ �ε���
	DWORD		dwTreasureItemID;			// ���� ������ �ε���
	DWORD		dwProb;						// ���� ������ Ȯ��
	DWORD		dwMaxQuantity;				// ���� ������ �ִ� ����
	DWORD		dwLoopCount;				// ���� Ƚ��
	DWORD		dwLimitCount;				// ���� ����
	bool		bNotice;					// �˸� ����

} T_TREASURE_KEY, *PT_TREASURE_KEY;

typedef struct	_T_TREASURE_ITEM
{
	DWORD		dwItemID;
	DWORD		dwQuantity;
	bool		bNotice;

} T_TREASURE_ITEM, *PT_TREASURE_ITEM;


// imParkth - 2012. 05. 17 - FLSpecStruct.h - Struct
typedef struct	_T_VIS_GROUP_LIST
{
	DWORD		dwVisGroupID;							// �� �׷�
	DWORD		dwGroupName;							// �� �����۸�
	DWORD		dwVisID[MAX_VIS_GROUP_ITEM_COUNT];		// �ش� �׷��� �����۵�

} T_VIS_GROUP_LIST, *PT_VIS_GROUP_LIST;

typedef struct	_T_VIS_GROUP_LIST_BY_ITEM_ID
{
	DWORD		dwVisItemID;							// �ش� �׷��� ������
	DWORD		dwVisGroupID;							// �� �׷�

} T_VIS_GROUP_LIST_BY_ITEM_ID, *PT_VIS_GROUP_LIST_BY_ITEM_ID;

typedef struct	_T_VIS_CHECK_LIST
{
	DWORD		dwVisItemID;							// �� ������ ID
	DWORD		dwGroupID[MAX_VIS_CONDITION_COUNT];		// ���� ������ �׷�� ( AND ���� )

} T_VIS_CHECK_LIST, *PT_VIS_CHECK_LIST;


#endif // __FLSPECSTRUCT_H__

