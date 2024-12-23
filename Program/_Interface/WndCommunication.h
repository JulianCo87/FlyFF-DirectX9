#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

class CWndCommand;
class CWndChatFilter;
//////////////////////////////////////////////////////////////////////////////////////
// ä�� ���� 
//
class CWndEditChat : public CWndEdit
{
public:
	CWndEditChat();
	~CWndEditChat();
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnSetFocus(CWndBase* pOldWnd);
	virtual void OnKillFocus(CWndBase* pNewWnd);
};
class CWndMacroChat : public CWndButton
{
public:
	CTexture* m_pTexMacro;
	CTexture m_texMacroChat;
	CWndMacroChat();
	~CWndMacroChat();
	virtual void OnDraw( C2DRender* p2DRender );
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual	void OnInitialUpdate();
};
class CWndTextChat : public CWndText
{
public:
	CWndTextChat();
	~CWndTextChat();
	virtual BOOL IsPickupSpace(CPoint point); 
};

//sun: �ý��� �޼��� ������ȭ... Neuz
class CWndChatLog : public CWndNeuz
{
	CWndMenu	 m_wndMenuPlace;
	CWndTextChat m_wndText;

public:
	CWndChatLog();   
	virtual ~CWndChatLog(); 
	void  PutString( LPCTSTR lpszString ); 
	void  PutString( LPCTSTR lpszString, DWORD dwColor ); 
	void  PutString( LPCTSTR lpszString, DWORD dwColor, DWORD dwPStyle ); 
//	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL,DWORD dwWndId = 0);
	// message
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase = NULL );
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
//	virtual BOOL OnEraseBkgnd(C2DRender* p2DRender);
	virtual void OnDestroy();
	virtual void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnDestroyChildWnd( CWndBase* pWndChild );
	virtual void OnSetFocus(CWndBase* pOldWnd);
	virtual void OnKillFocus(CWndBase* pNewWnd);
	virtual void AdditionalSkinTexture( LPWORD pDest, CSize size, D3DFORMAT d3dFormat );
	virtual void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void OnRButtonDown(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void SetWndRect( CRect rectWnd, BOOL bOnSize = TRUE);
	virtual BOOL Process ();	
};

#ifdef __HYPERLINK_ITEM16

struct HLITEM_DATA
{
	HLITEM_DATA() { reset(); }
	~HLITEM_DATA( ) { SAFE_DELETE( pItem ); }
	
	void reset() { _masterID = _itemID = _index = _level = 0, _wLine = _wSeq = 0, pItem = NULL; }

//	char _szOrg[ 64 ];
	CString _szOrg;		//original string ( $i........$ni )
	DWORD _masterID;	//master mover ID
	DWORD _itemID;		//item oid
	int _index;			//item index
	int _level;			//item level

	WORD _wLine;		//line
	WORD _wSeq;			//sequence of line 
    
	// �����κ��� ���������� ��û�� ������ tool-tip�� ������, �� �����Ͱ� NULL�� ��츸 ��û�Ѵ�.
	FLItemElem* pItem;	
};

typedef map< DWORD, HLITEM_DATA >			HyperLinkItemContainer;		//key = _wLine | _wSeq
typedef HyperLinkItemContainer::iterator	HyperLinkItemIter;

// ��ũ������ ������
// 1. ����Ʈ + Ŭ������ �������� �����Ѵ�
// 2. �ӽ� HLITEM_DATA �� Data�� ä���.
// 3. ���͸� �����ٸ� �ش� ���ڿ��� �����۸�ũ�κ��� �������� ��ü�ؼ� �����Ѵ�.

// 4. ���۹��� ä�� ���ڿ����� �����۸�ũ�� �ִ��� üũ�ϰ� �ִٸ� ������ ���ڿ��� ��ü���� HLITEM_DATA�� �����
// 5. �̶� Key�� �ش� ���ڿ��� ���ι�ȣ | ���� �� �����Ѵ�.
// 6. ������ �ش� �������� Ŭ���Ұ�� HLITEM_DATA container���� ã���� ������ �����۵����͸� ��û�Ѵ�.
// 7. ���� ������ ������ tooltip�� ����� �ѷ��ش�.

#endif //__HYPERLINK_ITEM16


class CWndChat : public CWndNeuz
{
	IMAGE m_wndChatEdit[3];
	//CSize m_sizeWndChatEdit[3];

	CWndMacroChat m_wndMacroChat;
	CTimer m_timerDobe;
	CTexture m_texEdit;
	CTimer m_timerInsMsg;
	CTimer m_timerAlertGGrade;	//sun: 10, __CSC_GAME_GRADE

	int m_nInsMsgCnt;
	int				m_nHistoryIndex;
	vector<CString> m_strHistory;
	CTimer m_timerInputTimeOut;

public:
#ifdef __HYPERLINK_ITEM16
	HLITEM_DATA				m_kTempHLData_Send;		//���������� �ӽ� �����
	HLITEM_DATA				m_kTempHLData_Receive;	//�����̳ʿ� �ֱ����� �����ϴ� �ӽ� �����
	HyperLinkItemContainer	m_cHyperLinkItems;		//���� �����ۿ� ���ؼ�... 

