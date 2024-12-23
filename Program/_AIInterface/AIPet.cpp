// AIPet.cpp : 
//

#include "stdafx.h"
#include "AIPet.h"
#include "lang.h"
#include "../WorldServer/User.h"
#include "../worldserver/FLMapGrid.h"
#include "../worldserver/LinkMap.h"

#include "../_Common/FLSkillSystem.h"


enum
{
	STATE_INIT = 1,
	STATE_IDLE,
	STATE_RAGE
};
BEGIN_AISTATE_MAP( CAIPet, CAIInterface )

	ON_STATE( STATE_INIT   , StateInit   )
	ON_STATE( STATE_IDLE   , StateIdle   )
	ON_STATE( STATE_RAGE   , StateRage   )

END_AISTATE_MAP()

#define		PETSTATE_IDLE		0
#define		PETSTATE_TRACE		1

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MEMPOOLER_IMPLEMENT( CAIPet, 128 );

CAIPet::CAIPet()
{
	Init();
}
CAIPet::CAIPet( CObj* pObj ) : CAIInterface( pObj )
{
	Init();
}

CAIPet::~CAIPet()
{
	Destroy();
}

void CAIPet::Init( void )
{
	m_idOwner	= NULL_ID;
	m_bLootMove		= FALSE;				
	m_idLootItem	= NULL_ID;				
	m_nState	= PETSTATE_IDLE;
	m_dwSkillId		= NULL_ID;	// 
	m_idPetItem		= NULL_ID;	// ���� ������
}

void CAIPet::Destroy( void )
{
	// ����� �ı��ڵ带 ������.


	Init();
}

void CAIPet::InitAI()
{
	PostAIMsg( AIMSG_SETSTATE, STATE_IDLE ); 
}

BOOL CAIPet::IsIdleMode()
{
	FUNCAISTATE pStateFunc	= GetAIStateFunc( STATE_IDLE );
	if( pStateFunc != NULL && pStateFunc == m_pStateFunc )
		return TRUE;

	return FALSE;
}

// vDst������ �̵�.
void CAIPet::MoveToDst(	D3DXVECTOR3 vDst )
{
	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();
	pMover->SetDestPos( vDst );
	g_xWSUserManager->AddSetDestPos( pMover, vDst, 1 );
	
}

// idTarget������ �̵�.
void CAIPet::MoveToDst(	OBJID idTarget )
{
	CMover* pMover = GetMover();
	if( pMover->GetDestId() == idTarget )
		return;
	pMover->SetDestObj( idTarget, 0.0f, TRUE ); // ��ǥ�� �缳�����ش�.
	//g_xWSUserManager->AddMoverSetDestObj( (CMover*)pMover, idTarget );
}


