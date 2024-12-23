
#include "stdafx.h"
#include "FLSkillSystem.h"

//////////////////////////////////////////////////////////////////////////
#include "Mover.h"
#include "RainbowRace.h"
#include "eveschool.h"

#ifdef	__WORLDSERVER
#include "party.h"
#include "worldmng.h"
#include "CtrlObjs.h"
#include "../worldserver/FLMapGrid.h"
#include "../worldserver/LinkMap.h"
#endif

#ifdef	__CLIENT
#include <defineText.h>
#include <ResData.h>
#include "../Neuz/DPClient.h"
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
extern	CGuildCombat	g_GuildCombatMng;

#ifdef	__WORLDSERVER
extern	CPartyMng		g_PartyMng;
extern	CWorldMng		g_WorldMng;
#endif

#ifdef	__CLIENT
extern	CDPClient		g_DPlay;
#endif
//////////////////////////////////////////////////////////////////////////


FLSkillSystem*	FLSkillSystem::GetInstance()
{
	static FLSkillSystem cSkillSystem;
	return &cSkillSystem;
}

FLSkillSystem::FLSkillSystem()
{
}

FLSkillSystem::~FLSkillSystem()
{
}

bool	FLSkillSystem::CanUseSkill( CMover* pUseMover, const DWORD dwSkillID, const DWORD dwSkillLevel, const OBJID idFocusObj, const SKILLUSETYPE sutType )
{
	//////////////////////////////////////////////////////////////////////////
	// ���� ã��
	SkillProp *pSkillProp = NULL;
	AddSkillProp *pAddSkillProp = NULL;
	if( pUseMover->GetSkillProp( &pSkillProp, &pAddSkillProp, dwSkillID, dwSkillLevel, "DoUseSkill" ) == FALSE )
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////

	// imParkth - Ÿ�� ��� ��ų�ε� �ڱ� �ڽ��� ����� �Ǹ� ����
	if( NULL == pSkillProp || EXT_ANOTHER == pSkillProp->dwExeTarget && pUseMover->GetId() == idFocusObj )
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���� Ȯ��
	if( pUseMover->IsDie() || pUseMover->m_pActMover->IsFly() )
	{
		return false;
	}

	// ���ݱ��� ���¿��� ��ų ����.
	if( pUseMover->m_dwFlag & MVRF_NOATTACK )
	{
#ifdef __WORLDSERVER	
		((FLWSUser*) pUseMover)->AddText( prj.GetText( TID_GAME_STILLNOTUSE ) );	
#endif
		return false;
	}
	if( pUseMover->IsMode( NO_ATTACK_MODE ) )
	{
		return false;
	}

	// �׼ǽ������� ����� ��ų�϶� �׼ǽ��� ��ϰ��ɿ��� üũ
	if( _GetContentState( CT_NEWUI_19 ) != CS_VER1 && ( sutType == SUT_QUEUESTART || sutType == SUT_QUEUEING ) )
	{
		if( pSkillProp->bCanUseActionSlot == false )
		{
			return false;
		}
	}

#ifdef __WORLDSERVER
	// ����ũ�� �ɷ��ִ� ���¿��� ��ų��� ����.
	if( pUseMover->HasBuff( BUFF_SKILL, SKILL_INDEX( 143, SI_BLD_SUP_BERSERK ) ) )
	{
		return false;
	}

	// ���κ��� ���̽�
	if( pSkillProp->dwID == SKILL_INDEX( 244, SI_RIG_HERO_RETURN ) && pUseMover->IsPlayer() &&
		CRainbowRaceMng::GetInstance()->IsEntry( pUseMover->m_idPlayer ) )
	{
		((FLWSUser*)pUseMover)->AddDefinedText( TID_GAME_RAINBOWRACE_NOTELEPORT );
		return false;
	}
#endif
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// ���(World) Ȯ��
	CWorld* pWorld	= pUseMover->GetWorld();
	if( pWorld == NULL )
	{
		return false;
	}

	DWORD dwWorldID = pWorld->GetID();
	if( dwWorldID == WI_WORLD_QUIZ )
	{
		return false;
	}
	// ����Ĺ��߿��� ��Ȱ��ų ����
	if( dwWorldID == WI_WORLD_GUILDWAR )
	{
		// ���½ý���	Neuz, World, Trans
		if( pSkillProp->dwID == SKILL_INDEX( 107, SI_MAG_MAG_BLINKPOOL ) || pSkillProp->dwID == SKILL_INDEX( 244, SI_RIG_HERO_RETURN ) )
		{
			return false;
		}

		if( g_GuildCombatMng.m_nState != CGuildCombat::WAR_STATE )
		{
			if( pSkillProp->dwID == SKILL_INDEX( 45, SI_ASS_HEAL_RESURRECTION ) )
			{
				return false;
			}
		}
	}
#ifdef __WORLDSERVER	
	// �������ʿ��� ������ ���
	if( dwWorldID == WI_WORLD_GUILDWAR )
	{
		// ���߼������� ��ų�� ����Ҽ� �����ϴ�
		if( pUseMover->m_nGuildCombatState == 0 )
		{
			if( pUseMover->IsPlayer() )
			{
				((FLWSUser*) pUseMover)->AddText( prj.GetText( TID_GAME_GUILDCOMBAT_STANDS_NOTUSESKILL ) );
			}

			return false;
		}
	}
#endif //__WORLDSERVER

	// �ϴ��� ������ �� ��ũǮ ��� ���� // ���������� �� üũ ������?
	if( WI_WORLD_GUILDWAR1TO1_0 <= dwWorldID && dwWorldID <= WI_WORLD_GUILDWAR1TO1_L )
	{
		if( pSkillProp->dwID == SKILL_INDEX( 107, SI_MAG_MAG_BLINKPOOL ) )
		{
#ifdef __WORLDSERVER
			((FLWSUser*) pUseMover)->AddText( prj.GetText( TID_GAME_NEVERKILLSTOP ) );
#else
			g_WndMng.PutString( prj.GetText( TID_GAME_NEVERKILLSTOP ), NULL, prj.GetTextColor( TID_GAME_NEVERKILLSTOP ) );
#endif // __WORLDSERVER
			
			return false;
		}
	}
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// Ÿ�� Ȯ��
	CMover* pTarget = prj.GetMover( idFocusObj );	// Ÿ���� �����͸� ��.
	if( IsInvalidObj( pTarget ) )
	{
		return false;
	}

	if( pSkillProp->dwID == SKILL_INDEX( 44, SI_ASS_HEAL_HEALING ) )	// ���� ��밡 �׾����� ���
	{
		if( pTarget->IsDie() )
		{
			return false;
		}
	}

	if( pSkillProp->dwID == SKILL_INDEX( 412, SI_WIN_YOYO_BACKSTEP ) )
	{
		if( pTarget->GetWorld() == pWorld )
		{
			D3DXVECTOR3 vStart = pUseMover->GetPos();
			vStart.y += 1.0f;
			D3DXVECTOR3 vEnd   = pTarget->GetPos();
			if( pWorld->IntersectObjLine( NULL, vStart, vEnd, FALSE, FALSE ) == TRUE )
			{
#ifdef __CLIENT
				g_WndMng.PutString( prj.GetText( TID_GAME_BLOCKTARGETING ), NULL, prj.GetTextColor( TID_GAME_BLOCKTARGETING ) );
#endif // __CLIENT
				return false;
			}
		}
		else
		{
			return false;
		}
	}

#ifdef __WORLDSERVER
	if( pSkillProp->IsDebuff() == TRUE )	//PK ������������ ���� ������ ��ų�� PVP ��뿡�Ը� ����ϵ���..
	{
		int nAttackerPK = 0, nDefenderPK = 0;
		if( pUseMover->IsPlayer() && pTarget->IsPlayer() && (pUseMover != pTarget) ) //(this!=pTarget) - Ÿ���� ����� ��ų�� Ÿ���� �������̴�.
		{
			DWORD dwRegionAttr	= pUseMover->GetPKPVPRegionAttr();
			nAttackerPK = pUseMover->IsPKPVPInspectionBase( dwRegionAttr, FALSE );
			nDefenderPK = pTarget->IsPKPVPInspectionBase( dwRegionAttr, FALSE );
			if( (nAttackerPK == 1 || nDefenderPK == 1) &&
				!(	pUseMover->IsPVPTarget( pTarget )
				|| pUseMover->IsWarTarget( pTarget )
				|| pUseMover->IsGuildCombatTarget( pTarget ) 
				|| pUseMover->IsArenaTarget( pTarget )
				|| pUseMover->IsEventArenaTarget( pTarget )
				) )
			{
				((FLWSUser*)pUseMover)->AddDefinedText( TID_GMAE_PK_NOT_AREA, "" );
				return false;
			}
		}
	}

	// ��... 
	if( ( pSkillProp->dwID == SKILL_INDEX( 114, SI_ASS_CHEER_QUICKSTEP )	&& pTarget->HasBuff( BUFF_SKILL, SKILL_INDEX( 317, SI_GEN_EVE_QUICKSTEP ) ) ) ||
		( pSkillProp->dwID == SKILL_INDEX( 20, SI_ASS_CHEER_HASTE )			&& pTarget->HasBuff( BUFF_SKILL, SKILL_INDEX( 318, SI_GEN_EVE_HASTE ) ) ) ||
		( pSkillProp->dwID == SKILL_INDEX( 49, SI_ASS_CHEER_HEAPUP )		&& pTarget->HasBuff( BUFF_SKILL, SKILL_INDEX( 319, SI_GEN_EVE_HEAPUP ) ) ) ||
		( pSkillProp->dwID == SKILL_INDEX( 116, SI_ASS_CHEER_ACCURACY )		&& pTarget->HasBuff( BUFF_SKILL, SKILL_INDEX( 320, SI_GEN_EVE_ACCURACY ) ) ) )
	{
		if( pTarget->IsPlayer() )
		{
			((FLWSUser*)pTarget)->AddDefinedText( TID_GAME_NPCBUFF_FAILED, "\"%s\"", pSkillProp->szName );
		}

		return false;
	}
	// ��Ż, ȥ���� �ϰݽ�ų�̸� �����Ϸ����������� �˻��Ѵ�.
	if( pSkillProp->dwID == SKILL_INDEX( 203, SI_ACR_YOYO_SNITCH ) || pSkillProp->dwID == SKILL_INDEX( 214, SI_JST_YOYO_VATALSTAB ) )
	{
		// ��ũ�Ϸ��� ���°� �ƴϸ� ������
		if( !pUseMover->HasBuff( BUFF_SKILL, SKILL_INDEX( 193, SI_ACR_SUP_DARKILLUSION ) ) )
		{
			if( pUseMover->IsPlayer() )
			{
				((FLWSUser*)pUseMover)->AddDefinedText( TID_GAME_REQ_DARK, "" );		// ��ũ�Ϸ������¿��� �����մϴ�.
			}

			return false;
		}
	}

	// ��Ÿ�� �˻�.
	if( _GetContentState( CT_NEWUI_19 ) == CS_VER1
		|| ( _GetContentState( CT_NEWUI_19 ) != CS_VER1 && sutType == SUT_NORMAL ) )
	{
		int nSkillIdx = pUseMover->GetSkillIdx( dwSkillID );		// ��ų����Ʈ �ε����� ã��.
		if( nSkillIdx >= 0 )
		{
			if( pUseMover->GetReuseDelay( nSkillIdx ) )		// ������ �ϴ� ��ų ��Ÿ���� ���������� ����ȵ�.
			{
				if( pUseMover->IsPlayer() )
				{
					((FLWSUser*)pUseMover)->AddDefinedText( TID_GAME_SKILLWAITTIME, "" );	// ���� ����� �� �����ϴ�.
				}
				return false;
			}
		}
	}

	if( pUseMover->CheckAttackDistance( pTarget, pSkillProp->dwAttackRange ) == false )
	{
		return false;
	}
#endif //__WORLDSERVER

	return true;
}

bool	FLSkillSystem::IsRemainSkillDelayTime( CMover* pUseMover, const DWORD dwSkillID ) const
{
	// ��Ÿ�� �˻�.
#ifdef __CLIENT
	/*
	Ŭ�󿡼��� �ٸ������� ����� ��ų�� ���ؼ��� �ش� ������ �״�� ź��.
	�ٸ� ������ ��ų ������ ������ ���� �����Ƿ� ��ų�ε����� �������� ���Ѵ�.
	��� üũ �Ұ���.. �ٸ� �����϶� �׳� ��Ÿ�� ���ٰ� ����..
	*/
	if( pUseMover->IsActiveMover() == FALSE )
	{
		return false;
	}
#endif // __CLIENT

	const int nSkillIdx = pUseMover->GetSkillIdx( dwSkillID );		// ��ų����Ʈ �ε����� ã��.
	if( nSkillIdx >= 0 && nSkillIdx <= MAX_SKILL_JOB )
	{
		if( pUseMover->GetReuseDelay( nSkillIdx ) <= 0 )		// ��Ÿ���� ���ų� �������Ƿ� ��� ����..
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		/************************************************************************/
		/*
		��ų �������� ��ų ���̺� �ִ� ��ų�� �ƴ� �ϵ��ڵ��� ��ų���� �����ִ�.( ex. ��ũ�ι��� ���� )
		�ش� ��ų���� ������ �⽺ų �ε����� �̾ƿ� �� �����Ƿ� �׳� �����̰� ���ٰ� �Ǵ��Ѵ�.
		*/
		/************************************************************************/

		return false;
	}
}

