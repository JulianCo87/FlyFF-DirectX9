#include "StdAfx.h"
#include "WndProfession.h"
#include "ResData.h"
#include "DPClient.h"
#include <iostream>
extern CDPClient	g_DPlay;

#ifdef __MODEL_VIEW
#include "WndModelView.h"
#endif

#ifdef __PROFESSION
CWndProfession::CWndProfession()
{
	m_wndProfessionCtrl = NULL;
	
	for( int i = 0; i < 3; i++ )
	{
		m_pItemElem[i] = NULL;
		m_rect[i] = NULL;
	}
}

CWndProfession::~CWndProfession()
{
	SAFE_DELETE(m_wndProfessionCtrl);
	
	for( int i = 0; i < 3; i++ )
	{
		if( m_pItemElem[i] )
			m_pItemElem[i]->SetExtra( 0 );
	}
}

void CWndProfession::OnDraw( C2DRender* p2DRender )
{
	if( m_wndProfessionCtrl->m_nSelect != -1 )
	{
		const auto craft = prj.m_profession.vCraft.at(m_wndProfessionCtrl->m_nSelect);
		for( int i = 0; i < 3; i++ )
		{
			if (!m_pItemElem[i]) {
				const auto prop = g_xSpecManager->GetSpecItem(craft.dwNeededItemId[i]);
				if (!prop) { continue; }

				const auto tx = CWndBase::m_textureMng.AddTexture(D3DDEVICE, MakePath(DIR_ICON, prop->szIcon), 0xffff00ff);
				if (!tx) { continue; }

				tx->Render(p2DRender, m_rect[i].TopLeft(), 128);
				continue;
			}

			if( m_pItemElem[i] )
			{
				if( m_pItemElem[i]->m_nItemNum >= craft.nNeededNum[i] && m_pItemElem[i]->GetTexture())
				{
					m_pItemElem[i]->GetTexture()->Render( p2DRender, m_rect[i].TopLeft(), 255 );
				
					if( m_pItemElem[i]->GetExtra() > 1 )
					{
						TCHAR szTemp[32];
						FLSPrintf( szTemp, _countof(szTemp), "%d", m_pItemElem[i]->GetExtra() );
						CSize size = p2DRender->m_pFont->GetTextExtent( szTemp );
						
						CD3DFont* pOldFont = p2DRender->GetFont();
						p2DRender->SetFont(CWndBase::m_Theme.m_pFontSRGiuld);
						p2DRender->TextOut( m_rect[i].right - size.cx, m_rect[i].bottom - 11, szTemp, 0xffffffff);
						p2DRender->SetFont(pOldFont);
					}
					
					if( m_rect[i].PtInRect( GetMousePoint() ) )
					{
						CPoint point = GetMousePoint();
						CRect rect = m_rect[i];
						ClientToScreen( &point );
						ClientToScreen( &rect );
						
						g_WndMng.PutToolTip_Item( (FLItemBase*)m_pItemElem[i], point, &rect );
					}
				}
				else
				{
					m_pItemElem[i]->SetExtra( 0 );
					m_pItemElem[i] = NULL;
				}
			}
		}
		LPWNDCTRL lpWndCtrl = GetWndCtrl(WIDC_STATIC10);
		CTexture* tPenyaIcon = CWndBase::m_textureMng.AddTexture(g_Neuz.m_pd3dDevice, MakePath(DIR_ICON, "itm_GolGolSeed.dds"), 0xffff00ff);
	
		if (tPenyaIcon)
			tPenyaIcon->Render(p2DRender, lpWndCtrl->rect.TopLeft(), 0xffff00ff);
	
		CString str;
		str.Format("%d", prj.m_profession.vCraft.at(m_wndProfessionCtrl->m_nSelect).nPenya);
		str = GetNumberFormatEx(str);
		
		CWndStatic *staticPenyaText = static_cast<CWndStatic*>(GetDlgItem(WIDC_STATIC9));
		staticPenyaText->SetTitle(str);
	}
	else
	{
		LPWNDCTRL lpWndCtrl = GetWndCtrl(WIDC_STATIC10);
		CTexture* tPenyaIcon = nullptr;
	
		if (tPenyaIcon)
			tPenyaIcon->Render(p2DRender, lpWndCtrl->rect.TopLeft(), 0xffff00ff);
		
		CWndStatic *staticPenyaText = static_cast<CWndStatic*>(GetDlgItem(WIDC_STATIC9));
		staticPenyaText->SetTitle("");
	}
}

