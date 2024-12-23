#include "StdAfx.h"

#if defined __WORLDSERVER && defined __AGGRO16

#include "FLAggroNormalFSM.h"

#include "dialogmsg.h"

#include "AIInterface.h"
#include "FLBehaviorFunction.h"
#include "InstanceDungeonBase.h"

#include "../WorldServer/User.h"

#undef max


const static float			RANGE_RETURN_TO_BEGIN	= 120.0f;
const static float			RANGE_MOVE				= 15.0f;



FLAggroNormalFSM::FLAggroNormalFSM( CMover & Owner, const DWORD dwAggroID ) 
: FLFSM< FLAggroNormalFSM >( Owner ) 
, m_pAggro( new FLAggro( dwAggroID ) )
{
	ClearAll();
	ChangeState( m_IdleState );
}

FLAggroNormalFSM::~FLAggroNormalFSM()
{
	delete m_pAggro;
}

void					
FLAggroNormalFSM::SetPartyLeader( const DWORD dwLeaderObjid )
{
	m_dwPartyOwnerOBJID		= dwLeaderObjid;
}

void
FLAggroNormalFSM::InitProp()
{
	ClearAll();
	
	m_vPosBegin				= GetMover().GetPos();//vPosBegin;
	m_fPatrolRange			= RANGE_MOVE;

	m_tmRageTimeOutInterval = RAGE_TIME_OVER_MAX;
}

void
FLAggroNormalFSM::ClearAll()
{
	m_dwPartyOwnerOBJID		= NULL_ID;

	m_vPosBegin				= D3DXVECTOR3( 0, 0, 0 );
	m_fPatrolRange			= 0;

	m_tmRageTimeOutInterval = 0;
	m_OwnerObjid			= NULL_ID;

	ClearWithoutProp();
}

void 
FLAggroNormalFSM::ClearWithoutProp()
{
	m_RageState.Clear();


	if( GetAggroMng() != NULL )
		GetAggroMng()->Clear();

	m_Patrol.Clear();
	m_Attack.Clear();
	m_TrySummonMonster.Clear();
	m_vOldPos				= D3DXVECTOR3( 0, 0, 0 );
}

FLAggro *		
FLAggroNormalFSM::GetAggroMng()
{
	return m_pAggro;
}

void			
FLAggroNormalFSM::sendAIMsg( const DWORD dwMessage, const DWORD dwParam1, const DWORD dwParam2 )
{
	///aggro ���� �ൿ ó��. STATE_ ���� ������ ����.
	switch( dwMessage )
	{
	case AIMSG_REQUEST_HELP:
		{
			const OBJID	objid	= dwParam1;
			if( GetAggroMng() != NULL )
				GetAggroMng()->SumAggro( GetMover(), objid, 1 );		//@@@@@@@@@@@@@@@@@@
		}
		break;
	case AIMSG_INIT:
		{
		}
		break;
	case AIMSG_INIT_TARGETCLEAR:		
	case AIMSG_DSTDIE:	
		{
			const OBJID objid	= ( OBJID )dwParam1;
			if( GetAggroMng() == NULL )
				break;

			GetAggroMng()->RemoveMover( objid );
			if( GetAggroMng()->Size() == 0 )
			{
				GetAggroMng()->Clear();		
			}
			else 
			{
				CMover * pTarget = FLScanTarget::ByAggro( GetMover(), m_OptinalFlags );
				if( pTarget != NULL )
				{
					GetAggroMng()->SumNextTagerAggro( GetMover(), *pTarget );
				}
			}
		}
		break;
	case AIMSG_DSTDAMAGE:
		{
			__noop;
		}
		break;
	case AIMSG_DAMAGE:
		{
			const OBJID objid = ( OBJID )dwParam1;
			const int nDamage		= ( int )dwParam2;
			if( GetAggroMng() != NULL )
			{
				if(  GetAggroMng()->SumFirstAttackerAggro( GetMover(), objid ) == FALSE )
					GetAggroMng()->SumAttackAggro( GetMover(), objid, nDamage );
			}
		}
		break;

	
	case AIMSG_EXIT:
		{
			if( GetAggroMng() != NULL )
				GetAggroMng()->Clear();

			m_LeaderBehavior.ClearAllSubMonster();
		}
		break;
	case AIMSG_DIE:	
		{
			if( GetAggroMng() != NULL )
				GetAggroMng()->Clear();

			m_LeaderBehavior.KillSubMonsterAll( ( CMover * )dwParam1 );
		}
		break;

	case AIMSG_TIMEOUT:
		{
			if( GetAggroMng() != NULL )
				GetAggroMng()->Clear();
		}
		break;
	default:
		int i = 0;		//breakpoint ��
		UNREFERENCED_PARAMETER( i );
		__noop;
		break;
	}

	FLFSM< FLAggroNormalFSM >::sendAIMsg( dwMessage, dwParam1, dwParam2 );
}

