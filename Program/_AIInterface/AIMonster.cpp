#include "stdafx.h"
#include "AIMonster.h"
#include "lang.h"
#include "dialogMsg.h"
#include "../WorldServer/User.h"
#include "../worldserver/FLMapGrid.h"
#include "../worldserver/LinkMap.h"
#include "Party.h"
#include "PathObj.h"

#include "../_Common/FLSkillSystem.h"



extern	CPartyMng		g_PartyMng;

#define RANGE_CANCEL	120.0f
#define RANGE_MOVE	30.0f
#define	RANGE_RETURN_TO_BEGIN	120.0F

#define	TIMEGETTIME		g_tmCurrent

enum
{
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

BEGIN_AISTATE_MAP( CAIMonster, CAIInterface )

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
	const int INITIAL_AI_MONSTER = 1024*5;
#endif

MEMPOOLER_IMPLEMENT( CAIMonster, INITIAL_AI_MONSTER );

static DWORD s_tmAttack = SEC(15);		//  m_tmAttack 20�� 

CAIMonster::CAIMonster()
{
	Init();
}

CAIMonster::CAIMonster( CObj* pObj ) : CAIInterface( pObj )
{
	Init();
}

void CAIMonster::Init()
{
	SetTarget( NULL_ID, 0 );
	m_bReturnToBegin	= FALSE;
	m_bGoTarget		= FALSE;
	m_bTargetNoMovePos	= FALSE;
	m_dwAtkMethod	= 0;
	m_bFirstRunaway		= FALSE;
	m_bCallHelper	= FALSE;
	m_tmAttack	= TIMEGETTIME + s_tmAttack;		// ���� �� 20�ʰ� �����ؿ�.
	m_vOldPos.x	= m_vOldPos.y = m_vOldPos.z = 0;
	m_bRangeAttack	= FALSE;

	m_tmSummon = TIMEGETTIME;
	m_tmHelp = 0;

	m_bLootMove	= FALSE;
	m_idLootItem	= NULL_ID;
	m_tmAttackDelay	= m_tmMove = m_tmReturnToBegin = TIMEGETTIME;
	m_fAngleBegine	= 0.0f;

	m_vPosDamage	= D3DXVECTOR3( 0, 0, 0 );
}

CAIMonster::~CAIMonster()
{
}

void CAIMonster::InitAI()
{
	m_vPosBegin = GetMover()->GetPos();
	m_fAngleBegine = GetMover()->GetAngle();
	PostAIMsg( AIMSG_SETSTATE, GetMover()->m_dwAIInterfaceState ); 	
}

BOOL CAIMonster::IsIdleMode()
{
	FUNCAISTATE pStateFunc	= GetAIStateFunc( STATE_IDLE );
	if( pStateFunc != NULL && pStateFunc == m_pStateFunc )
		return TRUE;

	return FALSE;
}

void CAIMonster::SetStop( DWORD dwTime ) 
{ 
	m_tmMove = TIMEGETTIME + dwTime;
	GetMover()->SetStop(); 
} 

BOOL CAIMonster::IsEndStop() 
{ 
	return ( TIMEGETTIME > m_tmMove ) && ( IsMove() == FALSE ) ; 
}

void CAIMonster::MoveToDst(	const D3DXVECTOR3& vDst )
{
	CMover* pMover = GetMover();
	if( pMover->GetDestPos() == vDst )
		return;

	pMover->SetDestPos( vDst );
	g_xWSUserManager->AddSetDestPos( pMover, vDst, 1 );
}

void CAIMonster::MoveToDst(	OBJID dwIdTarget )
{
	CMover* pMover = GetMover();
	if( pMover->GetDestId() == dwIdTarget )
		return;

	GetMover()->SetSpeedFactor( 2.0F );

	CMover* pTarget	= prj.GetMover( dwIdTarget );
	if( IsValidObj( pTarget ) )
		SetTarget( pTarget->GetId(), pTarget->m_idparty );

	pMover->SetDestObj( dwIdTarget, 0.0f, TRUE ); // ��ǥ�� �缳�����ش�.
	//g_xWSUserManager->AddMoverSetDestObj( (CMover*)pMover, dwIdTarget );
}

void CAIMonster::MoveToRandom( UINT nState )
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
		if( pMover->IsFlyingNPC() )			// ������� ��� �Ÿ� 3��
			fRange *= 3.0f;
		
		if( IsInRange( vDistant, fRange ) )	// 15���� �ݰ��� ����� ���� 
		{
			MoveToDst( vPos );
		}
		else
		{
			// �������� �߿� ����ȣ���̸� �浹ȸ�Ǵ�. ȸ������.
			if( m_bReturnToBegin )
				MoveToDst( vPos );
			else
			{
				// ���� ���� �̹� ���� ������ ���� ����ִٸ� ���ư����� �õ�.
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

BOOL CAIMonster::IsInRange( const D3DXVECTOR3& vDistant, FLOAT fRange )
{
	D3DXVECTOR3 vDist = vDistant;
	vDist.y	= 0.0f;
	const float fDistSq = D3DXVec3LengthSq( &vDist );	
	if( fDistSq > fRange * fRange )	
		return FALSE;
	return TRUE;
}

// ��ó�� �������� ������ ����.
BOOL CAIMonster::SubItemLoot()
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

	// ��ó�� �������� �˻���.
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
				fDistSq = D3DXVec3LengthSq( &vDist );			// �Ÿ� ����.
				if( fDistSq < 10.0f * 10.0f )					// 10���� �̳���... random
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
		MoveToDst( pMinObj->GetPos() );		// ��ǥ������ �̵�.
		m_idLootItem = ((CItem *)pMinObj)->GetId();
		m_bLootMove = TRUE;
		g_xWSUserManager->AddDlgEmoticon( pMover, DLGEMOT_LOOT );
	}

	return TRUE;
}

BOOL CAIMonster::StateInit( const AIMSG & /*msg*/ )
{
	return TRUE;
}

// �¾ �������� ���ư���.
void CAIMonster::DoReturnToBegin( BOOL bReturn )
{
	if( bReturn )
	{
		m_bReturnToBegin	= TRUE;
		m_tmReturnToBegin	= TIMEGETTIME;		// ���ư���� ������ ���� �ð� ���.
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

BOOL CAIMonster::MoveProcessIdle( const AIMSG & /*msg*/ )
{
	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();
	MoverProp *pProp = pMover->GetProp();

	// ������ ���°ų� �׾��� ��� �̵� ó�� ���� ���� 
	if( pMover->IsDie() || (pMover->m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL) )
		return FALSE;

#ifdef __MOVER_STATE_EFFECT
	if( pMover->m_pActMover->IsStateFlag( OBJSTAF_COMBAT ) == TRUE )
	{
		if( pMover->m_pActMover->SendActMsg( OBJMSG_MODE_PEACE ) == TRUE )
		{
			g_xWSUserManager->AddSendActMsg( pMover, OBJMSG_MODE_PEACE );
		}
	}
#endif // __MOVER_STATE_EFFECT

	// ���� ��ҷ� ��ȯ 
	if( m_bReturnToBegin )
	{
		if( IsInRange( m_vPosBegin - pMover->GetPos(), 7.0f ) )		// ������������ ���ƿԴ�.
		{
			DoReturnToBegin( FALSE );
			SendAIMsg( AIMSG_ARRIVAL, NULL_ID );
		}

		if( TIMEGETTIME - m_tmReturnToBegin >= SEC(20) )		// ������ ���ư��� �������� 10�ʳ� ������.
		{
			// ����� �����ư��� �ִٰ� �Ǵ��ϰ� ������������ ������Ŵ.
			pMover->SetPos( m_vPosBegin );
			g_xWSUserManager->AddSetPos( pMover, m_vPosBegin );
			m_tmReturnToBegin	= TIMEGETTIME;
		}
	}
	else
	{
		if( pMover->GetMovePattern() == 0 )	// �Ϲ� �����̵��϶��� ����.
		{
			if( IsEndStop() )						// ���ڸ� ��� �ð��� ��������
				MoveToRandom( STATE_IDLE );			// ���� ��ǥ�� ����.
		}
		else								// 8�ں�����		
		{
			if( pMover->GetMoveEvent() == 99 )		// ���Ϻ��� ��.
				pMover->SetMovePattern( 0 );		// �Ϲ��̵����� ��ȯ.
		}

		// ���ÿɼ��� �ִ� ���̴�.
		if( pProp->m_nLoot )
		{
			if( (pMover->GetCount() & 63) == 0 )		// ������ �ֺ��� ��ĵ�ؼ�.(63�̸� �� 4.2��)
			{	
				if(  pProp->m_nLootProb == 0 || xRandom(100) < (DWORD)( pProp->m_nLootProb )  )	// ����Ʈ��(0)�� 100% / �̰ų� ����Ȯ���� ���� ���ýõ�
					if( m_bLootMove == FALSE )		// �����Ϸ� ���� ������ �� üũ�ϸ� �Ȋ�
						SubItemLoot();		// �������� ������.
			}
		}

		if( m_bLootMove == TRUE )
		{
			CCtrl *pCtrl = prj.GetCtrl( m_idLootItem );		// �׾������� ��������� �����ϱ� �˻���.
			if( IsInvalidObj(pCtrl) )		// ������ ������ �̵��߿� ������ ��������
			{
				MoveToDst( pMover->GetPos() );	// ���ڸ��� ��.
				m_idLootItem = NULL_ID;
				m_bLootMove = FALSE;
			}
		}

		// -1 �̸� �ƹ��͵� ��ĵ���� �ʴ´�!!!
		if( pProp->m_nScanJob != -1 )
		{
			CMover* pTarget = NULL;
			int nAttackFirstRange = pProp->m_nAttackFirstRange;
			if( nAttackFirstRange > 10 || nAttackFirstRange < 0 )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "m_nAttackFirstRange �̻�:%d %s" ), pProp->m_nAttackFirstRange, pMover->GetName() );
				return TRUE;
			}

			// � ���ǿ� �ش��ϴ� �÷��̾ �ִ��� �����Ѵ�.
			if( pProp->m_nScanJob || pProp->m_dwScanQuestId || pProp->m_dwScanItemIdx || pProp->m_nScanChao )
				pTarget = ScanTarget( pMover, nAttackFirstRange, pProp->m_nScanJob, pProp->m_dwScanQuestId, pProp->m_dwScanItemIdx, pProp->m_nScanChao );

			// ���ǿ� �´� �÷��̾ ���� �������̸� ������ ������ ������ �Ѵ�.
			if( pTarget == NULL && GetMover()->m_bActiveAttack )//&& GetMover()->IsRegionAttr( RA_SAFETY ) == FALSE )
				pTarget = ScanTarget( pMover, nAttackFirstRange, JOB_ALL );

			if( pTarget )
			{
				if( IsValidObj( (CObj*)pTarget ) )
				{

					if( (pMover->IsFlyingNPC() || pTarget->m_pActMover->IsFly() == FALSE) || pProp->dwClass == RANK_GUARD )	// ����� ������ ����.
					{
						// Ÿ���� �����ϸ� ���� ���� ���� 
						if( pTarget )	
						{
							g_xWSUserManager->AddDlgEmoticon( pMover, DLGEMOT_ATTACK );
							SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, pTarget->GetId() );
						}
					}
				}
			}	// if( target)
		}	// if( scan )

	}
	return TRUE;
}

