
#pragma  once

// 17�� ���� UI class ����
// Ư���� ������ ������ 17�� ���� ���� CWndNeuz�� ����ϴ� Ŭ������ ���⿡ �����մϴ�.

//�߰��ŷ�
#include "../_CommonDefine/Packet/FLPacket_ConsignmentSale.h"
using namespace nsConsignmentSale;

#include "../_CommonDefine/packet/FLPacketItemUpgrade.h"


template < typename T >
struct BsMinMax
{
	BsMinMax( const T& min, const T& max ) { _min = min; _max = max; }
	T _min;
	T _max;

	float GetPercent( ) { if( _max == 0 ) return 0; return (float)(_min / _max); }
};

void MakeMoneyStyle( CString& str );


// CWnd �� �⺻��
class CWndWidget : public CWndNeuz
{
public:
	CWndWidget();
	virtual ~CWndWidget();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );
	virtual void OnDestroyChildWnd( CWndBase* pWndChild );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);
};


// ���� �����Է�����( EditBox )
class CWndInputNumCommon : public CWndNeuz
{
public:
	CWndInputNumCommon();
	virtual ~CWndInputNumCommon();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );

	//message
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);

	//����Ʈ�ڽ��� ���ڸ� ���ڷ� �����´�, ���ڰ� �ƴҰ�� -1
	int GetNumeric();

	void OnOK( );
};


// ��������� ������ ��� 
class CWndDoanationItem : public CWndNeuz
{
public:
	CWndDoanationItem();
	virtual ~CWndDoanationItem();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );
//	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point = 0 );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);

	void SetData( const DWORD dwItemIndex, const int nCount );

protected:
	CTexture*  m_pTexture;

	int m_nCount; //����
	DWORD m_dwItemIndex;
};

// ��������� ��� ��� 
class CWndDoanationPenya : public CWndNeuz
{
public:
	CWndDoanationPenya();
	virtual ~CWndDoanationPenya();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);

	void SetData( const int nCount );

protected:
	int m_nCount;	//����
};

// �������
class CWndDonation : public CWndNeuz
{
public:
	CWndDonation();
	virtual ~CWndDonation();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);
	virtual void OnMouseWndSurface( CPoint point );

	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();

	void SetEnableButtonOK( BOOL bEnable );
	void CreateSubWindow_Item( const DWORD dwIndex, const int nCount );
	void CreateSubWindow_Penya( const int nCount );

	void RenderRewardItems( C2DRender* p2DRender, DWORD index, DWORD wndid );
	void SetData_MainInfo( DWORD subject, __time64_t tRemainSec, DWORD condition );
	void SetData_Item( DWORD itemID, size_t unit, DWORD max, DWORD curr );
	void SetData_Money( size_t unit, int nGoldMax, int curr );
	void SetData_Reward( vector< DWORD >& items, vector< DWORD >& strings );
	BOOL PutTooltip_RewardItems( DWORD index, DWORD ui_index, const CPoint& point );

	struct IN_DATA
	{
		IN_DATA() { _id = 0; _unit = 0; _max = 100; _curr = 0;}
		DWORD _id; 
		size_t _unit;
		int _max;
		int _curr;
	};

protected:
	CTexture* m_pTexGuage;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBufferGauge;

	float m_fRate; 
	
	int m_nMaxGauage;
	int m_nCurrGauage;
	__time64_t m_n64RemainSec;
	float m_fTimeSaver;
	CWndDoanationItem* m_pWndItem;
	CWndDoanationPenya* m_pWndPenya;

	IN_DATA m_kData[2];			//0 : item, 1 : money
	vector< DWORD > m_vRewardItems;
};


//������� ��ŷ
class CWndDonationRanking : public CWndNeuz
{
public:
	CWndDonationRanking();
	virtual ~CWndDonationRanking();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);	

	void SetData( int arrCount, TCHAR szData[][MAX_NAME] );
};

//�ݷμ��� ����
class CWndColosseumJoin : public CWndNeuz
{
public:
	CWndColosseumJoin();
	virtual ~CWndColosseumJoin();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);

	//�װ��� ��ư�� �Ѱ��� ����, radio��ưó�� �����Ѵ�.
	void SetSelectedButton( int nID );
	void SendPacket( const int kind );	// 0 : �Ϲ�, 1 : ���

	void SetEnableButton( const DWORD id, const BOOL bEnable );
	void SetEnableButtonAll( const BOOL bEnable );
	void ResetButtonAll( );

protected:
	int m_nSelectedID;		//���õ� ��ư ID
};

//�ݷμ��� ���� ���۴��
class CWndColosseumReadyToStart : public CWndNeuz
{
public:
	CWndColosseumReadyToStart();
	virtual ~CWndColosseumReadyToStart();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);
	virtual void OnLButtonDown( UINT nFlags, CPoint point );

	void DropedIcon( const DWORD itemid );		//���а� �����, ĳ�������� ����
	void SetTimer( const float seconds );
	void SendPacket( );
	void SetItemID( const OBJID objid );
	void ReceivedPacketNoti( const DWORD itemindex );		//�����κ��� ��Ͻ����� ��� ����Ҷ�� ����

protected:
	float	m_fSec;
	OBJID	m_itemID;			//��ϵ� ������ (�����)
	DWORD	m_itemIndex;		//���� ���ſ� index, ������ �̰��� ��ϵǸ� �������� �׸���.

	CTexture*  m_pTexture;
	CWndMessageBox* m_pWndMsgBox;
};

//�ݷμ��� Ÿ�ӿ���, �絵��
class CWndColosseumRetry : public CWndNeuz
{
public:
	CWndColosseumRetry();
	virtual ~CWndColosseumRetry();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);

	void SetTimer( const float seconds );
	void SendPacket( );

protected:
	float m_fSec;
};

//�ݷμ��� �޼��� ( not modal )
class CWndColosseumModelessBox : public CWndNeuz
{
public:
	enum BOX_TYPE
	{
		BT_STAGE_READY,		//�������� �غ� 
		BT_STAGE_COUNTDOWN,	//���������� ī��Ʈ �ٿ�
		BT_RETRY_READY,	
	};
	CWndColosseumModelessBox();
	virtual ~CWndColosseumModelessBox();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );
	virtual BOOL IsPickupSpace(CPoint point); 

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);

	void SetType( BOX_TYPE eType, int nStage );
	void DoTimeOver( );
	void SetTimer( float fSec );

protected:
	BOX_TYPE m_eType;
	float m_fSec; //�����ð�
	CRect m_kRect; //���� ������
	BOOL m_bTimeOver; 
	int m_nCountDown;		//10...9...8 ȿ����
	float m_fEffectSound;	//ȯȣ��

	int m_nStage; //�������� ��ȣ
};

//�ݷμ��� MessageBox( modal )
class CWndColosseumModalBox : public CWndNeuz
{
public:
	enum BOX_TYPE
	{
		BT_INVITE,				//�ʴ�����
	};
	CWndColosseumModalBox( );
	virtual ~CWndColosseumModalBox( );

	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);

	void DoAction_Type1( );	//ù��°��ư ó��( ��ư ���ڿ��� �ٸ������� )
	void DoAction_Type2( );	//�ι�°��ư ó��( ��ư ���ڿ��� �ٸ������� )

	void SetType( BOX_TYPE eType );
	void SetDataWorldID( DWORD dwWorldID );
	void SetInvitePlayerName( const TCHAR* szName );

protected:
	BOX_TYPE m_eType;
	DWORD m_dwWorldID; 
	CString m_strInvitePlayerName;
};

//typedef for CDPClient::OnColosseum_AllClear
//typedef		 T_PACKET_COLOSSEUM_ALL_CLEAR_HEADER::FLRewardItem		FLRewardItem;

//�ݷμ��� ����( ���� )
class CWndColosseumCleared : public CWndNeuz
{
public:
	CWndColosseumCleared( );
	virtual ~CWndColosseumCleared( );

	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);

	void SetData( std::vector< FLRewardItem >& vData );
	void SetClearTime( const float fSeconds );
	void SetTimer( const float fSec );
	void SetRank( const bool bVisible, const int nRank );

	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();

protected:
	void MakeGaugeRect( );

protected:
	std::vector< FLRewardItem > m_kDatas;	//��������۵�
	float m_fClearTime;		//�� �ɸ��ð�
	float m_fSec;
	float m_fMaxSec;

	CTexture m_texGauFillNormal;
	LPDIRECT3DVERTEXBUFFER9 m_pGauge;
	CRect m_rectGauge;
};

//�ݷμ��� ��ŷ��
class CWndColosseumRankingTab : public CWndNeuz 
{ 
public: 
	CWndColosseumRankingTab(); 
	virtual ~CWndColosseumRankingTab(); 

	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 

	void SetData( const T_COLOSSEUM_RANKING& normal, const T_COLOSSEUM_RANKING& hard );
	void AddListData( CWndListBox* pList, int index, DWORD dwTick, const char* szGuild, DWORD style  = 0 );

protected:
}; 

//�ݷμ��� ��ŷ 
class CWndColosseumRanking : public CWndNeuz
{
public:
	CWndColosseumRanking( );
	virtual ~CWndColosseumRanking( );

	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);

	void SetData( const T_COLOSSEUM_RANKING_PACK* pDatas );
	void ResetItemAll( );	

protected:
	void ResetItem( const DWORD wndID );
	void UpdateData( );
	void UpdateData_Records( const DWORD wndID, const DWORD tick );
	void SetCheckRadio_level1( const DWORD child_id );
	void SetCheckRadio_level2( const DWORD child_id );
	