// ��ó�� �������� ������ ����.
BOOL CAIPet::SubItemLoot( void )
{
	CMover* pMover = GetMover();
	CMover* pOwner = prj.GetMover( m_idOwner );
	CWorld* pWorld = GetWorld();
//	MoverProp *pProp = pMover->GetProp();
	D3DXVECTOR3 vPos = pMover->GetPos();
	CObj *pObj = NULL;
	int nRange = DEFAULT_TILE_SIZE;
	D3DXVECTOR3 vDist;
	FLOAT fDistSq, fMinDist = 9999999.0f;
	CObj *pMinObj = NULL;

	vDist = pOwner->GetPos() - pMover->GetPos();
	fDistSq = D3DXVec3LengthSq( &vDist );
	if( fDistSq > 32.0f * 32.0f )	// ���δ԰��� �Ÿ��� 32���Ͱ� ������ ������ �����´�.
		return FALSE;

	if( pOwner && pOwner->IsFly() )
		return FALSE;
		
	// ��ó�� �������� �˻���. - ���δԲ��� �˻��ؾ��ҵ�...
	FOR_LINKMAP( pWorld, vPos, pObj, nRange, CObj::linkDynamic, pMover->GetLayer() )
	{
		if( pObj->GetType() == OT_ITEM )	// ���۸� �˻�
		{
			CItem *pItem = (CItem *)pObj;
			PT_ITEM_SPEC pItemProp	= pItem->GetProp();
			// �̰� ���� ���������� StateIdle ARRIVAL���� DoLoot()�ϰ� �����Ŀ� �ٽ� SubItemLoot()�� ȣ��������
			// Loot�� �������� ���� ���������� ���⼭ �� �˻��� �Ǵ����.. �׷��� �ߺ��Ǵ� �������� �˻� �ȵǰ� ���ĺô�.
//			if( pItem->GetId() != m_idLootItem )		
			if( pItem->IsDelete() == FALSE )
			{
				if( pItemProp )
				{
					if( pOwner->IsLoot( pItem, TRUE ) )	// ���õǴ¾��������� �˻���.
					{
#ifdef PAT_LOOTOPTION
						if ( LootType ( pOwner->m_nPetLootType, pItemProp->dwItemKind2, pItemProp->dwItemKind3 ) )
						{
#endif // PAT_LOOTOPTION
							vDist = pObj->GetPos() - pMover->GetPos();
							fDistSq = D3DXVec3LengthSq( &vDist );		// �Ÿ� ����.
							if( fDistSq < 15 * 15 && fDistSq < fMinDist )	// 10���� �̳���... ���� �Ÿ��� ����� ������ ã��.
								pMinObj = pObj;
#ifdef PAT_LOOTOPTION
						}
#endif // PAT_LOOTOPTION
					}
				}
			}
		}
	}
	END_LINKMAP

	if( pMinObj )
	{
		MoveToDst( pMinObj->GetPos() );		// ��ǥ������ �̵�.
		m_idLootItem = ((CItem *)pMinObj)->GetId();
		m_bLootMove = TRUE;
		
	}
		
	return m_bLootMove;
}
#ifdef PAT_LOOTOPTION
BOOL CAIPet::LootType( int nType, DWORD dwItemKind2, DWORD dwItemKind3 )
{
	bool bType[13] = { 0, };
	bType[0] = (dwItemKind2 == IK2_WEAPON_HAND || dwItemKind2 == IK2_WEAPON_DIRECT || dwItemKind2 == IK2_WEAPON_MAGIC);
	bType[1] = (dwItemKind2 == IK2_ARMOR || dwItemKind2 == IK2_ARMORETC);
	bType[2] = (dwItemKind2 == IK2_JEWELRY );
	bType[3] = (dwItemKind3 == IK3_SMELT_ULTIMATE_MATERIAL || dwItemKind3 == IK3_SMELT_GENERAL_MATERIAL || dwItemKind3 == IK3_SMELT_ACCESSORY_MATERIAL || dwItemKind3 == IK3_ULTIMATE );
	bType[5] = (dwItemKind3 == IK3_ELECARD);
	bType[6] = (dwItemKind3 == IK3_SOCKETCARD || dwItemKind3 == IK3_SOCKETCARD2
#ifdef BARUNA_UPGRADE_SUIT_PIERCING
 || dwItemKind3 == IK3_SOCKETCARD3
#endif
	);
	bType[7] = (dwItemKind3 == IK3_QUEST);
	bType[8] = (dwItemKind2 == IK2_FOOD);
	bType[9] = (dwItemKind2 == IK2_POTION);
	bType[10] = (dwItemKind2 == IK2_BUFF);
	bType[11] = (dwItemKind3 == IK3_PET);
	bType[12] = (dwItemKind2 == IK2_GEM);
	//if ( !nType )
	//	return TRUE;
	if ( (nType & 0x1) && bType[0] )
		return TRUE;
	if ( (nType & 0x2) && bType[1] )
		return TRUE;
	if ( (nType & 0x4) && bType[2] )
		return TRUE;
	if ( (nType & 0x8) && bType[3] )
		return TRUE;
	if ( (nType & 0x10) && bType[4] )
		return TRUE;
	if ( (nType & 0x20) && bType[5] )
		return TRUE;
	if ( (nType & 0x40) && bType[6] )
		return TRUE;
	if ( (nType & 0x80) && bType[7] )
		return TRUE;
	if ( (nType & 0x100) && bType[8] )
		return TRUE;
	if ( (nType & 0x200) && bType[9] )
		return TRUE;
	if ( (nType & 0x400) && bType[10] )
		return TRUE;
	if ( (nType & 0x800) && bType[11] )
		return TRUE;
	if ( (nType & 0x1000) && bType[12] )
		return TRUE;
	int nCount = 0;
	for ( int i = 0; i < 13; i++ ) nCount += bType[i];
	if ( (nType & 0x2000) && !nCount )
		return TRUE;
	return FALSE;
}
#endif // PAT_LOOTOPTION
BOOL CAIPet::StateInit( const AIMSG & /*msg*/ )
{
	return TRUE;
}

