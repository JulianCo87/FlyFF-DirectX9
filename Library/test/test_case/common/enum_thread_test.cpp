
#include <gtest/gtest.h>

#include <common/FLEnumThread.h>
#include <common/FLEnumProcess.h>

TEST( GetFirst, EnumThread )
{
	FLEnumThread cEnumThread;
	DWORD dwProcessID = ::GetCurrentProcessId();

	EXPECT_TRUE( cEnumThread.Open() );

	int nThreadCount = 0;
	DWORD dwThreadID = cEnumThread.GetFirst( dwProcessID );
	while( dwThreadID != 0 )
	{
		++nThreadCount;
		dwThreadID = cEnumThread.GetNext( dwProcessID );
	}

	cEnumThread.Close();

	EXPECT_TRUE( nThreadCount > 0 );	// �ּ� main ������� �ִ�.
}

TEST( GetProcess, EnumProcess )
{
	FLEnumProcess kEnumProcess;

	EXPECT_TRUE( kEnumProcess.Open() );

	const TCHAR* szProcess = kEnumProcess.GetFirst();
	while( szProcess != NULL )
	{
		szProcess = kEnumProcess.GetNext();
	}

	kEnumProcess.Close();
}
