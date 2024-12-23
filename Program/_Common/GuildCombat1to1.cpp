// GuildCombat1to1.cpp: implementation of the CGuildCombat1to1 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildCombat1to1.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//sun: 11, �ϴ��� ��� ����

#ifdef __WORLDSERVER
	#include "worldmng.h"
	#include "guild.h"
	#include "../WorldServer/user.h"	
	extern	CWorldMng	g_WorldMng;
	extern	CGuildMng	g_GuildMng;
	extern	CGuildCombat g_GuildCombatMng;
#endif // __WORLDSERVER
//////////////////////////////////////////////////////////////////////
//// CGuildCombat1To1 ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#ifdef __WORLDSERVER
// 1:1�������� ������ �� �����庰 ������ ����
CGuildCombat1to1::CGuildCombat1to1( vector<CGuildCombat1to1Mng::__GC1TO1TENDER>& vecTenderGuild, int nStageId )
{
	ClearTime();
	m_nState = GC1TO1WAR_CLOSE;
	m_nProgWarCount = m_nTotalWarCount = 0;
//	m_nRealWarCount = g_GuildCombat1to1Mng.m_nMaxJoinPlayer;
	m_vecTenderGuild = vecTenderGuild;

	int nIndex = 0;
	for( int i=0; i<(int)( m_vecTenderGuild.size() ); i++ )
	{
		if( m_vecTenderGuild.at( i ).nStageId == nStageId )
		{
			m_nIndex[nIndex] = i;
			m_vecTenderGuild.at( i ).nPosition = nIndex;
			m_vecTenderGuild.at( i ).nNowPlayerLife = g_GuildCombat1to1Mng.m_nPlayerLife;	// ����� �ʱ�ȭ
			nIndex = 1;
			if( (int)( m_vecTenderGuild.at( i ).vecMemberId.size() ) > m_nTotalWarCount )
				m_nTotalWarCount = m_vecTenderGuild.at( i ).vecMemberId.size();		
		}
	}
}

CGuildCombat1to1::~CGuildCombat1to1()
{

}

void	CGuildCombat1to1::ClearTime()
{
	m_nWaitTime = 0;
	m_dwTime = 0;
}

void	CGuildCombat1to1::Process()
{
	
	if( m_nWaitTime > 0 )
	{
		m_nWaitTime -= GetTickCount() - m_dwTime;
		m_dwTime = GetTickCount();	
	}
	
	switch( m_nState )
	{
		case GC1TO1WAR_CLOSE :	// 
			break;
		
		case GC1TO1WAR_WAIT :	// ���� ������ ���� ���� �����...
		{
			if( m_nWaitTime <=0 )
			{
				ClearTime();
				GuildCombat1to1War();
			}
			break;
		}

		case GC1TO1WAR_WAR :	// ���� ������ ���� ���� ��...
		{
			if( m_nWaitTime <= 0 )
			{
				ClearTime();
				GuildCombat1to1WarResultCheck();
			}
			break;
		}
		
		case GC1TO1WAR_FINISH :		// ���� ������ ���� ���� ����(��� ó��)
		{
			ClearTime();
			SendNowStateAllPlayer();
			SendWarResultAllPlayer();
			SetTeleportToWaitStageNowPlayers();
			m_nProgWarCount++;
			GuildCombat1to1WarWait();
			break;
		}

		case GC1TO1WAR_CLOSEWAIT:	// ���� �������� ������ ��� ����ǰ� ������ �ڷ���Ʈ �ϱ��� ��� �ð�..
		{
			if( m_nWaitTime <= 0 )
				GuildCombat1to1Close();
			break;
		}
	}
}

// ���� �����忡 �����ϴ� ��� �������� ���� ����
void	CGuildCombat1to1::SendNowStateAllPlayer()
{
	FLWSUser* pUser = NULL;
	for( int i=0; i<2; i++ )
	{
		for( int j=0; j<(int)( m_vecTenderGuild.at( m_nIndex[i] ).vecMemberId.size() ); j++ )
		{
			pUser = (FLWSUser*)prj.GetUserByID( m_vecTenderGuild.at( m_nIndex[i] ).vecMemberId.at( j ) );
			if( IsPossibleUser( pUser ) )
				SendNowState( pUser );
		}
	}
}

// ���� ���� ����
void	CGuildCombat1to1::SendNowState( FLWSUser* pUser )
{
	switch( m_nState )
	{
		case GC1TO1WAR_WAIT :
		{
			if( IsPossibleUser( pUser ) )
				pUser->AddGC1to1NowState( m_nState, m_nWaitTime, m_nProgWarCount );
			break;
		}
		
		case GC1TO1WAR_WAR :
		case GC1TO1WAR_CLOSEWAIT :
		{
			if( IsPossibleUser( pUser ) )
				pUser->AddGC1to1NowState( m_nState, m_nWaitTime );
			break;
		}

		case GC1TO1WAR_FINISH :
		case GC1TO1WAR_CLOSE :
		{
			if( IsPossibleUser( pUser ) )
				pUser->AddGC1to1NowState( m_nState, 0 );
			break;
		}
	}
}

// ���� ������ ��� ���۰� ����� �������� ��� �������� ����
void	CGuildCombat1to1::SendWarResultAllPlayer()
{
	FLWSUser* pUser = NULL;
	for( int i=0; i<2; i++ )
	{
		for( int j=0; j<(int)( m_vecTenderGuild.at( m_nIndex[i] ).vecMemberId.size() ); j++ )
		{
			pUser = (FLWSUser*)prj.GetUserByID( m_vecTenderGuild.at( m_nIndex[i] ).vecMemberId.at( j ) );
			if( IsPossibleUser( pUser ) )
			{
				u_long uIdPlayer = NULL_ID;
				if( (int)( m_vecTenderGuild.at( m_nIndex[i] ).vecMemberId.size() ) > m_nProgWarCount	)
					uIdPlayer = m_vecTenderGuild.at( m_nIndex[i] ).vecMemberId.at( m_nProgWarCount );
				//else
				//	continue;

				switch( m_nState )
				{
					case GC1TO1WAR_WAR :	// ��� ���۽�
					{
						pUser->AddGC1to1WarResult( g_GuildCombat1to1Mng.GC1TO1_PLAYER_WAR, uIdPlayer, 0, 0 );
						break;
					}

					case GC1TO1WAR_FINISH :		// ��� �����
					{
						int nOurWinCount = m_vecTenderGuild.at( m_nIndex[i] ).nWinCount;
						int nOtherWinCount = m_vecTenderGuild.at( m_nIndex[(i+1)%2] ).nWinCount;
						
						pUser->AddGC1to1WarResult( m_vecTenderGuild.at( m_nIndex[i] ).nLastWinState, uIdPlayer, nOurWinCount, nOtherWinCount );
						
						// ��ü �޼��� ���
						int nIndex = 2;	// ����� �� 
						if( m_vecTenderGuild.at( m_nIndex[0] ).nLastWinState == g_GuildCombat1to1Mng.GC1TO1_PLAYER_WIN )
							nIndex = 0; // 0��尡 �¸����� ��
						else if( m_vecTenderGuild.at( m_nIndex[1] ).nLastWinState == g_GuildCombat1to1Mng.GC1TO1_PLAYER_WIN )
							nIndex = 1;	// 1��尡 �¸����� ��
						
						// �������� ��� �������� �޼��� ���	
						if( nIndex == 2 ) // ����� ��
							pUser->AddDefinedCaption( TRUE, TID_GAME_GUILDCOMBAT1TO1_DRAWMSG, "%d", m_nProgWarCount+1 );
						else
						{
							FLWSUser* pWinUser = NULL;
							if( (int)( m_vecTenderGuild.at( m_nIndex[nIndex] ).vecMemberId.size() ) > m_nProgWarCount	)								
								pWinUser = (FLWSUser*)prj.GetUserByID( m_vecTenderGuild.at( m_nIndex[nIndex] ).vecMemberId.at( m_nProgWarCount ) );
							if( IsValidObj( pWinUser ) )
							{
								CGuild* pGuild = pWinUser->GetGuild();
								if( pGuild )
									pUser->AddDefinedCaption( TRUE, TID_GAME_GUILDCOMBAT1TO1_WINMSG, "%d %s %s", m_nProgWarCount+1, pGuild->m_szGuild, pWinUser->GetName() );
							}
						}
						break;
					}
				} // switch
			} // if
		} // for j
	} // for i
}

// ���� ������ ����ΰ�?
BOOL	CGuildCombat1to1::IsPossibleUser( FLWSUser* pUser )
{
	if( !pUser )
		return FALSE;
	
	int nIndex = g_GuildCombat1to1Mng.GetTenderGuildIndexByUser( pUser );
	if( nIndex != NULL_ID )
	{
		CWorld* pWorld = pUser->GetWorld();
		if( pWorld && pWorld->GetID() == m_vecTenderGuild.at( nIndex ).dwWorldId )
			return TRUE;
	}
		
	return FALSE;
}

// �¸��� ����޴� Ĩ����(��� ��ũ��Ʈ��)
int		CGuildCombat1to1::GetChipNum()
{
	return (int)g_GuildCombat1to1Mng.m_Lua.GetGlobalNumber( "WinChipNum" );
}

