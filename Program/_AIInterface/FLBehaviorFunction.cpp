#include "StdAfx.h"

#include "FLBehaviorFunction.h"
#include "AIInterface.h"
#include "DialogMsg.h"


#include "FLPartyAggroFSM.h"

#ifdef __WORLDSERVER
#include "../WorldServer/User.h"
#include "../worldserver/FLMapGrid.h"
#include "../worldserver/LinkMap.h"
#endif 



#include "party.h"
extern	CPartyMng			g_PartyMng;

#undef max

//-------------------------------------------------------------------------------------------------------------------------------------------------//

FLPartyAggroLeaderBehavior::FLPartyAggroLeaderBehavior(/* const DWORD dwMoverIndex ) : m_dwMoverIndex( dwMoverIndex )*/)
{

}

FLPartyAggroLeaderBehavior::~FLPartyAggroLeaderBehavior()
{

}

CMover * FLPartyAggroLeaderBehavior::TryScanTargetByNear_Party( const FLAIOptionalFlags & kOptionalFlags )
{
	for( CalledMonsterMap::iterator it = m_mapCalledMonster.begin(); it != m_mapCalledMonster.end(); ++it )
	{
		const OBJID objid	= it->second;
		CMover * pMover		= prj.GetMover( objid );
		if( IsValidObj( pMover ) && pMover->IsLive() && pMover->m_bActiveAttack == TRUE )
		{
			return FLScanTarget::ByNear( *pMover, *pMover, pMover->GetProp()->m_nAttackFirstRange, kOptionalFlags );
		}
	}

	return NULL;
}

void FLPartyAggroLeaderBehavior::TryDoSummon( CMover & Leader, CWorld * pWorld/*, const MoverProp & Prop*/ )
{
	typedef				MoverProp::SummonMonstersMap		Map;

	if( IsValidObj( &Leader ) == FALSE )
		return;

	if( Leader.IsDie() == TRUE )
		return;
	
	if( pWorld == NULL )
		return;


	MoverProp * pProp	= prj.GetMoverProp( Leader.m_dwIndex );
	if( pProp == NULL )
		return;

	//const float fBaseAngle	= 180.f / Prop.m_mapSubMonsters.size();
	//const float fDist		= 1.0f;

	// ����Ʈ �߷��� �װų� ���� �͸� ��ȯ
	int i = 0;
	for( Map::const_iterator it = pProp->m_mapSummonMonsters.begin(); it != pProp->m_mapSummonMonsters.end(); ++it, ++i )
	{
		MoverProp::FLMonsterKey monsterKey		= it->first;
		//const DWORD	dwMoverIndex	= it->first;
		//const DWORD dwSequence		= it->second;

		CalledMonsterMap::const_iterator subIt = m_mapCalledMonster.find( monsterKey );
		if( subIt != m_mapCalledMonster.end() )
		{
			const OBJID objid		= subIt->second;
			CMover * pSub			= prj.GetMover( objid );
			if( pSub != NULL )
				continue;
			//if( IsValidObj( pSub ) == TRUE && pSub->IsLive() )
			//	continue;
		}

		const D3DXVECTOR3 vPos	= GetSubMonsterPosition( Leader.GetPos(), Leader.GetAngle(), i, pProp->m_mapSummonMonsters.size(), SUB_MONSTER_DIST );
		CMover * pMover		= CreateMover( pWorld, monsterKey.dwMoverID, vPos, 0 );
		if( pMover == NULL )
		{
			DEBUG_BREAK;
			continue;
		}	

		FLPartyAggroSubMonsterFSM * pFSM = static_cast< FLPartyAggroSubMonsterFSM * >( pMover->m_pFSM );
		if( pMover->GetProp()->dwAI != AII_PARTY_AGGRO_SUB || pMover->m_pFSM == NULL )
		{
			DEBUG_BREAK;
			delete pMover;
		}

		pFSM->SetPartyLeader( Leader.GetId() );

		m_mapCalledMonster.insert( CalledMonsterMap::value_type( monsterKey, pMover->GetId() ) );
	}


	Update_CleanKilledSubMonster();
}

void FLPartyAggroLeaderBehavior::Update_CleanKilledSubMonster()
{
	for( CalledMonsterMap::iterator it = m_mapCalledMonster.begin(); it != m_mapCalledMonster.end(); )
	{
		const OBJID objid	= it->second;
		CMover * pMover		= prj.GetMover( objid );
		if( IsValidObj( pMover ) && pMover->IsLive() )
			++it;
		else
			it				= m_mapCalledMonster.erase( it );			//@@@@@@@@ ���� �ؾ��ϳ�?
	}
}


void FLPartyAggroLeaderBehavior::ClearAllSubMonster()
{
	for( CalledMonsterMap::iterator it = m_mapCalledMonster.begin(); it != m_mapCalledMonster.end(); ++it )
	{
		const OBJID objid	= it->second;
		CMover * pMover	= prj.GetMover( objid );
		if( pMover == NULL )
			continue;

		pMover->Delete();
	}
}

void FLPartyAggroLeaderBehavior::KillSubMonsterAll( CMover * pAttacker )
{
	for( CalledMonsterMap::iterator it = m_mapCalledMonster.begin(); it != m_mapCalledMonster.end(); ++it )
	{
		const OBJID objid	= it->second;
		CMover * pTarget	= prj.GetMover( objid );
		if( pTarget == NULL )
			continue;

		//������ ���� ����.
		if( pAttacker != NULL )
		{
			pAttacker->SubExperience( pTarget );		// pTarget�� ���̰� ������ m_pAttacker ����ġ ó��.
			pAttacker->m_nAtkCnt	= 0;				// Ÿ���� �׿����� �������� ����ī��Ʈ Ŭ����
		}

		switch( _GetContentState( CT_DROP_ITEM_REFORM  ) )
		{
		case CS_VER1:
			{
				pTarget->OnDie( *pAttacker );
			}
			break;
		default:
			pTarget->DropItemByDied( pAttacker );				// ���Ϳ��ٸ� ������ ���.
			break;
		}

		pTarget->DoDie( pAttacker );				// pTarget �׾��. 
		pTarget->m_nHitPoint	= 0;
	}	
}


void FLPartyAggroLeaderBehavior::SetSubMonsterPos( CMover & Leader )
{
	int i = 0;
	for( CalledMonsterMap::iterator it = m_mapCalledMonster.begin(); it != m_mapCalledMonster.end(); ++it, ++i )
	{
		const OBJID objid	= it->second;
		CMover * pMover	= prj.GetMover( objid );
		if( IsValidObj( pMover ) == FALSE )
			continue;

		if( pMover->IsDie() == TRUE )
			continue;

		pMover->SetStop();
		const D3DXVECTOR3 vDest = GetSubMonsterPosition( Leader.GetDestPos(), Leader.GetAngle(), i, m_mapCalledMonster.size(), SUB_MONSTER_DIST );
		//pMover->SetPos(  );
		SetMoveToDest( *pMover, vDest );
	}
}