protected:
	T_COLOSSEUM_RANKING_PACK	m_kData[2];	//0 normal, 1 hard

	DWORD m_dwSelectedID_level1;
	DWORD m_dwSelectedID_level2;
};

//----------------------------------------------------------------------------------------------------------------------------------------
//EVENT ARENA
//----------------------------------------------------------------------------------------------------------------------------------------

//��������
class CWndEArenaCharInfo : public CWndNeuz
{
public:
	CWndEArenaCharInfo(); 
	virtual ~CWndEArenaCharInfo(); 

	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual	BOOL Process();

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);
	virtual void OnLButtonDown( UINT nFlags, CPoint point );
	virtual void OnLButtonUp( UINT nFlags, CPoint point );

	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();

	void SetData( const T_EVENT_ARENA_TEAM_MEMBER_INFO& kData );
	void SetCharInfo( const OBJID idChar, int nCurrHP, int nMaxHP );

protected:
	void MakeGaugeRect( );

protected:
	//gauge
	CTexture m_texGauge;
	LPDIRECT3DVERTEXBUFFER9 m_pGauge;
	CRect m_rectGauge;

public:	
	T_EVENT_ARENA_TEAM_MEMBER_INFO m_kData;
	BOOL m_bSelected;
};

typedef std::vector< CWndEArenaCharInfo* >	WndEACharInfo_Container;
typedef WndEACharInfo_Container::iterator	WndEACharInfo_Iter;

//�ϳ��� �׷�( max 8 )
class CWndEArenaGroup : public CWndNeuz
{
public:
	enum 
	{
		EA_MAX = 8,
	};
	CWndEArenaGroup(); 
	virtual ~CWndEArenaGroup(); 

	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);
	virtual void SetWndRect( CRect rectWnd, BOOL bOnSize = TRUE);		//called when moving
	virtual BOOL IsPickupSpace(CPoint point); 
	virtual void OnLButtonDown(UINT nFlags, CPoint point);

	//
	void SetTitle( const char* pszTitle );
	void Reset( );
	void InsertMember( const T_EVENT_ARENA_TEAM_MEMBER_INFO& kMember );
	void AutoPositon( );
	void SetData( const T_EVENT_ARENA_TEAM_INFO& kData );
	CWndEArenaCharInfo* FindWndChar( const char* szName );
	void SetSelctedUser( const DWORD dwSelectedID );
	void SetVisibleMember( BOOL bHide );

public:
	void SetPartyID( const BYTE byPartyID );
	DWORD GetPartyID( )							{ return m_byPartyID; }
	T_EVENT_ARENA_TEAM_INFO m_kData;

protected:
	WndEACharInfo_Container m_vecMembers;
	BYTE m_byPartyID;
	BOOL m_bToggleVisible;
};

typedef std::vector< CWndEArenaGroup* >			WndEAGroup_Container;
typedef WndEAGroup_Container::iterator			WndEAGroup_iter;

//team
class CWndEArenaTeam : public CWndNeuz
{
public:
	enum
	{
		EA_MAX = 4,
	};

	enum E_ARENA_MODE
	{
		EAM_USER,
		EAM_MANAGER,
	};

	CWndEArenaTeam(); 
	virtual ~CWndEArenaTeam(); 

	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
//	virtual BOOL Process( void );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);
	virtual void SetWndRect( CRect rectWnd, BOOL bOnSize = TRUE);		//called when moving

	//mode
	void SetMode( E_ARENA_MODE eMode );
	void SetTitle( const char* pszTitle );

	//
	void SetData( const FLEventArenaTeamStream& kData );
	void UpdateData( const T_EVENT_ARENA_TEAM_MEMBER_INFO& kData );
	void AutoPosition( );
	void InitPosition( );

	void SelectedUser( const DWORD dwSelectedID );			//���ο��� ���õȰ��
	void DoSelectedUser( const DWORD dwSelectedID );		//�ܺο��� ���õȰ��

	void Reset( );
	
protected:
	//�����̵� ���
	void SetLock( BOOL bLock );
	int FindGroupIndex( DWORD dwPartyID );

protected:
	WndEAGroup_Container m_vecGroups;
	BOOL m_bLock;
	CRect m_rectForLock;
	E_ARENA_MODE m_eMode;
	FLEventArenaTeamStream m_kData;

};


//2teams
class CWndEArenaTeamMng : public CWndNeuz
{
public:
	enum
	{
		EAT_RED = 0,
		EAT_BLUE = 1,
		EAT_MAX = 2,
	};
	CWndEArenaTeamMng(); 
	virtual ~CWndEArenaTeamMng(); 

	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
//	virtual BOOL Process( void );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);
	virtual void SetWndRect( CRect rectWnd, BOOL bOnSize = TRUE);		//called when moving

public:
	CWndEArenaTeam m_kTeam[ EAT_MAX ];
};

