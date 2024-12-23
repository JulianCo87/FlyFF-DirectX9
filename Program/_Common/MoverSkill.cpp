#include "stdafx.h"
#include "defineText.h"
#include "ModelObject.h"
#include "CreateObj.h"
#include "eveschool.h"
#include "party.h"

#ifdef __CLIENT
	#include "../Neuz/DPClient.h"
	#include "defineSound.h"
#endif	// __CLIENT
#ifdef	__WORLDSERVER
	#include "..\_aiinterface\AIPet.h"
	#include "../WorldServer/User.h"
	#include "WorldMng.h"
	#include "../WorldServer/ItemUpgrade.h"
	#include "../WORLDSERVER/FLItemUsing.h"
	#include "../worldserver/FLMapGrid.h"
	#include "../WorldServer/LinkMap.h"
#endif	// __WORLDSERVER

#include "playerdata.h"	//sun: 11, ĳ���� ���� ����
#include "honor.h"		//sun: 13, ����

#include "../_AIInterface/FLFSM.h"

#include "FLSkillSystem.h"



#include "FunnyCoin.h"

#ifdef __CLIENT
	extern	CDPClient			g_DPlay;
#else
	extern	CWorldMng			g_WorldMng;
#include "GuildHouse.h"
#endif

extern	CPartyMng				g_PartyMng;
extern	CGuildCombat			g_GuildCombatMng;

extern void __SetQuest( DWORD dwIdMover, int nQuest );

#ifdef __WORLDSERVER

//sun: 10�� ���½ý���	Neuz, World, Trans
void	CMover::SetMasterSkillPointUp()
{
	if( m_nLevel != 72 && m_nLevel != 84 && m_nLevel != 96 && m_nLevel != 108 )
		return;
	if( IsHero() )
		return;

	for( int i = 0; i < MAX_SKILL_JOB; i++ ) 
	{				
		LPSKILL lpSkill = &(m_aJobSkill[i]);
		if( lpSkill && lpSkill->dwSkill != NULL_ID )
		{
			SkillProp* pSkillProp    = prj.GetSkillProp( lpSkill->dwSkill );			
			if( pSkillProp == NULL )
				continue;
			if( pSkillProp->dwSkillKind1 != JTYPE_MASTER )
				continue;

			lpSkill->dwLevel++;
			g_dpDBClient.SendLogSkillPoint( LOG_SKILLPOINT_USE, 1, this, &(m_aJobSkill[i]) );
		}
	}	
	g_xWSUserManager->AddCreateSfxObj(this, XI_INDEX( 108, XI_SYS_EXCHAN01 ), GetPos().x, GetPos().y, GetPos().z);

	FLWSUser *pUser = (FLWSUser *)this;
	pUser->AddDoUseSkillPoint( &m_aJobSkill[0], m_nSkillPoint );
}


// ��ųť�� ��� �ִ� �������� �� ĳ���� �ð��� ���.
int		CMover::GetQueueCastingTime()
{
	if( IsNPC() )	
		return 0;

	FLWSUser *pUser = (FLWSUser *)this;
	SHORTCUT *pSlotQueue = pUser->m_playTaskBar.m_aSlotQueue;
	int		nTotalTime = 0;

	for( int i = 0; i < MAX_SLOT_QUEUE; i ++ )
	{
		if( pSlotQueue[i].m_dwShortcut )
		{
			LPSKILL pSkill = GetSkill( 0, pSlotQueue[i].m_dwId );
			if( pSkill == NULL )
			{
				continue;
			}

			SkillProp *pSkillProp = pSkill->GetProp();
			if( !( pSkillProp ) )					// JobSkill ����Ʈ���� ���� ��ų�� ������Ƽ�� ����.
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "%s. ��ų(%d)�� ������Ƽ�� ����." ), m_szName, pSkill->dwSkill );
				return FALSE;	// property not found
			}
			
#ifdef __SKILL0517
			AddSkillProp* pAddSkillProp= prj.GetAddSkillProp( pSkillProp->dwSubDefine, GetSkillLevel( pSkill ) );
			if( !( pAddSkillProp ) )					// JobSkill ����Ʈ���� ���� ��ų�� ������Ƽ�� ����.
#else	// __SKILL0517
			AddSkillProp* pAddSkillProp= prj.GetAddSkillProp( pSkillProp->dwSubDefine, pSkill->dwLevel );
			if( !( pAddSkillProp ) )					// JobSkill ����Ʈ���� ���� ��ų�� ������Ƽ�� ����.
#endif	// __SKILL0517
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "%s. �ֵ彺ų(%d)�� ������Ƽ�� ����." ), m_szName, pSkill->dwSkill );
				return FALSE;	// property not found
			}

#ifdef _DEBUG
		//sun: 9, ���ο� AddSkillProp �÷� �߰� �� ����
			if( (int)pAddSkillProp->dwCastingTime <= 0 )
				FLERROR_LOG( PROGRAM_NAME, _T( "GetQueueCastingTime : %s�� addProp dwCastingTime�� %d" ), pSkillProp->szName, pAddSkillProp->dwCastingTime );
#endif

			nTotalTime += (int)pAddSkillProp->dwCastingTime;	//sun: 9, ���ο� AddSkillProp �÷� �߰� �� ����
		}
	}

	nTotalTime = (int)(nTotalTime * 0.7f);		// 70���� ��������.
	nTotalTime = (int)( (nTotalTime / 1000.0f) * SEC1 );	// ĳ���ô��� �ð����� ��ȯ.
	nTotalTime	= GetCastingTime( nTotalTime );

	return nTotalTime;
}

#endif // worldserver


// ������ ��ų ��� ����� ������
// Mover�� ��밡���� ��ų�迭(m_aJobSkill[3+14])���� nIdx�� �ش��ϴ� ��ų�� ���� �װ��� �����Ѵ�.
// sutType : ��ų���� ����Ű/��ųť�� ���θ� Ŭ��κ��� �޾Ƽ� ó��.  ĳ����Ÿ�� ����� �ٸ���.
//
#ifdef __CLIENT
DWORD	CMover::DoUseSkill( const int nType, const int nIdx, const OBJID idFocusObj, const SKILLUSETYPE sutType, const BOOL bControl, const int nCastingTime, DWORD dwSkill, DWORD dwLevel )
#else // __CLIENT
DWORD	CMover::DoUseSkill( const int nType, const int nIdx, const OBJID idFocusObj, const SKILLUSETYPE sutType, const BOOL bControl )
#endif // __CLIENT
{
	if( IsNPC() )
	{
		return FSC_SKILLUSE_FAIL;		// ���Ǿ��� ������ Ÿ�� �ȵȴ�.
	}

#ifdef __WORLDSERVER
	DWORD dwSkill = NULL_ID;
	DWORD dwLevel = 0;
#endif	// __WORLDSERVER

	if( nIdx >= 0 )
	{
		LPSKILL pSkill = GetSkill( nType, nIdx );
		if( pSkill == NULL )
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "%s�� nIdx�� ��ų�� ������ ���� �ʴ�. %d" ), m_szName, nIdx );
			return FSC_SKILLUSE_FAIL;
		}

		dwSkill	= pSkill->dwSkill;
#ifdef __SKILL0517
		dwLevel	= GetSkillLevel( pSkill );
#else	// __SKILL0517
		dwLevel	= pSkill->dwLevel;
#endif	// __SKILL0517
	}

	if( dwSkill == NULL_ID || dwLevel <= 0 )
	{
		return FSC_SKILLUSE_FAIL;
	}

	//////////////////////////////////////////////////////////////////////////
	// ĳ���� �ð� ����
