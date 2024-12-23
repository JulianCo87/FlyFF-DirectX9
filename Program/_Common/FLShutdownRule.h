#pragma once

class FLShutdownRule
{
public:		//�������̸� ���߿� spec���� ����
	static const int		SHUTDOWN_AGE		= 16;
	static const int		SHUTDOWN_START_HOUR = 0;
	static const int		SHUTDOWN_END_HOUR	= 6;

public:
	FLShutdownRule();
	~FLShutdownRule();

	static bool				InShutDownRule( const CTime & tmBirthDate );

	static bool				IsShutDownAge( const CTime & tmBirthDate );
	static bool				IsShutDownTime();
	
};