D3DXVECTOR3				FLPartyAggroLeaderBehavior::GetSubMonsterPosition( const D3DXVECTOR3 vCenterPos, const float fAngle, const DWORD nSequenceIndex, const DWORD nSequenceMax, const float fDist )
{
	const float fBaseAngle	= 180.f / ( nSequenceMax + 1 );

	const FLOAT fSubAngle		= fAngle + 90.0f + ( fBaseAngle * nSequenceIndex );
	D3DXVECTOR3 vSub;
	AngleToVectorXZ( &vSub, fSubAngle, fDist );						

	const D3DXVECTOR3 vPos	= vCenterPos + vSub;
	//const D3DXVECTOR3 vPos	= Leader.GetDestPos() + vSub;
	return vPos;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------//

FLPatrolBehavior::FLPatrolBehavior()
{
	Clear();
}


FLPatrolBehavior::~FLPatrolBehavior()
{

}

void FLPatrolBehavior::Clear()
{
	m_tmStopMoverTimeOut	= 0;
}

BOOL FLPatrolBehavior::PatrolUpdate( CMover & Src, const D3DXVECTOR3 & vPosBegin, const float fPatrolRange )
{
	if( Src.GetMovePattern() == 0 )			// ������ ���ٸ� ( AI�� �����;��� )
	{
		const DWORD tmTick	= ::GetTickCount();
		if( m_tmStopMoverTimeOut < tmTick && Src.m_pActMover->IsMove() == FALSE )//== FALSE )
		{
			SetRandomPatrolPos( Src, vPosBegin, fPatrolRange );
			return TRUE;
		}	
	}
	else if( Src.GetMoveEvent() == 99 )		//@@@@@@  8�ں��� ( �ϵ��ڵ� �Ǿ� ���� )
	{
		Src.SetMovePattern( 0 );				//�Ϲ� ���� ��ȯ
		return TRUE;
	}
	return FALSE;
}

void FLPatrolBehavior::SetRandomPatrolPos( CMover & Src, const D3DXVECTOR3 & vPosBegin, const float fPatrolRange )
{
	D3DXVECTOR3 vDest		= Src.GetPos();
	vDest.x					+= ( int )( xRand() % 21 ) - 10;		//@@@@ ���� �ҽ��� �ϵ� �ڵ� �Ǿ����� ��
	vDest.z					+= ( int )( xRand() % 21 ) - 10;

	const float fRange		= Src.IsFlyingNPC() ? fPatrolRange * 3.0f : fPatrolRange;
	D3DXVECTOR3 vDist		= vDest - vPosBegin;
	vDist.y = 0.0f;
	if( IsInRange( vDist, fRange ) == TRUE )					// 15���� �ݰ��� ����� ���� 
	{
		SetMoveToDest( Src, vDest );
		return;
	}

	SetMoveToDest( Src, vPosBegin );
	return;
}

void FLPatrolBehavior::SetStopMoverTimeOut( CMover & Src, const DWORD tmStopInterval )
{
	m_tmStopMoverTimeOut	= ::GetTickCount() + tmStopInterval;
	Src.SetStop();
}


//----------------------------------------------------------------------------------------------------//



FLRunawayNSelfHealBehavior::FLRunawayNSelfHealBehavior()
{
	Clear();
}

FLRunawayNSelfHealBehavior::~FLRunawayNSelfHealBehavior()
{

}

void FLRunawayNSelfHealBehavior::Clear() /*AIMSG_INIT */
{
	m_bFirstRunaway			= FALSE;
	m_tmStopMoverTimeOut	= 0;
}

void FLRunawayNSelfHealBehavior::StartInit( CMover & Src, CMover & Target )
{
	// ������ �ݴ� �������� ���� ���� 
	// 5�� ���� ������ ���� ����. ���� ũ�� Ŭ���� ������� ���·� �����δ�.
	Src.AddAngle( (float)( 180 + ( 20 - xRandom( 40 ) ) ) );

	const DWORD dwAttackRange	= Target.GetActiveHandItemProp()->dwAttackRange;
	const FLOAT fRange			= Target.GetAttackRange( dwAttackRange ) * 1.7f;
	const FLOAT fAngle			= Src.GetAngle();
	const D3DXVECTOR3 vDst		= Src.GetPos() + VelocityToVec( fAngle, fRange );
	SetMoveToDest( Src, vDst );
	m_bFirstRunaway				= TRUE; // ó�� �����ΰ�?
}

void FLRunawayNSelfHealBehavior::RunawayUpdate( CMover & Src, CMover & Target ) /*, const D3DXVECTOR3 & vPosBegin, const float fPatrolRange ) //MoveProcessRunaway */
{
	//if( m_tmStopMoverTimeOut < tmTick && Src.m_pActMover->IsMove() == FALSE )
	if( m_tmStopMoverTimeOut > ::GetTickCount() || Src.m_pActMover->IsMove() == TRUE )
		return;

	const DWORD dwAttackRange	= Target.GetActiveHandItemProp()->dwAttackRange;
	const FLOAT fRange			= Target.GetAttackRange( dwAttackRange ) * 1.7f;

	if( IsInRange( Target.GetPos() - Src.GetPos(), fRange ) == TRUE )
	{
		// 5�� ���� ������ ���� ����. ���� ũ�� Ŭ���� ������� ���·� �����δ�.
		Src.AddAngle( (float)( 20 - xRandom( 40 ) ) );
		// ������ ���� �Ŀ� ���弱�� �ִ� ��ǥ�� ���Ѵ�.
		const FLOAT fAngle	= Src.GetAngle();
		const D3DXVECTOR3 vDst	= Src.GetPos() + VelocityToVec( fAngle, fRange );
		SetMoveToDest( Src, vDst );
	}

	// ���⼭ �޽� �����ð����� HP�� ä���ش�.
	if( ::GetTickCount() > Src.m_dwTickRecovery )	// .
	{
		Src.m_dwTickRecovery= ::GetTickCount() + NEXT_TICK_RECOVERY;
		Src.IncHitPoint( Src.GetHPRecovery() );
	}
}

void FLRunawayNSelfHealBehavior::SetStopMoverTimeOut( CMover & Src, const DWORD tmStopInterval )
{
	//m_tmStopMoverTimeOut = tmStopInterval;
	m_tmStopMoverTimeOut	= ::GetTickCount() + tmStopInterval;
	Src.SetStop();
}

BOOL FLRunawayNSelfHealBehavior::IsFirstRunaway() const
{
	return m_bFirstRunaway;
}

//----------------------------------------------------------------------------------------------------//


FLAttackBehavior::FLAttackBehavior()
{
	Clear();
}

FLAttackBehavior::~FLAttackBehavior()
{

}

void FLAttackBehavior::Clear()
{
	m_tmAttackPoint			= 0;
	m_tmRageTimeOut			= 0;
}

void FLAttackBehavior::SetAttackPointTime( const DWORD tmAttackPoint )
{
	m_tmAttackPoint			= tmAttackPoint;
}

DWORD FLAttackBehavior::GetAttackPointTime() const
{
	return m_tmAttackPoint;
}

void FLAttackBehavior::SetRageTimeOutTime( const DWORD tmRangTimeOutInterval ) 
{
	m_tmRageTimeOut			= std::max( ::GetTickCount() + tmRangTimeOutInterval, tmRangTimeOutInterval );		// ������ �߰� 
	//m_tmRageTimeOut			= tmRageTimeOut;
}

DWORD FLAttackBehavior::GetRageTimeOutTime() const
{
	return m_tmRageTimeOut;
}

BOOL FLAttackBehavior::AutoChoiceRangeAttack( CMover & Src, CMover & Target )
{
	MoverProp * pProp	= Src.GetProp();


	int	nTargetJob		= 0;
	if( Target.IsPlayer() == TRUE )		nTargetJob = Target.GetJob();				// Ÿ���� ����.
	const BOOL bRange	= ( pProp->m_bRangeAttack[ nTargetJob ] ) ? TRUE : FALSE;	// AI�� �Ÿ��� ������ ���Ÿ� AI����.
	const int nRange	= pProp->m_bRangeAttack[ nTargetJob ] & 0x7F;

	const BOOL bMelee	= ( BOOL )pProp->m_bMeleeAttack;							// AI�� �ٰŸ����ݵ� �ִ���.
	if( bRange && bMelee )															// ���Ÿ�/�ٰŸ� �Ѵ� ������ ����
	{
		const D3DXVECTOR3 vDist	= Target.GetPos() - Src.GetPos();					// Ÿ�ٰ��� �Ÿ�.
		const float fDistSq		= D3DXVec3LengthSq( &vDist );

		const int nHalfRange	= nRange / 2;										// ��Ÿ��� ��.
		if( fDistSq < ( nHalfRange * nHalfRange ) )									// �ʹ� ������ �پ��־� �Ÿ������� �����.
		{
			return FALSE;								// ������������ ����.
		}
		else
		{
			return TRUE;								// �Ÿ��� ������ �����Ƿ� ���Ÿ� ����.
		}
	} 
	else if( bRange == TRUE && bMelee == FALSE )		// ���Ÿ��� ������ ����
	{
		return TRUE;
	}
	else if( bRange == FALSE && bMelee == FALSE )		
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID ATTACK TYPE : None RangeAttack & None MeeleeAttack : MonsterID(%d) ]" ), Src.GetIndex() );
	}

	// �ٰŸ��� ������ ����
	return FALSE;
}

