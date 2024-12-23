#include "stdafx.h"
#include "party.h"
#include "../WorldServer/User.h"
#include "guildquest.h"
#include "worldmng.h"
#include "guild.h"
#include "ScriptHelper.h"
#include "../WorldServer/WorldDialog.h"

#include "../_Common/FLSkillSystem.h"

extern	CPartyMng			g_PartyMng;
extern	CWorldMng			g_WorldMng;
extern	CGuildMng			g_GuildMng;


#if defined(__REMOVE_SCIRPT_060712)

enum QUEST_ENUM
{
	QE_QUEST_END,
	QE_QUEST_BEGIN,
	QE_QUEST_BEGIN_YES,
	QE_QUEST_BEGIN_NO,
	QE_QUEST_END_COMPLETE
	, QE_QUEST_NEXT_LEVEL
	, QE_QUEST_END_FAIL
};

static map< string, QUEST_ENUM > g_mapQuestKey;

// 미리 정의된 키들을 위한 table을 만든다.
void InitPredefineKey()
{
	g_mapQuestKey[ "QUEST_END" ]          = QE_QUEST_END;
	g_mapQuestKey[ "QUEST_BEGIN" ]        = QE_QUEST_BEGIN;
	g_mapQuestKey[ "QUEST_BEGIN_YES" ]    = QE_QUEST_BEGIN_YES;
	g_mapQuestKey[ "QUEST_BEGIN_NO" ]     = QE_QUEST_BEGIN_NO;
	g_mapQuestKey[ "QUEST_END_COMPLETE" ] = QE_QUEST_END_COMPLETE;
	g_mapQuestKey[ "QUEST_NEXT_LEVEL" ]   = QE_QUEST_NEXT_LEVEL;
	g_mapQuestKey[ "QUEST_END_FAIL" ]     = QE_QUEST_END_FAIL;
}

BOOL RunPredefineKey( LPCTSTR szKey, CNpcDialogInfo& info )
{
	int& nGlobal = *( info.GetGlobal() );

	map< string, QUEST_ENUM >::iterator it = g_mapQuestKey.find( szKey );
	if( it == g_mapQuestKey.end() )
		return FALSE;
	
	QUEST_ENUM type = it->second;
	switch( type )
	{
	case QE_QUEST_END:
		__QuestEnd( info.GetPcId(), info.GetNpcId(), nGlobal, info.GetQuest(), FALSE );
		break;
	case QE_QUEST_BEGIN:
		__QuestBegin( info.GetPcId(), info.GetNpcId(), info.GetQuest() );
		break;
	case QE_QUEST_BEGIN_YES:
		__QuestBeginYes( info.GetPcId(), info.GetNpcId(), info.GetQuest() );
		break;
	case QE_QUEST_BEGIN_NO:
		__QuestBeginNo( info.GetPcId(), info.GetNpcId(), info.GetQuest() );
		break;
	case QE_QUEST_END_COMPLETE:
		__QuestEndComplete( info.GetPcId(), info.GetNpcId(), nGlobal, info.GetReturn(), info.GetQuest() );
		break;
	case QE_QUEST_END_FAIL:
		__QuestEnd( info.GetPcId(), info.GetNpcId(), nGlobal, info.GetQuest(), TRUE );
		break;
	default:
		return FALSE;
	};

	return TRUE;
}
#endif // #if defined(__REMOVE_SCIRPT_060712)


	
CWorld* __GetWorld( int nPcId, int nSrcId, int nCaller )
{
	CWorld* pWorld = NULL;
	if( nCaller == WES_DIALOG )
	{
		FLWSUser* pUser = prj.GetUser( nPcId );
		pWorld = pUser->GetWorld();
	}
	else
	if( nCaller == WES_WORLD )
		pWorld = g_WorldMng.GetWorld( nSrcId );

	return pWorld;
}

int __AddKey( int nPcId, LPCTSTR lpszWord, LPCTSTR lpszKey, DWORD dwParam )
{
	CHAR szKey[ 128 ] = { 0, }, szWord[ 128 ] = { 0, };

	FLStrcpy( szWord, _countof( szWord ), lpszWord );

	if( lpszKey[0] == '\0' ) 
		FLStrcpy( szKey, _countof( szKey ), szWord );
	else
		FLStrcpy( szKey, _countof( szKey ), lpszKey );

	FLWSUser* pUser	= prj.GetUser( nPcId );
	RunScriptFunc rsf;
	rsf.wFuncType		= FUNCTYPE_ADDKEY;
	FLStrcpy( rsf.lpszVal1, _countof( rsf.lpszVal1 ), szWord );
	FLStrcpy( rsf.lpszVal2, _countof( rsf.lpszVal2 ), szKey );
	rsf.dwVal1	= dwParam;
	rsf.dwVal2 = 0;
	pUser->AddRunScriptFunc( rsf );
	return 1;
}

