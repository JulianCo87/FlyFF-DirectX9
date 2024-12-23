#include "stdafx.h"

#include "eveschool.h"
#ifdef __WORLDSERVER
#include "guild.h"
#include "../WorldServer/user.h"
extern	CGuildMng	g_GuildMng;


#include "WorldMng.h"
extern CWorldMng g_WorldMng;


#include "playerdata.h"	//sun: 11, ĳ���� ���� ����

CEveSchool::CEveSchool()
{
	m_pSchoolMng	= &g_GuildMng;
	m_dwsbstart2	= GetTickCount();
}

BOOL CEveSchool::Ready( void )
{
// "/sbready
	if( g_eLocal.GetState( EVE_SCHOOL_BATTLE ) != SBS_END )
		return FALSE;

	g_xWSUserManager->ModifyMode( OBSERVE_MODE );	// Observe mode	// all

	for( map<u_long, CGuild*>::iterator i = m_pSchoolMng->m_mapPGuild.begin(); i != m_pSchoolMng->m_mapPGuild.end(); ++i )
	{
		CGuild* pSchool	= i->second;
		pSchool->m_nDead	= 0;
		for( map<u_long, CGuildMember*>::iterator i2 = pSchool->m_mapPMember.begin(); i2 != pSchool->m_mapPMember.end(); ++i2 )
		{
			CGuildMember* pMember	= i2->second;
			FLWSUser* pUser	= g_xWSUserManager->GetUserByPlayerID( pMember->m_idPlayer );
			if( IsValidObj( (CObj*)pUser ) )
			{
				pUser->REPLACE( g_uIdofMulti, WI_WORLD_EVENT01, GetPos( pSchool->m_idGuild ), REPLACE_NORMAL, nDefaultLayer );
				pUser->SetNotMode( OBSERVE_MODE );	// Not observe mode
				g_xWSUserManager->AddModifyMode( pUser );
			}
		}
	}

	if( g_eLocal.SetState( EVE_SCHOOL_BATTLE, SBS_READY ) )
		g_xWSUserManager->AddSetLocalEvent( EVE_SCHOOL_BATTLE, SBS_READY );

	return TRUE;
}

BOOL CEveSchool::Start( void )
{
// "/sbstart"
	if( g_eLocal.GetState( EVE_SCHOOL_BATTLE ) != SBS_READY )
		return FALSE;

	if( g_eLocal.SetState( EVE_SCHOOL_BATTLE, SBS_START ) )
		g_xWSUserManager->AddSetLocalEvent( EVE_SCHOOL_BATTLE, SBS_START );
	return TRUE;
}

BOOL CEveSchool::Start2( void )
{
// "/sbstart2"
	if( g_eLocal.GetState( EVE_SCHOOL_BATTLE ) != SBS_START )
		return FALSE;

	if( g_eLocal.SetState( EVE_SCHOOL_BATTLE, SBS_START2 ) )
		g_xWSUserManager->AddSetLocalEvent( EVE_SCHOOL_BATTLE, SBS_START2 );

	m_dwsbstart2	= GetTickCount();
	return TRUE;
}

BOOL CEveSchool::Report( void )
{
//	"/sbreport"
	if( g_eLocal.GetState( EVE_SCHOOL_BATTLE ) == SBS_END )
		return FALSE;

	SCHOOL_ENTRY	school[MAX_SCHOOL];

	int iIndex	= 0;
	for( map<u_long, CGuild*>::iterator i = m_pSchoolMng->m_mapPGuild.begin();
		i != m_pSchoolMng->m_mapPGuild.end(); ++i )
	{
		CGuild* pSchool	= i->second;
		
		school[iIndex].id	= pSchool->m_idGuild;
		FLStrcpy( school[iIndex].lpName, _countof( school[iIndex].lpName ), pSchool->m_szGuild );
		school[iIndex].nDead	= pSchool->m_nDead;

		for( map<u_long, CGuildMember*>::iterator i2 = pSchool->m_mapPMember.begin();
		i2 != pSchool->m_mapPMember.end(); ++i2 )
		{
			CGuildMember* pMember	= i2->second;
			FLWSUser* pUser	= g_xWSUserManager->GetUserByPlayerID( pMember->m_idPlayer );
			if( IsValidObj( (CObj*)pUser ) )
			{
				school[iIndex].nSize++;
				if( pUser->IsRegionAttr( RA_FIGHT ) )
				{
					if( !pUser->IsMode( OBSERVE_MODE ) )
					{
						school[iIndex].nSurvivor++;
						school[iIndex].nLevel	+= pUser->GetLevel();
					}
				}
			}
		}
		iIndex++;
	}
	// sort
	for( int j = 0; j < MAX_SCHOOL-1; j++ )
	{
		for( int k = j + 1; k < MAX_SCHOOL; k++ )
		{
			int nAvg1, nAvg2;
			if( school[j].nSurvivor == 0 )
				nAvg1	= 0;
			else
				nAvg1	= school[j].nLevel / school[j].nSurvivor;
			if( school[k].nSurvivor == 0 )
				nAvg2	= 0;
			else
				nAvg2	= school[k].nLevel / school[k].nSurvivor;

			if( ( school[j].nSurvivor < school[k].nSurvivor ) ||
				( school[j].nSurvivor == school[k].nSurvivor && nAvg1 < nAvg2 ) )
			{	// swap
				SCHOOL_ENTRY tmp;
				memcpy( &tmp, &school[j], sizeof(SCHOOL_ENTRY) );
				memcpy( &school[j], &school[k], sizeof(SCHOOL_ENTRY) );
				memcpy( &school[k], &tmp, sizeof( SCHOOL_ENTRY) );
			}
		}
	}

	short nElapse	= (short)( (int)( GetTickCount() - m_dwsbstart2 ) / (int)MIN( 1 ) );
	g_xWSUserManager->AddSchoolReport( school, nElapse );	// neuz

	return TRUE;
}

BOOL CEveSchool::End( void )
{
// "/sbend"
	if( g_eLocal.GetState( EVE_SCHOOL_BATTLE ) != SBS_START2 )	// '/sbstart2'
		return FALSE;

	SCHOOL_ENTRY	school[MAX_SCHOOL];

	int iIndex	= 0;
	for( map<u_long, CGuild*>::iterator i = m_pSchoolMng->m_mapPGuild.begin();
		i != m_pSchoolMng->m_mapPGuild.end(); ++i )
	{
		CGuild* pSchool	= i->second;
		
		school[iIndex].id	= pSchool->m_idGuild;
		FLStrcpy( school[iIndex].lpName, _countof( school[iIndex].lpName ), pSchool->m_szGuild );
		school[iIndex].nDead	= pSchool->m_nDead;

		for( map<u_long, CGuildMember*>::iterator i2 = pSchool->m_mapPMember.begin();
		i2 != pSchool->m_mapPMember.end(); ++i2 )
		{
			CGuildMember* pMember	= i2->second;
			FLWSUser* pUser	= g_xWSUserManager->GetUserByPlayerID( pMember->m_idPlayer );
			if( IsValidObj( (CObj*)pUser ) )
			{
				if( pUser->IsRegionAttr( RA_FIGHT ) )
				{
					if( !pUser->IsMode( OBSERVE_MODE ) )
					{
						school[iIndex].nSurvivor++;
						school[iIndex].nLevel	+= pUser->GetLevel();
					}
				}
				else
				{
					pUser->SetMode( OBSERVE_MODE );		// MAKE observe mode
					g_xWSUserManager->AddModifyMode( pUser );
				}
			}
		}
		iIndex++;
	}

	// sort
	for( int j = 0; j < MAX_SCHOOL-1; j++ )
	{
		for( int k = j + 1; k < MAX_SCHOOL; k++ )
		{
			int nAvg1, nAvg2;
			if( school[j].nSurvivor == 0 )
				nAvg1	= 0;
			else
				nAvg1	= school[j].nLevel / school[j].nSurvivor;
			if( school[k].nSurvivor == 0 )
				nAvg2	= 0;
			else
				nAvg2	= school[k].nLevel / school[k].nSurvivor;

			if( ( school[j].nSurvivor < school[k].nSurvivor ) ||
				( school[j].nSurvivor == school[k].nSurvivor && nAvg1 < nAvg2 ) )
			{	// swap
				SCHOOL_ENTRY tmp;
				memcpy( &tmp, &school[j], sizeof(SCHOOL_ENTRY) );
				memcpy( &school[j], &school[k], sizeof(SCHOOL_ENTRY) );
				memcpy( &school[k], &tmp, sizeof( SCHOOL_ENTRY) );
			}
		}
	}

	if( g_eLocal.SetState( EVE_SCHOOL_BATTLE, SBS_END ) )
		g_xWSUserManager->AddSetLocalEvent( EVE_SCHOOL_BATTLE, SBS_END );

	g_dpDBClient.SendSchoolReport( school );	// log
	g_xWSUserManager->AddSchoolReport( school );	// neuz	

	return TRUE;
}

BOOL CEveSchool::LoadPos( LPCSTR lpszFileName )
{
	CScanner s;

	if( !s.Load( lpszFileName ) )
		return FALSE;
	
	u_long id;
	D3DXVECTOR3 vPos;

	id	= s.GetNumber();
	while( s.tok != FINISHED )
	{
		vPos.x	= s.GetFloat();
		vPos.y	= s.GetFloat();
		vPos.z	= s.GetFloat();

		if( id > MAX_SCHOOL || id < 1 )
			m_vDefault	= vPos;
		else
			m_avPos[id-1]	= vPos;
		id	= s.GetNumber();
	}
	return TRUE;
}

D3DXVECTOR3 CEveSchool::GetPos( u_long id )
{
	if( id > MAX_SCHOOL || id < 1 )
//		return D3DXVECTOR3( 0, 0, 0 );
		return m_vDefault;
	return m_avPos[id-1];
}

CEveSchool* CEveSchool::GetInstance()
{
	static	CEveSchool	sEveSchool;
	return & sEveSchool;
}
#endif // __WORLDSERVER

CGuildCombat::CGuildCombat()
{
	m_nState = CLOSE_STATE;
	m_nGCState = WAR_CLOSE_STATE;
	m_nGuildCombatIndex = 0;
	m_uWinGuildId = 0;
	m_nWinGuildCount = 0;
	m_uBestPlayer = 0;
	m_vecGCGetPoint.clear();
	m_vecGCPlayerPoint.clear();

#ifdef __WORLDSERVER
	vecGCSendItem.clear();			//sun: 8, // __GUILDCOMBAT_85
	m_vecGuildCombatMem.clear();	//sun: 10, __S_BUG_GC

	vecRequestRanking.clear();
	m_dwTime = 0;
	m_nStopWar = 0;
	m_nJoinPanya = 0;
	m_nGuildLevel = 0;
	m_nMaxJoinMember = 0;
	m_nMaxPlayerLife = 0;
	m_nMaxWarPlayer = 0;
	m_nMaxPlayerPercent = 0;
	m_nMaxGuildPercent = 0;
	m_nRequestCanclePercent = 0;
	m_nNotRequestPercent = 0;
	m_nItemPenya = 0;
	m_bMutex     = FALSE;
	m_bMutexMsg  = FALSE;

	memset( __AutoOpen, 0, sizeof(__AutoOpen) );	

	m_nDay	= 0;


	m_dwRewardItemID	= NULL_ID;
#endif // __WORLDSERVER
#ifdef __CLIENT
	m_bRequest = FALSE;
#endif // __CLIENT
}

CGuildCombat::~CGuildCombat()
{
	m_vecGCGetPoint.clear();
	m_vecGCPlayerPoint.clear();
#ifdef __WORLDSERVER
	vecGCSendItem.clear();
	for( vector<__GuildCombatMember*>::iterator kItr = m_vecGuildCombatMem.begin(); kItr != m_vecGuildCombatMem.end(); ++kItr )
	{
		__GuildCombatMember* pGCMember = *kItr;
		for( vector<__JOINPLAYER*>::iterator kItr2 = pGCMember->vecGCSelectMember.begin(); kItr2 != pGCMember->vecGCSelectMember.end(); ++kItr2 )
		{
			SAFE_DELETE( *kItr2 );
		}
		pGCMember->vecGCSelectMember.clear();
		SAFE_DELETE( *kItr );
	}
	m_vecGuildCombatMem.clear();
	vecRequestRanking.clear();
#endif // __WORLDSERVER
}

void CGuildCombat::GuildCombatClear( int nState )
{
	m_nState = CLOSE_STATE;
	m_nGCState = WAR_CLOSE_STATE;

	if( nState == 99 || nState == 1 )
	{
		m_nGuildCombatIndex = 0;
		m_uWinGuildId = 0;
		m_nWinGuildCount = 0;
		m_uBestPlayer = 0;
	}	

	m_vecGCGetPoint.clear();
#ifdef __WORLDSERVER

//sun: 10, __S_BUG_GC
	for( int gcmi = 0 ; gcmi < (int)( m_vecGuildCombatMem.size() ) ; ++gcmi )
		SAFE_DELETE( m_vecGuildCombatMem.at( gcmi ) );
	m_vecGuildCombatMem.clear();

	vecRequestRanking.clear();
	m_dwTime = 0;
	m_nStopWar = 0;
	if( g_eLocal.GetState( EVE_GUILDCOMBAT ) )
		g_DPCoreClient.SendGuildCombatState( CLOSE_STATE );		

	if( nState == 99 || nState == 3 )
	{
		m_nJoinPanya = 0;
		m_nGuildLevel = 0;
		m_nMaxJoinMember = 0;
		m_nMaxPlayerLife = 0;
		m_nMaxWarPlayer = 0;
		m_nMaxPlayerPercent = 0;
		m_nMaxGuildPercent = 0;
		m_nRequestCanclePercent = 0;
		m_nNotRequestPercent = 0;
		m_nItemPenya = 0;
	}
#endif // __WORLDSERVER
#ifdef __CLIENT
	m_bRequest = FALSE;
#endif // __CLIENT
}

void CGuildCombat::GuildCombatGameClear()
{
	GuildCombatClear( 2 );
}

void CGuildCombat::AddvecGCGetPoint( u_long uidGuildAttack, u_long uidGuildDefence, u_long uidPlayerAttack, u_long uidPlayerDefence, int nPoint,
									BOOL bKillDiffernceGuild, BOOL bMaster, BOOL bDefender, BOOL bLastLife )
{
	__GCGETPOINT GCGetPoint;
	GCGetPoint.uidGuildAttack = uidGuildAttack;
	GCGetPoint.uidGuildDefence = uidGuildDefence;
	GCGetPoint.uidPlayerAttack = uidPlayerAttack;
	GCGetPoint.uidPlayerDefence = uidPlayerDefence;
	GCGetPoint.nPoint = nPoint;
	GCGetPoint.bKillDiffernceGuild = bKillDiffernceGuild;
	GCGetPoint.bMaster = bMaster;
	GCGetPoint.bDefender = bDefender;
	GCGetPoint.bLastLife = bLastLife;
	m_vecGCGetPoint.push_back( GCGetPoint );
#ifdef __WORLDSERVER
	g_xWSUserManager->AddGCLogRealTimeWorld( GCGetPoint );
#endif // __WORLDSERVER
}

