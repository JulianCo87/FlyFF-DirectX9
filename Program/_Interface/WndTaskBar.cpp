// WndArcane.cpp: implementation of the CWndNeuz class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DialogMsg.h"
#include "WndField.h"
#include "WndManager.h"
#include "WndRegVend.h"

_ERROR_STATE g_Error_State;

#define WND_WIDTH 210
#define TASKBAR_HEIGHT 48


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �۾� ���� 
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
CWndShortcut::CWndShortcut()
{
}
CWndShortcut::~CWndShortcut()
{
}
void CWndShortcut::OnDraw(C2DRender* p2DRender)
{
	CWndButton::OnDraw( p2DRender );
}
BOOL CWndShortcut::Process()
{
	return CWndButton::Process();
}
void CWndShortcut::PaintFrame( C2DRender* p2DRender ) 
{
	CWndButton::PaintFrame( p2DRender );
} 
void CWndShortcut::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWndButton::OnLButtonUp( nFlags, point );
}
void CWndShortcut::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWndButton::OnLButtonDown( nFlags, point );
}
void CWndShortcut::OnRButtonUp(UINT nFlags, CPoint point)
{
	CWndButton::OnRButtonUp( nFlags, point );
}
void CWndShortcut::OnRButtonDown(UINT nFlags, CPoint point)
{
	CWndButton::OnRButtonDown( nFlags, point );
}
void CWndShortcut::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CWndButton::OnLButtonDblClk( nFlags, point );
}
void CWndShortcut::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	CWndButton::OnRButtonDblClk( nFlags, point );
}
void CWndShortcut::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWndButton::OnKeyDown( nChar, nRepCnt, nFlags );
}
void CWndShortcut::OnMouseMove(UINT nFlags, CPoint point)
{
	CWndButton::OnMouseMove( nFlags, point );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �� ����Ʈ 
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

CWndQuickList::CWndQuickList() 
{ 
} 
CWndQuickList::~CWndQuickList() 
{ 
} 
void CWndQuickList::OnDraw( C2DRender* p2DRender ) 
{
	CWndTaskBar* pWndTaskBar = (CWndTaskBar*)m_pParentWnd;
	// ������ ������ ��� 
	CPoint point = CPoint( 0, 0);//POINT_ITEM;
	for( int y = 0; y < 4; y++ )
	{
		for( int i = 0; i < MAX_SLOT_ITEM; i++ )
		{
			point = CPoint( i * 32 + 32 , y * 38 + 6 );
			LPSHORTCUT lpShortcut = &pWndTaskBar->m_aSlotItem[ y ][ i ] ;
			if( !lpShortcut->IsEmpty() )
			{
				if( lpShortcut->m_pTexture )
					p2DRender->RenderTexture( point, lpShortcut->m_pTexture );
				if( lpShortcut->m_dwShortcut == SHORTCUT_ITEM )
				{
					FLItemBase* pItemBase = g_pPlayer->GetItemId( lpShortcut->m_dwId );
					FLItemElem* pItemElem = (FLItemElem*)pItemBase;

					if( pItemElem  )
					{
						if( pItemElem->GetProp()->dwPackMax > 1 )
						{
							TCHAR szTemp[ 32 ];
							FLSPrintf( szTemp, _countof( szTemp ), "%d", pItemElem->m_nItemNum );
							CSize size = m_p2DRender->m_pFont->GetTextExtent( szTemp );
							p2DRender->TextOut( point.x + 32 - size.cx, point.y + 32 - size.cy, szTemp, 0xff0000ff );
							p2DRender->TextOut( point.x + 31 - size.cx, point.y + 31 - size.cy, szTemp, 0xffb0b0f0 );
						}
					}
					else
						lpShortcut->m_dwShortcut = SHORTCUT_NONE; 				
				}
			}
		}
	}
} 
void CWndQuickList::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndQuickList::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_QUICKLIST, 0, CPoint( 0, 0 ), pWndParent );
	
} 
/*
  ���� ������ ���� ��� 
BOOL CWndQuickList::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndQuickList::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndQuickList::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndQuickList::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndQuickList::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndQuickList::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

//#define TASKSHORTCUT 45
#define ICON_SIZE    32
#define SKILL_SIZE   28

/*
#define POINT_APPLET_X  87
#define POINT_APPLET_Y   9
#define POINT_ITEM_X   295
#define POINT_ITEM_Y     9
#define POINT_QUEUE_X  649
#define POINT_QUEUE_Y    9
*/
DWORD   POINT_APPLET_X ;
DWORD   POINT_APPLET_Y ;
DWORD   POINT_ITEM_X   ;
DWORD   POINT_ITEM_Y   ;
DWORD   POINT_QUEUE_X  ;
DWORD   POINT_QUEUE_Y  ;

#define HORIRC_APPLET( nIndex ) CRect( POINT_APPLET_X + nIndex * ICON_SIZE, 6, POINT_APPLET_X + nIndex * ICON_SIZE + ICON_SIZE , 6 + ICON_SIZE )
#define VERTRC_APPLET( nIndex ) CRect( 6, POINT_APPLET_X + nIndex * ICON_SIZE, 3 + ICON_SIZE, POINT_APPLET_X + nIndex * ICON_SIZE + ICON_SIZE )
#define HORIRC_ITEM( nIndex )   CRect( POINT_ITEM_X + nIndex * ICON_SIZE, 6, POINT_ITEM_X + nIndex * ICON_SIZE + ICON_SIZE , 6 + ICON_SIZE )
#define VERTRC_ITEM( nIndex )   CRect( 6, POINT_ITEM_X + nIndex * ICON_SIZE, 3 + ICON_SIZE, POINT_ITEM_X + nIndex * ICON_SIZE + ICON_SIZE )
#define HORIRC_QUEUE( nIndex )  CRect( POINT_QUEUE_X + nIndex * SKILL_SIZE, 6, POINT_QUEUE_X + nIndex * SKILL_SIZE + SKILL_SIZE, 7 + SKILL_SIZE )
#define VERTRC_QUEUE( nIndex )  CRect( 6, POINT_QUEUE_X + nIndex * SKILL_SIZE, 6 + SKILL_SIZE, POINT_QUEUE_X + nIndex * SKILL_SIZE + SKILL_SIZE )

#define RECT_APPLET CRect( POINT_APPLET_X, POINT_APPLET_Y, POINT_APPLET_X + m_nMaxSlotApplet * ICON_SIZE , POINT_APPLET_Y + ICON_SIZE  )
#define RECT_ITEM   CRect( POINT_ITEM_X  , POINT_ITEM_Y  , POINT_ITEM_X   + MAX_SLOT_ITEM   * ICON_SIZE , POINT_ITEM_Y   + ICON_SIZE  )
#define RECT_QUEUE  CRect( POINT_QUEUE_X , POINT_QUEUE_Y , POINT_QUEUE_X  + MAX_SLOT_QUEUE  * SKILL_SIZE, POINT_QUEUE_Y  + SKILL_SIZE )

#define POINT_APPLET CPoint( POINT_APPLET_X, POINT_APPLET_Y )
#define POINT_ITEM   CPoint( POINT_ITEM_X  , POINT_ITEM_Y   )
#define POINT_QUEUE  CPoint( POINT_QUEUE_X , POINT_QUEUE_Y  )


void CWndTaskBar::OnDestroyChildWnd( CWndBase* pWndChild )
{
	if( m_pWndQuickList == pWndChild )
		SAFE_DELETE( m_pWndQuickList );
}

void CWndTaskBar::Serialize( CAr & ar )
{
	if( ar.IsStoring() )
	{
		int nCount			= 0;
		ptrdiff_t nCountTag = 0;

		if( _GetContentState( CT_NEWUI_19 ) == CS_VER1 )
		{
			nCount		= 0;
			nCountTag	= ar.ReserveSpace( sizeof( nCount ) );

			for( int i = 0; i < MAX_SLOT_APPLET; i++ )
			{
				if( m_aSlotApplet[i].m_dwShortcut == SHORTCUT_NONE )		// ���� ����ִ��� �˻�
				{
					ar << i;
					ar << m_aSlotApplet[i].m_dwShortcut <<	m_aSlotApplet[i].m_dwId << m_aSlotApplet[i].m_dwType;
					ar << m_aSlotApplet[i].m_dwIndex <<	m_aSlotApplet[i].m_dwUserId << m_aSlotApplet[i].m_dwData;
					if( m_aSlotApplet[i].m_dwShortcut == SHORTCUT_CHAT )
						ar.WriteString( m_aSlotApplet[i].m_szString );
					nCount++;
				}
			}
			ar.WriteReservedSpace( nCountTag, &nCount, sizeof( nCount ) );
		}

		nCount		= 0;
		nCountTag	= ar.ReserveSpace( sizeof( nCount ) );

		for( int i = 0; i < MAX_SLOT_ITEM_COUNT; i++ )
		{
			for( int j = 0; j < MAX_SLOT_ITEM; j++ )
			{
				if( m_aSlotItem[i][j].m_dwShortcut == SHORTCUT_NONE )		// ���� ����ִ��� �˻�
				{
					ar << i << j;
					ar << m_aSlotItem[i][j].m_dwShortcut <<	m_aSlotItem[i][j].m_dwId << m_aSlotItem[i][j].m_dwType;
					ar << m_aSlotItem[i][j].m_dwIndex << m_aSlotItem[i][j].m_dwUserId << m_aSlotItem[i][j].m_dwData;
					if( m_aSlotItem[i][j].m_dwShortcut == SHORTCUT_CHAT)
						ar.WriteString( m_aSlotItem[i][j].m_szString );
					nCount++;
					
				}
			}
		}
		ar.WriteReservedSpace( nCountTag, &nCount, sizeof( nCount ) );

		nCount		= 0;
		nCountTag	= ar.ReserveSpace( sizeof( nCount ) );

		ar << nCount;
		for( int i = 0; i < MAX_SLOT_QUEUE; i++ )
		{
			if( m_aSlotQueue[i].m_dwShortcut == SHORTCUT_NONE )		// ���� ����ִ��� �˻�
			{
				ar << i;
				ar << m_aSlotQueue[i].m_dwShortcut << m_aSlotQueue[i].m_dwId << m_aSlotQueue[i].m_dwType;
				ar << m_aSlotQueue[i].m_dwIndex << m_aSlotQueue[i].m_dwUserId << m_aSlotQueue[i].m_dwData;
				nCount++;
			}
		}
 		ar << m_nActionPoint;

		ar.WriteReservedSpace( nCountTag, &nCount, sizeof( nCount ) );
	}
	else
	{
		InitTaskBar();
		int nCount	= 0;

		if( _GetContentState( CT_NEWUI_19 ) == CS_VER1 )
		{
			ar >> nCount;	// applet count

			for( int i = 0; i < nCount; i++ )
			{
				int nIndex	= 0;

				ar >> nIndex;
				ar >> m_aSlotApplet[nIndex].m_dwShortcut >>	m_aSlotApplet[nIndex].m_dwId >> m_aSlotApplet[nIndex].m_dwType;
				ar >> m_aSlotApplet[nIndex].m_dwIndex >> m_aSlotApplet[nIndex].m_dwUserId >> m_aSlotApplet[nIndex].m_dwData;
				if( m_aSlotApplet[nIndex].m_dwShortcut == SHORTCUT_CHAT)
					ar.ReadString( m_aSlotApplet[nIndex].m_szString, _countof( m_aSlotApplet[nIndex].m_szString ) );
				m_aSlotApplet[nIndex].m_dwIndex = nIndex;
				SetTaskBarTexture( &m_aSlotApplet[nIndex] );

				if( m_aSlotApplet[nIndex].m_dwShortcut == SHORTCUT_SKILL )	
				{
					SkillProp* pProp = NULL;
					if( m_aSlotApplet[nIndex].m_dwType == 2 )
						pProp =  prj.GetPartySkill( m_aSlotApplet[nIndex].m_dwId );
					else
					{
						LPSKILL pSkill = g_pPlayer->GetSkill( 0, m_aSlotApplet[nIndex].m_dwId );
						
						if(pSkill)
							pProp = prj.GetSkillProp( pSkill->dwSkill );	
					}
					if( pProp == NULL || (pProp->dwComboStyle != CT_STEP && pProp->dwComboStyle != CT_GENERAL) )	
					{
						m_aSlotApplet[nIndex].Empty();
					}
				}
#ifdef __CLIENT
				else if( m_aSlotApplet[nIndex].m_dwShortcut == SHORTCUT_ITEM )	//sun: 8�� �۾�
				{
					if( g_pPlayer )
					{
						FLItemBase* pItemBase	= g_pPlayer->GetItemId( m_aSlotApplet[nIndex].m_dwId );
						if( pItemBase && pItemBase->GetProp()->dwPackMax > 1 )	// ���� ������ �������̸�?
							m_aSlotApplet[nIndex].m_dwItemId	= pItemBase->m_dwItemId;
					}
				}
#endif	// __CLIENT
			}
		}

		nCount	= 0;
		ar >> nCount;	// slot item count
		
		for( int i = 0; i < nCount; i++ )
		{
			int nIndex	= 0;
			int nIndex2	= 0;

			ar >> nIndex >> nIndex2;	// index
			ar >> m_aSlotItem[nIndex][nIndex2].m_dwShortcut >>	m_aSlotItem[nIndex][nIndex2].m_dwId >> m_aSlotItem[nIndex][nIndex2].m_dwType;
			ar >> m_aSlotItem[nIndex][nIndex2].m_dwIndex >> m_aSlotItem[nIndex][nIndex2].m_dwUserId >> m_aSlotItem[nIndex][nIndex2].m_dwData;
			if( m_aSlotItem[nIndex][nIndex2].m_dwShortcut == SHORTCUT_CHAT )
				ar.ReadString( m_aSlotItem[nIndex][nIndex2].m_szString, _countof( m_aSlotItem[nIndex][nIndex2].m_szString ) );
#ifdef __CLIENT
			else if( m_aSlotItem[nIndex][nIndex2].m_dwShortcut == SHORTCUT_ITEM )		//sun: 8�� �۾�
			{
				if( g_pPlayer )
				{
					FLItemBase* pItemBase	= g_pPlayer->GetItemId( m_aSlotItem[nIndex][nIndex2].m_dwId );
					if( pItemBase && pItemBase->GetProp()->dwPackMax > 1 )	// ���� ������ �������̸�?
						m_aSlotItem[nIndex][nIndex2].m_dwItemId	= pItemBase->m_dwItemId;
				}
			}
#endif	// __CLIENT
			m_aSlotItem[nIndex][nIndex2].m_dwIndex = nIndex2;
			SetTaskBarTexture( &m_aSlotItem[nIndex][nIndex2] );
		}

		nCount	= 0;
		ar >> nCount;

		for( int i = 0; i < nCount; i++ )
		{
			int nIndex	= 0;
			int nIndex2	= 0;

			ar >> nIndex;
			ar >> m_aSlotQueue[nIndex].m_dwShortcut >> m_aSlotQueue[nIndex].m_dwId >> m_aSlotQueue[nIndex].m_dwType;
			ar >> m_aSlotQueue[nIndex].m_dwIndex >> m_aSlotQueue[nIndex].m_dwUserId >> m_aSlotQueue[nIndex].m_dwData;

			m_aSlotQueue[nIndex].m_dwIndex = nIndex;

			LPSKILL lpSkill = g_pPlayer->GetSkill( m_aSlotQueue[nIndex].m_dwType, m_aSlotQueue[nIndex].m_dwId );
			FLERROR_ASSERT_LOG_DO( lpSkill, continue, _T("[������ �۾� ��] ��ų �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

			m_aSlotQueue[nIndex].m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, lpSkill->GetProp()->szIcon/*pItemBase->GetProp()->szIcon*/), 0xffff00ff, FALSE );
		}
		ar >> m_nActionPoint;
		m_nCurQueueNum = nCount;
	}
}

void CWndTaskBar::SetTaskBarTexture( LPSHORTCUT pShortcut )		
{
	if( pShortcut->m_dwShortcut == SHORTCUT_APPLET )
	{
		AppletFunc* pAppletFunc = g_WndMng.GetAppletFunc( pShortcut->m_dwId );
		if( pAppletFunc )
		{
			pShortcut->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice,  MakePath( DIR_ICON, pAppletFunc->m_pszIconName ), 0xffff00ff );
		} else
		{
			//_SUN_CHECKDATA_ : ������ ������ ã�� �� ����. �̰�� ������ ������û�� ������.
			g_DPlay.SendRemoveAppletTaskBar( (BYTE)( pShortcut->m_dwIndex ) );

			//gmpbigsun : Log�� ��Ȯ���� �ø�, �� ������ �����ϰ�� �����̸���, ���������̸����� ��ü
			FLERROR_LOG( PROGRAM_NAME, _T( "Cannot find appletfunc, myname:%s shortcut :%s" ), g_pPlayer->GetName(), pShortcut->m_szString );
		}
	}
	else if( pShortcut->m_dwShortcut == SHORTCUT_ITEM )
	{
		FLItemBase* pItemBase;
		pItemBase	= g_pPlayer->GetItemId( pShortcut->m_dwId );
		if( pItemBase )
			pShortcut->m_pTexture	= pItemBase->GetTexture();
	}
	else if ( pShortcut->m_dwShortcut == SHORTCUT_SKILL)
	{
		if( pShortcut->m_dwType == 2 )
		{
			SkillProp* pProp =  prj.GetPartySkill( pShortcut->m_dwId );
			pShortcut->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, pProp->szIcon/*pItemBase->GetProp()->szIcon*/), 0xffff00ff );
		}
		else
		{	
			LPSKILL lpSkill = g_pPlayer->GetSkill( pShortcut->m_dwType, pShortcut->m_dwId );
			FLERROR_ASSERT_LOG_RETURN_VOID( lpSkill, _T("[������ �۾� ��] ��ų �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

			SkillProp* pSkillProp = prj.m_aPropSkill.GetAt( lpSkill->dwSkill );
			if( pSkillProp )
				pShortcut->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, pSkillProp->szIcon ), 0xffff00ff );
		}
	}
