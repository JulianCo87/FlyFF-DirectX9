#include "stdafx.h"
#include "CreateMonster.h"
#include "../WorldServer/user.h"


CCreateMonster::CCreateMonster()
:
m_nMaxCreateNum( 0 )
{

}

CCreateMonster::~CCreateMonster()
{
	for( MAPPROP::iterator it=m_mapCreateMonsterProp.begin(); it!=m_mapCreateMonsterProp.end(); it++ )
	{
		it->second->mapMonster.clear();
		SAFE_DELETE( it->second );
	}
	m_mapCreateMonsterProp.clear();
	m_mapCreateMonsterInfo.clear();
}

CCreateMonster* CCreateMonster::GetInstance( void )
{
	static CCreateMonster sCreateMonster;
	return &sCreateMonster;
}

void CCreateMonster::LoadScript()
{
	CLuaBase lua;

	const TCHAR szFileName[]	= _T( "CreateMonster.lua" );

	TCHAR szFullPath[ MAX_PATH ]	= { 0, };
	g_pScriptFileManager->GetScriptFileFullPath( szFullPath, _countof( szFullPath ), szFileName );

	if( lua.RunScript( szFullPath ) != 0 )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "[ FAILED LOAD SCRIPT. FILE_NAME:(%s) ]" ), szFullPath );
		exit(0);
	}

	m_nMaxCreateNum = static_cast<int>( lua.GetGlobalNumber( "m_nMaxCreateNum" ) );

	lua.GetGloabal( "tCreateMonster" );
	lua.PushNil();
	while( lua.TableLoop( -2 ) )
	{
		DWORD dwItemId = static_cast<DWORD>( CScript::GetDefineNum( lua.GetFieldToString( -1, "strItemId" ) ) );
		CREATE_MONSTER_PROP* pCreateMonsterProp = new CREATE_MONSTER_PROP;
		bool bResult = m_mapCreateMonsterProp.insert( make_pair( dwItemId, pCreateMonsterProp ) ).second;
		if( bResult == false )
		{
			SAFE_DELETE( pCreateMonsterProp );
		}
		else
		{
			pCreateMonsterProp->dwKeepTime = static_cast<DWORD>( lua.GetFieldToNumber( -1, "dwKeepTime" ) );
		}

		//lua_getfield( lua.GetLuaState(), -1, "tMonster" );
		lua.GetField( -1, "tMonster" );
		lua.PushNil();
		while( lua.TableLoop( -2 ) )
		{
			DWORD dwMonsterId = static_cast<DWORD>( CScript::GetDefineNum( lua.GetFieldToString( -1, "strMonsterId" ) ) );
			int nProbability = static_cast<int>( lua.GetFieldToNumber( -1, "nProbability" ) );
			if( pCreateMonsterProp != NULL )
			{
				pCreateMonsterProp->mapMonster.insert( make_pair( dwMonsterId, nProbability ) );
			}
			lua.Pop( 1 );
		}
		lua.Pop( 2 );
	}

	lua.Pop( 0 );	
}

void CCreateMonster::SetState( OBJID objId, const char chState )
{
	MAPINFO::iterator it = m_mapCreateMonsterInfo.find( objId );
	if( it == m_mapCreateMonsterInfo.end() )
		return;

	it->second.chState = chState;
}

CREATE_MONSTER_PROP* CCreateMonster::GetCreateMonsterProp( FLItemElem* pItemElem )
{
	MAPPROP::iterator it = m_mapCreateMonsterProp.find( pItemElem->m_dwItemId );
	if( it == m_mapCreateMonsterProp.end() )
        return NULL;

	return it->second;
}

