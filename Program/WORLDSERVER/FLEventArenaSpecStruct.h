
#ifndef	__FLEVENTARENASPECSTRUCT_H__
#define	__FLEVENTARENASPECSTRUCT_H__


#include <defineEventArena.h>
#include "../_CommonDefine/Spec/FLSpecStruct.h"
#include "../_CommonDefine/Packet/FLPacketEventArena.h"


struct	T_EVENT_ARENA_TEAM_MEMBER_SPEC
{
	BYTE	byPartyNumber;			// �ش� ��ȣ
	TCHAR	szCharName[MAX_NAME];	// ĳ���� �̸�
};

struct	T_EVENT_ARENA_TONEMENT_SPEC
{
	TCHAR	szName[MAX_EVENT_ARENA_TONEMENT_NAME];	// ��ʸ�Ʈ �̸�
	int		nNeedWinCount;							// �ʿ� �¼�
	TCHAR	szDesc[MAX_EVENT_ARENA_TONEMENT_DESC];	// ����
};

struct	T_EVENT_ARENA_GIVE_ITEM_SPEC
{
	DWORD	dwItemIndex;			// ������ �ε���
	BYTE	byCount;				// ���� ����
};


struct	T_EVENT_ARENA_TIME_CONFIG_SPEC
{
	DWORD	dwBattleTime;			// ���� �ð�(���� ��)
	DWORD	dwCallWaitTime;			// ��ȯ��� �ð�(���� ��)
	DWORD	dwBattleStandbyTime;	// ������� �ð�(���� ��)
	DWORD	dwWinnerDisplayTime;	// ����ǥ�� �ð�(���� ��)
};

struct	T_EVENT_ARENA_START_POSITION_SPEC
{
	int					nStartKind;	// ���� ��ġ ����
	int					nTeam;		// �� ����

	T_WORLD_POSITION	tPosition;	// ��ġ

	DWORD				dwRadius;	// �ݰ�
};




#endif