// UltimateWeapon.cpp: implementation of the CUltimateWeapon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UltimateWeapon.h"
#ifdef __WORLDSERVER
#include "../WorldServer/User.h"
#endif // __WORLDSERVER


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//sun: 9,10�� ���� __ULTIMATE
CUltimateWeapon::CUltimateWeapon()
{
#ifdef __WORLDSERVER
	m_nSetGemProb = 0;
	m_nRemoveGemProb = 0;
	m_nGen2UniProb = 0;
	m_nUni2UltiProb= 0;
#endif // __WORLDSERVER
}

CUltimateWeapon::~CUltimateWeapon()
{

}

BOOL CUltimateWeapon::Load_GemAbility()
{
	CScanner s;
	
	if( !s.Load( "Ultimate_GemAbility.txt" ) )
		return FALSE;
	
//	int nMaxGemNum = 0;
	s.GetToken();
	while( s.tok != FINISHED )
	{
		if( s.Token == _T( "ABILITY" ) )
		{
			__GEMABILITYKIND GemAbilityKind;
			s.GetToken();
			GemAbilityKind.nAbility = CScript::GetDefineNum( s.Token );
			
			s.GetToken(); // {
			s.GetToken();
			while( *s.token != '}' )
			{
				__GEMABILITY GemAbility;
				GemAbility.dwGemItemId = CScript::GetDefineNum( s.Token );
				s.GetToken();
				GemAbility.dwAbilityItemId = CScript::GetDefineNum( s.Token );
				for( int i=0; i<5; i++ )
					GemAbility.vecAbility.push_back( s.GetNumber() );
				GemAbilityKind.vecAbilityKind.push_back( GemAbility );
				s.GetToken();
			}
			m_vecGemAbilityKind.push_back( GemAbilityKind );
		}
		else if( s.Token == _T( "LEVEL_GEM" ) )
		{
			__MAKEGEMITEM	MakeGemItem;
			s.GetToken(); // {
			s.GetToken();
			while( *s.token != '}' )
			{
				MakeGemItem.dwItemID = CScript::GetDefineNum( s.Token );
				if( MakeGemItem.dwItemID == (DWORD)-1 )		// cr
				{
					FLERROR_LOG( PROGRAM_NAME, _T( "�������� ���°��� ��������" ) );
					return FALSE;
				}
				MakeGemItem.nMinLevel = s.GetNumber();
				MakeGemItem.nMaxLevel = s.GetNumber();

				m_vecMakeGemItem.push_back( MakeGemItem );
				s.GetToken();
			}
		}
		s.GetToken();
	}	
	return TRUE;
}

// ������ ���� ������ ���� ���� ����
DWORD CUltimateWeapon::GetGemKind( DWORD dwLimitLevel )
{
	for( DWORD i=0; i<m_vecMakeGemItem.size(); i++ )
	{
		if( (int)( dwLimitLevel ) <= m_vecMakeGemItem[i].nMaxLevel &&
			(int)( dwLimitLevel ) >= m_vecMakeGemItem[i].nMinLevel )
			return m_vecMakeGemItem[i].dwItemID;
	}

	return NULL_ID;
}

#ifdef __WORLDSERVER
BOOL CUltimateWeapon::Load_UltimateWeapon()
{
	CScanner s;
	
	const TCHAR szFileName[]	= _T( "Ultimate_UltimateWeapon.txt" );

	TCHAR szFullPath[ MAX_PATH ]	= { 0, };
	g_pScriptFileManager->GetScriptFileFullPath( szFullPath, _countof( szFullPath ), szFileName );

	if( !s.Load( szFullPath ) )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "[ FAILED LOAD SCRIPT. FILE_NAME:(%s) ]" ), szFullPath );
		return FALSE;
	}
	

	s.GetToken();
	while( s.tok != FINISHED )
	{
		if( s.Token == _T( "SET_GEM" ) )
			m_nSetGemProb = s.GetNumber();
		else if( s.Token == _T( "REMOVE_GEM" ) )
			m_nRemoveGemProb = s.GetNumber();
		else if( s.Token == _T( "GENERAL2UNIQUE" ) )
			m_nGen2UniProb = s.GetNumber();
		else if( s.Token == _T( "UNIQUE2ULTIMATE" ) )
			m_nUni2UltiProb = s.GetNumber();
		else if( s.Token == _T( "MAKE_GEM" ) )
		{
			__MAKEGEMPROB infoMakeGem;
			s.GetToken();	// {
			s.GetToken();
			while( *s.token != '}' )
			{
				int nEnchant = atoi( s.Token );
				infoMakeGem.dwGeneralProb = s.GetNumber();
				infoMakeGem.nGeneralNum = s.GetNumber();
				infoMakeGem.dwUniqueProb = s.GetNumber();
				infoMakeGem.nUniqueNum = s.GetNumber();

				m_mapMakeGemProb.insert( map<int, __MAKEGEMPROB>::value_type( nEnchant, infoMakeGem ) );
				s.GetToken();
			}
		}
		else if( s.Token == _T( "ULTIMATE_ENCHANT" ) )
		{
			int nNum = 0;
			int nProb= 0;
			s.GetToken(); // {
			s.GetToken();
			while( *s.token != '}' )
			{
				nNum = atoi( s.Token );
				nProb = s.GetNumber();
				m_mapUltimateProb.insert( map<int, int>::value_type( nNum, nProb ) );
				s.GetToken();
			}
		}
		s.GetToken();
	}	
	return TRUE;
}

