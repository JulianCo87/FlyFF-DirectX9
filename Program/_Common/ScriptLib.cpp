#include "stdafx.h"
#include "party.h"
#include "../WorldServer/User.h"
#include "guildquest.h"
#include "worldmng.h"
#include "guild.h"
#include "ScriptHelper.h"
#if defined(__REMOVE_SCIRPT_060712)
	#include "../WorldServer/WorldDialog.h"
#endif

extern	CPartyMng			g_PartyMng;
extern	CWorldMng			g_WorldMng;
extern	CGuildMng			g_GuildMng;


#if defined(__REMOVE_SCIRPT_060712)

Functions		g_functions;

// npc�̸��� Ű�� �̿��ؼ� �Լ��� �ִ��� ã�´�.
BOOL HasKey( NPCDIALOG_INFO* pInfo, LPCTSTR szKey )
{	
	return CWorldDialog::GetInstance().Find( pInfo->GetName(), szKey );
}

int APIENTRY SayName( NPCDIALOG_INFO* /*pInfo*/ )
{
	return 1;
}

int APIENTRY Speak( NPCDIALOG_INFO* /*pInfo*/, int nId, LPCTSTR lpszStr )
{
	CMover* pMover = prj.GetMover(nId);
	if(pMover && pMover->GetType() == OT_MOVER)
	{
		g_xWSUserManager->AddChat( (CCtrl*)pMover, lpszStr );
	}
	return 1;
}

int APIENTRY SpeakName( NPCDIALOG_INFO* /*pInfo*/ )
{
	return 1;
}

int APIENTRY GetVal( NPCDIALOG_INFO* pInfo )
{
	return pInfo->GetReturn();
}

int APIENTRY GetSrcId( NPCDIALOG_INFO* pInfo )
{
	return pInfo->GetPcId();
}

int APIENTRY GetDstId( NPCDIALOG_INFO* pInfo )
{
	return pInfo->GetNpcId();
}

int APIENTRY NpcId( NPCDIALOG_INFO* pInfo )
{
	return GetDstId( pInfo );
}

int APIENTRY PcId( NPCDIALOG_INFO* pInfo )
{
	return GetSrcId( pInfo );
}

int APIENTRY GetGValue_( NPCDIALOG_INFO* /*pInfo*/ )
{
	return 1;
}
int APIENTRY SetGValue( NPCDIALOG_INFO* /*pInfo*/ )
{
	return 1;
}
int APIENTRY AddGValue( NPCDIALOG_INFO* /*pInfo*/ )
{
	return 1;
}

int APIENTRY AddKey( NPCDIALOG_INFO* pInfo, LPCTSTR szWord, LPCTSTR szKey, DWORD dwParam )
{
	dwParam = 0;
	CString strWord, strKey;

	strWord = szWord;
	strKey = szKey;

	if( strKey.IsEmpty() )	
		strKey = strWord;

	if( HasKey( pInfo, strKey ) == FALSE ) 
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "key error AddKey %s %s" ), pInfo->GetName(), strKey );
		return 1;
	}
	__AddKey( pInfo->GetPcId(), strWord, strKey, dwParam );
	return 1;
}

int APIENTRY AddCondKey( NPCDIALOG_INFO* pInfo, LPCTSTR szWord, LPCTSTR szKey, DWORD dwParam )
{
	dwParam = 0;
	CString strWord, strKey;

	strWord = szWord;
	strKey = szKey;

	if( strKey.IsEmpty() )	
		strKey = strWord;

	if( HasKey( pInfo, strKey ) == FALSE ) 
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "key error %s %s" ), pInfo->GetName(), strKey );
		return 1;
	}
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );
	CMover* pMover = prj.GetMover( pInfo->GetNpcId() );

	for( size_t i = 0; i < pUser->m_nQuestSize; i++ )
	{
		QuestProp* pQuestProp = pUser->m_aQuest[ i ].GetProp();
		if( pQuestProp )
		{
			if( strcmp( pQuestProp->m_szEndCondDlgCharKey, pMover->m_szCharacterKey ) == 0 )
			{
				if( strcmp( pQuestProp->m_szEndCondDlgAddKey, strKey ) == 0 )
				{
					__AddKey( pInfo->GetPcId(), strWord, strKey, dwParam );
					return 1;
				}
			}
		}
	}
	return 0;
}

int APIENTRY AddAnswer( NPCDIALOG_INFO*  pInfo, LPCTSTR szWord, LPCTSTR szKey, DWORD dwParam )
{
	DWORD nParam1, nParam2;
	CString strWord, strKey;

	strWord = szWord;
	strKey = szKey;

	nParam1 = dwParam;
	nParam2 = 0;

	if( strKey.IsEmpty() )	
		strKey = strWord;

	if( HasKey( pInfo, strKey ) == FALSE ) 
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "key error AddAnswer %s %s" ), pInfo->GetName(), strKey );
		return 1;
	}
	return __AddAnswer( pInfo->GetPcId(), strWord, strKey, nParam1, nParam2 );
}

int APIENTRY RemoveQuest( NPCDIALOG_INFO* pInfo, int nQuest )      
{
	return __RemoveQuest( pInfo->GetPcId(), nQuest );
}

int APIENTRY RemoveAllKey(NPCDIALOG_INFO*  pInfo)
{
	__RemoveAllKey( pInfo->GetPcId() );
	return 1;
}

int APIENTRY SayQuest( NPCDIALOG_INFO* pInfo, int nQuest, int nDialog )
{
	__SayQuest( pInfo->GetPcId(), nQuest, nDialog );
	return 1;
}

int APIENTRY BeginQuest( NPCDIALOG_INFO* pInfo, int nQuest )
{
	return __RunQuest( pInfo->GetPcId(), pInfo->GetNpcId(), nQuest );
}

int APIENTRY EndQuest( NPCDIALOG_INFO* pInfo, int nQuest )
{
	return __EndQuest( pInfo->GetPcId(), nQuest, FALSE );
}

int APIENTRY AddQuestKey( NPCDIALOG_INFO* pInfo, int nQuest, LPCTSTR szKey )
{
	int nParam = 0;
	CString strKey = szKey;

	if( strKey.IsEmpty() )
	{
		QuestProp* pQuestProp = prj.m_aPropQuest.GetAt( nQuest );
		if( pQuestProp )
			strKey = pQuestProp->m_szTitle;
	}

	if( HasKey( pInfo, strKey ) == FALSE ) 
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "key error AddQuestKey %s %s" ), pInfo->GetName(), strKey );
		return 1;
	}

	return __AddQuestKey( pInfo->GetPcId(), nQuest, strKey, nParam );
}


int APIENTRY CreateMover( NPCDIALOG_INFO* /*pInfo*/ )
{
	return 1;
}     

int APIENTRY CreateChar( NPCDIALOG_INFO*  /*pInfo*/ )
{
	return 1;
}    

int APIENTRY RemoveMover( NPCDIALOG_INFO*  /*pInfo*/ )
{
	return 1;
}   

int APIENTRY GetDay( NPCDIALOG_INFO* /*pInfo*/ )
{
	return g_GameTimer.m_nDay;
}        

int APIENTRY GetMin( NPCDIALOG_INFO* /*pInfo*/ )
{
	return g_GameTimer.m_nMin;
}      

int APIENTRY GetHour( NPCDIALOG_INFO* /*pInfo*/ )
{
	return g_GameTimer.m_nHour;
}       
 
int APIENTRY SetTimer( NPCDIALOG_INFO*  /*pInfo*/ )  
{
	return 1;
}
int APIENTRY IsTimeOut( NPCDIALOG_INFO*  /*pInfo*/ )
{
	return 1;
}

int APIENTRY LaunchQuest( NPCDIALOG_INFO* pInfo )
{
	int* p = pInfo->GetGlobal();
	*p = 0;
	__QuestEnd( pInfo->GetPcId(), pInfo->GetNpcId(), *p );
	return 1;
}
int APIENTRY GetEmptyInventoryNum( NPCDIALOG_INFO* pInfo, DWORD dwItemId )
{
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );
	return pUser->m_Inventory.GetEmptyCountByItemId( dwItemId );
}

int APIENTRY GetQuestState( NPCDIALOG_INFO* pInfo, int nQuest )
{
	FLWSUser* pUser	= prj.GetUser( pInfo->GetPcId() );
	LPQUEST pQuest = pUser->GetQuest( nQuest );
	if( pQuest ) 
		return pQuest->m_nState;
	QUEST quest;
	if( pUser->MakeCompleteQuest( nQuest, &quest ) )
		return quest.m_nState;
	return -1;
}
int APIENTRY IsSetQuest( NPCDIALOG_INFO* pInfo, int nQuest )
{
	FLWSUser* pUser	= prj.GetUser( pInfo->GetPcId() );
	LPQUEST pQuest = pUser->GetQuest( nQuest );
	if( pQuest ) return TRUE;
	if( pUser->IsCompleteQuest( nQuest ) )
		return TRUE;
	return FALSE;
}

int APIENTRY SetMark( NPCDIALOG_INFO* /*pInfo*/ )
{
	return 1;
}
int APIENTRY GoMark( NPCDIALOG_INFO* /*pInfo*/ )
{
	return 1;
}

int APIENTRY RandomIndex( NPCDIALOG_INFO* /*pInfo*/) 
{
	return 1;
}

int APIENTRY RemoveKey(NPCDIALOG_INFO* pInfo, LPCTSTR szKey )
{
	if( HasKey(pInfo, szKey) ) 
	{
		FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );

		RunScriptFunc rsf;
		rsf.wFuncType = FUNCTYPE_REMOVEKEY;
		FLStrcpy( rsf.lpszVal1, _countof( rsf.lpszVal1 ), szKey );
		pUser->AddRunScriptFunc( rsf );
	}
	return 1;
}

int APIENTRY Say(NPCDIALOG_INFO* pInfo, LPCTSTR szMsg )
{
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );
	RunScriptFunc rsf;
	rsf.wFuncType = FUNCTYPE_SAY;
	FLStrcpy( rsf.lpszVal1, _countof( rsf.lpszVal1 ), szMsg );
	rsf.dwVal2 = 0;
	pUser->AddRunScriptFunc( rsf );
	return 1;
}

int APIENTRY EndSay( NPCDIALOG_INFO*  /*pInfo*/ )
{
	return 1;
}

int APIENTRY Random( NPCDIALOG_INFO*  /*pInfo*/, int n )
{
	return rand() % n;
}

int APIENTRY Exit( NPCDIALOG_INFO* pInfo )
{
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );
	RunScriptFunc rsf;
	rsf.wFuncType = FUNCTYPE_EXIT;
	pUser->AddRunScriptFunc( rsf );
	return TRUE;
}

int APIENTRY  SetScriptTimer( NPCDIALOG_INFO* /*pInfo*/, int /*nTimer*/ )
{
	return 1;
}

// Replace( int nWorld, int x, int y, int z )
int APIENTRY Replace( NPCDIALOG_INFO*  pInfo, int nWorld, float x, float y, float z )
{
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );

	if( x > 0.0f && z > 0.0f )
	{
		RunScriptFunc rsf;
		rsf.wFuncType = FUNCTYPE_EXIT;
		pUser->AddRunScriptFunc( rsf );
		if( g_eLocal.GetState( EVE_SCHOOL ) )
			g_dpDBClient.SendLogSchool( pUser->m_idPlayer, pUser->GetName() );

		pUser->REPLACE( g_uIdofMulti, nWorld, D3DXVECTOR3( x, y, z ), REPLACE_NORMAL, nTempLayer );
	}
	return 1;
}

int APIENTRY ReplaceKey( NPCDIALOG_INFO* pInfo, int nWorld, LPCTSTR szKey )
{
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );
	RunScriptFunc rsf;	
	rsf.wFuncType = FUNCTYPE_EXIT;
	pUser->AddRunScriptFunc( rsf );

	PRegionElem pRgnElem = g_WorldMng.GetRevivalPos( nWorld, szKey );
	if( pRgnElem )
		pUser->REPLACE( g_uIdofMulti, pRgnElem->m_dwWorldId, pRgnElem->m_vPos, REPLACE_NORMAL, nRevivalLayer );
	return 1;
}

int APIENTRY GetPlayerLvl( NPCDIALOG_INFO* pInfo )  
{
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );
	return pUser->GetLevel();
}

int APIENTRY GetPlayerJob( NPCDIALOG_INFO* pInfo )  
{
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );
	return pUser->GetJob();
}

int APIENTRY IsGuildQuest( NPCDIALOG_INFO* pInfo, int nQuest )
{
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );

	CGuild* pGuild	= pUser->GetGuild();
	if( pGuild )
	{
		PGUILDQUEST pQuest = pGuild->GetQuest( nQuest );
		if( pQuest )
			return TRUE;
	}
	return FALSE;
}

int APIENTRY GetGuildQuestState( NPCDIALOG_INFO* pInfo, int nQuest )
{
	FLWSUser* pUser	= prj.GetUser( pInfo->GetPcId() );
	CGuild* pGuild	= pUser->GetGuild();
	if( pGuild )
	{
		PGUILDQUEST pQuest	= pGuild->GetQuest( nQuest );
		if( pQuest )
		{
			int nState = pQuest->nState;
			return nState;
		}
	}
	return -1;
}

int APIENTRY IsWormonServer( NPCDIALOG_INFO* /*pInfo*/ )
{
	return g_eLocal.GetState( EVE_WORMON );
}

int APIENTRY PrintSystemMessage( NPCDIALOG_INFO* pInfo, int nText )
{
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );
	pUser->AddDefinedText( (DWORD)nText );
	return 1;
}