	int GetLineCount( );
	BOOL UpdateLineForHLItems( const int nBegin, const int nCount );	// ������ ��� ������ �����Ǹ� �����͸� �籸���Ѵ�.
#endif //__HYPERLINK_ITEM16

	BOOL m_bChatLock;

	BOOL m_bChatLog;	//sun: 8, //__CSC_VER8_1

	BOOL m_bMoveLock;
	static int m_nChatChannel;
	CWndChatFilter* m_pWndChatFilter;
	CString m_strCharName;
	CWndTextChat m_wndText;
	CStringArray m_strArray;
	CWndCommand* m_pWndCommand;
//	CWndButton m_wndMenu;
	//CWndText m_wndText;
	//CWndEdit m_wndBeginning;
	CWndEditChat m_wndEdit;
	//CWndText m_wndReceiver;

//	CWndButton m_wndShout;
	//CWndButton m_wndParty;
#ifdef __IMPROVE_MAP_SYSTEM
	enum EHyperlinkMode
	{
		HL_None, HL_Item, HL_UserMark
	};

	BOOL m_bHyperlinked;
	EHyperlinkMode m_eHyperlinkMode;
	DWORD m_dwHyperlinkTextStyle;
	int m_nHyperlinkTextStartPosition;
	int m_nHyperlinkTextLeftPosition;
	int m_nHyperlinkTextRightPosition;
	CString m_strHyperlinkMarkTitle;
	FLOAT m_fHyperlinkMarkPositionX;
	FLOAT m_fHyperlinkMarkPositionY;

	//���콺 ����Ʈ�� ��ũ�����۹��� ����� �ش� ������ �����Ѵ�. ���ʹ�ư�� �������� �̺����� �߰��˽� ���� �ٷ� �̿�
	int m_nSelectedLineForHLITEM;		
#endif // __IMPROVE_MAP_SYSTEM

	CWndChat();   
	virtual ~CWndChat(); 
	void SerializeRegInfo( CAr& ar, DWORD& dwVersion );
	void  Parsing( CString string );

	void  PutString( LPCTSTR lpszString );
	void  PutString( LPCTSTR lpszString, DWORD dwColor ); 
	void  PutString( LPCTSTR lpszString, DWORD dwColor, DWORD dwPStyle ); 

#ifdef __IMPROVE_MAP_SYSTEM
private:
	BOOL CalculateItemHyperlink( int nValidTextLine, const CPoint& pointMouse );
	BOOL CalculateUserMarkHyperlink( int nValidTextLine, const CPoint& pointMouse );
	BOOL IsUserMarkHyperlinkParsingValid( 
		const CString& strParsing, 
		int nParsing1stCharacterCounter, 
		int nParsing2ndCharacterCounter, 
		int nParsing3rdCharacterCounter, 
		CString& strParsingTitle, 
		FLOAT& fParsingPositionX, 
		FLOAT& fParsingPositionY );
public:
#endif // __IMPROVE_MAP_SYSTEM
//	virtual CItem* GetFocusItem() { return NULL; }
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL,DWORD dwWndId = 0);
	// message
	void SetChannel( BOOL bForced = FALSE );
	virtual	void SetWndRect( CRect rectWnd, BOOL bOnSize = TRUE);
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase = NULL );
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnEraseBkgnd(C2DRender* p2DRender);
	virtual void OnDestroy();
	virtual void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnDestroyChildWnd( CWndBase* pWndChild );
	virtual void OnSetFocus(CWndBase* pOldWnd);
	virtual void OnKillFocus(CWndBase* pNewWnd);
	virtual void AdditionalSkinTexture( LPWORD pDest, CSize size, D3DFORMAT d3dFormat );
	virtual void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void OnRButtonDown(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL Process ();	
#ifdef __IMPROVE_MAP_SYSTEM
	virtual void OnMouseWndSurface( CPoint point );
	virtual BOOL OnSetCursor( CWndBase* pWndBase, UINT nHitTest, UINT message );
#endif // __IMPROVE_MAP_SYSTEM

	enum
	{
		MODE_NORMAL,
		MODE_TARNSPARENT,
	};
	void SwitchMode( int nMode );
	int m_nMode;
};



class CFLWndChatNotice : public CWndNeuz
{
public:
	CFLWndChatNotice();
	~CFLWndChatNotice();
	virtual BOOL Initialize		( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual	void OnInitialUpdate( );
	virtual	BOOL Process( );
	virtual BOOL IsPickupSpace(CPoint point); 
	virtual void SetWndRect( CRect rectWnd, BOOL bOnSize = TRUE);
	void SetNoticeText(CString strNotice);

private:
	std::vector<CString>::iterator	m_itrNotice;
	std::vector<CString>	m_vecNotice;
	std::vector<CString>	m_vecTempNotice;

	CString m_szNotice;
	CWndStatic* m_pNotice;
	int m_nIndex;
};

#endif