// ���� �ռ��� �ο� �Ǵ� �ɷ�(�ɷ�)
DWORD CUltimateWeapon::GetGemAbilityKindRandom( DWORD dwGemItemid )
{
	if( m_vecGemAbilityKind.empty() == TRUE )
		return NULL_ID;

	int nRandom = xRandom( m_vecGemAbilityKind.size() );

	for(DWORD i=0; i<m_vecGemAbilityKind[nRandom].vecAbilityKind.size(); i++ )
	{
		if( m_vecGemAbilityKind[nRandom].vecAbilityKind[i].dwGemItemId == dwGemItemid )
			return m_vecGemAbilityKind[nRandom].vecAbilityKind[i].dwAbilityItemId;
	}
	return NULL_ID;
}

// ���� ���� - ���� ������ �ı�
int CUltimateWeapon::MakeGem( FLWSUser* pUser, OBJID objItemId, int & nNum )
{
	FLItemElem* pItemElem	= pUser->m_Inventory.GetAtId( objItemId );
	if( !IsUsableItem( pItemElem ) )
		return ULTIMATE_CANCEL;
	// ���Ⱑ �ƴ� ��
	if( pItemElem->GetProp()->dwItemKind1 != IK1_WEAPON )
	{
#ifdef __INTERNALSERVER
		pUser->AddText( "���Ⱑ �ƴ�." );
#endif // __INTERNALSERVER
		return ULTIMATE_CANCEL;
	}
	// �Ϲݹ��⳪ ����ũ �������� �ƴ� ��
	if(	pItemElem->GetProp()->dwReferStat1 != WEAPON_GENERAL &&
		pItemElem->GetProp()->dwReferStat1 != WEAPON_UNIQUE )
	{
#ifdef __INTERNALSERVER
	pUser->AddText( "dwReferStat1 ���� WEAPON_GENERAL, WEAPON_UNIQUE �� �ƴ�." );
#endif // __INTERNALSERVER
		return ULTIMATE_CANCEL;
	}
	
	// �����Ǿ� ������ ���
	if( pUser->m_Inventory.IsEquip( objItemId ) )
	{
		pUser->AddDefinedText( TID_GAME_ULTIMATE_ISEQUIP , "" );
		return ULTIMATE_CANCEL;
	}
	// ������ �ش��ϴ� ���� 
	DWORD dwItemId = GetGemKind( static_cast<DWORD>( pItemElem->GetProp()->nMinLimitLevel ) );
	if( dwItemId == NULL_ID )
		return ULTIMATE_CANCEL;

	int nOpt = pItemElem->GetAbilityOption();
	map<int, __MAKEGEMPROB>::iterator it = m_mapMakeGemProb.find( nOpt );
	
	if( it == m_mapMakeGemProb.end() )
		return ULTIMATE_CANCEL;

	DWORD dwProb = it->second.dwGeneralProb;
	int nItemNum = it->second.nGeneralNum;
	if( pItemElem->GetProp()->dwReferStat1 == WEAPON_UNIQUE )
	{
		dwProb = it->second.dwUniqueProb;
		nItemNum = it->second.nUniqueNum;
	}
	// �κ��丮�� ������ �� - ���
	FLItemElem itemElemTemp;
	itemElemTemp.m_dwItemId = dwItemId;
	itemElemTemp.m_nItemNum = nItemNum;
	PT_ITEM_SPEC pItemprop = itemElemTemp.GetProp();
	if( !pItemprop )
	{
		return ULTIMATE_CANCEL;
	}

	if( pUser->m_Inventory.IsFull( &itemElemTemp, itemElemTemp.m_nItemNum ) == TRUE )
	{
		return ULTIMATE_INVENTORY;
	}
	
	LogItemInfo aLogItem;
	//aLogItem.Action = "-";
	//aLogItem.SendName = pUser->GetName();
	//aLogItem.RecvName = "ULTIMATE_MAKEGEM";
	FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "-" );
	FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
	FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "ULTIMATE_MAKEGEM" );
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
	g_DPSrvr.OnLogItem( aLogItem, pItemElem, 1 );	
	pUser->RemoveItem( objItemId, 1 );
	
	nNum = 0;
	int nRandom = xRandom( 1000000 );
	if( (DWORD)( nRandom ) < dwProb )
	{
		nNum = nItemNum;
		FLItemElem itemElem;
		itemElem.m_dwItemId = dwItemId;
		itemElem.m_nItemNum	= nItemNum;
		itemElem.SetSerialNumber();
		itemElem.m_nHitPoint	= 0;

		pUser->CreateItem( &itemElem );
		//aLogItem.RecvName = "ULTIMATE_MAKEGEM_SUCCESS";
		FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "ULTIMATE_MAKEGEM_SUCCESS" );
		g_DPSrvr.OnLogItem( aLogItem, &itemElem, nItemNum );
		return ULTIMATE_SUCCESS;
	}
	FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "ULTIMATE_MAKEGEM_FAILED" );
	g_DPSrvr.OnLogItem( aLogItem );
	return ULTIMATE_FAILED;
}

