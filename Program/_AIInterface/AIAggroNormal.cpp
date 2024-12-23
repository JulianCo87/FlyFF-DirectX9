#include "stdafx.h"

#if defined __WORLDSERVER && defined __AGGRO16

#include "AIAggroNormal.h"

#include "lang.h"
#include "dialogMsg.h"
#include "../WorldServer/User.h"
#include "../worldserver/FLMapGrid.h"
#include "../worldserver/LinkMap.h"
#include "Party.h"
#include "PathObj.h"



extern	CPartyMng		g_PartyMng;

#define RANGE_CANCEL			120.0f
#define RANGE_MOVE				30.0f
#define	RANGE_RETURN_TO_BEGIN	120.0F

#define	TIMEGETTIME		g_tmCurrent

enum
{
	STATE_NONE = 0, 
	STATE_INIT = 1,
	STATE_IDLE,
	STATE_WANDER,
	STATE_PURSUE,
	STATE_EVADE,
	STATE_RUNAWAY,
	STATE_RAGE
	, STATE_STAND
	, STATE_PATROL	
	, STATE_RAGE_PATROL	
};

BEGIN_AISTATE_MAP( CAIAggroNormal, CAIAggroInterface )

	ON_STATE( STATE_INIT   , StateInit   )
	ON_STATE( STATE_IDLE   , StateIdle   )
	ON_STATE( STATE_WANDER , StateWander )
	ON_STATE( STATE_EVADE  , StateEvade  )
	ON_STATE( STATE_RAGE   , StateRage   )
	ON_STATE( STATE_RUNAWAY, StateRunaway  )
	ON_STATE( STATE_STAND  , StateStand   )
	ON_STATE( STATE_PATROL , StatePatrol  )
	ON_STATE( STATE_RAGE_PATROL , StateRagePatrol  )
	
END_AISTATE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __INTERNALSERVER
	const int INITIAL_AI_MONSTER = 4;
#else
	const int INITIAL_AI_MONSTER = 128 * 5;
#endif

MEMPOOLER_IMPLEMENT( CAIAggroNormal, INITIAL_AI_MONSTER );

static DWORD s_tmAttack = SEC(15);		//  m_tmAttack 20초 



CAIAggroNormal::CAIAggroNormal( CObj* pObj, const DWORD dwAggroID ) : CAIAggroInterface( pObj, dwAggroID )
{
	Init();
	GetAggroMng()->Initialize( pObj->GetIndex() );
}

void CAIAggroNormal::Init()
{
	SetTarget( NULL_ID, 0 );
	m_bReturnToBegin	= FALSE;
	m_bGoTarget			= FALSE;
	m_bTargetNoMovePos	= FALSE;
	m_dwAtkMethod		= 0;
	m_bFirstRunaway		= FALSE;
	m_bCallHelper		= FALSE;
	m_tmAttack			= TIMEGETTIME + s_tmAttack;		// 공격 후 20초간 추적해옴.
	m_vOldPos.x			= m_vOldPos.y = m_vOldPos.z = 0;
	m_bRangeAttack		= FALSE;
	m_tmSummon			= TIMEGETTIME;
	m_tmHelp			= 0;

	m_bLootMove			= FALSE;
	m_idLootItem		= NULL_ID;
	m_tmAttackDelay		= m_tmMove = m_tmReturnToBegin = TIMEGETTIME;
	m_fAngleBegine		= 0.0f;
	m_vPosDamage		= D3DXVECTOR3( 0, 0, 0 );
}

CAIAggroNormal::~CAIAggroNormal()
{
}

void CAIAggroNormal::InitAI()
{
	m_vPosBegin = GetMover()->GetPos();
	m_fAngleBegine = GetMover()->GetAngle();
	PostAIMsg( AIMSG_SETSTATE, GetMover()->m_dwAIInterfaceState ); 	
}

BOOL CAIAggroNormal::IsIdleMode()
{
	FUNCAISTATE pStateFunc	= GetAIStateFunc( STATE_IDLE );
	if( pStateFunc != NULL && pStateFunc == m_pStateFunc )
		return TRUE;

	return FALSE;
}

void CAIAggroNormal::SetStop( DWORD dwTime ) 
{ 
	m_tmMove = TIMEGETTIME + dwTime;
	GetMover()->SetStop(); 
} 

BOOL CAIAggroNormal::IsEndStop() 
{ 
	return ( TIMEGETTIME > m_tmMove ) && ( IsMove() == FALSE ) ; 
}

void CAIAggroNormal::MoveToDst(	const D3DXVECTOR3& vDst )
{
	CMover* pMover = GetMover();
	if( pMover->GetDestPos() == vDst )
		return;

	pMover->SetDestPos( vDst );
	g_xWSUserManager->AddSetDestPos( pMover, vDst, 1 );
}

void CAIAggroNormal::MoveToDst(	OBJID dwIdTarget )
{
	CMover* pMover = GetMover();
	if( pMover->GetDestId() == dwIdTarget )
		return;

	GetMover()->SetSpeedFactor( 2.0F );

	CMover* pTarget	= prj.GetMover( dwIdTarget );
	if( IsValidObj( pTarget ) )
		SetTarget( pTarget->GetId(), pTarget->m_idparty );

	pMover->SetDestObj( dwIdTarget, 0.0f, TRUE ); // 목표를 재설정해준다.
	//g_xWSUserManager->AddMoverSetDestObj( (CMover*)pMover, dwIdTarget );
}

void CAIAggroNormal::MoveToRandom( UINT nState )
{
	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();

	D3DXVECTOR3 vPos = pMover->GetPos();
	DWORD x1 = xRand();
	DWORD x2 = xRand();

	vPos.x += int( x1 % 21 ) - 10;
	vPos.z += int( x2 % 21 ) - 10;

	if( nState == STATE_IDLE )
	{
		D3DXVECTOR3 vDistant = vPos - m_vPosBegin;
		vDistant.y = 0.0f;

		float fRange = RANGE_MOVE;
		if( pMover->IsFlyingNPC() )			// 비행몹의 경우 거리 3배
			fRange *= 3.0f;
		
		if( IsInRange( vDistant, fRange ) )	// 15미터 반경을 벗어나지 못함 
		{
			MoveToDst( vPos );
		}
		else
		{
			// 집에가는 중에 랜덤호출이면 충돌회피다. 회피하자.
			if( m_bReturnToBegin )
				MoveToDst( vPos );
			else
			{
				// 만약 내가 이미 스폰 지역을 한참 벗어나있다면 돌아가도록 시도.
				vDistant = pMover->GetPos() - m_vPosBegin;
				if( IsInRange( vDistant, fRange ) == FALSE )
					DoReturnToBegin();
			}
		}
	}
	else
	{
		MoveToDst( vPos );
	}
}

BOOL CAIAggroNormal::IsInRange( const D3DXVECTOR3& vDistant, FLOAT fRange )
{
	D3DXVECTOR3 vDist = vDistant;
	vDist.y	= 0.0f;
	const float fDistSq = D3DXVec3LengthSq( &vDist );	
	if( fDistSq > fRange * fRange )	
		return FALSE;
	return TRUE;
}

// 근처의 아이템을 집으러 간다.
BOOL CAIAggroNormal::SubItemLoot()
{
	CMover* pMover = GetMover();
	CWorld* pWorld = GetWorld();
//	MoverProp *pProp = pMover->GetProp();
	D3DXVECTOR3 vPos = pMover->GetPos();
	CObj *pObj = NULL;
	int nRange = 0;
	D3DXVECTOR3 vDist;
	FLOAT fDistSq;
	CObj *pMinObj = NULL;

	// 근처의 아이템을 검색함.
	FOR_LINKMAP( pWorld, vPos, pObj, nRange, CObj::linkDynamic, pMover->GetLayer() )
	{
		if( pObj->GetType() == OT_ITEM )
		{
			PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( pObj->m_dwIndex );
			if( pItemProp && pItemProp->dwItemKind3 != IK3_QUEST && pItemProp->dwItemKind1 != IK1_GOLD
#ifdef __EVENT_0117
				&& pItemProp->dwID != ITEM_INDEX( 26409, II_SYS_SYS_SCR_BXPIG )
				&& pItemProp->dwID != ITEM_INDEX( 26412, II_SYS_SYS_SCR_BXPIG_TWN )
#endif	// __EVENT_0117
				)
			{
				vDist = pObj->GetPos() - pMover->GetPos();
				fDistSq = D3DXVec3LengthSq( &vDist );			// 거리 구함.
				if( fDistSq < 10.0f * 10.0f )					// 10미터 이내고... random
				{
					pMinObj = pObj;			
					goto NEXT;
				}
			}
		}
	}
	END_LINKMAP

NEXT:
	if( pMinObj )
	{
		MoveToDst( pMinObj->GetPos() );		// 목표쪽으로 이동.
		m_idLootItem = ((CItem *)pMinObj)->GetId();
		m_bLootMove = TRUE;
		g_xWSUserManager->AddDlgEmoticon( pMover, DLGEMOT_LOOT );
	}

	return TRUE;
}

BOOL CAIAggroNormal::StateInit( const AIMSG & /*msg*/ )
{
	return TRUE;
}

// 태어난 지점으로 돌아가라.
void CAIAggroNormal::DoReturnToBegin( BOOL bReturn )
{
	if( bReturn )
	{
		CAIAggroInterface::Init();					//어그로 AI 정보를 Init 한다.

		m_bReturnToBegin	= TRUE;
		m_tmReturnToBegin	= TIMEGETTIME;		// 돌아가라는 명령을 받은 시간 기록.
		SetTarget( NULL_ID, 0 );
		
		CMover* pMover	= GetMover();
		pMover->SetSpeedFactor( 2.66F );

		MoveToDst( m_vPosBegin );
	}
	else
	{
		m_bReturnToBegin	= FALSE;
		SetTarget( NULL_ID, 0 );

		CMover* pMover	= GetMover();
		pMover->SetPointParam( DST_HP, pMover->GetMaxHitPoint() );
		pMover->RemoveAllEnemies();
	}
}

