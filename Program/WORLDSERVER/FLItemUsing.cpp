
#include "stdafx.h"
#include "FLItemUsing.h"

#include "ItemUpgrade.h"

#include "../_Common/FunnyCoin.h"
#include "../_Common/FLSkillSystem.h"
#include "../_Common/ScriptHelper.h"
#include "../_Common/Party.h"
#include "../_Common/WorldMng.h"
#include "FLTeleportMapUserPointManager.h"
#include "FLItemUsingActiveUI.h"

#include <defineHonor.h>
#include "FLItemAction.h"

extern	CPartyMng			g_PartyMng;
extern	CWorldMng			g_WorldMng;


FLItemUsing*	FLItemUsing::GetInstance()
{
	static FLItemUsing kItemUsing;
	return &kItemUsing;
}

FLItemUsing::FLItemUsing()
{
}

FLItemUsing::~FLItemUsing()
{
}

void	FLItemUsing::HandleDoUseItem( FLWSUser* pUser, CAr & ar )
{
	DWORD dwData = 0;
	OBJID objid = 0;
	int	  nPart = 0;

	ar >> dwData >> objid >> nPart;
	if( nPart >= MAX_HUMAN_PARTS )	
		return;

	if( IsValidObj( pUser ) == FALSE )
	{
		return;
	}

	if( pUser->IsDie() == TRUE )
	{
		return;
	}

	WORD nId = HIWORD( dwData );
	if( nPart > 0 )
	{
		FLItemElem* pItemElem	= pUser->m_Inventory.GetAtId( nId );
		if( IsUsableItem( pItemElem ) == FALSE )
		{
			return;
		}

		if( pUser->m_Inventory.IsEquip( nId ) )
		{
			if( pItemElem != pUser->m_Inventory.GetEquip( nPart ) )
				return;
		}
	}

	FLItemElem* pItemElem	= pUser->m_Inventory.GetAtId( nId );
	if( IsUsableItem( pItemElem ) )
	{
		PT_ITEM_SPEC pItemProp	= pItemElem->GetProp();
		if( pItemProp && pItemProp->dwParts == PARTS_RIDE )
		{
			if( !pUser->m_Inventory.IsEquip( nId ) )
			{
				FLOAT fVal;
				ar >> fVal;
				if( fVal != pItemProp->fFlightSpeed )
				{
					pUser->AddDefinedText( TID_GAME_MODIFY_FLIGHT_SPEED );
					return;
				}
			}
		}
	}

	OnDoUseItem( pUser, dwData, objid, nPart );
}

