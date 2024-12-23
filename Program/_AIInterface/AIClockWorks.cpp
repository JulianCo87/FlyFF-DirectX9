#include "stdafx.h"

#include "AIClockWorks.h"
#include "../WorldServer/User.h"
#include "Party.h"

extern	CPartyMng		g_PartyMng;

#ifdef __INTERNALSERVER
static const TCHAR LOG_BOSS_AI_CLOCKWORKS[] = _T( "Boss(ClockWorks)" );
#endif

// Event Pattern Type
#define		EPT_READY			0		// 아무것도 안하고 있음.
#define		EPT_MOVING			1		// 감시/이동
#define		EPT_ATTACKING		2		// 공격중.
#define		EPT_TRACKING		3		// 추적중.

// ClockWorks Attack Pattern
#define		CAT_NONE			0
#define		CAT_SLASH			1		// 크게 위에서 아래로 베며 찍기.
#define		CAT_SHOOT			2		// 왼팔로 로켓포 발사
#define		CAT_LASER			3		// 레이저로 긁기.
#define		CAT_QUAKE			4		// 앞발로 밟아서 파동을 일으킴.

enum
{
	STATE_INIT = 1,
	STATE_APPEAR,		// 등장씬
	STATE_IDLE,			// 대기상태 - 보스몹을 일반 필드에 리스폰시킬땐 이것부터 실행하면 된다.
	STATE_RAGE,			// 공격상태.
	STATE_SUPER_RAGE	// 졸라 화난 상태.
};
BEGIN_AISTATE_MAP( CAIClockWorks, CAIInterface )

	ON_STATE( STATE_INIT   , StateInit   )
	ON_STATE( STATE_IDLE   , StateIdle   )
	ON_STATE( STATE_RAGE   , StateRage   )
	ON_STATE( STATE_SUPER_RAGE   , StateRage   )

END_AISTATE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MEMPOOLER_IMPLEMENT( CAIClockWorks, 4 );

CAIClockWorks::CAIClockWorks()
{
	Init();
}
CAIClockWorks::CAIClockWorks( CObj* pObj ) : CAIInterface( pObj )
{
	Init();
}

CAIClockWorks::~CAIClockWorks()
{
	Destroy();
}

void CAIClockWorks::Init( void )
{
	m_vPosBegin.x = m_vPosBegin.y = m_vPosBegin.z = 0;
	m_nEvent = EPT_READY;
	m_nAttackType = 0;
	m_tmReattack = m_tmAddReattack = m_tmTrace = m_tmTimeOver = timeGetTime();
	m_idTarget = NULL_ID;
	m_vTarget.x = m_vTarget.y = m_vTarget.z = 0;
	m_idLastAttacker = NULL_ID;
	m_bDefenseMode = FALSE;
	m_nAppearCnt = 0;
}

void CAIClockWorks::Destroy( void )
{
}

void CAIClockWorks::InitAI()
{
	m_vPosBegin = GetMover()->GetPos();
	PostAIMsg( AIMSG_SETSTATE, STATE_RAGE, NULL_ID );	// 최초 대기상태로 초기화.
#ifdef __INTERNALSERVER
	FLERROR_LOG( LOG_BOSS_AI_CLOCKWORKS, _T( "ClockWorks start" ) );
#endif
}

BOOL CAIClockWorks::IsIdleMode()
{
	FUNCAISTATE pStateFunc	= GetAIStateFunc( STATE_IDLE );
	if( pStateFunc != NULL && pStateFunc == m_pStateFunc )
		return TRUE;

	return FALSE;
}

BOOL CAIClockWorks::StateInit( const AIMSG & /*msg*/ )
{
	return TRUE;
}

