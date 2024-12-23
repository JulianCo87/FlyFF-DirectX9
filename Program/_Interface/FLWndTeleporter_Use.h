#pragma  once

class CFLWndChangeName;

class CFLWndTeleporter_Use : public CFLWndTeleporterMng
{
public:
	CFLWndTeleporter_Use(CTexture* pTexMap, DWORD dwItemObjID, std::vector< T_TELEPORTMAP_POINT > vecUserPoint);
	~CFLWndTeleporter_Use();

	virtual BOOL OnChildNotify	( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw			( C2DRender* p2DRender,BYTE BySelMapLocationID,CPoint pointMouse  ); 
	virtual	void PaintFrame		( C2DRender* p2DRender );
	virtual	void OnInitialUpdate( );
	virtual BOOL OnDropIcon		( LPSHORTCUT pShortcut, CPoint point );
	virtual void OnRButtonDown  ( UINT nFlags, CPoint point );
	virtual void OnLButtonDown  ( UINT nFlags, CPoint point );
	virtual	BOOL Process		( void );
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnMouseWndSurface( CPoint point );

	void SetAddUserPoint(T_TELEPORTMAP_POINT tUsePoint);
	void SetDelUserPoint(T_TELEPORTMAP_POINT tUsePoint);
	void SetDelAllUserPoint();
	void SetChangeUserPointName(T_TELEPORTMAP_POINT tUsePoint);
	void OpenChangeName();
	T_TELEPORTMAP_POINT GetetAddUserPoint();
	void SendPacket_AddPoint();
	void RenderAddUserPoint( C2DRender* p2DRender, 
  							BYTE BySelMapLocationID,
							CPoint pointMouse,
							CRect rectRevisedMapPosition,
							float fRevisedMapSizeRatio );

	void Req_ChangePointName(CString strChangeName);

private:
	void InitializeAddPointInformation();
	void SendPacket();// �ڷ���Ʈ �̵��� ��û�� 
	void Req_DelPoint();
	void Req_DelAllPoint();

private:
	DWORD					m_dwItemObjID;
	CTexture*				m_pTexTeleport_User;
	CWndMenu				m_menuUsePosition;				//�˾��޴� 
	int						m_nTelID;						// �ڷ���Ʈ �ε��� 	
	int						m_nChangeId;
	CRect					m_rectIconPT;					// ������ ��ǥ
	T_TELEPORTMAP_POINT		m_tUserPoint;
	BOOL					m_bIsChangeName;
	CString					m_strChangeName;
	FLPacketTeleportMapToPointReq		m_kTeleporter;		//�ڷ���Ʈ ��û 
	FLPacketTeleportMapAddMyPointReq	m_kAddMyPoint;		//���� ��ġ ��� ��û
	FLPacketTeleportMapDelPointReq		m_kDelPoint;		//������û
	FLPacketTeleportMapDelAllPointReq	m_kDelAllPoint;		//��� ���� ��û
	FLPacketTeleportMapChgPointNameReq	m_kChangePointName;	//�̸� ���� ��û

	TeleportationIconInfo*				m_pTeleportIconInfo;	//��ﶧ ������ �ʿ��� ����

	std::vector< TeleportationIconInfo* >	m_vecTeleportationPositionRect_User;
	std::vector< T_TELEPORTMAP_POINT >		m_vecUserPoints;
	CFLWndChangeName*						m_pWndChangeName;
};

class CFLWndChangeName : public CWndNeuz
{
public:
    CFLWndChangeName();
	~CFLWndChangeName();

	virtual	void OnInitialUpdate( );
	virtual BOOL Initialize		( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify	( UINT message, UINT nID, LRESULT* pLResult ); 
	CString		GetChageName(){return m_strChangeName;}

private:
	CString		m_strChangeName;
	CWndEdit* m_pWndEditChangeName;
	
};