int APIENTRY  MonHuntStart( NPCDIALOG_INFO* pInfo, int nQuest, int nState, int nState2, int n )
{
	CGuildQuestProcessor* pProcessor	= CGuildQuestProcessor::GetInstance();
	if( pProcessor->IsQuesting( nQuest ) )
	{
		return 0;
	}
	else
	{
		FLWSUser* pUser	= prj.GetUser( pInfo->GetPcId() );
		CGuild* pGuild	= pUser->GetGuild();
		if( pGuild )
		{
			GUILDQUESTPROP* pProp	= prj.GetGuildQuestProp( nQuest );
			if( pProp )
			{
				CMover* pWormon	= (CMover*)CreateObj( D3DDEVICE, OT_MOVER, pProp->dwWormon );
				if( pWormon )
				{
					pWormon->SetPos( pProp->vPos );
					pWormon->InitMotion( MTI_STAND );
					pWormon->UpdateLocalMatrix();
					pWormon->m_nQuestKeeping	= static_cast< BYTE >( nQuest );

					CWorld* pWorld = pUser->GetWorld();
					if( pWorld )
					{
						RunScriptFunc rsf;	// Exit()
						rsf.wFuncType = FUNCTYPE_EXIT;
						pUser->AddRunScriptFunc( rsf );
						pWormon->AddItToGlobalId();
						if( pWorld->ADDOBJ( pWormon, FALSE, pUser->GetLayer() ) == TRUE )
						{
							pGuild->SetQuest( nQuest, nState );
							if( nState == QS_BEGIN )
								g_dpDBClient.SendInsertGuildQuest( pGuild->m_idGuild, nQuest );
							else
								g_dpDBClient.SendUpdateGuildQuest( pGuild->m_idGuild, nQuest, nState );

							pProcessor->SetGuildQuest( nQuest, nState, nState2, n, pGuild->m_idGuild, pWormon->GetId() );
							D3DXVECTOR3 vPos( pProp->vPos.x, pProp->vPos.y, ( ( pProp->vPos.z * 2 ) + pProp->y2 ) / 3 );
							pGuild->Replace( pProp->dwWorldId, vPos, TRUE );
							return 1;
						}
						else
						{
							SAFE_DELETE( pWormon );
							return 0;
						}
					}
					else
					{
						SAFE_DELETE( pWormon );
						return 0;
					}
				}
			}
		}
	}
	return 0;
}

int APIENTRY  MonHuntStartParty( NPCDIALOG_INFO*  pInfo, int nQuest, int nState, int nState2, int n )
{
	CPartyQuestProcessor* pProcessor	= CPartyQuestProcessor::GetInstance();
	if( pProcessor->IsQuesting( nQuest ) )
	{
		return 0;
	}
	else
	{
		FLWSUser* pUser	= prj.GetUser( pInfo->GetPcId() );

		CParty* pParty	= g_PartyMng.GetParty( pUser->GetPartyId() );
		if( pParty )
		{
			PARTYQUESTPROP* pProp	= prj.GetPartyQuestProp( nQuest );
			if( pProp )
			{				
				if( pParty->ReplaceChkLv( pProp->nLevel+5 ) == FALSE )
				{
					CString str;
					str.Format( prj.GetText(TID_GAME_PARTYQUEST_NOT_JOIN), pProp->nLevel+5 );
					pUser->AddDiagText( str );

					return 0;
				}

				// ������ ����ִ�...��Ƽ�� ���� �÷��̾��... ������ ��ȯ��Ų��...
//				g_xWSUserManager->ReplaceWorldArea( pProp->dwWorldId, WI_WORLD_MADRIGAL, 6968.0f, 3328.8f, 0.5f, FALSE );				
				// �ϴ� ����
				pProcessor->RemoveAllDynamicObj( pProp );
				
				BOOL bSuccess = FALSE;
				for( DWORD i=0; i<pProp->vecWormon.size(); i++ )
				{
					WORMON* WorMon;
					WorMon = &(pProp->vecWormon.at( i ));
					CMover* pWormon	= (CMover*)CreateObj( D3DDEVICE, OT_MOVER, WorMon->dwWormon );

					if( pWormon )
					{
						pWormon->SetPos( WorMon->vPos );
						pWormon->InitMotion( MTI_STAND );
						pWormon->UpdateLocalMatrix();
						pWormon->m_nPartyQuestKeeping = static_cast< BYTE >( nQuest );

						CWorld* pWorld	= g_WorldMng.GetWorld( pProp->dwWorldId );
						if( pWorld )
						{
							RunScriptFunc rsf;	// Exit()
							rsf.wFuncType		= FUNCTYPE_EXIT;
							pUser->AddRunScriptFunc( rsf );
							pWormon->AddItToGlobalId();
							pWormon->m_bActiveAttack = TRUE;
							if( pWorld->ADDOBJ( pWormon, FALSE, pUser->GetLayer() ) == TRUE )
							{
								bSuccess = TRUE;
							}
							else
							{
								SAFE_DELETE( pWormon );
								bSuccess = FALSE;
							}
						}
						else
						{
							SAFE_DELETE( pWormon );
							bSuccess = FALSE;
						}
					}
				}
				
				if( bSuccess )
				{
					pProcessor->SetPartyQuest( nQuest, nState, nState2, n, pUser->GetPartyId(), 0 );
					pParty->Replace( pProp->dwWorldId, pProp->szWorldKey );
									
					return 1;
				}
			}
		}
	}
	return 0;
}

int APIENTRY InitStat( NPCDIALOG_INFO*  pInfo )
{
	FLWSUser* pUser	= prj.GetUser( pInfo->GetPcId() );
	pUser->m_Stat.SetOriginStr( DEFAULT_ORIGIN_STAT_VALUE );
	pUser->m_Stat.SetOriginInt( DEFAULT_ORIGIN_STAT_VALUE );
	pUser->m_Stat.SetOriginDex( DEFAULT_ORIGIN_STAT_VALUE );
	pUser->m_Stat.SetOriginSta( DEFAULT_ORIGIN_STAT_VALUE );

	pUser->m_Stat.SetRemainStatPoint( ( pUser->GetLevel() - 1 ) * 2 );
	RunScriptFunc rsf;
	rsf.wFuncType		= FUNCTYPE_INITSTAT;
	rsf.dwVal1	= pUser->m_Stat.GetRemainStatPoint();
	pUser->AddRunScriptFunc( rsf );

	g_xWSUserManager->AddSetState( pUser );
	return 1;
}

int	APIENTRY InitStr( NPCDIALOG_INFO*  pInfo )
{
	FLWSUser* pUser	= prj.GetUser( pInfo->GetPcId() );
	int nStr = pUser->m_Stat.GetOriginStr();
	if( nStr - DEFAULT_ORIGIN_STAT_VALUE > 0 )
	{
		pUser->m_Stat.SetOriginStr( DEFAULT_ORIGIN_STAT_VALUE );

		pUser->m_Stat.SetRemainStatPoint( pUser->m_Stat.GetRemainStatPoint() + nStr - DEFAULT_ORIGIN_STAT_VALUE );
	}
	
	RunScriptFunc rsf;
	rsf.wFuncType		= FUNCTYPE_INITSTR;
	rsf.dwVal1	= pUser->m_Stat.GetRemainStatPoint();
	pUser->AddRunScriptFunc( rsf );

	g_xWSUserManager->AddSetState( pUser );
	return 1;
}
int	APIENTRY InitSta( NPCDIALOG_INFO*  pInfo )
{
	FLWSUser* pUser	= prj.GetUser( pInfo->GetPcId() );
	int nSta = pUser->m_Stat.GetOriginSta();
	if( nSta - DEFAULT_ORIGIN_STAT_VALUE > 0 )
	{
		pUser->m_Stat.SetOriginSta( DEFAULT_ORIGIN_STAT_VALUE );

		pUser->m_Stat.SetRemainStatPoint( pUser->m_Stat.GetRemainStatPoint() + nSta - DEFAULT_ORIGIN_STAT_VALUE );
	}
	
	RunScriptFunc rsf;
	rsf.wFuncType		= FUNCTYPE_INITSTA;
	rsf.dwVal1	= pUser->m_Stat.GetRemainStatPoint();
	pUser->AddRunScriptFunc( rsf );

	g_xWSUserManager->AddSetState( pUser );
	return 1;
}
int	APIENTRY InitDex( NPCDIALOG_INFO*  pInfo )
{
	FLWSUser* pUser	= prj.GetUser( pInfo->GetPcId() );
	int nDex = pUser->m_Stat.GetOriginDex();
	if( nDex - DEFAULT_ORIGIN_STAT_VALUE > 0 )
	{
		pUser->m_Stat.SetOriginDex( DEFAULT_ORIGIN_STAT_VALUE );

		pUser->m_Stat.SetRemainStatPoint( pUser->m_Stat.GetRemainStatPoint() + nDex - DEFAULT_ORIGIN_STAT_VALUE );
	}
	
	RunScriptFunc rsf;
	rsf.wFuncType		= FUNCTYPE_INITDEX;
	rsf.dwVal1	= pUser->m_Stat.GetRemainStatPoint();
	pUser->AddRunScriptFunc( rsf );

	g_xWSUserManager->AddSetState( pUser );
	return 1;
}
int	APIENTRY InitInt( NPCDIALOG_INFO*  pInfo )
{
	FLWSUser* pUser	= prj.GetUser( pInfo->GetPcId() );
	int nInt = pUser->m_Stat.GetOriginInt();
	if( nInt - DEFAULT_ORIGIN_STAT_VALUE > 0 )
	{
		pUser->m_Stat.SetOriginInt( DEFAULT_ORIGIN_STAT_VALUE );

		pUser->m_Stat.SetRemainStatPoint( pUser->m_Stat.GetRemainStatPoint() + nInt - DEFAULT_ORIGIN_STAT_VALUE );
	}
	
	RunScriptFunc rsf;
	rsf.wFuncType		= FUNCTYPE_INITINT;
	rsf.dwVal1	= pUser->m_Stat.GetRemainStatPoint();
	pUser->AddRunScriptFunc( rsf );

	g_xWSUserManager->AddSetState( pUser );
	return 1;
}

int APIENTRY SetQuestState( NPCDIALOG_INFO* pInfo, int nQuest, int n )      
{
	return __SetQuestState( pInfo->GetPcId(), nQuest, n );
}
int APIENTRY SetQuest( NPCDIALOG_INFO* pInfo, int nQuest )      
{
	__SetQuest(  pInfo->GetPcId(), nQuest );
	return 1;
}

int APIENTRY CreateItem( NPCDIALOG_INFO* pInfo, DWORD dwID, int nCount )	
{
	__CreateItem( pInfo->GetPcId(), dwID, nCount );
	return 1;
}

int APIENTRY AddGold( NPCDIALOG_INFO* pInfo, int nGold )     
{
	__AddGold(  pInfo->GetPcId(), nGold );
	return 1;
}

int	APIENTRY RemoveGold( NPCDIALOG_INFO* pInfo, int nGold )
{
	__RemoveGold( pInfo->GetPcId(), nGold );
	return 1;
}

int APIENTRY GetPlayerGold( NPCDIALOG_INFO* pInfo )
{
	FLWSUser* pUser	= prj.GetUser( pInfo->GetPcId() );
	return pUser->GetGold();
}

int APIENTRY GetLocalEventState( NPCDIALOG_INFO* /*pInfo*/, int nEvent )
{
	return g_eLocal.GetState( nEvent );
}

int APIENTRY AddExp( NPCDIALOG_INFO* pInfo, int nExp )      
{
	FLWSUser* pUser	= prj.GetUser( pInfo->GetPcId() );
	if( pUser->AddExperience( nExp, TRUE, FALSE ) )
		pUser->LevelUpSetting();
	else
		pUser->ExpUpSetting();
	pUser->AddSetExperience( pUser->GetExp1(), (WORD)pUser->m_nLevel, pUser->m_nSkillPoint, pUser->m_nSkillLevel );
	pUser->AddDefinedText( TID_GAME_REAPEXP );
	return 1;
}

int APIENTRY IsParty( NPCDIALOG_INFO* pInfo )
{
	FLWSUser* pUser	= prj.GetUser( pInfo->GetPcId() );
		CParty* pParty = g_PartyMng.GetParty( pUser->m_idparty );
		int f = ( pParty != NULL && pParty->IsMember( pUser->m_idPlayer ) );
	return f;
}

int APIENTRY IsPartyMaster( NPCDIALOG_INFO* pInfo )
{
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );
		CParty* pParty	= g_PartyMng.GetParty( pUser->m_idparty );
		int f = ( pParty != NULL && pParty->IsLeader( pUser->m_idPlayer ) );
	return f;
}

int APIENTRY GetPartyNum( NPCDIALOG_INFO* pInfo )
{
	FLWSUser* pUser	= prj.GetUser( pInfo->GetPcId() );
	int nSize = 0;
	CParty* pParty	= g_PartyMng.GetParty( pUser->m_idparty );
	if( pParty )
		nSize = pParty->GetSizeofMember();
	return nSize;
}

int	APIENTRY GetPartyLevel( NPCDIALOG_INFO* pInfo )
{
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );
	int nLevel = 0;
	CParty* pParty = g_PartyMng.GetParty( pUser->m_idparty );
	if( pParty )
		nLevel = pParty->GetLevel();
	return nLevel;
}

