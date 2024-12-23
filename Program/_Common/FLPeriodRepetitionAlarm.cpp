#include "Stdafx.h"

#include "FLPeriodRepetitionAlarm.h"



FLTm					IPeriodRepetitionAlarm::getStartDate() const
{
	return m_startDate;
}

FLTm					IPeriodRepetitionAlarm::getStartTime() const
{
	return m_startTime;
}

FLTm					IPeriodRepetitionAlarm::getEndDate() const
{
	return m_endDate;
}

FLTm					IPeriodRepetitionAlarm::getEndTime() const
{
	return m_endTime;
}

void IPeriodRepetitionAlarm::Update( const FLTm kCurrTm )
{
	if( m_checkTime == INVALID_DATETIME )
		return;

	if( m_checkTime <= kCurrTm )
		OnAlarm();
}


//----------------------------------------------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------------------------------------------//

FLPeriodRepetitionAlarm_RealDateTime::FLPeriodRepetitionAlarm_RealDateTime()
{
	m_bStartProcess		= FALSE;
	m_checkTime = INVALID_DATETIME;
	m_tTimeDistSecond	= 0;

	m_startDate			= INVALID_DATETIME;					
	m_endDate			= INVALID_DATETIME;

	m_startTime			= INVALID_DATETIME;			
	m_endTime			= INVALID_DATETIME;
}

FLPeriodRepetitionAlarm_RealDateTime::~FLPeriodRepetitionAlarm_RealDateTime()
{

}

BOOL FLPeriodRepetitionAlarm_RealDateTime::Initialize( const FLTm startDate, const FLTm endDate, const FLTm startTime, const FLTm endTime )
{
	// check arg
	if( startDate == INVALID_DATETIME || endDate == INVALID_DATETIME || startTime == INVALID_DATETIME || endTime == INVALID_DATETIME )
	{
		OnError();
		return FALSE;
	}

	if( startDate > endDate || startTime == endTime )
	{
		OnError();
		return FALSE;
	}

	if( m_bStartProcess == TRUE )		//�ߺ� �˶� ����
	{
		OnError();
		return FALSE;
	}

	// start time�� end time ���� ���
	const long startSecond	= FLTimeToSecond( startTime.hour, startTime.minute, startTime.second );
	long endSecond			= FLTimeToSecond( endTime.hour, endTime.minute, endTime.second );
	if( endTime < startTime )
		endSecond	+= FLTm::ONE_DAY_SECOND;

	const long tTimeDistSecond		= endSecond - startSecond;
	if( tTimeDistSecond <= 0 )
	{
		OnError();
		return FALSE;
	}

	// assign
	m_tTimeDistSecond	= tTimeDistSecond;
	m_startDate			= startDate;					
	m_endDate			= endDate;

	m_startTime			= startTime;			
	m_endTime			= endTime;
	return TRUE;
}

BOOL FLPeriodRepetitionAlarm_RealDateTime::Start()
{
	// check not initialize and prevent infinite loop
	if( m_tTimeDistSecond == 0 )		
	{
		OnError();
		return FALSE;
	}

	// �Ⱓ ���� üũ
	const FLTm tmCurr		= DATE_TIMER().GetCurrentTime();
	if( isExpired( tmCurr ) == TRUE )
	{
		OnExpired();
		return FALSE;
	}

	// ���� ���� �ð� ���
	const FLTm tmStart		= getStartAlarm_RealTime( tmCurr );
	if( tmStart == INVALID_DATETIME )
	{
		OnError();
		return FALSE;
	}

	// ���
	m_bStartProcess		= TRUE;
	m_checkTime			= tmStart;
	//DATETIME_ALARMCLOCK().Regist( m_realAlarmStartDateTime, this );

	DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ year(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), m_checkTime.year, m_checkTime.month, m_checkTime.day, m_checkTime.hour, m_checkTime.minute, m_checkTime.second ) );
	return TRUE;
}

