#include "stdafx.h"
#include ".\itemupgrade.h"
#include <collecting.h>
#include "User.h"
#include <AIPet.h>
#include "FLBarunaUpgradeNpc.h"

CItemUpgrade::CItemUpgrade(void)
{
	LoadScript();
}

CItemUpgrade::~CItemUpgrade(void)
{
	m_mapSuitProb.clear();
#ifdef BARUNA_UPGRADE_SUIT_PIERCING
	m_mapBarunaSuitProb.clear();
#endif
	m_mapWeaponProb.clear();
	m_mapGeneralEnchant.clear();
#ifdef COSTUME_UPGRADE_ENHANCEMENT_GEM
	m_mapCostumeEnchant.clear();
#endif
	m_mapAttributeEnchant.clear();
#ifdef BARUNA_UPGRADE_SMELT_SAFETY
	m_mapBarunaNPCLevel.clear();
	m_mapBarunaWeaponEnchant.clear();
	m_mapBarunaArmorEnchant.clear();
#endif
}

CItemUpgrade* CItemUpgrade::GetInstance( void )
{	
	static CItemUpgrade sItemUpgrade;
	return &sItemUpgrade;
}

void CItemUpgrade::LoadScript()
{
	CLuaBase	lua;
	const char * szFileName = "ItemUpgrade.lua";

	TCHAR szFullPath[ MAX_PATH ]	= { 0, };
	g_pScriptFileManager->GetScriptFileFullPath( szFullPath, _countof( szFullPath ), szFileName );

	if( lua.RunScript( szFullPath ) != 0 )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "[ FAILED LOAD SCRIPT. FILE_NAME:(%s) ]" ), szFullPath );
		FLASSERT(0);
	}

	// �� �Ǿ��
	lua.GetGloabal( "tSuitProb" );
	lua.PushNil();
	while( lua.TableLoop( -2 ) )
	{
		m_mapSuitProb.insert( make_pair( static_cast<int>(lua.ToNumber(-2)), static_cast<int>(lua.ToNumber(-1)) ) );
		lua.Pop( 1 );
	}
	lua.Pop(0);

#ifdef BARUNA_UPGRADE_SUIT_PIERCING
	/*// �ٷ糪 �� �Ǿ��
	lua.GetGloabal( "tBarunaSuitProb" );
	lua.PushNil();
	while( lua.TableLoop( -2 ) )
	{
		m_mapBarunaSuitProb.insert( make_pair( static_cast<int>(lua.ToNumber(-2)), static_cast<int>(lua.ToNumber(-1)) ) );
		lua.Pop( 1 );
	}
	lua.Pop(0);*/
#endif
	// ���� �Ǿ��
	lua.GetGloabal( "tWeaponProb" );
	lua.PushNil();
	while( lua.TableLoop( -2 ) )
	{
		m_mapWeaponProb.insert( make_pair( static_cast<int>(lua.ToNumber(-2)), static_cast<int>(lua.ToNumber(-1)) ) );
		lua.Pop( 1 );
	}
	lua.Pop(0);
	
	// �Ϲ�����
	lua.GetGloabal( "tGeneral" );
	lua.PushNil();
	while( lua.TableLoop( -2 ) )
	{
		m_mapGeneralEnchant.insert( make_pair( static_cast<int>(lua.ToNumber(-2)), static_cast<int>(lua.ToNumber(-1)) ) );
		lua.Pop( 1 );
	}
	lua.Pop(0);
#ifdef COSTUME_UPGRADE_ENHANCEMENT_GEM
	// �ڽ�Ƭ ����
	lua.GetGloabal( "tCostumeEnhance" );
	lua.PushNil();
	while( lua.TableLoop( -2 ) )
	{
		m_mapCostumeEnchant.insert( make_pair( static_cast<int>(lua.ToNumber(-2)), static_cast<int>(lua.ToNumber(-1)) ) );
		lua.Pop( 1 );
	}
	lua.Pop(0);
#endif
 	// �Ӽ�����
	lua.GetGloabal( "tAttribute" );
 	lua.PushNil();
 	while( lua.TableLoop( -2 ) )
 	{
 		__ATTRIBUTE_ENCHANT attrEnchant;
		attrEnchant.nProb = static_cast<int>(lua.GetFieldToNumber( -1, "nProb" ));
		attrEnchant.nAddDamageRate = static_cast<int>(lua.GetFieldToNumber( -1, "nDamageRate" ));
		attrEnchant.nDefenseRate = static_cast<int>(lua.GetFieldToNumber( -1, "nDefenseRate" ));
		attrEnchant.nAddAtkDmgRate = static_cast<int>(lua.GetFieldToNumber( -1, "nAddAtkDmgRate" ));
		m_mapAttributeEnchant.insert( make_pair( static_cast<int>(lua.ToNumber(-2)), attrEnchant ) );  
		lua.Pop( 1 );
  	}
	lua.Pop(0);
#ifdef BARUNA_UPGRADE_SMELT_SAFETY
	// �ٷ糪 ����
	lua.GetGloabal( "tBarunaNPCLevel" );
 	lua.PushNil();
 	while( lua.TableLoop( -2 ) )
 	{
 		__BARUNA_NPC_LEVEL barunaNPCLevel;
		barunaNPCLevel.dwExpRequired = static_cast<DWORD>(lua.GetFieldToNumber( -1, "dwExpRequired" ));
		barunaNPCLevel.dwAdditionalSuccessProp = static_cast<DWORD>(lua.GetFieldToNumber( -1, "dwAdditionalSuccessProp" ));
		
		m_mapBarunaNPCLevel.insert( make_pair( static_cast<int>(lua.ToNumber(-2)), barunaNPCLevel ) );  
		lua.Pop( 1 );
  	}
	lua.Pop(0);

	lua.GetGloabal( "tBarunaWeaponEnchant" );
 	lua.PushNil();
 	while( lua.TableLoop( -2 ) )
 	{
 		__BARUNA_ENCHANT barunaWeaponEnchant;
		barunaWeaponEnchant.dwSuccessAdditionalExp = static_cast<DWORD>(lua.GetFieldToNumber( -1, "dwSuccessAdditionalExp" ));
		barunaWeaponEnchant.dwFailAddtionalExp = static_cast<DWORD>(lua.GetFieldToNumber( -1, "dwFailAddtionalExp" ));
		barunaWeaponEnchant.dwMinusAdditionalExp = static_cast<DWORD>(lua.GetFieldToNumber( -1, "dwMinusAdditionalExp" ));
		barunaWeaponEnchant.dwSuccessProb = static_cast<DWORD>(lua.GetFieldToNumber( -1, "dwSuccessProb" ));
		barunaWeaponEnchant.dwFailProb = static_cast<DWORD>(lua.GetFieldToNumber( -1, "dwFailProb" ));
		barunaWeaponEnchant.dwMinusProb = 10000 - barunaWeaponEnchant.dwSuccessProb - barunaWeaponEnchant.dwFailProb;
		m_mapBarunaWeaponEnchant.insert( make_pair( static_cast<int>(lua.ToNumber(-2)), barunaWeaponEnchant ) );  
		lua.Pop( 1 );
  	}
	lua.Pop(0);

	lua.GetGloabal( "tBarunaArmorEnchant" );
 	lua.PushNil();
 	while( lua.TableLoop( -2 ) )
 	{
 		__BARUNA_ENCHANT barunaArmorEnchant;
		barunaArmorEnchant.dwSuccessAdditionalExp = static_cast<DWORD>(lua.GetFieldToNumber( -1, "dwSuccessAdditionalExp" ));
		barunaArmorEnchant.dwFailAddtionalExp = static_cast<DWORD>(lua.GetFieldToNumber( -1, "dwFailAddtionalExp" ));
		barunaArmorEnchant.dwMinusAdditionalExp = static_cast<DWORD>(lua.GetFieldToNumber( -1, "dwMinusAdditionalExp" ));
		barunaArmorEnchant.dwSuccessProb = static_cast<DWORD>(lua.GetFieldToNumber( -1, "dwSuccessProb" ));
		barunaArmorEnchant.dwFailProb = static_cast<DWORD>(lua.GetFieldToNumber( -1, "dwFailProb" ));
		barunaArmorEnchant.dwMinusProb = 10000 - barunaArmorEnchant.dwSuccessProb - barunaArmorEnchant.dwFailProb;
		m_mapBarunaArmorEnchant.insert( make_pair( static_cast<int>(lua.ToNumber(-2)), barunaArmorEnchant ) );  
		lua.Pop( 1 );
  	}
	lua.Pop(0);
#endif
}


void CItemUpgrade::OnPiercingSize( FLWSUser* pUser, DWORD dwId1, DWORD dwId2, DWORD dwId3 )
{
	FLItemElem* pItemElem0	= pUser->m_Inventory.GetAtId( dwId1 );
	FLItemElem* pItemElem1	= pUser->m_Inventory.GetAtId( dwId2 );
	FLItemElem* pItemElem2	= pUser->m_Inventory.GetAtId( dwId3 );

	if( IsUsableItem( pItemElem0 ) == FALSE || IsUsableItem( pItemElem1 ) == FALSE )
	{
		return;
	}

	PT_ITEM_SPEC pMainProp		= g_xSpecManager->GetSpecItem( pItemElem0->m_dwItemId );
	PT_ITEM_SPEC pMaterialProp	= g_xSpecManager->GetSpecItem( pItemElem1->m_dwItemId );
	if( pMainProp == NULL || pMaterialProp == NULL )
	{
		return;
	}

	if( pMainProp->IsUpgradeAble( IUTYPE_PIERCING ) == FALSE )
	{
		pUser->AddDefinedText( TID_PIERCING_POSSIBLE_ITEM );
		return;
	}

	if( pMainProp->IsBaruna() == TRUE 
#ifdef BARUNA_UPGRADE_SUIT_PIERCING
		&& pMainProp->dwItemKind3 != IK3_SUIT
#endif
		)
	{
		pUser->AddDefinedText( TID_PIERCING_POSSIBLE_ITEM );
		return;
	}

	if( pUser->m_vtInfo.GetOther() || pUser->m_vtInfo.VendorIsVendor() ) // �ŷ� ���̸�...
		return;	
	
	if( pUser->m_Inventory.IsEquip( pItemElem0->m_dwObjId ) )
	{
		pUser->AddDefinedText( TID_GAME_EQUIPPUT );
		return;
	}	

	//////////////// ù��° ������ //////////////// 
	if( !pItemElem0->IsPierceAble( NULL_ID, TRUE ) )
	{
		pUser->AddDefinedText( TID_PIERCING_POSSIBLE_ITEM );
		return;
	}

#ifdef BARUNA_UPGRADE_SUIT_PIERCING
	if( pMainProp->IsBarunaArmor())
	{
		if( pMaterialProp->dwID != ITEM_INDEX( 25580, II_GEN_MAT_COSMOSTONE ) )
		{
			pUser->AddDefinedText( TID_SBEVE_NOTUSEITEM );			// �Ǿ�̿� �ʿ��� �ֻ����� �ƴϸ� �Ұ���
			return;
		}
		
		if( IsUsableItem( pItemElem2 ) && pItemElem2->m_dwItemId != ITEM_INDEX( 25581, II_SYS_SYS_SCR_PIEPROT3 ))
		{
			pUser->AddDefinedText( TID_SBEVE_NOTUSEITEM );			// ���������� �ƴϸ� �Ұ���
			return;
		}
	}
	else
#endif
	{
		if( pMaterialProp->dwID != ITEM_INDEX( 2036, II_GEN_MAT_MOONSTONE ) 
			&& pMaterialProp->dwID != ITEM_INDEX( 2083, II_GEN_MAT_MOONSTONE_1 ) )
		{
			pUser->AddDefinedText( TID_SBEVE_NOTUSEITEM );			// �Ǿ�̿� �ʿ��� �ֻ����� �ƴϸ� �Ұ���
			return;
		}
		
		if( IsUsableItem( pItemElem2 ) && pItemElem2->m_dwItemId != ITEM_INDEX( 10465, II_SYS_SYS_SCR_PIEPROT ) && pItemElem2->m_dwItemId != ITEM_INDEX( 25234, II_SYS_SYS_SCR_PIEPROT2 ))
		{
			pUser->AddDefinedText( TID_SBEVE_NOTUSEITEM );			// ���������� �ƴϸ� �Ұ���
			return;
		}
	}

	LogItemInfo aLogItem;
	//aLogItem.SendName = pUser->GetName();
	//aLogItem.RecvName = "PIERCING";
	FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
	FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "PIERCING" );
	aLogItem.WorldId = pUser->GetWorld()->GetID();

	int nCost = 100000;

	if( 0 < nCost )
	{
//		if( pUser->GetGold() < nCost )
		if( pUser->CheckUserGold( nCost, false ) == false )
		{
			pUser->AddDefinedText( TID_GAME_LACKMONEY , "" );
			return;
		}

		pUser->AddGold( -( nCost ) );

		aLogItem.Gold = pUser->GetGold() + nCost;
		aLogItem.Gold2 = pUser->GetGold();
		//aLogItem.Action = "!";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "!" );
		//aLogItem.ItemName = "SEED";
		FLSPrintf( aLogItem.kLogItem.szItemName, _countof( aLogItem.kLogItem.szItemName ), "%d", ITEM_INDEX( 12, II_GOLD_SEED1 ) );
		aLogItem.kLogItem.nQuantity = nCost;
		g_DPSrvr.OnLogItem( aLogItem );
	}
	else
	{
		return;
	}
	aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();

	int nPersent = 0;
	if( pItemElem1->GetProp()->dwID == ITEM_INDEX( 2036, II_GEN_MAT_MOONSTONE )
		|| pItemElem1->GetProp()->dwID == ITEM_INDEX( 2083, II_GEN_MAT_MOONSTONE_1 )
#ifdef BARUNA_UPGRADE_SUIT_PIERCING 
		|| pItemElem1->GetProp()->dwID == ITEM_INDEX( 25580, II_GEN_MAT_COSMOSTONE ) 
#endif
		)
		nPersent = GetSizeProb( pItemElem0 );

	if( nPersent < (int)( xRandom( 10000 ) ) )
	{	// ����
		if( pItemElem2 )								// ���ȭ �������� ����Ͽ�����...
			FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "PIERCING_PROTECTED" );
