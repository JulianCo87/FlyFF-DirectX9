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
CShip*			 g_pShip;				// �÷��̾ ���� �ϴ� �����.
BYTE             g_bKeyTable[256];		// Ű �Է� ���̺�
BOOL             g_bSlotSwitchAboutEquipItem[ MAX_SLOT_ITEM ];		// ���� �����ۿ� ���� Ű ���� ����ġ
CModelObject*    g_pBipedMesh;
CD3DApplication* g_pD3dApp; 
CGameTimer       g_GameTimer;
FLTickCount       g_TickCount;
CFlyffEvent      g_eLocal;
ADDSMMODE        g_AddSMMode;
DWORD	         g_tmCurrent = 0;		// �������� ����� ���� �ð�.
time_t		g_tCurrent;		//	time( NULL )
int		         g_nRenderCnt = 0;		// ����ȭ�鶧���� �ϳ��� ī��Ʈ �Ǵ� ����
int		         g_nProcessCnt = 0;		// ���μ������� �ϳ��� ī��Ʈ �Ǵ� ����.

#ifdef __BS_CONSOLE
DlgConsole g_Console;
#endif

vector< CString > g_vecEncryptedValidCertifierIP;

CNeuzApp         g_Neuz;                // �� ��������

CGuildCombat1to1Mng g_GuildCombat1to1Mng;	//sun: 11, �ϴ��� ��� ����
