
#ifndef __FLPACKETITEMEXCHANGE_H__
#define __FLPACKETITEMEXCHANGE_H__


#include "FLPacket.h"
#include "../_Common/FLItemElem.h"


//////////////////////////////////////////////////////////////////////////
// client -> world

class	FLPacketItemExchangeReq : public FLPacket
{
public:

	FLPacketItemExchangeReq();

	DWORD	dwMenuID;								// �޴� ���̵�
	DWORD	dwListNth;								// ��ȯ ����Ʈ ����
	int		nExchangeCount;							// �� ��ȯ ��
	bool	bMax;									// �Ǵµ��� ��ȯ �� ���ΰ�


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// client -> world
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// world -> client

class	FLSnapshotItemExchangeAck : public FLPacket
{
public:

	FLSnapshotItemExchangeAck();

	DWORD	dwMenuID;								// �޴� ���̵�
	DWORD	dwListNth;								// ��ȯ ����Ʈ ����
	bool	bSuccess;


private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// world -> client
//////////////////////////////////////////////////////////////////////////

#endif// __FLPACKETITEMEXCHANGE_H__