//void FLAggroNormalFSM::SetManualTarget( const OBJID objid )
//{
//	//if( GetAggroMng() == NULL )
//	//	return;
//
//	//m_OptinalFlags.bStalkTarget			= TRUE;
//	//GetAggroMng()->Clear();
//	//GetAggroMng()->SumAggro( GetMover(), objid, std::numeric_limits< UINT64 >::max() );
//
//	//ChangeState( m_RageState );
//}
//--------------------------------------------------------------------------------------------------------//

void FLAggroNormalFSM::FLIdleState::OnEnter( FLAggroNormalFSM & FSM )
{
	FSM.ClearWithoutProp();
	FSM.GetMover().SetSpeedFactor( 1.0F );
}

void FLAggroNormalFSM::FLIdleState::UpdateAI( FLAggroNormalFSM & FSM, const AIMSG & msg )
{
	CMover & Owner			= FSM.GetMover();

	if( AIMSG_DIE != msg.dwMessage )
		FSM.m_LeaderBehavior.TryDoSummon( FSM.GetMover(), FSM.GetMover().GetWorld() );

	switch( msg.dwMessage )
	{
	case AIMSG_REQUEST_HELP:
		{
			const OBJID objid	= ( OBJID )msg.dwParam1;
			UNREFERENCED_PARAMETER( objid );
			FSM.ChangeState( FSM.m_RageState );
		}
		break;

	case AIMSG_NONE:
	case AIMSG_PROCESS:
		{
			//��Ƽ ���͸��� �ϰ�� ���� �þ߷κ��� Ÿ���� ã�´�.
			CMover * pTarget	= FSM.m_LeaderBehavior.TryScanTargetByNear_Party( FSM.m_OptinalFlags );
			if( pTarget != NULL )
			{
				FSM.ChangeState( FSM.m_RageState );
				break;
			}

			// �ٸ� ������ ��׷θ� ����....
			pTarget				= FLScanTarget::ByAggro( FSM.GetMover(), FSM.m_OptinalFlags );
			if( pTarget != NULL )
			{
				FSM.ChangeState( FSM.m_RageState );
				break;
			}

			if( FSM.GetMover().m_bActiveAttack == TRUE )
			{
				CMover * pTarget = FLScanTarget::ByNear( Owner, Owner, Owner.GetProp()->m_nAttackFirstRange, FSM.m_OptinalFlags );
				if( pTarget != NULL )
				{
					if( FSM.GetAggroMng() != NULL )
						FSM.GetAggroMng()->SumFirstTargetAggro( Owner, pTarget->GetId() );

					FSM.ChangeState( FSM.m_RageState );
					break;
				}
			}

			//������ �׾��ų� ���ٸ�.
			CMover * pLeader = prj.GetMover( FSM.m_dwPartyOwnerOBJID );
			if( IsValidObj( pLeader ) == TRUE && pLeader->IsLive() )		
			{
				//�������� ���ø� �޴´�.
				//FSM.m_LeaderBehavior.SetSubMonsterPos( FSM.GetMover() );	//���� ����
			}
			else	//��ü patrol
			{
				if( FSM.m_Patrol.PatrolUpdate( Owner, FSM.m_vPosBegin, FSM.m_fPatrolRange ) == TRUE )
					FSM.m_LeaderBehavior.SetSubMonsterPos( FSM.GetMover() );	//���� ����
			}
		}
		break;
	case AIMSG_ARRIVAL:
		{
			CMover * pLeader = prj.GetMover( FSM.m_dwPartyOwnerOBJID );
			if( IsValidObj( pLeader ) == TRUE && pLeader->IsLive() )		//������ �ƴϸ�
			{
				FSM.GetMover().SetAngle( pLeader->GetAngle() );
				FSM.GetMover().SetStop();
			}
			else	// �����̰ų� ������ ������
			{
				FSM.m_Patrol.SetStopMoverTimeOut( FSM.GetMover(), SEC( 5 ) + xRandom( SEC( 1 ) )  );
			}
		}
		break;

	case AIMSG_DAMAGE:
		{
			FSM.ChangeState( FSM.m_RageState );
		}
		break;
	
	case AIMSG_COLLISION: 
		{
			if( FSM.m_OptinalFlags.bIgnoreCollision == FALSE )
				FSM.m_Patrol.SetRandomPatrolPos( Owner, FSM.m_vPosBegin, FSM.m_fPatrolRange );
		}
		break;

	default:
		__noop;
		break;
	}
}