BOOL CAIPet::MoveProcessIdle( const AIMSG & /*msg*/ )
{
	CMover *pMover = GetMover();
	CMover *pOwner = prj.GetMover( m_idOwner ); 
//	CWorld *pWorld = GetWorld();
//	MoverProp *pProp = pMover->GetProp();


	// ������ ���°ų� �׾��� ��� �̵� ó�� ���� ���� 
	if( pMover->IsDie() || (pMover->m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL) )
		return FALSE;

	if( NotOwnedPetInactivated() )
		return FALSE;
	
	if( m_bLootMove == FALSE )	// �����Ϸ� ���� �Ʒ� ó���� �ϸ� �ȵǱ���...
	{
		// ���δ԰��� �Ÿ��� �־����� ���δ������� �޷�����.
		if( m_nState == PETSTATE_IDLE )
		{
			D3DXVECTOR3 vDist = pOwner->GetPos() - pMover->GetPos();
			FLOAT fDistSq = D3DXVec3LengthSq( &vDist );
			if( fDistSq > 1.0f * 1.0f )
			{
				MoveToDst( m_idOwner );
				m_nState = PETSTATE_TRACE;
			}
		} else
		if( m_nState == PETSTATE_TRACE )
		{
			if( pOwner->IsRangeObj( pMover, 0 ) == TRUE )		// ���δ� ������ �ٰ�����.
			{
				m_nState = PETSTATE_IDLE;			// �����·� ��ȯ
				pMover->SendActMsg( OBJMSG_STOP );	// ��� ����
				pMover->ClearDest();				// �̵���ǥ Ŭ����.
			}
			if( pMover->IsEmptyDest() )			// �˼����� ������ ���� �̵����� �ʰ� ���ڸ� ������ .
			{
				m_bLootMove = FALSE;			// ������ ������ ���.
				m_idLootItem = NULL_ID;
				m_nState = PETSTATE_IDLE;
			}
			
		}
	}

	// ������ ��ĵ/����
	{
		{
			if( (pMover->GetCount() & 15) == 0 )		// ������ �ֺ��� ��ĵ�ؼ�... �����ϱ� 15�� 1��
			{	
				if( m_bLootMove == FALSE )		// �����Ϸ� ���� ������ �� üũ�ϸ� �Ȋ�
					if( SubItemLoot() )		// �������� ������.
						m_nState = PETSTATE_IDLE;	// ���ý��������� �����·� �ٲ����.
			}
		}

		if( m_bLootMove == TRUE )
		{
			CCtrl *pCtrl = prj.GetCtrl( m_idLootItem );		// �׾������� ��������� �����ϱ� �˻���.
			if( IsInvalidObj(pCtrl) )		// ������ ������ �̵��߿� ������ ��������
			{
				MoveToDst( pMover->GetPos() );	// ���ڸ��� ��.
				m_bLootMove = FALSE;
				m_idLootItem = NULL_ID;
				m_nState = PETSTATE_IDLE;
			} else
			{
				if( pMover->IsEmptyDest() )			// �˼����� ������ ���� �̵����� �ʰ� ���ڸ� ������ .
				{
					m_bLootMove = FALSE;			// ������ ������ ���.
					m_idLootItem = NULL_ID;
					m_nState = PETSTATE_IDLE;
				}
			}
		}
	}

	return TRUE;
}

BOOL CAIPet::StateIdle( const AIMSG & msg )
{
	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();

	BeginAIHandler( )

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT ) 
//		SetStop( SEC( 0 ) );
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT_TARGETCLEAR )
//		m_dwIdTarget = NULL_ID;
			
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_PROCESS ) 
		MoveProcessIdle( msg );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DAMAGE ) 

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DIE ) 
		SendAIMsg( AIMSG_EXIT );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_COLLISION )

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_ARRIVAL )
		{
			if( m_bLootMove )	// ������ ���ø�忴��.
			{
				CMover *pOwner = prj.GetMover( m_idOwner );
				if( IsValidObj(pOwner) )
				{
					BOOL bSuccess = FALSE;
					CCtrl *pCtrl = prj.GetCtrl( m_idLootItem );		// �׾������� ��������� �����ϱ� �˻���.
					if( IsValidObj(pCtrl) )
					{
						CItem *pItem = (CItem *)pCtrl;
						D3DXVECTOR3 vDist = pCtrl->GetPos() - pMover->GetPos();
						FLOAT fDistSq = D3DXVec3LengthSq( &vDist );
						if( fDistSq < 5.0f * 5.0f )		// �����ؼ� �Ÿ� �˻� �ѹ��� �ؼ� 
						{
							if( pItem->IsDelete() )
								return TRUE;

							// ������ �������� �ݴ°Ͱ� ���� ȿ���� ��.
							bSuccess = pOwner->DoLoot( pItem );
						}
					}
					if( bSuccess )
					{
						if( SubItemLoot() == FALSE )		// ���������� ������ �ѹ��� ��ĵ�ؼ� ������ ������ �ٽð��� ������ ���ο��� ���ư���.
						{
							m_bLootMove = FALSE;		// this�� ������ �������̶�°� ����.
							m_idLootItem = NULL_ID;
						}
					} else
					{
						m_bLootMove = FALSE;		// this�� ������ �������̶�°� ����.
						m_idLootItem = NULL_ID;
					}
				}
			}
		}
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_EXIT )	

	EndAIHandler( )

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////



