#pragma once

#include "AIInterface.h"

#include "..\_CommonDefine\FLSingleton.h"
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/static_assert.hpp>

class CMover;
class FLAggro;


struct FLAIOptionalFlags
{
public:
	FLAIOptionalFlags() 
		: bIgnoreTraceTimeOut( FALSE ), bIgnoreTraceRange( FALSE ), bIgnoreTransparent( FALSE ), bIgnoreMatchless( FALSE ), bIgnoreCollision( FALSE ), bOneKillMode( FALSE ), bUnconditionalHit( FALSE )
	{}
	~FLAIOptionalFlags()   	{}

	BOOL				bIgnoreTraceTimeOut;	//Ÿ�� �ƿ� ����
	BOOL				bIgnoreTraceRange;		//�ָ� �������� �i�ư�

	BOOL				bIgnoreTransparent;		//���� ����
	BOOL				bIgnoreMatchless;		//���� ����
	BOOL				bIgnoreCollision;		//�浹 ����

	BOOL				bOneKillMode;			//�ѹ濡 ����
	BOOL				bUnconditionalHit;		//������ ��Ʈ
};


// message ������ �ǹ̸� ����.
class IFSM
{
public:
	IFSM()	{}
	virtual ~IFSM()		{}

	//���°��� ������ ���. Mover�� IsReturnToBegin���� �������ָ� ��.
	virtual void			InitProp() = 0;
	virtual void			UpdateAI( const DWORD dwMessage, const DWORD dwParam1, const DWORD dwParam2 ) = 0;
	virtual void			PostAIMsg( const DWORD dwMessage, const DWORD dwParam1, const DWORD dwParam2 );

	virtual void			SetManualTarget( const OBJID objid ) = 0;

	virtual FLAggro *		GetAggroMng() = 0;

	//virtual const FLAIOptionalFlags &	GetOptionalFlags() const = 0;
	virtual FLAIOptionalFlags &	GetOptionalFlags() = 0;

protected:
	virtual void			sendAIMsg( const DWORD dwMessage, const DWORD dwParam1, const DWORD dwParam2 );

protected:
	queue< AIMSG >			m_MsgQ;
};



// ���� ��ȯ update
template < class _ConcreteFSM >			//template < class _Mover, class _ConcreteFSM >
class FLFSM : public IFSM
{
public:
	enum E_STATE 
	{
		IDLE_STATE				= 0,
		RAGE_STATE,
		RUNWAY_STATE,
		RETURN_TO_BEGIN_STATE,
	};

public:
	struct FLState														
	{
		FLState()				{}
		~FLState()				{}
		virtual	void			OnEnter( _ConcreteFSM & FSM ) = 0;
		virtual void			UpdateAI( _ConcreteFSM & FSM, const AIMSG & msg ) = 0;
		virtual	void			OnLeave( _ConcreteFSM & FSM ) = 0;
	};

	struct FLNullState : public FLState//, public FLSingleton< FLNullState >
	{
		FLNullState()			{}
		~FLNullState()			{}
		static FLNullState &	GetInstance()
		{
			static FLNullState inst;
			return inst;
		}

		virtual	void			OnEnter( _ConcreteFSM & /*FSM*/ )							{}
		virtual void			UpdateAI( _ConcreteFSM & /*FSM*/, const AIMSG & /*msg*/ )	{}
		virtual	void			OnLeave( _ConcreteFSM & /*FSM*/ )							{}
	};
	inline static			FLNullState & NULL_STATE()		{ return FLNullState::GetInstance();	}

public:
	FLFSM( CMover & Owner );
	virtual ~FLFSM();
	
	virtual void			UpdateAI( const DWORD dwMessage, const DWORD dwParam1, const DWORD dwParam2 );

	void					ChangeState( FLState & State ); 
	CMover &				GetMover();

	//virtual const FLAIOptionalFlags &	GetOptionalFlags() const;
	virtual FLAIOptionalFlags &	GetOptionalFlags();


//	private: virtual void			sendAIMsg( const DWORD dwMessage, const DWORD dwParam1, const DWORD dwParam2 );

public:
	CMover &				m_Owner;				
	FLState *				m_pState;

	FLAIOptionalFlags		m_OptinalFlags;
};


//------------------------------------------- ������ -------------------------------------------------------------//

template < class _ConcreteFSM >
FLFSM< _ConcreteFSM >::FLFSM( CMover & Owner )
: m_Owner( Owner )
, m_pState( & NULL_STATE() )
{
	BOOST_STATIC_ASSERT( ( boost::is_base_and_derived< FLFSM< _ConcreteFSM >, _ConcreteFSM >::value == true ) );	
}

template < class _ConcreteFSM >
FLFSM< _ConcreteFSM >::~FLFSM()
{
}


template < class _ConcreteFSM >
void
FLFSM< _ConcreteFSM >::UpdateAI( const DWORD dwMessage, const DWORD dwParam1, const DWORD dwParam2 )
{
	while( m_MsgQ.empty() == false )
	{
		const AIMSG qmsg		= m_MsgQ.front();
		m_MsgQ.pop();
		m_pState->UpdateAI( *static_cast< _ConcreteFSM * >( this ), qmsg );
	}


	AIMSG msg;
	msg.dwMessage			= dwMessage;
	msg.dwParam1			= dwParam1;
	msg.dwParam2			= dwParam2;

	m_pState->UpdateAI( *static_cast< _ConcreteFSM * >( this ), msg );
}

template < class _ConcreteFSM >
void 
FLFSM< _ConcreteFSM >::ChangeState( FLState & State )
{
	m_pState->OnLeave( * static_cast< _ConcreteFSM * >( this ) );
	m_pState			= & State;
	m_pState->OnEnter( * static_cast< _ConcreteFSM * >( this ) );
}

template < class _ConcreteFSM >
CMover &				
FLFSM< _ConcreteFSM >::GetMover()
{
	return m_Owner;
}

template < class _ConcreteFSM >			//template < class _Mover, class _ConcreteFSM >
FLAIOptionalFlags & FLFSM<_ConcreteFSM>::GetOptionalFlags()
{
	return m_OptinalFlags;
}