void CCreateMonster::ProcessRemoveMonster()
{
	if( m_mapCreateMonsterInfo.empty() )
		return;

	DWORD dwTick = GetTickCount();

	for( MAPINFO::iterator it=m_mapCreateMonsterInfo.begin(); it!=m_mapCreateMonsterInfo.end(); it++ )
	{
		CREATE_MONSTER_INFO* pCreateMonsterInfo = &it->second;
		if( dwTick >= pCreateMonsterInfo->dwEndTick )
		{
			CMover* pMover = prj.GetMover( it->first );
			if( IsValidObj( pMover ) )
			{
				if( pMover->IsLive() && !pMover->IsDelete() )
				{
					pCreateMonsterInfo->chState = 'R';
					pMover->Delete();
				}
			}
		}
	}
}

BOOL CCreateMonster::IsAttackAble( FLWSUser* pUser, CMover* pTarget, BOOL bTextOut )
{
	if( !IsValidObj( pUser ) || !IsValidObj( pTarget ) )
		return FALSE;
	
	// �����ۿ� ���� ��ȯ�� ���Ͱ� �ƴϸ� ���ݰ���
	MAPINFO::iterator it = m_mapCreateMonsterInfo.find( pTarget->GetId() );
	if( it == m_mapCreateMonsterInfo.end() )
		return TRUE;

	// ��ȯ ��ü�� �������� �ƴϸ� �ƹ��� ���ݸ��Ѵ�.
	FLWSUser* pOwner = static_cast<FLWSUser*>( prj.GetUserByID( it->second.dwOwnerId ) );
	// �����ڰ� ��ȯ ��ü�̰ų� ���� ��Ƽ���� ��츸 ���ݰ���...
	if( IsValidObj( pOwner )
		&& ( ( pOwner == pUser ) || ( pOwner->GetPartyId() && pOwner->GetPartyId() == pUser->GetPartyId() ) ) )
		return TRUE;

	if( bTextOut )
		pUser->AddDefinedText( TID_GAME_PRIORITYSUMMONMOB, "\"%s\"", CPlayerDataCenter::GetInstance()->GetPlayerString( it->second.dwOwnerId ) );

	return FALSE;
}