//sun: 12, ����
	else if ( pShortcut->m_dwShortcut == SHORTCUT_LORDSKILL)
	{
		CCLord* pLord = CCLord::Instance();
		FLERROR_ASSERT_LOG_RETURN_VOID( pLord, _T("[������ ���� ��ų] ���� �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

		CLordSkill* pLordSkill = pLord->GetSkills();
		FLERROR_ASSERT_LOG_RETURN_VOID( pLordSkill, _T("[������ ���� ��ų] ���� ��ų �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

		CLordSkillComponentExecutable* pComponent = pLordSkill->GetSkill(pShortcut->m_dwId);
		FLERROR_ASSERT_LOG_RETURN_VOID( pComponent, _T("[������ ���� ��ų] ���� ��ų ������Ʈ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

		if(pComponent) 
			pShortcut->m_pTexture = pComponent->GetTexture();
	}
	else if ( pShortcut->m_dwShortcut == SHORTCUT_MOTION )
	{
		MotionProp* pMotionProp = prj.GetMotionProp( pShortcut->m_dwId );
		if(pMotionProp)			//061206 ma	8���� �� ��ǰ����� ���� ���� �߰�	propMotion.txt
		{
			pMotionProp->pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, pMotionProp->szIconName ), 0xffff00ff );
			pShortcut->m_pTexture = pMotionProp->pTexture;
		}
	}
	else if( pShortcut->m_dwShortcut == SHORTCUT_CHAT )
	{
		pShortcut->m_pTexture	= m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, "icon_MacroChat.dds" ), 0xffff00ff );
	}
	else if( pShortcut->m_dwShortcut == SHORTCUT_EMOTICON )
	{
		if( pShortcut->m_dwId >= 0 && pShortcut->m_dwId < MAX_EMOTICON_NUM  )
		{
			TCHAR buffer[MAX_SHORTCUT_STRING] = { 0 };
			FLStrcat( buffer, _countof( buffer ), "/" );
			FLStrcat( buffer, _countof( buffer ), g_DialogMsg.m_EmiticonCmd[pShortcut->m_dwId].m_szCommand );
			FLStrcpy( pShortcut->m_szString, _countof( pShortcut->m_szString ), buffer );
			pShortcut->m_pTexture =	g_DialogMsg.m_texEmoticonUser.GetAt(pShortcut->m_dwId);
		}
	}
	else if( pShortcut->m_dwShortcut == SHORTCUT_SKILLFUN )
	{
		pShortcut->m_pTexture	= m_pTexture;
	}
}

/*
* �׼� ����Ʈ
. �׼� ����Ʈ ���� CWndTaskBar::m_nActionPoint
. ��ų�ٿ� ������ �簢�� �׸���
. �׼�����Ʈ �ø��������
. ���ϰ����� 2��° ������ ���ÿ��� �׼�����Ʈ ���� 1���� - 100�� �Ѱ�
. �׼� ����Ʈ�� ���� ���̵� �׸��� - ��������Ʈ������ �ٸ���.
. ��ų���� �ܰ躰�� �׼�����Ʈ �Ҹ� �ٸ���.

*/

CWndTaskBar::CWndTaskBar()
{
	m_nCurQueue = -1;
	m_nCurQueueNum = 0;
	//m_nPosition = TASKBAR_RIGHT;//LEFT;//TASKBAR_TOP;
	m_nPosition = TASKBAR_BOTTOM;//LEFT;//TASKBAR_TOP;
	memset( m_aSlotApplet, 0, sizeof( m_aSlotApplet ) );
	memset( m_aSlotItem  , 0, sizeof( m_aSlotItem ) );
	memset( m_aSlotQueue , 0, sizeof( m_aSlotQueue ) );
	memset( &m_aSlotSkill, 0, sizeof( m_aSlotSkill ) );

	m_pWndQuickList = NULL;
	m_pSelectShortcut = NULL;
	m_bStartTimeBar = FALSE;
	m_nSkillBar = 0;
	m_nUsedSkillQueue = SKILL_QUEUE_NONE;
	m_nExecute = 0;
	m_nSlotIndex = 0;
	m_paSlotItem = m_aSlotItem[ m_nSlotIndex ];
	m_nActionPoint = 0;
	m_dwHighAlpha = 0;
//	m_nMaxSlotApplet = 5;

	m_bLockActionSlot = FALSE;
}

CWndTaskBar::~CWndTaskBar()
{
	/*
	for( int i = 0; i < m_awndShortCut.GetSize(); i++ )
	{
		CWndButton* pButton = (CWndButton*)m_awndShortCut.GetAt( i );
		SAFE_DELETE( pButton );
	}
	*/
}

void CWndTaskBar::InitTaskBar()
{
	m_nCurQueue = -1;
	m_nCurQueueNum = 0;
	//m_nPosition = TASKBAR_RIGHT;//LEFT;//TASKBAR_TOP;
	m_nPosition = TASKBAR_BOTTOM;//LEFT;//TASKBAR_TOP;
	memset( m_aSlotApplet, 0, sizeof( m_aSlotApplet ) );
	memset( m_aSlotItem  , 0, sizeof( m_aSlotItem ) );
	memset( m_aSlotQueue , 0, sizeof( m_aSlotQueue ) );
	
	m_pWndQuickList = NULL;
	m_pSelectShortcut = NULL;
	m_bStartTimeBar = FALSE;
	m_nSkillBar = 0;
	m_nUsedSkillQueue = SKILL_QUEUE_NONE;
	m_nExecute = 0;
	m_nSlotIndex = 0;
	m_paSlotItem = m_aSlotItem[ m_nSlotIndex ];
	m_nActionPoint = 0;
	m_dwHighAlpha = 0;
	//m_nMaxSlotApplet = 5;
}

void CWndTaskBar::PutTooTip( LPSHORTCUT pShortcut, CPoint point, CRect* pRect, BOOL bConvert )
{
	if( bConvert )
	{
		ClientToScreen( &point );
		ClientToScreen( pRect );
	}

	if( pShortcut->m_dwShortcut == SHORTCUT_APPLET )
	{
		AppletFunc* pAppletFunc = g_WndMng.GetAppletFunc( pShortcut->m_dwId ); 
		FLERROR_ASSERT_LOG_RETURN_VOID( pAppletFunc, _T("[������ �۾� ��] ���ø� �Լ� �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
		
		CEditString strEdit;

		PFONTCOLOR_WNDTASKBAR pFontColorWndTaskBar = g_WndFontColorManager->GetWndTaskBar();
		FLERROR_ASSERT_LOG_RETURN_VOID( pAppletFunc, _T("[������ �۾� ��] ���ø� �Լ� �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );


		if( pAppletFunc->m_cHotkey == 0 )
			strEdit.AddString( pAppletFunc->m_pAppletDesc, pFontColorWndTaskBar->m_stToolTip_ShortCutApplet.GetFontColor() );
		else
		{
			strEdit.AddString( pAppletFunc->m_pAppletDesc, pFontColorWndTaskBar->m_stToolTip_ShortCutApplet.GetFontColor() );
			strEdit.AddString( "\n" );

			CString string;
			string.Format( "[%s %c]", prj.GetText( TID_GAME_TOOLTIP_HOTKEY ), pAppletFunc->m_cHotkey );

			strEdit.AddString( string, pFontColorWndTaskBar->m_stToolTip_ShortCutHotKey.GetFontColor() );
		}
		g_toolTip.PutToolTip( pShortcut->m_dwId, strEdit, *pRect, point, 0 );
	}
	else	
	if( pShortcut->m_dwShortcut == SHORTCUT_CHAT )
	{
		g_toolTip.PutToolTip( 10000, pShortcut->m_szString, *pRect, point, 0 );
	}
	else
	if( pShortcut->m_dwShortcut == SHORTCUT_EMOTICON )
	{
		g_toolTip.PutToolTip( 10000, pShortcut->m_szString, *pRect, point, 0 );
	}
	else	
	if( pShortcut->m_dwShortcut == SHORTCUT_ITEM  )
	{
		g_WndMng.PutToolTip_Item( pShortcut->m_dwType, pShortcut->m_dwId, point, pRect );
	}
	else	
	if( pShortcut->m_dwShortcut == SHORTCUT_SKILL )
	{
		if( pShortcut->m_dwType == 2 )
		{
			g_WndMng.PutToolTip_Troupe( pShortcut->m_dwId, point, pRect );
		}
		else
		{
			LPSKILL lpSkill = g_pPlayer->GetSkill( 0, pShortcut->m_dwId );
			FLERROR_ASSERT_LOG_RETURN_VOID( lpSkill, _T("[������ �۾� ��] ��ų �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

			g_WndMng.PutToolTip_Skill( lpSkill->dwSkill, lpSkill->dwLevel, point, pRect );
		}
	}
//sun: 12, ����
	else	
	if( pShortcut->m_dwShortcut == SHORTCUT_LORDSKILL )
	{
		CCLord*	pLord = CCLord::Instance();

		if( NULL == pLord )
			return ;

		CLordSkill* pLordSkill = pLord->GetSkills();

		if( NULL == pLordSkill )
			return ;

		CLordSkillComponentExecutable* pComponent = pLordSkill->GetSkill(pShortcut->m_dwId);

		if( NULL == pComponent )
			return ;

		CString			string( _T("") ), str( _T("") );
		CEditString		strEdit;

		string.Format( "#b#cff2fbe6d%s#nb#nc\n%s", pComponent->GetName(), pComponent->GetDesc());
		if(pComponent->GetTick() != 0)
		{
			str.Format( prj.GetText(TID_TOOLTIP_COOLTIME), pComponent->GetTick(), 0);
			string += "\n";
			string += str;
		}
	
		strEdit.SetParsingString( string );
		g_toolTip.PutToolTip(10000, strEdit, *pRect, point, 0);
	}
	else	
	if( pShortcut->m_dwShortcut == SHORTCUT_MOTION )
	{
		MotionProp* pMotionProp = prj.GetMotionProp( pShortcut->m_dwId );
		if(!pMotionProp)		//061206 ma	8���� �� ��ǰ����� ���� ���� �߰�	propMotion.txt
			return;

		CString string;

		string.Format( "%s", pMotionProp->szDesc );
		
		if( pMotionProp->dwID == MOT_BASE_CHEER )
		{
			if( g_pPlayer )
			{
				CString str;
				str.Format( prj.GetText(TID_CHEER_MESSAGE1), g_pPlayer->m_nCheerPoint );

				string += "\n\n";
				string += str;

				if( g_pPlayer->m_nCheerPoint < MAX_CHEERPOINT )
				{
					CTimeSpan ct( ((g_pPlayer->m_dwTickCheer - GetTickCount()) / 1000) );		// �����ð��� �ʴ����� ��ȯ�ؼ� �Ѱ���

					if( ct.GetMinutes() <= 0 )
					{
						str.Format( prj.GetText(TID_PK_LIMIT_SECOND), ct.GetSeconds() );
					}
					else
					{
						str.Format( prj.GetText(TID_PK_LIMIT_MINUTE), ct.GetMinutes() );
					}
					
					string += "\n";
					string += (prj.GetText(TID_CHEER_MESSAGE2)+str);
				}
			}
		}

		CEditString strEdit;

		PFONTCOLOR_WNDTASKBAR pFontColorWndTaskBar = g_WndFontColorManager->GetWndTaskBar();
		strEdit.SetParsingString( string, pFontColorWndTaskBar->m_stToolTip_ShortCutMotion.GetFontColor() );

		g_toolTip.PutToolTip( pShortcut->m_dwId , strEdit, *pRect, point, 0 );
	}
	else
	if( pShortcut->m_dwShortcut == SHORTCUT_SKILLFUN )
	{
		FLStrcpy( pShortcut->m_szString, _countof( pShortcut->m_szString ), prj.GetText(TID_GAME_SKILLSHORTCUT) );
		g_toolTip.PutToolTip( 10000, pShortcut->m_szString, *pRect, point, 0 );
	}
		
}

void CWndTaskBar::OnMouseWndSurface( CPoint point )
{
	CRect rect = CRect( POINT_APPLET_X, POINT_APPLET_Y, POINT_APPLET_X + ICON_SIZE, POINT_APPLET_Y + ICON_SIZE );
	for( int i = 0; i < m_nMaxSlotApplet; i++ )
	{
		LPSHORTCUT lpShortcut = &m_aSlotApplet[ i ] ;
		if( !lpShortcut->IsEmpty() && rect.PtInRect( point) )
		{
			MotionProp* pMotionProp = prj.GetMotionProp( lpShortcut->m_dwId );
			
			if( !IsShortcut( lpShortcut, SHORTCUT_MOTION, MOT_BASE_CHEER ) )
				PutTooTip( lpShortcut, point, &rect );
		}
		rect += CPoint( ICON_SIZE, 0 );
	}

	rect = CRect( POINT_ITEM_X, POINT_ITEM_Y, POINT_ITEM_X + ICON_SIZE, POINT_ITEM_Y + ICON_SIZE );
	for( int i = 0; i < MAX_SLOT_ITEM; i++ )
	{
		LPSHORTCUT lpShortcut = &m_paSlotItem[ i ] ;
		if( !lpShortcut->IsEmpty() && rect.PtInRect( point)  )
		{
			if( !IsShortcut( lpShortcut, SHORTCUT_MOTION, MOT_BASE_CHEER ) )
				PutTooTip( lpShortcut, point,&rect );
		}
		rect += CPoint( ICON_SIZE, 0 );
	}
	rect = CRect( POINT_QUEUE_X, POINT_QUEUE_Y, POINT_QUEUE_X + SKILL_SIZE, POINT_QUEUE_Y + SKILL_SIZE );
	for( int i = 0; i < MAX_SLOT_QUEUE; i++ )
	{
		LPSHORTCUT lpShortcut = &m_aSlotQueue[ i ] ;
		if( !lpShortcut->IsEmpty() && rect.PtInRect( point)  )
		{
			if( !IsShortcut( lpShortcut, SHORTCUT_MOTION, MOT_BASE_CHEER ) )
				PutTooTip( lpShortcut, point,&rect );
		}
		rect += CPoint( SKILL_SIZE, 0 );
	}

	rect = CRect( POINT_QUEUE_X - ICON_SIZE - 5, POINT_QUEUE_Y, (POINT_QUEUE_X - ICON_SIZE - 5) + 32, POINT_QUEUE_Y + 32);

	if( rect.PtInRect( point) )
	{
		SHORTCUT Shortcut;
		Shortcut.m_dwShortcut = SHORTCUT_SKILLFUN;
		PutTooTip( &Shortcut, point,&rect );
	}
}

#define DRAW_HOTKEY( p2DRender, point, cHotkey ) { CString string; string.Format( "%c", cHotkey ); \
					p2DRender->TextOut( point.x - 0 + 2, point.y - 0 - 4, string, 0xffffffff ); }
					
					
/*
#define DRAW_HOTKEY( p2DRender, point, cHotkey ) {  \
	if( cHotkey >= 'A' && cHotkey <= 'Z' ) m_texPack.GetAt( cHotkey - 'A' )->Render( p2DRender, point, m_nAlphaCount );  \
	if( cHotkey >= '0' && cHotkey <= '9' ) m_texPack.GetAt( cHotkey - '0' + 26 )->Render( p2DRender, point, m_nAlphaCount ); }
*/


void CWndTaskBar::OnDraw( C2DRender* p2DRender )
{
	if( NULL == g_pPlayer )
		return;

	DWORD dwCur = g_tmCurrent;
	CRect rect = GetWindowRect();
	CRect rectSkillQueue;

	CD3DFont* pOldFont = p2DRender->GetFont(); 
	p2DRender->SetFont( CWndBase::m_Theme.m_pFontWndTitle ); 
		
	if( m_nPosition == TASKBAR_TOP || m_nPosition == TASKBAR_BOTTOM )
	{
		 rectSkillQueue.SetRect( rect.Width() - 32 * 4 - 6, 0, rect.Width(), rect.Height() );
	}
	else
	if( m_nPosition == TASKBAR_LEFT || m_nPosition == TASKBAR_RIGHT )
	{
		rectSkillQueue.SetRect( 0, rect.Height() - 32 * 4 - 6, rect.Width(), rect.Height() );
	}

	// �巡�� ���̶���Ʈ ���� ǥ��
	if( m_GlobalShortcut.IsEmpty() == FALSE )
	{
		DWORD dwColor;
		if( m_dwHighAlpha < 128 )
			dwColor = D3DCOLOR_ARGB( m_dwHighAlpha, 0, 0, 255 );
		else
			dwColor = D3DCOLOR_ARGB( 128 - ( m_dwHighAlpha - 128 ), 0, 0, 255 );

		LPWNDCTRL pApplet = GetWndCtrl( WIDC_CUSTOM1 );
		LPWNDCTRL pItem   = GetWndCtrl( WIDC_CUSTOM2 );
		LPWNDCTRL pSkill  = GetWndCtrl( WIDC_CUSTOM3 );

		CRect rectApplet = pApplet->rect;
		CRect rectItem = pItem->rect;
		CRect rectSkill = pSkill->rect;

		rectApplet.top = 42;
		rectApplet.bottom = 47;
		rectApplet.left -= 6;
		rectApplet.right += 6;
		rectItem.top = 42;
		rectItem.bottom = 47;
		rectItem.left -= 6;
		rectItem.right += 6;
		rectSkill.top = 42;
		rectSkill.bottom = 47;
		rectSkill.left -= 3;
		rectSkill.right += 4;
		
		switch( m_GlobalShortcut.m_dwShortcut )
		{
			case SHORTCUT_APPLET:
			case SHORTCUT_ITEM:
			case SHORTCUT_MOTION:
				p2DRender->RenderFillRect( rectApplet, dwColor );
				p2DRender->RenderFillRect( rectItem, dwColor );
				break;
			case SHORTCUT_SKILL:
				if( m_GlobalShortcut.m_dwType == 0 )
				{
					DWORD dwSkill = g_pPlayer->GetSkill( 0, m_GlobalShortcut.m_dwId )->dwSkill;
					DWORD dwComboStyleSrc = prj.GetSkillProp( dwSkill )->dwComboStyle;
					if( dwComboStyleSrc == CT_STEP  || dwComboStyleSrc == CT_GENERAL  )		// �����̳� ���׷��̸� �հ�
						p2DRender->RenderFillRect( rectItem, dwColor );		
					p2DRender->RenderFillRect( rectSkill, dwColor );
				}
				else
					p2DRender->RenderFillRect( rectItem, dwColor );
				break;
		}		
	}

	// ���÷� ������ ���
	CPoint point = POINT_APPLET;
	for( int i = 0; i < m_nMaxSlotApplet; i++ )
	{
		LPSHORTCUT lpShortcut = &m_aSlotApplet[ i ] ;
		if( !lpShortcut->IsEmpty() )
		{
			if( lpShortcut->m_pTexture )
				p2DRender->RenderTexture( point, lpShortcut->m_pTexture );
			if( lpShortcut->m_dwShortcut == SHORTCUT_ITEM )
			{
				FLItemBase* pItemBase = g_pPlayer->GetItemId( lpShortcut->m_dwId );
				FLItemElem* pItemElem = (FLItemElem*)pItemBase;
				if( pItemElem )
				{
					if( pItemElem->GetProp()->dwPackMax > 1 )
					{
						TCHAR szTemp[ 32 ];
						
						FLSPrintf( szTemp, _countof( szTemp ), "%d", g_pPlayer?g_pPlayer->m_Inventory.GetItemNumByItemId( pItemElem->m_dwItemId ): 0 );	//sun: 8�� �۾�
						
						CSize size = m_p2DRender->m_pFont->GetTextExtent( szTemp );
						p2DRender->TextOut( point.x + 32 - size.cx, point.y + 32 - size.cy, szTemp, 0xff0000ff );
						p2DRender->TextOut( point.x + 31 - size.cx, point.y + 31 - size.cy, szTemp, 0xffb0b0f0 );
					}
					DWORD dwGroup = g_pPlayer->m_cooltimeMgr.GetGroup( pItemElem->GetProp() );
					if( dwGroup )
					{
						DWORD dwEnd = g_pPlayer->m_cooltimeMgr.GetTime( dwGroup );		// �̺�Ʈ ���� �ð� 
						if( dwEnd != 0 && dwEnd > dwCur )  
						{
							DWORD dwBase = g_pPlayer->m_cooltimeMgr.GetBase( dwGroup );	// �̺�Ʈ ���� �ð� 
							RenderRadar( p2DRender, point, dwCur - dwBase, dwEnd - dwBase );
						}
					}
				}
				else
				{
					//sun: 8�� �۾�
					PT_ITEM_SPEC pItemProp	= g_xSpecManager->GetSpecItem( lpShortcut->m_dwItemId );
					if( pItemProp && pItemProp->dwPackMax > 1 )	// ���� ������ �������̸�?
					{
						DWORD dwId;
						
						dwId	= g_pPlayer->m_Inventory.GetObjIdByItemId( lpShortcut->m_dwItemId );
						
						if( dwId != NULL_ID )	// ���� ������ �������� �ִٸ�?
						{
							lpShortcut->m_dwId	= dwId;
							g_DPlay.SendAddAppletTaskBar( i, lpShortcut );
						}
						else
						{
							lpShortcut->Empty();
							g_DPlay.SendRemoveAppletTaskBar( i );
						}
					}
					else
					{
						lpShortcut->Empty();
						g_DPlay.SendRemoveAppletTaskBar( i );
					}

				}
			}
			else
			if( lpShortcut->m_dwShortcut == SHORTCUT_APPLET )
			{
				AppletFunc* pAppletFunc = g_WndMng.GetAppletFunc( lpShortcut->m_dwId ); 
				if( pAppletFunc && pAppletFunc->m_cHotkey )
				{
					CPoint ptHotkey( point.x + 8, point.y - 9 );
					DRAW_HOTKEY( p2DRender, ptHotkey, pAppletFunc->m_cHotkey );
				}
			}
			else if( lpShortcut->m_dwShortcut == SHORTCUT_SKILL && lpShortcut->m_dwType != 2 ) //�شܽ�ų�� ��Ÿ�� ���� Render�� ���� �ʴ´�.
			{
				RenderCollTime( point, lpShortcut->m_dwId, p2DRender );
			}
		//sun: 12, ����
			else if( lpShortcut->m_dwShortcut == SHORTCUT_LORDSKILL)
			{
					RenderLordCollTime( point, lpShortcut->m_dwId, p2DRender );
			}
			else
			if( lpShortcut->m_dwShortcut == SHORTCUT_MOTION )
			{
				if( lpShortcut->m_dwId == MOT_BASE_ESCAPE )
				{
					PT_ITEM_SPEC pItem = g_xSpecManager->GetSpecItem( g_AddSMMode.dwSMItemID[SM_ESCAPE] );
					
					if( pItem && g_pPlayer )
					{
						CTimeSpan ct( g_pPlayer->m_dwSMTime[SM_ESCAPE] );
						
						if( ct.GetTotalSeconds() )
						{
							point.y -= 1;
							point.x += 1;
							RenderRadar( m_p2DRender, point, (DWORD)( pItem->dwCircleTime - ct.GetTotalSeconds() ), pItem->dwCircleTime );
						}
					}
				}
			}	
		}
		point += CPoint( ICON_SIZE, 0 );
	}
	// ������ ������ ��� 
	point = POINT_ITEM;
	for( int i = 0; i < MAX_SLOT_ITEM; i++ )
	{
		LPSHORTCUT lpShortcut = &m_paSlotItem[ i ] ;
		if( !lpShortcut->IsEmpty() )
		{
			if( lpShortcut->m_pTexture )
				p2DRender->RenderTexture( point, lpShortcut->m_pTexture );
			if( lpShortcut->m_dwShortcut == SHORTCUT_ITEM )
			{
				FLItemBase* pItemBase = g_pPlayer->GetItemId( lpShortcut->m_dwId );
				FLItemElem* pItemElem = (FLItemElem*)pItemBase;
				if( pItemElem  )
				{
					if( pItemElem->GetProp()->dwPackMax > 1 )
					{
						TCHAR szTemp[ 32 ];

						//sun: 8�� �۾�
						FLSPrintf( szTemp, _countof( szTemp ), "%d", g_pPlayer?g_pPlayer->m_Inventory.GetItemNumByItemId( pItemElem->m_dwItemId ): 0 );
						
						CSize size = m_p2DRender->m_pFont->GetTextExtent( szTemp );
						p2DRender->TextOut( point.x + 32 - size.cx, point.y + 32 - size.cy, szTemp, 0xff0000ff );
						p2DRender->TextOut( point.x + 31 - size.cx, point.y + 31 - size.cy, szTemp, 0xffb0b0f0 );
					}
			
					DWORD dwGroup = g_pPlayer->m_cooltimeMgr.GetGroup( pItemElem->GetProp() );
					if( dwGroup )
					{
						DWORD dwEnd = g_pPlayer->m_cooltimeMgr.GetTime( dwGroup );		// �̺�Ʈ ���� �ð� 
						if( dwEnd != 0 && dwEnd > dwCur )
						{
							DWORD dwBase = g_pPlayer->m_cooltimeMgr.GetBase( dwGroup );	// �̺�Ʈ ���� �ð� 
							RenderRadar( p2DRender, point, dwCur - dwBase, dwEnd - dwBase );
						}
					}
				}
			} 
			else if( lpShortcut->m_dwShortcut == SHORTCUT_SKILL && lpShortcut->m_dwType != 2 ) //�شܽ�ų�� ��Ÿ�� ���� Render�� ���� �ʴ´�.
			{
				RenderCollTime( point, lpShortcut->m_dwId, p2DRender );
			}
		//sun: 12, ����
			else if( lpShortcut->m_dwShortcut == SHORTCUT_LORDSKILL)
			{
					RenderLordCollTime( point, lpShortcut->m_dwId, p2DRender );
			}
			else
			if( lpShortcut->m_dwShortcut == SHORTCUT_MOTION )
			{
				if( lpShortcut->m_dwId == MOT_BASE_ESCAPE )
				{
					PT_ITEM_SPEC pItem = g_xSpecManager->GetSpecItem( g_AddSMMode.dwSMItemID[SM_ESCAPE] );

					if( pItem && g_pPlayer )
					{
						CTimeSpan ct( g_pPlayer->m_dwSMTime[SM_ESCAPE] );
						
						if( ct.GetTotalSeconds() )
						{
							point.y -= 1;
							point.x += 1;
							RenderRadar( m_p2DRender, point, (DWORD)( pItem->dwCircleTime - ct.GetTotalSeconds() ), pItem->dwCircleTime );
						}
					}
				}
			}

				
		}
		// hotkey ��� 
		CHAR cHotkey = ( i == 9 ) ? '0' : '1' + i;
		CPoint ptHotkey( point.x + 8, point.y - 9 );
		//DRAW_HOTKEY( p2DRender, ptHotkey, cHotkey );
		point += CPoint( ICON_SIZE, 0 );
	}
	
	point = CPoint( POINT_QUEUE_X - ICON_SIZE - 5, POINT_QUEUE_Y );
	p2DRender->RenderTexture( point, m_pTexture );

	// ��ų ť 
	point = POINT_QUEUE;
	//p2DRender->TextOut( point.x - 5, point.y - 16, "ACTION SLOT" );
	for( int i = 0; i < m_nCurQueueNum; i++ )
	{
		LPSHORTCUT lpShortcut = &m_aSlotQueue[ i ] ;
		if( !lpShortcut->IsEmpty() )
		{
			if( lpShortcut->m_pTexture )
			{
				p2DRender->RenderTexture( point, lpShortcut->m_pTexture );
//				LPSKILL pSkill = g_pPlayer->GetSkill( lpShortcut->m_dwType, lpShortcut->m_dwId );
//				if( g_pPlayer->m_nReUseDelay[ lpShortcut->m_dwId ] > 0 )
//					p2DRender->TextOut( point.x, point.y, g_pPlayer->m_nReUseDelay[ lpShortcut->m_dwId ] );

			}
		}
		point += CPoint( SKILL_SIZE, 0 );
	}

	
	if( g_pPlayer->IsSMMode( SM_ACTPOINT ) == FALSE && g_pPlayer->IsSMMode( SM_ACTPOINT_30M ) == FALSE )
	{
	#define LV2MAXAP	10		// 2�ܰ� ��ų�� �����µ� �ʿ��� action point
	#define LV3MAXAP	25		
	#define LV4MAXAP	50
	#define LV5MAXAP	100

		point = POINT_QUEUE;
		point.x += SKILL_SIZE;	// ��ų 2��°ĭ���� ���̵�ȴ�.
		{
			int nAP2Size;	// �׼�����Ʈ�� ���̶���Ʈ������ ��ȯ�Ѱ�.
			if( m_nActionPoint >= LV4MAXAP )	// LV4MAXAP�� �Ѿ�� 5�ܰ� ����.
				nAP2Size = (SKILL_SIZE * 3) + (((m_nActionPoint - LV4MAXAP) * SKILL_SIZE) / (LV5MAXAP - LV4MAXAP));
			else if( m_nActionPoint >= LV3MAXAP )
				nAP2Size = (SKILL_SIZE * 2) + (((m_nActionPoint - LV3MAXAP) * SKILL_SIZE) / (LV4MAXAP - LV3MAXAP));
			else if( m_nActionPoint >= LV2MAXAP )
				nAP2Size = (SKILL_SIZE * 1) + (((m_nActionPoint - LV2MAXAP) * SKILL_SIZE) / (LV3MAXAP - LV2MAXAP));
			else 
				nAP2Size = (m_nActionPoint * SKILL_SIZE) / LV2MAXAP;
			
			p2DRender->m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			p2DRender->m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
			point.x += nAP2Size;
			nAP2Size = (SKILL_SIZE * 4) - nAP2Size;
			p2DRender->RenderFillRect( CRect(point.x, point.y,  point.x + nAP2Size, point.y + SKILL_SIZE + 2), 
										D3DCOLOR_ARGB( 0x7f, 0, 0, 0 ) );
			
			if( m_nCurQueueNum > 0 )
			{
				int show = 1;
				
				if( m_nActionPoint >= LV2MAXAP )
					show = 2;
				if( m_nActionPoint >= LV3MAXAP )
					show = 3;
				if( m_nActionPoint >= LV4MAXAP )
					show = 4;
				if( m_nActionPoint >= LV5MAXAP )
					show = 5;
				
				if( m_nCurQueueNum <= show )
					show = m_nCurQueueNum;
				
				CPoint Point = CPoint( POINT_QUEUE_X, POINT_QUEUE_Y+1 );

				RenderOutLineLamp( POINT_QUEUE_X, POINT_QUEUE_Y+1, show, SKILL_SIZE );
			}
		}
	}

	CWndButton* pWndButton = (CWndButton*)GetDlgItem( WIDC_UP );
	rect = pWndButton->GetWindowRect( TRUE );
	rect.top = rect.bottom - 2;
	p2DRender->TextOut( rect.left, rect.top, m_nSlotIndex + 1, 0xffffffff );
	p2DRender->SetFont( pOldFont );
}

void CWndTaskBar::UpdateItem()
{
	if( g_pPlayer == NULL )
		return;
	
	for( int i = 0; i < MAX_SLOT_ITEM; i++ )
	{
		LPSHORTCUT lpShortcut = &m_paSlotItem[ i ] ;
		if( !lpShortcut->IsEmpty() )
		{
			if( lpShortcut->m_dwShortcut == SHORTCUT_ITEM )
			{
				FLItemBase* pItemBase = g_pPlayer->GetItemId( lpShortcut->m_dwId );
				FLItemElem* pItemElem = (FLItemElem*)pItemBase;

				if( pItemElem )
				{
					if( lpShortcut->m_pTexture != pItemElem->GetTexture() )
						lpShortcut->m_pTexture = pItemElem->GetTexture();
				}
				else
				{
					//sun: 8�� �۾�
					PT_ITEM_SPEC pItemProp	= g_xSpecManager->GetSpecItem( lpShortcut->m_dwItemId );
					if( pItemProp && pItemProp->dwPackMax > 1 )	// ���� ������ �������̸�?
					{
						DWORD dwId;
						
						dwId	= g_pPlayer->m_Inventory.GetObjIdByItemId( lpShortcut->m_dwItemId );
						
						if( dwId != NULL_ID )	// ���� ������ �������� �ִٸ�?
						{
							lpShortcut->m_dwId	= dwId;
							g_DPlay.SendAddItemTaskBar( m_nSlotIndex, i, lpShortcut );
						}
						else
						{
							lpShortcut->Empty(); 				
							g_DPlay.SendRemoveItemTaskBar( m_nSlotIndex, i );
						}
					}
					else
					{
						lpShortcut->Empty(); 				
						g_DPlay.SendRemoveItemTaskBar( m_nSlotIndex, i );
					}

				}
			}
		}
	}
}

void CWndTaskBar::OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags )
{
}
void CWndTaskBar::OnInitialUpdate()
{
	CWndNeuz::OnInitialUpdate();

	LPWNDCTRL pApplet = GetWndCtrl( WIDC_CUSTOM1 );
	LPWNDCTRL pItem   = GetWndCtrl( WIDC_CUSTOM2 );
	LPWNDCTRL pSkill  = GetWndCtrl( WIDC_CUSTOM3 );

	POINT_APPLET_X = pApplet->rect.left;
	POINT_APPLET_Y = pApplet->rect.top;
	POINT_ITEM_X   = pItem->rect.left;
	POINT_ITEM_Y   = pItem->rect.top;
	POINT_QUEUE_X  = pSkill->rect.left;
	POINT_QUEUE_Y  = pSkill->rect.top;

	CRect rect( 5, 7, 754, 40 );

	g_WndMng.m_pWndTaskBar = this;

	{
		rect = g_Neuz.GetDeviceRect();
		m_pWndRoot->m_rectLayout = rect;
		switch( m_nPosition )
		{
		case TASKBAR_TOP:
			rect.bottom = TASKBAR_HEIGHT;
			m_pWndRoot->m_rectLayout.top = rect.bottom;
			break;
		case TASKBAR_BOTTOM:
			rect.top = rect.bottom - TASKBAR_HEIGHT;
			//suntodo(20110324) : ���⼭ ��� WndBase�� Root�� WndManager�� m_rectLayout�� �ٲٰ� �ִ�.
			//viewport�������� �ǹǷ� ���� �߿��ϴ�.
			m_pWndRoot->m_rectLayout.bottom = rect.top;
			break;
		case TASKBAR_LEFT:
			rect.right = TASKBAR_HEIGHT;
			m_pWndRoot->m_rectLayout.left = rect.right;
			break;
		case TASKBAR_RIGHT:
			rect.left = rect.right - TASKBAR_HEIGHT;
			m_pWndRoot->m_rectLayout.right = rect.left;
			break;
		}
		SetWndRect( rect );
	}
	m_texPack.LoadScript( m_pApp->m_pd3dDevice, MakePath( DIR_THEME,g_xFlyffConfig->GetMainLanguage(), "hotkey.inc" ) );

	m_menuShortcut.CreateMenu( this );	
	m_menuShortcut.AppendMenu( 0, 0 ,_T( "����" ) );

	m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice,  MakePath( DIR_ICON, "icon_ActionSkill.dds" ), 0xffff00ff );
	m_aSlotSkill.m_dwShortcut = SHORTCUT_SKILLFUN;
	m_aSlotSkill.m_pTexture   = m_pTexture;
}

HRESULT CWndTaskBar::InvalidateDeviceObjects()
{
	CWndBase::InvalidateDeviceObjects();	
	return S_OK;
}

HRESULT CWndTaskBar::DeleteDeviceObjects()
{
	CWndBase::DeleteDeviceObjects();
	return S_OK;
}

HRESULT CWndTaskBar::RestoreDeviceObjects()
{
	CWndBase::RestoreDeviceObjects();
	return 0;
}

BOOL CWndTaskBar::Initialize(CWndBase* pWndParent,DWORD dwWndId)
{
	CRect rect = g_Neuz.GetDeviceRect();
	//m_rectLayout = rect;
/*
	switch( m_nPosition )
	{
	case TASKBAR_TOP:
		rect.bottom = TASKBAR_HEIGHT;
		m_pWndRoot->m_rectLayout.top = rect.bottom;
		break;
	case TASKBAR_BOTTOM:
		rect.top = rect.bottom - TASKBAR_HEIGHT;
		m_pWndRoot->m_rectLayout.bottom = rect.top;
		break;
	case TASKBAR_LEFT:
		break;
	case TASKBAR_RIGHT:
		break;
	}
	*/
	//m_pWndRoot->SetWndRect( rectRoot );

//CMainFrame
	//rect.top = rect.bottom;
	//rect.bottom += TASKBAR_HEIGHT;
//	rect.bottom = CLIENT_HEIGHT;
	//m_strMessage = lpszMessage;
	//m_dwType = nType;
	//SetTitle("������ ����");
//	return CWndBase::Create( 0 | /*WBS_MODAL|*/ WBS_MANAGER | WBS_SOUND | WBS_NOFRAME, rect, &g_WndMng, APP_TASKBAR );
	BOOL bResult;
	if( g_Option.m_nResWidth == 800 )
	{
		m_nMaxSlotApplet = 5;
		bResult = CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_TASKBAR800, WBS_MANAGER | WBS_SOUND, CPoint( 0, 0 ), pWndParent );
	}
	else
	if( g_Option.m_nResWidth == 1024 )
	{
		m_nMaxSlotApplet = 10;
		bResult = CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_TASKBAR1024, WBS_MANAGER | WBS_SOUND, CPoint( 0, 0 ), pWndParent );
	}
	else
	if( g_Option.m_nResWidth == 1280 )
	{
		m_nMaxSlotApplet = MAX_SLOT_APPLET;
		bResult = CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_TASKBAR1280, WBS_MANAGER | WBS_SOUND, CPoint( 0, 0 ), pWndParent );
	}
//sun: Wide�� ���ػ� �߰�
	else if( g_Option.m_nResWidth == 1360 )
	{
		m_nMaxSlotApplet = MAX_SLOT_APPLET;
		bResult = CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_TASKBAR1360W, WBS_MANAGER | WBS_SOUND, CPoint( 0, 0 ), pWndParent );
	}
	else if( g_Option.m_nResWidth == 1400 )
	{
		m_nMaxSlotApplet = MAX_SLOT_APPLET;
		bResult = CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_TASKBAR1400, WBS_MANAGER | WBS_SOUND, CPoint( 0, 0 ), pWndParent );
	}
	else if( g_Option.m_nResWidth == 1440 )
	{
		m_nMaxSlotApplet = MAX_SLOT_APPLET;
		bResult = CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_TASKBAR1440W, WBS_MANAGER | WBS_SOUND, CPoint( 0, 0 ), pWndParent );
	}
	else if( g_Option.m_nResWidth == 1600 )
	{
		m_nMaxSlotApplet = MAX_SLOT_APPLET;
		bResult = CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_TASKBAR1600, WBS_MANAGER | WBS_SOUND, CPoint( 0, 0 ), pWndParent );
	}
	else if( g_Option.m_nResWidth == 1680 )
	{
		m_nMaxSlotApplet = MAX_SLOT_APPLET;
		bResult = CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_TASKBAR1680W, WBS_MANAGER | WBS_SOUND, CPoint( 0, 0 ), pWndParent );
	}	
	
	DelWndStyle( WBS_MOVE );
	return bResult;
	
}
BOOL CWndTaskBar::RemoveDeleteObj()
{
	/*
	for( int i = 0; i < m_awndShortCut.GetSize(); i++ )
	{
		CWndButton* pWndButton = (CWndButton*)m_awndShortCut.GetAt( i );
		if( pWndButton ) 
		{
			if( pWndButton->m_shortcut.m_dwShortcut == SHORTCUT_OBJECT )
			{
				CObj* pObj = (CObj*) pWndButton->m_shortcut.m_dwData;
				if( IsValidObj( pObj ) == FALSE )
					pWndButton->m_shortcut.m_dwData = 0;
			}
			else
			if( pWndButton->m_shortcut.m_dwShortcut == SHORTCUT_ITEM )
			{
				FLItemBase* pItemBase = (FLItemBase*) pWndButton->m_shortcut.m_dwData;
				if( pItemBase && pItemBase->IsEmpty() )
					pWndButton->Destroy( TRUE );
			}
		}
	}
	*/
	return TRUE;
}
void CWndTaskBar::SetItemSlot( int nSlot )
{
	if( nSlot >= 0 && nSlot < MAX_SLOT_ITEM_COUNT)
		m_paSlotItem = m_aSlotItem[ m_nSlotIndex = nSlot ];
}
BOOL CWndTaskBar::OnChildNotify(UINT message,UINT nID,LRESULT* pLResult)
{
	CWndButton* pWndButton = (CWndButton*) pLResult;
	if( message == WIN_ITEMDROP )
	{
		/*
		LPSHORTCUT pShortcut = (LPSHORTCUT) pLResult;
		if( pShortcut && pShortcut->m_dwData == 2 ) // ��ų ť ��ư�� ���̵�� 1000 �̻� 
		{
			SetSkillQueue( nID - 1000, pShortcut->m_dwShortcut, pShortcut->m_dwType, pShortcut->m_dwId, pShortcut->m_dwData, pShortcut->m_pTexture );
		}
		*/
	}
	else
	switch( nID )
	{
		case 101:
			/*
			if( m_pWndQuickList )
				m_pWndQuickList->Destroy();
			else
			{
				m_pWndQuickList = new CWndQuickList;
				m_pWndQuickList->Initialize( this );//&g_WndMng );
			}
			*/
			break;
		case WIDC_UP:
			m_nSlotIndex--;
			if( m_nSlotIndex < 0 ) m_nSlotIndex = MAX_SLOT_ITEM_COUNT-1;
			m_paSlotItem = m_aSlotItem[ m_nSlotIndex ];
			break;
		case WIDC_DOWN:
			m_nSlotIndex++;
			if( m_nSlotIndex > MAX_SLOT_ITEM_COUNT-1 ) m_nSlotIndex = 0;
			m_paSlotItem = m_aSlotItem[ m_nSlotIndex ];
			break;
		case WIDC_BUTTON1: // Menu
		{
			g_WndMng.OpenMenu();
			CRect rect = g_Neuz.GetDeviceRect();
			CRect rectMenu = m_wndMenu.GetScreenRect();
			switch( m_nPosition )
			{
			case TASKBAR_TOP:
				if( g_WndMng.m_pWndMenu )
					g_WndMng.m_pWndMenu->Move( CPoint( 0, TASKBAR_HEIGHT ) );
				break;
			case TASKBAR_BOTTOM:
				if( g_WndMng.m_pWndMenu )
					g_WndMng.m_pWndMenu->Move( CPoint( 0, rect.bottom - TASKBAR_HEIGHT - g_WndMng.m_pWndMenu->GetWndRect().Height() ) );
				break;
			case TASKBAR_LEFT:
				if( g_WndMng.m_pWndMenu )
					g_WndMng.m_pWndMenu->Move( CPoint( TASKBAR_HEIGHT, 0 ) );
				break;
			case TASKBAR_RIGHT:
				if( g_WndMng.m_pWndMenu )
					g_WndMng.m_pWndMenu->Move( CPoint( rect.right - TASKBAR_HEIGHT - g_WndMng.m_pWndMenu->GetWndRect().Width(), 0 ) );
				break;
			}

		}
		default:
			{
				switch( pWndButton->m_shortcut.m_dwIndex )
				{
					case APP_TRADE:		g_DPlay.SendTrade( (CMover*)g_WorldMng.Get()->GetObjFocus() );	break;
					default:
						g_WndMng.ObjectExecutor( &pWndButton->m_shortcut );	
						break;
				}
			}
	}
	return TRUE;
}
BOOL CWndTaskBar::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase )
{
	return TRUE;
}
void CWndTaskBar::OnSize(UINT nType, int cx, int cy)
{
	CWndNeuz::OnSize( nType, cx, cy );
//	CRect rect = GetClientRect();
//	rect.DeflateRect( 5, 5);
//	rect.right = 37;
	//ect rect( 5, 5, 44, 40 );//GetClientRect();
	//wndMenu.SetWndRect( rect );

//ect = GetClientRect();

	// Create Command button
	//for(int i = 0; i < 10; i++)
		//m_pWndCommand[i].SetWndRect(CRect(60+i*18,3,76+i*18,21));

	//int i;
	// Create Tray button
	//int nMax = 3;
	//int nStart = (rect.Width() - nMax * 26);
	//for(i = 0; i < nMax; i++)
//		m_pWndTray[i].SetWndRect(CRect(nStart + i * 25, 6, nStart + 24 + i * 25, 30));
}
void CWndTaskBar::RemoveSkillQueue( int nIndex, BOOL bSend )
{
	if( m_nExecute )	return;		// ��ųť �����߿� �������� �ʴ´�.
BACK:
	if( !m_aSlotQueue[ nIndex ].IsEmpty() )
	{
		LPSKILL pSkill = g_pPlayer->GetSkill( 0, m_aSlotQueue[ nIndex ].m_dwId );
		FLERROR_ASSERT_LOG_RETURN_VOID( pSkill, _T("[������ �۾� ��] ��ų �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
		
		SkillProp* pSkillProp = pSkill->GetProp();
		FLERROR_ASSERT_LOG_RETURN_VOID( pSkillProp, _T("[������ �۾� ��] ��ų �Ӽ� �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
	
		DWORD dwComboStyleSrc = pSkillProp->dwComboStyle;
		int i = nIndex;
		for( ; i < MAX_SLOT_QUEUE - 1; i++ )
		{
			memcpy( &m_aSlotQueue[ i ], &m_aSlotQueue[ i + 1 ], sizeof( SHORTCUT ) );
			m_aSlotQueue[ i ].m_dwIndex = i;
		}
		m_aSlotQueue[ i ].m_dwShortcut = SHORTCUT_NONE;
		m_aSlotQueue[ i ].m_dwId = 0;
		m_nCurQueueNum--;
		if( m_nCurQueueNum < 0 ) 
			m_nCurQueueNum = 0;
		if( CheckAddSkill( dwComboStyleSrc, nIndex /*- 1*/ ) == FALSE )
			goto BACK;
	}
	if( bSend )
		g_DPlay.SendSkillTaskBar( );
	// ��ų�� ����� �� ������ ��������.	

}
void CWndTaskBar::OnRButtonUp( UINT nFlags, CPoint point )
{
	CRect rect = RECT_APPLET;
	if( rect.PtInRect( point ) )
	{
		point.x -= rect.left;
		point.x /= ICON_SIZE;
		if( FALSE == m_aSlotApplet[ point.x ].IsEmpty() )
		{
#ifdef __MAINSERVER
			if( point.x == 0 && m_aSlotApplet[ point.x ].m_dwId == 400 )
				return;
#endif //__MAINSERVER
			
			m_aSlotApplet[ point.x ].Empty();			
			g_DPlay.SendRemoveAppletTaskBar( (BYTE)( point.x ) );		// ������ ���� ������ ����
		}
	}
	rect = RECT_ITEM;
	if( rect.PtInRect( point ) )
	{
		point.x -= rect.left;

		point.x /= ICON_SIZE;
		if( FALSE == m_paSlotItem[ point.x ].IsEmpty() )
		{
			m_paSlotItem[ point.x ].Empty();
			g_DPlay.SendRemoveItemTaskBar( (BYTE)( m_nSlotIndex ), (BYTE)( point.x ) );	// ������ ���� ������ ����			
		}

	}
	rect = RECT_QUEUE;
	if( rect.PtInRect( point ) )
	{
		point.x -= rect.left;
		point.y -= rect.top;
		point.x /= SKILL_SIZE;
		RemoveSkillQueue( point.x );
	}
}

BOOL CWndTaskBar::SetShortcut( int nIndex, DWORD dwShortcut, DWORD dwType, DWORD dwId, CTexture* pTexture, int nWhere )
{
	LPSHORTCUT pShortcut = NULL;
	//CWndShortcut* pWndButton = new CWndShortcut;
//	pWndButton->Create( _T( "" ), 0, CRect( 65 + nIndex * 32, 3, 65 + nIndex * 32 + 32 , 3 + 32), this, dwId );

	//pShortcut = m_aSlotApplet[ nIndex ];
//sun: 11, �½�ũ�� Ȯ��
	// Chat Shortcut 10���� ����
	if(dwShortcut == SHORTCUT_CHAT)
	{
		int nchatshortcut = 0;
		for( int i=0; i<MAX_SLOT_ITEM_COUNT; i++ )
		{
			for( int j=0; j<MAX_SLOT_ITEM; j++ )
			{
				if( m_aSlotItem[i][j].m_dwShortcut == SHORTCUT_CHAT )
					nchatshortcut++;
			}
		}
		for( int k = 0; k < m_nMaxSlotApplet; ++k )
			if( m_aSlotApplet[ k ].m_dwShortcut == SHORTCUT_CHAT )
				++nchatshortcut;

		if(nchatshortcut > 9)
		{
			g_WndMng.PutString( prj.GetText( TID_GAME_MAX_SHORTCUT_CHAT ), NULL, prj.GetTextColor( TID_GAME_MAX_SHORTCUT_CHAT ) );
			return FALSE;
		}
	}

	switch( m_nPosition )
	{
	case TASKBAR_TOP:
	case TASKBAR_BOTTOM:
		if( nWhere == 0 )
			pShortcut = &m_aSlotApplet[ nIndex ];
		else
		if( nWhere == 1 )
			pShortcut = &m_paSlotItem[ nIndex ];
		break;
	case TASKBAR_LEFT:
	case TASKBAR_RIGHT:
		if( nWhere == 0 )
			pShortcut = &m_aSlotApplet[ nIndex ];
		else
		if( nWhere == 1 )
			pShortcut = &m_paSlotItem[ nIndex ];
		break;
	}
	if( pTexture == NULL )
	{
		FLItemBase* pItemBase;
		if( dwShortcut == SHORTCUT_APPLET ) 
		{
			AppletFunc* pAppletFunc = g_WndMng.GetAppletFunc( dwId );
			pShortcut->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice,  MakePath( DIR_ICON, pAppletFunc->m_pszIconName ), 0xffff00ff );
				//pWndButton->SetTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, pAppletFunc->m_pszIconName ) );
			//pCardElem->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, pCardElem->GetProp()->szIcon), 0xff000000 );

		}
		else
		if( dwShortcut == SHORTCUT_ITEM ) 
		{
			pItemBase = g_pPlayer->GetItemId( dwId );
			CTexture* pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, pItemBase->GetProp()->szIcon), 0xffff00ff );
			pItemBase->SetTexture( pTexture ); 
			pShortcut->m_pTexture = pTexture ;
		}
		else
		if( dwShortcut == SHORTCUT_MOTION ) 
		{
			MotionProp* pMotionProp = prj.GetMotionProp( dwId );
			if(pMotionProp)		//061206 ma	8���� �� ��ǰ����� ���� ���� �߰�	propMotion.txt
			{
				pMotionProp->pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, pMotionProp->szIconName ), 0xffff00ff );
				pShortcut->m_pTexture = pMotionProp->pTexture;
				//pWndButton->SetTexture( g_Neuz.m_pd3dDevice, pItemBase->m_pTexture );
			}
			else
				return FALSE;
		}
		/*
		else
		if( dwShortcut == SHORTCUT_CARD ) 
		{
			pItemBase = g_pPlayer->m_Card.GetAt( dwId );//m_apItem[ i ] ;
			pItemBase->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, pItemBase->GetProp()->szIcon), 0xff000000 );
			pWndButton->SetTexture( g_Neuz.m_pd3dDevice, pItemBase->m_pTexture );
		}
		else
		if( dwShortcut == SHORTCUT_JACKBOX ) 
		{
			pItemBase = g_pPlayer->m_Cube.GetAt( dwId );//m_apItem[ i ] ;
			pItemBase->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, pItemBase->GetProp()->szIcon), 0xff000000 );
			pWndButton->SetTexture( g_Neuz.m_pd3dDevice, pItemBase->m_pTexture );
		}
		*/
		else
		if( dwShortcut == SHORTCUT_SKILL ) 
		{
			if( pShortcut->m_dwType == 2 )
			{
				SkillProp* pProp =  prj.GetPartySkill( pShortcut->m_dwId );
				if( pProp )
					pShortcut->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, pProp->szIcon/*pItemBase->GetProp()->szIcon*/), 0xffff00ff );
			}
			else
			{	
				LPSKILL lpSkill = g_pPlayer->GetSkill( dwType, dwId );

				if( lpSkill )
					pShortcut->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, lpSkill->GetProp()->szIcon/*pItemBase->GetProp()->szIcon*/), 0xffff00ff, TRUE );
			}
		}	
		else
		if( dwShortcut == SHORTCUT_SKILLFUN ) 
			pShortcut->m_pTexture = pTexture;
			
	}
	else 
		pShortcut->m_pTexture = pTexture;

	//sun: 8�� �۾�
	if( dwShortcut == SHORTCUT_ITEM )
	{
		FLItemBase* pItemBase	= g_pPlayer->GetItemId( dwId );
		if( pShortcut && pItemBase && pItemBase->GetProp()->dwPackMax > 1 )	// ���� ������ �������̸�?
			pShortcut->m_dwItemId	= pItemBase->m_dwItemId;
	}

	pShortcut->m_dwShortcut = dwShortcut   ;
	pShortcut->m_dwType     = dwType;
	pShortcut->m_dwIndex    = nIndex;//0;//dwIndex;
	pShortcut->m_dwId       = dwId;
	pShortcut->m_dwUserId   = 0 ;
	pShortcut->m_dwData     = nWhere;
	FLStrcpy( pShortcut->m_szString, _countof( pShortcut->m_szString ), m_GlobalShortcut.m_szString );//, sizeof(pShortcut->m_szString) );
	//pWndButton->SetTitle( m_GlobalShortcut.m_szString );