//stop the battle
class CWndEArenaStopBattle : public CWndNeuz
{
public:
	CWndEArenaStopBattle(); 
	virtual ~CWndEArenaStopBattle(); 

	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
//	virtual BOOL Process( void );

	//message
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);

	//
	BOOL SendPacket( );
};

//message window
//��� : ��ȯ���
//APP_EVENTARENA_CALLADMIN
class CWndEArenaAdminMessage : public CWndNeuz
{
public:
	enum 
	{
		MSG_DOING_SUMMONS,		//��ȯ������
		MSG_DOING_ENTRANCE,		//����������
	};

	CWndEArenaAdminMessage(); 
	virtual ~CWndEArenaAdminMessage(); 

	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );

	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();

	//message
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);

	//
	void SetType( const int nType );
	void SetTimer( float fRemainSec, float fMaxSec );
	int GetType() { return m_nType; }

protected:
	void MakeGaugeRect( );

protected:
	float m_fMaxSec;			//�ִ�ð�
	float m_fRemainSec;			//�����ð�
	int m_nType;

	//gauge
	CTexture m_texGauge;
	LPDIRECT3DVERTEXBUFFER9 m_pGauge;
	CRect m_rectGauge;
};

//���� : ��ȯ�޼���
class CWndEArenaUserMessage_Summons : public CWndNeuz
{
public:
	CWndEArenaUserMessage_Summons(); 
	virtual ~CWndEArenaUserMessage_Summons(); 

	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );

	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();

	//message
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);

	void SendPacket( );
	void SetTimer( float fRemainSec, float fMaxSec );

protected:
	void MakeGaugeRect( );

protected:
	float m_fMaxSec;			//�ִ�ð�
	float m_fRemainSec;			//�����ð�

	//gauge
	CTexture m_texGauge;
	LPDIRECT3DVERTEXBUFFER9 m_pGauge;
	CRect m_rectGauge;
};

//referee window
class CWndEArenaReferee : public CWndNeuz
{
public:
	CWndEArenaReferee(); 
	virtual ~CWndEArenaReferee(); 

	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);
	virtual void OnDestroyChildWnd( CWndBase* pWndChild );

	void MakeMessageBox( int nType, DWORD dwTick );
	void SetTimer( float fRemainTime );
	void SetData( const FLSnapshotEventArenaManageWindowNoti& kData );
	void ResetAllChildWnd( );
	void EnableChildWnd( DWORD dwWIDC, BOOL bEnable );
	void ResetChildWnd( DWORD dwWIDC );

	T_EVENT_ARENA_TONEMENT* FindTournament( DWORD dwTourID );

	//update
	void UpdateData( const T_EVENT_ARENA_GAME_INFO& kData );
	void UpdateState( const T_EVENT_ARENA_GAME_INFO& kData  );

	//������ȯ �Ϸ��, �ؾ����� ó��
	void CompletedSummonsPlayer( );

	//�������� �Ϸ��
	void CompletedJoinPlayer( );

	//�Ѱ�� ����
	void CompletedOneGame( );

	//���ӳ���
	void CompletedFullGame( );

	void SwitchingTimer( BOOL bOnOff )	{ m_bStarted = bOnOff; }
	FLSnapshotEventArenaManageWindowNoti GetData( ) { return m_kData; }

protected:
	void AutoButton_SummonsPlayer( );
	void UpdateWinCount( const BYTE* byRedWins, const BYTE* byBlueWins );		//MAX : MAX_EVENT_ARENA_WIN_COUNT

protected:
	float m_fBattleTime;	//���� �����ð�
	BOOL m_bStarted;

	CWndEArenaStopBattle* m_pWndStopBattle;
	FLSnapshotEventArenaManageWindowNoti m_kData;

public:
	CWndEArenaAdminMessage* m_pWndMessageBox;
};

//winner
class CWndMatchInfo;
class CWndEArenaWinner : public CWndNeuz
{
public:
	enum
	{
		MODE_WINNER,
		MODE_FINAL_WINNER,
	};
	CWndEArenaWinner(); 
	virtual ~CWndEArenaWinner(); 

	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);
	virtual void OnDestroyChildWnd( CWndBase* pWndChild );

	//data
	void SetData_BattleResult( const FLSnapshotEventArenaBattleResultNoti& kData );
	void SetData_GameResult( const FLSnapshotEventArenaGameResultNoti& kData );
	void SetData_BattleInfo( const std::vector< T_BATTLE_RESULT_LOG >& kData );		//�ڽ������� ����â ������

	void SetMode( const int nMode );
	void SetTimer( float fTime ) { m_fTime = fTime; }

protected:
	int m_nMode;

	float m_fTime;
	CTexture*  m_pTexture;	//winner, final winner image

	CWndMatchInfo* m_pWndMatchInfo;
};

