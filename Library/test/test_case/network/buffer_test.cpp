
#include <gtest/gtest.h>
#include <network/FLBuffer.h>


TEST( Write, buffer )
{
	CBufferQueue queue;

	char dummy[MAX_BUFFER-1];
	EXPECT_TRUE( queue.Write( dummy, sizeof( dummy ) ) );

	char over[10];
	EXPECT_TRUE( queue.Write( over, sizeof( over ) ) );
}

TEST( DropHead, buffer )
{
	CBuffer buff( 100 );

	char dummy[50];
	EXPECT_TRUE( buff.Write( dummy, sizeof( dummy ) ) );

	int readable = buff.GetReadableBufferSize();

	EXPECT_TRUE( readable == sizeof( dummy ) );

	readable -= 20;
	EXPECT_TRUE( (buff.GetReadableBufferSize() - readable) == 20 );
	buff.DropHead( buff.GetReadableBufferSize() - readable );

	EXPECT_TRUE( buff.GetReadableBufferSize() == 30 );
}

TEST( AddTail, BufferQueue )
{
	CBufferQueue a;

	for( int i = 0; i < 10; ++i )
	{
		CBuffer* pBuffer = CBufferFactory::GetInstance().CreateBuffer( MAX_BUFFER );
		a.AddTail( pBuffer );
	}

	EXPECT_TRUE( a.GetCount() == 10 );

	CBufferQueue b;
	b.AddTail( &a );

	EXPECT_TRUE( a.GetCount() == 0 );
	EXPECT_TRUE( b.GetCount() == 10 );

	for( int i = 0; i < 2; ++i )
	{
		CBuffer* pBuffer = CBufferFactory::GetInstance().CreateBuffer( MAX_BUFFER );
		a.AddTail( pBuffer );
	}

	EXPECT_TRUE( a.GetCount() == 2 );

	a.AddTail( &b );

	EXPECT_TRUE( a.GetCount() == 12 );

	int count = 0;

	while( true )
	{
		CBuffer* pBuffer = a.PopHead();
		if( pBuffer == NULL )
		{
			break;
		}

		delete pBuffer;
		++count;
	}

	EXPECT_TRUE( count == 12 );
}