BOOL CAIAggroNormal::MoveProcessIdle( const AIMSG & /*msg*/ )
{
	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();
	MoverProp *pProp = pMover->GetProp();

	// 데미지 상태거나 죽었을 경우 이동 처리 수행 않음 
	if( pMover->IsDie() || (pMover->m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL) )
		return FALSE;

	if( pMover->m_pActMover->IsStateFlag( OBJSTAF_COMBAT ) == TRUE )
	{
		if( pMover->m_pActMover->SendActMsg( OBJMSG_MODE_PEACE ) == TRUE )
		{
			g_xWSUserManager->AddSendActMsg( pMover, OBJMSG_MODE_PEACE );
		}
	}

	// 스폰 장소로 귀환 
	if( m_bReturnToBegin )
	{
		if( IsInRange( m_vPosBegin - pMover->GetPos(), 7.0f ) )		// 시작지점으로 돌아왔다.
		{
			DoReturnToBegin( FALSE );
			SendAIMsg( AIMSG_ARRIVAL, NULL_ID );
		}

		if( TIMEGETTIME - m_tmReturnToBegin >= SEC(20) )		// 집으로 돌아가기 시작한지 10초나 지났다.
		{
			// 제대로 못돌아가고 있다고 판단하고 시작지점으로 워프시킴.
			pMover->SetPos( m_vPosBegin );
			g_xWSUserManager->AddSetPos( pMover, m_vPosBegin );
			m_tmReturnToBegin	= TIMEGETTIME;
		}
	}
	else
	{
		if( pMover->GetMovePattern() == 0 )	// 일반 패턴이동일때만 들어간다.
		{
			if( IsEndStop() )						// 제자리 대기 시간이 끝났으면
				MoveToRandom( STATE_IDLE );			// 다음 목표를 지정.
		}
		else								// 8자비행중		
		{
			if( pMover->GetMoveEvent() == 99 )		// 패턴비행 끝.
				pMover->SetMovePattern( 0 );		// 일반이동으로 전환.
		}

		// 루팅옵션이 있는 몹이다.
		if( pProp->m_nLoot )
		{
			if( (pMover->GetCount() & 63) == 0 )		// 가끔씩 주변을 스캔해서.(63이면 약 4.2초)
			{	
				if(  pProp->m_nLootProb == 0 || xRandom(100) < (DWORD)( pProp->m_nLootProb )  )	// 디폴트값(0)은 100% / 이거나 루팅확률에 들어가면 루팅시도
					if( m_bLootMove == FALSE )		// 루팅하러 가고 있을때 또 체크하면 안뒘
						SubItemLoot();		// 아이템을 루팅함.
			}
		}

		if( m_bLootMove == TRUE )
		{
			CCtrl *pCtrl = prj.GetCtrl( m_idLootItem );		// 그아이템이 사라졌을수 있으니까 검사함.
			if( IsInvalidObj(pCtrl) )		// 아이템 집으러 이동중에 아템이 없어지면
			{
				MoveToDst( pMover->GetPos() );	// 제자리에 섬.
				m_idLootItem = NULL_ID;
				m_bLootMove = FALSE;
			}
		}

		// -1 이면 아무것도 스캔하지 않는다!!!
		if( pProp->m_nScanJob != -1 )
		{
			CMover* pTarget = NULL;
			int nAttackFirstRange = pProp->m_nAttackFirstRange;
			//if( nAttackFirstRange > 10 || nAttackFirstRange < 0 )
			//{
			//	FLERROR_LOG( PROGRAM_NAME, _T( "m_nAttackFirstRange 이상:%d %s" ), pProp->m_nAttackFirstRange, pMover->GetName() );
			//	return TRUE;
			//}

			// 어떤 조건에 해당하는 플레이어가 있는지 스켄한다.
			//pTarget = ScanTargetAndSetTarget( nAttackFirstRange, pProp->m_nScanJob, pProp->m_dwScanQuestId, pProp->m_dwScanItemIdx, pProp->m_nScanChao );

			// 조건에 맞는 플레이어가 없고 선공몹이면 선공몹 형태의 스켄을 한다.
			if( pTarget == NULL && GetMover()->m_bActiveAttack )//&& GetMover()->IsRegionAttr( RA_SAFETY ) == FALSE )
				pTarget = ScanTargetAndSetTarget( nAttackFirstRange, JOB_ALL );

			if( pTarget && IsValidObj( (CObj*)pTarget ) )
			{
				if( (pMover->IsFlyingNPC() || pTarget->m_pActMover->IsFly() == FALSE) || pProp->dwClass == RANK_GUARD )	// 가드는 무조건 공격.
				{
					// 타겟이 존재하면 추적 모드로 변경 
					if( pTarget )	
					{
						g_xWSUserManager->AddDlgEmoticon( pMover, DLGEMOT_ATTACK );
						SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, pTarget->GetId() );
					}
				}
			}
		}	// if( scan )

	}
	return TRUE;
}

BOOL CAIAggroNormal::StateIdle( const AIMSG & msg )
{
	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();

	BeginAIHandler( )

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT ) 
//		if( pMover->TryDelete_IntoIdleMode() == TRUE )
//			return TRUE;

		int nRangeMove = (int)( RANGE_MOVE );
		if( pMover->IsFlyingNPC() )
			nRangeMove *= 3;

		D3DXVECTOR3	vTargetTo = pMover->GetPos() - m_vPosBegin;
		if( IsInRange( vTargetTo, (float)( nRangeMove ) ) == FALSE )
			DoReturnToBegin();
		SetStop( SEC( 0 ) );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT_TARGETCLEAR )
		SetTarget( NULL_ID, 0 );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_PROCESS ) 
		MoveProcessIdle( msg );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DAMAGE ) 
		//분노 모드로 변경

		if( m_bReturnToBegin == FALSE )
		{
			MoveToDst( msg.dwParam1 );
			SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, msg.dwParam1 );
			m_tmAttack	= TIMEGETTIME + s_tmAttack;		// 대기중일때 공격받으면 20초 타이머 시작

			m_vPosDamage	= pMover->GetPos();
		}
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DIE )		// 꽥!
		SendAIMsg( AIMSG_EXIT );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_COLLISION )  // 놀다가 충돌했음!"
		MoveToRandom( STATE_IDLE );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_ARRIVAL )
		if( pMover->IsFlyingNPC() )		
		{
			// 비행몹은 도착하고 나서 멈추지 않는다. 8자비행으로 이동패턴이 바뀐다.
			if( msg.dwParam2 == 1 )
			{
				int nPattern = 2;
				pMover->SetMovePattern( nPattern );
				// 클라로 전송
				g_xWSUserManager->AddSetMovePattern( pMover, nPattern, pMover->GetPos(), pMover->GetAngle(), pMover->GetAngleX() );
			}
		} 
		else
		{
			if( m_bReturnToBegin )
				SetStop( xRandom( SEC( 1 ) ) ); // 1초 동안 멈춰라 
			else
				SetStop( SEC( 5 ) + xRandom( SEC( 1 ) ) ); // 1초 동안 멈춰라 

			if( m_bLootMove )	// 아이템 루팅모드였음.
			{
				CCtrl *pCtrl = prj.GetCtrl( m_idLootItem );		// 그아이템이 사라졌을수 있으니까 검사함.
				if( IsValidObj(pCtrl) )
				{
					CItem *pItem = (CItem *)pCtrl;
					D3DXVECTOR3 vDist = pCtrl->GetPos() - pMover->GetPos();
					FLOAT fDistSq = D3DXVec3LengthSq( &vDist );
					if( fDistSq < 3.0f * 3.0f )		// 도착해서 거리 검사 한번더 해서 
					{
						if( pItem->IsDelete() )
							return TRUE;
						FLItemElem* pItemElem = (FLItemElem *)pItem->m_pItemBase;
						PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( pItem->m_dwIndex );
						BOOL bSuccess = FALSE;
						if( pItemProp->dwItemKind1 == IK1_GOLD )	// 주은아이템이 돈이냐?
						{
							pMover->AddGold( pItemElem->GetGold() );	// 돈증가.
							bSuccess = TRUE;
						} 
						else
						{
							// not GOLD
							bSuccess = pMover->CreateItem( pItemElem );			// 주은 아이템 생성.
						}

						if( bSuccess )
						{
							pItem->Delete();		// 인벤에까지 넣는데 성공하면 바닥의 아이템은 지움.
						}
					}
				}
				m_bLootMove = FALSE;		// this가 아이템 집는중이라는거 해제.
				m_idLootItem = NULL_ID;
			}
		}
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_EXIT )	

	EndAIHandler( )
	return TRUE;
}

BOOL CAIAggroNormal::StateWander( const AIMSG & /*msg*/ )
	{	return TRUE;	}
BOOL CAIAggroNormal::StateEvade( const AIMSG & /*msg*/ )
	{	return TRUE;	}


BOOL CAIAggroNormal::StateStand( const AIMSG & msg )
{
	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();

	BeginAIHandler( )

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT ) 
		D3DXVECTOR3	vTargetTo = pMover->GetPos() - m_vPosBegin;
		int nRangeMove = (int)( RANGE_MOVE );
		if( pMover->IsFlyingNPC() )
			nRangeMove *= 3;
		if( IsInRange( vTargetTo, (float)( nRangeMove ) ) == FALSE )
			DoReturnToBegin();
		
		SetStop( SEC( 0 ) );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT_TARGETCLEAR )
		SetTarget( NULL_ID, 0 );
			
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_PROCESS ) 
		MoveProcessStand( msg );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DAMAGE ) 
		//분노 모드로 변경

		if( m_bReturnToBegin == FALSE )
		{
			MoveToDst( msg.dwParam1 );
			SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, msg.dwParam1 );
			m_tmAttack	= TIMEGETTIME + s_tmAttack;		// 대기중일때 공격받으면 20초 타이머 시작

			m_vPosDamage	= pMover->GetPos();
		}


	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DIE )		// 꽥!
		SendAIMsg( AIMSG_EXIT );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_COLLISION )  // 놀다가 충돌했음!"
