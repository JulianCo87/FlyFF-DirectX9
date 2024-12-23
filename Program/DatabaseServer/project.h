#ifndef __PROJECT_H__
#define __PROJECT_H__

#include <ProjectCmn.h>

#include <vector>
#include <map>
#include <EventLua.h>
#include <GuildCombat1to1.h>

using namespace std;



typedef map< string, void* > CMapStrToPtr;
#define	MAX_WORLD	256


#ifdef __S1108_BACK_END_SYSTEM
#define MAX_RESPAWN 1024
#define MAX_MONSTER_PROP 1024
typedef struct	_MONSTER_RESPAWN
{
	char	szMonsterName[32];
	int		nRespawnIndex;
	D3DXVECTOR3	vPos;
	int		nQuantity;
	int		nAggressive;
	int		nRange;
	int		nTime;
	_MONSTER_RESPAWN()
	{	
		szMonsterName[0] = '\0';
		vPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		nQuantity = 0;
		nAggressive = 0;
		nRange = 0;
		nTime = 0;
		nRespawnIndex = 0;
	}
}
MONSTER_RESPAWN, *PMONSTER_RESPAWN;

typedef struct _MONSTER_PROP
{
	char	szMonsterName[32];
	int		nHitPoint;
	int		nAttackPower;
	int		nDefence;
	int		nExp;
	int		nItemDrop;
	int		nPenya;
	_MONSTER_PROP()
	{
		szMonsterName[0] = '\0';
		nHitPoint = 0;
		nAttackPower = 0;
		nDefence = 0;
		nExp = 0;
		nItemDrop = 0;
		nPenya = 0;
	}
}
MONSTER_PROP, *PMONSTER_PROP;
#endif // __S1108_BACK_END_SYSTEM

class CScript;

class CProject
{
public:
	CProject();
	virtual	~CProject();

private:
	map<string, DWORD>	m_mapII;

public:
	CIndexArray<SkillProp>	m_aPropSkill;
	int						m_nMoverPropSize;
	MoverProp*				m_pPropMover;	// m_aPropMover�迭�� �޸� ħ���� �־ ������.04.10.14
	CIndexArray< tagColorText >	m_colorText;

	CEventLua	m_EventLua;

	CGuildCombat1to1Mng m_GuildCombat1to1;

	SkillProp*	m_aJobSkill[MAX_JOB][40];
	DWORD	m_aJobSkillNum[MAX_JOB];
	EXPPARTY	m_aExpParty[MAX_PARTYLEVEL];
	EXPCHARACTER m_aExpCharacter[MAX_EXPCHARACTER];

	FLOAT m_fItemDropRate;			// ���� ������ ��ӷ�
	FLOAT m_fGoldDropRate;			// ���� ��� ��ӷ�
	FLOAT m_fMonsterExpRate;		// ���� ����ġ��
	FLOAT m_fMonsterHitRate;		// ���� ���ݷ�
	FLOAT m_fShopCost;				// ��������

	DWORD	m_dwConvMode;

	
#ifdef __S1108_BACK_END_SYSTEM
	FLOAT m_fMonsterRebirthRate;	// ���� ��������
	FLOAT m_fMonsterHitpointRate;	// ���� ����·�
	FLOAT m_fMonsterAggressiveRate; // �������ͷ�
	FLOAT m_fMonsterRespawnRate;	// ���� ��������
	BOOL  m_bBaseGameSetting;		// ���әV�� �Ϸ�
	BOOL  m_bBackEndSystem;
	MONSTER_RESPAWN	m_aMonsterRespawn[MAX_RESPAWN];		// Respawn
	MONSTER_RESPAWN n_aAddRespawn[MAX_RESPAWN];
	int				m_nAddMonsterRespawnSize;
	UINT			m_nMonsterRespawnSize;			// Respawn Size
	MONSTER_PROP	m_aMonsterProp[MAX_MONSTER_PROP];	// Monster Prop
	MONSTER_PROP	m_aAddProp[MAX_MONSTER_PROP];	// Monster Prop
	char			m_aRemoveProp[MAX_MONSTER_PROP][32];	// Monster Prop
	int				m_nAddMonsterPropSize;
	int				m_nRemoveMonsterPropSize;
	UINT			m_nMonsterPropSize;		// Monster Prop Size
	char			m_chBackEndSystemTime[15];
	char			m_chBackEndSystemChatTime[15];
	char			m_chGMChat[10][256];
#endif // __S1108_BACK_END_SYSTEM

