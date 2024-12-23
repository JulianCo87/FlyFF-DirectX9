#include "StdAfx.h"

#include "FLStalkerFSM.h"

#include "../WorldServer/User.h"

#undef max




const float MELEE_ATTACK_RANGE = 1.0f;




FLStalkerFSM::FLStalkerFSM( CMover & Owner ) 
: FLFSM< FLStalkerFSM >( Owner ) 
, m_TargetOBJID( NULL_ID )
{
	m_OptinalFlags.bIgnoreTraceTimeOut = TRUE;
	m_OptinalFlags.bIgnoreTraceRange = TRUE;
	
//	m_OptinalFlags.bIgnoreTransparent = TRUE;
//	m_OptinalFlags.bIgnoreMatchless = TRUE;
//	m_OptinalFlags.bIgnoreCollision = TRUE;


	ClearAll();
	ChangeState( m_IdleState );
}

FLStalkerFSM::~FLStalkerFSM()
{
}

void
FLStalkerFSM::InitProp()
{
	ClearAll();
	
	m_vPosBegin				= GetMover().GetPos();//vPosBegin;

	m_tmRageTimeOutInterval = ( m_OptinalFlags.bIgnoreTraceTimeOut == TRUE ) ? std::numeric_limits< DWORD >::max() : RAGE_TIME_OVER_MAX;
}

void
FLStalkerFSM::ClearAll()
{
	m_vPosBegin				= D3DXVECTOR3( 0, 0, 0 );			
	m_vOldPos				= D3DXVECTOR3( 0, 0, 0 );				//�ɸ��Ͱ� ������ üũ�ϴ� ��ġ

//	m_TargetOBJID			= NULL_ID;
}

void 
FLStalkerFSM::ClearWithoutProp()
{
	m_RageState.Clear();

	m_Attack.Clear();
	m_vOldPos				= D3DXVECTOR3( 0, 0, 0 );
}

void FLStalkerFSM::SetManualTarget( const OBJID objid )
{
	m_TargetOBJID = objid;
}


void			
FLStalkerFSM::sendAIMsg( const DWORD dwMessage, const DWORD dwParam1, const DWORD dwParam2 )
{
	FLFSM< FLStalkerFSM >::sendAIMsg( dwMessage, dwParam1, dwParam2 );
}

//--------------------------------------------------------------------------------------------------------//

void FLStalkerFSM::FLIdleState::OnEnter( FLStalkerFSM & FSM )
{
	FSM.ClearWithoutProp();
	FSM.GetMover().SetSpeedFactor( 2.0F );
}

void FLStalkerFSM::FLIdleState::UpdateAI( FLStalkerFSM & FSM, const AIMSG & msg )
{
	CMover & Owner			= FSM.GetMover();

	switch( msg.dwMessage )
	{
	case AIMSG_NONE:
	case AIMSG_PROCESS:
		{
			//��Ƽ ���͸��� �ϰ�� ���� �þ߷κ��� Ÿ���� ã�´�.
			// ������ ���°ų� �׾��� ��� �̵� ó�� ���� ���� 
			if( Owner.IsDie() || ( Owner.m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL ) )
				return;

			// Ÿ�� �˻�.
			CMover * pTarget	= FLScanTarget::Stalking( FSM.GetMover(), FSM.m_TargetOBJID, FSM.m_OptinalFlags );
			if( pTarget == NULL )
			{

				//--------------------------------------------------------------
#ifdef __INTERNALSERVER
				if( FSM.m_TargetOBJID == NULL_ID )
				{
					CMover * pTemp = FLScanTarget::ByNear( FSM.GetMover(), FSM.GetMover(), 2, FSM.m_OptinalFlags );
					if( pTemp != NULL )
					{
						FSM.SetManualTarget( pTemp->GetId() );
					}
   

				}                 

#endif //__INTERNALSERVER
				//--------------------------------------------------------------
				break;
			}
	
			FSM.ChangeState( FSM.m_RageState );
		}
		break;

	default:
		__noop;
		break;
	}
}

void FLStalkerFSM::FLIdleState::OnLeave( FLStalkerFSM & /*FSM*/ )
{
	__noop;
}


//--------------------------------------------------------------------------------------------------------//


FLStalkerFSM::FLRageState::FLRageState()
{
	Clear();
}

FLStalkerFSM::FLRageState::~FLRageState()
{

}

void FLStalkerFSM::FLRageState::Clear()
{
	m_tmAttackPoint			= 0;
}

void FLStalkerFSM::FLRageState::OnEnter( FLStalkerFSM & FSM )
{
	FSM.m_Attack.SetRageTimeOutTime( FSM.m_tmRageTimeOutInterval ); 

	FSM.GetMover().SetSpeedFactor( 2.0F );
}