//			aLogItem.RecvName = "PIERCING_PROTECTED";
		//aLogItem.Action = "!";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "!" );
		g_DPSrvr.OnLogItem( aLogItem, pItemElem0, pItemElem0->m_nItemNum );
		//aLogItem.RecvName = "PIERCING";
		FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "PIERCING" );

		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );
		g_xWSUserManager->AddCreateSfxObj((CMover *)pUser, XI_INDEX( 1715, XI_INT_FAIL ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);
		pUser->AddDefinedText( TID_MMI_PIERCINGFAIL , "" );
		
		if( pItemElem2 == NULL )								// ���ȭ �������� ������� �ʾҴٸ� 
			pUser->RemoveItem( dwId1, 1 );	// �Ǿ�� ��� �������� �����ȴ�.			
	}
	else
	{	// ����			
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );			
		g_xWSUserManager->AddCreateSfxObj((CMover *)pUser, XI_INDEX( 1714, XI_INT_SUCCESS ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);			
		pUser->UpdateItem( pItemElem0->m_dwObjId, UI_PIERCING_SIZE, pItemElem0->GetGeneralPiercingSize() + 1 );
		pUser->AddDefinedText( TID_MMI_PIERCINGSUCCESS , "" );

		//aLogItem.Action = "#";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "#" );
		g_DPSrvr.OnLogItem( aLogItem, pItemElem0, pItemElem0->m_nItemNum );
	}
	//aLogItem.Action = "!";
	FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "!" );
	g_DPSrvr.OnLogItem( aLogItem, pItemElem1, pItemElem1->m_nItemNum );

	// ���̽��� ��������� �����Ѵ�.
	pUser->RemoveItem( dwId2, 1 );

	if( dwId3 != NULL_ID )
	{
		//aLogItem.Action = "!";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "!" );
		if( IsUsableItem( pItemElem2 ) )
		{
			g_DPSrvr.OnLogItem( aLogItem, pItemElem2, pItemElem2->m_nItemNum );
			g_dpDBClient.SendLogSMItemUse( "2", pUser, pItemElem2, pItemElem2->GetProp() );
			pUser->RemoveItem( dwId3, 1 );
		}
		else
		{
			g_DPSrvr.OnLogItem( aLogItem, NULL, 0 );
		}
	}
}

int CItemUpgrade::GetSizeProb( FLItemElem* pItemElem )
{
	// �������...IK3_SOCKETCARD�� �����̸� ��Ʈ...
	if( pItemElem->IsPierceAble( IK3_SOCKETCARD ) )
	{
		//return m_vecSuitProb.size() >= pItemElem->GetGeneralPiercingSize() ? m_vecSuitProb[pItemElem->GetGeneralPiercingSize()] : 0;
		map<int, int>::iterator it = m_mapSuitProb.find( pItemElem->GetGeneralPiercingSize()+1 );
		if( it != m_mapSuitProb.end() )
			return it->second;
	}
#ifdef BARUNA_UPGRADE_SUIT_PIERCING
	// IK3_SOCKETCARD�� �����̸� ��Ʈ...
	if( pItemElem->IsPierceAble( IK3_SOCKETCARD3 ) )
	{
		//return m_vecSuitProb.size() >= pItemElem->GetGeneralPiercingSize() ? m_vecSuitProb[pItemElem->GetGeneralPiercingSize()] : 0;
		map<int, int>::iterator it = m_mapBarunaSuitProb.find( pItemElem->GetGeneralPiercingSize()+1 );
		if( it != m_mapBarunaSuitProb.end() )
			return it->second;
	}
#endif
	
	// �������...IK3_SOCKETCARD2�� �����̸� ������...
	if( pItemElem->IsPierceAble( IK3_SOCKETCARD2 ) )
	{
		//return m_vecWeaponProb.size() >= pItemElem->GetGeneralPiercingSize() ? m_vecWeaponProb[pItemElem->GetGeneralPiercingSize()] : 0;
		map<int, int>::iterator it = m_mapWeaponProb.find( pItemElem->GetGeneralPiercingSize()+1 );
		if( it != m_mapWeaponProb.end() )
			return it->second;
	}

	return 0;
}

void CItemUpgrade::OnPiercing( FLWSUser* pUser, DWORD dwItemId, DWORD dwSocketCard )
{
	// �κ��丮�� �ִ��� �����Ǿ� �ִ��� Ȯ���� �ؾ� ��
	FLItemElem* pItemElem0	= pUser->m_Inventory.GetAtId( dwItemId );
	FLItemElem* pItemElem1	= pUser->m_Inventory.GetAtId( dwSocketCard );
	if( IsUsableItem( pItemElem0 ) == FALSE || IsUsableItem( pItemElem1 ) == FALSE )
		return;

	PT_ITEM_SPEC pMainProp		= g_xSpecManager->GetSpecItem( pItemElem0->m_dwItemId );
	PT_ITEM_SPEC pMaterialProp	= g_xSpecManager->GetSpecItem( pItemElem1->m_dwItemId );
    if( pMainProp == NULL || pMaterialProp == NULL )
	{
		return;
	}

	if( pMainProp->IsUpgradeAble( IUTYPE_PIERCING ) == FALSE )
	{
		pUser->AddDefinedText(  TID_PIERCING_POSSIBLE_ITEM );
		return;
	}

	if( pMainProp->IsBaruna() == TRUE 
#ifdef BARUNA_UPGRADE_SUIT_PIERCING
&& pMainProp->dwItemKind3 != IK3_SUIT
#endif
		)
	{
		pUser->AddDefinedText( TID_PIERCING_POSSIBLE_ITEM );
		return;
	}

	// �����Ǿ� �ִ� �������� �Ǿ�� ����
	if( pUser->m_Inventory.IsEquip( pItemElem0->m_dwObjId ) )
	{
		pUser->AddDefinedText( TID_GAME_EQUIPPUT , "" );
		return;
	}			

	// ī�尡 �� �������� �Ǿ�� �������� �˻�
	if( !pItemElem0->IsPierceAble() )
	{
		pUser->AddDefinedText(  TID_PIERCING_POSSIBLE_ITEM, "" );
		return;
	}

	//  IK3_SOCKETCARD?�� �ƴϸ� �Ǿ�� ����
	if( !pItemElem0->IsPierceAble( pMaterialProp->dwItemKind3 ) )
	{
		pUser->AddDefinedText( TID_UPGRADE_ERROR_WRONGUPLEVEL , "" );			
		return;					
	}

	// �� �Ǿ�̵ȼ��� ��ü ���� ���Ѵ�.
	size_t nSize = pItemElem0->GetGeneralPiercingSize();

	size_t nCount = 0;
	for( size_t j = 0; j < nSize; j++ )
	{
		if( pItemElem0->GetGeneralPiercingItemID( j ) != 0 )
			nCount++;
	}

	// ����� ������ �ߴ�
	if( nCount == nSize )
	{
		pUser->AddDefinedText( TID_PIERCING_ERROR_NOPIERCING, "" );
		return;
	}

	// ˬ
	if( pUser->m_vtInfo.GetOther() )	// �ŷ����� ����� ������?
		return;
	if( pUser->m_vtInfo.VendorIsVendor() )		// ���� �Ȱ� ������?
		return;

	LogItemInfo aLogItem;
	//aLogItem.SendName = pUser->GetName();
	//aLogItem.RecvName = "PIERCING";
	FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
	FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "PIERCING" );
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();

	if( pUser->Pierce( pItemElem0, pItemElem1->m_dwItemId ) )
	{
		//aLogItem.Action = "$";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "$" );
		g_DPSrvr.OnLogItem( aLogItem, pItemElem0, pItemElem0->m_nItemNum );
		//aLogItem.Action = "!";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "!" );
		g_DPSrvr.OnLogItem( aLogItem, pItemElem1, pItemElem1->m_nItemNum );

		// ������ �ڱ� ����~
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );
		g_xWSUserManager->AddCreateSfxObj((CMover *)pUser, XI_INDEX( 1714, XI_INT_SUCCESS ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);
		// ��� ������ ����
		pUser->RemoveItem( dwSocketCard, 1 );
	}

}

void CItemUpgrade::OnPiercingRemove( FLWSUser* pUser, DWORD objId1, DWORD objId2 )
{
	FLItemElem* pItemElem = pUser->m_Inventory.GetAtId( objId1 );
	if( !IsUsableItem( pItemElem ) || !pItemElem->IsPierceAble() )
		return;

	if( pUser->m_Inventory.IsEquip( objId1 ) )
		return;

	PT_ITEM_SPEC pItemSpec = pItemElem->GetProp();
	if( pItemSpec == NULL )
	{
		return;
	}

	if( pItemSpec->IsBaruna() == TRUE 
#ifdef BARUNA_UPGRADE_SUIT_PIERCING
&& pItemSpec->dwItemKind3 != IK3_SUIT
#endif
)
	{
		return;
	}

#ifdef BARUNA_UPGRADE_SUIT_PIERCING
	FLItemElem* pItemElem2 = pUser->m_Inventory.GetAtId( objId2 );
	if( pItemSpec->IsBarunaArmor() )
	{
		if( !IsUsableItem( pItemElem2 ) )
			return;

		if( pUser->m_Inventory.IsEquip( objId2 ) )
			return;

		PT_ITEM_SPEC pItemSpec2 = pItemElem2->GetProp();
		if( pItemSpec2 == NULL )
		{
			return;
		}

//		if( pItemSpec2->dwItemKind3 != IK3_BARUNA_GEN_PIC_MAT_CANCEL )
		{
			return;
		}

		LogItemInfo aLogItem;
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "$" );
		FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
		FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "PIERCING_REMOVE_MATERIAL" );
		aLogItem.WorldId = pUser->GetWorld()->GetID();
		aLogItem.Gold = pUser->GetGold();
		aLogItem.Gold2 = pUser->GetGold();
		g_DPSrvr.OnLogItem( aLogItem, pItemElem2, 1 );

		// ��� ������ ����
		pUser->RemoveItem( objId2, 1 );
	}
#endif

	// �Ǿ�� �ɼ��� ���� ���
	if( pItemElem->GetGeneralPiercingSize() == 0 || pItemElem->GetGeneralPiercingItemID( 0 ) == 0 )
	{
		pUser->AddDefinedText( TID_GAME_REMOVE_PIERCING_ERROR );
		return;
	}

	int nPayPenya = 1000000; // ������ ���
//	if( pUser->GetGold() < nPayPenya )	// ��İ� �����ϴ�.
	if( pUser->CheckUserGold( nPayPenya, false ) == false )
	{
		pUser->AddDefinedText( TID_GAME_LACKMONEY );
		return;
	}

	g_xApplyItemEffect->RemoveEquipActiveSkillPiercing( pUser, pItemElem );

	for( int i=pItemElem->GetGeneralPiercingSize()-1; i>=0; i-- )
	{
		if( pItemElem->GetGeneralPiercingItemID( i ) != 0 )
		{
			pUser->AddGold( -nPayPenya );	// ��� ����
			pUser->AddDefinedText( TID_GAME_REMOVE_PIERCING_SUCCESS );
			pUser->UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_PIERCING, MAKELONG( i, 0 ) );

			LogItemInfo aLogItem;
			//aLogItem.Action = "$";
			//aLogItem.SendName = pUser->GetName();
			//aLogItem.RecvName = "PIERCING_REMOVE";
			FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "$" );
			FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
			FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "PIERCING_REMOVE" );
			aLogItem.WorldId = pUser->GetWorld()->GetID();
			aLogItem.Gold = pUser->GetGold() + nPayPenya;
			aLogItem.Gold2 = pUser->GetGold();
			aLogItem.Gold_1 = -nPayPenya;
			g_DPSrvr.OnLogItem( aLogItem, pItemElem, 1 );
			break;
		}
	}
}

void	CItemUpgrade::OnEnchant( FLWSUser* pUser, FLItemElem* pItemMain, FLItemElem* pItemMaterial )
{
	if( !IsUsableItem( pItemMain ) || !IsUsableItem( pItemMaterial ) )
	{
		return;
	}

	// ����� �������ΰ�?
	if( pUser->m_Inventory.IsEquip( pItemMain->m_dwObjId ) )
	{
		pUser->AddDefinedText( TID_GAME_EQUIPPUT , "" );
		return;
	}

	if( pItemMain->GetProp() == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "Main Item Property is NULL. ItemID:[%d]" ), pItemMain->m_dwItemId );
		return;
	}

	if( pItemMaterial->GetProp() == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "Material Item Property is NULL. ItemID:[%d]" ), pItemMaterial->m_dwItemId );
		return;
	}

	if( pItemMain->m_nResistSMItemId != 0 ) // ���ȭ ������ �������̸� �Ұ���
	{
		pUser->AddDefinedText( TID_GAME_NOTUPGRADE );
		return;
	}

	if( pItemMain->GetProp()->IsBaruna() == TRUE )
	{
		return;
	}
	
	switch( pItemMaterial->GetProp()->dwItemKind3 )
	{
		case IK3_ELECARD:	
			EnchantAttribute( pUser, pItemMain, pItemMaterial );
			break;

//		case IK3_ENCHANT:
		case IK3_SMELT_GENERAL_MATERIAL:
			EnchantGeneral( pUser, pItemMain, pItemMaterial );
			break;
#ifdef COSTUME_UPGRADE_ENHANCEMENT_GEM
		case IK3_SMELT_COSTUME_MATERIAL:
			EnchantCostume( pUser, pItemMain, pItemMaterial );
			break;
#endif
		default:
			if( pItemMain->IsAccessory() || pItemMain->GetProp()->IsAccessory() )
				RefineAccessory( pUser, pItemMain, pItemMaterial );

			else if( pItemMain->IsCollector() )
				RefineCollector( pUser, pItemMain, pItemMaterial );
			break;
	}
}

BYTE	CItemUpgrade::OnSmeltSafety( FLWSUser* pUser, FLItemElem* pItemMain, FLItemElem* pItemMaterial, FLItemElem* pItemProtScr, FLItemElem* pItemSmeltScr )
{
	if( IsUsableItem( pItemMain ) == FALSE || IsUsableItem( pItemMaterial ) == FALSE || IsUsableItem( pItemProtScr ) == FALSE )
	{
		return 0;
	}

	PT_ITEM_SPEC pMainProp		= g_xSpecManager->GetSpecItem( pItemMain->m_dwItemId );
	PT_ITEM_SPEC pMaterialProp	= g_xSpecManager->GetSpecItem( pItemMaterial->m_dwItemId );
	PT_ITEM_SPEC pProtScrProp	= g_xSpecManager->GetSpecItem( pItemProtScr->m_dwItemId );
	if( pMainProp == NULL || pMaterialProp == NULL || pProtScrProp == NULL )
	{
		return 0;
	}

	// ��ῡ ���� �б�
	switch( pMaterialProp->dwItemKind3 )
	{
		//	����Į���϶�
//		case IK3_ENCHANT:
		case IK3_SMELT_ULTIMATE_MATERIAL:
		case IK3_SMELT_GENERAL_MATERIAL:
#ifdef BARUNA_UPGRADE_SMELT_SAFETY
		case IK3_OPERCID:
#endif
			//	�Ϲ�����(�Ϲ� or ���͸ڿ������� ��˻�)
			return SmeltSafetyNormal( pUser, pItemMain, pItemMaterial, pItemProtScr, pItemSmeltScr );

		//	�������϶�
//		case IK3_PIERDICE:
		case IK3_SMELT_ACCESSORY_MATERIAL:
			//	�Ǽ������ΰ�
			if( pItemMain->IsAccessory() || pMainProp->IsAccessory() )
				return SmeltSafetyAccessory( pUser, pItemMain, pItemMaterial, pItemProtScr );
			//	�Ǿ���ΰ�
			else if( pItemMain->IsPierceAble( NULL_ID, TRUE ) )
				return SmeltSafetyPiercingSize( pUser, pItemMain, pItemMaterial, pItemProtScr );

		// �Ӽ�ī�� �϶�
		case IK3_ELECARD:
			return SmeltSafetyAttribute( pUser, pItemMain, pItemMaterial, pItemProtScr, pItemSmeltScr );
		
#ifdef COSTUME_UPGRADE_ENHANCEMENT_GEM
		case IK3_SMELT_COSTUME_MATERIAL:
			return SmeltSafetyCostume( pUser, pItemMain, pItemMaterial, pItemProtScr );
#endif
		default:
			break;
	}
	return 0;
}