// ���⿡ ���� �ռ�
int CUltimateWeapon::SetGem( FLWSUser* pUser, OBJID objItemId, OBJID objGemItemId )
{
	FLItemElem* pItemElem	= pUser->m_Inventory.GetAtId( objItemId );
	FLItemElem* pGemItemElem	= pUser->m_Inventory.GetAtId( objGemItemId );
	if( !IsUsableItem( pItemElem ) || !IsUsableItem( pGemItemElem ) )
		return ULTIMATE_CANCEL;

	// ���͸� ������ �ƴ� ��
	if( pItemElem->GetProp()->dwReferStat1 != WEAPON_ULTIMATE 
#ifdef BARUNA_UPGRADE_ENHANCEMENT_GEM
		&& !pItemElem->GetProp()->IsBarunaWeapon() 
#endif
		)
		return ULTIMATE_ISNOTULTIMATE;


	// �����Ǿ� ������ �ߴ�
	if( pUser->m_Inventory.IsEquip( objItemId ) )
	{
		pUser->AddDefinedText( TID_GAME_ULTIMATE_ISEQUIP , "" );
		return ULTIMATE_CANCEL;
	}
		
//sun: 12, ���� �Ǿ��
	size_t nCount=0;
	for( ; nCount < pItemElem->GetUltimatePiercingSize(); nCount++ )
		if( pItemElem->GetUltimatePiercingItemID( nCount ) == 0 )
			break;
		
	// ����� ������ �ߴ�
//sun: 12, ���� �Ǿ��
	if( nCount == pItemElem->GetUltimatePiercingSize() )
	{
		pUser->AddDefinedText( TID_GAME_ULTIMATE_GEMSPACE, "" );
		return ULTIMATE_CANCEL;
	}

	DWORD dwSetItemId = GetGemAbilityKindRandom( pGemItemElem->m_dwItemId );
	if( dwSetItemId == NULL_ID )
		return ULTIMATE_CANCEL;
	
	// ���� ����
	LogItemInfo aLogItem;
	//aLogItem.Action = "-";
	//aLogItem.SendName = pUser->GetName();
	//aLogItem.RecvName = "ULTIMATE_PIERCING";
	FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "-" );
	FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
	FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "ULTIMATE_PIERCING" );
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
	g_DPSrvr.OnLogItem( aLogItem, pGemItemElem, 1 );	
	pUser->RemoveItem( objGemItemId, 1 );
	
	int nRandom = xRandom( 1000000 );
	if( nRandom < m_nSetGemProb )
	{
		// ���� �ռ�
//sun: 12, ���� �Ǿ��
		pUser->UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_ULTIMATE_PIERCING, MAKELONG( nCount, dwSetItemId ) );

		//aLogItem.RecvName = "ULTIMATE_PIERCING_SUCCESS";
		FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "ULTIMATE_PIERCING_SUCCESS" );
		g_DPSrvr.OnLogItem( aLogItem, pItemElem, 1 );
		return ULTIMATE_SUCCESS;	
	}
	//aLogItem.RecvName = "ULTIMATE_PIERCING_FAILED";
	FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "ULTIMATE_PIERCING_FAILED" );
	g_DPSrvr.OnLogItem( aLogItem, pItemElem );
	return ULTIMATE_FAILED;
}

