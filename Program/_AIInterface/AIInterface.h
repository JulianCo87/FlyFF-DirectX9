#ifndef __AI_INTERFACE_H
#define __AI_INTERFACE_H

#include <MoverMsg.h>

#include <FLAggro.h>

class CAIInterface;

struct AIMSG
{
	DWORD dwMessage;
	DWORD dwParam1;
	DWORD dwParam2;
};

typedef BOOL (CAIInterface::*FUNCAISTATE)( const AIMSG & );



#pragma	warning( push )
#pragma warning( disable : 4121 )	
struct STATEMAP_ENTRIES
{
	DWORD dwAIStateId;
	FUNCAISTATE pStateFunc;
};
#pragma warning( pop )

struct STATEMAP
{
	STATEMAP* lpBaseStateMap;
	STATEMAP_ENTRIES* lpStateMapEntries;
};

enum
{
	AIMSG_NONE = 0,
	AIMSG_INIT = 1,
	AIMSG_PROCESS, 
	AIMSG_TIMEOUT,				//�i�ư��� Ÿ�Ӿƿ�?
	AIMSG_SETSTATE,	
	AIMSG_SETPROCESS,
	AIMSG_DSTDAMAGE,
	AIMSG_DSTDIE,				//���� ����?
	AIMSG_DAMAGE,
	AIMSG_DIE,					// ����		nParam1 ( Attacker pointer ) 
	AIMSG_COLLISION,			// �浹 Collision
	AIMSG_ARRIVAL,				// �̵� ��ǥ�� ���� 
	AIMSG_NOMOVE,				// �� �� ���� 
	AIMSG_BEGINMOVE,			// �̵� ���۽� 
	AIMSG_ATTACK_MELEE,			// �Ϲݰ��ݽ� Ÿ������ ȣ��.
	AIMSG_END_MELEEATTACK,		// �Ϲݰ��� �������� ȣ��.
	AIMSG_END_APPEAR,			// ����� ����.
	AIMSG_INIT_TARGETCLEAR,		// Ÿ���� Ŭ�����ϰ� ������ ���ư�.
	AIMSG_EXIT,					// DELETE?
	AIMSG_TARGET_FIND,			// Ÿ���� ã�Ҵ�.
	AIMSG_REATTACK_TIMEOUT,  
	AIMSG_INVALID_TARGET,		// Ÿ���� ���ų�, �������ų�, �װų� .. ������� ���� �Ұ��ϴ�. 

	AIMSG_REQUEST_HELP,			//dwParam1 : Ÿ��
	
	

//	AIMSG_SET_TARGET,			//dwParam1 : Ÿ��
//	AIMSG_PARTY_MONSTER_DAMAGE,	//dwparam1 : �������� ������
//	AIMSG_PARTY_MONSTER_ATTACK,	//dwparam1 : ������ �� ����
		

//	AIMSG_TARGET_FIND,			// PCObjID
	//AIMSG_FIRST_DAMAGE_NOTI,		//dwParam1 : AttackerObjID
	//AIMSG_FIRST_TARGET_NOTI,		//dwParam1 : TargetObjID
	//AIMSG_DEBUFF_NOTI,			//dwParam1 : AttackerObjID		//FLAggro::SumDebuffAggro( *pSrcMover, *pThisMover );
	//AIMSG_DST_BUFF_NOTI,			//dwParam1 : BehaviorObjID		//FLAggro::SumBuffAggro( *pSrcMover, *pThisMover );
	//AIMSG_DST_HEAL_NOTI,			//dwParam1 : BehaviorObjID
};

//struct AI_STATE	{	enum E {
//	IDLE			= 0,
//	RAGE,
//	RUNAWAY,
//
//};					};

enum AI2_STATE
{
	AI2_IDLE,
	AI2_MOVE,
	AI2_RAGE,
	AI2_SEARCH,			// Ž�� ���� 
	AI2_TRACKING,		// ���� ���� 
	AI2_ATTACK,			// ���� ���� 
};

//////////////////////////////////////////////////////////////////////////////
// AI �޽��� �ڵ鷯 ��ũ�� ���� 
#define BeginAIHandler() if( 0 ) { 
#define OnMessage( x ) return TRUE; } else if( msg.dwMessage == x )	{
#define EndAIHandler() return TRUE; }

