#include "StdAfx.h"

#include "FLDropItemCommon.h"
#include "FLMonsterDropItem.h"

#include "FLDropPenalty_ByLevelDist.h"

#include <spevent.h>
#include "FLCooperativeContributions.h"
#include "FLCC_Reward.h"
#include "slord.h"

#include "../_Common/party.h"
extern	CPartyMng	g_PartyMng;

extern	BOOL CanAdd( DWORD dwGold, int nPlus );



FLDropItem_Data::FLDropItem_Data() : m_dwProb( 0 ), m_dwItemID( 0 ), m_nItemCountMin( 0 ), m_nItemCountMax( 0 ), m_nQuestID( 0 )
{

}

FLDropItem_Data::~FLDropItem_Data()
{

}

//-------------------------------------------------------------------------------------------------------------//

FLDropSet::FLDropSet() : m_dwMainProb( 0 ), m_eSetKind( KIND_NONE )
{

}

FLDropSet::~FLDropSet()
{

}

void FLDropSet::DropItem( CMover & kMonster, FLWSUser & kUser )
{
	if( kMonster.GetProp() == NULL )
		return;

	//1. �ܺ� ����� ���
	const DWORD dwMainProb	= static_cast< DWORD >( m_dwMainProb * FLDoDrop::GetItemDropRateFactor( &kMonster, kUser, m_eSetKind ) );
	const DWORD dwMainRandom= ::xRandom( DROP_PROB_MAX );
	if( dwMainRandom >= dwMainProb )
		return;

	//2. ���
	DWORD dwAccumulatedProb	= 0;
	const DWORD dwRandom	= ::xRandom( DROP_PROB_MAX );
	for( DropItemVec::iterator it = m_vecDropItem.begin(); it != m_vecDropItem.end(); ++it )
	{
		FLDropItem_Data & kDropItem = *it;

		dwAccumulatedProb		+= kDropItem.m_dwProb;
		if( dwRandom >= dwAccumulatedProb )
			continue;

		if( kDropItem.m_nItemCountMax <= 0 )
			continue;

		int nItemCount	= static_cast< int >( ::xRandom( kDropItem.m_nItemCountMax - kDropItem.m_nItemCountMin ) + kDropItem.m_nItemCountMin );
		if( nItemCount <= 0 )
			nItemCount		= 1;

		const DWORD dwOwnerID	= ( kMonster.GetProp()->m_kDropItemAttr.bDropOwnerShipOff ) ? NULL_ID : kUser.GetId();
		
		if( kDropItem.m_nQuestID == 0 )
		{
			FLDoDrop::DropItem( kMonster, kUser, dwOwnerID, kDropItem.m_dwItemID, nItemCount );
			return;
		}

		QUEST * pQuest			= kUser.GetQuest( kDropItem.m_nQuestID );
		if( pQuest != NULL && pQuest->m_nState < QS_END )
		{
			FLDoDrop::DropItem( kMonster, kUser, dwOwnerID, kDropItem.m_dwItemID, nItemCount );
			return;
		}
	}
}


FLDropSet::E_DROP_SET_KIND FLDropSet::StringToKindEnum( std::string & strSetKind )
{
	std::transform( strSetKind.begin(), strSetKind.end(), strSetKind.begin(), toupper );
	if( strSetKind == "WEAPON" )
		return FLDropSet::KIND_WEAPON;
	else if( strSetKind == "ARMOR")
		return FLDropSet::KIND_ARMOR;
	else if( strSetKind == "ACCESSORY")
		return FLDropSet::KIND_ACCESSORY;
	else if( strSetKind == "MATERIAL")
		return FLDropSet::KIND_MATERIAL;
	else if( strSetKind == "ETC")
		return FLDropSet::KIND_ETC;
	else if( strSetKind == "QUEST")
		return FLDropSet::KIND_QUEST;
	else if( strSetKind == "EVENT")
		return FLDropSet::KIND_EVENT;
	else if( strSetKind == "RARE")
		return FLDropSet::KIND_RARE;

	return FLDropSet::KIND_NONE;
}

BOOL FLDropSet::Check_Item( const DWORD dwItemID, const int nQuestID )
{
	PT_ITEM_SPEC pItemSpec	= g_xSpecManager->GetSpecItem( dwItemID );
	if( pItemSpec == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID ITEM ID (%u) ]" ), dwItemID );
		return FALSE;
	}

	//����
	switch( m_eSetKind )
	{
	case FLDropSet::KIND_NONE:
		{
			if( nQuestID != 0 )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "[ SET QUEST (%d) ]" ), nQuestID );
				return FALSE;
			}
		}
		break;

	case FLDropSet::KIND_WEAPON:
		{
			if( pItemSpec->dwItemKind1 != IK1_WEAPON )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "[ NOT WEAPON SET (%u) ]" ), dwItemID );
				return FALSE;
			}
		}
		break;
	case FLDropSet::KIND_ARMOR:
		{
			if( pItemSpec->dwItemKind1 != IK1_ARMOR )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "[ NOT ARMOR SET (%u) ]" ), dwItemID );
				return FALSE;
			}
		}
		break;

	case FLDropSet::KIND_ACCESSORY:
		{
			if( pItemSpec->dwItemKind1 != IK1_GENERAL || pItemSpec->dwItemKind2 != IK2_JEWELRY )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "[ NOT ACCESSORY SET (%u) ]" ), dwItemID );
				return FALSE;
			}
		}
		break;

	case FLDropSet::KIND_MATERIAL:
		{
			if( pItemSpec->dwItemKind1 != IK1_GENERAL || pItemSpec->dwItemKind2 != IK2_MATERIAL )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "[ NOT MATERIAL SET (%u) ]" ), dwItemID );
				return FALSE;
			}
		}
		break;

	case FLDropSet::KIND_ETC:
		{
			// ETC�� ��ȹ������ üũ ���ϱ����
			__noop;
		}
		break;

	case FLDropSet::KIND_QUEST:
		{
			if( ( pItemSpec->dwItemKind1 != IK1_SYSTEM || pItemSpec->dwItemKind2 != IK2_SYSTEM || pItemSpec->dwItemKind3 != IK3_QUEST ) 
				&& ( pItemSpec->dwItemKind3 != IK3_EVENTSUB ) 
				&& ( pItemSpec->dwItemKind3 != IK3_EVENTMAIN ) )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "[ NOT QUEST SET (%u) ]" ), dwItemID );
				return FALSE;
			}

			QuestProp * pQuestProp	= prj.m_aPropQuest.GetAt( nQuestID );
			if( pQuestProp == NULL )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID QUEST ID (%d) ]" ), nQuestID );
				return FALSE;
			}
		}
		break;
	case FLDropSet::KIND_EVENT:
		{
			__noop;
		}
		break;
	case FLDropSet::KIND_RARE:
		{
			__noop;
		}	
		break;

	default:
		FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID DROP SET KIND(%d) ]" ), m_eSetKind );
		break;
	}

	return TRUE;
}

