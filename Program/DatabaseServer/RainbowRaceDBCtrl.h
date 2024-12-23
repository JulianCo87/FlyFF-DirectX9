#pragma once
#include "dbcontroller.h"

/////////////////////////////////////////////////////////////////////////
/// CRainbowRaceDBCtrl //////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
enum
{
	QUERY_RR_LOAD = 0,
	QUERY_RR_APPLICATION,
	QUERY_RR_FAILEDUSER,
	QUERY_RR_RANKING
};

class CRainbowRaceDBCtrl :	public CDbController
{
public:
	CRainbowRaceDBCtrl(void);
	virtual ~CRainbowRaceDBCtrl(void);

	virtual void Handler( LPDB_OVERLAPPED_PLUS pov, ULONG_PTR dwCompletionKey );

private:
	void LoadRainbowRace( DPID dpId );
	void Application( DWORD dwPlayerId );
	void FailedUser( DWORD dwPlayerId );
	void UpdateRanking( DWORD dwPlayerId, int nRanking );
	void UpdateWinnersLosers();

	int m_nTimes;
};


/////////////////////////////////////////////////////////////////////////
/// CRainbowRaceDBMng ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
class CRainbowRaceDBMng
{
public:
	CRainbowRaceDBMng(void);
	~CRainbowRaceDBMng(void);
	static CRainbowRaceDBMng* GetInstance();

	BOOL PostRequest( int nQuery, BYTE* lpBuf = NULL, int nBufSize = 0, ULONG_PTR dwCompletionKey = 0 )
	{ return m_RRDbCtrl.PostRequest( nQuery, lpBuf, nBufSize, dwCompletionKey );	}

private:
	CRainbowRaceDBCtrl	m_RRDbCtrl;
};