// dwSkill,nLevel�� ������ ��𼭵��� ��밡���� ����	
// ���ͱ��� ��밡��
DWORD	FLSkillSystem::DoUseSkill( CMover* pUseMover, const DWORD dwSkill, const int nLevel, const OBJID idFocusObj, const SKILLUSETYPE sutType, const BOOL bControl, const int nCastingTime )
{
	if( CanUseSkill( pUseMover, dwSkill, nLevel, idFocusObj, sutType ) == false )
	{
		return FSC_SKILLUSE_FAIL;
	}

	if( IsRemainSkillDelayTime( pUseMover, dwSkill ) == true )
	{
		return FSC_SKILLUSE_SKIP;
	}

	SkillProp *pSkillProp = NULL;
	AddSkillProp *pAddSkillProp = NULL;
	if( pUseMover->GetSkillProp( &pSkillProp, &pAddSkillProp, dwSkill, nLevel, "DoUseSkill" ) == FALSE )
	{
		return FSC_SKILLUSE_FAIL;
	}

	CMover* pTarget = prj.GetMover( idFocusObj );	// Ÿ���� �����͸� ��.
	if( IsValidObj( pTarget ) == FALSE )
	{
		return FSC_SKILLUSE_FAIL;
	}

	CWorld* pWorld	= pUseMover->GetWorld();
	if( pWorld == NULL )
	{
		return FSC_SKILLUSE_FAIL;
	}

	if( pTarget != pUseMover )	// Ÿ�ο��� ���� �����϶��� �Ʒ��˻縦 ��.
	{
#ifdef __WORLDSERVER
		// Ÿ�ο��� ���� ��ų�� ��ý����� ����?
		// (��û�� : Ÿ������ �����Ǿ� �ִ� ��󿡰� ����Ѵ�. Ÿ���ð� ������� �ڱ��ڽſ��� ���� ���.)
		// ������ �ΰ��� ����� �´°ų�.
		// �ڽſ��� ����ϴ� ��ų �ɷ�����.
		if( pSkillProp->dwUseChance == WUI_NOW )	// ˬ - ���ҽ� ��ŷ
			return FSC_SKILLUSE_FAIL;
		if( pSkillProp->dwExeTarget == EXT_SELFCHGPARAMET )
			return FSC_SKILLUSE_FAIL;

		if( pTarget->IsNPC() && pTarget->IsPeaceful() )
		{
			return FSC_SKILLUSE_FAIL;	// NPC���� ģ��?NPC���� ��ų ��� ����
		}

		if( !g_eLocal.GetState( EVE_STEAL ) )
		{
			if( pSkillProp->IsDebuff() == TRUE )	// ���۸����� �����Ҷ�
			{
				if( pUseMover->IsSteal( pTarget ) )		// pTarget�� ��ƿ �Ϸ��ϸ�
				{
					return FSC_SKILLUSE_FAIL;
				}
			}
		}
		if( pWorld->IsArena() == FALSE )		// �Ʒ����� �ƴϸ�,
		{
			if( pSkillProp->IsDebuff() == FALSE )	// ���� ������
			{
				HITTYPE hy = HITTYPE_FAIL;
				if( (hy = pUseMover->GetHitType2( pTarget, TRUE, TRUE )) != HITTYPE_FAIL )  	// �������� ��󿡰� �����Ҷ�
				{
					if( pUseMover->IsPlayer() )
					{
						((FLWSUser*)pUseMover)->AddDefinedText( TID_GAME_NEVERKILLSTOP, "" );		// ��ų�� ����� �� �����ϴ�.
					}

					return FSC_SKILLUSE_FAIL;		// ��ų ���
				}
			}
		}
		
#endif
		pUseMover->SetAngle( GetDegree( pTarget->GetPos(), pUseMover->GetPos() ) );		// ��ǥ������ ���� ����.
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// ������ �˻�
#ifdef __WORLDSERVER

	// ��ų�� ����Ҷ� ������ ���⸦ ��� �ִ°� �˻�
	PT_ITEM_SPEC pItemProp = NULL;
	DWORD	dwItemKind3 = 0;

	if( pSkillProp->dwNeedItemKind3 == IK3_SHIELD )	// �ʿ�ƾ������� �����϶�
	{
		FLItemElem *pItemElem = pUseMover->GetEquipItem( PARTS_SHIELD );
		if( pItemElem )
		{
			pItemProp = pItemElem->GetProp();
			dwItemKind3 = pItemProp->dwItemKind3;
		}
		else
		{
			dwItemKind3 = NULL_ID;
		}
	}
	else if( pSkillProp->dwNeedItemKind3 == IK3_ZEMBARUNA || pSkillProp->dwNeedItemKind3 == IK3_MAGICBARUNA || pSkillProp->dwNeedItemKind3 == IK3_SHILDBARUNA )
	{
		FLItemElem *pItemElem = pUseMover->GetEquipItem( PARTS_SHIELD );
		if( pItemElem )
		{
			pItemProp = pItemElem->GetProp();
			dwItemKind3 = pItemProp->dwItemKind3;
		}
		else
		{
			dwItemKind3 = NULL_ID;
		}
	}
	else	// �ʿ�������� ���а� �ƴҶ��� ��Ƽ�� ��������...
	{
		pItemProp = pUseMover->GetActiveHandItemProp();
		dwItemKind3 = pItemProp->dwItemKind3;
	}
	
	switch( pSkillProp->dwNeedItemKind3 )	// ��ų�� �ʿ���ϴ� ����.
	{
	case IK3_ENCHANTWEAPON:
		{
			if( pUseMover->IsPlayer() )
			{
				bool bSuccess = true;

				FLItemElem *pItemElemR = pUseMover->GetWeaponItem();
				FLItemElem *pItemElemL = pUseMover->GetLWeaponItem();

				if( pItemElemR && pItemElemL )	// ������ ������.
				{
					// ��մ� �Ӽ��� ������
					if( (pItemElemR && pItemElemR->m_byItemResist == SAI79::NO_PROP) &&
						(pItemElemL && pItemElemL->m_byItemResist == SAI79::NO_PROP) )
						bSuccess = false;		// ������
				} 
				else if( pItemElemR && pItemElemL == NULL )	// �����ո� ��� ������.
				{
					if( pItemElemR && pItemElemR->m_byItemResist == SAI79::NO_PROP )		// �����տ� �Ӽ��� ������
						bSuccess = false;		// ������
				} 
				else if( pItemElemR == NULL && pItemElemL == NULL )	// ��մ� ������
				{
					bSuccess = false;		// �̶��� ������ ������
				}

				if( bSuccess == false )
				{
					( (FLWSUser*)pUseMover )->AddDefinedText( TID_SKILL_NOTELEWEAPON, "" );	// �Ӽ����Ⱑ ���� ������.
					return FSC_SKILLUSE_FAIL;
				}
			}
		}
		break;
	case IK3_MAGICBOTH:
		{
			if( dwItemKind3 != IK3_WAND && dwItemKind3 != IK3_STAFF )	// �տ��� ���Ⱑ �ϵ嵵 �������� �ƴϸ�
			{
				if( pUseMover->IsPlayer() )
				{
					( (FLWSUser*)pUseMover )->AddDefinedText( TID_GAME_WRONGITEM, "" );
				}
				return FSC_SKILLUSE_FAIL;
			}
		}
		break;
	case IK3_YOBO:
		{
			if( dwItemKind3 != IK3_YOYO && dwItemKind3 != IK3_BOW )	
			{
				if( pUseMover->IsPlayer() )
				{
					( (FLWSUser*)pUseMover )->AddDefinedText( TID_GAME_WRONGITEM, "" );
				}
				return FSC_SKILLUSE_FAIL;
			}
		}
		break;
	default:
		{
			if( pSkillProp->dwNeedItemKind3 != NULL_ID && pSkillProp->dwNeedItemKind3 != dwItemKind3 )	// �׿ܴ� IK3�� ���ؼ� Ʋ���� ����.
			{
				if( pUseMover->IsPlayer() )
				{
					( (FLWSUser*)pUseMover )->AddDefinedText( TID_GAME_WRONGITEM, "" );
				}
				return FSC_SKILLUSE_FAIL;
			}
		}
		break;
	}

	// sun 8�� ����â ���� ����, Ÿ���� ������ �̹� �ɸ� ������ ������ ���� ��� ���� ���ϰ� ��.
	if( pSkillProp->dwSkillType == BUFF_SKILL )
	{
		IBuff* pBuff	= pTarget->m_buffs.GetBuff( BUFF_SKILL, (WORD)( pSkillProp->dwID ) );
		if( pBuff )
		{
			if( pBuff->GetLevel() > (DWORD)( nLevel ) )
			{
				if( pUseMover->IsPlayer() )
				{
					( (FLWSUser*)pUseMover )->AddDefinedText( TID_GAME_DONOTUSEBUFF, "" );
				}
				return FSC_SKILLUSE_FAIL;
			}
		}
	}

	// ���ڵ���̳� ���ڵ���̳��� ���ǰ˻�.
	if( pSkillProp->dwNeedItemHanded != NULL_ID )
	{
		// pItemProp�� NULL�̸�?
		if( pItemProp )
		{
			if( pUseMover->IsPlayer() )
			{
				if( pSkillProp->dwNeedItemHanded == HD_DUAL )		// ���� ��ų�ε�
				{
					if( pUseMover->IsDualWeapon() == FALSE )			// ���� ������ �ִ�.
					{
						((FLWSUser*)pUseMover)->AddDefinedText( TID_GAME_WRONGITEM, "" );
						return FSC_SKILLUSE_FAIL;
					}
				}
				else if( pItemProp->dwHanded != pSkillProp->dwNeedItemHanded )	// ��ų�� ��� �ִ� �������� Handed ������ �ٸ��� ����
				{
					((FLWSUser*)pUseMover)->AddDefinedText( TID_GAME_WRONGITEM, "" );
					return FSC_SKILLUSE_FAIL;
				}
			}
		}
	}

	// �ʿ��Ѿ������� �����Ǿ� �ִٸ�
	if( pUseMover->IsBullet( pSkillProp ) == FALSE )
	{
		return FSC_SKILLUSE_FAIL;
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// ��ų �˻�?
	if( pUseMover->IsPlayer() )
	{
//sun: 10�� ���½ý���	Neuz, World, Trans

		//////////////////////////////////////////////////////////////////////////
		// mirchang_100908 �ش� ��ų�� ��쿡�� �ش����� ���� üũ...
		if( pSkillProp->dwExeTarget == EXT_TROUPE )
		{
			CParty* pParty = g_PartyMng.GetParty( pUseMover->m_idparty );
			if( pParty == NULL )
			{
				((FLWSUser*)pUseMover)->AddDefinedText( TID_GAME_NEVERKILLSTOP );
				return FSC_SKILLUSE_FAIL;
			}
		}
		//////////////////////////////////////////////////////////////////////////
	}

	if( pSkillProp->dwID == SKILL_INDEX( 159, SI_BIL_PST_ASALRAALAIKUM ) )	// �ѻ���� ��� ����ó��.
	{
		if( pUseMover->IsPlayer() && pUseMover->GetFatiguePoint() < pUseMover->GetReqFp( pAddSkillProp->nReqFp )  )	// chipi_090917 �ʿ� FP �˻�
		{
			((FLWSUser*)pUseMover)->AddDefinedText( TID_GAME_REQFP ); // chipi_00917
			return FSC_SKILLUSE_FAIL;
		}
	}

	
#endif // __WORLDSERVER
	//////////////////////////////////////////////////////////////////////////

	OBJMSG dwMsg = OBJMSG_NONE;
	int nMotion = (int)pSkillProp->dwUseMotion;

	//////////////////////////////////////////////////////////////////////////
	//--- ������ ������� �˻� �ϰ� �޽��� Ÿ��(?) �����ϰ� ��� ����
	if( pSkillProp->dwSkillType == KT_SKILL )
	{
#ifdef __WORLDSERVER
			// ���½ý���	Neuz, World, Trans
			if( pUseMover->GetReqFp( pAddSkillProp->nReqFp ) == 9999999 )
			{
				if( pUseMover->GetManaPoint() != pUseMover->GetMaxManaPoint() )
				{
					if( TRUE == pUseMover->IsPlayer() )
					{
						( (FLWSUser*)pUseMover )->AddDefinedText( TID_GAME_REQMP, "" );
					}
					return FSC_SKILLUSE_FAIL;
				}
				if( pUseMover->GetFatiguePoint() != pUseMover->GetMaxFatiguePoint() )
				{
					if( TRUE == pUseMover->IsPlayer() )
					{
						( (FLWSUser*)pUseMover )->AddDefinedText( TID_GAME_REQFP, "" );
					}
					return FSC_SKILLUSE_FAIL;
				}
			}
			else
			{
				if( pUseMover->GetFatiguePoint() - pUseMover->GetReqFp( pAddSkillProp->nReqFp ) < 0 )	// FP�� ���ڸ��Ƿ� ���
				{
					if( TRUE == pUseMover->IsPlayer() )
					{
						( (FLWSUser*)pUseMover )->AddDefinedText( TID_GAME_REQFP, "" );
					}
					return FSC_SKILLUSE_FAIL;
				}

				// KT_SKILL�߿� MP�� �Ҹ�Ǵ� ���������� FP�� ���� �Ҹ�����ش�.
				if( (int)pAddSkillProp->nReqMp > 0 )
				{
					if( pUseMover->GetManaPoint() - pUseMover->GetReqMp( pAddSkillProp->nReqMp ) < 0 )		// ������ ���ڸ��Ƿ� ���
					{
						if( TRUE == pUseMover->IsPlayer() )
						{
							( (FLWSUser*)pUseMover )->AddDefinedText( TID_GAME_REQMP, "" );
						}
						return FSC_SKILLUSE_FAIL;
					}
				}
			}

#endif
		dwMsg = OBJMSG_MELEESKILL;	// Ŭ�󿡼��� �����ؾ���
	} 
	else if( pSkillProp->dwSkillType == KT_MAGIC )
	{
#ifdef __WORLDSERVER
		if( pUseMover->GetAdjParam( DST_CHRSTATE ) & CHS_SILENT )		// ħ�����¿��� ������ų �� �� ����.
		{
			((FLWSUser*) pUseMover)->AddText( prj.GetText( TID_GAME_NEVERKILLSTOP ) );	
			return FSC_SKILLUSE_FAIL;
		}

		//sun: 10�� ���½ý���	Neuz, World, Trans
		if( pUseMover->GetReqMp( pAddSkillProp->nReqMp ) == 9999999 )
		{
			if( pUseMover->GetManaPoint() != pUseMover->GetMaxManaPoint() )
			{
				if( TRUE == pUseMover->IsPlayer() )
				{
					( (FLWSUser*)pUseMover )->AddDefinedText( TID_GAME_REQMP, "" );
				}
				return FSC_SKILLUSE_FAIL;
			}
			if( pUseMover->GetFatiguePoint() != pUseMover->GetMaxFatiguePoint() )
			{
				if( TRUE == pUseMover->IsPlayer() )
				{
					( (FLWSUser*)pUseMover )->AddDefinedText( TID_GAME_REQFP, "" );
				}
				return FSC_SKILLUSE_FAIL;
			}
		}
		else
		{
			if( pUseMover->GetManaPoint() - pUseMover->GetReqMp( pAddSkillProp->nReqMp ) < 0 )		// ������ ���ڸ��Ƿ� ���
			{
				if( TRUE == pUseMover->IsPlayer() )
				{
					( (FLWSUser*)pUseMover )->AddDefinedText( TID_GAME_REQMP, "" );
				}
				return FSC_SKILLUSE_FAIL;
			}
		}
#endif // worldserver

		if( sutType == SUT_QUEUESTART )		// ��ųť ���� ��ų. - ť���� ��� ��ų�� ĳ������ ���Ƽ� �Ѵ�.
		{
			if( pUseMover->IsPlayer() )
			{
				dwMsg = OBJMSG_MAGICCASTING;
			}
		} 
		else if( sutType == SUT_QUEUEING )	// ��ųť ������ ��ų. - ĳ���� ���۾��� ����.
		{
			nMotion += 2;
			dwMsg = OBJMSG_MAGICSKILL;
		} 
		else
		{									// �Ϲ����� ���� ��ų ��� - ĳ���� ���� ����.
			dwMsg = OBJMSG_MAGICCASTING;
		}
	}
	else
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s dwSkillType = %d" ), pUseMover->GetName(), pSkillProp->dwSkillType );	// ������Ƽ���� �̻���.
	}

	//////////////////////////////////////////////////////////////////////////
	// ????????????
	if( sutType != SUT_QUEUEING && nCastingTime == 0 && pSkillProp->dwID != SKILL_INDEX( 223, SI_GEN_ATK_COUNTER ) )
		nMotion += 2;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// ��ų ���� �غ�
	pUseMover->SendActMsg( OBJMSG_STOP );
	pUseMover->ClearDest();
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//--- ���� ���� -----------------------------------------------------------------
	// ���½ý���	Neuz, World, Trans
	int nRet = pUseMover->m_pActMover->SendActMsg( dwMsg, nMotion, idFocusObj, nCastingTime, dwSkill );
	if( nRet == 1 )	// ��ų ���� ����.
	{
		// ��ɼ����� �ʿ��� �Ķ���ͼ���.
		pUseMover->SetActParam( OBJACT_USESKILL, dwSkill, idFocusObj, 0, nLevel, bControl );

#ifdef __WORLDSERVER
		if( pSkillProp->dwSkillType == KT_SKILL )
		{
			// ������ų�� ����ϴ� ��� fp����.
			if( (int)pAddSkillProp->nReqFp > 0 )
			{
				pUseMover->IncFatiguePoint( -(pUseMover->GetReqFp( pAddSkillProp->nReqFp )) );	// FP����
			}

			int nReqMp	= pUseMover->GetReqMp( pAddSkillProp->nReqMp );
			if( nReqMp > 0 )
			{
				pUseMover->IncManaPoint( -nReqMp );
			}
		}
		// ���½ý���	Neuz, World, Trans
		if( pSkillProp->dwSkillType == KT_MAGIC )
		{
			if( pUseMover->GetReqMp( pAddSkillProp->nReqMp ) == 9999999 )
			{
				pUseMover->IncFatiguePoint( -(pUseMover->GetFatiguePoint()) );	// FP����
				pUseMover->IncManaPoint( -(pUseMover->GetManaPoint()) );
			}
		}

		if( pSkillProp->dwNeedItemBulletKind2 != NULL_ID )	// �Ѿ�(?)�� �ʿ�� �ϴ°Ÿ�
		{
			FLItemElem *pItemElem = pUseMover->GetEquipItem( PARTS_BULLET );
			if( pItemElem )
			{
				pUseMover->ArrowDown( 1 );
			}
		}

		if( pSkillProp->IsDebuff() == TRUE )		// ���۸������θ� ���ͳ� �÷��̾ ���Ҽ��ִ�.
		{
			pTarget->SetJJim( pUseMover );			// pTarget�� ���ߴٴ°� ǥ��.
			pUseMover->m_nAtkCnt = 1;						// ���ڷ� �����̸� ���� ���� ī���� ����.
		}

		// DST param�� ����Ǵ� ��ų�� ���⼭ ������.
		g_xWSUserManager->AddUseSkill( pUseMover, dwSkill, nLevel, idFocusObj, sutType, nCastingTime );
#endif	// __WORLDSERVER
	} 
	else if( nRet == -2 )	// 0:ť�� �״´� -1:��  -2:���������Ͽ� ��ҽ�Ŵ   -3:��
	{
		return FSC_SKILLUSE_FAIL;
	}
	
	pUseMover->m_dwFlag |= MVRF_SKILL;		// ��ų��� ����� ���۵ƴ�.


#if defined __WORLDSERVER && defined __AGGRO16
	if( pSkillProp->dwID == SKILL_INDEX( 403, SI_LOD_SUP_ANGER ) )
	{
		if( pTarget->GetAggroMng() != NULL )
			pTarget->GetAggroMng()->SumLodSupAnger( *pTarget, *pUseMover );
	}

	if( pSkillProp->dwID == SKILL_INDEX( 400, SI_LOD_SUP_PULLING ) )
	{
		if( pTarget->GetAggroMng() != NULL )
			pTarget->GetAggroMng()->SumLodSupPulling( *pTarget, *pUseMover );
	}
#endif //__AGGRO16


#ifdef __WORLDSERVER
	if( g_eLocal.GetState( EVE_PK ) )
	{
		if( !pUseMover->IsChaotic() && ( pTarget->IsPKPink() || ( pTarget->IsChaotic() && pSkillProp->IsDebuff() == FALSE ) ) )
		{
			pUseMover->SetPKPink( GetTickCount() + SEC(prj.m_PKSetting.nReadyAttack) );
			g_xWSUserManager->AddPKPink( pUseMover, 1 );	
		}
	}
#endif // __WORLDSERVER


#ifdef __CLIENT
	if( pUseMover->IsActiveMover() )		// g_pPlayer�ϰ��
	{
		CWndTaskBar* pTaskBar = g_WndMng.m_pWndTaskBar;
		if( pTaskBar->m_nExecute == 1 )		// ��ų�� ��� ����߻��¿�����
			pTaskBar->m_nExecute = 2;		// ���� ��ų�� ��������� ��ȯ.
	}

	{
		DWORD dwLinkSfxDouble = NULL_ID;
		switch( dwSkill )
		{	// ����� XI_ �߰��ϸ� CreateSfx�� CSfxPartsLink()���� �߰����Ѿ� ��/
		case SKILL_INDEX( 140, SI_BLD_DOUBLEAX_SPRINGATTACK ):	dwLinkSfxDouble = XI_INDEX( 333, XI_SKILL_BLD_DOUBLEAX_SPRINGATTACK01 );	break;
		case SKILL_INDEX( 142, SI_BLD_DOUBLE_SONICBLADE ):		dwLinkSfxDouble = XI_INDEX( 336, XI_SKILL_BLD_DOUBLE_SONICBLADE01 );	break;
		case SKILL_INDEX( 133, SI_KNT_TWOSW_CHARGE ):			dwLinkSfxDouble = XI_INDEX( 315, XI_SKILL_KNT_TWOSW_CHARGE01 );	break;
		case SKILL_INDEX( 139, SI_BLD_DOUBLESW_BLADEDANCE ):	dwLinkSfxDouble = XI_INDEX( 334, XI_SKILL_BLD_DOUBLESW_BLADEDANCE01 );      break;

		// ������ ��� ����Ʈ : sfx�������� ���̸� �Ҹ���
		case SKILL_INDEX( 214, SI_JST_YOYO_VATALSTAB ):	dwLinkSfxDouble = XI_INDEX( 527, XI_SKILL_JST_YOYO_VATALSTAB01 );	break;
		case SKILL_INDEX( 212, SI_JST_YOYO_HITOFPENYA ):	dwLinkSfxDouble = XI_INDEX( 525, XI_SKILL_JST_YOYO_HITOFPENYA01 );	break;

		case SKILL_INDEX( 207, SI_JST_YOYO_CRITICALSWING ):	dwLinkSfxDouble = XI_INDEX( 520, XI_SKILL_JST_SUP_CRITICALSWING01 );	break;
		case SKILL_INDEX( 197, SI_ACR_SUP_SLOWSTEP ):	dwLinkSfxDouble = XI_INDEX( 510, XI_SKILL_ACR_YOYO_SLOWSTEP01 );	break;
		case SKILL_INDEX( 208, SI_JST_SUP_POISON ):		dwLinkSfxDouble = XI_INDEX( 521, XI_SKILL_JST_SUP_POISON01 );	break;
		case SKILL_INDEX( 209, SI_JST_SUP_BLEEDING ):	dwLinkSfxDouble = XI_INDEX( 522, XI_SKILL_JST_SUP_BLEEDING01 );	break;
		case SKILL_INDEX( 211, SI_JST_YOYO_BACKSTAB ):	dwLinkSfxDouble = XI_INDEX( 524, XI_SKILL_JST_YOYO_BACKSTAB01 );	break;
		case SKILL_INDEX( 210, SI_JST_SUP_ABSORB ):	dwLinkSfxDouble = XI_INDEX( 523, XI_SKILL_JST_SUP_ABSORB01 );	break;
			break;
		}
		if( dwLinkSfxDouble != NULL_ID )
		{
			CSfx *pSfx = CreateSfx( g_Neuz.m_pd3dDevice, dwLinkSfxDouble, pUseMover->GetPos(), pUseMover->GetId() );  // ���������̱⶧���� ������ �ڱ⿡�� ��Ÿ����.
			if( pSfx )
			{
				pSfx->SetPartLink( 0 );		// �����տ� ��ũ.
				pSfx = CreateSfx( g_Neuz.m_pd3dDevice, dwLinkSfxDouble, pUseMover->GetPos(), pUseMover->GetId() );  // ��ũ���� �ϳ��� ����.
				if( pSfx )
					pSfx->SetPartLink( 1 );		// �տ� ��ũ.
			}
		}
	}

	// �޼� ��ũ ����
	DWORD dwLinkSfxLeft = NULL_ID;
	switch( dwSkill )
	{	// ��ũ�κ� ��ų�̸� Ȱ ���� ����Ʈ �޼տ� ��ũ�Ѵ�
	case SKILL_INDEX( 216, SI_RAG_BOW_ICEARROW ):	dwLinkSfxLeft = XI_INDEX( 529, XI_SKILL_RAG_BOW_ICEARROW01 );	break;
	case SKILL_INDEX( 194, SI_ACR_BOW_JUNKBOW ):	dwLinkSfxLeft = XI_INDEX( 507, XI_SKILL_ACR_BOW_JUNKBOW01 );	break;
	case SKILL_INDEX( 196, SI_ACR_BOW_AIMEDSHOT ):	dwLinkSfxLeft = XI_INDEX( 509, XI_SKILL_ACR_BOW_AIMEDSHOT01 );	break;
	case SKILL_INDEX( 198, SI_ACR_BOW_SILENTSHOT ):	dwLinkSfxLeft = XI_INDEX( 511, XI_SKILL_ACR_BOW_SILENTSHOT01 );	break;
	case SKILL_INDEX( 200, SI_ACR_BOW_ARROWRAIN ):	dwLinkSfxLeft = XI_INDEX( 513, XI_SKILL_ACR_BOW_ARROWRAIN01 );      break;
	case SKILL_INDEX( 202, SI_ACR_BOW_AUTOSHOT ):	dwLinkSfxLeft = XI_INDEX( 515, XI_SKILL_ACR_BOW_AUTOSHOT01 );      break;
		
	case SKILL_INDEX( 215, SI_RAG_SUP_FASTATTACK ):	  dwLinkSfxLeft = XI_INDEX( 528, XI_SKILL_RAG_SUP_FASTATTACK01 );break;
	case SKILL_INDEX( 217, SI_RAG_BOW_FLAMEARROW ):	  dwLinkSfxLeft = XI_INDEX( 530, XI_SKILL_RAG_BOW_FLAMEARROW01 );break;
	case SKILL_INDEX( 218, SI_RAG_BOW_PIRCINGARROW ): dwLinkSfxLeft = XI_INDEX( 531, XI_SKILL_RAG_BOW_PIRCINGARROW01 );break;
	case SKILL_INDEX( 219, SI_RAG_BOW_POISONARROW ):  dwLinkSfxLeft = XI_INDEX( 532, XI_SKILL_RAG_BOW_POISONARROW01 );break;
	case SKILL_INDEX( 220, SI_RAG_BOW_SILENTARROW ):  dwLinkSfxLeft = XI_INDEX( 533, XI_SKILL_RAG_BOW_SILENTARROW01 );break;
	case SKILL_INDEX( 222, SI_RAG_BOW_TRIPLESHOT ):   dwLinkSfxLeft = XI_INDEX( 535, XI_SKILL_RAG_BOW_TRIPLESHOT01 );break;
		
		break;
	}
	
	if( dwLinkSfxLeft != NULL_ID )
	{
		CSfx *pSfx = CreateSfx( g_Neuz.m_pd3dDevice, dwLinkSfxLeft, pUseMover->GetPos(), pUseMover->GetId() );  // ���������̱⶧���� ������ �ڱ⿡�� ��Ÿ����.
		if( pSfx )
			pSfx->SetPartLink( 1 );		// �޼տ� ��ũ.
	}

	// ������ ��ũ ����
	DWORD dwLinkSfxRight = NULL_ID;
	switch( dwSkill )
	{
	case SKILL_INDEX( 4, SI_MER_ONE_SPLMASH ): dwLinkSfxRight = XI_INDEX( 1731, XI_SKILL_MER_ONE_SUPPORT04 ); break;
	case SKILL_INDEX( 12, SI_MER_ONE_GUILOTINE ): dwLinkSfxRight = XI_INDEX( 1729, XI_SKILL_MER_ONE_SUPPORT02 ); break;
	case SKILL_INDEX( 14, SI_MER_ONE_REFLEXHIT ): dwLinkSfxRight = XI_INDEX( 1728, XI_SKILL_MER_ONE_SUPPORT01 ); break;
	case SKILL_INDEX( 11, SI_MER_ONE_SPECIALHIT ): dwLinkSfxRight = XI_INDEX( 1730, XI_SKILL_MER_ONE_SUPPORT03 ); break;
	case SKILL_INDEX( 117, SI_ASS_KNU_POWERFIST ):	dwLinkSfxRight = XI_INDEX( 1734, XI_SKILL_ASS_KNU_SUPPORT03 );	break;
	case SKILL_INDEX( 104, SI_ASS_KNU_TAMPINGHOLE ):	dwLinkSfxRight = XI_INDEX( 1733, XI_SKILL_ASS_KNU_SUPPORT02 );	break;
	case SKILL_INDEX( 105, SI_ASS_KNU_BURSTCRACK ):	dwLinkSfxRight = XI_INDEX( 1732, XI_SKILL_ASS_KNU_SUPPORT01 );	break;
	case SKILL_INDEX( 223, SI_GEN_ATK_COUNTER ):	dwLinkSfxRight = XI_INDEX( 524, XI_SKILL_JST_YOYO_BACKSTAB01 );	break;
		break;
	}
	
	if( dwLinkSfxRight != NULL_ID )
	{
		CSfx *pSfx = CreateSfx( g_Neuz.m_pd3dDevice, dwLinkSfxRight, pUseMover->GetPos(), pUseMover->GetId() );  // ���������̱⶧���� ������ �ڱ⿡�� ��Ÿ����.
		if( pSfx )
			pSfx->SetPartLink( 0 );		// �����տ� ��ũ.
	}

	// ���� Ȥ�� ������ų�� �ߵ����� ����Ʈ�߻�
	// �ڱ�id GetId()
	// Ÿ��id idFocusObj
	if( pSkillProp->dwSfxObj != NULL_ID )
	{
		CreateSfx( g_Neuz.m_pd3dDevice, pSkillProp->dwSfxObj, pUseMover->GetPos(), pUseMover->GetId() );  // ���������̱⶧���� ������ �ڱ⿡�� ��Ÿ����.
	}	
	// ���ݽ�ų�� Ÿ���� ���� �̸� ȭ�鿡 ����� ����
	if( pSkillProp->dwExeTarget != EXT_MELEEATK && 
		pSkillProp->dwExeTarget != EXT_MAGICATK && 
		pSkillProp->dwExeTarget != EXT_AROUNDATK &&  
		pSkillProp->dwExeTarget != EXT_MAGICATKSHOT && 
		pTarget->IsPlayer() )
	{
		CString str;
		if( pUseMover == g_pPlayer ) // ���� ����
		{
			str.Format( prj.GetText( TID_GAME_SKILL_SELF ), pSkillProp->szName );
			g_WndMng.PutString( (LPCTSTR)str, NULL, prj.GetTextColor( TID_GAME_SKILL_SELF ) );	
		}
		else if( pTarget == g_pPlayer && ( pUseMover != pTarget || pSkillProp->dwID == SKILL_INDEX( 51, SI_ASS_CHEER_CIRCLEHEALING ) ) ) // ���� ������ �ƴ϶��
		{
			str.Format( prj.GetText( TID_GAME_SKILL_MATE ), pUseMover->GetName(), pSkillProp->szName );
			g_WndMng.PutString( (LPCTSTR)str, NULL, prj.GetTextColor( TID_GAME_SKILL_MATE ) );	
		}
	}

	if( g_xFlyffConfig->GetMainLanguage() == LANG_JAP )
	{
		if( g_pPlayer )
		{
			if( g_Option.m_bVoice )
			{
				if( pUseMover->IsActiveMover() )
				{
					int nindex = 0;
					
					if( NULL == pSkillProp )
						return TRUE;

					if( pSkillProp->dwID != NULL_ID )
					{
						if( pUseMover->GetSex() == 0 )
							nindex = pSkillProp->dwID+3000;
						else
							nindex = pSkillProp->dwID+3500;

						PLAYSND( nindex );
					}
				}
			}
		}
	}
#endif	// __CLIENT

	return FSC_SKILLUSE_SUCCESS;
}

// nLevel�� dwSkill�� pTarget�� ���Ͽ� �ߵ���.  ����Ŀ�� this
// bIgnoreProp�� TRUE�̸� ���ο��� Ȯ������� �����ϰ� 100% ����ǰ� �Ѵ�.
void	FLSkillSystem::DoActiveSkill( CCtrl* pUseCtrl, DWORD dwSkill, int nLevel, CCtrl *pTarget, bool bIgnoreProp, BOOL bControl )
{
	SkillProp *pSkillProp;
	AddSkillProp *pAddSkillProp;
	if( pUseCtrl->GetSkillProp( &pSkillProp, &pAddSkillProp, dwSkill, nLevel, "DoActiveSkill" ) == FALSE )
	{
		return;
	}

	if( pSkillProp == NULL || pAddSkillProp == NULL )
	{
		if( pUseCtrl->GetType() == OT_MOVER && ( static_cast<CMover*>(pUseCtrl) )->IsPlayer() == TRUE )
		{
			PT_ITEM_SPEC pItemProp = ( static_cast<CMover*>(pUseCtrl) )->GetActiveHandItemProp();
			if( pItemProp != NULL )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "GetActiveHandItemProp : ItemId:[%d]" ), pItemProp->dwID );
			}

			FLERROR_LOG( PROGRAM_NAME, _T( "ActiveSkill Invalid. PlayerId:[%07d], SkillId:[%d]" ), ( static_cast<CMover*>(pUseCtrl) )->m_idPlayer, dwSkill );
		}

		return;
	}

	if( pSkillProp->dwExeTarget == EXT_MAGICATKSHOT || pSkillProp->dwExeTarget == EXT_MAGICSHOT )		// ��� ���¸� sfx�� �߻�
	{
		ShootSkill( pUseCtrl, pTarget, pSkillProp, pAddSkillProp );
	}
	else
	{
		DoApplySkill( pUseCtrl, pTarget, pSkillProp, pAddSkillProp, bIgnoreProp, 0, FALSE, bControl );

#ifdef __WORLDSERVER		
		g_xWSUserManager->AddDoApplySkill( pUseCtrl, pTarget->GetId(), dwSkill, nLevel );
#endif
	}
}