BOOL CAIClockWorks::MoveProcessIdle()
{
#ifdef __MOVER_STATE_EFFECT
	CMover* pMover = GetMover();

	if( IsInvalidObj( pMover ) == TRUE && pMover->m_pActMover->IsStateFlag( OBJSTAF_COMBAT ) == TRUE )
	{
		if( pMover->m_pActMover->SendActMsg( OBJMSG_MODE_PEACE ) == TRUE )
		{
			g_xWSUserManager->AddSendActMsg( pMover, OBJMSG_MODE_PEACE );
		}
	}
#endif // __MOVER_STATE_EFFECT

	return TRUE;
}

BOOL CAIClockWorks::StateIdle( const AIMSG & msg )
{
//	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();
	
	BeginAIHandler( )
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT ) 
//		if( pMover->TryDelete_IntoIdleMode() == TRUE )
//			return TRUE;

		SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, NULL_ID );	// 보스는 IDLE패턴이 없다. 바로 공격에 들어간다.
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_PROCESS ) 
		MoveProcessIdle();
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DAMAGE ) 
		SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, NULL_ID );
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DIE ) 
		SendAIMsg( AIMSG_EXIT );
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_COLLISION )
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_ARRIVAL )
		
		///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_EXIT )	
		
	EndAIHandler( )
			return TRUE;
}

