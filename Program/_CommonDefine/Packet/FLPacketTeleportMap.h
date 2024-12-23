
#ifndef	__FLPACKETTELEPORTMAP_H__
#define	__FLPACKETTELEPORTMAP_H__


#include "FLPacket.h"
#include "../FLTypes.h"
#include "../Spec/FLSpecStruct.h"


//////////////////////////////////////////////////////////////////////////
// �ڷ���Ʈ ���� ����
enum
{
	MAX_TELEPORT_MAP_USER_POINT			= 5,	// ���� ��� ���� �ִ��
	MAX_TELEPORT_MAP_USER_POINT_NAME	= 17,	// �� ����
};


// �ڷ���Ʈ ��û
class	FLPacketTeleportMapToPointReq : public FLPacket
{
public:

	FLPacketTeleportMapToPointReq();

	DWORD	dwItemObjID;	// �ڷ���Ʈ ���� ������

	BYTE	byType;			// �ڷ���Ʈ ���̵� Ÿ��
	DWORD	dwTeleportID;	// �ڷ���Ʈ ���̵�

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};


// �ڷ���Ʈ ���� ������ ���� ��ġ ��� ��û
class	FLPacketTeleportMapAddMyPointReq : public FLPacket
{
public:

	FLPacketTeleportMapAddMyPointReq();

	DWORD	dwItemObjID;	// �ڷ���Ʈ ���� ������

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};


// �ڷ���Ʈ ���� ������ ��� ��ġ �̸� ���� ��û
class	FLPacketTeleportMapChgPointNameReq : public FLPacket
{
public:

	FLPacketTeleportMapChgPointNameReq();

	DWORD		dwItemObjID;									// �ڷ���Ʈ ���� ������

	DWORD		dwUserPointID;									// ���� ��� ���� �ε���
	TCHAR		szPointName[MAX_TELEPORT_MAP_USER_POINT_NAME];	// ���� ��� ���� �̸�

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};


// �ڷ���Ʈ ���� ������ ��� ��ġ ���� ��û
class	FLPacketTeleportMapDelPointReq : public FLPacket
{
public:

	FLPacketTeleportMapDelPointReq();

	DWORD	dwItemObjID;	// �ڷ���Ʈ ���� ������
	DWORD	dwUserPointID;	// ���� ��� ���� �ε���

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};


// �ڷ���Ʈ ���� ������ ��� ��ġ ��� ���� ��û
class	FLPacketTeleportMapDelAllPointReq : public FLPacket
{
public:

	FLPacketTeleportMapDelAllPointReq();

	DWORD	dwItemObjID;	// �ڷ���Ʈ ���� ������

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};


//////////////////////////////////////////////////////////////////////////
// client
//////////////////////////////////////////////////////////////////////////

// �ڷ���Ʈ ���� ����
class	FLSnapshotTeleportMapOpenNoti : public FLPacket
{
public:

	FLSnapshotTeleportMapOpenNoti();

	DWORD								dwItemObjID;	// ������
	BYTE								byType;			// �ڷ���Ʈ Ÿ��

	std::vector< T_TELEPORTMAP_POINT >	vecUserPoints;	// ���� ��� ���� ������

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// �ڷ���Ʈ ��û ����
class	FLSnapshotTeleportMapToPointAck : public FLPacket
{
public:

	FLSnapshotTeleportMapToPointAck();

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// �ڷ���Ʈ ���� ������ ���� ��ġ ��� ����
class	FLSnapshotTeleportMapAddMyPointAck : public FLPacket
{
public:

	FLSnapshotTeleportMapAddMyPointAck();

	T_TELEPORTMAP_POINT	tUserPoint;	// ��ϵ� ����

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// �ڷ���Ʈ ���� ������ ��� ��ġ �̸� ���� ����
class	FLSnapshotTeleportMapChgPointNameAck : public FLPacket
{
public:

	FLSnapshotTeleportMapChgPointNameAck();

	T_TELEPORTMAP_POINT	tUserPoint;	// ����� ����

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};


// �ڷ���Ʈ ���� ������ ��� ��ġ ���� ����
class	FLSnapshotTeleportMapDelPointAck : public FLPacket
{
public:

	FLSnapshotTeleportMapDelPointAck();

	T_TELEPORTMAP_POINT	tUserPoint;	// ������ ����

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};


// �ڷ���Ʈ ���� ������ ��� ��ġ ��� ���� ����
class	FLSnapShotTeleportMapDelAllPointAck : public FLPacket
{
public:

	FLSnapShotTeleportMapDelAllPointAck();

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};



//////////////////////////////////////////////////////////////////////////
// ������ ���
//////////////////////////////////////////////////////////////////////////

// �ڷ���Ʈ ���� ���� ��� ����Ʈ(DatabaseServer->WorldServer)
class	FLPacketTeleportMapPointListNoti : public FLPacket
{
public:

	FLPacketTeleportMapPointListNoti();

	OBJID									dwPlayerID;		// ���� ���̵�
	// ���� ��� ���� ������
	std::vector< T_TELEPORTMAP_POINT >	vecUserPoints;

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};

// �ڷ���Ʈ ���� ���� ��� �뺸(WorldServer->DatabaseServer)
class	FLPacketTeleportMapAddPointNoti : public FLPacket
{
public:

	FLPacketTeleportMapAddPointNoti();

	OBJID				dwPlayerID;	// ���� ���̵�
	T_TELEPORTMAP_POINT	tUserPoint;	// ������ ����

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};


// �ڷ���Ʈ ���� ���� ��� ��ġ �̸� ���� �뺸(WorldServer->DatabaseServer)
class	FLPacketTeleportMapChgPointNameNoti : public FLPacket
{
public:

	FLPacketTeleportMapChgPointNameNoti();

	OBJID				dwPlayerID;	// ���� ���̵�
	T_TELEPORTMAP_POINT	tUserPoint;	// ������ ����

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};


// �ڷ���Ʈ ���� ���� ��� ��ġ ���� �뺸(WorldServer->DatabaseServer)
class	FLPacketTeleportMapDelPointNoti : public FLPacket
{
public:

	FLPacketTeleportMapDelPointNoti();

	OBJID				dwPlayerID;	// ���� ���̵�
	T_TELEPORTMAP_POINT	tUserPoint;		// ������ ����

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};





#endif