// ���ڿ��� Ĩ�����ϰ� �޼��� ���(���ڴ� �޼����� ���)
void	CGuildCombat1to1::SetWinChip( FLWSUser* pUserWin, FLWSUser* pUserLost, BOOL bDraw, BOOL bDiag )
{
	if( bDraw )	// ����� ��(Ĩ ���� ���ϰ� �޼����� ���)
	{
		if( bDiag )	// ���� ���и� �޼��� �ڽ� ���
		{
			if( IsPossibleUser( pUserWin ) )
				pUserWin->AddDiagText( prj.GetText( TID_GAME_GUILDCOMBAT_1TO1_WARDRAW ) );
			if( IsPossibleUser( pUserLost ) )
				pUserLost->AddDiagText( prj.GetText( TID_GAME_GUILDCOMBAT_1TO1_WARDRAW ) );
		}
		return;
	}

	if( IsPossibleUser( pUserWin ) )
	{
		FLItemElem itemElem;
//		itemElem.m_dwItemId = ITEM_INDEX( 26460, II_CHP_RED );

		const DWORD dwRewardItemID	= g_GuildCombat1to1Mng.GetRewardItemID();
		itemElem.m_dwItemId = dwRewardItemID;
		
		itemElem.m_nItemNum = GetChipNum();
		if( itemElem.m_nItemNum < 1 )
			itemElem.m_nItemNum = 1;
		itemElem.m_bCharged = itemElem.GetProp()->bCharged;
		itemElem.SetSerialNumber();

		LogItemInfo aLogItem;
		//aLogItem.Action = "W";
		//aLogItem.SendName = pUserWin->GetName();
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "W" );
		FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUserWin->GetName() );

// 		aLogItem.Gold = pUserWin->GetItemNum( ITEM_INDEX( 26460, II_CHP_RED ) );
// 		aLogItem.Gold2 = pUserWin->GetItemNum( ITEM_INDEX( 26460, II_CHP_RED ) ) + itemElem.m_nItemNum;

		aLogItem.Gold = pUserWin->GetItemNum( dwRewardItemID );
		aLogItem.Gold2 = pUserWin->GetItemNum( dwRewardItemID ) + itemElem.m_nItemNum;

		aLogItem.Gold_1 = itemElem.m_nItemNum;
		aLogItem.WorldId = pUserWin->GetWorld()->m_dwWorldID;
				
		if( pUserWin->CreateItem( &itemElem ) )
		{
			//aLogItem.RecvName = "GUILDCOMBAT1TO1_CHIP";
			//if( !bDiag )
				//aLogItem.RecvName = "GUILDCOMBAT1TO1_G_CHIP";

			FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "GUILDCOMBAT1TO1_CHIP" );
			if( !bDiag )
				FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "GUILDCOMBAT1TO1_G_CHIP" );
		}
		else	// ���� ���н�(�κ��丮�� ��á�ų� ���� ������...) ���Ϸ� ����
		{
			g_dpDBClient.SendQueryPostMail( pUserWin->m_idPlayer, 0, itemElem, 0, itemElem.GetProp()->szName, (char*)GETTEXT( TID_GAME_GUILDCOMBAT1TO1_WINCHIPPOST ) );
			//pUserWin->AddDefinedText( TID_GAME_MINIGAME_ITEM_POST );
			//aLogItem.RecvName = "GUILDCOMBAT1TO1_CHIP_POST";
			//if( !bDiag )
			//	aLogItem.RecvName = "GUILDCOMBAT1TO1_CHIP_G_POST";
			FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "GUILDCOMBAT1TO1_CHIP_POST" );
			if( !bDiag )
				FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "GUILDCOMBAT1TO1_CHIP_G_POST" );
		}
		
		if( bDiag ) // ���� ���и� �޼��� �ڽ� ���
		{
			CString strTemp;
			strTemp.Format( prj.GetText( TID_GAME_GUILDCOMBAT_1TO1_WARWIN ), GetChipNum() );
			pUserWin->AddDiagText( strTemp );
		}
			
		g_DPSrvr.OnLogItem( aLogItem, &itemElem, itemElem.m_nItemNum );
	}

	if( IsPossibleUser( pUserLost ) && bDiag )
		pUserLost->AddDiagText( prj.GetText( TID_GAME_GUILDCOMBAT_1TO1_WARLOST ) );
}

// ���� �������� ������ ��� ������ �ڷ���Ʈ�� ��� �ð�...
void	CGuildCombat1to1::GuildCombat1to1CloseWait()
{
	ClearTime();
	m_nState = GC1TO1WAR_CLOSEWAIT;
	m_nWaitTime = (int)( g_GuildCombat1to1Mng.m_Lua.GetGlobalNumber( "WarCloseWaitTime" ) );
	m_dwTime = GetTickCount();
	SendNowStateAllPlayer();

	// �¸� ��� �Ǻ�
	int nWinIndex = NULL_ID;
	if( m_vecTenderGuild.at( m_nIndex[0] ).nWinCount > m_vecTenderGuild.at( m_nIndex[1] ).nWinCount )
		nWinIndex = m_nIndex[0];
	else if( m_vecTenderGuild.at( m_nIndex[0] ).nWinCount < m_vecTenderGuild.at( m_nIndex[1] ).nWinCount )
		nWinIndex = m_nIndex[1];
	
	// ��� ���� �޼��� ���(�¸� ���� Ĩ �߰� ����)
	FLWSUser* pUser = NULL;
	for( int i=0; i<2; i++ )
	{
		for( int j=0; j<(int)( m_vecTenderGuild.at( m_nIndex[i] ).vecMemberId.size() ); j++ )
		{
			pUser = (FLWSUser*)prj.GetUserByID( m_vecTenderGuild.at( m_nIndex[i] ).vecMemberId.at( j ) );
			if( IsPossibleUser( pUser ) )
			{
				if( m_nIndex[i] == nWinIndex )	// �¸� ���
				{
					SetWinChip( pUser, NULL, FALSE, FALSE );
					pUser->AddDefinedCaption( TRUE, TID_GAME_GUILDCOMBAT1TO1_WARGUILDWIN, "" );
					pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_WARGUILDWINCHIP, "%d", GetChipNum() );
				}
				else if( nWinIndex == NULL_ID ) // ���
				{
					SetWinChip( pUser, NULL, TRUE, FALSE );
					pUser->AddDefinedCaption( TRUE, TID_GAME_GUILDCOMBAT1TO1_WARGUILDDRAW, "" );
				}
				else							// �й� ���
				{
					SetWinChip( NULL, pUser, FALSE, FALSE );
					pUser->AddDefinedCaption( TRUE, TID_GAME_GUILDCOMBAT1TO1_WARGUILDLOST, "" );
				}

				SendNowState( pUser );
			}
		}
	}

	g_dpDBClient.SendGC1to1Tender( 'U', m_vecTenderGuild.at( m_nIndex[0] ).ulGuildId, m_vecTenderGuild.at( m_nIndex[0] ).nPenya, 'E' );
	g_dpDBClient.SendGC1to1Tender( 'U', m_vecTenderGuild.at( m_nIndex[1] ).ulGuildId, m_vecTenderGuild.at( m_nIndex[1] ).nPenya, 'E' );
	g_dpDBClient.SendGC1to1WarGuild( m_vecTenderGuild.at( m_nIndex[0] ).dwWorldId, 0, 0, 'E' );
}

// ���� ������ �����ϰ� ������ �ڷ���Ʈ
void	CGuildCombat1to1::GuildCombat1to1Close()
{
	ClearTime();
	m_nState = GC1TO1WAR_CLOSE;

	FLWSUser* pUser = NULL;
	for( int i=0; i<2; i++ )
	{
		for( int j=0; j<(int)( m_vecTenderGuild.at( m_nIndex[i] ).vecMemberId.size() ); j++ )
		{
			pUser = (FLWSUser*)prj.GetUserByID( m_vecTenderGuild.at( m_nIndex[i] ).vecMemberId.at( j ) );
			if( IsPossibleUser( pUser ) )
			{
				SendNowState( pUser );
				pUser->REPLACE( g_uIdofMulti, WI_WORLD_MADRIGAL, D3DXVECTOR3( 6983.0f, 0.0f, 3326.0f ), REPLACE_NORMAL, nDefaultLayer );
			}
		}
	}
}

// �����ϱ��� ���ð�.. �����ڰ� ���� ���� ����ó���� ���� �Ѵ�.
void	CGuildCombat1to1::GuildCombat1to1WarWait()
{
	// ��� ������ ������ ��
	if( (m_nProgWarCount >= m_nTotalWarCount) || (m_nProgWarCount >= g_GuildCombat1to1Mng.m_nMaxJoinPlayer) )
	{
		GuildCombat1to1CloseWait();
		return;
	}
	
	FLWSUser* pUser0 = NULL;
	FLWSUser* pUser1 = NULL;
	
	// ���� �ο����� ���� ���

	if( (int)( m_vecTenderGuild.at( m_nIndex[0] ).vecMemberId.size() ) > m_nProgWarCount )
	{
		pUser0 = (FLWSUser*)prj.GetUserByID( m_vecTenderGuild.at( m_nIndex[0] ).vecMemberId.at( m_nProgWarCount ) );
	}

	if( (int)( m_vecTenderGuild.at( m_nIndex[1] ).vecMemberId.size() ) > m_nProgWarCount	)
	{
		pUser1 = (FLWSUser*)prj.GetUserByID( m_vecTenderGuild.at( m_nIndex[1] ).vecMemberId.at( m_nProgWarCount ) );
	}
		
	// ���� �����ڰ� ��� ������ ��� �������
	if( IsPossibleUser( pUser0 ) && IsPossibleUser( pUser1 ) )
	{
		m_nState = GC1TO1WAR_WAIT;
		m_nWaitTime = (int)g_GuildCombat1to1Mng.m_Lua.GetGlobalNumber( "WarWaitTime" );
		m_dwTime	= GetTickCount();
		SendNowStateAllPlayer();
	}
	else	// ���� �����ڰ� ���� ��� ����ó��
		GuildCombat1to1WarResultCheck();
}