void	FLItemUsing::OnDoUseItem( FLWSUser* pUser, DWORD dwData, OBJID objid, int nPart )
{
	DWORD dwId = HIWORD( dwData );

	if( CheckUseItem( pUser, dwId ) == FALSE )
	{
		return;
	}

	FLItemElem* pItemElem	= pUser->m_Inventory.GetAtId( dwId );
	if( IsUsableItem( pItemElem ) == FALSE )
	{
		return;
	}

	PT_ITEM_SPEC pItemSpec	= pItemElem->GetProp();
	if( pItemSpec == NULL )
	{
		return;
	}

	if( pItemElem->CanUseLevel( static_cast<CMover*>( pUser ) ) == false )
	{
		if( pItemSpec->dwParts != NULL_ID )
		{
			pUser->AddDefinedText( TID_GAME_REQLEVEL, "\"%d\"", pItemElem->GetMinLimitLevel() );
		}
		else
		{
			TCHAR szMessage[32] = { 0, };
			FLSPrintf( szMessage, _countof( szMessage ), "%d - %d", pItemSpec->nMinLimitLevel, pItemSpec->nMaxLimitLevel );
			pUser->AddDefinedText( TID_GAME_EXP_NOTUSELEVEL, "\"%s\"", szMessage );
		}

		return;
	}

	if( pItemSpec->dwItemKind3 == IK3_VIS )
	{
		CItemUpgrade::GetInstance()->SetPetVisItem( pUser, pItemElem->m_dwObjId );
		return;
	}
	else if( pItemSpec->dwID == ITEM_INDEX( 26855, II_SYS_SYS_VIS_KEY01 ) )
	{
		CItemUpgrade::GetInstance()->PetVisSize( pUser, pItemElem->m_dwObjId );
		return;
	}
	else if( pItemSpec->dwID == ITEM_INDEX( 26910, II_SYS_SYS_SCR_PET_MAGIC ) )
	{
		CItemUpgrade::GetInstance()->TransFormVisPet( pUser, pItemElem->m_dwObjId );
		return;
	}
	else if( pItemSpec->dwID == ITEM_INDEX( 6318, II_SYS_SYS_EVE_FBOX ) )
	{
		DoUseEveFBOX( pUser, pItemElem );
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	FLPackItemProp::_T_ELEM * pPackItemElem = PACK_ITEM_PROP().Find( pItemSpec->dwID );
	if( pPackItemElem != NULL )
	{
		DoUsePackItem( pUser, pItemElem, pPackItemElem );
		return;
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	if( DoUseGiftbox( pUser, pItemElem, pItemSpec->dwID ) == TRUE )
	{
		return;
	}
	//////////////////////////////////////////////////////////////////////////

	DoUseItem( pUser, dwData, objid, nPart );
}

BOOL	FLItemUsing::CheckUseItem( FLWSUser* pUser, DWORD dwItemObjId )
{
	if( IsValidObj( pUser ) == FALSE ) {
		return FALSE;
	}

	if( pUser->IsUsableState( dwItemObjId ) == FALSE ) {
		return FALSE;
	}

	CWorld* pWorld = pUser->GetWorld();
	if( pWorld == NULL ) {
		return FALSE;
	}
	
	if( pWorld->GetID() == WI_WORLD_QUIZ ) {
		return FALSE;
	}

	FLItemElem* pItemElem = pUser->m_Inventory.GetAtId( dwItemObjId );
	if( !IsUsableItem( pItemElem ) ) {
		return FALSE;
	}

	const PT_ITEM_SPEC pItemSpec	= pItemElem->GetProp();
	if( pItemSpec == NULL ) {
		return FALSE;
	}

	// �Ⱓ ����
	if( pItemElem->IsFlag( FLItemElem::expired ) ) {
		const DWORD textID	= pItemSpec->dwItemKind3 == IK3_EGG ? TID_GAME_PET_DEAD : TID_GAME_ITEM_EXPIRED;
		pUser->AddDefinedText( textID );
		return FALSE;
	}

	return TRUE;
}

BOOL	FLItemUsing::DoUseItem( FLWSUser* pUser, DWORD dwData, DWORD /*dwFocusId*/, int nPart  )
{
	if( pUser->IsDie() )
		return FALSE;

	if( pUser->m_bAllAction == FALSE )
		return FALSE;

	WORD dwType = LOWORD( dwData );
	UNREFERENCED_PARAMETER( dwType );
	WORD dwId   = HIWORD( dwData );

	FLItemBase* pItemBase = pUser->GetItemId( dwId ); 
	if( IsUsableItem( pItemBase ) == FALSE )
		return FALSE;

	FLItemElem* pItemElem = (FLItemElem*)pItemBase;
	PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( pItemBase->m_dwItemId );
	if( pItemProp == NULL )
	{
		return FALSE;
	}

	// �������ʿ��� ������ ���
	CWorld* pWorld	= pUser->GetWorld();
	if( pWorld && pWorld->GetID() == WI_WORLD_GUILDWAR )
	{
		if( pUser->m_nGuildCombatState == 0 )
		{
			pUser->AddText( prj.GetText( TID_GAME_GUILDCOMBAT_STANDS_NOTUSEITEM ) );	//"�����ؾ��� : ���߼������� �������� ����Ҽ� �����ϴ�" );
			return FALSE;
		}
	}

	if( pUser->GetAdjParam( DST_CHRSTATE ) & CHS_SETSTONE )
		return FALSE;

	if( pItemProp->dwParts != NULL_ID )
	{
		// armor, weapon
		if( pUser->m_pActMover->IsActAttack() == FALSE )	// �����߿� ��� ���ٲ�.
		{
			DoUseEquipmentItem( pUser, pItemElem, dwId, nPart );
		}
		// ���� �������� Ż�� �����ؾ��ϹǷ� �Ⱓ ���ῡ ���� ��� ���� ����.
	}
	else	// �Ϲ����� ������ ���
	{
		if( DoUseGenericItem( pUser, pItemElem, dwId, nPart ) == FALSE )
		{
			return FALSE;
		}

		CompleteUseItem( pUser, dwId );
	}

	return TRUE;
}

// ���������� ����Ѵ�.( ���� -> Ż��, Ż�� -> ���� )
// ����: pItemElem�� IsUsableItem���� �˻�� ���¶�� �����Ѵ�.
BOOL	FLItemUsing::DoUseEquipmentItem( FLWSUser* pUser, FLItemElem* pItemElem, DWORD dwId, int nPart )
{
	BOOL bEquip	= !pUser->m_Inventory.IsEquip( dwId );	// equip
	if( bEquip && pUser->IsItemRedyTime( pItemElem->GetProp(), pItemElem->m_dwObjId, TRUE ) == FALSE )
		return FALSE;

	// chipi_090731 - Ŭ���̾�Ʈ ������ ���� �߻�
	PT_ITEM_SPEC pItemProp = pItemElem->GetProp();
	if( !pItemProp )
		return FALSE;

	if( static_cast< int >( pItemProp->dwParts ) != nPart && nPart != -1)
	{
		FLItemElem* pTempElem = pUser->m_Inventory.GetAt( nPart );
		if( !pTempElem || pTempElem->GetProp()->dwParts != pItemProp->dwParts )
			return FALSE;
	}
	// END chipi_090731 - Ŭ���̾�Ʈ ������ ���� �߻�

	pUser->EquipItem( pItemElem, bEquip, nPart );

	return TRUE;
}

BOOL	FLItemUsing::DoUseGenericItem( FLWSUser* pUser, FLItemElem* pItemElem, DWORD dwID, int nPart )
{
	PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( pItemElem->m_dwItemId );
	if( pItemProp == NULL )
	{
		return FALSE;
	}

	if( pItemProp->dwItemKind3 == IK3_LINK )
	{
		pItemProp	= g_xSpecManager->GetSpecItem( pItemProp->dwActiveSkill );
		if( pItemProp == NULL )
		{
			return FALSE;
		}
	}


	if( pUser->IsItemRedyTime( pItemProp, pItemElem->m_dwObjId, TRUE ) == FALSE )
		return FALSE;

	DWORD dwGroup = pUser->m_cooltimeMgr.GetGroup( pItemProp );
	if( dwGroup )
	{
		if( pUser->m_cooltimeMgr.CanUse( dwGroup ) == FALSE )
		{
			return FALSE;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// mirchang_101101
	if( pItemProp->dwItemKind1 == IK1_EFFECT )
	{
		int nResult = g_xUseEffectItem->OnDoUseEffectItem( pUser, pItemElem );

		if( nResult != FLUseEffectItem::ERR_SUCCESS_USE )
		{
			return FALSE;
		}
	}
	else if( pItemProp->dwItemKind1 == IK1_ACTIVE )
	{
		return FALSE;
	}
	else if( pItemProp->dwItemKind1 == IK1_ACTIVEUI )
	{
		g_pItemUsingActiveUI->OnDoUseActiveUIItem( pUser, dwID );
		return FALSE;
	}
	else if( pItemProp->dwItemKind1 == IK1_PASSIVE )
	{
		return FALSE;
	}
	else
	{
		// �� ���ȭ ������
		switch( pItemProp->dwItemKind2 )
		{
		case IK2_GUILDHOUSE_FURNITURE:
		case IK2_GUILDHOUSE_NPC:
		case IK2_GUILDHOUSE_PAPERING:
			{
				GH_Fntr_Info gfi( pItemProp->dwID );
				if( GuildHouseMng->SendWorldToDatabase( pUser, GUILDHOUSE_PCKTTYPE_LISTUP, gfi/*GH_Fntr_Info( pItemProp->dwID )*/ ) )
					g_DPSrvr.PutItemLog( pUser, "f", "GUILDHOUSE_LISTUP", pItemElem, 1 );
				else
					return FALSE;
			}
			break;
		case IK2_GUILDHOUES_COMEBACK:
			{
				if( CRainbowRaceMng::GetInstance()->IsEntry( pUser->m_idPlayer ) == TRUE )
				{
					pUser->AddDefinedText( TID_GAME_RAINBOWRACE_NOTELEPORT );
					return FALSE;
				}

				if( !GuildHouseMng->EnteranceGuildHouse( pUser, pItemProp->dwID ) )
					return FALSE;
			}
			break;

			//sun: 13, �Ͽ�¡ �ý���
		case IK2_FURNITURE:
		case IK2_PAPERING:
			{
				if(	CHousingMng::GetInstance()->ReqSetFurnitureList( pUser, pItemElem->m_dwItemId ) )
				{
					// �Ͽ�¡ ����Ʈ �߰� �α�
					LogItemInfo aLogItem;
					//aLogItem.Action	= "f";
					//aLogItem.SendName	= pUser->GetName();
					//aLogItem.RecvName = "HOUSING_USE";
					FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "f" );
					FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
					FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "HOUSING_USE" );
					aLogItem.WorldId	= pUser->GetWorld()->GetID();
					aLogItem.Gold	= pUser->GetGold();
					aLogItem.Gold2	= pUser->GetGold();
					
					g_DPSrvr.OnLogItem( aLogItem, pItemElem );
				}
				else
				{
					return FALSE;
				}
			}
			break;
		case IK2_TOCASH:
			{
				if( !CFunnyCoin::GetInstance()->DoUseFunnyCoin( pUser, pItemElem ) )
					return FALSE;
			}
			break;

		case IK2_WARP:
			return DoUseItemWarp( pUser, pItemProp, pItemElem );
			//sun: 11, ä�� �ý���

		case IK2_BUFF2:
			{
				if( pUser->IsDoUseBuff( pItemProp ) != 0 )
					return FALSE;

				CTime tm	= CTime::GetCurrentTime() + CTimeSpan( 0, 0, pItemProp->dwAbilityMin, 0 );
				time_t t	= (time_t)( tm.GetTime() );
				// wID: dwItemId
				// dwLevel
				// t ���� �� level�� ����? �ùٸ��ǰ�?
				pUser->AddBuff( BUFF_ITEM, (WORD)( pItemElem->m_dwItemId ), t, 0, 0, BT_TICK );	// TODO
			}
			break;

		case IK2_BUFF_TOGIFT:	// ���� �ð� ����Ǹ� ������ �����ϴ� �ý���(IK2_BUFF_TOGIFT)
		case IK2_BUFF:
			{
				if( pUser->IsDoUseBuff( pItemProp ) != 0 )
					return FALSE;

				g_xApplyItemEffect->DoApplyEffect( pUser, pUser, pItemProp );

				//sun: 8�� ���� ��ȯ Neuz, World, Trans
				if( pItemProp->dwItemKind3 == IK3_ANGEL_BUFF )
				{
					pUser->m_nAngelExpLog = 0;

					// ���� ���� �α�
					LogItemInfo aLogItem;
					//aLogItem.Action = "&";
					//aLogItem.SendName = pUser->GetName();
					//aLogItem.RecvName = "ANGEL_SUMMON";
					FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "&" );
					FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
					FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "ANGEL_SUMMON" );
					aLogItem.WorldId = pUser->GetWorld()->GetID();
					aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
					//aLogItem.ItemName = pItemProp->szName;
					FLSPrintf( aLogItem.kLogItem.szItemName, _countof( aLogItem.kLogItem.szItemName ), "%d", pItemProp->dwID );
					g_DPSrvr.OnLogItem( aLogItem );

					pUser->m_nAngelExp = 0;
					pUser->m_nAngelLevel = pUser->GetLevel() + 1;
					pUser->AddAngelInfo();
				}

			}
			break;

		case IK2_TEXT: //�ؽ�Ʈ ���� ó�� 
			// ����Ʈ�� ������ ����Ʈ ���� 
			if( pItemProp->dwQuestId && pItemElem->m_byItemResist == FALSE )
			{
				::__SetQuest( pUser->GetId(), pItemProp->dwQuestId );
				pItemElem->m_byItemResist = TRUE;
				pUser->UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_IR, pItemElem->m_byItemResist );	
			}
			break;

		case IK2_SYSTEM:
			{
				int nResult = 0;
				nResult = DoUseItemSystem( pUser, pItemProp, pItemElem, nPart );
				{
					if( 0 < nResult )
					{
						if( nResult == 2 )
						{
							pUser->AddDefinedText( TID_GAME_LIMITED_USE, "" ); // ������̾ ����Ҽ� ����
						}
						else if( nResult == 3 )
						{
							pUser->AddDefinedText( TID_GAME_NOTOVERLAP_USE, "" ); // �� �����۰��� �ߺ��Ͽ� ����� �� �����ϴ�
						}
						return FALSE;
					}
					else
					{
						if( pItemElem->m_dwItemId == ITEM_INDEX( 26316, II_SYS_SYS_SCR_RECCURENCE_LINK ) )
						{
							g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemElem, pItemProp );
						}

						//////////////////////////////////////////////////////////////////////////
						//	mulcom	BEGIN100125	�̺�Ʈ�� ����ų �� �̺�Ʈ�� ������Ʈ ��� ������ ���� �α� �߰�
						//						( e-mail : [����] ������ �α� �߰� ( 2010-01-25 17:33 ) ���� )
						else if( pItemElem->m_dwItemId == ITEM_INDEX( 10434, II_SYS_SYS_SCR_RECCURENCE ) && pItemElem->m_bCharged != TRUE )
						{
							g_DPSrvr.PutItemLog( pUser, "w", "USE_RECCURENCE_ITEM", pItemElem, 1 );
						}
						else if(  pItemElem->m_dwItemId == ITEM_INDEX( 10211, II_CHR_SYS_SCR_RESTATE ) && pItemElem->m_bCharged != TRUE )
						{
							g_DPSrvr.PutItemLog( pUser, "w", "USE_RESTATE_ITEM", pItemElem, 1 );
						}
						//	mulcom	END100125	�̺�Ʈ�� ����ų �� �̺�Ʈ�� ������Ʈ ��� ������ ���� �α� �߰�
						//////////////////////////////////////////////////////////////////////////
					}
				}
			}
			break;

		case IK2_BLINKWING:
			return DoUseItemBlinkWing( pUser, pItemProp, pItemElem );

		case IK2_REFRESHER:
			{
				if( pItemProp->dwCircleTime != (DWORD)-1 )
				{
					if( pUser->IsSMMode( SM_MAINTAIN_MP ) == FALSE && pUser->SetSMMode( SM_MAINTAIN_MP, pItemProp->dwCircleTime ) )
					{
						pUser->SetPointParam( DST_MP, pUser->GetMaxManaPoint() );
						if( pUser->GetAdjParam( DST_IGNORE_DMG_PVP ) > 0 )
						{
							pUser->ResetDestParam( DST_IGNORE_DMG_PVP, 0, TRUE );
							pUser->RemoveDstParamBuffs( DST_IGNORE_DMG_PVP );
						}
					}
					else
					{
						pUser->AddDefinedText( TID_GAME_LIMITED_USE, "" ); //   
						return FALSE;
					}
					g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemElem, pItemProp );
				}
				else
				{
					if( DoUseItemFood( pUser, pItemProp, pItemElem ) == FALSE )
					{
						return FALSE;
					}
					else
					{
						if( pUser->GetAdjParam( DST_IGNORE_DMG_PVP ) > 0 )
						{
							pUser->ResetDestParam( DST_IGNORE_DMG_PVP, 0, TRUE );
							pUser->RemoveDstParamBuffs( DST_IGNORE_DMG_PVP );
						}
					}
				}
			}
			break;

		case IK2_POTION:
			if( pItemProp->dwCircleTime != (DWORD)-1 )	// ���������� dwCircleTime�� �����. ���߿� dwSkillTime���� ���տ���.
			{
				if( pUser->IsSMMode( SM_MAINTAIN_FP ) == FALSE && pUser->SetSMMode( SM_MAINTAIN_FP, pItemProp->dwCircleTime ) )
				{
					pUser->SetPointParam( DST_FP, pUser->GetMaxFatiguePoint() );
					if( pUser->GetAdjParam( DST_IGNORE_DMG_PVP ) > 0 )
					{
						pUser->ResetDestParam( DST_IGNORE_DMG_PVP, 0, TRUE );
						pUser->RemoveDstParamBuffs( DST_IGNORE_DMG_PVP );
					}
				}
				else
				{
					pUser->AddDefinedText( TID_GAME_LIMITED_USE, "" ); //   
					return FALSE;
				}
				g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemElem, pItemProp );
			}
			else
			{
				if( DoUseItemFood( pUser, pItemProp, pItemElem ) == FALSE )
				{
					return FALSE;
				}
				else
				{
					if( pUser->GetAdjParam( DST_IGNORE_DMG_PVP ) > 0 )
					{
						pUser->ResetDestParam( DST_IGNORE_DMG_PVP, 0, TRUE );
						pUser->RemoveDstParamBuffs( DST_IGNORE_DMG_PVP );
					}
				}
			}
			break;

		case IK2_FOOD:
			if( DoUseItemFood( pUser, pItemProp, pItemElem ) == FALSE )
			{
				return FALSE;
			}
			else
			{
				if( pUser->GetAdjParam( DST_IGNORE_DMG_PVP ) > 0 )
				{
					pUser->ResetDestParam( DST_IGNORE_DMG_PVP, 0, TRUE );
					pUser->RemoveDstParamBuffs( DST_IGNORE_DMG_PVP );
				}
			}
			break;

		case IK2_AIRFUEL:	// ���࿬���
			{
				PT_ITEM_SPEC pRideProp = g_xSpecManager->GetSpecItem( pUser->m_dwRideItemIdx );	// ���ڷ� ������Ƽ
				if( pRideProp && pUser->m_pActMover->IsFly() )
				{
					if( pItemProp->dwItemKind3 == IK3_ACCEL )	// ���� ����
					{
						pUser->m_tmAccFuel = pItemProp->dwAFuelReMax * 1000;;	// ���ʰ� �����Ҽ� �ֳ�.
					}
					else
					{	// ���࿬��
						if( pRideProp->dwItemKind3 == IK3_BOARD && pItemProp->dwItemKind3 == IK3_CFLIGHT )	// Ÿ���ִ°� ���� / ����Ϸ��°� ���ڷ��
						{
							pUser->AddDefinedText( TID_GAME_COMBFUEL, "" );	// �̰� ���ڷ���̿�~
							return FALSE;
						}
						else if( pRideProp->dwItemKind3 == IK3_STICK && pItemProp->dwItemKind3 == IK3_BFLIGHT )	// Ÿ���ִ°� ���ڷ� / ����Ϸ��°� �����
						{
							pUser->AddDefinedText( TID_GAME_BOARDFUEL, "" );	// �̰� ������̿�~
							return FALSE;
						}
						pUser->m_nFuel += pItemProp->dwFuelRe;		// ������.
						if( (DWORD)( pUser->m_nFuel ) >= pRideProp->dwFFuelReMax )	// ���ڷ� �����뺸�� ũ�� ��.
							pUser->m_nFuel = pRideProp->dwFFuelReMax;
					}
					pUser->AddSetFuel( pUser->m_nFuel, pUser->m_tmAccFuel );		// ��ȭ�� ���� ����. - SetPointParam���� ���ĵ� ��������?
				}
				else
				{
					return FALSE;
				}
			}
			break;

		case IK2_GMTEXT:
			{
				BOOL bGMTime = TRUE;
				if( pItemProp->dwSkillTime != NULL_ID )		// ���ӽð��� �ִ� ������
				{
					if( pItemProp->dwItemKind3 == IK3_TEXT_DISGUISE )	// ���� �������� �����߿��� ����Ҽ� ����.
					{
						if( pUser->IsFly() )
						{
							pUser->AddDefinedText( TID_PK_FLIGHT_NOUSE, "" ); // ���� �߿��� ����� �� �����ϴ�.
							bGMTime = FALSE;
						}
						else if( CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon( pUser->GetWorld()->GetID() ) )
						{
							pUser->AddDefinedText( TID_GAME_INSTANCE_DISGUISE01 );
							bGMTime = FALSE;
						}
					}
					if( bGMTime )
					{
						if( !pUser->HasBuffByIk3( pItemProp->dwItemKind3 ) )
						{
							g_xApplyItemEffect->DoApplyEffect( pUser, pUser, pItemProp );
						}
						else
						{
							pUser->AddDefinedText( TID_GAME_LIMITED_USE, "" ); // ������̾ ����Ҽ� ����
							bGMTime = FALSE;
						}
					}
				}

				if(  pItemProp->dwID == ITEM_INDEX( 25999, II_SYS_SYS_GM_NODISGUISE ) )
				{
					if( pUser->HasBuffByIk3( IK3_TEXT_DISGUISE ) )
						pUser->RemoveIk3Buffs( IK3_TEXT_DISGUISE );
					else
						bGMTime = FALSE;
				}

				if( bGMTime )
				{
					if( pItemProp->dwID != ITEM_INDEX( 25999, II_SYS_SYS_GM_NODISGUISE ) )
					{
						char szGMCommand[64] = {0,};
						CString szGMText = pItemProp->szTextFileName;
						szGMText.Replace( '(', '\"' );
						szGMText.Replace( ')', '\"' ); 
						int nGMCount = szGMText.Find( "/", 1 );

						if( nGMCount != -1 )
							FLStrncpy( szGMCommand, _countof( szGMCommand ), szGMText, nGMCount );
						else
							FLStrcpy( szGMCommand, _countof( szGMCommand ), szGMText );

						ParsingCommand( szGMCommand, pUser, TRUE );
					}
				}
				else
				{
					return FALSE;
				}
			}
			break;

		case IK2_SKILL:
			{
//				BOOL bUseItem = FALSE;
				DWORD dwActiveSkill = pItemProp->dwActiveSkill;
				if( dwActiveSkill != NULL_ID )		// �߰� �ߵ� ��ų�� �ִ�.
				{
					if( pItemProp->dwActiveSkillRate == NULL_ID || xRandom(100) < pItemProp->dwActiveSkillRate )	// �ߵ�Ȯ���� �ɷȴ°�.
					{
						SkillProp* pSkillProp;
						pSkillProp = (SkillProp *)prj.GetSkillProp( dwActiveSkill );
						if( pSkillProp )
						{
							if( pSkillProp->dwUseChance == WUI_TARGETINGOBJ )	// Ÿ�ٿ��� ����ϴ� ���.
							{
								DWORD idTarget = pUser->m_idSetTarget;
								CMover *pTarget = prj.GetMover( idTarget );
								if( IsValidObj( pTarget ) )
								{
									g_cSkillSystem->DoActiveSkill( pUser, dwActiveSkill, pItemProp->dwActiveSkillLv, pTarget );	// dwActiveSkill �ߵ�.
								}
							}
							else if( pSkillProp->dwUseChance == WUI_NOW )		// �ڽſ��Ը� ����ϴ� ���.
							{
								g_cSkillSystem->DoActiveSkill( pUser, dwActiveSkill, pItemProp->dwActiveSkillLv, pUser );	// �ߵ�
							}
						}
						else
						{
							FLERROR_LOG( PROGRAM_NAME, _T( "DoUseItem, IK2_SKILL, item %s -> %d skill not prop" ), pItemProp->szName, dwActiveSkill );
						}
					}
				}
			}
			break;

		case IK2_TELEPORTMAP:	// �ڷ���Ʈ���� ������
			{
				if( g_pTeleportMapUserPointMng->OnDoUseTeleportMapItem( pUser, dwID, pItemProp ) == false )
				{
					return FALSE;
				}
			}
			break;

		default:	// @@@@ TODO ī�ε� ���� �� �������� �� ����Ÿ�Կ� ���� �÷� �߰�
			{
				switch( pItemProp->dwItemKind3 )
				{
				case IK3_EGG:
					DoUseSystemPet( pUser, pItemElem );
					break;
				case IK3_PET:
					DoUseEatPet( pUser, pItemElem );
					break;
				case IK3_SUMMON_NPC:
					DoUseSummonNPC( pUser, *pItemElem );
					break;
				default:
					if( pItemProp->dwSkillTime != NULL_ID )		// ���ӽð��� �ִ� ������
					{
						g_xApplyItemEffect->DoApplyEffect( pUser, pUser, pItemProp );
					}
					break;
				}
			}
			break;
		}	// switch ik2

