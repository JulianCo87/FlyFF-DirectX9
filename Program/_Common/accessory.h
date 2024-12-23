#ifndef __ACCESSORY_H__
#define	__ACCESSORY_H__

typedef	struct	_SINGLE_DST
{
	int	nDst;
	int	nAdj;
}	SINGLE_DST;

class CAccessoryProperty
{
public:
	CAccessoryProperty();
	virtual	~CAccessoryProperty();
	static	CAccessoryProperty*	GetInstance();

	BOOL	LoadScript( LPCTSTR szFile );
	BOOL	IsAccessory( DWORD dwItemId );
	DWORD	GetProbability( int nAbilityOption )	{	return m_adwProbability[nAbilityOption];		}

	LPDWORD		GetProbabilityPtr( void )		{	return	m_adwProbability;	}

	vector<SINGLE_DST>*	GetDst( DWORD dwItemId, int nAbilityOption );
private:
	DWORD	m_adwProbability[_MAX_ENCHANT_LEVEL_ACCESSORY];
	map<DWORD, vector<SINGLE_DST>*>	m_mapAccessory;
};
#endif	// __ACCESSORY_H__