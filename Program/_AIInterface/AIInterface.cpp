#include "stdafx.h"
#include "AIInterface.h"

#ifdef __WORLDSERVER
#include "../WorldServer/User.h"
#include "../worldserver/FLMapGrid.h"
#include "../worldserver/LinkMap.h"
#endif	// __CLIENT


BEGIN_AISTATE_MAP( CAIInterface, CAIInterface )
END_AISTATE_MAP()

CAIInterface::CAIInterface()
#if defined __WORLDSERVER && defined __AGGRO16
:m_HasAggro( FALSE )
#endif
{
	m_pStateFunc = NULL;

	m_pObj = NULL;
	m_uParty	= 0;	//sun: 9,	//__AI_0509
}
CAIInterface::CAIInterface( CObj* pObj ) 
#if defined __WORLDSERVER && defined __AGGRO16
:m_HasAggro( FALSE )
#endif
{ 
	m_pObj = pObj;
	m_pStateFunc = NULL;
	m_uParty	= 0;	//sun: 9,	//__AI_0509
}

CAIInterface::~CAIInterface()
{
}



FUNCAISTATE CAIInterface::GetAIStateFunc( DWORD dwState)
{
	const STATEMAP* lpCurrentStateMap = GetStateMap();
	const STATEMAP_ENTRIES* lpEntries = lpCurrentStateMap->lpStateMapEntries;
	// 하부 객체부터 일치하는 매시지를 찾는다. 
	while( lpCurrentStateMap->lpBaseStateMap->lpStateMapEntries != lpEntries )
	{
		int i = 0;
		while(  lpEntries[ i ].pStateFunc )
		{
			if( lpEntries[ i ].dwAIStateId == dwState )
			{
				return lpEntries[ i ].pStateFunc;
			}
			i++;
		}
		// 발견되지 않았다! 상위 객체로 포인터를 바꾼다.
		lpCurrentStateMap = lpCurrentStateMap->lpBaseStateMap;
		lpEntries = lpCurrentStateMap->lpStateMapEntries;
	}
	return NULL;
}


void CAIInterface::RouteMessage()
{
	AIMSG msg;

	while( !m_MsgQueue.empty() )
	{
		msg	= m_MsgQueue.front();
		m_MsgQueue.pop();
		if( msg.dwMessage == AIMSG_SETSTATE )
		{
			msg.dwMessage	= AIMSG_INIT;
			m_pStateFunc	= GetAIStateFunc( msg.dwParam1 );
		}
		if( m_pStateFunc )
			( this->*( m_pStateFunc ) ) ( msg );
	}
	if( m_pStateFunc )
	{
		msg.dwMessage	= AIMSG_PROCESS;
		msg.dwParam1	= 0;
		msg.dwParam2	= 0;
		( this->*( m_pStateFunc ) ) ( msg );
	}
}

void CAIInterface::SendAIMsg( DWORD dwMessage, DWORD dwParam1, DWORD dwParam2 )
{
	AIMSG msg;
	msg.dwMessage	= dwMessage;
	msg.dwParam1	= dwParam1;
	msg.dwParam2	= dwParam2;
	if( dwMessage == AIMSG_SETSTATE )
	{
		msg.dwMessage	= AIMSG_INIT;
		m_pStateFunc	= GetAIStateFunc( dwParam1 );
	}
	if( m_pStateFunc )
		( this->*( m_pStateFunc ) ) ( msg );
}

void CAIInterface::PostAIMsg( DWORD dwMessage, DWORD dwParam1, DWORD dwParam2 )
{
	//sun: 9,	//__AI_0509
	switch( dwMessage )
	{
	case AIMSG_ARRIVAL:
		{
			AIMSG msg;
			msg.dwMessage	= dwMessage;
			msg.dwParam1	= dwParam1;
			msg.dwParam2	= dwParam2;
			m_MsgQueue.push( msg );
			break;
		}
	default:	SendAIMsg( dwMessage, dwParam1, dwParam2 );	break;
	}
}