void CGuildCombat::AddvecGCPlayerPoint( u_long uidPlayer, int nJob, int nPoint )
{
	__GCPLAYERPOINT GCPlayerPoint;
	GCPlayerPoint.uidPlayer = uidPlayer;
	GCPlayerPoint.nJob = nJob;
	GCPlayerPoint.nPoint = nPoint;
	m_vecGCPlayerPoint.push_back( GCPlayerPoint );
}

#ifdef __WORLDSERVER
void CGuildCombat::SelectPlayerClear( u_long uidGuild )
{
//sun: 10, __S_BUG_GC
	__GuildCombatMember* pGCMember = FindGuildCombatMember( uidGuild );
	if( pGCMember != NULL )
		pGCMember->SelectMemberClear();
}

// ���� ��û
void CGuildCombat::JoinGuildCombat( u_long idGuild, DWORD dwPenya, BOOL bRequest )
{
//sun: 10, __S_BUG_GC
	__GuildCombatMember* pGCMember = FindGuildCombatMember( idGuild );
	if( pGCMember != NULL )
	{
		pGCMember->dwPenya = dwPenya;		
		pGCMember->bRequest = bRequest;
	}
	else
	{
		pGCMember	= new __GuildCombatMember;
		pGCMember->Clear();
		pGCMember->uGuildId = idGuild;
		pGCMember->dwPenya = dwPenya;
		pGCMember->bRequest = bRequest;
		m_vecGuildCombatMem.push_back( pGCMember );
	}

	SetRequestRanking();
}
// ���� Ż��
void CGuildCombat::OutGuildCombat( u_long idGuild )
{
	if( !g_eLocal.GetState( EVE_GUILDCOMBAT ) )
		return; 
	
//sun: 10, __S_BUG_GC
	__GuildCombatMember* pGCMember = FindGuildCombatMember( idGuild );
	if( pGCMember != NULL ) // ��ҽ� �ٷ� ���������� ����
	{
		CGuild* pGuild = g_GuildMng.GetGuild( idGuild );
		if( pGuild && pGCMember->bRequest && pGCMember->dwPenya > 0 )
		{
			FLItemElem itemElem;
			char szMsg[1000] = { 0, };
			FLSPrintf( szMsg, _countof( szMsg ), "%s", prj.GetText( TID_GAME_GC_CANCELREQUEST ) );
			char szMsg1[1000] = { 0, };
			FLSPrintf( szMsg1, _countof( szMsg1 ), "%s", prj.GetText( TID_GAME_GC_CANCELREQUEST1 ) );
			g_dpDBClient.SendQueryPostMail( pGuild->m_idMaster, 0, itemElem, MulDiv( pGCMember->dwPenya, m_nRequestCanclePercent, 100 ), szMsg, szMsg1 );

			pGCMember->bRequest = FALSE;
			pGCMember->dwPenya = 0;
		}

		DeleteGuildCombatTender( idGuild );
	}

	SetRequestRanking();
}

// ������ ��尡 ������ ������ ������ ����
void CGuildCombat::AddSelectPlayer( u_long idGuild, u_long uidPlayer )
{
//sun: 10, __S_BUG_GC
	__GuildCombatMember* pGCMember = FindGuildCombatMember( idGuild );
	if( pGCMember != NULL )
	{
		__JOINPLAYER* pJoinPlayer = new __JOINPLAYER;
		pJoinPlayer->nlife = m_nMaxPlayerLife;	
		pJoinPlayer->uidPlayer = uidPlayer;
		pJoinPlayer->nMap = 99;
		pJoinPlayer->nPoint = 0;
		pJoinPlayer->uKillidGuild = 0;
		pJoinPlayer->dwTelTime = 0;
		pGCMember->vecGCSelectMember.push_back( pJoinPlayer );
	}
}
void CGuildCombat::GetSelectPlayer( u_long idGuild, vector<__JOINPLAYER> &vecSelectPlayer )
{
//sun: 10, __S_BUG_GC
	__GuildCombatMember* pGCMember = FindGuildCombatMember( idGuild );
	if( pGCMember != NULL )
	{
		for( int veci = 0 ; veci < (int)( pGCMember->vecGCSelectMember.size() ) ; ++veci )
		{
			__JOINPLAYER* pJoinPlayer =  pGCMember->vecGCSelectMember.at( veci );
			vecSelectPlayer.push_back( *pJoinPlayer );
		}
	}	
}

void CGuildCombat::JoinWar( FLWSUser* pUser, int nMap , BOOL bWar)
{
	const DWORD maxPos	= 4;
	D3DXVECTOR3 v3Pos[maxPos];
	v3Pos[0] = D3DXVECTOR3(1263, 83, 1303);
	v3Pos[1] = D3DXVECTOR3(1310, 83, 1303);
	v3Pos[2] = D3DXVECTOR3(1265, 83, 1254);
	v3Pos[3] = D3DXVECTOR3(1314, 83, 1253);

// 	int nPos = xRandom(4);
// 	if( nMap == 99 )
// 		nMap = nPos;

	if( nMap < 0 || nMap >= maxPos ) {
		nMap = xRandom( maxPos );
	}

	if( m_nState == WAR_STATE )
	{
		if( m_nGCState == WAR_WAR_STATE )
			pUser->m_nGuildCombatState = 1;
		else if( m_nGCState == MAINTENANCE_STATE )
			pUser->m_nGuildCombatState = 2;

		if( m_nGCState == WAR_WAR_STATE || m_nGCState == MAINTENANCE_STATE )
			g_xWSUserManager->AddGuildCombatUserState( pUser );
	}
	
	((CMover*)pUser)->REPLACE( g_uIdofMulti, WI_WORLD_GUILDWAR, v3Pos[nMap], REPLACE_NORMAL, nDefaultLayer );
	pUser->m_vtInfo.SetOther( NULL );

	if( bWar == FALSE ) {
		pUser->AddText( prj.GetText(TID_GAME_GUILDCOMBAT_WELCOME) );
		pUser->AddText( prj.GetText(TID_GAME_GUILDCOMBAT_MASTER_MAP) );
	}
}

void CGuildCombat::OutWar( FLWSUser* pUser, FLWSUser* pLeader, BOOL bLogOut )
{
//sun: 10, __S_BUG_GC
	__GuildCombatMember* pGCMember = FindGuildCombatMember( pUser->m_idGuild );
	if( pGCMember != NULL )
	{
		// ���� �ٸ� ������ ã��
		SetPlayerChange( pUser, pLeader );
		// ������� �α׾ƿ����̸� �ٸ� �����ڰ� �ڵ����� ������� ��

		if( bLogOut )
		{
			if( pUser->m_idPlayer == pGCMember->m_uidDefender )	// ����� �˻�
			{
				CGuild* pGuild = g_GuildMng.GetGuild( pUser->m_idGuild  );
				if( pGuild )
				{
					vector<int> vecDefender;
					vecDefender.clear();
					for( int vecii0 = 0 ; vecii0 < (int)( pGCMember->vecGCSelectMember.size() ) ; ++vecii0 )
					{
						__JOINPLAYER* pJoinPlayer3 = pGCMember->vecGCSelectMember.at( vecii0 );
						CMover* pMover = prj.GetUserByID( pJoinPlayer3->uidPlayer );
						if( IsValidObj( pMover ) )
						{						
							if( pGuild->IsMaster( pMover->m_idPlayer ) == FALSE && pGCMember->m_uidDefender != pMover->m_idPlayer && 0 < pJoinPlayer3->nlife )
							{
								vecDefender.push_back( vecii0 );
							}
						}
					}
					if( 0 < vecDefender.size() )
					{
						int nPlayerPos = xRandom( vecDefender.size() );
						int nDefender = vecDefender[nPlayerPos];
						__JOINPLAYER* pJoinPlayer4 = pGCMember->vecGCSelectMember.at( nDefender );
						pGCMember->m_uidDefender = pJoinPlayer4->uidPlayer;
					}
				}
			}	// if( pUser->m_idPlayer == pGCMember->m_uidDefender )	// ����� �˻�
		}	// if( bLogOut )
	}

	g_xWSUserManager->AddGCGuildStatus( pUser->m_idGuild );
	g_xWSUserManager->AddGCWarPlayerlist();

	// �߰��� ���� �ߴ��ص� �Ǵ��� �˻�
	UserOutGuildCombatResult( pUser );
}
void CGuildCombat::JoinObserver( FLWSUser* pUser )
{
	if( pUser->IsBaseJob() )
	{
		pUser->AddDiagText( prj.GetText(TID_GAME_GUILDCOMBAT_WAR_ENTER) );
		return;
	}
	
	// ���� ���ֱ�
	pUser->RemoveCommonBuffs();
	g_xWSUserManager->AddRemoveAllSkillInfluence( pUser );
	((CMover*)pUser)->REPLACE( g_uIdofMulti, WI_WORLD_GUILDWAR, D3DXVECTOR3( 1361.6f, 0.0f, 1273.3f ), REPLACE_NORMAL, nDefaultLayer );
	pUser->m_vtInfo.SetOther( NULL );
	// �������� ���Ű� ȯ���մϴ�.
	pUser->AddText( prj.GetText(TID_GAME_GUILDCOMBAT_WELCOME) );
	// ������ ���� �����ϼ̽��ϴ�.
	pUser->AddText( prj.GetText(TID_GAME_GUILDCOMBAT_ZOOM_USE) );

	if( m_nGCState <= ENTER_STATE )
		pUser->AddGCWarPlayerlist();

	if( IsSelectPlayer( pUser ) )
	{
		pUser->m_nGuildCombatState		= 2;
		g_xWSUserManager->AddGuildCombatUserState( (CMover*)pUser );

		if( ENTER_STATE <= m_nGCState && m_nGCState <= WAR_CLOSE_STATE )
			g_DPCoreClient.SendGCRemoveParty( pUser->GetPartyId(), pUser->m_idPlayer );
	}
	g_xWSUserManager->AddGCGuildStatus( pUser->m_idGuild, pUser );
	g_xWSUserManager->AddGCGuildPrecedence( pUser );
	g_xWSUserManager->AddGCPlayerPrecedence( pUser );	
}

void CGuildCombat::GuildCombatRequest( FLWSUser* pUser, DWORD dwPenya )
{
	CGuild* pGuild	= g_GuildMng.GetGuild( pUser->m_idGuild );
	if( pGuild && pGuild->IsMaster( pUser->m_idPlayer ) )
	{
		// �ð� �˻� ( �� ~ �� )
		//�������߿��� ��û�� �� �����ϴ�.
		if( m_nState != CGuildCombat::CLOSE_STATE )
		{
			pUser->AddDiagText( prj.GetText(TID_GAME_GUILDCOMBAT_REQUESTEND) );
			return;
		}

		// ��巹�� m_nGuildLevel�� �� �̻����� �˻�
		if( pGuild->m_nLevel < m_nGuildLevel )
		{
			CString strMsg;
			strMsg.Format( prj.GetText( TID_GAME_GUILDCOMBAT_REQUEST_LEVEL ), m_nGuildLevel );
			pUser->AddDiagText( strMsg );
			return;
		}
		
		int nWinnerJoinPenya = m_nJoinPanya;

		if( m_uWinGuildId == pUser->m_idGuild && m_nWinGuildCount >= 1 )
		{
			nWinnerJoinPenya = m_nJoinPanya * (m_nWinGuildCount + 1);
		}

		if( nWinnerJoinPenya > dwPenya )
		{
			return;
		}
		// ������ ��û �� ���
		DWORD dwExistingPenya = 0;
//sun: 10, __S_BUG_GC
		__GuildCombatMember* pGCMember = FindGuildCombatMember( pUser->m_idGuild );
		if( pGCMember != NULL )
			dwExistingPenya = pGCMember->dwPenya;

		// INT_MAX���� ���� �ʾƾ� �Ѵ�
		if( dwPenya > INT_MAX )
			return;

		// �⺻ ���(m_nJoinPenya) ���� �� ���� ������ �ִ��� Ȯ��
		if( (int)( dwPenya ) < m_nJoinPanya )
		{
			CString strMsg;
			strMsg.Format( prj.GetText( TID_GAME_GUILDCOMBAT_REQUEST_BASEPENYA ), m_nJoinPanya );
			pUser->AddDiagText( strMsg );
			pUser->AddText( prj.GetText(TID_GAME_LACKMONEY) );	
			return;
		}
		
		// ���� ��û�� ��� ���� �� ���ƾ� �Ѵ�.
		if( dwPenya <= dwExistingPenya )
		{
			CString strMsg;
			strMsg.Format( prj.GetText( TID_GAME_GUILDCOMBAT_REQUEST_PENYA ), dwPenya );
			pUser->AddDiagText( strMsg );
			return;
		}


		// ��û�� ��İ� ������ �ִ��� Ȯ��
//		if( pUser->GetGold() < (int)( (dwPenya-dwExistingPenya) ) )
		if( pUser->CheckUserGold( (int)( (dwPenya-dwExistingPenya) ), false ) == false )
		{
			pUser->AddDiagText( prj.GetText( TID_GAME_GUILDCOMBAT_REQUEST_NOTPENYA ) );
			return;
		}

		LogItemInfo aLogItem;
		//aLogItem.Action = "9";
		//aLogItem.SendName = pUser->GetName();
		//aLogItem.RecvName = "GUILDCOMBAT";
		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "9" );
		FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUser->GetName() );
		FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "GUILDCOMBAT" );
		aLogItem.WorldId = pUser->GetWorld()->GetID();
		aLogItem.Gold = pUser->GetGold();
		aLogItem.Gold2 = pUser->GetGold() -(dwPenya-dwExistingPenya);
		//aLogItem.ItemName = "SEED";
		FLSPrintf( aLogItem.kLogItem.szItemName, _countof( aLogItem.kLogItem.szItemName ), "%d", ITEM_INDEX( 12, II_GOLD_SEED1 ) );
		aLogItem.kLogItem.nQuantity = dwPenya-dwExistingPenya;
		g_DPSrvr.OnLogItem( aLogItem );

		pUser->AddGold( (int)( (-1) * (int)( dwPenya-dwExistingPenya ) ) );
		g_dpDBClient.SendInGuildCombat( pUser->m_idGuild, dwPenya, dwExistingPenya );
	}
	else
	{
		//��尡 ���ų� ������� �ƴմϴ�.
		pUser->AddText( prj.GetText(TID_GAME_GUILDCOMBAT_NOT_GUILD_LEADER) );
	}
}