int APIENTRY IsPartyGuild( NPCDIALOG_INFO* pInfo )
{
	int f	= 0;
	u_long	aidPlayer[MAX_PTMEMBER_SIZE];
	int	nSize	= 0;
	FLWSUser* pUser	= prj.GetUser( pInfo->GetPcId() );

	CParty* pParty	= g_PartyMng.GetParty( pUser->m_idparty );
	if( pParty )
	{
		nSize	= pParty->m_nSizeofMember;
		for( int i = 0; i < pParty->m_nSizeofMember; i++ )
			aidPlayer[i]	= pParty->m_aMember[i].m_uPlayerId;
	}
	else
	{
		f	= 1;
	}

	for( int i = 0; i < nSize; i++ )
	{
		FLWSUser* pUsertmp	= g_xWSUserManager->GetUserByPlayerID( aidPlayer[i] );
		if( IsValidObj( pUsertmp ) )
		{
			if( pUsertmp->GetGuild() )
			{
				f	= 1;
				break;
			}
			CTime tCurrent = CTime::GetCurrentTime();
			if( tCurrent < pUsertmp->m_tGuildMember )
			{
				f	= 2;
				break;
			}
		}
		else
		{
			f	= 1;
			break;
		}
	}
	return f;
}

int APIENTRY IsGuild( NPCDIALOG_INFO* pInfo )
{
	FLWSUser* pUser	= prj.GetUser( pInfo->GetPcId() );
	CGuild* pGuild	= g_GuildMng.GetGuild( pUser->m_idGuild );
	int f	= ( pGuild && pGuild->IsMember( pUser->m_idPlayer ) );
	return f;
}

int APIENTRY IsGuildMaster( NPCDIALOG_INFO* pInfo )
{
	FLWSUser* pUser	= prj.GetUser( pInfo->GetPcId() );
	CGuild* pGuild	= g_GuildMng.GetGuild( pUser->m_idGuild );
	int f = ( pGuild && pGuild->IsMaster( pUser->m_idPlayer ) );
	return f;
}

int APIENTRY CreateGuild( NPCDIALOG_INFO*  pInfo )
{
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );
	int nSize = 0;
	GUILD_MEMBER_INFO	info[MAX_PTMEMBER_SIZE];

	CParty* pParty	= g_PartyMng.GetParty( pUser->m_idparty );
	if( pParty && pParty->IsLeader( pUser->m_idPlayer ) )
	{
		for( int i = 0; i < pParty->m_nSizeofMember; i++ )
		{
			FLWSUser* pUsertmp	= g_xWSUserManager->GetUserByPlayerID( pParty->GetPlayerId( i ) );
			if( IsValidObj( pUsertmp ) )
			{
				info[nSize].idPlayer	= pUsertmp->m_idPlayer;
				nSize++;
			}
			else
			{
				// error	- offline
				return 0;
			}
		}
	}
	else
	{
		// error	-
		return 0;
	}

	g_DPCoreClient.SendCreateGuild( info, nSize, "" );

	return 1;
}

int APIENTRY DestroyGuild( NPCDIALOG_INFO* pInfo )
{
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );

	CGuild* pGuild	= g_GuildMng.GetGuild( pUser->m_idGuild );
	if( pGuild && pGuild->IsMember( pUser->m_idPlayer ) )
	{
		if( pGuild->IsMaster( pUser->m_idPlayer ) )
		{
			RunScriptFunc rsf;
			rsf.wFuncType	= FUNCTYPE_DESTROYGUILD;
			pUser->AddRunScriptFunc( rsf );
		}
		else
			pUser->AddDefinedText( TID_GAME_COMDELNOTKINGPIN );
	}
	else
		pUser->AddDefinedText( TID_GAME_COMNOHAVECOM );

	return 1;
}

int APIENTRY IsPlayerSkillPoint( NPCDIALOG_INFO*  /*pInfo*/ )     
{
	return 0;
}

int APIENTRY ChangeJob( NPCDIALOG_INFO* pInfo, int nJob )     
{
	int v1	=nJob;
	FLWSUser* pUser	= prj.GetUser( pInfo->GetPcId() );
	if( pUser->IsBaseJob() && pUser->GetLevel() != MAX_JOB_LEVEL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "Error ChangeJob : 1" ) );
		return 0;
	}
	if( pUser->IsExpert() && pUser->GetLevel() < MAX_JOB_LEVEL + MAX_EXP_LEVEL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "Error ChangeJob : 2" ) );
		return 0;
	}
	if( pUser->IsPro() && pUser->GetLevel() < MAX_GENERAL_LEVEL && pUser->GetExpPercent() < 9999 )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "Error ChangeJob : 3" ) );
		return 0;
	}

	if( pUser->IsHero() == TRUE && pUser->GetLevel() < MAX_LEGEND_LEVEL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "Error ChangeJob : 4" ) );
		return 0;
	}

	if( pUser->AddChangeJob( v1 ) )
	{
		pUser->AddSetChangeJob( v1 );
		g_xWSUserManager->AddNearSetChangeJob( (CMover*)pUser, v1, &pUser->m_aJobSkill[MAX_JOB_SKILL] );
		// #dlvr
		g_dpDBClient.SendLogLevelUp( (FLWSUser*)pUser, 4 );
		if( g_eLocal.GetState( EVE_RECOMMEND ) && pUser->IsPlayer() )
			g_dpDBClient.SendRecommend( (FLWSUser*)pUser, v1 );


		g_dpDBClient.SendUpdatePlayerData( pUser );
	}
	else
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "Error ChangeJob : CMover::AddChangeJob" ) );
	}
	return 1;
}

int APIENTRY GetPlayerSex( NPCDIALOG_INFO* pInfo )  
{
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );
	return pUser->GetSex();
}
int APIENTRY GetItemNum( NPCDIALOG_INFO* pInfo, DWORD dwID )    
{
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );
	return pUser->GetItemNum( dwID );
}
int APIENTRY RemoveAllItem( NPCDIALOG_INFO* pInfo, DWORD dwID ) 
{
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );
	pUser->RemoveAllItem( dwID );
	return 1;
}
int APIENTRY EquipItem( NPCDIALOG_INFO* pInfo, DWORD dwID )     
{
	int v1	= dwID;
	FLWSUser* pUser	= prj.GetUser( pInfo->GetPcId() );
	
	DWORD adwItemObjID[MAX_INVENTORY] = { NULL_ID, };
	DWORD dwCount = 0;

	FLItemElem itemElem;
	itemElem.m_dwItemId		= v1;
	itemElem.m_nItemNum		= 1;
	itemElem.SetAbilityOption( 0 );
	if( pUser->CreateItem( &itemElem, adwItemObjID, NULL, &dwCount ) == TRUE )
	{
		for( DWORD i = 0; i < dwCount; ++i )
		{
			FLItemElem* pItemElem	= pUser->m_Inventory.GetAtId( adwItemObjID[ i ] );

			if( pItemElem && TRUE == pUser->DoEquip( pItemElem, TRUE ) )
			{
				g_xWSUserManager->AddDoEquip( (CMover*)pUser, -1, pItemElem, TRUE );
			}

			LogItemInfo aLogItem;
			//aLogItem.Action = "Q";
			//aLogItem.SendName = pUser->GetName();
			//aLogItem.RecvName = "QUEST";
			FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "Q" );
			FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
			FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "QUEST" );

			aLogItem.WorldId = pUser->GetWorld()->GetID();
			aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
			g_DPSrvr.OnLogItem( aLogItem, &itemElem, itemElem.m_nItemNum );
		}
	}
	else
	{
		FLItemElem* pItemElem	= new FLItemElem;
		pItemElem->m_dwItemId	= v1;
		PT_ITEM_SPEC pItemProp = pItemElem->GetProp();
		if( pItemProp != NULL )
		{
			pItemElem->m_nHitPoint	= ( pItemProp->dwEndurance == -1 ) ? 0 : pItemProp->dwEndurance;//pItemProp->dwEndurance;
			pItemElem->m_nItemNum	= 1;
			pItemElem->SetAbilityOption( 0 );
			CItem* pItem	= new CItem;
			pItem->m_pItemBase	= pItemElem;

			if( pItemElem->m_dwItemId == 0 ) FLERROR_LOG( PROGRAM_NAME, _T( "EquipItem SetIndex" ) );
			pItem->SetIndex( D3DDEVICE, pItemElem->m_dwItemId );
			pItem->SetPos( pUser->GetPos() );
			pItem->SetAngle( (float)( xRandom( 360 ) ) );
			pItem->m_idHolder	= pUser->m_idPlayer;
			if( pUser->GetWorld()->ADDOBJ( pItem, TRUE, pUser->GetLayer() ) == TRUE )
			{
				pUser->AddDefinedText( TID_GAME_DROPITEM, "\"%s\"", pItemProp->szName );
			}
			else
			{
				SAFE_DELETE( pItem );
			}
		}
		else
		{
			SAFE_DELETE( pItemElem );
		}
	}
	return 1;
}

int APIENTRY PlaySound(NPCDIALOG_INFO* pInfo, LPCTSTR szMusic )
{
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );
	pUser->AddPlaySound2( szMusic );
	return 1;
}

int APIENTRY DropQuestItem( NPCDIALOG_INFO* pInfo, DWORD dwID, DWORD dwProb )
{
	int v1 = dwID, v2 = dwProb;
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );

	if( (int)( xRandom( 1000 ) ) < v2 )
	{
		PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( v1 );
		if( pItemProp )
		{
			CWorld* pWorld	= pUser->GetWorld();
			if( pWorld )
			{
				FLItemElem* pItemElem	= new FLItemElem;
				pItemElem->m_dwItemId	= v1;
				CItem* pItem	= new CItem;
				pItem->m_pItemBase	= pItemElem;
				if( pItemElem->m_dwItemId == 0 ) FLERROR_LOG( PROGRAM_NAME, _T( "DropQuestItem SetIndex" ) );
				pItem->SetIndex( D3DDEVICE, pItemElem->m_dwItemId );
				pItem->m_idHolder	= pUser->m_idPlayer;
				pItem->m_idOwn	= pUser->GetId();
				pItem->m_dwDropTime	= timeGetTime();
				CMover* pMover	= prj.GetMover( pInfo->GetNpcId() );
				D3DXVECTOR3 vPos	= pMover->GetPos();
				vPos.x	+= ( xRandomF( 2.0f ) - 1.0f );
				vPos.z	+= ( xRandomF( 2.0f ) - 1.0f );
				pItem->SetPos( vPos );
				pItem->SetAngle( (float)( xRandom( 360 ) ) );
				if( pWorld->ADDOBJ( pItem, TRUE, pUser->GetLayer() ) == TRUE )
				{
					return 1;
				}
				else
				{
					SAFE_DELETE( pItem );
					return 0;
				}
			}
		}
		else
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "DropQuestItem, %d null prop" ), v1 );
		}
	}
	return 0;
}

int APIENTRY ExpQuestEffect(NPCDIALOG_INFO* pInfo, DWORD dwID, BOOL bNPC )
{
	int v1 = dwID, v2 = bNPC;
	CCtrl* pCtrl = ( v2? prj.GetCtrl( pInfo->GetNpcId() ): prj.GetCtrl( pInfo->GetPcId() ) );
	g_xWSUserManager->AddCreateSfxObj( pCtrl, v1 );
	return 1;
}

int APIENTRY RemoveItem( NPCDIALOG_INFO* pInfo, DWORD dwID, int nCount )
{
	int v1 = dwID, v2 = nCount;
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );
	return pUser->RemoveItemA( v1, v2 );
}

int APIENTRY Run( NPCDIALOG_INFO* pInfo, LPCTSTR szKey, int n )
{
	int v1 = n;
	CString string = szKey;

	CMover* pMover = prj.GetMover( pInfo->GetNpcId() );
	int nResult = 0;
	pMover->m_pNpcProperty->RunDialog( string, &nResult, v1, pInfo->GetNpcId(), pInfo->GetPcId(), 0 );

	return nResult;
}

int APIENTRY GetQuestId( NPCDIALOG_INFO* pInfo )
{
	return pInfo->GetQuest();
}
int APIENTRY GetLang( NPCDIALOG_INFO* /*pInfo*/ )
{
	return g_xFlyffConfig->GetMainLanguage();
}

// ĳ���� �� ����
int	APIENTRY QuerySetPlayerName( NPCDIALOG_INFO* pInfo )
{
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId()  );
	if( pUser->IsAuthHigher( AUTH_GAMEMASTER ) || pUser->IsMode( QUERYSETPLAYERNAME_MODE ) )
	{
		RunScriptFunc rsf;
		rsf.wFuncType = FUNCTYPE_QUERYSETPLAYERNAME;
		pUser->AddRunScriptFunc( rsf );
		return 1;
	}
	return 0;
}

int APIENTRY Trace(NPCDIALOG_INFO* /*pInfo*/, LPCTSTR szMsg )
{
	FLTRACE_LOG( PROGRAM_NAME, _T( "%s" ), szMsg );
	return 1;
}


int APIENTRY GetPlayerExpPercent( NPCDIALOG_INFO* pInfo )  
{
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );
	return pUser->GetExpPercent();
}

int APIENTRY SetLevel( NPCDIALOG_INFO* pInfo , int nSetLevel )  
{
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );
	int rtn_val = pUser->SetLevel(nSetLevel);
	g_dpDBClient.SendUpdatePlayerData( pUser );
	return rtn_val;
}

