#include "stdafx.h"
#include <ActionMover.h>
#include "User.h"
#include "AttackArbiter.h"
#include <guild.h>
extern	CGuildMng			g_GuildMng;

#include <party.h>
extern	CPartyMng	g_PartyMng;


#ifdef __EVENT_MONSTER
#include <EventMonster.h>
#endif // __EVENT_MONSTER

#include <CreateMonster.h>

#include "FLCC_Reward.h"

#include "../_Common/FLSkillSystem.h"
#include "FLEventArenaGlobal.h"

#include "../_AIInterface/FLFSM.h"

//////////////////////////////////////////////////////////////////////
// ATTACK_INFO
//////////////////////////////////////////////////////////////////////

ATK_TYPE ATTACK_INFO::GetAtkType() const
{
	ATK_TYPE type = ATK_GENERIC;

	if( dwAtkFlags & AF_MELEESKILL )
	{
		type = ATK_MELEESKILL;
	}
	else if( dwAtkFlags & AF_MAGICSKILL )
	{
		type = ATK_MAGICSKILL;
	}	
	else if( dwAtkFlags & AF_MAGIC )		// �Ϲ� �ϵ� ����
	{
		type = ATK_MAGIC;
	}
	else if( dwAtkFlags & AF_FORCE )		// �ݻ� ���������� ��� 
	{
		type = ATK_FORCE;
	}

	return type;
}

// ���� ������ ����ΰ�?
BOOL ATTACK_INFO::CanIgnoreDEF() const		
{
	int nSkill = GetSkill();
	switch ( nSkill )
	{
	case SKILL_INDEX( 159, SI_BIL_PST_ASALRAALAIKUM ) :
		return TRUE;
	case SKILL_INDEX( 212, SI_JST_YOYO_HITOFPENYA ) :
		return TRUE;
	}

	if( dwAtkFlags & AF_FORCE )		// �̸���� �� �������� ���, ���� ���� (��: �ݻ�, ����ű ��)  
		return TRUE;
	return FALSE;
}

int ATTACK_INFO::GetChargeLevel() const 
{
	if( IsSkillAttack( dwAtkFlags ) )
		return 0;
	
	return ( nParam & 0xFF);			// �ϵ�������
}

int ATTACK_INFO::GetSkillLevel() const 
{
	FLASSERT( IsSkillAttack( dwAtkFlags ) );
	return ( nParam & 0xFF);	
}

int ATTACK_INFO::GetAttackCount() const 
{
	return ( nParam >> 8) & 0xFF;		// �Ϲ� 0, ���ӵ����� 1�̻� 
}

int ATTACK_INFO::GetSkill() const
{
	if( IsSkillAttack( dwAtkFlags ) )
		return ( nParam >> 16) & 0xFFFF;	// ��ų�̾��ٸ� ��ųID
	else
		return 0;
}

BOOL ATTACK_INFO::IsRangeAttack() const
{
	return ( dwAtkFlags & AF_RANGE );
}

//////////////////////////////////////////////////////////////////////
// CAttackArbiter
//////////////////////////////////////////////////////////////////////

CAttackArbiter::CAttackArbiter( DWORD dwMsg, CMover* pAttacker, CMover* pDefender, DWORD dwAtkFlags, int nParam, BOOL bTarget, int nReflect )
		: m_dwMsg( dwMsg ), 
		  m_pAttacker( pAttacker ), 
		  m_pDefender( pDefender ), 
		  m_dwAtkFlags( dwAtkFlags ), 
		  m_nParam( nParam ),
		  m_bTarget( bTarget ),
		  m_nReflect( nReflect )		  
{
	m_nReflectDmg = 0;
}

CAttackArbiter::~CAttackArbiter()
{
}