void CGuildCombat::GuildCombatCancel( FLWSUser* pUser )
{
	CGuild* pGuild	= g_GuildMng.GetGuild( pUser->m_idGuild );
	if( pGuild && pGuild->IsMaster( pUser->m_idPlayer ) )
	{
		// �ð� �˻� ( �� ~ �� )
//sun: 10, __S_BUG_GC
		__GuildCombatMember* pGCMember = FindGuildCombatMember( pUser->m_idGuild );
		if( pGCMember != NULL )
		{
			if( pGCMember->bRequest )
			{
				if( m_nState != CGuildCombat::CLOSE_STATE )
				{
					pUser->AddText( prj.GetText(TID_GAME_GUILDCOMBAT_NOT_USE));	// ������ �������߿��� ������ Ż�� �Ҽ� �����ϴ�
					return;
				}
				g_dpDBClient.SendOutGuildCombat( pUser->m_idGuild );
			}
			else
			{
				pUser->AddText( prj.GetText(TID_GAME_GUILDCOMBAT_NOTAPP) );
			}
		}
		else
		{
			//������ ��û�� ���� �ʾҽ��ϴ�.
			pUser->AddText( prj.GetText(TID_GAME_GUILDCOMBAT_NOTAPP) );
		}
	}
	else
	{
		//��尡 ���ų� ������� �ƴմϴ�.
		pUser->AddText( prj.GetText(TID_GAME_GUILDCOMBAT_NOT_GUILD_LEADER) );
	}
}
// �߰��� ĳ���Ͱ� �װų� �α׾ƿ��ÿ� �¸���尡 ������ ���� ����
void CGuildCombat::UserOutGuildCombatResult( FLWSUser* pUser )
{
	int nCount = 0;
//sun: 10, __S_BUG_GC
	__GuildCombatMember* pGCMember = FindGuildCombatMember( pUser->m_idGuild );
	if( pGCMember != NULL )
	{
		for( int veci = 0 ; veci < (int)( pGCMember->vecGCSelectMember.size() ) ; ++veci )
		{
			__JOINPLAYER* pJoinPlayer = pGCMember->vecGCSelectMember.at( veci );
			// ������ �ִ� ĳ���� ������ �˻� ���ص� ��.
			if( 0 < pJoinPlayer->nlife )
				return;
		}

		// �ٸ������ ���� �ľ�( ����� ������ 1���̻����� �˻� 1���� ��常 ������ ���� ���� )
		for( int nVeci = 0 ; nVeci < (int)( vecRequestRanking.size() ) ; ++nVeci )
		{
			if( nVeci >= m_nMaxGuild )
				break;
			
			__REQUESTGUILD RequestGuild = vecRequestRanking.at( nVeci );
			__GuildCombatMember *pGCMember2 = FindGuildCombatMember( RequestGuild.uidGuild );
			if( pGCMember2 != NULL )
			{
				if( pUser->m_idGuild != pGCMember2->uGuildId )
				{
					BOOL bLive = FALSE;
					for( int veci2 = 0 ; veci2 < (int)( pGCMember2->vecGCSelectMember.size() ) ; ++veci2 )
					{
						__JOINPLAYER* pJoinPlayer = pGCMember2->vecGCSelectMember.at( veci2 );
						if( 0 < pJoinPlayer->nlife )
							bLive = TRUE;
					}
					if( bLive )
					{
						++nCount;
						if( nCount == 2 )
						{
							return; // 2�� �̻��̹Ƿ� ��� ����
						}
					}
				}
			}
		}
		// ��尡 �ϳ��ۿ� �����Ƿ� ���� ����
		if( nCount == 1 )
			m_nStopWar = 1;
	}
}

void CGuildCombat::GuildCombatResult( BOOL /*nResult*/, u_long /*idGuildWin*/ )
{
	vector<u_long> vecSameidGuildWin; 
	vecSameidGuildWin.clear();
	int nMaxPoint = -1;

	// �ְ� ������ ������ ��( �ְ� ������ �������� ������ ������ vecSameidGuildWin�� ���� )
	for( int nVeci = 0 ; nVeci < (int)( vecRequestRanking.size() ) ; ++nVeci )
	{
		if( nVeci >= m_nMaxGuild )
			break;
		
		__REQUESTGUILD RequestGuild = vecRequestRanking.at( nVeci );
//sun: 10, __S_BUG_GC
		__GuildCombatMember* pGCMember = FindGuildCombatMember( RequestGuild.uidGuild );
		if( pGCMember != NULL )
		{
			int nPoint = 0;
			for( int veci = 0 ; veci < (int)( pGCMember->vecGCSelectMember.size() ) ; ++veci )
			{
				__JOINPLAYER* pJoinPlayer = pGCMember->vecGCSelectMember.at( veci );
				nPoint += pJoinPlayer->nPoint + pJoinPlayer->nlife;
			}
			pGCMember->nGuildPoint = nPoint;
			// ���� �ٲ�
			if( nMaxPoint < nPoint )
			{
				nMaxPoint = nPoint;
				vecSameidGuildWin.clear();
				vecSameidGuildWin.push_back( pGCMember->uGuildId );
			}
			else if( nMaxPoint == nPoint )
			{
				vecSameidGuildWin.push_back( pGCMember->uGuildId );
			}
		}
	}

	// ��������� ������ ��Ȱ��ȸ�� ���� ���� ���� ū ��� �¸�
	int nMaxLife = 0;
	if( 1 < vecSameidGuildWin.size() )
	{
		vector<u_long> vecSameidGuildWinTmp;
		vecSameidGuildWinTmp.clear();
		for( int vecSame = 0 ; vecSame < (int)( vecSameidGuildWin.size() ) ; ++vecSame )
		{
			int nLife = 0;
//sun: 10, __S_BUG_GC
			__GuildCombatMember* pGCMember = FindGuildCombatMember( vecSameidGuildWin[vecSame] );
			if( pGCMember != NULL )
			{
				for( int veci2 = 0 ; veci2 < (int)( pGCMember->vecGCSelectMember.size() ) ; ++veci2 )
				{
					__JOINPLAYER* pJoinPlayer = pGCMember->vecGCSelectMember.at( veci2 );
					nLife += pJoinPlayer->nlife;
				}
				
				// ���� �ٲ�
				if( nMaxLife < nLife )
				{
					nMaxLife = nLife;
					vecSameidGuildWinTmp.clear();
					vecSameidGuildWinTmp.push_back( pGCMember->uGuildId );
				}
				else if( nMaxLife == nLife )
				{
					vecSameidGuildWinTmp.push_back( pGCMember->uGuildId );
				}
			}
		}
		vecSameidGuildWin = vecSameidGuildWinTmp;
	}

	// ��������� ������ ������ �ִ� ������ ��շ����� ū ��� �¸�
	float fMaxAvgLv = 0.0f;
	if( 1 < vecSameidGuildWin.size() )
	{
		vector<u_long> vecSameidGuildWinTmp;
		vecSameidGuildWinTmp.clear();
		for( int vecSame = 0 ; vecSame < (int)( vecSameidGuildWin.size() ) ; ++vecSame )
		{
			float fAvgLv = 0.0f;
			int nSubLevel = 0;
			int nValidObjCount = 0;
//sun: 10, __S_BUG_GC
			__GuildCombatMember* pGCMember = FindGuildCombatMember( vecSameidGuildWin[vecSame] );
			if( pGCMember != NULL )
			{
				for( int veci2 = 0 ; veci2 < (int)( pGCMember->vecGCSelectMember.size() ) ; ++veci2 )
				{
					__JOINPLAYER* pJoinPlayer = pGCMember->vecGCSelectMember.at( veci2 );
					if( 0 < pJoinPlayer->nlife )
					{
						FLWSUser* pUsertmp = (FLWSUser*)prj.GetUserByID( pJoinPlayer->uidPlayer );
						if( IsValidObj( pUsertmp ) )
						{
							nSubLevel += pUsertmp->GetLevel();
							++nValidObjCount;
						}
					}
				}
				
				if( nValidObjCount )
				{
					fAvgLv = (float)( nSubLevel / nValidObjCount );
				}
				
				// ���� �ٲ�
				if( fMaxAvgLv < fAvgLv )
				{
					fMaxAvgLv = fAvgLv;
					vecSameidGuildWinTmp.clear();
					vecSameidGuildWinTmp.push_back( pGCMember->uGuildId );
				}
				else if( fMaxAvgLv == fAvgLv )
				{
					vecSameidGuildWinTmp.push_back( pGCMember->uGuildId );
				}
			}
		}
		vecSameidGuildWin = vecSameidGuildWinTmp;
	}

	char str[512] = {0,};
	CString strPrizeMsg;

	// �� ��ó�� ��尡 ���( ��������̾.. )
	if( 0 < vecSameidGuildWin.size() )
	{
		CGuild* pGuild	= g_GuildMng.GetGuild( vecSameidGuildWin[0] );
		if( pGuild )
		{
			if( g_eLocal.GetState( EVE_GUILDCOMBAT ) )
			{
				g_xWSUserManager->AddWorldCreateSfxObj( XI_INDEX( 1099, XI_NAT_ROCKET02 ), 1291.0f, 85.0f, 1279.0f, FALSE, WI_WORLD_GUILDWAR );
				g_DPCoreClient.SendPlayMusic( WI_WORLD_GUILDWAR, BGM_IN_FITUP );
			}
			
			// ���� ���
			if( m_uWinGuildId == vecSameidGuildWin[0] )
				++m_nWinGuildCount;
			else
				m_nWinGuildCount = 1;
			
			m_uWinGuildId = vecSameidGuildWin[0];
			FLSPrintf( str, _countof( str ), prj.GetText(TID_GAME_GUILDCOMBAT_END) );
			g_DPCoreClient.SendSystem( str );
			g_DPCoreClient.SendCaption( str );
			FLSPrintf( str, _countof( str ), prj.GetText(TID_GAME_GUILDCOMBAT_WINNER), pGuild->m_szGuild );

			int nBufWinGuildCount = m_nWinGuildCount;
			if( m_nMaxGCSendItem < m_nWinGuildCount )
				nBufWinGuildCount = m_nMaxGCSendItem;
			// ���� ������ �ֱ�
			CString strGuildMsg;

			for( int si = 0 ; si < (int)( vecGCSendItem.size() ) ; ++si )
			{
				if( vecGCSendItem.at( si ).nWinCount != nBufWinGuildCount )
					continue;

				// ���â�� �ֱ�
				FLItemElem itemElem;
				itemElem.m_dwItemId = vecGCSendItem.at( si ).dwItemId;
				itemElem.m_nItemNum	= vecGCSendItem.at( si ).nItemNum;
				itemElem.m_nHitPoint = ( itemElem.GetProp()->dwEndurance == -1 ) ? 0 : itemElem.GetProp()->dwEndurance;//itemElem.GetProp()->dwEndurance;
				itemElem.SetSerialNumber();
				if ( pGuild->m_GuildBank.Add( &itemElem ) )
				{
					LogItemInfo aLogItem;
					//aLogItem.Action = "W";
					//aLogItem.SendName = "GUILDCOMBAT";
					//aLogItem.RecvName = "GUILDBANK";
					FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "W" );
					FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), "GUILDCOMBAT" );
					FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "GUILDBANK" );
					g_DPSrvr.OnLogItem( aLogItem, &itemElem, vecGCSendItem.at( si ).nItemNum );
					
					g_DPSrvr.UpdateGuildBank( pGuild, GUILD_PUT_ITEM, 0, 0, &itemElem, 0, vecGCSendItem.at( si ).nItemNum );
					g_xWSUserManager->AddPutItemElem( vecSameidGuildWin[0], &itemElem );

					CString strItemMsg;
					strItemMsg.Format( prj.GetText(TID_UPGRADE_SUPPORTM), itemElem.m_nItemNum );					
					strGuildMsg.Format("  - %s %s", itemElem.GetProp()->szName, strItemMsg );
					m_vecstrGuildMsg.push_back( strGuildMsg );
				}
				else
				{
					LogItemInfo aLogItem;
					//aLogItem.Action = "W";
					//aLogItem.SendName = "GUILDCOMBAT_NOT";
					//aLogItem.RecvName = "GUILDBANK";
					FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "W" );
					FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), "GUILDCOMBAT_NOT" );
					FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "GUILDBANK" );
					g_DPSrvr.OnLogItem( aLogItem, &itemElem, vecGCSendItem.at( si ).nItemNum );
				}
			}
		}
		u_long uBestPlayerGuild;
		int nGetPoint;
		m_uBestPlayer = GetBestPlayer( &uBestPlayerGuild, &nGetPoint );
		
		++m_nGuildCombatIndex;
		g_xWSUserManager->AddGCWinGuild();
		g_xWSUserManager->AddGCBestPlayer();

		g_xWSUserManager->AddGCLogWorld();
	}
}

