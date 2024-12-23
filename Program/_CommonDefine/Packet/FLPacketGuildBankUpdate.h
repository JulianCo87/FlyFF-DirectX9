
#ifndef __FLPACKETGUILDBANKUPDATE_H__
#define __FLPACKETGUILDBANKUPDATE_H__


#include "FLPacket.h"


//////////////////////////////////////////////////////////////////////////
// world -> trans

class	FLPacketGuildBankInputItemReq : public FLPacket
{
	public:
		FLPacketGuildBankInputItemReq();

		DWORD							dwGuildID;				// ��� ���̵�
		DWORD							dwPlayerID;				// �÷��̾� ���̵�
		CItemContainer< FLItemElem >	GuildBank;				// ��� ��ũ
		FLItemElem						kInputItemElem;			// ������ ����


	private:
		bool	BodySerialize( CAr & kAr ) const;
		bool	BodyDeserialize( CAr & kAr );
};

class	FLPacketGuildBankOutputItemReq : public FLPacket
{
	public:
		FLPacketGuildBankOutputItemReq();

		DWORD							dwGuildID;				// ��� ���̵�
		DWORD							dwPlayerID;				// �÷��̾� ���̵�
		CItemContainer< FLItemElem >	GuildBank;				// ��� ��ũ
		FLItemElem						kOutputItemElem;		// ������ ����


	private:
		bool	BodySerialize( CAr & kAr ) const;
		bool	BodyDeserialize( CAr & kAr );
};

class	FLPacketGuildBankInputGoldReq : public FLPacket
{
	public:
		FLPacketGuildBankInputGoldReq();

		DWORD		dwGuildID;					// ��� ���̵�
		DWORD		dwPlayerID;					// �÷��̾� ���̵�
		DWORD		dwGuildGold;				// ��� ���
		DWORD		dwInputGold;				// ���
		bool		bUpdateContribution;		// �÷��̾� ���� ���� ������Ʈ ����


	private:
		bool	BodySerialize( CAr & kAr ) const;
		bool	BodyDeserialize( CAr & kAr );
};

class	FLPacketGuildBankOutputGoldReq : public FLPacket
{
	public:
		FLPacketGuildBankOutputGoldReq();

		DWORD		dwGuildID;					// ��� ���̵�
		DWORD		dwPlayerID;					// �÷��̾� ���̵�
		DWORD		dwGuildGold;				// ��� ���
		DWORD		dwOutputGold;				// ���
		bool		bUpdateContribution;		// �÷��̾� ���嵵 ���� ������Ʈ ����


	private:
		bool	BodySerialize( CAr & kAr ) const;
		bool	BodyDeserialize( CAr & kAr );
};

// client -> world
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// trans -> world

class	FLPacketGuildBankInputItemAck : public FLPacket
{
	public:
		FLPacketGuildBankInputItemAck();

		bool			bSuccess;
		DWORD			dwGuildID;
		DWORD			dwPlayerID;
		FLItemElem		kInputItemElem;


	private:
		bool	BodySerialize( CAr & kAr )	const;
		bool	BodyDeserialize( CAr & kAr );
};

class	FLPacketGuildBankOutputItemAck : public FLPacket
{
	public:
		FLPacketGuildBankOutputItemAck();

		bool			bSuccess;
		DWORD			dwGuildID;
		DWORD			dwPlayerID;
		FLItemElem		kOutputItemElem;


	private:
		bool	BodySerialize( CAr & kAr )	const;
		bool	BodyDeserialize( CAr & kAr );
};

class	FLPacketGuildBankInputGoldAck : public FLPacket
{
	public:
		FLPacketGuildBankInputGoldAck();

		bool			bSuccess;
		DWORD			dwGuildID;
		DWORD			dwPlayerID;
		DWORD			dwInputGold;


	private:
		bool	BodySerialize( CAr & kAr )	const;
		bool	BodyDeserialize( CAr & kAr );
};

class	FLPacketGuildBankOutputGoldAck : public FLPacket
{
	public:
		FLPacketGuildBankOutputGoldAck();

		bool			bSuccess;
		DWORD			dwGuildID;
		DWORD			dwPlayerID;
		DWORD			dwOutputGold;


	private:
		bool	BodySerialize( CAr & kAr )	const;
		bool	BodyDeserialize( CAr & kAr );
};

// world -> client
//////////////////////////////////////////////////////////////////////////

#endif // __FLPACKETGUILDBANKUPDATE_H__
