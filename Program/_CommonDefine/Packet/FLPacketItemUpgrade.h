
#ifndef	__FLPACKETITEMUPGRADE_H__
#define	__FLPACKETITEMUPGRADE_H__

#include "FLPacket.h"
#include "../_Common/FLRandomOption.h"


//////////////////////////////////////////////////////////////////////////
// client -> world

// �Ϲ� ���� ��û
class	FLPacketItemUpgradeGeneralEnchantReq : public FLPacket
{
public:

	FLPacketItemUpgradeGeneralEnchantReq();

	DWORD	dwUpgradeNpcObjID;				// ���� ���� NPC
	DWORD	dwMainItemObjID;				// ������ ������
	DWORD	dwMaterialItemObjID;			// ��� ������
	DWORD	dwProtectionItemObjID;			// ��ȣ ������
	DWORD	dwProbIncreaseItemObjID;		// Ȯ�� ���� ������
	bool	bSafetyUpgrade;					// ���� ���� ����


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// �Ӽ� ���� ��û
class	FLPacketItemUpgradeAttributeEnchantReq : public FLPacket
{
public:

	FLPacketItemUpgradeAttributeEnchantReq();

	DWORD	dwUpgradeNpcObjID;				// ���� ���� NPC
	DWORD	dwMainItemObjID;				// ������ ������
	DWORD	dwMaterialItemObjID;			// ��� ������
	DWORD	dwProtectionItemObjID;			// ��ȣ ������
	DWORD	dwProbIncreaseItemObjID;		// Ȯ�� ���� ������
	bool	bSafetyUpgrade;					// ���� ���� ����


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// �Ӽ� ���� ��û
class	FLPacketItemUpgradeAttributeChangeReq : public FLPacket
{
public:

	FLPacketItemUpgradeAttributeChangeReq();

	DWORD	dwUpgradeNpcObjID;				// ���� ���� NPC
	DWORD	dwMainItemObjID;				// ������ ������
	DWORD	dwMaterialItemObjID;			// ��� ������
	BYTE	byAttribute;					// ���� ������ �Ӽ�


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// �Ӽ� ���� ��û
class	FLPacketItemUpgradeAttributeRemoveReq : public FLPacket
{
public:

	FLPacketItemUpgradeAttributeRemoveReq();

	DWORD	dwUpgradeNpcObjID;				// ���� ���� NPC
	DWORD	dwMainItemObjID;				// ������ ������


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// �Ǿ�� ���� ���� ��û
class	FLPacketItemUpgradePiercingSizeIncreaseReq : public FLPacket
{
public:

	FLPacketItemUpgradePiercingSizeIncreaseReq();

	DWORD	dwUpgradeNpcObjID;				// ���� ���� NPC
	DWORD	dwMainItemObjID;				// ������ ������
	DWORD	dwMaterialItemObjID;			// ��� ������
	DWORD	dwProtectionItemObjID;			// ��ȣ ������
	DWORD	dwProbIncreaseItemObjID;		// Ȯ�� ���� ������
	bool	bSafetyUpgrade;					// ���� ���� ����


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// �Ǿ�� ���� ���� ��û
class	FLPacketItemUpgradePiercingItemInsertReq : public FLPacket
{
public:

	FLPacketItemUpgradePiercingItemInsertReq();

	DWORD	dwUpgradeNpcObjID;				// ���� ���� NPC
	DWORD	dwMainItemObjID;				// ������ ������
	DWORD	dwMaterialItemObjID;			// ��� ������
	DWORD	dwProtectionItemObjID;			// ��ȣ ������
	DWORD	dwProbIncreaseItemObjID;		// Ȯ�� ���� ������


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// �Ǿ�� ���� �ɼ� ���� ��û
class	FLPacketItemUpgradePiercingItemRemoveReq : public FLPacket
{
public:

	FLPacketItemUpgradePiercingItemRemoveReq();

	DWORD	dwUpgradeNpcObjID;				// ���� ���� NPC
	DWORD	dwMainItemObjID;				// ������ ������
	DWORD	dwMaterialItemObjID;			// ��� ������
	DWORD	dwProtectionItemObjID;			// ��ȣ ������
	DWORD	dwProbIncreaseItemObjID;		// Ȯ�� ���� ������


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// ���� �ɼ� ���� ��û
class	FLPacketItemUpgradeRandomOptionGenerateReq : public FLPacket
{
public:

	FLPacketItemUpgradeRandomOptionGenerateReq();

	DWORD	dwUpgradeNpcObjID;				// ���� ���� NPC
	DWORD	dwMainItemObjID;				// ������ ������
	DWORD	dwMaterialItemObjID;			// ��� ������


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// ���� �ɼ� �籼�� ��û
class	FLPacketItemUpgradeRandomOptionGenerateRetryReq : public FLPacket
{
public:

	FLPacketItemUpgradeRandomOptionGenerateRetryReq();

	DWORD	dwUpgradeNpcObjID;				// ���� ���� NPC
	DWORD	dwMainItemObjID;				// ������ ������
	DWORD	dwMaterialItemObjID;			// ��� ������
	BYTE	byIndex;						// ��õ� �� �ɼ� �ε���


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// ���� �ɼ� ���� ��û
class	FLPacketItemUpgradeRandomOptionInitializeReq : public FLPacket
{
public:

	FLPacketItemUpgradeRandomOptionInitializeReq();

	DWORD	dwUpgradeNpcObjID;				// ���� ���� NPC
	DWORD	dwMainItemObjID;				// ������ ������
	DWORD	dwMaterialItemObjID;			// ��� ������


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// ���� ���� �϶� ��û
class	FLPacketItemUpgradeEquipLevelDecreaseReq : public FLPacket
{
public:

	FLPacketItemUpgradeEquipLevelDecreaseReq();

	DWORD	dwMainItemObjID;				// ������ ������
	DWORD	dwMaterialItemObjID;			// ��� ������


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// ���� ���� �϶� ���� ��û
class	FLPacketItemUpgradeEquipLevelInitializeReq : public FLPacket
{
public:

	FLPacketItemUpgradeEquipLevelInitializeReq();

	DWORD	dwMainItemObjID;				// ������ ������


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// ������ �ɼ� �ռ� ��û
class	FLPacketItemUpgradeCombineReq : public FLPacket
{
public:

	FLPacketItemUpgradeCombineReq();

	DWORD	dwMainItemObjID;				// ������ ������
	DWORD	dwMaterialItemObjID;			// ��� ������
	DWORD	dwProtectionItemObjID;			// ��ȣ ������


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// ������ �ɼ� �ռ� ���� ���� ������ ��û
class	FLPacketItemUpgradeCombineInitializeDataReq : public FLPacket
{
public:

	FLPacketItemUpgradeCombineInitializeDataReq();

	DWORD	dwMainItemObjID;				// ������ ������


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// ������ �ɼ� �ռ� ���� ��û
class	FLPacketItemUpgradeCombineInitializeReq : public FLPacket
{
public:

	FLPacketItemUpgradeCombineInitializeReq();

	DWORD	dwMainItemObjID;				// ������ ������


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// ���� ���� ��û
class	FLPacketItemUpgradeLooksChangeReq : public FLPacket
{
public:

	FLPacketItemUpgradeLooksChangeReq();

	DWORD	dwMainItemObjID;				// ������ ������
	DWORD	dwMaterialItemObjID;			// ��� ������


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// ���� ���� ���� ��û
class	FLPacketItemUpgradeLooksInitializeReq : public FLPacket
{
public:

	FLPacketItemUpgradeLooksInitializeReq();

	DWORD	dwMainItemObjID;				// ������ ������
	DWORD	dwMaterialItemObjID;			// ��� ������


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

#ifdef BARUNA_ULTIMATE_UPDATE
// �ٷ糪 -> �ٷ糪 ���͹� ��ȯ
class	FLPacketItemUpgradeUltimateTransReq : public FLPacket
{
public:

	FLPacketItemUpgradeUltimateTransReq();

