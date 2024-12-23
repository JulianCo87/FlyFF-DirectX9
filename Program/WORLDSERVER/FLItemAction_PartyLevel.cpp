
#include "StdAfx.h"
#include "FLItemAction_PartyLevel.h"

#include "../_Common/party.h"
extern CPartyMng g_PartyMng;


FLItemAction_PartyLevelUp & FLItemAction_PartyLevelUp::GetInstance()
{
	static FLItemAction_PartyLevelUp sPartyLevel;
	
	return sPartyLevel;
}

bool	FLItemAction_PartyLevelUp::Use( FLWSUser & kUser, FLItemElem & kItemElem, CAr & /*ar*/ )
{
	const PT_ITEM_SPEC pItemProp	= g_xSpecManager->GetSpecItem( kItemElem.m_dwItemId );
	if( pItemProp == NULL )
	{
		return false;
	}

	CParty* pParty	= g_PartyMng.GetParty( kUser.m_idparty );
	if( pParty == NULL )
	{
		kUser.AddDefinedText( TID_MMI_PARTYMASSAGEERROR02 );	// �ش� �Ҽ��� �ƴ� ���
		return false;
	}

	if( pParty->IsLeader( kUser.m_idPlayer ) == FALSE )
	{
		kUser.AddDefinedText( TID_MMI_PARTYMASSAGEERROR01 );	// ������ �ƴ� ���
		return false;
	}

	if( pParty->GetLevel() >= MIN_PARSKILL_MODE_LEVEL )			// �ش� ���� üũ
	{
		kUser.AddDefinedText( TID_MMI_PARTYMASSAGEERROR03 );
		return false;
	}

	/*
	�ش� �����̶���� �����ִ� ����Ʈ üũ�� ���� �ʰ� ������ ���� ������ �ϴ°��� ��ȹ �ǵ���...
	*/

// 	for( size_t Nth = 0; Nth < MAX_ITEM_PARAM; ++Nth )
// 	{
// 		if( pItemProp->dwDestParam[ Nth ] == DST_PARTY_LEVEL_UP )
// 		{
// 			pParty->SetPartyLevel( &kUser, pItemProp->nAdjParamVal[ Nth ], pItemProp->nEffectValue, 0 );
// 		}
// 	}

	pParty->SetPartyLevel( &kUser, MIN_PARSKILL_MODE_LEVEL, pItemProp->nEffectValue, 0 );

	g_dpDBClient.SendLogSMItemUse( "1", &kUser, &kItemElem, pItemProp );
	kUser.RemoveItem( kItemElem.m_dwObjId, 1 );

	return true;
}