BOOL CAIMonster::StateIdle( const AIMSG & msg )
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
		//�г� ���� ����

		if( m_bReturnToBegin == FALSE )
		{
			MoveToDst( msg.dwParam1 );
			SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, msg.dwParam1 );
			m_tmAttack	= TIMEGETTIME + s_tmAttack;		// ������϶� ���ݹ����� 20�� Ÿ�̸� ����
			m_vPosDamage	= pMover->GetPos();
		}
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DIE )		// ��!
		SendAIMsg( AIMSG_EXIT );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_COLLISION )  // ��ٰ� �浹����!"
		MoveToRandom( STATE_IDLE );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_ARRIVAL )
		if( pMover->IsFlyingNPC() )		
		{
			// ������� �����ϰ� ���� ������ �ʴ´�. 8�ں������� �̵������� �ٲ��.
			if( msg.dwParam2 == 1 )
			{
				int nPattern = 2;
				pMover->SetMovePattern( nPattern );
				// Ŭ��� ����
				g_xWSUserManager->AddSetMovePattern( pMover, nPattern, pMover->GetPos(), pMover->GetAngle(), pMover->GetAngleX() );
			}
		} 
		else
		{
			if( m_bReturnToBegin )
				SetStop( xRandom( SEC( 1 ) ) ); // 1�� ���� ����� 
			else
				SetStop( SEC( 5 ) + xRandom( SEC( 1 ) ) ); // 1�� ���� ����� 

			if( m_bLootMove )	// ������ ���ø�忴��.
			{
				CCtrl *pCtrl = prj.GetCtrl( m_idLootItem );		// �׾������� ��������� �����ϱ� �˻���.
				if( IsValidObj(pCtrl) )
				{
					CItem *pItem = (CItem *)pCtrl;
					D3DXVECTOR3 vDist = pCtrl->GetPos() - pMover->GetPos();
					FLOAT fDistSq = D3DXVec3LengthSq( &vDist );
					if( fDistSq < 3.0f * 3.0f )		// �����ؼ� �Ÿ� �˻� �ѹ��� �ؼ� 
					{
						if( pItem->IsDelete() )
							return TRUE;
						FLItemElem* pItemElem = (FLItemElem *)pItem->m_pItemBase;
						PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( pItem->m_dwIndex );
						BOOL bSuccess = FALSE;
						if( pItemProp->dwItemKind1 == IK1_GOLD )	// ������������ ���̳�?
						{
							pMover->AddGold( pItemElem->GetGold() );	// ������.
							bSuccess = TRUE;
						} 
						else
						{
							// not GOLD
							bSuccess = pMover->CreateItem( pItemElem );			// ���� ������ ����.
						}

						if( bSuccess )
						{
							pItem->Delete();		// �κ������� �ִµ� �����ϸ� �ٴ��� �������� ����.
						}
					}
				}
				m_bLootMove = FALSE;		// this�� ������ �������̶�°� ����.
				m_idLootItem = NULL_ID;
			}
		}
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_EXIT )	

	EndAIHandler( )
	return TRUE;
}

BOOL CAIMonster::StateWander( const AIMSG & /*msg*/ )
	{	return TRUE;	}
BOOL CAIMonster::StateEvade( const AIMSG & /*msg*/ )
	{	return TRUE;	}


BOOL CAIMonster::StateStand( const AIMSG & msg )
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
		//�г� ���� ����

		if( m_bReturnToBegin == FALSE )
		{
			MoveToDst( msg.dwParam1 );
			SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, msg.dwParam1 );
			m_tmAttack	= TIMEGETTIME + s_tmAttack;		// ������϶� ���ݹ����� 20�� Ÿ�̸� ����
			m_vPosDamage	= pMover->GetPos();
		}


	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DIE )		// ��!
		SendAIMsg( AIMSG_EXIT );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_COLLISION )  // ��ٰ� �浹����!"
//		MoveToRandom( STATE_IDLE );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_ARRIVAL )
		if( pMover->IsFlyingNPC() )		
		{
			// ������� �����ϰ� ���� ������ �ʴ´�. 8�ں������� �̵������� �ٲ��.
			if( msg.dwParam2 == 1 )
			{
				int nPattern = 2;
				pMover->SetMovePattern( nPattern );
				// Ŭ��� ����
				g_xWSUserManager->AddSetMovePattern( pMover, nPattern, pMover->GetPos(), pMover->GetAngle(), pMover->GetAngleX() );
			}
		} 
		else
		{
			if( m_bReturnToBegin )
			{
				SetStop( xRandom( SEC( 1 ) ) ); // 1�� ���� ����� 
			}
			else
			{
				SetStop( SEC( 5 ) + xRandom( SEC( 1 ) ) ); // 1�� ���� ����� 
			}

			if( m_bLootMove )	// ������ ���ø�忴��.
			{
				CCtrl *pCtrl = prj.GetCtrl( m_idLootItem );		// �׾������� ��������� �����ϱ� �˻���.
				if( IsValidObj(pCtrl) )
				{
					CItem *pItem = (CItem *)pCtrl;
					D3DXVECTOR3 vDist = pCtrl->GetPos() - pMover->GetPos();
					FLOAT fDistSq = D3DXVec3LengthSq( &vDist );
					if( fDistSq < 3.0f * 3.0f )		// �����ؼ� �Ÿ� �˻� �ѹ��� �ؼ� 
					{
						if( pItem->IsDelete() )
							return TRUE;
						FLItemElem* pItemElem = (FLItemElem *)pItem->m_pItemBase;
						PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( pItem->m_dwIndex );
						BOOL bSuccess = FALSE;
						if( pItemProp->dwItemKind1 == IK1_GOLD )	// ������������ ���̳�?
						{
							pMover->AddGold( pItemElem->GetGold() );	// ������.
							bSuccess = TRUE;
						} 
						else
						{
							// not GOLD
							bSuccess = pMover->CreateItem( pItemElem );			// ���� ������ ����.
						}

						if( bSuccess )
						{
							pItem->Delete();		// �κ������� �ִµ� �����ϸ� �ٴ��� �������� ����.
						}
					}
				}
				m_bLootMove = FALSE;		// this�� ������ �������̶�°� ����.
				m_idLootItem = NULL_ID;
			}
		}
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_EXIT )	

	EndAIHandler( )
	return TRUE;
}
BOOL CAIMonster::StatePatrol( const AIMSG & msg )
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
		//�г� ���� ����

		if( m_bReturnToBegin == FALSE )
		{
			MoveToDst( msg.dwParam1 );
			SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, msg.dwParam1 );
			m_tmAttack	= TIMEGETTIME + s_tmAttack;		// ������϶� ���ݹ����� 20�� Ÿ�̸� ����
			m_vPosDamage	= pMover->GetPos();
		}


	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DIE )		// ��!
		SendAIMsg( AIMSG_EXIT );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_COLLISION )  // ��η� �����̴ٰ� �浹����!"

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

