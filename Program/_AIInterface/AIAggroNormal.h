#pragma once 

#if defined __WORLDSERVER && defined __AGGRO16

//20100802 : �δ� �ܿ��� ���� ���� ������ ���ٰ� ��.
// ���� �δ��� �������. �� ������ ������ ó���� �������.

class CAIAggroNormal : public CAIAggroInterface
{
public:
	CAIAggroNormal( CObj* pObj, const DWORD dwAggroID );
	virtual ~CAIAggroNormal();

	virtual void			InitAI();
	virtual	BOOL			IsReturnToBegin( void )		{	return m_bReturnToBegin;	}
	virtual BOOL			IsIdleMode();

protected:
	DWORD					GetAtkMethod_Near();
	DWORD					GetAtkMethod_Far();
	DWORD					GetAtkRange( DWORD dwAtkMethod );
	PT_ITEM_SPEC			GetAtkProp( DWORD dwAtkMethod );
	DWORD					GetAtkItemId( DWORD dwAtkMethod );
	void					DoAttack( DWORD dwAtkMethod, CMover* pTarget );
	void					DoReturnToBegin( BOOL bReturn = TRUE );
	void					MoveToDst( OBJID dwIdTarget );
	void					MoveToDst( const D3DXVECTOR3& vDst );
	void					MoveToRandom( UINT nState );
	void					CallHelper( const MoverProp* pMoverProp );
	void					SetStop( DWORD dwTime );
	BOOL					IsEndStop();
	BOOL					IsMove() { return GetMover()->m_pActMover->IsMove(); }
	BOOL					IsInRange( const D3DXVECTOR3& vDistant, FLOAT fRange );
	int						SelectAttackType( CMover *pTarget );	// AI�� ���� ���ݹ���� ����.
	BOOL					MoveProcessIdle( const AIMSG & msg );
	BOOL					MoveProcessRage( const AIMSG & msg );
	BOOL					SubAttackChance( const AIMSG & msg, CMover *pTarget );
	BOOL					MoveProcessRunaway();
	BOOL					StopProcessIdle();
	void					SubSummonProcess( CMover *pTarget );
	BOOL					SubItemLoot();
	void					Init();
	BOOL					StateInit( const AIMSG & msg );
	BOOL					StateIdle( const AIMSG & msg );
	BOOL					StateWander( const AIMSG & msg );
	BOOL					StateRunaway( const AIMSG & msg );
	BOOL					StateEvade( const AIMSG & msg );
	BOOL					StateRage( const AIMSG & msg );

	BOOL					StateRagePatrol( const AIMSG & msg );
	BOOL					MoveProcessRagePatrol( const AIMSG & msg );	
	BOOL					StateStand( const AIMSG & msg );
	BOOL					StatePatrol( const AIMSG & msg );
	BOOL					MoveProcessStand( const AIMSG & msg );
	BOOL					MoveProcessPatrol( const AIMSG & msg );

	void					SetTarget( OBJID dwIdTarget, u_long uParty );


	CMover *				ScanTargetAndSetTarget( int nRangeMeter = 5, int nJobCond = 0, DWORD dwQuest = 0, DWORD dwItem = 0, int nChao = 0 );

public:
	MEMPOOLER_DECLARE( CAIAggroNormal );

	DECLARE_AISTATE( CAIAggroNormal )


protected:
	DWORD 					m_tmMove;
	DWORD					m_tmAttackDelay;
	DWORD					m_tmAttack;		
	DWORD					m_tmSummon;					// ��ȯ Ÿ�̸�.
	DWORD					m_tmHelp;					// ���� ��û Ÿ�̸�.
	DWORD					m_tmReturnToBegin;			// ������������ ���ư��� �ð�����.

	D3DXVECTOR3				m_vPosBegin;
	D3DXVECTOR3				m_vDestPos;
	D3DXVECTOR3				m_vOldPos;					// ���� ������ ��ǥ

	D3DXVECTOR3				m_vPosDamage;

	_VECINFO				m_vecPatrolInfo;
	FLOAT					m_fAngleBegine;

	BOOL					m_bReturnToBegin;
	BOOL					m_bGoTarget;
	BOOL					m_bTargetNoMovePos;
	BOOL					m_bFirstRunaway;
	BOOL					m_bCallHelper;
	BOOL					m_bRangeAttack;				// ������ �����̳� �ƴϳ�.
	BOOL					m_bLootMove;				// �����Ϸ� ������.

	DWORD					m_dwIdTarget;


	DWORD					m_dwAtkMethod;
	vector<OBJID>			m_vecIdSummon;					// ��ȯ�� ���ʹ� ���̵� ä������.
	DWORD					m_idLootItem;				// ������ ������.
};

#endif 