void FLPeriodRepetitionAlarm_RealDateTime::OnAlarm()
{
	if( m_bStartProcess == TRUE )
	{
		if( OnStart() == FALSE )
		{
			OnError();
			return;
		}

		// ���� �˶� ���
		m_bStartProcess		= FALSE;
		FLTm endDateTime	= m_checkTime;
		if( FLAddDateTime( endDateTime, m_tTimeDistSecond ) == FALSE )
		{
			OnError();
			return;
		}
#ifdef _DEBUG
//		const FLTm tmCurr = DATE_TIMER().GetCurrentTime();
		FLTRACE_LOG( PROGRAM_NAME, _T( "[ End calc : dist(%d) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), m_tTimeDistSecond, endDateTime.year, endDateTime.month, endDateTime.day, endDateTime.hour, endDateTime.minute, endDateTime.second );
#endif //_DEBUG
		m_checkTime			= endDateTime;
	//	DATETIME_ALARMCLOCK().Regist( endDateTime, this );
	}
	else
	{
		if( OnEnd() == FALSE )
		{
			OnError();
			return;
		}

		// ���� üũ
		const FLTm tmCurr		= DATE_TIMER().GetCurrentTime();
		if( isExpired( tmCurr ) == TRUE )
		{
			OnExpired();
			return;
		}

		// ���� �˶� ���
		if( FLAddDateTime( m_checkTime, FLTm::ONE_DAY_SECOND - m_tTimeDistSecond ) == FALSE )
		{
			OnError();
			return;
		}
		m_bStartProcess		= TRUE;
		//Start();
	}
}

FLTm FLPeriodRepetitionAlarm_RealDateTime::getStartAlarm_RealTime( const FLTm & tmCurr ) /*���۵� datetime�̳� ���۵� datetime?������. */
{
	const FLTm tmCurrDate		= FLExtractDate( tmCurr );
	const FLTm tmCurrTime		= FLExtractTime( tmCurr );

	//���� üũ
	if( isExpired( tmCurr ) == TRUE )
	{
		return INVALID_DATETIME;
	}

	if( m_startTime == m_endTime )
	{
		return INVALID_DATETIME;
	}

	FLTm realStartDateTime;
	if( m_startTime < m_endTime )					//�ð� ������ �Ϸ縦 �Ѿ�� ������ ex) 09:00:00 ~ 13:00:00
	{
		if( tmCurrTime < m_endTime )						//���� Alarm �︱ ����
		{
			realStartDateTime		= FLMergeTm( tmCurrDate, m_startTime );

			//DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Starting rcurr 1 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, tmCurr.year, tmCurr.month, tmCurr.day, tmCurr.hour, tmCurr.minute, tmCurr.second ) );
			//DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Started calc  1 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, realStartDateTime.year, realStartDateTime.month, realStartDateTime.day, realStartDateTime.hour, realStartDateTime.minute, realStartDateTime.second ) );
		}
		else// if( startTime >= tmCurrTime )		//���� Alarm �︱ ����
		{
			realStartDateTime		= FLMergeTm( tmCurrDate, m_startTime );
			FLAddDateTime( realStartDateTime, FLTm::ONE_DAY_SECOND );

			//DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Starting rcurr 2 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, tmCurr.year, tmCurr.month, tmCurr.day, tmCurr.hour, tmCurr.minute, tmCurr.second ) );
			//DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Started calc  2 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, realStartDateTime.year, realStartDateTime.month, realStartDateTime.day, realStartDateTime.hour, realStartDateTime.minute, realStartDateTime.second ) );
		}
	}
	else //if( startTime > endTime )			//�ð� ������ �Ϸ縦 �Ѿ�� ex) 18:00:00 ~ 03:00:00
	{
		//���� �ð� ������ ���۵�
		if( /*FLStringToFLTm( "00:00:00", FLTm::E_FORMAT::HHMMSS ) < tmCurrTime && */ tmCurrTime <= m_endTime )
		{
			realStartDateTime		= FLMergeTm( tmCurrDate, m_startTime );
			FLAddDateTime( realStartDateTime, - FLTm::ONE_DAY_SECOND );

			//DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Starting rcurr 3 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, tmCurr.year, tmCurr.month, tmCurr.day, tmCurr.hour, tmCurr.minute, tmCurr.second ) );
			//DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Started calc  3 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, realStartDateTime.year, realStartDateTime.month, realStartDateTime.day, realStartDateTime.hour, realStartDateTime.minute, realStartDateTime.second ) );
		}
		else	//���� ���� �ð� �����̶��
		{
			realStartDateTime		= FLMergeTm( tmCurrDate, m_startTime );

			//DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Starting rcurr 4 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, tmCurr.year, tmCurr.month, tmCurr.day, tmCurr.hour, tmCurr.minute, tmCurr.second ) );
			//DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Started calc  4 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, realStartDateTime.year, realStartDateTime.month, realStartDateTime.day, realStartDateTime.hour, realStartDateTime.minute, realStartDateTime.second ) );
		}
	}

	return realStartDateTime;
}

BOOL					FLPeriodRepetitionAlarm_RealDateTime::isExpired( const FLTm & tmCurr )
{
	const FLTm tmCurrDate		= FLExtractDate( tmCurr );
	const FLTm tmCurrTime		= FLExtractTime( tmCurr );

	if( ( m_endDate == tmCurrDate && m_endTime <= tmCurrTime ) || m_endDate < tmCurrDate )
	{
		return TRUE;
	}

	return FALSE;
}

//---------------------------------------------------------------------------------------------------------------------------//



FLPeriodRepetitionAlarm_RealDateGameTime::FLPeriodRepetitionAlarm_RealDateGameTime() /*������ ���� ����. */
{
	m_bStartProcess			= FALSE;
	//m_AlarmStartGameTime = INVALID_DATETIME;
	m_tTimeDistGameSecond	= 0;

	m_startDate				= INVALID_DATETIME;					
	m_endDate				= INVALID_DATETIME;

	m_startTime				= INVALID_DATETIME;			
	m_endTime				= INVALID_DATETIME;
}

FLPeriodRepetitionAlarm_RealDateGameTime::~FLPeriodRepetitionAlarm_RealDateGameTime()
{

}

BOOL FLPeriodRepetitionAlarm_RealDateGameTime::Initialize( const FLTm startDate, const FLTm endDate, const FLTm startGameTime, const FLTm endGameTime ) /*������ ����. */
{
	// check arg
	if( startDate == INVALID_DATETIME || endDate == INVALID_DATETIME || startGameTime == INVALID_DATETIME || endGameTime == INVALID_DATETIME )
	{
		OnError();
		return FALSE;
	}

	if( startDate > endDate || startGameTime == endGameTime )
	{
		OnError();
		return FALSE;
	}

	if( m_bStartProcess == TRUE )		//�ߺ� �˶� ����
	{
		OnError();
		return FALSE;
	}

	// start time�� end time ���� ���
	const long startGameSecond	= FLTimeToSecond( startGameTime.hour, startGameTime.minute, startGameTime.second );
	long endGameSecond			= FLTimeToSecond( endGameTime.hour, endGameTime.minute, endGameTime.second );
	if( endGameTime < startGameTime )
		endGameSecond	+= FLTm::ONE_DAY_SECOND;

	const long tTimeDistGameSecond	= ( endGameSecond - startGameSecond );//* GAME_TIMER().GetMultiple();		//@@@@@@@@@@@@@@@@@@ /byMultiple
	if( tTimeDistGameSecond <= 0 )
	{
		OnError();
		return FALSE;
	}

	// assign
	m_tTimeDistGameSecond	= tTimeDistGameSecond;
	m_startDate				= startDate;					
	m_endDate				= endDate;

	m_startTime				= startGameTime;			
	m_endTime				= endGameTime;
	return TRUE;
}

BOOL FLPeriodRepetitionAlarm_RealDateGameTime::Start()
{
	// check not initialize and prevent infinite loop
	if( m_tTimeDistGameSecond == 0 )		
	{
		OnError();
		return FALSE;
	}

	// �Ⱓ ���� üũ
	const FLGameTimerCommon::FLGameTime gtmCurr	= GAME_TIMER().GetCurrentGameTime();
	const FLTm tmCurr						= GAME_TIMER().ToRealTime( gtmCurr );
	if( tmCurr == INVALID_DATETIME )
	{
		OnError();
		return FALSE;
	}

	if( isExpired( tmCurr ) == TRUE )
	{
		OnExpired();
		return FALSE;
	}

	// ���� ���� �ð� ���
	//FLGameTimerCommon::FLGameTime	m_AlarmStartGameTime;
	const BOOL bRet = getStartAlarm_GameDateTime( m_AlarmStartGameTime, gtmCurr );
	if( bRet == FALSE )
	{
		OnError();
		return FALSE;
	}

	// �ǽð����� ��ȯ
	const FLTm tmStart	= GAME_TIMER().ToRealTime( m_AlarmStartGameTime );
	if( tmStart == INVALID_DATETIME )
	{
		OnError();
		return FALSE;
	}

	if( isExpired( tmStart ) == TRUE )
	{
		OnExpired();
		return FALSE;
	}

	//end �ð��� ����ð��� ������?
	FLGameTimerCommon::FLGameTime gtmEnd	= m_AlarmStartGameTime;
	GAME_TIMER().AddSecond( gtmEnd, m_tTimeDistGameSecond );
	const FLTm endDateTime		= GAME_TIMER().ToRealTime( gtmEnd );
	if( endDateTime == INVALID_DATETIME )
	{
		OnError();
		return FALSE;
	}

	if( isExpired( endDateTime ) )
	{
		DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ End calc : (%u) dist(%d) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, m_tTimeDistGameSecond, endDateTime.year, endDateTime.month, endDateTime.day, endDateTime.hour, endDateTime.minute, endDateTime.second ) );
		OnExpired();
		return FALSE;
	}

	// ���
	m_bStartProcess			= TRUE;
	m_checkTime				= tmStart;
	//DATETIME_ALARMCLOCK().Regist( tmStart, this );
	return TRUE;
}

