
#ifndef	__FLSTREAMPROTOCOL_H__
#define	__FLSTREAMPROTOCOL_H__


#include "FLNetworkCommon.h"
#include "FLBuffer.h"


class	FLStreamProtocol
{
protected:

	FLStreamProtocol();
	virtual ~FLStreamProtocol();

public:

	// recv stream
	virtual	bool	FetchStream( CBuffer** pBuffer, CBufferQueue* pQueue );
	virtual bool	PushSystemStream( CBuffer* pBuffer, LPBYTE lpData, u_long uDataSize );
	// send stream
	virtual bool	SendStream( CBufferQueue* pQueue, LPBYTE lpData, u_long uDataSize );

	// creator
	virtual FLStreamProtocol*	Clone();
	virtual void				Release();

	// protocol sync
	virtual bool		StartProtocol( CBufferQueue* /*pQueue*/ ){ return true;}
	virtual bool		ReadySyncProtocol(){ return true; }
	virtual bool		WaitSyncProtocol( DWORD /*dwTimeout*/ ){ return true; }

	// message handling
	virtual void		GetData( void* pStream, void** pData, u_long* nDataSize );
	virtual void*		GetNextStream( void* pStream );
	static	FLStreamProtocol*	GetInstance();
};
/*
class	FLStreamProtocol : public FLStreamProtocol
{
private:

	FLStreamProtocol();
	~FLStreamProtocol();

public:

	bool	FetchStream( CBuffer** pBuffer, CBufferQueue* pQueue );
	bool	PushSystemStream( CBuffer* pBuffer, LPBYTE lpData, u_long uDataSize );

	bool	SendStream( CBufferQueue* pQueue, LPBYTE lpData, u_long uDataSize );

	FLStreamProtocol*	Clone();
	void				Release();

	void		GetData( void* pStream, void** pData, u_long* nDataSize );
	void*		GetNextStream( void* pStream );


	static	FLStreamProtocol*	GetInstance();

};
*/

class	FLXORProtocol : public FLStreamProtocol
{
private:

	FLXORProtocol();
	~FLXORProtocol();

public:

	bool	FetchStream( CBuffer** pBuffer, CBufferQueue* pQueue );
	bool	PushSystemStream( CBuffer* pBuffer, LPBYTE lpData, u_long uDataSize );

	bool	SendStream( CBufferQueue* pQueue, LPBYTE lpData, u_long uDataSize );

	FLStreamProtocol*	Clone();
	void				Release();

	bool		StartProtocol( CBufferQueue* pQueue );
	bool		ReadySyncProtocol();
	bool		WaitSyncProtocol( DWORD dwTimeout );

	void		GetData( void* pStream, void** pData, u_long* nDataSize );
	void*		GetNextStream( void* pStream );

	static	FLStreamProtocol*	GetInstance();

public:

	MEMPOOLER_DECLARE( FLXORProtocol );

private:

	DWORD	m_dwProtocolId;
	BYTE	m_byRecvIndex;
	BYTE	m_bySendIndex;
	HANDLE	m_hInitWait;

};





#endif