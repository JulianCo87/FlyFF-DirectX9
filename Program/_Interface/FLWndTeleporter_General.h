#pragma once


class CFLWndTeleporter_General : public CFLWndTeleporterMng
{
public:
	CFLWndTeleporter_General(CTexture* pTexMap,DWORD dwItemObjID);
	~CFLWndTeleporter_General();

	virtual BOOL OnChildNotify	( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual	void OnInitialUpdate( );
	virtual void OnRButtonDown  ( UINT nFlags, CPoint point );
	virtual void OnLButtonDown  ( UINT nFlags, CPoint point );
	virtual	BOOL Process		( void );
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnMouseWndSurface( CPoint point );

	void SendPacket();
	FLPacketTeleportMapToPointReq GetPacket(){return m_kTeleporter;}
	void RenderTeleportationPosition( C2DRender* p2DRender, 
									  BYTE BySelMapLocationID, 
									  CPoint pointMouse,
									  CRect rectRevisedMapPosition,
									  float fRevisedMapSizeRatio);

private:
	
	void InitializeTeleportInformation();								//�ڷ���Ʈ ���� ����

private:
	CTexture*		m_pTexTeleport_General;
	FLPacketTeleportMapToPointReq m_kTeleporter;

	vector< TeleportationIconInfo* > m_vecTeleportationPositionRect;
	TeleportationIconInfo*				m_pTeleportIconInfo;	//��ﶧ ������ �ʿ��� ����

	DWORD			m_dwItemObjID;	
	CRect			m_rectIconPT;	// ������ ��ǥ
	BYTE			m_byItemType;	// �Ϲ��� or ��������� 
	int				m_nTelID;		// �ڷ���Ʈ �ε��� 	
};