//score board
class CWndScoreContent: public CWndNeuz
{
	friend CWndScoreTitle;
public:
	CWndScoreContent(); 
	virtual ~CWndScoreContent(); 

	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
//	virtual void SetWndRect( CRect rectWnd, BOOL bOnSize );
//	virtual BOOL Process( void );

	//message
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);

	void UpdateData( const FLSnapshotEventArenaGameScoreboardNoti& kData );

protected:
	int m_nAni;
	CTexture m_kEffTex[2];
	float m_fDt;
	float m_fMaxDt;

	BOOL m_bEffectChangedState;
	float m_fEffTime;
};

class CWndScoreTitle: public CWndNeuz
{
public:
	CWndScoreTitle(); 
	virtual ~CWndScoreTitle(); 

	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual void SetWndRect( CRect rectWnd, BOOL bOnSize );
	virtual BOOL Process( void );
	virtual void OnDestroyChildWnd( CWndBase* pWndChild );

	//message
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);

	void SetData_Board( const FLSnapshotEventArenaGameScoreboardNoti& kData );
	void SetData_Board2( const std::vector< T_BATTLE_RESULT_LOG >& vecData );
	void BeginEff_ChangedState( );
	void SetUpDownButton( BOOL bOn );
	void SetUpDownButton2( BOOL bOn );
	void AutoPosition_MatchInfo( );

	const std::vector< T_BATTLE_RESULT_LOG >& GetMatchInfoData( ) { return m_vecMatchInfoData; }

protected:
	CWndMatchInfo* m_pWndMatchInfo;
	std::vector< T_BATTLE_RESULT_LOG > m_vecMatchInfoData;

	CWndScoreContent m_kContent;
	FLSnapshotEventArenaGameScoreboardNoti m_kData;
	
	BOOL m_bToggleContent;
	BOOL m_bToggleMatchInfo;
	CRect m_rectMy;
	CRect m_rectContent;
	float m_fDt;
	float m_fAddedDt;
	BOOL m_bBlinked;
};

typedef std::vector< T_BATTLE_RESULT_LOG >::iterator BRL_Iter;
class CWndMatchInfo: public CWndNeuz
{
public:
	CWndMatchInfo( );
	virtual ~CWndMatchInfo( );

	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );

	//message
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);

	void SetData( const std::vector< T_BATTLE_RESULT_LOG >& vecData );

protected:
	//data
	std::vector< T_BATTLE_RESULT_LOG >	m_vecLog;

public:
	float m_fDelta;
};

//SFX SHOW
class EArenaSfxShow
{
public:
	EArenaSfxShow( );
	virtual ~EArenaSfxShow( );

	void Init( );
	BOOL Update( float fDt );

protected:
	float m_fDt;

	vector< D3DXVECTOR3 > m_vTargetPoint;
};

//----------------------------------------------------------------------------------------------------------------------------------------
//�ŷ� �߰� �ý���
//----------------------------------------------------------------------------------------------------------------------------------------

class CWndConsignmentMarket;
typedef FLPacket_SearchAck::SaleInfoVec::iterator SaleDataIter;

static const int MAX_ITEMCOUNT_PAGE = 10;

//tab1 : list
class CWndCM_Sub1List : public CWndNeuz
{
public:
	friend CWndConsignmentMarket;
	enum GRADE_TYPE
	{
		GT_NOMAL = 0x01,	//
		GT_UNIQUE = 0x02,
		GT_ULTRA = 0x04,
		GT_BARUNA = 0x08
	};

	//for treectrl
	struct TREEDATA
	{
		DWORD _dwKey;
		DWORD _dwParentKey;

		CString _strTitle;
		DWORD _dwType[2];

		TREEDATA( ) { _dwKey = 0; _dwParentKey = 0; _dwType[0] = 0; _dwType[1] = 0; }
	};

	typedef map< DWORD, TREEDATA* >			mapTreeDataContainer;
	typedef mapTreeDataContainer::iterator	mapTreeDataIter;

	CWndCM_Sub1List(); 
	virtual ~CWndCM_Sub1List(); 

	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual	void OnInitialUpdate();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual BOOL Process( void );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);
	virtual void OnDestroyChildWnd( CWndBase* pWndChild );
	virtual void OnMouseWndSurface( CPoint point );
	virtual void OnMouseMove(UINT nFlags, CPoint point);

	void OnPage( BOOL bOn = TRUE );

	//data oper
	static void CStringFormat( CString& strBuffer, int nNum );
	void SetTotalItemNum( const int nNum );
	void SetListData( const FLPacket_SearchAck::SaleInfoVec& vecData );
	void SetPageInfo( int nCurrPage, int nMaxPage );

	//packet
	void SendPacket_Refresh( );	

