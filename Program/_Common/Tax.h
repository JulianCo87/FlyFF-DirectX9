#pragma once

//sun: 12, ����
#include "Continent.h"

#define TAX_SALES		(BYTE)0	// �Ǹ� ����
#define	TAX_PURCHASE	(BYTE)1 // ���� ����
#define	TAX_ADMISSION	(BYTE)2 // �����(õ���� ž)

#ifdef __DBSERVER
#include "../DatabaseServer/dbcontroller.h"

enum {	QUERY_TAX_LOAD, 
		QUERY_TAX_PAY,
		QUERY_TAX_CHANGENEXT,
		QUERY_TAX_SETNEXT_SECRETROOMWINGUILD, 
		QUERY_TAX_SETNEXT_LORD,
		QUERY_TAX_SETNEXT_TAXRATE,
		QUERY_TAX_ADDTAX,
};

class CTaxDBController : public CDbController
{
public:
	CTaxDBController(void);
	~CTaxDBController(void);

	virtual	void	Handler( LPDB_OVERLAPPED_PLUS pov, DWORD dwCompletionKey );
	virtual void	OnTimer();

private:
	void PayTaxToPost();	// ���� ���� ������ �������� ������.
	void LoadTaxInfo();		// DB�� ����� ���� ������ �ε� �� ���� ������ ����
	void InsertToDB();		// ����� ������ DB�� �߰�
	void UpdateToDB( BYTE nContinent ); // �ش� ����� ���� ������ ����
	void UpdateAllToDB();

	int m_nTimes;			// ȸ��
	BOOL m_bLoadTaxInfo;		// DB�� ���� �ε��ߴ°�?
};
#endif // __DBSERVER


struct __TAXDETAIL
{
	int nTaxRate;
#ifdef __DBSERVER
	int nTaxCount;
	int	nTaxGold;
	int nTaxPerin;
	int	nNextTaxRate;
#endif // __DBSERVER
	__TAXDETAIL()
	{
		nTaxRate = 0;
#ifdef __DBSERVER
		nTaxCount = 0;
		nTaxGold = 0;
		nTaxPerin = 0;
		nNextTaxRate = 0;
#endif // __DBSERVER
	}
};
typedef map<BYTE, __TAXDETAIL*> TAXDETAILMAP;

struct __TAXINFO
{
	DWORD			dwCurrPlayerID;
	BOOL			bSetTaxRate;
	TAXDETAILMAP	mapTaxDetail;
	DWORD			dwNextPlayerID;
	
	__TAXINFO()
	{
		dwCurrPlayerID	= NULL_ID;
		bSetTaxRate		= TRUE;
		mapTaxDetail.insert( make_pair( TAX_SALES, new __TAXDETAIL ) );			// ���� ������ �⺻
		mapTaxDetail.insert( make_pair( TAX_PURCHASE, new __TAXDETAIL ) );		// �Ǹ� ������ �⺻
		dwNextPlayerID	= NULL_ID;
	}
};
typedef map<BYTE, __TAXINFO*> TAXINFOMAP;


class CTax
{
public:
	CTax(void);
	~CTax(void);

	static CTax* GetInstance( void );
	void Serialize( CAr & ar );
	
	int m_nMinTaxRate;					// �ּ� ����
	int m_nMaxTaxRate;					// �ִ� ����

	TAXINFOMAP m_mapTaxInfo;	// �� ����� ���ɱ�� ���� ���� �� ���� ����

	__TAXINFO* GetTaxInfo( BYTE nContinent );
	void SetNextSecretRoomGuild( BYTE nCont, DWORD dwGuildId );
	void SetNextLord( DWORD dwIdPlayer );
	void SetNextTaxRate( BYTE nCont, int nSalesTaxRate, int nPurchaseTaxRate );
	void SetApplyTaxRateNow();	// GM������� ������ ���� ������ �����Ѵ�.
	
	void	UpdateTaxRate( const int nTaxRate );

	BOOL AddTax( BYTE nCont, int nTax, BYTE nTaxKind );

#ifdef __WORLDSERVER
	void SendSetTaxRateOpenWnd( BYTE nCont, DWORD dwGuildId );
	void SendNoSetTaxRateOpenWnd( FLWSUser* pUser );
#endif // __WORLDSERVER

#ifdef __DBSERVER
	void LoadScript();
	BOOL CheckPayTime();		// ���� ���� ���� �ð� �˻�		
	void CheckChangeTime( BOOL bPay, BOOL bGameMaster = FALSE );		// ���� ���� ���� �ð� �˻�
	void SetChangeNextTax();	// ���� ���� ����
	float GetEarningRate( BYTE nCont, BYTE nTaxKind ); // ���ͷ�
	void LoadTaxInfo( DWORD dpId );

	void	SetUpdateTaxRate( const int nTaxRate );

	int m_nTaxSecretRoomRate;		// ����� �� ���ɱ�� ���ͷ�
	int m_nTaxLordRate;				// ���� ���� ���ͷ�
	int m_nAdmissionSecretRoomRate;	// ���� ���ɱ�� ����� ���ͷ�
	int m_nAdmissionLordRate;		// ���� ����� ���ͷ�
	int m_nDBSaveCount;				// DB�� ���� ��(�Ǽ�)
	
	CLuaBase m_Lua;
	CTaxDBController m_taxDBController;
	string	m_strChangedDate;		// ���������� ������ ����� �ð�
#endif // __DBSERVER

#ifndef __DBSERVER
	// �Ʒ� ��� pMover�� ���� �Ǹ����̴�.( �Ǹ��ڰ� NPC�� ��� pMover�� NPC�̴�. )
	BYTE	GetContinent( CMover* pMover );
	float	GetSalesTaxRate( BYTE nContinent );
	float	GetSalesTaxRate( CMover* pMover );
	float	GetPurchaseTaxRate( BYTE nContinent );
	float	GetPurchaseTaxRate( CMover* pMover );
	bool	IsApplyPurchaseTaxRate( CMover* pMover, FLItemElem* pItemElem );
	bool	IsApplySalesTaxRate( CMover* pMover, FLItemElem* pItemElem );
private:
	bool	IsApplyTaxRate( CMover* pMover, FLItemElem* pItemElem );	// ���������� ������ �������ΰ�?
public:
#endif // __DBSERVER

	vector<DWORD> m_vecdwOccupationShopItem;

#ifdef __WORLDSERVER
	BOOL IsOccupationGuildMember( CMover* pMover );
	BOOL IsOccupationShopItem( DWORD dwItemId );
#endif // __WORLDSERVER
};
