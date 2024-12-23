#include "stdafx.h"
#include "..\_AIInterface\ActionMover.h"
#include "Mover.h"
#include "ModelObject.h"
#include "lang.h"
#include "ModelObject.h"  
#include "authorization.h"
#include "CreateObj.h"
#include "eveschool.h"

extern	CGuildCombat	g_GuildCombatMng;
#include "..\_aiinterface\aipet.h"

#include "pet.h"	//sun: 9, 9-10�� ��

//sun: 12, ����
#ifdef __WORLDSERVER
#include "../WorldServer/slord.h"
#include "../worldserver/FLMapGrid.h"
#include "../worldserver/LinkMap.h"
#include "../worldserver/GlobalGameValueCalculator.h"
#endif	// __WORLDSERVER


#include "playerdata.h"		//sun: 11, ĳ���� ���� ����
#include "honor.h"			//sun: 13, ����
#include "Quiz.h"

#ifdef __CLIENT
	#include "..\_Common\ParticleMng.h"
	#include "Resdata.h"
	#include "../Neuz/DPClient.h"
	#include "DialogMsg.h"
	extern	CDPClient	g_DPlay;
	#include "GuildRender.h"
	#include "defineText.h"
	#include "defineSound.h"
#endif	// __CLIENT

#ifdef	__WORLDSERVER
	#include "../WorldServer/User.h"
	#include "../WorldServer/UserMacro.h"
	#include "WorldMng.h"
	#include "spevent.h"
extern	CWorldMng	g_WorldMng;
#endif


#include "..\_AIInterface\AIInterface.h"
#include "../_aiinterface/FLFSM.h"

#include "party.h"
extern	CPartyMng	g_PartyMng;

#include "guild.h"
extern	CGuildMng	g_GuildMng;
#include "guildwar.h"
extern	CGuildWarMng	g_GuildWarMng;

#include "Ship.h"

#ifdef __WORLDSERVER
#include "../WorldServer/User.h"
#include "../WORLDSERVER/FLEventDropItem.h"
#include "../WORLDSERVER/FLLevelDropItem.h"
#include "../WORLDSERVER/FLMonsterDropItem.h"
#include "../WORLDSERVER/FLEventArenaGlobal.h"
#include "../worldserver/FLMadrigalGift.h"
#include "../WORLDSERVER/couplehelper.h"
#endif __WORLDSERVER

#ifdef __EVENT_MONSTER
#include "EventMonster.h"
#endif // __EVENT_MONSTER

#include "SecretRoom.h"		//sun: 12, ����� ��
#include "CreateMonster.h"	//sun: 12, // __NEW_ITEMCREATEMON_SERVER

extern	BOOL CanAdd( DWORD dwGold, int nPlus );

#ifdef __WORLDSERVER
MEMPOOLER_IMPLEMENT( CMover, 1024 );
#else	// __WORLDSERVER
MEMPOOLER_IMPLEMENT( CMover, 128 );
#endif	// __WORLDSERVER


#if defined __WORLDSERVER && defined __ENCHANT_BARUNA16		
#include "NPCSummoner.h"
#endif //__ENCHANT_BARUNA16


#include "../_Common/FLCommonFunction.h"


const int	MAX_DIALOGFILENAME = 32;

#ifdef __CLIENT
// ����Ʈ ������ Ȯ�� ���μ��� 
BOOL CMover::m_bQuestEmoticonAdd = TRUE;
FLOAT CMover::m_fQuestEmoticonScale = 1.0f;

//sun: �����ڰŸ���... Neuz
LPDIRECT3DTEXTURE9 CMover::m_pTextureEye[2][MAX_HEAD] = { 0, };
LPDIRECT3DTEXTURE9 CMover::m_pTextureEyeFlash[2][MAX_HEAD] = { 0, };

#endif

#include "collecting.h"	//sun: 11, ä�� �ý���

// ��Ʈ ������ ����Ʈ ȿ�� ���� �ܰ�
#pragma	warning( push )
#pragma warning( disable : 4480 )

namespace CMover_SetItemSFX
{
	static enum eSetItemSFX_Level : BYTE
	{
		eLevel_3 = 3,	
		eLevel_4,
		eLevel_5,
		eLevel_6,
		eLevel_7,
		eLevel_8,
		eLevel_9,
		eLevel_10,
	};
};

#pragma	warning( pop )

using namespace CMover_SetItemSFX;

// ��Ʈ�� ���� ���� �ȵǰ�..

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#pragma	warning( push )
#pragma warning( disable : 4355 )

CMover::CMover()
:	m_dwRegisterElldinsJarID( NULL )
#ifdef __WORLDSERVER
, m_nNearPCCount( 0 )
#endif
, m_autoSynchronizer( this )
{
	m_buffs.SetMover( this );
	Init();
}

#pragma	warning( pop )

CMover::~CMover()  
{
#if defined __WORLDSERVER && defined __ENCHANT_BARUNA16		
	if( IsPlayer() == TRUE )																							//���� ���� m_bPlayer�� m_CalledNPCOBJID�� ����
		GetBarunaNPCSummoner().InactivateNPC( * static_cast< FLWSUser * >( this ) );
#endif //__ENCHANT_BARUNA16

#ifdef __CLIENT
	SAFE_DELETE( m_pLadolf );		// g_Object3DMng�� �ı��Ǳ����� �θ���
	SAFE_DELETE( m_pAngel );		//sun: 8�� ���� ��ȯ Neuz, World, Trans
	SAFE_DELETE( m_pBalloon );		//sun:10, __EVE_BALLOON
// ������ ���信 �����ҷ��� �õ��� �ڵ�		2016.07.11
//	SAFE_DELETE( m_pCloakWing );
#endif //__CLIENT

#ifdef __WORLDSERVER
	SAFE_DELETE( m_pNpcProperty );
#endif

#ifdef __WORLDSERVER
	CCreateMonster::GetInstance()->RemoveInfo( this );	//sun: 12, // __NEW_ITEMCREATEMON_SERVER
#endif // __WORLDSERVER

	CMover* pOther = m_vtInfo.GetOther();
	if( IsValidObj( pOther ) )
	{
		if( pOther->m_vtInfo.GetOther() == this )
			pOther->m_vtInfo.TradeClear();
	}

#ifdef __CLIENT
	m_vtInfo.VendorClose();
	g_DialogMsg.ClearMessage( (CObj*)this );
#endif	// __CLIENT

	SAFE_DELETE( m_pActMover );

	for( int i = 0; i < MAX_VENDOR_INVENTORY_TAB; i++ )
		SAFE_DELETE( m_ShopInventory[ i ] );

	if( GetWorld() )
	{
#if !defined(__CLIENT)
#ifdef __LAYER_1021
		GetWorld()->m_respawner.Increment( GetRespawn(), GetRespawnType(), m_bActiveAttack, GetLayer() );
#else	// __LAYER_1021
		GetWorld()->m_respawner.Increment( GetRespawn(), GetRespawnType(), m_bActiveAttack );
#endif	// __LAYER_1021
#endif
	}

	SAFE_DELETE_ARRAY( m_aQuest );
	SAFE_DELETE_ARRAY( m_aCompleteQuest );
	SAFE_DELETE_ARRAY( m_aCheckedQuest );

#ifdef __CLIENT
	m_pSfxWing = NULL;
	m_pSfxBuffPet = NULL;
#ifdef __BS_EFFECT_LUA
	CSfxModelMng::GetThis()->SubData( GetId() );
#endif //__BS_EFFECT_LUA

#endif	// __CLIENT
}

void CMover::Init()
{
//	m_dwUpdateDestTick	= 0;

	::memset( (PT_MOVE_CHECK_DATA)&m_tMoveCheckData, 0, sizeof(m_tMoveCheckData) );
	//	m_pNewAIInterface	= NULL;
#ifdef __PROFESSION
	m_nProfessionLevel 	= 1;
	m_nProfessionExp 	= 0;
#endif // __PROFESSION

	m_dwMotion		    = static_cast< DWORD >( -1 );

	m_dwMotionOption    = 0;
	m_dwType			= OT_MOVER;
	m_vMarkingPos	    = D3DXVECTOR3( 0, 0, 0 );
	m_nCount			= xRandom( 128 );		// ������ �Ǵ� ������ ����ī���͸� �ణ�� Ʋ���� �ؼ� ���� �پ��� ������ ������ �ߴ�.
	m_bPlayer			= FALSE;
	m_dwTypeFlag		= 0;
	m_pRide				= NULL;  
	m_pAIInterface		= NULL;
	m_nHitPoint			= 100;
	m_nManaPoint		= 100;
	m_nFatiguePoint		= 100;
	m_vDestPos			= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_idDest			= NULL_ID;
	m_fArrivalRange		= 0.0f;
	SetSex( SEX_FEMALE );
	m_dwVirtItem		= ITEM_INDEX( 11, II_WEA_HAN_HAND );
	m_dwVirtType		= VT_ITEM;
	m_nHitPoint			= 100;
	m_nManaPoint		= 100;
	m_nFatiguePoint		= 100;
	m_dwRideItemIdx		= ITEM_INDEX( 5000, II_RID_RID_STI_MAGIC );
	m_nDefenseMin		= 0;
	m_nDefenseMax		= 0;
	m_nJob				= -1;
	memset( m_szName, 0, sizeof(m_szName) );
	m_nAdjHitRate		= 0;
	m_nAdjParry         = 0;
	m_Stat.InitializeOriginStat();
	m_nLevel			= 1;
	m_nExp1				= 0;
	m_nDeathExp			= 0;
	m_nDeathLevel		= 0;
	m_dwFace			= 0;
	m_dwSkinSet			= 0;
	m_dwHairMesh		= 0;
	m_dwHairColor		= 0;
	m_dwHeadMesh		= 0;

	m_nTutorialState	= 0;	//sun: 12, Ʃ�丮�� ����

	m_nFxp				= 0;
	m_szCharacterKey[ 0 ] = 0;
	{
		int i;
		for( i = 0; i < MAX_ADJPARAMARY; i ++ )		m_adjParamAry[i] = 0;
		for( i = 0; i < MAX_ADJPARAMARY; i ++ )		m_chgParamAry[i] = 0x7FFFFFFF;	// ������ �ִ�.
	}

	ClearEquipInfo();

	m_vtInfo.Init( this );

	//sun: 11, �ָӴ�
#ifdef __WORLDSERVER
	m_Pocket.Init( this );
#endif	// __WORLDSERVER

	m_idPlayer			= NULL_ID;
	m_dwAuthorization	= AUTH_GENERAL;
	m_idMarkingWorld	= NULL_ID;
	m_dwRegionAttr		= 0;
	m_dwOldRegionAttr	= 0;
	m_bPositiveX		= m_bPositiveZ	= false;
	m_bForward			= true;
	m_dwAuthorization	= AUTH_GENERAL;
	m_dwMode			= 0;
	m_fWaitQueryGetPos	= FALSE;
	m_fHairColorR		= 1.0f;
	m_fHairColorG		= 1.0f;
	m_fHairColorB		= 1.0f;
	m_dwHairColor		= D3DCOLOR_COLORVALUE( 1.0f, 1.0f, 1.0f, 1.0f );
	m_nQuestSize		= 0;
	m_aQuest	        = NULL;
	m_aCompleteQuest    = NULL; 
	m_nCompleteQuestSize = 0; 
	m_aCheckedQuest		= NULL;
	m_nCheckedQuestSize = 0;
#ifdef __WORLDSERVER
	m_timerQuestLimitTime.Set( MIN( 1 ) );
	m_dwPKTargetLimit = 0;
#else // __WORLDSERVER
	m_nQuestEmoticonIndex = -1; 
	m_bShowQuestEmoticon = TRUE;
#endif // __WORLDSERVER
	m_idparty			= 0;

	m_oaCmd				= m_oaAct = OBJACT_NONE;
	memset( m_nCParam, 0, sizeof(m_nCParam) );
	memset( m_nAParam, 0, sizeof(m_nAParam) );
	m_dwFlag			= 0;
	m_nAtkCnt			= 0;
	m_wStunCnt			= 0;

	m_idAttacker		= NULL_ID;
	m_idTargeter		= NULL_ID;
	m_idTracking		= NULL_ID;
	m_fExpRatio			= 1.0f;
	m_idMurderer		= NULL_ID;
	m_tmActionPoint		= timeGetTime();
	m_nDead				= 0;
	//sun: 8, // __S8_PK
	m_dwPKTime			= 0;
	m_nPKValue			= 0;
	m_dwPKPropensity	= 0;
	m_dwPKExp			= 0;
	m_wDarkCover        = 0;

	m_idLastHitMover	= NULL_ID;			// this�� ���������� �ƴ� �������̵�
	m_nFame				= 0;				// ��
	m_nDuel				= 0;				// ������ΰ�.
	m_idDuelOther		= NULL_ID;
	m_idDuelParty		= 0;
	m_nFuel				= -1;				// �ʱⰪ�� -1�̴�.
	m_tmAccFuel			= 0;				// 0�̸� ���� ���ϴ� ����.
	m_fAniSpeed			= 1.0f;
	m_idPoisonAttacker	= NULL_ID;
	m_wPoisonCnt		= 0;
	m_tmPoisonUnit		= 0;
	m_wPoisonDamage		= 0;
	m_wDarkCnt			= 0;
	m_wDarkVal			= 0;
	m_idBleedingAttacker	= NULL_ID;
	m_wBleedingCnt		= 0;
	m_tmBleedingUnit	= 0;
	m_wBleedingDamage	= 0;
	m_nMovePattern		= 0;
	m_nMoveEvent		= 0;
	m_nMoveEventCnt		= 0;


	m_idGuild			= 0;
	m_idGuildCloak		= 0;
	m_bGuildBank		= FALSE;
	m_idWar				= 0;

	m_nPlusMaxHitPoint = 0;
	memset( m_dwSMTime, 0, sizeof( m_dwSMTime ) );
	m_nAttackResistLeft = (BYTE)0xff;
	m_nAttackResistRight = (BYTE)0xff;
	m_nDefenseResist = (BYTE)0xff;

#ifdef __WORLDSERVER
	m_decreaseExperience	= true;		// ������, ���, �Ʒ��� �� Ư�� ��Ȳ �̿ܿ��� ��� �� ����ġ �϶� üũ.
	m_tGuildMember	= CTime::GetCurrentTime();

	m_nQuestKeeping	= 0;			// �������Ʈ Ŭ����ũ ����Ʈ�� ���� 
	m_nPartyQuestKeeping	= 0;

	//	m_idCollecter	= NULL_ID;
	//	m_nResource		= 0;
	//	m_nCollectOwnCnt = 0;
#endif // __WORLDSERVER

#ifdef __CLIENT
	m_nDmgCnt			= 0;
	m_nWaterCircleCount = 0;
	m_idSfxHit		= 0;
	m_dwReqFlag		= 0;
	m_pCloakTexture = NULL;
	m_fDestScale	= 1.0f;
	m_fDestScaleSlerp = 0;
	m_szPartyName[0]	= '\0';
#else // CLIENT
	m_nReflexDmg = 0;
#endif	// not Client

	m_pActMover = new CActionMover( this );
	m_pActMover->SetMover( this );

	m_dwTickRecovery = ::timeGetTime() + NEXT_TICK_RECOVERY;
	m_dwTickRecoveryStand = ::timeGetTime() + NEXT_TICK_RECOVERYSTAND;
	m_dwTickDuel = ::timeGetTime() + NEXT_TICK_DUEL;
	m_dwTickEndDuel = ::timeGetTime() + NEXT_TICK_ENDDUEL;
	m_nDuelState = 0;

	memset( &m_Resurrection_Data, 0, sizeof(m_Resurrection_Data) );

	m_bRegenItem		= TRUE;
	m_bActiveAttack		= FALSE;
	m_dwGold			= 0;		// 0���� �Ұ�.  -xuzhu-

	for( int i = 0; i < MAX_VENDOR_INVENTORY_TAB; i++ )
		m_ShopInventory[ i ] = 0;

	//sun: 8,     // 8�� ������� ������� PVP�����ϰ���   Neuz, World
	m_bPVPEnd	=	FALSE;

	m_nLegend	=	LEGEND_CLASS_NORMAL;	//sun: 10�� ���½ý���	Neuz, World, Trans

	memset( dwRemoveSfxObj, 0, sizeof( dwRemoveSfxObj ) );		//sun, 11, Ȯ����ų ȿ������ world,neuz

#ifdef	__CLIENT
	m_dwLadolfFlag = 0;
	m_pLadolf = NULL;

	//sun: 8�� ���� ��ȯ Neuz, World, Trans
	m_pAngel = NULL;			
	m_pAngelFlag = FALSE;
	m_dwAngelKind = 0;

	//sun:10, __EVE_BALLOON
	m_pBalloonFlag = FALSE;
	m_pBalloon = NULL;
	m_dwBalloonKind = 0;
	m_fBalloonYPos = 1.0f;
	m_bBalloon = TRUE;


	m_eWingStateFlag = FLOATING;
	m_fOldLengthSq = 0.0f;

	// ������ ���信 �����ҷ��� �õ��� �ڵ�		2016.07.11
	//m_pCloakWing = NULL;

#endif //__CLIENT
	m_dwStateMode = 0;
	m_nReadyTime = 0;
	m_dwUseItemId = 0;
	m_bItemFind = TRUE;
	m_dwTickCheer= 0;
	m_nCheerPoint	= 0;
	m_pIADestPos = NULL;
	SetEatPetId( NULL_ID );

	ZeroMemory( m_tmReUseDelay, sizeof( m_tmReUseDelay ) );

#ifdef __WORLDSERVER
	m_pNpcProperty = NULL;
	m_dwTickCreated	= 0;
#endif

	m_nGuildCombatState = 0;
	m_dwCtrlReadyTime   = 0xffffffff;
	m_dwCtrlReadyId     = NULL_ID;

	m_nSkillLevel = 0;
	m_nSkillPoint = 0;

	m_dwMotionArrive = OBJMSG_STAND;

	m_SkillTimerStop = FALSE;

	//sun: 8�� ���� ��ȯ Neuz, World, Trans
	m_nAngelExp = 0;				/// ���� ����ġ
	m_nAngelLevel = 0;				/// ���� Level

	//sun: �����ڰŸ���... Neuz
#ifdef __CLIENT
	m_tmEye[0].Set( SEC(6) );
	m_tmEye[1].Set( 200 );

	m_bExclusiveLowBody = false;
#endif //__CLIENT

#ifdef __CLIENT
	m_SkillTimer.Set(1500); //Function Key������ ���� ��� Skill Delay��.
#endif //__CLIENT

	//sun: 9, 9-10�� ��
	m_dwPetId	= NULL_ID;
	m_nHealCnt	= 0;

	m_fSpeedFactor	= 1.0F;	//sun: 9,	//__AI_0509

#ifdef __CLIENT
	m_pSfxWing = NULL;
	m_dwWingTime = -1;

	m_pSfxBuffPet = NULL;
#endif //__CLIENT

	m_dwMute	= 0;

	//sun: 13, ����
	m_nHonor = -1;					// ���μ��� 
	m_dwHonorCheckTime = 0;			//���� �ð�üũ
	memset( m_aHonorTitle, 0, sizeof( m_aHonorTitle ) );// ���μ�ġ
	m_strTitle.Empty();

	m_nAccountPlayTime = -1;
	m_objIdVisPet = NULL_ID;
	m_dwMoverSfxId = NULL_ID;
	m_idCampus = 0;
	m_nCampusPoint = 0;

#if defined __WORLDSERVER && defined __ENCHANT_BARUNA16		
	m_szCharacterKey[ 0 ]		= '\0';

	m_NPCOwnerOBJID				= NULL_ID;
#endif //__ENCHANT_BARUNA16

	m_dwSleepingDuration = 0;

#if defined __WORLDSERVER
	m_pFSM					= NULL;
	m_IsReturnToBegin		= FALSE;

	m_bDelete_IntoIdleMode	= FALSE;		// ������ ������ �����Ǵ� ����
#endif	// __WORLDSERVER

#if defined __WORLDSERVER && defined __AGGRO16
	m_AggroOwnerList.clear();
#endif //__WORLDSERVER && defined __AGGRO16

#ifdef __CLIENT
	m_nEATeam = EAPlayers::EA_NOTEAM;
#endif

#ifdef	__WORLDSERVER
	m_dwDisguiseID = 0;
#endif
#ifdef PAT_LOOTOPTION
	m_nPetLootType = 0;
#endif // PAT_LOOTOPTION
}

// AI �Ž��� ���� : ��ü�� �ൿ ����, ���� ���¸� �Ž����� ������ 
void CMover::SendAIMsg( DWORD dwMsg, DWORD dwParam1, DWORD dwParam2 )
{
	if( m_pAIInterface )
		m_pAIInterface->SendAIMsg( dwMsg, dwParam1, dwParam2 );

#ifdef __WORLDSERVER
	if( m_pFSM )
		m_pFSM->PostAIMsg( dwMsg, dwParam1, dwParam2 );
#endif //__WORLDSERVER
}
// AI �Ž��� ���� : ��ü�� �ൿ ����, ���� ���¸� �Ž����� ������ 
void CMover::PostAIMsg( DWORD dwMsg, DWORD dwParam1, DWORD dwParam2 )
{
	if( m_pAIInterface )
		m_pAIInterface->PostAIMsg( dwMsg, dwParam1, dwParam2 );

#ifdef __WORLDSERVER
	if( m_pFSM )
		m_pFSM->PostAIMsg( dwMsg, dwParam1, dwParam2 );
#endif //#ifdef __WORLDSERVER
}

//raiders.2006.11.28	 trade���� ��꿡 �����ϴ� ���� ����
BOOL CMover::AddGold( int nGold, BOOL bSend )
{
	__int64 n64Gold = (__int64)nGold;

	if( n64Gold == 0 )
	{
		return TRUE;
	}

	//#ifdef __WORLDSERVER
	//	if( m_vtInfo.TradeGetGold() != 0 )
	//		return FALSE;
	//#endif	// __WORLDSERVER

	__int64 n64Total = static_cast<__int64>( GetGold() ) + n64Gold;

	if( n64Total > static_cast<__int64>( INT_MAX ) )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "Overflow. UserID:[%07d], Gold:[%d]" ), m_idPlayer, n64Gold );
		n64Total = static_cast<__int64>( INT_MAX );

		return FALSE;
	}
	else if( n64Total < 0 )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "LackMoney. UserID:[%07d], Gold:[%d]" ), m_idPlayer, n64Gold );
		n64Total = 0;
		// ah.............
	}

	SetGold( static_cast<int>( n64Total ) );


#ifdef __WORLDSERVER
	if( bSend == TRUE )
	{
		g_xWSUserManager->AddSetPointParam( this, DST_GOLD, static_cast<int>( n64Total ) );
	}
#endif	// __WORLDSERVER

	return TRUE;
}

// �̵����� ����.
void CMover::SetMovePattern( int nPattern )		
{
	m_nMovePattern = nPattern;
	m_nMoveEvent = 0;
	m_nMoveEventCnt = 0;
	ClearDest();		// ��ǥ�� ����.
}

// ���ο� �Ӹ������� ����Ǳ� ���� ���Լ��� ����ؾ���...
// ���� �Ӹ������� �����ϱ� ����...
int	CMover::GetHairCost( CMover* pMover, BYTE nR, BYTE nG, BYTE nB, BYTE nHair )
{
	BYTE nOrignalR = (BYTE)( pMover->m_fHairColorR * 255 );
	BYTE nOrignalG = (BYTE)( pMover->m_fHairColorG * 255 );
	BYTE nOrignalB = (BYTE)( pMover->m_fHairColorB * 255 );

	int nHairCost = 0;
	// sun: 8�� ������ ���� �� �������� ��� �߰�.
	int nHairColorCost = 0;

	if( nR != nOrignalR || nG != nOrignalG || nB != nOrignalB )
		nHairColorCost = HAIRCOLOR_COST;
	else
		nHairColorCost = 0;

	if( pMover->m_dwHairMesh+1 != static_cast< DWORD >( nHair+1 ) )
		nHairCost = HAIR_COST;
	else
		nHairCost = 0;

	return (nHairCost + nHairColorCost);

	// sun: 8�� ������ ���� �� �������� ��� �߰�. ( < 8 )
	//int nHairColorCostR = 0;
	//int nHairColorCostG = 0;
	//int nHairColorCostB = 0;
	//
	//if( nR >= nOrignalR  )
	//	nHairColorCostR = (nR - nOrignalR)*13;
	//else
	//	nHairColorCostR = (nOrignalR - nR)*7;
	//
	//if( nG >= nOrignalG  )
	//	nHairColorCostG = (nG - nOrignalG)*13;
	//else
	//	nHairColorCostG = (nOrignalG - nG)*7;
	//
	//if( nB >= nOrignalB  )
	//	nHairColorCostB = (nB - nOrignalB)*13;
	//else
	//	nHairColorCostB = (nOrignalB - nB)*7;
	//
	//if( pMover->m_dwHairMesh+1 != nHair+1 )
	//{
	//	switch( nHair+1 )
	//	{
	//		case 1: nHairCost = 2500;	break;
	//		case 2:	nHairCost = 2500;	break;
	//		case 3:	nHairCost = 2500;	break;
	//		case 4:	nHairCost = 2500;	break;
	//		case 5:	nHairCost = 2500;	break;
	//		default: nHairCost = 4000;	break;
	//	}
	//}
	//else
	//{
	//	nHairCost = 0;
	//}

	//return ( nHairColorCostR + nHairColorCostG + nHairColorCostB + nHairCost );
}

#ifdef __WORLDSERVER
void CMover::SubSMMode()
{
	time_t tmCur = (time_t)( CTime::GetCurrentTime().GetTime() );
	for( int i = 0 ; i < SM_MAX ; ++i )
	{
		if( m_dwSMTime[i] > 0 )
		{
			if( g_AddSMMode.bSMModetime[i] )	// ����ð�
			{
				if( m_dwSMTime[i] < (DWORD)( tmCur ) )
				{
					m_dwSMTime[i] = 0;
				}
			}
			else								// ī��Ʈ
			{
				if( i != SM_REVIVAL && i != SM_REVIVAL2)
					--m_dwSMTime[i];	
			}

			if( m_dwSMTime[i] == 0 )
			{
				FLItemElem* pItemElemParts = NULL;
				if( i == SM_RESIST_ATTACK_LEFT || i == SM_RESIST_ATTACK_RIGHT )
				{
					int nAttackResist;
					if( i == SM_RESIST_ATTACK_LEFT )
						nAttackResist = m_nAttackResistLeft;
					else
						nAttackResist = m_nAttackResistRight;

					switch( nAttackResist )
					{
					case SAI79::FIRE:
						g_AddSMMode.dwSMItemID[i] = ITEM_INDEX( 10277, II_CHR_SYS_SCR_FIREASTONE );
						break;
					case SAI79::WATER:
						g_AddSMMode.dwSMItemID[i] = ITEM_INDEX( 10278, II_CHR_SYS_SCR_WATEILSTONE );
						break;
					case SAI79::WIND:
						g_AddSMMode.dwSMItemID[i] = ITEM_INDEX( 10279, II_CHR_SYS_SCR_WINDYOSTONE );
						break;
					case SAI79::ELECTRICITY:
						g_AddSMMode.dwSMItemID[i] = ITEM_INDEX( 10280, II_CHR_SYS_SCR_LIGHTINESTONE );
						break;
					case SAI79::EARTH:
						g_AddSMMode.dwSMItemID[i] = ITEM_INDEX( 10281, II_CHR_SYS_SCR_EARTHYSTONE );
						break;
					}
					if( i == SM_RESIST_ATTACK_LEFT )
					{
						m_nAttackResistLeft = (BYTE)0xff;
						pItemElemParts = m_Inventory.GetEquip( PARTS_LWEAPON );
					}
					else
					{
						m_nAttackResistRight = (BYTE)0xff;
						pItemElemParts = m_Inventory.GetEquip( PARTS_RWEAPON );
					}
				}
				else if( i == SM_RESIST_DEFENSE )
				{
					switch( m_nDefenseResist )
					{
					case SAI79::FIRE:
						g_AddSMMode.dwSMItemID[i] = ITEM_INDEX( 10282, II_CHR_SYS_SCR_DEFIREASTONE );
						break;
					case SAI79::WATER:
						g_AddSMMode.dwSMItemID[i] = ITEM_INDEX( 10283, II_CHR_SYS_SCR_DEWATEILSTONE );
						break;
					case SAI79::WIND:
						g_AddSMMode.dwSMItemID[i] = ITEM_INDEX( 10284, II_CHR_SYS_SCR_DEWINDYOSTONE );
						break;
					case SAI79::ELECTRICITY:
						g_AddSMMode.dwSMItemID[i] = ITEM_INDEX( 10285, II_CHR_SYS_SCR_DELIGHTINESTONE );
						break;
					case SAI79::EARTH:
						g_AddSMMode.dwSMItemID[i] = ITEM_INDEX( 10286, II_CHR_SYS_SCR_DEEARTHYSTONE );
						break;
					}
					m_nDefenseResist = (BYTE)0xff;
					pItemElemParts = m_Inventory.GetEquip( PARTS_UPPER_BODY );
				}

				if( pItemElemParts && ( i == SM_RESIST_DEFENSE || i == SM_RESIST_ATTACK_LEFT || i == SM_RESIST_ATTACK_RIGHT ) )
				{
					pItemElemParts->m_nResistSMItemId = 0;
					((FLWSUser*)this)->AddCommercialElem( pItemElemParts->m_dwObjId, 0 );
				}

				PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( g_AddSMMode.dwSMItemID[i] );
				if( pItemProp )
				{
					if( i == SM_MAX_HP )
					{
						ResetDestParam( pItemProp->dwDestParam[0], pItemProp->nAdjParamVal[1], 1 );
					}
					else if( i == SM_MAX_HP50 )
					{
						ResetDestParam( pItemProp->dwDestParam[0], m_nPlusMaxHitPoint, 1 );
					}
					else if( i == SM_VELOCIJUMP || i == SM_JUMP)
					{
						for( size_t Nth = 0; Nth < MAX_ITEM_PARAM; ++Nth )
						{
							if( pItemProp->dwDestParam[ Nth ] != NULL_ID )
							{
								ResetDestParam( pItemProp->dwDestParam[ Nth ], pItemProp->nAdjParamVal[ Nth ] );
							}
						}
// 						if( pItemProp->dwDestParam1 != -1 )
// 							ResetDestParam( pItemProp->dwDestParam1, pItemProp->nAdjParamVal1 );
// 						if( pItemProp->dwDestParam2 != -1 )
// 							ResetDestParam( pItemProp->dwDestParam2, pItemProp->nAdjParamVal2 );
// 						if( pItemProp->dwDestParam3 != -1 )
// 							ResetDestParam( pItemProp->dwDestParam3, pItemProp->nAdjParamVal3 );
					}
					//sun: 12, // 12�� �ش����������
					// 090917 mirchang - �Ľ�ųǮ ������ ��� �Ⱓ ����
					else if( i == SM_PARTYSKILL30 || i == SM_PARTYSKILL15 || i == SM_PARTYSKILL1 )
					{
//						g_DPCoreClient.SendUserPartySkill( m_idPlayer, PARTY_PARSKILL_MODE, 0, 0, 1 );

						g_PartyMng.NotiRefreshPartyEffectModeToCS( m_idPlayer, m_idparty, PARTY_EFFECT_PARSKILLFULL );
					}
				}
				else
				{
					FLERROR_LOG( PROGRAM_NAME, _T( "No SMItem dwSMItemID[%d] : %d / Name : %s" ), i, g_AddSMMode.dwSMItemID[i], m_szName );
				}

				((FLWSUser*)this)->AddSMMode( i, 0 );
			}
			else
			{
				if( i == SM_MAINTAIN_MP && m_nManaPoint != GetMaxManaPoint() )
				{
					SetPointParam( DST_MP, GetMaxManaPoint() );
				}
				else if( i == SM_MAINTAIN_FP && m_nFatiguePoint != GetMaxFatiguePoint() )
				{
					SetPointParam( DST_FP, GetMaxFatiguePoint() );
				}
			}
		}
	}
}

void CMover::ClearAllSMMode()
{
//	time_t tmCur = (time_t)( CTime::GetCurrentTime().GetTime() );
	for( int i = 0 ; i < SM_MAX ; ++i )
	{
		if( m_dwSMTime[i] > 0 )
		{
			m_dwSMTime[i] = 0;
			FLItemElem* pItemElemParts = NULL;
			if( i == SM_RESIST_ATTACK_LEFT || i == SM_RESIST_ATTACK_RIGHT )
			{
				int nAttackResist;
				if( i == SM_RESIST_ATTACK_LEFT )
					nAttackResist = m_nAttackResistLeft;
				else
					nAttackResist = m_nAttackResistRight;

				switch( nAttackResist )
				{
				case SAI79::FIRE:
					g_AddSMMode.dwSMItemID[i] = ITEM_INDEX( 10277, II_CHR_SYS_SCR_FIREASTONE );
					break;
				case SAI79::WATER:
					g_AddSMMode.dwSMItemID[i] = ITEM_INDEX( 10278, II_CHR_SYS_SCR_WATEILSTONE );
					break;
				case SAI79::WIND:
					g_AddSMMode.dwSMItemID[i] = ITEM_INDEX( 10279, II_CHR_SYS_SCR_WINDYOSTONE );
					break;
				case SAI79::ELECTRICITY:
					g_AddSMMode.dwSMItemID[i] = ITEM_INDEX( 10280, II_CHR_SYS_SCR_LIGHTINESTONE );
					break;
				case SAI79::EARTH:
					g_AddSMMode.dwSMItemID[i] = ITEM_INDEX( 10281, II_CHR_SYS_SCR_EARTHYSTONE );
					break;
				}
				if( i == SM_RESIST_ATTACK_LEFT )
				{
					m_nAttackResistLeft = (BYTE)0xff;
					pItemElemParts = m_Inventory.GetEquip( PARTS_LWEAPON );
				}
				else
				{
					m_nAttackResistRight = (BYTE)0xff;
					pItemElemParts = m_Inventory.GetEquip( PARTS_RWEAPON );
				}

			}
			else if( i == SM_RESIST_DEFENSE )
			{
				switch( m_nDefenseResist )
				{
				case SAI79::FIRE:
					g_AddSMMode.dwSMItemID[i] = ITEM_INDEX( 10282, II_CHR_SYS_SCR_DEFIREASTONE );
					break;
				case SAI79::WATER:
					g_AddSMMode.dwSMItemID[i] = ITEM_INDEX( 10283, II_CHR_SYS_SCR_DEWATEILSTONE );
					break;
				case SAI79::WIND:
					g_AddSMMode.dwSMItemID[i] = ITEM_INDEX( 10284, II_CHR_SYS_SCR_DEWINDYOSTONE );
					break;
				case SAI79::ELECTRICITY:
					g_AddSMMode.dwSMItemID[i] = ITEM_INDEX( 10285, II_CHR_SYS_SCR_DELIGHTINESTONE );
					break;
				case SAI79::EARTH:
					g_AddSMMode.dwSMItemID[i] = ITEM_INDEX( 10286, II_CHR_SYS_SCR_DEEARTHYSTONE );
					break;
				}
				m_nDefenseResist = (BYTE)0xff;
				pItemElemParts = m_Inventory.GetEquip( PARTS_UPPER_BODY );
			}

			if( pItemElemParts && ( i == SM_RESIST_DEFENSE || i == SM_RESIST_ATTACK_LEFT || i == SM_RESIST_ATTACK_RIGHT ) )
			{
				pItemElemParts->m_nResistSMItemId = 0;
				((FLWSUser*)this)->AddCommercialElem( pItemElemParts->m_dwObjId, 0 );
			}

			PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( g_AddSMMode.dwSMItemID[i] );
			if( pItemProp )
			{
				if( i == SM_MAX_HP )
				{
					ResetDestParam( pItemProp->dwDestParam[0], pItemProp->nAdjParamVal[1], 1 );
				}
				else if( i == SM_MAX_HP50 )
				{
					ResetDestParam( pItemProp->dwDestParam[0], m_nPlusMaxHitPoint, 1 );
				}
				else if( i == SM_VELOCIJUMP || i == SM_JUMP)
				{
					for( size_t Nth = 0; Nth < MAX_ITEM_PARAM; ++Nth )
					{
						if( pItemProp->dwDestParam[ Nth ] != NULL_ID )
						{
							ResetDestParam( pItemProp->dwDestParam[ Nth ], pItemProp->nAdjParamVal[ Nth ] );
						}
					}
// 					if( pItemProp->dwDestParam1 != -1 )
// 						ResetDestParam( pItemProp->dwDestParam1, pItemProp->nAdjParamVal1 );
// 					if( pItemProp->dwDestParam2 != -1 )
// 						ResetDestParam( pItemProp->dwDestParam2, pItemProp->nAdjParamVal2 );
// 					if( pItemProp->dwDestParam3 != -1 )
// 						ResetDestParam( pItemProp->dwDestParam3, pItemProp->nAdjParamVal3 );
				}
				//sun: 12, // 12�� �ش����������
				// 090917 mirchang - �Ľ�ųǮ ������ ��� �Ⱓ ����
				else if( i == SM_PARTYSKILL30 || i == SM_PARTYSKILL15 || i == SM_PARTYSKILL1 )
				{
//					g_DPCoreClient.SendUserPartySkill( m_idPlayer, PARTY_PARSKILL_MODE, 0, 0, 1 );

					g_PartyMng.NotiRefreshPartyEffectModeToCS( m_idPlayer, m_idparty, PARTY_EFFECT_PARSKILLFULL );
				}
			}
			else
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "No SMItem dwSMItemID[%d] : %d / Name : %s" ), i, g_AddSMMode.dwSMItemID[i], m_szName );
			}

			((FLWSUser*)this)->AddSMMode( i, 0 );

		}
	}
}


BOOL CMover::SetSMMode( int nType, DWORD dwTime )
{
	if( g_AddSMMode.bSMModetime[nType] )	// ����ð�
	{
		CTime tRealtime	= CTime::GetCurrentTime();
		CTimeSpan ctp( 0, 0, 0, dwTime );
		tRealtime += ctp;
		m_dwSMTime[nType] = (time_t)( tRealtime.GetTime() );
	}
	else	// ī��Ʈ
	{
		m_dwSMTime[nType] = dwTime;
	}

	((FLWSUser*)this)->AddSMMode( nType, m_dwSMTime[nType] );

	if( nType == SM_PARTYSKILL1 || nType == SM_PARTYSKILL15 || nType == SM_PARTYSKILL30 )
	{
		g_PartyMng.NotiSetPartyEffectModeToCS( m_idPlayer, m_idparty, PARTY_EFFECT_PARSKILLFULL, true );
	}

	return TRUE;
}

#endif // __WORLDSERVER

LPCHARACTER CMover::GetCharacter()
{
	return prj.GetCharacter( m_szCharacterKey );
}

#ifdef __WORLDSERVER
void CMover::SetMarkingPos()
{
	m_vMarkingPos = GetPos();
	m_idMarkingWorld	= GetWorld()->GetID();
}
#endif

// NPC���� ������Ƽ�� �ʱ�ȭ�Ѵ�.
void CMover::InitNPCProperty()
{
#ifdef __WORLDSERVER
	if( m_pNpcProperty == NULL )
		m_pNpcProperty = new CNpcProperty;
#endif
}

#if defined(__WORLDSERVER)	
void CMover::InitCharacter( LPCHARACTER lpCharacter )
{
	if( lpCharacter )
	{
		if( lpCharacter->m_nEquipNum > 0 )
			m_Inventory.SetItemContainer( ITYPE_ITEM, CONTAINER_TYPE_INVENTORY, MAX_INVENTORY, MAX_HUMAN_PARTS );

		FLStrcpy( m_szName, _countof( m_szName ), lpCharacter->m_strName );
		for( int i = 0; i < lpCharacter->m_nEquipNum; i++ )
		{
			DWORD adwItemObjID[MAX_INVENTORY] = { NULL_ID, };
			int anNum[MAX_INVENTORY] = { 0, };
			DWORD dwCount = 0;

			FLItemElem itemElem;
			itemElem.m_dwItemId	= lpCharacter->m_adwEquip[ i ];
			itemElem.m_nItemNum	= 1;
			itemElem.SetSerialNumber( static_cast< SERIALNUMBER >( CSerialNumber::SN_IGNORE_SERIAL_NUMBER ) );
			m_Inventory.Add( &itemElem, adwItemObjID, anNum, &dwCount );
			FLItemElem* pAddItem = m_Inventory.GetAtId( adwItemObjID[0] );
			if( pAddItem != NULL )
			{
				m_Inventory.DoEquip( pAddItem->m_dwObjIndex, itemElem.GetProp()->dwParts );
			}
		}

		m_dwHairMesh = lpCharacter->m_dwHairMesh;
		m_dwHairColor = lpCharacter->m_dwHairColor;
		m_dwHeadMesh = lpCharacter->m_dwHeadMesh;

		AllocShopInventory( lpCharacter );
		LoadDialog();		// npcproperty->LoadDialog()
	}
	m_nHitPoint = GetMaxHitPoint();
}

BOOL CMover::LoadDialog()
{
	if( m_pNpcProperty )
	{
		LPCHARACTER lpCharacter = GetCharacter();
		if( lpCharacter )
			return m_pNpcProperty->LoadDialog( lpCharacter );
	}

	return FALSE;
}
#endif // __WORLDSERVER



void CMover::ProcessQuest()
{
#ifdef __WORLDSERVER 
	if( IsPlayer() == FALSE ) 
		return;
	BOOL bTimer = FALSE;
	if( m_timerQuestLimitTime.IsTimeOut() )
	{
		m_timerQuestLimitTime.Reset();
		bTimer = TRUE;
	}
	for( size_t i = 0; i < m_nQuestSize; i++ )
	{
		LPQUEST lpQuest = (LPQUEST) &m_aQuest[ i ];
		if( lpQuest )
		{
			QuestProp* pQuestProp = prj.m_aPropQuest.GetAt( lpQuest->m_wId );
			if( pQuestProp )
			{
				D3DXVECTOR3 vPos = GetPos();
				// ����Ʈ ���������� ���� ��Ʈ�� �ӹ� ���� �÷� ���� 
				if( pQuestProp->m_dwEndCondPatrolWorld == GetWorld()->m_dwWorldID && pQuestProp->m_rectEndCondPatrol.PtInRect( CPoint( (int) vPos.x, (int) vPos.z ) ) )
				{
					if( lpQuest->m_bPatrol == FALSE )
					{
						lpQuest->m_bPatrol = TRUE;
						// ���⼭ Ŭ�� �Ž����� �ϳ� ������. 
						// Ž�縦 �Ϸ��߽��ϴ�.
						// 20 ������ 2������ ��ġ�߽��ϴ�.
						((FLWSUser*)this)->AddSetQuest( lpQuest ); 
					}
				}
				// �ð� ���� ����Ʈ �ð� ī��Ʈ 
				if( pQuestProp->m_nEndCondLimitTime && bTimer )
				{
					if( lpQuest->m_wTime && !(lpQuest->m_wTime & 0x8000) ) 
					{
						lpQuest->m_wTime--;
						((FLWSUser*)this)->AddSetQuest( lpQuest ); 
					}
				}
			}
			else
			{
				//FLERROR_LOG( PROGRAM_NAME, _T( "ProcessQuest : pQuestProp NULL�̴�." ) );
				//FLTRACE_LOG( PROGRAM_NAME, _T( "%s�� Quest %d�� pQuestProp NULL�̴�." ), m_szName, lpQuest->m_wId );
			}
		}
		else
		{
			//FLERROR_LOG( PROGRAM_NAME, _T( "ProcessQuest : lpQuest�� NULL�̴�." ) );
			//FLTRACE_LOG( PROGRAM_NAME, _T( "%s�� lpQuest�� NULL�̴�." ), m_szName );
		}
	}
#else // __WORLDSERVER
	if( IsPlayer()  ) 
		return;
	LPCHARACTER lpCharacter = GetCharacter();
	if( lpCharacter )
	{
		CMover* pMover = GetActiveMover();
		m_nQuestEmoticonIndex = -1;
		// NPC ����Ʈ �̸�Ƽ�� �켱���� ���� - �Ϸ� > �ű� > ���� > �ٷ���
		for( int i = 0; i < lpCharacter->m_awSrcQuest.GetSize() ; i++ )
		{
			int nQuest = lpCharacter->m_awSrcQuest.GetAt( i );
			int nItem = lpCharacter->m_anSrcQuestItem.GetAt( i );
			LPQUEST lpQuest = pMover->GetQuest( nQuest );

			// ���ο� ����Ʈ�� ���
			if( lpQuest == NULL && pMover->IsCompleteQuest( nQuest ) == FALSE )
			{
				// ���� ����Ʈ ���� �����ΰ�?
				if( __IsBeginQuestCondition( pMover, nQuest ) && ( nItem == 0 || pMover->GetItemNum( nItem ) ) )
					m_nQuestEmoticonIndex = (nQuest << 8) | 0x01;
				// ���ο� ����Ʈ�� ���� �ٷ����� ���� �� �ִ� ����Ʈ�� �����Ѵٸ�..
				// gmpbigsun( 20100502 ) : �켱���� ���� ���� 
				else if( (m_nQuestEmoticonIndex & 0xFF) != 0x01 && __IsNextLevelQuest( pMover, nQuest ) && ( nItem == 0 || pMover->GetItemNum( nItem ) ) )
					m_nQuestEmoticonIndex = (nQuest << 8) | 0x04;
			}
			// �������� ����Ʈ�� ���
			if( lpQuest && prj.m_aPropQuest.GetAt( lpQuest->m_wId ) && pMover->IsCompleteQuest( nQuest ) == FALSE && lpQuest->m_nState != QS_END )
			{
				if( (m_nQuestEmoticonIndex & 0xFF) != 0x01 && ( nItem == 0 || pMover->GetItemNum( nItem ) ) )
					m_nQuestEmoticonIndex = (nQuest << 8) | 0x02;
			}
		}
		for( int i = 0; i < lpCharacter->m_awDstQuest.GetSize(); i++ )
		{
			int nQuest = lpCharacter->m_awDstQuest.GetAt( i );
			int nItem = lpCharacter->m_anDstQuestItem.GetAt( i );
			LPQUEST lpQuest = pMover->GetQuest( nQuest );

			// ����Ʈ�� ���� ���� ��� 
			if( lpQuest && prj.m_aPropQuest.GetAt( lpQuest->m_wId ) && pMover->IsCompleteQuest( nQuest ) == FALSE && lpQuest->m_nState != QS_END )
			{
				// ���� ����Ʈ ���� �����ΰ�?
				if( __IsEndQuestCondition( pMover, nQuest ) && ( nItem == 0 || pMover->GetItemNum( nItem ) ) )
					m_nQuestEmoticonIndex = (nQuest << 8) | 0x03;
				// ���� ����Ʈ ���� ������ �ƴϰ� ���ο� ����Ʈ�� �������� ������..
				else if( (m_nQuestEmoticonIndex & 0xFF) != 0x03 && (m_nQuestEmoticonIndex & 0xFF) != 0x01 && ( nItem == 0 || pMover->GetItemNum( nItem ) ) )
					m_nQuestEmoticonIndex = (nQuest << 8) | 0x02;
			}
		}
	}
#endif // __WORLDSERVER
}

#ifdef __WORLDSERVER
void CMover::ProcessAbnormalState()
{
	if( m_wStunCnt > 0 )
	{
		if( --m_wStunCnt <= 0 )
			SetStun( FALSE );	// ���� ���� ����.
	}

	if( m_wDarkCover > 0 )
	{
		if( --m_wDarkCover <= 0 )
			SetDarkCover( FALSE );	// ���� ���� ����.
	}

	if( m_dwSleepingDuration > 0 )
	{
		if( --m_dwSleepingDuration <= 0 )
		{
			SetSleeping( false );	// ������ ���� ����
		}
	}

	if( GetAdjParam( DST_CHRSTATE ) & CHS_POISON )
	{
		if( IsLive() )
		{
			int nUnitCnt = (int)( (m_tmPoisonUnit / 1000.0f) * PROCESS_COUNT );
			if( nUnitCnt <= 0 )		
			{
				nUnitCnt = 1;
				SetPoison( FALSE );
			} 
			else
			{
				if( (m_nCount % nUnitCnt) == 0 )		// xx�ʸ��� �ѹ��� ������.
				{
					CCtrl *pAttacker = prj.GetCtrl( m_idPoisonAttacker );
					if( IsValidObj( pAttacker ) && pAttacker->GetType() == OT_MOVER )
					{
						m_pActMover->SendDamage( AF_FORCE, 
							pAttacker->GetId(), 
							m_wPoisonDamage );
					}
				}

				if( m_wPoisonCnt > 0 )	// �� ���� �ð�����.. SkillInfluence�� ������ �̰� 0�̴�
					if( --m_wPoisonCnt <= 0 )	// �� �ð� �ٵǸ�
						SetPoison( FALSE );		// ������ ����.

			}
		}
	}

	if( m_wDarkCnt > 0 )	// ���� ���ӽð�����...
	{
		if( --m_wDarkCnt <= 0 )
			SetDark( FALSE );
	}

	if( GetAdjParam( DST_CHRSTATE ) & CHS_BLEEDING )
	{
		if( IsLive() )
		{
			int nUnitCnt = (int)( (m_tmBleedingUnit / 1000.0f) * PROCESS_COUNT );
			if( nUnitCnt <= 0 )		
			{
				nUnitCnt = 1;
				SetBleeding( FALSE );
			} 
			else
			{
				if( (m_nCount % nUnitCnt) == 0 )		// xx�ʸ��� �ѹ��� ������.
				{
					CCtrl *pAttacker = prj.GetCtrl( m_idBleedingAttacker );
					if( IsValidObj( pAttacker ) && pAttacker->GetType() == OT_MOVER )
					{
						m_pActMover->SendDamage( AF_FORCE, 
							pAttacker->GetId(), 
							m_wBleedingDamage );
					}
				}

				if( m_wBleedingCnt > 0 )	// ���� ���� �ð�����..
					if( --m_wBleedingCnt <= 0 )	// �� �ð� �ٵǸ�
						SetBleeding( FALSE );		// ������ ����.
			}
		}
	}

	int nHeal	= GetAdjParam( DST_HEAL );
	if( nHeal > 0 )
	{
		if( m_nHealCnt > 0 )
			m_nHealCnt--;

		if( m_nHealCnt == 0 )
		{
			if( IsLive() )
			{
				m_nHealCnt	= (short)( PROCESS_COUNT * 2.0f );		// ��Ÿ�� 2��
				SetDestParam( DST_HP, nHeal, NULL_CHGPARAM, TRUE );
				//SetPointParam( DST_HP, nHeal );
				// mirchang_100830 ȸ���� �⵵..
			}
		}
	}
}

void	CMover::ClearAbnormalState()
{
	if( GetAdjParam( DST_CHRSTATE ) & CHS_STUN )
	{
		SetStun( FALSE );	// ���� ���� ����.
	}

	if( GetAdjParam( DST_CHRSTATE ) & CHS_INVISIBILITY )
	{
		SetDarkCover( FALSE );	// ���� ���� ����.
	}

	if( GetAdjParam( DST_CHRSTATE ) & CHS_SLEEPING )
	{
		SetSleeping( false );	// ������ ���� ����
	}

	if( GetAdjParam( DST_CHRSTATE ) & CHS_POISON )
	{
		SetPoison( FALSE );
	}

	if( GetAdjParam( DST_CHRSTATE ) & CHS_DARK )
	{
		SetDark( FALSE );
	}

	if( GetAdjParam( DST_CHRSTATE ) & CHS_BLEEDING )
	{
		SetBleeding( FALSE );
	}
}

// ������ ��� ���������� idAttacker, m_idTargeter�� �˻��ؼ� ���������̸� ������.
void CMover::ProcessTarget()
{
	if( m_idAttacker != NULL_ID || m_idTargeter != NULL_ID )		
	{
		CMover *pAttacker = GETMOVER( m_idAttacker );
		if( IsValidObj( pAttacker ) )
		{
			if( IsValidArea( pAttacker, 32.0f ) == FALSE )
			{
				m_idAttacker = NULL_ID;
			}
		} 
		else
		{
			m_idAttacker = NULL_ID;		// ����Ŀ�� �ƿ� ���������� 
		}

		CMover *pTargeter = GETMOVER( m_idTargeter );
		if( IsValidObj( pTargeter ) )
		{
			if( IsValidArea( pTargeter, 32.0f ) == FALSE )
			{
				m_idTargeter = NULL_ID;
			}
		} 
		else
		{
			m_idTargeter = NULL_ID;		// Ÿ���Ͱ� �ƿ� ���������� �ʱ�ȭ.
		}
	}
}

void CMover::ProcessScript()
{ 
	if( IsPlayer() ) 
		return;

	if( m_szCharacterKey[ 0 ] && m_pNpcProperty->IsTimeOut() ) 
	{
		m_pNpcProperty->ResetTimer();
		m_pNpcProperty->RunDialog("#auto",NULL,0,GetId(),GetId(),0);
	}
}

void CMover::ProcessRegenItem()
{
	if( IsNPC() )
	{
		if( m_bRegenItem )
		{

			m_bRegenItem = FALSE;
			//
			// vender item �߻� 
			//
			//#define VENDER_DELAY (60 * 5) // 5�� 
			LPCHARACTER pCharacter = GetCharacter();
			BOOL fShop	= FALSE;
			if( !pCharacter )
				return;

			LPVENDOR_ITEM pVendor;
			for( int i = 0; i < MAX_VENDOR_INVENTORY_TAB; i ++ )
			{
				//sun: 11, Ĩ���� ���� �̿� �ϱ� (��� ���� ����)
				if(pCharacter->m_nVenderType == VENDOR_TYPE_CHIP) // Ĩ���� �ŷ��ϴ� vender�� ���
				{
					if(pCharacter->m_venderItemAry2[i].GetSize())
					{
						fShop	= TRUE;
						m_ShopInventory[i]->Clear();		// m_pack�� �� ���ش�.
						for( int j = 0; j < pCharacter->m_venderItemAry2[i].GetSize(); j++ )
						{
							pVendor	= (LPVENDOR_ITEM)pCharacter->m_venderItemAry2[i].GetAt(j);
							FLItemElem itemElem;
							itemElem.m_dwItemId	= pVendor->m_dwItemId;
							itemElem.m_nItemNum	= (int)( g_xSpecManager->GetSpecItem( pVendor->m_dwItemId )->dwPackMax );

							const PT_ITEM_SPEC pProp = g_xSpecManager->GetSpecItem( pVendor->m_dwItemId );
							itemElem.m_nHitPoint = ( pProp->dwEndurance == -1 ) ? 0 : pProp->dwEndurance;//->dwEndurance;
							itemElem.SetSerialNumber( static_cast< SERIALNUMBER >( CSerialNumber::SN_IGNORE_SERIAL_NUMBER ) );

							if( (int)itemElem.GetChipCost() < 1 )
								FLERROR_LOG( PROGRAM_NAME, _T( "chip cost < 1 : npc = %s, item = %d" ), pCharacter->m_szKey, pVendor->m_dwItemId );
							else
								m_ShopInventory[i]->Add( &itemElem );
						}
					}
				}
				else
				{

					if( pCharacter->m_venderItemAry[i].GetSize() )
					{
						fShop	= TRUE;
						{
							m_ShopInventory[i]->Clear();		// m_pack�� �� ���ش�.

							PT_ITEM_SPEC apItemProp[MAX_VENDOR_INVENTORY];
							int cbSize	= 0;
							// generate
							for( int j = 0; j < pCharacter->m_venderItemAry[i].GetSize(); j++ )
							{
								pVendor		= (LPVENDOR_ITEM)pCharacter->m_venderItemAry[i].GetAt(j);
								GenerateVendorItem( apItemProp, &cbSize, MAX_VENDOR_INVENTORY, pVendor );
							}
							// sort
							for( int j = 0; j < cbSize - 1; j++ )
							{
								for( int k = j + 1; k < cbSize; k++ )
								{
									if( ( apItemProp[k]->dwItemKind1 < apItemProp[j]->dwItemKind1 ) ||
										( apItemProp[k]->dwItemKind1 == apItemProp[j]->dwItemKind1 && apItemProp[k]->dwItemRare < apItemProp[j]->dwItemRare ) )
									{
										PT_ITEM_SPEC ptmp	= apItemProp[j];
										apItemProp[j]	= apItemProp[k];
										apItemProp[k]	= ptmp;
									}
								}
							}
							// add
							for( int j = 0; j < cbSize; j++ )
							{
								FLItemElem itemElem;
								itemElem.m_dwItemId	= apItemProp[j]->dwID;
								itemElem.m_nItemNum		= (int)( apItemProp[j]->dwPackMax );
								itemElem.m_nHitPoint	= ( apItemProp[j]->dwEndurance == -1 ) ? 0 : apItemProp[j]->dwEndurance;
								itemElem.SetSerialNumber( static_cast< SERIALNUMBER >( CSerialNumber::SN_IGNORE_SERIAL_NUMBER ) );

								if( m_ShopInventory[i]->Add( &itemElem ) == FALSE )
									break;
							}
						}
					}
					//sun: 11, Ĩ���� ���� �̿� �ϱ� (��� ���� ����)
				}

			}

			if( fShop )
				g_xWSUserManager->AddVendor( this );

		}
	}
}

#endif	// __WORLDSERVER

CModel* CMover::LoadModel( LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwType, DWORD dwIndex )
{
	if( m_dwIndex == OBJECT_INDEX( 12, MI_FEMALE ) || m_dwIndex == OBJECT_INDEX( 11, MI_MALE ) )
		return prj.m_modelMng.LoadModel( pd3dDevice, dwType, dwIndex, TRUE );
	return prj.m_modelMng.LoadModel( pd3dDevice, dwType, dwIndex );
}



// �� �Լ��� �����ϱ� ���� �ε����� Ÿ���� ���� ���õǾ� �־���Ѵ�.
void CMover::InitProp( BOOL bInitAI )
{
	MoverProp* pProp = GetProp();
	if( pProp == NULL )
	{  
		FLERROR_LOG( PROGRAM_NAME, _T( "GetProp(%d) return NULL" ), GetIndex() );
		FLASSERT( 0 );
		return;
	}

	if( m_szName[0] == '\0' )
		FLStrcpy( m_szName, _countof( m_szName ), pProp->szName );

#ifdef __WORLDSERVER
	if( bInitAI )
		SetAIInterface( pProp->dwAI );
#else
	UNUSED_ALWAYS( bInitAI );
#endif	

	m_dwBelligerence = pProp->dwBelligerence;

	if( IsPlayer() )
	{
		m_Inventory.SetItemContainer( ITYPE_ITEM, CONTAINER_TYPE_INVENTORY, MAX_INVENTORY, MAX_HUMAN_PARTS );
		m_Pocket.Avail( 0 );
	}

	m_nDBDataSlot = 0;
	m_bBank = FALSE;
	for( int k = 0 ; k < MAX_CHARACTER_SLOT ; ++k )
	{
		if( IsPlayer() )
		{
			m_Bank[k].SetItemContainer( ITYPE_ITEM, CONTAINER_TYPE_USER_BANK, MAX_BANK ) ;
		}
		m_dwGoldBank[k] = 0;
		m_idPlayerBank[k] = 0;
	}
	ZeroMemory( m_ShopInventory, sizeof( m_ShopInventory ) );	

	ZeroMemory( m_aJobSkill, sizeof( m_aJobSkill ) );
	ZeroMemory( m_tmReUseDelay, sizeof( m_tmReUseDelay ) );

	// �⽺ų �ʱ�ȭ 
	for( int i = 0; i < MAX_SKILL_JOB; i++ )
	{
		m_aJobSkill[ i ].dwSkill = NULL_ID;
	}

	if( m_nJob != -1 ) 
	{
		SkillProp** apSkillProp = prj.m_aJobSkill[ m_nJob ];
		int nJobNum = prj.m_aJobSkillNum[ m_nJob ];

		LPSKILL lpSkill;
		for( int i = 0; i < nJobNum; i++ )
		{
			SkillProp* pSkillProp = apSkillProp[ i ];
			lpSkill = &m_aJobSkill[ i ];
			lpSkill->dwSkill = pSkillProp->dwID;
		}
	}

	// �⺻ �ɷ�ġ �ʱ�ȭ 
	m_nLevel = pProp->dwLevel;
	if( m_nLevel < 1 ) 
		m_nLevel = 1;

	if( IsPlayer() )
	{
		if( m_nJob != -1 )
		{
			m_nHitPoint     = GetMaxHitPoint();
			m_nManaPoint    = GetMaxManaPoint();
			m_nFatiguePoint = GetMaxFatiguePoint();
		}
		else
		{
			m_nHitPoint     = 1;
			m_nManaPoint    = 1;
			m_nFatiguePoint = 1;
		}

		m_pActMover->m_fSpeed = pProp->fSpeed;
	}
	else
	{
		m_Stat.SetOriginStr( pProp->dwStr );
		m_Stat.SetOriginSta( pProp->dwSta );
		m_Stat.SetOriginDex( pProp->dwDex );
		m_Stat.SetOriginInt( pProp->dwInt );

		m_nHitPoint     = GetMaxHitPoint();
		m_nManaPoint    = GetMaxManaPoint();
		m_nFatiguePoint = GetMaxFatiguePoint();
	}

	UpdateParam();
	m_dwVirtItem = pProp->dwAtk1;	// dwVirtItem�� ������ dwAtk1���� ����.

	if( m_bPlayer == FALSE && pProp->dwFlying == 1 )	// ������ �����ΰ�?
		m_dwTypeFlag |= OBJTYPE_FLYING;

	//#ifdef __WORLDSERVER
	//	m_nResource = pProp->dwMaterialAmount;		// �ڿ���.
	//#endif

	if( IsPlayer() )
	{
		m_aQuest = new QUEST[ MAX_QUEST ]; 
		m_aCompleteQuest = new WORD[ MAX_COMPLETE_QUEST ]; 
		m_aCheckedQuest = new WORD[ MAX_CHECKED_QUEST ];
	}
}

// void CMover::InitLevel( int nJob, LONG nLevel, BOOL bGamma )
// {
// #ifdef __WORLDSERVER
// 	// ��� ������� ������ �ϴ°���
// 	MoverProp* pProp = GetProp();
// 	if( pProp )
// 	{
// 		// ����� ��ų �� ������ �ֱ�.
// 		int nJobSkillBuf[MAX_JOB_SKILL];
// 		int nJobSkillLevelBuf[MAX_JOB_SKILL];
// 		for( int i = 0 ; i < MAX_JOB_SKILL ; ++i )
// 		{
// 			nJobSkillBuf[ i ] = m_aJobSkill[ i ].dwSkill;
// 			nJobSkillLevelBuf[ i ] = m_aJobSkill[ i ].dwLevel;
// 		}
// 		ZeroMemory( m_aJobSkill, sizeof( m_aJobSkill ) );
// 
// 		LPSKILL lpSkill;
// 		// �⽺ų �ʱ�ȭ 
// 		for( int i = 0; i < MAX_SKILL_JOB; i++ )
// 		{
// 			m_aJobSkill[ i ].dwSkill = NULL_ID;
// 			m_aJobSkill[ i ].dwLevel = 0;
// 		}
// 
// 		// 1. ����� ��ų�� �׳� ����
// 		m_nJob	= JOB_VAGRANT;
// 		SkillProp** apSkillProp = prj.m_aJobSkill[ m_nJob ];
// 		int nJobNum = prj.m_aJobSkillNum[ m_nJob ];
// 		for( int i = 0; i < nJobNum; i++ )
// 		{
// 			SkillProp* pSkillProp = apSkillProp[ i ];
// 			lpSkill = &m_aJobSkill[ i ];
// 			lpSkill->dwSkill = pSkillProp->dwID;
// 		}
// 		// 2. �⺻ �ɷ�ġ �ʱ�ȭ 
// 		m_nLevel	= 1;
// 		m_nStr	= m_nSta	= m_nDex	= m_nInt	= 15;
// 		m_nRemainGP = 0;
// 		m_nSkillLevel = 0;
// 		m_nSkillPoint = 0;	
// 
// 		// 3. LP, GP, ����
// 		//////////////////////////////////////////////////////////////////////////
// 		// stat point
// 		for( int i = 1 ; i < nLevel ; i++ )
// 		{
// 			m_nLevel	= i + 1;
// 			m_nRemainGP += prj.m_aExpCharacter[ m_nLevel ].dwLPPoint;
// 
// 			//sun: 10�� ���½ý���	Neuz, World, Trans
// 
// 			if( MAX_PROFESSIONAL <= nJob && i > 59 )
// 			{
// 				++m_nRemainGP;
// 			}
// 
// 			if( nJob < MAX_PROFESSIONAL )
// 			{
// 				if( ( i + 1 ) == MAX_JOB_LEVEL )
// 				{
// 					if( nJob < MAX_EXPERT )
// 					{
// 						AddChangeJob( nJob );
// 					}
// 					else
// 					{
// 						if( nJob % 2 != 0 )
// 						{
// 							AddChangeJob( ( nJob - 5 ) / 2 );
// 						}
// 						else
// 						{
// 							AddChangeJob( ( nJob - 4 ) / 2 );
// 						}
// 					}
// 				}
// 				else
// 					if( ( i + 1 ) == MAX_JOB_LEVEL + MAX_EXP_LEVEL )
// 					{
// 						AddChangeJob( nJob );
// 					}
// 			}
// 			else
// 			{
// 				if( ( i + 1 ) == MAX_JOB_LEVEL )
// 				{
// 					if( nJob < MAX_MASTER )
// 					{
// 						if( nJob % 2 != 0 )
// 						{
// 							AddChangeJob( ( nJob - 15 ) / 2 );
// 						}
// 						else
// 						{
// 							AddChangeJob( ( nJob - 14 ) / 2 );
// 						}
// 					}
// 
// #ifdef __3RD_LEGEND16
// 
// 					else if( nJob < MAX_HERO )
// 					{
// 						if( nJob % 2 != 0 )
// 						{
// 							AddChangeJob( ( nJob - 23 ) / 2 );
// 						}
// 						else
// 						{
// 							AddChangeJob( ( nJob - 22 ) / 2 );
// 						}
// 					}
// 
// 					else if( nJob < MAX_LEGEND_HERO )
// 					{
// 						if( nJob % 2 != 0 )
// 						{
// 							AddChangeJob( ( nJob - 31 ) / 2 );
// 						}
// 						else
// 						{
// 							AddChangeJob( ( nJob - 30 ) / 2 );
// 						}
// 					}
// 
// #else // __3RD_LEGEND16
// 
// 					else
// 					{
// 						if( nJob % 2 != 0 )
// 						{
// 							AddChangeJob( ( nJob - 23 ) / 2 );
// 						}
// 						else
// 						{
// 							AddChangeJob( ( nJob - 22 ) / 2 );
// 						}
// 					}
// 
// #endif // __3RD_LEGEND16
// 
// 				}
// 				else
// 					if( ( i + 1 ) == MAX_JOB_LEVEL + MAX_EXP_LEVEL )
// 					{
// 						if( nJob < MAX_MASTER )
// 						{
// 							AddChangeJob( nJob - 10 );
// 							AddChangeJob( nJob );
// 						}
// 
// #ifdef __3RD_LEGEND16
// 						else if( nJob < MAX_HERO )
// 						{
// 							AddChangeJob( nJob - 18 );
// 							AddChangeJob( nJob - 8);
// 							AddChangeJob( nJob );
// 						}
// 
// 						else if( nJob < MAX_LEGEND_HERO )
// 						{
// 							AddChangeJob( nJob - 26 );
// 							AddChangeJob( nJob - 16 );
// 							AddChangeJob( nJob - 8 );
// 							AddChangeJob( nJob );
// 						}
// 
// #else // __3RD_LEGEND16
// 
// 						else
// 						{
// 							AddChangeJob( nJob - 18 );
// 							AddChangeJob( nJob - 8);
// 							AddChangeJob( nJob );
// 						}
// 
// #endif // __3RD_LEGEND16
// 
// 					}
// 			}
// 
// 		}
// 
// 		if( MAX_MASTER <= nJob )
// 		{
// 			m_nRemainGP += 12;
// 		}
// 		// END stat point
// 		//////////////////////////////////////////////////////////////////////////
// 
// 		int nSkillPoint = GetTotalSkillPoint( nJob, m_nLevel );
// 
// 		m_nSkillLevel = m_nSkillPoint = nSkillPoint;
// 		
// 
// // 		int nPoint = 0;
// // 		if( m_nLevel <= 20 )
// // 			nPoint = 2 * m_nLevel - 2;	
// // 		else if( m_nLevel <= 40 )
// // 			nPoint = 3 * m_nLevel - 22;	
// // 		else if( m_nLevel <= 60 )
// // 			nPoint = 4 * m_nLevel - 62;	
// // 		else if( m_nLevel <= 80 )
// // 			nPoint = 5 * m_nLevel - 122;	
// // 		else if( m_nLevel <= 100 )
// // 			nPoint = 6 * m_nLevel - 202;	
// // 
// // #ifdef __3RD_LEGEND16
// // 
// // 		// ���̺�� ����...
// // 		else
// // 		{
// // 			if( IsLegendHero() == TRUE )
// // 			{
// // 				if( m_nJob == JOB_LORDTEMPLER_HERO || m_nJob == JOB_FLORIST_HERO )
// // 				{
// // 					nPoint = 6 * m_nLevel - 172;
// // 				}
// // 				else
// // 				{
// // 					nPoint = 4 * m_nLevel + 88;
// // 				}
// // 			}
// // 			else
// // 			{
// // 				nPoint = 7 * m_nLevel - 302;
// // 			}
// // 		}
// // 
// // #else // __3RD_LEGEND16
// // 		else
// // 			nPoint = 7 * m_nLevel - 302;		
// // #endif // __3RD_LEGEND16
// // 
// // 		if( m_nJob == JOB_MERCENARY )
// // 			nPoint += 40;
// // 		else if( m_nJob == JOB_ACROBAT )
// // 			nPoint += 50;
// // 		else if( m_nJob == JOB_ASSIST )
// // 			nPoint += 60;
// // 		else if( m_nJob == JOB_MAGICIAN )
// // 			nPoint += 90;
// // 		else if( m_nJob ==  JOB_KNIGHT || m_nJob ==  JOB_BLADE )
// // 			nPoint += 120;
// // 		else if( m_nJob ==  JOB_JESTER || m_nJob ==  JOB_RANGER )
// // 			nPoint += 150;
// // 		else if( m_nJob ==  JOB_RINGMASTER )
// // 			nPoint += 160;
// // 		else if( m_nJob ==  JOB_BILLPOSTER || m_nJob ==  JOB_PSYCHIKEEPER )
// // 			nPoint += 180;
// // 		else if( m_nJob ==  JOB_ELEMENTOR )
// // 			nPoint += 390;
// // 
// // #ifdef __3RD_LEGEND16
// // 
// // 		else
// // 		{
// // 			if( IsLegendHero() == TRUE )
// // 			{
// // 				if( m_nJob == JOB_LORDTEMPLER_HERO || m_nJob == JOB_FLORIST_HERO )
// // 				{
// // 					nPoint += 24;
// // 				}
// // 				else if( m_nJob == JOB_MENTALIST_HERO )
// // 				{
// // 					nPoint += 20;
// // 				}
// // 				else
// // 				{
// // 					nPoint += 16;
// // 				}
// // 			}
// // 		}
// // 
// // #endif // __3RD_LEGEND16
// // 
// // 		m_nSkillLevel = m_nSkillPoint = nPoint;
// 
// 		SetJobLevel( nLevel, nJob );
// 		m_nDeathLevel = nLevel;
// 		//sun: 10�� ���½ý���	Neuz, World, Trans
// 		if(IsMaster())
// 		{
// 			int dwTmpSkLevel = 1;//60, 72, 84, 96, 108
// 			if( nLevel > 59 && nLevel < 72 )
// 				dwTmpSkLevel = 1;
// 			else if( nLevel > 71 && nLevel < 84 )
// 				dwTmpSkLevel = 2;
// 			else if( nLevel > 83 && nLevel < 96 )
// 				dwTmpSkLevel = 3;
// 			else if( nLevel > 95 && nLevel < 108 )
// 				dwTmpSkLevel = 4;
// 			else if( nLevel > 107 && nLevel < 120 )
// 				dwTmpSkLevel = 5;
// 			for( int i = 0; i < MAX_SKILL_JOB; i++ ) 
// 			{				
// 				LPSKILL lpSkill = &(m_aJobSkill[i]);
// 				if( lpSkill && lpSkill->dwSkill != NULL_ID )
// 				{
// 					SkillProp* pSkillProp    = prj.GetSkillProp( lpSkill->dwSkill );			
// 					if( pSkillProp == NULL )
// 						continue;
// 					if( pSkillProp->dwSkillKind1 != JTYPE_MASTER )
// 						continue;
// 					lpSkill->dwLevel = dwTmpSkLevel;
// 				}
// 			}
// 		}
// #ifdef __3RD_LEGEND16
// 		else if( IsHero() == TRUE || IsLegendHero() == TRUE )
// #else // __3RD_LEGEND16
// 		else if(IsHero())
// #endif // __3RD_LEGEND16
// 		{
// 			for( int i = 0; i < MAX_SKILL_JOB; i++ ) 
// 			{				
// 				LPSKILL lpSkill = &(m_aJobSkill[i]);
// 				if( lpSkill && lpSkill->dwSkill != NULL_ID )
// 				{
// 					SkillProp* pSkillProp    = prj.GetSkillProp( lpSkill->dwSkill );			
// 					if( pSkillProp == NULL )
// 						continue;
// 					if( pSkillProp->dwSkillKind1 != JTYPE_MASTER )
// 						continue;
// 					lpSkill->dwLevel = 5;
// 				}
// 			}
// 		}
// 
// 		if( bGamma )
// 		{
// 			m_nExp1 = 0;
// 		}
// 
// 		if( m_nLevel >= 20 )
// 			SetFlightLv( 1 );
// 		else
// 			SetFlightLv( 0 );
// 
// 		( (FLWSUser*)this )->AddSetChangeJob( nJob );
// 		g_xWSUserManager->AddNearSetChangeJob( this, nJob, &((FLWSUser*)this)->m_aJobSkill[MAX_JOB_SKILL] );
// 
// 		//sun: 11, ĳ���� ���� ����
// 		g_dpDBClient.SendUpdatePlayerData( (FLWSUser*)this );
// 
// 		SetHitPoint( GetMaxHitPoint() );
// 		SetManaPoint( GetMaxManaPoint() );
// 		SetFatiguePoint( GetMaxFatiguePoint() );
// 
// 		g_xWSUserManager->AddSetLevel( this, (WORD)m_nLevel );
// 		( (FLWSUser*)this )->AddSetGrowthLearningPoint( m_nRemainGP );
// 		( (FLWSUser*)this )->AddSetExperience( GetExp1(), (WORD)m_nLevel, m_nSkillPoint, m_nSkillLevel );
// 		( (FLWSUser*)this )->m_playTaskBar.InitTaskBarShorcutKind( SHORTCUT_SKILL );
// 		( (FLWSUser*)this )->AddTaskBar();
// 		((FLWSUser*)this)->AddSetState( m_nStr, m_nSta, m_nDex, m_nInt, m_nRemainGP );
// 		//sun: 13, ����
// 		((FLWSUser*)this)->CheckHonorStat();
// 		((FLWSUser*)this)->AddHonorListAck();
// 		g_xWSUserManager->AddHonorTitleChange( this, m_nHonor);
// 
// 		g_pMadrigalGift->OnLevel( static_cast<FLWSUser*>( this ) );
// 	}
// #endif // __WORLDSERVER
// }

void CMover::InitLevel( int nJob, LONG nLevel, BOOL bGamma )
{
#ifdef __WORLDSERVER
	if( nJob < JOB_VAGRANT || nJob >= MAX_JOB || nLevel <= 0 || nLevel > MAX_CHARACTER_LEVEL )
	{
		nJob = JOB_VAGRANT;
		nLevel = 1;
	}

	// ��� ������� ������ �ϴ°���
	const MoverProp* pMoverProp = GetProp();
	if( pMoverProp == NULL )
	{
		return;
	}
	{
		ZeroMemory( m_aJobSkill, sizeof( m_aJobSkill ) );

		// �⽺ų �ʱ�ȭ 
		for( int i = 0; i < MAX_SKILL_JOB; i++ )
		{
			m_aJobSkill[ i ].dwSkill = NULL_ID;
			m_aJobSkill[ i ].dwLevel = 0;
		}

		// ��ų ����
		DWORD dwSubLineJob	= nJob;
		do 
		{
			dwSubLineJob	= FLCommonFunction::GetSubLineJob( dwSubLineJob );

			if( dwSubLineJob != NULL_ID )
			{
				AddChangeJob( dwSubLineJob );
			}

		} while ( dwSubLineJob != NULL_ID );

		AddChangeJob( nJob );

		// 2. �⺻ �ɷ�ġ �ʱ�ȭ 
		m_nLevel	= 1;
		m_Stat.InitializeOriginStat();
		m_nSkillLevel = 0;
		m_nSkillPoint = 0;	

		// 3. LP, GP, ����
		//////////////////////////////////////////////////////////////////////////
		// stat point
		for( int i = 1 ; i < nLevel ; i++ )
		{
			m_nLevel	= i + 1;
			m_Stat.SetRemainStatPoint( m_Stat.GetRemainStatPoint() + prj.m_aExpCharacter[ m_nLevel ].dwLPPoint );

			//sun: 10�� ���½ý���	Neuz, World, Trans

			if( MAX_PROFESSIONAL <= nJob && i > 59 )
			{
				m_Stat.SetRemainStatPoint( m_Stat.GetRemainStatPoint() + 1 );
			}
		}

		if( MAX_MASTER <= nJob )
		{
			m_Stat.SetRemainStatPoint( m_Stat.GetRemainStatPoint() + 12 );
		}
		// END stat point
		//////////////////////////////////////////////////////////////////////////

		const int nSkillPoint = FLCommonFunction::GetTotalSkillPoint( nJob, m_nLevel );

		m_nSkillLevel = m_nSkillPoint = nSkillPoint;


		SetJobLevel( nLevel, nJob );
		m_nDeathLevel = nLevel;
		//sun: 10�� ���½ý���	Neuz, World, Trans
		if(IsMaster())
		{
			int dwTmpSkLevel = 1;//60, 72, 84, 96, 108
			if( nLevel > 59 && nLevel < 72 )
				dwTmpSkLevel = 1;
			else if( nLevel > 71 && nLevel < 84 )
				dwTmpSkLevel = 2;
			else if( nLevel > 83 && nLevel < 96 )
				dwTmpSkLevel = 3;
			else if( nLevel > 95 && nLevel < 108 )
				dwTmpSkLevel = 4;
			else if( nLevel > 107 && nLevel < 120 )
				dwTmpSkLevel = 5;
			for( int i = 0; i < MAX_SKILL_JOB; i++ ) 
			{				
				LPSKILL lpSkill = &(m_aJobSkill[i]);
				if( lpSkill && lpSkill->dwSkill != NULL_ID )
				{
					SkillProp* pSkillProp    = prj.GetSkillProp( lpSkill->dwSkill );			
					if( pSkillProp == NULL )
						continue;
					if( pSkillProp->dwSkillKind1 != JTYPE_MASTER )
						continue;
					lpSkill->dwLevel = dwTmpSkLevel;
				}
			}
		}
		else if( IsHero() == TRUE || IsLegendHero() == TRUE )
		{
			for( int i = 0; i < MAX_SKILL_JOB; i++ ) 
			{				
				LPSKILL lpSkill = &(m_aJobSkill[i]);
				if( lpSkill && lpSkill->dwSkill != NULL_ID )
				{
					SkillProp* pSkillProp    = prj.GetSkillProp( lpSkill->dwSkill );			
					if( pSkillProp == NULL )
						continue;
					if( pSkillProp->dwSkillKind1 != JTYPE_MASTER )
						continue;
					lpSkill->dwLevel = 5;
				}
			}
		}

		if( bGamma )
		{
			m_nExp1 = 0;
		}

		if( m_nLevel >= 20 )
			SetFlightLv( 1 );
		else
			SetFlightLv( 0 );

		( (FLWSUser*)this )->AddSetChangeJob( nJob );
		g_xWSUserManager->AddNearSetChangeJob( this, nJob, &((FLWSUser*)this)->m_aJobSkill[MAX_JOB_SKILL] );

		//sun: 11, ĳ���� ���� ����
		g_dpDBClient.SendUpdatePlayerData( (FLWSUser*)this );

		SetHitPoint( GetMaxHitPoint() );
		SetManaPoint( GetMaxManaPoint() );
		SetFatiguePoint( GetMaxFatiguePoint() );

		g_xWSUserManager->AddSetLevel( this, (WORD)m_nLevel );
		( (FLWSUser*)this )->AddSetExperience( GetExp1(), (WORD)m_nLevel, m_nSkillPoint, m_nSkillLevel );
		( (FLWSUser*)this )->m_playTaskBar.InitTaskBarShorcutKind( SHORTCUT_SKILL );
		( (FLWSUser*)this )->AddTaskBar();
		g_xWSUserManager->AddSetState( this );
		//sun: 13, ����
		((FLWSUser*)this)->CheckHonorStat();
		((FLWSUser*)this)->AddHonorListAck();
		g_xWSUserManager->AddHonorTitleChange( this, m_nHonor);

		g_pMadrigalGift->OnLevel( static_cast<FLWSUser*>( this ) );
	}
#endif // __WORLDSERVER
}

//////////////////////////////////////////////////////////////////////////
// #ifdef __WORLDSERVER
// void	CMover::CorrectErrorsSkillPoint()
// {
// 	const DWORD dwLevel				= ( m_nDeathLevel > m_nLevel ) ? m_nDeathLevel : m_nLevel;
// 
// 	const int nTotalSkillPoint		= FLCommonFunction::GetTotalSkillPoint( m_nJob, dwLevel );
// 	const int nUseSkillPoint		= GetUseSkillPoint();
// 
// 	// ĳ���Ͱ� ������ �ִ� �� ��ų����Ʈ�� ���� ������ �־�� �� �� ��ų����Ʈ�� �ٸ��� ��й�..
// 	if( m_nSkillLevel != nTotalSkillPoint )
// 	{
// 		m_nSkillLevel = nTotalSkillPoint;
// 
// 		const int nRewardPoint = nTotalSkillPoint - nUseSkillPoint - m_nSkillPoint;
// 
// 		g_dpDBClient.SendLogSkillPoint( LOG_SKILLPOINT_GET_REWARD, nRewardPoint, this, NULL );
// 	}
// 
// 	// ���� ��ų ����Ʈ ������Ʈ
// 	if( nTotalSkillPoint > nUseSkillPoint )
// 	{
// 		m_nSkillPoint = ( nTotalSkillPoint - nUseSkillPoint );
// 	}
// 	else
// 	{
// 		m_nSkillPoint = 0;
// 	}
// }
// 
// int		CMover::GetUseSkillPoint() const
// {
// 	int nUseSkillPoint = 0;
// 
// 	for( int i = 0 ; i < MAX_SKILL_JOB ; ++i )
// 	{
// 		const SKILL* pSkill = &m_aJobSkill[ i ];
// 		if( pSkill != NULL && pSkill->dwSkill != NULL_ID )
// 		{
// 			const SkillProp* pSkillProp = prj.GetSkillProp( pSkill->dwSkill );
// 			if( pSkillProp != NULL )
// 			{
// 				if( pSkill->dwLevel > 0 && pSkillProp->dwSkillKind1 != JTYPE_MASTER && pSkillProp->dwSkillKind1 != JTYPE_HERO )
// 				{
// 					nUseSkillPoint += ( pSkill->dwLevel * prj.GetSkillPoint( pSkillProp ) ); 
// 				}
// 			}
// 		}
// 	}
// 
// 	return nUseSkillPoint;
// }
// #endif // __WORLDSERVER 
//////////////////////////////////////////////////////////////////////////

int   CMover::SetLevel( int nSetLevel )
{
#ifdef __WORLDSERVER
	//sun: 10�� ���½ý���	Neuz, World, Trans
	m_nLevel = nSetLevel;
	m_nExp1 = 0;
	m_nDeathExp = GetExp1();
	m_nDeathLevel = nSetLevel - 1;
	( (FLWSUser *)this )->AddSetExperience( GetExp1(), (WORD)m_nLevel,  m_nSkillPoint, m_nSkillLevel, m_nDeathExp, (WORD)m_nDeathLevel );		// �ش��������� exp1,exp2����� ������ ����.
	g_xWSUserManager->AddSetLevel( this, (WORD)m_nLevel );
	g_dpDBClient.SendLogLevelUp( this, 1 );	// ������ �α�
#endif
	return 0; 
}

int   CMover::AddGPPoint( int nAddGPPoint )
{
#ifdef __WORLDSERVER
	//sun: 10�� ���½ý���	Neuz, World, Trans
	m_Stat.SetRemainStatPoint( m_Stat.GetRemainStatPoint() + nAddGPPoint );
	( (FLWSUser*)this )->AddSetGrowthLearningPoint( m_Stat.GetRemainStatPoint() );		// pUser���� GP�����Ȱ��� ����.
	g_dpDBClient.SendLogLevelUp( this, 1 );	// ������ �α�
#endif
	return 0; 
}


BOOL CMover::InitSkillExp()
{
	for( int i = 0 ; i < MAX_SKILL_JOB ; ++i )
	{
		LPSKILL pSkill = &m_aJobSkill[ i ];
		if( pSkill != NULL && pSkill->dwSkill != NULL_ID )
		{
			SkillProp* pSkillProp = prj.GetSkillProp( pSkill->dwSkill );
			if( pSkillProp == NULL )
				return FALSE;

			//sun: 10�� ���½ý���	Neuz, World, Trans
			if( 0 < pSkill->dwLevel && pSkillProp->dwSkillKind1 != JTYPE_MASTER && pSkillProp->dwSkillKind1 != JTYPE_HERO )
			{
				m_nSkillPoint += (  pSkill->dwLevel * prj.GetSkillPoint( pSkillProp ) ); 
				pSkill->dwLevel = 0;
			}

			//if( 0 < pSkill->dwLevel )
			//	m_nSkillPoint += (  pSkill->dwLevel * prj.GetSkillPoint( pSkillProp ) ); 
			//pSkill->dwLevel = 0;

		}			
	}

	//sun: 10�� ���½ý���	Neuz, World, Trans
// 	int nMaxPoint = SKILLPOINT_MAX;	// �ִ��� JOB_ELEMENTOR ���� ������
// 	if( m_nJob == JOB_KNIGHT || m_nJob == JOB_BLADE || m_nJob == JOB_KNIGHT_MASTER || m_nJob == JOB_BLADE_MASTER || m_nJob == JOB_KNIGHT_HERO || m_nJob == JOB_BLADE_HERO )
// 		nMaxPoint = SKILLPOINT_MAX_MERCENARY;
// 	else if( m_nJob == JOB_JESTER || m_nJob == JOB_RANGER || m_nJob == JOB_JESTER_MASTER || m_nJob == JOB_RANGER_MASTER || m_nJob == JOB_JESTER_HERO || m_nJob == JOB_RANGER_HERO )
// 		nMaxPoint = SKILLPOINT_MAX_ACROBAT;
// 	else if( m_nJob == JOB_RINGMASTER || m_nJob == JOB_RINGMASTER_MASTER || m_nJob == JOB_RINGMASTER_HERO )
// 		nMaxPoint = SKILLPOINT_MAX_RINGMASTER;
// 	else if( m_nJob == JOB_BILLPOSTER || m_nJob == JOB_PSYCHIKEEPER || m_nJob == JOB_BILLPOSTER_MASTER || m_nJob == JOB_PSYCHIKEEPER_MASTER || m_nJob == JOB_BILLPOSTER_HERO || m_nJob == JOB_PSYCHIKEEPER_HERO )
// 		nMaxPoint = SKILLPOINT_MAX_BILLPO_PSYCHI;
// 
// 	if( m_nSkillPoint > nMaxPoint)
// 		m_nSkillPoint = nMaxPoint;

#ifdef __WORLDSERVER
	( (FLWSUser*)this )->m_playTaskBar.InitTaskBarShorcutKind( SHORTCUT_SKILL );
	( (FLWSUser*)this )->AddTaskBar();
#endif //__WORLDSERVER
	return TRUE;
}

#ifdef __CLIENT
int CMover::GetCurrentMaxSkillPoint()
{
	int nCurrentMaxSkillPoint = m_nSkillPoint;
	for( int i = 0 ; i < MAX_SKILL_JOB ; ++i )
	{
		LPSKILL pSkill = &m_aJobSkill[ i ];
		if( pSkill != NULL && pSkill->dwSkill != NULL_ID )
		{
			SkillProp* pSkillProp = prj.GetSkillProp( pSkill->dwSkill );
			if( pSkillProp != NULL )
			{
				if( 0 < pSkill->dwLevel && pSkillProp->dwSkillKind1 != JTYPE_MASTER && pSkillProp->dwSkillKind1 != JTYPE_HERO )
					nCurrentMaxSkillPoint += (  pSkill->dwLevel * prj.GetSkillPoint( pSkillProp ) );
			}
		}			
	}

// 	int nMaxPoint = SKILLPOINT_MAX;	// �ִ��� JOB_ELEMENTOR ���� ������
// 	if( m_nJob == JOB_KNIGHT || m_nJob == JOB_BLADE || m_nJob == JOB_KNIGHT_MASTER || m_nJob == JOB_BLADE_MASTER || m_nJob == JOB_KNIGHT_HERO || m_nJob == JOB_BLADE_HERO )
// 		nMaxPoint = SKILLPOINT_MAX_MERCENARY;
// 	else if( m_nJob == JOB_JESTER || m_nJob == JOB_RANGER || m_nJob == JOB_JESTER_MASTER || m_nJob == JOB_RANGER_MASTER || m_nJob == JOB_JESTER_HERO || m_nJob == JOB_RANGER_HERO )
// 		nMaxPoint = SKILLPOINT_MAX_ACROBAT;
// 	else if( m_nJob == JOB_RINGMASTER || m_nJob == JOB_RINGMASTER_MASTER || m_nJob == JOB_RINGMASTER_HERO )
// 		nMaxPoint = SKILLPOINT_MAX_RINGMASTER;
// 	else if( m_nJob == JOB_BILLPOSTER || m_nJob == JOB_PSYCHIKEEPER || m_nJob == JOB_BILLPOSTER_MASTER || m_nJob == JOB_PSYCHIKEEPER_MASTER || m_nJob == JOB_BILLPOSTER_HERO || m_nJob == JOB_PSYCHIKEEPER_HERO )
// 		nMaxPoint = SKILLPOINT_MAX_BILLPO_PSYCHI;
// 
// 	if( nCurrentMaxSkillPoint > nMaxPoint)
// 		nCurrentMaxSkillPoint = nMaxPoint;

	return nCurrentMaxSkillPoint;
}

//������ ����Ʈ 
void CMover::SetSfxBuffPet( const DWORD idEffect )
{
	if( m_pSfxBuffPet )
	{
		g_WorldMng.Get()->RemoveObj( m_pSfxBuffPet );
		m_pSfxBuffPet = NULL;
	}

	if( NULL_ID != idEffect )
		m_pSfxBuffPet =  CreateSfx( g_Neuz.m_pd3dDevice, idEffect, GetPos(), GetId(), GetPos(), GetId(), -1 );
}

#endif //__CLIENT
void CMover::ReState()
{
#ifdef __WORLDSERVER
	m_Stat.InitializeOriginStat();

	int nLevelFor = m_nLevel;
	if( m_nLevel < m_nDeathLevel )
		nLevelFor = m_nDeathLevel;
	//sun: 10�� ���½ý���	Neuz, World, Trans
	for( int i = 1 ; i < nLevelFor ; i++ )
	{
		m_Stat.SetRemainStatPoint( m_Stat.GetRemainStatPoint() + prj.m_aExpCharacter[ i + 1 ].dwLPPoint );

		if( ( IsHero() == TRUE || IsLegendHero() == TRUE ) && i == MAX_GENERAL_LEVEL )
		{
			m_Stat.SetRemainStatPoint( m_Stat.GetRemainStatPoint() + 12 );
		}
		if( ( IsMaster() == TRUE || IsHero() == TRUE || IsLegendHero() == TRUE ) && i > 59 )
		{
			m_Stat.SetRemainStatPoint( m_Stat.GetRemainStatPoint() + 1 );
		}
	}


	SetHitPoint( GetMaxHitPoint() );
	SetManaPoint( GetMaxManaPoint() );
	SetFatiguePoint( GetMaxFatiguePoint() );
	g_xWSUserManager->AddSetState( this );
	//sun: 13, ����
	((FLWSUser*)this)->CheckHonorStat();
	((FLWSUser*)this)->AddHonorListAck();
	g_xWSUserManager->AddHonorTitleChange( this, m_nHonor);

	g_dpDBClient.SendLogLevelUp( this, 2 );

#endif // __WORLDSERVER
}

//sun: 10, __S_ADD_RESTATE
void CMover::ReStateOne( int nKind )
{
#ifdef __WORLDSERVER
	int nAdd = 0;
	switch( nKind )
	{
	case 0:		// Str
		{
			nAdd = m_Stat.GetOriginStr() - DEFAULT_ORIGIN_STAT_VALUE;
			m_Stat.SetOriginStr( DEFAULT_ORIGIN_STAT_VALUE );
		}
		break;
	case 1:		// Sta
		{
			nAdd = m_Stat.GetOriginSta() - DEFAULT_ORIGIN_STAT_VALUE;
			m_Stat.SetOriginSta( DEFAULT_ORIGIN_STAT_VALUE );
		}
		break;
	case 2:		// Dex
		{
			nAdd = m_Stat.GetOriginDex() - DEFAULT_ORIGIN_STAT_VALUE;
			m_Stat.SetOriginDex( DEFAULT_ORIGIN_STAT_VALUE );
		}
		break;
	case 3:		// Int
		{
			nAdd = m_Stat.GetOriginInt() - DEFAULT_ORIGIN_STAT_VALUE;
			m_Stat.SetOriginInt( DEFAULT_ORIGIN_STAT_VALUE );
		}
		break;
	}

	m_Stat.SetRemainStatPoint( m_Stat.GetRemainStatPoint() + nAdd );

	SetHitPoint( GetMaxHitPoint() );
	SetManaPoint( GetMaxManaPoint() );
	SetFatiguePoint( GetMaxFatiguePoint() );
	g_xWSUserManager->AddSetState( this );
	//sun: 13, ����
	((FLWSUser*)this)->CheckHonorStat();
	((FLWSUser*)this)->AddHonorListAck();
	g_xWSUserManager->AddHonorTitleChange( this, m_nHonor);

	g_dpDBClient.SendLogLevelUp( this, 2 );

#endif // __WORLDSERVER
}


void CMover::ReStateOneLow( int nKind )
{
#ifdef __WORLDSERVER
	int nAdd = 0;
	switch( nKind )
	{
	case 0:		// Str
		{
			if( m_Stat.GetOriginStr() >= RESTATE_LOW + DEFAULT_ORIGIN_STAT_VALUE )
			{
				nAdd = RESTATE_LOW;
				m_Stat.SetOriginStr( m_Stat.GetOriginStr() - RESTATE_LOW );
			}
			else
			{
				nAdd = m_Stat.GetOriginStr() - DEFAULT_ORIGIN_STAT_VALUE;
				m_Stat.SetOriginStr( DEFAULT_ORIGIN_STAT_VALUE );
			}
		}
		break;
	case 1:		// Sta
		{
			if( m_Stat.GetOriginSta() >= RESTATE_LOW + DEFAULT_ORIGIN_STAT_VALUE )
			{
				nAdd = RESTATE_LOW;
				m_Stat.SetOriginSta( m_Stat.GetOriginSta() - RESTATE_LOW );
			}
			else
			{
				nAdd = m_Stat.GetOriginSta() - DEFAULT_ORIGIN_STAT_VALUE;
				m_Stat.SetOriginSta( DEFAULT_ORIGIN_STAT_VALUE );
			}
		}
		break;
	case 2:		// Dex
		{
			if( m_Stat.GetOriginDex() >= RESTATE_LOW + DEFAULT_ORIGIN_STAT_VALUE )
			{
				nAdd = RESTATE_LOW;
				m_Stat.SetOriginDex( m_Stat.GetOriginDex() - RESTATE_LOW );
			}
			else
			{
				nAdd = m_Stat.GetOriginDex() - DEFAULT_ORIGIN_STAT_VALUE;
				m_Stat.SetOriginDex( DEFAULT_ORIGIN_STAT_VALUE );
			}
		}
		break;
	case 3:		// Int
		{
			if( m_Stat.GetOriginInt() >= RESTATE_LOW + DEFAULT_ORIGIN_STAT_VALUE )
			{
				nAdd = RESTATE_LOW;
				m_Stat.SetOriginInt( m_Stat.GetOriginInt() - RESTATE_LOW );
			}
			else
			{
				nAdd = m_Stat.GetOriginInt() - DEFAULT_ORIGIN_STAT_VALUE;
				m_Stat.SetOriginInt( DEFAULT_ORIGIN_STAT_VALUE );
			}
		}
		break;
	}

	m_Stat.SetRemainStatPoint( m_Stat.GetRemainStatPoint() + nAdd );

	SetHitPoint( GetMaxHitPoint() );
	SetManaPoint( GetMaxManaPoint() );
	SetFatiguePoint( GetMaxFatiguePoint() );
	g_xWSUserManager->AddSetState( this );
	//sun: 13, ����
	((FLWSUser*)this)->CheckHonorStat();
	((FLWSUser*)this)->AddHonorListAck();
	g_xWSUserManager->AddHonorTitleChange( this, m_nHonor);

	g_dpDBClient.SendLogLevelUp( this, 2 );

#endif // __WORLDSERVER
}

#ifdef __WORLDSERVER

BOOL CMover::ReplaceInspection( REGIONELEM* pPortkey )
{
	BOOL bResult = TRUE;
	if( bResult != FALSE && pPortkey->m_uItemId != 0xffffffff )
	{
		int nCount = GetItemNum( pPortkey->m_uItemId );
		if( (DWORD)( nCount ) <= pPortkey->m_uiItemCount )
		{
			bResult = TRUE;
		}
		else
		{
			bResult = FALSE;
		}

		FLTRACE_LOG( PROGRAM_NAME, _T( "ReplaceInspection Item %d Count %d/%d Inspection %d" ), pPortkey->m_uItemId, nCount, pPortkey->m_uiItemCount, bResult );
	}
	if( bResult != FALSE && pPortkey->m_uiMinLevel != 0xffffffff )
	{
		if( pPortkey->m_uiMinLevel <= (UINT)( m_nLevel ) && (UINT)( m_nLevel ) <= pPortkey->m_uiMaxLevel)
		{
			bResult = TRUE;
		}
		else
		{
			bResult = FALSE;
		}

		FLTRACE_LOG( PROGRAM_NAME, _T( "ReplaceInspection Level (%d~%d) %d Inspection %d" ), pPortkey->m_uiMinLevel, pPortkey->m_uiMaxLevel, m_nLevel, bResult );
	}
	if( bResult != FALSE && pPortkey->m_iQuest != 0xffffffff )
	{
		LPQUEST pCurQuest = GetQuest( pPortkey->m_iQuest );
		if( pCurQuest && ( pPortkey->m_iQuestFlag == 0 || pPortkey->m_iQuestFlag == pCurQuest->m_nState ) )
			bResult = TRUE;
		else
			bResult = FALSE;
		FLTRACE_LOG( PROGRAM_NAME, _T( "ReplaceInspection Quest %d Flag %d/%d Inspection %d" ), pPortkey->m_iQuest, pPortkey->m_iQuestFlag, pCurQuest->m_nState, bResult );
	}
	if( bResult != FALSE && pPortkey->m_iJob != 0xffffffff )
	{
		if( pPortkey->m_iJob == m_nJob )
			bResult = TRUE;
		else
			bResult = FALSE;
		FLTRACE_LOG( PROGRAM_NAME, _T( "ReplaceInspection Job %d/%d Inspection %d" ), pPortkey->m_iJob, m_nJob, bResult );
	}
	if( bResult != FALSE && pPortkey->m_iGender != 0xffffffff )
	{
		if( pPortkey->m_iGender == GetSex() )
			bResult = TRUE;
		else
			bResult = FALSE;
		FLTRACE_LOG( PROGRAM_NAME, _T( "ReplaceInspection Gender %d/%d Inspection %d" ), pPortkey->m_iGender, GetSex(), bResult );
	}
	if( bResult != FALSE && pPortkey->m_bCheckParty != FALSE )
	{
		CParty* pParty	= g_PartyMng.GetParty( m_idparty );
		if( pParty != NULL && pParty->IsMember( m_idPlayer ) )
			bResult = TRUE;
		else 
			bResult = FALSE;
		FLTRACE_LOG( PROGRAM_NAME, _T( "ReplaceInspection Party Inspection %d" ), bResult );
	}
	if( bResult != FALSE && pPortkey->m_bCheckGuild != FALSE )
	{
		CGuild* pGuild	= g_GuildMng.GetGuild( m_idGuild );
		if( pGuild && pGuild->IsMember( m_idPlayer ) )
			bResult = TRUE;
		else
			bResult = FALSE;
		FLTRACE_LOG( PROGRAM_NAME, _T( "ReplaceInspection Guild Inspection %d" ), bResult );
	}	

	return bResult;
}

// default type =  REPLACE_NORMAL
#ifdef __LAYER_1015
BOOL CMover::Replace( u_long uIdofMulti, DWORD dwWorldID, const D3DXVECTOR3& vPos, REPLACE_TYPE type, int nLayer  )
#else	// __LAYER_1015
BOOL CMover::Replace( u_long uIdofMulti, DWORD dwWorldID, const D3DXVECTOR3& vPos, REPLACE_TYPE type  )
#endif	// __LAYER_1015

{
	CWorld* pWorld	= GetWorld();
	if( !pWorld )
		return FALSE;
	if( dwWorldID == 0 || dwWorldID == NULL_ID )
		return FALSE;

#ifdef __LAYER_1015
	CWorld* pTargetWorld	= g_WorldMng.GetWorld( dwWorldID );
	if( !pTargetWorld || !pTargetWorld->m_linkMap.GetLinkMap( nLayer ) )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "Layer not found Layer: %d" ), nLayer );
		return FALSE;
	}
#endif	// __LAYER_1015

	// ���ǰ˻� 
	if( type == REPLACE_NORMAL )
	{
		if( IsAuthHigher( AUTH_GAMEMASTER ) == FALSE && IsFly() ) 
			return FALSE;
	}

	// �̵��� �����.
	//ClearDest();
	SendActMsg( OBJMSG_ACC_STOP );
	SendActMsg( OBJMSG_STOP_TURN );
	SendActMsg( OBJMSG_STAND );

	//sun: 11, ä�� �ý���
	if( IsCollecting() )
		StopCollecting();

	if( m_vtInfo.GetOtherID() != NULL_ID ) {
		OnTradeRemoveUser();
	}

	//////////////////////////////////////////////////////////////////////////
	// BEGIN110503 �ٸ������ �̵��Ҷ��� �Ⱦ��� ����
// 	if( HasActivatedEatPet() )
//		InactivateEatPet();
	// END110503
	//////////////////////////////////////////////////////////////////////////

	LPREPLACEOBJ lpReplaceObj	= NULL;

	for( int i = 0; i < pWorld->m_cbModifyLink; i++ )
	{
		if( pWorld->m_apModifyLink[i] == this )
		{
			pWorld->m_apModifyLink[i]	= NULL;
			m_vRemoval	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			break;
		}
	}

	for( int i = 0; i < pWorld->m_cbReplaceObj; i++ )
	{
		if( pWorld->m_aReplaceObj[i].pObj == this )
		{
			lpReplaceObj = &pWorld->m_aReplaceObj[i];
			break;
		}
	}
	if( lpReplaceObj == NULL )
	{
		FLASSERT( pWorld->m_cbReplaceObj < MAX_REPLACEOBJ );
		lpReplaceObj = &pWorld->m_aReplaceObj[pWorld->m_cbReplaceObj++];
		lpReplaceObj->pObj	= this;
	}

	//lpReplaceObj->dwOldWorldID = pWorld->GetID();
	lpReplaceObj->dwWorldID	 = dwWorldID;
	lpReplaceObj->vPos       = vPos;
	lpReplaceObj->uIdofMulti = uIdofMulti;
#ifdef __LAYER_1015
	lpReplaceObj->nLayer	= nLayer;
#endif	// __LAYER_1015

	//if( IsPlayer() )
	//{
	//	// �������� �������� �̵��Ѵٸ�??
	//	// �ι�° �˻縦 �ܼ��� pWorld->GetID() != dwWorldID üũ�ϴ°� ���� ��.
	//	if( CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon( pWorld->GetID() )
	//		&& !CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon( dwWorldID ) )
	//	{
	//		//���� �ο����� ���ҽ�Ų��.
	//		CInstanceDungeonHelper::GetInstance()->LeaveDungeon( static_cast<FLWSUser*>( this ), pWorld->GetID() );
	//	}
	//}

	return TRUE;
}

BOOL CMover::CreateItem( FLItemBase* pItemBase, DWORD* pItemObjID, int* pQuantity, DWORD* pCount )
{
	//////////////////////////////////////////////////////////////////////////
	//	BEGIN100708
	if( pItemBase == NULL )
	{
		return	FALSE;
	}
	//	END100708
	//////////////////////////////////////////////////////////////////////////


	if( pItemBase->m_dwItemId == 0 )
	{
		return FALSE;
	}

	FLItemElem* pItemElem = (FLItemElem*)pItemBase;

	//////////////////////////////////////////////////////////////////////////
	//	BEGIN100708
	if( pItemElem == NULL )
	{
		return	FALSE;
	}

	if( pItemElem->m_nItemNum <= 0 )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "m_idPlayer [%d], ItemNum [%d]" ), m_idPlayer, pItemElem->m_nItemNum );

		return	FALSE;
	}

	PT_ITEM_SPEC pItemProp = pItemElem->GetProp();
	if( pItemProp == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "Item Property Invalid. m_idPlayer [%d], ItemId [%d]" ), m_idPlayer, pItemElem->m_dwItemId );
		return FALSE;
	}
	FLINFO_LOG( PROGRAM_NAME, _T( "User CreateItem m_idPlayer : %d / Item ID : %d / NUM : %d / Charge : %d" ), m_idPlayer, pItemElem->m_dwItemId, pItemElem->m_nItemNum, pItemElem->m_bCharged );
	//	END100708
	//////////////////////////////////////////////////////////////////////////

	if( pItemElem->m_nHitPoint <= 0 )
	{
		PT_ITEM_SPEC pItemProp		= pItemElem->GetProp();
		if( pItemProp )
			pItemElem->m_nHitPoint		= ( pItemProp->dwEndurance == -1 ) ? 0 : pItemProp->dwEndurance;//pItemProp->dwEndurance;
		else
			pItemElem->m_nHitPoint		= 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// mirchang_100820 �Ⱓ�� ������ ����..
	if( pItemElem->IsPeriod() == TRUE && pItemElem->m_dwKeepTime == 0 )
	{
		PT_ITEM_SPEC pItemProp = pItemElem->GetProp();
		if( pItemProp != NULL )
		{
			CTime tm	= CTime::GetCurrentTime() + CTimeSpan( 0, 0, pItemProp->dwSkillTime, 0 );
			pItemElem->m_dwKeepTime	= (DWORD)( tm.GetTime() );
		}
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// mirchang_101126 ȹ�� �� �ͼ� ó��
	if( pItemElem->IsFlag( FLItemElem::reset_bind ) && pItemProp->nResetBindCondition == BIND_CONDITION_GET ) {
		pItemElem->SetFlag( FLItemElem::binds );
	}

// 	if( pItemProp->nResetBindCondition == BIND_CONDITION_GET
// 		&& pItemElem->IsFlag( FLItemElem::binds ) == FALSE
// 		&& pItemElem->IsFlag( FLItemElem::reset_bind ) == TRUE )
// 	{
// 		pItemElem->SetFlag( FLItemElem::binds );
// 	}
	//////////////////////////////////////////////////////////////////////////

	BOOL fSuccess	= FALSE;
	DWORD dwCount	= 0;

	/*if( pItemBase->GetSerialNumber() == 0 )
		pItemBase->SetSerialNumber();*/

	DWORD adwItemObjID[MAX_INVENTORY] = { NULL_ID, };
	int anNum[MAX_INVENTORY] = { 0, };

	fSuccess	= m_Inventory.Add( (FLItemElem*)pItemBase, adwItemObjID, anNum, &dwCount );

	if( fSuccess && IsPlayer() )
	{
		for( DWORD i = 0; i < dwCount; ++i )
		{
			FLItemBase * pCreateItem = m_Inventory.GetAtId( adwItemObjID[i] );
			if( pCreateItem != NULL )
			{
				if( pCreateItem->GetSerialNumber() == 0 )
					pCreateItem->SetSerialNumber();
				( (FLWSUser*)this )->AddCreateItem( pCreateItem, adwItemObjID[i], anNum[i] );
			}
		}
	}
	if( pItemObjID != NULL )
	{
		memcpy( pItemObjID, adwItemObjID, sizeof(DWORD) * dwCount );
	}
	if( pQuantity != NULL )
	{
		memcpy( pQuantity, anNum, sizeof(int) * dwCount );
	}
	if( pCount != NULL )
	{
		*pCount	= dwCount;
	}

	return fSuccess;
}

void CMover::RemoveItem( DWORD dwItemObjID, int nNum )
{
	FLItemBase* pItemBase	= GetItemId( dwItemObjID );

	if( pItemBase != NULL )
	{
		//////////////////////////////////////////////////////////////////////////
		//	BEGIN100708
		if( nNum <= 0 || nNum > (( (FLItemElem*)pItemBase )->m_nItemNum ) )
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "m_idPlayer [%d], ItemNumber [%d]" ), m_idPlayer, nNum );

			return;
		}
		//	END100708
		//////////////////////////////////////////////////////////////////////////

		UpdateItem( dwItemObjID, UI_NUM, ( (FLItemElem*)pItemBase )->m_nItemNum - nNum );
	}
}

// ���ϰ��� 0���� Ŭ��� �� ������ ���� ���..
int		CMover::RemoveItemByItemID( const DWORD dwItemID, const int nRemoveQuantity, const TCHAR* pszLogAction, const TCHAR* pszLogContext )
{
	int nRemainQuantity	= nRemoveQuantity;

	if( nRemoveQuantity <= 0 )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID REMOVE COUNT. PlayerID:(%07d), ItemID:(%d), Remove:(%d) ]" )
			, m_idPlayer, dwItemID, nRemoveQuantity );
		return nRemainQuantity;
	}

	const int nHaveQuantity	= m_Inventory.GetItemNumByItemId( dwItemID );
	if( nHaveQuantity < nRemoveQuantity )
	{
		return nRemainQuantity;
	}

	const PT_ITEM_SPEC pItemProp	= g_xSpecManager->GetSpecItem( dwItemID );
	if( pItemProp == NULL )
	{
		return nRemainQuantity;
	}

	DWORD dwStart	= 0;
	DWORD dwEnd		= 0;
	if( m_Inventory.GetObjIndexRange( pItemProp, &dwStart, &dwEnd ) == false )
	{
		return nRemainQuantity;
	}

	for( DWORD Nth = dwStart; Nth < dwEnd && nRemainQuantity > 0; ++Nth )
	{
		FLItemElem* pItemElem	= m_Inventory.GetAt( Nth );
		if( pItemElem == NULL || pItemElem->m_dwItemId != dwItemID )
		{
			continue;
		}

		const int nRemove	= pItemElem->m_nItemNum < nRemainQuantity ? pItemElem->m_nItemNum : nRemainQuantity;

#ifdef __WORLDSERVER
		if( IsPlayer() == TRUE && pszLogAction != NULL && pszLogContext != NULL )
		{
			g_DPSrvr.PutItemLog( static_cast<FLWSUser*>( this ), pszLogAction, pszLogContext, pItemElem, nRemove );
		}
#endif // __WORLDSERVER

		RemoveItem( pItemElem->m_dwObjId, nRemove );

		nRemainQuantity	-= nRemove;
	}

	const int nRemainHaveQuantity	= m_Inventory.GetItemNumByItemId( dwItemID );
	if( ( nHaveQuantity - nRemoveQuantity ) != nRemainHaveQuantity )
	{
		if( IsPlayer() == TRUE )
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "[ CRITICAL ERROR. INVALID REMOVE COUNT. PlayerID:(%07d), ItemID:(%d), Have:(%d), Remove:(%d), Remain:(%d) ]" )
				, m_idPlayer, dwItemID, nHaveQuantity, nRemoveQuantity, nRemainHaveQuantity );
		}
	}

	return nRemainQuantity;
}

// IK3�� �����ϱ� ��) IK3_CLOAK
void CMover::RemoveItemIK3( DWORD dwItemKind3 )
{
	DWORD dwSize = m_Inventory.GetMax();
	for( DWORD dwItemObjID = 0 ; dwItemObjID < dwSize; ++dwItemObjID )
	{
		FLItemElem* pItemElem = m_Inventory.GetAtId( dwItemObjID );
		if( IsUsableItem( pItemElem ) && pItemElem->GetProp()->dwItemKind3 == dwItemKind3 )
		{
			if( pItemElem->GetProp()->dwItemKind3 == IK3_CLOAK && pItemElem->m_idGuild == 0 )
				continue;

			if( m_Inventory.IsEquip( pItemElem->m_dwObjId ) )		
			{
				if( DoEquip( pItemElem, FALSE ) )
				{
					g_xWSUserManager->AddDoEquip( this, -1, pItemElem, FALSE );
					UpdateItem( pItemElem->m_dwObjId, UI_NUM, 0 );		// remove
				}
				else
				{
					// �κ� full�̰�, ���������̸� ������ �� ����. 
					// FLERROR_LOG( PROGRAM_NAME, _T( "RemoveItemIK3->DoEquip : User = %s, Item = %d %d %d" ), 
					// GetName(), pItemElem->m_dwObjId, pItemElem->GetProp()->dwItemKind3, dwItemKind3 );
				}
			}
			else
			{
				UpdateItem( pItemElem->m_dwObjId, UI_NUM, 0 );		// remove
			}
		}
	}
}

void CMover::RemoveVendorItem( CHAR chTab, DWORD dwItemObjID, int nNum )
{
	int nTmpNum;
	FLItemElem* pItemElem = m_ShopInventory[chTab]->GetAtId( dwItemObjID );

	if( pItemElem ) 
	{
		if( ( nTmpNum = pItemElem->m_nItemNum - nNum ) > 0 )
			pItemElem->m_nItemNum	= nTmpNum;
		else
			m_ShopInventory[chTab]->RemoveAtId( dwItemObjID );
		g_xWSUserManager->AddUpdateVendor( this, chTab, dwItemObjID, nTmpNum );
	}
}
#endif	// __WORLDSERVER

void CMover::UpdateItemBank( int nSlot, DWORD dwItemObjID, CHAR cParam, DWORD dwValue )
{
	FLItemBase* pItemBase = GetItemBankId( nSlot, dwItemObjID );

	if( pItemBase )
	{
		switch( cParam )
		{
		case UI_NUM:
			{
				if( dwValue == 0 ) 
				{
					RemoveItemBankId( nSlot, dwItemObjID );
				}
				else
				{
					//////////////////////////////////////////////////////////////////////////
					//	BEGIN100708
					PT_ITEM_SPEC	pItemPorp	= pItemBase->GetProp();

					if( pItemPorp != NULL)
					{
						if( dwValue > pItemPorp->dwPackMax )
						{
							FLERROR_LOG( PROGRAM_NAME, _T( "UI_NUM Error. m_idPlayer [%d], Serial[%d], nSlot [%d], nId [%d], OBJID[%d], ItemID[%d]" ),
								m_idPlayer, pItemBase->GetSerialNumber(), nSlot, dwItemObjID, pItemBase->m_dwObjId, pItemBase->m_dwItemId );

							RemoveItemBankId( nSlot, dwItemObjID );
						}
						else
						{
							( (FLItemElem*)pItemBase )->m_nItemNum	= (int)( dwValue );
						}
					}
					else
					{
						FLERROR_LOG( PROGRAM_NAME, _T( "UI_NUM Error2. m_idPlayer [%d], Serial[%d], nSlot [%d], nId [%d], OBJID[%d], ItemID[%d]" ),
							m_idPlayer, pItemBase->GetSerialNumber(), nSlot, dwItemObjID, pItemBase->m_dwObjId, pItemBase->m_dwItemId );

					}
					//	END100708
					//////////////////////////////////////////////////////////////////////////
				}
				break;
			}
		default:
			break;
		}
	}
#ifdef __WORLDSERVER
	if( IsPlayer() )
		( (FLWSUser*)this )->AddUpdateBankItem( static_cast< BYTE >( nSlot ), dwItemObjID, cParam, dwValue );
#endif
}

#ifdef __WORLDSERVER
// �κ��丮������ 1�� �������� ����Ʈ����.
int	CMover::DoDropItemRandom( BOOL bExcludeEquip, CMover* pAttacker, BOOL bOnlyEquip )
{
	int nRealMax, nDropCnt;
	FLItemElem* pItemElem;
	FLItemElem *pElemBuff[ MAX_INVENTORY + MAX_EQUIPMENT ] = { NULL, };

	nRealMax = nDropCnt = 0;

	DWORD dwMax = m_Inventory.GetMax();
	for( DWORD dwObjIndex = 0; dwObjIndex < dwMax; ++dwObjIndex ) 		// �κ� ������ŭ ���鼭 �������� �ִ�ĭ�� �����͸� �̾Ƴ��´�.
	{
		pItemElem = m_Inventory.GetAt( dwObjIndex );
		if( pItemElem == NULL )
			continue;

		if( pItemElem->IsQuest() )					// ����Ʈ ������ ���� 
			continue;

		if( pItemElem->IsCharged() )	// ���� ��ǰ������ ����
			continue;

		//sun: 8, // __S8_PK
		if( pItemElem->IsEatPet()
			|| IsUsing( pItemElem )	)	//sun: 9, 9-10�� ��
			continue;

		if( pItemElem->IsOwnState() )
			continue;

		if( bOnlyEquip )
		{
			if( m_Inventory.IsEquip( pItemElem->m_dwObjId ) )		// ������ ������ ����  
				pElemBuff[ nRealMax++ ] = pItemElem;
		}
		else
		{
			if( bExcludeEquip && m_Inventory.IsEquip( pItemElem->m_dwObjId ) )		// ������ ������ ����  
				continue;

			pElemBuff[ nRealMax++ ] = pItemElem;
		}
	}

	while( nRealMax )	// �˻���� �������� ���̻� ������� ��.
	{
		int nIdx = xRandom( nRealMax );
		pItemElem = pElemBuff[ nIdx ];		// ������ ����Ʈ���� �������� �ϳ��� �̾ƿ�

		int nPartBuf = -1;
		//sun: 8, // __S8_PK
		FLItemElem* pItemElemLWepon = m_Inventory.GetEquip( PARTS_LWEAPON );
		FLItemElem* pItemElemRWepon = m_Inventory.GetEquip( PARTS_RWEAPON );
		if( pItemElem == pItemElemLWepon )
			nPartBuf = PARTS_LWEAPON;
		else if( pItemElem == pItemElemRWepon )
			nPartBuf = PARTS_RWEAPON; 

		if( m_Inventory.IsEquip( pItemElem->m_dwObjId ) )	// �����ϰ� ������ ���� 			
		{
			if( DoEquip( pItemElem, FALSE, nPartBuf ) )
				g_xWSUserManager->AddDoEquip( this, nPartBuf, pItemElem, FALSE );
		}

		CItem* pItem = DropItem( pItemElem->m_dwObjId, 0, GetPos(), TRUE );
		if( pItem )	
		{
			UNUSED_ALWAYS( pAttacker );
			return 1;
		}

		// drop�� ������ �������̶� ����Ʈ���� ������.
		for( int j = nIdx; j < nRealMax-1; j ++ )		// ������ ��ĭ�� ����.
			pElemBuff[j] = pElemBuff[j+1];
		nRealMax --;	// �ϳ��� ����Ʈ������ ������ ũ�⸦ �ϳ� ����
	}		

	return 0;
}
#endif // __WORLDSERVER

CItem *CMover::_DropItemNPC( DWORD /*dwItemType*/, DWORD dwID, int nDropNum, const D3DXVECTOR3 &vPos )
{
#ifdef __WORLDSERVER
	if( IsInvalidObj(this) )	
		return NULL;

	CWorld *pWorld = GetWorld();
	if( pWorld == NULL )	
		return NULL;	
	FLItemBase* pItemBase = GetItemId( dwID );
	if( NULL == pItemBase )		
		return NULL;

	int nNum	= 1;
	nNum = ((FLItemElem*)pItemBase)->m_nItemNum;
	if( m_Inventory.IsEquip( pItemBase->m_dwObjId ) ) 
		return NULL;

	if( nDropNum == 0 )
		nDropNum = nNum;
	else if( nNum - nDropNum < 0 )
		return NULL;

	CItem* pItem	                             = new CItem;
	pItem->m_pItemBase	                         = new FLItemElem;
	*((FLItemElem*)pItem->m_pItemBase)            = *((FLItemElem*)pItemBase);
	((FLItemElem*)pItem->m_pItemBase)->m_nItemNum = nDropNum;

	if( pItemBase->m_dwItemId == 0 ) 
		FLERROR_LOG( PROGRAM_NAME, _T( "SetIndex: %s" ), GetName() ); 

	pItem->SetIndex( D3DDEVICE, pItemBase->m_dwItemId );
	if( pItem->m_pModel )
	{
		D3DXVECTOR3 v = vPos;
		v.y += ( pItem->m_pModel->m_vMax.y - pItem->m_pModel->m_vMin.y ) / 2.0f;
		pItem->SetPos( v );
		pItem->SetAngle( (float)( xRandom( 360 ) ) );
		pItem->m_dwDropTime = timeGetTime();	// ����Ʈ�������� �ð�.
		if( pWorld->ADDOBJ( (CObj*)pItem, TRUE, GetLayer() ) == TRUE )
		{
			RemoveItem( dwID, nDropNum );
			return pItem;
		}
		else
		{
			SAFE_DELETE( pItem );
		}
	}
	else
	{
		SAFE_DELETE( pItem );
	}
#endif // WORLDSERVER
	return NULL;
}

// �������� ���� �� �ִ°�? PK - PK�� ���� ���� TRUE
BOOL CMover::IsDropableState( BOOL bPK )
{
	if( IsInvalidObj(this) )	
		return FALSE;

#ifdef __WORLDSERVER
	CWorld *pWorld = GetWorld();
	if( pWorld == NULL )	
		return FALSE;

	if( m_pActMover->m_bGround == 0 || m_pActMover->IsFly() )	// �����ִ� ���̰ų� ���߿� ���ִ»��¿��� �ϴ� ������Ʈ���� ����.
	{
		if( bPK == FALSE ) //  PK���� ��찡 �ƴҰ�츸 ������.
		{
			((FLWSUser*)this)->AddDefinedText( TID_GAME_FLIGHTDROP, "" );
			return FALSE;
		}
	}

	if( m_vtInfo.GetOtherID() != NULL_ID )		// Ʈ���̵����϶� �ϴ� ������Ʈ���� ����
	{
		((FLWSUser*)this)->AddDefinedText( TID_GAME_TRADELIMITITEM, "" );
		return FALSE;
	}

	if( m_vtInfo.VendorIsVendor() )
		return FALSE;

	if( IsMode( ITEM_MODE ) )
		return FALSE;

#endif // __WORLDSERVER
	return TRUE;
}	


// �κ��� �������� �ٴڿ� ���������� ����Ѵ�.
// dwType, dwID�� �������� vPos�� ����Ʈ����.
// bPK == YES�� �÷��̾ PK���ؼ� ����Ʈ�� �������̴�.
CItem* CMover::DropItem( DWORD dwID, int nDropNum, const D3DXVECTOR3 &vPos, BOOL bPK )
{
#ifdef __WORLDSERVER
	if( IsPlayer() == FALSE )	
		return _DropItemNPC( 0, dwID, nDropNum, vPos );	// ���� ����߸��� �̰ɻ��.

	FLItemBase* pItemBase = GetItemId( dwID );

	if( IsDropable( (FLItemElem*)pItemBase, bPK ) == FALSE )
		return NULL;

	PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( pItemBase->m_dwItemId );	// 2016.04.22
	if ( pItemProp && pItemProp->bCharged == TRUE )
		return NULL;

	int nNum	= ((FLItemElem*)pItemBase)->m_nItemNum;

	if( nDropNum == 0 )
	{
		nDropNum = nNum;
	}

	//////////////////////////////////////////////////////////////////////////
	//	BEGIN100708
	if( nDropNum < 0 || nDropNum > nNum )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "m_idPlayer [%d], ItemNumber[%d]" ), m_idPlayer, nDropNum );

		return	NULL;
	}
	//	END100708
	//////////////////////////////////////////////////////////////////////////

	CItem* pItem	= new CItem;
	pItem->m_pItemBase	= new FLItemElem;
	*( (FLItemElem*)pItem->m_pItemBase )		= *( (FLItemElem*)pItemBase );
	( (FLItemElem*)pItem->m_pItemBase )->m_nItemNum		= nDropNum;

// 	if( nDropNum < nNum )
// 	{
// 		pItem->m_pItemBase->SetSerialNumber();
// 	}

	if( pItemBase->IsQuest() )
		pItem->m_idHolder	= m_idPlayer;

	if( ( (FLItemElem*)pItemBase )->IsLogable() )
	{
		LogItemInfo aLogItem;
		//aLogItem.Action = "D";
		//aLogItem.SendName = GetName();
		//aLogItem.RecvName = "GROUND";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "D" );
		FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), GetName() );
		FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "GROUND" );
		aLogItem.WorldId = GetWorld()->GetID();
		aLogItem.Gold = aLogItem.Gold2 = GetGold();
		g_DPSrvr.OnLogItem( aLogItem, ( (FLItemElem*)pItemBase ), nDropNum );
	}

	pItem->SetIndex( D3DDEVICE, pItemBase->m_dwItemId );

	if( pItem->m_pModel )
	{
		D3DXVECTOR3 v = vPos;
		v.y += ( pItem->m_pModel->m_vMax.y - pItem->m_pModel->m_vMin.y ) / 2.0f;
		pItem->SetPos( v );
		pItem->SetAngle( (float)( xRandom( 360 ) ) );
		pItem->m_dwDropTime = timeGetTime();	// ����Ʈ�������� �ð�.
		UNUSED_ALWAYS( bPK );
		if( GetWorld()->ADDOBJ( (CObj*)pItem, TRUE, GetLayer() ) == TRUE )
		{
			RemoveItem( dwID, nDropNum );
			return pItem;
		}
		else
		{
			SAFE_DELETE( pItem );
		}
	}
	else
	{
		SAFE_DELETE( pItem );
	}

#endif // WORLDSERVER
	return NULL;
}

// TODO_OPTIMIZE: ��ǥ�� ����� �� ȣ��ǰ��Ѵ�. ( rect�� Ʈ�������� ���� ã�� �ϴ� �͵� ���ڴ�.)
REGIONELEM* CMover::UpdateRegionAttr()
{
	REGIONELEM* pPortkey = NULL;
	const DWORD dwCheck = ( RA_SAFETY | RA_PENALTY_PK | RA_PK | RA_FIGHT ) ;

	D3DXVECTOR3 vPos = GetPos();
	POINT pt = { (LONG)( vPos.x ), (LONG)( vPos.z ) };
	LPREGIONELEM lpRegionElem;
	int nSize = GetWorld()->m_aRegion.GetSize();
	DWORD	dwRegionAttr	= 0;
	for( int i = 0; i < nSize; i++ )
	{
		lpRegionElem = GetWorld()->m_aRegion.GetAt( i );
		if( lpRegionElem && lpRegionElem->m_rect.PtInRect( pt ) )
		{
			dwRegionAttr	|= lpRegionElem->m_dwAttribute;
			if( lpRegionElem->m_dwIdTeleWorld != WI_WORLD_NONE )
				pPortkey = lpRegionElem;
		}
	}

	if( dwRegionAttr & dwCheck )
	{
		m_dwOldRegionAttr	= m_dwRegionAttr;
		m_dwRegionAttr	= dwRegionAttr;
	}
	else
	{
		m_dwOldRegionAttr	= m_dwRegionAttr;
		m_dwRegionAttr	= GetWorld()->m_nPKMode;
	}

	return pPortkey;
}

//
//REGIONELEM* CMover::UpdateRegion_ToIndun( const DWORD dwWorldID )
//{
//	REGIONELEM* pPortkey = NULL;
//	const DWORD dwCheck = ( RA_SAFETY | RA_PENALTY_PK | RA_PK | RA_FIGHT ) ;
//
//	//D3DXVECTOR3 vPos = GetPos();
//	//POINT pt = { (LONG)( vPos.x ), (LONG)( vPos.z ) };
//	LPREGIONELEM lpRegionElem;
//	int nSize = GetWorld()->m_aRegion.GetSize();
//	DWORD	dwRegionAttr	= 0;
//	for( int i = 0; i < nSize; i++ )
//	{
//		lpRegionElem = GetWorld()->m_aRegion.GetAt( i );
//
//		dwRegionAttr	|= lpRegionElem->m_dwAttribute;
//		if( lpRegionElem->m_dwIdTeleWorld != WI_WORLD_NONE )
//			pPortkey = lpRegionElem;
//	}
//
//	if( dwRegionAttr & dwCheck )
//	{
//		m_dwOldRegionAttr	= m_dwRegionAttr;
//		m_dwRegionAttr	= dwRegionAttr;
//	}
//	else
//	{
//		m_dwOldRegionAttr	= m_dwRegionAttr;
//		m_dwRegionAttr	= GetWorld()->m_nPKMode;
//	}
//
//	SetPosChanged( FALSE );
//	return pPortkey;
//}

//�ٸ� �������� �̵��ߴ��� �˻� ( ��ȭ ����, ���Ƽ ������ ����, ���� ������ ���� )
BOOL CMover::IsRegionMove( DWORD dwOldAttr, DWORD dwRegionAttr )
{
	if( ( dwOldAttr & RA_SAFETY ) == RA_SAFETY ) 
	{
		if( ( dwRegionAttr & RA_SAFETY ) != RA_SAFETY )
		{
			return TRUE;
		}
	}
	else if( ( dwOldAttr & RA_PENALTY_PK ) == RA_PENALTY_PK ) 
	{
		if( ( dwRegionAttr & RA_PENALTY_PK ) != RA_PENALTY_PK )
		{
			return TRUE;
		}
	}
	else if( ( dwOldAttr & RA_PK ) == RA_PK ) 
	{
		if( ( dwRegionAttr & RA_PK ) != RA_PK )
		{
			return TRUE;
		}
	}
	else if( ( dwOldAttr & RA_FIGHT ) == RA_FIGHT )
	{
		if( ( dwRegionAttr & RA_FIGHT ) != RA_FIGHT )
		{
			return TRUE;
		}
	}

	return FALSE;
}

DWORD	CMover::GetPKPVPRegionAttr()
{
	if( IsRegionAttr( RA_FIGHT ) )
		return RA_FIGHT;
	else if( IsRegionAttr( RA_PENALTY_PK ) )
		return RA_PENALTY_PK;
	else if( IsRegionAttr( RA_PK ) )
		return RA_PK;
	else
		return RA_SAFETY;
}

void CMover::SetStateMode( DWORD dwMode, BYTE nFlag )
{
	m_dwStateMode |= dwMode;
#ifdef __WORLDSERVER
	g_xWSUserManager->AddStateMode( ((FLWSUser*)this), nFlag );
#endif // __WORLDSERVER
}

void CMover::SetStateNotMode( DWORD dwMode, BYTE nFlag )
{
	m_dwStateMode &= (~dwMode);
#ifdef __WORLDSERVER
	g_xWSUserManager->AddStateMode( ((FLWSUser*)this), nFlag );
#endif // __WORLDSERVER
} 

BOOL CMover::IsUseItemReadyTime( PT_ITEM_SPEC pItemProp, OBJID dwObjItemId )
{
	if( m_Inventory.IsEquip( dwObjItemId ) )
	{		
		return TRUE;
	}

	if( pItemProp->dwParts == PARTS_RIDE )
	{
		CWorld* pWorld = GetWorld();
		if( pWorld == NULL )
			return FALSE;

		int nLimitLv = pItemProp->dwFlightLimit;
		if( nLimitLv == NULL_ID )
			nLimitLv = 1;
		//sun: all, __NOLIMIT_RIDE_ITEM
		if( pItemProp->dwID == ITEM_INDEX( 26514, II_RID_RID_STI_MAGIC01 ) )
			nLimitLv = 0;

		if( GetFlightLv() < nLimitLv ) // ���෹���� �ȵǸ� ��ź��.
		{
			PrintString( this, TID_GAME_USEAIRCRAFT );	// ���־�� ����Ҽ� �ֽ��ϴ�
			return FALSE;
		}

		int nAttr = pWorld->GetHeightAttribute( GetPos().x, GetPos().z );
		if( !pWorld->m_bFly || nAttr == HATTR_NOFLY ) // ������������Դϴ�.
		{
			PrintString( this, TID_ERROR_NOFLY );
			return FALSE;
		}
	}

	if( pItemProp->dwSkillReadyType != 0 && 
		pItemProp->dwSkillReadyType != 0xffffffff )
	{
		if( m_vtInfo.IsVendorOpen() )	// ���λ��� �߿��� ��ź��
		{
			return FALSE;
		}
		else if( IsStateMode( STATE_BASEMOTION_MODE ) )	// �̹� �������̸� �޼��� ó��
		{
			PrintString( this, TID_PK_BLINK_LIMIT );	// �����߿��� ����Ҽ� �����ϴ�
			return FALSE;		
		}
		else if( m_pActMover->IsState( OBJSTA_STAND ) == FALSE )	// ���ִ� ���°� �ƴϸ� �޼��� ó��
		{
			PrintString( this, TID_PK_STAND_LIMIT );	// ���־�� ����Ҽ� �ֽ��ϴ�
			return FALSE;		
		}
		else if( IsFly() )
		{
#ifdef __CLIENT
			g_WndMng.PutString( prj.GetText( TID_PK_FLIGHT_NOUSE ), NULL, prj.GetTextColor( TID_PK_FLIGHT_NOUSE ) );
#endif // __CLINET
			return FALSE;
		}
	}
	return TRUE;
}

void CMover::ProcessRegion()
{
	if( FALSE == IsPlayer() )
		return;

	REGIONELEM* pPortkey = NULL;
	if( IsPosChanged() )
	{
		pPortkey = UpdateRegionAttr();			//���� ����� �ڷ���Ʈ ��ġ�� ã�´�.
		SetPosChanged( FALSE );
	}

#ifdef __WORLDSERVER

	if( pPortkey )
	{
		if( IsFly() )	// ReplaceInspection() �� �ִ� ���� ������ �����Դ�.
			return;

		if(	CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon( pPortkey->m_dwIdTeleWorld ) )
		{
			// �� ���� �δ��� �ƴ϶��
			if(	!CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon( static_cast<FLWSUser*>( this )->GetWorld()->GetID() ) ) 
			{
				DWORD eDungeonLevel		= 0;		//@@@@@@@@@@�̹� �δ��� �����Ǿ��ٴ� �Ǵ�.
				DWORD dwGuildID			= 0;
				
				const BOOL bRet			= CInstanceDungeonHelper::GetInstance()->EnteranceDungeon( static_cast<FLWSUser*>( this ), pPortkey->m_dwIdTeleWorld, dwGuildID, eDungeonLevel );
				if( bRet == FALSE )
				{
					if( IsPlayer() == TRUE && GetWorld() != NULL )
					{
						float fPushPower = 0.5f;
						AngleToVectorXZ( &m_pActMover->m_vDeltaE, GetAngle() + 180.0f, fPushPower );
						this->Replace( g_uIdofMulti, GetWorld()->GetID(), GetPos() + m_pActMover->m_vDeltaE, REPLACE_NORMAL, GetLayer() );
						g_xWSUserManager->AddPushPower( this, GetPos(), GetAngle(), GetAngle() + 180.0f, fPushPower );
					}
				}
				//if( CInstanceDungeonHelper::GetInstance()->GetDungeonLevel( pPortkey->m_dwIdTeleWorld, static_cast<FLWSUser*>( this )->GetLayer(), eDungeonLevel ) == TRUE )
                //    CInstanceDungeonHelper::GetInstance()->EnteranceDungeon( static_cast<FLWSUser*>( this ), pPortkey->m_dwIdTeleWorld, eDungeonLevel );
			}
			else
				REPLACE( g_uIdofMulti, pPortkey->m_dwIdTeleWorld, pPortkey->m_vTeleWorld, REPLACE_NORMAL, static_cast<FLWSUser*>( this )->GetLayer() );

			return;
		}

		if( ReplaceInspection( pPortkey ) == FALSE )
			return;

		REPLACE( g_uIdofMulti, pPortkey->m_dwIdTeleWorld, pPortkey->m_vTeleWorld, REPLACE_NORMAL, nTempLayer );
	}
#endif	// __WORLDSERVER
}

BOOL CMover::Read( CFileIO* pFile )
{
	CObj::Read( pFile );

	char szDialogFile[MAX_DIALOGFILENAME];
	DWORD dwExtraFlag = 0;

	pFile->Read( m_szName, sizeof( m_szName ) );
	pFile->Read( szDialogFile, sizeof( szDialogFile ) );
	pFile->Read( m_szCharacterKey, sizeof( m_szCharacterKey ) );	
	pFile->Read( &m_dwBelligerence, sizeof( m_dwBelligerence ) );
	pFile->Read( &dwExtraFlag, sizeof( dwExtraFlag ) );

	UpdateParam();

#if defined(__WORLDSERVER)	
	InitCharacter( GetCharacter() );
#endif

	return TRUE;
}

FLItemElem* CMover::GetLWeaponItem()
{
	return GetWeaponItem( PARTS_LWEAPON );
}

FLItemElem* CMover::GetWeaponItem( int nParts )
{
	if( IsPlayer() )
		return (FLItemElem*)m_Inventory.GetEquip( nParts );
	return NULL;
}

PT_ITEM_SPEC CMover::GetActiveHandItemProp( int nParts )
{
	if( IsPlayer() )
	{
#ifdef __CLIENT
		if( IsActiveMover() )
		{
			FLItemElem* pItemElem = (FLItemElem*)m_Inventory.GetEquip( nParts );
			if( pItemElem )
			{
				return pItemElem->GetProp();
			}
			return g_xSpecManager->GetSpecItem( ITEM_INDEX( 11, II_WEA_HAN_HAND ) );
		}
		else	// Ŭ���̾�Ʈ������ Ÿ ĳ���ʹ� m_adwEquipment�� dwObjId�� �ƴ� dwItemId�� ������ �ִ�.
		{
			DWORD dwWeaponId	= m_aEquipInfo[nParts].dwItemID;
			if( dwWeaponId != 0 )
			{
				return g_xSpecManager->GetSpecItem( dwWeaponId );
			}
			return g_xSpecManager->GetSpecItem( ITEM_INDEX( 11, II_WEA_HAN_HAND ) );
		}
#else	// __CLIENT
		FLItemElem* pItemElem = (FLItemElem*)m_Inventory.GetEquip( nParts ); 
		if( pItemElem )
		{
			return pItemElem->GetProp();
		}
		return g_xSpecManager->GetSpecItem( ITEM_INDEX( 11, II_WEA_HAN_HAND ) );
#endif	// __CLIENT
	}

	// virt�� NPC���� ����ϴ� ���� ������ �Ǵ� �÷��̾ ���⸦ �������� �ʾ��� �� ����� �������̴�. 
	// �̰��� �����Ͽ� ���� ����� �������� �����Ѵ�.
	// �� �������� �ַ� ���⳪, ��ų�� �ǹ��� ���̸�, �ַ� ���ݿ����� ���� ���̴�.
	if( m_dwVirtItem != NULL_ID )
	{
		if( m_dwVirtType == VT_ITEM )
		{
			return g_xSpecManager->GetSpecItem( m_dwVirtItem );
		}
		if( m_dwVirtType == VT_SKILL )
		{
			//return prj.GetSkillProp( m_dwVirtItem );
		}
	}
	return NULL;
}
PT_ITEM_SPEC CMover::GetTransyItem( PT_ITEM_SPEC pItemProp, BOOL bCheck, LPCTSTR lpszFileName )
{
	PT_ITEM_SPEC pItemPropChange = NULL;
	int nCount = 0;
	BOOL bSetIteFirst = FALSE;

	if( pItemProp && ( pItemProp->dwItemKind2 == IK2_ARMOR || pItemProp->dwItemKind2 == IK2_ARMORETC ) 
		&& ( pItemProp->dwItemSex == SEX_MALE || pItemProp->dwItemSex == SEX_FEMALE ) )
	{
		for( SpecItemIdItr it = g_xSpecManager->m_SpecItemIdMap.begin(); it != g_xSpecManager->m_SpecItemIdMap.end(); ++it )
		{
			BOOL bSetIteSecond = FALSE;

			PT_ITEM_SPEC ptItemSpecOther = &it->second;

			if( ptItemSpecOther && pItemProp->dwID != ptItemSpecOther->dwID 
				&& ( ptItemSpecOther->dwItemKind2 == IK2_ARMOR || ptItemSpecOther->dwItemKind2 == IK2_ARMORETC )
				&& ( ptItemSpecOther->dwItemSex == SEX_MALE || ptItemSpecOther->dwItemSex == SEX_FEMALE )
				&& pItemProp->dwItemSex != ptItemSpecOther->dwItemSex )
			{
				if( pItemProp->dwItemKind1 == ptItemSpecOther->dwItemKind1 
					&& pItemProp->dwItemKind2 == ptItemSpecOther->dwItemKind2 
					&& pItemProp->dwItemKind3 == ptItemSpecOther->dwItemKind3
					&& pItemProp->dwItemJob == ptItemSpecOther->dwItemJob 
					&& pItemProp->dwHanded == ptItemSpecOther->dwHanded 
					&& pItemProp->dwParts == ptItemSpecOther->dwParts  
					&& pItemProp->dwItemLV == ptItemSpecOther->dwItemLV 
					&& pItemProp->dwAbilityMin == ptItemSpecOther->dwAbilityMin 
					&& pItemProp->dwAbilityMax == ptItemSpecOther->dwAbilityMax 
					&& pItemProp->fAttackSpeed == ptItemSpecOther->fAttackSpeed
					)
				{
					if( CSetItemFinder::GetInstance()->GetSetItemByItemId( pItemProp->dwID ) )
						bSetIteFirst = TRUE;

					if( CSetItemFinder::GetInstance()->GetSetItemByItemId( ptItemSpecOther->dwID ) )
						bSetIteSecond = TRUE;

					// ���� ���� ����? ( ��Ʈ? �Ϲ�? )
					if( bSetIteFirst == bSetIteSecond )
					{
						pItemPropChange = ptItemSpecOther;
						++nCount;

						if( bCheck )
						{
							if( 0 < nCount )
								FLINFO_LOG( lpszFileName, _T( "%s -> %s (%d)" ), pItemProp->szName, ptItemSpecOther->szName, nCount );
							else
								FLINFO_LOG( lpszFileName, _T( "%s -> %s" ), pItemProp->szName, ptItemSpecOther->szName );
						}
						else
						{
							break;
						}
					}
				}
			}
		}
		if( bCheck && nCount == 0 )
		{
			FLINFO_LOG( lpszFileName, _T( "%s -> %s" ), pItemProp->szName, _T( "NULL" ) );
		}
	}

	return pItemPropChange;
}

FLItemElem* CMover::GetEquipItem( int nParts )
{
	return (FLItemElem*)m_Inventory.GetEquip( nParts );
}

PT_ITEM_SPEC CMover::GetEquipItemProp( CItemContainer<FLItemElem>* pInventory, PEQUIP_INFO pEquipInfo, int nParts )
{
	PT_ITEM_SPEC pItemProp = NULL;
	if( pInventory == NULL  )
	{
		if( pEquipInfo[nParts].dwItemID != 0 )
		{
			pItemProp = g_xSpecManager->GetSpecItem( pEquipInfo[nParts].dwItemID );
		}
	} else
	{
		FLItemElem *pItemElem = pInventory->GetEquip( nParts );		
		if( pItemElem )
		{
			pItemProp = pItemElem->GetProp();
		}
	}

	return pItemProp;
}

// ��Į��� �ΰ�.
// ��տ� ��Į�� ��� �ֳ�.
BOOL CMover::IsDualWeapon()
{
#ifdef __CLIENT
	if( IsActiveMover() )
#else
	if( IsPlayer() )
#endif
	{
		FLItemElem *pItemElemR = m_Inventory.GetEquip( PARTS_RWEAPON );
		FLItemElem *pItemElemL = m_Inventory.GetEquip( PARTS_LWEAPON );
		if( pItemElemR && pItemElemL )
			return TRUE;
		else
			return FALSE;
	} 

	if( m_aEquipInfo[PARTS_RWEAPON].dwItemID != 0 && m_aEquipInfo[PARTS_LWEAPON].dwItemID != 0 )
		return TRUE;
	else
		return FALSE;
}


void CMover::ProcessAniSpeed()
{
//	static DWORD i = 0;
//	++i;

//	if( IsPlayer() )
	{
		switch( m_pActMover->GetState() & OBJSTA_ATK_ALL )
		{
		case OBJSTA_ATK1:
		case OBJSTA_ATK2:
		case OBJSTA_ATK3:
		case OBJSTA_ATK4:
		case OBJSTA_ATK5:
		case OBJSTA_ATK_MAGIC1:
		case OBJSTA_ATK_RANGE1:
			m_fAniSpeed = GetAttackSpeed();
			break;
		case OBJSTA_ATK_CASTING1:
		case OBJSTA_ATK_CASTING2:
		case OBJSTA_ATK_MAGICSKILL:
			m_fAniSpeed	= GetCastingAniSpeed();
			break;
		default:
			{
				m_fAniSpeed = 1.0f;		// ������ �⺻ �ӵ�.			
			}
		}
	} 
// 	else
// 	{	
// 		m_fAniSpeed = 1.0f;
// 	}
}

#ifdef __CLIENT
void CMover::ProcessScaleSlerp()
{
	if( m_fDestScaleSlerp > 0 )		// �پ� ���� �ϴ� ���´�.
	{
		MoverProp* pMoverProp = GetProp();
		if( pMoverProp )
		{
			D3DXVECTOR3 vScale = GetScale();
			if( pMoverProp->dwClass != RANK_MATERIAL && pMoverProp->dwClass != RANK_SUPER && pMoverProp->dwClass != RANK_MIDBOSS )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "�ڿ����� �ƴѵ� ���Դ�.%s %f, %f, %f %f %f" ), GetName(), m_fDestScaleSlerp, m_fDestScale, vScale.x, vScale.y, vScale.z );
				m_fDestScaleSlerp = 0;
			}
			else
			{
				D3DXVECTOR3 vDestScale = D3DXVECTOR3( m_fDestScale, m_fDestScale, m_fDestScale );
				D3DXVec3Lerp( &vScale, &vDestScale, &vScale, m_fDestScaleSlerp );
				SetScale( vScale );
				m_fDestScaleSlerp -= 0.001f;
			}
		}
	}
}

void CMover::ProcessWaterCircle( const D3DXVECTOR3& vPosTemp, const D3DXVECTOR3& vPos )
{
	if( !IsMode( TRANSPARENT_MODE ) )	
	{
		LPWATERHEIGHT pWaterHeight = GetWorld()->GetWaterHeight( GetPos() );

		if(pWaterHeight && (pWaterHeight->byWaterTexture & (byte)(~MASK_WATERFRAME)) == WTYPE_WATER)
		{
			FLOAT fHeight = (FLOAT)pWaterHeight->byWaterHeight;
			if( vPosTemp.y > fHeight && vPos.y <= fHeight && m_pActMover->IsActJump() ) 
			{
				CreateSfx( g_Neuz.m_pd3dDevice, XI_INDEX( 71, XI_GEN_WATERCROWN01 ), D3DXVECTOR3( vPos.x, fHeight, vPos.z ) );
			}
			if( vPos != vPosTemp && vPos.y < fHeight && vPos.y > ( fHeight - 1.5f )  && m_nWaterCircleCount > 7 ) 
			{
				CreateSfx( g_Neuz.m_pd3dDevice, XI_INDEX( 70, XI_GEN_WATERCIRCLE01 ), D3DXVECTOR3( vPos.x, fHeight + .01f, vPos.z) );
				m_nWaterCircleCount = 0;
			}
		}
		m_nWaterCircleCount++;
	}
}

// �÷��̾���� �۶� ����Ҹ��� ����...�ݶ󿡼� ������������ �÷����ϴ°��� �ƴ�.
// �ӽ������� ��Ʈ����Ʈ�� ���� �÷��� �������� �����...
void CMover::ProcessWalkSound()
{
	if( IsPlayer() && !( IsMode( TRANSPARENT_MODE ) ) )
	{
#ifdef __Y_INTERFACE_VER3
		if( m_pActMover->GetMoveState() & OBJSTA_FMOVE || m_pActMover->GetMoveState() & OBJSTA_LMOVE || m_pActMover->GetMoveState() & OBJSTA_RMOVE )
#else //__Y_INTERFACE_VER3
		if( m_pActMover->GetMoveState() & OBJSTA_FMOVE )							
#endif //__Y_INTERFACE_VER3
		{
			CModelObject*	pModel = (CModelObject*)m_pModel;
			if( pModel->IsAttrHit() )
			{
				PLAYSND( SND_WALK_LANDSOFT, &GetPos() );
			}
		}
	}
}

void CMover::ProcessDustSFX()
{
	if( ! ( IsMode( TRANSPARENT_MODE ) ) )		// ������°� �ƴҶ��� ����.
	{
		CModelObject*	pModel = (CModelObject*)m_pModel;
		MOTION_ATTR* pAttr = pModel->IsAttrSound();
		if( IsPlayer() && pAttr && pAttr->m_nSndID > 0 )  // ȿ���� �Ӽ��� �ִٸ� �÷���, pause���¸� ���� ��� ����
		{
			if( m_pActMover->IsActJump() )
				CreateSfx( g_Neuz.m_pd3dDevice, XI_INDEX( 1010, XI_NAT_DUST_JUMP ), GetPos() );
			else
				if( m_pActMover->IsRun() && !m_pActMover->IsActAttack() )
					CreateSfx( g_Neuz.m_pd3dDevice, XI_INDEX( 1005, XI_NAT_DUST_RUN ), GetPos() );
		}
	}
}

// ��Ÿ ó�� 
void CMover::ProcessETC()
{
	CreateAbilityOption_SetItemSFX( GetSetItemClient() );

	CModelObject*	pModel = (CModelObject*)m_pModel;
	if( pModel->m_SparkInfo.m_bUsed )
	{
		if( pModel->m_SparkInfo.m_nCnt != 0 )
		{
			pModel->m_SparkInfo.m_fLerp -= 0.05f;

			if( pModel->m_SparkInfo.m_fLerp < 0.0f )
			{
				pModel->m_SparkInfo.m_bUsed = FALSE;
			}
		}

		pModel->m_SparkInfo.m_nCnt++;
	}

	if( (m_nCount & 131071) == 0 ) // 30�и���
	{
		if( xRandom(100) > 50 )
		{
			PT_ITEM_SPEC pItemProp = NULL;

			if( IsActiveMover() )
			{
				FLItemElem* pItemElem	= GetEquipItem( PARTS_HAT );

				if( pItemElem )
					pItemProp = pItemElem->GetProp();
			}
			else
			{
				DWORD dwId	= m_aEquipInfo[PARTS_HAT].dwItemID;
				O3D_ELEMENT*	pElement = NULL;

				if( dwId != 0 )
					pItemProp	= g_xSpecManager->GetSpecItem( dwId );
			}


			if( pItemProp )
			{
				if( GetSex() == SEX_MALE )
				{
					if( pItemProp->dwID == ITEM_INDEX( 15206, II_ARM_M_CHR_DARKVADER01HAT ) )
					{
						char szFormat[128] = {0};
						FLSPrintf( szFormat, _countof( szFormat ), "/s %s", prj.GetText(TID_GAME_FATHER) );
						g_DPlay.SendChat( szFormat );
					}
				}
				else
				{
					if( pItemProp->dwID == ITEM_INDEX( 15210, II_ARM_F_CHR_DARKVADER01HAT ) )
					{
						char szFormat[128] = {0};
						FLSPrintf( szFormat, _countof( szFormat ), "/s %s", prj.GetText(TID_GAME_MOTHER) );
						g_DPlay.SendChat( szFormat );
					}
				}
			}		
		}
	}

	// ����Ĺ� ���̶��...���ϸ� ���̱� ���� ���¿� ���� �������� �����Ѵ�.
	if( GetWorld() && GetWorld()->GetID() == WI_WORLD_GUILDWAR )
	{
		CWndWorld *pWndWorld = (CWndWorld *)g_WndMng.m_pWndWorld;

		if( IsPlayer() && g_pPlayer && pWndWorld )
		{
			BOOL bFlag = FALSE;
			int nActivePlayer = pWndWorld->IsGCStatusPlayerWar( g_pPlayer->m_idPlayer );
			int nPlayer = pWndWorld->IsGCStatusPlayerWar( m_idPlayer );

			// ��Ƽ��(���ΰ�)�� �������̸�
			if( nActivePlayer == 1 )
			{
				// �������� �ƴѾֵ��� ��������
				if( nPlayer != 1 )
				{
					bFlag = TRUE;
				}
			}
			else
				// ��Ƽ��(���ΰ�)�� ������̸�
				if( nActivePlayer == 0 )
				{
					// �Ϲ��ε��� ��������
					if( nPlayer == -1 )
					{
						bFlag = TRUE;
					}
				}

				if( bFlag )
				{
					m_dwMode |= GCWAR_NOT_CLICK_MODE;
					m_dwMode |= GCWAR_RENDER_SKIP_MODE;
				}
				else
				{
					m_dwMode &= ~GCWAR_NOT_CLICK_MODE;
					m_dwMode &= ~GCWAR_RENDER_SKIP_MODE;				
				}
		}
	}
	else
	{
		m_dwMode &= ~GCWAR_NOT_CLICK_MODE;
		m_dwMode &= ~GCWAR_RENDER_SKIP_MODE;				
	}

	if( GetWorld() && GetWorld()->GetID() == WI_WORLD_QUIZ )
	{
		CWndWorld *pWndWorld = (CWndWorld *)g_WndMng.m_pWndWorld;

		if( IsPlayer() && CQuiz::GetInstance()->GetZoneType( this ) != CQuiz::ZONE_QUIZ && pWndWorld )
			m_dwMode |= QUIZ_RENDER_SKIP_MODE;
		else
			m_dwMode &= ~QUIZ_RENDER_SKIP_MODE;
	}
	else
		m_dwMode &= ~QUIZ_RENDER_SKIP_MODE;
}

extern 	void CreateFlyParticle( CMover *pMover, float fAngX, int nType );

#endif // __CLIENT


void CMover::ProcessIAObjLink( D3DXVECTOR3& vPos )
{
	if( GetIAObjLink() )
	{
		D3DXMATRIX mLocal, mTrans, mRot, mWorld, *pmWorldIA;
		D3DXMatrixTranslation( &mTrans, vPos.x, vPos.y, vPos.z );
		D3DXMatrixRotationY( &mRot, D3DXToRadian( -GetAngle() ) );
		mWorld = m_matScale;

		pmWorldIA = GetIAObjLink()->GetMatrixWorldPtr();
		D3DXMatrixMultiply( &mWorld, &mWorld, &mRot );
		D3DXMatrixMultiply( &mWorld, &mWorld, &mTrans );
		D3DXMatrixMultiply( &mLocal, &mWorld, GetIAObjLink()->GetInvTM() );	// IA������Ʈ�κ����� ���� ���.
		D3DXMatrixMultiply( &mWorld, &mLocal, pmWorldIA );

		vPos.x = mWorld._41;
		vPos.y = mWorld._42;
		vPos.z = mWorld._43;

		D3DXQUATERNION qRot;
		D3DXVECTOR3 vYPW;
		D3DXQuaternionRotationMatrix( &qRot, &mWorld );
		QuaternionRotationToYPW( qRot, vYPW );
		FLOAT fAngNew = -D3DXToDegree(vYPW.y);
		SetAngle( fAngNew );
	}
}

void CMover::ProcessActionPoint()
{
	if( _GetContentState( CT_NEWUI_19 ) != CS_VER1 )
	{
		return;
	}

	// 3�ʸ��� �׼�����Ʈ �ڵ� ���.
	if( (int)(g_tmCurrent - m_tmActionPoint) > (1000 * 3) )		
	{
		FLOAT fTime = (g_tmCurrent - m_tmActionPoint) / 1000.0f;	// ������ �ð��� �Ҽ��� ������ ȯ��.
		fTime /= 3.0f;		// 3�ʸ��� �����̹Ƿ�.
		if( fTime < 0 )	fTime = 0;

#ifdef __WORLDSERVER
		((FLWSUser*)this)->m_playTaskBar.m_nActionPoint += (int)(fTime * 2.0f);
		if( ((FLWSUser*)this)->m_playTaskBar.m_nActionPoint > 100 )
			((FLWSUser*)this)->m_playTaskBar.m_nActionPoint = 100;

		if( m_nDuel == 1 )		// �÷��̾ / ������϶�.
		{
			CMover *pDstDuel = prj.GetMover( m_idDuelOther );
			if( IsInvalidObj(pDstDuel) )		// ����밡 ����������.
			{
				( (FLWSUser*)this )->AddDuelCancel( NULL_ID );
				ClearDuel();
			}
		}
#else
		if( IsActiveMover() )
		{
			g_WndMng.m_pWndTaskBar->m_nActionPoint += (int)(fTime * 2.0f);
			if( g_WndMng.m_pWndTaskBar->m_nActionPoint > 100 )
				g_WndMng.m_pWndTaskBar->m_nActionPoint = 100;

			g_Neuz.m_NeuzEnemy.CheckInvalid();
		}
#endif // CLIENT

		m_tmActionPoint = g_tmCurrent;
	} 
}

BOOL CMover::IsMonster()
{
	//        NPC
	// Monster, citizen    

	if( IsNPC() && FALSE == IsCitizen() )
		return TRUE;

	return FALSE;
}
	
BOOL CMover::IsCitizen()
{
	MoverProp* pProp = GetProp();
	if( NULL == pProp )
		return FALSE;

	if( RANK_GUARD == pProp->dwClass || RANK_CITIZEN == pProp->dwClass )
		return TRUE;

	return FALSE;
}

#ifdef __WORLDSERVER
#define	CF_SEC	15
#else	// __WORLDSERVER
#define	CF_SEC	63
#endif	// __WORLDSERVER

void CMover::Process()
{
	if( IsDelete() )
		return;

	CModelObject*	pModel = (CModelObject*)m_pModel;
	D3DXVECTOR3		vPos, vPosTemp;
	BOOL			bCollision;	

#ifdef __WORLDSERVER
	//if( /*m_2pc.size() == 0 &&*/ m_dwAIInterface != AII_PET )		//�ֺ��� pc�� ���� .. �ֵ� �ƴϸ�    //pc�� �ƴϰ�(�ֺ��� pc�� ���� ) �ֵ� �ƴϸ� ����
	//	return;


	DEBUG_ASSERT( m_nNearPCCount >= 0 );
	if( m_bPlayer == FALSE && m_nNearPCCount <= 0 && m_dwAIInterface != AII_PET )
		return;


	//const DWORD UPDATE_TICK		= 1000;

	//if( m_dwUpdateDestTick + UPDATE_TICK < ::GetTickCount() ) //���� ���ξִ� �Ⱥ����� ��
	//{
	////	if( IsEmptyDest() == FALSE )
	//	{
	//		if( IsEmptyDestObj() == FALSE )
	//			g_xWSUserManager->AddUpdateMoverSetDestObj( this );
	//		else if( IsEmptyDestPos() == FALSE )
	//			g_xWSUserManager->AddUpdateMoverSetDestPos( this );

	//		//g_xWSUserManager->AddSetPosAngle( this, GetPos(), GetAngle() );
	//	}
	//	m_dwUpdateDestTick		= ::GetTickCount();
	//}
#endif	// __WORLDSERVER


#ifdef __CLIENT
	//sun: 12, �ɼ�â ����
	//if(IsPlayer()) 
	//{
	O3D_ELEMENT* pMask = pModel->GetParts(PARTS_MASK);
	if(pMask)
	{
		if(IsOptionRenderMask() )
			pModel->SetEffectOff(PARTS_MASK, XE_HIDE);
		else
			pModel->SetEffect(PARTS_MASK, XE_HIDE);
	}
	//}

	// ���� ���� 
	if( m_nDmgCnt > 0 )		
		--m_nDmgCnt;
#else	
	if( m_nDead > 0 )		
		--m_nDead;
#endif	


#ifdef __CLIENT
	if( IsNPC() && IsFlyingNPC() )				// ������� ��ƼŬ ����
		CreateFlyParticle( this, GetAngleX(), 0 );

	ProcessScaleSlerp();
	AutoSynchronizer()->UpdateGhost();

	if( m_pRide && m_pRide->m_pBone )
	{
		WingMotionSetting( pModel );
	}

	if( m_pRide )
		m_pRide->FrameMove( NULL, GetRideFrameSpeed() );

	if( m_pLadolf )
		m_pLadolf->FrameMove();
	
	// ������ ���信 �����ҷ��� �õ��� �ڵ�		2016.07.11
	//if( m_pCloakWing )
	//	m_pCloakWing->FrameMove();

	//sun: 8�� ���� ��ȯ Neuz, World, Trans
	if( IsPlayer() && HasBuffByIk3( IK3_ANGEL_BUFF ) )
	{
		if( m_pAngel == NULL )
		{
			m_pAngel = new CModelObject;
			m_pAngel->InitDeviceObjects( g_Neuz.m_pd3dDevice );

			IBuff* pBuff	= m_buffs.GetBuffByIk3( IK3_ANGEL_BUFF );
			PT_ITEM_SPEC pItemProp	= NULL;
			if( pBuff )
				pItemProp	= pBuff->GetSpecItem();
			if( pItemProp )
			{
				switch(pItemProp->dwSfxElemental)
				{
				case ELEMENTAL_ANGEL_BLUE: m_dwAngelKind = 13; break;
				case ELEMENTAL_ANGEL_RED:  m_dwAngelKind = 16; break;
				case ELEMENTAL_ANGEL_WHITE: m_dwAngelKind = 19; break;
				case ELEMENTAL_ANGEL_GREEN: m_dwAngelKind = 22; break;
				}

				CString textFile;
				textFile.Format("%s.chr", pItemProp->szTextFileName);
				m_pAngel->LoadBone( textFile );
				textFile.Format("%s.o3d", pItemProp->szTextFileName);
				m_pAngel->LoadElement( textFile, 0 );
				textFile.Format("%s_stand01.ani", pItemProp->szTextFileName);
				m_pAngel->LoadMotion( textFile );

				m_AngelPos = GetPos();

				m_AngelWorldM = m_matWorld;
				D3DXMATRIX mScal;			
				D3DXMatrixScaling( &mScal, 0.2f, 0.2f, 0.2f );	

				m_AngelWorldM = mScal * m_matRotation;

				m_AngelPos.x += 1.0f;
				m_AngelWorldM._41 = m_AngelPos.x;
				m_AngelWorldM._42 = m_AngelPos.y + 1.0f;
				m_AngelWorldM._43 = m_AngelPos.z;

				m_pAngelFlag = TRUE;
			}
		}
	}
	else
	{
		m_pAngelFlag = FALSE;
		if( m_pAngel )
		{
			SAFE_DELETE(m_pAngel);
			m_pAngel = NULL;
		}
	}

	if( m_pAngel )
	{
		m_pAngel->FrameMove();
		AngelMoveProcess();
	}


	//sun:10, __EVE_BALLOON
	if( IsPlayer() != FALSE && HasBuffByIk3( IK3_BALLOON ) != FALSE )
	{
		if( m_pBalloon == NULL )
		{
			m_pBalloon = new CModelObject;
			m_pBalloon->InitDeviceObjects( g_Neuz.m_pd3dDevice );

			IBuff* pBuff = m_buffs.GetBuffByIk3( IK3_BALLOON );
			if( pBuff != NULL )
			{
				PT_ITEM_SPEC pItemProp = pBuff->GetSpecItem();
				if( pItemProp )
				{
					TCHAR szModelName[ MAX_PATH ];
					prj.m_modelMng.MakeModelName( szModelName, _countof( szModelName ), OT_ITEM, pItemProp->dwID );
					m_pBalloon->LoadElement( szModelName, 0 );

					m_BalloonPos = GetPos();

					m_BalloonWorldM = m_matWorld;
					D3DXMATRIX mScal;			
					D3DXMatrixScaling( &mScal, 1.5f, 1.5f, 1.5f );	

					m_BalloonWorldM = mScal;// * m_matRotation;

					m_BalloonPos.x += 1.0f;
					m_BalloonWorldM._41 = m_BalloonPos.x;
					m_BalloonWorldM._42 = m_BalloonPos.y + 1.0f;
					m_BalloonWorldM._43 = m_BalloonPos.z;

					m_pBalloonFlag = TRUE;
				}
			}
		}
	}
	else
	{
		m_pBalloonFlag = FALSE;
		if( m_pBalloon )
		{
			SAFE_DELETE(m_pBalloon);
			m_pBalloon = NULL;
			m_fBalloonYPos = 1.0f;
		}
	}

	if( m_pBalloon )
	{
		m_pBalloon->FrameMove();
		BalloonMoveProcess();
	}


	if( g_tmCurrent - m_dwWingTime > 1100 )
	{
		if(HasBuff( BUFF_ITEM, ITEM_INDEX( 10223, II_CHR_MAG_TRI_ANGELWING ) ) || HasBuff( BUFF_ITEM, ITEM_INDEX( 6319, II_SYS_SYS_EVE_WINGS ) ))
		{
			if(m_pSfxWing == NULL)				
				m_pSfxWing = CreateSfx( D3DDEVICE, XI_INDEX( 1095, XI_NAT_WINGANGEL01 ), GetPos(), GetId(), D3DXVECTOR3(0,0,0), NULL_ID, -1 );				
		}
		else
		{
			if(m_pSfxWing != NULL)
			{
				m_pSfxWing->Delete();
				m_pSfxWing = NULL;
			}
		}
		m_dwWingTime = g_tmCurrent;
	}
#endif // __CLIENT

	// 1�� 1ȸ ����� ó��
	if( IsPlayer() )
	{
#ifdef __CLIENT
		if( IsActiveMover() && ( ( m_nCount & 31 ) == 0 ) )
			g_WorldMng.CheckMapKey();
#endif // __CLIENT

		if( ( m_nCount & CF_SEC ) == 0 )
		{
			ProcessPet();	//sun: 9, 9-10�� ��

			//sun: 11, ä�� �ý���
#ifdef __WORLDSERVER
			ProcessCollecting();
			( (FLWSUser*)this )->m_Pocket.ProcessExpiring();
#else	// ProcessCollecting
			if( IsActiveMover() )
				ProcessCollecting();
#endif	// ProcessCollecting

			if( m_dwMute > 0 )
				m_dwMute--;
		}
	}

#ifdef __CLIENT
	ProcessBuff( );
#else
	if( ( m_nCount & CF_SEC ) == 0 )
	{
		ProcessBuff();
	}
#endif

	{
		ProcessMove();	
	}

	{
		ProcessAI();							// 1ȸ �̵��ϸ� ProcessAI�� �ݵ�� ȣ��Ǿ�� �Ѵ�.
	}

	vPosTemp = vPos	= GetPos();

	{
		m_pActMover->ProcessPreAction();		// ���� �ൿ ���¿� ���� ������	 ����� ó����
		ProcessAniSpeed();						// ���� �ִϸ��̼� �ӵ� ����.
	}

	{
#ifdef __X15
		for( int i = 0; i < 4; ++i )
#endif
		{
			ProcessMovePattern();

			m_pActMover->ProcessAction( &vPos );

#ifdef __CLIENT
			ProcessWalkSound();
#endif //__CLIENT

			if( IsAnimate() )
			{
				FLOAT fFactor	= 1.0F;

//				if( !( IsPlayer() && IsFly() ) && ( m_pActMover->IsState( OBJSTA_FMOVE ) || m_pActMover->IsState( OBJSTA_TURN_ALL ) ) )
				if( IsFly() == FALSE
					&& m_pActMover->IsState( OBJSTA_ATK_ALL ) == FALSE
					&& ( m_pActMover->IsState( OBJSTA_FMOVE ) || m_pActMover->IsState( OBJSTA_TURN_ALL ) )
					)
				{
					int nMaxFrame	= pModel->GetMaxFrame();
					FLOAT fSpeed	= GetSpeed( m_pActMover->m_fSpeed );
					//					FLOAT fMaxWidth	= pModel->GetMaxWidth();
					//					FLOAT fMaxHeight	= pModel->GetMaxHeight();
					MoverProp* pProp	= GetProp();
					if( IsNPC() )
					{
						switch( pProp->dwAI )
						{
						case AII_MONSTER:
						case AII_PET:
						case AII_EGG:
						case AII_PARTY_AGGRO_LEADER:
							{
								fFactor	= fSpeed * nMaxFrame / 1.1F;
								break;
							}
						}
					}
					else
						fFactor	= fSpeed * nMaxFrame / 2.2F;

					if( fabs( pProp->fFrame - 0.0F ) < 0.000001F )		// 0�̸� ����
						fFactor		= 1.0F;
					else if( pProp->dwFlying == 1 )
						fFactor		= 1.0F;
					else
						fFactor		*= pProp->fFrame;
					if( fabs( fFactor - 0.0F ) < 0.000001F )		// fFactor�� 0�̸� �⺻ ��
						fFactor		= 1.0F;
				}
				pModel->FrameMove( &vPos, m_fAniSpeed * fFactor );		// �ִϸ��̼� ������ ����

				//!				pModel->FrameMove( &vPos, m_fAniSpeed );		// �ִϸ��̼� ������ ����

#ifdef __CLIENT
				ProcessDustSFX();
#endif // CLIENT
			}
		}	// for( 4 )

		if( m_nAtkCnt > 0 )		// ���������� �ϰ� ���Ĵ� ī��Ʈ�� ��� ���� �Ѵ�.
			++m_nAtkCnt;
	}
	/*
	#ifdef __CLIENT
	if(this == g_pPlayer)
	{
	int n = 0;
	}
	#endif*/
	{
		bCollision = m_pActMover->ProcessCollision( &vPos );	// �̵����� ���ϱ� & �浹ó��.
	}

#ifdef __CLIENT
	ProcessWaterCircle( vPosTemp, vPos );		// ���� ȿ�� - ������ �ƴҶ��� ���
#endif

	{
		ProcessIAObjLink( vPos );
	}

	{
#ifdef __CLIENT
		if( GetWorld()->GetLandscape( vPos ) )
#endif
		{
// 			if( IsPlayer() == TRUE )
// 			{
// 				if( m_vPos != vPos )
// 				{
// 					FLOAT fDistance	= 0.0F;
// 					D3DXVECTOR3 vDist = m_vPos - vPos;
// 					fDistance	=  sqrtf(vDist.x * vDist.x + vDist.z * vDist.z);
// 
// 					const DWORD dwCurrTick	= GetTickCount();
// 
// 					const DWORD	dwTickGap	= dwCurrTick - m_tMoveCheckData.dwLastMoveTick;
// 
// 					FLTRACE_LOG( PROGRAM_NAME, _T( " MoveTick [%d], fDistance [%f]" ), dwTickGap, fDistance );
// 
// 					m_tMoveCheckData.dwLastMoveTick	= dwCurrTick;
// 
// 					m_tMoveCheckData.dwPrevMoveTick	= m_tMoveCheckData.dwLastMoveTick;
// 
// 					m_tMoveCheckData.dwLastMoveTick	= GetTickCount();
// 				}
// 			}
			
			SetPos( vPos );

			if( bCollision )
				OnActCollision();
			AutoSynchronizer()->CollisionState( bCollision );
		}
	}

	{
		ProcessRegion();
		ProcessQuest();
#ifdef __WORLDSERVER
		ProcessDeleteRespawn();
#endif // __WORLDSERVER
	}

#ifdef __WORLDSERVER
	{
		ProcessRegenItem();
		ProcessScript();
	}
#endif	// __WORLDSERVER

	{
#ifdef __WORLDSERVER
		ProcessRecovery();	// ��Ʈ����Ʈ, ��������Ʈ, �Ƿ�����Ʈ ȸ�� �ϱ�  �ϴ��� �÷��̾ �ڵ����� ����.
		ProcessAbnormalState();

		if( IsNPC() )
		{
			//			if( m_nCollectOwnCnt > 0 )
			//				--m_nCollectOwnCnt;

			if( (m_nCount & 127) == 0 )		// �� 8�ʸ��� �ѹ���
			{
				ProcessTarget();
				ProcInstantBanker();
				ProcInstantGC();
			}
		}
#endif // __WORLDSERVER

		if( IsPlayer() && IsLive() )
			ProcessActionPoint();
	}	
#ifdef __WORLDSERVER
	ProcessSFXDamage();
#endif	// __WORLDSERVER

#if defined(__CLIENT)
	ProcessETC();
	ProcessEyeFlash();

	if( NULL_ID != m_dwMoverSfxId && !m_pSfxBuffPet )		//gmpbigsun: buffpet effect
		SetSfxBuffPet( m_dwMoverSfxId );

#endif //defined(__CLIENT) 

	++m_nCount;		// ������ �������� ���� ������ ī����. ������ �ܿ� 0���� �ʱ�ȭ ���� ����.

}

#ifdef __CLIENT

//sun: 8�� ���� ��ȯ Neuz, World, Trans
void CMover::AngelMoveProcess()
{
	if( m_pAngelFlag && m_pAngel != NULL )
	{
		D3DXVECTOR3 vDist = GetPos() - m_AngelPos;
		FLOAT fDistSq = D3DXVec3LengthSq( &vDist );

		if(fDistSq > 100.0f * 100.0f) //Teleport�� �Ÿ��� �־��� ��� �ٽ� �����ϵ��� ��.
		{
			if(m_pAngel)
				SAFE_DELETE(m_pAngel);
			m_pAngel = NULL;
			m_pAngelFlag = FALSE;
			return;
		}
		if( fDistSq > 1.0f * 1.0f )
		{
			D3DXVECTOR3 vf1 = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			D3DXVECTOR3 vf2 = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
			D3DXVECTOR3 vf3;

			FLOAT fSerp = fDistSq / 10.0f;

			D3DXVec3Lerp( &vf3, &vf1, &vf2, fSerp );

			FLOAT fSpeed = 0.0f;
			m_AngelWorldM = m_matWorld;
			D3DXMATRIX mScal;			
			D3DXMatrixScaling( &mScal, 0.2f, 0.2f, 0.2f );	

			m_AngelWorldM = mScal * m_matRotation;

			D3DXVec3Normalize( &vDist, &vDist );

			fSpeed = GetSpeed( m_pActMover->m_fSpeed ) * vf3.x;

			vDist = vDist * fSpeed;

			m_AngelPos.x += vDist.x;
			m_AngelPos.y += vDist.y;
			m_AngelPos.z += vDist.z;

			m_AngelWorldM._41 = m_AngelPos.x;
			m_AngelWorldM._42 = m_AngelPos.y + 1.0f;
			m_AngelWorldM._43 = m_AngelPos.z;
		}

		CreateAngelParticle( m_AngelPos );
	}
}


//sun:10, __EVE_BALLOON
void CMover::BalloonMoveProcess()
{
	if( m_pBalloonFlag && m_pBalloon != NULL )
	{
		D3DXVECTOR3 vDist = GetPos() - m_BalloonPos;
		FLOAT fDistSq = D3DXVec3LengthSq( &vDist );

		D3DXMATRIX mScal, mRot;
		if(fDistSq > 100.0f * 100.0f) //Teleport�� �Ÿ��� �־��� ��� �ٽ� �����ϵ��� ��.
		{
			if(m_pBalloon)
				SAFE_DELETE(m_pBalloon);
			m_pBalloon = NULL;
			m_pBalloonFlag = FALSE;
			return;
		}
		if( fDistSq > 1.0f * 1.0f )
		{
			D3DXVECTOR3 vf1 = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			D3DXVECTOR3 vf2 = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
			D3DXVECTOR3 vf3;

			FLOAT fSerp = fDistSq / 10.0f;

			D3DXVec3Lerp( &vf3, &vf1, &vf2, fSerp );

			FLOAT fSpeed = 0.0f;
			m_BalloonWorldM = m_matWorld;			
			D3DXMatrixScaling( &mScal, 1.5f, 1.5f, 1.5f );	

			m_BalloonWorldM = mScal;// * m_matRotation;

			D3DXVec3Normalize( &vDist, &vDist );

			fSpeed = GetSpeed( m_pActMover->m_fSpeed ) * vf3.x;

			vDist = vDist * fSpeed;

			m_BalloonPos.x += vDist.x;
			m_BalloonPos.y += vDist.y;
			m_BalloonPos.z += vDist.z;

			m_BalloonWorldM._41 = m_BalloonPos.x;
			m_BalloonWorldM._42 = m_BalloonPos.y + m_fBalloonYPos;
			m_BalloonWorldM._43 = m_BalloonPos.z;

			if( m_bBalloon == TRUE )
			{
				m_fBalloonYPos += 0.006f;

				if( m_fBalloonYPos > 1.5 )
					m_bBalloon = FALSE;
			}
			else
			{
				m_fBalloonYPos -= 0.006f;

				if( m_fBalloonYPos < 0.5 )
					m_bBalloon = TRUE;
			}
		}
		else
		{
			m_BalloonWorldM = m_matWorld;
			D3DXMATRIX mScal;			
			D3DXMatrixScaling( &mScal, 1.5f, 1.5f, 1.5f );

			m_BalloonWorldM = mScal;// * m_matRotation;

			m_BalloonWorldM._41 = m_BalloonPos.x;
			m_BalloonWorldM._42 = m_BalloonPos.y + m_fBalloonYPos;
			m_BalloonWorldM._43 = m_BalloonPos.z;

			if( m_bBalloon == TRUE )
			{
				m_fBalloonYPos += 0.002f;

				if( m_fBalloonYPos > 1.3 )
					m_bBalloon = FALSE;
			}
			else
			{
				m_fBalloonYPos -= 0.002f;

				if( m_fBalloonYPos < 0.7 )
					m_bBalloon = TRUE;
			}
		}

		D3DXMatrixRotationY(&mRot,timeGetTime()/3000.0f);
		m_BalloonWorldM = mRot * m_BalloonWorldM;
	}
}

int CMover::GetSetItemSFX_EffectValue()
{
	IBuff* pBuff( m_buffs.GetBuffByIk3( IK3_ARMOREFFECTCHANGE ) );
	if( NULL == pBuff )
		return NULL;

	const T_ITEM_SPEC* pItemProp = pBuff->GetSpecItem();
	if( NULL == pItemProp )
		return NULL;

	return pItemProp->nEffectValue;
}


//---------------------------------------------------------------------------------------------
// ��Ʈ ������ ����Ʈ�� �ε��� ��� 
// param	: ������
// return	: void
// ����		: ( �ð��� �ٱ��Ͽ� PD�԰� �ǳ��� ����ĥ �ð��� ������... �˼��մϴ�... )
//---------------------------------------------------------------------------------------------
void CMover::GetItemSFXIndex( IN const int nAbilityOption, OUT DWORD& dwSFXIndex, OUT DWORD& dwSFXHandsIndex )
{
	if( FALSE == HasBuffByIk3( IK3_ARMOREFFECTCHANGE ) )
	{
		switch( nAbilityOption )
		{
			case eSetItemSFX_Level::eLevel_3:	dwSFXIndex = XI_INDEX( 1719, XI_GEN_ITEM_SETITEM03 );		break;
			case eSetItemSFX_Level::eLevel_4:	dwSFXIndex = XI_INDEX( 1720, XI_GEN_ITEM_SETITEM04 );		break;
			case eSetItemSFX_Level::eLevel_5:	dwSFXIndex = XI_INDEX( 1721, XI_GEN_ITEM_SETITEM05 );		break;
			case eSetItemSFX_Level::eLevel_6:	dwSFXIndex = XI_INDEX( 1722, XI_GEN_ITEM_SETITEM06 );		break;
			case eSetItemSFX_Level::eLevel_7:	dwSFXIndex = XI_INDEX( 1723, XI_GEN_ITEM_SETITEM07 );		break;
			case eSetItemSFX_Level::eLevel_8:	dwSFXIndex = XI_INDEX( 1724, XI_GEN_ITEM_SETITEM08 );		break;
			case eSetItemSFX_Level::eLevel_9:	dwSFXIndex = XI_INDEX( 1725, XI_GEN_ITEM_SETITEM09 );		break;
			case eSetItemSFX_Level::eLevel_10:	dwSFXIndex = XI_INDEX( 1726, XI_GEN_ITEM_SETITEM10 );		break;
			default:							FLASSERT( NULL );											break;
		}

		dwSFXHandsIndex = XI_INDEX( 1735, XI_SETIEM_EFFECTHAND );
		return;
	}

	switch( GetSetItemSFX_EffectValue() )
	{
		case 1:	// Ȳ�ݻ�
			{
				switch( nAbilityOption )
				{
					case eSetItemSFX_Level::eLevel_3:	dwSFXIndex = XI_INDEX( 2528, XI_GEN_ITEM_SETITEM03_GOLD );		break;
					case eSetItemSFX_Level::eLevel_4:	dwSFXIndex = XI_INDEX( 2529, XI_GEN_ITEM_SETITEM04_GOLD );		break;
					case eSetItemSFX_Level::eLevel_5:	dwSFXIndex = XI_INDEX( 2530, XI_GEN_ITEM_SETITEM05_GOLD );		break;
					case eSetItemSFX_Level::eLevel_6:	dwSFXIndex = XI_INDEX( 2531, XI_GEN_ITEM_SETITEM06_GOLD );		break;
					case eSetItemSFX_Level::eLevel_7:	dwSFXIndex = XI_INDEX( 2532, XI_GEN_ITEM_SETITEM07_GOLD );		break;
					case eSetItemSFX_Level::eLevel_8:	dwSFXIndex = XI_INDEX( 2533, XI_GEN_ITEM_SETITEM08_GOLD );		break;
					case eSetItemSFX_Level::eLevel_9:	dwSFXIndex = XI_INDEX( 2534, XI_GEN_ITEM_SETITEM09_GOLD );		break;
					case eSetItemSFX_Level::eLevel_10:	dwSFXIndex = XI_INDEX( 2535, XI_GEN_ITEM_SETITEM10_GOLD );		break;
					default:							FLASSERT( NULL );												break;
				}

				dwSFXHandsIndex = XI_INDEX( 2576, XI_SETIEM_EFFECTHAND_GOLD );
				return ;
			}
			break;
		case 2:	// ����
			{
				switch( nAbilityOption )
				{
					case eSetItemSFX_Level::eLevel_3:	dwSFXIndex = XI_INDEX( 2536, XI_GEN_ITEM_SETITEM03_RED );		break;
					case eSetItemSFX_Level::eLevel_4:	dwSFXIndex = XI_INDEX( 2537, XI_GEN_ITEM_SETITEM04_RED );		break;
					case eSetItemSFX_Level::eLevel_5:	dwSFXIndex = XI_INDEX( 2538, XI_GEN_ITEM_SETITEM05_RED );		break;
					case eSetItemSFX_Level::eLevel_6:	dwSFXIndex = XI_INDEX( 2539, XI_GEN_ITEM_SETITEM06_RED );		break;
					case eSetItemSFX_Level::eLevel_7:	dwSFXIndex = XI_INDEX( 2540, XI_GEN_ITEM_SETITEM07_RED );		break;
					case eSetItemSFX_Level::eLevel_8:	dwSFXIndex = XI_INDEX( 2541, XI_GEN_ITEM_SETITEM08_RED );		break;
					case eSetItemSFX_Level::eLevel_9:	dwSFXIndex = XI_INDEX( 2542, XI_GEN_ITEM_SETITEM09_RED );		break;
					case eSetItemSFX_Level::eLevel_10:	dwSFXIndex = XI_INDEX( 2543, XI_GEN_ITEM_SETITEM10_RED );		break;
					default:							FLASSERT( NULL );												break;
				}

				dwSFXHandsIndex = XI_INDEX( 2577, XI_SETIEM_EFFECTHAND_RED );
				return ;
			}
			break;
		case 3:	// ������
			{
				switch( nAbilityOption )
				{
					case eSetItemSFX_Level::eLevel_3:	dwSFXIndex = XI_INDEX( 2544, XI_GEN_ITEM_SETITEM03_BLACK );		break;
					case eSetItemSFX_Level::eLevel_4:	dwSFXIndex = XI_INDEX( 2545, XI_GEN_ITEM_SETITEM04_BLACK );		break;
					case eSetItemSFX_Level::eLevel_5:	dwSFXIndex = XI_INDEX( 2546, XI_GEN_ITEM_SETITEM05_BLACK );		break;
					case eSetItemSFX_Level::eLevel_6:	dwSFXIndex = XI_INDEX( 2547, XI_GEN_ITEM_SETITEM06_BLACK );		break;
					case eSetItemSFX_Level::eLevel_7:	dwSFXIndex = XI_INDEX( 2548, XI_GEN_ITEM_SETITEM07_BLACK );		break;
					case eSetItemSFX_Level::eLevel_8:	dwSFXIndex = XI_INDEX( 2549, XI_GEN_ITEM_SETITEM08_BLACK );		break;
					case eSetItemSFX_Level::eLevel_9:	dwSFXIndex = XI_INDEX( 2550, XI_GEN_ITEM_SETITEM09_BLACK );		break;
					case eSetItemSFX_Level::eLevel_10:	dwSFXIndex = XI_INDEX( 2551, XI_GEN_ITEM_SETITEM10_BLACK );		break;
					default:							FLASSERT( NULL );												break;
				}

				dwSFXHandsIndex = XI_INDEX( 2578, XI_SETIEM_EFFECTHAND_BLACK );
				return ;
			}
			break;
		case 4:	// �����
			{
				switch( nAbilityOption )
				{
					case eSetItemSFX_Level::eLevel_3:	dwSFXIndex = XI_INDEX( 2552, XI_GEN_ITEM_SETITEM03_PUPPLE );	break;
					case eSetItemSFX_Level::eLevel_4:	dwSFXIndex = XI_INDEX( 2553, XI_GEN_ITEM_SETITEM04_PUPPLE );	break;
					case eSetItemSFX_Level::eLevel_5:	dwSFXIndex = XI_INDEX( 2554, XI_GEN_ITEM_SETITEM05_PUPPLE );	break;
					case eSetItemSFX_Level::eLevel_6:	dwSFXIndex = XI_INDEX( 2555, XI_GEN_ITEM_SETITEM06_PUPPLE );	break;
					case eSetItemSFX_Level::eLevel_7:	dwSFXIndex = XI_INDEX( 2556, XI_GEN_ITEM_SETITEM07_PUPPLE );	break;
					case eSetItemSFX_Level::eLevel_8:	dwSFXIndex = XI_INDEX( 2557, XI_GEN_ITEM_SETITEM08_PUPPLE );	break;
					case eSetItemSFX_Level::eLevel_9:	dwSFXIndex = XI_INDEX( 2558, XI_GEN_ITEM_SETITEM09_PUPPLE );	break;
					case eSetItemSFX_Level::eLevel_10:	dwSFXIndex = XI_INDEX( 2559, XI_GEN_ITEM_SETITEM10_PUPPLE );	break;
					default:							FLASSERT( NULL );												break;
				}

				dwSFXHandsIndex = XI_INDEX( 2579, XI_SETIEM_EFFECTHAND_PUPPLE );
				return ;
			}
			break;
		case 5:	// ���
			{
				switch( nAbilityOption )
				{
					case eSetItemSFX_Level::eLevel_3:	dwSFXIndex = XI_INDEX( 2560, XI_GEN_ITEM_SETITEM03_GREEN );		break;
					case eSetItemSFX_Level::eLevel_4:	dwSFXIndex = XI_INDEX( 2561, XI_GEN_ITEM_SETITEM04_GREEN );		break;
					case eSetItemSFX_Level::eLevel_5:	dwSFXIndex = XI_INDEX( 2562, XI_GEN_ITEM_SETITEM05_GREEN );		break;
					case eSetItemSFX_Level::eLevel_6:	dwSFXIndex = XI_INDEX( 2563, XI_GEN_ITEM_SETITEM06_GREEN );		break;
					case eSetItemSFX_Level::eLevel_7:	dwSFXIndex = XI_INDEX( 2564, XI_GEN_ITEM_SETITEM07_GREEN );		break;
					case eSetItemSFX_Level::eLevel_8:	dwSFXIndex = XI_INDEX( 2565, XI_GEN_ITEM_SETITEM08_GREEN );		break;
					case eSetItemSFX_Level::eLevel_9:	dwSFXIndex = XI_INDEX( 2566, XI_GEN_ITEM_SETITEM09_GREEN );		break;
					case eSetItemSFX_Level::eLevel_10:	dwSFXIndex = XI_INDEX( 2567, XI_GEN_ITEM_SETITEM10_GREEN );		break;
					default:							FLASSERT( NULL );												break;
				}

				dwSFXHandsIndex = XI_INDEX( 2580, XI_SETIEM_EFFECTHAND_GREEN );
				return ;
			}
			break;
		case 6:	// �Ͼ��
			{
				switch( nAbilityOption )
				{
					case eSetItemSFX_Level::eLevel_3:	dwSFXIndex = XI_INDEX( 2568, XI_GEN_ITEM_SETITEM03_WHITE );		break;
					case eSetItemSFX_Level::eLevel_4:	dwSFXIndex = XI_INDEX( 2569, XI_GEN_ITEM_SETITEM04_WHITE );		break;
					case eSetItemSFX_Level::eLevel_5:	dwSFXIndex = XI_INDEX( 2570, XI_GEN_ITEM_SETITEM05_WHITE );		break;
					case eSetItemSFX_Level::eLevel_6:	dwSFXIndex = XI_INDEX( 2571, XI_GEN_ITEM_SETITEM06_WHITE );		break;
					case eSetItemSFX_Level::eLevel_7:	dwSFXIndex = XI_INDEX( 2572, XI_GEN_ITEM_SETITEM07_WHITE );		break;
					case eSetItemSFX_Level::eLevel_8:	dwSFXIndex = XI_INDEX( 2573, XI_GEN_ITEM_SETITEM08_WHITE );		break;
					case eSetItemSFX_Level::eLevel_9:	dwSFXIndex = XI_INDEX( 2574, XI_GEN_ITEM_SETITEM09_WHITE );		break;
					case eSetItemSFX_Level::eLevel_10:	dwSFXIndex = XI_INDEX( 2575, XI_GEN_ITEM_SETITEM10_WHITE );		break;
					default:							FLASSERT( NULL );									break;
				}

				dwSFXHandsIndex = XI_INDEX( 2581, XI_SETIEM_EFFECTHAND_WHITE );
				return ;
			}
			break;
		default:
			{
				FLTRACE_LOG( PROGRAM_NAME, _T("[��Ʈ ������ ����Ʈ] ���� ����Ʈ �ܰ�") );
				FLASSERT( NULL );
			}
			break;
	}
}


//---------------------------------------------------------------------------------------------
// ��Ʈ ������ ����Ʈ �����ϱ�
// param	: ������
// return	: void
//---------------------------------------------------------------------------------------------
void CMover::CreateAbilityOption_SetItemSFX( int nAbilityOption )
{
	if( nAbilityOption >= eSetItemSFX_Level::eLevel_3 &&
		!(m_dwFlag & MVRF_SETITEMSFX) )
	{
		m_dwFlag |= MVRF_SETITEMSFX;
	}
	else
		return ;


	CSfxPartsLinkShoulder*		pSfx( NULL );
	DWORD 						dwSfx( NULL );
	DWORD 						dwSfxHands( NULL );
	D3DXVECTOR3					v3Scal( D3DXVECTOR3( 1.0f, 1.0f, 1.0f ) );
	int							nEffectValue( GetSetItemSFX_EffectValue() );

	GetItemSFXIndex( nAbilityOption, dwSfx, dwSfxHands );

	if( nAbilityOption == 3 )
	{
		// ���
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 0;
		pSfx->m_v3Offset.y = 0.1f;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 1;
		pSfx->m_v3Offset.y = 0.1f;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;
	}
	else if( nAbilityOption == 4 )
	{
		// ����
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 6;
		pSfx->SetScale(D3DXVECTOR3( 3.0f, 3.0f, 3.0f ));
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;
	}
	else if( nAbilityOption == 5 )
	{
		// ���
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 0;
		pSfx->m_v3Offset.y = 0.1f;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 1;
		pSfx->m_v3Offset.y = 0.1f;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		// ���, �ȸ� �߰�
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 4;		
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 5;	
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;
	}
	else if( nAbilityOption == 6 )
	{
		// ���
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 0;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 1;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		// ���, �ȸ� �߰�
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 4;		
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 5;	
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		// �ȸ�
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 2;
		pSfx->m_v3Offset.y = 0.1f;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 3;		
		pSfx->m_v3Offset.y = 0.1f;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

	}
	else if( nAbilityOption == 7 )
	{
		// ���
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 0;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;


		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 1;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		// ���, �ȸ� �߰�
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 4;		
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 5;	
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		// �ȸ�
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 2;
		pSfx->m_v3Offset.y = 0.1f;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 3;		
		pSfx->m_v3Offset.y = 0.1f;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		// ��	
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 8;			
		pSfx->m_v3Offset.y = 0.1f;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 9;		
		pSfx->m_v3Offset.y = 0.1f;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;
	}
	else if( nAbilityOption == 8 )
	{
		// ���
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 0;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 1;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		// ���, �ȸ� �߰�
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 4;		
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 5;	
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		// �ȸ�
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 2;
		pSfx->m_v3Offset.y = 0.1f;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 3;		
		pSfx->m_v3Offset.y = 0.1f;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		// ��	
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 8;			
		pSfx->m_v3Offset.y = 0.1f;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 9;		
		pSfx->m_v3Offset.y = 0.1f;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfxHands, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 9;			
		pSfx->m_v3Offset.y = 0.2f;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;
	}
	else if( nAbilityOption == 9 )
	{
		// ���
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 0;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 1;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		// ���, �ȸ� �߰�
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 4;		
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 5;	
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		// �ȸ�
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 2;
		pSfx->m_v3Offset.y = 0.1f;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 3;		
		pSfx->m_v3Offset.y = 0.1f;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		// ��	
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 8;			
		pSfx->m_v3Offset.y = 0.1f;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 9;		
		pSfx->m_v3Offset.y = 0.1f;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		// ��
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 26;	
		pSfx->SetScale(D3DXVECTOR3( 1.5f, 1.5f, 1.5f ));		
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 29;
		pSfx->SetScale(D3DXVECTOR3( 1.5f, 1.5f, 1.5f ));
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfxHands, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 9;			
		pSfx->m_v3Offset.y = 0.2f;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;
	}
	else if( nAbilityOption == 10 )
	{
		// ���
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 0;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 1;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		// ���, �ȸ� �߰�
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 4;		
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 5;	
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		// ����
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 6;
		pSfx->SetScale(D3DXVECTOR3( 3.0f, 4.5f, 3.0f ));
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 7;		
		pSfx->SetScale(D3DXVECTOR3( 3.0f, 3.0f, 3.0f ));		
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		// ��	
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 8;			
		pSfx->m_v3Offset.y = 0.1f;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 9;	
		pSfx->m_v3Offset.y = 0.1f;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		// �ȸ�
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 2;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 3;		
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		// ��
		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 26;			
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfx, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 29;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;


		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfxHands, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 8;			
		pSfx->m_v3Offset.y = 0.2f;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;

		pSfx = (CSfxPartsLinkShoulder*)CreateSfx( D3DDEVICE, dwSfxHands, GetPos(), GetId(), GetPos(), GetId(), -1 );
		pSfx->m_nPartsLink = 9;			
		pSfx->m_v3Offset.y = 0.2f;
		pSfx->m_nOldAbilityOption = nAbilityOption;
		pSfx->m_nOldEffectValue = nEffectValue;
	}
}

#endif //__CLIENT

//int nItemKind, int nItemKind2, int nNumMax, int nUniqueMin, int nUniqueMax, int nTotalNum, CAnim* pAnimParent, int nMaterialCount )
void CMover::GenerateVendorItem( T_ITEM_SPEC** apItemProp, int* pcbSize, int nMax, LPVENDOR_ITEM pVendor )
{
	CPtrArray* pItemKindAry		= prj.GetItemKindAry( pVendor->m_nItemkind3 );
	PT_ITEM_SPEC pItemProp		= NULL;
	int cbSizeOld	= *pcbSize;
	UNREFERENCED_PARAMETER( cbSizeOld );

	FLASSERT( pVendor->m_nUniqueMin >= 0 );
	FLASSERT( pVendor->m_nUniqueMax >= 0 );

	if( *pcbSize >= nMax )
		return;

	int nMinIdx	= -1, nMaxIdx	= -1;

	for( int j = pVendor->m_nUniqueMin; j <= pVendor->m_nUniqueMax; j++ )
	{
		nMinIdx		= prj.GetMinIdx( pVendor->m_nItemkind3, j );
		if( nMinIdx != -1 )
			break;
	}
	for( int j = pVendor->m_nUniqueMax; j >= pVendor->m_nUniqueMin; j-- )
	{
		nMaxIdx		= prj.GetMaxIdx( pVendor->m_nItemkind3, j );
		if( nMaxIdx != -1 )
			break;
	}
	if( nMinIdx < 0 )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "VENDORITEM//%s//%d-%d//%d" ), GetName(), pVendor->m_nUniqueMin, pVendor->m_nUniqueMax, pVendor->m_nItemkind3 );
		return;
	}

	for( int k = nMinIdx; k <= nMaxIdx; k++ )
	{
		pItemProp	= (PT_ITEM_SPEC)pItemKindAry->GetAt( k );

		if( ( NULL == pItemProp ) ||
			( pItemProp->dwShopAble == (DWORD)-1 ) ||
			( pVendor->m_nItemJob != -1 && pItemProp->dwItemJob != static_cast< DWORD >( pVendor->m_nItemJob ) ) )
			continue;

		if( *pcbSize >= nMax )
			break;

		apItemProp[*pcbSize]	= pItemProp;
		(*pcbSize)++;
	}
}


// ��ǥ ��ġ�� �������� ��� ȣ��� 
void CMover::OnArrive( DWORD dwParam1, DWORD dwParam2 )
{
#ifdef __WORLDSERVER
	if( IsPlayer() )	{
		((FLWSUser* )this)->OnMsgArrival( dwParam1 );    // dwParam1 - pickup���
	}
	else	{
		PostAIMsg( AIMSG_ARRIVAL, dwParam1, dwParam2 );
		ClearDestObj();		// ��ǥ�� �����ϸ� ������ ����.
		g_xWSUserManager->AddMoverBehavior( this );
	}

#endif // __WORLDSERVER
}

void CMover::OnArriveAtPos()
{  
//	SetPos( m_vDestPos );
	SendActMsg( m_dwMotionArrive );			
	OnArrive( NULL_ID, 1 );
	ClearDestPos();
}

BOOL CMover::AddItem( FLItemBase* pItemBase )
{
	return m_Inventory.Add( (FLItemElem*)pItemBase );
}

BOOL CMover::AddItemBank( int nSlot, FLItemElem* pItemElem )
{
	return m_Bank[nSlot].Add( pItemElem );
}

FLItemBase* CMover::GetItemId( DWORD dwId )
{
	return m_Inventory.GetAtId( dwId );
}

FLItemBase* CMover::GetItemBankId( int nSlot, DWORD dwId )
{
	FLItemBase* pItemBase = NULL;
	pItemBase = m_Bank[nSlot].GetAtId( dwId );
	return pItemBase;
}

void CMover::RemoveItemId( DWORD dwId )
{
	if( m_Inventory.IsEquip( dwId ) )
		DoEquip( (FLItemElem*)m_Inventory.GetAtId( dwId ), FALSE );		// ������ ���, ���� ���� ȿ���� ��Ÿ���� �����Ƿ� ����� �ʿ���.
	m_Inventory.RemoveAtId( dwId );
}

void CMover::SetKeeptimeInven( DWORD dwItemId, DWORD dwTime )
{
	DWORD dwSize = m_Inventory.GetMax();
	for( DWORD dwItemObjID = 0 ; dwItemObjID < dwSize; ++dwItemObjID )
	{
		FLItemElem* pItemElem = m_Inventory.GetAtId( dwItemObjID );
		if( pItemElem->GetProp()->dwID == dwItemId )
		{
			SetKeeptimeItem( pItemElem, dwTime );
		}
	}		
}
void CMover::SetKeeptimeBank( DWORD dwItemId, DWORD dwTime )
{
	const BYTE byProgramDataSlot	= GET_PLAYER_SLOT( m_nDBDataSlot );
	
	DWORD dwSize = m_Bank[byProgramDataSlot].GetMax();
	for( DWORD i = 0 ; i < dwSize; i++ )
	{
		FLItemElem* pItemElem = m_Bank[byProgramDataSlot].GetAtId( i );
		if( pItemElem->GetProp()->dwID == dwItemId )
		{
			SetKeeptimeItem( pItemElem, dwTime );
		}
	}			
}
void CMover::SetKeeptimeItem( FLItemElem* pItemElem, DWORD dwTime )
{	
	pItemElem->m_dwKeepTime = dwTime;
}

void CMover::RemoveItemBankId( int nSlot, DWORD dwId )
{
	m_Bank[nSlot].RemoveAtId( dwId );
}


#ifdef __CLIENT
void CMover::DialogOut( LPCTSTR lpszText )
{
	g_DialogMsg.AddMessage( this, lpszText, 0xffffffff );
}
#endif

// bSWDForce��ſ� Option�� �Ἥ ��Ʈ�� �������.
// 0x01 : �˱�
// 0x02 : �������(+100���ϴ�..)
// 0x04 : Motion Transition ������� ����.
BOOL CMover::SetMotion( DWORD dwMotion, int nLoop, DWORD dwOption )
{
	//gmpbigsun( 20110322 ) --BEGIN
	//#0012912 [������_����_16��,17��] ĳ���Ͱ� �ɾҴ� �Ͼ� �� �� ��������� ����� �̵� �Ұ��Ǵ� ����
	// ��ǰ� ���°� �뷫 �����ִ�. 
	// �Ͼ�� ������ ���ظ� ������ �ᱹ SIT���°� ������ ���Ҽ��� �ִ�. 
	// �Ѹ���� ��ǰ� ���´� �и��Ǿ�� �ϴµ� ���� ������ ��ġ���ִ�. 
#ifdef __CLIENT
	if( MTI_GETUP == m_dwMotion && MTI_GETUP != dwMotion )
	{
		//�Ͼ���� ���ظ� ������ �ٷ� ���½�Ų��.
		m_pActMover->ResetState( OBJSTA_MOVE_ALL );		
	}
#endif 
	//--END

	CModelObject* pModel = (CModelObject*)m_pModel;
	DWORD	dwOrigMotion = dwMotion;
	PT_ITEM_SPEC pItemProp = GetActiveHandItemProp();
	if( m_pActMover->IsStateFlag( OBJSTAF_COMBAT ) && (dwOption & MOP_FIXED) == 0 )
	{
#if defined(__CLIENT) 
		if( pItemProp == NULL )
		{
			if( IsPlayer() )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "Player %s %d %d" ), GetName(), IsActiveMover(), m_aEquipInfo[PARTS_RWEAPON].dwItemID );
			}
			else
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "NPC %s %d" ), GetName(), m_dwVirtItem );
			}

			return FALSE;
		}
#endif // __CLIENT

		// ���� ��� dwItemKind3�� virtual �������̴�.
		// �÷��̾�� ���� ������ ���� ����� ����������, ���� virtual �����ۿ��� ����� �����Ѵ�.
		int nIndex = GetIndex();
		if( !IsPlayer() || ( IsPlayer() && 
			( nIndex == OBJECT_INDEX( 11, MI_MALE ) || 
			nIndex == OBJECT_INDEX( 12, MI_FEMALE ) || 
			nIndex == OBJECT_INDEX( 203, MI_CROWNIBLIS ) || 
			nIndex == OBJECT_INDEX( 204, MI_CROWNSHADE ) || 
			nIndex == OBJECT_INDEX( 205, MI_CROWNBUBBLE ) || 
			nIndex == OBJECT_INDEX( 202, MI_DWARPETMAS ) ) ) )  
		{
			BOOL bMasterPlayer = FALSE;

			if( nIndex == OBJECT_INDEX( 203, MI_CROWNIBLIS ) || nIndex == OBJECT_INDEX( 204, MI_CROWNSHADE ) || nIndex == OBJECT_INDEX( 205, MI_CROWNBUBBLE ) || nIndex == OBJECT_INDEX( 202, MI_DWARPETMAS ) ) 
				bMasterPlayer = TRUE;

			if( IsDualWeapon() && !bMasterPlayer )
			{
				dwMotion += 900;		// �̵����� ���� ���⸦ ����� ���� �̵��� ������ �ȴ�.
			} 
			else
			{
				BOOL bHanded = ( pItemProp->dwHanded == HD_TWO ) ? TRUE : FALSE;	// ���ڵ�� ���ڵ��?
				switch( pItemProp->dwItemKind3 )
				{
				case IK3_YOYO: 
					if( !bMasterPlayer ) dwMotion += MTI_STAND_14;
					else dwMotion += 200;
					break;

				case IK3_BOW:
#ifdef __ADDWEAPON_CROSSBOW16
					//�츮 ��ų�� �ʿ乫�⸦ 2���̻� ����Ҽ� ���� �ؼ� IK3_CROSSBOW->IK3_BOW�� �ٲ㼭 �� �����ְ� �ϰ�
					//����� ũ�ν����� ����� ������ �ؾ��Ѵ�.
					if( pItemProp->nDestData1[0] == 100 )	
					{
						if( !bMasterPlayer ) dwMotion += MTI_STAND_15;
						else dwMotion += 200;
					}
					else 
					{
						if( !bMasterPlayer ) dwMotion += MTI_STAND_13;
						else dwMotion += 200;
					}
#else 
					if( !bMasterPlayer ) dwMotion += MTI_STAND_13;
					else dwMotion += 200;
#endif //__ADDWEAPON_CROSSBOW16
					break;        

#ifdef __ADDWEAPON_CROSSBOW16
				case IK3_CROSSBOW:
					if( !bMasterPlayer ) dwMotion += MTI_STAND_15;
					else dwMotion += 200;
					break;
#endif // __ADDWEAPON_CROSSBOW16

				case IK3_SWD: 
					if( bHanded )	dwMotion += 800;	
					else			dwMotion += 200; 
					break;
				case IK3_AXE: 
					if( bHanded )	dwMotion += 800;
					else			dwMotion += 500; 
					break;               
				case IK3_WAND:

					//sun: 10�� ���½ý���	Neuz, World, Trans
					//gmpbigsun( 20101111 ) : ��Ż����Ʈ �̵���� ����
					if( GetJob() == JOB_PSYCHIKEEPER
						|| GetJob() == JOB_PSYCHIKEEPER_MASTER
						|| GetJob() == JOB_PSYCHIKEEPER_HERO
						|| GetJob() == JOB_MENTALIST_HERO
						)	
						dwMotion += MTI_STAND_11;
					else
						dwMotion += MTI_STAND_02; 
					break; 
				case IK3_CHEERSTICK: dwMotion += 400; break; 
				case IK3_STAFF: 
					dwMotion += MTI_STAND_05; 
					break; 
				case IK3_KNUCKLEHAMMER: dwMotion += 700; break; 
				default: 
					if( IsPlayer() ) dwMotion += 200; 
					break;
				}
			}
		}
	}
	if( m_dwMotion == dwOrigMotion && m_dwMotionOption == dwOption )	// ���� ����� �϶�� �ߴµ�...
	{
		if( nLoop == ANILOOP_LOOP )		return FALSE;	// ���θ�� �̸� �� ����
		if( pModel->m_bEndFrame == FALSE )		// ���� �ִϸ��̼����϶��� 
			return FALSE;						// ���.
		if( pModel->m_bEndFrame && nLoop == ANILOOP_CONT )	// �ִϸ��̼��� �������°� ���Ӹ��� ������ ���������� ����
			return FALSE;
	}
#ifdef _DEBUG
	if( dwOrigMotion == MTI_WALK )
	{
		int a = 0;
		UNREFERENCED_PARAMETER( a );
	}
#endif
	prj.m_modelMng.LoadMotion( m_pModel, m_dwType, m_dwIndex, dwMotion );
	m_dwMotion = dwOrigMotion;		// +100 ���� ���� ���� ��������.
	m_dwMotionOption = dwOption;

	pModel->m_bEndFrame = FALSE;
	pModel->SetLoop( nLoop );

	// �� ��� �ִϸ��̼� ���ϴ� Mover���� ����� ���� ������ ����ó��.
	if(m_pModel && m_pModel->m_pModelElem && (m_pModel->m_pModelElem->m_dwIndex == OBJECT_INDEX( 1029, MI_NPC_SNOWMAN01 ) || m_pModel->m_pModelElem->m_dwIndex == OBJECT_INDEX( 1030, MI_NPC_SNOWMAN02 )))
		dwOption |= MOP_NO_TRANS;

	if( dwOption & MOP_NO_TRANS )		pModel->SetMotionBlending( FALSE );
	else								pModel->SetMotionBlending( TRUE );
	if( dwOption & MOP_SWDFORCE )
	{
		if( IsPlayer() )
		{
			int nOption = 0;
			DWORD dwColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
			if( IsActiveMover() )
			{
				FLItemElem *pItemElem = GetWeaponItem();
				if( pItemElem )
					nOption = pItemElem->GetAbilityOption();
			} 
			else
			{	
				// Ÿ �÷��̾�� ����ũ�������̹Ƿ� ���⼭���� ���� ���´�.
				nOption		= m_aEquipInfo[PARTS_RWEAPON].nOption & 0xFF;
			}

			if( nOption == 10 )
				dwColor = D3DCOLOR_ARGB( 255, 255, 0, 0 );
			else if( nOption == 9 )
				dwColor = D3DCOLOR_ARGB( 255, 255, 150, 0 );
			else if( nOption >= 7 )
				dwColor = D3DCOLOR_ARGB( 255, 198, 0, 255 );
			else if( nOption >= 5 )
				dwColor = D3DCOLOR_ARGB( 255, 123, 82, 255 );
			else if( nOption >= 3)
				dwColor = D3DCOLOR_ARGB( 255, 129, 221, 251 );
			else if( nOption >= 1 )
				dwColor = D3DCOLOR_ARGB( 255, 201, 251, 190 );
			else
				dwColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );

			pModel->MakeSWDForce( PARTS_RWEAPON, pItemProp->dwItemKind3, (dwOption & MOP_HITSLOW) ? TRUE : FALSE, dwColor, m_fAniSpeed );
			pModel->MakeSWDForce( PARTS_LWEAPON, pItemProp->dwItemKind3, (dwOption & MOP_HITSLOW) ? TRUE : FALSE, dwColor, m_fAniSpeed );
		}
	}
	else
	{
		if( pModel->m_pForce )
			pModel->m_pForce->Clear();
		if( pModel->m_pForce2 )
			pModel->m_pForce2->Clear();
	}

	return TRUE;
}

BOOL CMover::InitMotion(DWORD dwMotion)
{
	prj.m_modelMng.LoadMotion( m_pModel, m_dwType, m_dwIndex, dwMotion );
	m_dwMotion = dwMotion;
	return TRUE;
}


//���� ���ڸ� ���Ѵ�.
int CMover::GetEnemyCount()
{
	int nCount = 0;
	nCount = m_idEnemies.size();
	return nCount;
}

OBJID CMover::GetMaxEnemyHitID()
{
	OBJID objId = NULL_ID;
	int	nHitPoint = 0;

	for( SET_OBJID::iterator i = m_idEnemies.begin(); i != m_idEnemies.end(); ++i )
	{
		HIT_INFO info = (*i).second;
		if( nHitPoint < info.nHit )
		{
			objId = (*i).first;
			nHitPoint = info.nHit;
		}
	}
	return objId;
}

int CMover::GetEnemyHit( OBJID objid, DWORD* pdwTick )
{
	int nHit = 0;
	DWORD dwTick = 0;

	SET_OBJID::iterator i	= m_idEnemies.find( objid );
	if( i != m_idEnemies.end() )
	{
		HIT_INFO info = (*i).second;
		nHit = info.nHit;
		dwTick = info.dwTick;
	}

	if( pdwTick )
		*pdwTick = dwTick;

	return nHit;
}

// ���������� ���� �ð��� �����Ѵ�.
DWORD CMover::AddEnemy( OBJID objid, int nHit )
{
	DWORD dwLast = 0;
	SET_OBJID::iterator i	= m_idEnemies.find( objid );
	if( i == m_idEnemies.end() )
	{
		HIT_INFO info;
		info.nHit = nHit;
		info.dwTick = GetTickCount();
		m_idEnemies.insert( make_pair(objid, info) );
	}
	else
	{
		HIT_INFO& info = (*i).second;
		dwLast = info.dwTick;
		info.dwTick = GetTickCount();
		info.nHit += nHit;
	}
	return dwLast;
}

void CMover::RemoveEnemy( OBJID objid )
{
	SET_OBJID::iterator i	= m_idEnemies.find( objid );
	if( i != m_idEnemies.end() )
	{
		if( m_pActMover->GetObjHit() == objid )
			m_pActMover->SetObjHit( NULL_ID );
#ifdef __WORLDSERVER
		PostAIMsg( AIMSG_DSTDIE, objid );
#endif

		if( GetDestId() == objid )
		{
			ClearDestObj();
			SendActMsg( OBJMSG_STAND );
		}

		m_idEnemies.erase( i );
	}
}

void CMover::RemoveAllEnemies()
{
	vector<OBJID>	adwEnemy;
	for( SET_OBJID::iterator it = m_idEnemies.begin(); it != m_idEnemies.end(); ++it )
		adwEnemy.push_back( it->first );
	m_idEnemies.clear();

	CMover* pEnemy;
	while( adwEnemy.size() > 0 )
	{
		OBJID objid		= adwEnemy.back();
		pEnemy	= prj.GetMover( objid );
		if( IsValidObj( (CObj*)pEnemy ) )
			pEnemy->RemoveEnemy( GetId() );
		adwEnemy.pop_back();
	}
	adwEnemy.clear();
	/*
	int nSizeofEnemies	= 0;
	OBJID	idEnemies[1024];

	for( SET_OBJID::iterator i = m_idEnemies.begin(); i != m_idEnemies.end(); ++i )
	{
	idEnemies[nSizeofEnemies++]	= (*i).first;
	if( nSizeofEnemies >= 1024 )
	break;
	}

	m_idEnemies.clear();

	CMover* pEnemy;
	for( int j = 0; j < nSizeofEnemies; j++ )
	{
	pEnemy	= prj.GetMover( idEnemies[j] );
	if( IsValidObj( (CObj*)pEnemy ) )
	pEnemy->RemoveEnemy( GetId() );
	}
	*/
}

//
// this�� �װ� ����� ���. - �ܺο��� ���� ȣ���ؼ� ���̱⸸ �Ҷ��� �����ִ�.
// Attacker�� NULL�� ���� �ִ�.  ����������� �� ������ ���.
int CMover::DoDie( CCtrl *pAttackCtrl, DWORD dwMsg )
{
#ifdef __WORLDSERVER
	if( IsPlayer() && m_nDead )
	{
		//FLERROR_LOG( PROGRAM_NAME, _T( "%s ���� �� 5���̳� ���׾���. �ι�������?" ), GetName() );		//������ �� �ִ� �ڵ� ����. �α� ���� ������
		return 0;
	}

	// Ŭ���̾�Ʈ�� �������� ���� ����.
	BOOL	bBehavior = FALSE;
	CMover *pAttacker = NULL;

	if( pAttackCtrl && pAttackCtrl->GetType() == OT_MOVER )		// ����Ŀ�� ������� ���� ������ ����� �д�.
		pAttacker = (CMover *)pAttackCtrl;

	if( pAttacker && pAttacker != this && pAttacker->IsPlayer() )		// �ڻ��Ѱ� �ƴϰ�, �÷��̾�� �׾��� �ƴϳ�.
	{
		PVP_MODE mode = GetPVPCase( pAttacker ); 
		
		if( mode == PVP_MODE_PK ) {		// PK���
			if( g_eLocal.GetState( EVE_PKCOST ) && GetWorld()->IsArena() == FALSE && g_pEventArenaGlobal->IsArenaChannel() == false ) {
				SetDecreaseExperienceRevival( true );
			}
			else {
				SetDecreaseExperienceRevival( false );
			}
		}
		else {
			SetDecreaseExperienceRevival( false );
		}

		if( GetWorld()->GetID() == WI_WORLD_GUILDWAR && g_GuildCombatMng.m_nState != CGuildCombat::CLOSE_STATE ) {
			SetDecreaseExperienceRevival( false );
		}
		//sun: 11, �ϴ��� ��� ����
		if( g_GuildCombat1to1Mng.IsPossibleUser( (FLWSUser*)this ) ) {
			SetDecreaseExperienceRevival( false );
		}

		//sun: 13, ����
		if(IsNPC())
		{
			((FLWSUser*)pAttacker)->SetHonorAdd(GetIndex(),HI_HUNT_MONSTER);

			MoverProp* pMoverProp = NULL;
			pMoverProp = GetProp();
			if( pMoverProp != NULL && pMoverProp->dwClass == RANK_BOSS )
			{
				g_dpDBClient.SendErrorLogToDB( static_cast<FLWSUser*>( pAttacker ), 'D', GetName() );
			}
		}
	}

	m_nAtkCnt = 0;		// �װ� �� �Ŀ��� ����ī��Ʈ�� Ŭ���� �ؼ� ���ڷ縦 Ż �� �ְ�����.

	if( IsPlayer() )
	{
		if( IsStateMode( STATE_BASEMOTION_MODE ) )
		{
			SetStateNotMode( STATE_BASEMOTION_MODE, STATEMODE_BASEMOTION_OFF );
			m_nReadyTime = 0;
			m_dwUseItemId = 0;
		}

		SetStun( FALSE );		// �÷��̾ ��Ȱ�Ŀ� ���� �Ȱɷ� �ֵ��� Ǯ����.
		SetPoison( FALSE );
		SetDark( FALSE );
		SetBleeding( FALSE );
		SetSleeping( false );
	}

	BOOL fValid	= IsValidObj( this );

	if( IsPlayer() && m_pActMover->IsFly() )	// �����ִ� �߿� �׾�����
		UnequipRide();

	SendActMsg( OBJMSG_STOP );
	SendActMsg( OBJMSG_DIE, dwMsg, (int)pAttacker );

	// �÷��̾��� ���� �׾����� ���� Ÿ������ ����ִ����� Ǯ����. �α׾ƿ� ������ �Ҷ��� �������� ó���� �ؾ���.
	if( IsPlayer() )
	{
		CMover *pLastHit = prj.GetMover( m_idLastHitMover );
		if( IsValidObj( pLastHit ) )
		{
			if( pLastHit->m_idAttacker == GetId() )	// pLastHit�� �����ڰ� ���� �����Ǿ� �������� Ǯ����,.
				pLastHit->m_idAttacker = NULL_ID;	
			if( pLastHit->m_idTargeter == GetId() )
				pLastHit->m_idTargeter = NULL_ID;
		}

		// ������ �׾������� �α� ����
		if( fValid )
		{
			g_dpDBClient.SendLogPlayDeath( this, pAttacker );
			//sun: 9, 9-10�� ��
			// ĳ���Ͱ��� ������ ���� ��� ĳ������ ��� �� ���� ������� �ʴ´�.	// 0723
			if( IsValidObj( pAttacker ) == FALSE || pAttacker->IsNPC() )
				//			if( m_bLastPK == FALSE && m_bGuildCombat == FALSE )
			{
				// ĳ���� ��� �� ��ȯ���� �굵 ���
				CPet* pPet	= GetPet();
				if( pPet && pPet->GetLevel() != PL_EGG )	// �� '��'�� ������� �ʵ��� ����	// 0723
				{
					FLItemElem* pItemElem	= GetPetItem();
					int nLife	= (int)pPet->GetLife();
					if( --nLife >= 0 )
					{
						// ������ 0 �̻��̸�, ��Ȱ
						pPet->SetLife( static_cast< WORD >( nLife ) );
						pPet->SetEnergy( CPetProperty::GetInstance()->GetMaxEnergy( pPet->GetLevel() ) );
						//					pPet->SetExp( 0 );

						g_dpDBClient.CalluspPetLog( m_idPlayer, pItemElem->GetSerialNumber(), 0, PETLOGTYPE_DEATH, pPet );
					}
					else
					{
						pPet->SetLife( 0 );
						pPet->SetEnergy( 0 );
						pPet->SetExp( 0 );
						pItemElem->SetFlag( FLItemElem::expired );
						UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_FLAG, MAKELONG( pItemElem->m_dwObjIndex, pItemElem->m_byFlag ) );

						g_dpDBClient.CalluspPetLog( m_idPlayer, pItemElem->GetSerialNumber(), 0, PETLOGTYPE_DEATH, pPet );
					}
					PetRelease();
					( (FLWSUser*)this )->AddPetState( pItemElem->m_dwObjId , pPet->GetLife(), pPet->GetEnergy(), pPet->GetExp() );
				}
			}
		}
		// ����� �÷��̾��� ��� 
		// �������� �Ǵ��ؼ� ����� ��ҽ�Ű�� 
		// �� ��Ƽ���鿡�� ����� �뺸��.
	}

	if( bBehavior ) {
		g_xWSUserManager->AddMoverBehavior( this, TRUE );
	}
	if( fValid )
	{
		//		g_DPCoreClient.SendMoverDeath( this, pAttacker, dwMsg );
		g_xWSUserManager->AddMoverDeath( this, pAttacker, dwMsg );

		RemoveAllEnemies();		// pAttacker->SendAIMsg( AIMSG_DSTDIE, GetId() );
	}	// fValid

	if( pAttacker )
		pAttacker->m_pActMover->SetObjHit( NULL_ID );

	// Ÿ���� ������ AI���� Ÿ���� Ŭ������.
	if( IsValidObj(pAttacker) && pAttacker->IsNPC() )
	{
		pAttacker->SendAIMsg( AIMSG_INIT_TARGETCLEAR, m_objid );		// �����ڴ� Ÿ���� Ŭ�����ϰ� ������ ���ư�.
	}

	// ���͸� ���̸� ���Ͱ� ����Ʈ�� �䱸�ϴ� ������ �Ǵ��ؼ� ų ī��Ʈ ���� 
	if( IsPlayer() == FALSE && pAttacker && pAttacker->IsPlayer() )
	{
		//for( size_t i = 0; i < pAttacker->m_nQuestSize; i++ )
		//{
		//	LPQUEST lpQuest = (LPQUEST) &pAttacker->m_aQuest[ i ]; 
		//	if( !lpQuest )	continue;
		//	QuestProp* pQuestProp = prj.m_aPropQuest.GetAt( lpQuest->m_wId );
		//	if( !pQuestProp )	continue;

		//	for( int j = 0; j < MAX_QUEST_KILL_NPC_COUNT; ++j )
		//	{
		//		// ���� ���� ����Ʈ�� ���� ���ǰ� ���� NPC�ΰ�?
		//		if( pQuestProp->m_nEndCondKillNPCIdx[ j ] != static_cast< int >( GetIndex() ) )	continue;

		//		// �ش� ����Ʈ�� ���
		//		//if( pQuestProp->m_nBeginCondParty == 2 )
		//		//{
		//		CParty* pParty	= g_PartyMng.GetParty( pAttacker->m_idparty );
		//		// �شܿ��߿� �ش� ����Ʈ�� �����ϰ� ������..
		//		if( pParty && pParty->IsMember( pAttacker->m_idPlayer ) )
		//		{
		//			for( int k = 0; k < pParty->GetSizeofMember(); ++k )
		//			{
		//				PartyMember* pPartyMember	= &pParty->m_aMember[k];
		//				CMover* pMember		= prj.GetUserByID( pPartyMember->m_uPlayerId );
		//				if( IsValidObj( pMember ) && IsValidArea( pMember, 64.0f ) )
		//				{
		//					LPQUEST pMemberQuest	= pMember->GetQuest( lpQuest->m_wId );
		//					if( pMemberQuest && pMemberQuest->m_nKillNPCNum[j]  < pQuestProp->m_nEndCondKillNPCNum[j] )
		//					{
		//						++pMemberQuest->m_nKillNPCNum[j];
		//						( (FLWSUser*)pMember )->AddSetQuest( pMemberQuest ); 
		//					}
		//				}
		//			}
		//		}
		//		//}
		//		else
		//			if( lpQuest->m_nKillNPCNum[ j ]  < pQuestProp->m_nEndCondKillNPCNum[ j ] )
		//			{
		//				++lpQuest->m_nKillNPCNum[ j ];
		//				( (FLWSUser*)pAttacker )->AddSetQuest( lpQuest );
		//			}
		//	}
		//}


		CParty* pParty	= g_PartyMng.GetParty( pAttacker->m_idparty );
		// �شܿ��߿� �ش� ����Ʈ�� �����ϰ� ������..
		if( pParty && pParty->IsMember( pAttacker->m_idPlayer ) )
		{
			for( int k = 0; k < pParty->GetSizeofMember(); ++k )
			{
				PartyMember* pPartyMember	= &pParty->m_aMember[k];
				CMover* pMember		= prj.GetUserByID( pPartyMember->m_uPlayerId );
				if( IsValidObj( pMember ) && IsValidArea( pMember, 64.0f ) )
				{
					for( size_t i = 0; i < pMember->m_nQuestSize; i++ )
					{
						LPQUEST lpQuest = (LPQUEST) &pMember->m_aQuest[ i ]; 
						if( !lpQuest )	continue;
						QuestProp* pQuestProp = prj.m_aPropQuest.GetAt( lpQuest->m_wId );
						if( !pQuestProp )	continue;

						for( int j = 0; j < MAX_QUEST_KILL_NPC_COUNT; ++j )
						{
							if( pQuestProp->m_nEndCondKillNPCIdx[ j ] != static_cast< int >( GetIndex() ) )	continue;
							
							if( lpQuest->m_nKillNPCNum[ j ]  < pQuestProp->m_nEndCondKillNPCNum[ j ] )
							{
								++lpQuest->m_nKillNPCNum[ j ];
								( (FLWSUser*)pMember )->AddSetQuest( lpQuest );
							}
						}
					}
				}
			}
		}
		else
		{

			for( size_t i = 0; i < pAttacker->m_nQuestSize; i++ )
			{
				LPQUEST lpQuest = (LPQUEST) &pAttacker->m_aQuest[ i ]; 
				if( !lpQuest )	continue;
				QuestProp* pQuestProp = prj.m_aPropQuest.GetAt( lpQuest->m_wId );
				if( !pQuestProp )	continue;

				for( int j = 0; j < MAX_QUEST_KILL_NPC_COUNT; ++j )
				{
					// ���� ���� ����Ʈ�� ���� ���ǰ� ���� NPC�ΰ�?
					if( pQuestProp->m_nEndCondKillNPCIdx[ j ] != static_cast< int >( GetIndex() ) )	continue;

					if( lpQuest->m_nKillNPCNum[ j ]  < pQuestProp->m_nEndCondKillNPCNum[ j ] )
					{
						++lpQuest->m_nKillNPCNum[ j ];
						( (FLWSUser*)pAttacker )->AddSetQuest( lpQuest );
					}
				}
			}
		}
	}

	if( g_pEventArenaGlobal->IsArenaChannel() )
	{
		g_pEventArena->OnPlayerDied( this );
	}

	if( IsValidObj( pAttacker ) == TRUE && pAttacker->IsPlayer() == TRUE && IsPlayer() == FALSE )
	{
		g_pMadrigalGift->OnMonsterKill( static_cast<FLWSUser*>( pAttacker ), this );
	}

#else // WORLDSERVER
#ifndef __CLIENT
	CMover *pAttacker = NULL;
	if( pAttackCtrl && pAttackCtrl->GetType() == OT_MOVER )		// ����Ŀ�� ������� ���� ������ ����� �д�.
		pAttacker = (CMover *)pAttackCtrl;
	SendActMsg( OBJMSG_DIE, dwMsg, (int)pAttacker );
#endif
#endif // not WORLDSERVER


#if defined(__WORLDSERVER)
	if( GetType() == OT_MOVER )
	{
		RemoveCommonBuffs();
		g_xWSUserManager->AddRemoveAllSkillInfluence( this );
	}

	if( m_nGuildCombatState )
	{
		if( pAttacker && pAttacker->IsPlayer() && GetWorld() )
			GetWorld()->OnDie( this, pAttacker );
	}

	//sun: 12, ����� ��
	if( CSecretRoomMng::GetInstance()->IsInTheSecretRoom( this ) )
	{
		if( IsNPC() && IsValidObj( pAttacker ) == TRUE && pAttacker->IsPlayer() )
			CSecretRoomMng::GetInstance()->SetSecretRoomKill( GetWorld()->GetID(), GetProp()->dwID );
		if( IsPlayer() && CSecretRoomMng::GetInstance()->IsGuildMaster( (FLWSUser*)this ) )
			CSecretRoomMng::GetInstance()->SetFailGuild( (FLWSUser*)this );
	}

	//sun: 12, // __NEW_ITEMCREATEMON_SERVER
	if( IsNPC() )
		CCreateMonster::GetInstance()->SetState( GetId(), 'D' );

	if( CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon( GetWorld()->GetID() ) )
	{
		if( !IsPlayer() && IsValidObj( pAttacker ) == TRUE && pAttacker->IsPlayer() )
			CInstanceDungeonHelper::GetInstance()->SetInstanceDungeonKill( GetWorld()->GetID(), static_cast<DWORD>( this->GetLayer() ), GetProp()->dwID );
	}
#ifdef __EVENTLUA_SPAWN
	if( IsNPC() )
		prj.m_EventLua.RemoveSpawnedMonster( GetId() );
#endif // __EVENTLUA_SPAWN

#endif //__WORLDSERVER	

#if defined __WORLDSERVER && defined __AGGRO16
	//pMonster->SendAIMsg( AIMSG_DSTDIE, m_objid );				//�ȿ��� m_AggroOwnerList.erase�� �ϱ⶧����.. vec���� ������ send
	typedef			std::vector< OBJID >		MonsterVec;
	MonsterVec vecMonster;
	vecMonster.reserve( m_AggroOwnerList.size() );

	for( FLAggro::MapAggroOwner::const_iterator it = m_AggroOwnerList.begin(); it != m_AggroOwnerList.end(); ++it )
	{
		const OBJID objid	= it->first;
		vecMonster.push_back( objid );
	}

	for( MonsterVec::iterator it = vecMonster.begin(); it != vecMonster.end(); ++it )
	{
		const OBJID objid	= *it;
		CMover * pMonster	= prj.GetMover( objid );		
		if( IsValidObj( pMonster ) == FALSE )					
			continue;

		pMonster->SendAIMsg( AIMSG_DSTDIE, m_objid );				//�ȿ��� m_AggroOwnerList.erase�� �ϱ⶧����.. vec���� ������ send
	}
#endif //__WORLDSERVER && defined __AGGRO16

	return 1;
}


//sun: 8,     // 8�� ������� ������� PVP�����ϰ���   Neuz, World

void	CMover::EndPVP(int	nPVPHP)
{
	if(m_bPVPEnd)
		m_bPVPEnd = FALSE;		
	else
		SetPointParam( DST_HP, nPVPHP );	// 2007/11 ����(HP �񵿱� ����)
	//m_nHitPoint = nPVPHP;
}
//
// ��󳡳��� ó��
void	CMover::DoPVPEnd( CCtrl *pAttackCtrl, bool /*bWinner*/ , DWORD /*dwMsg*/ )
{
#ifdef __WORLDSERVER

	CMover *pAttacker = NULL;

	if( pAttackCtrl && pAttackCtrl->GetType() == OT_MOVER )		//  ������� ���� ������ ����� �д�.
		pAttacker = (CMover *)pAttackCtrl;
	else
		return;

	if( IsPlayer() )
	{
		if( IsStateMode( STATE_BASEMOTION_MODE ) )
		{
			SetStateNotMode( STATE_BASEMOTION_MODE, STATEMODE_BASEMOTION_OFF );
			m_nReadyTime = 0;
			m_dwUseItemId = 0;
		}
	}

	SendActMsg( OBJMSG_STOP );
	pAttacker->SendActMsg( OBJMSG_STOP );

	m_nDead = PROCESS_COUNT * 3;		// ��� �� 3�ʰ��� ����
	pAttacker->m_nDead = PROCESS_COUNT * 3;		// ��� �� 3�ʰ��� ����

	// �÷��̾���  ���� Ÿ������ ����ִ����� Ǯ����. 
	if( IsPlayer() )
	{
		CMover *pLastHit = prj.GetMover( m_idLastHitMover );
		if( IsValidObj( pLastHit ) )
		{
			if( pLastHit->m_idAttacker == GetId() )	// pLastHit�� �����ڰ� ���� �����Ǿ� �������� Ǯ����,.
				pLastHit->m_idAttacker = NULL_ID;	
			if( pLastHit->m_idTargeter == GetId() )
				pLastHit->m_idTargeter = NULL_ID;
		}
	}

	m_bPVPEnd = TRUE;	

	if( pAttacker )
	{
		pAttacker->m_pActMover->SetObjHit( NULL_ID );
	}

#endif //__WORLDSERVER	

}


#ifdef __WORLDSERVER

int	CMover::SubPK( CMover *pAttacker, int nReflect )
{
	if( !g_eLocal.GetState( EVE_PK )
		|| GetWorld()->IsArena()
		|| g_pEventArenaGlobal->IsArenaChannel()
		)
		return 1;

	//sun: 8, // __S8_PK
	if( GetWorld()->GetID() == WI_WORLD_GUILDWAR && g_GuildCombatMng.m_nState != CGuildCombat::CLOSE_STATE )
		return 1;

	if( pAttacker->GetWorld()->GetID() == WI_WORLD_GUILDWAR && g_GuildCombatMng.m_nState != CGuildCombat::CLOSE_STATE )
		return 1;

	//sun: 11, �ϴ��� ��� ����
	if( g_GuildCombat1to1Mng.IsPossibleUser( (FLWSUser*)this ) && g_GuildCombat1to1Mng.IsPossibleUser( (FLWSUser*)pAttacker ) )
		return 1;

	if( g_eLocal.GetState( EVE_PKCOST ) 
		&& GetWorld()->IsArena() == FALSE
		)
	{
		if( IsPlayer() && IsChaotic() )		
		{
#if !defined(__INTERNALSERVER)
			if( m_dwAuthorization < AUTH_HELPER )
#endif
			{
				// ������ ��� 
				// - �κ� ���( �������� ����ϸ� �ȵ� - �κ������� �����Ͽ� ����Ҽ� ���� )
				CHAO_PROPENSITY Propensity = prj.GetPropensityPenalty( GetPKPropensity() );
				int nInvenDrop = xRandom( Propensity.nInvenDorpMin, Propensity.nInvenDorpMax + 1 );
				for( int i=0; i < nInvenDrop; ++i )
				{
					if( DoDropItemRandom( TRUE, pAttacker ) == 0 )	
						break;
				}

				// - ���� ���
				int nEquipDrop = xRandom( Propensity.nEquipDorpMin, Propensity.nEquipDorpMax + 1 );
				for( int i=0; i < nEquipDrop; ++i )
				{
					if( DoDropItemRandom( FALSE, pAttacker, TRUE ) == 0 )	
						break;
				}				
			}
		}				
	}

	if( !pAttacker->IsPlayer() || IsNPC() )
		return 1;

	BOOL bAdd = FALSE;
	if( pAttacker->IsChaotic() )	// ī������
	{
		if( !IsChaotic() )
			bAdd = TRUE;
	}
	else							// ��ũ����, ��������
	{
		if( !(IsChaotic() || IsPKPink()) )
			bAdd = TRUE;
	}

	/// ī����ġ, ������ �÷���
	if( bAdd && nReflect == 0 && pAttacker != this ) // chipi_081119 ������ ������ ��ų �ް� �����ϸ� �ڻ�Ǿ� ī���Ǵ� ���� ����...
	{
		if( IsPlayer() && pAttacker->IsPlayer() )
		{
			if( !pAttacker->IsChaotic() )
				((FLWSUser*)pAttacker)->AddPlaySound( SND_PC_CHAOS );
			CString szSendMsg;
			szSendMsg.Format( prj.GetText(TID_GAME_PK_DEFENDER) , pAttacker->GetName() );
			((FLWSUser*)this)->AddText( szSendMsg );
			szSendMsg.Format( prj.GetText(TID_GAME_PK_ATTACKER) , GetName() );
			((FLWSUser*)pAttacker)->AddText( szSendMsg );
		}

		pAttacker->SetPKValue( pAttacker->GetPKValue() + 1 );
		((FLWSUser*)pAttacker)->AddPKValue();

// 		pAttacker->SetPKPropensity( pAttacker->GetPKPropensity() + NextPKPropensity( pAttacker->GetPKValue() ) );
// 		g_xWSUserManager->AddPKPropensity( pAttacker );

		//////////////////////////////////////////////////////////////////////////
		const int nRemainCount = pAttacker->GetRemainCountBuff( IK3_PKPENALTYSCROLL_A );
		if( nRemainCount > 0 )
		{
			IBuff* pBuff = pAttacker->GetBuffByIk3( IK3_PKPENALTYSCROLL_A );
			if( pBuff != NULL )
			{
				const int nSetAdjValue	= pBuff->GetAdjValue() + 1;
				pBuff->SetAdjValue( nSetAdjValue );
				if( pAttacker->GetRemainCountBuff( IK3_PKPENALTYSCROLL_A ) <= 0 )
				{
					pAttacker->RemoveBuff( pBuff->GetType(), pBuff->GetId() );
				}
				else
				{
					static_cast<FLWSUser*>( pAttacker )->AddBuffAdjValueRefresh( IK3_PKPENALTYSCROLL_A, pBuff->GetAdjValue() );
				}
			}
			else
			{
				pAttacker->SetPKPropensity( pAttacker->GetPKPropensity() + NextPKPropensity( pAttacker->GetPKValue() ) );
				g_xWSUserManager->AddPKPropensity( pAttacker );
			}
		}
		else
		{
			pAttacker->SetPKPropensity( pAttacker->GetPKPropensity() + NextPKPropensity( pAttacker->GetPKValue() ) );
			g_xWSUserManager->AddPKPropensity( pAttacker );
		}
		//////////////////////////////////////////////////////////////////////////

		g_dpDBClient.SendLogPkPvp( pAttacker, this, 0, 'P' );
		//sun: 13, ����
#ifndef __MAINSERVER
		if(!pAttacker->IsPlayer())
			FLERROR_LOG( PROGRAM_NAME, _T( "subpk()AddHonorListAck()" ) );
#endif // __MAINSERVER
		((FLWSUser*)pAttacker)->SetHonorAdd(HS_PK_COUNT,HI_COUNT_CHECK);

	}

	return 1;
}

int CMover::SubDuel( CMover *pAttacker )
{
	if( m_nDuel )
		pAttacker->DuelResult( this );	// ����� ��� ó��.

	return 1;
}

int CMover::SubWar( CMover *pAttacker )
{
	// ����� ������ �����ΰ�.
	if( g_eLocal.GetState( EVE_GUILDWAR ) == 0 )	
		return 0;

	// ���� �ٸ�����ΰ�(�����ΰ��� �̰ɷ� �ȵȴ�.)
	if( m_idGuild == pAttacker->m_idGuild )
		return 0;

	// ����������� �׾���.
	g_DPCoreClient.SendWarDead( m_idPlayer );
	return 1;
}

PVP_MODE CMover::GetPVPCase( CMover *pAttacker )
{
	BOOL bPKEnable = FALSE;
	if( g_eLocal.GetState( EVE_18 ) == 1 )			// 18�� ���� 
		bPKEnable = TRUE;

	if( bPKEnable == FALSE || 
		pAttacker == NULL  || 
		pAttacker->IsNPC() || // ���γ��� NPC�� �� ����.
		IsNPC() )			  // �������� NPC�� ������
		return PVP_MODE_NONE;

	// �̰� ����� ����� �ο��̴�!
	if( m_idWar && pAttacker->m_idWar == m_idWar )	// ���� ���� ���￡ �������� ���ΰ�.
	{
		return PVP_MODE_GUILDWAR;
	}

	if( m_nDuel == 0 || m_nDuelState == 300 )	// PK
	{
		/*		if( GetTickCount() < m_dwPKTargetLimit )		// PK�����Ұ��ð����ȿ��� ������ �ȵ�  
		{
		return PVP_MODE_NONE;
		}	*/	// mirchang 100114 ����� Ÿ �������� PK���Ѱ�� ����ġ �϶��� ���� �ʴ´�. ���� ī���� ��� ī����ġ�� �������� ������ �ǿ��� ������ ����..
		return PVP_MODE_PK;
	} 
	else				//��� ��Ȳ.	
	{
		return PVP_MODE_DUEL; 
	}
}

// this�� �װ� ���� PVP(PK, ���, �����)ó��
void CMover::SubPVP( CMover *pAttacker, int nReflect )
{
	if( m_nDuel == 1 )
	{
		if( GetId() != pAttacker->m_idDuelOther )
		{
			FLWSUser* pOther	= prj.GetUser( m_idDuelOther );
			if( IsValidObj( (CObj*)pOther ) )
			{
				pOther->AddDuelCancel( pOther->m_idDuelOther );
				pOther->ClearDuel();
			}
			( (FLWSUser*)this )->AddDuelCancel( m_idDuelOther );
			ClearDuel();
			//return;	// mirchang 100114 ��� �� �ٸ� �������� PK�� ���Ѱ�� SubPK() �� ������..
		}
	}
	else if( m_nDuel == 2 )
	{
		if( m_idDuelParty != pAttacker->m_idparty || pAttacker->m_nDuelState == 300 )
		{
			CParty *pLoseParty = g_PartyMng.GetParty( m_idparty );

			if( pLoseParty && pLoseParty->IsLeader( m_idPlayer ) )
			{
				g_DPCoreClient.SendSetPartyDuel( m_idDuelParty, pLoseParty->m_uPartyId, FALSE );

				CParty* pParty2	= g_PartyMng.GetParty( m_idDuelParty );
				if( pParty2 )
					pParty2->DoDuelPartyCancel( pLoseParty );
				pLoseParty->DoDuelPartyCancel( pParty2 );
			}
		}
	}

	PVP_MODE mode = GetPVPCase( pAttacker );
	switch( mode )
	{
	case PVP_MODE_GUILDWAR:
		SubWar( pAttacker );
		break;

	case PVP_MODE_NONE:						//sun: 8, // __S8_PK
	case PVP_MODE_PK:
		SubPK( pAttacker, nReflect );		// PK�� �����ƾ.
		break;
	case PVP_MODE_DUEL:
		SubDuel( pAttacker );
		break;
	}
}

void CMover::AddPartyMemberExperience( FLWSUser * pUser, EXPINTEGER nExp, int nFxp )
{
	if( nFxp )	// �������ġ�� 0�ΰ��� �ƹ� ó���� ���ص� �ȴ�.
	{
		if( pUser->AddFxp( nFxp ) )
		{
			// ���෹����!
			g_xWSUserManager->AddSetFlightLevel( pUser, pUser->GetFlightLv() );
		} else
		{
			// ���������� �ʰ� ����ġ�� �ö�.
			// �α� �����.
		}
		pUser->AddSetFxp( pUser->m_nFxp, pUser->GetFlightLv() );		// ����ڿ��� �������ġ/������ ����.
	}

	if( nExp > prj.m_aExpCharacter[pUser->m_nLevel].nLimitExp )
		nExp	= prj.m_aExpCharacter[pUser->m_nLevel].nLimitExp;

	if( pUser->AddExperience( nExp, TRUE, TRUE, TRUE ) )
		pUser->LevelUpSetting();
	else
		pUser->ExpUpSetting();

	pUser->AddSetExperience( pUser->GetExp1(), (WORD)pUser->m_nLevel, pUser->m_nSkillPoint, pUser->m_nSkillLevel );
}

// this�� ����������.
// this�� �߽����� �ݰ� fRange �̳��� �ִ� ����鿡�� ����ġ�� �й��Ѵ�.
void CMover::SubAroundExp( CMover *pAttacker, float fRange )
{
#define MAX_AROUND_USER		512

	D3DXVECTOR3	vPos = GetPos();
	D3DXVECTOR3 vDist;
	FLOAT		fDistSq;
	FLWSUser *pUser;
	FLWSUser *pList[ MAX_AROUND_USER ], **ptr;
	int		nMaxList = 0;
	int		i;

	FLTRACE_LOG( PROGRAM_NAME, _T( "������ ���� %s" ), pAttacker->GetName() );
	fRange *= fRange;		// Sq �������� �ٲ�.

	memset( pList, 0, sizeof(pList) );
	ptr = pList;

	// �ݰ泻�� �ִ� ������ �߷���.
	FOR_VISIBILITYRANGE( this )
	{
		pUser = USERPTR;
		vDist = vPos - pUser->GetPos();
		fDistSq = D3DXVec3LengthSq( &vDist );		// �����Ѱ� ���������� �Ÿ�Sq�� ���� 
		if( fDistSq <= fRange )				// �ݰ� fRange ���� �̳��� �ִ� �����
		{
			*ptr++ = pUser;
			nMaxList ++;
		}
	}	
	NEXT_VISIBILITYRANGE( this )

		if( nMaxList == 0 )
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "%s������ ������ ����." ), GetName() );
			return;
		}


		EXPINTEGER nExp		= GetProp()->nExpValue / (EXPINTEGER)nMaxList;	// 1�δ� �󸶾� ��еǾ� �ϴ°�.
		// �߷��� �����鿡�� ����ġ�� ��.
		ptr = pList;
		for( i = 0; i < nMaxList; i ++ )
		{
			pUser = *ptr++;

			if( nExp > prj.m_aExpCharacter[pUser->m_nLevel].nLimitExp )
				nExp	= prj.m_aExpCharacter[pUser->m_nLevel].nLimitExp;

			if( pUser->AddExperience( nExp, TRUE, TRUE, TRUE ) )
			{
				// ������ �ƴ�.
				g_xWSUserManager->AddSetLevel( pUser, (WORD)pUser->m_nLevel );		// pUser�� ����������� pUser�� ������ �ö��ٴ°� ����.
				((FLWSUser*)pUser)->AddSetGrowthLearningPoint( m_Stat.GetRemainStatPoint() );		// pUser���� GP�����Ȱ��� ����.
				g_dpDBClient.SendLogLevelUp( pUser, 1 );	// ������ �α�
				//sun: 11, ĳ���� ���� ����
				g_dpDBClient.SendUpdatePlayerData( pUser );

			}
			else
			{
				// ������ �ȵǰ� ��ġ�� �ö���.
				// ���� 5�̻��϶��� ����ġ ���� �α�_������ ���̺� �α׸� �����
				// ����ġ 20% ������ �α׸� ����
				int nNextExpLog = (int)(pUser->m_nExpLog/20 + 1) * 20;	
				int nExpPercent = (int)( GetExp1() * 100 / GetMaxExp1() );
				if( nExpPercent >= nNextExpLog )
				{
					pUser->m_nExpLog = nExpPercent;
					g_dpDBClient.SendLogLevelUp( this, 5 );
				}
			}
			// pUser���� ����ġ �ٲ�� ����
			((FLWSUser*)pUser)->AddSetExperience( pUser->GetExp1(), (WORD)pUser->m_nLevel, pUser->m_nSkillPoint, pUser->m_nSkillLevel );
		}
}

void CMover::AddKillRecovery()
{
	if( IsPlayer() == FALSE )	return; // this(Attacker)�� ���͸� ó������.

	int nHPPoint = GetParam( DST_KILL_HP, 0 );
	int nMPPoint = GetParam( DST_KILL_MP, 0 );
	int nFPPoint = GetParam( DST_KILL_FP, 0 );

	BOOL bSfx = FALSE;
	if( nHPPoint )
	{
		int nHP	   = GetPointParam( DST_HP );
		int nHPMax = GetMaxPoint( DST_HP );
		if( nHP + nHPPoint >= nHPMax )	// ����Ʈ �������� ���ٵǴ°� ó��.
			nHP = nHPMax;
		else
			nHP = nHP + nHPPoint;
		SetPointParam( DST_HP, nHP );
		bSfx = TRUE;
	}

	if( nMPPoint )
	{
		int nMP	   = GetPointParam( DST_MP );
		int nMPMax = GetMaxPoint( DST_MP );
		if( nMP + nMPPoint >= nMPMax )	// ����Ʈ �������� ���ٵǴ°� ó��.
			nMP = nMPMax;
		else
			nMP = nMP + nMPPoint;
		SetPointParam( DST_MP, nMP );
		bSfx = TRUE;
	}

	if( nFPPoint )
	{
		int nFP	   = GetPointParam( DST_FP );
		int nFPMax = GetMaxPoint( DST_FP );
		if( nFP + nFPPoint >= nFPMax )	// ����Ʈ �������� ���ٵǴ°� ó��.
			nFP = nFPMax;
		else
			nFP = nFP + nFPPoint;
		SetPointParam( DST_FP, nFP );
		bSfx = TRUE;
	}

	if( bSfx )
		g_xWSUserManager->AddCreateSfxObj( this, XI_INDEX( 1727, XI_KILL_RECOVERY ) );
}

// pDead�� ������ ����ġ ó��
int CMover::SubExperience( CMover *pDead )
{
	if( IsPlayer() == FALSE )	return 0;	// this(Attacker)�� ���͸� ó������.
	if( pDead->IsPlayer() )	return 0;			// �������� �÷��̾�� ����ġ ó�������� ����.

	MoverProp* pMoverProp	= pDead->GetProp();
	FLASSERT( pMoverProp );

	EXPFLOAT fExpValue = 0;
	float	fFxpValue = 0.0f;
#ifdef __S1108_BACK_END_SYSTEM
	//			fExpValue	= pMoverProp->nExpValue * static_cast<EXPFLOAT>( prj.m_fMonsterExpRate ) * static_cast<EXPFLOAT>( pMoverProp->m_fExp_Rate );
	//			fFxpValue	= pMoverProp->nFxpValue * prj.m_fMonsterExpRate * pMoverProp->m_fExp_Rate;
	fExpValue	= pMoverProp->nExpValue * static_cast<EXPFLOAT>( pMoverProp->m_fExp_Rate );
	fFxpValue	= pMoverProp->nFxpValue * pMoverProp->m_fExp_Rate;
#else // __S1108_BACK_END_SYSTEM
	//			fExpValue	= pMoverProp->nExpValue * static_cast<EXPFLOAT>( prj.m_fMonsterExpRate );
	// 			fFxpValue	= pMoverProp->nFxpValue * prj.m_fMonsterExpRate;
	fExpValue	= pMoverProp->nExpValue;
	fFxpValue	= pMoverProp->nFxpValue;
#endif // __S1108_BACK_END_SYSTEM

	if( IsAuthHigher( AUTH_ADMINISTRATOR ) )	// ��� �����϶�
	{
		if( IsMode( MODE_EXPUP_STOP ) )			// ����ġ ��� ���� ���¸�
			fExpValue = 0;						// ����ġ�� 0
	}

	AddExperienceKillMember( pDead, fExpValue, pMoverProp, fFxpValue );
	return 1;
}

BOOL CMover::IsValidArea( CMover* pMover, float fLength )
{
	float fDist;
	D3DXVECTOR3	vDist;
	if( IsValidObj( (CObj*)this) && IsValidObj( (CObj*)pMover ) && GetWorld() == pMover->GetWorld()/*2008.04.11 ���� �˻� �߰�*/
#ifdef __LAYER_1015
		&& GetLayer() == pMover->GetLayer() // 2008.12.26 ���̾� �˻�
#endif // __LAYER_1015
		)
	{
		vDist = pMover->GetPos() - GetPos();
		fDist = D3DXVec3LengthSq( &vDist );
		if( fDist < fLength * fLength )
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CMover::GetPartyMemberFind( CParty* pParty, FLWSUser* apMember[], int* nTotalLevel, int* nMaxLevel10, int* nMaxLevel, int* nMemberSize )
{
	FLWSUser* pUsertmp = NULL;
	D3DXVECTOR3	vDist;

	for( int i = 0 ; i < pParty->m_nSizeofMember ; i++ )	
	{
		pUsertmp	= g_xWSUserManager->GetUserByPlayerID( pParty->m_aMember[i].m_uPlayerId );
		if( IsValidArea( (CMover*)pUsertmp, 64.0f ) )
		{
			apMember[(*nMemberSize)++]	= pUsertmp;
			(*nTotalLevel)		+= pUsertmp->GetLevel();
			if( (*nMaxLevel10) < pUsertmp->GetLevel() )
			{
				(*nMaxLevel) = (*nMaxLevel10) = pUsertmp->GetLevel();
			}			
		}
	}

	if( 0 < (*nMaxLevel10) - 25 )
	{
		(*nMaxLevel10) -= 25;
	}
	else
	{
		(*nMaxLevel10) = 0;
	}

	if( (*nMemberSize) == 0 || (*nTotalLevel) == 0 )
	{
		return FALSE;
	}

	return TRUE;
}

void CMover::AddExperienceKillMember( CMover *pDead, EXPFLOAT fExpValue, MoverProp* pMoverProp, float fFxpValue )
{
	vector<OBJID>	adwEnemy;
	vector<int>		anHitPoint;
	DWORD	dwMaxEnemyHit	= 0;
	for( SET_OBJID::iterator it = pDead->m_idEnemies.begin(); it != pDead->m_idEnemies.end(); ++it )
	{
		adwEnemy.push_back( (*it).first );
		anHitPoint.push_back( (*it).second.nHit );
		dwMaxEnemyHit	+= (*it).second.nHit;
	}

	if( adwEnemy.size() > 1024 )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "enemy size is too big" ) );
	}

	if( dwMaxEnemyHit == 0 )		//
		return;

	for( DWORD j = 0; j < adwEnemy.size(); j++ )
	{
		if( adwEnemy[j] == 0 )		// ����
			continue;
		CMover* pEnemy	= prj.GetMover( adwEnemy[j] );
		if( IsValidObj( pEnemy ) && pDead->IsValidArea( pEnemy, 64.0f ) && pEnemy->IsPlayer() )		// �÷��̾�, ���� �˻�
		{
			DWORD dwHitPointParty	= 0;
			CParty* pParty	= g_PartyMng.GetParty( pEnemy->m_idparty );
			if( pParty && pParty->IsMember( pEnemy->m_idPlayer ) )
			{
				dwHitPointParty		= anHitPoint[j];
				for( DWORD k = j + 1 ; k < adwEnemy.size(); k++ )
				{
					if( adwEnemy[k] == 0 )
						continue;	// �ߺ� ó�� ��ŵ
					CMover* pEnemy2	= prj.GetMover( adwEnemy[k] );
					if( IsValidObj( pEnemy2 ) && pDead->IsValidArea( pEnemy2, 64.0f ) && pEnemy2->IsPlayer() )	// �÷��̾�, ���� �˻�
					{
						if( pEnemy->m_idparty == pEnemy2->m_idparty && pParty->IsMember( pEnemy2->m_idPlayer ) )	// ���� ��Ƽ ���� ���
						{
							dwHitPointParty		+= anHitPoint[k];
							adwEnemy[k]	= 0;	// �ߺ� ó�� ����
						}
					}
					else
					{
						adwEnemy[k]	= 0;		// ���� ���
					}
				}
			}
			if( dwHitPointParty > 0 )
				anHitPoint[j]	= dwHitPointParty;
			float fExpValuePerson = (float)( fExpValue * ( float( anHitPoint[j] ) / float( dwMaxEnemyHit ) ) );
			if( dwHitPointParty )		// �ش� ����ġ �й�
			{	
				int nTotalLevel		= 0;
				int nMaxLevel10	= 0;
				int nMaxLevel	= 0;
				int nMemberSize		= 0;
				FLWSUser* apMember[MAX_PTMEMBER_SIZE] = { 0 };
				//mem_set( apMember, 0, sizeof(apMember) );
				// 1. �ֺ� ��� �˻�
				if( pEnemy->GetPartyMemberFind( pParty, apMember, &nTotalLevel, &nMaxLevel10, &nMaxLevel, &nMemberSize ) == FALSE )
					break;

				fExpValuePerson *= CPCBang::GetInstance()->GetPartyExpFactor( apMember, nMemberSize );
				if( 1 < nMemberSize )	// ��Ƽ���� ���� ����	// ��Ƽ���� ����ġ �ֱ�
					pEnemy->AddExperienceParty( pDead, fExpValuePerson, pMoverProp, fFxpValue, pParty, apMember, &nTotalLevel, &nMaxLevel10, &nMaxLevel, &nMemberSize );
				else	// ȥ�ڼ� �ο������ ó��.
					pEnemy->AddExperienceSolo( fExpValuePerson, pMoverProp, fFxpValue, TRUE );
			}
			else
			{
				if( IsPlayer() )
					fExpValuePerson *= CPCBang::GetInstance()->GetExpFactor( static_cast<FLWSUser*>( this ) );

				pEnemy->AddExperienceSolo( fExpValuePerson, pMoverProp, fFxpValue, FALSE );
			}
		}
	}
}

void CMover::AddExperienceSolo( EXPFLOAT fExpValue, MoverProp* pMoverProp, float fFxpValue, BOOL bParty )
{
	if( g_eLocal.GetState( EVE_EVENT0214 ) && !bParty )	// �߷�Ÿ�� �̺�Ʈ �� �ַ� �÷��� �� ����ġ 1.5��
		fExpValue	*= static_cast<EXPFLOAT>( 1.5f );

	// ������ ���� ����ġ�� �ش�. ���� ������ ������� 70  %, ������ ������� 130 %
	int dw_Level = GetLevel() - (int)pMoverProp->dwLevel;
	if( dw_Level > 0 )
	{
		if( dw_Level >= 1 && dw_Level <= 5 )		// 1~5 ����
		{
			fExpValue	*= static_cast<EXPFLOAT>( 0.7f );
			fFxpValue	*= 0.7f;
		}
		else if( dw_Level >= 6 && dw_Level <= 10 )	// 6~10 ����
		{
			fExpValue	*= static_cast<EXPFLOAT>( 0.5f );
			fFxpValue	*= 0.5f;
		}
		else if( dw_Level >= 10 && dw_Level <= 14 )	// 10~14 ����
		{
			fExpValue	*= static_cast<EXPFLOAT>( 0.3f );
			fFxpValue	*= 0.3f;
		}
		else										// 5�̻� ����
		{
			fExpValue	*= static_cast<EXPFLOAT>( 0.1f );
			fFxpValue	*= 0.1f;
		}
	}

	EXPINTEGER	iLogExp		= GetExp1() * 100 / GetMaxExp1();
	iLogExp	/= 20;

	if( fExpValue > static_cast<EXPFLOAT>( prj.m_aExpCharacter[m_nLevel].nLimitExp ) )
		fExpValue	= static_cast<EXPFLOAT>( prj.m_aExpCharacter[m_nLevel].nLimitExp );

	// �ø� �������ġ�� ���ٸ� ������ص� �ȴ�.
	if( fFxpValue )
	{
		if( AddFxp( (int)fFxpValue ) )
		{
			g_xWSUserManager->AddSetFlightLevel( this, GetFlightLv() );
		}
		else
		{
			// �������ġ ȹ�� �α� ������!
		}
		( (FLWSUser*)this )->AddSetFxp( m_nFxp, GetFlightLv() );
	}

	FLTRACE_LOG( PROGRAM_NAME, _T( "Name : %s ������� : %I64d" ), this->GetName(), static_cast<EXPINTEGER>( fExpValue ) );
	FLTRACE_LOG( PROGRAM_NAME, _T( "�������� �ʿ���:%I64d" ), prj.m_aExpCharacter[m_nLevel+1].nExp1 - m_nExp1 );

	if( AddExperience( static_cast<EXPINTEGER>( fExpValue ), TRUE, TRUE, TRUE ) )	// lv up
		((FLWSUser*)this)->LevelUpSetting();
	else
		((FLWSUser*)this)->ExpUpSetting();

	( (FLWSUser*)this )->AddSetExperience( GetExp1(), (WORD)m_nLevel, m_nSkillPoint, m_nSkillLevel );
}

void CMover::AddExperienceParty( CMover *pDead, EXPFLOAT fExpValue, MoverProp* pMoverProp, float fFxpValue, CParty* pParty, FLWSUser* apMember[], int* nTotalLevel, int* nMaxLevel10, int* nMaxLevel, int* nMemberSize )
{
	// ���̳� �ֺ��� ��Ƽ������ �ְ� ���� ��Ƽ������ ����ġ�� ���� ��Ŵ
	float fFactor = GetExperienceReduceFactor( (int)pMoverProp->dwLevel, *nMaxLevel );
	fExpValue *= static_cast<EXPFLOAT>( fFactor );
	fFxpValue *= fFactor;

	// ���� ������ �ش��ѷ����� 5�̻� ���ͷ����� ������ ����ġ�� ����Ʈ�� �ȿö� - 20���� �����մϴ�(2016.05.03.)
	pParty->GetPoint( (*nTotalLevel), (*nMemberSize), pDead->GetLevel() );

	if( pParty->GetKind() == PARTY_KIND_EXTEND ) // ��ȸ�ش�
	{
		switch( pParty->m_nTroupsShareExp ) // ����ġ �й� ���
		{
		case 1 :	// �⿩�й�
			{
				AddExperiencePartyContribution( pDead, apMember, pParty, fExpValue, fFxpValue, (*nMemberSize), (*nMaxLevel10) );
			}
			break;
		case 2 :	// ���Ϻй�
			{
				// ���� ��ȹ�� ���� ����ȵ�
			}
			break;
		default:	// �⺻ �й�(�����й�)
			{
				AddExperiencePartyLevel( apMember, pParty, fExpValue, fFxpValue, (*nMemberSize), (*nMaxLevel10) );
			}
			break;
		}
	}
	else // �ܸ��ش�
	{
		// �ܸ��ش��� ��ȸ�ش��� ���� �й�� ����
		AddExperiencePartyLevel( apMember, pParty, fExpValue, fFxpValue, (*nMemberSize), (*nMaxLevel10) );
	}
}

// �شܽ� �⿩�й�
void CMover::AddExperiencePartyContribution( CMover *pDead, FLWSUser* apMember[], CParty* pParty, EXPFLOAT fExpValue, float /*fFxpValue*/, int nMemberSize, int nMaxLevel10 )
{
	// ȹ���� ����ġ = �ڽ��� Ÿ�ݿ� ���� ����ġ + ���ʽ� ����ġ + �ɼ� (���Ͱ���ġ * ��Ƽ���� ������� / 100 )
	// ���ʽ� ����ġ = ( ���� ����ġ * 0.15) *( �ش� �ο� -1 ) * ((�ڽ��Ƿ����� ����)/( ��Ƽ�� ������ ������ ������ ��))
	EXPINTEGER	nMemberExp;
	int nAttackMember = 0;
	float fMaxMemberLevel = 0.0f;
	for( int i = 0 ; i < nMemberSize ; i++ )
	{
//		float fContribution = 0;
		int nHit = pDead->GetEnemyHit( apMember[i]->GetId() );
		if( nHit )
		{
			++nAttackMember;
		}
		fMaxMemberLevel += ((float)apMember[i]->GetLevel() * (float)apMember[i]->GetLevel());
	}
	float fAddExp = 0.0f;		// ���ʽ� ����ġ
	fAddExp = (float)( ( fExpValue * 0.2f ) * ( pParty->m_nSizeofMember - 1 ) );
	float fFullParty = 0.0f;	// Ǯ��Ƽ ���ʽ� ����ġ
	if( nMemberSize == MAX_PTMEMBER_SIZE )
	{
		fFullParty = (float)( ( fExpValue * 0.1f ) );
	}
	float fOptionExp = 0.0f;	// �ɼ� ����ġ
	if( 1 < nAttackMember )
	{
		fOptionExp = (float)( (fExpValue * (float)nAttackMember / 100.0f ) );
	}

	float fAddEventExp = 0.0f;	// �ش� ����ġ �̺�Ʈ
	
	fAddEventExp = static_cast<float>( static_cast<float>( fExpValue ) * ( prj.m_EventLua.GetTroupeExpUpFactor( static_cast<DWORD>( nMemberSize ) ) - 1.0f ) );

	if( fAddEventExp < 0.0f )
	{
		fAddEventExp = 0.0f;
	}

	FLTRACE_LOG( PROGRAM_NAME, _T( "GetExp(Contribution) : %s -> %f" ), pParty->m_sParty, fExpValue );

	for( int i = 0 ; i < nMemberSize ; i++ )
	{
		float fContribution = 0;
		int nHit = pDead->GetEnemyHit( apMember[i]->GetId() );
		if( nHit )
		{
			fContribution = (float)nHit * 100 / (float)pDead->GetMaxHitPoint();
			if( 100 < fContribution )
			{
				fContribution = 100.0f;
			}
			if( nMaxLevel10 < apMember[i]->GetLevel() )
			{
				nMemberExp	= static_cast<EXPINTEGER>( ( fExpValue * ( fContribution / 100.0f ) ) + ( fAddExp * ( ((float)apMember[i]->GetLevel() * (float)apMember[i]->GetLevel()) / fMaxMemberLevel ) ) + fOptionExp + fFullParty + fAddEventExp );

				FLTRACE_LOG( PROGRAM_NAME, _T( "GetExp(Contribution) : %s -> %I64d" ), apMember[i]->GetName(), static_cast<EXPINTEGER>( nMemberExp ) );
				AddPartyMemberExperience( apMember[i], nMemberExp, 0 );
			}
		}
		else	// �Ѵ뵵 �ȶ������� ���ʽ� ��ġ�� �ɼ� ��ġ�� Ǯ��Ƽ���ʽ���ġ �� �ش�
		{
			if( nMaxLevel10 < apMember[i]->GetLevel() )
			{
				nMemberExp	= static_cast<EXPINTEGER>( ( fAddExp * ( ((float)apMember[i]->GetLevel() * (float)apMember[i]->GetLevel()) / fMaxMemberLevel ) ) + fOptionExp + fFullParty + fAddEventExp );

				FLTRACE_LOG( PROGRAM_NAME, _T( "GetExp(Contribution) : %s -> %I64d" ), apMember[i]->GetName(), static_cast<EXPINTEGER>( nMemberExp ) );
				AddPartyMemberExperience( apMember[i], nMemberExp, 0 );
			}
		}
	}
}

void CMover::AddExperiencePartyLevel( FLWSUser* apMember[], CParty* pParty, EXPFLOAT fExpValue, float /*fFxpValue*/, int nMemberSize, int nMaxLevel10 )
{
	EXPINTEGER nMemberExp;
	//	int	  nMemberFxp;
	float fMaxMemberLevel = 0.0f;
	for( int i = 0 ; i < nMemberSize ; i++ )
	{
		fMaxMemberLevel += ((float)apMember[i]->GetLevel() * (float)apMember[i]->GetLevel());
	}
	float fAddExp = 0.0f;
	fAddExp = (float)( ( fExpValue * 0.2f ) * ( nMemberSize - 1 ) );
	FLTRACE_LOG( PROGRAM_NAME, _T( "GetExp(Level) : %s -> %f" ), pParty->m_sParty, fExpValue );

	for( int i = 0 ; i < nMemberSize ; i++ )
	{
		if( nMaxLevel10 < apMember[i]->GetLevel() )
		{
			//[ ������ ����ġ + {(���� ����ġ * 0.15) * (�ش� �ο� - 1)} * (�ڽ��� ������ ����/��� ��Ƽ�� ���� ������ ��) ]
			nMemberExp = static_cast<EXPINTEGER>( ( ( fExpValue + fAddExp ) * ( ((float)apMember[i]->GetLevel() * (float)apMember[i]->GetLevel()) / fMaxMemberLevel ) ) ); //+ fFullAddExp );

			AddPartyMemberExperience( apMember[i], nMemberExp, 0 );
			FLTRACE_LOG( PROGRAM_NAME, _T( "GetExp(Level) : %s -> %I64d" ), apMember[i]->GetName(), static_cast<EXPINTEGER>( nMemberExp ) );
		}
	}
}

// ����ġ ���Ұ��� ����: ���� �ֺ��� ��Ƽ������ �ְ� ���� ��Ƽ���� ���̸� �������� ...
float CMover::GetExperienceReduceFactor( int nLevel, int nMaxLevel ) 
{
	float fFactor = 1.0f;
	int nDelta = nMaxLevel - nLevel;
	if( nDelta > 0 )
	{
		nDelta = min( nDelta, 9 );	// �ִ� 9

		if( g_xFlyffConfig->GetMainLanguage() == LANG_KOR )
		{
			float fFactors[10] = { 0.7f, 0.7f, 0.7f, 0.4f, 0.4f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f };
			return fFactors[ nDelta ];
		}
		else
		{
			float fFactors[10] = { 0.8f, 0.8f, 0.6f, 0.35f, 0.2f, 0.12f, 0.08f, 0.04f, 0.02f, 0.01f };
			return fFactors[ nDelta ];
		}
	}

	return fFactor;
}
#endif	// __WORLDSERVER not client




// ���������� ���Ǵ� �Ź���.
BOOL CMover::IsAttackAble( CObj *pObj )
{
	//sun: ��, ���� ������ ��۹��� ����... Neuz, World
	if( HasBuffByIk3(IK3_TEXT_DISGUISE) )
		return FALSE;

	BOOL bAble	= FALSE;	

//	CWorld* pWorld = GetWorld();
	if( pObj )
	{
		if( IsStateMode( STATE_BASEMOTION_MODE ) )	
		{
#ifdef __CLIENT
			g_DPlay.SendStateModeCancel( STATE_BASEMOTION_MODE, STATEMODE_BASEMOTION_CANCEL );
#else
			SetStateNotMode( STATE_BASEMOTION_MODE, STATEMODE_BASEMOTION_CANCEL );
			m_nReadyTime = 0;
			m_dwUseItemId = 0;
#endif 
			return FALSE;		
		}

		if( pObj->GetType() == OT_MOVER )
		{
#ifdef __CLIENT
			CWorld* pWorld = GetWorld();
			CMover *pMover = (CMover *)pObj;

			if( pMover->IsPlayer() )
			{
				if( g_eLocal.GetState( EVE_18 ) )
				{
					if( IsPVPTarget( pMover ) )			// pvp
					{
						if( m_nDuelState == 1 && !IsFly() && !pMover->IsFly() )
							bAble	= TRUE;
					}
					else if( IsWarTarget( pMover ) )	// war
						bAble	= TRUE;
					else if( IsSchoolTarget( pMover ) )	// event
						bAble	= TRUE;
					else if( IsArenaTarget( pMover ) )
						bAble	= TRUE;
					else if( IsEventArenaTarget( pMover ) )
						bAble	= TRUE;
					else								// pk
					{
						bAble	= IsPKAttackAble( pMover );
						if( bAble )
							( (CWndWorld *)g_WndMng.m_pWndWorld )->SetLastTarget( pMover->GetId() );
					}
				}

				//sun: 8, // 8�� ��� 061226 ma
				if( IsPVPTarget( pMover ) )			// pvp
				{
					if( m_nDuelState == 1 && !IsFly() && !pMover->IsFly() )
						bAble	= TRUE;
				}

				if( g_eLocal.GetState( EVE_GUILDCOMBAT ) )
				{
					if( pWorld->GetID() == WI_WORLD_GUILDWAR )
					{
						if( IsGuildCombatTarget( pMover ) ) // GuildCombat
							bAble = TRUE;
					}
				}
				//sun: 11, �ϴ��� ��� ����
				if( g_eLocal.GetState( EVE_GUILDCOMBAT1TO1 ) )
				{
					if( g_GuildCombat1to1Mng.IsPossibleMover( pMover ) && IsGuildCombatTarget( pMover ) )
						bAble = TRUE;
				}

			}
			else
			{
				bAble	= IsAttackAbleNPC( pMover );
			}
#else // __CLIENT			
			if( GetHitType( (CMover *)pObj, TRUE, 0 ) == HITTYPE_FAIL )
				bAble = FALSE;
			else
				bAble = TRUE;
#endif // __CLIENT
		}
		else
		{
			// no mover
			bAble	= TRUE;
		}

#ifdef __CLIENT
		if( !bAble )
			( (CWndWorld *)g_WndMng.m_pWndWorld )->m_bAutoAttack	= FALSE;
#endif
	}
	return bAble;
}

BOOL CMover::IsAttackAbleNPC( CMover* pNPC )
{
	//sun: ��, ���� ������ ��۹��� ����... Neuz, World
	if( HasBuffByIk3(IK3_TEXT_DISGUISE) )
		return FALSE;

	BOOL bAble = TRUE;
	if( pNPC->IsDie() )	// �̹� �������� ��Ŀ�� ������Ű�� ���̻� �������� ����.
	{
#ifdef __CLIENT
		g_WorldMng()->SetObjFocus( NULL );
#else
		if( IsPlayer() )
			((FLWSUser *)this)->AddSetTarget( NULL_ID );
#endif
		bAble = FALSE;
	} 
	else
	{
		MoverProp *pMoverProp = pNPC->GetProp();	

		if( pMoverProp->bKillable == 0 )	
			return FALSE;		// �����̺Ұ����� NPC�� ������ ���ݸ���.

		// ��ȭ���̸� ������.
		if( pNPC->IsPeaceful() == TRUE )	// ��ȭ���γ��� ���� �ȵ�
		{
			bAble = FALSE;
		}
		else
		{
			if( pMoverProp->dwClass == RANK_GUARD )					// �����϶��� ������ ���Ѵ�.  
			{
				if( pMoverProp->nChaotic < 0 && IsChaotic() == TRUE )			// ������ ���� ������ ���ݸ���.
					bAble = FALSE;
				else if( pMoverProp->nChaotic > 0 && IsChaotic() == FALSE )		// ������ ���� ������ ���ݸ���.
					bAble = FALSE;
			}

			if( m_pActMover->IsFly() && pMoverProp->dwFlying == 0 )	// �����ڰ� �������̰� Ÿ���� ������� �ƴϸ�
				bAble = FALSE;
			else if( m_pActMover->IsFly() == FALSE && pMoverProp->dwFlying == 1 )	// ������ ���� / Ÿ�� �����
				bAble = FALSE;
		}
	}

#ifdef __CLIENT
	if( bAble == TRUE )
		if( GetAsyncKeyState(VK_CONTROL) & 0x8000 )	// NPC���� ����Ű�� ������ ������ Ŭ���ϸ� �ڵ�����.
			((CWndWorld *)g_WndMng.m_pWndWorld)->m_bAutoAttack = TRUE;
#endif // Client

	return bAble;
}

#ifdef __CLIENT
BOOL CMover::IsPKAttackAble( CMover* pMover )
{
	if( NULL == pMover )
		return FALSE;

	if( IsArenaTarget( pMover ) )
		return TRUE;

	if( !g_eLocal.GetState( EVE_PK ) )
		return FALSE;
	return IsPKInspection( pMover );
}

BOOL CMover::IsPKInspection( CMover* pOther )
{
	DWORD dwRegionAttr	= GetPKPVPRegionAttr();
	BOOL bAble = TRUE;

	int nAttackerPK, nDefenderPK;
	nAttackerPK = nDefenderPK = 0;
	if( nAttackerPK = IsPKPVPInspectionBase( dwRegionAttr, FALSE ) )
		bAble	= FALSE;
	if( nDefenderPK = pOther->IsPKPVPInspectionBase( dwRegionAttr, FALSE ) )
		bAble	= FALSE;

	//sun: 8, // __S8_PK
	if( bAble == FALSE && ( pOther->IsChaotic() || (GetAsyncKeyState(VK_CONTROL) & 0x8000)) )
	{
		if(GetWorld() && GetWorld()->GetID() == WI_WORLD_GUILDWAR && IsGuildCombatTarget( pOther ) )	// ������ ���� �������Դ� �޼����� �ȳ����� ����
			nAttackerPK = nDefenderPK = 9;
		//sun: 11, �ϴ��� ��� ����
		else if( g_GuildCombat1to1Mng.IsPossibleMover( this ) )
			nAttackerPK = nDefenderPK = 9;
		else if(!IsFly() && !pOther->IsFly())
		{
			CWndWorld* pWndWorld	= (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
			if( pWndWorld && pWndWorld->m_bLButtonDown && !pWndWorld->m_bLButtonDowned )
			{
				if( nAttackerPK == 1 || nDefenderPK == 1 )
					g_WndMng.PutString( prj.GetText(TID_GMAE_PK_NOT_AREA), NULL, prj.GetTextColor(TID_GMAE_PK_NOT_AREA) );		
				else if( nAttackerPK == 5 )
					g_WndMng.PutString( prj.GetText(TID_GAME_PK_LIMIT_LEVEL0), NULL, prj.GetTextColor(TID_GAME_PK_LIMIT_LEVEL0) );
				else if( nDefenderPK == 5 )
					g_WndMng.PutString( prj.GetText(TID_GAME_PK_LIMIT_LEVEL1), NULL, prj.GetTextColor(TID_GAME_PK_LIMIT_LEVEL1) );
			}
		}		
	}

	if( bAble )
	{
		if( pOther->m_vtInfo.IsVendorOpen() )
		{
			if( IsForceAttack() )
			{
				;
			}
			else if( !( g_Neuz.m_NeuzEnemy.IsPKing( pOther->GetId() ) ) )
				bAble	= FALSE;
		}
		else
		{
			if( dwRegionAttr == RA_PENALTY_PK )
			{
				if( IsForceAttack() )
				{
					;
				}
				//sun: 8, // __S8_PK
				else if( !pOther->IsChaotic() )
				{
					bAble	= FALSE;
				}
			}
		}
	}

	return bAble;
}
#endif // CLIENT

#ifdef __WORLDSERVER
BOOL CMover::IsPVPInspection( CMover* pMover, int nFlag )
{
	//int nError	= 0;

	DWORD dwAttr	= GetPKPVPRegionAttr();
	//sun: 8,     // 8�� ������� ������� PVP�����ϰ���   Neuz, World
	DWORD dwWorldIDtmp = 0;
	DWORD dwDestWorldIDtmp = 1;

	if( GetWorld() && pMover->GetWorld() )
	{
		dwWorldIDtmp = GetWorld()->GetID();
		dwDestWorldIDtmp = pMover->GetWorld()->GetID();
	}

	switch( nFlag )
	{
	case 1:		// ���� PVP
		{
			//				1
			if( abs( GetLevel() - pMover->GetLevel() ) >= 30 )	// ���� 30�̻� ���°�
			{
				((FLWSUser*)this)->AddDefinedText( TID_PK_LEVEL_GAP, "" );	// ������ 30�̻� ���̰� ���� PVP�� �Ҽ� �����ϴ�
				return FALSE;
			}
			//				1	// 2	// 3	// 4	// 5

			//sun: 8,     // 8�� ������� ������� PVP�����ϰ���   Neuz, World
			int nError	= IsPVPInspectionBase( dwAttr,dwDestWorldIDtmp );
			if( nError == 0 )
			{
				nError = pMover->IsPVPInspectionBase( dwAttr ,dwWorldIDtmp);
			}
			if( nError != 0 )
			{
				switch( nError )
				{
				case 1:
					((FLWSUser*)this)->AddDefinedText( TID_PK_SAFETY_NO, "" );	// �������������� PVP�Ҽ� �����ϴ�
					break;
				case 2:
					((FLWSUser*)this)->AddDefinedText( TID_PK_SAME_NO, "" );	// ���������� �־�� PVP�� �Ҽ� �ֽ��ϴ�
					break;
				case 3:
					((FLWSUser*)this)->AddDefinedText( TID_PK_CHANGEJOB_NO, "" );	// 1������ ���Ŀ� PVP�� �Ҽ� �ֽ��ϴ�
					break;
				}
				return FALSE;
			}
			return TRUE;
		}
	case 2:		// �ش� PVP
		{
			CParty* pSrc	= g_PartyMng.GetParty( m_idparty );
			CParty* pDest	= g_PartyMng.GetParty( pMover->m_idparty );
			if( pSrc && pDest )
			{
//				DWORD dwTick	= GetTickCount();
				u_long anPlayer[MAX_PTMEMBER_SIZE*2];
				int	nSize	= 0;
				for( int i = 0; i < pSrc->GetSizeofMember(); i++ )
					anPlayer[nSize++]	= pSrc->GetPlayerId( i );
				for( int i = 0; i < pDest->GetSizeofMember(); i++ )
					anPlayer[nSize++]	= pDest->GetPlayerId( i );

				FLWSUser* pLeaderSrc	= (FLWSUser*)pSrc->GetLeader();
				FLWSUser* pLeaderDest	= (FLWSUser*)pDest->GetLeader();
				if( IsValidObj( (CObj*)pLeaderSrc ) && IsValidObj( (CObj*)pLeaderDest ) )
				{
					if( abs( pLeaderSrc->GetLevel() - pLeaderDest->GetLevel() ) >= 30 )
					{
						((FLWSUser*)this)->AddDefinedText( TID_PK_LEVEL_GAP, "" );	// ������ 30�̻� ���̰� ���� PVP�� �Ҽ� �����ϴ�
						return FALSE;
					}
				}
				else
				{
					//
					return FALSE;
				}

				for( int i = 0; i < nSize; i++ )
				{
					FLWSUser* pPlayer	= g_xWSUserManager->GetUserByPlayerID( anPlayer[i] );
					if( IsValidObj( (CObj*)pPlayer ) )
					{
						//sun: 8,     // 8�� ������� ������� PVP�����ϰ���   Neuz, World
						int nError  = pPlayer->IsPVPInspectionBase( dwAttr,dwWorldIDtmp );
						if( ( nError ) )
							//!							if( ( nError = pPlayer->IsPKPVPInspectionBase( dwAttr ) ) )
						{
							if( nError != 0 )
							{
								switch( nError )
								{
								case 1:
									((FLWSUser*)this)->AddDefinedText( TID_PK_SAFETY_NO, "" );	// �������������� PVP�Ҽ� �����ϴ�
									break;
								case 2:
									((FLWSUser*)this)->AddDefinedText( TID_PK_SAME_NO, "" );	// ���������� �־�� PVP�� �Ҽ� �ֽ��ϴ�
									break;
								case 3:
									((FLWSUser*)this)->AddDefinedText( TID_PK_CHANGEJOB_NO, "" );	// 1������ ���Ŀ� PVP�� �Ҽ� �ֽ��ϴ�
									break;
								}
								return FALSE;
							}
						}
					}
				}
				return TRUE;
			}
			return FALSE;
		}
	}
	return TRUE;
}


#endif	// __WORLDSERVER

int CMover::IsPKPVPInspectionBase( DWORD dwRegionAttr, BOOL bPVP )
{
	DWORD dwAttr = GetPKPVPRegionAttr();

	if( dwRegionAttr == RA_SAFETY )		// �������� �˻�
		return 1;
	if( dwRegionAttr != dwAttr )		// ���������� �ִ��� �˻�
		return 2;

#ifdef __CLIENT
	if( GetAsyncKeyState( VK_MENU ) & 0x8000 )
		return 9;
#endif	// __CLIENT

	if( bPVP && (dwAttr == RA_FIGHT || dwAttr != RA_PK) )	// PVP�� Penalty �����̸� �˻�
	{
		if( IsBaseJob() )
			return 3;
	}

	//sun: 8, // __S8_PK
	if( !bPVP && GetLevel() <= prj.m_PKSetting.nLimitLevel )	// PK
		return 5;

	if( bPVP )
	{
		if( m_nDuelState != 0 )
			return 9;
	}

	if( IsFly() )
		return 1;
	if( m_idWar > 0 )
		return 1;

	return 0;
}

//8�� ������� ������� PVP�����ϰ���   Neuz, World
//sun: 8,     // 8�� ������� ������� PVP�����ϰ���   Neuz, World
int CMover::IsPVPInspectionBase( DWORD /*dwRegionAttr*/,DWORD dwWorldID, BOOL bPVP )
{

#ifdef __CLIENT
	if( GetAsyncKeyState( VK_MENU ) & 0x8000 )
		return 9;
#endif	// __CLIENT


	if( IsBaseJob() )
		return 3;

	if( bPVP )
	{
		if( m_nDuelState != 0 )
			return 9;
	}

	if(GetWorld() && GetWorld()->GetID() == WI_WORLD_GUILDWAR)
		return	10;

	if(GetWorld() && GetWorld()->GetID() != dwWorldID)
		return	2;

	if( IsFly() )
		return 1;
	if( m_idWar > 0 )
		return 1;

	if( GetWorld() && GetWorld()->GetID() == WI_WORLD_QUIZ )
		return	1;

	return 0;
}



BOOL CMover::SubPKPVPInspectionBase( CMover* pMover, CMover* pMover1, DWORD dwPKAttr, int /*nFlag*/ )
{
	if( IsBaseJob() ) // ������ ������
	{
#ifdef __CLIENT
		g_WndMng.PutString( prj.GetText( TID_PK_CHANGEJOB_NO ), NULL, prj.GetTextColor( TID_PK_CHANGEJOB_NO ) );
#endif // __CLIENT
#ifdef __WORLDSERVER
		((FLWSUser*)this)->AddDefinedText( TID_PK_CHANGEJOB_NO, "" );	// 1������ ���Ŀ� PVP�� �Ҽ� �ֽ��ϴ�
		if( IsValidObj( pMover ) )
			((FLWSUser*)pMover)->AddDefinedText( TID_PK_CHANGEJOB_NO, "" );	// 1������ ���Ŀ� PVP�� �Ҽ� �ֽ��ϴ�
#endif // __WORLDSERVER
		//		pMover->PrintString( pMover1, TID_GAME_PVP_LOWLEVEL2 );
		return FALSE;
	}
	if( IsFly() )	// ���� ����
	{
#ifdef __CLIENT
		g_WndMng.PutString( prj.GetText( TID_PK_NOFLIGHT_DURING ), NULL, prj.GetTextColor( TID_PK_NOFLIGHT_DURING ) ); // �������̸� ����� �Ҽ� �����ϴ�
#endif // __CLIENT
#ifdef __WORLDSERVER
		((FLWSUser*)this)->AddDefinedText( TID_PK_NOFLIGHT_DURING, "" );	// �������̸� ����� �Ҽ� �����ϴ�
		if( IsValidObj( pMover ) )
			((FLWSUser*)pMover)->AddDefinedText( TID_PK_NOFLIGHT_DURING, "" );	// �������̸� ����� �Ҽ� �����ϴ�
#endif // __WORLDSERVER		
		//		pMover->PrintString( pMover1, TID_GAME_PVP_LOWLEVEL2 );
		return FALSE;
	}

#ifdef __WORLDSERVER
	if( m_idWar )	// �����߿� ���, PK ����.
	{
		if( g_eLocal.GetState( EVE_GUILDWAR ) )
		{
			pMover->PrintString( pMover1, TID_GAME_GUILDWARERRORDUEL );	// �����߿� ��� ���Դ�.
			return FALSE;
		}
	} 
	else
	{
		if( m_nDuelState != 0 )
		{
			return FALSE;
		}
	}
#endif __WORLDSERVER

	if( IsRegionAttr( dwPKAttr ) == FALSE )	// ���� ������ �ִ��� Ȯ��
	{
#ifdef __CLIENT
		g_WndMng.PutString( prj.GetText( TID_PK_SAME_NO ), NULL, prj.GetTextColor( TID_PK_SAME_NO ) );
#endif // __CLIENT
#ifdef __WORLDSERVER
		((FLWSUser*)this)->AddDefinedText( TID_PK_SAME_NO, "" );	// ���������� �־�� PVP�� �Ҽ� �ֽ��ϴ�
		if( IsValidObj( pMover ) )
			((FLWSUser*)pMover)->AddDefinedText( TID_PK_SAME_NO, "" );	// ���������� �־�� PVP�� �Ҽ� �ֽ��ϴ�
#endif // __WORLDSERVER	
		//		pMover->PrintString( pMover1, TID_GAME_PVP_SAFEZONENOATTACK );	//g_WndMng.PutString( prj.GetText( TID_GAME_PVP_SAFEZONENOATTACK ), NULL, prj.GetTextColor( TID_GAME_PVP_SAFEZONENOATTACK ) );
		return FALSE;
	}
	return TRUE;
}

void CMover::PrintString( CMover* pMover, DWORD dwId )
{
#ifdef __CLIENT
	g_WndMng.PutString( prj.GetText( dwId ), NULL, prj.GetTextColor( dwId ) );
#endif // __CLIENT
#ifdef __WORLDSERVER
	((FLWSUser*)this)->AddDefinedText( dwId, "" );
	if( IsValidObj( pMover ) )
		((FLWSUser*)pMover)->AddDefinedText( dwId, "" );
#endif // __WORLDSERVER
}
// this�� ������
// this�� pTarget�� ��ƿ�Ϸ����� �˻�.
// pTarget�� �ݵ�� IsValid üũ�� �ϰ� �Ѿ�;� �Ѵ�.
#ifdef __WORLDSERVER
int CMover::IsSteal( CMover *pTarget )
{
	if( IsNPC() || g_xFlyffConfig->GetMainLanguage() == LANG_JAP )	
		return 0;		// �����ڰ� NPC�� ��ƿ üũ��� �ƴ�.

	if( g_xFlyffConfig->GetMainLanguage() == LANG_THA && pTarget->GetIndex() == OBJECT_INDEX( 199, MI_MRPUMPKIN5 ) )	// �±��� ��� ���� �̽�����Ų�� ��ƿ�� ����.
		return 0;												

	if( pTarget->GetIndex() == OBJECT_INDEX( 196, MI_DEMIAN5 ) || pTarget->GetIndex() == OBJECT_INDEX( 197, MI_KEAKOON5 ) || pTarget->GetIndex() == OBJECT_INDEX( 198, MI_MUFFRIN5 ) )
		return 0;		// ����� �̺�Ʈ���̹Ƿ� �ƹ��� ��ƿ ����

	// �÷��̾ ����� ���´�.
	//sun: 8, // __S8_PK
	if( pTarget->IsNPC() )	// ������:�÷��̾�, �´���:NPC 
		//!	if( pTarget->IsNPC() || (pTarget->IsPlayer() && pTarget->IsChaotic() == FALSE) )	// ������:�÷��̾�, �´���:NPC Ȥ�� ���ѻ��. (�����ʹ� �ٱ������ص� �ȴ�).
	{
		BOOL bStealCheck = TRUE;
		if( pTarget->m_idTargeter == NULL_ID )	
		{
			return 0;	//Ÿ������ �������� ���� ������ ������ ��ƿ�ƴ�.
		}
		CMover *pOtherPlayer = prj.GetMover( pTarget->m_idTargeter );		// pTarget�� ���� �ٸ� ����.
		if( IsValidObj(pOtherPlayer) )
		{
			// �����ڰ� ��Ƽ�� �ְ� pTarget�� ���ȴ� ����� ���� ��Ƽ�� ��ƿ�� �ƴϴ�.
			if( m_idparty && (pOtherPlayer->m_idparty == m_idparty) )
				bStealCheck = FALSE;
			if( m_idWar && m_idGuild && (pOtherPlayer->m_idGuild == m_idGuild) ) // �����߿� �������� ��峢�� ġ�°� ��ƿ�� �ƴϴ�.
				bStealCheck = FALSE;
			if( g_eLocal.GetState( EVE_SCHOOL ) )
				bStealCheck	= FALSE;
			if( pTarget->GetProp()->dwClass == RANK_SUPER )		// �������� ��ƿ����.
				bStealCheck = FALSE;
		} else
			bStealCheck = FALSE;	// pTarget�� ���ȴ� ������ ��ȿ���� ������ ��ƿ�� �ƴϴ�.

		if( bStealCheck && 
			pTarget->m_idTargeter != NULL_ID && pTarget->m_idTargeter != GetId()) // pTarget�� ���ѳ��� �ְ� �װ� ��������ڰ� �ƴ϶�� ��ƿ.
		{
			((FLWSUser*)this)->AddDefinedText( TID_GAME_PRIORITYMOB, "" );	// ���Ͱ� �ٸ�ĳ���Ͷ� �������Ӵ�.
			return 1;		// ��ƿ.
		}
	}

	return 0;	// ��ƿ�� �ƴ�.
}


// 
// �׾����� ����ġ ��� �κ�.
//
// void  CMover::GetDieDecExp( int nLevel, FLOAT& fRate, FLOAT& fDecExp, BOOL& bPxpClear, BOOL& bLvDown )
// {
// 	//sun: 8,  
// 	bPxpClear = FALSE;
// 
// 	// HPȸ����ġ(%)
// 	DWORD	veci = 0;
// 	for( veci = 0 ; veci < prj.m_vecRevivalPenalty.size() ; ++veci )
// 	{
// 		if( veci == 0 && nLevel <= prj.m_vecRevivalPenalty[veci].nLevel )
// 			break;
// 		if( veci != 0 && prj.m_vecRevivalPenalty[veci-1].nLevel < nLevel && nLevel <= prj.m_vecRevivalPenalty[veci].nLevel )
// 			break;
// 	}
// 	if( prj.m_vecRevivalPenalty.size() <= veci )
// 	{
// 		veci = prj.m_vecRevivalPenalty.size()-1;
// 	}
// 	fRate = (float)prj.m_vecRevivalPenalty[veci].nValue / 10000.0f;
// 
// 	// ����ġ�ٿ�(%)
// 	for( veci = 0 ; veci < prj.m_vecDecExpPenalty.size() ; ++veci )
// 	{
// 		if( veci == 0 && nLevel <= prj.m_vecDecExpPenalty[veci].nLevel )
// 			break;
// 		if( veci != 0 && prj.m_vecDecExpPenalty[veci-1].nLevel < nLevel && nLevel <= prj.m_vecDecExpPenalty[veci].nLevel )
// 			break;
// 	}
// 	if( prj.m_vecDecExpPenalty.size() <= veci )
// 	{
// 		veci = prj.m_vecDecExpPenalty.size()-1;
// 	}
// 	fDecExp = (float)prj.m_vecDecExpPenalty[veci].nValue / 10000.0f;
// 
// 	// �����ٿ�(��/��)
// 	for( veci = 0 ; veci < prj.m_vecLevelDownPenalty.size() ; ++veci )
// 	{
// 		if( veci == 0 && nLevel <= prj.m_vecLevelDownPenalty[veci].nLevel )
// 			break;
// 		if( veci != 0 && prj.m_vecLevelDownPenalty[veci-1].nLevel < nLevel && nLevel <= prj.m_vecLevelDownPenalty[veci].nLevel )
// 			break;
// 	}
// 	if( prj.m_vecLevelDownPenalty.size() <= veci )
// 	{
// 		veci = prj.m_vecLevelDownPenalty.size()-1;
// 	}
// 	bLvDown = (prj.m_vecLevelDownPenalty[veci].nValue)? TRUE : FALSE;
// 
// }
// 
// void CMover::GetDieDecExpRate( FLOAT& fDecExp, DWORD dwDestParam, BOOL bResurrection )
// {
// 	// �ε彺Ÿ/����Ʈ�� ��Ȱ�Ѱ��� ����ġ �϶�.
// 	if( dwDestParam != 0 )
// 	{
// 		FLOAT fAddDec = (float)(100 - dwDestParam) / 100.0f;
// 		fDecExp = fDecExp - (fDecExp * fAddDec);
// 	}
// 
// 	if( IsChaotic() && ( IsSMMode( SM_REVIVAL ) || bResurrection ) )
// 		fDecExp = fDecExp * 0.9f;
// 	else if( IsSMMode( SM_REVIVAL ) )
// 		fDecExp = 0.0f;
// }
// 
// float CMover::SubDieDecExp( BOOL bTransfer, DWORD dwDestParam, BOOL bResurrection )
// {
// 	if( IsNPC() )	
// 		return 0.0f;
// 
// 	float fRate = 0.1f, fDecExp = 0.0f;
// 	BOOL  bPxpClear = FALSE, bLvDown = FALSE;
// 	int nLevel	= GetLevel();
// 
// 	GetDieDecExp( nLevel, fRate, fDecExp, bPxpClear, bLvDown );
// 
// 	if( m_bLastPK )		// ������ ����ġ �ȱ�´�...
// 		return fRate;
// 
// 	if( m_bGuildCombat )
// 		return fRate;
// 
// 	if( m_bLastDuelParty )
// 	{
// 		m_bLastDuelParty = FALSE;
// 		return fRate;
// 	}
// 
// 	if( fDecExp )
// 	{
// 		if( IsAfterDeath() == FALSE ) 
// 		{
// 			m_nDeathExp = m_nExp1;		// ���� ����ġ�� �׾��� �� ����ġ�� ��� 
// 			m_nDeathLevel = m_nLevel;	// ���� ���� ��� 
// 		}
// 
// 		GetDieDecExpRate( fDecExp, dwDestParam, bResurrection );
// 
// 		if( bResurrection == FALSE && IsSMMode( SM_REVIVAL ) )
// 			((FLWSUser*)this)->SetSMMode( SM_REVIVAL, 0 );
// 
// 		bLvDown = DecExperiencePercent( fDecExp, bPxpClear, bLvDown );
// 
// 		if( bTransfer )
// 		{
// 			if( bLvDown )	// ���� �ٿ��� �Ǿ��°�?
// 			{
// 				g_xWSUserManager->AddSetLevel( this, (WORD)m_nLevel );		// this�̿��� �ֺ��� �������������� ����.
// 				((FLWSUser *)this)->AddSetGrowthLearningPoint( m_Stat.GetRemainStatPoint() );		// �ش��������� GP���������� ����.
// 
// 				g_dpDBClient.SendUpdatePlayerData( static_cast<FLWSUser*>( this ) );		// �÷��̾� ������ ����
// 			}
// 
// 			( (FLWSUser *)this )->AddSetExperience( GetExp1(), (WORD)m_nLevel,  m_nSkillPoint, m_nSkillLevel, m_nDeathExp, (WORD)m_nDeathLevel );		// �ش��������� exp1,exp2����� ������ ����.
// 
// 			g_pMadrigalGift->OnLevel( static_cast<FLWSUser*>( this ) );
// 		}
// 	}
// 
// 	return fRate;	// ��Ȱ�Ҷ� ���� HP�� ȸ���Ҳ���.
// }

#endif // WORLDSERVER



BOOL CMover::DropItemByDied( CMover* pAttacker )
{
	BOOL bResult	= FALSE;
	OBJID objid = GetMaxEnemyHitID();
	if( objid != NULL_ID )
	{
		CMover* pMover	= prj.GetMover( objid );
		if( IsValidObj( pMover ) )
			bResult = DropItem( pMover );
		else
			objid = NULL_ID;
	}

	if( objid == NULL_ID )
		bResult = DropItem( pAttacker );

	return bResult;
}



BOOL CMover::OnDie( CMover & kAttacker )
{
#ifdef __WORLDSERVER
	if( GetProp() == NULL )
		return FALSE;
#endif //__WORLDSERVER

	CMover * pMaxHiter		= &kAttacker;
	const OBJID objid		= GetMaxEnemyHitID();
	if( objid != NULL_ID )
	{
		CMover * pMover		= prj.GetMover( objid );
		if( IsValidObj( pMover ) == TRUE )
			pMaxHiter		= pMover;
	}

#ifdef __WORLDSERVER

	// ��Ʈ�� �ð� ����
	if( g_xFlyffConfig->GetMainLanguage() == LANG_VTN )
	{
		if( pMaxHiter->m_nAccountPlayTime > MIN( 300 ) )
			return TRUE;
	}

	if( m_nQuestKeeping > 0 )
	{
		CGuildQuestProcessor* pProcessor	= CGuildQuestProcessor::GetInstance();
		GUILDQUESTELEM* pElem	= pProcessor->GetGuildQuest( m_nQuestKeeping );
		if( pElem && pElem->objidWormon == GetId() )
		{
			CGuild* pGuild		= pMaxHiter->GetGuild();
			if( pGuild )
			{
				pGuild->SetQuest( pElem->nId, pElem->ns );
				g_dpDBClient.SendUpdateGuildQuest( pGuild->m_idGuild, pElem->nId, pElem->ns );
				pElem->nProcess		= GQP_GETITEM;
				pElem->dwEndTime	= GetTickCount() + MIN( 20 );
				pElem->ns			= pElem->nf	= 0;
				pElem->nState		= 0;
				pElem->objidWormon	= NULL_ID;
			}
		}
	}

	if( m_nPartyQuestKeeping > 0 ) //��Ƽ����Ʈ �������϶�? ��Ƽ����Ʈ�� Ű���ؾ��Ҷ�?
	{
		CPartyQuestProcessor* pProcessor	= CPartyQuestProcessor::GetInstance();
		PARTYQUESTELEM* pElem			= pProcessor->GetPartyQuest( m_nPartyQuestKeeping );
		if( pElem && pElem->objidWormon == GetId() )
		{
			CParty* pParty	= g_PartyMng.GetParty( pMaxHiter->m_idparty );				
			if( pParty )
			{
				pElem->nProcess		= PQP_GETITEM;
				pElem->dwEndTime	= GetTickCount() + MIN( 20 );
				pElem->ns			= pElem->nf	= 0;
				pElem->nState		= 0;
				pElem->objidWormon	= NULL_ID;

				pProcessor->SendQuestLimitTime( PQP_GETITEM, MIN( 20 ), pMaxHiter->m_idparty );
			}
		}
	}

#endif //__WORLDSERVER

	return DropItem( *pMaxHiter );

	//OBJID objid				= GetMaxEnemyHitID();
	//if( objid != NULL_ID )
	//{
	//	CMover * pMover		= prj.GetMover( objid );
	//	if( IsValidObj( pMover ) )
	//		return DropItem( *pMover );
	//	else
	//		objid			= NULL_ID;
	//}

	//if( objid == NULL_ID )
	//	return DropItem( kAttacker );

	//return FALSE;
}

BOOL CMover::DropItem( CMover & kAttacker )
{
#ifdef __WORLDSERVER
	if( GetProp() == NULL )
		return FALSE;

	if( IsNPC() == FALSE )
		return FALSE;

	if( kAttacker.IsPlayer() == FALSE )
		return FALSE;



	// event drop
	EVENTDROPITEM_MNG().DropItem( *this, static_cast< FLWSUser & >( kAttacker ) );

	// level area drop
	LEVELDROPITEM_MNG().DropItem( *this, static_cast< FLWSUser & >( kAttacker ) );

	// monster drop
	int nLoop				= 1 + FLDoDrop::GetAdditinalLoopCount( static_cast< FLWSUser & >( kAttacker ) );
	while( nLoop-- )
	{
		MONSTERDROPITEM_MNG().DropItemNGold( *this, static_cast< FLWSUser & >( kAttacker ) );
	}


	// NPC�� ���� �������� �b�´�.
//	MoverProp* pMoverProp	= GetProp();
	if( GetProp()->m_nLoot == 2 )	// 2�� d�ɼ�.  ������ �԰� ������ �ϴ� �ɼ�.
	{
		DWORD dwStart = m_Inventory.GetMax() - m_Inventory.GetSize();
		const DWORD dwSize = m_Inventory.GetMax();
		
		for( DWORD i = dwStart; i < dwSize; ++i)
		{
			FLItemElem *pElem	= m_Inventory.GetAt( i );
			if( pElem )
			{
				D3DXVECTOR3 vPos = GetPos();
				vPos.x			+= ( xRandomF(2.0f) - 1.0f );
				vPos.z			+= ( xRandomF(2.0f) - 1.0f );

				CItem *pItem	= DropItem( i, pElem->m_nItemNum, vPos, FALSE );
				pItem->m_idOwn	= kAttacker.GetId();
				pItem->m_dwDropTime		= timeGetTime();
			}
		}
	}
#endif //__WORLDSERVER
	return TRUE;
}

#ifdef __WORLDSERVER
bool	CMover::IsForceItemDropMonster( CMover* pAttacker )
{
	if( IsPlayer() == FALSE && pAttacker != NULL && pAttacker->IsPlayer() != FALSE )
	{
// 		// mirchang_110628 �ݷμ��򿡼� ���� ���ʹ� �������̿� ���� ����� ���ϰ� ������ ����ó��..
// 		if( CInstanceDungeonHelper::GetInstance()->GetDungeonKind( static_cast<FLWSUser*>( pAttacker ) ) == IDKIND_COLOSSEUM )
// 		{
// 			return true;
// 		}

		DWORD dwIndex = GetIndex();

		if( dwIndex == OBJECT_INDEX( 164, MI_CLOCKWORK1 )
			|| dwIndex == OBJECT_INDEX( 196, MI_DEMIAN5 )
			|| dwIndex == OBJECT_INDEX( 197, MI_KEAKOON5 )
			|| dwIndex == OBJECT_INDEX( 198, MI_MUFFRIN5 )
			|| dwIndex == OBJECT_INDEX( 1391, MI_COLOBANG_1 )
			|| dwIndex == OBJECT_INDEX( 1392, MI_COLOWAGJAK_1 )
			|| dwIndex == OBJECT_INDEX( 1393, MI_COLOREDMANTIS_1 )
			|| dwIndex == OBJECT_INDEX( 1394, MI_COLOJACKTHEHAMMER_1 )
			|| dwIndex == OBJECT_INDEX( 1395, MI_COLOBERKRO_1 )
			|| dwIndex == OBJECT_INDEX( 1396, MI_COLOHOWBOW_1 )
			|| dwIndex == OBJECT_INDEX( 1397, MI_COLOROKEPELER_1 )
			|| dwIndex == OBJECT_INDEX( 1398, MI_COLOSTIMEWORK_1 )
			|| dwIndex == OBJECT_INDEX( 1400, MI_COLOGRIMONG_1 )
			|| dwIndex == OBJECT_INDEX( 1399, MI_COLOGRREUNG_1 )
			|| dwIndex == OBJECT_INDEX( 1401, MI_COLOLUIA_1 )
			|| dwIndex == OBJECT_INDEX( 1402, MI_COLOGONGRI_1 )
			|| dwIndex == OBJECT_INDEX( 1403, MI_COLOKEREUN_1 )
			|| dwIndex == OBJECT_INDEX( 1436, MI_GPOTATO02 )
			|| dwIndex == OBJECT_INDEX( 1410, MI_COLOBIGFOOT_1 )
			|| dwIndex == OBJECT_INDEX( 1412, MI_COLOLYCANOS_1 )
			|| dwIndex == OBJECT_INDEX( 1411, MI_COLOLUCIFER_1 )
			|| dwIndex == OBJECT_INDEX( 1413, MI_COLOSOULVAMPAIN_1 )
			|| dwIndex == OBJECT_INDEX( 1415, MI_COLOTUTTLEKING_1 )
			|| dwIndex == OBJECT_INDEX( 1414, MI_COLOSKELGENERAL_1 )
			|| dwIndex == OBJECT_INDEX( 1420, MI_COLOSKELDEVIL_1 )
			|| dwIndex == OBJECT_INDEX( 1416, MI_COLORYCANBARGA_1 )
			|| dwIndex == OBJECT_INDEX( 1417, MI_COLOANGRYBEHEMOTH_1 )
			|| dwIndex == OBJECT_INDEX( 1432, MI_GPOTATO01 )
			|| dwIndex == OBJECT_INDEX( 1359, MI_COLOBANG )
			|| dwIndex == OBJECT_INDEX( 1360, MI_COLOWAGJAK )
			|| dwIndex == OBJECT_INDEX( 1361, MI_COLOREDMANTIS )
			|| dwIndex == OBJECT_INDEX( 1362, MI_COLOJACKTHEHAMMER )
			|| dwIndex == OBJECT_INDEX( 1363, MI_COLOBERKRO )
			|| dwIndex == OBJECT_INDEX( 1364, MI_COLOHOWBOW )
			|| dwIndex == OBJECT_INDEX( 1365, MI_COLOROKEPELER )
			|| dwIndex == OBJECT_INDEX( 1366, MI_COLOSTIMEWORK )
			|| dwIndex == OBJECT_INDEX( 1368, MI_COLOGRIMONG )
			|| dwIndex == OBJECT_INDEX( 1367, MI_COLOGRREUNG )
			|| dwIndex == OBJECT_INDEX( 1369, MI_COLOLUIA )
			|| dwIndex == OBJECT_INDEX( 1370, MI_COLOGONGRI )
			|| dwIndex == OBJECT_INDEX( 1371, MI_COLOKEREUN )
			|| dwIndex == OBJECT_INDEX( 1378, MI_COLOBIGFOOT )
			|| dwIndex == OBJECT_INDEX( 1380, MI_COLOLYCANOS )
			|| dwIndex == OBJECT_INDEX( 1379, MI_COLOLUCIFER )
			|| dwIndex == OBJECT_INDEX( 1381, MI_COLOSOULVAMPAIN )
			|| dwIndex == OBJECT_INDEX( 1383, MI_COLOTUTTLEKING )
			|| dwIndex == OBJECT_INDEX( 1382, MI_COLOSKELGENERAL )
			|| dwIndex == OBJECT_INDEX( 1388, MI_COLOSKELDEVIL )
			|| dwIndex == OBJECT_INDEX( 1384, MI_COLORYCANBARGA )
			|| dwIndex == OBJECT_INDEX( 1385, MI_COLOANGRYBEHEMOTH )
			)
		{
			return true;
		}
	}

	return false;
}
#endif // __WORLDSERVER


// ���� �׾ �������� ����Ҷ� ���
// Ŭ��� �������� �ȵ��´�.
// �׾ ������ ����Ʈ���³��� this��.
BOOL CMover::DropItem( CMover* pAttacker )
{
	MoverProp* lpMoverProp	= GetProp();
#ifdef __WORLDSERVER
	if( pAttacker->IsPlayer() && IsNPC() )
	{
		//	mulcom	BEGIN100315	��Ʈ�� �ð� ����
 		if( g_xFlyffConfig->GetMainLanguage() == LANG_VTN )
 		{
			if( pAttacker->m_nAccountPlayTime > MIN( 300 ) )
			{
				return	TRUE;
			}
		}
		//	mulcom	END100315	��Ʈ�� �ð� ����

		if( !lpMoverProp )
			return FALSE;

		if( m_nQuestKeeping > 0 )
		{
			CGuildQuestProcessor* pProcessor	= CGuildQuestProcessor::GetInstance();
			GUILDQUESTELEM* pElem	= pProcessor->GetGuildQuest( m_nQuestKeeping );
			if( pElem && pElem->objidWormon == GetId() )
			{
				CGuild* pGuild	= pAttacker->GetGuild();
				if( pGuild )
				{
					pGuild->SetQuest( pElem->nId, pElem->ns );
					g_dpDBClient.SendUpdateGuildQuest( pGuild->m_idGuild, pElem->nId, pElem->ns );
					pElem->nProcess	= GQP_GETITEM;
					pElem->dwEndTime	= GetTickCount() + MIN( 20 );
					pElem->ns	= pElem->nf	= 0;
					pElem->nState	= 0;
					pElem->objidWormon	= NULL_ID;
				}
			}
		}

		if( m_nPartyQuestKeeping > 0 ) //��Ƽ����Ʈ �������϶�? ��Ƽ����Ʈ�� Ű���ؾ��Ҷ�?
		{
			CPartyQuestProcessor* pProcessor	= CPartyQuestProcessor::GetInstance();
			PARTYQUESTELEM* pElem	= pProcessor->GetPartyQuest( m_nPartyQuestKeeping );
			if( pElem && pElem->objidWormon == GetId() )
			{
				CParty* pParty	= g_PartyMng.GetParty( pAttacker->m_idparty );				
				if( pParty )
				{
					pElem->nProcess	= PQP_GETITEM;
					pElem->dwEndTime	= GetTickCount() + MIN( 20 );
					pElem->ns	= pElem->nf	= 0;
					pElem->nState	= 0;
					pElem->objidWormon	= NULL_ID;

					pProcessor->SendQuestLimitTime( PQP_GETITEM, MIN( 20 ), pAttacker->m_idparty );
				}

			}
		}

		if( ( pAttacker->m_nLevel - (int)lpMoverProp->dwLevel ) < 10 )
		{
			int nNum	= 0;
			CEventItem* pEventItem	= CEventGeneric::GetInstance()->GetItem( &nNum );
			while( pEventItem )
			{
				if( lpMoverProp->dwFlying )	//
				{
					FLItemElem itemElem;
					itemElem.m_dwItemId	= pEventItem->m_dwItemId;
					itemElem.m_nItemNum		= nNum;
					itemElem.SetSerialNumber();
					if( pAttacker->CreateItem( &itemElem ) == TRUE )
					{
						PT_ITEM_SPEC pItemProp	= itemElem.GetProp();
						if( pItemProp )
							( (FLWSUser*)pAttacker )->AddDefinedText( TID_GAME_REAPITEM, "\"%s\"", pItemProp->szName );
					}
				}
				else
				{
					FLItemElem* pItemElem	= new FLItemElem;
					pItemElem->m_dwItemId	= pEventItem->m_dwItemId;
					pItemElem->m_nItemNum	= nNum;
					pItemElem->m_nHitPoint	= 0;
					pItemElem->SetSerialNumber();
					CItem* pItem	= new CItem;
					pItem->m_pItemBase	= pItemElem;
					pItem->m_idOwn	= pAttacker->GetId();
					pItem->m_dwDropTime		= timeGetTime();
					pItem->m_bDropMob	= TRUE;
					pItem->SetIndex( D3DDEVICE, pItem->m_pItemBase->m_dwItemId );
					D3DXVECTOR3 vPos = GetPos();
					vPos.x += ( xRandomF(2.0f) - 1.0f );
					vPos.z += ( xRandomF(2.0f) - 1.0f );
					vPos.y = GetPos().y;
					pItem->SetPos( vPos );
					if( GetWorld()->ADDOBJ( pItem, TRUE, GetLayer() ) == FALSE )
					{
						SAFE_DELETE( pItem );
					}
				}
				pEventItem	= CEventGeneric::GetInstance()->GetItem( &nNum );
			}
			//sun: 9, �̺�Ʈ (��� ��ũ��Ʈ ����)
			map<DWORD, int> mapItemList = prj.m_EventLua.GetItem( lpMoverProp->dwLevel );

			for( map<DWORD, int>::iterator it=mapItemList.begin(); it!=mapItemList.end(); it++ )
			{
				if( lpMoverProp->dwFlying )	//
				{
					FLItemElem itemElem;
					itemElem.m_dwItemId	= it->first;
					itemElem.m_nItemNum	= it->second;
					itemElem.SetSerialNumber();
					if( pAttacker->CreateItem( &itemElem ) == TRUE )
					{
						PT_ITEM_SPEC pItemProp	= itemElem.GetProp();
						if( pItemProp )
							( (FLWSUser*)pAttacker )->AddDefinedText( TID_GAME_REAPITEM, "\"%s\"", pItemProp->szName );
					}
				}
				else
				{
					FLItemElem* pItemElem	= new FLItemElem;
					pItemElem->m_dwItemId	= it->first;
					pItemElem->m_nItemNum	= it->second;
					pItemElem->m_nHitPoint	= 0;
					pItemElem->SetSerialNumber();
					CItem* pItem	= new CItem;
					pItem->m_pItemBase	= pItemElem;
					pItem->m_idOwn	= pAttacker->GetId();
					pItem->m_dwDropTime		= timeGetTime();
					pItem->m_bDropMob	= TRUE;
					pItem->SetIndex( D3DDEVICE, pItem->m_pItemBase->m_dwItemId );
					D3DXVECTOR3 vPos = GetPos();
					vPos.x += ( xRandomF(2.0f) - 1.0f );
					vPos.z += ( xRandomF(2.0f) - 1.0f );
					vPos.y = GetPos().y;
					pItem->SetPos( vPos );
					if( GetWorld()->ADDOBJ( pItem, TRUE, GetLayer() ) == FALSE )
					{
						SAFE_DELETE( pItem );
					}
				}
			}
		}

		// ����Ʈ ������ ���
		QUESTITEM* pQuestItem;
		int nNum;
		u_long uSizeOfQuestItem	= lpMoverProp->m_QuestItemGenerator.GetSize();
		for( u_long i = 0; i < uSizeOfQuestItem; i++ ) 
		{
			pQuestItem	= lpMoverProp->m_QuestItemGenerator.GetAt( i );
			LPQUEST lpQuest = pAttacker->GetQuest( pQuestItem->dwQuest );
			if( lpQuest && lpQuest->m_nState == pQuestItem->dwState ) 
			{
				DWORD dwProbability	= pQuestItem->dwProbability;

				if( pQuestItem->dwQuest == QUEST_INDEX( 51, QUEST_VALENTINE ) || pQuestItem->dwQuest == QUEST_INDEX( 52, QUEST_WHITEDAY ) )
				{
					float f;
					int  d	= pAttacker->m_nLevel - (int)lpMoverProp->dwLevel;
					if( d <= 1 )	f	= 1.0f;
					else if( d <= 2 )	f	= 0.8f;
					else if( d <= 4 )	f	= 0.6f;
					else if( d <= 7 )	f	= 0.3f;
					else	f	= 0.1f;
					dwProbability	= (DWORD)( dwProbability * f );
				}

				if( xRandom( 3000000000 ) <= dwProbability ) 
				{
					if( pQuestItem->dwNumber == 0 )
						FLERROR_LOG( PROGRAM_NAME, _T( "%s�� quest item drop %d��°�� dwNumber�� 0" ), GetName(), i );
					nNum	= (int)( xRandom( pQuestItem->dwNumber ) + 1 );
					if( pQuestItem->dwIndex == 0 )
					{
						FLERROR_LOG( PROGRAM_NAME, _T( "%s" ), lpMoverProp->szName );
						break;
					}

					CParty* pParty	= g_PartyMng.GetParty( pAttacker->m_idparty );
					if( pParty && pParty->IsMember( pAttacker->m_idPlayer ) )	// party
					{
						for( int j = 0; j < pParty->GetSizeofMember(); ++j )
						{
							PartyMember* pPartyMember	= &pParty->m_aMember[j];
							CMover* pMember		= prj.GetUserByID( pPartyMember->m_uPlayerId );
							if( !IsValidObj( pMember ) || !IsValidArea( pMember, 64.0f ) )
								continue;

							LPQUEST pMemberQuest	= pMember->GetQuest( pQuestItem->dwQuest );
							if( pMemberQuest && pMemberQuest->m_nState == pQuestItem->dwState )
							{
								DWORD adwItemObjID[MAX_INVENTORY] = { NULL_ID, };
								DWORD dwCount = 0;

								FLItemElem itemElem;
								itemElem.m_dwItemId		= pQuestItem->dwIndex;
								itemElem.m_nItemNum		= nNum;
								itemElem.m_nHitPoint	= 0;
								if( pMember->CreateItem( &itemElem, adwItemObjID, NULL, &dwCount ) == FALSE )
								{
									FLItemElem* pItemElem	= new FLItemElem;
									pItemElem->m_dwItemId	= pQuestItem->dwIndex;
									pItemElem->m_nItemNum	= nNum;
									PT_ITEM_SPEC pItemProp		= pItemElem->GetProp();
									if( pItemProp )
										pItemElem->m_nHitPoint		= ( pItemProp->dwEndurance == -1 ) ? 0 : pItemProp->dwEndurance;//pItemProp->dwEndurance;
									pItemElem->SetSerialNumber();
									CItem* pItem	= new CItem;
									pItem->m_pItemBase	= pItemElem;
									if( pItemElem->m_dwItemId == 0 )
										FLERROR_LOG( PROGRAM_NAME, _T( "DropItem:1st %s" ), GetName() );
									pItem->SetIndex( D3DDEVICE, pItemElem->m_dwItemId );
									pItem->SetPos( pMember->GetPos() );
									pItem->SetAngle( (float)( xRandom( 360 ) ) );
									pItem->m_idHolder	= pMember->m_idPlayer;
									pItem->m_dwDropTime		= timeGetTime();
									if( pMember->GetWorld()->ADDOBJ( pItem, TRUE, GetLayer() ) == FALSE )
									{
										SAFE_DELETE( pItem );
									}
								}
								else
								{
									( (FLWSUser*)pMember )->AddDefinedText( TID_EVE_REAPITEM, "\"%s\"", g_xSpecManager->GetSpecItem( pQuestItem->dwIndex )->szName );
									for( DWORD i = 0; i < dwCount; ++i )
									{
										FLItemBase* pItemBase	= pMember->GetItemId( adwItemObjID[ i ] );
										if( pItemBase != NULL )
										{
											FLItemElem* pItemElem	= (FLItemElem*)pItemBase;
											LogItemInfo aLogItem;
											//aLogItem.Action = "Q";
											//aLogItem.SendName = pMember->m_szName;
											//aLogItem.RecvName = "QUEST";
											FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "Q" );
											FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pMember->m_szName );
											FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "QUEST" );
											aLogItem.WorldId = pMember->GetWorld()->GetID();
											aLogItem.Gold = aLogItem.Gold2 = pMember->GetGold();
											g_DPSrvr.OnLogItem( aLogItem, pItemElem, pItemElem->m_nItemNum );
										}
									}
								}
							}
						}
					}
					else	// solo
					{
						DWORD adwItemObjID[MAX_INVENTORY] = { 0, };
						DWORD dwCount = 0;
						FLItemElem itemElem;
						itemElem.m_dwItemId		= pQuestItem->dwIndex;
						itemElem.m_nItemNum		= nNum;
						itemElem.m_nHitPoint	= 0;
						if( pAttacker->CreateItem( &itemElem, adwItemObjID, NULL, &dwCount ) == FALSE )
						{
							FLItemElem* pItemElem	= new FLItemElem;
							pItemElem->m_dwItemId	= pQuestItem->dwIndex;
							pItemElem->m_nItemNum	= nNum;
							PT_ITEM_SPEC pItemProp		= pItemElem->GetProp();
							if( pItemProp )
								pItemElem->m_nHitPoint		= ( pItemProp->dwEndurance == -1 ) ? 0 : pItemProp->dwEndurance;//pItemProp->dwEndurance;
							pItemElem->SetSerialNumber();
							CItem* pItem	= new CItem;
							pItem->m_pItemBase	= pItemElem;
							if( pItemElem->m_dwItemId == 0 )
								FLERROR_LOG( PROGRAM_NAME, _T( "DropItem:1st %s" ), GetName() );
							pItem->SetIndex( D3DDEVICE, pItemElem->m_dwItemId );
							pItem->SetPos( pAttacker->GetPos() );
							pItem->SetAngle( (float)( xRandom( 360 ) ) );
							pItem->m_idHolder	= pAttacker->m_idPlayer;
							pItem->m_dwDropTime		= timeGetTime();
							if( pAttacker->GetWorld()->ADDOBJ( pItem, TRUE, GetLayer() ) == FALSE )
							{
								SAFE_DELETE( pItem );
							}
						}
						else
						{
							( (FLWSUser*)pAttacker )->AddDefinedText( TID_EVE_REAPITEM, "\"%s\"", g_xSpecManager->GetSpecItem( pQuestItem->dwIndex )->szName );
							for( DWORD i = 0; i < dwCount; ++i )
							{
								FLItemBase* pItemBase	= pAttacker->GetItemId( adwItemObjID[ i ] );
								if( pItemBase != NULL )
								{
									FLItemElem* pItemElem	= (FLItemElem*)pItemBase;
									LogItemInfo aLogItem;
									//aLogItem.Action = "Q";
									//aLogItem.SendName = pAttacker->m_szName;
									//aLogItem.RecvName = "QUEST";
									FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "Q" );
									FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pAttacker->m_szName );
									FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "QUEST" );
									aLogItem.WorldId = pAttacker->GetWorld()->GetID();
									aLogItem.Gold = aLogItem.Gold2 = pAttacker->GetGold();
									g_DPSrvr.OnLogItem( aLogItem, pItemElem, pItemElem->m_nItemNum );
								}
							}
						}
					}
				}
			}
		}

		int nloop = 1;
		BOOL bUnique = FALSE;
		if( pAttacker->m_idparty ) // ���� ��Ƽ�� ���� ����
		{
			CParty* pParty = g_PartyMng.GetParty( pAttacker->m_idparty );
			if( pParty )
			{
				// ��ȸ�ش��̸� ��ũ�������̸� ���常 �������� �����ش�
				if( pParty->GetKind() == PARTY_KIND_EXTEND )
				{
					if( pParty->m_dwModeTime[PARTY_GIFTBOX_MODE] || pParty->m_dwModeTime[PARTY_FORTUNECIRCLE_MODE] )
					{
						//sun: 12, �Ľ�ų ������ ����
//						if( pParty->m_nModeTime[PARTY_PARSKILL_MODE] )
						if( pParty->IsActivatedEffect( PARTY_EFFECT_PARSKILLFULL ) == true )
						{
							if( pParty->m_dwModeTime[PARTY_GIFTBOX_MODE] )
								nloop = 2;
							if( pParty->m_dwModeTime[PARTY_FORTUNECIRCLE_MODE] )
								bUnique = TRUE;
						}
						else
						{
							FLWSUser* pLeader = g_xWSUserManager->GetUserByPlayerID( pParty->m_aMember[0].m_uPlayerId );
							if( IsValidObj( (CObj*)pLeader )/* && pLeader->IsNearPC( (FLWSUser*)pAttacker ) && IsOrigin()*/ )
							{
								if( pLeader->IsValidArea( pAttacker, 255.0f ) )
								{
									if( pParty->m_dwModeTime[PARTY_GIFTBOX_MODE] )
										nloop = 2;
									if( pParty->m_dwModeTime[PARTY_FORTUNECIRCLE_MODE] )
										bUnique = TRUE;
								}
							}
						}

					}
				}
			}
		}
		//sun: 9, // __II_SYS_SYS_SCR_GET
		if( pAttacker->HasBuff( BUFF_ITEM, ITEM_INDEX( 30115, II_SYS_SYS_SCR_GET01 ) ) )
			nloop += 1;
		if( pAttacker->HasBuff( BUFF_ITEM, ITEM_INDEX( 30116, II_SYS_SYS_SCR_GET02 ) ) )
			nloop += 2;

		nloop += pAttacker->GetAdjParam( DST_GIFTBOX );
		D3DXVECTOR3 vPos;		// ����� ��ġ.

#ifdef __EVENT_MONSTER
		// �̺�Ʈ ���ʹ� ������ ���� �ѹ��� ����Ѵ�.
		if( CEventMonster::GetInstance()->SetEventMonster( lpMoverProp->dwID ) )
			if( !CEventMonster::GetInstance()->IsGiftBoxAble() )
				nloop = 1;
#endif // __EVENT_MONSTER

		for( int k = 0 ; k < nloop ; k++ )
		{
			// ������ ��� Ȯ��		
			int nProbability	= 100, nPenyaRate	= 100;
			BOOL bAdjDropRate = TRUE;	// �Ϲ������� �������� ���� ��������ϰ� �̷�����.

			if( IsForceItemDropMonster( pAttacker ) != false )
			{
				bAdjDropRate = FALSE;
			}

#ifdef __EVENT_MONSTER
			if( CEventMonster::GetInstance()->SetEventMonster( lpMoverProp->dwID ) )
				bAdjDropRate = FALSE;
#endif // __EVENT_MONSTER
#ifdef __EVENTLUA_SPAWN
			if( prj.m_EventLua.IsEventSpawnMonster( lpMoverProp->dwID ) )
				bAdjDropRate = FALSE;
#endif // __EVENTLUA_SPAWN
			if( bAdjDropRate )
			{
				int d	= pAttacker->m_nLevel - (int)lpMoverProp->dwLevel;
				if( d <= 1 )	{	nProbability	= 100;	nPenyaRate	= 100;	}
				else if( d <= 2 )	{	nProbability	= 80;	nPenyaRate	= 100;	}
				else if( d <= 4 )	{	nProbability	= 60;	nPenyaRate	= 80;	}
				else if( d <= 7 )	{	nProbability	= 30;	nPenyaRate	= 65;	}
				else	{	nProbability	= 10;	nPenyaRate	= 50;	}
			}

			FLOAT fItemDropRate		= nProbability * GetItemDropRateFactor( pAttacker );

			if( xRandom( 100 ) < fItemDropRate )	// �������� ������� ���� ����. �������� ���� ���� �ƿ� ����Ʈ���� �ʴ´�.
			{
				int nSize	= lpMoverProp->m_DropItemGenerator.GetSize();
				int nNumber	= 0;
				DROPITEM* lpDropItem;

				for( int i = 0; i < nSize; i++ )
				{
					if( ( lpDropItem = lpMoverProp->m_DropItemGenerator.GetAt( i, bUnique, GetPieceItemDropRateFactor( pAttacker ) ) ) != NULL )
					{
						if( lpDropItem->dtType == DROPTYPE_NORMAL )
						{
							DWORD dwNumMax		= lpDropItem->dwNumber;
							if( dwNumMax == (DWORD)-1 )
								dwNumMax	= 1;

							const DWORD dwNumMin= ( lpDropItem->dwNumber2 == (DWORD)-1 ) ? 0 : lpDropItem->dwNumber2;

							if( lpMoverProp->dwFlying )
							{
								FLItemElem itemElem;
								itemElem.m_dwItemId	= lpDropItem->dwIndex;
								//itemElem.m_nItemNum	= (int)( xRandom( dwNumMax ) + 1 );
								itemElem.m_nItemNum	= (int)( xRandom( dwNumMax - dwNumMin ) + dwNumMin );
								itemElem.m_nItemNum	= max( 1, itemElem.m_nItemNum );	// �ּ� �ϳ��� ���ž��ϳ� �� ��

								itemElem.SetAbilityOption( lpDropItem->dwLevel );

								if( pAttacker->CreateItem( &itemElem ) == TRUE )
								{	// log
									PT_ITEM_SPEC pItemProp	= itemElem.GetProp();
									if( pItemProp )
									{
										( (FLWSUser*)pAttacker )->AddDefinedText( TID_GAME_REAPITEM, "\"%s\"", pItemProp->szName );
										if( pItemProp->dwItemKind1 == IK1_WEAPON || pItemProp->dwItemKind1 == IK1_ARMOR || ( pItemProp->dwItemKind1 == IK1_GENERAL && pItemProp->dwItemKind2 == IK2_JEWELRY ) )
										{
											switch( pItemProp->nLog )
											{
											case 1:		g_dpDBClient.SendLogUniqueItem2( pAttacker, itemElem.GetProp(), itemElem.GetAbilityOption() );	break;
											case 2:		g_dpDBClient.SendLogUniqueItem2( pAttacker, itemElem.GetProp(), 200 );	break;
											case 3:		g_dpDBClient.SendLogUniqueItem2( pAttacker, itemElem.GetProp(), 100 );	break;
											}
										}
									}
									if( lpDropItem->dwNumber != (DWORD)-1 )
										nNumber++;
									if(  (DWORD)( nNumber ) >= lpMoverProp->m_DropItemGenerator.m_dwMax )
										break;
								}
								continue;
							}
							FLItemElem* pItemElem	= new FLItemElem;
							pItemElem->m_dwItemId	= lpDropItem->dwIndex;
							//pItemElem->m_nItemNum	= (int)( xRandom( dwNumMax ) + 1 );
							pItemElem->m_nItemNum	= (int)( xRandom( dwNumMax - dwNumMin ) + dwNumMin );
							pItemElem->m_nItemNum	= max( 1, pItemElem->m_nItemNum );	// �ּ� �ϳ��� ���ž��ϳ� �� ��

							
							PT_ITEM_SPEC pItemProp		= pItemElem->GetProp();
							if( pItemProp )
							{
								pItemElem->m_nHitPoint	= ( pItemProp->dwEndurance == -1 ) ? 0 : pItemProp->dwEndurance;//pItemProp->dwEndurance;
								const WORD wOriginID = static_cast< WORD >( CRandomOptItemGen::GetInstance()->GenRandomOptItem( lpMoverProp->dwLevel, (FLOAT)nProbability / 100.0f, pItemProp, lpMoverProp->dwClass ) );
								pItemElem->SetRandomOptionOriginID( wOriginID );
							}
							pItemElem->SetAbilityOption( lpDropItem->dwLevel );		// �߰� �ɷ�ġ +1, +2 ������.
							pItemElem->SetSerialNumber();
							CItem* pItem	= new CItem;
							pItem->m_pItemBase	= pItemElem;
							BOOL bJJim = TRUE;		// ������ �ִٴ°� ǥ���ؾ��ϴ� ���ΰ�.
							if( lpMoverProp->dwClass == RANK_SUPER )		// �������� ����� �������� �ƹ��� ������ �ִ�.
								bJJim = FALSE;
							if( GetIndex() == OBJECT_INDEX( 196, MI_DEMIAN5 ) || GetIndex() == OBJECT_INDEX( 197, MI_KEAKOON5 ) || GetIndex() == OBJECT_INDEX( 198, MI_MUFFRIN5 ) )
								bJJim = FALSE;		// ����� �̺�Ʈ���̹Ƿ� ����س��� �ȴ�. �ƹ��� ������ ����
							if( bJJim )
							{
								pItem->m_idOwn = pAttacker->GetId();	// �� �������� ������ pAttacker(����Ŀ)������ ǥ��.
								pItem->m_dwDropTime = timeGetTime();	// ��� ��������� �ð��� �����.
							}
							pItem->m_bDropMob = TRUE;		// ���� �׾ ���� ���� ǥ�ø� �ص�.
							if( pItem->m_pItemBase->m_dwItemId == 0 ) FLERROR_LOG( PROGRAM_NAME, _T( "DropItem:2nd %s" ), GetName() );
							pItem->SetIndex( D3DDEVICE, pItem->m_pItemBase->m_dwItemId );

							vPos = GetPos();
							vPos.x += ( xRandomF(2.0f) - 1.0f );
							vPos.z += ( xRandomF(2.0f) - 1.0f );
							vPos.y = GetPos().y;
#ifdef __EVENT_MONSTER
							// �̺�Ʈ ���Ͱ� ����� �������� ������ ID�� ����Ѵ�(���� �� �ݰ�...)
							if( CEventMonster::GetInstance()->SetEventMonster( lpMoverProp->dwID ) )
							{
								// �̺�Ʈ ���ʹ� ������ �������� ���´�.
								pItem->m_idOwn	= pAttacker->GetId();
								pItem->m_dwDropTime		= timeGetTime();

								pItem->m_IdEventMonster = lpMoverProp->dwID;
								float fItemDropRange = CEventMonster::GetInstance()->GetItemDropRange(); 
								vPos = GetPos();
								vPos.x += ( xRandomF( fItemDropRange ) - (fItemDropRange / 2.0f) );
								vPos.z += ( xRandomF( fItemDropRange ) - (fItemDropRange / 2.0f) );
							}
#endif // __EVENT_MONSTER
							pItem->SetPos( vPos );

							if( GetWorld()->ADDOBJ( pItem, TRUE, GetLayer() ) == TRUE )
							{
								if( pItemProp->dwItemKind1 == IK1_WEAPON
									|| pItemProp->dwItemKind1 == IK1_ARMOR
									|| ( pItemProp->dwItemKind1 == IK1_GENERAL && pItemProp->dwItemKind2 == IK2_JEWELRY ) )
								{
									switch( pItemProp->nLog )
									{
									case 1:		g_dpDBClient.SendLogUniqueItem( pAttacker, pItem, pItemElem->GetAbilityOption() );	// �Ϲ� ������ +������
										break;
									case 2:		g_dpDBClient.SendLogUniqueItem( pAttacker, pItem, 200 );	// ����Ʈ ������
										break;
									case 3:		g_dpDBClient.SendLogUniqueItem( pAttacker, pItem, 100 );	// ����ũ ������
										break;
									}
								}

								if( lpDropItem->dwNumber != NULL_ID )
								{
									nNumber++;
								}

								if( static_cast< DWORD >( nNumber ) == lpMoverProp->m_DropItemGenerator.m_dwMax )
								{
									break;
								}
							}
							else
							{
								SAFE_DELETE( pItem );
							}
						}
						// ���� �����Ƕ������� �Ѵ�.
						else if( lpDropItem->dtType == DROPTYPE_SEED && k == 0 )
						{
							int	nSeedID = 0;
							int nNumGold = lpDropItem->dwNumber + xRandom( lpDropItem->dwNumber2 - lpDropItem->dwNumber );	// Number ~ Number2 ������ ������.
							nNumGold	= nNumGold * nPenyaRate / 100;
#ifdef __S1108_BACK_END_SYSTEM
							nNumGold	= (int)( nNumGold * prj.m_fGoldDropRate * lpMoverProp->m_fPenya_Rate );
							if( nNumGold == 0 )
								continue;
#else // __S1108_BACK_END_SYSTEM
							nNumGold	*= prj.m_fGoldDropRate;
#endif // __S1108_BACK_END_SYSTEM

							nNumGold	= (int)( nNumGold * prj.m_EventLua.GetGoldDropFactor() );	//sun: 9, �̺�Ʈ (��� ��ũ��Ʈ ����)

							if( lpMoverProp->dwFlying )
							{
								if( CanAdd( pAttacker->GetGold(), nNumGold ) )
								{
									pAttacker->AddGold( nNumGold );
									( (FLWSUser*)pAttacker )->AddGoldText( nNumGold );
								}
							}
							else
							{
								// ���׼��� ���� ������ �õ带 ������� �����Ѵ�.
								if( nNumGold <= (int)( g_xSpecManager->GetSpecItem( ITEM_INDEX( 12, II_GOLD_SEED1 ) )->dwAbilityMax ) )
									nSeedID = ITEM_INDEX( 12, II_GOLD_SEED1 );
								else if( nNumGold <= (int)( g_xSpecManager->GetSpecItem( ITEM_INDEX( 13, II_GOLD_SEED2 ) )->dwAbilityMax ) )
									nSeedID = ITEM_INDEX( 13, II_GOLD_SEED2 );
								else if( nNumGold <= (int)( g_xSpecManager->GetSpecItem( ITEM_INDEX( 14, II_GOLD_SEED3 ) )->dwAbilityMax ) )
									nSeedID = ITEM_INDEX( 14, II_GOLD_SEED3 );
								else 
									nSeedID = ITEM_INDEX( 15, II_GOLD_SEED4 );

								FLItemElem* pItemElem	= new FLItemElem;
								pItemElem->m_dwItemId	= nSeedID;
								pItemElem->m_nItemNum	= nNumGold;	// ���׼�
								pItemElem->m_nHitPoint	= nNumGold;
								CItem* pItem	= new CItem;
								pItem->m_pItemBase	= pItemElem;
								BOOL bJJim = TRUE;
								if( lpMoverProp->dwClass == RANK_SUPER )		// �������� ����� �������� �ƹ��� ������ �ִ�.
									bJJim = FALSE;
								if( GetIndex() == OBJECT_INDEX( 196, MI_DEMIAN5 ) || GetIndex() == OBJECT_INDEX( 197, MI_KEAKOON5 ) || GetIndex() == OBJECT_INDEX( 198, MI_MUFFRIN5 ) )
									bJJim = FALSE;		// ����� �̺�Ʈ���̹Ƿ� ����س��� �ȴ�. �ƹ��� ������ ����
								if( bJJim )
								{
									pItem->m_idOwn = pAttacker->GetId();	// �� �������� ������ pAttacker(����Ŀ)������ ǥ��.
									pItem->m_dwDropTime = timeGetTime();	// ��� ��������� �ð��� �����.
								}
								pItem->m_bDropMob = TRUE;		// ���� �׾ ���� ���� ǥ�ø� �ص�.
								if( pItem->m_pItemBase->m_dwItemId == 0 ) FLERROR_LOG( PROGRAM_NAME, _T( "DropItem: 3rd %s" ), GetName() );
								pItem->SetIndex( D3DDEVICE, pItem->m_pItemBase->m_dwItemId );

								vPos = GetPos();
								vPos.x += ( xRandomF(2.0f) - 1.0f );
								vPos.z += ( xRandomF(2.0f) - 1.0f );
#ifdef __EVENT_MONSTER
								// �̺�Ʈ ���Ͱ� ����� �������� ������ ID�� ����Ѵ�(���� �� �ݰ�...)
								if( CEventMonster::GetInstance()->SetEventMonster( lpMoverProp->dwID ) )
								{
									// �̺�Ʈ ���ʹ� ������ �������� ���´�.
									pItem->m_idOwn	= pAttacker->GetId();
									pItem->m_dwDropTime		= timeGetTime();

									pItem->m_IdEventMonster = lpMoverProp->dwID;
									float fItemDropRange = CEventMonster::GetInstance()->GetItemDropRange(); 
									vPos = GetPos();
									vPos.x += ( xRandomF( fItemDropRange ) - (fItemDropRange / 2.0f) );
									vPos.z += ( xRandomF( fItemDropRange ) - (fItemDropRange / 2.0f));
								}
#endif // __EVENT_MONSTER
								pItem->SetPos( vPos );
								if( GetWorld()->ADDOBJ( pItem, TRUE, GetLayer() ) == FALSE )
								{
									SAFE_DELETE( pItem );
								}
							}
						} // DROPTYPE_SEED
					} // if
					//////////////
					//  ������� for-loop�ȳ������� continue�ϴ� ���� �����ϱ� ����� �ڵ� ���� ����.
					///////////
				} // for nSize

				nSize	= lpMoverProp->m_DropKindGenerator.GetSize();
				DROPKIND* pDropKind;
				CPtrArray* pItemKindAry;
				int nAbilityOption; //, nDropLuck;
				BOOL bDrop = FALSE;
				for( int i = 0; i < nSize; i++ )
				{
					bDrop = FALSE;
					pDropKind	= lpMoverProp->m_DropKindGenerator.GetAt( i );
					pItemKindAry	= prj.GetItemKindAry( pDropKind->dwIK3 );
					int nMinIdx	= -1,	nMaxIdx		= -1;
					for( int j = pDropKind->nMinUniq; j <= pDropKind->nMaxUniq; j++ )
					{
						nMinIdx		= prj.GetMinIdx( pDropKind->dwIK3, j );
						if( nMinIdx != -1 )
							break;
					}
					for( int j = pDropKind->nMaxUniq; j >= pDropKind->nMinUniq; j-- )
					{
						nMaxIdx		= prj.GetMaxIdx( pDropKind->dwIK3, j );
						if( nMaxIdx != -1 )
							break;
					}
					if( nMinIdx < 0 || nMaxIdx < 0 )
					{
#ifdef __INTERNALSERVER
						FLERROR_LOG( PROGRAM_NAME, _T( "dropkind, 1, dwIndex = %d, nIndex = %d" ), lpMoverProp->dwID, i );
#endif	// __INTERNALSERVER
						continue;
					}
					PT_ITEM_SPEC pItemProp		= (PT_ITEM_SPEC)pItemKindAry->GetAt( nMinIdx + xRandom( nMaxIdx - nMinIdx + 1 ) );
					if( NULL == pItemProp )
					{
#ifdef __INTERNALSERVER
						FLERROR_LOG( PROGRAM_NAME, _T( "dropkind, 2, dwIndex = %d, nIndex = %d" ), lpMoverProp->dwID, i );
#endif	// __INTERNALSERVER
						continue;
					}

					nAbilityOption	= xRandom( 11 );	// 0 ~ 10

					DWORD dwAdjRand;

					for( int k = nAbilityOption; k >= 0; k-- )
					{
						DWORD dwPrabability		= (DWORD)( prj.m_adwExpDropLuck[( pItemProp->dwItemLV > 120? 119: pItemProp->dwItemLV-1 )][k]
						* ( (float)lpMoverProp->dwCorrectionValue / 100.0f ) );

						dwAdjRand	= xRandom( 3000000000 );
						if( bUnique && dwPrabability <= 10000000 )
							dwAdjRand	/= 2;

						if( dwAdjRand < dwPrabability )
						{
							if( lpMoverProp->dwFlying )
							{
								FLItemElem itemElem;
								itemElem.m_dwItemId		= pItemProp->dwID;
								itemElem.m_nItemNum		= 1;
								itemElem.SetAbilityOption( k );
								const WORD wOriginID = static_cast< WORD >( CRandomOptItemGen::GetInstance()->GenRandomOptItem( lpMoverProp->dwLevel, (FLOAT)nProbability / 100.0f, pItemProp, lpMoverProp->dwClass ) );
								itemElem.SetRandomOptionOriginID( wOriginID );

								if( pAttacker->CreateItem( &itemElem ) == TRUE )
								{	// log
									( (FLWSUser*)pAttacker )->AddDefinedText( TID_GAME_REAPITEM, "\"%s\"", pItemProp->szName );
									if( pItemProp->dwItemKind1 == IK1_WEAPON || pItemProp->dwItemKind1 == IK1_ARMOR || ( pItemProp->dwItemKind1 == IK1_GENERAL && pItemProp->dwItemKind2 == IK2_JEWELRY ) )
									{
										switch( pItemProp->nLog )
										{
										case 1:		g_dpDBClient.SendLogUniqueItem2( pAttacker, itemElem.GetProp(), itemElem.GetAbilityOption() );	break;
										case 2:		g_dpDBClient.SendLogUniqueItem2( pAttacker, itemElem.GetProp(), 200 );	break;
										case 3:		g_dpDBClient.SendLogUniqueItem2( pAttacker, itemElem.GetProp(), 100 );	break;
										}
									}
									break;
								}
							}

							FLItemElem* pItemElem	= new FLItemElem;
							pItemElem->m_dwItemId	= pItemProp->dwID;
							pItemElem->m_nItemNum	= 1;
							pItemElem->m_nHitPoint	= ( pItemProp->dwEndurance == -1 ) ? 0 : pItemProp->dwEndurance;//pItemProp->dwEndurance;
							pItemElem->SetAbilityOption( k );
							const WORD wOriginID = static_cast< WORD >( CRandomOptItemGen::GetInstance()->GenRandomOptItem( lpMoverProp->dwLevel, (FLOAT)nProbability / 100.0f, pItemProp, lpMoverProp->dwClass ) );
							pItemElem->SetRandomOptionOriginID( wOriginID );
							pItemElem->SetSerialNumber();
							CItem* pItem	= new CItem;
							pItem->m_pItemBase	= pItemElem;
							BOOL bJJim = TRUE;

							if( lpMoverProp->dwClass == RANK_SUPER )		// �������� ����� �������� �ƹ��� ������ �ִ�.
								bJJim = FALSE;
							if( GetIndex() == OBJECT_INDEX( 196, MI_DEMIAN5 ) || GetIndex() == OBJECT_INDEX( 197, MI_KEAKOON5 ) || GetIndex() == OBJECT_INDEX( 198, MI_MUFFRIN5 ) )
								bJJim = FALSE;		// ����� �̺�Ʈ���̹Ƿ� ����س��� �ȴ�. �ƹ��� ������ ����
							if( bJJim )
							{
								pItem->m_idOwn	= pAttacker->GetId();
								pItem->m_dwDropTime		= timeGetTime();
							}
							pItem->m_bDropMob	= TRUE;
#ifdef __EVENT_MONSTER
							if( CEventMonster::GetInstance()->SetEventMonster( lpMoverProp->dwID ) )
							{
								// �̺�Ʈ ���ʹ� ������ �������� ���´�.
								pItem->m_idOwn	= pAttacker->GetId();
								pItem->m_dwDropTime		= timeGetTime();

								pItem->m_IdEventMonster = lpMoverProp->dwID;
							}
#endif // __EVENT_MONSTER
							if( pItem->m_pItemBase->m_dwItemId == 0 ) FLERROR_LOG( PROGRAM_NAME, _T( "DropItem: 4th %s" ), GetName() );
							pItem->SetIndex( D3DDEVICE, pItem->m_pItemBase->m_dwItemId );
							pItem->SetPos( GetPos() );
							if( GetWorld()->ADDOBJ( pItem, TRUE, GetLayer() ) == TRUE )
							{
								bDrop = TRUE;

								if( pItemProp->dwItemKind1 == IK1_WEAPON
									|| pItemProp->dwItemKind1 == IK1_ARMOR
									|| ( pItemProp->dwItemKind1 == IK1_GENERAL && pItemProp->dwItemKind2 == IK2_JEWELRY ) )
								{
									switch( pItemProp->nLog )
									{
									case 1:		g_dpDBClient.SendLogUniqueItem( pAttacker, pItem, pItemElem->GetAbilityOption() );	break;
									case 2:		g_dpDBClient.SendLogUniqueItem( pAttacker, pItem, 200 );	break;
									case 3:		g_dpDBClient.SendLogUniqueItem( pAttacker, pItem, 100 );	break;
									}
								}
							}
							else
							{
								SAFE_DELETE( pItem );
							}
							break;
						}
					}

					if( GetProp()->dwClass == RANK_SUPER )
					{
						if( bDrop )		// KIND�������� �ϳ� ����Ʈ�ȴٸ� 
							break;		// ���̻� ����Ʈ���� �ʰ� ����.
					}
				} // for( i = 0; i < nSize; i++ )
			}	// if( xRandom( 100 ) < fItemDropRate )
		}	// for( int k = 0 ; k < nloop ; k++ )
	}

	if( IsNPC() )	// ���� �׾�����.. // ���� ������ �b�� �ε�??????
	{
		MoverProp* pMoverProp	= GetProp();
		if( pMoverProp->m_nLoot == 2 )	// 2�� d�ɼ�.  ������ �԰� ������ �ϴ� �ɼ�.
		{
			DWORD dwStart = m_Inventory.GetMax() - m_Inventory.GetSize();
			DWORD dwSize = m_Inventory.GetMax();
			FLItemElem *pElem = NULL;
			for( DWORD i = dwStart; i < dwSize; i ++ )
			{
				pElem = m_Inventory.GetAt( i );
				if( pElem )
				{
					D3DXVECTOR3 vPos = GetPos();
//#ifdef __WORLDSERVER
					vPos.x += ( xRandomF(2.0f) - 1.0f );
					vPos.z += ( xRandomF(2.0f) - 1.0f );
//#endif
					CItem *pItem = DropItem( i, pElem->m_nItemNum, vPos, FALSE );
					pItem->m_idOwn	= pAttacker->GetId();
					pItem->m_dwDropTime		= timeGetTime();
				}
			}
		}
	}

#endif	// __WORLDSERVER

	return TRUE;
}

// this������Ʈ���� ���ѻ��(JJimer -_-;;;) �� ���̵� �ھƵּ� 
// this�� ����ߴٴ°� ǥ��.
void CMover::SetJJim( CMover *pJJimer )
{
	if( g_xFlyffConfig->GetMainLanguage() == LANG_THA )	// �±��� ���
		if( GetIndex() == OBJECT_INDEX( 199, MI_MRPUMPKIN5 ) )	// ���� �̽�����Ų��
			return;													// ��ƿ�� ����.

	if( GetProp()->dwClass == RANK_SUPER )		// ���۸��� ��ƿ ���.
		return;

#ifdef __EVENT_MONSTER
	// �̺�Ʈ ���ʹ� ��ƿ ���
	if( CEventMonster::GetInstance()->IsEventMonster( GetProp()->dwID ) )
		return;
#endif // __EVENT_MONSTER

	// ���ѳ��� ��� / ����ѳ��� NPC�϶���.
	if( pJJimer->IsPlayer() && IsNPC() ) //this != pJJimer )		// ���ѻ���� ����ϰ�츸 ����./ �ڱ��ڽ��� ���Ҽ� ����.
	{
		m_idTargeter = pJJimer->GetId();
	}
}


CGuild* CMover::GetGuild()
{
#ifdef __WORLDSERVER
	//	locked
	CGuild* pGuild	= g_GuildMng.GetGuild( m_idGuild );
	if( pGuild && pGuild->IsMember( m_idPlayer ) )
		return pGuild;
	return NULL;
#else	// __WORLDSERVER
#ifdef __CLIENT
	if( m_idGuild > 0 && CMover::GetActiveMover() &&
		m_idGuild == CMover::GetActiveMover()->m_idGuild )
	{
		CGuild* pGuild	= g_GuildMng.GetGuild( m_idGuild );
		if( pGuild && pGuild->IsMember( m_idPlayer ) )
			return pGuild;
		return NULL;
	}
	else
	{
		return g_GuildMng.GetGuild( m_idGuild );
	}
#endif	// __CLIENT
#endif	// __WORLDSERVER
}

CGuildWar* CMover::GetWar()
{
	return g_GuildWarMng.GetWar(m_idWar);
}

#ifdef  __WORLDSERVER
// HP, MP, FPȸ���� ó���Ѵ�.
void CMover::ProcessRecovery()
{
	if( IsPlayer() == FALSE || IsDie() )
		return;

	DWORD dwCurTick = g_tmCurrent;

	if( dwCurTick > m_dwTickDuel )	// �ξ� ������ 1�ʿ� 3, 2, 1, Fight
	{
		if( 100 <= m_nDuelState && m_nDuelState < 200 )		// 100 ~ 200 �� ��� ������ ���� Ÿ��
		{
			--m_nDuelState;
			if( 100 == m_nDuelState )	// ������ Fight �޼����� ����
			{
				m_nDuelState = 1;		// ��� ���̶�� ����
				if( m_nDuel == 1 )
				{
					CMover *pDuelOther = prj.GetMover( m_idDuelOther );
					if( pDuelOther )
					{
						((FLWSUser*)this)->AddDuelStart( pDuelOther->m_idPlayer, 1 );	// Fight
						((FLWSUser*)this)->AddDuelCount( 0 );
					}
				}
				else if( m_nDuel == 2 )
				{
					CParty* pParty = g_PartyMng.GetParty( m_idDuelParty );		// ��û���� ��Ƽ����
					if( pParty )
					{
						LPCSTR pszPartyName;
						if( pParty->GetKind() == PARTY_KIND_EXTEND ) // ��ȸ�ش�
							pszPartyName = pParty->m_sParty;
						else // �ܸ��ش�
							//sun: 11, ĳ���� ���� ����
							pszPartyName = CPlayerDataCenter::GetInstance()->GetPlayerString( pParty->GetPlayerId( 0 ) );

						if( !pszPartyName )
						{
							FLERROR_LOG( PROGRAM_NAME, _T( "Error Party PVP 2" ) );
						}
						else
						{
							int		j;
							OBJID	pDstMember[ MAX_PTMEMBER_SIZE ] = { NULL_ID };
							//mem_set( pDstMember, 0xff, sizeof(pDstMember) );
							CMover *pMember2;

							for( j = 0; j < pParty->m_nSizeofMember; j ++ )
							{
								pMember2 = (CMover *)g_xWSUserManager->GetUserByPlayerID( pParty->m_aMember[j].m_uPlayerId );
								if( IsValidObj(pMember2) )
									pDstMember[j] = pMember2->GetId();
							}
							// ��� ��Ƽ�� ���� ������ �츮����鿡�� ����
							((FLWSUser *)this)->AddDuelPartyStart( pszPartyName, pParty->m_nSizeofMember, pDstMember, pParty->m_uPartyId, 1 );		// ���� ����� ID�� �ٺ�����. 2������
							((FLWSUser*)this)->AddDuelCount( 0 );
						}
					}
				}
			}
			else
			{
				((FLWSUser*)this)->AddDuelCount( m_nDuelState - 100 );
			}
		}
		m_dwTickDuel = dwCurTick + NEXT_TICK_DUEL;		
	}

	if( m_nDuel != 0 && dwCurTick > m_dwTickEndDuel )
	{
		// ������ 3�е��� ������ ������
		if( m_nDuel == 1 )
		{
			CMover *pDuelOther = prj.GetMover( m_idDuelOther );
			if( pDuelOther )
			{
				pDuelOther->ClearDuel();
				( (FLWSUser*)pDuelOther )->AddSetDuel( pDuelOther );
				ClearDuel();
				( (FLWSUser*)this )->AddSetDuel( this );
			}
		}
		else if( m_nDuel == 2 )
		{
			CParty* pParty = g_PartyMng.GetParty( m_idparty );
			if( pParty && pParty->IsLeader( m_idPlayer ) )
			{
				u_long uidDuelParty = m_idDuelParty;
				CMover *pMember;
				CParty* pParty2 = g_PartyMng.GetParty( uidDuelParty );
				for( int k = 0 ; k < pParty->m_nSizeofMember ; ++k )
				{
					pMember	= (CMover *)g_xWSUserManager->GetUserByPlayerID( pParty->m_aMember[k].m_uPlayerId );					
					if( IsValidObj( pMember ) )
					{
						if( pParty2 )
						{
							pParty2->DoDuelPartyCancel( pParty );
						}
						else 
						{
							pParty->DoDuelPartyCancel( NULL );
							FLERROR_LOG( PROGRAM_NAME, _T( "��Ƽ��� %s�� �����̻�. %d %d" ), pMember->GetName(), pMember->m_idDuelParty, uidDuelParty );
						}
					}
				}
				if( pParty2 )
				{
					for( int k = 0 ; k < pParty2->m_nSizeofMember ; ++k )
					{
						pMember	= (CMover *)g_xWSUserManager->GetUserByPlayerID( pParty2->m_aMember[k].m_uPlayerId );					
						if( IsValidObj( pMember ) )
						{	
							pParty->DoDuelPartyCancel( pParty2 );
						}
					}
				}
				g_DPCoreClient.SendSetPartyDuel( m_idparty, uidDuelParty, FALSE );
			}
		}
	}

	if( m_pActMover->IsSit() )		
	{
		// �ɾ����� ��
		float fRecovery = 1.0f;
		if( dwCurTick <= m_dwTickRecovery )	
			return;
		m_dwTickRecovery = dwCurTick + NEXT_TICK_RECOVERY;

		// ��Ƽ PARTY_STRETCHING_MODE�� �� 
		CParty* pParty = g_PartyMng.GetParty( m_idparty );
		if( pParty && pParty->IsMember( m_idPlayer ) )
		{
// 			if( pParty->m_nKindTroup == PARTY_KIND_EXTEND && pParty->m_nModeTime[PARTY_STRETCHING_MODE] )
// 			{
// 				//sun: 12, �Ľ�ų ������ ����
// 				BOOL nMItem = FALSE;
// 				FLWSUser* pLeader = g_xWSUserManager->GetUserByPlayerID( pParty->m_aMember[0].m_uPlayerId );
// 				if( pParty->m_nModeTime[PARTY_PARSKILL_MODE] )
// 				{
// 					nMItem = TRUE;
// 				}
// 				else
// 				{
// 					if( IsValidObj( (CObj*)pLeader ) )
// 					{
// 						if( IsValidArea( pLeader, 255.0f ) )
// 							nMItem = TRUE;
// 					}				
// 				}
// 				if( nMItem )
// 				{
// 					if( IsValidObj( pLeader ) && ( pLeader->GetJob() == JOB_ASSIST || pLeader->GetJob() == JOB_RINGMASTER || pLeader->GetJob() == JOB_BILLPOSTER ) )
// 						fRecovery = 1.8f;
// 					else
// 						fRecovery = 1.5f;
// 				}
// 			}

			if( pParty->CanApplySkillMode( m_idPlayer, PARTY_STRETCHING_MODE ) == true )
			{
// 				if( IsValidObj( pLeader ) && ( pLeader->GetJob() == JOB_ASSIST || pLeader->GetJob() == JOB_RINGMASTER || pLeader->GetJob() == JOB_BILLPOSTER ) )
// 					fRecovery = 1.8f;
// 				else
					fRecovery = 1.5f;
			}
		}
		else
		{
			m_idparty	= 0;
		}
		IncHitPoint( (int)( GetHPRecovery() * fRecovery ) );
		IncManaPoint( (int)( GetMPRecovery() * fRecovery ) );
		IncFatiguePoint( (int)( GetFPRecovery() * fRecovery ) );
	} 
	else	
	{
		// ������ ��
		m_dwTickRecovery = dwCurTick + NEXT_TICK_RECOVERY; // �ɾ��ֱ� ȸ���� ���´�.
		//if( m_pActMover->IsActAttack() == FALSE )	
		if( IsAttackMode() == FALSE )	// IsActAttack�� ���ݵ����� ����, IsAttackMode�� ������
		{
			// ���û��°� �ƴҶ��� �����ð����� ������.
			if( dwCurTick > m_dwTickRecoveryStand )	
			{
				m_dwTickRecoveryStand = dwCurTick + NEXT_TICK_RECOVERYSTAND;
				//sun: 10�� ���½ý���	Neuz, World, Trans
				if( ! (GetAdjParam( DST_CHRSTATE ) & CHS_SETSTONE) )
				{
					IncHitPoint( GetHPRecovery() );
					IncManaPoint( GetMPRecovery() );
					IncFatiguePoint( GetFPRecovery() );
				}
			}
		} 
		else
		{
			// ���� ������ ���� ��� �ð��� �����Ѵ�.
			m_dwTickRecoveryStand = dwCurTick + NEXT_TICK_RECOVERYSTAND;	
		}
	}
}
#endif // __WORLDSERVER

void CMover::AllocShopInventory( LPCHARACTER /*pCharacter*/ )
{
	if( IsVendorNPC() )
	{
		m_ShopInventory[ 0 ] = new CItemContainer< FLItemElem  >; m_ShopInventory[ 0 ]->SetItemContainer( ITYPE_ITEM, CONTAINER_TYPE_SHOP, MAX_VENDOR_INVENTORY );
		m_ShopInventory[ 1 ] = new CItemContainer< FLItemElem  >; m_ShopInventory[ 1 ]->SetItemContainer( ITYPE_ITEM, CONTAINER_TYPE_SHOP, MAX_VENDOR_INVENTORY );
		m_ShopInventory[ 2 ] = new CItemContainer< FLItemElem  >; m_ShopInventory[ 2 ]->SetItemContainer( ITYPE_ITEM, CONTAINER_TYPE_SHOP, MAX_VENDOR_INVENTORY );
		m_ShopInventory[ 3 ] = new CItemContainer< FLItemElem  >; m_ShopInventory[ 3 ]->SetItemContainer( ITYPE_ITEM, CONTAINER_TYPE_SHOP, MAX_VENDOR_INVENTORY );
	}
}

BOOL CMover::IsVendorNPC()
{
	LPCHARACTER pCharacter	= GetCharacter();
	if( !pCharacter )
		return FALSE;
	for( int i = 0; i < MAX_VENDOR_INVENTORY_TAB; i++ )
	{
		if( pCharacter->m_venderItemAry[i].GetSize() )
			return TRUE;
	}
	//sun: 11, Ĩ���� ���� �̿� �ϱ� (��� ���� ����)
	if(pCharacter->m_nVenderType == VENDOR_TYPE_CHIP)
		for( int i = 0; i < MAX_VENDOR_INVENTORY_TAB; i++ )
		{
			if( pCharacter->m_venderItemAry2[i].GetSize() )
				return TRUE;
		}
		return FALSE;
}

#ifdef __CLIENT	
BOOL CMover::EndMotion()
{
	if( !m_pActMover->IsActAttack() &&
		!m_pActMover->IsActDamage() &&
		!m_pActMover->IsActJump() && 
		!m_pActMover->IsFly() && 
		!m_pActMover->IsDie() && 
		!m_pActMover->IsSit() )
	{
		SetAngle( GetDegree(GetWorld()->GetCamera()->m_vPos, GetPos()) );		// ��ǥ������ ���� ����.
		SetMotion( MTI_LOGOUT, ANILOOP_CONT, MOP_FIXED );
		return TRUE;
	}
	return FALSE;
}
#endif //__CLIENT

BOOL CMover::IsBullet( PT_ITEM_SPEC pItemProp )
{
	// �ʿ��Ѿ������� �����Ǿ� �ִٸ�
	if( pItemProp->dwLinkKindBullet != NULL_ID )
	{
		DWORD dwTip = TID_TIP_NEEDSKILLITEM;
		if( pItemProp->dwItemKind3 == IK3_BOW || pItemProp->dwLinkKind == IK3_BOW )
		{
			dwTip = TID_TIP_NEEDSATTACKITEM;			
		}
#ifdef __ADDWEAPON_CROSSBOW16
		if( pItemProp->dwItemKind3 == IK3_CROSSBOW || pItemProp->dwLinkKind == IK3_CROSSBOW )
		{
			dwTip = TID_TIP_NEEDSATTACKITEM;
		}
#endif //__ADDWEAPON_CROSSBOW16

		PT_ITEM_SPEC pProp = GetActiveHandItemProp( PARTS_BULLET );
		if( pProp )
		{
			if( pProp->dwItemKind2 != pItemProp->dwLinkKindBullet )		// �ʿ��Ѿ������� �ٸ��� �����ϰ� ������ ����
			{
#ifdef __CLIENT
				g_WndMng.PutString( prj.GetText( dwTip ), NULL, prj.GetTextColor( dwTip ) );
				( (CWndWorld *)g_WndMng.m_pWndWorld )->m_bAutoAttack	= FALSE;
#endif // __CLIENT
#ifdef __WORLDSERVER
				((FLWSUser*)this)->AddDefinedText( dwTip, "" );
#endif // __WORLDSERVER
				return FALSE;
			}
		} else
		{	// �Ѿ�(?)�� ��� ����
#ifdef __CLIENT
			g_WndMng.PutString( prj.GetText( dwTip ), NULL, prj.GetTextColor( dwTip ) );
#endif // __CLIENT
#ifdef __WORLDSERVER
			((FLWSUser*)this)->AddDefinedText( dwTip, "" );
#endif // __WORLDSERVER
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CMover::IsBullet( SkillProp* pSkillProp )
{
	// �ʿ��Ѿ������� �����Ǿ� �ִٸ�
	if( pSkillProp->dwNeedItemBulletKind2 != NULL_ID )
	{
		DWORD dwTip = TID_TIP_NEEDSKILLITEM;
		if( pSkillProp->dwNeedItemKind3 == IK3_BOW )
		{
			dwTip = TID_TIP_NEEDSATTACKITEM;			
		}
#ifdef __ADDWEAPON_CROSSBOW16
		if( pSkillProp->dwNeedItemKind3 == IK3_CROSSBOW )
		{
			dwTip = TID_TIP_NEEDSATTACKITEM;
		}
#endif //__ADDWEAPON_CROSSBOW16

		PT_ITEM_SPEC pProp = GetActiveHandItemProp( PARTS_BULLET );
		if( pProp )
		{
			if( pProp->dwItemKind2 != pSkillProp->dwNeedItemBulletKind2 )		// �ʿ��Ѿ������� �ٸ��� �����ϰ� ������ ����
			{
#ifdef __CLIENT
				g_WndMng.PutString( prj.GetText( dwTip ), NULL, prj.GetTextColor( dwTip ) );
				( (CWndWorld *)g_WndMng.m_pWndWorld )->m_bAutoAttack	= FALSE;
#endif // __CLIENT
#ifdef __WORLDSERVER
				((FLWSUser*)this)->AddDefinedText( dwTip, "" );
#endif // __WORLDSERVER
				return FALSE;
			}
		} else
		{	// �Ѿ�(?)�� ��� ����
#ifdef __CLIENT
			g_WndMng.PutString( prj.GetText( dwTip ), NULL, prj.GetTextColor( dwTip ) );
#endif // __CLIENT
#ifdef __WORLDSERVER
			((FLWSUser*)this)->AddDefinedText( dwTip, "" );
#endif // __WORLDSERVER
			return FALSE;
		}
	}
	return TRUE;
}

#ifdef __WORLDSERVER
// ȭ���� nCount��� ����
// ȭ���� �����Ǿ� �ִ°��� �Ҹ������� �ڵ����� ���� ȭ���� ����
// ���� ȭ������ üũ
void CMover::ArrowDown( int nCount )
{
	if( IsNPC() )
	{
		return;
	}
	FLItemElem* pItemElem = m_Inventory.GetEquip( PARTS_BULLET );	// �����Ǿ� �ִ� ȭ�� ������ ����
	if( pItemElem != NULL )
	{
		DWORD dwItemId = pItemElem->m_dwItemId;
		BOOL bDoEquip = FALSE;
		int nRemoveArrow = nCount;

		for( int i = 0; i < nRemoveArrow; ++i )
		{
			BOOL bGetItem = FALSE;
			
			if( pItemElem->m_nItemNum <= nCount )
			{
				nCount -= pItemElem->m_nItemNum;
				pItemElem->m_nItemNum = 0;
				bGetItem = TRUE;
			}
			else
			{
				pItemElem->m_nItemNum -= nCount;
				nCount = 0;
				bGetItem = FALSE;
			}
			
			UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_NUM, pItemElem->m_nItemNum );
			
			if( bGetItem )
			{
				bDoEquip = FALSE;
				pItemElem = m_Inventory.GetAtByItemId( dwItemId );
				if( pItemElem == NULL )
				{
					break;
				}
				else
				{
					bDoEquip = TRUE;
				}
			}
			else
			{
				break;
			}
		}

		if( bDoEquip == TRUE && pItemElem != NULL && pItemElem->GetProp() != NULL )
		{
			if( TRUE == DoEquip( pItemElem, TRUE, pItemElem->GetProp()->dwParts ) )
			{
				g_xWSUserManager->AddDoEquip( this, pItemElem->GetProp()->dwParts, pItemElem, TRUE );
			}
		}
	}
}
#endif // __WORLDSERVER


void CMover::ClearDuel()
{
	m_nDuel		= 0;
	m_idDuelOther	= NULL_ID;
	m_nDuelState	= 0;
	m_dwTickEndDuel = ::timeGetTime() + NEXT_TICK_ENDDUEL; // 3 min
	SetPKTargetLimit( 10 );								   // PK������ ���ϰ� ������	
}

void CMover::ClearDuelParty()
{
	m_nDuel		= 0;
	m_idDuelParty	= 0;
	m_nDuelState	= 0;
	m_dwTickEndDuel = ::timeGetTime() + NEXT_TICK_ENDDUEL;	// 3 min
	SetPKTargetLimit( 10 );									// PK������ ���ϰ� ������
}

void CMover::SetPKTargetLimit( int nSec )
{
#if defined( __WORLDSERVER ) 
	m_dwPKTargetLimit = GetTickCount() + SEC( nSec );
#endif // defined( __WORLDSERVER )
}

#if defined(__WORLDSERVER)
BOOL CMover::IsItemRedyTime( PT_ITEM_SPEC pItemProp, OBJID dwObjid, BOOL bItemFind )
{
	if( pItemProp->dwSkillReadyType != 0 &&
		pItemProp->dwSkillReadyType != 0xffffffff )
	{
		if( pItemProp->dwItemKind2 == IK2_RIDING )
		{
			if( HasActivatedEatPet() || HasActivatedSystemPet() || HasPet() )
			{
				( (FLWSUser*)this )->AddDefinedText( TID_GAME_CANNOT_FLY_WITH_PET );
				m_nReadyTime = 0;
				m_dwUseItemId = 0;
				m_bItemFind = FALSE;
				SetStateNotMode( STATE_BASEMOTION_MODE, STATEMODE_BASEMOTION_CANCEL );	// �V�� ����
				return FALSE;
			}

			if( m_dwFlag & MVRF_NOACTION )
			{
				return FALSE;
			}
		}

		// ���̵带 ������ �Ϸ��� ������Ƽ�� �ִ°��� �����Ͽ� ������ �ؾ���
		if( IsStateMode( STATE_BASEMOTION_MODE ) ) 
		{
			if( m_nReadyTime != 0 )	// ������
			{
				( (FLWSUser*)this )->AddDefinedText(TID_PK_BLINK_LIMIT, "" );	// �����߿��� ����Ҽ� �����ϴ�
				return FALSE;
			}
			else	// ������ �Ϸ�� ����
			{
				SetStateNotMode( STATE_BASEMOTION_MODE, STATEMODE_BASEMOTION_OFF );
			}
		}
		else	// ���� �V��
		{
			//sun: 9, // __S_9_ADD
			if( pItemProp->dwItemKind2 == IK2_BLINKWING && m_pActMover->IsSit() )
			{
				( (FLWSUser*)this )->AddDefinedText(TID_GAME_NOTSIT_BLINK, "" );	
				return FALSE;
			}

			m_nReadyTime = timeGetTime() + pItemProp->dwSkillReadyType;
			m_dwUseItemId = dwObjid;
			m_bItemFind = bItemFind;
			SetStateMode( STATE_BASEMOTION_MODE, STATEMODE_BASEMOTION_ON );
			return FALSE;
		}
	}
	return TRUE;
}
#endif // __WORLDSERVER

#ifdef __CLIENT
CSfx* CMover::CreateSfxArrow( DWORD dwSfxObjArrow, DWORD dwSfxObjHit, D3DXVECTOR3 vPosDest, int idTarget  )
{
	CSfx* pSfx = NULL;

	// ȭ���� �޼տ� ����...
	D3DXVECTOR3 vPos;
	CModelObject *pModel = (CModelObject *)m_pModel;
	pModel->GetHandPos( &vPos, PARTS_LWEAPON, GetMatrixWorld() );		// �ָ� ������ǥ ����	

	pSfx = CreateSfx( D3DDEVICE, dwSfxObjArrow, vPos, GetId(), vPosDest , idTarget );

	if( dwSfxObjHit != NULL_ID )
	{
		if( pSfx ) 
		{
			((CSfxShoot*)pSfx)->m_dwSfxHit = dwSfxObjHit;
		}
	}

	return pSfx;

}
#endif
BOOL CMover::IsAfterDeath()	
{	
	if( m_nDeathLevel > m_nLevel )
		return TRUE;

	if( m_nDeathLevel == m_nLevel )
	{
		//sun: 8,  
		if( m_nExp1 == 0 )
			return TRUE;

		if( m_nDeathExp > m_nExp1 )
			return TRUE;
	}
	return FALSE;
}

int CMover::GetSetItem( FLItemElem* pItemElem )
{
	static	DWORD adwParts[4]	= { PARTS_UPPER_BODY, PARTS_HAND, PARTS_FOOT, PARTS_CAP	};
	if( !pItemElem )
	{
		pItemElem	= GetEquipItem( PARTS_UPPER_BODY );
	}
	else
	{
		if( !IsSetItemPart( pItemElem->GetProp()->dwParts ) )
			return 0;
	}

	if( !pItemElem )
		return 0;

	if( pItemElem->IsFlag( FLItemElem::expired ) )
		return 0;

	int nAbilityOption	= pItemElem->GetAbilityOption();
	DWORD dwParts	= pItemElem->GetProp()->dwParts;

	for( int i = 0; i < 4; i++ )
	{
		if( dwParts != adwParts[i] )
		{
			pItemElem	= GetEquipItem( adwParts[i] );
			if( pItemElem && !pItemElem->IsFlag( FLItemElem::expired ) )
			{
				if( nAbilityOption > pItemElem->GetAbilityOption() )
					nAbilityOption	= pItemElem->GetAbilityOption();
				continue;
			}
			return 0;
		}
	}
	return nAbilityOption;
}

int CMover::GetSetItemParts(DWORD dwParts)
{
	FLItemElem* pItemElem = NULL;
//	PT_ITEM_SPEC pItemProp = NULL;

	pItemElem = GetEquipItem( dwParts );

	int nAbilityOption = 0;

	if( pItemElem )
	{
		nAbilityOption = pItemElem->GetAbilityOption();
	} else
	{
		if( IsActiveMover() )
			return 0;
		nAbilityOption	= (m_aEquipInfo[dwParts].nOption & 0xFF );
	}

	return nAbilityOption;
}

int CMover::GetSetItemClient()
{
	static	DWORD adwParts[4]	= { PARTS_UPPER_BODY, PARTS_HAND, PARTS_FOOT, PARTS_CAP	};

	int nAbilityOption	= 10;

	for( int i = 0; i < 4; i++ )
	{
		int nValue	= GetSetItemParts( adwParts[i] );
		if( nValue == 0 )
			return 0;
		if( nAbilityOption > nValue )
			nAbilityOption	= nValue;
	}

	return nAbilityOption;
}

BOOL CMover::IsSetItemPart( DWORD dwParts )
{
	switch( dwParts )
	{
	case PARTS_UPPER_BODY:
	case PARTS_HAND:
	case PARTS_FOOT:
	case PARTS_CAP:
		return TRUE;
	}
	return FALSE;
}

void CMover::SetSetItemAvail( int nAbilityOption )
{
	PSETITEMAVAIL pSetItemAvail		= prj.GetSetItemAvail( nAbilityOption );
	if( pSetItemAvail )
	{
		//		FLTRACE_LOG( PROGRAM_NAME, _T( "SetSetItemAvail" ) );
		if( pSetItemAvail->nHitRate != 0 )
			SetDestParam( DST_ADJ_HITRATE, pSetItemAvail->nHitRate, NULL_CHGPARAM );
		if( pSetItemAvail->nBlock != 0 )
		{
			SetDestParam( DST_BLOCK_RANGE, pSetItemAvail->nBlock, NULL_CHGPARAM );
			SetDestParam( DST_BLOCK_MELEE, pSetItemAvail->nBlock, NULL_CHGPARAM );
		}
		if( pSetItemAvail->nMaxHitPointRate != 0 )
			SetDestParam( DST_HP_MAX_RATE, pSetItemAvail->nMaxHitPointRate, NULL_CHGPARAM );
		if( pSetItemAvail->nAddMagic != 0 )
			SetDestParam( DST_ADDMAGIC, pSetItemAvail->nAddMagic, NULL_CHGPARAM );
		if( pSetItemAvail->nAdded != 0 )
		{
			SetDestParam( DST_STR, pSetItemAvail->nAdded, NULL_CHGPARAM );
			SetDestParam( DST_DEX, pSetItemAvail->nAdded, NULL_CHGPARAM );
			SetDestParam( DST_INT, pSetItemAvail->nAdded, NULL_CHGPARAM );
			SetDestParam( DST_STA, pSetItemAvail->nAdded, NULL_CHGPARAM );
		}
	}
}

void CMover::ResetSetItemAvail( int nAbilityOption )
{
	PSETITEMAVAIL pSetItemAvail		= prj.GetSetItemAvail( nAbilityOption );
	if( pSetItemAvail )
	{
		//		FLTRACE_LOG( PROGRAM_NAME, _T( "ResetSetItemAvail" ) );
		if( pSetItemAvail->nHitRate != 0 )
			ResetDestParam( DST_ADJ_HITRATE, pSetItemAvail->nHitRate, TRUE );
		if( pSetItemAvail->nBlock != 0 )
		{
			ResetDestParam( DST_BLOCK_RANGE, pSetItemAvail->nBlock, TRUE );
			ResetDestParam( DST_BLOCK_MELEE, pSetItemAvail->nBlock, TRUE );
		}
		if( pSetItemAvail->nMaxHitPointRate != 0 )
			ResetDestParam( DST_HP_MAX_RATE, pSetItemAvail->nMaxHitPointRate, TRUE );
		if( pSetItemAvail->nAddMagic != 0 )
			ResetDestParam( DST_ADDMAGIC, pSetItemAvail->nAddMagic, TRUE );
		if( pSetItemAvail->nAdded != 0 )
		{
			ResetDestParam( DST_STR, pSetItemAvail->nAdded, TRUE );
			ResetDestParam( DST_DEX, pSetItemAvail->nAdded, TRUE );
			ResetDestParam( DST_INT, pSetItemAvail->nAdded, TRUE );
			ResetDestParam( DST_STA, pSetItemAvail->nAdded, TRUE );
		}
	}
}

void CMover::DestParamPiercingAvail( FLItemElem* pItemElem, BOOL bSET )
{
	if( pItemElem == NULL )
	{
		return;
	}

	if( pItemElem->IsPierceAble() == FALSE )	// TODO : �� �̰ɷ� üũ�� �ϴ°�?
	{
		return;
	}

	PIERCINGAVAIL piercingAvail_1 = { 0 };
	//mem_set( &piercingAvail, 0, sizeof(piercingAvail) );

	pItemElem->GetPiercingAvail( &piercingAvail_1 );

	for( int i = 0; i < piercingAvail_1.nSize; i++ )
	{
		if( bSET )
			SetDestParam( piercingAvail_1.anDstParam[i], piercingAvail_1.anAdjParam[i], NULL_CHGPARAM );
		else
			ResetDestParam( piercingAvail_1.anDstParam[i], piercingAvail_1.anAdjParam[i], TRUE );
	}

	//mem_set( &piercingAvail, 0, sizeof(piercingAvail) );
	PIERCINGAVAIL piercingAvail_2 = { 0 };

	pItemElem->GetCombinedPiercingAvail( &piercingAvail_2 );

	for( int i = 0; i < piercingAvail_2.nSize; i++ )
	{
		if( bSET )
			SetDestParam( piercingAvail_2.anDstParam[i], piercingAvail_2.anAdjParam[i], NULL_CHGPARAM );
		else
			ResetDestParam( piercingAvail_2.anDstParam[i], piercingAvail_2.anAdjParam[i], TRUE );
	}
}

#ifdef COSTUME_UPGRADE_ENHANCEMENT_GEM
void CMover::DestParamGemAvail( FLItemElem* pItemElem, BOOL bSET )
{
	if( pItemElem == NULL )
	{
		return;
	}

	int nAblity[5] = {0,};

	for( size_t i = 0; i < pItemElem->GetUltimatePiercingSize(); i++ )
	{
		int nDst = pItemElem->GetUltimatePiercingItemID(i);
		int nAdj = pItemElem->GetGemAvail(nDst);

		for( int j = 0; j < 5; j++ )
		{
			if( nDst == nAblity[j] )
			{
				nAdj = 0;
				break;
			}
		}

		nAblity[i] = nDst;

		if( bSET )
			SetDestParam( nDst, nAdj, NULL_CHGPARAM );
		else
			ResetDestParam( nDst, nAdj, TRUE );
	}
}
#endif

BOOL CMover::Pierce( FLItemElem *pItemElem, const DWORD dwPierceItemID )
{
	for( size_t i = 0; i < pItemElem->GetGeneralPiercingSize(); i++ )
	{
		if( pItemElem->GetGeneralPiercingItemID( i ) == 0 )
		{
			UpdateItem( pItemElem->m_dwObjId, UI_PIERCING, MAKELONG( i, dwPierceItemID ) );
			return TRUE;
		}
	}

	return FALSE;
}

#ifdef __WORLDSERVER
void CMover::CheckTickCheer()
{
	if( m_nCheerPoint >= MAX_CHEERPOINT )
		return;

	DWORD dwTickCount	= GetTickCount();
	if( dwTickCount > m_dwTickCheer )
		SetCheerParam( m_nCheerPoint + 1, dwTickCount, TICK_CHEERPOINT );
}
#endif	// __WORLDSERVER

void CMover::SetCheerParam( int nCheerPoint, DWORD dwTickCount, DWORD dwRest )
{
#ifdef __WORLDSERVER	// chipi_080411
	BOOL bAdd = FALSE;
	if( m_nCheerPoint < nCheerPoint )
		bAdd = TRUE;
#endif // __WORLDSERVER
	m_nCheerPoint	= nCheerPoint;
	m_dwTickCheer	= dwTickCount + dwRest;
#ifdef __WORLDSERVER
	( (FLWSUser*)this )->AddSetCheerParam( nCheerPoint, dwRest, /* chipi_080411 */ bAdd );
#endif	// __WORLDSERVER
}

BOOL CMover::IsDisguise()
{
	if( IsPlayer() )
	{
#ifdef	__WORLDSERVER
		if( m_dwDisguiseID != 0 )
		{
			return TRUE;
		}
#endif

		return ( GetIndex() != OBJECT_INDEX( 11, MI_MALE ) && GetIndex() != OBJECT_INDEX( 12, MI_FEMALE ) );
	}
	return FALSE;
}

BOOL CMover::Disguise( LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwMoverIndex )
{
#ifdef	__WORLDSERVER
	UNREFERENCED_PARAMETER( pd3dDevice );
	m_dwDisguiseID = dwMoverIndex;
#endif
#ifdef __CLIENT
	return SetIndex( pd3dDevice, dwMoverIndex );
#endif // __CLIENT
	return TRUE;
}

BOOL CMover::NoDisguise( LPDIRECT3DDEVICE9 pd3dDevice )     
{ 
#ifdef	__WORLDSERVER
	UNREFERENCED_PARAMETER( pd3dDevice );
	m_dwDisguiseID = 0;
#endif
#ifdef __CLIENT
	// �̹� ��� ����̹Ƿ� ���� ó�� �� �ʿ� ����. 
	if( GetIndex() == OBJECT_INDEX( 11, MI_MALE ) || GetIndex() == OBJECT_INDEX( 12, MI_FEMALE ) )
		return FALSE;
	// ���� �� �ı� 
	DWORD dwIndex = (GetSex() == SEX_MALE ? OBJECT_INDEX( 11, MI_MALE ) : OBJECT_INDEX( 12, MI_FEMALE ));
	SetIndex( pd3dDevice, dwIndex, FALSE, FALSE );
#endif  //__CLIENT
	return TRUE;
}

BOOL CMover::SetIndex( LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwMoverIndex, BOOL bInitProp, BOOL bDestParam )
{ 
	MoverProp* pMoverProp = prj.GetMoverProp( dwMoverIndex );
	if( pMoverProp == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "property not found: %d" ), dwMoverIndex );
		return FALSE;
	}
	// ���� �� �ı� 
	if( m_pModel && m_pModel->IsAniable() )
		SAFE_DELETE( m_pModel );
	SetTypeIndex( pd3dDevice, OT_MOVER, dwMoverIndex, bInitProp );//
	m_dwMotion = static_cast< DWORD >( -1 );
	SetMotion( MTI_STAND );
	// �̷��� ������Ƽ�� �̿��ϴ°� ����.
	if( m_bPlayer )
		// �÷��̾��� ��쿡�� ������ ����. �Ϲ� npc�� ������ �����ϸ� �������� ���� �˻� ���� ����, 
		// �ɷ� �� ���� ������ ��� Ż���� ���� �ֱ� ������, �ƿ� ������ ���� �ʴ´�.
		// ������ RedoEquip�� �ð����� ���̹Ƿ�, ���� ���ص� Inventory �������� ������ �Ǿ� �ִ�.
	{
		if( dwMoverIndex == OBJECT_INDEX( 11, MI_MALE ) || dwMoverIndex == OBJECT_INDEX( 12, MI_FEMALE ) )
		{
#if defined( __CLIENT )
			RedoEquip( !IsActiveMover(), bDestParam );
#else
			UNREFERENCED_PARAMETER( bDestParam );
			RedoEquip( FALSE );
#endif
			SetHairColor( m_dwHairColor );
		}
	}
	ResetScale();
	UpdateLocalMatrix();
	return TRUE;
}

LPCTSTR CMover::GetName( BOOL bNickname )
{
	if( !bNickname )
		return (LPCTSTR)m_szName;

	if( m_dwAuthorization >= AUTH_GAMEMASTER && ( GetIndex() == OBJECT_INDEX( 203, MI_CROWNIBLIS ) || GetIndex() == OBJECT_INDEX( 204, MI_CROWNSHADE ) || GetIndex() == OBJECT_INDEX( 205, MI_CROWNBUBBLE ) ) )
	{
		MoverProp* pMoverProp	= GetProp();
		if( pMoverProp )
			return (LPCTSTR)pMoverProp->szName;
	}
	return (LPCTSTR)m_szName;
}

void CMover::SetDestParamSetItem( FLItemElem* pItemElem )
{
	ITEMAVAIL itemAvail; // = { 0 };
	//mem_set( &itemAvail, 0, sizeof(itemAvail) );

	if( pItemElem && !pItemElem->IsFlag( FLItemElem::expired ) )
	{
		CSetItem* pSetItem	= CSetItemFinder::GetInstance()->GetSetItemByItemId( pItemElem->m_dwItemId );
		if( pSetItem )
		{
			int nEquiped	= GetEquipedSetItemNumber( pSetItem );
			pSetItem->GetItemAvail( &itemAvail, nEquiped, FALSE );
		}
	}
	else
	{
		set<CSetItem*> SetItems;
		for( int nParts = 0; nParts < MAX_HUMAN_PARTS; nParts++ )	{
			pItemElem	= GetEquipItem( nParts );
			if( pItemElem && !pItemElem->IsFlag( FLItemElem::expired ) )	{
				CSetItem* pSetItem	= CSetItemFinder::GetInstance()->GetSetItemByItemId( pItemElem->m_dwItemId );
				if( pSetItem )
					SetItems.insert( pSetItem );
			}
		}
		
		for( set<CSetItem*>::iterator it = SetItems.begin(); it != SetItems.end(); ++it )	{
			CSetItem* pSetItem	= (*it);
			int nEquiped = GetEquipedSetItemNumber( pSetItem );
			pSetItem->GetItemAvail( &itemAvail, nEquiped, TRUE );
		}
	}

	for( int i = 0; i < itemAvail.nSize; i++ )
		SetDestParam( itemAvail.anDstParam[i], itemAvail.anAdjParam[i], NULL_CHGPARAM );
}

void CMover::ResetDestParamSetItem( FLItemElem* pItemElem )
{
	if( pItemElem && !pItemElem->IsFlag( FLItemElem::expired ) )
	{
		CSetItem* pSetItem	= CSetItemFinder::GetInstance()->GetSetItemByItemId( pItemElem->m_dwItemId );
		if( pSetItem )
		{
			ITEMAVAIL itemAvail; // = { 0 };
			//mem_set( &itemAvail, 0, sizeof(itemAvail) );
			int nEquiped	= GetEquipedSetItemNumber( pSetItem, pItemElem );
			pSetItem->GetItemAvail( &itemAvail, nEquiped, FALSE );
			for( int i = 0; i < itemAvail.nSize; i++ )
				ResetDestParam( itemAvail.anDstParam[i], itemAvail.anAdjParam[i], TRUE );				
		}
	}
}

int CMover::GetEquipedSetItemNumber( CSetItem* pSetItem, FLItemElem* pUnequipItem )
{
	int nEquiped	= 0;
	for( int i = 0; i < pSetItem->m_nElemSize; ++i )	{
		PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( pSetItem->m_adwItemId[i] );
		FLItemElem* pItemElem[2];
		pItemElem[0] = pItemProp->dwParts	!=	NULL_ID ? GetEquipItem( pItemProp->dwParts )	:	NULL;
		pItemElem[1] = pItemProp->dwPartsub	!=	NULL_ID ? GetEquipItem( pItemProp->dwPartsub )	:	NULL;
		for( int j = 0; j < pSetItem->m_anEquipCount[i]; ++j )	{
			for( int k = 0; k < 2; ++k )	{
				if( pUnequipItem != NULL && pUnequipItem->m_dwItemId == pItemProp->dwID && !pUnequipItem->IsFlag( FLItemElem::expired ) )	{
					pUnequipItem = NULL;
					++nEquiped;
					break;
				}
				else if( pItemElem[k] != NULL && pItemElem[k]->m_dwItemId == pItemProp->dwID && !pItemElem[k]->IsFlag( FLItemElem::expired ) )	{
					pItemElem[k] = NULL;
					++nEquiped;
					break;
				}				
			}
		}
	}
	return nEquiped;
}

void CMover::GetEquipedSetItem( CSetItem* pSetItem, int* pbEquipedCount, int* pnTotal, int* pnEquip )
{
	*pnEquip = 0;
	*pnTotal = 0;
	for( int i = 0; i < pSetItem->m_nElemSize; ++i )	{
		*pnTotal += pSetItem->m_anEquipCount[i];
		PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( pSetItem->m_adwItemId[i] );
		FLItemElem* pItemElem[2];
		pItemElem[0] = pItemProp->dwParts	!=	NULL_ID ? GetEquipItem( pItemProp->dwParts )	:	NULL;
		pItemElem[1] = pItemProp->dwPartsub	!=	NULL_ID ? GetEquipItem( pItemProp->dwPartsub )	:	NULL;
		for( int j = 0; j < pSetItem->m_anEquipCount[i]; ++j )	{
			for( int k = 0; k < 2; ++k )	{
				if( pItemElem[k] != NULL && pItemElem[k]->m_dwItemId == pItemProp->dwID && !pItemElem[k]->IsFlag( FLItemElem::expired ) )	{
					pItemElem[k] = NULL;
					++pbEquipedCount[i];
					++(*pnEquip);
					break;
				}				
			}
		}
	}
}

void CMover::SetDestParamRandomOptOrigin( FLItemElem* pItemElem )
{
	if( pItemElem == NULL )
	{
		return;
	}

	if( pItemElem->GetRandomOptionOriginID() > 0 )
	{
		const PRANDOMOPTITEM pRandomOptItem	= CRandomOptItemGen::GetInstance()->GetRandomOptItem( pItemElem->GetRandomOptionOriginID() );
		if( pRandomOptItem )
		{
			for( int i = 0; i < pRandomOptItem->ia.nSize; i++ )
				SetDestParam( pRandomOptItem->ia.anDstParam[i], pRandomOptItem->ia.anAdjParam[i], NULL_CHGPARAM );
		}
	}

	if( pItemElem->GetCombinedRandomOptionOriginID() > 0 )
	{
		const PRANDOMOPTITEM pRandomOptItem	= CRandomOptItemGen::GetInstance()->GetRandomOptItem( pItemElem->GetCombinedRandomOptionOriginID() );
		if( pRandomOptItem )
		{
			for( int i = 0; i < pRandomOptItem->ia.nSize; i++ )
				SetDestParam( pRandomOptItem->ia.anDstParam[i], pRandomOptItem->ia.anAdjParam[i], NULL_CHGPARAM );
		}
	}
}

void CMover::SetDestParamRandomOptExtension( FLItemElem* pItemElem )
{
	if( pItemElem == NULL )
	{
		return;
	}

	if( pItemElem->IsSetSafeGuardRandomOptionExtensionFlag() == false && pItemElem->IsSetRandomOptionExtension() == true )
	{
		for( size_t Nth = 0; Nth < pItemElem->GetRandomOptionExtensionSize(); ++Nth )
		{
			const WORD wDstID = pItemElem->GetRandomOptionExtensionDstID( Nth );
			const short shAdjValue = pItemElem->GetRandomOptionExtensionAdjValue( Nth );

			SetDestParam( static_cast<int>( wDstID ), static_cast<int>( shAdjValue ), NULL_CHGPARAM );
		}
	}

	if( pItemElem->IsSetCombinedRandomOptionExtension() == true )
	{
		for( size_t Nth = 0; Nth < pItemElem->GetCombinedRandomOptionExtensionSize(); ++Nth )
		{
			const WORD wDstID = pItemElem->GetCombinedRandomOptionExtensionDstID( Nth );
			const short shAdjValue = pItemElem->GetCombinedRandomOptionExtensionAdjValue( Nth );

			SetDestParam( static_cast<int>( wDstID ), static_cast<int>( shAdjValue ), NULL_CHGPARAM );
		}
	}
}

void CMover::SetDestParamRandomOpt( FLItemElem* pItemElem )
{
	SetDestParamRandomOptOrigin( pItemElem );
	SetDestParamRandomOptExtension( pItemElem );
}

void CMover::ResetDestParamRandomOptOrigin( FLItemElem* pItemElem )
{
	if( pItemElem == NULL )
	{
		return;
	}

	if( pItemElem->GetRandomOptionOriginID() > 0 )
	{
		const PRANDOMOPTITEM pRandomOptItem	= CRandomOptItemGen::GetInstance()->GetRandomOptItem( pItemElem->GetRandomOptionOriginID() );
		if( pRandomOptItem )
		{
			for( int i = 0; i < pRandomOptItem->ia.nSize; i++ )
			{
				ResetDestParam( pRandomOptItem->ia.anDstParam[i], pRandomOptItem->ia.anAdjParam[i], TRUE );
			}
		}
	}

	if( pItemElem->GetCombinedRandomOptionOriginID() > 0 )
	{
		const PRANDOMOPTITEM pRandomOptItem	= CRandomOptItemGen::GetInstance()->GetRandomOptItem( pItemElem->GetCombinedRandomOptionOriginID() );
		if( pRandomOptItem )
		{
			for( int i = 0; i < pRandomOptItem->ia.nSize; i++ )
			{
				ResetDestParam( pRandomOptItem->ia.anDstParam[i], pRandomOptItem->ia.anAdjParam[i], TRUE );
			}
		}
	}
}

void CMover::ResetDestParamRandomOptExtension( FLItemElem* pItemElem )
{
	if( pItemElem == NULL )
	{
		return;
	}

	if( pItemElem->IsSetSafeGuardRandomOptionExtensionFlag() == false && pItemElem->IsSetRandomOptionExtension() == true )
	{
		for( size_t Nth = 0; Nth < pItemElem->GetRandomOptionExtensionSize(); ++Nth )
		{
			const WORD wDstID = pItemElem->GetRandomOptionExtensionDstID( Nth );
			const short shAdjValue = pItemElem->GetRandomOptionExtensionAdjValue( Nth );

			ResetDestParam( static_cast<int>( wDstID ), static_cast<int>( shAdjValue ), TRUE );
		}
	}

	if( pItemElem->IsSetCombinedRandomOptionExtension() == true )
	{
		for( size_t Nth = 0; Nth < pItemElem->GetCombinedRandomOptionExtensionSize(); ++Nth )
		{
			const WORD wDstID = pItemElem->GetCombinedRandomOptionExtensionDstID( Nth );
			const short shAdjValue = pItemElem->GetCombinedRandomOptionExtensionAdjValue( Nth );

			ResetDestParam( static_cast<int>( wDstID ), static_cast<int>( shAdjValue ), TRUE );
		}
	}
}

void CMover::ResetDestParamRandomOpt( FLItemElem* pItemElem )
{
	ResetDestParamRandomOptOrigin( pItemElem );
	ResetDestParamRandomOptExtension( pItemElem );
}

#ifdef __CLIENT
// pModel�� ���ڷ� ���� ������....�۷ι����� ����ϴ� �ѵ��� �־...
void CMover::OverCoatItemRenderCheck( CModelObject* pModel )
{
	if( !pModel )
		return;

	// �����ǻ��� �Ծ��ٸ� �ֹ� �ǻ��� �����...			
	int nArryEquip1[5] = { PARTS_CAP, PARTS_UPPER_BODY, PARTS_HAND, PARTS_FOOT, PARTS_CLOAK };
	int nArryEquip2[5] = { PARTS_HAT, PARTS_CLOTH, PARTS_GLOVE, PARTS_BOOTS, PARTS_CLOAK2 };

	int nTemp[5];

	/*if(!IsOptionRenderCostume(0))
	{
		memcpy( nTemp, nArryEquip1, sizeof(int) * 5 );
		memcpy( nArryEquip1, nArryEquip2, sizeof(int) * 5 );
		memcpy( nArryEquip2, nTemp, sizeof(int) * 5 );
	}*/

	for( int i = 0; i < 5; i++ )
	{
		if(!IsOptionRenderCostume(i))
		{
			nTemp[i] = nArryEquip1[i];
			nArryEquip1[i] = nArryEquip2[i];
			nArryEquip2[i] = nTemp[i];
		}
	}


	


	if( IsActiveMover() )
	{
		FLItemElem* pItemElem = NULL;
		PT_ITEM_SPEC pItemProp = NULL;
		pItemElem = m_Inventory.GetEquip( PARTS_UPPER_BODY );//AtId( padwEquipment[ i ] );

		if(IsOptionRenderCostume(1))
		{
			pItemElem = m_Inventory.GetEquip( PARTS_CLOTH );
		}

		if( pItemElem )
		{
			if( !pItemElem->IsFlag( FLItemElem::expired ) )
			{
				//�������� ó��
				if( pItemElem->GetLooksChangeItemID( ) != 0 )
					pItemProp	= g_xSpecManager->GetSpecItem( pItemElem->GetLooksChangeItemID( ) );
				else
					pItemProp = pItemElem->GetProp();
			}
		}
		O3D_ELEMENT *pElemLowerBody = NULL;
		pElemLowerBody = pModel->GetParts( PARTS_LOWER_BODY );

		//if( !IsOptionRenderCostume(1) && pModel->GetParts( PARTS_CLOTH ) != NULL && pItemElem == NULL)
		if( !IsOptionRenderCostume(1) && pItemElem == NULL)
		{
			if( pElemLowerBody == NULL)
				SetDefaultLowerBody(pModel);
			else
				pElemLowerBody->m_nEffect &= ~XE_HIDE;
		}
		else
		{
			if(pItemProp && pItemProp->dwExclusive != NULL_ID)
			{
				if( pElemLowerBody )
					pElemLowerBody->m_nEffect |= XE_HIDE;
			}
		}
		
	}
	else
	{
		O3D_ELEMENT *pElemLowerBody = NULL;
		pElemLowerBody = pModel->GetParts( PARTS_LOWER_BODY );

		//if( !IsOptionRenderCostume(1) && pModel->GetParts( PARTS_CLOTH ) != NULL)
		if( !IsOptionRenderCostume(1) && m_aEquipInfo[PARTS_UPPER_BODY].dwItemID == 0)
		{
			SetDefaultLowerBody(pModel);
		}
		else
		{
			if(m_bExclusiveLowBody)
			{
				if( pElemLowerBody )
					pElemLowerBody->m_nEffect |= XE_HIDE;
			}
		}
	}
	

	O3D_ELEMENT *pElem = NULL;
	O3D_ELEMENT *pElem2 = NULL;

	for( int j=0; j<5; j++ )
	{
		pElem = pModel->GetParts( nArryEquip1[j] );
		pElem2 = pModel->GetParts( nArryEquip2[j] );

		

		if( pElem2 )
			pElem2->m_nEffect &= ~XE_HIDE;
		

		if( !pElem )
			continue;

		pElem->m_nEffect &= ~XE_HIDE;

		

		if( pElem2 )
		{
			//pElem2->m_nEffect &= ~XE_HIDE;
			if( IsActiveMover() )
			{
				FLItemElem* pItemElemOvercoat = GetEquipItem( nArryEquip2[j] );

				if( pItemElemOvercoat )
				{
					if( pItemElemOvercoat->IsFlag( FLItemElem::expired ) )
					{
						pElem->m_nEffect &= ~XE_HIDE;
						pElem2->m_nEffect |= XE_HIDE;
						continue;
					}
				}
			}
			else
			{
				if( (m_aEquipInfo[ nArryEquip2[j] ].byFlag & FLItemElem::expired) )
				{
					pElem->m_nEffect &= ~XE_HIDE;
					pElem2->m_nEffect |= XE_HIDE;					
					continue;
				}
			}

			pElem->m_nEffect |= XE_HIDE;
		}
		else
		{
			if(!IsOptionRenderCostume(0) && j == 0)
			{
				pElem->m_nEffect |= XE_HIDE;
			}
		}

	}
}
#endif	// __CLIENT

#ifdef __WORLDSERVER
void CMover::ProcInstantBanker()
{
	if( m_dwTickCreated && m_dwIndex == OBJECT_INDEX( 200, MI_INFO_PENG ) )
	{
		LPCHARACTER lpCharacter	= GetCharacter();
		if( lpCharacter && lstrcmp( lpCharacter->m_szKey, "MaFl_InstantBank" ) == 0 )
		{
			if( GetTickCount() > m_dwTickCreated + TIMEWAIT_INSTANT_BANKER )
				Delete();
		}
	}
}
void CMover::ProcInstantGC()
{
	if( CGuildCombat::WAR_STATE <= g_GuildCombatMng.m_nState  && m_dwIndex == OBJECT_INDEX( 229, MI_MADA_ACHABEN ) )
	{
		LPCHARACTER lpCharacter	= GetCharacter();
		if( lpCharacter && lstrcmp( lpCharacter->m_szKey, "Mada_Guildcombatshop" ) == 0 )
		{
			Delete();
		}
	}
}
#endif	// __WORLDSERVER

FLItemBase*	CMover::GetVendorItem()
{
	return NULL;
}

void CMover::AddSkillPoint( int nPoint )
{
// 	int nMaxPoint = SKILLPOINT_MAX;	// �ִ��� JOB_ELEMENTOR ���� ������
// 	if( IsPro() )
// 	{
// 		if( m_nJob == JOB_KNIGHT || m_nJob == JOB_BLADE )
// 			nMaxPoint = SKILLPOINT_MAX_MERCENARY;
// 		else if( m_nJob == JOB_JESTER || m_nJob == JOB_RANGER )
// 			nMaxPoint = SKILLPOINT_MAX_ACROBAT;
// 		else if( m_nJob == JOB_RINGMASTER )
// 			nMaxPoint = SKILLPOINT_MAX_RINGMASTER;
// 		else if( m_nJob == JOB_BILLPOSTER || m_nJob == JOB_PSYCHIKEEPER )
// 			nMaxPoint = SKILLPOINT_MAX_BILLPO_PSYCHI;
// 	}
// 
// 	if( m_nSkillLevel >= nMaxPoint )
// 		return;
// 
// 	if( nMaxPoint < m_nSkillLevel + nPoint )	// �� ������ �Ѵ� ���(���� ���� ����)
// 		nPoint = nMaxPoint - m_nSkillLevel;	// �ƽ�ġ�� ���ڸ� ��ŭ�� ����Ʈ ����

	m_nSkillPoint += nPoint;
	m_nSkillLevel += nPoint;
}

// �������ΰ�?
BOOL CMover::IsAttackMode()
{
	// 9�� �߰� (Ÿ��, �ǰ� 10�� �� �ڵ� ȸ��)
	return ( m_nAtkCnt && m_nAtkCnt < (SEC1 * 10) );
	//	return ( m_nAtkCnt && m_nAtkCnt < (SEC1 * 5) );		// 9�� �߰� (Ÿ��, �ǰ� 10�� �� �ڵ� ȸ��)( < 9 )
}

BOOL CMover::IsDropable( FLItemElem* pItemElem, BOOL bPK )
{
	return ( IsDropableState( bPK )
		&& IsUsableItem( pItemElem )
		&& !pItemElem->IsOwnState()
		&& !( pItemElem->GetProp()->dwItemKind3 == IK3_CLOAK && pItemElem->m_idGuild )
		&& !m_Inventory.IsEquip( pItemElem->m_dwObjId )
		&& !IsUsing( pItemElem )
		&& !( pItemElem->GetProp()->dwParts == PARTS_RIDE && pItemElem->GetProp()->dwItemJob == JOB_VAGRANT )
		&& !pItemElem->IsCharged()
		&& pItemElem->GetProp()->dwItemKind3 != IK3_EGG	//sun: 9, 9-10�� ��
		&& pItemElem->m_dwItemId != ITEM_INDEX( 26460, II_CHP_RED )
		//sun, 11 �ɸ��� ���� �ŷ� ��� world,database,neuz
		&& pItemElem->m_dwItemId != ITEM_INDEX( 26476,II_SYS_SYS_SCR_SEALCHARACTER )

		);
}

#ifdef __CLIENT
BOOL CMover::IsStateDbuff()
{
	int nAdjParam = GetAdjParam( DST_CHRSTATE );
	if( nAdjParam & CHS_STUN || nAdjParam & CHS_POISON || nAdjParam & CHS_DARK ||
		nAdjParam & CHS_GROGGY || nAdjParam & CHS_SILENT || nAdjParam & CHS_BLEEDING )
	{
		return TRUE;
	}

	return FALSE;
}
#endif


bool __IsEndQuestCondition( CMover* pMover, int nQuestId )
{
	QuestProp* pQuestProp = prj.m_aPropQuest.GetAt( nQuestId );

	if( pQuestProp )
	{
		int nResult = 0;
		LPQUEST pCurQuest = pMover->GetQuest( nQuestId );
		if( pCurQuest == NULL || pCurQuest->m_nState == QS_END ) 
			return false;
		// �ð� ���� üũ 1
		if( pQuestProp->m_nEndCondLimitTime == 0 || pCurQuest->m_wTime )
			nResult++;
		// ���� �ӹ�  2
		if( pQuestProp->m_dwEndCondPatrolWorld == 0 || pCurQuest->m_bPatrol )
			nResult++;
		// NPC ��ġ ����  3,4
		for( int i = 0; i < MAX_QUEST_KILL_NPC_COUNT; i++ )
		{
			if( pQuestProp->m_nEndCondKillNPCIdx[ i ] == 0 || pCurQuest->m_nKillNPCNum[ i ] >= pQuestProp->m_nEndCondKillNPCNum[ i ] ) 
				nResult++;
		}
		// ��ų 5
		if( pQuestProp->m_nEndCondSkillIdx == 0 )
			nResult++;
		else
		{
			LPSKILL lpSkill = pMover->GetSkill( pQuestProp->m_nEndCondSkillIdx );
			if( lpSkill )
			{
				if( pMover->CheckSkill( pQuestProp->m_nEndCondSkillIdx ) && lpSkill->dwLevel >= (DWORD)( pQuestProp->m_nEndCondSkillLvl ) )
					nResult++;
			}
		}
		//sun: 8, // __S8_PK
		// Penya 6
		if( pQuestProp->m_nEndCondGold == 0 )
			nResult++;
		else
		{
			//			if( pMover->GetGold() >= pQuestProp->m_nEndCondGold )
			if( pMover->CheckUserGold( pQuestProp->m_nEndCondGold, false ) == true )
			{
				nResult++;
			}
		}
		// ���� 7
		if( pQuestProp->m_nEndCondLevelMin == 0 || ( pMover->GetLevel() >= pQuestProp->m_nEndCondLevelMin && pMover->GetLevel() <= pQuestProp->m_nEndCondLevelMax ) )
			nResult++;
		//sun: 10�� ���½ý���	Neuz, World, Trans
		// 8
		if( pQuestProp->m_nEndCondExpPercentMin == 0 || ( pMover->GetExpPercent() >= pQuestProp->m_nEndCondExpPercentMin && pMover->GetExpPercent() <= pQuestProp->m_nEndCondExpPercentMax ) )
			nResult++;

		//sun: 9, 9-10�� ��
		// 9
		if( pQuestProp->m_nEndCondPetExp == 0 )
			nResult++;
		else
		{
			CPet* pPet	= pMover->GetPet();
			if( pPet && pPet->GetExpPercent() >= pQuestProp->m_nEndCondPetExp )
				nResult++;
		}
		// 10
		if( pQuestProp->m_nEndCondPetLevel == -1 )
			nResult++;
		else
		{
			CPet* pPet	= pMover->GetPet();
			if( pPet && pPet->GetLevel() == pQuestProp->m_nEndCondPetLevel )
				nResult++;
		}

		// ���� 8 11
		if( pQuestProp->m_nEndCondDisguiseMoverIndex == 0 )
			nResult++;
		else
		{
			if( pQuestProp->m_nEndCondDisguiseMoverIndex == -1 && pMover->IsDisguise() == FALSE )
				nResult++;
			else
				if( pQuestProp->m_nEndCondDisguiseMoverIndex == static_cast< int >( pMover->GetIndex() ) )
					nResult++;
		}
		// ������Ʈ üũ 9  12
		if( pQuestProp->m_nEndCondState == 0 )
		{
			int i = QS_BEGIN + 1;
			for( ; i < QS_END; i++ ) // (1~13)
			{
				if( pQuestProp->m_questState[ i ] == NULL )
					break;
			}
			if( ( i - 1 ) == pCurQuest->m_nState )
				nResult++;
		}
		else
			if( pQuestProp->m_nEndCondState == pCurQuest->m_nState )
				nResult++;
		// �Ϸ� ����Ʈ üũ 10,11,12,13,14, 15  18
		if( pQuestProp->m_nEndCondCompleteQuestOper == 0 ) // 
			nResult += 6;
		else
			if( pQuestProp->m_nEndCondCompleteQuestOper == 1 ) // or
			{
				int i = 0;
				for( ; i < 6; i++ )
				{
					if( pMover->IsCompleteQuest( pQuestProp->m_nEndCondCompleteQuest[ i ] ) )
						break;
				}
				if( i != 6 )
					nResult += 6;
			}
			else
				if( pQuestProp->m_nEndCondCompleteQuestOper == 2 ) // and
				{
					for( int i = 0; i < 6; i++ )
					{
						if( pQuestProp->m_nEndCondCompleteQuest[ i ] == 0 || pMover->IsCompleteQuest( pQuestProp->m_nEndCondCompleteQuest[ i ] ) )
							nResult++;
					}
				}
				//		else
				// ��Ƽ ���� 16 19
				if( pQuestProp->m_nEndCondParty == 0 )
					nResult++;
				else
				{
					BOOL bParty = FALSE;
					BOOL bLeader = FALSE;
					int nSize = 0;
#ifdef __WORLDSERVER
					CParty* pParty = g_PartyMng.GetParty( pMover->m_idparty );
					if( pParty )
					{
						bParty = TRUE;
						nSize = pParty->GetSizeofMember();
						bLeader	= pParty->m_aMember[0].m_uPlayerId == pMover->m_idPlayer;
					}
#else
					bParty = g_Party.IsMember( g_pPlayer->m_idPlayer );
					nSize = g_Party.GetSizeofMember();
					bLeader	= ( g_Party.m_aMember[0].m_uPlayerId == pMover->m_idPlayer );
#endif
					if( pQuestProp->m_nEndCondParty == 1 && bParty == FALSE )
						nResult++;
					else
						if( pQuestProp->m_nEndCondParty == 2 && bParty == TRUE )
						{
							if( pQuestProp->m_nEndCondPartyLeader == 0 || ( pQuestProp->m_nEndCondPartyLeader - 1 ) == bLeader )
							{
								if( pQuestProp->m_nEndCondPartyNum == 0 )
									nResult++;
								else
									if( pQuestProp->m_nEndCondPartyNumComp == 0 )
									{
										if( nSize == pQuestProp->m_nEndCondPartyNum )
											nResult++;
									}
									else
										if( pQuestProp->m_nEndCondPartyNumComp == 1 )
										{
											if( nSize >= pQuestProp->m_nEndCondPartyNum )
												nResult++;
										}	
										else
											if( pQuestProp->m_nEndCondPartyNumComp == -1 )
											{
												if( nSize <= pQuestProp->m_nEndCondPartyNum )
													nResult++;
											}						
							}
						}
				}
				// ��� ���� 17  20
				if( pQuestProp->m_nEndCondGuild == 0 )
					nResult++;
				else
				{
					BOOL bGuild = FALSE;
					int nLeader = 0;
					int nSize = 0;

					CGuild* pGuild	= g_GuildMng.GetGuild( pMover->m_idGuild );
					if( pGuild != NULL && pGuild->IsMember( pMover->m_idPlayer ) == TRUE )
					{
						bGuild	= TRUE;
						nSize	= pGuild->GetSize();
						if( pGuild->m_idMaster == pMover->m_idPlayer )
						{
							nLeader = 2;
						}
						else
						{
							nLeader = 1;
						}
					}

					if( pQuestProp->m_nEndCondGuild == 1 && bGuild == FALSE )
						nResult++;
					else
						if( pQuestProp->m_nEndCondGuild == 2 && bGuild == TRUE )
						{
							if( pQuestProp->m_nEndCondGuildLeader == 0 || pQuestProp->m_nEndCondGuildLeader == nLeader )
							{
								if( pQuestProp->m_nEndCondGuildNum == 0 )
									nResult++;
								else
									if( pQuestProp->m_nEndCondGuildNumComp == 0 )
									{
										if( nSize == pQuestProp->m_nEndCondGuildNum )
											nResult++;
									}
									else
										if( pQuestProp->m_nEndCondGuildNumComp == 1 )
										{
											if( nSize >= pQuestProp->m_nEndCondGuildNum )
												nResult++;
										}	
										else
											if( pQuestProp->m_nEndCondGuildNumComp == -1 )
											{
												if( nSize <= pQuestProp->m_nEndCondGuildNum )
													nResult++;
											}
							}
						}
				}

				//sun: 8, // __S8_PK
				// ���� �������� ������ ������ ������ ����
				int nBufResult = nResult + MAX_QUESTCONDITEM;
				if( pQuestProp->m_nEndCondOneItemNum == 0 )
					nResult += MAX_QUESTCONDITEM;
				else
					for( int i = 0; i < MAX_QUESTCONDITEM; i++ )
					{
						if( i < pQuestProp->m_nEndCondOneItemNum )
						{
							QuestPropItem* pEndCondItem = &pQuestProp->m_paEndCondOneItem[ i ];
							if( pEndCondItem->m_nSex == -1 || pEndCondItem->m_nSex == pMover->GetSex() )
							{
								if( pEndCondItem->m_nType == 0 )
								{
									if( pEndCondItem->m_nJobOrItem == -1 || pEndCondItem->m_nJobOrItem == pMover->GetJob() )
									{
										if( pMover->GetItemNum( pEndCondItem->m_nItemIdx ) >= pEndCondItem->m_nItemNum )
										{
											nResult = nBufResult;
											break;
										}
									}
									else
										nResult++;
								}
								else
									if( pEndCondItem->m_nType == 1 )
									{
										if( pEndCondItem->m_nJobOrItem == -1 || pMover->GetItemNum( pEndCondItem->m_nJobOrItem ) )
										{
											if( pMover->GetItemNum( pEndCondItem->m_nItemIdx ) >= pEndCondItem->m_nItemNum ) 
											{
												nResult = nBufResult;
												break;
											}
										}
										else
											nResult++;
									}
							}
						}
						else
							nResult++;
					}

					// ���� ������ ����  17 + MAX_QUESTCONDITEM
					if( pQuestProp->m_nEndCondItemNum == 0 )
						nResult += MAX_QUESTCONDITEM;
					else
						for( int i = 0; i < MAX_QUESTCONDITEM; i++ )
						{
							if( i < pQuestProp->m_nEndCondItemNum )
							{
								QuestPropItem* pEndCondItem = &pQuestProp->m_paEndCondItem[ i ];
								if( pEndCondItem->m_nSex == -1 || pEndCondItem->m_nSex == pMover->GetSex() )
								{
									if( pEndCondItem->m_nType == 0 )
									{
										if( pEndCondItem->m_nJobOrItem == -1 || pEndCondItem->m_nJobOrItem == pMover->GetJob() )
										{
											if( pEndCondItem->m_nItemIdx == 0 || pMover->GetItemNum( pEndCondItem->m_nItemIdx ) >= pEndCondItem->m_nItemNum ) 
												nResult++;
										}
										else
											nResult++;
									}
									else
										if( pEndCondItem->m_nType == 1 )
										{
											if( pEndCondItem->m_nJobOrItem == -1 || pMover->GetItemNum( pEndCondItem->m_nJobOrItem ) )
											{
												if( pEndCondItem->m_nItemIdx == 0 || pMover->GetItemNum( pEndCondItem->m_nItemIdx ) >= pEndCondItem->m_nItemNum ) 
													nResult++;
											}
											else
												nResult++;
										}
								}
							}
							else
								nResult++;
						}

						// 21
						if( pQuestProp->m_nEndCondTSP == 0 )
							++nResult;
						else
						{
							if( pMover->GetCampusPoint() >= pQuestProp->m_nEndCondTSP )
								++nResult;
						}

						// 22
						if( pQuestProp->m_nEndCondGuildWarWin == 0 )
						{
							++nResult;
						}
						else
						{
							CGuild* pGuild = g_GuildMng.GetGuild( pMover->m_idGuild );
							if( pGuild != NULL )
							{
								if( pGuild->m_nWin >= pQuestProp->m_nEndCondGuildWarWin )		// ���� �¼�..
								{
									++nResult;
								}
							}
						}

						// ������ 17 + MAX_QUESTCONDITEM�� ����Ʈ ���� ���� 
						//sun: 8, // __S8_PK
						//sun: 9, 9-10�� ��
						//sun: 10�� ���½ý���	Neuz, World, Trans


						if( nResult == 20 + MAX_QUEST_KILL_NPC_COUNT + MAX_QUESTCONDITEM + MAX_QUESTCONDITEM ) 
						{
							// ��� ���� �޼� ������ Ÿ�̸� ī��Ʈ ���� ��Ű�� 
							// ���Ŀ� �κ��丮�� �����ؼ� ����Ʈ�� �Ϸ� ����Ű���� �ð��� �����ȴ�.
							if( pCurQuest->m_wTime )
								pCurQuest->m_wTime |= 0x8000;
							return true;
						}
	}

	return false;
}

#ifdef __WORLDSERVER
#include "../WorldServer/User.h"
#endif // __WORLDSERVER
bool __IsBeginQuestCondition( CMover* pMover, int nQuestId )
{
	if( IsValidObj( pMover ) == FALSE )
	{
		return false;
	}

	QuestProp* pQuestProp = prj.m_aPropQuest.GetAt( nQuestId );
	if( pQuestProp )
	{
		int nResult = 0;
		LPQUEST pCurQuest = pMover->GetQuest( nQuestId );
		BOOL bComplete = pMover->IsCompleteQuest( nQuestId );
		// ����Ʈ�� �����ϰų�, �̹� �Ϸ�� ���� �ִ� ����Ʈ�� ���� ���� �ȵ�. �ݺ��ϱ� ���ؼ��� ������ �����Ͽ� �Ϸ� �迭�� ��� �Ѵ�.
		if( pCurQuest || bComplete ) 
			return false;

#ifdef __WORLDSERVER
		// ���� �������� ������ �� �κ��丮�� ��ĭ�� �ִ��� �˻�
		vector<T_ADDITEM_INFO> vecAddItemInfo;
		vecAddItemInfo.clear();

		for( int j = 0; j < 4; ++j )
		{
			if( pQuestProp->m_nBeginSetAddItemIdx[ j ] )
			{
				T_ADDITEM_INFO tAddItemInfo;

				tAddItemInfo.dwItemId	= pQuestProp->m_nBeginSetAddItemIdx[ j ];
				tAddItemInfo.nItemNum	= pQuestProp->m_nBeginSetAddItemNum[ j ];

				vecAddItemInfo.push_back( tAddItemInfo );
			}
		}

		if( static_cast<FLWSUser*>(pMover)->m_Inventory.IsEnoughEmptySpace( vecAddItemInfo ) == false )
		{
			static_cast<FLWSUser*>(pMover)->AddDefinedText( TID_GAME_LACKSPACE );
			return false;
		}
#endif // __WORLDSERVER
		// ���� ����Ʈ 1,2,3,4,5,6
		for( int i = 0; i < 6; i++ )
		{
			if( pQuestProp->m_anBeginCondPreviousQuest[ i ] == 0 ) 
				nResult++;
			else
			{
				LPQUEST pPreQuest = pMover->GetQuest( pQuestProp->m_anBeginCondPreviousQuest[ i ] );
				BOOL bPreComplete = pMover->IsCompleteQuest( pQuestProp->m_anBeginCondPreviousQuest[ i ] );
				if( pQuestProp->m_nBeginCondPreviousQuestType == 0 )
				{
					if( pPreQuest || bPreComplete )
						nResult++;
				}
				else
					if( pQuestProp->m_nBeginCondPreviousQuestType == 1 )
					{
						if( pPreQuest == NULL && bPreComplete )
							nResult++;
					}
					else
						if( pQuestProp->m_nBeginCondPreviousQuestType == 2 )
						{
							if( pPreQuest && bPreComplete == FALSE )
								nResult++;
						}
			}
		}
		// ���� ����Ʈ 7,8,9,10,11,12
		for( int i = 0; i < 6; i++ )
		{
			if( pQuestProp->m_anBeginCondExclusiveQuest[ i ] == 0 ) 
				nResult++;
			else
			{
				LPQUEST pPreQuest = pMover->GetQuest( pQuestProp->m_anBeginCondExclusiveQuest[ i ] );
				BOOL bPreComplete = pMover->IsCompleteQuest( pQuestProp->m_anBeginCondPreviousQuest[ i ] );
				if( pPreQuest == NULL && bPreComplete == FALSE )
					nResult++;
			}
		}
		// ���� 13
		if( pQuestProp->m_nBeginCondJobNum == 0 )
			nResult++;
		else
			for( int i = 0; i < pQuestProp->m_nBeginCondJobNum; i++ )
			{
				if( pQuestProp->m_nBeginCondJob[ i ] == pMover->GetJob() )
				{
					nResult++;
					break;
				}
			}
			// ���� 14
			if( pQuestProp->m_nBeginCondLevelMin == 0 || ( pMover->GetLevel() >= pQuestProp->m_nBeginCondLevelMin && pMover->GetLevel() <= pQuestProp->m_nBeginCondLevelMax ) )
				nResult++;
			// ��Ƽ ���� 15
			if( pQuestProp->m_nBeginCondParty == 0 )
				nResult++;
			else
			{
				BOOL bParty = FALSE;
				BOOL bLeader = FALSE;
				int nSize = 0;
#ifdef __WORLDSERVER
				CParty* pParty = g_PartyMng.GetParty( pMover->m_idparty );
				if( pParty )
				{
					bParty = TRUE;
					nSize = pParty->GetSizeofMember();

				}
#else
				bParty = g_Party.IsMember( g_pPlayer->m_idPlayer );
				nSize = g_Party.GetSizeofMember();
#endif
				if( pQuestProp->m_nBeginCondParty == 1 && bParty == FALSE )
					nResult++;
				else
					if( pQuestProp->m_nBeginCondParty == 2 && bParty == TRUE )
					{
						if( pQuestProp->m_nBeginCondPartyLeader == -1 || pQuestProp->m_nBeginCondPartyLeader == bLeader )
						{
							if( pQuestProp->m_nBeginCondPartyNum == 0 )
								nResult++;
							else
								if( pQuestProp->m_nBeginCondPartyNumComp == 0 )
								{
									if( nSize == pQuestProp->m_nBeginCondPartyNum )
										nResult++;
								}
								else
									if( pQuestProp->m_nBeginCondPartyNumComp == 1 )
									{
										if( nSize >= pQuestProp->m_nBeginCondPartyNum )
											nResult++;
									}	
									else
										if( pQuestProp->m_nBeginCondPartyNumComp == -1 )
										{
											if( nSize <= pQuestProp->m_nBeginCondPartyNum )
												nResult++;
										}			
						}
					}
			}
			// ��� ���� 16
			if( pQuestProp->m_nBeginCondGuild == 0 )
				nResult++;
			else
			{
				BOOL bGuild = FALSE;
				int nLeader = 0;
				int nSize = 0;

				CGuild* pGuild	= g_GuildMng.GetGuild( pMover->m_idGuild );
				if( pGuild != NULL && pGuild->IsMember( pMover->m_idPlayer ) == TRUE )
				{
					bGuild	= TRUE;
					nSize	= pGuild->GetSize();
					if( pGuild->m_idMaster == pMover->m_idPlayer )
					{
						nLeader = 2;
					}
					else
					{
						nLeader = 1;
					}
				}

				if( pQuestProp->m_nBeginCondGuild == 1 && bGuild == FALSE )
					nResult++;
				else
					if( pQuestProp->m_nBeginCondGuild == 2 && bGuild == TRUE )
					{
						if( pQuestProp->m_nBeginCondGuildLeader == 0 || pQuestProp->m_nBeginCondGuildLeader == nLeader )
						{
							if( pQuestProp->m_nBeginCondGuildNum == 0 )
								nResult++;
							else
								if( pQuestProp->m_nBeginCondGuildNumComp == 0 )
								{
									if( nSize == pQuestProp->m_nBeginCondGuildNum )
										nResult++;
								}
								else
									if( pQuestProp->m_nBeginCondGuildNumComp == 1 )
									{
										if( nSize >= pQuestProp->m_nBeginCondGuildNum )
											nResult++;
									}	
									else
										if( pQuestProp->m_nBeginCondGuildNumComp == -1 )
										{
											if( nSize <= pQuestProp->m_nBeginCondGuildNum )
												nResult++;
										}			
						}
					}
			}
			// ���� 17
			if( pQuestProp->m_nBeginCondSex == -1 || pQuestProp->m_nBeginCondSex == pMover->GetSex() )
				nResult++;
			// ��ų 18
			if( pQuestProp->m_nBeginCondSkillIdx == 0 )
				nResult++;
			else
			{
				LPSKILL lpSkill = pMover->GetSkill( pQuestProp->m_nBeginCondSkillIdx );
				if( lpSkill )
				{
					if( pMover->CheckSkill( pQuestProp->m_nBeginCondSkillLvl ) && lpSkill->dwLevel >= (DWORD)( pQuestProp->m_nBeginCondSkillLvl ) )
						nResult++;
				}
			}
			//sun: 8, // __S8_PK
			// PK Value 19
			if( pQuestProp->m_nBeginCondPKValue == 0 )
				nResult++;
			else
			{
				if( pQuestProp->m_nBeginCondPKValue <= pMover->GetPKValue() )			
					nResult++;
			}
			// �������� ���°� �˻� MAX_QUESTCONDITEM
			if( pQuestProp->m_nBeginCondNotItemNum == 0 )
				nResult += MAX_QUESTCONDITEM;
			else
				for( int i = 0; i < MAX_QUESTCONDITEM; i++ )
				{
					if( i < pQuestProp->m_nBeginCondNotItemNum )
					{
						QuestPropItem* pBeginCondItem = &pQuestProp->m_paBeginCondNotItem[ i ];
						if( pBeginCondItem->m_nSex == -1 || pBeginCondItem->m_nSex == pMover->GetSex() )
						{
							if( pBeginCondItem->m_nType == 0 )
							{
								if( pBeginCondItem->m_nJobOrItem == -1 || pBeginCondItem->m_nJobOrItem == pMover->GetJob() )
								{
									if( pBeginCondItem->m_nItemIdx == 0 || pMover->GetItemNum( pBeginCondItem->m_nItemIdx ) < pBeginCondItem->m_nItemNum ) 
										nResult++;
								}
								else
									nResult++;
							}
							else
								if( pBeginCondItem->m_nType == 1 )
								{
									if( pBeginCondItem->m_nJobOrItem == -1 || pMover->GetItemNum( pBeginCondItem->m_nJobOrItem ) )
									{
										if( pBeginCondItem->m_nItemIdx == 0 || pMover->GetItemNum( pBeginCondItem->m_nItemIdx ) < pBeginCondItem->m_nItemNum ) 
											nResult++;
									}
									else
										nResult++;
								}
						}
					}
					else
						nResult++;
				}

				// ���� 21 
				if( pQuestProp->m_nBeginCondDisguiseMoverIndex == 0 )
					nResult++;
				else
				{
					if( pQuestProp->m_nBeginCondDisguiseMoverIndex == -1 && pMover->IsDisguise() == FALSE )
						nResult++;
					else
						if( pQuestProp->m_nBeginCondDisguiseMoverIndex == static_cast< int >( pMover->GetIndex() ) )
							nResult++;
				}
				//sun: 9, 9-10�� ��
				if( pQuestProp->m_nBeginCondPetExp == 0 )
					nResult++;
				else
				{
					CPet* pPet	= pMover->GetPet();
					if( pPet && pPet->GetExpPercent() >= pQuestProp->m_nBeginCondPetExp )
						nResult++;
				}
				if( pQuestProp->m_nBeginCondPetLevel == -1 )
					nResult++;
				else
				{
					CPet* pPet	= pMover->GetPet();
					if( pPet && pPet->GetLevel() == pQuestProp->m_nBeginCondPetLevel )
						nResult++;
				}

				//sun: 12, Ʃ�丮�� ����
				if( pQuestProp->m_nBeginCondTutorialState == -1
					|| pMover->GetTutorialState() >= pQuestProp->m_nBeginCondTutorialState )
					nResult++;

				// ���� ������ ����  21 + MAX_QUESTCONDITEM
				if( pQuestProp->m_nBeginCondItemNum == 0 )
					nResult += MAX_QUESTCONDITEM;
				else
					for( int i = 0; i < MAX_QUESTCONDITEM; i++ )
					{
						if( i < pQuestProp->m_nBeginCondItemNum )
						{
							QuestPropItem* pBeginCondItem = &pQuestProp->m_paBeginCondItem[ i ];
							if( pBeginCondItem->m_nSex == -1 || pBeginCondItem->m_nSex == pMover->GetSex() )
							{
								if( pBeginCondItem->m_nType == 0 )
								{
									if( pBeginCondItem->m_nJobOrItem == -1 || pBeginCondItem->m_nJobOrItem == pMover->GetJob() )
									{
										if( pBeginCondItem->m_nItemIdx == 0 || pMover->GetItemNum( pBeginCondItem->m_nItemIdx ) >= pBeginCondItem->m_nItemNum ) 
											nResult++;
									}
									else
										nResult++;
								}
								else
									if( pBeginCondItem->m_nType == 1 )
									{
										if( pBeginCondItem->m_nJobOrItem == -1 || pMover->GetItemNum( pBeginCondItem->m_nJobOrItem ) )
										{
											if( pBeginCondItem->m_nItemIdx == 0 || pMover->GetItemNum( pBeginCondItem->m_nItemIdx ) >= pBeginCondItem->m_nItemNum ) 
												nResult++;
										}
										else
											nResult++;
									}
							}
						}
						else
							nResult++;
					}

					if( pQuestProp->m_nBeginCondTSP == 0 )
						++nResult;
					else
					{
						if( pMover->GetCampusPoint() >= pQuestProp->m_nBeginCondTSP )
							++nResult;
					}


#ifdef __GUILD_HOUSE_MIDDLE

					if( pQuestProp->m_nBeginCondGuildWarWin == 0 )
						++nResult;
					else
					{
						CGuild* pGuild = g_GuildMng.GetGuild( pMover->m_idGuild );
						if( pGuild != NULL )
						{
							if( pGuild->m_nWin >= pQuestProp->m_nBeginCondGuildWarWin )		// ���� �¼�..
							{
								++nResult;
							}
						}
					}

#endif // __GUILD_HOUSE_MIDDLE

					//sun: 12, Ʃ�丮�� ����

#ifdef __GUILD_HOUSE_MIDDLE

					if( nResult == 25 + MAX_QUESTCONDITEM + MAX_QUESTCONDITEM )

#else // __GUILD_HOUSE_MIDDLE

					if( nResult == 24 + MAX_QUESTCONDITEM + MAX_QUESTCONDITEM )

#endif // __GUILD_HOUSE_MIDDLE
						return true;
	}

	return false;
}

bool __IsNextLevelQuest( CMover* pMover, int nQuestId )
{
	QuestProp* pQuestProp = prj.m_aPropQuest.GetAt( nQuestId );
	if( pQuestProp )
	{
		int nResult = 0;
		LPQUEST pCurQuest = pMover->GetQuest( nQuestId );
		BOOL bComplete = pMover->IsCompleteQuest( nQuestId );
		// ����Ʈ�� �����ϰų�, �̹� �Ϸ�� ���� �ִ� ����Ʈ�� ���� ���� �ȵ�. �ݺ��ϱ� ���ؼ��� ������ �����Ͽ� �Ϸ� �迭�� ��� �Ѵ�.
		if( pCurQuest || bComplete ) 
			return false;

		// ���� ����Ʈ 1,2,3,4,5,6
		for( int i = 0; i < 6; i++ )
		{
			if( pQuestProp->m_anBeginCondPreviousQuest[ i ] == 0 ) 
				nResult++;
			else
			{
				LPQUEST pPreQuest = pMover->GetQuest( pQuestProp->m_anBeginCondPreviousQuest[ i ] );
				BOOL bPreComplete = pMover->IsCompleteQuest( pQuestProp->m_anBeginCondPreviousQuest[ i ] );
				if( pQuestProp->m_nBeginCondPreviousQuestType == 0 )
				{
					if( pPreQuest || bPreComplete )
						nResult++;
				}
				else
					if( pQuestProp->m_nBeginCondPreviousQuestType == 1 )
					{
						if( pPreQuest == NULL && bPreComplete )
							nResult++;
					}
					else
						if( pQuestProp->m_nBeginCondPreviousQuestType == 2 )
						{
							if( pPreQuest && bPreComplete == FALSE )
								nResult++;
						}
			}
		}
		// ���� ����Ʈ 7,8,9,10,11,12
		for( int i = 0; i < 6; i++ )
		{
			if( pQuestProp->m_anBeginCondExclusiveQuest[ i ] == 0 ) 
				nResult++;
			else
			{
				LPQUEST pPreQuest = pMover->GetQuest( pQuestProp->m_anBeginCondExclusiveQuest[ i ] );
				BOOL bPreComplete = pMover->IsCompleteQuest( pQuestProp->m_anBeginCondPreviousQuest[ i ] );
				if( pPreQuest == NULL && bPreComplete == FALSE )
					nResult++;
			}
		}
		// ���� 13
		if( pQuestProp->m_nBeginCondJobNum == 0 )
			nResult++;
		else
			for( int i = 0; i < pQuestProp->m_nBeginCondJobNum; i++ )
			{
				if( pQuestProp->m_nBeginCondJob[ i ] == pMover->GetJob() )
				{
					nResult++;
					break;
				}
			}
			// ���� 14
			if( pMover->GetLevel() < pQuestProp->m_nBeginCondLevelMin && pMover->GetLevel() + 5 >= pQuestProp->m_nBeginCondLevelMin )
				nResult++;
			// ��Ƽ ���� 15
			if( pQuestProp->m_nBeginCondParty == 0 )
				nResult++;
			else
			{
				BOOL bParty = FALSE;
				BOOL bLeader = FALSE;
				int nSize = 0;
#ifdef __WORLDSERVER
				CParty* pParty = g_PartyMng.GetParty( pMover->m_idparty );
				if( pParty )
				{
					bParty = TRUE;
					nSize = pParty->GetSizeofMember();

				}
#else
				bParty = g_Party.IsMember( g_pPlayer->m_idPlayer );
				nSize = g_Party.GetSizeofMember();
#endif
				if( pQuestProp->m_nBeginCondParty == 1 && bParty == FALSE )
					nResult++;
				else
					if( pQuestProp->m_nBeginCondParty == 2 && bParty == TRUE )
					{
						if( pQuestProp->m_nBeginCondPartyLeader == -1 || pQuestProp->m_nBeginCondPartyLeader == bLeader )
						{
							if( pQuestProp->m_nBeginCondPartyNum == 0 )
								nResult++;
							else
								if( pQuestProp->m_nBeginCondPartyNumComp == 0 )
								{
									if( nSize == pQuestProp->m_nBeginCondPartyNum )
										nResult++;
								}
								else
									if( pQuestProp->m_nBeginCondPartyNumComp == 1 )
									{
										if( nSize >= pQuestProp->m_nBeginCondPartyNum )
											nResult++;
									}	
									else
										if( pQuestProp->m_nBeginCondPartyNumComp == -1 )
										{
											if( nSize <= pQuestProp->m_nBeginCondPartyNum )
												nResult++;
										}			
						}
					}
			}
			// ��� ���� 16
			if( pQuestProp->m_nBeginCondGuild == 0 )
				nResult++;
			else
			{
				BOOL bGuild = FALSE;
				int nLeader = 0;
				int nSize = 0;

				CGuild* pGuild	= g_GuildMng.GetGuild( pMover->m_idGuild );
				if( pGuild != NULL && pGuild->IsMember( pMover->m_idPlayer ) == TRUE )
				{
					bGuild	= TRUE;
					nSize	= pGuild->GetSize();
					if( pGuild->m_idMaster == pMover->m_idPlayer )
					{
						nLeader = 2;
					}
					else
					{
						nLeader = 1;
					}
				}

				if( pQuestProp->m_nBeginCondGuild == 1 && bGuild == FALSE )
					nResult++;
				else
					if( pQuestProp->m_nBeginCondGuild == 2 && bGuild == TRUE )
					{
						if( pQuestProp->m_nBeginCondGuildLeader == 0 || pQuestProp->m_nBeginCondGuildLeader == nLeader )
						{
							if( pQuestProp->m_nBeginCondGuildNum == 0 )
								nResult++;
							else
								if( pQuestProp->m_nBeginCondGuildNumComp == 0 )
								{
									if( nSize == pQuestProp->m_nBeginCondGuildNum )
										nResult++;
								}
								else
									if( pQuestProp->m_nBeginCondGuildNumComp == 1 )
									{
										if( nSize >= pQuestProp->m_nBeginCondGuildNum )
											nResult++;
									}	
									else
										if( pQuestProp->m_nBeginCondGuildNumComp == -1 )
										{
											if( nSize <= pQuestProp->m_nBeginCondGuildNum )
												nResult++;
										}			
						}
					}
			}
			// ���� 17
			if( pQuestProp->m_nBeginCondSex == -1 || pQuestProp->m_nBeginCondSex == pMover->GetSex() )
				nResult++;
			// ��ų 18
			if( pQuestProp->m_nBeginCondSkillIdx == 0 )
				nResult++;
			else
			{
				LPSKILL lpSkill = pMover->GetSkill( pQuestProp->m_nBeginCondSkillIdx );
				if( lpSkill )
				{
					if( pMover->CheckSkill( pQuestProp->m_nBeginCondSkillLvl ) && (int)( lpSkill->dwLevel ) >= pQuestProp->m_nBeginCondSkillLvl )
						nResult++;
				}
			}
			//sun: 8, // __S8_PK
			// PK Value 19
			if( pQuestProp->m_nBeginCondPKValue == 0 )
				nResult++;
			else
			{
				if( pQuestProp->m_nBeginCondPKValue <= pMover->GetPKValue() )			
					nResult++;
			}
			// �������� ���°� �˻� MAX_QUESTCONDITEM
			if( pQuestProp->m_nBeginCondNotItemNum == 0 )
				nResult += MAX_QUESTCONDITEM;
			else
				for( int i = 0; i < MAX_QUESTCONDITEM; i++ )
				{
					if( i < pQuestProp->m_nBeginCondNotItemNum )
					{
						QuestPropItem* pBeginCondItem = &pQuestProp->m_paBeginCondNotItem[ i ];
						if( pBeginCondItem->m_nSex == -1 || pBeginCondItem->m_nSex == pMover->GetSex() )
						{
							if( pBeginCondItem->m_nType == 0 )
							{
								if( pBeginCondItem->m_nJobOrItem == -1 || pBeginCondItem->m_nJobOrItem == pMover->GetJob() )
								{
									if( pBeginCondItem->m_nItemIdx == 0 || pMover->GetItemNum( pBeginCondItem->m_nItemIdx ) < pBeginCondItem->m_nItemNum ) 
										nResult++;
								}
								else
									nResult++;
							}
							else
								if( pBeginCondItem->m_nType == 1 )
								{
									if( pBeginCondItem->m_nJobOrItem == -1 || pMover->GetItemNum( pBeginCondItem->m_nJobOrItem ) )
									{
										if( pBeginCondItem->m_nItemIdx == 0 || pMover->GetItemNum( pBeginCondItem->m_nItemIdx ) < pBeginCondItem->m_nItemNum ) 
											nResult++;
									}
									else
										nResult++;
								}
						}
					}
					else
						nResult++;
				}

				// ���� 21 
				if( pQuestProp->m_nBeginCondDisguiseMoverIndex == 0 )
					nResult++;
				else
				{
					if( pQuestProp->m_nBeginCondDisguiseMoverIndex == -1 && pMover->IsDisguise() == FALSE )
						nResult++;
					else
						if( pQuestProp->m_nBeginCondDisguiseMoverIndex == static_cast< int >( pMover->GetIndex() ) )
							nResult++;
				}

				//sun: 9, 9-10�� ��
				if( pQuestProp->m_nBeginCondPetExp == 0 )
					nResult++;
				else
				{
					CPet* pPet	= pMover->GetPet();
					if( pPet && pPet->GetExpPercent() >= pQuestProp->m_nBeginCondPetExp )
						nResult++;
				}
				if( pQuestProp->m_nBeginCondPetLevel == -1 )
					nResult++;
				else
				{
					CPet* pPet	= pMover->GetPet();
					if( pPet && pPet->GetLevel() == pQuestProp->m_nBeginCondPetLevel )
						nResult++;
				}
				//sun: 12, Ʃ�丮�� ����
				if( pQuestProp->m_nBeginCondTutorialState == -1
					|| pMover->GetTutorialState() >= pQuestProp->m_nBeginCondTutorialState )
					nResult++;

				// ���� ������ ����  21 + MAX_QUESTCONDITEM
				if( pQuestProp->m_nBeginCondItemNum == 0 )
					nResult += MAX_QUESTCONDITEM;
				else
					for( int i = 0; i < MAX_QUESTCONDITEM; i++ )
					{
						if( i < pQuestProp->m_nBeginCondItemNum )
						{
							QuestPropItem* pBeginCondItem = &pQuestProp->m_paBeginCondItem[ i ];
							if( pBeginCondItem->m_nSex == -1 || pBeginCondItem->m_nSex == pMover->GetSex() )
							{
								if( pBeginCondItem->m_nType == 0 )
								{
									if( pBeginCondItem->m_nJobOrItem == -1 || pBeginCondItem->m_nJobOrItem == pMover->GetJob() )
									{
										if( pBeginCondItem->m_nItemIdx == 0 || pMover->GetItemNum( pBeginCondItem->m_nItemIdx ) >= pBeginCondItem->m_nItemNum ) 
											nResult++;
									}
									else
										nResult++;
								}
								else
									if( pBeginCondItem->m_nType == 1 )
									{
										if( pBeginCondItem->m_nJobOrItem == -1 || pMover->GetItemNum( pBeginCondItem->m_nJobOrItem ) )
										{
											if( pBeginCondItem->m_nItemIdx == 0 || pMover->GetItemNum( pBeginCondItem->m_nItemIdx ) >= pBeginCondItem->m_nItemNum ) 
												nResult++;
										}
										else
											nResult++;
									}
							}
						}
						else
							nResult++;
					}

					if( pQuestProp->m_nBeginCondTSP == 0 )
						++nResult;
					else
					{
						if( pMover->GetCampusPoint() >= pQuestProp->m_nBeginCondTSP )
							++nResult;
					}

#ifdef __GUILD_HOUSE_MIDDLE

					if( pQuestProp->m_nBeginCondGuildWarWin == 0 )
						++nResult;
					else
					{
						CGuild* pGuild = g_GuildMng.GetGuild( pMover->m_idGuild );
						if( pGuild != NULL )
						{
							if( pGuild->m_nWin >= pQuestProp->m_nBeginCondGuildWarWin )		// ���� �¼�..
							{
								++nResult;
							}
						}
					}

#endif // __GUILD_HOUSE_MIDDLE

					//sun: 12, Ʃ�丮�� ����
#ifdef __GUILD_HOUSE_MIDDLE

					if( nResult == 25 + MAX_QUESTCONDITEM + MAX_QUESTCONDITEM )

#else // __GUILD_HOUSE_MIDDLE

					if( nResult == 24 + MAX_QUESTCONDITEM + MAX_QUESTCONDITEM )

#endif // __GUILD_HOUSE_MIDDLE
						return true;
	}

	return false;
}

//sun: 8, // __S8_PK
void CMover::SetPKValue( int nValue )
{
	if( nValue >= 0 )	// overflow!!
		m_nPKValue = nValue;
}
void CMover::SetPKPropensity( DWORD dwValue )
{
	int nValue = dwValue;

	if( nValue >= 0 )	// overflow!!
		m_dwPKPropensity = dwValue;
}

DWORD CMover::NextPKPropensity( int nPKValue )
{
	int nResult = 60 * nPKValue * nPKValue;
	if( nResult < 0 )	// overflow!!
		nResult = 0x7fffffff;
	return nResult;
}

void CMover::SetJobLevel( int /*nLevel*/, int /*nJob*/ )
{
}

#ifdef __WORLDSERVER
void CMover::ProcessSFXDamage( void )
{
	ProcessSFXExpire();

	map<OBJID, queue<SFXHIT_INFO> >::iterator iInfo	= m_mapSFXInfo.begin();
	while( iInfo != m_mapSFXInfo.end() )
	{
		map<OBJID, queue<SFXHIT_INFO> >::iterator iInfo2	= iInfo;
		++iInfo;
		OBJID idTarget	= iInfo2->first;
		queue<SFXHIT_INFO> &qInfo	= iInfo2->second;
		map<OBJID, queue<SFXHIT_COUNT> >::iterator iCount	= m_mapSFXCount.find( idTarget );
		if( iCount == m_mapSFXCount.end() )
			continue;
		queue<SFXHIT_COUNT> &qCount	= iCount->second;
		// qInfo, qCount
		while( qInfo.size() > 0 && qCount.size() > 0 )
		{
			SFXHIT_INFO &si	= qInfo.front();
			SFXHIT_COUNT &sc	= qCount.front();
			if( si.dwSkill == sc.dwSkill )
			{
				if( AttackBySFX( this, si ) == FALSE )	// ������ ó��
				{
					m_mapSFXInfo.erase( idTarget );
					m_mapSFXCount.erase( idTarget );
					break;
				}
				qInfo.pop();
				qCount.pop();
			}
			else
			{
				FLTRACE_LOG( PROGRAM_NAME, _T( "���� ó��" ) );
				if( si.dwTickCount > sc.dwTickCount )
				{
					while( qCount.size() > 0 )
					{
						SFXHIT_COUNT &sc1	= qCount.front();
						if( si.dwSkill == sc1.dwSkill )
							break;
						qCount.pop();
					}
				}
				else if( si.dwTickCount < sc.dwTickCount )
				{
					while( qInfo.size() > 0 )
					{
						SFXHIT_INFO &si1	= qInfo.front();
						if( si1.dwSkill == sc.dwSkill )
							break;
						qInfo.pop();
					}
				}
				else
				{
					// print
					qInfo.pop();
					qCount.pop();
				}
			}
		}
	}
}

void CMover::ProcessSFXExpire( void )
{
	DWORD dwTickCount	= GetTickCount();

	// 1.1
	map<OBJID, queue<SFXHIT_INFO> >::iterator iInfo	= m_mapSFXInfo.begin();
	while( iInfo != m_mapSFXInfo.end() )
	{
		map<OBJID, queue<SFXHIT_INFO> >::iterator i2	= iInfo;
		++iInfo;
		OBJID idTarget	= i2->first;
		queue<SFXHIT_INFO> &q	= i2->second;

		while( q.size() > 0 )
		{
			SFXHIT_INFO &si	= q.front();
			if( si.dwTickCount + SEC( 10 ) < dwTickCount )	// ���� �ð� ���
			{
				q.pop();	// ���� ���� ����
				FLTRACE_LOG( PROGRAM_NAME, _T( "idTarget=%d, INFO �������� ���� ��" ), idTarget );
				continue;
			}
			else
				break;
		}
		if( q.size() == 0 )
		{
			m_mapSFXInfo.erase( idTarget );
			FLTRACE_LOG( PROGRAM_NAME, _T( "idTarget=%d, INFO ���� ����" ), idTarget);
		}
	}
	// 1.2
	map<OBJID, queue<SFXHIT_COUNT> >::iterator iCount	= m_mapSFXCount.begin();
	while( iCount != m_mapSFXCount.end() )
	{
		map<OBJID, queue<SFXHIT_COUNT> >::iterator i2	= iCount;
		++iCount;
		OBJID idTarget	= i2->first;
		queue<SFXHIT_COUNT> &q	= i2->second;

		while( q.size() > 0 )
		{
			SFXHIT_COUNT &sc	= q.front();
			if( sc.dwTickCount + SEC( 10 ) < dwTickCount )	// ���� �ð� ���
			{
				q.pop();	// ���� ���� ����
				FLTRACE_LOG( PROGRAM_NAME, _T( "idTarget=%d, COUNT �������� ���� ��" ), idTarget );
				continue;
			}
			else
				break;
		}
		if( q.size() == 0 )
		{
			m_mapSFXCount.erase( idTarget );
			FLTRACE_LOG( PROGRAM_NAME, _T( "idTarget=%d, COUNT ���� ����" ), idTarget);
		}
	}
}
#endif	// __WORLDSERVER

#ifdef __CLIENT
void CMover::ProcessEyeFlash()
{
	//sun: �����ڰŸ���... Neuz
	if( IsPlayer() && m_pModel && HasBuffByIk3(IK3_TEXT_DISGUISE) == FALSE && IsDisguise() == FALSE )
	{
		CModelObject* pModelObj = (CModelObject*)m_pModel;

		O3D_ELEMENT* pElement = NULL;
		pElement = pModelObj->GetParts(PARTS_HEAD);

		if( pElement && pElement->m_pObject3D )
		{
			GMOBJECT* pGmObj = pElement->m_pObject3D->GetGMOBJECT(0);

			if( pGmObj )
			{
				if( m_fDistCamera <= 11.0f )
				{
					if( IsDie() )
					{
						(*pGmObj->m_pMtrlBlkTexture) = CMover::m_pTextureEyeFlash[m_bySex][m_dwHeadMesh];
					}
					else
					{
						if( m_tmEye[0].IsTimeOut() )
						{
							FLOAT fSec = 50;  // ���ڰŸ��� �ð�...
							(*pGmObj->m_pMtrlBlkTexture) = CMover::m_pTextureEyeFlash[m_bySex][m_dwHeadMesh];
							m_tmEye[0].Set( fSec+1000 );
							m_tmEye[1].Set( fSec );
						}

						if( m_tmEye[1].IsTimeOut() )
						{
							// �����ִ� �ð�...
							FLOAT fSec = xRandomF( 2 ) + 5;
							fSec *= 1000.0f;

							if( xRandom( 100 ) < 10 )
								fSec = 500.0f;

							(*pGmObj->m_pMtrlBlkTexture) = CMover::m_pTextureEye[m_bySex][m_dwHeadMesh];
							m_tmEye[1].Set( fSec+1000 );
							m_tmEye[0].Set( fSec );
						}	
					}
				}
				else
				{
					if( IsDie() )
						(*pGmObj->m_pMtrlBlkTexture) = CMover::m_pTextureEyeFlash[m_bySex][m_dwHeadMesh];
					else
						(*pGmObj->m_pMtrlBlkTexture) = CMover::m_pTextureEye[m_bySex][m_dwHeadMesh];
				}
			}
		}
	}
}

void CMover::WingMotionSetting( const CModelObject* pModel )
{
	if( m_pRide == NULL && m_pRide->m_pBone == NULL )
		return;

///>_SUN_RIDER_ANIMATION --BEGIN
	//STAND�� ACC���¿��� LTURN, RTURN���°� �ߺ��ǹǷ�
	//else if�� ó���ϱ����ؼ� L,RTURN�� ���� ó���ؾ� �Ѵ�.
	if( m_pActMover->GetTurnState() & OBJSTA_LTURN ) 
	{
		if( m_eWingStateFlag != TURNING_L )
		{
			CString strMotion = m_pRide->GetMotionFileName( _T("lturn") );
			FLASSERT( strMotion != _T("") );
			m_pRide->LoadMotion( strMotion );
			m_pRide->SetMotionBlending( TRUE );
			m_pRide->SetLoop( ANILOOP_LOOP );
			m_eWingStateFlag = TURNING_L;
		}
	}
	else if( m_pActMover->GetTurnState() & OBJSTA_RTURN ) 
	{
		if( m_eWingStateFlag != TURNING_R )
		{
			CString strMotion = m_pRide->GetMotionFileName( _T("rturn") );
			FLASSERT( strMotion != _T("") );
			m_pRide->LoadMotion( strMotion );
			m_pRide->SetMotionBlending( TRUE );
			m_pRide->SetLoop( ANILOOP_LOOP );
			m_eWingStateFlag = TURNING_R;
		}
	}
	else 
///>_SUN_RIDER_ANIMATION --END
	if( ( m_pActMover->GetState() & OBJSTA_STAND ) && ( D3DXVec3LengthSq( &m_pActMover->m_vDelta ) <= 0.000f ) && pModel->m_fFrameCurrent == 0.0f )
	{
		if( m_eWingStateFlag != FLOATING )
		{
			CString strMotion = m_pRide->GetMotionFileName( _T("stand") );
			FLASSERT( strMotion != _T("") );
			m_pRide->LoadMotion( strMotion );
			m_pRide->SetMotionBlending( TRUE );
			m_pRide->SetLoop( ANILOOP_LOOP );
			m_eWingStateFlag = FLOATING;
		}
	}
	else if( m_pActMover->GetStateFlag() & OBJSTAF_ACC )
	{
		if( m_eWingStateFlag != FLYING )
		{
			CString strMotion = m_pRide->GetMotionFileName( _T("walk") );
			FLASSERT( strMotion != _T("") );
			m_pRide->LoadMotion( strMotion );
			m_pRide->SetMotionBlending( TRUE );
			m_pRide->SetLoop( ANILOOP_LOOP );
			m_eWingStateFlag = FLYING;
		}
	}
}

float CMover::GetRideFrameSpeed( void )
{
	float fFrameSpeed = 1.0f;
	if( m_pRide )
	{
		float fNowLengthSq = D3DXVec3LengthSq( &m_pActMover->m_vDelta );
		fFrameSpeed = 1.0f + ( fabs( fNowLengthSq - m_fOldLengthSq ) * 7000.0f );
		m_fOldLengthSq = fNowLengthSq;

		//_SUN_RIDER_ANIMATION
		if( m_pActMover->GetTurnState() == OBJSTA_LTURN || m_pActMover->GetTurnState() == OBJSTA_RTURN )
			fFrameSpeed = 1.0f;
		else if( ( m_pActMover->GetStateFlag() & OBJSTAF_ACC ) && ( m_pActMover->GetStateFlag() & OBJSTAF_TURBO ) )// ���� �����̰� �ͺ� ������ ��
			fFrameSpeed += 1.2f;

		if( fFrameSpeed < 1.0f )
			fFrameSpeed = 1.0f;

		if( fFrameSpeed > 4.0f )
			fFrameSpeed = 4.0f;
	}
	return fFrameSpeed;
}
#endif //__CLIENT

#ifdef __CLIENT
//sun: 8�� ���� ��ȯ Neuz, World, Trans
void CMover::CreateAngelParticle( D3DXVECTOR3 vPos )
{
	FLOAT fAngXZ, fAngH, fDist, fSpeed;
	D3DXVECTOR3 vVel;

	fAngXZ = GetAngle();
	fAngXZ += 180.0f; 
	fAngXZ = D3DXToRadian( fAngXZ );

	vPos.y += 1.1f;

	for( int j = 0; j < 2; j ++ )
	{
		vPos.y -= 0.03f;

		{
			fAngXZ = (float)(-45.0f + xRandomF(90));
			fAngXZ += GetAngle();
			fAngXZ += 180.0f;

			fAngXZ = D3DXToRadian( fAngXZ );
			fAngH = (float)(GetAngleX() + xRandomF(-30) + xRandomF(30) );
			fAngH = D3DXToRadian( fAngH );
			fSpeed = xRandomF(0.02f);

			fDist = cosf(-fAngH) * fSpeed;
			vVel.x = sinf(fAngXZ) * fDist;
			vVel.z = -cosf(fAngXZ) * fDist;
			vVel.y = -sinf(-fAngH) * fSpeed;		

			g_ParticleMng.CreateParticle( m_dwAngelKind + xRandom(3), vPos, vVel, GetPos().y + 0.5f );
		}	
	}
}


//sun: 9, 9-10�� ��
void CMover::CreatePetParticle( D3DXVECTOR3 vPos )
{
	FLOAT fAngXZ, fAngH, fDist, fSpeed;
	D3DXVECTOR3 vVel;

	fAngXZ = GetAngle();
	fAngXZ += 180.0f; 
	fAngXZ = D3DXToRadian( fAngXZ );

	for( int j = 0; j < 4; j ++ )
	{
		vPos.y -= 0.03f;

		{
			fAngXZ = (float)(-45.0f + xRandomF(360));
			fAngXZ += GetAngle();
			fAngXZ += 180.0f;

			fAngXZ = D3DXToRadian( fAngXZ );
			fAngH = (float)(GetAngleX() + xRandomF(-130) + xRandomF(130) );
			fAngH = D3DXToRadian( fAngH );
			fSpeed = xRandomF(0.015f);

			fDist = cosf(-fAngH) * fSpeed;
			vVel.x = sinf(fAngXZ) * fDist;
			vVel.z = -cosf(fAngXZ) * fDist;
			vVel.y = -sinf(-fAngH) * fSpeed;

			g_ParticleMng.CreateParticle( 13 + xRandom(11), vPos, vVel, GetPos().y + 0.5f );
		}	
	}
}

#ifdef _DEBUG
//_SUN_ENCHANT_EFFECT17
BOOL CMover::CreateEnchantEffect_NEW( const int nKind, const TCHAR* fname )
{
	if( nKind >= 2 || nKind < 0 )
	{
		FLASSERT( 0 );
		return FALSE;
	}

	CModelObject* pModel = DeleteEnchantEffect_NEW( nKind );

	BOOL bRst = FALSE;
	if( pModel )
	{
		pModel->m_pPartsEffectNew_Debug[ nKind ] = new CPartsWeaponEffect;
		CPartsWeaponEffect* pEffect = pModel->m_pPartsEffectNew_Debug[ nKind ];
		bRst = pEffect->Create( D3DDEVICE, D3DXVECTOR3( .0f, .0f, .0f ), const_cast<TCHAR*>(fname), D3DXVECTOR3( 1.0f, 1.0f, 1.0f ) );
		if( bRst == FALSE )
			DeleteEnchantEffect_NEW( nKind );
	}

	return bRst;
}

CModelObject* CMover::DeleteEnchantEffect_NEW( const int nKind )
{
	if( nKind >= 2 || nKind < 0 )
	{
		FLASSERT( 0 );
		return NULL;
	}

	CModelObject* pModel = (CModelObject*)GetModel( );
	if( !pModel)
		return NULL;

	SAFE_DELETE( pModel->m_pPartsEffectNew_Debug[nKind] );

	return pModel;
}

#endif //DEBUG

#endif //__CLIENT

//sun: 9, 9-10�� ��
void CMover::ProcessPet( void )
{
#ifdef __CLIENT
	// m_dwPetId �� �����ϴµ�, m_pPet�� ������, �����Ѵ�.
	// ������, ActiveMover�� ����, �ƴѰ�츦 �и��ؼ� ó���Ѵ�.

	if( HasActivatedSystemPet() )
	{
		if( m_pet.GetObj() == NULL )
		{
			DWORD dwIndex	= 0;

			if( IsActiveMover() )
			{
				FLItemElem* pItemElem	= GetPetItem();
				if( pItemElem && pItemElem->m_pPet )
					dwIndex	= pItemElem->m_pPet->GetIndex();
			}
			else	//
				dwIndex	= (DWORD)LOWORD( GetPetId() );

			if( dwIndex > 0 )
			{
				CMover* pPet	= (CMover *)CreateObj( D3DDEVICE, OT_MOVER, dwIndex );
				//sun: ?, __PET_1024
				if( m_pet.HasName() )
					pPet->SetName( m_pet.GetName() );

				pPet->InitMotion( MTI_STAND );
				pPet->UpdateLocalMatrix();
				pPet->m_pAIInterface	= new CAIEgg( pPet, GetId() );
				pPet->m_dwAIInterface	= AII_EGG;
				pPet->m_dwAIInterfaceState	= 2;
				pPet->SetPos( GetPos() );
				GetWorld()->AddObj( pPet, FALSE );
				m_pet.SetObj( pPet );

				pPet->AutoSynchronizer()->Initialize();

				if( IsActiveMover() )
					FLStrcpy( pPet->m_szCharacterKey, _countof( pPet->m_szCharacterKey ), "MaFl_Pet" );
				CreatePetSfx();
				m_pet.SetLevelup( 0 );

				if( FALSE == g_WndMng.GetWndBase( APP_PET_STATUS ) && this->IsActiveMover() )
				{
					SAFE_DELETE( g_WndMng.m_pWndPetStatus );
					g_WndMng.m_pWndPetStatus = new CWndPetStatus;
					g_WndMng.m_pWndPetStatus->Initialize();
				}

				//	if( FALSE == g_WndMng.GetWndBase( APP_BUFFPET_STATUS ) )
				//	{
				//		SAFE_DELETE( g_WndMng.m_pWndBuffPetStatus );
				//		g_WndMng.m_pWndBuffPetStatus = new CWndBuffPetStatus;
				//		g_WndMng.m_pWndBuffPetStatus->Initialize();
				//		}
			}
		}				
	}
	else 
	{
		if( m_pet.GetObj() )
		{
			m_pet.GetObj()->Delete();
			m_pet.SetObj( NULL );

			if( g_WndMng.GetWndBase( APP_PET_STATUS )  && this->IsActiveMover() )
				g_WndMng.m_pWndPetStatus->Destroy();

			//	if( g_WndMng.GetWndBase( APP_BUFFPET_STATUS ) && this->IsActiveMover( ) )
			//		g_WndMng.m_pWndBuffPetStatus->Destroy();
		}
	}

	if( m_pet.GetObj() != NULL )
	{
		if(IsActiveMover())
		{
			FLItemElem* m_pPetElem	= g_pPlayer->GetPetItem();
			if(m_pPetElem != NULL && m_pPetElem->m_pPet->GetLevel() == PL_S )
				CreatePetParticle( m_pet.GetObj()->GetScrPos());
		}
		else
		{
			DWORD dwLevel = (DWORD)HIWORD( GetPetId() );
			if(dwLevel == PL_S)
				CreatePetParticle( m_pet.GetObj()->GetScrPos());
		}
	}
#else	// __CLIENT
	ProcessPetAvail();
	ProcessPetEnergy();
	ProcessPetExp();
	ProcessVisPet();
#endif	// __CLIENT
}

FLItemElem* CMover::GetPetItem( void )
{
	if( HasActivatedSystemPet() )
	{
		FLItemElem* pItemElem	= static_cast<FLItemElem*>( GetItemId( m_dwPetId ) );
		if( pItemElem && pItemElem->IsPet() )
			return pItemElem;
	}
	return NULL;
}

CPet* CMover::GetPet( void )
{
	FLItemElem* pItemElem	= GetPetItem();
	if( pItemElem )
		return pItemElem->m_pPet;
	return NULL;
}

void CMover::PetLevelup( void )
{
#ifdef __CLIENT
	if( m_pet.GetObj() )	// ���� �ϸ�, ���μ������� ���
	{
		m_pet.SetLevelup( PF_PET_LEVEL_UP );
		m_pet.GetObj()->Delete();
		m_pet.SetObj( NULL );
	}
#else	// __CLIENT
	FLItemElem* pItemElem	= GetPetItem();
	CPet* pPet	= pItemElem->m_pPet;

	if( pPet->GetLevel() == PL_EGG )	// ��ȭ
	{
		pPet->SetKind( CPetProperty::GetInstance()->Hatch() );
		pPet->SetLife( 1 );

		((FLWSUser*)this)->SetHonorAdd(HS_HATCHING_EGG,HI_COUNT_CHECK);	//sun: 13, ����
	}

	pPet->IncLevel();
	pItemElem->m_dwItemId	= pPet->GetItemId();
	pPet->SetExp( 0 );
	pPet->SetEnergy( pPet->GetMaxEnergy() );

	BYTE nAvailLevel	= CPetProperty::GetInstance()->GetLevelupAvailLevel( pPet->GetLevel() );
	pPet->SetAvailLevel( pPet->GetLevel(), nAvailLevel );
	if( HasPet() )
		RemovePet();

	g_dpDBClient.CalluspPetLog( m_idPlayer, pItemElem->GetSerialNumber(), 0, PETLOGTYPE_LEVELUP, pPet );

	( (FLWSUser*)this )->AddPet( pPet, PF_PET_LEVEL_UP );	// �
	g_xWSUserManager->AddPetLevelup( this, MAKELONG( (WORD)pPet->GetIndex(), (WORD)pPet->GetLevel() ) );	// ��
#endif	// __CLIENT
}

#ifdef __WORLDSERVER
void CMover::ProcessPetAvail( void )
{
	CPet* pPet	= GetPet();
	if( pPet )
	{
		if( HasPet() == FALSE )
		{
			FLItemElem* pItemElem	= GetPetItem();
			DWORD dwDestParam;
			int nParam;
			pPet->GetAvailDestParam( dwDestParam, nParam );
			if( dwDestParam > 0 )
			{
				// wID: dwItemId
				// dwLevel: dwDestParam(H) + nParam(L)
				AddBuff( BUFF_PET, (WORD)( pItemElem->m_dwItemId ), MAKELONG( (WORD)nParam, (WORD)dwDestParam ), 0, 0, BT_TICK );	// TODO
				SetDestParam( dwDestParam, nParam, NULL_CHGPARAM, TRUE );
			}
			//sun: 12, �� ����
			SetDestParamRandomOptExtension( pItemElem );
		}
	}
	else
	{
		PetRelease();
	}
}

void CMover::ProcessPetEnergy( void )
{
	//#ifdef __INTERNALSERVER
	//	if( ( m_nCount & 3 ) != 0 )	// ����� 0.2�ʿ� 1p ����
	//#else	// __INTERNALSERVER
	if( ( m_nCount & 15 ) != 0 )	// ����� 1�ʿ� 1p ����
		//#endif	// __INTERNALSERVER
		return;

	CPet* pPet	= GetPet();
	if( pPet == NULL )
		return;

	if( pPet->GetLevel() == PL_EGG )
		return;

	if( HasBuff( BUFF_ITEM, ITEM_INDEX( 21032, II_SYS_SYS_SCR_PET_TONIC_A ) )
		|| HasBuff( BUFF_ITEM, ITEM_INDEX( 21033, II_SYS_SYS_SCR_PET_TONIC_B ) )
		)	// �� ���� ������(A)	// �� ���� ������(B)	���� ��� ���� ����.
		return;
	//sun: 12, �ش����������
	if( HasBuff( BUFF_ITEM, ITEM_INDEX( 26573, II_SYS_SYS_SCR_SPETGOOD ) )
		&& pPet->GetLevel() == PL_S 
		)	// S�� �� ���� ����	
		return;

	// �ڵ� ���� ����
	if( HasBuff( BUFF_ITEM, ITEM_INDEX( 21035, II_SYS_SYS_SCR_PET_FEED_POCKET ) )
		|| HasBuff( BUFF_ITEM, ITEM_INDEX( 26531, II_SYS_SYS_SCR_PET_FEED_POCKET02 ) )
		)
	{
		// ���̸� �˻��ؼ� ������ ���ҽ�Ų��.
		FLItemElem* ptr;
		DWORD dwMax	= m_Inventory.GetMax();
		for( DWORD i = 0 ; i < dwMax; i++ )
		{
			ptr	= m_Inventory.GetAtId( i );
			if( IsUsableItem( ptr ) && ptr->m_dwItemId == ITEM_INDEX( 21037, II_SYS_SYS_FEED_01 ) )	// ��� ������ ���̸�
			{
				if( ( m_nCount & 0X1F ) == 0 )	// 2�ʴ� ���� 1�Ҹ�		// 0723
					UpdateItem( (BYTE)( ptr->m_dwObjId ), UI_NUM, ptr->m_nItemNum - 1 );
				return;
			}
		}
	}

	FLItemElem* pItemElem	= GetPetItem();
	// ����� 0���ϸ�, ���� 1 ����
	int nEnergy		= (int)pPet->GetEnergy();
	if( --nEnergy <= 0 )
	{
		int nLife	= (int)pPet->GetLife();
		if( --nLife >= 0 )
		{
			// ������ 0 �̻��̸�, ��Ȱ
			pPet->SetLife( static_cast< WORD >( nLife ) );
			pPet->SetEnergy( CPetProperty::GetInstance()->GetMaxEnergy( pPet->GetLevel() ) );
			//			pPet->SetExp( 0 );

			g_dpDBClient.CalluspPetLog( m_idPlayer, pItemElem->GetSerialNumber(), 0, PETLOGTYPE_DEATH, pPet );
		}
		else
		{
			// ������ 0 �̸��̸�, ���� �Ҹ�
			pPet->SetLife( 0 );
			pPet->SetEnergy( 0 );
			pPet->SetExp( 0 );
			// ���� �������� ���, ���̸� ����̶�� ǥ��
			pItemElem->SetFlag( FLItemElem::expired );
			UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_FLAG, MAKELONG( pItemElem->m_dwObjIndex, pItemElem->m_byFlag ) );

			g_dpDBClient.CalluspPetLog( m_idPlayer, pItemElem->GetSerialNumber(), 0, PETLOGTYPE_DEATH, pPet );

			// �� ��ȯ ����
			PetRelease();
		}
	}
	else
	{
		pPet->SetEnergy( static_cast< WORD >( nEnergy ) );
	}
	( (FLWSUser*)this )->AddPetState( pItemElem->m_dwObjId , pPet->GetLife(), pPet->GetEnergy(), pPet->GetExp() );
}

void CMover::ProcessPetExp( void )
{
	//#ifdef __INTERNALSERVER
	//	if( ( m_nCount & 3 ) != 0 )	// 0.2��
	//#else	// __INTERNALSERVER
	if( ( m_nCount & 1023 ) != 0 )	// // ����ġ�� 1�п� 1ȸ ���
		//#endif	// // ����ġ�� 1�п� 1ȸ ���
		return;

	CPet* pPet	= GetPet();
	if( pPet == NULL )
		return;

	// ����� ���̰ų�, S�� �ƴϸ�, 
	if( pPet->GetLevel() != PL_EGG && pPet->GetLevel() != PL_S )
	{
		if( pPet->GetExp() == MAX_PET_EXP )
			return;

		float fFactor	= HasBuff( BUFF_ITEM, ITEM_INDEX( 21033, II_SYS_SYS_SCR_PET_TONIC_B ) )? 1.5F: 1.0F;

		// 100000 = 100%
		DWORD dwExp	= (DWORD)( pPet->GetExp() + CPetProperty::GetInstance()->GetIncrementExp( pPet->GetLevel() ) * fFactor );
		if( dwExp > MAX_PET_EXP )
			dwExp	= MAX_PET_EXP;
		pPet->SetExp( dwExp );
		( (FLWSUser*)this )->AddPetSetExp( dwExp );
	}
}

#endif	// __WORLDSERVER

#ifdef __CLIENT
void	CMover::CreatePetSfx( void )
{
	CMover* pPet	= m_pet.GetObj();
	if( !pPet )
		return;

	switch( m_pet.GetLevelup() )
	{
	case PF_PET_LEVEL_UP:
		//level up Sound
		PLAYSND( "PCSkillCastCheer.wav" );
		//level up Effect
		break;
	case PF_PET_LEVEL_DOWN:	// ���� : �� ���̸��� �Ǽ� ��� ����
		// ���� ȿ��
		break;
	case PF_PET_GET_AVAIL:	// ���� : �� ���̸��� ���� ��� ����
		if( IsActiveMover() )
		{
			CWndPetMiracle* pWndMiracle = (CWndPetMiracle*)g_WndMng.GetWndBase( APP_PET_MIRACLE );
			if(pWndMiracle != NULL)
			{
				CPet* pCPet	= GetPet();
				int nLevel = pCPet->GetLevel();
				pWndMiracle->ReceiveResult(pCPet->GetAvailLevel(nLevel-1), pCPet->GetAvailLevel(nLevel)); 
			}
		}
		break;
	}
}


BOOL	CMover::IsOptionRenderCostume(int nCostumeIdx)	
{	
		DWORD dwMode = 0;
		
		switch(nCostumeIdx)
		{
			case 0:
				dwMode = MODE_OPTION_DONT_RENDER_COSTUME1;
				break;
			case 1:
				dwMode = MODE_OPTION_DONT_RENDER_COSTUME2;
				break;
			case 2:
				dwMode = MODE_OPTION_DONT_RENDER_COSTUME3;
				break;
			case 3:
				dwMode = MODE_OPTION_DONT_RENDER_COSTUME4;
				break;
			case 4:
				dwMode = MODE_OPTION_DONT_RENDER_COSTUME5;
				break;

		}
		return !IsMode( dwMode );	
}
#endif	// __CLIENT
void CMover::PetRelease( void )
{
#ifdef __CLIENT
	if( m_pet.GetObj() )
	{
		m_pet.GetObj()->Delete();
		m_pet.SetObj( NULL );
	}
	SetPetId( NULL_ID );

	PLAYSND( "ItemGnCollector.wav" );

#else	// __CLIENT
	FLItemElem* pItemElem	= GetPetItem();
	if( !( pItemElem && pItemElem->m_pPet ) )
		return;
	CPetProperty* pProperty	= CPetProperty::GetInstance();
	CPet* pPet	= pItemElem->m_pPet;
	PPETPENALTY pPenalty	= pProperty->GetPenalty( pPet->GetLevel() );
	if( pPenalty )
	{
		if( pPenalty->fExp > 0.0f )
		{
			//Pet ��ȯ �� ���� �� D~A�������� ����ġ�� �Ұ� �Ǹ�,
			// �ּ� 0%������ �� �̻� �������� �ʴ´�. 
			DWORD dwExp	= (DWORD)( 100000 * pPenalty->fExp / 100.0F );
			if( dwExp > pPet->GetExp() )
				dwExp	= pPet->GetExp();
			pPet->SetExp( pPet->GetExp() - dwExp );
		}
		if( pPenalty->wEnergy > 0 )
		{
			// S�������� HP�� �����ϸ�, �ּ� 5P ���Ϸδ� �� �̻� �������� �ʴ´�. 
			int nHP	= (int)pPet->GetEnergy();
			int nPenalty	= (int)pPenalty->wEnergy;
			if( nHP < 5 )
				nPenalty	= 0;
			else if( nHP - nPenalty < 5 )
				nPenalty	= nHP - 5;
			pPet->SetEnergy( static_cast< WORD >( nHP - (WORD)nPenalty ) );
		}
		( (FLWSUser*)this )->AddPetState( pItemElem->m_dwObjId, pPet->GetLife(), pPet->GetEnergy(), pPet->GetExp() );
	}

	if( HasPet() )
		RemovePet();

	g_dpDBClient.CalluspPetLog( m_idPlayer, pItemElem->GetSerialNumber(), 0, PETLOGTYPE_RELEASE, pItemElem->m_pPet );
	SetPetId( NULL_ID );
	g_xWSUserManager->AddPetRelease( this );
#endif	// __CLIENT
}

//sun: 12, �� ����
// 11�� �������� ���� ��ȯ���� ���� Ȯ�� �Ұ�
// �̸� �����ϰ� �ϱ� ���� ���� ��ü�� ������ �ĺ��ڸ� �߰���
BOOL CMover::IsUsingEatPet( FLItemElem* pItemElem ) const
{
	if( HasActivatedEatPet() )	// ��ȯ���� ������ ������
	{
		CMover* pEatPet		= prj.GetMover( GetEatPetId() );	// ���� ��ü�� ã�Ƽ�
		if( IsValidObj( pEatPet ) )
		{
			// ������ �ĺ��ڰ� ������ ��ȯ���� �����̴�
			CAIPet* pAIPet	= static_cast<CAIPet*>( pEatPet->m_pAIInterface );
			if( pAIPet && pAIPet->GetPetItemId() == pItemElem->m_dwObjId )
				return TRUE;
		}
	}
	return FALSE;
}

BOOL CMover::IsUsing( FLItemElem* pItemElem )
{	// ������� �������ΰ�?
	//sun: 12, �� ����
	if( IsUsingEatPet( pItemElem ) )	// ��ȯ���� �����̸�
		return TRUE;

	if ( pItemElem->m_dwObjId == m_objIdVisPet )	// 2018.02.27 �������þ� ������ ������ ��ȯ �� ������� �ϰ� ����� �������� �����̳� ��ȯ�� ���� �̵��ϰ� �ٸ� �� ��ȯ�� �ٽ� ��ȯ�ϸ� ��ø���� �߻�
		return TRUE;
	//sun: 9, 9-10�� ��
	if( GetPetId() == pItemElem->m_dwObjId )
		return TRUE;

	if( pItemElem->m_dwItemId == ITEM_INDEX( 21035, II_SYS_SYS_SCR_PET_FEED_POCKET ) && pItemElem->m_dwKeepTime > 0
		&& HasBuff( BUFF_ITEM, (WORD)( pItemElem->m_dwItemId ) ) && !pItemElem->IsFlag( FLItemElem::expired ) )
		return TRUE;

	if( pItemElem->IsFlag( FLItemElem::is_using ) != FALSE )
		return TRUE;

#if defined __WORLDSERVER && defined __ENCHANT_BARUNA16	
	if( GetBarunaNPCSummoner().GetNPCItemOBJID() == pItemElem->m_dwObjId )		//@���ʸ��ϰ� �����ҵ�.
		return TRUE;
#endif //__ENCHANT_BARUNA16
	return FALSE;
}

//sun: 13, ����

void	CMover::SetHonorCount(int nIdx , int nCount )
{	
	if( nIdx < 0 || nIdx >= MAX_HONOR_TITLE)
		return;

	if( nCount < INT_MAX )
		m_aHonorTitle[nIdx] = nCount;	
}
void	CMover::SetHonorAdd(int nSmallIdx,int nLargeGroup,int nDefault )
{
#ifdef __WORLDSERVER
	FLASSERT( IsPlayer() );
	HonorData*	pHonorData = CTitleManager::Instance()->GetHonorDataByID(nSmallIdx,nLargeGroup);
	if(pHonorData)
	{
		if( nDefault == 0)
			SetHonorCount(pHonorData->nID,m_aHonorTitle[pHonorData->nID] + 1);
		else
			SetHonorCount(pHonorData->nID,nDefault);
		if(pHonorData->nNeed == GetHonorTitle(pHonorData->nID) )
		{
			( (FLWSUser*)this )->AddHonorListAck();
			g_dpDBClient.SendLogGetHonorTime(this,pHonorData->nID);
		}

	}

	/*
	int nIdx = CTitleManager::Instance()->GetIdx(nSmallIdx,nLargeGroup);
	if( nIdx >= 0)
	{
	SetHonorCount(nIdx,m_aHonorTitle[nIdx] + 1);
	int nNeedCount = CTitleManager::Instance()->GetNeedCount(nSmallIdx,nLargeGroup);
	if( nNeedCount == GetHonorTitle(nIdx) )
	{
	( (FLWSUser*)this )->AddHonorListAck();
	g_dpDBClient.SendLogGetHonorTime(this,nIdx);
	}
	}
	*/
#endif	// __WORLDSERVER			
}

void	CMover::CheckHonorStat()
{
#ifdef __WORLDSERVER
	if( m_nHonor < -1 || m_nHonor >= MAX_HONOR_TITLE)
		m_nHonor = -1;

	//////////////////////////////////////////////////////////////////////////
	// initialize
	for( int job_index = MAX_PROFESSIONAL; job_index < MAX_JOB; ++job_index ) {
		const int honor_index	= CTitleManager::Instance()->GetIdx( job_index, HI_EARN_TITLE );
		SetHonorCount( honor_index, 0 );
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// set
	for( int sub_job_index	= m_nJob; sub_job_index >= MAX_PROFESSIONAL && sub_job_index < MAX_JOB;  ) {
		const int honor_index	= CTitleManager::Instance()->GetIdx( sub_job_index, HI_EARN_TITLE );
		SetHonorCount( honor_index, 1 );
		sub_job_index = FLCommonFunction::GetSubLineJob( sub_job_index );
	}
	//////////////////////////////////////////////////////////////////////////

	int nIdx = 0;

	nIdx = CTitleManager::Instance()->GetIdx(HS_STR,HI_COUNT_CHECK);
	SetHonorCount(nIdx,m_Stat.GetOriginStr());

	nIdx = CTitleManager::Instance()->GetIdx(HS_STA,HI_COUNT_CHECK);
	SetHonorCount(nIdx,m_Stat.GetOriginSta());


	nIdx = CTitleManager::Instance()->GetIdx(HS_DEX,HI_COUNT_CHECK);
	SetHonorCount(nIdx,m_Stat.GetOriginDex());

	nIdx = CTitleManager::Instance()->GetIdx(HS_INT,HI_COUNT_CHECK);
	SetHonorCount(nIdx,m_Stat.GetOriginInt());

	//10	HI_COUNT_CHECK	HS_PVP_POINT01	10	IDS_TITLE_TXT_0011	//�ʺ� ������			//	�ʺ� ������
	//11	HI_COUNT_CHECK	HS_PVP_POINT02	100	IDS_TITLE_TXT_0012	//�߽� ������			//	�߽� ������
	//12	HI_COUNT_CHECK	HS_PVP_POINT03	1000	IDS_TITLE_TXT_0013	//����ϴ� ������			//	����ϴ� ������
	//1/3	HI_COUNT_CHECK	HS_PVP_POINT04	4000	IDS_TITLE_TXT_0014	//�б��ִ� ������			//	�б��ִ� ������
	//14	HI_COUNT_CHECK	HS_PVP_POINT05	10000	IDS_TITLE_TXT_0015	//�˷��� ������			//	�˷��� ������
	//15	HI_COUNT_CHECK	HS_PVP_POINT06	20000	IDS_TITLE_TXT_0016	//���� ������			//	���� ������
	//16	HI_COUNT_CHECK	HS_PVP_POINT07	50000	IDS_TITLE_TXT_0017	//������ ������			//	������ ������
	//17	HI_COUNT_CHECK	HS_PVP_POINT08	1000000	IDS_TITLE_TXT_0018	//�Ǹ��� ������			//	�Ǹ��� ������
	//18	HI_COUNT_CHECK	HS_PVP_POINT09	5000000	IDS_TITLE_TXT_0019	//�α⸹�� ������			//	�α⸹�� ������
	//19	HI_COUNT_CHECK	HS_PVP_POINT10	100000000	IDS_TITLE_TXT_0020	//�ְ��� ������			//
	nIdx = CTitleManager::Instance()->GetIdx(HS_PVP_POINT01,HI_COUNT_CHECK);
	SetHonorCount(nIdx,m_nFame);

	nIdx = CTitleManager::Instance()->GetIdx(HS_PVP_POINT02,HI_COUNT_CHECK);
	SetHonorCount(nIdx,m_nFame);

	nIdx = CTitleManager::Instance()->GetIdx(HS_PVP_POINT03,HI_COUNT_CHECK);
	SetHonorCount(nIdx,m_nFame);

	nIdx = CTitleManager::Instance()->GetIdx(HS_PVP_POINT04,HI_COUNT_CHECK);
	SetHonorCount(nIdx,m_nFame);

	nIdx = CTitleManager::Instance()->GetIdx(HS_PVP_POINT05,HI_COUNT_CHECK);
	SetHonorCount(nIdx,m_nFame);

	nIdx = CTitleManager::Instance()->GetIdx(HS_PVP_POINT06,HI_COUNT_CHECK);
	SetHonorCount(nIdx,m_nFame);

	nIdx = CTitleManager::Instance()->GetIdx(HS_PVP_POINT07,HI_COUNT_CHECK);
	SetHonorCount(nIdx,m_nFame);

	nIdx = CTitleManager::Instance()->GetIdx(HS_PVP_POINT08,HI_COUNT_CHECK);
	SetHonorCount(nIdx,m_nFame);

	nIdx = CTitleManager::Instance()->GetIdx(HS_PVP_POINT09,HI_COUNT_CHECK);
	SetHonorCount(nIdx,m_nFame);

	nIdx = CTitleManager::Instance()->GetIdx(HS_PVP_POINT10,HI_COUNT_CHECK);
	SetHonorCount(nIdx,m_nFame);


	nIdx = CTitleManager::Instance()->GetIdx(HS_PK_COUNT,HI_COUNT_CHECK);
	SetHonorCount(nIdx,m_nPKValue);


	nIdx = CTitleManager::Instance()->GetIdx(HS_LORD,HI_EARN_TITLE);
	if( CSLord::Instance()->IsLord( m_idPlayer ) )
		SetHonorCount(nIdx,1);
	else
		SetHonorCount(nIdx,0);

	nIdx = CTitleManager::Instance()->GetIdx( HS_COUPLE_LV, HI_COUNT_CHECK );
	if( CCoupleHelper::Instance()->GetCouple( m_idPlayer ) == NULL ) {
		SetHonorCount( nIdx, 0 );
	}

	if( GetHonorTitle(m_nHonor) < CTitleManager::Instance()->GetNeedCount(m_nHonor,-1))
		m_nHonor = -1;

#endif	// __WORLDSERVER
}

void	CMover::CheckHonorTime()
{
#ifdef __WORLDSERVER
	// m_dwHonorCheckTime
	if( m_pActMover->GetActionState() == OBJSTA_COLLECT )	//ä����
	{
		DWORD dwTick = GetTickCount();
		if( ( m_dwHonorCheckTime + 3600000 ) < dwTick )
		{
			m_dwHonorCheckTime = dwTick;
			((FLWSUser*)this)->SetHonorAdd(HS_COLLECT,HI_ELASPED_TIME);
		}
	}

	if( m_vtInfo.VendorIsVendor() || m_vtInfo.IsVendorOpen() )	//���λ�����
	{
		DWORD dwTick = GetTickCount();
		if( ( m_dwHonorCheckTime + 3600000 ) < dwTick )
		{
			m_dwHonorCheckTime = dwTick;
			((FLWSUser*)this)->SetHonorAdd(HS_TRADE,HI_ELASPED_TIME);
		}
	}
#endif	// __WORLDSERVER
}


//sun: 9,	//__AI_0509
#ifdef __WORLDSERVER
BOOL CMover::IsReturnToBegin( void )
{
	if( m_pAIInterface )
		return m_pAIInterface->IsReturnToBegin();

	if( m_pFSM )
		return m_IsReturnToBegin;

	return FALSE;
}
#endif	// __WORLDSERVER

void CMover::SetSpeedFactor( FLOAT fSpeedFactor )
{
	if( fabs( m_fSpeedFactor - fSpeedFactor ) > 0.000001F )
	{
		m_fSpeedFactor	= fSpeedFactor;
#ifdef __WORLDSERVER
		g_xWSUserManager->AddSetSpeedFactor( this, fSpeedFactor );
#endif	// __WORLDSERVER
	}
}

void	CMover::ProcessCollecting( void )
{
#ifdef __CLIENT
	FLItemElem* pCol		= GetCollector();
	// pCol	= ä����
	if( pCol == NULL )	
	{
		// ��� ����A
		g_WndMng.CloseCollecting();
	}
	else if( m_pActMover->GetActionState() == OBJSTA_COLLECT )
	{
		// ä�� ����
		// SFX
		if( ( m_dwFlag & MVRF_COLLECT ) == 0 )
		{
			PT_ITEM_SPEC pHandProp	= GetActiveHandItemProp();
			if( pHandProp->dwSfxObj2 != NULL_ID )
			{
				D3DXVECTOR3 vSrc, vLocal	= D3DXVECTOR3( 0, 0.5f, 0 );
				( (CModelObject *)m_pModel )->GetForcePos( &vLocal, 0, PARTS_RWEAPON, GetMatrixWorld() );
				vSrc	= vLocal;
				CSfx *pSfx	= CreateSfx( D3DDEVICE, pHandProp->dwSfxObj2, vSrc, GetId(), D3DXVECTOR3( 0, 0, 0 ), NULL_ID, -1 );
				if( pSfx )
				{
					pSfx->SetAngle( -GetAngle() + 90.0f );
					//pSfx->SetAngleX(90.0f);
					m_dwFlag	|= MVRF_COLLECT;
				}
			}
			if(g_WndMng.m_pWndCollecting) g_WndMng.m_pWndCollecting->SetButtonCaption(true);
		}	// SFX
		if(g_WndMng.m_pWndCollecting) g_WndMng.m_pWndCollecting->Update();
		if(m_nCltTime < m_nMaxCltTime) ++m_nCltTime;
		else if(m_nCltTime >= m_nMaxCltTime) m_nCltTime = m_nMaxCltTime;
	}
	else
	{
		// ä�� ��� ����
		g_WndMng.OpenCollecting();
		g_WndMng.m_pWndCollecting->SetButtonCaption(false);
		g_WndMng.m_pWndCollecting->Update();
	}
#endif	// __CLIENT
}

void	CMover::StartCollecting( void )
{
	ClearDest();
	m_dwFlag |= MVRF_NOACTION;
	m_dwMode |= DONMOVE_MODE;
	SendActMsg( OBJMSG_STOP );
	SendActMsg( OBJMSG_COLLECT );
#ifdef __CLIENT
	FLItemElem*			 pCollector = GetCollector();
	CCollectingProperty* pProperty	= CCollectingProperty::GetInstance();
	if( pCollector ) m_nMaxCltTime	= pProperty->GetCool( pCollector->GetAbilityOption() ) + 1;
	m_nCltTime = 0;
#endif	// __CLIENT
}

void	CMover::StopCollecting( void )
{
	m_dwFlag &= (~MVRF_NOACTION);
	m_dwMode &= (~DONMOVE_MODE);
	m_pActMover->ResetState( OBJSTA_ACTION_ALL );
	m_pActMover->SetMoveState( OBJSTA_STAND );
	m_pActMover->RemoveStateFlag( OBJSTAF_ETC );
	SetMotion( MTI_STAND );
}

FLItemElem* CMover::GetCollector( void )
{
	FLItemElem* pCol		= GetWeaponItem();
	if( pCol && ( !pCol->IsCollector() || pCol->IsFlag( FLItemElem::expired ) ) )
		pCol	= NULL;
	return pCol;
}


//sun: 11, �ָӴ�
FLItemElem*	CMover::GetItemId2( int nPocket, DWORD dwItemObjID, BOOL bExpiration )
{
	if( nPocket < 0 )
		return m_Inventory.GetAtId( dwItemObjID );
	return m_Pocket.GetAtId( nPocket, dwItemObjID, bExpiration );
}

BOOL	CMover::CreateItem2( FLItemElem* pItem, int nPocket )
{
#ifdef __WORLDSERVER
	if( nPocket == -1 )
	{
		return CreateItem( pItem );
	}
#endif	// __WORLDSERVER

	return m_Pocket.Add( nPocket, pItem );
}

void	CMover::RemoveItem2( DWORD dwItemObjID, int nNum, int nPocket, BOOL bExpiration )
{
#ifdef __WORLDSERVER
	if( nPocket < 0 )
	{
		//////////////////////////////////////////////////////////////////////////
		//	BEGIN100708
		if( nNum <= 0 )
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "m_idPlayer [%d], ItemNumber [%d]" ), m_idPlayer, nNum );
		}
		else
		{
			RemoveItem( dwItemObjID, nNum );
		}
		//	END100708
		//////////////////////////////////////////////////////////////////////////

	}
	else
#endif	// __WORLDSERVER
		m_Pocket.RemoveAtId( nPocket, dwItemObjID, nNum, bExpiration );
}

BOOL CMover::IsShoutFull( void )
{
	return	( IsSMMode( SM_SHOUT15 ) || IsSMMode( SM_SHOUT30 ) || IsSMMode( SM_SHOUT001 ) );
}

BOOL CMover::IsCommBank( void )
{
	return	( IsSMMode( SM_BANK15 ) || IsSMMode( SM_BANK30 ) || IsSMMode( SM_BANK001 ) );
}

#ifdef __BS_EFFECT_LUA
const char* CMover::GetNameO3D( )
{
	CObject3D* pObj = ((CModelObject*)m_pModel)->GetObject3D( );
	if( !pObj )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "pObj == NULL" ) );
		return NULL;
	}

	return pObj->m_szFileName;
}

//�̸���Ģ : ������ UV Texture animation�� ���� �����̸� ���� �ٴ� �̸���Ģ�̴�.
static const char* STR_EFF_MOVERALPHATEX_TAIL = "-ef01.tga";

BOOL CMover::SetDataMTE( const char* alphaTex, const char* eff2ndTex )
{
	// ��¥ ����Ҷ� ȣ��ȴ�.
	// �̶� ��μ� m_pMteData�� �Ҵ�ȴ�.

	CObject3D* pObj = ((CModelObject*)m_pModel)->GetObject3D( );
	if( !pObj )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "pObj == NULL" ) );
		return FALSE;
	}

	if( NULL == pObj->m_pMteData )
		pObj->m_pMteData = new CObject3D::MTE_DATA;

	CObject3D::MTE_DATA* pData = pObj->m_pMteData;

	char	szAlphaTexture[MAX_PATH];
	GetFileTitle( alphaTex, szAlphaTexture, _countof( szAlphaTexture ) );
	FLStrcat( szAlphaTexture, _countof( szAlphaTexture ), STR_EFF_MOVERALPHATEX_TAIL );

	pData->_pTex[ 0 ] = TexturePool::Get()->GetTexture( DIR_SFXTEX, std::string(szAlphaTexture) );
	pData->_pTex[ 1 ] = TexturePool::Get()->GetTexture( DIR_SFXTEX, std::string(eff2ndTex) );

	if( NULL == pData->_pTex[ 0 ] || NULL == pData->_pTex[ 1 ] )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "Failed loading MTE texture %s or %s" ), szAlphaTexture, eff2ndTex );
		return FALSE;
	}

	//ok... now you can use this data
	return TRUE;
}

#endif //__BS_EFFECT_LUA

//sun: 12, ����
int CMover::GetPerinNum( void )
{
	int nPerin	= 0;
	for( DWORD i = 0; i < m_Inventory.GetMax(); i++ )
	{
		FLItemElem* pItem	= static_cast<FLItemElem*>( GetItemId( i ) );
		if( pItem && pItem->IsPerin() && ::IsUsableItem( pItem ) )
			nPerin	+= pItem->m_nItemNum;
	}
	return nPerin;
}

// �����ϰ��ִ� �丰�� ����� ���� ��ȯ�Ѵ�.
__int64 CMover::GetTotalGold( void )
{
	return static_cast<__int64>( GetPerinNum() ) * PERIN_VALUE + static_cast<__int64>( GetGold() );
}

#ifdef __CLIENT //_SUN_CHECKDATA_
void CMover::SetGold( int nGold )
{
	DWORD dwCheckGold = nGold;
	if( dwCheckGold > MAX_PEYNA_PERSON )
	{
		//		g_DPlay.SendError( CLERR_PENYA_OVERLIMIT_P, nGold );
		g_WndMng.OpenMessageBox( "Over the limited money" );
		FLERROR_LOG( PROGRAM_NAME, _T( "nGold %x" ), dwCheckGold );
		return;
	}

	m_dwGold = dwCheckGold;
}

void CMover::SetAngle( FLOAT fAngle )
{
	//gmpbigsun( 20101124 ) : nBodyState => 1.nodir
	MoverProp* pProp = GetProp( );
	if( pProp )
	{
		if( pProp->nBodyState != NULL_ID )
		{
			if( pProp->nBodyState & 1 )		//������ȯ ����
				return; 
		}
	}

	CObj::SetAngle( fAngle );
}

#endif 

#ifdef __WORLDSERVER
int CMover::RemovePerin( int nPerin )
{
	int nRest	= nPerin;
	for( DWORD i = 0; i < m_Inventory.GetMax() && nRest > 0; i++ )
	{
		FLItemElem* pItem	= static_cast<FLItemElem*>( GetItemId( i ) );
		if( pItem && pItem->IsPerin() && ::IsUsableItem( pItem ) )
		{
			int nRemove		= nRest >= pItem->m_nItemNum? pItem->m_nItemNum: nRest;
			UpdateItem( i, UI_NUM, pItem->m_nItemNum - nRemove );
			nRest	-= nRemove;
		}
	}
	return nPerin - nRest;
}

// iGold��ŭ�� �丰�� ��ĸ� �Ҹ��ϰ� �̸� Ŭ���̾�Ʈ �뺸�Ѵ�.
// ˬ 2008-10-08: ����
int CMover::RemoveTotalGold( __int64 iGold )
{
	FLASSERT( iGold <= GetTotalGold() );
	int nPerin	= (int)( RemovePerin( (int)( iGold / PERIN_VALUE ) ) );
	__int64 iRest	= iGold - ( static_cast<__int64>( nPerin ) * PERIN_VALUE );
	if( iRest > GetGold() )		// ��İ� ���ڸ���
	{
		RemovePerin( 1 );	// 1�� ��ĸ� �߰� �����ϰ�,
		iRest	=  -( PERIN_VALUE - iRest );	// '1�� - ������' ��ĸ� ���ϵ��� ��ȣ�� �ٲ۴�.
	}
	AddGold( static_cast<int>( -iRest ), TRUE );
	return nPerin;
}
#endif	// __WORLDSERVER


//////////////////////////////////////////////////////////////////////////
// mirchang_20100705
bool CMover::CheckUserGold( int nGold, bool bAdd )
{
	__int64 n64Gold = (__int64)nGold;

	if( n64Gold < 0 || n64Gold > INT_MAX )
	{
		return false;
	}

	__int64 n64UserGold = (__int64)m_dwGold;

	if( bAdd == true )
	{
		if( n64UserGold + n64Gold > INT_MAX )
		{
			return false;
		}
	}
	else
	{
		if( nGold < 0 )
		{
			return false;
		}

		if( n64UserGold - n64Gold < 0 )
		{
			return false;
		}
	}

	return true;
}

#ifdef __WORLDSERVER

BOOL CMover::TryDelete_IntoIdleMode()
{
	if( m_bDelete_IntoIdleMode == TRUE )
	{
		m_bDelete_IntoIdleMode	= FALSE;
		if( IsDelete() == FALSE )
			Delete();
		return TRUE;
	}
	return FALSE;
}

void CMover::SetDelete_IntoIdleMode( const BOOL bDelete )
{
	m_bDelete_IntoIdleMode		= bDelete;
}

#endif //__WORLDSERVER
//////////////////////////////////////////////////////////////////////////
#ifdef __CLIENT
CClientPet::CClientPet()
:
m_pObj( NULL ),
m_nLevelup( 0 )
{
	m_szName[0]		= '\0';		//sun: ?, __PET_1024
}

CClientPet::~CClientPet()
{
}

//sun: ?, __PET_1024
void CClientPet::SetName( char * szName )
{
	FLStrncpy( m_szName, _countof( m_szName ), szName, MAX_PET_NAME-1 );
}

#endif	// __CLIENT

bool	CMover::IsAbnormalState( const int nCHS )
{
	if( GetAdjParam( DST_CHRSTATE ) & nCHS )
	{
		return true;
	}

	return false;
}

void CMover::SetDestObj( CCtrl* pObj, float fRange /*= 0.0f */ )
{
	SetDestObj( pObj->GetId(), fRange );
}

void CMover::ClearDestObj()
{
	m_idDest = NULL_ID; m_fArrivalRange = 0.0f;
}

void CMover::ClearDest()
{
	ClearDestObj(); ClearDestPos();
}

int		CMover::GetUsableItemQuantityByItemID( const DWORD dwItemID ) const
{
	int nTotalQuantity	= 0;

	const PT_ITEM_SPEC pItemProp	= g_xSpecManager->GetSpecItem( dwItemID );
	if( pItemProp == NULL )
	{
		return nTotalQuantity;
	}

	DWORD dwStart	= 0;
	DWORD dwEnd		= 0;
	if( m_Inventory.GetObjIndexRange( pItemProp, &dwStart, &dwEnd ) == false )
	{
		return nTotalQuantity;
	}

	for( DWORD Nth = dwStart; Nth < dwEnd; ++Nth )
	{
		FLItemElem* pItemElem = static_cast<FLItemElem*>( m_Inventory.GetAt( Nth ) );
		if( IsUsableItem( pItemElem ) == FALSE || pItemElem->m_dwItemId != dwItemID )
		{
			continue;
		}

		nTotalQuantity += pItemElem->m_nItemNum;
	}

	return nTotalQuantity;
}

DWORD	CMover::CanSellItemToNPC( const DWORD dwItemObjID, const int nSellQuantity )
{
	if( IsPlayer() == FALSE )
	{
		return FSC_SELLITEM_NOT_PLAYER;
	}

	if( IsDie() == TRUE )
	{
		return FSC_SELLITEM_PLAYER_DIE;
	}

	if( !::IsValidObj( m_vtInfo.GetOther() ) || m_vtInfo.GetOther()->IsNPC() == FALSE )
	{
		return FSC_SELLITEM_NPC_NOT_EXIST;
	}

#ifdef __WORLDSERVER
	if( CNpcChecker::GetInstance()->IsCloseNpc( MMI_TRADE, GetWorld(), GetPos() ) == FALSE )
	{
		return FSC_SELLITEM_NPC_NOT_EXIST;
	}
#endif // __WORLDSERVER

	FLItemElem* pItemElem	= static_cast<FLItemElem*>( m_Inventory.GetAtId( dwItemObjID ) );
	if( IsUsableItem( pItemElem ) == FALSE )
	{
		return FSC_SELLITEM_INVALID_ITEM;
	}

	if( nSellQuantity <= 0 || pItemElem->m_nItemNum < nSellQuantity )
	{
		return FSC_SELLITEM_INVALID_QUANTITY;
	}

	if( pItemElem->CanSellNPC() == false )
	{
		return FSC_SELLITEM_DO_NOT_SELL_ITEM;
	}

	if( IsUsing( pItemElem ) == TRUE )
	{
		return FSC_SELLITEM_USING_ITEM;
	}

	if( m_Inventory.IsEquip( dwItemObjID ) == TRUE )
	{
		return FSC_SELLITEM_EQUIP_ITEM;
	}

	int nSellCost = 0;
	if( GetUnitCostSellItemToNPC( nSellCost, dwItemObjID ) == false )
	{
		return FSC_SELLITEM_GOLD_OVERFLOW;
	}

	const __int64 n64BaseCost	= nSellCost * nSellQuantity;
	if( CheckUserGold( static_cast<int>( n64BaseCost ), true ) == false )
	{
		return FSC_SELLITEM_GOLD_OVERFLOW;
	}

	return FSC_SELLITEM_SELL_SUCCESS;
}

bool	CMover::GetUnitCostSellItemToNPC( int & nSellCost, const DWORD dwItemObjID )
{
	if( IsPlayer() == FALSE )
	{
		return false;
	}

	FLItemElem* pItemElem	= m_Inventory.GetAtId( dwItemObjID );
	if( pItemElem == NULL )
	{
		return false;
	}

	const int nBaseCost	= pItemElem->GetCost();
	if( nBaseCost <= 0 )
	{
		return false;
	}

	nSellCost		= nBaseCost / SELL_ITEM_TO_NPC_COST_DIVIDE;

#ifdef __WORLDSERVER
	if( FLGetGlobalSellFactor( nSellCost ) == FALSE )
	{
		return false;
	}
#endif // __WORLDSERVER

	return true;
}

bool	CMover::CheckAttackDistance( CObj* pTarget, const DWORD AttackRangeType )
{
	/************************************************************************/
	/*
	���� �Ÿ� �˻�. this - Attacker, pTarget - Defender
	*/
	/************************************************************************/
#ifdef __WORLDSERVER
	if( IsPlayer() == FALSE )
		return true;
	if( pTarget->GetType() != OT_MOVER || ::IsInvalidObj( pTarget ) )
		return false;

	CMover* pTargetMover = static_cast<CMover*>( pTarget );

	const float fAllowedFrame = 120.0f;	// 2.0 sec
	const float fAllowedRange = pTargetMover->GetSpeed( pTargetMover->m_pActMover->m_fSpeed ) * fAllowedFrame;
	if( IsRangeObj( pTarget, GetAttackRange( AttackRangeType ) + fAllowedRange ) == FALSE )
		return false;
#endif // __WORLDSERVER
	return true;
}

/************************************************************************/
#ifdef __WORLDSERVER
bool	CMover::CanRevival() const
{
	if( IsPlayer() == FALSE ) {
		return false;
	}

	if( IsDie() == FALSE ) {
		return false;
	}

	if( m_Resurrection_Data.bUseing == FALSE && g_pEventArenaGlobal->IsArenaChannel() == true ) {
		return false;
	}

	return true;
}

void	CMover::Revival( const bool bTransfer, const bool bResurrection )
{
	if( CanRevival() == false ) {
		return;
	}

	DecreaseExperience( bTransfer, bResurrection );

	RecoveryRevival();
}

void	CMover::DecreaseExperience( const bool Transfer, const bool Resurrection )
{
	nsRevival::DECREASE_EXP_DATA decreaseExpData;

	if( GetDecreaseExperienceRevival( decreaseExpData ) == false ) {
		return;
	}

	bool useExpProtect	= ( IsSMMode( SM_REVIVAL ) || IsSMMode( SM_REVIVAL2 ) || Resurrection == true ) ? true : false;

	if( IsSMMode( SM_REVIVAL ) == TRUE && Resurrection == false ) {
		SetSMMode( SM_REVIVAL, 0 );
	}

	if( IsSMMode( SM_REVIVAL2 ) == TRUE && Resurrection == false ) {
		SetSMMode( SM_REVIVAL2, 0 );
	}

	if( IsChaotic() == TRUE && useExpProtect == true ) {
		decreaseExpData.decreaseExpRate *= 0.9f;
		useExpProtect	= false;
	}

	if( useExpProtect == true ) {
		return;
	}

	bool downLevel = false;
	if( DoDecreaseExperience( decreaseExpData, downLevel ) == false ) {
		return;
	}

	if( Transfer == true ) {
		if( downLevel == true ) {
			g_xWSUserManager->AddSetLevel( this, static_cast<WORD>( m_nLevel ) );						// this�̿��� �ֺ��� �������������� ����.
			static_cast<FLWSUser*>( this )->AddSetGrowthLearningPoint( m_Stat.GetRemainStatPoint() );	// �ش��������� GP���������� ����.
			g_dpDBClient.SendUpdatePlayerData( static_cast<FLWSUser*>( this ) );						// �÷��̾� ������ ����
			g_pMadrigalGift->OnLevel( static_cast<FLWSUser*>( this ) );
		}

		static_cast<FLWSUser*>( this )->AddSetExperience( GetExp1(), static_cast<WORD>( m_nLevel ),  m_nSkillPoint, m_nSkillLevel, m_nDeathExp, static_cast<WORD>( m_nDeathLevel ) );		// �ش��������� exp1,exp2����� ������ ����.
	}
}

void	CMover::RecoveryRevival()
{
	if( m_Resurrection_Data.bUseing == FALSE ) {
		m_nDead	= PROCESS_COUNT * 5;
	}

	DoRecoveryRevival( GetRateRecoveryRevival() );
	m_pActMover->ClearState();
	m_Resurrection_Data.bUseing = FALSE;
	SetDecreaseExperienceRevival( true );
}

bool	CMover::pushedByAnotherPower( const float angle, const float power )
{
	MoverProp* pMoverProp = prj.GetMoverProp( GetIndex() );
	if( !pMoverProp ) {
		return false;
	}

	if( !IsPlayer() ) {
		if( pMoverProp->fSpeed <= 0.0f ) {
			return false;
		}

// 		if( IsRank( RANK_SUPER ) || IsRank( RANK_MIDBOSS ) ) {
// 			return false;
// 		}
	}

	AngleToVectorXZ( &m_pActMover->m_vDeltaE, angle, power );
	g_xWSUserManager->AddPushPower( this, GetPos(), GetAngle(), angle, power );

	return true;
}
#endif // __WORLDSERVER
/************************************************************************/
#ifdef SCRIPT_ENTER_INSTANCEDUNGEON
bool CMover::EnterInstanceDungeon( DWORD dwTeleWorld, int nX, int nY, int nZ )
{
	bool bRet = FALSE;
#ifdef __WORLDSERVER
	D3DXVECTOR3   vTeleWorld;
	vTeleWorld.x = nX;	
	vTeleWorld.y = nY;
	vTeleWorld.z = nZ;

	

	if( FALSE == IsPlayer() )
		return FALSE;
	if( IsFly() )	// ReplaceInspection() �� �ִ� ���� ������ �����Դ�.
		return FALSE;
	if( CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon( dwTeleWorld ) )
	{
		// �� ���� �δ��� �ƴ϶��
		if( !CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon( static_cast<FLWSUser*>( this )->GetWorld()->GetID() ) ) 	
		{
			DWORD eDungeonLevel		= 0;		//@@@@@@@@@@�̹� �δ��� �����Ǿ��ٴ� �Ǵ�.
			DWORD dwGuildID			= 0;
				
			bRet			= CInstanceDungeonHelper::GetInstance()->EnteranceDungeon( static_cast<FLWSUser*>( this ), dwTeleWorld, dwGuildID, eDungeonLevel );
		}
		else
			REPLACE( g_uIdofMulti, dwTeleWorld, vTeleWorld, REPLACE_NORMAL, static_cast<FLWSUser*>( this )->GetLayer() );
	}
#endif // __WORLDSERVER
	return bRet;
}
#endif	// SCRIPT_ENTER_INSTANCEDUNGEON