void CCreateMonster::CreateMonster( FLWSUser* pUser, DWORD dwObjId, D3DXVECTOR3 vPos )
{
	if( (int)( m_mapCreateMonsterInfo.size() ) >= m_nMaxCreateNum )
	{
		pUser->AddDefinedText( TID_GAME_CREATEMON_LIMIT );
		return;
	}
	
	FLItemElem* pItemElem = pUser->m_Inventory.GetAtId( dwObjId );
	if( !IsUsableItem( pItemElem ) )
		return;

	CREATE_MONSTER_PROP* pCreateMonsterProp = GetCreateMonsterProp( pItemElem );
	if( !pCreateMonsterProp )
		return;

	DWORD dwMonsterId = pCreateMonsterProp->GetRandomMonsterId();
	if( dwMonsterId == NULL_ID )
		return;

	CWorld* pWorld = pUser->GetWorld();
	MoverProp* pMoverProp = prj.GetMoverProp( dwMonsterId );

	if( pWorld && pMoverProp && pMoverProp->dwID != 0 )
	{
		D3DXVECTOR3 vDist2 = pUser->GetPos() - vPos;
		float fDist = D3DXVec3Length( &vDist2 );			// ����ǥ���� �Ÿ�
		if( 15.f < fDist )
		{
			pUser->AddDefinedText( TID_GAME_CREATEMON_F_15 );
			return;
		}

		int nAttr = pWorld->GetHeightAttribute( vPos.x, vPos.z );		// �̵��� ��ġ�� �Ӽ� ����.
		if( nAttr == HATTR_NOWALK || nAttr == HATTR_NOMOVE )		// �� �����̴� ���̸� Pass
		{
			pUser->AddDefinedText( TID_GAME_CREATEMON_F_AREA );
			return;
		}
		if( pUser->IsRegionAttr( RA_SAFETY ))		// ���������̸� Pass
		{
			pUser->AddDefinedText( TID_GAME_CREATEMON_F_AREA );
			return;
		}
		if( pWorld->GetID() != WI_WORLD_MADRIGAL )
		{
			pUser->AddDefinedText( TID_GAME_CREATEMON_F_AREA );
			return;
		}

		CObj* pObj	= CreateObj( D3DDEVICE, OT_MOVER, pMoverProp->dwID );
		if( NULL == pObj )	return;	// FLASSERT( pObj );
		pObj->SetPos( vPos );
		pObj->InitMotion( MTI_STAND );
		pObj->UpdateLocalMatrix();
		((CMover*)pObj)->m_bActiveAttack = FALSE;
		((CMover*)pObj)->AddItToGlobalId();
		
		if( pWorld->ADDOBJ( pObj, FALSE, pUser->GetLayer() ) == TRUE )
		{
			LogItemInfo aLogItem;
			//aLogItem.Action = "i";
			//aLogItem.SendName = pUser->GetName();
			//char szTemp[128] = {0,};
			//FLSPrintf( szTemp, _countof( szTemp ), "M_C_%s", pMoverProp->szName ); 
			//mem_set( szTemp+31, 0, sizeof(szTemp)-31 );
			//aLogItem.RecvName = szTemp;

			FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "i" );
			FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
			char szTemp[128] = {0,};
			FLSPrintf( szTemp, _countof( szTemp ), "M_C_%s", pMoverProp->szName ); 
			memset( szTemp+31, 0, sizeof(szTemp)-31 );
			FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), szTemp );


			aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
			aLogItem.Gold_1 = ((CMover*)pObj)->GetId();
			aLogItem.WorldId = pUser->GetWorld()->GetID();
			g_DPSrvr.OnLogItem( aLogItem, pItemElem, 1 );

			pUser->AddDefinedText( TID_GAME_CREATEMON_S, "\"%s\"", pMoverProp->szName );
			pUser->RemoveItem( dwObjId, 1 );

			CREATE_MONSTER_INFO createMonsterInfo;
			createMonsterInfo.chState = 'N';
			createMonsterInfo.dwOwnerId = pUser->m_idPlayer;
			createMonsterInfo.dwEndTick = pCreateMonsterProp->dwKeepTime + GetTickCount();
			m_mapCreateMonsterInfo.insert( make_pair( ((CMover*)pObj)->GetId(), createMonsterInfo ) );
		}
		else
		{
			SAFE_DELETE( pObj );
		}
	}
}

void CCreateMonster::RemoveInfo( CMover* pMover )
{
	MAPINFO::iterator it=m_mapCreateMonsterInfo.find( pMover->GetId() );
	if( it == m_mapCreateMonsterInfo.end() )
		return;
	
	CREATE_MONSTER_INFO createMonsterInfo = it->second;
	if( pMover )
	{
		LogItemInfo aLogItem;
		//aLogItem.Action = "i";
		//aLogItem.SendName = CPlayerDataCenter::GetInstance()->GetPlayerString( createMonsterInfo.dwOwnerId );
		//char szTemp[128] = {0,};
		//FLSPrintf( szTemp, _countof( szTemp ), "M_%c_%s", createMonsterInfo.chState, pMover->GetName() ); 
		//memset( szTemp+31, 0, sizeof(szTemp)-31 );
		//aLogItem.RecvName = szTemp;

		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "i" );
		FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), CPlayerDataCenter::GetInstance()->GetPlayerString( createMonsterInfo.dwOwnerId ) );
		char szTemp[128] = {0,};
		FLSPrintf( szTemp, _countof( szTemp ), "M_%c_%s", createMonsterInfo.chState, pMover->GetName() ); 
		memset( szTemp+31, 0, sizeof(szTemp)-31 );
		FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), szTemp );

		aLogItem.Gold = aLogItem.Gold2 = 0;
		aLogItem.Gold_1 = pMover->GetId();
		aLogItem.WorldId = pMover->GetWorld()->GetID();
		g_DPSrvr.OnLogItem( aLogItem );
	}

	m_mapCreateMonsterInfo.erase( it );
}