BOOL	CAIClockWorks::SelectTarget( void )
{
	CMover	*pMover = GetMover();
//	CWorld	*pWorld = GetWorld();
//	CModelObject *pModel = (CModelObject *)pMover->GetModel();

	FLOAT fRadius = pMover->GetRadiusXZ();		// this의 반지름
	FLOAT fRadiusSq = fRadius * fRadius;		// 반지름Sq버전.
	
	CMover *pLastAttacker = prj.GetMover( m_idLastAttacker );
	if( pLastAttacker == NULL )			// LastAttacker가 없어졌으면 타겟 다시 잡을 수 있도록 하자.
	{
		m_idLastAttacker = NULL_ID;
	} else
	{		
		D3DXVECTOR3 vDist = pLastAttacker->GetPos() - pMover->GetPos();
		FLOAT fDistSq = D3DXVec3LengthSq( &vDist );		// 목표지점까지의 거리.
		if( fDistSq >= fRadiusSq * 10.0f )				// 라스트어태커가 내 반지름의 10배이상 떨어져있으면
		{
			// 타겟 포기
#ifdef __INTERNALSERVER
			FLERROR_LOG( LOG_BOSS_AI_CLOCKWORKS, _T( "LastAtker(%s)가 너무 멀어서(%5.2f) 타겟포기함" ), pLastAttacker->GetName(), sqrt(fDistSq) );
#endif
			
			m_idLastAttacker = NULL_ID;
			pLastAttacker = NULL;
		}
	}


	m_idTarget = NULL_ID;
	m_vTarget.x = m_vTarget.y = m_vTarget.z = 0;	// 일단 이건 안쓰는걸로 하자.

	if( m_idLastAttacker == NULL_ID )		// 아직 날 때린쉐리가 없다.
	{
		// 2006.06.08 ata2k JOB_ALL추가
		CMover* pTarget = ScanTarget( pMover, 80, JOB_ALL );		// 80미터 반경내에서 아무나 고른다.
		if( pTarget )
		{
			// this가 비행형 몬스터거나 || 타겟이 비행중이 아닐때만 공격.
			if( pMover->IsFlyingNPC() || pTarget->m_pActMover->IsFly() == FALSE )	
			{
				m_idTarget = pTarget->GetId();
#ifdef __INTERNALSERVER
				FLERROR_LOG( LOG_BOSS_AI_CLOCKWORKS, _T( "scanTarget:%s" ), pTarget->GetName() );
#endif
				
			}
			else
				return FALSE;		// 못 찾았으면 걍 리턴. - 원래는 다른타겟을 찾아야겠지.
		} else
			return FALSE;
	} else
	// 날 때린 쉐리가 있다.
	{
		DWORD dwNum = xRandom( 100 );		// 0 ~ 99까지으 난수.
		if( dwNum < 50 )		
		{
			// 50% 확률로 마지막으로 날 때린넘 공격.
			m_idTarget = m_idLastAttacker;		// 날 공격한 쉐리를 타겟으로 지정하자.
		} else
		if( dwNum < 75 )
		{
			// 50미터 반경내에서 가장 쎈넘을 잡자.
			CMover *pTarget = ScanTargetStrong( pMover, 50 );		
			if( pTarget )
			{
				// this가 비행형 몬스터거나 || 타겟이 비행중이 아닐때만 공격.
				if( pMover->IsFlyingNPC() || pTarget->m_pActMover->IsFly() == FALSE )	
				{
					m_idTarget = pTarget->GetId();
#ifdef __INTERNALSERVER
					FLERROR_LOG( LOG_BOSS_AI_CLOCKWORKS, _T( "Leader:%s %d" ), pTarget->GetName(), pTarget->GetLevel() );
#endif
				}
				else
					m_idTarget = m_idLastAttacker;		// 타겟이 공격하기가 여의치 않으면 마지막으로 때린쉐리 공격하자.
			} else
				m_idTarget = m_idLastAttacker;		// 타겟이 공격하기가 여의치 않으면 마지막으로 때린쉐리 공격하자.
		} else
		if( dwNum < 100 )
		{
			// 오버힐하는 어시를 죽이자.
			CMover *pTarget = ScanTargetOverHealer( pMover, 50 );		
			if( pTarget )
			{
				// this가 비행형 몬스터거나 || 타겟이 비행중이 아닐때만 공격.
				if( pMover->IsFlyingNPC() || pTarget->m_pActMover->IsFly() == FALSE )	
				{
					m_idTarget = pTarget->GetId();
#ifdef __INTERNALSERVER
					FLERROR_LOG( LOG_BOSS_AI_CLOCKWORKS, _T( "OverHealer:%s" ), pTarget->GetName() );
#endif
				}
				else
					m_idTarget = m_idLastAttacker;		// 타겟이 공격하기가 여의치 않으면 마지막으로 때린쉐리 공격하자.
			} else
				m_idTarget = m_idLastAttacker;		// 타겟이 공격하기가 여의치 않으면 마지막으로 때린쉐리 공격하자.
		}
#ifdef __INTERNALSERVER
		{
			CMover *pTarget = prj.GetMover( m_idTarget );
			CMover *pMover = prj.GetMover( m_idLastAttacker );
			CString str;
			if( pMover )
			{
				if( pTarget )
					FLERROR_LOG( LOG_BOSS_AI_CLOCKWORKS, _T( "rand:%d LastAttacker:%s SelectTarget:%s" ), dwNum, pMover->GetName(), pTarget->GetName() );
				else
					FLERROR_LOG( LOG_BOSS_AI_CLOCKWORKS, _T( "rand:%d LastAttacker:%s SelectTarget:%s" ), dwNum, pMover->GetName(), _T( "없음" ) );
			}
			else
			{
				if( pTarget )
					FLERROR_LOG( LOG_BOSS_AI_CLOCKWORKS, _T( "rand:%d LastAttacker:%s SelectTarget:%s" ), dwNum, _T( "없음." ), pTarget->GetName() );
				else
					FLERROR_LOG( LOG_BOSS_AI_CLOCKWORKS, _T( "rand:%d LastAttacker:%s SelectTarget:%s" ), dwNum, _T( "없음." ), _T( "없음" ) );
			}

		}
#endif
	}

	// 적절한 공격대상을 정하자.
	// .가장 마지막으로 날 때린넘 - 50%. - 골고루 공격한다는 느낌을 줌.
	// .나에게 가장 위협적인넘(데미지를 많이 주는넘 혹은 가장 강한 넘) - 25%
	// .오버힐 하는 어시 - 25%.
	// .떼거리로 가장 몰려있는 구역의 중앙에 있는넘. - 10% - 일단 이건 나중에

	return TRUE;
}