//
// ��ų ����� �۵��ϴ� Ÿ�� �������� �Ǹ� �� �Լ��� ȣ���� 
// ������ ��ų.    
//
BOOL	FLSkillSystem::OnMeleeSkill( CMover* pUseMover, int /*nType*/, int nCount )
{
	if( pUseMover->GetAct() != OBJACT_USESKILL )	
		return FALSE;	// ��ų����� Ÿ���� �Ǳ����� �ٸ� ������� �ٲ���. �̷���Ȳ�� �ͼ� �ȵȴ�.

	int		nSkill = pUseMover->GetActParam( 0 );
	OBJID	idTarget = pUseMover->GetActParam( 1 );
	DWORD	dwLevel	= pUseMover->GetActParam( 3 );
	BOOL	bControl = pUseMover->GetActParam( 4 );

	SkillProp* pSkillProp;
	AddSkillProp* pAddSkillProp;
	// ��ų������Ƽ ������.
	if( pUseMover->GetSkillProp( &pSkillProp, &pAddSkillProp, nSkill, dwLevel, "OnMeleeSkill" ) == FALSE )
	{
		return FALSE;
	}

	// .Ÿ�ٿ��� ��ųȿ��(����������..)�� �ش�.
	CMover* pFocusObj = prj.GetMover( idTarget );		// Ÿ�ٹ��������͸� ��.
	if( IsInvalidObj( pFocusObj ) )	
		return FALSE;

#if defined(__CLIENT)
	CModelObject* pModel =(CModelObject*)pUseMover->GetModel();
	if( pModel )
	{
		pModel->m_SparkInfo.m_bUsed  = TRUE;
		pModel->m_SparkInfo.m_v3Color.x = 2.0f;
		pModel->m_SparkInfo.m_v3Color.y = 2.0f;
		pModel->m_SparkInfo.m_v3Color.z = 2.0f;
		pModel->m_SparkInfo.m_nCnt = 0;
		pModel->m_SparkInfo.m_fLerp= 1.0f;

		D3DXVECTOR3 vDir;
		if( pFocusObj == pUseMover )
			vDir = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		else
			vDir = pUseMover->GetPos() - pFocusObj->GetPos();

		D3DXVec3Normalize( &pModel->m_SparkInfo.m_v3SparkDir, &vDir );
	}
#endif //defined(__CLIENT) 

	int nDamage = 1;

	// ��� ���¸� sfx�߻�.
	if( pSkillProp->dwExeTarget == EXT_MAGICATKSHOT || pSkillProp->dwExeTarget == EXT_MAGICSHOT	)
	{
#ifdef __WORLDSERVER
		int nMaxDmgCnt	= 1;
		if( pSkillProp->tmContinuousPain != NULL_ID )
			nMaxDmgCnt	= ( pAddSkillProp->dwSkillTime / pAddSkillProp->dwPainTime ) + 1;

		float fDmgPower = 0;
		if( pSkillProp->dwDmgShift != NULL_ID )
			fDmgPower	= (int)pSkillProp->dwDmgShift * 0.2f;

		pUseMover->OnAttackSFX( pFocusObj->GetId(),
								pAddSkillProp->dwSkillLvl,
								pSkillProp->dwID,
								0,
								pUseMover->GetAngle(),
								fDmgPower,
								( pSkillProp->dwSkillType == KT_SKILL? AF_MELEESKILL: AF_MAGICSKILL ),
								bControl );

#endif	// __WORLDSERVER
		ShootSkill( pUseMover, pFocusObj, pSkillProp, pAddSkillProp );		// �������� �̺κ��� ����ȵȴ�.
	}
	else
	{
		// ������� ó��. ���ο��� ApplySkill()�� ȣ��.
		nDamage = DoApplySkill( pUseMover, pFocusObj, pSkillProp, pAddSkillProp, false, 0, FALSE, bControl );

	}
	FLTRACE_LOG( PROGRAM_NAME, _T( "OnMeleeSkill%s, " ), pSkillProp->szName );

	if( nDamage && nCount == 0 )		// ����Ÿ���ϰ�� ù��° Ÿ������ ����
	{
		// ��Ÿ�� �ð� ���� - Ŭ�� ���� ���ÿ� ���ư���
#ifdef __CLIENT
		if( pUseMover->IsActiveMover() )
#endif
		{
			//sun: 9, ���ο� AddSkillProp �÷� �߰� �� ����
			if( pAddSkillProp->dwCooldown != NULL_ID )	// ��Ÿ���� �ִ� ��ų�� ���
			{
				pUseMover->SetCoolTime( pAddSkillProp, "OnMeleeSkill" );
			}

		}
	}

	return TRUE;
}


//
//	���� ��ų�� Ÿ���� ȣ��.
//  nCount ���ͺ��� ��� ������ ȣ���� �Ǳ⶧���� nCount==0 �� ��츸 ����ġ�� �ö󰣴�.
//
BOOL	FLSkillSystem::OnMagicSkill( CMover* pUseMover, int /*nType*/, int nCount )
{
#ifdef __WORLDSERVER
	UNREFERENCED_PARAMETER( nCount );
#endif	//__WORLDSERVER


	if( pUseMover->GetAct() != OBJACT_USESKILL )	
		return FALSE;	// ��ų����� Ÿ���� �Ǳ����� �ٸ� ������� �ٲ���. �̷���Ȳ�� �ͼ� �ȵȴ�.

	int		nSkill = pUseMover->GetActParam( 0 );
	OBJID	idTarget = pUseMover->GetActParam( 1 );
	DWORD	dwLevel = pUseMover->GetActParam( 3 );
	BOOL	bControl = pUseMover->GetActParam( 4 );

	SkillProp*	pSkillProp = NULL;
	AddSkillProp* pAddSkillProp = NULL;
//	LPSKILL		pSkill	= NULL;

	BOOL bRet = pUseMover->GetSkillProp( &pSkillProp, &pAddSkillProp, nSkill, dwLevel, "FLSkillSystem::OnMagicSkill" );
	if( bRet == FALSE )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "��ų�����б� ������. ������������ Motion=%d %d" ), pUseMover->m_dwMotion, dwLevel );
		return FALSE;
	}

	CMover* pFocusObj = prj.GetMover( idTarget );		// Ÿ�ٹ��������͸� ��.
	if( IsInvalidObj( pFocusObj ) )		
		return FALSE;

	if( pSkillProp->dwID == SKILL_INDEX( 44, SI_ASS_HEAL_HEALING ) )	// ���� ��밡 �׾����� ���
	{
		if( pFocusObj->IsDie() )
			return FALSE;
	}

	BOOL bSuccess = TRUE;
	// ��� ���¸� sfx�߻�.
	if( pSkillProp->dwExeTarget == EXT_MAGICATKSHOT || pSkillProp->dwExeTarget == EXT_MAGICSHOT	)
	{
#ifdef __WORLDSERVER
		int nMaxDmgCnt	= 1;
		if( pSkillProp->tmContinuousPain != NULL_ID )
			nMaxDmgCnt	= ( pAddSkillProp->dwSkillTime / pAddSkillProp->dwPainTime ) + 1;

		float fDmgPower = 0;
		if( pSkillProp->dwDmgShift != NULL_ID )
			fDmgPower	= (int)pSkillProp->dwDmgShift * 0.2f;

		pUseMover->OnAttackSFX( pFocusObj->GetId(),
								pAddSkillProp->dwSkillLvl,
								pSkillProp->dwID,
								0,
								pUseMover->GetAngle(),
								fDmgPower,
								( pSkillProp->dwSkillType == KT_SKILL? AF_MELEESKILL: AF_MAGICSKILL ),
								bControl );

#endif	// __WORLDSERVER
		ShootSkill( pUseMover, pFocusObj, pSkillProp, pAddSkillProp );		// �������� �̺κ��� ����ȵȴ�.
	}
	else
	{
		// ������� ó��. ���ο��� ApplySkill()�� ȣ��.
		int nDamage = DoApplySkill( pUseMover, pFocusObj, pSkillProp, pAddSkillProp, false, 0, FALSE, bControl );
		if( nDamage == 0 )
			return FALSE;

#ifdef __CLIENT	
		if( pSkillProp->dwID == SKILL_INDEX( 107, SI_MAG_MAG_BLINKPOOL ) )
		{
			CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );

			if( pUseMover->IsActiveMover() )
			{
				D3DXVECTOR3 vStart;
				D3DXVECTOR3 vEnd;

				vStart = pUseMover->GetPos();		vStart.y += 1.0f;
				vEnd = pWndWorld->m_vTelePos;

				CWorld *pWorld = pUseMover->GetWorld();

				if( pWorld->IntersectObjLine( NULL, vStart, vEnd, FALSE, FALSE ) )
				{
					g_WndMng.m_pWndWorld->SetNextSkill( NEXTSKILL_NONE );
					g_WndMng.PutString( prj.GetText( TID_GAME_BLOCKTARGETING ), NULL, prj.GetTextColor( TID_GAME_BLOCKTARGETING ) );
					return FALSE;
				}

				g_DPlay.SendTeleSkill( pUseMover->GetId(), pWndWorld->m_vTelePos );
			}
		}