int __AddAnswer( int nPcId, LPCTSTR lpszWord, LPCTSTR lpszKey, DWORD dwParam1, int nQuest )
{
	CHAR szKey[ 128 ] = { 0, }, szWord[ 128 ] = { 0, };

	FLStrcpy( szWord, _countof( szWord ), lpszWord );

	if( lpszKey[0] == '\0' ) 
		FLStrcpy( szKey, _countof( szKey ), szWord );
	else
		FLStrcpy( szKey, _countof( szKey ), lpszKey );

	FLWSUser* pUser = prj.GetUser( nPcId );
	RunScriptFunc rsf;
	rsf.wFuncType		= FUNCTYPE_ADDANSWER;
	FLStrcpy( rsf.lpszVal1, _countof( rsf.lpszVal1 ), szWord );
	FLStrcpy( rsf.lpszVal2, _countof( rsf.lpszVal2 ), szKey );
	rsf.dwVal1 = (DWORD)dwParam1;
	rsf.dwVal2 = nQuest;
	pUser->AddRunScriptFunc( rsf );
	return 1;
}

int __RemoveQuest( int nPcId, int nQuest )      
{
	QuestProp* pQuestProp	= prj.m_aPropQuest.GetAt( nQuest );
	if( pQuestProp )
	{
		FLWSUser* pUser	= prj.GetUser( nPcId );
		LPQUEST pQuest	= pUser->GetQuest( nQuest );

		if( pQuest && pQuest->m_nState < 14 ) 
			pUser->AddDefinedText( TID_EVE_CANCELQUEST, "\"%s\"", pQuestProp->m_szTitle );
		pUser->RemoveQuest( nQuest );
		pUser->AddRemoveQuest( nQuest );
		// 시작시 변신을 했으면 퀘스트 삭제시 변신 해제시킨다.
		if( pQuest && pQuestProp->m_nBeginSetDisguiseMoverIndex )
		{
			FLWSUser* pUser = prj.GetUser( nPcId );
			pUser->NoDisguise( NULL );
			g_xWSUserManager->AddNoDisguise( pUser );
		}
	}
	return 1;
}

int __RemoveAllKey( int nPcId )
{
	FLWSUser* pUser	= prj.GetUser( nPcId );
	RunScriptFunc rsf;
	rsf.wFuncType		= FUNCTYPE_REMOVEALLKEY;
	pUser->AddRunScriptFunc( rsf );

	return 1;
}

int __SayQuest( int nPcId,int nQuestId, int nIdx )
{
	CString strToken;

	QuestProp* pQuestProp = prj.m_aPropQuest.GetAt( nQuestId );
	if( pQuestProp && pQuestProp->m_apQuestDialog[ nIdx ] )
		strToken = pQuestProp->m_apQuestDialog[ nIdx ];
	if( strToken.IsEmpty() == TRUE )
		return FALSE;

	FLWSUser* pUser = prj.GetUser( nPcId );
	RunScriptFunc rsf;
	rsf.wFuncType		= FUNCTYPE_SAY;//QUEST;
	FLStrcpy( rsf.lpszVal1, _countof( rsf.lpszVal1 ), strToken );
	rsf.dwVal2 = nQuestId;
	pUser->AddRunScriptFunc( rsf );
	return 1;
}
int __RunQuest( int nPcId, int /*nNpcId*/, int nQuestId )
{
//	CMover* pMover = prj.GetMover( nNpcId );
	QuestProp* pQuestProp = prj.m_aPropQuest.GetAt( nQuestId );

	if( pQuestProp )
	{
		for( int  i = 0; i < 4; i++ )
		{
			if( pQuestProp->m_nBeginSetAddItemIdx[ i ] )
			{
				__CreateItem( nPcId, pQuestProp->m_nBeginSetAddItemIdx[ i ], pQuestProp->m_nBeginSetAddItemNum[ i ] );
			}
		}
		if( pQuestProp->m_nBeginSetAddGold )
		{
			__AddGold( nPcId, pQuestProp->m_nBeginSetAddGold );
		}

		// 퀘스트 시작시 변신함 
		if( pQuestProp->m_nBeginSetDisguiseMoverIndex )
		{
			FLWSUser* pUser = prj.GetUser( nPcId );
			pUser->Disguise( NULL, pQuestProp->m_nBeginSetDisguiseMoverIndex );
			g_xWSUserManager->AddDisguise( pUser, pQuestProp->m_nBeginSetDisguiseMoverIndex );
		}
		__SetQuest( nPcId, nQuestId );
	}
	return 1;
}

