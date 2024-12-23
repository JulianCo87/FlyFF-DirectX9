
#pragma once 

#pragma warning( disable : 4482 )

namespace WndElldinsJar
{
	enum eSlot
	{
		eSlot_Jar = 0,
		eSlot_JarImage,
		eSlot_1stPotion,
		eSlot_2ndPotion,

		eSlot_Max,
	};
}

using namespace WndElldinsJar;

//---------------------------------------------------------------------------------------------
// ������ �׾Ƹ�
//---------------------------------------------------------------------------------------------
class CWndElldinsJar		:	public CWndNeuz
{

public:


							CWndElldinsJar();
	virtual					~CWndElldinsJar();

	virtual BOOL 			Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);

	virtual	void 			OnInitialUpdate();
	virtual BOOL			OnDropIcon( LPSHORTCUT pShortcut, CPoint point );
	virtual BOOL			OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void 			OnDraw(C2DRender* p2DRender);


private:


	bool					FindRegisterUnSealElldinsJar( OBJID dwItemID );

	bool					FindRegister1stPostion( OBJID dwItemID );
	bool					FindRegister2ndPostion( OBJID dwItemID );

	

	bool					IsUnSealElldinsJar( DWORD dwItemID );
	bool					IsPossibleRegisterElldinsJarPotion( DWORD dwItemID, eSlot eSlotIndex );

	bool					DrawFillTexture( C2DRender* p2DRender );
	bool					DrawIconInSlot( C2DRender* p2DRender, FLItemElem* pItem, eSlot eSlotIndex );
	bool					DrawTooltip( C2DRender* p2DRender, FLItemElem* pItem, eSlot eSlotIndex );
	bool					DrawTextHP( C2DRender* p2DRender );

	void					CheckItemFromInventory( eSlot eSlotIndex );
	void					CheckEnableOKButton( void );

	bool					ButtonOK_Process();


private:

	CTexture*				m_pFillTexture;

	FLItemElem*				m_pUnSealedElldinsJar;		// ���� ���� �� ������ �׾Ƹ�
	FLItemElem*				m_p1stPotion;				// 1��° ���� ���� ������
	FLItemElem*				m_p2ndPotion;				// 2��° ���� ���� ������

	OBJID					m_dwUnSealElldinsJarID;		// ���� ���� �� ������ ���̸� ������ ���̵�
	OBJID					m_dw1stPostionID;			// 1��° ���� ���� ������ ���̵�
	OBJID					m_dw2ndPostionID;			// 1��° ���� ���� ������ ���̵�

	CWndButton*				m_pWndButtonOK;

	CWndStatic*				m_pWndCurrentSavedHP;		// ���� ���� �� ü��
	CWndStatic*				m_pWndCanBeMaximumSaveHP;	// ���� ������ �ִ� ü��

	CRect					m_Rect[eSlot::eSlot_Max];

};