int CAttackArbiter::OnDamageMsgW()
{
	if( !CheckValidDamageMsg() )
	{
		return 0;
	}

	ATTACK_INFO info;
	info.dwAtkFlags = m_dwAtkFlags;
	info.pAttacker  = m_pAttacker;
	info.pDefender  = m_pDefender;
	info.nParam     = m_nParam;
	info.nParts     = PARTS_RWEAPON;

	int nDamage = 0;
	BYTE cbHandFlag	= GetHandFlag();
	for( BYTE cbFlag = 0x01; cbFlag <= 0x02; ++cbFlag )	
	{
		if( cbHandFlag & cbFlag )
		{
			info.nParts = (cbFlag & 0x01) ? PARTS_RWEAPON : PARTS_LWEAPON;
#ifdef __EVENTLUA_SPAWN
			const BOOL bAIOnekillMode	= m_pAttacker->m_pFSM != NULL && ( m_pAttacker->m_pFSM->GetOptionalFlags().bOneKillMode == TRUE );

			if( m_pDefender->IsNPC() && (prj.m_EventLua.IsEventSpawnMonster( m_pDefender->GetIndex() ) && ( !m_pDefender->IsRank( RANK_SUPER ) && !m_pDefender->IsRank( RANK_MIDBOSS ) && !m_pDefender->IsRank( RANK_MIDBOSS )))
				&& !m_pAttacker->IsMode( ONEKILL_MODE )
				&& bAIOnekillMode == FALSE )
			{
				nDamage = 1;
				continue;
			}
#endif // __EVENTLUA_SPAWN
			int n = CalcDamage( &info );					// �������� ���Ѵ�. (m_dwAtkFlags�� ����� �� �ִ�) 
			if( n > 0 )
			{
				if( m_pAttacker->IsPlayer() == TRUE && m_pDefender->IsPlayer() == TRUE )
				{
					if( m_pDefender->GetAdjParam( DST_IGNORE_DMG_PVP ) == 0 )
					{
						ProcessAbnormal( n, &info );			// ����, ����, ��, �ݻ� ����ó��
						StealHP( n, info.GetAtkType() );		// ����
					}
				}
				else
				{
					ProcessAbnormal( n, &info );			// ����, ����, ��, �ݻ� ����ó��
					StealHP( n, info.GetAtkType() );		// ����
				}

				nDamage += n;
			}
		}
	}

	if( m_pAttacker->IsPlayer() && m_pDefender->IsPlayer() )
	{
		nDamage -= (nDamage * m_pDefender->GetParam( DST_PVP_DMG_RATE, 0 ) / 100);
		if( !m_pAttacker->IsGuildCombatTarget( m_pDefender ) )
		{
			int nDamageDecRate = m_pAttacker->GetPKValue() * 5;
			if( nDamageDecRate > 99 )
				nDamageDecRate = 99;

			nDamage -= (int)( nDamage * ( (float)nDamageDecRate/(float)100) );
		}
	}

	DWORD dwFakeDamage = (DWORD)nDamage;
	if( m_pAttacker->IsPlayer() == TRUE && m_pDefender->IsPlayer() == TRUE )
	{
		int nMaxGuardPVPDamage = m_pDefender->GetAdjParam( DST_IGNORE_DMG_PVP );
		if( nMaxGuardPVPDamage > 0 )
		{
			IBuff* pBuff	= m_pDefender->m_buffs.GetBuffByDestParam( DST_IGNORE_DMG_PVP );
			if( pBuff != NULL )
			{
				int nOnPVPDamage = pBuff->GetAdjValue();
				nOnPVPDamage += nDamage;
				if( nOnPVPDamage < 0 )
				{
					m_pDefender->ResetDestParam( DST_IGNORE_DMG_PVP, 0, TRUE );
					m_pDefender->RemoveDstParamBuffs( DST_IGNORE_DMG_PVP );
				}
				else if( nOnPVPDamage >= nMaxGuardPVPDamage )
				{
					nDamage = nOnPVPDamage - nMaxGuardPVPDamage;
					m_pDefender->ResetDestParam( DST_IGNORE_DMG_PVP, 0, TRUE );
					m_pDefender->RemoveDstParamBuffs( DST_IGNORE_DMG_PVP );
				}
				else
				{
					pBuff->SetAdjValue( nOnPVPDamage );
					nDamage = 0;
				}
			}
		}
	}


	nDamage = max( nDamage, 1 );
	int nHP = MinusHP( &nDamage );							// HP ���� ��Ŵ 

	if( CMonsterSkill::GetInstance()->MonsterTransform( m_pDefender, nHP ) )
		return 0;

	if( m_pDefender->m_pActMover->IsSit() )					// �ɱ����� & �̵��� ���� 
		m_pDefender->m_pActMover->ResetState( OBJSTA_MOVE_ALL );

	// �̵����϶� ������ �ʴ´�.
	if( m_pDefender->m_pActMover->IsMove() == FALSE )
	{
		m_pDefender->m_pActMover->SendActMsg( OBJMSG_STAND );
	}

	m_pDefender->OnAttacked( m_pAttacker, nDamage, m_bTarget, m_nReflect );		// ������ �ǰ��� ����, ����ġ ������ ����	
	if( nHP > 0 )
	{
		OnDamaged( nDamage, dwFakeDamage );

		// MinusHP()���� ������ - ������ ���� ȸ�����Ѿ� �Ѵ�.
		int nActionHPRate = m_pDefender->GetAdjParam( DST_AUTOHP );							// �ߵ� HP��
		if( nActionHPRate > 0 )
		{
			float fHPPercent = (float)nHP / m_pDefender->GetMaxHitPoint();					// ���� HP�� �ۼ�Ʈ ��
			if( fHPPercent <= (nActionHPRate / 100.0f) )									// ���� HP�� �ߵ�HP���ϰ� �Ǹ�
			{
				float fAddHP = m_pDefender->GetChgParam( DST_AUTOHP ) / 100.0f;				// ȸ����ų HP��
				int nAddHP = (int)( m_pDefender->GetMaxHitPoint() * fAddHP );				// MaxHP�� ���������� ����.
				nHP += nAddHP;
				m_pDefender->SetPointParam( DST_HP, nHP, TRUE );									// HP����.
				g_xWSUserManager->AddCreateSfxObj( m_pDefender, XI_INDEX( 62, XI_GEN_CURE01 ) );					// ȸ�� ȿ�� �߻�.
				m_pDefender->RemoveBuff( BUFF_SKILL, SKILL_INDEX( 48, SI_ASS_HEAL_PREVENTION ) );	// ����.
			}
		}

		if( m_nReflectDmg )
		{
			m_pAttacker->m_pActMover->SendDamage( AF_FORCE, m_pDefender->GetId(), m_nReflectDmg, TRUE, 2 );
			m_nReflectDmg = 0;
		}
	}
	else	
		OnDied();											// ����ġ, ������ ��� 
	

	m_pDefender->EndPVP(nHP);

	return nDamage;
}

// �������� �Ա� ���� ������ �����ϴ°�? TRUE - ���� 
BOOL CAttackArbiter::CheckValidDamageMsg()
{
#ifdef __EVENT_MONSTER
	// �̺�Ʈ �����̰� ������ 15�Ѱ� ���̳��� MISS
	if( m_pAttacker->IsPlayer() && !m_pDefender->IsPlayer() )
	{
		MoverProp* lpMoverProp = m_pDefender->GetProp();
		if( lpMoverProp && CEventMonster::GetInstance()->SetEventMonster( lpMoverProp->dwID ) )
		{
			if( m_pAttacker->GetLevel() - m_pDefender->GetLevel() >= CEventMonster::GetInstance()->GetLevelGap() )
				m_dwAtkFlags = AF_MISS;
		}
	}
#endif // __EVENT_MONSTER

	if( m_pAttacker->IsPlayer() && !CCreateMonster::GetInstance()->IsAttackAble( static_cast<FLWSUser*>(m_pAttacker), m_pDefender, TRUE ) )
	{
		m_dwAtkFlags = AF_MISS;
		m_pDefender->m_idAttacker = NULL_ID;
		m_pDefender->m_idTargeter = NULL_ID;
	}

	if( m_dwAtkFlags & AF_MISS ) 
	{
		m_pDefender->PostAIMsg( AIMSG_DAMAGE, m_pAttacker->GetId(), 0 );
        g_xWSUserManager->AddDamage( m_pDefender, GETID( m_pAttacker ), 0, m_dwAtkFlags );
		return FALSE;
	}

	// �´½������� ��ƿüũ�� �ؾ��Ѵ�.
	// ���ÿ� ��ų�� ������ �ߵ��ϴ½����� ��ų�� ����ϱⶫ�� �Ѵ� ��ƿ���°� �ƴ�ä�� ������ �����̴�.
	// �ߵ��ϴ� �������� ���� ��ƿüũ�� �ؾ��ϴ°��� ������ ������ �����ϴ� ����� �ƿ� ������ �ʱ� ���ؼ� �̸� üũ �ؾ��ϱ⶧��.
	if( !g_eLocal.GetState( EVE_STEAL ) )
	{
		if( m_pAttacker->IsSteal( m_pDefender ) )	// m_pAttacker�� ��ƿ�Ϸ� �ϴ°�?
			return FALSE;					
	}

	return TRUE;
}