//sun: 12, ����
	if( dwShortcut == SHORTCUT_LORDSKILL)
		pShortcut->m_dwId--;
	if( nWhere == 0 ) //m_aSlotApplet
	{
		g_DPlay.SendAddAppletTaskBar( nIndex, pShortcut );
	}
	else
	if( nWhere == 1 ) //m_paSlotItem
	{
		g_DPlay.SendAddItemTaskBar( m_nSlotIndex, nIndex, pShortcut );
	}
	return TRUE;
}
BOOL CWndTaskBar::CheckAddSkill( int nSkillStyleSrc, int nQueueDest  )
{
	/*
	START -> FINISH, FG 
	CIRCLE -> START, CIRCLE
	FINISH -> START, CIRCLE
	*/
	DWORD dwComboStyleSrc = nSkillStyleSrc;
	if( nQueueDest == 0 )		// ùĭ�� ������ �õ� �������
	{
		/*
		if( �̹� ��ųâ�� ��ϵǾ� �ִ� )
		{
			if( �����Ϸ��°� CT_GENERAL�ΰ� )
				if( �̹� ���ԵǾ� �ִ°� CT_GENERAL�ΰ� )
				return TRUE;
			return FALSE;		// CT_GENERAL�̿��� ���� ùĭ�� �����Ϸ��� ��� ����.
		} else
		{
			// �ƹ��͵� ��ϵǾ� ���� �ʴ�.
			// ����, ���׷��� �հ�.
			if( dwComboStyleSrc == CT_STEP  || dwComboStyleSrc == CT_GENERAL  ) 
				return TRUE;
			return FALSE;	// �׿ܿ� ��� ���հ�.
		}
		 */
		if( m_nCurQueueNum == 0 )		// ����ִ�
		{
			if( dwComboStyleSrc == CT_STEP  || dwComboStyleSrc == CT_GENERAL  )		// �����̳� ���׷��̸� �հ�
				return TRUE;
			return	FALSE;
		} else
		// ���� �ִ�.
		{
			DWORD dwComboStyleDest;
			DWORD dwSkill = g_pPlayer->GetSkill( 0, m_aSlotQueue[ nQueueDest ].m_dwId )->dwSkill;
				//m_aSlotQueue[ nQueueDest ].m_dwId;	// �������� ĭ�� ��ų
			if( dwSkill )
				dwComboStyleDest = prj.GetSkillProp( dwSkill )->dwComboStyle;		// ��ĭ�� ��Ÿ��
			else 
				dwComboStyleDest = 0;
			if( dwComboStyleSrc == CT_GENERAL && dwComboStyleDest == CT_GENERAL )	// �̹��ִ°͵� ���׷��̰� �����Ϸ��°͵� ���׷��̸� �հ�
				return TRUE;
			return FALSE;	// �׿ܴ� ��� ùĭ�� ������ �� ����.
		}
	}
	else
	if( nQueueDest == 4 )		// ������ĭ�� �������� �������
	{
		if( dwComboStyleSrc == CT_STEP )	// ������ ùĭ�� �ƴϸ� ������ ���ִ´�.
			return FALSE;
		DWORD dwSkill = g_pPlayer->GetSkill( 0, m_aSlotQueue[ nQueueDest - 1 ].m_dwId )->dwSkill;
			//m_aSlotQueue[ nQueueDest - 1 ].m_dwId;	// �������� ĭ�� �տ��ִ� ��ų
		DWORD dwComboStylePrev;
		if( dwSkill )
			dwComboStylePrev = prj.GetSkillProp( dwSkill )->dwComboStyle;		// ��ĭ�� ��Ÿ��
		else
			dwComboStylePrev = 0;
		if( dwComboStylePrev == CT_FINISH )		// �� ��ų�� �ǴϽ��� ���̻� ���� �� ����.
			return FALSE;
		// ��Ŭ����� �������ϰ� ��ĭ�� ���׷��̸� ������.
		if( dwComboStyleSrc == CT_CIRCLE && dwComboStylePrev == CT_GENERAL )
			return FALSE;
		// �ǴϽ�(�ϳ�)�� �������ϰ� ��ĭ�� ���׷��̸� ������.
		if( dwComboStyleSrc == CT_FINISH && dwComboStylePrev == CT_GENERAL )
			return FALSE;
		// ���׷��� �������ϰ� ��ĭ�� ���׷��� �ƴϸ� ������.
		if( dwComboStyleSrc == CT_GENERAL && dwComboStylePrev != CT_GENERAL )
			return FALSE;
		
		return TRUE;	// �׿ܴ� ��� �հ�
	}
	else
	// �׿� �߰��� ������ �Ѱ��.
	{
		// ��°ĭ �̻���� �������� ������ ó��.
		if( dwComboStyleSrc == CT_STEP )	// ������ ùĭ�� �ƴϸ� ������ ���ִ´�.
			return FALSE;
		DWORD dwComboStylePrev;
		DWORD dwComboStyleDest;
		DWORD dwSkill = g_pPlayer->GetSkill( 0, m_aSlotQueue[ nQueueDest - 1 ].m_dwId )->dwSkill;
			//m_aSlotQueue[ nQueueDest - 1 ].m_dwId;	// �������� ĭ�� �տ��ִ� ��ų
		if( dwSkill )
			dwComboStylePrev = prj.GetSkillProp( dwSkill )->dwComboStyle;		// ��ĭ�� ��Ÿ��
		else
			dwComboStylePrev = 0;
		dwSkill = g_pPlayer->GetSkill( 0, m_aSlotQueue[ nQueueDest ].m_dwId )->dwSkill;
		//dwSkill = m_aSlotQueue[ nQueueDest ].m_dwId;	// �������� ĭ�� ��ų
		if( dwSkill )
			dwComboStyleDest = prj.GetSkillProp( dwSkill )->dwComboStyle;		// ��ĭ�� ��Ÿ��
		else
			dwComboStyleDest = 0;
		if( dwComboStylePrev == CT_FINISH )		// �� ��ų�� �ǴϽ��� ���̻� ���� �� ����.
			return FALSE;
		// ��Ŭ����� �������ϰ� ��ĭ�� ���׷��̸� ������.
		if( dwComboStyleSrc == CT_CIRCLE && dwComboStylePrev == CT_GENERAL )
			return FALSE;
		// �ǴϽ�(�ϳ�)�� �������ϰ� ��ĭ�� ���׷��̸� ������.
		if( dwComboStyleSrc == CT_FINISH && dwComboStylePrev == CT_GENERAL )
			return FALSE;
		// �ǴϽ��� �������ϰ� ��ĭ�� ��Ŭ�̸� ������.
		if( dwComboStyleSrc == CT_FINISH && dwComboStyleDest == CT_CIRCLE )
			return FALSE;
		// �ǴϽ��� �������ϰ� ��ĭ�� �ǴϽ��̸� ������.
		if( dwComboStyleSrc == CT_FINISH && dwComboStyleDest == CT_FINISH )
			return FALSE;
		// ���׷��� �������ϰ� 
		if( dwComboStyleSrc == CT_GENERAL )
		{
			if( dwComboStylePrev != CT_GENERAL )	// ��ĭ�� ���׷��� �ƴϸ� ������.
				return FALSE;
			if( dwComboStyleDest != CT_GENERAL && m_nCurQueueNum > nQueueDest )	// ��ĭ�� ���׷��� �ƴϸ� ���ִµ� �� ���̸� ������ �ִ�.
					return FALSE;
		}
		
		return TRUE;	// �� �ܴ� ��� �հ�
	}
	return FALSE;
}
BOOL CWndTaskBar::SetSkillQueue( int nIndex, DWORD dwType, DWORD dwId, CTexture* pTexture )
{
	//gmpbigsun( 20111215 ) : nIndex�� �ְ����ϴ� ���Թ�ȣ���� ���������� ��꿡 ���� ������ ���� ���Թ�ȣ�� ä����.

	if( m_nExecute )		return FALSE;		// ��ųť �����߿� ��� �ȵ�.
	if( m_nCurQueueNum >= 5 )
		return FALSE;

	// m_nCurQueueNum ���� ���� ���Կ� ����ִ°� �ִٸ� ���� ���´�. Ŭ��������.
	for( int i = 0; i < m_nCurQueueNum; i++ )
	{
		if( m_aSlotQueue[ i ].IsEmpty() )
		{
			for( i = 0; i < m_nCurQueueNum; i++ )
				m_aSlotQueue[ i ].Empty();
			m_nCurQueueNum = 0;
			break;
		}
	}
	 if( nIndex > m_nCurQueueNum )
		 nIndex = m_nCurQueueNum;

	LPSHORTCUT pShortcut = NULL;
	CRect rect = GetWindowRect();
	LPSKILL lpSkill = g_pPlayer->GetSkill( 0, dwId );
	FLERROR_ASSERT_LOG_RETURN( lpSkill, FALSE, _T("[������ �۾� ��] ��ų �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

	DWORD dwSkill = lpSkill->dwSkill;
	DWORD dwLevel = lpSkill->dwLevel;
	SkillProp *pSkillProp = prj.GetSkillProp( dwSkill );
	DWORD dwComboStyleSrc = pSkillProp->dwComboStyle;

	if( dwLevel <= 0 )
		return FALSE;

//sun: 9, ���ο� AddSkillProp �÷� �߰� �� ����
	AddSkillProp* pAddSkillProp	= prj.GetAddSkillProp( pSkillProp->dwSubDefine, dwLevel );
	FLASSERT( pAddSkillProp );
	if( (int)pAddSkillProp->dwCooldown > 0 )	//  ��Ÿ���ִ� ��ų�� �׼ǽ��Կ� ����
	{
		CString str;
		str.Format( prj.GetText(TID_GAME_SKILLLNOTUSE), pSkillProp->szName );
		g_WndMng.PutString( str , NULL, prj.GetTextColor(TID_GAME_SKILLLNOTUSE) );
		return FALSE;
	}

//	DWORD dwComboStyleDest;
//	SkillProp* pItemProp1, * pItemProp2,* pItemProp3; 
	if( nIndex < m_nCurQueueNum ) // insert		// nIndex�� ������ ĭ�� �ε���
	{
		if( !CheckAddSkill( dwComboStyleSrc, nIndex ) )		// Src�� nIndex�� �־ �Ǵ��� üũ
			return FALSE;
		for( int i = m_nCurQueueNum; i > nIndex; i-- )
		{
			memcpy( &m_aSlotQueue[ i ], &m_aSlotQueue[ i - 1 ], sizeof( SHORTCUT ) );
		}
		pShortcut = &m_aSlotQueue[ nIndex ];
		m_nCurQueueNum++;
	}
	else // add
	{
		if( !CheckAddSkill( dwComboStyleSrc,  m_nCurQueueNum ) )	// ���� ������ ĭ�� �־ �Ǵ��� �׽�Ʈ.
			return FALSE;
		pShortcut = &m_aSlotQueue[ m_nCurQueueNum ];
		nIndex = m_nCurQueueNum;
		m_nCurQueueNum++;
	}
	m_nCurQueue = -1;
	if( pTexture == NULL )
	{
		LPSKILL lpSkill = g_pPlayer->GetSkill( dwType, dwId );

		if( lpSkill )
			pShortcut->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, lpSkill->GetProp()->szIcon), 0xff000000, FALSE );
	}
	else 
		pShortcut->m_pTexture = pTexture;


	pShortcut->m_dwShortcut = SHORTCUT_SKILL   ;
	pShortcut->m_dwType     = dwType; // ���� 
	pShortcut->m_dwIndex    = nIndex;//dwIndex; // ��ų ��Ʈ�ѿ����� ���� 
	pShortcut->m_dwId       = dwId; // ��ų �ε��� 
	pShortcut->m_dwUserId   = 0 ;
	pShortcut->m_dwData     = 2;
	FLStrcpy( pShortcut->m_szString, _countof( pShortcut->m_szString ), m_GlobalShortcut.m_szString );
	g_DPlay.SendSkillTaskBar();
	// ��ų ��� ���⼭ ������ ������ ��
	return TRUE;
}

BOOL CWndTaskBar::OnDropIcon( LPSHORTCUT pShortcut, CPoint point )
{
	CRect rect = GetWindowRect();
	rect.DeflateRect( 3, 3);
	// ������ ������ ��� 
	if( pShortcut->m_pFromWnd == this )
	{
		LPSHORTCUT lpShortcut = (LPSHORTCUT) pShortcut->m_dwData;
		// ����Ʈ�� �׽�ũ�ٰ� �ƴ� ���̶��. �ǵ������� ������ ������� �½�ũ�� �ۿ� ���� ����. ������.
		if( GetClientRect().PtInRect( point ) == FALSE )
		{
			// ������ ���� ������ ����
			if( lpShortcut->m_dwData == 0 )
				g_DPlay.SendRemoveAppletTaskBar( (BYTE)( lpShortcut->m_dwIndex ) );
			else
			if( lpShortcut->m_dwData == 1 )
				g_DPlay.SendRemoveItemTaskBar( m_nSlotIndex, (BYTE)( lpShortcut->m_dwIndex ) );
			else
			if( lpShortcut->m_dwData == 2 )
			{
				RemoveSkillQueue( lpShortcut->m_dwIndex );
				return TRUE;
				//g_DPlay.SendSkillTaskBar( );
			}
			lpShortcut->Empty();
			return TRUE;
		}
	//	return TRUE;
	}
	CWndBase* pWndFrame =  pShortcut->m_pFromWnd->GetFrameWnd();

#ifdef __SKILL_UI16
	if( pWndFrame && ( pWndFrame->GetWndId() != APP_INVENTORY && pWndFrame != g_WndMng.m_pWndTaskBar && pWndFrame->GetWndId() != APP_COMMUNICATION_CHAT && pWndFrame->GetWndId() != APP_SKILL4 && pWndFrame->GetWndId() != APP_MOTION && pWndFrame->GetWndId() != APP_PARTY && pWndFrame->GetWndId() != APP_LORD_SKILL) )
#else 

//sun: 9�� ���°��� Clienet
//sun: 12, ����
	if( pWndFrame && ( pWndFrame->GetWndId() != APP_INVENTORY && pWndFrame != g_WndMng.m_pWndTaskBar && pWndFrame->GetWndId() != APP_COMMUNICATION_CHAT && pWndFrame->GetWndId() != APP_SKILL3 && pWndFrame->GetWndId() != APP_MOTION && pWndFrame->GetWndId() != APP_PARTY && pWndFrame->GetWndId() != APP_LORD_SKILL) )
#endif //__SKILL_UI16
	{
		SetForbid( TRUE );
		return FALSE;
	}
	
	if( pShortcut->m_dwShortcut == SHORTCUT_SKILL && pShortcut->m_dwType != 2 ) // �شܽ�ų�� �ȵ�
	{
		if( !g_pPlayer->isLearnSkill( pShortcut->m_dwId ) )
// 		LPSKILL pSkill = g_pPlayer->GetSkill( 0, pShortcut->m_dwId );
// 		if( pSkill && (pSkill->dwLevel <= 0 || g_pPlayer->CheckSkill( pSkill->dwSkill ) == FALSE) )
		{
			SetForbid( TRUE );
			return FALSE;
		}
	}
//sun: 12, ����
	if( pShortcut->m_dwId == 0 && strlen( pShortcut->m_szString ) < 1 && pShortcut->m_dwShortcut != SHORTCUT_SKILLFUN && pShortcut->m_dwShortcut != SHORTCUT_LORDSKILL)
	{
		SetForbid( TRUE );
		return FALSE;
	}
	// ���ο��� �̵��ǰų�, �ܺο��� ���η� 
	if( m_nPosition == TASKBAR_TOP || m_nPosition == TASKBAR_BOTTOM )
	{
		BOOL bForbid = TRUE;
		CRect rect = RECT_APPLET;
		if( rect.PtInRect( point ) )
		{
			point.x -= rect.left;
			point.y -= rect.top;
			point.x /= ICON_SIZE;
			// ���ο��� �̵� 
			if( pShortcut->m_pFromWnd == this )
			{
				LPSHORTCUT lpCurShortcut = (LPSHORTCUT) pShortcut->m_dwData;			

				if( &m_aSlotApplet[ point.x ] == lpCurShortcut )
				{
					//SetForbid( TRUE );
					return FALSE;
				}
				// ���������� �׼ǽ����̴�.
				//if( lpCurShortcut->m_dwData == 2 )
				//	RemoveSkillQueue( lpCurShortcut->m_dwIndex );
			}
			if( pShortcut->m_dwShortcut == SHORTCUT_SKILL )	
			{
				SkillProp* pProp;
				if( pShortcut->m_dwType == 2 )
					pProp =  prj.GetPartySkill( pShortcut->m_dwId );
				else
				{
					LPSKILL pSkill = g_pPlayer->GetSkill( 0, pShortcut->m_dwId );
					
					if( pSkill )
						pProp = prj.GetSkillProp( pSkill->dwSkill );	
				}
				if( pProp->dwComboStyle != CT_STEP && pProp->dwComboStyle != CT_GENERAL )	
				{
					SetForbid( TRUE );
					return FALSE;	// �����Կ� ����Ϸ��� ��ų�� ���ܱ���� �ƴѰ� ��� ����.
				}
/*				
				SkillProp *pProp = prj.GetSkillProp( pShortcut->m_dwId );	
				if( pProp->dwComboStyle != CT_STEP && pProp->dwComboStyle != CT_GENERAL )	
				{
					SetForbid( TRUE );
					return FALSE;	// �����Կ� ����Ϸ��� ��ų�� ���ܱ���� �ƴѰ� ��� ����.
				}				
				*/
				//if( pProp->dwComboStyle != CT_STEP )
				//{
				//	SetForbid( TRUE );
				//	return FALSE;	// �����Կ� ����Ϸ��� ��ų�� ���ܱ���� �ƴѰ� ��� ����.
				//}
			}

#ifdef __MAINSERVER
			if( point.x == 0 && m_aSlotApplet[ point.x ].m_dwId == 400 )
				return FALSE;
#endif //__MAINSERVER
				
			SetShortcut( point.x, pShortcut->m_dwShortcut, pShortcut->m_dwType, pShortcut->m_dwId, pShortcut->m_pTexture, 0 );
			bForbid = FALSE;
		}
		rect = RECT_ITEM;
//		rect.right += 32;
		if( rect.PtInRect( point ) )
		{
			point.x -= rect.left;
			point.y -= rect.top;
			point.x /= ICON_SIZE;
			// ���ο��� �̵� 
			if( pShortcut->m_pFromWnd == this )
			{
				LPSHORTCUT lpCurShortcut = (LPSHORTCUT) pShortcut->m_dwData;
				if( &m_aSlotItem[ m_nSlotIndex ][ point.x ] == lpCurShortcut )
					return FALSE;
				// ���������� �׼ǽ����̴�.
				//if( lpCurShortcut->m_dwData == 2 )
				//	RemoveSkillQueue( lpCurShortcut->m_dwIndex );
			}
			if( pShortcut->m_dwShortcut == SHORTCUT_CHAT )
			{
				CString sChat	= m_GlobalShortcut.m_szString;
				sChat.Replace( "--", "" );
				sChat.Replace( "'", "" );
				sChat.Replace( "/*", "" );
				sChat.Replace( "*/", "" );
				FLStrcpy( m_GlobalShortcut.m_szString, _countof( m_GlobalShortcut.m_szString ), (const char*)sChat );
			}
			else if( pShortcut->m_dwShortcut == SHORTCUT_SKILL )	
			{
				SkillProp* pProp( NULL );
				if( pShortcut->m_dwType == 2 )
					pProp =  prj.GetPartySkill( pShortcut->m_dwId );
				else
				{
					LPSKILL pSkill = g_pPlayer->GetSkill( 0, pShortcut->m_dwId );
					
					if( pSkill )
						pProp = prj.GetSkillProp( pSkill->dwSkill );	
				}
				if( pProp->dwComboStyle != CT_STEP && pProp->dwComboStyle != CT_GENERAL )	
				{
					SetForbid( TRUE );
					return FALSE;	// �����Կ� ����Ϸ��� ��ų�� ���ܱ���� �ƴѰ� ��� ����.
				}
			}
			SetShortcut( point.x, pShortcut->m_dwShortcut, pShortcut->m_dwType, pShortcut->m_dwId, pShortcut->m_pTexture, 1 );
			bForbid = FALSE;
		}
		rect = RECT_QUEUE;
		if( rect.PtInRect( point ) )
		{
			if( pShortcut->m_dwShortcut == SHORTCUT_SKILL && pShortcut->m_dwType == 0 )		// ��ų�ϰ�츸 ���
			{
				point.x -= rect.left;
				point.y -= rect.top;
				point.x /= SKILL_SIZE;
				// ���ο��� �̵� ; ������ �Ʒ� �κп��� ������ ó���ϳ�, ��ų ť������ �̵��� ������ �ٷ� �߰��ؾ��ϱ� ������ ���⼭ ������ �Ѵ�.
				if( pShortcut->m_pFromWnd == this )
				{
					LPSHORTCUT lpShortcut = (LPSHORTCUT) pShortcut->m_dwData;
					if( lpShortcut->m_dwData == 2 )
					{
						RemoveSkillQueue( lpShortcut->m_dwIndex, FALSE );
						SetSkillQueue( point.x, pShortcut->m_dwType, pShortcut->m_dwId, pShortcut->m_pTexture );// == FALSE )
						return TRUE;
					}
				}
				if( SetSkillQueue( point.x, pShortcut->m_dwType, pShortcut->m_dwId, pShortcut->m_pTexture ) == FALSE )
					SetForbid( TRUE );
				bForbid = FALSE;
			}
		}
		if( pShortcut->m_pFromWnd != this )
			SetForbid( bForbid );
	}
	// ������ ������ ���, �Ǵ� �ȿ��� �̵����� ��� �������� �κ� ����  
	// �½�ũ�ٿ� �ִ� �������� ���� ���� ���� �κп� �巡������ ��� 
	if( pShortcut->m_pFromWnd == this )
	{
		LPSHORTCUT lpShortcut = (LPSHORTCUT) pShortcut->m_dwData;
		// ������ ���� ������ ����
		if( lpShortcut->m_dwData == 0 )
			g_DPlay.SendRemoveAppletTaskBar( (BYTE)( lpShortcut->m_dwIndex ) );
		else
		if( lpShortcut->m_dwData == 1 )
			g_DPlay.SendRemoveItemTaskBar( m_nSlotIndex, (BYTE)( lpShortcut->m_dwIndex ) );
		else
		if( lpShortcut->m_dwData == 2 )
		{
			RemoveSkillQueue( lpShortcut->m_dwIndex );
			//g_DPlay.SendSkillTaskBar( );
			return TRUE;
		}
		lpShortcut->Empty();
	}
	return TRUE;
}
LPSHORTCUT CWndTaskBar::Select( CPoint point )
{
	CRect rect = RECT_APPLET;
	LPSHORTCUT pShortcut = NULL;
	if( rect.PtInRect( point ) )
	{
		point.x -= rect.left;
		point.y -= rect.top;
		point.x /= ICON_SIZE;
		pShortcut = &m_aSlotApplet[ point.x ];
		if( !pShortcut->IsEmpty() )
		{
			return pShortcut;
		}
	}
	rect = RECT_ITEM;
	if( rect.PtInRect( point ) )
	{
		point.x -= rect.left;
		point.y -= rect.top;
		point.x /= ICON_SIZE;
		pShortcut = &m_paSlotItem[ point.x ];
		if( !pShortcut->IsEmpty() )
			return pShortcut;
	}
	rect = RECT_QUEUE;
	if( rect.PtInRect( point ) )
	{
		point.x -= rect.left;
		point.y -= rect.top;
		point.x /= SKILL_SIZE;
		pShortcut = &m_aSlotQueue[ point.x ];
		if( !pShortcut->IsEmpty() )
			return pShortcut;

	}
	rect = CRect( POINT_QUEUE_X - ICON_SIZE - 5, POINT_QUEUE_Y, (POINT_QUEUE_X - ICON_SIZE - 5) + 32, POINT_QUEUE_Y + 32);
	if( rect.PtInRect( point ) )
	{
		m_aSlotSkill.m_dwShortcut = SHORTCUT_SKILLFUN;

		pShortcut   = &m_aSlotSkill;

		if( !pShortcut->IsEmpty() )
			return pShortcut;
	}

	return NULL;
}
void CWndTaskBar::OnMouseMove(UINT nFlags, CPoint point)
{
	if( m_bLButtonDown && IsPush() )
	{
	//PSHORTCUT pShortcut = Select( point );
	//	if( pShortcut && pShortcut == m_pSelectShortcut && !pShortcut->IsEmpty() )
		if( m_pSelectShortcut && !m_pSelectShortcut->IsEmpty() ) //&& m_pSelectShortcut->m_dwShortcut != SHORTCUT_SKILL )
		{
#ifdef __MAINSERVER
			if( m_pSelectShortcut->m_dwId == 400 )
				return;
#endif //__MAINSERVER
//sun: 12, ����
			if(m_pSelectShortcut->m_dwShortcut == SHORTCUT_LORDSKILL)
			{
				m_pSelectShortcut->m_dwId++;
			}
			memcpy( &m_GlobalShortcut, m_pSelectShortcut, sizeof( m_GlobalShortcut ) );
			m_GlobalShortcut.m_pFromWnd = this;
			m_GlobalShortcut.m_dwData = (DWORD)m_pSelectShortcut;
			m_pSelectShortcut = NULL;
		}
	}
	CWndNeuz::OnMouseMove( nFlags, point );
}

void CWndTaskBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(g_WndMng.m_pWndMenu && g_WndMng.m_pWndMenu->IsVisible())
		g_WndMng.m_pWndMenu->SetVisible(FALSE);

	m_pSelectShortcut = Select( point );
	SetCapture();
}
void CWndTaskBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	LPSHORTCUT pShortcut = Select( point );
	
	if( pShortcut && pShortcut == m_pSelectShortcut && !pShortcut->IsEmpty() && pShortcut->m_dwData != 2 )
	{
		g_WndMng.ObjectExecutor( pShortcut );
	}

}

BOOL CWndTaskBar::OnEraseBkgnd(C2DRender* p2DRender)
{
	return TRUE;
}
void CWndTaskBar::PaintFrame( C2DRender* p2DRender )
{
	CRect rect = GetWindowRect();
	//m_pTheme->RenderWndTaskBar( p2DRender, &rect );
	CWndBase::PaintFrame( p2DRender );
}

LPSKILL CWndTaskBar::GetCurrentSkillQueue()
{
	LPSKILL pSkill = NULL;

	if( m_nUsedSkillQueue >= SKILL_QUEUE_USE )
	{
		LPSHORTCUT pShortcut = &m_aSlotQueue[ m_nUsedSkillQueue ];

		if( pShortcut && pShortcut->IsEmpty() == FALSE )
			pSkill = g_pPlayer->GetSkill( 0, pShortcut->m_dwId );
	}

	return pSkill;
}
//
// ��ųť ���� ��!
//
BOOL CWndTaskBar::UseSkillQueue( CCtrl* pTargetObj )
{
	if( m_bLockActionSlot )
		return FALSE;

	if( m_nExecute > 0 )	return FALSE;	// �̹� ��ų�� �������̶�� �ٽ� ������ ����.
	if( g_pPlayer->m_pActMover->IsActAttack() )			return FALSE;		// �̹� ���ݵ����� �̷����� ������ ������ ����.
	if( g_pPlayer->m_pActMover->IsActJump() )			return FALSE;		// �����߿� ��ų��� �ȵ�.
//	if( g_pPlayer->m_pActMover->IsActDamage() )			return FALSE;		// ������ ����߿� ��ų��� �ȵ�.
	if( g_pPlayer->m_pActMover->IsFly() )				return FALSE;
//	if( g_pPlayer->m_dwFlag & MVRF_SKILL )		return FALSE;				// �̹� ��ų ������̸� ��� �ȵ�.
	if( pTargetObj && pTargetObj->GetType() != OT_MOVER )	return FALSE;
	if( g_pPlayer->m_dwReqFlag & REQ_USESKILL )
	{
		return FALSE;
	}

	// ������Ʈ�� ������ Ŭ�� ������
	if( pTargetObj )
	{
		if( pTargetObj->GetType() == OT_OBJ )		// ������Ʈ�� OT_OBJ(���)�̸� ���� ���Ѱɷ� ������.
			m_idTarget = NULL_ID;
		else
			m_idTarget = ((CCtrl*)pTargetObj)->GetId();		// OT_OBJ�� �ƴϸ� ���̵� ������
	}
	else
		m_idTarget = NULL_ID;		// Ÿ���� �������� �ʾ����� NULL_ID
	CMover *pTargetMover = (CMover*)pTargetObj;		// prj.GetMover( m_idTarget );
	if( IsInvalidObj( pTargetMover ) )		// �Žñ��� Ÿ���̾����� Ÿ�� ���Ѱɷ� ����.
		m_idTarget = NULL_ID;

	
	m_nUsedSkillQueue = SKILL_QUEUE_USE;
	
	LPSHORTCUT pShortcut = &m_aSlotQueue[ m_nUsedSkillQueue ];

	if( pShortcut->IsEmpty() == FALSE )
	{
		LPSKILL pSkill = g_pPlayer->GetSkill( 0, pShortcut->m_dwId );
		if( pSkill == NULL )	return FALSE;
		
		if( g_pPlayer->IsBullet( pSkill->GetProp() ) == FALSE )
			return FALSE;

		m_nExecute = 1;		// 1 ��ų�ٻ�� �������� 
		// ���⿣ m_idTarget�� NULL_ID�� ������ �ִ�.
		if( g_pPlayer->CMD_SetUseSkill( m_idTarget, pShortcut->m_dwId, SUT_QUEUESTART ) == 0 )		// ������ ����� ����. �̵� + ��ų����� ������ ���.
		{
			OnCancelSkill();	// ù��ų���� �����ߴٸ� ��ųť ����� ���.
		}
// 		else
// 		{	// success
// 			// 1�ܰ� ������ ap�� �Ҹ���� �ʴ´�.
// 			PT_ITEM_SPEC pItemProp = g_pPlayer->GetActiveHandItemProp();
// 			if( pItemProp )
// 			{   // �տ� ��� �ִ°� �������� ġ�ƽ�� �ƴҶ��� NEXTSKILL_NONE�� ����. ���ϸ� ��ų�� �̾����� ����.
// 				if( pItemProp->dwItemKind3 != IK3_STAFF && pItemProp->dwItemKind3 != IK3_CHEERSTICK )			
// 					g_WndMng.m_pWndWorld->SetNextSkill( NEXTSKILL_NONE );	// 2006/06/12 ��ųť�� ���������� ��ųť��� ��� Ŭ����.-xuzhu-
// 			}
// 
// 		}
		return TRUE;
	} else
		g_WndMng.m_pWndWorld->SetNextSkill( NEXTSKILL_NONE );	// �׼ǽ�ų�� ��������� ���.

	return FALSE;
}

BOOL CWndTaskBar::Process( void )
{
	// ���� ��� �����ؾ��ϴ°͵��� ���⼭...
	CPoint point = GetMousePoint();

	CRect rect = CRect( POINT_APPLET_X, POINT_APPLET_Y, POINT_APPLET_X + ICON_SIZE, POINT_APPLET_Y + ICON_SIZE );
	for( int i = 0; i < m_nMaxSlotApplet; i++ )
	{
		LPSHORTCUT lpShortcut = &m_aSlotApplet[ i ] ;
		if( !lpShortcut->IsEmpty() && rect.PtInRect( point) )
		{
			MotionProp* pMotionProp = prj.GetMotionProp( lpShortcut->m_dwId );
			
			if( IsShortcut( lpShortcut, SHORTCUT_MOTION, MOT_BASE_CHEER ) )
				PutTooTip( lpShortcut, point, &rect );
		}
		rect += CPoint( ICON_SIZE, 0 );
	}
	
	rect = CRect( POINT_ITEM_X, POINT_ITEM_Y, POINT_ITEM_X + ICON_SIZE, POINT_ITEM_Y + ICON_SIZE );
	for( int i = 0; i < MAX_SLOT_ITEM; i++ )
	{
		LPSHORTCUT lpShortcut = &m_paSlotItem[ i ] ;
		if( !lpShortcut->IsEmpty() && rect.PtInRect( point)  )
		{
			if( IsShortcut( lpShortcut, SHORTCUT_MOTION, MOT_BASE_CHEER ) )
				PutTooTip( lpShortcut, point,&rect );
		}
		rect += CPoint( ICON_SIZE, 0 );
	}
	rect = CRect( POINT_QUEUE_X, POINT_QUEUE_Y, POINT_QUEUE_X + SKILL_SIZE, POINT_QUEUE_Y + SKILL_SIZE );
	for( int i = 0; i < MAX_SLOT_QUEUE; i++ )
	{
		LPSHORTCUT lpShortcut = &m_aSlotQueue[ i ] ;
		if( !lpShortcut->IsEmpty() && rect.PtInRect( point)  )
		{
			if( IsShortcut( lpShortcut, SHORTCUT_MOTION, MOT_BASE_CHEER ) )
				PutTooTip( lpShortcut, point,&rect );
		}
		rect += CPoint( SKILL_SIZE, 0 );
	}

	if( m_nExecute )
	{
		if( m_idTarget != NULL_ID )		// m_idTarget�� NULL_ID�ΰ��� �ڱ� �ڽſ��� �����ⶫ�� ĵ�����Ѽ� �ȵȴ�.
		{
			CMover* pObj = prj.GetMover( m_idTarget );
			if( IsInvalidObj(pObj) )						// �����߿� ����� �������ٸ� ��ų����� ����.
				OnCancelSkill();
		}
//		if( pObj && pObj->IsDie() )						// ������ ����� �׾����� ��ų�� ���� ����.
//			OnCancelSkill();

//		if( (g_pPlayer->m_dwFlag & MVRF_SKILL) == 0 )
//			OnCancelSkill();													// ��ų��� ����
	}
	// �巡�� ���̶���Ʈ ���� ���� ó�� 
	if( m_GlobalShortcut.IsEmpty() == FALSE )
	{
		m_dwHighAlpha += 10;
		if( m_dwHighAlpha >= 256 )
			m_dwHighAlpha = 0;
	}
	// �巡���߿��� ��Ű�� ����� �� ���� 
	else
	{
		const PFLSHORTCUTKEY_SYSTEM		pShortcutKey_System( g_xKeyManager->GetShortcutKey_System() );

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ��Ű ó�� 
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if( g_bKeyTable[ pShortcutKey_System->KeyEnter ] ) 
		{
			g_bKeyTable[ pShortcutKey_System->KeyEnter ] = FALSE;
			CWndBase* pWndBase = GetWndBase( APP_COMMUNICATION_CHAT );
			if( pWndBase == NULL )
			{
				g_WndMng.ObjectExecutor( SHORTCUT_APPLET, APP_COMMUNICATION_CHAT );
				CWndChat* pWndChat = (CWndChat*)GetWndBase( APP_COMMUNICATION_CHAT );
				if( pWndChat != NULL )
				{
					pWndChat->m_wndEdit.SetFocus();
				}
			}
		}
		else
		{
			AppletFunc* pApplet;
			DWORD dwIdApplet;
			POSITION pos = g_WndMng.m_mapAppletFunc.GetStartPosition();
			BOOL bKey = FALSE;
			while( pos )
			{
				g_WndMng.m_mapAppletFunc.GetNextAssoc( pos, dwIdApplet, (void*&)pApplet );
				
				if( g_bKeyTable[ pApplet->m_cHotkey ] )			//gmpbigsun: �Ͽ�¡ YŰ( ����â ) �˻� 
				{
					g_Error_State.m_ch = pApplet->m_cHotkey;
					g_Error_State.m_bIsShowWnd = TRUE;
					g_Error_State.m_dwWndId = pApplet->m_dwIdApplet;
					
					g_WndMng.ObjectExecutor( SHORTCUT_APPLET, pApplet->m_dwIdApplet );
					g_bKeyTable[ pApplet->m_cHotkey ] = FALSE;
				}
			}
			
			CWndRegVend* pWndRegVend = (CWndRegVend*)g_WndMng.GetWndBase(APP_VENDOREX_SELL);
			CWndRegVend* pWndVendorBuy = (CWndRegVend*)g_WndMng.GetWndBase(APP_VENDOREX_BUY);
			
			if( !pWndRegVend || !pWndVendorBuy )
			{
				DWORD *dwHotkey;
#ifdef __Y_INTERFACE_VER3				
				DWORD dwHotkey1[10] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
				DWORD dwHotkey2[10] = { 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79 };

				if( g_Option.m_nInterface == 2 )
					dwHotkey = dwHotkey1;
				else
					dwHotkey = dwHotkey2;								
#else //__Y_INTERFACE_VER3
				DWORD dwHotkey1[10] = { 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79 };
				dwHotkey = dwHotkey1;
#endif //__Y_INTERFACE_VER3

				for( int i = 0; i < 10; i++ )
				{
					if( g_bKeyTable[ dwHotkey[i] ] && g_bSlotSwitchAboutEquipItem[ i ] == FALSE )
					{
						LPSHORTCUT lpShortcut = &m_paSlotItem[ i ] ;
						if( lpShortcut->m_dwShortcut == SHORTCUT_ITEM )
						{
							FLItemBase* pItemBase = g_pPlayer->GetItemId( lpShortcut->m_dwId );
							if( pItemBase )
							{
								int nPart = pItemBase->GetProp()->dwParts;
								if( nPart != -1 )
									g_bSlotSwitchAboutEquipItem[ i ] = TRUE;
							}
						}
						g_WndMng.ObjectExecutor( lpShortcut );	
						g_bKeyTable[ dwHotkey[i] ] = 0;
					}
				}			
			}
		}
	}

	UpdateItem();
	
	return TRUE;
	//return CWndBase::Process();
}

// ��ų�� ����� ���(�ߴ�)��.
void CWndTaskBar::OnCancelSkill( void )
{
	if( NULL ==  g_pPlayer )
		return;

	if( g_pPlayer->m_dwReqFlag & REQ_USESKILL )
		return;

	m_nExecute = 0;
	m_nUsedSkillQueue = SKILL_QUEUE_NONE;
	g_pPlayer->ClearDestObj();
	g_WndMng.m_pWndWorld->SetNextSkill( NEXTSKILL_NONE );
}