// ���⿡ ���������� �ռ��� ���� ����
int CUltimateWeapon::RemoveGem( FLWSUser* pUser, OBJID objItemId, OBJID objItemGem )
{
	FLItemElem* pItemElem	= pUser->m_Inventory.GetAtId( objItemId );
	FLItemElem* pItemElemGem	= pUser->m_Inventory.GetAtId( objItemGem );
	if( !IsUsableItem( pItemElem ) || !IsUsableItem( pItemElemGem ) )
		return ULTIMATE_CANCEL;
	
	if( pItemElem->GetProp()->dwReferStat1 != WEAPON_ULTIMATE 
#ifdef BARUNA_UPGRADE_ENHANCEMENT_GEM
		&& !pItemElem->GetProp()->IsBarunaWeapon()
#endif		
		)
		return ULTIMATE_ISNOTULTIMATE;
	
	// �ռ��� ������ ���� �� X
//sun: 12, ���� �Ǿ��
	if( pItemElem->GetUltimatePiercingItemID( 0 ) == 0 )
		return ULTIMATE_CANCEL;

	if( pItemElemGem->m_dwItemId != ITEM_INDEX( 2036, II_GEN_MAT_MOONSTONE )
		&& pItemElemGem->m_dwItemId != ITEM_INDEX( 2083, II_GEN_MAT_MOONSTONE_1 ) 
#ifdef BARUNA_UPGRADE_ENHANCEMENT_GEM
		&& pItemElemGem->m_dwItemId != ITEM_INDEX( 25584, II_GEN_MAT_BARUNA_CANCEL ) 
#endif
		)
		return ULTIMATE_CANCEL;

	if ( pItemElem->GetProp()->IsBarunaWeapon() && pItemElemGem->m_dwItemId != ITEM_INDEX( 25584, II_GEN_MAT_BARUNA_CANCEL ) )
		return ULTIMATE_CANCEL;
 
	// ���� �Ǿ� ���� �� X
	if( pUser->m_Inventory.IsEquip( objItemId ) )
	{
		pUser->AddDefinedText( TID_GAME_ULTIMATE_ISEQUIP , "" );
		return ULTIMATE_CANCEL;
	}	

	// ������ ����
	LogItemInfo aLogItem;
	//aLogItem.Action = "-";
	//aLogItem.SendName = pUser->GetName();
	//aLogItem.RecvName = "ULTIMATE_PIERCING_REMOVE";
	FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "-" );
	FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
	FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "ULTIMATE_PIERCING_REMOVE" );
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
	g_DPSrvr.OnLogItem( aLogItem, pItemElemGem, 1 );
	pUser->RemoveItem( objItemGem, 1 );
	
	int nRandom = xRandom( 1000000 );
	if( nRandom < m_nRemoveGemProb )
	{
		// ������ - ���� ����
//sun: 12, ���� �Ǿ��
		for( int i=pItemElem->GetUltimatePiercingSize()-1; i>=0; i-- )
		{
			if( pItemElem->GetUltimatePiercingItemID( i ) != 0 )
			{
				pUser->UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_ULTIMATE_PIERCING, MAKELONG( i, 0 ) );
				//aLogItem.RecvName = "ULTIMATE_PIERCING_REMOVE_SUC";
				FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "ULTIMATE_PIERCING_REMOVE_SUC" );
				g_DPSrvr.OnLogItem( aLogItem, pItemElem, 1 );
				break;
			}
		}
		return ULTIMATE_SUCCESS;
	}
	//aLogItem.RecvName = "ULTIMATE_PIERCING_REMOVE_FAIL";
	FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "ULTIMATE_PIERCING_REMOVE_FAIL" );
	g_DPSrvr.OnLogItem( aLogItem, pItemElem );
	return ULTIMATE_FAILED;
}

// ������ ����Į�� ���� - ����Į�� 5��, ������ 5�� �ռ�

//############# neodeath : ���� ������ �ڵ�� ���������������� ���� ���� �� 
int CUltimateWeapon::MakeItem( FLWSUser* pUser, OBJID* objItemId )
{
	map<OBJID, int> mapObjId1;
	map<OBJID, int> mapObjId2;

	for( int i=0; i<5; i++ )
	{
		map<OBJID, int>::iterator it1 = mapObjId1.find(objItemId[i]);
		if( it1 != mapObjId1.end() )
			++(it1)->second;
		else
			mapObjId1.insert(map<OBJID, int>::value_type( objItemId[i], 1 ));
		
		map<OBJID, int>::iterator it2 = mapObjId2.find(objItemId[i+5]);
		if( it2 != mapObjId2.end() )
			++(it2)->second;
		else
			mapObjId2.insert(map<OBJID, int>::value_type( objItemId[i+5], 1 ));
	}

	int nItemCount = 0;
	for( map<OBJID, int>::iterator it = mapObjId1.begin() ; it != mapObjId1.end() ; ++it )
	{
		FLItemElem* pItemElem;
		pItemElem = pUser->m_Inventory.GetAtId( it->first );
		if( !IsUsableItem( pItemElem ) )
			return ULTIMATE_CANCEL;
		
		if( pItemElem->GetProp()->dwID != ITEM_INDEX( 2035, II_GEN_MAT_ORICHALCUM01 )
			&& pItemElem->GetProp()->dwID != ITEM_INDEX( 2082, II_GEN_MAT_ORICHALCUM01_1 ) )
			return ULTIMATE_CANCEL;
		if( it->second > pItemElem->m_nItemNum )
			return ULTIMATE_CANCEL;

		nItemCount += it->second;
	}
	if( nItemCount != 5 )
		return ULTIMATE_CANCEL;

	nItemCount = 0;
	for( map<OBJID, int>::iterator it = mapObjId2.begin() ; it != mapObjId2.end() ; ++it )
	{
		FLItemElem* pItemElem;
		pItemElem = pUser->m_Inventory.GetAtId( it->first );
		if( !IsUsableItem( pItemElem ) )
			return ULTIMATE_CANCEL;
		
		if( pItemElem->GetProp()->dwID != ITEM_INDEX( 2036, II_GEN_MAT_MOONSTONE )
			&& pItemElem->GetProp()->dwID != ITEM_INDEX( 2083, II_GEN_MAT_MOONSTONE_1 ) )
			return ULTIMATE_CANCEL;
		if( it->second > pItemElem->m_nItemNum )
			return ULTIMATE_CANCEL;
		
		nItemCount += it->second;
	}

	if( nItemCount != 5 )
		return ULTIMATE_CANCEL;
	
	//�κ��丮�� ������ �� - ���
	FLItemElem itemElemTemp;
	itemElemTemp.m_dwItemId = ITEM_INDEX( 2034, II_GEN_MAT_ORICHALCUM02 );
	itemElemTemp.m_nItemNum = 1;
	if( !itemElemTemp.GetProp() || pUser->m_Inventory.IsFull( &itemElemTemp, itemElemTemp.m_nItemNum ) )
	{
		return ULTIMATE_INVENTORY;
	}
	
	LogItemInfo aLogItem;
	//aLogItem.Action = "-";
	//aLogItem.SendName = pUser->GetName();
	//aLogItem.RecvName = "ULTIMATE_MAKEITEM";
	FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "-" );
	FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
	FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "ULTIMATE_MAKEITEM" );
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
	
	FLItemElem* pItemElem;
	for( map<OBJID, int>::iterator it = mapObjId1.begin() ; it != mapObjId1.end() ; ++it )
	{
		pItemElem = pUser->m_Inventory.GetAtId( it->first );
		g_DPSrvr.OnLogItem( aLogItem, pItemElem, it->second );	
		pUser->RemoveItem( it->first, it->second );
	}
	for( map<OBJID, int>::iterator it = mapObjId2.begin() ; it != mapObjId2.end() ; ++it )
	{
		pItemElem = pUser->m_Inventory.GetAtId( it->first );
		g_DPSrvr.OnLogItem( aLogItem, pItemElem, it->second );	
		pUser->RemoveItem( it->first, it->second );
	}

	FLItemElem itemElem;
	itemElem.m_dwItemId = ITEM_INDEX( 2034, II_GEN_MAT_ORICHALCUM02 );
	itemElem.m_nItemNum	= 1;
	itemElem.SetSerialNumber();
	itemElem.m_nHitPoint	= 0;
	
	pUser->CreateItem( &itemElem );
	//aLogItem.RecvName = "ULTIMATE_MAKEITEM_SUCCESS";
	FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "ULTIMATE_MAKEITEM_SUCCESS" );
	g_DPSrvr.OnLogItem( aLogItem, &itemElem, 1 );
	
	return ULTIMATE_SUCCESS;
}

