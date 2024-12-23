// WndArcane.h: interface for the CWndNeuz class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WNDTASKBAR_H__A93F3186_63D6_43C1_956F_EC8691E0C7D9__INCLUDED_)
#define AFX_WNDTASKBAR_H__A93F3186_63D6_43C1_956F_EC8691E0C7D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWndShortcut : public CWndButton
{
public:
	CWndShortcut();
	~CWndShortcut();

	//BOOL Create(LPCTSTR lpszCaption,DWORD dwStyle,const RECT& rect,CWndBase* pParentWnd,UINT nID,CSprPack* pSprPack,int nSprIdx,int nColorTable = 0);
	//BOOL Create(LPCTSTR lpszCaption,DWORD dwStyle,const RECT& rect,CWndBase* pParentWnd,UINT nID);// { return Create(lpszCaption,dwStyle,rect,pParentWnd,nID,NULL,0); }
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	BOOL Process();
	virtual	void PaintFrame( C2DRender* p2DRender );
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void OnRButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual void OnRButtonDblClk(UINT nFlags, CPoint point);
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnMouseMove(UINT nFlags, CPoint point);

/*
	void SetMenu(CWndMenu* pWndMenu);

	// Attributes
	//BOOL IsButtonStyle( DWORD dwStyle ) { return ( m_dwButtonStyle & dwStyle ) ? TRUE : FALSE; }
	void SetPushTime(int nTime); // ��ư�� ������ nTime���Ŀ� ��� OnCommand�� �оƮ���� ������.
	UINT GetState() const; // Retrieves the check state, highlight state, and focus state of a button control.
	void SetState(BOOL bHighlight); // Sets the highlighting state of a button control
	int  GetCheck() const; // Retrieves the check state of a button control.
	void SetCheck(int nCheck); // Sets the check state of a button control.
	//UINT GetButtonStyle() const; // Retrieves information about the button control style.
	//void SetButtonStyle(UINT nStyle, BOOL bRedraw = TRUE); // Changes the style of a button.
	void SetString(CString strSndEffect); 
	BOOL IsHighLight() { return m_bHighLight; }
	void SetFontColor     (DWORD dwColor) { m_nFontColor      = (DWORD)dwColor; }
	void SetPushColor     (DWORD dwColor) { m_nPushColor      = (DWORD)dwColor; }
	void SetDisableColor  (DWORD dwColor) { m_nDisableColor   = (DWORD)dwColor; }
	void SetHighLightColor(DWORD dwColor) { m_nHighlightColor = (DWORD)dwColor; }
	DWORD GetFontColor     () { return m_nFontColor     ; }
	DWORD GetPushColor     () { return m_nPushColor     ; }
	DWORD GetDisableColor  () { return m_nDisableColor  ; }
	DWORD GetHighLightColor() { return m_nHighlightColor; }
	void SetWndExecute(CWndBase* pWndBase);
	CWndBase* GetWndExecute() { return m_pWndExecute; }
	void SetPushPoint(int x,int y) { m_ptPush = CPoint(x,y); }
	*/
};
class CWndQuickList : public CWndNeuz 
{ 
public: 
	CWndQuickList(); 
	~CWndQuickList(); 

	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
};

//////////////////////////////////////////////////////////////////////////////////////
// �۾� ���� 
// �޴� ��ư, ��� ������, ���� ������, Ȱ��ȭ ��� ���
//
class CWndTaskBar : public CWndNeuz
{
public:
	BOOL		   IsShortcut( LPSHORTCUT lpShortcut, DWORD dwShortcut, DWORD dwId );

public:
	DWORD			m_dwHighAlpha;
	CWndMenu       m_menuShortcut;
	CWndQuickList* m_pWndQuickList;
	CTexturePack   m_texPack;
	CWndButton     m_wndMenu;
	CWndButton     m_wndQuickList;
	CWndButton     m_wndQuickListUp;
	CWndButton     m_wndQuickListDn;
	SHORTCUT       m_aSlotApplet[ MAX_SLOT_APPLET ]; // 1 ~ 20
	SHORTCUT       m_aSlotItem  [ MAX_SLOT_ITEM_COUNT ][ MAX_SLOT_ITEM ]; // 1 ~ 0(10)
	