// 		switch( pItemProp->dwItemKind3 )
// 		{
// 		case IK3_EGG:
// 			DoUseSystemPet( pUser, pItemElem );
// 			break;
// 		case IK3_PET:
// 			DoUseEatPet( pUser, pItemElem );
// 			break;
// #ifdef __ENCHANT_BARUNA16
// 		case IK3_SUMMON_NPC:
// 			DoUseSummonNPC( pUser, *pItemElem );
// 			break;
// #endif //__ENCHANT_BARUNA16
// 		}	// switch ik3
	}

	return TRUE;
}

// pItemElem�� ����ϴµ� ���� ������ PACKITEMELEM�� �����Ѵ�.
// pItemElem�� ����� ������ 
void	FLItemUsing::DoUsePackItem( FLWSUser* pUser, FLItemElem* pItemElem, FLPackItemProp::_T_ELEM * pPackItemElem )
{
	time_t t	= 0;
	if( pPackItemElem->nSpan )	// minutes
	{
		CTime time	= CTime::GetCurrentTime() + CTimeSpan( 0, 0, pPackItemElem->nSpan, 0 );
		t	= (time_t)( time.GetTime() );
	}

	vector<T_ADDITEM_INFO> vecAddItemInfo;
	vecAddItemInfo.clear();

	for( size_t i = 0; i < pPackItemElem->vecItemInfo.size(); ++i )
	{
		T_ADDITEM_INFO tAddItemInfo;

		tAddItemInfo.dwItemId	= pPackItemElem->vecItemInfo.at(i).dwItemID;
		tAddItemInfo.nItemNum	= pPackItemElem->vecItemInfo.at(i).nNum;

		vecAddItemInfo.push_back( tAddItemInfo );
	}

	if( pUser->m_Inventory.IsEnoughEmptySpace( vecAddItemInfo ) == false )
	{
		pUser->AddDefinedText( TID_GAME_LACKSPACE );
		return;
	}

	for( size_t i = 0; i < pPackItemElem->vecItemInfo.size(); ++i )
	{
		FLItemElem itemElem;

		itemElem.m_dwItemId		= pPackItemElem->vecItemInfo.at(i).dwItemID;
		itemElem.SetAbilityOption( pPackItemElem->vecItemInfo.at(i).nAbilityOption );
		itemElem.m_nItemNum		= pPackItemElem->vecItemInfo.at(i).nNum;

		itemElem.m_bCharged		= itemElem.GetProp()->bCharged;
		itemElem.m_dwKeepTime	= (DWORD)t;

		if ( itemElem.IsPet() ) // packItem�� �� �������� ������� �� ó���մϴ�.
		{
			SAFE_DELETE( itemElem.m_pPet );
			itemElem.m_pPet= new CPet;
			int nPetLevel = itemElem.m_nAbilityOption;
			int nItemKind = itemElem.m_pPet->GetKind(itemElem.m_dwItemId);		// ȣ���� 0, ���� 1, �䳢 2, ����ȣ 3, ��� 4, �׸��� 5, �� 6
			itemElem.m_pPet->SetKind(nItemKind);
			itemElem.m_pPet->SetLevel(nPetLevel);
			itemElem.m_pPet->SetExp(0);
			itemElem.m_pPet->SetEnergy( itemElem.m_pPet->GetMaxEnergy() );
			BYTE anAvail[PL_MAX - 1]	= {1,3,5,7,9};
			for( BYTE i = PL_D; i <= nPetLevel; i++ )
				itemElem.m_pPet->SetAvailLevel( i, anAvail[i-1] );
			for( BYTE i = nPetLevel + 1; i <= PL_S; i++ )
				itemElem.m_pPet->SetAvailLevel( i, 0 );

			itemElem.m_pPet->SetLife( 1 );
		}

		if( pItemElem->IsOwnState() )
		{
			itemElem.SetFlag( FLItemElem::binds );
		}

		if( pUser->CreateItem( &itemElem ) )
		{
			pUser->AddDefinedText( TID_GAME_REAPITEM, "\"%s\"", itemElem.GetProp()->szName );
			g_DPSrvr.PutCreateItemLog( pUser, &itemElem , "E", "PACK" );
		}
		else
		{
			// critical err
		}
	}

	OnAfterUseItem( pUser, pItemElem->GetProp() );
	pUser->UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_NUM, pItemElem->m_nItemNum - 1 );
}

BOOL	FLItemUsing::DoUseGiftbox( FLWSUser* pUser, FLItemElem* pItemElem, DWORD dwItemId )
{
	GIFTBOXRESULT	result;

	if( CGiftboxMan::GetInstance()->Open( dwItemId, &result ) )
	{
		if( CGiftboxMan::GetInstance()->IsEnoughEmptySpace( pUser, dwItemId ) == FALSE )
		{
			pUser->AddDefinedText( TID_GAME_LACKSPACE );
			return TRUE;
		}

		if( pItemElem )
		{
			OnAfterUseItem( pUser, pItemElem->GetProp() );
			pUser->UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_NUM, pItemElem->m_nItemNum - 1 );
		}

		FLItemElem itemElem;
		itemElem.m_dwItemId	= result.dwItem;
		itemElem.m_nItemNum	= result.nNum;
		if( result.nFlag != 4 )	// ignore property
		{
			itemElem.m_byFlag	= result.nFlag;
			itemElem.m_bCharged		= itemElem.GetProp()->bCharged;
		}

		time_t t	= 0;
		if( result.nSpan )	// minutes
		{
			CTime time	= CTime::GetCurrentTime() + CTimeSpan( 0, 0, result.nSpan, 0 );
			t	= (time_t)( time.GetTime() );
		}
		itemElem.m_dwKeepTime	= t;
		itemElem.SetAbilityOption( result.nAbilityOption );

		if( pUser->CreateItem( &itemElem ) )
		{
			pUser->AddDefinedText( TID_GAME_REAPITEM, "\"%s\"", itemElem.GetProp()->szName );						
			LogItemInfo aLogItem;
			//aLogItem.Action = "E";
			//aLogItem.SendName = pUser->GetName();
			//aLogItem.RecvName = "GIFTBOX";
			FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "E" );
			FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
			FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "GIFTBOX" );
			aLogItem.WorldId = pUser->GetWorld()->GetID();
			aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
			g_DPSrvr.OnLogItem( aLogItem, &itemElem, result.nNum );
		}

		return TRUE;
	}

	return FALSE;
}


void	FLItemUsing::DoUseEveFBOX( FLWSUser* pUser, FLItemElem* pItemElem ) 
{
	DWORD dwIdx;
	if( pItemElem )
	{
		OnAfterUseItem( pUser, pItemElem->GetProp() );
		pUser->UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_NUM, pItemElem->m_nItemNum - 1 );
	}

	int nRand	= xRandom( 1, 1000 );
	if( nRand > 470 )   // 53%
		dwIdx    = ITEM_INDEX( 2802, II_GEN_FOO_INS_CHOCOLATE );
	else if( nRand > 130 )         // 34%
		dwIdx    = ITEM_INDEX( 10222, II_CHR_MAG_TRI_HEARTBOMB );
	else if( nRand > 108 )         // 2.2%
		dwIdx    = ITEM_INDEX( 4405, II_ARM_S_CLO_SHO_WARMSLEEPER );
	else if( nRand > 86 )         // 2.2%
		dwIdx    = ITEM_INDEX( 4404, II_ARM_S_CLO_HAT_MUSHROOM );
	else if( nRand > 71 )         // 1.5%
		dwIdx    = ITEM_INDEX( 4401, II_ARM_S_CLO_HAT_STATHAT );
	else if( nRand > 66 )   // 0.5%
		dwIdx    = ITEM_INDEX( 6320, II_WEA_SWO_HARTBONG );
	else if( nRand > 56 )         // 1%
		dwIdx    = ITEM_INDEX( 10343, II_ARM_M_CLO_KOREAN01SUIT );
	else if( nRand > 41 )          // 1.5%
		dwIdx    = ITEM_INDEX( 10344, II_ARM_M_CLO_KOREAN01SHOES );
	else if( nRand > 31 )          // 1%
		dwIdx    = ITEM_INDEX( 10346, II_ARM_F_CLO_KOREAN01SUIT );
	else if( nRand > 16 )          // 1.5%
		dwIdx    = ITEM_INDEX( 10347, II_ARM_F_CLO_KOREAN01SHOES );
	else if( nRand > 6 )          // 1%
		dwIdx    = ITEM_INDEX( 6319, II_SYS_SYS_EVE_WINGS );
	else if( nRand > 5 )          // 0.1%
		dwIdx    = ITEM_INDEX( 6323, II_WEA_KNU_ISHOFIST );
	else	// 0.5%
		dwIdx    = ITEM_INDEX( 10345, II_ARM_F_CLO_KOREAN01HAT );

	FLItemElem itemElem;
	itemElem.m_dwItemId	= dwIdx;
	itemElem.m_nItemNum	= 1;
	if( pUser->CreateItem( &itemElem ) )
	{
		LogItemInfo aLogItem;
		//aLogItem.Action = "E";
		//aLogItem.SendName = pUser->GetName();
		//aLogItem.RecvName = "GIFTBOX";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "E" );
		FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
		FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "GIFTBOX" );
		aLogItem.WorldId = pUser->GetWorld()->GetID();
		aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
		g_DPSrvr.OnLogItem( aLogItem, &itemElem, 1 );
	}
	else
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "GIFTBOX//0//%s" ), pUser->GetName() );
	}
}