// ���⺯ȯ(�Ϲ�->����ũ, ����ũ->���͸�)
int CUltimateWeapon::TransWeapon( FLWSUser* pUser, OBJID objItem, OBJID objGem1, OBJID objGem2 )
{
	FLItemElem* pItemElemWeapon	= pUser->m_Inventory.GetAtId( objItem );
	FLItemElem* pItemElemGem1	= pUser->m_Inventory.GetAtId( objGem1 );
	FLItemElem* pItemElemGem2	= pUser->m_Inventory.GetAtId( objGem2 );
	if( IsUsableItem( pItemElemWeapon ) == FALSE
		|| IsUsableItem( pItemElemGem1 ) == FALSE
		|| IsUsableItem( pItemElemGem2 ) == FALSE )
		return ULTIMATE_CANCEL;

	PT_ITEM_SPEC pMainProp = g_xSpecManager->GetSpecItem( pItemElemWeapon->m_dwItemId );
	if( pMainProp == NULL )
	{
		return ULTIMATE_CANCEL;
	}
	
	// �����ϰ� ������ ���
	if( pUser->m_Inventory.IsEquip( objItem ) )
	{
		pUser->AddDefinedText( TID_GAME_ULTIMATE_ISEQUIP , "" );
		return ULTIMATE_CANCEL;
	}
		
	// ���Ⱑ �ƴϸ� ���
	if( pItemElemWeapon->GetProp()->dwItemKind1 != IK1_WEAPON )
	{
#ifdef __INTERNALSERVER
		pUser->AddText( "���Ⱑ �ƴ�." );
#endif // __INTERNALSERVER
		return ULTIMATE_CANCEL;
	}

	// Ÿ�� ID �� ���� ��
	if( pItemElemWeapon->GetProp()->dwReferTarget1 == NULL_ID )
	{
#ifdef __INTERNALSERVER
		pUser->AddText( "dwReferTarget1 �� ��ȯ�� ������ ID�� ����." );
#endif // __INTERNALSERVER
		return ULTIMATE_CANCEL;	
	}
	
	// �Ϲ�, ����ũ �������� �ƴ� ��
	if( pItemElemWeapon->GetProp()->dwReferStat1 != WEAPON_GENERAL &&
		  pItemElemWeapon->GetProp()->dwReferStat1 != WEAPON_UNIQUE )
	{
#ifdef __INTERNALSERVER
		pUser->AddText( "dwReferStat1 ���� WEAPON_GENERAL, WEAPON_UNIQUE �� �ƴ�." );
#endif // __INTERNALSERVER
		return ULTIMATE_CANCEL;
	}

	DWORD dwItemId = GetGemKind( static_cast<DWORD>( pItemElemWeapon->GetProp()->nMinLimitLevel ) );
	if( dwItemId != pItemElemGem1->m_dwItemId || pItemElemGem2->m_dwItemId != ITEM_INDEX( 2034, II_GEN_MAT_ORICHALCUM02 ) )
		return ULTIMATE_CANCEL;

	CString strTemp = "UNIQUE_"; 
	CString strLog;
	int nProb = m_nGen2UniProb;
	if( pItemElemWeapon->GetProp()->dwReferStat1 == WEAPON_UNIQUE )
	{
		if( pItemElemWeapon->GetAbilityOption() != 10 )
			return ULTIMATE_CANCEL;
		nProb = m_nUni2UltiProb;
		strTemp = "ULTIMATE_";
	}
	// propItem.txt -> dwReferTarget2(����Ÿ��2)�� ���� ��ȯ Ȯ���� �ȴ�.
	// ���� ������ Ultimate_UltimateWeapon.txt�� ���ǵ� ���� ��ȯ Ȯ���� �ȴ�.
	if( pItemElemWeapon->GetProp()->dwReferTarget2 != NULL_ID )
		nProb = pItemElemWeapon->GetProp()->dwReferTarget2;
	
	BOOL bSmelprot3	= FALSE;
	if( pUser->HasBuff( BUFF_ITEM, ITEM_INDEX( 10488, II_SYS_SYS_SCR_SMELPROT3 ) )
		&& pItemElemWeapon->GetProp()->dwReferStat1 == WEAPON_UNIQUE )
	{
		bSmelprot3	= TRUE;
		pUser->RemoveBuff( BUFF_ITEM, ITEM_INDEX( 10488, II_SYS_SYS_SCR_SMELPROT3 ) );
		
	}

	LogItemInfo aLogItem;
//	aLogItem.Action = "-";
//	aLogItem.SendName = pUser->GetName();
//	strLog = strTemp + "TRANS";
//	aLogItem.RecvName = (LPCTSTR)strLog;
	FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "-" );
    FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
	strLog = strTemp + "TRANS";
	FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), (LPCTSTR)strLog );

	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
	g_DPSrvr.OnLogItem( aLogItem, pItemElemGem1, 1 );
	pUser->RemoveItem( objGem1, 1 );
	g_DPSrvr.OnLogItem( aLogItem, pItemElemGem2, 1 );
	pUser->RemoveItem( objGem2, 1 );

	int nAddProp = g_xApplyItemEffect->GetAdjValueGeneralEnchantRateBuff( pUser, pItemElemWeapon, IK3_ULTIMATE_UPGRADE_RATE, true );
	if( nAddProp > 0 )
	{
		nAddProp *= 100;
		nProb += nAddProp;
	}

	int nRandom = xRandom( 1000000 );
	if( nRandom < nProb )
	{
		FLItemElem CreateItem;
		CreateItem	= *pItemElemWeapon;
		CreateItem.m_dwItemId = pItemElemWeapon->GetProp()->dwReferTarget1;
		CreateItem.SetSerialNumber();

		if( pItemElemWeapon->GetProp()->dwReferStat1 == WEAPON_UNIQUE )
			CreateItem.SetAbilityOption( 0 );
		

		g_DPSrvr.OnLogItem( aLogItem, pItemElemWeapon, 1 );
		pUser->RemoveItem( objItem, 1 );
		if( pUser->CreateItem( &CreateItem ) )
		{
			//aLogItem.RecvName = "ULTIMATE_TRANS_SUCCESS";
			strLog = strTemp + "TRANS_SUCCESS";
			//aLogItem.RecvName = (LPCTSTR)strLog;
			FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), (LPCTSTR)strLog );
			g_DPSrvr.OnLogItem( aLogItem, &CreateItem, 1 );
			return ULTIMATE_SUCCESS;
		}
	}
	else //����
	{
		if( !bSmelprot3 )
		{
			//aLogItem.RecvName = "ULTIMATE_TRANS_FAILED";
			strLog = strTemp + "TRANS_FAILED";
			//aLogItem.RecvName = (LPCTSTR)strLog;
			FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), (LPCTSTR)strLog );
			g_DPSrvr.OnLogItem( aLogItem, pItemElemWeapon, 1 );
			pUser->RemoveItem( objItem, 1 );
		}
		else
		{
			//aLogItem.RecvName = "ULTIMATE_TRANS_PROTECT";
			strLog = strTemp + "TRANS_PROTECT";
			//aLogItem.RecvName = (LPCTSTR)strLog;
			FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), (LPCTSTR)strLog );
			g_DPSrvr.OnLogItem( aLogItem, pItemElemWeapon );
		}
	}
	
	return ULTIMATE_FAILED;
}

