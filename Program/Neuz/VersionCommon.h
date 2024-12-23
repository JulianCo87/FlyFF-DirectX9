#ifndef __VERSION_COMMON_H__
#define __VERSION_COMMON_H__
#if !defined( _DEBUG )
#define	__MAINSERVER
//#define __EXTERNALTEST
#endif
#define	__VER	23

//debug�� �ܺ��׼��� �����ϰ� ������---------------------------------//
//#define __MAINSERVER
//#define NO_GAMEGUARD
//-------------------------------------------------------------------//



// offi

#if !defined( __MAINSERVER )
	#define __INTERNALSERVER
#endif

#define		__S1108_BACK_END_SYSTEM		// Trans, World, Neuz : ��ص�ý��� 
#define		__VENDOR_1106				// ���λ��� ��� ����

#define		__SFX_OPT					// ����Ʈ(��ƼŬ) ����ȭ ���� TODO Ŭ���̾�Ʈ, PatchClient��
#define		__CPU_UTILDOWN_060502		// CPU ���� �����۾� TODO ��� �� ���θ�?, //gmpbigsun(20110104) ���� �ܺε� ���̽����


// 16��
#if __VER >= 16
	#define		__IMPROVE_MAP_SYSTEM			// ���� ���� �ý���
	#define		__GUILD_HOUSE_MIDDLE			// ����Ͽ콺 ����
	#define		__BS_ADDOBJATTR_INVISIBLE		// CObj Invisible �� ���� �Ӽ��߰�   --> 16�� ���� 
	#define		__BS_EFFECT_LUA					// ������Ʈ ���º� ȿ������ ( Lua base ) : CLIENT ONLY!!!!! EVER!!! ABSOLUTE!!!
	#define		__BS_ADD_CONTINENT_WEATHER		// ��� ���� �߰� ( �³�ȭ�� ���� �¾糯��, �ٸ� ������ε� ������ ���� )
	#define		__BS_CHANGEABLE_WORLD_SEACLOUD	// ���氡���� ���� �ٴٱ��� 
	#define		__ADDWEAPON_CROSSBOW16			// 16�� �߰����� ũ�ν�����
	#define		__SKILL_UI16					// 16�� SKILL UI
	#define		__HYPERLINK_ITEM16				// ������ ��ũ..
	#define		__ENCHANT_BARUNA16				// ������ ( �ٷ糪 )
	#define		__AGGRO16			//��׷�
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
#define ADD_CHARACTER_INFO_DISPLAY
#define COSTUME_UPGRADE_ENHANCEMENT_GEM
#define COSTUME_UPGRADE_MIX
#define ADD_INVENTORY_EDGE
#define ENCHANT_ABSOLUTE_MIX
#define BATTERY_PREMIUM
// 21.2 Version
#define GUILD_WINNER_BUFF
#define BARUNA_UPGRADE_ENHANCEMENT_GEM
#define BARUNA_UPGRADE_SUIT_PIERCING
#define BARUNA_UPGRADE_SMELT_SAFETY
#define INVENTORY_PET_COSTUME42
#endif

#if	  defined(__INTERNALSERVER)	// ���� �繫�� �׽�Ʈ����

// BEGIN =======================================================================================
// note: ���� ��ó����� �ּ�ó���� �Ǿ��ִ��� �ջ��� �����ų� �ڵ忡�� ���ŵǼ� �ȵ˴ϴ�. ( Only client )
//	#define		__YENV
//	#define		__YENV_WITHOUT_BUMP
//	#define		__Y_INTERFACE_VER3			// �������̽� ���� 3.0 - Neuz
//	#define		__CSC_UPDATE_WORLD3D		// World3D Object Culling�κ� ������Ʈ
//	#define		__FLYFF_INITPAGE_EXT
//	#define		__BS_DEATH_ACTION				// die ���·� ���Խ� ���� ȿ�� 
//	#define		__BS_CONSOLE
// END =========================================================================================

#ifndef	NO_GAMEGUARD
	#define		NO_GAMEGUARD
#endif

//	#define		__CPU_UTILDOWN_060502		// CPU ���� �����۾� TODO ��� �� ���θ�?
	#define		__SLIDE_060502				// ������ ������ ���� ���� TODO ��� �� ���θ�?

	#define		__GUILDVOTE					// ��� ��ǥ 

	#define		__IAOBJ0622					// ��� ��ǥ ��ǥ ����	// ����, ����
	#define		__YNOTICE_UNI1026			// �������� �����ڵ� ����
	#define		__SKILL0517					// ��ų ���� �Ķ����

	#define		__TRAFIC_1218				// �������� ���� ��Ŷ ����
	#define		__Y_HAIR_BUG_FIX			// TODO �̰� �׽�Ʈ ���ΰ�?


	#define		__GLOBAL_COUNT_0705			// CTime::GetTimer TODO �� ��������?

	#define		__ATTACH_MODEL				// �𵨿� �ٸ� �� ���̱� (����...)

	#define		__BS_ADJUST_COLLISION		// �浹 ��ƾ ���� ( 2009. 07. 28 )

