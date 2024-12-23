// CampusHelper.h: interface for the CCampusHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMPUSHELPER_H__8A983495_B49A_40BF_8D99_6663B0F63C26__INCLUDED_)
#define AFX_CAMPUSHELPER_H__8A983495_B49A_40BF_8D99_6663B0F63C26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Campus.h>
#include <playerdata.h>

#define		COMPLETE_PUPIL_LEVEL	75
#define		MIN_MASTER_LEVEL		91
#define		REMOVE_CAMPUS_POINT		5

class CCampusHelper  
{
public:
struct REWARD
{
	int nMasterReward;
	int nPupilReward;
	REWARD() : nMasterReward( 0 ), nPupilReward( 0 )	{}
};
typedef map<int, REWARD> MAP_CREWORD;
typedef map<int, DWORD> MAP_CBUFF;

	CCampusHelper();
	~CCampusHelper();

	static CCampusHelper* GetInstance();

	void	Serialize( CAr & ar )	{	m_pCampusMng->Serialize( ar );	}
	u_long	AddCampus( CCampus* pCampus )	{	return m_pCampusMng->AddCampus( pCampus );	}
	BOOL	RemoveCampus( u_long idCampus )	{	return m_pCampusMng->RemoveCampus( idCampus );	}
	
	CCampus*	GetCampus( u_long idCampus )	{	return m_pCampusMng->GetCampus( idCampus );	}

	bool	AddPlayerId2CampusId( u_long idPlayer, u_long idCampus )	{	return m_pCampusMng->AddPlayerId2CampusId( idPlayer, idCampus );	}
	void	RemovePlayerId2CampusId( u_long idPlayer )	{	m_pCampusMng->RemovePlayerId2CampusId( idPlayer );	}
	u_long	GetCampusIdByPlayerId( u_long idPlayer )	{	return m_pCampusMng->GetCampusIdByPlayerId( idPlayer );	}

	void	Clear();
	void	OnAddCampusMember( CAr & ar );
	void	OnRemoveCampusMember( CAr & ar );
	void	OnUpdateCampusPoint( CAr & ar );
	void	OnUpdatePlayerData( u_long idPlayer, PlayerData* pPlayerData );

	void	LoadScript();
	BOOL	IsInviteAble( FLWSUser* pRequest, FLWSUser* pTarget );
	BOOL	IsMasterLevel( FLWSUser* pUser );
	BOOL	IsPupilLevel( FLWSUser* pUser );
	int		GetMaxPupilNum( FLWSUser* pUser );
	BOOL	IsCompleteCampusQuest( FLWSUser* pUser );
	void	AddAllMemberUpdateCampus( CCampus* pCampus );
	void	AddAllMemberRemoveCampus( CCampus* pCampus );
	DWORD	GetCampusBuff( int nLevel );
	void	RecoveryCampusPoint( FLWSUser* pUser, DWORD dwTick );
	void	SetLevelUpReward( FLWSUser* pUser );
	REWARD* GetReward( int nLevel );

	void	OnInviteCampusMember( FLWSUser* pRequest, FLWSUser* pTarget );
	void	OnAcceptCampusMember( FLWSUser* pRequest, FLWSUser* pTarget );
	void	OnRemoveCampusMember( FLWSUser* pRequest, u_long idTarget );

private:
	CCampusMng* m_pCampusMng;
	DWORD		m_dwRecoveryTime;
	int			m_nRecoveryPoint;

	vector<int>	m_vecCQuest;
	MAP_CBUFF	m_mapCBuff;
	MAP_CREWORD	m_mapCReward;
};

#endif // !defined(AFX_CAMPUSHELPER_H__8A983495_B49A_40BF_8D99_6663B0F63C26__INCLUDED_)