void FLStalkerFSM::FLRageState::UpdateAI( FLStalkerFSM & FSM, const AIMSG & msg )
{	
	CMover & Owner			= FSM.GetMover();

	switch( msg.dwMessage )
	{
	case AIMSG_NONE:
	case AIMSG_PROCESS:
		{
			// ������ ���°ų� �׾��� ��� �̵� ó�� ���� ���� 
			if( Owner.IsDie() || ( Owner.m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL ) )
				return;

			// Ÿ�� �˻�.
			CMover * pTarget	= FLScanTarget::Stalking( FSM.GetMover(), FSM.m_TargetOBJID, FSM.m_OptinalFlags );
			if( pTarget == NULL )
			{
				FSM.GetMover().SetStop();
				FSM.ChangeState( FSM.m_IdleState );
				break;
			}

			// ���� �Ÿ� ����
			const BOOL bRangeAttack	= FSM.m_Attack.AutoChoiceRangeAttack( Owner, *pTarget );
			if( IsInRange( pTarget->GetPos() - Owner.GetPos(), MELEE_ATTACK_RANGE ) == TRUE )
			{
				if( Owner.IsEmptyDest() == FALSE )	// �̵�����.
				{
					SetMoveToDest( Owner, Owner.GetPos() );	// ������ġ�� �̵� - �������� ��ž�� ���� ���. Ŭ�󿡼� ������ ��ġ�� �̵�.
					//Owner.SetAngle( Owner.GetAngle() );
				}
			}
			else
			{
				SetMoveToDest( Owner, pTarget->GetId() );
			}

			//SetMoveToDest( Owner, pTarget->GetId() );		// �ٰŸ��� ������ ��ǥ������ ����.

			// ���� �����̸� ���� ����
			if( ::GetTickCount() > FSM.m_Attack.GetAttackPointTime() )
			{
				if( FSM.m_Attack.TryAttack( Owner, *pTarget, bRangeAttack, FSM.m_tmRageTimeOutInterval ) == FALSE )
					break;
			}
		}
		break;

	case AIMSG_DSTDIE:
		{
			//const OBJID objid	= ( OBJID )msg.dwParam1;
			//CMover * pTarget		= prj.GetMover( objid );
			//if( pTarget == NULL )
			//	FSM.GetMover().SetStop();
			//else
			//	SetMoveToDest( Owner, pTarget->GetPos() );

			FSM.ChangeState( FSM.m_TraceCarcassState );
		}
		break;

	case AIMSG_COLLISION: 
		{
			if( FSM.m_OptinalFlags.bIgnoreCollision == FALSE )
				FLSetRandomPos( FSM.GetMover() );
		}
		break;
	}
}

void FLStalkerFSM::FLRageState::OnLeave( FLStalkerFSM & /*FSM*/ )
{
	__noop;
}

//--------------------------------------------------------------------------------------------//


void FLStalkerFSM::FLTraceCarcassState::OnEnter( FLStalkerFSM & FSM )
{
	FSM.ClearWithoutProp();
}

void FLStalkerFSM::FLTraceCarcassState::UpdateAI( FLStalkerFSM & FSM, const AIMSG & msg )
{
	CMover & Owner			= FSM.GetMover();

	switch( msg.dwMessage )
	{
	case AIMSG_NONE:
	case AIMSG_PROCESS:
		{
			//��Ƽ ���͸��� �ϰ�� ���� �þ߷κ��� Ÿ���� ã�´�.
			// ������ ���°ų� �׾��� ��� �̵� ó�� ���� ���� 
			if( Owner.IsDie() || ( Owner.m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL ) )
				return;

			//��ü ���¶��
			CMover * pCarcass		= prj.GetMover( FSM.m_TargetOBJID );
			if( pCarcass != NULL && pCarcass->IsDie() == TRUE )
			{
				if( IsInRange( pCarcass->GetPos() - Owner.GetPos(), MELEE_ATTACK_RANGE ) == TRUE )
				{
					if( Owner.IsEmptyDest() == FALSE )	// �̵�����.
					{
						SetMoveToDest( Owner, Owner.GetPos() );		// ������ġ�� �̵� - �������� ��ž�� ���� ���. Ŭ�󿡼� ������ ��ġ�� �̵�.
					}
				}
				else
				{
					SetMoveToDest( Owner, pCarcass->GetId() );
				}
				break;
			}
			
			FSM.ChangeState( FSM.m_IdleState );
		}
		break;
	case AIMSG_ARRIVAL:
		FSM.ChangeState( FSM.m_IdleState );
		break;

	default:
		__noop;
		break;
	}
}

void FLStalkerFSM::FLTraceCarcassState::OnLeave( FLStalkerFSM & /*FSM*/ )
{
	__noop;
}