void FLPeriodRepetitionAlarm_RealDateGameTime::OnAlarm()
{
	if( m_bStartProcess == TRUE )
	{
		if( OnStart() == FALSE )
		{
			OnError();
			return;
		}

		// ���� �˶� ���
		m_bStartProcess				= FALSE;

		FLGameTimerCommon::FLGameTime gtmEnd	= m_AlarmStartGameTime;
		GAME_TIMER().AddSecond( gtmEnd, m_tTimeDistGameSecond );
		const FLTm endDateTime		= GAME_TIMER().ToRealTime( gtmEnd );
		if( endDateTime == INVALID_DATETIME )
		{
			OnError();
			return;
		}

#ifdef _DEBUG
//		const FLTm tmCurr = DATE_TIMER().GetCurrentTime();
		FLTRACE_LOG( PROGRAM_NAME, _T( "[ End calc : (%u) dist(%d) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, m_tTimeDistGameSecond, endDateTime.year, endDateTime.month, endDateTime.day, endDateTime.hour, endDateTime.minute, endDateTime.second );
#endif //_DEBUG
		m_checkTime				= endDateTime;
		//DATETIME_ALARMCLOCK().Regist( endDateTime, this );
	}
	else
	{
#ifdef _DEBUG
		FLGameTimerCommon::FLGameTime gtmCurr	= GAME_TIMER().GetCurrentGameTime();
		const FLTm tmCurrD = DATE_TIMER().GetCurrentTime();
		FLTRACE_LOG( PROGRAM_NAME, _T( "[ End r: (%u) dist(%d) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, m_tTimeDistGameSecond, tmCurrD.year, tmCurrD.month, tmCurrD.day, tmCurrD.hour, tmCurrD.minute, tmCurrD.second );
		FLTRACE_LOG( PROGRAM_NAME, _T( "[ End g: (%u) day(%u) hour(%u) minute(%u) second(%u) ]" ), (DWORD)this, gtmCurr.wAccumulatedDay, gtmCurr.time.hour, gtmCurr.time.minute, gtmCurr.time.second );
#endif //_DEBUG
		if( OnEnd() == FALSE )
		{
			OnError();
			return;
		}

		// ���� üũ
		const FLTm tmCurr		= DATE_TIMER().GetCurrentTime();
		if( isExpired( tmCurr ) == TRUE )
		{
			OnExpired();
			return;
		}

		//end �ð��� ����ð��� ������?
		++m_AlarmStartGameTime.wAccumulatedDay;

		FLGameTimerCommon::FLGameTime gtmEnd	= m_AlarmStartGameTime;
		GAME_TIMER().AddSecond( gtmEnd, m_tTimeDistGameSecond );
		const FLTm endDateTime		= GAME_TIMER().ToRealTime( gtmEnd );
		if( endDateTime == INVALID_DATETIME )
		{
			OnError();
			return;
		}

		if( isExpired( endDateTime ) )
		{
			OnExpired();
			return;
		}

		m_checkTime			= GAME_TIMER().ToRealTime( m_AlarmStartGameTime );
		if( m_checkTime == INVALID_DATETIME )
		{
			OnError();
			return;
		}

		m_bStartProcess		= TRUE;
	}
}

BOOL					FLPeriodRepetitionAlarm_RealDateGameTime::isExpired( const FLTm & tmRealCurr )
{
	const FLTm tmRealCurrDate		= FLExtractDate( tmRealCurr );
	if( /*( m_endDate == tmCurrDate && m_endGameTime <= tmCurrTime ) ||*/ m_endDate < tmRealCurrDate )	//�������� ������ ���ӳ�¥ ������ ���� �ð�
	{
		return TRUE;
	}

	return FALSE;
}

BOOL
FLPeriodRepetitionAlarm_RealDateGameTime::getStartAlarm_GameDateTime( FLGameTimerCommon::FLGameTime & o_AlarmStartGameTime, const FLGameTimerCommon::FLGameTime & tmGameCurr )
{
	////1 ���� ��¥�� �Ѿ�� �ʾ�����
	const FLTm tmCurr		= GAME_TIMER().ToRealTime( tmGameCurr );

#ifdef _DEBUG
	const FLTm curr			= DATE_TIMER().GetCurrentTime();
	FLTRACE_LOG( PROGRAM_NAME, _T( "[ compare r : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, curr.year, curr.month, curr.day, curr.hour, curr.minute, curr.second );
	FLTRACE_LOG( PROGRAM_NAME, _T( "[ compare g : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, tmCurr.year, tmCurr.month, tmCurr.day, tmCurr.hour, tmCurr.minute, tmCurr.second );
#endif //_DEBUG


	if( tmCurr == INVALID_DATETIME )
	{
		return FALSE;
	}
	if( isExpired( tmCurr ) == TRUE )
	{
		return FALSE;
	}

	if( m_startTime == m_endTime )
	{
		return FALSE;
	}

	o_AlarmStartGameTime	= tmGameCurr;
	if( m_startTime < m_endTime )					//�ð� ������ �Ϸ縦 �Ѿ�� ������ ex) 09:00:00 ~ 13:00:00
	{
		if( tmGameCurr.time < m_endTime )						//���� Alarm �︱ ����
		{
			o_AlarmStartGameTime.time		= m_startTime;

			//DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Starting rcurr 1 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, tmCurr.year, tmCurr.month, tmCurr.day, tmCurr.hour, tmCurr.minute, tmCurr.second ) );
			//DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Starting gcurr 1 : (%u) day(%u) hour(%u) minute(%u) second(%u) ]" ), (DWORD)this, tmGameCurr.wAccumulatedDay, tmGameCurr.time.hour, tmGameCurr.time.minute, tmGameCurr.time.second ) );
			//DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Started calc  1 : (%u) day(%u) hour(%u) minute(%u) second(%u) ]" ), (DWORD)this, o_AlarmStartGameTime.wAccumulatedDay, o_AlarmStartGameTime.time.hour, o_AlarmStartGameTime.time.minute, o_AlarmStartGameTime.time.second ) );
			//DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Started calc  1 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).year, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).month, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).day, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).hour, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).minute, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).second ) );

		}
		else// if( startTime >= tmCurrTime )		//���� Alarm �︱ ����
		{
			++o_AlarmStartGameTime.wAccumulatedDay;
			o_AlarmStartGameTime.time		= m_startTime;

			//DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Starting rcurr 1 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, tmCurr.year, tmCurr.month, tmCurr.day, tmCurr.hour, tmCurr.minute, tmCurr.second ) );
			//DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Starting gcurr 2 : (%u) day(%u) hour(%u) minute(%u) second(%u) ]" ), (DWORD)this, tmGameCurr.wAccumulatedDay, tmGameCurr.time.hour, tmGameCurr.time.minute, tmGameCurr.time.second ) );
			//DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Started calc  2 : (%u) day(%u) hour(%u) minute(%u) second(%u) ]" ), (DWORD)this, o_AlarmStartGameTime.wAccumulatedDay, o_AlarmStartGameTime.time.hour, o_AlarmStartGameTime.time.minute, o_AlarmStartGameTime.time.second ) );
			//DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Started calc  1 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).year, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).month, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).day, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).hour, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).minute, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).second ) );
		}
	}
	else //if( m_AlarmStartGameTime > m_endGameTime )			//�ð� ������ �Ϸ縦 �Ѿ�� ex) 18:00:00 ~ 03:00:00
	{
		//���� �ð� ������ ���۵�
		if( /*FLStringToFLTm( "00:00:00", FLTm::E_FORMAT::HHMMSS ) < tmCurrTime && */ tmGameCurr.time < m_endTime )			//@@@@@ 6���� ������ �������� �ִ�
		{
			--o_AlarmStartGameTime.wAccumulatedDay;		//@@@@@@@@ ����.
			o_AlarmStartGameTime.time		= m_startTime;

			//DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Starting rcurr 1 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, tmCurr.year, tmCurr.month, tmCurr.day, tmCurr.hour, tmCurr.minute, tmCurr.second ) );
			//DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Starting gcurr 3 : (%u) day(%u) hour(%u) minute(%u) second(%u) ]" ), (DWORD)this, tmGameCurr.wAccumulatedDay, tmGameCurr.time.hour, tmGameCurr.time.minute, tmGameCurr.time.second ) );
			//DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Started calc  3 : (%u) day(%u) hour(%u) minute(%u) second(%u) ]" ), (DWORD)this, o_AlarmStartGameTime.wAccumulatedDay, o_AlarmStartGameTime.time.hour, o_AlarmStartGameTime.time.minute, o_AlarmStartGameTime.time.second ) );
			//DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Started calc  1 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).year, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).month, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).day, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).hour, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).minute, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).second ) );
		}
		else	//���� ���� �ð� �����̶��
		{
			o_AlarmStartGameTime.time		= m_startTime;

			//DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Starting rcurr 1 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, tmCurr.year, tmCurr.month, tmCurr.day, tmCurr.hour, tmCurr.minute, tmCurr.second ) );
			//DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Starting gcurr 4 : (%u) day(%u) hour(%u) minute(%u) second(%u) ]" ), (DWORD)this, tmGameCurr.wAccumulatedDay, tmGameCurr.time.hour, tmGameCurr.time.minute, tmGameCurr.time.second ) );
			//DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Started calc  4 : (%u) day(%u) hour(%u) minute(%u) second(%u) ]" ), (DWORD)this, o_AlarmStartGameTime.wAccumulatedDay, o_AlarmStartGameTime.time.hour, o_AlarmStartGameTime.time.minute, o_AlarmStartGameTime.time.second ) );
			//DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Started calc  1 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).year, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).month, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).day, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).hour, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).minute, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).second ) );
		}
	}

	return TRUE;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------//
#include "../WorldServer/FLRandomRespawnNPC.hpp"
#include "../_Common/FLGameTimer.h"

FLPeriodRepetitionAlarm_Mng::FLPeriodRepetitionAlarm_Mng()
{
	this->LoadRandomRespawn();
}

namespace 
{
	// NPC ���� �дµ� �ʿ��� �ڷ���
	struct MonsterData
	{
		BOOL				bGameTime;
		FLTm				startTime;
		FLTm				endTime;
		DWORD				dwMoverIndex;
		BOOL				bActiveAttack;
	};

	struct DialogNPCData
	{
		BOOL				bGameTime;
		FLTm				startTime;
		FLTm				endTime;
		DWORD				dwMoverIndex;
		std::string			strCharacterKey;
	};
}


void					FLPeriodRepetitionAlarm_Mng::LoadRandomRespawn()
{
	const char * szFileName	= "NPC/PeriodRepetitionEvent.txt";		//���� ���� ����

	TCHAR szFullPath[ MAX_PATH ]	= { 0, };
	g_pScriptFileManager->GetScriptFileFullPath( szFullPath, _countof( szFullPath ), szFileName );

	CScript script;
	if( script.Load( szFullPath ) == FALSE )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "[ FAILED LOAD SCRIPT. FILE_NAME:(%s) ]" ), szFullPath );
		return;
	}

	while( script.tok != FINISHED )
	{
		// ���� : title
		script.GetToken();		//skip next
		const std::string strTitle	= script.Token;

		if( script.Token == "" )
			continue;

		// {
		script.GetToken();		//skip next
		if( script.Token != "{" )
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID TOKEN : pos({) (%s) ]" ), script.Token );
			return;
		}

		// 
		FLTm startDate			= INVALID_DATETIME;
		FLTm endDate			= INVALID_DATETIME;

		typedef					std::vector< MonsterData >	VecMonsterData;
		typedef					std::vector< DialogNPCData > VecDialogNPCData;

		VecMonsterData vecMonsterData;
		VecDialogNPCData vecDialogNPCData;

		// ���� ��ġ list
		FLRandomRespawnNPC_Data::VecRandomPos vecPos;

		for( ;; )
		{
			//�Ϸ� }
			script.GetToken();		//skip next
			if( script.Token == "}" )
				break;

			//�Ⱓ
			if( script.Token == "Period" )			//g_xFlyffConfig->GetMainLanguage() 
			{
				script.GetToken();											//"("
				RELEASE_ASSERT( script.Token == "(" );

				const int iLang		= script.GetNumber();					//LANG_KOR

				script.GetToken();	script.Token;							//","
				script.GetToken();	
				const FLTm sDate	= FLStringToFLTm( script.Token, FLTm::E_FORMAT::YYYYMMDD );		//start date
				if( sDate == INVALID_DATETIME )
					FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID DATA : startDate(%s) ]" ), script.Token );

				script.GetToken();	script.Token;							//","
				script.GetToken();	
				const FLTm eDate	= FLStringToFLTm( script.Token, FLTm::E_FORMAT::YYYYMMDD  );	//end date
				if( eDate == INVALID_DATETIME )
					FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID DATA : endDate(%s) ]" ), script.Token );

				script.GetToken();												//")"
				RELEASE_ASSERT( script.Token == ")" );
				script.GetToken();												//";"
				RELEASE_ASSERT( script.Token == ";" );

				// set period
				if( iLang == g_xFlyffConfig->GetMainLanguage() || iLang == LANG_MAX )
				{
					RELEASE_ASSERT( startDate == INVALID_DATETIME && endDate == INVALID_DATETIME ); //�ߺ� ���� üũ
					startDate		= sDate;
					endDate			= eDate;
				}
			}
			//�ð� �� data
			else if( script.Token == "AddMonster_RealTime" || script.Token == "AddMonster_GameTime" )
			{
				MonsterData data;
				data.bGameTime		= ( script.Token == "AddMonster_GameTime" );

				//read
				script.GetToken();												//"("
				RELEASE_ASSERT( script.Token == "(" );
				const int iLang		= script.GetNumber();						//LANG_KOR

				script.GetToken();	script.Token;								//","
				data.dwMoverIndex	= script.GetNumber();					// MI

				script.GetToken();	script.Token;								//","
				data.bActiveAttack	= ( script.GetNumber() != 0 );			//���� ����

				script.GetToken();	script.Token;								//","
				script.GetToken();	
				data.startTime		= FLStringToFLTm( script.Token, FLTm::E_FORMAT::HHMMSS );			//start time
				if( data.startTime == INVALID_DATETIME )
					FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID DATA : startTime(%s) ]" ), script.Token );

				script.GetToken();	script.Token;								//","
				script.GetToken();	
				data.endTime		= FLStringToFLTm( script.Token, FLTm::E_FORMAT::HHMMSS );			//end time
				if( data.endTime == INVALID_DATETIME )
					FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID DATA : endTime(%s) ]" ), script.Token );

				script.GetToken();												//")"
				RELEASE_ASSERT( script.Token == ")" );
				script.GetToken();												//";"
				RELEASE_ASSERT( script.Token == ";" );

				//add 	
				if( iLang == g_xFlyffConfig->GetMainLanguage() || iLang == LANG_MAX )
				{
					vecMonsterData.push_back( data );
				}
			}
			else if( script.Token == "AddDialogNPC_RealTime" || script.Token == "AddDialogNPC_GameTime" )
			{
				DialogNPCData data;
				data.bGameTime		= ( script.Token == "AddDialogNPC_GameTime" );

				//read
				script.GetToken();												//"("
				RELEASE_ASSERT( script.Token == "(" );
				const int iLang		= script.GetNumber();						//LANG_KOR

				script.GetToken();	script.Token;								//","
				data.dwMoverIndex	= script.GetNumber();					// MI

				script.GetToken();	script.Token;								//","
				script.GetToken();	
				data.strCharacterKey = script.Token;							//CharacterKey

				script.GetToken();	script.Token;								//","
				script.GetToken();	
				data.startTime		 = FLStringToFLTm( script.Token, FLTm::E_FORMAT::HHMMSS );			//start time
				if( data.startTime == INVALID_DATETIME )
					FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID DATA : startTime(%s) ]" ), script.Token );

				script.GetToken();	script.Token;								//","
				script.GetToken();	
				data.endTime		= FLStringToFLTm( script.Token, FLTm::E_FORMAT::HHMMSS );			//end time
				if( data.endTime == INVALID_DATETIME )
					FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID DATA : endTime(%s) ]" ), script.Token );

				script.GetToken();												//")"
				RELEASE_ASSERT( script.Token == ")" );
				script.GetToken();												//";"
				RELEASE_ASSERT( script.Token == ";" );

				//add 	
				if( iLang == g_xFlyffConfig->GetMainLanguage() || iLang == LANG_MAX )
				{
					// ��ȭ�� NPC DATA
					vecDialogNPCData.push_back( data );
				}
			}
			//��ġ
			else if( script.Token == "AddRandomPos" )
			{
				//read
				FLRandomRespawnNPC_Data::Pos pos;

				script.GetToken();											//"("
				RELEASE_ASSERT( script.Token == "(" );
				pos.worldID			= script.GetNumber();		//WorldIndex

				script.GetToken();	script.Token;				//","
				pos.pos.x			= ( float )script.GetNumber();//x

				script.GetToken();	script.Token;				//","
				pos.pos.y			= ( float )script.GetNumber();//y

				script.GetToken();	script.Token;				//","
				pos.pos.z			= ( float )script.GetNumber();//z

				script.GetToken();												//")"
				RELEASE_ASSERT( script.Token == ")" );
				script.GetToken();												//";"
				RELEASE_ASSERT( script.Token == ";" );

				// add
				vecPos.push_back( pos );
			}
			else
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID FUNTION (\"%s\") ]" ), script.Token );
			}
		}

		// �˶� ���
		if( startDate == INVALID_DATETIME || endDate == INVALID_DATETIME )			//��ϵ� �Ⱓ�� ����
			continue;

		typedef		FLRandomRespawnNPCAlarm< FLPeriodRepetitionAlarm_RealDateGameTime >		GameTimeAlarm;
		typedef		FLRandomRespawnNPCAlarm< FLPeriodRepetitionAlarm_RealDateTime >			RealTimeAlarm;

		for( VecMonsterData::iterator it = vecMonsterData.begin(); it != vecMonsterData.end(); ++it )
		{
			MonsterData & data	= *it;
			vecPos;
			if( data.bGameTime == TRUE )
			{
				GameTimeAlarm * p = new GameTimeAlarm;
				if( p->SetData( strTitle, startDate, endDate, data.startTime, data.endTime, data.dwMoverIndex, std::string(""), data.bActiveAttack, vecPos ) == TRUE )
					m_vecIAlarm.push_back( p );
				else
					delete p;
			}
			else
			{
				RealTimeAlarm * p = new RealTimeAlarm;
				if( p->SetData( strTitle, startDate, endDate, data.startTime, data.endTime, data.dwMoverIndex, std::string(""), data.bActiveAttack, vecPos ) == TRUE )
					m_vecIAlarm.push_back( p );
				else
					delete p;
			}
		}


		for( VecDialogNPCData::iterator it = vecDialogNPCData.begin(); it != vecDialogNPCData.end(); ++it )
		{
			DialogNPCData & data= *it;
			if( data.bGameTime == TRUE )
			{
				GameTimeAlarm * p = new GameTimeAlarm;
				if( p->SetData( strTitle, startDate, endDate, data.startTime, data.endTime, data.dwMoverIndex, data.strCharacterKey, FALSE, vecPos ) == TRUE )
					m_vecIAlarm.push_back( p );
				else
					delete p;
			}
			else
			{
				RealTimeAlarm * p = new RealTimeAlarm;
				if( p->SetData( strTitle, startDate, endDate, data.startTime, data.endTime, data.dwMoverIndex, data.strCharacterKey, FALSE, vecPos ) == TRUE )
					m_vecIAlarm.push_back( p );
				else
					delete p;
			}
		}

	}
}

