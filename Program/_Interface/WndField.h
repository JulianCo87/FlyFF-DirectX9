#if !defined(AFX_WNDFIELD_H__A93F3186_63D6_43C1_956F_EC8691E0C7D9__INCLUDED_)
#define AFX_WNDFIELD_H__A93F3186_63D6_43C1_956F_EC8691E0C7D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "post.h"
#include "guild.h"

//sun: 13, Ŀ�� �ý���
#include "couple.h"
#include "couplehelper.h"

extern 	int g_nSkillCurSelect;
const int MAX_WANTED_LIST =	100;

#include "honor.h"	//sun: 13, ����

#include "WndGold.h"
#include "WndControl_Tree.h"
#include "WndMailRequestingBox.h"
#include "../Neuz/FLExchangeEvent.h"
//////////////////////////////////////////////////////////////////////////////////////
// ���÷� ���� 
//
class CWndApplet : public CWndNeuz
{
public:
	CWndListBox m_wndList;
	CWndButton m_wndButton1;
	CWndButton m_wndButton2;
	CWndButton m_wndButton3;

	CWndApplet();
	virtual ~CWndApplet();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL,DWORD dwWndId = 0);
	// message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase = NULL );
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);

};

//////////////////////////////////////////////////////////////////////////////////////
// ��Ӿ����� 
//
class CWndDropItem : public CWndNeuz 
{ 
public: 
	FLItemElem* m_pItemElem;
	D3DXVECTOR3 m_vPos;
	CWndEdit* m_pEdit;

	CWndDropItem(); 
	virtual ~CWndDropItem(); 
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual BOOL Process( void );
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
}; 

class CWndDropConfirm : public CWndNeuz 
{ 
public: 
	FLItemElem* m_pItemElem;
	D3DXVECTOR3 m_vPos;
	
	CWndDropConfirm(); 
	virtual ~CWndDropConfirm(); 
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual BOOL Process( void );
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
}; 

class CWndRandomScrollConfirm : public CWndNeuz 
{ 
public:
	BOOL bFlag;
	OBJID objid, objid1;
	void SetItem( OBJID objidBuf, OBJID objid1Buf, BOOL bFlagBuf = FALSE );
public: 
	CWndRandomScrollConfirm(); 
	virtual ~CWndRandomScrollConfirm(); 
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
}; 

class CWndQuestItemWarning : public CWndNeuz 
{ 
public: 
	FLItemElem* m_pItemElem;
	D3DXVECTOR3 m_vPos;

	CWndQuestItemWarning(); 
	virtual ~CWndQuestItemWarning(); 
	
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
//  ����Ʈ ������ ���� â
//
class CWndQuestItemInfo : public CWndNeuz
{
public:
	CWndQuestItemInfo();
	virtual ~CWndQuestItemInfo();

	FLItemBase*	m_pItemBase;
	CPoint		m_Position;

	inline void SetItemBase(FLItemBase* pItemBase = NULL) { m_pItemBase = pItemBase; };
	
	BOOL Create(FLItemBase* pItemBase, UINT nID, CWndBase* pWndParent = NULL );
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, FLItemBase* pItemBase = NULL); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
};

class CWndRemoveJewelConfirm;		//sun: 9�� ���°��� Clienet
class CWndConfirmBuy;

//---------------------------------------------------------------------------------------------
// ������ �κ��丮
//---------------------------------------------------------------------------------------------
class CWndInventory		: public CWndNeuz
{


public:

		
								CWndInventory();
	virtual						~CWndInventory();

	virtual BOOL				Initialize(CWndBase* pWndParent = NULL,DWORD dwWndId = 0);
	virtual BOOL				Process ( void );

	void						DoUnSeal( void );	
	void						CancelUnSeal( void );
	void						UpDateModel( void );
	void						UpdateParts( void );
	void						UpdateTooltip( void );		//sun: 9�� ���°��� Clienet// _jin 
	void						BaseMouseCursor( void );
	void						ChangeMouseCursor( void );
	void						RunUpgrade( FLItemBase* pItem );

	CModelObject*				GetModel( void )						{	return m_pModel;			}
	CWndTabCtrl*				GetTabCtrl( void )						{	return m_pTabCtrl;			}

	
	const FLItemBase*			GetUnSealKeyItem( void )				{	return m_pUnSealKeyItem;	}
	void						SetUnSealItem( FLItemBase* pItem )		{	m_pUnSealItem = pItem;		}

	// message
	virtual	void				OnInitialUpdate( void );
	virtual void				OnDraw(C2DRender* p2DRender);
	virtual void 				OnDestroy( void );
	virtual void 				OnDestroyChildWnd( CWndBase* pWndChild );
	virtual void 				OnLButtonUp(UINT nFlags, CPoint point);
	virtual void 				OnLButtonDown(UINT nFlags, CPoint point);
	virtual void 				OnLButtonDblClk( UINT nFlags, CPoint point);
	virtual void 				OnRButtonDown(UINT nFlags, CPoint point);
	virtual void 				OnMouseMove(UINT nFlags, CPoint point);
	virtual void				OnMouseWndSurface( CPoint point );
	virtual void 				OnSize(UINT nType, int cx, int cy);
	virtual BOOL 				OnSetCursor ( CWndBase* pWndBase, UINT nHitTest, UINT message );
	virtual BOOL 				OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);
	virtual BOOL 				OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase = NULL );
	virtual BOOL 				OnDropIcon( LPSHORTCUT pShortcut, CPoint point = 0 );


	//gmpbigsun( 20110325 ) : �������̽� ���� ( CT_NEW_INVENTORY )
	void						CISetDieFlag_ItemCtrl( const BOOL bFlag );		//CI: combine interface


protected:


	BOOL						ExcuteItem( FLItemBase* pItem );


private:


	CModelObject*				m_pModel;
	CWndTabCtrl*				m_pTabCtrl;
	FLItemElem*					m_pSelectItem;
	CTexture*					m_TexRemoveItem;
#ifdef INVENTORY_ITEM_ALIGN
	CTexture*					m_TexAlignItem;
#endif	// INVENTORY_ITEM_ALIGN
	FLItemBase*					m_pUnSealKeyItem;
	FLItemBase*					m_pUnSealItem;

	FLOAT						m_fRot;
	CPoint						m_OldPos;

	BOOL						m_bReport;
	BOOL						m_bLButtonDownRot;
	
	CRect						m_InvenRect[MAX_HUMAN_PARTS];
	CModelObject				m_meshStage;
	

public:
	

	CSfx*						m_pSfxUpgrade;
	FLItemBase*					m_pUpgradeItem;
	FLItemBase*					m_pUpgradeMaterialItem;
	CWndConfirmBuy*				m_pWndConfirmBuy;

	BOOL						m_bIsUpgradeMode;
	DWORD						m_dwEnchantWaitTime;

	CWndGold					m_wndGold;


	//sun: 9�� ���°��� Clienet
	BOOL						m_bRemoveJewel;
	CWndRemoveJewelConfirm*		m_pWndRemoveJewelConfirm;


	CWndItemCtrl				m_wndItemCtrl;

	//_JIN_NEW_INVENTORY
	CWndItemCtrl				m_wndItemCtrl_[MAX_VENDOR_NEW_INVENTAB];

};



class CWndQueryEquip : public CWndNeuz
{
	CPoint				m_OldPos;
	FLOAT				m_fRot;
	CModelObject*		m_pModel;
	BOOL				m_bLButtonDownRot;
	DWORD		 m_ObjID;
	CRect		 m_InvenRect[MAX_HUMAN_PARTS];

//	EQUIP_INFO_ADD	m_aEquipInfoAdd[MAX_HUMAN_PARTS];
	FLItemElem	m_aEquipInfoAdd[MAX_HUMAN_PARTS];

public:

	CMover* GetMover() 
	{ 
		if( m_ObjID != NULL_ID )
			return prj.GetMover( m_ObjID ); 

		return NULL;
	}
	void	SetMover( DWORD		 ObjID );
//	void	SetEquipInfoAdd( EQUIP_INFO_ADD* aEquipInfoAdd );
	void	SetEquipInfoAdd( FLItemElem* aEquipInfoAdd );
	
	CWndQueryEquip();
	virtual ~CWndQueryEquip();
	virtual BOOL Process ();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual void OnMouseWndSurface( CPoint point );
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL,DWORD dwWndId = 0);
	virtual void OnMouseMove (UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
};


//////////////////////////////////////////////////////////////////////////////////////
// ���� 
//
class CWndStateConfirm : public CWndNeuz 
{ 
public:
	UINT m_nId;
	void OnSetState( UINT nId );
	void SendYes( void );
public: 
	CWndStateConfirm(); 
	virtual ~CWndStateConfirm(); 
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
}; 
class CWndCharacterBase : public CWndBase
{
	CTexture m_txInfo;

public:
	CWndCharacterBase();
	virtual ~CWndCharacterBase();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL,DWORD dwWndId = 0);
	// message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase = NULL );
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
};

class CWndCharacterDetail : public CWndBase
{
	CWndButton m_wndStr;
	CWndButton m_wndSta;
	CWndButton m_wndDex;
	CWndButton m_wndInt;
	
	int			m_nCnt;

public:
	BOOL	m_fWaitingConfirm;
	CWndCharacterDetail();
	virtual ~CWndCharacterDetail();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL,DWORD dwWndId = 0);
	// message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase = NULL );
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnMouseWndSurface( CPoint point );

protected:
	void RenderATK( C2DRender* p2DRender, int x, int y );
};

//sun: 9�� ����â ���� ���� Neuz
class CWndCharacterDetail2 : public CWndBase
{
public:
	CWndButton m_wndStrPlus, m_wndStrMinus;
	CWndButton m_wndStaPlus, m_wndStaMinus;
	CWndButton m_wndDexPlus, m_wndDexMinus;
	CWndButton m_wndIntPlus, m_wndIntMinus;
	
	CWndButton m_wndApply, m_wndReset;
	
