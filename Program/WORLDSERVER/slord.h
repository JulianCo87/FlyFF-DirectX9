#ifndef __SLORD_H__
#define	__SLORD_H__

#include <lord.h>

////////////////////////////////////////////////////////////////////////////////
class CSLord
	: public CLord
{
public:
	CSLord();
	virtual	~CSLord();
	static	CSLord*	Instance( void );
	virtual	void	CreateColleagues( void );
	virtual void	DestroyColleagues( void );
};

////////////////////////////////////////////////////////////////////////////////
class CSElection :
	public IElection
{
public:
		CSElection( CLord* pLord );
		virtual	~CSElection();

protected:
	virtual	BOOL	DoTestBeginCandidacy( void );
	virtual	BOOL	DoTestBeginVote( int & nRequirement );
	virtual	BOOL	DoTestEndVote( u_long idPlayer );
	virtual	void	DoAddDepositComplete( u_long idPlayer, __int64 iDeposit, time_t tCreate );
	virtual	BOOL	DoTestSetPledge( u_long idPlayer, const char* szPledge );
	virtual	BOOL	DoTestIncVote( u_long idPlayer, u_long idElector );
};

////////////////////////////////////////////////////////////////////////////////
class CLEvent :
	public ILordEvent
{
public:
		CLEvent( CLord* pLord );
		virtual	~CLEvent();
protected:
	virtual	BOOL	DoTestAddComponent( CLEComponent* pComponent );
	virtual	BOOL	DoTestInitialize( void );
};

////////////////////////////////////////////////////////////////////////////////
class CSLordSkill
	: public CLordSkill
{
public:
	CSLordSkill( CLord* pLord );
	virtual	~CSLordSkill();
	virtual	CLordSkillComponentExecutable*	CreateSkillComponent( int nType );	// template method
};

////////////////////////////////////////////////////////////////////////////////
class FLWSUser;
namespace	election
{
	int	AddDepositRequirements( FLWSUser* pUser, __int64 iTotal, __int64 & iDeposit );
	int	SetPledgeRequirements( FLWSUser* pUser, int & nCost );
	int	IncVoteRequirements( FLWSUser* pUser, u_long idPlayer );
};

namespace	lordevent
{
	int	CreateRequirements( FLWSUser* pUser, int iEEvent, int iIEvent );
};

////////////////////////////////////////////////////////////////////////////////


#endif	// __SLORD_H__