//------------------------------------------------------------------------------------------------------------------//
FLPeriodRepetitionAlarm_Mng & FLPeriodRepetitionAlarm_Mng::GetInstance()
{
	static FLPeriodRepetitionAlarm_Mng inst;
	return inst;
}

FLPeriodRepetitionAlarm_Mng::~FLPeriodRepetitionAlarm_Mng()
{
	for( VecIAlarm::iterator it = m_vecIAlarm.begin(); it != m_vecIAlarm.end(); ++it )
	{
		SAFE_DELETE( *it );
	}
	m_vecIAlarm.clear();
}

void FLPeriodRepetitionAlarm_Mng::Start()
{
	for( VecIAlarm::iterator it = m_vecIAlarm.begin(); it != m_vecIAlarm.end(); ++it )
	{
		IPeriodRepetitionAlarm * p = *it;
		DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ 2 : %u ]" ), ( DWORD )p ) );
		p->Start();
	}
}

void FLPeriodRepetitionAlarm_Mng::Update()
{
	const FLTm tmCurr		= DATE_TIMER().GetCurrentTime();
	for( VecIAlarm::iterator it = m_vecIAlarm.begin(); it != m_vecIAlarm.end(); ++it )
	{
		IPeriodRepetitionAlarm * p = *it;
		p->Update( tmCurr );
	}
}