	CWndEdit m_editStrCount;
	CWndEdit m_editStaCount;
	CWndEdit m_editDexCount;
	CWndEdit m_editIntCount;

	int m_nStrCount;
	int m_nStaCount;
	int m_nDexCount;
	int m_nIntCount;

	int m_nGpPoint;

	int m_nATK;
	int m_nDEF;
	int m_nCritical;
	int m_nATKSpeed;
	
public:
	CWndCharacterDetail2();
	virtual ~CWndCharacterDetail2();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL,DWORD dwWndId = 0);
	// message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase = NULL );
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnMouseWndSurface( CPoint point );
	virtual BOOL Process ();
	
	void RefreshStatPoint();
	void GetVirtualATK(int* pnMin, int* pnMax);
	int GetVirtualDEF();
	int GetVirtualCritical();
	float GetVirtualATKSpeed();

protected:
	void RenderATK( C2DRender* p2DRender, int x, int y );
};

class CWndChangeJob;

class CWndPvpBase : public CWndBase
{
	CTexture m_txInfo;
	
public:
	CWndButton m_wndChangeJob;
	CWndChangeJob* m_pWndChangeJob;
	BOOL	m_fWaitingConfirm;
	BOOL m_bExpert;
	int m_nDisplay;			// ����â ȭ�� �ɼ�

	CWndPvpBase();
	virtual ~CWndPvpBase();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL,DWORD dwWndId = 0);
	// message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase = NULL );
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
};

class CWndChangeJob : public CWndNeuz
{
public:
	int m_nJob;
	CWndButton m_wndExpert[ 6 ];
	CWndButton m_wndOk;
	CWndButton m_wndCancel;

	CWndChangeJob( int nJob );
	virtual ~CWndChangeJob();
	virtual void OnDraw( C2DRender* p2DRender );
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize( CWndBase* pWndParent = NULL,DWORD dwWndId = 0 );
	// message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase = NULL );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnLButtonUp( UINT nFlags, CPoint point );
	virtual void OnLButtonDown( UINT nFlags, CPoint point );
};

//sun: 13, ĳ��������â ������
class CWndCharInfo : public CWndBase
{

public:

	CWndChangeJob*			m_pWndChangeJob;
	BOOL					m_fWaitingConfirm;
	BOOL					m_bExpert;
	int						m_nDisplay;

	int 					m_nStrCount;
	int 					m_nStaCount;
	int 					m_nDexCount;
	int 					m_nIntCount;

	int 					m_nGpPoint;

	int 					m_nATK;
	int 					m_nDEF;
	int 					m_nCritical;
	int 					m_nATKSpeed;

	DWORD					m_dwPropertyNameColor;
	DWORD					m_dwPropertyNumberColor;
	DWORD					m_dwIncreasePropertyNumberColor;
	DWORD					m_dwIncreaseAbilityNumberColor;
	DWORD					m_dwDecreaseAbilityNumberColor;

	CWndButton				m_wndChangeJob;
	CWndButton				m_wndApply;
	CWndButton				m_wndReset;

	CWndButton 				m_wndStrPlus;
	CWndButton 				m_wndStaPlus;
	CWndButton 				m_wndDexPlus;
	CWndButton 				m_wndIntPlus;
	CWndButton 				m_wndStrMinus;
	CWndButton 				m_wndStaMinus;
	CWndButton 				m_wndDexMinus;
	CWndButton 				m_wndIntMinus;
	
	CWndEdit 				m_editStrCount;
	CWndEdit 				m_editStaCount;
	CWndEdit 				m_editDexCount;
	CWndEdit 				m_editIntCount;

	Stat					m_RenderStat;

public:

	CWndCharInfo();
	virtual ~CWndCharInfo();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL,DWORD dwWndId = 0);
	// message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase = NULL );
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnMouseWndSurface( CPoint point );
	virtual	void PaintFrame( C2DRender* p2DRender );
	virtual BOOL Process ();
	void RefreshStatPoint();
	void GetVirtualATK(int* pnMin, int* pnMax);
	int GetVirtualDEF();
	int GetVirtualCritical();
	float GetVirtualATKSpeed();

protected:
	void RenderATK( C2DRender* p2DRender, int x, int y );
};

class CWndHonor : public CWndNeuz
{
private:
	int						m_nSelectedId;
	int						m_nCount;
	vector<EarnedTitle>		m_vecTitle;	
public:

	CWndHonor();
	virtual ~CWndHonor();
	virtual void OnDraw( C2DRender* p2DRender );
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize( CWndBase* pWndParent = NULL,DWORD dwWndId = 0 );
	// message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase = NULL );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnLButtonUp( UINT nFlags, CPoint point );
	virtual void OnLButtonDown( UINT nFlags, CPoint point );
	void RefreshList();

};

class CWndCharacter : public CWndNeuz
{
public:
//sun: 13, ĳ��������â ������
	CWndCharInfo		m_wndCharInfo;
	CWndHonor			m_wndHonor;

//	CWndCharacterBase   m_wndStateBase;	
//	CWndPvpBase         m_wndPvp;

public:
	CWndCharacter();
	virtual ~CWndCharacter();
	virtual void SerializeRegInfo( CAr& ar, DWORD& dwVersion );
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL,DWORD dwWndId = 0);
	// message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase = NULL );
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
};


class CWndReSkillControl : public CWndNeuz
{
public:
	CWndReSkillControl();
	virtual ~CWndReSkillControl();
	virtual void OnDraw( C2DRender* p2DRender );
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize( CWndBase* pWndParent = NULL,DWORD dwWndId = 0 );
	// message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase = NULL );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnLButtonUp( UINT nFlags, CPoint point );
	virtual void OnLButtonDown( UINT nFlags, CPoint point );
};

class CWndReSkillWarning : public CWndNeuz
{
	BOOL	m_bParentDestroy;
public:
	CWndReSkillWarning();
	virtual ~CWndReSkillWarning();

	void InitItem( BOOL bParentDestroy );
	virtual void OnDraw( C2DRender* p2DRender );
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize( CWndBase* pWndParent = NULL,DWORD dwWndId = 0 );
	// message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase = NULL );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnLButtonUp( UINT nFlags, CPoint point );
	virtual void OnLButtonDown( UINT nFlags, CPoint point );
	virtual void OnDestroy();
};



//////////////////////////////////////////////////////////////////////////////////////
// ��ų EX 
//
class CWndSkillTreeEx : public CWndNeuz
{
protected:
	int			  m_nCount;					//���� ������ �ش��ϴ� ��ų �� ����

	int           m_nCurSelect  ;			//�� �Ⱦ����ִµ�?
	BOOL          m_bDrag;					//���콺�� Ŭ���ߴµ� ��ų������ ���� �� �� ��� TRUE
	LPSKILL       m_pFocusItem;				//���õ� ������ : �� ��ų�� ����Ŭ���ϰų� �ؼ� ������ �ø����ִ� ���·� ���� ��ų������
	int				m_nCurrSkillPoint;		//���� ���� ��ų����Ʈ
	CTexturePack	m_textPackNum;			//��ų �������� ǥ�ÿ� �׸� ( 1, 2,..... max )
	DWORD			m_dwMouseSkill;			//���콺�� ��ġ�� ��ų
	CWndButton*		m_pWndButton[4];		//+, -, reset, finish
	LPSKILL       m_apSkill;				//��ų ���
	CTexture*     m_atexSkill[ MAX_SKILL_JOB ];
	CTexture*	  m_aSkillLevel[ 3 ];		//�̰Ÿ��� ������ �ʰ�....gmpbigsun
	//CTexture*     m_atexJobPannel[ 2 ];
	LPIMAGE       m_atexJobPannel[ 2 ];
	CString		  m_strHeroSkilBg;			//����� �̹��� ���� �̸�		//sun: 9�� ���°��� Clienet
	int           m_nJob         ;			//class ��ȣ
	
	CTexture m_texGauEmptyNormal ;
	CTexture m_texGauFillNormal  ;
	
	LPDIRECT3DVERTEXBUFFER9 m_pVBGauge;

	CTexture*     m_atexTopDown[2];
	int 	m_nTopDownGap;

	BOOL	m_bSlot[4];		//sun: 9�� ���°��� Clienet

//sun: 9�� ���°��� Clienet
	BOOL m_bLegend;							//����
	CWndStatic* m_pWndHeroStatic[2];

public:
	BOOL	IsReSkillPointDone();
	int		GetCurrSkillPoint() { return m_nCurrSkillPoint; }
	void	SubSkillPointDown( LPSKILL lpSkill );
	LPSKILL GetSkill() { return m_apSkill; }
	BOOL	IsDownPoint(DWORD skill);
	int		GetCalcImagePos( int nIndex );
	int		GetCurSelect() { return g_nSkillCurSelect; }
	BOOL	GetSkillPoint(DWORD dwSkillID, CRect& rect );
	LPSKILL GetSkill( int i );
	LPSKILL GetdwSkill( DWORD dwSkill );
	BOOL	CheckSkill( int i );
	void LoadTextureSkillicon();
	void InitItem( int nJob, LPSKILL apSkill, BOOL bReset = FALSE );
	CWndSkillTreeEx();
	virtual ~CWndSkillTreeEx();

	void AfterSkinTexture( LPWORD pDest, CSize size, D3DFORMAT d3dFormat = D3DFMT_A4R4G4B4 );
	void SetJob( int nJob );
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual BOOL Process ();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL,DWORD dwWndId = 0);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnMouseWndSurface( CPoint point );
	// message
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase = NULL );
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point);
	virtual void OnRButtonDblClk( UINT nFlags, CPoint point);
	void SetActiveSlot( int nSlot, BOOL bFlag );
	virtual void SerializeRegInfo ( CAr& ar, DWORD& dwVersion );
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();	
};



//////////////////////////////////////////////////////////////////////////////////////
// CWndStatus
//

