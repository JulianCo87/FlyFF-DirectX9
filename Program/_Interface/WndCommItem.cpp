#include "stdafx.h"
#include "WndCommItem.h"


/****************************************************
  WndId : APP_COMM_ITEM - ��� ITEM ��������
  CtrlId : WIDC_TABCTRL1 - 
****************************************************/

CWndCommItem::CWndCommItem() 
{ 
} 
CWndCommItem::~CWndCommItem() 
{ 
} 
void CWndCommItem::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndCommItem::OnInitialUpdate() 
{
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���

	CWndTabCtrl* pWndTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
	CRect rect = GetClientRect();
	rect.left = 5;
	rect.top = 0;
	m_wndCommItemCtrl.Create( WBS_CHILD | WBS_NODRAWFRAME, rect, pWndTabCtrl, APP_COMM_ITEM );


	WTCITEM tabTabItem;
	
	tabTabItem.mask = WTCIF_TEXT | WTCIF_PARAM;
	tabTabItem.pszText = prj.GetText( TID_TOOLTIP_ITEMTIME );
	tabTabItem.pWndBase = &m_wndCommItemCtrl;
	pWndTabCtrl->InsertItem( 0, &tabTabItem );

	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndCommItem::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_COMM_ITEM, 0, CPoint( 0, 0 ), pWndParent );
} 

BOOL CWndCommItem::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndCommItem::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndCommItem::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndCommItem::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndCommItem::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 






///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CWndPartyCtrl
////////////////////////////////////////////////////////////////////////////////////////////////////////////

CWndCommItemCtrl::CWndCommItemCtrl() 
{
	m_pVBGauge = NULL;
	pWndWorld = NULL;
	memset( m_dwDraw, 0, sizeof( m_dwDraw ) );
	m_nMaxDraw = 0;
}
CWndCommItemCtrl::~CWndCommItemCtrl()
{
	DeleteDeviceObjects();
}

void CWndCommItemCtrl::Create( DWORD dwListCtrlStyle, RECT& rect, CWndBase* pParentWnd, UINT nID )
{
	m_dwListCtrlStyle = dwListCtrlStyle;
	CWndBase::Create( WBS_CHILD, rect, pParentWnd, nID );
}

