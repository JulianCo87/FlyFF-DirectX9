// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define	_WIN32_WINNT	0x0501
#define	WINVER			0x0501

#if	defined( _DLL ) && defined( _MT )
	#define	_AFXDLL
#endif

#define __ACCOUNTSERVER

#define		_HAS_ITERATOR_DEBUGGING 0
// Windows Header Files:

#pragma warning ( disable : 6262 )

#include <afxwin.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>


#include <tchar.h>
#include <mmsystem.h>



#pragma	warning( push )
#pragma warning( disable : 4819 )	
#include <d3dx9tex.h>
#pragma	warning( pop )



#include <afxdisp.h>        // MFC Automation classes

#include <map>
#include <set>
#include <list>
#include <vector>
#include <string>
using	namespace	std;



//#define	FLASSERT_ENABLE
//#define	FLASSERT_HOOKER_ENABLE

#ifndef	_DEBUG
#define	__FLLOG_EXCLUDE_DEBUG_TRACE
#endif

extern	TCHAR	PROGRAM_NAME[128];
extern	TCHAR	NETWORK_LOG_NAME[128];




#include <common/FLStringFunction.h>
#include <thread/FLCriticalSection.h>
#include <thread/FLAutoLock.h>
#include <logging/FLLogFacade.h>
#include <network/FLNetworkCommon.h>


#include "VersionCommon.h"
#include "..\\Global\\FLVersionAll.h"

#include <DefineCommon.h>
#include <cmnhdr.h>


#include "..\_CommonDefine\FLCommonLength.h"
#include "../_CommonDefine/RefectoringDefine.h"
#include "..\_CommonDefine\FLFlyffConfig.h"
#include "..\_CommonDefine\FlyffCommonDefine.h"
#include "..\_CommonDefine\FLPacketStruct.h"
#include "..\_CommonDefine\FLCommonStruct.h"
#include "../_CommonDefine/Packet/FLPacketAllDefine.h"
#include "..\_Common\FLContentSwitch.h"
#include "..\_CommonDefine\FLReturnStateCode.h"

#include <dxutil.h>



#include <vutil.h>
#include <data.h>
#include <file.h>
#include <d3dfont.h>
#include <scanner.h>
#include <timer.h>
#include <xutil.h>



#include <buyinginfo.h>

#ifdef __M_UTIL
#include "..\\Global\\M_Util.h"
#endif

const UINT	IDT_SENDPLAYERCOUNT	= 0;
const UINT	IDT_RECONNECT		= 1;
const UINT  IDT_KEEPALIVE		= 2;
const UINT  IDT_KICKOUT			= 3;
const UINT  IDT_PREVENT_EXCESS	= 4;
const UINT	IDT_BUYING_INFO		= 5;
const UINT  IDT_TIME_CHECKADDR	= 7;


const UINT		WM_CONNENT_BILLING = WM_USER + 100;

extern	char	DSN_NAME_LOG[256];
extern	char	DB_ADMIN_ID_LOG[256];

extern	char	DSN_NAME_LOGIN[256];
extern	char	DB_ADMIN_ID_LOGIN[256];

extern	char	DSN_NAME_BILLING[256];
extern	char	DB_ADMIN_ID_BILLING[256];


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
