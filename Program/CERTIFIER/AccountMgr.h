// AccountMgr.h: interface for the CAccountMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCOUNTMGR_H__57A5D903_C611_4722_99DD_AAA9FB5A8D57__INCLUDED_)
#define AFX_ACCOUNTMGR_H__57A5D903_C611_4722_99DD_AAA9FB5A8D57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FLDBOverlappedPlus.h"

enum ACCOUNT_CHECK {
	CHECK_OK,				// Ʋ�� ���� ���ų�, ó�� 
	CHECK_1TIMES_ERROR,		// 1ȸ Ʋ��
	CHECK_3TIMES_ERROR,		// 3ȸ Ʋ�� 
};

struct ACCOUNT_CACHE 
{
	TCHAR	szAccount[MAX_ACCOUNT];
	DWORD	m_dwIP;
	int		m_nError;
	time_t	m_tmError;

	ACCOUNT_CACHE()
	{
		szAccount[0]	= '\0';
		m_dwIP			= 0;
		m_nError		= 0;
		m_tmError		= 0;
	}
};

//////////////////////////////////////////////////////////////////////////
// Ư�� Ƚ�� �̻� ��й�ȣ ������ �����ϴ� Ŭ����
// Ŭ���̾�Ʈ�� ���� IP�� �������� �ϱ� ������ �̹��� ������ �ִ�.
// ���� ������ ���� �����ϴ� �������� Ŭ���̾�Ʈ���� ��й�ȣ ������ ���ٸ�,
// ���� ���� IP�̹Ƿ� ���� Ƚ�� ���� �����Ѵ�.
// (���� Account������ DB�����带 �и��ϹǷ� ���� �����忡�� ����Ǿ�� �Ѵٴ� ������ �ִ�.)
// ���� ������ �� Account������ �� �ӽź����� ��Ȯ�� ��õǾ�� ��Ȯ�� ������ ������ �� ���� �� ����.
class CAccountMgr
{
public:

	list< ACCOUNT_CACHE* >	m_cache;		// LRU�� �����Ǵ� ĳ�� ���� 

public:

	CAccountMgr();
	~CAccountMgr();

	ACCOUNT_CHECK	Check( const AUTH_INFO & kAuthInfo );

	void			IncErrorCount();
	void			ResetErrorCount();

};

#endif // !defined(AFX_ACCOUNTMGR_H__57A5D903_C611_4722_99DD_AAA9FB5A8D57__INCLUDED_)