// ���� �����ڰ� ��� �����ϸ� ���� ����
void	CGuildCombat1to1::GuildCombat1to1War()
{
	FLWSUser* pUser0 = NULL;
	FLWSUser* pUser1 = NULL;

	if( (int)( m_vecTenderGuild.at( m_nIndex[0] ).vecMemberId.size() ) > m_nProgWarCount	)
		pUser0 = (FLWSUser*)prj.GetUserByID( m_vecTenderGuild.at( m_nIndex[0] ).vecMemberId.at( m_nProgWarCount ) );
	if( (int)( m_vecTenderGuild.at( m_nIndex[1] ).vecMemberId.size() ) > m_nProgWarCount	)
		pUser1 = (FLWSUser*)prj.GetUserByID( m_vecTenderGuild.at( m_nIndex[1] ).vecMemberId.at( m_nProgWarCount ) );

	// ���� �����ڰ� ��� �����ϸ� ���� ����
	if( IsPossibleUser( pUser0 ) && IsPossibleUser( pUser1 ) )
	{
		g_dpDBClient.SendGC1to1WarPerson( m_vecTenderGuild.at( m_nIndex[0] ).ulGuildId, m_vecTenderGuild.at( m_nIndex[0] ).vecMemberId.at( m_nProgWarCount ), 'N' );
		g_dpDBClient.SendGC1to1WarPerson( m_vecTenderGuild.at( m_nIndex[1] ).ulGuildId, m_vecTenderGuild.at( m_nIndex[1] ).vecMemberId.at( m_nProgWarCount ), 'N' );
		SetTeleportToWarStage( pUser0 );	// ���������� �ڷ���Ʈ
		SetTeleportToWarStage( pUser1 );	// ���������� �ڷ���Ʈ 

		m_nState = GC1TO1WAR_WAR;
		m_nWaitTime = (int)g_GuildCombat1to1Mng.m_Lua.GetGlobalNumber( "WarTime" );
		m_dwTime = GetTickCount();
		SendNowStateAllPlayer();
		SendWarResultAllPlayer();	// ���� ���� �˸�
	}
	else
		GuildCombat1to1WarWait();

}

// ���� ��� ó�� �� ������ �α� �ƿ��� ó��
void	CGuildCombat1to1::GuildCombat1to1WarResultCheck( BOOL bLogOut, FLWSUser* pUserLogOut )
{
	FLWSUser* pUser0 = NULL;
	FLWSUser* pUser1 = NULL;
	if( (int)( m_vecTenderGuild.at( m_nIndex[0] ).vecMemberId.size() ) > m_nProgWarCount	)
		pUser0 = (FLWSUser*)prj.GetUserByID( m_vecTenderGuild.at( m_nIndex[0] ).vecMemberId.at( m_nProgWarCount ) );
	if( (int)( m_vecTenderGuild.at( m_nIndex[1] ).vecMemberId.size() ) > m_nProgWarCount	)
		pUser1 = (FLWSUser*)prj.GetUserByID( m_vecTenderGuild.at( m_nIndex[1] ).vecMemberId.at( m_nProgWarCount ) );

	// �α� �ƿ� ��Ȳ�� ��
	if( bLogOut )
	{
		if( pUser0 == pUserLogOut )
			pUser0 = NULL;
		if( pUser1 == pUserLogOut )
			pUser1 = NULL;
	}

	// ���� ������ ��� ������ ��
	if( IsPossibleUser( pUser0 ) && IsPossibleUser( pUser1 ) )
	{
		for( int i=0; i<2; i++ )
		{
			if( m_vecTenderGuild.at( m_nIndex[i] ).nNowPlayerLife == 0 )
			{
				m_vecTenderGuild.at( m_nIndex[(i+1)%2] ).nWinCount++;
				SetLastWinState( (i+1)%2 );
				m_nState = GC1TO1WAR_FINISH;
				// �¸�( Ĩ ���� )
				if( (i+1)%2 == 0 )
					SetWinChip( pUser0, pUser1 );
				else
					SetWinChip( pUser1, pUser0 );
				return;
			}
		}
		
		// ���� �ð��� ����Ǿ��� ��
		if( m_nWaitTime <= 0 )
		{
			if( m_vecTenderGuild.at( m_nIndex[0] ).nNowPlayerLife > m_vecTenderGuild.at( m_nIndex[1] ).nNowPlayerLife )
			{
				m_vecTenderGuild.at( m_nIndex[0] ).nWinCount++;
				SetLastWinState( 0 );
				SetWinChip( pUser0, pUser1 );	// Ĩ ����
			}
			else if( m_vecTenderGuild.at( m_nIndex[0] ).nNowPlayerLife < m_vecTenderGuild.at( m_nIndex[1] ).nNowPlayerLife )
			{
				m_vecTenderGuild.at( m_nIndex[1] ).nWinCount++;
				SetLastWinState( 1 );
				SetWinChip( pUser1, pUser0 );	// Ĩ ����
			}
			else
			{
				SetLastWinState( 2 );	// 2�� ����� ��
				SetWinChip( pUser0, pUser1, TRUE );	// Ĩ �������� �ʰ� ��� �޼��� ���
			}
				
			m_nState = GC1TO1WAR_FINISH;
		}
	}
	else if( IsPossibleUser( pUser0 ) && !IsPossibleUser( pUser1 ) )
	{
		m_vecTenderGuild.at( m_nIndex[0] ).nWinCount++;
		SetLastWinState( 0 );
		m_nState = GC1TO1WAR_FINISH;
		SetWinChip( pUser0, pUser1 );	// �¸�( Ĩ ���� )
		pUser0->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_WARNOOTHER );
	}
	else if( !IsPossibleUser( pUser0 ) && IsPossibleUser( pUser1 ) )
	{
		m_vecTenderGuild.at( m_nIndex[1] ).nWinCount++;
		SetLastWinState( 1 );
		m_nState = GC1TO1WAR_FINISH;
		SetWinChip( pUser1, pUser0 );	// �¸�( Ĩ ���� )
		pUser1->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_WARNOOTHER );
	}
	else
	{
		SetLastWinState( 2 );	// 2�� ����� ��
		m_nState = GC1TO1WAR_FINISH;
		SetWinChip( pUser0, pUser1, TRUE );	// ���
	}
}

// �ֱ� ��� ����� �����Ѵ�.
void	CGuildCombat1to1::SetLastWinState( int nIndex )
{
	// ����� �ʱ�ȭ
	for( int i=0; i<2; i++ )
		m_vecTenderGuild.at( m_nIndex[i] ).nNowPlayerLife = g_GuildCombat1to1Mng.m_nPlayerLife;

	switch( nIndex )
	{
		case 0 :	// 0�� �̰��� ��
			m_vecTenderGuild.at( m_nIndex[0] ).nLastWinState = g_GuildCombat1to1Mng.GC1TO1_PLAYER_WIN;
			if( (int)( m_vecTenderGuild.at( m_nIndex[0] ).vecMemberId.size() ) > m_nProgWarCount	)
				g_dpDBClient.SendGC1to1WarPerson( m_vecTenderGuild.at( m_nIndex[0] ).ulGuildId, m_vecTenderGuild.at( m_nIndex[0] ).vecMemberId.at( m_nProgWarCount ), 'W' );
			m_vecTenderGuild.at( m_nIndex[1] ).nLastWinState = g_GuildCombat1to1Mng.GC1TO1_PLAYER_LOSE;
			if( (int)( m_vecTenderGuild.at( m_nIndex[1] ).vecMemberId.size() ) > m_nProgWarCount	)
				g_dpDBClient.SendGC1to1WarPerson( m_vecTenderGuild.at( m_nIndex[1] ).ulGuildId, m_vecTenderGuild.at( m_nIndex[1] ).vecMemberId.at( m_nProgWarCount ), 'L' );
			break;

		case 1 :	// 1�� �̰��� ��
			m_vecTenderGuild.at( m_nIndex[0] ).nLastWinState = g_GuildCombat1to1Mng.GC1TO1_PLAYER_LOSE;
			if( (int)( m_vecTenderGuild.at( m_nIndex[0] ).vecMemberId.size() ) > m_nProgWarCount	)
				g_dpDBClient.SendGC1to1WarPerson( m_vecTenderGuild.at( m_nIndex[0] ).ulGuildId, m_vecTenderGuild.at( m_nIndex[0] ).vecMemberId.at( m_nProgWarCount ), 'L' );
			m_vecTenderGuild.at( m_nIndex[1] ).nLastWinState = g_GuildCombat1to1Mng.GC1TO1_PLAYER_WIN;
			if( (int)( m_vecTenderGuild.at( m_nIndex[1] ).vecMemberId.size() ) > m_nProgWarCount	)
				g_dpDBClient.SendGC1to1WarPerson( m_vecTenderGuild.at( m_nIndex[1] ).ulGuildId, m_vecTenderGuild.at( m_nIndex[1] ).vecMemberId.at( m_nProgWarCount ), 'W' );
			break;

		case 2 :	// ����� ��
			m_vecTenderGuild.at( m_nIndex[0] ).nLastWinState = g_GuildCombat1to1Mng.GC1TO1_PLAYER_DRAW;
			if( (int)( m_vecTenderGuild.at( m_nIndex[0] ).vecMemberId.size() ) > m_nProgWarCount	)
				g_dpDBClient.SendGC1to1WarPerson( m_vecTenderGuild.at( m_nIndex[0] ).ulGuildId, m_vecTenderGuild.at( m_nIndex[0] ).vecMemberId.at( m_nProgWarCount ), 'D' );
			m_vecTenderGuild.at( m_nIndex[1] ).nLastWinState = g_GuildCombat1to1Mng.GC1TO1_PLAYER_DRAW;
			if( (int)( m_vecTenderGuild.at( m_nIndex[1] ).vecMemberId.size() ) > m_nProgWarCount	)
				g_dpDBClient.SendGC1to1WarPerson( m_vecTenderGuild.at( m_nIndex[1] ).ulGuildId, m_vecTenderGuild.at( m_nIndex[1] ).vecMemberId.at( m_nProgWarCount ), 'D' );
			break;
	}
}

