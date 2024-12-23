#pragma once


#if defined __WORLDSERVER && defined __ENCHANT_BARUNA16

class FLNPCSummoner
{
public:
	struct E_OPERATION {	enum E {
		NO_OPERATION,
		INACTIVE,
		ACTIVE,
	};						};

public:
	FLNPCSummoner();
	virtual ~FLNPCSummoner();

	//�ѹ� �����ϸ� ��ȯ �ٽý����ϸ� ��ȯ ���� 
	E_OPERATION::E			ExcuteCallNPCByItem( FLWSUser & Owner, FLItemElem & ItemElem/*, const DWORD dwMoverIndex*//*, const TCHAR * szCharacterName*/ );
	OBJID					GetNPCOBJID() const;
	DWORD					GetNPCItemOBJID() const;

	E_OPERATION::E			TryInactivateNPCByRange( FLWSUser & Owner, const float fRange );				// ( return TRUE�� ��ȯ : FALSE�� ���� )

	E_OPERATION::E			InactivateNPC( FLWSUser & Owner );

protected:
	E_OPERATION::E			activateNPC( FLWSUser & Owner, FLItemElem & ItemElem/*, const DWORD dwMoverIndex*//*, const TCHAR * szCharacterName*/ );
	
private:
	DWORD					m_dwNPCItemOBJID;					//@ ���� ���ʸ��ϰ� ItemSpec�� ���� ���� && ���� ���� ����   �ɼ��� �ִ´�.
	OBJID					m_CalledNPCOBJID;
};


//#include <boost/bimap.hpp>

// ItemObjID, 
// NPCObjID -> ItemObjID
// ItemID,	( ������ ���� Ƚ�� & ��Ÿ�� ������ )

//class FLCalledNPCMng
//{
//public:
//	struct E_OPERATION {	enum E {
//		NO_OPERATION,
//		INACTIVE,
//		ACTIVE,
//	};						};
//
//	enum					{	ITEM_CALL_NPC_MAX	= 3 };
//
//public:
//	FLCalledNPCMng();
//	virtual ~FLCalledNPCMng();
//	
//	BOOL					OnUseSkill( FLWSUser & Owner );
//	BOOL					OnUseItem( FLWSUser & Owner, FLItemElem & ItemElem );
//
//	void					TryInactivateByRangeInfo( FLWSUser & Owner );
//	BOOL					InactivateNPC( FLWSUser & Owner, FLItemElem & ItemElem );
//
//private:
//	BOOL					activateNPC( FLWSUser & Owner, FLItemElem & ItemElem );
//
//private:
//	//typedef					boost::bimap< DWORD/*ItemID */, OBJID /*NPC ObjID*/ >	ItemID_NPCBimap;
//	//typedef					boost::bimap< DWORD/*ItemObjID */, DWORD /*ItemID*/ >	ItemObjID_ItemIDBimap;
//	//typedef					map< OBJID/*NPC ObjID*/, u_long/*Range*/ >				NPC_RangeMap;
//
//	//ItemID_NPCBimap			bimapCalledNPC;
//	//ItemObjID_ItemIDBimap	bimapUsedItem;
//	//NPC_RangeMap			mapRangeInfo;
//};
//
//
//



#endif //__WORLDSERVER && defined __ENCHANT_BARUNA16