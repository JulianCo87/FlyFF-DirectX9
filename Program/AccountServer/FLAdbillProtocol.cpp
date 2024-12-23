
#include "stdafx.h"

#include "FLAdbillProtocol.h"
#include "../_Common/buyinginfo.h"


#pragma pack( 1 )

// �ý��� �޽����� �ֱ����� ����� �ƴ� ���� �ش�
// ���������θ� ����ϸ� �ܺη� Send �� ��� �߰��ؼ��� �ȵȴ�.
struct	T_DUMMY_HEADER
{
	u_long	nSize;	// header ������
};

#pragma pack()

FLStreamProtocol*	FLAdbillProtocol::GetInstance()
{
	static FLAdbillProtocol xAdbillProtocol;
	return &xAdbillProtocol;
}

FLAdbillProtocol::FLAdbillProtocol()
{
}

FLAdbillProtocol::~FLAdbillProtocol()
{
}

bool	FLAdbillProtocol::FetchStream( CBuffer** pBuffer, CBufferQueue* pQueue )
{
	// ���� �ش��� �߰��ϱ� ���� ���� �����͸� ���ο� ���۷� �����ϰ�
	// ���� ������ �����͸� ������ ����.

	u_long nRemnant;
	LPBYTE ptr	= (*pBuffer)->GetReadableBuffer( &nRemnant );
	CBuffer* pCompleted = NULL;

	while( 1 )
	{
		// �ش��� ������ �� ���� ���߰ų� ���ۿ� ������ ���ٸ�
		if( nRemnant < sizeof( BUYING_INFO ) )
		{
			return true;
		}
		else	// �� �޾Ҵ�.
		{
			if( pCompleted == NULL || pCompleted->GetWritableBufferSize() < (sizeof( T_DUMMY_HEADER ) + sizeof( BUYING_INFO )) )
			{
				pCompleted = CBufferFactory::GetInstance().CreateBuffer( MAX_BUFFER );
				if( pCompleted == NULL )
				{
					return false;
				}

				pQueue->AddTail( pCompleted );
			}

			// ���� �ش� ����
			T_DUMMY_HEADER tHeader;
			tHeader.nSize = sizeof( BUYING_INFO );

			/*
			T_C_BUFFER_INFO tBufferInfo[2];
			tBufferInfo[0].pBuffer = &tHeader;
			tBufferInfo[0].nSize = sizeof( tHeader );

			tBufferInfo[1].pBuffer = ptr;
			tBufferInfo[1].nSize = sizeof( BUYING_INFO );

			pQueue->WriteToOneBuffer( tBufferInfo, 2 );
			*/

			pCompleted->Write( &tHeader, sizeof( tHeader ) );
			pCompleted->Write( ptr, sizeof( BUYING_INFO ) );

			pCompleted->IncBlockCount();

			nRemnant	-= sizeof( BUYING_INFO );
			ptr			+= sizeof( BUYING_INFO );
		}
	}

	return true;
}

bool	FLAdbillProtocol::PushSystemStream( CBuffer* pBuffer, LPBYTE lpData, u_long uDataSize )
{
	T_DUMMY_HEADER tHeader;
	tHeader.nSize = uDataSize;

	if( pBuffer->Write( &tHeader, sizeof( tHeader ) ) == false )	return false;
	if( pBuffer->Write( lpData, uDataSize ) == false )	return false;

	return true;
}

bool	FLAdbillProtocol::SendStream( CBufferQueue* pQueue, LPBYTE lpData, u_long uDataSize )
{
	if( pQueue->Write( lpData, uDataSize ) == false )
	{
		return false;
	}

	return true;
}

FLStreamProtocol*	FLAdbillProtocol::Clone()
{
	return this;
}

void	FLAdbillProtocol::Release()
{
}

void	FLAdbillProtocol::GetData( void* pStream, void** pData, u_long* nDataSize )
{
	T_DUMMY_HEADER* pHeader = reinterpret_cast< T_DUMMY_HEADER* >( pStream );
	*pData = pHeader+1;
	*nDataSize = pHeader->nSize;
}

void*	FLAdbillProtocol::GetNextStream( void* pStream )
{
	T_DUMMY_HEADER* pHeader = reinterpret_cast< T_DUMMY_HEADER* >( pStream );
	char* pStart = reinterpret_cast< char* >( pHeader );
	return pStart + sizeof( *pHeader ) + pHeader->nSize;
}