	DWORD	dwUpgradeNpcObjID;				// ���� ���� NPC
	DWORD	dwMainItemObjID;				// ������ ������
	DWORD	dwMaterialItemObjID1;			// ��� ������1
	DWORD	dwMaterialItemObjID2;			// ��� ������2


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};
#endif

#ifdef CARD_UPGRADE_SYSTEM
class	FLPacketItemUpgradeCardReq : public FLPacket
{
public:
	FLPacketItemUpgradeCardReq();
	DWORD	dwMainItemObjID;				// ������ ������
	DWORD	dwProtectionItemObjID;				// ��ȣ ������
private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};
#endif	// CARD_UPGRADE_SYSTEM

#ifdef COSTUME_UPGRADE_ENHANCEMENT_GEM
class	FLPacketItemUpgradeSetGemReq : public FLPacket
{
public:
	FLPacketItemUpgradeSetGemReq();
	DWORD	dwMainItemObjID;				// ������ ������
	DWORD	dwGemItemObjID;				// ���� ������
private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

class	FLPacketItemUpgradeRemoveGemReq : public FLPacket
{
public:
	FLPacketItemUpgradeRemoveGemReq();
	DWORD	dwMainItemObjID;				// ������ ������
	DWORD	dwMaterialItemObjID;				// ��� ������
private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};
#endif

#ifdef COSTUME_UPGRADE_MIX
class	FLPacketItemUpgradeMixReq : public FLPacket
{
public:
	FLPacketItemUpgradeMixReq();
	DWORD	dwMaterialItemObjID[5];				// ��� ������
private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};
#endif

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// world -> client

// �Ϲ� ���� ��� ����
class	FLSnapshotItemUpgradeGeneralEnchantResultAck : public FLPacket
{
public:

	FLSnapshotItemUpgradeGeneralEnchantResultAck();

	DWORD	dwResult;			// �����
	bool	bSafetyUpgrade;		// ���� ���� ����


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// �Ӽ� ���� ��� ����
class	FLSnapshotItemUpgradeAttributeEnchantResultAck : public FLPacket
{
public:

	FLSnapshotItemUpgradeAttributeEnchantResultAck();

	DWORD	dwResult;			// �����
	bool	bSafetyUpgrade;		// ���� ���� ����


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// �Ӽ� ���� ��� ����
class	FLSnapshotItemUpgradeAttributeChangeResultAck : public FLPacket
{
public:

	FLSnapshotItemUpgradeAttributeChangeResultAck();

	DWORD	dwResult;			// �����


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// �Ӽ� ���� ��� ����
class	FLSnapshotItemUpgradeAttributeRemoveResultAck : public FLPacket
{
public:

	FLSnapshotItemUpgradeAttributeRemoveResultAck();

	DWORD	dwResult;			// �����


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// �Ǿ�� ���� ���� ��� ����
class	FLSnapshotItemUpgradePiercingSizeIncreaseResultAck : public FLPacket
{
public:

	FLSnapshotItemUpgradePiercingSizeIncreaseResultAck();

	DWORD	dwResult;			// �����
	bool	bSafetyUpgrade;		// ���� ���� ����


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// �Ǿ�� ���� ���� ��� ����
class	FLSnapshotItemUpgradePiercingItemInsertResultAck : public FLPacket
{
public:

	FLSnapshotItemUpgradePiercingItemInsertResultAck();

	DWORD	dwResult;			// �����


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// �Ǿ�� ���� ���� ���� ����
class	FLSnapshotItemUpgradePiercingItemRemoveResultAck : public FLPacket
{
public:

	FLSnapshotItemUpgradePiercingItemRemoveResultAck();

	DWORD	dwResult;			// �����


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// ���� �ɼ� ���� ��� ����
class	FLSnapshotItemUpgradeRandomOptionGenerateResultAck : public FLPacket
{
public:

	FLSnapshotItemUpgradeRandomOptionGenerateResultAck();

	DWORD	dwResult;			// �����


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// ���� �ɼ� �籼�� ��� ����
class	FLSnapshotItemUpgradeRandomOptionGenerateRetryResultAck : public FLPacket
{
public:

	FLSnapshotItemUpgradeRandomOptionGenerateRetryResultAck();

	DWORD	dwMainItemObjID;	// ������ ������
	BYTE	byIndex;			// ��õ� �� �ɼ� �ε���
	DWORD	dwResult;			// �����
	