	SHORTCUT       m_aSlotQueue [ MAX_SLOT_QUEUE  ];
	LPSHORTCUT     m_paSlotItem;
	LPSHORTCUT     m_pSelectShortcut;
	SHORTCUT       m_aSlotSkill;
	CTexture*      m_pTexture;
	int            m_nMaxSlotApplet;
	BOOL           m_bStartTimeBar;
	int            m_nUsedSkillQueue;
	int            m_nPosition;
	int            m_nSkillBar;
	int            m_nCurQueueNum;
	int            m_nCurQueue;
	CTimer         m_timer;
	int			   m_nExecute;		// 0: ���������� ���� 1:��������(��ų���� �޷�������) 2:������.
	OBJID		   m_idTarget;		// ��ų�����.
	int            m_nSlotIndex;	// m_aSlotItem 2�����迭 ÷��
	int		       m_nActionPoint;		// �׼� ����Ʈ - �ø�������� ���.
	BOOL		   m_bLockActionSlot;

	void RenderLordCollTime( CPoint pt, DWORD dwSkillId, C2DRender* p2DRender );		//sun: 12, ����

	void RenderCollTime( CPoint pt, DWORD dwSkillId, C2DRender* p2DRender );
	void UpdateItem();
	void PutTooTip( LPSHORTCUT pShortcut, CPoint point, CRect* pRect, BOOL bConvert = TRUE );
	BOOL CheckAddSkill( int nSkillStyleSrc, int nQueueDest );
	BOOL UseSkillQueue( CCtrl* pTargetObj );
	LPSKILL GetCurrentSkillQueue();
	BOOL SetShortcut( int nIndex, DWORD dwShortcut, DWORD dwType, DWORD dwId, CTexture* pTexture = NULL, int nWhere = 0 );
	void OnEndSkill( void );
	BOOL RemoveDeleteObj();
	
	virtual BOOL SetSkillQueue( int nIndex, DWORD dwType, DWORD dwId, CTexture* pTexture = NULL );
	virtual void RemoveSkillQueue( int nIndex, BOOL bSend = TRUE );
	virtual LPSHORTCUT Select( CPoint point );
	virtual void InitTaskBar();
	virtual void OnCancelSkill( void );
	virtual void UsedSkill( int nType, BOOL bPassed = FALSE )	{ }
	virtual void SetItemSlot( int nSlot );
	virtual void TurnPage( BOOL bRight = TRUE )	{ }
	virtual void SyncActionSlotReady( )		{ }
	virtual void AllVisible( BOOL bVisible ) { }
	
	BOOL IsLockedActionSlot( )				{ return m_bLockActionSlot; }
	void LockActionSlot( BOOL bLock )		{ m_bLockActionSlot = bLock; }

	CWndTaskBar();
	virtual ~CWndTaskBar();
//	virtual CItem* GetFocusItem() { return NULL; }
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL,DWORD dwWndId = 0);
	// message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point = 0 );
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);
	virtual	void PaintFrame( C2DRender* p2DRender );
	virtual BOOL OnEraseBkgnd(C2DRender* p2DRender);
	virtual BOOL Process();
	virtual void OnMouseWndSurface( CPoint point );
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual	void OnDestroyChildWnd( CWndBase* pWndChild );
	virtual void Serialize( CAr & ar );

	void	SetTaskBarTexture( LPSHORTCUT pShortcut );
	
	HRESULT	RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

protected:
	void RenderOutLineLamp(int x, int y, int num, DWORD size);
	
};
//////////////////////////////////////////////////////////////////////////////////////
// �½�ũ���� �Ŵ� 
//
class CWndTaskMenu : public CWndMenu //Neuz
{
public:
	CWndTaskMenu();
	virtual ~CWndTaskMenu();
	void SetTexture( CWndButton* pWndButton );
//	virtual CItem* GetFocusItem() { return NULL; }
	virtual CWndButton* AppendMenu( CWndMenu* pWndMenu, UINT nFlags, UINT nIDNewItem, LPCTSTR lpszNewItem );
	virtual void OnDraw(C2DRender* p2DRender);
	virtual void PaintFrame(C2DRender* p2DRender);
	virtual BOOL OnEraseBkgnd(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL,DWORD dwWndId = 0);
	// message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase = NULL );
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);
	virtual BOOL Process();
	virtual void OnKillFocus(CWndBase* pNewWnd);
