
#include "StdAfx.h"
#include <cassert>

//date : 20101108
//author : gmpbigsun

#include "FLContentSwitch.h"

BOOL FLContentData::IsValidUserKey( ContentType eType )
{
	return ( m_cUserData.find( eType ) != m_cUserData.end() );
}

BOOL FLContentData::SetupAllUserState( ContentState eState )
{
	//std::fill( m_cUserData.begin(), m_cUserData.end(), eState );
	for( CSIter iter = m_cUserData.begin(); iter != m_cUserData.end(); ++iter )
		iter->second = eState;

	return TRUE;
}

BOOL FLContentData::RegisterBaseContent( ContentType eType, DWORD dwBitCombination, ContentState eDefaultType )
{
	if( (dwBitCombination & 0xFFFF0000) || ( eDefaultType & 0xFFFF0000 ) )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "bit:%x defaultype:%x" ), dwBitCombination, eDefaultType );
		return FALSE;
	}

	dwBitCombination |= ( eDefaultType << 16 );

	pair< CSIter, bool > rst = m_cBaseData.insert( CSContainer::value_type( eType, dwBitCombination ) );
	if( false == rst.second )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "already exist BaseContent key %d" ), eType );
		//m_cBaseData[ eType ] = dwBitComposion;

		return FALSE;
	}

	return TRUE;
}

BOOL FLContentData::RegisterUserContent( ContentType eType, ContentState eState )
{
	CSIter finder = m_cBaseData.find( eType );
	if( finder != m_cBaseData.end() )
	{
		DWORD dwBaseState = finder->second;
		DWORD dwTargetState = eState;

		if( 0 == ( dwBaseState & dwTargetState ) )
		{
			AfxMessageBox( "[FLContentSwitch]�������� �ʴ� ������ ���¸� ����Ϸ��մϴ�. Ȯ�����ּ���. ����Ʈ�� ��ȯ�մϴ�." ); 
			FLERROR_LOG( PROGRAM_NAME, _T( "eType:%d eState:%d" ), eType, eState );
			
			// set default value
			m_cUserData[ eType ] = ( dwBaseState & 0xFFFF0000 );
		}
	}
	else
	{
		AfxMessageBox( "[FLContentSwitch]�˼� ���� �������� ����Ϸ� �߽��ϴ�. ���α׷� ����" );
		FLERROR_LOG( PROGRAM_NAME, _T( "type:%d state:%d" ), eType, eState );
		PostQuitMessage( 0 );
		return FALSE;
	}


	pair< CSIter, bool > rst = m_cUserData.insert( CSContainer::value_type( eType, eState ) );
	if( false == rst.second )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "already exist UserContent key %d" ), eType );
		//m_cBaseData[ eType ] = dwBitComposion;

		return FALSE;
	}

	return TRUE;
}

DWORD FLContentData::GetUserContent( ContentType eType )
{
	if( FALSE == IsValidUserKey( eType ) )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "error ContentType %d" ), eType );
		return CS_ERROR;
	}

	return m_cUserData[ eType ];
}

DWORD FLContentData::GetDefaultContentState( ContentType eType )
{
	CSIter finder = m_cBaseData.find( eType );
	if( finder == m_cBaseData.end() )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "error, %d" ), eType );
		return CS_ERROR;
	}

	DWORD dwVal = m_cBaseData[ eType ];
	return ( dwVal >> 16 );
}


BOOL IsInclusion( pair< DWORD, DWORD > nBase,  pair< DWORD, DWORD > nElem )
{
	return (BOOL)( nBase.second & nElem.second );
}

BOOL FLContentData::CheckUserContent( )
{
	size_t nBase = m_cBaseData.size();
	size_t nUser = m_cUserData.size();

	if( nBase != nUser )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "Not same size" ) );
		return FALSE;
	}

	bool bEqual = equal( m_cBaseData.begin(), m_cBaseData.end(), m_cUserData.begin(), IsInclusion );
	if( false == bEqual )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "Userdata is not inclusion" ) );
		return FALSE;
	}

	return TRUE;
}

void FLContentData::Reset( )
{
	m_cBaseData.clear( );
	m_cUserData.clear( );
}