int APIENTRY AddGPPoint( NPCDIALOG_INFO* pInfo , int nAddGPPoint )  
{
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );
	return pUser->AddGPPoint(nAddGPPoint);
}
#ifdef SCRIPT_ENTER_INSTANCEDUNGEON
int APIENTRY EnterInstanceDungeon( NPCDIALOG_INFO* pInfo , DWORD dwTeleWorld, int nX, int nY, int nZ ) 
{
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );
	return pUser->EnterInstanceDungeon(dwTeleWorld, nX, nY, nZ);
}

int	APIENTRY ChkPartyLowLevel( NPCDIALOG_INFO* pInfo, int nLv )
{
	FLWSUser* pUser = prj.GetUser( pInfo->GetPcId() );
	int nChk = 0;
	CParty* pParty = g_PartyMng.GetParty( pUser->m_idparty );
	if( pParty )
		nChk = pParty->ReplaceChkLowLv(nLv);
	return nChk;
}
#endif	// SCRIPT_ENTER_INSTANCEDUNGEON

Functions* GetDialogFunctions()
{
	return &g_functions;
}

void InitDialogFunctions()
{
	InitPredefineKey();		// �̸� ���ǵ� Ű���� ���� table�� �����.

	g_functions.SayName = SayName;
	g_functions.Speak = Speak;
	g_functions.SpeakName = SpeakName;
	g_functions.GetVal = GetVal;
	g_functions.GetSrcId = GetSrcId;
	g_functions.GetDstId = GetDstId;
	g_functions.NpcId = NpcId;
	g_functions.PcId = PcId;
	g_functions.GetGValue_ = GetGValue_;
	g_functions.SetGValue = SetGValue;
	g_functions.AddGValue = AddGValue;
	g_functions.AddKey = AddKey;
	g_functions.AddCondKey = AddCondKey;
	g_functions.AddAnswer = AddAnswer;
	g_functions.RemoveQuest = RemoveQuest;
	g_functions.RemoveAllKey = RemoveAllKey;
	g_functions.SayQuest = SayQuest;
	g_functions.BeginQuest = BeginQuest;
	g_functions.EndQuest = EndQuest;
	g_functions.AddQuestKey = AddQuestKey;
	g_functions.CreateMover = CreateMover;
	g_functions.CreateChar = CreateChar;
	g_functions.RemoveMover = RemoveMover;
	g_functions.GetDay = GetDay;
	g_functions.GetMin = GetMin;
	g_functions.GetHour = GetHour;
	g_functions.SetTimer = SetTimer;
	g_functions.IsTimeOut = IsTimeOut;
	g_functions.LaunchQuest = LaunchQuest;
	g_functions.GetEmptyInventoryNum = GetEmptyInventoryNum;
	g_functions.GetQuestState = GetQuestState;
	g_functions.IsSetQuest = IsSetQuest;
	g_functions.SetMark = SetMark;
	g_functions.GoMark = GoMark;
	g_functions.RandomIndex = RandomIndex;
	g_functions.RemoveKey = RemoveKey;
	g_functions.Say = Say;
	g_functions.EndSay = EndSay;
	g_functions.Random = Random;
	g_functions.Exit = Exit;
	g_functions.SetScriptTimer = SetScriptTimer;
	g_functions.Replace = Replace;
	g_functions.ReplaceKey = ReplaceKey;
	g_functions.GetPlayerLvl = GetPlayerLvl;
	g_functions.GetPlayerJob = GetPlayerJob;
	g_functions.IsGuildQuest = IsGuildQuest;
	g_functions.GetGuildQuestState = GetGuildQuestState;
	g_functions.IsWormonServer = IsWormonServer;
	g_functions.PrintSystemMessage = PrintSystemMessage;
	g_functions.MonHuntStart = MonHuntStart;
	g_functions.MonHuntStartParty = MonHuntStartParty;
	g_functions.InitStat = InitStat;
	g_functions.InitStr = InitStr;
	g_functions.InitSta = InitSta;
	g_functions.InitDex = InitDex;
	g_functions.InitInt = InitInt;
	g_functions.SetQuestState = SetQuestState;
	g_functions.SetQuest = SetQuest;
	g_functions.CreateItem = CreateItem;
	g_functions.AddGold = AddGold;
	g_functions.RemoveGold = RemoveGold;
	g_functions.GetPlayerGold = GetPlayerGold;
	g_functions.GetLocalEventState = GetLocalEventState;
	g_functions.AddExp = AddExp;
	g_functions.IsParty = IsParty;
	g_functions.IsPartyMaster = IsPartyMaster;
	g_functions.GetPartyNum = GetPartyNum;
	g_functions.GetPartyLevel = GetPartyLevel;
	g_functions.IsPartyGuild = IsPartyGuild;
	g_functions.IsGuild = IsGuild;
	g_functions.IsGuildMaster = IsGuildMaster;
	g_functions.CreateGuild = CreateGuild;
	g_functions.DestroyGuild = DestroyGuild;
	g_functions.IsPlayerSkillPoint = IsPlayerSkillPoint;
	g_functions.ChangeJob = ChangeJob;
	g_functions.GetPlayerSex = GetPlayerSex;
	g_functions.GetItemNum = GetItemNum;
	g_functions.RemoveAllItem = RemoveAllItem;
	g_functions.EquipItem = EquipItem;
	g_functions.PlaySound = PlaySound;
	g_functions.DropQuestItem = DropQuestItem;
	g_functions.ExpQuestEffect = ExpQuestEffect;
	g_functions.RemoveItem = RemoveItem;
	g_functions.Run = Run;
	g_functions.GetQuestId = GetQuestId;
	g_functions.GetLang = GetLang;
	g_functions.QuerySetPlayerName = QuerySetPlayerName;
	g_functions.Trace = Trace;
	g_functions.GetPlayerExpPercent = GetPlayerExpPercent;
	g_functions.SetLevel = SetLevel;
	g_functions.AddGPPoint = AddGPPoint;
#ifdef SCRIPT_ENTER_INSTANCEDUNGEON
	g_functions.EnterInstanceDungeon = EnterInstanceDungeon;
	g_functions.ChkPartyLowLevel = ChkPartyLowLevel;
#endif	// SCRIPT_ENTER_INSTANCEDUNGEON
}


#else

#define GetToken      pScript->GetToken
#define PutBack       pScript->PutBack
#define Compute       pScript->Compute
#define token         pScript->token
#define tokenType     pScript->tokenType

LPSCRIPT GetScript( CScript* pScript, LPCTSTR szKey )
{
	CScriptDialog* pScriptDlg = (CScriptDialog*)pScript->GetFunc();
	return pScriptDlg->Lookup( szKey );
}

//
// Func(1);�� �������� �� (�� 1�� )�� �д´�.
//
int EvalExp( CScript *pScript )
{
	int v;
	GetToken(); // (
	Compute(&v);
	GetToken(); // )
	return v;
}
int EvalDefault(CScript *pScript,int nDefault)
{
	GetToken(); // , or ) or (
	int v2;
	if(*token == ',' || *token == '(')
		Compute(&v2);
	else
	{
		v2 = nDefault;
		PutBack();
	}
	return v2;
}
//
// Func(1);�� �������� �� (�� 1�� �д´�.
//
void Eval1(CScript *pScript,int &v1)
{
	GetToken(); Compute(&v1);
}
//
// Func(1,2);�� �������� �� (�� 1,2�� �д´�.
//
void Eval2(CScript *pScript,int &v1,int &v2)
{
	GetToken(); Compute(&v1);
	GetToken(); Compute(&v2);
}
void Eval3(CScript *pScript,int &v1,int &v2,int &v3)
{
	GetToken(); Compute(&v1);
	GetToken(); Compute(&v2);
	GetToken(); Compute(&v3);
}
void Eval4(CScript *pScript,int &v1,int &v2,int &v3,int &v4)
{
	GetToken(); Compute(&v1);
	GetToken(); Compute(&v2);
	GetToken(); Compute(&v3);
	GetToken(); Compute(&v4);
}
void Eval5(CScript *pScript,int &v1,int &v2,int &v3,int &v4,int &v5)
{
	GetToken(); Compute(&v1);
	GetToken(); Compute(&v2);
	GetToken(); Compute(&v3);
	GetToken(); Compute(&v4);
	GetToken(); Compute(&v5);
}

void Eval6(CScript *pScript,int &v1,int &v2,int &v3,int &v4,int &v5,int &v6)
{
	GetToken(); Compute(&v1);
	GetToken(); Compute(&v2);
	GetToken(); Compute(&v3);
	GetToken(); Compute(&v4);
	GetToken(); Compute(&v5);
	GetToken(); Compute(&v6);
}
void Eval7(CScript *pScript,int &v1,int &v2,int &v3,int &v4,int &v5,int &v6,int &v7)
{
	GetToken(); Compute(&v1);
	GetToken(); Compute(&v2);
	GetToken(); Compute(&v3);
	GetToken(); Compute(&v4);
	GetToken(); Compute(&v5);
	GetToken(); Compute(&v6);
	GetToken(); Compute(&v7);
}
void Get2Tkn( CScript *pScript )
{
	GetToken(); GetToken();    
}

////////////////////////////////
// ����� ���� �Լ��� 
////////////////////////////////

// SayName(int nId,int nNameId,LPCTSTR lpszStr,...);
int SayName( CScript *pScript )
{
	int array[32], v1,v2, i = 0;
	CString strToken;
	CString string;
	Eval2(pScript,v1,v2);
	GetToken(); // ,
	GetToken(); // lpszStr
	strToken = token;
	GetToken(); 
	while(*token != ')')
	{
		Compute(&array[i++]);
		GetToken();
	}
	string.FormatV(strToken,(va_list)array);
	CMover* pMover = prj.GetMover(v1);
	CMover* pMoverName = prj.GetMover(v2);
	if(pMover && pMover->GetType() == OT_MOVER && pMoverName && pMoverName->GetType() == OT_MOVER)
	{
		CString stringName;
		stringName = pMoverName->GetName( TRUE );
		stringName += "! ";
		stringName += string;
	}
	return 1;
}
// �������� Ŭ���̾�Ʈ�� ��ȭ �޽����� ���� �� 
// Speak(int nId,LPCTSTR lpszStr,...);
int Speak( CScript *pScript )
{
	int array[32], v1, i = 0;
	CString strToken;
	CString string;
	Eval1(pScript,v1);
	GetToken(); // ,
	GetToken(); // lpszStr
	strToken = token;
	GetToken(); 
	while(*token != ')')
	{
		Compute(&array[i++]);
		GetToken();
	}
	string.FormatV(strToken,(va_list)array);

	CMover* pMover = prj.GetMover(v1);
	if(pMover && pMover->GetType() == OT_MOVER)
	{
		g_xWSUserManager->AddChat( (CCtrl*)pMover, (LPCSTR)string );
	}
	return 1;
}

// �������� Ŭ���̾�Ʈ�� ��ȭ �޽����� ���� �� 
// SpeakName(int nId,int nNameId,LPCTSTR lpszStr,...);
int SpeakName( CScript *pScript )
{
	int array[32], v1,v2, i = 0;
	CString strToken;
	CString string;
	Eval2(pScript,v1,v2);
	GetToken(); // ,
	GetToken(); // lpszStr
	strToken = token;
	GetToken(); 
	while(*token != ')')
	{
		Compute(&array[i++]);
		GetToken();
	}
	string.FormatV(strToken,(va_list)array);
	CMover* pMover = prj.GetMover(v1);
	CMover* pMoverName = prj.GetMover(v2);
	if(pMover && pMover->GetType() == OT_MOVER && pMoverName && pMoverName->GetType() == OT_MOVER)
	{
		CString stringName;
		stringName = pMoverName->GetName( TRUE );
		stringName += "! ";
		stringName += string;
		g_xWSUserManager->AddChat( (CCtrl*)pMover, (LPCSTR)stringName );
	}
	return 1;
}

int GetVal( CScript *pScript )
{
	Get2Tkn(pScript); 
	return pScript->GetReturn();
}

int GetSrcId( CScript *pScript )
{
	Get2Tkn(pScript); 
	return pScript->GetPcId();
}

int GetDstId( CScript *pScript )
{
	Get2Tkn(pScript); 
	return pScript->GetNpcId();
}

int NpcId( CScript *pScript )
{
	return GetDstId( pScript );
}

int PcId( CScript *pScript )
{
	return GetSrcId( pScript );
}