int CAttackArbiter::CalcATK( ATTACK_INFO* pInfo )
{
	int nATK = 0;
	int nCount = pInfo->GetAttackCount();		// �Ϲ� 0, ���ӵ����� 1�̻� 

	switch( pInfo->GetAtkType() )
	{
	case ATK_FORCE:								// �̸����� ������ (�ݻ�, ����ű ��)
		nATK = m_nParam;						
		nCount = 0;		
		break;
	case ATK_MELEESKILL:						// �и���ų 
		nATK = m_pAttacker->GetMeleeSkillPower( pInfo );
		break;
	case ATK_MAGICSKILL:						// ���� ��ų 
		nATK = m_pAttacker->GetMagicSkillPower( pInfo );
		break;
	case ATK_MAGIC:								// �ϵ� ���� 
		nATK = m_pAttacker->GetMagicHitPower( pInfo->GetChargeLevel() );
		break;
	case ATK_GENERIC:							// �Ϲ� ���� 
		nATK = m_pAttacker->GetHitPower( pInfo );
		break;
	}

	nATK	= (int)( nATK * m_pAttacker->GetATKMultiplier( m_pDefender, pInfo->dwAtkFlags ) );	// �������ݷ��� �����Ѵ�. 	
	if( nCount > 0 )															// ���ӵ������� ���� ATK 10%
		nATK	= (int)( nATK * 0.1f );

	nATK	= static_cast<int>( static_cast<float>( nATK ) * GetAdjustDamageFactor( *pInfo ) );

	nATK	+= m_pAttacker->GetParam( DST_ATKPOWER, 0 );

	if( _GetContentState( CT_COMPOSE_TWOWEAPON19 ) == CS_VER1 )
	{
		FLItemElem* pItemElem	= m_pAttacker->GetWeaponItem( pInfo->nParts );
		if( pItemElem != NULL )
		{
			nATK	+= pItemElem->GetCombinedAddDamage();
		}
	}

	if( m_pAttacker->IsPlayer() )
	{
		nATK += prj.m_EventLua.GetAttackPower() + nsCooperativeContributions::GLOBAL_REWARD_ALARM_ATTACK_POWER().GetRewardValue();
	}

	if( nATK < 0 )
		nATK	= 0;

	return nATK;
}


// �������� ���Ѵ�. (m_dwAtkFlags�� ����� �� �ִ�)
int CAttackArbiter::CalcDamage( ATTACK_INFO* pInfo )
{
	if( m_pAttacker->m_dwMode & ONEKILL_MODE )	// ��ų ���
		return m_pDefender->GetHitPoint();	

	if( m_pAttacker->m_pFSM != NULL && m_pAttacker->m_pFSM->GetOptionalFlags().bOneKillMode == TRUE )
		return m_pDefender->GetHitPoint();	

	int nDamage = PostCalcDamage( CalcATK( pInfo ), pInfo );
	m_dwAtkFlags = pInfo->dwAtkFlags;

	return nDamage;
}

int CAttackArbiter::OnAfterDamage( ATTACK_INFO* pInfo, int nDamage )
{
	CMover* pAttacker = pInfo->pAttacker;
	CMover* pDefender = pInfo->pDefender;

	if( pAttacker->IsPlayer() && nDamage > 0 )
	{
		if( pDefender->GetAdjParam( DST_CHRSTATE ) & CHS_DOUBLE )	// �������� ���� �ɷȴ�
			pDefender->RemoveChrStateBuffs( CHS_DOUBLE );
	}

	if( pDefender->GetAdjParam( DST_CHRSTATE) & CHS_ATK_COUNTERATTACK  ) // ���ݴ������� : ������ ����
	{
		if( pDefender->IsRangeObj( pAttacker->GetPos(), 3.0f ) )
		{
			pDefender->RemoveChrStateBuffs( CHS_ATK_COUNTERATTACK );

			int nLevel = 1;
			if( pDefender->IsPlayer() )
			{
				LPSKILL pSkill = pDefender->GetSkill(SKILL_INDEX( 204, SI_ACR_YOYO_COUNTER ));
				if( pSkill )
				{
#ifdef __SKILL0517
					nLevel	= pDefender->GetSkillLevel( pSkill );
#else	// __SKILL0517
					nLevel = pSkill->dwLevel;
#endif	// __SKILL0517
				}
			}
			g_cSkillSystem->DoUseSkill( pDefender, SKILL_INDEX( 223, SI_GEN_ATK_COUNTER ), nLevel, pAttacker->GetId(), SUT_NORMAL, FALSE, 0 );

			nDamage = 0;
			pInfo->dwAtkFlags = AF_MISS;
		}
	}
	else if( pDefender->GetAdjParam( DST_CHRSTATE) & CHS_DMG_COUNTERATTACK  ) // ���ݴ������� : ������ ����
	{
		if( pDefender->IsRangeObj( pAttacker->GetPos(), 3.0f ) )
		{
			pDefender->RemoveChrStateBuffs( CHS_DMG_COUNTERATTACK );
			g_cSkillSystem->DoUseSkill( pDefender, SKILL_INDEX( 223, SI_GEN_ATK_COUNTER ), pInfo->GetSkillLevel(), pAttacker->GetId(), SUT_NORMAL, FALSE, 0 );
		}
	}

	if( pInfo->GetSkill() == SKILL_INDEX( 212, SI_JST_YOYO_HITOFPENYA ) && nDamage > 0 )
	{
		nDamage = min( pAttacker->GetGold(), nDamage );
		pAttacker->AddGold( -nDamage );
	}

	if( pDefender->IsReturnToBegin() )
	{
		nDamage = 0;
		pInfo->dwAtkFlags = AF_MISS;
	}

	return nDamage;
}