#ifdef __WORLDSERVER
	SkillProp* pSkillProp = NULL;
	AddSkillProp* pAddSkillProp = NULL;
	if( GetSkillProp( &pSkillProp, &pAddSkillProp, dwSkill, dwLevel, "DoUseSkill_P" ) == FALSE )
	{
		return FSC_SKILLUSE_FAIL;
	}

	int nCastingTime = 0;
	if( pSkillProp->dwSkillType == KT_MAGIC )
	{
		if( sutType == SUT_QUEUESTART )		// ��ųť ���� ��ų
		{
			nCastingTime = GetQueueCastingTime();	// ť���� ��� ��ų�� ĳ������ ���Ƽ� �Ѵ�.
		} 
		else if( sutType == SUT_QUEUEING )	// ��ųť ������ ��ų
		{
			nCastingTime = 0;				// ĳ���� ���۾��� ����.
		} 
		else
		{									// �Ϲ����� ���� ��ų ���
			//sun: 9, ���ο� AddSkillProp �÷� �߰� �� ����
			nCastingTime	= (int)( ( pAddSkillProp->dwCastingTime / 1000.0f ) * SEC1 );	// ĳ���� ���� ����.
			nCastingTime = GetCastingTime( nCastingTime );
		}
	}
	else if( pSkillProp->dwSkillType == KT_SKILL )
	{
		nCastingTime = 1;
	}
#endif
	//////////////////////////////////////////////////////////////////////////

	return g_cSkillSystem->DoUseSkill( this, dwSkill, dwLevel, idFocusObj, sutType, bControl, nCastingTime );
}

// �Ϲ� ���� ������ �������� ȣ��.
void	CMover::OnEndMeleeAttack( DWORD /*dwState*/ )
{
#ifdef __WORLDSERVER
	PostAIMsg( AIMSG_END_MELEEATTACK );
#endif // WorldServer
}

//sun, 11, Ȯ����ų ȿ������ world,neuz
DWORD	CMover::GetRemoveSfxObj( DWORD dwRemoveSfxObjID )
{
	DWORD	dwResult = 0;
	for( int i = 0; i < MAX_SKILLBUFF_COUNT; i++ )
	{
		if( dwRemoveSfxObj[i] == dwRemoveSfxObjID )
		{
			dwResult = dwRemoveSfxObj[i];
			dwRemoveSfxObj[i] = 0;
			return dwResult;
		}
	}

	return	dwResult;
}

void	CMover::SetRemoveSfxObj( DWORD dwRemoveSfxObjID )
{
	for( int i = 0; i < MAX_SKILLBUFF_COUNT; i++ )
	{
		if( dwRemoveSfxObj[i] == dwRemoveSfxObjID )
			return;
	}

	for( int i = 0; i < MAX_SKILLBUFF_COUNT; i++ )
	{
		if( dwRemoveSfxObj[i] == 0 )
		{
			dwRemoveSfxObj[i] = dwRemoveSfxObjID;
			return;
		}
	}
}
/*--------------------------------------------------------------------------------------------------------


											Do Use Item


----------------------------------------------------------------------------------------------------------*/

// ������ ID�� ������ ����(�κ��丮�� ������� ���)
BOOL CMover::DoUseItemVirtual( DWORD dwItemId, BOOL bEffectSkip )
{
#ifdef __WORLDSERVER
	//�κ����� ���� ���ô� ������ ������� ���డ���ϰ��Ѵ�.
	PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( dwItemId );
	if( pItemProp != NULL )
	{
		if( IsItemRedyTime( pItemProp, pItemProp->dwID, FALSE ) == FALSE )
			return FALSE;

		if( dwItemId == ITEM_INDEX( 10469, II_SYS_SYS_SCR_RETURN ) )
		{
			// ���ư��� 
			if( HasBuff( BUFF_ITEM, ITEM_INDEX( 10469, II_SYS_SYS_SCR_RETURN ) ) )
			{
				RemoveBuff( BUFF_ITEM, ITEM_INDEX( 10469, II_SYS_SYS_SCR_RETURN ) );
			}

			DWORD dwWorldID = 0;
			D3DXVECTOR3 vPos = ((FLWSUser*)this)->GetReturnPos( &dwWorldID );
			((FLWSUser*)this)->REPLACE( g_uIdofMulti, dwWorldID, vPos, REPLACE_NORMAL, nTempLayer );
			return TRUE;
		}

		if( dwItemId == ITEM_INDEX( 10435, II_CHR_SYS_SCR_ESCAPEBLINKWING ) )
		{
			if( pItemProp->dwCircleTime != NULL_ID )
			{
				if( !IsSMMode( SM_ESCAPE ) )
				{
					SetSMMode( SM_ESCAPE, pItemProp->dwCircleTime );
				}
				else
				{
					return FALSE;
				}
			}

			return g_pItemUsing->DoUseItemBlinkWing( static_cast< FLWSUser* >( this ), pItemProp, NULL, FALSE );
		}

		D3DXVECTOR3 sPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		DWORD	dwSfxID = pItemProp->dwSfxObj3;
		if( dwSfxID != NULL_ID && !bEffectSkip )		// ������ ���ÿ� ������ ����Ʈ
		{
			g_xWSUserManager->AddCreateSfxObj( this, dwSfxID, sPos.x, sPos.y, sPos.z );	// ������ǥ�� ����.
		}	
	}
#endif //__WORLDSERVER

	return TRUE;
}


