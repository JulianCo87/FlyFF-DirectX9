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

#define __CERTIFIER

#define		_HAS_ITERATOR_DEBUGGING 0

#pragma warning ( disable : 6262 )

#include <afxwin.h>
#include <mmsystem.h>
#include <afxdisp.h>        // MFC Automation classes

#include <list>
#include <map>
#include <vector>
#include <string>
using namespace std;

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
#include <CmnHdr.h>

#include "..\_CommonDefine\FLCommonLength.h"
#include "../_CommonDefine/RefectoringDefine.h"
#include "..\_CommonDefine\FLFlyffConfig.h"
#include "..\_CommonDefine\FLReturnStateCode.h"


#include <DXUtil.h>
#include <data.h>
#include <Query.h>
#include <file.h>
#include <scanner.h>
#pragma	warning( push )
#pragma warning( disable : 4819 )	
#include <d3dfont.h>
#pragma	warning( pop )
#include <timer.h>
#include <xutil.h>
#include <vutil.h>


#include "../_Common/FLContentSwitch.h"
#include "..\_CommonDefine\FlyffCommonDefine.h"
#include "..\_CommonDefine\FLPacketStruct.h"
#include "../_CommonDefine/Packet/FLPacketAllDefine.h"
#ifdef __M_UTIL
#include "..\\Global\\M_Util.h"
#endif
extern	BOOL	g_fInternal;


//////////////////////////////////////////////////////////////////////////
//	mulcom	BEGIN100218	�н����� ��ȣȭ.
#include <crypto/FLRijndael.h>
//	mulcom	END100218	�н����� ��ȣȭ.
//////////////////////////////////////////////////////////////////////////



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)