enum POSTCALC_TYPE
{
	POSTCALC_DPC,				// Defense, Parry, Critical ����
	POSTCALC_MAGICSKILL,		// �Ӽ����� ���� ���� ��ų  
	POSTCALC_GENERIC,			// ��������  
};

// ���ݰ�� Ÿ��
POSTCALC_TYPE GetPostCalcType( DWORD dwAtkFlags )
{
	if ( dwAtkFlags & AF_MAGICSKILL )		// ���� ��ų
		return POSTCALC_MAGICSKILL;

	if ( dwAtkFlags & AF_GENERIC )			// �Ϲ� ����
		return POSTCALC_GENERIC;
	
	return POSTCALC_DPC;
}

int CAttackArbiter::PostCalcDamage( int nATK, ATTACK_INFO* pInfo )
{
	if ( pInfo->dwAtkFlags & AF_FORCE )		// 
		return nATK;

	CMover* pAttacker = pInfo->pAttacker;
	CMover* pDefender = pInfo->pDefender;

	// ����ó��: ������ų�� �ƴϸ�, ���Ͱ� �÷��̾ ������ �� �������� ���� ���ݷ�UP
	if( ((pInfo->dwAtkFlags & AF_MAGICSKILL) == 0) && pAttacker->IsNPC() && pDefender->IsPlayer() )	
	{
		int nDelta = pAttacker->GetLevel() - pDefender->GetLevel();
		if( nDelta > 0 )
		{
			float fpower = 1.0f + ( 0.05f * nDelta );
			nATK	= (int)( nATK * fpower );
		}
	}

	int nDamage = nATK;
	switch( GetPostCalcType( pInfo->dwAtkFlags ) )
	{
	case POSTCALC_DPC:
		nDamage = pDefender->ApplyDPC( nATK, pInfo );	// Defense, Parry, Critical ����
		break;
	case POSTCALC_MAGICSKILL:
		nDamage = pAttacker->PostCalcMagicSkill( nATK, pInfo );
		break;
	case POSTCALC_GENERIC:
		nDamage = pAttacker->PostCalcGeneric( nATK, pInfo );
		break;
	}

	if( nDamage <= 0 )
		return 0;

	nDamage += pAttacker->CalcLinkAttackDamage( nDamage );						// ��ũ���� ��ų�� �����Ѵ�.
	if( pInfo->GetSkill() == SKILL_INDEX( 159, SI_BIL_PST_ASALRAALAIKUM ) )
		nDamage += PostAsalraalaikum();

	nDamage	= (int)( nDamage * pAttacker->GetDamageMultiplier( pInfo ) );		// ���������� 
	nDamage = OnAfterDamage( pInfo, nDamage );
	return nDamage;
}	

int CAttackArbiter::PostAsalraalaikum()
{
	int nAddDmg = 0;

	LPSKILL pSkill = m_pAttacker->GetSkill( SKILL_INDEX( 159, SI_BIL_PST_ASALRAALAIKUM ) );
	DWORD dwSkillLevel = 
#ifdef __SKILL0517
		m_pAttacker->GetSkillLevel( pSkill );
#else	// __SKILL0517
		pSkill ? pSkill->dwLevel : 0;
#endif	// __SKILL0517

	if( m_pAttacker->IsNPC() )
		dwSkillLevel = CMonsterSkill::GetInstance()->GetMonsterSkillLevel( m_pAttacker, SKILL_INDEX( 159, SI_BIL_PST_ASALRAALAIKUM ) );

	switch( dwSkillLevel )
	{
		case 1:		nAddDmg = 20;	break;
		case 2:		nAddDmg = 30;	break;
		case 3:		nAddDmg = 40;	break;
		case 4:		nAddDmg = 50;	break;
		case 5:		nAddDmg = 60;	break;
		case 6:		nAddDmg = 70;	break;
		case 7:		nAddDmg = 80;	break;
		case 8:		nAddDmg = 90;	break;
		case 9:		nAddDmg = 100;	break;
		case 10:	nAddDmg = 150;	break;
	}

	int nMP = m_pAttacker->GetManaPoint();						// �ϴ� �޾Ƴ���
	if( m_pAttacker->IsPlayer() && m_pAttacker->IsSMMode( SM_MAINTAIN_MP ) == FALSE )		// MP ��������� �Ծ������� MP��� �ȵȴ�.
		m_pAttacker->SetPointParam( DST_MP, 0 );				// 0���� ����.
	return ( ( ( m_pAttacker->GetStr() / 10 ) * dwSkillLevel ) * ( 5 + nMP / 10 ) + nAddDmg );
}

//
//
void CAttackArbiter::ChanceSkill( int nDestParam, CCtrl *pAttackCtrl, CCtrl *pDefendCtrl )
{
#ifdef _DEBUG
	if( nDestParam == DST_CHR_CHANCESTEALHP )
	{
		int a = 0;
		a;
	}
#endif
	if( m_pAttacker->GetType() != OT_MOVER )	return;
	if( m_pDefender->GetType() != OT_MOVER )	return;
	
	CMover *pAttacker = (CMover *)pAttackCtrl;
	CMover *pDefender = (CMover *)pDefendCtrl;

	int nRate = pAttacker->GetAdjParam( nDestParam );
	if( nRate > 0 )	// �����ڰ� ��ų�ߵ�����
	{
		if( (int)( xRandom(100) ) <= nRate )
		{
			DWORD dwAddSkill = pAttacker->GetChgParam( nDestParam );		// Ÿ�ݽ� �ߵ��� �ֵ彺ų
			
			AddSkillProp *pAddSkillProp;
			pAddSkillProp = prj.GetAddSkillProp( dwAddSkill );
			if( pAddSkillProp )
				g_cSkillSystem->DoActiveSkill( pAttacker, pAddSkillProp->dwSkillPropID, pAddSkillProp->dwSkillLvl, pDefender );		// dwActiveSkill Lv1�� �ߵ���.
			//				m_pDefender->SetPoison( TRUE, m_pAttacker->GetId(), m_pAttacker->GetChgParam( DST_CHR_CHANCEPOISON ), m_pAttacker->m_tmPoisonUnit, m_pAttacker->m_wPoisonDamage );
		}
		
	}
}


