#ifndef __WNDOPTIONGAME__H
#define __WNDOPTIONGAME__H

class CWndOptionGame : public CWndNeuz 
{ 
public: 
	CWndOptionGame(); 
	~CWndOptionGame(); 

	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
//sun: 12, 옵션창 개선
	CTexture         m_Texture;
	CTexture         m_TexturePt;
	BOOL			 m_bLButtonClick;
	BOOL			 m_bLButtonClick2;
	int				 m_nStep[2];

	virtual void OnMouseMove(UINT nFlags, CPoint point);

	void GetRangeSlider( DWORD dwWndId, int& nStep, CPoint point );
	int GetSliderStep( DWORD dwWndId, int &nStep, CPoint point );
	CPoint GetStepPos( int nStep, int nWidth, int nDivision );

	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();

	//gmpbigsun(20101208) : 디폴트 기능 추가
	void UpdateData( );

}; 
#endif