BOOL CAIMonster::MoveProcessStand( const AIMSG & /*msg*/ )
{
	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();
	MoverProp *pProp = pMover->GetProp();

	// ������ ���°ų� �׾��� ��� �̵� ó�� ���� ���� 
	if( pMover->IsDie() || (pMover->m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL) )
		return FALSE;

	// ���� ��ҷ� ��ȯ 
	if( m_bReturnToBegin )
	{
		if( IsInRange( m_vPosBegin - pMover->GetPos(), 0.1f ) )		// ������������ ���ƿԴ�.
		{
			DoReturnToBegin( FALSE );
			pMover->SetAngle( m_fAngleBegine );	
			g_xWSUserManager->AddMoverBehavior( pMover );
			SendAIMsg( AIMSG_ARRIVAL, NULL_ID );
		}

		if( TIMEGETTIME - m_tmReturnToBegin >= SEC(10) )	// ������ ���ư��� �������� 10�ʳ� ������.
		{
			// ����� �����ư��� �ִٰ� �Ǵ��ϰ� ������������ ������Ŵ.
			pMover->SetPos( m_vPosBegin );
			g_xWSUserManager->AddSetPos( pMover, m_vPosBegin );
			m_tmReturnToBegin	= TIMEGETTIME;
		}
	}
	else
	{
		if( pMover->GetMovePattern() == 0 )	// �Ϲ� �����̵��϶��� ����.
		{
		}
		else								// 8�ں�����		
		{
			if( pMover->GetMoveEvent() == 99 )		// ���Ϻ��� ��.
				pMover->SetMovePattern( 0 );		// �Ϲ��̵����� ��ȯ.
		}

		// ���ÿɼ��� �ִ� ���̴�.
		if( pProp->m_nLoot )
		{
			if( (pMover->GetCount() & 63) == 0 )		// ������ �ֺ��� ��ĵ�ؼ�.(63�̸� �� 4.2��)
			{	
				if( pProp->m_nLootProb == 0 || (int)( xRandom(100) ) < pProp->m_nLootProb  )	// ����Ʈ��(0)�� 100% / �̰ų� ����Ȯ���� ���� ���ýõ�
					if( m_bLootMove == FALSE )		// �����Ϸ� ���� ������ �� üũ�ϸ� �Ȋ�
						SubItemLoot();		// �������� ������.
			}
		}

		if( m_bLootMove == TRUE )
		{
			CCtrl *pCtrl = prj.GetCtrl( m_idLootItem );		// �׾������� ��������� �����ϱ� �˻���.
			if( IsInvalidObj(pCtrl) )		// ������ ������ �̵��߿� ������ ��������
			{
				MoveToDst( pMover->GetPos() );	// ���ڸ��� ��.
				m_idLootItem = NULL_ID;
				m_bLootMove = FALSE;
			}
		}

		// -1 �̸� �ƹ��͵� ��ĵ���� �ʴ´�!!!
		if( pProp->m_nScanJob != -1 )
		{
			CMover* pTarget = NULL;
			int nAttackFirstRange = pProp->m_nAttackFirstRange;
			if( nAttackFirstRange > 10 || nAttackFirstRange < 0 )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "m_nAttackFirstRange �̻�:%d %s" ), pProp->m_nAttackFirstRange, pMover->GetName() );
				return TRUE;
			}

			// � ���ǿ� �ش��ϴ� �÷��̾ �ִ��� �����Ѵ�.
			if( pProp->m_nScanJob || pProp->m_dwScanQuestId || pProp->m_dwScanItemIdx || pProp->m_nScanChao )
				pTarget = ScanTarget( pMover, nAttackFirstRange, pProp->m_nScanJob, pProp->m_dwScanQuestId, pProp->m_dwScanItemIdx, pProp->m_nScanChao );

			// ���ǿ� �´� �÷��̾ ���� �������̸� ������ ������ ������ �Ѵ�.
			if( pTarget == NULL && GetMover()->m_bActiveAttack )
				pTarget = ScanTarget( pMover, nAttackFirstRange, JOB_ALL );

			if( pTarget )
			{
				if( IsValidObj( (CObj*)pTarget ) )
				{

					if( (pMover->IsFlyingNPC() || pTarget->m_pActMover->IsFly() == FALSE) || pProp->dwClass == RANK_GUARD )	// ����� ������ ����.
					{
						// Ÿ���� �����ϸ� ���� ���� ���� 
						if( pTarget )	
						{
							g_xWSUserManager->AddDlgEmoticon( pMover, DLGEMOT_ATTACK );
							SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, pTarget->GetId() );
						}
					}
				}
			}	// if( target)
		}	// if( scan )

	}
	return TRUE;
}
BOOL CAIMonster::MoveProcessPatrol( const AIMSG & /*msg*/ )
{
	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();
	MoverProp *pProp = pMover->GetProp();

	// ������ ���°ų� �׾��� ��� �̵� ó�� ���� ���� 
	if( pMover->IsDie() || (pMover->m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL) )
		return FALSE;

	// ���� ��ҷ� ��ȯ 
	if( m_bReturnToBegin )
	{
		if( IsInRange( m_vPosBegin - pMover->GetPos(), 3.0f ) )		// ������������ ���ƿԴ�.
		{
			DoReturnToBegin( FALSE );
//			SendAIMsg( AIMSG_ARRIVAL, NULL_ID );
		}

		if( TIMEGETTIME - m_tmReturnToBegin >= SEC(10) )	// ������ ���ư��� �������� 10�ʳ� ������.
		{
			// ����� �����ư��� �ִٰ� �Ǵ��ϰ� ������������ ������Ŵ.
			pMover->SetPos( m_vPosBegin );
			g_xWSUserManager->AddSetPos( pMover, m_vPosBegin );
			m_tmReturnToBegin	= TIMEGETTIME;
		}
	}
	else
	{
		if( pMover->GetMovePattern() == 0 )	// �Ϲ� �����̵��϶��� ����.
		{
			D3DXVECTOR3 vPos1 = pMover->GetPos();
			D3DXVECTOR3 vPos2 = m_vDestPos;

			if( IsEndStop() )
			{
				MoveToDst( m_vDestPos );
			}
		}
		else								// 8�ں�����		
		{
			if( pMover->GetMoveEvent() == 99 )		// ���Ϻ��� ��.
				pMover->SetMovePattern( 0 );		// �Ϲ��̵����� ��ȯ.
		}

		// ���ÿɼ��� �ִ� ���̴�.
		if( pProp->m_nLoot )
		{
			if( (pMover->GetCount() & 63) == 0 )		// ������ �ֺ��� ��ĵ�ؼ�.(63�̸� �� 4.2��)
			{	
				if( pProp->m_nLootProb == 0 || (int)( xRandom(100) ) < pProp->m_nLootProb  )	// ����Ʈ��(0)�� 100% / �̰ų� ����Ȯ���� ���� ���ýõ�
					if( m_bLootMove == FALSE )		// �����Ϸ� ���� ������ �� üũ�ϸ� �Ȋ�
						SubItemLoot();		// �������� ������.
			}
		}

		if( m_bLootMove == TRUE )
		{
			CCtrl *pCtrl = prj.GetCtrl( m_idLootItem );		// �׾������� ��������� �����ϱ� �˻���.
			if( IsInvalidObj(pCtrl) )		// ������ ������ �̵��߿� ������ ��������
			{
				MoveToDst( pMover->GetPos() );	// ���ڸ��� ��.
				m_idLootItem = NULL_ID;
				m_bLootMove = FALSE;
			}
		}

		// -1 �̸� �ƹ��͵� ��ĵ���� �ʴ´�!!!
		if( pProp->m_nScanJob != -1 )
		{
			CMover* pTarget = NULL;
			int nAttackFirstRange = pProp->m_nAttackFirstRange;
			if( nAttackFirstRange > 10 || nAttackFirstRange < 0 )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "m_nAttackFirstRange �̻�:%d %s" ), pProp->m_nAttackFirstRange, pMover->GetName() );
				return TRUE;
			}

			// � ���ǿ� �ش��ϴ� �÷��̾ �ִ��� �����Ѵ�.
			if( pProp->m_nScanJob || pProp->m_dwScanQuestId || pProp->m_dwScanItemIdx || pProp->m_nScanChao )
				pTarget = ScanTarget( pMover, nAttackFirstRange, pProp->m_nScanJob, pProp->m_dwScanQuestId, pProp->m_dwScanItemIdx, pProp->m_nScanChao );

			// ���ǿ� �´� �÷��̾ ���� �������̸� ������ ������ ������ �Ѵ�.
			if( pTarget == NULL && GetMover()->m_bActiveAttack )
				pTarget = ScanTarget( pMover, nAttackFirstRange, JOB_ALL );

			if( pTarget )
			{
				if( IsValidObj( (CObj*)pTarget ) )
				{

					if( (pMover->IsFlyingNPC() || pTarget->m_pActMover->IsFly() == FALSE) || pProp->dwClass == RANK_GUARD )	// ����� ������ ����.
					{
						// Ÿ���� �����ϸ� ���� ���� ���� 
						if( pTarget )	
						{
							g_xWSUserManager->AddDlgEmoticon( pMover, DLGEMOT_ATTACK );
							SendAIMsg( AIMSG_SETSTATE, STATE_RAGE_PATROL, pTarget->GetId() );
						}
					}
				}
			}	// if( target)
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

void CAIMonster::CallHelper( const MoverProp* pMoverProp ) 
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

	// �ش� ���ǿ� �´� ���͸� �����Ѵ�.
	int nRange = 20;
	switch( pMoverProp->m_nHelpRangeMul )
	{
	case 0: nRange = pMoverProp->m_nAttackFirstRange / 2;	break;
	default: nRange = pMoverProp->m_nAttackFirstRange * pMoverProp->m_nHelpRangeMul;	break;	// �þ߹����� ���.
	}

	FOR_LINKMAP( pMover->GetWorld(), vPos, pObj, nRange / MPU, CObj::linkDynamic, pMover->GetLayer() )
	{
		if( pObj != pMover && pObj->GetType() == OT_MOVER )
		{
			// FOR_LINKMAP ��ü�� ������ ������ �� ������ ���ʿ� �־���.
			for( int i = 0; i < pMoverProp->m_nCallHelperMax; i++ )
			{
				int nCallHelperIdx = pMoverProp->m_nCallHelperIdx[ i ];
				UNREFERENCED_PARAMETER( nCallHelperIdx );

				int nCallHelperNum = pMoverProp->m_nCallHelperNum[ i ];
				// �ƹ��� �θ��°��̰ų� || ���� ������ �θ��� �ɼ��϶� ���� �ε����ΰ�?
				// �׸���.............
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
	// ��Ƽ ���� ���� 
	pTarget = prj.GetMover( m_dwIdTarget );
	for( int i = 0; i < pMoverProp->m_nCallHelperMax; i++ )
		bParty |= pMoverProp->m_bCallHelperParty[ i ] ? TRUE : FALSE;

	if( bParty && IsValidObj( (CObj*)pTarget ) )
	{
		pParty = g_PartyMng.GetParty( pTarget->m_idparty );
		// ��Ƽ�� Ȯ���ϴٸ� ��Ƽ���� apPlayer�� �����Ѵ�.
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
					// ������ ���Ϳ��� Ÿ���� �������� �����Ѵ�. 
					CMover* pNewTarget = (CMover*)apPlayer[ rand() % nCountPlayer ];
					if( pNewTarget )
						apMonster[ j ][ i ]->PostAIMsg( AIMSG_SETSTATE, STATE_RAGE, pNewTarget->GetId() );
				}
			}
		}
		else
		{ 
			for( int j = 0; j < anCountMonster[ i ]; j++ )
				apMonster[ j ][ i ]->PostAIMsg( AIMSG_SETSTATE, STATE_RAGE, m_dwIdTarget );
		}
	}
}

DWORD CAIMonster::GetAtkMethod_Far()
{
	CMover* pMover = GetMover();
//	MoverProp* pMoverProp = pMover->GetProp();
	DWORD dwRandom = xRandom( 100 );

	// �ʻ� ���� ����!
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
DWORD CAIMonster::GetAtkMethod_Near()
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
DWORD CAIMonster::GetAtkRange( DWORD dwAtkMethod )
{
	PT_ITEM_SPEC pItemProp  = GetAtkProp( dwAtkMethod );
#ifdef _DEBUG
	if( pItemProp == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "ItemProp->dwAtk%d�� NULL�̴�. (%s)" ), dwAtkMethod, GetMover()->GetName() );
	}