// ���͸� ���� ����
int CUltimateWeapon::EnchantWeapon( FLWSUser* pUser, OBJID objItem, OBJID objItemGem )
{
	FLItemElem* pItemElemWeapon	= pUser->m_Inventory.GetAtId( objItem );
	FLItemElem* pItemElemGem	= pUser->m_Inventory.GetAtId( objItemGem );
	if( IsUsableItem( pItemElemWeapon ) == FALSE || IsUsableItem( pItemElemGem ) == FALSE )
		return ULTIMATE_CANCEL;

	//���͸� ������ �ƴϰų� ������ ����Į���� �ƴϸ� ���
	if( pItemElemWeapon->GetProp()->dwReferStat1 != WEAPON_ULTIMATE ||
		pItemElemGem->m_dwItemId != ITEM_INDEX( 2034, II_GEN_MAT_ORICHALCUM02 ) )
		return ULTIMATE_ISNOTULTIMATE;

	// �����ϰ� ������ ���
	if( pUser->m_Inventory.IsEquip( objItem ) )
	{
		pUser->AddDefinedText( TID_GAME_ULTIMATE_ISEQUIP , "" );
		return ULTIMATE_CANCEL;
	}

	int pAbilityOpt = pItemElemWeapon->GetAbilityOption();
	if( (pAbilityOpt+1) > 10 )
	{
		pUser->AddDefinedText( TID_UPGRADE_MAXOVER , "" );
		return ULTIMATE_CANCEL;
	}

	BOOL bSmelprot3	= FALSE;
	if( pUser->HasBuff( BUFF_ITEM, ITEM_INDEX( 10488, II_SYS_SYS_SCR_SMELPROT3 ) ) )
	{
		bSmelprot3	= TRUE;
		pUser->RemoveBuff( BUFF_ITEM, ITEM_INDEX( 10488, II_SYS_SYS_SCR_SMELPROT3 ) );
		
	}
	
	//���� ����
	LogItemInfo aLogItem;
	//aLogItem.Action = "-";
	//aLogItem.SendName = pUser->GetName();
	//aLogItem.RecvName = "ULTIMATE_ENCHANT";
	FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "-" );
	FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
	FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "ULTIMATE_ENCHANT" );
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
	g_DPSrvr.OnLogItem( aLogItem, pItemElemGem, 1 );	
	pUser->RemoveItem( objItemGem, 1 );

	int nRandom = xRandom( 1000000 );
	map<int, int>::iterator it = m_mapUltimateProb.find( pAbilityOpt+1 );
	if( it == m_mapUltimateProb.end() )
		return ULTIMATE_CANCEL;
	
	if( nRandom < it->second )
	{
		pUser->UpdateItem( (BYTE)pItemElemWeapon->m_dwObjId, UI_AO, ++pAbilityOpt );
		if( pAbilityOpt > 5 )
//sun: 12, ���� �Ǿ��
			pUser->UpdateItem( (BYTE)pItemElemWeapon->m_dwObjId, UI_ULTIMATE_PIERCING_SIZE, pAbilityOpt - 5 );

		//aLogItem.RecvName = "ULTIMATE_ENCHANT_SUCCESS";
		FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "ULTIMATE_ENCHANT_SUCCESS" );
		g_DPSrvr.OnLogItem( aLogItem, pItemElemWeapon, 1 );
		return ULTIMATE_SUCCESS;
	}
	else
	{
		if( bSmelprot3 )	// ��ȣ�� �η縶��(�ֻ��) ����
		{
			//aLogItem.RecvName = "ULTIMATE_ENCHANT_PROTECT";
			FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "ULTIMATE_ENCHANT_PROTECT" );
			g_DPSrvr.OnLogItem( aLogItem, pItemElemWeapon );
		}
		else	// ���͸� ���� �ı�
		{
			//aLogItem.RecvName = "ULTIMATE_ENCHANT_FAILED";
			FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "ULTIMATE_ENCHANT_FAILED" );
			g_DPSrvr.OnLogItem( aLogItem, pItemElemWeapon, 1 );
			pUser->RemoveItem( objItem, 1 );
		}
	}	
	return ULTIMATE_FAILED;
}