int FLContentData::InsertMissedUserContent( )
{
	int nNum = 0;
	for( CSIter iter = m_cBaseData.begin(); iter != m_cUserData.end(); ++iter )
	{
		CSIter userIter = m_cUserData.find( iter->first );
		if( userIter == m_cUserData.end() )
		{
			//���� Default���·� ����
			DWORD dwBitCobination = iter->second;
			m_cUserData.insert( CSContainer::value_type( iter->first, dwBitCobination >> 16 ) );
			++nNum;
		}
	}

	return nNum;	//�߰��� ��Ҽ�
}

//---------------------------------------------------------------------------------------------------------
// FLContentSwitch
//---------------------------------------------------------------------------------------------------------
FLContentSwitch* FLContentSwitch::GetSingletonPtr( )
{
	static FLContentSwitch thisObject;
	return &thisObject;
}

BOOL FLContentSwitch::Init( )
{
	SetupAllContent( );

#if defined( __INTERNALSERVER )
	SetupUserContent_DEV( );
#elif defined( __MAINSERVER )
	SetupUserContent( __VER );
#endif

	BOOL bCheckOK = m_kContent.CheckUserContent();
	if( FALSE == bCheckOK )
	{
		AfxMessageBox( "[FLContentSwitch]FUCK! Content initialize error, terminate the neuz" );
		PostQuitMessage( 0 );	
	}

	return TRUE;
}

