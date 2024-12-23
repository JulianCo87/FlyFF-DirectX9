
#ifndef	__FLEVENTARENAGLOBAL_H__
#define	__FLEVENTARENAGLOBAL_H__


#include "FLEventArena.h"
#include "FLEventArenaSpec.h"


class	FLEventArenaGlobal
{
public:

	static FLEventArenaGlobal*	GetInstance();

private:

	FLEventArenaGlobal();
	~FLEventArenaGlobal();

public:

	bool			Build();
	void			Clear();

	void			SetArenaChannel();
	void			SetConfigFile( const TCHAR* pszConfigFile );

	bool			IsArenaChannel()	const;
	FLEventArena*	GetEventArena()		const;


private:

	bool					m_bIsArenaChannel;			// �Ʒ��� ä���ΰ�?
	TCHAR					m_szConfigFile[MAX_PATH];	// �Ʒ��� ���� ����

	FLEventArenaSpec		m_kSpec;					// �Ʒ��� ����
	FLEventArenaTeamManager	m_kTeamManager;				// �� ����Ʈ
	FLEventArena*			m_pEventArena;				// �Ʒ���

};


#define	g_pEventArenaGlobal		FLEventArenaGlobal::GetInstance()
#define	g_pEventArena			g_pEventArenaGlobal->GetEventArena()





#endif