#endif
	return pItemProp->dwAttackRange;
}

PT_ITEM_SPEC CAIMonster::GetAtkProp( DWORD dwAtkMethod )
{
	return g_xSpecManager->GetSpecItem( GetAtkItemId( dwAtkMethod ) );
}

DWORD CAIMonster::GetAtkItemId( DWORD dwAtkMethod )
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

void CAIMonster::DoAttack( DWORD dwAtkMethod, CMover* pTarget )
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
		// ��Ÿ� ������, ���õ� ���� ���(dwAtkMethod)�� �ش� ���� ������ �Ӽ����� �Ӽ��� ��� ��Ÿ� �����̶�� �Ǵ���.
		// �������� idSfx�� ���� �ڱ��ڽſ��� ��Ͻ��ѵ�. ���߿� Ŭ�󿡼� �¾Ƽ� ������ ������ �Ѿ���� �װͰ� ���ؼ� �������� �Ѱ���.
		int id = pMover->m_sfxHitArray.Add( 0, pTarget->GetId(), AF_GENERIC, 0 );	// �̺κ��� DoAttackRange������ ���� �Ұ� ����.
		DWORD dwItemID = dwAtkItem;
		if( dwItemID == NULL_ID )
			FLERROR_LOG( PROGRAM_NAME, _T( "��� : %s�� dwAtk2�� �������� �ʾҴ�" ), pMover->GetName() );

		if( pMover->DoAttackRange( pTarget, dwItemID, id ) != 1 )
			pMover->m_sfxHitArray.RemoveSfxHit( id, FALSE );	// sfxHitArray.Add�־������� �ٽû�. 
	}
	else
	{
		// �ʻ� ������, ������Ƽ���� ������ ���� �� �����Ƿ� ���� ���(dwAtkMethod)���� ����.
		// �ʻ� ���� ��ƾ�� ���ο��� �ܰŸ� ������ �� ���� �ִ�.
		DWORD dwItemID = dwAtkItem;
		if( dwItemID == NULL_ID )
			FLERROR_LOG( PROGRAM_NAME, _T( "��� : %s�� ����%d�� �������� �ʾҴ�" ), pMover->GetName(), dwAtkMethod );
		if( dwAtkMethod == 3 )
			pMover->DoAttackSP( pTarget, dwItemID );
		else
		{
			//int nError = pMover->SendActMsg( OBJMSG_ATK1, pTarget->GetId(), 0, 0 );
			//if( nError > 0 ) 
			if( pMover->DoAttackMelee( pTarget, OBJMSG_ATK1 ) )
 			{
				//���� ���� ������ �ޱ۰� pos�� ����.. �̻���
				const float fAngle = GetDegree(pTarget->GetPos(), pMover->GetPos() );
				if( pMover->GetAngle() != fAngle )
					pMover->SetAngle( fAngle );			// ��ǥ������ ���� ����.
				
				g_xWSUserManager->AddSetPosAngle( pMover, pMover->GetPos(), pMover->GetAngle() );

				//
				//g_xWSUserManager->AddMeleeAttack( pMover, OBJMSG_ATK1, pTarget->GetId(), 0, MAKELONG( 0, (WORD)nError ) );
			}
 			//else
 			//	nError = -1;		// �극��ũ ����Ʈ �������� �س�����.
		}
	}

	m_tmAttack	= TIMEGETTIME + s_tmAttack;		// ���� �� s_tmAttack�ʰ� �����ؿ�.
}

//
// ���� �����̰� ������ �ٽ� ���������� ���ƿö�.
// 11�� �Ź����ڵ��Դϴ�. �������� ����� �ڵ����� ����~ -xuzhu-
BOOL CAIMonster::SubAttackChance( const AIMSG & /*msg*/, CMover *pTarget )
{
	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();
	MoverProp *pProp = pMover->GetProp();
	int		nTargetJob = 0;
	if( pTarget->IsPlayer() )
		nTargetJob = pTarget->GetJob();		// Ÿ���� ����.
	
	BOOL bAttack = TRUE;
	D3DXVECTOR3	vTargetTo = pTarget->GetPos() - pMover->GetPos();	
	
//	BOOL bRangeAttack = (pProp->m_bRangeAttack[ nTargetJob ] & 0x7F) ? TRUE : FALSE;	// ��Ÿ� ���� AI�� �ֳ�.

	if( pProp->m_bRecvCond )	// ȸ�� AI�� �ִ���
	{
		if( pMover->GetHitPointPercent( 100 ) <= pProp->m_nRecvCondMe )	// HP�� m_nRecvCondMe% ���Ϸ� �������°�?
		{
			if( pMover->GetManaPoint() > 0 )
			{
				// m_nHow% ��ŭ�� HP�� ȸ��
				int nHP = (int)(pMover->GetMaxHitPoint() * (pProp->m_nRecvCondHow / 100.0f));	
				pMover->IncHitPoint( nHP );
				
				g_xWSUserManager->AddCreateSfxObj( pMover, XI_INDEX( 62, XI_GEN_CURE01 ), pMover->GetPos().x, pMover->GetPos().y, pMover->GetPos().z );	
				if( pProp->m_nRecvCondMP > 0 )		
				{
					// ������ �Ҹ�.
					int nMP = pProp->m_nRecvCondMP;			
					pMover->IncManaPoint( -nMP );
				} 
				else
				{
					// 0���� �����Ǿ� ������ ���� �Ҹ�.
					int nMP = pMover->GetMaxManaPoint();	
					pMover->IncManaPoint( -nMP );
				}
				bAttack = FALSE;		// ȸ���� ������ ������ �����Ͽ�.
			}
		}
		m_tmAttackDelay = TIMEGETTIME + SEC( 3 );	// 3���Ŀ� �ٽ� ���ݱ�ȸ.
	}

	if( bAttack )
	{
		if( m_bRangeAttack )
			m_dwAtkMethod = GetAtkMethod_Far();		// ���� ��� ���� 
		else
			m_dwAtkMethod = GetAtkMethod_Near();		// ���� ��� ���� 

		DWORD dwAtkRange = GetAtkRange( m_dwAtkMethod );
		// ��Ÿ� ���� �����̶��?
		if( dwAtkRange == AR_RANGE || dwAtkRange == AR_WAND )
		{
			// ��ȿ �Ÿ� �Ǵ�
			FLOAT fRange = (float)(pProp->m_bRangeAttack[ nTargetJob ] & 0x7F);
			fRange += 1.0f;		// ��Ÿ� +1���� ������ �� ���.
			if( IsInRange( vTargetTo, fRange ) == TRUE )		// AI���� ������ ���ݰŸ����� �ٰ����°�?
			{
				DoAttack( m_dwAtkMethod, (CMover*)pTarget );	// ����
				m_tmAttackDelay	= TIMEGETTIME + SEC( 3 );		// ��Ÿ� �����̴�. 3�� ���� ������ ����.
			}
		} 
		else
		{
			FLOAT fRange = pMover->GetAttackRange( dwAtkRange );
			if( pMover->IsRangeObj( pTarget, fRange ) )
			{
				DoAttack( m_dwAtkMethod, pTarget );		//  ����
				m_bGoTarget = TRUE;  // <- �̰� ���ϴ°�? 
				// ������ ������ ���� ������ ����.
				int nDelay = (int)pProp->dwReAttackDelay - SEC( 1 )  + xRandom( SEC( 2 ) );
				m_tmAttackDelay		= TIMEGETTIME + nDelay;
				FLTRACE_LOG( PROGRAM_NAME, _T( "AtkDelay:%d" ), (int)nDelay );
			}
		}
		// ���������� ARRIVAL���� ó���Ѵ�. ���⼭ ó���ص� ���� ������?
	}
	
	return TRUE;
}


//
// AI�� ���� ���ݹ�� ����.
//
// return : 0:�ٰŸ� 1:���Ÿ�
int CAIMonster::SelectAttackType( CMover *pTarget )
{
	int nAttackType = 0;

	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();
	MoverProp *pProp = pMover->GetProp();
	
	int		nTargetJob = 0;
	int		nRange = pProp->m_bRangeAttack[ nTargetJob ] & 0x7F;
	
	if( pTarget->IsPlayer() )		nTargetJob = pTarget->GetJob();		// Ÿ���� ����.

	BOOL bRange = ( pProp->m_bRangeAttack[ nTargetJob ] ) ? TRUE : FALSE;	// AI�� �Ÿ��� ������ ���Ÿ� AI����.
	BOOL bMelee = (BOOL)pProp->m_bMeleeAttack;								// AI�� �ٰŸ����ݵ� �ִ���.


	if( bRange && bMelee )		// ���Ÿ�/�ٰŸ� �Ѵ� ������ ����
	{
		D3DXVECTOR3	vDist = pTarget->GetPos() - pMover->GetPos();	// Ÿ�ٰ��� �Ÿ�.
		FLOAT		fDistSq = D3DXVec3LengthSq( &vDist );

		int nHalfRange = nRange / 2;		// ��Ÿ��� ��.
		if( fDistSq < (nHalfRange * nHalfRange) )	// �ʹ� ������ �پ��־� �Ÿ������� �����.
			nAttackType = 0;	// ������������ ����.
		else
			nAttackType = 1;	// �Ÿ��� ������ �����Ƿ� ���Ÿ� ����.
	} else
	if( bRange && bMelee == FALSE )		// ���Ÿ��� ������ ����
	{
		nAttackType = 1;
	} else
	if( bRange == FALSE && bMelee )		// �ٰŸ��� ������ ����
	{
		nAttackType = 0;
	}

	return nAttackType;
}

