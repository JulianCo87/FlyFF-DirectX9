#include "stdafx.h"

#include "AIClockWorks.h"
#include "../WorldServer/User.h"
#include "Party.h"

extern	CPartyMng		g_PartyMng;

#ifdef __INTERNALSERVER
static const TCHAR LOG_BOSS_AI_CLOCKWORKS[] = _T( "Boss(ClockWorks)" );
#endif

// Event Pattern Type
#define		EPT_READY			0		// �ƹ��͵� ���ϰ� ����.
#define		EPT_MOVING			1		// ����/�̵�
#define		EPT_ATTACKING		2		// ������.
#define		EPT_TRACKING		3		// ������.

// ClockWorks Attack Pattern
#define		CAT_NONE			0
#define		CAT_SLASH			1		// ũ�� ������ �Ʒ��� ���� ���.
#define		CAT_SHOOT			2		// ���ȷ� ������ �߻�
#define		CAT_LASER			3		// �������� �ܱ�.
#define		CAT_QUAKE			4		// �չ߷� ��Ƽ� �ĵ��� ����Ŵ.

enum
{
	STATE_INIT = 1,
	STATE_APPEAR,		// �����
	STATE_IDLE,			// ������ - �������� �Ϲ� �ʵ忡 ��������ų�� �̰ͺ��� �����ϸ� �ȴ�.
	STATE_RAGE,			// ���ݻ���.
	STATE_SUPER_RAGE	// ���� ȭ�� ����.
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
	PostAIMsg( AIMSG_SETSTATE, STATE_RAGE, NULL_ID );	// ���� �����·� �ʱ�ȭ.
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

		SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, NULL_ID );	// ������ IDLE������ ����. �ٷ� ���ݿ� ����.
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

	FLOAT fRadius = pMover->GetRadiusXZ();		// this�� ������
	FLOAT fRadiusSq = fRadius * fRadius;		// ������Sq����.
	
	CMover *pLastAttacker = prj.GetMover( m_idLastAttacker );
	if( pLastAttacker == NULL )			// LastAttacker�� ���������� Ÿ�� �ٽ� ���� �� �ֵ��� ����.
	{
		m_idLastAttacker = NULL_ID;
	} else
	{		
		D3DXVECTOR3 vDist = pLastAttacker->GetPos() - pMover->GetPos();
		FLOAT fDistSq = D3DXVec3LengthSq( &vDist );		// ��ǥ���������� �Ÿ�.
		if( fDistSq >= fRadiusSq * 10.0f )				// ��Ʈ����Ŀ�� �� �������� 10���̻� ������������
		{
			// Ÿ�� ����
#ifdef __INTERNALSERVER
			FLERROR_LOG( LOG_BOSS_AI_CLOCKWORKS, _T( "LastAtker(%s)�� �ʹ� �־(%5.2f) Ÿ��������" ), pLastAttacker->GetName(), sqrt(fDistSq) );
#endif
			
			m_idLastAttacker = NULL_ID;
			pLastAttacker = NULL;
		}
	}


	m_idTarget = NULL_ID;
	m_vTarget.x = m_vTarget.y = m_vTarget.z = 0;	// �ϴ� �̰� �Ⱦ��°ɷ� ����.

	if( m_idLastAttacker == NULL_ID )		// ���� �� ���������� ����.
	{
		// 2006.06.08 ata2k JOB_ALL�߰�
		CMover* pTarget = ScanTarget( pMover, 80, JOB_ALL );		// 80���� �ݰ泻���� �ƹ��� ����.
		if( pTarget )
		{
			// this�� ������ ���Ͱų� || Ÿ���� �������� �ƴҶ��� ����.
			if( pMover->IsFlyingNPC() || pTarget->m_pActMover->IsFly() == FALSE )	
			{
				m_idTarget = pTarget->GetId();
#ifdef __INTERNALSERVER
				FLERROR_LOG( LOG_BOSS_AI_CLOCKWORKS, _T( "scanTarget:%s" ), pTarget->GetName() );
#endif
				
			}
			else
				return FALSE;		// �� ã������ �� ����. - ������ �ٸ�Ÿ���� ã�ƾ߰���.
		} else
			return FALSE;
	} else
	// �� ���� ������ �ִ�.
	{
		DWORD dwNum = xRandom( 100 );		// 0 ~ 99������ ����.
		if( dwNum < 50 )		
		{
			// 50% Ȯ���� ���������� �� ������ ����.
			m_idTarget = m_idLastAttacker;		// �� ������ ������ Ÿ������ ��������.
		} else
		if( dwNum < 75 )
		{
			// 50���� �ݰ泻���� ���� ����� ����.
			CMover *pTarget = ScanTargetStrong( pMover, 50 );		
			if( pTarget )
			{
				// this�� ������ ���Ͱų� || Ÿ���� �������� �ƴҶ��� ����.
				if( pMover->IsFlyingNPC() || pTarget->m_pActMover->IsFly() == FALSE )	
				{
					m_idTarget = pTarget->GetId();
#ifdef __INTERNALSERVER
					FLERROR_LOG( LOG_BOSS_AI_CLOCKWORKS, _T( "Leader:%s %d" ), pTarget->GetName(), pTarget->GetLevel() );
#endif
				}
				else
					m_idTarget = m_idLastAttacker;		// Ÿ���� �����ϱⰡ ����ġ ������ ���������� �������� ��������.
			} else
				m_idTarget = m_idLastAttacker;		// Ÿ���� �����ϱⰡ ����ġ ������ ���������� �������� ��������.
		} else
		if( dwNum < 100 )
		{
			// �������ϴ� ��ø� ������.
			CMover *pTarget = ScanTargetOverHealer( pMover, 50 );		
			if( pTarget )
			{
				// this�� ������ ���Ͱų� || Ÿ���� �������� �ƴҶ��� ����.
				if( pMover->IsFlyingNPC() || pTarget->m_pActMover->IsFly() == FALSE )	
				{
					m_idTarget = pTarget->GetId();
#ifdef __INTERNALSERVER
					FLERROR_LOG( LOG_BOSS_AI_CLOCKWORKS, _T( "OverHealer:%s" ), pTarget->GetName() );
#endif
				}
				else
					m_idTarget = m_idLastAttacker;		// Ÿ���� �����ϱⰡ ����ġ ������ ���������� �������� ��������.
			} else
				m_idTarget = m_idLastAttacker;		// Ÿ���� �����ϱⰡ ����ġ ������ ���������� �������� ��������.
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
					FLERROR_LOG( LOG_BOSS_AI_CLOCKWORKS, _T( "rand:%d LastAttacker:%s SelectTarget:%s" ), dwNum, pMover->GetName(), _T( "����" ) );
			}
			else
			{
				if( pTarget )
					FLERROR_LOG( LOG_BOSS_AI_CLOCKWORKS, _T( "rand:%d LastAttacker:%s SelectTarget:%s" ), dwNum, _T( "����." ), pTarget->GetName() );
				else
					FLERROR_LOG( LOG_BOSS_AI_CLOCKWORKS, _T( "rand:%d LastAttacker:%s SelectTarget:%s" ), dwNum, _T( "����." ), _T( "����" ) );
			}

		}