//sun: 9,	//__AI_0509
CMover* CAIInterface::ScanTargetNext( CObj* pObjCenter, int nRange, OBJID dwIdTarget, u_long uParty )
{
	// 타겟이 없어졌을 때, 공격 조건을 만족하는 주변에 같은 파티원을 검색한다.
	// 기존 타겟이 파티가 없었거나, 가드면 무시
	if( uParty == 0 || nRange <= 0 || GetMover()->GetProp()->dwClass == RANK_GUARD )
		return NULL;

	FLTRACE_LOG( PROGRAM_NAME, _T( "ScanTargetNext" ) );
	CObj* pObj;
	D3DXVECTOR3 vPos	= pObjCenter->GetPos();
	D3DXVECTOR3 vDist;
	CMover *pTarget	= NULL;

	BOOL bFlyMob	= GetMover()->IsFlyingNPC();
	float fRadius	= (float)( nRange * nRange );

	FOR_LINKMAP( pObjCenter->GetWorld(), vPos, pObj, nRange, CObj::linkPlayer, GetMover()->GetLayer() )
	{
		pTarget	= (CMover *)pObj;
		if( pTarget->IsLive() && pTarget->GetId() != dwIdTarget ) 
		{
			if( bFlyMob == pTarget->IsFly() || bFlyMob == FALSE  )
			{
				vDist	= pTarget->GetPos() - vPos;				
				if( D3DXVec3LengthSq( &vDist ) < fRadius && pTarget->m_idparty == uParty )
				{
					if( pTarget->IsMode( TRANSPARENT_MODE ) == FALSE )
						if( pTarget->HasBuffByIk3( IK3_TEXT_DISGUISE ) == FALSE )
							return pTarget;
				}
			}
		}
	}
	END_LINKMAP
		return NULL;
}


// pObjCenter를 중심으로 nRangeMeter반경내에 들어오는넘들중 가장먼저 걸리는 넘 골라냄/
// nJobCond : 해당 직업만 감시함. 직업검사 안함.
CMover* CAIInterface::ScanTarget( CObj* pObjCenter, int nRangeMeter, int nJobCond, DWORD dwQuest, DWORD dwItem, int nChao )
{
	CObj* pObj;
	D3DXVECTOR3 vPos = pObjCenter->GetPos();
	D3DXVECTOR3 vDist;
	CMover *pTarget = NULL;
	int nAble = 0;
	BOOL bCondScan = FALSE;
	if( nJobCond || dwQuest || dwItem || nChao )
		bCondScan = TRUE;

	BOOL bGuard = ( GetMover()->GetProp()->dwClass == RANK_GUARD );
	BOOL bFlyMob = GetMover()->IsFlyingNPC();

	float fRadius = (float)( nRangeMeter * nRangeMeter );	// 거리 

	FOR_LINKMAP( pObjCenter->GetWorld(), vPos, pObj, nRangeMeter, CObj::linkPlayer, GetMover()->GetLayer() )
	{
		FLASSERT( pObj->GetType() == OT_MOVER && ((CMover *)pObj)->IsPlayer() );
		pTarget = (CMover *)pObj;

		// 조건 스켄이 아니라면 늘 가능하다.
		nAble = 0;
		// 조건 스켄이라면 프랍과 일치하는 놈만 가능하다.
		if( bCondScan == TRUE && pTarget->IsLive() ) 
		{
			// 1. 직업 조건 체크    
			if( nJobCond == 0 )
				nAble++;
			else if( nJobCond == JOB_ALL || pTarget->GetJob() == nJobCond )		// JOB_ALL : 모든직업
				nAble++;
			// 2. 아이템 조건 체크  
			if( dwItem == 0 )
				nAble++;
			else if( pTarget->GetItemNum( dwItem ) != 0 )	
				nAble++;
			// 3. 퀘스트 조건 체크 
			if( dwQuest == 0 )
				nAble++;
			else if( pTarget->GetQuest( dwQuest ) != NULL )
				nAble++;	
			//4. 카오 조건 체크  
			if( nChao == 0 )
				nAble++;
			else
			{
				// 100 = 카오유저, 101 = 비카오
				if( nChao == 100 )
				{
					//sun: 8, // __S8_PK
					if( pTarget->IsChaotic() )
						//!					if( pTarget->IsGuardReactionChao() )
						nAble++;
				}
				else if( nChao == 101 )
				{
					//sun: 8, // __S8_PK
					if( !pTarget->IsChaotic() )
						//!					if( pTarget->IsGuardReactionNormal() )
						nAble++;
				}
			}
		}
		if( nAble == 4 )
		{
			// 가드는 무조건 공격			
			// 비행몹은 비행플레이어만 공격, 지상몹은 지상플레이어만만 공격 - 1. true true  2. false false

			if( bGuard || bFlyMob == pTarget->IsFly() )		
			{
				vDist = pTarget->GetPos() - vPos;				
				if( D3DXVec3LengthSq( &vDist ) < fRadius )	// 두 객체간의 거리가 범위 이내이면 
				{
					if( pTarget->IsMode( TRANSPARENT_MODE ) == FALSE )
					{
						if( pTarget->HasBuffByIk3( IK3_TEXT_DISGUISE ) == FALSE )
						{
							if( pTarget->HasBuff( BUFF_SKILL, SKILL_INDEX( 193, SI_ACR_SUP_DARKILLUSION ) ) == FALSE )				//@ 하드 코딩이다... 은신 상태를 표시할수 있는 무언가가 필요?
								return pTarget;
						}
					}
				}
			}
		}
	}
	END_LINKMAP
		return NULL;
}

