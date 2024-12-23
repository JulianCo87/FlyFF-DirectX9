#ifndef __MISC_H__
#define	__MISC_H__

#ifdef __TRAFIC_1218
#include <network/FLAr.h>
#endif	// __TRAFIC_1218

#include "../../../_CommonDefine/FLTypes.h"


// ���� ��ɰ��� ��Ʈ���� 
struct TAG_ENTRY
{
	u_long		idFrom;				// ���� 
	DWORD		dwDate;				// ���� 
	char		szString[256];		// ���� ���� 
};

const int MAX_TAGS = 20;			// �ִ� ���� ����ġ 

// ��� ���� enum
enum GUILD_STAT
{
	GUILD_STAT_LOGO,
	GUILD_STAT_PXPCOUNT,
	GUILD_STAT_PENYA,
	GUILD_STAT_NOTICE,
};

const int WANTED_MSG_MAX = 20;

// ����� �׸� 
struct WANTED_ENTRY
{
	char		szPlayer[64];						// ����� �̸� 
	long		nEnd;								// ������ 
	__int64		nGold;								// ����ݾ� 
	char		szMsg[WANTED_MSG_MAX + 1];			// ������ 
};


// ��� â�� �α� ��� world,database,neuz
const int GUILDBANKLOGVIEW_MAX = 100;

struct __GUILDBANKLOG_ENTRY
{
	char		szPlayer[42];						// �ɸ��� �̸� 
	long		nDate;								// ��¥ 
	long		nItemID;								// �����۾��̵�,��
	long		nItemAbilityOption;								// �����۾��̵�,��
	long		nItemCount;								// �����ۼ���
	__GUILDBANKLOG_ENTRY()
	{
		szPlayer[0]	= '\0';
		nDate = 0;	
		nItemID = 0;	
		nItemAbilityOption = 0;
		nItemCount = 0;
	};
};

//sun, 11 �ɸ��� ���� �ŷ� ��� world,database,neuz
const int SEALCHAR_MAX = 3;

struct __SEALCHAR_ENTRY
{
	char		szPlayer[42];						// �ɸ��� �̸� 
	u_long		idPlayer;							//  
	long		nPlayerSlot;								//
	__SEALCHAR_ENTRY()
	{
		szPlayer[0]	= '\0';
		idPlayer = 0;	
		nPlayerSlot = 0;	
	};
};


#ifdef __TRAFIC_1218
class CTraficLog	: private std::map<int, int>
{
private:
	DWORD	m_dwTotalPacket;
	DWORD	m_dwTotalBytes;

public:

//	Constructions
	CTraficLog()	
	{
		m_dwTotalPacket	= m_dwTotalBytes	= 0;
	}
	virtual	~CTraficLog()
	{
		Clear();
	}

//	Operations
	void	AddTotal( DWORD dwBytes )
	{
		m_dwTotalPacket++;
		m_dwTotalBytes	+= dwBytes;
	}
	void	Add( int nHdr )
	{
		iterator	i	= find( nHdr );
		if( i == end() )
			insert( value_type( nHdr, 1 ) );
		else
			i->second++;
	}

	void	Serialize( CAr & ar )
	{
		if( ar.IsStoring() )
		{
			ar << m_dwTotalPacket << m_dwTotalBytes;
			ar << (int)size();
			for( iterator i = begin(); i != end(); ++i )
				ar << i->first << i->second;
		}
		else	// load
		{
			Clear();

			ar >> m_dwTotalPacket >> m_dwTotalBytes;
			int nSize;
			ar >> nSize;
			int nHdr;
			int nCount;
			for( int i = 0; i < nSize; i++ )
			{
				ar >> nHdr >> nCount;
				insert( value_type( nHdr, nCount ) );
			}
		}
	}

	void	Clear( void )
	{
		clear();
		m_dwTotalPacket	= m_dwTotalBytes	= 0;
	}

};
#endif	// __TRAFIC_1218

#endif	// __MISC_H__