#pragma once

//#include "Query.h"


class FLDBTableInfo
{
//private:
//	class FLDBTableData
//	{
//		//�ѻ���� ��� offset�� �����.
//		BYTE *					m_pData;
//		size_t					m_uDataSize;
//	};

public:
	FLDBTableInfo()					{}
	virtual ~FLDBTableInfo()		{}

	const CQuery::FLColumnInfo &	GetColumnInfo( const char * szColumn );
	const CQuery::FLColumnInfo &	GetColumnInfo( const int nColumnIndex );
	
private:
	BOOL							loadColumnInfo();
	BOOL							loadData();

	// Send TableInfo( )
	// MakeSelectQuery( ��Ŷ )
    
private:
	typedef							std::map< std::string, CQuery::FLColumnInfo >	ColumnInfoMap;

//	ColumnInfoMap					m_mapColumnInfo;
	CQuery::ColumnInfoVec			m_vecColumnInfo;
//	FLDBTableData					m_TableData;
};