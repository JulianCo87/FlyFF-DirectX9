#include "stdafx.h"

#include "slord.h"
#include "lordskillexecutable.h"
#include "user.h"

////////////////////////////////////////////////////////////////////////////////
CSLord::CSLord()
:
CLord()
{
}

CSLord::~CSLord()
{
}

CSLord* CSLord::Instance( void )
{
	static CSLord sLord;
	return &sLord;
}

void CSLord::CreateColleagues( void )
{
	CSElection * pElection	= new CSElection( this );
	CLEvent* pEvent		= new CLEvent( this );
	CLordSkill* pSkills		= new CSLordSkill( this );
	pEvent->Initialize( "lordevent.inc" );
	pSkills->Initialize( "lordskill.txt" );
	m_pElection		= pElection;
	m_pEvent	= pEvent;
	m_pSkills	= pSkills;
}

void CSLord::DestroyColleagues( void )
{
	SAFE_DELETE( m_pElection );
	SAFE_DELETE( m_pEvent );
	SAFE_DELETE( m_pSkills );
}

////////////////////////////////////////////////////////////////////////////////
CSElection::CSElection( CLord* pLord )
:
IElection( pLord )
{
}

CSElection::~CSElection()
{
}

BOOL CSElection::DoTestBeginCandidacy( void )
{
	g_xWSUserManager->AddElectionBeginCandidacy();
	return TRUE;
}

BOOL CSElection::DoTestBeginVote( int & nRequirement )
{
	g_xWSUserManager->AddElectionBeginVote( nRequirement );
	return TRUE;
}

BOOL CSElection::DoTestEndVote( u_long idPlayer )
{
	g_xWSUserManager->AddQueryPlayerData( idPlayer );
	g_xWSUserManager->AddElectionEndVote( idPlayer );
	return TRUE;
}

void CSElection::DoAddDepositComplete( u_long idPlayer, __int64 iDeposit, time_t tCreate )
{
	g_xWSUserManager->AddElectionAddDeposit( idPlayer, iDeposit, tCreate );
}

BOOL CSElection::DoTestSetPledge( u_long idPlayer, const char* szPledge )
{
	g_xWSUserManager->AddElectionSetPledge( idPlayer, szPledge );
	return TRUE;
}