// �����̻�(�ݻ�, ����, ����, ��)ó��
void CAttackArbiter::ProcessAbnormal( int nDamage, ATTACK_INFO* pInfo )
{
	int	nSkill = pInfo->GetSkill();

	//����ڰ� �ݻ�����̸�?  �����ڵ� �������� �Դ´�.
	int nReflectDmgRate = m_pDefender->GetAdjParam( DST_REFLECT_DAMAGE );
	if( nReflectDmgRate > 0 )
	{
		int nReflectRate = m_pDefender->GetChgParam( DST_REFLECT_DAMAGE );	

		BOOL bAble = TRUE;
		if( nReflectRate > 0 )			// �ݻ��ų Ȯ���� ������ Ȯ���˻� ��
		{
			if( (int)( xRandom(100) ) > nReflectRate )	// Ȯ�� ���г�?
				bAble = FALSE;
		}

		ATK_TYPE atkType = pInfo->GetAtkType();
		switch( atkType )
		{
		case ATK_MELEESKILL:
		case ATK_MAGICSKILL:
			if( m_pDefender->HasBuff( BUFF_SKILL, SKILL_INDEX( 165, SI_PSY_NLG_CRUCIOSPELL ) ) )		// ����ڰ� ũ��ÿ� ������ ���
				bAble = FALSE;
			break;
		case ATK_FORCE:															// �̸����� ������
			bAble = FALSE;
			break;
		}

		if( bAble )
		{
			int nDmg = (int)( nDamage * (float)(nReflectDmgRate / 100.0f) );
			if( m_pAttacker->IsPlayer() && m_pDefender->IsPlayer() )
				nDmg = (int)( nDmg * 0.1f );

			m_nReflectDmg = nDmg;
		}
	}

	// chipi_080325 - AF_MAGICSKILL �ΰ�� Ư�� ��ų�� ���� �����̻� ó��
	if( (m_dwAtkFlags & AF_MAGICSKILL) && m_pAttacker->IsPlayer() )
	{
		switch( nSkill )
		{
			case SKILL_INDEX( 158, SI_BIL_PST_BGVURTIALBOLD ) :
			case SKILL_INDEX( 159 ,SI_BIL_PST_ASALRAALAIKUM ) :
				ChanceSkill( DST_CHR_CHANCESTUN,	m_pAttacker, m_pDefender );
				break;
		}
	}
		
	if( m_dwAtkFlags & (AF_GENERIC | AF_MELEESKILL) )		// ������Ÿ/������ų�� �¾Ҵ�.
	{
		if( m_pAttacker->IsPlayer() )
		{
			ChanceSkill( DST_CHR_CHANCESTUN,	m_pAttacker, m_pDefender );
			ChanceSkill( DST_CHR_CHANCEPOISON,	m_pAttacker, m_pDefender );
			ChanceSkill( DST_CHR_CHANCEDARK,	m_pAttacker, m_pDefender );
			ChanceSkill( DST_CHR_CHANCEBLEEDING, m_pAttacker, m_pDefender );
			ChanceSkill( DST_CHR_CHANCESTEALHP, m_pAttacker, m_pDefender );
		}
		
		// Active Skill(���� �ߵ�)
		{
			PT_ITEM_SPEC pAttackerHandItemProp = m_pAttacker->GetActiveHandItemProp();
			if( pAttackerHandItemProp )
			{
				DWORD dwActiveSkill	= pAttackerHandItemProp->dwActiveSkill;
				DWORD dwActiveSkillLevel = pAttackerHandItemProp->dwActiveSkillLv;
				if( dwActiveSkillLevel == NULL_ID )
					dwActiveSkillLevel = 1;
				if(  dwActiveSkill != NULL_ID )	// �������� ���⿡ �ߵ������� �ִ�.
				{
					if( pAttackerHandItemProp->dwActiveSkillRate == NULL_ID ||		// = �� 100%
						(DWORD)( random(100) ) < pAttackerHandItemProp->dwActiveSkillRate )	// �ߵ�Ȯ���� �ɷȴ°�.
					{
						g_cSkillSystem->DoActiveSkill( m_pAttacker, dwActiveSkill, dwActiveSkillLevel, m_pDefender, true );		// dwActiveSkill Lv1�� �ߵ���. true�� �ذ�. ���⼭ Ȯ����� �����Ƿ� ���ο����� ��������¶�.
						FLTRACE_LOG( PROGRAM_NAME, _T( "%s�� ActiveSkill �ߵ�, " ), m_pAttacker->GetName() );
					}
				}
			}
		}
	} // AF_GENERIC

	if( m_dwAtkFlags & ( AF_MELEESKILL | AF_MAGICSKILL ) )	// ��ų������ �õ��ɶ�
	{
		SkillProp* pSkillProp;
		AddSkillProp *pAddSkillProp;
		m_pAttacker->GetSkillProp( &pSkillProp, &pAddSkillProp, nSkill, pInfo->GetSkillLevel(), "ProcessAbnormal" );
		DWORD dwActiveSkill = pAddSkillProp->dwActiveSkill;
		if( dwActiveSkill != NULL_ID )		// �߰� �ߵ� ��ų�� �ִ�.
		{
			DWORD dwActiveSkillRate	= pAddSkillProp->dwActiveSkillRate;
			if( m_pAttacker->IsPlayer() && m_pDefender->IsPlayer() )
				dwActiveSkillRate	= pAddSkillProp->dwActiveSkillRatePVP;

			// �ߵ�Ȯ���� �ɷȴ°�.
			if( dwActiveSkillRate == NULL_ID || xRandom(100) < dwActiveSkillRate )
				g_cSkillSystem->DoActiveSkill( m_pAttacker, dwActiveSkill, pAddSkillProp->dwSkillLvl, m_pDefender );	// �ߵ�
		}
	}


	const DWORD dwProb = xRandom( 10000 );

	//////////////////////////////////////////////////////////////////////////
	// ���� �� ��� �ߵ� �Ǵ� ��ų�� �ִ��� üũ
	g_xApplyItemEffect->DoApplyHitActiveSkill( m_pAttacker, m_pDefender, dwProb );
	g_xApplyItemEffect->DoApplyDamageActiveSkill( m_pDefender, m_pAttacker, dwProb );
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// �������� ������ �ߵ� �Ǵ� ��ų�� �ִ��� üũ
	m_pAttacker->DoApplyHitActiveSkill( m_pDefender, dwProb );
	m_pDefender->DoApplyDamageActiveSkill( m_pAttacker, dwProb );
	//////////////////////////////////////////////////////////////////////////
}