void CWndCommItemCtrl::LoadListBoxScript(LPCTSTR lpFileName) 
{
}	
void CWndCommItemCtrl::InterpriteScript(CScanner& scanner,CPtrArray& ptrArray) 
{
}
HRESULT CWndCommItemCtrl::RestoreDeviceObjects()
{
	CWndBase::RestoreDeviceObjects();
	if( m_pVBGauge == NULL )
		return m_pApp->m_pd3dDevice->CreateVertexBuffer( sizeof( TEXTUREVERTEX2 ) * 3 * 6, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_TEXTUREVERTEX2, D3DPOOL_DEFAULT, &m_pVBGauge, NULL );
	return S_OK;
}
HRESULT CWndCommItemCtrl::InvalidateDeviceObjects()
{
	CWndBase::InvalidateDeviceObjects();
    SAFE_RELEASE( m_pVBGauge );
	return S_OK;
}
HRESULT CWndCommItemCtrl::DeleteDeviceObjects()
{
	CWndBase::DeleteDeviceObjects();
	SAFE_RELEASE( m_pVBGauge );
	return S_OK;
}
void CWndCommItemCtrl::OnInitialUpdate()
{
	CRect rect = GetWindowRect();
	m_wndScrollBar.AddWndStyle( WBS_DOCKING );
	m_wndScrollBar.Create( WBS_VERT, rect, this, 1000 );//,m_pSprPack,-1);
	m_nFontHeight = 30;

	pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );

	RestoreDeviceObjects();
	m_texGauEmptyNormal.LoadTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(),"GauEmptySmall.bmp" ), 0xffff00ff, TRUE );
	m_texGauFillNormal.LoadTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(),"GauFillSmall.bmp" ), 0xffff00ff, TRUE );
}
void CWndCommItemCtrl::OnMouseWndSurface( CPoint point )
{
	PFONTCOLOR_WNDCOMMITEMCTRL pFontColorWndCommItemCtrl( g_WndFontColorManager->GetWndCommItemCtrl() );

	CPoint pt( 3, 3 );
	
	int nCountinueCount = 0;
	for( int i = 0; i < m_nMaxDraw; i++ ) 
	{
		int x = 0, nWidth = m_rectClient.Width();// - 1;
		CRect rectHittest = CRect( x + 3, pt.y, x + 3 + nWidth, pt.y + 32 );// pt.x, pt.y+ 12, pt.x + m_rectWindow.Width() - m_wndScrollBar.GetClientRect().Width(), pt.y + m_nFontHeight+ 12 );

		if( rectHittest.PtInRect( point ) )
		{
			ClientToScreen( &point );
			ClientToScreen( &rectHittest );

			PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( m_dwDraw[i] );
			if( pItemProp == NULL )
				continue;

			CEditString		str( _T("") );
			CString			strTemp( _T("") );
			CString			strEnter( _T('\n') );

			str.AddString( pItemProp->szName, pFontColorWndCommItemCtrl->m_stItemName.GetFontColor(), ESSTY_BOLD );
			str += strEnter;

			// ������ �뵵�� �Ѹ���� ��ȹ�� ���ǵ�
//			if( pItemProp->dwID == ITEM_INDEX( 10430, II_SYS_SYS_SCR_BLESSING ) )
//			{
//				strTemp.Format( prj.GetText( TID_GAME_COND_USE ) );	// ���ӽð� : 
//				str += strTemp;
//			}
//			else if( pItemProp->dwID == ITEM_INDEX( 10464, II_SYS_SYS_SCR_SMELPROT ) || pItemProp->dwID == ITEM_INDEX( 10468, II_SYS_SYS_SCR_SMELTING )
//				|| pItemProp->dwID == ITEM_INDEX( 26203, II_SYS_SYS_SCR_SUPERSMELTING )
//				|| pItemProp->dwID == ITEM_INDEX( 10488, II_SYS_SYS_SCR_SMELPROT3 )		//sun: 9,10�� ���� __ULTIMATE
////sun: 11, ä�� �ý���
//				|| pItemProp->dwID == ITEM_INDEX( 26473, II_SYS_SYS_SCR_SMELPROT4 )
//				|| pItemProp->dwID == ITEM_INDEX( 10489, II_SYS_SYS_SCR_SMELTING2 )
//			)
//			{
//				strTemp.Format( "%s", prj.GetText( TID_GAME_DEMOL_USE ) );
//				str += strTemp;
//			}
//
//			//sun: 8�� ���� ��ȯ Neuz, World, Trans
//			else if( pItemProp->dwItemKind3 == IK3_ANGEL_BUFF )
//			{
//				strTemp.Format( "%s", prj.GetText( TID_GAME_TIP_ANGEL_PLAIN ) );
//				str += strTemp;
//			}
//			else if( g_xSpecManager->IsChargeZoneTicket( pItemProp->dwID ) != false )
//			{
//				FLItemElem* pTicket	= NULL;
//
//				for( DWORD dwObjID = 0; dwObjID < g_pPlayer->m_Inventory.GetMax(); ++dwObjID )
//				{
//					pTicket	= g_pPlayer->m_Inventory.GetAtId( dwObjID );
//					if( pTicket != NULL
//						&& pTicket->m_dwItemId == pItemProp->dwID
//						&& pTicket->IsFlag( FLItemElem::expired ) == FALSE
//						&& pTicket->m_dwKeepTime > 0
//						)
//					{
//						break;
//					}
//					else
//					{
//						pTicket	= NULL;
//					}
//				}
//				
//				CString str1, str2;
//				if( pTicket )
//				{
//					time_t t	= pTicket->m_dwKeepTime - time_null();
//					if( t > 0 )
//					{
//						CTimeSpan time( t );
////sun: 11, �Ⱓ�� ������ ���ӽð� �ʴ��� ǥ��
//						str1.Format( prj.GetText( TID_TOOLTIP_DATE ), static_cast<int>(time.GetDays()), time.GetHours(), time.GetMinutes(), time.GetSeconds() );
//					}
//
//					str2	= str1 + prj.GetText( TID_TOOLTIP_PERIOD );
//					str		+= str2;
//				}
//				else
//				{
//					//str		= "";
//				}
//			}
////sun: 9�� ���°��� Clienet
//			else if( pItemProp->dwID == ITEM_INDEX( 21035, II_SYS_SYS_SCR_PET_FEED_POCKET ) )
//			{
//				//�˻��ؼ� Ȱ��ȭ �� ���� �ָӴϸ� ã�´�.
//				FLItemElem* ptr;
//				FLItemElem* pItemElem = NULL;
//
//				int nMax = g_pPlayer->m_Inventory.GetMax();
//				for( int i = 0 ; i < nMax; i++ )
//				{
//					ptr	= g_pPlayer->m_Inventory.GetAtId( i );
//					if( IsUsableItem( ptr ) && ptr->m_dwItemId == ITEM_INDEX( 21035, II_SYS_SYS_SCR_PET_FEED_POCKET ) &&
//						ptr->m_dwKeepTime > 0 && !ptr->IsFlag( FLItemElem::expired ) )	// Ȱ��ȭ�� ���� �ָӴ��� ���
//					{						
//						pItemElem = ptr;
//						i = nMax;
//					}
//				}
//				if(pItemElem != NULL)
//				{
//					CString strTemp;
//					//��� ���� ����
//					time_t t = pItemElem->m_dwKeepTime - time_null();
////					if( pItemElem->m_dwKeepTime && !pItemElem->IsFlag( FLItemElem::expired ) )
//					{
//						if( t > 0 )
//						{
//							CTimeSpan ct( t );
//
//							if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
//							{
//								strTemp.Format( prj.GetText( TID_TOOLTIP_ITEMTIME ) );	// ���ӽð� : 
//								str += strTemp;
//								strTemp.Format( " : " );
//								str += strTemp;
//							}
//							
//							strTemp.Format( prj.GetText( TID_TOOLTIP_DATE ), static_cast<int>(ct.GetDays()), ct.GetHours(), ct.GetMinutes(), ct.GetSeconds() );	// ���ӽð� : 
//							str += strTemp;
//						}
//					}
//				}
//			}
////sun: 11, ä�� �ý���
//			else if( pItemProp->IsAbsoluteTime() == TRUE )
//			{
//				IBuff* pBuff	= g_pPlayer->m_buffs.GetBuff( BUFF_ITEM, (WORD)( pItemProp->dwID ) );
//				time_t				t = 0;
//
//				if( !pBuff )
//				{
//					t = 0;
//				}
//				else
//				{
//					t = (time_t)pBuff->GetLevel() - time_null();
//					if( t < 0 )  t = 0;
//				}
//				
//				CTimeSpan ts( t );
//
//				if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
//				{
//					strTemp.Format( prj.GetText( TID_TOOLTIP_ITEMTIME ) );	// ���ӽð� : 
//					str += strTemp;
//					strTemp.Format( " : " );
//					str += strTemp;
//				}
//				
//				strTemp.Format( prj.GetText( TID_TOOLTIP_DATE ), static_cast<int>(ts.GetDays()), ts.GetHours(), ts.GetMinutes(), ts.GetSeconds() );	// ���ӽð� : 
//				str += strTemp;
//			}
//			else
//			{
//				//TODO_�̼۴�
//				long lData;
//
//				if( pItemProp->dwCircleTime != NULL_ID )
//				{
//					lData = pItemProp->dwCircleTime;
//				}
//				else if( pItemProp->dwSkillTime != NULL_ID )
//				{
//					lData = (long)(pItemProp->dwSkillTime / 1000.0f);
//				}
//
//				CTimeSpan ct( lData );
//				strTemp.Format( prj.GetText( TID_TOOLTIP_ITEMTIME ) );	// ���ӽð� : 
//				str += strTemp;
//				strTemp.Format( " : " );
//				str += strTemp;
//
//				//gmpbigsun : ���ӽð��� 999999999 �̻��̰�, �ٸ� �����ð��� ������� ������ó��
//				if( pItemProp->dwSkillTime >= 0x3b9ac9ff && 
//					pItemProp->dwCircleTime == NULL_ID && 
//					pItemProp->dwAbilityMin == NULL_ID )		
//				{
//					strTemp = prj.GetText( TID_GAME_TOOLTIP_PERMANENTTIME_1 ); //"UNLIMITED";
//					str += strTemp;
//				}
//				else
//				{
//					strTemp.Format( prj.GetText( TID_TOOLTIP_DATE ), static_cast<int>(ct.GetDays()), ct.GetHours(), ct.GetMinutes(), ct.GetSeconds() );	// ���ӽð� : 
//	  				str += strTemp;
//				}
//			}
//				
//			str += strEnter;
			strTemp.Format( prj.GetText( TID_TOOLTIP_USE ), pItemProp->szCommand );	// �뵵 :
			str += strTemp;
			g_toolTip.PutToolTip( m_dwDraw[i], str, rectHittest, point, 0 );
		}
		pt.y += m_nFontHeight;
	}
}