//sun: 11, ������ Ĩ���� �� Ĩ�� ���� ���� �̿�
void CGuildCombat::GuildCombatResultRanking()
{
	vector<__REQUESTGUILD> vecGCRanking;
	vecGCRanking = vecRequestRanking;
	
	for( int i=0; i<(int)( vecGCRanking.size()-1 ); i++ )
	{
		if( i >= m_nMaxGuild )
			break;
		// �������� ����
		for( int j=0; j<(int)( vecGCRanking.size()-1-i ); j++ )
		{
			__REQUESTGUILD RequestGuild1 = vecGCRanking.at( j );
			__GuildCombatMember* pGCMember1 = FindGuildCombatMember( RequestGuild1.uidGuild );
			__REQUESTGUILD RequestGuild2 = vecGCRanking.at( j+1 );
			__GuildCombatMember* pGCMember2 = FindGuildCombatMember( RequestGuild2.uidGuild );
			
			if( !pGCMember1 || !pGCMember2 )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "pGCMember1 or pGCMember2 is NULL" ) );
				return;
			}

			// ������ ���� ��尡 ���� ����
			if( pGCMember1->nGuildPoint < pGCMember2->nGuildPoint )
			{
				__REQUESTGUILD temp = vecGCRanking.at( j );
				vecGCRanking.at( j ) = vecGCRanking.at( j+1 );
				vecGCRanking.at( j+1 ) = temp;
			}
			// ������ ���� �� ������ �������� ���� ����
			else if( pGCMember1->nGuildPoint == pGCMember2->nGuildPoint )
			{
				int nLife1=0, nLife2=0;
				for( int k=0 ; k<(int)( pGCMember1->vecGCSelectMember.size() ); k++ )
				{
					__JOINPLAYER* pJoinPlayer = pGCMember1->vecGCSelectMember.at( k );
					if( pJoinPlayer )
						nLife1 += pJoinPlayer->nlife;
				}
				for( int k=0 ; k<(int)( pGCMember2->vecGCSelectMember.size() ); k++ )
				{
					__JOINPLAYER* pJoinPlayer = pGCMember2->vecGCSelectMember.at( k );
					if( pJoinPlayer )
						nLife2 += pJoinPlayer->nlife;
				}
	
				if( nLife1 < nLife2 )
				{
					__REQUESTGUILD temp = vecGCRanking.at( j );
					vecGCRanking.at( j ) = vecGCRanking.at( j+1 );
					vecGCRanking.at( j+1 ) = temp;
				}
				// ���� ���� �� ������ ���� �������� ��շ����� ���� ���
				else if( nLife1 == nLife2 )
				{
					int nLevel=0, nCount = 0;
					float fAvg1=0, fAvg2=0;
					for( int k=0 ; k<(int)( pGCMember1->vecGCSelectMember.size() ); k++ )
					{
						__JOINPLAYER* pJoinPlayer = pGCMember1->vecGCSelectMember.at( k );
						if( pJoinPlayer && 0 < pJoinPlayer->nlife )
						{
							FLWSUser* pUsertmp = (FLWSUser*)prj.GetUserByID( pJoinPlayer->uidPlayer );
							if( IsValidObj( pUsertmp ) )
							{
								nLevel += pUsertmp->GetLevel();
								nCount++;
							}
						}
					}
					if( nCount )
						fAvg1 = (float)( nLevel/nCount );
					else
						fAvg1 = 1.0f;
					nLevel = nCount = 0;
					for( int k=0 ; k<(int)( pGCMember2->vecGCSelectMember.size() ); k++ )
					{
						__JOINPLAYER* pJoinPlayer = pGCMember2->vecGCSelectMember.at( k );
						if( pJoinPlayer && 0 < pJoinPlayer->nlife )
						{
							FLWSUser* pUsertmp = (FLWSUser*)prj.GetUserByID( pJoinPlayer->uidPlayer );
							if( IsValidObj( pUsertmp ) )
							{
								nLevel += pUsertmp->GetLevel();
								nCount++;
							}
						}
					}
					if( nCount )
						fAvg2 = (float)( nLevel/nCount );
					else
						fAvg2 = 1.0f;

					if( fAvg1 < fAvg2 )
					{
						__REQUESTGUILD temp = vecGCRanking.at( j );
						vecGCRanking.at( j ) = vecGCRanking.at( j+1 );
						vecGCRanking.at( j+1 ) = temp;
					}
				} //if ����
			} //if ����
		} //for j
	} //for i

	// ������(����Ĩ) ����
//	for( int i=0; i<(int)( vecGCRanking.size() ); i++ )
//	{
//		if( i >= 3 ) break;
//
//		__REQUESTGUILD RequestGuild = vecGCRanking[i];
//		__GuildCombatMember* pGCMember = FindGuildCombatMember( RequestGuild.uidGuild );
//		if( !pGCMember )
//		{
//			FLERROR_LOG( PROGRAM_NAME, _T( "pGCMember is NULL" ) );
//			continue;
//		}
//
//		FLItemElem itemElem;
////		itemElem.m_dwItemId = ITEM_INDEX( 26460, II_CHP_RED );
//
//		const DWORD dwRewardItemID	= GetRewardItemID();
//		itemElem.m_dwItemId = dwRewardItemID;
//
//		float fChipNum = m_nJoinPanya * vecGCRanking.size() * 0.9f * 0.00001f * 0.1f;
//		switch( i )
//		{
//			case 0 :	// 1�� 
//				fChipNum *= 0.7f;
//				break;
//			case 1 :	// 2��
//				fChipNum *= 0.2f;
//				break;
//			case 2 :	// 3��
//				fChipNum *= 0.1f;
//				break;
//		}
//		itemElem.m_nItemNum = (int)fChipNum;
//		if( itemElem.m_nItemNum < 1 )
//			itemElem.m_nItemNum		= 1;
//		itemElem.m_bCharged = itemElem.GetProp()->bCharged;
//
//		LogItemInfo aLogItem;
//		//aLogItem.Action = "W";
//		FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "W" );
//		aLogItem.Gold_1 = i;
//					
//		for( int j=0 ; j<(int)( pGCMember->vecGCSelectMember.size() ); j++ )
//		{
//			__JOINPLAYER* pJoinPlayer = pGCMember->vecGCSelectMember.at( j );
//			if( !pJoinPlayer )
//			{
//				FLERROR_LOG( PROGRAM_NAME, _T( "pJoinPlayer is NULL" ) );
//				continue;
//			}
//
//			FLWSUser* pUsertmp = (FLWSUser*)prj.GetUserByID( pJoinPlayer->uidPlayer );
//			if( IsValidObj( pUsertmp ) )
//			{
//				//aLogItem.SendName = pUsertmp->GetName();
//				FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUsertmp->GetName() );
//				// ������ ����
//				itemElem.SetSerialNumber();
//				if( pUsertmp->CreateItem( &itemElem ) )
//				{
//					//aLogItem.RecvName = "GUILDCOMBAT_CHIP";
//					FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "GUILDCOMBAT_CHIP" );
//					pUsertmp->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_WARGUILDWINCHIP, "%d", itemElem.m_nItemNum );
//				}
//				else
//				{
//					g_dpDBClient.SendQueryPostMail( pUsertmp->m_idPlayer, 0, itemElem, 0, itemElem.GetProp()->szName, itemElem.GetProp()->szName );
//					//pUsertmp->AddDefinedText( TID_GAME_MINIGAME_ITEM_POST, "" );
//					//aLogItem.RecvName = "GUILDCOMBAT_CHIP_POST";
//					FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "GUILDCOMBAT_CHIP_POST" );
//				}
////				int cbChip	= pUsertmp->GetItemNum( ITEM_INDEX( 26460, II_CHP_RED ) );
//				const int cbChip	= pUsertmp->GetItemNum( dwRewardItemID );
//
//				aLogItem.Gold	= cbChip;
//				aLogItem.Gold2	= cbChip + itemElem.m_nItemNum;
//				aLogItem.Gold_1 = itemElem.m_nItemNum;
//				g_DPSrvr.OnLogItem( aLogItem, &itemElem, itemElem.m_nItemNum );
//				FLTRACE_LOG( PROGRAM_NAME, _T( "GC REDCHIP - %s, %d��, %s : %d��" ), pUsertmp->GetName(), i+1, itemElem.GetProp()->szName, itemElem.m_nItemNum );
//			}
//		}
//	}

	for( int i=0; i<(int)( vecGCRanking.size() ); i++ )
	{
		//if( i >= 3 ) break;

		__REQUESTGUILD RequestGuild = vecGCRanking[i];
		__GuildCombatMember* pGCMember = FindGuildCombatMember( RequestGuild.uidGuild );
		if( !pGCMember )
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "pGCMember is NULL" ) );
			continue;
		}

		FLItemElem itemElem;
		
		for( int k=0 ; k<(int)( m_vecRewardItem.size() ); k++ )
		{
			if ( i != m_vecRewardItem[k].m_dwRank -1 )
				continue;
			if(m_uWinGuildId == RequestGuild.uidGuild && m_nWinGuildCount != m_vecRewardItem[k].m_nWinCount)
				continue;

			itemElem.m_dwItemId = m_vecRewardItem[k].m_dwRewardItemID;
			itemElem.m_nItemNum		= 1;
			itemElem.m_bCharged = itemElem.GetProp()->bCharged;

			LogItemInfo aLogItem;
			//aLogItem.Action = "W";
			FLStrcpy( aLogItem.Action, _countof( aLogItem.Action ), "W" );
			aLogItem.Gold_1 = i;
						
			for( int j=0 ; j<(int)( pGCMember->vecGCSelectMember.size() ); j++ )
			{
				__JOINPLAYER* pJoinPlayer = pGCMember->vecGCSelectMember.at( j );
				if( !pJoinPlayer )
				{
					FLERROR_LOG( PROGRAM_NAME, _T( "pJoinPlayer is NULL" ) );
					continue;
				}

				FLWSUser* pUsertmp = (FLWSUser*)prj.GetUserByID( pJoinPlayer->uidPlayer );
				if( IsValidObj( pUsertmp ) )
				{
					//aLogItem.SendName = pUsertmp->GetName();
					FLStrcpy( aLogItem.SendName, _countof( aLogItem.SendName ), pUsertmp->GetName() );
					// ������ ����
					itemElem.SetSerialNumber();
					if( pUsertmp->CreateItem( &itemElem ) )
					{
						//aLogItem.RecvName = "GUILDCOMBAT_CHIP";
						FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "GUILDCOMBAT_CHIP" );
						pUsertmp->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_WARGUILDWINCHIP, "%d", itemElem.m_nItemNum );
					}
					else
					{
						g_dpDBClient.SendQueryPostMail( pUsertmp->m_idPlayer, 0, itemElem, 0, itemElem.GetProp()->szName, itemElem.GetProp()->szName );
						//pUsertmp->AddDefinedText( TID_GAME_MINIGAME_ITEM_POST, "" );
						//aLogItem.RecvName = "GUILDCOMBAT_CHIP_POST";
						FLStrcpy( aLogItem.RecvName, _countof( aLogItem.RecvName ), "GUILDCOMBAT_CHIP_POST" );
					}
	//				int cbChip	= pUsertmp->GetItemNum( ITEM_INDEX( 26460, II_CHP_RED ) );
					int cbChip	= pUsertmp->GetItemNum( m_vecRewardItem[k].m_dwRewardItemID );

					aLogItem.Gold	= cbChip;
					aLogItem.Gold2	= cbChip + itemElem.m_nItemNum;
					aLogItem.Gold_1 = itemElem.m_nItemNum;
					g_DPSrvr.OnLogItem( aLogItem, &itemElem, itemElem.m_nItemNum );
					FLTRACE_LOG( PROGRAM_NAME, _T( "GC REDCHIP - %s, %d��, %s : %d��" ), pUsertmp->GetName(), i+1, itemElem.GetProp()->szName, itemElem.m_nItemNum );
				}
			}
		}
	}
#ifdef GUILD_WINNER_BUFF
	CGuild* pGuild	= g_GuildMng.GetGuild(vecGCRanking[0].uidGuild);

	CGuildMember* pMember;
	CTime tNextTime = GetNextGuildCobmatTime();
	time_t d = (time_t)( tNextTime.GetTime() - time_null() );

	for( map<u_long, CGuildMember*>::iterator i = pGuild->m_mapPMember.begin();	i != pGuild->m_mapPMember.end(); ++i )
	{
		pMember		= i->second;

		FLWSUser* pUsertmp = (FLWSUser*)prj.GetUserByID( pMember->m_idPlayer );
		if( IsValidObj( pUsertmp ) )
		{
			if(m_nWinGuildCount == 1)
				pUsertmp->AddBuff( BUFF_ITEM, ITEM_INDEX( 25582, II_SYS_SYS_LS_WINNERDIGNITY ), 0, d * 1000, 0, BT_TICK );
			else if(m_nWinGuildCount == 2)
				pUsertmp->AddBuff( BUFF_ITEM, ITEM_INDEX( 25583, II_SYS_SYS_LS_SPLENDIDGLORY ), 0, d * 1000, 0, BT_TICK );
		}
	}
#endif
}

void CGuildCombat::GuildCombatCloseTeleport()
{
	if( m_nStopWar != 200 )
		g_dpDBClient.SendGuildCombatResult();
	
	PRegionElem pRgnElem = g_WorldMng.GetRevivalPos( WI_WORLD_MADRIGAL, "flaris" );
	if( NULL == pRgnElem )
		return;

#ifdef __LAYER_1015
	g_xWSUserManager->ReplaceWorld( WI_WORLD_GUILDWAR, WI_WORLD_MADRIGAL, 6968.0f, 3328.8f, nDefaultLayer );
#else	// __LAYER_1015
	g_xWSUserManager->ReplaceWorld( WI_WORLD_GUILDWAR, WI_WORLD_MADRIGAL, 6968.0f, 3328.8f );
#endif	// __LAYER_1015

//sun: 8, // __GUILDCOMBAT_85
	for( int i = 0 ; i < (int)( m_vecstrGuildMsg.size() ) ; ++i )
		g_xWSUserManager->AddGuildMsg( m_uWinGuildId, m_vecstrGuildMsg.at( i ) );

	m_nState = CLOSE_STATE;
	m_nGCState = WAR_CLOSE_STATE;
	m_dwTime = 0;
	m_nStopWar = 0;
	if( g_eLocal.GetState( EVE_GUILDCOMBAT ) )
		g_DPCoreClient.SendGuildCombatState( CLOSE_STATE );	

	m_bMutex = FALSE;	

	SetNpc();
}

void CGuildCombat::SetNpc( void )
{
	CWorld* pWorld	= g_WorldMng.GetWorld( WI_WORLD_GUILDWAR );
	if( pWorld != NULL )
	{
		CMover* pMover	= (CMover*)CreateObj( D3DDEVICE, OT_MOVER, OBJECT_INDEX( 229, MI_MADA_ACHABEN ) );
		if( pMover != NULL )
		{
			FLStrcpy( pMover->m_szCharacterKey, _countof( pMover->m_szCharacterKey ), "Mada_Guildcombatshop" );
			pMover->InitNPCProperty();
			pMover->InitCharacter( pMover->GetCharacter() );
			pMover->SetPos( D3DXVECTOR3(1343, 94, 1215) );
			pMover->InitMotion( MTI_STAND );
			pMover->UpdateLocalMatrix();
			if( pWorld->ADDOBJ( pMover, TRUE, nDefaultLayer ) == FALSE )
			{
				SAFE_DELETE( pMover );
			}
		}

		CMover* pMover1	= (CMover*)CreateObj( D3DDEVICE, OT_MOVER, OBJECT_INDEX( 229, MI_MADA_ACHABEN ) );
		if( pMover1 != NULL )
		{
			FLStrcpy( pMover1->m_szCharacterKey, _countof( pMover1->m_szCharacterKey ), "Mada_Guildcombatshop" );
			pMover1->InitNPCProperty();
			pMover1->InitCharacter( pMover1->GetCharacter() );
			pMover1->SetPos( D3DXVECTOR3(1288, 98, 1356) );
			pMover1->InitMotion( MTI_STAND );
			pMover1->UpdateLocalMatrix();
			if( pWorld->ADDOBJ( pMover1, TRUE, nDefaultLayer ) == FALSE )
			{
				SAFE_DELETE( pMover1 );
			}
		}
	}
}