// ���� �� ������ ó��
void	CGuildCombat1to1::SetLost( FLWSUser* pUserDef )
{
	FLWSUser* pUser0 = NULL;
	FLWSUser* pUser1 = NULL;
	
	if( (int)( m_vecTenderGuild.at( m_nIndex[0] ).vecMemberId.size() ) > m_nProgWarCount	)
		pUser0 = (FLWSUser*)prj.GetUserByID( m_vecTenderGuild.at( m_nIndex[0] ).vecMemberId.at( m_nProgWarCount ) );
	if( (int)( m_vecTenderGuild.at( m_nIndex[1] ).vecMemberId.size() ) > m_nProgWarCount	)
		pUser1 = (FLWSUser*)prj.GetUserByID( m_vecTenderGuild.at( m_nIndex[1] ).vecMemberId.at( m_nProgWarCount ) );

	if( IsPossibleUser( pUser0 ) && IsPossibleUser( pUser1 ) )
	{
		if( pUser0 == pUserDef )
			m_vecTenderGuild.at( m_nIndex[0] ).nNowPlayerLife--;
		else if( pUser1 == pUserDef )
			m_vecTenderGuild.at( m_nIndex[1] ).nNowPlayerLife--;
	}

	GuildCombat1to1WarResultCheck();
}

// �ֱ� �����ڸ� �����忡�� ���Ƿ� �ڷ���Ʈ �Ѵ�.
void	CGuildCombat1to1::SetTeleportToWaitStageNowPlayers()
{
	FLWSUser* pUser0 = NULL;
	FLWSUser* pUser1 = NULL;

	if( (int)( m_vecTenderGuild.at( m_nIndex[0] ).vecMemberId.size() ) > m_nProgWarCount	)
		pUser0 = (FLWSUser*)prj.GetUserByID( m_vecTenderGuild.at( m_nIndex[0] ).vecMemberId.at( m_nProgWarCount ) );
	if( (int)( m_vecTenderGuild.at( m_nIndex[1] ).vecMemberId.size() ) > m_nProgWarCount	)
		pUser1 = (FLWSUser*)prj.GetUserByID( m_vecTenderGuild.at( m_nIndex[1] ).vecMemberId.at( m_nProgWarCount ) );
	
	if( IsPossibleUser( pUser0 ) )
		SetTeleportToWaitStage( pUser0 );
	if( IsPossibleUser( pUser1 ) )
		SetTeleportToWaitStage( pUser1 );
}

// ���Ƿ� �ڷ���Ʈ(�����, ���������)
void	CGuildCombat1to1::SetTeleportToWaitStage( FLWSUser* pUser )
{
	int nIndex = g_GuildCombat1to1Mng.GetTenderGuildIndexByUser( pUser );
	if( !g_WorldMng.GetWorld( m_vecTenderGuild.at( nIndex ).dwWorldId ) )
	{
		pUser->AddText( "Not Exist World" );
		return;
	}
	pUser->m_nGuildCombatState = 2; // ������ ���
	g_xWSUserManager->AddGuildCombatUserState( pUser );
	
	int nRandx = xRandom(4) - 2;
	int nRandz = xRandom(4) - 2;
	if( m_vecTenderGuild.at( nIndex ).nPosition == 0 )
	{
		pUser->SetAngle( 0 );
		pUser->REPLACE( g_uIdofMulti, m_vecTenderGuild.at( nIndex ).dwWorldId, D3DXVECTOR3( (float)( 256+nRandx ), 90.0f, (float)( 277+nRandz ) ), REPLACE_NORMAL, nDefaultLayer );
	}
	else
	{
		pUser->SetAngle( 180 );
		pUser->REPLACE( g_uIdofMulti, m_vecTenderGuild.at( nIndex ).dwWorldId, D3DXVECTOR3( (float)( 272+nRandx ), 90.0f, (float)( 212+nRandz ) ), REPLACE_NORMAL, nDefaultLayer );
	}
}

// ���������� �ڷ���Ʈ
void	CGuildCombat1to1::SetTeleportToWarStage( FLWSUser* pUser )
{
	int nIndex = g_GuildCombat1to1Mng.GetTenderGuildIndexByUser( pUser );

	pUser->m_nGuildCombatState = 1; // ������ ���
	g_xWSUserManager->AddGuildCombatUserState( pUser );
	if( m_vecTenderGuild.at( nIndex ).nPosition == 0 )
	{
		pUser->SetAngle( 0 );
		pUser->REPLACE( g_uIdofMulti, m_vecTenderGuild.at( nIndex ).dwWorldId, D3DXVECTOR3( 260, 75, 252 ), REPLACE_NORMAL, nDefaultLayer );
	}
	else
	{
		pUser->SetAngle( 180 );
		pUser->REPLACE( g_uIdofMulti, m_vecTenderGuild.at( nIndex ).dwWorldId, D3DXVECTOR3( 260, 75, 235 ), REPLACE_NORMAL, nDefaultLayer );
	}
}

#endif // __WORLDSERVER



//////////////////////////////////////////////////////////////////////
//// CGuildCombat1To1Mng /////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
CGuildCombat1to1Mng::CGuildCombat1to1Mng()
{
	m_nJoinPenya = 0;
	m_nMaxJoinPlayer = 0;
	m_nMinJoinPlayer = 0;
	m_nMinJoinPlayerLevel = 0;
#ifdef __DBSERVER
	m_nCombatId = 0;
#endif // __DBSERVER
#if defined(__WORLDSERVER) || defined(__DBSERVER)
	m_nGuildLevel = 0;
	m_nPlayerLife = 0;
	m_nMaxJoinGuild = 0;
	m_nMinJoinGuild = 0;
	m_nCancelReturnRate = 0;
	m_nFailReturnRate = 0;

	m_dwRewardItemID	= NULL_ID;
#endif // __WORLDSERVER || __DBSERVER
	ClearTime();
	m_nState = GC1TO1_CLOSE;
#ifdef __CLIENT
	m_vecGuildCombat1to1_Players.clear();
	m_nGuildCombat1to1Guild = 0;
	m_nMyGuildCount = 0;
	m_nVsGuildCount = 0;
#endif //__CLIENT
}

CGuildCombat1to1Mng::~CGuildCombat1to1Mng()
{

}

#ifdef __CLIENT
void CGuildCombat1to1Mng::AddGuildCombat1to1Player(u_long nPlayerId)
{
	__GC1TO1_PLAYER_INFO nTempInfo;
	nTempInfo.m_uidPlayer = nPlayerId;
	nTempInfo.m_nState = GC1TO1_PLAYER_READY;
	m_vecGuildCombat1to1_Players.push_back(nTempInfo);
}

void CGuildCombat1to1Mng::SetGuildCombat1to1Guild(u_long nGuildId)
{
	m_nGuildCombat1to1Guild = nGuildId;
}

void CGuildCombat1to1Mng::SetGuildCombat1to1State(int nState, u_long nPlayer, int nMyGuildPoint, int nVsGuildPoint)
{
	vector < __GC1TO1_PLAYER_INFO >::iterator it = m_vecGuildCombat1to1_Players.begin();
	for( ; it != m_vecGuildCombat1to1_Players.end(); ++it )
	{
		__GC1TO1_PLAYER_INFO &Playerinfo = *it;
		if( Playerinfo.m_uidPlayer ==  nPlayer )
			Playerinfo.m_nState = nState;
	}

	if(nState != GC1TO1_PLAYER_READY && nState != GC1TO1_PLAYER_WAR)
	{
		m_nMyGuildCount = nMyGuildPoint;
		m_nVsGuildCount = nVsGuildPoint;
	}
}

// Ŭ��� ���� ������ �����ΰ�?
BOOL	CGuildCombat1to1Mng::IsPossibleMover( CMover* pMover )
{
	if( !pMover )
		return FALSE;
	
	CWorld* pWorld = pMover->GetWorld();
	if( pWorld )
	{
		if( pWorld->GetID() >= WI_WORLD_GUILDWAR1TO1_0 &&
			pWorld->GetID() <= WI_WORLD_GUILDWAR1TO1_L )
		{
			if( pMover->m_nGuildCombatState != 0 )
				return TRUE;
		}
	}
	
	return FALSE;
}

#endif //__CLIENT

void	CGuildCombat1to1Mng::ClearTime()
{
	m_nWaitTime = 0;
	m_dwTime = 0;
}

