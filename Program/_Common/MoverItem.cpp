#include "stdafx.h"
#include "mover.h"
#include "defineText.h"
#ifdef __WORLDSERVER
#include "../WorldServer/user.h"
#endif	// WORLDSERVER

//_SUN_CHECKDATA_
//#include "DPClient.h"
//extern	CDPClient	g_DPlay;


// pc, npc�� �и� 
// m_nCost���� 

CVTInfo::CVTInfo()
{
	Clear();
}

void CVTInfo::Clear()
{
	m_dwTradeGold		= 0;
	memset( m_apItem_VT, 0, sizeof( m_apItem_VT ) );
	m_objId				= NULL_ID;
	m_pOwner			= NULL;
	m_state				= TRADE_STEP_ITEM;
}


// �ŷ� ������ ��´�.
CMover* CVTInfo::GetOther() const
{
//	return m_pOther;
	if( m_objId == NULL_ID )
		return NULL;

	return prj.GetMover( m_objId );
}

// �ŷ� ������ ���Ѵ�.
void CVTInfo::SetOther( CMover* pMover )
{
//	m_pOther = pMover;
	if( pMover )
	{
		m_objId = pMover->GetId();
	}
	else
	{
		m_objId = NULL_ID;
	}
}

FLItemBase* CVTInfo::GetItem( BYTE byNth )
{
	return m_apItem_VT[byNth];
}

void CVTInfo::SetItem( BYTE byNth, FLItemBase* pItemBase )
{
	m_apItem_VT[byNth] = pItemBase;
}

LPCTSTR	CVTInfo::GetTitle()
{
	return m_strTitle.c_str();
}

void CVTInfo::SetTitle( LPCTSTR szTitle )
{
	m_strTitle = szTitle;
}

BOOL CVTInfo::IsVendorOpen()
{
	return (m_strTitle.empty() != true);
}

void CVTInfo::Init( CMover* pOwner )
{
	m_pOwner = pOwner;
	ZeroMemory( m_apItem_VT, sizeof( m_apItem_VT ) );
	TradeSetGold( 0 );							// raiders.2006.11.28 
	TradeClear();
	m_strTitle = "";
}




void CVTInfo::TradeClear()
{
	SetOther( NULL );
	for( int i = 0; i < MAX_TRADE; i++ )
	{
		if( m_apItem_VT[i] )
		{
			m_apItem_VT[i]->SetExtra( 0 );
			m_apItem_VT[i] = NULL;
		}
	}
#ifdef __WORLDSERVER	
	// raiders.2006.11.28	�κ��� = �κ��� + �� �ŷ�â ��
	int nGold = TradeGetGold();
	if( nGold > 0 && m_pOwner )
		m_pOwner->AddGold( nGold );
	//
#endif

	TradeSetGold( 0 );
	TradeSetState( TRADE_STEP_ITEM );
}

void CVTInfo::TradeSetGold( DWORD dwGold )
{
#ifdef __CLIENT		
	//_SUN_CHECKDATA_
	if( dwGold > MAX_PEYNA_PERSON )
	{
//		g_DPlay.SendError( CLERR_PENYA_OVERLIMIT_P, 0 );
		return;
	}
#endif	//__CLIENT
	m_dwTradeGold = dwGold;
}

int CVTInfo::TradeGetGold()
{
	return m_dwTradeGold;
}

void CVTInfo::TradeSetItem( DWORD dwItemObjID, BYTE byNth, int nItemNum )
{
	FLItemBase* pItemBase = m_pOwner->GetItemId( dwItemObjID );

	if( pItemBase )
	{
		m_apItem_VT[byNth] = pItemBase;
		pItemBase->SetExtra( nItemNum );
	}
}

BOOL CVTInfo::TradeClearItem( BYTE byNth )
{
	FLItemBase* pItemBase = m_apItem_VT[byNth];
	if( IsUsingItem( pItemBase ) )
	{
		pItemBase->SetExtra( 0 );		// clear - using flag 
		m_apItem_VT[byNth] = NULL;

		return TRUE;
	}
	else
		return FALSE;
}