class CWndStatus : public CWndNeuz
{
	BOOL m_bHPVisible;
	BOOL m_bExpVisible;
public:
	CModel* m_pModel;
	int		m_nDisplay;
public:
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	
	CTexture m_texGauEmptyNormal ;
	CTexture m_texGauEmptySmall  ;
	CTexture m_texGauFillNormal  ;
	CTexture m_texGauFillSmall   ;
	
	LPDIRECT3DVERTEXBUFFER9 m_pVBHPGauge;
	LPDIRECT3DVERTEXBUFFER9 m_pVBMPGauge;
	LPDIRECT3DVERTEXBUFFER9 m_pVBFPGauge;
	LPDIRECT3DVERTEXBUFFER9 m_pVBEXPGauge;
	
	BOOL m_bVBHPGauge;
	BOOL m_bVBMPGauge;
	BOOL m_bVBFPGauge;
	BOOL m_bVBEXPGauge;

	BOOL m_bShowTip_AEXP;
	BOOL m_bVBAEXPGauge;
	LPDIRECT3DVERTEXBUFFER9 m_pVBAEXPGauge;
	
	int m_nHPWidth ;
	int m_nMPWidth ;
	int m_nFPWidth ;
	int m_nEXPWidth;
	int m_nPXPWidth;
	
	void MakeGaugeVertex();
	
	CWndStatus(); 
	virtual ~CWndStatus();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL,DWORD dwWndId = 0);
	// message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase = NULL );
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnEraseBkgnd(C2DRender* p2DRender);
	virtual	void SetWndRect( CRect rectWnd, BOOL bOnSize = TRUE );
	virtual	void PaintFrame( C2DRender* p2DRender );

	virtual void OnMouseWndSurface( CPoint point );
};

class CWndQuit : public CWndMessageBox
{
	BOOL	m_bFlag;	
public:
	CWndQuit() { }
	virtual ~CWndQuit() { }
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD dwWndId = 0 );
	// message
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
};	
class CWndLogOut : public CWndMessageBox
{
public:
	CWndLogOut(LPCTSTR  lpszMessage = "" ,int nType = LOGOUT_TYPE1);
	virtual ~CWndLogOut();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	// message
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	enum LOGOUT
	{
		LOGOUT_TYPE1,//�α׾ƿ� �޼����� �����ΰ��
		LOGOUT_TYPE2,//�α׾ƿ� �޼����� ����ɰ�� 
	};
private:
	int m_nType; 
	LPCTSTR m_lpszMessage;
};	


class CWndLogOutWaitting : public CWndNeuz
{
	BOOL  m_bIsLogOut;
	DWORD dwTime;
public:
	enum
	{
		MSGTYPE_NONE,
		MSGTYPE_SHUTDOWN,
	};
	CWndLogOutWaitting();
	virtual ~CWndLogOutWaitting();
	void	SetIsLogOut(BOOL bFlag) { m_bIsLogOut = bFlag; }
	virtual BOOL Process ();
	virtual BOOL Initialize( CWndBase* pWndParent );
	virtual	void OnInitialUpdate();
	// message
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 

public:
	int m_nMsgType;
};	

//DECLARE_WNDMESSAGEBOX( CWndLogOut    ) // �α׾ƿ� ó���ϴ� �޽��� �ڽ� Ŭ���� 

//BEGIN_WNDCLASS( CWndMacro )
//END_WNDCLASS

//BEGIN_WNDCLASS( CWndStyle )
//END_WNDCLASS

class CWndTradeGold : public CWndNeuz 
{ 
public: 
	UINT m_nIdWndTo;
	SHORTCUT m_Shortcut;
	D3DXVECTOR3 m_vPos;
	DWORD m_dwGold;
	CWndBase* m_pWndBase;
	CWndStatic* pStatic;
	CWndStatic* pStaticCount;
//sun: 11, �ָӴ�
	char m_nSlot;
	char m_nPutSlot;

	CWndTradeGold(); 
	virtual ~CWndTradeGold(); 

	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );
	virtual BOOL Process( void );
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
};

class CWndConfirmTrade : public CWndNeuz 
{ 
	OBJID m_objid;
public: 
	CWndConfirmTrade(); 
	virtual ~CWndConfirmTrade(); 

	void OnSetName( const char* szName, OBJID objid );
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
}; 

class CWndTradeConfirm : public CWndNeuz 
{ 
public: 
	BOOL bMsg;
	CWndTradeConfirm(); 
	virtual ~CWndTradeConfirm(); 
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
}; 

BEGIN_WNDCLASS( CWndTrade )

	CWndTradeCtrl m_wndItemCtrlYou;
	CWndTradeCtrl m_wndItemCtrlI  ;
	CWndStatic    m_wndGold       ;
	int           m_nGoldI, m_nGoldYou;

	void DoCancel();

END_WNDCLASS

//////////

BEGIN_WNDCLASS( CWndEmotion )
	CWndTreeCtrl  m_wndViewCtrl;
	CWndEdit      m_wndEdit;
	CWndButton    m_wndDefault;
	LPTREEELEM    m_lpSelectTreeElem;
END_WNDCLASS


class CWndRevival : public CWndNeuz 
{ 
public: 
	CWndButton* m_pLodeLight;
	CWndButton* m_pLodeStar;
	CWndButton* m_pRevival;
	CWndButton* m_pShop;		//sun: 9�� �߰���

	CWndButton* m_pReChallenge;		//gmpbigsun : �ݷμ��� �絵��
	BOOL m_bColosseumAllDie;
	float m_fTime;

	CWndRevival(); 
	virtual ~CWndRevival(); 

	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 

	virtual	BOOL Process();
	
	void	EnableButton( int nButton, BOOL bEnable );
	void	SetRemainTime( float fTime );
}; 

class CWndReWanted : public CWndNeuz 
{ 
public: 
	CWndReWanted(); 
	virtual ~CWndReWanted(); 

private:
	CString      m_strWanted;
	
public: 
	BOOL         CheckWantedInfo( int nGold, LPCTSTR szMsg );
	void		 SetWantedName( LPCTSTR szWanted );

	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
}; 

class CWndResurrectionConfirm : public CWndNeuz 
{ 
public: 
	CWndResurrectionConfirm(); 
	virtual ~CWndResurrectionConfirm(); 
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
}; 

class CWndWantedConfirm : public CWndNeuz 
{ 
	int		 m_nGold;
	char	 m_szName[ MAX_NAME ];
public: 
	CWndWantedConfirm(); 
	virtual ~CWndWantedConfirm(); 
	
	void		 SetInfo( const char szName[], const int nGold );
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
}; 


struct WANTEDLIST
{
	char	 szName[ MAX_NAME ];		// �̸�.
	__int64	 nGold;						// �����
	char     szDate[32];
	char	 szMsg[WANTED_MSG_MAX + 1];	// ������ 
};

class CWndWanted : public CWndNeuz 
{ 
private: 
	CWndWantedConfirm*  m_pWantedConfirm;
	time_t				m_recvTime;
	CWndScrollBar		m_wndScrollBar;
	int					m_nMax;
	WANTEDLIST			m_aList[MAX_WANTED_LIST];
	int					m_nSelect;

public: 
	CWndWanted(); 
	virtual ~CWndWanted(); 
	
	int          GetSelectIndex( const CPoint& point );	
	void		 Init( time_t lTime );
	void		 InsertWanted( const char szName[], __int64 nGold,  int nTime, const char szMsg[] );	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
	virtual	void OnRButtonUp( UINT nFlags, CPoint point );
	virtual void OnRButtonDown( UINT nFlags, CPoint point );
	virtual void OnMouseMove(UINT nFlags, CPoint point );
	virtual BOOL OnMouseWheel( UINT nFlags, short zDelta, CPoint pt );
	virtual void OnMouseWndSurface( CPoint point );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point);	
}; 


class CWndCommItemDlg : public CWndNeuz 
{ 
public: 
	CWndEdit*		m_pWndEdit;
	DWORD			m_dwObjId;
	DWORD			m_dwCtrlId;
	void			SetItem( DWORD dwDefindText, DWORD dwObjId, DWORD dwCtrlId );
	CWndCommItemDlg(); 
	virtual ~CWndCommItemDlg(); 
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
}; 

class CWndChangeClass1 : public CWndNeuz 
{ 
public: 
	int			nJob;
	void		SetJob();

	CWndChangeClass1(); 
	virtual ~CWndChangeClass1(); 
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
}; 

class CWndChangeClass2 : public CWndNeuz 
{ 
public: 
	int			nJob;
	void		SetJob();
	
	CWndChangeClass2(); 
	virtual ~CWndChangeClass2(); 
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
}; 

class CWndPostSendConfirm;

class CWndPostSend : public CWndNeuz
{
	int		m_nCost;
	int		m_nCount;
	BYTE    m_nItem;

public:	
	void ClearData();
	CWndPostSend(); 
	virtual ~CWndPostSend(); 
	
	
	BYTE GetItemId() { return m_nItem; }
	void SetItemId( BYTE nId );
	void SetCost( int nCost ) { m_nCost = nCost; }
	void SetCount( int nCount ) { m_nCount = nCount; }
	void SetReceive( char* pchar );
	void SetTitle( char* pchar );	
	void SetText( char* pchar );	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );		
	virtual void OnRButtonUp( UINT nFlags, CPoint point );
	virtual void AdditionalSkinTexture( LPWORD pDest, CSize sizeSurface, D3DFORMAT d3dFormat = D3DFMT_A4R4G4B4 );
};

class CWndPostItemWarning : public CWndNeuz 
{ 
public: 
	int	   m_nMailIndex;
	
	CWndPostItemWarning(); 
	virtual ~CWndPostItemWarning(); 
	
	void	SetIndex( int nIndex ) { m_nMailIndex = nIndex; }
	void	SetString( char* string );
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
}; 