protected:
	int InitSearch( );		    //�˻� �ʱ�ȭ 
	BOOL ReadTreeFile( const char* szFileName );
	void MakeTreeItem( );
	void MakeListItem( );
	void UpdatePageButton( );
	void UpdateCheckButton_Grade( );		//������Ʈ ��� üũ�ڽ� ��ư 
	
	int GetLastPageSet( );
	int PageIdToIndex( );		//Button ID �� 0���ͽ����ϴ� �ε������
	void SetPage( const int nPage );
	
	void CreateMessageBox_Buy( );

	BOOL PutTooltip( const int nListIndex );	
	
	//packet
	BOOL SendPacket_Search( DWORD dwBtnId = 0, int nPage = -1 );
	void MakePacketData( FLPacket_SearchReq& kPacket );
	BOOL SendPacket_Buy( );
	BOOL SendPacket_Page( const DWORD dwPage );
	void SendPacket_MovePageSet( const int nType );	//nType :  -2 �Ǿ�, -1���������V��, 1 ���������V��, 2�ǵ�

protected:
	enum 
	{
		MAX_PAGE = 10,
		MAX_PAGE_SET = 19,

		NEXT_PAGE_SET = 100,		//�����������V
		LAST_PAGE_SET,
		PREVIOUS_PAGE_SET,
		FIRST_PAGE_SET,
	};
	static const int m_snButtonID[ MAX_PAGE ];
	int m_nCurPageButtonID;	
	int m_nPageSet;				//�������V ��ȣ ( �Ѱ��������� = 0~9 )
	
	int m_nCurrPage;			//���絥���� ������( ���������� )
	int m_nPageMax;				//���������۰����� ���� ������( ���������� )
	DWORD m_dwSearchGrade;		//��� : �Ϲ�, ����ũ, ���͸�, �ٷ糪

	int m_nSearchMinLevel;
	int m_nSearchMaxLevel;
	int m_nSearchMinEnchantLevel;
	int m_nSearchMaxEnchantLevel;
	CString m_strSearchString;

	BOOL m_bItem;		// FALSE : ��������, TRUE : �ø�����
	BOOL m_bLevel;		// FALSE : ��������, TRUE : �ø�����
	BOOL m_bPrice;		// FALSE : ��������, TRUE : �ø�����
	BOOL m_bEachPrice;	// FALSE : ��������, TRUE : �ø�����
	BOOL m_bSeller;		// FUCK

	int m_nSelected;
	BOOL m_bReceivedData; //�˻���û�� �����͸� �޾ҵ�.
	
	FLPacket_SearchReq m_kOldPacket;			//������ �ϰų� ������ �ѱ��ϰ�� ���� �˻������� �����.
	FLPacket_SearchAck::SaleInfoVec m_vecListData;
	mapTreeDataContainer m_mapTreeData;

	//childs
	CWndMessageBox* m_pMsgBox_Buy;

	//effects
	float m_fAlpha;

	//user support
	BOOL m_bPushedSearch;
};

class CWndInputNumCommon2;
//tab2 : Registration
class CWndCM_Sub2Registration : public CWndNeuz
{
public:
	CWndCM_Sub2Registration( );
	virtual ~CWndCM_Sub2Registration( );
	
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual	void OnInitialUpdate();
	virtual void OnDraw(C2DRender* p2DRender);
//	virtual BOOL Process( void );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );
	virtual void OnDestroyChildWnd( CWndBase* pWndChild );
	virtual void OnMouseMove(UINT nFlags, CPoint point);

	void OnPage( BOOL bOn = TRUE );

	void CreateInputWnd( DWORD dwItemId );
	void CreateMsgBox_CancelReg( );
	void CreateMsgBox_ConfirmReg( const int nTax );
	BOOL PutTooltip( const int nListIndex );

	//Data
	void SetListData( const FLPacket_RegistedInfoListAck::SaleInfoVec& vecData );
	void SetRegCount( const DWORD dwCurr, const DWORD dwMax );

	//packet
	void SendPacket_CancelReg( );	
	void SendPacket_Tax( const int nPrice );
	void SendPakcet_Registration( );
	void OnPacket_Tax( const int nTax );

protected:
	CWndMessageBox* m_pWndMsgBox_CancelReg;			//������� ����
	CWndMessageBox* m_pWndMsgBox_ConfirmReg;		//��� ����
	CWndInputNumCommon2* m_pWndInput;
	FLPacket_RegistedInfoListAck::SaleInfoVec m_vecListData;

	DWORD m_dwRegItemId;

	DWORD m_dwCurrRegCount;
	DWORD m_dwMaxRegCount;
};

//tab3 : Calculation
class CWndCM_Sub3Calculation : public CWndNeuz
{
public:
	CWndCM_Sub3Calculation( );
	virtual ~CWndCM_Sub3Calculation( );

	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual	void OnInitialUpdate();
	virtual void OnDestroyChildWnd( CWndBase* pWndChild );
	virtual void OnDraw(C2DRender* p2DRender);
	virtual void OnMouseMove(UINT nFlags, CPoint point);

	void OnPage( BOOL bOn );