private:
	CWndMenu* m_pMenu1;
	CWndMenu* m_pMenu2;
	CWndMenu* m_pMenu3;
	CWndMenu* m_pMenu4;
	CWndMenu* m_pMenu5;
	CWndMenu* m_pMenu6;
	CWndMenu* m_pMenu7;
};

enum HUD_NUM
{
	HUD_ITEMSLOT_MAX = MAX_SLOT_ITEM,			//ţ���� �Ѱ���
	HUD_ACTIONSLOT_MAX = MAX_SLOT_QUEUE,		//�׼ǽ��� �Ѱ���
};


//Action slot
class CWndActionSlot_HUD : public CWndNeuz
{
	friend class CWndTaskBar_HUD;
public:

	enum MODE
	{
		MODE_NONE,
		MODE_START,
		MODE_START_PASSED,
		MODE_ROTATE,
	};

	enum
	{
		SLOT_SIZE = 46,
//		COOLTIME_MAX = ACTIONSLOT_DEFAULT_COOLTIME,
	};

	enum COLOR
	{
		COLOR_COOLDOWN = 0xff444444,
		COLOR_ACTIVATION = 0xffffffff,
	};

	CWndActionSlot_HUD( );
	virtual ~CWndActionSlot_HUD( );

	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );
	virtual void OnDestroyChildWnd( CWndBase* pWndChild );
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnMouseWndSurface( CPoint point );

	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);

	int GetSlotIndex( CPoint kPt );
	void CustomInit( );
	void Start( BOOL bPassed = FALSE );	
	void Rotate( int nDegree = 60 ); //360 
	void Rewind( );

	void RemoveSkillQueue( int nIndex, BOOL bSend = TRUE );
	BOOL SetSkillQueue( int nIndex, DWORD dwType, DWORD dwId, CTexture* pTexture = NULL );
	void SyncActionSlotReady( );
	void OnDraw_Effect( C2DRender* p2DRender, float fFrameUp );
	
protected:
	void End( );
	void ReadyToUse( );			// ��밡���� ���·�!
	BOOL MakeCircleSlot( );		//��ų �����ܰ� ���� ���ĸʰ� �ռ�
	void LoadTexture( );
	void DrawRotate( C2DRender* p2DRender );
	void UpdateSlot( );

protected:
	CWndTaskBar_HUD* m_pWndMaster;
	MODE m_eMode;
	CPoint m_kSlotPoint[ 6 ];	//������ ��������
	int m_nDegree;				//���� Degree
	int m_nTargetDegree;		//��ǥ Degree
	int m_nOnSlot;				//���콺 �µ� ���Թ�ȣ
	float m_fCoolTime;
	int m_nCurrIndex;			

	BOOL m_bRunning;			// ��ü �ߵ�ȿ���� 
	BOOL m_bReadyToActive;		//�ߵ��߿� ��Ŷ�� �ð�� �غ��س��� �ٷξ����ְ�..
	int m_nCooldowning;			// ���� ��Ž���� ������ �����س��� ������ ����� Update

	//for effects
	CSize m_kSizeCircle;
	CTexture m_kTexCenter1;
	CTexture m_kTexCenter2;
	CTexture m_kTexAlpha;
	CTexture m_kTexCircleSlot;
	CTexture m_kTexDecoration1;
	CTexture m_kTexSlotEff1;
	
	//���� �� ���� ����Ʈ
	CTexture m_kTexSlotEff2;
	int m_nEff2;
	int m_nEff2_Slot;
	
	//�ߵ� ����Ʈ
	int m_nEff3;
	CTexture m_kTexSlotEff3; 
	CTexture m_kTexSlotEff4;
	BOOL m_bReadyFireEffect;			//�ߵ��� �ϳ��� ��ų�̹ߵ��Ǵ� ���� ����Ʈ
	BOOL m_bUpdateSlot;

	int m_nEff5;
	CTexture m_kTexSlotEff5;

	//��Ȱ�� ����Ʈ
	int m_nEff6;
	int m_nEff7;
	BOOL m_bRadySlotEffect;		//��Ÿ�� �Ϸ��� ��Ȱ�� ����Ʈ
	
	CTexture m_kTexRTarget_Skill;				//��ų ������ 6��		
	CTexture m_kTexRTarget_Final;				//���� ���Ա׸��� ����������� �ռ�

	BOOL m_bInitialized;	// for device reset
};