BOOL FLDropSet::Check_Prob() const
{
	DWORD dwTotProb		= 0;

	for( FLDropSet::DropItemVec::const_iterator itItem = m_vecDropItem.begin(); itItem != m_vecDropItem.end(); ++itItem )
	{
		const FLDropItem_Data & kItemData = * itItem;
		dwTotProb			+= kItemData.m_dwProb;
	}

	if( dwTotProb != DROP_PROB_MAX )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID PROB : Name(%s) TotalProb(%u) ]"), m_strMainProbName.c_str(), dwTotProb );
		return FALSE;
	}

	return TRUE;
}




//----------------------------------------------------------------------------------------------------------------------------------------//

namespace {
	bool	addItemToFlyingAttacker( CMover& kMonster, FLWSUser& kAttacker, const DWORD dwItemID, const int nItemCount )
	{
		if( kMonster.GetProp() == NULL || !kMonster.GetProp()->dwFlying ) {
			return false;
		}

		const PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( dwItemID );
		if( pItemProp == NULL ) {
			return false;
		}

		if( kAttacker.m_Inventory.GetEquip( PARTS_RIDE ) == NULL
			|| ( kAttacker.m_Inventory.GetEquip( PARTS_RIDE )->GetProp()->GetInventoryType() == pItemProp->GetInventoryType()
			&& kAttacker.m_Inventory.GetEmptyCountByInventoryType( pItemProp->GetInventoryType() ) <= 1 ) ) {
				return false;
		}

		FLItemElem itemElem;
		itemElem.m_dwItemId	= dwItemID;
		itemElem.m_nItemNum	= nItemCount;
		itemElem.SetSerialNumber();
		if( !kAttacker.CreateItem( &itemElem ) ) {
			return false;
		}

		kAttacker.AddDefinedText( TID_GAME_REAPITEM, "\"%s\"", pItemProp->szName );
		if( pItemProp->dwItemKind1 == IK1_WEAPON || pItemProp->dwItemKind1 == IK1_ARMOR || ( pItemProp->dwItemKind1 == IK1_GENERAL && pItemProp->dwItemKind2 == IK2_JEWELRY ) )
		{
			switch( pItemProp->nLog )
			{
			case 1:		g_dpDBClient.SendLogUniqueItem2( &kAttacker, itemElem.GetProp(), itemElem.GetAbilityOption() );
				break;
			case 2:		g_dpDBClient.SendLogUniqueItem2( &kAttacker, itemElem.GetProp(), 200 );
				break;
			case 3:		g_dpDBClient.SendLogUniqueItem2( &kAttacker, itemElem.GetProp(), 100 );
				break;
			default:	__noop;
				break;
			}
		}

		return true;
	}

	bool	addGoldToFlyingAttacker( CMover& kMonster, FLWSUser & kAttacker, const int nGold )
	{
		if( kMonster.GetProp() == NULL || !kMonster.GetProp()->dwFlying ) {
			return false;
		}

		if( !CanAdd( kAttacker.GetGold(), nGold ) ) {
			return false;
		}

		kAttacker.AddGold( nGold );
		kAttacker.AddGoldText( nGold );

		return true;
	}
}

// nProbability * GetItemDropRateFactor( pAttacker );/* �̺�Ʈ */

//void FLDoDrop::Drop( FLWUser & kMonster, FLWSUser & kAttacker )
//{
//	
//}

float FLDoDrop::GetItemDropRateFactor( CMover * pMonster, FLWSUser & kAttacker, const FLDropSet::E_DROP_SET_KIND eSetKind )
{
	float fFactor			= 1.0f;
	fFactor					+= prj.m_fItemDropRate - 1.0f;
	fFactor					+= CEventGeneric::GetInstance()->GetItemDropRateFactor() - 1.0f;
	fFactor					+= prj.m_EventLua.GetItemDropRate() - 1.0f;
	fFactor					+= ( nsCooperativeContributions::GLOBAL_REWARD_ALARM_DROP_RATE().GetRewardValue() / ( float )REWARD_PRECISION ) - 1.0f;
	fFactor					+= CPCBang::GetInstance()->GetPieceItemDropFactor( static_cast< FLWSUser * >( &kAttacker ) ) - 1.0f;
	//const float fEventLuaPieceItemRate	= prj.m_EventLua.GetPieceItemDropRate() - 1.0f;
	
	//sun: 12, ����
	ILordEvent* pEvent		= CSLord::Instance()->GetEvent();
	if( pEvent != NULL )
	{
		fFactor				+= pEvent->GetIFactor() - 1.0f;
		fFactor				+= kAttacker.HasBuff( BUFF_ITEM, ITEM_INDEX( 20019, II_SYS_SYS_LS_CHEERING ) ) ? 1.1f - 1.0f : 0.0f;
		fFactor				+= kAttacker.HasBuff( BUFF_ITEM, ITEM_INDEX( 20036, II_SYS_SYS_VOTE_THANKS ) ) ? 1.05f - 1.0f : 0.0f;
		fFactor				+= kAttacker.HasBuff( BUFF_ITEM, ITEM_INDEX( 26740, II_COUPLE_BUFF_MIRACLE_01 ) ) ? 1.05f - 1.0f : 0.0f;
	}

	if( pMonster != NULL && pMonster->GetProp() != NULL )
	{
#ifdef __S1108_BACK_END_SYSTEM
		fFactor				+= pMonster->GetProp()->m_fItemDrop_Rate - 1.0f;
#endif // __S1108_BACK_END_SYSTEM
		if( eSetKind != FLDropSet::KIND_QUEST && pMonster->GetProp()->m_kDropItemAttr.bLevelDropPanaltyOff == FALSE )
		{
			fFactor			*= FLDropPenalty_ByLevelDist::GetInstance().GetDeclineProb_Item( kAttacker.GetLevel(), pMonster->GetLevel() );
		}
	}

	//�ش� ��ų
	//const DWORD SKILL_PROB_MAX			= 10000;
	CParty* pParty = g_PartyMng.GetParty( kAttacker.m_idparty );
	if( pParty && eSetKind == FLDropSet::KIND_RARE )
	{
		// ��ȸ�ش��̸� ��ũ�������̸� ���常 �������� �����ش�
		if( pParty->GetKind() == PARTY_KIND_EXTEND && pParty->m_dwModeTime[ PARTY_FORTUNECIRCLE_MODE ] )
		{
			SkillProp* pSkillProp	= prj.GetPartySkill( SKILL_INDEX( 6, ST_FORTUNECIRCLE )  );
			if( pSkillProp != NULL )
			{
				//sun: 12, �Ľ�ų ������ ����
//				if( pParty->m_nModeTime[ PARTY_PARSKILL_MODE ] )
				if( pParty->IsActivatedEffect( PARTY_EFFECT_PARSKILLFULL ) == true )
				{
					//fFactor			+= pSkillProp->nProbability / 10000;
					fFactor				= fFactor + ( ( static_cast< float >( pSkillProp->nProbability) / 10000 ) - 1.0f );
				}
				else
				{
					FLWSUser* pLeader = g_xWSUserManager->GetUserByPlayerID( pParty->m_aMember[0].m_uPlayerId );
					if( IsValidObj( (CObj*)pLeader )/* && pLeader->IsNearPC( (FLWSUser*)pAttacker ) && IsOrigin()*/ )
					{
						if( pLeader->IsValidArea( & kAttacker, 255.0f ) )
						{
							//fFactor			+= pSkillProp->nProbability / 10000;
							fFactor				= fFactor + ( ( static_cast< float >( pSkillProp->nProbability) / 10000 ) - 1.0f );
						}
					}
				}
			}
		}
	}

	const int nParam	= kAttacker.GetParam( DST_DROP_ITEM_ALLGRADE_RATE );

	fFactor	+= static_cast<float>( nParam )	/ 100.0f;

	return fFactor > 0.0f ? fFactor : 0.0f;
}