// IK2_FOOD / IK2_REFRESHER
BOOL	FLItemUsing::DoUseItemFood( FLWSUser* pUser, PT_ITEM_SPEC pItemProp, FLItemElem* pItemElem )
{
	if( pItemProp->dwID == ITEM_INDEX( 2826, II_GEN_FOO_INS_SUPERHOTDOG ) )
	{
		pUser->SetPointParam( DST_HP, pUser->GetMaxHitPoint() );
		pUser->SetPointParam( DST_MP, pUser->GetMaxManaPoint() );
		pUser->SetPointParam( DST_FP, pUser->GetMaxFatiguePoint() );
		return TRUE;
	}

	int nPoint;
	int nMax, nHP, nHPMax;

	for( int i = 0; i < MAX_ITEM_PARAM; i ++ )	//sun: 10, __PROP_0827
	{
		if( pItemProp->dwDestParam[i] != NULL_ID )
		{
			nHP	   = pUser->GetPointParam( pItemProp->dwDestParam[i] );
			nPoint = pItemProp->nAdjParamVal[i];
			nHPMax = pUser->GetMaxPoint( pItemProp->dwDestParam[i]);
			nMax   = pItemProp->dwAbilityMin;

			if( pItemProp->dwCircleTime != -1)	// ��������۸� dwCircleTime���� �� �����....
			{
				BOOL bRet = DoUseItemFood_SM( pUser, pItemProp, pItemElem );
				if( bRet == FALSE )
					return FALSE;
			}
			else
			{
				if( pItemProp->dwDestParam[i] == DST_HP || pItemProp->dwDestParam[i] == DST_MP || pItemProp->dwDestParam[i] == DST_FP )
				{
					if( nPoint != -1 )
					{
						if( nHP >= nMax )
						{
							if( nHP + (nPoint * 0.3f) >= nHPMax )
							{
								nHP = nHPMax;
							}
							else
							{
								if( TRUE == pUser->IsPlayer() )
								{
									DWORD dwParam = 0;
									switch( pItemProp->dwDestParam[i] )
									{
									case DST_HP:	dwParam = TID_GAME_LIMITHP;		break;
									case DST_MP:	dwParam = TID_GAME_LIMITMP;		break;
									case DST_FP:	dwParam = TID_GAME_LIMITFP;		break;
									}

									pUser->AddDefinedText( dwParam, "" );
								}
								nPoint = (int)( nPoint * 0.3f );
								nHP = nHP + nPoint;
							}
						}
						else
						{
							if( nHP + nPoint >= nHPMax )	// ����Ʈ �������� ���ٵǴ°� ó��.
								nHP = nHPMax;
							else
								nHP = nHP + nPoint;
						}
					}

					pUser->SetPointParam( pItemProp->dwDestParam[i], nHP );
				} // DST_HP, MP, FP
				else
				{
					g_xApplyItemEffect->DoApplyEffect( pUser, pUser, pItemProp );
				}
			}
		}
	} // for

	return TRUE;
}

// �ҽ����� �򰥸��ϱ� ��������۸� ���� ����.
BOOL	FLItemUsing::DoUseItemFood_SM( FLWSUser* pUser, PT_ITEM_SPEC pItemProp, FLItemElem* /*pItemElem*/ )
{
	if( !( pUser->IsSMMode( SM_MAX_HP ) || pUser->IsSMMode( SM_MAX_HP50 ) ) )		// MAXHP ������ �������� ����.
	{
		int nAdjParamVal = 0;
		int nType;
		if( pItemProp->dwID == ITEM_INDEX( 10209, II_CHR_FOO_COO_BULLHAMS ) )
		{
			nAdjParamVal = pItemProp->nAdjParamVal[1];
			nType = SM_MAX_HP;
		}
		else
		{
			if( pItemProp->dwID == ITEM_INDEX( 10210, II_CHR_FOO_COO_GRILLEDEEL ) )
			{
				pUser->m_nPlusMaxHitPoint = nAdjParamVal = int( pUser->GetMaxOriginHitPoint() / ( 100 / pItemProp->nAdjParamVal[0] ) );
				nType = SM_MAX_HP50;
			}
			else
			{
				return FALSE;
			}
		}

		pUser->SetDestParam( pItemProp->dwDestParam[0], nAdjParamVal, nAdjParamVal );
		pUser->SetSMMode( nType, pItemProp->dwCircleTime );
	}
	else
	{
		pUser->AddDefinedText( TID_GAME_LIMITED_USE, "" );	// ������� ��� ������
		return FALSE;
	}

	return TRUE;
}

void	FLItemUsing::DoUseSystemPet( FLWSUser* pUser, FLItemElem* pItemElem )
{
	if( pUser->HasActivatedSystemPet() )
		pUser->InactivateSystemPet( pItemElem );
	else
		pUser->ActivateSystemPet( pItemElem );
}

void	FLItemUsing::DoUseEatPet( FLWSUser* pUser, FLItemElem* pItemElem )
{
	if( pUser->HasActivatedEatPet() )
		pUser->InactivateEatPet();
	else
	{
		pUser->ActivateEatPet( pItemElem );
#ifdef PAT_LOOTOPTION
		pUser->AddPetLootOption();
#endif // PAT_LOOTOPTION
	}
}		

void	FLItemUsing::DoUseSummonNPC( FLWSUser* pUser, FLItemElem & ItemElem )
{
	if( pUser->IsPlayer() == FALSE )
		return;

	PT_ITEM_SPEC pItemProp = ItemElem.GetProp();
	if( pItemProp == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "[ NO DATA - ItemID(%u) ObjID(%u) ObjIndex(%u) ]" ), ItemElem.m_dwItemId, ItemElem.m_dwObjId, ItemElem.m_dwObjIndex );
		return;
	}

	if( pUser->m_oiEatPet != NULL_ID && pUser->m_dwPetId != NULL_ID )		//@ 2�����̻� ���� ��ȯ�Ǿ������� ��ȯ�Ҽ� ���� ( �ϵ� �ڵ� )
	{
		pUser->AddDefinedText( TID_GAME_SUMMON_NPC_FULL );
		return;
	}

	if( pUser->GetBarunaNPCSummoner().GetNPCOBJID() != NULL_ID && pUser->GetBarunaNPCSummoner().GetNPCItemOBJID() != ItemElem.m_dwObjId )
	{
		pUser->AddDefinedText( TID_GAME_ALREADY_CALLED_NPC );
		return;
	}

	const OBJID	BarunaNPCOBJIDSaved		= pUser->GetBarunaNPCSummoner().GetNPCOBJID();
	const FLNPCSummoner::E_OPERATION::E eResult = pUser->GetBarunaNPCSummoner().ExcuteCallNPCByItem( *pUser, ItemElem/*, pItemProp->szTextFileName*//*pCharacterKey*//*_T( "MaEw_Branka" )*/ );
	if( eResult == FLNPCSummoner::E_OPERATION::ACTIVE )
	{
		pUser->AddSummonBarunaNPC();
	}
	else if( eResult == FLNPCSummoner::E_OPERATION::INACTIVE )
	{
		pUser->AddReleaseBarunaNPC( BarunaNPCOBJIDSaved );
	}
}

BOOL	FLItemUsing::DoUseItemWarp( FLWSUser* pUser, PT_ITEM_SPEC pItemProp, FLItemElem* pItemElem )
{
	switch( pItemProp->dwID )
	{
	case ITEM_INDEX( 3600, II_GEN_WARP_COUPLERING ):
	case ITEM_INDEX( 18186, II_GEN_WARP_WEDDING_BAND ):
	case ITEM_INDEX( 3601,II_GEN_WARP_COUPLERING01 ):		//sun: 8, // __S_COUPLERING01
		{
			if( pUser->IsFly() )
			{
				return FALSE;
			}

			u_long idCouple	= static_cast<u_long>( pItemElem->GetCoupleId() );
			if( idCouple == pUser->m_idPlayer )
			{
				pUser->AddDefinedText( TID_GAME_COUPLERING_ERR01, "%s", pUser->GetName() );
				return FALSE;
			}

			if( idCouple > 0 )
			{
				// teleport
				//sun: 11, ĳ���� ���� ����
				const char* lpszPlayer	= CPlayerDataCenter::GetInstance()->GetPlayerString( idCouple );

				if( lpszPlayer )
				{
					FLWSUser* pDestUser	= (FLWSUser*)prj.GetUserByID( idCouple );
					if( IsValidObj( pDestUser ) )
					{
						if( pDestUser->IsFly() )
						{
							pUser->AddDefinedText( TID_GAME_COUPLERING_ERR03 );
							return FALSE;
						}
						if( pDestUser->GetWorld() && pDestUser->GetWorld()->GetID() == WI_WORLD_GUILDWAR )
							return FALSE;
						if( pUser->GetWorld() && pUser->GetWorld()->GetID() == WI_WORLD_GUILDWAR )
							return FALSE;
						//sun: 11, �ϴ��� ��� ����
						if( pUser->GetWorld() && pUser->GetWorld()->GetID() >= WI_WORLD_GUILDWAR1TO1_0
							&& pUser->GetWorld()->GetID() <= WI_WORLD_GUILDWAR1TO1_L )
							return FALSE;
						if( pDestUser->GetWorld() && pDestUser->GetWorld()->GetID() >= WI_WORLD_GUILDWAR1TO1_0
							&& pDestUser->GetWorld()->GetID() <= WI_WORLD_GUILDWAR1TO1_L )
							return FALSE;
						//sun: 13, ���κ��� ���̽�
						if( CRainbowRaceMng::GetInstance()->IsEntry( pUser->m_idPlayer )
							|| CRainbowRaceMng::GetInstance()->IsEntry( idCouple ) )
						{
							pUser->AddDefinedText( TID_GAME_RAINBOWRACE_NOTELEPORT );
							return FALSE;
						}

						if( pUser->GetWorld() == pDestUser->GetWorld() && !prj.IsGuildQuestRegion( pUser->GetWorld()->GetID(), pDestUser->GetPos() )
							&& pUser->GetLayer() == pDestUser->GetLayer()
							&& pUser->GetWorld()->GetID() != WI_WORLD_QUIZ
							&& pDestUser->GetWorld()->GetID() != WI_WORLD_QUIZ
							)
						{
							pDestUser->SummonPlayer( pUser );
//							g_DPCoreClient.SendTeleportPlayer( pUser->m_idPlayer, idCouple );
						}
						else
						{
							pUser->AddDefinedText( TID_GAME_COUPLERING_ERR02, "%s", lpszPlayer );
						}
					}
					else
					{
						pUser->AddDefinedText( TID_DIAG_0061, "%s", lpszPlayer );
					}
				}
			}
			else
			{
				// carve
				//sun: 11, ����, �ູ
				pItemElem->SetCoupleId( pUser->m_idPlayer );

				if( ITEM_INDEX( 3600, II_GEN_WARP_COUPLERING ) == pItemProp->dwID )
					pUser->UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_KEEPTIME, 21600 );	// 15 days

				pUser->AddDefinedText( TID_GAME_COUPLERING_CARVE, "%s", pUser->GetName() );
			}

			return TRUE;
		}
	default:
		break;
	}

	return FALSE;
}