// HP ���� ��Ŵ 
int CAttackArbiter::MinusHP( int *pnDamage )
{
	if( m_pAttacker->IsPlayer() == TRUE && m_pDefender->IsPlayer() == TRUE && m_pDefender->GetAdjParam( DST_IGNORE_DMG_PVP ) > 0 )
	{
		*pnDamage = 0;
		return m_pDefender->GetHitPoint();
	}

	int nHP = m_pDefender->GetHitPoint() - *pnDamage;	// HP�� ��´�.
	if( nHP <= 0 )
	{
		nHP				= 0;

		// �ݹ��� üũ
		if( ( m_pAttacker->m_pFSM == NULL )		
			|| ( ( m_pAttacker->m_pFSM != NULL && m_pAttacker->m_pFSM->GetOptionalFlags().bIgnoreMatchless == FALSE ) ) 
			)
		{
			if( m_pDefender->m_dwMode & MATCHLESS2_MODE )	// HP�� 0�̸����� �������µ� �ݹ��� ���³�
				nHP = 1;									// hp�� 1�� ��������
		}

	//	else
	//		nHP = 0;									// �ݹ��� �ƴϸ� �� �״°� �´�.
		*pnDamage = m_pDefender->GetHitPoint() - nHP;
	}
	/*
	else	
	{
		int nActionHPRate = m_pDefender->GetAdjParam( DST_AUTOHP );							// �ߵ� HP��
		if( nActionHPRate > 0 )
		{
			float fHPPercent = (float)nHP / m_pDefender->GetMaxHitPoint();					// ���� HP�� �ۼ�Ʈ ��
			if( fHPPercent <= (nActionHPRate / 100.0f) )									// ���� HP�� �ߵ�HP���ϰ� �Ǹ�
			{
				float fAddHP = m_pDefender->GetChgParam( DST_AUTOHP ) / 100.0f;				// ȸ����ų HP��
				int nAddHP = m_pDefender->GetMaxHitPoint() * fAddHP;						// MaxHP�� ���������� ����.
				nHP += nAddHP;
				m_pDefender->SetPointParam( DST_HP, nHP );									// HP����.
				g_xWSUserManager->AddCreateSfxObj( m_pDefender, XI_INDEX( 62, XI_GEN_CURE01 ) );					// ȸ�� ȿ�� �߻�.
				m_pDefender->RemoveBuff( BUFF_SKILL, SI_ASS_HEAL_PREVENTION );	// ����.				
			}
		}
	}
	*/

	return nHP;
}

// ���� 
void CAttackArbiter::StealHP( int nDamage, ATK_TYPE type )
{
	PT_ITEM_SPEC pHandItem = m_pAttacker->GetActiveHandItemProp();
	if( pHandItem )
	{
		// TODO : ??????????????????????????
		if( pHandItem->dwDestParam[0] == DST_CHR_STEALHP )					// ��� �ִ� ���⿡ ���� �ɼ��� ������
		{
			int nStealHP = (int)( nDamage * (pHandItem->nAdjParamVal[0] / 100.0f) );	// ���������� �������� %�� ����.
			m_pAttacker->SetDestParam( DST_HP, nStealHP, NULL_CHGPARAM );				// �����ڿ��� ����HP�� ��.
		}
	}

	if( type == ATK_GENERIC && m_pAttacker->IsPlayer() )
	{
		int nDstHP = m_pAttacker->GetParam( DST_MELEE_STEALHP, 0 );
		int nStealHP = (int)( nDamage * (nDstHP / 100.0f) );
		if( 0 < nDstHP )
			m_pAttacker->SetDestParam( DST_HP, nStealHP, NULL_CHGPARAM );				// �����ڿ��� ����HP�� ��.
	}
}




// ���ݴ��ϰ� �������� ���� ��� ó�� 
void CAttackArbiter::OnDamaged( int nDamage, DWORD dwFakeDamage )
{
	m_pDefender->m_nAtkCnt = 1;
	m_pDefender->SetDamagedMotion( m_pAttacker, m_dwAtkFlags );

	g_xWSUserManager->AddDamage( m_pDefender, GETID( m_pAttacker ), nDamage, m_dwAtkFlags, dwFakeDamage );
}

// ���ݴ��ϰ� �״� ��� ó�� 
void CAttackArbiter::OnDied()
{
	if( m_pDefender == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, "[ m_pDefender is NULL ]" );
		return;
	}

	if( m_pDefender->IsDie() )
	{
		return;
	}

	if( m_pDefender && m_pAttacker && m_pDefender->m_nDuel != 0 && m_pAttacker->m_nDuel != 0 && 
		m_pDefender->m_idDuelOther == m_pAttacker->GetId() )
	{
		OnDiedPVP();
		return;
	}

	m_pDefender->SubPVP( m_pAttacker, m_nReflect );					// m_pDefender�� �װ� �� �����Ͱ��� ó��.

	m_pAttacker->AddKillRecovery();

	OnDiedSchoolEvent();

	if( m_pDefender->IsNPC() && m_pDefender->IsDie() )	 
	{
		m_pDefender->Delete();							// �ϴ� ����... �̷����� �� ������.
		return;
	}

	if( m_pDefender->m_dwIndex == OBJECT_INDEX( 164, MI_CLOCKWORK1 ) || m_pDefender->m_dwIndex == OBJECT_INDEX( 165, MI_BIGMUSCLE ) || m_pDefender->m_dwIndex == OBJECT_INDEX( 166, MI_KRRR ) )
		m_pDefender->SubAroundExp( m_pAttacker, 50.0f );		// �ݰ�50���� �̳����� �� ����ġ ��������.
	else
		m_pAttacker->SubExperience( m_pDefender );		// m_pDefender�� ���̰� ������ m_pAttacker ����ġ ó��.
	
	switch( _GetContentState( CT_DROP_ITEM_REFORM  ) )
	{
	case CS_VER1:
		{
			m_pDefender->OnDie( *m_pAttacker );
		}
		break;
	default:
		m_pDefender->DropItemByDied( m_pAttacker );				// ���Ϳ��ٸ� ������ ���.
		break;
	}

	BOOL bArena		= m_pDefender->GetWorld() && m_pDefender->GetWorld()->IsArena();
	bArena = g_pEventArenaGlobal->IsArenaChannel() ? TRUE : bArena;

	if( m_pDefender->IsPlayer() && m_pDefender->IsChaotic() && !bArena )
	{
		DWORD dwPropensity = m_pDefender->GetPKPropensity(); // undorflow
		m_pDefender->SetPKPropensity( m_pDefender->GetPKPropensity() - xRandom( prj.m_PKSetting.nDiePropensityMin, prj.m_PKSetting.nDiePropensityMax + 1 ));
		if( dwPropensity <= m_pDefender->GetPKPropensity() )
			m_pDefender->SetPKPropensity( 0 );
		g_xWSUserManager->AddPKPropensity( m_pDefender );
		g_dpDBClient.SendLogPkPvp( m_pDefender, m_pAttacker, 0, 'P' );
	}	

	m_pAttacker->m_nAtkCnt = 0;							// Ÿ���� �׿����� �������� ����ī��Ʈ Ŭ����
	m_pDefender->DoDie( m_pAttacker, m_dwMsg );			// m_pDefender�� �׾��. 

	m_pAttacker->ClearDestObj();						// ��ǥ�� �׿����� �̵���ǥ�� Ŭ����.
}