//float FLDoDrop::GetItemDropRateFactor( CMover & kMonster, FLWSUser & kAttacker, const BOOL bQuest )
//{
//	float fFactor			= 1.0f;
//
//	fFactor					*= prj.m_fItemDropRate;
//#ifdef __S1108_BACK_END_SYSTEM
//	fFactor					*= kMonster.GetProp()->m_fItemDrop_Rate;
//#endif // __S1108_BACK_END_SYSTEM
//
//	fFactor					*= CEventGeneric::GetInstance()->GetItemDropRateFactor();
//
//	const float fMulRate	= ( prj.m_EventLua.GetItemDropRate() - 1.0f ) + ( nsCooperativeContributions::GLOBAL_REWARD_ALARM_DROP_RATE().GetRewardValue() - 1.0f ) + 1.0f;
//	fFactor					*= fMulRate;
//
//	fFactor					*= prj.m_EventLua.GetPieceItemDropRate();		//sun: 9, �̺�Ʈ (��� ��ũ��Ʈ ����)
//
//	//sun: 12, ����
//	{
//		ILordEvent* pEvent		= CSLord::Instance()->GetEvent();
//		fFactor					*= pEvent->GetIFactor();
//		if( kAttacker.HasBuff( BUFF_ITEM, ITEM_INDEX( 20019, II_SYS_SYS_LS_CHEERING ) ) )
//			fFactor				*=	1.1F;
//		if( kAttacker.HasBuff( BUFF_ITEM, ITEM_INDEX( 20036, II_SYS_SYS_VOTE_THANKS ) ) )
//			fFactor				*= 1.05F;
//
//		//sun: 13, Ŀ�� ����
//		if( kAttacker.HasBuff( BUFF_ITEM, ITEM_INDEX( 26740, II_COUPLE_BUFF_MIRACLE_01 ) ) )
//			fFactor				*= 1.05F;
//	}
//
//	fFactor					*= CPCBang::GetInstance()->GetPieceItemDropFactor( static_cast<FLWSUser*>( &kAttacker ) );
//
//	if( bQuest == TRUE || ( kMonster.GetProp() != NULL && kMonster.GetProp()->m_kDropItemAttr.bLevelDropPanaltyOff == FALSE ) )
//		fFactor					*= FLDropPenalty_ByLevelDist::GetInstance().GetDeclineProb_Item( kAttacker.GetLevel(), kMonster.GetLevel() );
//
//	return fFactor;
//}
//
//
//float FLDoDrop::GetItemDropRateFactor( CMover & kMonster, FLWSUser & kAttacker, const BOOL bQuest )
//{
//	float fFactor			= 1.0f;
//
//	fFactor					*= prj.m_fItemDropRate;
//#ifdef __S1108_BACK_END_SYSTEM
//	fFactor					*= kMonster.GetProp()->m_fItemDrop_Rate;
//#endif // __S1108_BACK_END_SYSTEM
//
//	fFactor					*= CEventGeneric::GetInstance()->GetItemDropRateFactor();
//
//	const float fMulRate	= ( prj.m_EventLua.GetItemDropRate() - 1.0f ) + ( nsCooperativeContributions::GLOBAL_REWARD_ALARM_DROP_RATE().GetRewardValue() - 1.0f ) + 1.0f;
//	fFactor					*= fMulRate;
//
//	fFactor					*= prj.m_EventLua.GetPieceItemDropRate();		//sun: 9, �̺�Ʈ (��� ��ũ��Ʈ ����)
//
//	//sun: 12, ����
//	{
//		ILordEvent* pEvent		= CSLord::Instance()->GetEvent();
//		fFactor					*= pEvent->GetIFactor();
//		if( kAttacker.HasBuff( BUFF_ITEM, ITEM_INDEX( 20019, II_SYS_SYS_LS_CHEERING ) ) )
//			fFactor				*=	1.1F;
//		if( kAttacker.HasBuff( BUFF_ITEM, ITEM_INDEX( 20036, II_SYS_SYS_VOTE_THANKS ) ) )
//			fFactor				*= 1.05F;
//
//		//sun: 13, Ŀ�� ����
//		if( kAttacker.HasBuff( BUFF_ITEM, ITEM_INDEX( 26740, II_COUPLE_BUFF_MIRACLE_01 ) ) )
//			fFactor				*= 1.05F;
//	}
//
//	fFactor					*= CPCBang::GetInstance()->GetPieceItemDropFactor( static_cast<FLWSUser*>( &kAttacker ) );
//
//	if( bQuest == TRUE || ( kMonster.GetProp() != NULL && kMonster.GetProp()->m_kDropItemAttr.bLevelDropPanaltyOff == FALSE ) )
//		fFactor					*= FLDropPenalty_ByLevelDist::GetInstance().GetDeclineProb_Item( kAttacker.GetLevel(), kMonster.GetLevel() );
//
//	return fFactor;
//}
//