class CWndPostDeleteConfirm;
class CWndPostRead : public CWndNeuz
{
	int					m_nMailIndex;
	CWndGold			m_wndGold;
	BOOL				m_bDrag;
	CWndPostDeleteConfirm* m_pDeleteConfirm;
	
public:	
	BOOL				m_bRequested;		//by gmpbigsun( 20120118 ) : �б� ����ȭ����, ~�� ��û�ߴ�!
	CWndPostItemWarning*		m_pWndPostItemWarning;
	void MailReceiveItem();
	void MailReceiveGold();
	void ClearData();
	CWndPostRead(); 
	virtual ~CWndPostRead(); 
	
	void SetValue( int nMailIndex );
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
	virtual void OnMouseMove(UINT nFlags, CPoint point );	
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );		
	virtual void OnRButtonUp( UINT nFlags, CPoint point );
};


class CWndPostReceive : public CWndNeuz
{
	CTexture			m_Texture[3];
	CWndScrollBar		m_wndScrollBar;
	int					m_nSelect;
	int					m_nMax;
	CWndPostRead*		m_wndPostRead;
public:	
	void UpdateScroll();
	
	CWndPostReceive(); 
	virtual ~CWndPostReceive(); 

	int          GetSelectIndex( const CPoint& point );	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
	virtual BOOL OnMouseWheel( UINT nFlags, short zDelta, CPoint pt );
	virtual	void OnLButtonDblClk( UINT nFlags, CPoint point );
	virtual HRESULT DeleteDeviceObjects();
	virtual void OnMouseWndSurface( CPoint point );
};
class CWndPost : public CWndNeuz
{
public:
	CWndPostSend		m_PostTabSend;
	CWndPostReceive		m_PostTabReceive;
	
	CWndPost(); 
	virtual ~CWndPost(); 
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 

public:
	void LoadLastMailBox( void );
	void SaveLastMailBox( void );
	void CloseMailRequestingBox( void );

	void EnableWindow_Receive( BOOL bEnable );

private:
	CWndMailRequestingBox* m_pWndMailRequestingBox;
};
class CWndPostSendConfirm : public CWndNeuz
{
protected:
	CWndText	m_wndText;
	
	BYTE		m_nItem;
	int			m_nItemNum;
	int			m_nGold;
	char		m_szReceiver[MAX_NAME];
	char		m_szTitle[MAX_MAILTITLE];	
	char		m_szText[MAX_MAILTEXT];		
public: 
	CWndPostSendConfirm();
	virtual ~CWndPostSendConfirm();
	
	void			SetValue( DWORD dwItemObjID, int nItemNum, char* lpszReceiver, int nGold, char* lpszTitle, char* lpszText );
	virtual	BOOL	Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK );
	virtual	BOOL	OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );
	virtual	void	OnDraw( C2DRender* p2DRender );
	virtual	void	OnInitialUpdate();
	virtual	BOOL	OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual	void	OnSize( UINT nType, int cx, int cy );
	virtual void	OnLButtonUp( UINT nFlags, CPoint point );
	virtual	void	OnLButtonDown( UINT nFlags, CPoint point );
}; 

class CWndPostDeleteConfirm : public CWndNeuz
{
protected:
	CWndText	m_wndText;
	int		m_nIndex;
public: 
	CWndPostDeleteConfirm();
	virtual ~CWndPostDeleteConfirm();
	
	void			SetValue( int nIndex ) { m_nIndex = nIndex; }
	virtual	BOOL	Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK );
	virtual	BOOL	OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );
	virtual	void	OnDraw( C2DRender* p2DRender );
	virtual	void	OnInitialUpdate();
	virtual	BOOL	OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual	void	OnSize( UINT nType, int cx, int cy );
	virtual void	OnLButtonUp( UINT nFlags, CPoint point );
	virtual	void	OnLButtonDown( UINT nFlags, CPoint point );
}; 

struct GUILDLIST
{
	char	 szGuild[ MAX_NAME ];		// �̸�.
	char	 szName[ MAX_NAME ];		// �̸�.
	int		 nNum;
};

class CWndGuildCombatOffer : public CWndNeuz
{
protected:
	DWORD			m_dwReqGold;
	DWORD			m_dwMinGold;
	DWORD			m_dwBackupGold;

public: 
	CWndGuildCombatOffer();
	virtual ~CWndGuildCombatOffer();
	
	virtual	BOOL	Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK );
	virtual	BOOL	OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );
	virtual	void	OnDraw( C2DRender* p2DRender );
	virtual	void	OnInitialUpdate();
	virtual	BOOL	OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual	void	OnSize( UINT nType, int cx, int cy );
	virtual void	OnLButtonUp( UINT nFlags, CPoint point );
	virtual	void	OnLButtonDown( UINT nFlags, CPoint point );
	void			SetGold( DWORD nCost );
	void			SetTotalGold( __int64 nCost );
	void			SetMinGold( DWORD dwMinGold )		{ m_dwMinGold    = dwMinGold; }
	void			SetReqGold( DWORD dwReqGold )		{ m_dwReqGold    = dwReqGold; }
	void			SetBackupGold( DWORD dwBackupGold ) { m_dwBackupGold = dwBackupGold; }
	void			EnableAccept( BOOL bFlag );	
}; 

class CWndGuildCombatBoard : public CWndNeuz
{
protected:

public:
	int m_nCombatType;		//sun: 11, �ϴ��� ��� ����

	CWndGuildCombatBoard(int m_nCombatType);		//sun: 11, �ϴ��� ��� ����

	virtual ~CWndGuildCombatBoard();
	
	virtual	BOOL	Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK );
	virtual	BOOL	OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );
	virtual	void	OnDraw( C2DRender* p2DRender );
	virtual	void	OnInitialUpdate();
	virtual	BOOL	OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual	void	OnSize( UINT nType, int cx, int cy );
	virtual void	OnLButtonUp( UINT nFlags, CPoint point );
	virtual	void	OnLButtonDown( UINT nFlags, CPoint point );

	virtual void	PaintFrame( C2DRender* p2DRender );		//sun: 11, �ϴ��� ��� ����

	void			SetString( CHAR* szChar );
}; 

class CGuildCombatSelectionClearMessageBox : public CWndMessageBox
{ 
public: 
	CString m_strMsg;
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD dwWndId = 0 );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
}; 

class CGuildCombatInfoMessageBox : public CWndNeuz
{
public:
	int	m_nCombatType;		//sun: 11, �ϴ��� ��� ����

public:
	CGuildCombatInfoMessageBox(int nCombatType);		//sun: 11, �ϴ��� ��� ����

	virtual ~CGuildCombatInfoMessageBox();
	void	SetString( CHAR* szChar );
	virtual	void OnInitialUpdate();
	virtual	BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );
//sun: 11, �ϴ��� ��� ����
	virtual void PaintFrame( C2DRender* p2DRender );
	void	SetString( CString strMsg );
}; 

class CGuildCombatInfoMessageBox2 : public CWndNeuz
{ 
public: 
	CGuildCombatInfoMessageBox2();
	virtual ~CGuildCombatInfoMessageBox2();
	void	SetString( CHAR* szChar );
	virtual	void OnInitialUpdate();
	virtual	BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
}; 



class CWndGuildCombatSelection : public CWndNeuz
{
protected:	
	multimap<int, CGuildMember*>	m_mapSelectPlayer;   // ��帮��Ʈ...��������

	vector<u_long>					m_vecGuildList   ;   // ��� ����Ʈ
	vector<u_long>					m_vecSelectPlayer;   // ������ ����Ʈ..

	u_long							m_uidDefender;
	CTexture						m_TexDefender;
	int								m_nDefenderIndex;
	
	int								nMaxJoinMember;
	int								nMaxWarMember;
	
public: 
	void Reset();
	CWndGuildCombatSelection();
	virtual ~CWndGuildCombatSelection();
	
	virtual	BOOL	Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK );
	virtual	BOOL	OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );
	virtual	void	OnDraw( C2DRender* p2DRender );
	virtual	void	OnInitialUpdate();
	virtual	BOOL	OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual	void	OnSize( UINT nType, int cx, int cy );
	virtual void	OnLButtonUp( UINT nFlags, CPoint point );
	virtual	void	OnLButtonDown( UINT nFlags, CPoint point );
	void			EnableFinish( BOOL bFlag );		

	void			SetDefender( u_long uiPlayer );
	void			UpDateGuildListBox();

	void			AddCombatPlayer( u_long uiPlayer );
	void			AddGuildPlayer( u_long uiPlayer );

	void			RemoveCombatPlayer( int nIndex ) ;
	void			RemoveGuildPlayer( int nIndex ) ;

	u_long			FindCombatPlayer( u_long uiPlayer );
	u_long			FindGuildPlayer( u_long uiPlayer );

	void			SetMemberSize( int nMaxJoin,  int nMaxWar );
}; 

class CWndGuildCombatState : public CWndNeuz 
{ 
private: 
	CTimeSpan		m_ct;
	time_t    		m_tEndTime; // timegettime+
	time_t    		m_tCurrentTime; // timegettime+
	
public:
	int				m_nCombatType;		//sun: 11, �ϴ��� ��� ����

public:
	CWndGuildCombatState(int nCombatType);	//sun: 11, �ϴ��� ��� ����

	virtual ~CWndGuildCombatState(); 
	
	void		 InsertTitle( const char szTitle[] );
	int          GetSelectIndex( const CPoint& point );	
	void		 Init( time_t lTime );
	void		 InsertGuild( const char szGuild[], const char szName[], int nNum, int nSelect = 0  );	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
	virtual	void OnRButtonUp( UINT nFlags, CPoint point );
	virtual void OnRButtonDown( UINT nFlags, CPoint point );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point);	
	virtual BOOL Process ();

	//sun: 11, �ϴ��� ��� ����
	virtual void PaintFrame( C2DRender* p2DRender );		
	void		 SetTotalGold( __int64 nGold );		

	void		 SetGold( int nGold );
	void		 SetRate( int nRate );
	void	 	 SetTime( time_t tTime ) { m_tCurrentTime = 0; m_tEndTime = time_null() + tTime; }
}; 

