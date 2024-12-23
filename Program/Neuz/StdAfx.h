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
#define _WIN32_WINNT 0x0501

#if	defined( _DLL ) && defined( _MT )
	#define	_AFXDLL
#endif

#define		__CLIENT
#define		_HAS_ITERATOR_DEBUGGING 0
   
// Windows or Standard        
#include <afxwin.h>      
#include <AFXTEMPL.H>        // MFC Automation classes  
//#include <xdebug>   
#include <winuser.h>    
#include <basetsd.h>  
#include <memory.h>  
#include <mmsystem.h>   
#include <string>   
#include <string.h> 
#include <stdio.h>  
#include <tchar.h>  
#include <time.h> 
#include <stdlib.h>
#include <process.h>


#include <afxdisp.h>        // MFC Automation classes

// MultiMedia
#include <atlbase.h>

// DirectX or 3D Core

#pragma	warning( push )
#pragma warning( disable : 4819 )
#include <D3D9.h>
#include <dxerr9.h>
#include <dsound.h>
#include <dshow.h>
#pragma	warning( pop )

// STL 
#include <vector>
#include <list>
#include <map>
#include <string> 
#include <queue>
using namespace std;

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

// exception handler���� 

#include "NeuzVersion.h"  
#include "VersionCommon.h"
#include "..\\Global\\FLVersionAll.h"

#include "..\_CommonDefine\FLCommonLength.h"
#include "..\_CommonDefine\RefectoringDefine.h"
#include "..\_CommonDefine\FLResourceDefines.h"
#include "..\_CommonDefine\FLReturnStateCode.h"

#include "DefineCommon.h"
#include "Resource.h"    
#include "authorization.h" 

#pragma	warning( push )
#pragma warning( disable : 4819 )
#include "dxutil.h"
#include "d3denumeration.h" 
#include "d3dsettings.h"
#include "d3dapp.h" 
#include "vutil.h"
#include "d3dfont.h"
#include "d3dutil.h"
#include "DSUtil.h"
#pragma	warning( pop )

// _Network
#include "CmnHdr.h"
#include "ErrorCode.h"
#include "MsgHdr.h"
#include "tools.h"

#include "File.h"

#include "../_Interface/AppDefine.h"
#include "../_Network/Misc/Include/Misc.h"

// Resource
#include <define.h>
#include <defineAttribute.h>
#include <defineItemkind.h>
#include <defineEventArena.h>
#include <defineItemType.h>
#include <defineItemGrade.h>
#include <defineJob.h>
#include <defineWorld.h>
#include <defineText.h>
#include <defineSound.h>
#include <WndStyle.h>
#include <ResData.h>
#ifdef __M_UTIL
#include "..\\Global\\M_Util.h"
#endif
#include "../_CommonDefine/FLTm.h"
#include "../_CommonDefine/FLFlyffConfig.h"
#include "../_CommonDefine/FLFileCompare.h"
#include "../_CommonDefine/FLCommonCount.h"
#include "../_CommonDefine/FLMakeFileData.h"
#include "../_CommonDefine/FLPacketStruct.h"
#include "../_CommonDefine/FlyffCommonDefine.h"

#include "../_CommonDefine/UTIL/FLKeyGenerator.h"

#include "../_CommonDefine/Packet/FLPacketAllDefine.h"
#include "../_CommonDefine/Packet/FLPacketItemUpgrade.h"

#include "../_UnhandledException/ExceptionHandler.h"
#include "FLContentSwitch.h"

// Common
#include "Data.h"
#include "Timer.h"
#include "Scanner.h"
#include "Script.h"
#include "2DRender.h"
#include "TextureManager.h"
#include "SkyBox.h"
#include "patch.h"
#include "Project.h"
#include "path.h"
#include "landscape.h"
#include "xUtil3D.h"
#include "Graphic3D.h"
#include "Music.h"
#include "HwOption.h"
#include "yUtil.h"


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

#include "../Neuz/FLApplyItemEffect.h"

#include "TexturePool.h"	//gmpbigsun

// Model
#include "Model.h"
#include "Billboard.h"
#include "Material.h"
#include "Bone.h"
#include "Object3D.h"
#include "ModelObject.h"
#include "ModelMng.h"
#include "Particle.h"

// object
#include "Obj.h"
#include "Ctrl.h"
#include "Item.h"
#include "sfxbase.h"
#include "Mover.h"   
#include "Region.h"
//#include "Weather.h"
#include "Camera.h"
#include "Light.h"
#include "World.h"
#include "WorldMng.h"

#include "CreateObj.h"
#include "authorization.h"