void CWndCommItemCtrl::DrawSM( C2DRender* p2DRender, CPoint* pPoint, int x, int &nScroll )
{
	PFONTCOLOR_WNDCOMMITEMCTRL pFontColorWndCommItemCtrl( g_WndFontColorManager->GetWndCommItemCtrl() );

	for( int i = 0 ; i < SM_MAX; ++i )
	{
		if( g_pPlayer->m_dwSMTime[i] <= 0 )
			continue;
		
		++nScroll;
		if( ( m_wndScrollBar.GetScrollPos() >= nScroll ) )
			continue;
		
		PT_ITEM_SPEC pItem = g_xSpecManager->GetSpecItem( g_AddSMMode.dwSMItemID[i] );
		
		int nResistTexture = 1000;
		if( i == SM_RESIST_ATTACK_LEFT )
			nResistTexture = g_pPlayer->m_nAttackResistLeft - 1;
		else if( i == SM_RESIST_ATTACK_RIGHT )
			nResistTexture = g_pPlayer->m_nAttackResistRight - 1;
		else if( i == SM_RESIST_DEFENSE )
			nResistTexture = g_pPlayer->m_nDefenseResist + 5 - 1;

		if( pItem != NULL && 
			( pWndWorld->m_dwSMItemTexture[i] != NULL || 
			( ( i == SM_RESIST_ATTACK_LEFT || i == SM_RESIST_ATTACK_RIGHT || i == SM_RESIST_DEFENSE ) && pWndWorld->m_dwSMResistItemTexture[nResistTexture] != NULL && SAI79::END_PROP > nResistTexture ) 
			) )
		{
			if( i == SM_RESIST_ATTACK_LEFT || i == SM_RESIST_ATTACK_RIGHT || i == SM_RESIST_DEFENSE )
				p2DRender->RenderTexture( CPoint( 2, pPoint->y ), pWndWorld->m_dwSMResistItemTexture[nResistTexture], 192 );
			else
				p2DRender->RenderTexture( CPoint( 2, pPoint->y ), pWndWorld->m_dwSMItemTexture[i], 192 );
			m_dwDraw[m_nMaxDraw] = g_AddSMMode.dwSMItemID[i];
			++m_nMaxDraw;
		}
			
		CEditString strEdit( _T("") );
		strEdit.SetString( pItem->szName, pFontColorWndCommItemCtrl->m_stItemName.GetFontColor(), ESSTY_BOLD );
	
		p2DRender->TextOut_EditString( x + 40, pPoint->y + 3, strEdit );	

		CString string( _T("") );
		CTimeSpan ct( g_pPlayer->m_dwSMTime[i] );

		if( pItem->dwID == ITEM_INDEX( 10430, II_SYS_SYS_SCR_BLESSING ) || pItem->dwID == ITEM_INDEX( 25197, II_SYS_SYS_SCR_BLESSING_02 ))
			string.Format( prj.GetText( TID_GAME_COND_USE ) );
		else
			string.Format( prj.GetText( TID_TOOLTIP_DATE ), static_cast<int>(ct.GetDays()), ct.GetHours(), ct.GetMinutes(), ct.GetSeconds() );

		p2DRender->TextOut( x + 40, pPoint->y + 18, string, m_dwColor );
		pPoint->y += m_nFontHeight;
	}
}

void	CWndCommItemCtrl::DrawTicket( C2DRender* p2DRender, CPoint* pPoint, int x, int &nScroll )
{
	PFONTCOLOR_WNDCOMMITEMCTRL		pFontColorWndCommItemCtrl( g_WndFontColorManager->GetWndCommItemCtrl() );

	CString 						str( _T("") );
	CString 						string( _T("") );

	FLItemElem*						pItem( NULL );
	PT_ITEM_SPEC					pProp( NULL );


	for( DWORD dwItem = 0; dwItem < g_pPlayer->m_Inventory.GetMax(); dwItem++ )
	{
		pItem = g_pPlayer->m_Inventory.GetAtId(dwItem);

		if( !IsUsableItem( pItem ) )
			return;

		pProp = pItem->GetProp();
		if( NULL == pProp )
			return;
	
		if(pProp->dwItemKind3 == IK3_TICKET)
		{
			if( pItem != NULL && 
				pItem->m_dwKeepTime > 0 && 
				pItem->IsFlag( FLItemElem::expired ) == FALSE)
			{
				if( pItem )
				{
					if( pProp->bCharged == FALSE )
						continue;
					if( pProp->dwItemKind3 == IK3_EGG )
						continue;
					//sun: 13, �Ͽ�¡ �ý���
					if( pProp->dwItemKind1 == IK1_HOUSING )
						continue;

					if( pItem->m_pTexture == NULL )
						continue;

					++nScroll;
					if( ( m_wndScrollBar.GetScrollPos() >= nScroll ) )
						continue;

					p2DRender->RenderTexture( CPoint( 2, pPoint->y ), pItem->m_pTexture, 192 );

					CEditString strEdit( _T("") );
					strEdit.SetString( pProp->szName, pFontColorWndCommItemCtrl->m_stItemName.GetFontColor(), ESSTY_BOLD );
				
					p2DRender->TextOut_EditString( x + 40, pPoint->y + 3, strEdit );	

					time_t t	= pItem->m_dwKeepTime - time_null();
					if( t > 0 )
					{
						CTimeSpan time( t );
						str.Format( prj.GetText( TID_TOOLTIP_DATE ), static_cast<int>(time.GetDays()), time.GetHours(), time.GetMinutes(), time.GetSeconds() );
					}
					string	= str + prj.GetText( TID_TOOLTIP_PERIOD );
					p2DRender->TextOut( x + 40, pPoint->y + 18, string, m_dwColor );

					pPoint->y += m_nFontHeight;// + 3;
					m_dwDraw[m_nMaxDraw] = pProp->dwID;
					++m_nMaxDraw;
				}
				else
				{
					string = _T("");
				}
			}
			else
			{
				pItem = NULL;
			}
		}
	}
	
	
}

//---------------------------------------------------------------------------------------------
// ������ �׾Ƹ� ���ӽð� �׸���
// param	: ������, ��ġ... 
// return	: void
//---------------------------------------------------------------------------------------------
void CWndCommItemCtrl::DrawElldinsJar( C2DRender* p2DRender, CPoint* pPoint, int x, int &nScroll )
{
	PFONTCOLOR_WNDCOMMITEMCTRL		pFontColorWndCommItemCtrl( g_WndFontColorManager->GetWndCommItemCtrl() );

	CString 						str( _T("") );
	CString 						string( _T("") );

	for( DWORD dwItem = 0; dwItem < g_pPlayer->m_Inventory.GetMax(); dwItem++ )
	{
		FLItemElem* pItem( g_pPlayer->m_Inventory.GetAtId( dwItem ) );

		if( !IsUsableItem( pItem ) )
			continue;

		PT_ITEM_SPEC pProp( pItem->GetProp() );
		if( NULL == pProp )
			continue;
	
		if( pProp->dwItemKind2 != IK2_ELLDINPOTION ||
			pProp->dwItemKind3 != IK3_SAVEPOTION )
			continue;

		if( pItem->m_dwKeepTime > 0 && 
			pItem->IsFlag( FLItemElem::expired ) == FALSE )
		{
			if( pItem->m_pTexture == NULL )
				continue;

			++nScroll;
			if( ( m_wndScrollBar.GetScrollPos() >= nScroll ) )
				continue;

			p2DRender->RenderTexture( CPoint( 2, pPoint->y ), pItem->m_pTexture, 192 );

			CEditString strEdit( _T("") );
			strEdit.SetString( pProp->szName, pFontColorWndCommItemCtrl->m_stItemName.GetFontColor(), ESSTY_BOLD );
		
			p2DRender->TextOut_EditString( x + 40, pPoint->y + 3, strEdit );	

			time_t t = pItem->m_dwKeepTime - time_null();
			if( t > 0 )
			{
				CTimeSpan time( t );
				str.Format( prj.GetText( TID_TOOLTIP_DATE ), static_cast<int>(time.GetDays()), time.GetHours(), time.GetMinutes(), time.GetSeconds() );
			}
			string	= str + prj.GetText( TID_TOOLTIP_PERIOD );
			p2DRender->TextOut( x + 40, pPoint->y + 18, string, m_dwColor );

			pPoint->y += m_nFontHeight;
			m_dwDraw[m_nMaxDraw] = pProp->dwID;
			++m_nMaxDraw;
		}
	}
}