#ifdef __DBSERVER
// ������ ���� �ð� üũ
int		CGuildCombat1to1Mng::CheckOpenTIme()
{
	int nReturnValue = 0;
	if( m_Lua.GetLuaFunction( "CheckOpenTime" ) )
	{
		m_Lua.CallLuaFunction( 0, 1 );
		
		nReturnValue = (int)( m_Lua.ToNumber( -1 ) );
	}
	m_Lua.Pop(0);

	return nReturnValue;
}
#endif // __DBSERVER

#if defined(__WORLDSERVER) || defined(__DBSERVER)
// ��ũ��Ʈ �ε� (����, �����ͺ��̽�)
BOOL CGuildCombat1to1Mng::LoadScript()
{
	const TCHAR szFileName[]	= _T( "GuildCombat1TO1.lua" );

	TCHAR szFullPath[ MAX_PATH ]	= { 0, };
	g_pScriptFileManager->GetScriptFileFullPath( szFullPath, _countof( szFullPath ), szFileName );

	if( m_Lua.RunScript( szFullPath ) != 0 )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "[ FAILED LOAD SCRIPT. FILE_NAME:(%s) ]" ), szFullPath );
		FLASSERT(0);
	}

	m_nJoinPenya = (int)m_Lua.GetGlobalNumber( "MinJoinPenya" );
	m_nGuildLevel = (int)m_Lua.GetGlobalNumber( "MinGuildLevel" );
	m_nMinJoinGuild = (int)m_Lua.GetGlobalNumber( "MinJoinGuild" );
	m_nMaxJoinGuild = (int)m_Lua.GetGlobalNumber( "MaxJoinGuild" );
	m_nMinJoinPlayer = (int)m_Lua.GetGlobalNumber( "MinJoinPlayer" );
#ifndef __MAINSERVER
	m_nMinJoinPlayer = 1;
#endif // __MAINSERVER
	m_nMaxJoinPlayer = (int)m_Lua.GetGlobalNumber( "MaxJoinPlayer" );
	m_nMinJoinPlayerLevel = (int)m_Lua.GetGlobalNumber( "MinJoinPlayerLevel" );
	m_nCancelReturnRate = (int)m_Lua.GetGlobalNumber( "CancelReturnRate" );
	m_nFailReturnRate = (int)m_Lua.GetGlobalNumber( "FailReturnRate" );
	m_nPlayerLife = (int)m_Lua.GetGlobalNumber( "PlayerLife" );

	const DWORD dwRewardItemID		= static_cast<DWORD>( CScript::GetDefineNum( m_Lua.GetGlobalString( "REWARD_ITEM" ) ) );
	const T_ITEM_SPEC* pItemSpec	= g_xSpecManager->GetSpecItem( dwRewardItemID );
	if( pItemSpec == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID REWARD ITEM ID. ITEM_ID(%u) ]" ), dwRewardItemID );
		RELEASE_BREAK;
		return FALSE;
	}

	SetRewardItemID( dwRewardItemID );
	
	return TRUE;
}
#endif // __WORLDSERVER || __DBSERVER

#ifdef __WORLDSERVER
// 1:1������ ���½� �� ������ ����
void	CGuildCombat1to1Mng::CreateGuildCombat1to1()
{
	multimap<int, int> mapRandom;

	int	i = 0;
	for( i=0; i<(int)( m_vecTenderGuild.size() ); i++ )
	{
		mapRandom.insert( make_pair( xRandom(m_vecTenderGuild.size()), i/2 ) );
	}
	
	i = 0;
	for( multimap<int, int>::iterator it=mapRandom.begin(); it!=mapRandom.end(); it++ )
	{
		m_vecTenderGuild.at( i ).nStageId = it->second;
		// �� �������� WorldID�� ����
		m_vecTenderGuild.at( i ).dwWorldId = WI_WORLD_GUILDWAR1TO1_0 + it->second;
		i++;
	}

	m_vecGuilCombat1to1.clear();
	for( i=0; i<(int)( m_vecTenderGuild.size()/2 ); i++ )
	{
		CGuildCombat1to1 GuildCombat1to1( m_vecTenderGuild, i );
		m_vecGuilCombat1to1.push_back( GuildCombat1to1 );
	}
}

// ���� ���(m_vecTenderGuild)�� �ε��� ��
int		CGuildCombat1to1Mng::GetTenderGuildIndexByUser( FLWSUser* pUser )
{
	CGuild* pGuild = pUser->GetGuild();
	if( pGuild )
	{
		for( int i=0; i<(int)( m_vecTenderGuild.size() ); i++ )
		{
			if( pGuild->GetGuildId() == m_vecTenderGuild.at( i ).ulGuildId )
				return i;
		}
	}

	return NULL_ID;
}

// ��� ����� index ��
int		CGuildCombat1to1Mng::GetTargetTenderGuildIndexByUser( FLWSUser* pUser )
{
	int nIndex = GetTenderGuildIndexByUser( pUser );
	for( int i=0; i<(int)( m_vecTenderGuild.size() ); i++ )
	{
		if( nIndex != i && m_vecTenderGuild.at( nIndex ).nStageId == m_vecTenderGuild.at( i ).nStageId )
			return i;
	}

	return NULL_ID;
}

// ������ ������ ���Ե� �����ΰ�?
BOOL	CGuildCombat1to1Mng::IsLineUpMember( FLWSUser* pUser )
{
	int nIndex = GetTenderGuildIndexByUser( pUser );
	if( nIndex == NULL_ID )
		return FALSE;

	for( int i=0; i<(int)( m_vecTenderGuild.at( nIndex ).vecMemberId.size() ); i++ )
	{
		if( m_vecTenderGuild.at( nIndex ).vecMemberId.at( i ) == pUser->m_idPlayer )
			return TRUE;
	}
	
	return FALSE;
}

// ��帶���� �ΰ�?
BOOL	CGuildCombat1to1Mng::IsGuildMaster( FLWSUser* pUser )
{
	CGuild* pGuild = pUser->GetGuild();
	if( pGuild && pGuild->IsMaster( pUser->m_idPlayer ) )
		return TRUE;

	return FALSE;
}

// �̹� �������� ��û�� ����ΰ�?
BOOL	CGuildCombat1to1Mng::IsGCTenderGuild( FLWSUser* pUser )
{
	if( g_GuildCombatMng.FindGuildCombatMember( pUser->m_idGuild ) &&
		g_GuildCombatMng.FindGuildCombatMember( pUser->m_idGuild )->bRequest )
		return TRUE;

	return FALSE;
}

// ���� �������� ���� ������ �����ΰ�?
BOOL	CGuildCombat1to1Mng::IsPossibleUser( FLWSUser* pUser )
{
	if( !pUser )
		return FALSE;

	int nIndex = GetTenderGuildIndexByUser( pUser );
	if( nIndex != NULL_ID )
	{
		int nStageId = m_vecTenderGuild.at( nIndex ).nStageId;
		if( nStageId != NULL_ID && m_vecGuilCombat1to1.at( nStageId ).IsPossibleUser( pUser ) )
			return TRUE;
	}

	return FALSE;
}

void	CGuildCombat1to1Mng::Process()
{
	if( !g_eLocal.GetState( EVE_GUILDCOMBAT1TO1 ) )
		return;

	if( m_nWaitTime > 0 )
	{
		m_nWaitTime -= GetTickCount() - m_dwTime;
		m_dwTime = GetTickCount();	
	}
	
	switch( m_nState )
	{
		case GC1TO1_CLOSE :	// ��� �������� ������ ����Ǿ��ų� ���µ��� ���� ����
			break;
		
		case GC1TO1_OPEN : // 1:1 ������ ���µ� ����...(������ �����ð�)
		{
			if( m_nWaitTime <= 0 )
			{
				ClearTime();
				CreateGuildCombat1to1();
				GuildCombat1to1EntranceWait();
			}
			break;
		}

		case GC1TO1_ENTRANCE :	// ���� ��� �ð�
		{
			if( m_nWaitTime <=0 )
			{
				ClearTime();
				GuildCombat1to1War();
			}
			break;
		}

		case GC1TO1_WAR :	// ��� ������ ���� �� ���� ��
		{
			int nCloseCount=0;
			for( int i=0; i<(int)( m_vecGuilCombat1to1.size() ); i++ )
			{
				m_vecGuilCombat1to1.at( i ).Process();	// �� �����庰 process
				if( m_vecGuilCombat1to1.at( i ).IsClosed() )
					nCloseCount++;
			}

			if( nCloseCount >= (int)( m_vecGuilCombat1to1.size() ) )	// ��� �������� ������ ����Ǿ��� �� ó��
			{
				ClearTime();
				g_DPCoreClient.SendCaption( prj.GetText( TID_GAME_GUILDCOMBAT1TO1_ALLCLOSE ) );
				GuildCombat1to1AllClose();
			}
			break;
		}
	}
}