int	FLDoDrop::GetAdditinalLoopCount( FLWSUser & kAttacker )
{
	int nLoop				= 0;

	nLoop	+= kAttacker.GetAdjValueDuplicationBuff( IK3_MOREGETITEMSCROLL );

// 	if( kAttacker.HasBuff( BUFF_ITEM, ITEM_INDEX( 30115, II_SYS_SYS_SCR_GET01 ) ) )		//@@@@@@@@@@@@@@
// 		nLoop				+= 1;
// 	if( kAttacker.HasBuff( BUFF_ITEM, ITEM_INDEX( 30116, II_SYS_SYS_SCR_GET02 ) ) )		//@@@@@@@@@@@@@@
// 		nLoop				+= 2;

	nLoop	+= kAttacker.GetAdjParam( DST_GIFTBOX );
	
	CParty* pParty			= g_PartyMng.GetParty( kAttacker.m_idparty );
	if( pParty != NULL && pParty->GetKind() == PARTY_KIND_EXTEND )
	{
		if( pParty->m_dwModeTime[ PARTY_GIFTBOX_MODE ] )
		{
//			if( pParty->m_nModeTime[ PARTY_PARSKILL_MODE ] )
			if( pParty->IsActivatedEffect( PARTY_EFFECT_PARSKILLFULL ) == true )
			{
				nLoop	+= 1;
			}
			else
			{
				FLWSUser* pLeader = g_xWSUserManager->GetUserByPlayerID( pParty->m_aMember[0].m_uPlayerId );
				if( IsValidObj( (CObj*)pLeader ) && pLeader->IsValidArea( & kAttacker, 255.0f ) )
				{
					nLoop	+= 1;
				}
			}
		}
	}

	if( ::xRandom( DROP_PROB_MAX ) < static_cast<DWORD>( kAttacker.GetParam( DST_DROP_ITEM_ONEMORE_CHANCE ) ) )
	{
		nLoop	+= 1;
	}

	return nLoop > 0 ? nLoop : 0;
}




float FLDoDrop::GetGoldDropRateFactor( CMover & kMonster, FLWSUser & kAttacker )
{
	/*
	//@@@@@@@@ ����ġ
	//	int	nSeedID = 0;
	//	int nNumGold = lpDropItem->dwNumber + xRandom( lpDropItem->dwNumber2 - lpDropItem->dwNumber );	// Number ~ Number2 ������ ������.
	//nNumGold	= nNumGold * nPenyaRate / 100;		//����ġ	

	#ifdef __S1108_BACK_END_SYSTEM
	nNumGold	= (int)( nNumGold * prj.m_fGoldDropRate * lpMoverProp->m_fPenya_Rate );
	if( nNumGold == 0 )
	continue;
	#else // __S1108_BACK_END_SYSTEM
	nNumGold	*= prj.m_fGoldDropRate;
	#endif // __S1108_BACK_END_SYSTEM

	nNumGold	= (int)( nNumGold * prj.m_EventLua.GetGoldDropFactor() );	//sun: 9, �̺�Ʈ (��� ��ũ��Ʈ ����)
	*/

	//@@@@@@@@ ����ġ
	float fFactor			= 1.0f;
	fFactor					+= prj.m_fGoldDropRate - 1.0f;
	fFactor					+= prj.m_EventLua.GetGoldDropFactor() - 1.0f;

	if( kMonster.GetProp() != NULL && kMonster.GetProp()->m_kDropItemAttr.bLevelDropPanaltyOff == FALSE )
		fFactor				*= FLDropPenalty_ByLevelDist::GetInstance().GetDeclineProb_Gold( kAttacker.GetLevel(), kMonster.GetLevel() );

	return fFactor;
}




void FLDoDrop::DropItem( CMover & kMonster, FLWSUser & kAttacker, const DWORD dwOwnerPlayerID, const DWORD dwItemID, const int nItemCount )
{
	if( nItemCount <= 0 )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID ITEM QUANTITY : nItemCount(%d) ]" ), nItemCount );
		return;
	}

	PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( dwItemID );
	if( pItemProp == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID ITEM ID : dwItemID(%u) ]" ), dwItemID );
		return;
	}

	CWorld* pWorld = kAttacker.GetWorld();
	if( pWorld == NULL )
	{
		return;
	}

	if( !addItemToFlyingAttacker( kMonster, kAttacker, dwItemID, nItemCount ) ) {
		FLItemElem * pItemElem	= new FLItemElem;
		pItemElem->m_dwItemId	= dwItemID;
		pItemElem->m_nItemNum	= nItemCount;
		pItemElem->m_nHitPoint	= ( pItemProp->dwEndurance == -1 ) ? 0 : pItemProp->dwEndurance;//pItemProp->dwEndurance;

		
		// Random option start
		MoverProp* lpMoverProp	= kMonster.GetProp();
		const WORD wOriginID = static_cast< WORD >( CRandomOptItemGen::GetInstance()->GenRandomOptItem( lpMoverProp->dwLevel, (FLOAT)100 / 100.0f, pItemProp, lpMoverProp->dwClass ) );
		//const WORD wOriginID = static_cast< WORD >( CRandomOptItemGen::GetInstance()->GenRandomOptItem( 10, (FLOAT)100 / 100.0f, pItemProp, 1 ) );
		pItemElem->SetRandomOptionOriginID( wOriginID );
		int nSize = lpMoverProp->m_DropItemGenerator.GetSize();
		
		DWORD dwEnhanceProb = xRandom( 3000000000 );
		DWORD dwEnhanceRandom	= xRandom( 3 );
		if( dwEnhanceProb > 2100000000 && ( pItemProp->dwItemKind1 == IK1_WEAPON || pItemProp->dwItemKind1 == IK1_ARMOR ) )
		{

			
			if ( !pItemProp->IsBaruna() && !pItemProp->IsUltimate() )	pItemElem->SetAbilityOption( dwEnhanceRandom+1 );		// �߰� �ɷ�ġ +1, +2 ������.
			else if ( dwEnhanceProb > 2700000000)						pItemElem->SetAbilityOption( 1 );		// �߰� �ɷ�ġ +1, +2 ������.
			
		}

		
		
		DROPITEM* lpDropItem;
		for ( int i = 0; i < nSize; i++ )
		{
			if( ( lpDropItem = lpMoverProp->m_DropItemGenerator.GetAt( i, FALSE, 100.0f ) ) != NULL )
			{
				if ( lpDropItem->dwIndex == dwItemID )
					pItemElem->SetAbilityOption( lpDropItem->dwLevel );		// �߰� �ɷ�ġ +1, +2 ������.
			}
		}
		// end

		pItemElem->SetSerialNumber();

		CItem* pItem			= new CItem;
		pItem->m_pItemBase		= pItemElem;
		pItem->m_idOwn			= dwOwnerPlayerID;		// �� �������� ������ pAttacker(����Ŀ)������ ǥ��.
		pItem->m_dwDropTime		= ::GetTickCount();		// ��� ��������� �ð��� �����.
		pItem->m_bDropMob		= TRUE;		// ���� �׾ ���� ���� ǥ�ø� �ص�.
		pItem->SetIndex( D3DDEVICE, pItem->m_pItemBase->m_dwItemId );

		D3DVECTOR vPos			= kMonster.GetPos();
		vPos.x					+= ( xRandomF( 2.0f ) - 1.0f );
		vPos.z					+= ( xRandomF( 2.0f ) - 1.0f );
// 		vPos.y					+= 0.1f;									//���� y���� üũ�ϸ� �ٴ����� ��������.vPos.y
// 		vPos.y					= pWorld->GetUnderHeight( vPos );
		pItem->SetPos( vPos );

		if( pWorld->ADDOBJ( pItem, TRUE, kAttacker.GetLayer() ) == FALSE )
		{
			SAFE_DELETE( pItem );
			return;
		}

		if( pItemProp->dwItemKind1 == IK1_WEAPON || pItemProp->dwItemKind1 == IK1_ARMOR || ( pItemProp->dwItemKind1 == IK1_GENERAL && pItemProp->dwItemKind2 == IK2_JEWELRY ) )
		{
			switch( pItemProp->nLog )
			{
			case 1:		g_dpDBClient.SendLogUniqueItem( &kAttacker, pItem, pItemElem->GetAbilityOption() );	// �Ϲ� ������ +������
				break;
			case 2:		g_dpDBClient.SendLogUniqueItem( &kAttacker, pItem, 200 );	// ����Ʈ ������
				break;
			case 3:		g_dpDBClient.SendLogUniqueItem( &kAttacker, pItem, 100 );	// ����ũ ������
				break;
			}
		}
	}
}