#endif // Client		
	}

	if( bSuccess )	// ��ų��� �����ϸ� �����ȱ���. / ����ġ �ȿ���.
	{
#ifdef __WORLDSERVER
		// �߻��� ������ �������� ��´�.
		int nReqMp = pUseMover->GetReqMp( pAddSkillProp->nReqMp );

		//sun: 10�� ���½ý���	Neuz, World, Trans
		if( nReqMp != 9999999 )
		{
			if( pAddSkillProp->nSkillCount > 1 )
				nReqMp /= pAddSkillProp->nSkillCount;

			pUseMover->IncManaPoint( -nReqMp );
			if( pAddSkillProp->nReqFp != NULL_ID )
				pUseMover->IncFatiguePoint( -(pUseMover->GetReqFp( pAddSkillProp->nReqFp )) );
		}

		//sun: 10�� ���½ý���	Neuz, World, Trans
		if( pSkillProp->dwID == SKILL_INDEX( 244, SI_RIG_HERO_RETURN ) )
		{
			REPLACE_TYPE type = REPLACE_NORMAL;

			PRegionElem pRgnElem	= NULL;
			CWorld* pWorld	= pUseMover->GetWorld();
			if( !pWorld )
				return FALSE;

			if( pUseMover->IsChaotic() )
			{
				if( pWorld->GetID() != pWorld->m_dwIdWorldRevival && pWorld->m_dwIdWorldRevival != 0 )
				{
					pRgnElem	= g_WorldMng.GetRevivalPosChao( pWorld->m_dwIdWorldRevival, pWorld->m_szKeyRevival );
				}
				if( NULL == pRgnElem )	// Find near revival pos
				{
					pRgnElem	= g_WorldMng.GetNearRevivalPosChao( pWorld->GetID(), pUseMover->GetPos() );		
				}
			}
			else
			{
				if( pWorld->GetID() != pWorld->m_dwIdWorldRevival && pWorld->m_dwIdWorldRevival != 0 )
				{
					pRgnElem	= g_WorldMng.GetRevivalPos( pWorld->m_dwIdWorldRevival, pWorld->m_szKeyRevival );
				}
				if( NULL == pRgnElem )	// Find near revival pos
				{
					pRgnElem	= g_WorldMng.GetNearRevivalPos( pWorld->GetID(), pUseMover->GetPos() );
				}
			}

			if( NULL != pRgnElem )
			{
				pUseMover->REPLACE( g_uIdofMulti, pRgnElem->m_dwWorldId, pRgnElem->m_vPos, type, nRevivalLayer );
			}
			else
			{
				pUseMover->REPLACE( g_uIdofMulti, pWorld->GetID(), pUseMover->GetPos(), type, nDefaultLayer );
			}
		}
		else if( pSkillProp->dwID == SKILL_INDEX( 238, SI_KNT_HERO_DRAWING ) )
		{
			D3DXVECTOR3 vSrc	= pUseMover->GetPos();
			FLOAT		fAngle	= GetDegree( pFocusObj->GetPos(), pUseMover->GetPos() );	// �����ڿ� Ÿ���� ������ ����
			//vSrc.x -= 1.0f;
			//vSrc.z -= 1.0f;

			CWorld* pFocusObjWorld = pFocusObj->GetWorld();
			if( NULL == pFocusObjWorld )
				return FALSE;

			CWorld* pUseMoverWorld = pUseMover->GetWorld();
			if( NULL == pUseMoverWorld )
				return FALSE;

			if( pFocusObjWorld->GetID() != pUseMoverWorld->GetID() || pFocusObj->GetLayer() != pUseMover->GetLayer() )
			{
				FLWARNING_LOG( PROGRAM_NAME, _T("SI_KNT_HERO_DRAWING Focus[%d][%d] UseMover[%d][%d]"), 
					pFocusObjWorld->GetID(), pFocusObj->GetLayer(), pUseMoverWorld->GetID(), pUseMover->GetLayer() );
				return FALSE;
			}

// 			pFocusObj->SetPos(vSrc);
// 			g_xWSUserManager->AddSetPos( pFocusObj, vSrc );
// 			g_xWSUserManager->AddPushPower( pFocusObj, pFocusObj->GetPos(), pFocusObj->GetAngle(), fAngle, 0.2f );
// 			pFocusObj->ClearDestObj();

			D3DXVECTOR3 pos = pFocusObj->GetPos() - pUseMover->GetPos();
			const float distance = ::sqrt( D3DXVec3LengthSq( &pos ) ) * -0.1f;
			if( !pFocusObj->pushedByAnotherPower( fAngle, distance ) ) {
				return FALSE;
			}
		}

#endif // WORLDSERVER

		// ��Ÿ�� �ð� ���� - Ŭ�� ���� ���ÿ� ���ư���
#ifdef __CLIENT
		if( pUseMover->IsActiveMover() && nCount == 0 )		// ����Ÿ���ϰ�� ù��° Ÿ������ ����
#endif // __CLIENT
			//sun: 9, ���ο� AddSkillProp �÷� �߰� �� ����
			if( pAddSkillProp->dwCooldown != NULL_ID )
				pUseMover->SetCoolTime( pAddSkillProp, "OnMagicSkill" );

	} // bSuccess

#if defined(__CLIENT)
	CModelObject* pModel = (CModelObject*)pUseMover->GetModel();
	if( pModel )
	{
		pModel->m_SparkInfo.m_bUsed  = TRUE;
		pModel->m_SparkInfo.m_v3Color.x = 2.0f;
		pModel->m_SparkInfo.m_v3Color.y = 2.0f;
		pModel->m_SparkInfo.m_v3Color.z = 2.0f;
		pModel->m_SparkInfo.m_nCnt = 0;
		pModel->m_SparkInfo.m_fLerp= 1.0f;

		AngleToVectorXZ( &pModel->m_SparkInfo.m_v3SparkDir, pUseMover->GetAngle(), -1.0f );
	}
#endif //defined(__CLIENT)

	return TRUE;
}

//
//	���Ӽ��� ���� ��ų�� �ð��� �ٵ� �������� ȣ���.
//
void	FLSkillSystem::OnEndSkillState( CMover* pTarget, DWORD dwSkill, DWORD dwLevel )
{
	SkillProp* pSkillProp = NULL;
	AddSkillProp *pAddSkillProp = NULL;
	if( pTarget->GetSkillProp( &pSkillProp, &pAddSkillProp, dwSkill, dwLevel, "OnEndSkillState" ) == FALSE )
	{
		return;
	}

	if( dwSkill == SKILL_INDEX( 204, SI_ACR_YOYO_COUNTER ) )	// ī���� ���ý�ų�� ���
	{
		pTarget->m_pActMover->ResetState( OBJSTA_ATK_ALL );		// ���ݵ��� �ߴܽ�Ŵ.  (�̷� ������ ��ų�� ������ �Ϲ�ȭ ��Ű��)
	}

	//#if defined( __CLIENT ) 
	//
	//	switch( dwSkill )	// 091022 mirchang - �����ؼ�, ��ٸ��� ��ų ���� ���� �� sfx ����
	//	{
	//		case SKILL_INDEX( 65, SI_MAG_FIRE_HOTAIR ):
	//			SetRemoveSfxObj( pSkillProp->dwSfxObj2 );
	//			break;
	//		case SKILL_INDEX( 242, SI_PSY_HERO_STONE ):
	//		case SKILL_INDEX( 10, SI_MER_SHIELD_PANBARRIER ):
	//		case SKILL_INDEX( 9, SI_MER_SHIELD_PROTECTION ):
	//			SetRemoveSfxObj( pSkillProp->dwSfxObj4 );
	//			break;
	//
	//			//gmpbigsun( 20100823 ) �� .. �ڵ����� �����ϰ� �ٲ��� �Ѵ�. �ϴ� �ϰ�...
	//		default:
	//			SetRemoveSfxObj( pSkillProp->dwSfxObj4 );
	//			break;
	//	}
	//  #endif	// 

#ifdef __WORLDSERVER
	// �����ϰ�� �����鿡�� ������.
	for( size_t Nth = 0; Nth < MAX_SKILL_PARAM; ++Nth )
	{
		pTarget->ResetDestParam( pAddSkillProp->dwDestParam[ Nth ], pAddSkillProp->dwAdjParamVal[ Nth ], TRUE );
	}
// 	pTarget->ResetDestParam( pAddSkillProp->dwDestParam1, pAddSkillProp->dwAdjParamVal[0], TRUE );
// 	pTarget->ResetDestParam( pAddSkillProp->dwDestParam2, pAddSkillProp->dwAdjParamVal[1], TRUE );
#endif
}

// �߻������� ��ų�� �߻�ü�� �����ϴ� �κ�
// �� �߻�ü�� Ÿ�ٿ��� �¾����� DoApplySkill()�� �ߵ��ȴ�.
#ifdef __CLIENT
void	FLSkillSystem::ShootSkill( CCtrl* pUseCtrl, CCtrl* pTarget, SkillProp* pSkillProp, AddSkillProp* pAddSkillProp )
{
	DWORD dwShootObj = pSkillProp->dwSfxObj2;
	if( dwShootObj != NULL_ID )
	{
		D3DXVECTOR3 vPos = pUseCtrl->GetPos();

		if( ((CMover*)pUseCtrl)->IsPlayer() )
		{
			// ���쿡�� ������ ȭ���� ��� ������ ������
			if( pSkillProp->dwNeedItemKind3 == IK3_BOW )
			{
				// ȭ���� �޼տ� ����...
				CModelObject *pModel = (CModelObject *)pUseCtrl->GetModel();
				pModel->GetHandPos( &vPos, PARTS_LWEAPON, pUseCtrl->GetMatrixWorld() );		// �ָ� ������ǥ ����		
				vPos.y -=	1.0f;
			}

#ifdef __ADDWEAPON_CROSSBOW16
			if( pSkillProp->dwNeedItemKind3 == IK3_CROSSBOW )
			{
				// ȭ���� �޼տ� ����...
				CModelObject *pModel = (CModelObject *)pUseCtrl->GetModel();
				pModel->GetHandPos( &vPos, PARTS_LWEAPON, pUseCtrl->GetMatrixWorld() );		// �ָ� ������ǥ ����		
				vPos.y -=	1.0f;
			}
#endif //__ADDWEAPON_CROSSBOW16
		}

		CSfxShoot *pShootSfx = CreateShootSfx( D3DDEVICE, dwShootObj, vPos, pUseCtrl->GetId(), D3DXVECTOR3(0,0,0), pTarget->GetId() );
		if( pShootSfx )
		{		
			D3DXVECTOR3 vPos = pUseCtrl->GetPos();
			PLAYSND( pSkillProp->dwSndAttack1, &vPos );		// �߻� ȿ����.

			if( pUseCtrl->GetType() == OT_MOVER )
			{
				if( ((CMover*)pUseCtrl)->IsActiveMover() )		// g_pPlayer�� �򶧸� �����ؼ� ����.
				{
					int idSfxHit = ++(((CMover*)pUseCtrl)->m_idSfxHit);	// Ŭ�󿡼� �߻�ü�� ������ ��� ID�� ����� �װ��� ������ ����.
					int		nMaxDmgCnt = 1;
					if( pSkillProp->tmContinuousPain != NULL_ID )		// ���ӽ�ų�� �ִ� ������ ���¸�
						nMaxDmgCnt = (pAddSkillProp->dwSkillTime / pAddSkillProp->dwPainTime) + 1;		// ��� �������� �Գ�.

					if( pSkillProp->dwSkillType == KT_SKILL )
						g_DPlay.SendSfxID( pTarget->GetId(), idSfxHit, AF_MELEESKILL, NULL_ID, nMaxDmgCnt );
					else
						g_DPlay.SendSfxID( pTarget->GetId(), idSfxHit, AF_MAGICSKILL, NULL_ID, nMaxDmgCnt );
					pShootSfx->m_idSfxHit	= idSfxHit;
				}
			}

			pShootSfx->SetSkill( pSkillProp->dwID );
			pShootSfx->m_nMagicPower	= pAddSkillProp->dwSkillLvl;	// ��ų������ MagicPower������ ���̽ᵵ �ɰ� ����.
			pShootSfx->m_dwSfxHit = pSkillProp->dwSfxObj3;	// ���� ����Ʈ ���.

		}
		// ���ӽð����� ����Ʈ�� �����Ǿ�� �ϴ°��� CSkillInfluence::Process()���� ó���Ѵ�.
	}
	else
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s dwSfxObj2�� �������� ����" ), pSkillProp->szName );
	}
}
#else
void	FLSkillSystem::ShootSkill( CCtrl* /*pUseCtrl*/, CCtrl* /*pTarget*/, SkillProp* /*pSkillProp*/, AddSkillProp* /*pAddSkillProp*/ )	{}
#endif // CLIENT

int		FLSkillSystem::DoApplySkill( CCtrl* pUseCtrl, CCtrl *pTarget, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp, bool bIgnoreProp, int nParam, BOOL bOnlyDmg, BOOL bControl )
{
	int nResult = DoApplySkillEx( pUseCtrl, pTarget, pSkillProp, pAddSkillProp, bIgnoreProp, nParam, bOnlyDmg, bControl );
	if( nResult == 1 )
	{
#ifdef	__WORLDSERVER
		if( pUseCtrl != pTarget && pUseCtrl->GetType() == OT_MOVER && pTarget->GetType() == OT_MOVER )
		{
			if( pSkillProp->IsDebuff() == TRUE )	// ���۸����� ���� ���ݰ� ���ٰ� �����Ѵ�.
			{
				((CMover*)pTarget)->OnAttacked( (CMover*)pUseCtrl, 0, TRUE, 0 );	// TRUE�� Ÿ���̴�.
			}
		}
#endif 
	}

	return nResult;
}

// 
// ������ ��ų�� ȿ���� pTarget���� �����Ѵ�.
// ���������� ���� pTarget�� �߽����� �ٸ� target���� ����ȴ�.
// �̰��� ����ϴ� ��ü�� ����� �ƴҼ��� �ִ�
// ���⼭�� SpellRegion�� ���� �������� �з��� �ϰ� ���� ȿ��������
// ApplySkill���� ó���Ѵ�.
// bOnlyDmg : ���� �����������Ű�°͸� �Ѵ�.  ���� ���ӵ�������ü���� �����.
int		FLSkillSystem::DoApplySkillEx( CCtrl* pUseCtrl, CCtrl *pTarget, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp, bool bIgnoreProp, int nParam, BOOL bOnlyDmg, BOOL bControl )
{
	int		nRet = 1;
	switch( pSkillProp->dwSpellRegion )
	{
	case SRO_AROUND:	// ��� 0% ���� 100%
	case SRO_REGION:	// ��� 100% ���� 100%
		{
			if( pSkillProp->dwExeTarget != EXT_TROUPE && pSkillProp->dwExeTarget != EXT_TROUPEWITH )	// �شܴ���� �ƴҶ�.
			{
				int nApplyType = 0;
				nApplyType = pUseCtrl->TargetSelecter( pTarget );
				ApplySkillAround( pUseCtrl, pTarget, nApplyType, pSkillProp, pAddSkillProp, bIgnoreProp, bOnlyDmg, 0.0f, bControl );
			}
			else if( pSkillProp->dwExeTarget == EXT_TROUPE || pSkillProp->dwExeTarget == EXT_TROUPEWITH )	// �ش� ���(�ڱ� �ڽ� ����)
			{
				if( pUseCtrl->GetType() == OT_MOVER && ((CMover *)pUseCtrl)->IsPlayer() )
				{
					ApplySkillAroundTroupe( pUseCtrl, ((CMover *)pUseCtrl)->m_idparty, pSkillProp, pAddSkillProp, bIgnoreProp );
				}
			}

#ifdef __WORLDSERVER
			// chipi - ���� ctrl ���� ��ų�� ���� ��� ���� �� ī���Ǵ� ���� ����
			if( pSkillProp->tmContinuousPain != NULL_ID && !(((CMover*)pTarget)->IsPlayer() && ((CMover*)pTarget)->m_nDead) )	// ���ӵ����� ���³�?
			{
				CContDamageCtrl *pDmgCtrl = new CContDamageCtrl;	// ���ӵ������� �ִ� ���������Ʈ ���� - �޸�Ǯ�� ó�� �Ұ�
				if( pDmgCtrl )
				{
					pDmgCtrl->m_pSkillProp = pSkillProp;
					pDmgCtrl->m_pAddSkillProp = pAddSkillProp;
					pDmgCtrl->m_idSrc = pUseCtrl->GetId();
					pDmgCtrl->m_idTarget = pTarget->GetId();
					pDmgCtrl->SetPos( pTarget->GetPos() );		// �߽��� �Ǵ� Ÿ���� ��ǥ��...
					pDmgCtrl->m_bControl = bControl;

					// chipi - ���� ctrl ���� ��ų�� ���� ��� ���� �� ī���Ǵ� ���� ����
					if( ((CMover*)pUseCtrl)->IsPVPTarget( ((CMover*)pTarget) ) )
						pDmgCtrl->m_bDuelTarget = TRUE;

					if( pUseCtrl->GetWorld()->ADDOBJ( pDmgCtrl, FALSE, pUseCtrl->GetLayer() ) == FALSE )
					{
						SAFE_DELETE( pDmgCtrl );
					}
				}
			}
#endif
		}
		break;
	case SRO_LINE:		// this�� �߽����� m_fAngle����.
		{
			int nApplyType = 0;
			nApplyType = pUseCtrl->TargetSelecter( pTarget );
			ApplySkillLine( pUseCtrl, nApplyType, pSkillProp, pAddSkillProp, 2.5f, 2.5f, (float)pAddSkillProp->dwSkillRange, bIgnoreProp, bControl );
		}
		break;
	case SRO_DIRECT:	// ��� �ϳ����� ����
	default:
		if( pSkillProp->dwID != SKILL_INDEX( 163, SI_PSY_PSY_PSYCHICWALL ) )		// ����ű���� ó�� ����.
		{
			ApplySkill( pUseCtrl, pTarget, pSkillProp, pAddSkillProp, bIgnoreProp, nParam );
		}
		break;
	} 

#ifdef __WORLDSERVER		
	// ����ű���� ���� ����.
	if( pSkillProp->dwID == SKILL_INDEX( 163, SI_PSY_PSY_PSYCHICWALL ) )
	{
		BOOL bCreate = FALSE;
		int i = 0;
		if( pUseCtrl->GetType() == OT_MOVER )
		{
			if( ((CMover *)pUseCtrl)->IsPlayer() )
			{
				FLWSUser *pUser = (FLWSUser *)pUseCtrl;
				for( i = 0; i < 2; i ++ )		// �ѹ��� 2���̻� �������� �ʰ���.
				{
					if( pUser->m_pWall[i] == NULL )
					{
						CObj *pObj = CreateObj( D3DDEVICE, OT_CTRL, CTRL_INDEX( 29, CI_PSYCHICWALL ) );		// �� ��Ʈ�� ����.
						if( pObj )
						{
							pObj->SetPos( pUseCtrl->GetPos() );
							pObj->SetAngle( -(pUseCtrl->GetAngle()) );
							((CCommonCtrl *)pObj)->m_pSkillProp = pSkillProp;
							((CCommonCtrl *)pObj)->m_pAddSkillProp = pAddSkillProp;
							((CCommonCtrl *)pObj)->m_idAttacker = pUseCtrl->GetId();
							((CCommonCtrl *)pObj)->m_bControl = bControl;

							if( pUseCtrl->GetWorld()->ADDOBJ( pObj, TRUE, pUseCtrl->GetLayer() ) == TRUE )
							{
								pUser->m_pWall[i] = pObj;
								bCreate = TRUE;
							}
							else
							{
								SAFE_DELETE( pObj );
								bCreate = FALSE;
							}
							break;
						}
					}
				}
			}
		}
		if( bCreate == FALSE )	// ������������
			nRet = 0;			// ���� ����

	}


	if( pUseCtrl->GetType() == OT_MOVER )
	{
		// ��� ��ų���� ��ũ�Ϸ��������̸� �����Ѵ�!!~
		if( pSkillProp->dwID != SKILL_INDEX( 193, SI_ACR_SUP_DARKILLUSION ) )
		{
			if( ((CMover *)pUseCtrl)->HasBuff( BUFF_SKILL, SKILL_INDEX( 193, SI_ACR_SUP_DARKILLUSION ) ) )
			{
				((CMover *)pUseCtrl)->SetDarkCover( FALSE );
				((CMover *)pUseCtrl)->RemoveBuff( BUFF_SKILL, SKILL_INDEX( 193, SI_ACR_SUP_DARKILLUSION ) );		// ����.
			}
		}

		// ������ ������ Ư�� ������ ������ �����ϰ� �ɾ����
		if( pSkillProp->dwID == SKILL_INDEX( 208, SI_JST_SUP_POISON ) )
		{
			((CMover *)pUseCtrl)->RemoveBuff( BUFF_SKILL, SKILL_INDEX( 209, SI_JST_SUP_BLEEDING ) );
			((CMover *)pUseCtrl)->RemoveBuff( BUFF_SKILL, SKILL_INDEX( 210, SI_JST_SUP_ABSORB ) );
		}
		else if( pSkillProp->dwID == SKILL_INDEX( 209, SI_JST_SUP_BLEEDING ) )
		{
			((CMover *)pUseCtrl)->RemoveBuff( BUFF_SKILL, SKILL_INDEX( 208, SI_JST_SUP_POISON ) );
			((CMover *)pUseCtrl)->RemoveBuff( BUFF_SKILL, SKILL_INDEX( 210, SI_JST_SUP_ABSORB ) );
		}
		else if( pSkillProp->dwID == SKILL_INDEX( 210, SI_JST_SUP_ABSORB ) )
		{
			((CMover *)pUseCtrl)->RemoveBuff( BUFF_SKILL, SKILL_INDEX( 209, SI_JST_SUP_BLEEDING ) );
			((CMover *)pUseCtrl)->RemoveBuff( BUFF_SKILL, SKILL_INDEX( 208, SI_JST_SUP_POISON ) );
		}

	}	
#endif // WorldServer

#ifdef __CLIENT
	// ��ų�� �ʿ��� sfx�� ����.
	CreateSkillSfx( pUseCtrl, pTarget, pSkillProp, pAddSkillProp );
#endif

	// 2�� ��ų �ߵ�. ActiveSkill
	if( pSkillProp->dwActiveSkill != NULL_ID )
	{
		if( pSkillProp->dwActiveSkillRate == NULL_ID || xRandom(100) <= pSkillProp->dwActiveSkillRate )	// �ߵ�Ȯ���� �ɷȴ°�.
			DoActiveSkill( pUseCtrl, pSkillProp->dwActiveSkill, MIN_SKILL_USE_LEVEL, pTarget, false, bControl );		// dwActiveSkill Lv1�� �ߵ���.

	}

	return nRet;
}