//raiders.2006.11.28  ������ ���� ( �κ��� = �κ��� + ���� �ŷ�â �� )
BOOL CVTInfo::TradeConsent( CAr & ar )
{
	int nGold = 0;
	ar >> nGold;
	m_pOwner->SetGold( nGold );

	int nCount = 0;
	ar >> nCount;
	for( int i = 0; i < nCount; ++i )
	{
		DWORD dwObjID = 0;
		int nItemNum = 0;
		ar >> dwObjID >> nItemNum;

		if( nItemNum > 0 )
		{
			FLItemElem * pItemElem = m_pOwner->m_Inventory.GetAtId( dwObjID );
			pItemElem->m_nItemNum = nItemNum;
		}
		else
		{
			m_pOwner->m_Inventory.RemoveAtId( dwObjID );
		}
	}

	nCount = 0;
	ar >> nCount;
	for( int i = 0; i < nCount; ++i )
	{
		FLItemElem itemElem;
		itemElem.Deserialize( ar );

		m_pOwner->m_Inventory.Add( &itemElem );
	}

	CMover* pTrader	= GetOther();
	if( pTrader != NULL )
	{
		pTrader->m_vtInfo.TradeSetGold( 0 );	// ���� �ȵǰ� 
		pTrader->m_vtInfo.TradeClear();
	}

	// step2. m_dwTradeGold�� clear
	TradeSetGold( 0 );						// ���� �ȵǰ� 
	TradeClear();

	return TRUE;
}

DWORD CVTInfo::TradeSetItem2( DWORD dwItemObjID, BYTE byNth, int & nItemNum )
{
	FLItemBase* pItemBase = m_pOwner->GetItemId( dwItemObjID );
	if( IsUsableItem( pItemBase ) == FALSE || m_apItem_VT[byNth] != NULL )  
		return (DWORD)TID_GAME_CANNOTTRADE_ITEM;

	if( m_pOwner->m_Inventory.IsEquip( pItemBase->m_dwObjId ) ) 
		return (DWORD)TID_GAME_CANNOTTRADE_ITEM;
	
	if( ( (FLItemElem*)pItemBase )->IsQuest() )
		return (DWORD)TID_GAME_CANNOTTRADE_ITEM;

	if( ( (FLItemElem*)pItemBase )->IsOwnState() )
		return (DWORD)TID_GAME_CANNOTTRADE_ITEM;

	if( m_pOwner->IsUsing( (FLItemElem*)pItemBase ) )
		return (DWORD)TID_GAME_CANNOT_DO_USINGITEM;

	if( pItemBase->GetProp()->dwItemKind3 == IK3_CLOAK && ( (FLItemElem*)pItemBase )->m_idGuild != 0 )
		return (DWORD)TID_GAME_CANNOTTRADE_ITEM;
	
	if( pItemBase->GetProp()->dwParts == PARTS_RIDE && pItemBase->GetProp()->dwItemJob == JOB_VAGRANT )
		return (DWORD)TID_GAME_CANNOTTRADE_ITEM;
	
	if( nItemNum < 1)
	{
		nItemNum = 1;
	}

	if( nItemNum > ( (FLItemElem*)pItemBase )->m_nItemNum )
	{
		nItemNum = ( (FLItemElem*)pItemBase )->m_nItemNum;
	}
	
	TradeSetItem( dwItemObjID, byNth, nItemNum );

	return 0;
}

// nMinus - ���� �� 
// nPlus  - ���� �� 
BOOL CheckTradeGold( CMover* pMover, int nMinus, int nPlus )
{
	if( nMinus >= 0 )
	{
//		if( pMover->GetGold() >= nMinus )
		if( pMover->CheckUserGold( nMinus, false ) == true )
		{
			int nGold = pMover->GetGold() - nMinus;
			int nResult = nGold + nPlus;
			if( nPlus >= 0 )
			{
				if( nResult >= nGold )	// overflow�� �ƴϸ�?
					return TRUE;
			}
			else
			{
				if( nResult >= 0 )		// underflow�� �ƴϸ�?
					return TRUE;
			}

		}
	}

	return FALSE;
}

