#include "stdafx.h"


#include "AIBear.h"
#include "../WorldServer/User.h"
#include "Party.h"

#ifdef __INTERNALSERVER
static const TCHAR LOG_BOSS_AI_BEAR[] = _T( "Boss(CAIBear)" );
#endif

extern	CPartyMng		g_PartyMng;

// Event Pattern Type
#define		EPT_READY			0		// �ƹ��͵� ���ϰ� ����.
#define		EPT_MOVING			1		// ����/�̵�
#define		EPT_ATTACKING		2		// ������.
#define		EPT_TRACKING		3		// ������.

// ClockWorks Attack Pattern
#define		CAT_NONE					0
#define		CAT_NORMAL					1		// �⺻����
#define		CAT_LOOT					2		// �μ����� ���� ����ħ 
#define		CAT_BUFFTIME				3		// ��Ƽ�ο� �Ѱ����� ��Ƴ��� �ҵ��� ������!!
#define		CAT_LASTER					4		// 

enum
{
	STATE_INIT = 1,
	STATE_APPEAR,		// �����
	STATE_IDLE,			// ������ - �������� �Ϲ� �ʵ忡 ��������ų�� �̰ͺ��� �����ϸ� �ȴ�.
	STATE_RAGE,			// ���ݻ���.
	STATE_SUPER_RAGE	// ���� ȭ�� ����.
};
BEGIN_AISTATE_MAP( CAIBear, CAIInterface )

	ON_STATE( STATE_INIT   , StateInit   )
	ON_STATE( STATE_IDLE   , StateIdle   )
	ON_STATE( STATE_RAGE   , StateRage   )
	ON_STATE( STATE_SUPER_RAGE   , StateRage   )
//	ON_STATE( STATE_RUNAWAY, StateRunaway  )

END_AISTATE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MEMPOOLER_IMPLEMENT( CAIBear, 4 );

CAIBear::CAIBear()
{
	Init();
}
CAIBear::CAIBear( CObj* pObj ) : CAIInterface( pObj )
{
	Init();
}

CAIBear::~CAIBear()
{
	Destroy();
}

void CAIBear::Init( void )
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

	memset( m_bEventFlag, 0, sizeof(m_bEventFlag) );
}

void CAIBear::Destroy( void )
{
}

void CAIBear::InitAI()
{
	m_cTimeBuff.Set( SEC(30) );
	m_vPosBegin = GetMover()->GetPos();
	PostAIMsg( AIMSG_SETSTATE, STATE_RAGE, NULL_ID );	// ���� �����·� �ʱ�ȭ.
#ifdef __INTERNALSERVER
	FLERROR_LOG( LOG_BOSS_AI_BEAR, _T( "CAIBear start" ) );
#endif
}

BOOL CAIBear::IsIdleMode()
{
	FUNCAISTATE pStateFunc	= GetAIStateFunc( STATE_IDLE );
	if( pStateFunc != NULL && pStateFunc == m_pStateFunc )
		return TRUE;

	return FALSE;
}

BOOL CAIBear::StateInit( const AIMSG & /*msg*/ )
{
	return TRUE;
}

BOOL CAIBear::MoveProcessIdle()
{
#ifdef __MOVER_STATE_EFFECT
	CMover* pMover = GetMover();

	if( IsValidObj( pMover ) == TRUE && pMover->m_pActMover->IsStateFlag( OBJSTAF_COMBAT ) == TRUE )
	{
		if( pMover->m_pActMover->SendActMsg( OBJMSG_MODE_PEACE ) == TRUE )
		{
			g_xWSUserManager->AddSendActMsg( pMover, OBJMSG_MODE_PEACE );
		}
	}
#endif // __MOVER_STATE_EFFECT

	return TRUE;
}

BOOL CAIBear::StateIdle( const AIMSG & msg )
{
//	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();
	
	BeginAIHandler( )
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT ) 
//		if( pMover->TryDelete_IntoIdleMode() == TRUE )
//			return TRUE;