//		MoveToRandom( STATE_IDLE );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_ARRIVAL )
		if( pMover->IsFlyingNPC() )		
		{
			// 비행몹은 도착하고 나서 멈추지 않는다. 8자비행으로 이동패턴이 바뀐다.
			if( msg.dwParam2 == 1 )
			{
				int nPattern = 2;
				pMover->SetMovePattern( nPattern );
				// 클라로 전송
				g_xWSUserManager->AddSetMovePattern( pMover, nPattern, pMover->GetPos(), pMover->GetAngle(), pMover->GetAngleX() );
			}
		} 
		else
		{
			if( m_bReturnToBegin )
			{
				SetStop( xRandom( SEC( 1 ) ) ); // 1초 동안 멈춰라 
			}
			else
			{
				SetStop( SEC( 5 ) + xRandom( SEC( 1 ) ) ); // 1초 동안 멈춰라 
			}

			if( m_bLootMove )	// 아이템 루팅모드였음.
			{
				CCtrl *pCtrl = prj.GetCtrl( m_idLootItem );		// 그아이템이 사라졌을수 있으니까 검사함.
				if( IsValidObj(pCtrl) )
				{
					CItem *pItem = (CItem *)pCtrl;
					D3DXVECTOR3 vDist = pCtrl->GetPos() - pMover->GetPos();
					FLOAT fDistSq = D3DXVec3LengthSq( &vDist );
					if( fDistSq < 3.0f * 3.0f )		// 도착해서 거리 검사 한번더 해서 
					{
						if( pItem->IsDelete() )
							return TRUE;
						FLItemElem* pItemElem = (FLItemElem *)pItem->m_pItemBase;
						PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( pItem->m_dwIndex );
						BOOL bSuccess = FALSE;
						if( pItemProp->dwItemKind1 == IK1_GOLD )	// 주은아이템이 돈이냐?
						{
							pMover->AddGold( pItemElem->GetGold() );	// 돈증가.
							bSuccess = TRUE;
						} 
						else
						{
							// not GOLD
							bSuccess = pMover->CreateItem( pItemElem );			// 주은 아이템 생성.
						}

						if( bSuccess )
						{
							pItem->Delete();		// 인벤에까지 넣는데 성공하면 바닥의 아이템은 지움.
						}
					}
				}
				m_bLootMove = FALSE;		// this가 아이템 집는중이라는거 해제.
				m_idLootItem = NULL_ID;
			}
		}
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_EXIT )	

	EndAIHandler( )
	return TRUE;
}
BOOL CAIAggroNormal::StatePatrol( const AIMSG & msg )
{
	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();

	BeginAIHandler( )

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT ) 
		D3DXVECTOR3	vTargetTo	= pMover->GetPos() - m_vPosBegin;
		int nRangeMove	= (int)( RANGE_MOVE );
		if( pMover->IsFlyingNPC() )
			nRangeMove	*= 3;
		if( IsInRange( vTargetTo, (float)( nRangeMove ) ) == FALSE )
			DoReturnToBegin();
		SetStop( SEC( 0 ) );
		if( pMover->m_dwPatrolIndex != NULL_ID && m_bReturnToBegin == FALSE )
		{
			CPatrolPath::GetInstance()->GetNextPosInfo( (CObj*)pMover, pMover->GetPos(), m_vDestPos, m_vecPatrolInfo );
			g_xWSUserManager->AddMotionArrive( pMover, (m_vecPatrolInfo.m_dwDelay == 0 ) ? OBJMSG_STOP_RUN: OBJMSG_STOP );
			m_vPosBegin	= m_vDestPos;			
		}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT_TARGETCLEAR )
		SetTarget( NULL_ID, 0 );
			
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_PROCESS ) 
		MoveProcessPatrol( msg );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_TIMEOUT )
		
		
	///////////////////////////////////////////////////////////////////////////////////////////////////			
	OnMessage( AIMSG_DAMAGE ) 
		//분노 모드로 변경

		if( m_bReturnToBegin == FALSE )
		{
			MoveToDst( msg.dwParam1 );
			SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, msg.dwParam1 );
			m_tmAttack	= TIMEGETTIME + s_tmAttack;		// 대기중일때 공격받으면 20초 타이머 시작

			m_vPosDamage	= pMover->GetPos();
		}


	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DIE )		// 꽥!
		SendAIMsg( AIMSG_EXIT );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_COLLISION )  // 경로로 움직이다가 충돌했음!"

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_ARRIVAL )
	{
		g_xWSUserManager->AddMotionArrive( pMover, (m_vecPatrolInfo.m_dwDelay == 0 ) ? OBJMSG_STOP_RUN: OBJMSG_STOP );
		SetStop( SEC(m_vecPatrolInfo.m_dwDelay) );
		D3DXVECTOR3 v3Pos = m_vDestPos;
//		FLTRACE_LOG( PROGRAM_NAME, _T( "Before = %d\t After = %d" ), pMover->m_dwPatrolIndexCount, pMover->m_dwPatrolIndexCount+1 );		
		CPatrolPath::GetInstance()->GetNextPosInfo( (CObj*)pMover, v3Pos, m_vDestPos, m_vecPatrolInfo );
//		CPatrolPath::GetInstance()->GetNextPosInfo( pMover->m_dwPatrolIndex, pMover->m_dwPatrolIndexCount, v3Pos, m_vDestPos, m_vecPatrolInfo );
		m_vPosBegin = m_vDestPos;			
	}
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_EXIT )	

	EndAIHandler( )
	return TRUE;
}

