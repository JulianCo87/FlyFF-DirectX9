#include "stdafx.h"

#include ".\minigamegawibawibo.h"

CMiniGameGawibawibo::CMiniGameGawibawibo(void)
: m_nWinProb( 33 ), m_nDrawProb( 34 ), m_nLostProb( 33 ), m_nWinCount( 0 )
{

}

CMiniGameGawibawibo::CMiniGameGawibawibo( CMiniGameBase* pMiniGame )
: m_nWinCount( 0 )
{
	CMiniGameGawibawibo* pMiniGameGawibawibo = static_cast<CMiniGameGawibawibo*>( pMiniGame );
	
	m_nWinProb = pMiniGameGawibawibo->m_nWinProb;
	m_nDrawProb = pMiniGameGawibawibo->m_nDrawProb;
	m_nLostProb = pMiniGameGawibawibo->m_nLostProb;
}

CMiniGameGawibawibo::~CMiniGameGawibawibo(void)
{
}

BOOL CMiniGameGawibawibo::Excute( FLWSUser* pUser, __MINIGAME_PACKET* pMiniGamePacket )
{
	if( CNpcChecker::GetInstance()->IsCloseNpc( MMI_LORD_RAINBOW_KAWIBAWIBO, pUser->GetWorld(), pUser->GetPos() ) == FALSE ) {
		return FALSE;
	}

	BOOL bReturn = FALSE;		// 미니게임 완료 상태

	__MINIGAME_PACKET MP( pMiniGamePacket->wNowGame );
	if( pMiniGamePacket->nState == MP_OPENWND )
	{
		MP.nState = MP_OPENWND;
		m_nWinCount = 0;	// 창을 새로 열면 연승 초기화..
		MP.nParam1 = m_nWinCount;
		SendPacket( pUser, MP );
		return bReturn;
	}

	int nRandom = xRandom( 100 );
	if( nRandom < m_nWinProb )
	{
		MP.nState = MP_TRUE;
		m_nWinCount++;		// 연승 추가
	}
	else if( nRandom < m_nWinProb + m_nDrawProb )
		MP.nState = MP_FALSE;
	else
	{
		MP.nState = MP_FAIL;
		m_nWinCount = 0;	// 연승 초기화
	}

	MP.nParam1 = m_nWinCount;
	if( m_nWinCount == 3 )	// 3연승이면 완료...
	{
		MP.nState = MP_FINISH;
		bReturn = TRUE;
	}

	SendPacket( pUser, MP );    
	return bReturn;
}