void FLAttackBehavior::SetMoveForRangeAttacking( CMover & Src, CMover & Target )
{
	int nTargetJob		= std::max( Target.GetJob(), 0 );
	if( nTargetJob >= MAX_JOB )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "����ħ�� %d" ), nTargetJob );
		nTargetJob = 0;
	}

	MoverProp * pProp	= Src.GetProp();
	const FLOAT	fRange  = ( FLOAT )( pProp->m_bRangeAttack[ nTargetJob ] & 0x7F );		// ��Ÿ�
	const BOOL bKeepRange = ( pProp->m_bRangeAttack[ nTargetJob ] & 0x80) ? TRUE : FALSE;	// ���� 1��Ʈ�� ��? //�Ÿ����� flag

	const D3DXVECTOR3 vTargetTo = Target.GetPos() - Src.GetPos();
	const FLOAT fDist	= ::D3DXVec3Length( &vTargetTo );		// ������ �Ÿ�.
	if( bKeepRange )
	{
		if( ::fabs( fDist - fRange ) < 1.0f )	// ������ �Ÿ��� ��Ÿ��� ���̰� 1���� �̳��� ���̻� ������ �ʿ� ����.
		{
			if( Src.IsEmptyDest() == FALSE )	// �̵�����.
			{
				SetMoveToDest( Src, Src.GetPos() );	// ������ġ�� �̵� - �������� ��ž�� ���� ���. Ŭ�󿡼� ������ ��ġ�� �̵�.
			}
		} 
		else
		{
			// fRange�� �Ÿ��� �����ϵ��� ...
			if( fDist < fRange )	
			{
				const FLOAT fAngle	= GetDegreeX( Src.GetPos(), Target.GetPos() );	// dest, src�� �Ķ���� ��ġ�� �ٲٸ� �ݴ������ ���´�.
				D3DXVECTOR3 vDst	= Src.GetPos() + VelocityToVec( fAngle, 10 );
				SetMoveToDest( Src, vDst );  // �ݴ�������� �޷��� �Ÿ��� ����.
			} 
			else
			{
				SetMoveToDest( Src, Target.GetId() );	// ��ǥ���� �Ÿ��� ��Ÿ����� �� �Ӵϱ� ������.
			}
		}
	} 
	else	// not keepRange
	{
		if( fDist < fRange )		// ��Ÿ��ȿ� ������ ���ڸ��� ����.
		{
			if( Src.IsEmptyDest() == FALSE )	// �̵�����.
			{
				SetMoveToDest( Src, Src.GetPos() );		// ������ġ�� �̵� - �������� ��ž�� ���� ���. Ŭ�󿡼� ������ ��ġ�� �̵�.
			}
		} 
		else
		{
			SetMoveToDest( Src, Target.GetId() );	// ��Ÿ� ���̸� ��ǥ������ ����.
		}
	}
}

BOOL FLAttackBehavior::TryAttack( CMover & Src, CMover & Target, const BOOL bRangeAttack, const DWORD tmRageTimeoutInterval )
{
	//CMover & Src			= AI.GetMover();
	MoverProp *pProp		= Src.GetProp();
	if( pProp == NULL )
		return FALSE;

	int	nTargetJob			= 0;
	if( Target.IsPlayer() )
		nTargetJob			= std::max( Target.GetJob(), 0 );

	if( nTargetJob >= MAX_JOB )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "����ħ�� %d" ), nTargetJob );
		nTargetJob = 0;
	}

	//const BOOL bRangeAttack = ( pProp->m_bRangeAttack[ nTargetJob ] & 0x7F ) ? TRUE : FALSE;	// ��Ÿ� ���� AI�� �ֳ�.

	//FLAggro::SumFirstTargetAggro( Src, Target.GetId() );

	const DWORD dwAtkMethod		= ( bRangeAttack ) ? GetAtkMethod_Far( Src ) : GetAtkMethod_Near( Src );
	const DWORD dwAtkRange		= GetAtkRange( Src, dwAtkMethod );
	// ��Ÿ� ���� �����̶��?
	if( dwAtkRange == AR_RANGE || dwAtkRange == AR_WAND )
	{
		// ��ȿ �Ÿ� �Ǵ�
		const FLOAT fRange		= ( float )( pProp->m_bRangeAttack[ nTargetJob ] & 0x7F ) + 1.0f;		// ��Ÿ� +1���� ������ �� ���.

		const D3DXVECTOR3 vTargetTo = Src.GetPos() - Target.GetPos();	
		if( IsInRange( vTargetTo, fRange ) == TRUE )		// AI���� ������ ���ݰŸ����� �ٰ����°�?
		{
			DoAttack( Src, Target, dwAtkMethod, tmRageTimeoutInterval );	//@@@@@@@@@@@@@@@@@@@@@@@@ ����		
			m_tmAttackPoint		= ::GetTickCount() + SEC( 3 );		// ��Ÿ� �����̴�. 3�� ���� ������ ����.
		}
	} 
	else
	{
		const FLOAT fRange	= Src.GetAttackRange( dwAtkRange );
		if( Src.IsRangeObj( &Target, fRange ) )			// 3D �Ÿ� üũ?
		{
			DoAttack( Src, Target, dwAtkMethod, tmRageTimeoutInterval );	//@@@@@@@@@@@@@@@@@@@@@@@@ ����	
			//			m_bGoTarget = TRUE;  // <- �̰� ���ϴ°�? 
			// ������ ������ ���� ������ ����.
			const int nDelay	= (int)pProp->dwReAttackDelay - SEC( 1 ) + xRandom( SEC( 2 ) );
			m_tmAttackPoint		= ::GetTickCount() + nDelay;
			FLTRACE_LOG( PROGRAM_NAME, _T( "AtkDelay:%d" ), (int)nDelay );
		}
	}
	// ���������� ARRIVAL���� ó���Ѵ�. ���⼭ ó���ص� ���� ������?

	return TRUE;
}

BOOL FLAttackBehavior::DoAttack( CMover & Src, CMover & Target, const DWORD dwAttackMethod, const DWORD tmRageTimeoutInterval )
{
	//CMover & Src			= AI.GetMover();
	if( IsInvalidObj( &Src ) || dwAttackMethod == 0 )	
		return FALSE;

	const DWORD dwAtkItemID = GetAtkItemId( Src, dwAttackMethod );
	PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( dwAtkItemID );

	if( pItemProp == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s %d %d" ), Src.GetName(), dwAttackMethod, dwAtkItemID );
		return FALSE;
	}

	if( pItemProp->dwAttackRange == AR_RANGE || pItemProp->dwAttackRange == AR_WAND )
	{
		// ��Ÿ� ������, ���õ� ���� ���(dwAtkMethod)�� �ش� ���� ������ �Ӽ����� �Ӽ��� ��� ��Ÿ� �����̶�� �Ǵ���.
		// �������� idSfx�� ���� �ڱ��ڽſ��� ��Ͻ��ѵ�. ���߿� Ŭ�󿡼� �¾Ƽ� ������ ������ �Ѿ���� �װͰ� ���ؼ� �������� �Ѱ���.
		const int idSfxHit	= Src.m_sfxHitArray.Add( 0, Target.GetId(), AF_GENERIC, 0 );	// �̺κ��� DoAttackRange������ ���� �Ұ� ����.
		if( dwAtkItemID == NULL_ID )
			FLERROR_LOG( PROGRAM_NAME, _T( "��� : %s�� dwAtk2�� �������� �ʾҴ�" ), Src.GetName() );

		if( Src.DoAttackRange( &Target, dwAtkItemID, idSfxHit ) != 1 )
			Src.m_sfxHitArray.RemoveSfxHit( idSfxHit, FALSE );	// sfxHitArray.Add�־������� �ٽû�. 
	}
	else
	{
		// �ʻ� ������, ������Ƽ���� ������ ���� �� �����Ƿ� ���� ���(dwAtkMethod)���� ����.
		// �ʻ� ���� ��ƾ�� ���ο��� �ܰŸ� ������ �� ���� �ִ�.
		if( dwAtkItemID == NULL_ID )
			FLERROR_LOG( PROGRAM_NAME, _T( "��� : %s�� ����%d�� �������� �ʾҴ�" ), Src.GetName(), dwAttackMethod );
		if( dwAttackMethod == 3 )
			Src.DoAttackSP( &Target, dwAtkItemID );
		else
		{
			Src.DoAttackMelee( &Target, OBJMSG_ATK1 );
 			//int nError = Src.SendActMsg( OBJMSG_ATK1, Target.GetId(), 0, 0 );
 			//if( nError > 0 ) 
 			//	g_xWSUserManager->AddMeleeAttack( &Src, OBJMSG_ATK1, Target.GetId(), 0, MAKELONG( 0, (WORD)nError ) );
		}
	}

	//m_tmRageTimeOut			= ::GetTickCount() + tmRageTimeoutInterval;		// ���� �� s_tmAttack�ʰ� �����ؿ�.
	m_tmRageTimeOut			= std::max( ::GetTickCount() + tmRageTimeoutInterval, tmRageTimeoutInterval );		// ������ �߰� 
	return TRUE;
}

