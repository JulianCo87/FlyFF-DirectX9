// WndArcane.h: interface for the CWndNeuz class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WNDWORLD_H__A93F3186_63D6_43C1_956F_EC8691E0C7D9__INCLUDED_)
#define AFX_WNDWORLD_H__A93F3186_63D6_43C1_956F_EC8691E0C7D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "buff.h"

typedef struct tagCAPTION
{
	TCHAR m_szCaption[ 512 ];
	CD3DFontAPI* m_pFont;

	CTexture m_texture;
	CSize m_size;
	FLOAT m_fXScale;
	FLOAT m_fYScale;
	FLOAT m_fAddScale;
	int m_nAlpha;

} CAPTION,* LPCAPTION;

class CCaption
{
public:
	BOOL m_bEnd;
	CTimer m_timer;
	int m_nCount;
	CPtrArray m_aCaption;
	CCaption();
	~CCaption();

	void RemoveAll();
	void Process();

	void Render( CPoint ptBegin, C2DRender* p2DRender );
	void AddCaption( LPCTSTR lpszCaption, CD3DFontAPI* pFont, BOOL bChatLog = TRUE, DWORD dwColor = D3DCOLOR_ARGB(  255, 255, 255, 255 ) );

    // Initializing and destroying device-dependent objects
    HRESULT InitDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice );
    HRESULT DeleteDeviceObjects();
    HRESULT RestoreDeviceObjects();
    HRESULT InvalidateDeviceObjects();
};

class CCapTime 
{
public:
	CD3DFontAPI* m_pFont;
	CTexture m_texture;
	CSize m_size;

	FLOAT m_fXScale;
	FLOAT m_fYScale;
	FLOAT m_fAddScale;
	int m_nAlpha;
	int m_nTime;
	int m_nStep;
	BOOL m_bRender;
	CCapTime();
	~CCapTime();

	void Process();
	void Render( CPoint ptBegin, C2DRender* p2DRender );
	void SetTime( int nTime, CD3DFontAPI* pFont );
    HRESULT InitDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice );
    HRESULT DeleteDeviceObjects();
    HRESULT RestoreDeviceObjects();
    HRESULT InvalidateDeviceObjects();
};

extern CCapTime g_CapTime;
extern CCaption g_Caption1;
//////////////////////////////////////////////////////////////////////////////////////
// 월드 
//
#define NEXTSKILL_NONE				0xffffffff	// 다음스킬 사용하지 않음
#define NEXTSKILL_ACTIONSLOT		0x7fffffff	// 다음스킬 액션슬롯으로 사용
// 그외는 스킬인덱스.

typedef struct BUFFSKILL
{
	BOOL m_bFlsh;
	int   m_nAlpha;
	CTexture* m_pTexture;
} BUFFSKILL;

typedef	struct	_SET_NAVIGATOR
{
	DWORD dwWorldId;
	D3DXVECTOR3 vPos;
	_SET_NAVIGATOR()
	{
		dwWorldId	= 0;
	}
}
SET_NAVIGATOR;


struct BUFFICON_INFO
{
	CPoint	pt;			// render위치 
	int		nCount;		// 그려진 갯수 
	int		nDelta;		// 이동할 좌표 
};

struct BUFFICONRECT_INFO
{
	RECT    rc;
	DWORD	dwID;
};

// 버튼형식의 도움말( 클릭시 지정된 창 실행 )
#define MAX_ADVBUTTON	10

typedef struct BUTTON_INFO
{
	CWndButton*		m_pwndButton;
	DWORD			m_dwRunWindow;
} BUTTON_INFO, *PBUTTON_INFO;


class CAdvMgr
{


public:

							CAdvMgr( void );
							~CAdvMgr( void );

	void 					Init( CWndBase* pParentWnd );
	void 					AddAdvButton( DWORD dwid, BOOL bForced = FALSE );	
	void					DeleteButton(DWORD dwID);
	BOOL					RunButton( DWORD dwID );
	void 					RemoveButton( void );
	void 					SortButton( void );

	BUTTON_INFO*			FindRunWindowButton( DWORD dwID );

	void					SetVisible( BOOL bVisible );

private:

	int						m_nIndex;
	CWndBase*				m_pParentWnd;
	vector<BUTTON_INFO>		m_vecButton;

};

typedef struct __GUILDRATE
{
	u_long m_uidPlayer;
	int    nLife;
	BOOL   bJoinReady;
} __GUILDRATE;

