#ifndef __AI_MONSTER_H
#define __AI_MONSTER_H

#include <memory/FLMemPooler.h>
#include <Mover.h>
#include <PathObj.h>

class CAIMonster : public CAIInterface
{
protected:
	DWORD 			m_tmMove;
	DWORD			m_tmAttackDelay;
	DWORD			m_tmAttack;		
	DWORD			m_tmSummon;					// ��ȯ Ÿ�̸�.
	DWORD			m_tmHelp;					// ���� ��û Ÿ�̸�.
	DWORD			m_tmReturnToBegin;			// ������������ ���ư��� �ð�����.

	D3DXVECTOR3		m_vPosBegin;
	D3DXVECTOR3		m_vDestPos;
	D3DXVECTOR3		m_vOldPos;					// ���� ������ ��ǥ

	D3DXVECTOR3		m_vPosDamage;
	
	_VECINFO		m_vecPatrolInfo;
	FLOAT			m_fAngleBegine;

	BOOL			m_bReturnToBegin;
	BOOL			m_bGoTarget;
	BOOL			m_bTargetNoMovePos;
	BOOL			m_bFirstRunaway;
	BOOL			m_bCallHelper;
	BOOL			m_bRangeAttack;				// ������ �����̳� �ƴϳ�.
	BOOL			m_bLootMove;				// �����Ϸ� ������.

	DWORD			m_dwIdTarget;
	DWORD			m_dwAtkMethod;

	vector<OBJID>	m_vecIdSummon;					// ��ȯ�� ���ʹ� ���̵� ä������.

	DWORD			m_idLootItem;				// ������ ������.

	DWORD			GetAtkMethod_Near();
	DWORD			GetAtkMethod_Far();
	DWORD			GetAtkRange( DWORD dwAtkMethod );
	PT_ITEM_SPEC		GetAtkProp( DWORD dwAtkMethod );
	DWORD			GetAtkItemId( DWORD dwAtkMethod );
	void			DoAttack( DWORD dwAtkMethod, CMover* pTarget );
	void			DoReturnToBegin( BOOL bReturn = TRUE );
	void			MoveToDst( OBJID dwIdTarget );
	void			MoveToDst( const D3DXVECTOR3& vDst );
	void			MoveToRandom( UINT nState );
	void			CallHelper( const MoverProp* pMoverProp );
	void			SetStop( DWORD dwTime );
	BOOL			IsEndStop();
	BOOL			IsMove() { return GetMover()->m_pActMover->IsMove(); }
	BOOL			IsInRange( const D3DXVECTOR3& vDistant, FLOAT fRange );
	int				SelectAttackType( CMover *pTarget );	// AI�� ���� ���ݹ���� ����.
	BOOL			MoveProcessIdle( const AIMSG & msg );
	BOOL			MoveProcessRage( const AIMSG & msg );
	BOOL			SubAttackChance( const AIMSG & msg, CMover *pTarget );
	BOOL			MoveProcessRunaway();
	BOOL			StopProcessIdle();
	void			SubSummonProcess( CMover *pTarget );
	BOOL			SubItemLoot();
	void			Init();
	BOOL			StateInit( const AIMSG & msg );
	BOOL			StateIdle( const AIMSG & msg );
	BOOL			StateWander( const AIMSG & msg );
	BOOL			StateRunaway( const AIMSG & msg );
	BOOL			StateEvade( const AIMSG & msg );
	BOOL			StateRage( const AIMSG & msg );

	BOOL			StateRagePatrol( const AIMSG & msg );
	BOOL			MoveProcessRagePatrol( const AIMSG & msg );	
	BOOL			StateStand( const AIMSG & msg );
	BOOL			StatePatrol( const AIMSG & msg );
	BOOL			MoveProcessStand( const AIMSG & msg );
	BOOL			MoveProcessPatrol( const AIMSG & msg );

	void	SetTarget( OBJID dwIdTarget, u_long uParty );

public:
	virtual	BOOL	IsReturnToBegin( void )		{	return m_bReturnToBegin;	}
protected:
	
public:
	CAIMonster();
	CAIMonster( CObj* pObj );
	virtual ~CAIMonster();
	
	virtual void	InitAI();
	virtual BOOL	IsIdleMode();

	MEMPOOLER_DECLARE( CAIMonster );
	
	DECLARE_AISTATE( CAIMonster )
};


struct __MONSTERSKILL
{
	DWORD	dwAtkMethod;
	DWORD	dwSkillID;
	DWORD	dwSkillLV;
	int		nHitCount;
	int		nRange;
	int		nApplyProbabilty;
	DWORD	dwSkillTime;
	bool	bIgnoreSkillProb;
	__MONSTERSKILL()
	{
       dwAtkMethod	= 0;
		dwSkillID	= NULL_ID;
		dwSkillLV	= 0;
		nHitCount	= 0;
		nRange		= 0;
		nApplyProbabilty = 0;
		dwSkillTime = 0;
		bIgnoreSkillProb = 0;
	}
};

typedef map<DWORD, vector<__MONSTERSKILL> > MAPMONSTERSKILL;
typedef map<DWORD, DWORD> MAPATKMETHOD;

const DWORD ATK_MELEE = 1;
const DWORD ATK_RANGE = 2;

class CMonsterSkill
{
private:
	CMonsterSkill();
public:
	~CMonsterSkill();
	static CMonsterSkill* GetInstance( void );

	void	LoadScript();
	void	Clear();

	DWORD	GetMonsterSkillLevel( CMover* pAttacker, DWORD dwSkillId );
	BOOL	ApplySkill( CMover* pAttacker, CMover* pTarget, DWORD dwAtkMethod );
	BOOL	MonsterTransform( CMover* pMover, int nMoverHP );

private:
	MAPMONSTERSKILL		m_mapMonsterSkill;
};

#endif