DWORD FLAttackBehavior::GetAtkRange( CMover & Src, const DWORD dwAtkMethod )
{
	PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( GetAtkItemId( Src, dwAtkMethod ) );
#ifdef _DEBUG
	if( pItemProp == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "ItemProp->dwAtk%d�� NULL�̴�. (%s)" ), dwAtkMethod, Src.GetName() );
	}
#endif
	return pItemProp->dwAttackRange;
}

DWORD FLAttackBehavior::GetAtkMethod_Near( CMover & Src )
{
//	MoverProp* pMoverProp = Src.GetProp();
	const DWORD dwRandom = xRandom( 100 );
	const int nDeathblow = (int)( Src.GetHitPointPercent() / 4.5f );
	if( nDeathblow )
	{
		if( !xRandom( nDeathblow ) )
			return 3;
	}

	switch( Src.m_dwBelligerence )
	{
	case BELLI_MELEE2X:
		if( dwRandom < 60 )
			return 1;
		return 2;
		break;
	case BELLI_MELEE:
		if( dwRandom < 80 )
			return 1;
		return 2;
		break;
	case BELLI_RANGE:
		if( dwRandom < 50 )
			return 1;
		return 2;
		break;
	}
	return 1;
}

DWORD FLAttackBehavior::GetAtkMethod_Far( CMover & Src )
{
//	MoverProp* pMoverProp = Src.GetProp();
	const DWORD dwRandom = xRandom( 100 );

	// �ʻ� ���� ����!
	const int nDeathblow = (int)( Src.GetHitPointPercent() / 4.5f );
	if( nDeathblow )
	{
		if( !xRandom( nDeathblow ) )
			return 3;
	}

	switch( Src.m_dwBelligerence )
	{
	case BELLI_MELEE2X:
		if( dwRandom < 50 )
			return 1;
		return 2;
		break;
	case BELLI_MELEE:
		if( dwRandom < 30 )
			return 1;
		return 2;
		break;
	case BELLI_RANGE:
		if( dwRandom < 20 )
			return 1;
		return 2;
	}
	return 1;
}

DWORD FLAttackBehavior::GetAtkItemId( CMover & Src, const DWORD dwAtkMethod )
{
	switch( dwAtkMethod )
	{ 
	case 1:
		return Src.GetProp()->dwAtk1;
	case 2:
		return Src.GetProp()->dwAtk2;
	case 3:
		return Src.GetProp()->dwAtk3;
	}
	return 0;
}

//----------------------------------------------------------------------------------------------------//

FLSummonMonsterBehavior::FLSummonMonsterBehavior()
{
	Clear();
}

FLSummonMonsterBehavior::~FLSummonMonsterBehavior()
{
}

void FLSummonMonsterBehavior::Clear()
{
	m_tmSummonPoint			= ::GetTickCount() + SEC( 20 );
}

void FLSummonMonsterBehavior::TrySummonUpdate( CMover & Src )
{
	if( ::GetTickCount() < m_tmSummonPoint )
		return;

	MoverProp * pProp		= Src.GetProp();
	if( pProp == NULL )
		return;

	m_tmSummonPoint			= ::GetTickCount() + SEC( 20 );

	for( SummonVector::iterator it = m_vecCalledObjID.begin(); it != m_vecCalledObjID.end(); )		//m_vecIdSummon
	{
		CMover * pZako		= prj.GetMover( *it );
		if( IsValidObj( pZako ) && pZako->IsLive() )
		{
			++it;
		}
		else
		{
			it				= m_vecCalledObjID.erase( it );			//@@@@@@@@ ���� �ؾ��ϳ�?
		}
	}

	if( pProp->m_nSummNum <= ( int )m_vecCalledObjID.size() )
		return;

	if( (int)( xRandom(100) ) < pProp->m_nSummProb )	// ��ȯ �õ�.
	{
		CObj * pObj			= CreateObj( D3DDEVICE, OT_MOVER, (DWORD)pProp->m_nSummID );
		if( NULL == pObj )	
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "��ȯ ���� %d" ), pProp->m_nSummID );
			return;
		}

		D3DXVECTOR3 vLocal;
		vLocal.x			= 1.5f - xRandomF( 3.0f );		// ���� ������ �������� ����.
		vLocal.y			= 0;
		vLocal.z			= 1.5f - xRandomF( 3.0f );

		D3DXVECTOR3 vPos	= Src.GetPos();
		vPos				+= vLocal;

		pObj->SetPos( vPos );
		pObj->InitMotion( MTI_STAND );
		pObj->UpdateLocalMatrix();
		( ( CMover * )pObj )->m_bActiveAttack = TRUE;	// ���ϴ� ���� ������ ��ȯ����.
		( ( CMover * )pObj )->AddItToGlobalId();
		if( Src.GetWorld()->ADDOBJ( pObj, FALSE, Src.GetLayer() ) == TRUE )
		{
			m_vecCalledObjID.push_back( ( ( CMover * )pObj )->GetId() );
			g_xWSUserManager->AddDlgEmoticon( &Src, DLGEMOT_SUMMON );
		}
		else
		{
			SAFE_DELETE( pObj );
		}
	}
}

//----------------------------------------------------------------------------------------------------//




FLCallHelperBehavior::FLCallHelperBehavior()
{
	Clear();
}

FLCallHelperBehavior::~FLCallHelperBehavior()
{

}

void FLCallHelperBehavior::Clear()
{
	m_tmHelpPoint		= 0;
	m_bStopCallHelper	= FALSE;
}