int		FLItemUsing::DoUseItemSystem( FLWSUser* pUser, PT_ITEM_SPEC pItemProp, FLItemElem* pItemElem, int nPart )
{
	int nResult = 0;

	if( pUser->IsPlayer() )
	{
		if( pUser->HasInput() )
			return DoUseItemInput( pUser, pItemProp, pItemElem );
	}

	switch( pItemProp->dwID )
	{
	case ITEM_INDEX( 10211, II_CHR_SYS_SCR_RESTATE ):
		{
			if(		pUser->m_Stat.GetOriginStr() == DEFAULT_ORIGIN_STAT_VALUE
				&&	pUser->m_Stat.GetOriginSta() == DEFAULT_ORIGIN_STAT_VALUE
				&&	pUser->m_Stat.GetOriginDex() == DEFAULT_ORIGIN_STAT_VALUE
				&&	pUser->m_Stat.GetOriginInt() == DEFAULT_ORIGIN_STAT_VALUE
				)
			{
				pUser->AddDefinedText( TID_GAME_DONOTUSE_RESTATE );
				nResult = 1;
			}
			else
			{
				pUser->ReState();
			}
		}
		break;
		//sun: 10, __S_ADD_RESTATE
	case ITEM_INDEX( 30151, II_CHR_SYS_SCR_RESTATE_STR ):
		{
			if( pUser->m_Stat.GetOriginStr() == DEFAULT_ORIGIN_STAT_VALUE )
			{
				pUser->AddDefinedText( TID_GAME_DONOTUSE_RESTATE );			
				nResult = 1;
			}
			else
				pUser->ReStateOne( 0 );
		}
		break;
	case ITEM_INDEX( 30153, II_CHR_SYS_SCR_RESTATE_STA ):
		{
			if( pUser->m_Stat.GetOriginSta() == DEFAULT_ORIGIN_STAT_VALUE )
			{
				pUser->AddDefinedText( TID_GAME_DONOTUSE_RESTATE );			
				nResult = 1;
			}
			else
				pUser->ReStateOne( 1 );
		}
		break;
	case ITEM_INDEX( 30152, II_CHR_SYS_SCR_RESTATE_DEX ):
		{
			if( pUser->m_Stat.GetOriginDex() == DEFAULT_ORIGIN_STAT_VALUE )
			{
				pUser->AddDefinedText( TID_GAME_DONOTUSE_RESTATE );			
				nResult = 1;
			}
			else
				pUser->ReStateOne( 2 );
		}
		break;
	case ITEM_INDEX( 30154, II_CHR_SYS_SCR_RESTATE_INT ):
		{
			if( pUser->m_Stat.GetOriginInt() == DEFAULT_ORIGIN_STAT_VALUE )
			{
				pUser->AddDefinedText( TID_GAME_DONOTUSE_RESTATE );			
				nResult = 1;
			}
			else
				pUser->ReStateOne( 3 );
		}
		break;

	case ITEM_INDEX( 24309, II_CHR_SYS_SCR_STAT_DICE ):
	case ITEM_INDEX( 24391, II_CHR_SYS_SCR_STAT_DICE_EXCEPT_VAGRANT ):
		{
			if( pItemProp->dwID == ITEM_INDEX( 24391, II_CHR_SYS_SCR_STAT_DICE_EXCEPT_VAGRANT ) && pUser->GetJob() <= JOB_VAGRANT ) {
				nResult = 1;
				break;
			}

			pUser->m_Stat.ApplyRandomDiceStats( pUser->GetJob() );
			g_xWSUserManager->AddSetState( pUser, Stat::StatDice_Set );
			nResult = 0;

			//////////////////////////////////////////////////////////////////////////
			FLItemElem kLogItem( *pItemElem );
			kLogItem.SetGeneralPiercingSize( 4 );
			kLogItem.SetGeneralPiercingItemID( 0, pUser->m_Stat.GetDiceStr() );
			kLogItem.SetGeneralPiercingItemID( 1, pUser->m_Stat.GetDiceSta() );
			kLogItem.SetGeneralPiercingItemID( 2, pUser->m_Stat.GetDiceDex() );
			kLogItem.SetGeneralPiercingItemID( 3, pUser->m_Stat.GetDiceInt() );
			g_DPSrvr.SendPutItemLog( pUser, "s", pUser->GetName(), "STAT_DICE_SET", &kLogItem, kLogItem.m_nItemNum );
			//////////////////////////////////////////////////////////////////////////
		}
		break;
	case ITEM_INDEX( 24310, II_CHR_SYS_SCR_STAT_DICE_RESET ):
		{
			if( pUser->m_Stat.ResetDiceStat() )	{
				//////////////////////////////////////////////////////////////////////////
				FLItemElem kLogItem( *pItemElem );

				kLogItem.SetGeneralPiercingSize( 4 );

				kLogItem.SetGeneralPiercingItemID( 0, pUser->m_Stat.GetDiceStr() );
				kLogItem.SetGeneralPiercingItemID( 1, pUser->m_Stat.GetDiceSta() );
				kLogItem.SetGeneralPiercingItemID( 2, pUser->m_Stat.GetDiceDex() );
				kLogItem.SetGeneralPiercingItemID( 3, pUser->m_Stat.GetDiceInt() );

				g_DPSrvr.SendPutItemLog( pUser, "r", pUser->GetName(), "STAT_DICE_RESET", &kLogItem, kLogItem.m_nItemNum );
				//////////////////////////////////////////////////////////////////////////

				g_xWSUserManager->AddSetState( pUser, Stat::StatDice_Reset );
				nResult = 0;
			}
			else
			{
				pUser->AddDefinedText( TID_MMI_DONOTUSE_STATDICE );
				nResult = 1;
			}
		}
		break;

	case ITEM_INDEX( 10220, II_CHR_SYS_SCR_TRANSY ):
		{
			if( DoUseItemSexChange( pUser, nPart ) == FALSE )
			{
				nResult = 1;
			}
			else
			{
				if( pUser->HasBuffByIk3( IK3_TEXT_DISGUISE ) )
					pUser->RemoveIk3Buffs( IK3_TEXT_DISGUISE );
			}
		}
		break;
		//sun: 9, 9-10�� ��
	case ITEM_INDEX( 21034, II_SYS_SYS_SCR_PET_LIFE ):
	case ITEM_INDEX( 26562, II_SYS_SYS_SCR_PET_LIFE02 ):
		{
			CPet* pPet	= pUser->GetPet();
			if( pPet == NULL )
			{
				pUser->AddDefinedText( TID_GAME_PET_NOT_FOUND );
				nResult	= 1;
				break;
			}

			if( pPet->GetLevel() == PL_EGG )
			{
				pUser->AddDefinedText( TID_GAME_PET_CANT_USE_TO_EGG );
				nResult	= 1;
				break;
			}

			if( pPet->GetLife() >= MAX_PET_LIFE )
			{
				nResult	= 1;
				break;
			}

			WORD wLife	= pPet->GetLife();
			wLife	+= ( pItemProp->dwID == ITEM_INDEX( 21034, II_SYS_SYS_SCR_PET_LIFE )? CPetProperty::GetInstance()->GetAddLife(): 1 );

			pPet->SetLife( wLife > MAX_PET_LIFE? MAX_PET_LIFE: wLife );

			FLItemElem* pItemElem	= pUser->GetPetItem();
			pUser->AddPetState( pItemElem->m_dwObjId, pPet->GetLife(), pPet->GetEnergy(), pPet->GetExp() );
			g_dpDBClient.CalluspPetLog( pUser->m_idPlayer, pItemElem->GetSerialNumber(), wLife, PETLOGTYPE_LIFE, pPet );
			break;
		}
	case ITEM_INDEX( 21036, II_SYS_SYS_SCR_PET_HATCH ):
	case ITEM_INDEX( 20487, II_SYS_SYS_SCR_PET_HATCH1 ):
		{
			CPet* pPet	= pUser->GetPet();
			if( pPet == NULL )
			{
				pUser->AddDefinedText( TID_GAME_PET_NOT_FOUND );
				nResult	= 1;
				break;
			}
			if( pPet->GetLevel() != PL_EGG )
			{
				pUser->AddDefinedText( TID_GAME_PET_4EGG );
				nResult	= 1;
				break;
			}
			pUser->PetLevelup();
			break;
		}
	case ITEM_INDEX( 26453, II_GEN_TOO_COL_NORMALBATTERY ):
		{
			if( !pUser->DoUseItemBattery() )
			{
				pUser->AddDefinedText( TID_GAME_CANT_USE_BATTERY );
				nResult	= 1;
			}
			break;
		}
	case ITEM_INDEX( 10212, II_CHR_SYS_SCR_SHOUTFULL15 ):
		{
			if( !pUser->IsShoutFull() )
				pUser->SetSMMode( SM_SHOUT15, pItemProp->dwCircleTime );
			else
				nResult = 2;
		}
		break;
	case ITEM_INDEX( 10213, II_CHR_SYS_SCR_SHOUTFULL30 ):
		{
			if( !pUser->IsShoutFull() )
				pUser->SetSMMode( SM_SHOUT30, pItemProp->dwCircleTime );
			else
				nResult = 2;
		}
		break;
	case ITEM_INDEX( 26557, II_CHR_SYS_SCR_SHOUTFULL001 ):
		{
			if( !pUser->IsShoutFull() )
				pUser->SetSMMode( SM_SHOUT001, pItemProp->dwCircleTime );
			else
				nResult = 2;
		}
		break;
	case ITEM_INDEX( 30057, II_CHR_SYS_SCR_PSKILLFULL1 ):
		{
			if( !( pUser->IsSMMode( SM_PARTYSKILL1 ) || pUser->IsSMMode( SM_PARTYSKILL15 ) || pUser->IsSMMode( SM_PARTYSKILL30 ) ) )
				//sun: 12, �Ľ�ų ������ ����
			{
				pUser->SetSMMode( SM_PARTYSKILL1, pItemProp->dwCircleTime );
//				g_DPCoreClient.SendUserPartySkill( pUser->m_idPlayer, PARTY_PARSKILL_MODE, 1000, 0, 1 );

				if( pUser->m_idparty > 0 )
				{
					g_PartyMng.NotiSetPartyEffectModeToCS( pUser->m_idPlayer, pUser->m_idparty, PARTY_EFFECT_PARSKILLFULL, true );
				}
			}
			else
				nResult = 2;
		}
		break;
	case ITEM_INDEX( 10214, II_CHR_SYS_SCR_PSKILLFULL15 ):
		{
			if( !( pUser->IsSMMode( SM_PARTYSKILL1 ) || pUser->IsSMMode( SM_PARTYSKILL15 ) || pUser->IsSMMode( SM_PARTYSKILL30 ) ) )
				//sun: 12, �Ľ�ų ������ ����
			{
				pUser->SetSMMode( SM_PARTYSKILL15, pItemProp->dwCircleTime );
//				g_DPCoreClient.SendUserPartySkill( pUser->m_idPlayer, PARTY_PARSKILL_MODE, 1000, 0, 1 );

				if( pUser->m_idparty > 0 )
				{
					g_PartyMng.NotiSetPartyEffectModeToCS( pUser->m_idPlayer, pUser->m_idparty, PARTY_EFFECT_PARSKILLFULL, true );
				}
			}
			else
				nResult = 2;
		}
		break;
	case ITEM_INDEX( 10215, II_CHR_SYS_SCR_PSKILLFULL30 ):
		{
			if( !( pUser->IsSMMode( SM_PARTYSKILL1 ) || pUser->IsSMMode( SM_PARTYSKILL15 ) || pUser->IsSMMode( SM_PARTYSKILL30 ) ) )
				//sun: 12, �Ľ�ų ������ ����
			{
				pUser->SetSMMode( SM_PARTYSKILL30, pItemProp->dwCircleTime );
//				g_DPCoreClient.SendUserPartySkill( pUser->m_idPlayer, PARTY_PARSKILL_MODE, 1000, 0, 1 );

				if( pUser->m_idparty > 0 )
				{
					g_PartyMng.NotiSetPartyEffectModeToCS( pUser->m_idPlayer, pUser->m_idparty, PARTY_EFFECT_PARSKILLFULL, true );
				}
			}
			else
				nResult = 2;
		}
		break;
	case ITEM_INDEX( 10216, II_CHR_SYS_SCR_COMMBANK15 ):
		{
			if( !pUser->IsCommBank() )
				pUser->SetSMMode( SM_BANK15, pItemProp->dwCircleTime );
			else
				nResult = 2;
		}
		break;
	case ITEM_INDEX( 10217, II_CHR_SYS_SCR_COMMBANK30 ):
		{
			if( !pUser->IsCommBank() )
				pUser->SetSMMode( SM_BANK30, pItemProp->dwCircleTime );
			else
				nResult = 2;
		}
		break;
	case ITEM_INDEX( 26559, II_CHR_SYS_SCR_COMMBANK001 ):
		{
			if( !pUser->IsCommBank() )
				pUser->SetSMMode( SM_BANK001, pItemProp->dwCircleTime );
			else
				nResult = 2;
		}
		break;
	case ITEM_INDEX( 10218, II_CHR_SYS_SCR_ACTIVITION ):
		{
			if( pUser->IsSMMode( SM_ACTPOINT ) == FALSE && pUser->IsSMMode( SM_ACTPOINT_30M ) == FALSE )
			{
				pUser->SetSMMode( SM_ACTPOINT, pItemProp->dwCircleTime );
			}
			else
			{
				nResult = 2;
			}
		}
		break;
	case ITEM_INDEX( 20515, II_CHR_SYS_SCR_NEWACTIVITION ):
		{
			if( pUser->IsSMMode( SM_ACTPOINT ) == FALSE && pUser->IsSMMode( SM_ACTPOINT_30M ) == FALSE )
			{
				pUser->SetSMMode( SM_ACTPOINT_30M, pItemProp->dwCircleTime );
			}
			else
			{
				nResult = 2;
			}
		}
		break;
	case ITEM_INDEX( 30058, II_CHR_SYS_SCR_UPCUTSTONE01 ):
		{
			if( !( pUser->IsSMMode( SM_ATTACK_UP1 ) || pUser->IsSMMode( SM_ATTACK_UP ) ) )
				pUser->SetSMMode( SM_ATTACK_UP1, pItemProp->dwCircleTime );
			else
				nResult = 2;
		}
		break;
	case ITEM_INDEX( 10270, II_CHR_SYS_SCR_UPCUTSTONE ):
		{
			if( !( pUser->IsSMMode( SM_ATTACK_UP1 ) || pUser->IsSMMode( SM_ATTACK_UP ) ) )
			{
				pUser->SetSMMode( SM_ATTACK_UP, pItemProp->dwCircleTime );
			}
			else
				nResult = 2;
		}
		break;
	case ITEM_INDEX( 10430, II_SYS_SYS_SCR_BLESSING ):
		{
			if( !pUser->IsSMMode( SM_REVIVAL ) && !pUser->IsSMMode( SM_REVIVAL2 ) )
				pUser->SetSMMode( SM_REVIVAL, pItemProp->dwCircleTime );
			else
				nResult = 2;					
		}
		break;
	case ITEM_INDEX( 25197, II_SYS_SYS_SCR_BLESSING_02 ):
		{
			if( !pUser->IsSMMode( SM_REVIVAL ) && !pUser->IsSMMode( SM_REVIVAL2 ) )
				pUser->SetSMMode( SM_REVIVAL2, pItemProp->dwCircleTime );
			else
				nResult = 2;					
		}
		break;
	case ITEM_INDEX( 10428, II_SYS_SYS_SCR_SELPAR ):
		{
			CParty* pParty;
			pParty = g_PartyMng.GetParty( pUser->GetPartyId() );
			if( pParty )
			{
				if( pParty->m_nLevel < MAX_PARTYLEVEL )	// �ܸ��ش��϶��� ��밡��
					pParty->SetPartyLevel( pUser, 10, 180, 0 );
				else
					nResult = 1;
			}
			else
			{
				nResult = 1;
			}
			if( nResult == 1 )
				pUser->AddDefinedText( TID_GAME_NTROUPEO10, "" );
		}
		break;
	case ITEM_INDEX( 10426, II_SYS_SYS_SCR_HOLY ):
		pUser->RemoveDebufBuffs();
		break;
	case ITEM_INDEX( 10432, II_SYS_SYS_SCR_VELOCIJUMP ):
		{
			if( !( pUser->IsSMMode( SM_VELOCIJUMP ) ) && !pUser->IsSMMode( SM_JUMP ) )
			{
				for( size_t Nth = 0; Nth < MAX_ITEM_PARAM; ++Nth )
				{
					if( pItemProp->dwDestParam[ Nth ] != NULL_ID )
					{
						pUser->SetDestParam( pItemProp->dwDestParam[ Nth ], pItemProp->nAdjParamVal[ Nth ], pItemProp->nAdjParamVal[ Nth ] );
					}
				}
// 				if( pItemProp->dwDestParam1 != -1 )
// 					pUser->SetDestParam( pItemProp->dwDestParam1, pItemProp->nAdjParamVal1, pItemProp->nAdjParamVal1 );
// 				if( pItemProp->dwDestParam2 != -1 )
// 					pUser->SetDestParam( pItemProp->dwDestParam2, pItemProp->nAdjParamVal2, pItemProp->nAdjParamVal2 );
// 				if( pItemProp->dwDestParam3 != -1 )
// 					pUser->SetDestParam( pItemProp->dwDestParam3, pItemProp->nAdjParamVal3, pItemProp->nAdjParamVal2 );
				pUser->SetSMMode( SM_VELOCIJUMP, pItemProp->dwCircleTime );
			}
			else
			{
				nResult = 2;	
			}
		}
		break;
	case ITEM_INDEX( 25564, II_SYS_SYS_SCR_JUMPUP ):
		{
			if( !( pUser->IsSMMode( SM_VELOCIJUMP ) ) && !pUser->IsSMMode( SM_JUMP ) )
			{
				for( size_t Nth = 0; Nth < MAX_ITEM_PARAM; ++Nth )
				{
					if( pItemProp->dwDestParam[ Nth ] != NULL_ID )
					{
						pUser->SetDestParam( pItemProp->dwDestParam[ Nth ], pItemProp->nAdjParamVal[ Nth ], pItemProp->nAdjParamVal[ Nth ] );
					}
				}
// 				if( pItemProp->dwDestParam1 != -1 )
// 					pUser->SetDestParam( pItemProp->dwDestParam1, pItemProp->nAdjParamVal1, pItemProp->nAdjParamVal1 );
// 				if( pItemProp->dwDestParam2 != -1 )
// 					pUser->SetDestParam( pItemProp->dwDestParam2, pItemProp->nAdjParamVal2, pItemProp->nAdjParamVal2 );
// 				if( pItemProp->dwDestParam3 != -1 )
// 					pUser->SetDestParam( pItemProp->dwDestParam3, pItemProp->nAdjParamVal3, pItemProp->nAdjParamVal2 );
				pUser->SetSMMode( SM_JUMP, pItemProp->dwCircleTime );
			}
			else
			{
				nResult = 2;	
			}
		}
		break;
	case ITEM_INDEX( 10434, II_SYS_SYS_SCR_RECCURENCE ):
		{
			if( pUser->InitSkillExp() )
				pUser->AddInitSkill();
			else
				nResult = 1;
		}
		break;

	case ITEM_INDEX( 10467, II_SYS_SYS_SCR_GLVCHEER ):	// ������ �� �尩
	case ITEM_INDEX( 10464, II_SYS_SYS_SCR_SMELPROT ):	// "��ȣ�� �η縶��"		
		//sun: 9,10�� ���� __ULTIMATE
	case ITEM_INDEX( 10488, II_SYS_SYS_SCR_SMELPROT3 ): // " ��ȣ�� �η縶��(�ֻ��)
		//sun: 11, ä�� �ý���
	case ITEM_INDEX( 10468, II_SYS_SYS_SCR_SMELTING ):	// "������ �η縶��"	
	case ITEM_INDEX( 26200, II_SYS_SYS_SCR_SUPERLEADERPARTY ):	// "������ �ູ"		
	case ITEM_INDEX( 26203, II_SYS_SYS_SCR_SUPERSMELTING ):	// "���� �η縶��(��)"
	case ITEM_INDEX( 30011, II_SYS_SYS_SCR_FONTEDIT ):	// "Font Edit"		
	case ITEM_INDEX( 10477, II_SYS_SYS_SCR_SKLINSTRUCT ):	// "����� ������"
	case ITEM_INDEX( 10489, II_SYS_SYS_SCR_SMELTING2 ):	// �Ӽ� ������ �η縶��
#ifdef COSTUME_UPGRADE_ENHANCEMENT_GEM
	case ITEM_INDEX( 25336, II_SYS_SYS_SCR_COSTUMEPROT ):
#endif
		{
			if( pUser->HasBuff( BUFF_ITEM, (WORD)( pItemProp->dwID ) ) )
			{
				nResult = 2;
			}
			else
			{
				g_xApplyItemEffect->DoApplyEffect( pUser, pUser, pItemProp );
			}
		}
		break;
		//	mulcom	BEGIN100405	���� ��ȣ�� �η縶��
	case	ITEM_INDEX( 26961, II_SYS_SYS_SCR_AWAKESAFE ):
		{
			if( pUser->HasBuff( BUFF_ITEM, (WORD)( pItemProp->dwID ) ) )
			{
				nResult = 2;
			}
			else
			{
				g_xApplyItemEffect->DoApplyEffect( pUser, pUser, pItemProp );
			}
		}
		break;
		//	mulcom	END100405	���� ��ȣ�� �η縶��

		//sun: 9, 9-10�� ��
	case ITEM_INDEX( 21032, ITEM_INDEX( 21032, II_SYS_SYS_SCR_PET_TONIC_A ) ):
	case ITEM_INDEX( 21033, ITEM_INDEX( 21033, II_SYS_SYS_SCR_PET_TONIC_B ) ):
		//sun: 12, �� ����
		nResult		= DoUseItemPetTonic( pUser, pItemElem );	// �� ������ ���
		break;

	case ITEM_INDEX( 21035, II_SYS_SYS_SCR_PET_FEED_POCKET ):
		{
			nResult = DoUseItemFeedPocket( pUser, pItemElem );
			break;
		}

		//sun: 9, // __II_SYS_SYS_SCR_GET
	case ITEM_INDEX( 30115, II_SYS_SYS_SCR_GET01 ):
	case ITEM_INDEX( 30116, II_SYS_SYS_SCR_GET02 ):
		{
			if( pUser->HasBuff( BUFF_ITEM, ITEM_INDEX( 30115, II_SYS_SYS_SCR_GET01 ) ) ||
				pUser->HasBuff( BUFF_ITEM, ITEM_INDEX( 30116, II_SYS_SYS_SCR_GET02 ) ) )
			{
				nResult = 2;
			}
			else
			{
				g_xApplyItemEffect->DoApplyEffect( pUser, pUser, pItemProp );
			}
		}
		break;

	case ITEM_INDEX( 10469, II_SYS_SYS_SCR_RETURN ):		// "��ȯ�� �η縶��"		
		{
			if( pUser->IsPlayer() )
			{
				if( pUser->HasBuff( BUFF_ITEM, ITEM_INDEX( 10469, II_SYS_SYS_SCR_RETURN ) ) )
				{
					nResult = 2;
				}
				else
				{
					CWorld* pWorld	= pUser->GetWorld();
					if( !pWorld )
						return 0;

					D3DXVECTOR3 vPos = pUser->GetPos();
					//Ŭ����ũ ��/ �ɹٶ󽺿����� �� �������� ����� �� ������ ���� ����Ϸ� �õ� �� '�� ������ ���� �����̹Ƿ� �������� ����� �� �����ϴ�.'��� �޽����� ����Ѵ�
					if( pWorld->GetID() == WI_WORLD_GUILDWAR || pWorld->GetID() == WI_WORLD_KEBARAS || prj.IsGuildQuestRegion( pWorld->GetID(), vPos ) )
					{			
						pUser->AddDefinedText( TID_GAME_LIMITZONE_USE );
						return 0;
					}

					//sun: 11, �ϴ��� ��� ����
					if( g_GuildCombat1to1Mng.IsPossibleUser( pUser ) )
					{
						pUser->AddDefinedText( TID_GAME_LIMITZONE_USE );
						return 0;
					}

					pUser->SetReturnPos( vPos );

					//1.��ȯ���� ���� 
					g_xApplyItemEffect->DoApplyEffect( pUser, pUser, pItemProp );
					//2. ���õ� ������ �̵� 
					LPCTSTR lpszKey = pUser->m_lpszVillage;
					if( lpszKey )
					{
						PRegionElem pRgnElem = g_WorldMng.GetRevivalPos( pUser->GetWorld()->GetID(), lpszKey );
						if( pRgnElem )
							pUser->REPLACE( g_uIdofMulti, pRgnElem->m_dwWorldId, pRgnElem->m_vPos, REPLACE_NORMAL, nRevivalLayer );
					}
					else
					{
						FLERROR_LOG( PROGRAM_NAME, _T( "II_SYS_SYS_SCR_RETURN selected village is NULL" ) );
					}

// 					if( lpszKey )
// 					{
// 						PRegionElem pRgnElem = g_WorldMng.GetRevivalPos( WI_WORLD_MADRIGAL, lpszKey );
// 						if( pRgnElem != NULL )
// 						{
// 							pUser->REPLACE( g_uIdofMulti, pRgnElem->m_dwWorldId, pRgnElem->m_vPos, REPLACE_NORMAL, nRevivalLayer );
// 						}
// 						else
// 						{
// 							FLERROR_LOG( PROGRAM_NAME, _T( "[ FAILED GET RETURN REGION. USER:(%07d), WORLD_ID:(%d), RETURN_REGION:(%s) ]" )
// 								, pUser->m_idPlayer, pWorld->GetID(), lpszKey );
// 							return 0;
// 						}
// 					}
// 					else
// 					{
// 						FLERROR_LOG( PROGRAM_NAME, _T( "II_SYS_SYS_SCR_RETURN selected village is NULL" ) );
// 						return 0;
// 					}
				}
			}
		}
		break;
	case ITEM_INDEX( 15220, II_SYS_SYS_SCR_CUSTODY2 ):
		{
			CMover* pMover	= (CMover*)CreateObj( D3DDEVICE, OT_MOVER, OBJECT_INDEX( 200, MI_INFO_PENG ) );
			FLStrcpy( pMover->m_szCharacterKey, _countof( pMover->m_szCharacterKey ), "MaFl_InstantBank" );
			pMover->InitNPCProperty();
			pMover->InitCharacter( pMover->GetCharacter() );
			pMover->SetPos( pUser->GetPos() );
			pMover->InitMotion( MTI_STAND );
			pMover->UpdateLocalMatrix();
			pMover->m_dwTickCreated		= GetTickCount();
			if( pUser->GetWorld()->ADDOBJ( pMover, TRUE, pUser->GetLayer() ) == FALSE )
			{
				SAFE_DELETE( pMover );
				return 0;
			}
			break;
		}

	case ITEM_INDEX( 26201, II_SYS_SYS_SCR_FRIENDSUMMON_A ):
	case ITEM_INDEX( 26217, II_SYS_SYS_SCR_FRIENDSUMMON_B ):
		{
			int nState = pUser->GetSummonState();
			if( nState != 0 )
			{
				DWORD dwMsgId = 0;
				if( nState == 1 )	// �ŷ���
					dwMsgId = TID_GAME_TRADE_NOTUSE;
				else if( nState == 2 ) // ����
					dwMsgId = TID_GAME_DIE_NOTUSE;
				else if( nState == 3 ) // ���λ��� ��
					dwMsgId = TID_GAME_VENDOR_NOTUSE;
				else if( nState == 4 ) // ������
					dwMsgId = TID_GAME_ATTACK_NOTUSE;
				else if( nState == 5 ) // ������
					dwMsgId = TID_GAME_FLY_NOTUSE;

				pUser->AddDefinedText( TID_GAME_STATE_NOTUSE , "\"%s\"", prj.GetText( dwMsgId ) );
			}
			//sun: 11, �ϴ��� ��� ����
			else if( g_GuildCombat1to1Mng.IsPossibleUser( pUser ) )
			{
				pUser->AddDefinedText( TID_GAME_STATE_NOTUSE , "\"%s\"", prj.GetText( TID_GAME_ATTACK_NOTUSE ) );
			}
			else
			{
				pUser->AddSummonFriendUse( pItemElem );	
			}
			nResult = 99;
		}
		break;

		//sun: ��ǥ ��� �丰 ����
	case ITEM_INDEX( 26456, II_SYS_SYS_SCR_PERIN ):
		{
			int nState = pUser->GetSummonState();
			if( nState != 0 )
			{
				DWORD dwMsgId = 0;
				if( nState == 1 )	// �ŷ���
					dwMsgId = TID_GAME_TRADE_NOTUSE;
				else if( nState == 2 ) // ����
					dwMsgId = TID_GAME_DIE_NOTUSE;
				else if( nState == 3 ) // ���λ��� ��
					dwMsgId = TID_GAME_VENDOR_NOTUSE;
				else if( nState == 4 ) // ������
					dwMsgId = TID_GAME_ATTACK_NOTUSE;
				else if( nState == 5 ) // ������
					dwMsgId = TID_GAME_FLY_NOTUSE;

				pUser->AddDefinedText( TID_GAME_STATE_NOTUSE , "\"%s\"", prj.GetText( dwMsgId ) );
				nResult = 99;
			}
			else
			{
				float	fTmpGold = (float)( pUser->GetGold() );
				if( fTmpGold >= 2000000000 || fTmpGold < 0  )
				{
					pUser->AddDiagText( prj.GetText(TID_GAME_TOOMANYMONEY_USE_PERIN) );
					return 99;
				}
				else
				{
					pUser->AddGold(100000000,true);

					// chipi080711 - �丰��� �α�
					LogItemInfo aLogItem;
					//aLogItem.Action = "C";
					//aLogItem.SendName = pUser->GetName();
					//aLogItem.RecvName = "USE_PERIN";
					FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "C" );
					FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
					FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "USE_PERIN" );
					aLogItem.WorldId = pUser->GetWorld()->GetID();
					aLogItem.Gold = (DWORD)( fTmpGold );
					aLogItem.Gold2 = pUser->GetGold();
					aLogItem.Gold_1  = pUser->GetGold() - static_cast<int>(fTmpGold);
					//aLogItem.ItemName = pItemElem->GetName();
					FLSPrintf( aLogItem.kLogItem.szItemName, _countof( aLogItem.kLogItem.szItemName ), "%d", pItemProp->dwID );
					aLogItem.kLogItem.nQuantity = 1;
					aLogItem.kLogItem.dwSerialNumber = pItemElem->GetSerialNumber();
					g_DPSrvr.OnLogItem( aLogItem );
				}
			}
		}
		break;

	case ITEM_INDEX( 30337, II_CHR_SYS_SCR_RESTATE_STR_LOW ):
		{
			if( pUser->m_Stat.GetOriginStr() == DEFAULT_ORIGIN_STAT_VALUE )
			{
				pUser->AddDefinedText( TID_GAME_DONOTUSE_RESTATE );			
				nResult = 1;
			}
			else
				pUser->ReStateOneLow( 0 );
		}
		break;
	case ITEM_INDEX( 30339, II_CHR_SYS_SCR_RESTATE_STA_LOW ):
		{
			if( pUser->m_Stat.GetOriginSta() == DEFAULT_ORIGIN_STAT_VALUE )
			{
				pUser->AddDefinedText( TID_GAME_DONOTUSE_RESTATE );			
				nResult = 1;
			}
			else
				pUser->ReStateOneLow( 1 );
		}
		break;
	case ITEM_INDEX( 30338, II_CHR_SYS_SCR_RESTATE_DEX_LOW ):
		{
			if( pUser->m_Stat.GetOriginDex() == DEFAULT_ORIGIN_STAT_VALUE )
			{
				pUser->AddDefinedText( TID_GAME_DONOTUSE_RESTATE );			
				nResult = 1;
			}
			else
				pUser->ReStateOneLow( 2 );
		}
		break;
	case ITEM_INDEX( 30340, II_CHR_SYS_SCR_RESTATE_INT_LOW ):
		{
			if( pUser->m_Stat.GetOriginInt() == DEFAULT_ORIGIN_STAT_VALUE )
			{
				pUser->AddDefinedText( TID_GAME_DONOTUSE_RESTATE );			
				nResult = 1;
			}
			else
				pUser->ReStateOneLow( 3 );
		}
		break;
	case ITEM_INDEX( 26473, II_SYS_SYS_SCR_SMELPROT4 ):	// �Ǽ��縮 ��ȣ�� �η縶��
	case ITEM_INDEX( 25233, II_SYS_SYS_SCR_SMELPROT6 ):	// �Ǽ��縮 ��ȣ�� �η縶��
		{
			if( pUser->HasBuff( BUFF_ITEM, ITEM_INDEX( 26473, II_SYS_SYS_SCR_SMELPROT4 ) ) ||
				pUser->HasBuff( BUFF_ITEM, ITEM_INDEX( 25233, II_SYS_SYS_SCR_SMELPROT6 ) ) )
			{
				nResult = 2;
			}
			else
			{
				g_xApplyItemEffect->DoApplyEffect( pUser, pUser, pItemProp );
			}
		}
		break;
	default:
		nResult = 1;
		break;
	}

	if( pItemProp->dwItemKind3 == IK3_BALLOON )
	{
		if( pUser->HasBuffByIk3( IK3_BALLOON ) != FALSE )
		{
			nResult = 2;
		}
		else
		{
			g_xApplyItemEffect->DoApplyEffect( pUser, pUser, pItemProp );
			nResult = 0;
		}
	}

	return nResult;
}