#ifdef __WORLDSERVER
BOOL CMover::IsDoUseBuff( PT_ITEM_SPEC pItemProp )
{
	int nResult = 0;
	if( pItemProp->dwItemKind3 == IK3_POTION_BUFF_STR
		|| pItemProp->dwItemKind3 == IK3_POTION_BUFF_DEX
		|| pItemProp->dwItemKind3 == IK3_POTION_BUFF_INT
		|| pItemProp->dwItemKind3 == IK3_POTION_BUFF_STA
		|| pItemProp->dwItemKind3 == IK3_POTION_BUFF_DEFENSE		
		|| pItemProp->dwItemKind3 == IK3_ANGEL_BUFF				//sun: 8�� ���� ��ȯ Neuz, World, Trans
		|| pItemProp->dwItemKind2 == IK2_BUFF_TOGIFT
		)
	{
		if( HasBuffByIk3( pItemProp->dwItemKind3 ) )
			nResult = 1;

//sun: 8�� ���� ��ȯ Neuz, World, Trans
//sun: 10, __PKSERVER_USE_ANGEL
		if( pItemProp->dwItemKind3 == IK3_ANGEL_BUFF )
		{
			if(g_eLocal.GetState( EVE_PK ))	
			{
				if( 0 >= GetPKValue() )
					nResult = 2;
				if( GetQuest( QUEST_INDEX( 5004, QUEST_PKMINUS ) ) == NULL )
					nResult = 3;
				if( m_Inventory.GetAtByItemId( ITEM_INDEX( 30039, II_SYS_SYS_QUE_ANGEL_RED100 ) ) != NULL 
					|| m_Inventory.GetAtByItemId( ITEM_INDEX( 30040, II_SYS_SYS_QUE_ANGEL_BLUE100 ) ) != NULL 
					|| m_Inventory.GetAtByItemId( ITEM_INDEX( 30041, II_SYS_SYS_QUE_ANGEL_GREEN100 ) ) != NULL 
					|| m_Inventory.GetAtByItemId( ITEM_INDEX( 30042, II_SYS_SYS_QUE_ANGEL_WHITE100 ) ) != NULL 
					)
					nResult = 4;
			}
			else
				nResult = 5;
		}
	}
//sun: 8,
	else if( pItemProp->dwID == ITEM_INDEX( 26114, II_SYS_SYS_SCR_DEFENSE20 ) || pItemProp->dwID == ITEM_INDEX( 26115, II_SYS_SYS_SCR_DEFENSE50 ) )
	{
		if( HasBuff( BUFF_ITEM, ITEM_INDEX( 26114, II_SYS_SYS_SCR_DEFENSE20 ) ) || HasBuff( BUFF_ITEM, ITEM_INDEX( 26115, II_SYS_SYS_SCR_DEFENSE50 ) ) )
			nResult = 1;
	}
	else if( pItemProp->dwID == ITEM_INDEX( 26531, II_SYS_SYS_SCR_PET_FEED_POCKET02 ) )
	{
		DWORD dwMax	= m_Inventory.GetMax();
		FLItemElem* pItem;
		for( DWORD i = 0 ; i < dwMax; i++ )
		{
			pItem	= m_Inventory.GetAtId( i );
			if( IsUsableItem( pItem )
				&& pItem->m_dwItemId == ITEM_INDEX( 21035, II_SYS_SYS_SCR_PET_FEED_POCKET )
				&& pItem->m_dwKeepTime > 0
				&& !pItem->IsFlag( FLItemElem::expired )
				)
			{
				nResult		= 1;
				break;
			}
		}
		if( nResult != 1 )
		{
			IBuff* pBuff	= m_buffs.GetBuff( BUFF_ITEM, (WORD)( pItemProp->dwID ) );
			if( pBuff )
			{
				DWORD dwCurr	= ::timeGetTime();
				if( static_cast<int>( pItemProp->dwSkillTime ) > static_cast<int>( pItemProp->dwSkillTime + ( pBuff->GetTotal() - ( dwCurr - pBuff->GetInst() ) ) ) )
					nResult		= 1;
			}
		}
	}
	else if( HasBuff( BUFF_ITEM, (WORD)( pItemProp->dwID ) ) )
	{
		nResult = 1;
#ifdef DAILY_QUEST
		if ( pItemProp->dwItemKind3 == IK3_DAILY_QUEST )
			nResult = 6;
#endif // DAILY_QUEST	
	}
//sun: 11, ä�� �ý���
	else if( pItemProp->dwID == ITEM_INDEX( 26454, II_GEN_TOO_COL_SILVERBATTERY )
		|| pItemProp->dwID == ITEM_INDEX( 26455, II_GEN_TOO_COL_GOLDBATTERY )
		|| pItemProp->dwID == ITEM_INDEX( 26560, II_GEN_TOO_COL_BATTERY001 )
#ifdef BATTERY_PREMIUM
		|| pItemProp->dwID == ITEM_INDEX( 25455, II_GEN_TOO_COL_SILVERBATTERY_P )
		|| pItemProp->dwID == ITEM_INDEX( 25456, II_GEN_TOO_COL_GOLDBATTERY_P )
		|| pItemProp->dwID == ITEM_INDEX( 25457, II_GEN_TOO_COL_BATTERY001_P )
#endif
		)
	{
		if( HasBuff( BUFF_ITEM, ITEM_INDEX( 26454, II_GEN_TOO_COL_SILVERBATTERY ) )
			|| HasBuff( BUFF_ITEM, ITEM_INDEX( 26455, II_GEN_TOO_COL_GOLDBATTERY ) )
			|| HasBuff( BUFF_ITEM, ITEM_INDEX( 26560, II_GEN_TOO_COL_BATTERY001 ) )
#ifdef BATTERY_PREMIUM
			|| HasBuff( BUFF_ITEM, ITEM_INDEX( 25455, II_GEN_TOO_COL_SILVERBATTERY_P ) )
			|| HasBuff( BUFF_ITEM, ITEM_INDEX( 25456, II_GEN_TOO_COL_GOLDBATTERY_P ) )
			|| HasBuff( BUFF_ITEM, ITEM_INDEX( 25457, II_GEN_TOO_COL_BATTERY001_P ) )
#endif
			)
			nResult = 1;
	}

//sun: 12, �ش����������
	else if( pItemProp->dwID == ITEM_INDEX( 26537, II_SYS_SYS_SCR_PARTYEXPUP01 )
		|| pItemProp->dwID == ITEM_INDEX( 26538, II_SYS_SYS_SCR_PARTYEXPUP02 )
		|| pItemProp->dwID == ITEM_INDEX( 20296, II_SYS_SYS_SCR_PARTYEXPUP01_01 )
		)
	{
		if( HasBuff( BUFF_ITEM, ITEM_INDEX( 26537, II_SYS_SYS_SCR_PARTYEXPUP01 ) )
			|| HasBuff( BUFF_ITEM, ITEM_INDEX( 26538, II_SYS_SYS_SCR_PARTYEXPUP02 ) )
			|| HasBuff( BUFF_ITEM, ITEM_INDEX( 20296, II_SYS_SYS_SCR_PARTYEXPUP01_01 ) )
			)
			nResult = 1;
	}
	else if( pItemProp->dwID == ITEM_INDEX( 26573, II_SYS_SYS_SCR_SPETGOOD )
		)
	{
		if( HasBuff( BUFF_ITEM, ITEM_INDEX( 26573, II_SYS_SYS_SCR_SPETGOOD ) )
			)
			nResult = 1;
	}
	else if( pItemProp->dwID == ITEM_INDEX( 26539, II_SYS_SYS_SCR_PARTYSKILLUP01 )
		|| pItemProp->dwID == ITEM_INDEX( 26540, II_SYS_SYS_SCR_PARTYSKILLUP02 )
		|| pItemProp->dwID == ITEM_INDEX( 20297, II_SYS_SYS_SCR_PARTYSKILLUP01_01 )
		)
	{
		if( HasBuff( BUFF_ITEM, ITEM_INDEX( 26539, II_SYS_SYS_SCR_PARTYSKILLUP01 ) )
			|| HasBuff( BUFF_ITEM, ITEM_INDEX( 26540, II_SYS_SYS_SCR_PARTYSKILLUP02 ) )
			|| HasBuff( BUFF_ITEM, ITEM_INDEX( 20297, II_SYS_SYS_SCR_PARTYSKILLUP01_01 ) )
			)
			nResult = 1;
	}

	
	switch( nResult )
	{
	case 1:
		((FLWSUser*)this)->AddDefinedText( TID_GAME_LIMITED_USE, "" ); // ������̾ ����Ҽ� ����
		break;
	//sun: 8�� ���� ��ȯ Neuz, World, Trans
	case 2:
		((FLWSUser*)this)->AddDefinedText( TID_GAME_PKVALUE_NOT_ANGEL );
		break;
	case 3:
		((FLWSUser*)this)->AddDefinedText( TID_GAME_NOT_QUEST_ANGEL );
		break;	
	case 4:
		((FLWSUser*)this)->AddDefinedText( TID_GAME_NOT_COMPLETE_ANGEL );
		break;	
#ifdef DAILY_QUEST
	case 6:
		((FLWSUser*)this)->AddDefinedText( TID_GAME_DAILYQUEST_COMPLETE );	// �̹� ��������Ʈ�� �Ϸ��Ͽ����ϴ�
		break;
#endif // DAILY_QUEST
	}
	return nResult;
}

#ifdef __AGGRO16
FLAggro *		
CMover::GetAggroMng()
{
	if( m_pAIInterface != NULL )
	{
		return m_pAIInterface->GetAggroMng();
	}
	else if( m_pFSM != NULL )
	{
		return m_pFSM->GetAggroMng();
	}
	return NULL;
}

#endif //__AGGRO16

#endif // __WORLDSERVER


