#ifndef __FLYFFEVENT_H__
#define	__FLYFFEVENT_H__

#define	MAX_EVENT	1024

enum
{
	EVE_0401A, EVE_18, ENABLE_GUILD_INVENTORY,
	EVE_GUILDWAR,
	EVE_SCHOOL, EVE_SCHOOL_BATTLE,
	EVE_NOFLYMONSTER, // ���� ���� ���� 
	EVE_NODARKON, // ���� ���� 
	EVE_NOGUILD, // ��� ���� 
	EVE_WORMON,
	EVE_RESPAWN,
	EVE_PK,
	EVE_PKCOST,
	EVE_STEAL,
	EVE_GUILDCOMBAT,
	EVE_DROPITEMREMOVE,
	EVE_EVENT1206,
	EVE_EVENT1219,
	EVE_EVENT0127,
	EVE_EVENT0214,
	EVE_RECOMMEND,				//sun: 10, __S_RECOMMEND_EVE
	EVE_GUILDCOMBAT1TO1,		//sun: 11, �ϴ��� ��� ����

	EVE_ARENA, 

	EVE_SECRETROOM,	// 12�� ����� ��
	EVE_RAINBOWRACE,
};

#define	EVE_FLAG1219	(DWORD)0x00000001
#define	EVE_FLAG0127	(DWORD)0x00000002
#define	EVE_FLAG0214	(DWORD)0x00000004

class CFlyffEvent
{
private:
	BYTE	m_aEvent[MAX_EVENT];
public:
//	Constructions
	CFlyffEvent();
	~CFlyffEvent()	{}
//	Operations
	BOOL	SetState( int id, BYTE nState );
	BYTE	GetState( int id );
	BOOL	ClearEvent( int id )	{	return SetState( id, 0 );	}

	void	Serialize( CAr & ar );
};

#endif	// __FLYFFEVENT_H__