// pTarget�� �߽����� �ֺ��� ȿ���� �����Ѵ�.
// ------------------------------------------
// Ʈ���� ������ �ְ�. ���Ͱ��������ִ�(��), ���ΰ��� ��ų�� ����Ҽ��� �ִ�.
// ȿ���� �ݵ�� SkillProp�� ���� �� ���̵� �Ѱܾ� �Ѵ�.
//
// ��쿡���� pAddSkillProp�� NULL�ϼ��� �ִ�.  �����ۿ��� ����Ҽ��� �ֱ⶧��. �������� �ֵ彺ų������ ����.
// fRangeCustom�� ���� ������쿡�� ������Ƽ nRange���� �����ϰ� fRangeCustom���� �ٲ��.
void	FLSkillSystem::ApplySkillAround( CCtrl* pUseCtrl, CCtrl* pTarget, int nApplyType, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp, bool bIgnoreProp, BOOL bOnlyDmg, FLOAT fRangeCustom, BOOL bControl )
{
#ifdef __WORLDSERVER
	int nRange	= 4;	// 4m
	CObj* pObj;
	D3DXVECTOR3 vPos = pTarget->GetPos();
	D3DXVECTOR3 vDist;

	if( pSkillProp == NULL )
		FLERROR_LOG( PROGRAM_NAME, _T( "pSkillProp == NULL D:%d A:%d-%d" ), pTarget->GetIndex(), pUseCtrl->GetType(), pUseCtrl->GetIndex() );

	FLOAT fRange = 4.0f;
	if( pAddSkillProp )										// �ֵ彺ų�� ������ �ű⼭ ����
		fRange = (float)pAddSkillProp->dwSkillRange;
	else
		fRange = (float)pSkillProp->dwSkillRange;			// ������ ��ų������Ƽ���� ����.

	if( fRange <= 4.0f )		nRange = 4;
	else if( fRange <= 8.0f )	nRange = 8;
	else if( fRange <= 16.0f )	nRange = 16;
	else						nRange = 32;

	if( fRangeCustom != 0.0f )
	{
		fRange = fRangeCustom;
	}

	if( fRange <= 0 )	// ������ 0�̰ų� �����ϼ��� ����.
		FLERROR_LOG( PROGRAM_NAME, _T( "D:%d A:%d-%d %d %f" ), pTarget->GetIndex(), pUseCtrl->GetType(), pUseCtrl->GetIndex(), pSkillProp->dwID, fRange );

	FLOAT fTargetRatio = 0.0f;
	if( pSkillProp->dwSpellRegion == SRO_REGION )	// REGION�Ӽ��϶� Ÿ�ٵ� �������� ����.
		fTargetRatio = 1.0f;

	BOOL	bApply = FALSE;
	BOOL	bTarget = FALSE;

	//------------ �������� �÷��̾��ΰ� 
	if( nApplyType & OBJTYPE_PLAYER )	
	{
		FOR_LINKMAP( pTarget->GetWorld(), vPos, pObj, nRange, CObj::linkPlayer, pTarget->GetLayer() )
		{
			bApply = FALSE;	// �ϴ� FALSE�� �ʱ�ȭ

			if( pObj->GetType() != OT_MOVER )
				FLERROR_LOG( PROGRAM_NAME, _T( "pObj�� MOVER�� �ƴϴ� %d" ), pObj->GetType() );

			if( pUseCtrl->GetType() == OT_MOVER )		// �����ڰ� ������.
			{
				CMover *pAttacker = (CMover *)pUseCtrl;
				if( pAttacker->IsPlayer() )			// �����ڰ� �÷��̾��
				{
					bApply	= TRUE;
					CMover *pDefender = (CMover *)pObj;
					if( pDefender->IsPlayer() )
					{
						bTarget = (pTarget == pDefender);

						if( bControl == FALSE && pAttacker->GetHitType( pDefender, bTarget, FALSE ) == HITTYPE_PK )
						{
							bApply = FALSE;
						}
						else if( pAttacker->m_nDuel == 1 && pDefender->m_nDuel == 1 && pAttacker->GetHitType( pDefender, bTarget, FALSE ) != HITTYPE_PVP )
						{
							bApply = FALSE;
						}
					}
				}
				else
				{	// �����ڰ� ���͸�
					bApply = TRUE;		// 
				}
			}
			else
			{	// �����ڰ� ������ �ƴϴ�.
				bApply = TRUE;
			}

			if( bApply )
			{
				if( pObj->IsRangeObj( pTarget, fRange ) )	// pTarget(Center)�� ���Ǿ�� pObj�� ���Ǿ �浹�ߴ���
				{
					if( pObj == pTarget && fTargetRatio == 0.0f )	// Ÿ�ٵ����� ������ 0�̸� �ڱ��ڽ�(Ÿ��)�� �������� ��������.
					{
					}
					else
					{
						if( pObj != pUseCtrl )		// ����Ŀ�� �˻���󿡼� ����.
						{
							CMover *pTargetObj = (CMover *)pObj;		// <- ���� ������ pObj�� �÷��̾�Ƿ� �ص���.
							bTarget = (pTarget == pTargetObj);
							if( IsValidObj( pTargetObj ) && pTargetObj->IsLive() )
								ApplySkill( pUseCtrl, pTargetObj, pSkillProp, pAddSkillProp, bIgnoreProp, 0, bOnlyDmg, bTarget );		// ��󿡰� ȿ���� ������.
						}
					}
				}

				bApply = FALSE;	// ���� ������ ���ؼ� �ʱ�ȭ.
			} // bApply
		}
		END_LINKMAP
	} // OBJTYPE_PLAYER

	// �������� ����/��Ʈ�� �ΰ�.
	if( nApplyType & (OBJTYPE_MONSTER | OBJTYPE_CTRL) )
	{
		FOR_LINKMAP( pTarget->GetWorld(), vPos, pObj, nRange, CObj::linkDynamic, pTarget->GetLayer() )	// linkDynamic�� ����
		{
			bApply = FALSE;
			if( pObj->GetType() == OT_MOVER )				// ����� ������.
			{
				CMover *pTargetObj = (CMover *)pObj;
				if( pTargetObj->IsPeaceful() == FALSE )		// NPC�� �ƴѰ�츸 ����
					bApply = TRUE;

				//sun: 8, 8.5�� ��� ������ų ����ȿ�� �Ұ��� ���� World
				CMover *pAttacker = (CMover *)pUseCtrl;
				if( pAttacker->IsPlayer() && pAttacker->IsChaotic() == FALSE && pTargetObj->GetProp()->dwClass == RANK_GUARD )
					bApply = FALSE;

			}
			else
			{
				// ����� ��Ʈ���̸�.
				if( pObj->GetType() == OT_CTRL )
				{
					if( pUseCtrl->GetType() == OT_MOVER )			// �����ڰ� �����϶��� ����. ��Ʈ���� ��Ʈ�ѿ��� ������ �ټ� ����.
						bApply = TRUE;							
				}
			}

			if( bApply )
			{
				if( pObj->IsRangeObj( pTarget, fRange ) )	// this(Center)�� ���Ǿ�� pObj�� ���Ǿ �浹�ߴ���
				{
					if( pObj == pTarget && fTargetRatio == 0.0f )	// Ÿ�ٵ����� ������ 0�̸� �ڱ��ڽ�(Ÿ��)�� �������� ��������.
					{
					}
					else
					{
						if( pObj != pUseCtrl )		// �����ڴ� �˻��󿡼� ����.
						{
							if( pObj->GetType() == OT_MOVER )	// Ÿ���� ������?
							{
								if( IsValidObj( pObj ) && ((CMover *)pObj)->IsLive() )		// Ÿ���� ������ ����ִ��� �˻�.
									ApplySkill( pUseCtrl, (CCtrl*)pObj, pSkillProp, pAddSkillProp, bIgnoreProp, 0, bOnlyDmg );		// ��󿡰� ȿ���� ������.
							}
							else
							{
								// Ÿ���� ������ �ƴϳ�?
								ApplySkill( pUseCtrl, (CCtrl*)pObj, pSkillProp, pAddSkillProp, bIgnoreProp, 0, bOnlyDmg );		// ��󿡰� ȿ���� ������.
							}
						}
					}
				}
			} // bApply
		}
		END_LINKMAP
	}

#endif // WORLDSERVER		
}

//
// idParty �شܴ�� ���� ȿ��.
//
void	FLSkillSystem::ApplySkillAroundTroupe( CCtrl* pUseCtrl, u_long idParty, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp, bool bIgnoreProp )
{
#ifdef __WORLDSERVER
	if( pUseCtrl->GetType() != OT_MOVER || ((CMover *)pUseCtrl)->IsPlayer() == FALSE )
	{
		return;
	}

	CParty *pParty	= g_PartyMng.GetParty( idParty );
	if( pParty != NULL )
	{
		D3DXVECTOR3	vDist;
		float		fDist;
		float		fMaxDistSq = (float)pAddSkillProp->dwSkillRange;
		fMaxDistSq *= fMaxDistSq;
		FLOAT fTargetRatio = 0.0f;

		if( pSkillProp->dwSpellRegion == SRO_REGION )
		{
			fTargetRatio = 1.0f;
		}

		for( int i = 0; i < pParty->m_nSizeofMember; i++ )
		{
			FLWSUser* pMember	= g_xWSUserManager->GetUserByPlayerID( pParty->m_aMember[i].m_uPlayerId );
			if( IsValidObj( pMember ) && !pMember->IsDie()
				&& pUseCtrl->GetWorld() && pMember->GetWorld() && pUseCtrl->GetLayer() == pMember->GetLayer() )	// mirchang_100217 ���� �˻� ���� ����
			{
				if( pMember == pUseCtrl && fTargetRatio == 0.0f )	// Ÿ�ٵ����� ������ 0�̸� �ڱ��ڽ�(Ÿ��)�� �������� ��������.
				{
				}
				else
				{
					vDist = pMember->GetPos() - pUseCtrl->GetPos();
					fDist = D3DXVec3LengthSq( &vDist );			// ��Ƽ ����� �����ڿ��� �Ÿ�.
					if( fDist < fMaxDistSq )					// xx���ͺ��� ������
					{
						ApplySkill( pUseCtrl, pMember, pSkillProp, pAddSkillProp, bIgnoreProp );	// ��� �������� ȿ�� ����.
					}
				}
			}
		}
	}
	else
	{
		if( pSkillProp->dwExeTarget == EXT_TROUPEWITH )
		{
			ApplySkill( pUseCtrl, pUseCtrl, pSkillProp, pAddSkillProp, bIgnoreProp );	
		}
	}
#endif // WorldServer
} // ApplySkillAroundTroupe

// 
// ��, ����, ���̸� �����Ͽ� ����ü ������ ������ ������ ����.
// nDmgType : AF_???
// nApplyType : OBJTYPE_PLAYER , MONSTER
// nAttackID : ��ų�̳� ������ ID
// fWidth, fHeight, fDepth : ��, ����, ����.
// this == Attacker
void	FLSkillSystem::ApplySkillLine( CCtrl* pUseCtrl, int nApplyType, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp, float fWidth, float fHeight, float fDepth, bool bIgnoreProp, BOOL bControl )
{
#ifdef __WORLDSERVER
	int nRange	= 4;	// 4m
	CObj* pObj;
	D3DXVECTOR3 vPos = pUseCtrl->GetPos();
	D3DXVECTOR3 vDist;

	if( pSkillProp == NULL )
		FLERROR_LOG( PROGRAM_NAME, _T( "pSkillProp == NULL %d" ), pUseCtrl->GetIndex() );

	// ������ ���� Local AABB.
	D3DXVECTOR3 vMin1 = D3DXVECTOR3( -fWidth / 2.0f, -fHeight / 2.0f, -fDepth );
	D3DXVECTOR3 vMax1 = D3DXVECTOR3(  fWidth / 2.0f,  fHeight / 2.0f, 0 );

	D3DXMATRIX mInv;		// �������� ���忪���
	D3DXMATRIX m1, m2, m3;
	D3DXMatrixRotationY( &m1, D3DXToRadian( -(pUseCtrl->GetAngle()) ) );
	D3DXMatrixTranslation( &m2, pUseCtrl->GetPos().x, pUseCtrl->GetPos().y, pUseCtrl->GetPos().z );
	D3DXMatrixMultiply( &m3, &m1, &m2 );	// world = rot * trans

	D3DXMatrixInverse( &mInv, NULL, &m3 );		// �������� ����� ����.

	// Ÿ���� ��ǥ�� �������� �������� ����ȯ �Ѱ�.
	D3DXVECTOR3 vDestLocal;

	// �Ϲ������� fDepth�� ���� ��⶧���� �˻� ������ fDepth�� �ߴ�. 
	if( fDepth <= 0 )	// ������ 0�̰ų� �����ϼ��� ����.
		FLERROR_LOG( PROGRAM_NAME, _T( "%d %d" ), pUseCtrl->GetIndex(), fDepth );

	if( fDepth <= 4.0f )		nRange = 4;
	else if( fDepth <= 8.0f )	nRange = 8;
	else if( fDepth <= 16.0f )	nRange = 16;
	else						nRange = 32;

	BOOL	bApply = FALSE;
	BOOL	bTarget = FALSE;
	if( nApplyType & OBJTYPE_PLAYER )	// �������� �÷��̾��ΰ� 
	{
		FOR_LINKMAP( pUseCtrl->GetWorld(), vPos, pObj, nRange, CObj::linkPlayer, pUseCtrl->GetLayer() )
		{
			bApply = FALSE;	// ������ FALSE

			if( pObj->GetType() != OT_MOVER )
				FLERROR_LOG( PROGRAM_NAME, _T( "pObj�� MOVER�� �ƴϴ� %d" ), pObj->GetType() );

			if( pUseCtrl->GetType() == OT_MOVER )		// �����ڰ� ������.
			{
				CMover *pAttacker = (CMover *)pUseCtrl;
				if( pAttacker->IsPlayer() )			// �����ڰ� �÷��̾��
				{
					bApply = TRUE;

					CMover *pDefender = (CMover *)pObj;
					if( pDefender->IsPlayer() )
					{
						if( bControl == FALSE && pAttacker->GetHitType( pDefender, bTarget, FALSE ) == HITTYPE_PK )
						{
							bApply = FALSE;
						}
						else if( pAttacker->m_nDuel == 1 && pDefender->m_nDuel == 1 && pAttacker->GetHitType( pDefender, bTarget, FALSE ) != HITTYPE_PVP )
						{
							bApply = FALSE;
						}
					}
				}
				else
				{
					// �����ڰ� ���͸�
					bApply = TRUE;		// 
				}
			}
			else
			{
				// �����ڰ� ������ �ƴϴ�.
				bApply = TRUE;
			}

			if( bApply && pObj != pUseCtrl )		// this�� ���ݴ�󿡼� ����.
			{
				CMover *pTarget = (CMover *)pObj;
				if( IsValidObj( pTarget ) && pTarget->IsLive() )
				{
					// Ÿ���� ��ǥ�� ����ȯ.
					D3DXVec3TransformCoord( &vDestLocal, &pTarget->GetPos(), &mInv );
					// Ÿ���� AABB
					D3DXVECTOR3 vMin2 = vDestLocal + pTarget->m_pModel->m_vMin;
					D3DXVECTOR3 vMax2 = vDestLocal + pTarget->m_pModel->m_vMax;
					if( ::IsTouchAABB( vMin1, vMax1, vMin2, vMax2 ) )		// AABB�浹�˻�.
					{
						ApplySkill( pUseCtrl, pTarget, pSkillProp, pAddSkillProp, bIgnoreProp, 0, FALSE, bTarget );		// bTarget Ÿ���� �����Ƿ� FALSE�� ����. PK�� ������ ĥ������
					}
				}
			} // bApply
		}
		END_LINKMAP
	}

	// �������� �����ΰ�.
	if( nApplyType & (OBJTYPE_MONSTER | OBJTYPE_CTRL) )
	{
		FOR_LINKMAP( pUseCtrl->GetWorld(), vPos, pObj, nRange, CObj::linkDynamic, pUseCtrl->GetLayer() )
		{
			bApply = FALSE;
			if( pObj->GetType() == OT_MOVER )				// ����� ������.
			{
				CMover *pTarget = (CMover *)pObj;
				if( pTarget->IsPeaceful() == FALSE )		// NPC�� �ƴѰ�츸 ����
					bApply = TRUE;

				//sun: 8, 8.5�� ��� ������ų ����ȿ�� �Ұ��� ���� World
				CMover *pAttacker = (CMover *)pUseCtrl;
				if( pAttacker->IsPlayer() && pAttacker->IsChaotic() == FALSE && pTarget->GetProp()->dwClass == RANK_GUARD )
					bApply = FALSE;

			}
			else
			{
				// ����� ��Ʈ���̸�
				if( pObj->GetType() == OT_CTRL )
				{
					if( pUseCtrl->GetType() == OT_MOVER )			// �����ڰ� �����϶��� ����. ��Ʈ���� ��Ʈ�ѿ��� ������ �ټ� ����.
						bApply = TRUE;							
				}
			}


			if( bApply )
			{
				if( pObj->GetType() == OT_MOVER )
				{
					CMover *pTarget = (CMover *)pObj;

					if( IsValidObj( pTarget ) && pTarget->IsLive() )
					{
						// Ÿ���� ��ǥ�� ����ȯ.
						D3DXVec3TransformCoord( &vDestLocal, &pTarget->GetPos(), &mInv );
						// Ÿ���� AABB
						D3DXVECTOR3 vMin2 = vDestLocal + pTarget->m_pModel->m_vMin;
						D3DXVECTOR3 vMax2 = vDestLocal + pTarget->m_pModel->m_vMax;
						if( ::IsTouchAABB( vMin1, vMax1, vMin2, vMax2 ) )		// AABB�浹�˻�.
						{
							ApplySkill( pUseCtrl, (CCtrl*)pObj, pSkillProp, pAddSkillProp, bIgnoreProp );		// ��󿡰� ȿ���� ������.
						}
					}
				}
				else
				{
					ApplySkill( pUseCtrl, (CCtrl*)pObj, pSkillProp, pAddSkillProp, bIgnoreProp );		// ��󿡰� ȿ���� ������.
				}
			}
		}
		END_LINKMAP
	}

#endif // WORLDSERVER		
}