// SummonAI�� ������ ��ȯ�� �õ��Ѵ�. xx�� ����.
void CAIMonster::SubSummonProcess( CMover * /*pTarget*/ )
{
	if( TIMEGETTIME - m_tmSummon < SEC(10) )		// 20�ʸ��� �ѹ��� ��ȯ�� �õ�.
		return;

	m_tmSummon = TIMEGETTIME;

	CMover* pMover = GetMover();
	CWorld* pWorld = GetWorld();
	MoverProp *pProp = pMover->GetProp();
	// ��ȯ AI
	if( pProp && pProp->m_nSummProb )	// ���� ������ ��ȯ AI�� �ִ�.
	{
		for( vector<OBJID>::iterator it = m_vecIdSummon.begin(); it!=m_vecIdSummon.end(); )
		{
			CMover *pZako = prj.GetMover( (*it) );
			if( IsValidObj(pZako) && pZako->IsLive() )	// �׳ѵ��� ����ִ��������� �˻��ؾ��Ѵ�.
				it++;
			else
				it = m_vecIdSummon.erase( it );
		}
		
		if( pProp->m_nSummNum > (int)( m_vecIdSummon.size() ) )
		{
			if( (int)( xRandom(100) ) < pProp->m_nSummProb )	// ��ȯ �õ�.
			{
				D3DXVECTOR3 vLocal, vPos;
				CObj* pObj	= CreateObj( D3DDEVICE, OT_MOVER, (DWORD)pProp->m_nSummID );
				if( NULL == pObj )	
				{
					FLERROR_LOG( PROGRAM_NAME, _T( "��ȯ ���� %d" ), pProp->m_nSummID );
					return;
				}
				vLocal.x = 1.5f - xRandomF( 3.0f );		// ���� ������ �������� ����.
				vLocal.y = 0;
				vLocal.z = 1.5f - xRandomF( 3.0f );
				vPos = pMover->GetPos();
				vPos += vLocal;
				pObj->SetPos( vPos );
				pObj->InitMotion( MTI_STAND );
				pObj->UpdateLocalMatrix();
				((CMover*)pObj)->m_bActiveAttack = TRUE;	// ���ϴ� ���� ������ ��ȯ����.
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



BOOL CAIMonster::MoveProcessRage( const AIMSG & msg )
{
	CMover* pMover = GetMover();
	CWorld* pWorld = GetWorld();
	MoverProp *pProp = pMover->GetProp();
	
	// ������ ���°ų� �׾��� ��� �̵� ó�� ���� ���� 
	if( pMover->IsDie() || (pMover->m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL) )
		return FALSE;
	
	CMover* pTarget = prj.GetMover( m_dwIdTarget ); 

	DWORD dwAIState = pMover->m_dwAIInterfaceState;

	if( IsValidObj( pTarget ) == FALSE || ((CMover*)pTarget)->IsDie()
		|| pTarget->GetWorld() != pMover->GetWorld()
		)	
	{
//		int nAttackFirstRange	= pProp->m_nAttackFirstRange;
		CMover* pTarget	= NULL;
		if( CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon( pWorld->GetID() ) )
			pTarget	= ScanTargetNext( pMover, 160, m_dwIdTarget, m_uParty );
		else
			pTarget	= ScanTargetNext( pMover, 15, m_dwIdTarget, m_uParty );
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

	if( pTarget->IsRegionAttr( RA_SAFETY ) && pProp->dwClass != RANK_GUARD )	// Ÿ�� ���� ���� Ү
	{
		DoReturnToBegin();
		SendAIMsg( AIMSG_SETSTATE, dwAIState );
		return TRUE;
	}

	if( pMover->IsFlyingNPC() != ((CMover*)pTarget)->m_pActMover->IsFly() )	// this�� �������� �ƴϰ� ��밡 �������̶��
	{
		if( dwAIState == STATE_STAND )
			DoReturnToBegin();
		
		SendAIMsg( AIMSG_SETSTATE, dwAIState );
		return TRUE;
	}

	if( TIMEGETTIME > m_tmAttack )	// ���� ������ 20�� ������?
	{
		m_tmAttack	= TIMEGETTIME + s_tmAttack;	// Ÿ�̸� ����
		// 5���� ��ǥ�� ���� ��ǥ�� �Ÿ��� ���.
		D3DXVECTOR3 vDist = pMover->GetPos() - m_vOldPos;
		vDist.y	= 0;	// ������ ��¦�������µ� ������ Ȯ �ö����� �Ÿ��� ��û �ּ� �ִ�. �׷��� �̷��� �Ѱ�.
		if( IsInRange( vDist, 2.0f ) )	// 2���͸� ������ٸ� ��򰡿� �����ٴ� ��.
		{
			if( pMover->GetSpeed( pMover->m_pActMover->m_fSpeed ) > 0 )		// ���������� �������� ���ϰ� �Ȼ��¿��� �����ϸ� �ȵ�.
			{
				D3DXVECTOR3 vTarget	= pTarget->GetPos();
				pMover->SetPos( vTarget );	// �������� Ÿ�������� �ٷ� ����.
				g_xWSUserManager->AddSetPos( pMover, pMover->GetPos() );
			} 
		} 

		m_vOldPos = pMover->GetPos();	// ���� ��ǥ�� ������.
	}

	D3DXVECTOR3	vTargetTo = pTarget->GetPos() - pMover->GetPos();	
	// ������ �ǰ� ��ġ�� ���� 40 ���� �������� ���� ��ġ�� ���ư���.
	// ���� ��ġ�� ���� 40 + 15 OR 45 ���� �������� ���� ��ġ�� ���ư���.
	D3DXVECTOR3 v1	= pMover->GetPos() - m_vPosBegin;
	
	FLOAT fRange	= RANGE_MOVE;
	if( pMover->IsFlyingNPC() )
		fRange	*= 3.0F;
	
	if( IsInRange( v1, RANGE_RETURN_TO_BEGIN + fRange ) == FALSE
		&& !CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon( pWorld->GetID() )
		)
	{
		DoReturnToBegin();
		SendAIMsg( AIMSG_SETSTATE, dwAIState );
		return TRUE;
	}
	if( m_vPosDamage == D3DXVECTOR3( 0, 0, 0 ) )	// ���� ���� �� �˻� ����
		m_vPosDamage	= pMover->GetPos();

	D3DXVECTOR3 v2	= pMover->GetPos() - m_vPosDamage;
	if( IsInRange( v2, RANGE_RETURN_TO_BEGIN ) == FALSE
		&& !CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon( pWorld->GetID() )
		)
	{
		DoReturnToBegin();
		SendAIMsg( AIMSG_SETSTATE, dwAIState );
		return TRUE;
	}
	else if( pTarget->GetType() == OT_MOVER && ((CMover *)pTarget)->IsMode( TRANSPARENT_MODE ) )		// �i�� �ִ� Ÿ���� ����ȭ ���°� �Ǹ� 
	{
		if( CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon( pWorld->GetID() ) )
		{
			CMover* pTarget = ScanTargetNext( pMover, 160, m_dwIdTarget, m_uParty );	// ���� 50���� �̳��� ��ĵ�Ѵ�.
			if( pTarget	)
			{
				SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, pTarget->GetId() );
				return TRUE;
			}
		}

		if( dwAIState == STATE_STAND )
			DoReturnToBegin();	// SetTarget( NULL_ID, 0 )// Ÿ���� ������.
		SendAIMsg( AIMSG_SETSTATE, dwAIState );
		return TRUE;
	}
	else
	{
		m_bRangeAttack = SelectAttackType( pTarget );		// AI������ ���ݹ�� ����.

		if( m_bRangeAttack )	// ���Ÿ� �������� ��������.
		{
			int nTargetJob = pTarget->GetJob();
			if( nTargetJob < 0 )	nTargetJob = 0;		// ����Ʈ�� -1�� �ʱ�ȭ �ϱⶫ�� �˻��ؾ���.
			if( nTargetJob >= MAX_JOB )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "����ħ�� %d" ), nTargetJob );
				nTargetJob = 0;
			}

			FLOAT	fRange = (FLOAT)(pProp->m_bRangeAttack[ nTargetJob ] & 0x7F);		// ��Ÿ�
			BOOL	bKeepRange = (pProp->m_bRangeAttack[ nTargetJob ] & 0x80) ? TRUE : FALSE;	// ���� 1��Ʈ�� ��?
			FLOAT	fDist = D3DXVec3Length( &vTargetTo );		// ������ �Ÿ�.
			if( bKeepRange )
			{
				if( fabs(fDist - fRange) < 1.0f )	// ������ �Ÿ��� ��Ÿ��� ���̰� 1���� �̳��� ���̻� ������ �ʿ� ����.
				{
					if( pMover->IsEmptyDest() == FALSE )	// �̵�����.
						MoveToDst( pMover->GetPos() );		// ������ġ�� �̵� - �������� ��ž�� ���� ����. Ŭ�󿡼� ������ ��ġ�� �̵�.
				} else
				{
					// fRange�� �Ÿ��� �����ϵ��� ...
					if( fDist < fRange )	
					{
						FLOAT fAngle = GetDegreeX( pMover->GetPos(), pTarget->GetPos() );	// dest, src�� �Ķ���� ��ġ�� �ٲٸ� �ݴ������ ���´�.
						D3DXVECTOR3 vDst = pMover->GetPos() + VelocityToVec( fAngle, 10 );
						MoveToDst( vDst );		// �ݴ�������� �޷��� �Ÿ��� ����.
					} else
						MoveToDst( pTarget->GetId() );	// ��ǥ���� �Ÿ��� ��Ÿ����� �� �Ӵϱ� ������.
				}
			} else
			// not keepRange
			{
				if( fDist < fRange )		// ��Ÿ��ȿ� ������ ���ڸ��� ����.
				{
					if( pMover->IsEmptyDest() == FALSE )	// �̵�����.
						MoveToDst( pMover->GetPos() );		// ������ġ�� �̵� - �������� ��ž�� ���� ����. Ŭ�󿡼� ������ ��ġ�� �̵�.
				} else
					MoveToDst( pTarget->GetId() );	// ��Ÿ� ���̸� ��ǥ������ ����.
			}
		} else
		// �ٰŸ�
		{
			MoveToDst( pTarget->GetId() );		// �ٰŸ��� ������ ��ǥ������ ����.
		}

		if( pProp->m_nSummProb )			// Summon AI�� �ִ���?
			SubSummonProcess( pTarget );	// ��ȯ�� �õ�
		
		// ���� ����.
		if( TIMEGETTIME > m_tmAttackDelay )
		{
			if( SubAttackChance( msg, pTarget ) == FALSE )
				return FALSE;
		}
	}
	return TRUE;
}

BOOL CAIMonster::MoveProcessRagePatrol( const AIMSG & msg )
{
	CMover* pMover = GetMover();
	CWorld* pWorld = GetWorld();
	MoverProp *pProp = pMover->GetProp();
	
	// ������ ���°ų� �׾��� ��� �̵� ó�� ���� ���� 
	if( pMover->IsDie() || (pMover->m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL) )
		return FALSE;
	
	CMover* pTarget = prj.GetMover( m_dwIdTarget ); 

	DWORD dwAIState = pMover->m_dwAIInterfaceState;

	if( IsValidObj( pTarget ) == FALSE )
	{
		DoReturnToBegin();	// SetTarget( NULL_ID, 0 );
		SendAIMsg( AIMSG_SETSTATE, dwAIState );
		return TRUE;
	}
	
	if( ((CMover*)pTarget)->IsDie() )	// ��밡 �׾���.
	{
		DoReturnToBegin();	// SetTarget( NULL_ID, 0 );
		SendAIMsg( AIMSG_SETSTATE, dwAIState );
		return TRUE;
	}

	if( pMover->IsFlyingNPC() == FALSE && ((CMover*)pTarget)->m_pActMover->IsFly() )	// this�� �������� �ƴϰ� ��밡 �������̶��
	{
		DoReturnToBegin();
		SendAIMsg( AIMSG_SETSTATE, dwAIState );
		return TRUE;
	}

	if( TIMEGETTIME > m_tmAttack )	// ���� ������ 20�� ������?
	{
		m_tmAttack = TIMEGETTIME + s_tmAttack;		// Ÿ�̸� ����
		// 5���� ��ǥ�� ���� ��ǥ�� �Ÿ��� ���.
		D3DXVECTOR3 vDist = pMover->GetPos() - m_vOldPos;
		vDist.y = 0;					// ������ ��¦�������µ� ������ Ȯ �ö����� �Ÿ��� ��û �ּ� �ִ�. �׷��� �̷��� �Ѱ�.
		if( IsInRange( vDist, 2.0f ) )	// 2���͸� ������ٸ� ��򰡿� �����ٴ� ��.
		{
			if( pMover->GetSpeed( pMover->m_pActMover->m_fSpeed ) > 0 )		// ���������� �������� ���ϰ� �Ȼ��¿��� �����ϸ� �ȵ�.
			{
				D3DXVECTOR3 vTarget = pTarget->GetPos();
				pMover->SetPos( vTarget );		// �������� Ÿ�������� �ٷ� ����.
				g_xWSUserManager->AddSetPos( pMover, pMover->GetPos() );
			} 
		} 
		else
		{
			m_tmReturnToBegin = TIMEGETTIME;			// �ٽ� ���ư��� �ð��� ������.  x�ʰ��� �¾Ƶ� �ݰݾ��ϰ� ��� ����.
			DoReturnToBegin();
			SendAIMsg( AIMSG_SETSTATE, dwAIState );
			return TRUE;
		}
		m_vOldPos = pMover->GetPos();	// ���� ��ǥ�� ������.
	}

	D3DXVECTOR3	vTargetTo = pTarget->GetPos() - pMover->GetPos();	
	// ���� ��밡 ������ �� ���� ���� �ִٸ� ���� �����ϰ� ���ư���.
	if( IsInRange( vTargetTo, RANGE_CANCEL ) == FALSE )
	{
		DoReturnToBegin();	// SetTarget( NULL_ID, 0 );	// Ÿ���� ������.
		SendAIMsg( AIMSG_SETSTATE, dwAIState );
		return TRUE;
	}
	else if( pTarget->GetType() == OT_MOVER && ((CMover *)pTarget)->IsMode( TRANSPARENT_MODE ) )		// �i�� �ִ� Ÿ���� ����ȭ ���°� �Ǹ� 
	{
		if( CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon( pWorld->GetID() ) )
		{
			CMover* pTarget = ScanTargetNext( pMover, 160, m_dwIdTarget, m_uParty );	// ���� 50���� �̳��� ��ĵ�Ѵ�.
			if( pTarget	)
			{
				SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, pTarget->GetId() );
				return TRUE;
			}
		}

		DoReturnToBegin();	// SetTarget( NULL_ID, 0 );	// Ÿ���� ������.
		SendAIMsg( AIMSG_SETSTATE, dwAIState );
		return TRUE;
	}
	else
	{
		m_bRangeAttack = SelectAttackType( pTarget );		// AI������ ���ݹ�� ����.

		if( m_bRangeAttack )	// ���Ÿ� �������� ��������.
		{
			int nTargetJob = pTarget->GetJob();
			if( nTargetJob < 0 )	nTargetJob = 0;		// ����Ʈ�� -1�� �ʱ�ȭ �ϱⶫ�� �˻��ؾ���.
			if( nTargetJob >= MAX_JOB )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "����ħ�� %d" ), nTargetJob );
				nTargetJob = 0;
			}

			FLOAT	fRange = (FLOAT)(pProp->m_bRangeAttack[ nTargetJob ] & 0x7F);		// ��Ÿ�
			BOOL	bKeepRange = (pProp->m_bRangeAttack[ nTargetJob ] & 0x80) ? TRUE : FALSE;	// ���� 1��Ʈ�� ��?
			FLOAT	fDist = D3DXVec3Length( &vTargetTo );		// ������ �Ÿ�.
			if( bKeepRange )
			{
				if( fabs(fDist - fRange) < 1.0f )	// ������ �Ÿ��� ��Ÿ��� ���̰� 1���� �̳��� ���̻� ������ �ʿ� ����.
				{
					if( pMover->IsEmptyDest() == FALSE )	// �̵�����.
						MoveToDst( pMover->GetPos() );		// ������ġ�� �̵� - �������� ��ž�� ���� ����. Ŭ�󿡼� ������ ��ġ�� �̵�.
				} else
				{
					// fRange�� �Ÿ��� �����ϵ��� ...
					if( fDist < fRange )	
					{
						FLOAT fAngle = GetDegreeX( pMover->GetPos(), pTarget->GetPos() );	// dest, src�� �Ķ���� ��ġ�� �ٲٸ� �ݴ������ ���´�.
						D3DXVECTOR3 vDst = pMover->GetPos() + VelocityToVec( fAngle, 10 );
						MoveToDst( vDst );		// �ݴ�������� �޷��� �Ÿ��� ����.
					} else
						MoveToDst( pTarget->GetId() );	// ��ǥ���� �Ÿ��� ��Ÿ����� �� �Ӵϱ� ������.
				}
			} else
			// not keepRange
			{
				if( fDist < fRange )		// ��Ÿ��ȿ� ������ ���ڸ��� ����.
				{
					if( pMover->IsEmptyDest() == FALSE )	// �̵�����.
						MoveToDst( pMover->GetPos() );		// ������ġ�� �̵� - �������� ��ž�� ���� ����. Ŭ�󿡼� ������ ��ġ�� �̵�.
				} else
					MoveToDst( pTarget->GetId() );	// ��Ÿ� ���̸� ��ǥ������ ����.
			}
		} else
		// �ٰŸ�
		{
			MoveToDst( pTarget->GetId() );		// �ٰŸ��� ������ ��ǥ������ ����.
		}

		if( pProp->m_nSummProb )			// Summon AI�� �ִ���?
			SubSummonProcess( pTarget );	// ��ȯ�� �õ�
		
		// ���� ����.
		if( TIMEGETTIME > m_tmAttackDelay )
		{
			if( SubAttackChance( msg, pTarget ) == FALSE )
				return FALSE;
		}
	}
	return TRUE;
}