void CWndCommItemCtrl::DrawSkill( C2DRender* p2DRender, CPoint* pPoint, int x, int &nScroll )
{
	multimap<DWORD, BUFFSKILL>::value_type* pp = NULL;

	PFONTCOLOR_WNDCOMMITEMCTRL	pFontColorWndCommItemCtrl( g_WndFontColorManager->GetWndCommItemCtrl() );
	BOOL						bExpRander[7] = { NULL, };
	BOOL						IsOverlap( FALSE );

	for( int iRander = 0 ; iRander < 7 ; ++iRander )
	{
		bExpRander[iRander]		= TRUE;
	}

	InitRenderBuffList();
	PT_ITEM_SPEC pMainProp = NULL;
	for( BuffItr it1 = g_pPlayer->m_buffs.m_listBuffs.begin(); it1 != g_pPlayer->m_buffs.m_listBuffs.end(); ++it1 )
	{
		IBuff* ptr1	= *it1;
		if( ptr1 != NULL && ( ptr1->GetType() == BUFF_SKILL || ptr1->GetType() == BUFF_EQUIP ) )
		{
			continue;
		}

		DWORD dwSkillID		= ptr1->GetId();
		int nExpCount	= 0;
		DWORD dwExpTime[_MAX_ES_SCROLL_NUM];
		ZeroMemory( dwExpTime, sizeof(dwExpTime) );

		pMainProp = g_xSpecManager->GetSpecItem( dwSkillID );
		if( pMainProp == NULL )
		{
			continue;
		}

		if( pMainProp->dwItemKind3 == IK3_EXP_RATE )
		{
			//gmpbigsun( 20111028 ) : �������� �״�� �� �����ִ°ɷ� ������. #20187
			bool	bRenderBuff = false;
			//bRenderBuff = IsRenderBuff( (WORD)pMainProp->dwID );	// �ߺ��˻� ����

			if( bRenderBuff == true )
			{
				//	���� �������� ����.
				continue;
			}
			else
			{
				InsertRenderBuffList( (WORD)pMainProp->dwID );
			}
		}

		if( pMainProp->bCharged == FALSE )
			continue;
		if( pMainProp->dwItemKind3 == IK3_EGG )
			continue;
//sun: 13, �Ͽ�¡ �ý���
		if( pMainProp->dwItemKind1 == IK1_HOUSING )
			continue;

		if(pWndWorld->m_pBuffTexture[2].find(dwSkillID) != pWndWorld->m_pBuffTexture[2].end())
			pp	= &( *( pWndWorld->m_pBuffTexture[2].find( dwSkillID ) ) );

		if(pp == NULL)
			continue;

		if( pp->second.m_pTexture == NULL )
			continue;

		++nScroll;
		if( ( m_wndScrollBar.GetScrollPos() >= nScroll ) )
			continue;

		p2DRender->RenderTexture( CPoint( 2, pPoint->y ), pp->second.m_pTexture, 192 );

		CEditString strEdit( _T("") );
		strEdit.SetString( pMainProp->szName, pFontColorWndCommItemCtrl->m_stItemName.GetFontColor(), ESSTY_BOLD );
				
		p2DRender->TextOut_EditString( x + 40, pPoint->y + 3, strEdit );	

		CString string( _T("") );

		if( pMainProp->dwID == ITEM_INDEX( 10464, II_SYS_SYS_SCR_SMELPROT ) || pMainProp->dwID == ITEM_INDEX( 10468, II_SYS_SYS_SCR_SMELTING )
			|| pMainProp->dwID == ITEM_INDEX( 26203, II_SYS_SYS_SCR_SUPERSMELTING )
			|| pMainProp->dwID == ITEM_INDEX( 10488, II_SYS_SYS_SCR_SMELPROT3 )
			|| pMainProp->dwID == ITEM_INDEX( 26473, II_SYS_SYS_SCR_SMELPROT4 )
			|| pMainProp->dwID == ITEM_INDEX( 25233, II_SYS_SYS_SCR_SMELPROT6 )
			|| pMainProp->dwID == ITEM_INDEX( 10489, II_SYS_SYS_SCR_SMELTING2 )
#ifdef COSTUME_UPGRADE_ENHANCEMENT_GEM
			|| pMainProp->dwID == ITEM_INDEX( 25336, II_SYS_SYS_SCR_COSTUMEPROT )
#endif
		)
		{
			string.Format( "%s", prj.GetText( TID_GAME_DEMOL_USE ) );
		}
		else if( pMainProp->dwID == ITEM_INDEX( 10469, II_SYS_SYS_SCR_RETURN ) )
		{
			string.Format( "%s", prj.GetText( TID_TOOLTIP_RETURN_USE ) );
		}
		else if( pMainProp->dwItemKind3 == IK3_EXP_RATE )
		{
			//gmpbigsun( 20111109, #20187 ) : �� �����ۺ��� �ð� ����
			//������ �ߺ��Ǵ� ������ �Ѱ��� ����ϰ��־���, �׷��� �����Ǵ³༮�鸸 ���� �����ؼ� �����ϴ� �ڵ尡 �־����� ������.
			//�ߺ� ���ο� ������� ��� ���� ���.
			DWORD dwTimeThis = 0;
			if( pMainProp->IsAbsoluteTime() == TRUE )
			{
				time_t	t = (time_t)ptr1->GetLevel() - time_null();
				if( t < 0 )
					t	= 0;

				dwTimeThis	= t;
			}
			else
			{
				dwTimeThis = (DWORD)( ( ptr1->GetTotal() - ( g_tmCurrent - ptr1->GetInst() ) ) / 1000.0f );
			}

			CTimeSpan ct( (long)(dwTimeThis) );		// �����ð��� �ʴ����� ��ȯ�ؼ� �Ѱ���
				
			CString strTemp, str;

			if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
			{
				strTemp.Format( prj.GetText( TID_TOOLTIP_ITEMTIME ) );	// ���ӽð� : 
				str += strTemp;
				strTemp.Format( " : " );
				str += strTemp;
			}

			strTemp.Format( prj.GetText( TID_MMI_BUFF_TIME ), ct.GetDays(), ct.GetHours(), ct.GetMinutes(), ct.GetSeconds() );	// ���ӽð� : 
			str += strTemp;
			string	= str;	
		}
		else
		{
			if( pMainProp->dwItemKind3 == IK3_ANGEL_BUFF )
			{
				string.Format( prj.GetText( TID_GAME_TIP_ANGEL_PLAIN ) );	
			}
			else if( pMainProp->IsAbsoluteTime() == TRUE )
			{
				time_t	t = (time_t)ptr1->GetLevel() - time_null();
				if( t < 0 )
					t	= 0;
				CTimeSpan ts( t );

				CString strTemp, str;
				
				if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )				
				{
					strTemp.Format( prj.GetText( TID_TOOLTIP_ITEMTIME ) );	// ���ӽð� : 
					str += strTemp;
					strTemp.Format( " : " );
					str += strTemp;
				}
				
				strTemp.Format( prj.GetText( TID_TOOLTIP_DATE ), static_cast<int>(ts.GetDays()), ts.GetHours(), ts.GetMinutes(), ts.GetSeconds() );	// ���ӽð� : 
				str += strTemp;
				string	= str;	
			}
			else if( g_xSpecManager->IsChargeZoneTicket( pMainProp->dwID ) != false )
			{
				FLItemElem* pTicket	= NULL;

				for( DWORD dwObjID = 0; dwObjID < g_pPlayer->m_Inventory.GetMax(); ++dwObjID )
				{
					pTicket	= g_pPlayer->m_Inventory.GetAtId( dwObjID );
					if( pTicket != NULL
						&& pTicket->m_dwItemId == pMainProp->dwID
						&& pTicket->IsFlag( FLItemElem::is_using ) != FALSE
						&& pTicket->IsFlag( FLItemElem::expired ) == FALSE
						&& pTicket->m_dwKeepTime > 0
						)
					{
						break;
					}
					else
					{
						pTicket	= NULL;
					}
				}
				
				CString str;
				
				if( pTicket )
				{
					time_t t	= pTicket->m_dwKeepTime - time_null();
					if( t > 0 )
					{
						CTimeSpan time( t );
						str.Format( prj.GetText( TID_TOOLTIP_DATE ), static_cast<int>(time.GetDays()), time.GetHours(), time.GetMinutes(), time.GetSeconds() );
					}
					string	= str + prj.GetText( TID_TOOLTIP_PERIOD );
				}
				else
				{
					string	= "";
				}
			}
			else if( pMainProp->dwID == ITEM_INDEX( 21035, II_SYS_SYS_SCR_PET_FEED_POCKET ) )
			{
				//�˻��ؼ� Ȱ��ȭ �� ���� �ָӴϸ� ã�´�.
				FLItemElem* ptr;
				FLItemElem* pItemElem = NULL;

				int nMax = g_pPlayer->m_Inventory.GetMax();
				for( int i = 0 ; i < nMax; i++ )
				{
					ptr	= g_pPlayer->m_Inventory.GetAtId( i );
					if( IsUsableItem( ptr ) && ptr->m_dwItemId == ITEM_INDEX( 21035, II_SYS_SYS_SCR_PET_FEED_POCKET ) &&
						ptr->m_dwKeepTime > 0 && !ptr->IsFlag( FLItemElem::expired ) )	// Ȱ��ȭ�� ���� �ָӴ��� ���
					{						
						pItemElem = ptr;
						i = nMax;
					}
				}
				if(pItemElem != NULL)
				{
					CString strTemp;
					//��� ���� ����
					time_t t = pItemElem->m_dwKeepTime - time_null();
//					if( pItemElem->m_dwKeepTime && !pItemElem->IsFlag( FLItemElem::expired ) )
					{
						if( t > 0 )
						{
							CTimeSpan ct( t );
							string.Format( prj.GetText( TID_TOOLTIP_DATE ), static_cast<int>(ct.GetDays()), ct.GetHours(), ct.GetMinutes(), ct.GetSeconds() );
						}
					}
				}
			}
			else
			{

 				if( pMainProp->dwSkillTime >= 0x3b9ac9ff )		//gmpbigsun : (999999999) �ð����� ���� 
 				{
 					string = prj.GetText( TID_GAME_TOOLTIP_PERMANENTTIME_1 ); // �ð� ������
 				}
 				else
 				{
					DWORD dwOddTime = ptr1->GetTotal() - ( g_tmCurrent - ptr1->GetInst() );
					CTimeSpan ct( (dwOddTime / 1000 ) );		// �����ð��� �ʴ����� ��ȯ�ؼ� �Ѱ���

					CString strTemp, str;

					if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
					{
						strTemp.Format( prj.GetText( TID_TOOLTIP_ITEMTIME ) );	// ���ӽð� : 
						str += strTemp;
						strTemp.Format( " : " );
						str += strTemp;
					}
					
					strTemp.Format( prj.GetText( TID_TOOLTIP_DATE ), static_cast<int>( ct.GetDays() ), ct.GetHours(), ct.GetMinutes(), ct.GetSeconds() );	// ���ӽð� : 
					str += strTemp;
					string	= str;	

// 
// 					string	= prj.GetText( TID_TOOLTIP_DATE );
// 					string.Format( prj.GetText( TID_TOOLTIP_DATE ), static_cast<int>( ct.GetDays() ), ct.GetHours(), ct.GetMinutes(), ct.GetSeconds() );
				}
			}
		}

		p2DRender->TextOut( x + 40, pPoint->y + 18, string, m_dwColor );
		
		pPoint->y += m_nFontHeight;// + 3;
		m_dwDraw[m_nMaxDraw] = pMainProp->dwID;
		++m_nMaxDraw;
	}
}