class CWndGuildCombatJoinSelection : public CWndNeuz
{
protected:
	int	   m_nMapNum;
	CTimer m_timerInputTimeOut;
	DWORD  m_dwOldTime;
	
public: 
	CWndGuildCombatJoinSelection();
	virtual ~CWndGuildCombatJoinSelection();
	
	virtual	BOOL	Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK );
	virtual	BOOL	OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );
	virtual	void	OnDraw( C2DRender* p2DRender );
	virtual	void	OnInitialUpdate();
	virtual	BOOL	OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual	void	OnSize( UINT nType, int cx, int cy );
	virtual void	OnLButtonUp( UINT nFlags, CPoint point );
	virtual	void	OnLButtonDown( UINT nFlags, CPoint point );
	virtual BOOL	Process ();

	void			SetTimeOut( DWORD dwSec ) { m_dwOldTime = g_tmCurrent + dwSec; }
	void			SetMapNum( int nMap ) { m_nMapNum = nMap; }
}; 

class CWndGuildWarAppConfirm : public CWndNeuz
{
protected:
	CWndText	m_wndText;
public: 
	CWndGuildWarAppConfirm();
	virtual ~CWndGuildWarAppConfirm();
	
	virtual	BOOL	Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK );
	virtual	BOOL	OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );
	virtual	void	OnDraw( C2DRender* p2DRender );
	virtual	void	OnInitialUpdate();
	virtual	BOOL	OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual	void	OnSize( UINT nType, int cx, int cy );
	virtual void	OnLButtonUp( UINT nFlags, CPoint point );
	virtual	void	OnLButtonDown( UINT nFlags, CPoint point );
}; 
class CWndGuildWarCancelConfirm : public CWndNeuz
{
protected:
	CWndText	m_wndText;

	int			m_nCombatType;		//sun: 11, �ϴ��� ��� ����

public:
	CWndGuildWarCancelConfirm(int nCombatType);		//sun: 11, �ϴ��� ��� ����

	virtual ~CWndGuildWarCancelConfirm();
	
	virtual	BOOL	Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK );
	virtual	BOOL	OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );
	virtual	void	OnDraw( C2DRender* p2DRender );
	virtual	void	OnInitialUpdate();
	virtual	BOOL	OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual	void	OnSize( UINT nType, int cx, int cy );
	virtual void	OnLButtonUp( UINT nFlags, CPoint point );
	virtual	void	OnLButtonDown( UINT nFlags, CPoint point );

	virtual void	PaintFrame( C2DRender* p2DRender );	//sun: 11, �ϴ��� ��� ����
}; 
class CWndGuildWarJoinConfirm : public CWndNeuz
{
public:
	int			m_nCombatType;		//sun: 11, �ϴ��� ��� ����

protected:
	CWndText	m_wndText;
public:
	CWndGuildWarJoinConfirm(int nCombatType);	//sun: 11, �ϴ��� ��� ����

	virtual ~CWndGuildWarJoinConfirm();
	
	virtual	BOOL	Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK );
	virtual	BOOL	OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );
	virtual	void	OnDraw( C2DRender* p2DRender );
	virtual	void	OnInitialUpdate();
	virtual	BOOL	OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual	void	OnSize( UINT nType, int cx, int cy );
	virtual void	OnLButtonUp( UINT nFlags, CPoint point );
	virtual	void	OnLButtonDown( UINT nFlags, CPoint point );

	virtual void	PaintFrame( C2DRender* p2DRender );		//sun: 11, �ϴ��� ��� ����
}; 

class CWndGuildWarState : public CWndNeuz 
{ 
private: 
	CWndScrollBar		m_wndScrollBar;
	int					m_nMax;
	GUILDLIST			m_aList[MAX_WANTED_LIST];
	int					m_nSelect;
	
public: 
	CWndGuildWarState(); 
	virtual ~CWndGuildWarState(); 
	
	void		 InsertTitle( const char szTitle[] );
	int          GetSelectIndex( const CPoint& point );	
	void		 Init( time_t lTime );
	void		 InsertGuild( const char szGuild[], const char szName[], int nNum );	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
	virtual	void OnRButtonUp( UINT nFlags, CPoint point );
	virtual void OnRButtonDown( UINT nFlags, CPoint point );
	virtual void OnMouseMove(UINT nFlags, CPoint point );
	virtual BOOL OnMouseWheel( UINT nFlags, short zDelta, CPoint pt );
	virtual void OnMouseWndSurface( CPoint point );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point);	
}; 


struct GUILDNAME
{
	char	 szGuild[ MAX_NAME ];		// �̸�.
};

class CWndGuildCombatRanking : public CWndNeuz 
{ 
private: 
	CWndScrollBar		m_wndScrollBar;
	int					m_nMax;
	multimap< int, GUILDNAME > m_multimapRanking;
	int					m_nSelect;
	
public: 
	void SortRanking();
	CWndGuildCombatRanking(); 
	virtual ~CWndGuildCombatRanking(); 
	
	int          GetSelectIndex( const CPoint& point );	
	void		 Init( time_t lTime );
	void		 InsertGuild( const char szGuild[], int nWinCount );	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
	virtual	void OnRButtonUp( UINT nFlags, CPoint point );
	virtual void OnRButtonDown( UINT nFlags, CPoint point );
	virtual void OnMouseMove(UINT nFlags, CPoint point );
	virtual BOOL OnMouseWheel( UINT nFlags, short zDelta, CPoint pt );
	virtual void OnMouseWndSurface( CPoint point );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point);	
}; 

// ��� ��� �α� - ���
class CWndGuildCombatTabResultRate : public CWndNeuz 
{ 
public: 
	CWndGuildCombatTabResultRate(); 
	virtual ~CWndGuildCombatTabResultRate(); 
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
}; 
// ��� ��� �α� - ����
class CWndGuildCombatTabResultLog : public CWndNeuz 
{ 
public: 
	CWndGuildCombatTabResultLog(); 
	virtual ~CWndGuildCombatTabResultLog(); 
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
}; 

// ��� ��� �α�â
class CWndGuildCombatResult : public CWndNeuz 
{ 
public: 
	void InsertLog( CString str );
	void InsertPersonRate( CString str );
	void InsertGuildRate( CString str );
	CWndGuildCombatResult(); 
	virtual ~CWndGuildCombatResult(); 
	
	CWndGuildCombatTabResultRate   m_WndGuildCombatTabResultRate;
	CWndGuildCombatTabResultLog    m_WndGuildCombatTabResultLog;
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
	virtual void OnMouseMove(UINT nFlags, CPoint point );
}; 



typedef struct __GUILDCOMBAT_RANK_INFO
{
	u_long	uidPlayer;
	int		nJob;
} __GUILDCOMBAT_RANK_INFO;

typedef struct __GUILDCOMBAT_RANK_INFO2
{
	CString strName;
	CString strJob;
	u_long	uidPlayer;
	int		nPoint;	
} __GUILDCOMBAT_RANK_INFO2;

#define MAX_GUILDCOMBAT_RANK_PER_PAGE 11
#define MAX_GUILDCOMBAT_RANK		  100


// ��� ��ŷ ��- ������
class CWndGuildCombatRank_Class : public CWndNeuz 
{ 
public: 
	CWndScrollBar		m_wndScrollBar;
	int					m_nMax;
	int					m_nSelect;

	int		m_nRate;
	int		m_nOldRate;

	__GUILDCOMBAT_RANK_INFO2		m_listRank[MAX_GUILDCOMBAT_RANK];

	CWndGuildCombatRank_Class(); 
	virtual ~CWndGuildCombatRank_Class(); 

	void		 InsertRank( int nJob, u_long uidPlayer, int nPoint );
	int          GetSelectIndex( const CPoint& point );	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual BOOL OnMouseWheel( UINT nFlags, short zDelta, CPoint pt );
	virtual	void OnLButtonDown( UINT nFlags, CPoint point );
}; 

// ��� ��ŷ - ������
class CWndGuildCombatRank_Person : public CWndNeuz 
{ 
public: 
	multimap< int, __GUILDCOMBAT_RANK_INFO > m_mTotalRanking;
			
	CWndGuildCombatRank_Person(); 
	virtual ~CWndGuildCombatRank_Person(); 
	
	CWndGuildCombatRank_Class    m_WndGuildCombatTabClass_Tot;
	CWndGuildCombatRank_Class    m_WndGuildCombatTabClass_Mer;
	CWndGuildCombatRank_Class    m_WndGuildCombatTabClass_Mag;
	CWndGuildCombatRank_Class    m_WndGuildCombatTabClass_Acr;
	CWndGuildCombatRank_Class    m_WndGuildCombatTabClass_Ass;
	
	CWndGuildCombatRank_Class*  __GetJobKindWnd( int nJob );
	void						InsertRank( int nJob, u_long	uidPlayer, int nPoint );
	void						UpdateList();
	void						DivisionList();
	void						UpdatePlayer( int nJob, u_long idPlayer );
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
	virtual void OnMouseMove(UINT nFlags, CPoint point );

}; 

class CWndFontEdit : public CWndNeuz 
{ 
	CTexture*        m_pTexture;
	CPoint			 m_ColorScrollBar[3];
	CPoint			 m_OriginalColorScrollBar[3];
	
	CRect			 m_ColorRect[3];
	FLOAT			 m_fColor[3];
	
	BOOL			 m_bLButtonClick;
	
public: 
	CWndText* m_pWndText;
	CWndFontEdit(); 
	virtual ~CWndFontEdit(); 
	
	void	ReSetBar( FLOAT r, FLOAT g, FLOAT b );
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
	virtual void OnMouseWndSurface( CPoint point );
}; 

// sun 8�� ����â ���� ����
#include "WndWorld.h"

class CWndBuffStatus : public CWndNeuz 
{
public:
	vector< multimap<DWORD, BUFFSKILL> > m_pBuffTexture;
	list<BUFFICON_INFO> m_pBuffIconInfo;