	BOOL			m_bItemUpdate;		// 
	TCHAR	m_apszWorld[MAX_WORLD][64];
	TCHAR	m_apszWorldName[MAX_WORLD][64];
	vector<BEGINITEM>	m_aryBeginItem;
	CMapStrToPtr	m_mapBeginPos;
	JOBITEM		m_jobItem[MAX_JOBITEM];
//	DWORD	m_aExpJobLevel[MAX_EXPJOBLEVEL + 1];

	set<string>		m_sInvalidNames;
	BOOL	LoadInvalidName( void );
	BOOL	IsInvalidName( LPCSTR szName );

	set<char>	m_sAllowedLetter;
#ifdef __VENDOR_1106
	set<char>	m_sAllowedLetter2;
	BOOL	LoadAllowedLetter( BOOL bVendor = FALSE );
#else	// __VENDOR_1106
	BOOL	LoadAllowedLetter( void );
#endif	// __VENDOR_1106
	BOOL	IsAllowedLetter( LPCSTR szName, BOOL bVendor = FALSE );
	void	Formalize( LPSTR szName, size_t cchName );

public:
	bool	LoadDefines();
	bool	LoadPreFiles();
	bool	LoadStrings();
	BOOL	OpenProject( LPCTSTR lpszFileName );
	int		GetBeginItemSize( void );
	LPBEGINITEM	GetBeginItem( int nIndex );
	void	LoadBeginPos( void );
	void	AddBeginPos( const char* lpszWorld, const D3DXVECTOR3 & vPos );
	BOOL	GetRandomBeginPos( DWORD dwWorldID, LPD3DXVECTOR3 pvOut );
	BOOL	LoadPropMover( LPCTSTR lpszFileName );
	BOOL	LoadPropSkill( LPCTSTR lpszFileName, CIndexArray<SkillProp>* apObjProp );
	BOOL	LoadDefOfWorld( LPCTSTR lpszFileName );
	BOOL	LoadJobItem( LPCTSTR lpszFileName );
	BOOL	LoadText( LPCTSTR lpszFileName );

	//////////////////////////////////////////////////////////////////////////
	DWORD	GetSkillPoint( const SkillProp* pSkillProp ) const;
	//////////////////////////////////////////////////////////////////////////

	LPCTSTR GetText( DWORD dwIndex ) 
	{ 
		if( m_colorText.GetAt( dwIndex ) == NULL )
			return "";
		return m_colorText.GetAt( dwIndex )->lpszData; 
	}
	DWORD GetTextColor( DWORD dwIndex ) 
	{ 
		if( m_colorText.GetAt( dwIndex ) == NULL )
			return 0;
		return m_colorText.GetAt( dwIndex )->dwColor; 
	}

	CString GetLangScript( CScript& script );

#if defined(__DBSERVER)
	BOOL	LoadExpTable( LPCTSTR lpszFileName );
#endif	// __DBSERVER
	MoverProp*  GetMoverProp( int nIndex ) 
	{ 
		if( nIndex < 0 || nIndex >= m_nMoverPropSize )	
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "out of range. %d %d" ), nIndex, m_nMoverPropSize );
			return NULL;
		}
		if( m_pPropMover[nIndex].dwID )
			return m_pPropMover + nIndex;
		return NULL;
	}

	SkillProp*	GetSkillProp( int nIndex )
	{
		if( nIndex < 0 && nIndex >= m_aPropSkill.GetSize() )
			return NULL;
		return m_aPropSkill.GetAt( nIndex ); 
	}
};

inline LPBEGINITEM CProject::GetBeginItem( int nIndex )
{	
	return (LPBEGINITEM)&m_aryBeginItem.at( nIndex );	
}

inline int CProject::GetBeginItemSize( void )
{	
	return m_aryBeginItem.size();	
}

#endif	// __PROJECT_H__