// PVP���ݴ��ϰ� �״� ��� ó�� 
void CAttackArbiter::OnDiedPVP()
{
	if( m_pDefender->IsNPC() && m_pDefender->IsDie() )	 
	{
		m_pDefender->Delete();							// �ϴ� ����... �̷����� �� ������.
		return;
	}

	m_pDefender->SubDuel( m_pAttacker );//����� ������
	
	m_pDefender->SetPosChanged( TRUE );
	m_pAttacker->SetPosChanged( TRUE );

	m_pAttacker->ClearDuel();
	( (FLWSUser*)m_pAttacker )->AddSetDuel( m_pAttacker );
	m_pDefender->ClearDuel();
	( (FLWSUser*)m_pDefender )->AddSetDuel( m_pDefender );

	m_pDefender->AddKillRecovery();
	m_pAttacker->AddKillRecovery();

	m_pAttacker->m_nAtkCnt = 0;							//  �������� ����ī��Ʈ Ŭ����
	m_pDefender->m_nAtkCnt = 0;							//  ������� ����ī��Ʈ Ŭ����

	m_pDefender->DoPVPEnd( m_pAttacker, false ,m_dwMsg );			//

	( (FLWSUser*)m_pAttacker )->AddDuelStart( m_pDefender->m_idPlayer , 3 );	// ���� ���濡 ���� ���̵� �����ָ� �ȴ�. 3:�̱�, 4 : ��
	( (FLWSUser*)m_pDefender )->AddDuelStart( m_pAttacker->m_idPlayer , 4 );


	m_pAttacker->ClearDestObj();						// �̵���ǥ�� Ŭ����.
	m_pDefender->ClearDestObj();						// �̵���ǥ�� Ŭ����.
}


// �����̺�Ʈ ó�� 
void CAttackArbiter::OnDiedSchoolEvent()
{
	if( g_eLocal.GetState( EVE_SCHOOL_BATTLE ) == SBS_START2 )	// '/sbstart2'������ �ǰ�
	{
		if( m_pDefender->m_dwAuthorization < AUTH_GAMEMASTER )
		{
			if( !m_pDefender->IsMode( OBSERVE_MODE ) )
			{
				CGuild* pGuild	= m_pDefender->GetGuild();
				if( pGuild )
					pGuild->m_nDead++;	
			}
			m_pDefender->SetMode( OBSERVE_MODE );
			g_xWSUserManager->AddModifyMode( (FLWSUser*)m_pDefender );
		}

		static EVENTITEM eventitem[3] =
		{
			ITEM_INDEX( 2805, II_GEN_FOO_INS_HOTDOG ),		12,
			ITEM_INDEX( 2539, II_GEN_REF_REF_TENTH ),		7,
			ITEM_INDEX( 2552, II_GEN_POT_DRI_VITAL900 ),	7,
		};

		for( int i = 0; i < 3; i++ )
		{
			FLItemElem* pItemElem		= new FLItemElem;
			pItemElem->m_dwItemId		= eventitem[i].dwIndex;
			pItemElem->m_nItemNum		= (int)( eventitem[i].dwPrabability );
			pItemElem->m_nHitPoint		= ( pItemElem->GetProp()->dwEndurance == -1 ) ? 0 : pItemElem->GetProp()->dwEndurance;//pItemElem->GetProp()->dwEndurance;
			pItemElem->SetSerialNumber();

			CItem* pItem		= new CItem;
			pItem->m_pItemBase	= pItemElem;
			pItem->SetIndex( D3DDEVICE, pItemElem->m_dwItemId );
			pItem->SetPos( m_pDefender->GetPos() );
			pItem->SetAngle( (float)( xRandom( 360 ) ) );
			if( m_pDefender->GetWorld()->ADDOBJ( pItem, TRUE, m_pDefender->GetLayer() ) == FALSE )
			{
				SAFE_DELETE( pItem );
			}
		}
	}
}

BYTE CAttackArbiter::GetHandFlag()
{
	BYTE cbHandFlag = 0x01;				// ����Ʈ�� �������̴� .

	if( m_dwAtkFlags & AF_RANGE )		// Ȱ�� ��� ������ ������ 
		return cbHandFlag;

	if( m_dwAtkFlags & AF_GENERIC )		// ��Ÿ�϶�
	{
		cbHandFlag = static_cast< BYTE >( m_nParam & 0x03 );	// ���� 2��Ʈ�� ������ ���������� ģ���� �޼�����ģ���� ���´�.
		if( cbHandFlag == 0 )	
			cbHandFlag = 0x01;			// ���� ������ ����Ʈ�� ������
	}

	return cbHandFlag;
}

float		CAttackArbiter::GetAdjustDamageFactor( ATTACK_INFO & kAttackInfo ) const
{
	float fFactor = 1.0;

	fFactor += GetFactor_AttackType( kAttackInfo );

	fFactor	+= GetFactor_AbnormalState( kAttackInfo );

	fFactor += GetFactor_MonsterAttribute( kAttackInfo );

	return fFactor > 0.0f ? fFactor : 0.0f;
}