/*--------------------------------------------------------------------------------------------------------


											Do Wand Attack


----------------------------------------------------------------------------------------------------------*/
//
// �ϵ������ �϶�!
//
int	 CMover::DoAttackMagic( CObj *pTargetObj, int nMagicPower, int idSfxHit )
{
	if( IsInvalidObj(pTargetObj) )		// Ÿ���� �Žñ� �ϸ� ����
		return( -1 );

	PT_ITEM_SPEC pItemProp;
#ifndef __CLIENT
	FLItemElem* pItemElem = GetWeaponItem();	// �տ��� ���Ⱑ ������ ����
	if( pItemElem == NULL )
		return( -1 );

	pItemProp	= pItemElem->GetProp();
#else	// __CLIENT
	if( IsActiveMover() )
	{
		FLItemElem* pItemElem = GetWeaponItem();
		if( pItemElem == NULL )
			return( -1 );

		pItemProp	= pItemElem->GetProp();
	}
	else
	{
		DWORD dwItemId	= m_aEquipInfo[PARTS_RWEAPON].dwItemID;
		if( dwItemId == 0 )
			return( -1 );

		pItemProp	= g_xSpecManager->GetSpecItem( dwItemId );
	}
#endif	// __CLIENT

	if( pItemProp->dwItemKind3 != IK3_WAND )		// ����ִ� ���Ⱑ �ϵ尡 �ƴϸ� ����.
		return( -1 );

#ifdef __CLIENT
	// ���� ������ ��ǥ�������� �����°� ������ �޽��� ����ϰ� ����
	CWorld *pWorld = GetWorld();
	D3DXVECTOR3 vStart = GetPos();				vStart.y += 0.5f;
	D3DXVECTOR3 vEnd   = pTargetObj->GetPos();	vEnd.y += 0.5f;

	if( pWorld->IntersectObjLine( NULL, vStart, vEnd, FALSE, FALSE ) )
	{
		g_WndMng.PutString( prj.GetText( TID_GAME_BLOCKTARGETING ), NULL, prj.GetTextColor( TID_GAME_BLOCKTARGETING ) );
		return -1;
	}
#endif  // CLIENT

	SendActMsg( OBJMSG_STOP );

	OBJID	idTarget = ((CCtrl*)pTargetObj)->GetId();

	int nError;
	if( ( nError = m_pActMover->SendActMsg( OBJMSG_ATK_MAGIC1, idTarget ) ) == 1 )	// �ϵ���� ���� ����.
	{
		if( m_pActMover->IsFly() == FALSE )		
		{
			const float fAngle = GetDegree(pTargetObj->GetPos(), GetPos());
			if( GetAngle() != fAngle )
			{
				SetAngle( fAngle );			// ��ǥ������ ���� ����.
#ifdef __WORLDSERVER
				if( IsPlayer() == FALSE )
					g_xWSUserManager->AddSetPosAngle( this, GetPos(), GetAngle() );
#endif //__WORDSERVER
			}
			//SetAngle( GetDegree(pTargetObj->GetPos(), GetPos()) );		// ��ǥ������ ���� ����.
		}

		SetActParam( OBJACT_MAGIC_ATTACK, idTarget, nMagicPower, 0, CheckAttackDistance( pTargetObj, pItemProp->dwAttackRange ) ? AF_MAGIC : AF_MISS );		// ������ ����� �Ķ���� ���

#ifdef __CLIENT	
		if( _GetContentState( CT_NEWUI_19 ) == CS_VER1 && nMagicPower >= 1 )	// ���� 2�̻��� ���� �׼�����Ʈ �ϳ� �ö�.
		{
			if( g_WndMng.m_pWndTaskBar->m_nActionPoint < 100 )
			{
				g_WndMng.m_pWndTaskBar->m_nActionPoint++;
			}
		}

		if( IsActiveMover() )
		{
			SetActParam( OBJACT_MAGIC_ATTACK, idTarget, nMagicPower );	// ȿ�� �ĺ��� ���
			g_DPlay.SendMagicAttack( OBJMSG_ATK_MAGIC1, ( (CCtrl*)pTargetObj )->GetId(), 0, 0, nMagicPower, m_idSfxHit );
		}
#endif
		
#ifdef __WORLDSERVER
		if( _GetContentState( CT_NEWUI_19 ) == CS_VER1 && IsPlayer() == TRUE )
		{
			if( nMagicPower >= 1 && ((FLWSUser*)this)->m_playTaskBar.m_nActionPoint < 100 )	// 2�ܰ������̻� ����ϸ� �׼�����Ʈ �ö�.
			{
				((FLWSUser*)this)->m_playTaskBar.m_nActionPoint++;
			}
		}

		if( nMagicPower == 3 )		// 4�ܰ�¥���� ���� ���� 5����Ʈ ����
			IncManaPoint( -GetReqMp( pItemProp->dwReqMp ) );	

		g_xWSUserManager->AddMagicAttack( this, OBJMSG_ATK_MAGIC1, idTarget, 0, 0, nMagicPower, idSfxHit );
#endif	// __WORLDSERVER
	}
	return( nError );
}

/*--------------------------------------------------------------------------------------------------------


											Do Range Attack


----------------------------------------------------------------------------------------------------------*/
//
// ������������ �϶�!
// dwItemID : ��������� ���̵�
//
int	 CMover::DoAttackRange( CObj *pTargetObj, DWORD dwItemID, int idSfxHit )
{
	PT_ITEM_SPEC pItemProp = NULL;
	int nPower = 0;

	if( IsInvalidObj(pTargetObj) )
		return -1;
	

#ifdef __CLIENT
	if( IsActiveMover() )
	{
		// ���� ������ ��ǥ�������� �����°� ������ �޽��� ����ϰ� ����
		CWorld *pWorld = GetWorld();
		D3DXVECTOR3 vStart = GetPos();				vStart.y += 0.5f;
		D3DXVECTOR3 vEnd   = pTargetObj->GetPos();	vEnd.y += 0.5f;

		if( pWorld->IntersectObjLine( NULL, vStart, vEnd, FALSE, FALSE ) )
		{
			g_WndMng.PutString( prj.GetText( TID_GAME_BLOCKTARGETING ), NULL, prj.GetTextColor( TID_GAME_BLOCKTARGETING ) );
			
			return -1;
		}
	}
#endif  // CLIENT
	

	if( IsPlayer() )
	{
		pItemProp = GetActiveHandItemProp();			
		nPower = dwItemID;

	#ifdef __WORLDSERVER
		FLItemElem* pItemElem = m_Inventory.GetEquip( PARTS_BULLET );	// ȭ���� �ִ��� üũ
#ifdef __ADDWEAPON_CROSSBOW16
		if( pItemElem == NULL || ( pItemElem->GetProp()->dwItemKind3 != IK3_ARROW && pItemElem->GetProp()->dwItemKind3 != IK3_CROSSARROW ) )
#else // __ADDWEAPON_CROSSBOW16
		if( pItemElem == NULL || pItemElem->GetProp()->dwItemKind3 != IK3_ARROW )
#endif // __ADDWEAPON_CROSSBOW16
			return -1;
	#endif // __WORLDSERVER
	}
	else
	{
		if( dwItemID == NULL_ID || dwItemID == 0 )	
			pItemProp = GetActiveHandItemProp();		// �̷����� �� �տ� �� ����(Ȥ�� �⺻������)
		else
			pItemProp = g_xSpecManager->GetSpecItem( dwItemID );
	}
	
	if( pItemProp == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s ActiveHandItemProp = NULL" ), m_szName );
		return -1;
	}

	if( pItemProp->dwWeaponType != WT_RANGE && pItemProp->dwWeaponType != WT_RANGE_BOW )		// ����ִ� ���Ⱑ RANGE�� �ƴϸ� ����
	{
		return -1;
	}

	SendActMsg( OBJMSG_STOP );
	
	DWORD dwUseMotion = pItemProp->dwUseMotion;	
	if( dwUseMotion == NULL_ID )
	{
		if( IsPlayer() )
		{
			dwUseMotion = MTI_ATK_13 - MTI_STAND_13;//23;
		}
		else
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "%s [%d]'s dwUseMotion = NULL_ID" ), m_szName, pItemProp->dwID );
			dwUseMotion = MTI_ATK1;		// ������Ƽ�� ��Ÿ� ���� ����� �������� �ʾ����� �⺻ ������� �Ѵ�.
		}
	}

	OBJID	idTarget = ((CCtrl*)pTargetObj)->GetId();

	int nError = -1;
	if( ( nError = m_pActMover->SendActMsg( OBJMSG_ATK_RANGE1, idTarget, (int)dwUseMotion ) ) == 1 )	// ��Ÿ�����.
	{
		const float fAngle = GetDegree(pTargetObj->GetPos(), GetPos());
		if( GetAngle() != fAngle )
		{
			SetAngle( fAngle );			// ��ǥ������ ���� ����.
#ifdef __WORLDSERVER
			if( IsPlayer() == FALSE )
				g_xWSUserManager->AddSetPosAngle( this, GetPos(), GetAngle() );
#endif //__WORDSERVER
		}
		SetActParam( OBJACT_RANGE_ATTACK, idTarget, dwItemID, idSfxHit, CheckAttackDistance( pTargetObj, pItemProp->dwAttackRange ) ? AF_GENERIC : AF_MISS );	// ȿ�� �ĺ��� ���
		
	#ifdef __WORLDSERVER
		g_xWSUserManager->AddRangeAttack( this, OBJMSG_ATK_RANGE1, idTarget, dwItemID, 0, idSfxHit );
		ArrowDown( 1 );		// ȭ�� 1�� ����
	#else	// __WORLDSERVER
		if( IsActiveMover() )	// Ŭ�󿡼� �÷��̾ ����ѰŶ�� ������ ���� ����.
			g_DPlay.SendRangeAttack( OBJMSG_ATK_RANGE1, ( (CCtrl*)pTargetObj )->GetId(), dwItemID, m_idSfxHit ); 
	#endif	// __WORLDSERVER
	}
	return nError;
}

