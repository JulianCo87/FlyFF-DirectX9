#include "stdafx.h"

#include ".\minigametyping.h"

CMiniGameTyping::CMiniGameTyping(void)
{
}

CMiniGameTyping::CMiniGameTyping( CMiniGameBase* /*pMiniGame*/ )
{
	//CMiniGameTyping* pMiniGameTyping = static_cast<CMiniGameTyping*>( pMiniGame );
}

CMiniGameTyping::~CMiniGameTyping(void)
{
}

BOOL CMiniGameTyping::Excute( FLWSUser* pUser, __MINIGAME_PACKET* pMiniGamePacket )
{
	if( CNpcChecker::GetInstance()->IsCloseNpc( MMI_LORD_RAINBOW_TYPING, pUser->GetWorld(), pUser->GetPos() ) == FALSE ) {
		return FALSE;
	}

	BOOL bReturn = FALSE;

	__MINIGAME_EXT_PACKET MP( pMiniGamePacket->wNowGame );	
	if( pMiniGamePacket->nState == MP_OPENWND )
	{
		MP.nState = MP_OPENWND;
		MakeQuestion();
		MP.vecszData.assign( m_vecTyping.begin(), m_vecTyping.end() );
		SendExtPacket( pUser, MP );
		return bReturn;
	}

	__MINIGAME_EXT_PACKET* pPacket = static_cast<__MINIGAME_EXT_PACKET*>( pMiniGamePacket );

	MP.nState = MP_FINISH;
	bReturn = TRUE;
	for( int i=0; i<(int)( m_vecTyping.size() ); i++ )
	{
		if( m_vecTyping.at( i ) != pPacket->vecszData.at( i ) )
		{
			MP.nState = MP_FAIL;
			bReturn = FALSE;
			break;
		}
	}

	SendExtPacket( pUser, MP );
	return bReturn;
}

void CMiniGameTyping::MakeQuestion()
{
	m_vecTyping.clear();
	for( int i=0; i<3; i++ )
	{
		string strTemp;
		for( int j=0; j<20; j++ )
		{
			switch( xRandom( 3 ) )
			{
				case 0:	strTemp += static_cast<char>( xRandom( 48, 58 ) );	break;	// 숫자 0 ~ 9
				case 1:	strTemp += static_cast<char>( xRandom( 97, 110 ) );	break;	// 영문 소문 a ~ m
				case 2:	strTemp += static_cast<char>( xRandom( 110, 123 ) );	break;	// 영문 소문 n ~ z
				//case 2:	strTemp += static_cast<char>( xRandom( 65, 91 ) );	break;	// 영문 대문자
			}
		}
		m_vecTyping.push_back( strTemp );
	}
}