// ���� State�� Client�� �����Ѵ�.(���ӽ�, state �����)
void	CGuildCombat1to1Mng::SendNowState( FLWSUser* pUser )
{
	if( !g_eLocal.GetState( EVE_GUILDCOMBAT1TO1 ) )
		return;
	
	switch( m_nState )
	{
		case GC1TO1_OPEN :
		{
			int nIndex = GetTenderGuildIndexByUser( pUser );
//			if( nIndex != NULL_ID && IsGuildMaster( pUser ) )
			if( nIndex != NULL_ID )
				pUser->AddGC1to1NowState( m_nState, m_nWaitTime );
			break;
		}
			
		case GC1TO1_ENTRANCE :
		{
			if( IsLineUpMember( pUser ) )
				pUser->AddGC1to1NowState( m_nState, m_nWaitTime );
			else if( IsGuildMaster( pUser ) )
				pUser->AddGC1to1NowState( GC1TO1_WAR, 0 );
			break;
		}		

		case GC1TO1_WAR :
		{
			int nIndex = GetTenderGuildIndexByUser( pUser );
			if( nIndex != NULL_ID )
				pUser->AddGC1to1NowState( m_nState, 0 );
			break;
		}

		case GC1TO1_CLOSE :
		{
			int nIndex = GetTenderGuildIndexByUser( pUser );
			if( nIndex != NULL_ID )
				pUser->AddGC1to1NowState( m_nState, 0 );
			break;
		}
	}
}

// 1��1 ������ ����
void	CGuildCombat1to1Mng::GuildCombat1to1Open( BOOL bGMText )
{
	if( !g_eLocal.GetState( EVE_GUILDCOMBAT1TO1 ) )
		return;
	
	vector<__GC1TO1TENDER>::iterator it;
	if( !bGMText )
	{
		for( it=m_vecTenderFailGuild.begin(); it!=m_vecTenderFailGuild.end(); it++ )
		{
			// ���� 1:1�������� ���� ���� ��尡 ��û���� �������� �ʾ��� ���
			g_dpDBClient.SendGC1to1Tender( 'U', (*it).ulGuildId, (*it).nPenya, 'N' );
		}
		m_vecTenderFailGuild.clear();
	}
	
	// ��û��  ������ �ּ� ���� �������� ���� ��
	if( (int)( m_vecTenderGuild.size() ) < m_nMinJoinGuild )
	{
		// 1��1 ������ ��� �� ���� ó��
		CString strTemp;
		strTemp.Format( prj.GetText( TID_GAME_GUILDCOMBAT1TO1_NOGAME ), m_nMinJoinGuild );
		g_DPCoreClient.SendCaption( strTemp );	
		ClearTime();
		m_nState = GC1TO1_CLOSE;
		return;
	}
	
	// 1:1������ ���� �޼���..
	g_DPCoreClient.SendCaption( prj.GetText( TID_GAME_GUILDCOMBAT1TO1_OPEN ) );
	g_DPCoreClient.SendCaption( prj.GetText( TID_GAME_GUILDCOMBAT1TO1_LINEUP ) );
	
	// ������ ���� �ð� ����
	m_nState = GC1TO1_OPEN;
	m_nWaitTime = (int)m_Lua.GetGlobalNumber( "MemberLineUpTime" );
	m_dwTime = GetTickCount();	

	// ���� ������ ���� ���� ��� ����
	int nCount = 0;
	for( it=m_vecTenderGuild.begin(); it!=m_vecTenderGuild.end(); )
	{
		nCount++;
		if( nCount <= m_nMaxJoinGuild )
			it++;
		else // ���� �������� �з��� ���
		{
			CGuild* pGuild = g_GuildMng.GetGuild( (*it).ulGuildId );
			if( pGuild )
			{
				FLWSUser* pUser = (FLWSUser*)prj.GetUserByID( pGuild->m_idMaster );
				if( IsValidObj( pUser ) )
					pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_FAILTENDER );
			}
			
			g_dpDBClient.SendGC1to1Tender( 'U', (*it).ulGuildId, (*it).nPenya, 'F' );
			m_vecTenderFailGuild.push_back( (*it) );
			it = m_vecTenderGuild.erase( it );
		}
	}		

	// ���� �������� ������ ��� ��尡 ���� ���(Ȧ��)
	if( m_vecTenderGuild.size() % 2 != 0 )
	{
		it = m_vecTenderGuild.end(); it--;	// ������ ����		// ��Ʈ :  rbegin()�� ����Ͻÿ�.
		CGuild* pGuild = g_GuildMng.GetGuild( (*it).ulGuildId );
		if( pGuild )
		{
			FLWSUser* pUser = (FLWSUser*)prj.GetUserByID( pGuild->m_idMaster );
			if( IsValidObj( pUser ) )
				pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_FAILTENDER );
		}

		g_dpDBClient.SendGC1to1Tender( 'U', (*it).ulGuildId, (*it).nPenya, 'F' );
		m_vecTenderFailGuild.push_back( (*it) );
		m_vecTenderGuild.erase( it );
	}

	// ���� ������ ����� ��� �������� OPEN STATE �˸�(��� �����ʹ� ������ ���� �ð� ���)
	for( it=m_vecTenderGuild.begin(); it!=m_vecTenderGuild.end(); it++ )
	{
		CGuild* pGuild = g_GuildMng.GetGuild( (*it).ulGuildId );
		if( pGuild )
		{
			for( map<u_long, CGuildMember*>::iterator it2=pGuild->m_mapPMember.begin(); it2!=pGuild->m_mapPMember.end(); it2++ )
			{
				FLWSUser* pUser = (FLWSUser*)prj.GetUserByID( it2->first );
				if( IsValidObj( pUser ) )
					SendNowState( pUser );
			}
		}
	}

	g_dpDBClient.SendGC1to1StateToDBSrvr();	// 1:1�������� ���µǾ��ٰ� DBServer�� �˸�
}

// �� �������� 1:1������ ����
void	CGuildCombat1to1Mng::GuildCombat1to1War()
{
	ClearTime();
	m_nState = GC1TO1_WAR;
	for( int i=0; i<(int)( m_vecTenderGuild.size() ); i++ )
	{
		for( int j=0; j<(int)( m_vecTenderGuild.at( i ).vecMemberId.size() ); j++ )
		{
			FLWSUser* pUser = (FLWSUser*)prj.GetUserByID( m_vecTenderGuild.at( i ).vecMemberId.at( j ) );
			if( IsValidObj( pUser ) )
				SendNowState( pUser );
		}
	}

	for( int i=0; i<(int)( m_vecGuilCombat1to1.size() ); i++ )
	{
		DWORD dwWorldId = m_vecTenderGuild.at( m_vecGuilCombat1to1.at( i ).m_nIndex[0] ).dwWorldId;
		u_long uGuildId_0 = m_vecTenderGuild.at( m_vecGuilCombat1to1.at( i ).m_nIndex[0] ).ulGuildId;
		u_long uGuildId_1 = m_vecTenderGuild.at( m_vecGuilCombat1to1.at( i ).m_nIndex[1] ).ulGuildId;
		g_dpDBClient.SendGC1to1WarGuild( dwWorldId, uGuildId_0, uGuildId_1, 'W' );

		m_vecGuilCombat1to1.at( i ).GuildCombat1to1WarWait();
	}
}

// ��� �������� �������� ������.
void	CGuildCombat1to1Mng::GuildCombat1to1AllClose()
{
	ClearTime();
	m_nState = GC1TO1_CLOSE;
	for( int i=0; i<(int)( m_vecTenderGuild.size() ); i++ )
	{
		CGuild* pGuild = g_GuildMng.GetGuild( m_vecTenderGuild.at( i ).ulGuildId );
		if( pGuild )
		{
			for( map<u_long, CGuildMember*>::iterator it=pGuild->m_mapPMember.begin(); it!=pGuild->m_mapPMember.end(); it++ )
			{
				FLWSUser* pUser = (FLWSUser*)prj.GetUserByID( it->first );
				if( IsValidObj(pUser) )
					SendNowState( pUser );
			}
		}
	}

	m_vecGuilCombat1to1.clear();
	m_vecTenderGuild.clear();
	g_dpDBClient.SendGC1to1StateToDBSrvr();
}

// ���� ��û �����츦 �����Ҷ� 
void	CGuildCombat1to1Mng::SendTenderGuildOpenWnd( FLWSUser* pUser )
{
	if( !g_eLocal.GetState( EVE_GUILDCOMBAT1TO1 ) )
		return;
	
	if( m_nState != GC1TO1_CLOSE )	// ������ �� ����.
	{
		pUser->AddDiagText( prj.GetText( TID_GAME_GUILDCOMBAT1TO1_NOTENDER ) );
		return;
	}
	
	// ��帶���͸� ���� ����
	if( !IsGuildMaster( pUser ) )
	{
		pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_NOTMASTER );
		return;
	}

	// �������� ������ ���� ������ �� ����.
	if( IsGCTenderGuild( pUser ) )
	{
		pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_ISGCTENDER );
		return;
	}

	// ������ �� �ִ� �ּ� ��巹�� �˻�...
	if( pUser->GetGuild()->m_nLevel < m_nGuildLevel )
	{
		pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_LOWGUILDLEVEL, "%d", m_nGuildLevel );
		return;
	}

	int nIndex = GetTenderGuildIndexByUser( pUser );
	if( nIndex != NULL_ID )
		pUser->AddGC1to1TenderOpenWnd( m_vecTenderGuild.at( nIndex ).nPenya );
	else
		pUser->AddGC1to1TenderOpenWnd( 0 );
}