// vPos�� �߽����� �ֺ��� ȿ���� �����Ѵ�.
// ------------------------------------------
// Ʈ���� ������ �ְ�. ���Ͱ��������ִ�(��), ���ΰ��� ��ų�� ����Ҽ��� �ִ�.
// ȿ���� �ݵ�� SkillProp�� ���� �� ���̵� �Ѱܾ� �Ѵ�.
//
// ��쿡���� pAddSkillProp�� NULL�ϼ��� �ִ�.  �����ۿ��� ����Ҽ��� �ֱ⶧��. �������� �ֵ彺ų������ ����.
// pCenter : ���� ���ӽ�ų�� Ÿ��. NULL�ϼ��� �ִ�
void	FLSkillSystem::ApplySkillRegion( CCtrl* pUseCtrl, const D3DXVECTOR3 &vPos, int nApplyType, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp, bool bIgnoreProp, BOOL bOnlyDmg, CCtrl* pCenter, BOOL bControl )
{
#ifdef __WORLDSERVER
	int nRange	= 4;	// 4m
	CObj* pObj;
	CCtrl *pSrc = pUseCtrl;
	D3DXVECTOR3 vDist;

	if( pSkillProp == NULL )
		FLERROR_LOG( PROGRAM_NAME, _T( "pSkillProp == NULL A:%d-%d" ), pSrc->GetType(), pSrc->GetIndex() );

	FLOAT fRange = 4.0f;
	if( pAddSkillProp )										// �ֵ彺ų�� ������ �ű⼭ ����
	{
		fRange = (float)pAddSkillProp->dwSkillRange;
		if( pAddSkillProp->dwSkillRange == NULL_ID )
			FLERROR_LOG( PROGRAM_NAME, _T( "��ų %d�� dwSkillRange�� �������� ����" ), pAddSkillProp->dwID );
	}
	else
	{
		fRange = (float)pSkillProp->dwSkillRange;			// ������ ��ų������Ƽ���� ����.
		if( pSkillProp->dwSkillRange == NULL_ID )
			FLERROR_LOG( PROGRAM_NAME, _T( "��ų %d�� dwSkillRange�� �������� ����" ), pSkillProp->dwID );
	}

	if( fRange <= 4.0f )		nRange = 4;
	else if( fRange <= 8.0f )	nRange = 8;
	else if( fRange <= 16.0f )	nRange = 16;
	else						nRange = 32;

	if( fRange <= 0 )	// ������ 0�̰ų� �����ϼ��� ����.
		FLERROR_LOG( PROGRAM_NAME, _T( "D:%d A:%d-%d %d %f" ), pUseCtrl->GetIndex(), pSrc->GetType(), pSrc->GetIndex(), pSkillProp->dwID, fRange );

	BOOL	bApply = FALSE;
	BOOL	bTarget = TRUE;

	//------------ �������� �÷��̾��ΰ� 
	if( nApplyType & OBJTYPE_PLAYER )	
	{
		FOR_LINKMAP( pUseCtrl->GetWorld(), vPos, pObj, nRange, CObj::linkPlayer, pUseCtrl->GetLayer() )
		{
			bApply = FALSE;	// �ϴ� FALSE�� �ʱ�ȭ

			if( pObj->GetType() != OT_MOVER )
				FLERROR_LOG( PROGRAM_NAME, _T( "pObj�� MOVER�� �ƴϴ� %d" ), pObj->GetType() );

			if( pSrc->GetType() == OT_MOVER )		// �����ڰ� ������.
			{
				CMover *pAttacker = (CMover *)pSrc;
				if( pAttacker->IsPlayer() )			// �����ڰ� �÷��̾��
				{
					bApply = TRUE;
					//sun: 8, // __S8_PK
					CMover *pDefender = (CMover *)pObj;
					if( pDefender->IsPlayer() )
					{
						if( pCenter )
							bTarget = (pCenter == pDefender);
						else
							bTarget = FALSE;

						if( bControl == FALSE && pAttacker->GetHitType( pDefender, bTarget, FALSE ) == HITTYPE_PK )
							bApply = FALSE;
						else if( pAttacker->m_nDuel == 1 && pDefender->m_nDuel == 1 && pAttacker->GetHitType( pDefender, bTarget, FALSE ) != HITTYPE_PVP )
							bApply = FALSE;
					}

				}
				else
				{	// �����ڰ� ���͸�
					bApply = TRUE;		// 
				}
			}
			else
			{	// �����ڰ� ������ �ƴϴ�.
				bApply = TRUE;
			}

			if( bApply )
			{
				if( pObj != pSrc )		// ����Ŀ�� �˻���󿡼� ����.
				{
					if( pObj->IsRangeObj( vPos, fRange ) )	// vCenter + fRange�� pObj�� ���Ǿ �浹�ߴ���
					{
						CMover *pTarget = (CMover *)pObj;		// <- ���� ������ pObj�� �÷��̾�Ƿ� �ص���.

						if( pCenter )
							bTarget = (pCenter == pTarget);
						else
							bTarget = FALSE;

						if( IsValidObj( pTarget ) && pTarget->IsLive() )
						{
							ApplySkill( pSrc, pTarget, pSkillProp, pAddSkillProp, bIgnoreProp, 0, bOnlyDmg, bTarget );		// ��󿡰� ȿ���� ������.
						}
					}
				}

				bApply = FALSE;	// ���� ������ ���ؼ� �ʱ�ȭ.
			} // bApply
		}
		END_LINKMAP
	} // OBJTYPE_PLAYER

	// �������� ����/��Ʈ�� �ΰ�.
	if( nApplyType & (OBJTYPE_MONSTER | OBJTYPE_CTRL) )
	{
		FOR_LINKMAP( pUseCtrl->GetWorld(), vPos, pObj, nRange, CObj::linkDynamic, pUseCtrl->GetLayer() )	// linkDynamic�� ����
		{
			bApply = FALSE;
			if( pObj->GetType() == OT_MOVER )				// ����� ������.
			{
				CMover *pTarget = (CMover *)pObj;
				if( pTarget->IsPeaceful() == FALSE )		// NPC�� �ƴѰ�츸 ����
					bApply = TRUE;
				//sun: 8, 8.5�� ��� ������ų ����ȿ�� �Ұ��� ���� World
				CMover *pAttacker = (CMover *)pSrc;
				if( pAttacker->IsPlayer() && pAttacker->IsChaotic() == FALSE && pTarget->GetProp()->dwClass == RANK_GUARD )
					bApply = FALSE;

			}
			else
			{
				// ����� ��Ʈ���̸�.
				if( pObj->GetType() == OT_CTRL )
				{
					if( pSrc->GetType() == OT_MOVER )			// �����ڰ� �����϶��� ����. ��Ʈ���� ��Ʈ�ѿ��� ������ �ټ� ����.
						bApply = TRUE;							
				}
			}

			if( bApply )
			{
				if( pObj != pSrc )		// �����ڴ� �˻��󿡼� ����.
				{
					if( pObj->IsRangeObj( vPos, fRange ) )	// vCenter + fRange�� pObj�� ���Ǿ �浹�ߴ���
					{
						if( pObj->GetType() == OT_MOVER )	// Ÿ���� ������?
						{
							if( IsValidObj( pObj ) && ((CMover *)pObj)->IsLive() )		// Ÿ���� ������ ����ִ��� �˻�.
								ApplySkill( pSrc, (CCtrl*)pObj, pSkillProp, pAddSkillProp, bIgnoreProp, 0, bOnlyDmg );		// ��󿡰� ȿ���� ������.
						}
						else
						{
							// Ÿ���� ������ �ƴϳ�?
							ApplySkill( pSrc, (CCtrl*)pObj, pSkillProp, pAddSkillProp, bIgnoreProp, 0, bOnlyDmg );		// ��󿡰� ȿ���� ������.
						}
					}
				}
			} // bApply
		}
		END_LINKMAP
	}

#endif // WORLDSERVER		
} // ApplySkillRegion()


// pSkillProp�� this�� �����Ѵ�.  �������� �ɼ��� �ְ�. ���º�ȭ�� �ɼ����ִ�.
// pUseCtrl�� ������(Ȥ�� ����Ŀ)
// pAddSkillProp�� NULL�ϼ��� ������ ����.
void	FLSkillSystem::ApplySkill( CCtrl* pUseCtrl, CCtrl* pTarget, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp, bool bIgnoreProp, int nParam, BOOL bOnlyDmg, BOOL bTarget )
{
	if( pUseCtrl->GetType() == OT_MOVER && pTarget->GetType() == OT_MOVER )
	{
		if( ((CMover *)pUseCtrl)->IsPlayer() && ((CMover *)pTarget)->IsPlayer() && pSkillProp->dwSkillType != NULL_ID )
		{
			DWORD dwPKRegionAttr = ((CMover *)pTarget)->GetPKPVPRegionAttr();
			int nAttackerPK = ((CMover *)pUseCtrl)->IsPKPVPInspectionBase( dwPKRegionAttr, FALSE );
			int nDefenDerPK = ((CMover *)pTarget)->IsPKPVPInspectionBase( dwPKRegionAttr, FALSE );
			if( nAttackerPK == 1 || nDefenDerPK == 1 )
			{
				if( pSkillProp->IsDebuff() == TRUE )
				{
					CMover* pAttacker = (CMover*)pUseCtrl;
					CMover* pDefender = (CMover*)pTarget;
					if( !(pAttacker->IsPVPTarget( pDefender ) || pAttacker->IsWarTarget( pDefender ) ||
						pAttacker->IsGuildCombatTarget( pDefender ) || pAttacker->IsArenaTarget( pDefender ) || pAttacker->IsEventArenaTarget( pDefender ))
						)
					{
						return;
					}

					//if( !((CMover *)pSrc)->IsPVPTarget((CMover *)this) )
					//	return;
				}
			}
		}
	}

	// �������Ӽ��� ������� �װ��� ó��.
	int nDamage = ApplyDamage( pUseCtrl, pTarget, pSkillProp, pAddSkillProp, nParam, bTarget );
	if( bOnlyDmg == TRUE )		
	{
		return;
	}

	if( CanApplyBuff( static_cast<CMover*>( pUseCtrl ), static_cast<CMover*>( pTarget ), pSkillProp ) == false )
	{
		return;
	}

#if defined(__WORLDSERVER)
	if( pTarget != pUseCtrl && pUseCtrl->GetType() == OT_MOVER && pTarget->GetType() == OT_MOVER )		// ���� ����� �� Ÿ���̾ƴϸ� skip
	{
		if( ((CMover *)pTarget)->IsDie() == FALSE )
		{
			if( pSkillProp->IsDebuff() == TRUE )													// ���۸�����
			{
				if( ((CMover *)pUseCtrl)->GetHitType2(((CMover *)pTarget), bTarget, FALSE ) == HITTYPE_FAIL )  	// ��ȭ���λ�뿡��
					return;																			// ����ȵ�
			}

			CWorld* pWorld	= pUseCtrl->GetWorld();
			if(  pWorld && pWorld->IsArena() == FALSE )		// �Ʒ����� �ƴϸ�,
			{
				if( pSkillProp->IsDebuff() == FALSE )													// ���� ������
				{
					if( ((CMover *)pUseCtrl)->GetHitType2(((CMover *)pTarget), bTarget, TRUE ) != HITTYPE_FAIL )  	// �������λ�뿡��
						return;																			// ����ȵ�
				}
			}
		}
		else
		{
			if( pSkillProp->dwExeTarget != EXT_MAGICATKSHOT )
			{
				if( pSkillProp->dwSfxObj2 != NULL_ID )
					g_xWSUserManager->AddCreateSfxObj( pTarget, pSkillProp->dwSfxObj2 );
			}

			if( pSkillProp->IsDebuff() == TRUE )
				return;
		}
	}
#endif //defined(__WORLDSERVER)

	int nProb	= pSkillProp->nProbability;
	if( pAddSkillProp )
	{
		nProb	= pAddSkillProp->dwProbability;
		//sun: 9, ���ο� AddSkillProp �÷� �߰� �� ����
		BOOL bPVP	= ( pUseCtrl->GetType() == OT_MOVER && ( (CMover *)pUseCtrl )->IsPlayer() && pTarget->GetType() == OT_MOVER && ( (CMover *)pTarget )->IsPlayer() );
		if( bPVP )
			nProb	= pAddSkillProp->dwProbabilityPVP;
	}

	if( bIgnoreProp )	// Ȯ�����ÿ��� ������ ������ 100%
		nProb = NULL_ID;

	if( nProb == NULL_ID || (int)( xRandom( 100 ) ) < nProb  )		// ���� Ȯ��, =�� �Ǿ� ������ 100% ����
	{
		if( pTarget->GetType() == OT_MOVER )		// Ÿ���� �����϶��� ���ӽð� ó��
		{
			DWORD dwSkillTime = 0;
			DWORD dwPainTime = 0;
			if( pAddSkillProp )
			{
				dwSkillTime = pAddSkillProp->dwSkillTime;
				dwPainTime = pAddSkillProp->dwPainTime;
			}
			else
			{
				dwSkillTime = pSkillProp->dwSkillTime;
				dwPainTime = pSkillProp->tmContinuousPain;
			}

			BOOL bNew = TRUE;	// ��ų�� ÷ ����ƴ°�.
			//sun: ��, ���� ������ ��۹��� ����... Neuz, World
			if( dwSkillTime != NULL_ID )	// ���ӽð��� �ִ� ��ų(�����ؼ� ��)
			{
				DWORD dwTime1 = 0, dwTime2 = 0;
				if( pSkillProp->dwReferTarget1 == RT_TIME || pSkillProp->dwReferTarget2 == RT_TIME )
				{
					if( pUseCtrl->GetType() == OT_MOVER )		// �����ڰ� �����϶���.
						((CMover *)pUseCtrl)->SubReferTime( &dwTime1, &dwTime2, pSkillProp, pAddSkillProp );
				}

				WORD wType;
				if( pSkillProp->dwSkillType == NULL_ID )	
					wType = BUFF_ITEM;
				else										
					wType = BUFF_SKILL;

				// pTarget�� this�� �ɼ����ְ� Ÿ���� �ɼ��� �ִ�.
#if defined(__WORLDSERVER)
				int nLevel = 0;
				if( pAddSkillProp )		
					nLevel = pAddSkillProp->dwSkillLvl;
				else					
					nLevel = 0;

				// dwSkillTime�� ��ų��ü�� ���ӽð����ε� ���̰� �ΰ�ȿ���� ���ӽð����ε� ���̱⶧���� �̷������� ���� �ʴ�
				// �̴�ζ�� �����ʵ嵵 ����ó�� �ɷ�������.
				if( pSkillProp->dwID == SKILL_INDEX( 170, SI_ELE_FIRE_BURINGFIELD ) )
				{
					bNew = FALSE;	//  �����ʵ�� ����ó��
				}
				else
				{
					CMover * pThisMover = static_cast<CMover*>( pTarget );
					CMover * pSrcMover = static_cast< CMover * >( pUseCtrl );

					//#if defined __WORLDSERVER && defined __AGGRO16
					//					const BOOL bCanNotUseBuffInAttackMode	= ( pSrcMover->IsAttackMode() == TRUE && ( wType == BUFF_SKILL && pSkillProp->dwID == SKILL_INDEX( 193, SI_ACR_SUP_DARKILLUSION ) ) );		//������ ���� ���� ���� �����. �ϵ��ڵ��ض�.
					//					if( bCanNotUseBuffInAttackMode == FALSE )
					//					{
					//#endif		//__AGGRO16
					bNew	= pThisMover->AddBuff( wType, (WORD)( pSkillProp->dwID ), nLevel, dwSkillTime + (int)dwTime1 + (int)dwTime2, dwPainTime, pSkillProp->byBuffTickType, pUseCtrl->GetId() );
#if defined __WORLDSERVER && defined __AGGRO16
					if( bNew == TRUE )
					{
						if(  pSkillProp->IsDebuff() == TRUE )	//���� ��ų
						{
							if( pThisMover->GetAggroMng() != NULL )
								pThisMover->GetAggroMng()->SumDebuffAggro( *pSrcMover, *pThisMover );
						}
						else
						{
							FLAggro::SumBuffAggro( *pSrcMover, *pThisMover );
						}
					}
					//}
					//else if( pSrcMover->IsPlayer() == TRUE )
					//{
					//	static_cast< FLWSUser * >( pSrcMover )->AddDefinedText( TID_MMI_GHOUSE_UNSKILL );
					//	bNew					= FALSE;
					//}

#endif //__WORLDSERVER __AGGRO16
				}


#elif defined(__CLIENT)
				bNew = ((CMover *)pTarget)->HasBuff( wType, (WORD)( pSkillProp->dwID ) ) ? FALSE : TRUE;
#endif // Client
			}

			// ���� ����� ��ų����
			if( bNew )	// �̹� �ɷ��ִ� ���¿��� �� ���� bNew�� FALSE�� �ȴ�.
			{
#ifdef __WORLDSERVER
				// ��ų�� ����ó�� �ؾ��Ұ��� ó����.
				if( ApplySkillHardCoding( pUseCtrl, pTarget, pSkillProp, pAddSkillProp ) == FALSE )
					return;

				// ��Ȱ��ų������ �Ǿ��ִٸ� �Ʒ��͵� ����
				if( ((CMover *)pTarget)->m_Resurrection_Data.bUseing )
					return;

				// �������� �ָ鼭 �Ķ���͵� ���ϴ� ��ų�� �ֱⶫ�� �������� ���� ó��.
				switch( pSkillProp->dwExeTarget )
				{
				case EXT_SELFCHGPARAMET:		// ������ �ڽ�.
					if( pUseCtrl->GetType() == OT_MOVER )	// Ÿ��(���⼱ Ÿ���� ������)�� �����϶���...
						ApplyParam( (CMover *)pUseCtrl, pUseCtrl, pSkillProp, pAddSkillProp, TRUE, nDamage );
					break;

				case EXT_OBJCHGPARAMET:
				case EXT_MAGIC:
				case EXT_ANOTHER:
				case EXT_ANOTHERWITH:
				default:
					if( pTarget->GetType() == OT_MOVER )	// Ÿ���� �����϶���.
						ApplyParam( (CMover *)pUseCtrl, pTarget, pSkillProp, pAddSkillProp, TRUE , nDamage );
					break;
				}

				if( pTarget->GetType() == OT_MOVER )
				{
					// �и��� ���� �ִ°�?
					CMover *pTargetMover = (CMover *)pTarget;
					DWORD dwDmgShift;
					if( pAddSkillProp )
						dwDmgShift = pAddSkillProp->dwDmgShift;
					else
						dwDmgShift = pSkillProp->dwDmgShift;

					if( dwDmgShift != NULL_ID && pTargetMover->GetProp()->dwClass != RANK_SUPER && pTargetMover->GetProp()->dwClass != RANK_MIDBOSS )
					{
						pTargetMover->pushedByAnotherPower( GetDegree( pTargetMover->GetPos(), pUseCtrl->GetPos() ), 0.85f );
					}

					// ������ �������
					if( pSkillProp->dwID == SKILL_INDEX( 206, SI_ACR_YOYO_PULLING )	|| pSkillProp->dwID == SKILL_INDEX( 400, SI_LOD_SUP_PULLING ) )
					{
						if( pAddSkillProp && pAddSkillProp->arrAddSkillDestNumData1[2] != NULL_ID )
						{
							// ����� �Ÿ�
							FLOAT fPullingLen = (FLOAT)pAddSkillProp->arrAddSkillDestNumData1[2] * 0.001f; 

							// Ÿ�ٰ��� �Ÿ�
							D3DXVECTOR3 v3Len = pTargetMover->GetPos() - pUseCtrl->GetPos();
							FLOAT fTotalLen = D3DXVec3LengthSq( &v3Len ) * 0.1f;
							{
								if( fTotalLen > pTargetMover->GetAttackRange( AR_HRANGE ) )
									fTotalLen = pTargetMover->GetAttackRange( AR_HRANGE );

								// �Ÿ��� ���� ������� ���� �ٸ��� �����
								FLOAT fDest = fTotalLen / pTargetMover->GetAttackRange( AR_HRANGE );
								{
									pTargetMover->pushedByAnotherPower( GetDegree( pTargetMover->GetPos(), pUseCtrl->GetPos() ), fPullingLen * -fDest );
								}
							}
						}
					}
				}
#endif // __WORLDSERVER
			} // if( bNew )

#ifdef __WORLDSERVER
			if( pSkillProp->dwID == SKILL_INDEX( 187, SI_GEN_FLASH ) && pTarget->GetType() == OT_MOVER && ((CMover *)pTarget)->IsPlayer() )
			{
				((FLWSUser *)pTarget)->AddCommonSkill( pSkillProp->dwID, pAddSkillProp->dwSkillLvl );
			}
#endif // __WORLDSERVER

		}	// this == OT_MOVER
	}	// if( nProb == NULL_ID || xRandom(100) < nProb  )
#ifdef __WORLDSERVER
	//sun, 11, Ȯ����ų ȿ������ world,neuz
	else
	{
		if(pSkillProp->dwID == SKILL_INDEX( 242, SI_PSY_HERO_STONE ) )
			g_xWSUserManager->AddRemoveSfxObj( pTarget, pSkillProp->dwSfxObj4 );
	}
#endif	//__WORLDSERVER

#ifdef __WORLDSERVER
	switch( pSkillProp->dwSpellRegion )
	{
	case SRO_AROUND:
	case SRO_REGION:	// ���������϶� ���⼭ Ÿ�ٵ� ������ sfxobj3�� �ѷ���.
		if( pSkillProp->dwExeTarget != EXT_MAGICATKSHOT )
			if( pSkillProp->dwSfxObj3 != NULL_ID )
				g_xWSUserManager->AddCreateSfxObj( pTarget, pSkillProp->dwSfxObj3 );
		break;
	}

	// ����ó��: ��Ż��ų - ��ĸ� ��ġ�鼭 �������� �ش�
	if( pSkillProp->dwID == SKILL_INDEX( 203, SI_ACR_YOYO_SNITCH ) )
	{
		// pSrc - ������ �÷��̾�, this - �ǰ����� ���� 
		// �ǰ����� �ܾ��� 0�� �ƴϸ� ��Ż �Ѵ�.
		FLWSUser* pUser = (FLWSUser*)pUseCtrl;
		FLWSUser* pDest = (FLWSUser*)pTarget;
		if( pDest->GetType() == OT_MOVER && pDest->GetGold() > 0 )
		{
			int nPlus = (pDest->GetLevel()*20) - (pUser->GetLevel()*10);

			if( pUser->GetLevel() <= 20 )
			{
			}
			else
			{
				if( pUser->GetLevel() <= 30 )
				{
					nPlus	= (int)( nPlus * 0.7f );
				}
				else
				{
					if( pUser->GetLevel() <= 40 )
					{
						nPlus	= (int)( nPlus * 0.5f );
					}
					else
					{
						nPlus	= (int)( nPlus * 0.3f );
					}
				}
			}

			if( nPlus > 0 )
			{				
				nPlus = min( nPlus, pDest->GetGold() );		// �ǰ����� �� ���ٴ� ���� �ʰ� 
				pUser->AddGold( nPlus );
				pDest->AddGold( -nPlus );

				pUser->AddGoldText( nPlus );
			}
		}		
	}
#endif	//__WORLDSERVER
}


