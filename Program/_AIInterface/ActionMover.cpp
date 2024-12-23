#include "stdafx.h"
#include "ActionMover.h"
#include "..\_Common\ParticleMng.h"

#ifdef __CLIENT
#include "DialogMsg.h"
#include "../Neuz/DPClient.h"
extern CDialogMsg	g_DialogMsg;
extern	CDPClient	g_DPlay;
#endif

#ifdef __WORLDSERVER
#include "../WorldServer/User.h"
#endif

#include "party.h"
extern	CPartyMng	g_PartyMng;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CMeleeAtkMsgq::Process( CAction* pActMover )
{
#ifdef __CLIENT
	if( pActMover->GetMover()->IsActiveMover() )
		return FALSE;
#endif	// __CLIENT

	LPACTMSG lpActMsg	= GetHead();
	int nError;
	while( lpActMsg )
	{
		nError	= pActMover->SendActMsg( (OBJMSG)lpActMsg->dwMsg, lpActMsg->nParam1, lpActMsg->nParam2, lpActMsg->nParam3 );
		if( nError > 0 ) {
			RemoveHead();
			return( TRUE );
		}
		else if( nError == 0 ) {
			return( FALSE );
		}
		else if( nError < 0 ) {
			RemoveHead();
			lpActMsg	= GetHead();
		}
	}
	return( FALSE );
}