BYTE	CItemUpgrade::SmeltSafetyNormal( FLWSUser* pUser, FLItemElem* pItemMain, FLItemElem* pItemMaterial, FLItemElem* pItemProtScr, FLItemElem* pItemSmeltScr )
{
	if( IsUsableItem( pItemMain ) == FALSE || IsUsableItem( pItemMaterial ) == FALSE || IsUsableItem( pItemProtScr ) == FALSE )
	{
		return 0;
	}

	PT_ITEM_SPEC pMainProp		= g_xSpecManager->GetSpecItem( pItemMain->m_dwItemId );
	PT_ITEM_SPEC pMaterialProp	= g_xSpecManager->GetSpecItem( pItemMaterial->m_dwItemId );
	PT_ITEM_SPEC pProtScrProp	= g_xSpecManager->GetSpecItem( pItemProtScr->m_dwItemId );
	if( pMainProp == NULL || pMaterialProp == NULL || pProtScrProp == NULL )
	{
		return 0;
	}

	//	����Į���ΰ�, ������ ����Į���ΰ�
	switch( pItemMaterial->GetProp()->dwID )
	{
		//	����Į���϶� �Ϲ�����
		case ITEM_INDEX( 2035, II_GEN_MAT_ORICHALCUM01 ):
		case ITEM_INDEX( 2082, II_GEN_MAT_ORICHALCUM01_1 ):
			return SmeltSafetyGeneral( pUser, pItemMain, pItemMaterial, pItemProtScr, pItemSmeltScr );

		//	������ ����Į���϶� ���͸ڿ��� ����
		case ITEM_INDEX( 2034, II_GEN_MAT_ORICHALCUM02 ):
			return prj.m_UltimateWeapon.SmeltSafetyUltimate( pUser, pItemMain, pItemMaterial, pItemProtScr );
#ifdef BARUNA_UPGRADE_SMELT_SAFETY
		case ITEM_INDEX( 7049, II_GEN_MAT_OPERCID ):
			return SmeltSafetyBarunaGeneral( pUser, pItemMain, pItemMaterial, pItemProtScr);		
#endif
		default:
			break;
	}
	return 0;
}

BYTE	CItemUpgrade::SmeltSafetyGeneral( FLWSUser* pUser, FLItemElem* pItemMain, FLItemElem* pItemMaterial, FLItemElem* pItemProtScr, FLItemElem* pItemSmeltScr )
{
	if( IsUsableItem( pItemMain ) == FALSE || IsUsableItem( pItemMaterial ) == FALSE || IsUsableItem( pItemProtScr ) == FALSE )
	{
		return 0;
	}

	PT_ITEM_SPEC pMainProp		= g_xSpecManager->GetSpecItem( pItemMain->m_dwItemId );
	PT_ITEM_SPEC pMaterialProp	= g_xSpecManager->GetSpecItem( pItemMaterial->m_dwItemId );
	PT_ITEM_SPEC pProtScrProp	= g_xSpecManager->GetSpecItem( pItemProtScr->m_dwItemId );
	if( pMainProp == NULL || pMaterialProp == NULL || pProtScrProp == NULL )
	{
		return 0;
	}

	if( pMainProp->IsUpgradeAble( IUTYPE_ENCHANT_GENERAL ) == FALSE )
	{
		return 0;
	}

	//	���ð����� �������� �ƴҰ�� ����
	if( !FLItemElem::IsDiceRefineryAble( pMainProp ) )
	{
		//pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return 0;
	}
	
	//	���͸ڿ��� �̰ų� �Ϲݺ�ȣ�� �η縶���� �ƴҰ�� ����
	if( pMainProp->dwReferStat1 == WEAPON_ULTIMATE || pProtScrProp->dwID != ITEM_INDEX( 10464, II_SYS_SYS_SCR_SMELPROT ) )
	{
		//pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return 0;
	}

#ifdef __ENCHANT_BARUNA16

	if( pMainProp->IsBaruna() == TRUE )
	{
		return 0;
	}

#endif // __ENCHANT_BARUNA16
	
	//	���ü�ġ�� maxġ�� �Ѿ����� ����
	if( pItemMain->GetAbilityOption() >= GetMaxGeneralEnchantSize() )
	{
		//pUser->AddDefinedText( TID_UPGRADE_MAXOVER );
		return 3;
	}
	
	// 1000������ ���� �ۼ�Ʈ
	int nPercent = GetGeneralEnchantProb( pItemMain->GetAbilityOption() );
	
	//	������ �η縶���� ����ߴٸ�
	if( pItemSmeltScr != NULL )
	{
		//	������ �η縶���� �´��� Ȯ��
		if( IsUsableItem( pItemSmeltScr ) && pItemSmeltScr->GetProp()->dwID == ITEM_INDEX( 10468, II_SYS_SYS_SCR_SMELTING ) )
		{
			//	������ �η縶�� ��밡�� ��ġ�ΰ�
			if( pItemMain->GetAbilityOption() < 7 )
			{
				nPercent += 1000;
				PT_ITEM_SPEC pItemProp = pItemSmeltScr->GetProp();
				if( pItemProp)
				{
					g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemSmeltScr, pItemProp );
					g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
				}
				pUser->RemoveItem( pItemSmeltScr->m_dwObjId, 1 );
			}
		}
		//	������ �η縶���� �ƴҶ�
		else
			return 0;
	}

	LogItemInfo aLogItem;
	//aLogItem.Action = "N";
	//aLogItem.SendName = pUser->GetName();
	//aLogItem.RecvName = "UPGRADEITEM_SMELTSAFETY";
	FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "N" );
	FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
	FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "UPGRADEITEM_SMELTSAFETY" );
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = pUser->GetGold();
	aLogItem.Gold2 = pUser->GetGold();

	g_DPSrvr.OnLogItem( aLogItem, pItemMaterial, pItemMaterial->m_nItemNum );
	// ���þ��� ���� - �����̴�, ���д�...
	pUser->RemoveItem( pItemMaterial->m_dwObjId, 1 );

	//	��ȣ�� �η縶�� �α׳����
	g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemProtScr, pProtScrProp );
	g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pProtScrProp );

	pUser->RemoveItem( pItemProtScr->m_dwObjId, 1 );

	// �ش� �������� �Ӽ�, �Ϲ� ������ ��� Ȯ���� ������.
	if( (int)( xRandom( 10000 ) ) > nPercent )
	{
		// ���� �޼��� ���
		//pUser->AddDefinedText( TID_UPGRADE_FAIL );
		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );
		
		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
			g_xWSUserManager->AddCreateSfxObj( (CMover *)pUser, XI_INDEX( 1715, XI_INT_FAIL ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );
		
		//aLogItem.Action = "F";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "F" );
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );

		return 2;
	}
	else
	{
		// ����
		//pUser->AddDefinedText( TID_UPGRADE_SUCCEEFUL );
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );
		
		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
			g_xWSUserManager->AddCreateSfxObj( (CMover *)pUser, XI_INDEX( 1714, XI_INT_SUCCESS ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );
		
		pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_AO,  pItemMain->GetAbilityOption() + 1 );
		//aLogItem.Action = "H";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "H" );
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );

		return 1;
	}
////	return 0;
}

#ifdef BARUNA_UPGRADE_SMELT_SAFETY
BYTE	CItemUpgrade::SmeltSafetyBarunaGeneral( FLWSUser* pUser, FLItemElem* pItemMain, FLItemElem* pItemMaterial, FLItemElem* pItemProtScr )
{
	DWORD dwResult = 0;
	if( IsUsableItem( pItemMain ) == FALSE || IsUsableItem( pItemMaterial ) == FALSE || IsUsableItem( pItemProtScr ) == FALSE )
	{
		return 0;
	}

	if( pUser->GetBarunaNPCSummoner().GetNPCOBJID() == NULL_ID )			//�ٷ糪 ������ ȣ�� �Ǿ� ���� �ʴٸ�.
	{
		return 0;
	}

	PT_ITEM_SPEC pMainProp		= g_xSpecManager->GetSpecItem( pItemMain->m_dwItemId );
	PT_ITEM_SPEC pMaterialProp	= g_xSpecManager->GetSpecItem( pItemMaterial->m_dwItemId );
	PT_ITEM_SPEC pProtScrProp	= g_xSpecManager->GetSpecItem( pItemProtScr->m_dwItemId );
	if( pMainProp == NULL || pMaterialProp == NULL || pProtScrProp == NULL )
	{
		return 0;
	}

	if( pMainProp->IsUpgradeAble( IUTYPE_ENCHANT_GENERAL ) == FALSE )
	{
		return 0;
	}

	//	���ð����� �������� �ƴҰ�� ����
	if( !FLItemElem::IsDiceRefineryAble( pMainProp ) )
	{
		//pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return 0;
	}
	
	//	���͸ڿ��� �̰ų� �Ϲݺ�ȣ�� �η縶���� �ƴҰ�� ����
	if( pMainProp->dwReferStat1 == WEAPON_ULTIMATE || (pProtScrProp->dwItemKind3 != IK3_BARUNA_PROTECTION && pProtScrProp->dwItemKind3 != IK3_BARUNA_PROTECTION_KEEP))
	{
		//pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return 0;
	}

	
	//	���ü�ġ�� maxġ�� �Ѿ����� ����
	if( pItemMain->GetAbilityOption() >= GetMaxBarunaGeneralEnchantSize(pMainProp->dwItemKind1) )
	{
		//pUser->AddDefinedText( TID_UPGRADE_MAXOVER );
		return 3;
	}

	
	bool bMinusProtect		= false;
	int nProtectionResult = g_xUseActiveItem->OnDoUseActiveItem( pUser, pItemProtScr, pItemMain );

	if( nProtectionResult == FLUseActiveItem::ERR_SUCCESS_BARUNA_GENERAL_ENCHANT_PROTECT_KEEP )
	{
		bMinusProtect = true;
	}

	g_DPSrvr.PutItemLog( pUser, "N", "BARUNA_SMELT_ENCHANT_PROT", pItemProtScr );
	pUser->RemoveItem( pItemProtScr->m_dwObjId, 1 );

	g_DPSrvr.PutItemLog( pUser, "N", "BARUNA_SMELT_ENCHANT_MATERIAL", pItemMaterial );
	pUser->RemoveItem( pItemMaterial->m_dwObjId, 1 );

	const int nAbilityOption					= pItemMain->GetAbilityOption();

	__BARUNA_ENCHANT barunaEnchant = GetBarunaEnchant ( nAbilityOption + 1, pMainProp->dwItemKind1 );

	DWORD dwAddSuccessProb				= g_xBarunaUpgradeNpc->GetAdditionalSuccessProb( pUser );

//	dwAddSuccessProb += g_xApplyItemEffect->GetAdjValueGeneralEnchantRateBuff( pUser, pItemMain, IK3_BARUNA_ENCHANT_RATE, true );

	const DWORD dwRandomProb					= ::xRandom( 10000 );

	// �ش� �������� �Ӽ�, �Ϲ� ������ ��� Ȯ���� ������.
	if( dwRandomProb < barunaEnchant.dwSuccessProb + dwAddSuccessProb )		// ����
	{
		// ����
		//pUser->AddDefinedText( TID_UPGRADE_SUCCEEFUL );
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );
		
		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
			g_xWSUserManager->AddCreateSfxObj( (CMover *)pUser, XI_INDEX( 1714, XI_INT_SUCCESS ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );
		
		pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_AO,  pItemMain->GetAbilityOption() + 1 );
#ifdef BARUNA_UPGRADE_ENHANCEMENT_GEM
		if( pMainProp->dwItemKind1 == IK1_WEAPON && (nAbilityOption + 1) >= 12 )
		{
			int nPiercingSize = ((nAbilityOption + 1) - 10) * 0.5f;
			pUser->UpdateItem( pItemMain->m_dwObjId, UI_ULTIMATE_PIERCING_SIZE, nPiercingSize );
		}
#endif
		g_DPSrvr.PutItemLog( pUser, "S", "BARUNA_SMELT_ENCHANT_SUCCESS", pItemMain );

		dwResult = 1;
	}
	else if( dwRandomProb < barunaEnchant.dwSuccessProb + barunaEnchant.dwFailProb )	// ����
	{
		// ���� �޼��� ���
		//pUser->AddDefinedText( TID_UPGRADE_FAIL );
		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );
		
		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
			g_xWSUserManager->AddCreateSfxObj( (CMover *)pUser, XI_INDEX( 1715, XI_INT_FAIL ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );
		
		g_DPSrvr.PutItemLog( pUser, "F", "BARUNA_SMELT_ENCHANT_FAIL", pItemMain );

		dwResult = 2;
	}
	else if( dwRandomProb < barunaEnchant.dwSuccessProb + barunaEnchant.dwFailProb + barunaEnchant.dwMinusProb)	// �϶�
	{
		// ���� �޼��� ���
		//pUser->AddDefinedText( TID_UPGRADE_FAIL );
		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );
		
		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
			g_xWSUserManager->AddCreateSfxObj( (CMover *)pUser, XI_INDEX( 1715, XI_INT_FAIL ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );
		
		if( bMinusProtect == true )
		{
			g_DPSrvr.PutItemLog( pUser, "F", "BARUNA_SMELT_ENCHANT_MINUS", pItemMain );
			dwResult = 2;
		}
		else
		{
			pUser->UpdateItem( pItemMain->m_dwObjId, UI_AO, nAbilityOption - 1 );
			g_DPSrvr.PutItemLog( pUser, "M", "BARUNA_SMELT_ENCHANT_MINUS", pItemMain );
			dwResult = 4;
		}
	}
	else
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "[ CRITICAL ERROR : dwItemKind1(%u) probSuccess(%u) AddSuccessProp(%u) probFail(%u) probMinus(%u) ]" )
			, pMainProp->dwItemKind1, barunaEnchant.dwSuccessProb, dwAddSuccessProb, barunaEnchant.dwFailProb, barunaEnchant.dwMinusProb );

		return 0;
	}

	DWORD dwUpgradeResult = 0;
	if ( dwResult == 1 )
	{
		dwUpgradeResult = FSC_ITEMUPGRADE_UPGRADE_SUCCESS;
	}
	else if ( dwResult == 2 )
	{
		dwUpgradeResult = FSC_ITEMUPGRADE_UPGRADE_FAIL;
	}
	else if ( dwResult == 4 )
	{
		dwUpgradeResult = FSC_ITEMUPGRADE_UPGRADE_MINUS;
	}

	g_xBarunaUpgradeNpc->AddExpToBarunaNPC( pUser, IUTYPE_ENCHANT_GENERAL, nAbilityOption + 1, dwUpgradeResult );
	
	return (BYTE)dwResult;
}
#endif