// ApplySkill ���ο��� ������ �ִºκи� ����.
int		FLSkillSystem::ApplyDamage( CCtrl* pUseCtrl, CCtrl* pTarget, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp, int nParam, BOOL bTarget ) 
{
	int nDamage = 0;
	// �������� ��ų�� ������ ����.
	switch( pSkillProp->dwExeTarget )
	{
	case EXT_MELEEATK:		// ������ų����������
	case EXT_MAGICATK:		// ��ø�������������
	case EXT_MAGICATKSHOT:
		{
			if( pSkillProp->dwID == SKILL_INDEX( 65, SI_MAG_FIRE_HOTAIR ) )	// �ֿ��� ����ó��.
			{
				nDamage = pTarget->SendDamage( AF_MAGICSKILL, pUseCtrl->GetId(), nParam, bTarget );
			}
			else
			{
				int nAtkFlag = 0;
				if( pSkillProp->dwSkillType == KT_MAGIC )
					nAtkFlag = AF_MAGICSKILL;
				else
					nAtkFlag = AF_MELEESKILL;

				if( pSkillProp->dwComboStyle == CT_FINISH )
					nDamage = pTarget->SendDamageForce( nAtkFlag, pUseCtrl->GetId(), (pSkillProp->dwID << 16) | pAddSkillProp->dwSkillLvl, bTarget );
				else
					nDamage = pTarget->SendDamage( nAtkFlag, pUseCtrl->GetId(), (pSkillProp->dwID << 16) | pAddSkillProp->dwSkillLvl, bTarget );
			}
		}
		break;
	}

	return nDamage;
}

//
// ApplySkill ���ο��� ��ų�� �ϵ��ڵ� ó����.
// this : Target
// pSrc : ������.
//
BOOL	FLSkillSystem::ApplySkillHardCoding( CCtrl* pUseCtrl, CCtrl* pTarget, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp )
{
#ifdef __WORLDSERVER
	switch( pSkillProp->dwID )
	{
	case SKILL_INDEX( 45, SI_ASS_HEAL_RESURRECTION ):		// ��Ȱ	
		if( pTarget->GetType() == OT_MOVER )
		{
			CMover *pTargetMover = (CMover *)pTarget;
			if( pTargetMover->IsPlayer() && pTargetMover->IsDie() )	// �÷��̾��̰� Ÿ���� �׾�������
			{
				if( pTargetMover->m_pActMover->GetDmgState() == OBJSTA_RESURRECTION )
				{
					return FALSE;		// �̹� ��Ȱ���� ������� ���Ŷ�� ���.
				}
				else
				{
					g_dpDBClient.SendLogLevelUp( pTargetMover, 10 );	// ��Ȱ ��ų �α�

					if( ((FLWSUser *)pTarget)->m_Resurrection_Data.bUseing != TRUE )
					{
						((FLWSUser *)pTarget)->AddRevivalBySkillMessage();

						((FLWSUser *)pTarget)->m_Resurrection_Data.bUseing        = TRUE;
						((FLWSUser *)pTarget)->m_Resurrection_Data.dwPlayerID     = ((CMover*)pUseCtrl)->m_idPlayer;
						((FLWSUser *)pTarget)->m_Resurrection_Data.pSkillProp     = pSkillProp;
						((FLWSUser *)pTarget)->m_Resurrection_Data.pAddSkillProp  = pAddSkillProp;
					}
				}
			}
		}
		break;
	}
#endif // WorldServer
	return TRUE;
}

//
// this���� �Ķ���͸� �����Ѵ�.
// SetDestParam�� �ϰ������� �ϴ� �κ�.
// pSrc�� ������.
BOOL	FLSkillSystem::ApplyParam( CCtrl* pUseCtrl, CCtrl* pTarget, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp, BOOL bSend, int nDamage )
{
	if( pUseCtrl == NULL )		return FALSE;

	int		i;
	CMover *pTargetMover = (CMover *)pTarget;

	DWORD	dwDestParam, dwSkillTime, dwPainTime;
	int		nAdjParam, nChgParam;
	int		nDestData[3];

	for( i = 0; i < MAX_SKILL_PARAM; i ++ )
	{
		if( DoActiveParam( pUseCtrl, pTarget, pSkillProp, pAddSkillProp, i, bSend, nDamage ) == TRUE )
		{
			continue;
		}

		if( pAddSkillProp )
		{
			dwDestParam = pAddSkillProp->dwDestParam[i];
			dwSkillTime = pAddSkillProp->dwSkillTime;
			dwPainTime = pAddSkillProp->dwPainTime;

			nAdjParam	= pAddSkillProp->dwAdjParamVal[i];
			nChgParam	= (int)pAddSkillProp->dwChgParamVal[i];
			nDestData[0] = pAddSkillProp->arrAddSkillDestNumData1[0];
			nDestData[1] = pAddSkillProp->arrAddSkillDestNumData1[1];
			nDestData[2] = pAddSkillProp->arrAddSkillDestNumData1[2];
		}
		else
		{	
			dwDestParam = pSkillProp->dwDestParam[i];
			dwSkillTime = pSkillProp->dwSkillTime;
			dwPainTime  = pSkillProp->tmContinuousPain;
			nAdjParam	= pSkillProp->nAdjParamVal[i];
			nChgParam	= (int)pSkillProp->dwChgParamVal[i];
			nDestData[0] = pSkillProp->nDestData1[0];
			nDestData[1] = pSkillProp->nDestData1[1];
			nDestData[2] = pSkillProp->nDestData1[2];
		}
		// destParam 1
		switch( dwDestParam )
		{
		case NULL_ID:
			break;
		case DST_CHRSTATE:		// ���� ����
			if( nAdjParam & CHS_INVISIBILITY )	// ��ũ�Ϸ�ǻ��·�...
			{
				if( pTargetMover->SetDarkCover( TRUE, dwSkillTime ) == FALSE )
					return FALSE;
				pTargetMover->m_wDarkCover = 0;		
			}
			else if( nAdjParam & CHS_STUN )	// ���ϻ��·�...
			{
				if( pTargetMover->SetStun( TRUE, dwSkillTime ) == FALSE )
					return FALSE;

				// �ϴ� ����. 
				// ������ �ɸ��°� �ΰ��� ��Ʈ�� �ִµ�
				// 1. ��ų�� �¾�����.
				// 2. ���Ͼ����ۿ� �¾�����
				// 3. ��Ÿ ��Ȳ(Ʈ������..)
				// ��ų�� ����ؼ� ������ �ɸ��� SkillInflu���� ó���ؼ�
				// �������� �����ֱⶫ�� m_wStunCnt�� ���� ���� �α����� ������ ��Ű�� �ȴ�.
				// �����ۿ� ���� ������ �ɸ��� SkillInflu���� ó���� ���ϹǷ� m_wStunCnt�� �ʿ��ϴ�.
				// �̰��� ��ų�� ���ؼ� ������ ���̱� ����.
				// ������ �߸�����ƴ�.  ��ų�߽��� �ƴϰ� �����߽����� ó���� �ƾ�� �ߴ�.
				pTargetMover->m_wStunCnt = 0;		
			}
			else if( nAdjParam & CHS_POISON )	// �����·�..
			{
				if( pTargetMover->SetPoison( TRUE, pUseCtrl->GetId(), dwSkillTime, dwPainTime, static_cast< short >( nDestData[0] ) ) == FALSE )	// nDestData[0] ƽ�� ���� ������
					return FALSE;
				pTargetMover->m_wPoisonCnt = 0;		// �����̶� ���� ����
			}
			else if( nAdjParam & CHS_DARK )	// �������
			{
				if( pTargetMover->SetDark( TRUE, dwSkillTime, nDestData[1] ) == FALSE )		// nDestData1[1] : ���߷� ����ġ.
					return FALSE;
				pTargetMover->m_wDarkCnt = 0;		// �����̶� ���� ����
			}
			else if( nAdjParam & CHS_BLEEDING )	// �������·�..
			{
				if( nDestData[0] < 0 )
					nDestData[0]	= nDamage / 10;
				if( pTargetMover->SetBleeding( TRUE, pUseCtrl->GetId(), dwSkillTime, dwPainTime, static_cast< short >( nDestData[0] ) ) == FALSE )
					return FALSE;
				pTargetMover->m_wBleedingCnt = 0;		// �����̶� ���� ����
			}
			else if( nAdjParam & CHS_SLEEPING )	// �����λ��·�...
			{
				if( pTargetMover->SetSleeping( true, dwSkillTime ) == false )
				{
					return FALSE;
				}
				pTargetMover->SetSleepingDuration( 0 );
			}
			else
			{
				pTargetMover->SetDestParam( dwDestParam, nAdjParam, NULL_CHGPARAM, TRUE );
			}
			break;

		case DST_CURECHR:		// ���� ����
			pTargetMover->RemoveDebuff( nAdjParam );
			break;

		case DST_CLEARBUFF:		// ����� ������ �ı�.
			if( nAdjParam == NULL_ID )
			{
				pTargetMover->RemoveBuffAll();
			}
			else
			{
				for( int i = 0; i < nAdjParam; i ++ )
					pTargetMover->RemoveBuffOne();
			}
			break;
		case DST_CHR_CHANCESTUN:	// adj:Ȯ��		data1:��ų
		case DST_CHR_CHANCEPOISON:	// adj:Ȯ��		data1:��ų
		case DST_CHR_CHANCEDARK:	// adj:Ȯ��		data1:��ų
		case DST_CHR_CHANCEBLEEDING:// adj:Ȯ��		data1:��ų
		case DST_CHR_CHANCESTEALHP:	// adj:Ȯ��		data1:��ų
			pTargetMover->SetDestParam( dwDestParam, nAdjParam, nChgParam, TRUE );
			break;

		//////////////////////////////////////////////////////////////////////////
		case DST_DEBUFF_ALL_CLEAR:
			pTargetMover->RemoveDebufBuffs();
			break;
		//////////////////////////////////////////////////////////////////////////

		default: // �׿� DST_�� �Ϲ����� ��ƾ�� ź��.
			// �Ϲ� DST_ó��
			pTargetMover->SetDestParam( dwDestParam, nAdjParam, nChgParam, bSend );
			break;
		}
	}

	return TRUE;
}

BOOL	FLSkillSystem::ApplyActiveParam( CCtrl* pUseCtrl, CCtrl* pTarget, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp, BOOL bSend, int nDamage )
{
	if( pUseCtrl == NULL )		return FALSE;

	for( int i = 0; i < MAX_SKILL_PARAM; i ++ )
	{
		DoActiveParam( pUseCtrl, pTarget, pSkillProp, pAddSkillProp, i, bSend, nDamage );
	}

	return TRUE;
}

