// FunnyCoin.cpp: implementation of the CFunnyCoin class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FunnyCoin.h"

const int FC_DBSUCCESS = 1;
const int FC_DBFAILED = -1;
const int FC_SAMEUNIQUENO = -2;

#ifdef __WORLDSERVER
//////////////////////////////////////////////////////////////////////
// CFunnyCoin ////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#include "../WorldServer/User.h"

CFunnyCoin::CFunnyCoin()
{
	m_vecAckWaitPlayer.clear();
}

CFunnyCoin::~CFunnyCoin()
{
	for( size_t i=0; i<m_vecAckWaitPlayer.size(); i++ )
		FLERROR_LOG( PROGRAM_NAME, _T( "FunnyCoin AckWait Player - %07d" ), m_vecAckWaitPlayer.at( i ) );
	
	m_vecAckWaitPlayer.clear();
}

CFunnyCoin* CFunnyCoin::GetInstance()
{
	static CFunnyCoin sFunnyCoin;
	return & sFunnyCoin;
}

BOOL CFunnyCoin::DoUseFunnyCoin( FLWSUser* pUser, FLItemElem* pItemElem )	// �۴������� ����Ѵ�.
{
	if( g_xFlyffConfig->GetMainLanguage() != LANG_KOR )
	{
		return FALSE;
	}

	if ( !IsUsable( pUser ) )
		return FALSE;
	
	m_vecAckWaitPlayer.push_back( pUser->m_idPlayer );
	g_dpDBClient.SendFunnyCoinReqUse( pUser->m_idPlayer, pItemElem );	// Trans Server �� ����
	g_DPSrvr.PutItemLog( pUser, "M", "FUNNYCOIN_USE", pItemElem );
	
	return TRUE;
}

void CFunnyCoin::OnFunnyCoinAckUse( CAr & ar )	// �۴����� ��뿡 ���� ������ TranServer�� ���� �޾Ҵ�.
{
	DWORD dwPlayerId = 0, dwItemId = 0;
	SERIALNUMBER iSerialNumber = 0;
	int nResult = 0;

	ar >> dwPlayerId >> dwItemId >> iSerialNumber >> nResult;

	if( g_xFlyffConfig->GetMainLanguage() != LANG_KOR )
	{
		return;
	}

	if( IsAckWaitPlayer( dwPlayerId, TRUE ) )
	{
		FLWSUser* pUser = static_cast<FLWSUser*>( prj.GetUserByID( dwPlayerId ) );	
		switch( nResult )
		{
		case FC_DBSUCCESS:
			{
				PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( dwItemId );
				if( IsValidObj( pUser ) && pItemProp )
					pUser->AddDefinedText( TID_GAME_FUNNYCOIN_USECOMPLETE, "\"%s\"", pItemProp->szName );
			}
			break;

		case FC_DBFAILED:
			{
				if( IsValidObj( pUser ) )
				{
					FLItemElem itemElem;
					itemElem.m_dwItemId = dwItemId;
					itemElem.m_nItemNum = 1;
					itemElem.SetSerialNumber( iSerialNumber );		// ���� �ø��� �ѹ��� �����Ѵ�.
					if( pUser->CreateItem( &itemElem ) )	
						g_DPSrvr.PutItemLog( pUser, "M", "FUNNYCOIN_REPAIR_SUCCESS", &itemElem );
					else	// �κ��丮�� �������� ���� ������ ������ ���� ����!!
					{
						g_DPSrvr.PutItemLog( pUser, "M", "FUNNYCOIN_REPAIR_FAILED", &itemElem );
						FLERROR_LOG( PROGRAM_NAME, _T( "FunnyCoin Repair Failed! [PlayerId:%07d], [Item:%d], [SerialNumber:%d]" ),
								dwPlayerId, dwItemId, iSerialNumber );
					}
				}
				else	// �ش� �÷��̾ ���� ������ �ʴ�.
				{
					FLERROR_LOG( PROGRAM_NAME, _T( "FunnyCoin Repair Failed!(Invalid User) [PlayerId:%07d], [Item:%d], [SerialNumber:%d]" ),
							dwPlayerId, dwItemId, iSerialNumber );
				}
			}
			break;

		case FC_SAMEUNIQUENO:
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "Same SerialNumber! [PlayerId:%07d], [Item:%d], [SerialNumber:%d]" ),
						dwPlayerId, dwItemId, iSerialNumber );
			}
			break;
		}
	}
	else	// �۴����� ��� ���� ��� �÷��̾ �ƴϴ�.
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "Is Not AckWait Player [PlayerId:%07d]" ), dwPlayerId );
	}
}