void FLAggroNormalFSM::FLIdleState::OnLeave( FLAggroNormalFSM & /*FSM*/ )
{
	__noop;
}

//-------------------------------------------------------------------------------------------//

FLAggroNormalFSM::FLRageState::FLRageState()
{
	Clear();
}

FLAggroNormalFSM::FLRageState::~FLRageState()
{

}

void FLAggroNormalFSM::FLRageState::Clear()
{
	m_tmAttackPoint			= 0;
}

void FLAggroNormalFSM::FLRageState::OnEnter( FLAggroNormalFSM & FSM )
{
	FSM.m_Attack.SetRageTimeOutTime( /*::GetTickCount() + */FSM.m_tmRageTimeOutInterval ); 

	FSM.GetMover().SetSpeedFactor( 2.0F );
}

void FLAggroNormalFSM::FLRageState::UpdateAI( FLAggroNormalFSM & FSM, const AIMSG & msg )
{	
	CMover & Owner			= FSM.GetMover();

	switch( msg.dwMessage )
	{
	case AIMSG_INVALID_TARGET:
	case AIMSG_DSTDIE:
	case AIMSG_NONE:
	case AIMSG_PROCESS:
		{
			// ������ ���°ų� �׾��� ��� �̵� ó�� ���� ���� 
			if( Owner.IsDie() || ( Owner.m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL ) )
				return;

			// ���� ������ �Ÿ��� �ְų� || �ð� �ʰ��̸� RETURN TO HOME
			if( FSM.m_OptinalFlags.bIgnoreTraceRange == FALSE )
			{
				const D3DXVECTOR3 vDist = Owner.GetPos() - FSM.m_vPosBegin;
				const float fRange		= Owner.IsFlyingNPC() ? ( RANGE_MOVE * 3.0f ) : RANGE_MOVE;		
				if( CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon( Owner.GetWorld()->GetID() ) == FALSE
					&& IsInRange( vDist, RANGE_RETURN_TO_BEGIN + fRange ) == FALSE )	
				{
					FSM.ChangeState( FSM.m_ReturnToBeginState );
					break;
				}
			}

			// Ÿ�� �˻�.
			CMover * pTarget	= FLScanTarget::ByAggro( FSM.GetMover(), FSM.m_OptinalFlags );
			if( pTarget == NULL )
			{
				FSM.ChangeState( FSM.m_ReturnToBeginState );
				break;
			}

			//neodeath : �м� ��� �������� ���¿��� ���� ���� �ȿ��� �����ð����� ������ �ѹ��� ������ ��� 
			//			�����ִٰ� �Ǵ��Ͽ� ���� ��Ŵ ( ����� ���� �ڵ� )
			if( ::GetTickCount() > FSM.m_Attack.GetRageTimeOutTime() )			// ���� ���� �ð��� ���� �Ǿ�����.
			{
				//���� ���Ѱ��� �����ڵ��ε�	
				FSM.m_Attack.SetRageTimeOutTime( /*::GetTickCount() + */FSM.m_tmRageTimeOutInterval );
				// 5���� ��ǥ�� ���� ��ǥ�� �Ÿ��� ���.
				D3DXVECTOR3 vDist	= Owner.GetPos() - FSM.m_vOldPos;
				vDist.y				= 0;
				if( IsInRange( vDist, 2.0f ) )			//@@@@@ 2���͸� ������ٸ� ��򰡿� �����ٴ� ��.
				{
					if( Owner.GetSpeed( Owner.m_pActMover->m_fSpeed ) > 0 )	//@@@@@@  ���������� �������� ���ϰ� �Ȼ��¿��� �����ϸ� �ȵ�.
					{
						Owner.SetPos( pTarget->GetPos() );
						g_xWSUserManager->AddSetPos( &Owner, Owner.GetPos() );
					}
				}
				FSM.m_vOldPos			= Owner.GetPos();
			}

			// �ڱ��� �õ�
			const BOOL bRecvCondition = RecvConditionUpdate( Owner );
			if( bRecvCondition == TRUE )
			{
				FSM.m_Attack.SetAttackPointTime( ::GetTickCount() + SEC( 3 ) );
			}

			// ���� �Ÿ� ����
			const BOOL bRangeAttack	= FSM.m_Attack.AutoChoiceRangeAttack( Owner, *pTarget );
			SetMoveToDest( Owner, pTarget->GetId() );		// �ٰŸ��� ������ ��ǥ������ ����.

			// ���� �����̸� ���� ����
			if( ::GetTickCount() > FSM.m_Attack.GetAttackPointTime() )
			{
				if( FSM.m_Attack.TryAttack( Owner, *pTarget, bRangeAttack, FSM.m_tmRageTimeOutInterval ) == FALSE )
					break;
			}

			// �ڽ� ���� ��ȯ �õ�		
			FSM.m_TrySummonMonster.TrySummonUpdate( Owner );	
		}
		break;

	case AIMSG_DAMAGE:
		{
			// Ÿ�� �˻�.			
			CMover * pTarget = FLScanTarget::ByAggro( FSM.GetMover(), FSM.m_OptinalFlags );
			if( pTarget == NULL )
			{
				if( FSM.m_RunawayNSelfHeal.IsFirstRunaway() == FALSE )
					FSM.ChangeState( FSM.m_ReturnToBeginState );
				break;
			}

			// ���ְ� ON�̶��
			if( Owner.GetHitPointPercent( 100 ) < Owner.GetProp()->m_nRunawayHP )
			{
				FSM.ChangeState( FSM.m_RunawayState );
			}

			// �ֺ� ���Ḧ �θ� �� �ִٸ�.
			if( Owner.GetProp()->m_bHelpWho )	// ����AI�� �������.
			{
				FSM.m_CallHelper.TryCallHelper( FSM.GetMover(), *pTarget );
			}
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

void FLAggroNormalFSM::FLRageState::OnLeave( FLAggroNormalFSM & /*FSM*/ )
{
	__noop;
}

//------------------------------------------------------------------------------------------------------//



void FLAggroNormalFSM::FLTokenToBeginState::OnEnter( FLAggroNormalFSM & FSM )
{
	FSM.GetMover().m_IsReturnToBegin		= TRUE;
	SetMoveToDest( FSM.GetMover(), FSM.m_vPosBegin );
	FSM.GetMover().SetSpeedFactor( 2.66F );
}

void FLAggroNormalFSM::FLTokenToBeginState::UpdateAI( FLAggroNormalFSM & FSM, const AIMSG & msg )
{
	switch( msg.dwMessage )
	{
	case AIMSG_NONE:
	case AIMSG_PROCESS:
		{
			if( IsInRange( FSM.m_vPosBegin - FSM.GetMover().GetPos(), 7.0f ) == TRUE )		//neodeath : 7.0f �ϵ� �ڵ� �Ǿ��־���. �� �߸��� �ƴ�	
			{
				SetMaxHP( FSM.GetMover() );
				FSM.ChangeState( FSM.m_IdleState );		
			}
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

void FLAggroNormalFSM::FLTokenToBeginState::OnLeave( FLAggroNormalFSM & FSM )
{
	FSM.GetMover().m_IsReturnToBegin		= FALSE;
}


//------------------------------------------------------------------------------------------------------//
void FLAggroNormalFSM::FLRunawayState::OnEnter( FLAggroNormalFSM & FSM )
{
	CMover * pTarget	= FLScanTarget::ByAggro( FSM.GetMover(), FSM.m_OptinalFlags );
	if( pTarget == NULL )
	{
		FSM.ChangeState( FSM.m_ReturnToBeginState );
		return;
	}

	g_xWSUserManager->AddDlgEmoticon( &FSM.GetMover(), DLGEMOT_EVADE );
	FSM.m_RunawayNSelfHeal.StartInit( FSM.GetMover(), *pTarget );
}

void FLAggroNormalFSM::FLRunawayState::UpdateAI( FLAggroNormalFSM & FSM, const AIMSG & msg )
{
	CMover & Owner			= FSM.GetMover();

	switch( msg.dwMessage )
	{
	case AIMSG_DSTDIE:
	case AIMSG_NONE:
	case AIMSG_PROCESS:
		{
			// ������ ���°ų� �׾��� ��� �̵� ó�� ���� ���� 
			if( Owner.IsDie() || ( Owner.m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL ) )
				return;

			// Ÿ�� �˻�.
			CMover * pTarget	= FLScanTarget::ByAggro( FSM.GetMover(), FSM.m_OptinalFlags );
			if( pTarget == NULL )
			{
				FSM.ChangeState( FSM.m_ReturnToBeginState );
				break;
			}

			// ü���� 50 % �̻� �Ǹ� �ݰ� �õ�. ( ��𼱰� ���� �Ѵٸ� )
			if( Owner.GetHitPointPercent( 100 ) >= 50 )
			{
				FSM.ChangeState( FSM.m_RageState );
				break;
			}

			// ���� ������Ʈ.
			if( pTarget != NULL )
				FSM.m_RunawayNSelfHeal.RunawayUpdate( Owner, *pTarget );
		}
		break;
	case AIMSG_COLLISION:
		{
			// ������ �ݴ� �������� ����
			// 5�� ���� ������ ���� ����. ���� ũ�� Ŭ���� ������� ���·� �����δ�. 
			Owner.AddAngle( ( float )( 180 + ( 20 - xRandom( 40 ) ) ) );

			const FLOAT fAngle		= Owner.GetAngle();
			const D3DXVECTOR3 vDst	= Owner.GetPos() + VelocityToVec( fAngle, 5 );
			SetMoveToDest( Owner, vDst );
		}
		break;

	case AIMSG_ARRIVAL:
		{
			if( Owner.GetProp()->m_dwRunawayDelay )
			{
				FSM.m_RunawayNSelfHeal.SetStopMoverTimeOut( Owner, Owner.GetProp()->m_dwRunawayDelay + xRandom( SEC( 1 ) ) );
			}
		}
		break;
	}
}

void FLAggroNormalFSM::FLRunawayState::OnLeave( FLAggroNormalFSM & /*FSM*/ )
{
	__noop;
}



#endif //defined __WORLDSERVER && defined __AGGRO16