BOOL CAIAggroNormal::MoveProcessStand( const AIMSG & /*msg*/ )
{
	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();
	MoverProp *pProp = pMover->GetProp();

	// 데미지 상태거나 죽었을 경우 이동 처리 수행 않음 
	if( pMover->IsDie() || (pMover->m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL) )
		return FALSE;

	// 스폰 장소로 귀환 
	if( m_bReturnToBegin )
	{
		if( IsInRange( m_vPosBegin - pMover->GetPos(), 0.1f ) )		// 시작지점으로 돌아왔다.
		{
			DoReturnToBegin( FALSE );
			pMover->SetAngle( m_fAngleBegine );
			g_xWSUserManager->AddMoverBehavior( pMover );
			SendAIMsg( AIMSG_ARRIVAL, NULL_ID );
		}

		if( TIMEGETTIME - m_tmReturnToBegin >= SEC(10) )	// 집으로 돌아가기 시작한지 10초나 지났다.
		{
			// 제대로 못돌아가고 있다고 판단하고 시작지점으로 워프시킴.
			pMover->SetPos( m_vPosBegin );
			g_xWSUserManager->AddSetPos( pMover, m_vPosBegin );
			m_tmReturnToBegin	= TIMEGETTIME;
		}
	}
	else
	{
		if( pMover->GetMovePattern() == 0 )	// 일반 패턴이동일때만 들어간다.
		{
		}
		else								// 8자비행중		
		{
			if( pMover->GetMoveEvent() == 99 )		// 패턴비행 끝.
				pMover->SetMovePattern( 0 );		// 일반이동으로 전환.
		}

		// 루팅옵션이 있는 몹이다.
		if( pProp->m_nLoot )
		{
			if( (pMover->GetCount() & 63) == 0 )		// 가끔씩 주변을 스캔해서.(63이면 약 4.2초)
			{	
				if( pProp->m_nLootProb == 0 || (int)( xRandom(100) ) < pProp->m_nLootProb  )	// 디폴트값(0)은 100% / 이거나 루팅확률에 들어가면 루팅시도
					if( m_bLootMove == FALSE )		// 루팅하러 가고 있을때 또 체크하면 안뒘
						SubItemLoot();		// 아이템을 루팅함.
			}
		}

		if( m_bLootMove == TRUE )
		{
			CCtrl *pCtrl = prj.GetCtrl( m_idLootItem );		// 그아이템이 사라졌을수 있으니까 검사함.
			if( IsInvalidObj(pCtrl) )		// 아이템 집으러 이동중에 아템이 없어지면
			{
				MoveToDst( pMover->GetPos() );	// 제자리에 섬.
				m_idLootItem = NULL_ID;
				m_bLootMove = FALSE;
			}
		}

		// -1 이면 아무것도 스캔하지 않는다!!!
		if( pProp->m_nScanJob != -1 )
		{
			CMover* pTarget = NULL;
			int nAttackFirstRange = pProp->m_nAttackFirstRange;
			//if( nAttackFirstRange > 10 || nAttackFirstRange < 0 )
			//{
			//	FLERROR_LOG( PROGRAM_NAME, _T( "m_nAttackFirstRange 이상:%d %s" ), pProp->m_nAttackFirstRange, pMover->GetName() );
			//	return TRUE;
			//}

			// 어떤 조건에 해당하는 플레이어가 있는지 스켄한다.
			if( pProp->m_nScanJob || pProp->m_dwScanQuestId || pProp->m_dwScanItemIdx || pProp->m_nScanChao )
				pTarget = ScanTargetAndSetTarget( nAttackFirstRange, pProp->m_nScanJob, pProp->m_dwScanQuestId, pProp->m_dwScanItemIdx, pProp->m_nScanChao );

			// 조건에 맞는 플레이어가 없고 선공몹이면 선공몹 형태의 스켄을 한다.
			if( pTarget == NULL && GetMover()->m_bActiveAttack )
				pTarget = ScanTargetAndSetTarget( nAttackFirstRange, JOB_ALL );

			if( pTarget && IsValidObj( (CObj*)pTarget ) )
				{
					if( (pMover->IsFlyingNPC() || pTarget->m_pActMover->IsFly() == FALSE) || pProp->dwClass == RANK_GUARD )	// 가드는 무조건 공격.
					{
						// 타겟이 존재하면 추적 모드로 변경 
						if( pTarget )	
						{
							g_xWSUserManager->AddDlgEmoticon( pMover, DLGEMOT_ATTACK );
							SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, pTarget->GetId() );
						}
					}
				}
		}	// if( scan )

	}
	return TRUE;
}
BOOL CAIAggroNormal::MoveProcessPatrol( const AIMSG & /*msg*/ )
{
	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();
	MoverProp *pProp = pMover->GetProp();

	// 데미지 상태거나 죽었을 경우 이동 처리 수행 않음 
	if( pMover->IsDie() || (pMover->m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL) )
		return FALSE;

	// 스폰 장소로 귀환 
	if( m_bReturnToBegin )
	{
		if( IsInRange( m_vPosBegin - pMover->GetPos(), 3.0f ) )		// 시작지점으로 돌아왔다.
		{
			DoReturnToBegin( FALSE );
//			SendAIMsg( AIMSG_ARRIVAL, NULL_ID );
		}

		if( TIMEGETTIME - m_tmReturnToBegin >= SEC(10) )	// 집으로 돌아가기 시작한지 10초나 지났다.
		{
			// 제대로 못돌아가고 있다고 판단하고 시작지점으로 워프시킴.
			pMover->SetPos( m_vPosBegin );
			g_xWSUserManager->AddSetPos( pMover, m_vPosBegin );
			m_tmReturnToBegin	= TIMEGETTIME;
		}
	}
	else
	{
		if( pMover->GetMovePattern() == 0 )	// 일반 패턴이동일때만 들어간다.
		{
			D3DXVECTOR3 vPos1 = pMover->GetPos();
			D3DXVECTOR3 vPos2 = m_vDestPos;

			if( IsEndStop() )
			{
				MoveToDst( m_vDestPos );
			}
		}
		else								// 8자비행중		
		{
			if( pMover->GetMoveEvent() == 99 )		// 패턴비행 끝.
				pMover->SetMovePattern( 0 );		// 일반이동으로 전환.
		}

		// 루팅옵션이 있는 몹이다.
		if( pProp->m_nLoot )
		{
			if( (pMover->GetCount() & 63) == 0 )		// 가끔씩 주변을 스캔해서.(63이면 약 4.2초)
			{	
				if( pProp->m_nLootProb == 0 || (int)( xRandom(100) ) < pProp->m_nLootProb  )	// 디폴트값(0)은 100% / 이거나 루팅확률에 들어가면 루팅시도
					if( m_bLootMove == FALSE )		// 루팅하러 가고 있을때 또 체크하면 안뒘
						SubItemLoot();		// 아이템을 루팅함.
			}
		}

		if( m_bLootMove == TRUE )
		{
			CCtrl *pCtrl = prj.GetCtrl( m_idLootItem );		// 그아이템이 사라졌을수 있으니까 검사함.
			if( IsInvalidObj(pCtrl) )		// 아이템 집으러 이동중에 아템이 없어지면
			{
				MoveToDst( pMover->GetPos() );	// 제자리에 섬.
				m_idLootItem = NULL_ID;
				m_bLootMove = FALSE;
			}
		}

		// -1 이면 아무것도 스캔하지 않는다!!!
		if( pProp->m_nScanJob != -1 )
		{
			CMover* pTarget = NULL;
			int nAttackFirstRange = pProp->m_nAttackFirstRange;
			//if( nAttackFirstRange > 10 || nAttackFirstRange < 0 )
			//{
			//	FLERROR_LOG( PROGRAM_NAME, _T( "m_nAttackFirstRange 이상:%d %s" ), pProp->m_nAttackFirstRange, pMover->GetName() );
			//	return TRUE;
			//}

			// 어떤 조건에 해당하는 플레이어가 있는지 스켄한다.
			if( pProp->m_nScanJob || pProp->m_dwScanQuestId || pProp->m_dwScanItemIdx || pProp->m_nScanChao )
				pTarget = ScanTargetAndSetTarget( nAttackFirstRange, pProp->m_nScanJob, pProp->m_dwScanQuestId, pProp->m_dwScanItemIdx, pProp->m_nScanChao );

			// 조건에 맞는 플레이어가 없고 선공몹이면 선공몹 형태의 스켄을 한다.
			if( pTarget == NULL && GetMover()->m_bActiveAttack )
				pTarget = ScanTargetAndSetTarget( nAttackFirstRange, JOB_ALL );

			if( pTarget && IsValidObj( (CObj*)pTarget ) )
			{
				if( (pMover->IsFlyingNPC() || pTarget->m_pActMover->IsFly() == FALSE) || pProp->dwClass == RANK_GUARD )	// 가드는 무조건 공격.
				{
					// 타겟이 존재하면 추적 모드로 변경 
					if( pTarget )	
					{
						g_xWSUserManager->AddDlgEmoticon( pMover, DLGEMOT_ATTACK );
						SendAIMsg( AIMSG_SETSTATE, STATE_RAGE_PATROL, pTarget->GetId() );
					}
				}
			}
		}	// if( scan )

	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_ARRAY 40

void CAIAggroNormal::CallHelper( const MoverProp* pMoverProp ) 
{
	CMover* pMover = GetMover();
	D3DXVECTOR3 vPos = pMover->GetPos();
	int anCountMonster[ 5 ];
	int nCountPlayer = 0;

	ZeroMemory( anCountMonster, sizeof( anCountMonster ) );

	CMover* pTarget = NULL;
	CParty* pParty = NULL;
	BOOL bParty = FALSE;

	CObj* pObj;

	CMover* apMonster[ MAX_ARRAY ][ 5 ];
	CMover* apPlayer[ MAX_ARRAY ];

	///////////////////////////////////////////////////////

	// 해당 조건에 맞는 몬스터를 수집한다.
	int nRange = 20;
	switch( pMoverProp->m_nHelpRangeMul )
	{
	case 0: nRange = pMoverProp->m_nAttackFirstRange / 2;	break;
	default: nRange = pMoverProp->m_nAttackFirstRange * pMoverProp->m_nHelpRangeMul;	break;	// 시야범위의 배수.
	}

	FOR_LINKMAP( pMover->GetWorld(), vPos, pObj, nRange / MPU, CObj::linkDynamic, pMover->GetLayer() )
	{
		if( pObj != pMover && pObj->GetType() == OT_MOVER )
		{
			// FOR_LINKMAP 자체가 느리기 때문에 이 루프를 안쪽에 넣었다.
			for( int i = 0; i < pMoverProp->m_nCallHelperMax; i++ )
			{
				int nCallHelperIdx = pMoverProp->m_nCallHelperIdx[ i ];
				UNREFERENCED_PARAMETER( nCallHelperIdx );

				int nCallHelperNum = pMoverProp->m_nCallHelperNum[ i ];
				// 아무나 부르는것이거나 || 같은 종족만 부르는 옵션일때 같은 인덱스인가?
				// 그리고.............
				if( (pMoverProp->m_bHelpWho == 1 || (pMoverProp->m_bHelpWho == 2 && pObj->GetIndex() == pMover->GetIndex())) &&
					( anCountMonster[ i ] < nCallHelperNum || nCallHelperNum == 0 )	)	
				{
					apMonster[ anCountMonster[ i ] ][ i ] = (CMover*)pObj;
					anCountMonster[ i ]++;
					if( anCountMonster[ i ] >= MAX_ARRAY ) 
						goto NEXT;
				}
			}
		}
	}
	END_LINKMAP
NEXT:
	// 파티 가능 여부 
	pTarget = prj.GetMover( m_dwIdTarget );
	for( int i = 0; i < pMoverProp->m_nCallHelperMax; i++ )
		bParty |= pMoverProp->m_bCallHelperParty[ i ] ? TRUE : FALSE;

	if( bParty && IsValidObj( (CObj*)pTarget ) )
	{
		pParty = g_PartyMng.GetParty( pTarget->m_idparty );
		// 파티가 확실하다면 파티원을 apPlayer에 수집한다.
		if( pParty )
		{
			FOR_LINKMAP( pMover->GetWorld(), vPos, pObj, 20 / MPU, CObj::linkPlayer, pMover->GetLayer() )
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
	for( int i = 0; i < pMoverProp->m_nCallHelperMax; i++ )
	{
		if( pParty  )
		{
			if( nCountPlayer )
			{
				for( int j = 0; j < anCountMonster[ i ]; j++ )
				{
					// 각각의 몬스터에게 타겟을 랜덤으로 배정한다. 
					CMover* pNewTarget = (CMover*)apPlayer[ rand() % nCountPlayer ];
					if( pNewTarget )
					{
						apMonster[ j ][ i ]->PostAIMsg( AIMSG_SETSTATE, STATE_RAGE, pNewTarget->GetId() );
					}
				}
			}
		}
		else
		{ 
			for( int j = 0; j < anCountMonster[ i ]; j++ )
			{
				apMonster[ j ][ i ]->PostAIMsg( AIMSG_SETSTATE, STATE_RAGE, m_dwIdTarget );
			}
		}
	}
}

DWORD CAIAggroNormal::GetAtkMethod_Far()
{
	CMover* pMover = GetMover();
//	MoverProp* pMoverProp = pMover->GetProp();
	DWORD dwRandom = xRandom( 100 );

	// 필살 판정 성공!
	int nDeathblow = (int)( pMover->GetHitPointPercent() / 4.5f );
	if( nDeathblow )
	{
		if( !xRandom( nDeathblow ) )
			return 3;
	}

	switch( pMover->m_dwBelligerence )
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
DWORD CAIAggroNormal::GetAtkMethod_Near()
{
	CMover* pMover = GetMover();
//	MoverProp* pMoverProp = pMover->GetProp();
	DWORD dwRandom = xRandom( 100 );
	int nDeathblow = (int)( pMover->GetHitPointPercent() / 4.5f );
	if( nDeathblow )
	{
		if( !xRandom( nDeathblow ) )
			return 3;
	}

	switch( pMover->m_dwBelligerence )
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
DWORD CAIAggroNormal::GetAtkRange( DWORD dwAtkMethod )
{
	PT_ITEM_SPEC pItemProp = GetAtkProp( dwAtkMethod );
#ifdef _DEBUG
	if( pItemProp == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "SkillProp->dwAtk%d이 NULL이다. (%s)" ), dwAtkMethod, GetMover()->GetName() );
	}
#endif
	return pItemProp->dwAttackRange;
}

PT_ITEM_SPEC CAIAggroNormal::GetAtkProp( DWORD dwAtkMethod )
{
	return g_xSpecManager->GetSpecItem( GetAtkItemId( dwAtkMethod ) );
}

DWORD CAIAggroNormal::GetAtkItemId( DWORD dwAtkMethod )
{
	CMover* pMover = GetMover();
	switch( dwAtkMethod )
	{ 
	case 1:
		return pMover->GetProp()->dwAtk1;
	case 2:
		return pMover->GetProp()->dwAtk2;
	case 3:
		return pMover->GetProp()->dwAtk3;
	}
	return 0;
}

void CAIAggroNormal::DoAttack( DWORD dwAtkMethod, CMover* pTarget )
{
	CMover* pMover = GetMover();
	if( IsInvalidObj( pMover ) || dwAtkMethod == 0 )	
		return;

	DWORD dwAtkItem = GetAtkItemId( dwAtkMethod );
	PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( dwAtkItem );
	if( pItemProp == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s %d %d" ), pMover->GetName(), dwAtkMethod, dwAtkItem );
		return;
	}
	
	if( pItemProp->dwAttackRange == AR_RANGE || pItemProp->dwAttackRange == AR_WAND )
	{
		// 장거리 공격은, 선택된 공격 방식(dwAtkMethod)의 해당 공격 아이템 속성에서 속성을 얻어 장거리 공격이라고 판단함.
		// 서버에서 idSfx를 만들어서 자기자신에게 등록시켜둠. 나중에 클라에서 맞아서 서버로 정보가 넘어오면 그것과 비교해서 데미지를 넘겨줌.
		int id = pMover->m_sfxHitArray.Add( 0, pTarget->GetId(), AF_GENERIC, 0 );	// 이부분은 DoAttackRange안으로 들어가야 할거 같다.
		DWORD dwItemID = dwAtkItem;
		if( dwItemID == NULL_ID )
			FLERROR_LOG( PROGRAM_NAME, _T( "경고 : %s의 dwAtk2가 지정되지 않았다" ), pMover->GetName() );

		if( pMover->DoAttackRange( pTarget, dwItemID, id ) != 1 )
			pMover->m_sfxHitArray.RemoveSfxHit( id, FALSE );	// sfxHitArray.Add넣었던것을 다시뺌. 
	}
	else
	{
		// 필살 공격은, 프로퍼티에서 정보를 얻어올 수 없으므로 공격 방식(dwAtkMethod)으로 결정.
		// 필살 공격 루틴은 내부에서 단거리 공격을 할 수도 있다.
		DWORD dwItemID = dwAtkItem;
		if( dwItemID == NULL_ID )
			FLERROR_LOG( PROGRAM_NAME, _T( "경고 : %s의 공격%d가 지정되지 않았다" ), pMover->GetName(), dwAtkMethod );
		if( dwAtkMethod == 3 )
			pMover->DoAttackSP( pTarget, dwItemID );
		else
		{
			pMover->DoAttackMelee( pTarget, OBJMSG_ATK1 );
// 			int nError = pMover->SendActMsg( OBJMSG_ATK1, pTarget->GetId(), 0, 0 );
// 			if( nError > 0 ) 
// 				g_xWSUserManager->AddMeleeAttack( pMover, OBJMSG_ATK1, pTarget->GetId(), 0, MAKELONG( 0, (WORD)nError ) );
// 			else
// 				nError = -1;		// 브레이크 포인트 잡으려고 해놓은거.
		}
	}

	m_tmAttack	= TIMEGETTIME + s_tmAttack;		// 공격 후 s_tmAttack초간 추적해옴.
}

//
// 어택 딜레이가 끝난후 다시 어택찬스가 돌아올때.
// 11월 신버전코드입니다. 구버전은 여기다 코딩하지 마셈~ -xuzhu-
BOOL CAIAggroNormal::SubAttackChance( const AIMSG & /*msg*/, CMover *pTarget )
{
	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();
	MoverProp *pProp = pMover->GetProp();
	int		nTargetJob = 0;
	if( pTarget->IsPlayer() )
		nTargetJob = pTarget->GetJob();		// 타겟의 직업.
	
	BOOL bAttack = TRUE;
	D3DXVECTOR3	vTargetTo = pTarget->GetPos() - pMover->GetPos();	
	
//	BOOL bRangeAttack = (pProp->m_bRangeAttack[ nTargetJob ] & 0x7F) ? TRUE : FALSE;	// 장거리 공격 AI가 있냐.

	if( pProp->m_bRecvCond )	// 회복 AI가 있느냐
	{
		if( pMover->GetHitPointPercent( 100 ) <= pProp->m_nRecvCondMe )	// HP가 m_nRecvCondMe% 이하로 떨어졌는가?
		{
			if( pMover->GetManaPoint() > 0 )
			{
				// m_nHow% 만큼의 HP를 회복
				int nHP = (int)(pMover->GetMaxHitPoint() * (pProp->m_nRecvCondHow / 100.0f));	
				pMover->IncHitPoint( nHP );
				
				g_xWSUserManager->AddCreateSfxObj( pMover, XI_INDEX( 62, XI_GEN_CURE01 ), pMover->GetPos().x, pMover->GetPos().y, pMover->GetPos().z );	
				if( pProp->m_nRecvCondMP > 0 )		
				{
					// 마나를 소모.
					int nMP = pProp->m_nRecvCondMP;			
					pMover->IncManaPoint( -nMP );
				} 
				else
				{
					// 0으로 지정되어 있으면 몽땅 소모.
					int nMP = pMover->GetMaxManaPoint();	
					pMover->IncManaPoint( -nMP );
				}
				bAttack = FALSE;		// 회복을 했으면 공격은 다음턴에.
			}
		}
		m_tmAttackDelay = TIMEGETTIME + SEC( 3 );	// 3초후에 다시 공격기회.
	}

	if( bAttack )
	{
		GetAggroMng()->SumFirstTargetAggro( *GetMover(), pTarget->GetId() );

		if( m_bRangeAttack )
			m_dwAtkMethod = GetAtkMethod_Far();		// 공격 방법 세팅 
		else
			m_dwAtkMethod = GetAtkMethod_Near();		// 공격 방법 세팅 

		DWORD dwAtkRange = GetAtkRange( m_dwAtkMethod );
		// 장거리 공격 선택이라면?
		if( dwAtkRange == AR_RANGE || dwAtkRange == AR_WAND )
		{
			// 유효 거리 판단
			FLOAT fRange = (float)(pProp->m_bRangeAttack[ nTargetJob ] & 0x7F);
			fRange += 1.0f;		// 사거리 +1미터 정도는 걍 쏜다.
			if( IsInRange( vTargetTo, fRange ) == TRUE )		// AI에서 지정한 공격거리까지 다가갔는가?
			{
				DoAttack( m_dwAtkMethod, (CMover*)pTarget );	// 공격
				m_tmAttackDelay	= TIMEGETTIME + SEC( 3 );		// 장거리 상태이다. 3초 공격 딜레이 설정.
			}
		} 
		else
		{
			FLOAT fRange = pMover->GetAttackRange( dwAtkRange );
			if( pMover->IsRangeObj( pTarget, fRange ) )
			{
				DoAttack( m_dwAtkMethod, pTarget );		//  공격
				m_bGoTarget = TRUE;  // <- 이건 뭐하는거? 
				// 공격을 했으면 어택 딜레이 리셋.
				int nDelay = (int)pProp->dwReAttackDelay - SEC( 1 )  + xRandom( SEC( 2 ) );
				m_tmAttackDelay		= TIMEGETTIME + nDelay;
				FLTRACE_LOG( PROGRAM_NAME, _T( "AtkDelay:%d" ), (int)nDelay );
			}
		}
		// 근접공격은 ARRIVAL에서 처리한다. 여기서 처리해도 좋지 않을까?
	}
	
	return TRUE;
}


//
// AI에 따른 공격방식 선택.
//
// return : 0:근거리 1:원거리
int CAIAggroNormal::SelectAttackType( CMover *pTarget )
{
	int nAttackType = 0;

	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();
	MoverProp *pProp = pMover->GetProp();
	
	int		nTargetJob = 0;
	int		nRange = pProp->m_bRangeAttack[ nTargetJob ] & 0x7F;
	
	if( pTarget->IsPlayer() )		nTargetJob = pTarget->GetJob();		// 타겟의 직업.

	BOOL bRange = ( pProp->m_bRangeAttack[ nTargetJob ] ) ? TRUE : FALSE;	// AI에 거리가 있으면 원거리 AI있음.
	BOOL bMelee = (BOOL)pProp->m_bMeleeAttack;								// AI에 근거리공격도 있는지.


	if( bRange && bMelee )		// 원거리/근거리 둘다 가지고 있음
	{
		D3DXVECTOR3	vDist = pTarget->GetPos() - pMover->GetPos();	// 타겟과의 거리.
		FLOAT		fDistSq = D3DXVec3LengthSq( &vDist );

		int nHalfRange = nRange / 2;		// 사거리의 반.
		if( fDistSq < (nHalfRange * nHalfRange) )	// 너무 가까이 붙어있어 거리유지가 히들면.
			nAttackType = 0;	// 근접공격으로 하자.
		else
			nAttackType = 1;	// 거리에 여유가 있으므로 원거리 공격.
	} else
	if( bRange && bMelee == FALSE )		// 원거리만 가지고 있음
	{
		nAttackType = 1;
	} else
	if( bRange == FALSE && bMelee )		// 근거리만 가지고 있음
	{
		nAttackType = 0;
	}

	return nAttackType;
}

// SummonAI가 있을때 소환을 시도한다. xx초 마다.
void CAIAggroNormal::SubSummonProcess( CMover * /*pTarget*/ )
{
	if( TIMEGETTIME - m_tmSummon < SEC(10) )		// 20초마다 한번씩 소환을 시도.
		return;

	m_tmSummon = TIMEGETTIME;

	CMover* pMover = GetMover();
	CWorld* pWorld = GetWorld();
	MoverProp *pProp = pMover->GetProp();
	// 소환 AI
	if( pProp && pProp->m_nSummProb )	// 값이 있으면 소환 AI가 있다.
	{
		for( vector<OBJID>::iterator it = m_vecIdSummon.begin(); it!=m_vecIdSummon.end(); )
		{
			CMover *pZako = prj.GetMover( (*it) );
			if( IsValidObj(pZako) && pZako->IsLive() )	// 그넘들이 살아있는지까지도 검사해야한다.
				it++;
			else
				it = m_vecIdSummon.erase( it );
		}
		
		if( pProp->m_nSummNum > (int)( m_vecIdSummon.size() ) )
		{
			if( (int)( xRandom(100) ) < pProp->m_nSummProb )	// 소환 시도.
			{
				D3DXVECTOR3 vLocal, vPos;
				CObj* pObj	= CreateObj( D3DDEVICE, OT_MOVER, (DWORD)pProp->m_nSummID );
				if( NULL == pObj )	
				{
					FLERROR_LOG( PROGRAM_NAME, _T( "소환 실패 %d" ), pProp->m_nSummID );
					return;
				}
				vLocal.x = 1.5f - xRandomF( 3.0f );		// 보스 주위에 랜덤으로 생성.
				vLocal.y = 0;
				vLocal.z = 1.5f - xRandomF( 3.0f );
				vPos = pMover->GetPos();
				vPos += vLocal;
				pObj->SetPos( vPos );
				pObj->InitMotion( MTI_STAND );
				pObj->UpdateLocalMatrix();
				((CMover*)pObj)->m_bActiveAttack = TRUE;	// 부하는 선공 형으로 소환하자.
				((CMover*)pObj)->AddItToGlobalId();
				if( pWorld->ADDOBJ( pObj, FALSE, pMover->GetLayer() ) == TRUE )
				{
					m_vecIdSummon.push_back( ((CMover *)pObj)->GetId() );
					g_xWSUserManager->AddDlgEmoticon( pMover, DLGEMOT_SUMMON );
				}
				else
				{
					SAFE_DELETE( pObj );
				}
			}
		}
	}
}




BOOL CAIAggroNormal::MoveProcessRage( const AIMSG & msg )
{
	CMover* pMover = GetMover();
	CWorld* pWorld = GetWorld();
	MoverProp *pProp = pMover->GetProp();
	
	// 데미지 상태거나 죽었을 경우 이동 처리 수행 않음 
	if( pMover->IsDie() || (pMover->m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL) )
		return FALSE;
	
	CMover* pTarget = ScanTargetAndSetTarget( CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon( pWorld->GetID() ) ? 160 : 15, 
		pProp->m_nScanJob, pProp->m_dwScanQuestId, pProp->m_dwScanItemIdx, pProp->m_nScanChao ); 

	DWORD dwAIState = pMover->m_dwAIInterfaceState;

	if( IsValidObj( pTarget ) == FALSE || ((CMover*)pTarget)->IsDie() || pTarget->GetWorld() != pMover->GetWorld() )	
	{
		GetAggroMng()->RemoveMover( /**GetMover(),*/ m_dwIdTarget );

//		int nAttackFirstRange	= pProp->m_nAttackFirstRange;

		CMover* pTarget	= NULL;
		if( CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon( pWorld->GetID() ) )
			pTarget = ScanTargetAndSetTarget( 160/*, m_dwIdTarget, m_uParty*/ );
		else
			pTarget = ScanTargetAndSetTarget( 15/*, m_dwIdTarget, m_uParty*/ );

		if( pTarget	== NULL )
		{
			if( dwAIState == STATE_STAND )
				DoReturnToBegin();	// SetTarget( NULL_ID, 0 );
			SendAIMsg( AIMSG_SETSTATE, dwAIState );
			return TRUE;
		}
		else
		{
			SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, pTarget->GetId() );
			return TRUE;
		}
	}

	if( pTarget->IsRegionAttr( RA_SAFETY ) && pProp->dwClass != RANK_GUARD )	// 타겟 안전 영역 內
	{
		DoReturnToBegin();
		SendAIMsg( AIMSG_SETSTATE, dwAIState );
		return TRUE;
	}

	if( pMover->IsFlyingNPC() != ((CMover*)pTarget)->m_pActMover->IsFly() )	// this가 비행형이 아니고 상대가 비행중이라면
	{
		if( dwAIState == STATE_STAND )
			DoReturnToBegin();
		
		SendAIMsg( AIMSG_SETSTATE, dwAIState );
		return TRUE;
	}

	if( TIMEGETTIME > m_tmAttack )	// 공격 못한지 20초 지났냐?
	{
		m_tmAttack	= TIMEGETTIME + s_tmAttack;	// 타이머 리셋
		// 5초전 좌표와 지금 좌표의 거리를 계산.
		D3DXVECTOR3 vDist = pMover->GetPos() - m_vOldPos;
		vDist.y	= 0;	// 옆으로 살짝움직였는데 산위로 확 올라갔을땐 거리가 엄청 멀수 있다. 그래서 이렇게 한것.
		if( IsInRange( vDist, 2.0f ) )	// 2미터를 못벗어났다면 어딘가에 갖혔다는 뜻.
		{
			if( pMover->GetSpeed( pMover->m_pActMover->m_fSpeed ) > 0 )		// 마법적으로 움직이지 못하게 된상태에선 워프하면 안됨.
			{
				D3DXVECTOR3 vTarget	= pTarget->GetPos();
				pMover->SetPos( vTarget );	// 갖혔으면 타겟쪽으로 바로 워프.
				g_xWSUserManager->AddSetPos( pMover, pMover->GetPos() );
			} 
		} 
		m_vOldPos = pMover->GetPos();	// 현재 좌표를 저장함.
	}

	D3DXVECTOR3	vTargetTo = pTarget->GetPos() - pMover->GetPos();	

	// 마지막 피격 위치로 부터 40 미터 떨어지면 생성 위치로 돌아간다.
	// 생성 위치로 부터 40 + 15 OR 45 미터 떨어지면 생성 위치로 돌아간다.
	D3DXVECTOR3 v1	= pMover->GetPos() - m_vPosBegin;
	
	FLOAT fRange	= RANGE_MOVE;
	if( pMover->IsFlyingNPC() )
		fRange	*= 3.0F;
	
	if( IsInRange( v1, RANGE_RETURN_TO_BEGIN + fRange ) == FALSE && !CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon( pWorld->GetID() ) )
	{
		DoReturnToBegin();
		SendAIMsg( AIMSG_SETSTATE, dwAIState );
		return TRUE;
	}
	if( m_vPosDamage == D3DXVECTOR3( 0, 0, 0 ) )	// 선공 몬스터 적 검색 직후
		m_vPosDamage	= pMover->GetPos();

	D3DXVECTOR3 v2	= pMover->GetPos() - m_vPosDamage;
	if( IsInRange( v2, RANGE_RETURN_TO_BEGIN ) == FALSE	&& !CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon( pWorld->GetID() )	)
	{
		DoReturnToBegin();
		SendAIMsg( AIMSG_SETSTATE, dwAIState );
		return TRUE;
	}
	else if( pTarget->GetType() == OT_MOVER && ((CMover *)pTarget)->IsMode( TRANSPARENT_MODE ) )		// 쫒고 있던 타겟이 투명화 상태가 되면 
	{
		if( CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon( pWorld->GetID() ) )
		{
			CMover* pNewTarget = ScanTargetAndSetTarget( 160/*, m_dwIdTarget, m_uParty*/ );	// 범위 50미터 이내를 스캔한다.
			if( pNewTarget	)
			{
				SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, pNewTarget->GetId() );
				return TRUE;
			}
		}

		if( dwAIState == STATE_STAND )
			DoReturnToBegin();	// SetTarget( NULL_ID, 0 )// 타겟을 포기함.
		SendAIMsg( AIMSG_SETSTATE, dwAIState );
		return TRUE;
	}
	else
	{
		m_bRangeAttack = SelectAttackType( pTarget );		// AI에따른 공격방식 선택.

		if( m_bRangeAttack )	// 원거리 공격으로 결정됐음.
		{
			int nTargetJob = pTarget->GetJob();
			if( nTargetJob < 0 )	nTargetJob = 0;		// 디폴트로 -1로 초기화 하기땜에 검사해야함.
			if( nTargetJob >= MAX_JOB )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "범위침범 %d" ), nTargetJob );
				nTargetJob = 0;
			}

			FLOAT	fRange = (FLOAT)(pProp->m_bRangeAttack[ nTargetJob ] & 0x7F);		// 사거리
			BOOL	bKeepRange = (pProp->m_bRangeAttack[ nTargetJob ] & 0x80) ? TRUE : FALSE;	// 상위 1비트가 온?
			FLOAT	fDist = D3DXVec3Length( &vTargetTo );		// 적과의 거리.
			if( bKeepRange )
			{
				if( fabs(fDist - fRange) < 1.0f )	// 적과의 거리와 사거리의 차이가 1미터 이내면 더이상 움직일 필요 없음.
				{
					if( pMover->IsEmptyDest() == FALSE )	// 이동중임.
						MoveToDst( pMover->GetPos() );		// 현재위치로 이동 - 서버에선 스탑과 같은 명령. 클라에선 서버의 위치로 이동.
				} else
				{
					// fRange의 거리를 유지하도록 ...
					if( fDist < fRange )	
					{
						FLOAT fAngle = GetDegreeX( pMover->GetPos(), pTarget->GetPos() );	// dest, src의 파라메터 위치를 바꾸면 반대방향이 나온다.
						D3DXVECTOR3 vDst = pMover->GetPos() + VelocityToVec( fAngle, 10 );
						MoveToDst( vDst );		// 반대방향으로 달려서 거리를 벌림.
					} else
						MoveToDst( pTarget->GetId() );	// 목표와의 거리가 사거리보다 더 머니까 근접함.
				}
			} else
			// not keepRange
			{
				if( fDist < fRange )		// 사거리안에 있으면 제자리에 있음.
				{
					if( pMover->IsEmptyDest() == FALSE )	// 이동중임.
						MoveToDst( pMover->GetPos() );		// 현재위치로 이동 - 서버에선 스탑과 같은 명령. 클라에선 서버의 위치로 이동.
				} else
					MoveToDst( pTarget->GetId() );	// 사거리 밖이면 목표쪽으로 접근.
			}
		} else
		// 근거리
		{
			MoveToDst( pTarget->GetId() );		// 근거리는 무조건 목표쪽으로 접근.
		}

		if( pProp->m_nSummProb )			// Summon AI가 있느냐?
			SubSummonProcess( pTarget );	// 소환을 시도
		
		// 공격 찬스.
		if( TIMEGETTIME > m_tmAttackDelay )
		{
			if( SubAttackChance( msg, pTarget ) == FALSE )
				return FALSE;
		}
	}
	return TRUE;
}