//int __EndQuest( int nPcId, int nQuestId, BOOL IsEndQuestCondition = TRUE )
int __EndQuest( int nPcId, int nQuestId, BOOL IsEndQuestCondition )
{
	int nNum;
	QuestProp* pQuestProp = prj.m_aPropQuest.GetAt( nQuestId );

	if( pQuestProp )
	{
		FLWSUser* pUser = prj.GetUser( nPcId );
		if( IsEndQuestCondition )
		{
			if( __IsEndQuestCondition( pUser, nQuestId ) == false )
				return FALSE;
		}

		////////////////////////////
		// 삭제 될 아이템이 사용중이거나 착용 중인가?
		////////////////////////////
		for( size_t idx = 0; idx < _countof( pQuestProp->m_dwEndRemoveItemIdx ); ++idx )
		{
			if( pQuestProp->m_dwEndRemoveItemIdx[ idx ] )
			{
				for( DWORD c = 0 ; c < pUser->m_Inventory.GetMax(); ++c )
				{
					FLItemElem * pItemElem = pUser->m_Inventory.GetAtId( c );
					if( pItemElem && pItemElem->m_dwItemId == pQuestProp->m_dwEndRemoveItemIdx[ idx ] )
					{
						if( pUser->IsUsing( pItemElem ) || pUser->m_Inventory.IsEquip( pItemElem->m_dwObjId ) )
						{
							pUser->AddDefinedText( TID_MMI_USING_QUEST_GIFT_ITEM );
							RunScriptFunc rsf;
							rsf.wFuncType		= FUNCTYPE_EXIT;
							pUser->AddRunScriptFunc( rsf );
							return FALSE;
						}
					}
				}
			}
		}


		// 보상 아이템 갯수 만큼, 인벤토리 여유가 있는가?
		vector<T_ADDITEM_INFO> vecAddItemInfo;
		vecAddItemInfo.clear();

		for( int i = 0; i < pQuestProp->m_nEndRewardItemNum; i++ )
		{
			QuestPropItem* pEndRewardItem = &pQuestProp->m_paEndRewardItem[ i ];
			if( pEndRewardItem->m_nSex == -1 || pEndRewardItem->m_nSex == pUser->GetSex() )
			{
				if( pEndRewardItem->m_nType == 0 )
				{
					if( pEndRewardItem->m_nJobOrItem == -1 || pEndRewardItem->m_nJobOrItem == pUser->GetJob() )
					{
						if( pEndRewardItem->m_nItemIdx )
						{
							T_ADDITEM_INFO tAddItemInfo;

							tAddItemInfo.dwItemId	= pEndRewardItem->m_nItemIdx;
							tAddItemInfo.nItemNum	= pEndRewardItem->m_nItemNum;

							vecAddItemInfo.push_back( tAddItemInfo );
						}
					}
				}
				else
				if( pEndRewardItem->m_nType == 1 )
				{
					if( pEndRewardItem->m_nJobOrItem == -1 || pUser->GetItemNum( pEndRewardItem->m_nJobOrItem ) )
					{
						if( pEndRewardItem->m_nItemIdx )
						{
							T_ADDITEM_INFO tAddItemInfo;

							tAddItemInfo.dwItemId	= pEndRewardItem->m_nItemIdx;
							tAddItemInfo.nItemNum	= pEndRewardItem->m_nItemNum;

							vecAddItemInfo.push_back( tAddItemInfo );
						}
					}
				}
			}
		}

		if( pUser->m_Inventory.IsEnoughEmptySpace( vecAddItemInfo ) == false ) 
		{
			pUser->AddDefinedText( TID_QUEST_NOINVENTORYSPACE ); // 인벤토리 공간이 없어서 퀘스트를 완료할 수 없습니다.
			RunScriptFunc rsf;
			rsf.wFuncType		= FUNCTYPE_EXIT;
			pUser->AddRunScriptFunc( rsf );
			return FALSE;
		}



		////////////////////////////
		// 보상 지급 
		////////////////////////////
		for( int i = 0; i < pQuestProp->m_nEndRewardItemNum; i++ )
		{
			QuestPropItem* pEndRewardItem = &pQuestProp->m_paEndRewardItem[ i ];
			if( pEndRewardItem->m_nSex == -1 || pEndRewardItem->m_nSex == pUser->GetSex() )
			{
				if( pEndRewardItem->m_nType == 0 )
				{
					if( pEndRewardItem->m_nJobOrItem == -1 || pEndRewardItem->m_nJobOrItem == pUser->GetJob() )
					{
						if( pEndRewardItem->m_nItemIdx )
						{
							int nItemNum = pEndRewardItem->m_nItemNum;
							__CreateItem( nPcId, pEndRewardItem->m_nItemIdx, nItemNum, pEndRewardItem->m_nAbilityOption, pEndRewardItem->m_byFlag );
						}
					}
				}
				else
				if( pEndRewardItem->m_nType == 1 )
				{
					if( pEndRewardItem->m_nJobOrItem == -1 || pUser->GetItemNum( pEndRewardItem->m_nJobOrItem ) )
					{
						if( pEndRewardItem->m_nItemIdx )
						{
							int nItemNum = pEndRewardItem->m_nItemNum;
							__CreateItem( nPcId, pEndRewardItem->m_nItemIdx, nItemNum, pEndRewardItem->m_nAbilityOption, pEndRewardItem->m_byFlag );
						}
					}
				}
			}
		}

		if( pQuestProp->m_bEndRewardPetLevelup )
			pUser->PetLevelup();

		if(	pQuestProp->m_nEndRewardGoldMin )
		{
			nNum = pQuestProp->m_nEndRewardGoldMax - pQuestProp->m_nEndRewardGoldMin + 1; 
			int nGold = pQuestProp->m_nEndRewardGoldMin + xRandom( nNum ); 
			pUser->AddGold( nGold );
			pUser->AddGoldText( nGold );
		}
		if(	pQuestProp->m_n64EndRewardExpMin > 0 )
		{
			EXPINTEGER n64Gap = pQuestProp->m_n64EndRewardExpMax - pQuestProp->m_n64EndRewardExpMin + 1;
			if( n64Gap > 0 )
			{
				EXPINTEGER n64Exp = pQuestProp->m_n64EndRewardExpMin + (EXPINTEGER)( xRandom( (DWORD)n64Gap ) );
				if( pUser->AddExperience( n64Exp, TRUE, FALSE ) )
					pUser->LevelUpSetting();
				else
					pUser->ExpUpSetting();

				pUser->AddSetExperience( pUser->GetExp1(), (WORD)pUser->m_nLevel, pUser->m_nSkillPoint, pUser->m_nSkillLevel );
				pUser->AddDefinedText( TID_GAME_REAPEXP );
			}
		}
		if( pQuestProp->m_nEndRewardSkillPoint )
		{
			pUser->AddSkillPoint( pQuestProp->m_nEndRewardSkillPoint );
			pUser->AddSetExperience( pUser->GetExp1(), (WORD)pUser->m_nLevel, pUser->m_nSkillPoint, pUser->m_nSkillLevel );
			g_dpDBClient.SendLogSkillPoint( LOG_SKILLPOINT_GET_QUEST, pQuestProp->m_nEndRewardSkillPoint, (CMover*)pUser, NULL );
		}

		if( pQuestProp->m_nEndRewardPKValueMin || pQuestProp->m_nEndRewardPKValueMax )
		{
			if( pQuestProp->m_nEndRewardPKValueMin <= pQuestProp->m_nEndRewardPKValueMax )
			{
				int nPKValue = pUser->GetPKValue() - xRandom( pQuestProp->m_nEndRewardPKValueMin, pQuestProp->m_nEndRewardPKValueMax + 1 );
				if( nPKValue < 0 )
					nPKValue = 0;
				pUser->SetPKValue( nPKValue );
				pUser->AddPKValue();
				g_dpDBClient.SendLogPkPvp( (CMover*)pUser, NULL, 0, 'P' );
				pUser->CheckHonorStat();
				g_xWSUserManager->AddHonorTitleChange( pUser, pUser->m_nHonor);
			}
		}

		if( pQuestProp->m_nEndRewardTeleport != 0 )
			pUser->REPLACE( g_uIdofMulti, pQuestProp->m_nEndRewardTeleport, pQuestProp->m_nEndRewardTeleportPos, REPLACE_NORMAL, nTempLayer );

		// 시작시 변신을 했으면 종료시 변신 해제시킨다.
		if( pQuestProp->m_nBeginSetDisguiseMoverIndex )
		{
			FLWSUser* pUser = prj.GetUser( nPcId );
			pUser->NoDisguise( NULL );
			g_xWSUserManager->AddNoDisguise( pUser );
		}

		if( pQuestProp->m_nEndRewardTSP )
			g_dpDBClient.SendUpdateCampusPoint( pUser->m_idPlayer, pQuestProp->m_nEndRewardTSP, TRUE, 'Q' );

		if( pQuestProp->m_nEndRemoveTSP )
			g_dpDBClient.SendUpdateCampusPoint( pUser->m_idPlayer, pQuestProp->m_nEndRemoveTSP, FALSE, 'Q' );

		for( size_t Nth = 0; Nth < MAX_QUEST_REWARD_BUFF; ++Nth )
		{
			SkillProp* pSkillProp		= prj.GetSkillProp( pQuestProp->m_kEndRewardSkillBuff[ Nth ].dwSkillID );
			if( pSkillProp == NULL )
			{
				break;
			}

			AddSkillProp* pAddSkillProp	= prj.GetAddSkillProp( pSkillProp->dwSubDefine, pQuestProp->m_kEndRewardSkillBuff[ Nth ].dwSkillLevel );
			if( pAddSkillProp == NULL )
			{
				break;
			}

			g_cSkillSystem->DoApplySkill( pUser, pUser, pSkillProp, pAddSkillProp );
			g_xWSUserManager->AddDoApplySkill( pUser, pUser->GetId(), pQuestProp->m_kEndRewardSkillBuff[ Nth ].dwSkillID, pQuestProp->m_kEndRewardSkillBuff[ Nth ].dwSkillLevel );
		}
#ifdef DAILY_QUEST
		// 아이템 버프
		PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( pQuestProp->m_nRewardBuffIndex );
		if( pItemProp )
		{
			if( pUser->IsDoUseBuff( pItemProp ) != 0 )
			{
				return FALSE;
			}
		}
		
		CTime timeCur	= CTime::GetCurrentTime();
		CTime timeNext = timeCur;
		if ( !pQuestProp->m_nRewardBuffTime )	// 일일퀘스트 버프일때
		{
			if ( timeCur.GetHour() < 6 )
			{
				timeNext = CTime::GetCurrentTime() + CTimeSpan( 0, 5-timeCur.GetHour(), 59-timeCur.GetMinute(), 59 );
			}
			else
			{
				timeNext = CTime::GetCurrentTime() + CTimeSpan( 0, 29-timeCur.GetHour(), 59-timeCur.GetMinute(), 59 );
			}
		}
		else
		{
			timeNext = CTime::GetCurrentTime() + CTimeSpan( 0, 0, pQuestProp->m_nRewardBuffTime, 0 );
		}

		//CTime tm	= CTime::GetCurrentTime() + CTimeSpan( 0, 0, pQuestProp->m_nEndRewardBuffTime, 0 );
		time_t t	= (time_t)( timeNext.GetTime() );
		// wID: dwItemId
		// dwLevel
		// t 값이 왜 level로 들어가지? 올바른건가?
		pUser->AddBuff( BUFF_ITEM, (WORD)( pQuestProp->m_nRewardBuffIndex ), t, 0, 0, BT_TICK );	// TODO
#endif // DAILY_QUEST
		////////////////////////////
		// 아이템 제거 
		////////////////////////////
		for( int i = 0; i < 8; i++ )
		{
			if( pQuestProp->m_dwEndRemoveItemIdx[ i ] )
			{
				int nItemNum = pQuestProp->m_nEndRemoveItemNum[ i ];
				pUser->RemoveItemA( pQuestProp->m_dwEndRemoveItemIdx[ i ], nItemNum );
			}
		}
		if( pQuestProp->m_nEndRemoveGold )
			__RemoveGold( nPcId, pQuestProp->m_nEndRemoveGold );
		////////////////////////////
		// 퀘스트 제거 (완료,진행 모두 제거함)
		////////////////////////////
		for( int i = 0; i < MAX_QUESTREMOVE; i++ )
		{
			if( pQuestProp->m_anEndRemoveQuest[ i ] )
			{
				__RemoveQuest( nPcId, pQuestProp->m_anEndRemoveQuest[ i ] );
			}
		}
		// 퀘스트 종료 
		__SetQuestState( nPcId, nQuestId, QS_END );
	}
	return 1;
}

