#pragma once

//class FLDropPenalty_ByLevelDist
class FLDropPenalty_ByLevelDist
{
public:
	struct FLPenaltyRate_Data
	{
		FLPenaltyRate_Data( const int levelDist, const float itemRate, const float goldRate ) 
			: nLevelDist( levelDist ), fItemRate( itemRate ), fGoldRate( goldRate )
		{}

		int					nLevelDist;
		float				fItemRate;
		float				fGoldRate;
	};

public:
	FLDropPenalty_ByLevelDist();
	virtual ~FLDropPenalty_ByLevelDist();

	static FLDropPenalty_ByLevelDist & GetInstance();

	void					Load();

	//����� ����� DeclineProb�� �����Ѵ�
	float					GetDeclineProb_Item( const int nAttackerLevel, const int nDroperLevel );
	float					GetDeclineProb_Gold( const int nAttackerLevel, const int nDroperLevel );

public:
	typedef					std::vector< FLPenaltyRate_Data >		FLPenaltyRateVec;

	FLPenaltyRateVec		m_vecPenaltyRate;
	//FLPenaltyRateVec		m_vecDeclineDropRate_Gold;
};

inline static				FLDropPenalty_ByLevelDist & DROPPENALTY_BYLEVELDIST()		{ return FLDropPenalty_ByLevelDist::GetInstance();	}