// ���� Ư������
int		CMover::DoAttackSP( CObj *pTargetObj, DWORD dwItemID )
{
	if( IsInvalidObj(pTargetObj) )
		return( -1 );
	
	PT_ITEM_SPEC pItemProp;
	if( dwItemID == NULL_ID || dwItemID == 0 )	
		pItemProp = GetActiveHandItemProp();	// �̷����� �� �տ� �� ����(Ȥ�� �⺻������)
	else
		pItemProp = g_xSpecManager->GetSpecItem( dwItemID );	// ���� �������� ������ ���� �װɷ� ������(���Ͱ� �ַ� �����)
	if( pItemProp == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s �� SkillProp�� NULL %d" ), m_szName, (int)dwItemID );
		return FALSE;
	}

	DWORD dwUseMotion = pItemProp->dwUseMotion;
	if( dwUseMotion == NULL_ID )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s�� ����%d�� dwUseMotion�� �������� �ʾҴ�" ), m_szName, pItemProp->dwID );
		dwUseMotion = MTI_ATK1;		// ������Ƽ�� ��Ÿ� ���� ����� �������� �ʾ����� �⺻ ������� �Ѵ�.
	}
	
	OBJID	idTarget = ((CCtrl*)pTargetObj)->GetId();
	
	int nError;
	if( ( nError = m_pActMover->SendActMsg( OBJMSG_SP_ATK1, idTarget, (int)dwItemID, (int)dwUseMotion ) ) == 1 )	// ��Ÿ�����.
	{
		const float fAngle = GetDegree(pTargetObj->GetPos(), GetPos());
		if( GetAngle() != fAngle )
		{
			SetAngle( fAngle );			// ��ǥ������ ���� ����.
#ifdef __WORLDSERVER
			if( IsPlayer() == FALSE )
				g_xWSUserManager->AddSetPosAngle( this, GetPos(), GetAngle() );
#endif //__WORDSERVER
		}

		//SetAngle( GetDegree(pTargetObj->GetPos(), GetPos()) );		// ��ǥ������ ���� ����.
		SetActParam( OBJACT_SP_ATTACK, idTarget, (int)dwItemID );	// ȿ�� �ĺ��� ���
		
#ifdef __WORLDSERVER
		g_xWSUserManager->AddAttackSP( this, OBJMSG_SP_ATK1, idTarget, dwItemID, 0 );
#else	// __WORLDSERVER

	//gmpbigsun: �ߵ��� �����ڿ� ���� �ߵ� effect
	DWORD dwSfxObj = pItemProp->dwSfxObj;
	if( NULL_ID != dwSfxObj )
		CreateSfx( D3DDEVICE, dwSfxObj, GetPos() );

		
#endif	// __WORLDSERVER
	}

	return( nError );
}

int		CMover::DoAttackMelee( CMover *pTarget, OBJMSG dwMsg, DWORD dwItemID )
{
	SendActMsg( OBJMSG_STOP );		// ���ڸ� ����.

	if( IsInvalidObj(pTarget) )
		return 0;
	
	PT_ITEM_SPEC pItemProp;
	if( dwItemID == NULL_ID || dwItemID == 0 )	
		pItemProp = GetActiveHandItemProp();	// �̷����� �� �տ� �� ����(Ȥ�� �⺻������)
	else
		pItemProp = g_xSpecManager->GetSpecItem( dwItemID );	// ���� �������� ������ ���� �װɷ� ������(���Ͱ� �ַ� �����)
	if( pItemProp == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s �� SkillProp�� NULL %d" ), GetName(), (int)dwItemID );
		return FALSE;
	}

	// ��� �����ϰ�.
	int nError	= SendActMsg( dwMsg, pTarget->GetId(), MAKELONG( 0, 0 ), MAKELONG( 0, 0 ) );
	if( nError > 0 )
	{
		const float fAngle = GetDegree(pTarget->GetPos(), GetPos());
		if( GetAngle() != fAngle )
		{
			SetAngle( fAngle );			// ��ǥ������ ���� ����.
		}

		const DWORD attackFlag	= ( CheckAttackDistance( pTarget, pItemProp->dwAttackRange ) && GetAttackResult( pTarget ) ) ? AF_GENERIC : AF_MISS;
		SetActParam( OBJACT_MELEE_ATTACK, pTarget->GetId(), (int)dwItemID, 0, attackFlag );	// OnAttackMelee���� ���� �ֵ��� �Ķ���� ���.
#ifdef __CLIENT
		if( IsActiveMover() )
		{
			g_DPlay.SendMeleeAttack( dwMsg, pTarget->GetId(), MAKELONG( 0, 0 ), MAKELONG( 0, 0 )/*, fVal*/ );
			PlayCombatMusic();
		}
#endif	// __CLIENT
	}
	else if( nError == 0 )
		m_pActMover->m_qMeleeAtkMsg.AddTail( new ACTMSG( dwMsg, pTarget->GetId(), 0, 0 ) );
#ifdef __WORLDSERVER
	if( nError != -2 )
		g_xWSUserManager->AddMeleeAttack( this, dwMsg, pTarget->GetId(), MAKELONG( 0, 0 ), MAKELONG( 0, 0 ) );
#endif // __WORLDSERVER

	return TRUE;	
}	



// �ֺ��� �ִ� �������� �ϳ��� ���´�.
void	CMover::DoPickupItemAround()
{
#ifdef __CLIENT
	if( m_pActMover->IsActAttack() )	return;		// ������ ���ȵ�.
	if( IsDie() )		return;						// �׾����� ���ȵ�.
	
	int nRange	= 4;	// 4m
	float fDistMin = 99999.0f, fDistSq;
	CObj* pObj, *pMinObj = NULL;
	D3DXVECTOR3 vPos = GetPos();
	D3DXVECTOR3 vDist;
	FOR_LINKMAP( GetWorld(), vPos, pObj, nRange, CObj::linkDynamic, GetLayer() )
	{
		if( pObj->GetType() == OT_ITEM )
		{
			vDist = pObj->GetPos() - vPos;		// this -> �����۱����� ����
			fDistSq = D3DXVec3LengthSq( &vDist );
			if( fDistSq < 4.0f * 4.0f )		// �����۰��� �Ÿ��� x���� �̳��ΰ��� �������.
			{
				if( fDistSq < fDistMin )	// ���� �� ���������� ����
				{
					fDistMin = fDistSq;
					pMinObj = pObj;
				}
			}
		}
	}
	END_LINKMAP

	// ������ ������ �ִ�.
	if( pMinObj )
	{
		CMD_SetUseItem( (CCtrl*)pMinObj );
	}
#endif // __CLIENT
}