BOOL CMagicAtkMsgq::Process( CMover* pMover )
{
#ifndef __WORLDSERVER
	if( pMover->IsActiveMover() )
		return( FALSE );
#endif	// __WORLDSERVER

	LPMAGICATKMSG pMagicAtkMsg	= GetHead();
	int nError;
	CCtrl* pObj;
	while( pMagicAtkMsg )
	{
		pObj	= prj.GetCtrl( pMagicAtkMsg->nParam1 );
		if( pMagicAtkMsg->nParam2 == 0 )
			nError = pMover->DoAttackMagic( pObj, pMagicAtkMsg->nMagicPower, pMagicAtkMsg->idSfxHit );
		else
			nError = pMover->DoAttackRange( pObj, pMagicAtkMsg->nParam3, pMagicAtkMsg->idSfxHit );

		if( nError == 1 ) 
		{
			RemoveHead();
			return( TRUE );
		}
		else if( nError == 0 ) 
		{
			return( FALSE );
		}
		else if( nError == -1 ) 
		{
			RemoveHead();
			pMagicAtkMsg	= GetHead();
		}
	}
	return( FALSE );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __WORLDSERVER
	MEMPOOLER_IMPLEMENT( CActionMover, 1024 );
#else	// __WORLDSERVER
	MEMPOOLER_IMPLEMENT( CActionMover, 128 );
#endif	// __WORLDSERVER

CActionMover::CActionMover() 
{
	Init();
}
CActionMover::CActionMover( CMover* pMover ) : CAction( pMover )
{
	Init();
}

CActionMover::~CActionMover()
{
	Destroy();
}

void	CActionMover::Init( void )
{
	m_nCount		= 0;
	m_idTarget		= NULL_ID;
	m_objidHit		= NULL_ID;
	m_fTurnAngle	= 0.0f;
	m_fAccPower		= 0;
	m_fSpeed		= 0;		// �̰��� �۶� this�� ���ǵ带 ���Ѵ�.  ���߿��� ������� ����. ���͵� �⺻ �ٴ°� �ȴ�,.
	m_vDelta.x		= m_vDelta.y = m_vDelta.z = 0;
	m_vDeltaE.x		= m_vDeltaE.y = m_vDeltaE.z = 0;
	m_nMotionEx		= -1;
	m_bGround		= 0;
	m_bMove			= 0;
	m_nCastingTime	= 0;

	//sun: 10�� ���½ý���	Neuz, World, Trans
	m_dwCastingEndTick	= 0;
	m_nCastingTick	= 0;
	m_nCastingSKillID = 0;

	m_fDistance		= 0.0f;
	m_nShootCnt		= 0;
	m_nMotionHitCount = 0;
	m_fCurrentHeight = 0.0f;
	

#ifdef __WORLDSERVER
	m_nDeadCnt		= 0;	
#endif

#ifdef __CLIENT
	m_dwCtrlMsg		= 0;
	m_pTail			= NULL;
#endif
	m_vDeltaAccu	= m_vPosLast	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
}

void	CActionMover::Destroy( void )
{
#ifdef __CLIENT
	if( m_pTail )
		g_TailEffectMng.Delete( m_pTail );
	m_pTail = NULL;
#endif
}

BOOL	CActionMover::ProcessCollision( D3DXVECTOR3 *vPos )
{
	BOOL f	= FALSE;
	BOOL bFly = FALSE;
	
	CMover *pMover = m_pMover;
	MoverProp *pProp = pMover->GetProp();
	if( IsFly() || pProp->dwFlying == 1 )
		bFly = TRUE;
	else 
		bFly = FALSE;
	
	if( bFly )
		f	= ProcessCollisionFly( vPos );
	else
		f	= ProcessCollisionGround( vPos );
	m_vPosLast = *vPos;		// ������ ��ǥ���� �޾Ƶ�

	return( f );
}

//
// ProcessAction()ó���� �ѹ� �鸣�� ��, ProcessAction()�� �׹� ������ �̰��� �ѹ��� �鸥��
//
void	CActionMover :: ProcessPreAction( void )
{
	m_vDeltaAccu.x	= m_vDeltaAccu.y	= m_vDeltaAccu.z	= 0.0f;
}

//
//		���� �׼� ó��
//		���� ������ ���ۻ��¿� ���� ������ ����� �÷��� ����
//	vPos : pMover->m_vPos.  
//
int		CActionMover :: ProcessAction( const D3DXVECTOR3 *vPos )
{
	CMover* pMover = m_pMover;

	if( IsFly() == FALSE )
		m_vDelta.x	= m_vDelta.z	= 0.0f;		
//	m_vDelta.x	= m_vDelta.z = m_vDelta.y	= 0.0f;		
	
	BOOL bFly = FALSE;
	if( pMover->IsFlyingNPC() )		// ������ �����ΰ�.
		bFly = TRUE;

	// �ƹ��͵� �ƴѻ���
	if( GetState() == 0 ) 
	{
		SendActMsg( OBJMSG_STAND );
#ifdef __CLIENT
		if( pMover->IsActiveMover() )
		{
			if( IsFly() )
				g_DPlay.SendPlayerBehavior();
		}
#endif	// __CLIENT
	}
	else
	{
		float fSpeed = pMover->GetSpeed( m_fSpeed );

		//--------- ���� ó�� ----------------------------
		// �⺻������ �������� ���°� ���� �� �ִ�.
		// �����¿� / �̵��߻��¸� ó��
		if( GetState() & OBJSTA_MOVE_ALL )
		{
			ProcessState( GetState() & OBJSTA_MOVE_ALL, fSpeed );
		}
		
		if( GetState() & OBJSTA_TURN_ALL )
		{
			ProcessState( GetState() & OBJSTA_TURN_ALL, fSpeed );
		}
		
		if( GetState() & OBJSTA_LOOK_ALL )
		{
			ProcessState( GetState() & OBJSTA_LOOK_ALL, fSpeed );
		}
		
		if( GetState() & OBJSTA_JUMP_ALL )
		{
			ProcessState( GetState() & OBJSTA_JUMP_ALL, fSpeed );
		}
		
		if( GetState() & OBJSTA_ATK_ALL )
		{
			ProcessState( GetState() & OBJSTA_ATK_ALL, fSpeed );
		}
		
		if( GetState() & OBJSTA_DMG_ALL )
		{
			ProcessState( GetState() & OBJSTA_DMG_ALL, fSpeed );
		}
		
		if( GetState() & OBJSTA_ACTION_ALL )
		{
			ProcessState( GetState() & OBJSTA_ACTION_ALL, fSpeed );
		}
	}

	m_bMove = 1;
	if( m_vDelta.x == 0 && m_vDelta.z == 0 )	// x,z��ǥ �̵��� ����. �������� �ʾ���
		if( m_vDeltaE.x == 0 && m_vDeltaE.y == 0 && m_vDeltaE.z == 0 )
			m_bMove = 0;
	
	if( m_vDelta.y != 0 || m_bMove )			// delta x, y, z���� ������ �־��ٸ� "���ִ�" ����
		m_bGround = 0;	
	else if( fabs(vPos->y - m_vPosLast.y) > 0.001f )
		m_bGround = 0;
	
	if( IsFly() == FALSE )	
	{
		// ������ - �߷º��� ����
		if( !bFly && GetDmgState() != OBJSTA_DISAPPEAR )		// �׾� ��������� �߿� �߷�ó�� ����
		{
			if( m_bGround != 1 )		// ���� ���ִ� ��찡 �ƴҶ���, �߷� ó��
			{
				m_vDelta.y -= 0.005f;
				if( m_vDelta.y < -0.50f )		
					m_vDelta.y = -0.50f;
			}
		}
	} 
	else
		ProcessFlyMove();

	if( IsDie() )	
		m_vDeltaE.x = m_vDeltaE.y = m_vDeltaE.z = 0;

#ifdef __CLIENT
	if( m_vDeltaE.x >= 0.025f || m_vDeltaE.z >= 0.025f ) 
	{
		if( m_bGround )		// ������ �̲������� ����.
		{
			if( g_nProcessCnt & 1 )
				CreateSfx( g_Neuz.m_pd3dDevice, XI_INDEX( 1005, XI_NAT_DUST_RUN ), pMover->GetPos() );
		}
	}
#endif
	
	m_vDelta += m_vDeltaE;		// �ܺ����� ����.
	m_vDeltaAccu += m_vDelta;

	if( (GetState() & OBJSTA_DMG_FLY) == 0 )
	{
	//sun: 11, �浹 ���� ����
		if(D3DXVec3LengthSq(&m_vDeltaE) < 0.01f) 
		{
			m_vDeltaE.x = 0.0f; m_vDeltaE.y = 0.0f; m_vDeltaE.z = 0.0f;
		}
	
		m_vDeltaE *= 0.9f;			// �ܺ����� ���������� ��� ����.
	}

	m_nCount++;

	return 1;
}


int CActionMover::SendActMsg( OBJMSG dwMsg, int nParam1, int nParam2, int nParam3, int nParam4, int nParam5 )
{	
	if( IsFly() )
	{
		return ProcessActMsgFly( m_pMover, dwMsg, nParam1, nParam2, nParam3, nParam4, nParam5 );
	}
	else
	{
		return ProcessActMsg( m_pMover, dwMsg, nParam1, nParam2, nParam3, nParam4, nParam5 );
	}
}

void CActionMover::ProcessState( DWORD dwState, float fSpeed ) 
{ 
	if( IsFly() )
	{
		ProcessStateFly( m_pMover, dwState, fSpeed );
	}
	else
	{
		ProcessState( m_pMover, dwState, fSpeed );
	}
}


//
// XZ��� ������ Y(������)���� �׸��� ���� �Ѱܹ޾� ���͸� ����
// 6�ù��� 0�� �ð�ݴ�� ���ư��� 3�ù����� 90��
void	CActionMover::DoDamageFly( float fAngleXZ, float fAngleY, float fPower )
{
	float fTheta = D3DXToRadian( fAngleXZ );
	float fTheta2 = D3DXToRadian( fAngleY );
	m_vDelta.y += -cosf( fTheta2 ) * fPower;
	float fDist = sinf( fTheta2 ) * fPower;
	m_vDeltaE.x += sinf( fTheta ) * fDist;
	m_vDeltaE.z += -cosf( fTheta ) * fDist;
}

int	CActionMover::SendDamageForce( DWORD dwAtkFlags, OBJID idSender, int nParam, BOOL bTarget )	// ������
{
//sun: 10, ���׿���Ŀ AI ����
	// �غ��� ���ʹ� ���� �鿪
	if( m_pMover->IsRank( RANK_MIDBOSS ) )
		return SendActMsg( OBJMSG_DAMAGE, dwAtkFlags, idSender, nParam, bTarget );
	return SendActMsg( OBJMSG_DAMAGE_FORCE, dwAtkFlags, idSender, nParam, bTarget );
}