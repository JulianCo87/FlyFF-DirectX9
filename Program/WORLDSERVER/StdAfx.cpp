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
DWORD			g_tmCurrent = 0;		// �������� ����� ���� �ð�.
time_t			g_tCurrent	= 0;
int				g_nProcessCnt = 0;	// ���μ������� �ϳ��� ī��Ʈ �Ǵ� ����.

CGuildCombat1to1Mng	g_GuildCombat1to1Mng;