void FLDoDrop::DropGold( CMover & kMonster, FLWSUser & kAttacker, const DWORD dwOwnerPlayerID, const int nGold )
{
	if( nGold <= 0 )
		return;

	CWorld* pWorld = kAttacker.GetWorld();
	if( pWorld == NULL )
	{
		return;
	}

	//1. ��� ������ ����
	const DWORD dwItemID	= FLGoldIconSet::GetInstance().GetIconItemID( nGold );
	const PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( dwItemID );
	if( pItemProp == NULL ) {
		return;
	}

	if( !addGoldToFlyingAttacker( kMonster, kAttacker, nGold ) ) {
		FLItemElem* pItemElem	= new FLItemElem;
		pItemElem->m_dwItemId	= dwItemID;
		pItemElem->m_nItemNum	= nGold;	// ���׼�
		pItemElem->m_nHitPoint	= nGold;

		CItem* pItem			= new CItem;
		pItem->m_pItemBase		= pItemElem;
		pItem->m_idOwn			= dwOwnerPlayerID;	// �� �������� ������ pAttacker(����Ŀ)������ ǥ��.
		pItem->m_dwDropTime		= ::GetTickCount();	// ��� ��������� �ð��� �����.
		pItem->m_bDropMob = TRUE;		// ���� �׾ ���� ���� ǥ�ø� �ص�.
		pItem->SetIndex( D3DDEVICE, pItem->m_pItemBase->m_dwItemId );

		D3DVECTOR vPos			= kMonster.GetPos();
		vPos.x					+= ( xRandomF( 2.0f ) - 1.0f );
		vPos.z					+= ( xRandomF( 2.0f ) - 1.0f );
// 		vPos.y					+= 0.1f;									//���� y���� üũ�ϸ� �ٴ����� ��������.vPos.y
// 		vPos.y					= pWorld->GetUnderHeight( vPos );
		pItem->SetPos( vPos );
		if( pWorld->ADDOBJ( pItem, TRUE, kAttacker.GetLayer() ) == FALSE )
		{
			SAFE_DELETE( pItem );
		}
	}
}