////		SendAIMsg( AIMSG_SETPROCESS, TRUE );
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
//		SendAIMsg( AIMSG_SETPROCESS, FALSE );
		
	EndAIHandler( )
			return TRUE;
}

BOOL	CAIBear::SelectTarget( void )
{
	CMover	*pMover = GetMover();
//	CWorld	*pWorld = GetWorld();
//	CModelObject *pModel = (CModelObject *)pMover->GetModel();

	FLOAT fRadius = pMover->GetRadiusXZ();		// this�� ������
	FLOAT fRadiusSq = fRadius * fRadius;		// ������Sq����.
	
	CMover *pLastAttacker = prj.GetMover( m_idLastAttacker );
	if( pLastAttacker == NULL || (pLastAttacker && pLastAttacker->IsDie()) )			// LastAttacker�� ���������� Ÿ�� �ٽ� ���� �� �ֵ��� ����.
	{
		m_idLastAttacker = NULL_ID;
		pLastAttacker = NULL;
	} else
	{		
		D3DXVECTOR3 vDist = pLastAttacker->GetPos() - pMover->GetPos();
		FLOAT fDistSq = D3DXVec3LengthSq( &vDist );		// ��ǥ���������� �Ÿ�.
		if( fDistSq >= fRadiusSq * 10.0f )				// ��Ʈ����Ŀ�� �� �������� 10���̻� ������������
		{
			// Ÿ�� ����
#ifdef __INTERNALSERVER
			FLERROR_LOG( LOG_BOSS_AI_BEAR, _T( "LastAtker(%s)�� �ʹ� �־(%5.2f) Ÿ��������" ), pLastAttacker->GetName(), sqrt(fDistSq) );
#endif
			
			m_idLastAttacker = NULL_ID;
			pLastAttacker = NULL;
		}
	}


	m_idTarget = NULL_ID;
	m_vTarget.x = m_vTarget.y = m_vTarget.z = 0;	// �ϴ� �̰� �Ⱦ��°ɷ� ����.

	if( m_idLastAttacker == NULL_ID )		// ���� �� ���������� ����.
	{
		CMover* pTarget = NULL;
		pTarget = ScanTarget( pMover, 40, JOB_ALL );		// 40���� �ݰ泻���� �ƹ��� ����.
		if( pTarget )
		{
			// this�� ������ ���Ͱų� || Ÿ���� �������� �ƴҶ��� ����.
			if( pMover->IsFlyingNPC() || pTarget->m_pActMover->IsFly() == FALSE )	
			{
				m_idTarget = pTarget->GetId();
#ifdef __INTERNALSERVER
				FLERROR_LOG( LOG_BOSS_AI_BEAR, _T( "scanTarget:%s" ), pTarget->GetName() );
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
		DWORD dwAggroRate = 50;
		
		if( IsValidObj( pLastAttacker ) )
		{
			if( pLastAttacker->GetJob() == JOB_MERCENARY )		// ���������� ������ ������ �Ӽ��� ��׷� ���� ����.
				dwAggroRate = 70;
		}
		if( dwNum < dwAggroRate )		
		{
			// dwAggroRate% Ȯ���� ���������� �� ������ ����.
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
					FLERROR_LOG( LOG_BOSS_AI_BEAR, _T( "Leader:%s %d" ), pTarget->GetName(), pTarget->GetLevel() );
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
					FLERROR_LOG( LOG_BOSS_AI_BEAR, _T( "OverHealer:%s" ), pTarget->GetName() );
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
			if( pMover )
			{
				if( pTarget )
					FLERROR_LOG( LOG_BOSS_AI_BEAR, _T( "rand:%d LastAttacker:%s SelectTarget:%s" ), dwNum, pMover->GetName(), pTarget->GetName() );
				else
					FLERROR_LOG( LOG_BOSS_AI_BEAR, _T( "rand:%d LastAttacker:%s SelectTarget:%s" ), dwNum, pMover->GetName(), _T( "����" ) );
			}
			else
			{
				if( pTarget )
					FLERROR_LOG( LOG_BOSS_AI_BEAR, _T( "rand:%d LastAttacker:%s SelectTarget:%s" ), dwNum, _T( "����." ), pTarget->GetName() );
				else
					FLERROR_LOG( LOG_BOSS_AI_BEAR, _T( "rand:%d LastAttacker:%s SelectTarget:%s" ), dwNum, _T( "����." ), _T( "����" ) );
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

void CAIBear::SummonMonster( DWORD dwObjIndex, D3DXVECTOR3 vPos )
{
	CObj* pObj	= CreateObj( D3DDEVICE, OT_MOVER, dwObjIndex );
	if( pObj )
	{
		pObj->SetPos( vPos );
		pObj->InitMotion( MTI_STAND );
		pObj->UpdateLocalMatrix();
		((CMover*)pObj)->m_bActiveAttack = 1;		
		((CMover*)pObj)->SetGold(((CMover*)pObj)->GetLevel()*15);  // ���� ������ �⺻ ��ĸ� ����

		if( GetWorld()->ADDOBJ( pObj, TRUE, GetMover()->GetLayer() ) == FALSE )
		{
			SAFE_DELETE( pObj );
		}
	}
}

BOOL CAIBear::MoveProcessRage()
{
	CMover	*pMover = GetMover();
//	CWorld	*pWorld = GetWorld();
	CModelObject *pModel = (CModelObject *)pMover->GetModel();

	// �̺�Ʈ �޼���
	// �������Ͱ� �������� ���� �Ѵ�.
	CMover * pTarget = NULL;
	
	if( m_bEventFlag[0] == FALSE )
	{
		pTarget = ScanTarget( pMover, 80, JOB_ALL );		// 80���� �ݰ泻���� �ƹ��� ����.

		if( pTarget )
		{
			m_bEventFlag[0] = TRUE;

			g_DPCoreClient.SendCaption( prj.GetText(TID_GAME_BOSS_MUSHMOOT_MSG_01), pTarget->GetWorld()->GetID(), FALSE, pTarget->m_idparty );	

			D3DXVECTOR3 vLocal;
			D3DXVECTOR3 vPos = pTarget->GetPos();

			AngleToVectorXZ( &vLocal, pTarget->GetAngle(), 2.0f );
			vPos+=vLocal;
			DWORD dwIndex; 
			dwIndex = ( pTarget->GetWorld()->GetID() == WI_INSTANCE_CONTAMINTRAILS ? OBJECT_INDEX( 1553, MI_SMALL_MUSHPOIE2 ) : OBJECT_INDEX( 168, MI_SMALL_MUSHPOIE ) );
			
			if( pTarget->GetWorld()->GetID() != WI_DUNGEON_FL_MAS )
			{
				SummonMonster( dwIndex, vPos );
				vPos = pTarget->GetPos();
				AngleToVectorXZ( &vLocal, pTarget->GetAngle(), -2.0f );
				vPos+=vLocal;
				
				SummonMonster( dwIndex, vPos );	
			}
		}
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
		{
			bEnd = TRUE;		// ���� ������ �����°�. - ����� �̰� �ȳ־ �Ǵµ� ���� ���ݵ����� ���� ��ġ�� ���� ���涧�� ����ؼ� �־�θ� ��Ÿ.
		}
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
	
//			int ppp = pMover->GetHitPointPercent();


			if( fDistSq < fRadiusSq * 32.0f )		// �ٰŸ���.
			{
				DWORD dwRate = xRandom(100);

				if( dwRate > 85 )
				{
					if( m_nAttackType != CAT_LOOT )
					{
						m_nAttackType = CAT_LOOT;
						fArrivalRange = 7.0f;		
					}
				}
				else
				{
					m_nAttackType = CAT_NORMAL;		// �չ߷� ���.
					fArrivalRange = 7.0f;				// ���پ ��ƾ� �Ѵ�.
				}
			} 
			else
			{
				// ��û �ָ� �������ִ³��� ����.
				m_idTarget = NULL_ID;
				return FALSE;
			}
					
			if( m_cTimeBuff.IsTimeOut() )
			{
				m_nAttackType = CAT_BUFFTIME;
				m_cTimeBuff.Set( SEC(30) );
				fArrivalRange = 10.0f;	
			}
				
			m_tmTrace = timeGetTime();		// ���� ������ �ð�.
			m_nEvent = EPT_TRACKING;		// �����߰ݻ��� ��ȯ.
			pMover->CMD_SetMeleeAttack( m_idTarget, fArrivalRange );		// ���ݹݰ��� this�� ������ ũ�������� �����ϴ�.

#ifdef __INTERNALSERVER
			{
				CString strType;
				switch( m_nAttackType )
				{
				case CAT_NORMAL:		strType = "�⺻";	break;
				case CAT_LOOT:		strType = "�μճ���ħ-����";	break;
				case CAT_BUFFTIME:		strType = "�Ѽ� ����ħ";	break;
				}

				FLERROR_LOG( LOG_BOSS_AI_BEAR, _T( "AtkType:%s Dist:%5.2f" ), strType, sqrt(fDistSq) );
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
BOOL CAIBear::StateRage( const AIMSG & msg )
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
		case CAT_NORMAL:
			{
				if( xRandom(2) )
					dwMsg = OBJMSG_ATK1;
				else
					dwMsg = OBJMSG_ATK2;
				
				dwItemID = pProp->dwAtk1;
			}
			break;
		case CAT_LOOT:			 dwMsg = OBJMSG_ATK3;	dwItemID = pProp->dwAtk2;	break;
		case CAT_BUFFTIME:		 dwMsg = OBJMSG_ATK4;	dwItemID = pProp->dwAtk3;	break;
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

					CMover *pTarget = NULL;
					pTarget = prj.GetMover( m_idTarget );
					
					// �̺�Ʈ �޼���
					// �������Ͱ� �������� ���� �Ѵ�.
					switch( m_nAttackType )
					{
					case CAT_NORMAL:	
						{
						}
						break;
					case CAT_LOOT:
						{
							g_xWSUserManager->AddWorldShout( pMover->GetName(), prj.GetText(TID_GAME_BOSS_MUSHMOOT_MSG_02),
								pMover->GetPos(), pMover->GetWorld() );
						}
						break;
					case CAT_BUFFTIME:
						{
							g_DPCoreClient.SendCaption( prj.GetText(TID_GAME_BOSS_MUSHMOOT_MSG_03), pMover->GetWorld()->GetID(), FALSE, pTarget->m_idparty );

							// ��ȯ��
							int nCount = 2;
							D3DXVECTOR3 vLocal;
							D3DXVECTOR3 vPos = pTarget->GetPos();
							DWORD dwIndex; 
							dwIndex = ( pTarget->GetWorld()->GetID() == WI_INSTANCE_CONTAMINTRAILS ? OBJECT_INDEX( 1553, MI_SMALL_MUSHPOIE2 ) : OBJECT_INDEX( 168, MI_SMALL_MUSHPOIE ) );
							if( pTarget->GetWorld()->GetID() == WI_DUNGEON_FL_MAS )
								nCount = 0;
							while( nCount-- )
							{
								vPos.x += xRandomF( 9.0f ) + -4.0f;
								vPos.z += xRandomF( 9.0f ) + -4.0f;
								SummonMonster( dwIndex, vPos );
							}							
						}
						break;
					}
					
				} else
					m_nEvent = EPT_MOVING;			// ������ �����ؼ� �ٽ� �����·� ���ư�.
			}
		} else
			m_nEvent = EPT_MOVING;		// ���������� ����. ������� ����.
	}

	

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_ATTACK_MELEE )
		switch( m_nAttackType )			// ���õ� ���� ����.
		{
		case CAT_NORMAL:
			// ���� ����Ʈ �������� ���.
			// ���� ����Ʈ ����;
			// ���� ����;
			break;
		case CAT_LOOT:
			// �̻��� ������Ʈ ����;
			break;
		case CAT_BUFFTIME:		// ��� ���� Ÿ��.
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
//		SendAIMsg( AIMSG_SETPROCESS, FALSE );
	
	EndAIHandler( )
	return TRUE;
}