void CGuildCombat::GuildCombatOpen( void )
{
	// ������� ������ �ִ� ��� ĳ���͵��� ��� ��������..
	if( m_nState != CLOSE_STATE )
		return;

	m_vecstrGuildMsg.clear();		//sun: 8, // __GUILDCOMBAT_85

	if( g_eLocal.GetState( EVE_GUILDCOMBAT ) )
		g_DPCoreClient.SendGuildCombatState( OPEN_STATE );	

	m_vecGCGetPoint.clear();
	m_GCResultValueGuild.clear();
	m_GCResultValuePlayer.clear();
	g_dpDBClient.SendGuildCombatStart();
	m_nGCState = NOTENTER_STATE;
	m_nProcessGo	= 0;
	CString strOK;
	CString strCancle;
	strOK.Format( "%s", prj.GetText(TID_GAME_GUILDCOMBAT_JOIN_OK) );
	strCancle.Format( "%s", prj.GetText(TID_GAME_GUILDCOMBAT_JOIN_CANCLE) );

	for( int veci = 0 ; veci < (int)( vecRequestRanking.size() ) ; ++veci )
	{
		__REQUESTGUILD RequestGuild = vecRequestRanking.at( veci );

		CGuild* pGuild = g_GuildMng.GetGuild( RequestGuild.uidGuild );
		if( pGuild )
		{
			for( map<u_long, CGuildMember*>::iterator Guildi = pGuild->m_mapPMember.begin();	// neuz
			Guildi != pGuild->m_mapPMember.end(); ++Guildi )
			{
				CGuildMember* pGuildMember = Guildi->second;
				if( pGuildMember->m_nMemberLv == GUD_MASTER || pGuildMember->m_nMemberLv == GUD_KINGPIN )
				{
					FLWSUser* pUser	= (FLWSUser*)prj.GetUserByID( pGuildMember->m_idPlayer );
					if( IsValidObj( pUser ) )
					{
						if( veci < m_nMaxGuild )
							pUser->AddGCDiagMessage( strOK );
						else
							pUser->AddGCDiagMessage( strCancle );
					}
				}
			}
		}
	}
}
void CGuildCombat::SetRequestRanking( void )
{
	vecRequestRanking.clear();
//sun: 10, __S_BUG_GC
	for( int gcmi = 0 ; gcmi < (int)( m_vecGuildCombatMem.size() ) ; ++gcmi )
	{
		__GuildCombatMember* pGCMember = m_vecGuildCombatMem.at( gcmi );

		if( pGCMember->bRequest == FALSE )
			continue;
		
		__REQUESTGUILD RequstGuild;
		RequstGuild.dwPenya = pGCMember->dwPenya;
		RequstGuild.uidGuild = pGCMember->uGuildId;
		if( vecRequestRanking.empty() == true )
		{
			vecRequestRanking.push_back( RequstGuild );
		}
		else
		{
			BOOL bInsert = FALSE;
			for( vector<__REQUESTGUILD>::iterator  itv = vecRequestRanking.begin() ; itv != vecRequestRanking.end() ; ++itv )
			{
				if( ((__REQUESTGUILD)*itv).dwPenya  == pGCMember->dwPenya )
				{
					BOOL bSamPenya = FALSE;
					while( ((__REQUESTGUILD)*itv).dwPenya == pGCMember->dwPenya )
					{
						++itv;
						if( itv == vecRequestRanking.end() )
						{
							bSamPenya = TRUE; 
							--itv;
							break;
						}							
					}
					
					if( bSamPenya )
					{
						vecRequestRanking.push_back( RequstGuild );
					}
					else
					{
						vecRequestRanking.insert( itv, RequstGuild );
					}
					bInsert = TRUE;
					break;
				}
				else
				if( ((__REQUESTGUILD)*itv).dwPenya  < pGCMember->dwPenya )
				{
					vecRequestRanking.insert( itv, RequstGuild );
					bInsert = TRUE;
					break;
				}				
			}
			if( bInsert == FALSE )
			{
				vecRequestRanking.push_back( RequstGuild );
			}
		}
	}

}
void CGuildCombat::SetDefender( u_long uidGuild, u_long uidDefender )
{
//sun: 10, __S_BUG_GC
	__GuildCombatMember* pGCMember = FindGuildCombatMember( uidGuild );
	if( pGCMember != NULL )
		pGCMember->m_uidDefender = uidDefender;
}
u_long CGuildCombat::GetDefender( u_long uidGuild )
{
	u_long uidDefender = 0;
//sun: 10, __S_BUG_GC
	__GuildCombatMember* pGCMember = FindGuildCombatMember( uidGuild );
	if( pGCMember != NULL )
		uidDefender = pGCMember->m_uidDefender;

	return uidDefender;
}
u_long CGuildCombat::GetBestPlayer( u_long* dwGetGuildId, int* nGetPoint )
{
	u_long uBestPlayer = 0;
	map<u_long, int> mapSame;	// ������ �߻��� ó���ϱ� ���� playerId
	// ����Ʈ �˻�
	BOOL bResult = FALSE;
	int nMaxPoint = -1;
	for( int nVeci = 0 ; nVeci < (int)( vecRequestRanking.size() ) ; ++nVeci )
	{
		if( nVeci >= m_nMaxGuild )
			break;
		
		__REQUESTGUILD RequestGuild = vecRequestRanking.at( nVeci );
//sun: 10, __S_BUG_GC
		__GuildCombatMember* pGCMember = FindGuildCombatMember( RequestGuild.uidGuild );
		if( pGCMember != NULL )
		{
			for( int veci = 0 ; veci < (int)( pGCMember->vecGCSelectMember.size() ) ; ++veci )
			{
				__JOINPLAYER* pJoinPlayer = pGCMember->vecGCSelectMember.at( veci );
				if( nMaxPoint < pJoinPlayer->nPoint )
				{
					nMaxPoint = pJoinPlayer->nPoint;
					uBestPlayer = pJoinPlayer->uidPlayer;
					*nGetPoint = pJoinPlayer->nPoint;
					*dwGetGuildId = pGCMember->uGuildId;
					bResult = TRUE;
					mapSame.clear();
					mapSame.insert( make_pair( pJoinPlayer->uidPlayer, nVeci ) );
				}
				else if( nMaxPoint == pJoinPlayer->nPoint )
				{
					bResult = FALSE;
					mapSame.insert( make_pair( pJoinPlayer->uidPlayer, nVeci ) );
				}
			}
		}
	}

	// ���� & ����ġ �˻�
//sun: 10, __S_BUG_GC	// ������ ó�� ���� ���� chipi 080218
	int nMinLevel = 0x7fffffff;
	EXPINTEGER nMinExp = (EXPINTEGER)0x7fffffffffffffff;

	if( bResult == FALSE )
	{
		for( int nVeci = 0 ; nVeci < (int)( vecRequestRanking.size() ) ; ++nVeci )
		{
			if( nVeci >= m_nMaxGuild )
				break;

			__REQUESTGUILD RequestGuild = vecRequestRanking.at( nVeci );
//sun: 10, __S_BUG_GC
			__GuildCombatMember* pGCMember = FindGuildCombatMember( RequestGuild.uidGuild );
			if( pGCMember != NULL )
			{
				for( int veci = 0 ; veci < (int)( pGCMember->vecGCSelectMember.size() ) ; ++veci )
				{
					__JOINPLAYER* pJoinPlayer = pGCMember->vecGCSelectMember.at( veci );
					// �ְ� ������ �����ڰ� �ƴ� ��� continue
					if( !pJoinPlayer || mapSame.find( pJoinPlayer->uidPlayer ) == mapSame.end() )
						continue;
					
					CMover* pMover = prj.GetUserByID( pJoinPlayer->uidPlayer );
					if( IsValidObj( pMover ) )
					{
						//if( nMaxLevel < pMover->GetLevel() )
						if( nMinLevel > pMover->GetLevel() )
						{
							//nMaxLevel = pMover->GetLevel();
							//nMaxExp = pMover->GetExp1();
							nMinLevel = pMover->GetLevel();
							nMinExp = pMover->GetExp1();
							uBestPlayer = pJoinPlayer->uidPlayer;
							*nGetPoint = pJoinPlayer->nPoint;
							*dwGetGuildId = pGCMember->uGuildId;
						}
						//else if( nMaxLevel == pMover->GetLevel() )
						else if( nMinLevel == pMover->GetLevel() )
						{
							if( nMinExp > pMover->GetExp1() )
							{
								//nMaxLevel = pMover->GetLevel();
								//nMaxExp = pMover->GetExp1();
								nMinLevel = pMover->GetLevel();
								nMinExp = pMover->GetExp1();
								uBestPlayer = pJoinPlayer->uidPlayer;
								*nGetPoint = pJoinPlayer->nPoint;
								*dwGetGuildId = pGCMember->uGuildId;
							}
						}
					}
				}
			}
		}
	}
	return uBestPlayer;
}

// ���ݱ����� �� ���
__int64 CGuildCombat::GetPrizePenya( int nFlag )
{
	// ��ݿ� ������ ����..
	__int64 nPrizePenya = 0;
	for( int veci = 0 ; veci < (int)( vecRequestRanking.size() ) ; ++veci )
	{
		if( veci >= m_nMaxGuild )
			break;
		
		nPrizePenya += vecRequestRanking.at( veci ).dwPenya;
	}
	__int64 nResult = 0;

	switch( nFlag )
	{
	case 0:
		nResult = MulDiv( (int)( nPrizePenya ), m_nMaxGuildPercent, 100 );
		break;
	case 1:
		nResult = MulDiv( (int)( nPrizePenya ), m_nMaxPlayerPercent, 100 );
		break;
	case 2:
		nResult = nPrizePenya;
	}

	return nResult;
}
// ����� ��û�� Penya
DWORD CGuildCombat::GetRequstPenya( u_long uidGuild )
{
	DWORD dwRequestPenya = 0;
//sun: 10, __S_BUG_GC
	__GuildCombatMember* pGCMember = FindGuildCombatMember( uidGuild );
	if( pGCMember != NULL )
		dwRequestPenya = pGCMember->dwPenya;

	return dwRequestPenya;
}
// ���ݺ��� ���� ������ ���۵� �����ð�
CTime CGuildCombat::GetNextGuildCobmatTime()
{
	CTime tCurrentTime = CTime::GetCurrentTime();
	CTime tNextCombat;
	int nDayofWeek = m_nDay - tCurrentTime.GetDayOfWeek();
	if( 0 < nDayofWeek )
		tNextCombat = tCurrentTime + CTimeSpan( nDayofWeek, 0, 0, 0 ); 
	else if( 0 == nDayofWeek )
	{
		CTimeSpan tCTime = CTimeSpan( 0, tCurrentTime.GetHour(), tCurrentTime.GetMinute(), tCurrentTime.GetSecond() );
		CTimeSpan tNTime = CTimeSpan( 0, __AutoOpen[m_nDay-1].nHour, __AutoOpen[m_nDay-1].nMinute, 0 );
		if( tCTime <= tNTime )
			tNextCombat = tCurrentTime;
		else
			tNextCombat = tCurrentTime + CTimeSpan( m_nDay, 0, 0, 0 );
	}
	else if( 0 > nDayofWeek )
	{
		nDayofWeek = 7 + nDayofWeek;
		tNextCombat = tCurrentTime + CTimeSpan( nDayofWeek, 0, 0, 0 ); 
	}
	tNextCombat = CTime( tNextCombat.GetYear(), tNextCombat.GetMonth(), tNextCombat.GetDay(), __AutoOpen[m_nDay-1].nHour, __AutoOpen[m_nDay-1].nMinute, 0 );
	return tNextCombat;
}

//sun: 10, __S_BUG_GC
CGuildCombat::__GuildCombatMember* CGuildCombat::FindGuildCombatMember( u_long GuildId )
{
	__GuildCombatMember* pGCMem = NULL;
	for( int i = 0 ; i < (int)( m_vecGuildCombatMem.size() ) ; ++i )
	{
		if( GuildId == m_vecGuildCombatMem.at( i )->uGuildId )
		{
			pGCMem = m_vecGuildCombatMem.at( i );
			break;
		}
	}
	return pGCMem;
}

bool	CGuildCombat::DeleteGuildCombatTender( const u_long idGuild )
{
	for( std::vector< __GuildCombatMember* >::iterator pos = m_vecGuildCombatMem.begin(); pos != m_vecGuildCombatMem.end(); ++pos )
	{
		__GuildCombatMember* pGCMem = *pos;
		if( pGCMem->uGuildId == idGuild )
		{
			for( std::vector< __JOINPLAYER* >::iterator kItr = pGCMem->vecGCSelectMember.begin(); kItr != pGCMem->vecGCSelectMember.end(); ++kItr )
			{
				__JOINPLAYER* pJoinPlayer = *kItr;
				SAFE_DELETE( pJoinPlayer );
			}
			pGCMem->vecGCSelectMember.clear();

			SAFE_DELETE( pGCMem );
			m_vecGuildCombatMem.erase( pos );

			return true;
		}
	}

	return false;
}