BOOL CAIAggroNormal::MoveProcessRagePatrol( const AIMSG & msg )
{
	CMover* pMover = GetMover();
	CWorld* pWorld = GetWorld();
	MoverProp *pProp = pMover->GetProp();
	
	// 데미지 상태거나 죽었을 경우 이동 처리 수행 않음 
	if( pMover->IsDie() || (pMover->m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL) )
		return FALSE;
	
	CMover* pTarget = ScanTargetAndSetTarget( CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon( pWorld->GetID() ) ? 160 : 15, 
		pProp->m_nScanJob, pProp->m_dwScanQuestId, pProp->m_dwScanItemIdx, pProp->m_nScanChao ); 

	DWORD dwAIState = pMover->m_dwAIInterfaceState;

	if( IsValidObj( pTarget ) == FALSE )
	{
		GetAggroMng()->RemoveMover( /**GetMover(),*/ m_dwIdTarget );

		DoReturnToBegin();	// SetTarget( NULL_ID, 0 );
		SendAIMsg( AIMSG_SETSTATE, dwAIState );
		return TRUE;
	}
	
	if( ((CMover*)pTarget)->IsDie() )	// 상대가 죽었다.
	{
		DoReturnToBegin();	// SetTarget( NULL_ID, 0 );
		SendAIMsg( AIMSG_SETSTATE, dwAIState );
		return TRUE;
	}

	if( pMover->IsFlyingNPC() == FALSE && ((CMover*)pTarget)->m_pActMover->IsFly() )	// this가 비행형이 아니고 상대가 비행중이라면
	{
		DoReturnToBegin();
		SendAIMsg( AIMSG_SETSTATE, dwAIState );
		return TRUE;
	}

	if( TIMEGETTIME > m_tmAttack )	// 공격 못한지 20초 지났냐?
	{
		m_tmAttack = TIMEGETTIME + s_tmAttack;		// 타이머 리셋
		// 5초전 좌표와 지금 좌표의 거리를 계산.
		D3DXVECTOR3 vDist = pMover->GetPos() - m_vOldPos;
		vDist.y = 0;					// 옆으로 살짝움직였는데 산위로 확 올라갔을땐 거리가 엄청 멀수 있다. 그래서 이렇게 한것.
		if( IsInRange( vDist, 2.0f ) )	// 2미터를 못벗어났다면 어딘가에 갖혔다는 뜻.
		{
			if( pMover->GetSpeed( pMover->m_pActMover->m_fSpeed ) > 0 )		// 마법적으로 움직이지 못하게 된상태에선 워프하면 안됨.
			{
				D3DXVECTOR3 vTarget = pTarget->GetPos();
				pMover->SetPos( vTarget );		// 갖혔으면 타겟쪽으로 바로 워프.
				g_xWSUserManager->AddSetPos( pMover, pMover->GetPos() );
			} 
		} 
		else
		{
			m_tmReturnToBegin = TIMEGETTIME;			// 다시 돌아갈땐 시간을 재기시작.  x초간은 맞아도 반격안하고 계속 간다.
			DoReturnToBegin();
			SendAIMsg( AIMSG_SETSTATE, dwAIState );
			return TRUE;
		}
		m_vOldPos = pMover->GetPos();	// 현재 좌표를 저장함.
	}

	D3DXVECTOR3	vTargetTo = pTarget->GetPos() - pMover->GetPos();	
	// 만약 상대가 공격할 수 없는 곳에 있다면 공격 포기하고 돌아가자.
	if( IsInRange( vTargetTo, RANGE_CANCEL ) == FALSE )
	{
		DoReturnToBegin();	// SetTarget( NULL_ID, 0 );	// 타겟을 포기함.
		SendAIMsg( AIMSG_SETSTATE, dwAIState );
		return TRUE;
	}
	else if( pTarget->GetType() == OT_MOVER && ((CMover *)pTarget)->IsMode( TRANSPARENT_MODE ) )		// 쫒고 있던 타겟이 투명화 상태가 되면 
	{
		if( CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon( pWorld->GetID() ) )
		{
			GetAggroMng()->RemoveMover( /**GetMover(),*/ pTarget->GetId() );
			CMover * pNewTarget = ScanTargetAndSetTarget( 160/*, m_dwIdTarget, m_uParty*/ );	// 범위 50미터 이내를 스캔한다.
			if( pNewTarget	)
			{
				SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, pNewTarget->GetId() );
				return TRUE;
			}
		}

		DoReturnToBegin();	// SetTarget( NULL_ID, 0 );	// 타겟을 포기함.
		SendAIMsg( AIMSG_SETSTATE, dwAIState );
		return TRUE;
	}
	else
	{
		m_bRangeAttack = SelectAttackType( pTarget );		// AI에따른 공격방식 선택.

		if( m_bRangeAttack )	// 원거리 공격으로 결정됐음.
		{
			int nTargetJob = pTarget->GetJob();
			if( nTargetJob < 0 )	nTargetJob = 0;		// 디폴트로 -1로 초기화 하기땜에 검사해야함.
			if( nTargetJob >= MAX_JOB )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "범위침범 %d" ), nTargetJob );
				nTargetJob = 0;
			}

			FLOAT	fRange = (FLOAT)(pProp->m_bRangeAttack[ nTargetJob ] & 0x7F);		// 사거리
			BOOL	bKeepRange = (pProp->m_bRangeAttack[ nTargetJob ] & 0x80) ? TRUE : FALSE;	// 상위 1비트가 온?
			FLOAT	fDist = D3DXVec3Length( &vTargetTo );		// 적과의 거리.
			if( bKeepRange )
			{
				if( fabs(fDist - fRange) < 1.0f )	// 적과의 거리와 사거리의 차이가 1미터 이내면 더이상 움직일 필요 없음.
				{
					if( pMover->IsEmptyDest() == FALSE )	// 이동중임.
						MoveToDst( pMover->GetPos() );		// 현재위치로 이동 - 서버에선 스탑과 같은 명령. 클라에선 서버의 위치로 이동.
				} else
				{
					// fRange의 거리를 유지하도록 ...
					if( fDist < fRange )	
					{
						FLOAT fAngle = GetDegreeX( pMover->GetPos(), pTarget->GetPos() );	// dest, src의 파라메터 위치를 바꾸면 반대방향이 나온다.
						D3DXVECTOR3 vDst = pMover->GetPos() + VelocityToVec( fAngle, 10 );
						MoveToDst( vDst );		// 반대방향으로 달려서 거리를 벌림.
					} else
						MoveToDst( pTarget->GetId() );	// 목표와의 거리가 사거리보다 더 머니까 근접함.
				}
			} else
			// not keepRange
			{
				if( fDist < fRange )		// 사거리안에 있으면 제자리에 있음.
				{
					if( pMover->IsEmptyDest() == FALSE )	// 이동중임.
						MoveToDst( pMover->GetPos() );		// 현재위치로 이동 - 서버에선 스탑과 같은 명령. 클라에선 서버의 위치로 이동.
				} else
					MoveToDst( pTarget->GetId() );	// 사거리 밖이면 목표쪽으로 접근.
			}
		} else
		// 근거리
		{
			MoveToDst( pTarget->GetId() );		// 근거리는 무조건 목표쪽으로 접근.
		}

		if( pProp->m_nSummProb )			// Summon AI가 있느냐?
			SubSummonProcess( pTarget );	// 소환을 시도
		
		// 공격 찬스.
		if( TIMEGETTIME > m_tmAttackDelay )
		{
			if( SubAttackChance( msg, pTarget ) == FALSE )
				return FALSE;
		}
	}
	return TRUE;
}