	int m_BuffIconViewOpt;
public:
	CWndBuffStatus(); 
	virtual ~CWndBuffStatus(); 
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
	virtual void OnRButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnRButtonDown( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point );
	virtual void OnRButtonDblClk( UINT nFlags, CPoint point );
	virtual void OnMouseWndSurface( CPoint point );
	virtual	void PaintFrame( C2DRender* p2DRender );

	void RenderBuffIconSkill( C2DRender *p2DRender, IBuff* pBuff, BOOL bPlayer, BUFFICON_INFO* pInfo, CPoint ptMouse );
	void RenderBuffIconItem( C2DRender *p2DRender, IBuff* pBuff, BOOL bPlayer, BUFFICON_INFO* pInfo, CPoint ptMouse );
	void RenderBuffIcon( C2DRender *p2DRender, IBuff* pBuff, BOOL bPlayer, BUFFICON_INFO* pInfo, CPoint ptMouse );
	void RenderOptBuffTime(C2DRender *p2DRender, CPoint& point, CTimeSpan &ct, DWORD dwColor );
	void SetBuffIconInfo();
	BOOL GetHitTestResult();
};


//sun: 9�� ���°��� Clienet
/*******************************
	���� �ý��� ���� Window
********************************/
#include "WndSummonAngel.h"

class CWndMixJewelConfirm : public CWndNeuz
{
public: 
	CWndMixJewelConfirm(); 
	virtual ~CWndMixJewelConfirm(); 
	
	virtual void OnDestroy();
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
};

class CWndMixJewel : public CWndNeuz 
{ 
public:
	CWndText* m_pText;
	GENMATDIEINFO m_MatJewel[MAX_JEWEL];	// cr : uw :
	ItemCountSet m_ItemInfo[MAX_JEWEL];		// cr : uw :
	
	int m_nitemcount;
	int m_nSelectCtrl;
	int m_nOrichalcum;
	int m_nMoonstone;
	
	BOOL m_bStart;
	
	CWndInventory* m_pWndInventory;
	CWndMixJewelConfirm* m_pConfirm;
public: 
	
	CWndMixJewel(); 
	~CWndMixJewel(); 
	
	virtual void SerializeRegInfo( CAr& ar, DWORD& dwVersion );
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point );
	virtual void OnRButtonDblClk( UINT nFlags, CPoint point );
	virtual	void OnMouseMove( UINT nFlags, CPoint point );
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );
	virtual void OnDestroy( void );
	virtual	void	OnDestroyChildWnd( CWndBase * pWndChild );	// cr : uw :
	
	void SetDescription( CHAR* szChar );		
	int HitTest( CPoint point );
	void ReceiveResult(int nResult);
	void SetJewel(FLItemElem* pItemElem);
	void SetStartBtn(BOOL buse);
	void SetConfirmInit();
};

class CWndItemMixConfirm : public CWndNeuz
{
private:
	int m_nResultItemID;

public: 
	CWndItemMixConfirm(int nResultItemID); 
	virtual ~CWndItemMixConfirm(); 
	
	virtual void OnDestroy();
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
};

#ifdef COSTUME_UPGRADE_MIX
#define MAX_MIXCOSTUME	5

class CWndItemMix : public CWndNeuz 
{
public:
	enum WndMode { WND_COSTUME, WND_MATERIAL };
public:
	WndMode m_eWndMode;
public:
	CWndText* m_pText;
	GENMATDIEINFO m_Material[MAX_MIXCOSTUME];	// cr : uw :
	ItemCountSet m_ItemInfo[MAX_MIXCOSTUME];		// cr : uw :
	
	int m_nitemcount;
	int m_nSelectCtrl;

	
	BOOL m_bStart;
	
	CWndInventory* m_pWndInventory;
	CWndItemMixConfirm* m_pConfirm;
public: 
	
	CWndItemMix(WndMode eWndMode); 
	~CWndItemMix(); 
	
	virtual void SerializeRegInfo( CAr& ar, DWORD& dwVersion );
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point );
	virtual void OnRButtonDblClk( UINT nFlags, CPoint point );
	virtual	void OnMouseMove( UINT nFlags, CPoint point );
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );
	virtual void OnDestroy( void );
	virtual	void	OnDestroyChildWnd( CWndBase * pWndChild );	// cr : uw :
	
	void SetDescription( CHAR* szChar );		
	int HitTest( CPoint point );
	void ReceiveResult(int nResult, int nItemID);
	void SetJewel(FLItemElem* pItemElem);
	void SetStartBtn(BOOL buse);
	void SetConfirmInit();

	void SendPacket();
};
#endif

class CWndExtraction : public CWndNeuz
{
public:
	CWndText* m_pText;
	FLItemElem* m_pItemElem;
	PT_ITEM_SPEC m_pEItemProp;
	
public: 
	CWndExtraction(); 
	virtual ~CWndExtraction(); 
	
	virtual void OnDestroy();
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point );
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );
	
	void SetDescription(CHAR* szChar);
	void ReceiveResult(int result, int nCount);
	void SetWeapon(FLItemElem* pItemElem);
};

class CWndSmeltJewel : public CWndNeuz
{
public:
	enum WndMode { WND_ULTIMATE, WND_BARUNA };
private:
	WndMode m_eWndMode;
public:
	CModelObject* m_pMainItem;
	FLItemElem* m_pItemElem;
	FLItemElem* m_pJewelElem;
	CWndText* m_pText;
	
	DWORD m_dwJewel[5];
	int m_nJewelSlot[5];
	int m_nJewelCount;
	int m_nUsableSlot;
	OBJID m_objJewelId;

	float m_fRotate;
	float m_fAddRot;

	int m_nStatus;
	int m_nCount;
	int m_nDelay;
	int m_nAlpha;
	float m_nEyeYPos;
	BOOL m_bFlash;
	
public:
	CWndSmeltJewel(WndMode eWndMode); 
	~CWndSmeltJewel();
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point );
	virtual	void OnDestroy( void );
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );
	virtual BOOL Process ();

	void OnUltimateDropIcon( LPSHORTCUT pShortcut, CPoint point );
#ifdef BARUNA_UPGRADE_ENHANCEMENT_GEM
	void OnBarunaDropIcon( LPSHORTCUT pShortcut, CPoint point );
#endif
	void SetDescription(CHAR* szChar);
	void ReceiveResult(int result);
	void InitializeJewel(FLItemElem* pItemElem);
	void SetJewel(FLItemElem* pItemElem);
};

#ifdef COSTUME_UPGRADE_ENHANCEMENT_GEM
class CWndNewSmeltJewel : public CWndNeuz
{
public:
	enum WndMode { WND_ULTIMATE, WND_COSTUME };
private:
	WndMode m_eWndMode;
public:
	CModelObject* m_pMainItem;
	FLItemElem* m_pItemElem;
	FLItemElem* m_pJewelElem;
	CWndText* m_pText;

	DWORD m_dwMain;
	
	DWORD m_dwJewel[5];
	int m_nJewelSlot[5];
	int m_nJewelCount;
	int m_nUsableSlot;
	OBJID m_objJewelId;

	float m_fRotate;
	float m_fAddRot;

	int m_nStatus;
	int m_nCount;
	int m_nDelay;
	int m_nAlpha;
	float m_nEyeYPos;
	BOOL m_bFlash;
	
public:
	CWndNewSmeltJewel(WndMode eWndMode); 
	~CWndNewSmeltJewel();
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point );
	virtual	void OnDestroy( void );
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );
	virtual BOOL Process ();

	void OnCostumeDropIcon( LPSHORTCUT pShortcut, CPoint point );

	void SetDescription(CHAR* szChar);
	void SendPacket();
	void ReceiveResult(int result);
	void InitializeJewel(FLItemElem* pItemElem);
	void SetJewel(FLItemElem* pItemElem);

};
#endif

class CWndChangeWeapon : public CWndNeuz
{
public:
	CWndText* m_pText1;
	CWndText* m_pText2;
	FLItemElem* m_pWItemElem;
	FLItemElem* m_pJItemElem[2];
	int m_nWeaponType;
	BOOL m_bIsSendChange;
	
public:
	CWndChangeWeapon(int nType);
	virtual ~CWndChangeWeapon(); 
	
	virtual void OnDestroy();
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point );
	virtual void OnMouseWndSurface( CPoint point );	
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );
	virtual BOOL Process ();
	
	void SetDescription(CHAR* szChar);
	void ReceiveResult(int result);
	void SetItem(FLItemElem* pItemElem);
};

class CWndRemoveJewelConfirm : public CWndNeuz 
{
public:
	CWndInventory* m_pInventory;
	FLItemBase*	m_pUpgradeItem;

public: 
	CWndRemoveJewelConfirm(); 
	virtual ~CWndRemoveJewelConfirm(); 
	
	virtual void OnDestroy();
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point );

	void SetItem(FLItemBase*	m_pItem);
};

//sun: 10�� ���½ý���	Neuz, World, Trans
/*******************************
	���� �ý��� ���� Window
********************************/
class CWndHeroSkillUp : public CWndNeuz
{
public:
	CWndText* m_pText;
	FLItemElem* m_pItemElem[5];
	int m_JewelID[5];
	int m_SlotID[5];
	int m_PicJewel[5];
	BOOL m_bSendHeroSkillup;
	
public: 
	CWndHeroSkillUp(); 
	virtual ~CWndHeroSkillUp(); 
	
	virtual void OnDestroy();
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point );
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );
	
	int HitTest( CPoint point );
	void SetDescription(CHAR* szChar);
	void ReceiveResult(int nresult);
	void SetJewel(FLItemElem* pItemElem);
};



//sun:10, __TRADESYS

#define MAX_LIST_COUNT 15

//sun: 12, �ɿ��� ž
#define MAX_FLOOR_COUNT 15

class CWndHeavenTower : public CWndNeuz
{
public:
	vector<int> m_nVecList;
	vector<int> m_nDescList;
	vector<int> m_nMsgList;
	vector<int>	m_nCost;