// ��ų ���� �������� ȣ��.
void CWndTaskBar::OnEndSkill( void )
{
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �½�ũ ���� �Ŵ� ó�� ����  
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

CWndTaskMenu::CWndTaskMenu()
{
	m_pMenu1 = NULL;
	m_pMenu2 = NULL;
	m_pMenu3 = NULL;
	m_pMenu4 = NULL;
	m_pMenu5 = NULL;
	m_pMenu6 = NULL;
	m_pMenu7 = NULL;
}

CWndTaskMenu::~CWndTaskMenu()
{
	SAFE_DELETE( m_pMenu1 );
	SAFE_DELETE( m_pMenu2 );
	SAFE_DELETE( m_pMenu3 );
	SAFE_DELETE( m_pMenu4 );
	SAFE_DELETE( m_pMenu5 );
	SAFE_DELETE( m_pMenu6 );
	SAFE_DELETE( m_pMenu7 );
}
void CWndTaskMenu::OnDraw(C2DRender* p2DRender)
{
	return;
	p2DRender->TextOut( 10, 20,  _T( "MASQUERADE" ),  D3DCOLOR_ARGB( 255, 250, 250, 250 ) );

	CRect rect = GetWindowRect();
	// �׵θ� �ڽ� 
	DWORD dwColor1t = D3DCOLOR_ARGB( 055,  91, 104, 205 );
	DWORD dwColor2t = D3DCOLOR_ARGB( 055, 116, 128, 220 );
	DWORD dwColor4t = D3DCOLOR_ARGB( 055, 143, 173, 245 );
	DWORD dwColor1b = D3DCOLOR_ARGB( 055,  41, 104, 155 );
	DWORD dwColor2b = D3DCOLOR_ARGB( 055,  66,  78, 170 );
	DWORD dwColor4b = D3DCOLOR_ARGB( 055, 143, 173, 245 );

	DWORD dwColor3t = D3DCOLOR_ARGB( 155, 250, 250, 250 );
	DWORD dwColor3b = D3DCOLOR_ARGB(  55,  00,  00,  00 );
	
	//p2DRender->RenderRoundRect( rect, dwColor1t, dwColor1t, dwColor1b, dwColor1b );
	rect.DeflateRect(1,1);
	//p2DRender->RenderRoundRect( rect, dwColor2t, dwColor2t, dwColor2b, dwColor2b );
//	rect.DeflateRect(1,1);

	CRect rect1 = rect; rect1.bottom = rect1.top + 40;
	CRect rect2 = rect; rect2.top    = rect2.top + 40;

	p2DRender->RenderFillRect( rect1, dwColor3t, dwColor3t, dwColor3b, dwColor3b );
	p2DRender->RenderFillRect( rect2, dwColor3b, dwColor3b, dwColor4b, dwColor4b );

//	p2DRender->RenderLine( CPoint( 10, 5 * 22 + 54 ), CPoint( 140, 5 * 22 + 54 ), dwColor1t );
	// client ���� ĥ�ϱ� 
	//OnEraseBkgnd(p2DRender);
	//.OffsetRect(-rect.TopLeft());
	//p2DRender->PaintRect( rect,MKHIGHRGB(255>>3,255>>3,255>>3) );
	//p2DRender->TextOut( 5,20,"HP : 100 / 100", dwColor1t );
	//p2DRender->TextOut( 5,35,"MP : 100 / 100", dwColor1t );
}
BOOL CWndTaskMenu::Process()
{
	if(	CWndBase::m_GlobalShortcut.m_dwShortcut != SHORTCUT_NONE )
		return CWndBase::Process();

	if( IsVisible() == FALSE )
		return CWndBase::Process();

	for( int i = 0; i < m_awndMenuItem.GetSize(); i++ )
	{
		BOOL bHighLight = FALSE;
		CWndButton* pWndButton = (CWndButton*)m_awndMenuItem.GetAt( i );
		if( pWndButton->GetClientRect( TRUE ).PtInRect( m_ptMouse ) )
		{
			// ��� ����� 
			if( pWndButton->m_pWndMenu == NULL || pWndButton->m_pWndMenu->IsVisible( ) == FALSE ) 
			{
				for( int i2 = 0; i2 < m_awndMenuItem.GetSize(); i2++)
				{
					if( ((CWndButton*)m_awndMenuItem.GetAt( i2 ) )->m_pWndMenu )
						((CWndButton*)m_awndMenuItem.GetAt( i2 ) )->m_pWndMenu->SetVisibleSub( FALSE );
				}
			}
			// �� �Ŵ��� ���̰� ��Ŀ�� �ֱ� 
			if( pWndButton->m_pWndMenu )
			{
				if( pWndButton->m_pWndMenu->IsVisible() == FALSE )
				{
					CRect rect = pWndButton->GetScreenRect();
					pWndButton->m_pWndMenu->Move( CPoint( rect.right , rect.top ) );
				}
				// �޴��� ��ǥ ���� 
				CRect rcButton = pWndButton->GetScreenRect();
				pWndButton->m_pWndMenu->Move( CPoint( rcButton.right, rcButton.top ) );
				// �׷��� �� �޴��� ȭ���� ����ٸ� ��ġ�� ���� 
				CRect rcMenu = pWndButton->m_pWndMenu->GetScreenRect();
				CRect rcLayout = m_pWndRoot->GetLayoutRect();
				CPoint pt = rcMenu.TopLeft();
				if( rcMenu.right > rcLayout.right )
				{
					pt.x = rcButton.left - rcMenu.Width();
					//pWndButton->m_pWndMenu->Move( CPoint( rcButton.left - rcMenu.Width(), rcButton.top ) );
				}
				if( rcMenu.bottom > rcLayout.bottom )
				{
					pt.y -= rcMenu.bottom - rcLayout.bottom;
					//pWndButton->m_pWndMenu->Move( CPoint( rcButton.left - rcMenu.Width(), rcButton.top ) );
				}
				pWndButton->m_pWndMenu->Move( pt );
				pWndButton->m_pWndMenu->SetVisible( TRUE );
				pWndButton->m_pWndMenu->SetFocus();
			}
		}
	}
	return CWndBase::Process();
}
void CWndTaskMenu::SetTexture( CWndButton* pWndButton )
{
	pWndButton->SetTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, g_WndMng.GetAppletFunc( pWndButton->GetWndId() )->m_pszIconName ) );
}

void CWndTaskMenu::OnInitialUpdate()
{
	CWndMenu::OnInitialUpdate();

	CWndButton* pWndButton;
	// 0 ~ 9�� 
	if( g_pPlayer->IsAuthHigher( AUTH_GAMEMASTER ) )
	{
		pWndButton = AppendMenu( this, 0, APP_DEBUGINFO     , GETTEXT( TID_APP_DEBUGINFO     ) ); 
	}

	pWndButton = AppendMenu( this, 0, APP_STATUS1    , GETTEXT( TID_APP_STATUS        ) );//05
	pWndButton = AppendMenu( this, 0, APP_NAVIGATOR , GETTEXT( TID_APP_NAVIGATOR     ) );
//sun: 13, ĳ��������â ������
	pWndButton = AppendMenu( this, 0, APP_CHARACTER3 , GETTEXT( TID_APP_CHARACTER     ) );

	pWndButton = AppendMenu( this, 0, APP_INVENTORY , GETTEXT( TID_APP_INVENTORY     ) );
//sun: 11, �ָӴ�
	pWndButton = AppendMenu( this, 0, APP_BAG_EX , GETTEXT( TID_APP_BAG_EX     ) );

	pWndButton = AppendMenu( this, 0, APP_HOUSING, GETTEXT(TID_GAME_HOUSING_BOX));			//sun: 13, �Ͽ�¡ �ý���

//sun: 8, // __S8_VENDOR_REVISION
	pWndButton = AppendMenu( this, 0, APP_VENDOR_REVISION , GETTEXT( TID_APP_VENDOR     ) );
//!	pWndButton = AppendMenu( this, 0, APP_VENDOREX , GETTEXT( TID_APP_VENDOR     ) );

#ifdef __SKILL_UI16
	pWndButton = AppendMenu( this, 0, APP_SKILL4     , GETTEXT( TID_APP_SKILL         ) );
#else //sun: 9�� ���°��� Clienet
	pWndButton = AppendMenu( this, 0, APP_SKILL3     , GETTEXT( TID_APP_SKILL         ) );
#endif //__SKILL_UI16

	pWndButton = AppendMenu( this, 0, APP_QUEST_EX_LIST     , GETTEXT( TID_APP_QUEST         ) );
	pWndButton = AppendMenu( this, 0, APP_MOTION   , GETTEXT( TID_APP_MOTION       ) );

//sun: 9, ��ŸƮ�޴��� ��ü���� �޴� �߰�
#ifdef __IMPROVE_MAP_SYSTEM
	pWndButton = AppendMenu( this, 0, APP_MAP_EX, GETTEXT( TID_APP_MAP ) );
#else // __IMPROVE_MAP_SYSTEM
	pWndButton = AppendMenu( this, 0, APP_MAP, GETTEXT( TID_APP_MAP));//GETTEXT( TID_APP_MAP));
#endif // __IMPROVE_MAP_SYSTEM

	CWndButton* pWndButton7 = NULL;
	pWndButton7 = AppendMenu( this, 0, 0 , GETTEXT( TID_APP_COMMITEM  ) ); pWndButton7->SetTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, _T( "icon_Folder.dds" ) ) ); 

	CWndButton* pWndButton1 = AppendMenu( this, 0, 0 , GETTEXT( TID_APP_COMMUNICATION ) ); pWndButton1->SetTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, _T( "icon_Folder.dds" ) ) );
	CWndButton* pWndButton2 = AppendMenu( this, 0, 0 , GETTEXT( TID_APP_COMMUNITY     ) ); pWndButton2->SetTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, _T( "icon_Folder.dds" ) ) );
//	CWndButton* pWndButton3 = AppendMenu( this, 0, 0 , GETTEXT( TID_APP_FIND          ) ); pWndButton3->SetTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, _T( "icon_Folder.dds" ) ) );
	CWndButton* pWndButton5 = AppendMenu( this, 0, 0 , GETTEXT( TID_APP_INFOMATION    ) ); pWndButton5->SetTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, _T( "icon_Folder.dds" ) ) );
	CWndButton* pWndButton6 = AppendMenu( this, 0, 0 , GETTEXT( TID_APP_HELPER        ) ); pWndButton6->SetTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, _T( "icon_Folder.dds" ) ) ); // �����, �ó���
	
	CWndButton* pWndButton4 = NULL;
	switch( _GetContentState( CT_OPTION_17 ) )
	{
	case CS_VER1:
		pWndButton4 = AppendMenu( this, 0, APP_OPTIONEX, GETTEXT(TID_APP_OPTION) );
		break;

	case CS_VER2:
		pWndButton4 = AppendMenu( this, 0, APP_OPTIONEX2, GETTEXT(TID_APP_OPTION) );
		break;

	default: 
		FLASSERT( 0 );
		break;
	}

	pWndButton = AppendMenu( this, 0, APP_LOGOUT    , GETTEXT( TID_APP_LOGOUT      ) );
	pWndButton = AppendMenu( this, 0, APP_QUIT      , GETTEXT( TID_APP_QUIT          ) );

//	if( g_xFlyffConfig->GetMainLanguage() == LANG_TWN )
//		pWndButton	= AppendMenu( this, 0, APP_WEBBOX2	, GETTEXT( TID_TIP_QOODO	) );

	CWndBase::SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME,g_xFlyffConfig->GetMainLanguage(), _T( "WndTaskMenu.tga" ) ), TRUE );

	for(int i = 0; i < m_awndMenuItem.GetSize(); i++)
	{
		CWndButton* pWndButton = GetMenuItem( i );
		pWndButton->SetWndRect( CRect(  10, 60 + ( i * WndButton::ICONIMAGE_SIZE ), m_pTexture->m_size.cx - 20, 60 + WndButton::ICONIMAGE_SIZE + ( i * WndButton::ICONIMAGE_SIZE ) ) );
	}
	SetWndRect( CRect( 0, 0, m_pTexture->m_size.cx, m_pTexture->m_size.cy ) );

//	CRect rect = g_WndMng.m_pWndTaskBar->m_wndMenu.GetScreenRect();
//	Move( CPoint( 0, rect.top - GetWindowRect().Height() ) );

	m_pMenu1 = new CWndMenu;
	m_pMenu1->CreateMenu( this );
	pWndButton = AppendMenu( m_pMenu1, 0, APP_COMMUNICATION_CHAT , GETTEXT( TID_APP_COMMUNICATION_CHAT    ) );
	pWndButton = AppendMenu( m_pMenu1, 0, APP_MESSENGER_         , GETTEXT( TID_APP_MESSENGER ) );

	pWndButton = AppendMenu( m_pMenu1, 0, APP_COUPLE_MAIN        , GETTEXT( TID_GAME_COUPLE ) );	//sun: 13, Ŀ�� �ý���

	pWndButton->m_shortcut.m_dwShortcut = SHORTCUT_APPLET; SetTexture( pWndButton );
	pWndButton1->SetMenu( m_pMenu1 );

	m_pMenu2 = new CWndMenu;
	m_pMenu2->CreateMenu( this );
	pWndButton = AppendMenu( m_pMenu2, 0, APP_PARTY      , GETTEXT( TID_APP_PARTY          ) ) ;
	pWndButton = AppendMenu( m_pMenu2, 0, APP_GUILD      , GETTEXT( TID_APP_COMPANY         ) ) ;
	
#ifdef __GUILDVOTE
	pWndButton = AppendMenu( m_pMenu2, 0, APP_GUILD_VOTE      , GETTEXT( TID_GAME_TOOLTIP_GUILDVOTE     ) ) ;
#endif	
	pWndButton2->SetMenu( m_pMenu2 );

	m_pMenu5 = new CWndMenu;
	m_pMenu5->CreateMenu( this );
	switch(_GetContentState(CT_INFO_NOTICE))
	{
	case CS_VER1:
		pWndButton = AppendMenu( m_pMenu5, 0, APP_INFO_NOTICE   , GETTEXT( TID_APP_INFO_NOTICE   ) ); 
		break;
	case CS_VER2:
		pWndButton = AppendMenu( m_pMenu5, 0, APP_NOTICE   , GETTEXT( TID_APP_INFO_NOTICE   ) ); 
		break;
	}
	pWndButton5->SetMenu( m_pMenu5 );

	m_pMenu6 = new CWndMenu;
	m_pMenu6->CreateMenu( this );
	pWndButton = AppendMenu( m_pMenu6, 0, APP_HELPER_HELP , GETTEXT( TID_APP_HELPER_HELP ) );
	pWndButton = AppendMenu( m_pMenu6, 0, APP_HELPER_TIP  , GETTEXT( TID_APP_HELPER_TIP  ) );
	pWndButton = AppendMenu( m_pMenu6, 0, APP_HELPER_FAQ  , GETTEXT( TID_APP_HELPER_FAQ  ) );

	pWndButton = AppendMenu( m_pMenu6, 0, APP_INFOPANG , GETTEXT( TID_APP_INFOPANG ) );		//sun: 12, Ʃ�丮�� ����
	//pWndButton->SetTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, _T( "icon_Folder.dds" ) ) );

#ifdef __INTERNALSERVER
	pWndButton = AppendMenu( m_pMenu6, 0, APP_WEBBOX2 , GETTEXT( TID_GAME_HELPER_WEB_BOX_ICON_TITLE ) );
#else // __INTERNALSERVER
	if( ( g_xFlyffConfig->GetMainLanguage() == LANG_USA ) )
	{
		pWndButton = AppendMenu( m_pMenu6, 0, APP_WEBBOX2 , GETTEXT( TID_GAME_HELPER_WEB_BOX_ICON_TITLE ) );
	}
#endif // __INTERNALSERVER
	pWndButton6->SetMenu( m_pMenu6 );
	
	m_pMenu7 = new CWndMenu;
	m_pMenu7->CreateMenu( this );
	pWndButton = AppendMenu( m_pMenu7, 0, APP_WEBBOX, GETTEXT( TID_APP_ITEMMALL ) ); pWndButton->m_shortcut.m_dwShortcut = SHORTCUT_APPLET; SetTexture( pWndButton );
	pWndButton = AppendMenu( m_pMenu7, 0, APP_COMM_ITEM , GETTEXT( TID_APP_ITEMTIME ) ); pWndButton->m_shortcut.m_dwShortcut = SHORTCUT_APPLET; SetTexture( pWndButton );
	pWndButton7->SetMenu( m_pMenu7 );
}
CWndButton* CWndTaskMenu::AppendMenu( CWndMenu* pWndMenu, UINT nFlags, UINT nIDNewItem,	LPCTSTR lpszNewItem )
{
	CWndButton* pWndButton = (CWndButton*)pWndMenu->AppendMenu( nFlags, nIDNewItem, lpszNewItem );
	AppletFunc* pAppletFunc = g_WndMng.GetAppletFunc( nIDNewItem ); 
	if( pAppletFunc )
	{
		pWndButton->m_cHotkey = pAppletFunc->m_cHotkey;
		CString string;
		if( pAppletFunc->m_cHotkey == 0 )
			string.Format( "%s", pAppletFunc->m_pAppletDesc );
		else
			string.Format( "%s\n[%s %c]", pAppletFunc->m_pAppletDesc,  prj.GetText( TID_GAME_TOOLTIP_HOTKEY ), pAppletFunc->m_cHotkey );
		pWndButton->m_strToolTip = string;
		pWndButton->m_shortcut.m_dwShortcut = SHORTCUT_APPLET; 
		SetTexture( pWndButton );
	}
	
//	SetWndRect( CRect( 0, 0, 180, 50 + 5 + 5 + ( m_awndMenuItem.GetSize() * 22 ) ) );

	return pWndButton;
	/*

  CString string;
  if( pAppletFunc->m_cHotkey == 0 )
  string.Format( "���÷�\n%s", pAppletFunc->m_pAppletDesc );
  else
  string.Format( "���÷�\n%s\n[����Ű %c]", pAppletFunc->m_pAppletDesc, pAppletFunc->m_cHotkey );
  g_toolTip.PutToolTip( pShortcut->m_dwId , string, *pRect, point, 0 );
  
	CWndButton* pWndButton = new CWndButton;
	int nCount = m_awndMenuItem.GetSize();
	CSize size = m_pFont->GetTextExtent( lpszNewItem );
	if( size.cx > m_nLargeWidth )
		m_nLargeWidth = size.cx + 60;
	pWndButton->Create(lpszNewItem, WBS_MENUITEM | WBS_HIGHLIGHT, CRect( 2, 2 + ( nCount * 22 ), m_nLargeWidth, 2 + ( nCount * 22 ) + 20 ), this, nIDNewItem );
	m_awndMenuItem.Add( pWndButton );
	for( int i = 0; i < m_awndMenuItem.GetSize(); i++ )
	{
		pWndButton = (CWndButton*)m_awndMenuItem.GetAt( i );
		CRect rect = pWndButton->GetWindowRect( TRUE );
		rect.right = rect.left + m_nLargeWidth; 
		pWndButton->SetWndRect( rect );
	}
	SetWndRect( CRect( 0, 0, m_nLargeWidth + 10, 5 + 5 + ( ( nCount + 1 ) * 22 ) ) );
	return pWndButton;
	*/
}

BOOL CWndTaskMenu::Initialize(CWndBase* pWndParent,DWORD dwWndId)
{
	CreateMenu( &g_WndMng );
	SetVisible(TRUE);
	return TRUE;
}

void CWndTaskMenu::OnKillFocus(CWndBase* pNewWnd)
{
	if(g_WndMng.m_pWndTaskBar != pNewWnd)
		CWndMenu::OnKillFocus(pNewWnd);
}

BOOL CWndTaskMenu::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase )
{
	return TRUE;
}
BOOL CWndTaskMenu::OnChildNotify(UINT message,UINT nID,LRESULT* pLResult)
{
	if( message == WNM_CLICKED )
		g_WndMng.ObjectExecutor( SHORTCUT_APPLET, nID );
	return TRUE;
}
void CWndTaskMenu::OnSize(UINT nType, int cx, int cy)
{
	CWndMenu::OnSize( nType, cx, cy );
}
void CWndTaskMenu::OnLButtonUp(UINT nFlags, CPoint point)
{
	//if(IsWndStyle(WBS_CAPTION) && m_bPickup)
	{//
	//	m_wndTitleBar.m_wndMinimize.SetVisible(TRUE);
		//m_wndTitleBar.m_wndMaximize.SetVisible(TRUE);
	}
}
void CWndTaskMenu::OnLButtonDown(UINT nFlags, CPoint point)
{
//	CWndBase::OnLButtonDown(nFlags,point
	if(IsWndRoot())
		return;
//	return;

}

void CWndTaskMenu::PaintFrame( C2DRender* p2DRender )
{
	if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		CWndBase::PaintFrame( p2DRender );
	
	if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
	{
		if( m_pTexture )
			RenderWnd();
	}
}
BOOL CWndTaskMenu::OnEraseBkgnd( C2DRender* p2DRender )
{
	CRect rect = GetWindowRect();
//	rect.top += 50;
//	p2DRender->RenderFillRect(rect, D3DCOLOR_ARGB( 255, 255,255,255 ) );
	return CWndBase::OnEraseBkgnd( p2DRender );
}


void CWndTaskBar::RenderCollTime(CPoint pt, DWORD dwSkillId, C2DRender* p2DRender )
{
	LPSKILL lpSkill = &g_pPlayer->m_aJobSkill[ dwSkillId ];
	if( lpSkill )
	{
		DWORD dwDelay = g_pPlayer->GetReuseDelay( dwSkillId );
		if( dwDelay > 0 )
		{
//sun: 9, ���ο� AddSkillProp �÷� �߰� �� ����
			SkillProp* pSkillProp	= lpSkill->GetProp();
			FLASSERT( pSkillProp );
			AddSkillProp* pAddSkillProp	= prj.GetAddSkillProp( pSkillProp->dwSubDefine, lpSkill->dwLevel );
			FLASSERT( pAddSkillProp );
			RenderRadar( p2DRender, pt, 
			             pAddSkillProp->dwCooldown - dwDelay, 
						 pAddSkillProp->dwCooldown );	

		}
	}				
}

//sun: 12, ����
void CWndTaskBar::RenderLordCollTime( CPoint pt, DWORD dwSkillId, C2DRender* p2DRender )
{
	CCLord* pLord = CCLord::Instance();

	if( NULL == pLord )
		return ;

	CLordSkill* pLordSkill = pLord->GetSkills();

	if( NULL == pLordSkill )
		return ;

	CLordSkillComponentExecutable* pComponent = pLordSkill->GetSkill(dwSkillId);

	if( NULL == pComponent )
		return ;

	DWORD dwDelay = 0;
	if(pComponent->GetTick() > 0)
		RenderRadar( p2DRender, pt, pComponent->GetCooltime() - pComponent->GetTick(), pComponent->GetCooltime() );	
}


void CWndTaskBar::RenderOutLineLamp(int x, int y, int num, DWORD size)
{
	CPoint Point = CPoint( x, y );
	LONG   thick = 10;

	D3DXCOLOR dwColorDest2  = D3DCOLOR_ARGB( 0, 40, 0, 255 );

	D3DXCOLOR dwColor  = D3DCOLOR_ARGB( 196, 40, 0, 255 );

	D3DXCOLOR dwColor1 = D3DCOLOR_ARGB( 0, 255, 255, 255 );
	D3DXCOLOR dwColor2 = D3DCOLOR_ARGB( 0, 255, 255, 255 );

	static BOOL  bReverse = FALSE;
	static FLOAT fLerp = 0.0f;

	if( bReverse )
		fLerp-=0.05f;
	else
		fLerp+=0.05f;

	if( fLerp > 1.0f )
	{
		bReverse = TRUE;
		fLerp = 1.0f;
	}
	else
	if( fLerp < 0.0f )
	{
		bReverse = FALSE;
		fLerp = 0.0f;
	}
	
	D3DXColorLerp( &dwColor2, &dwColor, &dwColorDest2, fLerp);
	
	CRect Rect = CRect( 0, 0, (size * num), size );
	
	m_p2DRender->RenderFillRect( CRect( (Point.x+Rect.left)-thick, Point.y+Rect.top, (Point.x+Rect.left), Point.y+Rect.bottom ),
		dwColor1, dwColor2, dwColor1, dwColor2 );
	
	m_p2DRender->RenderFillRect( CRect( (Point.x+Rect.left), (Point.y+Rect.top)-thick, (Point.x+Rect.right), Point.y+Rect.top ),
		dwColor1, dwColor1, dwColor2, dwColor2 );
	
	m_p2DRender->RenderFillRect( CRect( (Point.x+Rect.right), Point.y+Rect.top, (Point.x+Rect.right)+thick, Point.y+Rect.bottom ),
		dwColor2, dwColor1, dwColor2, dwColor1 );
	
	m_p2DRender->RenderFillRect( CRect( (Point.x+Rect.left), (Point.y+Rect.bottom), (Point.x+Rect.right), (Point.y+Rect.bottom)+thick ),
		dwColor2, dwColor2, dwColor1, dwColor1 );
}

BOOL CWndTaskBar::IsShortcut( LPSHORTCUT lpShortcut, DWORD dwShortcut, DWORD dwId )
{
	if( lpShortcut->m_dwShortcut == SHORTCUT_APPLET )
	{
	}
	else
	if( lpShortcut->m_dwShortcut == SHORTCUT_SKILL )
	{
	}
	else
	if( lpShortcut->m_dwShortcut == SHORTCUT_ITEM )
	{
	}
	else
	if( lpShortcut->m_dwShortcut == SHORTCUT_CHAT )
	{
	}
	else
	if( lpShortcut->m_dwShortcut == SHORTCUT_MOTION )
	{
		if( lpShortcut->m_dwShortcut == dwShortcut )
		{
			MotionProp* pMotionProp = prj.GetMotionProp( dwId );
		
			if( pMotionProp )
			{
				return TRUE;
			}
		}
	}
	
	return FALSE;
}


//----------------------------------------------------------------------------------------------------------
//CWndActionSlot_HUD
//----------------------------------------------------------------------------------------------------------
CWndActionSlot_HUD::CWndActionSlot_HUD( )
{
	m_pWndMaster = NULL;
	m_eMode = MODE_ROTATE;
	m_nDegree = 0;
	m_nTargetDegree = 0;
	m_nOnSlot = -1;

	m_nEff2 = 255;
	m_nEff2_Slot = -1;

	m_nEff3 = 0;
	m_nEff5 = 0;
	m_nEff6 = 0;
	m_nEff7 = 0;
	m_bReadyFireEffect = FALSE;
	m_bRadySlotEffect = FALSE;
	m_fCoolTime = 0.0f;
	m_nCurrIndex = 0;
	m_bReadyToActive = FALSE;
	m_bRunning = FALSE;
	m_bUpdateSlot = FALSE;
	m_nCooldowning = 0;
	m_bInitialized = FALSE;
}

CWndActionSlot_HUD::~CWndActionSlot_HUD( )
{
	m_kTexCenter1.DeleteDeviceObjects( );
	m_kTexCenter2.DeleteDeviceObjects( );
	m_kTexAlpha.DeleteDeviceObjects( );
	m_kTexCircleSlot.DeleteDeviceObjects( );
	m_kTexDecoration1.DeleteDeviceObjects( );
	m_kTexRTarget_Skill.DeleteDeviceObjects( );
	m_kTexRTarget_Final.DeleteDeviceObjects( );
	m_kTexSlotEff1.DeleteDeviceObjects( );
	m_kTexSlotEff2.DeleteDeviceObjects( );
	m_kTexSlotEff3.DeleteDeviceObjects( );
	m_kTexSlotEff4.DeleteDeviceObjects( );
	m_kTexSlotEff5.DeleteDeviceObjects( );
}

// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndActionSlot_HUD::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_SLOT, WBS_MANAGER| WBS_TOPMOST, CPoint( 0, 0 ), pWndParent );
} 

void CWndActionSlot_HUD::OnInitialUpdate() 
{ 
	//CWndNeuz::OnInitialUpdate() ���� �����ؾ��� ( size�� ���� )
	LoadTexture( );

	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���

	m_pWndMaster = static_cast< CWndTaskBar_HUD* > ( m_pParentWnd );
} 

BOOL CWndActionSlot_HUD::Process( void )
{
	if( NULL ==  g_pPlayer )
		return FALSE;

	//-----------------------------------------------------------------------------------
	// gmpbigsun( 20120515 ) : ��Ÿ�� ��ų üũ
	// ��ų��ü���� ��Ÿ���� ���ҽ�Ű�� ����� �ƴϱ⶧���� �ܺο��� �������� �ؾ���.
	// �����¿����� Update
	if( FALSE == m_pWndMaster->m_bLockActionSlot )
	{
		int nCooldown = 0;
		for( int i = 0; i < HUD_ACTIONSLOT_MAX; ++i )
		{
			LPSHORTCUT lpShortcut = &m_pWndMaster->m_aSlotQueue[i];
			if( NULL == lpShortcut )
				continue;

			DWORD dwDelay = g_pPlayer->GetReuseDelay( lpShortcut->m_dwId );
			if( dwDelay > 0 )
				++nCooldown;
		}

		//�������� ������Ʈ�� ��Ÿ�� ������ ���� Ʋ�����°�� Update!
		if( m_nCooldowning != nCooldown )
			UpdateSlot();	
	}
	//-----------------------------------------------------------------------------------

	if( m_bUpdateSlot )
	{
		++m_nCurrIndex;
		UpdateSlot( );

		m_bUpdateSlot = FALSE;
	}

	CPoint ptMouse = GetMousePoint();
	ClientToScreen( &ptMouse );

	//���콺�� this���� �ִ���...
	if( this != IsOnWndBase2( ptMouse ) )
		m_nOnSlot = -1;
	else
		m_nOnSlot = GetSlotIndex( GetMousePoint() );

	if( m_fCoolTime > 0.0f )
	{
		m_fCoolTime -= _GetDeltaTimeSec( );
		if( m_fCoolTime <= 0.0f )
		{
			//ok wait a packet
			m_fCoolTime = 0.0f;
		}
	}

	//��ü ȿ�� Ÿ�� ó�� 
	if( m_pWndMaster->m_nExecute == 0 )
	{
		if( m_bRunning )
		{
			if( m_nTargetDegree == m_nDegree )
			{
				//Rewind���� ��ģ���� ( ���� ���� )
				End();

				//�����κ��� �̹� ��밡�ɻ��� ��Ŷ�� �°�� ȿ�������� �ٷ� ��밡�ɻ��·�.
				if( m_bReadyToActive )
					ReadyToUse( );

			}
		}
	}

	return TRUE;
}

void CWndActionSlot_HUD::OnDestroyChildWnd( CWndBase* pWndChild )
{

}

BOOL CWndActionSlot_HUD::OnDropIcon( LPSHORTCUT pShortcut, CPoint point )
{
	if( m_pWndMaster->IsLockedActionSlot() == TRUE ) {
		return FALSE;
	}

	if( SHORTCUT_SKILLFUN == pShortcut->m_dwShortcut )
		return FALSE;

	int nSlot = GetSlotIndex( point );

	// ������ ������ ��� 
	if( pShortcut->m_pFromWnd == this )
	{
		LPSHORTCUT lpShortcut = (LPSHORTCUT) pShortcut->m_dwData;
		// ����Ʈ�� �׽�ũ�ٰ� �ƴ� ���̶��. �ǵ������� ������ ������� �½�ũ�� �ۿ� ���� ����. ������.
		if( -1 == nSlot )
		{
			// ������ ���� ������ ����
			if( lpShortcut->m_dwData == 0 )
				g_DPlay.SendRemoveAppletTaskBar( (BYTE)( lpShortcut->m_dwIndex ) );
			else
			if( lpShortcut->m_dwData == 1 )
				g_DPlay.SendRemoveItemTaskBar( m_pWndMaster->m_nSlotIndex, (BYTE)( lpShortcut->m_dwIndex ) );
			else
			if( lpShortcut->m_dwData == 2 )
			{
				RemoveSkillQueue( lpShortcut->m_dwIndex );
				m_nEff2_Slot = nSlot;
				PLAYSND( SND_PC_ACTIONREMOVE );
				return TRUE;
				//g_DPlay.SendSkillTaskBar( );
			}
			lpShortcut->Empty();
			return TRUE;
		}
	//	return TRUE;
	}

	if( pShortcut->m_dwShortcut == SHORTCUT_SKILL && pShortcut->m_dwType == 0 )		// ��ų�ϰ�츸 ���
	{
		// ���ο��� �̵� ; ������ �Ʒ� �κп��� ������ ó���ϳ�, ��ų ť������ �̵��� ������ �ٷ� �߰��ؾ��ϱ� ������ ���⼭ ������ �Ѵ�.
		if( pShortcut->m_pFromWnd == this )
		{
			LPSHORTCUT lpShortcut = (LPSHORTCUT) pShortcut->m_dwData;
			if( lpShortcut->m_dwData == 2 )
			{
				RemoveSkillQueue( lpShortcut->m_dwIndex, FALSE );
				SetSkillQueue( nSlot, pShortcut->m_dwType, pShortcut->m_dwId, pShortcut->m_pTexture );// == FALSE )

				//slot for effect
				m_nEff2_Slot = nSlot;
				if( m_pWndMaster->m_nCurQueueNum > 0 && m_nEff2_Slot >= m_pWndMaster->m_nCurQueueNum )
					m_nEff2_Slot = ( m_pWndMaster->m_nCurQueueNum - 1 ); 

				PLAYSND( SND_PC_ACTIONREGISTER );
				
				return TRUE;
			}
		}

		//���԰�谡 �ƴѰ�� �� �������� �ø��Ŷ� �Ǵ��Ѵ�.
		if( -1 == nSlot )
			nSlot = m_pWndMaster->m_nCurQueueNum;
			
		BOOL bOK = SetSkillQueue( nSlot, pShortcut->m_dwType, pShortcut->m_dwId, pShortcut->m_pTexture );

		if( bOK )
		{
			//slot for effect
			m_nEff2_Slot = nSlot;
			if( m_pWndMaster->m_nCurQueueNum > 0 && m_nEff2_Slot >= m_pWndMaster->m_nCurQueueNum )
				m_nEff2_Slot = ( m_pWndMaster->m_nCurQueueNum - 1 ); 
	//		SetForbid( TRUE );

	//		bForbid = FALSE;

			PLAYSND( SND_PC_ACTIONREGISTER );
		}
	}
	
	return TRUE;
}

void CWndActionSlot_HUD::OnLButtonDown(UINT nFlags, CPoint point)
{
	if( m_pWndMaster->IsLockedActionSlot() == TRUE ) {
		return; 
	}

	LPWNDCTRL pCtrl = GetWndCtrl(  WIDC_CUSTOM1 );
	if( pCtrl )
	{
		if( pCtrl->rect.PtInRect( point ) )
		{
			CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
			if( NULL == pWndWorld )
				return;

			m_pWndMaster->m_aSlotSkill.m_dwShortcut = SHORTCUT_SKILLFUN;
			m_pWndMaster->m_pSelectShortcut = &m_pWndMaster->m_aSlotSkill;

			SetCapture();

			
		//	pWndWorld->SetNextActionSkill( );
			return;
		}
	}

	int nSlot  =  GetSlotIndex( point );
	if( -1 != nSlot )
	{
		m_pWndMaster->m_pSelectShortcut = &m_pWndMaster->m_aSlotQueue[ nSlot ];
		SetCapture();
	}
}

void CWndActionSlot_HUD::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	LPWNDCTRL pCtrl = GetWndCtrl(  WIDC_CUSTOM1 );
	if( pCtrl )
	{
		if( pCtrl->rect.PtInRect( point ) )
		{
			CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
			if( NULL == pWndWorld )
				return;

			pWndWorld->SetNextActionSkill( );
			return;
		}
	}

	int nSlot  =  GetSlotIndex( point );
	if( -1 == nSlot )
		return;

	LPSHORTCUT pShortcut = &m_pWndMaster->m_aSlotQueue[ nSlot ];
	if( pShortcut == m_pWndMaster->m_pSelectShortcut && !pShortcut->IsEmpty() && pShortcut->m_dwData != 2 )
	{
		g_WndMng.ObjectExecutor( pShortcut );
	}
}

void CWndActionSlot_HUD::OnMouseMove(UINT nFlags, CPoint point)
{
	if( m_pWndMaster->IsLockedActionSlot() == TRUE ) {
		return;
	}

	if( m_bLButtonDown && IsPush() )
	{
		LPSHORTCUT pSelectShortcut = m_pWndMaster->m_pSelectShortcut;
	
		if( pSelectShortcut && !pSelectShortcut->IsEmpty() )
		{
#ifdef __MAINSERVER
			if( pSelectShortcut->m_dwId == 400 )
				return;
#endif //__MAINSERVER
//sun: 12, ����
			if(pSelectShortcut->m_dwShortcut == SHORTCUT_LORDSKILL)
			{
				pSelectShortcut->m_dwId++;
			}
			memcpy( &m_GlobalShortcut, pSelectShortcut, sizeof( m_GlobalShortcut ) );
			m_GlobalShortcut.m_pFromWnd = this;
			m_GlobalShortcut.m_dwData = (DWORD)pSelectShortcut;
			pSelectShortcut = NULL;
		}
	}

	CWndNeuz::OnMouseMove( nFlags, point );
}

void CWndActionSlot_HUD::OnMouseWndSurface( CPoint point )
{
	LPWNDCTRL pWndSlot = NULL;
	CRect rect;
	
	LPWNDCTRL pCtrl = GetWndCtrl(  WIDC_CUSTOM1 );
	if( pCtrl )
	{
		if( pCtrl->rect.PtInRect( point ) )
		{
			SHORTCUT Shortcut;
			Shortcut.m_dwShortcut = SHORTCUT_SKILLFUN;

			rect = pCtrl->rect;
			ClientToScreen( &point );
			ClientToScreen( &rect );
			FLStrcpy( Shortcut.m_szString, _countof( Shortcut.m_szString ), prj.GetText(TID_GAME_SKILLSHORTCUT) );
			g_toolTip.PutToolTip( 10000, Shortcut.m_szString, rect, point, 0 );
		}
	}

	int nSlot = GetSlotIndex( point );
	if( -1 != nSlot && nSlot < HUD_ACTIONSLOT_MAX )
	{
		LPSHORTCUT lpShortcut = &m_pWndMaster->m_aSlotQueue[ nSlot ];
		if( NULL == lpShortcut )
			return;

		rect.left = m_kSlotPoint[ nSlot ].x;
		rect.top = m_kSlotPoint[ nSlot ].y;
		rect.right = rect.left + SLOT_SIZE;
		rect.bottom = rect.top + SLOT_SIZE;

		if( FALSE == lpShortcut->IsEmpty() && rect.PtInRect( point)  )
		{
			if( FALSE == m_pWndMaster->IsShortcut( lpShortcut, SHORTCUT_MOTION, MOT_BASE_CHEER ) )
			{
				LPSKILL lpSkill = NULL;
				if( g_pPlayer )
					lpSkill = g_pPlayer->GetSkill( 0, lpShortcut->m_dwId );

				ClientToScreen( &point );
				ClientToScreen( &rect );

				if( lpSkill )
					g_WndMng.PutToolTip_Skill( lpSkill->dwSkill, lpSkill->dwLevel, point, &rect);
				return;
			}
		}
	}
}

