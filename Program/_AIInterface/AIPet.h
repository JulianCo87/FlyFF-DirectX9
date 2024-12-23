#ifndef __AI_PET_H
#define __AI_PET_H

#include "..\_AIInterface\AIInterface.h"
#include <memory/FLMemPooler.h>
#include <Mover.h>

class CAIPet : public CAIInterface
{
private:
	OBJID m_idOwner;		// ���δ�
	BOOL	m_bLootMove;				// �����Ϸ� ������.
	DWORD	m_idLootItem;				// ������ ������.
	int		m_nState;					// ���� ����.
	
	void MoveToDst(	OBJID idTarget );
	void MoveToDst(	D3DXVECTOR3 vDst );

	BOOL  MoveProcessIdle( const AIMSG & msg );
	BOOL  MoveProcessRage( const AIMSG & msg );

	void Init( void );
	void Destroy( void );
	
public:
	CAIPet();
	CAIPet( CObj* pObj );
	virtual ~CAIPet();

	virtual void InitAI();
	virtual BOOL			IsIdleMode();

	void	SetOwner( OBJID idOwner ) { m_idOwner = idOwner; }
	BOOL	GetLootMove( void ) { return m_bLootMove; }
	DWORD	GetLootItem( void ) { return m_idLootItem; }
	int		GetState( void ) { return m_nState; }
	BOOL	StateInit( const AIMSG & msg );
	BOOL	StateIdle( const AIMSG & msg );
	BOOL	StateRage( const AIMSG & msg );
#ifdef PAT_LOOTOPTION
	BOOL	LootType( int nType, DWORD dwItemKind2, DWORD dwItemKind3 );
#endif // PAT_LOOTOPTION
	BOOL SubItemLoot( void );

	// ���ΰ� ���� ������ ����, ���� �� ���� ȿ�� ����
	void	SetItem( CMover* pPlayer, FLItemElem* pItem );
	// �������κ��� ���� ���� ȿ�� ����
	void	ResetItem( void );
	// �ش� ���� ������ �ĺ��� ��ȯ
	OBJID	GetPetItemId( void )	{	return m_idPetItem;		}
private:
	void	SetSkillId( DWORD dwSkill )		{ m_dwSkillId = dwSkill; }	// ��� �� ����
	DWORD	GetSkillId()	{	return m_dwSkillId;	}	// ��� �� ����
	void	SetSkill( CMover* pPlayer, PT_ITEM_SPEC pProp );	// ��� �� ����
	void	ResetSkill( CMover* pPlayer );	// ��� �� ����
	DWORD	m_dwSkillId;	// ��� �� ����
	OBJID	m_idPetItem;	// ���� ������ �ĺ���

private:
	BOOL	NotOwnedPetInactivated();

public:
	MEMPOOLER_DECLARE( CAIPet );
	
	DECLARE_AISTATE( CAIPet )
};

#endif