BYTE	CItemUpgrade::SmeltSafetyAccessory(FLWSUser* pUser, FLItemElem* pItemMain, FLItemElem* pItemMaterial, FLItemElem* pItemProtScr )
{
	if( IsUsableItem( pItemMain ) == FALSE || IsUsableItem( pItemMaterial ) == FALSE || IsUsableItem( pItemProtScr ) == FALSE )
	{
		return 0;
	}

	PT_ITEM_SPEC pMainProp		= g_xSpecManager->GetSpecItem( pItemMain->m_dwItemId );
	PT_ITEM_SPEC pMaterialProp	= g_xSpecManager->GetSpecItem( pItemMaterial->m_dwItemId );
	PT_ITEM_SPEC pProtScrProp	= g_xSpecManager->GetSpecItem( pItemProtScr->m_dwItemId );
	if( pMainProp == NULL || pMaterialProp == NULL || pProtScrProp == NULL )
	{
		return 0;
	}

	if( pMainProp->IsUpgradeAble( IUTYPE_ENCHANT_GENERAL ) == FALSE )
	{
		return 0;
	}

	// ��ᰡ �������ΰ�?
	if( pMaterialProp->dwID != ITEM_INDEX( 2036, II_GEN_MAT_MOONSTONE ) && pMaterialProp->dwID != ITEM_INDEX( 2083, II_GEN_MAT_MOONSTONE_1 ) )
	{
		//pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return 0;
	}
	//	�Ǽ����� ��ȣ�� �η縶���� �ƴҰ�� ����
	if( pProtScrProp->dwID != ITEM_INDEX( 26473, II_SYS_SYS_SCR_SMELPROT4 ) && pProtScrProp->dwID != ITEM_INDEX( 25233, II_SYS_SYS_SCR_SMELPROT6 ))
	{
		//pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return 0;
	}
	//	���ü�ġ�� maxġ�� �Ѿ����� ����
	if( pItemMain->GetAbilityOption() >= _MAX_ENCHANT_LEVEL_ACCESSORY )	// 20
	{
		//pUser->AddDefinedText( TID_GAME_ACCESSORY_MAX_AAO );
		return 3;
	}

#ifdef __ENCHANT_BARUNA16

	if( pMainProp->IsBaruna() == TRUE )
	{
		return 0;
	}

#endif // __ENCHANT_BARUNA16

	// log
	LogItemInfo aLogItem;
	//aLogItem.SendName	= pUser->GetName();
	//aLogItem.RecvName	= "UPGRADEITEM_SMELTSAFETY";
	FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
	FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "UPGRADEITEM_SMELTSAFETY" );
	aLogItem.WorldId	= pUser->GetWorld()->GetID();
	aLogItem.Gold	= pUser->GetGold();
	aLogItem.Gold2	= pUser->GetGold();

	//aLogItem.Action	= "N";
	FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "N" );
	g_DPSrvr.OnLogItem( aLogItem, pItemMaterial, pItemMaterial->m_nItemNum );
	pUser->RemoveItem( pItemMaterial->m_dwObjId, 1 );
	// �׼����� ��ȣ�� �η縶�� �α� �����
	g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemProtScr, pProtScrProp );
	g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pProtScrProp );

	pUser->RemoveItem( pItemProtScr->m_dwObjId, 1 );

	// ���� Ȯ��
	DWORD dwProbability = CAccessoryProperty::GetInstance()->GetProbability( pItemMain->GetAbilityOption() );

	if( xRandom( 10000 ) > dwProbability )	// ����
	{
		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );
		if( pUser->IsMode( TRANSPARENT_MODE ) == 0 )
			g_xWSUserManager->AddCreateSfxObj( (CMover *)pUser, XI_INDEX( 1715, XI_INT_FAIL ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );

		//aLogItem.Action	= "L";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "L" );
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );

		return 2;
	}
	else	// ����
	{
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );
			
		if( pUser->IsMode( TRANSPARENT_MODE ) == 0)
			g_xWSUserManager->AddCreateSfxObj( (CMover *)pUser, XI_INDEX( 1714, XI_INT_SUCCESS ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);

		pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_AO,  pItemMain->GetAbilityOption()+1 );

		//aLogItem.Action		= "H";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "H" );
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );

		return 1;
	}
//	return 0;
}

BYTE	CItemUpgrade::SmeltSafetyPiercingSize(FLWSUser* pUser, FLItemElem* pItemMain, FLItemElem* pItemMaterial, FLItemElem* pItemProtScr )
{
	if( IsUsableItem( pItemMain ) == FALSE || IsUsableItem( pItemMaterial ) == FALSE || IsUsableItem( pItemProtScr ) == FALSE )
	{
		return 0;
	}

	PT_ITEM_SPEC pMainProp		= g_xSpecManager->GetSpecItem( pItemMain->m_dwItemId );
	PT_ITEM_SPEC pMaterialProp	= g_xSpecManager->GetSpecItem( pItemMaterial->m_dwItemId );
	PT_ITEM_SPEC pProtScrProp	= g_xSpecManager->GetSpecItem( pItemProtScr->m_dwItemId );
	if( pMainProp == NULL || pMaterialProp == NULL || pProtScrProp == NULL )
	{
		return 0;
	}

	if( pMainProp->IsUpgradeAble( IUTYPE_PIERCING ) == FALSE )
	{
		return 0;
	}
#ifdef __ENCHANT_BARUNA16

	if( pMainProp->IsBaruna() == TRUE 
#ifdef BARUNA_UPGRADE_SUIT_PIERCING
&& pMainProp->dwItemKind3 != IK3_SUIT
#endif
)
	{
		return 0;
	}

#endif // __ENCHANT_BARUNA16

#ifdef BARUNA_UPGRADE_SUIT_PIERCING
	if( pMainProp->IsBaruna() == TRUE && pMainProp->dwItemKind3 == IK3_SUIT)
	{
		// ��ᰡ �ڽ������ΰ�?
		if( pMaterialProp->dwID != ITEM_INDEX( 25580, II_GEN_MAT_COSMOSTONE ))
		{
			//pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
			return 0;
		}
		//	�Ǿ�� ��ȣ�� �η縶���ΰ�
		if( pItemProtScr->m_dwItemId != ITEM_INDEX( 25581, II_SYS_SYS_SCR_PIEPROT3 ))
		{
			//pUser->AddDefinedText( TID_SBEVE_NOTUSEITEM );			// ���������� �ƴϸ� �Ұ���
			return 0;
		}
	}
	else
#endif
	{
		// ��ᰡ �������ΰ�?
		if( pMaterialProp->dwID != ITEM_INDEX( 2036, II_GEN_MAT_MOONSTONE ) && pMaterialProp->dwID != ITEM_INDEX( 2083, II_GEN_MAT_MOONSTONE_1 ) )
		{
			//pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
			return 0;
		}
		//	�Ǿ�� ��ȣ�� �η縶���ΰ�
		if( pItemProtScr->m_dwItemId != ITEM_INDEX( 10465, II_SYS_SYS_SCR_PIEPROT ) && pItemProtScr->m_dwItemId != ITEM_INDEX( 25234, II_SYS_SYS_SCR_PIEPROT2 ))
		{
			//pUser->AddDefinedText( TID_SBEVE_NOTUSEITEM );			// ���������� �ƴϸ� �Ұ���
			return 0;
		}
	}


	LogItemInfo aLogItem;
	//aLogItem.SendName = pUser->GetName();
	//aLogItem.RecvName = "PIERCING_SMELTSAFETY";
	FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
	FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "PIERCING_SMELTSAFETY" );
	aLogItem.WorldId = pUser->GetWorld()->GetID();

	int nCost = 100000;

	if( 0 < nCost )
	{
//		if( pUser->GetGold() < nCost )
		if( pUser->CheckUserGold( nCost, false ) == false )
		{
			pUser->AddDefinedText( TID_GAME_LACKMONEY );
			return 0;
		}

		pUser->AddGold( -( nCost ) );

		aLogItem.Gold = pUser->GetGold() + nCost;
		aLogItem.Gold2 = pUser->GetGold();
		//aLogItem.Action = "!";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "!" );
		//aLogItem.ItemName = "SEED";
		FLSPrintf( aLogItem.kLogItem.szItemName, _countof( aLogItem.kLogItem.szItemName ), "%d", ITEM_INDEX( 12, II_GOLD_SEED1 ) );
		aLogItem.kLogItem.nQuantity = nCost;
		g_DPSrvr.OnLogItem( aLogItem );
	}
	else
	{
		return 0;
	}
	aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();

	// ���� �η縶�� ����.
	//aLogItem.Action = "!";
	FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "!" );
	g_DPSrvr.OnLogItem( aLogItem, pItemMaterial, pItemMaterial->m_nItemNum );
	pUser->RemoveItem( pItemMaterial->m_dwObjId, 1 );
	//	�Ǿ�� ��ȣ�� �η縶�� �α� �����
	g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemProtScr, pProtScrProp );
	g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pProtScrProp );

	pUser->RemoveItem( pItemProtScr->m_dwObjId, 1 );
	
	int nPercent = GetSizeProb( pItemMain );

	if( nPercent < (int)( xRandom( 10000 ) ) )
	{	// ����
		//pUser->AddDefinedText( TID_MMI_PIERCINGFAIL );
		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );
		if( pUser->IsMode( TRANSPARENT_MODE ) == 0)
			g_xWSUserManager->AddCreateSfxObj( (CMover *)pUser, XI_INDEX( 1715, XI_INT_FAIL ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);
		
		//aLogItem.Action = "!";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "!" );
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );

		return 2;
	}
	else
	{	// ����
		//pUser->AddDefinedText( TID_MMI_PIERCINGSUCCESS );
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );
		if( pUser->IsMode( TRANSPARENT_MODE ) == 0)
			g_xWSUserManager->AddCreateSfxObj( (CMover *)pUser, XI_INDEX( 1714, XI_INT_SUCCESS ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );
		pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_PIERCING_SIZE, pItemMain->GetGeneralPiercingSize() + 1 );

		//aLogItem.Action = "#";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "#" );
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );

		return 1;
	}
//	return 0;
}

#ifdef COSTUME_UPGRADE_ENHANCEMENT_GEM
BYTE	CItemUpgrade::SmeltSafetyCostume(FLWSUser* pUser, FLItemElem* pItemMain, FLItemElem* pItemMaterial, FLItemElem* pItemProtScr )
{
	if( IsUsableItem( pItemMain ) == FALSE || IsUsableItem( pItemMaterial ) == FALSE || IsUsableItem( pItemProtScr ) == FALSE )
	{
		return 0;
	}

	PT_ITEM_SPEC pMainProp		= g_xSpecManager->GetSpecItem( pItemMain->m_dwItemId );
	PT_ITEM_SPEC pMaterialProp	= g_xSpecManager->GetSpecItem( pItemMaterial->m_dwItemId );
	PT_ITEM_SPEC pProtScrProp	= g_xSpecManager->GetSpecItem( pItemProtScr->m_dwItemId );
	if( pMainProp == NULL || pMaterialProp == NULL || pProtScrProp == NULL )
	{
		return 0;
	}

	if( pMainProp->IsUpgradeAble( IUTYPE_ENCHANT_GENERAL ) == FALSE )
	{
		return 0;
	}

	//	�ڽ�Ƭ�� �ƴѰ��
	if( !pMainProp->IsCostumeEnhanceParts())
	{
		//pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return 0;
	}

	if( pProtScrProp->dwID != ITEM_INDEX( 25336, II_SYS_SYS_SCR_COSTUMEPROT ) )
	{
		return 0;
	}

#ifdef __ENCHANT_BARUNA16

	if( pMainProp->IsBaruna() == TRUE )
	{
		return 0;
	}

#endif // __ENCHANT_BARUNA16
	
	//	���ü�ġ�� maxġ�� �Ѿ����� ����
	if( pItemMain->GetAbilityOption() >= GetMaxCostumeEnchantSize() )
	{
		//pUser->AddDefinedText( TID_UPGRADE_MAXOVER );
		return 3;
	}

	if( pItemMain->m_dwKeepTime != 0 )
	{
		return 0;
	}

	
	// 1000������ ���� �ۼ�Ʈ
	int nPercent = GetCostumeEnchantProb( pItemMain->GetAbilityOption() );

	LogItemInfo aLogItem;
	//aLogItem.Action = "N";
	//aLogItem.SendName = pUser->GetName();
	//aLogItem.RecvName = "UPGRADEITEM_SMELTSAFETY";
	FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "N" );
	FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
	FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "UPGRADEITEM_SMELTSAFETY" );
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = pUser->GetGold();
	aLogItem.Gold2 = pUser->GetGold();

	g_DPSrvr.OnLogItem( aLogItem, pItemMaterial, pItemMaterial->m_nItemNum );
	// ���þ��� ���� - �����̴�, ���д�...
	pUser->RemoveItem( pItemMaterial->m_dwObjId, 1 );

	//	��ȣ�� �η縶�� �α׳����
	g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemProtScr, pProtScrProp );
	g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pProtScrProp );

	pUser->RemoveItem( pItemProtScr->m_dwObjId, 1 );

	// �ش� �������� �Ӽ�, �Ϲ� ������ ��� Ȯ���� ������.
	if( (int)( xRandom( 10000 ) ) > nPercent )
	{
		// ���� �޼��� ���
		//pUser->AddDefinedText( TID_UPGRADE_FAIL );
		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );
		
		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
			g_xWSUserManager->AddCreateSfxObj( (CMover *)pUser, XI_INDEX( 1715, XI_INT_FAIL ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );
		
		//aLogItem.Action = "F";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "F" );
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );

		return 2;
	}
	else
	{
		// ����
		//pUser->AddDefinedText( TID_UPGRADE_SUCCEEFUL );
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );
		
		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
			g_xWSUserManager->AddCreateSfxObj( (CMover *)pUser, XI_INDEX( 1714, XI_INT_SUCCESS ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );
		
		pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_AO,  pItemMain->GetAbilityOption() + 1 );