BOOL CAIAggroNormal::StateRagePatrol( const AIMSG & msg )
{
	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();
	if( IsInvalidObj(pMover) )	return FALSE;

	BeginAIHandler( )

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT ) 
		if( msg.dwParam2 != NULL_ID )
		{
			CMover* pTarget	= prj.GetMover( msg.dwParam2 );
			if( IsValidObj( pTarget ) )
				SetTarget( msg.dwParam2, pTarget->m_idparty );
		}
		m_tmAttack = TIMEGETTIME + s_tmAttack;
		m_tmAttackDelay = TIMEGETTIME; // 공격 딜레이를 초기화. 그렇지 않으면 처음 공격이 딜레이 먹고 들어간다.
		SetStop( 0 ); // 0으로 세팅하면 정지 상태가 바로 끝나게 되어 공격이 가능하게 된다. 
		              // 정지 상태에서는 정지 상태를 일정 시간동안 유지해주어야하는데, 그렇게 되면 공격을 못할 수 있다.

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT_TARGETCLEAR )		// 타겟을 클리어하고 대기모드로 돌아감.
		SetTarget( NULL_ID, 0 );
		SendAIMsg( AIMSG_SETSTATE, STATE_PATROL );
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_PROCESS ) 
		MoveProcessRagePatrol( msg );
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DAMAGE ) 
		if( IsMove() && m_bGoTarget == FALSE )
		{
			if( xRandom( 2 ) == 0 )		// 1/2확률로 공격자를 때림.
			{
				// 공격자에게 목표를 설정 
				MoveToDst( msg.dwParam1 );
			}
			m_bGoTarget = TRUE;
		}
		MoverProp* pMoverProp = pMover->GetProp();
