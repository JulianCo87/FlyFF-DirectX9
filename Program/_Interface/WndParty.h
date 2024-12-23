
#pragma once 

#include "WndPartyQuick.h"	
#include "WndPartyChangeTroup.h"


//---------------------------------------------------------------------------------------------
// ������ ��Ƽ ���� Ŭ����
//---------------------------------------------------------------------------------------------
class CWndPartyInfo : public CWndNeuz 
{ 

public:


								CWndPartyInfo(); 
								~CWndPartyInfo(); 

	virtual BOOL 				Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	
	virtual HRESULT 			RestoreDeviceObjects( void );
	virtual HRESULT 			InvalidateDeviceObjects( void );
	virtual HRESULT 			DeleteDeviceObjects( void );
	
	virtual	void 				OnInitialUpdate( void ); 
	virtual void 				OnLButtonDown( UINT nFlags, CPoint point ); 
	virtual void 				OnDraw( C2DRender* p2DRender ); 


private:

	DWORD						GetFillColorofStatus( CMover* pMember, UINT nMemberIndex );

	BOOL						CheckSummary( void );
	void						DrawStatusBox( C2DRender* p2DRender, CMover* pMember, UINT nMemberIndex, UINT nPosY );
	void						DrawMemberInfo( C2DRender* p2DRender, CMover* pMember, UINT nMemberIndex, UINT nPosY );
	void						DrawGauge( C2DRender* p2DRender, CMover* pMember, UINT nPosY );


public:


	int							m_nSelected;	// ���� ��Ƽâ���� ���� �� �ε��� (���������� zero base)

	LPDIRECT3DVERTEXBUFFER9		m_pVBGauge;

	CTexture					m_texGauEmptyNormal;
	CTexture					m_texGauFillNormal;

	CTexture*					m_pGauEmptyNormal;
	CTexture*					m_pGauFillNormal;
}; 


//---------------------------------------------------------------------------------------------
// ������ ��Ƽ ��ų Ŭ����
//---------------------------------------------------------------------------------------------
class CWndPartySkill : public CWndNeuz 
{ 
public: 
	CTexture* m_atexSkill[ 10 ];
	int m_nSkillSelect;
	
	CWndPartySkill(); 
	~CWndPartySkill(); 

	virtual void OnMouseWndSurface( CPoint point );
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point);
}; 


//---------------------------------------------------------------------------------------------
// ������ ��Ƽ Ŭ����
//---------------------------------------------------------------------------------------------
class CWndParty : public CWndNeuz 
{ 

public:


	CWndButton* 			m_pWndLeave;
	CWndButton* 			m_pWndChange;
	CWndButton* 			m_pWndTransfer;
	
	CWndButton*				m_pBtnPartyQuick;
	CWndPartyQuick*			m_pWndPartyQuick;

	
	CWndPartyInfo			m_wndPartyInfo;
	CWndPartySkill			m_wndPartySkill;
	CWndPartyChangeTroup*	m_WndPartyChangeTroup;


public: 


							CWndParty(); 
							~CWndParty(); 

	virtual BOOL 			Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual void 			SerializeRegInfo( CAr& ar, DWORD& dwVersion );


	virtual	void 			OnInitialUpdate( void ); 
	virtual BOOL 			OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual BOOL 			OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void 			OnLButtonDown( UINT nFlags, CPoint point ); 
	virtual void 			OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void 			OnSize( UINT nType, int cx, int cy ); 
	virtual void 			OnDraw( C2DRender* p2DRender ); 
}; 