#ifdef COSTUME_UPGRADE_ENHANCEMENT_GEM
	if( pItemMain->GetAbilityOption() > 6 )
		pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_ULTIMATE_PIERCING_SIZE, pItemMain->GetAbilityOption() - 6 );
#endif

		//aLogItem.Action = "H";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "H" );
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );

		return 1;
	}
////	return 0;
}
#endif

void	CItemUpgrade::RefineAccessory( FLWSUser* pUser, FLItemElem* pItemMain, FLItemElem* pItemMaterial )
{
	if( IsUsableItem( pItemMain ) == FALSE || IsUsableItem( pItemMaterial ) == FALSE )
	{
		return;
	}

	PT_ITEM_SPEC pMainProp = g_xSpecManager->GetSpecItem( pItemMain->m_dwItemId );
	PT_ITEM_SPEC pMaterialProp = g_xSpecManager->GetSpecItem( pItemMaterial->m_dwItemId );
	if( pMainProp == NULL || pMaterialProp == NULL )
	{
		return;
	}

	if( pMainProp->IsUpgradeAble( IUTYPE_ENCHANT_GENERAL ) == FALSE )
	{
		pUser->AddDefinedText( TID_GAME_SMELT_SAFETY_ERROR01 );
		return;
	}

	// ��ᰡ �������ΰ�?
	if( pMaterialProp->dwID != ITEM_INDEX( 2036, II_GEN_MAT_MOONSTONE ) && pMaterialProp->dwID != ITEM_INDEX( 2083, II_GEN_MAT_MOONSTONE_1 ) )
	{
		pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return;
	}
	if( pItemMain->GetAbilityOption() >= _MAX_ENCHANT_LEVEL_ACCESSORY )	// 20
	{
		pUser->AddDefinedText( TID_GAME_ACCESSORY_MAX_AAO );
		return;
	}

	// log
	LogItemInfo aLogItem;
	//aLogItem.SendName	= pUser->GetName();
	//aLogItem.RecvName	= "UPGRADEITEM";
	FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
	FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "UPGRADEITEM" );
	aLogItem.WorldId	= pUser->GetWorld()->GetID();
	aLogItem.Gold	= pUser->GetGold();
	aLogItem.Gold2	= pUser->GetGold();

	DWORD dwProbability		= CAccessoryProperty::GetInstance()->GetProbability( pItemMain->GetAbilityOption() );
	// �׼����� ��ȣ�� �η縶��
	BOOL bSmelprot	= FALSE;
	if( pUser->HasBuff( BUFF_ITEM, ITEM_INDEX( 26473, II_SYS_SYS_SCR_SMELPROT4 ) ) )
	{
		bSmelprot	= TRUE;
		pUser->RemoveBuff( BUFF_ITEM, ITEM_INDEX( 26473, II_SYS_SYS_SCR_SMELPROT4 ) );
	}

	if( pUser->HasBuff( BUFF_ITEM, ITEM_INDEX( 25233, II_SYS_SYS_SCR_SMELPROT6 ) ) )
	{
		bSmelprot	= TRUE;
		pUser->RemoveBuff( BUFF_ITEM, ITEM_INDEX( 25233, II_SYS_SYS_SCR_SMELPROT6 ) );
	}

	if( xRandom( 10000 ) < dwProbability )	// ����
	{
		pUser->AddDefinedText( TID_UPGRADE_SUCCEEFUL );
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );
			
		if( pUser->IsMode( TRANSPARENT_MODE ) == 0)
			g_xWSUserManager->AddCreateSfxObj( (CMover *)pUser, XI_INDEX( 1714, XI_INT_SUCCESS ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);

		pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_AO,  pItemMain->GetAbilityOption()+1 );

		//aLogItem.Action		= "H";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "H" );
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );
	}
	else	// ����
	{
		pUser->AddDefinedText( TID_UPGRADE_FAIL );
		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );
		if( pUser->IsMode( TRANSPARENT_MODE ) == 0 )
		{
			g_xWSUserManager->AddCreateSfxObj( (CMover *)pUser, XI_INDEX( 1715, XI_INT_FAIL ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );
		}

		bool bDestroy	= false;

		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "F" );

		if( bSmelprot == FALSE && pItemMain->GetAbilityOption() > MAX_SAFE_LEVEL_GENERAL_ENCHANT )		// ����
		{
			bDestroy	= true;
			FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "L" );
		}

		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );
		if( bDestroy == true )
		{
			pUser->RemoveItem( pItemMain->m_dwObjId, pItemMain->m_nItemNum );
		}
	}

	//aLogItem.Action	= "N";
	FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "N" );
	g_DPSrvr.OnLogItem( aLogItem, pItemMaterial, pItemMaterial->m_nItemNum );
	pUser->RemoveItem( pItemMaterial->m_dwObjId, 1 );
	//pUser->UpdateItem( (BYTE)pItemMaterial->m_dwObjId, UI_NUM, pItemMaterial->m_nItemNum - 1 );
}

void	CItemUpgrade::RefineCollector( FLWSUser* pUser, FLItemElem* pItemMain, FLItemElem* pItemMaterial )
{
	if( IsUsableItem( pItemMain ) == FALSE || IsUsableItem( pItemMaterial ) == FALSE )
	{
		return;
	}

	PT_ITEM_SPEC pMainProp = g_xSpecManager->GetSpecItem( pItemMain->m_dwItemId );
	PT_ITEM_SPEC pMaterialProp = g_xSpecManager->GetSpecItem( pItemMaterial->m_dwItemId );
	if( pMainProp == NULL || pMaterialProp == NULL )
	{
		return;
	}

	if( pMainProp->IsUpgradeAble( IUTYPE_ENCHANT_GENERAL ) == FALSE )
	{
		pUser->AddDefinedText( TID_GAME_SMELT_SAFETY_ERROR01 );
		return;
	}

	// ��ᰡ �������ΰ�?
	if( pMaterialProp->dwID != ITEM_INDEX( 2036, II_GEN_MAT_MOONSTONE ) && pMaterialProp->dwID != ITEM_INDEX( 2083, II_GEN_MAT_MOONSTONE_1 ) )
	{
		pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return;
	}

	CCollectingProperty* pProperty	= CCollectingProperty::GetInstance();
	if( pItemMain->GetAbilityOption() >= pProperty->GetMaxCollectorLevel() )
	{
		pUser->AddDefinedText( TID_GAME_MAX_COLLECTOR_LEVEL );
		return;
	}
	
	int nProb	= pProperty->GetEnchantProbability( pItemMain->GetAbilityOption() );
	if( nProb == 0 )
		return;

	// log
	LogItemInfo aLogItem;
	//aLogItem.SendName	= pUser->GetName();
	//aLogItem.RecvName	= "UPGRADEITEM";
	FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
	FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "UPGRADEITEM" );
	aLogItem.WorldId	= pUser->GetWorld()->GetID();
	aLogItem.Gold	= pUser->GetGold();
	aLogItem.Gold2	= pUser->GetGold();

	DWORD dwRand	= xRandom( 1000 );	// 0 - 999
	if( (int)( dwRand ) < nProb )
	{
		pUser->AddDefinedText( TID_UPGRADE_SUCCEEFUL );
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );
		if( pUser->IsMode( TRANSPARENT_MODE ) == 0 )
			g_xWSUserManager->AddCreateSfxObj( (CMover *)pUser, XI_INDEX( 1714, XI_INT_SUCCESS ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );
		pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_AO,  pItemMain->GetAbilityOption()+1 );
		//aLogItem.Action		= "H";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "H" );
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );
	}
	else
	{
		pUser->AddDefinedText( TID_UPGRADE_FAIL );
		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );
		if( pUser->IsMode( TRANSPARENT_MODE ) == 0 )
			g_xWSUserManager->AddCreateSfxObj( (CMover *)pUser, XI_INDEX( 1715, XI_INT_FAIL ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );
		// log
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "F" );
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );
	}
	//aLogItem.Action	= "N";
	FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "N" );
	g_DPSrvr.OnLogItem( aLogItem, pItemMaterial, pItemMaterial->m_nItemNum );
	pUser->UpdateItem( (BYTE)pItemMaterial->m_dwObjId, UI_NUM, pItemMaterial->m_nItemNum - 1 );
}

BYTE	CItemUpgrade::SmeltSafetyAttribute(FLWSUser* pUser, FLItemElem* pItemMain, FLItemElem* pItemMaterial, FLItemElem* pItemProtScr, FLItemElem* pItemSmeltScr )
{
	if( IsUsableItem( pItemMain ) == FALSE || IsUsableItem( pItemMaterial ) == FALSE || IsUsableItem( pItemProtScr ) == FALSE )
	{
		return 0;
	}

	PT_ITEM_SPEC pMainProp		= g_xSpecManager->GetSpecItem( pItemMain->m_dwItemId );
	PT_ITEM_SPEC pMaterialProp	= g_xSpecManager->GetSpecItem( pItemMaterial->m_dwItemId );
	PT_ITEM_SPEC pProtScrProp	= g_xSpecManager->GetSpecItem( pItemProtScr->m_dwItemId );
	if( pMainProp == NULL || pMaterialProp == NULL || pProtScrProp == NULL )
	{
		return 0;
	}

	if( pMainProp->IsUpgradeAble( IUTYPE_ENCHANT_ATTRIBUTE ) == FALSE )
	{
		return 0;
	}

	if( pMainProp->IsBaruna() == TRUE )
	{
		return 0;
	}

	// �Ӽ� ������ �����Ѱ�
	if( !FLItemElem::IsEleRefineryAble( pMainProp ) )
		return 0;

	// �Ӽ��� �Ѱ�����
	if( pItemMain->m_byItemResist != SAI79::NO_PROP )
	{
		if( pItemMain->m_byItemResist != pMaterialProp->eItemType )
			return 0;
	}

	// �Ӽ� �� �ϳ��� �Ӽ� ���� ī�带 ����ϵ��� ����
	if( pMaterialProp->dwID != WhatEleCard( pMaterialProp->eItemType ) )
		return 0;

	// �ִ� ���� �Ѵ� ��� �ߴ�
	if( pItemMain->m_nResistAbilityOption >= GetMaxAttributeEnchantSize() )
		return 3;

	// 10000������ ���� �ۼ�Ʈ
	int nPercent = GetAttributeEnchantProb( pItemMain->m_nResistAbilityOption );

	//	�Ӽ� ������ �η縶���� ����ߴٸ�
	if( pItemSmeltScr != NULL )
	{
		//	�Ӽ� ������ �η縶���� �´��� Ȯ��
		if( IsUsableItem( pItemSmeltScr ) && pItemSmeltScr->GetProp()->dwID == ITEM_INDEX( 10489, II_SYS_SYS_SCR_SMELTING2 ) )
		{
			//	�Ӽ� ������ �η縶�� ��밡�� ��ġ�ΰ�
			if( pItemMain->m_nResistAbilityOption < 10 )
			{
				nPercent	+= 1000;
				//	�Ӽ� ������ �η縶�� �α� �����
				PT_ITEM_SPEC pItemProp = pItemSmeltScr->GetProp();
				if( pItemProp )
				{
					g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemSmeltScr, pItemProp );
					g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
				}
				pUser->RemoveItem( pItemSmeltScr->m_dwObjId, 1 );
			}
		}
		//	�Ӽ� ������ �η縶���� �ƴҶ�
		else
			return 0;
	}

	LogItemInfo aLogItem;
	//aLogItem.Action = "N";
	//aLogItem.SendName = pUser->GetName();
	//aLogItem.RecvName = "UPGRADEITEM_SMELTSAFETY";
	FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "N" );
	FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
	FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "UPGRADEITEM_SMELTSAFETY" );
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = pUser->GetGold();
	aLogItem.Gold2 = pUser->GetGold();

	g_DPSrvr.OnLogItem( aLogItem, pItemMaterial, pItemMaterial->m_nItemNum );
	DWORD dwValue = pItemMaterial->GetProp()->eItemType;
	pUser->RemoveItem( pItemMaterial->m_dwObjId, 1 );

	g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemProtScr, pProtScrProp );
	g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pProtScrProp );

	pUser->RemoveItem( pItemProtScr->m_dwObjId, 1 );

	// �ش� �������� �Ӽ�, �Ϲ� ������ ��� Ȯ���� ������.
	if( (int)( xRandom( 10000 ) ) > nPercent )
	{
		// ����
		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );

		if( !pUser->IsMode( TRANSPARENT_MODE ) )
			g_xWSUserManager->AddCreateSfxObj( (CMover *)pUser, XI_INDEX( 1715, XI_INT_FAIL ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );

		//aLogItem.Action = "J";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "J" );
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );

		return 2;
	}
	else
	{
		// ����
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );

		if( !pUser->IsMode( TRANSPARENT_MODE ) )
			g_xWSUserManager->AddCreateSfxObj( (CMover *)pUser, XI_INDEX( 1714, XI_INT_SUCCESS ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );

		pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_IR, dwValue );
		pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_RAO,  pItemMain->m_nResistAbilityOption + 1 );
		//aLogItem.Action = "O";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "O" );
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );

		return 1;
	}
//	return 0;
}

