#include "stdafx.h"

#include ".\minigamepairgame.h"

CMiniGamePairgame::CMiniGamePairgame(void)
: m_nCorrectCount( 0 )
{
}

CMiniGamePairgame::CMiniGamePairgame( CMiniGameBase* /*pMiniGame*/ ) 
: m_nCorrectCount( 0 )
{
	//CMiniGamePairgame* pMiniGamePairgame = static_cast<CMiniGamePairgame*>( pMiniGame );
}

CMiniGamePairgame::~CMiniGamePairgame(void)
{

}

BOOL CMiniGamePairgame::Excute( FLWSUser* pUser, __MINIGAME_PACKET* pMiniGamePacket )
{
	if( CNpcChecker::GetInstance()->IsCloseNpc( MMI_LORD_RAINBOW_CARD, pUser->GetWorld(), pUser->GetPos() ) == FALSE ) {
		return FALSE;
	}

	BOOL bReturn = FALSE;
	if( pMiniGamePacket->nState == MP_OPENWND )
	{
		__MINIGAME_EXT_PACKET MP( pMiniGamePacket->wNowGame );	
		MP.nState = MP_OPENWND;
		m_nCorrectCount = 0;
		MP.vecszData.push_back( MakeGame() );
		SendExtPacket( pUser, MP );
		return bReturn;
	}

	__MINIGAME_PACKET MP( pMiniGamePacket->wNowGame );	

	int nPos_1 = pMiniGamePacket->nParam1;
	int nPos_2 = pMiniGamePacket->nParam2;

	MP.nState = MP_FALSE;
	if( nPos_1 >= 0  && nPos_1 < (int)( m_strCardStream.length() ) && nPos_2 >= 0 && nPos_2 < (int)( m_strCardStream.length() ) )
	{
		MP.nState = MP_FALSE;
		if( (nPos_1 != nPos_2) && (m_strCardStream[nPos_1] != 'x') && (m_strCardStream[nPos_2] != 'x') )
		{
			if( m_strCardStream[nPos_1] == m_strCardStream[nPos_2] )
			{
				m_nCorrectCount++;
				m_strCardStream[nPos_1] = m_strCardStream[nPos_2] = 'x';
				MP.nState = MP_TRUE;
				if( m_nCorrectCount == 9 )	// 9쌍을 성공하면 완료!!
				{
					MP.nState = MP_FINISH;
					bReturn = TRUE;
				}
			}
		}
	}

	SendPacket( pUser, MP );
	return bReturn;
}



struct PAIR
{
	char card;
	BYTE nSize;
	PAIR( char c ) : card(c), nSize(2) {}
};

string CMiniGamePairgame::MakeGame()
{
	vector<PAIR> vecTemp;
	for( char c=1; c<10; c++ )			// 9종류 카드 생성
		vecTemp.push_back( PAIR(c) );

	m_strCardStream.clear();
	for( ; vecTemp.size() > 0; )
	{
		vector<PAIR>::iterator it = vecTemp.begin();
		int nNum = xRandom( vecTemp.size() );
		it += nNum;
		m_strCardStream += (*it).card;
		(*it).nSize--;
		if( (*it).nSize == 0 )
			vecTemp.erase( it );
	}

	return m_strCardStream;
}