//
//-- drop item
//{
//	FLItemElem* pItemElem	= new FLItemElem;
//	pItemElem->m_dwItemId	= lpDropItem->dwIndex;
//	pItemElem->m_nItemNum	= (int)( xRandom( dwNum ) + 1 );
//	PT_ITEM_SPEC pItemProp		= pItemElem->GetProp();
//	if( pItemProp )
//	{
//		pItemElem->m_nHitPoint	= pItemProp->dwEndurance;
//		pItemElem->SetRandomOpt( CRandomOptItemGen::GetInstance()->GenRandomOptItem( lpMoverProp->dwLevel, (FLOAT)nProbability / 100.0f, pItemProp, lpMoverProp->dwClass ) );
//	}
//	pItemElem->SetAbilityOption( lpDropItem->dwLevel );		// �߰� �ɷ�ġ +1, +2 ������.
//	pItemElem->SetSerialNumber();
//	CItem* pItem	= new CItem;
//	pItem->m_pItemBase	= pItemElem;
//	BOOL bJJim = TRUE;		// ������ �ִٴ°� ǥ���ؾ��ϴ� ���ΰ�.
//	if( lpMoverProp->dwClass == RANK_SUPER )		// �������� ����� �������� �ƹ��� ������ �ִ�.
//		bJJim = FALSE;
//	if( GetIndex() == OBJECT_INDEX( 196, MI_DEMIAN5 ) || GetIndex() == OBJECT_INDEX( 197, MI_KEAKOON5 ) || GetIndex() == OBJECT_INDEX( 198, MI_MUFFRIN5 ) )
//		bJJim = FALSE;		// ����� �̺�Ʈ���̹Ƿ� ����س��� �ȴ�. �ƹ��� ������ ����
//	if( bJJim )
//	{
//		pItem->m_idOwn = pAttacker->GetId();	// �� �������� ������ pAttacker(����Ŀ)������ ǥ��.
//		pItem->m_dwDropTime = timeGetTime();	// ��� ��������� �ð��� �����.
//	}
//	pItem->m_bDropMob = TRUE;		// ���� �׾ ���� ���� ǥ�ø� �ص�.
//	if( pItem->m_pItemBase->m_dwItemId == 0 ) FLERROR_LOG( PROGRAM_NAME, _T( "DropItem:2nd %s" ), GetName() );
//	pItem->SetIndex( D3DDEVICE, pItem->m_pItemBase->m_dwItemId );
//
//	vPos = GetPos();
//	vPos.x += ( xRandomF(2.0f) - 1.0f );
//	vPos.z += ( xRandomF(2.0f) - 1.0f );
//	vPos.y = GetPos().y;
//#ifdef __EVENT_MONSTER
//	// �̺�Ʈ ���Ͱ� ����� �������� ������ ID�� ����Ѵ�(���� �� �ݰ�...)
//	if( CEventMonster::GetInstance()->SetEventMonster( lpMoverProp->dwID ) )
//	{
//		// �̺�Ʈ ���ʹ� ������ �������� ���´�.
//		pItem->m_idOwn	= pAttacker->GetId();
//		pItem->m_dwDropTime		= timeGetTime();
//
//		pItem->m_IdEventMonster = lpMoverProp->dwID;
//		float fItemDropRange = CEventMonster::GetInstance()->GetItemDropRange(); 
//		vPos = GetPos();
//		vPos.x += ( xRandomF( fItemDropRange ) - (fItemDropRange / 2.0f) );
//		vPos.z += ( xRandomF( fItemDropRange ) - (fItemDropRange / 2.0f) );
//	}
//#endif // __EVENT_MONSTER
//	pItem->SetPos( vPos );
//
//	GetWorld()->ADDOBJ( pItem, TRUE, GetLayer() );
//
//	if( lpDropItem->dwNumber != (DWORD)-1 )
//		nNumber++;
//
//	if( pItemProp->dwItemKind1 == IK1_WEAPON || pItemProp->dwItemKind1 == IK1_ARMOR || ( pItemProp->dwItemKind1 == IK1_GENERAL && pItemProp->dwItemKind2 == IK2_JEWELRY ) )
//	{
//		switch( pItemProp->nLog )
//		{
//		case 1:		g_dpDBClient.SendLogUniqueItem( pAttacker, pItem, pItemElem->GetAbilityOption() );	// �Ϲ� ������ +������
//			break;
//		case 2:		g_dpDBClient.SendLogUniqueItem( pAttacker, pItem, 200 );	// ����Ʈ ������
//			break;
//		case 3:		g_dpDBClient.SendLogUniqueItem( pAttacker, pItem, 100 );	// ����ũ ������
//			break;
//		}
//	}
//
//	if( nNumber == lpMoverProp->m_DropItemGenerator.m_dwMax )
//		break;
//};
//
//
//drop gold
//// ���� �����Ƕ������� �Ѵ�.
//if( lpDropItem->dtType == DROPTYPE_SEED && k == 0 )
//{
//	int	nSeedID = 0;
//	int nNumGold = lpDropItem->dwNumber + xRandom( lpDropItem->dwNumber2 - lpDropItem->dwNumber );	// Number ~ Number2 ������ ������.
//	nNumGold	= nNumGold * nPenyaRate / 100;
//#ifdef __S1108_BACK_END_SYSTEM
//	nNumGold	= (int)( nNumGold * prj.m_fGoldDropRate * lpMoverProp->m_fPenya_Rate );
//	if( nNumGold == 0 )
//		continue;
//#else // __S1108_BACK_END_SYSTEM
//	nNumGold	*= prj.m_fGoldDropRate;
//#endif // __S1108_BACK_END_SYSTEM
//
//	nNumGold	= (int)( nNumGold * prj.m_EventLua.GetGoldDropFactor() );	//sun: 9, �̺�Ʈ (��� ��ũ��Ʈ ����)
//
//	if( lpMoverProp->dwFlying )
//	{
//		if( CanAdd( pAttacker->GetGold(), nNumGold ) )
//		{
//			pAttacker->AddGold( nNumGold );
//			( (FLWSUser*)pAttacker )->AddGoldText( nNumGold );
//		}
//	}
//	else
//	{
//		// ���׼��� ���� ������ �õ带 ������� �����Ѵ�.
//		if( nNumGold <= (int)( g_xSpecManager->GetSpecItem( ITEM_INDEX( 12, II_GOLD_SEED1 ) )->dwAbilityMax ) )
//			nSeedID = ITEM_INDEX( 12, II_GOLD_SEED1 );
//		else if( nNumGold <= (int)( g_xSpecManager->GetSpecItem( ITEM_INDEX( 13, II_GOLD_SEED2 ) )->dwAbilityMax ) )
//			nSeedID = ITEM_INDEX( 13, II_GOLD_SEED2 );
//		else if( nNumGold <= (int)( g_xSpecManager->GetSpecItem( ITEM_INDEX( 14, II_GOLD_SEED3 ) )->dwAbilityMax ) )
//			nSeedID = ITEM_INDEX( 14, II_GOLD_SEED3 );
//		else 
//			nSeedID = ITEM_INDEX( 15, II_GOLD_SEED4 );
//
//		FLItemElem* pItemElem	= new FLItemElem;
//		pItemElem->m_dwItemId	= nSeedID;
//		pItemElem->m_nItemNum	= nNumGold;	// ���׼�
//		pItemElem->m_nHitPoint	= nNumGold;
//		CItem* pItem	= new CItem;
//		pItem->m_pItemBase	= pItemElem;
//		BOOL bJJim = TRUE;
//		if( lpMoverProp->dwClass == RANK_SUPER )		// �������� ����� �������� �ƹ��� ������ �ִ�.
//			bJJim = FALSE;
//		if( GetIndex() == OBJECT_INDEX( 196, MI_DEMIAN5 ) || GetIndex() == OBJECT_INDEX( 197, MI_KEAKOON5 ) || GetIndex() == OBJECT_INDEX( 198, MI_MUFFRIN5 ) )
//			bJJim = FALSE;		// ����� �̺�Ʈ���̹Ƿ� ����س��� �ȴ�. �ƹ��� ������ ����
//		if( bJJim )
//		{
//			pItem->m_idOwn = pAttacker->GetId();	// �� �������� ������ pAttacker(����Ŀ)������ ǥ��.
//			pItem->m_dwDropTime = timeGetTime();	// ��� ��������� �ð��� �����.
//		}
//		pItem->m_bDropMob = TRUE;		// ���� �׾ ���� ���� ǥ�ø� �ص�.
//		if( pItem->m_pItemBase->m_dwItemId == 0 ) FLERROR_LOG( PROGRAM_NAME, _T( "DropItem: 3rd %s" ), GetName() );
//		pItem->SetIndex( D3DDEVICE, pItem->m_pItemBase->m_dwItemId );
//
//		vPos = GetPos();
//		vPos.x += ( xRandomF(2.0f) - 1.0f );
//		vPos.z += ( xRandomF(2.0f) - 1.0f );
//#ifdef __EVENT_MONSTER
//		// �̺�Ʈ ���Ͱ� ����� �������� ������ ID�� ����Ѵ�(���� �� �ݰ�...)
//		if( CEventMonster::GetInstance()->SetEventMonster( lpMoverProp->dwID ) )
//		{
//			// �̺�Ʈ ���ʹ� ������ �������� ���´�.
//			pItem->m_idOwn	= pAttacker->GetId();
//			pItem->m_dwDropTime		= timeGetTime();
//
//			pItem->m_IdEventMonster = lpMoverProp->dwID;
//			float fItemDropRange = CEventMonster::GetInstance()->GetItemDropRange(); 
//			vPos = GetPos();
//			vPos.x += ( xRandomF( fItemDropRange ) - (fItemDropRange / 2.0f) );
//			vPos.z += ( xRandomF( fItemDropRange ) - (fItemDropRange / 2.0f));
//		}
//#endif // __EVENT_MONSTER
//		pItem->SetPos( vPos );
//		GetWorld()->ADDOBJ( pItem, TRUE, GetLayer() );
//	}
//} // DROPTYPE_SEED