//		int nHPPercent = pMover->GetHitPointPercent( 100 );
		
		// Call HP 퍼센트보다 현재 HP퍼센트가 작을 경우 동료를 부르자.
		if( pMoverProp->m_bHelpWho )	// 헬프AI가 있을경우.
		{
			if( TIMEGETTIME - m_tmHelp > pMoverProp->m_tmUnitHelp )	// 최초 m_tmHelp는 0이므로 헬프를 한다.
			{
				if( m_bCallHelper == FALSE )
				{
					CallHelper( pMoverProp );
					g_xWSUserManager->AddDlgEmoticon( pMover, DLGEMOT_HELPER );
					m_tmHelp = TIMEGETTIME;
					if( pMoverProp->m_tmUnitHelp == 0 )		// 0이면 한번만 부르고 안부름.
						m_bCallHelper = TRUE;
				}
			}
		}
		else
			m_bCallHelper = FALSE;

		m_vPosDamage	= pMover->GetPos();

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_COLLISION )

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DIE ) 

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DSTDIE ) 
		{
			CMover * pNewTarget = ScanTargetAndSetTarget( 15/*, m_dwIdTarget, m_uParty*/ );	// 범위 50미터 이내를 스캔한다.
			if( pNewTarget	)
			{
				SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, pNewTarget->GetId() );
				return TRUE;
			}
			else
			{
				DoReturnToBegin();
				SetTarget( NULL_ID, 0 );
			}
		}
		FLTRACE_LOG( PROGRAM_NAME, _T( "TARGET CLEARED" ) );
		SendAIMsg( AIMSG_SETSTATE, STATE_PATROL );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_BEGINMOVE )
		m_bGoTarget = FALSE; 

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_ARRIVAL )

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_EXIT )	

	EndAIHandler( )
	return TRUE;
}