BYTE CUltimateWeapon::SmeltSafetyUltimate( FLWSUser* pUser, FLItemElem* pItemMain, FLItemElem* pItemMaterial, FLItemElem* pItemProtScr )
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

	//	���͸ڿ����� �ƴϸ� ����
	if( pMainProp->dwReferStat1 != WEAPON_ULTIMATE )
	{
		//pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return 0;
	}

	//	�ֻ�� ��ȣ�� �η縶���� �ƴϸ� ����
	if( pProtScrProp->dwID != ITEM_INDEX( 10488, II_SYS_SYS_SCR_SMELPROT3 ))
	{
		//pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return 0;
	}

	//	���ü�ġ�� maxġ�� �Ѿ����� ����
	if( pItemMain->GetAbilityOption() >= 10 )
	{
		//pUser->AddDefinedText( TID_UPGRADE_MAXOVER );
		return 3;
	}

#ifdef __ENCHANT_BARUNA16

	if( pMainProp->IsBaruna() == TRUE )
	{
		return 0;
	}

#endif // __ENCHANT_BARUNA16


	LogItemInfo aLogItem;
	//aLogItem.Action = "-";
	//aLogItem.SendName = pUser->GetName();
	//aLogItem.RecvName = "ULTIMATE_ENC_SMELTSAFETY";
	FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "-" );
	FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
	FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "ULTIMATE_ENC_SMELTSAFETY" );
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = pUser->GetGold();
	aLogItem.Gold2 = pUser->GetGold();
	
	// ���þ��� ���� - �����̴�, ���д�...
	g_DPSrvr.OnLogItem( aLogItem, pItemMaterial, pItemMaterial->m_nItemNum );
	pUser->RemoveItem( pItemMaterial->m_dwObjId, 1 );
	//	�η縶�� �α׳����
	g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemProtScr, pProtScrProp );
	g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pProtScrProp );

	pUser->RemoveItem( pItemProtScr->m_dwObjId, 1 );

	// ���� Ȯ��
	map<int, int>::iterator it = m_mapUltimateProb.find( pItemMain->GetAbilityOption() + 1 );
	if( it == m_mapUltimateProb.end() )
		return 0;
	
	if( (int)( xRandom( 1000000 ) ) > it->second )
	{
		// ����
		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );
		if( ( pUser->IsMode( TRANSPARENT_MODE ) ) == 0 )
			g_xWSUserManager->AddCreateSfxObj( ( CMover * )pUser, XI_INDEX( 1715, XI_INT_FAIL ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );

		//aLogItem.RecvName = "ULTIMATE_ENC_FAIL_SMELTSAFETY";
		FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "ULTIMATE_ENC_FAIL_SMELTSAFETY" );
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );

		return 2;
	}
	else
	{
		// ����
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );		
		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0 )
			g_xWSUserManager->AddCreateSfxObj( ( CMover * )pUser, XI_INDEX( 1714, XI_INT_SUCCESS ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z );
		
		pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_AO, pItemMain->GetAbilityOption() + 1 );
		if( pItemMain->GetAbilityOption() > 5 )
			pUser->UpdateItem( (BYTE)pItemMain->m_dwObjId, UI_ULTIMATE_PIERCING_SIZE, pItemMain->GetAbilityOption() - 5 );
		
		//aLogItem.RecvName = "ULTIMATE_ENC_SUC_SMELTSAFETY";
		FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "ULTIMATE_ENC_SUC_SMELTSAFETY" );
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );

		return 1;
	}