BOOL CAIMonster::StateRagePatrol( const AIMSG & msg )
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
		m_tmAttackDelay = TIMEGETTIME; // ���� �����̸� �ʱ�ȭ. �׷��� ������ ó�� ������ ������ �԰� ����.
		SetStop( 0 ); // 0���� �����ϸ� ���� ���°� �ٷ� ������ �Ǿ� ������ �����ϰ� �ȴ�. 
		              // ���� ���¿����� ���� ���¸� ���� �ð����� �������־���ϴµ�, �׷��� �Ǹ� ������ ���� �� �ִ�.

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT_TARGETCLEAR )		// Ÿ���� Ŭ�����ϰ� ������ ���ư�.
		SetTarget( NULL_ID, 0 );
		SendAIMsg( AIMSG_SETSTATE, STATE_PATROL );
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_PROCESS ) 
		MoveProcessRagePatrol( msg );
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DAMAGE ) 
		if( IsMove() && m_bGoTarget == FALSE )
		{
			if( xRandom( 2 ) == 0 )		// 1/2Ȯ���� �����ڸ� ����.
			{
				// �����ڿ��� ��ǥ�� ���� 
				MoveToDst( msg.dwParam1 );
			}
			m_bGoTarget = TRUE;
		}
		MoverProp* pMoverProp = pMover->GetProp();