// this�� Ÿ������ �ֺ��� �������� �ش�.
// ------------------------------------------
// nDmgType : ������ Ÿ�� ex) AF_MAGICSKILL
// pAttacker : ������(������) - ������ CCtrl*�� �ٲ�� �Ѵ�
// nApplyType : ������. ex) OBJTYPE_PLAYER | OBJTYPE_MONSTER
// nSkill : ����� ��ų����
// fRange : Ÿ���� �߽������� ������ ����(����)
// fTargetRatio : Ÿ�ٿ��� ����Ǵ� ������ ����
// fAroundRatio : �ֺ��� ����Ǵ� ������ ����
// (AF_MAGICSKILL, this, nSkill, 5.0f, 0.0, 1.0f )
void	CMover::SendDamageAround( int nDmgType, CMover *pAttacker, int nApplyType, int nAttackID, float fRange, float fTargetRatio, float /*fAroundRatio*/ )
{
#ifdef __WORLDSERVER
	int nRange	= 4;	// 4m
	float fDistSq;
	CObj* pObj;
	CMover *pTarget;
	D3DXVECTOR3 vPos = GetPos();
	D3DXVECTOR3 vDist;

	if( fRange <= 4.0f )
		nRange = 4;
	else if( fRange <= 8.0f )
		nRange = 8;
	else if( fRange <= 16.0f )
		nRange = 16;
	else
		nRange = 32;
	
	if( fRange <= 0 )	// ������ 0�̰ų� �����ϼ��� ����.
		FLERROR_LOG( PROGRAM_NAME, _T( "D:%s A:%s %d %f" ), GetName(), pAttacker->GetName(), nAttackID, fRange );

	SkillProp* pProp;
	if( nDmgType == AF_MAGICSKILL )
	{
		pProp = prj.GetSkillProp( nAttackID );		// UseSkill���� ����� ��ų�� ������Ƽ ����
		if( pProp == NULL )
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "%s. ��ų(%d)�� ������Ƽ�� ����." ), m_szName, nAttackID );
			return;	// property not found
		}
	}
	else
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s. ������(%d)�� ������Ƽ�� ����." ), m_szName, nAttackID );
		return;	// property not found
	}
	
	BOOL	bDamage = FALSE;
	BOOL	bTarget = FALSE;

	if( nApplyType & OBJTYPE_PLAYER )	// �������� �÷��̾��ΰ� 
	{
		FOR_LINKMAP( GetWorld(), vPos, pObj, nRange, CObj::linkPlayer, GetLayer() )
		{
			if( pObj->GetType() == OT_MOVER )
			{
				bDamage = TRUE;

				if( bDamage )
				{
					vDist = pObj->GetPos() - vPos;		// this -> Ÿ�ٱ����� ����
					fDistSq = D3DXVec3LengthSq( &vDist );
					if( fDistSq < fRange * fRange )		// Ÿ�ٰ��� �Ÿ��� fRange���� �̳��ΰ��� �������.
					{
						if( pObj == this && fTargetRatio == 0.0f )	// Ÿ�ٵ����� ������ 0�̸� �ڱ��ڽ�(Ÿ��)�� �������� ��������.
						{
						} else
						{
							if( pObj != pAttacker )		// ����Ŀ�� �˻���󿡼� ����.
							{
								pTarget = (CMover *)pObj;
								bTarget = ( pTarget == this );
								if( IsValidObj( (CObj*)pTarget ) && pTarget->IsLive() )
								{
									if( pProp->dwComboStyle == CT_FINISH )
										pTarget->m_pActMover->SendDamageForce( nDmgType, pAttacker->GetId(), (nAttackID << 16), bTarget );
									else
										pTarget->m_pActMover->SendDamage( nDmgType, pAttacker->GetId(), (nAttackID << 16), bTarget );
								}
							}
						}
					}
					
					bDamage = FALSE;	// ���� ������ ���ؼ� �ʱ�ȭ.
				} // bDamage
			}
		}
		END_LINKMAP
	}

	// �������� �����ΰ�.
	if( nApplyType & OBJTYPE_MONSTER )
	{
		FOR_LINKMAP( GetWorld(), vPos, pObj, nRange, CObj::linkDynamic, GetLayer() )
		{
			if( pObj->GetType() == OT_MOVER && ((CMover *)pObj)->IsPeaceful() == FALSE )
			{
				vDist = pObj->GetPos() - vPos;		// this -> Ÿ�ٱ����� ����
				fDistSq = D3DXVec3LengthSq( &vDist );
				if( fDistSq < fRange * fRange )		// Ÿ�ٰ��� �Ÿ��� fRange���� �̳��ΰ��� �������.
				{
					if( pObj == this && fTargetRatio == 0.0f )	// Ÿ�ٵ����� ������ 0�̸� �ڱ��ڽ�(Ÿ��)�� �������� ��������.
					{
					} else
					{
						if( pObj != pAttacker )		// �����ڴ� �˻��󿡼� ����.
						{
							pTarget = (CMover *)pObj;
							if( IsValidObj( (CObj*)pTarget ) && pTarget->IsLive() )
							{
								if( pProp->dwComboStyle == CT_FINISH )
									pTarget->m_pActMover->SendDamageForce( nDmgType, pAttacker->GetId(), (nAttackID << 16) );
								else
									pTarget->m_pActMover->SendDamage( nDmgType, pAttacker->GetId(), (nAttackID << 16) );
							}
						}
					}
				}
			}
		}
		END_LINKMAP
	}
		
#endif // WORLDSERVER		
}

// nIndex skill�� ���� �����̸� �����Ѵ�. 
// nIndex - skill�� index, ( m_tmReUseDelay, m_aJobSkill�� �ε����� ���� �ǹ� )
DWORD CMover::GetReuseDelay( int nIndex )
{
	FLASSERT( nIndex >= 0 );

	if( nIndex < 0 || nIndex >= MAX_SKILL_JOB )
	{
		return INT_MAX;
	}

	DWORD dwCur = ::timeGetTime();
	if( dwCur > m_tmReUseDelay[nIndex] )	// ����ð� ���� ���� �ð��� ũ�� 
		return 0;							// ������ ����.
	else									// �ƴϸ� 
		return m_tmReUseDelay[nIndex] - dwCur;	// ����ð� - ����ð� 
}



#ifdef __WORLDSERVER

void CMover::ActivateSystemPet( FLItemElem* pItemElem )
{
	if( GetWorld() && GetWorld()->GetID() == WI_WORLD_QUIZ )
		return;

	if( IsFly() )
	{
		static_cast<FLWSUser*>( this )->AddDefinedText( TID_GAME_CANNOT_CALL_PET_ON_FLYING );
		return;	
	}
	BOOL bFirst		= FALSE;
	if( pItemElem->m_pPet == NULL )
	{
		pItemElem->m_pPet	= new CPet;
		pItemElem->m_pPet->InitEgg();
		bFirst	= TRUE;
	}
	SetPetId( pItemElem->m_dwObjId );

	g_dpDBClient.CalluspPetLog( m_idPlayer, pItemElem->GetSerialNumber(), 0, PETLOGTYPE_CALL, pItemElem->m_pPet );
	// pet, log
//sun: ?, __PET_1024
	g_xWSUserManager->AddPetCall( this, pItemElem->m_dwObjId, pItemElem->m_pPet->GetIndex(), (BYTE)pItemElem->m_pPet->GetLevel(), pItemElem->m_pPet->GetName() );
//	g_xWSUserManager->AddPetCall( this, pItemElem->m_dwObjId, pItemElem->m_pPet->GetIndex(), (BYTE)pItemElem->m_pPet->GetLevel() );
	
	if( bFirst )
		static_cast<FLWSUser*>(this )->AddPet( pItemElem->m_pPet, PF_PET );
}

void CMover::InactivateSystemPet( FLItemElem* pItemElem )
{
	if( GetPetId() == pItemElem->m_dwObjId )
		PetRelease();
	else
		static_cast<FLWSUser*>(this )->AddDefinedText( TID_GAME_PET_NOWUSE );
}