BOOL CAIAggroNormal::StateRage( const AIMSG & msg )
{
	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();
//	MoverProp *pProp	= pMover->GetProp();
	if( IsInvalidObj(pMover) )	return FALSE;

	DWORD dwAIState = pMover->m_dwAIInterfaceState;

	BeginAIHandler( )

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT ) 
		if( msg.dwParam2 != NULL_ID )
		{
			CMover* pTarget	= prj.GetMover( msg.dwParam2 );
			if( IsValidObj( pTarget ) )
				SetTarget( msg.dwParam2, pTarget->m_idparty );
		}
		m_tmAttack = TIMEGETTIME + s_tmAttack;
		m_tmAttackDelay = TIMEGETTIME; // 공격 딜레이를 초기화. 그렇지 않으면 처음 공격이 딜레이 먹고 들어간다.
		SetStop( 0 ); // 0으로 세팅하면 정지 상태가 바로 끝나게 되어 공격이 가능하게 된다. 
		              // 정지 상태에서는 정지 상태를 일정 시간동안 유지해주어야하는데, 그렇게 되면 공격을 못할 수 있다.

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT_TARGETCLEAR )		// 타겟을 클리어하고 대기모드로 돌아감.
		CMover * pNewTarget = ScanTargetAndSetTarget( 15/*, m_dwIdTarget, m_uParty*/ );	// 범위 50미터 이내를 스캔한다.
		if( pNewTarget )
		{
			SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, pNewTarget->GetId() );
			return TRUE;
		}
		else
		{
			DoReturnToBegin();
			SetTarget( NULL_ID, 0 );
			SendAIMsg( AIMSG_SETSTATE, dwAIState );
		}
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_PROCESS ) 
		MoveProcessRage( msg );
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DAMAGE ) 
		if( IsMove() && m_bGoTarget == FALSE )
		{
			if( xRandom( 2 ) == 0 )		// 1/2확률로 공격자를 때림.
			{
				// 공격자에게 목표를 설정 
				MoveToDst( msg.dwParam1 );
			}
			m_bGoTarget = TRUE;
		}
		MoverProp* pMoverProp = pMover->GetProp();
		int nHPPercent = pMover->GetHitPointPercent( 100 );

		// 도주 HP 퍼센트보다 현재 HP퍼센트가 작을 경우 도망가자.
		if( nHPPercent < pMoverProp->m_nRunawayHP )
		{
			// 도주 모드로 세팅 
			SendAIMsg( AIMSG_SETSTATE, STATE_RUNAWAY );
			g_xWSUserManager->AddDlgEmoticon( pMover, DLGEMOT_EVADE );
		}
		// Call HP 퍼센트보다 현재 HP퍼센트가 작을 경우 동료를 부르자.
		if( pMoverProp->m_bHelpWho )	// 헬프AI가 있을경우.
		{
			if( TIMEGETTIME - m_tmHelp > pMoverProp->m_tmUnitHelp )	// 최초 m_tmHelp는 0이므로 헬프를 한다.
			{
				if( m_bCallHelper == FALSE )
				{
					CallHelper( pMoverProp );
					g_xWSUserManager->AddDlgEmoticon( pMover, DLGEMOT_HELPER );
					m_tmHelp = TIMEGETTIME;
					if( pMoverProp->m_tmUnitHelp == 0 )		// 0이면 한번만 부르고 안부름.
						m_bCallHelper = TRUE;
				}
			}
		}
		else
			m_bCallHelper = FALSE;

		m_vPosDamage	= pMover->GetPos();

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_COLLISION )
		MoveToRandom( STATE_RAGE );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DIE ) 

	///////////////////////////////////////////////////////////////////////////////////////////////////
//	OnMessage( AIMSG_DSTDAMAGE ) 
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DSTDIE ) 
		{
			CMover * pNewTarget = ScanTargetAndSetTarget( 15/*, m_dwIdTarget, m_uParty*/ );	// 범위 50미터 이내를 스캔한다.
			if( pNewTarget )
			{
				SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, pNewTarget->GetId() );
				return TRUE;
			}
			else
			{
				DoReturnToBegin();
				SetTarget( NULL_ID, 0 );
			}
		}
		SendAIMsg( AIMSG_SETSTATE, dwAIState );


	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_BEGINMOVE )
		m_bGoTarget = FALSE; 

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_ARRIVAL )

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_EXIT )	

	EndAIHandler( )
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CAIAggroNormal::MoveProcessRunaway()
{
	CMover* pMover = GetMover();
	CWorld* pWorld = GetWorld();

	// 데미지 상태거나 죽었을 경우 이동 처리 수행 않음 
	if( pMover->IsDie() || (pMover->m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL))
		return FALSE;

	DWORD dwAIState = pMover->m_dwAIInterfaceState;

	MoverProp *pProp = pMover->GetProp();
	CCtrl * pTarget = ScanTargetAndSetTarget( CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon( pWorld->GetID() ) ? 160 : 15, 
		pProp->m_nScanJob, pProp->m_dwScanQuestId, pProp->m_dwScanItemIdx, pProp->m_nScanChao ); 

	// 타겟이 무효인가?
	if( IsValidObj( pTarget ) == FALSE )
	{
		GetAggroMng()->RemoveMover( /**GetMover(),*/ m_dwIdTarget );
		SetTarget( NULL_ID, 0 );
		SendAIMsg( AIMSG_SETSTATE, dwAIState );
	}
	else
	// 멈춤 대기 시간이 끝났나?
	if( IsEndStop()  )
	{
		CMover* pMoverTarget = (CMover*)pTarget;
		DWORD dwAttackRange = pMoverTarget->GetActiveHandItemProp()->dwAttackRange;
		FLOAT fRange = pMoverTarget->GetAttackRange( dwAttackRange ) * 1.7f;

		// 나와 20미터 안쪽(근처) 거리에 존재하면 줄행랑!
		if( IsInRange( pTarget->GetPos() - pMover->GetPos(), fRange ) == TRUE )
		{
			// 5도 폭을 가지고 각도 변경. 폭이 크면 클수록 지그재그 형태로 움직인다.
			pMover->AddAngle( (float)( 20 - xRandom( 40 ) ) );
			// 각도를 꺼낸 후에 연장선에 있는 좌표를 구한다.
			FLOAT fAngle = pMover->GetAngle();
			D3DXVECTOR3 vDst = pMover->GetPos() + VelocityToVec( fAngle, fRange );
			MoveToDst( vDst );
		}
		if( m_bTargetNoMovePos == FALSE )
		{
			// 여기서 휴식 일정시간마다 HP를 채워준다.
			if( ::TIMEGETTIME > pMover->m_dwTickRecovery )	// .
			{
				pMover->m_dwTickRecovery = ::TIMEGETTIME + NEXT_TICK_RECOVERY;
				pMover->IncHitPoint( pMover->GetHPRecovery() );
				// 체력이 80 % 이상 되면 반격 시도.
				if( pMover->GetHitPointPercent( 100 ) >= 50 )
					SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, NULL_ID );
			}
		}
	}
	return TRUE;
}

BOOL CAIAggroNormal::StateRunaway( const AIMSG & msg )
{
	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();

	DWORD dwAIState = pMover->m_dwAIInterfaceState;

	BeginAIHandler( )

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT ) 
		// 이경우 정상적인 옵션에 의한 도주가 아니라, 상대가 공격할 수 없는, 지리상 잇점이 있는 장소에
		// 있기 때문에 줄행랑을 치는 경우다. 
		m_bTargetNoMovePos	= msg.dwParam2;
		// 현재의 반대 방향으로 도주 시작 
		// 5도 폭을 가지고 각도 변경. 폭이 크면 클수록 지그재그 형태로 움직인다.
		pMover->AddAngle( (float)( 180 + ( 20 - xRandom( 40 ) ) ) );
		// 각도를 꺼낸 후에 연장선에 있는 좌표를 구한다.
		CMover* pMoverTarget = (CMover*)prj.GetCtrl( m_dwIdTarget ); // 목표를 재설정해준다.
		if( IsValidObj(pMoverTarget) )
		{
			DWORD dwAttackRange = pMoverTarget->GetActiveHandItemProp()->dwAttackRange;
			FLOAT fRange = pMoverTarget->GetAttackRange( dwAttackRange ) * 1.7f;
			FLOAT fAngle = pMover->GetAngle();
			D3DXVECTOR3 vDst = pMover->GetPos() + VelocityToVec( fAngle, fRange );
			MoveToDst( vDst );
			m_bFirstRunaway = TRUE; // 처음 도주인가?
		}
		else
		{
			SendAIMsg( AIMSG_SETSTATE, dwAIState );
		}
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_PROCESS ) 
		MoveProcessRunaway();
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DAMAGE ) 
		if( m_bTargetNoMovePos == TRUE && m_bFirstRunaway == FALSE )
			SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, NULL_ID );

		m_vPosDamage	= pMover->GetPos();

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_COLLISION )
		// 현재의 반대 방향으로 도주 시작 
		// 5도 폭을 가지고 각도 변경. 폭이 크면 클수록 지그재그 형태로 움직인다.
		pMover->AddAngle( (float)( 180 + ( 20 - xRandom( 40 ) ) ) );
		// 각도를 꺼낸 후에 연장선에 있는 좌표를 구한다.
		FLOAT fAngle = pMover->GetAngle();
		D3DXVECTOR3 vDst = pMover->GetPos() + VelocityToVec( fAngle, 5 );
		MoveToDst( vDst );
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DIE ) 
	///////////////////////////////////////////////////////////////////////////////////////////////////
//	OnMessage( AIMSG_DSTDAMAGE ) 
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DSTDIE ) 
		{
			CMover * pNewTarget = ScanTargetAndSetTarget( 15/*, m_dwIdTarget, m_uParty*/ );	// 범위 50미터 이내를 스캔한다.
			if( pNewTarget )
			{
				SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, pNewTarget->GetId() );
				return TRUE;
			}
			else
			{
				SetTarget( NULL_ID, 0 );
			}
		}
		SendAIMsg( AIMSG_SETSTATE, dwAIState );
		
	OnMessage( AIMSG_ARRIVAL )
		m_bFirstRunaway = FALSE;
		MoverProp* pMoverProp = pMover->GetProp();
		if( pMoverProp->m_dwRunawayDelay )
			SetStop( pMover->GetProp()->m_dwRunawayDelay + xRandom( SEC( 1 ) ) ); // + xRandom은 약간의 가변 차이를 주기 위한 것.

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_EXIT )	

	EndAIHandler( )

	return TRUE;
}

void	CAIAggroNormal::SetTarget( OBJID dwIdTarget, u_long uParty )
{
	m_dwIdTarget	= dwIdTarget;

	m_uParty	= uParty;
	if( dwIdTarget == NULL_ID )
		GetMover()->SetSpeedFactor( 1.0F );
}

CMover *	CAIAggroNormal::ScanTargetAndSetTarget( int nRangeMeter, int nJobCond, DWORD dwQuest, DWORD dwItem, int nChao )
{
	CMover * pTarget = CAIAggroInterface::ScanTargetByContidion( nRangeMeter, nJobCond, dwQuest, dwItem, nChao );
	if( pTarget )			
		SetTarget( pTarget->GetId(), pTarget->m_idparty );
	else					
		SetTarget( NULL_ID, 0 );

	return pTarget;
}

#endif