//raiders.2006.11.28  ������ ���� 
//�α׿� üũ�� ����� Ŭ��� ����. �����ؼ� refactoring����.
TRADE_CONFIRM_TYPE CVTInfo::TradeLastConfirm( CAr& ownerar, CAr& traderar, CAr & logar )
{
	// Ʈ���̵� �޽��� ����
	// ���� ���, ���� ī��Ʈ ���� ������ ������(������ ������ �ε���, ����(0�̸� ���� ����)), �߰� ī��Ʈ(�߰��� ������ ����)
	TRADE_CONFIRM_TYPE		result = TRADE_CONFIRM_ERROR;
	CMover*					pTrader	= GetOther();

	// gold_step1. GetGold(), AddGold() �Լ��� ���ϴµ��� ���۵ǵ��� ���� �Ŀ� clear�صд�.
	int nTraderGold        = pTrader->m_vtInfo.TradeGetGold();
	int nUserGold          = TradeGetGold();
	
	// gold_step2. �ٵ��� ������ ���صд�.
	int nThisAdd   = nTraderGold;
	int nTraderAdd = nUserGold;

	// gold_step3. overflow�� �˻� 
	if( CheckTradeGold( m_pOwner, 0, nTraderGold ) == FALSE ||
		CheckTradeGold( pTrader, 0, nUserGold ) == FALSE )
	{
		TradeClear();
		pTrader->m_vtInfo.TradeClear();
		return result;
	}

	// ��ȯ �� ��ŭ ������ �κ��丮 ������ ������ �ִ� ���� �˻��Ѵ�.
	vector<T_ADDITEM_INFO> vecAddItemInfo;
	vector<T_ADDITEM_INFO> vecTraderAddItemInfo;
	vecAddItemInfo.clear();
	vecTraderAddItemInfo.clear();

	for( BYTE i = 0; i < MAX_TRADE; ++i )
	{
		{
			FLItemElem* pItemElem = (FLItemElem*)pTrader->m_vtInfo.GetItem( i );
			if( pItemElem != NULL )
			{
				T_ADDITEM_INFO tAddItemInfo;

				tAddItemInfo.dwItemId	= pItemElem->m_dwItemId;
				tAddItemInfo.nItemNum	= pItemElem->m_nItemNum;

				vecAddItemInfo.push_back( tAddItemInfo );
			}
		}

		{
			FLItemElem* pItemElem = (FLItemElem*)GetItem( i );
			if( pItemElem != NULL )
			{
				T_ADDITEM_INFO tAddItemInfo;

				tAddItemInfo.dwItemId	= pItemElem->m_dwItemId;
				tAddItemInfo.nItemNum	= pItemElem->m_nItemNum;

				vecTraderAddItemInfo.push_back( tAddItemInfo );
			}
		}
	}

	if( m_pOwner->m_Inventory.IsEnoughEmptySpace( vecAddItemInfo ) == false || pTrader->m_Inventory.IsEnoughEmptySpace( vecTraderAddItemInfo ) == false )
	{
		result = TRADE_CONFIRM_ERROR;
	}
	else if ( m_pOwner->m_Inventory.CheckItem( vecTraderAddItemInfo ) == false || pTrader->m_Inventory.CheckItem( vecAddItemInfo ) == false )
	{
		result = TRADE_CONFIRM_ERROR;
	}
	else
	{
		result = TRADE_CONFIRM_OK;
	}

	if( result == TRADE_CONFIRM_OK )
	{
		CItemContainer<FLItemElem> tempContainer;
		tempContainer.SetItemContainer( ITYPE_ITEM, CONTAINER_TYPE_TRADE, MAX_TRADE );

		// gold_step4. ���� ���Ѵ�.
		m_pOwner->AddGold( nThisAdd, FALSE );
		pTrader->AddGold( nTraderAdd, FALSE );
		// TradeClear���� ������ �ȵǱ� ���ؼ� 
		pTrader->m_vtInfo.TradeSetGold( 0 );
		TradeSetGold( 0 );

		//////////////////////////////////////////////////////////////////////////
		// �� ������
		ownerar << m_pOwner->GetGold();
		traderar << pTrader->GetGold();

//  	logar.WriteString( "T" );
//  	logar.WriteString( pTrader->GetName() );
//  	logar.WriteString( m_pOwner->GetName() );
// 
// 		//////////////////////////////////////////////////////////////////////////
// 		// mirchang_101011 LOG_ITEM_STR Send, Recv PlayerID �߰�
// #ifdef __WORLDSERVER
// 		u_long idSendPlayer, idRecvPlayer;
// 		idSendPlayer = idRecvPlayer = 0;
// 
// 		TCHAR szSendName[MAX_NAME] = { 0, };
// 		TCHAR szRecvName[MAX_NAME] = { 0, };
// 
// 		FLStrcpy( szSendName, _countof( szSendName ), pTrader->GetName() );
// 		FLStrcpy( szRecvName, _countof( szRecvName ), m_pOwner->GetName() );
// 
// 		idSendPlayer = CPlayerDataCenter::GetInstance()->GetPlayerId( szSendName );
// 		idRecvPlayer = CPlayerDataCenter::GetInstance()->GetPlayerId( szRecvName );
// 
// 		logar << idSendPlayer;
// 		logar << idRecvPlayer;
// #endif // __WORLDSERVER
// 		//////////////////////////////////////////////////////////////////////////
// 
// 		logar << m_pOwner->GetWorld()->GetID();
// 		logar << nTraderGold << nUserGold;
// 		logar << pTrader->GetGold() << m_pOwner->GetGold();
// 		logar << m_pOwner->m_idPlayer << m_pOwner->GetLevel() << m_pOwner->GetJob();
// 		logar << pTrader->m_idPlayer << pTrader->GetLevel() << pTrader->GetJob();
// #ifdef __WORLDSERVER
// 		logar.WriteString( ( (FLWSUser*)m_pOwner )->m_playAccount.lpAddr );
// 		logar.WriteString( ( (FLWSUser*)pTrader )->m_playAccount.lpAddr );
// #endif	// __WORLDSERVER
// 
// 		u_long uSize1	= 0;
// 		ptrdiff_t nSizeTag1 = logar.ReserveSpace( sizeof( uSize1 ) );
// 		u_long uSize2	= 0;
// 		ptrdiff_t nSizeTag2 = logar.ReserveSpace( sizeof( uSize1 ) );

		//////////////////////////////////////////////////////////////////////////
		// ������ ������ ����
		int nUserCount = 0;
		ptrdiff_t nUserTag = ownerar.ReserveSpace( sizeof( nUserCount ) );

		int nTraderCount = 0;
		ptrdiff_t nTraderTag = traderar.ReserveSpace( sizeof( nTraderCount ) );

		//////////////////////////////////////////////////////////////////////////
		// item_step1. m_pOwner->�ӽ� 
		FLItemBase* pItemBase;
		for( int i = 0; i < MAX_TRADE; i++ )
		{
			pItemBase = m_apItem_VT[i];
			if( pItemBase == NULL )
				continue;

			FLItemElem* pItemElem = ( FLItemElem* )pItemBase;
	
			if ( pItemElem->GetProp() == NULL )
				continue;

			// ī��Ʈ ����
			++nUserCount;

			m_apItem_VT[i] = NULL;
			

			if( pItemElem->GetProp()->dwPackMax > 1 )
			{
				if( pItemBase->GetExtra() < 1 )
				{
					pItemBase->SetExtra( 1 );
				}
				if( pItemElem->m_nItemNum < pItemBase->GetExtra() )
				{
					pItemBase->SetExtra( pItemElem->m_nItemNum );
				}
			}
			else
			{
				pItemBase->SetExtra( pItemElem->m_nItemNum );
			}

			FLItemElem itemElem;
			itemElem	= *pItemElem;
			itemElem.m_nItemNum		= pItemBase->GetExtra();

			// �Ϻθ� ���� ���� ���̶�� 
// 			if( itemElem.m_nItemNum < pItemElem->m_nItemNum )
// 			{
// 				itemElem.SetSerialNumber();
// 			}

			pItemElem->m_nItemNum	= pItemElem->m_nItemNum - itemElem.m_nItemNum;
			pItemElem->SetExtra( 0 );

			// Owner ������ ����
			ownerar << pItemElem->m_dwObjId << pItemElem->m_nItemNum;

			if( pItemElem->m_nItemNum <= 0 )
			{
				m_pOwner->m_Inventory.RemoveAtId( pItemBase->m_dwObjId );	// ����
			}

			tempContainer.Add( &itemElem );
		}
		//////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////
		// Owner ������ �߰�

		//////////////////////////////////////////////////////////////////////////
		// trader -> owner
#ifdef __WORLDSERVER
		T_LOG_TRADE kLogTradeTrader;

		FLStrcpy( kLogTradeTrader.szPlayerName, _countof( kLogTradeTrader.szPlayerName ), pTrader->GetName() );

		kLogTradeTrader.idPlayerID			= pTrader->m_idPlayer;
		kLogTradeTrader.dwWorldID			= pTrader->GetWorld()->GetID();
		kLogTradeTrader.nTradeGold			= nTraderGold;
		kLogTradeTrader.nHaveGold			= pTrader->GetGold();
		kLogTradeTrader.nPlayerLevel		= pTrader->GetLevel();
		kLogTradeTrader.nPlayerJob			= pTrader->GetJob();

		FLStrcpy( kLogTradeTrader.szIP, _countof( kLogTradeTrader.szIP ), ( static_cast<FLWSUser*>(pTrader) )->m_playAccount.lpAddr );

		kLogTradeTrader.dwTradeItemCount	= 0;

		std::vector< T_LOG_ITEM >	vecLogItemTrader;
		vecLogItemTrader.clear();
#endif // __WORLDSERVER
		//////////////////////////////////////////////////////////////////////////

		ownerar.WriteReservedSpace( nUserTag, &nUserCount, sizeof( nUserCount ) );

		nUserCount = 0;
		nUserTag = ownerar.ReserveSpace( sizeof( nUserCount ) );
		
		// item_step2. pTrader -> m_pOwner
		for( BYTE i = 0; i < MAX_TRADE; i++ )
		{
			pItemBase = pTrader->m_vtInfo.GetItem( i );
			if( pItemBase == NULL )
				continue;

			FLItemElem* pItemElem = ( FLItemElem* )pItemBase;
			if( pItemElem->GetProp() == NULL )
				continue;


			pTrader->m_vtInfo.SetItem( i, NULL );

#ifdef __WORLDSERVER
			++kLogTradeTrader.dwTradeItemCount;
#endif // __WORLDSERVER

// 			uSize1++;
// 			logar << pItemBase->m_dwItemId;
// 			logar << pItemBase->GetSerialNumber();
// 			//ar.WriteString( pItemBase->GetProp()->szName );
// 			char szItemId[32] = { 0, };
// 			FLSPrintf( szItemId, _countof( szItemId ), "%d", pItemBase->GetProp()->dwID );
// 			logar.WriteString( szItemId );

			// ī��Ʈ ����
			++nUserCount;
			++nTraderCount;

			


			if( pItemElem->GetProp() && pItemElem->GetProp()->dwPackMax > 1 )
			{
				if( pItemBase->GetExtra() < 1 )
				{
					pItemBase->SetExtra( 1 );
				}
				if( pItemElem->m_nItemNum < pItemBase->GetExtra() )
				{
					pItemBase->SetExtra( pItemElem->m_nItemNum );
				}
			}
			else
			{
				pItemBase->SetExtra( pItemElem->m_nItemNum );
			}

#ifdef __WORLDSERVER
			T_LOG_ITEM	kLogItemTrader;
			kLogItemTrader.CopyItemInfo( *pItemElem );
			vecLogItemTrader.push_back( kLogItemTrader );
#endif // __WORLDSERVER

			FLItemElem itemElem;
			itemElem	= *pItemElem;
			itemElem.m_nItemNum		= pItemBase->GetExtra();

			// �Ϻθ� ���� ���� ���̶�� 
// 			if( itemElem.m_nItemNum < pItemElem->m_nItemNum )
// 			{
// 				itemElem.SetSerialNumber();
// 			}

			pItemElem->m_nItemNum	= pItemElem->m_nItemNum - itemElem.m_nItemNum;
			pItemElem->SetExtra( 0 );

			// Ʈ���̴� ������ ����
			traderar << pItemElem->m_dwObjId << pItemElem->m_nItemNum;

			if( pItemElem->m_nItemNum <= 0 )
			{
				pTrader->m_Inventory.RemoveAtId( pItemBase->m_dwObjId );
			}

			m_pOwner->m_Inventory.Add( &itemElem );

			// Owner ������ ����
			itemElem.Serialize( ownerar );
		}

		// Owner ���� ������ ����
		ownerar.WriteReservedSpace( nUserTag, &nUserCount, sizeof( nUserCount ) );
		nUserCount = 0;
		// Owner ��

		// Ʈ���̴� ���� ������ ����
		traderar.WriteReservedSpace( nTraderTag, &nTraderCount, sizeof( nTraderCount ) );
		nTraderCount = 0;

		nTraderTag = traderar.ReserveSpace( sizeof( nTraderCount ) );


		//////////////////////////////////////////////////////////////////////////
		// item_step3. �ӽ� -> pTrader

		//////////////////////////////////////////////////////////////////////////
		// owner -> trader
#ifdef __WORLDSERVER
		T_LOG_TRADE kLogTradeOwner;

		FLStrcpy( kLogTradeOwner.szPlayerName, _countof( kLogTradeOwner.szPlayerName ), m_pOwner->GetName() );

		kLogTradeOwner.idPlayerID			= m_pOwner->m_idPlayer;
		kLogTradeOwner.dwWorldID			= m_pOwner->GetWorld()->GetID();
		kLogTradeOwner.nTradeGold			= nUserGold;
		kLogTradeOwner.nHaveGold			= m_pOwner->GetGold();
		kLogTradeOwner.nPlayerLevel		= m_pOwner->GetLevel();
		kLogTradeOwner.nPlayerJob			= m_pOwner->GetJob();

		FLStrcpy( kLogTradeOwner.szIP, _countof( kLogTradeOwner.szIP ), ( static_cast<FLWSUser*>(m_pOwner) )->m_playAccount.lpAddr );

		kLogTradeOwner.dwTradeItemCount	= 0;

		std::vector< T_LOG_ITEM >	vecLogItemOwner;
		vecLogItemOwner.clear();
#endif // __WORLDSERVER
		//////////////////////////////////////////////////////////////////////////

		for( DWORD dwObjId = 0; dwObjId < tempContainer.GetMax(); ++dwObjId )
		{
			FLItemElem* pItemElem = tempContainer.GetAtId( dwObjId );

			if( pItemElem != NULL && pItemElem->GetProp() != NULL )
			{
#ifdef __WORLDSERVER
				++kLogTradeOwner.dwTradeItemCount;

				T_LOG_ITEM	kLogItemOwner;
				kLogItemOwner.CopyItemInfo( *pItemElem );
				vecLogItemOwner.push_back( kLogItemOwner );
#endif // __WORLDSERVER

// 				uSize2++;
// 				logar << pItemElem->m_dwItemId;
// 				logar << pItemElem->GetSerialNumber();
// 				//ar.WriteString( pItemBase->GetProp()->szName );
// 				char szItemId[32] = {0, };
// 				FLSPrintf( szItemId, _countof( szItemId ), "%d", pItemElem->GetProp()->dwID );
// 				logar.WriteString( szItemId );
// 
				pTrader->m_Inventory.Add( pItemElem );

				// Ʈ���̴� ������ ����
				++nTraderCount;
				pItemElem->Serialize( traderar );

				tempContainer.RemoveAtId( pItemElem->m_dwObjId );
			}
		}

		// Ʈ���̴� ���� ������ ����
		traderar.WriteReservedSpace( nTraderTag, &nTraderCount, sizeof( nTraderCount ) );
		nTraderCount = 0;
		// Ʈ���̴� ��

// 		logar.WriteReservedSpace( nSizeTag1, &uSize1, sizeof( uSize1 ) );
// 		logar.WriteReservedSpace( nSizeTag2, &uSize2, sizeof( uSize2 ) );

#ifdef __WORLDSERVER
		//////////////////////////////////////////////////////////////////////////
		if( kLogTradeOwner.dwTradeItemCount > 0 || vecLogItemOwner.empty() == false )
		{
			const DWORD dwOwnerLogItemSize	= vecLogItemOwner.size();
			if( kLogTradeOwner.dwTradeItemCount != dwOwnerLogItemSize )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "[ OWNER TRADE COUNT INVALID. Count(%d), Size(%d) ]" )
					, kLogTradeOwner.dwTradeItemCount, dwOwnerLogItemSize );

				kLogTradeOwner.dwTradeItemCount	= dwOwnerLogItemSize;
			}
		}

		if( kLogTradeTrader.dwTradeItemCount > 0 || vecLogItemTrader.empty() == false )
		{
			const DWORD dwTraderLogItemSize	= vecLogItemTrader.size();
			if( kLogTradeTrader.dwTradeItemCount != dwTraderLogItemSize )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "[ TRADER TRADE COUNT INVALID. Count(%d), Size(%d) ]" )
					, kLogTradeTrader.dwTradeItemCount, dwTraderLogItemSize );

				kLogTradeTrader.dwTradeItemCount	= dwTraderLogItemSize;
			}
		}

		kLogTradeOwner.Serialize( logar );

		kLogTradeTrader.Serialize( logar );

		for( std::vector< T_LOG_ITEM >::iterator Itr = vecLogItemOwner.begin(); Itr != vecLogItemOwner.end(); ++Itr )
		{
			Itr->Serialize( logar );
		}

		for( std::vector< T_LOG_ITEM >::iterator Itr = vecLogItemTrader.begin(); Itr != vecLogItemTrader.end(); ++Itr )
		{
			Itr->Serialize( logar );
		}
		//////////////////////////////////////////////////////////////////////////