HRESULT CWndActionSlot_HUD::RestoreDeviceObjects()
{
	CWndBase::RestoreDeviceObjects();

	BOOL bOK  = m_kTexRTarget_Skill.CreateTexture( m_pApp->m_pd3dDevice, m_kSizeCircle.cx, m_kSizeCircle.cy, 1, 
			D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT );

	FLASSERT( bOK == TRUE );

	bOK  = m_kTexRTarget_Final.CreateTexture( m_pApp->m_pd3dDevice, m_kSizeCircle.cx, m_kSizeCircle.cy,	1, 
			D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT );

	FLASSERT( bOK == TRUE );

	if( m_bInitialized )
		MakeCircleSlot();
	
	return S_OK;
}
HRESULT CWndActionSlot_HUD::InvalidateDeviceObjects()
{
	CWndBase::InvalidateDeviceObjects();
   	
	m_kTexRTarget_Skill.DeleteDeviceObjects();
	m_kTexRTarget_Final.DeleteDeviceObjects();

	return S_OK;
}
HRESULT CWndActionSlot_HUD::DeleteDeviceObjects()
{
	CWndBase::DeleteDeviceObjects();

	m_kTexRTarget_Skill.DeleteDeviceObjects();
	m_kTexRTarget_Final.DeleteDeviceObjects();

	return S_OK;
}


BOOL CWndActionSlot_HUD::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase )
{
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
}

BOOL CWndActionSlot_HUD::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

void CWndActionSlot_HUD::OnDraw( C2DRender* p2DRender ) 
{
	if( NULL == m_kTexRTarget_Final.m_pTexture )
		return;

	switch( m_eMode )
	{
	case MODE_NONE:
		m_kTexRTarget_Final.Render( p2DRender, CPoint( 0, 0 ) );
		break;

	case MODE_START:
	case MODE_START_PASSED:
	case MODE_ROTATE: 
		DrawRotate( p2DRender );
		break;

	default:
		break;
	}

	//frame ��� ������ ��ġ��!
	float fFrameUP = ( g_Neuz.m_fFPS / 60.0f  );
	if( fFrameUP > 1.0f )
		fFrameUP = 1.0f;
	else fFrameUP = 60.0f / g_Neuz.m_fFPS;

	int nSpeedFactor = 0;

	LPWNDCTRL pCtrl = GetWndCtrl( WIDC_CUSTOM1 );
	if( NULL == pCtrl )
		return;

	if( m_fCoolTime > 0.0f )
	{
		p2DRender->RenderTexture( pCtrl->rect.TopLeft(), &m_kTexCenter1, 60 );

		float fRate = (m_fCoolTime / (float)(ACTIONSLOT_DEFAULT_COOLTIME) ) - 0.1f;
		if( fRate < 0.0f )
			fRate = 0.0f;
		else if( fRate > 1.0f )
			fRate = 1.0f;

		p2DRender->RenderTextureUV( pCtrl->rect.TopLeft(), m_kTexCenter2.m_sizePitch, &m_kTexCenter2, 0.0f, fRate, 1.0f, 1.0f );
	}
	else 
	{
		if( m_pWndMaster->m_nExecute == 0 )
			p2DRender->RenderTexture( pCtrl->rect.TopLeft(), &m_kTexCenter2 );
		else
			p2DRender->RenderTexture( pCtrl->rect.TopLeft(), &m_kTexCenter1 );
	}

	//���콺 �»��� ǥ��
	static BOOL bToggle2 = TRUE;
	static int nAlpha2 = 255;
	if( m_pWndMaster->m_nExecute == 0 && -1 != m_nOnSlot && m_nEff2_Slot != m_nOnSlot )
	{
		if( bToggle2 )
		{
			nSpeedFactor = int( -8.0f * fFrameUP );
			nAlpha2 += nSpeedFactor;
			if( nAlpha2 <= 4 )
			{
				nAlpha2 = 0;
				bToggle2 = FALSE;
				PLAYSND( SND_PC_ACTIONSLOTMOVE02 );
			}
		}
		else
		{
			nSpeedFactor = int( 8.0f * fFrameUP );
			nAlpha2 += nSpeedFactor;
			if( nAlpha2 >= 250 )
			{
				bToggle2 = TRUE;
				nAlpha2 = 255;
				PLAYSND( SND_PC_ACTIONSLOTMOVE01 );
			}
		}

		CPoint kSlotPt = m_kSlotPoint[ m_nOnSlot ];
		kSlotPt -= CPoint ( 9, 9 );
	//	p2DRender->RenderTextureWithEffect1( kSlotPt, &m_kTexSlotEff1, nAlpha2 );
		p2DRender->RenderTexture( kSlotPt, &m_kTexSlotEff1, nAlpha2 );
	}

	//������ ���� ǥ��
	if( m_nEff2_Slot != -1 )
	{
		nSpeedFactor = int( -8.0f * fFrameUP );

		m_nEff2 += nSpeedFactor;
		if( m_nEff2 <= 0 )
			m_nEff2 = 0;

		CPoint kSlotPt = m_kSlotPoint[ m_nEff2_Slot ];
		kSlotPt -= CPoint ( 9, 9 );
		p2DRender->RenderTextureWithEffect1( kSlotPt, &m_kTexSlotEff2, m_nEff2 );

		//ǥ�� ��.
		if( m_nEff2 <= 0 )
		{
			m_nEff2_Slot = -1;
			m_nEff2 = 255;
		}
	}

	//�ߵ� ǥ��
	if( m_pWndMaster->m_nExecute == 2 )
	{
		if( m_nDegree == m_nTargetDegree )
		{
			//�ߵ��� �������� �ߵ���ġ�� ������ 1ȸ effect
			if( m_bReadyFireEffect )
			{
				m_nEff2_Slot = 0;
				m_nEff2 = 255;
				m_bReadyFireEffect = FALSE;
				m_bUpdateSlot = TRUE;

				PLAYSND( SND_PC_ACTIONSTART );
			}

			//ù��ų ������ �н���� No effect
			if( MODE_START_PASSED != m_eMode )
			{
				nSpeedFactor = int ( 12.0f * fFrameUP );

				m_nEff3 += nSpeedFactor;
				if( m_nEff3 > 360 )
					m_nEff3 -= 360;

				CPoint kPt = m_kSlotPoint[ 0 ];
				kPt -= CPoint( 9, 9 );
				m_kTexSlotEff3.RenderRotate( p2DRender, kPt, D3DXToRadian( m_nEff3 ), TRUE );
			}
		
		}
		else
		{
			m_nEff3 = 0;
		}

		nSpeedFactor = int ( 12.0f * fFrameUP );
		m_nEff5 += nSpeedFactor;
		if( m_nEff5 > 360 )
			m_nEff5 -= 360;
		m_kTexSlotEff5.RenderRotate( p2DRender, CPoint( 0, 0 ), D3DXToRadian( m_nEff5 ), TRUE );
	}

	OnDraw_Effect( p2DRender, fFrameUP );

	if( m_kTexDecoration1.m_pTexture )
	{
		p2DRender->RenderTexture( CPoint( 0, 0 ), &m_kTexDecoration1 );
	}
}

int CWndActionSlot_HUD::GetSlotIndex( CPoint kPt )
{
	CRect rect;

	for( int i = 0; i < HUD_ACTIONSLOT_MAX; ++i )
	{
		rect.left = m_kSlotPoint[ i ].x;
		rect.top = m_kSlotPoint[ i ].y;
		rect.right = rect.left + SLOT_SIZE;
		rect.bottom = rect.top + SLOT_SIZE;

		if( rect.PtInRect( kPt ) )
			return i;
	}

	return -1;
}

void CWndActionSlot_HUD::CustomInit( )
{
	m_nCurrIndex = 0;
	MakeCircleSlot( );

	m_bInitialized = TRUE;
}

void CWndActionSlot_HUD::Start( BOOL bSkillPassed )
{
	//ù ��ų�� pass���� ������츸 effect����
	m_bReadyFireEffect = !bSkillPassed;

	m_pWndMaster->LockActionSlot( TRUE );
	m_fCoolTime = (float)( ACTIONSLOT_DEFAULT_COOLTIME );

	m_bRunning = TRUE;

	m_nCurrIndex = 0;

	PLAYSND( SND_PC_ACTIONOULINE02 );

	if( bSkillPassed )
		m_eMode = MODE_START_PASSED;
	else m_eMode = MODE_START;
}

void CWndActionSlot_HUD::End( )
{
	m_bRunning = FALSE;
	m_nCurrIndex = 0;
}

void CWndActionSlot_HUD::ReadyToUse( )
{
	m_fCoolTime = 0.0f;
	m_pWndMaster->LockActionSlot( FALSE );
	m_nCurrIndex = 0;
	UpdateSlot();

	m_bRadySlotEffect = TRUE;
	m_bReadyToActive = FALSE;

	PLAYSND( SND_PC_ACTIONOULINE01 );
}

void CWndActionSlot_HUD::Rotate( int nDegree )
{
	if( nDegree > 0 )
		m_bReadyFireEffect = TRUE;
	
	m_nTargetDegree += nDegree;

	if( m_nTargetDegree < 0 )
		m_nTargetDegree = 0;

	m_eMode = MODE_ROTATE;
}

void CWndActionSlot_HUD::Rewind( )
{
	if( 0 != m_nTargetDegree )
		Rotate( -m_nTargetDegree );

	m_nCurrIndex = 0;
}

void CWndActionSlot_HUD::DrawRotate( C2DRender* p2DRender )
{
	if( NULL == m_kTexRTarget_Final.m_pTexture )
		return;

	//FPS �� ����Ѱ�( �ִ� 4degree )
	float fFrameUP = ( g_Neuz.m_fFPS / 60.0f  );
	if( fFrameUP > 1.0f )
		fFrameUP = 1.0f;
	else fFrameUP = 60.0f / g_Neuz.m_fFPS;

	if( m_nDegree != m_nTargetDegree )
	{

		//FPS �� ����Ѱ�( �ִ� 4degree )
		int nSpeedFactor =  m_nDegree > m_nTargetDegree ? -8 : 4 ;

		m_nDegree +=  (int) ( fFrameUP * (float)nSpeedFactor );

		if( m_nTargetDegree - 10 < m_nDegree && m_nDegree < m_nTargetDegree + 10 )
			m_nDegree = m_nTargetDegree;
	}

	m_kTexRTarget_Final.RenderRotate( p2DRender, CPoint( 0, 0 ), D3DXToRadian( m_nDegree ), TRUE );
		
	if( m_pWndMaster->m_nExecute != 0 )
	{
		static int siAlpha = 255;
		static BOOL sbToggle = TRUE;

		int nSpeedFactor =  int( fFrameUP * 8.0f );

		if( sbToggle )
		{
			siAlpha -= nSpeedFactor;
			if( siAlpha < 50 )
			{
				siAlpha = 50;
				sbToggle = FALSE;
			}
		}
		else
		{
			siAlpha += nSpeedFactor;
			if( siAlpha > 230 )
			{
				siAlpha = 255;
				sbToggle = TRUE;
			}
		}
		
		m_kTexSlotEff4.RenderRotate( p2DRender, CPoint( 0, 0 ), D3DXToRadian( m_nDegree ), TRUE, siAlpha );
	}
}

void CWndActionSlot_HUD::RemoveSkillQueue( int nIndex, BOOL bSend )
{
	m_pWndMaster->RemoveSkillQueue( nIndex, bSend );
}

BOOL CWndActionSlot_HUD::SetSkillQueue( int nIndex, DWORD dwType, DWORD dwId, CTexture* pTexture )
{
	return m_pWndMaster->SetSkillQueue( nIndex, dwType, dwId, pTexture );
}

void CWndActionSlot_HUD::SyncActionSlotReady( )
{
	//����ȭ�� ���� ȣ��Ǿ�� �Ѵ�.
	if( FALSE == m_bRunning )
		ReadyToUse( );
	else 
		m_bReadyToActive = TRUE;
}

void CWndActionSlot_HUD::OnDraw_Effect( C2DRender* p2DRender, float fFrameUp )
{
	if( m_bRadySlotEffect )
	{
		m_nEff6 += (int)( fFrameUp * 8.0f );
		m_kTexSlotEff5.RenderRotate( p2DRender, CPoint( 0, 0 ), D3DXToRadian( m_nEff6 ), TRUE );

		m_nEff7 -= (int)( fFrameUp * 8.0f );
		m_kTexSlotEff5.RenderRotate( p2DRender, CPoint( 0, 0 ), D3DXToRadian( m_nEff7 ), TRUE );

		if( m_nEff6 > 360 )
		{
			m_bRadySlotEffect = FALSE;
			m_nEff6 = 0;
			m_nEff7 = 0;
		}
	}
}

void CWndActionSlot_HUD::UpdateSlot( ) 
{
	if( NULL == g_pPlayer )
		return;

	// ���� ��Ÿ�� �ؾ��� ��ų������ �����Ѵ�. �̰����� Update�ؾ��� ��Ȳ�� �Ǻ��ϱ� ����...
	m_nCooldowning = 0;
	for( int i = 0; i < HUD_ACTIONSLOT_MAX; ++i )
	{
		LPSHORTCUT lpShortcut = &m_pWndMaster->m_aSlotQueue[i];
		if( NULL == lpShortcut )
			continue;

		DWORD dwDelay = g_pPlayer->GetReuseDelay( lpShortcut->m_dwId );
		if( dwDelay > 0 )
			++m_nCooldowning;
	}

	MakeCircleSlot( );
}

void CWndActionSlot_HUD::LoadTexture( )
{
	m_kSizeCircle.cx = 166;
	m_kSizeCircle.cy = 166;

	//test
	m_kSlotPoint[3] = CPoint( 60, 10 );
	m_kSlotPoint[2] = CPoint( 105, 35 );
	m_kSlotPoint[1] = CPoint( 105, 85 );
	m_kSlotPoint[0] = CPoint( 60, 110 );
	m_kSlotPoint[5] = CPoint( 15, 85 );
	m_kSlotPoint[4] = CPoint( 15, 35 );

	LPDIRECT3DDEVICE9 pd3dDevice = g_Neuz.m_pd3dDevice;
	m_kTexCenter1.LoadTexture( pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), "slot02.tga" ), 0xffff00ff, TRUE ); 
	m_kTexCenter2.LoadTexture( pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), "slot03.tga" ), 0xffff00ff, TRUE ); 
	m_kTexAlpha.LoadTexture( pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), "slot07.tga" ), 0xffff00ff, TRUE ); 
	m_kTexCircleSlot.LoadTexture( pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage() , "slot04.tga" ), 0xffff00ff, TRUE ); 
	m_kTexDecoration1.LoadTexture( pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage() , "slot05.tga" ), 0xffff00ff, TRUE ); 
	m_kTexSlotEff1.LoadTexture( pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage() , "ActionSlotEff.tga" ), 0xffff00ff, TRUE ); 
	m_kTexSlotEff2.LoadTexture( pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage() , "ActionSlotEff2.tga" ), 0xffff00ff, TRUE ); 
	m_kTexSlotEff3.LoadTexture( pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage() , "ActionSlotEff3_02.tga" ), 0xffff00ff, TRUE ); 
	m_kTexSlotEff4.LoadTexture( pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage() , "ActionSlotEff4.tga" ), 0xffff00ff, TRUE ); 
	m_kTexSlotEff5.LoadTexture( pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage() , "ActionSlotEff5.tga" ), 0xffff00ff, TRUE ); 
}

BOOL CWndActionSlot_HUD::MakeCircleSlot( )
{
	// ��ų�������� �����ؽ��Ŀ� �ռ��Ѱ��� �׸���
	// �װ��� �ٽ� ���ư��� �����ؽ��Ŀ� �ռ��ؼ� �׷����´�.

	//1. render skill icon and alpha texture to rendertarget

	C2DRender* p2DRender = &g_Neuz.m_2DRender;
	if( !p2DRender )
		return FALSE;

	LPDIRECT3DDEVICE9 pd3dDevice = g_Neuz.m_pd3dDevice;

	pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );

	//Fog���� �̻����� �߻� ��..
    pd3dDevice->SetRenderState( D3DRS_FOGENABLE,        FALSE );
	
	D3DVIEWPORT9 viewportOld;
	pd3dDevice->GetViewport(&viewportOld);

	D3DVIEWPORT9 viewport;
	viewport.X      = 0;
	viewport.Y      = 0;
	viewport.Width  = m_kSizeCircle.cx;
	viewport.Height = m_kSizeCircle.cy;
	viewport.MinZ   = 0.0f;
	viewport.MaxZ   = 1.0f;
	pd3dDevice->SetViewport(&viewport);

	IDirect3DSurface9* pOldSurface = NULL;
	IDirect3DSurface9* pDstSurface = NULL;

	//����Ÿ���� �ؽ��Ŀ� ����� ������ surface�� �ٲ۴�.
	pd3dDevice->GetRenderTarget( 0, &pOldSurface );
	m_kTexRTarget_Skill.m_pTexture->GetSurfaceLevel( 0, &pDstSurface );
	pd3dDevice->SetRenderTarget( 0, pDstSurface );//, pOldSurfaceZ );

	pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET , 0x00ffffff/*CWorld::m_dwBgColor*/, 1.0f, 0);

	DWORD dwDelay = 0;
	DWORD dwColor = COLOR_ACTIVATION;
	CTexture* pTexIcon = NULL;
	if( pd3dDevice->BeginScene() == S_OK )
	{
		//��ų ������ �׸���.
		int nPassed = 0;
		for( int i = 0; i < HUD_ACTIONSLOT_MAX; ++i )
		{
			LPSHORTCUT lpShortcut = &m_pWndMaster->m_aSlotQueue[i];
			if( NULL == lpShortcut )
				continue;

			DWORD dwDelay = 0;
			if( g_pPlayer )
				dwDelay = g_pPlayer->GetReuseDelay( lpShortcut->m_dwId );

			if( dwDelay > 0 )
				dwColor = COLOR_COOLDOWN;
			else
				dwColor = ( i < m_nCurrIndex ? COLOR_COOLDOWN : COLOR_ACTIVATION );

			if( FALSE == lpShortcut->IsEmpty() && lpShortcut->m_pTexture )
				p2DRender->RenderTextureWithAlpha( m_kSlotPoint[i], lpShortcut->m_pTexture, &m_kTexAlpha, CSize( SLOT_SIZE, SLOT_SIZE ), dwColor );
		}
		pd3dDevice->EndScene();
	}

	pDstSurface->Release();

	m_kTexRTarget_Final.m_pTexture->GetSurfaceLevel( 0, &pDstSurface );
	pd3dDevice->SetRenderTarget( 0, pDstSurface );//, pOldSurfaceZ );

	//2. compose _skill and circle
	pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET , 0x00ffffff/*CWorld::m_dwBgColor*/, 1.0f, 0);

	if( pd3dDevice->BeginScene() == S_OK )
	{
		p2DRender->RenderTexture( CPoint( 0, 0 ), &m_kTexCircleSlot );
		p2DRender->RenderTexture( CPoint( 0, 0 ), &m_kTexRTarget_Skill );

		pd3dDevice->EndScene();
	}

	pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
//	pd3dDevice->SetRenderState( D3DRS_FOGENABLE, TRUE );

	pd3dDevice->SetRenderTarget( 0, pOldSurface );//, pOldSurfaceZ );
	pOldSurface->Release();
	pDstSurface->Release();
	pd3dDevice->SetViewport(&viewportOld);

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------
//CWndTaskBar_HUD
//----------------------------------------------------------------------------------------------------------
CWndTaskBar_HUD::CWndTaskBar_HUD( )
{
	m_nExtendPage = 0;
	m_bLock = TRUE;
	m_pGaugeHP = NULL;
	m_pGaugeMP = NULL;
	m_pGaugeFP = NULL;
	m_pGaugeEXP = NULL;

	m_pWndActionSlot = NULL;
	m_pWndEX1 = NULL;
	m_pWndEX2[0] = NULL;
	m_pWndEX2[1] = NULL;
	m_pWndEX3 = NULL;
}

CWndTaskBar_HUD::~CWndTaskBar_HUD( )
{
	m_texGau_HP.DeleteDeviceObjects( );
	m_texGau_MP.DeleteDeviceObjects( );
	m_texGau_FP.DeleteDeviceObjects( );
	m_texGau_EXP.DeleteDeviceObjects( );
	m_texGau_EXP_Deco.DeleteDeviceObjects( );

	m_texOnSlot.DeleteDeviceObjects( );
	m_texUsingSlot.DeleteDeviceObjects( );

	SAFE_DELETE( m_pWndActionSlot );
	SAFE_DELETE( m_pWndEX1 );
	SAFE_DELETE( m_pWndEX2[0] );
	SAFE_DELETE( m_pWndEX2[1] );
	SAFE_DELETE( m_pWndEX3 );
}

// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndTaskBar_HUD::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_NEWTASKBAR, WBS_MANAGER| WBS_TOPMOST , CPoint( 0, 0 ), pWndParent );
} 

void CWndTaskBar_HUD::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���

	m_texGau_HP.LoadTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(),"Targetgauge01.tga" ), 0xffff00ff, TRUE );
	m_texGau_MP.LoadTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(),"Targetgauge02.tga" ), 0xffff00ff, TRUE );
	m_texGau_FP.LoadTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(),"Targetgauge03.tga" ), 0xffff00ff, TRUE );
	m_texGau_EXP.LoadTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(),"WndMainBargauge.tga" ), 0xffff00ff, TRUE );
	m_texGau_EXP_Deco.LoadTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(),"WndMainBar.tga" ), 0xffff00ff, TRUE );
	m_texOnSlot.LoadTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(),"SlotSelect.tga" ), 0xffff00ff, TRUE );
	m_texUsingSlot.LoadTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(),"UseSlot.tga" ), 0xffff00ff, TRUE );

	m_texPack.LoadScript( m_pApp->m_pd3dDevice, MakePath( DIR_THEME,g_xFlyffConfig->GetMainLanguage(), "hotkey.inc" ) );

	m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice,  MakePath( DIR_ICON, "icon_ActionSkill.dds" ), 0xffff00ff );
	m_aSlotSkill.m_dwShortcut = SHORTCUT_SKILLFUN;
	m_aSlotSkill.m_pTexture   = m_pTexture;
} 

void CWndTaskBar_HUD::InitTaskBar()
{
	CWndTaskBar::InitTaskBar( );

	//create ActionSlot
	if( NULL == m_pWndActionSlot )
	{
		SAFE_DELETE( m_pWndActionSlot );
		m_pWndActionSlot = new CWndActionSlot_HUD;
		m_pWndActionSlot->Initialize( this );
	}

	//create TaskBarEx2
	if( NULL == m_pWndEX2[ 0 ] )
	{
		SAFE_DELETE( m_pWndEX2[ 0 ] );
		m_pWndEX2[0] = new CWndTaskBar_HUD_EX2;
		m_pWndEX2[0]->Initialize( this );
	}
	m_pWndEX2[0]->SetHotKeyPrefix( TRUE, "A" );
	m_pWndEX2[0]->SetVisible( m_kSavedInfo[0]._bVisible );
	m_pWndEX2[0]->Move( m_kSavedInfo[0]._kPt );

	if( NULL == m_pWndEX2[ 1 ] )
	{
		SAFE_DELETE( m_pWndEX2[ 1 ] );
		m_pWndEX2[1] = new CWndTaskBar_HUD_EX2;
		m_pWndEX2[1]->Initialize( this );
	}
	m_pWndEX2[1]->SetHotKeyPrefix( TRUE, "F" );
	m_pWndEX2[1]->SetVisible( m_kSavedInfo[1]._bVisible );
	m_pWndEX2[1]->Move( m_kSavedInfo[1]._kPt );

	if( NULL == m_pWndEX3 )
	{
		SAFE_DELETE( m_pWndEX3 );
		m_pWndEX3 = new CWndTaskBar_HUD_EX3;
		m_pWndEX3->Initialize( this );
	}
	m_pWndEX3->SetPage( PAGE_3 );
	m_pWndEX3->SetVisible( m_kSavedInfo[2]._bVisible );
	m_pWndEX3->Move( m_kSavedInfo[2]._kPt );

	//create TaskBarEx1
	if( NULL == m_pWndEX1 )
	{
		SAFE_DELETE( m_pWndEX1 );
		m_pWndEX1 = new CWndTaskBar_HUD_EX1;
		m_pWndEX1->Initialize( this );
	}
	m_pWndEX1->CustomInit( PAGE_4 );
	m_pWndEX1->SetVisible( TRUE );

	//all page set
	SetPage( PAGE_0 );
	m_pWndEX2[0]->SetPage( PAGE_1 );
	m_pWndEX2[1]->SetPage( PAGE_2 );
	m_pWndEX3->SetPage( PAGE_3 );
	m_pWndEX1->SetPage( PAGE_4 );

	//init
	ExtendPage( m_nExtendPage );
	SetItemSlot( m_nSlotIndex );
	SetLock( m_bLock );

	CRect kRectChild_Action = m_pWndActionSlot->m_rectWindow;

	//self
	int nX = m_pWndRoot->m_rectWindow.right - kRectChild_Action.Width() - m_rectWindow.Width();
	int nY = m_pWndRoot->m_rectWindow.bottom - m_rectWindow.Height();
	Move( nX, nY );
	DelWndStyle( WBS_MOVE );

	//childs
	nX = m_rectWindow.right;
	nY = m_rectWindow.bottom - kRectChild_Action.Height() - 4; //-_-;
	m_pWndActionSlot->Move( nX, nY );
	m_pWndActionSlot->DelWndStyle( WBS_MOVE );

	//����� �����Ͱ� ���ٸ� �ʱ���ġ��.
	int nGap = m_pWndEX2[0]->m_rectWindow.Height();
	if( m_kSavedInfo[0]._kPt == CPoint( 0, 0 ) )
	{
		nX = m_rectWindow.left + nGap;
		nY = m_rectWindow.top - nGap;
		m_pWndEX2[0]->Move( nX, nY );
	}

	if( m_kSavedInfo[1]._kPt == CPoint( 0, 0 ) )
	{
		nX = m_rectWindow.left + nGap;
		nY = nY - nGap - 2;
		m_pWndEX2[1]->Move( nX, nY );
	}

	if( m_kSavedInfo[2]._kPt == CPoint( 0, 0 ) )
	{
		nX = g_Option.m_nResWidth - m_pWndEX3->m_rectWindow.Width();
		nY = 200;
		m_pWndEX3->Move( nX, nY );
	}

	if( m_pWndEX1 )
		m_pWndEX1->UpdateMaxViewSlot( m_kSavedInfo[3]._bVisible );


	//set Wnd ID , no time and go
	DWORD dwSlotIds[ HUD_ITEMSLOT_MAX ] = { WIDC_CUSTOM1, WIDC_CUSTOM2, WIDC_CUSTOM3, WIDC_CUSTOM4, WIDC_CUSTOM5, WIDC_CUSTOM6, WIDC_CUSTOM7, WIDC_CUSTOM8, WIDC_CUSTOM9 };// , WIDC_CUSTOM10 };
	if( 10 == HUD_ITEMSLOT_MAX )
		dwSlotIds[9] = WIDC_CUSTOM10;

	for( int i = 0; i < HUD_ITEMSLOT_MAX; ++i )
		m_dwSlotIDs[i] = dwSlotIds[ i ];
}

void CWndTaskBar_HUD::Serialize( CAr & ar )
{
	//////////////////////////////////////////////////////////////////////////
	if( ar.IsStoring() )
	{
		int nCount			= 0;
		ptrdiff_t nCountTag = 0;

		nCount		= 0;
		nCountTag	= ar.ReserveSpace( sizeof( nCount ) );

		for( int i = 0; i < MAX_SLOT_ITEM_COUNT; i++ )
		{
			for( int j = 0; j < MAX_SLOT_ITEM; j++ )
			{
				if( m_aSlotItem[i][j].m_dwShortcut == SHORTCUT_NONE )		// ���� ����ִ��� �˻�
				{
					ar << i << j;
					ar << m_aSlotItem[i][j].m_dwShortcut <<	m_aSlotItem[i][j].m_dwId << m_aSlotItem[i][j].m_dwType;
					ar << m_aSlotItem[i][j].m_dwIndex << m_aSlotItem[i][j].m_dwUserId << m_aSlotItem[i][j].m_dwData;
					if( m_aSlotItem[i][j].m_dwShortcut == SHORTCUT_CHAT)
						ar.WriteString( m_aSlotItem[i][j].m_szString );
					nCount++;

				}
			}
		}
		ar.WriteReservedSpace( nCountTag, &nCount, sizeof( nCount ) );

		nCount		= 0;
		nCountTag	= ar.ReserveSpace( sizeof( nCount ) );

		ar << nCount;
		for( int i = 0; i < MAX_SLOT_QUEUE; i++ )
		{
			if( m_aSlotQueue[i].m_dwShortcut == SHORTCUT_NONE )		// ���� ����ִ��� �˻�
			{
				ar << i;
				ar << m_aSlotQueue[i].m_dwShortcut << m_aSlotQueue[i].m_dwId << m_aSlotQueue[i].m_dwType;
				ar << m_aSlotQueue[i].m_dwIndex << m_aSlotQueue[i].m_dwUserId << m_aSlotQueue[i].m_dwData;
				nCount++;
			}
		}
		ar << m_nActionPoint;

		ar.WriteReservedSpace( nCountTag, &nCount, sizeof( nCount ) );
	}
	else
	{
		InitTaskBar();
		int nCount	= 0;

		nCount	= 0;
		ar >> nCount;	// slot item count

		for( int i = 0; i < nCount; i++ )
		{
			int nIndex	= 0;
			int nIndex2	= 0;

			ar >> nIndex >> nIndex2;	// index
			ar >> m_aSlotItem[nIndex][nIndex2].m_dwShortcut >>	m_aSlotItem[nIndex][nIndex2].m_dwId >> m_aSlotItem[nIndex][nIndex2].m_dwType;
			ar >> m_aSlotItem[nIndex][nIndex2].m_dwIndex >> m_aSlotItem[nIndex][nIndex2].m_dwUserId >> m_aSlotItem[nIndex][nIndex2].m_dwData;
			if( m_aSlotItem[nIndex][nIndex2].m_dwShortcut == SHORTCUT_CHAT )
				ar.ReadString( m_aSlotItem[nIndex][nIndex2].m_szString, _countof( m_aSlotItem[nIndex][nIndex2].m_szString ) );
#ifdef __CLIENT
			else if( m_aSlotItem[nIndex][nIndex2].m_dwShortcut == SHORTCUT_ITEM )		//sun: 8�� �۾�
			{
				if( g_pPlayer )
				{
					FLItemBase* pItemBase	= g_pPlayer->GetItemId( m_aSlotItem[nIndex][nIndex2].m_dwId );
					if( pItemBase && pItemBase->GetProp()->dwPackMax > 1 )	// ���� ������ �������̸�?
						m_aSlotItem[nIndex][nIndex2].m_dwItemId	= pItemBase->m_dwItemId;
				}
			}
#endif	// __CLIENT
			m_aSlotItem[nIndex][nIndex2].m_dwIndex = nIndex2;
			SetTaskBarTexture( &m_aSlotItem[nIndex][nIndex2] );
		}

		nCount	= 0;
		ar >> nCount;

		for( int i = 0; i < nCount; i++ )
		{
			int nIndex	= 0;
			int nIndex2	= 0;

			ar >> nIndex;
			ar >> m_aSlotQueue[nIndex].m_dwShortcut >> m_aSlotQueue[nIndex].m_dwId >> m_aSlotQueue[nIndex].m_dwType;
			ar >> m_aSlotQueue[nIndex].m_dwIndex >> m_aSlotQueue[nIndex].m_dwUserId >> m_aSlotQueue[nIndex].m_dwData;

			m_aSlotQueue[nIndex].m_dwIndex = nIndex;

			LPSKILL lpSkill = g_pPlayer->GetSkill( m_aSlotQueue[nIndex].m_dwType, m_aSlotQueue[nIndex].m_dwId );
			if( lpSkill )
			{
				SkillProp* pProp = lpSkill->GetProp( );
				if( pProp )
					m_aSlotQueue[nIndex].m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, pProp->szIcon/*pItemBase->GetProp()->szIcon*/), 0xffff00ff, FALSE );
			}
		}

		ar >> m_nActionPoint;
		m_nCurQueueNum = nCount;
	}
	//////////////////////////////////////////////////////////////////////////

	//�����͸� �޾����� �׸��� �ϼ��ؾ� �Ѵ�.
	if( m_pWndActionSlot )
		m_pWndActionSlot->CustomInit( );
}

void CWndTaskBar_HUD::OnCancelSkill( void )
{
	if( NULL == g_pPlayer )
		return;

	g_pPlayer->m_dwReqFlag &= (~REQ_USESKILL);	// ���� �޾���.
	m_nExecute = 0;
	m_nUsedSkillQueue = SKILL_QUEUE_NONE;
	g_pPlayer->ClearDestObj();
	g_WndMng.m_pWndWorld->SetNextSkill( NEXTSKILL_NONE );

	m_pWndActionSlot->Rewind( );
}

void CWndTaskBar_HUD::UsedSkill( int nType, BOOL bSkillPassed )
{
	//SUT_QUEUESTART		���ʽ���
	//SUT_QUEUEING ���ʽ����� �������۵�.
	//int nAngle = 60;
	//if( bPassed )
	//	nAngle += 60;

	if( SUT_QUEUESTART == nType )
		m_pWndActionSlot->Start( bSkillPassed );
	else if( SUT_QUEUEING == nType )
		m_pWndActionSlot->Rotate( );
	else
		return;
}

void CWndTaskBar_HUD::SyncActionSlotReady( )
{
	m_pWndActionSlot->SyncActionSlotReady();
}

void CWndTaskBar_HUD::AllVisible( BOOL bVisible )
{
	if( m_pWndActionSlot )
		m_pWndActionSlot->SetVisible( bVisible );

	if( m_pWndEX1 )
		m_pWndEX1->SetVisible( bVisible );

	if( m_pWndEX2[0] )
		m_pWndEX2[0]->SetVisible( bVisible );

	if( m_pWndEX2[1] )
		m_pWndEX2[1]->SetVisible( bVisible );

	if( m_pWndEX3 )
		m_pWndEX3->SetVisible( bVisible );
}

void CWndTaskBar_HUD::ExtendPage( const int nAdded )
{
	if( nAdded > 0 )
		PLAYSND( SND_PC_SLOTADD );
	else if( nAdded < 0 )
		PLAYSND( SND_PC_SLOTREMOVE );
	else
	{
		//ok
	}

	m_nExtendPage += nAdded;

	if( m_nExtendPage < 0 )
		m_nExtendPage = 0;

	if( m_nExtendPage > 3 )
		m_nExtendPage = 3;

	if( m_nExtendPage < 0 || m_nExtendPage > 3 )
	{
		FLASSERT( 0 );
		return;
	}

	m_pWndEX2[0]->SetVisible( FALSE );
	m_pWndEX2[1]->SetVisible( FALSE );
	m_pWndEX3->SetVisible( FALSE );

	switch( m_nExtendPage )
	{
	case 0:
		break;
	case 3:
		m_pWndEX3->SetVisible( TRUE );
	case 2:
		m_pWndEX2[1]->SetVisible( TRUE );
	case 1:
		m_pWndEX2[0]->SetVisible( TRUE );
		break;
	default: 
		break;
	}
}

void CWndTaskBar_HUD::SetPage( const int nPage )
{
	m_nSlotIndex = nPage;
}

