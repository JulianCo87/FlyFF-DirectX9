// EventLua.h: interface for the CEventLua class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENTLUA_H__CB7F2A4F_B0F3_47A4_BC55_55047E95D5D2__INCLUDED_)
#define AFX_EVENTLUA_H__CB7F2A4F_B0F3_47A4_BC55_55047E95D5D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LuaBase.h"

// #ifdef __WORLDSERVER
// #include "FLFlyffPieceDailyPayment.h"
// #endif // __WORLDSERVER

#ifdef __WORLDSERVER
class CEventLua;
class CEventLuaProxy
{
public:
	CEventLuaProxy();
	virtual	~CEventLuaProxy();
public:
	vector<BYTE>	GetEventList()	{	return m_vEventLists;	}
	void	SetEventList( const vector<BYTE> & vEventLists )	{	m_vEventLists.assign( vEventLists.begin(), vEventLists.end() );	}
	float	GetExpFactor()	{	return m_fExpFactor;	}
	void	SetExpFactor( float fExpFactor )	{	m_fExpFactor	= fExpFactor;	}
	float	GetItemDropRate()	{	return m_fItemDropRate;	}
	void	SetItemDropRate( float fItemDropRate )	{	m_fItemDropRate	= fItemDropRate;	}
	float	GetPieceItemDropRate()	{	return m_fPieceItemDropRate;	}
	void	SetPieceItemDropRate( float fPieceItemDropRate )	{	m_fPieceItemDropRate	= fPieceItemDropRate;	}
	float	GetGoldDropFactor()	{	return m_fGoldDropFactor;	}
	void	SetGoldDropFactor( float GoldDropFactor )	{	m_fGoldDropFactor	= GoldDropFactor;	}
	int		GetAttackPower()	{	return m_nAttackPower;	}
	void	SetAttackPower( int nAttackPower )	{	m_nAttackPower	= nAttackPower;		}
	int		GetDefensePower()	{	return m_fDefensePower;		}
	void	SetDefensePower( int fDefensePower )	{	m_fDefensePower	= fDefensePower;	}
	DWORD	GetCouponEvent()	{	return m_dwCouponEvent;	}
	void	SetCouponEvent( DWORD dwCouponEvent )	{	m_dwCouponEvent	= dwCouponEvent;	}
#ifdef __EVENTLUA_CHEEREXP
	void	SetCheerExpFactor( float fCheerExpFactor ) { m_fCheerExpFactor = fCheerExpFactor; }
	float	GetCheerExpFactor() { return m_fCheerExpFactor; }
#endif // __EVENTLUA_CHEEREXP
#ifdef __EVENTLUA_KEEPCONNECT
	DWORD	GetKeepConnectTime()	{	return m_dwKeepConnect;	}
	void	SetKeepConnectTime( DWORD dwKeepConnect )	{	m_dwKeepConnect	= dwKeepConnect;	}
#endif // __EVENTLUA_KEEPCONNECT

	void	SetWeatherEventExpFactor( float fWeatherEventExpFactor )	{ m_fWeatherEventExpFactor = fWeatherEventExpFactor; }
	float	GetWeatherEventExpFactor()	{ return m_fWeatherEventExpFactor; }

	void	SetShopBuyFactor( float fShopBuyFactor )	{	m_fShopBuyFactor = fShopBuyFactor;	}
	float	GetShopBuyFactor()	{	return m_fShopBuyFactor;	}
	void	SetShopSellFactor( float fShopSellFactor )	{	m_fShopSellFactor = fShopSellFactor;	}
	float	GetShopSellFactor()	{	return m_fShopSellFactor;	}

	void	Initialize( CEventLua* pEventLua );
private:
	vector<BYTE>	m_vEventLists;
	float	m_fExpFactor;
	float	m_fItemDropRate;
	float	m_fPieceItemDropRate;
	float	m_fGoldDropFactor;
	int		m_nAttackPower;
	int		m_fDefensePower;
	DWORD	m_dwCouponEvent;
#ifdef __EVENTLUA_CHEEREXP
	float	m_fCheerExpFactor;
#endif // __EVENTLUA_CHEEREXP
#ifdef __EVENTLUA_KEEPCONNECT
	DWORD	m_dwKeepConnect;
#endif // __EVENTLUA_KEEPCONNECT
	float	m_fWeatherEventExpFactor;
	float	m_fShopBuyFactor;
	float	m_fShopSellFactor;
};