// ���� ��Ȳ ����
void	CGuildCombat1to1Mng::SendTenderGuildView( FLWSUser* pUser )
{
	if( !g_eLocal.GetState( EVE_GUILDCOMBAT1TO1 ) )
		return;

	if( m_nState != GC1TO1_CLOSE )
	{
		pUser->AddDiagText( prj.GetText( TID_GAME_GUILDCOMBAT1TO1_NOTENDER ) );
		return;
	}

	int nTenderPenya = 0;
	int nTenderRanking = 0;

	// ������ ����� �����̸�..
	int nIndex = GetTenderGuildIndexByUser( pUser );
	if( nIndex != NULL_ID )
	{
		nTenderPenya = m_vecTenderGuild.at( nIndex ).nPenya;
		nTenderRanking = nIndex+1;
	}
		
	time_t t = 0;
	if( m_Lua.GetLuaFunction( "GetRemainNextTime" ) )	// ���±��� ���� �ð� ����(���)
	{
		m_Lua.CallLuaFunction( 0, 1 );
		t = (time_t)( m_Lua.ToNumber( -1 ) );
	}
	m_Lua.Pop(0);
	
	pUser->AddGC1to1TenderGuildView( nTenderPenya, nTenderRanking, t, m_vecTenderGuild );
}

// ���� �� ������ ������ ����
void	CGuildCombat1to1Mng::SetTenderGuild( FLWSUser* pUser, int nPenya )
{
	if( !g_eLocal.GetState( EVE_GUILDCOMBAT1TO1 ) )
		return;
	
	if( m_nState != GC1TO1_CLOSE )
	{
		pUser->AddDiagText( prj.GetText( TID_GAME_GUILDCOMBAT1TO1_NOTENDER ) );
		return;
	}
	
	// �������� ������ ���� ������ �� ����.
	if( IsGCTenderGuild( pUser ) )
	{
		pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_ISGCTENDER );
		return;
	}
	
	// ��帶���͸� ���� ����
	if( !IsGuildMaster( pUser ) )
	{
		pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_NOTMASTER );
		return;
	}

	// ������ �� �ִ� �ּ� ��巹�� �˻�...
	if( pUser->GetGuild()->m_nLevel < m_nGuildLevel )
	{
		pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_LOWGUILDLEVEL, "%d", m_nGuildLevel );
		return;
	}
	
	// �ּ� ���� ��� ���� ���ƾ� �Ѵ�.
	if( nPenya < m_nJoinPenya )
	{
		pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_LIMIT_MIN );
		return;
	}

	// �α� ���� �غ�(�����۷α�)
	LogItemInfo aLogItem;
	//aLogItem.Action = "9";
	//aLogItem.SendName = pUser->GetName();
	//aLogItem.RecvName = "GUILDCOMBAT1TO1";
	FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "9" );
	FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
	FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "GUILDCOMBAT1TO1" );
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	//aLogItem.ItemName = "SEED";
	FLSPrintf( aLogItem.kLogItem.szItemName, _countof( aLogItem.kLogItem.szItemName ), "%d", ITEM_INDEX( 12, II_GOLD_SEED1 ) );
	
	int nIndex = GetTenderGuildIndexByUser( pUser );
	if( nIndex != NULL_ID ) // �̹� ������ ���� ������ �ݾ����� �����ؾ� �Ѵ�.
	{
		if( m_vecTenderGuild.at( nIndex ).nPenya < nPenya )
		{
			if( (__int64)( nPenya ) > (__int64)( pUser->GetGold() + m_vecTenderGuild.at( nIndex ).nPenya ) )
			{
				pUser->AddDefinedText( TID_GAME_LACKMONEY );	// ��İ� �����ϴ�.
				return;
			}

			pUser->AddGold( m_vecTenderGuild.at( nIndex ).nPenya - nPenya );
			CString strTemp;
			strTemp.Format( prj.GetText( TID_GAME_GUILDCOMBAT1TO1_MORETENDER ), nPenya - m_vecTenderGuild.at( nIndex ).nPenya, nPenya );
			pUser->AddDiagText( strTemp );
			m_vecTenderGuild.at( nIndex ).nPenya = nPenya;
			// �α� ����
			aLogItem.Gold = pUser->GetGold() + (nPenya - m_vecTenderGuild.at( nIndex ).nPenya) ;
			aLogItem.Gold2 = pUser->GetGold();
			aLogItem.Gold_1 = m_vecTenderGuild.at( nIndex ).nPenya - nPenya;
			g_DPSrvr.OnLogItem( aLogItem );
			g_dpDBClient.SendGC1to1Tender( 'U', pUser->m_idGuild, nPenya, 'T' );
		}
		else // �������� �� ���� ��� ���� �䱸
		{
			pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_MORE_CURRENT_REQUEST );
			return;
		}
	}
	else	// ó�� �����ϴ� ����̸�..
	{
//		if( nPenya > pUser->GetGold() )	// ��� ����
		if( pUser->CheckUserGold( nPenya, false ) == false )
		{
			pUser->AddDefinedText( TID_GAME_LACKMONEY );
			return;
		}

		__GC1TO1TENDER	TenderGuild;
		TenderGuild.ulGuildId = pUser->m_idGuild;
		TenderGuild.nPenya = nPenya;
		m_vecTenderGuild.push_back( TenderGuild );
		pUser->AddGold( -nPenya );
		CString strTemp;
		strTemp.Format( prj.GetText( TID_GAME_GUILDCOMBAT1TO1_TENDERPENYA ), nPenya );
		pUser->AddDiagText( strTemp );
		// �α� ����
		aLogItem.Gold = pUser->GetGold() + nPenya;
		aLogItem.Gold2 = pUser->GetGold();
		aLogItem.Gold_1 = -nPenya;
		g_DPSrvr.OnLogItem( aLogItem );
		g_dpDBClient.SendGC1to1Tender( 'I', pUser->m_idGuild, nPenya, 'T' );
	}

	// ������ ������ ����
	if( m_vecTenderGuild.size() < 2 )
		return;
	for( int i=0; i<(int)( m_vecTenderGuild.size()-1 ); i++ )
	{
		for( int j=0; j<(int)( m_vecTenderGuild.size()-i-1 ); j++ )
		{
			if( m_vecTenderGuild.at( j ).nPenya < m_vecTenderGuild.at( j+1 ).nPenya )
			{
				__GC1TO1TENDER temp = m_vecTenderGuild.at( j );
				m_vecTenderGuild.at( j ) = m_vecTenderGuild.at( j+1 );
				m_vecTenderGuild.at( j+1 ) = temp;
			}
		}
	}
}

// ���� ��� 
void	CGuildCombat1to1Mng::SetCancelTenderGuild( FLWSUser* pUser )
{
	if( m_nState != GC1TO1_CLOSE )
	{
		pUser->AddDiagText( prj.GetText( TID_GAME_GUILDCOMBAT1TO1_NOTENDER ) );
		return;
	}

	// ��帶���͸� ���� ��� ����
	if( !IsGuildMaster( pUser ) )
	{
		pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_NOTMASTER );
		return;
	}

	CGuild* pGuild = pUser->GetGuild();
	if( pGuild )
	{
		for( vector<__GC1TO1TENDER>::iterator it=m_vecTenderGuild.begin(); it!=m_vecTenderGuild.end(); it++ )
		{
			if( (*it).ulGuildId == pGuild->GetGuildId() )
			{
				int nReturnGold = (int)( (*it).nPenya * ( (float)m_nCancelReturnRate / 100 ) );
				//�κ� �� á���� ����ó�� �ؾ���..
//				int nTotal = pUser->GetGold() + nReturnGold;
//				if( nReturnGold > 0 && nTotal < 0 )	// overflow
				if( pUser->CheckUserGold( nReturnGold, true ) == false )
				{
					pUser->AddDefinedText( TID_GAME_LACKSPACE );
					return;
				}

				pUser->AddGold( nReturnGold );
			
				g_dpDBClient.SendGC1to1Tender( 'U', (*it).ulGuildId, (*it).nPenya, 'C' );
				m_vecTenderGuild.erase( it );
				
				CString strTemp;
				strTemp.Format( prj.GetText( TID_GAME_GUILDCOMBAT1TO1_CANCELTENDER ), nReturnGold );
				pUser->AddDiagText( strTemp );

				// �α� ����(�����۷α�)
				LogItemInfo aLogItem;
				//aLogItem.Action = "9";
				//aLogItem.SendName = pUser->GetName();
				//aLogItem.RecvName = "GUILDCOMBAT1TO1_CANCELRETURN";
				FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "9" );
				FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
				FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "GUILDCOMBAT1TO1_CANCELRETURN" );
				aLogItem.WorldId = pUser->GetWorld()->GetID();
				//aLogItem.ItemName = "SEED";
				FLSPrintf( aLogItem.kLogItem.szItemName, _countof( aLogItem.kLogItem.szItemName ), "%d", ITEM_INDEX( 12, II_GOLD_SEED1 ) );
				aLogItem.Gold = pUser->GetGold() - nReturnGold;
				aLogItem.Gold2 = pUser->GetGold();
				aLogItem.Gold_1 = nReturnGold;
				g_DPSrvr.OnLogItem( aLogItem );
				break;
			}
		}
	}
}