void CMover::ActivateEatPet( FLItemElem* pItemElem )
{
	if( IsUsableItem( pItemElem ) == FALSE )
	{
		return;
	}

	PT_ITEM_SPEC pItemProp	= pItemElem->GetProp();
	if( pItemProp == NULL )
	{
		return;
	}

	if( GetWorld() && GetWorld()->GetID() == WI_WORLD_QUIZ )
	{
		return;
	}

	if( IsFly() )
	{
		static_cast<FLWSUser*>( this )->AddDefinedText( TID_GAME_CANNOT_CALL_PET_ON_FLYING );
		return;
	}
//		FLASSERT( pItemProp->dwLinkKind != NULL_ID )
#ifdef __LAYER_1015
	CMover *pEatPet	= CreateMover( GetWorld(), pItemProp->dwLinkKind, GetPos(), GetLayer() );
#else	// __LAYER_1015
	CMover *pEatPet	= CreateMover( GetWorld(), pItemProp->dwLinkKind, GetPos() );
#endif	// __LAYER_1015
	if( pEatPet )
	{
		CAIPet *pAIPet	= static_cast<CAIPet*>( pEatPet->m_pAIInterface );
		if( pAIPet )
		{
			pAIPet->SetOwner( GetId() );
			SetEatPetId( pEatPet->GetId() );

			pAIPet->SetItem( this, pItemElem );		//sun: 12, �� ���� // ���� Ȱ��ȭ �� ���� ȿ�� ����

			if( pItemElem->IsVisPet() )
			{
				if( pItemElem->GetGeneralPiercingSize() <= 0 )	// ù ��ȯ�� �����̸� �� ���� 2��(�⺻) �վ���
				{
					UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_PETVIS_SIZE, PETVIS_DEFAULT_VIS_SLOT_SZIE );
				}

				SetVisPetItem( pItemElem->m_dwObjId );	// �ش� ������ �����̸� ���� �ɷ�ġ�� �����ϰ� ����Ʈ�� �����Ѵ�.
				SetPetVisDST( pItemElem );
				pEatPet->m_dwMoverSfxId = pItemElem->GetVisPetSfxId();
				static_cast<FLWSUser*>(this)->AddActivateVisPet( pItemElem->m_dwObjId, GetEatPetId() );
			}
		}
	}
}

void CMover::InactivateEatPet( void )
{
	CMover* pEatPet		= prj.GetMover( GetEatPetId() );
	if( IsValidObj( pEatPet ) )
	{
//sun: 12, �� ����
		// ���� ��ȯ ���� �� ���� ȿ�� ����
		CAIPet *pAIPet	= static_cast<CAIPet*>( pEatPet->m_pAIInterface );
		if( pAIPet )
		{
			FLItemElem* pItemElem = GetVisPetItem();
			if( pItemElem )
			{
				ResetPetVisDST( pItemElem );
				SetVisPetItem( NULL_ID );
				static_cast<FLWSUser*>(this)->AddActivateVisPet( NULL_ID, NULL_ID );
			}

			pAIPet->ResetItem();
		}

		pEatPet->Delete();
	}
	else
	{
		FLItemElem* pItemElem = GetVisPetItem();
		if( pItemElem )
		{
			ResetPetVisDST( pItemElem );
			SetVisPetItem( NULL_ID );
			static_cast<FLWSUser*>(this)->AddActivateVisPet( NULL_ID, NULL_ID );
		}
	}
	
	SetEatPetId( NULL_ID );
}

void CMover::SetPetVisDST( FLItemElem* pItemElem )
{
	if( pItemElem && pItemElem->IsVisPet() )
	{
		AddBuff( BUFF_EQUIP, static_cast<WORD>( pItemElem->m_dwItemId ), 1, 999999999, 0, BT_TICK );	// TODO
		
		vector<BYTE> vecValidTable = GetValidVisTable( pItemElem );
		for( size_t i=0; i<pItemElem->GetGeneralPiercingSize(); i++ )
		{
			PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( pItemElem->GetGeneralPiercingItemID( i ) );
			if( vecValidTable.at( i ) == SUCCSESS_NEEDVIS && pItemProp )
			{
				for( size_t Nth = 0; Nth < MAX_ITEM_PARAM; ++Nth )
				{
					g_xApplyItemEffect->SetDestParameter( this, Nth, pItemProp );
				}
// 				g_xApplyItemEffect->SetDestParameter( this, 0, pItemProp );
// 				g_xApplyItemEffect->SetDestParameter( this, 1, pItemProp );
// 				g_xApplyItemEffect->SetDestParameter( this, 2, pItemProp );
			}
			else
			{
				if( IsPlayer() && pItemProp )
					static_cast<FLWSUser*>( this )->AddDefinedText( TID_GAME_BUFFPET_NOTREQVIS, "\"%s\"", pItemProp->szName );
			}
		}
	}
}

void CMover::ResetPetVisDST( FLItemElem* pItemElem )
{
	if( pItemElem && pItemElem->IsVisPet() )
	{
		m_buffs.RemoveBuff( BUFF_EQUIP, (WORD)( pItemElem->m_dwItemId ), FALSE );

		vector<BYTE> vecValidTable = GetValidVisTable( pItemElem );
		for( size_t i=0; i<pItemElem->GetGeneralPiercingSize(); i++ )
		{
			PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( pItemElem->GetGeneralPiercingItemID( i ) );
			if( vecValidTable.at( i ) == SUCCSESS_NEEDVIS && pItemProp )
			{
				for( size_t Nth = 0; Nth < MAX_ITEM_PARAM; ++Nth )
				{
					g_xApplyItemEffect->ResetDestParameter( this, Nth, pItemProp );
				}
// 				g_xApplyItemEffect->ResetDestParameter( this, 0, pItemProp );
// 				g_xApplyItemEffect->ResetDestParameter( this, 1, pItemProp );
// 				g_xApplyItemEffect->ResetDestParameter( this, 2, pItemProp );
			}
		}
	}
}

void CMover::ProcessVisPet()
{
// 	if( HasActivatedVisPet() )
// 	{
// 		FLItemElem* pItemElem = GetVisPetItem();
// 		if( pItemElem )
// 		{
// 			for( size_t i=0; i<pItemElem->GetGeneralPiercingSize(); i++ )
// 			{
// //				skilltime == 999999999 continue;
// 				if( pItemElem->GetGeneralPiercingItemID( i ) != 0 && time_null() >= pItemElem->GetVisPetPiercingItemKeepTime( i ) )
// 				{
// 					PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( pItemElem->GetGeneralPiercingItemID( i ) );
// 					if( pItemProp )
// 						static_cast<FLWSUser*>( this )->AddDefinedText( TID_GAME_BUFFPET_TIMEOUTVIS, "\"%s\"", pItemProp->szName );
// 					CItemUpgrade::GetInstance()->RemovePetVisItem( static_cast<FLWSUser*>( this ), i, TRUE );
// 				}
// 			}
// 		}		
// 	}

	if( !HasActivatedVisPet() ) {
		return;
	}

	FLItemElem* pItemElem = GetVisPetItem();
	if( !pItemElem ) {
		return;
	}

	for( size_t nth = 0; nth < pItemElem->GetGeneralPiercingSize(); ++nth )
	{
		if( pItemElem->GetGeneralPiercingItemID( nth ) != 0 && time_null() >= pItemElem->GetVisPetPiercingItemKeepTime( nth ) )
		{
			/************************************************************************/
			/* dwSkillTime �� 999999999 �̸� ����. �ð� ��� ���� �ʴ´�.           */
			/************************************************************************/
			PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( pItemElem->GetGeneralPiercingItemID( nth ) );
			if( pItemProp ) {
				if( pItemProp->dwSkillTime == 999999999 ) {
					continue;
				}
				static_cast<FLWSUser*>( this )->AddDefinedText( TID_GAME_BUFFPET_TIMEOUTVIS, "\"%s\"", pItemProp->szName );
			}
			CItemUpgrade::GetInstance()->RemovePetVisItem( static_cast<FLWSUser*>( this ), nth, TRUE );
		}
	}
}


#ifdef __ENCHANT_BARUNA16


void	CMover::SetNPCOwner( const OBJID OwnerOBJID )
{
	m_NPCOwnerOBJID			= OwnerOBJID;
}

OBJID CMover::GetNPCOwner() const
{
	return m_NPCOwnerOBJID;
}


FLNPCSummoner & CMover::GetBarunaNPCSummoner()
{
	return m_BarunaNPCSummoner;
}

#endif //__ENCHANT_BARUNA16


#endif	// __WORLDSERVER

vector<BYTE> CMover::GetValidVisTable( FLItemElem* pItemElem )
{
	vector<BYTE> vecTemp;
	vecTemp.resize( pItemElem->GetGeneralPiercingSize(), UNDEFINED_NEEDVIS );

	for( size_t i=0; i<vecTemp.size(); i++ )
		if( pItemElem->GetGeneralPiercingItemID( i ) == 0 )
			vecTemp.at( i ) = FAILED_BOTH_NEEDVIS;
	
	for( size_t i=0; i<vecTemp.size(); i++ )
		if( vecTemp.at( i ) == UNDEFINED_NEEDVIS )
			SetValidNeedVis( pItemElem, i, vecTemp );

	return vecTemp;
}