//
//
//FLPeriodRepetitionAlarm_RealDateTime::FLPeriodRepetitionAlarm_RealDateTime()
//{
//	m_bStartProcess		= FALSE;
//	m_realAlarmStartDateTime = INVALID_DATETIME;
//	m_tTimeDistSecond	= 0;
//
//	m_startDate			= INVALID_DATETIME;					
//	m_endDate			= INVALID_DATETIME;
//
//	m_startTime			= INVALID_DATETIME;			
//	m_endTime			= INVALID_DATETIME;
//}
//
//FLPeriodRepetitionAlarm_RealDateTime::~FLPeriodRepetitionAlarm_RealDateTime()
//{
//
//}
//
//BOOL FLPeriodRepetitionAlarm_RealDateTime::Initialize( const FLTm startDate, const FLTm endDate, const FLTm startTime, const FLTm endTime )
//{
//	// check arg
//	if( startDate == INVALID_DATETIME || endDate == INVALID_DATETIME || startTime == INVALID_DATETIME || endTime == INVALID_DATETIME )
//	{
//		OnError();
//		return FALSE;
//	}
//
//	if( startDate > endDate || startTime == endTime )
//	{
//		OnError();
//		return FALSE;
//	}
//
//	if( m_bStartProcess == TRUE )		//�ߺ� �˶� ����
//	{
//		OnError();
//		return FALSE;
//	}
//
//	// start time�� end time ���� ���
//	const long startSecond	= FLTimeToSecond( startTime.hour, startTime.minute, startTime.second );
//	long endSecond			= FLTimeToSecond( endTime.hour, endTime.minute, endTime.second );
//	if( endTime < startTime )
//		endSecond	+= FLTm::ONE_DAY_SECOND;
//
//	const long tTimeDistSecond		= endSecond - startSecond;
//	if( tTimeDistSecond <= 0 )
//	{
//		OnError();
//		return FALSE;
//	}
//
//	// assign
//	m_tTimeDistSecond	= tTimeDistSecond;
//	m_startDate			= startDate;					
//	m_endDate			= endDate;
//
//	m_startTime			= startTime;			
//	m_endTime			= endTime;
//	return TRUE;
//}
//
//BOOL FLPeriodRepetitionAlarm_RealDateTime::Start()
//{
//	// check not initialize and prevent infinite loop
//	if( m_tTimeDistSecond == 0 )		
//	{
//		OnError();
//		return FALSE;
//	}
//
//	// �Ⱓ ���� üũ
//	const FLTm tmCurr		= DATE_TIMER().GetCurrentTime();
//	if( isExpired( tmCurr ) == TRUE )
//	{
//		OnExpired();
//		return FALSE;
//	}
//
//	// ���� ���� �ð� ���
//	m_realAlarmStartDateTime		= getStartAlarm_RealTime( tmCurr );
//	if( m_realAlarmStartDateTime == INVALID_DATETIME )
//	{
//		OnError();
//		return FALSE;
//	}
//
//	// ���
//	m_bStartProcess		= TRUE;
//	DATETIME_ALARMCLOCK().Regist( m_realAlarmStartDateTime, this );
//	return TRUE;
//}
//
//void FLPeriodRepetitionAlarm_RealDateTime::OnAlarm()
//{
//	if( m_bStartProcess == TRUE )
//	{
//		if( OnStart() == FALSE )
//		{
//			OnError();
//			return;
//		}
//
//		// ���� �˶� ���
//		m_bStartProcess		= FALSE;
//		FLTm endDateTime	= m_realAlarmStartDateTime;
//		if( FLAddDateTime( endDateTime, m_tTimeDistSecond ) == FALSE )
//		{
//			OnError();
//			return;
//		}
//#ifdef _DEBUG
//		const FLTm tmCurr = DATE_TIMER().GetCurrentTime();
//		FLTRACE_LOG( PROGRAM_NAME, _T( "[ End calc : dist(%d) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), m_tTimeDistSecond, endDateTime.year, endDateTime.month, endDateTime.day, endDateTime.hour, endDateTime.minute, endDateTime.second );
//#endif //_DEBUG
//		DATETIME_ALARMCLOCK().Regist( endDateTime, this );
//	}
//	else
//	{
//		if( OnEnd() == FALSE )
//		{
//			OnError();
//			return;
//		}
//
//		// ���� üũ
//		const FLTm tmCurr		= DATE_TIMER().GetCurrentTime();
//		if( isExpired( tmCurr ) == TRUE )
//		{
//			OnExpired();
//			return;
//		}
//
//		// ���� �˶� ���
//		Start();
//	}
//}
//
//FLTm FLPeriodRepetitionAlarm_RealDateTime::getStartAlarm_RealTime( const FLTm & tmCurr ) /*���۵� datetime�̳� ���۵� datetime?������. */
//{
//	const FLTm tmCurrDate		= FLExtractDate( tmCurr );
//	const FLTm tmCurrTime		= FLExtractTime( tmCurr );
//
//	//���� üũ
//	if( isExpired( tmCurr ) == TRUE )
//	{
//		return INVALID_DATETIME;
//	}
//
//	if( m_startTime == m_endTime )
//	{
//		return INVALID_DATETIME;
//	}
//
//	FLTm realStartDateTime;
//	if( m_startTime < m_endTime )					//�ð� ������ �Ϸ縦 �Ѿ�� ������ ex) 09:00:00 ~ 13:00:00
//	{
//		if( tmCurrTime < m_endTime )						//���� Alarm �︱ ����
//		{
//			realStartDateTime		= FLMergeTm( tmCurrDate, m_startTime );
//
//			DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Starting rcurr 1 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, tmCurr.year, tmCurr.month, tmCurr.day, tmCurr.hour, tmCurr.minute, tmCurr.second ) );
//			DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Started calc  1 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, realStartDateTime.year, realStartDateTime.month, realStartDateTime.day, realStartDateTime.hour, realStartDateTime.minute, realStartDateTime.second ) );
//		}
//		else// if( startTime >= tmCurrTime )		//���� Alarm �︱ ����
//		{
//			realStartDateTime		= FLMergeTm( tmCurrDate, m_startTime );
//			FLAddDateTime( realStartDateTime, FLTm::ONE_DAY_SECOND );
//
//			DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Starting rcurr 2 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, tmCurr.year, tmCurr.month, tmCurr.day, tmCurr.hour, tmCurr.minute, tmCurr.second ) );
//			DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Started calc  2 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, realStartDateTime.year, realStartDateTime.month, realStartDateTime.day, realStartDateTime.hour, realStartDateTime.minute, realStartDateTime.second ) );
//		}
//	}
//	else //if( startTime > endTime )			//�ð� ������ �Ϸ縦 �Ѿ�� ex) 18:00:00 ~ 03:00:00
//	{
//		//���� �ð� ������ ���۵�
//		if( /*FLStringToFLTm( "00:00:00", FLTm::E_FORMAT::HHMMSS ) < tmCurrTime && */ tmCurrTime <= m_endTime )
//		{
//			realStartDateTime		= FLMergeTm( tmCurrDate, m_startTime );
//			FLAddDateTime( realStartDateTime, - FLTm::ONE_DAY_SECOND );
//
//			DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Starting rcurr 3 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, tmCurr.year, tmCurr.month, tmCurr.day, tmCurr.hour, tmCurr.minute, tmCurr.second ) );
//			DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Started calc  3 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, realStartDateTime.year, realStartDateTime.month, realStartDateTime.day, realStartDateTime.hour, realStartDateTime.minute, realStartDateTime.second ) );
//		}
//		else	//���� ���� �ð� �����̶��
//		{
//			realStartDateTime		= FLMergeTm( tmCurrDate, m_startTime );
//
//			DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Starting rcurr 4 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, tmCurr.year, tmCurr.month, tmCurr.day, tmCurr.hour, tmCurr.minute, tmCurr.second ) );
//			DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Started calc  4 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, realStartDateTime.year, realStartDateTime.month, realStartDateTime.day, realStartDateTime.hour, realStartDateTime.minute, realStartDateTime.second ) );
//		}
//	}
//
//	return realStartDateTime;
//}
//
//BOOL					FLPeriodRepetitionAlarm_RealDateTime::isExpired( const FLTm & tmCurr )
//{
//	const FLTm tmCurrDate		= FLExtractDate( tmCurr );
//	const FLTm tmCurrTime		= FLExtractTime( tmCurr );
//
//	if( ( m_endDate == tmCurrDate && m_endTime <= tmCurrTime ) || m_endDate < tmCurrDate )
//	{
//		return TRUE;
//	}
//
//	return FALSE;
//}
//
////---------------------------------------------------------------------------------------------------------------------------//
//
//
//
//FLPeriodRepetitionAlarm_RealDateGameTime::FLPeriodRepetitionAlarm_RealDateGameTime() /*������ ���� ����. */
//{
//	m_bStartProcess			= FALSE;
//	//m_AlarmStartGameTime = INVALID_DATETIME;
//	m_tTimeDistGameSecond	= 0;
//
//	m_startDate				= INVALID_DATETIME;					
//	m_endDate				= INVALID_DATETIME;
//
//	m_startTime				= INVALID_DATETIME;			
//	m_endTime				= INVALID_DATETIME;
//}
//
//FLPeriodRepetitionAlarm_RealDateGameTime::~FLPeriodRepetitionAlarm_RealDateGameTime()
//{
//
//}
//
//BOOL FLPeriodRepetitionAlarm_RealDateGameTime::Initialize( const FLTm startDate, const FLTm endDate, const FLTm startGameTime, const FLTm endGameTime ) /*������ ����. */
//{
//	// check arg
//	if( startDate == INVALID_DATETIME || endDate == INVALID_DATETIME || startGameTime == INVALID_DATETIME || endGameTime == INVALID_DATETIME )
//	{
//		OnError();
//		return FALSE;
//	}
//
//	if( startDate > endDate || startGameTime == endGameTime )
//	{
//		OnError();
//		return FALSE;
//	}
//
//	if( m_bStartProcess == TRUE )		//�ߺ� �˶� ����
//	{
//		OnError();
//		return FALSE;
//	}
//
//	// start time�� end time ���� ���
//	const long startGameSecond	= FLTimeToSecond( startGameTime.hour, startGameTime.minute, startGameTime.second );
//	long endGameSecond			= FLTimeToSecond( endGameTime.hour, endGameTime.minute, endGameTime.second );
//	if( endGameTime < startGameTime )
//		endGameSecond	+= FLTm::ONE_DAY_SECOND;
//
//	const long tTimeDistGameSecond	= ( endGameSecond - startGameSecond );//* GAME_TIMER().GetMultiple();		//@@@@@@@@@@@@@@@@@@ /byMultiple
//	if( tTimeDistGameSecond <= 0 )
//	{
//		OnError();
//		return FALSE;
//	}
//
//	// assign
//	m_tTimeDistGameSecond	= tTimeDistGameSecond;
//	m_startDate				= startDate;					
//	m_endDate				= endDate;
//
//	m_startTime				= startGameTime;			
//	m_endTime				= endGameTime;
//	return TRUE;
//}
//
//BOOL FLPeriodRepetitionAlarm_RealDateGameTime::Start()
//{
//	// check not initialize and prevent infinite loop
//	if( m_tTimeDistGameSecond == 0 )		
//	{
//		OnError();
//		return FALSE;
//	}
//
//	// �Ⱓ ���� üũ
//	const FLGameTimerCommon::FLGameTime gtmCurr	= GAME_TIMER().GetCurrentGameTime();
//	const FLTm tmCurr						= GAME_TIMER().ToRealTime( gtmCurr );
//	if( tmCurr == INVALID_DATETIME )
//	{
//		OnError();
//		return FALSE;
//	}
//
//	if( isExpired( tmCurr ) == TRUE )
//	{
//		OnExpired();
//		return FALSE;
//	}
//
//	// ���� ���� �ð� ���
//	const BOOL bRet = getStartAlarm_GameDateTime( m_AlarmStartGameTime, gtmCurr );
//	if( bRet == FALSE )
//	{
//		OnError();
//		return FALSE;
//	}
//
//	// �ǽð����� ��ȯ
//	const FLTm tmStart	= GAME_TIMER().ToRealTime( m_AlarmStartGameTime );
//	if( tmStart == INVALID_DATETIME )
//	{
//		OnError();
//		return FALSE;
//	}
//
//	if( isExpired( tmStart ) == TRUE )
//	{
//		OnExpired();
//		return FALSE;
//	}
//
//	//end �ð��� ����ð��� ������?
//	FLGameTimerCommon::FLGameTime gtmEnd	= m_AlarmStartGameTime;
//	GAME_TIMER().AddSecond( gtmEnd, m_tTimeDistGameSecond );
//	const FLTm endDateTime		= GAME_TIMER().ToRealTime( gtmEnd );
//	if( endDateTime == INVALID_DATETIME )
//	{
//		OnError();
//		return FALSE;
//	}
//
//	if( isExpired( endDateTime ) )
//	{
//		DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ End calc : (%u) dist(%d) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, m_tTimeDistGameSecond, endDateTime.year, endDateTime.month, endDateTime.day, endDateTime.hour, endDateTime.minute, endDateTime.second ) );
//		OnExpired();
//		return FALSE;
//	}
//
//	// ���
//	m_bStartProcess			= TRUE;
//	DATETIME_ALARMCLOCK().Regist( tmStart, this );
//	return TRUE;
//}
//
//void FLPeriodRepetitionAlarm_RealDateGameTime::OnAlarm()
//{
//	if( m_bStartProcess == TRUE )
//	{
//		if( OnStart() == FALSE )
//		{
//			OnError();
//			return;
//		}
//
//		// ���� �˶� ���
//		m_bStartProcess				= FALSE;
//
//		FLGameTimerCommon::FLGameTime gtmEnd	= m_AlarmStartGameTime;
//		GAME_TIMER().AddSecond( gtmEnd, m_tTimeDistGameSecond );
//		const FLTm endDateTime		= GAME_TIMER().ToRealTime( gtmEnd );
//		if( endDateTime == INVALID_DATETIME )
//		{
//			OnError();
//			return;
//		}
//
//#ifdef _DEBUG
//		const FLTm tmCurr = DATE_TIMER().GetCurrentTime();
//		FLTRACE_LOG( PROGRAM_NAME, _T( "[ End calc : (%u) dist(%d) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, m_tTimeDistGameSecond, endDateTime.year, endDateTime.month, endDateTime.day, endDateTime.hour, endDateTime.minute, endDateTime.second );
//#endif //_DEBUG
//		DATETIME_ALARMCLOCK().Regist( endDateTime, this );
//	}
//	else
//	{
//#ifdef _DEBUG
//		FLGameTimerCommon::FLGameTime gtmCurr	= GAME_TIMER().GetCurrentGameTime();
//		const FLTm tmCurrD = DATE_TIMER().GetCurrentTime();
//		FLTRACE_LOG( PROGRAM_NAME, _T( "[ End r: (%u) dist(%d) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, m_tTimeDistGameSecond, tmCurrD.year, tmCurrD.month, tmCurrD.day, tmCurrD.hour, tmCurrD.minute, tmCurrD.second );
//		FLTRACE_LOG( PROGRAM_NAME, _T( "[ End g: (%u) day(%u) hour(%u) minute(%u) second(%u) ]" ), (DWORD)this, gtmCurr.wAccumulatedDay, gtmCurr.time.hour, gtmCurr.time.minute, gtmCurr.time.second );
//#endif //_DEBUG
//		if( OnEnd() == FALSE )
//		{
//			OnError();
//			return;
//		}
//
//		// ���� üũ
//		const FLTm tmCurr		= DATE_TIMER().GetCurrentTime();
//		if( isExpired( tmCurr ) == TRUE )
//		{
//			OnExpired();
//			return;
//		}
//
//		// ���� �˶� ���
//		Start();
//	}
//}
//
//BOOL					FLPeriodRepetitionAlarm_RealDateGameTime::isExpired( const FLTm & tmRealCurr )
//{
//	const FLTm tmRealCurrDate		= FLExtractDate( tmRealCurr );
//	if( /*( m_endDate == tmCurrDate && m_endGameTime <= tmCurrTime ) ||*/ m_endDate < tmRealCurrDate )	//�������� ������ ���ӳ�¥ ������ ���� �ð�
//	{
//		return TRUE;
//	}
//
//	return FALSE;
//}
//
//BOOL
//FLPeriodRepetitionAlarm_RealDateGameTime::getStartAlarm_GameDateTime( FLGameTimerCommon::FLGameTime & o_AlarmStartGameTime, const FLGameTimerCommon::FLGameTime & tmGameCurr )
//{
//	////1 ���� ��¥�� �Ѿ�� �ʾ�����
//	const FLTm tmCurr		= GAME_TIMER().ToRealTime( tmGameCurr );
//
//#ifdef _DEBUG
//	const FLTm curr			= DATE_TIMER().GetCurrentTime();
//	FLTRACE_LOG( PROGRAM_NAME, _T( "[ compare r : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, curr.year, curr.month, curr.day, curr.hour, curr.minute, curr.second );
//	FLTRACE_LOG( PROGRAM_NAME, _T( "[ compare g : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, tmCurr.year, tmCurr.month, tmCurr.day, tmCurr.hour, tmCurr.minute, tmCurr.second );
//#endif //_DEBUG
//
//
//	if( tmCurr == INVALID_DATETIME )
//	{
//		return FALSE;
//	}
//	if( isExpired( tmCurr ) == TRUE )
//	{
//		return FALSE;
//	}
//
//	if( m_startTime == m_endTime )
//	{
//		return FALSE;
//	}
//
//	o_AlarmStartGameTime	= tmGameCurr;
//	if( m_startTime < m_endTime )					//�ð� ������ �Ϸ縦 �Ѿ�� ������ ex) 09:00:00 ~ 13:00:00
//	{
//		if( tmGameCurr.time < m_endTime )						//���� Alarm �︱ ����
//		{
//			o_AlarmStartGameTime.time		= m_startTime;
//
//			DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Starting rcurr 1 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, tmCurr.year, tmCurr.month, tmCurr.day, tmCurr.hour, tmCurr.minute, tmCurr.second ) );
//			DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Starting gcurr 1 : (%u) day(%u) hour(%u) minute(%u) second(%u) ]" ), (DWORD)this, tmGameCurr.wAccumulatedDay, tmGameCurr.time.hour, tmGameCurr.time.minute, tmGameCurr.time.second ) );
//			DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Started calc  1 : (%u) day(%u) hour(%u) minute(%u) second(%u) ]" ), (DWORD)this, o_AlarmStartGameTime.wAccumulatedDay, o_AlarmStartGameTime.time.hour, o_AlarmStartGameTime.time.minute, o_AlarmStartGameTime.time.second ) );
//			DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Started calc  1 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).year, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).month, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).day, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).hour, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).minute, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).second ) );
//
//		}
//		else// if( startTime >= tmCurrTime )		//���� Alarm �︱ ����
//		{
//			++o_AlarmStartGameTime.wAccumulatedDay;
//			o_AlarmStartGameTime.time		= m_startTime;
//
//			DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Starting rcurr 1 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, tmCurr.year, tmCurr.month, tmCurr.day, tmCurr.hour, tmCurr.minute, tmCurr.second ) );
//			DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Starting gcurr 2 : (%u) day(%u) hour(%u) minute(%u) second(%u) ]" ), (DWORD)this, tmGameCurr.wAccumulatedDay, tmGameCurr.time.hour, tmGameCurr.time.minute, tmGameCurr.time.second ) );
//			DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Started calc  2 : (%u) day(%u) hour(%u) minute(%u) second(%u) ]" ), (DWORD)this, o_AlarmStartGameTime.wAccumulatedDay, o_AlarmStartGameTime.time.hour, o_AlarmStartGameTime.time.minute, o_AlarmStartGameTime.time.second ) );
//			DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Started calc  1 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).year, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).month, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).day, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).hour, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).minute, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).second ) );
//		}
//	}
//	else //if( m_AlarmStartGameTime > m_endGameTime )			//�ð� ������ �Ϸ縦 �Ѿ�� ex) 18:00:00 ~ 03:00:00
//	{
//		//���� �ð� ������ ���۵�
//		if( /*FLStringToFLTm( "00:00:00", FLTm::E_FORMAT::HHMMSS ) < tmCurrTime && */ tmGameCurr.time < m_endTime )			//@@@@@ 6���� ������ �������� �ִ�
//		{
//			--o_AlarmStartGameTime.wAccumulatedDay;		//@@@@@@@@ ����.
//			o_AlarmStartGameTime.time		= m_startTime;
//
//			DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Starting rcurr 1 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, tmCurr.year, tmCurr.month, tmCurr.day, tmCurr.hour, tmCurr.minute, tmCurr.second ) );
//			DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Starting gcurr 3 : (%u) day(%u) hour(%u) minute(%u) second(%u) ]" ), (DWORD)this, tmGameCurr.wAccumulatedDay, tmGameCurr.time.hour, tmGameCurr.time.minute, tmGameCurr.time.second ) );
//			DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Started calc  3 : (%u) day(%u) hour(%u) minute(%u) second(%u) ]" ), (DWORD)this, o_AlarmStartGameTime.wAccumulatedDay, o_AlarmStartGameTime.time.hour, o_AlarmStartGameTime.time.minute, o_AlarmStartGameTime.time.second ) );
//			DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Started calc  1 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).year, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).month, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).day, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).hour, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).minute, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).second ) );
//		}
//		else	//���� ���� �ð� �����̶��
//		{
//			o_AlarmStartGameTime.time		= m_startTime;
//
//			DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Starting rcurr 1 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, tmCurr.year, tmCurr.month, tmCurr.day, tmCurr.hour, tmCurr.minute, tmCurr.second ) );
//			DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Starting gcurr 4 : (%u) day(%u) hour(%u) minute(%u) second(%u) ]" ), (DWORD)this, tmGameCurr.wAccumulatedDay, tmGameCurr.time.hour, tmGameCurr.time.minute, tmGameCurr.time.second ) );
//			DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Started calc  4 : (%u) day(%u) hour(%u) minute(%u) second(%u) ]" ), (DWORD)this, o_AlarmStartGameTime.wAccumulatedDay, o_AlarmStartGameTime.time.hour, o_AlarmStartGameTime.time.minute, o_AlarmStartGameTime.time.second ) );
//			DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ Started calc  1 : (%u) y(%I64u) month(%I64u) day(%I64u) hour(%I64u) minute(%I64u) second(%I64u) ]" ), (DWORD)this, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).year, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).month, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).day, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).hour, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).minute, GAME_TIMER().ToRealTime( o_AlarmStartGameTime ).second ) );
//		}
//	}
//
//	return TRUE;
//}
//
////---------------------------------------------------------------------------------------------------------------------------------------------------------//
//#include "../WorldServer/FLRandomRespawnNPC.hpp"
//#include "../_Common/FLGameTimer.h"
//
//FLPeriodRepetitionAlarm_Mng::FLPeriodRepetitionAlarm_Mng()
//{
//	this->LoadRandomRespawn();
//}
//
//namespace 
//{
//	// NPC ���� �дµ� �ʿ��� �ڷ���
//	struct MonsterData
//	{
//		BOOL				bGameTime;
//		FLTm				startTime;
//		FLTm				endTime;
//		DWORD				dwMoverIndex;
//		BOOL				bActiveAttack;
//	};
//
//	struct DialogNPCData
//	{
//		BOOL				bGameTime;
//		FLTm				startTime;
//		FLTm				endTime;
//		DWORD				dwMoverIndex;
//		std::string			strCharacterKey;
//	};
//}
//
//
//void					FLPeriodRepetitionAlarm_Mng::LoadRandomRespawn()
//{
//	const char * szFileName	= "NPC/PeriodRepetitionEvent.txt";		//���� ���� ����
//	CScript script;
//	if( script.Load( szFileName ) == FALSE )
//	{
//		FLERROR_LOG( PROGRAM_NAME, _T( "[ NOT EXIST : (%s) ]" ), szFileName );
//		return;
//	}
//
//	while( script.tok != FINISHED )
//	{
//		// ���� : title
//		script.GetToken();		//skip next
//		const std::string strTitle	= script.Token;
//
//		if( script.Token == "" )
//			continue;
//
//		// {
//		script.GetToken();		//skip next
//		if( script.Token != "{" )
//		{
//			FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID TOKEN : pos({) (%s) ]" ), script.Token );
//			return;
//		}
//
//		// 
//		FLTm startDate			= INVALID_DATETIME;
//		FLTm endDate			= INVALID_DATETIME;
//
//		typedef					std::vector< MonsterData >	VecMonsterData;
//		typedef					std::vector< DialogNPCData > VecDialogNPCData;
//
//		VecMonsterData vecMonsterData;
//		VecDialogNPCData vecDialogNPCData;
//
//		// ���� ��ġ list
//		FLRandomRespawnNPC_Data::VecRandomPos vecPos;
//
//		for( ;; )
//		{
//			//�Ϸ� }
//			script.GetToken();		//skip next
//			if( script.Token == "}" )
//				break;
//
//			//�Ⱓ
//			if( script.Token == "Period" )			//g_xFlyffConfig->GetMainLanguage() 
//			{
//				script.GetToken();											//"("
//				RELEASE_ASSERT( script.Token == "(" );
//
//				const int iLang		= script.GetNumber();					//LANG_KOR
//
//				script.GetToken();	script.Token;							//","
//				script.GetToken();	
//				const FLTm sDate	= FLStringToFLTm( script.Token, FLTm::E_FORMAT::YYYYMMDD );		//start date
//				if( sDate == INVALID_DATETIME )
//					FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID DATA : startDate(%s) ]" ), script.Token );
//
//				script.GetToken();	script.Token;							//","
//				script.GetToken();	
//				const FLTm eDate	= FLStringToFLTm( script.Token, FLTm::E_FORMAT::YYYYMMDD  );	//end date
//				if( eDate == INVALID_DATETIME )
//					FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID DATA : endDate(%s) ]" ), script.Token );
//
//				script.GetToken();												//")"
//				RELEASE_ASSERT( script.Token == ")" );
//				script.GetToken();												//";"
//				RELEASE_ASSERT( script.Token == ";" );
//
//				// set period
//				if( iLang == g_xFlyffConfig->GetMainLanguage() || iLang == LANG_MAX )
//				{
//					RELEASE_ASSERT( startDate == INVALID_DATETIME && endDate == INVALID_DATETIME ); //�ߺ� ���� üũ
//					startDate		= sDate;
//					endDate			= eDate;
//				}
//			}
//			//�ð� �� data
//			else if( script.Token == "AddMonster_RealTime" || script.Token == "AddMonster_GameTime" )
//			{
//				MonsterData data;
//				data.bGameTime		= ( script.Token == "AddMonster_GameTime" );
//
//				//read
//				script.GetToken();												//"("
//				RELEASE_ASSERT( script.Token == "(" );
//				const int iLang		= script.GetNumber();						//LANG_KOR
//
//				script.GetToken();	script.Token;								//","
//				data.dwMoverIndex	= script.GetNumber();					// MI
//
//				script.GetToken();	script.Token;								//","
//				data.bActiveAttack	= ( script.GetNumber() != 0 );			//���� ����
//
//				script.GetToken();	script.Token;								//","
//				script.GetToken();	
//				data.startTime		= FLStringToFLTm( script.Token, FLTm::E_FORMAT::HHMMSS );			//start time
//				if( data.startTime == INVALID_DATETIME )
//					FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID DATA : startTime(%s) ]" ), script.Token );
//
//				script.GetToken();	script.Token;								//","
//				script.GetToken();	
//				data.endTime		= FLStringToFLTm( script.Token, FLTm::E_FORMAT::HHMMSS );			//end time
//				if( data.endTime == INVALID_DATETIME )
//					FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID DATA : endTime(%s) ]" ), script.Token );
//
//				script.GetToken();												//")"
//				RELEASE_ASSERT( script.Token == ")" );
//				script.GetToken();												//";"
//				RELEASE_ASSERT( script.Token == ";" );
//
//				//add 	
//				if( iLang == g_xFlyffConfig->GetMainLanguage() || iLang == LANG_MAX )
//				{
//					vecMonsterData.push_back( data );
//				}
//			}
//			else if( script.Token == "AddDialogNPC_RealTime" || script.Token == "AddDialogNPC_GameTime" )
//			{
//				DialogNPCData data;
//				data.bGameTime		= ( script.Token == "AddDialogNPC_GameTime" );
//
//				//read
//				script.GetToken();												//"("
//				RELEASE_ASSERT( script.Token == "(" );
//				const int iLang		= script.GetNumber();						//LANG_KOR
//
//				script.GetToken();	script.Token;								//","
//				data.dwMoverIndex	= script.GetNumber();					// MI
//
//				script.GetToken();	script.Token;								//","
//				script.GetToken();	
//				data.strCharacterKey = script.Token;							//CharacterKey
//
//				script.GetToken();	script.Token;								//","
//				script.GetToken();	
//				data.startTime		 = FLStringToFLTm( script.Token, FLTm::E_FORMAT::HHMMSS );			//start time
//				if( data.startTime == INVALID_DATETIME )
//					FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID DATA : startTime(%s) ]" ), script.Token );
//
//				script.GetToken();	script.Token;								//","
//				script.GetToken();	
//				data.endTime		= FLStringToFLTm( script.Token, FLTm::E_FORMAT::HHMMSS );			//end time
//				if( data.endTime == INVALID_DATETIME )
//					FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID DATA : endTime(%s) ]" ), script.Token );
//
//				script.GetToken();												//")"
//				RELEASE_ASSERT( script.Token == ")" );
//				script.GetToken();												//";"
//				RELEASE_ASSERT( script.Token == ";" );
//
//				//add 	
//				if( iLang == g_xFlyffConfig->GetMainLanguage() || iLang == LANG_MAX )
//				{
//					// ��ȭ�� NPC DATA
//					vecDialogNPCData.push_back( data );
//				}
//			}
//			//��ġ
//			else if( script.Token == "AddRandomPos" )
//			{
//				//read
//				FLRandomRespawnNPC_Data::Pos pos;
//
//				script.GetToken();											//"("
//				RELEASE_ASSERT( script.Token == "(" );
//				pos.worldID			= script.GetNumber();		//WorldIndex
//
//				script.GetToken();	script.Token;				//","
//				pos.pos.x			= ( float )script.GetNumber();//x
//
//				script.GetToken();	script.Token;				//","
//				pos.pos.y			= ( float )script.GetNumber();//y
//
//				script.GetToken();	script.Token;				//","
//				pos.pos.z			= ( float )script.GetNumber();//z
//
//				script.GetToken();												//")"
//				RELEASE_ASSERT( script.Token == ")" );
//				script.GetToken();												//";"
//				RELEASE_ASSERT( script.Token == ";" );
//
//				// add
//				vecPos.push_back( pos );
//			}
//			else
//			{
//				FLERROR_LOG( PROGRAM_NAME, _T( "[ INVALID FUNTION (\"%s\") ]" ), script.Token );
//			}
//		}
//
//		// �˶� ���
//		if( startDate == INVALID_DATETIME || endDate == INVALID_DATETIME )			//��ϵ� �Ⱓ�� ����
//			continue;
//
//		typedef		FLRandomRespawnNPCAlarm< FLPeriodRepetitionAlarm_RealDateGameTime >		GameTimeAlarm;
//		typedef		FLRandomRespawnNPCAlarm< FLPeriodRepetitionAlarm_RealDateTime >			RealTimeAlarm;
//
//		for( VecMonsterData::iterator it = vecMonsterData.begin(); it != vecMonsterData.end(); ++it )
//		{
//			MonsterData & data	= *it;
//			vecPos;
//			if( data.bGameTime == TRUE )
//			{
//				GameTimeAlarm * p = new GameTimeAlarm;
//				if( p->SetData( strTitle, startDate, endDate, data.startTime, data.endTime, data.dwMoverIndex, std::string(""), data.bActiveAttack, vecPos ) == TRUE )
//					m_vecIAlarm.push_back( p );
//				else
//					delete p;
//			}
//			else
//			{
//				RealTimeAlarm * p = new RealTimeAlarm;
//				if( p->SetData( strTitle, startDate, endDate, data.startTime, data.endTime, data.dwMoverIndex, std::string(""), data.bActiveAttack, vecPos ) == TRUE )
//					m_vecIAlarm.push_back( p );
//				else
//					delete p;
//			}
//		}
//
//
//		for( VecDialogNPCData::iterator it = vecDialogNPCData.begin(); it != vecDialogNPCData.end(); ++it )
//		{
//			DialogNPCData & data= *it;
//			if( data.bGameTime == TRUE )
//			{
//				GameTimeAlarm * p = new GameTimeAlarm;
//				if( p->SetData( strTitle, startDate, endDate, data.startTime, data.endTime, data.dwMoverIndex, data.strCharacterKey, FALSE, vecPos ) == TRUE )
//					m_vecIAlarm.push_back( p );
//				else
//					delete p;
//			}
//			else
//			{
//				RealTimeAlarm * p = new RealTimeAlarm;
//				if( p->SetData( strTitle, startDate, endDate, data.startTime, data.endTime, data.dwMoverIndex, data.strCharacterKey, FALSE, vecPos ) == TRUE )
//					m_vecIAlarm.push_back( p );
//				else
//					delete p;
//			}
//		}
//
//	}
//}
//
//
//
//
//
//FLPeriodRepetitionAlarm_Mng::~FLPeriodRepetitionAlarm_Mng()
//{
//	for( VecIAlarm::iterator it = m_vecIAlarm.begin(); it != m_vecIAlarm.end(); ++it )
//		delete *it;
//}
//
//void FLPeriodRepetitionAlarm_Mng::Start()
//{
//	for( VecIAlarm::iterator it = m_vecIAlarm.begin(); it != m_vecIAlarm.end(); ++it )
//	{
//		IPeriodRepetitionAlarm * p = *it;
//		DEBUG_EXCUTE( FLTRACE_LOG( PROGRAM_NAME, _T( "[ 2 : %u ]" ), ( DWORD )p ) );
//		p->Start();
//	}
//}