void FLCallHelperBehavior::TryCallHelper( CMover & Src, CMover & Target )
{
	if( IsValidObj( &Src ) == FALSE || Src.IsDie() )
		return;
	if( IsValidObj( &Target ) == FALSE || Target.IsDie() )
		return;

	MoverProp * pProp	= Src.GetProp();
	if( pProp == NULL )
		return;

	if( m_tmHelpPoint > ::GetTickCount() + pProp->m_tmUnitHelp )
		return;

	if( m_bStopCallHelper == TRUE )
		return;

	// ���� ���		
	int nRange				= 20;
	switch( pProp->m_nHelpRangeMul )
	{
	case 0:		nRange		= pProp->m_nAttackFirstRange / 2;	break;
	default:	nRange		= pProp->m_nAttackFirstRange * pProp->m_nHelpRangeMul;	break;	// �þ߹����� ���.
	}


	// ���ʹ޶�� �� ���� ����.
	CObj* pObj;

	const size_t MAX_ARRAY			= 40;
	CMover* apMonster[ MAX_ARRAY ][ 5 ];
	int anCountMonster[ 5 ]			= { 0 };
	const D3DXVECTOR3 vCenterPos	= Src.GetPos();

	FOR_LINKMAP( Src.GetWorld(), vCenterPos, pObj, nRange / MPU, CObj::linkDynamic, Src.GetLayer() )
	{
		if( pObj != static_cast< CObj * >( &Src ) && pObj->GetType() == OT_MOVER )
		{
			// FOR_LINKMAP ��ü�� ������ ������ �� ������ ���ʿ� �־���.
			for( int i = 0; i < pProp->m_nCallHelperMax; i++ )
			{
				//int nCallHelperIdx = Src.GetProp()->m_nCallHelperIdx[ i ];
				const int nCallHelperNum = Src.GetProp()->m_nCallHelperNum[ i ];
				// �ƹ��� �θ��°��̰ų� || ���� ������ �θ��� �ɼ��϶� ���� �ε����ΰ�?
				// �׸���.............
				if( ( pProp->m_bHelpWho == 1 || (pProp->m_bHelpWho == 2 && pObj->GetIndex() == Src.GetIndex() ) ) 
					&& ( anCountMonster[ i ] < nCallHelperNum || nCallHelperNum == 0 )	)	
				{
					if( pObj->m_pFSM != NULL )
					{
						apMonster[ anCountMonster[ i ] ][ i ] = (CMover*)pObj;
						anCountMonster[ i ]++;
						if( anCountMonster[ i ] >= MAX_ARRAY ) 
							goto NEXT;
					}
				}
			}
		}
	}
	END_LINKMAP

NEXT:
	// ��Ƽ�� ���� ����.
	int nCountPlayer				= 0;
	CMover* apPlayer[ MAX_ARRAY ]	= { 0 };
	BOOL bParty						= FALSE;

	CParty * pParty					= NULL;
	// ���� ��Ƽ ���� ? ���� ���� 
	for( int i = 0; i < pProp->m_nCallHelperMax; i++ )
		bParty |= pProp->m_bCallHelperParty[ i ] ? TRUE : FALSE;

	if( bParty && IsValidObj( &Target ) )
	{
		pParty = g_PartyMng.GetParty( Target.m_idparty );
		// ��Ƽ�� Ȯ���ϴٸ� ��Ƽ���� apPlayer�� �����Ѵ�.
		if( pParty )
		{
			FOR_LINKMAP( Src.GetWorld(), vCenterPos, pObj, 20 / MPU, CObj::linkPlayer, Src.GetLayer() )
			{
				if( pObj->GetType() == OT_MOVER && ((CMover*)pObj)->IsPlayer() && pParty->IsMember( ((CMover*)pObj)->m_idPlayer ) )
				{
					apPlayer[ nCountPlayer++ ] = (CMover*)pObj;
					if( nCountPlayer >= MAX_ARRAY )
						goto NEXT2;
				}
			}
			END_LINKMAP
		}
	}

NEXT2:
	for( int i = 0; i < pProp->m_nCallHelperMax; i++ )
	{
		if( pParty )
		{
			if( nCountPlayer )
			{
				for( int j = 0; j < anCountMonster[ i ]; j++ )
				{
					// ������ ���Ϳ��� Ÿ���� �������� �����Ѵ�. 
					CMover* pNewTarget = (CMover*)apPlayer[ rand() % nCountPlayer ];
					if( pNewTarget )
						apMonster[ j ][ i ]->PostAIMsg( AIMSG_REQUEST_HELP, pNewTarget->GetId(), 0 );		//RAGE ���·� ����
				}
			}
		}
		else
		{ 
			for( int j = 0; j < anCountMonster[ i ]; j++ )
				apMonster[ j ][ i ]->PostAIMsg( AIMSG_REQUEST_HELP, Target.GetId(), 0 );					//RAGE ���·� ����
		}
	}


	g_xWSUserManager->AddDlgEmoticon( &Src, DLGEMOT_HELPER );

	m_tmHelpPoint		= ::GetTickCount() + pProp->m_tmUnitHelp;
	if( pProp->m_tmUnitHelp == 0 )		// 0�̸� �ѹ��� �θ��� �Ⱥθ�.
		m_bStopCallHelper = TRUE;
}

//-------------------------------------------------------------------------------------------------------------------------------//


void FLSetRandomPos( CMover & Src )
{
	D3DXVECTOR3 vDest		= Src.GetPos();
	vDest.x					+= ( int )( xRand() % 21 ) - 10;		//@@@@ ���� �ҽ��� �ϵ� �ڵ� �Ǿ����� ��
	vDest.z					+= ( int )( xRand() % 21 ) - 10;

	SetMoveToDest( Src, vDest );
}


//-------------------------------------------------------------------------------------------------//

BOOL				
IsInRange( const D3DXVECTOR3 & vDist, const float fRange )
{
	D3DXVECTOR3 dist	= vDist;
	dist.y				= 0.0f;
	const float fDist	= ::D3DXVec3LengthSq( & dist );
	return ( fDist > fRange * fRange ) ? FALSE : TRUE;
}

void				
SetMoveToDest( CMover & Src, const D3DXVECTOR3 & vDest )		//TRUE : ����
{
	//CMover::ProcessMover()���� ó���� 
	Src.SetDestPos( vDest );
	g_xWSUserManager->AddSetDestPos( &Src, vDest, 1 );
}

void				
SetMoveToDest( CMover & Src, const OBJID dwOBJID )
{
	if( Src.GetDestId() == dwOBJID )
		return;

	Src.SetSpeedFactor( 2.0F );

	//CMover * pTarget	= prj.GetMover( dwOBJID );
	//if( IsValidObj( pTarget ) == TRUE )
	//	SetTaget( pTarget->GetId(), pTarget->m_idparty );


	Src.SetDestObj( dwOBJID, 0.0f, TRUE );

//	g_xWSUserManager->AddMoverSetDestObj( &Src, dwOBJID );
}

void				
SetReturnToBegin( CMover & Src, const D3DXVECTOR3 & vPosBegin )
{
	//Src.SetTarget( NULL_ID, 0 );
	//Src.AIInit();

	Src.SetSpeedFactor( 2.66F );
	SetMoveToDest( Src, vPosBegin );
}

void				
SetMaxHP( CMover & Src )
{
	Src.SetPointParam( DST_HP, Src.GetMaxHitPoint() );
	//Src.RemoveAllEnemies(); //�ڱ� ���� ����Ʈ
}










//CMover * TryScanValidTargetByAggroFSM( FLAggro & AggroMng )
//{
//	//	if( AggroMng.GetOwnerObjID() == NULL_ID )
//	//		return NULL;
//
//	CMover * pAggroOwner		= prj.GetMover( AggroMng.GetOwnerObjID() );
//	if( IsValidObj( pAggroOwner ) == FALSE )
//		return NULL;
//
//	OBJID aggroOBJID;
//	if( AggroMng.GetMaxAggro( aggroOBJID ) == TRUE )
//	{
//		CMover * pAggroTarget	= prj.GetMover( aggroOBJID );
//		if( IsValidObj( (CObj*)pAggroTarget ) == FALSE ||
//			pAggroTarget->GetWorld()->GetID() != pAggroOwner->GetWorld()->GetID() ||
//			pAggroTarget->IsDie() == TRUE ||
//			pAggroTarget->HasBuffByIk3( IK3_TEXT_DISGUISE ) == TRUE ||										// ���� ���̶�� 
//			pAggroTarget->HasBuff( BUFF_SKILL, SKILL_INDEX( 193, SI_ACR_SUP_DARKILLUSION ) ) == TRUE ||		// ���� ���̶��
//			pAggroTarget->IsRegionAttr( RA_SAFETY ) == TRUE ||												// ���� �����̶��
//			pAggroTarget->IsFly() != pAggroOwner->IsFlyingNPC()												// ������ ���� �ִٸ�.
//			)		
//		{
//			AggroMng.RemoveMover( /**pAggroOwner,*/ aggroOBJID );
//		}
//		else
//		{
//			return pAggroTarget;
//		}
//	}
//
//	return NULL;
//}