//		int nHPPercent = pMover->GetHitPointPercent( 100 );
/*
		// ���� HP �ۼ�Ʈ���� ���� HP�ۼ�Ʈ�� ���� ��� ��������.
		if( nHPPercent < pMoverProp->m_nRunawayHP )
		{
			// ���� ���� ���� 
			SendAIMsg( AIMSG_SETSTATE, STATE_RUNAWAY );
			g_xWSUserManager->AddDlgEmoticon( pMover, DLGEMOT_EVADE );
		}
*/
		// Call HP �ۼ�Ʈ���� ���� HP�ۼ�Ʈ�� ���� ��� ���Ḧ �θ���.
		if( pMoverProp->m_bHelpWho )	// ����AI�� �������.
		{
			if( TIMEGETTIME - m_tmHelp > pMoverProp->m_tmUnitHelp )	// ���� m_tmHelp�� 0�̹Ƿ� ������ �Ѵ�.
			{
				if( m_bCallHelper == FALSE )
				{
					CallHelper( pMoverProp );
					g_xWSUserManager->AddDlgEmoticon( pMover, DLGEMOT_HELPER );
					m_tmHelp = TIMEGETTIME;
					if( pMoverProp->m_tmUnitHelp == 0 )		// 0�̸� �ѹ��� �θ��� �Ⱥθ�.
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
		if( m_dwIdTarget == msg.dwParam1 )
		{
			CMover* pTarget	= ScanTargetNext( pMover, /*pProp->m_nAttackFirstRange*/ 15, m_dwIdTarget, m_uParty );
			if( pTarget )
			{
				SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, pTarget->GetId() );
				return TRUE;
			}
			else
			{
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

BOOL CAIMonster::StateRage( const AIMSG & msg )
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
		m_tmAttackDelay = TIMEGETTIME; // ���� �����̸� �ʱ�ȭ. �׷��� ������ ó�� ������ ������ �԰� ����.
		SetStop( 0 ); // 0���� �����ϸ� ���� ���°� �ٷ� ������ �Ǿ� ������ �����ϰ� �ȴ�. 
		              // ���� ���¿����� ���� ���¸� ���� �ð����� �������־���ϴµ�, �׷��� �Ǹ� ������ ���� �� �ִ�.

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT_TARGETCLEAR )		// Ÿ���� Ŭ�����ϰ� ������ ���ư�.
		if( m_dwIdTarget == msg.dwParam1 )
		{
			CMover* pTarget	= ScanTargetNext( pMover, /*pProp->m_nAttackFirstRange*/ 15, m_dwIdTarget, m_uParty );
			if( pTarget )
			{
				SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, pTarget->GetId() );
				return TRUE;
			}
			else
			{
				SetTarget( NULL_ID, 0 );
				SendAIMsg( AIMSG_SETSTATE, dwAIState );
			}
		}
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_PROCESS ) 
		MoveProcessRage( msg );
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DAMAGE ) 
		if( IsMove() && m_bGoTarget == FALSE )
		{
			if( xRandom( 2 ) == 0 )		// 1/2Ȯ���� �����ڸ� ����.
			{
				// �����ڿ��� ��ǥ�� ���� 
				MoveToDst( msg.dwParam1 );
			}
			m_bGoTarget = TRUE;
		}
		MoverProp* pMoverProp = pMover->GetProp();
		int nHPPercent = pMover->GetHitPointPercent( 100 );

		// ���� HP �ۼ�Ʈ���� ���� HP�ۼ�Ʈ�� ���� ��� ��������.
		if( nHPPercent < pMoverProp->m_nRunawayHP )
		{
			// ���� ���� ���� 
			SendAIMsg( AIMSG_SETSTATE, STATE_RUNAWAY );
			g_xWSUserManager->AddDlgEmoticon( pMover, DLGEMOT_EVADE );
		}
		// Call HP �ۼ�Ʈ���� ���� HP�ۼ�Ʈ�� ���� ��� ���Ḧ �θ���.
		if( pMoverProp->m_bHelpWho )	// ����AI�� �������.
		{
			if( TIMEGETTIME - m_tmHelp > pMoverProp->m_tmUnitHelp )	// ���� m_tmHelp�� 0�̹Ƿ� ������ �Ѵ�.
			{
				if( m_bCallHelper == FALSE )
				{
					CallHelper( pMoverProp );
					g_xWSUserManager->AddDlgEmoticon( pMover, DLGEMOT_HELPER );
					m_tmHelp = TIMEGETTIME;
					if( pMoverProp->m_tmUnitHelp == 0 )		// 0�̸� �ѹ��� �θ��� �Ⱥθ�.
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
		if( m_dwIdTarget == msg.dwParam1 )
		{
			CMover* pTarget	= ScanTargetNext( pMover, /*pProp->m_nAttackFirstRange*/ 15, m_dwIdTarget, m_uParty );
			if( pTarget )
			{
				SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, pTarget->GetId() );
				return TRUE;
			}
			else
			{
				SetTarget( NULL_ID, 0 );
				SendAIMsg( AIMSG_SETSTATE, dwAIState );
			}
		}
		

	///////////////////////////////////////////////////////////////////////////////////////////////////
//	OnMessage( AIMSG_NOMOVE ) 
		// Ÿ�ٿ� �� ���� ���µ�, ��밡 ���� �ö���. �׷��ٸ� IDLE�� �ٲٰ� �׷��� �ʴٸ� ��������(��Ÿ� ���ݼ��� ��ֹ� ���� �ִ� ��Ȳ)
//		MoveToRandom( STATE_RAGE );

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

BOOL CAIMonster::MoveProcessRunaway()
{
	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();

	// ������ ���°ų� �׾��� ��� �̵� ó�� ���� ���� 
	if( pMover->IsDie() || (pMover->m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL))
		return FALSE;

	DWORD dwAIState = pMover->m_dwAIInterfaceState;

	CCtrl* pTarget = prj.GetCtrl( m_dwIdTarget ); // ��ǥ�� �缳�����ش�.

	// Ÿ���� ��ȿ�ΰ�?
	if( IsValidObj( pTarget ) == FALSE )
	{
		SetTarget( NULL_ID, 0 );
		SendAIMsg( AIMSG_SETSTATE, dwAIState );
	}
	else
	// ���� ��� �ð��� ������?
	if( IsEndStop()  )
	{
		CMover* pMoverTarget = (CMover*)pTarget;
		DWORD dwAttackRange = pMoverTarget->GetActiveHandItemProp()->dwAttackRange;
		FLOAT fRange = pMoverTarget->GetAttackRange( dwAttackRange ) * 1.7f;

		// ���� 20���� ����(��ó) �Ÿ��� �����ϸ� �����!
		if( IsInRange( pTarget->GetPos() - pMover->GetPos(), fRange ) == TRUE )
		{
			// 5�� ���� ������ ���� ����. ���� ũ�� Ŭ���� ������� ���·� �����δ�.
			pMover->AddAngle( (float)( 20 - xRandom( 40 ) ) );
			// ������ ���� �Ŀ� ���弱�� �ִ� ��ǥ�� ���Ѵ�.
			FLOAT fAngle = pMover->GetAngle();
			D3DXVECTOR3 vDst = pMover->GetPos() + VelocityToVec( fAngle, fRange );
			MoveToDst( vDst );
		}
		if( m_bTargetNoMovePos == FALSE )
		{
			// ���⼭ �޽� �����ð����� HP�� ä���ش�.
			if( ::TIMEGETTIME > pMover->m_dwTickRecovery )	// .
			{
				pMover->m_dwTickRecovery = ::TIMEGETTIME + NEXT_TICK_RECOVERY;
				pMover->IncHitPoint( pMover->GetHPRecovery() );
				// ü���� 80 % �̻� �Ǹ� �ݰ� �õ�.
				if( pMover->GetHitPointPercent( 100 ) >= 50 )
					SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, NULL_ID );
			}
		}
	}
	return TRUE;
}

BOOL CAIMonster::StateRunaway( const AIMSG & msg )
{
	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();

	DWORD dwAIState = pMover->m_dwAIInterfaceState;

	BeginAIHandler( )

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT ) 
		// �̰�� �������� �ɼǿ� ���� ���ְ� �ƴ϶�, ��밡 ������ �� ����, ������ ������ �ִ� ��ҿ�
		// �ֱ� ������ ������� ġ�� ����. 
		m_bTargetNoMovePos	= msg.dwParam2;
		// ������ �ݴ� �������� ���� ���� 
		// 5�� ���� ������ ���� ����. ���� ũ�� Ŭ���� ������� ���·� �����δ�.
		pMover->AddAngle( (float)( 180 + ( 20 - xRandom( 40 ) ) ) );
		// ������ ���� �Ŀ� ���弱�� �ִ� ��ǥ�� ���Ѵ�.
		CMover* pMoverTarget = (CMover*)prj.GetCtrl( m_dwIdTarget ); // ��ǥ�� �缳�����ش�.
		if( IsValidObj(pMoverTarget) )
		{
			DWORD dwAttackRange = pMoverTarget->GetActiveHandItemProp()->dwAttackRange;
			FLOAT fRange = pMoverTarget->GetAttackRange( dwAttackRange ) * 1.7f;
			FLOAT fAngle = pMover->GetAngle();
			D3DXVECTOR3 vDst = pMover->GetPos() + VelocityToVec( fAngle, fRange );
			MoveToDst( vDst );
			m_bFirstRunaway = TRUE; // ó�� �����ΰ�?
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
		// ������ �ݴ� �������� ���� ���� 
		// 5�� ���� ������ ���� ����. ���� ũ�� Ŭ���� ������� ���·� �����δ�.
		pMover->AddAngle( (float)( 180 + ( 20 - xRandom( 40 ) ) ) );
		// ������ ���� �Ŀ� ���弱�� �ִ� ��ǥ�� ���Ѵ�.
		FLOAT fAngle = pMover->GetAngle();
		D3DXVECTOR3 vDst = pMover->GetPos() + VelocityToVec( fAngle, 5 );
		MoveToDst( vDst );
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DIE ) 
	///////////////////////////////////////////////////////////////////////////////////////////////////
//	OnMessage( AIMSG_DSTDAMAGE ) 
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DSTDIE ) 
		if( m_dwIdTarget == msg.dwParam1 )
		{
			CMover* pTarget	= ScanTargetNext( pMover, /*pProp->m_nAttackFirstRange*/ 15, m_dwIdTarget, m_uParty );
			if( pTarget )
			{
				SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, pTarget->GetId() );
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
			SetStop( pMover->GetProp()->m_dwRunawayDelay + xRandom( SEC( 1 ) ) ); // + xRandom�� �ణ�� ���� ���̸� �ֱ� ���� ��.

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_EXIT )	

	EndAIHandler( )

	return TRUE;
}

void	CAIMonster::SetTarget( OBJID dwIdTarget, u_long uParty )
{
	m_dwIdTarget	= dwIdTarget;
	m_uParty	= uParty;
	if( dwIdTarget == NULL_ID )
		GetMover()->SetSpeedFactor( 1.0F );
}


CMonsterSkill::CMonsterSkill()
{

}


CMonsterSkill::~CMonsterSkill()
{
	Clear();
}

void CMonsterSkill::Clear()
{
	for( MAPMONSTERSKILL::iterator it=m_mapMonsterSkill.begin(); it!=m_mapMonsterSkill.end(); it++ )
		it->second.clear();

	m_mapMonsterSkill.clear();
}


CMonsterSkill* CMonsterSkill::GetInstance( void )
{
	static CMonsterSkill sMonsterSkill;
	return &sMonsterSkill;
}

