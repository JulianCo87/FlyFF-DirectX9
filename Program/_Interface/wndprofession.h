#ifndef __WND_PROFESSION_H_
#define __WND_PROFESSION_H_

#ifdef __PROFESSION
struct __PROFESSION_TYPE
{
	DWORD dwID;
	/*CString*/DWORD strType;
};

#if !defined __WIKIPEDIA_ITEM
struct __WIKI_TYPE
{
	DWORD dwID;
	DWORD strType;
};
#endif

class CWndProfessionCtrl : public CWndListBox
{
private:
	int nCategorie;
	map<DWORD, ProfessionCraft> m_aCraft;
public:
	CWndProfessionCtrl();
	~CWndProfessionCtrl();
	
	virtual void OnInitialUpdate();
	virtual void OnDraw( C2DRender* p2DRender );
	virtual void OnLButtonDown( UINT nFlags, CPoint point );
	int GetSelectIndex( CPoint point );
	void OnCategoriesList(int nCat);
	void FillTask();
	
public:
	int m_nSelect;
};

class CWndProfession : public CWndNeuz
{
private:
	CWndComboBox*		m_pComboKind;
	CWndComboBox*		m_pComboKind1;
	CWndProfessionCtrl* m_wndProfessionCtrl;
public:
	CWndProfession();
	~CWndProfession();
	
	virtual void OnDraw( C2DRender* p2DRender );
	virtual BOOL OnDropIcon( LPSHORTCUT lpShortcut, CPoint point );
	virtual void OnInitialUpdate();
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK );
	virtual void OnRButtonUp( UINT nFlags, CPoint point );
	void GetCategoriesList();
	
	map<DWORD, pair</*CString*/DWORD, vector<__PROFESSION_TYPE>>> m_aCategories;
	map<DWORD, vector<DWORD>> m_aPack;
	void loadScript();	
public:
	FLItemElem* m_pItemElem[3];
	CRect m_rect[3];
};
#endif // __PROFESSION
#endif // __WND_PROFESSION_H_