//BOOL CMover::SetValidNeedVis( FLItemElem* pItemElem, int nPos, vector<BYTE> & vecValid )
//{
//	if( vecValid.at( nPos ) != UNDEFINED_NEEDVIS )
//		return ( vecValid.at( nPos ) == SUCCSESS_NEEDVIS );
//
//	PT_ITEM_SPEC pPropVis = g_xSpecManager->GetSpecItem( pItemElem->GetGeneralPiercingItemID( nPos ) );
//	if( !pPropVis )
//	{
//		vecValid.at( nPos ) = FAILED_BOTH_NEEDVIS;
//		return FALSE;
//	}
//
//	if( pPropVis->dwReferTarget1 == NULL_ID && pPropVis->dwReferTarget2 == NULL_ID )
//	{
//		vecValid.at( nPos ) = SUCCSESS_NEEDVIS;
//		return TRUE;
//	}
//
//	BOOL bResult1 = FALSE, bResult2 = FALSE;
//	if( pPropVis->dwReferTarget1 == NULL_ID ) bResult1 = TRUE;
//	if( pPropVis->dwReferTarget2 == NULL_ID ) bResult2 = TRUE;
//
//	for( size_t i=0; i<pItemElem->GetGeneralPiercingSize(); i++ )
//	{
//		if( pPropVis->dwReferTarget1 == pItemElem->GetGeneralPiercingItemID( i ) )
//			bResult1 = SetValidNeedVis( pItemElem, i, vecValid );
//
//		if( pPropVis->dwReferTarget2 == pItemElem->GetGeneralPiercingItemID( i ) )
//			bResult2 = SetValidNeedVis( pItemElem, i, vecValid );
//
//		if( bResult1 && bResult2 )
//		{
//			vecValid.at( nPos ) = SUCCSESS_NEEDVIS;
//			return TRUE;
//		}
//	}
//
//	if( !bResult1 && bResult2 )
//		vecValid.at( nPos ) = FAILED_1ST_NEEDVIS;
//	else if( bResult1 && !bResult2 )
//		vecValid.at( nPos ) = FAILED_2ND_NEEDVIS;
//	else
//		vecValid.at( nPos ) = FAILED_BOTH_NEEDVIS;
//	return FALSE;
//}


BOOL CMover::SetValidNeedVis( FLItemElem* pItemElem, int nPos, vector<BYTE> & vecValid )
{
	if( vecValid.at( nPos ) != UNDEFINED_NEEDVIS )
		return ( vecValid.at( nPos ) == SUCCSESS_NEEDVIS );

	DWORD dwRemoveVisID = pItemElem->GetGeneralPiercingItemID( nPos );
	PT_ITEM_SPEC pPropVis = g_xSpecManager->GetSpecItem( dwRemoveVisID );
	if( !pPropVis )
	{
		vecValid.at( nPos ) = FAILED_BOTH_NEEDVIS;
		return FALSE;
	}

	BYTE byStatus = IsSatisfyNeedVis( pItemElem, pPropVis );
	vecValid.at( nPos ) = byStatus;

	if( SUCCSESS_NEEDVIS == byStatus )
		return TRUE;
	else
		return FALSE;
}

//BYTE CMover::IsSatisfyNeedVis( FLItemElem* pItemElemVisPet, PT_ITEM_SPEC pItemPropVis )
//{
//	if( pItemPropVis )
//	{
//		DWORD	dwNeeds[2] = { pItemPropVis->dwReferTarget1, pItemPropVis->dwReferTarget2 };
//		if( dwNeeds[0] == NULL_ID && dwNeeds[1] == NULL_ID )
//			return SUCCSESS_NEEDVIS;
//		
//		vector<BYTE> vecValidTable = GetValidVisTable( pItemElemVisPet );
//		for( size_t i=0; i<pItemElemVisPet->GetGeneralPiercingSize(); i++ )	// �ʿ� �񽺰� �������� ���� ���� �ɷ�ġ�� ������� �ʾ����Ƿ� �ɷ�ġ�� ���� �ȵȴ�.
//		{
//			DWORD dwVis = pItemElemVisPet->GetGeneralPiercingItemID( i );
//			if( dwNeeds[0] == dwVis && vecValidTable.at( i ) == SUCCSESS_NEEDVIS && time_null() < pItemElemVisPet->GetVisPetPiercingItemKeepTime( i ) )	dwNeeds[0] = NULL_ID;
//			if( dwNeeds[1] == dwVis && vecValidTable.at( i ) == SUCCSESS_NEEDVIS && time_null() < pItemElemVisPet->GetVisPetPiercingItemKeepTime( i ) )	dwNeeds[1] = NULL_ID;
//			if( dwNeeds[0] == NULL_ID && dwNeeds[1] == NULL_ID )
//				return SUCCSESS_NEEDVIS;
//		}
//			
//		if( dwNeeds[0] != NULL_ID && dwNeeds[1] == NULL_ID )
//			return FAILED_1ST_NEEDVIS;
//		else if( dwNeeds[0] == NULL_ID && dwNeeds[1] != NULL_ID )
//			return FAILED_2ND_NEEDVIS;
//	}
//
//	return FAILED_BOTH_NEEDVIS;
//}


BYTE CMover::IsSatisfyNeedVis( FLItemElem* pItemElemVisPet, PT_ITEM_SPEC pItemPropVis )
{
	if( pItemPropVis )
	{
		// Ư�� ���� �������� ����ϱ� ���� ���� �׷� ������ ��´�.
		PT_VIS_CHECK_LIST pNeeds = NULL;
		pNeeds = g_xSpecManager->GetVisCheckList( pItemPropVis->dwID );
		if( NULL == pNeeds )
		{
			// ����Ʈ�� ���� ���� ������. ���� ������ ���� ���������� ����
			return SUCCSESS_NEEDVIS;
		}

		// ���� �κ��丮 �����ŭ for���� ������.
		bool bCheck[MAX_VIS_CONDITION_COUNT] = { 0, };
		for ( size_t iCnt = 0 ; iCnt < pItemElemVisPet->GetGeneralPiercingSize() ; ++iCnt )
		{
			// �꿡 �ִ� ���� ������ ID�� ��´�. ( ������� )
			DWORD dwPiercingItemID = pItemElemVisPet->GetGeneralPiercingItemID( iCnt );
			if( NULL == dwPiercingItemID )
				continue;

			// ������ SPEC Ȯ��
			PT_ITEM_SPEC pSpec = g_xSpecManager->GetSpecItem( dwPiercingItemID );
			if( NULL == pSpec )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID ITEM ID:(%d) ]" ), dwPiercingItemID );
				return FAILED_BOTH_NEEDVIS;
			}

			// �꿡 �����Ǿ� �ִ� ���� �������� �׷� ������ ���´�.
			DWORD dwGroupID = g_xSpecManager->GetVisGroupID( dwPiercingItemID );
			if( NULL_ID == dwGroupID )
				continue;

			// �꿡 �����Ǿ� �ִ� ���� �������� �׷� ������ ����ϱ� ����
			// ������ ���� ������ �´��� Ȯ������
			if( pNeeds->dwGroupID[0] == dwGroupID || NULL_ID == pNeeds->dwGroupID[0] )
				bCheck[0] = true;

			if( pNeeds->dwGroupID[1] == dwGroupID || NULL_ID == pNeeds->dwGroupID[1] )
				bCheck[1] = true;	
		}

		// ����� ���� ���ϰ� ����
		if( true == bCheck[0] && true == bCheck[1] )
			return SUCCSESS_NEEDVIS;
		if( false == bCheck[0] && true == bCheck[1] )
			return FAILED_1ST_NEEDVIS;
		else if( true == bCheck[0] && false == bCheck[1] )
			return  FAILED_2ND_NEEDVIS;
		else
			return FAILED_BOTH_NEEDVIS;
	}

	return FAILED_BOTH_NEEDVIS;
}