BOOL	FLScanTarget::IsValidTarget( CMover & AIMover, CMover & Target, const FLAIOptionalFlags & kOptionalFlags )
{
	if( IsValidObj( &Target ) == FALSE )
		return FALSE;

	if( Target.GetWorld()->GetID() != AIMover.GetWorld()->GetID() )
		return FALSE;

	if( Target.IsDie() == TRUE )
		return FALSE;

	// ����
	//if( kOptionalFlags.bIgnoreMatchless == FALSE )
	//{
	//	if( Target.m_dwMode & ( MATCHLESS_MODE | MATCHLESS2_MODE ) )
	//		return FALSE;
	//}

	// ����
	if( kOptionalFlags.bIgnoreTransparent == FALSE )
	{
		if( Target.HasBuff( BUFF_SKILL, SKILL_INDEX( 193, SI_ACR_SUP_DARKILLUSION ) ) == TRUE )
			return FALSE;

		if( Target.m_dwMode & TRANSPARENT_MODE )
			return FALSE;
	}

	if( Target.IsFly() == TRUE )
		return FALSE;

	if( Target.HasBuffByIk3( IK3_TEXT_DISGUISE ) == TRUE )										// ���� ���̶��
		return FALSE;

	if(	Target.IsRegionAttr( RA_SAFETY ) == TRUE )											// ���� �����̶��
		return FALSE;

	return TRUE;
}

CMover * FLScanTarget::Stalking( CMover & AIMover, const OBJID objid, const FLAIOptionalFlags & kOptionalFlags )
{
	CMover * pTarget		= prj.GetMover( objid );
	if( IsValidObj( pTarget ) == FALSE )
		return NULL;

	if( IsValidTarget( AIMover, *pTarget, kOptionalFlags ) == FALSE )
		return NULL;

	return pTarget;
}

//CMover *				
//ScanTargetByNear( CMover & Behavior, CObj & CenterObj, const int nRangeMeter/*, const E_AI_TARGET_TYPE eTargetType*/ )
//{
//	const D3DXVECTOR3 vCenter	= CenterObj.GetPos();
//
//	CObj * pObj;
//	CMover * pTarget		= NULL;
//	const float fRadius		= ( float )( nRangeMeter * nRangeMeter );
//	D3DXVECTOR3 vDist;
//
//	FOR_LINKMAP( CenterObj.GetWorld(), vCenter, pObj, nRangeMeter, CObj::linkPlayer, Behavior.GetLayer() )
//	{
//		FLASSERT( pObj->GetType() == OT_MOVER && static_cast< CMover * >( pObj )->IsPlayer() );
//		pTarget				= static_cast< CMover * >( pObj );
//		vDist			= pTarget->GetPos() - vCenter;
//		if( D3DXVec3LengthSq( & vDist ) < fRadius )
//		{
//			if( IsValidObj( (CObj*)pTarget ) == TRUE &&
//				pTarget->IsDie() == FALSE &&
//				pTarget->HasBuffByIk3( IK3_TEXT_DISGUISE ) == FALSE &&										// ���� ���̶�� 
//				pTarget->HasBuff( BUFF_SKILL, SKILL_INDEX( 193, SI_ACR_SUP_DARKILLUSION ) ) == FALSE &&		// ���� ���̶��
//				pTarget->IsRegionAttr( RA_SAFETY ) == FALSE &&// ���� �����̶��
//				pTarget->IsFly() == Behavior.IsFlyingNPC() &&
//				pTarget->IsMode( TRANSPARENT_MODE ) == FALSE )	//����ȭ ����?
//			{
//				return pTarget;
//			}
//		}
//	}
//	END_LINKMAP
//		return NULL;
//}

CMover * FLScanTarget::ByNear( CMover & Behavior, CObj & CenterObj, const int nRangeMeter, const FLAIOptionalFlags & kOptionalFlags  )
{
	const D3DXVECTOR3 vCenter	= CenterObj.GetPos();

	CObj * pObj;
	CMover * pTarget		= NULL;
	const float fRadius		= ( float )( nRangeMeter * nRangeMeter );
	D3DXVECTOR3 vDist;

	FOR_LINKMAP( CenterObj.GetWorld(), vCenter, pObj, nRangeMeter, CObj::linkPlayer, Behavior.GetLayer() )
	{
		FLASSERT( pObj->GetType() == OT_MOVER && static_cast< CMover * >( pObj )->IsPlayer() );
		pTarget				= static_cast< CMover * >( pObj );
		vDist			= pTarget->GetPos() - vCenter;
		if( D3DXVec3LengthSq( & vDist ) < fRadius )
		{
			if( IsValidObj( pTarget ) == TRUE 
				|| IsValidTarget( Behavior, *pTarget, kOptionalFlags ) == TRUE )
			{
				return pTarget;
			}
		}
	}
	END_LINKMAP
		return NULL;
}






CMover * FLScanTarget::ByAggro( CMover & AIMover, const FLAIOptionalFlags & kOptionalFlags )
{
	if( AIMover.GetAggroMng() == NULL )
		return NULL;

	OBJID aggroOBJID;
	if( AIMover.GetAggroMng()->GetMaxAggro( aggroOBJID ) == TRUE )
	{
		CMover * pAggroTarget	= prj.GetMover( aggroOBJID );
		if( IsValidObj( (CObj*)pAggroTarget ) == FALSE 
			|| IsValidTarget( AIMover, *pAggroTarget, kOptionalFlags ) == FALSE )
		{
			AIMover.GetAggroMng()->RemoveMover( aggroOBJID );
		}
		else
		{
			return pAggroTarget;
		}
	}

	return NULL;
}

//CMover * TryScanValidTargetByAggro( CMover & AggroOwner )
//{
//	//��׷� ��ü�� ã�� 
//	if( AggroOwner.m_pAIInterface == NULL || AggroOwner.m_pAIInterface->GetAggroMng() == NULL )
//		return NULL;
//
//	FLAggro & AggroMng		= * AggroOwner.m_pAIInterface->GetAggroMng(); 
//
//
//	OBJID aggroOBJID;
//	if( AggroMng.GetMaxAggro( aggroOBJID ) == TRUE )
//	{
//		CMover * pAggroTarget	= prj.GetMover( aggroOBJID );
//		if( IsValidObj( (CObj*)pAggroTarget ) == FALSE ||
//			pAggroTarget->GetWorld()->GetID() != AggroOwner.GetWorld()->GetID() ||
//			pAggroTarget->IsDie() == TRUE ||
//			pAggroTarget->HasBuffByIk3( IK3_TEXT_DISGUISE ) == TRUE ||										// ���� ���̶�� 
//			pAggroTarget->HasBuff( BUFF_SKILL, SKILL_INDEX( 193, SI_ACR_SUP_DARKILLUSION ) ) == TRUE ||		// ���� ���̶��
//			pAggroTarget->IsRegionAttr( RA_SAFETY ) == TRUE ||												// ���� �����̶��
//			pAggroTarget->IsFly() != AggroOwner.IsFlyingNPC()													// ������ ���� �ִٸ�.
//			)		
//		{
//			AggroMng.RemoveMover( /*AggroOwner,*/ aggroOBJID );
//		}
//		else
//		{
//			return pAggroTarget;
//		}
//	}
//
//	return NULL;
//}


//CMover * TryScanValidTargetByAggroFSM( const FLAIOptionalFlags & kOptionalFlags, FLAggro & AggroMng )
//{
//	CMover * pAggroOwner	= prj.GetMover( AggroMng.GetOwnerObjID() );
//	if( IsValidObj( pAggroOwner ) == FALSE )
//		return NULL;
//
//	OBJID aggroOBJID;
//	if( AggroMng.GetMaxAggro( aggroOBJID ) == TRUE )
//	{
//		CMover * pAggroTarget	= prj.GetMover( aggroOBJID );
//		if( IsValidObj( (CObj*)pAggroTarget ) == FALSE 
//			|| IsValidTarget( kOptionalFlags, *pAggroOwner, *pAggroTarget ) == FALSE )
//		{
//			AggroMng.RemoveMover( /**pAggroOwner,*/ aggroOBJID );
//		}
//		else
//		{
//			return pAggroTarget;
//		}
//	}
//
//	return NULL;
//}

