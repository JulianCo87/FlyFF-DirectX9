
//Date : 20100727
//Author : gmpbigsun
//Note : all about BARUNA ( new enchant )

#pragma  once 

#ifdef __ENCHANT_BARUNA16

#define _EBARUNA_SAFE_LEVEL 2
#define MAX_BARUNA_ENCHANT_LEVEL 20

//---------------------------------------------------------------------------------------------------------
class CWndEnchantBaruna : public CWndNeuz
{
	//�ٷ糪 ���� 
public:
	CWndEnchantBaruna( );
	virtual ~CWndEnchantBaruna( );

	virtual void OnDestroy();

	virtual BOOL Initialize		( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify	( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw			( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate( );
	virtual BOOL OnDropIcon		( LPSHORTCUT pShortcut, CPoint point );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point);
	virtual BOOL Process ();

	BOOL SetItem(FLItemElem* pItemElem);

	BOOL SendPacket( );
//#ifdef BARUNA_ULTIMATE_UPDATE
//	BOOL SendUltimatePacket();
//#endif
	void ReceivedPacket( );

protected:
	void DoModalConfirm( );
	void InitCtrl();

protected:
	enum {  N_CHASHITEM = 0,
			N_OFFERSEED,
			N_ITEM,
            N_MAX,};	
	CRect	m_rectSlot_EnchantBaruna[N_MAX];		//ĳ�þ�����, ���۽õ�, ������
	FLItemElem* m_pItem[N_MAX];

	BOOL m_bSendPacket;
	DWORD m_dwDeltaTime;
};
#ifdef BARUNA_ULTIMATE_UPDATE
//---------------------------------------------------------------------------------------------------------
class CWndBarunaUltimateTrans : public CWndNeuz
{
	//�ٷ糪 ���� 
public:
	CWndBarunaUltimateTrans( );
	virtual ~CWndBarunaUltimateTrans( );

	virtual BOOL Initialize		( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify	( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw			( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate( );
	virtual BOOL OnDropIcon		( LPSHORTCUT pShortcut, CPoint point );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point);
	virtual BOOL Process ();

	BOOL SendPacket( );

	void ReceivedPacket( );

protected:
	void DoModalConfirm( );
	void InitCtrl();

protected:
	enum {  N_OFFERSEED = 0,
			N_ITEM,
			N_JEWELRY,
            N_MAX,};	
	CRect	m_rectSlot_Baruna[N_MAX];		//ĳ�þ�����, ���۽õ�, ������
	OBJID	m_objID_Baruna[N_MAX];

	FLItemElem* m_pItem[N_MAX];

	BOOL m_bSendPacket;
	DWORD m_dwDeltaTime;
};
#endif

//---------------------------------------------------------------------------------------------------------
class CWndProduceOfferSeed : public CWndNeuz
{
	//���� �õ� ����
public:
	CWndProduceOfferSeed( );
	virtual ~CWndProduceOfferSeed( );

	virtual void OnDestroy();
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point);

	virtual BOOL Initialize		( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify	( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw			( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate( );
	virtual BOOL OnDropIcon		( LPSHORTCUT pShortcut, CPoint point );

	virtual BOOL Process ();

	void ReceivedPacket( );

	BOOL SetItem(FLItemElem* pItem);

protected:
	BOOL SendPacket( );

protected:
	enum {  N_OFFER = 0,
			N_CID,
			N_MEAL,
			N_MAX,};	
	CRect m_rectSlot[N_MAX];	//����, �õ�, �ٷ��� ����
	//OBJID m_objID[N_MAX];

	FLItemElem* m_pItem[N_MAX];
	BOOL m_bSendPacket;
};

//---------------------------------------------------------------------------------------------------------
class CWndProduceOffer : public CWndNeuz
{
	//���� ����
public:
	CWndProduceOffer( );
	virtual ~CWndProduceOffer( );

	virtual void OnDestroy();
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point);

	virtual BOOL Initialize		( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify	( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw			( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate( );
	virtual BOOL OnDropIcon		( LPSHORTCUT pShortcut, CPoint point );

	virtual BOOL Process ();

	void ReceivedPacket( );

	BOOL SetItem(FLItemElem* pItem);

protected:
	BOOL SendPacket( );

protected:
    enum {  N_OFFER = 0,
		    N_OFFERMIX,
			N_MAX,};	
	CRect m_rectSlot[N_MAX];

	FLItemElem* m_pItem[N_MAX];
	BOOL m_bSendPacket;
};

//---------------------------------------------------------------------------------------------------------
class CWndProduceSeed : public CWndNeuz
{
	//�õ� ���� �� ���׷��̵�
	//�õ巹���� 2��� �����ϰ��� ���׷��̵��.
public:
	CWndProduceSeed( );
	virtual ~CWndProduceSeed( );

	virtual void OnDestroy();
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point);

	virtual BOOL Initialize		( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify	( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw			( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate( );
	virtual BOOL OnDropIcon		( LPSHORTCUT pShortcut, CPoint point );

	virtual BOOL Process ();

	void ReceivedPacket( );

	BOOL SetItem(FLItemElem* pItem);

protected:
	BOOL SendPacket( );

protected:
	enum { N_CID = 0,
		   N_CIDMIX,
		   N_MAX,};	
	CRect m_rectSlot[N_MAX];
	//OBJID m_objID[N_MAX];

	FLItemElem* m_pItem[N_MAX];
	BOOL m_bSendPacket;
};

//---------------------------------------------------------------------------------------------------------
class CWndExtractOffer : public CWndNeuz
{
	//���� ����
public:
	CWndExtractOffer( );
	virtual ~CWndExtractOffer( );

	virtual void OnDestroy();
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point);

	virtual BOOL Initialize		( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify	( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw			( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate( );
	virtual BOOL OnDropIcon		( LPSHORTCUT pShortcut, CPoint point );

	virtual BOOL Process ();

	BOOL SetItem(FLItemElem* pItemElem);

	void ReceivedPacket( );	

protected:
	BOOL SendPacket( );

protected:
	enum { N_EXTOFFER = 0,
		   N_MAX,};	
	CRect m_rectSlot[N_MAX];
	//OBJID m_objID[N_MAX];
	
	FLItemElem* m_pItem[N_MAX];
	BOOL m_bSendPacket;
};

//---------------------------------------------------------------------------------------------------------
class FLWndBarunaPearcing : public CWndNeuz
{
public:
	FLWndBarunaPearcing();
	virtual ~FLWndBarunaPearcing();

	virtual void OnDestroy();
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point);

	virtual BOOL Initialize		( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify	( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw			( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate( );
	virtual BOOL Process		( );
	virtual BOOL OnDropIcon		( LPSHORTCUT pShortcut, CPoint point );
	virtual BOOL OnCommand		( UINT nID, DWORD dwMessage, CWndBase* pWndBase );

	BOOL SendPacket( );
	void ReceivedPacket( const DWORD dwResult );

	BOOL SetItem(FLItemElem* pItem);

private:
	enum { N_PEARCINGITEM = 0,		// �Ǿ���� ������
		   N_RUNE,					// ��� ���ڼ�
		   N_PROTECT,				// ���ڼ� ��ȣ ������
		   N_ITEM,					// ����Ȯ�� ���� ������
		   N_MAX = 4,};	
	CRect m_rectSlot_Pearcing[N_MAX];	//item,��빮�ڼ�,���ڼ���ȣ������,����Ȯ������������
	//OBJID m_objID_Pearcing[N_MAX];

	FLItemElem* m_pItem[N_MAX];
	BOOL m_bSendPacket;

	void StartPearcing();
};

//---------------------------------------------------------------------------------------------------------
class CFLWndBaruna_Wakeup: public CWndNeuz
{
public:
	CFLWndBaruna_Wakeup();
	virtual ~CFLWndBaruna_Wakeup();
	virtual void OnDestroy();

	virtual BOOL Initialize		( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify	( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw			( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate( );
	virtual BOOL Process		( );
	virtual BOOL OnDropIcon		( LPSHORTCUT pShortcut, CPoint point );
	virtual BOOL OnCommand		( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point);

	void	ReceivedPacket( );	

	BOOL SetItem(FLItemElem* pItem);

private:

	CRect m_rectSlot_BrunaITem;	//������ ������ 
	//OBJID m_objID_BrunaITem[N_MAX];

	FLItemElem* m_pItem;

	//�ʿ����
	CWndStatic* m_pWndBarunaPenya;
	CString m_strPenya;

    //�������� ���� 
	CTexture* m_pTexGuage;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBufferGauge;
	int m_nCurrGauage;
	HRESULT RestoreDeviceObjects();
	HRESULT DeleteDeviceObjects();
	HRESULT InvalidateDeviceObjects();

	BOOL	m_bGauageBarState;
	BOOL	m_bSend;
	
	void	InitCtrl();
	void	SetWakeup(BOOL bSend);
};
//---------------------------------------------------------------------------------------------------------
class CFLWndBaruna_Wakeup_Cancel: public CWndNeuz
{
public:
	CFLWndBaruna_Wakeup_Cancel();
	virtual ~CFLWndBaruna_Wakeup_Cancel();
	virtual void OnDestroy();

	virtual BOOL Initialize		( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify	( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw			( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate( );
	virtual BOOL Process		( );
	virtual BOOL OnDropIcon		( LPSHORTCUT pShortcut, CPoint point );
	virtual BOOL OnCommand		( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point);
	void	ReceivedPacket( );	

	BOOL SetItem(FLItemElem* pItem);

private:
	enum { N_WAKEUP_CANCEL = 0,//��������� ������ 
			N_OBLIVION = 1,	//������
			N_MAX,};	
	CRect m_rectSlot_BrunaITem_Cancel[N_MAX];	
	//OBJID m_objID_BrunaITem_Cancel[N_MAX];

	FLItemElem* m_pItem[N_MAX];

	// �������� ���� 
	HRESULT RestoreDeviceObjects();
	HRESULT DeleteDeviceObjects();
	HRESULT InvalidateDeviceObjects();

	int m_nCurrGauage;
	CTexture* m_pTexGuage;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBufferGauge;

	BOOL	m_bGauageBarState;
	BOOL	m_bSend;
	void	InitCtrl();
	void	SendWakeupCancel(BOOL bSend);
};
//---------------------------------------------------------------------------------------------------------
class CFLWndBaruna_Element_Smelt: public CWndNeuz
{
public:
	CFLWndBaruna_Element_Smelt();
	virtual ~CFLWndBaruna_Element_Smelt();
	virtual void OnDestroy();

	virtual BOOL Initialize		( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify	( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw			( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate( );
	virtual BOOL Process		( );
	virtual BOOL OnDropIcon		( LPSHORTCUT pShortcut, CPoint point );
	virtual BOOL OnCommand		( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point);

	void	ReceivedPacket( );	
	void	SendPacket(BOOL bSend);
	void	SetGauageBarState(BOOL bState){m_bGauageBarState = bState;}
	BOOL	GetGauageBarState(){return m_bGauageBarState;}

	BOOL SetItem(FLItemElem* pItem);

private:
	enum {  N_BARUNA_ITEM = 0,	//�ٷ糪 ���
			N_ORB,			//�Ӽ�����
			N_GRACE,		//�ٷ糪�� ����
			N_REJU,			//���� Ȱ����
			N_MAX,};

	HRESULT RestoreDeviceObjects();
	HRESULT DeleteDeviceObjects();
	HRESULT InvalidateDeviceObjects();

	CTexture* m_pTexGuage;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBufferGauge;
	int		m_nCurrGauage;
	BOOL	m_bGauageBarState;
	BOOL	m_bSend;
	
	OBJID	m_objID_Confirm;
	CRect	m_rect_Element_Smelt[N_MAX];
	//OBJID	m_objID_Element_Smelt[N_MAX];

	FLItemElem* m_pItem[N_MAX];

	BOOL m_bSendPacket;
	
	void	StartSmelt();
	void	InitCtrl();
	void	Element_Smelt_Check_Orb(FLItemElem* pItemBaruna);
	void	ItemRegister(int nSlotID, FLItemElem* pItem);
	void	ItemConfirmRegister(DWORD dwItemID);
};
//---------------------------------------------------------------------------------------------------------

class CWndBarunaNPC : public CWndNeuz
{
//���ð��� ���� �����츦 ��� �����Ѵ�.
public:
	enum BARUNA_WNDS
	{
		BW_BARUNA,	//�ٷ糪 ����
		BW_OPERCID, //���۽õ�
		BW_OPER,	//����
		BW_CID,		//�õ�
		BW_EXTRACT_OPER, //��������
		BW_BARUNA_PEARCING, //�ٷ糪 �Ǿ��
		BW_BARUNA_WAKEUP,		//�ٷ糪 ����
		BW_BARUNA_WAKEUP_CANCEL,//�ٷ糪 ���� ���
		BW_BARUNA_ELEMENT_SMELT,//�ٷ糪 �Ӽ� ����
#ifdef BARUNA_ULTIMATE_UPDATE
		BW_BARUNA_ULTIMATE_UPDATETRANS
#endif	// BARUNA_ULTIMATE_UPDATE
	};
	CWndBarunaNPC( );
	virtual ~CWndBarunaNPC( );

	virtual BOOL Initialize		( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify	( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw			( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate( );
	virtual BOOL Process		( );
	virtual BOOL OnDropIcon		( LPSHORTCUT pShortcut, CPoint point );

		
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();

	void		MakeGaugeRect	( );

	void		SetNPCInfo		( const OBJID id, DWORD level, DWORD exp );
	CWndNeuz*	CreateWnd		( BARUNA_WNDS eWnd );
	void		ReceivedPacket	( BARUNA_WNDS eWnd, const DWORD dwResult = FSC_ITEMUPGRADE_SYSTEM_ERROR );

	OBJID		GetNpcID( )		{ return m_idNPC; }

protected:
	OBJID	m_idNPC;
	DWORD   m_dwLevel;
	DWORD   m_dwExpRatio;	//exp �����, 0~100
	BOOL	m_bWait;		//��ü�� �����캸�� ���߿� �����ǹǷ�, ��ٷȴ� �����Ѵ�.
	
	CTexture m_texGauFillNormal  ;
	CRect	m_rectEXP;
	int		m_nWidth;
	LPDIRECT3DVERTEXBUFFER9 m_pGaugeEXP;

	//child wnds
	CWndEnchantBaruna*				m_pEnchantBaruna;
	CWndProduceOfferSeed*			m_pOperCid;
	CWndProduceOffer*				m_pOper;
	CWndProduceSeed*				m_pCid;
	CWndExtractOffer*				m_pExtractOper;
	FLWndBarunaPearcing*			m_pPearcing;				//�ٷ糪 �Ǿ��
	CFLWndBaruna_Wakeup*			m_pWakeup;					//�ٷ糪 ����
	CFLWndBaruna_Wakeup_Cancel*		m_pWakeup_Cancel;			//�ٷ糪 ���� ���
	CFLWndBaruna_Element_Smelt*		m_pElement_Smelt;			//�ٷ糪 �Ӽ� ���� 
#ifdef BARUNA_ULTIMATE_UPDATE
	CWndBarunaUltimateTrans*				m_pUltimateTrans;			//�ٷ糪 ���͸� ��ȯ
#endif
};


#endif //__ENCHANT_BARUNA16