//	#define		__USE_SOUND_LIB_FMOD		// change the FMOD

//	#define		__BS_TEST_MTE

#elif defined(__MAINSERVER)  // �ܺ� ����

#endif	// end - ���������� define 

#ifndef NO_GAMEGUARD 
	#define	__NPROTECT_VER	4	
#endif	

#endif // VERSION_COMMON_H

//================================================================================================================================================================
//Note BEGIN
// ������ ��ȣ�� �ش��ڵ尡 �ּ��� �ǹ̶�°��� �˷��ݴϴ�. ( �ڵ��� ���Ἲ�� ���� #define�� ������� �ʽ��ϴ� )
// �۾� ���Ǽ� �� �ٸ� ����ڸ� ���� ������ ������� ���˴ϴ�.
//	_SUN_CHECKDATA_	,				gmpbigsun( 20100705 ) : ������ ���� �� ���� ����  
//  _SUN_LOCALIZE_WNDSTATUS ,		gmpbigsun( 20100727 ) : Tile�� �����ʴ� �����쿡 ���ؼ� ��¥�̹��� localizing
//  _SUN_DEFAULT_WINDOW,			gmpbigsun( 20100830 ) : ���� ����� â����
//  _SUN_SKILLSFX_TO_LUA,			gmpbigsun( 20100928 ) : // skill sfx -> lua ( ��ƾ����, XI_SKILL_ ���� ), PropSkill->sfx�ʵ�� ����
//	_SUN_PICKING_WITH_WND,			gmpbigsun( 20101006 ) : ������Ʈ ��ŷ �õ��� ������˻�
//  _SUN_RESPWANINFO_VER8,			gmpbigsun( 20101012 ) : ������ ���� �߰�( AI )
//	_SUN_JAPAN_HANGAME_UI,			gmpbigsun( 20101021 ) : �Ϻ� �Ѱ��� ���� UI��ɺ���
//	_JIN_SHOP_CART,					flyingjin( 20101012 ) : 17�� ��ٱ��� ����
//	_JIN_ITEM_CANCELLATION,			flyingjin( 20101109 ) : 17�� �ͼ����� ��ũ��
//	_JIN_NEW_INVENTORY,				flyingjin( 201011010 ): 17�� �κ��丮 Ȯ��
//  _JIN_NEW_BARUNA_PEARCING,		flyingjin( 20101115 ) : 17�� �ٷ糪 �Ǿ��
//  _JIN_ITEM_CHARGE,				flyingjin( 20101221 ) : 17�� ����������ȭ
//  _SUN_ENCHANT_EFFECT17,			gmpbigsun( 20110116 ) : 17�� �� ��������Ʈ
//  _JIN_COSTUME_MIX,				flyingjin( 20110119 ) : 17�� �ڽ�Ƭ�ռ�
//	_JIN_TELEPORT,					flyingjin( 20110221 ) : 17�� �ڷ���Ʈ ����
//  _SUN_RIDER_ANIMATION			gmpbigsun( 20110328 ) : ���̴� ���ϸ��̼� �߰�
//  _SUN_RENDER_QUESTEMOTICON		gmpbigsun( 20110414 ) : ����Ʈ �̸�Ƽ�� ����( ĳ����, �̴ϸ�, ��ü�� )
//  _SUN_SEMI_INDOOR				gmpbigsun( 20110414 ) : outdoor �� ������ �Ϻ� ( light������Ʈ���� )�� �ε���ó�� ����
//	_JIN_WORLD_FREETICKET			flyingjin( 20010628 ) : 18�� �������� �����̿��
//	_JIN_TREASURE_CHEST				flyingjin( 20010712 ) : 18�� �������� ������
//  _SUN_UPGRADE_WNDMANAGER			gmpbigsun( 20110725 ) : CWndManager ����
//  _JIN_CHAR_SERVER_MOVEMENT       flyingjin( 20110822 ) : ĳ���� ���� ����
//	_SUN_CIRCLE_MINIMAP				gmpbigsun( 20111020 ) : ���� �̴ϸ�

//Note END
//================================================================================================================================================================