void CWndCommItemCtrl::OnDraw( C2DRender* p2DRender ) 
{
	if( NULL == g_pPlayer ||
		NULL == p2DRender )
		return;

	CPoint pt( 3, 3 );

//	int		nMaxCount( GetMaxBuff() );

	int		x( NULL );
	int		nWidth( m_rectClient.Width() );

		// ���� ���̴� ������ ���������μ� ���� ũ�� ���̴� ������ ���������μ��� ���� 
	//if( nMax - m_wndScrollBar.GetScrollPos() > m_wndScrollBar.GetScrollPage() )
	//	nMax = m_wndScrollBar.GetScrollPage() + m_wndScrollBar.GetScrollPos();
	//if( nMax < m_wndScrollBar.GetScrollPos() )
	//	nMax = 0;
	
	//for( int i = 0; i < m_wndScrollBar.GetScrollPos() && i < nMaxCount; ++i );

	CRect rect( x, pt.y, x + nWidth, pt.y + m_nFontHeight );
	rect.SetRect( x + 3, pt.y + 6, x + 3 + 32, pt.y + 6 + 32 ); 

	memset( m_dwDraw, 0, sizeof( m_dwDraw ) );
	m_nMaxDraw = 0;
	
	int nScroll( 0 );

	DrawSM( p2DRender, &pt, x, nScroll );
	DrawSkill( p2DRender, &pt, x, nScroll );
	DrawTicket( p2DRender, &pt, x, nScroll );
	DrawElldinsJar( p2DRender, &pt, x, nScroll );

	m_wndScrollBar.SetScrollRange( 0, nScroll );
	m_wndScrollBar.SetScrollPage( GetClientRect().Height() / ( m_nFontHeight ) );
}

