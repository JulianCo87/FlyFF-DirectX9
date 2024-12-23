#ifndef __FLQUEST_INTERFACE_H__
#define __FLQUEST_INTERFACE_H__

#include <boost/noncopyable.hpp>

// user ���� ����.
struct IUserBeginCondition : boost::noncopyable	{
	virtual	BOOL 		IsValid( FLWSUser & User ) = 0;
};

// Global ���� - �Ϸ� ����
struct IGlobalEndCondition : boost::noncopyable	{
	virtual void		SerializeOut_Info( CAr & ar ) = 0;						// ��� ��ǥ ���� ��Ŷ Seralizer
	//virtual DWORD		GetTextID() const = 0;									// ��� ��ǥ ���� text

	virtual BOOL		Query_Contribute( const DWORD dwContributionID, const FLTm & tmStartDate, const FLTm & tmEndDate, const u_long idPlayer, const TCHAR * szName ) = 0; //��� ����
	virtual BOOL		Contribute( FLWSUser & User ) = 0;						// ���
	virtual void		SetContributed( const DWORD dwContiributedCount ) = 0;	// ��� ���� ����

	//virtual size_t		GetContributionUnit() const = 0;						// �ѹ��� ����ϴ� ����
	virtual BOOL		IsSameInfo( const E_CONTRIBUTION_KIND eContributionKind, const DWORD dwIndexOfKind ) = 0;	// ���� ��� �����ΰ� üũ
	virtual BOOL		IsComplete( const DWORD dwContributionID ) = 0;			// �Ϸ� �Ǿ���?
	//virtual BOOL		TrySetContributedCount( const E_CONTRIBUTION_KIND eContributionKind, const DWORD dwIndexOfKind, const DWORD dwContributedCount ) = 0;
};

// User �����ο��� ����������ϴ� ����
struct IUserReward : boost::noncopyable		{
	virtual void		SerializeOut_Info( CAr & ar ) = 0;						// ���� ���� ��Ŷ Seralizer

//	virtual DWORD		GetRewardTextID() const	= 0;							// ���� ���� text
	virtual void		Query_TryWriteRewardInfo( const DWORD dwContributionID, const FLTm & tmStart, const FLTm & tmEnd ) = 0;		//���� ���� ������ ������ DB�� ����.
	virtual BOOL		TryReward( const DWORD dwContributionID, FLWSUser & User ) = 0;		// ��� �õ��� �Ѵ�.
};



// Global ������ ���õǾ� ���� ���õǸ� �˾Ƽ� ��������
struct IGlobalReward : boost::noncopyable	{
	virtual void		SerializeOut_Info( CAr & ar ) = 0;						// ���� ���� ��Ŷ Seralizer
	//virtual DWORD		GetRewardTextID() const	= 0;							// ���� ���� text

	virtual BOOL		TryReward() = 0;										// ���� �õ�
	virtual void		Clean_ExpiredReward() = 0;									// ���� üũ �� ó��

	//virtual void		RewardNotify( FLWSUser & User )	= 0;					// ���� ��Ŷ �뺸
};





// User �����ο��� ����������ϴ� �������� ����
//struct IUserRewards	: boost::noncopyable	{
//	typedef				std::vector< DWORD >		TextIDVec;
//
//public:
//	virtual BOOL		TryReward( const DWORD dwContributionID, FLWSUser & User ) = 0;
//	const TextIDVec &	GetRewardTextIDList() const		{		return m_vecTextID;		}
//
//protected:
//	void				pushTextID( const DWORD dwTextID )		{		m_vecTextID.push_back( dwTextID );	}
//protected:
//	TextIDVec			m_vecTextID;
//};

#endif //__FLQUEST_INTERFACE_H__