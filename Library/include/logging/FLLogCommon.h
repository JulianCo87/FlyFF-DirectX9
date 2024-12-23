
#ifndef	__FLLOGCOMMON_H__
#define	__FLLOGCOMMON_H__


#include "../common/FLCommon.h"

enum
{
	FLLOG_MAX_LOG_NAME		= 128,
	FLLOG_MAX_LOG_STRING	= 8*1024,
};


enum
{
	FLLOG_ALL,		// ��� �α� ���

	FLLOG_DEBUG,
	FLLOG_TRACE,
	FLLOG_INFO,
	FLLOG_WARNING,
	FLLOG_ERROR,
	FLLOG_FAULT,

	FLLOG_NONE,		// ��� �α� ��� ����
};


const TCHAR*	GetLogLevelString( int nLevel );




#endif