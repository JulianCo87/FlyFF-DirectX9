
#ifndef __FLPACKETITEMOPTION_H__
#define __FLPACKETITEMOPTION_H__


#include "FLPacket.h"
#include "../_Common/FLItemElem.h"


//////////////////////////////////////////////////////////////////////////
// client -> world

class	FLPacketItemOptionRandomOptionSyncReq : public FLPacket
{
public:

	FLPacketItemOptionRandomOptionSyncReq();

	DWORD	dwItemObjID;					// �ɼ� ����ȭ �� ������


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// client -> world
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// world -> client

class	FLSnapshotItemUpgradeRandomOptionSyncAck : public FLPacket
{
public:

	FLSnapshotItemUpgradeRandomOptionSyncAck();

	DWORD	dwItemObjID;					// �ɼ� ����ȭ �� ������
	bool	bSuccess;						// ����ȭ ����


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

class	FLSnapshotItemUpgradeRandomOptionAck : public FLPacket
{
public:

	FLSnapshotItemUpgradeRandomOptionAck();

	DWORD	dwItemObjID;
	DWORD	dwSerialNumber;

	FLRandomOption		kRandomOption;


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// world -> client
//////////////////////////////////////////////////////////////////////////

#endif// __FLPACKETITEMOPTION_H__