#endif
	}

	// ������ ���ݴ���� ������.
	// .���� ���������� �� ������ - 50%. - ���� �����Ѵٴ� ������ ��.
	// .������ ���� �������γ�(�������� ���� �ִ³� Ȥ�� ���� ���� ��) - 25%
	// .������ �ϴ� ��� - 25%.
	// .���Ÿ��� ���� �����ִ� ������ �߾ӿ� �ִ³�. - 10% - �ϴ� �̰� ���߿�

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

	if( m_bDefenseMode == FALSE && pMover->GetHitPointPercent() <= 100 )		// 20%���Ϸ� �ǰ� ������
	{
		m_bDefenseMode = TRUE;
		pMover->SetDestParam( DST_ADJDEF_RATE, 800, NULL_CHGPARAM );			// �� 30%����.
	}


	if( m_nEvent == EPT_READY )
	{
		if( pMover->m_pActMover->m_bGround == 1 && pMover->m_pActMover->GetActionState() != OBJSTA_APPEAR )		// �ٴڿ� ��������..
			pMover->SendActMsg( OBJMSG_APPEAR );		// ��ȿ. �̰� Ŭ�󿡼� �������� �ȳ��ò���...
		if( ++m_nAppearCnt >= PROCESS_COUNT * 5 )		// 
		{
			PostAIMsg( AIMSG_END_APPEAR );		// ����� �ִϸ��̼��� �������� �˸�.
			pMover->m_pActMover->ResetState( OBJSTA_ACTION_ALL );
			m_nAppearCnt = 0;
		}
		// �ƹ��͵� ����.
	} else
	// ������ �ִ�.
	if( m_nEvent == EPT_ATTACKING )
	{
		BOOL bEnd = FALSE;

		if( pMover->m_dwMotion == MTI_STAND || pModel->IsEndFrame()	)
			bEnd = TRUE;		// ���� ������ �����°�. - ����� �̰� �ȳ־ �Ǵµ� ���� ���ݵ����� ���� ��ġ�� ���� ���涧�� ����ؼ� �־�θ� ��Ÿ.
		if( timeGetTime() >= m_tmTimeOver + SEC(15) )				// ���ݽ������� 10���̻���������� �� ����.
			bEnd = TRUE;
		if( bEnd )
		{
			m_nEvent = EPT_MOVING;				// �̵����·� ��ȯ.
			m_tmReattack = timeGetTime();		// ������ Ÿ�� ����;
			m_tmAddReattack = 2000; // xRandom( 2000 );	// x�� ���������� �������� ����� �� �ִ�.
			m_idTarget = NULL_ID;				// Ÿ���� Ŭ����.
		} 
	} else
	// ������ ���� �߰���.
	if( m_nEvent == EPT_TRACKING )
	{
		if( timeGetTime() >= m_tmTrace + 3000 )		// ������ �������� 3�ʰ� ������ ������ ����
		{
			m_nEvent = EPT_MOVING;
			m_idTarget = NULL;			// ���⼭ �̰� Ŭ���� ����� �Ʒ����� SelectTarget()�� �ٽ� �Ѵ� .
			m_vTarget.x = m_vTarget.y = m_vTarget.z = 0;			
		}
	} else
	//------------- �������� �ƴ�. --------------------
	{

		if( timeGetTime() >= m_tmReattack + (2000 - m_tmAddReattack) )		// ������ �ð��� �ƴ°�?  �⺻������ 5�ʰ��ݿ� AddReattack���� �پ�� �� ����.
		{
			FLOAT fRadius = pMover->GetRadiusXZ();		// this�� ������
			FLOAT fRadiusSq = fRadius * fRadius;		// ������Sq����.
			
			if( m_idTarget == NULL_ID &&										// ���ݴ���� �������� �ʾҰ�
				(m_vTarget.x == 0 && m_vTarget.y == 0 && m_vTarget.z == 0) )	// ������ġ�� �������� �ʾҴ�.
			{
				// � ������ �����ұ�....? �� ������.
				if( SelectTarget() == FALSE )
					return FALSE;
			}

			D3DXVECTOR3	vTarget;
			FLOAT	fDistSq = 0;					// ���������� this�� �Ÿ�.
			CMover *pTarget = NULL;
			if( m_idTarget != NULL_ID )		// Ÿ�ٿ������� ������
			{
				pTarget = prj.GetMover( m_idTarget );
				if( IsValidObj(pTarget) )
					vTarget = pTarget->GetPos();		// ������ǥ�� Ÿ�ٹ����� ��ǥ
				else
				{
					m_idTarget = NULL_ID;
					return FALSE;		// Ÿ���� �Žñ� �ϸ� ������.
				}
			} else
			if( m_vTarget.x && m_vTarget.y && m_vTarget.z )		// ���� ��ǥ�� �����Ǿ� ������.
			{
				vTarget = m_vTarget;
			} else
			{
//				FLERROR_LOG( PROGRAM_NAME, _T( "Ÿ���� ����" ) );
				return FALSE;
			}

			D3DXVECTOR3 vDist = vTarget - pMover->GetPos();
			fDistSq = D3DXVec3LengthSq( &vDist );		// ��ǥ���������� �Ÿ�.
			FLOAT fArrivalRange = fRadius;			// �󸶳� �����ؾ��ϴ°�? ����Ʈ�� ������ ����.

			if( fDistSq < fRadiusSq * 1.5f )		// �ٰŸ���.
			{
				m_nAttackType = CAT_QUAKE;		// �չ߷� ���.
				fArrivalRange = 0;				// ���پ ��ƾ� �Ѵ�.
			} else
			if( fDistSq < fRadiusSq * 3.0f )		// this�� ������x��Ÿ� ���� ������ �߰Ÿ�.
			{
				if( xRandom(100) < 50 )
					m_nAttackType = CAT_SLASH;	// Į�� ���
				else
					m_nAttackType = CAT_LASER;	// ������
				fArrivalRange = fRadius;		// ������ ���̸�ŭ ��������.
			
			} else
			if( fDistSq < fRadiusSq * 6.0f )		// �������� x�� ���ϴ� ���.
			{								
				// ������ 3��Ÿ� �̻��̸� ���Ÿ�.
				m_nAttackType = CAT_SHOOT;		// �����.
				fArrivalRange = 30.0f;			// xx���� ���� ��������.
			} else
			{
				// ��û �ָ� �������ִ³��� ����.
				m_idTarget = NULL_ID;
				return FALSE;
			}

//			m_nAttackType = CAT_LASER;	// ������
//			fArrivalRange = fRadius;		// ������ ���̸�ŭ ��������.
			
			m_tmTrace = timeGetTime();		// ���� ������ �ð�.
			m_nEvent = EPT_TRACKING;		// �����߰ݻ��� ��ȯ.
			pMover->CMD_SetMeleeAttack( m_idTarget, fArrivalRange );		// ���ݹݰ��� this�� ������ ũ�������� �����ϴ�.

#ifdef __INTERNALSERVER
			{
				CString strType;
				switch( m_nAttackType )
				{
				case CAT_SLASH:		strType = "����";	break;
				case CAT_SHOOT:		strType = "���";	break;
				case CAT_LASER:		strType = "������";	break;
				case CAT_QUAKE:		strType = "���";	break;
				}
				FLERROR_LOG( LOG_BOSS_AI_CLOCKWORKS, _T( "AtkType:%s Dist:%5.2f" ), strType, sqrt(fDistSq) );
			}
#endif
			
				
		} else	// ������ Ÿ�ֿ̹� ����.
		{
			// �����ϱ� ������ ��ġ�� �̵�����.
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
		// ������ ������ ���� �س���� ����ϰ� ����. ���߿� ��¡����.
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
		// �߰��Ͽ� �����ϸ� ���õǾ��� ���ݹ���� �����Ų��.
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
					m_nEvent = EPT_ATTACKING;		// ���ݻ��� ��ȯ.
					m_tmTimeOver = timeGetTime();	// ���ݽ����� Ÿ�ӿ��� ����
				} else
					m_nEvent = EPT_MOVING;			// ������ �����ؼ� �ٽ� �����·� ���ư�.
				

/*				CMover *pTarget = prj.GetMover( m_idTarget );
				if( IsValidObj(pTarget) )
				{
					int nError = pMover->SendActMsg( dwMsg, pTarget->GetId(), MAKELONG( 0, 0 ), MAKELONG( 0, 0 ) );
					if( nError > 0 ) 
					{
						g_xWSUserManager->AddMeleeAttack( pMover, dwMsg, pTarget->GetId(), MAKELONG( 0, 0 ), MAKELONG( 0, (WORD)nError ) );
					}
					m_nEvent = EPT_ATTACKING;		// ���ݻ��� ��ȯ.
				} else
					m_nEvent = EPT_MOVING;		// ���������� ����. ������� ����.
*/
			}
		} else
			m_nEvent = EPT_MOVING;		// ���������� ����. ������� ����.
	}

	

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_ATTACK_MELEE )
		switch( m_nAttackType )			// ���õ� ���� ����.
		{
		case CAT_SLASH:
			// ���� ����Ʈ �������� ���.
			// ���� ����Ʈ ����;
			// ���� ����;
			break;
		case CAT_LASER:
//			if( Ÿ�� ���� ���� )
			{
				//������ ��ǥ �̵�;
				//���� ������;
			}
			break;
		case CAT_SHOOT:
			// �̻��� ������Ʈ ����;
			break;
		case CAT_QUAKE:		// ��� ���� Ÿ��.
			// ���� ������;
			break;
		default:
			FLERROR_LOG( PROGRAM_NAME, _T( "AIMSG_ATTACK_MELEE %d" ), m_nAttackType );
			break;
		}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_END_APPEAR )	
		m_nEvent = EPT_MOVING;		// ������°� ������ ����/�̵����·� ��ȯ.

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_END_MELEEATTACK )	
		m_nEvent = EPT_MOVING;				// �̵����·� ��ȯ.
		m_tmReattack = timeGetTime();		// ������ Ÿ�� ����;
		m_tmAddReattack = xRandom( 2000 );	// x�� ���������� �������� ����� �� �ִ�.
		m_idTarget = NULL_ID;				// Ÿ���� Ŭ����.
				
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_EXIT )	
	
	EndAIHandler( )
	return TRUE;
}