//
//	Rage Processs
//
BOOL CAIClockWorks::MoveProcessRage()
{
	CMover	*pMover = GetMover();
//	CWorld	*pWorld = GetWorld();
	CModelObject *pModel = (CModelObject *)pMover->GetModel();

	if( m_bDefenseMode == FALSE && pMover->GetHitPointPercent() <= 100 )		// 20%이하로 피가 빠지면
	{
		m_bDefenseMode = TRUE;
		pMover->SetDestParam( DST_ADJDEF_RATE, 800, NULL_CHGPARAM );			// 방어도 30%증가.
	}


	if( m_nEvent == EPT_READY )
	{
		if( pMover->m_pActMover->m_bGround == 1 && pMover->m_pActMover->GetActionState() != OBJSTA_APPEAR )		// 바닥에 떨어지면..
			pMover->SendActMsg( OBJMSG_APPEAR );		// 포효. 이게 클라에선 등장모션이 안나올꺼샤...
		if( ++m_nAppearCnt >= PROCESS_COUNT * 5 )		// 
		{
			PostAIMsg( AIMSG_END_APPEAR );		// 등장씬 애니메이션이 끝났음을 알림.
			pMover->m_pActMover->ResetState( OBJSTA_ACTION_ALL );
			m_nAppearCnt = 0;
		}
		// 아무것도 안함.
	} else
	// 공격중 애니.
	if( m_nEvent == EPT_ATTACKING )
	{
		BOOL bEnd = FALSE;

		if( pMover->m_dwMotion == MTI_STAND || pModel->IsEndFrame()	)
			bEnd = TRUE;		// 공격 프레임 끝났는가. - 여기다 이걸 안넣어도 되는데 만약 공격동작의 끝을 놓치는 일이 생길때를 대비해서 넣어두면 조타.
		if( timeGetTime() >= m_tmTimeOver + SEC(15) )				// 공격시작한지 10초이상오버됐으면 걍 끝남.
			bEnd = TRUE;
		if( bEnd )
		{
			m_nEvent = EPT_MOVING;				// 이동상태로 전환.
			m_tmReattack = timeGetTime();		// 리어택 타임 리셋;
			m_tmAddReattack = 2000; // xRandom( 2000 );	// x초 범위내에서 리어택이 단축될 수 있다.
			m_idTarget = NULL_ID;				// 타겟을 클리어.
		} 
	} else
	// 공격을 위해 추격중.
	if( m_nEvent == EPT_TRACKING )
	{
		if( timeGetTime() >= m_tmTrace + 3000 )		// 추적을 시작한지 3초가 지나면 추적을 포기
		{
			m_nEvent = EPT_MOVING;
			m_idTarget = NULL;			// 여기서 이걸 클리어 해줘야 아래에서 SelectTarget()을 다시 한다 .
			m_vTarget.x = m_vTarget.y = m_vTarget.z = 0;			
		}
	} else
	//------------- 공격중이 아님. --------------------
	{

		if( timeGetTime() >= m_tmReattack + (2000 - m_tmAddReattack) )		// 리어택 시간이 됐는가?  기본적으로 5초간격에 AddReattack으로 줄어들 수 있음.
		{
			FLOAT fRadius = pMover->GetRadiusXZ();		// this의 반지름
			FLOAT fRadiusSq = fRadius * fRadius;		// 반지름Sq버전.
			
			if( m_idTarget == NULL_ID &&										// 공격대상이 정해지지 않았고
				(m_vTarget.x == 0 && m_vTarget.y == 0 && m_vTarget.z == 0) )	// 공격위치도 정해지지 않았다.
			{
				// 어떤 쉐리를 공격할까....? 를 선정함.
				if( SelectTarget() == FALSE )
					return FALSE;
			}

			D3DXVECTOR3	vTarget;
			FLOAT	fDistSq = 0;					// 공격지점과 this의 거리.
			CMover *pTarget = NULL;
			if( m_idTarget != NULL_ID )		// 타겟오브젝이 있을때
			{
				pTarget = prj.GetMover( m_idTarget );
				if( IsValidObj(pTarget) )
					vTarget = pTarget->GetPos();		// 공격좌표는 타겟무버의 좌표
				else
				{
					m_idTarget = NULL_ID;
					return FALSE;		// 타겟이 거시기 하면 걍리턴.
				}
			} else
			if( m_vTarget.x && m_vTarget.y && m_vTarget.z )		// 공격 좌표로 설정되어 있을때.
			{
				vTarget = m_vTarget;
			} else
			{
//				FLERROR_LOG( PROGRAM_NAME, _T( "타겟이 없다" ) );
				return FALSE;
			}

			D3DXVECTOR3 vDist = vTarget - pMover->GetPos();
			fDistSq = D3DXVec3LengthSq( &vDist );		// 목표지점까지는 거리.
			FLOAT fArrivalRange = fRadius;			// 얼마나 근접해야하는가? 디폴트로 반지름 길이.

			if( fDistSq < fRadiusSq * 1.5f )		// 근거리면.
			{
				m_nAttackType = CAT_QUAKE;		// 앞발로 밟기.
				fArrivalRange = 0;				// 딱붙어서 밟아야 한다.
			} else
			if( fDistSq < fRadiusSq * 3.0f )		// this의 반지름x배거리 내에 있으면 중거리.
			{
				if( xRandom(100) < 50 )
					m_nAttackType = CAT_SLASH;	// 칼로 찍기
				else
					m_nAttackType = CAT_LASER;	// 레이저
				fArrivalRange = fRadius;		// 반지름 길이만큼 접근하자.
			
			} else
			if( fDistSq < fRadiusSq * 6.0f )		// 반지름의 x배 이하는 쏘기.
			{								
				// 반지름 3배거리 이상이면 원거리.
				m_nAttackType = CAT_SHOOT;		// 포쏘기.
				fArrivalRange = 30.0f;			// xx미터 까지 접근하자.
			} else
			{
				// 엄청 멀리 떨어져있는넘은 포기.
				m_idTarget = NULL_ID;
				return FALSE;
			}

//			m_nAttackType = CAT_LASER;	// 레이저
//			fArrivalRange = fRadius;		// 반지름 길이만큼 접근하자.
			
			m_tmTrace = timeGetTime();		// 추적 시작한 시간.
			m_nEvent = EPT_TRACKING;		// 공격추격상태 전환.
			pMover->CMD_SetMeleeAttack( m_idTarget, fArrivalRange );		// 공격반경은 this의 반지름 크기정도가 적당하다.

#ifdef __INTERNALSERVER
			{
				CString strType;
				switch( m_nAttackType )
				{
				case CAT_SLASH:		strType = "베기";	break;
				case CAT_SHOOT:		strType = "쏘기";	break;
				case CAT_LASER:		strType = "레이저";	break;
				case CAT_QUAKE:		strType = "밟기";	break;
				}
				FLERROR_LOG( LOG_BOSS_AI_CLOCKWORKS, _T( "AtkType:%s Dist:%5.2f" ), strType, sqrt(fDistSq) );
			}
#endif
			
				
		} else	// 리어택 타이밍에 공격.
		{
			// 공격하기 적절한 위치로 이동하자.
		}
	}

	return TRUE;
}