//-----------------------------------------------------------------------------------------------------------------------------------------------------//

//< CEventGeneric::GetInstance() >
//{
//
//	if( ( pAttacker->m_nLevel - (int)lpMoverProp->dwLevel ) < 10 )
//	{
//		int nNum	= 0;
//		CEventItem* pEventItem	= NULL;//	= CEventGeneric::GetInstance()->GetItem( &nNum );
//		while( pEventItem = CEventGeneric::GetInstance()->GetItem( &nNum ) )
//		{
//			FLItemElem* pItemElem	= new FLItemElem;
//			pItemElem->m_dwItemId	= pEventItem->m_dwItemId;
//			pItemElem->m_nItemNum	= nNum;
//			pItemElem->m_nHitPoint	= -1;
//			pItemElem->SetSerialNumber();
//			CItem* pItem	= new CItem;
//			pItem->m_pItemBase	= pItemElem;
//			pItem->m_idOwn	= pAttacker->GetId();
//			pItem->m_dwDropTime		= timeGetTime();
//			pItem->m_bDropMob	= TRUE;
//			pItem->SetIndex( D3DDEVICE, pItem->m_pItemBase->m_dwItemId );
//			D3DXVECTOR3 vPos = GetPos();
//			vPos.x += ( xRandomF(2.0f) - 1.0f );
//			vPos.z += ( xRandomF(2.0f) - 1.0f );
//			vPos.y = GetPos().y;
//			pItem->SetPos( vPos );
//			GetWorld()->ADDOBJ( pItem, TRUE, GetLayer() );
//		}
//		//sun: 9, �̺�Ʈ (��� ��ũ��Ʈ ����)
//		map<DWORD, int> mapItemList = prj.m_EventLua.GetItem( lpMoverProp->dwLevel );
//
//		for( map<DWORD, int>::iterator it=mapItemList.begin(); it!=mapItemList.end(); it++ )
//		{
//			FLItemElem* pItemElem	= new FLItemElem;
//			pItemElem->m_dwItemId	= it->first;
//			pItemElem->m_nItemNum	= it->second;
//			pItemElem->m_nHitPoint	= -1;
//			pItemElem->SetSerialNumber();
//			CItem* pItem	= new CItem;
//			pItem->m_pItemBase	= pItemElem;
//			pItem->m_idOwn	= pAttacker->GetId();
//			pItem->m_dwDropTime		= timeGetTime();
//			pItem->m_bDropMob	= TRUE;
//			pItem->SetIndex( D3DDEVICE, pItem->m_pItemBase->m_dwItemId );
//			D3DXVECTOR3 vPos = GetPos();
//			vPos.x += ( xRandomF(2.0f) - 1.0f );
//			vPos.z += ( xRandomF(2.0f) - 1.0f );
//			vPos.y = GetPos().y;
//			pItem->SetPos( vPos );
//			GetWorld()->ADDOBJ( pItem, TRUE, GetLayer() );
//		}
//	}
//}
//
//
//
//
//// ����Ʈ ������ ���
//QUESTITEM* pQuestItem;
//short nNum;
//u_long uSizeOfQuestItem	= lpMoverProp->m_QuestItemGenerator.GetSize();
//for( u_long i = 0; i < uSizeOfQuestItem; i++ ) 
//{
//	pQuestItem	= lpMoverProp->m_QuestItemGenerator.GetAt( i );
//	LPQUEST lpQuest = pAttacker->GetQuest( pQuestItem->dwQuest );
//	if( lpQuest && lpQuest->m_nState == pQuestItem->dwState )				//����Ʈ�� �ְ� .  ���°� ���ٸ�.
//	{
//		DWORD dwProbability	= pQuestItem->dwProbability;
//
//		if( xRandom( 3000000000 ) <= dwProbability ) 
//		{
//			if( pQuestItem->dwNumber == 0 )
//				FLERROR_LOG( PROGRAM_NAME, _T( "%s�� quest item drop %d��°�� dwNumber�� 0" ), GetName(), i );
//			nNum	= (short)( xRandom( pQuestItem->dwNumber ) + 1 );
//			if( pQuestItem->dwIndex == 0 )
//			{
//				FLERROR_LOG( PROGRAM_NAME, _T( "%s" ), lpMoverProp->szName );
//				break;
//			}
//
//			// party �϶� 
//			CParty* pParty	= g_PartyMng.GetParty( pAttacker->m_idparty );
//			if( pParty && pParty->IsMember( pAttacker->m_idPlayer ) )	// party
//			{
//				for( int j = 0; j < pParty->GetSizeofMember(); ++j )
//				{
//					PartyMember* pPartyMember	= &pParty->m_aMember[j];
//					CMover* pMember		= prj.GetUserByID( pPartyMember->m_uPlayerId );
//					if( !IsValidObj( pMember ) || !IsValidArea( pMember, 64.0f ) )
//						continue;
//
//					LPQUEST pMemberQuest	= pMember->GetQuest( pQuestItem->dwQuest );
//					if( pMemberQuest && pMemberQuest->m_nState == pQuestItem->dwState )
//					{
//						BYTE nId;
//						FLItemElem itemElem;
//						itemElem.m_dwItemId		= pQuestItem->dwIndex;
//						itemElem.m_nItemNum		= nNum;
//						itemElem.m_nHitPoint	= -1;
//						if( pMember->CreateItem( &itemElem, &nId ) == FALSE )
//						{
//							FLItemElem* pItemElem	= new FLItemElem;
//							pItemElem->m_dwItemId	= pQuestItem->dwIndex;
//							pItemElem->m_nItemNum	= nNum;
//							PT_ITEM_SPEC pItemProp		= pItemElem->GetProp();
//							if( pItemProp )
//								pItemElem->m_nHitPoint		= pItemProp->dwEndurance;
//							pItemElem->SetSerialNumber();
//							CItem* pItem	= new CItem;
//							pItem->m_pItemBase	= pItemElem;
//							if( pItemElem->m_dwItemId == 0 )
//								FLERROR_LOG( PROGRAM_NAME, _T( "DropItem:1st %s" ), GetName() );
//							pItem->SetIndex( D3DDEVICE, pItemElem->m_dwItemId );
//							pItem->SetPos( pMember->GetPos() );
//							pItem->SetAngle( (float)( xRandom( 360 ) ) );
//							pItem->m_idHolder	= pMember->m_idPlayer;
//							pItem->m_dwDropTime		= timeGetTime();
//							pMember->GetWorld()->ADDOBJ( pItem, TRUE, GetLayer() );
//						}
//						else
//						{
//							( (FLWSUser*)pMember )->AddDefinedText( TID_EVE_REAPITEM, "\"%s\"", g_xSpecManager->GetSpecItem( pQuestItem->dwIndex )->szName );
//							FLItemBase* pItemBase	= pMember->GetItemId( nId );
//							if( pItemBase )
//							{
//								FLItemElem* pItemElem	= (FLItemElem*)pItemBase;
//								LogItemInfo aLogItem;
//								aLogItem.Action = "Q";
//								aLogItem.SendName = pMember->m_szName;
//								aLogItem.RecvName = "QUEST";
//								aLogItem.WorldId = pMember->GetWorld()->GetID();
//								aLogItem.Gold = aLogItem.Gold2 = pMember->GetGold();
//								g_DPSrvr.OnLogItem( aLogItem, pItemElem, pItemElem->m_nItemNum );
//							}
//						}
//					}
//				}
//			}
//			else	// solo
//			{
//				BYTE nId;
//				FLItemElem itemElem;
//				itemElem.m_dwItemId		= pQuestItem->dwIndex;
//				itemElem.m_nItemNum		= nNum;
//				itemElem.m_nHitPoint	= -1;
//				if( pAttacker->CreateItem( &itemElem, &nId ) == FALSE )
//				{
//					FLItemElem* pItemElem	= new FLItemElem;
//					pItemElem->m_dwItemId	= pQuestItem->dwIndex;
//					pItemElem->m_nItemNum	= nNum;
//					PT_ITEM_SPEC pItemProp		= pItemElem->GetProp();
//					if( pItemProp )
//						pItemElem->m_nHitPoint		= pItemProp->dwEndurance;
//					pItemElem->SetSerialNumber();
//					CItem* pItem	= new CItem;
//					pItem->m_pItemBase	= pItemElem;
//					if( pItemElem->m_dwItemId == 0 )
//						FLERROR_LOG( PROGRAM_NAME, _T( "DropItem:1st %s" ), GetName() );
//					pItem->SetIndex( D3DDEVICE, pItemElem->m_dwItemId );
//					pItem->SetPos( pAttacker->GetPos() );
//					pItem->SetAngle( (float)( xRandom( 360 ) ) );
//					pItem->m_idHolder	= pAttacker->m_idPlayer;
//					pItem->m_dwDropTime		= timeGetTime();
//					pAttacker->GetWorld()->ADDOBJ( pItem, TRUE, GetLayer() );
//				}
//				else
//				{
//					( (FLWSUser*)pAttacker )->AddDefinedText( TID_EVE_REAPITEM, "\"%s\"", g_xSpecManager->GetSpecItem( pQuestItem->dwIndex )->szName );
//					FLItemBase* pItemBase	= pAttacker->GetItemId( nId );
//					if( pItemBase )
//					{
//						FLItemElem* pItemElem	= (FLItemElem*)pItemBase;
//						LogItemInfo aLogItem;
//						aLogItem.Action = "Q";
//						aLogItem.SendName = pAttacker->m_szName;
//						aLogItem.RecvName = "QUEST";
//						aLogItem.WorldId = pAttacker->GetWorld()->GetID();
//						aLogItem.Gold = aLogItem.Gold2 = pAttacker->GetGold();
//						g_DPSrvr.OnLogItem( aLogItem, pItemElem, pItemElem->m_nItemNum );
//					}
//				}
//			}
//		}
//	}
//}
//
//
//< ������ ��뿡 ���� Ƚ�� ���� >
////sun: 9, // __II_SYS_SYS_SCR_GET
//if( pAttacker->HasBuff( BUFF_ITEM, ITEM_INDEX( 30115, II_SYS_SYS_SCR_GET01 ) ) )
//nloop += 1;
//if( pAttacker->HasBuff( BUFF_ITEM, ITEM_INDEX( 30116, II_SYS_SYS_SCR_GET02 ) ) )
//nloop += 2;
//
//nloop += pAttacker->GetAdjParam( DST_GIFTBOX );
//
//
//< �̺�Ʈ�� ���� Ƚ�� ���� >
//#ifdef __EVENT_MONSTER
//// �̺�Ʈ ���ʹ� ������ ���� �ѹ��� ����Ѵ�.
//if( CEventMonster::GetInstance()->SetEventMonster( lpMoverProp->dwID ) )
//if( !CEventMonster::GetInstance()->IsGiftBoxAble() )
//nloop = 1;
//#endif // __EVENT_MONSTER
//
//
//< ���� �׾����� ������ �b�� >
//if( IsNPC() )	// ���� �׾�����.. // ���� ������ �b�� �ε�??????
//{
//	MoverProp* pMoverProp	= GetProp();
//	if( pMoverProp->m_nLoot == 2 )	// 2�� d�ɼ�.  ������ �԰� ������ �ϴ� �ɼ�.
//	{
//		DWORD dwStart = m_Inventory.GetMax() - m_Inventory.GetSize();
//		DWORD dwSize = m_Inventory.GetMax();
//		FLItemElem *pElem = NULL;
//		for( DWORD i = dwStart; i < dwSize; i ++ )
//		{
//			pElem = m_Inventory.GetAt( i );
//			if( pElem )
//			{
//				D3DXVECTOR3 vPos = GetPos();
//#ifdef __WORLDSERVER
//				vPos.x += ( xRandomF(2.0f) - 1.0f );
//				vPos.z += ( xRandomF(2.0f) - 1.0f );
//#endif
//				CItem *pItem = DropItem( i, pElem->m_nItemNum, vPos, FALSE );
//				pItem->m_idOwn	= pAttacker->GetId();
//				pItem->m_dwDropTime		= timeGetTime();
//			}
//		}
//	}
//}
//


