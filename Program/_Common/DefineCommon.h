#ifndef __COMMON_DEFINE__
#define __COMMON_DEFINE__

#include <windows.h>

//////////////////////////////////////////////////////////////////////////////
// Ŭ���̾�Ʈ �� �������� �������� ����ϴ� ��(constant), ���� �����ϴ� ����
//////////////////////////////////////////////////////////////////////////////
const int MAX_SHORTCUT_STRING = 128;
const int MAX_GOLD_COIN = 32767;
const int MAX_GOLD_REWARD = INT_MAX;
const int MAX_INPUT_REWARD = 200000000;
const int MIN_INPUT_REWARD = 1000;
const int REQ_WANTED_GOLD = 1000;			// ����� ��ġ���� ���� �� 

const unsigned char	REMOVE_VOTE	= 0;		// ��ǥ���� 
const unsigned char	CLOSE_VOTE	= 1;		// ��ǥ���� 
const unsigned char	CAST_VOTE	= 2;		// ��ǥ�ϱ� 
const int			TIMEWAIT_CLOSE = 10;	// �� ��ȭ���� ������ �ð�  
const	int		TIMEWAIT_RELEASE	= 3600;
const	int		TIMEWAIT_INSTANT_BANKER		= 180000;


const int MAX_CHARGE_LEVEL = 4;

extern const int OLD_MPU;
extern int g_MPU;

enum ATK_TYPE 
{
	ATK_FORCE,					// �̸����� �������� ������ �� ��� 
	ATK_MELEESKILL,				// �и���ų 
	ATK_MAGICSKILL,				// ���� ��ų 
	ATK_MAGIC,					// �ϵ� ���� 
	ATK_GENERIC,				// �Ϲ� ���� 
};

class CMover;

struct ATTACK_INFO 
{
	CMover*		pAttacker;			// ������ 
	CMover*		pDefender;			// ����� 
	DWORD		dwAtkFlags;			// ���� �÷���
	int			nParam;				// ���� �߰� �÷��� 
	int			nParts;				// �������ΰ� �޼��ΰ� 
	int			nDEFFactor;			// ���� ���� 

	ATK_TYPE	GetAtkType() const;
	BOOL		CanIgnoreDEF() const;
	int			GetChargeLevel() const;
	int			GetAttackCount() const;
	int			GetSkill() const;
	BOOL		IsRangeAttack() const;
	int			GetSkillLevel() const; 
};

#ifdef __CLIENT
	inline int ATTACK_INFO::GetSkill() const
	{
		return 0;
	}
#endif

//gmpbigsun( 20120221 ) : ���⺰ �⺻���ݵ��� maxframe 
// frame ������ġ�� 0.5�̹Ƿ� maxframe * 2 * ( 1 / attackspeed ) �� ���� ���ݰ� �ð��̴�.
// �ּ��� ��� �ּ�Ÿ���� ���⶧���� ��տ� ���⸦ ���� TYPE2_USERTYPE1���� ã�ƾ��Ѵ�.
// usage : int nMaxFrame = _GetAniFrameNAT( ����Ÿ�� );
#include <defineItemType.h>
struct ANI_FRAME_NORMALATTACK
{
	std::map< int, int > _mapDatas;
	std::map< int, int >::iterator _mapIter;
	typedef std::map< int, int >::value_type _mapValueType;

	enum
	{
		TYPE2_USERTYPE1 = 100,	//�ּ�
	};

	int GetMaxFrame( int nWeaponType ) 
	{
		_mapIter = _mapDatas.find( nWeaponType );

		if( _mapIter != _mapDatas.end() )
		{
			return _mapDatas[ nWeaponType ];
		}
		else
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "GetMaxFrame nWeaponType is Error nWeaponType [ %d ]" ), nWeaponType );
			FLASSERT( 0 );

			return	( _mapDatas[ TYPE2_WEAPON_ONESWORD ] );
		}

//		return	( _mapDatas[ TYPE2_WEAPON_ONESWORD ] );
	}

	void Init( )
	{
		_mapDatas.clear();
		_mapDatas.insert( _mapValueType( TYPE2_WEAPON_ONESWORD, 17 ) );
		_mapDatas.insert( _mapValueType( TYPE2_WEAPON_TWOSWORD, 21 ) );
		_mapDatas.insert( _mapValueType( TYPE2_WEAPON_ONEAXE,	21 ) );
		_mapDatas.insert( _mapValueType( TYPE2_WEAPON_TWOAXE,	21 ) );
		_mapDatas.insert( _mapValueType( TYPE2_WEAPON_KNUCKLE,	16 ) );
		_mapDatas.insert( _mapValueType( TYPE2_WEAPON_STICK,	18 ) );
		_mapDatas.insert( _mapValueType( TYPE2_WEAPON_BOW,		29 ) );
		_mapDatas.insert( _mapValueType( TYPE2_WEAPON_STAFF,	17 ) );
		_mapDatas.insert( _mapValueType( TYPE2_WEAPON_YOYO,		24 ) );
		_mapDatas.insert( _mapValueType( TYPE2_WEAPON_WAND,		36 ) );
//		_mapDatas.insert( _mapValueType( TYPE2_USERTYPE1,		21 ) );
	}

	void UnInit( )
	{
		_mapDatas.clear( );
	}

	static ANI_FRAME_NORMALATTACK* GetThis( )
	{
		static ANI_FRAME_NORMALATTACK stThis;
		return &stThis;
	}

	ANI_FRAME_NORMALATTACK() { Init(); }
	~ANI_FRAME_NORMALATTACK( ) { UnInit(); }
};

_inline int _GetAniFrameNAT( int nType ) { return ANI_FRAME_NORMALATTACK::GetThis()->GetMaxFrame(nType); }		//NAT : Normal Attack





#endif	// __COMMON_DEFINE__