	void CreateMsgBox( );
	BOOL PutTooltip( const int nListIndex );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);

	//Data
	void SetTotalNum( const DWORD nNum );
	void SetListData( const FLPacket_SaleGoldInfoListAck::SaleGoldInfoVec& vecData );

	//packet
	void SendPacket_OneItem( );		//�Ѱ��� ������ �����û
	void SendPacket_AllItem( );		//��� �����û

protected:
	CWndMessageBox* m_pWndMsgBox_OneItem;
	FLPacket_SaleGoldInfoListAck::SaleGoldInfoVec m_vecListData;
};

//frame : �߰��ŷ���
class CWndConsignmentMarket : public CWndNeuz
{
public:
	enum
	{
		TEXT_COLOR = 0xffffffff,
		TEXT_COLOR_WARNING = 0xffff0000,
		LINE_COLOR = 0x77777777,
		COUNT_COLOR = 0xffffffff,
		SELECT1_COLOR = 0x2211ff11,
		SELECT2_COLOR = 0x7711ff11,
	};
	CWndConsignmentMarket(); 
	virtual ~CWndConsignmentMarket(); 

	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );
	virtual void OnMouseWndSurface( CPoint point );

	void UpdateVisablePage( );
	BOOL IsShowWndId( DWORD dwWndId );
	void DoModalRegister( DWORD dwItemId );					//���â�� �Է±� �����!
	void SetRegistCount( DWORD dwCurr, DWORD dwMax );		//����Ѱ���, �ִ밹��
	void InitCustom( );

public:
	CWndCM_Sub1List m_kWndPage1;
	CWndCM_Sub2Registration m_kWndPage2;
	CWndCM_Sub3Calculation m_kWndPage3;

protected:
	DWORD m_dwCurPage;
	DWORD m_dwCurButton;
};
typedef CWndConsignmentMarket CWndCM;

// ���� �Է±�
class CWndInputNumCommon2 : public CWndNeuz
{
public:
	CWndInputNumCommon2(); 
	virtual ~CWndInputNumCommon2(); 

	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);

	int GetCount( );			//����
	__int64 GetEachPrice( );	//���� �ǸŰ���
	__int64 GetTotalPrice( );

	void SetItem( DWORD dwItemID );						
	void SetPriceMAX( const __int64 n64MaxPrice )		{ m_n64MaxPrice = n64MaxPrice; }	//���� �ִ��ġ ����

protected:
	void SetCount( const int nCount );
	void AddCount( const int nCount );			//���� ��ġ����( ���簪�� ���� )
	void AddString( const char* szString );		//�ǵڿ� ����
	void SubString( );							//�ǵ� �ѱ��� ����
	void ResetString( const DWORD dwWndId );
	void UpdateTotalMoney( );
	BOOL CheckCount( int nCount );
	BOOL GetMaxCount( );

	void SetTargetWnd( const DWORD dwWndId );	//���ڹ�ư Ŭ���� ����� Editbox

	void Process_All( );		//all��ư 
	void EnableChildWnd( DWORD dwWndId, BOOL bEnable );

protected:
	DWORD m_dwTargetWnd;
	DWORD m_dwItemID;
	__int64 m_n64MaxPrice;

};

//��չ��� �ռ�
class CWndComposeTwohandWeapon : public CWndNeuz
{
public:
	enum
	{
		SLOT_1 = 0,
		SLOT_2,
		SLOT_3,
		SLOT_MAX,
	};
	CWndComposeTwohandWeapon(); 
	virtual ~CWndComposeTwohandWeapon(); 

	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );
	virtual void OnMouseWndSurface( CPoint point );
	virtual void OnLButtonDown( UINT nFlags, CPoint point );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point );
	virtual void OnDestroyChildWnd( CWndBase* pWndChild );

	void ReceivedPacket_Result( const DWORD dwResult );

	CRect GetSlotRect( int nSlot );
	void DropedIcon( DWORD dwItemId );
	void ResetSlot( int nSlot );		//�ش� ������ �ʱ�ȭ ��ŵ�ϴ�.

	//�����۵�Ͽ� ���Ѱ��� RegItem���θ� ���ٰ���
	BOOL RegItem( DWORD dwItemId, int nSlot = -1 );		//-1 : �ڵ����

protected:
	BOOL PutItem( int nSlot, DWORD dwItemId );
	void UpdateButton( );
	void UpdateSlot( );
	void UpdateOptionTip( FLItemElem* pItem );		//���� �ɼ��������â update!
	BOOL MakeTipTex( FLItemElem* pItem, CEditString* pEdit );
	void SendPacket( );
	void DoDirection( );			//�������
	void Reset( );

