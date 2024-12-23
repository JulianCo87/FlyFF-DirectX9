#include <gtest/gtest.h>

//#define	__FL_NOT_LOGGING
#include <logging/FLLogFacade.h>

TEST( AddLog, logging )
{
	EXPECT_TRUE( FLLogFacade::GetInstance()->Add( _T( "test" ) ) );
	FLLOG_ADD( _T( "test2" ) );
}

TEST( PushLog, logging )
{
	FLDEBUG_LOG( _T( "test" ), _T( "debug log, %s" ), _T( "test message" ) );
	FLWARNING_LOG( _T( "notcreated" ), _T( "warning log.. %d" ), 1 );
}

/*
TEST( ChangeLevel, logging )
{
	EXPECT_TRUE( FLLogFacade::GetInstance()->ChangeLevel( _T( "test" ), FLLOG_ERROR ) == FLLOG_ALL );
	FLWARNING_LOG( _T( "test" ), _T( "warning noprint," ) );
	FLERROR_LOG( _T( "test" ), _T( "error \"print\"," ) );

	Sleep( 1000 );
	FLLOG_LEVEL( _T( "test" ), FLLOG_ALL );
	FLDEBUG_LOG( _T( "test" ), _T( "debug print." ) );
}
*/

/*
TEST( AddPublisher, logging )
{
	Sleep( 1000 );

	FLLogPubMessageBox cMessageBoxPublisher;

	FLLOG_ADD_PUBLISHER( _T( "test" ), &cMessageBoxPublisher );

	FLWARNING_LOG( _T( "test" ), _T( "this message show message box. 1" ) );
	FLERROR_LOG( _T( "test" ), _T( "this message show message box. 2" ) );

	Sleep( 1000 );
	FLLOG_DEL_PUBLISHER( _T( "test" ), &cMessageBoxPublisher );

	FLERROR_LOG( _T( "test" ), _T( "this message is not show message box. 3" ) );
}
*/


/*
TEST( StressTest, logging )
{
	int nCount = 0;
	while( true )
	{
		++nCount;
		FLDEBUG_LOG( _T( "test" ), ( _T( "qwertyuiop[];lkjhgfdsazxcvbnm,.(%d)" ), nCount ) );

		//if( (nCount % 128) == 0 )
		{
			//::Sleep( 1 );
		}
	}
}
*/