BOOL CFunnyCoin::IsAckWaitPlayer( DWORD dwPlayerId, BOOL bRemove )	// �۴������� ����� �� DB������ ��ٸ��� �ִ� �÷��̾� �ΰ�?
{																	// bRemove�� TRUE�̸� �ش� ����ڸ� �����Ѵ�.
	for( vector<DWORD>::iterator it=m_vecAckWaitPlayer.begin(); it!=m_vecAckWaitPlayer.end(); it++ )
	{
		if( (*it) == dwPlayerId )
		{
			if( bRemove )	
				m_vecAckWaitPlayer.erase( it );

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CFunnyCoin::IsUsable( FLWSUser* pUser )
{
	switch( g_xFlyffConfig->GetMainLanguage() )
	{
		case LANG_KOR :	
			{
				CString strTemp;
				strTemp.Format( "%s", pUser->m_playAccount.lpszAccount );
				if( strTemp.Right( 4 ) == "__bu" )
				{
					pUser->AddDefinedText( TID_GAME_FUNNYCOIN_NOUSEBUDDYPLAYER );
					return FALSE;
				}

				if( IsAckWaitPlayer( pUser->m_idPlayer ) )
				{
					//pUser->AddText( "����Ŀ� �ٽ� ����ϰԳ�..." );
					return FALSE;
				}

				return TRUE;
			}
	}
	
	return FALSE;
}
#endif // __WORLDSERVER

#ifdef __DBSERVER
//////////////////////////////////////////////////////////////////////
// CFunnyCoinDbCtrl //////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#include "../DatabaseServer/dptrans.h"
extern AppInfo g_appInfo;

CFunnyCoinDbCtrl::CFunnyCoinDbCtrl()
{
}

CFunnyCoinDbCtrl::~CFunnyCoinDbCtrl()
{
}

void CFunnyCoinDbCtrl::Handler( LPDB_OVERLAPPED_PLUS pov, ULONG_PTR dwCompletionKey )
{
	switch( pov->nQueryMode )
	{
	case FC_USE:
		{
			DWORD dwPlayerId = 0, dwItemId = 0;
			SERIALNUMBER iSerialNumber = 0;

			CAr ar( pov->lpBuf, pov->uBufSize );
			ar >> dwPlayerId >> dwItemId >> iSerialNumber;

			int nResult = InsertFunnyCoin( dwPlayerId, dwItemId, iSerialNumber );
			CDPTrans::GetInstance()->SendFunnyCoinAckUse( dwPlayerId, dwItemId, iSerialNumber, nResult, dwCompletionKey );
		}
		break;

	default:
		break;
	}
}

int CFunnyCoinDbCtrl::InsertFunnyCoin( DWORD dwPlayerId, DWORD dwItemId, SERIALNUMBER iSerialNumber )	// ����� �۴������� DB�� �����Ѵ�.
{
	if( g_xFlyffConfig->GetMainLanguage() != LANG_KOR )
	{
		return FC_DBFAILED;
	}

	PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( dwItemId );
	if( !pItemProp )
		return FC_DBFAILED;
	
	CQuery* pQuery = GetQueryObject();
	if( !pQuery->Execute( "usp_FunnyCoin_input '%02d', '%07d', %d, %d, %d", g_appInfo.dwSys, dwPlayerId, dwItemId, pItemProp->dwCost, iSerialNumber ) )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "Call \"usp_FunnyCoin_input\" FAILED! - %07d, %d" ), dwPlayerId, dwItemId );
		return FC_DBFAILED;
	}

	if( pQuery->Fetch() )
	{
		int nResult = pQuery->GetInt( "result" );
		if( nResult != FC_DBSUCCESS )	// ����chipi	
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "usp_FunnyCoin_input Result is Fail - ErrorCode : %d, %07d, %d, %d" ), nResult, dwPlayerId, dwItemId, iSerialNumber );
			return nResult;
		}
	}
	else
	{
		return FC_DBFAILED;
	}

	// ���� �ߴ�!!
	return FC_DBSUCCESS;
}

//////////////////////////////////////////////////////////////////////
// CFunnyCoin ////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
CFunnyCoinDbMng::CFunnyCoinDbMng()
{
	if( !m_FCDbCtrl.CreateDbHandler() )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "m_FCDbCtrl.CreateDbHandler()" ) );
	}
}

CFunnyCoinDbMng::~CFunnyCoinDbMng()
{
	m_FCDbCtrl.CloseDbHandler();
}

CFunnyCoinDbMng* CFunnyCoinDbMng::GetInstance()
{
	static CFunnyCoinDbMng sFunnyCoinDbMng;
	return &sFunnyCoinDbMng;
}
#endif // __DBSERVER