void CMonsterSkill::LoadScript()
{
	CLuaBase Lua;

	const TCHAR szFileName[]	= _T( "MonsterSkill.lua" );

	TCHAR szFullPath[ MAX_PATH ]	= { 0, };
	g_pScriptFileManager->GetScriptFileFullPath( szFullPath, _countof( szFullPath ), szFileName );

	if( Lua.RunScript( szFullPath ) != 0 )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "[ FAILED LOAD SCRIPT. FILE_NAME:(%s) ]" ), szFullPath );
		exit(0);
	}

	Lua.GetGloabal( "tMonsterSkill" );
	Lua.PushNil();
	while( Lua.TableLoop( -2 ) )
	{
		DWORD dwMonsterId = CScript::GetDefineNum( Lua.GetFieldToString( -1, "strMonsterId" ) );
		vector<__MONSTERSKILL> vecMonsterSkill;

		//lua_getfield( Lua.GetLuaState(), -1, "tSkill" );
		Lua.GetField( -1, "tSkill" );
		Lua.PushNil();
		while( Lua.TableLoop( -2 ) )
		{
			__MONSTERSKILL monsterSkill;
			monsterSkill.dwAtkMethod	= static_cast<DWORD>( Lua.GetFieldToNumber( -1, "nAtkMethod" ) );
			monsterSkill.dwSkillID	= CScript::GetDefineNum( Lua.GetFieldToString( -1, "strSkillId" ) );
			monsterSkill.dwSkillLV	= static_cast<DWORD>( Lua.GetFieldToNumber( -1, "dwSkillLv" ) );
			monsterSkill.nHitCount	= static_cast<int>( Lua.GetFieldToNumber( -1, "nHitCount" ) );
			monsterSkill.nRange = static_cast<int>( Lua.GetFieldToNumber( -1, "nRange" ) );
			monsterSkill.nApplyProbabilty = static_cast<int>( Lua.GetFieldToNumber( -1, "nProb" ) );
			monsterSkill.dwSkillTime = static_cast<DWORD>( Lua.GetFieldToNumber( -1, "dwSkillTime" ) );
			monsterSkill.bIgnoreSkillProb = Lua.GetFieldToBool( -1, "bIgnore" ) ? true : false;

			SkillProp* pSkillProp = prj.GetSkillProp( monsterSkill.dwSkillID );
			if( !pSkillProp || monsterSkill.dwSkillLV < 1	|| monsterSkill.dwSkillLV > pSkillProp->dwExpertMax )
			{ 
				FLERROR_LOG( PROGRAM_NAME, _T( "Out of Skill Level Range -> CMonsterSkill::LoadScript() - %d, %s, %d" ), dwMonsterId, Lua.GetFieldToString( -1, "strSkillId" ), monsterSkill.dwSkillLV );
				FLASSERT(0);
			}
			
			vecMonsterSkill.push_back( monsterSkill );
			Lua.Pop( 1 );
		}

		m_mapMonsterSkill.insert( make_pair( dwMonsterId, vecMonsterSkill ) );
		Lua.Pop( 2 );
	}

	Lua.Pop(0);
}

DWORD CMonsterSkill::GetMonsterSkillLevel( CMover* pAttacker, DWORD dwSkillId )
{
	MAPMONSTERSKILL::iterator it = m_mapMonsterSkill.find( pAttacker->m_dwIndex );
	if( it == m_mapMonsterSkill.end() )
		return 0;

	vector<__MONSTERSKILL> &vecMonsterSkill	= it->second;
	for( int i=0; i<(int)( vecMonsterSkill.size() ); i++ )
	{
		if( vecMonsterSkill[i].dwSkillID == dwSkillId )
			return vecMonsterSkill[i].dwSkillLV;
	}

	return 0;
}

BOOL CMonsterSkill::ApplySkill( CMover* pAttacker, CMover* pTarget, DWORD dwAtkMethod )
{
	if( !IsValidObj( pAttacker ) || !IsValidObj( pTarget ) )
		return FALSE;

	MAPMONSTERSKILL::iterator it = m_mapMonsterSkill.find( pAttacker->m_dwIndex );
	if( it == m_mapMonsterSkill.end() )
		return FALSE;

	vector<__MONSTERSKILL> &vecMonsterSkill	= it->second;
//	vecMonsterSkill.assign( it->second.begin(), it->second.end() );
	
	// ���� ��ų�� �����Ѵ�.
	BOOL bSuccess = FALSE;
	int nTotalProb = 0;
	int nProb = xRandom( 100 );
	int	i = 0;
	for( i=0; i<(int)( vecMonsterSkill.size() ); i++ )
	{
		if( vecMonsterSkill[i].dwAtkMethod != dwAtkMethod )
			continue;

		nTotalProb += vecMonsterSkill[i].nApplyProbabilty;
		if( nProb < nTotalProb )
		{
			bSuccess = TRUE;
			break;
		}
	}

	if( !bSuccess )
		return FALSE;

	// ��ų�� �����Ǿ���. �������� ��ų���� �˻��Ѵ�.
	SkillProp* pSkillProp = prj.GetSkillProp( vecMonsterSkill[i].dwSkillID );
	if( !pSkillProp ) 
		return FALSE;
	AddSkillProp* pAddSkillProp = prj.GetAddSkillProp( pSkillProp->dwSubDefine + vecMonsterSkill[i].dwSkillLV - 1 );
	if( !pAddSkillProp ) 
		return FALSE;

	if( pSkillProp->dwID == SKILL_INDEX( 65, SI_MAG_FIRE_HOTAIR ) )
		return FALSE;

	if( vecMonsterSkill[i].nHitCount != pAttacker->m_pActMover->m_nMotionHitCount )
		return FALSE;
	
	// ��ų ���ӽð��� �����ϱ� ���� ����صд�.
	DWORD dwReferTarget1Backup = pSkillProp->dwReferTarget1;
	DWORD dwReferTarget2Backup = pSkillProp->dwReferTarget2;
	DWORD dwSkillTimeBackup = pAddSkillProp->dwSkillTime;
	pSkillProp->dwReferTarget1 = NULL_ID;
	pSkillProp->dwReferTarget2 = NULL_ID;
	pAddSkillProp->dwSkillTime = vecMonsterSkill[i].dwSkillTime;
	
	//g_xWSUserManager->AddCreateSfxObj( pAttacker, pSkillProp->dwSfxObj ); // ��ų ���� ����Ʈ
	if( vecMonsterSkill[i].nRange == 0 ) // ������ 0�̸� Ÿ�����׸� ��ų ����
	{
		g_cSkillSystem->ApplySkill( pAttacker, pTarget, pSkillProp, pAddSkillProp, vecMonsterSkill[i].bIgnoreSkillProb );
		if( pSkillProp->dwSfxObj2 != NULL_ID )
			g_xWSUserManager->AddCreateSfxObj( pTarget, pSkillProp->dwSfxObj2 );
		g_xWSUserManager->AddCreateSfxObj( pTarget, pSkillProp->dwSfxObj3 );
	}
	else	// ���� ��ų�̸� �����ȿ� ��� ĳ���Ϳ��� ��ų ����
	{		
		CObj* pObj = NULL;
		FOR_LINKMAP( pAttacker->GetWorld(), pAttacker->GetPos(), pObj, vecMonsterSkill[i].nRange, CObj::linkPlayer, pAttacker->GetLayer() )
		{
			if( pObj->GetType() == OT_MOVER && ((CMover*)pObj)->IsPlayer() && ((CMover*)pObj)->IsLive()
				&& pObj != pAttacker && pObj->IsRangeObj( pAttacker, (float)( vecMonsterSkill[i].nRange ) ) )
			{
				g_cSkillSystem->ApplySkill( pAttacker, (CMover*)pObj, pSkillProp, pAddSkillProp, vecMonsterSkill[i].bIgnoreSkillProb );
				if( pSkillProp->dwSfxObj2 != NULL_ID )
					g_xWSUserManager->AddCreateSfxObj( (CMover*)pObj, pSkillProp->dwSfxObj2 );
				g_xWSUserManager->AddCreateSfxObj( (CMover*)pObj, pSkillProp->dwSfxObj3 );
			}
		}
		END_LINKMAP
	}

	//������ ����ص� ��ų ���ӽð��� �ǵ�����.
	pSkillProp->dwReferTarget1 = dwReferTarget1Backup;
	pSkillProp->dwReferTarget2 = dwReferTarget2Backup;
	pAddSkillProp->dwSkillTime = dwSkillTimeBackup;

	return TRUE;
}

BOOL CMonsterSkill::MonsterTransform( CMover* pMover, int nMoverHP )
{
	if( !IsValidObj( pMover ) || !pMover->IsNPC() || pMover->GetProp()->m_MonsterTransform.dwMonsterId == NULL_ID )
		return FALSE;

	if( pMover->GetProp()->m_MonsterTransform.fHPRate < MulDiv( nMoverHP, 100, pMover->GetMaxHitPoint() ) )
		return FALSE;
	
	CWorld* pWorld = pMover->GetWorld();
	MoverProp* pMoverProp = prj.GetMoverProp( pMover->GetProp()->m_MonsterTransform.dwMonsterId );

	if( pWorld && pMoverProp )
	{
		CObj* pObj	= CreateObj( D3DDEVICE, OT_MOVER, pMoverProp->dwID );
		if( NULL == pObj )
			return FALSE;
		pObj->SetPos( pMover->GetPos() );
		pObj->InitMotion( MTI_STAND );
		pObj->UpdateLocalMatrix();
		((CMover*)pObj)->m_bActiveAttack = TRUE;
		((CMover*)pObj)->AddItToGlobalId();
		if( pWorld->ADDOBJ( pObj, FALSE, pMover->GetLayer() ) == TRUE )
		{
			g_xWSUserManager->AddCreateSfxObj( pMover, XI_INDEX( 248, XI_SKILL_MAG_EARTH_LOOTING01 ) );
			if( pMover->IsLive() && !pMover->IsDelete() )
			{
				pMover->Delete();
			}
		}
		else
		{
			SAFE_DELETE( pObj );
			return FALSE;
		}
	}
	else
		return FALSE;

	return TRUE;
}