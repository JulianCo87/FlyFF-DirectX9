
#pragma once 

//---------------------------------------------------------------------------------------------
// ���� ���� �Է� �޽��� �ڽ�
// ���� :	APP_TRADE_GOLD ���ø��� �⺻���� ����������ϴ�. ���� ���� �����ϱ⿡ Resource ���� ��
//			�۾��ð��� �����Ű�� ���ؼ� �Դϴ�. �������ּ���...
//---------------------------------------------------------------------------------------------
class CWndCommonInputCount	:	public CWndNeuz
{

private:


	CWndEdit*									m_pWndEditCount;

	BYTE										m_bySlot;		// ���࿡�� ���� ���� ��ȣ ���� �뵵
	int											m_nCount;		// �Է� �� ����										

	SHORTCUT									m_stShortcut;

public:


												CWndCommonInputCount( void );
	virtual										~CWndCommonInputCount( void );

	virtual BOOL 								Initialize(CWndBase* pWndParent = NULL, DWORD dwWndId = 0);

	virtual	void 								OnInitialUpdate( void );
	virtual BOOL								OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 

	void										SetInputCountInfo( const SHORTCUT* const pShortcut, const BYTE bySlot = 0 );


private:


	bool										IsValidShortCutInfo( void );

	BOOL										ProcessPrivateBank( void );
	bool										ProcessChildNotifyFromPrivateBank( void );										

	BOOL										ProcessGuildBank( void );
	bool										ProcessChildNotifyFromGuildBank( void );										

	BOOL										ProcessMobileBag( void );
	bool										ProcessChildNotifyFromMobileBag( void );										


};