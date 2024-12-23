#include "stdafx.h"
#include "resData.h"
#include "defineText.h"
#include "WndAdvTeleport.h"
#ifdef __MODEL_VIEW
#include "WndModelView.h"
#endif
#include "DPClient.h"
#include "MsgHdr.h"

extern	CDPClient	g_DPlay;

#ifdef __APP_TELEPORTER

CWndTeleportDestination::CWndTeleportDestination() : m_mapType(eMapType::ALL)
{
	m_nCurSelect = 0;
	m_nFontHeight = 40;
}
void CWndTeleportDestination::UpdateList()
{
	ResetContent();
	m_wndScrollBar.SetScrollPos(0);
	
	m_vecDest.clear();

	if (IsMapType(eMapType::REGION)) {
		m_vecDest.insert(std::make_pair(CProject::eDest::FLARIS, TID_TELEPORT_FLARIS_ZONE));
		m_vecDest.insert(std::make_pair(CProject::eDest::FLARIS_COLLECT, TID_TELEPORT_FLARIS_COLLECTE));
		m_vecDest.insert(std::make_pair(CProject::eDest::SAINT_CITY, TID_TELEPORT_SAINTMORNING));
		m_vecDest.insert(std::make_pair(CProject::eDest::RHISIS, TID_TELEPORT_JARDINRHISIS));
		m_vecDest.insert(std::make_pair(CProject::eDest::DARKON_1, TID_TELEPORT_D1_ZONE));
		m_vecDest.insert(std::make_pair(CProject::eDest::DARKON_2, TID_TELEPORT_D2_ZONE));
		m_vecDest.insert(std::make_pair(CProject::eDest::DARKON_3, TID_TELEPORT_D3_ZONE));
		m_vecDest.insert(std::make_pair(CProject::eDest::HARMONIN, TID_TELEPORT_HARMONIN_ZONE));
		m_vecDest.insert(std::make_pair(CProject::eDest::ISHTAR, TID_TELEPORT_ISHTAR_ZONE));
		m_vecDest.insert(std::make_pair(CProject::eDest::KAILUN, TID_TELEPORT_KAILUN));
		m_vecDest.insert(std::make_pair(CProject::eDest::ARHAKIS, TID_TELEPORT_ARHAKIS));
		m_vecDest.insert(std::make_pair(CProject::eDest::REALM, TID_GAME_TELEPORTER_PYRAMIDE));
	}

	if (IsMapType(eMapType::DUNGEON)) {
		m_vecDest.insert(std::make_pair(CProject::eDest::VOLCANE, TID_TELEPORT_VOLCANE));
		// m_vecDest.insert(std::make_pair(CProject::eDest::DESIREA, TID_TELEPORTER_DESIREA)); // Unused on this server
		m_vecDest.insert(std::make_pair(CProject::eDest::DESIREA_M, TID_TELEPORT_DESIREA_MASTER));
		m_vecDest.insert(std::make_pair(CProject::eDest::AMINUS, TID_TELEPORT_VAMPIRIQUE));
		m_vecDest.insert(std::make_pair(CProject::eDest::AMINUS_M, TID_TELEPORT_ANCES));
		m_vecDest.insert(std::make_pair(CProject::eDest::FROZEN, TID_TELEPORT_FROZEN_ZONE));
		m_vecDest.insert(std::make_pair(CProject::eDest::PANDORA, TID_TELEPORT_PANDORA_ZONE));
		m_vecDest.insert(std::make_pair(CProject::eDest::MAIDEN, TID_TELEPORT_MAIDEN));
		m_vecDest.insert(std::make_pair(CProject::eDest::FERIAL, TID_TELEPORT_FERIAL_ZONE));
		m_vecDest.insert(std::make_pair(CProject::eDest::HYLAL, TID_GAME_TELEPORTER_HYLAL));
	}
}
void CWndTeleportDestination::OnDraw(C2DRender * p2DRender)
{
	DWORD			dwColor = 0xffffffff;
	CWndTeleporter * pWndTeleporter = (CWndTeleporter*)GetParentWnd();
	int				nIndex = 0;

	//ScrollBar
	{
		int nPage = GetClientRect().Height() / m_nFontHeight;
		int nRange = m_vecDest.size();
		if (IsWndStyle(WBS_VSCROLL))
		{
			m_wndScrollBar.SetVisible(TRUE);
			m_wndScrollBar.SetScrollRange(0, nRange);
			m_wndScrollBar.SetScrollPage(nPage);
		}
		else
			m_wndScrollBar.SetVisible(FALSE);
	}
	int nDrawCount = 0;
	int nLine = 0;

	for (const auto& dest : m_vecDest) {
		if (nLine < GetScrollPos())
		{
			nLine++;
			continue;
		}
		else
			nLine++;

		const auto i = dest.first;
		const auto right = GetWndRect().Width()-32;

		const CRect fullRect(0, nIndex * m_nFontHeight, GetWndRect().Width() - 32, nIndex * m_nFontHeight + m_nFontHeight);
		const CRect itemRect(fullRect.left, fullRect.top + 3, fullRect.right, fullRect.bottom);
		if (itemRect.PtInRect(GetMousePoint()) || (nLine - 1) == m_nCurSelect) {
			p2DRender->RenderFillRect(itemRect, D3DCOLOR_ARGB(255, 156, 181, 191));
			p2DRender->RenderRect(itemRect, D3DCOLOR_ARGB(255, 106, 131, 141));
		}
		p2DRender->RenderLine(CPoint(itemRect.left, itemRect.bottom), CPoint(fullRect.right, fullRect.bottom), 0xcc000000);
		p2DRender->TextOut(itemRect.left, itemRect.top + GetFontHeight() / 2, GETTEXT(dest.second), dwColor);

		++nIndex;
		++nDrawCount;
	}
}