//BOOL					AutoChoiceRangeAttack( CMover & Src, CMover & Target )			//SelectAttackType
//{
//	MoverProp * pProp	= Src.GetProp();
//	
//
//	int	nTargetJob		= 0;
//	if( Target.IsPlayer() == TRUE )		nTargetJob = Target.GetJob();				// Ÿ���� ����.
//	const BOOL bRange	= ( pProp->m_bRangeAttack[ nTargetJob ] ) ? TRUE : FALSE;	// AI�� �Ÿ��� ������ ���Ÿ� AI����.
//	const int nRange	= pProp->m_bRangeAttack[ nTargetJob ] & 0x7F;
//
//	const BOOL bMelee	= ( BOOL )pProp->m_bMeleeAttack;							// AI�� �ٰŸ����ݵ� �ִ���.
//	if( bRange && bMelee )															// ���Ÿ�/�ٰŸ� �Ѵ� ������ ����
//	{
//		const D3DXVECTOR3 vDist	= Target.GetPos() - Src.GetPos();					// Ÿ�ٰ��� �Ÿ�.
//		const float fDistSq		= D3DXVec3LengthSq( &vDist );
//
//		const int nHalfRange	= nRange / 2;										// ��Ÿ��� ��.
//		if( fDistSq < ( nHalfRange * nHalfRange ) )									// �ʹ� ������ �پ��־� �Ÿ������� �����.
//		{
//			return FALSE;								// ������������ ����.
//		}
//		else
//		{
//			return TRUE;								// �Ÿ��� ������ �����Ƿ� ���Ÿ� ����.
//		}
//	} 
//	else if( bRange == TRUE && bMelee == FALSE )		// ���Ÿ��� ������ ����
//	{
//		return TRUE;
//	}
//	else if( bRange == FALSE && bMelee == FALSE )		// ���Ÿ��� ������ ����
//	{
//		FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID ATTACK TYPE : None RangeAttack & None MeeleeAttack : MonsterID(%d) ]" ), Src.GetIndex() );
//	}
//
//	// �ٰŸ��� ������ ����
//	return FALSE;
//}

//void						SetMoveForRangeAttacking( CMover & Src, CMover & Target )
//{
//	int nTargetJob		= std::max( Target.GetJob(), 0 );
//	if( nTargetJob >= MAX_JOB )
//	{
//		FLERROR_LOG( PROGRAM_NAME, _T( "����ħ�� %d" ), nTargetJob );
//		nTargetJob = 0;
//	}
//
//	MoverProp * pProp	= Src.GetProp();
//	const FLOAT	fRange  = ( FLOAT )( pProp->m_bRangeAttack[ nTargetJob ] & 0x7F );		// ��Ÿ�
//	const BOOL bKeepRange = ( pProp->m_bRangeAttack[ nTargetJob ] & 0x80) ? TRUE : FALSE;	// ���� 1��Ʈ�� ��? //�Ÿ����� flag
//
//	const D3DXVECTOR3 vTargetTo = Target.GetPos() - Src.GetPos();
//	const FLOAT fDist	= ::D3DXVec3Length( &vTargetTo );		// ������ �Ÿ�.
//	if( bKeepRange )
//	{
//		if( ::fabs( fDist - fRange ) < 1.0f )	// ������ �Ÿ��� ��Ÿ��� ���̰� 1���� �̳��� ���̻� ������ �ʿ� ����.
//		{
//			if( Src.IsEmptyDest() == FALSE )	// �̵�����.
//				SetMoveToDest( Src, Src.GetPos() );	// ������ġ�� �̵� - �������� ��ž�� ���� ���. Ŭ�󿡼� ������ ��ġ�� �̵�.
//		} 
//		else
//		{
//			// fRange�� �Ÿ��� �����ϵ��� ...
//			if( fDist < fRange )	
//			{
//				const FLOAT fAngle	= GetDegreeX( Src.GetPos(), Target.GetPos() );	// dest, src�� �Ķ���� ��ġ�� �ٲٸ� �ݴ������ ���´�.
//				D3DXVECTOR3 vDst	= Src.GetPos() + VelocityToVec( fAngle, 10 );
//				SetMoveToDest( Src, vDst );  // �ݴ�������� �޷��� �Ÿ��� ����.
//			} 
//			else
//				SetMoveToDest( Src, Target.GetId() );	// ��ǥ���� �Ÿ��� ��Ÿ����� �� �Ӵϱ� ������.
//		}
//	} 
//	else	// not keepRange
//	{
//		if( fDist < fRange )		// ��Ÿ��ȿ� ������ ���ڸ��� ����.
//		{
//			if( Src.IsEmptyDest() == FALSE )	// �̵�����.
//				SetMoveToDest( Src, Src.GetPos() );		// ������ġ�� �̵� - �������� ��ž�� ���� ���. Ŭ�󿡼� ������ ��ġ�� �̵�.
//		} else
//			SetMoveToDest( Src, Target.GetId() );	// ��Ÿ� ���̸� ��ǥ������ ����.
//	}
//}

BOOL					RecvConditionUpdate( CMover & Src )
{
	MoverProp * pProp	= Src.GetProp();
	if( pProp == NULL )
		return FALSE;

	if( pProp->m_bRecvCond == FALSE )
		return FALSE;

	if( Src.GetManaPoint() < 0 )
		return FALSE;


	if( Src.GetHitPointPercent( 100 ) <= pProp->m_nRecvCondMe )	// HP�� m_nRecvCondMe% ���Ϸ� �������°�?
	{
		// m_nHow% ��ŭ�� HP�� ȸ��
		const int nHP		= ( int )( Src.GetMaxHitPoint() * ( pProp->m_nRecvCondHow / 100.0f ) );	
		Src.IncHitPoint( nHP );

		g_xWSUserManager->AddCreateSfxObj( &Src, XI_INDEX( 62, XI_GEN_CURE01 ), Src.GetPos().x, Src.GetPos().y, Src.GetPos().z );	

		if( pProp->m_nRecvCondMP > 0 )
		{
			const int nMP	= ( pProp->m_nRecvCondMP > 0 ) ? pProp->m_nRecvCondMP * -1 : Src.GetMaxManaPoint();	// 0���� �����Ǿ� ������ ���� �Ҹ�.		
			Src.IncManaPoint( nMP );
		}
		return TRUE;
	}
	return FALSE;
}