//	return 0;
}
#endif // __WORLDSERVER

// ���� �ռ� �� �ɷ�ġ ����
void CUltimateWeapon::SetDestParamUltimate( CMover* pMover, FLItemElem* pItemElem, BOOL bEquip, DWORD dwParts )
{
	
	if( dwParts != PARTS_RWEAPON )
		return;
	
	map<int, int> mapDst = GetDestParamUltimate( pItemElem );
	for(map<int, int>::iterator it = mapDst.begin(); it != mapDst.end(); it++ )
	{
		if( bEquip )
			pMover->SetDestParam( it->first, it->second, NULL_CHGPARAM );
		else 
			pMover->ResetDestParam( it->first, it->second, TRUE );
	}
}

// �ռ��� �������� ���� �� �ɷ�
map<int, int> CUltimateWeapon::GetDestParamUltimate( FLItemElem* pItemElem )
{
	map<int, int> mapDst;  
	if( !pItemElem || (pItemElem->GetProp()->dwReferStat1 != WEAPON_ULTIMATE 
#ifdef BARUNA_UPGRADE_ENHANCEMENT_GEM
		&& !pItemElem->GetProp()->IsBarunaWeapon()
#endif
	) )
		return mapDst;
	
	map<DWORD, int> mapItem;		
//sun: 12, ���� �Ǿ��
	for( size_t i=0; i<pItemElem->GetUltimatePiercingSize(); i++ )
	{
		if( pItemElem->GetUltimatePiercingItemID( i ) == 0 )
			break;

		map<DWORD, int>::iterator it=mapItem.find( pItemElem->GetUltimatePiercingItemID( i ) );
		if( it==mapItem.end() )
			mapItem.insert( map<DWORD, int>::value_type( pItemElem->GetUltimatePiercingItemID( i ), 1 ) );
		else
			it->second++;
	}
	
	for( map<DWORD, int>::iterator it=mapItem.begin(); it!=mapItem.end(); it++ )
	{
		PT_ITEM_SPEC pItemProp	= g_xSpecManager->GetSpecItem( it->first );
		int nResultDst = GetDST( it->first, it->second, pItemProp->dwReferStat2 );
		
		map<int, int>::iterator iter=mapDst.find( pItemProp->dwReferStat2 );
		if( iter==mapDst.end() )
			mapDst.insert( map<int, int>::value_type( pItemProp->dwReferStat2, nResultDst ) );
		else
			iter->second += nResultDst;
	}
	return mapDst;
}

// �ռ��� ������ ������ ������ ���� �ɷ�ġ
int CUltimateWeapon::GetDST( DWORD dwItemId, int nItemNum, int nDSTInfo )
{
	int nResultDST = 0;
	__GEMABILITYKIND	GemAbilityKind;

	BOOL bFind = FALSE;
	for( DWORD i=0; i<m_vecGemAbilityKind.size(); i++ )
	{
		if( m_vecGemAbilityKind[i].nAbility == nDSTInfo )
		{
			GemAbilityKind = m_vecGemAbilityKind[i];
			bFind = TRUE;
			break;
		}
	}

	if( !bFind )
		return 0;

	for( DWORD i = 0 ; i < GemAbilityKind.vecAbilityKind.size() ; ++i )
	{
		if( dwItemId == GemAbilityKind.vecAbilityKind[i].dwAbilityItemId )
		{
			nResultDST = GemAbilityKind.vecAbilityKind[i].vecAbility[nItemNum-1];
			break;
		}
	}	
	return nResultDST;
}