int		FLItemUsing::DoUseItemInput( FLWSUser* pUser, PT_ITEM_SPEC pProp, FLItemElem* /*pItem*/ )
{
	switch( pProp->dwID )
	{
	case ITEM_INDEX( 20039, II_SYS_SYS_SCR_PET_NAMING ):
		return DoUseItemPetNaming( pUser );
	default:
		return 1;
	}
//	return 1;
}

BOOL	FLItemUsing::DoUseItemSexChange( FLWSUser* pUser, int nFace )
{
	// 1.
	for( DWORD dwParts = 0; dwParts < MAX_HUMAN_PARTS; dwParts++ )
	{
		if( dwParts == PARTS_HEAD || dwParts == PARTS_HAIR || dwParts == PARTS_LWEAPON
			|| dwParts == PARTS_RWEAPON || dwParts == PARTS_SHIELD || dwParts == PARTS_RIDE
			|| ( dwParts >= PARTS_NECKLACE1 && dwParts <= PARTS_BULLET ) )
			continue;

		FLItemElem* pArmor	= pUser->m_Inventory.GetEquip( dwParts );
		if( pArmor )
		{
			pUser->AddDefinedText( TID_GAME_CHECK_EQUIP, "" );
			return FALSE;
		}
	}

	// 2.
	DWORD dwIndex;
	if( pUser->GetSex() == SEX_MALE )
	{
		dwIndex		= OBJECT_INDEX( 12, MI_FEMALE );
		pUser->SetSex( SEX_FEMALE );
	}
	else
	{
		dwIndex	= OBJECT_INDEX( 11, MI_MALE );
		pUser->SetSex( SEX_MALE );
	}

	pUser->SetTypeIndex( D3DDEVICE, OT_MOVER, dwIndex );
	pUser->ResetScale();
	pUser->SetMotion( MTI_WALK );
	//RedoEquip( FALSE );		// chipi_091125 ����
	pUser->RedoEquip( FALSE, FALSE );	// chipi_091125 �߰� - �Ǽ��縮, ���� ���� �������� ���� ���·� Ʈ������ �����ѵ� �ɷ�ġ�� �ٽ�(�ߺ�) �����Ű�� ������ �־���.
	pUser->UpdateLocalMatrix();

	// 3.
	g_xWSUserManager->AddSexChange( pUser );

	// 4.
	pUser->m_dwHeadMesh	= (DWORD)nFace;
	g_xWSUserManager->AddChangeFace( pUser->m_idPlayer, (DWORD)nFace );

	//sun: 11, ĳ���� ���� ����
	g_dpDBClient.SendUpdatePlayerData( pUser );

	return TRUE;
}