// 반경내에서 가장 강한넘(레벨로)을 골라냄.
CMover* CAIInterface::ScanTargetStrong( CObj* pObjCenter, FLOAT fRangeMeter  )
{
	CObj *pObj;
	CObj *pObjMax = NULL;	// 가장쎈넘 포인터.
	D3DXVECTOR3 vPos = pObjCenter->GetPos();
	D3DXVECTOR3 vDist;

	// 지름 
	FLOAT fRadius = fRangeMeter * fRangeMeter;

	FOR_LINKMAP( pObjCenter->GetWorld(), vPos, pObj, (int)( fRangeMeter / MPU ), CObj::linkPlayer, GetMover()->GetLayer() )
	{
		vDist = pObj->GetPos() - vPos;	// 두좌표간 벡터
		float fDistSq = D3DXVec3LengthSq( &vDist );		// 두오브젝트간의 거리Sq
		if( fDistSq < fRadius )	
		{
			if(  !( ((CMover*)pObj)->IsMode( TRANSPARENT_MODE ) ) )
			{
				if( pObjMax )
				{
					if( ((CMover *)pObj)->GetLevel() > ((CMover *)pObjMax)->GetLevel() )		// 더 쎈넘을 찾았다.
						pObjMax = pObj;
				} else
					pObjMax = pObj;

			}
		}
	}
	END_LINKMAP

		return (CMover *)pObjMax;
}

// 반경내에서 오버힐 하는 어시 찾아서 죽이자.
CMover* CAIInterface::ScanTargetOverHealer( CObj* pObjCenter, FLOAT fRangeMeter  )
{
#ifndef __CLIENT
	CObj *pObj;
	D3DXVECTOR3 vPos = pObjCenter->GetPos();
	D3DXVECTOR3 vDist;

	// 지름 
	FLOAT fRadius = fRangeMeter * fRangeMeter;

	FOR_LINKMAP( pObjCenter->GetWorld(), vPos, pObj, (int)( fRangeMeter / MPU ), CObj::linkPlayer, GetMover()->GetLayer() )
	{
		vDist = pObj->GetPos() - vPos;	// 두좌표간 벡터
		float fDistSq = D3DXVec3LengthSq( &vDist );		// 두오브젝트간의 거리Sq
		if( fDistSq < fRadius )	
		{
			if(  !( ((CMover*)pObj)->IsMode( TRANSPARENT_MODE ) ) )
			{
				CMover *pMover = (CMover *)pObj;

				if( pMover->IsPlayer() )
					if( ((FLWSUser *)pMover)->m_nOverHeal > 0 )		// 오버힐러를 찾았다.
						return pMover;
			}
		}
	}
	END_LINKMAP
#endif	// __CLIENT
		return NULL;
}

//--------------------------------------------------------------------------------------------------------------------------------------//

#if defined __WORLDSERVER && defined __AGGRO16

CAIAggroInterface::CAIAggroInterface( CObj * pMover, const DWORD dwAggroID ) 
: CAIInterface( pMover )
, m_pAggro( new FLAggro( dwAggroID ) )
{
	Init();
	m_HasAggro = TRUE;
}

CAIAggroInterface::~CAIAggroInterface()
{
	delete m_pAggro;
}

FLAggro * CAIAggroInterface::GetAggroMng()
{
	return m_pAggro;//.get();		//&m_AggroMng;
}

void CAIAggroInterface::Init()
{
	GetAggroMng()->Clear();
	//m_AggroMng.Clear();
}


CMover * CAIAggroInterface::ScanTargetByContidion( const int nRangeMeter, const int nJobCond, DWORD dwQuest, const DWORD dwItem, const int nChao )
{
	// Aggro Max
	CMover * pTarget	= NULL;

	OBJID aggroOBJID;
	if( GetAggroMng() != NULL && GetAggroMng()->GetMaxAggro( aggroOBJID ) == TRUE )
	{
		CMover * pAggroTarget	= prj.GetMover( aggroOBJID );
		//if( IsValidObj( static_cast< CObj * >( static_cast< CCtrl * >( pAggroTarget ) ) ) == FALSE ||
		//if( IsValidObj( ( CObj * )pAggroTarget ) == FALSE ||
		if( IsValidObj( (CObj*)pAggroTarget ) == FALSE ||
			pAggroTarget->GetWorld()->GetID() != GetWorld()->GetID() ||
			pAggroTarget->IsDie() == TRUE ||
			pAggroTarget->HasBuff( BUFF_SKILL, SKILL_INDEX( 193, SI_ACR_SUP_DARKILLUSION ) ) == TRUE )
		{
			GetAggroMng()->RemoveMover( /**GetMover(),*/ aggroOBJID );
		}
		else
		{
			pTarget				= pAggroTarget;
			return pTarget;
		}
	}

	const int jobCond			= GetMover()->m_bActiveAttack ? JOB_ALL : nJobCond;

	pTarget = CAIInterface::ScanTarget( GetMover(), nRangeMeter, jobCond, dwQuest, dwItem, nChao );
	if( pTarget != NULL )
	{
		GetAggroMng()->SumFirstTargetAggro( *GetMover(), pTarget->GetId() );
	}
	else
	{
		Init();
	}

	return pTarget;
}

void CAIAggroInterface::SendAIMsg( DWORD dwMessage, DWORD dwParam1 /*= 0*/, DWORD dwParam2 /*= 0 */ )
{
	///aggro 관련 행동 처리. STATE_ 와의 관련은 없다.
	if( GetAggroMng() != NULL )
	{
		switch( dwMessage )
		{
		case AIMSG_INIT_TARGETCLEAR:		
		case AIMSG_DSTDIE:	
			{
				const OBJID objid = ( OBJID )dwParam1;
				GetAggroMng()->RemoveMover( /**GetMover(),*/ objid );
				if( GetAggroMng()->Size() == 0 )
				{
					Init();		
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
				//const int nDamage		= ( dwParam2 == 0 ); ? 1 : ( int )dwParam2;
				const int nDamage		= dwParam2;
				if( nDamage <= 0 )
					break;

				if( GetAggroMng()->SumFirstAttackerAggro( *GetMover(), objid ) == FALSE )
					GetAggroMng()->SumAttackAggro( *GetMover(), objid, nDamage );
			}
			break;
		case AIMSG_INIT:
			//case AIMSG_INIT_TARGETCLEAR:
		case AIMSG_TIMEOUT:
		case AIMSG_DIE:	
		case AIMSG_EXIT:
			{
				Init();
			}
			break;
		default:
			int i = 0;		//breakpoint 용
			UNREFERENCED_PARAMETER( i );
			__noop;
			break;
		}
	}

	CAIInterface::SendAIMsg( dwMessage, dwParam1, dwParam2 );
}

#endif