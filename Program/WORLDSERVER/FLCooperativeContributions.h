#ifndef __FLCOOPERATIVE_CONTRIBUTIONS_H__
#define __FLCOOPERATIVE_CONTRIBUTIONS_H__

#include "../_Common/post.h"
#include "../_CommonDefine/FLTm.h"
#include "../_CommonDefine/FLSingleton.h"
#include "../_CommonDefine/FLStdContainer.h"

#include "FLQuestInterface.h"
#include "FLCC_Contribution.h"


class FLWUser;


#include <boost/noncopyable.hpp>

//------------------------------------------------- ���� ��� ��ü --------------------------------------------------//
namespace nsCooperativeContributions {

class FLCooperativeContributions : boost::noncopyable//, public FLSingleton< FLCooperativeContributions >
{
public:
	FLCooperativeContributions();
	virtual ~FLCooperativeContributions();

	static FLCooperativeContributions & GetInstance();

	void					Load( const BYTE * pBuffer, const int nBufferSize );
	FLContribution *		FindContribution( const DWORD nContributionID );	//��ο� ���� ������ ��ε� �������� ��ü

	void					TryReward_LoginUser( FLWSUser & User );				//�� �������� ���� : Login�� ����
	void					TryClean_ExpiredGlobalReward();						//���� �Ⱓ ���� üũ �� ����

	void					Send_ContributionInfo( FLWSUser & User );

	// Query
	BOOL					Query_GetContributedInfo( /*const FLContribution & contribution*/ );
	BOOL					Query_GetRanking( const u_long idPlayer, const FLContribution & contribution );
	BOOL					Query_TryWriteRewardInfo();

private:
	typedef					FLMap_DeleteSecond< DWORD/* ContributionID */, FLContribution * >	ContributionMap;

	ContributionMap			m_mapContribution;			// ��� ���� & ���� & ��� ���� ����Ʈ
};

} //nsCooperativeContributions


inline static				nsCooperativeContributions::FLCooperativeContributions & COOPERATIVE_CONTRIBUTIONS()		
{		
	return nsCooperativeContributions::FLCooperativeContributions::GetInstance();		
}

#endif //__FLCOOPERATIVE_CONTRIBUTIONS_H__