// IK2_BLINKWING�� ó��.
BOOL	FLItemUsing::DoUseItemBlinkWing( FLWSUser* pUser, PT_ITEM_SPEC pItemProp, FLItemElem* pItemElem, BOOL bUse )
{
	if( pUser->m_pActMover->IsSit() ) // 2005.12.08 raiders �ɾ� ������ �ȵǰ� 
		return FALSE;

	if( pUser->IsStateMode( STATE_BASEMOTION_MODE ) ) 
		return FALSE;

	if( bUse )
	{
		if( pUser->GetWorld()->m_dwWorldID == WI_WORLD_KEBARAS )
			return FALSE;
	}

// 	if( pUser->GetWorld()->m_dwWorldID == WI_WORLD_GUILDWAR )
// 		return FALSE;
// 
// 	//sun: 11, �ϴ��� ��� ����
// 	if( g_GuildCombat1to1Mng.IsPossibleUser( pUser ) )
// 	{
// 		if( pUser->IsPlayer() )
// 			pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_NOBLINKWING );
// 		return FALSE;
// 	}
// 
// 	//sun: 12, ����� ��
// 	if( CSecretRoomMng::GetInstance()->IsInTheSecretRoom( pUser ) )
// 	{
// 		if( pUser->IsPlayer() )
// 			pUser->AddDefinedText( TID_GAME_SECRETROOM_USEBRINKITEM );
// 		return FALSE;
// 	}
// 
// 	//sun: 13, ���κ��� ���̽�
// 	if( CRainbowRaceMng::GetInstance()->IsEntry( pUser->m_idPlayer ) )
// 	{
// 		if( pUser->IsPlayer() )
// 			pUser->AddDefinedText( TID_GAME_RAINBOWRACE_NOTELEPORT );
// 		return FALSE;
// 	}
// 
// 	//sun: 13, �Ͽ�¡ �ý���
// 	if( pUser->GetWorld()->GetID() == WI_WORLD_MINIROOM )	// �Ͽ�¡������ ��ũ�� ����.
// 		return FALSE;

	if( !canUseTeleportation( pUser ) ) {
		return FALSE;
	}

	if( pItemProp->dwItemKind3 == IK3_TOWNBLINKWING )	// IK3_TOWNBLINKWING
	{
		REPLACE_TYPE type = REPLACE_NORMAL;
		if( pItemProp->dwID == ITEM_INDEX( 10435, II_CHR_SYS_SCR_ESCAPEBLINKWING ) )
			type = REPLACE_FORCE; 

		// lodestar
		if( bUse )
		{
			if( IsUsableItem( pItemElem ) == FALSE )
				return FALSE;

//			pItemElem->UseItem();
			pUser->UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_NUM, pItemElem->m_nItemNum );
		}

		PRegionElem pRgnElem	= NULL;
		CWorld* pWorld	= pUser->GetWorld();
		if( !pWorld )
			return FALSE;

		if( pUser->IsChaotic() )
		{
			if( pWorld->GetID() != pWorld->m_dwIdWorldRevival && pWorld->m_dwIdWorldRevival != 0 )
				pRgnElem	= g_WorldMng.GetRevivalPosChao( pWorld->m_dwIdWorldRevival, pWorld->m_szKeyRevival );
			if( NULL == pRgnElem )	// Find near revival pos
				pRgnElem	= g_WorldMng.GetNearRevivalPosChao( pWorld->GetID(), pUser->GetPos() );
		}
		else

		{
			if( pWorld->GetID() != pWorld->m_dwIdWorldRevival && pWorld->m_dwIdWorldRevival != 0 )
				pRgnElem	= g_WorldMng.GetRevivalPos( pWorld->m_dwIdWorldRevival, pWorld->m_szKeyRevival );
			if( NULL == pRgnElem )	// Find near revival pos
				pRgnElem	= g_WorldMng.GetNearRevivalPos( pWorld->GetID(), pUser->GetPos() );
		}
		if( NULL != pRgnElem )
			pUser->REPLACE( g_uIdofMulti, pRgnElem->m_dwWorldId, pRgnElem->m_vPos, type, nRevivalLayer );
		else
			pUser->REPLACE( g_uIdofMulti, pWorld->GetID(), pUser->GetPos(), type, nDefaultLayer );
	}
	else
	{
		//		x: dwItemAtkOrder1
		//		y: dwItemAtkOrder2
		//		z: dwItemAtkOrder3
		//		angle: dwItemAtkOrder4
		//		dwWeaponType: worldid
		if( pItemProp->dwWeaponType == WI_WORLD_NONE || pItemProp->dwWeaponType == 0xffffffff )
			return FALSE;

		D3DXVECTOR3 vPos( (float)( pItemProp->dwItemAtkOrder1 ), (float)( pItemProp->dwItemAtkOrder2 ), (float)( pItemProp->dwItemAtkOrder3 ) );
		pUser->SetAngle( (float)pItemProp->dwItemAtkOrder4 );
		// remove & transfer
		if( bUse )
		{
			if( IsUsableItem( pItemElem ) == FALSE )
				return FALSE;

//			pItemElem->UseItem();
			pUser->UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_NUM, pItemElem->m_nItemNum );
		}

		if( pUser->IsChaotic() )
		{
			PRegionElem pRgnElem	= NULL;
			CWorld* pWorld = g_WorldMng.GetWorld( pItemProp->dwWeaponType );
			if( pWorld )
				pRgnElem	= g_WorldMng.GetRevivalPosChao( pItemProp->dwWeaponType, pItemProp->szTextFileName );
			if( NULL == pRgnElem )	// Find near revival pos
				pRgnElem	= g_WorldMng.GetNearRevivalPosChao( pWorld->GetID(), pUser->GetPos() );

			if( pRgnElem )
				pUser->REPLACE( g_uIdofMulti, pRgnElem->m_dwWorldId, pRgnElem->m_vPos, REPLACE_NORMAL, nRevivalLayer );
		}
		else
		{
			pUser->REPLACE( g_uIdofMulti, pItemProp->dwWeaponType, vPos, REPLACE_NORMAL, nTempLayer );
		}
	}

	return TRUE;
}