//int __AddQuestKey( int nPcId, int nQuestId, LPCTSTR lpKey, int nParam = 0 )
int __AddQuestKey( int nPcId, int nQuestId, LPCTSTR lpKey, int nParam, BOOL bNew )
{
	CHAR szWord[ 128 ] = { 0, }, szKey[ 128 ] = { 0, };
	QuestProp* pQuestProp = prj.m_aPropQuest.GetAt( nQuestId );
	if( pQuestProp )
		FLStrcpy( szWord, _countof( szWord ), pQuestProp->m_szTitle );
	if( lpKey[0] == '\0' ) 
		FLStrcpy( szKey, _countof( szKey ), szWord );
	else
		FLStrcpy( szKey, _countof( szKey ), lpKey );

	FLWSUser* pUser	= prj.GetUser( nPcId );
	RunScriptFunc rsf;

	if( bNew )
		rsf.wFuncType		= FUNCTYPE_NEWQUEST;
	else
		rsf.wFuncType		= FUNCTYPE_CURRQUEST;

	FLStrcpy( rsf.lpszVal1, _countof( rsf.lpszVal1 ), szWord );
	FLStrcpy( rsf.lpszVal2, _countof( rsf.lpszVal2 ), szKey );
	rsf.dwVal1	= nParam;
	rsf.dwVal2 = nQuestId;
	pUser->AddRunScriptFunc( rsf );
	return 1;
}
void __QuestBegin( int nPcId, int nNpcId, int nQuestId )
{
	__SayQuest( nPcId, nQuestId, QSAY_BEGIN1 );
	__SayQuest( nPcId, nQuestId, QSAY_BEGIN2 );
	__SayQuest( nPcId, nQuestId, QSAY_BEGIN3 );
	__SayQuest( nPcId, nQuestId, QSAY_BEGIN4 );
	__SayQuest( nPcId, nQuestId, QSAY_BEGIN5 );
	__AddAnswer( nPcId, "__YES__", "QUEST_BEGIN_YES", 0, nQuestId );
	__AddAnswer( nPcId, "__NO__", "QUEST_BEGIN_NO", 0, nQuestId  );

	// 기타 대화를 위한 키를 추가한다.
	CMover* pMover = prj.GetMover( nNpcId );
	pMover->m_pNpcProperty->RunDialog( "#questBegin", NULL, 0, nNpcId, nPcId, nQuestId );
}