void CWndProfession::OnInitialUpdate()
{
	CWndNeuz::OnInitialUpdate();
	MoveParentCenter();
	loadScript();
	GetCategoriesList();

	m_wndProfessionCtrl = new CWndProfessionCtrl();
	m_wndProfessionCtrl->Create(WBS_VSCROLL, GetDlgItem(WIDC_LISTBOX)->GetWndRect(), this, WIDC_LISTBOX);
	//m_wndProfessionCtrl->OnCategoriesList(0);

	LPWNDCTRL lpCustomItem[3];
	lpCustomItem[0] = GetWndCtrl( WIDC_STATIC5 );
	lpCustomItem[1] = GetWndCtrl( WIDC_STATIC6 );
	lpCustomItem[2] = GetWndCtrl( WIDC_STATIC7 );
	
	for( int i = 0; i < 3; i++ )
		m_rect[i] = lpCustomItem[i]->rect;
		
	CWndButton* pWndButton = (CWndButton*)GetDlgItem( WIDC_BUTTON1 );
	pWndButton->EnableWindow( FALSE );
}

BOOL CWndProfession::Initialize( CWndBase* pWndParent, DWORD nType )
{
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_PROFESSION, 0, 0, pWndParent );
}

BOOL CWndProfession::OnDropIcon( LPSHORTCUT lpShortcut, CPoint point )
{
	CWndBase* pWndFrame = lpShortcut->m_pFromWnd->GetFrameWnd();
	if( !pWndFrame )
		return FALSE;
		
	if( g_pPlayer->m_Inventory.IsEquip( lpShortcut->m_dwId ) )
	{
		SetForbid( TRUE );
		return FALSE;
	}
	
	if( m_wndProfessionCtrl->m_nSelect != -1 )
	{	
		FLItemElem* pItemElem = (FLItemElem*)g_pPlayer->GetItemId( lpShortcut->m_dwId );
		if( pItemElem )
		{
			for( int i = 0; i < 3; i++ )
			{
				if( PtInRect( m_rect[i], point ) )
				{
					PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( prj.m_profession.vCraft.at( m_wndProfessionCtrl->m_nSelect ).dwNeededItemId[i] );
					if( pItemProp )
					{
						if( pItemElem->GetProp()->dwID == pItemProp->dwID )
						{
							if( pItemElem->m_nItemNum >= prj.m_profession.vCraft.at( m_wndProfessionCtrl->m_nSelect ).nNeededNum[i] )
							{
								if( m_pItemElem[i] )
									m_pItemElem[i]->SetExtra( 0 );
								
								pItemElem->SetExtra( prj.m_profession.vCraft.at( m_wndProfessionCtrl->m_nSelect ).nNeededNum[i] );
								m_pItemElem[i] = pItemElem;
								
								if( m_pItemElem[0] && m_pItemElem[1] && m_pItemElem[2] )
								{
									CWndButton* pWndButton = (CWndButton*)GetDlgItem( WIDC_BUTTON1 );
									pWndButton->EnableWindow( TRUE );
								}
							}
						}
					}
				}
			}
		}
	}
	
	return TRUE;
}

BOOL CWndProfession::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult )
{
	switch(nID)
	{
		case WIDC_BUTTON1:
			if( m_wndProfessionCtrl->m_nSelect != -1 )
			{
				if( g_pPlayer->m_nProfessionLevel >= prj.m_profession.vCraft.at( m_wndProfessionCtrl->m_nSelect ).nLevel )
				{
					if( m_pItemElem[0] && m_pItemElem[1] && m_pItemElem[2] )
					{
						g_DPlay.SendProfessionCraft( m_wndProfessionCtrl->m_nSelect, m_pItemElem[0]->m_dwObjId, m_pItemElem[1]->m_dwObjId, m_pItemElem[2]->m_dwObjId );
					}
				}
				else
					g_WndMng.OpenMessageBox( "Niveau de métier insuffisant" );
			}
			break;
		case WIDC_TREECTRL:
			{
				LPTREEELEM m_pFocusElem = reinterpret_cast<LPTREEELEM>(pLResult);
				m_wndProfessionCtrl->OnCategoriesList(m_pFocusElem->m_dwData);
			}
	}
	
	return CWndNeuz::OnChildNotify( message, nID, pLResult );
}