BOOL CAIPet::MoveProcessRage( const AIMSG & /*msg*/ )
{
	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();
//	MoverProp *pProp = pMover->GetProp();
	
	// ������ ���°ų� �׾��� ��� �̵� ó�� ���� ���� 
	if( pMover->IsDie() || (pMover->m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL) )
		return FALSE;

	return TRUE;
}

BOOL CAIPet::StateRage( const AIMSG & msg )
{
	CMover* pMover = GetMover();
//	CWorld* pWorld = GetWorld();
	if( IsInvalidObj(pMover) )	return FALSE;

	BeginAIHandler( )

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT ) 

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT_TARGETCLEAR )		// Ÿ���� Ŭ�����ϰ� ������ ���ư�.
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_PROCESS ) 
		MoveProcessRage( msg );
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DAMAGE ) 

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_COLLISION )
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DIE ) 
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DSTDIE ) 
		SendAIMsg( AIMSG_SETSTATE, STATE_IDLE );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_BEGINMOVE )

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_ARRIVAL )

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_EXIT )	
//		SendAIMsg( AIMSG_SETPROCESS, FALSE );

	EndAIHandler( )
	
	return TRUE;
}

BOOL CAIPet::NotOwnedPetInactivated( void )
{
	CMover *pEatPet		= GetMover();
	CMover *pOwner	= prj.GetMover( m_idOwner ); 

	if( IsInvalidObj( pOwner ) )
	{
		pEatPet->Delete();
		return TRUE;
	}

	if( pOwner->IsDie() )
	{
		pOwner->InactivateEatPet();
		return TRUE;
	}
/*	else if( !pOwner->IsValidArea( pEatPet, 32 ) )
	{
		FLItemBase* pItemBase = pOwner->GetItemId( m_idPetItem );
		if( !IsUsableItem( pItemBase ) )
		{
			pOwner->InactivateEatPet();
			return TRUE;
		}
		FLItemElem* pItemElem = (FLItemElem*)pItemBase;
		pOwner->InactivateEatPet();
		pOwner->DoUseItem( MAKELONG( ITYPE_ITEM, pItemElem->m_dwObjId ), pItemElem->m_dwObjId );
		return FALSE;
	}*/

	return FALSE;
}

void CAIPet::SetItem( CMover* pPlayer, FLItemElem* pItem )
{	// ����ڿ��� ���� ���� ȿ�� ����
	m_idPetItem		= pItem->m_dwObjId;
	pPlayer->SetDestParamRandomOptExtension( pItem );
	SetSkill( pPlayer, pItem->GetProp() );		// ���� ��� �� ����
																		// ������ ���� �۾� �κ��� �����丵�ϸ鼭 �̰����� �̵� ��Ŵ
}

void CAIPet::SetSkill( CMover* pPlayer, PT_ITEM_SPEC Prop )
{	// ���� ��� �� ����
	if( Prop->dwActiveSkill != NULL_ID )
	{
		SetSkillId( Prop->dwActiveSkill );
		g_cSkillSystem->DoActiveSkill( pPlayer, Prop->dwActiveSkill, MIN_SKILL_USE_LEVEL, pPlayer );
	}
}

void CAIPet::ResetItem( void )
{	// �������κ��� ���� ���� ȿ�� ����
	CMover* pPlayer		= prj.GetMover( m_idOwner );
	if( IsValidObj( pPlayer ) )
	{
		FLItemElem* pItem	= static_cast<FLItemElem*>( pPlayer->GetItemId( m_idPetItem ) );
		if( pItem && pItem->IsEatPet() )
		{
			pPlayer->ResetDestParamRandomOptExtension( pItem );
			ResetSkill( pPlayer );	// ��� �� ����
		}
	}
	m_idPetItem		= NULL_ID;
}

void CAIPet::ResetSkill( CMover* pPlayer )
{	// ��� �� ����
	if( GetSkillId() != NULL_ID && pPlayer->HasBuff( BUFF_SKILL, (WORD)( GetSkillId() ) ) )
		pPlayer->RemoveBuff( BUFF_SKILL, (WORD)( GetSkillId() ) );
}
