// stdafx.cpp : source file that includes just the standard includes
//	WorldServer.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

TCHAR	PROGRAM_NAME[128] = _T( "WorldServer" );
TCHAR	NETWORK_LOG_NAME[128] = _T( "WSNetwork" );

char			g_szDBAddr[16];
CProject		prj;
FLTickCount		g_TickCount;
int				g_fX3	= 1;
u_long			g_uIdofMulti;
char			g_sHeartbeat[32] = {0, };
CGameTimer		g_GameTimer;
CFlyffEvent		g_eLocal;
ADDSMMODE		g_AddSMMode;
ULONG			g_uKey;
DWORD			g_tmCurrent = 0;		// 전역으로 사용할 현재 시간.
time_t			g_tCurrent	= 0;
int				g_nProcessCnt = 0;	// 프로세스마다 하나씩 카운트 되는 변수.

CGuildCombat1to1Mng	g_GuildCombat1to1Mng;


