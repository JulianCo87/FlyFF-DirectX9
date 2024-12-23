#pragma once

const int		DEFAULT_ORIGIN_STAT_VALUE	= 15;

class Stat
{
public:
	enum	{ StatDice_None = 0, StatDice_Set, StatDice_Reset	};
public:
	Stat();
	virtual ~Stat();
	Stat&	operator=( const Stat& rhs );
	void	Serialize( CAr & ar );

	void	InitializeOriginStat();
	void	InitializeDiceStat();
	
	int		GetStr()					{	return GetOriginStr() + GetDiceStr();	}
	int		GetSta()					{	return GetOriginSta() + GetDiceSta();	}
	int		GetDex()					{	return GetOriginDex() + GetDiceDex();	}
	int		GetInt()					{	return GetOriginInt() + GetDiceInt();	}

	void	SetOriginStr( int _str )	{	SetOriginStat( Str, _str );	}
	void	SetOriginSta( int _sta )	{	SetOriginStat( Sta, _sta );	}
	void	SetOriginDex( int _dex )	{	SetOriginStat( Dex, _dex );	}
	void	SetOriginInt( int _int )	{	SetOriginStat( Int, _int );	}
	int		GetOriginStr()				{	return GetOriginStat( Str );	}
	int		GetOriginSta()				{	return GetOriginStat( Sta );	}
	int		GetOriginDex()				{	return GetOriginStat( Dex );	}
	int		GetOriginInt()				{	return GetOriginStat( Int );	}

#ifdef __DBSERVER	// DB �ε���
	void	SetDiceStr( int _str )		{	
		if(_str < 0){
			_str = 0;
		}
		SetDiceStat( Str, _str );	
	}
	void 	SetDiceSta( int _sta )		{	
		if(_sta < 0){
			_sta = 0;
		}
		SetDiceStat( Sta, _sta );	
	}
	void	SetDiceDex( int _dex )		{	
		if(_dex < 0){
			_dex = 0;
		}
		SetDiceStat( Dex, _dex );	
	}
	void	SetDiceInt( int _int )		{	
		if(_int < 0){
			_int = 0;
		}
		SetDiceStat( Int, _int );	
	}
#endif // __DBSERVER
	
	int		GetDiceStr()				{	return GetDiceStat( Str );	}
	int		GetDiceSta()				{	return GetDiceStat( Sta );	}
	int		GetDiceDex()				{	return GetDiceStat( Dex );	}
	int		GetDiceInt()				{	return GetDiceStat( Int );	}

	bool	SetRemainStatPoint( const int _RemainStatPoint );

	long	GetRemainStatPoint() const		{	return m_RemainStatPoint;	}

private:
	enum	StatType	{	LoopBaseStatType = 0, Str = 0, Sta, Dex, Int, MaxStatType };
	
	int*	GetOriginStatPtr( StatType st );
	int		GetOriginStat( StatType st );
	void	SetOriginStat( StatType st, int value );
	
	int*	GetDiceStatPtr( StatType st );
	int		GetDiceStat( StatType st );
	void	SetDiceStat( StatType st, int value );

#ifdef __WORLDSERVER
// Stat Dice ������ ��� ����..
public:
	void	ApplyRandomDiceStats( int job );
	bool	ResetDiceStat();
private:
	int		GetVirtualJob( int job );
	BYTE	gettable( int job );
	bool	IsStatUp( int job, StatType st, const BYTE table );
#endif // __WORLDSERVER

private:
	// Origin Stat
	int		m_OriginStr;
	int		m_OriginSta;
	int		m_OriginDex;
	int		m_OriginInt;
	
	// Dice Stat
	int 	m_DiceStr;
	int		m_DiceSta;
	int		m_DiceDex;
	int		m_DiceInt;

	long	m_RemainStatPoint;
};