void	CItemUpgrade::EnchantGeneral( FLWSUser* pUser, FLItemElem* pItemMain, FLItemElem* pItemMaterial )
{
	if( IsUsableItem( pItemMain ) == FALSE || IsUsableItem( pItemMaterial ) == FALSE )
	{
		return;
	}

	PT_ITEM_SPEC pMainProp = g_xSpecManager->GetSpecItem( pItemMain->m_dwItemId );
	PT_ITEM_SPEC pMaterialProp = g_xSpecManager->GetSpecItem( pItemMaterial->m_dwItemId );
	if( pMainProp == NULL || pMaterialProp == NULL )
	{
		return;
	}

	if( pMainProp->IsUpgradeAble( IUTYPE_ENCHANT_GENERAL ) == FALSE )
	{
		pUser->AddDefinedText( TID_GAME_SMELT_SAFETY_ERROR01 );
		return;
	}

	int*  pAbilityOption = pItemMain->GetAbilityOptionPtr();
	if( pAbilityOption == NULL )
		return;

	// �Ϲ������� ��, ����
	if( pMainProp->dwReferStat1 == WEAPON_ULTIMATE )
	{
		pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return;
	}

	if( pMaterialProp->dwID != ITEM_INDEX( 2035, II_GEN_MAT_ORICHALCUM01 )
		&& pMaterialProp->dwID != ITEM_INDEX( 2082, II_GEN_MAT_ORICHALCUM01_1 ) 
#ifdef ENCHANT_ABSOLUTE
		&& pMaterialProp->dwID != ITEM_INDEX( 25435, II_SYS_SYS_SCR_ENCHANT_ABSOLUTE06 ) 
		&& pMaterialProp->dwID != ITEM_INDEX( 25436, II_SYS_SYS_SCR_ENCHANT_ABSOLUTE07 ) 
		&& pMaterialProp->dwID != ITEM_INDEX( 25437, II_SYS_SYS_SCR_ENCHANT_ABSOLUTE08 ) 
		&& pMaterialProp->dwID != ITEM_INDEX( 25438, II_SYS_SYS_SCR_ENCHANT_ABSOLUTE09 ) 
		&& pMaterialProp->dwID != ITEM_INDEX( 25439, II_SYS_SYS_SCR_ENCHANT_ABSOLUTE10 ) 
#endif		
		)
	{
		pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return;
	}
#ifdef ENCHANT_ABSOLUTE
	if( pMaterialProp->dwID == ITEM_INDEX( 25435, II_SYS_SYS_SCR_ENCHANT_ABSOLUTE06 ) && pItemMain->GetAbilityOption() >= 6 )
	{
		pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return;
	}
	else if( pMaterialProp->dwID == ITEM_INDEX( 25436, II_SYS_SYS_SCR_ENCHANT_ABSOLUTE07 ) && pItemMain->GetAbilityOption() >= 7 )
	{
		pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return;
	}
	else if( pMaterialProp->dwID == ITEM_INDEX( 25437, II_SYS_SYS_SCR_ENCHANT_ABSOLUTE08 ) && pItemMain->GetAbilityOption() >= 8 )
	{
		pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return;
	}
	else if( pMaterialProp->dwID == ITEM_INDEX( 25438, II_SYS_SYS_SCR_ENCHANT_ABSOLUTE09 ) && pItemMain->GetAbilityOption() >= 9 )
	{
		pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return;
	}
	else if( pMaterialProp->dwID == ITEM_INDEX( 25439, II_SYS_SYS_SCR_ENCHANT_ABSOLUTE10 ) && pItemMain->GetAbilityOption() >= 10 )
	{
		pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return;
	}
#endif

	if( !FLItemElem::IsDiceRefineryAble( pMainProp ) )
	{
		pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );			
		return;								
	}

	if( *pAbilityOption >= GetMaxGeneralEnchantSize() )
	{
		pUser->AddDefinedText( TID_UPGRADE_MAXOVER );			
		return;
	}

	// 1000������ ���� �ۼ�Ʈ 
	int nPercent = GetGeneralEnchantProb( *pAbilityOption );

	LogItemInfo aLogItem;
	//aLogItem.SendName = pUser->GetName();
	//aLogItem.RecvName = "UPGRADEITEM";
	FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
	FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "UPGRADEITEM" );
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = pUser->GetGold();
	aLogItem.Gold2 = pUser->GetGold();

	BOOL bSmelprot	= FALSE;
	if( pUser->HasBuff( BUFF_ITEM, ITEM_INDEX( 10464, II_SYS_SYS_SCR_SMELPROT ) ) )
	{
		bSmelprot	= TRUE;
		pUser->RemoveBuff( BUFF_ITEM, ITEM_INDEX( 10464, II_SYS_SYS_SCR_SMELPROT ) );

	}

	// �Ϲ� ����
	nPercent += g_xApplyItemEffect->GetAdjValueGeneralEnchantRateBuff( pUser, pItemMain, IK3_GENERAL_ENCHANT_RATE, true );
	
	// �Ϲ� + �Ӽ� ����
	nPercent += g_xApplyItemEffect->GetAdjValueGeneralEnchantRateBuff( pUser, pItemMain, IK3_GEN_ATT_ENCHANT_RATE, true );
	
	if( pMainProp->dwItemKind1 == IK1_WEAPON )
	{
		// �Ϲ� ���� ����
		nPercent += g_xApplyItemEffect->GetAdjValueGeneralEnchantRateBuff( pUser, pItemMain, IK3_GENERAL_WEAPON_ENCHANT_RATE, true );
	}
	
	int nEnchantAbsoluteOption = 0;
#ifdef ENCHANT_ABSOLUTE
	if( pMaterialProp->dwID >= ITEM_INDEX( 25435, II_SYS_SYS_SCR_ENCHANT_ABSOLUTE06 ) && pMaterialProp->dwID <= ITEM_INDEX( 25439, II_SYS_SYS_SCR_ENCHANT_ABSOLUTE10 ))
	{
		nPercent = 10000;
		// �������� ���̵� - ���밭ȭ��ũ�� ������ ���� ItemID + 6
		// ��) ���������� +7 ���� ��ȭ ��ũ�� ��� -> 25434 - 25433 + 6 = 7 
		nEnchantAbsoluteOption = pMaterialProp->dwID - ITEM_INDEX( 25435, II_SYS_SYS_SCR_ENCHANT_ABSOLUTE06 ) + 6;
	}
	

#endif	

// 	if( pUser->HasBuff( BUFF_ITEM, ITEM_INDEX( 10468, II_SYS_SYS_SCR_SMELTING ) ) )
// 	{
// 		if( *pAbilityOption < 7 )
// 		{
// 			nPercent	+= 1000;
// 			pUser->RemoveBuff( BUFF_ITEM, ITEM_INDEX( 10468, II_SYS_SYS_SCR_SMELTING ) );
// 
// 			PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( ITEM_INDEX( 10468, II_SYS_SYS_SCR_SMELTING ) );
// 			if( pItemProp )
// 				g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
// 		}
// 	}

// 	if( pUser->HasBuff( BUFF_ITEM, ITEM_INDEX( 26203, II_SYS_SYS_SCR_SUPERSMELTING ) ) )
// 	{
// 		if( *pAbilityOption < 7 )
// 		{
// 			nPercent	+= 10000;
// 			pUser->RemoveBuff( BUFF_ITEM, ITEM_INDEX( 26203, II_SYS_SYS_SCR_SUPERSMELTING ) );
// 
// 			PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( ITEM_INDEX( 26203, II_SYS_SYS_SCR_SUPERSMELTING ) );
// 			if( pItemProp )
// 				g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
// 		}				
// 	}

	// �ش� �������� �Ӽ�, �Ϲ� ������ ��� Ȯ���� ������.
	if( (int)( xRandom( 10000 ) ) > nPercent )
	{
		// ���� �޼��� ���
		pUser->AddDefinedText( TID_UPGRADE_FAIL );
		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );

		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
			g_xWSUserManager->AddCreateSfxObj((CMover *)pUser, XI_INDEX( 1715, XI_INT_FAIL ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);

		bool bDestroy	= false;
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "F" );

		// �����ϸ� 3�̻��̸� ������ ����
		if( bSmelprot == FALSE && pItemMain->GetAbilityOption() > MAX_SAFE_LEVEL_GENERAL_ENCHANT )	// ����
		{
			bDestroy	= true;
			FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "L" );
		}

		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );
		if( bDestroy == true )
		{
			pUser->RemoveItem( pItemMain->m_dwObjId, pItemMain->m_nItemNum );
		}
	}
	else
	{
		// ����
		pUser->AddDefinedText( TID_UPGRADE_SUCCEEFUL );
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );

		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
			g_xWSUserManager->AddCreateSfxObj((CMover *)pUser, XI_INDEX( 1714, XI_INT_SUCCESS ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);

		if( nEnchantAbsoluteOption == 0 )
			pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_AO,  pItemMain->GetAbilityOption()+1 );
		else
			pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_AO,  nEnchantAbsoluteOption );
	//	aLogItem.Action = "H";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "H" );
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );
	}

	//aLogItem.Action = "N";
	FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "N" );
	g_DPSrvr.OnLogItem( aLogItem, pItemMaterial, pItemMaterial->m_nItemNum );
	// ��� ���� ���� - �����̴�, ���д�...
	pUser->RemoveItem( pItemMaterial->m_dwObjId, 1 );
}

int		CItemUpgrade::GetGeneralEnchantProb( int nAbilityOption )
{
	int nProb = 0;
	
	map<int, int>::iterator it = m_mapGeneralEnchant.find( nAbilityOption + 1 );
	if( it != m_mapGeneralEnchant.end() )
		nProb = it->second;

	if( g_xFlyffConfig->GetMainLanguage() != LANG_KOR && nAbilityOption >= 3 )	// ���� 4���� 10% Ȯ�� ����(�ؿܸ�)
		nProb = static_cast<int>(static_cast<float>(nProb) * 0.9f);
	
	return nProb;
}

#ifdef COSTUME_UPGRADE_ENHANCEMENT_GEM
void	CItemUpgrade::EnchantCostume( FLWSUser* pUser, FLItemElem* pItemMain, FLItemElem* pItemMaterial )
{
	if( IsUsableItem( pItemMain ) == FALSE || IsUsableItem( pItemMaterial ) == FALSE )
	{
		return;
	}

	PT_ITEM_SPEC pMainProp = g_xSpecManager->GetSpecItem( pItemMain->m_dwItemId );
	PT_ITEM_SPEC pMaterialProp = g_xSpecManager->GetSpecItem( pItemMaterial->m_dwItemId );
	if( pMainProp == NULL || pMaterialProp == NULL )
	{
		return;
	}

	if( pMainProp->IsUpgradeAble( IUTYPE_ENCHANT_GENERAL ) == FALSE )
	{
		pUser->AddDefinedText( TID_GAME_SMELT_SAFETY_ERROR01 );
		return;
	}

	int*  pAbilityOption = pItemMain->GetAbilityOptionPtr();
	if( pAbilityOption == NULL )
		return;

	if( pMaterialProp->dwID != ITEM_INDEX( 25334, II_GEN_MAT_LIGHTNINGSTONE ) )
	{
		pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return;
	}

	if( *pAbilityOption >= GetMaxCostumeEnchantSize() )
	{
		pUser->AddDefinedText( TID_UPGRADE_MAXOVER );			
		return;
	}

	//	�ڽ�Ƭ�� �ƴѰ��
	if( !pMainProp->IsCostumeEnhanceParts())
	{
		//pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return;
	}

	if( pItemMain->m_dwKeepTime != 0 )
	{
		return;
	}

	// 1000������ ���� �ۼ�Ʈ 
	int nPercent = GetCostumeEnchantProb( *pAbilityOption );

	LogItemInfo aLogItem;
	//aLogItem.SendName = pUser->GetName();
	//aLogItem.RecvName = "UPGRADEITEM";
	FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
	FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "UPGRADEITEM" );
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = pUser->GetGold();
	aLogItem.Gold2 = pUser->GetGold();

	BOOL bSmelprot	= FALSE;
	if( pUser->HasBuff( BUFF_ITEM, ITEM_INDEX( 25336, II_SYS_SYS_SCR_COSTUMEPROT ) ) )
	{
		bSmelprot	= TRUE;
		pUser->RemoveBuff( BUFF_ITEM, ITEM_INDEX( 25336, II_SYS_SYS_SCR_COSTUMEPROT ) );

	}

	// �Ϲ� ����
	//nPercent += g_xApplyItemEffect->GetAdjValueGeneralEnchantRateBuff( pUser, pItemMain, IK3_GENERAL_ENCHANT_RATE, true );
	//
	//// �Ϲ� + �Ӽ� ����
	//nPercent += g_xApplyItemEffect->GetAdjValueGeneralEnchantRateBuff( pUser, pItemMain, IK3_GEN_ATT_ENCHANT_RATE, true );
	//
	//if( pMainProp->dwItemKind1 == IK1_WEAPON )
	//{
	//	// �Ϲ� ���� ����
	//	nPercent += g_xApplyItemEffect->GetAdjValueGeneralEnchantRateBuff( pUser, pItemMain, IK3_GENERAL_WEAPON_ENCHANT_RATE, true );
	//}

// 	if( pUser->HasBuff( BUFF_ITEM, ITEM_INDEX( 10468, II_SYS_SYS_SCR_SMELTING ) ) )
// 	{
// 		if( *pAbilityOption < 7 )
// 		{
// 			nPercent	+= 1000;
// 			pUser->RemoveBuff( BUFF_ITEM, ITEM_INDEX( 10468, II_SYS_SYS_SCR_SMELTING ) );
// 
// 			PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( ITEM_INDEX( 10468, II_SYS_SYS_SCR_SMELTING ) );
// 			if( pItemProp )
// 				g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
// 		}
// 	}

// 	if( pUser->HasBuff( BUFF_ITEM, ITEM_INDEX( 26203, II_SYS_SYS_SCR_SUPERSMELTING ) ) )
// 	{
// 		if( *pAbilityOption < 7 )
// 		{
// 			nPercent	+= 10000;
// 			pUser->RemoveBuff( BUFF_ITEM, ITEM_INDEX( 26203, II_SYS_SYS_SCR_SUPERSMELTING ) );
// 
// 			PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( ITEM_INDEX( 26203, II_SYS_SYS_SCR_SUPERSMELTING ) );
// 			if( pItemProp )
// 				g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
// 		}				
// 	}

	// �ش� �������� �Ӽ�, �Ϲ� ������ ��� Ȯ���� ������.
	if( (int)( xRandom( 10000 ) ) > nPercent )
	{
		// ���� �޼��� ���
		pUser->AddDefinedText( TID_UPGRADE_FAIL );
		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );

		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
			g_xWSUserManager->AddCreateSfxObj((CMover *)pUser, XI_INDEX( 1715, XI_INT_FAIL ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);

		bool bDestroy	= false;
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "F" );

		// �����ϸ� 3�̻��̸� ������ ����
		if( bSmelprot == FALSE && pItemMain->GetAbilityOption() > MAX_SAFE_LEVEL_GENERAL_ENCHANT )	// ����
		{
			bDestroy	= true;
			FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "L" );
		}

		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );
		if( bDestroy == true )
		{
			pUser->RemoveItem( pItemMain->m_dwObjId, pItemMain->m_nItemNum );
		}
	}
	else
	{
		// ����
		pUser->AddDefinedText( TID_UPGRADE_SUCCEEFUL );
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );

		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
			g_xWSUserManager->AddCreateSfxObj((CMover *)pUser, XI_INDEX( 1714, XI_INT_SUCCESS ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);

		pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_AO,  pItemMain->GetAbilityOption()+1 );

		
		// ���� �ռ�
		if( pItemMain->GetAbilityOption() > 6 )
			pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_ULTIMATE_PIERCING_SIZE, pItemMain->GetAbilityOption() - 6 );

	//	aLogItem.Action = "H";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "H" );
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );
	}

	//aLogItem.Action = "N";
	FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "N" );
	g_DPSrvr.OnLogItem( aLogItem, pItemMaterial, pItemMaterial->m_nItemNum );
	// ��� ���� ���� - �����̴�, ���д�...
	pUser->RemoveItem( pItemMaterial->m_dwObjId, 1 );
}


int		CItemUpgrade::GetCostumeEnchantProb( int nAbilityOption )
{
	int nProb = 0;
	
	map<int, int>::iterator it = m_mapCostumeEnchant.find( nAbilityOption + 1 );
	if( it != m_mapCostumeEnchant.end() )
		nProb = it->second;
	
	return nProb;
}
#endif