BOOL CWndTaskBar_HUD::SetShortcut2( int nPage, int nIndex, DWORD dwShortcut, DWORD dwType, DWORD dwId, CTexture* pTexture )
{
	if( NULL == g_pPlayer )
		return FALSE;

	LPSHORTCUT pShortcut = NULL;
	if(dwShortcut == SHORTCUT_CHAT)
	{
		int nchatshortcut = 0;
		for( int i=0; i<MAX_SLOT_ITEM_COUNT; i++ )
		{
			for( int j=0; j<MAX_SLOT_ITEM; j++ )
			{
				if( m_aSlotItem[i][j].m_dwShortcut == SHORTCUT_CHAT )
					nchatshortcut++;
			}
		}

		if(nchatshortcut > 9)
		{
			g_WndMng.PutString( prj.GetText( TID_GAME_MAX_SHORTCUT_CHAT ), NULL, prj.GetTextColor( TID_GAME_MAX_SHORTCUT_CHAT ) );
			return FALSE;
		}
	}

	if( nPage < 0 || nPage >= MAX_SLOT_ITEM_COUNT || nIndex < 0 || nIndex >= MAX_SLOT_ITEM )
	{
		FLASSERT( 0 );
		FLERROR_LOG( PROGRAM_NAME, _T( "nPage %d, nIndex %d" ), nPage, nIndex );
		return FALSE;
	}

	pShortcut = &m_aSlotItem[nPage][ nIndex ];

	if( pTexture == NULL )
	{
		FLItemBase* pItemBase;
		if( dwShortcut == SHORTCUT_APPLET ) 
		{
			AppletFunc* pAppletFunc = g_WndMng.GetAppletFunc( dwId );
			pShortcut->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice,  MakePath( DIR_ICON, pAppletFunc->m_pszIconName ), 0xffff00ff );
		}
		else
		if( dwShortcut == SHORTCUT_ITEM ) 
		{
			pItemBase = g_pPlayer->GetItemId( dwId );

			CTexture* pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, pItemBase->GetProp()->szIcon), 0xffff00ff );
			
			if( pItemBase )
				pItemBase->SetTexture( pTexture ); 

			pShortcut->m_pTexture = pTexture ;
		}
		else
		if( dwShortcut == SHORTCUT_MOTION ) 
		{
			MotionProp* pMotionProp = prj.GetMotionProp( dwId );
			if(pMotionProp)		//061206 ma	8���� �� ��ǰ����� ���� ���� �߰�	propMotion.txt
			{
				pMotionProp->pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, pMotionProp->szIconName ), 0xffff00ff );
				pShortcut->m_pTexture = pMotionProp->pTexture;
			}
			else
				return FALSE;
		}
		else
		if( dwShortcut == SHORTCUT_SKILL ) 
		{
			if( pShortcut->m_dwType == 2 )
			{
				SkillProp* pProp =  prj.GetPartySkill( pShortcut->m_dwId );

				if( pProp )
					pShortcut->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, pProp->szIcon/*pItemBase->GetProp()->szIcon*/), 0xffff00ff );
			}
			else
			{	
				LPSKILL lpSkill = g_pPlayer->GetSkill( dwType, dwId );

				if( lpSkill )
					pShortcut->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, lpSkill->GetProp()->szIcon/*pItemBase->GetProp()->szIcon*/), 0xffff00ff, TRUE );
			}
		}	
		else
		if( dwShortcut == SHORTCUT_SKILLFUN ) 
			pShortcut->m_pTexture = pTexture;
			
	}
	else 
		pShortcut->m_pTexture = pTexture;

	if( dwShortcut == SHORTCUT_ITEM )
	{
		FLItemBase* pItemBase	= g_pPlayer->GetItemId( dwId );
		if( pShortcut && pItemBase && pItemBase->GetProp()->dwPackMax > 1 )	// ���� ������ �������̸�?
			pShortcut->m_dwItemId	= pItemBase->m_dwItemId;
	}

	pShortcut->m_dwShortcut = dwShortcut   ;
	pShortcut->m_dwType     = dwType;
	pShortcut->m_dwIndex    = nIndex;//0;//dwIndex;
	pShortcut->m_dwId       = dwId;
	pShortcut->m_dwUserId   = 0 ;
	pShortcut->m_dwData     = 1;		//nWhere �� �׻� Item���� 19�� ���ķ� ����
	FLStrcpy( pShortcut->m_szString, _countof( pShortcut->m_szString ), m_GlobalShortcut.m_szString );//, sizeof(pShortcut->m_szString) );

	if( dwShortcut == SHORTCUT_LORDSKILL)
		pShortcut->m_dwId--;

	g_DPlay.SendAddItemTaskBar( nPage, nIndex, pShortcut );

	return TRUE;
}

BOOL CWndTaskBar_HUD::Process( void )
{
	// ���� ��� �����ؾ��ϴ°͵��� ���⼭...
	//CPoint point = GetMousePoint();

	//CRect rect = CRect( POINT_APPLET_X, POINT_APPLET_Y, POINT_APPLET_X + ICON_SIZE, POINT_APPLET_Y + ICON_SIZE );
	//for( int i = 0; i < m_nMaxSlotApplet; i++ )
	//{
	//	LPSHORTCUT lpShortcut = &m_aSlotApplet[ i ] ;
	//	if( !lpShortcut->IsEmpty() && rect.PtInRect( point) )
	//	{
	//		MotionProp* pMotionProp = prj.GetMotionProp( lpShortcut->m_dwId );
	//		
	//		if( IsShortcut( lpShortcut, SHORTCUT_MOTION, MOT_BASE_CHEER ) )
	//			PutTooTip( lpShortcut, point, &rect );
	//	}
	//	rect += CPoint( ICON_SIZE, 0 );
	//}
	//
	//rect = CRect( POINT_ITEM_X, POINT_ITEM_Y, POINT_ITEM_X + ICON_SIZE, POINT_ITEM_Y + ICON_SIZE );
	//for( int i = 0; i < MAX_SLOT_ITEM; i++ )
	//{
	//	LPSHORTCUT lpShortcut = &m_paSlotItem[ i ] ;
	//	if( !lpShortcut->IsEmpty() && rect.PtInRect( point)  )
	//	{
	//		if( IsShortcut( lpShortcut, SHORTCUT_MOTION, MOT_BASE_CHEER ) )
	//			PutTooTip( lpShortcut, point,&rect );
	//	}
	//	rect += CPoint( ICON_SIZE, 0 );
	//}
	//rect = CRect( POINT_QUEUE_X, POINT_QUEUE_Y, POINT_QUEUE_X + SKILL_SIZE, POINT_QUEUE_Y + SKILL_SIZE );
	//for( int i = 0; i < MAX_SLOT_QUEUE; i++ )
	//{
	//	LPSHORTCUT lpShortcut = &m_aSlotQueue[ i ] ;
	//	if( !lpShortcut->IsEmpty() && rect.PtInRect( point)  )
	//	{
	//		if( IsShortcut( lpShortcut, SHORTCUT_MOTION, MOT_BASE_CHEER ) )
	//			PutTooTip( lpShortcut, point,&rect );
	//	}
	//	rect += CPoint( SKILL_SIZE, 0 );
	//}

	if( m_nExecute )
	{
		if( m_idTarget != NULL_ID )		// m_idTarget�� NULL_ID�ΰ��� �ڱ� �ڽſ��� �����ⶫ�� ĵ�����Ѽ� �ȵȴ�.
		{
			CMover* pObj = prj.GetMover( m_idTarget );
			if( IsInvalidObj(pObj) )						// �����߿� ����� �������ٸ� ��ų����� ����.
				OnCancelSkill();
		}
//		if( pObj && pObj->IsDie() )						// ������ ����� �׾����� ��ų�� ���� ����.
//			OnCancelSkill();

//		if( (g_pPlayer->m_dwFlag & MVRF_SKILL) == 0 )
//			OnCancelSkill();													// ��ų��� ����
	}
	// �巡�� ���̶���Ʈ ���� ���� ó�� 
//	if( m_GlobalShortcut.IsEmpty() == FALSE )
//	{
//		m_dwHighAlpha += 10;
//		if( m_dwHighAlpha >= 256 )
//			m_dwHighAlpha = 0;
//	}
	// �巡���߿��� ��Ű�� ����� �� ���� 
//	else
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ��Ű ó�� 
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		const PFLSHORTCUTKEY_SYSTEM		pShortcutKey_System( g_xKeyManager->GetShortcutKey_System() );
		
		if( g_bKeyTable[ pShortcutKey_System->KeyEnter ] ) 
		{
			g_bKeyTable[ pShortcutKey_System->KeyEnter ] = FALSE;
			CWndBase* pWndBase = GetWndBase( APP_COMMUNICATION_CHAT );
			if( pWndBase == NULL )
			{
				g_WndMng.ObjectExecutor( SHORTCUT_APPLET, APP_COMMUNICATION_CHAT );
				CWndChat* pWndChat = (CWndChat*)GetWndBase( APP_COMMUNICATION_CHAT );
				if( pWndChat != NULL )
				{
					pWndChat->m_wndEdit.SetFocus();
				}
			}
		}
		else
		{
			AppletFunc* pApplet;
			DWORD dwIdApplet;
			POSITION pos = g_WndMng.m_mapAppletFunc.GetStartPosition();
			BOOL bKey = FALSE;
			while( pos )
			{
				g_WndMng.m_mapAppletFunc.GetNextAssoc( pos, dwIdApplet, (void*&)pApplet );
				
				if( g_bKeyTable[ pApplet->m_cHotkey ] )			//gmpbigsun: �Ͽ�¡ YŰ( ����â ) �˻� 
				{
					g_Error_State.m_ch = pApplet->m_cHotkey;
					g_Error_State.m_bIsShowWnd = TRUE;
					g_Error_State.m_dwWndId = pApplet->m_dwIdApplet;
					
					g_WndMng.ObjectExecutor( SHORTCUT_APPLET, pApplet->m_dwIdApplet );
					g_bKeyTable[ pApplet->m_cHotkey ] = FALSE;
				}
			}
			
			CWndRegVend* pWndRegVend = (CWndRegVend*)g_WndMng.GetWndBase(APP_VENDOREX_SELL);
			CWndRegVend* pWndVendorBuy = (CWndRegVend*)g_WndMng.GetWndBase(APP_VENDOREX_BUY);
			
			if( NULL == pWndRegVend && NULL == pWndVendorBuy )
			{
				DWORD* dwHotkey = NULL;

				DWORD dwHotkey1[10] = { 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79 };		//f1,f2,f3...
				DWORD dwHotkey2[10] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30 };		//1,2,3...

				//���� Ű �˻�
				int nRealPage = -1;
				int i = 0;
				for( i; i < 10; ++i )
				{
					if( g_bKeyTable[ VK_MENU ] && g_bKeyTable[ dwHotkey2[i] ] )
					{
						nRealPage = m_pWndEX2[0]->GetPage();
						dwHotkey = dwHotkey2;
						break;
					}
					else if( g_bKeyTable[ dwHotkey2[i] ] )
					{
						nRealPage = GetPage();
						dwHotkey = dwHotkey2;
						break;
					}
					else if( g_bKeyTable[ dwHotkey1[i] ] )
					{
						nRealPage = m_pWndEX2[1]->GetPage();
						dwHotkey = dwHotkey1;
						break;
					}
				}

				if( -1 != nRealPage )
				{
					LPSHORTCUT lpShortcut = &m_aSlotItem[nRealPage][i];
					if( lpShortcut )
						g_WndMng.ObjectExecutor( lpShortcut );	
					g_bKeyTable[ dwHotkey[i] ] = FALSE;
				}
			}
		}
	}

	UpdateItem();

	if( m_pWndActionSlot )
		m_pWndActionSlot->Process( );

	if( m_pWndEX1 )
		m_pWndEX1->Process( );

	if( m_pWndEX2[0] )
		m_pWndEX2[0]->Process( );

	if( m_pWndEX2[1] )
		m_pWndEX2[1]->Process( );

	if( m_pWndEX3 )
		m_pWndEX3->Process( );
	
	return TRUE;
}

void CWndTaskBar_HUD::OnDestroyChildWnd( CWndBase* pWndChild )
{
	if( pWndChild == m_pWndActionSlot )
	{
		SAFE_DELETE( m_pWndActionSlot );
	}else if( pWndChild == m_pWndEX1 )
	{
		SAFE_DELETE( m_pWndEX1 );
	}else if( pWndChild == m_pWndEX2[0] )
	{
		SAFE_DELETE( m_pWndEX2[0] );
	}else if( pWndChild == m_pWndEX2[1] )
	{
		SAFE_DELETE( m_pWndEX2[1] );
	}else if( pWndChild == m_pWndEX3 )
	{
		SAFE_DELETE( m_pWndEX3 );
	}
}

BOOL CWndTaskBar_HUD::OnDropIcon( LPSHORTCUT pShortcut, CPoint point )
{
	if( NULL == g_pPlayer )
		return FALSE;

	//gmpbigsun( 20111208 ) : ���������� �˼����� ��κ� �״�� ������.
	CRect rect = GetWindowRect();
	rect.DeflateRect( 3, 3);

	m_pWndEX1->SubOnDropIcon( pShortcut, m_pWndEX1->GetMousePoint() );
	if( m_pWndEX2[0]->IsVisible() )
		m_pWndEX2[0]->SubOnDropIcon( pShortcut, m_pWndEX2[0]->GetMousePoint() );

	if( m_pWndEX2[1]->IsVisible() )
		m_pWndEX2[1]->SubOnDropIcon( pShortcut, m_pWndEX2[1]->GetMousePoint() );

	if( m_pWndEX3->IsVisible() )
		m_pWndEX3->SubOnDropIcon( pShortcut, m_pWndEX3->GetMousePoint() );

	// ������ ������ ��� 
	if( pShortcut->m_pFromWnd == this )
	{
		LPSHORTCUT lpShortcut = (LPSHORTCUT) pShortcut->m_dwData;
		// ����Ʈ�� �׽�ũ�ٰ� �ƴ� ���̶��. �ǵ������� ������ ������� �½�ũ�� �ۿ� ���� ����. ������.
		if( GetClientRect().PtInRect( point ) == FALSE )
		{
			// ������ ���� ������ ����
			if( lpShortcut->m_dwData == 0 )
				g_DPlay.SendRemoveAppletTaskBar( (BYTE)( lpShortcut->m_dwIndex ) );
			else
			if( lpShortcut->m_dwData == 1 )
				g_DPlay.SendRemoveItemTaskBar( m_nSlotIndex, (BYTE)( lpShortcut->m_dwIndex ) );
			else
			if( lpShortcut->m_dwData == 2 )
			{
				RemoveSkillQueue( lpShortcut->m_dwIndex );
				return TRUE;
			}
			lpShortcut->Empty();
			return TRUE;
		}
	}

	CWndBase* pWndFrame =  pShortcut->m_pFromWnd->GetFrameWnd();
	if( pWndFrame && ( pWndFrame->GetWndId() != APP_INVENTORY && pWndFrame != g_WndMng.m_pWndTaskBar && pWndFrame->GetWndId() != APP_COMMUNICATION_CHAT 
					&& pWndFrame->GetWndId() != APP_SKILL4 && pWndFrame->GetWndId() != APP_MOTION && pWndFrame->GetWndId() != APP_PARTY 
					&& pWndFrame->GetWndId() != APP_LORD_SKILL && pWndFrame->GetWndId() != APP_NEWTASKBAR02 && pWndFrame->GetWndId() != APP_NEWTASKBAR03 
					&& pWndFrame->GetWndId() != APP_NEWTASKBAR04 && pWndFrame->GetWndId() != APP_SLOT ) )
	{
		SetForbid( TRUE );
		return FALSE;
	}
	
	if( pShortcut->m_dwShortcut == SHORTCUT_SKILL && pShortcut->m_dwType != 2 ) // �شܽ�ų�� �ȵ�
	{
		if( !g_pPlayer->isLearnSkill( pShortcut->m_dwId ) )
// 		LPSKILL pSkill = g_pPlayer->GetSkill( 0, pShortcut->m_dwId );
// 		if( pSkill && (pSkill->dwLevel <= 0 || g_pPlayer->CheckSkill( pSkill->dwSkill ) == FALSE) )
		{
			SetForbid( TRUE );
			return FALSE;
		}
	}

	if( pShortcut->m_dwId == 0 && strlen( pShortcut->m_szString ) < 1 && pShortcut->m_dwShortcut != SHORTCUT_SKILLFUN && pShortcut->m_dwShortcut != SHORTCUT_LORDSKILL)
	{
		SetForbid( TRUE );
		return FALSE;
	}

	// ���ο��� �̵��ǰų�, �ܺο��� ���η� 
	BOOL bForbid = TRUE;

	int nSlot = GetSlotIndexByPoint( point );
	if( -1 != nSlot )
	{
		// ���ο��� �̵� 
		if( pShortcut->m_pFromWnd == this )
		{
			//�����༮�̸� �н�
			LPSHORTCUT lpCurShortcut = (LPSHORTCUT)pShortcut->m_dwData;
			if( &m_aSlotItem[ m_nSlotIndex ][ nSlot ] == lpCurShortcut )
				return FALSE;
		}

		if( pShortcut->m_dwShortcut == SHORTCUT_CHAT )
		{
			CString sChat	= m_GlobalShortcut.m_szString;
			sChat.Replace( "--", "" );
			sChat.Replace( "'", "" );
			sChat.Replace( "/*", "" );
			sChat.Replace( "*/", "" );
			FLStrcpy( m_GlobalShortcut.m_szString, _countof( m_GlobalShortcut.m_szString ), (const char*)sChat );
		}
		else if( pShortcut->m_dwShortcut == SHORTCUT_SKILL )	
		{
			SkillProp* pProp;
			if( pShortcut->m_dwType == 2 )
				pProp =  prj.GetPartySkill( pShortcut->m_dwId );
			else
			{
				LPSKILL pSkill = g_pPlayer->GetSkill( 0, pShortcut->m_dwId );

				if( pSkill )
					pProp = prj.GetSkillProp( pSkill->dwSkill );	
			}
			if( pProp->dwComboStyle != CT_STEP && pProp->dwComboStyle != CT_GENERAL )	
			{
				SetForbid( TRUE );
				return FALSE;	// �����Կ� ����Ϸ��� ��ų�� ���ܱ���� �ƴѰ� ��� ����.
			}
		}
		SetShortcut2( m_nSlotIndex, nSlot, pShortcut->m_dwShortcut, pShortcut->m_dwType, pShortcut->m_dwId, pShortcut->m_pTexture );
		bForbid = FALSE;
	}

//	if( pShortcut->m_pFromWnd != this )
//		SetForbid( bForbid );

	// ������ ������ ���, �Ǵ� �ȿ��� �̵����� ��� �������� �κ� ����  
	// �½�ũ�ٿ� �ִ� �������� ���� ���� ���� �κп� �巡������ ��� 
	if( pShortcut->m_pFromWnd == this )
	{
		LPSHORTCUT lpShortcut = (LPSHORTCUT) pShortcut->m_dwData;
		// ������ ���� ������ ����
		if( lpShortcut->m_dwData == 0 )
			g_DPlay.SendRemoveAppletTaskBar( (BYTE)( lpShortcut->m_dwIndex ) );
		else
		if( lpShortcut->m_dwData == 1 )
			g_DPlay.SendRemoveItemTaskBar( m_nSlotIndex, (BYTE)( lpShortcut->m_dwIndex ) );
		else
		if( lpShortcut->m_dwData == 2 )
		{
			RemoveSkillQueue( lpShortcut->m_dwIndex );
			//g_DPlay.SendSkillTaskBar( );
			return TRUE;
		}
		lpShortcut->Empty();
	}
	return TRUE;
}

BOOL CWndTaskBar_HUD::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase )
{
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
}

void CWndTaskBar_HUD::OnMouseWndSurface( CPoint point )
{
	if( NULL == g_pPlayer )
		return;

	LPWNDCTRL pWnd = NULL;
	CRect rect;

	pWnd = GetWndCtrl( WIDC_CUSTOM14 );
	if( NULL == pWnd )
		return;

	if( pWnd->rect.PtInRect( point ) )
	{
		CString strBuffer;
		strBuffer.Format( prj.GetText( TID_MMI_UIEXPTOOLTIP ), g_pPlayer->GetExp1(), g_pPlayer->GetMaxExp1(), static_cast<float>(g_pPlayer->GetExpPercent() / 100.f) );
		CRect copyRect = pWnd->rect;
		CPoint copyPt = point;
		ClientToScreen( &copyPt );
		ClientToScreen( &copyRect );
		g_toolTip.PutToolTip( 1111, strBuffer, copyRect, copyPt, 0 );
		return;
	}
	
	for( int i = 0; i < HUD_ITEMSLOT_MAX; i++ )
	{
		pWnd = GetWndCtrl( m_dwSlotIDs[ i ] );
		if( NULL == pWnd )
			continue;

		rect = pWnd->rect;

		LPSHORTCUT lpShortcut = &m_paSlotItem[ i ] ;
		if( rect.PtInRect( point) )
		{
			if( FALSE == lpShortcut->IsEmpty() )
			{
			//	if( FALSE == IsShortcut( lpShortcut, SHORTCUT_MOTION, MOT_BASE_CHEER ) )		//#24359 ������� ���������
				{
					PutTooTip( lpShortcut, point, &rect );
					return;
				}
			}
		}
	}

	
}

void CWndTaskBar_HUD::OnRButtonUp(UINT nFlags, CPoint point)
{
	//������ ����
	//int nSlot = GetSlotIndexByPoint( point );
	//if( -1 == nSlot || nSlot >= MAX_SLOT_ITEM )
	//	return;
	//
	//if( FALSE == m_paSlotItem[ nSlot ].IsEmpty() )
	//{
	//	m_paSlotItem[ nSlot ].Empty();
	//	g_DPlay.SendRemoveItemTaskBar( (BYTE)( m_nSlotIndex ), (BYTE)( nSlot ) );	// ������ ���� ������ ����			
	//}
}

void CWndTaskBar_HUD::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	LPSHORTCUT pShortcut = Select( point );
	if( NULL == pShortcut )
		return;
	
	if( pShortcut == m_pSelectShortcut && !pShortcut->IsEmpty() && pShortcut->m_dwData != 2 )
	{
		g_WndMng.ObjectExecutor( pShortcut );
	}
}

void CWndTaskBar_HUD::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_pSelectShortcut = Select( point );
	SetCapture();
}

void CWndTaskBar_HUD::OnMouseMove(UINT nFlags, CPoint point)
{
	if( m_bLock )
		return;

	if( m_bLButtonDown && IsPush() )
	{
		if( m_pSelectShortcut && !m_pSelectShortcut->IsEmpty() ) 
		{
#ifdef __MAINSERVER
			if( m_pSelectShortcut->m_dwId == 400 )
				return;
#endif //__MAINSERVER

			if(m_pSelectShortcut->m_dwShortcut == SHORTCUT_LORDSKILL)
				m_pSelectShortcut->m_dwId++;

			memcpy( &m_GlobalShortcut, m_pSelectShortcut, sizeof( m_GlobalShortcut ) );
			m_GlobalShortcut.m_pFromWnd = this;
			m_GlobalShortcut.m_dwData = (DWORD)m_pSelectShortcut;
			m_pSelectShortcut = NULL;
		}
	}
	
	CWndNeuz::OnMouseMove( nFlags, point );
}

#ifdef __CLIENT
void CWndTaskBar_HUD::SerializeRegInfo( CAr& ar, DWORD& dwVersion )
{
	if( ar.IsLoading() )
	{	
		if( dwVersion == 1 )
		{
			ar >> m_bLock >> m_nExtendPage >> m_nSlotIndex;
			ar >> m_kSavedInfo[0] >> m_kSavedInfo[1] >> m_kSavedInfo[2] >> m_kSavedInfo[3];
		}
	}
	else
	{
		dwVersion = 1;

		ar << m_bLock << m_nExtendPage << m_nSlotIndex;


		INFOCHILD kInfo;
		kInfo._bVisible = m_pWndEX2[0]->IsVisible();
		kInfo._kPt = m_pWndEX2[0]->m_rectWindow.TopLeft();
		ar << kInfo;

		kInfo._bVisible = m_pWndEX2[1]->IsVisible();
		kInfo._kPt = m_pWndEX2[1]->m_rectWindow.TopLeft();
		ar << kInfo;

		kInfo._bVisible = m_pWndEX3->IsVisible();
		kInfo._kPt = m_pWndEX3->m_rectWindow.TopLeft();
		ar << kInfo;

		kInfo._bVisible = m_pWndEX1->GetMaxViewSlot( );		//���� Ȯ�彽���� ������ ���̴°��̰�, ��� ���̴����� �����Ѵ�.
		kInfo._kPt = CPoint( 0, 0 );
		ar << kInfo;
	}
}
#endif

BOOL CWndTaskBar_HUD::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	CWndButton* pWndButton = (CWndButton*) pLResult;
	switch( nID )
	{
		case WIDC_BUTTON1:
			SetLock( !m_bLock );
			break;


		case WIDC_BUTTON2:		//page up
			//SetItemSlot( m_nSlotIndex );
			ExtendPage( 1 );
			break;

		case WIDC_BUTTON3:		//page down
			ExtendPage( -1 );
			break;

		case WIDC_BUTTON4:
			TurnPage( );
			break;

		default:
			{
				if( NULL == pWndButton )
					break;

				switch( pWndButton->m_shortcut.m_dwIndex )
				{
					case APP_TRADE:		g_DPlay.SendTrade( (CMover*)g_WorldMng.Get()->GetObjFocus() );	break;
					default:
						g_WndMng.ObjectExecutor( &pWndButton->m_shortcut );	
						break;
				}
			}
			break;
	}

	return TRUE;
//	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

void CWndTaskBar_HUD::OnDraw( C2DRender* p2DRender ) 
{
	if( NULL == g_pPlayer )
		return;

	//HP
	int nHP_Rate = g_pPlayer->GetHitPointPercent( ); 
	LPWNDCTRL lpHP   = GetWndCtrl( WIDC_CUSTOM11 );
	CRect rectGauge = lpHP->rect;

	int nWidthClient = lpHP->rect.Width();
	int nOffsetRight = (int)( (float)nWidthClient * ( (float)nHP_Rate * 0.01f ) ) ;
	rectGauge.right = rectGauge.left + nOffsetRight;
	m_pTheme->RenderGauge( p2DRender, &rectGauge, 0xffffffff, m_pGaugeHP, &m_texGau_HP );

	//MP
	int nMP_Rate = g_pPlayer->GetManaPointPercent(); 
	LPWNDCTRL lpMP   = GetWndCtrl( WIDC_CUSTOM12 );
	rectGauge = lpMP->rect;

	nWidthClient = lpMP->rect.Width();
	nOffsetRight = (int)( (float)nWidthClient * ( (float)nMP_Rate * 0.01f ) ) ;
	rectGauge.right = rectGauge.left + nOffsetRight;
	m_pTheme->RenderGauge( p2DRender, &rectGauge, 0xffffffff, m_pGaugeMP, &m_texGau_MP );

	// FP
	int nFP_Rate = g_pPlayer->GetFatiguePointPercent();
	LPWNDCTRL lpFP   = GetWndCtrl( WIDC_CUSTOM13 );
	rectGauge = lpFP->rect;
		
	nWidthClient = lpFP->rect.Width();
	nOffsetRight = (int)( (float)nWidthClient * ( (float)nFP_Rate * 0.01f ) ) ;
	rectGauge.right = rectGauge.left + nOffsetRight;
	m_pTheme->RenderGauge( p2DRender, &rectGauge, 0xffffffff, m_pGaugeFP, &m_texGau_FP );

	// EXP
	int nEXP_Rate = g_pPlayer->GetExpPercent( ) / 100;
	if( nEXP_Rate > 100 )
		nEXP_Rate = 100;

	LPWNDCTRL lpEXP   = GetWndCtrl( WIDC_CUSTOM14 );
	rectGauge = lpEXP->rect;
		
	nWidthClient = lpEXP->rect.Width();
	nOffsetRight = (int)( (float)nWidthClient * ( (float)nEXP_Rate * 0.01f ) ) ;
	rectGauge.right = rectGauge.left + nOffsetRight;
	m_pTheme->RenderGauge( p2DRender, &rectGauge, 0xffffffff, m_pGaugeEXP, &m_texGau_EXP );

	p2DRender->RenderTexture( lpEXP->rect.TopLeft(), &m_texGau_EXP_Deco );

	// ������ ��ġ ǥ�� ( �����ڵ带 ������ )
	DWORD dwColor = D3DCOLOR_ARGB(255, 255, 255, 255 );

	char cbufHp[16] = {0,};
	char cbufMp[16] = {0,};
	char cbufFp[16] = {0,};

	int nTopGap = 0;
	
	int nCharHP = 0, nCharMP = 0, nCharFP = 0; // nCharEXP; //, nCharPXP;
	CSize size = p2DRender->m_pFont->GetTextExtent("8");

	nCharHP = FLSPrintf(cbufHp, _countof( cbufHp ), "%d", g_pPlayer->GetHitPoint());
	nCharMP = FLSPrintf(cbufMp, _countof( cbufMp ), "%d", g_pPlayer->GetManaPoint());
	nCharFP = FLSPrintf(cbufFp, _countof( cbufFp ), "%d", g_pPlayer->GetFatiguePoint());

	int x = lpHP->rect.right - 90;
	p2DRender->TextOut( x - (int)(((float)nCharHP / 2.0f) * size.cx), lpHP->rect.top - nTopGap, cbufHp, dwColor, 0xff000000 );
	p2DRender->TextOut( x - (int)(((float)nCharMP / 2.0f) * size.cx), lpMP->rect.top - nTopGap, cbufMp, dwColor, 0xff000000 );
	p2DRender->TextOut( x - (int)(((float)nCharFP / 2.0f) * size.cx), lpFP->rect.top - nTopGap, cbufFp, dwColor, 0xff000000 );
	
	nCharHP = FLSPrintf(cbufHp, _countof( cbufHp ), "%d", g_pPlayer->GetMaxHitPoint());
	nCharMP = FLSPrintf(cbufMp, _countof( cbufMp ), "%d", g_pPlayer->GetMaxManaPoint());
	nCharFP = FLSPrintf(cbufFp, _countof( cbufFp ), "%d", g_pPlayer->GetMaxFatiguePoint());
	
	x = lpHP->rect.right - 40;
	p2DRender->TextOut( x - (int)(((float)nCharHP / 2.0f) * size.cx), lpHP->rect.top - nTopGap, cbufHp, dwColor, 0xff000000 );
	p2DRender->TextOut( x - (int)(((float)nCharMP / 2.0f) * size.cx), lpMP->rect.top - nTopGap, cbufMp, dwColor, 0xff000000 );
	p2DRender->TextOut( x - (int)(((float)nCharFP / 2.0f) * size.cx), lpFP->rect.top - nTopGap, cbufFp, dwColor, 0xff000000 );
		
	int nWidthCustom = lpHP->rect.Width();
	int nGap = (int)( ((nWidthCustom / 2.0f) + (size.cx / 2.0f)) );

	p2DRender->TextOut( lpHP->rect.right - nGap, lpHP->rect.top - nTopGap, "/", dwColor, 0xff000000 );
	p2DRender->TextOut( lpMP->rect.right - nGap, lpMP->rect.top - nTopGap, "/", dwColor, 0xff000000 );
	p2DRender->TextOut( lpFP->rect.right - nGap, lpFP->rect.top - nTopGap, "/", dwColor, 0xff000000 );

	//WndTaskBar::OnDraw ����.
	DWORD dwCur = g_tmCurrent;
	CRect rect = GetWindowRect();

	CD3DFont* pOldFont = p2DRender->GetFont(); 
	p2DRender->SetFont( CWndBase::m_Theme.m_pFontWndTitle ); 

	// ������ ������ ��� 
	LPWNDCTRL pWnd = NULL;
	CPoint point;

	int nOnSlot = -1;
	CPoint ptMouse = GetMousePoint();
	ClientToScreen( &ptMouse );
	if( this == IsOnWndBase2( ptMouse ) )
		nOnSlot = GetSlotIndexByPoint( GetMousePoint() );

	float fFrameUP = ( g_Neuz.m_fFPS / 60.0f  );
	if( fFrameUP > 1.0f )
		fFrameUP = 1.0f;
	else fFrameUP = 60.0f / g_Neuz.m_fFPS;

	for( int i = 0; i < HUD_ITEMSLOT_MAX; i++ )
	{
		pWnd = GetWndCtrl( m_dwSlotIDs[i] );
		if( NULL == pWnd )
		{
			FLASSERT( 0 );
			continue;
		}

		point = pWnd->rect.TopLeft( );

		//���콺 �� ���� ǥ��
		if( nOnSlot == i )
			p2DRender->RenderTexture( point - CPoint( 4, 4 ), &m_texOnSlot );

		LPSHORTCUT lpShortcut = &m_paSlotItem[ i ] ;
		if( !lpShortcut->IsEmpty() )
		{
			if( lpShortcut->m_pTexture )
			{
				p2DRender->RenderTexture( point, lpShortcut->m_pTexture );
			}

			//����ȿ��
			if( lpShortcut->m_nExtraFrame > 0 )
			{
				float fRate = (float)lpShortcut->m_nExtraFrame / 60.0f;
				
				p2DRender->RenderTexture( point, &m_texUsingSlot, DWORD(255 * fRate) );
				lpShortcut->m_nExtraFrame -= (int)( fFrameUP * 4.0f );
				if( lpShortcut->m_nExtraFrame < 0 )
					lpShortcut->m_nExtraFrame = 0;
			}

			if( lpShortcut->m_dwShortcut == SHORTCUT_ITEM )
			{
				FLItemBase* pItemBase = g_pPlayer->GetItemId( lpShortcut->m_dwId );
				FLItemElem* pItemElem = (FLItemElem*)pItemBase;
				if( pItemElem  )
				{
					if( pItemElem->GetProp()->dwPackMax > 1 )
					{
						TCHAR szTemp[ 32 ];

						//sun: 8�� �۾�
						FLSPrintf( szTemp, _countof( szTemp ), "%d", g_pPlayer?g_pPlayer->m_Inventory.GetItemNumByItemId( pItemElem->m_dwItemId ): 0 );
						
						CSize size = m_p2DRender->m_pFont->GetTextExtent( szTemp );
						p2DRender->TextOut( point.x + 32 - size.cx, point.y + 32 - size.cy + 2, szTemp, 0xffffffff );
						//p2DRender->TextOut( point.x + 31 - size.cx, point.y + 31 - size.cy, szTemp, 0xffb0b0f0 );
					}
			
					DWORD dwGroup = g_pPlayer->m_cooltimeMgr.GetGroup( pItemElem->GetProp() );
					if( dwGroup )
					{
						DWORD dwEnd = g_pPlayer->m_cooltimeMgr.GetTime( dwGroup );		// �̺�Ʈ ���� �ð� 
						if( dwEnd != 0 && dwEnd > dwCur )
						{
							DWORD dwBase = g_pPlayer->m_cooltimeMgr.GetBase( dwGroup );	// �̺�Ʈ ���� �ð� 
							RenderRadar( p2DRender, point, dwCur - dwBase, dwEnd - dwBase );
						}
					}
				}
			} 
			else if( lpShortcut->m_dwShortcut == SHORTCUT_SKILL && lpShortcut->m_dwType != 2 ) //�شܽ�ų�� ��Ÿ�� ���� Render�� ���� �ʴ´�.
			{
				RenderCollTime( point, lpShortcut->m_dwId, p2DRender );
			}
		//sun: 12, ����
			else if( lpShortcut->m_dwShortcut == SHORTCUT_LORDSKILL)
			{
					RenderLordCollTime( point, lpShortcut->m_dwId, p2DRender );
			}
			else
			if( lpShortcut->m_dwShortcut == SHORTCUT_MOTION )
			{
				if( lpShortcut->m_dwId == MOT_BASE_ESCAPE )
				{
					PT_ITEM_SPEC pItem = g_xSpecManager->GetSpecItem( g_AddSMMode.dwSMItemID[SM_ESCAPE] );

					if( pItem && g_pPlayer )
					{
						CTimeSpan ct( g_pPlayer->m_dwSMTime[SM_ESCAPE] );
						
						if( ct.GetTotalSeconds() )
						{
							point.y -= 1;
							point.x += 1;
							RenderRadar( m_p2DRender, point, (DWORD)( pItem->dwCircleTime - ct.GetTotalSeconds() ), pItem->dwCircleTime );
						}
					}
				}
			}

				
		}
		// hotkey ��� 
		CHAR cHotkey = ( i == 9 ) ? '0' : '1' + i;
		CPoint ptHotkey( point.x + 8, point.y - 9 );
		DRAW_HOTKEY( p2DRender, ptHotkey, cHotkey );
	}

	p2DRender->SetFont( pOldFont );
}

BOOL CWndTaskBar_HUD::SetSkillQueue( int nIndex, DWORD dwType, DWORD dwId, CTexture* pTexture )
{
	if( NULL == g_pPlayer )
		return FALSE;

	// ��ųť �����߿� ��� �ȵ�.
	if( 0 != m_nExecute )		
		return FALSE;		

	if( nIndex < 0 || nIndex >= HUD_ACTIONSLOT_MAX )
		return FALSE;
	

	if( m_nCurQueueNum >= HUD_ACTIONSLOT_MAX )
		return FALSE;

	// m_nCurQueueNum ���� ���� ���Կ� ����ִ°� �ִٸ� ���� ���´�. Ŭ��������.
	for( int i = 0; i < m_nCurQueueNum; i++ )
	{
		if( m_aSlotQueue[ i ].IsEmpty() )
		{
			for( i = 0; i < m_nCurQueueNum; i++ )
				m_aSlotQueue[ i ].Empty();
			
			m_nCurQueueNum = 0;
			break;
		}
	}

	 if( nIndex > m_nCurQueueNum )
		 nIndex = m_nCurQueueNum;

	LPSHORTCUT pShortcut = NULL;
	CRect rect = GetWindowRect();

	LPSKILL lpSkill = g_pPlayer->GetSkill( 0, dwId );
	FLERROR_ASSERT_LOG_RETURN( lpSkill, FALSE, _T("[������ �۾� �� HUD] ��ų �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

	DWORD dwSkill = lpSkill->dwSkill;
	DWORD dwLevel = lpSkill->dwLevel;
	SkillProp *pSkillProp = prj.GetSkillProp( dwSkill );
	DWORD dwComboStyleSrc = pSkillProp->dwComboStyle;

	if( dwLevel <= 0 )
		return FALSE;
	
	if( nIndex < m_nCurQueueNum ) // insert		// nIndex�� ������ ĭ�� �ε���
	{
		for( int i = m_nCurQueueNum; i > nIndex; i-- )
		{
			memcpy( &m_aSlotQueue[ i ], &m_aSlotQueue[ i - 1 ], sizeof( SHORTCUT ) );
		}
		pShortcut = &m_aSlotQueue[ nIndex ];
		m_nCurQueueNum++;
	}
	else // add
	{
		pShortcut = &m_aSlotQueue[ m_nCurQueueNum ];
		nIndex = m_nCurQueueNum;
		m_nCurQueueNum++;
	}
	
	m_nCurQueue = -1;
	if( pTexture == NULL )
	{
		LPSKILL lpSkill = g_pPlayer->GetSkill( dwType, dwId );

		if( lpSkill )
			pShortcut->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, lpSkill->GetProp()->szIcon), 0xff000000, FALSE );
	}
	else 
		pShortcut->m_pTexture = pTexture;


	pShortcut->m_dwShortcut = SHORTCUT_SKILL   ;
	pShortcut->m_dwType     = dwType; // ���� 
	pShortcut->m_dwIndex    = nIndex;//dwIndex; // ��ų ��Ʈ�ѿ����� ���� 
	pShortcut->m_dwId       = dwId; // ��ų �ε��� 
	pShortcut->m_dwUserId   = 0 ;
	pShortcut->m_dwData     = 2;
	FLStrcpy( pShortcut->m_szString, _countof( pShortcut->m_szString ), m_GlobalShortcut.m_szString );
	
	// ��ų ��� ���⼭ ������ ������ ��
	g_DPlay.SendSkillTaskBar();	

	m_pWndActionSlot->UpdateSlot( );

	return TRUE;
}

