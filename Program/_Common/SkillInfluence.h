#ifndef __SKILLINFLUENCE_H__
#define	__SKILLINFLUENCE_H__

#define	BUFF_ITEM	0
#define	BUFF_SKILL	1
#define	BUFF_PET	2
#define	BUFF_ITEM2	3	// 110217 ���̻� ������� ����. item spec �� bAbsoluteTime ���.
#define	BUFF_EQUIP	4	// ������ �����ۿ� Ư�� DST(DST_GIFTBOX)�� ���� ���� �����ܸ� ���
#define	BUFF_NULL_ID	(WORD)0xFFFF
#define	MAX_SKILLINFLUENCE	64

#ifdef SKILL_BUFF21
#define	MAX_SKILLBUFF_COUNT	21
#else
#define	MAX_SKILLBUFF_COUNT	14
#endif	// SKILL_BUFF21


#endif	// __SKILLINFLUENCE_H__