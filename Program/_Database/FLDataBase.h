#pragma once

#include "Query.h"

class FLQueryCommand;


class FLDBChild;

class FLDataBase
{
public:
	FLDataBase();
	virtual ~FLDataBase();
	
	BOOL					Start( const size_t uDBThreadCount, const TCHAR * szDSNName, const TCHAR * szID, const TCHAR * szPW );
	void					Stop();

	void					PostQueryCommand( const unsigned long uKey, FLQueryCommand * pCommand );
	CQuery *				GetQuery( const unsigned long uKey );			//��Ȥ blocking ��� ���� �����Ҷ� �ʿ�. ���� Init�Ҷ� load�ϴ� ������.. �����ϴ� ��� ����

private:
	FLDBChild *				m_parDB;
	size_t					m_uDBThreadCount;
};



class FLDBChild
{
	friend FLDataBase;

	enum					{ 	MAX_CONNECT_STRING = 256,	};
	enum					E_COMMAND
	{
		DB_EXIT		= 0,
		DB_QUERY,
	};

public:
	FLDBChild();
	virtual ~FLDBChild();

	//��õ� ��ƾ
	BOOL					Start( const TCHAR * szDSNName, const TCHAR * szID, const TCHAR * szPW );
	void					Stop();

	void					PostQueryCommand( FLQueryCommand * pCommand );



protected:
	BOOL					connect( const TCHAR * szDSNName, const TCHAR * szID, const TCHAR * szPW );
	static unsigned	WINAPI	ThreadProc( void * pVoid );
	
private:
	HANDLE					m_hThread;
	HANDLE					m_hIOCP;
    
	TCHAR					m_szDSNName[ MAX_CONNECT_STRING ];
	TCHAR					m_szID[ MAX_CONNECT_STRING ];
	TCHAR					m_szPW[ MAX_CONNECT_STRING ];
	CQuery					m_Query;
};



class FLCharacterDB : public FLDataBase
{
public:
	static FLCharacterDB &		GetInstance() {			static FLCharacterDB inst;	return inst;	}
};
inline static FLCharacterDB &	CHARACTER_DB(){		return FLCharacterDB::GetInstance();		}


class FLLogDB : public FLDataBase
{
public:	
	static FLLogDB &			GetInstance() {			static FLLogDB inst;	return inst;		}
};
inline static FLLogDB &			LOG_DB(){			return FLLogDB::GetInstance();				}