	BYTE	byDummyIndex;								// ���� �����Ϳ��� ��õ� �� �ɼ��� �ε���
	RandomOptionExtensionVec	vecDummyRandomOption;	// ��õ� �� �ɼ��� ���� �� ���� ������


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// ���� �ɼ� ���� ��� ����
class	FLSnapshotItemUpgradeRandomOptionInitializeResultAck : public FLPacket
{
public:

	FLSnapshotItemUpgradeRandomOptionInitializeResultAck();

	DWORD	dwResult;			// �����


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// ���� ���� �϶� ��� ����
class	FLSnapshotItemUpgradeEquipLevelDecreaseResultAck : public FLPacket
{
public:

	FLSnapshotItemUpgradeEquipLevelDecreaseResultAck();

	DWORD	dwResult;			// �����


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// ���� ���� �϶� ���� ��� ����
class	FLSnapshotItemUpgradeEquipLevelInitializeResultAck : public FLPacket
{
public:

	FLSnapshotItemUpgradeEquipLevelInitializeResultAck();

	DWORD	dwResult;			// �����


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// ������ �ɼ� �ռ� ��� ����
class	FLSnapshotItemUpgradeCombineResultAck : public FLPacket
{
public:

	FLSnapshotItemUpgradeCombineResultAck();

public:
	DWORD	dwResult;			// �����


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// ������ �ɼ� �ռ� ���� ������ ����
class	FLSnapshotItemUpgradeCombineInitializeDataAck : public FLPacket
{
public:

	FLSnapshotItemUpgradeCombineInitializeDataAck();

	bool	bCanInitialize;			// �ʱ�ȭ ���� ����
	int		nInitializePenya;		// �ʱ�ȭ �� �Ҹ�Ǵ� ���


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// ������ �ɼ� �ռ� ���� ��� ����
class	FLSnapshotItemUpgradeCombineInitializeResultAck : public FLPacket
{
public:

	FLSnapshotItemUpgradeCombineInitializeResultAck();

	DWORD	dwResult;			// �����


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// ���� ���� ��� ����
class	FLSnapshotItemUpgradeLooksChangeResultAck : public FLPacket
{
public:

	FLSnapshotItemUpgradeLooksChangeResultAck();

	DWORD	dwResult;			// �����


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// ���� ���� ���� ��� ����
class	FLSnapshotItemUpgradeLooksInitializeResultAck : public FLPacket
{
public:

	FLSnapshotItemUpgradeLooksInitializeResultAck();

	DWORD	dwResult;			// �����


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

#ifdef BARUNA_ULTIMATE_UPDATE
// �ٷ糪 -> �ٷ糪 ���͹� ��ȯ
class	FLSnapshotItemUpgradeUltimateTransResultAck : public FLPacket
{
public:

	FLSnapshotItemUpgradeUltimateTransResultAck();

	DWORD	dwResult;			// �����


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};
#endif

#ifdef CARD_UPGRADE_SYSTEM
class	FLSnapshotItemUpgradCardResultAck : public FLPacket
{
public:

	FLSnapshotItemUpgradCardResultAck();

	DWORD	dwResult;			// �����


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};
#endif	// CARD_UPGRADE_SYSTEM

#ifdef COSTUME_UPGRADE_ENHANCEMENT_GEM
// ���� �ռ� ����
class	FLSnapshotItemUpgradeSetGemResultAck : public FLPacket
{
public:

	FLSnapshotItemUpgradeSetGemResultAck();

	DWORD	dwResult;			// �����


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// ���� ���� ����
class	FLSnapshotItemUpgradeRemoveGemResultAck : public FLPacket
{
public:

	FLSnapshotItemUpgradeRemoveGemResultAck();

	DWORD	dwResult;			// �����


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};
#endif

#ifdef COSTUME_UPGRADE_MIX
// ������ �ռ� ����
class	FLSnapshotItemUpgradeMixResultAck : public FLPacket
{
public:

	FLSnapshotItemUpgradeMixResultAck();

	DWORD	dwResult;			// �����

	DWORD	dwResultItemObjID;	// ������ ������
private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};
#endif



//////////////////////////////////////////////////////////////////////////

#endif // __FLPACKETITEMUPGRADE_H__