int GetGValue_( CScript *pScript )
{
	int v = EvalExp(pScript);
	if( v < 0 || v > 15 )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "GetGValue_���� %d(0~15) �� �̻�" ), v );
		v = 0;
	}
	return pScript->m_panGlobalVal[ v ];
}
int SetGValue( CScript *pScript )
{
	int v1, v2;
	Eval2(pScript,v1,v2);
	GetToken();
	if( v1 < 0 || v1 > 15 )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "SetGValue���� %d(0~15) �� �̻�" ), v1 );
		v1 = 0;
	}
	pScript->m_panGlobalVal[ v1 ] = v2;
	return TRUE;
}
int AddGValue( CScript *pScript )
{
	int v1, v2;
	Eval2(pScript,v1,v2);
	GetToken();
	if( v1 < 0 || v1 > 15 )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "AddGValue���� %d(0~15) �� �̻�" ), v1 );
		v1 = 0;
	}
	pScript->m_panGlobalVal[ v1 ] += v2;
	return TRUE;
}
int AddKey( CScript* pScript )
{
	DWORD dwParam = 0;
	CString strWord, strKey;
	GetToken(); // (
	for( int i = 0; *token != ')' && i < 3; i++ )
	{
		GetToken(); // data
		if( i == 0 )
			strWord		= token;
		else
		if( i == 1 )
			strKey	= token;
		else
		if( i == 2 )
			dwParam		= atoi( token );
		GetToken(); // ,
	}
	if( strKey.IsEmpty() )	
		strKey	= strWord;
	LPSCRIPT lpScript = GetScript( pScript, strKey );
	if( lpScript == NULL ) 
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s(%d) : %s" ), pScript->m_strFileName, pScript->GetLineNum(), strKey );
		return 1;
	}
	__AddKey( pScript->GetPcId(), strWord, strKey, dwParam );

	return 1;
}
int AddCondKey( CScript* pScript )
{
	DWORD dwParam = 0;
	CString strWord, strKey;
	GetToken(); // (
	for( int i = 0; *token != ')' && i < 3; i++ )
	{
		GetToken(); // data
		if( i == 0 )
			strWord		= token;
		else
		if( i == 1 )
			strKey	= token;
		else
		if( i == 2 )
			dwParam		= atoi( token );
		GetToken(); // ,
	}
	if( strKey.IsEmpty() )	
		strKey	= strWord;
	
	LPSCRIPT lpScript = GetScript( pScript, strKey );
	if( lpScript == NULL ) 
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s(%d) : %s" ), pScript->m_strFileName, pScript->GetLineNum(), strKey );
		return 1;
	}
	FLWSUser* pUser = prj.GetUser( pScript->GetPcId() );
	CMover* pMover = prj.GetMover( pScript->GetNpcId() );

	for( i = 0; i < pUser->m_nQuestSize; i++ )
	{
		LPQUEST lpQuest = &pUser->m_aQuest[ i ];
		QuestProp* pQuestProp = pUser->m_aQuest[ i ].GetProp();
		if( pQuestProp )
		{
			if( strcmp( pQuestProp->m_szEndCondDlgCharKey, pMover->m_szCharacterKey ) == 0 )
			{
				if( strcmp( pQuestProp->m_szEndCondDlgAddKey, strKey ) == 0 )
				{
					__AddKey( pScript->GetPcId(), strWord, strKey, dwParam );
					return 1;
				}
			}
		}
	}
	return 0;
}
int AddAnswer( CScript* pScript )
{
	int nParam1	= 0;
	int nParam2	= 0;
	CString strWord, strKey;
	GetToken(); // (
	for( int i = 0; *token != ')' && i < 3; i++ )
	{
		if( i == 0 )
		{
			GetToken();
			strWord	= token;
		}
		else
		if( i == 1 )
		{
			GetToken();
			if( tokenType == STRING )
				strKey = token;
			else
			{
				PutBack();
				i++;
			}
		}
		else
		if( i == 2 )
		{
			Compute( &nParam1 );
		}
		else
		if( i == 3 )
		{
			Compute( &nParam2 );
		}

		GetToken();	// ,
	}
	if( strKey.IsEmpty() )	
		strKey	= strWord;
	LPSCRIPT lpScript = GetScript( pScript, strKey );
	if( lpScript == NULL ) 
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s(%d) : %s" ), pScript->m_strFileName, pScript->GetLineNum(), strKey );
		return 1;
	}
	return __AddAnswer( pScript->GetPcId(), strWord, strKey, nParam1, nParam2 );
}
int GetParam( CScript* pScript )
{
	int v1, v2;
	Eval2( pScript, v1, v2 );

	if( v1 < 0 || v1 > 3 )
	{
		v1 = 0;
		FLERROR_LOG( PROGRAM_NAME, _T( "GetParam���� Idx �� ���� �ʰ�(0~3)" ) ); 
	}
	QuestProp* pQuestProp = prj.m_aPropQuest.GetAt( v2 );
	if( pQuestProp )
		return pQuestProp->m_nParam[ v1 ];
	return 0;
}
int RemoveQuest( CScript* pScript )      
{
	int v1	= EvalExp( pScript );
	return __RemoveQuest( pScript->GetPcId(), v1 );
}
int RemoveAllKey(CScript* pScript)
{
	Get2Tkn( pScript );
	__RemoveAllKey( pScript->GetPcId() );
	return 1;
}
// SayQuest( int nQuest, int nDialog )
int SayQuest( CScript* pScript )
{
	int v1, v2, i = 0;
	Eval2( pScript, v1, v2 );
	GetToken(); // )

	__SayQuest( pScript->GetPcId(), v1, v2 );
	return 1;
}

int BeginQuest( CScript* pScript )
{
	int v1	= EvalExp( pScript );
	return __RunQuest( pScript->GetPcId(), pScript->GetNpcId(), v1 );
}
/*
int IsEndQuestCondition( CScript* pScript )
{
	int v1 = EvalExp( pScript );
	FLWSUser* pUser = prj.GetUser( pScript->GetPcId() );
	return __IsEndQuestCondition( pUser, v1 );
}
*/
int IsBeginQuestCondition( CScript* pScript )
{
	int v1 = EvalExp( pScript );
	FLWSUser* pUser = prj.GetUser( pScript->GetPcId() );
	return __IsBeginQuestCondition( pUser, v1 );
}
int EndQuest( CScript* pScript )
{
	int v1 = EvalExp( pScript );
	return __EndQuest( pScript->GetPcId(), v1, FALSE );
}

// void AddQuestKey( int nQuest_szTitle, LPCTSTR lpKey, int nParam );
int AddQuestKey( CScript* pScript )
{
	int nParam = 0;
	int nQuest = -1;
	CString strKey;
	GetToken(); // (
	for( int i = 0; *token != ')' && i < 3; i++ )
	{
		if( i == 0 )
		{
			Compute( &nQuest );

		}
		if( i == 1 )
		{
			GetToken(); // data
			if( tokenType == STRING )
				strKey = token;
			else
			{
				PutBack();
				i++;
			}
		}
		if( i == 2 )
		{
			Compute( &nParam );
		}
		GetToken(); // ,
	}
	if( strKey.IsEmpty() )
	{
		QuestProp* pQuestProp = prj.m_aPropQuest.GetAt( nQuest );
		if( pQuestProp )
			strKey = pQuestProp->m_szTitle;
	}
	LPSCRIPT lpScript = GetScript( pScript, strKey );
	if( lpScript == NULL ) 
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s(%d) : %s" ), pScript->m_strFileName, pScript->GetLineNum(), strKey );
		return 1;
	}

	return __AddQuestKey( pScript->GetPcId(), nQuest, strKey, nParam );
}
// SysMessage( int nLeft, int nTop, int nTight, int nBottom, LPCTSTR lpszMessage );
int SysMessage( CScript* pScript )
{
	CRect rect;
	Eval4( pScript, (int&)rect.left, (int&)rect.top, (int&)rect.right, (int&)rect.bottom );
	GetToken(); // ,
	GetToken(); // string
	rect.NormalizeRect();
	g_xWSUserManager->AddWorldMsg( &rect, token );
	GetToken(); // )
	return 1;
}
// CreateMover( int nMoverId, int nMoverIdx, int x, int y, int z, BOOL bActAttack )      
int CreateMover( CScript* pScript )
{
	int nMoverId, nMoverIdx;
	int x, y, z, nActAttack;

	Eval2( pScript, nMoverId, nMoverIdx );
	Eval3( pScript, x, y, z );
	Eval1( pScript, nActAttack );
	GetToken(); // )

	CWorld* pWorld = __GetWorld( pScript->GetPcId(), pScript->GetSrcId(), pScript->GetCaller() );
	if( pWorld == NULL )
		return 0;

	DWORD dwData;
	if( pWorld->m_mapCreateChar.Lookup( nMoverId, (void*&)dwData ) )
	{
		// ������ Ű�� ����ִ�. ���ŵǱ� ������ �ȵ�.
		return FALSE;
	}
	MoverProp* pMoverProp = prj.GetMoverProp( nMoverIdx );

	if( pMoverProp && pMoverProp->dwID != 0 )
	{
		CMover* pMover = (CMover*)CreateObj( D3DDEVICE, OT_MOVER, pMoverProp->dwID );
		if( NULL == pMover )	
			return FALSE;
		D3DXVECTOR3 vPos( (FLOAT)x, (FLOAT)y, (FLOAT)z );
		pMover->SetPos( vPos );
		vPos.y -= 1.0f;
		pWorld->GetUnderHeight( vPos );
		pMover->InitMotion( MTI_STAND );
		pMover->UpdateLocalMatrix();
		if( nActAttack )
			pMover->m_bActiveAttack = nActAttack;
		pMover->AddItToGlobalId();
		if( pWorld->AddObj( pMover, FALSE ) == TRUE )
		{
			pWorld->m_mapCreateChar.SetAt( nMoverId, (void*)pMover->GetId() );
		}
		else
		{
			SAFE_DELETE( pMover );
			return 0;
		}
	}
	return 1;
}     
// CreateChar( int nMoverId, int nMoverIdx, LPCTSTR lpszCharKey, int x,int y,int z )
int CreateChar( CScript* pScript )
{
	int nMoverId, nMoverIdx;
	int x, y, z;
	CHAR szKey[ 64 ];

	Eval2( pScript, nMoverId, nMoverIdx );
	GetToken(); // ,
	GetToken();
	strcpy( szKey, token );
	Eval3( pScript, x, y, z );
	GetToken(); // )

	CWorld* pWorld = __GetWorld( pScript->GetPcId(), pScript->GetSrcId(), pScript->GetCaller() );
	if( pWorld == NULL )
		return 0;

	DWORD dwData;
	if( pWorld->m_mapCreateChar.Lookup( nMoverId, (void*&)dwData ) )
	{
		// ������ Ű�� ����ִ�. ���ŵǱ� ������ �ȵ�.
		return FALSE;
	}
	//CWorld* pWorld = g_WorldMng.GetWorld( globalArray[1] );
	MoverProp* pMoverProp = prj.GetMoverProp( nMoverIdx );
	if( pMoverProp && pMoverProp->dwID != 0 )
	{
		CMover* pMover = (CMover*)CreateObj( D3DDEVICE, OT_MOVER, pMoverProp->dwID );
		if( NULL == pMover ) return FALSE; // FLASSERT( pObj );
		strcpy( pMover->m_szCharacterKey, szKey );
		pMover->InitNPCProperty();
		pMover->InitCharacter( pMover->GetCharacter() );
		D3DXVECTOR3 vPos( (FLOAT)x, (FLOAT)y, (FLOAT)z );
		pMover->SetPos( vPos );
		vPos.y -= 1.0f;
		pWorld->GetUnderHeight( vPos );
		pMover->InitMotion( MTI_STAND );
		pMover->UpdateLocalMatrix();
		pMover->AddItToGlobalId();
		if( pWorld->AddObj( pMover, FALSE ) == TRUE )
		{
			pWorld->m_mapCreateChar.SetAt( nMoverId, (void*)pMover->GetId() );
		}
		else
		{
			SAFE_DELETE( pMover );
			return FALSE;
		}
	}
	return TRUE;
}    
// RemoveMover( int nMoverId );
int RemoveMover( CScript* pScript )
{
	BOOL bResult = FALSE;
	int nMoverId = EvalExp( pScript );

	CWorld* pWorld = __GetWorld( pScript->GetPcId(), pScript->GetSrcId(), pScript->GetCaller() );
	if( pWorld == NULL )
		return 0;

	OBJID idMover;
	if( pWorld->m_mapCreateChar.Lookup( nMoverId, (void*&)idMover ) )
	{
		CMover* pMover	= prj.GetMover( idMover );
		if( IsValidObj( (CObj*)pMover ) )
		{
			if( pMover->IsNPC() )
			{
				pMover->Delete();
				bResult = TRUE;
			}
		}
		pWorld->m_mapCreateChar.RemoveKey( nMoverId );
	}

	return bResult;
}   
int GetDay( CScript* pScript )
{
	Get2Tkn( pScript );
	return g_GameTimer.m_nDay;
}        
int GetMin( CScript* pScript )
{
	Get2Tkn( pScript );
	return g_GameTimer.m_nMin;
}      
int GetHour( CScript* pScript )
{
	Get2Tkn( pScript );
	return g_GameTimer.m_nHour;
}       
 
int SetTimer( CScript* pScript )  
{
	int nTimerId, nSec;
	Eval2( pScript, nTimerId, nSec );
	GetToken();
	if( nTimerId >= MAX_SCRIPT_TIMER )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "Script : SetTimer���� Ÿ�̸� ID Max �ʰ� ( %d / %d )" ), nTimerId, MAX_SCRIPT_TIMER  );
		nTimerId = 0 ;
	}
	pScript->m_paTimer[ nTimerId ].Set( SEC( nSec ) );
	return 1;
}
int IsTimeOut( CScript* pScript )
{
	int nTimerId = EvalExp( pScript );
	if( nTimerId >= MAX_SCRIPT_TIMER )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "Script : IsTimeOut Ÿ�̸� ID Max �ʰ� ( %d / %d )" ), nTimerId, MAX_SCRIPT_TIMER  );
		nTimerId = 0 ;
	}
	if( pScript->m_paTimer[ nTimerId ].IsTimeOut() )
	{
		pScript->m_paTimer[ nTimerId ].Reset();
		return TRUE;
	}
	return FALSE;
}

int LaunchQuest( CScript* pScript )
{
	Get2Tkn( pScript );
	pScript->m_panGlobalVal[ 15 ] = 0;
	__QuestEnd( pScript->GetPcId(), pScript->GetNpcId(), pScript->m_panGlobalVal[ 15 ] );
	return 1;
}
int GetEmptyInventoryNum( CScript* pScript, DWORD dwItemId )
{
	Get2Tkn( pScript );
	FLWSUser* pUser = prj.GetUser( pScript->GetPcId() );
	return pUser->m_Inventory.GetEmptyCountByItemId( dwItemId );
}