void	CItemUpgrade::EnchantAttribute( FLWSUser* pUser, FLItemElem* pItemMain, FLItemElem* pItemMaterial )
{
	if( IsUsableItem( pItemMain ) == FALSE || IsUsableItem( pItemMaterial ) == FALSE )
	{
		return;
	}

	PT_ITEM_SPEC pMainProp = g_xSpecManager->GetSpecItem( pItemMain->m_dwItemId );
	PT_ITEM_SPEC pMaterialProp = g_xSpecManager->GetSpecItem( pItemMaterial->m_dwItemId );
	if( pMainProp == NULL || pMaterialProp == NULL )
	{
		return;
	}

	if( pMainProp->IsBaruna() == TRUE )
	{
		pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return;
	}

	int*  pAbilityOption = &(pItemMain->m_nResistAbilityOption);
	if( pAbilityOption == NULL )
		return;

	// 2���� �Ӽ��� �����Ҽ� ����
	if( pItemMain->m_byItemResist != SAI79::NO_PROP )
	{
		if( pItemMain->m_byItemResist != pMaterialProp->eItemType )
		{
			pUser->AddDefinedText( TID_UPGRADE_ERROR_TWOELEMENT );								
			return;
		}
	}

	if( !FLItemElem::IsEleRefineryAble( pMainProp ) )
	{
		pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return;								
	}

	if( pMainProp->IsUpgradeAble( IUTYPE_ENCHANT_ATTRIBUTE ) == FALSE )
	{
		pUser->AddDefinedText( TID_UPGRADE_ERROR_NOELEUPGRADE );
		return;
	}

	// �Ӽ� �� �ϳ��� �Ӽ� ���� ī�带 ����ϵ��� ����
	DWORD dwReqCard	= WhatEleCard( pMaterialProp->eItemType );

	if( pMaterialProp->dwID != dwReqCard )
	{
		pUser->AddDefinedText( TID_UPGRADE_ERROR_WRONGUPLEVEL );			
		return;					
	}

	// �ִ� ���� �Ѵ� ��� �ߴ�
	if( *pAbilityOption >= GetMaxAttributeEnchantSize() )
	{
		pUser->AddDefinedText( TID_UPGRADE_MAXOVER );			
		return;
	}
	// 10000������ ���� �ۼ�Ʈ 
	int nPercent = GetAttributeEnchantProb( *pAbilityOption );

	LogItemInfo aLogItem;
	//aLogItem.SendName = pUser->GetName();
	//aLogItem.RecvName = "UPGRADEITEM";
	FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
	FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "UPGRADEITEM" );
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = pUser->GetGold();
	aLogItem.Gold2 = pUser->GetGold();


	BOOL bSmelprot	= FALSE;
	if( pUser->HasBuff( BUFF_ITEM, ITEM_INDEX( 10464, II_SYS_SYS_SCR_SMELPROT ) ) )
	{
		bSmelprot	= TRUE;
		pUser->RemoveBuff( BUFF_ITEM, ITEM_INDEX( 10464, II_SYS_SYS_SCR_SMELPROT ) );

	}

	nPercent += g_xApplyItemEffect->GetAdjValueAttributeEnchantRateBuff( pUser, pItemMain, IK3_GEN_ATT_ENCHANT_RATE, true );
	
	if( pUser->HasBuff( BUFF_ITEM, ITEM_INDEX( 10489, II_SYS_SYS_SCR_SMELTING2 ) ) )	// �Ӽ� ������ �η縶��
	{
		if( *pAbilityOption < 10 )
		{
			nPercent	+= 1000;
			pUser->RemoveBuff( BUFF_ITEM, ITEM_INDEX( 10489, II_SYS_SYS_SCR_SMELTING2 ) );
		}
	}

// 	if( pUser->HasBuff( BUFF_ITEM, ITEM_INDEX( 26203, II_SYS_SYS_SCR_SUPERSMELTING ) ) )
// 	{
// 		if( *pAbilityOption < 7 )
// 		{
// 			nPercent	+= 10000;
// 			pUser->RemoveBuff( BUFF_ITEM, ITEM_INDEX( 26203, II_SYS_SYS_SCR_SUPERSMELTING ) );
// 
// 			PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( ITEM_INDEX( 26203, II_SYS_SYS_SCR_SUPERSMELTING ) );
// 			if( pItemProp )
// 				g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
// 		}				
// 	}

	// �ش� �������� �Ӽ�, �Ϲ� ������ ��� Ȯ���� ������.
	if( (int)( xRandom( 10000 ) ) > nPercent )
	{
		// ���� �޼��� ���
		pUser->AddDefinedText( TID_UPGRADE_FAIL );
		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );

		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
			g_xWSUserManager->AddCreateSfxObj((CMover *)pUser, XI_INDEX( 1715, XI_INT_FAIL ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);

		bool bDestroy	= false;
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "J" );

		// �����ϸ� nLevDown�̻��̸� ������ ����
		if( bSmelprot == FALSE && pItemMain->GetResistAbilityOption() > MAX_SAFE_LEVEL_GENERAL_ENCHANT )	// ����
		{
			bDestroy	= true;
			FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "L" );
		}

		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );
		if( bDestroy == true )
		{
			pUser->RemoveItem( pItemMain->m_dwObjId, pItemMain->m_nItemNum );
		}
	}
	else
	{
		// ����
		pUser->AddDefinedText( TID_UPGRADE_SUCCEEFUL );
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );

		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0)
			g_xWSUserManager->AddCreateSfxObj((CMover *)pUser, XI_INDEX( 1714, XI_INT_SUCCESS ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);

		pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_IR,  pItemMaterial->GetProp()->eItemType );
		pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_RAO,  pItemMain->m_nResistAbilityOption+1 );
		//aLogItem.Action = "O";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "O" );
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );
	}

	//aLogItem.Action = "N";
	FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "N" );
	g_DPSrvr.OnLogItem( aLogItem, pItemMaterial, pItemMaterial->m_nItemNum );
	// ���þ��� ���� - �����̴�, ���д�...
	pUser->RemoveItem( pItemMaterial->m_dwObjId, 1 );
}


void CItemUpgrade::ChangeAttribute( FLWSUser* pUser, OBJID dwTargetItem, OBJID dwUseItem , SAI79::ePropType nAttribute )
{
	FLItemElem* pTargetItemElem	= pUser->m_Inventory.GetAtId( dwTargetItem );
	FLItemElem* pUseItemElem = pUser->m_Inventory.GetAtId( dwUseItem );		

	if( pUser->m_vtInfo.GetOther() )	// �ŷ����� ����� ������?
		return;
	if( pUser->m_vtInfo.VendorIsVendor() )		// ���� �Ȱ� ������?
		return;

	if( !IsUsableItem( pTargetItemElem ) || !IsUsableItem( pUseItemElem ) )
		return;

	// ����� �������ΰ�?
	if( pUser->m_Inventory.IsEquip( dwTargetItem ) )
	{
		pUser->AddDefinedText( TID_GAME_EQUIPPUT , "" );
		return;
	}
	
	if( pTargetItemElem->GetProp() == NULL )
	{
		return;
	}

	if( pTargetItemElem->GetProp()->dwItemKind3 != IK3_SUIT &&
		pTargetItemElem->GetProp()->dwItemKind2 != IK2_WEAPON_MAGIC &&
		pTargetItemElem->GetProp()->dwItemKind2 != IK2_WEAPON_DIRECT )
	{
		return;
	}

	if( pUseItemElem->m_dwItemId != ITEM_INDEX( 20037, II_SYS_SYS_SCR_SOKCHANG ) )	// �Ӽ����� �������� �ƴϳ�...
		return;

	if( nAttribute >= SAI79::END_PROP || nAttribute <= SAI79::NO_PROP )
		return;

	if( pTargetItemElem->m_byItemResist == nAttribute )	// ���� �Ӽ��� ��� �Ӽ����� �Ұ�!!
	{
		pUser->AddDefinedText( TID_GAME_NOCHANGE_SAME_ATTRIBUTE );
		return;
	}
	
	if( (pTargetItemElem->m_byItemResist != SAI79::NO_PROP) && (pTargetItemElem->m_nResistAbilityOption > 0) )
	{
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );
		if( ( pUser->IsMode( TRANSPARENT_MODE ) ) == 0 )
			g_xWSUserManager->AddCreateSfxObj((CMover *)pUser, XI_INDEX( 1714, XI_INT_SUCCESS ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);
		pUser->UpdateItem( (BYTE)pTargetItemElem->m_dwObjId, UI_IR,  nAttribute );	// �Ӽ� ����

		// �Ӽ����� ���� ���� �α�
		LogItemInfo aLogItem;
		//aLogItem.SendName = pUser->GetName();
		//aLogItem.RecvName = "CHANGE_ATTRIBUTE_TARGET";
		FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
		FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "CHANGE_ATTRIBUTE_TARGET" );
		aLogItem.WorldId = pUser->GetWorld()->GetID();
		aLogItem.Gold = pUser->GetGold();
		aLogItem.Gold2 = pUser->GetGold();
		//aLogItem.Action = "O";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "O" );
		g_DPSrvr.OnLogItem( aLogItem, pTargetItemElem );
		
		//aLogItem.RecvName = "CHANGE_ATTRIBUTE_MATERIAL";
		FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "CHANGE_ATTRIBUTE_MATERIAL" );
		g_DPSrvr.OnLogItem( aLogItem, pUseItemElem );
		pUser->RemoveItem( dwUseItem, 1 );	// ���� ������ ����
	}
	else
		pUser->AddDefinedText( TID_GAME_NOTELEMENT );
}

int CItemUpgrade::GetAttributeEnchantProb( int nAbilityOption )
{
	map<int, __ATTRIBUTE_ENCHANT>::iterator it = m_mapAttributeEnchant.find( nAbilityOption + 1 );
	if( it != m_mapAttributeEnchant.end() )
		return it->second.nProb;

	return 0;
}

int CItemUpgrade::GetAttributeDamageFactor( int nAbilityOption )
{
	if( nAbilityOption > GetMaxAttributeEnchantSize() )
		nAbilityOption = GetMaxAttributeEnchantSize();

	map<int, __ATTRIBUTE_ENCHANT>::iterator it = m_mapAttributeEnchant.find( nAbilityOption );
	if( it != m_mapAttributeEnchant.end() )
		return it->second.nAddDamageRate;

	return 0;
}

int CItemUpgrade::GetAttributeDefenseFactor( int nAbilityOption )
{
	if( nAbilityOption > GetMaxAttributeEnchantSize() )
		nAbilityOption = GetMaxAttributeEnchantSize();

	map<int, __ATTRIBUTE_ENCHANT>::iterator it = m_mapAttributeEnchant.find( nAbilityOption );
	if( it != m_mapAttributeEnchant.end() )
		return it->second.nDefenseRate;

	return 0;
}

int CItemUpgrade::GetAttributeAddAtkDmgFactor( int nAbilityOption )
{
	if( nAbilityOption > GetMaxAttributeEnchantSize() )
		nAbilityOption = GetMaxAttributeEnchantSize();

	map<int, __ATTRIBUTE_ENCHANT>::iterator it = m_mapAttributeEnchant.find( nAbilityOption );
	if( it != m_mapAttributeEnchant.end() )
		return it->second.nAddAtkDmgRate;

	return 0;
}

#ifdef BARUNA_UPGRADE_SMELT_SAFETY
__BARUNA_NPC_LEVEL CItemUpgrade::GetBarunaNPCLevel( int nAbilityOption )
{
	__BARUNA_NPC_LEVEL barunaNPCLevel;
	map<int, __BARUNA_NPC_LEVEL>::iterator it = m_mapBarunaNPCLevel.find( nAbilityOption );
	if( it != m_mapBarunaNPCLevel.end() )
		return it->second;

	return barunaNPCLevel;
}

__BARUNA_ENCHANT CItemUpgrade::GetBarunaEnchant( int nAbilityOption, DWORD dwItemKind1 )
{
	__BARUNA_ENCHANT barunaEnchant;

	if ( dwItemKind1 == IK1_WEAPON )
	{
		map<int, __BARUNA_ENCHANT>::iterator it = m_mapBarunaWeaponEnchant.find( nAbilityOption );
		if( it != m_mapBarunaWeaponEnchant.end() )
			return it->second;
	}
	else if ( dwItemKind1 == IK1_ARMOR )
	{
		map<int, __BARUNA_ENCHANT>::iterator it = m_mapBarunaArmorEnchant.find( nAbilityOption );
		if( it != m_mapBarunaArmorEnchant.end() )
			return it->second;
	}

	return barunaEnchant;
}

int		CItemUpgrade::GetMaxBarunaGeneralEnchantSize( DWORD dwItemKind1 ) 
{
	switch ( dwItemKind1 )
	{
		case IK1_WEAPON:
			return m_mapBarunaWeaponEnchant.size(); 
		case IK1_ARMOR:
			return m_mapBarunaArmorEnchant.size(); 
	}

	return 0;
}
#endif

DWORD CItemUpgrade::WhatEleCard( DWORD dwItemType )
{	// �Ӽ� ���� �� ī���� ������ 
	// �Ӽ� �� �ϳ��� ���յ�
	switch( dwItemType )
	{
	case SAI79::FIRE:
		return ITEM_INDEX( 3206, II_GEN_MAT_ELE_FLAME );
	case SAI79::WATER:
		return ITEM_INDEX( 3211, II_GEN_MAT_ELE_RIVER );
	case SAI79::ELECTRICITY:
		return ITEM_INDEX( 3216, II_GEN_MAT_ELE_GENERATOR );
	case SAI79::EARTH:
		return ITEM_INDEX( 3221, II_GEN_MAT_ELE_DESERT );
	case SAI79::WIND:
		return ITEM_INDEX( 3226, II_GEN_MAT_ELE_CYCLON );
	default:
		return 0;
	}
}