typedef struct __GCWARSTATE
{
	u_long m_uidPlayer;
	BOOL   m_bWar;
} __GCWARSTATE;

typedef struct __PGUEST_TIME_TEXT
{
	BOOL bFlag;
	int  nState;
	DWORD dwQuestTime;
} __PGUEST_TIME_TEXT;

//sun: 12, 비밀의 방
typedef struct __SRGUILDINFO
{
	DWORD dwGuildId;
	int nWarState;
	int nKillCount;
} __SRGUILDINFO;

typedef struct __KILLCOUNTCIPHERS
{
	BOOL bDrawMyGuildKillCount;
	char szMyGuildKillCount;
	CPoint ptPos;
	float fScaleX;
	float fScaleY;
	int nAlpha;
} __KILLCOUNTCIPHERS;

#define MAX_KILLCOUNT_CIPHERS 3

class EArenaSfxShow;

class CWndWorld : public CWndNeuz
{
	BOOL m_bBGM;
	FLOAT m_fHigh;
	FLOAT m_fLow;
	CPoint m_ptMouseOld;
	CPoint m_ptMouseSpot;
	BOOL m_bFreeMove;
	BOOL m_bFlyMove;
	BOOL m_bSelectTarget;
#ifdef __VRCAMERA
	BOOL m_bCameraMode;
#endif
	BOOL		m_bViewMap;

public:
	enum
	{
		ID_MAINMENU = 7878,
	};

	CAdvMgr		m_AdvMgr;
	CObj*		m_pSelectRenderObj;

	CAdvMgr*	GetAdvMgr() { return &m_AdvMgr; }
	
	BOOL	m_IsMailTexRender;
	BOOL	m_bCtrlInfo;
	BOOL	m_bCtrlPushed;
	BOOL	m_bRenderFPS;

	//sun: 8차 엔젤 소환 Neuz, World, Trans
	BOOL	m_bShiftPushed; //다이스 옮길 때 한번에 넣기.
	BOOL	m_bAngelFinish; //Angel 경험치 완료 유무.

	BOOL	m_bTabPushed;	// tab키 down일때 TRUE(alt+tab문제로 타겟팅을 키업에서 키다운으로 변경)

//sun: 11, 타겟 표시 방법 변경
	CObj*	m_pNextTargetObj;
	CObj*	m_pRenderTargetObj;

	DWORD m_dwIdBgmMusic;

	CTexturePack m_texTarget;		// 지상에서의 4귀퉁이 타겟그림		//sun!
	CTexturePack m_texTargetFly;	// 비행중에서의 4귀퉁이 타겟그림.
	CTexturePack m_texTargetArrow;	// 타겟이 화면을 벗어났을때 화살표방향표시.
	CTexturePack m_texGauFlight;	// 비행 게이지 인터페이스.

	BOOL s_bUped, s_bDowned, s_bLefted, s_bRighted;
	BOOL s_bCombatKeyed, s_bFlyKeyed, s_bAccKeyed, s_bSitKeyed;
	BOOL s_bTempKeyed, m_bTemp2ed, m_bTemp3ed;
	BOOL s_bBoarded;

	CTimer m_timerFocusAttack;
	CTimer m_timerLButtonDown;
	CTimer m_timerRButtonDown;
	//CTimer m_timerSpell;
	DWORD  m_dwPowerTick;				// 마우스를 누른 상태로 힘을 모아 공격하는 패턴에 사용하는 시작시각 
	CTimer m_timerAutoRun;
	CTimer m_timerAutoRunPush;
	CTimer m_timerAutoRunBlock;
	BOOL   m_nDubleUp;
//	CTexture m_texFlaris;

	//CObj* m_pCaptureObj;

	CWndMenu m_wndMenuMover;

	BOOL m_bSetQuestNPCDest;
	D3DXVECTOR3 m_vQuestNPCDest;
	D3DXVECTOR3 m_vDestinationArrow;

	//ect m_rectBound;

	CTexture m_texLvUp;
	CTexture m_texLvDn;
	CTexture m_texLvUp2;
	CTexture m_texLvDn2;
	CTexture m_texGauEmptyNormal;
	CTexture m_texGauFillNormal ;
	LPDIRECT3DVERTEXBUFFER9 m_pVBGauge;