#endif // __WORLDSERVER

class CEventLua  
{
public:
	CEventLua();
	virtual ~CEventLua();

	void			Clear();

#ifdef __DBSERVER
	BOOL			IsNoticeTime();
	vector<string>	GetNoticeMessage();
	BOOL			CheckEventState();
	map<BYTE, BOOL>	m_mapState;
	CMclCritSec		m_Access;
private:
	BOOL			m_bTimeLimit;
public:
	BOOL			IsTimeLimit()	{ return m_bTimeLimit; }
#endif // __DBSERVER

#if defined(__WORLDSERVER) || defined(__DBSERVER)
	vector<BYTE>	GetEventList( BOOL bProxy = TRUE );
	void			LoadScript();
	BOOL			IsPossible() { return m_bRun; }
	BOOL			m_bRun;
	CLuaBase m_Lua;
#endif // __WORLDSERVER, __DBSERVER

#ifdef __WORLDSERVER
	void			SetState( BYTE nId, BOOL bState );
	float			GetExpFactor( BOOL bProxy = TRUE );
	float			GetItemDropRate( BOOL bProxy = TRUE );
	float			GetPieceItemDropRate( BOOL bProxy = TRUE );
	float			GetGoldDropFactor( BOOL bProxy = TRUE );
	int				GetAttackPower( BOOL bProxy = TRUE );
	int				GetDefensePower( BOOL bProxy = TRUE );

	void			SetCoupon( FLWSUser* pUser, DWORD dwTick );

	string			GetDesc( BYTE nId );	// x
#ifdef __EVENTLUA_GIFT
	void			SetLevelUpGift( FLWSUser* pUser, int nLevel );		// x
#endif // __EVENTLUA_GIFT
	map<DWORD, int>	GetItem( DWORD dwLevel );	// x
	void GetAllEventList( FLWSUser* pUser );	// x
	void GetEventInfo( FLWSUser* pUser, int nId );	// x
#ifdef __EVENTLUA_CHEEREXP
	float GetCheerExpFactor( BOOL bProxy = TRUE );
#endif // __EVENTLUA_CHEEREXP

	float			GetWeatherEventExpFactor( BOOL bProxy = TRUE );
	string			GetWeatherEventTitle();

	float			GetShopBuyFactor( BOOL bProxy = TRUE );
	float			GetShopSellFactor( BOOL bProxy = TRUE );
#endif // __WORLDSERVER
	
	DWORD	GetCouponEvent( BOOL bProxy = TRUE );

#ifdef __WORLDSERVER
	void	PrepareProxy()	{	m_proxy.Initialize( this );	}
private:
	CEventLuaProxy	m_proxy;

#ifdef __EVENTLUA_SPAWN
private:
	struct __SPAWNINFO
	{
		int		nType;
		DWORD	dwId, dwInterval, dwTimeOut;

		//////////////////////////////////////////////////////////////////////////
		// mirchang_101012 Ÿ�Կ� ���� ���� �߰�
		BYTE	byFlag;

		__SPAWNINFO( int nT, DWORD dwI, DWORD dwIT, BYTE byFl ) : nType( nT ), dwId( dwI ), dwInterval( dwIT ), byFlag( byFl ), dwTimeOut( 0 ) {}
		//////////////////////////////////////////////////////////////////////////

		BOOL	IsTimeOut()
		{
			DWORD dwCurrentTime = timeGetTime();
			if( dwTimeOut > dwCurrentTime )
				return FALSE;
			dwTimeOut = dwCurrentTime + dwInterval;
			return TRUE;
		}
	};
	typedef vector<__SPAWNINFO> VECSPAWNINFO;

	struct __SPAWNREGION
	{
		DWORD	dwWorldId;		// ���� ��ȣ
		CRect	rect;	// ���� ����
		__SPAWNREGION( DWORD dwWI, CRect & r ) : dwWorldId( dwWI ), rect( r ) {}
		D3DXVECTOR3 GetPos() { return D3DXVECTOR3( (float)( xRandom( rect.left, rect.right ) ), 0.0f, (float)( xRandom( rect.top, rect.bottom ) ) ); }
	};

