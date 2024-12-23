#ifndef __FLITEMSHOPPING_H__
#define __FLITEMSHOPPING_H__


class FLItemShopping
{
	public:
		static FLItemShopping* GetInstance();

		FLItemShopping();
		~FLItemShopping();

		int		BuyItemByBasket( FLWSUser* pUser, const PT_PACKET_SHOPPING_BASKET ptShoppingBasket );


	private:
		int		BuyItemFromNPC( FLWSUser* pUser, const PT_PACKET_SHOPPING_BASKET ptShoppingBasket );
		int		CheckBuyItemFromNPC( FLWSUser* pUser, const PT_PACKET_SHOPPING_BASKET ptShoppingBasket );
		int		CreateItemFromNPC( FLWSUser* pUser, const PT_PACKET_SHOPPING_BASKET ptShoppingBasket );


		int		BuyItemFromPC( FLWSUser* pUser, const PT_PACKET_SHOPPING_BASKET ptShoppingBasket );


	public:
		enum ITEM_SHOPPING_RESULT
		{
			ERR_GENERAL_ERROR = 0
			, ERR_SUCCESS						// ����
			, ERR_USER_INVALID					// �̻��� ����
			, ERR_ITEM_INVALID					// �̻��� ������
			, ERR_VENDOR_INVALID				// �ŷ��� �̻�
			, ERR_SHOPPING_INFO_INVALID			// �������� �̻�
			, ERR_LACK_SPACE					// ���� ����
			, ERR_LACK_MONEY					// �� ����
			, ERR_LACK_CHIP						// Ĩ ����
			, ERR_BUY_ONLY_OCCUPATION			// ���ɱ�� ���� ������
			, ERR_SUMCOST_OVERFLOW				// �� ���� �������� ������ ������ ���� �� �ִ� ��ķ��� �ʰ�
			, ERR_SUMCHIP_OVERFLOW				// �� ���� �������� Ĩ�� ������ ���� �� �ִ� Ĩ�� �ʰ�
		};

};

#define g_xItemShopping	FLItemShopping::GetInstance()


#endif // __FLITEMSHOPPING_H__