	//CWndTradeGold* m_pWndTradeGold;
	int		m_nMouseMode;		// 비행중일때. 0:우버튼+드래그 빗자루회전   1:걍드래그 빗자루회전.
	
	int		ControlPlayer( DWORD dwMessage, CPoint point );
	int		ControlGround( DWORD dwMessage, CPoint point );
	int		ControlFlying( DWORD dwMessage, CPoint point );
	int		ControlShip( DWORD dwMessage, CPoint point );
	
	//CTexture m_texTargetGauge;
	CModelObject m_meshArrow; // 타겟의 방향을 알려주는 화살표 오브젝트이다.
	
	CModelObject	m_meshArrowWanted; // 타겟의 방향을 알려주는 화살표 오브젝트이다.
	BOOL			m_bRenderArrowWanted;
	DWORD			m_dwRenderArrowTime;
	D3DXVECTOR3		m_v3Dest;
	void			SetRenderArrowWanted( BOOL b, D3DXVECTOR3 vDest ) { m_v3Dest = vDest; m_bRenderArrowWanted = b; m_dwRenderArrowTime = g_tmCurrent; }
	
	void RenderArrow();
	BOOL UseSkillToFocusObj( CCtrl* pFocusObj );
	void GetBoundRect( CObj* pObj, CRect* pRect );

	void RenderArrow_Text( LPDIRECT3DDEVICE9 pDevice, const D3DXVECTOR3& vDest, const D3DXMATRIX& mat );	//gmpbigsun : refactoring
	
	CDWordArray		m_aFlyTarget;

	DWORD	m_dwNextSkill;		// 치고있는중에 스킬치기 예약.
	vector <BUFFICONRECT_INFO> m_rcCheck;

	CTexture		m_TexGuildWinner;
	CTexture		m_TexGuildBest;
#ifdef NEW_GUILD_WINLOG
	CTexture		m_TexGuildHoner;
#endif // NEW_GUILD_WINLOG
	multimap< int, CString >	m_mmapGuildCombat_GuildPrecedence;
	multimap< int, u_long >		m_mmapGuildCombat_PlayerPrecedence;
	vector  < __GUILDRATE >		m_vecGuildCombat_GuildStatus;
	map< u_long, vector<__GCWARSTATE> >  m_mapGC_GuildStatus;	

	CWndBase* m_pWndBuffStatus;		// sun 8차 버프창 관련 적용

public:
	void InitEyeFlash();
	void AddGuildPrecedence( int, CString );
	void AddPlayerPrecedence( int, u_long );
	void AddGuildStatus( u_long uidPlayer, int nLife, BOOL bJoinReady );
	void AddGCStatus( u_long uidDefender, u_long uidPlayer, BOOL bWar );
	u_long GetGCStatusDefender( u_long uidDefender );
	int  IsGCStatusPlayerWar( u_long uidPlayer );
	void ClearGuildPrecedence()   { m_mmapGuildCombat_GuildPrecedence.clear(); }
	void ClearPlayerPrecedence()  { m_mmapGuildCombat_PlayerPrecedence.clear(); }
	void ClearGuildStatus()		  { m_vecGuildCombat_GuildStatus.clear(); }
	void SetViewMap(BOOL bViewMap){ m_bViewMap = bViewMap; }
	BOOL IsViewMap()			  { return m_bViewMap; }
	BOOL GetBuffIconRect( DWORD dwID, const CPoint& point );
	int  GetBuffTimeGap();
	void RenderOptBuffTime( C2DRender *p2DRender, CPoint& point, CTimeSpan &ct, DWORD dwColor );
	void RenderWantedArrow();
	void RenderMoverBuff( CMover* pMover, C2DRender *p2DRender );

	CD3DFontAPI* m_pFontAPICaption;
	CD3DFontAPI* m_pFontAPITitle;
	CD3DFontAPI* m_pFontAPITime;

	CTexturePack	m_texMsgIcon;
	SET_NAVIGATOR	m_stnv;
	CTexturePack	m_texAttrIcon;

	CTexturePack	m_texPlayerDataIcon;		//sun: 11, 메신저창 개선

	CTexture			m_pTextureLogo[CUSTOM_LOGO_MAX];
	DWORD				m_dwOneSecCount;
	ADDSMMODE			m_AddSMMode;
	CTexture*			m_dwSMItemTexture[SM_MAX ];
	CTexture*			m_dwSMResistItemTexture[10];
	BOOL				m_bSMFlsh[SM_MAX];
	int					m_nSMAlpha[SM_MAX];
	CWndGuideSystem*    m_pWndGuideSystem;	