void FLContentSwitch::SetupAllContent( )
{
	// ���⿡ ��� �������� �� �������� ������ ���� �׸��� Defalut������ ��� ����ؾ� �մϴ�.
	// RegisterBaseContent( type, ver, default ver )
	m_kContent.RegisterBaseContent( CT_OPTION_17,				CS_VER1 | CS_VER2,							CS_VER1 );
	m_kContent.RegisterBaseContent( CT_SHOP_CART_17,			CS_VER1 | CS_VER2,							CS_VER1 );
	m_kContent.RegisterBaseContent( CT_CANCELLATION,			CS_VER1 | CS_NOT_SUPPORTED,					CS_NOT_SUPPORTED );
	m_kContent.RegisterBaseContent( CT_NEW_INVENTORY,			CS_VER1 | CS_VER2,							CS_VER1 );
	m_kContent.RegisterBaseContent( CT_COLOSSEUM,				CS_VER1 | CS_NOT_SUPPORTED,					CS_NOT_SUPPORTED );
	m_kContent.RegisterBaseContent( CT_LOADSEQ_IMAGEDATA,		CS_VER1 | CS_VER2,							CS_VER1 );
	m_kContent.RegisterBaseContent( CT_ITEM_CHARGE,				CS_VER1 | CS_NOT_SUPPORTED,					CS_NOT_SUPPORTED );
	m_kContent.RegisterBaseContent( CT_BARUNA_PEARCING,			CS_VER1 | CS_NOT_SUPPORTED,					CS_NOT_SUPPORTED );
	m_kContent.RegisterBaseContent( CT_COSTUME_MIX,				CS_VER1 | CS_NOT_SUPPORTED,					CS_NOT_SUPPORTED );
	m_kContent.RegisterBaseContent( CT_DONATION,				CS_VER1 | CS_NOT_SUPPORTED,					CS_NOT_SUPPORTED );
	m_kContent.RegisterBaseContent( CT_BLIND_WNDMAP,			CS_VER1 | CS_NOT_SUPPORTED,					CS_NOT_SUPPORTED );
	m_kContent.RegisterBaseContent( CT_TELEPORTER,				CS_VER1 | CS_NOT_SUPPORTED,					CS_NOT_SUPPORTED );
	m_kContent.RegisterBaseContent( CT_NEW_ENCHANTEFFECT,		CS_VER1 | CS_VER2,							CS_VER1 );
	m_kContent.RegisterBaseContent( CT_RENDER_QUESTEMOTICON,	CS_VER1 | CS_VER2,							CS_VER1 );
	m_kContent.RegisterBaseContent( CT_EVENT_ARENA,				CS_VER1 | CS_NOT_SUPPORTED,					CS_NOT_SUPPORTED );
	m_kContent.RegisterBaseContent( CT_INFO_NOTICE,				CS_VER1 | CS_VER2,							CS_VER1 );
	m_kContent.RegisterBaseContent( CT_TICKETITEM,				CS_VER1 | CS_NOT_SUPPORTED,					CS_NOT_SUPPORTED );
	m_kContent.RegisterBaseContent( CT_DROP_ITEM_REFORM,		CS_VER1 | CS_NOT_SUPPORTED, 				CS_NOT_SUPPORTED );
	m_kContent.RegisterBaseContent( CT_TREASURE_CHEST,			CS_VER1 | CS_NOT_SUPPORTED,					CS_NOT_SUPPORTED );
	m_kContent.RegisterBaseContent( CT_CONSIGNMENT_MARKET,		CS_VER1 | CS_NOT_SUPPORTED,					CS_NOT_SUPPORTED );
	m_kContent.RegisterBaseContent( CT_SERVER_MOVEMENT,			CS_VER1 | CS_NOT_SUPPORTED,					CS_NOT_SUPPORTED );
	m_kContent.RegisterBaseContent( CT_ELLDINS_JAR,				CS_VER1 | CS_NOT_SUPPORTED,					CS_NOT_SUPPORTED );
	m_kContent.RegisterBaseContent( CT_MADRIGAL_GIFT,			CS_VER1 | CS_NOT_SUPPORTED,					CS_NOT_SUPPORTED );
	m_kContent.RegisterBaseContent( CT_SHUTDOWN_RULE,			CS_VER1 | CS_NOT_SUPPORTED,					CS_NOT_SUPPORTED );
	m_kContent.RegisterBaseContent( CT_COMPOSE_TWOWEAPON19,		CS_VER1 | CS_NOT_SUPPORTED,					CS_NOT_SUPPORTED );
	m_kContent.RegisterBaseContent( CT_NEWUI_19,				CS_VER1 | CS_VER2,							CS_VER1);
	m_kContent.RegisterBaseContent( CT_LOOKS_CHANGE,			CS_VER1 | CS_NOT_SUPPORTED,					CS_NOT_SUPPORTED );
	m_kContent.RegisterBaseContent( CT_DB_ITEM_EXTEND,			CS_VER1 | CS_NOT_SUPPORTED,					CS_NOT_SUPPORTED );
	m_kContent.RegisterBaseContent( CT_ATTR_SYSTEM_MODIFY,		CS_VER1 | CS_NOT_SUPPORTED,					CS_NOT_SUPPORTED );
	m_kContent.RegisterBaseContent( CT_REAWAKENING,				CS_NOT_SUPPORTED | CS_VER1 | CS_VER2,		CS_NOT_SUPPORTED );
	m_kContent.RegisterBaseContent( CT_FLYFF_PIECE_SYSTEM,		CS_VER1 | CS_VER2,							CS_NOT_SUPPORTED );

#ifdef __APP_TELEPORTER
	//m_kContent.RegisterBaseContent(CT_APP_TELEPORTER, CS_VER1 | CS_NOT_SUPPORTED, CS_NOT_SUPPORTED);
#endif
	if( CT_MAX > m_kContent.GetSize_BaseContent( ) )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "FUCK! Not registered all contents" ) );
		PostQuitMessage( 0 );
	}
}

#pragma warning ( push )
#pragma warning ( disable : 6326 )

