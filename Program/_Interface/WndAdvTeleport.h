#ifndef __WNDADVTELEPORT__H
#define __WNDADVTELEPORT__H

enum class eMapType : uint8_t {
	REGION, DUNGEON, ALL
	, MAX
};

#ifdef __APP_TELEPORTER
class CWndTeleportList : public CWndListBox
{
private:
	vector<DWORD> m_vecItem;
public:
	CWndTeleportList();
	~CWndTeleportList();

	void UpdateList(CProject::eDest dwWorldId = CProject::eDest::DEFAULT);
	virtual void OnDraw(C2DRender*p2DRender);
	virtual void OnMouseWndSurface(CPoint pt);
#if defined (__MODEL_VIEW)
    virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
#endif
};

class CWndTeleportDestination : public CWndListBox
{
private:
	std::map<CProject::eDest, DWORD>			m_vecDest;
	eMapType						m_mapType;

public:
	CWndTeleportDestination();

	// int m_nCurSelect;
	void							UpdateList();
	virtual void					OnDraw(C2DRender*p2DRender);
	virtual void					OnLButtonDown( UINT nFlags, CPoint point ); 
	virtual void					OnMouseWndSurface(CPoint pt);

	void							SetMapType(eMapType);
	bool							IsMapType(eMapType mt) { return m_mapType == eMapType::ALL || m_mapType == mt; }

	CProject::eDest							GetDest() const {
		if (m_nCurSelect < 0 || m_nCurSelect >= this->m_vecDest.size()) {
			return CProject::eDest::DEFAULT;
		}

		int i = 0;
		for (const auto& m : m_vecDest) {
			if (i == m_nCurSelect) {
				return m.first;
			}
			++i;
		}

		return CProject::eDest::DEFAULT;
	}
};

class CWndTeleporter : public CWndNeuz
{
private:
	DWORD						GetMapTypeText(uint8_t i) {
		switch (static_cast<eMapType>(i)) {
		case eMapType::REGION:
			return TID_TELEPORTER_COMBO_REGION;
			break;
		case eMapType::DUNGEON:
			return TID_TELEPORTER_COMBO_DUNGEON;
			break;
		case eMapType::ALL:
		default:
			return TID_TELEPORTER_COMBO_ALL;
			break;
		}
	}

public:
    CProject::eDest						m_nSelected;
    CTexture*					m_pPicTexture;
	CWndTeleportList*			m_pWndTeleportList;
	CWndTeleportDestination*	m_pWndTeleportDestination;
	CWndComboBox*				m_pComboType;
	
    CWndTeleporter( void );

    virtual BOOL				Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
    virtual BOOL				OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
    virtual void				OnDraw( C2DRender* p2DRender ); 
    virtual void				OnInitialUpdate();
	void						UpdateList(CProject::eDest m_nSelected = CProject::eDest::DEFAULT);

#ifdef __TIMER
public:
	void	OnDungeonTimer( CAr & ar );
private:
	DWORD m_timer;
	vector<CoolDungeons> vDun;
	const void ConvertDungeons( vector<CoolDungeons>& v );
	const DWORD GetTimer( int nselected );
#endif
}; 
#endif //__APP_TELEPORTER
#endif // __WNDADVTELEPORT__H




