	void UseSkill();
	DWORD m_dwDropTime;
	CTexturePack m_texFontDigital;	// 디지탈 모양의 폰트(비행속도에 사용)
	
	static D3DXVECTOR3 m_vTerrainPoint;
	
	CRect m_rectSelectBegine;
	BOOL m_bNewSelect;
	//CObj* m_pSelectObj;
	BOOL m_bAutoAttack;
	OBJID	m_idLastTarget;
	BOOL	IsLastTarget( OBJID objid )	{	return( m_idLastTarget == objid );	}
	void	SetLastTarget( OBJID objid )	{	m_idLastTarget	= objid;	}
	DWORD	GetNextSkill( void ) { return m_dwNextSkill; }
	void	SetNextSkill( DWORD dwNextSkill )
	{
		m_dwNextSkill = dwNextSkill;
	}

	int		GetGMLogoIndex();

	float	m_fRollAng;		// 롤링 앵글.

	int		n_nMoverSelectCount;
	DWORD	m_dwGuildCombatTime;
	
	__PGUEST_TIME_TEXT m_QuestTime;
	
	char	m_szGuildCombatStr[64];
	struct __GuildCombatJoin
	{
		u_long uidGuild;
		char szJoinGuildName[MAX_NAME];
		int nJoinSize;
		int nOutSize;
	};

	vector< __GuildCombatJoin > m_vecGuildCombatJoin;		//sun: 10, __S_BUG_GC

	vector< __GuildCombatJoin > m_vGuildCombatSort;

	CWndButton m_wndMenu;
	D3DXVECTOR3		m_vTelePos;
	void SetMouseMode( int nMode ) 
	{
		m_nMouseMode = nMode;
		if( nMode == 0 )
			ClipCursor( NULL ); // 커서가 다시 윈도우를 벗어나게 한다.
	}
	int	GetMouseMode( void ) { return m_nMouseMode; }

	void Projection( LPDIRECT3DDEVICE9 pd3dDevice );

	CObj* PickObj( POINT point, BOOL bOnlyNPC = FALSE );
	CObj* SelectObj( POINT point );
	CObj* HighlightObj( POINT point );

	int		m_nSelect;		// 현재 선택된 타겟 인덱스.
	void	ClearFlyTarget( void )
	{
		m_aFlyTarget.RemoveAll();
	}

	void	AddFlyTarget( OBJID idMover )
	{
		m_aFlyTarget.Add( idMover );
	}
	
	OBJID	m_objidTracking;
	void	ClearTracking();

	CWndWorld();
	virtual ~CWndWorld();
	void RenderAltimeter();
//sun: 11, 타겟 표시 방법 변경
	void RenderFocusObj( CObj* pObj, CRect rect, DWORD dwColor1, DWORD dwColor2 );
	void SetNextTarget();

	void RenderFocusArrow( CPoint pt );
	void RenderGauFlight( C2DRender* p2DRender );		// 비행모드시 게이지 인터페이스 Draw

	BOOL						m_bFirstFlying;
	int                         m_nLimitBuffCount;

	CBuffMgr	m_buffs;
	vector< multimap<DWORD, BUFFSKILL> >	m_pBuffTexture;

	DWORD	GetSystemPetTextureKey( IBuff* pBuff );
	//////////////////////////////////////////////////////////////////////////
	WORD	m_wRenderBuffList[ MAX_RENDER_BUFF_COUNT ];
	void	InitRenderBuffList();
	void	InsertRenderBuffList( WORD wRendBuffID );
	bool	IsRenderBuff( WORD wRendBuffID );
	//////////////////////////////////////////////////////////////////////////
	void	RenderBuff( C2DRender* p2DRender );
	void	RenderSelectObj( C2DRender* pRender, CObj* pObj );
	void	RenderSelectObj_TargetWnd( C2DRender* pRender, CObj* pObj );

	void	RenderExpBuffIcon( C2DRender *p2DRender, IBuff* pBuff, BUFFICON_INFO* pInfo, CPoint ptMouse, DWORD dwItemID );
	void	RenderBuffIcon( C2DRender *p2DRender, IBuff* pBuff, BOOL bPlayer, BUFFICON_INFO* pInfo, CPoint ptMouse );
	void	RenderSMBuff( C2DRender *p2DRender, BUFFICON_INFO* pInfo, CPoint ptMouse );
	void	RenderCasting(C2DRender *p2DRender);

