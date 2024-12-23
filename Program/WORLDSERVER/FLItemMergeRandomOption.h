
#ifndef __FLITEMMERGERANDOMOPTION_H__
#define __FLITEMMERGERANDOMOPTION_H__

#include "FLItemMergeBase.h"


class FLItemMergeRandomOption : public FLItemMergeBase
{
	public:
		FLItemMergeRandomOption();
		virtual ~FLItemMergeRandomOption();


		bool	LoadScript();

		virtual void	OnItemMerge( FLWSUser* pUser, PT_PACKET_ITEM_MERGE ptItemMerge );


	private:
		void	Clear();
		int		IsItemMergeAbleState( FLWSUser* pUser, const PT_PACKET_ITEM_MERGE ptItemMerge );
		DWORD	GetItemMergeProb( FLWSUser* pUser, const PT_PACKET_ITEM_MERGE ptItemMerge );
		int		DoItemMerge( FLWSUser* pUser, PT_PACKET_ITEM_MERGE ptItemMerge );


//////////////////////////////////////////////////////////////////////////


	public:
		enum ITEM_MERGE_ERROR
		{
			ERR_GENERAL_ERROR = 0
			, ERR_SUCCESS
			, ERR_FAIL_DESTROY
			, ERR_FAIL_PROTECT
			, ERR_VALID_STATE				// 
			, ERR_USER_INVALID				// �̻��� ����
			, ERR_ITEM_INVALID				// �̻��� ������
			, ERR_DATA_INVALID				// ��Ŷ������ �̻�
			, ERR_DONOT_VENDOR				// �ŷ����� ����
			, ERR_DONOT_EQUIP				// �������� ������
			, ERR_DONOT_PROTECTION			// ���밡���� ��ȣ�������� �ƴ�
			, ERR_DONOT_PROB				// ���밡���� Ȯ�������������� �ƴ�
			, ERR_ITEM_MISMATCH
			, ERR_ITEM_DUPLICATION
			, ERR_NOT_RANDOMOPTION
			, ERR_OVER_RANDOMOPTION
			, ERR_NOT_EXIST_MERGE_DATA
			, ERR_SIZE_MISMATCH
		};

	private:
		ListItemMergeData		m_listItemMergeData;
};


#endif // __FLITEMMERGERANDOMOPTION_H__