#ifdef __INTERNALSERVER
void FLContentSwitch::SetupUserContent_DEV()
{
	//����� ���� ���߹����� ���� ����ϴ� ���Դϴ�.
#pragma message( "[����������] : �����׽�Ʈ(__INTERNALSERVER)������ �����Ǿ����ϴ�." )
	if( __VER == 17 )
	{
		m_kContent.RegisterUserContent( CT_OPTION_17,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_SHOP_CART_17,			CS_VER2 );
		m_kContent.RegisterUserContent( CT_CANCELLATION,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEW_INVENTORY,			CS_VER2 );
		m_kContent.RegisterUserContent( CT_COLOSSEUM,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_LOADSEQ_IMAGEDATA,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_ITEM_CHARGE,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_BARUNA_PEARCING,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_COSTUME_MIX,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_DONATION,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_BLIND_WNDMAP,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_TELEPORTER,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEW_ENCHANTEFFECT,		CS_VER2 );
		m_kContent.RegisterUserContent( CT_RENDER_QUESTEMOTICON,	CS_VER2 );
		m_kContent.RegisterUserContent( CT_EVENT_ARENA,				CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_INFO_NOTICE,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_TICKETITEM,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_DROP_ITEM_REFORM,		CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_TREASURE_CHEST,			CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_CONSIGNMENT_MARKET,		CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_SERVER_MOVEMENT,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_ELLDINS_JAR ,			CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_MADRIGAL_GIFT,			CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_SHUTDOWN_RULE,			CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_COMPOSE_TWOWEAPON19 ,	CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_NEWUI_19 ,				CS_VER1);
		m_kContent.RegisterUserContent( CT_LOOKS_CHANGE,			CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_DB_ITEM_EXTEND,			CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_ATTR_SYSTEM_MODIFY,		CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_REAWAKENING,				CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_FLYFF_PIECE_SYSTEM,		CS_NOT_SUPPORTED );
	}
	else if( __VER == 18 )
	{
		m_kContent.RegisterUserContent( CT_OPTION_17,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_SHOP_CART_17,			CS_VER2 );
		m_kContent.RegisterUserContent( CT_CANCELLATION,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEW_INVENTORY,			CS_VER2 );
		m_kContent.RegisterUserContent( CT_COLOSSEUM,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_LOADSEQ_IMAGEDATA,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_ITEM_CHARGE,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_BARUNA_PEARCING,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_COSTUME_MIX,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_DONATION,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_BLIND_WNDMAP,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_TELEPORTER,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEW_ENCHANTEFFECT,		CS_VER2 );
		m_kContent.RegisterUserContent( CT_RENDER_QUESTEMOTICON,	CS_VER2 );
		m_kContent.RegisterUserContent( CT_EVENT_ARENA,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_INFO_NOTICE,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_TICKETITEM,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_DROP_ITEM_REFORM,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_TREASURE_CHEST,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_CONSIGNMENT_MARKET,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_SERVER_MOVEMENT,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_ELLDINS_JAR ,			CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_MADRIGAL_GIFT,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_SHUTDOWN_RULE,			CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_COMPOSE_TWOWEAPON19 ,	CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_NEWUI_19 ,				CS_VER1);
		m_kContent.RegisterUserContent( CT_LOOKS_CHANGE,			CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_DB_ITEM_EXTEND,			CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_ATTR_SYSTEM_MODIFY,		CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_REAWAKENING,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_FLYFF_PIECE_SYSTEM,		CS_VER1 );
	}
	else if( __VER == 19 )
	{
		m_kContent.RegisterUserContent( CT_OPTION_17,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_SHOP_CART_17,			CS_VER2 );
		m_kContent.RegisterUserContent( CT_CANCELLATION,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEW_INVENTORY,			CS_VER2 );
		m_kContent.RegisterUserContent( CT_COLOSSEUM,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_LOADSEQ_IMAGEDATA,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_ITEM_CHARGE,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_BARUNA_PEARCING,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_COSTUME_MIX,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_DONATION,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_BLIND_WNDMAP,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_TELEPORTER,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEW_ENCHANTEFFECT,		CS_VER2 );
		m_kContent.RegisterUserContent( CT_RENDER_QUESTEMOTICON,	CS_VER2 );
		m_kContent.RegisterUserContent( CT_EVENT_ARENA,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_INFO_NOTICE,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_TICKETITEM,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_DROP_ITEM_REFORM,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_TREASURE_CHEST,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_CONSIGNMENT_MARKET,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_SERVER_MOVEMENT,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_ELLDINS_JAR ,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_MADRIGAL_GIFT,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_SHUTDOWN_RULE,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_COMPOSE_TWOWEAPON19 ,	CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEWUI_19 ,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_LOOKS_CHANGE,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_DB_ITEM_EXTEND,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_ATTR_SYSTEM_MODIFY,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_REAWAKENING,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_FLYFF_PIECE_SYSTEM,		CS_VER2 );
	}
	else if( __VER == 20 )
	{
		m_kContent.RegisterUserContent( CT_OPTION_17,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_SHOP_CART_17,			CS_VER2 );
		m_kContent.RegisterUserContent( CT_CANCELLATION,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEW_INVENTORY,			CS_VER2 );
		m_kContent.RegisterUserContent( CT_COLOSSEUM,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_LOADSEQ_IMAGEDATA,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_ITEM_CHARGE,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_BARUNA_PEARCING,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_COSTUME_MIX,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_DONATION,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_BLIND_WNDMAP,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_TELEPORTER,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEW_ENCHANTEFFECT,		CS_VER2 );
		m_kContent.RegisterUserContent( CT_RENDER_QUESTEMOTICON,	CS_VER2 );
		m_kContent.RegisterUserContent( CT_EVENT_ARENA,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_INFO_NOTICE,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_TICKETITEM,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_DROP_ITEM_REFORM,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_TREASURE_CHEST,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_CONSIGNMENT_MARKET,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_SERVER_MOVEMENT,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_ELLDINS_JAR ,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_MADRIGAL_GIFT,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_SHUTDOWN_RULE,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_COMPOSE_TWOWEAPON19 ,	CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEWUI_19 ,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_LOOKS_CHANGE,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_DB_ITEM_EXTEND,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_ATTR_SYSTEM_MODIFY,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_REAWAKENING,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_FLYFF_PIECE_SYSTEM,		CS_VER2 );
	}

	else if( __VER >= 21 )
	{
		m_kContent.RegisterUserContent( CT_OPTION_17,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_SHOP_CART_17,			CS_VER2 );
		m_kContent.RegisterUserContent( CT_CANCELLATION,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEW_INVENTORY,			CS_VER2 );
		m_kContent.RegisterUserContent( CT_COLOSSEUM,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_LOADSEQ_IMAGEDATA,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_ITEM_CHARGE,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_BARUNA_PEARCING,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_COSTUME_MIX,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_DONATION,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_BLIND_WNDMAP,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_TELEPORTER,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEW_ENCHANTEFFECT,		CS_VER2 );
		m_kContent.RegisterUserContent( CT_RENDER_QUESTEMOTICON,	CS_VER2 );
		m_kContent.RegisterUserContent( CT_EVENT_ARENA,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_INFO_NOTICE,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_TICKETITEM,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_DROP_ITEM_REFORM,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_TREASURE_CHEST,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_CONSIGNMENT_MARKET,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_SERVER_MOVEMENT,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_ELLDINS_JAR ,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_MADRIGAL_GIFT,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_SHUTDOWN_RULE,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_COMPOSE_TWOWEAPON19 ,	CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEWUI_19 ,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_LOOKS_CHANGE,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_DB_ITEM_EXTEND,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_ATTR_SYSTEM_MODIFY,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_REAWAKENING,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_FLYFF_PIECE_SYSTEM,		CS_VER2 );
	}


#ifdef __APP_TELEPORTER
	//m_kContent.RegisterUserContent(CT_APP_TELEPORTER, CS_VER1);
#endif

	if( m_kContent.GetSize_UserContent( ) != m_kContent.GetSize_BaseContent( ) )
	{
		FLASSERT( 0 && "���� ���߹��� �������� ���� �������� �ʾҴ� Ȯ�ιٶ�, �����ϸ� DEFAULT" );
		m_kContent.InsertMissedUserContent();
	}
}
#endif