#endif // __WORLDSERVER
	}

	TradeClear();
	pTrader->m_vtInfo.TradeClear();
	return result;
}

TRADE_STATE CVTInfo::TradeGetState()
{
	return m_state;
}

void CVTInfo::TradeSetState( TRADE_STATE state )
{
	m_state = state;
}

///////////////////////////////////////////////////////////////////////////////
// ���λ��� 
///////////////////////////////////////////////////////////////////////////////

//void CDPClient::OnUnregisterPVendorItem( OBJID objid, CAr & ar )
BOOL CVTInfo::VendorClearItem( BYTE byNth )
{
	FLItemBase* pItemBase = m_apItem_VT[byNth];
	if( pItemBase )
	{
		pItemBase->SetExtra( 0 );
		pItemBase->m_nCost = 0;
		m_apItem_VT[byNth]     = NULL;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// void CDPClient::OnRegisterPVendorItem( OBJID objid, CAr & ar )
void CVTInfo::VendorSetItem( DWORD dwItemObjID, BYTE byNth, int nNum, int nCost )
{
	FLItemBase* pItemBase = m_pOwner->GetItemId( dwItemObjID );
	if( pItemBase )
	{
		m_apItem_VT[byNth] = pItemBase;
		pItemBase->SetExtra( nNum );
		pItemBase->m_nCost = nCost;
	}
}

//void CDPClient::OnPVendorItemNum( OBJID objid, CAr & ar )
// nNum - ���� ���� 
void CVTInfo::VendorItemNum( BYTE byNth, int nNum )
{
	FLItemBase* pItemBase = m_apItem_VT[byNth];
	if( pItemBase )
	{
		pItemBase->SetExtra( nNum );
		if( nNum == 0 )
		{
		#ifdef __CLIENT
			if( m_pOwner->IsActiveMover() == FALSE )
				SAFE_DELETE( m_apItem_VT[byNth] );
		#endif
			m_apItem_VT[byNth] = NULL;
		}
	}
}

// ����Ÿ ī�Ǹ� �ؼ� ����?
// void CDPClient::OnPVendorItem( OBJID objid, CAr & ar )
void CVTInfo::VendorCopyItems( FLItemBase** ppItemVd )
{
	memcpy( (void*)m_apItem_VT, ppItemVd, sizeof(m_apItem_VT) );
}


void CVTInfo::VendorClose( BOOL bClearTitle )
{
	for( int i = 0; i < MAX_VENDITEM; i++ )
	{
		if( m_apItem_VT[i] )
		{
			m_apItem_VT[i]->SetExtra( 0 );
			m_apItem_VT[i]->m_nCost	= 0;
		#ifdef __CLIENT
			if( FALSE == m_pOwner->IsActiveMover() )
				SAFE_DELETE( m_apItem_VT[i] );
		#endif	// __CLIENT
			m_apItem_VT[i]	= NULL;
		}
	}

	if( bClearTitle )
		m_strTitle = "";

	SetOther( NULL );	
}


// ���� �Ǹ��� �ΰ�? 
BOOL CVTInfo::VendorIsVendor() 
{
	for( int i=0; i<MAX_VENDITEM; ++i )
	{
		if( m_apItem_VT[i] )	// ����� �������� �ִ°�?
			return TRUE;
	}

	return FALSE;
}

BOOL CVTInfo::IsTrading( FLItemElem* pItemElem )
{
	for( int i = 0; i < MAX_VENDITEM; i++ )
	{
		if( m_apItem_VT[i] == pItemElem )
			return TRUE;
	}
	return FALSE;
}

//CDPSrvr::OnBuyPVendorItem
#ifdef __WORLDSERVER
BOOL CVTInfo::VendorSellItem( CMover* pBuyer, BYTE byNth, DWORD dwItemId, int nNum, VENDOR_SELL_RESULT& result )
{
	result.nRemain    = 0;
	result.nErrorCode = 0;

	if( IsVendorOpen() == FALSE )
		return FALSE;

	FLItemBase* pItemBase = m_apItem_VT[byNth];
	if( IsUsingItem( pItemBase ) == FALSE || pItemBase->m_dwItemId != dwItemId )
		return FALSE;

	if( nNum < 1 )
		nNum = 1;
	if( nNum > pItemBase->GetExtra() )
		nNum = pItemBase->GetExtra();

//	06.10.26
//	if( pItemBase->m_nCost > 0 && (float)pBuyer->GetGold() < (float)nNum * (float)pItemBase->m_nCost )

	//if( pItemBase->m_nCost > 0 &&  pBuyer->CheckUserGold( nNum * pItemBase->m_nCost, false ) == false )		
	//->
	const __int64 nnPenya	= ( __int64 )( (float)nNum * (float)pItemBase->m_nCost );
	if( nnPenya < 0 || nnPenya > INT_MAX )
	{
		result.nErrorCode = TID_GAME_LACKMONEY;
		return FALSE;
	}
	const int nPenya		= ( int ) nnPenya;

	if( pItemBase->m_nCost > 0 &&  pBuyer->CheckUserGold( nPenya, false ) == false )
	{
//sun: 8, // __S8_VENDOR_REVISION
		result.nErrorCode = TID_GAME_LACKMONEY;
		return FALSE;

//!		nNum = (short)( pBuyer->GetGold() / pItemBase->m_nCost );

	}
	
	if( nNum == 0 )
	{
		result.nErrorCode = TID_GAME_LACKMONEY;
		return FALSE;
	}

	FLItemElem* pItemElem = (FLItemElem*)pItemBase;
	FLItemElem itemElem;
	itemElem	= *pItemElem;
	itemElem.m_nItemNum	 = nNum;
//	itemElem.SetSerialNumber();

	if( pBuyer->CreateItem( &itemElem ) == FALSE )
	{
		result.nErrorCode = TID_GAME_LACKSPACE;
		return FALSE;
	}

	// FLItemElem�� ���� �����ڿ� m_nCost�� ���ܵǾ� �ִ�.
	int nCost	= pItemBase->m_nCost;
	pBuyer->AddGold( -( pItemBase->m_nCost * nNum ) );
	m_pOwner->AddGold( pItemBase->m_nCost * nNum );

	pItemBase->SetExtra( pItemBase->GetExtra() - nNum );
	int nRemain = pItemBase->GetExtra();
	if( nRemain <= 0 )
		m_apItem_VT[byNth] = NULL;

//sun: 11, ���λ��� ����
#ifdef __WORLDSERVER
	g_xWSUserManager->AddPVendorItemNum( (FLWSUser*)m_pOwner, byNth, nRemain, pBuyer->GetName() );
#endif	// __WORLDSERVER

	m_pOwner->RemoveItem( pItemBase->m_dwObjId, nNum );

	result.item = itemElem;
	result.item.m_nCost	= nCost;
	result.nRemain = nRemain;
	return TRUE;
}
#endif // __WORLDSERVER