// Interface
#include "EditString.h"      // CString�� Ȯ���� CEditString
#include "ToolTip.h"         // ���� 
#include "ITheme.h"          // �������̽��� ����, ��Ų, ��Ʈ ������ ���� Ŭ���� 
#include "WndBase.h"         // ������ �⺻ Ŭ����, ��Ÿ ���� ��Ʈ�� (��ư, ��ũ�ѹ� ���) 
#include "WndControl.h"      // ���� ���� ��Ʈ�� (��ư, ��ũ�ѹ� ���)
#include "wndEditCtrl.h"     // ����Ʈ ��Ʈ��, IME, ���� �Է�
#include "WndItemCtrl.h"     // ���� ���� ��Ʈ�� (��ư, ��ũ�ѹ� ���)
#include "WndTradeCtrl.h"     // ���� ���� ��Ʈ�� (��ư, ��ũ�ѹ� ���)
#include "WndNeuz.h"         // Neuz ������ ���� ��� �⺻ Ŭ���� 
#include "WndMessageBox.h"   // �Ž��� �ڽ�, ���, ���� ��� 
#include "WndGuideSystem.h"
#include "WndTitle.h"        // Title�� ���� Ŭ���� (Login, Character List ���)
#include "WndTaskBar.h"      // �½�ũ �� Ŭ���� ����, �Ŵ� ��� 
#include "WndField.h"        // Field�� ���� Ŭ���� (����, �κ��丮 ���)
#include "WndCommunication.h"// Field�� ���� Ŭ���� (����, �κ��丮 ���)
#include "WndOption.h"       // Option ���� 
#include "WndOptionGame.h"	 // ���� �ɼ�
#include "WndDialog.h"       // ��ȭ  
#include "WndShop.h"         // ���� 
#include "WndMessengerSearch.h" // �޽��� �˻�â
#include "WndManager.h"      // ���� �Ŵ��� 
#include "WndWorld.h"        // ���� ���� 
#include "WndGuild.h"			// ��� ����
#include "WndRankGuild.h"		// ��� ��ŷ
#include "WndRankInfo.h"
#include "WndRankWar.h"
#include "WndGuildVote.h"         // �����ǥ ����
#include "WndUpgradeBase.h"         // ������� 

#include "FLColorManager.h"
#include "WndFontColorManager.h"

#include "FLShortcutKeyManager.h"


#ifdef __YCLOTH
#include "cloth.h"
#endif

// Application
#include "DialogMsg.h"
#include "Neuz.h"
#include <common/FLTickCount.h>
#include <common/FLStdFile.h>
#include "flyffevent.h"

// console
#ifdef __BS_CONSOLE
#include "Console.h"
#endif

#ifdef __BS_EFFECT_LUA
//#include "BsCommon.h"
#include "../_CommonDefine/FLDeleter.h"
#endif

#include "GlobalTickManager.h"		//gmpbigsun( 20100713 )

#include "../Neuz/DPClient.h"

#define D3DDEVICE   g_Neuz.m_pd3dDevice


extern BYTE             g_bKeyTable[256]; // Ű �Է� ���̺� 
extern BOOL             g_bSlotSwitchAboutEquipItem[ MAX_SLOT_ITEM ];
extern CNeuzApp         g_Neuz        ; // ���ø����̼� Ŭ����, DX3D ������ ��ũ 
extern CToolTip         g_toolTip     ; // ���� ��� 
extern CToolTip         g_toolTipSub1 ;
extern CToolTip         g_toolTipSub2 ;
extern CMover*          g_pPlayer     ; // �÷��̾� ��ü 
extern CWorldMng	    g_WorldMng    ;
extern CProject         prj;
extern CModelObject*    g_pBipedMesh;
extern CD3DApplication* g_pD3dApp;
extern CGameTimer       g_GameTimer;
extern CDPClient		g_DPlay;


struct CULLINFO
{
    D3DXVECTOR3 vecFrustum[8];    // corners of the view frustum
    D3DXPLANE planeFrustum[6];    // planes of the view frustum
};
extern CULLINFO     g_cullinfo;
extern FLTickCount	g_TickCount;
extern CFlyffEvent	g_eLocal; 
extern ADDSMMODE	g_AddSMMode;
extern DWORD		g_tmCurrent;	// �������� ����� ���� �ð�.
extern int			g_nRenderCnt;	// ����ȭ�鶧���� �ϳ��� ī��Ʈ �Ǵ� ����
extern int			g_nProcessCnt;	// ���μ������� �ϳ��� ī��Ʈ �Ǵ� ����.

#include "GuildCombat1to1.h"						//sun: 11, �ϴ��� ��� ����
extern CGuildCombat1to1Mng g_GuildCombat1to1Mng;

#ifdef __BS_CONSOLE
extern DlgConsole g_Console;
#endif
extern vector< CString > g_vecEncryptedValidCertifierIP;

#include <crypto/FLRijndael.h>
#include <crypto/FLMD5.h>


#endif 