void CWndTeleportDestination::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWndTeleporter* pWndTeleporter = (CWndTeleporter*)GetParentWnd();
	int nLine = 0;

	int nIndex = 0;
	for (const auto& dest : m_vecDest) {
		if (nLine < GetScrollPos())
		{
			nLine++;
			continue;
		}
		else
			nLine++;

		const CRect fullRect(0, nIndex * m_nFontHeight, GetWndRect().Width() - 32, nIndex * m_nFontHeight + m_nFontHeight);
		const CRect itemRect(fullRect.left, fullRect.top + 3, fullRect.right, fullRect.bottom);
		if (itemRect.PtInRect(point))
		{
			pWndTeleporter->m_nSelected = dest.first;
			pWndTeleporter->UpdateList(dest.first);
			m_nCurSelect = nLine-1;
			return;
		}

		++nIndex;
	}
}

void CWndTeleportDestination::OnMouseWndSurface(CPoint pt)
{
	CWndTeleporter * pWndTeleporter = (CWndTeleporter*)GetParentWnd();
	int nLine = 0;

	int nIndex = 0;
	for (int i = 0; i < (int)m_vecDest.size(); i++)
	{
		if (nLine < GetScrollPos())
		{
			nLine++;
			continue;
		}
		else
			nLine++;

		CEditString strEdit;
		DWORD dwColorType = D3DCOLOR_XRGB(0, 93, 0);

		const CRect fullRect(0, nIndex * m_nFontHeight, GetWndRect().Width() - 32, nIndex * m_nFontHeight + m_nFontHeight);
		const CRect itemRect(fullRect.left, fullRect.top + 3, fullRect.right, fullRect.bottom);
		if (itemRect.PtInRect(pt))
		{
			CRect rect2 = itemRect;
			CPoint pt2 = pt;
			ClientToScreen(&rect2);
			ClientToScreen(&pt2);

			if (i >= 11 && i <= 19)
			{
				strEdit.AddString(GETTEXT(TID_GAME_TELEPORT_DJN_PARTY), dwColorType, ESSTY_BOLD);
				g_toolTip.PutToolTip(100, strEdit, &rect2, pt2);
			}
		}
		++nIndex;

	}
}
void CWndTeleportDestination::SetMapType(eMapType mt) {
	m_nCurSelect = 0;
	this->m_mapType = mt;
	this->UpdateList();
}

CWndTeleportList::CWndTeleportList()
{
	CWndListBox::m_nFontHeight = 40;
}

CWndTeleportList::~CWndTeleportList()
{

}