void CWndCommItemCtrl::OnSize( UINT nType, int cx, int cy )
{
	CRect rect = GetWindowRect();
	rect.left = rect.right - 15;
	m_wndScrollBar.SetWndRect( rect );

	int nPage, nRange;
	nPage = GetClientRect().Height() / m_nFontHeight;
	int nCount = 0;
	for( int k = 0 ; k < SM_MAX ; ++k )
	{
		if( 0 < g_pPlayer->m_dwSMTime[k] )
		{
			++nCount;
		}
	}

	nCount	+= g_pPlayer->m_buffs.GetCommercialCount();

	nRange = nCount;//m_pItemContainer->m_dwIndexNum;// - nPage;
	m_wndScrollBar.SetScrollRange( 0, nRange );
	m_wndScrollBar.SetScrollPage( nPage );
	
	CWndBase::OnSize( nType, cx, cy);
}
void CWndCommItemCtrl::SetWndRect( CRect rectWnd, BOOL bOnSize )
{
	m_rectWindow = rectWnd;
	m_rectClient = m_rectWindow;
	m_rectClient.DeflateRect( 3, 3 );

	if( bOnSize )
		OnSize( 0, m_rectClient.Width(), m_rectClient.Height() );
}


void CWndCommItemCtrl::PaintFrame( C2DRender* p2DRender )
{
}

//////////////////////////////////////////////////////////////////////////
void	CWndCommItemCtrl::InitRenderBuffList()
{
	memset( m_wRenderBuffList, DEFAULT_RENDER_BUFF_VALUE, sizeof(m_wRenderBuffList) );

	return;
}

void	CWndCommItemCtrl::InsertRenderBuffList( WORD wRendBuffID )
{
	bool	bRenderBuff = false;
	bRenderBuff = IsRenderBuff( wRendBuffID );

	if( bRenderBuff == true )
	{
		//gmpbigsun( 20111109 ) : �ߺ��ؼ� ���ü�����.
		//FLERROR_LOG( PROGRAM_NAME, _T( "IsRendBuff[%d]" ), wRendBuffID );
	}
	else
	{
		for( int i=0; i<MAX_RENDER_BUFF_COUNT; ++i )
		{
			if( m_wRenderBuffList[i] == DEFAULT_RENDER_BUFF_VALUE )
			{
				m_wRenderBuffList[i] = wRendBuffID;

				break;
			}
		}
	}

	return;
}

bool	CWndCommItemCtrl::IsRenderBuff( WORD wRendBuffID )
{
	bool	bRenderBuff = false;

	for( int i=0; i<MAX_RENDER_BUFF_COUNT; ++i )
	{
		if( m_wRenderBuffList[i] == wRendBuffID )
		{
			bRenderBuff = true;

			break;
		}
	}

	return	bRenderBuff;
}
//////////////////////////////////////////////////////////////////////////

/****************************************************
  WndId : APP_COMMERCIAL_ELEM - ��� �Ӽ� ������ ���
  CtrlId : WIDC_STATIC1 - �Ӽ� �ɷ�
  CtrlId : WIDC_STATIC3 - ������
  CtrlId : WIDC_STATIC4 - �Ӽ� ������
  CtrlId : WIDC_EDIT1 - 
  CtrlId : WIDC_EDIT2 - 
  CtrlId : WIDC_CUSTOM1 - Custom
  CtrlId : WIDC_CUSTOM2 - Custom
  CtrlId : WIDC_ELEM_OK - Button
  CtrlId : WIDC_ELEM_CANCEL - Button
****************************************************/

CWndCommercialElem::CWndCommercialElem() 
{ 
	SetPutRegInfo( FALSE );
	m_pItemElem[0] = NULL;
	m_pItemElem[1] = NULL;
	memset( m_Rect, 0, sizeof(m_Rect) );
} 
CWndCommercialElem::~CWndCommercialElem() 
{ 
	InitSetting();
} 
void CWndCommercialElem::OnDraw( C2DRender* p2DRender ) 
{ 
	for( int i=0; i<2; i++ )
	{
		if( m_pItemElem[i] && m_pItemElem[i]->GetTexture() )
		{
			m_pItemElem[i]->GetTexture()->Render( p2DRender, m_Rect[i].TopLeft(), 255 );

			if( m_pItemElem[i]->m_nItemNum > 1 )
			{
				TCHAR szTemp[32];
				FLSPrintf( szTemp, _countof( szTemp ), "%d", m_pItemElem[i]->GetExtra() );
				p2DRender->TextOut( m_Rect[i].right-11,  m_Rect[i].bottom-11 , szTemp, 0xff1010ff );
			}
			CRect hitrect = m_Rect[i];
			CPoint point = GetMousePoint();
			if( m_Rect[i].PtInRect( point ) )
			{
				CPoint point2 = point;
				ClientToScreen( &point2 );
				ClientToScreen( &hitrect );
				
				g_WndMng.PutToolTip_Item( (FLItemBase*)m_pItemElem[i], point2, &hitrect );
			}
		}
	}
} 
void CWndCommercialElem::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	InitSetting();
	LPWNDCTRL pCustom = NULL;
	pCustom = GetWndCtrl( WIDC_CUSTOM1 );
	m_Rect[0] = pCustom->rect;
	pCustom = GetWndCtrl( WIDC_CUSTOM2 );
	m_Rect[1] = pCustom->rect;	

	// ������ �߾����� �ű�� �κ�.
	CWndInventory* pWndInventory = (CWndInventory*)g_WndMng.CreateApplet( APP_INVENTORY );
	
	CRect rcInventory	= pWndInventory->GetWindowRect( TRUE );
	CRect rcVendor = GetWindowRect( TRUE );
	CPoint ptInventory	= rcInventory.TopLeft();
	CPoint point;
	if( ptInventory.x > m_pWndRoot->GetWndRect().Width() / 2 )
		point	= ptInventory - CPoint( rcVendor.Width(), 0 );
	else
		point	= ptInventory + CPoint( rcInventory.Width(), 0 );
	
	Move( point );
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndCommercialElem::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_COMMERCIAL_ELEM, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  ���� ������ ���� ��� 
BOOL CWndCommercialElem::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndCommercialElem::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndCommercialElem::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndCommercialElem::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndCommercialElem::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 