void CWndProfession::OnRButtonUp( UINT nFlags, CPoint point )
{
	for( int i = 0; i < 3; i++ )
	{
		if( PtInRect( m_rect[i], point ) )
		{
			if( m_pItemElem[i] )
			{
				m_pItemElem[i]->SetExtra( 0 );
				m_pItemElem[i] = NULL;
				
				CWndButton* pWndButton = (CWndButton*)GetDlgItem( WIDC_BUTTON1 );
				pWndButton->EnableWindow( TRUE );
			}
		}
	}
}

void CWndProfession::GetCategoriesList()
{
	CWndTreeCtrl * pTreeCtrl = (CWndTreeCtrl*)GetDlgItem(WIDC_TREECTRL);

	for(auto& it = m_aCategories.begin(); it != m_aCategories.end(); it++)
	{
		LPTREEELEM lpTreeElem = pTreeCtrl->FindTreeElem(it->second.first);
		if (!lpTreeElem) 
			lpTreeElem = pTreeCtrl->InsertItem(NULL, prj.GetText(it->second.first), it->first);

		for (int i = 0; i < it->second.second.size(); i++)
		{
			pTreeCtrl->InsertItem(lpTreeElem, prj.GetText(it->second.second[i].strType), it->second.second[i].dwID);
		}
	}
}

void CWndProfession::loadScript() 
{
	CScript s;
	if (!s.Load("ProfessionCategory.inc")) 
	{
		FLERROR_LOG(PROGRAM_NAME, _T("ProfessionCategory.inc not found."));
		return;
	}

	m_aCategories.clear();
	m_aPack.clear();
	vector<__PROFESSION_TYPE> vecType;
	vecType.clear();
	s.GetToken(); //AddCategory
	while (s.tok != FINISHED) 
	{
		if (s.Token == _T("AddProfessionCategory"))
		{
			/*CString*/DWORD strCategory;
			int dwID;
			map<CString, vector<__WIKI_TYPE>> m_aTempMap;
			vecType.clear();
			s.GetToken(); // (
			dwID = s.GetNumber();
			s.GetToken(); // ,
			strCategory = s.GetNumber();
			//s.GetToken(); // category
			//strCategory = s.token;
			s.GetToken(); // )
			s.GetToken();	// {
			while (*s.token != '}')
			{
				s.GetToken(); // AddItemType
				if (s.Token == "AddProfessionType")
				{
					__PROFESSION_TYPE type;
					s.GetToken(); // (
					type.dwID = s.GetNumber();
					s.GetToken(); // ,
					type.strType = s.GetNumber();
					//s.GetToken(); // title
					//type.strType = s.token;
					s.GetToken(); // )
					vecType.push_back(type);
				}
			}
			//m_aTempMap.insert(make_pair(strCategory, vecType));
			m_aCategories.insert(make_pair(dwID, make_pair(strCategory, vecType)));
			m_aTempMap.clear();
		}
		if (s.Token == _T("AddProfessionPack"))
		{
			int dwID, dwPackID;
			vector<DWORD> vec;
			s.GetToken(); // (
			dwID = s.GetNumber();
			s.GetToken(); // )
			s.GetToken();	// {
			while (*s.token != '}')
			{
				s.GetToken(); // AddItemType
				if (s.Token == "AddProfessionID")
				{
					s.GetToken(); // (
					dwPackID = s.GetNumber();
					s.GetToken(); // )
					vec.push_back(dwPackID);
				}
			}
			m_aPack.insert(make_pair(dwID, vec));
			vec.clear();
		}
		s.GetToken();
	}
}