void CItemUpgrade::OnItemTransy( FLWSUser* pUser, OBJID objidTarget, OBJID objidTransy, DWORD dwChangeId, BOOL bCash )
{
	FLItemElem* pItemElemTarget = pUser->m_Inventory.GetAtId( objidTarget );	
	if( !IsUsableItem( pItemElemTarget ) )
		return;

	// �����Ǿ� �ִ� �������̸� ����( ������~~~ ���� )
	if( pUser->m_Inventory.IsEquip( objidTarget ) )
	{
		pUser->AddDefinedText( TID_GAME_EQUIPPUT , "" );
		return;
	}

	// ���������� �Ǿ������� �Ұ�.
	if( pItemElemTarget->GetLooksChangeItemID() > 0 ) {
		pUser->AddDefinedText( TID_MMI_NOT_USE_TRANGE );
		return;
	}

	if( bCash )
	{
		FLItemElem* pItemElemTransy = pUser->m_Inventory.GetAtId( objidTransy );
		if( !IsUsableItem( pItemElemTransy ) )
			return;

		// ��ᰡ Ʈ����(ITM)���� �˻�
		if( pItemElemTransy->GetProp()->dwID != ITEM_INDEX( 26204, II_CHR_SYS_SCR_ITEMTRANSY_A ) && pItemElemTransy->GetProp()->dwID != ITEM_INDEX( 26218, II_CHR_SYS_SCR_ITEMTRANSY_B ) )
			return;
		
		// ���� �˻�
		if( pItemElemTransy->GetProp()->dwID == ITEM_INDEX( 26204, II_CHR_SYS_SCR_ITEMTRANSY_A ) )
		{
			if( pItemElemTarget->GetProp()->nMinLimitLevel > 60 )
				return;
		}
		else
		{
			if( pItemElemTarget->GetProp()->nMinLimitLevel < 61 )
				return;
		}

		if( RunItemTransy( pUser, pItemElemTarget, dwChangeId ) )
		{
			g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemElemTransy, pItemElemTransy->GetProp(), "RemoveItem" );	
			pUser->RemoveItem( objidTransy, 1 );		// ���� ������ ��� ����
		}
	}
}

int		CItemUpgrade::GetMaxEnchantSize( FLItemElem* pItemElem )
{
	if( pItemElem != NULL)
	{
		PT_ITEM_SPEC pItemProp = pItemElem->GetProp();
		if( pItemProp != NULL )
		{
			if( pItemProp->IsBaruna() == TRUE )
			{
				return _MAX_ENCHANT_LEVEL_BARUNA;
			}
			else if( pItemElem->IsCollector() == TRUE )
			{
				return _MAX_ENCHANT_LEVEL_COLLECTOR;
			}
			else if( pItemElem->IsAccessory() == TRUE || pItemProp->IsAccessory() == TRUE )
			{
				return _MAX_ENCHANT_LEVEL_ACCESSORY;
			}
			else
			{
				return _MAX_ENCHANT_LEVEL_GENERAL;
			}
		}
	}

	return 0;
}

BOOL CItemUpgrade::RunItemTransy( FLWSUser* pUser, FLItemElem* pItemElemTarget, DWORD dwChangeId )
{
	PT_ITEM_SPEC pItemProp = pItemElemTarget->GetProp();
	PT_ITEM_SPEC pItemPropChange = g_xSpecManager->GetSpecItem( dwChangeId );
	
	// ����� �������� ������ �´��� �˻�.
	if( !pItemProp || !pItemPropChange || pItemProp->dwID == pItemPropChange->dwID 
		|| ( pItemProp->dwItemKind2 != IK2_ARMOR && pItemProp->dwItemKind2 != IK2_ARMORETC )
		|| ( pItemProp->dwItemSex != SEX_MALE && pItemProp->dwItemSex != SEX_FEMALE )
		|| ( pItemPropChange->dwItemKind2 != IK2_ARMOR && pItemPropChange->dwItemKind2 != IK2_ARMORETC )
		|| ( pItemPropChange->dwItemSex != SEX_MALE && pItemPropChange->dwItemSex != SEX_FEMALE )
		|| pItemProp->dwItemSex == pItemPropChange->dwItemSex
		|| pItemProp->dwItemKind1 != pItemPropChange->dwItemKind1 || pItemProp->dwItemKind2 != pItemPropChange->dwItemKind2
		|| pItemProp->dwItemKind3 != pItemPropChange->dwItemKind3 || pItemProp->dwItemJob != pItemPropChange->dwItemJob 
		|| pItemProp->dwHanded != pItemPropChange->dwHanded || pItemProp->dwParts != pItemPropChange->dwParts  
		|| pItemProp->dwItemLV != pItemPropChange->dwItemLV || pItemProp->dwAbilityMin != pItemPropChange->dwAbilityMin
		|| pItemProp->dwAbilityMax != pItemPropChange->dwAbilityMax || pItemProp->fAttackSpeed != pItemPropChange->fAttackSpeed
		)
		return FALSE;


	// ������ Ʈ���� ����
	pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );			
	g_xWSUserManager->AddCreateSfxObj((CMover *)pUser, XI_INDEX( 1714, XI_INT_SUCCESS ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);			

	// ���� �������� Elem ������ ���� �ϰ� ����...
	FLItemElem ItemElemSend;
	ItemElemSend = *pItemElemTarget;
	ItemElemSend.m_dwItemId = pItemPropChange->dwID;
	ItemElemSend.m_nHitPoint	= ( pItemPropChange->dwEndurance == -1 ) ? 0 : pItemPropChange->dwEndurance;//pItemPropChange->dwEndurance;		// ������ 100%

	g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemElemTarget, pItemElemTarget->GetProp(), "RemoveItem" );	
	g_dpDBClient.SendLogSMItemUse( "1", pUser, &ItemElemSend, ItemElemSend.GetProp(), "CreateItem" );	
	pUser->AddDefinedText( TID_GAME_ITEM_TRANSY_SUCCESS, "\"%s\" \"%s\"", pItemElemTarget->GetProp()->szName, ItemElemSend.GetProp()->szName );

	// ���� ������ ��� ����
	pUser->RemoveItem( pItemElemTarget->m_dwObjId, 1 );
	
	// ���ο� ������ ����
	pUser->CreateItem( &ItemElemSend );
	
	return TRUE;
}

void CItemUpgrade::PetVisSize( FLWSUser* pUser, OBJID objIdMaterial )
{
	if( !IsValidObj( pUser ) )
		return;

	FLItemElem* pItemElemPet = pUser->GetVisPetItem();
	FLItemElem* pItemElemMaterial = pUser->m_Inventory.GetAtId( objIdMaterial );

	if( !IsUsableItem( pItemElemPet ) || !pItemElemPet->IsVisPet() )
	{
		pUser->AddDefinedText( TID_GAME_BUFFPET_NOSUMMON02 );	
		return;
	}

	if( !IsUsableItem( pItemElemMaterial ) )
		return;

	if( pItemElemMaterial->m_dwItemId != ITEM_INDEX( 26855, II_SYS_SYS_VIS_KEY01 ) )
		return;


	if( !pItemElemPet->IsPierceAble( NULL_ID, TRUE ) ) // �� ���� Ȯ�� ���� �˻�
	{
		pUser->AddDefinedText( TID_GAME_BUFFPET_EXPANSION );
		return;
	}
		
	g_DPSrvr.PutItemLog( pUser, "!", "VIS_SLOT_MATERIAL", pItemElemMaterial );
	if( pItemElemMaterial->m_bCharged )
		g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemElemMaterial, pItemElemMaterial->GetProp() );	
	pUser->UpdateItem( pItemElemPet->m_dwObjId, UI_PETVIS_SIZE, pItemElemPet->GetGeneralPiercingSize() + 1 );
	g_DPSrvr.PutItemLog( pUser, "#", "VIS_SLOT_SIZE", pItemElemPet );
	pUser->RemoveItem( objIdMaterial, 1 );
}

void CItemUpgrade::SetPetVisItem( FLWSUser* pUser, OBJID objIdVis )
{
	if( !IsValidObj( pUser ) )
		return;

	FLItemElem* pItemElemPet = pUser->GetVisPetItem();
	FLItemElem* pItemElemVis = pUser->m_Inventory.GetAtId( objIdVis );

	if( !IsUsableItem( pItemElemPet ) )
	{
		pUser->AddDefinedText( TID_GAME_BUFFPET_NOSUMMON01 );
		return;
	}
		
	if( !IsUsableItem( pItemElemVis ) )
		return;

	PT_ITEM_SPEC pVisProp = g_xSpecManager->GetSpecItem( pItemElemVis->m_dwItemId );
	if( !pVisProp )
		return;

	if( !pItemElemPet->IsPierceAble( pVisProp->dwItemKind3 ) )	// �� ���� ���� �˻�.
		return;

	int nFirstEmptySlot = NULL_ID;
	for( size_t i=0; i<pItemElemPet->GetGeneralPiercingSize(); i++ )
	{
		DWORD dwVisId = pItemElemPet->GetGeneralPiercingItemID( i );
		if( dwVisId == pVisProp->dwID )	// �̹� ���� ������ �񽺰� �����Ǿ� �ִ�.
		{
			pUser->AddDefinedText( TID_GAME_BUFFPET_EQUIPVIS );
			return;
		}

		if( nFirstEmptySlot == NULL_ID && dwVisId == 0 )
			nFirstEmptySlot = i;
	}

	if( nFirstEmptySlot == NULL_ID )	// ����ִ� �� ������ ����.
	{
		pUser->AddDefinedText( TID_GAME_BUFFPET_LACKSLOT );
		return;
	}

	if( pUser->IsSatisfyNeedVis( pItemElemPet, pVisProp ) != SUCCSESS_NEEDVIS )
	{
		pUser->AddDefinedText( TID_GAME_BUFFPET_REQVIS );
		return;
	}

	
	pUser->ResetPetVisDST( pItemElemPet );
	pUser->UpdateItem( pItemElemPet->m_dwObjId, UI_PETVIS_ITEM, MAKELONG( nFirstEmptySlot, pItemElemVis->m_dwItemId ), pVisProp->dwAbilityMin );
	PutPetVisItemLog( pUser, "!", "VIS_MATERIAL", pItemElemPet, nFirstEmptySlot );
	if( pItemElemVis->m_bCharged )		// ���ȭ ������ �α�
		g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemElemVis, pVisProp );		
	g_DPSrvr.PutItemLog( pUser, "$", "VIS_PIERCING", pItemElemPet );
	pUser->RemoveItem( objIdVis, 1 );
	pUser->SetPetVisDST( pItemElemPet );
	ChangeVisPetSfx( pUser, pItemElemPet );
}

void CItemUpgrade::RemovePetVisItem( FLWSUser* pUser, int nPosition, BOOL bExpired )
{
	if( !IsValidObj( pUser ) )
		return;

	FLItemElem* pItemElemPet = pUser->GetVisPetItem();
	if( !IsUsableItem( pItemElemPet ) )
		return;

	DWORD dwItemId = pItemElemPet->GetGeneralPiercingItemID( nPosition );
	if( dwItemId  == 0 )	// �̹� ����ִ� ����
		return;

	pUser->ResetPetVisDST( pItemElemPet );
	if( bExpired )
		PutPetVisItemLog( pUser, "$", "VIS_REMOVE_EXPIRED", pItemElemPet, nPosition );
	else
		PutPetVisItemLog( pUser, "$", "VIS_REMOVE_BYUSER", pItemElemPet, nPosition );
	pUser->UpdateItem( pItemElemPet->m_dwObjId, UI_PETVIS_ITEM, MAKELONG( nPosition, 0 ), 0 ); // �ش� ������ ����.
	PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( dwItemId );
	if( pItemProp )
		pUser->AddDefinedText( TID_GAME_BUFFPET_REMOVEVIS, "\"%s\"", pItemProp->szName );
	pUser->SetPetVisDST( pItemElemPet );
	ChangeVisPetSfx( pUser, pItemElemPet );
}

void CItemUpgrade::PutPetVisItemLog( FLWSUser* pUser, const char* szAction, const char* szContext, FLItemElem* pItem, int nPosition )
{	// �������� ���ŵ� ���Ŀ� ȣ����� �ʵ��� �����ؾ� ��
	LogItemInfo	log;
	//log.Action	=  szAction;
	//log.SendName	= pUser->GetName();
	//log.RecvName	= szContext;
	FLStrcpy( log.Action, _countof( log.Action ), szAction );
	FLStrcpy( log.SendName, _countof( log.SendName ), pUser->GetName() );
	FLStrcpy( log.RecvName, _countof( log.RecvName ), szContext );


	log.WorldId		= pUser->GetWorld() ? pUser->GetWorld()->GetID() : WI_WORLD_NONE;	// chipi_090623 ���� - ù ���ӽ� ����� ������ ��� ���尡 ���� ���·� ���´�. 
	log.Gold	= nPosition;
	log.Gold2	= pItem->GetVisPetPiercingItemKeepTime( nPosition ) - time_null();
	g_DPSrvr.OnLogItem( log, pItem, pItem->m_nItemNum );
}

void CItemUpgrade::SwapVis( FLWSUser* pUser, int nPos1, int nPos2 )
{
	if( !IsValidObj( pUser ) )
		return;

	FLItemElem* pItemElemPet = pUser->GetVisPetItem();
	if( !IsUsableItem( pItemElemPet ) )
		return;
	
	pUser->UpdateItem( (BYTE)( pItemElemPet->m_dwObjId ), UI_PETVIS_ITEMSWAP, MAKELONG( nPos1, nPos2 ) );
}

// �ְ��� �񽺿� ��ȭ�� ���� ��� SFX�� ��ȭ��Ų��.
void CItemUpgrade::ChangeVisPetSfx( FLWSUser* pUser, FLItemElem* pItemElemPet )
{
	CMover* pVisPet = prj.GetMover( pUser->GetEatPetId() );
	if( IsValidObj( pUser ) && IsValidObj( pVisPet ) )
	{
		DWORD dwSfxId = pItemElemPet->GetVisPetSfxId();
		if( pVisPet->m_dwMoverSfxId != dwSfxId )
		{
			pVisPet->m_dwMoverSfxId = dwSfxId;
			g_xWSUserManager->AddChangeMoverSfxId( pVisPet );
		}
	}
}

void CItemUpgrade::TransFormVisPet( FLWSUser* pUser, OBJID objIdMaterial )
{
	CMover* pEatPet = prj.GetMover( pUser->GetEatPetId() );
	if( IsValidObj( pEatPet ) )
	{
		CAIPet* pAI = static_cast<CAIPet*>( pEatPet->m_pAIInterface );
		if( pAI )
		{
			FLItemElem* pItemEatPet = pUser->m_Inventory.GetAtId( pAI->GetPetItemId() );
			FLItemElem* pItemMaterial = pUser->m_Inventory.GetAtId( objIdMaterial );
			if( IsUsableItem( pItemEatPet ) && IsUsableItem( pItemMaterial ) )
			{
				if( pItemEatPet->IsVisPet() )
				{
					pUser->AddDefinedText( TID_GAME_PET_TRAN_FAILURE );
					return;
				}
								
				if( pItemMaterial->m_bCharged )		// ���ȭ ������ �α�
					g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemMaterial, pItemMaterial->GetProp() );
				pUser->RemoveItem( objIdMaterial, 1 );
				pUser->UpdateItem( pItemEatPet->m_dwObjId, UI_TRANSFORM_VISPET, TRUE );
				g_DPSrvr.PutItemLog( pUser, "!", "TRANSFORM_VISPET", pItemEatPet );
				pUser->AddDefinedText( TID_GAME_PET_TRAN_SUCCESS, "\"%s\"", pItemEatPet->GetProp()->szName );
			}
		}
	}
	else
	{
		pUser->AddDefinedText( TID_GAME_PET_TRAN_FAILURE );
	}
}