int GetQuestState( CScript* pScript )
{
	int v1 = EvalExp( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	LPQUEST pQuest = pUser->GetQuest( v1 );
	if( pQuest ) 
		return pQuest->m_nState;
	QUEST quest;
	if( pUser->MakeCompleteQuest( v1, &quest ) )
		return quest.m_nState;
	return -1;
}
int IsSetQuest( CScript* pScript )
{
	int v1 = EvalExp( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	LPQUEST pQuest = pUser->GetQuest( v1 );
	if( pQuest ) return TRUE;
	if( pUser->IsCompleteQuest( v1 ) )
		return TRUE;
	return FALSE;
}

int SetMark( CScript* pScript )
{
	return 1;
}
int GoMark( CScript* pScript )
{
	return 1;
}

int RandomIndex( CScript* pScript) //100, 20, 12, 1, 20, 12, 1, 20, 13, 1 )
{
	int nRandom, nMax = 1;
	Eval1( pScript, nRandom ); // ( max
	GetToken(); // , or )
	int anValue[64], v1;
	anValue[ 0 ] = 0;
	while( *token != ')' )
	{
		Compute( &v1 );
		anValue[ nMax++ ] = v1;
		GetToken(); // , or )
	}
	int nResult = 0;
	for( int i = 0; i < nMax; i++ )
	{
		nResult += anValue[ i ];
		anValue[ i ] = nResult;
	}
	int nRand = random( nRandom );
	for( i = 0; i < nMax - 1;  i++ )
	{
		if( nRand >= anValue[ i ] && nRand < anValue[ i + 1 ]  )
			return i;
	}
	return 0;
}

int RemoveKey(CScript* pScript)
{
	GetToken();
	GetToken();
	CString strWord		= token;
	GetToken();

	LPSCRIPT lpScript = GetScript( pScript, strWord );
	if( lpScript )
	{
		FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );

		RunScriptFunc rsf;
		rsf.wFuncType		= FUNCTYPE_REMOVEKEY;
		lstrcpy( rsf.lpszVal1, strWord );
		pUser->AddRunScriptFunc( rsf );
	}
	return 1;
}

int Say(CScript* pScript)
{
	int array[32], i	= 0;
	CString strToken;
	CString string;
	GetToken(); // (
	GetToken(); // lpszStr
	strToken	= token;
	GetToken(); 
	while( *token != ')' )
	{
		Compute( &array[i++] );
		GetToken();
	}
	string.FormatV( strToken, (va_list)array );

	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	RunScriptFunc rsf;
	rsf.wFuncType		= FUNCTYPE_SAY;
	lstrcpy( rsf.lpszVal1, string );
	rsf.dwVal2 = 0;
	pUser->AddRunScriptFunc( rsf );
	return 1;
}

int EndSay( CScript* pScript )
{
	Get2Tkn( pScript );
	// ��� ���� 
	return 1;
}

int Random( CScript* pScript )
{
	int val = EvalExp( pScript );
	return rand() % val;
}

int Exit( CScript* pScript )
{
	Get2Tkn( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	RunScriptFunc rsf;
	rsf.wFuncType		= FUNCTYPE_EXIT;
	pUser->AddRunScriptFunc( rsf );
	return TRUE;
}

int SetScriptTimer( CScript* pScript )
{
	UINT nTimer = EvalExp( pScript );
	CMover* pMover = prj.GetMover( pScript->GetNpcId() );
	return 1;
}

// Replace( int nWorld, int x, int y, int z )
int Replace( CScript* pScript )
{
	int v1, v2, v3, v4;
	GetToken(); // (
	Compute( &v1 );
	GetToken(); // ,
	Compute( &v2 );
	GetToken(); // ,
	Compute( &v3 );
	GetToken(); // ,
	Compute( &v4 );
	GetToken(); // )

	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	if( v2 > 0 && v4 > 0 )
	{
		RunScriptFunc rsf;	// Exit()
		rsf.wFuncType		= FUNCTYPE_EXIT;
		pUser->AddRunScriptFunc( rsf );
		if( g_eLocal.GetState( EVE_SCHOOL ) )
		{
			g_dpDBClient.SendLogSchool( pUser->m_idPlayer, pUser->GetName() );
		}

		pUser->Replace( g_uIdofMulti, v1, D3DXVECTOR3( v2, v3, v4 ) );
	}
	return 1;
}

// Replace( int nWorld, LPCTSTR szKey )
int ReplaceKey( CScript* pScript )
{
	int v1;
	CString strKey;
	GetToken(); // (
	Compute( &v1 );
	GetToken(); // ,
	GetToken(); // key
	strKey = token;
	GetToken(); // )

	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	RunScriptFunc rsf;	// Exit()
	rsf.wFuncType		= FUNCTYPE_EXIT;
	pUser->AddRunScriptFunc( rsf );

	PRegionElem pRgnElem	= g_WorldMng.GetRevivalPos( v1, strKey );
	if( NULL != pRgnElem )
	{
		pUser->Replace( g_uIdofMulti, pRgnElem->m_dwWorldId, pRgnElem->m_vPos );
	}
	return 1;
}

int GetPlayerLvl( CScript* pScript )  
{
	Get2Tkn( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	return pUser->GetLevel();
}



int GetPlayerJob( CScript* pScript )  
{
	Get2Tkn( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	return pUser->GetJob();
}

int IsGuildQuest( CScript* pScript )
{
	int v1	= EvalExp( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	CGuild* pGuild	= pUser->GetGuild();
	if( pGuild )
	{
		PGUILDQUEST pQuest	= pGuild->GetQuest( v1 );
		if( pQuest )
			return TRUE;
	}
	return FALSE;
}

int GetGuildQuestState( CScript* pScript )
{
	int v1	= EvalExp( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	CGuild* pGuild	= pUser->GetGuild();
	if( pGuild )
	{
		PGUILDQUEST pQuest	= pGuild->GetQuest( v1 );
		if( pQuest )
			return pQuest->nState;
	}
	return -1;
}

int IsWormonServer( CScript* pScript )
{
	Get2Tkn( pScript );
	return g_eLocal.GetState( EVE_WORMON );
}


int PrintSystemMessage( CScript* pScript )
{
	int v1	= EvalExp( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	pUser->AddDefinedText( (DWORD)v1 );
	return 1;
}

int MonHuntStart( CScript* pScript )
{
	int v1, v2, v3, v4;
	Eval4( pScript, v1, v2, v3, v4 );
	GetToken();
	CGuildQuestProcessor* pProcessor	= CGuildQuestProcessor::GetInstance();
	if( pProcessor->IsQuesting( v1 ) )
	{
		return 0;
	}
	else
	{
		FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
		CGuild* pGuild	= pUser->GetGuild();
		if( pGuild )
		{
			GUILDQUESTPROP* pProp	= prj.GetGuildQuestProp( v1 );
			if( pProp )
			{
				CMover* pWormon	= (CMover*)CreateObj( D3DDEVICE, OT_MOVER, pProp->dwWormon );
				if( pWormon )
				{
					pWormon->SetPos( pProp->vPos );
					pWormon->InitMotion( MTI_STAND );
					pWormon->UpdateLocalMatrix();
					pWormon->m_nQuestKeeping	= v1;

					CWorld* pWorld;
					if( ( pWorld = pUser->GetWorld() ) )
					{
						RunScriptFunc rsf;	// Exit()
						rsf.wFuncType		= FUNCTYPE_EXIT;
						pUser->AddRunScriptFunc( rsf );
						pWormon->AddItToGlobalId();
						if( pWorld->AddObj( pWormon, FALSE ) == TRUE )
						{
							pGuild->SetQuest( v1, v2 );
							if( v2 == QS_BEGIN )
								g_dpDBClient.SendInsertGuildQuest( pGuild->m_idGuild, v1 );
							else
								g_dpDBClient.SendUpdateGuildQuest( pGuild->m_idGuild, v1, v2 );

							pProcessor->SetGuildQuest( v1, v2, v3, v4, pGuild->m_idGuild, pWormon->GetId() );
							D3DXVECTOR3 vPos( pProp->vPos.x, pProp->vPos.y, ( ( pProp->vPos.z * 2 ) + pProp->y2 ) / 3 );
							pGuild->Replace( pProp->dwWorldId, vPos, TRUE );
							return 1;
						}
						else
						{
							SAFE_DELETE( pWormon );
							return 0;
						}
					}
					else
					{
						SAFE_DELETE( pWormon );
						return 0;
					}
				}
			}
		}
	}
	return 0;
}

int MonHuntStartParty( CScript* pScript )
{
	int v1, v2, v3, v4;
	Eval4( pScript, v1, v2, v3, v4 );
	GetToken();
	CPartyQuestProcessor* pProcessor	= CPartyQuestProcessor::GetInstance();
	if( pProcessor->IsQuesting( v1 ) )
	{
		return 0;
	}
	else
	{
		FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );

		CParty* pParty	= g_PartyMng.GetParty( pUser->GetPartyId() );
		if( pParty )
		{
			PARTYQUESTPROP* pProp	= prj.GetPartyQuestProp( v1 );
			if( pProp )
			{				
				if( pParty->ReplaceChkLv( pProp->nLevel+5 ) == FALSE )
				{
					CString str;
					str.Format( prj.GetText(TID_GAME_PARTYQUEST_NOT_JOIN), pProp->nLevel+5 );
					pUser->AddDiagText( str );

					return 0;
				}

				// ������ ����ִ�...��Ƽ�� ���� �÷��̾��... ������ ��ȯ��Ų��...
//				g_xWSUserManager->ReplaceWorldArea( pProp->dwWorldId, WI_WORLD_MADRIGAL, 6968.0f, 3328.8f, 0.5f, FALSE );				

				BOOL bSuccess = FALSE;

				for( int i=0; i<pProp->vecWormon.size(); i++ )
				{
					WORMON* WorMon;
					WorMon = &(pProp->vecWormon.at( i ));
					CMover* pWormon	= (CMover*)CreateObj( D3DDEVICE, OT_MOVER, WorMon->dwWormon );

					if( pWormon )
					{
						pWormon->SetPos( WorMon->vPos );
						pWormon->InitMotion( MTI_STAND );
						pWormon->UpdateLocalMatrix();
						pWormon->m_nPartyQuestKeeping = v1;

						CWorld* pWorld	= g_WorldMng.GetWorld( pProp->dwWorldId );
						if( pWorld )
						{
							RunScriptFunc rsf;	// Exit()
							rsf.wFuncType		= FUNCTYPE_EXIT;
							pUser->AddRunScriptFunc( rsf );
							pWormon->AddItToGlobalId();
							pWormon->m_bActiveAttack = TRUE;
							if( pWorld->AddObj( pWormon, FALSE ) == TRUE )
							{
								bSuccess = TRUE;
							}
							else
							{
								SAFE_DELETE( pWormon );
								bSuccess = FALSE;
							}
						}
						else
						{
							SAFE_DELETE( pWormon );
							bSuccess = FALSE;
						}
					}
				}
				
				if( bSuccess )
				{
					pProcessor->SetPartyQuest( v1, v2, v3, v4, pUser->GetPartyId(), 0 );
					pParty->Replace( pProp->dwWorldId, pProp->szWorldKey );
									
					return 1;
				}
			}
		}
	}
	return 0;
}

int	InitStat( CScript* pScript )
{
	Get2Tkn( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	pUser->m_Stat.SetOriginStr( DEFAULT_ORIGIN_STAT_VALUE );
	pUser->m_Stat.SetOriginInt( DEFAULT_ORIGIN_STAT_VALUE );
	pUser->m_Stat.SetOriginDex( DEFAULT_ORIGIN_STAT_VALUE );
	pUser->m_Stat.SetOriginSta( DEFAULT_ORIGIN_STAT_VALUE );

	pUser->m_Stat.SetRemainStatPoint( ( pUser->GetLevel() - 1 ) * 2 );
	RunScriptFunc rsf;
	rsf.wFuncType		= FUNCTYPE_INITSTAT;
	rsf.dwVal1	= pUser->m_Stat.GetRemainStatPoint();
	pUser->AddRunScriptFunc( rsf );

	g_xWSUserManager->AddSetState( pUser );
	return 1;
}

int	InitStr( CScript* pScript )
{
	Get2Tkn( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	int nStr = pUser->m_Stat.GetOriginStr();
	if( nStr - DEFAULT_ORIGIN_STAT_VALUE > 0 )
	{
		pUser->m_Stat.SetOriginStr( DEFAULT_ORIGIN_STAT_VALUE );

		pUser->m_Stat.SetRemainStatPoint( pUser->m_Stat.GetRemainStatPoint() + nStr - DEFAULT_ORIGIN_STAT_VALUE );
	}
	
	RunScriptFunc rsf;
	rsf.wFuncType		= FUNCTYPE_INITSTR;
	rsf.dwVal1	= pUser->m_Stat.GetRemainStatPoint();
	pUser->AddRunScriptFunc( rsf );

	g_xWSUserManager->AddSetState( pUser );
	return 1;
}
int	InitSta( CScript* pScript )
{
	Get2Tkn( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	int nSta = pUser->m_Stat.GetOriginSta();
	if( nSta - DEFAULT_ORIGIN_STAT_VALUE > 0 )
	{
		pUser->m_Stat.SetOriginSta( DEFAULT_ORIGIN_STAT_VALUE );

		pUser->m_Stat.SetRemainStatPoint( pUser->m_Stat.GetRemainStatPoint() + nSta - DEFAULT_ORIGIN_STAT_VALUE );
	}
	
	RunScriptFunc rsf;
	rsf.wFuncType		= FUNCTYPE_INITSTA;
	rsf.dwVal1	= pUser->m_Stat.GetRemainStatPoint();
	pUser->AddRunScriptFunc( rsf );

	g_xWSUserManager->AddSetState( pUser );
	return 1;
}
int	InitDex( CScript* pScript )
{
	Get2Tkn( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	int nDex = pUser->m_Stat.GetOriginDex();
	if( nDex - DEFAULT_ORIGIN_STAT_VALUE > 0 )
	{
		pUser->m_Stat.SetOriginDex( DEFAULT_ORIGIN_STAT_VALUE );

		pUser->m_Stat.SetRemainStatPoint( pUser->m_Stat.GetRemainStatPoint() + nDex - DEFAULT_ORIGIN_STAT_VALUE );
	}
	
	RunScriptFunc rsf;
	rsf.wFuncType		= FUNCTYPE_INITDEX;
	rsf.dwVal1	= pUser->m_Stat.GetRemainStatPoint();
	pUser->AddRunScriptFunc( rsf );

	g_xWSUserManager->AddSetState( pUser );
	return 1;
}
int	InitInt( CScript* pScript )
{
	Get2Tkn( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	int nInt = pUser->m_Stat.GetOriginInt();
	if( nInt - DEFAULT_ORIGIN_STAT_VALUE > 0 )
	{
		pUser->m_Stat.SetOriginInt( DEFAULT_ORIGIN_STAT_VALUE );

		pUser->m_Stat.SetRemainStatPoint( pUser->m_Stat.GetRemainStatPoint() + nInt - DEFAULT_ORIGIN_STAT_VALUE );
	}
	
	RunScriptFunc rsf;
	rsf.wFuncType		= FUNCTYPE_INITINT;
	rsf.dwVal1	= pUser->m_Stat.GetRemainStatPoint();
	pUser->AddRunScriptFunc( rsf );

	g_xWSUserManager->AddSetState( pUser );
	return 1;
}

int SetQuestState( CScript* pScript )      
{
	int v1, v2;
	Eval2( pScript, v1, v2 );
	GetToken();
	return __SetQuestState( pScript->GetPcId(), v1, v2 );
}
int SetQuest( CScript* pScript )      
{
	int v1	= EvalExp( pScript );
	__SetQuest(  pScript->GetPcId(), v1 );
	return 1;
}

int CreateItem( CScript* pScript )	
{
	int v1, v2;
	Eval2( pScript, v1, v2 );
	GetToken();
	__CreateItem( pScript->GetPcId(), v1, v2 );
	return 1;
}

int AddGold( CScript* pScript )     
{
	int v1	= EvalExp( pScript );
	__AddGold(  pScript->GetPcId(), v1 );
	return 1;
}

int	RemoveGold( CScript* pScript )
{
	int v1	= EvalExp( pScript );
	__RemoveGold( pScript->GetPcId(), v1 );
	return 1;
}
int GetPlayerGold( CScript* pScript )
{
	Get2Tkn( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	return pUser->GetGold();
}


int GetLocalEventState( CScript* pScript )
{
	int v1	= EvalExp( pScript );
	return g_eLocal.GetState( v1 );
}

int AddExp( CScript* pScript )      
{
	int v1	= EvalExp( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	if( pUser->AddExperience( v1, TRUE, FALSE ) )
		pUser->LevelUpSetting();
	else
		pUser->ExpUpSetting();

	pUser->AddSetExperience( pUser->GetExp1(), (WORD)pUser->m_nLevel, pUser->m_nSkillPoint, pUser->m_nSkillLevel );
	pUser->AddDefinedText( TID_GAME_REAPEXP );
	return 1;
}

int IsParty( CScript* pScript )
{
	Get2Tkn( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	CParty* pParty	= g_PartyMng.GetParty( pUser->m_idparty );
	int f	= ( pParty != NULL && pParty->IsMember( pUser->m_idPlayer ) );
	return f;
}

int IsPartyMaster( CScript* pScript )
{
	Get2Tkn( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	CParty* pParty	= g_PartyMng.GetParty( pUser->m_idparty );
	int f	= ( pParty != NULL && pParty->IsLeader( pUser->m_idPlayer ) );
	return f;
}

int GetPartyNum( CScript* pScript )
{
	Get2Tkn( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	int nSize	= 0;
	CParty* pParty	= g_PartyMng.GetParty( pUser->m_idparty );
	if( pParty )
		nSize	= pParty->GetSizeofMember();
	return nSize;
}

int	GetPartyLevel( CScript* pScript )
{
	Get2Tkn( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	int nLevel	= 0;
	CParty* pParty	= g_PartyMng.GetParty( pUser->m_idparty );
	if( pParty )
		nLevel	= pParty->GetLevel();
	return nLevel;
}

int IsPartyGuild( CScript* pScript )
{
	Get2Tkn( pScript );

	int f	= 0;
	u_long	aidPlayer[MAX_PTMEMBER_SIZE];
	int	nSize	= 0;
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );

	CParty* pParty	= g_PartyMng.GetParty( pUser->m_idparty );
	if( pParty )
	{
		nSize	= pParty->m_nSizeofMember;
		for( int i = 0; i < pParty->m_nSizeofMember; i++ )
			aidPlayer[i]	= pParty->m_aMember[i].m_uPlayerId;
	}
	else
	{
		f	= 1;
	}

	for( int i = 0; i < nSize; i++ )
	{
		FLWSUser* pUsertmp	= g_xWSUserManager->GetUserByPlayerID( aidPlayer[i] );
		if( IsValidObj( pUsertmp ) )
		{
			if( pUsertmp->GetGuild() )
			{
				f	= 1;
				break;
			}
			CTime tCurrent = CTime::GetCurrentTime();
			if( tCurrent < pUsertmp->m_tGuildMember )
			{
				f	= 2;
				break;
			}
		}
		else
		{
			f	= 1;
			break;
		}
	}
	return f;

}

int IsGuild( CScript* pScript )
{
	Get2Tkn( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	CGuild* pGuild	= g_GuildMng.GetGuild( pUser->m_idGuild );
	int f	= ( pGuild && pGuild->IsMember( pUser->m_idPlayer ) );
	return f;
}

int IsGuildMaster( CScript* pScript )
{
	Get2Tkn( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	CGuild* pGuild	= g_GuildMng.GetGuild( pUser->m_idGuild );
	int f	= ( pGuild && pGuild->IsMaster( pUser->m_idPlayer ) );
	return f;
}

int CreateGuild( CScript* pScript )
{
	Get2Tkn( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	int nSize	= 0;
	GUILD_MEMBER_INFO	info[MAX_PTMEMBER_SIZE];

	CParty* pParty	= g_PartyMng.GetParty( pUser->m_idparty );
	if( pParty && pParty->IsLeader( pUser->m_idPlayer ) )
	{
		for( int i = 0; i < pParty->m_nSizeofMember; i++ )
		{
			FLWSUser* pUsertmp	= g_xWSUserManager->GetUserByPlayerID( pParty->GetPlayerId( i ) );
			if( IsValidObj( pUsertmp ) )
			{
				info[nSize].idPlayer	= pUsertmp->m_idPlayer;
				info[nSize].nLevel	= pUsertmp->GetLevel();
				info[nSize].nJob	= pUsertmp->GetJob();
				info[nSize].dwSex	= pUsertmp->GetSex();
				nSize++;
			}
			else
			{
				// error	- offline
				return 0;
			}
		}
	}
	else
	{
		// error	-
		return 0;
	}

	g_DPCoreClient.SendCreateGuild( info, nSize, "" );

	return 1;
}

int DestroyGuild( CScript* pScript )
{
	Get2Tkn( pScript );

	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );

	CGuild* pGuild	= g_GuildMng.GetGuild( pUser->m_idGuild );
	if( pGuild && pGuild->IsMember( pUser->m_idPlayer ) )
	{
		if( pGuild->IsMaster( pUser->m_idPlayer ) )
		{
			RunScriptFunc rsf;
			rsf.wFuncType	= FUNCTYPE_DESTROYGUILD;
			pUser->AddRunScriptFunc( rsf );
		}
		else
			pUser->AddDefinedText( TID_GAME_COMDELNOTKINGPIN );
	}
	else
		pUser->AddDefinedText( TID_GAME_COMNOHAVECOM );


	return 1;
}

int IsPlayerSkillPoint( CScript* pScript )     
{
	return 0;
}

int ChangeJob( CScript* pScript )     
{
	int v1	= EvalExp( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	if( pUser->IsBaseJob() && pUser->GetLevel() != MAX_JOB_LEVEL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "Error ChangeJob : 1" ) );
		return 0;
	}
	if( pUser->IsExpert() && pUser->GetLevel() < MAX_JOB_LEVEL + MAX_EXP_LEVEL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "Error ChangeJob : 2" ) );
		return 0;
	}
	if( pUser->IsPro()  && pUser->GetLevel() < MAX_CHARACTER_LEVEL && pUser->GetExpPercent() < 9999 )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "Error ChangeJob : 3" ) );
		return 0;
	}
	if( pUser->AddChangeJob( v1 ) )
	{
		pUser->AddSetChangeJob( v1 );
		g_xWSUserManager->AddNearSetChangeJob( (CMover*)pUser, v1, &pUser->m_aJobSkill[MAX_JOB_SKILL] );
		// #dlvr
		g_dpDBClient.SendLogLevelUp( (FLWSUser*)pUser, 4 );
		if( g_eLocal.GetState( EVE_RECOMMEND ) && pUser->IsPlayer() )
			g_dpDBClient.SendRecommend( (FLWSUser*)pUser, v1 );
		g_DPDBClient.SendUpdatePlayerData( pUser );
	}
	else
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "Error ChangeJob : CMover::AddChangeJob" ) );
	}
	return 1;
}

int GetPlayerSex( CScript* pScript )  
{
	Get2Tkn(pScript);
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	return pUser->GetSex();
}
int GetItemNum( CScript* pScript )    
{
	int v1	= EvalExp( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	return pUser->GetItemNum( v1 );
}
int RemoveAllItem( CScript* pScript ) 
{
	int v1	= EvalExp( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	pUser->RemoveAllItem( v1 );
	return 1;
}
int EquipItem( CScript* pScript )     
{
	int v1	= EvalExp( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	DWORD dwItemObjID;
	
	FLItemElem itemElem;
	itemElem.m_dwItemId		= v1;
	itemElem.m_nItemNum		= 1;
	itemElem.SetAbilityOption( 0 );
	if( pUser->CreateItem( &itemElem, &dwItemObjID ) == TRUE )
	{
		FLItemElem* pItemElem	= pUser->m_Inventory.GetAtId( dwItemObjID );
		if( pItemElem && TRUE == pUser->DoEquip( pItemElem, TRUE ) )
		{
			g_xWSUserManager->AddDoEquip( (CMover*)pUser, -1, pItemElem, TRUE );
		}

		LogItemInfo aLogItem;
		/*aLogItem.Action = "Q";
		aLogItem.SendName = pUser->GetName();
		aLogItem.RecvName = "QUEST";*/
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "Q" );
		FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
		FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "QUEST" );
		aLogItem.WorldId = pUser->GetWorld()->GetID();
		aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
		g_DPSrvr.OnLogItem( aLogItem, &itemElem, itemElem.m_nItemNum );
	}
	else
	{
		FLItemElem* pItemElem	= new FLItemElem;
		pItemElem->m_dwItemId	= v1;
		PT_ITEM_SPEC pItemProp		= pItemElem->GetProp();
		if( pItemProp )
		{
			pItemElem->m_nHitPoint	= pItemElem->m_nMaxHitPoint = ( pItemProp->dwEndurance == -1 ) ? 0 : pItemProp->dwEndurance;//pItemProp->dwEndurance;
			if( pItemElem->m_nMaxHitPoint < 0 )
				pItemElem->m_nMaxHitPoint = 0;

			pItemElem->m_nItemNum	= 1;
			pItemElem->SetAbilityOption( 0 );
			CItem* pItem	= new CItem;
			pItem->m_pItemBase	= pItemElem;

			if( pItemElem->m_dwItemId == 0 ) FLERROR_LOG( PROGRAM_NAME, _T( "EquipItem SetIndex" ) );
			pItem->SetIndex( D3DDEVICE, pItemElem->m_dwItemId );
			pItem->SetPos( pUser->GetPos() );
			pItem->SetAngle( xRandom( 360 ) );
			pItem->m_idHolder	= pUser->m_idPlayer;
			if( pUser->GetWorld()->AddObj( pItem, TRUE ) == TRUE )
			{
				pUser->AddDefinedText( TID_GAME_DROPITEM, "\"%s\"", g_xSpecManager->GetSpecItem( v1 )->szName );
			}
			else
			{
				SAFE_DELETE( pItem );
			}
		}
		else
		{
			SAFE_DELETE( pItemElem );
		}
	}
	return 1;
}

// int PlaySound( LPCTSTR lpszSoundFileName )
int PlaySound(CScript* pScript)
{
	GetToken();
	GetToken();
	FLWSUser* pUser = prj.GetUser( pScript->GetPcId() );
	if( pUser )
	{
		pUser->AddPlaySound2( token );
	}
	GetToken();
	return 1;
}

int DropQuestItem( CScript* pScript )
{
// ( dwItemId, dwProbability )
	int v1, v2;
	Eval2( pScript, v1, v2 );
	GetToken();
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );

	if( xRandom( 1000 ) <  v2 )
	{
		PT_ITEM_SPEC pItemProp	= g_xSpecManager->GetSpecItem( v1 );
		if( pItemProp )
		{
			CWorld* pWorld	= pUser->GetWorld();
			if( pWorld )
			{
				FLItemElem* pItemElem	= new FLItemElem;
				pItemElem->m_dwItemId	= v1;
				CItem* pItem	= new CItem;
				pItem->m_pItemBase	= pItemElem;
				if( pItemElem->m_dwItemId == 0 ) FLERROR_LOG( PROGRAM_NAME, _T( "DropQuestItem SetIndex" ) );
				pItem->SetIndex( D3DDEVICE, pItemElem->m_dwItemId );
				pItem->m_idHolder	= pUser->m_idPlayer;
				pItem->m_idOwn	= pUser->GetId();
				pItem->m_dwDropTime	= timeGetTime();
				CMover* pMover	= prj.GetMover( pScript->GetNpcId() );
				D3DXVECTOR3 vPos	= pMover->GetPos();
				vPos.x	+= ( xRandomF( 2.0f ) - 1.0f );
				vPos.z	+= ( xRandomF( 2.0f ) - 1.0f );
				pItem->SetPos( vPos );
				pItem->SetAngle( xRandom( 360 ) );
				if( pWorld->AddObj( pItem, TRUE ) == FALSE )
				{
					SAFE_DELETE( pItem );
					return 0;
				}
				return 1;
			}
		}
		else
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "DropQuestItem, %d null prop" ), v1 );
		}
	}
	return 0;
}

int ExpQuestEffect(CScript* pScript)
{
// ( dwSfx, bNPC )
	int v1, v2;
	Eval2( pScript, v1, v2 );
	GetToken();
	CCtrl* pCtrl	= ( v2? prj.GetCtrl( pScript->GetNpcId() ): prj.GetCtrl( pScript->GetPcId() ) );
	g_xWSUserManager->AddCreateSfxObj( pCtrl, v1 );
	return 1;
}

int RemoveItem( CScript* pScript )
{
	int v1, v2;
	Eval2( pScript, v1, v2 );
	GetToken();
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	return pUser->RemoveItemA( v1, v2 );
}

int Run( CScript* pScript )
{
	int v1;
	CString string;
	GetToken(); // (
	GetToken(); // key
	string = token;
	Eval1( pScript, v1 );
	GetToken(); // )

	CScriptDialog::SetBeforeRun( (const char*)string );

	CMover* pMover = prj.GetMover( pScript->GetNpcId() );
	int nResult = 0;
	pMover->m_pNpcProperty->RunDialog( string, &nResult, v1, pScript->GetNpcId(), pScript->GetPcId(), 0 );

	CScriptDialog::SetAfterRun( (const char*)string );

	return nResult;
}

int GetQuestId( CScript* pScript )
{
	Get2Tkn( pScript );
	return pScript->GetQuest();
}
int GetLang( CScript* pScript )
{
	Get2Tkn( pScript );
	return g_xFlyffConfig->GetMainLanguage();
}

// ĳ���� �� ����
int	QuerySetPlayerName( CScript* pScript )
{
	Get2Tkn( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId()  );
	if( pUser->IsAuthHigher( AUTH_GAMEMASTER ) || pUser->IsMode( QUERYSETPLAYERNAME_MODE ) )
	{
		RunScriptFunc rsf;
		rsf.wFuncType	= FUNCTYPE_QUERYSETPLAYERNAME;
		pUser->AddRunScriptFunc( rsf );
		return 1;
	}
	return 0;
}

int Trace(CScript* pScript)
{
	int array[32], i	= 0;
	CString strToken;
	CString string;
	GetToken(); // (
	GetToken(); // lpszStr
	strToken	= token;
	GetToken(); 
	while( *token != ')' )
	{
		Compute( &array[i++] );
		GetToken();
	}
	string.FormatV( strToken, (va_list)array );

	FLTRACE_LOG( PROGRAM_NAME, _T( "%s" ), string );
	return 1;
}


int GetPlayerExpPercent( CScript* pScript )  
{
	Get2Tkn( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	return pUser->GetExpPercent();
}

int SetLevel( CScript* pScript )  
{
	int v1	= EvalExp( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	return pUser->SetLevel(v1);
}

int AddGPPoint( CScript* pScript )  
{
	int v1	= EvalExp( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	return pUser->AddGPPoint(v1);
}
#ifdef SCRIPT_ENTER_INSTANCEDUNGEON
int EnterInstanceDungeon( CScript* pScript ) 
{
	int v1, v2, v3, v4;
	Eval2( pScript, v1, v2, v3, v4 );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	return pUser->EnterInstanceDungeon(v1, v2, v3, v4);
}

int	ChkPartyLowLevel( CScript* pScript )
{
	int v1	= EvalExp( pScript );
	FLWSUser* pUser	= prj.GetUser( pScript->GetPcId() );
	int nChk	= 0;
	CParty* pParty	= g_PartyMng.GetParty( pUser->m_idparty );
	if( pParty )
		nChk	= pParty->ReplaceChkLowLv(v1);
	return nChk;
}
#endif	// SCRIPT_ENTER_INSTANCEDUNGEON
//
// �Լ� ���̺�
//
InterFuncType interFunc[] = 
{
	"GetVal"            ,	GetVal            ,
	"GetSrcId"          ,	GetSrcId          ,
	"GetDstId"          ,	GetDstId          ,
	"NpcId"             ,	NpcId             ,
	"PcId"              ,	PcId              ,
	"Src"               ,	GetSrcId          ,
	"Dst"               ,	GetDstId          ,
	"PC"                ,	GetSrcId          ,
	"NPC"               ,	GetDstId          ,
	"Say"               ,	Say               ,
	"SayName"           ,	SayName           ,
	"AddSay"            ,	Say               ,
	"EndSay"            ,	EndSay            ,
	"Speak"             ,	Speak             ,
	"SpeakName"         ,	SpeakName         ,
	"Random"            ,	Random            ,
	"Replace"			,	Replace           ,
	"ReplaceKey"		,	ReplaceKey        ,
	"GetPlayerLvl"        , GetPlayerLvl      ,//( Id ) ĳ������ ������ ���´�.
	"GetPlayerJob"        , GetPlayerJob      ,//( Id ) ĳ������ ������ ���´�. defineJob�� �ѹ�
	"SetQuestState"       , SetQuestState     ,//( Id ) �ش� �̺�Ʈ�� ��������ش�.
	"GetQuestState"       , GetQuestState     ,//( Id ) �ش� �̺�Ʈ�� ���������� Ȯ���Ѵ�.
	"IsSetQuest"          , IsSetQuest        ,//( Id ) �ش� �̺�Ʈ�� ���������� Ȯ���Ѵ�.
	"SetQuest"            , SetQuest          ,//( Id ) �ش� �̺�Ʈ�� ���������� Ȯ���Ѵ�.
	"RemoveQuest"         , RemoveQuest       ,//( Id ) �ش� �̺�Ʈ�� ���������� Ȯ���Ѵ�.
	"AddExp"              , AddExp            ,// ����ġ �� 
	"CreateItem"          , CreateItem        ,//( Id ) �ش� �̺�Ʈ�� ���������� Ȯ���Ѵ�.
	"AddGold"         ,		AddGold           ,//( Id ) �ش� �̺�Ʈ�� ���������� Ȯ���Ѵ�.
	"RemoveGold"	,		RemoveGold	      ,
	"GetPlayerGold"		,	GetPlayerGold	  ,
	"QuerySetPlayerName",	QuerySetPlayerName,// �÷��̾� �� ����
	"IsGuildQuest"	,		IsGuildQuest	  ,
	"GetGuildQuestState",	GetGuildQuestState,// ( nQuestId )
	"IsWormonServer"	,	IsWormonServer    ,// ()
	"MonHuntStart"	,		MonHuntStart      ,// ( nQuestId, nState, ns, nf )
	"MonHuntStartParty"	,	MonHuntStartParty ,// ( nQuestId, nState, ns, nf )
	"DropQuestItem"	,		DropQuestItem	  ,// ( dwItemId, dwProbability )
	"ExpQuestEffect"	,	ExpQuestEffect	  ,// ( dwSfx, bNPC )
	"RemoveItem"	,		RemoveItem	      ,//( dwItemId, num )
	"GetLocalEventState",	GetLocalEventState,//��������(�����̺�Ʈ)���� 
	"InitStat",				InitStat          ,// ���� �ʱ�ȭ
	"InitStr",				InitStr           ,// ���� Str �ʱ�ȭ
	"InitSta",				InitSta           ,// ���� Sta �ʱ�ȭ
	"InitDex",				InitDex           ,// ���� Dex �ʱ�ȭ
	"InitInt",				InitInt           ,// ���� Int �ʱ�ȭ
	"RandomIndex"         , RandomIndex       ,//( Id ) �ش� �̺�Ʈ�� ���������� Ȯ���Ѵ�.
	"GetItemNum"        ,	GetItemNum        ,//( Id, Item_ID ) �ش� ������ �ش� �������� �κ��丮�� � ������ �ִ��� Ȯ���Ѵ�. ���â ����
	"RemoveAllItem"     ,	RemoveAllItem     ,//( Id, Item_ID) �ش� ������ �ش� �������� ��� ���� �Ѵ�.
	"ChangeJob"         ,	ChangeJob         ,//( Id ) �ش� �ѹ��� ������ �������ش�. defineJob�� �ѹ�
	"GetPlayerSex"      ,	GetPlayerSex      ,// ( ID ) �ش� ������ ���� �������� Ȯ�� defineAttribute.h�� ����
	"EquipItem"         ,	EquipItem         ,//( ID, Item_ID) �ش� �������� �ش� �������� �������� �ɷ�ġ�� �ȵǸ� �κ��丮�� �־��ָ� �κ��丮�� ������ �ƹ��ų� ���� ����߸� �����ϰ� �������� �����۵� �����ϰ� ó��
	"IsPlayerSkillPoint",	IsPlayerSkillPoint,// �ش� ������ m_dwSkillPoint���� ������ TRUE, 0�̸� FALSE
	"SetScriptTimer"    ,	SetScriptTimer    ,
	"AddKey"           ,	AddKey            ,
	"AddAnswer"          ,	AddAnswer         ,
	"RemoveKey"        ,	RemoveKey         ,
	"RemoveAllKey"     ,	RemoveAllKey      ,
	"SetMark"          ,	SetMark           ,
	"GoMark"           ,	GoMark            ,
	"IsParty"        ,		IsParty           ,
	"IsPartyMaster"  ,		IsPartyMaster     ,
	"GetPartyNum"	,		GetPartyNum	      ,
	"GetPartyLevel"	,		GetPartyLevel     ,
	"IsPartyGuild"	,		IsPartyGuild	  ,	// ���Ἲ�� üũ���� 
	"IsGuild"        ,		IsGuild           ,
	"IsGuildMaster"  ,		IsGuildMaster     ,
	"CreateGuild"    ,		CreateGuild       ,
	"DestroyGuild"   ,		DestroyGuild      ,
	"PlaySound"        ,	PlaySound         ,
	"GetLang"          ,	GetLang           ,
	"Run"	            ,	Run               ,
	"GetQuestId"        ,	GetQuestId        ,
	"BeginQuest"        ,	BeginQuest        ,
	"EndQuest"          ,	EndQuest          ,
	"AddQuestKey"       ,	AddQuestKey       ,
	"SayQuest"          ,	SayQuest          ,
	"PrintSystemMessage",	PrintSystemMessage,
	"GetDay"            ,	GetDay            , 
	"GetMin"            ,	GetMin            , 
	"GetHour"           ,	GetHour           , 
	"SetTimer"          ,	SetTimer          , 
	"CreateChar"        ,	CreateChar        , 
	"RemoveMover"       ,	RemoveMover       , 
	"AddCondKey"        ,	AddCondKey        , 
	"GetEmptyInventoryNum", GetEmptyInventoryNum,
	"GetGValue"			,	GetGValue_		  , //�׽�Ʈ�� 
	"SetGValue"			,	SetGValue		  , //�׽�Ʈ�� 
	"AddGValue"			,	AddGValue		  , //�׽�Ʈ�� 
	"SysMessage"        ,	SysMessage        , //�׽�Ʈ�� 
	"CreateMover"       ,	CreateMover       , //�׽�Ʈ��  
	"IsTimeOut"         ,	IsTimeOut         , //�׽�Ʈ�� 
	"LaunchQuest"       ,	LaunchQuest       , //�׽�Ʈ�� 
	"GetParam"          ,	GetParam          , //�׽�Ʈ�� 
	"Trace"             ,	Trace             ,
	"Exit"              ,	Exit              ,
	"GetPlayerExpPercent"        , GetPlayerExpPercent      ,//( Id ) ĳ������ exp�� %�� ���´�. 10000
	"SetLevel"        , SetLevel      ,						//�ɸ����� ������ �����
	"AddGPPoint"        , AddGPPoint      ,					//ĳ���Ϳ� GPPOINT�� �����ش�.
#ifdef SCRIPT_ENTER_INSTANCEDUNGEON
	"EnterInstanceDungeon",	EnterInstanceDungeon,			//�ν��Ͻ� ������ �����Ѵ�.(��Ƽ)
	"ChkPartyLowLevel", ChkPartyLowLevel,
#endif	// SCRIPT_ENTER_INSTANCEDUNGEON
	0                   ,   0                      
};
#endif // #if !defined(__REMOVE_SCIRPT_060712)