//#undef max
//BOOL		TryAttack( CMover & Src, CMover & Target, const BOOL bRangeAttack, const DWORD tmRageTimeout )	//SubAttackChance
//{
//	//CMover & Src			= AI.GetMover();
//	MoverProp *pProp		= Src.GetProp();
//	if( pProp == NULL )
//		return FALSE;
//
//	int	nTargetJob			= 0;
//	if( Target.IsPlayer() )
//		nTargetJob			= std::max( Target.GetJob(), 0 );
//
//	if( nTargetJob >= MAX_JOB )
//	{
//		FLERROR_LOG( PROGRAM_NAME, _T( "����ħ�� %d" ), nTargetJob );
//		nTargetJob = 0;
//	}
//
//	//const BOOL bRangeAttack = ( pProp->m_bRangeAttack[ nTargetJob ] & 0x7F ) ? TRUE : FALSE;	// ��Ÿ� ���� AI�� �ֳ�.
//
//	FLAggro::SumFirstTargetAggro( Src, Target.GetId() );
//
//	const DWORD dwAtkMethod		= ( bRangeAttack ) ? GetAtkMethod_Far( Src ) : GetAtkMethod_Near( Src );
//	const DWORD dwAtkRange		= GetAtkRange( Src, dwAtkMethod );
//	// ��Ÿ� ���� �����̶��?
//	if( dwAtkRange == AR_RANGE || dwAtkRange == AR_WAND )
//	{
//		// ��ȿ �Ÿ� �Ǵ�
//		const FLOAT fRange		= ( float )( pProp->m_bRangeAttack[ nTargetJob ] & 0x7F ) + 1.0f;		// ��Ÿ� +1���� ������ �� ���.
//
//		const D3DXVECTOR3 vTargetTo = Src.GetPos() - Target.GetPos();	
//		if( IsInRange( vTargetTo, fRange ) == TRUE )		// AI���� ������ ���ݰŸ����� �ٰ����°�?
//		{
//			DoAttack( Src, Target, dwAtkMethod, tmRageTimeout );	//@@@@@@@@@@@@@@@@@@@@@@@@ ����		
//			Src.m_AIMonsterData.tmAttackPoint	= ::GetTickCount() + SEC( 3 );		// ��Ÿ� �����̴�. 3�� ���� ������ ����.
//		}
//	} 
//	else
//	{
//		const FLOAT fRange	= Src.GetAttackRange( dwAtkRange );
//		if( Src.IsRangeObj( &Target, fRange ) )			// 3D �Ÿ� üũ?
//		{
//			DoAttack( Src, Target, dwAtkMethod, tmRageTimeout );	//@@@@@@@@@@@@@@@@@@@@@@@@ ����	
////			m_bGoTarget = TRUE;  // <- �̰� ���ϴ°�? 
//			// ������ ������ ���� ������ ����.
//			const int nDelay	= (int)pProp->dwReAttackDelay - SEC( 1 ) + xRandom( SEC( 2 ) );
//			Src.m_AIMonsterData.tmAttackPoint	= ::GetTickCount() + nDelay;
//			FLTRACE_LOG( PROGRAM_NAME, _T( "AtkDelay:%d" ), (int)nDelay );
//		}
//	}
//	// ���������� ARRIVAL���� ó���Ѵ�. ���⼭ ó���ص� ���� ������?
//
//	return TRUE;
//}

//BOOL						DoAttack( CMover & Src, CMover & Target, const DWORD dwAttackMethod, const DWORD tmRageTimeout )
//{
//	//CMover & Src			= AI.GetMover();
//	if( IsInvalidObj( &Src ) || dwAttackMethod == 0 )	
//		return FALSE;
//
//	const DWORD dwAtkItemID = GetAtkItemId( Src, dwAttackMethod );
//	ItemProp* pItemProp		= prj.GetItemProp( dwAtkItemID );
//	if( pItemProp == NULL )
//	{
//		FLERROR_LOG( PROGRAM_NAME, _T( "%s %d %d" ), Src.GetName(), dwAttackMethod, dwAtkItemID );
//		return FALSE;
//	}
//
//	if( pItemProp->dwAttackRange == AR_RANGE || pItemProp->dwAttackRange == AR_WAND )
//	{
//		// ��Ÿ� ������, ���õ� ���� ���(dwAtkMethod)�� �ش� ���� ������ �Ӽ����� �Ӽ��� ��� ��Ÿ� �����̶�� �Ǵ���.
//		// �������� idSfx�� ���� �ڱ��ڽſ��� ��Ͻ��ѵ�. ���߿� Ŭ�󿡼� �¾Ƽ� ������ ������ �Ѿ���� �װͰ� ���ؼ� �������� �Ѱ���.
//		const int idSfxHit	= Src.m_sfxHitArray.Add( 0, Target.GetId(), AF_GENERIC, 0 );	// �̺κ��� DoAttackRange������ ���� �Ұ� ����.
//		if( dwAtkItemID == NULL_ID )
//			FLERROR_LOG( PROGRAM_NAME, _T( "��� : %s�� dwAtk2�� �������� �ʾҴ�" ), Src.GetName() );
//
//		if( Src.DoAttackRange( &Target, dwAtkItemID, idSfxHit ) != 1 )
//			Src.m_sfxHitArray.RemoveSfxHit( idSfxHit, FALSE );	// sfxHitArray.Add�־������� �ٽû�. 
//	}
//	else
//	{
//		// �ʻ� ������, ������Ƽ���� ������ ���� �� �����Ƿ� ���� ���(dwAtkMethod)���� ����.
//		// �ʻ� ���� ��ƾ�� ���ο��� �ܰŸ� ������ �� ���� �ִ�.
//		if( dwAtkItemID == NULL_ID )
//			FLERROR_LOG( PROGRAM_NAME, _T( "��� : %s�� ����%d�� �������� �ʾҴ�" ), Src.GetName(), dwAttackMethod );
//		if( dwAttackMethod == 3 )
//			Src.DoAttackSP( &Target, dwAtkItemID );
//		else
//		{
//			int nError = Src.SendActMsg( OBJMSG_ATK1, Target.GetId(), 0, 0 );
//			if( nError > 0 ) 
//				g_xWSUserManager->AddMeleeAttack( &Src, OBJMSG_ATK1, Target.GetId(), 0, MAKELONG( 0, (WORD)nError ) );
//		}
//	}
//
//	Src.m_AIMonsterData.tmRageTimeOut			= ::GetTickCount() + tmRageTimeout;		// ���� �� s_tmAttack�ʰ� �����ؿ�.
//	return TRUE;
//}


//DWORD					GetAtkMethod_Near( CMover & Src )
//{
//	MoverProp* pMoverProp = Src.GetProp();
//	const DWORD dwRandom = xRandom( 100 );
//	const int nDeathblow = (int)( Src.GetHitPointPercent() / 4.5f );
//	if( nDeathblow )
//	{
//		if( !xRandom( nDeathblow ) )
//			return 3;
//	}
//
//	switch( Src.m_dwBelligerence )
//	{
//	case BELLI_MELEE2X:
//		if( dwRandom < 60 )
//			return 1;
//		return 2;
//		break;
//	case BELLI_MELEE:
//		if( dwRandom < 80 )
//			return 1;
//		return 2;
//		break;
//	case BELLI_RANGE:
//		if( dwRandom < 50 )
//			return 1;
//		return 2;
//		break;
//	}
//	return 1;
//}


//DWORD				GetAtkMethod_Far( CMover & Src )
//{
//	MoverProp* pMoverProp = Src.GetProp();
//	const DWORD dwRandom = xRandom( 100 );
//
//	// �ʻ� ���� ����!
//	const int nDeathblow = (int)( Src.GetHitPointPercent() / 4.5f );
//	if( nDeathblow )
//	{
//		if( !xRandom( nDeathblow ) )
//			return 3;
//	}
//
//	switch( Src.m_dwBelligerence )
//	{
//	case BELLI_MELEE2X:
//		if( dwRandom < 50 )
//			return 1;
//		return 2;
//		break;
//	case BELLI_MELEE:
//		if( dwRandom < 30 )
//			return 1;
//		return 2;
//		break;
//	case BELLI_RANGE:
//		if( dwRandom < 20 )
//			return 1;
//		return 2;
//	}
//	return 1;
//}
//
//DWORD					GetAtkRange( CMover & Src, const DWORD dwAtkMethod )
//{
//	//ItemProp* pItemProp = GetAtkProp( dwAtkMethod );
//	ItemProp * pItemProp	= prj.GetItemProp( GetAtkItemId( Src, dwAtkMethod ) );
//#ifdef _DEBUG
//	if( pItemProp == NULL )
//	{
//		FLERROR_LOG( PROGRAM_NAME, _T( "ItemProp->dwAtk%d�� NULL�̴�. (%s)" ), dwAtkMethod, Src.GetName() );
//	}
//#endif
//	return pItemProp->dwAttackRange;
//}
//
//DWORD					GetAtkItemId( CMover & Src, const DWORD dwAtkMethod )
//{
//	switch( dwAtkMethod )
//	{ 
//	case 1:
//		return Src.GetProp()->dwAtk1;
//	case 2:
//		return Src.GetProp()->dwAtk2;
//	case 3:
//		return Src.GetProp()->dwAtk3;
//	}
//	return 0;
//}