BOOL	FLSkillSystem::DoActiveParam( CCtrl* pUseCtrl, CCtrl* pTarget, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp, int nIndex, BOOL /*bSend*/, int nDamage )
{
	CMover *pTargetMover = (CMover *)pTarget;

	DWORD	dwDestParam, dwSkillTime, dwPainTime;
	int		nAdjParam, nChgParam;
	int		nDestData[3];

	if( pAddSkillProp )
	{
		dwDestParam = pAddSkillProp->dwDestParam[nIndex];
		dwSkillTime = pAddSkillProp->dwSkillTime;
		dwPainTime = pAddSkillProp->dwPainTime;

		nAdjParam	= pAddSkillProp->dwAdjParamVal[nIndex];
		nChgParam	= (int)pAddSkillProp->dwChgParamVal[nIndex];
		nDestData[0] = pAddSkillProp->arrAddSkillDestNumData1[0];
		nDestData[1] = pAddSkillProp->arrAddSkillDestNumData1[1];
		nDestData[2] = pAddSkillProp->arrAddSkillDestNumData1[2];
	}
	else
	{	
		dwDestParam = pSkillProp->dwDestParam[nIndex];
		dwSkillTime = pSkillProp->dwSkillTime;
		dwPainTime  = pSkillProp->tmContinuousPain;
		nAdjParam	= pSkillProp->nAdjParamVal[nIndex];
		nChgParam	= (int)pSkillProp->dwChgParamVal[nIndex];
		nDestData[0] = pSkillProp->nDestData1[0];
		nDestData[1] = pSkillProp->nDestData1[1];
		nDestData[2] = pSkillProp->nDestData1[2];
	}
	// destParam 1
	switch( dwDestParam )
	{
	case NULL_ID:
		break;
	case DST_CHR_STEALHP: 			// HP���.  ������ DST_�� �ϱⰡ �� ������.
		{
			int nStealHP = (int)( nDamage * (nAdjParam / 100.0f) );	// ���������� ���������� ����Ҿ��� ����.
			if( pUseCtrl->GetType() == OT_MOVER )
				((CMover *)pUseCtrl)->SetDestParam( DST_HP, nStealHP, NULL_CHGPARAM );		// ������ HP�� ���.
		}
		break;
	case DST_CHR_STEALHP_IMM: 			// �����Ⱦ��� HP���.  
		{
			if( pTargetMover->GetType() == OT_MOVER )
			{
				int nHP = pTargetMover->GetHitPoint();		// ������� hp
				int nStealHP = nAdjParam;	// ����ؾ��� hp
				if( nHP < nStealHP )	// �Ǹ� �̾ƿ;��ϴµ� �����ǰ� �� ������.
					nStealHP = nHP;		// �����Ǹ�ŭ �̾ƿ�
				//SetDestParam( DST_HP, -nStealHP, 0 );		// �Ǹ� ����.
				if( pUseCtrl->GetType() == OT_MOVER )
				{
					//   						((CMover*)this)->m_pActMover->SendDamage( AF_FORCE,		// Ÿ���� �������԰�
					//   													      GetId(), 
					//  														  nStealHP );
					// chipi_080303 ���� ���� - ���Ͱ� �ڱ� �ڽ��� Ÿ������ �����ϴ� ����
					pTargetMover->m_pActMover->SendDamage( AF_FORCE,		// Ÿ���� �������԰�
						((CMover*)pUseCtrl)->GetId(), 
						nStealHP );


					((CMover *)pUseCtrl)->SetDestParam( DST_HP, nStealHP, NULL_CHGPARAM, TRUE  );		// ������ HP�� ���.
				}
			}
		}
		break;

#ifdef __WORLDSERVER
	case DST_HP:	// ����.
		if( pAddSkillProp )
		{
			if( pSkillProp->dwReferTarget1 == RT_HEAL || pSkillProp->dwReferTarget2 == RT_HEAL )
			{
				DWORD dwHp1 = 0, dwHp2 = 0;

				if( pUseCtrl->GetType() == OT_MOVER )	// �����ڰ� ����϶��� ���ȿ� ���� ����
				{
					if( pSkillProp->dwReferTarget1 == RT_HEAL )
					{
						switch( pSkillProp->dwReferStat1 )
						{
						case DST_STA:		dwHp1 = ((CMover *)pUseCtrl)->GetSta();			break;
						case DST_DEX:		dwHp1 = ((CMover *)pUseCtrl)->GetDex();			break;
						case DST_INT:		dwHp1 = ((CMover *)pUseCtrl)->GetInt();			break;
						}
						dwHp1 = (DWORD)( ((pSkillProp->dwReferValue1 / 10.0f) * dwHp1) + (pAddSkillProp->dwSkillLvl * FLOAT(dwHp1 / 50.0f)) );
					}

					if( pSkillProp->dwReferTarget2 == RT_HEAL )
					{
						switch( pSkillProp->dwReferStat2 )
						{
						case DST_STA:		dwHp2 = ((CMover *)pUseCtrl)->GetSta();			break;
						case DST_DEX:		dwHp2 = ((CMover *)pUseCtrl)->GetDex();			break;
						case DST_INT:		dwHp2 = ((CMover *)pUseCtrl)->GetInt();			break;
						}
						dwHp2 = (DWORD)( ((pSkillProp->dwReferValue2 / 10.0f) * dwHp2) + (pAddSkillProp->dwSkillLvl * FLOAT(dwHp2 / 50.0f)) );
					}
				}

				// ������ HP��.
				int nIncHP = pAddSkillProp->dwAdjParamVal[0] + dwHp1 + dwHp2;
				ApplyHeal( (CMover *)pUseCtrl, pTargetMover, nIncHP, pAddSkillProp->dwChgParamVal[0], pSkillProp->dwExeTarget );
			}
		}
		break;

	case DST_FORCE_DAMAGE_MAX_HP_RATE:	// �ִ� HP�� x%��ŭ �������� ������. AdjParam ����(�ִ�10000%)
		{
			int nNeedDamage = static_cast< int >( pTargetMover->GetMaxHitPoint() * (nAdjParam/10000.) );
			if( nNeedDamage > 0 )
			{
				pTargetMover->m_pActMover->SendDamage( AF_FORCE, pUseCtrl->GetId(), nNeedDamage );
			}
		}
		break;

	case DST_FORCE_DAMAGE_SAFE_HP_RATE:	// �ִ� HP�� x%�� HP�� ���� �������� ������. AdjParam ����(�ִ�10000%)
		{
			int nRemainHP = static_cast< int >( pTargetMover->GetMaxHitPoint() * (nAdjParam/10000.) );
			int nNeedDamage = pTargetMover->GetHitPoint() - nRemainHP;
			if( nNeedDamage > 0 )
			{
				pTargetMover->m_pActMover->SendDamage( AF_FORCE, pUseCtrl->GetId(), nNeedDamage );
			}
		}
		break;

	case DST_FORCE_DAMAGE_VALUE:	// ����ġ ������ �ֱ� AdjParam(������)
		{
			pTargetMover->m_pActMover->SendDamage( AF_FORCE, pUseCtrl->GetId(), nAdjParam );
		}
		break;

	case DST_HEAL_HP_RATE:	// HP ���� ȸ�� AdjParam(�ִ� 10000%)
		{
			CMover* pSrcMover = (CMover*) pUseCtrl;
			if( pAddSkillProp )
			{
				if( pTargetMover->GetMaxHitPoint() > pTargetMover->GetHitPoint() )
				{
					int nIncHP = static_cast< int >( pTargetMover->GetMaxHitPoint() * (nAdjParam/10000.) );
					if( nIncHP <= 0 )
					{
						nIncHP = 1;
					}

					ApplyHeal( pSrcMover, pTargetMover, nIncHP, NULL_CHGPARAM, pSkillProp->dwExeTarget );
				}
			}
		}
		break;

	case DST_HEAL_MP_RATE:	// MP ���� ȸ�� AdjParam(�ִ� 10000%)
		{
//			CMover* pSrcMover = (CMover*) pUseCtrl;
			if( pAddSkillProp )
			{
				if( pTargetMover->GetMaxManaPoint() > pTargetMover->GetManaPoint() )
				{
					int nIncMP = static_cast< int >( pTargetMover->GetMaxManaPoint() * (nAdjParam/10000.) );
					if( nIncMP <= 0 )
					{
						nIncMP = 1;
					}

					pTargetMover->SetDestParam( DST_MP, nIncMP, NULL_CHGPARAM, TRUE );
				}
			}
		}
		break;

	case DST_HEAL_FP_RATE:	// FP ���� ȸ�� AdjParam(�ִ� 10000%)
		{
//			CMover* pSrcMover = (CMover*) pUseCtrl;
			if( pAddSkillProp )
			{
				if( pTargetMover->GetMaxFatiguePoint() > pTargetMover->GetFatiguePoint() )
				{
					int nIncFP = static_cast< int >( pTargetMover->GetMaxFatiguePoint() * (nAdjParam/10000.) );
					if( nIncFP <= 0 )
					{
						nIncFP = 1;
					}

					pTargetMover->SetDestParam( DST_FP, nIncFP, NULL_CHGPARAM, TRUE );
				}
			}
		}
		break;
	case DST_HEAL_HP_VALUE:		// HP ����ġ ȸ�� AdjParam
		{
			CMover* pSrcMover = (CMover*) pUseCtrl;
			if( pAddSkillProp )
			{
				if( pTargetMover->GetMaxHitPoint() > pTargetMover->GetHitPoint() )
				{
					ApplyHeal( pSrcMover, pTargetMover, nAdjParam, NULL_CHGPARAM, pSkillProp->dwExeTarget );
				}
			}
		}
		break;
	case DST_HEAL_MP_VALUE:		// MP ����ġ ȸ�� AdjParam
		{
//			CMover* pSrcMover = (CMover*) pUseCtrl;
			if( pAddSkillProp )
			{
				if( pTargetMover->GetMaxManaPoint() > pTargetMover->GetManaPoint() )
				{
					pTargetMover->SetDestParam( DST_MP, nAdjParam, NULL_CHGPARAM, TRUE );
				}
			}
		}
		break;
	case DST_HEAL_FP_VALUE:		// FP ����ġ ȸ�� AdjParam
		{
//			CMover* pSrcMover = (CMover*) pUseCtrl;
			if( pAddSkillProp )
			{
				if( pTargetMover->GetMaxFatiguePoint() > pTargetMover->GetFatiguePoint() )
				{
					pTargetMover->SetDestParam( DST_FP, nAdjParam, NULL_CHGPARAM, TRUE );
				}
			}
		}
		break;
#endif
	default:
		{
			return FALSE;
		}
		break;
	}

	return TRUE;
}

bool	FLSkillSystem::CanApplyBuff( CMover* pSrcMover, CMover* pTargetMover, SkillProp* pSkillProp )
{
	if( pSrcMover == NULL || pTargetMover == NULL || pSkillProp == NULL )
	{
		return false;
	}

	if( pSrcMover->GetType() != OT_MOVER || pTargetMover->GetType() != OT_MOVER )
	{
		return false;
	}

	MoverProp* pTargetProp	= pTargetMover->GetProp();
	if( pTargetProp == NULL )
	{
		return false;
	}

	if( pSkillProp->IsDebuff() == TRUE )
	{
		// �����̸� ��� ����� �ɸ��� ����.
		if( pTargetMover->m_dwMode & MATCHLESS_MODE )
		{
			return false;
		}

		// ���Ϳ��� ����� �ɶ� ��� üũ.
		if( pTargetMover->IsPlayer() == FALSE && pTargetProp->dwClass > pSkillProp->dwAdjMonsterGrade )
		{
			return false;
		}
	}
	else
	{
		// ���Ϳ��� ���� ������ �ɸ��� ����.
		if( pSrcMover->IsPlayer() == TRUE && pTargetMover->IsPlayer() == FALSE )
		{
			return false;
		}
	}

	return true;
}

#ifdef __WORLDSERVER
void	FLSkillSystem::ApplyHeal( CMover* pHealer, CMover* pTarget, int nHealValue, DWORD dwChgParamVal, DWORD dwExeTarget )
{
	if( nHealValue > 0 )
	{
		if( pHealer->GetType() == OT_MOVER )
		{
			if( pHealer->IsPlayer() )	// �����ڰ� �÷��̾��϶�
			{
				if( pTarget->GetMaxHitPoint() < pTarget->GetHitPoint() + nHealValue )		// Ÿ���� ������ �Ƴ�?
					((FLWSUser *)pHealer)->m_nOverHeal = PROCESS_COUNT * 30;				// �����ڴ� 30�ʰ� ������ ����. 
			}
		}
	}

#ifdef __AGGRO16
	FLAggro::SumHealAggro( *static_cast< CMover * >( pHealer ), *pTarget, ( pTarget->GetHitPoint() + nHealValue ) - pTarget->GetMaxHitPoint(), dwExeTarget );
#endif //__AGGRO16

	// ������.
	pTarget->SetDestParam( DST_HP, nHealValue, dwChgParamVal, TRUE );
}
#endif


#ifdef __CLIENT
//
// ��ų�� ������ Ŭ�󿡼� �����ؾ��� sfx ó��
//
void	FLSkillSystem::CreateSkillSfx( CCtrl* pUseCtrl, CCtrl *pTarget, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp )
{
	//sun skill here
	{
		// �߻�ü ���°� �ƴϴ�.
		CSfx *pSfx = NULL;
		if( (int)pAddSkillProp->dwSkillTime > 0 )	// ���ӽð��� �ִ� ��ų��
		{
			if( pSkillProp->dwSfxObj4 == NULL_ID )		// ������ �����Ǵ� ��ų�� �ƴҰ�쿡��.
			{
				if( pSkillProp->dwExeTarget == EXT_MELEEATK )
				{
					D3DXVECTOR3 vLocal;

					if( pSkillProp->dwNeedItemKind3 == IK3_YOYO )
					{
						vLocal = pTarget->GetPos();
						vLocal.y += 1.0f;
					}
					else
					{
						AngleToVectorXZ( &vLocal, pUseCtrl->GetAngle(), 1.3f );	// �����¹��� 1���;�.
						vLocal += pUseCtrl->GetPos();
						vLocal.y += 1.0f;
					}

					pSfx = CreateSfx( D3DDEVICE, pSkillProp->dwSfxObj3, pUseCtrl->GetPos(), pUseCtrl->GetId(), vLocal, pTarget->GetId(), 0 );

					if( pSkillProp->dwSfxObj5 != NULL_ID )		// �����ڿ��� �߻�
						pSfx = CreateSfx( D3DDEVICE, pSkillProp->dwSfxObj5, pUseCtrl->GetPos(), pUseCtrl->GetId(), pUseCtrl->GetPos(), pUseCtrl->GetId(), 0 );  // 5�� ����Ʈ�� �����.
				} else
				{
					if( pSkillProp->dwSfxObj3 != NULL_ID )		// Ÿ�ٿ��� �߻�
						pSfx = CreateSfx( D3DDEVICE, pSkillProp->dwSfxObj3, pUseCtrl->GetPos(), pUseCtrl->GetId(), pTarget->GetPos(), pTarget->GetId(), 0 );  // 3�� ����Ʈ�� �����.
					if( pSkillProp->dwSfxObj5 != NULL_ID )		// �����ڿ��� �߻�
						pSfx = CreateSfx( D3DDEVICE, pSkillProp->dwSfxObj5, pUseCtrl->GetPos(), pUseCtrl->GetId(), pUseCtrl->GetPos(), pUseCtrl->GetId(), 0 );  // 5�� ����Ʈ�� �����.
				}

				if( pSkillProp->dwNeedItemKind3 == IK3_YOYO )  //���� ������������Ƽ�����Ͽ� ����Ʈ ����(����ó��)
					CreateYoyoSkill( pUseCtrl, pSfx, pTarget, pSkillProp, pAddSkillProp );

			} else
			{
				if( pUseCtrl->GetType() == OT_MOVER )
				{
					//gmpbigsun( 20100824 ) : CreateSfx�� Return���� �޾Ƽ� ���� ����� �����ϰ� ��.
					if( pSkillProp->dwSkillType == NULL_ID )	// NULL_ID�� �����۵�
					{
						if( ((CMover *)this)->HasBuff( BUFF_ITEM, (WORD)( pSkillProp->dwID ) ) == FALSE )	// �̹� ����ǰ� ���� �������� ���� ���
							pSfx = CreateSfx( D3DDEVICE, pSkillProp->dwSfxObj4, pUseCtrl->GetPos(), pUseCtrl->GetId(), pTarget->GetPos(), pTarget->GetId(), (int)(pAddSkillProp->dwSkillTime / 1000.0f) );
					} else
					{
						// �̹� ����ǰ� ���� �������� ���� ���
						//						BOOL bRst = ((CMover *)this)->HasBuff( BUFF_SKILL, (WORD)( pSkillProp->dwID ) );

						//gmpbigsun(20100826) : �������´� ���� IBuff::Process���� sfx�� �����ϹǷ� ���⼭ sfx�� ������ �ʿ䰡 ����.
						//if( ((CMover *)this)->HasBuff( BUFF_SKILL, (WORD)( pSkillProp->dwID ) ) == FALSE
						//	&& pSkillProp->dwID != SI_MER_SHIELD_PROTECTION && pSkillProp->dwID != SI_MER_SHIELD_PANBARRIER )	// 091022 mirchang - ���⼭ ���� ���� �� IBuff.Process���� sfx�� �ٽ� �����ϴ� ���� �߻�. ����.
						//	pSfx = CreateSfx( D3DDEVICE, pSkillProp->dwSfxObj4, this->GetPos(), this->GetId(), pTarget->GetPos(), pTarget->GetId(), (int)(pAddSkillProp->dwSkillTime / 1000.0f) );

						//gmpbigsun(20100920, #0010102) : �Ϻν�ų ��������Ʈ �ȳ��´ٴ� ����
						//�ƴ� �̰� ������ �׳� �������� �˼��ִ� ����� ����??
						//�ϴ� hardly
						if( pSkillProp->dwID == 170 || pSkillProp->dwID == 151 || pSkillProp->dwID == 225 )
							pSfx = CreateSfx( D3DDEVICE, pSkillProp->dwSfxObj4, pUseCtrl->GetPos(), pUseCtrl->GetId(), pTarget->GetPos(), pTarget->GetId(), (int)(pAddSkillProp->dwSkillTime / 1000.0f) );

						D3DXVECTOR3 vPos = pUseCtrl->GetPos();
						PLAYSND( pSkillProp->dwSndAttack1, &vPos );		// �߻� ȿ����.
					}
				}
			}
		} else
		{	// ���ӽð� ���� ��ų
			if( pSkillProp->dwExeTarget == EXT_MELEEATK )	// �������� ��ų�� Ÿ�� ����Ʈ.
			{
				if( pSkillProp->dwSfxObj3 != NULL_ID )	
				{
					D3DXVECTOR3 vLocal;

					if( pSkillProp->dwNeedItemKind3 == IK3_YOYO )
					{
						vLocal = pTarget->GetPos();
						vLocal.y += 1.0f;
					}
					else
					{
						AngleToVectorXZ( &vLocal, pUseCtrl->GetAngle(), 1.3f );	// �����¹��� 1���;�.
						vLocal += pUseCtrl->GetPos();		// GetPos()�� this���� pTarget���� �ٲ��. 2006/6/20 xuzhu.
						vLocal.y += 1.0f;
					}

					pSfx = CreateSfx( D3DDEVICE, pSkillProp->dwSfxObj3, vLocal, pUseCtrl->GetId(), vLocal, pTarget->GetId(), 0 );	// 2006/6/20 xuzhu �� �ٲ�.
				}
			} else
			{
				if( pSkillProp->dwSfxObj3 != NULL_ID )
				{
					D3DXVECTOR3 vPos = pTarget->GetPos();
					vPos.y += 1.0f;
					pSfx = CreateSfx( D3DDEVICE, pSkillProp->dwSfxObj3, pUseCtrl->GetPos(), pUseCtrl->GetId(), vPos, pTarget->GetId(), 0 );
				}
			}

			if( pSkillProp->dwNeedItemKind3 == IK3_YOYO )  //���� ������������Ƽ�����Ͽ� ����Ʈ ����(����ó��)
			{
				CreateYoyoSkill( pUseCtrl, pSfx, pTarget, pSkillProp, pAddSkillProp );
			}
			else
				if( pSkillProp->dwSfxObj5 != NULL_ID )		// �����ڿ��� �߻�
					pSfx = CreateSfx( D3DDEVICE, pSkillProp->dwSfxObj5, pUseCtrl->GetPos(), pUseCtrl->GetId(), pUseCtrl->GetPos(), pUseCtrl->GetId(), 0 );  // 5�� ����Ʈ�� ������ �ڽſ��� ���
		}
		if( pSfx )
		{
			D3DXVECTOR3 vPos = pUseCtrl->GetPos();
			PLAYSND( pSkillProp->dwSndAttack1, &vPos );		// �߻� ȿ����.
		}

		//dwSkillRange
		if( pSkillProp->dwID == SKILL_INDEX( 187, SI_GEN_FLASH ) && pSfx )
		{
			FLOAT fRange = 1.5f;
			pSfx->SetScale(  D3DXVECTOR3(fRange, fRange, fRange) );
		}
	}
} // CreateSkillSfx


void	FLSkillSystem::CreateYoyoSkill( CCtrl* pUseCtrl, CSfx* pSfx, CCtrl *pTarget, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp )
{
	PT_ITEM_SPEC pItemProp = ((CMover *)pUseCtrl)->GetActiveHandItemProp();

	if(pItemProp)
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vLocal;
		D3DXVECTOR3 vPosSrc   = pUseCtrl->GetPos() + D3DXVECTOR3( 0.0f, 1.0f, 0.0f ); // �߻� ������ ���Ƿ� �÷��ش�. ���� 
		D3DXVECTOR3 vPosDest  = pTarget->GetPos() + D3DXVECTOR3( 0.0f, 1.0f, 0.0f ); // ��ǥ ������ ���Ƿ� �÷��ش�. ���� 

		CModelObject *pModel = (CModelObject *)pUseCtrl->GetModel();

		int nSkill = pSkillProp->dwID;

		switch( nSkill )
		{
		case SKILL_INDEX( 197, SI_ACR_SUP_SLOWSTEP ):
		case SKILL_INDEX( 212, SI_JST_YOYO_HITOFPENYA ):
		case SKILL_INDEX( 214, SI_JST_YOYO_VATALSTAB ):
			{
				pModel->GetHandPos( &vPos, PARTS_RWEAPON, pUseCtrl->GetMatrixWorld() );
				pSfx = CreateSfxYoYo( D3DDEVICE, pItemProp->dwSfxObj2, vPos, pUseCtrl->GetId(), vPosDest );	
				((CSfxItemYoyoAtk*)pSfx)->MakePath(PARTS_RWEAPON);
			}
			break;
		case SKILL_INDEX( 205, SI_ACR_YOYO_DEADLYSWING ):
		case SKILL_INDEX( 201, SI_ACR_YOYO_CROSSLINE ):
			{
				pModel->GetHandPos( &vPos, PARTS_RWEAPON, pUseCtrl->GetMatrixWorld() );
				pSfx = CreateSfxYoYo( D3DDEVICE, pItemProp->dwSfxObj2, vPos, pUseCtrl->GetId(), vPosDest );	
				((CSfxItemYoyoAtk*)pSfx)->MakePath(PARTS_RWEAPON);

				pModel->GetHandPos( &vPos, PARTS_LWEAPON, pUseCtrl->GetMatrixWorld() );
				pSfx = CreateSfxYoYo( D3DDEVICE, pItemProp->dwSfxObj2, vPos, pUseCtrl->GetId(), vPosDest );	
				((CSfxItemYoyoAtk*)pSfx)->MakePath(PARTS_LWEAPON);
			}
			break;							
		}
	}

}

#endif // Client