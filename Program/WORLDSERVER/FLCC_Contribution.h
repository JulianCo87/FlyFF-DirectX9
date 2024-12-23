#ifndef __FLCONTRIBUTION_H__
#define __FLCONTRIBUTION_H__


#include "../_CommonDefine/FLTm.h"
#include "../_CommonDefine/FLStdContainer.h"
#include "../_Common/post.h"
#include "FLQuestInterface.h"

#include <boost/noncopyable.hpp>

namespace nsCooperativeContributions
{

// ��� ���� // ���� // ��ε� ����
class FLContribution : boost::noncopyable
{
public:
	FLContribution( const DWORD dwContributionID );
	virtual ~FLContribution();

	//BOOL					HasEndCondition( const E_CONTRIBUTION_KIND eContributionKind, const DWORD dwIndexOfKind );
	IGlobalEndCondition *	GetGlobalEndCondition();
	IGlobalEndCondition *	GetGlobalEndCondition( const E_CONTRIBUTION_KIND eContributionKind, const DWORD dwIndexOfKind );	//ó�� ��ȹ�� ���� ������ �� �� �ִٴ� ������ �پ����� �ڵ�

	void					TryRewardAllUser();								// ��� �������� ���� 

	BOOL					IsBeginCondition( FLWSUser & User ) const;		// ��� ������ �Ǵ°�?
	BOOL					IsComplete();									// ��� �Ϸᰡ �Ǿ���?

	BOOL					IsEventPeriod() const;							// �̺�Ʈ �Ⱓ�ΰ�?
	BOOL					IsExpired() const;								// �̺�Ʈ ���� �ΰ�?

	//void					Query_TryRewardItem();							// db�� �Ѱ�� ��� �Ϸᰡ �Ǿ����� üũ�� ���� �Ϸ�ɶ����� �õ��������
	//bool					IsValid_EndCond() const;

//private:
//	void					tryRewardOneUser( FLWSUser & User );

public:
	typedef					FLVector_autoDelete< IUserBeginCondition * >	UserBeginConditionVec;
	typedef					FLVector_autoDelete< IUserReward * >			UserRewardVec;
	//typedef					FLVector_autoDelete< IUserRewards * >			UserRewardsVec;
	typedef					FLVector_autoDelete< IGlobalReward * >			GlobalRewardVec;
	typedef					FLVector_autoDelete< IGlobalEndCondition * >	GlobalConditionVec;			

public:
	// id
	DWORD					m_dwID;

	DWORD					m_dwDescription_TextID;
	DWORD					m_dwEndCond_TextID;

	BOOL					m_bRewarded;

	FLTm					m_tmStartDate;
	FLTm					m_tmEndDate;

	UserBeginConditionVec	m_vecBeginCondition_User;
	UserRewardVec			m_vecReward_User;
	//UserRewardsVec			m_vecRewards_User;
	GlobalRewardVec			m_vecReward_Global;
	GlobalConditionVec		m_vecGlobalCondition;
};


} /* nsCooperativeContributions */
//-------------------------------------------------------------------------------------------------------------------//


#endif //__FLCONTRIBUTION_H__