	int m_nChoiceNum;
	int m_nListCount;
	int m_nDescCount;
	int m_nCurrentPage;
	int m_nGap;			//List Gap
	int m_nListOffset;	//Selected List Rect Offset
	
public:
	CWndHeavenTower();
	virtual ~CWndHeavenTower(); 
	
	virtual void OnDestroy();
	virtual BOOL Initialize( CWndBase* pWndParent = NULL,DWORD dwWndId = 0 ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	
	void InitText();
	void InitWnd();
};

class CWndHeavenTowerEntranceConfirm : public CWndMessageBox
{ 
public: 
	DWORD	m_nFloor;

public:
	void	SetValue( CString str, DWORD nFloor );
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD dwWndId = 0 );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
}; 

//sun: 10, �Ӽ����� ����(10���� ����)

class CWndRemoveAttributeConfirm : public CWndNeuz 
{
public: 
	CWndRemoveAttributeConfirm(); 
	virtual ~CWndRemoveAttributeConfirm(); 
	
	virtual void OnDestroy();
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point );
};

class CWndRemoveAttribute : public CWndNeuz
{
public:
	CWndText* m_pText;
	FLItemElem* m_pItemElem;
	PT_ITEM_SPEC m_pEItemProp;
	CTexture* m_pTexture;
	
	CWndRemoveAttributeConfirm* m_pWndConfirm;

public: 
	CWndRemoveAttribute(); 
	virtual ~CWndRemoveAttribute(); 
	
	virtual void OnDestroy();
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point );
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );
	
	void SetDescription(CHAR* szChar);
	void ReceiveResult(BOOL result);
	void SetWeapon(FLItemElem* pItemElem);
};

//sun: 11, �Ǿ�� �ɼ� ����
class CWndRemovePiercing : public CWndNeuz
{
public:
	enum WndMode { WND_GENERAL, WND_BARUNA };
private:
	WndMode m_eWndMode;
public:
	CWndText*	m_pText;
	FLItemElem*	m_pItemElem;
#ifdef BARUNA_UPGRADE_SUIT_PIERCING
	FLItemElem*	m_pMaterial;
#endif

	PT_ITEM_SPEC m_pEItemProp;
	CTexture*	m_pTexture;
#ifdef BARUNA_UPGRADE_SUIT_PIERCING
	CTexture*	m_pTextureMat;
#endif
	int			m_nInfoSlot[10];		//sun: 12, �Ǿ�� ���� â ���� �� ���͸� ���� ���� â �߰�
	
public: 
	CWndRemovePiercing(WndMode eWndMode); 
	virtual ~CWndRemovePiercing(); 
	
	virtual void OnDestroy();
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point );
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );
	
	void SetDescription(CHAR* szChar);
//sun: 12, �Ǿ�� ���� â ���� �� ���͸� ���� ���� â �߰�
	virtual void OnMouseWndSurface( CPoint point );
	void SetItem(FLItemElem* pItemElem);
};

//sun: 12, �Ǿ�� ���� â ���� �� ���͸� ���� ���� â �߰�
class CWndRemoveJewel : public CWndNeuz
{
public:
	enum WndMode { WND_ULTIMATE, WND_COSTUME, WND_BARUNA };
private:
	WndMode m_eWndMode;
public:
	CWndText*	m_pText;
	FLItemElem*	m_pItemElem;
	FLItemElem*	m_pMaterial;
	PT_ITEM_SPEC	m_pEItemProp;
	CTexture*	m_pTexture;
	CTexture*	m_pMaterialTex;
	int			m_nJewelCount;
	int			m_nJewelID[5];
	CTexture*	m_pJewelTex[5];
	int			m_nJewelSlot[5];
	int			m_nInfoSlot[5];
	
public: 
	CWndRemoveJewel(WndMode eWndMode); 
	virtual ~CWndRemoveJewel(); 
	
	virtual void OnDestroy();
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point );
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );
	virtual void OnMouseWndSurface( CPoint point );

	void SetDescription(CHAR* szChar);
	void SetItem(FLItemElem* pItemElem);

	void DrawUltimate( C2DRender* p2DRender );
#ifdef COSTUME_UPGRADE_ENHANCEMENT_GEM
	void DrawCostume( C2DRender* p2DRender );

	void SendPacket();
#endif

private:
	void ResetJewel();

};

//sun:13, ���� Ȯ��(�Ӽ�, �Ϲ�)
class CWndChangeAttribute : public CWndNeuz 
{
public:
	CWndText* m_pText;
	int m_nAttributeNum;
	int m_nAttributeStaticID[5];
	int m_nTooltipTextIndx[6];
	FLItemElem* m_pItemElem;
	FLItemElem* m_pChangeItem;
	CTexture* m_pTexture;

public: 
	CWndChangeAttribute(); 
	virtual ~CWndChangeAttribute();
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual	void OnInitialUpdate();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual void OnLButtonUp( UINT nFlags, CPoint point );
	virtual BOOL OnSetCursor( CWndBase* pWndBase, UINT nHitTest, UINT message );
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point );
	virtual BOOL Process();

public:
	void SetChangeItem( FLItemElem* pItemElem );
	void FillRect(C2DRender *p2DRender, CRect rectBg, DWORD dwColorstart, DWORD dwColorend);
};

//sun: 13, Ŀ�� �ý���
class CWndCoupleMessage : public CWndMessageBox
{
public:
	enum { CM_SENDPROPOSE = 0, CM_RECEIVEPROPOSE, CM_CANCELCOUPLE };

	CString m_strText;
	int m_nMode;
	CObj* m_pTargetObj;

public:	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD dwWndId = 0 );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );

public:
	void SetMessageMod(CString strText, int nMode, CObj* pTargetObj = NULL);
};

class CWndCoupleTabInfo : public CWndNeuz 
{

public:

	LPDIRECT3DVERTEXBUFFER9		m_pVBGauge;

	CCouple*					m_pCouple;

	CTexture* 					m_pGaugeExp;
	CTexture* 					m_pGaugeBg;

	CTexture					m_texGauEmptyNormal;
	CTexture					m_texGauFillNormal;

public: 


	CWndCoupleTabInfo(); 
	virtual ~CWndCoupleTabInfo();
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual	void OnInitialUpdate();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();	
};

class CWndCoupleTabSkill : public CWndNeuz 
{
public:
	CWndText* m_pText;
	CTexture* m_pSkillBgTexture;

public: 
	CWndCoupleTabSkill(); 
	virtual ~CWndCoupleTabSkill();
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual	void OnInitialUpdate();
	virtual void OnDraw(C2DRender* p2DRender);
};

class CWndCoupleManager : public CWndNeuz 
{
public:
	CWndCoupleTabInfo	m_wndCoupleTabInfo;
	CWndCoupleTabSkill	m_wndCoupleTabSkill;
public: 
	CWndCoupleManager(); 
	virtual ~CWndCoupleManager();
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual	void OnInitialUpdate();
};


class CWndFunnyCoinConfirm : public CWndNeuz 
{
public:
	DWORD m_dwItemId;
	FLItemElem* m_pItemElem;

public: 
	CWndFunnyCoinConfirm(); 
	virtual ~CWndFunnyCoinConfirm(); 
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual	void OnInitialUpdate(); 
	void SetInfo(DWORD dwItemId, FLItemElem* pItemElem);
};

class CWndSmeltSafety : public CWndNeuz
{

public:
	enum WndMode { WND_NORMAL, WND_ACCESSARY, WND_PIERCING, WND_ELEMENT, WND_COSTUME };
	enum { SMELT_MAX = 10 };
	enum { ENCHANT_TIME = 1 };
	enum { EXTENSION_PIXEL = 32, HALF_EXTENSION_PIXEL = EXTENSION_PIXEL / 2 };
	enum { GENERAL_NON_USING_SCROLL2_LEVEL = 7, ELEMENTAL_NON_USING_SCROLL2_LEVEL = 10 };
	
private:
	WndMode m_eWndMode;
	bool m_bBaruna;
	FLItemElem* m_pItemElem;
	CTexture* m_pItemTexture;
	CTexture* m_pNowGaugeTexture;
	CTexture* m_pSuccessTexture;
	CTexture* m_pFailureTexture;
	int m_nMaterialCount;
	int m_nScroll1Count;
	int m_nScroll2Count;
	int m_nResultCount;
	BOOL m_bStart;
	bool m_bResultSwitch;
	DWORD m_dwEnchantTimeStart;
	DWORD m_dwEnchantTimeEnd;
	float m_fGaugeRate;
	int m_nValidSmeltCounter;
	int m_nCurrentSmeltNumber;
	int m_nResultStaticID[SMELT_MAX];
	GENMATDIEINFO m_Material[SMELT_MAX];
	GENMATDIEINFO m_Scroll1[SMELT_MAX];
	GENMATDIEINFO m_Scroll2[SMELT_MAX];
	bool m_bResultStatic[SMELT_MAX];
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBufferGauge;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBufferSuccessImage;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBufferFailureImage;
	PT_ITEM_SPEC m_pSelectedElementalCardItemProp;

public:
	CWndSmeltSafety(WndMode eWndMode, bool bBaruna = false);
	virtual ~CWndSmeltSafety();

	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK );
	virtual	void OnInitialUpdate();
	virtual BOOL Process();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point = 0 );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point);
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();

	void SetItem(FLItemElem* pItemElem);
	void RefreshInformation(void);
	void RefreshText(void);
	void RefreshValidSmeltCounter(void);
	void StopSmelting(void);
	void DisableScroll2(void);
	void ResetData(void);
	void AddListItem(GENMATDIEINFO* pListItem, FLItemElem* pItemElem);
	void SubtractListItem(GENMATDIEINFO* pListItem);
	void DrawListItem(C2DRender* p2DRender);
	BOOL IsDropMaterialZone(CPoint point);
	BOOL IsDropScroll1Zone(CPoint point);
	BOOL IsDropScroll2Zone(CPoint point);
	BOOL IsAcceptableMaterial(PT_ITEM_SPEC pItemProp);
	BOOL IsAcceptableScroll1(PT_ITEM_SPEC pItemProp);
	BOOL IsAcceptableScroll2(PT_ITEM_SPEC pItemProp);
	int GetNowSmeltValue(void);
	int GetDefaultMaxSmeltValue(void);
	void SetResultSwitch(bool bResultSwitch) { m_bResultSwitch = bResultSwitch; }
	void SetCurrentSmeltNumber(int nCurrentSmeltNumber) { m_nCurrentSmeltNumber = nCurrentSmeltNumber; }
	void SetResultStatic(bool bResultStatic, int nIndex) { m_bResultStatic[nIndex] = bResultStatic; }
	FLItemElem* GetItemElement(void) const { return m_pItemElem; }
	bool GetResultSwitch(void) const { return m_bResultSwitch; }
	int GetCurrentSmeltNumber(void) const { return m_nCurrentSmeltNumber; }
	int GetResultStaticID(int nIndex) const { return m_nResultStaticID[nIndex]; }
};

