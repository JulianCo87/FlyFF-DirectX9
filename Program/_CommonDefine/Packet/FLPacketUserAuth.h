
#ifndef	__FLPACKETUSERAUTH_H__
#define	__FLPACKETUSERAUTH_H__


#include "FLPacket.h"


// SNAPSHOTTYPE_SET_AUTHORIZATION_NOTI			// ĳ���� ���� ����
class	FLSnapshotSetAuthorizationNoti : public FLPacket
{
public:

	FLSnapshotSetAuthorizationNoti();

	DWORD	dwAuthorization;

private:

	bool	BodySerialize( CAr& kAr )	const;
	bool	BodyDeserialize( CAr& kAr );

};



#endif
