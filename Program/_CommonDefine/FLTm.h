#ifndef __FLTM_H__
#define __FLTM_H__

union FLTm;

#include "../_CommonDefine/FLSingleton.h"

//neodeath comment
//	: DateTime �� Date �� Time���� �и�
//	: distance ���ϱ� �Լ� �߰� (second ����)
//	: time_t �� ���������� 3000�� 12�� 31�� 23�� 59�� 59�ʰ� �ִ�ġ ( time_t�� ȣȯ�� �� �ش� �Լ��鿡 �����ϹǷ� )

//localtime : time_t to tm
//time		: get time
//_mktime64 : tm to time_t




class FLDateTimer// : public FLSingleton< FLDateTimer >
{
public:
	FLDateTimer();
	virtual ~FLDateTimer();

	static FLDateTimer &	GetInstance()
	{
		static FLDateTimer inst;
		return inst;
	}

	FLTm					GetCurrentTime();
	WORD					GetMultiple() const;

	
	WORD					GetTimeSpeed_DebugMode() const;
	//Debug ��忡���� ���. �ȱ׷��� ������ �ð����� ���� �ð� �Ѱ�ġ�� �Ѿ.
	void					SetTimeSpeed_DebugMode( const WORD wMultiple );		//���� �ð����� byMultiple ��� ��ŭ �ð��� ���� ���ư�.

private:
	__time64_t				m_tStart;
	WORD					m_wMultiple;
};

inline static FLDateTimer &	DATE_TIMER()		{		return FLDateTimer::GetInstance();			}

union FLTm
{
	enum
	{
		ONE_MINUTE_SECOND	= 60,
		ONE_HOUR_SECOND		= 60 * ONE_MINUTE_SECOND,
		ONE_DAY_SECOND		= 24 * ONE_HOUR_SECOND,
		ONE_YEAR_SECOND		= 365 * ONE_DAY_SECOND,
	};

	enum 		
	{
		DATETIME_STRING_MAX = 20,		//"2000-09-10 16:00:00"
		DATEMIN_STRING_MAX	= 17,		//"2000-09-10 16:00"
		DATE_STRING_MAX		= 11,		//"2000-09-10"
		TIME_STRING_MAX		= 9,		//"16:00:00"
	};

	struct E_FORMAT	{ enum E{
		YYYYMMDD_HHMMSS = 0,
		YYYYMMDD_HHMM,
		YYYYMMDD,
		HHMMSS, 
	};			};

	struct 
	{
		unsigned __int64	padding		: 24;	//	�е�
		//unsigned __int64	milisec		: 10;		
	
		unsigned __int64	second		: 6;	
		unsigned __int64	minute		: 6;	
		unsigned __int64	hour		: 5;	
		
		unsigned __int64	day			: 5;	
		unsigned __int64	month		: 4;	
		unsigned __int64	year		: 14;	
	};

	struct 
	{
		unsigned __int64	value;
	};

	bool operator == ( const FLTm & tm ) const	{	return ( value == tm.value	);	}
	bool operator != ( const FLTm & tm ) const	{	return ( value != tm.value	);	}
	bool operator > ( const FLTm & tm )	const	{	return ( value > tm.value	);	}
	bool operator >= ( const FLTm & tm ) const	{	return ( value >= tm.value	);	}
	bool operator < ( const FLTm & tm ) const	{	return ( value < tm.value	);	}
	bool operator <= ( const FLTm & tm ) const 	{	return ( value <= tm.value	);	}
};


// calculation
BOOL						FLAddDateTime( FLTm & o_fltm, const long nSecond );				// YYYYMMDD�� HHMMSS ������ ������ �ȵ�. 
//__time64_t					FLGetDistance( const FLTm & left, const FLTm & right );

// convert 
BOOL						FLDateTimeToTimeT( __time64_t & o_timet, const FLTm fltm );			// YYYYMMDD�� HHMMSS ������ ������ �ȵ�. ���������� _mktime64�� ���Ƿ�
BOOL						FLTimeTToDateTime( FLTm & o_fltm, const __time64_t t );
FLTm						FLStringToFLTm( const TCHAR * szDateTime, const FLTm::E_FORMAT::E eStrFormat );
tstring						FLTmToString( const FLTm & tm, const FLTm::E_FORMAT::E eFormat );		
long						FLTimeToSecond( const long hour, const long minute, const long second );
//UINT64						FLTimeToSecond( const long year, const long hour, const long minute, const long second );

// extract
FLTm						FLExtractDate( const FLTm & fltm );
FLTm						FLExtractTime( const FLTm & fltm );

// merge
FLTm						FLMergeTm( const FLTm & tmDateArea, const FLTm & tmTimeArea );

// compare
//BOOL						FLLessThanTimeOnly( const FLDateTime::FLTm & src, const FLDateTime::FLTm & dst );

// constant
FLTm						FLInvalidDateTime();

#define						INVALID_DATETIME		FLInvalidDateTime()

CTime						FLStringToCTime( const char * szDateTime, const FLTm::E_FORMAT::E eFormat );
int							FLDateStringToInt( const char * szDateTime );


//const char CTIME_MIN_STR[]		= "197001011200";
//const int CTIME_MIN				= atoi( CTIME_MIN_STR );
//
const char CTIME_DATE_MIN_STR[]	= "1970-01-02";
const int CTIME_DATE_MIN		= 19700102;


#endif //__FLTM_H__