void CWndTaskBar_HUD::RemoveSkillQueue( int nIndex, BOOL bSend )
{
	// ��ųť �����߿� �������� �ʴ´�.
	if( 0 != m_nExecute )	
		return;		

	if( nIndex < 0 || nIndex >= HUD_ACTIONSLOT_MAX )
	{
		FLASSERT( 0 );
		return;
	}

	if( FALSE == m_aSlotQueue[ nIndex ].IsEmpty() )
	{
		int i = nIndex;

		for( ; i < HUD_ACTIONSLOT_MAX - 1; ++i )
		{
			memcpy( &m_aSlotQueue[ i ], &m_aSlotQueue[ i+1 ], sizeof( SHORTCUT ) );
			m_aSlotQueue[i].m_dwIndex = i;
		}
		
		//�ϳ��� ���� �Ǹ������� ������ �󽽷��� �ǹǷ�..
		m_aSlotQueue[i].m_dwShortcut = SHORTCUT_NONE;
		m_aSlotQueue[i].m_dwId = 0;
		m_nCurQueueNum--;

		if( m_nCurQueueNum < 0 )
			m_nCurQueueNum = 0;

	}

	if( bSend )
		g_DPlay.SendSkillTaskBar( );

	m_pWndActionSlot->UpdateSlot( );
}

LPSHORTCUT CWndTaskBar_HUD::Select( CPoint point )
{
	int nSlot = GetSlotIndexByPoint( point );
	if( -1 == nSlot || nSlot >= MAX_SLOT_ITEM )
		return NULL;

	LPSHORTCUT pShortcut = &m_paSlotItem[ nSlot ];
	if( FALSE ==  pShortcut->IsEmpty() )
		return pShortcut;

	return NULL;
}

void CWndTaskBar_HUD::SetItemSlot( int nPage )
{
	//���ν��԰� Ȯ�彽��2���� ��ȯ�ϴ� ������.
	if( nPage < 0 )
		nPage = 2;

	if( nPage > 2 )
		nPage = 0;

	m_nSlotIndex = nPage;
	m_paSlotItem = m_aSlotItem[ m_nSlotIndex ];

	int nPage1 = nPage + 1;
	if( nPage1 > 2 )
		nPage1 -= 3;

	int nPage2 = nPage1 + 1;
	if( nPage2 > 2 )
		nPage2 -= 3;

	m_pWndEX2[0]->SetPage( nPage1 );
	m_pWndEX2[1]->SetPage( nPage2 );

}

void CWndTaskBar_HUD::TurnPage( BOOL bRight )
{
	if( bRight )
		SetItemSlot( m_nSlotIndex - 1 );
	else 
		SetItemSlot( m_nSlotIndex + 1 );

	PLAYSND( SND_PC_SLOTCHANGE );
}

void CWndTaskBar_HUD::UpdateButton( )
{
	CWndButton* pWndLock = ( CWndButton* )GetDlgItem( WIDC_BUTTON1 );
	if( NULL == pWndLock )
		return;

	if( m_bLock )
	{
		pWndLock->SetCheck( 0 );
		m_pWndEX1->DelWndStyle( WBS_MOVE );
		m_pWndEX2[0]->DelWndStyle( WBS_MOVE );
		m_pWndEX2[1]->DelWndStyle( WBS_MOVE );
		m_pWndEX3->DelWndStyle( WBS_MOVE );
	}
	else 
	{
		pWndLock->SetCheck( 2 );
		m_pWndEX1->AddWndStyle( WBS_MOVE );
		m_pWndEX2[0]->AddWndStyle( WBS_MOVE );
		m_pWndEX2[1]->AddWndStyle( WBS_MOVE );
		m_pWndEX3->AddWndStyle( WBS_MOVE );
	}
}

void CWndTaskBar_HUD::SetLock( BOOL bLock )
{
	m_bLock = bLock;
	UpdateButton( );
}

void CWndTaskBar_HUD::Rotate( int nAngle )
{
	m_pWndActionSlot->Rotate( nAngle );
}
	
void CWndTaskBar_HUD::Rewind( )
{
	m_pWndActionSlot->Rewind( );
}

int CWndTaskBar_HUD::GetSlotIndexByPoint( const CPoint& kPoint )
{
	LPWNDCTRL pWnd = NULL;
	for( int i = 0; i < HUD_ITEMSLOT_MAX; ++i )
	{
		pWnd = GetWndCtrl( m_dwSlotIDs[ i ] );
		if( pWnd )
		{
			if( pWnd->rect.PtInRect( kPoint ) )
				return i;
		}
	}

	return -1;
}

HRESULT CWndTaskBar_HUD::RestoreDeviceObjects()
{
	CWndBase::RestoreDeviceObjects();
	if( m_pGaugeHP == NULL )
		m_pApp->m_pd3dDevice->CreateVertexBuffer(sizeof(TEXTUREVERTEX2) * 3 * 6, 
												 D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 
												 D3DFVF_TEXTUREVERTEX2, 
												 D3DPOOL_DEFAULT, 
												 &m_pGaugeHP, 
												 NULL);
	FLASSERT(m_pGaugeHP != NULL);

	if( m_pGaugeMP == NULL )
		m_pApp->m_pd3dDevice->CreateVertexBuffer(sizeof(TEXTUREVERTEX2) * 3 * 6, 
												 D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 
												 D3DFVF_TEXTUREVERTEX2, 
												 D3DPOOL_DEFAULT, 
												 &m_pGaugeMP, 
												 NULL);
	FLASSERT(m_pGaugeMP != NULL);

	if( m_pGaugeFP == NULL )
		m_pApp->m_pd3dDevice->CreateVertexBuffer(sizeof(TEXTUREVERTEX2) * 3 * 6, 
												 D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 
												 D3DFVF_TEXTUREVERTEX2, 
												 D3DPOOL_DEFAULT, 
												 &m_pGaugeFP, 
												 NULL);
	FLASSERT(m_pGaugeMP != NULL);

	if( m_pGaugeEXP == NULL )
		m_pApp->m_pd3dDevice->CreateVertexBuffer(sizeof(TEXTUREVERTEX2) * 3 * 6, 
												 D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 
												 D3DFVF_TEXTUREVERTEX2, 
												 D3DPOOL_DEFAULT, 
												 &m_pGaugeEXP, 
												 NULL);
	FLASSERT(m_pGaugeEXP != NULL);
	
	return S_OK;
}
HRESULT CWndTaskBar_HUD::InvalidateDeviceObjects()
{
	CWndBase::InvalidateDeviceObjects();
    SAFE_RELEASE( m_pGaugeHP );
	SAFE_RELEASE( m_pGaugeMP );
	SAFE_RELEASE( m_pGaugeFP );
	SAFE_RELEASE( m_pGaugeEXP );

	return S_OK;
}
HRESULT CWndTaskBar_HUD::DeleteDeviceObjects()
{
	CWndBase::DeleteDeviceObjects();
	SAFE_RELEASE( m_pGaugeHP );
	SAFE_RELEASE( m_pGaugeMP );
	SAFE_RELEASE( m_pGaugeFP );
	SAFE_RELEASE( m_pGaugeEXP );
	return S_OK;
}

//----------------------------------------------------------------------------------------------------------
//CWndTaskBar_HUD_EX1
//----------------------------------------------------------------------------------------------------------
CWndTaskBar_HUD_EX1::CWndTaskBar_HUD_EX1( )
{
	m_pWndMaster = NULL;
	m_nMyPage = -1;
	m_bReady = FALSE;
	m_nMaxViewSlot = -1;

	m_paSlotItem = NULL;
	m_bPullMinMax = FALSE;
}

CWndTaskBar_HUD_EX1::~CWndTaskBar_HUD_EX1( )
{
}

// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndTaskBar_HUD_EX1::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_NEWTASKBAR02, 0, CPoint( 0, 0 ), pWndParent );
} 

void CWndTaskBar_HUD_EX1::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
		
	// ������ �߾����� �ű�� �κ�.
	//MoveParentCenter();

	m_kTexSlot.LoadTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), "WndExpansionSlot01_1.tga" ), 0xffff00ff, TRUE ); 

	m_pWndMaster = (CWndTaskBar_HUD*)m_pParentWnd;

	CWndStatic* pWndStatic = ( CWndStatic* )GetDlgItem( WIDC_STATIC1 );
	if( pWndStatic )
		pWndStatic->AddWndStyle( WSS_NOPICKUP );

	UpdateWndOrder( );
} 

BOOL CWndTaskBar_HUD_EX1::Process( void )
{
	UpdateItem();
	return TRUE;
}

void CWndTaskBar_HUD_EX1::OnDestroyChildWnd( CWndBase* pWndChild )
{
}

void CWndTaskBar_HUD_EX1::SetWndRect( CRect rectWnd, BOOL bOnSize )
{
	if( m_bReady )
	{
		//top �׻� �θ��� top, bottom
		rectWnd.top = m_pParentWnd->m_rectWindow.top + 22;
		rectWnd.bottom = rectWnd.top + m_kOrgRect.Height();

		int nMaxLeftX = m_pParentWnd->m_rectWindow.left - 12;		//���ʱ����̰� ������ ��.
		int nMinLeftX = m_pParentWnd->m_rectWindow.left - m_kOrgRect.Width() + 60; // �����λ� �ʿ���� ��������
		int nRightX = m_pParentWnd->m_rectWindow.left + 35;		//35 �������� ���� ���ƺ��̴±���.

		//left
		if( rectWnd.left > nMaxLeftX )
			rectWnd.left = nMaxLeftX;

		if( rectWnd.left < nMinLeftX )
			rectWnd.left = nMinLeftX;

		//right
		rectWnd.right = nRightX;

		UpdateSlotIndex( );
	}
	
	CWndNeuz::SetWndRect( rectWnd, bOnSize );
}

void CWndTaskBar_HUD_EX1::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_pWndMaster->m_pSelectShortcut = Select( point );
	SetCapture();
}

void CWndTaskBar_HUD_EX1::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	LPSHORTCUT pShortcut = Select( point );
	if( NULL == pShortcut )
		return;
	
	if( pShortcut == m_pWndMaster->m_pSelectShortcut && !pShortcut->IsEmpty() && pShortcut->m_dwData != 2 )
	{
		g_WndMng.ObjectExecutor( pShortcut );
	}
}

void CWndTaskBar_HUD_EX1::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	LPWNDCTRL pWndCtrl = GetWndCtrl( WIDC_STATIC1 );
	if( pWndCtrl )
	{
		if( pWndCtrl->rect.PtInRect( point ) )
		{
			PullMinMax( );
			return;
		}
	}
}

void CWndTaskBar_HUD_EX1::OnMouseMove(UINT nFlags, CPoint point)
{
	if( m_pWndMaster->m_bLock )
		return;

	if( m_bLButtonDown && IsPush() )
	{
		if( m_pWndMaster->m_pSelectShortcut && !m_pWndMaster->m_pSelectShortcut->IsEmpty() ) 
		{
#ifdef __MAINSERVER
			if( m_pWndMaster->m_pSelectShortcut->m_dwId == 400 )
				return;
#endif //__MAINSERVER

			if( m_pWndMaster->m_pSelectShortcut->m_dwShortcut == SHORTCUT_LORDSKILL)
				m_pWndMaster->m_pSelectShortcut->m_dwId++;

			memcpy( &m_GlobalShortcut, m_pWndMaster->m_pSelectShortcut, sizeof( m_GlobalShortcut ) );
			m_GlobalShortcut.m_pFromWnd = this;
			m_GlobalShortcut.m_dwData = (DWORD)m_pWndMaster->m_pSelectShortcut;
			m_pWndMaster->m_pSelectShortcut = NULL;
		}
	}
	
	CWndNeuz::OnMouseMove( nFlags, point );
}

BOOL CWndTaskBar_HUD_EX1::OnDropIcon( LPSHORTCUT pShortcut, CPoint point )
{
	m_pWndMaster->OnDropIcon( pShortcut, m_pWndMaster->GetMousePoint() );

	return TRUE;
}

void CWndTaskBar_HUD_EX1::OnMouseWndSurface( CPoint point )
{
	LPWNDCTRL pWndSlot = NULL;
	CRect rect;
	
	for( int i = 0; i < HUD_ITEMSLOT_MAX; i++ )
	{
		pWndSlot = GetWndCtrl( m_pWndMaster->m_dwSlotIDs[ i ] );
		if( NULL == pWndSlot )
			continue;

		rect = pWndSlot->rect;

		LPSHORTCUT lpShortcut = &m_paSlotItem[ i ] ;
		if( rect.PtInRect( point) )
		{
			if( FALSE == lpShortcut->IsEmpty() )
			{
				//if( FALSE == m_pWndMaster->IsShortcut( lpShortcut, SHORTCUT_MOTION, MOT_BASE_CHEER ) )	//#24359 ������� ���������
				{
					ClientToScreen( &point );
					ClientToScreen( &rect );

					m_pWndMaster->PutTooTip( lpShortcut, point, &rect, FALSE);
					return;
				}
			}
		}
	}
}

void CWndTaskBar_HUD_EX1::SetFocus()
{
	CWndBase::SetFocus();

	UpdateWndOrder( );
}

void CWndTaskBar_HUD_EX1::OnSize( UINT nType, int cx, int cy )
{
	UpdateSlotIndex( );
	CWndNeuz::OnSize( nType, cx, cy ); 
}

BOOL CWndTaskBar_HUD_EX1::IsPickupSpace( CPoint point )
{
	if( m_pWndMaster->m_bLock )
		return FALSE;

	LPWNDCTRL pCtrl = GetWndCtrl( WIDC_STATIC1 );
	if( pCtrl )
	{
		return pCtrl->rect.PtInRect( point );
	}

	return FALSE;
}

BOOL CWndTaskBar_HUD_EX1::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase )
{
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
}

BOOL CWndTaskBar_HUD_EX1::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

BOOL CWndTaskBar_HUD_EX1::SubOnDropIcon( LPSHORTCUT pShortcut, CPoint point )
{
	if( NULL == g_pPlayer )
		return FALSE;

	CRect rect = GetWindowRect();
	rect.DeflateRect( 3, 3);
	
	// ������ ������ ��� 
	if( pShortcut->m_pFromWnd == this )
	{
		LPSHORTCUT lpShortcut = (LPSHORTCUT) pShortcut->m_dwData;
		// ����Ʈ�� �׽�ũ�ٰ� �ƴ� ���̶��. �ǵ������� ������ ������� �½�ũ�� �ۿ� ���� ����. ������.
		if( GetClientRect().PtInRect( point ) == FALSE )
		{
			// ������ ���� ������ ����
			if( lpShortcut->m_dwData == 0 )
				g_DPlay.SendRemoveAppletTaskBar( (BYTE)( lpShortcut->m_dwIndex ) );
			else
			if( lpShortcut->m_dwData == 1 )
				g_DPlay.SendRemoveItemTaskBar( m_nMyPage, (BYTE)( lpShortcut->m_dwIndex ) );
			else
			if( lpShortcut->m_dwData == 2 )
			{
				m_pWndMaster->RemoveSkillQueue( lpShortcut->m_dwIndex );
				return TRUE;
			}
			lpShortcut->Empty();
			return TRUE;
		}
	}

	CWndBase* pWndFrame =  pShortcut->m_pFromWnd->GetFrameWnd();
	if( pWndFrame && ( pWndFrame->GetWndId() != APP_INVENTORY && pWndFrame != g_WndMng.m_pWndTaskBar && pWndFrame->GetWndId() != APP_COMMUNICATION_CHAT 
					&& pWndFrame->GetWndId() != APP_SKILL4 && pWndFrame->GetWndId() != APP_MOTION && pWndFrame->GetWndId() != APP_PARTY 
					&& pWndFrame->GetWndId() != APP_LORD_SKILL && pWndFrame->GetWndId() != APP_NEWTASKBAR02 && pWndFrame->GetWndId() != APP_NEWTASKBAR03 
					&& pWndFrame->GetWndId() != APP_NEWTASKBAR04 && pWndFrame->GetWndId() != APP_SLOT ) )
	{
		SetForbid( TRUE );
		return FALSE;
	}
	
	if( pShortcut->m_dwShortcut == SHORTCUT_SKILL && pShortcut->m_dwType != 2 ) // �شܽ�ų�� �ȵ�
	{
		if( !g_pPlayer->isLearnSkill( pShortcut->m_dwId ) )
// 		LPSKILL pSkill = g_pPlayer->GetSkill( 0, pShortcut->m_dwId );
// 		if( pSkill && (pSkill->dwLevel <= 0 || g_pPlayer->CheckSkill( pSkill->dwSkill ) == FALSE) )
		{
			SetForbid( TRUE );
			return FALSE;
		}
	}

	if( pShortcut->m_dwId == 0 && strlen( pShortcut->m_szString ) < 1 && pShortcut->m_dwShortcut != SHORTCUT_SKILLFUN && pShortcut->m_dwShortcut != SHORTCUT_LORDSKILL)
	{
		SetForbid( TRUE );
		return FALSE;
	}

	// ���ο��� �̵��ǰų�, �ܺο��� ���η� 
	BOOL bForbid = TRUE;

	int nSlot = GetSlotIndexByPoint( point );
	if( -1 != nSlot && nSlot < m_nMaxViewSlot )
	{
		// ���ο��� �̵� 
		if( pShortcut->m_pFromWnd == this )
		{
			//�����༮�̸� �н�
			LPSHORTCUT lpCurShortcut = (LPSHORTCUT)pShortcut->m_dwData;
			if( &m_pWndMaster->m_aSlotItem[ m_nMyPage ][ nSlot ] == lpCurShortcut )
				return FALSE;
		}
	
		if( pShortcut->m_dwShortcut == SHORTCUT_CHAT )
		{
			CString sChat	= m_GlobalShortcut.m_szString;
			sChat.Replace( "--", "" );
			sChat.Replace( "'", "" );
			sChat.Replace( "/*", "" );
			sChat.Replace( "*/", "" );
			FLStrcpy( m_GlobalShortcut.m_szString, _countof( m_GlobalShortcut.m_szString ), (const char*)sChat );
		}
		else if( pShortcut->m_dwShortcut == SHORTCUT_SKILL )	
		{
			SkillProp* pProp;
			if( pShortcut->m_dwType == 2 )
				pProp =  prj.GetPartySkill( pShortcut->m_dwId );
			else
			{
				LPSKILL pSkill = g_pPlayer->GetSkill( 0, pShortcut->m_dwId );

				if( pSkill )
					pProp = prj.GetSkillProp( pSkill->dwSkill );	
			}
			if( pProp->dwComboStyle != CT_STEP && pProp->dwComboStyle != CT_GENERAL )	
			{
				SetForbid( TRUE );
				return FALSE;	// �����Կ� ����Ϸ��� ��ų�� ���ܱ���� �ƴѰ� ��� ����.
			}
		}
		m_pWndMaster->SetShortcut2( m_nMyPage, nSlot, pShortcut->m_dwShortcut, pShortcut->m_dwType, pShortcut->m_dwId, pShortcut->m_pTexture );
		bForbid = FALSE;
	}

//	if( pShortcut->m_pFromWnd != this )
//		SetForbid( bForbid );

	// ������ ������ ���, �Ǵ� �ȿ��� �̵����� ��� �������� �κ� ����  
	// �½�ũ�ٿ� �ִ� �������� ���� ���� ���� �κп� �巡������ ��� 
	if( pShortcut->m_pFromWnd == this ) //->������κ��� �Դٸ� ���� �����Ǿ����.
	{
		LPSHORTCUT lpShortcut = (LPSHORTCUT) pShortcut->m_dwData;
		// ������ ���� ������ ����
		if( lpShortcut->m_dwData == 0 )
			g_DPlay.SendRemoveAppletTaskBar( (BYTE)( lpShortcut->m_dwIndex ) );
		else
		if( lpShortcut->m_dwData == 1 )
			g_DPlay.SendRemoveItemTaskBar( m_nMyPage, (BYTE)( lpShortcut->m_dwIndex ) );
		else
		if( lpShortcut->m_dwData == 2 )
		{
			m_pWndMaster->RemoveSkillQueue( lpShortcut->m_dwIndex );
			//g_DPlay.SendSkillTaskBar( );
			return TRUE;
		}
		lpShortcut->Empty();
	}
	return FALSE;
}

void CWndTaskBar_HUD_EX1::OnDraw( C2DRender* p2DRender ) 
{
	CD3DFont* pOldFont = p2DRender->GetFont(); 
	p2DRender->SetFont( CWndBase::m_Theme.m_pFontWndTitle ); 

	DWORD dwCur = g_tmCurrent;

	int nOnSlot = -1;
	CPoint ptMouse = GetMousePoint();
	ClientToScreen( &ptMouse );
	if( this == IsOnWndBase2( ptMouse ) )
		nOnSlot = GetSlotIndexByPoint( GetMousePoint() );

	float fFrameUP = ( g_Neuz.m_fFPS / 60.0f  );
	if( fFrameUP > 1.0f )
		fFrameUP = 1.0f;
	else fFrameUP = 60.0f / g_Neuz.m_fFPS;

	DWORD* pSlotIds = m_pWndMaster->m_dwSlotIDs;
	CPoint point;
	LPWNDCTRL pWnd = NULL;
	for( int i = 0; i < m_nMaxViewSlot; ++i )
	{
		pWnd = GetWndCtrl( m_pWndMaster->m_dwSlotIDs[ i ] );
		if( !pWnd )
			continue;

		m_kTexSlot.Render( p2DRender, pWnd->rect.TopLeft() );

		point = pWnd->rect.TopLeft( );
		point += CPoint( 1, 1 );

		//���콺 �� ���� ǥ��
		if( nOnSlot == i )
			p2DRender->RenderTexture( point - CPoint( 4, 4 ), &m_pWndMaster->m_texOnSlot );

		LPSHORTCUT lpShortcut = &m_paSlotItem[ i ] ;
		if( !lpShortcut->IsEmpty() )
		{
			if( lpShortcut->m_pTexture )
			{
				p2DRender->RenderTexture( point, lpShortcut->m_pTexture );
			}

			//����ȿ��
			if( lpShortcut->m_nExtraFrame > 0 )
			{
				float fRate = (float)lpShortcut->m_nExtraFrame / 60.0f;
				
				p2DRender->RenderTexture( point, &m_pWndMaster->m_texUsingSlot, DWORD(255 * fRate) );
				lpShortcut->m_nExtraFrame -= (int)( fFrameUP * 4.0f );
				if( lpShortcut->m_nExtraFrame < 0 )
					lpShortcut->m_nExtraFrame = 0;
			}

			if( lpShortcut->m_dwShortcut == SHORTCUT_ITEM )
			{
				FLItemBase* pItemBase = g_pPlayer->GetItemId( lpShortcut->m_dwId );
				FLItemElem* pItemElem = (FLItemElem*)pItemBase;
				if( pItemElem  )
				{
					if( pItemElem->GetProp()->dwPackMax > 1 )
					{
						TCHAR szTemp[ 32 ];

						//sun: 8�� �۾�
						FLSPrintf( szTemp, _countof( szTemp ), "%d", g_pPlayer?g_pPlayer->m_Inventory.GetItemNumByItemId( pItemElem->m_dwItemId ): 0 );
						
						CSize size = m_p2DRender->m_pFont->GetTextExtent( szTemp );
						p2DRender->TextOut( point.x + 32 - size.cx, point.y + 32 - size.cy + 2, szTemp, 0xffffffff );
						//p2DRender->TextOut( point.x + 31 - size.cx, point.y + 31 - size.cy, szTemp, 0xffb0b0f0 );
					}
			

					DWORD dwGroup = g_pPlayer->m_cooltimeMgr.GetGroup( pItemElem->GetProp() );
					if( dwGroup )
					{
						DWORD dwEnd = g_pPlayer->m_cooltimeMgr.GetTime( dwGroup );		// �̺�Ʈ ���� �ð� 
						if( dwEnd != 0 && dwEnd > dwCur )
						{
							DWORD dwBase = g_pPlayer->m_cooltimeMgr.GetBase( dwGroup );	// �̺�Ʈ ���� �ð� 
							RenderRadar( p2DRender, point, dwCur - dwBase, dwEnd - dwBase );
						}
					}
				}
			} 
			else if( lpShortcut->m_dwShortcut == SHORTCUT_SKILL && lpShortcut->m_dwType != 2 ) //�شܽ�ų�� ��Ÿ�� ���� Render�� ���� �ʴ´�.
			{
				m_pWndMaster->RenderCollTime( point, lpShortcut->m_dwId, p2DRender );
			}
		//sun: 12, ����
			else if( lpShortcut->m_dwShortcut == SHORTCUT_LORDSKILL)
			{
				m_pWndMaster->RenderLordCollTime( point, lpShortcut->m_dwId, p2DRender );
			}
			else
			if( lpShortcut->m_dwShortcut == SHORTCUT_MOTION )
			{
				if( lpShortcut->m_dwId == MOT_BASE_ESCAPE )
				{
					PT_ITEM_SPEC pItem = g_xSpecManager->GetSpecItem( g_AddSMMode.dwSMItemID[SM_ESCAPE] );

					if( pItem && g_pPlayer )
					{
						CTimeSpan ct( g_pPlayer->m_dwSMTime[SM_ESCAPE] );
						
						if( ct.GetTotalSeconds() )
						{
							point.y -= 1;
							point.x += 1;
							RenderRadar( m_p2DRender, point, (DWORD)( pItem->dwCircleTime - ct.GetTotalSeconds() ), pItem->dwCircleTime );
						}
					}
				}
			}
		}
	}

	p2DRender->SetFont( pOldFont );

	//int nGarbage = 80;
	//int nWidth = m_rectWindow.Width();
	//int nRealWidth = nWidth - nGarbage;
	//if( nRealWidth > 36 )
	//{
	//	int nViewMax = nRealWidth / 36;

	//	LPWNDCTRL pWndSlot = NULL;
	//	for( int i = 0; i < nViewMax; ++i )
	//	{
	//		pWndSlot = GetWndCtrl( m_pWndMaster->m_dwSlotIDs[i] );
	//		if( pWndSlot )
	//		{
	//			m_kTexSlot.Render( p2DRender, pWndSlot->rect.TopLeft() );
	//		}
	//	}
	//}
}

void CWndTaskBar_HUD_EX1::CustomInit( const int nPage )
{
	// m_rectWindow�� Initialize�� �ǰ� ���Ŀ� ���õǱ⶧���� ���Լ��� ���������.
	m_bReady = TRUE;
	m_kOrgRect = m_rectWindow;

	SetPage( nPage );
}

void CWndTaskBar_HUD_EX1::SetPage( const int nPage )
{
	if( nPage < 0 || nPage >= MAX_SLOT_ITEM_COUNT )
	{
		FLASSERT( 0 );
		return;
	}

	m_nMyPage = nPage;
	m_paSlotItem = m_pWndMaster->m_aSlotItem[ m_nMyPage ];
}

void CWndTaskBar_HUD_EX1::UpdateItem()
{
	if( g_pPlayer == NULL )
		return;
	
	for( int i = 0; i < MAX_SLOT_ITEM; i++ )
	{
		LPSHORTCUT lpShortcut = &m_paSlotItem[ i ] ;
		if( lpShortcut->IsEmpty() )
			continue;

		if( lpShortcut->m_dwShortcut != SHORTCUT_ITEM )
			continue;

		FLItemBase* pItemBase = g_pPlayer->GetItemId( lpShortcut->m_dwId );
		FLItemElem* pItemElem = (FLItemElem*)pItemBase;

		if( pItemElem )
		{
			if( lpShortcut->m_pTexture != pItemElem->GetTexture() )
				lpShortcut->m_pTexture = pItemElem->GetTexture();
		}
		else
		{
			//sun: 8�� �۾�
			PT_ITEM_SPEC pItemProp	= g_xSpecManager->GetSpecItem( lpShortcut->m_dwItemId );
			if( pItemProp && pItemProp->dwPackMax > 1 )	// ���� ������ �������̸�?
			{
				DWORD dwId = g_pPlayer->m_Inventory.GetObjIdByItemId( lpShortcut->m_dwItemId );
				
				if( dwId != NULL_ID )	// ���� ������ �������� �ִٸ�?
				{
					lpShortcut->m_dwId	= dwId;
					g_DPlay.SendAddItemTaskBar( m_nMyPage, i, lpShortcut );
				}
				else
				{
					lpShortcut->Empty(); 				
					g_DPlay.SendRemoveItemTaskBar( m_nMyPage, i );
				}
			}
			else
			{
				lpShortcut->Empty(); 				
				g_DPlay.SendRemoveItemTaskBar( m_nMyPage, i );
			}

		}

	}//end of for	
}


void CWndTaskBar_HUD_EX1::UpdateMaxViewSlot( const int nMaxViewSlot )
{
	m_nMaxViewSlot = nMaxViewSlot;

	//��������� ���Լ��� ������ ���γ��̸� ����Ѵ�.
	int nWidthX = 12 + 25; // �����ִ� ����?
	nWidthX = nWidthX + ( 36 * m_nMaxViewSlot );

	if( NULL == m_pParentWnd )
		return;

	int nParentStartX = m_pParentWnd->m_rectWindow.left;
	int nParentStartY = m_pParentWnd->m_rectWindow.top;
	Move( nParentStartX - nWidthX, nParentStartY );
}

void CWndTaskBar_HUD_EX1::UpdateSlotIndex( )
{
	int nOldView = m_nMaxViewSlot;

	//!
	int nWidth = m_rectWindow.Width();
	nWidth = nWidth - 12 - 35;

	if( nWidth > 36 )
		m_nMaxViewSlot = nWidth / 36;
	else 
		m_nMaxViewSlot = -1;

	if( nOldView != m_nMaxViewSlot )		//������ ����ų� �ٶ�����.
		PLAYSND( SND_PC_SLOTDRAGEADD );		
}

int	 CWndTaskBar_HUD_EX1::GetSlotIndexByPoint( const CPoint& kPoint )
{
	LPWNDCTRL pWnd = NULL;
	for( int i = 0; i < HUD_ITEMSLOT_MAX; ++i )
	{
		pWnd = GetWndCtrl( m_pWndMaster->m_dwSlotIDs[ i ] );
		if( pWnd )
		{
			if( pWnd->rect.PtInRect( kPoint ) )
				return i;
		}
	}

	return -1;
}

LPSHORTCUT CWndTaskBar_HUD_EX1::Select( CPoint point )
{
	int nSlot = GetSlotIndexByPoint( point );
	if( -1 == nSlot || nSlot >= MAX_SLOT_ITEM )
		return NULL;

	LPSHORTCUT pShortcut = &m_paSlotItem[ nSlot ];
	if( NULL == pShortcut )
		return NULL;

	if( FALSE ==  pShortcut->IsEmpty() )
	{
		return pShortcut;
	}

	return NULL;
}

void CWndTaskBar_HUD_EX1::UpdateWndOrder( )
{
	// CWndTaskBar_HUD�� �ڽ����� ��ϵǾ�������
	// �����׷����� �ϴ� ����� ������.
	int nMyIndex = -1;
	for(int i = 0; i < m_wndOrder.GetSize(); i++)
	{
		CWndBase* pWnd = (CWndBase*)m_wndOrder.GetAt(i);
		if( this == pWnd )
		{
			nMyIndex = i;
			m_wndOrder.RemoveAt( i );
			break;
		}
	}

	if( nMyIndex != -1 )
	{
		for(int i = 0; i < m_wndOrder.GetSize(); i++)
		{
			CWndBase* pWnd = (CWndBase*)m_wndOrder.GetAt(i);
			if( m_pParentWnd == pWnd )
			{
				nMyIndex = i;
				m_wndOrder.InsertAt( i, this );
				break;
			}
		}
	}
}

void CWndTaskBar_HUD_EX1::PullMinMax( )
{
	if( m_pWndMaster->m_bLock )
		return;

	int nMaxLeftX = m_pParentWnd->m_rectWindow.left - 12;		//���ʱ����̰� ������ ��.
	int nMinLeftX = m_pWndMaster->m_rectWindow.left - m_kOrgRect.Width() + 60; // �����λ� �ʿ���� ��������
	int nRightX = m_pWndMaster->m_rectWindow.left + 35;		//35 �������� ���� ���ƺ��̴±���.

	if( m_rectWindow.left <= nMinLeftX )
		Move( nMaxLeftX, nRightX );
	else 
		Move( nMinLeftX, nRightX );
}

//----------------------------------------------------------------------------------------------------------
//CWndTaskBar_HUD_EX2
//----------------------------------------------------------------------------------------------------------
CWndTaskBar_HUD_EX2::CWndTaskBar_HUD_EX2( )
{
	m_nMyPage = -1;
	m_paSlotItem = NULL;
	m_pWndMaster = NULL;
	m_bEnableHotkey = FALSE;
}

CWndTaskBar_HUD_EX2::~CWndTaskBar_HUD_EX2( )
{
}

// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndTaskBar_HUD_EX2::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_NEWTASKBAR03, WBS_CHILDFRAME, CPoint( 0, 0 ), pWndParent );
} 

void CWndTaskBar_HUD_EX2::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
		
	// ������ �߾����� �ű�� �κ�.
	//MoveParentCenter();

	m_pWndMaster = (CWndTaskBar_HUD*)m_pParentWnd;

	CWndStatic* pWndStatic = ( CWndStatic* )GetDlgItem( WIDC_STATIC1 );
	if( pWndStatic )
		pWndStatic->AddWndStyle( WSS_NOPICKUP );

	pWndStatic = ( CWndStatic* )GetDlgItem( WIDC_STATIC2 );
	if( pWndStatic )
		pWndStatic->AddWndStyle( WSS_NOPICKUP );
} 

BOOL CWndTaskBar_HUD_EX2::Process( void )
{
	UpdateItem();
	return TRUE;
}

void CWndTaskBar_HUD_EX2::OnDestroyChildWnd( CWndBase* pWndChild )
{
}

void CWndTaskBar_HUD_EX2::OnMouseWndSurface( CPoint point )
{
	LPWNDCTRL pWndSlot = NULL;
	CRect rect;
	
	for( int i = 0; i < HUD_ITEMSLOT_MAX; i++ )
	{
		pWndSlot = GetWndCtrl( m_pWndMaster->m_dwSlotIDs[ i ] );
		if( NULL == pWndSlot )
			continue;

		rect = pWndSlot->rect;

		LPSHORTCUT lpShortcut = &m_paSlotItem[ i ] ;
		if( rect.PtInRect( point) )
		{
			if( FALSE == lpShortcut->IsEmpty() )
			{
				//if( FALSE == m_pWndMaster->IsShortcut( lpShortcut, SHORTCUT_MOTION, MOT_BASE_CHEER ) )	//#24359 ������� ���������
				{
					ClientToScreen( &point );
					ClientToScreen( &rect );

					m_pWndMaster->PutTooTip( lpShortcut, point, &rect, FALSE);
					return;
				}
			}
		}
	}
}

BOOL CWndTaskBar_HUD_EX2::OnDropIcon( LPSHORTCUT pShortcut, CPoint point )
{
	m_pWndMaster->OnDropIcon( pShortcut, m_pWndMaster->GetMousePoint() );
	return TRUE;
}

void CWndTaskBar_HUD_EX2::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_pWndMaster->m_pSelectShortcut = Select( point );

	SetCapture();
}

void CWndTaskBar_HUD_EX2::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	LPSHORTCUT pShortcut = Select( point );
	if( NULL == pShortcut )
		return;
	
	if( pShortcut == m_pWndMaster->m_pSelectShortcut && !pShortcut->IsEmpty() && pShortcut->m_dwData != 2 )
	{
		g_WndMng.ObjectExecutor( pShortcut );
	}
}

void CWndTaskBar_HUD_EX2::OnMouseMove(UINT nFlags, CPoint point)
{
	if( m_pWndMaster->m_bLock )
		return;

	if( m_bLButtonDown && IsPush() )
	{
		if( m_pWndMaster->m_pSelectShortcut && !m_pWndMaster->m_pSelectShortcut->IsEmpty() ) 
		{
#ifdef __MAINSERVER
			if( m_pWndMaster->m_pSelectShortcut->m_dwId == 400 )
				return;
#endif //__MAINSERVER

			if( m_pWndMaster->m_pSelectShortcut->m_dwShortcut == SHORTCUT_LORDSKILL)
				m_pWndMaster->m_pSelectShortcut->m_dwId++;

			memcpy( &m_GlobalShortcut, m_pWndMaster->m_pSelectShortcut, sizeof( m_GlobalShortcut ) );
			m_GlobalShortcut.m_pFromWnd = this;
			m_GlobalShortcut.m_dwData = (DWORD)m_pWndMaster->m_pSelectShortcut;
			m_pWndMaster->m_pSelectShortcut = NULL;
		}
	}
	
	CWndNeuz::OnMouseMove( nFlags, point );
}