//
// 
//
BOOL CAIClockWorks::StateRage( const AIMSG & msg )
{
	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();
	
	BeginAIHandler( )
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT ) 

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_PROCESS ) 
		MoveProcessRage();
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DAMAGE ) 
		// 나에게 데미지 많이 준놈들을 기억하고 있자. 나중에 응징하자.
		m_idLastAttacker = msg.dwParam1;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DIE ) 
		SendAIMsg( AIMSG_EXIT );
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_COLLISION )
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_ARRIVAL )
	{
		OBJMSG	dwMsg = OBJMSG_NONE;
		DWORD	dwItemID = 0;
		MoverProp	*pProp = pMover->GetProp();
		// 추격하여 도착하면 선택되었던 공격방식을 적용시킨다.
		switch( m_nAttackType )
		{
		case CAT_SLASH:		dwMsg = OBJMSG_ATK1;	dwItemID = pProp->dwAtk1;	break;
		case CAT_LASER:		dwMsg = OBJMSG_ATK3;	dwItemID = pProp->dwAtk2;	break;
		case CAT_SHOOT:		dwMsg = OBJMSG_ATK2;	dwItemID = pProp->dwAtk3;	break;
		case CAT_QUAKE:		dwMsg = OBJMSG_ATK4;	dwItemID = pProp->dwAtk4;	break;
		default:
			FLERROR_LOG( PROGRAM_NAME, _T( "AIMSG_ARRIVAL m_nAttackType=%d" ), m_nAttackType );
		}

		if( dwMsg )
		{
			if( m_idTarget != NULL_ID )
			{
				if( pMover->DoAttackMelee( m_idTarget, dwMsg, dwItemID ) == 1 )
				{
					m_nEvent = EPT_ATTACKING;		// 공격상태 전환.
					m_tmTimeOver = timeGetTime();	// 공격시작후 타임오버 계산용
				} else
					m_nEvent = EPT_MOVING;			// 공격이 실패해서 다시 대기상태로 돌아감.
				

/*				CMover *pTarget = prj.GetMover( m_idTarget );
				if( IsValidObj(pTarget) )
				{
					int nError = pMover->SendActMsg( dwMsg, pTarget->GetId(), MAKELONG( 0, 0 ), MAKELONG( 0, 0 ) );
					if( nError > 0 ) 
					{
						g_xWSUserManager->AddMeleeAttack( pMover, dwMsg, pTarget->GetId(), MAKELONG( 0, 0 ), MAKELONG( 0, (WORD)nError ) );
					}
					m_nEvent = EPT_ATTACKING;		// 공격상태 전환.
				} else
					m_nEvent = EPT_MOVING;		// 비정상적인 상태. 원래대로 돌림.
*/
			}
		} else
			m_nEvent = EPT_MOVING;		// 비정상적인 상태. 원래대로 돌림.
	}

	

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_ATTACK_MELEE )
		switch( m_nAttackType )			// 선택된 공격 패턴.
		{
		case CAT_SLASH:
			// 폭발 이펙트 생성지점 계산.
			// 폭발 이펙트 생성;
			// 범위 공격;
			break;
		case CAT_LASER:
//			if( 타점 이후 부터 )
			{
				//레이저 좌표 이동;
				//범위 데미지;
			}
			break;
		case CAT_SHOOT:
			// 미사일 오브젝트 생성;
			break;
		case CAT_QUAKE:		// 밟는 공격 타점.
			// 범위 데미지;
			break;
		default:
			FLERROR_LOG( PROGRAM_NAME, _T( "AIMSG_ATTACK_MELEE %d" ), m_nAttackType );
			break;
		}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_END_APPEAR )	
		m_nEvent = EPT_MOVING;		// 등장상태가 끝나면 감시/이동상태로 전환.

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_END_MELEEATTACK )	
		m_nEvent = EPT_MOVING;				// 이동상태로 전환.
		m_tmReattack = timeGetTime();		// 리어택 타임 리셋;
		m_tmAddReattack = xRandom( 2000 );	// x초 범위내에서 리어택이 단축될 수 있다.
		m_idTarget = NULL_ID;				// 타겟을 클리어.
				
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_EXIT )	
	
	EndAIHandler( )
	return TRUE;
}