void CWndCommercialElem::OnRButtonUp( UINT nFlags, CPoint point )
{
	if( PtInRect(&m_Rect[0], point) )
	{
		InitSetting();
	}
	else
	if( PtInRect(&m_Rect[1], point) )
	{
		m_pItemElem[1]->SetExtra( 0 );
		m_pItemElem[1] = NULL;
	}	
}

BOOL CWndCommercialElem::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	switch( nID )
	{
	case WIDC_ELEM_OK:
		{
			if( m_pItemElem[0] && m_pItemElem[1] )
			{
				g_DPlay.SendCommercialElem( m_pItemElem[0]->m_dwObjId, m_pItemElem[1]->m_dwObjId );
				InitSetting();
				Destroy();
			}
		}
		break;
	case WIDC_ELEM_CANCEL:
		{
			InitSetting();
			Destroy();
		}
		break;
	case WTBID_CLOSE:
		{
			InitSetting();
			Destroy();
		}
		break;
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

void CWndCommercialElem::InitSetting( void )
{
	if( m_pItemElem[0] )
	{
		m_pItemElem[0]->SetExtra( 0 );
		m_pItemElem[0] = NULL;
	}
	if( m_pItemElem[1] )
	{
		m_pItemElem[1]->SetExtra( 0 );
		m_pItemElem[1] = NULL;
	}


}

BOOL CWndCommercialElem::OnDropIcon( LPSHORTCUT pShortcut, CPoint point )
{
	CWndBase* pWndFrame =  pShortcut->m_pFromWnd->GetFrameWnd();
	
	// �������� �κ��丮���� �Դ°�?
	if( pShortcut->m_dwShortcut == SHORTCUT_ITEM && pWndFrame->GetWndId() == APP_INVENTORY )
	{
		BOOL bbid = FALSE;
		FLItemElem* pItemElem;
		pItemElem  = (FLItemElem*)g_pPlayer->GetItemId( pShortcut->m_dwId );
		if( pItemElem && IsRestrictionItem( pItemElem ) )
		{
			if( m_pItemElem[0] == NULL && PtInRect(&m_Rect[0], point) && IsUpgradeItem( pItemElem ) )
			{
				if( m_pItemElem[1] )
				{
					m_pItemElem[1]->SetExtra( 0 );
					m_pItemElem[1] = NULL;
				}
				pItemElem->SetExtra( 1 );
				m_pItemElem[0] = pItemElem;
			}
			else
			if( m_pItemElem[0] && m_pItemElem[1] == NULL && PtInRect(&m_Rect[1], point) && IsSMItem( pItemElem ) )
			{
				pItemElem->SetExtra( 1 );
				m_pItemElem[1] = pItemElem;
			}
			else
			{
				bbid = TRUE;
			}
		}
		else
		{
			bbid = TRUE;
		}

		if( bbid )
			SetForbid( TRUE );
	}
	else
		SetForbid( FALSE );

	return FALSE;
}

BOOL CWndCommercialElem::IsRestrictionItem( FLItemElem* pItemElem, BOOL bMessage )
{
	// �����Ǿ� �ִ���? ����ũ ���������� �˻�
	if( g_pPlayer->m_Inventory.IsEquip( pItemElem->m_dwObjId ) )
	{
		g_WndMng.PutString( prj.GetText(TID_GAME_EQUIPPUT), NULL, prj.GetTextColor(TID_GAME_EQUIPPUT) );
		return FALSE;
	}

	if( pItemElem->GetProp()->nLog >=2 )
	{
		g_WndMng.OpenMessageBox( prj.GetText(TID_UPGRADE_ERROR_NOUNICK), MB_OK, this );
		return FALSE;
	}

	return TRUE;
}

BOOL CWndCommercialElem::IsUpgradeItem( FLItemElem* pItemElem, BOOL bMessage )
{
	// ��(��Ʈ��), ������� �ƴϸ� ���úҰ���
	if( !( ( pItemElem->GetProp()->dwItemKind2 == IK2_WEAPON_MAGIC ||
		pItemElem->GetProp()->dwItemKind2 == IK2_WEAPON_DIRECT ) ||
		( ( pItemElem->GetProp()->dwItemKind2 == IK2_ARMOR || pItemElem->GetProp()->dwItemKind2 == IK2_ARMORETC ) 
		&& pItemElem->GetProp()->dwItemKind3 == IK3_SUIT )
		) )
	{
		CString str;
		str.Format( prj.GetText(TID_GAME_NOTEQUALELEM), pItemElem->GetProp()->szName );
		g_WndMng.PutString( str, NULL, prj.GetTextColor(TID_GAME_NOTEQUALELEM) );
		return FALSE;
	}

	if( pItemElem->m_byItemResist <= SAI79::NO_PROP || SAI79::END_PROP <= pItemElem->m_byItemResist )
	{
//		CString str;
//		str.Format( "%s �Ӽ����� ����Ǿ� �־�� �մϴ�", pItemElem->GetProp()->szName );
		g_WndMng.PutString( prj.GetText(TID_GAME_NOTELEMENT), NULL, prj.GetTextColor(TID_GAME_NOTELEMENT) );		
		return FALSE;
	}

	if( pItemElem->m_nResistSMItemId != 0 ) // �̹������� �������̸� �Ұ���
	{
//		CString str;
//		str.Format( "%s �̹� ����Ǿ� �ִ� ������ �̹Ƿ� �����ų�� �����ϴ�", pItemElem->GetProp()->szName );
		g_WndMng.PutString( prj.GetText(TID_GAME_ALREADYELEM), NULL, prj.GetTextColor(TID_GAME_ALREADYELEM) );
		return FALSE;
	}

	return TRUE;
}

BOOL CWndCommercialElem::IsSMItem( FLItemElem* pItemElem, BOOL bMessage )
{
	if( pItemElem->GetProp()->dwItemKind2 == IK2_SYSTEM )
	{
		BYTE nResist;
		switch( pItemElem->m_dwItemId )
		{
		case ITEM_INDEX( 10277, II_CHR_SYS_SCR_FIREASTONE ):
			nResist = SAI79::FIRE;
			break;
		case ITEM_INDEX( 10278, II_CHR_SYS_SCR_WATEILSTONE ):
			nResist = SAI79::WATER;
			break;
		case ITEM_INDEX( 10279, II_CHR_SYS_SCR_WINDYOSTONE ):
			nResist = SAI79::WIND;
			break;
		case ITEM_INDEX( 10280, II_CHR_SYS_SCR_LIGHTINESTONE ):
			nResist = SAI79::ELECTRICITY;
			break;
		case ITEM_INDEX( 10281, II_CHR_SYS_SCR_EARTHYSTONE ):
			nResist = SAI79::EARTH;
			break;
		case ITEM_INDEX( 10282, II_CHR_SYS_SCR_DEFIREASTONE ):
			nResist = SAI79::FIRE;
			break;
		case ITEM_INDEX( 10283, II_CHR_SYS_SCR_DEWATEILSTONE ):
			nResist = SAI79::WATER;
			break;
		case ITEM_INDEX( 10284, II_CHR_SYS_SCR_DEWINDYOSTONE ):
			nResist = SAI79::WIND;
			break;
		case ITEM_INDEX( 10285, II_CHR_SYS_SCR_DELIGHTINESTONE ):
			nResist = SAI79::ELECTRICITY;
			break;
		case ITEM_INDEX( 10286, II_CHR_SYS_SCR_DEEARTHYSTONE ):
			nResist = SAI79::EARTH;
			break;
		}
		
		// �Ӽ� ���ݷ� �߰�
		if( pItemElem->m_dwItemId == ITEM_INDEX( 10277, II_CHR_SYS_SCR_FIREASTONE ) ||
			pItemElem->m_dwItemId == ITEM_INDEX( 10278, II_CHR_SYS_SCR_WATEILSTONE ) ||
			pItemElem->m_dwItemId == ITEM_INDEX( 10279, II_CHR_SYS_SCR_WINDYOSTONE ) ||
			pItemElem->m_dwItemId == ITEM_INDEX( 10280, II_CHR_SYS_SCR_LIGHTINESTONE ) ||
			pItemElem->m_dwItemId == ITEM_INDEX( 10281, II_CHR_SYS_SCR_EARTHYSTONE ) ) 

		{
			if( m_pItemElem[0]->m_byItemResist == SAI79::NO_PROP )
			{
				g_WndMng.PutString( prj.GetText(TID_GAME_NOTELEMENT), NULL, prj.GetTextColor(TID_GAME_NOTELEMENT) );
				return FALSE;
			}
			
			if( m_pItemElem[0]->GetProp()->dwItemKind2 == IK2_ARMOR ||
				m_pItemElem[0]->GetProp()->dwItemKind2 == IK2_ARMORETC )
			{
				g_WndMng.PutString( prj.GetText(TID_GAME_NOTEQUALITEM), NULL, prj.GetTextColor(TID_GAME_NOTEQUALITEM) );
				return FALSE;
			}

			if( m_pItemElem[0]->m_byItemResist != nResist )
			{
				CString str;
				str.Format( prj.GetText(TID_GAME_NOTEQUALELEM), pItemElem->GetProp()->szName );
				g_WndMng.PutString( str, NULL, prj.GetTextColor(TID_GAME_NOTEQUALELEM) );
				return FALSE;
			}
		}
		else // �Ӽ� ���� �߰�
		if(	pItemElem->m_dwItemId == ITEM_INDEX( 10282, II_CHR_SYS_SCR_DEFIREASTONE ) ||
			pItemElem->m_dwItemId == ITEM_INDEX( 10283, II_CHR_SYS_SCR_DEWATEILSTONE ) ||
			pItemElem->m_dwItemId == ITEM_INDEX( 10284, II_CHR_SYS_SCR_DEWINDYOSTONE ) ||
			pItemElem->m_dwItemId == ITEM_INDEX( 10285, II_CHR_SYS_SCR_DELIGHTINESTONE ) ||
			pItemElem->m_dwItemId == ITEM_INDEX( 10286, II_CHR_SYS_SCR_DEEARTHYSTONE ) )
		{
			if( m_pItemElem[0]->m_byItemResist == SAI79::NO_PROP )
			{
				g_WndMng.PutString( prj.GetText(TID_GAME_NOTELEMENT), NULL, prj.GetTextColor(TID_GAME_NOTELEMENT) );
				return FALSE;
			}
			
			if( m_pItemElem[0]->GetProp()->dwItemKind2 == IK2_WEAPON_MAGIC ||
				m_pItemElem[0]->GetProp()->dwItemKind2 == IK2_WEAPON_DIRECT )
			{
				g_WndMng.PutString( prj.GetText(TID_GAME_NOTEQUALITEM), NULL, prj.GetTextColor(TID_GAME_NOTEQUALITEM) );
				return FALSE;
			}

			if( m_pItemElem[0]->m_byItemResist != nResist )
			{
				CString str;
				str.Format( prj.GetText(TID_GAME_NOTEQUALELEM), pItemElem->GetProp()->szName );
				g_WndMng.PutString( str, NULL, prj.GetTextColor(TID_GAME_NOTEQUALELEM) );
				return FALSE;
			}
		}
		else // �Ӽ� ����
		if( pItemElem->m_dwItemId == ITEM_INDEX( 10276, II_CHR_SYS_SCR_TINEINEDSTONE ) )
		{
			if( m_pItemElem[0]->m_nResistAbilityOption <= 0 )
				return FALSE;
		}
		else
			return FALSE;
	}
	else 
		return FALSE;

	return TRUE;
}

/****************************************************
  WndId : APP_REMOVE_ELEM - �Ӽ� ����
  CtrlId : WIDC_STATIC_MESSAGE - 
  CtrlId : WIDC_BTN_ELEMOK - 
  CtrlId : WIDC_BTN_ELEMCANCEL - Button
****************************************************/

CWndRemoveElem::CWndRemoveElem() 
{ 
	m_nType		= 0;
	m_nParts	= 0;
	m_dwItemId	= 0;
	m_objid		= 0;
	m_pItemElem = NULL;
	m_bSetting	= FALSE;
} 
CWndRemoveElem::~CWndRemoveElem() 
{ 
} 

void CWndRemoveElem::OnSetItem( BYTE nType, DWORD dwItemId, OBJID objid, int nParts, FLItemElem* pItemElem )
{
	m_nType = nType;
	m_nParts = nParts;
	m_dwItemId = dwItemId;
	m_objid = objid;
	m_pItemElem = pItemElem;
	m_bSetting = TRUE;
}
void CWndRemoveElem::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndRemoveElem::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate();
	m_bSetting = FALSE;
	// ���⿡ �ڵ��ϼ���
	CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT_MESSAGE );
	if( pWndText )
	{
		CString string;
		string.Format( prj.GetText( TID_GAME_REMOVEELEM ) );
		pWndText->m_string = string;
	}

	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndRemoveElem::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_REMOVE_ELEM, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  ���� ������ ���� ��� 
BOOL CWndRemoveElem::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndRemoveElem::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndRemoveElem::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndRemoveElem::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndRemoveElem::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndRemoveElem::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	switch( nID )
	{
	case WIDC_BTN_ELEMOK:
		{
			if( m_bSetting )
			{
				if( m_nType == 1 )
				{
					g_DPlay.SendDoUseItem( m_dwItemId, m_objid, m_nParts, FALSE );
				}
				else
				if( m_nType == 2 )
				{
					g_DPlay.SendDoEquip( m_pItemElem, m_nParts, FALSE );
				}
				m_bSetting = FALSE;
			}
			Destroy();
		}
		break;
	case WIDC_BTN_ELEMCANCEL:
		{
			Destroy();
		}
		break;
	case WTBID_CLOSE:
		{
			Destroy();
		}
		break;
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 