void __QuestEnd( int nPcId, int nNpcId, int& /*nGlobal*/, int nQuestId, BOOL bButtOK )
{
	// 현재 진행 중인 퀘스트 중에서 현재 NPC가 처리해야될 퀘스트를 실행(자신일 수도 있음)
	CMover* pMover = prj.GetMover( nNpcId );
	FLWSUser* pUser = prj.GetUser( nPcId );

	LPQUEST lpQuest;
	BOOL bNewQuest = FALSE;

	__RemoveAllKey( nPcId );
	pMover->m_pNpcProperty->RunDialog( "#addKey", NULL, 0, nNpcId, nPcId, 0 );
	
	// 퀘스트 리스트 send
	vector<int> vecNewQuest;
	vector<int> vecNextQuest;
	vector<int> vecEndQuest;
	vector<int> vecCurrQuest;
	LPCHARACTER lpChar = prj.GetCharacter( pMover->m_szCharacterKey );
	if( lpChar )
	{
		for( int i = 0; i < lpChar->m_awSrcQuest.GetSize(); i++ )
		{
			int nQuest = lpChar->m_awSrcQuest.GetAt( i );
			lpQuest = pUser->GetQuest( nQuest );

			// new quest
			if( lpQuest == NULL && pUser->IsCompleteQuest( nQuest ) == FALSE )
			{
				// now
				if( __IsBeginQuestCondition( pUser, nQuest ) )
				{
					bNewQuest = TRUE;
					vecNewQuest.push_back( nQuest );
				}
				// next
				else if( __IsNextLevelQuest( pUser, nQuest ) )
					vecNextQuest.push_back( nQuest );
			}
			// current quest
			else if( lpQuest && pUser->IsCompleteQuest( nQuest ) == FALSE && lpQuest->m_nState != QS_END )
			{
				// complete
				if( __IsEndQuestCondition( pUser, nQuest ) )
					vecEndQuest.push_back( nQuest );
				// running
				else
					vecCurrQuest.push_back( nQuest );
			}
		}

		// sort
		__QuestSort( vecNewQuest );
		__QuestSort( vecNextQuest );
		__QuestSort( vecEndQuest );
		__QuestSort( vecCurrQuest );

		// send
		for( DWORD i = 0; i < vecNewQuest.size(); ++i )
			__AddQuestKey( nPcId, vecNewQuest[ i ], "QUEST_BEGIN", 0, TRUE );
		for( DWORD i = 0; i < vecNextQuest.size(); ++i )
			__AddQuestKey( nPcId, vecNextQuest[ i ], "QUEST_NEXT_LEVEL", 0, TRUE );
		for( DWORD i = 0; i < vecEndQuest.size(); ++i )
			__AddQuestKey( nPcId, vecEndQuest[ i ], "QUEST_END", 0, FALSE );
		for( DWORD i = 0; i < vecCurrQuest.size(); ++i )
			__AddQuestKey( nPcId, vecCurrQuest[ i ], "QUEST_END", 0, FALSE );
	}
	
	BOOL bDialogText = TRUE;
	BOOL bCompleteCheck = TRUE;
	
	if( nQuestId )
	{
		lpQuest = pUser->GetQuest( nQuestId );
		QuestProp* pQuestProp = prj.m_aPropQuest.GetAt( nQuestId );
		if( lpQuest && lpQuest->m_nState != QS_END && pQuestProp )	// 진행중인 퀘스트 선택 시
		{
			bCompleteCheck = FALSE;
			if( !bButtOK ) // 퀘스트 목록을 선택하고 들어온 경우
			{
				if( _strcmpi( pQuestProp->m_szEndCondCharacter, pMover->m_szCharacterKey ) == 0 && __IsEndQuestCondition( pUser, nQuestId ) )
				{
					__processCompleteQuest( nPcId, nQuestId );
					//__SayQuest( nPcId, nQuestId, QSAY_END_COMPLETE1 );
					//__SayQuest( nPcId, nQuestId, QSAY_END_COMPLETE2 );
					//__SayQuest( nPcId, nQuestId, QSAY_END_COMPLETE3 );
					//__AddAnswer( nPcId,"__OK__", "QUEST_END_COMPLETE", 0, nQuestId );
				}
				else
				{
					__SayQuest( nPcId, nQuestId, QSAY_END_FAILURE1 );
					__SayQuest( nPcId, nQuestId, QSAY_END_FAILURE2 );
					__SayQuest( nPcId, nQuestId, QSAY_END_FAILURE3 );
					__AddAnswer( nPcId,"__OK__", "QUEST_END_FAIL", 0, nQuestId );
				}
				bDialogText = FALSE;
			}
		}
	}
	
	// 완료 가능한 퀘스트가 있는지 검사
	if( bCompleteCheck )
	{
		for( size_t i = 0; i < pUser->m_nQuestSize; ++i )
		{
			lpQuest = &pUser->m_aQuest[i];
			if( lpQuest )
			{
				QuestProp* pQuestProp = prj.m_aPropQuest.GetAt( lpQuest->m_wId );
				if( pQuestProp )
				{
					if( _strcmpi( pQuestProp->m_szEndCondCharacter, pMover->m_szCharacterKey ) == 0 && lpQuest->m_nState != QS_END )
					{
						if( __IsEndQuestCondition( pUser, lpQuest->m_wId ) )
						{
							__processCompleteQuest( nPcId, lpQuest->m_wId );
							//__SayQuest( nPcId, lpQuest->m_wId, QSAY_END_COMPLETE1 );
							//__SayQuest( nPcId, lpQuest->m_wId, QSAY_END_COMPLETE2 );
							//__SayQuest( nPcId, lpQuest->m_wId, QSAY_END_COMPLETE3 );
							//__AddAnswer( nPcId,"__OK__", "QUEST_END_COMPLETE", 0, lpQuest->m_wId );
							bDialogText = FALSE;
							break;
						}
					}
				}
			}
		}
	}

	if( bDialogText )
	{
		if( bNewQuest )
		{
			if( vecNewQuest.size() == 1 && bCompleteCheck )	// 진행가능한 퀘스트가 하나면 바로 퀘스트 수락창 표시
				__QuestBegin( nPcId, nNpcId, vecNewQuest[ 0 ] );
			else
				pMover->m_pNpcProperty->RunDialog( "#yesQuest", NULL, 0, nNpcId, nPcId, 0 ); // 준비된 퀘스트가 있을 때의 인사말
		}
		else
			pMover->m_pNpcProperty->RunDialog( "#noQuest", NULL, 0, nNpcId, nPcId, 0 );	// 준비된 퀘스트가 없을 때의 인사말
	}
}
void __QuestBeginYes( int nPcId, int nNpcId, int nQuestId )
{
	FLWSUser* pUser	= prj.GetUser( nPcId );

	//	mulcom	BEGIN100315	베트남 시간 제한
	if( g_xFlyffConfig->GetMainLanguage() == LANG_VTN )
	{
		if( pUser->IsPlayer() == TRUE )
		{
			if( pUser->m_nAccountPlayTime > MIN( 180 ) )
			{
				pUser->AddDefinedText( TID_GAME_ERROR_QUEST_1 );

				return;
			}
		}
	}
	//	mulcom	END100315	베트남 시간 제한

#ifdef DAILY_QUEST
	QuestProp* pQuestProp = prj.m_aPropQuest.GetAt( nQuestId );
	if( pQuestProp )
	{
		PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( pQuestProp->m_nCheckBuffIndex );
		if( pItemProp )
		{
			if( pUser->IsDoUseBuff( pItemProp ) )		
			return;
		}
		
		
	}
#endif	// DAILY_QUEST
	LPQUEST lpQuest		= pUser->GetQuest( nQuestId );
	if( __IsBeginQuestCondition( pUser, nQuestId )
		&& lpQuest == NULL
		&& pUser->IsCompleteQuest( nQuestId ) == FALSE )
	{
		if( pUser->m_nQuestSize >= MAX_DOING_QUEST )
		{
			pUser->AddDefinedText( TID_MMI_QUEST_ESCESS );
			return;
		}
		__SayQuest( nPcId, nQuestId, QSAY_BEGIN_YES );
		__RunQuest( nPcId, nNpcId, nQuestId );
		CMover* pMover = prj.GetMover( nNpcId );
		__RemoveAllKey( nPcId );
		pMover->m_pNpcProperty->RunDialog( "#addKey", NULL, 0, nNpcId, nPcId, 0 );
		LPQUEST lpQuestList;
		// 퀘스트 리스트 send
		vector<int> vecNewQuest;
		vector<int> vecNextQuest;
		vector<int> vecEndQuest;
		vector<int> vecCurrQuest;
		LPCHARACTER lpChar = prj.GetCharacter( pMover->m_szCharacterKey );
		if( lpChar )
		{
			for( int i = 0; i < lpChar->m_awSrcQuest.GetSize(); i++ )
			{
				int nQuest = lpChar->m_awSrcQuest.GetAt( i );
				lpQuestList = pUser->GetQuest( nQuest );
				
				// new quest
				if( lpQuestList == NULL && pUser->IsCompleteQuest( nQuest ) == FALSE )
				{
					// now
					if( __IsBeginQuestCondition( pUser, nQuest ) )
						vecNewQuest.push_back( nQuest );
					// next
					else if( __IsNextLevelQuest( pUser, nQuest ) )
						vecNextQuest.push_back( nQuest );
				}
				// current quest
				else if( lpQuestList && pUser->IsCompleteQuest( nQuest ) == FALSE )
				{
					// complete
					if( __IsEndQuestCondition( pUser, nQuest ) )
						vecEndQuest.push_back( nQuest );
					// running
					else
						vecCurrQuest.push_back( nQuest );
				}
			}
			
			// sort
			__QuestSort( vecNewQuest );
			__QuestSort( vecNextQuest );
			__QuestSort( vecEndQuest );
			__QuestSort( vecCurrQuest );
			
			// send
			for( DWORD i = 0; i < vecNewQuest.size(); ++i )
				__AddQuestKey( nPcId, vecNewQuest[ i ], "QUEST_BEGIN", 0, TRUE );
			for( DWORD i = 0; i < vecNextQuest.size(); ++i )
				__AddQuestKey( nPcId, vecNextQuest[ i ], "QUEST_NEXT_LEVEL", 0, TRUE );
			for( DWORD i = 0; i < vecEndQuest.size(); ++i )
				__AddQuestKey( nPcId, vecEndQuest[ i ], "QUEST_END", 0, FALSE );
			for( DWORD i = 0; i < vecCurrQuest.size(); ++i )
				__AddQuestKey( nPcId, vecCurrQuest[ i ], "QUEST_END", 0, FALSE );
		}

		pMover->m_pNpcProperty->RunDialog( "#questBeginYes", NULL, 0, nNpcId, nPcId, nQuestId );
	}

}
void __QuestBeginNo( int nPcId, int nNpcId, int nQuestId )
{
	__SayQuest( nPcId, nQuestId, QSAY_BEGIN_NO );
	CMover* pMover = prj.GetMover( nNpcId );
	pMover->m_pNpcProperty->RunDialog( "#questBeginNo", NULL, 0, nNpcId, nPcId, nQuestId );
}
void __QuestEndComplete( int nPcId, int nNpcId, int& nGlobal, int /*nVal*/, int nQuestId )
{
	FLWSUser* pUser	= prj.GetUser( nPcId );
	LPQUEST lpQuest		= pUser->GetQuest( nQuestId );

	//	mulcom	BEGIN100315	베트남 시간 제한
	if( g_xFlyffConfig->GetMainLanguage() == LANG_VTN )
	{
		if( pUser->IsPlayer() == TRUE )
		{
			if( pUser->m_nAccountPlayTime > MIN( 180 ) )
			{
				pUser->AddDefinedText( TID_GAME_ERROR_QUEST_1 );
				return;
			}
		}
	}
	//	mulcom	END100315	베트남 시간 제한

	// 퀘스트가 진행 중인 경우 
	if( lpQuest && pUser->IsCompleteQuest( nQuestId ) == FALSE
		&& __IsEndQuestCondition( pUser, nQuestId ) )
	{
		CMover* pMover = prj.GetMover( nNpcId );
		if( __EndQuest( nPcId, nQuestId ) == TRUE )
			pMover->m_pNpcProperty->RunDialog( "#questEndComplete", NULL, 0, nNpcId, nPcId, nQuestId );

		__QuestEnd( nPcId, nNpcId, nGlobal, nQuestId, TRUE );

#ifdef __GUILD_HOUSE_MIDDLE
		GuildHouseMng->CheckGuildHouseQuest( pUser, nQuestId );
#endif // __GUILD_HOUSE_MIDDLE
	}	
}
int __SetQuestState( DWORD dwIdMover, int nQuest, int nState )
{
	FLWSUser* pUser	= prj.GetUser( dwIdMover );
	if( nState == QS_END )
	{
		QuestProp* pQuestProp = prj.m_aPropQuest.GetAt( nQuest );
		if( pQuestProp )
		{
			if( nQuest != QUEST_INDEX( 142, QUEST_CREGUILD ) )
				pUser->AddDefinedText( TID_EVE_ENDQUEST, "\"%s\"", pQuestProp->m_szTitle );
		}
		g_dpDBClient.CalluspLoggingQuest( pUser->m_idPlayer, nQuest, 20 );
	}
	else
	if( nState == QS_BEGIN )
	{
		g_dpDBClient.CalluspLoggingQuest( pUser->m_idPlayer, nQuest, 10 );
	}
	QUEST quest;
	if( pUser->SetQuest( nQuest, nState, &quest ) )
		pUser->AddSetQuest( &quest );
	return 1;
}

