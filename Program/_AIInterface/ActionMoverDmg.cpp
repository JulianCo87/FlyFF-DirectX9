#include "stdafx.h"
#include "ActionMover.h"

#ifdef __CLIENT
#include "../Neuz/DPClient.h"
extern	CDPClient	g_DPlay;
#endif

#ifdef __WORLDSERVER
#include "../WorldServer/User.h"
#include "guild.h"

extern	CGuildMng			g_GuildMng;
#endif // __WORLDSERVER

#include "party.h"
#ifdef __WORLDSERVER
#include "../WorldServer/AttackArbiter.h"
#endif


extern	CPartyMng	g_PartyMng;


/////////////////////////////////////////////////////////////////////////////////////
// CActionMover
/////////////////////////////////////////////////////////////////////////////////////



DWORD CActionMover::_ProcessMsgDmg( DWORD dwMsg, CMover* pAttacker, DWORD dwAtkFlags, int nParam, BOOL bTarget, int nReflect )
{
#if		defined(__WORLDSERVER)
	CAttackArbiter arbiter( dwMsg, pAttacker, GetMover(), dwAtkFlags, nParam, bTarget, nReflect );
	return arbiter.OnDamageMsgW();
#elif	defined(__CLIENT)
	return OnDamageMsgC( dwMsg, pAttacker, dwAtkFlags, nParam );
#else
	return 0;
#endif
}

#ifdef	__CLIENT
DWORD CActionMover::OnDamageMsgC( DWORD dwMsg, CMover* pAttacker, DWORD dwAtkFlags, int nParam )
{
	CMover*	pMover = GetMover();
	BOOL bValid = IsValidObj( pMover ) && IsValidObj( pAttacker );

	if( !bValid || IsState( OBJSTA_DIE_ALL ) )
		return 0;

	if( IsSit() )										// �ɾ��ִٰ� ������ �ɱ����� �Ѵ�.
		ResetState( OBJSTA_MOVE_ALL );
	
	// �̵��߿� �������� ������ ������ �ʰ� ��� �̵�.
	if( IsMove() == FALSE )
	{
		SendActMsg( OBJMSG_STAND );
	}

	// �������� ���� ������ �����.
	if( pMover->IsNPC() && pAttacker->IsPlayer() )		// �������� NPC , ����Ŀ�� �÷��̾� �϶��� �����
	{
		pMover->m_idAttacker = pAttacker->GetId();		// �� �������� ����ΰ��� �����.
		pMover->m_idTargeter = pAttacker->GetId();	
	}
	pAttacker->m_idLastHitMover = pMover->GetId();		// ����Ŀ�� ���������� ���ȴ����� ������ �����.

	if( (dwAtkFlags & AF_GENERIC) )	
	{
		PT_ITEM_SPEC pAttackerProp = pAttacker->GetActiveHandItemProp();
		
		D3DXVECTOR3 vLocal;
		if( pAttackerProp && pAttackerProp->dwItemKind3 == IK3_YOYO )
		{
			vLocal    = pMover->GetPos();
			vLocal.y += 1.0f;
		}
		else
		{
			AngleToVectorXZ( &vLocal, pAttacker->GetAngle(), 1.0f );
			vLocal += pMover->GetPos();		//gmpbigsun : �ǰ��� �Ϲ� effect 09_12_17
			vLocal.y += 1.0f;			// 2006/6/20 xuzhu
		}

		if( pAttackerProp && pAttackerProp->dwSfxObj3 != NULL_ID )
			CreateSfx( g_Neuz.m_pd3dDevice, pAttackerProp->dwSfxObj3, vLocal );

		
		if( pAttackerProp && pAttackerProp->dwSfxObj5 != NULL_ID ) //gmpbigsun: ������ �Ϲ� effect 09_12_17
		{
			vLocal = pAttacker->GetPos( );		
			CreateSfx( g_Neuz.m_pd3dDevice, pAttackerProp->dwSfxObj5, vLocal );
		}
		
	}
	else if ( (dwAtkFlags & AF_MONSTER_SP_CLIENT) )
	{
		// hitter
		PT_ITEM_SPEC pAttackerProp = g_xSpecManager->GetSpecItem( nParam >> 16 );
		FLASSERT( pAttackerProp );
		DWORD dwSfxObj = pAttackerProp->dwSfxObj3;		// gmpbigsun:Ư�����ݿ� ����Ʈ�� �ִٸ� 3�����.
		if( dwSfxObj != NULL_ID )
			CreateSfx( D3DDEVICE, dwSfxObj, pMover->GetPos() );

		// attacker
		dwSfxObj = pAttackerProp->dwSfxObj5;
		if( NULL_ID != dwSfxObj )
			CreateSfx( D3DDEVICE, dwSfxObj, pAttacker->GetPos() );
	}

	pMover->m_nDmgCnt = 10;								// ��鸮�� �ð��̴�.

	pMover->SetDamagedMotion( pAttacker, dwAtkFlags );
	return 1;
}
#endif	// __CLIENT


