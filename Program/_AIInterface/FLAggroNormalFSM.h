#pragma once

#include "FLFSM.h"
//#include "FLMoverAction.h"
#include "FLBehaviorFunction.h"

#include "FLAggro.h"



#if defined __WORLDSERVER && defined __AGGRO16
class FLAggroNormalFSM : public FLFSM< FLAggroNormalFSM >
{
private:
	enum						{			RAGE_TIME_OVER_MAX	= SEC( 15 ),		};


private:
	class FLIdleState : public FLAggroNormalFSM::FLState			//��Ȱ�� �Ҷ�� templateȭ �ؼ� ������ ���ô����ϼ�.
	{
	public:
		virtual	void			OnEnter( FLAggroNormalFSM & FSM );
		virtual void			UpdateAI( FLAggroNormalFSM & FSM, const AIMSG & msg );
		virtual	void			OnLeave( FLAggroNormalFSM & FSM );
	};

	class FLRageState : public FLAggroNormalFSM::FLState
	{
	public:
		FLRageState();
		virtual ~FLRageState();

		void					Clear();

		virtual	void			OnEnter( FLAggroNormalFSM & FSM );
		virtual void			UpdateAI( FLAggroNormalFSM & FSM, const AIMSG & msg );
		virtual	void			OnLeave( FLAggroNormalFSM & FSM );

	private:
		DWORD					m_tmAttackPoint;
	};

	class FLTokenToBeginState : public FLAggroNormalFSM::FLState
	{
		virtual	void			OnEnter( FLAggroNormalFSM & FSM );
		virtual void			UpdateAI( FLAggroNormalFSM & FSM, const AIMSG & msg );
		virtual	void			OnLeave( FLAggroNormalFSM & FSM );
	};

	class FLRunawayState : public FLAggroNormalFSM::FLState
	{
		virtual	void			OnEnter( FLAggroNormalFSM & FSM );
		virtual void			UpdateAI( FLAggroNormalFSM & FSM, const AIMSG & msg );
		virtual	void			OnLeave( FLAggroNormalFSM & FSM );
	};

public:
	FLAggroNormalFSM( CMover & Owner, const DWORD dwAggroID );//, AggroPtr pAggro );
	virtual ~FLAggroNormalFSM();

	void					SetPartyLeader( const DWORD dwLeaderObjid );

	virtual void			InitProp();

	void					ClearAll();
	void					ClearWithoutProp();
	virtual FLAggro *		GetAggroMng();

	virtual void			SetManualTarget( const OBJID /*objid*/ ) { __noop; }

protected:
	virtual void			sendAIMsg( const DWORD dwMessage, const DWORD dwParam1, const DWORD dwParam2 );

public:
	FLIdleState				m_IdleState;
	FLRageState				m_RageState;
	FLTokenToBeginState	m_ReturnToBeginState;
	FLRunawayState			m_RunawayState;

public:
	//FLAIOptionalFlags			m_OptinalFlags;


	OBJID					m_dwPartyOwnerOBJID;
	// 
	OBJID					m_OwnerObjid;
	FLAggro *				m_pAggro;

	// ��Ƽ ����.
	FLPartyAggroLeaderBehavior	m_LeaderBehavior;
	

	//neodeath : �Ѵ��� �ʿ��� �͵��� ���� ���°� �ƴ϶� FSM�� ���� ���� (�ʱ�ȭ �ڵ嵵 �Ѱ��� ����, ���� ���� �Լ� ����� ������. -_-)
	//IDLE
	FLPatrolBehavior			m_Patrol;
	D3DXVECTOR3					m_vPosBegin;			
	float						m_fPatrolRange;
	
	//RAGE 
	FLAttackBehavior			m_Attack;
	D3DXVECTOR3					m_vOldPos;				//�ɸ��Ͱ� ������ üũ�ϴ� ��ġ


	FLSummonMonsterBehavior		m_TrySummonMonster;
	DWORD						m_tmRageTimeOutInterval;

	FLCallHelperBehavior		m_CallHelper;			//�ֺ� ���� �θ�.
	
	//RUNAWAY
	FLRunawayNSelfHealBehavior	m_RunawayNSelfHeal;
};


#endif //defined __WORLDSERVER && defined __AGGRO16