//sun: 12, �� ����
// �� ������ ���
int		FLItemUsing::DoUseItemPetTonic( FLWSUser* pUser, FLItemElem* pItemElem )
{
	// �� ������ �� ������ ��� ����� �� ����
	if( ( pItemElem->m_dwItemId == ITEM_INDEX( 21032, II_SYS_SYS_SCR_PET_TONIC_A ) && pUser->HasBuff( BUFF_ITEM, ITEM_INDEX( 21033, II_SYS_SYS_SCR_PET_TONIC_B ) ) )
		|| ( pItemElem->m_dwItemId == ITEM_INDEX( 21033, II_SYS_SYS_SCR_PET_TONIC_B ) && pUser->HasBuff( BUFF_ITEM, ITEM_INDEX( 21032, II_SYS_SYS_SCR_PET_TONIC_A ) ) )
		)
	{
		return 2;
	}

	// �ð� �����÷ο� ����
	DWORD dwCurr	= ::timeGetTime();
	IBuff* pBuff	= pUser->m_buffs.GetBuff( BUFF_ITEM, (WORD)( pItemElem->m_dwItemId ) );
	if( pBuff && static_cast<int>( pItemElem->GetProp()->dwSkillTime ) > static_cast<int>( pItemElem->GetProp()->dwSkillTime + ( pBuff->GetTotal() - ( dwCurr - pBuff->GetInst() ) ) ) )
		return 1;

	// ȿ�� ����
	g_xApplyItemEffect->DoApplyEffect( pUser, pUser, pItemElem->GetProp() );
	return 0;
}

int		FLItemUsing::DoUseItemFeedPocket( FLWSUser* pUser, FLItemElem* pPocket )
{
	int nResult = 0;

	if( pUser->HasBuff( BUFF_ITEM, ITEM_INDEX( 26531, II_SYS_SYS_SCR_PET_FEED_POCKET02 ) ) )
	{
		pUser->AddDefinedText( TID_GAME_DONOTUSE_PETPOCKET );
		return	4;
	}
	// Ȱ��ȭ �˻�
	if( pUser->HasBuff( BUFF_ITEM, (WORD)( pPocket->m_dwItemId ) ) )
	{
		if( pPocket->m_dwKeepTime != 0 )
		{
			pUser->RemoveBuff( BUFF_ITEM, (WORD)( pPocket->m_dwItemId ) );
			nResult = 4;
		}
		else
			nResult = 2;
	}
	else
	{
		PT_ITEM_SPEC pProp	= pPocket->GetProp();
		if( pPocket->m_dwKeepTime == 0 )	// ���� �ð��� �����Ǿ� ���� �����Ƿ� ���� ����̴�.
		{
			DWORD dwMax	= pUser->m_Inventory.GetMax();
			FLItemElem* pItemElem;
			for( DWORD i = 0 ; i < dwMax; i++ )
			{
				pItemElem	= pUser->m_Inventory.GetAtId( i );
				if( IsUsableItem( pItemElem )
					&& pItemElem->m_dwItemId == pPocket->m_dwItemId
					&& pItemElem->m_dwKeepTime > 0
					&& !pItemElem->IsFlag( FLItemElem::expired )
					)
				{
					// �Ⱓ�� ������� ���� ���� �������� �����Ͽ� ����� �� ����.
					pUser->AddDefinedText( TID_GAME_DONOTUSE_PETPOCKET, "" );
					nResult = 4;
					return nResult;
				}
			}
			FLASSERT( pProp->dwAbilityMin < 259200 );
			pUser->UpdateItem( (BYTE)( pPocket->m_dwObjId ), UI_KEEPTIME, pProp->dwAbilityMin );	// 1440	// 1 days
		}
		g_xApplyItemEffect->DoApplyEffect( pUser, pUser, pProp );
	}

	return nResult;
}

int		FLItemUsing::DoUseItemPetNaming( FLWSUser* pUser )
{
	CPet* pPet	= pUser->GetPet();
	if( !pPet )
	{
		pUser->AddDefinedText( TID_GAME_NAME_PET_E00 );
		return 1;
	}
	if( prj.IsInvalidName( pUser->GetInput() ) || !prj.IsAllowedLetter( pUser->GetInput() ) )
	{
		pUser->AddDefinedText( TID_GAME_NAME_PET_E01 );
		return 1;
	}
	pPet->SetName( pUser->GetInput() );
	g_xWSUserManager->AddSetPetName( pUser, pPet->GetName() );
	return 0;
}

bool	FLItemUsing::canUseTeleportation( FLWSUser* pUser ) const
{
	if( IsInvalidObj( pUser ) ) {
		return false;
	}

	CWorld* pWorld	= pUser->GetWorld();
	if( pWorld == NULL ) {
		return false;
	}

	/************************************************************************/
	/*
	����� �� ���� �����ΰ�..
	*/
	const DWORD userWorldID	= pWorld->GetID();

	switch ( userWorldID )
	{
	case WI_WORLD_GUILDWAR:
	case WI_WORLD_QUIZ:
	case WI_WORLD_MINIROOM:
		pUser->AddDefinedText( TID_MMI_NOT_USE_AREA );
		return false;

	default:
		break;
	}

	if( prj.IsGuildQuestRegion( userWorldID, pUser->GetPos() ) ) {
		pUser->AddDefinedText( TID_MMI_NOT_USE_AREA );
		return false;
	}
	/************************************************************************/

	/************************************************************************/
	/*
	����� �� ���� �����ΰ�..
	*/
	if( g_GuildCombat1to1Mng.IsPossibleUser( pUser ) ) {
		pUser->AddDefinedText( TID_MMI_NOT_USE_GUILDBATTLE );
		return false;
	}

	if( CRainbowRaceMng::GetInstance()->IsEntry( pUser->m_idPlayer ) ) {
		pUser->AddDefinedText( TID_MMI_NOT_USE_RAINBOWRACE );
		return false;
	}

	if( CSecretRoomMng::GetInstance()->IsInTheSecretRoom( pUser ) ) {
		pUser->AddDefinedText( TID_MMI_NOT_USE_SECRETROOM );
		return false;
	}
	/************************************************************************/

	return true;
}

// �������� ����ϴ� ������ ȣ��ȴ�. 
void	FLItemUsing::OnAfterUseItem( FLWSUser* pUser, const PT_ITEM_SPEC pItemProp )
{
	if( pItemProp == NULL )
		return;

	D3DXVECTOR3 sPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	DWORD dwSfxID = pItemProp->dwSfxObj3;		// ������ ���ÿ� ������ ����Ʈ

	if(	pItemProp->dwItemKind3 == IK3_MAGICTRICK ) // ����Ʈ �����۷�
	{
		float	fTheta = D3DXToRadian( pUser->GetAngle() );
		switch( pItemProp->dwID )
		{
		case ITEM_INDEX( 2905, II_GEN_MAG_TRI_FIRESHOWER ):
		case ITEM_INDEX( 2908, II_GEN_MAG_TRI_HWFIREWORKS ):
		case ITEM_INDEX( 10221, II_CHR_MAG_TRI_ROCKETBOMB ):
		case ITEM_INDEX( 10222, II_CHR_MAG_TRI_HEARTBOMB ):
		case ITEM_INDEX( 10224, II_CHR_MAG_TRI_TWISTERBOMB ):
			sPos.x = pUser->GetPos().x + sinf(fTheta) * 3.0f;			// �ٶ󺸴� ���� 1���� �տ��� �߻���Ŵ.
			sPos.z = pUser->GetPos().z + -cosf(fTheta) * 3.0f;
			sPos.y = pUser->GetPos().y + 1.5f;
			sPos.y = pUser->GetWorld()->GetUnderHeight( D3DXVECTOR3(sPos.x,sPos.y,sPos.z) );
			break;

		case ITEM_INDEX( 2904, II_GEN_MAG_TRI_NEWYEARBOMB ):
		case ITEM_INDEX( 2906, II_GEN_MAG_TRI_SULNALBOMB ):
		case ITEM_INDEX( 2907, II_GEN_MAG_TRI_GOODBYEBOMB ):
			sPos.x = pUser->GetPos().x;
			sPos.z = pUser->GetPos().z;
			sPos.y = pUser->GetPos().y + 3.0f;
			break;
		}
	}

	if( dwSfxID != NULL_ID )		
		g_xWSUserManager->AddCreateSfxObj( pUser, dwSfxID, sPos.x, sPos.y, sPos.z );	// ������ǥ�� ����.

	pUser->SetHonorAdd(pItemProp->dwID,HI_USE_ITEM);
}

void	FLItemUsing::CompleteUseItem( FLWSUser* pUser, DWORD dwId )
{
	FLItemBase* pItemBase = pUser->GetItemId( dwId );
	if( IsUsableItem( pItemBase ) == FALSE )
		return;

	FLItemElem* pItemElem = (FLItemElem*)pItemBase;
	PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( pItemElem->m_dwItemId );
	if( pItemProp == NULL )
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// ������ ��� �� ����
	OnAfterUseItem( pUser, pItemProp );	// raiders 06.04.20

	pItemElem->UseItem();			// --m_nItemNum;

	if( pItemElem->m_bCharged == TRUE || pItemProp->bCharged == TRUE )		// ���ȭ ������ �α�
	{
		g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemElem, pItemProp );		
	}

	CHAR cUIParam = UI_NUM;

	DWORD dwGroup = pUser->m_cooltimeMgr.GetGroup( pItemProp );
	if( dwGroup )	// ��Ÿ�� �������̸� ���ð��� ����Ѵ�.
	{
		pUser->m_cooltimeMgr.SetTime( dwGroup, pItemProp->GetCoolTime() );	
		cUIParam = UI_COOLTIME;
	}

	pUser->UpdateItem( (BYTE)( dwId ), cUIParam, pItemElem->m_nItemNum );	// ������ 0�̸�  ������ ���� , ���� 

	// ���� �ͼ� ó��
	if( pItemElem != NULL && pItemElem->m_nItemNum > 0 )
	{
		if( pItemProp->nBindCondition == BIND_CONDITION_USE
			&& pItemElem->IsFlag( FLItemElem::binds ) == FALSE
			&& pItemElem->IsFlag( FLItemElem::reset_bind ) == FALSE )
		{
			pItemElem->SetFlag( FLItemElem::binds );
			pUser->UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_FLAG, MAKELONG( pItemElem->m_dwObjIndex, pItemElem->m_byFlag ) );
		}
		else if( pItemProp->nResetBindCondition == BIND_CONDITION_USE
			&& pItemElem->IsFlag( FLItemElem::binds ) == FALSE
			&& pItemElem->IsFlag( FLItemElem::reset_bind ) == TRUE )
		{
			pItemElem->SetFlag( FLItemElem::binds );
			pUser->UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_FLAG, MAKELONG( pItemElem->m_dwObjIndex, pItemElem->m_byFlag ) );
		}
	}
	//////////////////////////////////////////////////////////////////////////
}

bool FLItemUsing::UnbindPeriodLock( FLWSUser & kUser, FLItemElem & kItemElem )
{
	if( kItemElem.IsExpiring() != FALSE || kItemElem.IsFlag( FLItemElem::expired ) != FALSE )
	{
		return false;
	}

	const PT_ITEM_SPEC pSpec	= kItemElem.GetProp();
	if( pSpec == NULL )
		return false;

	if( kItemElem.m_dwKeepTime != 0 )
	{
		kUser.AddDefinedText( TID_MMI_ELLDINPOTION_TEXT07 );	//�̹� ���� �Ǿ� �ְų� ���� �Ҽ� ���� ������ �Դϴ�.
		return false;
	}

	if( FLItemCommonFunction::HasSameGroup( pSpec->nItemGroup, pSpec->nUseLimitGroup, kUser.m_Inventory, kUser.m_Pocket ) == true )
	{
		kUser.AddDefinedText( TID_MMI_LOCKTYPE_TEXT01 );
		return false;
	}

	//if( kItemElem.IsFlag( FLItemElem::normal ) == FALSE )
	//	return false;

	kUser.UpdateItem( static_cast<BYTE>( kItemElem.m_dwObjId ), UI_KEEPTIME, kItemElem.GetProp()->dwAbilityMin );
	kItemElem.SetFlag( FLItemElem::is_using );
	kUser.UpdateItem( static_cast<BYTE>( kItemElem.m_dwObjId ), UI_FLAG, MAKELONG( kItemElem.m_dwObjIndex, kItemElem.m_byFlag ) );
	return true;
}