void __SetQuest( DWORD dwIdMover, int nQuest )
{
	QuestProp* pQuestProp	= prj.m_aPropQuest.GetAt( nQuest );
	FLWSUser* pUser	= prj.GetUser( dwIdMover );
	QUEST quest;
	if( pUser->SetQuest( nQuest, 0, &quest ) )
	{
		if( pQuestProp )
		{
			pUser->AddDefinedText( TID_EVE_STARTQUEST, "\"%s\"", pQuestProp->m_szTitle );
		}
		
		pUser->AddSetQuest( &quest );

		g_dpDBClient.CalluspLoggingQuest( pUser->m_idPlayer, nQuest, 10 );
	}
	else
	{
		pUser->AddDefinedText( TID_MMI_QUEST_ESCESS );
	}
}

void __CreateItem( DWORD dwIdMover, int v1, int v2, int v3, BYTE v4 )
{
	FLWSUser* pUser	= prj.GetUser( dwIdMover );
	PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( v1 );
	if( pItemProp )
		pUser->AddDefinedText( TID_EVE_REAPITEM, "\"%s\"", pItemProp->szName );
	FLItemElem itemElem;
	itemElem.m_dwItemId		= v1;
	itemElem.m_nItemNum		= v2;
	itemElem.SetAbilityOption( v3 );

	itemElem.SetFlag( v4 );

	if( pUser->CreateItem( &itemElem ) )
	{
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
void __AddGold( DWORD dwIdMover, int nGold )     
{
	FLWSUser* pUser = prj.GetUser( dwIdMover );
	pUser->AddGold( nGold );
	pUser->AddGoldText( nGold );
}

void __RemoveGold( DWORD dwIdMover, int nGold )
{
	FLWSUser* pUser	= prj.GetUser( dwIdMover );
	if( pUser->AddGold( -nGold ) == FALSE )
		pUser->SetGold( 0 );
}

void __QuestSort( vector<int> & vecQuestId )
{
	QuestProp* pPrevQuestProp = NULL;
	QuestProp* pNextQuestProp = NULL;
	int nTmpQuestId;
	for( DWORD i = 0; i < vecQuestId.size(); ++i )
	{
		for( DWORD j = i + 1; j < vecQuestId.size(); ++j )
		{
			pPrevQuestProp = prj.m_aPropQuest.GetAt( vecQuestId[ i ] );
			pNextQuestProp = prj.m_aPropQuest.GetAt( vecQuestId[ j ] );
			if( pPrevQuestProp->m_nBeginCondLevelMin < pNextQuestProp->m_nBeginCondLevelMin )
			{
				nTmpQuestId = vecQuestId[ i ];
				vecQuestId[ i ] = vecQuestId[ j ];
				vecQuestId[ j ] = nTmpQuestId;
			}
			else if( pPrevQuestProp->m_nBeginCondLevelMin == pNextQuestProp->m_nBeginCondLevelMin )
			{
				if( pPrevQuestProp->m_nBeginCondLevelMax < pNextQuestProp->m_nBeginCondLevelMax )
				{
					nTmpQuestId = vecQuestId[ i ];
					vecQuestId[ i ] = vecQuestId[ j ];
					vecQuestId[ j ] = nTmpQuestId;
				}
				else if( pPrevQuestProp->m_nBeginCondLevelMax == pNextQuestProp->m_nBeginCondLevelMax && vecQuestId[ i ] < vecQuestId[ j ] )
				{
					nTmpQuestId = vecQuestId[ i ];
					vecQuestId[ i ] = vecQuestId[ j ];
					vecQuestId[ j ] = nTmpQuestId;
				}
			}
		}
	}
}

//#include "../WorldServer/FLCooperativeContributions.h"

void __processCompleteQuest( const int nPCID, const int nQuestID )
{
	__SayQuest( nPCID, nQuestID, QSAY_END_COMPLETE1 );
	__SayQuest( nPCID, nQuestID, QSAY_END_COMPLETE2 );
	__SayQuest( nPCID, nQuestID, QSAY_END_COMPLETE3 );
	__AddAnswer( nPCID,"__OK__", "QUEST_END_COMPLETE", 0, nQuestID );

	//COOPERATIVE_CONTRIBUTIONS().OnCompleteQuest( nPCID, nQuestID );
}