// ���� ���� ����� ��û�� ����
void	CGuildCombat1to1Mng::SetFailedTenderGuild( FLWSUser* pUser )
{
	// ��帶���͸� ���� ���� ����
	if( !IsGuildMaster( pUser ) )
	{
		pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_NOTMASTER );
		return;
	}

	CGuild* pGuild = pUser->GetGuild();
	if( pGuild )
	{
		for( vector<__GC1TO1TENDER>::iterator it=m_vecTenderFailGuild.begin(); it!=m_vecTenderFailGuild.end(); it++ )
		{
			if( (*it).ulGuildId == pGuild->GetGuildId() )
			{
				//int nReturnGold = (int)( (*it).nPenya * ( (float)m_nFailReturnRate / 100 ) );	
				//-->
				const __int64 nnReturnGold = ( __int64 )( (*it).nPenya * ( (float)m_nFailReturnRate / 100 ) );
				if( nnReturnGold > INT_MAX || nnReturnGold < INT_MIN )
				{
					pUser->AddDefinedText( TID_GAME_LACKSPACE );
					return;

				}
				const int nReturnGold		= ( int )nnReturnGold;

				//�κ� �� á���� ����ó�� �ؾ���..
// 				int nTotal = pUser->GetGold() + nReturnGold;
// 				if( nReturnGold > 0 && nTotal < 0 )	// overflow
				if( pUser->CheckUserGold( nReturnGold, true ) == false )
				{
					pUser->AddDefinedText( TID_GAME_LACKSPACE );
					return;
				}

				pUser->AddGold( nReturnGold );
				
				g_dpDBClient.SendGC1to1Tender( 'U', pUser->m_idGuild, (*it).nPenya, 'G' );
				m_vecTenderFailGuild.erase( it );
				
				CString strTemp;
				strTemp.Format( prj.GetText( TID_GAME_GUILDCOMBAT1TO1_FAILRETURN ), nReturnGold );
				pUser->AddDiagText( strTemp );

				// �α� ����(�����۷α�)
				LogItemInfo aLogItem;
				//aLogItem.Action = "9";
				//aLogItem.SendName = pUser->GetName();
				//aLogItem.RecvName = "GUILDCOMBAT1TO1_FAILRETURN";
				FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "9" );
				FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
				FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "GUILDCOMBAT1TO1_FAILRETURN" );
				aLogItem.WorldId = pUser->GetWorld()->GetID();
				//aLogItem.ItemName = "SEED";
				FLSPrintf( aLogItem.kLogItem.szItemName, _countof( aLogItem.kLogItem.szItemName ), "%d", ITEM_INDEX( 12, II_GOLD_SEED1 ) );
				aLogItem.Gold = pUser->GetGold() - nReturnGold;
				aLogItem.Gold2 = pUser->GetGold();
				aLogItem.Gold_1 = nReturnGold;
				g_DPSrvr.OnLogItem( aLogItem );
				return;
			}
		}

		pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_NOTRETURNGUILD );
		return;
	}
}

// ���� ������ �ۼ� ������ ����...
void	CGuildCombat1to1Mng::SendMemberLineUpOpenWnd( FLWSUser* pUser )
{
	if( m_nState != GC1TO1_OPEN )
	{
		pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_NOLINEUP );
		return;
	}
	
	int nIndex = GetTenderGuildIndexByUser( pUser );
	if( nIndex == NULL_ID )
	{
		pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_CANNOT_MAKEUP );
		return;
	}
	
	// ��帶���� �Ǵ� ŷ�ɱ޸� ����
	if( !IsGuildMaster( pUser ) )
	{
		CGuild* pGuild = pUser->GetGuild();
		if( pGuild )
		{
			CGuildMember* pGuildMember = pGuild->GetMember( pUser->m_idPlayer );
			if( pGuildMember->m_nMemberLv != GUD_KINGPIN )
			{
				pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_NOLINEUPAUTH );
				return;
			}
		}
	}

	pUser->AddGC1to1MemberLineUpOpenWnd( m_vecTenderGuild.at( nIndex ).vecMemberId );
}

// ������ ����
void	CGuildCombat1to1Mng::SetMemberLineUp( FLWSUser* pUser, vector<u_long>& vecMemberId )
{
	if( m_nState != GC1TO1_OPEN )	// ������ ���� �ð��� �ƴϴ�.
	{
		pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_NOLINEUP );
		return;
	}

	int nIndex = GetTenderGuildIndexByUser( pUser );
	if( nIndex == NULL_ID )	// �������� �ʾҰų� �������� �и� ���
	{
		pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_CANNOT_MAKEUP );
		return;
	}

	if( (int)( vecMemberId.size() ) < m_nMinJoinPlayer )	// �ּ� ������ ������ ����.
	{
		pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_LOWMINPLAYER, "%d", m_nMinJoinPlayer );
		return;
	}

	if( (int)( vecMemberId.size() ) > m_nMaxJoinPlayer )
	{
		pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_OVERMAXPLAYER, "%d", m_nMaxJoinPlayer );
		return;
	}

	// ��帶���� �Ǵ� ŷ�ɱ޸� ����
	CGuild* pGuild = pUser->GetGuild();
	if( pGuild )
	{
		CGuildMember* pGuildMember = pGuild->GetMember( pUser->m_idPlayer );
		if( pGuildMember )
		{
			if( pGuildMember->m_nMemberLv != GUD_MASTER && pGuildMember->m_nMemberLv != GUD_KINGPIN )
			{
				pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_NOLINEUPAUTH );
				return;
			}
		}
	}
			
	BOOL bMasterOrKingpin = FALSE;
	// �ּ� ���� ���� �˻�
	for( int i=0; i<(int)( vecMemberId.size() ); i++ )
	{
		FLWSUser* pUserTemp = (FLWSUser*)prj.GetUserByID( vecMemberId.at( i ) );
		if( IsValidObj( pUserTemp ) && pUserTemp->GetLevel() < m_nMinJoinPlayerLevel )
		{
			pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_LIMIT_LEVEL_NOTICE );
			return;
		}
		if( IsValidObj( pUserTemp ) && !bMasterOrKingpin )
		{
			CGuildMember* pGuildMember = pGuild->GetMember( pUserTemp->m_idPlayer );
			if( pGuildMember )
				if( pGuildMember->m_nMemberLv == GUD_MASTER || pGuildMember->m_nMemberLv == GUD_KINGPIN )
					bMasterOrKingpin = TRUE;	
		}
	}
	if( !bMasterOrKingpin )
	{
		pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_HAVENOT_MASTER );
		return;
	}
			
	m_vecTenderGuild.at( nIndex ).vecMemberId.clear();
	m_vecTenderGuild.at( nIndex ).vecMemberId = vecMemberId;
	pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_LINEUPCOMPLETE );
	g_dpDBClient.SendGC1to1LineUp( m_vecTenderGuild.at( nIndex ).ulGuildId, vecMemberId );
}

// ������ ���...
void	CGuildCombat1to1Mng::GuildCombat1to1EntranceWait()
{
	m_nState = GC1TO1_ENTRANCE;
	m_nWaitTime = (int)m_Lua.GetGlobalNumber( "EntranceWaitTime" );
	m_dwTime = GetTickCount();

	for( int i=0; i<(int)( m_vecTenderGuild.size() ); i++ )
	{
		CGuild* pGuild = g_GuildMng.GetGuild( m_vecTenderGuild.at( i ).ulGuildId );
		if( pGuild )
		{
			FLWSUser* pUser = (FLWSUser*)prj.GetUserByID( pGuild->m_idMaster );
			if( IsValidObj( pUser ) && !IsLineUpMember( pUser ) )
				SendNowState( pUser );
		}

		for( int j=0; j<(int)( m_vecTenderGuild.at( i ).vecMemberId.size() ); j++ )
		{
			FLWSUser* pUser = (FLWSUser*)prj.GetUserByID( m_vecTenderGuild.at( i ).vecMemberId.at( j ) );
			if( IsValidObj( pUser ) )
			{
				pUser->AddDefinedCaption( TRUE, TID_GAME_GUILDCOMBAT1TO1_ENTRANCETIME, "" );
				SendNowState( pUser );
			}
		}
	}
}

// �����ϱ� ���� NPC���� �ڷ���Ʈ
void	CGuildCombat1to1Mng::SetTeleportToNPC( FLWSUser* pUser )
{
	if( m_nState != GC1TO1_ENTRANCE )
	{
		pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_NOENTRANCETIME );
		return;
	}
	
	PRegionElem pRgnElem = g_WorldMng.GetRevivalPos( WI_WORLD_MADRIGAL, "flaris" );
	if( pRgnElem )
		pUser->REPLACE( g_uIdofMulti, WI_WORLD_MADRIGAL, D3DXVECTOR3( 6983.0f, 0.0f, 3326.0f ), REPLACE_NORMAL, nDefaultLayer );
}

// ������ ���Ƿ� ����
void	CGuildCombat1to1Mng::SetTeleportToStage( FLWSUser* pUser )
{
	if( m_nState != GC1TO1_ENTRANCE )
	{
		pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_NOENTRANCETIME );
		return;
	}
	
	if( !IsLineUpMember( pUser ) )
	{
		pUser->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_NOT_GUILD_MEMBER );
		return;
	}

	int nIndex = GetTenderGuildIndexByUser( pUser );
	if( nIndex == NULL_ID )
		return;
	int nStageId = m_vecTenderGuild.at( nIndex ).nStageId;
	if( nStageId == NULL_ID )
		return;
	m_vecGuilCombat1to1.at( nStageId ).SetTeleportToWaitStage( pUser );
	
	// ���� ���ֱ�
	pUser->RemoveCommonBuffs();
	g_xWSUserManager->AddRemoveAllSkillInfluence( pUser );
	// ��Ƽ Ż��
	if( pUser->GetPartyId() > 0 )
		g_DPCoreClient.SendGCRemoveParty( pUser->GetPartyId(), pUser->m_idPlayer );
}	
#endif // __WORLDSERVER
