#ifndef __AI_BIGMUSCLE_H
#define __AI_BIGMUSCLE_H

#include <memory/FLMemPooler.h>

class CAIBigMuscle : public CAIInterface
{
	BOOL	m_bEventFlag[5];
	D3DXVECTOR3 m_vPosBegin;		// ���� ������ ��ǥ.
	int		m_nEvent;				// ���� ���� ����.
	int		m_nAttackType;			// ���� ���.
	DWORD	m_tmReattack;			// ����� Ÿ�̸�.
	DWORD	m_tmAddReattack;		// ����� Ÿ�̸��� ����������.(�� ���������� �� ���� ��� ��쵵 ����)
	DWORD	m_tmTrace;				// �����ð�.
	DWORD	m_tmTimeOver;			// ����ġ ���� ��Ȳ���� ���̻� ������ ���ϴ��� �ϴ°�츦 ����
	OBJID	m_idTarget;				// ���� ���
	D3DXVECTOR3	m_vTarget;			// ���� ��ġ.
	int		m_nAppearCnt;			// ���� ī����.
	
	OBJID	m_idLastAttacker;		// �� ���������� ������ ����.

	BOOL	m_bDefenseMode;			// ����¼� ���.

	BOOL	MoveProcessIdle();
	BOOL	MoveProcessRage();
	BOOL	MoveProcessRunaway();
	
	BOOL	StopProcessIdle();

	BOOL	SelectTarget( void );		// ������ Ÿ�̹��� ������ Ÿ���� ������.
		
	void Init( void );
	void Destroy( void );

public:
	
	CAIBigMuscle();
	CAIBigMuscle( CObj* pObj );
	virtual ~CAIBigMuscle();
	
	virtual void			InitAI();
	virtual BOOL			IsIdleMode();

	int GetEvent( void ) { return m_nEvent; }
	BOOL	StateInit( const AIMSG & msg );
	BOOL	StateIdle( const AIMSG & msg );
	BOOL	StateWander( const AIMSG & msg );
	BOOL	StateRunaway( const AIMSG & msg );
	BOOL	StateEvade( const AIMSG & msg );
	BOOL	StateRage( const AIMSG & msg );

public:
	MEMPOOLER_DECLARE( CAIBigMuscle );
	
	DECLARE_AISTATE( CAIBigMuscle )
};

#endif

