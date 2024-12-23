#ifndef __FLUSEACTIVEITEM_H__
#define __FLUSEACTIVEITEM_H__


class FLUseActiveItem
{
	public:
		static	FLUseActiveItem*	GetInstance();

		FLUseActiveItem();
		~FLUseActiveItem();

		int		OnDoUseActiveItem( FLWSUser* pUser, FLItemElem* pSrcItem, FLItemElem* pDestItem );
		int		OnDoUseActiveItem( FLWSUser* pUser, FLItemElem* pSrcItem );

	private:
		int		DoUseActiveItem( FLWSUser* pUser, FLItemElem* pSrcItem, FLItemElem* pDestItem );
		int		CheckActiveItem( FLWSUser* pUser, FLItemElem* pSrcItem, FLItemElem* pDestItem );
		int		UseResetBind( FLWSUser* pUser, FLItemElem* pSrcItem, FLItemElem* pDestItem );
		int		UseBarunaGeneralEnchantProtection( FLWSUser* pUser, FLItemElem* pSrcItem, FLItemElem* pDestItem );
		int		UseBarunaAttributeEnchantProtection( FLWSUser* pUser, FLItemElem* pSrcItem, FLItemElem* pDestItem );
		int		UseBarunaAttributeEnchantProbability( FLWSUser* pUser, FLItemElem* pSrcItem, FLItemElem* pDestItem );
		int		UseBarunaPiercingProtection( FLWSUser* pUser, FLItemElem* pSrcItem, FLItemElem* pDestItem );
		int		UseBarunaPiercingProb( FLWSUser* pUser, FLItemElem* pSrcItem, FLItemElem* pDestItem );

		int		DoUseActiveItem( FLWSUser* pUser, FLItemElem* pSrcItem );
		int		CheckActiveItem( FLWSUser* pUser, FLItemElem* pSrcItem );
		int		UseColosseumRetry( FLWSUser* pUser, FLItemElem* pSrcItem );
		int		UseColosseumBossRate( FLWSUser* pUser, FLItemElem* pSrcItem );


	public:
		enum USE_ACTIVEITEM_RESULT
		{
			ERR_GENERAL_ERROR = 0
			, ERR_SUCCESS_USE										// ����
			, ERR_USER_INVALID										// �̻��� ����
			, ERR_ITEM_INVALID										// �̻��� ������
			, ERR_DONOT_USE_STATE									// ���� ������ ���°� �ƴ�
			, ERR_DONOTUSE_ANOTHER									// ��� ���ѵ� �׷�����۰� ���� ��� ����
			, ERR_MISMATCH_USE_LEVEL								// ��� ������ �ȵ�
			, ERR_MISMATCH_ENCHANT_LEVEL							// ���� ������ �ȸ���

			, ERR_SUCCESS_RESET_BIND								// �ͼ� ���� ����
			, ERR_NOT_BIND_ITEM										// �ͼ� ���� �������� �ƴ�
			, ERR_DONOT_RESET_BIND									// �ͼ� ���� �Ұ� ������

			, ERR_SUCCESS_BARUNA_GENERAL_ENCHANT_PROTECT			// �ٷ糪 �Ϲ� ���� �ı� ��ȣ
			, ERR_SUCCESS_BARUNA_GENERAL_ENCHANT_PROTECT_KEEP		// �ٷ糪 �Ϲ� ���� �ı� ��ȣ �� ���̳ʽ� ��ȣ
			, ERR_SUCCESS_BARUNA_ATTRIBUTE_ENCHANT_PROTECT			// �ٷ糪 �Ӽ� ���� �ı� ��ȣ
			, ERR_SUCCESS_BARUNA_ATTRIBUTE_ENCHANT_PROTECT_KEEP		// �ٷ糪 �Ӽ� ���� �ı� ��ȣ �� ���̳ʽ� ��ȣ
			, ERR_SUCCESS_BARUNA_ATTRIBUTE_ENCHANT_PROB				// �ٷ糪 �Ӽ� ���� ���� Ȯ�� ����
			, ERR_SUCCESS_BARUNA_PIERCING_PROTECT					// �ٷ糪 �Ǿ�� ���ڼ� �ı� ��ȣ
			, ERR_SUCCESS_BARUNA_PIERCING_PROB						// �ٷ糪 �Ǿ�� ���� Ȯ�� ����

			, ERR_SUCCESS_COLOSSEUM_RETRY							// �ݷμ��� ��õ� ��¼��..
			, ERR_FAIL_COLOSSEUM_RETRY								// �ݷμ��� ��õ� ��¼��..

			, ERR_SUCCESS_COLOSSEUM_BOSS_RATE						// �ݷμ��� ������ Ȯ�� ��¼��..
			, ERR_FAIL_COLOSSEUM_BOSS_RATE							// �ݷμ��� ������ Ȯ�� ��¼��..

		};

};


#define		g_xUseActiveItem	FLUseActiveItem::GetInstance()


#endif // __FLUSEACTIVEITEM_H__

