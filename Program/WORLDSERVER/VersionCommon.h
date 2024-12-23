#ifndef __VERSION_COMMON_H__
#define __VERSION_COMMON_H__

#define	__VER	23

#if !defined( _DEBUG )
	#define	__MAINSERVER
#endif
#if !defined( __MAINSERVER )
	#define __INTERNALSERVER
#endif


// OFFI
#define		D3DDEVICE	NULL

#define		__SERVER					// Ŭ���̾�Ʈ �����ڵ带 �������� �ʱ� ���� define
#define		__X15

#define		__S1108_BACK_END_SYSTEM		// ��ص�ý��� Trans, World, Neuz
#define		__RES0807					// ������ ����
#define		__CPU_UTILDOWN_060502		// CPU ���� �����۾� 
#define		__SLIDE_060502				// ������ ������ ���� ���� 


#define		__VENDOR_1106				// ���λ��� ��� ����

#define		__EVENTLUA_GIFT				// Ư�� ������ �������� ������ ���� TODO worldserver�� ����
#define		__EVENTLUA_CHEEREXP			// ��� �̺�Ʈ - ���� ����ġ ���� TODO worldserver�� ����
#define		__EVENTLUA_SPAWN			// ������ �� ���� ���� �̺�Ʈ
#define		__EVENTLUA_KEEPCONNECT		// ���� ���� ������ ���� �̺�Ʈ


#define		__EVENT_MONSTER				// �̺�Ʈ ����(WorldServer) TODO worldserver�� ����


#define		__LAYER_1015				// ���� ��ü ��
#define		__LAYER_1021				// __LAYER_1015 ������


#define		__PERIN_BUY_BUG				// ��� �ݺ����� ���� Ȯ�ο� �ڵ�
#define		__GLOBAL_COUNT_0705			// Ÿ�̸Ӱ� timeGetTime()�� ���� ȣ������ �ʵ��� ����
#define		__EVENT_0117				// ���� �̺�Ʈ TODO worldserver�� ����



// 16��
#if __VER >= 16
	#define		__GUILD_HOUSE_MIDDLE		// ����Ͽ콺 ����
	#define		__MOVER_STATE_EFFECT		// ���� ���°� ��ȭ�� ���� ����Ʈ ����
	#define		__ENCHANT_BARUNA16			// ������ ( �ٷ糪 )
	#define		__HYPERLINK_ITEM16			// ������ ��ũ..
	#define		__AGGRO16					// ��׷� �߰�
	#define		__ADDWEAPON_CROSSBOW16		// 16�� �߰����� ũ�ν�����
#endif

// 20��
#if __VER >= 20

#define METEONYKER_SUBSUMMON		// meteonyker������ �������� ��ȯ����
#define	PARTY_WARP			// PartyDungeon.lua�� SetPartyWarp( 1 )�� ���� �� ��Ƽ������ ��������
#define SCRIPT_ENTER_INSTANCEDUNGEON	// ��ũ��Ʈ���� ���� ����
#define	DAILY_QUEST			// ���� ����Ʈ ����
#define	INVENTORY_GENERAL54		// �κ��丮 42->54
#define	INVENTORY_ITEM_ALIGN		// �κ��丮 ������ ����
#define CARD_UPGRADE_SYSTEM			// ī�� ���׷��̵� �ý��� 
#define KEYBOARD_SET
//#define PAT_LOOTOPTION			// �� �ɼ��߰�
//#define	NEW_GUILD_WINLOG		// ���ο� ������ �¸���
//#define	SKILL_BUFF21			// ��ų�������� 14->21
//#define BARUNA_ULTIMATE_UPDATE		// �ٷ糪 ���͸� �߰�
#define		PASSWORD_RESET_2ND
#endif

#if __VER >= 21
#define PAT_LOOTOPTION			// �� �ɼ��߰�
#define	SKILL_BUFF21
#define COSTUME_UPGRADE_ENHANCEMENT_GEM
#define COSTUME_UPGRADE_MIX
#define	ENCHANT_ABSOLUTE
#define ENCHANT_ABSOLUTE_MIX
#define BATTERY_PREMIUM
// 21.2 Version
#define GUILD_WINNER_BUFF		// 21�� ����� ���� �׽�Ʈ
#define BARUNA_UPGRADE_ENHANCEMENT_GEM
#define BARUNA_UPGRADE_SUIT_PIERCING
#define BARUNA_UPGRADE_SMELT_SAFETY
#define INVENTORY_PET_COSTUME42

#endif

#if	  defined(__INTERNALSERVER)	// ���� �繫�� �׽�Ʈ����

	#define		__GUILDVOTE					// ��� ��ǥ

	#define		__IAOBJ0622					// ��� ��ǥ ��ǥ ����	// ����, ����
	#define		__SKILL0517

	#define		__Y_HAIR_BUG_FIX
	

	#define		__PERF_0226

#elif defined(__MAINSERVER)	// �ܺ� ����
 
	// �ؿ� ���� ���� ����
	#define		__ON_ERROR
	#define		__IDC

#endif	// end - ���������� define 

// �ӽ� - ����� ���� ���� 
#define		__REMOVE_SCIRPT_060712		 

#endif