protected:
	DWORD m_dwSlotItem[SLOT_MAX];		//1: ���⽽��1, 2:���⽽��2, 3: ĳ�ý���
	CItem* m_pItem;
	float m_fDTime;
	float m_fDirectTime; //����ð�
	BOOL m_bOnDirection;
	static const float m_sfDirectTimeMAX;

	CWndMessageBox* m_pMsgBoxStart;		//�����Ҷ��� �ȳ� �޼���
	CWndMessageBox* m_pMsgBoxClose;		//�������� �ȳ� �޼���
	CEditString m_strEditString;
};

//��չ��� �ռ� ���
class CWndCancel_ComposeWeapon : public CWndNeuz
{
public:
	CWndCancel_ComposeWeapon(); 
	virtual ~CWndCancel_ComposeWeapon(); 

	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult);
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );
	virtual void OnLButtonDown( UINT nFlags, CPoint point );
	virtual void OnDestroyChildWnd( CWndBase* pWndChild );

	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();

	void SendPakcet( );

	void SetSlotItem( DWORD dwItemId );
	void ResetSlot( );
	void DropedIcon( DWORD dwItemId );
	void SetTimer( float fMaxSec );
	void Reset( );
	void ReceivedPacket( const FLSnapshotItemUpgradeCombineInitializeDataAck& kData ); 
	void ReceivedPacket_Result( const DWORD dwResult );

protected:
	void MakeGaugeRect( );
	void UpdateButton( );
	void DoDirection( );			//�������

protected:
	DWORD m_dwSlotItem;

	//gauge
	CTexture m_texGauFillNormal;
	LPDIRECT3DVERTEXBUFFER9 m_pGauge;
	CRect m_rectGauge;

	BOOL m_bDirection;
	float m_fRemainSec;
	float m_fMaxSec;

	CWndMessageBox* m_pMsgBox;
};

class CWndChangeLook : public CWndNeuz
{
public:
	enum
	{
		SLOT_1 = 0,
		SLOT_2,
		SLOT_3,
		SLOT_MAX,
	};
	CWndChangeLook( );
	virtual ~CWndChangeLook( );

	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );
	virtual void OnDestroyChildWnd( CWndBase* pWndChild );
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);

	void ResetSlot( int nSlot );		//�ش� ������ �ʱ�ȭ ��ŵ�ϴ�.
	BOOL RegItem( DWORD dwItemId, int nSlot = -1 );		//-1 : �ڵ����
	void DropedIcon( DWORD dwItemId );

	void ReceivedPacket( );

protected:
	BOOL PutItem( int nSlot, DWORD dwItemId );
	CRect GetSlotRect( int nSlot );

	//1,2�� ���Կ� ���������� ������ ������� ������.
	void UpdateSlot3( );
	void UpdateButton( );

	void SendPacket( );

protected:
	DWORD m_dwSlotItem[SLOT_MAX];

	CWndMessageBox* m_pMsgBoxConfirm;

	FLItemElem* m_pVirtualRstItem;		//������ ���������.

};

//Target info 
class CWndTargetInfo_HUD : public CWndNeuz
{
public:
	enum
	{
		RANK_KIND_NUM = 4,
		RANK_EACH_NUM = 6,
		ELEMENT_KIND_NUM = 6,
	};

	enum COLOR
	{
		C_NORMAL = 0xffffffff,
		C_ATTACK = 0xffff0000,	//����������
		C_DIE = 0xffa8a49e,		//����
		C_CAO = 0xffff0000,		//ī��ƽ
	};

	CWndTargetInfo_HUD();
	virtual ~CWndTargetInfo_HUD();
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL Process( void );
	virtual void OnDestroyChildWnd( CWndBase* pWndChild );
	virtual	void PaintFrame( C2DRender* p2DRender );

	//message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);

	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();

protected:
	void DrawMonsterMark( C2DRender* p2DRender, CMover* pMover );
	void DrawCitizenMark( C2DRender* p2DRender,CMover* pMover );
	void DrawPlayerMark( C2DRender* p2DRender, CMover* pMover );
	void DrawMoverBuff( C2DRender* p2DRender, CMover* pMover );		//not used, wndWorld���� �ϰ�����. ���⼭ �ҷ��� ���������� daisy���� �÷������.

	void DrawNoMover( C2DRender* p2DRender, CObj* pObj );
	void DrawObjectName( C2DRender* p2DRender, const CString strName, const DWORD dwFontColor = C_NORMAL ); 

protected:
	CTexture m_texAttrCircle[ELEMENT_KIND_NUM];		//�Ӽ�ǥ������
	CTexture m_texRank[RANK_KIND_NUM][RANK_EACH_NUM];
	CTexture* m_pTexMark;

	CTexture m_texBaseForNonMonster;
	CTexture m_texZeroToNine[10];

	CTexture m_kTexEff;
	D3DXMATRIX m_kMat;

	
	//gauge
	CTexture m_texGau_HP;
	CTexture m_texGau_MP;
	LPDIRECT3DVERTEXBUFFER9 m_pGaugeHP;
	LPDIRECT3DVERTEXBUFFER9 m_pGaugeMP;
};



