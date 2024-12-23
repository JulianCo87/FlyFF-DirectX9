#ifndef __VERSION_COMMON__
#define __VERSION_COMMON__
#define	__VER	23

#if !defined( _DEBUG )
	#define	__MAINSERVER
#endif

#if !defined( __MAINSERVER )
	#define __INTERNALSERVER
#endif

#define		__SERVER					// Ŭ���̾�Ʈ �����ڵ带 �������� �ʱ� ���� define

#define		__S1108_BACK_END_SYSTEM		// ��ص�ý��� Trans, World, Neuz TODO CoreServer �̻��
#define		__SKILL_0205				// ��ų ���� �κи� ���� TODO DatabaseServer�� ���
#define		__VENDOR_1106				// ���λ��� ���� ��� ���� TODO CoreServer �̻��
#define		__LAYER_1015				// ���̾� ����
#define		__PERIN_BUY_BUG				// ��� �ݺ����� ���� Ȯ�ο� �ڵ� TODO Ŭ���̾�Ʈ �̻��

#define		DAILY_QUEST

#define		PASSWORD_RESET_2ND

// 16��
#if __VER >= 16
	#define		__GUILD_HOUSE_MIDDLE		// ����Ͽ콺 ����
	#define		__ENCHANT_BARUNA16			// ������ ( �ٷ糪 )
#endif

// 20��
#if __VER >= 20
#define	INVENTORY_GENERAL54		// �κ��丮 42->54
#endif


#if __VER >= 21
#define COSTUME_UPGRADE_ENHANCEMENT_GEM
#define BARUNA_UPGRADE_ENHANCEMENT_GEM
#define INVENTORY_PET_COSTUME42
#endif


#if	  defined(__INTERNALSERVER)			// ���� �繫�� �׽�Ʈ����

	#define		__GUILDVOTE				// ��� ��ǥ 

#elif defined(__MAINSERVER)				// �ܺ� ���� 

	#define		__IDC

#endif	// end - ���������� define 

#endif