// CWndTaskBar_HUD Ȯ�� ( ���� )
class CWndTaskBar_HUD_EX1 : public CWndNeuz		
{
public:
	CWndTaskBar_HUD_EX1();
	virtual ~CWndTaskBar_HUD_EX1();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );
	virtual void OnDestroyChildWnd( CWndBase* pWndChild );
	virtual	void SetWndRect( CRect rectWnd, BOOL bOnSize = TRUE );
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point );
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );
	virtual void OnMouseWndSurface( CPoint point );
	virtual void SetFocus();
	virtual void OnSize( UINT nType, int cx, int cy );
	virtual BOOL IsPickupSpace( CPoint point ); 

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);

	BOOL		SubOnDropIcon( LPSHORTCUT pShortcut, CPoint point );			//OnDropIcon sub

	void		CustomInit( const int nPage );
	void		SetPage( const int nPage );
	void		UpdateItem();
	void		UpdateMaxViewSlot( const int nMaxViewSlot );
	const int	GetMaxViewSlot( )								{ return m_nMaxViewSlot; }

protected:
	void UpdateSlotIndex( );
	int	 GetSlotIndexByPoint( const CPoint& kPoint );		//no slot is -1
	LPSHORTCUT	Select( CPoint point );
	void UpdateWndOrder( );
	void PullMinMax( );		//���� �ִ� ���� ū ũ��� ����.

protected:
	CWndTaskBar_HUD* m_pWndMaster;
	int m_nMyPage;

	CRect m_kOrgRect;
	BOOL m_bReady;
	BOOL m_bPullMinMax;		//-_-for click
	
	CTexture m_kTexSlot;
	int m_nMaxViewSlot;			//���� �������� �ִ� �����ε���
	LPSHORTCUT     m_paSlotItem;
};

//CWndTaskBar_HUD Ȯ��2
class CWndTaskBar_HUD_EX2 : public CWndNeuz
{
public:
	CWndTaskBar_HUD_EX2();
	virtual ~CWndTaskBar_HUD_EX2();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );
	virtual void OnDestroyChildWnd( CWndBase* pWndChild );
	virtual void OnMouseWndSurface( CPoint point );
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnRButtonUp(UINT nFlags, CPoint point);
	virtual BOOL IsPickupSpace( CPoint point ); 

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);

	BOOL		SubOnDropIcon( LPSHORTCUT pShortcut, CPoint point );			//OnDropIcon sub

	int			GetPage( ) { return m_nMyPage; }
	void		SetPage( const int nPage );

	int			GetSlotIndexByPoint( const CPoint& kPoint );		//no slot is -1
	void		UpdateItem();
	void		SetHotKeyPrefix( BOOL bEnable, const char* szPrefix );

protected:
	LPSHORTCUT	Select( CPoint point );

protected:
	CWndTaskBar_HUD* m_pWndMaster;
	LPSHORTCUT     m_paSlotItem;
	int m_nMyPage;
	BOOL m_bEnableHotkey;
	CString m_strPrefix;
};

//CWndTaskBar_HUD Ȯ��3 ��������
class CWndTaskBar_HUD_EX3 : public CWndNeuz
{
public:
	CWndTaskBar_HUD_EX3();
	virtual ~CWndTaskBar_HUD_EX3();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );
	virtual void OnDestroyChildWnd( CWndBase* pWndChild );
	virtual void OnMouseWndSurface( CPoint point );
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL IsPickupSpace( CPoint point ); 

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);

	BOOL		SubOnDropIcon( LPSHORTCUT pShortcut, CPoint point );			//OnDropIcon sub

	void		SetPage( const int nPage );
	int			GetSlotIndexByPoint( const CPoint& kPoint );		//no slot is -1
	LPSHORTCUT	Select( CPoint point );
	void		UpdateItem();