	void RenderEventIcon( C2DRender* p2DRender, BUFFICON_INFO* pInfo, CPoint ptMouse );	//sun: 12, 군주

	void PutPetTooltipInfo( FLItemElem* pItemElem, CEditString* pEdit );	//sun: 9차 전승관련 Clienet
	void PutRemainPKPenaltyCount( PT_ITEM_SPEC pItemProp, IBuff* pBuff, CEditString* pEdit );

//sun: 11, 일대일 길드 대전
	void DrawGuildCombat1to1Info(C2DRender *p2DRender);
	void DrawGuildCombat1to1PlayerInfo(C2DRender *p2DRender);
	void DrawGuildCombat1ot1GuildInfo(C2DRender *p2DRender);
	BOOL m_bGuildCombat1to1Wait;

//sun: 12, 비밀의 방
	char m_szSecretRoomStr[256];
	vector<__SRGUILDINFO> m_vecGuildList;
	__KILLCOUNTCIPHERS m_stKillCountCiphers[MAX_KILLCOUNT_CIPHERS];
	BOOL m_bFlashBackground;

	//gmpbigsun(20110217): 텍스쳐 효과
	void RenderEffect_StageStart( C2DRender* p2DRender );
	float m_fEffectTime_StateStart;
	CTexture* m_pTextureStageStart;

	void DrawSecretRoomGuildInfo(C2DRender *p2DRender, BOOL bIsMyGuild, int nRank, __SRGUILDINFO stGuildInfo, CPoint ptRank, CPoint ptLogo, 
		CPoint ptGName, CPoint ptHypoon, CPoint ptState, float fLogoScaleX, float fLogoScaleY, CRect rectBg);
	void DrawSecretRoomInfo(C2DRender *p2DRender);
	void DrawMyGuildKillCount(C2DRender *p2DRender, __SRGUILDINFO stGuildInfo, CPoint ptState, int nMax);
	void DrawOutLineLamp(C2DRender *p2DRender, CRect rectBg, DWORD dwColorstart, DWORD dwColorend, int nState, int nRank, BOOL bIsMyGuild);
	void DrawOutLineFlash(C2DRender *p2DRender, CRect rectBg, DWORD dwColorstart, DWORD dwColorend);

	void InviteParty( u_long uidPlayer );
	void InviteCompany( OBJID objId );

	virtual void OnDraw(C2DRender* p2DRender); 
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL,DWORD dwWndId = 0);
	// message
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);
	virtual BOOL OnSetCursor( CWndBase* pWndBase, UINT nHitTest, UINT message );
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void OnRButtonDown(UINT nFlags, CPoint point);
	virtual void OnMButtonUp(UINT nFlags, CPoint point);
	virtual void OnMButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point);
	virtual void OnMButtonDblClk( UINT nFlags, CPoint point);
	virtual void OnRButtonDblClk( UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnMouseWheel( UINT nFlags, short zDelta, CPoint pt );
	virtual void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual	BOOL Process();
	virtual BOOL OnEraseBkgnd(C2DRender* p2DRender);
	virtual LRESULT WndMsgProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point = 0 );
	virtual	void SetWndRect( CRect rectWnd, BOOL bOnSize = TRUE);
	virtual void OnMouseWndSurface( CPoint point );
	
	BOOL UseFocusObj( CCtrl* pFocusObj );

	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();

public:
	void BeginEArenaShow( );
	EArenaSfxShow* m_pEAShow;

private:
	void	ShowMoverMenu( CMover* pTarget );
	int		GetGaugePower( int* pnValue );

	void	ShowCCtrlMenu( CCtrl* pCCtrl );		
	BOOL MenuException( CPoint point );	// 메뉴를 띄우기 전에 처리해 줘야 할 예외 사항들

	bool	CheckOpenedUI( UINT nUIID );
	void	CloseUIWhenCharacterMove( void );

public:
	CWndButton m_wndMainMenu;	//v19
	void	ShowMainMenu( BOOL bShow );

	void	SetNextActionSkill( );	//prefectoring
};
#endif // !defined(AFX_WNDFIELD_H__A93F3186_63D6_43C1_956F_EC8691E0C7D9__INCLUDED_)