	vector<__SPAWNREGION>	m_vecSpawnRegion;		// spawn �̺�Ʈ ����� ������ �� ������ ������ġ(���� �� ��ǥ) ����

	map<BYTE, VECSPAWNINFO>	m_mapSpawnList;			// spawn �̺�Ʈ ����� ������ ������ �� ���� ����
	map<DWORD, BYTE>		m_mapMonsterId;			// spawn �̺�Ʈ ����� ������ ���Ͱ� �̺�Ʈ�� ���� �˻��ϱ� ���� �ʿ�
	map<OBJID, BYTE>		m_mapSpawnedMonster;	// spawn �̺�Ʈ ����� ������ ������ ���͵��� ��� ����(�̺�Ʈ ����� �����ϱ� ���� �ʿ�)
	vector<BYTE>			m_vecEndSpawnEvent;			// ����� �̺�Ʈ ID ���( �����ִ� spawn ���Ͱ� ������ �� ������ �� ID�� �����Ѵ�

	void	SetSpawnEvent( BYTE nId, BOOL bState );
	__SPAWNREGION GetSpawnRegion() { return m_vecSpawnRegion.at( xRandom( m_vecSpawnRegion.size() ) ); }
public:
	void	SetAddSpawnRegion( DWORD dwWorldId, CRect & rect ) { m_vecSpawnRegion.push_back( __SPAWNREGION( dwWorldId, rect ) ); }
	BOOL	IsEventSpawnMonster( DWORD dwMonsterId ) { return m_mapMonsterId.find( dwMonsterId ) != m_mapMonsterId.end(); }
	void	EventSpawnProcess();
	void	DeleteEndEventProcess();
	void	RemoveSpawnedMonster( OBJID objId ) { m_mapSpawnedMonster.erase( objId ); }
#endif // __EVENTLUA_SPAWN

#ifdef __EVENTLUA_KEEPCONNECT
private:

	typedef struct _T_KEEP_CONNECT_ITEM
	{
		DWORD	dwItemId;
		int		nItemNum;
		BYTE	byFlag;

	} T_KEEP_CONNECT_ITEM, *PT_KEEP_CONNECT_ITEM;

	vector<u_long>	m_vecKeepConnectUser;
	vector<T_KEEP_CONNECT_ITEM> m_vecKeepConnectItem;

public:
	void			SetKeepConnectEvent( FLWSUser* pUser, DWORD dwTick );
	DWORD			GetKeepConnectTime( BOOL bProxy = TRUE );
	void			GetKeepConnectItem();
	void			KeepConnectEventProcess();
#endif // __EVENTLUA_KEEPCONNECT

	private:

		typedef struct _T_TROUPE_EXPUP_DATA
		{
			DWORD	dwTroupeMemberCount;
			float	fTroupeExpUpFactor;

			_T_TROUPE_EXPUP_DATA()
			{
				dwTroupeMemberCount		= 0;
				fTroupeExpUpFactor		= 1.0f;
			}

		} T_TROUPE_EXPUP_DATA, *PT_TROUPE_EXPUP_DATA;

		vector<T_TROUPE_EXPUP_DATA>		m_vecTroupeExpUpData;

	public:
		void		GetTroupeExpUpEvent();
		float		GetTroupeExpUpFactor( DWORD dwTroupeMemberCount );


	//////////////////////////////////////////////////////////////////////////
// 	private:
// 		FLFlyffPieceDailyPayment	m_kFlyffPiece;
// 
// 	public:
// 		void	GetDailyKeepConnectEvent();
// 		void	OnDailyKeepConnection( FLWSUser* pUser, const DWORD dwTick ) const;
// 
// 		bool	StartBurningMode( const T_BURNING_MODE & kMode );
// 		bool	EndBurningMode();
// 
// 		void	ProcessCheckBurningMode( const time_t time );
	//////////////////////////////////////////////////////////////////////////


#endif // __WORLDSERVER
};

#endif // !defined(AFX_EVENTLUA_H__CB7F2A4F_B0F3_47A4_BC55_55047E95D5D2__INCLUDED_)