void CWndTaskBar_HUD_EX2::OnRButtonUp(UINT nFlags, CPoint point)
{
	////������ ����
	//int nSlot = GetSlotIndexByPoint( point );
	//if( -1 == nSlot || nSlot >= MAX_SLOT_ITEM )
	//	return;
	//
	//if( FALSE == m_paSlotItem[ nSlot ].IsEmpty() )
	//{
	//	m_paSlotItem[ nSlot ].Empty();
	//	g_DPlay.SendRemoveItemTaskBar( (BYTE)( m_nMyPage ), (BYTE)( nSlot ) );	// ������ ���� ������ ����			
	//}
}

BOOL CWndTaskBar_HUD_EX2::IsPickupSpace( CPoint point )
{
	if( m_pWndMaster->m_bLock )
		return FALSE;

	LPWNDCTRL pCtrl = GetWndCtrl( WIDC_STATIC1 );
	LPWNDCTRL pCtrl2 = GetWndCtrl( WIDC_STATIC2 );
	if( pCtrl && pCtrl2 )
	{
		if( pCtrl->rect.PtInRect( point ) )
			return TRUE;

		if( pCtrl2->rect.PtInRect( point ) )
			return TRUE;
	}

	return FALSE;
}

BOOL CWndTaskBar_HUD_EX2::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase )
{
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
}

BOOL CWndTaskBar_HUD_EX2::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

BOOL CWndTaskBar_HUD_EX2::SubOnDropIcon( LPSHORTCUT pShortcut, CPoint point )
{
	if( NULL == g_pPlayer )
		return FALSE;

	CRect rect = GetWindowRect();
	rect.DeflateRect( 3, 3);
	
	// ������ ������ ��� 
	if( pShortcut->m_pFromWnd == this )
	{
		LPSHORTCUT lpShortcut = (LPSHORTCUT) pShortcut->m_dwData;
		// ����Ʈ�� �׽�ũ�ٰ� �ƴ� ���̶��. �ǵ������� ������ ������� �½�ũ�� �ۿ� ���� ����. ������.
		if( GetClientRect().PtInRect( point ) == FALSE )
		{
			// ������ ���� ������ ����
			if( lpShortcut->m_dwData == 0 )
				g_DPlay.SendRemoveAppletTaskBar( (BYTE)( lpShortcut->m_dwIndex ) );
			else
			if( lpShortcut->m_dwData == 1 )
				g_DPlay.SendRemoveItemTaskBar( m_nMyPage, (BYTE)( lpShortcut->m_dwIndex ) );
			else
			if( lpShortcut->m_dwData == 2 )
			{
				m_pWndMaster->RemoveSkillQueue( lpShortcut->m_dwIndex );
				return TRUE;
			}
			lpShortcut->Empty();
			return TRUE;
		}
	}

	CWndBase* pWndFrame =  pShortcut->m_pFromWnd->GetFrameWnd();
	if( pWndFrame && ( pWndFrame->GetWndId() != APP_INVENTORY && pWndFrame != g_WndMng.m_pWndTaskBar && pWndFrame->GetWndId() != APP_COMMUNICATION_CHAT 
					&& pWndFrame->GetWndId() != APP_SKILL4 && pWndFrame->GetWndId() != APP_MOTION && pWndFrame->GetWndId() != APP_PARTY 
					&& pWndFrame->GetWndId() != APP_LORD_SKILL && pWndFrame->GetWndId() != APP_NEWTASKBAR02 && pWndFrame->GetWndId() != APP_NEWTASKBAR03 
					&& pWndFrame->GetWndId() != APP_NEWTASKBAR04 && pWndFrame->GetWndId() != APP_SLOT ) )
	{
		SetForbid( TRUE );
		return FALSE;
	}
	
	if( pShortcut->m_dwShortcut == SHORTCUT_SKILL && pShortcut->m_dwType != 2 ) // �شܽ�ų�� �ȵ�
	{
		if( !g_pPlayer->isLearnSkill( pShortcut->m_dwId ) )
// 		LPSKILL pSkill = g_pPlayer->GetSkill( 0, pShortcut->m_dwId );
// 		if( pSkill && (pSkill->dwLevel <= 0 || g_pPlayer->CheckSkill( pSkill->dwSkill ) == FALSE) )
		{
			SetForbid( TRUE );
			return FALSE;
		}
	}

	if( pShortcut->m_dwId == 0 && strlen( pShortcut->m_szString ) < 1 && pShortcut->m_dwShortcut != SHORTCUT_SKILLFUN && pShortcut->m_dwShortcut != SHORTCUT_LORDSKILL)
	{
		SetForbid( TRUE );
		return FALSE;
	}

	// ���ο��� �̵��ǰų�, �ܺο��� ���η� 
	BOOL bForbid = TRUE;

	int nSlot = GetSlotIndexByPoint( point );
	if( -1 != nSlot )
	{
		// ���ο��� �̵� 
		if( pShortcut->m_pFromWnd == this )
		{
			//�����༮�̸� �н�
			LPSHORTCUT lpCurShortcut = (LPSHORTCUT)pShortcut->m_dwData;
			if( &m_pWndMaster->m_aSlotItem[ m_nMyPage ][ nSlot ] == lpCurShortcut )
				return FALSE;
		}

		if( pShortcut->m_dwShortcut == SHORTCUT_CHAT )
		{
			CString sChat	= m_GlobalShortcut.m_szString;
			sChat.Replace( "--", "" );
			sChat.Replace( "'", "" );
			sChat.Replace( "/*", "" );
			sChat.Replace( "*/", "" );
			FLStrcpy( m_GlobalShortcut.m_szString, _countof( m_GlobalShortcut.m_szString ), (const char*)sChat );
		}
		else if( pShortcut->m_dwShortcut == SHORTCUT_SKILL )	
		{
			SkillProp* pProp;
			if( pShortcut->m_dwType == 2 )
				pProp =  prj.GetPartySkill( pShortcut->m_dwId );
			else
			{
				LPSKILL pSkill = g_pPlayer->GetSkill( 0, pShortcut->m_dwId );

				if( pSkill )
					pProp = prj.GetSkillProp( pSkill->dwSkill );	
			}
			if( pProp->dwComboStyle != CT_STEP && pProp->dwComboStyle != CT_GENERAL )	
			{
				SetForbid( TRUE );
				return FALSE;	// �����Կ� ����Ϸ��� ��ų�� ���ܱ���� �ƴѰ� ��� ����.
			}
		}
		m_pWndMaster->SetShortcut2( m_nMyPage, nSlot, pShortcut->m_dwShortcut, pShortcut->m_dwType, pShortcut->m_dwId, pShortcut->m_pTexture );
		bForbid = FALSE;
	}

	//if( pShortcut->m_pFromWnd != this )
	//	SetForbid( bForbid );

	// ������ ������ ���, �Ǵ� �ȿ��� �̵����� ��� �������� �κ� ����  
	// �½�ũ�ٿ� �ִ� �������� ���� ���� ���� �κп� �巡������ ��� 
	if( pShortcut->m_pFromWnd == this )
	{
		LPSHORTCUT lpShortcut = (LPSHORTCUT) pShortcut->m_dwData;
		// ������ ���� ������ ����
		if( lpShortcut->m_dwData == 0 )
			g_DPlay.SendRemoveAppletTaskBar( (BYTE)( lpShortcut->m_dwIndex ) );
		else
		if( lpShortcut->m_dwData == 1 )
			g_DPlay.SendRemoveItemTaskBar( m_nMyPage, (BYTE)( lpShortcut->m_dwIndex ) );
		else
		if( lpShortcut->m_dwData == 2 )
		{
			m_pWndMaster->RemoveSkillQueue( lpShortcut->m_dwIndex );
			//g_DPlay.SendSkillTaskBar( );
			return TRUE;
		}
		lpShortcut->Empty();
	}
	return FALSE;
}

void CWndTaskBar_HUD_EX2::OnDraw( C2DRender* p2DRender ) 
{
	// ������ ������ ��� 
	if( NULL ==  g_pPlayer )
		return;

	DWORD dwCur = g_tmCurrent;
	CRect rect = GetWindowRect();

	LPWNDCTRL pWnd = NULL;
	CPoint point;
	CString strBuffer;

	CD3DFont* pOldFont = p2DRender->GetFont(); 
	p2DRender->SetFont( CWndBase::m_Theme.m_pFontWndTitle ); 

	int nOnSlot = -1;
	CPoint ptMouse = GetMousePoint();
	ClientToScreen( &ptMouse );
	if( this == IsOnWndBase2( ptMouse ) )
		nOnSlot = GetSlotIndexByPoint( GetMousePoint() );

	float fFrameUP = ( g_Neuz.m_fFPS / 60.0f  );
	if( fFrameUP > 1.0f )
		fFrameUP = 1.0f;
	else fFrameUP = 60.0f / g_Neuz.m_fFPS;

	DWORD* pSlotIds = m_pWndMaster->m_dwSlotIDs;
	for( int i = 0; i < HUD_ITEMSLOT_MAX; i++ )
	{
		pWnd = GetWndCtrl( pSlotIds[i] );
		if( NULL == pWnd )
		{
			FLASSERT( 0 );
			continue;
		}

		point = pWnd->rect.TopLeft( );

		//���콺 �� ���� ǥ��
		if( nOnSlot == i )
			p2DRender->RenderTexture( point - CPoint( 4, 4 ), &m_pWndMaster->m_texOnSlot );

		LPSHORTCUT lpShortcut = &m_paSlotItem[ i ] ;
		if( !lpShortcut->IsEmpty() )
		{
			if( lpShortcut->m_pTexture )
			{
				p2DRender->RenderTexture( point, lpShortcut->m_pTexture );
			}

			//����ȿ��
			if( lpShortcut->m_nExtraFrame > 0 )
			{
				float fRate = (float)lpShortcut->m_nExtraFrame / 60.0f;
				
				p2DRender->RenderTexture( point, &m_pWndMaster->m_texUsingSlot, DWORD(255 * fRate) );
				lpShortcut->m_nExtraFrame -= (int)( fFrameUP * 4.0f );
				if( lpShortcut->m_nExtraFrame < 0 )
					lpShortcut->m_nExtraFrame = 0;
			}

			if( lpShortcut->m_dwShortcut == SHORTCUT_ITEM )
			{
				FLItemBase* pItemBase = g_pPlayer->GetItemId( lpShortcut->m_dwId );
				FLItemElem* pItemElem = (FLItemElem*)pItemBase;
				if( pItemElem  )
				{
					if( pItemElem->GetProp()->dwPackMax > 1 )
					{
						TCHAR szTemp[ 32 ];

						//sun: 8�� �۾�
						FLSPrintf( szTemp, _countof( szTemp ), "%d", g_pPlayer?g_pPlayer->m_Inventory.GetItemNumByItemId( pItemElem->m_dwItemId ): 0 );
						
						CSize size = m_p2DRender->m_pFont->GetTextExtent( szTemp );
						p2DRender->TextOut( point.x + 32 - size.cx, point.y + 32 - size.cy + 2, szTemp, 0xffffffff );
						//p2DRender->TextOut( point.x + 31 - size.cx, point.y + 31 - size.cy, szTemp, 0xffb0b0f0 );
					}
				
					DWORD dwGroup = g_pPlayer->m_cooltimeMgr.GetGroup( pItemElem->GetProp() );
					if( dwGroup )
					{
						DWORD dwEnd = g_pPlayer->m_cooltimeMgr.GetTime( dwGroup );		// �̺�Ʈ ���� �ð� 
						if( dwEnd != 0 && dwEnd > dwCur )
						{
							DWORD dwBase = g_pPlayer->m_cooltimeMgr.GetBase( dwGroup );	// �̺�Ʈ ���� �ð� 
							RenderRadar( p2DRender, point, dwCur - dwBase, dwEnd - dwBase );
						}
					}
				}
			} 
			else if( lpShortcut->m_dwShortcut == SHORTCUT_SKILL && lpShortcut->m_dwType != 2 ) //�شܽ�ų�� ��Ÿ�� ���� Render�� ���� �ʴ´�.
			{
				m_pWndMaster->RenderCollTime( point, lpShortcut->m_dwId, p2DRender );
			}
		//sun: 12, ����
			else if( lpShortcut->m_dwShortcut == SHORTCUT_LORDSKILL)
			{
				m_pWndMaster->RenderLordCollTime( point, lpShortcut->m_dwId, p2DRender );
			}
			else
			if( lpShortcut->m_dwShortcut == SHORTCUT_MOTION )
			{
				if( lpShortcut->m_dwId == MOT_BASE_ESCAPE )
				{
					PT_ITEM_SPEC pItem = g_xSpecManager->GetSpecItem( g_AddSMMode.dwSMItemID[SM_ESCAPE] );

					if( pItem && g_pPlayer )
					{
						CTimeSpan ct( g_pPlayer->m_dwSMTime[SM_ESCAPE] );
						
						if( ct.GetTotalSeconds() )
						{
							point.y -= 1;
							point.x += 1;
							RenderRadar( m_p2DRender, point, (DWORD)( pItem->dwCircleTime - ct.GetTotalSeconds() ), pItem->dwCircleTime );
						}
					}
				}
			}
		}

		// hotkey ��� 
		if( m_bEnableHotkey )
		{
			int nKey = (i+1);
			if( 10 == nKey && m_strPrefix == "A" )
				nKey = 0;

			strBuffer.Format( "%s%d", m_strPrefix.GetString(), nKey );
			CPoint ptHotkey( point.x + 4, point.y -4 );
			p2DRender->TextOut( ptHotkey.x - 0 + 2, ptHotkey.y, 0.9f, 0.9f, strBuffer, 0xffffffff ); 
		}
	}

	p2DRender->SetFont( pOldFont ); 
}

void CWndTaskBar_HUD_EX2::SetPage( const int nPage )
{
	if( nPage < 0 || nPage > 2 )
	{
		FLASSERT( 0 );
		return;
	}

	m_nMyPage = nPage;
	m_paSlotItem = m_pWndMaster->m_aSlotItem[ m_nMyPage ];
}

int CWndTaskBar_HUD_EX2::GetSlotIndexByPoint( const CPoint& kPoint )
{
	LPWNDCTRL pWnd = NULL;
	for( int i = 0; i < HUD_ITEMSLOT_MAX; ++i )
	{
		pWnd = GetWndCtrl( m_pWndMaster->m_dwSlotIDs[ i ] );
		if( pWnd )
		{
			if( pWnd->rect.PtInRect( kPoint ) )
				return i;
		}
	}

	return -1;
}

LPSHORTCUT CWndTaskBar_HUD_EX2::Select( CPoint point )
{
	int nSlot = GetSlotIndexByPoint( point );
	if( -1 == nSlot || nSlot >= MAX_SLOT_ITEM )
		return NULL;

	LPSHORTCUT pShortcut = &m_paSlotItem[ nSlot ];
	if( NULL == pShortcut )
		return NULL;

	if( FALSE ==  pShortcut->IsEmpty() )
	{
		return pShortcut;
	}

	return NULL;
}

void CWndTaskBar_HUD_EX2::UpdateItem()
{
	if( g_pPlayer == NULL )
		return;
	
	for( int i = 0; i < MAX_SLOT_ITEM; i++ )
	{
		LPSHORTCUT lpShortcut = &m_paSlotItem[ i ] ;
		if( lpShortcut->IsEmpty() )
			continue;

		if( lpShortcut->m_dwShortcut != SHORTCUT_ITEM )
			continue;

		FLItemBase* pItemBase = g_pPlayer->GetItemId( lpShortcut->m_dwId );
		FLItemElem* pItemElem = (FLItemElem*)pItemBase;

		if( pItemElem )
		{
			if( lpShortcut->m_pTexture != pItemElem->GetTexture() )
				lpShortcut->m_pTexture = pItemElem->GetTexture();
		}
		else
		{
			//sun: 8�� �۾�
			PT_ITEM_SPEC pItemProp	= g_xSpecManager->GetSpecItem( lpShortcut->m_dwItemId );
			if( pItemProp && pItemProp->dwPackMax > 1 )	// ���� ������ �������̸�?
			{
				DWORD dwId = g_pPlayer->m_Inventory.GetObjIdByItemId( lpShortcut->m_dwItemId );
				
				if( dwId != NULL_ID )	// ���� ������ �������� �ִٸ�?
				{
					lpShortcut->m_dwId	= dwId;
					g_DPlay.SendAddItemTaskBar( m_nMyPage, i, lpShortcut );
				}
				else
				{
					lpShortcut->Empty(); 				
					g_DPlay.SendRemoveItemTaskBar( m_nMyPage, i );
				}
			}
			else
			{
				lpShortcut->Empty(); 				
				g_DPlay.SendRemoveItemTaskBar( m_nMyPage, i );
			}

		}

	}//end of for	
}

void CWndTaskBar_HUD_EX2::SetHotKeyPrefix( BOOL bEnable, const char* szPrefix )
{
	m_bEnableHotkey = bEnable;
	m_strPrefix = CString( szPrefix );
}

//----------------------------------------------------------------------------------------------------------
//CWndTaskBar_HUD_EX3
//----------------------------------------------------------------------------------------------------------
CWndTaskBar_HUD_EX3::CWndTaskBar_HUD_EX3( )
{
	m_nMyPage = -1;
	m_paSlotItem = NULL;
	m_pWndMaster = NULL;
}

CWndTaskBar_HUD_EX3::~CWndTaskBar_HUD_EX3( )
{
}

// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndTaskBar_HUD_EX3::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_NEWTASKBAR04, 0, CPoint( 0, 0 ), pWndParent );
} 

void CWndTaskBar_HUD_EX3::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
		
	// ������ �߾����� �ű�� �κ�.
	//MoveParentCenter();
	
	m_pWndMaster = (CWndTaskBar_HUD*)m_pParentWnd;

	CWndStatic* pWndStatic = ( CWndStatic* )GetDlgItem( WIDC_STATIC1 );
	if( pWndStatic )
		pWndStatic->AddWndStyle( WSS_NOPICKUP );

	pWndStatic = ( CWndStatic* )GetDlgItem( WIDC_STATIC2 );
	if( pWndStatic )
		pWndStatic->AddWndStyle( WSS_NOPICKUP );
} 

BOOL CWndTaskBar_HUD_EX3::Process( void )
{
	UpdateItem();

	return TRUE;
}

void CWndTaskBar_HUD_EX3::OnDestroyChildWnd( CWndBase* pWndChild )
{
}

void CWndTaskBar_HUD_EX3::OnMouseWndSurface( CPoint point )
{
	LPWNDCTRL pWndSlot = NULL;
	CRect rect;
	
	for( int i = 0; i < HUD_ITEMSLOT_MAX; i++ )
	{
		pWndSlot = GetWndCtrl( m_pWndMaster->m_dwSlotIDs[ i ] );
		if( NULL == pWndSlot )
			continue;

		rect = pWndSlot->rect;

		LPSHORTCUT lpShortcut = &m_paSlotItem[ i ] ;
		if( rect.PtInRect( point) )
		{
			if( FALSE == lpShortcut->IsEmpty() )
			{
			//	if( FALSE == m_pWndMaster->IsShortcut( lpShortcut, SHORTCUT_MOTION, MOT_BASE_CHEER ) )	//#24359 ������� ���������
				{
					ClientToScreen( &point );
					ClientToScreen( &rect );
					m_pWndMaster->PutTooTip( lpShortcut, point, &rect, FALSE );
					return;
				}
			}
		}
	}
}

BOOL CWndTaskBar_HUD_EX3::OnDropIcon( LPSHORTCUT pShortcut, CPoint point )
{
	m_pWndMaster->OnDropIcon( pShortcut, m_pWndMaster->GetMousePoint() );
	return TRUE;
}

void CWndTaskBar_HUD_EX3::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_pWndMaster->m_pSelectShortcut = Select( point );

	SetCapture();
}

void CWndTaskBar_HUD_EX3::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	LPSHORTCUT pShortcut = Select( point );
	if( NULL == pShortcut )
		return;
	
	if( pShortcut == m_pWndMaster->m_pSelectShortcut && !pShortcut->IsEmpty() && pShortcut->m_dwData != 2 )
	{
		g_WndMng.ObjectExecutor( pShortcut );
	}
}

void CWndTaskBar_HUD_EX3::OnMouseMove(UINT nFlags, CPoint point)
{
	if( m_pWndMaster->m_bLock )
		return;

	if( m_bLButtonDown && IsPush() )
	{
		if( m_pWndMaster->m_pSelectShortcut && !m_pWndMaster->m_pSelectShortcut->IsEmpty() ) 
		{
#ifdef __MAINSERVER
			if( m_pWndMaster->m_pSelectShortcut->m_dwId == 400 )
				return;
#endif //__MAINSERVER

			if( m_pWndMaster->m_pSelectShortcut->m_dwShortcut == SHORTCUT_LORDSKILL)
				m_pWndMaster->m_pSelectShortcut->m_dwId++;

			memcpy( &m_GlobalShortcut, m_pWndMaster->m_pSelectShortcut, sizeof( m_GlobalShortcut ) );
			m_GlobalShortcut.m_pFromWnd = this;
			m_GlobalShortcut.m_dwData = (DWORD)m_pWndMaster->m_pSelectShortcut;
			m_pWndMaster->m_pSelectShortcut = NULL;
		}
	}
	
	CWndNeuz::OnMouseMove( nFlags, point );
}

BOOL CWndTaskBar_HUD_EX3::IsPickupSpace( CPoint point )
{
	if( m_pWndMaster->m_bLock )
		return FALSE;

	LPWNDCTRL pCtrl = GetWndCtrl( WIDC_STATIC1 );
	LPWNDCTRL pCtrl2 = GetWndCtrl( WIDC_STATIC2 );
	if( pCtrl && pCtrl2 )
	{
		if( pCtrl->rect.PtInRect( point ) )
			return TRUE;

		if( pCtrl2->rect.PtInRect( point ) )
			return TRUE;
	}

	return FALSE;
}

BOOL CWndTaskBar_HUD_EX3::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase )
{
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
}

BOOL CWndTaskBar_HUD_EX3::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

BOOL CWndTaskBar_HUD_EX3::SubOnDropIcon( LPSHORTCUT pShortcut, CPoint point )
{
	if( NULL ==  g_pPlayer )
		return FALSE;

	CRect rect = GetWindowRect();
	rect.DeflateRect( 3, 3);
	
	// ������ ������ ��� 
	if( pShortcut->m_pFromWnd == this )
	{
		LPSHORTCUT lpShortcut = (LPSHORTCUT) pShortcut->m_dwData;
		// ����Ʈ�� �׽�ũ�ٰ� �ƴ� ���̶��. �ǵ������� ������ ������� �½�ũ�� �ۿ� ���� ����. ������.
		if( GetClientRect().PtInRect( point ) == FALSE )
		{
			// ������ ���� ������ ����
			if( lpShortcut->m_dwData == 0 )
				g_DPlay.SendRemoveAppletTaskBar( (BYTE)( lpShortcut->m_dwIndex ) );
			else
			if( lpShortcut->m_dwData == 1 )
				g_DPlay.SendRemoveItemTaskBar( m_nMyPage, (BYTE)( lpShortcut->m_dwIndex ) );
			else
			if( lpShortcut->m_dwData == 2 )
			{
				m_pWndMaster->RemoveSkillQueue( lpShortcut->m_dwIndex );
				return TRUE;
			}
			lpShortcut->Empty();
			return TRUE;
		}
	}

	CWndBase* pWndFrame =  pShortcut->m_pFromWnd->GetFrameWnd();
	if( pWndFrame && ( pWndFrame->GetWndId() != APP_INVENTORY && pWndFrame != g_WndMng.m_pWndTaskBar && pWndFrame->GetWndId() != APP_COMMUNICATION_CHAT 
					&& pWndFrame->GetWndId() != APP_SKILL4 && pWndFrame->GetWndId() != APP_MOTION && pWndFrame->GetWndId() != APP_PARTY 
					&& pWndFrame->GetWndId() != APP_LORD_SKILL && pWndFrame->GetWndId() != APP_NEWTASKBAR02 && pWndFrame->GetWndId() != APP_NEWTASKBAR03 
					&& pWndFrame->GetWndId() != APP_NEWTASKBAR04 && pWndFrame->GetWndId() != APP_SLOT ) )
	{
		SetForbid( TRUE );
		return FALSE;
	}
	
	if( pShortcut->m_dwShortcut == SHORTCUT_SKILL && pShortcut->m_dwType != 2 ) // �شܽ�ų�� �ȵ�
	{
		if( !g_pPlayer->isLearnSkill( pShortcut->m_dwId ) )
// 		LPSKILL pSkill = g_pPlayer->GetSkill( 0, pShortcut->m_dwId );
// 		if( pSkill && (pSkill->dwLevel <= 0 || g_pPlayer->CheckSkill( pSkill->dwSkill ) == FALSE) )
		{
			SetForbid( TRUE );
			return FALSE;
		}
	}

	if( pShortcut->m_dwId == 0 && strlen( pShortcut->m_szString ) < 1 && pShortcut->m_dwShortcut != SHORTCUT_SKILLFUN && pShortcut->m_dwShortcut != SHORTCUT_LORDSKILL)
	{
		SetForbid( TRUE );
		return FALSE;
	}

	// ���ο��� �̵��ǰų�, �ܺο��� ���η� 
	BOOL bForbid = TRUE;

	int nSlot = GetSlotIndexByPoint( point );
	if( -1 != nSlot )
	{
		// ���ο��� �̵� 
		if( pShortcut->m_pFromWnd == this )
		{
			//�����༮�̸� �н�
			LPSHORTCUT lpCurShortcut = (LPSHORTCUT)pShortcut->m_dwData;
			if( &m_pWndMaster->m_aSlotItem[ m_nMyPage ][ nSlot ] == lpCurShortcut )
				return FALSE;
		}

		if( pShortcut->m_dwShortcut == SHORTCUT_CHAT )
		{
			CString sChat	= m_GlobalShortcut.m_szString;
			sChat.Replace( "--", "" );
			sChat.Replace( "'", "" );
			sChat.Replace( "/*", "" );
			sChat.Replace( "*/", "" );
			FLStrcpy( m_GlobalShortcut.m_szString, _countof( m_GlobalShortcut.m_szString ), (const char*)sChat );
		}
		else if( pShortcut->m_dwShortcut == SHORTCUT_SKILL )	
		{
			SkillProp* pProp;
			if( pShortcut->m_dwType == 2 )
				pProp =  prj.GetPartySkill( pShortcut->m_dwId );
			else
			{
				LPSKILL pSkill = g_pPlayer->GetSkill( 0, pShortcut->m_dwId );
				
				if( pSkill )
					pProp = prj.GetSkillProp( pSkill->dwSkill );	
			}
			if( pProp->dwComboStyle != CT_STEP && pProp->dwComboStyle != CT_GENERAL )	
			{
				SetForbid( TRUE );
				return FALSE;	// �����Կ� ����Ϸ��� ��ų�� ���ܱ���� �ƴѰ� ��� ����.
			}
		}
		m_pWndMaster->SetShortcut2( m_nMyPage, nSlot, pShortcut->m_dwShortcut, pShortcut->m_dwType, pShortcut->m_dwId, pShortcut->m_pTexture );
		bForbid = FALSE;
	}

//	if( pShortcut->m_pFromWnd != this )
//		SetForbid( bForbid );

	// ������ ������ ���, �Ǵ� �ȿ��� �̵����� ��� �������� �κ� ����  
	// �½�ũ�ٿ� �ִ� �������� ���� ���� ���� �κп� �巡������ ��� 
	if( pShortcut->m_pFromWnd == this )
	{
		LPSHORTCUT lpShortcut = (LPSHORTCUT) pShortcut->m_dwData;
		// ������ ���� ������ ����
		if( lpShortcut->m_dwData == 0 )
			g_DPlay.SendRemoveAppletTaskBar( (BYTE)( lpShortcut->m_dwIndex ) );
		else
		if( lpShortcut->m_dwData == 1 )
			g_DPlay.SendRemoveItemTaskBar( m_nMyPage, (BYTE)( lpShortcut->m_dwIndex ) );
		else
		if( lpShortcut->m_dwData == 2 )
		{
			m_pWndMaster->RemoveSkillQueue( lpShortcut->m_dwIndex );
			//g_DPlay.SendSkillTaskBar( );
			return TRUE;
		}
		lpShortcut->Empty();
	}
	return FALSE;	
}

void CWndTaskBar_HUD_EX3::OnDraw( C2DRender* p2DRender ) 
{
	if( NULL == g_pPlayer )
		return;

	// ������ ������ ��� 
	DWORD dwCur = g_tmCurrent;
	CRect rect = GetWindowRect();

	LPWNDCTRL pWnd = NULL;
	CPoint point;
	CString strBuffer;

	CD3DFont* pOldFont = p2DRender->GetFont(); 
	p2DRender->SetFont( CWndBase::m_Theme.m_pFontWndTitle ); 

	int nOnSlot = -1;
	CPoint ptMouse = GetMousePoint();
	ClientToScreen( &ptMouse );
	if( this == IsOnWndBase2( ptMouse ) )
		nOnSlot = GetSlotIndexByPoint( GetMousePoint() );
	
	DWORD* pSlotIds = m_pWndMaster->m_dwSlotIDs;

	float fFrameUP = ( g_Neuz.m_fFPS / 60.0f  );
	if( fFrameUP > 1.0f )
		fFrameUP = 1.0f;
	else fFrameUP = 60.0f / g_Neuz.m_fFPS;

	for( int i = 0; i < HUD_ITEMSLOT_MAX; i++ )
	{
		pWnd = GetWndCtrl( pSlotIds[i] );
		if( NULL == pWnd )
		{
			FLASSERT( 0 );
			continue;
		}

		point = pWnd->rect.TopLeft( );

		//���콺 �� ���� ǥ��
		if( nOnSlot == i )
			p2DRender->RenderTexture( point - CPoint( 4, 4 ), &m_pWndMaster->m_texOnSlot );

		LPSHORTCUT lpShortcut = &m_paSlotItem[ i ] ;
		if( !lpShortcut->IsEmpty() )
		{
			if( lpShortcut->m_pTexture )
			{
				p2DRender->RenderTexture( point, lpShortcut->m_pTexture );
			}

			//����ȿ��
			if( lpShortcut->m_nExtraFrame > 0 )
			{
				float fRate = (float)lpShortcut->m_nExtraFrame / 60.0f;
				
				p2DRender->RenderTexture( point, &m_pWndMaster->m_texUsingSlot, DWORD(255 * fRate) );
				lpShortcut->m_nExtraFrame -= (int)( fFrameUP * 4.0f );
				if( lpShortcut->m_nExtraFrame < 0 )
					lpShortcut->m_nExtraFrame = 0;
			}

			if( lpShortcut->m_dwShortcut == SHORTCUT_ITEM )
			{
				FLItemBase* pItemBase = g_pPlayer->GetItemId( lpShortcut->m_dwId );
				FLItemElem* pItemElem = (FLItemElem*)pItemBase;
				if( pItemElem  )
				{
					if( pItemElem->GetProp()->dwPackMax > 1 )
					{
						TCHAR szTemp[ 32 ];

						//sun: 8�� �۾�
						FLSPrintf( szTemp, _countof( szTemp ), "%d", g_pPlayer?g_pPlayer->m_Inventory.GetItemNumByItemId( pItemElem->m_dwItemId ): 0 );
						
						CSize size = m_p2DRender->m_pFont->GetTextExtent( szTemp );
						p2DRender->TextOut( point.x + 32 - size.cx, point.y + 32 - size.cy + 2, szTemp, 0xffffffff );
					//	p2DRender->TextOut( point.x + 31 - size.cx, point.y + 31 - size.cy, szTemp, 0xffb0b0f0 );
					}
			
					DWORD dwGroup = g_pPlayer->m_cooltimeMgr.GetGroup( pItemElem->GetProp() );
					if( dwGroup )
					{
						DWORD dwEnd = g_pPlayer->m_cooltimeMgr.GetTime( dwGroup );		// �̺�Ʈ ���� �ð� 
						if( dwEnd != 0 && dwEnd > dwCur )
						{
							DWORD dwBase = g_pPlayer->m_cooltimeMgr.GetBase( dwGroup );	// �̺�Ʈ ���� �ð� 
							RenderRadar( p2DRender, point, dwCur - dwBase, dwEnd - dwBase );
						}
					}
				}
			} 
			else if( lpShortcut->m_dwShortcut == SHORTCUT_SKILL && lpShortcut->m_dwType != 2 ) //�شܽ�ų�� ��Ÿ�� ���� Render�� ���� �ʴ´�.
			{
				m_pWndMaster->RenderCollTime( point, lpShortcut->m_dwId, p2DRender );
			}
		//sun: 12, ����
			else if( lpShortcut->m_dwShortcut == SHORTCUT_LORDSKILL)
			{
				m_pWndMaster->RenderLordCollTime( point, lpShortcut->m_dwId, p2DRender );
			}
			else
			if( lpShortcut->m_dwShortcut == SHORTCUT_MOTION )
			{
				if( lpShortcut->m_dwId == MOT_BASE_ESCAPE )
				{
					PT_ITEM_SPEC pItem = g_xSpecManager->GetSpecItem( g_AddSMMode.dwSMItemID[SM_ESCAPE] );

					if( pItem && g_pPlayer )
					{
						CTimeSpan ct( g_pPlayer->m_dwSMTime[SM_ESCAPE] );
						
						if( ct.GetTotalSeconds() )
						{
							point.y -= 1;
							point.x += 1;
							RenderRadar( m_p2DRender, point, (DWORD)( pItem->dwCircleTime - ct.GetTotalSeconds() ), pItem->dwCircleTime );
						}
					}
				}
			}

			// hotkey ��� 
//			strBuffer.Format( "f%d", (i+1) );
//			CPoint ptHotkey( point.x + 8, point.y );
//			p2DRender->TextOut( ptHotkey.x - 0 + 2, ptHotkey.y, strBuffer, 0xffffffff ); 

		}
	}

	p2DRender->SetFont( pOldFont ); 
}

void CWndTaskBar_HUD_EX3::SetPage( const int nPage )
{
	if( nPage < 0 || nPage >= MAX_SLOT_ITEM_COUNT )
	{
		FLASSERT( 0 );
		return;
	}

	m_nMyPage = nPage;
	m_paSlotItem = m_pWndMaster->m_aSlotItem[ m_nMyPage ];
}

int CWndTaskBar_HUD_EX3::GetSlotIndexByPoint( const CPoint& kPoint )
{
	LPWNDCTRL pWnd = NULL;
	for( int i = 0; i < HUD_ITEMSLOT_MAX; ++i )
	{
		pWnd = GetWndCtrl( m_pWndMaster->m_dwSlotIDs[ i ] );
		if( pWnd )
		{
			if( pWnd->rect.PtInRect( kPoint ) )
				return i;
		}
	}

	return -1;
}

LPSHORTCUT CWndTaskBar_HUD_EX3::Select( CPoint point )
{
	int nSlot = GetSlotIndexByPoint( point );
	if( -1 == nSlot || nSlot >= MAX_SLOT_ITEM )
		return NULL;

	LPSHORTCUT pShortcut = &m_paSlotItem[ nSlot ];
	if( NULL == pShortcut )
		return NULL;

	if( FALSE ==  pShortcut->IsEmpty() )
	{
		return pShortcut;
	}

	return NULL;
}

void CWndTaskBar_HUD_EX3::UpdateItem()
{
	if( g_pPlayer == NULL )
		return;
	
	for( int i = 0; i < MAX_SLOT_ITEM; i++ )
	{
		LPSHORTCUT lpShortcut = &m_paSlotItem[ i ] ;
		if( lpShortcut->IsEmpty() )
			continue;

		if( lpShortcut->m_dwShortcut != SHORTCUT_ITEM )
			continue;

		FLItemBase* pItemBase = g_pPlayer->GetItemId( lpShortcut->m_dwId );
		FLItemElem* pItemElem = (FLItemElem*)pItemBase;

		if( pItemElem )
		{
			if( lpShortcut->m_pTexture != pItemElem->GetTexture() )
				lpShortcut->m_pTexture = pItemElem->GetTexture();
		}
		else
		{
			//sun: 8�� �۾�
			PT_ITEM_SPEC pItemProp	= g_xSpecManager->GetSpecItem( lpShortcut->m_dwItemId );
			if( pItemProp && pItemProp->dwPackMax > 1 )	// ���� ������ �������̸�?
			{
				DWORD dwId = g_pPlayer->m_Inventory.GetObjIdByItemId( lpShortcut->m_dwItemId );
				
				if( dwId != NULL_ID )	// ���� ������ �������� �ִٸ�?
				{
					lpShortcut->m_dwId	= dwId;
					g_DPlay.SendAddItemTaskBar( m_nMyPage, i, lpShortcut );
				}
				else
				{
					lpShortcut->Empty(); 				
					g_DPlay.SendRemoveItemTaskBar( m_nMyPage, i );
				}
			}
			else
			{
				lpShortcut->Empty(); 				
				g_DPlay.SendRemoveItemTaskBar( m_nMyPage, i );
			}

		}

	}//end of for	
}