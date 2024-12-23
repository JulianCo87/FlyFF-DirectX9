#ifndef __FLUSEEFFECTITEM_H__
#define __FLUSEEFFECTITEM_H__


class FLUseEffectItem
{
	public:
		static	FLUseEffectItem*	GetInstance();

		FLUseEffectItem();
		~FLUseEffectItem();

		int		OnDoUseEffectItem( FLWSUser* pUser, FLItemElem* pItemElem );

	private:
		int		DoUseEffectItem( FLWSUser* pUSer, FLItemElem* pItemElem );
		int		CheckEffectItem( FLWSUser* pUser, PT_ITEM_SPEC pItemProp );
		int		GetLimitCountUseMax( PT_ITEM_SPEC pItemProp );


	public:
		enum USE_EFFECTITEM_RESULT
		{
			ERR_GENERAL_ERROR = 0
			, ERR_SUCCESS_USE							// ����
			, ERR_USER_INVALID							// �̻��� ����
			, ERR_ITEM_INVALID							// �̻��� ������
			, ERR_DONOT_USE_STATE						// ���� ������ ���°� �ƴ�
			, ERR_ALEADY_USE							// �̹� �������� ������ �����
			, ERR_DONOTUSE_ANOTHER						// ��� ���ѵ� �׷�����۰� ���� ��� ����
			, ERR_MISMATCH_USE_LEVEL					// ��� ������ �ȵ�
			, ERR_MAX_OVER								// �ִ� ��� ���� �ʰ�
			, ERR_NOT_PK_ENABLE_CHANNEL					// PK ������ ä���� �ƴ�
			, ERR_NOT_CHAO_PLAYER						// ī�� �÷��̾ �ƴ�( PK ��ġ�� ���� )

		};

};


#define		g_xUseEffectItem	FLUseEffectItem::GetInstance()


#endif // __FLUSEEFFECTITEM_H__

