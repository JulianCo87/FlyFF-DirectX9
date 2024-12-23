// stdafx.cpp : source file that includes just the standard includes
//	Neuz.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"  

#include "Ship.h"

TCHAR	PROGRAM_NAME[128] = _T( "Neuz" );
TCHAR	NETWORK_LOG_NAME[128] = _T( "CLNetwork" );


CProject         prj;
CWorldMng  	     g_WorldMng;
CToolTip         g_toolTip;
CToolTip         g_toolTipSub1;
CToolTip         g_toolTipSub2;
CULLINFO         g_cullinfo;
CMover*          g_pPlayer;
CShip*			 g_pShip;				// 플레이어가 조종 하는 비공정.
BYTE             g_bKeyTable[256];		// 키 입력 테이블
BOOL             g_bSlotSwitchAboutEquipItem[ MAX_SLOT_ITEM ];		// 장착 아이템에 대한 키 눌림 스위치
CModelObject*    g_pBipedMesh;
CD3DApplication* g_pD3dApp; 
CGameTimer       g_GameTimer;
FLTickCount       g_TickCount;
CFlyffEvent      g_eLocal;
ADDSMMODE        g_AddSMMode;
DWORD	         g_tmCurrent = 0;		// 전역으로 사용할 현재 시간.
time_t		g_tCurrent;		//	time( NULL )
int		         g_nRenderCnt = 0;		// 렌더화면때마다 하나씩 카운트 되는 변수
int		         g_nProcessCnt = 0;		// 프로세스마다 하나씩 카운트 되는 변수.

#ifdef __BS_CONSOLE
DlgConsole g_Console;
#endif

vector< CString > g_vecEncryptedValidCertifierIP;

CNeuzApp         g_Neuz;                // 맨 마지막에

CGuildCombat1to1Mng g_GuildCombat1to1Mng;	//sun: 11, 일대일 길드 대전