BOOL CSElection::DoTestIncVote( u_long idPlayer, u_long idElector )
{
	g_xWSUserManager->AddElectionIncVote( idPlayer, idElector );
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
CLEvent::CLEvent( CLord* pLord )
:
ILordEvent( pLord )
{
}

CLEvent::~CLEvent()
{
}

BOOL CLEvent::DoTestAddComponent( CLEComponent* pComponent )
{
	g_xWSUserManager->AddLEventCreate( pComponent );
	return TRUE;
}

BOOL CLEvent::DoTestInitialize( void )
{
	g_xWSUserManager->AddLEventInitialize();
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
CSLordSkill::CSLordSkill( CLord* pLord )
: CLordSkill( pLord )
{
}

CSLordSkill::~CSLordSkill()
{
}

CLordSkillComponentExecutable* CSLordSkill::CreateSkillComponent( int nType )
{
	switch( nType )
	{
		case SKILL_INDEX( 2, LT_ITEMBUF ):
			return new CLordSkillComponentItemBuf;
		case SKILL_INDEX( 3, LT_SUMMON ):
			return new CLordSkillComponentSummon;
		case SKILL_INDEX( 4, LT_TELEPORT ):
			return new CLordSkillComponentTeleport;
		case SKILL_INDEX( 1, LT_PASSIVE ):
		default:
			return new CLordSkillComponentExecutable;
	}
}

////////////////////////////////////////////////////////////////////////////////
namespace	election
{
	int	AddDepositRequirements( FLWSUser* pUser, __int64 iTotal, __int64 & iDeposit )
	{
//		if( !pUser->IsMaster() && !pUser->IsHero() )
		if( pUser->GetLegendChar() <= LEGEND_CLASS_NORMAL )
		{
			return TID_GAME_ELECTION_ADD_DEPOSIT_E001;	// "�����, ������ ����� ĳ���͸� ���� �ĺ��� ������ �Ͻ� �� �ֽ��ϴ�."
		}
		if( iTotal < CCandidate::nMinDeposit )
			return TID_GAME_ELECTION_ADD_DEPOSIT_E002;	// "���� �ݾ��� �����մϴ�."
		IElection* pElection	= CSLord::Instance()->GetElection();

		if( pElection->GetState() != IElection::eCandidacy )
			return TID_GAME_ELECTION_ADD_DEPOSIT_E004;		//  �ĺ� ��� �Ⱓ�� �ƴմϴ�.

		CCandidate* pCandidate	= pElection->GetCandidate( pUser->m_idPlayer );
		__int64 iOld	= ( pCandidate? pCandidate->GetDeposit(): 0 );
		if( iOld >= iTotal )
			return TID_GAME_ELECTION_ADD_DEPOSIT_E003;	// "���� ��ĺ��� �� ���� �ݾ��� ��û�Ͻñ� �ٶ��ϴ�."
		if( pUser->IsQuerying() )
			return TID_GAME_LORD_IS_QUERYING;

		iDeposit	= iTotal - iOld;
		if( iDeposit > INT_MAX || iDeposit < 0 || pUser->CheckUserGold( ( int ) iDeposit, false ) == false )		//��ȿ ���� : 0 <= iDeposit <= INT_MAX 
		//		if( iDeposit > INT_MAX || pUser->GetGold() < static_cast<int>( iDeposit ) )
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "Check_ - \tPenya:[%I64d]" ), iDeposit );
			return TID_GAME_ELECTION_ADD_DEPOSIT_E002;	// "���� �ݾ��� �����մϴ�."
		}
		if( iDeposit < static_cast<__int64>( 10000000 ) )
			return TID_GAME_ELECTION_ADD_DEPOSIT_E005;	// �߰� ������ �ּ� 10,000,000 ��İ� �ʿ��մϴ�.
		pUser->SetQuerying( TRUE );
		return 0;
	}

	int SetPledgeRequirements( FLWSUser* pUser, int & nCost )
	{
		nCost	= 0;
		IElection* pElection	= CSLord::Instance()->GetElection();
		CCandidate* pCandidate	= pElection->GetCandidate( pUser->m_idPlayer );
		if( pCandidate )
		{
			if( pElection->GetState() != IElection::eVote )
				return TID_GAME_ELECTION_SET_PLEDGE_E003;

			if( lstrlen( pCandidate->GetPledge() ) > 0 )
			{
//				if( pUser->GetGold() < CCandidate::nSetPledgeCost )
				if( pUser->CheckUserGold( CCandidate::nSetPledgeCost, false ) == false )
				{
					return TID_GAME_ELECTION_SET_PLEDGE_E002;
				}
				nCost	= CCandidate::nSetPledgeCost;
			}
			if( pUser->IsQuerying() )
				return TID_GAME_LORD_IS_QUERYING;

			pUser->SetQuerying( TRUE );
			return 0;
		}
		return TID_GAME_ELECTION_SET_PLEDGE_E001;
	}

	int	IncVoteRequirements( FLWSUser* pUser, u_long idPlayer )
	{
		IElection* pElection	= CSLord::Instance()->GetElection();
		CCandidate* pCandidate	= pElection->GetCandidate( idPlayer );
		if( !pCandidate )
			return TID_GAME_ELECTION_INC_VOTE_E001;		// �ĺ��� ���ų� �ĺ����� �߸� �Է� �ϼ̽��ϴ�. Ȯ�� �� �ٽ� �Է��� �ֽñ� �ٶ��ϴ�.
		if( pElection->GetState() != IElection::eVote )
			return TID_GAME_ELECTION_INC_VOTE_E002;		// ��ǥ �Ⱓ�� �ƴմϴ�.
		if( pUser->GetLevel() < IElection::nLevelRequirements )
			return TID_GAME_ELECTION_INC_VOTE_E003;		// �����/������ ���, ĳ���� ���� 60�̻��� ĳ���͸� ��ǥ���� �����ϴ�.
		if( pUser->IsQuerying() )
			return TID_GAME_LORD_IS_QUERYING;
		if( pUser->GetElection() == pElection->GetId() )	// ˬ
			return TID_GAME_ELECTION_INC_VOTE_E004;		// �̹� ��ǥ�� �����ϼ̽��ϴ�.

		pUser->SetQuerying( TRUE );
		return 0;
	}
};

////////////////////////////////////////////////////////////////////////////////
namespace	lordevent
{
	int	CreateRequirements( FLWSUser* pUser, int iEEvent, int iIEvent )
	{
		ILordEvent* pEvent		= CSLord::Instance()->GetEvent();
		if( !CSLord::Instance()->IsLord( pUser->m_idPlayer ) )	// is he lord?
			 return TID_GAME_L_EVENT_CREATE_E001;
		if( pEvent->GetComponent( pUser->m_idPlayer ) )		// is there no event?
			return TID_GAME_L_EVENT_CREATE_E002;
		if( pUser->IsQuerying() )
			return TID_GAME_LORD_IS_QUERYING;
		__int64 iCost	= pEvent->GetCost( iEEvent, iIEvent );
		if( iCost == 0 )
			return TID_GAME_L_EVENT_CREATE_E004;
		if( iCost > pUser->GetTotalGold() )
			return TID_GAME_L_EVENT_CREATE_E003;

		pUser->SetQuerying( TRUE );
		return 0;
	}
};

////////////////////////////////////////////////////////////////////////////////