protected:
	CWndTaskBar_HUD* m_pWndMaster;
	LPSHORTCUT     m_paSlotItem;
	int m_nMyPage;
};


//Taskbar
class CWndTaskBar_HUD : public CWndTaskBar
{
	friend class CWndActionSlot_HUD;
	friend class CWndTaskBar_HUD_EX1;
	friend class CWndTaskBar_HUD_EX2;
	friend class CWndTaskBar_HUD_EX3;

public:
	enum
	{
		PAGE_0 = 0,
		PAGE_1,
		PAGE_2,
		PAGE_3,
		PAGE_4,
		PAGE_MAX,
	};
	CWndTaskBar_HUD();
	virtual ~CWndTaskBar_HUD();
	
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);

	virtual BOOL Process( void );
	virtual void OnDraw(C2DRender* p2DRender);
	virtual void OnDestroyChildWnd( CWndBase* pWndChild );
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point = 0 );
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);
	virtual void OnMouseWndSurface( CPoint point );
	virtual void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);

#ifdef __CLIENT
	virtual void SerializeRegInfo( CAr& ar, DWORD& dwVersion );
#endif

	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();

	//overwrite
	virtual BOOL SetSkillQueue( int nIndex, DWORD dwType, DWORD dwId, CTexture* pTexture = NULL );
	virtual void RemoveSkillQueue( int nIndex, BOOL bSend = TRUE );
	virtual LPSHORTCUT Select( CPoint point );
	virtual void InitTaskBar();
	virtual void Serialize( CAr & ar );
	virtual void OnCancelSkill( void );
	virtual void UsedSkill( int nType, BOOL bPassed = FALSE );
	virtual void SyncActionSlotReady( );
	virtual void AllVisible( BOOL bVisible );

	BOOL SetShortcut2( int nPage, int nIndex, DWORD dwShortcut, DWORD dwType, DWORD dwId, CTexture* pTexture = NULL );
	void ExtendPage( const int nAdded );	//������ Ȯ���϶� Ȥ�� �ٿ���

	void SetPage( const int nPage );
	int GetPage( ) { return m_nSlotIndex; }

	//�ڽĵ��� ���������� �ڵ�����
	virtual void SetItemSlot( int nSlot );
	virtual void TurnPage( BOOL bRight = TRUE );
	
	void SetLock( BOOL bLock );
	
	//order to child
	void Rotate( int nAngle = 60 );
	void Rewind( );

	//support wnd hierachy


protected:
	int GetSlotIndexByPoint( const CPoint& kPoint );		//no slot is -1
	void UpdateButton( );


protected:
	CWndActionSlot_HUD* m_pWndActionSlot;
	CWndTaskBar_HUD_EX1* m_pWndEX1;
	CWndTaskBar_HUD_EX2* m_pWndEX2[2];
	CWndTaskBar_HUD_EX3* m_pWndEX3;

	DWORD m_dwSlotIDs[ HUD_ITEMSLOT_MAX ]; 

	int m_nExtendPage;
	BOOL m_bLock;

	struct INFOCHILD
	{
		INFOCHILD() { _kPt = CPoint( 0, 0 ); _bVisible = FALSE; }
		CPoint _kPt;
		BOOL _bVisible;
	};
	INFOCHILD m_kSavedInfo[ 4 ];		//using client 

	//Solt effects
	CTexture m_texOnSlot;
	CTexture m_texUsingSlot;
	
	//gauge
	CTexture m_texGau_HP;
	CTexture m_texGau_MP;
	CTexture m_texGau_FP;
	CTexture m_texGau_EXP;
	CTexture m_texGau_EXP_Deco;
	LPDIRECT3DVERTEXBUFFER9 m_pGaugeHP;
	LPDIRECT3DVERTEXBUFFER9 m_pGaugeMP;
	LPDIRECT3DVERTEXBUFFER9 m_pGaugeFP;
	LPDIRECT3DVERTEXBUFFER9 m_pGaugeEXP;
};

#endif // !defined(AFX_WNDTASKBAR_H__A93F3186_63D6_43C1_956F_EC8691E0C7D9__INCLUDED_)

