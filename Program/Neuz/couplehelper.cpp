#include "stdafx.h"


#include "couplehelper.h"
#include "playerdata.h"

CCoupleHelper::CCoupleHelper()
:
m_pCouple( NULL )
{
}

CCoupleHelper::~CCoupleHelper()
{
	Clear();
}

void CCoupleHelper::Clear()
{
	SAFE_DELETE( m_pCouple );
}

CCoupleHelper* CCoupleHelper::Instance()
{
	static CCoupleHelper sCoupleHelper;
	return &sCoupleHelper;
}

BOOL CCoupleHelper::Initialize()
{
	return CCoupleProperty::Instance()->Initialize();
}

void CCoupleHelper::OnCouple( CAr & ar )
{
	FLTRACE_LOG( PROGRAM_NAME, _T( "C: CCoupleHelper.OnCouple" ) );
	SAFE_DELETE( m_pCouple );
	m_pCouple	= new CCouple;
	m_pCouple->Serialize( ar );
}

void CCoupleHelper::OnProposeResult( CAr & ar )
{
	u_long idProposer;
	char szProposer[MAX_PLAYER]	= { 0,};
	ar >> idProposer;
	ar.ReadString( szProposer, _countof( szProposer ) );
	FLTRACE_LOG( PROGRAM_NAME, _T( "C: CCoupleHelper.OnProposeResult: %d, %s" ), idProposer, szProposer );
	char szText[200]	= { 0,};
	FLSPrintf( szText, _countof( szText ), prj.GetText( TID_GAME_RECEIVEPROPOSAL ), szProposer );
	CPlayerDataCenter::GetInstance()->GetPlayerString( idProposer );
	FLTRACE_LOG( PROGRAM_NAME, szText );

	// 프러포즈 받은 메세지 창 출력
	if(g_WndMng.m_pWndCoupleMessage)
		SAFE_DELETE(g_WndMng.m_pWndCoupleMessage);

	g_WndMng.m_pWndCoupleMessage = new CWndCoupleMessage;
	if(g_WndMng.m_pWndCoupleMessage)
	{
		g_WndMng.m_pWndCoupleMessage->SetMessageMod(szText, CWndCoupleMessage::CM_RECEIVEPROPOSE);
		g_WndMng.m_pWndCoupleMessage->Initialize(NULL);
	}
}

void CCoupleHelper::OnCoupleResult( CAr & ar )
{
	if( !g_pPlayer )	return;
	u_long idPartner;
	char szPartner[MAX_PLAYER]	= { 0,};
	ar >> idPartner;
	ar.ReadString( szPartner, _countof( szPartner ) );
	FLTRACE_LOG( PROGRAM_NAME, _T( "C: CCoupleHelper.OnCoupleResult: %d, %s" ), idPartner, szPartner );
	SAFE_DELETE( m_pCouple );
	m_pCouple	= new CCouple( g_pPlayer->m_idPlayer, idPartner );
	CPlayerDataCenter::GetInstance()->GetPlayerString( idPartner );		//
	char szText[200]	= { 0,};
	FLSPrintf( szText, _countof( szText ), prj.GetText( TID_GAME_COUPLE_S02 ), szPartner );
	g_WndMng.PutString( szText, NULL, prj.GetTextColor( TID_GAME_COUPLE_S02 ) );		// %s님과 커플이 되었습니다.
}

void CCoupleHelper::OnDecoupleResult()
{
	FLTRACE_LOG( PROGRAM_NAME, _T( "C: CCoupleHelper.OnDecoupleResult" ) );
	CCouple* pCouple	= GetCouple();
	if( pCouple )
	{
		Clear();
		g_WndMng.PutString( prj.GetText( TID_GAME_COUPLE_S03 ), NULL, prj.GetTextColor( TID_GAME_COUPLE_S03 ) );		// %s님과 커플이 되었습니다.
	}
}

//sun: 13, 커플 보상
void CCoupleHelper::OnAddCoupleExperience( CAr & ar )
{
	int nExperience;
	ar >> nExperience;
	CCouple* pCouple	= GetCouple();
	if( pCouple )
	{
		BOOL bLevelup	= pCouple->AddExperience( nExperience );
		if( bLevelup )
		{
			char szString[200]	= { 0,};
			FLSPrintf( szString, _countof( szString ), prj.GetText( TID_GAME_COUPLE_LEVELUP ), pCouple->GetLevel() );
			g_WndMng.PutString( szString, NULL, prj.GetTextColor( TID_GAME_COUPLE_LEVELUP ) );		// 커플이 %d레벨이 되었습니다.
		}
	}
}

