// WndArcane.h: interface for the CWndNeuz class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WNDTITLE_H__A93F3186_63D6_43C1_956F_EC8691E0C7D9__INCLUDED_)
#define AFX_WNDTITLE_H__A93F3186_63D6_43C1_956F_EC8691E0C7D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

DECLARE_WNDMESSAGEBOX( CWndConnectingBox ) 
DECLARE_WNDMESSAGEBOX( CWndDisconnectedBox ) 
DECLARE_WNDMESSAGEBOX( CWndCharBlockBox )
DECLARE_WNDMESSAGEBOX( CWndAllCharBlockBox )

#include "Wnd2ndPassword.h"

class CWndLoginExJapan : public CWndNeuz
{
public:
	CWndLoginExJapan( void );
	~CWndLoginExJapan( void );

public:
	virtual BOOL Initialize(CWndBase* pWndParent = NULL,DWORD dwStyle = 0);
	virtual	void OnInitialUpdate();
};

class CWndLogin : public CWndNeuz
{
	//CTexture   m_Texture;
public:

	BOOL m_bDisconnect;

	CWndEdit   m_wndAccount     ;
	CWndEdit   m_wndPassword    ;
	CWndButton m_wndSaveAccount ;
	CWndButton m_wndSavePassword;
	CWndButton m_wndFindAccount ;
	
	CWndButton m_wndLogin    ;
	CWndButton m_wndRegist   ;
	CWndButton m_wndQuit     ;

	CWndLoginExJapan m_WndLoginExJapan;

	int GetCurrentDomainChannel( void );

	void Connected();

	void Login();

	CWndLogin();
	virtual ~CWndLogin();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL,DWORD dwStyle = 0);
	virtual BOOL Process ();
	// message
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);

	void LoginButtonEnableSetting( void );
};

BEGIN_WNDCLASS( CWndSelectServer )
public:
	vector<CString> m_vecStrBanner;	
	DWORD		 m_dwChangeBannerTime;
	LPIMAGE		 m_atexPannel;
	
	CWndEdit     m_wndURL       ;
	CWndButton   m_wndSearch    ;
	CWndListCtrl	m_wndServerList;
	CWndButton   m_wndBack      ;
	CWndButton   m_wndAccept    ;

	virtual BOOL Process ();
	void AfterSkinTexture( LPWORD pDest, CSize size, D3DFORMAT d3dFormat = D3DFMT_A4R4G4B4 );
	void Connected();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);	

	std::vector< T_SERVER_DESC > m_vecServerDesc;
END_WNDCLASS


BEGIN_WNDCLASS( CWndDeleteChar )
public:
/*
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
*/
	void DeletePlayer( int nSelect, LPCTSTR szNo );
	virtual void AdditionalSkinTexture( LPWORD pDest, CSize sizeSurface, D3DFORMAT d3dFormat = D3DFMT_A4R4G4B4 );
END_WNDCLASS
/*
class CWndConfirmSell : public CWndNeuz 
{ 
public: 
	CMover* m_pMover;
	FLItemElem* m_pItemElem;
	CWndConfirmSell(); 
	~CWndConfirmSell(); 

	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
};
*/
BEGIN_WNDCLASS( CWndSelectChar )
public:
	CWndText m_wndText1;
	CWndText m_wndText2;
	CWndText m_wndText3;
	
	CWndDeleteChar* m_pWndDeleteChar;
	CWnd2ndPassword* m_pWnd2ndPassword;
static int m_nSelectCharacter; 
	CRect m_aRect[ MAX_CHARACTER_SLOT ];
	DWORD m_dwMotion[ MAX_CHARACTER_SLOT ];
	CModelObject* m_pBipedMesh[ MAX_CHARACTER_SLOT ];
	BOOL m_bDisconnect;

private:
	BOOL m_CreateApply; //서버통합 관련 특정 기간 캐릭터 생성 금지.
	CWndButton* m_pWndPageLeft;
	CWndButton* m_pWndPageRight;
	CWndStatic* m_pStaticCharCnt;
	DWORD		m_dwNameColor[MAX_CHARACTER_SLOT];
	CRect		m_rcCharName[ MAX_CHARACTER_SLOT ];
	DWORD		m_dwHighAlpha;

public:
	void DeleteCharacter();
	void UpdateCharacter();
	void Connected();
	void OnDestroyChildWnd( CWndBase* pWndChild );

	BOOL SetMotion( CModelObject* pModel, DWORD dwIndex, DWORD dwMotion, int nLoop, DWORD dwOption );
		
	void SelectCharacter( int i );

	virtual BOOL    Process();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void	OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

END_WNDCLASS

BEGIN_WNDCLASS( CWndCreateChar )
	CWndButton   m_wndAccept   ;

/*
	CWndComboBox m_wndComboGender;
	CWndComboBox m_wndComboHair;
	CWndComboBox m_wndComboSkin;
	CWndComboBox m_wndComboHairColor;
	CWndComboBox m_wndComboCostume;
	CWndComboBox m_wndComboJob ;
	CWndEdit     m_wndName     ;
	CWndButton   m_wndCancel   ;
	CWndButton   m_wndCreate   ;

	CWndButton   m_wndMale;
	CWndButton   m_wndFemale;
	CWndButton   m_wndLHair;
	CWndButton   m_wndRHair;
	CWndButton   m_wndLHairColor;
	CWndButton   m_wndRHairColor;
	CWndButton   m_wndLFace;
	CWndButton   m_wndRFace;
	*/
	//CWndButton   m_wndLCostume;
	//CWndButton   m_wndRCostume;
	//CWndButton   m_wndNude;

	CModelObject* m_pModel;

	PLAYER m_Player;

	void SetSex( int nSex );
	void Connected();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();

	enum { MAX_2ND_PASSWORD_NUMBER = 4 };

END_WNDCLASS

#endif // !defined(AFX_WNDTITLE_H__A93F3186_63D6_43C1_956F_EC8691E0C7D9__INCLUDED_)