float		CAttackArbiter::GetFactor_AttackType( ATTACK_INFO & kAttackInfo ) const
{
	float fFactor = 0.0f;

	if( kAttackInfo.pAttacker == NULL || kAttackInfo.pDefender == NULL )
	{
		return fFactor;
	}

	if( kAttackInfo.pDefender->IsPlayer() == FALSE )
	{
		return fFactor;
	}

	int nParamID	= DST_NONE;

	if( kAttackInfo.GetAtkType() == ATK_MAGICSKILL )
	{
		nParamID	= m_pAttacker->IsPlayer() == TRUE ? DST_TAKE_PVP_DMG_MAGIC_RATE : DST_TAKE_PVE_DMG_MAGIC_RATE;
	}
	else
	{
		nParamID	= m_pAttacker->IsPlayer() == TRUE ? DST_TAKE_PVP_DMG_PHYSICAL_RATE : DST_TAKE_PVE_DMG_PHYSICAL_RATE;
	}

	const int nParam	= kAttackInfo.pDefender->GetParam( nParamID, 0 );

	fFactor				+= static_cast<float>( nParam ) / 100.f;

	return fFactor;
}

float		CAttackArbiter::GetFactor_AbnormalState( ATTACK_INFO & kAttackInfo ) const
{
	float fFactor	= 0.0f;

	if( kAttackInfo.pAttacker == NULL || kAttackInfo.pDefender == NULL )
	{
		return fFactor;
	}

	if( kAttackInfo.pDefender->IsAbnormalState( CHS_STUN ) == true )
	{
		const int nParam	 = kAttackInfo.pAttacker->GetParam( DST_GIVE_DMG_RATE_ENEMY_STUN );
		fFactor				+= static_cast<float>( nParam ) / 100.0f;
	}

	if( kAttackInfo.pDefender->IsAbnormalState( CHS_DARK ) == true )
	{
		const int nParam	= kAttackInfo.pAttacker->GetParam( DST_GIVE_DMG_RATE_ENEMY_DARK );
		fFactor				+= static_cast<float>( nParam ) / 100.0f;
	}

	if( kAttackInfo.pDefender->IsAbnormalState( CHS_POISON ) == true )
	{
		const int nParam	= kAttackInfo.pAttacker->GetParam( DST_GIVE_DMG_RATE_ENEMY_POISON );
		fFactor				+= static_cast<float>( nParam ) / 100.0f;
	}

	if( kAttackInfo.pDefender->IsAbnormalState( CHS_SLOW ) == true )
	{
		const int nParam	= kAttackInfo.pAttacker->GetParam( DST_GIVE_DMG_RATE_ENEMY_SLOW );
		fFactor				+= static_cast<float>( nParam ) / 100.0f;
	}

	if( kAttackInfo.pDefender->IsAbnormalState( CHS_BLEEDING ) == true )
	{
		const int nParam	= kAttackInfo.pAttacker->GetParam( DST_GIVE_DMG_RATE_ENEMY_BLEEDING );
		fFactor				+= static_cast<float>( nParam ) / 100.0f;
	}

	if( kAttackInfo.pDefender->IsAbnormalState( CHS_SILENT ) == true )
	{
		const int nParam	= kAttackInfo.pAttacker->GetParam( DST_GIVE_DMG_RATE_ENEMY_SILENT );
		fFactor				+= static_cast<float>( nParam ) / 100.0f;
	}

	if( kAttackInfo.pDefender->IsAbnormalState( CHS_LOOT ) == true )
	{
		const int nParam	 = kAttackInfo.pAttacker->GetParam( DST_GIVE_DMG_RATE_ENEMY_LOOT );
		fFactor				+= static_cast<float>( nParam ) / 100.0f;
	}

	if( kAttackInfo.pDefender->IsAbnormalState( CHS_SETSTONE ) == true )
	{
		const int nParam	= kAttackInfo.pAttacker->GetParam( DST_GIVE_DMG_RATE_ENEMY_SETSTONE );
		fFactor				+= static_cast<float>( nParam ) / 100.0f;
	}

	if( kAttackInfo.pDefender->IsAbnormalState( CHS_SLEEPING ) == true )
	{
		const int nParam	= kAttackInfo.pAttacker->GetParam( DST_GIVE_DMG_RATE_ENEMY_SLEEPING );
		fFactor				+= static_cast<float>( nParam ) / 100.0f;
	}

	return fFactor;
}

float		CAttackArbiter::GetFactor_MonsterAttribute( ATTACK_INFO & kAttackInfo ) const
{
	float fFactor	= 0.0f;

	if( kAttackInfo.pAttacker == NULL || kAttackInfo.pDefender == NULL )
	{
		return fFactor;
	}

	if( kAttackInfo.pAttacker->IsPlayer() == FALSE || kAttackInfo.pDefender->IsPlayer() == TRUE )
	{
		return fFactor;
	}

	MoverProp* pMoverProp	= prj.GetMoverProp( kAttackInfo.pDefender->m_dwIndex );
	if( pMoverProp == NULL )
	{
		return fFactor;
	}

	int nParamID	= DST_NONE;

	switch( pMoverProp->eElementType )
	{
	case SAI79::FIRE:
		nParamID	= DST_GIVE_PVE_DMG_ELEMENT_FIRE_RATE;
		break;

	case SAI79::WATER:
		nParamID	= DST_GIVE_PVE_DMG_ELEMENT_WATER_RATE;
		break;

	case SAI79::ELECTRICITY:
		nParamID	= DST_GIVE_PVE_DMG_ELEMENT_ELECT_RATE;
		break;

	case SAI79::WIND:
		nParamID	= DST_GIVE_PVE_DMG_ELEMENT_WIND_RATE;
		break;

	case SAI79::EARTH:
		nParamID	= DST_GIVE_PVE_DMG_ELEMENT_EARTH_RATE;
		break;
	}

	const int nParam	= kAttackInfo.pAttacker->GetParam( nParamID, 0 );

	fFactor				+= static_cast<float>( nParam ) / 100.0f;

	return fFactor;
}