class CWndSmeltSafetyConfirm : public CWndNeuz
{
public:
	enum ErrorMode { WND_ERROR1, WND_ERROR2, WND_ERROR3, WND_ERROR4 };

private:
	ErrorMode m_eErrorMode;
	FLItemElem* m_pItemElem;

public:
	CWndSmeltSafetyConfirm(ErrorMode eErrorMode);
	virtual ~CWndSmeltSafetyConfirm();

public:
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK );
	virtual	void OnInitialUpdate();
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );
	virtual void OnDestroy( void );

	void SetWndMode(FLItemElem* pItemElem);
};


class CWndEquipBindConfirm : public CWndNeuz
{
public:
	enum EquipAction { EQUIP_DOUBLE_CLICK, EQUIP_DRAG_AND_DROP };

public:
	CWndEquipBindConfirm(EquipAction eEquipAction);
	virtual ~CWndEquipBindConfirm(void);

public:
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK );
	virtual	void OnInitialUpdate( void );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );

public:
	void SetInformation(FLItemBase* pItemBase, DWORD dwObjId);
	void SetInformation(FLItemElem* pItemElem);

private:
	void EquipItem( void );

private:
	EquipAction m_eEquipAction;
	FLItemBase* m_pItemBase;
	DWORD m_dwObjId;
	FLItemElem* m_pItemElem;
};

class CWndRestateConfirm : public CWndNeuz
{
public:
	CWndRestateConfirm(DWORD dwItemID);
	virtual ~CWndRestateConfirm(void);

public:
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK );
	virtual	void OnInitialUpdate( void );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );

public:
	void SetInformation(DWORD dwItemObjID, OBJID m_ObjID, int nPart = -1);

private:
	DWORD m_dwItemID;
	DWORD m_dwItemObjID;
	OBJID m_ObjID;
	int m_nPart;
};

class CWndCampusInvitationConfirm : public CWndNeuz
{
public:
	CWndCampusInvitationConfirm( u_long idSender = 0, const CString& rstrSenderName = _T( "" ) );
	virtual ~CWndCampusInvitationConfirm( void );

public:
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK );
	virtual	void OnInitialUpdate( void );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );

private:
	u_long m_idSender;
	CString m_strSenderName;
};

class CWndCampusSeveranceConfirm : public CWndNeuz
{
public:
	CWndCampusSeveranceConfirm( u_long idTarget = 0, const CString& rstrTargetName = _T( "" ) );
	virtual ~CWndCampusSeveranceConfirm( void );

public:
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK );
	virtual	void OnInitialUpdate( void );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );

private:
	u_long m_idTarget;
	CString m_strTargetName;
};

#ifdef __SKILL_UI16
class CWndSkill_16 : public CWndNeuz
{
public: 
	CWndSkill_16( );
	virtual ~CWndSkill_16( );

//virtual
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK );
	virtual	void OnInitialUpdate( void );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );
	virtual BOOL Process ();
	virtual void OnMouseWndSurface( CPoint point );
	virtual void OnDraw(C2DRender* p2DRender);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void SerializeRegInfo ( CAr& ar, DWORD& dwVersion );

	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();	

//exposed
	void	InitItem( int nJob, LPSKILL apSkill, BOOL bReset = FALSE );	//��ų�����͸� ���� 
	SKILL*  GetdwSkill( DWORD dwSkill );								//�ش罺ų ������
	
	BOOL	CheckToLearnSkill( int nSkill );							//�� ��ų�� ���� �ִ°�? ( ���ེų, ���ེų���� �� )
	LPSKILL GetSkill( int i );
	BOOL	CanDownPoint( DWORD dwSkill );								//�ش罺ų�� �����ٿ��ų�� �ִ°�? ( ���� �������ִ� ��ų���� ������ ������? )
	void	SubSkillPointDown( LPSKILL lpSkill );						//�� ��ų�� ���ེų�� �������ִ³༮�� ã�Ƽ� ������ ������ ������ �ൿ�� �Ѵ�.

	void	SetJob( int nJob )	{ m_nJob = nJob; }
	LPSKILL GetSkill( )			{ return m_apSkill; }
	int		GetCurSelect()		{ return g_nSkillCurSelect; }
	int		GetCurrSkillPoint() { return m_nCurrSkillPoint; }

protected:
	BOOL	GetSkillIconRect(DWORD dwSkillID, CRect& rect );			//�ش� ��ų���̵� ������ ��ġ�����
	void	LoadTextureSkillIcon( );									//��罺ų�� �������� �ҷ���
	void	AfterSkinTexture( LPWORD pDest, CSize size, D3DFORMAT d3dFormat = D3DFMT_A4R4G4B4 );
	int		GetJobByJobLevel( int jobLevel );							// ���������� �ش��ϴ� ������� ( ����Ʈ���.. �����lv0 -> �Ӽ��ʸ�lv1 -> ����Ʈlv2 )
	void	AutoControlClassBtn( );										//��������(��ư) Enable/Disable
	int		GetJobLevelByJob( int Job );
	BOOL	GetFileNameClassBtn( const int nJobLevel, OUT CString& strBuffer );

protected:
	int				m_nMaxCurrJob;				//���� ������ ��ų �� ����
	int				m_nCurrSkillPoint;			//���� ���� ��ų����Ʈ
	CTexturePack	m_kTexLevel;				//��ų �������� ǥ�ÿ� �׸� ( 1, 2,..... max )
	SKILL*			m_apSkill;					//��ų ���
	SKILL*			m_pFocusItem;				//���õ� ������ : �� ��ų�� ����Ŭ���ϰų� �ؼ� ������ �ø����ִ� ���·� ���� ��ų������
	CTexture*       m_pTexSkill[ MAX_SKILL_JOB ];
	IMAGE*          m_pTexJobPannel;			//��ųƮ�� �̹���(��¥)
	CWndButton*		m_pWndButton[4];			//+, -, ok, close

	int				m_nJob;						//class ��ȣ
	DWORD			m_dwMouseSkill;				//���콺�� ��ġ�� ��ų
	CString			m_strHeroSkilBg;			//����� �̹��� ���� �̸�
	BOOL			m_bLegend;					//�����ΰ�?

	BOOL			m_bDrag;					//���콺�� Ŭ���ߴµ� ��ų������ ���� �� �� ��� TRUE
	int				m_nSelectedClassLevel;		//������ ���� ����(����� ����)

	CTexture*		m_pTexSeletionBack;			//������ ��ų�� ��׶���

};
#endif //__SKILL_UI16


// ����ȭ ������
class CRenderStatDice
{
public:
	CRenderStatDice();
	~CRenderStatDice();

	void	Init( );
	void	Process( float fTime );
	void	SetTime( float fTime );
	void	Render( C2DRender* p2DRender, CRect rect );
	void	Release();
	void	SetResult( int nResult ) { m_nResult = nResult; }
	void	ViewResult( bool bFlag );
	bool	IsFinish( ) const { return m_bFinish; }

public:
	static const int nTextureMax = 28;
	CTexture m_StatDiceTexture[ nTextureMax ];
	CTimer	 m_tmFlash;
	bool	 m_bViewResult;
	int		 m_nResult;
	int		 m_nRenderIndex;
	bool	 m_bFinish;
};

class CWndStatDice : public CWndNeuz 
{	
private:
	BOOL  m_bUseCoupon;
	DWORD m_TargetWndId;
	BOOL  m_checkClose;
	int   m_MainFlag;

	static const int nResultMax = 4;
	CRenderStatDice* m_pStatDice;
	CRect m_DiceRect[ nResultMax ];
	int	  m_nResultCount;

	CTimer	 m_tmStart;
	CTimer	 m_tmStartTab;

public: 
	CWndStatDice(); 
	virtual ~CWndStatDice(); 

	void	SetFinish();
	bool	IsFinish( )		const;
	void	SetResult( BOOL bStr, BOOL bSta, BOOL bDex, BOOL bInt );
	virtual void OnDestroy();
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
}; 

#ifdef CARD_UPGRADE_SYSTEM
class CWndCardUpgrade : public CWndNeuz 
{ 
public: 
	CWndCardUpgrade(); 
	~CWndCardUpgrade(); 

	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnDropIcon		( LPSHORTCUT pShortcut, CPoint point );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point); 
	virtual BOOL Process ();
	void SendPacket();
	void ReceivedPacket(const DWORD dwResult);
	void SetItem(FLItemElem* pItemElem);
protected:
	void DoModalConfirm( );
	void InitCtrl();
protected:
	bool m_bSendPacket;
	enum {  N_COMBINE = 0,
			N_CASH,
			N_RESULT,
			N_MAX
            };
	CRect	m_rectSlot_Item[N_MAX];
	OBJID	m_objID_Item[N_MAX];
	FLItemElem* m_pItemElem;
	DWORD	m_nResultItemIdx;

}; 
#endif	// CARD_UPGRADE_SYSTEM

#endif // !defined(AFX_WNDFIELD_H__A93F3186_63D6_43C1_956F_EC8691E0C7D9__INCLUDED_)