#ifdef __MAINSERVER
void FLContentSwitch::SetupUserContent( const int nUserVer )
{
	//����� ������ ������ ���� ����ϴ� ���Դϴ�.

#pragma message( "[FLContentSwitch] : ���ļ���(__MAINSERVER)������ �����Ǿ����ϴ�." )

	if( nUserVer == 17 )
	{
#pragma message( "[FLContentSwitch] : 17�� �����Դϴ�" )
		m_kContent.RegisterUserContent( CT_OPTION_17,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_SHOP_CART_17,			CS_VER2 );
		m_kContent.RegisterUserContent( CT_CANCELLATION,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEW_INVENTORY,			CS_VER2 );
		m_kContent.RegisterUserContent( CT_COLOSSEUM,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_LOADSEQ_IMAGEDATA,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_ITEM_CHARGE,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_BARUNA_PEARCING,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_COSTUME_MIX,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_DONATION,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_BLIND_WNDMAP,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_TELEPORTER,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEW_ENCHANTEFFECT,		CS_VER2 );
		m_kContent.RegisterUserContent( CT_RENDER_QUESTEMOTICON,	CS_VER2 );
		m_kContent.RegisterUserContent( CT_EVENT_ARENA,				CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_INFO_NOTICE,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_TICKETITEM,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_DROP_ITEM_REFORM,		CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_TREASURE_CHEST,			CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_CONSIGNMENT_MARKET,		CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_SERVER_MOVEMENT,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_ELLDINS_JAR ,			CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_MADRIGAL_GIFT ,			CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_SHUTDOWN_RULE,			CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_COMPOSE_TWOWEAPON19 ,	CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_NEWUI_19 ,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_LOOKS_CHANGE,			CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_DB_ITEM_EXTEND,			CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_ATTR_SYSTEM_MODIFY,		CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_REAWAKENING,				CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_FLYFF_PIECE_SYSTEM,		CS_NOT_SUPPORTED );
	}
	else if( nUserVer == 18 )
	{
#pragma message( "[FLContentSwitch] : 18�� �����Դϴ�" )
		m_kContent.RegisterUserContent( CT_OPTION_17,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_SHOP_CART_17,			CS_VER2 );
		m_kContent.RegisterUserContent( CT_CANCELLATION,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEW_INVENTORY,			CS_VER2 );
		m_kContent.RegisterUserContent( CT_COLOSSEUM,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_LOADSEQ_IMAGEDATA,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_ITEM_CHARGE,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_BARUNA_PEARCING,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_COSTUME_MIX,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_DONATION,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_BLIND_WNDMAP,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_TELEPORTER,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEW_ENCHANTEFFECT,		CS_VER2 );
		m_kContent.RegisterUserContent( CT_RENDER_QUESTEMOTICON,	CS_VER2 );
		m_kContent.RegisterUserContent( CT_EVENT_ARENA,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_INFO_NOTICE,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_TICKETITEM,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_DROP_ITEM_REFORM,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_TREASURE_CHEST,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_CONSIGNMENT_MARKET,		CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_SERVER_MOVEMENT,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_ELLDINS_JAR ,			CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_MADRIGAL_GIFT ,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_SHUTDOWN_RULE,			CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_COMPOSE_TWOWEAPON19 ,	CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_NEWUI_19 ,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_LOOKS_CHANGE,			CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_DB_ITEM_EXTEND,			CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_ATTR_SYSTEM_MODIFY,		CS_NOT_SUPPORTED );
		m_kContent.RegisterUserContent( CT_REAWAKENING,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_FLYFF_PIECE_SYSTEM,		CS_VER1 );
	}
	else if( nUserVer == 19 )
	{
#pragma message( "[FLContentSwitch] : 19�� �����Դϴ�" )
		m_kContent.RegisterUserContent( CT_OPTION_17,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_SHOP_CART_17,			CS_VER2 );
		m_kContent.RegisterUserContent( CT_CANCELLATION,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEW_INVENTORY,			CS_VER2 );
		m_kContent.RegisterUserContent( CT_COLOSSEUM,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_LOADSEQ_IMAGEDATA,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_ITEM_CHARGE,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_BARUNA_PEARCING,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_COSTUME_MIX,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_DONATION,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_BLIND_WNDMAP,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_TELEPORTER,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEW_ENCHANTEFFECT,		CS_VER2 );
		m_kContent.RegisterUserContent( CT_RENDER_QUESTEMOTICON,	CS_VER2 );
		m_kContent.RegisterUserContent( CT_EVENT_ARENA,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_INFO_NOTICE,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_TICKETITEM,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_DROP_ITEM_REFORM,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_TREASURE_CHEST,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_CONSIGNMENT_MARKET,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_SERVER_MOVEMENT,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_ELLDINS_JAR ,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_MADRIGAL_GIFT ,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_SHUTDOWN_RULE,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_COMPOSE_TWOWEAPON19 ,	CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEWUI_19 ,				CS_VER2 );		//gmpbigsun(20111209) :  ������
		m_kContent.RegisterUserContent( CT_LOOKS_CHANGE,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_DB_ITEM_EXTEND,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_ATTR_SYSTEM_MODIFY,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_REAWAKENING,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_FLYFF_PIECE_SYSTEM,		CS_VER2 );
	}
	else if( nUserVer == 20 )
	{
#pragma message( "[FLContentSwitch] : 20�� �����Դϴ�" )
		m_kContent.RegisterUserContent( CT_OPTION_17,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_SHOP_CART_17,			CS_VER2 );
		m_kContent.RegisterUserContent( CT_CANCELLATION,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEW_INVENTORY,			CS_VER2 );
		m_kContent.RegisterUserContent( CT_COLOSSEUM,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_LOADSEQ_IMAGEDATA,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_ITEM_CHARGE,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_BARUNA_PEARCING,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_COSTUME_MIX,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_DONATION,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_BLIND_WNDMAP,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_TELEPORTER,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEW_ENCHANTEFFECT,		CS_VER2 );
		m_kContent.RegisterUserContent( CT_RENDER_QUESTEMOTICON,	CS_VER2 );
		m_kContent.RegisterUserContent( CT_EVENT_ARENA,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_INFO_NOTICE,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_TICKETITEM,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_DROP_ITEM_REFORM,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_TREASURE_CHEST,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_CONSIGNMENT_MARKET,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_SERVER_MOVEMENT,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_ELLDINS_JAR ,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_MADRIGAL_GIFT ,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_SHUTDOWN_RULE,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_COMPOSE_TWOWEAPON19 ,	CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEWUI_19 ,				CS_VER2 );		//gmpbigsun(20111209) :  ������
		m_kContent.RegisterUserContent( CT_LOOKS_CHANGE,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_DB_ITEM_EXTEND,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_ATTR_SYSTEM_MODIFY,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_REAWAKENING,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_FLYFF_PIECE_SYSTEM,		CS_VER2 );
	}

	else if( nUserVer >= 21 )
	{
#pragma message( "[FLContentSwitch] : 21�� �����Դϴ�" )
		m_kContent.RegisterUserContent( CT_OPTION_17,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_SHOP_CART_17,			CS_VER2 );
		m_kContent.RegisterUserContent( CT_CANCELLATION,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEW_INVENTORY,			CS_VER2 );
		m_kContent.RegisterUserContent( CT_COLOSSEUM,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_LOADSEQ_IMAGEDATA,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_ITEM_CHARGE,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_BARUNA_PEARCING,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_COSTUME_MIX,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_DONATION,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_BLIND_WNDMAP,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_TELEPORTER,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEW_ENCHANTEFFECT,		CS_VER2 );
		m_kContent.RegisterUserContent( CT_RENDER_QUESTEMOTICON,	CS_VER2 );
		m_kContent.RegisterUserContent( CT_EVENT_ARENA,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_INFO_NOTICE,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_TICKETITEM,				CS_VER1 );
		m_kContent.RegisterUserContent( CT_DROP_ITEM_REFORM,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_TREASURE_CHEST,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_CONSIGNMENT_MARKET,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_SERVER_MOVEMENT,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_ELLDINS_JAR ,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_MADRIGAL_GIFT ,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_SHUTDOWN_RULE,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_COMPOSE_TWOWEAPON19 ,	CS_VER1 );
		m_kContent.RegisterUserContent( CT_NEWUI_19 ,				CS_VER2 );		//gmpbigsun(20111209) :  ������
		m_kContent.RegisterUserContent( CT_LOOKS_CHANGE,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_DB_ITEM_EXTEND,			CS_VER1 );
		m_kContent.RegisterUserContent( CT_ATTR_SYSTEM_MODIFY,		CS_VER1 );
		m_kContent.RegisterUserContent( CT_REAWAKENING,				CS_VER2 );
		m_kContent.RegisterUserContent( CT_FLYFF_PIECE_SYSTEM,		CS_VER2 );
	}

	if( m_kContent.GetSize_UserContent( ) != m_kContent.GetSize_BaseContent( ) )
	{
		int rst = AfxMessageBox( "Found unchecking content, do you keep going with DEFAULT set? or quit?", MB_OKCANCEL );
		if( MB_OK == rst )
		{
			int nNum = m_kContent.InsertMissedUserContent( );
			FLERROR_LOG( PROGRAM_NAME, _T( "size mismatch base:%d user:%d added:%d" ), m_kContent.GetSize_BaseContent(), m_kContent.GetSize_UserContent(), nNum );
		}
		else if( MB_CANCEL == rst )
		{
			PostQuitMessage( 0 );
		}
	}

}
#endif

#pragma warning ( pop )