#if defined (__MODEL_VIEW)
void CWndTeleportList::OnLButtonDown( UINT nFlags, CPoint point )
{
	CWndTeleporter * pWndTeleporter = (CWndTeleporter*)GetParentWnd();
	int nLine = 0;

	int nIndex = 0;
	for (auto Item = m_vecItem.begin(); Item != m_vecItem.end(); Item++)
	{


		if (nLine < GetScrollPos())
		{
			nLine++;
			continue;
		}
		else
			nLine++;
		CItemElem pItemElem;
		pItemElem.m_dwItemId = *Item;

		CRect rect(0, nIndex * m_nFontHeight, GetWndRect().right - 20, m_nFontHeight + nIndex * m_nFontHeight);
		if (rect.PtInRect(point))
		{
			if( GetAsyncKeyState( VK_LCONTROL ) & 0x8000 )
			{
#if defined (__BOX_ITEM_CONTAIN)	
				PPACKITEMELEM pPackItemElem	= CPackItem::GetInstance()->Open(pItemElem.m_dwItemId);
				map<DWORD, int>::iterator i		= CGiftboxMan::GetInstance()->m_mapIdx.find(pItemElem.m_dwItemId);
				
				if( pPackItemElem || i != CGiftboxMan::GetInstance()->m_mapIdx.end())
				{
					SAFE_DELETE(g_WndMng.m_pWndBoxItemContain);
					g_WndMng.m_pWndBoxItemContain = new CWndBoxItemContain;
					g_WndMng.m_pWndBoxItemContain->UpdateBox(pItemElem.m_dwItemId);
					g_WndMng.m_pWndBoxItemContain->Initialize();
				}
#endif //__BOX_ITEM_CONTAIN	
			}
			else if( GetAsyncKeyState( VK_SHIFT ) & 0x8000 )
			{
				PPACKITEMELEM pPackItemElem	= CPackItem::GetInstance()->Open(pItemElem.m_dwItemId);
				if( pPackItemElem )
				{
#ifndef __M_MODEL_VIEWER_FIX
					for( int i = 0; i < pPackItemElem->nSize; i++ )
					{
						CWndModelView *pWndViewer = (CWndModelView*)g_WndMng.GetApplet(APP_MODEL_VIEW);
						CItemElem* itemElem = new CItemElem;
						itemElem->m_dwItemId = pPackItemElem->adwItem[i];
						pWndViewer = (CWndModelView*)g_WndMng.CreateApplet(APP_MODEL_VIEW);
						if( !pWndViewer )
						{
							if( pWndViewer )
							{
								if( !pWndViewer->SetPart(itemElem->GetProp()) )
									pWndViewer->Destroy();
							}
						}else
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
				else
				{
#ifndef __M_MODEL_VIEWER_FIX
					CWndModelView *pWndViewer = (CWndModelView*)g_WndMng.GetApplet(APP_MODEL_VIEW);
					if( !pWndViewer )
					{
						pWndViewer = (CWndModelView*)g_WndMng.CreateApplet(APP_MODEL_VIEW);
						if( pWndViewer )
						{
							pWndViewer->AddWndStyle(WBS_TOPMOST);
							if( !pWndViewer->SetPart( pItemElem.GetProp() ) )
								pWndViewer->Destroy();
						}
					}
					else
						pWndViewer->SetPart( pItemElem.GetProp() );
#else
					if (!g_WndMng.m_pWndModelView)
					{
						g_WndMng.m_pWndModelView = new CWndModelView();
						g_WndMng.m_pWndModelView->Initialize();
						if (!g_WndMng.m_pWndModelView->SetPart(pItemElem.GetProp())) {
							g_WndMng.m_pWndModelView->Destroy();
						}
				}
					else {
						g_WndMng.m_pWndModelView->SetPart(pItemElem.GetProp());
					}
#endif
				}
			}
		}
		++nIndex;
		
	}
}
#endif

void CWndTeleportList::UpdateList(CProject::eDest destId)
{
	m_vecItem.clear();
	ResetContent();

	const auto items = prj.GetTeleportItem(static_cast<int8_t>(destId));
	if (!items) {
		return;
	}

	for (auto item : *items) {
		AddString(" ");
		m_vecItem.push_back(item);
	}
}
void CWndTeleportList::OnDraw(C2DRender * p2DRender)
{
	DWORD			dwColor = 0xffffffff;
	CWndTeleporter * pWndTeleporter = (CWndTeleporter*)GetParentWnd();
	int				nIndex = 0;

	//ScrollBar
	{
		int nPage = GetClientRect().Height() / m_nFontHeight;
		int nRange = m_vecItem.size();
		if (IsWndStyle(WBS_VSCROLL))
		{
			m_wndScrollBar.SetVisible(TRUE);
			m_wndScrollBar.SetScrollRange(0, nRange);
			m_wndScrollBar.SetScrollPage(nPage);
		}
		else
			m_wndScrollBar.SetVisible(FALSE);
	}
	int nLine = 0;

	for (auto Item = m_vecItem.begin(); Item != m_vecItem.end(); Item++)
	{
		if (nLine < GetScrollPos())
		{
			nLine++;
			continue;
		}
		else
			nLine++;

		FLItemElem pItemElem;
		pItemElem.m_dwItemId = *Item;
		pItemElem.SetTexture();

		const CRect fullRect(0, nIndex * m_nFontHeight, GetWndRect().Width() - 32, nIndex * m_nFontHeight + m_nFontHeight);
		const CRect itemRect(fullRect.left, fullRect.top+3, fullRect.right, fullRect.bottom);
		CPoint itemPoint(itemRect.left, itemRect.top);
		if (itemRect.PtInRect(GetMousePoint()) || (nLine - 1) == m_nCurSelect) {
			p2DRender->RenderFillRect(itemRect, D3DCOLOR_ARGB(255, 156, 181, 191));
			p2DRender->RenderRect(itemRect, D3DCOLOR_ARGB(255, 106, 131, 141));
			itemPoint.x += 4;
		}
		p2DRender->RenderLine(CPoint(itemRect.left, itemRect.bottom), CPoint(fullRect.right, fullRect.bottom), 0xcc000000);

		constexpr std::size_t max_len = 128;
		TCHAR szCutTitle[max_len+3];
		memset(szCutTitle, 0, sizeof(szCutTitle));
		GetStrCut(pItemElem.GetName(), szCutTitle, max_len);

		if (GetStrLen(pItemElem.GetName()) >= max_len)
			_tcscat(szCutTitle, "...");

		p2DRender->TextOut(itemRect.left+40, itemRect.top+GetFontHeight()/2, szCutTitle, dwColor);
		p2DRender->RenderTexture(itemPoint, pItemElem.GetTexture());

		++nIndex;
	}
}
void CWndTeleportList::OnMouseWndSurface(CPoint pt)
{
	CWndTeleporter * pWndTeleporter = (CWndTeleporter*)GetParentWnd();
	int nLine = 0;

	int nIndex = 0;
	for (auto Item = m_vecItem.begin(); Item != m_vecItem.end(); Item++)
	{
		if (nLine < GetScrollPos())
		{
			nLine++;
			continue;
		}
		else
			nLine++;

		//make tt
		const CRect fullRect(0, nIndex * m_nFontHeight, GetWndRect().Width() - 32, nIndex * m_nFontHeight + m_nFontHeight);
		const CRect itemRect(fullRect.left, fullRect.top + 3, fullRect.left+32, fullRect.top + 3 + 32);
		if (itemRect.PtInRect(pt))
		{
			FLItemElem pItemElem;
			pItemElem.m_dwItemId = *Item;

			CRect rect2 = itemRect;
			CPoint pt2 = pt;
			ClientToScreen(&rect2);
			ClientToScreen(&pt2);

			g_WndMng.PutToolTip_Item((FLItemElem*)&pItemElem, pt2, &rect2,0);
			return;
		}

		++nIndex;
	}
}

CWndTeleporter::CWndTeleporter( void ) :
	m_nSelected(CProject::eDest::DEFAULT),
	m_pPicTexture(NULL),
	m_pWndTeleportList(nullptr),
	m_pWndTeleportDestination(nullptr),
#ifdef __TIMER
	m_timer(0),
#endif
	m_pComboType(nullptr)
{
}

BOOL CWndTeleporter::Initialize( CWndBase* pWndParent, DWORD dwType )
{
    return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_TELEPORTER, 0, CPoint( 0, 0 ), pWndParent );
}

void CWndTeleporter::OnDraw( C2DRender* p2DRender )
{
    CWndStatic* pStatic = (CWndStatic*)GetDlgItem( WIDC_STATIC2 );
    CWndText* pText = (CWndText*)GetDlgItem( WIDC_TEXT1 );
    CString strFile, strLandName, strDes;
	switch (this->m_pWndTeleportDestination->GetDest())
	{
	default: case CProject::eDest::FLARIS: strFile = "flaristp.png"; strLandName = GETTEXT(TID_TELEPORT_FLARIS_ZONE); strDes.Format(GETTEXT(TID_TELEPORT_MONSTER_ZONE_LEVEL), 1, 23); break;
	case CProject::eDest::FLARIS_COLLECT: strFile = "Collectetp.png"; strLandName = GETTEXT(TID_TELEPORT_FLARIS_COLLECTE); strDes.Format(GETTEXT(TID_TELEPORT_MONSTER_ZONE_LEVEL), 1, 150); break;
	case CProject::eDest::SAINT_CITY: strFile = "Stcitytp.png"; strLandName = GETTEXT(TID_TELEPORT_SAINTMORNING); strDes.Format(GETTEXT(TID_TELEPORT_MONSTER_ZONE_LEVEL), 21, 42); break;
	case CProject::eDest::RHISIS: strFile = "Jardinderishistp.png"; strLandName = GETTEXT(TID_TELEPORT_JARDINRHISIS); strDes.Format(GETTEXT(TID_TELEPORT_MONSTER_ZONE_LEVEL), 40, 50); break;
	case CProject::eDest::DARKON_1: strFile = "D1TP.png"; strLandName = GETTEXT(TID_TELEPORT_D1_ZONE); strDes.Format(GETTEXT(TID_TELEPORT_MONSTER_ZONE_LEVEL), 51, 71); break;
	case CProject::eDest::DARKON_2: strFile = "D2TP.png"; strLandName = GETTEXT(TID_TELEPORT_D2_ZONE); strDes.Format(GETTEXT(TID_TELEPORT_MONSTER_ZONE_LEVEL), 55, 77); break;
	case CProject::eDest::DARKON_3: strFile = "D3TP.png"; strLandName = GETTEXT(TID_TELEPORT_D3_ZONE); strDes.Format(GETTEXT(TID_TELEPORT_MONSTER_ZONE_LEVEL), 75, 105); break;
	case CProject::eDest::HARMONIN: strFile = "HARMONINTP.png"; strLandName = GETTEXT(TID_TELEPORT_HARMONIN_ZONE); strDes.Format(GETTEXT(TID_TELEPORT_MONSTER_ZONE_LEVEL), 105, 120); break;
	case CProject::eDest::ISHTAR: strFile = "ISHTARTP.png"; strLandName = GETTEXT(TID_TELEPORT_ISHTAR_ZONE); strDes.Format(GETTEXT(TID_TELEPORT_MONSTER_ZONE_LEVEL), 121, 133); break;
	case CProject::eDest::KAILUN: strFile = "KAILUNTP.png"; strLandName = GETTEXT(TID_TELEPORT_KAILUN); strDes.Format(GETTEXT(TID_TELEPORT_MONSTER_ZONE_LEVEL), 129, 140); break;
	case CProject::eDest::ARHAKIS: strFile = "Baharatp.png"; strLandName = GETTEXT(TID_TELEPORT_ARHAKIS); strDes.Format(GETTEXT(TID_TELEPORT_MONSTER_ZONE_LEVEL), 137, 146); break;
	case CProject::eDest::REALM: strFile = "wdrealmtp.png"; strLandName = GETTEXT(TID_GAME_TELEPORTER_PYRAMIDE); strDes.Format(GETTEXT(TID_TELEPORT_MONSTER_ZONE_LEVEL), 1, 150); break;

	case CProject::eDest::VOLCANE: strFile = "Volcanetp.png"; strLandName = GETTEXT(TID_TELEPORT_VOLCANE); strDes.Format(GETTEXT(TID_TELEPORT_MONSTER_ZONE_LEVEL), 118, 120); break;

	case CProject::eDest::DESIREA: strFile = "luzakaMtp.png"; strLandName = GETTEXT(TID_TELEPORTER_DESIREA); strDes.Format(GETTEXT(TID_TELEPORT_DG), 60, 80, 30, 60); break;
	case CProject::eDest::DESIREA_M: strFile = "luzakaMtp.png"; strLandName = GETTEXT(TID_TELEPORT_DESIREA_MASTER); strDes.Format(GETTEXT(TID_TELEPORT_DG_MASTER), 60, 80, 30, 60); break;
	case CProject::eDest::AMINUS: strFile = "VampiriqueTP.png"; strLandName = GETTEXT(TID_TELEPORT_VAMPIRIQUE); strDes.Format(GETTEXT(TID_TELEPORT_DG), 110, 120, 45, 60); break;
	case CProject::eDest::AMINUS_M: strFile = "AncesTP.png"; strLandName = GETTEXT(TID_TELEPORT_ANCES); strDes.Format(GETTEXT(TID_TELEPORT_DG_MASTER), 110, 121, 60, 100); break;
	case CProject::eDest::FROZEN: strFile = "Frozen.png"; strLandName = GETTEXT(TID_TELEPORT_FROZEN_ZONE); strDes.Format(GETTEXT(TID_TELEPORT_DG_MASTER), 110, 121, 30, 100); break;
	case CProject::eDest::PANDORA: strFile = "Pandora.png"; strLandName = GETTEXT(TID_TELEPORT_PANDORA_ZONE); strDes.Format(GETTEXT(TID_TELEPORT_DG), 130, 130, 180, 250); break;
	case CProject::eDest::MAIDEN: strFile = "Maiden.png"; strLandName = GETTEXT(TID_TELEPORT_MAIDEN); strDes.Format(GETTEXT(TID_TELEPORT_DG), 130, 130, 180, 250); break;
	case CProject::eDest::FERIAL: strFile = "christmastp.png"; strLandName = GETTEXT(TID_TELEPORT_FERIAL_ZONE); strDes.Format(GETTEXT(TID_TELEPORT_DG), 100, 130, 60, 0); break;
	case CProject::eDest::HYLAL: strFile = "Hylal.png"; strLandName = GETTEXT(TID_GAME_TELEPORTER_HYLAL); strDes.Format(GETTEXT(TID_TELEPORT_DG), 150, 150, 300, 500); break;


	}
    pStatic->SetTitle( strLandName );

#ifdef __TIMER
	if(m_timer > 0)
	{
		DWORD dwRemainMin = m_timer / MIN( 1 );
		strDes.Format( GETTEXT(TID_TELEPORT_COOLTIME), strDes, dwRemainMin );
	}
#endif

    pText->SetString( strDes, 0xffffffff);
	const auto strPath = MakePath(DIR_THEME, g_xFlyffConfig->GetMainLanguage(), strFile);
    m_pPicTexture = m_textureMng.AddTexture( m_pApp->m_pd3dDevice, strPath, 0xff000000 );
    if( m_pPicTexture != NULL )
    {
        LPWNDCTRL lpWndCtrl = GetWndCtrl( WIDC_STATIC1 );
        if( g_Option.m_nWindowAlpha > 200 )
            m_pPicTexture->Render( p2DRender, lpWndCtrl->rect.TopLeft(), g_Option.m_nWindowAlpha - 55 );
        else
            m_pPicTexture->Render( p2DRender, lpWndCtrl->rect.TopLeft(), g_Option.m_nWindowAlpha );
    }	
}
void CWndTeleporter::OnInitialUpdate( void )
{
    CWndNeuz::OnInitialUpdate();
#ifdef __TIMER
	m_timer = 0;
	g_DPlay.SendHdr(PacketType::PACKETTYPE_TIMER);
#endif
#if defined (__DUNGEON_PLAYER)
	g_DPlay.SendHdr( PacketType::PACKETTYPE_DUNGEON_PLAYER );
#endif
	m_pWndTeleportList = new CWndTeleportList;
	m_pWndTeleportList->Create(WBS_VSCROLL, GetWndCtrl(WIDC_LISTBOX2)->rect, this, 1337);
	m_pWndTeleportList->UpdateList(m_nSelected);
    RestoreDeviceObjects();
    CRect rectRoot = m_pWndRoot->GetLayoutRect();
    CRect rectWindow = GetWindowRect();
    CPoint point( rectRoot.right - rectWindow.Width(), 110 );
    Move( point );
    MoveParentCenter();

	m_pWndTeleportDestination = new CWndTeleportDestination;
	m_pWndTeleportDestination->Create(WBS_VSCROLL, GetWndCtrl(WIDC_LISTBOX1)->rect, this, 1337);
	m_pWndTeleportDestination->UpdateList();


	m_pComboType = dynamic_cast<CWndComboBox*>(GetDlgItem(WIDC_COMBOBOX1));
	m_pComboType->AddWndStyle(EBS_READONLY);
	for (uint8_t i = 0; i != static_cast<uint8_t>(eMapType::MAX); i++) {
		m_pComboType->AddString(prj.GetText(GetMapTypeText(i)));
	}
	m_pComboType->SetCurSel(static_cast<uint8_t>(eMapType::ALL));
	m_pWndTeleportDestination->SetMapType(eMapType::ALL);
}

BOOL CWndTeleporter::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult )
{
    if (nID == WIDC_LISTBOX1)
    {
		if (m_pWndTeleportDestination->GetCurSel() != -1 || m_pWndTeleportDestination->GetCurSel() > 14)
		{
			m_nSelected = static_cast<CProject::eDest>(m_pWndTeleportDestination->GetCurSel());
			if (m_pWndTeleportList)
				m_pWndTeleportList->UpdateList(m_nSelected);
		}
    }
    else if (nID == WIDC_BUTTON1)
    {
		CWndShop* pShop = (CWndShop*)g_WndMng.GetWndBase(APP_SHOPEX); //Crash fix
		if (pShop)
			pShop->Destroy();

        g_DPlay.SendTeleport( static_cast<int>(m_nSelected) );
        Destroy();
    }
	else if (nID == WIDC_COMBOBOX1 && message == WNM_SELCHANGE) {
		m_nSelected = CProject::eDest::DEFAULT;
		if (m_pWndTeleportList)
			m_pWndTeleportList->UpdateList(m_nSelected);

		if (this->m_pComboType->GetCurSel() != -1)
			m_pWndTeleportDestination->SetMapType(static_cast<eMapType>(this->m_pComboType->GetCurSel()));
	}
    return CWndNeuz::OnChildNotify( message, nID, pLResult );
}

