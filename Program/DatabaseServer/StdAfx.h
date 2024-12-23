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

#define __DBSERVER

#pragma warning( disable : 6262 )
#define		_HAS_ITERATOR_DEBUGGING 0
#include <afx.h>
#include <afxwin.h>
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPO


#pragma	warning( push )
#pragma warning( disable : 4819 )
#include <d3dx9math.h>
#pragma	warning( pop )
#include <mmsystem.h>

#include <map>
#include <set>
#include <string>
#include <vector>
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

#include "../_CommonDefine/RefectoringDefine.h"
#include "..\_CommonDefine\FLResourceDefines.h"

#include <DefineCommon.h>
#include <CmnHdr.h>

#include "..\_CommonDefine\FLCommonLength.h"
#include "..\_CommonDefine\FLFlyffConfig.h"
#include "..\_CommonDefine\FlyffCommonDefine.h"
#include "..\_CommonDefine\FLPacketStruct.h"
#include "../_CommonDefine/Packet/FLPacketAllDefine.h"
#include "..\_Common\FLContentSwitch.h"
#include "../_Common/FLCommonCheck.h"
#include "..\_CommonDefine\FLReturnStateCode.h"

#include <vutil.h>
#include <DXUtil.h>
#include <xutil.h>
#include <path.h>
#include <Define.h>
#include <DefineNeuz.h>
#include <DefineJob.h>
#include <DefineAttribute.h>
#include <DefineWorld.h>
#include <resdata.h>


#include <Data.h>
#include <defineitemkind.h>
#include <defineItemGrade.h>
#include <defineItemType.h>
#include <file.h>
#include <authorization.h>
#include <Scanner.h>
#include <ScriptDialog.h>
#include "Project.h"
#include <Timer.h>
#include <d3dfont.h>
#include <Query.h>
#include <lang.h>
#ifdef __M_UTIL
#include "..\\Global\\M_Util.h"
#endif

//////////////////////////////////////////////////////////////////////////
// script file manager
#include "../_CommonDefine/FLScriptFileManager.h"
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	spec
//////////////////////////////////////////////////////////////////////////
#include "../_CommonDefine/Spec/FLSpecStruct.h"
#include "../_CommonDefine/Spec/FLSpecManager.h"
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// item container
#include "../_CommonDefine/FLItemContainer.h"
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Item Log
#include "../_CommonDefine/Log/FLItemLogStruct.h"
//////////////////////////////////////////////////////////////////////////



#include <DbManager.h>
#include <buyinginfo.h>




extern	HWND	hMainWnd;
extern	HINSTANCE	g_hInstance;

typedef	struct	AppInfo
{
	DWORD	dwSys;
	DWORD	dwId;		// server id
	int		nSerialNumberStatus;	// �ø���ѹ� ���� ����(�ش� ������ �۴ٸ� �ø���ѹ��� ���� �Ҵ��ؾ� �Ѵ�.)
}	APP_INFO, *LPAPP_INFO;

extern	char	DSN_NAME_CHARACTER01[260];
extern	char	DB_ADMIN_ID_CHARACTER01[260];
extern	char	DSN_NAME_LOG[260];
extern	char	DB_ADMIN_ID_LOG[260];

#ifdef __S1108_BACK_END_SYSTEM
extern	char	DSN_NAME_BACKSYSTEM[260];
extern	char	DB_ADMIN_ID_BACKSYSTEM[260];
#endif // __S1108_BACK_END_SYSTEM

extern	char	DSN_NAME_ITEMUPDATE[260];
extern	char	DB_ADMIN_ID_ITEMUPDATE[260];


struct	T_SLOT_ACTION_FLAG
{
	bool	bNotCreate;	// ���� �Ұ�
	bool	bNotDelete;	// ���� �Ұ�
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)