CWndProfessionCtrl::CWndProfessionCtrl()
{
	CWndListBox::m_nFontHeight = 40;

	m_pTexture = NULL;
	m_nSelect = -1;
}

CWndProfessionCtrl::~CWndProfessionCtrl()
{

}

void CWndProfessionCtrl::OnInitialUpdate()
{
	CRect rect = GetWindowRect();
	m_wndScrollBar.AddWndStyle( WBS_DOCKING );
	m_wndScrollBar.Create( WBS_VERT, rect, this, 1000 );
}

void CWndProfessionCtrl::OnDraw( C2DRender* p2DRender )
{
	CWndProfession * pWndProfession = (CWndProfession*)GetParentWnd();

	if (!pWndProfession)
		return;

	int nIndex = 0;
	int nLine = 0;
	const int nMax = GetScrollPos() + (GetClientRect().Height() / m_nFontHeight);

	//ScrollBar
	{
		int nPage = GetClientRect().Height() / m_nFontHeight;
		int nRange = m_aCraft.size();
		if (IsWndStyle(WBS_VSCROLL))
		{
			m_wndScrollBar.SetVisible(TRUE);
			m_wndScrollBar.SetScrollRange(0, nRange);
			m_wndScrollBar.SetScrollPage(nPage);
		}
		else
			m_wndScrollBar.SetVisible(FALSE);
	}

	for( auto& it = m_aCraft.begin(); it != m_aCraft.end(); it++ )
	{		
		if (nLine < GetScrollPos())
		{
			nLine++;
			continue;
		}
		else
			nLine++;

		if (nLine > nMax) {
			break;
		}
		
		const CRect fullRect(0, nIndex * m_nFontHeight, GetWndRect().Width() - 32, nIndex * m_nFontHeight + m_nFontHeight);
		const CRect itemRect(fullRect.left, fullRect.top + 3, fullRect.right, fullRect.bottom);
		CPoint itemPoint(itemRect.left, itemRect.top);
		if (itemRect.PtInRect(GetMousePoint()) || (nLine - 1) == m_nCurSelect) {
			p2DRender->RenderListRect(itemRect);
			itemPoint.x += 4;
		}
		p2DRender->RenderLine(CPoint(itemRect.left, itemRect.bottom), CPoint(fullRect.right, fullRect.bottom), 0xcc000000);
		
		CD3DFont* pOldFont = p2DRender->GetFont();
		PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( it->second.dwRewardId );
		if( pItemProp )
		{
			CTexture* tItem = CWndBase::m_textureMng.AddTexture(g_Neuz.m_pd3dDevice, MakePath(DIR_ICON, pItemProp->szIcon), 0xffff00ff);
			if( tItem )
				tItem->Render( p2DRender, itemPoint, 0xffff00ff );
			
			TCHAR szTemp[32];
			sprintf( szTemp, "%d", it->second.nRewardNum );
			CSize size = p2DRender->m_pFont->GetTextExtent( szTemp );
			p2DRender->SetFont(CWndBase::m_Theme.m_pFontSRGiuld);
			p2DRender->TextOut(itemRect.left + 34, itemRect.top + 32 - GetFontHeight(), szTemp, 0xffffffff );
			p2DRender->SetFont(pOldFont);

			CRect rect(itemRect.left, itemRect.top, itemRect.left+32, itemRect.bottom);
			if( PtInRect( rect, GetMousePoint() ) )
			{
				CPoint point = GetMousePoint();
				CRect rect2 = rect;
				
				ClientToScreen( &point );
				ClientToScreen( &rect2 );
				
				FLItemElem itemElem;
				itemElem.m_dwItemId = it->second.dwRewardId;
				g_WndMng.PutToolTip_Item( (FLItemBase*)&itemElem, point, &rect2 );
			}
		}

		CPoint pt(130, 30 + (nIndex)* m_nFontHeight);
		
		CString szTempPercent;
		szTempPercent.Format( "Luck: %.2f%%", it->second.nPercent );
		p2DRender->SetFont(CWndBase::m_Theme.m_pFontSRGiuld);	
		p2DRender->TextOut( pt.x, itemRect.top + GetFontHeight() / 2, szTempPercent, 0xffffffff );
		p2DRender->SetFont(pOldFont);
		
		for( int j = 0; j < 3; j++ )
		{
			pt.x = 290 + ( j * 60 );
		
			PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( it->second.dwNeededItemId[j] );
			if( pItemProp )
			{
				CTexture* tItem = CWndBase::m_textureMng.AddTexture(g_Neuz.m_pd3dDevice, MakePath(DIR_ICON, pItemProp->szIcon), 0xffff00ff);
				if( tItem )
					tItem->Render( p2DRender, CPoint( pt.x, itemRect.top), 0xffff00ff );
					
				TCHAR szTemp[32];
				sprintf( szTemp, "%d", it->second.nNeededNum[j] );
				p2DRender->SetFont(CWndBase::m_Theme.m_pFontSRGiuld);
				CSize size = p2DRender->m_pFont->GetTextExtent( szTemp );
				p2DRender->TextOut( pt.x + 34 - size.cx, itemRect.top + 32 - GetFontHeight(), szTemp, 0xffffffff );
				p2DRender->SetFont(pOldFont);
				
				CRect rect( pt.x, itemRect.top, pt.x + 32, itemRect.bottom );
				if( PtInRect( rect, GetMousePoint() ) )
				{
					CPoint point = GetMousePoint();
					CRect rect2 = rect;
					
					ClientToScreen( &point );
					ClientToScreen( &rect2 );
					
					FLItemElem itemElem;
					itemElem.m_dwItemId = it->second.dwNeededItemId[j];
					g_WndMng.PutToolTip_Item( (FLItemBase*)&itemElem, point, &rect2 );
				}
			}
		}
		
		++nIndex;
	}	
}