void CGuildCombat::SetSelectMap( FLWSUser* pUser, int nMap )
{
//sun: 10, __S_BUG_GC
	__GuildCombatMember* pGCMember = FindGuildCombatMember( pUser->m_idGuild );
	if( pGCMember != NULL )
	{	
		for( int veci = 0 ; veci < (int)( pGCMember->vecGCSelectMember.size() ) ; ++veci )
		{
			__JOINPLAYER* pJoinPlayer = pGCMember->vecGCSelectMember.at( veci );
			if( pJoinPlayer->uidPlayer == pUser->m_idPlayer )
			{
				pJoinPlayer->nMap = nMap;
				break;
			}
		}
	}
}
void CGuildCombat::GuildCombatEnter( FLWSUser* pUser )
{
	BOOL bJoin = TRUE;
	if( m_nState == OPEN_STATE || m_nState == CLOSE_STATE || m_nState == WAR_STATE )
	{
		BOOL bJoin = TRUE;
		if( m_nState == OPEN_STATE && m_nGCState != ENTER_STATE )
		{
			bJoin = FALSE;
			pUser->AddText( prj.GetText(TID_GAME_GUILDCOMBAT_NOT_JOIN) );
		}

		if( bJoin )
		{
			JoinObserver( pUser );
		}
	}
	else if( m_nState == COMPLET_CLOSE_STATE )
	{
		// �ƹ��� ������.. ������ �����ִ� ����
		pUser->AddText( prj.GetText(TID_GAME_GUILDCOMBAT_NOT_OPEN) );
		bJoin = FALSE;
	}
	else
	{
		bJoin = FALSE;
	}

	if( bJoin )
	{
		if( 0 < m_dwTime )
		{		
			pUser->AddGuildCombatNextTime( m_dwTime - GetTickCount(), GuildCombatProcess[m_nProcessGo - 1].dwParam );
		}
	}
}
// ���� �غ� �ð�
void CGuildCombat::SetMaintenance()
{
	CString strMsgMaster = prj.GetText( TID_GAME_GUILDCOMBAT_JOIN_MSG_MASTER );
	CString strMsgDefender = prj.GetText( TID_GAME_GUILDCOMBAT_JOIN_MSG_DEFENDER );
	CString strMsgGeneral = prj.GetText( TID_GAME_GUILDCOMBAT_JOIN_MSG_GENERAL );
	
	BOOL bWinGuild_Continue = FALSE;		// ��������� ��� �Ǿ��ٸ� ������ ���� ����
	int nCount = 0;
	for( int nVeci = 0 ; nVeci < (int)( vecRequestRanking.size() ) ; ++nVeci )
	{
		// �ִ�� ���� �ִ� ��常 �鸸 ��������
		if( nVeci >= m_nMaxGuild )
			break;

		__REQUESTGUILD RequestGuild = vecRequestRanking.at( nVeci );

		if( RequestGuild.uidGuild == m_uWinGuildId )
			bWinGuild_Continue = TRUE;

//sun: 10, __S_BUG_GC
		__GuildCombatMember* pGCMember = FindGuildCombatMember( RequestGuild.uidGuild );
		if( pGCMember != NULL )
		{
			CGuild* pGuild = g_GuildMng.GetGuild( pGCMember->uGuildId );

			FLERROR_LOG( PROGRAM_NAME, _T( "Guild id, %d" ), pGCMember->uGuildId );		//Test
			if( pGuild )
			{
				pGCMember->nJoinCount = 0;
				pGCMember->nWarCount = 0;
//				BOOL bMaxWarCount = FALSE;
				FLERROR_LOG( PROGRAM_NAME, _T( "SelectMember size, %d" ), pGCMember->vecGCSelectMember.size() );		//Test
				for( int i = 0; i < (int)( pGCMember->vecGCSelectMember.size() ) ; ++i )
				{
					__JOINPLAYER* pJoinPlayer	= pGCMember->vecGCSelectMember.at( i );
					FLERROR_LOG( PROGRAM_NAME, _T( "SelectMember uidPlayer, %d" ), pJoinPlayer->uidPlayer);		//Test
					if( 0 < pJoinPlayer->nlife )
						pGCMember->lspFifo.push_back( pJoinPlayer );
				}

				while( pGCMember->lspFifo.empty() == false )
				{
					__JOINPLAYER* pJoinPlayer	= pGCMember->lspFifo.front();
					CMover* pMover	= prj.GetUserByID( pJoinPlayer->uidPlayer );
					if( IsValidObj( pMover ) && pMover->GetWorld()->GetID() == WI_WORLD_GUILDWAR )
					{
						pGCMember->nWarCount++;
//						pJoinPlayer->bEntry	= TRUE;
						pJoinPlayer->dwTelTime	= timeGetTime();
						((FLWSUser*)pMover)->AddGCJoinWarWindow( pJoinPlayer->nMap, m_nMaxMapTime );
						pGCMember->lspFifo.pop_front();
					}
					else
					{
						pGCMember->lspFifo.pop_front();
						pJoinPlayer->nlife--;
						if( pJoinPlayer->nlife > 0 )
							pGCMember->lspFifo.push_back( pJoinPlayer );
						if( pJoinPlayer->nlife < 0 )
							pJoinPlayer->nlife	= 0;
					}
					if( pGCMember->nWarCount == m_nMaxWarPlayer )
						break;
				}

				g_xWSUserManager->AddGCGuildStatus( pGCMember->uGuildId );

				for( int veci = 0; veci < (int)( pGCMember->vecGCSelectMember.size() ) ; ++veci )
				{
					__JOINPLAYER* pJoinPlayer	= pGCMember->vecGCSelectMember.at( veci );
					CMover* pMover	= prj.GetUserByID( pJoinPlayer->uidPlayer );
					if( IsValidObj( pMover ) && pMover->GetWorld()->GetID() == WI_WORLD_GUILDWAR )
					{
						// Message
						if( pGuild->IsMaster( pMover->m_idPlayer ) )	// Master
							((FLWSUser*)pMover)->AddDiagText( strMsgMaster );
						else if( pGCMember->m_uidDefender == pMover->m_idPlayer )	// Defender
							((FLWSUser*)pMover)->AddDiagText( strMsgDefender );
						else
							((FLWSUser*)pMover)->AddDiagText( strMsgGeneral );
					}
				}

				if( pGCMember->vecGCSelectMember.empty() == false )
					++nCount;
				FLERROR_LOG( PROGRAM_NAME, _T( "SelectMember nCount, %d" ), nCount);		//Test
			}
		}
	}

	// ������ ��尡 2�� �̻��϶��� �������� �����ϰ���.
//sun: 8, // __GUILDCOMBAT_85
	if( nCount < m_nMinGuild )
//!	if( nCount < 2 )
	{
		// ������ �ȵǾ�����
		// ������ ��尡 �����ϰ� ������ �¼� �̾
		// �������� �ʾ����� �¼� ������( �¸��� ��尡 ���� )
		if( bWinGuild_Continue == FALSE )
		{
			g_dpDBClient.SendGuildcombatContinue( m_nGuildCombatIndex-1, m_uWinGuildId, 0 );
			m_uWinGuildId = 0;
			m_nWinGuildCount = 0;

			g_xWSUserManager->AddGCWinGuild();
		}
		CString str;
		str.Format( prj.GetText(TID_GAME_GUILDWAR_NOCONFIG), m_nMinGuild );
		g_DPCoreClient.SendSystem( str );
		SetGuildCombatCloseWait( TRUE );
	}
}

void CGuildCombat::SetEnter()
{
	char str[512] = {0,};
	FLSPrintf( str, _countof( str ), prj.GetText(TID_GAME_GUILDCOMBAT_ENTER) );
	g_DPCoreClient.SendSystem( str );
	g_DPCoreClient.SendCaption( str, 0, TRUE );

	for( int nVeci = 0 ; nVeci < (int)( vecRequestRanking.size() ) ; ++nVeci )
	{
		// �ִ�� ���� �ִ� ��常 �鸸 ��������
		if( nVeci >= m_nMaxGuild )
			break;
		
		__REQUESTGUILD RequestGuild = vecRequestRanking.at( nVeci );
		__GuildCombatMember* pGCMember = FindGuildCombatMember( RequestGuild.uidGuild );
		if( pGCMember != NULL )
		{
			for( int veci = 0 ; veci < (int)( pGCMember->vecGCSelectMember.size() ) ; ++veci )
			{
				__JOINPLAYER* pJoinPlayer = pGCMember->vecGCSelectMember.at( veci );
				CMover* pMover = prj.GetUserByID( pJoinPlayer->uidPlayer );
				if( IsValidObj( pMover ) )
				{
					FLSPrintf( str, _countof( str ), "%s", prj.GetText(TID_GAME_GUILDCOMBAT_TELE) );
					((FLWSUser*)pMover)->AddGCTele( str );
				}
			}
		}
	}
}
// ���� ����
void CGuildCombat::SetGuildCombatStart()
{
	__GuildCombatMember* pGuildCombatMem;
	__JOINPLAYER* pJoinPlayer;
	__REQUESTGUILD RequestGuild;

	for( int nVeci = 0 ; nVeci < (int)( vecRequestRanking.size() ) ; ++nVeci )
	{
		if( nVeci >= m_nMaxGuild )
			break;
		
		RequestGuild = vecRequestRanking.at( nVeci );

		pGuildCombatMem = FindGuildCombatMember( RequestGuild.uidGuild );
		if( pGuildCombatMem != NULL )
		{
			for( int veci = 0 ; veci < (int)( pGuildCombatMem->vecGCSelectMember.size() ) ; ++veci )
			{
				pJoinPlayer = pGuildCombatMem->vecGCSelectMember.at( veci );
				BOOL bFind	= FALSE;
				for( list<__JOINPLAYER*>::iterator i1 = pGuildCombatMem->lspFifo.begin(); i1 != pGuildCombatMem->lspFifo.end(); ++i1 )
				{
					if( pJoinPlayer == *i1 )
					{
						bFind	= TRUE;
						break;
					}
				}				
				if( pJoinPlayer->nlife > 0 && !bFind )
				{
					CMover* pMover = prj.GetUserByID( pJoinPlayer->uidPlayer );
					if( IsValidObj( pMover ) )
					{
						pMover->m_nGuildCombatState		= 1;
						g_xWSUserManager->AddGuildCombatUserState( pMover );
					}
				}
			}
		}
	}
}
// ���� ����
void CGuildCombat::SetGuildCombatClose( BOOL bGM )
{
	if( m_nState == CLOSE_STATE )
		return;

	if( bGM )
	{
		m_nStopWar = 1;	
		return;
	}

	for( int nVeci = 0 ; nVeci < (int)( vecRequestRanking.size() ) ; ++nVeci )
	{
		if( nVeci >= m_nMaxGuild )
			break;
		
		__REQUESTGUILD RequestGuild = vecRequestRanking.at( nVeci );

		__GuildCombatMember* pGCMember = FindGuildCombatMember( RequestGuild.uidGuild );
		if( pGCMember != NULL )
		{
			for( int veci = 0 ; veci < (int)( pGCMember->vecGCSelectMember.size() ) ; ++veci )
			{
				__JOINPLAYER* pJoinPlayer = pGCMember->vecGCSelectMember.at( veci );
				CMover* pMover = prj.GetUserByID( pJoinPlayer->uidPlayer );
				if( IsValidObj( pMover ) )
				{
					pMover->m_nGuildCombatState = 0;
					g_xWSUserManager->AddGuildCombatUserState( pMover );
				}
			}
		}
	}

	// ��� ��Ȱ ����Ʈ ���� ȹ�� �ֱ�
	for( int nVeci = 0 ; nVeci < (int)( vecRequestRanking.size() ) ; ++nVeci )
	{
		if( nVeci >= m_nMaxGuild )
			break;
		
		__REQUESTGUILD RequestGuild = vecRequestRanking.at( nVeci );

		__GuildCombatMember* pGCMember = FindGuildCombatMember( RequestGuild.uidGuild );
		if( pGCMember != NULL )
		{
			int nRevivalPoint = 0;
			for( int veci = 0 ; veci < (int)( pGCMember->vecGCSelectMember.size() ) ; ++veci )
			{
				__JOINPLAYER* pJoinPlayer = pGCMember->vecGCSelectMember.at( veci );
				nRevivalPoint += pJoinPlayer->nlife;
			}
			pGCMember->nGuildPoint += nRevivalPoint;

			CGuild* pGuildMsg;
			CGuildMember*	pMember;
			FLWSUser*			pUsertmp;
			pGuildMsg = g_GuildMng.GetGuild( RequestGuild.uidGuild );
			if( pGuildMsg )
			{
				for( map<u_long, CGuildMember*>::iterator iGuild = pGuildMsg->m_mapPMember.begin();
				iGuild != pGuildMsg->m_mapPMember.end(); ++iGuild )
				{
					pMember		= iGuild->second;
					pUsertmp	= (FLWSUser*)prj.GetUserByID( pMember->m_idPlayer );
					if( IsValidObj( pUsertmp ) && pUsertmp->GetWorld() && pUsertmp->GetWorld()->GetID() == WI_WORLD_GUILDWAR )
						pUsertmp->AddDefinedCaption( TRUE, TID_GAME_GUILDCOMBAT_POINT_REVIVAL, "%d", nRevivalPoint );
				}
			}
		}
	}
	g_xWSUserManager->AddGCGuildPrecedence();
}
// ���� ������ �����
void CGuildCombat::SetGuildCombatCloseWait( BOOL bGM )
{
	if( m_nState == CLOSE_STATE )
		return;

	if( bGM )
	{
		m_nStopWar = 2;
		return;
	}
	else
	{
		GuildCombatResult();

		GuildCombatResultRanking();		//sun: 11, ������ Ĩ���� �� Ĩ�� ���� ���� �̿�
	}	
}
// ��û�� ����߿� �����Ҽ� �ִ� �������?
BOOL CGuildCombat::IsRequestWarGuild( u_long uidGuild, BOOL bAll )
{
	for( int veci = 0 ; veci < (int)( vecRequestRanking.size() ) ; ++veci )
	{
		if( bAll == FALSE )
		{
			if( m_nMaxGuild <= veci )
				break;
		}

		__REQUESTGUILD RequestGuild = vecRequestRanking.at( veci );
		if( uidGuild == RequestGuild.uidGuild )
			return TRUE;
	}
	return FALSE;
}

// ������ ��������?
BOOL CGuildCombat::IsSelectPlayer( FLWSUser* pUser )
{
	__GuildCombatMember* pGuildCombatMem;
	__JOINPLAYER* pJoinPlayer;
	__REQUESTGUILD RequestGuild;

	for( int nVeci = 0 ; nVeci < (int)( vecRequestRanking.size() ) ; ++nVeci )
	{
		if( nVeci >= m_nMaxGuild )
			break;

		RequestGuild = vecRequestRanking.at( nVeci );
		if( RequestGuild.uidGuild == pUser->m_idGuild )
		{
			pGuildCombatMem = FindGuildCombatMember( RequestGuild.uidGuild );
			if( pGuildCombatMem != NULL )
			{
				for( int veci = 0 ; veci < (int)( pGuildCombatMem->vecGCSelectMember.size() ) ; ++veci )
				{
					pJoinPlayer = pGuildCombatMem->vecGCSelectMember.at( veci );
					if( pJoinPlayer->uidPlayer == pUser->m_idPlayer )
					{
						if( pJoinPlayer->nlife > 0 )
							return TRUE;
						break;
					}
				}
			}
			break;
		}
	}
	return FALSE;
}

void CGuildCombat::Process()
{
#ifdef __WORLDSERVER
//	FLTRACE_LOG( PROGRAM_NAME, _T( "<%d>, <%d>" ), m_nState, m_nGuildCombatState);

	if( m_nState != CLOSE_STATE )
	{
		if( m_nProcessGo < m_nProcessCount[0] )
			ProcessCommand();
		ProcessJoinWar();	//
	}
	else
	{
		CTime ctime = CTime::GetCurrentTime();

		if( __AutoOpen[ctime.GetDayOfWeek()-1].bUseing )
		{
			// ��ŵ �� �� �����Ƿ� ������ �ʿ�� ��
			if( __AutoOpen[ctime.GetDayOfWeek()-1].nHour == ctime.GetHour() &&
				__AutoOpen[ctime.GetDayOfWeek()-1].nMinute == ctime.GetMinute() && m_bMutex == FALSE )
			{
				if( (int)( m_vecGuildCombatMem.size() ) >= m_nMinGuild )	//sun: 10, __S_BUG_GC
				{				
					GuildCombatOpen();
				}
				else
				{
					m_ctrMutexOut.Set( SEC(60) );
					m_bMutexMsg = TRUE;
					g_DPCoreClient.SendSystem( prj.GetText( TID_GAME_GUILDCOMBAT_NEXT_COMBAT ) );	
					g_DPCoreClient.SendSystem( prj.GetText( TID_GAME_GUILDCOMBAT_ENJOY ) );	
				}

				m_bMutex = TRUE;				
			}

			if( m_bMutexMsg && m_ctrMutexOut.IsTimeOut() )
			{
				m_bMutexMsg = FALSE;
				m_bMutex    = FALSE;
			}
		}
	}
#endif //__WORLDSERVER
}

