// FunnyCoin.h: interface for the CFunnyCoin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUNNYCOIN_H__1CC36555_32D6_40AD_A74E_8B5DFA2F411F__INCLUDED_)
#define AFX_FUNNYCOIN_H__1CC36555_32D6_40AD_A74E_8B5DFA2F411F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifdef __WORLDSERVER
//////////////////////////////////////////////////////////////////////
// CFunnyCoin ////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class CFunnyCoin  
{
public:
	CFunnyCoin();
	~CFunnyCoin();
	static CFunnyCoin* GetInstance();

	BOOL	DoUseFunnyCoin( FLWSUser* pUser, FLItemElem* pItemElem );		// �۴������� ����Ѵ�.
	void	OnFunnyCoinAckUse( CAr & ar );	// �۴����� ��뿡 ���� ������ TranServer�� ���� �޾Ҵ�.
		
private:
	BOOL	IsUsable( FLWSUser* pUser );		// �۴������� ����� �� �ִ� Player�ΰ�?
	BOOL	IsAckWaitPlayer( DWORD dwPlayerId, BOOL bRemove = FALSE );	// �۴������� ����� �� DB������ ��ٸ��� �ִ� �÷��̾� �ΰ�?
																		// bRemove�� TRUE�̸� �ش� ����ڸ� ����Ʈ���� �����Ѵ�.
	vector<DWORD> m_vecAckWaitPlayer;				// �۴����� ��� �� DB������ ��ٸ��� �÷��̾� ���
};
#endif // __WORLDSERVER


#ifdef __DBSERVER
//////////////////////////////////////////////////////////////////////
// CFunnyCoinDbCtrl //////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#include "../DatabaseServer/dbcontroller.h"
class CFunnyCoinDbCtrl : public CDbController
{
public:	
	enum { FC_USE = 0 };

	CFunnyCoinDbCtrl();
	virtual ~CFunnyCoinDbCtrl();

	virtual void Handler( LPDB_OVERLAPPED_PLUS pov, ULONG_PTR dwCompletionKey );

private:
	int InsertFunnyCoin( DWORD dwPlayerId, DWORD dwItemId, SERIALNUMBER iSerialNumber );	// ����� �۴������� DB�� �����Ѵ�.
};


//////////////////////////////////////////////////////////////////////
// CFunnyCoinDbMng ///////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class CFunnyCoinDbMng
{
public:
	CFunnyCoinDbMng();
	~CFunnyCoinDbMng();
	static CFunnyCoinDbMng* GetInstance();

	BOOL PostRequest( int nQuery, BYTE* lpBuf = NULL, int nBufSize = 0, ULONG_PTR dwCompletionKey = 0 )
	{ return m_FCDbCtrl.PostRequest( nQuery, lpBuf, nBufSize, dwCompletionKey );	}

private:
	CFunnyCoinDbCtrl	m_FCDbCtrl;
};
#endif // __DBSERVER


#endif // !defined(AFX_FUNNYCOIN_H__1CC36555_32D6_40AD_A74E_8B5DFA2F411F__INCLUDED_)
