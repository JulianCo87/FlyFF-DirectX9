#ifndef __DPLOGINSRVR_H__
#define __DPLOGINSRVR_H__

#pragma once

#include <network/FLDPMng.h>
#include <MsgHdr.h>

//class CAccount;
class CDPLoginSrvr : public CDPMng
{
public:
//	Constructions
	CDPLoginSrvr();
	virtual	~CDPLoginSrvr();
	
//	Operations
	virtual void	SysMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom );
	virtual void	UserMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom );

//	void	SendPlayerList( CAccount* pAccount, DPID dpid );
	void	SendPlayerList( const char* lpszAccount, const char* lpszPassword, DPID dpid, DWORD dwAuthKey, BYTE bySelectPage = 0 );
	void	SendError( LONG lError, DPID dpid );	//
	void	SendCloseExistingConnection( const char* lpszAccount, LONG lError );
	void	SendFail( long lError, const char*lpszAccount );
	void	SendOneHourNotify( const char* lpszAccount);

	void	OnAddConnection( DPID dpid );
	void	OnRemoveConnection( DPID dpid );

	USES_PFNENTRIES( CDPLoginSrvr, ( CAr & ar, DPID, LPBYTE lpBuf, u_long uBufSize ) );
public:
//	Handlers
	void	OnGetPlayerList( CAr & ar, DPID dpid, LPBYTE lpBuf, u_long uBufSize );
	void	OnCreatePlayer( CAr & ar, DPID dpid, LPBYTE lpBuf, u_long uBufSize );
	void	OnRemovePlayer( CAr & ar, DPID dpid, LPBYTE lpBuf, u_long uBufSize );
	void	OnLeave( CAr & ar, DPID dpid, LPBYTE lpBuf, u_long uBufSize );
	void	OnCloseError( CAr & ar, DPID dpid, LPBYTE lpBuf, u_long uBufSize );
	void	OnLoginProtect( CAr & ar, DPID dpid, LPBYTE lpBuf, u_long uBufSize );
#ifdef PASSWORD_RESET_2ND
	void	OnChangeBankPass( CAr & ar, DPID dpid, LPBYTE lpBuf, u_long uBufSize );
#endif	// PASSWORD_RESET_2ND
	void	SendLoginProtect( const char* lpszAccount, const char* lpszPlayer, u_long idPlayer, BOOL bLogin, DPID dpId );

private:

	DPID	m_dpid;
};

#endif	// __DPLOGINSRVR_H__