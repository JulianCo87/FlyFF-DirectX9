
//date : 2010_07_12
//author : gmpbigsun
//note : managing delta time

#pragma  once

class GlobalTickManager
{
	/// �����Ӱ� �ð��� ������
public:
	GlobalTickManager( );
	virtual ~GlobalTickManager( );

	static GlobalTickManager* GetThisPtr();
	
	void Update( );
	DWORD GetDelta( ) { return _dwDelta; }
	void GetHMSbySec( int sec, OUT int& h, OUT int& m, OUT int& s );
	void GetDHMbySec( int sec, OUT int& d, OUT int& h, OUT int& m );

protected:

	DWORD _dwLastTime;
	DWORD _dwDelta;		//�� �����Ӱ��� �ð���
};

_inline DWORD	_GetDeltaTime( ) 	{ return GlobalTickManager::GetThisPtr()->GetDelta(); }
_inline void	_UpdateGTM( )	   	{ GlobalTickManager::GetThisPtr()->Update(); }
_inline float	_GetDeltaTimeSec( ) { return (float)_GetDeltaTime() * 0.001f; }
_inline void	_GetHMS_bySec( int sec, OUT int& h, OUT int& m, OUT int& s ) { GlobalTickManager::GetThisPtr()->GetHMSbySec( sec, h, m, s ); }
_inline void	_GetDHM_bySec( int sec, OUT int& d, OUT int& h, OUT int& m ) { GlobalTickManager::GetThisPtr()->GetDHMbySec( sec, d, h, m ); }


typedef std::vector< std::string >	StringContainer;
typedef StringContainer::iterator	StringIter;
class EAPlayers
{
	//Event Arena Players
public:
	enum
	{
		EA_NOTEAM,			//������
		EA_REDTEAM,			//���������(��) ������
		EA_BLUETEAM			//���������(��) �����
	};

	EAPlayers( ) { }
	virtual ~EAPlayers( ) { }

    static EAPlayers* GetThisPtr( );

	int GetTeam( const std::string& strName );
	BOOL IsRedTeam( const std::string& strName );
	BOOL IsBlueTeam( const std::string& strName );
	void SetRedTeam( const StringContainer& vecPlayers )	{ m_vecRedTeamMembers = vecPlayers; }
	void SetBlueTeam( const StringContainer& vecPlayers )	{ m_vecBlueTeamMembers = vecPlayers; }

	void Update( );		//���������� ������ ����
	void UpdatePlayer( CMover* pMover );

	BOOL IsEnemy( const int nTeam, CMover* pMover );

protected:
	StringContainer	m_vecRedTeamMembers;
	StringContainer	m_vecBlueTeamMembers;
};

_inline EAPlayers* g_EAPlayersPTR()	{ return EAPlayers::GetThisPtr(); }