void CGuildCombat::ProcessJoinWar()
{
	for( int nVeci = 0 ; nVeci < (int)( vecRequestRanking.size() ) ; ++nVeci )
	{
		if( nVeci >= m_nMaxGuild )
			break;
		
		__REQUESTGUILD RequestGuild = vecRequestRanking.at( nVeci );
//sun: 10, __S_BUG_GC
		__GuildCombatMember* pGCMember = FindGuildCombatMember( RequestGuild.uidGuild );
		if( pGCMember != NULL )
		{
			for( int veci = 0 ; veci < (int)( pGCMember->vecGCSelectMember.size() ) ; ++veci )
			{
				__JOINPLAYER* pJoinPlayer = pGCMember->vecGCSelectMember.at( veci );
				if( pJoinPlayer->dwTelTime != 0 )
				{
					if( pJoinPlayer->dwTelTime + m_nMaxMapTime * 1000 <= timeGetTime()  )
					{
						pJoinPlayer->dwTelTime = 0;
						CMover* pMover = prj.GetUserByID( pJoinPlayer->uidPlayer );					
						if( IsValidObj( pMover ) )
						{
							JoinWar( (FLWSUser*)pMover, pJoinPlayer->nMap );
						}
					}
				}
			}
		}
	}
}
void CGuildCombat::ProcessCommand()
{
	if( m_nStopWar == 1 )
	{
		m_nProcessGo = m_nProcessCount[COMPLET_CLOSE_STATE];
		m_dwTime = 0;
		m_nStopWar = 0;
	}
	else if( m_nStopWar == 2 )
	{
		m_nProcessGo = m_nProcessCount[GM_COLSE_STATE];
		m_dwTime = 0;
		m_nStopWar = 200;
	}
	if( m_dwTime <= GetTickCount() )
	{
		if( GuildCombatProcess[m_nProcessGo].nState == WARSTATE )
		{
			if( GuildCombatProcess[m_nProcessGo].dwCommand == NOTENTER_COUNT_STATE )
#ifdef __LAYER_1015
				g_xWSUserManager->ReplaceWorld( WI_WORLD_GUILDWAR, WI_WORLD_MADRIGAL, 6968.0f, 3328.8f, nDefaultLayer );
#else	// __LAYER_1015
				g_xWSUserManager->ReplaceWorld( WI_WORLD_GUILDWAR, WI_WORLD_MADRIGAL, 6968.0f, 3328.8f );
#endif	// __LAYER_1015
			else if( GuildCombatProcess[m_nProcessGo].dwCommand == MAINTENANCE_STATE )
			{
				// ���� �غ� ����
				SetMaintenance();
			}
			else if( GuildCombatProcess[m_nProcessGo].dwCommand == ENTER_STATE )
			{
				// ���ü� �ִ� ����
				SetEnter();
			}
			else if( GuildCombatProcess[m_nProcessGo].dwCommand == WAR_WAR_STATE )
			{
				// ���� ���� 
				SetGuildCombatStart();
			}
			else if( GuildCombatProcess[m_nProcessGo].dwCommand == WAR_CLOSE_STATE )
			{
				// ���� ����
				SetGuildCombatClose();
			}
			else if( GuildCombatProcess[m_nProcessGo].dwCommand == WAR_CLOSE_WAIT_STATE )
			{
				// ���� ������ ����� ������ ���
				SetGuildCombatCloseWait();
			}
			else if( GuildCombatProcess[m_nProcessGo].dwCommand == WAR_TELEPORT_STATE )
			{
				// ���� ������ �ڷ���Ʈ
				GuildCombatCloseTeleport();
			}
		}

		switch( GuildCombatProcess[m_nProcessGo].nState )
		{
		case ALLMSG:
			{
//sun: 11, �ϴ��� ��� ����
				if( GuildCombatProcess[m_nProcessGo].dwCommand != TID_GAME_GUILDCOMBAT_OPEN_MSG )
				{
					CString str;
					str.Format( prj.GetText( GuildCombatProcess[m_nProcessGo].dwCommand ), GuildCombatProcess[m_nProcessGo].dwTime / 1000 );
					g_DPCoreClient.SendSystem( str );
				}

				if( GuildCombatProcess[m_nProcessGo].dwCommand == TID_GAME_GUILDCOMBAT_OPEN_MSG )
					SendGuildCombatEnterTime( );
			}
			break;
		case GUILDMSG:
			{
			}
			break;
		case JOINMSG:
			{
// 				CString str;
// 				str.Format( prj.GetText( GuildCombatProcess[m_nProcessGo].dwCommand ), GuildCombatProcess[m_nProcessGo].dwTime / 1000 );
//				SendJoinMsg( str );
			}
			break;
		case WORLDMSG:
			{
				CString str;
				str.Format( prj.GetText( GuildCombatProcess[m_nProcessGo].dwCommand ), GuildCombatProcess[m_nProcessGo].dwTime / 1000 );
				g_xWSUserManager->AddWorldMsg( WI_WORLD_GUILDWAR, str );
			}
			break;
		case STATE:
			{
				if( g_eLocal.GetState( EVE_GUILDCOMBAT ) )
					g_DPCoreClient.SendGuildCombatState( GuildCombatProcess[m_nProcessGo].dwCommand );
//				m_nState = GuildCombatProcess[m_nProcessGo].dwCommand;
			}
			break;
		case WARSTATE:
			{
				m_nGCState = GuildCombatProcess[m_nProcessGo].dwCommand;
				g_xWSUserManager->AddGuildCombatState();
			}
			break;
		}
		m_dwTime = GetTickCount() + GuildCombatProcess[m_nProcessGo].dwTime;
		if( GuildCombatProcess[m_nProcessGo].dwParam != 0 )
			g_xWSUserManager->AddGuildCombatNextTimeWorld( GuildCombatProcess[m_nProcessGo].dwTime, GuildCombatProcess[m_nProcessGo].dwParam );
		++m_nProcessGo;
	}
}


BOOL CGuildCombat::LoadScript( LPCSTR lpszFileName )
{
	CScanner s;

	TCHAR szFullPath[ MAX_PATH ]	= { 0, };
	g_pScriptFileManager->GetScriptFileFullPath( szFullPath, _countof( szFullPath ), lpszFileName );
	
	if( !s.Load( szFullPath ) )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "[ FAILED LOAD SCRIPT. FILE_NAME:(%s) ]" ), szFullPath );
		return FALSE;
	}

	ZeroMemory( m_nProcessCount, sizeof( int ) * 25 );
	ZeroMemory( GuildCombatProcess, sizeof( __GuildCombatProcess ) * 250 );
	int		nCount;
	int		nAutoOpenCnt = 0;

	while( s.tok != FINISHED )
	{
		nCount = -1;

#if defined(__MAINSERVER)
		if( s.Token == _T( "AUTO_OPEN_IDC" ) )
		{
			s.GetToken();	// {
			s.GetToken();	// {
			
			int nDay, nTime1, nTime2;
			while( *s.token != '}' )
			{
				if( nAutoOpenCnt >= 7 )
				{
					FLERROR_LOG( PROGRAM_NAME, _T( "�ڵ� ���� ���� ������ ����!!" ) );
					return FALSE;
				}

				nDay = atoi( s.Token );
				m_nDay = nDay;
				if( nDay <= 0 || nDay > 7 )
				{
					FLERROR_LOG( PROGRAM_NAME, _T( "Day Error = %d" ), nDay );
					return FALSE;
				}
				__AutoOpen[nDay-1].bUseing = TRUE;

				nTime1 = s.GetNumber();
				if( nTime1 < 0 || nTime1 > 24 )
				{
					FLERROR_LOG( PROGRAM_NAME, _T( "Time Error = %d" ), nTime1 );
					return FALSE;
				}
				__AutoOpen[nDay-1].nHour = nTime1;
				
				nTime2 = s.GetNumber();
				if( nTime2 < 0 || nTime2 > 59 )
				{
					FLERROR_LOG( PROGRAM_NAME, _T( "Time Error = %d" ), nTime2 );
					return FALSE;
				}
				__AutoOpen[nDay-1].nMinute = nTime2;

				nAutoOpenCnt++;

				s.GetToken();
			}		
		}
#else
		if( s.Token == _T( "AUTO_OPEN_TEST" ) )
		{
			s.GetToken();	// {
			s.GetToken();	// {
			
			int nDay, nTime1, nTime2;
			while( *s.token != '}' )
			{
				if( nAutoOpenCnt >= 7 )
				{
					FLERROR_LOG( PROGRAM_NAME, _T( "�ڵ� ���� ���� ������ ����!!" ) );
					return FALSE;
				}
				
				nDay = atoi( s.Token );
				m_nDay = nDay;
				if( nDay <= 0 || nDay > 7 )
				{
					FLERROR_LOG( PROGRAM_NAME, _T( "Day Error = %d" ), nDay );
					return FALSE;
				}
				__AutoOpen[nDay-1].bUseing = TRUE;
				
				nTime1 = s.GetNumber();
				if( nTime1 < 0 || nTime1 > 24 )
				{
					FLERROR_LOG( PROGRAM_NAME, _T( "Time Error = %d" ), nTime1 );
					return FALSE;
				}
				__AutoOpen[nDay-1].nHour = static_cast< BYTE >( nTime1 );
				
				nTime2 = s.GetNumber();
				if( nTime2 < 0 || nTime2 > 59 )
				{
					FLERROR_LOG( PROGRAM_NAME, _T( "Time Error = %d" ), nTime2 );
					return FALSE;
				}
				__AutoOpen[nDay-1].nMinute = static_cast< BYTE >( nTime2 );
				
				nAutoOpenCnt++;
				
				s.GetToken();
			}		
		}
#endif
		else if( s.Token == _T( "JOINPENYA" ) )
		{
			m_nJoinPanya = s.GetNumber();
		}
		else if( s.Token == _T( "GUILDLEVEL" ) )
		{
			m_nGuildLevel = s.GetNumber();
		}
//sun: 8, // __GUILDCOMBAT_85
		else if( s.Token == _T( "MINJOINGUILDSIZE" ) )
		{
			m_nMinGuild = s.GetNumber();
		}

		else if( s.Token == _T( "MAXJOINGUILDSIZE" ) )
		{
			m_nMaxGuild = s.GetNumber();
		}
		else if( s.Token == _T( "MAXPLAYERLIFE" ) )
		{
			m_nMaxPlayerLife = s.GetNumber();
		}
		else if( s.Token == _T( "MAXWARPLAYER" ) )
		{
			m_nMaxWarPlayer = s.GetNumber();
		}
		else if( s.Token == _T( "MAXMAPTIME" ) )
		{
			m_nMaxMapTime = s.GetNumber();
		}
		else if( s.Token == _T( "MAXJOINMEMBERSIZE" ) )
		{
			m_nMaxJoinMember = s.GetNumber();
		}
		else if( s.Token == _T( "MAXGUILDPERCENT" ) )
		{
			m_nMaxGuildPercent = s.GetNumber();
		}
		else if( s.Token == _T( "MAXPLAYERPERCENT" ) )
		{
			m_nMaxPlayerPercent = s.GetNumber();
		}
		else if( s.Token == _T( "REQUESTCANCLEPERCENT" ) )
		{
			m_nRequestCanclePercent = s.GetNumber();
		}
		else if( s.Token == _T( "NOTREQUESTPERCENT" ) )
		{
			m_nNotRequestPercent = s.GetNumber();
		}
		else if( s.Token == _T( "ITEMPENYA" ) )
		{
			m_nItemPenya = s.GetNumber();
		}
//sun: 8, // __GUILDCOMBAT_85
		else if( s.Token == _T( "MAX_GCSIENDITEM" ) )
		{
			m_nMaxGCSendItem = s.GetNumber();
		}
		else if( s.Token == _T( "GCSENDITEM" ) )
		{
			int nWinCount = s.GetNumber();
			s.GetToken();
			DWORD dwItemId = CScript::GetDefineNum( s.Token );
			if( dwItemId == -1 )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "�������� ���°��� ��������" ) );
				return FALSE;
			}
			int nItemNum = s.GetNumber();
			__GCSENDITEM GCSendItem;
			GCSendItem.nWinCount = nWinCount;
			GCSendItem.dwItemId = dwItemId;
			GCSendItem.nItemNum = nItemNum;
			vecGCSendItem.push_back( GCSendItem );
		}
		else if( s.Token == _T( "REWARD_ITEM" ) )
		{
			//s.GetToken();
			__GCREWARDITEM rewardItem;
			rewardItem.m_dwRank = s.GetNumber();
			rewardItem.m_nWinCount  = s.GetNumber();
			s.GetToken();
			rewardItem.m_dwRewardItemID	= static_cast<DWORD>( CScript::GetDefineNum( s.Token ) );
			const T_ITEM_SPEC* pITemSpec	= g_xSpecManager->GetSpecItem( rewardItem.m_dwRewardItemID );
			if( pITemSpec == NULL )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID REWARD ITEM ID. ITEM_ID(%u) ]" ), rewardItem.m_dwRewardItemID );
				//RELEASE_BREAK;
				return FALSE;
			}
			//s.GetToken();
			rewardItem.m_nItemNum = s.GetNumber();
			m_vecRewardItem.push_back(rewardItem);

			//SetRewardItemID( dwRewardItemID );
		}

		else if( s.Token == _T( "OPEN" ) )
		{
			nCount = 0;
		}
		else if( s.Token == _T( "MAINTENANCE" ) )
		{
			nCount = 1;
		}
		else if( s.Token == _T( "WAR" ) )
		{
			nCount = 2;
		}

		if( nCount != -1 )
		{
			m_nProcessCount[nCount] = s.GetNumber();
			s.GetToken();	// {
			s.GetToken();	// {
			int nSoCount = 0;
			while( *s.token != '}' )
			{
				if( s.Token == _T( "ALLMSG" ) )
				{
					GuildCombatProcess[25*nCount+nSoCount].nState	= ALLMSG;
				}
				else if( s.Token == _T( "GUILDMSG" ) )
				{
					GuildCombatProcess[25*nCount+nSoCount].nState	= GUILDMSG;
				}
				else if( s.Token == _T( "JOINMSG" ) )
				{
					GuildCombatProcess[25*nCount+nSoCount].nState	= JOINMSG;
				}
				else if( s.Token == _T( "WORLDMSG" ) )
				{
					GuildCombatProcess[25*nCount+nSoCount].nState	= WORLDMSG;
				}
				else if( s.Token == _T( "STATE" ) )
				{
					GuildCombatProcess[25*nCount+nSoCount].nState	= STATE;
				}
				else if( s.Token == _T( "WARSTATE" ) )
				{
					GuildCombatProcess[25*nCount+nSoCount].nState	= WARSTATE;
				}
				else if( s.Token == _T( "WAIT" ) )
				{
					GuildCombatProcess[25*nCount+nSoCount].nState	= WAIT;
				}
				GuildCombatProcess[25*nCount+nSoCount].dwTime	= s.GetNumber();
				GuildCombatProcess[25*nCount+nSoCount].dwCommand	= s.GetNumber();
				GuildCombatProcess[25*nCount+nSoCount].dwParam		= s.GetNumber();
				if( GuildCombatProcess[25*nCount+nSoCount].nState == STATE )
					m_nProcessCount[GuildCombatProcess[25*nCount+nSoCount].dwCommand] = 25*nCount+nSoCount;
				
				++nSoCount;
				s.GetToken();
			}
		}

		s.GetToken();
	}

	if( CheckScriptData() == false )
	{
		return FALSE;
	}

	return TRUE;
}