// State Map �ڵ鷯 ��ũ�� ���� 
#define BEGIN_AISTATE_MAP( theClass, baseClass ) \
	const STATEMAP* theClass::GetStateMap() const  \
		{ return (STATEMAP*)&theClass::stateMap; } \
	const STATEMAP theClass::stateMap = \
		{ (STATEMAP*)&baseClass::stateMap, (STATEMAP_ENTRIES*)&theClass::stateMapEntries[0] }; \
	const STATEMAP_ENTRIES theClass::stateMapEntries[] = { \

#define END_AISTATE_MAP() 0, NULL };
#define ON_STATE( id, pStateFunc ) id, (FUNCAISTATE)&thisClass::pStateFunc, 
#define DECLARE_AISTATE( theClass ) \
	typedef	theClass	thisClass;	\
	static const STATEMAP_ENTRIES stateMapEntries[]; \
	static const STATEMAP stateMap; \
	virtual const STATEMAP* GetStateMap() const; 

//////////////////////////////////////////////////////////////////////////////

#pragma	warning( push )
#pragma warning( disable : 4121 )	

class CAIInterface
{
protected:
	FUNCAISTATE GetAIStateFunc( DWORD dwState );

protected:
	queue< AIMSG >	m_MsgQueue;
//	CMclCritSec		m_AddRemoveLock;
	FUNCAISTATE		m_pStateFunc;
	CObj*			m_pObj;

#if defined __WORLDSERVER && defined __AGGRO16
	BOOL			m_HasAggro;
#endif //__AGGRO16

public:
	DECLARE_AISTATE( CAIInterface )

	CAIInterface();
	CAIInterface( CObj* pObj );
	virtual	~CAIInterface();

#if defined __WORLDSERVER && defined __AGGRO16
	BOOL				HasAggro() const 				{ return m_HasAggro; }
	virtual FLAggro *	GetAggroMng() 					{ return NULL; }
#endif //__AGGRO16

	// �Ž��� ���� ó�� �żҵ� 
	virtual void	RouteMessage();
	virtual void	SendAIMsg( DWORD dwMessage, DWORD dwParam1 = 0, DWORD dwParam2 = 0 );
	virtual void	PostAIMsg( DWORD dwMessage, DWORD dwParam1 = 0, DWORD dwParam2 = 0 );
	virtual void	InitAI() { }

	virtual	BOOL	IsReturnToBegin( void )		{	return FALSE;	}
	virtual BOOL	IsIdleMode()			= 0;

protected:
	CMover* GetMover() { return (CMover*)m_pObj; }
	CWorld* GetWorld() { return m_pObj->GetWorld(); }

	//��Ÿ ����� �żҵ� 
	//CMover *				CAIInterface::ScanMaxAggro();

	CMover* ScanTarget( CObj* pObjCenter, int nRangeMeter = 5, int nJobCond = 0, DWORD dwQuest = 0, DWORD dwItem = 0, int nChao = 0 );
	CMover* ScanTargetStrong( CObj* pObjCenter, FLOAT fRangeMeter );
	CMover* ScanTargetOverHealer( CObj* pObjCenter, FLOAT fRangeMeter  );

	CMover*	ScanTargetNext( CObj* pObjCenter, int nRange,  OBJID dwIdTarget, u_long uParty );
	u_long	m_uParty;
};
//////////////////////////////////////////////////////////////////////////////

#pragma warning( pop )


#if defined __WORLDSERVER && defined __AGGRO16
class CAIAggroInterface : public CAIInterface 
{
public:
	CAIAggroInterface( CObj * pMover, const DWORD dwAggroID ) ;		//@  AIInterface���� ������ Obj�� �ƴ϶� Mover���� �ؾ���
	virtual ~CAIAggroInterface();

	void					Init();
	virtual FLAggro *		GetAggroMng();

	CMover *				ScanTargetByContidion( const int nRangeMeter = 5, const int nJobCond = 0, const DWORD dwQuest = 0, const DWORD dwItem = 0, const int nChao = 0 );
	virtual void			SendAIMsg( DWORD dwMessage, DWORD dwParam1 = 0, DWORD dwParam2 = 0 );

public:
	FLAggro *				m_pAggro;
	//AggroPtr				m_pAggro;
	//FLAggro					m_AggroMng;
};
#endif //__AGGRO16


#endif