int CWndProfessionCtrl::GetSelectIndex( CPoint point )
{
	int nBase = m_wndScrollBar.GetScrollPos();
	int nIndex = ( ( point.y - 10 ) / 36 );
	
	if( nIndex >= 0 && nIndex < 5 )
	{
		int nSelect = nIndex + nBase;
	}
	
	return -1;
}

void CWndProfessionCtrl::OnLButtonDown( UINT nFlags, CPoint point )
{
	CWndProfession * pWndProfession = (CWndProfession*)GetParentWnd();
	
	if (!pWndProfession)
		return;
	
	int nIndex = 0;
	int nLine = 0;
	const int nMax = GetScrollPos() + (GetClientRect().Height() / m_nFontHeight);

	for( auto& it = m_aCraft.begin(); it != m_aCraft.end(); it++ )
	{
		if (nLine < GetScrollPos())
		{
			nLine++;
			continue;
		}
		else
			nLine++;

		if (nLine > nMax) {
			break;
		}

		const CRect fullRect(0, nIndex * m_nFontHeight, GetWndRect().Width() - 32, nIndex * m_nFontHeight + m_nFontHeight);
		if (fullRect.PtInRect(GetMousePoint())) {
			const auto pItemProp = g_xSpecManager->GetSpecItem(it->second.dwRewardId);
			if (!pItemProp) {
				break;
			}

			if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
			{
#if defined (__BOX_ITEM_CONTAIN)	
				PPACKITEMELEM pPackItemElem = CPackItem::GetInstance()->Open(pItemProp->dwID);
				map<DWORD, int>::iterator i = CGiftboxMan::GetInstance()->m_mapIdx.find(pItemProp->dwID);

				if (pPackItemElem || i != CGiftboxMan::GetInstance()->m_mapIdx.end())
				{
					SAFE_DELETE(g_WndMng.m_pWndBoxItemContain);
					g_WndMng.m_pWndBoxItemContain = new CWndBoxItemContain;
					g_WndMng.m_pWndBoxItemContain->UpdateBox(pItemProp->dwID);
					g_WndMng.m_pWndBoxItemContain->Initialize();
				}
#endif //__BOX_ITEM_CONTAIN	
				break;
			}
			else if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
			{
#ifdef __MODEL_VIEW
				PPACKITEMELEM pPackItemElem = CPackItem::GetInstance()->Open(pItemProp->dwID);
				if (pPackItemElem)
				{
#ifndef __M_MODEL_VIEWER_FIX
					for (int i = 0; i < pPackItemElem->nSize; i++)
					{
						CWndModelView* pWndViewer = (CWndModelView*)g_WndMng.GetApplet(APP_MODEL_VIEW);
						CItemElem* itemElem = new CItemElem;
						itemElem->m_dwItemId = pPackItemElem->adwItem[i];
						pWndViewer = (CWndModelView*)g_WndMng.CreateApplet(APP_MODEL_VIEW);
						if (!pWndViewer)
						{
							if (pWndViewer)
							{
								if (!pWndViewer->SetPart(itemElem->GetProp()))
									pWndViewer->Destroy();
							}
						}
						else
							pWndViewer->SetPart(itemElem->GetProp());
					}
#else
					if (!g_WndMng.m_pWndModelView) {
						g_WndMng.m_pWndModelView = new CWndModelView();
						g_WndMng.m_pWndModelView->Initialize();
					}
					for (int i = 0; i < pPackItemElem->nSize; i++) {
						g_WndMng.m_pWndModelView->SetPart(prj.GetItemProp(pPackItemElem->adwItem[i]));
					}
#endif
				}
				else if (pItemProp)
				{
#ifndef __M_MODEL_VIEWER_FIX
					CWndModelView* pWndViewer = (CWndModelView*)g_WndMng.GetApplet(APP_MODEL_VIEW);
					if (!pWndViewer)
					{
						pWndViewer = (CWndModelView*)g_WndMng.CreateApplet(APP_MODEL_VIEW);
						if (pWndViewer)
						{
							if (!pWndViewer->SetPart(pItemProp))
								pWndViewer->Destroy();
						}
					}
					else
						pWndViewer->SetPart(pItemProp);
#else
					if (!g_WndMng.m_pWndModelView)
					{
						g_WndMng.m_pWndModelView = new CWndModelView();
						g_WndMng.m_pWndModelView->Initialize();
						if (!g_WndMng.m_pWndModelView->SetPart(pItemProp)) {
							g_WndMng.m_pWndModelView->Destroy();
						}
					}
					else {
						g_WndMng.m_pWndModelView->SetPart(pItemProp);
					}
#endif
				}
#endif // __MODEL_VIEW
				break;
			}
			else {
				if (it->first != m_nSelect)
				{
					for (int i = 0; i < 3; i++)
					{
						if (pWndProfession->m_pItemElem[i])
						{
							pWndProfession->m_pItemElem[i]->SetExtra(0);
							pWndProfession->m_pItemElem[i] = nullptr;
						}
					}
				}
				m_nSelect = it->first;
				m_nCurSelect = nLine - 1;
				break;
			}
		}

		++nIndex;
	}
}

void CWndProfessionCtrl::OnCategoriesList(int nCat)
{
	nCategorie = nCat;
	FillTask();
	m_nSelect = -1;
}

void CWndProfessionCtrl::FillTask()
{
	CWndProfession * pWndProfession = (CWndProfession*)GetParentWnd();

	if (!pWndProfession)
		return;

	ResetContent();
	m_aCraft.clear();

	vector<ProfessionCraft> craft = prj.m_profession.vCraft;
	
	for(int i = 0; i < craft.size(); i++)
	{
		bool bValid = true;

		if(nCategorie != (DWORD(-1)))
		{
			auto& it = pWndProfession->m_aPack.find(nCategorie);

			if(it != pWndProfession->m_aPack.end())
			{
				bValid = false;
				for(int j = 0; j < it->second.size(); j++)
				{
					if(craft[i].nCategorie == it->second[j])
					{
						bValid = true;
						break;
					}
				}
			}
			else if(craft[i].nCategorie != nCategorie)
			{
				continue;
			}	
		}
		
		if( !bValid )
			continue;
		
		DWORD dwIndex = i;

		m_aCraft.insert( make_pair( dwIndex, craft[i] ) );
	}
}
#endif // __PROFESSION