bool	CGuildCombat::CheckScriptData() const
{
	/*const DWORD dwRewardItemID		= GetRewardItemID();
	const T_ITEM_SPEC* pItemSpec	= g_xSpecManager->GetSpecItem( dwRewardItemID );
	if( pItemSpec == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID REWARD ITEM ID. ITEM_ID(%u) ]" ), dwRewardItemID );
		RELEASE_BREAK;
		return false;
	}*/

	for ( int i = 0; i < (int)m_vecRewardItem.size(); i++ )
	{
		const DWORD dwRewardItemID		= m_vecRewardItem[i].m_dwRewardItemID;
		const T_ITEM_SPEC* pItemSpec	= g_xSpecManager->GetSpecItem( dwRewardItemID );
		if( pItemSpec == NULL )
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID REWARD ITEM ID. ITEM_ID(%u) ]" ), dwRewardItemID );
			RELEASE_BREAK;
			return false;
		}
	}

	return true;
}

void CGuildCombat::SendGCLog( void )
{
	for( int nVeci = 0 ; nVeci < (int)( vecRequestRanking.size() ) ; ++nVeci )
	{
		if( nVeci >= m_nMaxGuild )
			break;
		
		__REQUESTGUILD RequestGuild = vecRequestRanking.at( nVeci );
//sun: 10, __S_BUG_GC
		__GuildCombatMember* pGCMember = FindGuildCombatMember( RequestGuild.uidGuild );
		if( pGCMember != NULL )
		{
//			int nPoint = 0;
			for( int veci = 0 ; veci < (int)( pGCMember->vecGCSelectMember.size() ) ; ++veci )
			{
				__JOINPLAYER* pJoinPlayer = pGCMember->vecGCSelectMember.at( veci );
				FLWSUser* pUser = (FLWSUser*)prj.GetUserByID( pJoinPlayer->uidPlayer );
				if( IsValidObj( pUser ) )
					pUser->AddGCLog();
			}
		}
	}
}

void CGuildCombat::SendJoinMsg( LPCTSTR /*lpszString*/  )
{

}

void CGuildCombat::SendGuildCombatEnterTime( void )
{
	g_xWSUserManager->AddGuildCombatEnterTime( GuildCombatProcess[m_nProcessGo].dwTime );
/*	for( map<u_long, __GuildCombatMember*>::iterator ita = m_GuildCombatMem.begin() ; ita != m_GuildCombatMem.end() ; ++ita )
	{
		CGuild* pGuild = g_GuildMng.GetGuild( ita->first );
		if( pGuild )
		{
			CGuildMember*	pMember;
			FLWSUser*			pUsertmp;
			for( map<u_long, CGuildMember*>::iterator i = pGuild->m_mapPMember.begin();
			i != pGuild->m_mapPMember.end(); ++i )
			{
				pMember		= i->second;
				pUsertmp	= (FLWSUser*)prj.GetUserByID( pMember->m_idPlayer );
				if( IsValidObj( pUsertmp ) )
				{
					pUsertmp->AddGuildCombatEnterTime( GuildCombatProcess[m_nProcessGo].dwTime );
				}
			}
		}
	}
*/
}

void CGuildCombat::SetPlayerChange( FLWSUser* pUser, FLWSUser* /*pLeader*/ )
{
//sun: 10, __S_BUG_GC
	__GuildCombatMember* pGCMember = FindGuildCombatMember( pUser->m_idGuild );
	if( pGCMember != NULL )
	{
		for( int i = 0; i < (int)( pGCMember->vecGCSelectMember.size() ) ; ++i )
		{
			__JOINPLAYER* pJoinPlayer	= pGCMember->vecGCSelectMember.at( i );
			if( pJoinPlayer->uidPlayer == pUser->m_idPlayer )
			{
				// ��⿭�� ���� �ϴ�  pUser�� ������ �����ϹǷ� ����
				for( list<__JOINPLAYER*>::iterator i1 = pGCMember->lspFifo.begin(); i1 != pGCMember->lspFifo.end(); ++i1 )
				{
					if( pJoinPlayer == *i1 )
						return;
				}
//				pJoinPlayer->bEntry	= FALSE;
				pJoinPlayer->dwTelTime	= 0;
				pUser->m_nGuildCombatState	= 0;
				g_xWSUserManager->AddGuildCombatUserState( pUser );
				--pGCMember->nWarCount;
				pJoinPlayer->nlife--;
				if( pJoinPlayer->nlife > 0 )
					pGCMember->lspFifo.push_back( pJoinPlayer );
				if( pJoinPlayer->nlife < 0 )
					pJoinPlayer->nlife	= 0;

				while( pGCMember->lspFifo.empty() == false )
				{
					__JOINPLAYER* pJoinPlayer	= pGCMember->lspFifo.front();
					CMover* pMover	= prj.GetUserByID( pJoinPlayer->uidPlayer );
					if( IsValidObj( pMover ) && pMover->GetWorld()->GetID() == WI_WORLD_GUILDWAR )
					{
						pGCMember->nWarCount++;
//						pJoinPlayer->bEntry	= TRUE;
						pJoinPlayer->dwTelTime	= timeGetTime();
						((FLWSUser*)pMover)->AddGCJoinWarWindow( pJoinPlayer->nMap, m_nMaxMapTime );
						pGCMember->lspFifo.pop_front();
					}
					else
					{
						pGCMember->lspFifo.pop_front();
						pJoinPlayer->nlife--;
						if( pJoinPlayer->nlife > 0 )
							pGCMember->lspFifo.push_back( pJoinPlayer );
						if( pJoinPlayer->nlife < 0 )
							pJoinPlayer->nlife	= 0;
					}
					if( pGCMember->nWarCount == m_nMaxWarPlayer )
						break;
				}	// while
				break;
			}	// if
		}	// for
	}	// if
}
void CGuildCombat::GetPoint( FLWSUser* pAttacker, FLWSUser* pDefender )
{
	// ������ �ѵ� ������ ����
	BOOL bMaster = FALSE;
	BOOL bDefender = FALSE;
	BOOL bLastLife = FALSE;
	BOOL bKillDiffernceGuild = FALSE;
	int nGetPoint = 2;
	__JOINPLAYER* pJoinPlayerAttacker = NULL;
	__JOINPLAYER* pJoinPlayerDefender = NULL;
	__GuildCombatMember* pGCMemberAttacker = NULL;
	CGuild* pGuild	= g_GuildMng.GetGuild( pAttacker->m_idGuild );
	if( pGuild && pGuild->IsMaster( pAttacker->m_idPlayer ) )
		bMaster = TRUE;

//sun: 10, __S_BUG_GC
	pGCMemberAttacker = FindGuildCombatMember( pAttacker->m_idGuild );
	if( pGCMemberAttacker != NULL )
	{
		for( int veci = 0 ; veci < (int)( pGCMemberAttacker->vecGCSelectMember.size() ) ; ++veci )
		{
			__JOINPLAYER* pJoinPlayer = pGCMemberAttacker->vecGCSelectMember.at( veci );
			if( pJoinPlayer->uidPlayer == pAttacker->m_idPlayer )
			{
				pJoinPlayerAttacker = pJoinPlayer;
				break;
			}
		}
		if( pJoinPlayerAttacker == NULL )
			return;

		if( pJoinPlayerAttacker->uKillidGuild != 0 && static_cast< u_long >( pJoinPlayerAttacker->uKillidGuild ) != pDefender->m_idGuild )
			bKillDiffernceGuild = TRUE;

		if( pJoinPlayerAttacker->nlife == 1 )
			bLastLife = TRUE;
	}

	if( pGCMemberAttacker == NULL )
		return;

//sun: 10, __S_BUG_GC
	__GuildCombatMember* pGCMember = FindGuildCombatMember( pDefender->m_idGuild );
	if( pGCMember != NULL )
	{
		for( int veci = 0 ; veci < (int)( pGCMember->vecGCSelectMember.size() ) ; ++veci )
		{
			__JOINPLAYER* pJoinPlayer = pGCMember->vecGCSelectMember.at( veci );
			if( pJoinPlayer->uidPlayer == pDefender->m_idPlayer )
			{
				pJoinPlayerDefender = pJoinPlayer;
				break;
			}
		}
		if( pJoinPlayerDefender == NULL )
			return;

		if( pDefender->m_idPlayer == pGCMember->m_uidDefender )
			bDefender = TRUE;
	}	

	// ������ �ٸ� ������ Kill ���� ���
	if( bKillDiffernceGuild )
		++nGetPoint;
	// ��帶���Ͱ� �� ������ Kill ���� ���
	if( bMaster )
		++nGetPoint;
	// ������� Kill ���� ���
	if( bDefender )
		++nGetPoint;
	// ��Ȱ����Ʈ 0�� �� Kill ���� ���
	if( bLastLife )
		++nGetPoint;

	pJoinPlayerAttacker->nPoint += nGetPoint;
	pGCMemberAttacker->nGuildPoint += nGetPoint;
	
	CGuild* pGuildMsg;
	CGuildMember*	pMember;
	FLWSUser*			pUsertmp;

	pGuildMsg = pAttacker->GetGuild();
	if( pGuildMsg )
	{
		for( map<u_long, CGuildMember*>::iterator i = pGuildMsg->m_mapPMember.begin();
		i != pGuildMsg->m_mapPMember.end(); ++i )
		{
			pMember		= i->second;
			pUsertmp	= (FLWSUser*)prj.GetUserByID( pMember->m_idPlayer );
			if( IsValidObj( pUsertmp ) && pUsertmp->GetWorld() && pUsertmp->GetWorld()->GetID() == WI_WORLD_GUILDWAR )
			{
				if( bMaster )
					pUsertmp->AddDefinedCaption( TRUE, TID_GAME_GUILDCOMBAT_POINT_MASTER, "%d", nGetPoint );
				else 
					pUsertmp->AddDefinedCaption( TRUE, TID_GAME_GUILDCOMBAT_POINT_GENERAL, "%d", nGetPoint );
			}
		}
	}

	if( bDefender )
	{
		pGuildMsg = pDefender->GetGuild();
		if( pGuildMsg )
		{
			for( map<u_long, CGuildMember*>::iterator i = pGuildMsg->m_mapPMember.begin();
			i != pGuildMsg->m_mapPMember.end(); ++i )
			{
				pMember		= i->second;
				pUsertmp	= (FLWSUser*)prj.GetUserByID( pMember->m_idPlayer );
				if( IsValidObj( pUsertmp ) && pUsertmp->GetWorld() && pUsertmp->GetWorld()->GetID() == WI_WORLD_GUILDWAR )
				{
					if( pGuild )
						pUsertmp->AddDefinedCaption( TRUE, TID_GAME_GUILDCOMBAT_POINT_DEFENDER, "\"%s\"", pGuild->m_szGuild );	
				}
			}
		}
	}
	AddvecGCGetPoint( pAttacker->m_idGuild, pDefender->m_idGuild, pAttacker->m_idPlayer, pDefender->m_idPlayer, nGetPoint, bKillDiffernceGuild, bMaster, bDefender, bLastLife );
	g_xWSUserManager->AddGCGuildPrecedence();
	g_xWSUserManager->AddGCPlayerPrecedence();
}

void CGuildCombat::SerializeGCWarPlayerList( CAr & ar )
{
	if( m_nMaxGuild <= (int)( vecRequestRanking.size() ) )
		ar << m_nMaxGuild;
	else
		ar << (int)vecRequestRanking.size();

	for( int nVeci = 0 ; nVeci < (int)( vecRequestRanking.size() ) ; ++nVeci )
	{
		if( nVeci >= m_nMaxGuild )
			break;
		
		__REQUESTGUILD RequestGuild = vecRequestRanking.at( nVeci );
//sun: 10, __S_BUG_GC
		__GuildCombatMember* pGCMember = FindGuildCombatMember( RequestGuild.uidGuild );
		if( pGCMember != NULL )
		{
			__JOINPLAYER* pJoinPlayer1;

			ar << pGCMember->m_uidDefender;
			ar << (int)pGCMember->vecGCSelectMember.size();
			for( int vecSi = 0 ; vecSi < (int)( pGCMember->vecGCSelectMember.size() ) ; ++vecSi )
			{
				pJoinPlayer1 = pGCMember->vecGCSelectMember.at( vecSi );

				ar << pJoinPlayer1->uidPlayer;
				if( 0 < pJoinPlayer1->nlife )
				{
					BOOL bFind	= FALSE;
					for( list<__JOINPLAYER*>::iterator i1 = pGCMember->lspFifo.begin(); i1 != pGCMember->lspFifo.end(); ++i1 )
					{
						if( pJoinPlayer1 == *i1 )
						{
							bFind	= TRUE;
							break;
						}
					}
				
					if( bFind )	// �����
					{
						ar << (int)0;
					}
					else // ������
					{
						ar << (int)1;
					}
				}
				else // �����
				{
					ar << (int)0;
				}
			}
		}
		else
		{
			ar << (u_long)0;
			ar << (int)0;
		}
	}
}
#endif	// __WORLDSERVER

CGuildCombat	g_GuildCombatMng;