void CWndTeleporter::UpdateList(CProject::eDest m_nSelected)
{
	m_pWndTeleportList->UpdateList(m_nSelected);
#ifdef __TIMER
	m_timer = GetTimer( static_cast<int>(m_nSelected) );
#endif
}

#ifdef __TIMER
const void CWndTeleporter::ConvertDungeons( vector<CoolDungeons>& v )
{
	for(auto it = v.begin(); it != v.end(); ++it)
	{
		switch(it->dwWorld)
		{
		case WI_INSTANCE_RUSTIA: it->dwWorld = 11; break;
		case WI_INSTANCE_RUSTIA_1: it->dwWorld = 12; break;
		case WI_INSTANCE_OMINOUS: it->dwWorld = 13; break;
		case WI_INSTANCE_OMINOUS_1: it->dwWorld = 14; break;
		case WI_INSTANCE_DREADFULCAVE: it->dwWorld = 15; break;
		case WI_INSTANCE_BEHAMAH: it->dwWorld = 16; break;
		case WI_INSTANCE_KALGAS: it->dwWorld = 17; break;
//		case WI_INSTANCE_OREN: it->dwWorld = 18; break;
//		case WI_INSTANCE_FFPY: it->dwWorld = 19; break;
		}
	}
}

void CWndTeleporter::OnDungeonTimer( CAr & ar )
{
	ar.ReadContainerPOD( vDun, 50 );
	ConvertDungeons( vDun );
}

const DWORD CWndTeleporter::GetTimer( int nSelected )
{
	auto it = std::find_if( vDun.begin(), vDun.end(), [nSelected]
		( const CoolDungeons& p ) {return p.dwWorld == nSelected; } );

	if(it != vDun.end())
	{
		//it->dwCool += g_tmCurrent;
		return it->dwCool;
	}

	return 0;
}
#endif
#endif


















