----------------------------------------------------------------------
---- �̺�Ʈ ���� ��� �Լ� �ε� --------------------------------------
----------------------------------------------------------------------
dofile(".\\LuaFunc\\EventFunc.lua")

----------------------------------------------------------------------
-- �ڵ� ���� (�� ������ TRANS Server �� �����ϸ� �ſ� ^^) ------------
----------------------------------------------------------------------
bNotice = true		-- �ڵ� ���� ���� ����(true or false)
Notice( 1 )
--{
	SetNoticeTime( "Thu 8:50", 3 , 3 )	-- �ڵ������ð�, ��������(MIN), ���� Ƚ��
	AddMessage( "�ȳ��ϼ���. Flyff �Դϴ�." )
	AddMessage( "����� 9�� ���� ���������� ����� �����̿��� " )
	AddMessage( "������ ��ҿ��� ������ ������ �ֽñ� �ٶ��ϴ�." )	
--}

Notice( 2 )
--{
	SetNoticeTime( "Thu 10:00", 120, 99 )	-- �ڵ������ð�, ��������(MIN), ���� Ƚ��
	AddMessage( "�ȳ��ϼ���. Flyff �Դϴ�." )
	AddMessage( "Ÿ�ο��� �������� �����ְų� �뿩�ϴ� ������ ���� �� ������ �Ұ��� �մϴ�." )
	AddMessage( "����� Ÿ�ο��� �ڽ��� �������� �����ִ� ������ �ﰡ�� �ֽñ� �ٶ��ϴ�." )
--}

Notice( 3 )
--{
	SetNoticeTime( "Any 23:00", 1440, 99 )	-- �ڵ������ð�, ��������(MIN), ���� Ƚ��
	AddMessage( "�ȳ��ϼ���. Flyff �Դϴ�." )
	AddMessage( "û�ҳ� ��ȣ ������ ����ʿ� ���� �� 16�� �̸��� ������" )
	AddMessage( "���� 12�ú��� ���� 6�� ���� ������ ���� �ǿ��� �� �� �����Ͽ�" )
	AddMessage( "������ ��ҿ��� ������ ������ �ֽñ� �ٶ��ϴ�." )	
--}

Notice( 4 )
--{
	SetNoticeTime( "Any 23:50", 1440, 99 )	-- �ڵ������ð�, ��������(MIN), ���� Ƚ��
	AddMessage( "�ȳ��ϼ���. Flyff �Դϴ�." )
	AddMessage( "û�ҳ� ��ȣ ������ ����ʿ� ���� �� 16�� �̸��� ������" )
	AddMessage( "���� 12�ú��� ���� 6�� ���� ������ ���� �ǿ��� �� �� �����Ͽ�" )
	AddMessage( "������ ��ҿ��� ������ ������ �ֽñ� �ٶ��ϴ�." )	
--}

----------------------------------------------------------------------
---- �ʱ�ȭ ----------------------------------------------------------
----------------------------------------------------------------------

-- �ð��뺰 ������ ��� ����ġ
tHour = { 505, 409, 324, 280, 220, 203, 202, 212,
	  227, 261, 302, 349, 571, 701, 764, 803,
	  790, 789, 754, 849, 936, 940, 919, 720 }
----------------------------------------------------------------------


----------------------------------------------------------------------------------------------------------------
----  1. AddEvent( strDesc )				-- �̺�Ʈ �߰� �� ���� ���
----  2. SetTime( strStartTime, strEndTime )	-- �ش� �̺�Ʈ�� ���� �ð�, ���� �ð� ���(������ ���� ����)
----											   ( �ð� ���� -- "2007-05-03 17:53" )
----  3. SetItem( ItemId, nMax, nNum, nMinLevel, nMaxLevel )	-- �̺�Ʈ�� ��� ������, ���� �ִ뷮, ��� ����(����),
----   								�������� ����� ������ �ּ�, �ִ� ���� - ������ ����
----  4. SetExpFactor( fFactor )			-- ������ ���� ���
----  5. SetItemDropRate( fFactor )			-- ������ ��ӷ� ���� ���
----  6. SetPieceItemDropRate( fFactor )		-- ���Ͱ� ������ �ִ� ���� �������� ����� ���� ���
----  7. SetGoldDropFactor( fFactor )			-- ��� ��� ���
----  8. SetAttackPower( nAttackPower )		-- ���ݷ� ����
----  9. SetDefensePower( nDefensePower )		-- ���� ����
---- 10. SetCouponEvent( SEC(n) )			-- ���� �̺�Ʈ( ���ӽð� - SEC(n) �Ǵ� MIN(n) )
---- 11. SetLevelUpGift( nLevel, "all", ItemId, nNum, byFlag, nLegendClass, nJob ) -- ������ ����( nLevel�޼��� ������ ����, "all" �κп� Ư�� ���� ���� ����(��. "__bu" - ����, "__an" - ���� ), nJob���� ��������(JOB_MAX ���Խ� ��� ����) 
---- 12. SetCheerExpFactor( fFactor )			-- ���� ����ġ ����
---- 13. SetSpawn( TYPE, strId, nNum, byFlag )		-- �����̺�Ʈ : Type - ITEM �Ǵ� MONSTER, ID, 1�� ������, �÷��װ�( ������ : 0-�Ϲ�, 2-�ͼ� / ���� : �������� ����(0:�񼱰�, 1:����) 
---- 14. SetKeepConnectEvent( nTime, strItemId, nItemNum, byFlag )	-- ���� ���� ������ ���� �̺�Ʈ( ���ӽð�, ������ ID, ������ ����, �÷��װ�:0-�Ϲ�, 2-�ͼ� )
---- 15. SetWeatherEvent( fExpFactor, strTitle )		-- �����̺�Ʈ(����ġ ���� ���, ���� �̺�Ʈ ����) : 15���� �߰���
---- 16. SetShopBuyFactor( fBuyFactor )		-- ���� ���� ���� ����(������ ��� ���� ���� ����)
---- 17. SetShopSellFactor( fSellFactor )		-- ���� ���� ���� ����(������ �Ĵ� ���� ���� ����)
---- 18. SetTroupeExpUpEvent( nTroupeMemberCount, fExpFactor )	-- �ش� ��� �� ����ġ ���� ���( ȿ�� ���� �ޱ� ���� �ּ� �شܿ� ��, ����ġ ���� ��� )
---- 19. SetDailyKeepConnectEvent( ����(0 = Sunday, 1 = Monday, ..., 6 = Saturday), ���ӽð�, ������ ���̵�, �ּ����ް���(�Ϲݸ��), �ִ����ް���(���׸��), ������ �÷���:0-�Ϲ�, 2-�ͼ� )
---- *** �̺�Ʈ �߰� ��Ͻ� 1������ �ݺ��ϰ� 3~19���� �ʿ信 ���� ���������ϴ�.
----------------------------------------------------------------------------------------------------------------
--[[ SAMPLE
AddEvent( "EVENT TEST 01" )
--{
	SetTime( "2007-06-08 14:23", "2007-06-08 16:11" )
	SetTime( "2007-06-09 14:23", "2007-06-10 00:00" )
	
	SetItem( "II_SYS_SYS_EVE_HAPPYMONEY01", 30000, 5, 15, 80 )
	SetItem( "II_SYS_SYS_EVE_PUMPKIN01", 2000, 3, 81, 150 )
	SetExpFactor( 2 )
	SetItemDropRate( 2 )
	SetPieceItemDropRate( 2 )
	SetGoldDropFactor( 2 )
	SetAttackPower( 150 )
	SetDefensePower( 100 )
	SetCouponEvent( MIN(120) )
	SetLevelUpGift( 60, "all", "II_SYS_SYS_SCR_BX_PET_LAWOLF7", 1, 2, LEGEND_CLASS_NORMAL, JOB_KNIGHT )
	SetLevelUpGift( 80, "all", "II_SYS_SYS_SCR_BX_PET_LAWOLF7", 1, 2, LEGEND_CLASS_MASTER, JOB_KNIGHT_MASTER )
	SetLevelUpGift( 125, "all", "II_SYS_SYS_SCR_BX_PET_LAWOLF7", 1, 2, LEGEND_CLASS_HERO, JOB_KNIGHT_HERO)
	SetLevelUpGift( 140, "all", "II_SYS_SYS_SCR_BX_PET_LAWOLF7", 1, 2, LEGEND_CLASS_LEGEND_HERO, JOB_LORDTEMPLER_HERO )
	SetCheerExpFactor( 1.3 )
	SetSpawn( ITEM, "II_SYS_SYS_EVE_HAPPYMONEY01", 10000, 0 )
	SetSpawn( ITEM, "II_SYS_SYS_EVE_HAPPYMONEY01", 10000, 2 )
	SetSpawn( MONSTER, "MI_AIBATT1", 2000, 0 )
	SetSpawn( MONSTER, "MI_AIBATT4", 2000, 1 )
	SetKeepConnectEvent( MIN(60), "II_SYS_SYS_SCR_BXTREASURE01", 1, 2 )
	SetWeatherEvent( 2, "�񰡿��� ����ġ�� 2��~!" )
	SetShopBuyFactor( 1.5 )
	SetShopSellFactor( 0.5 )
	SetTroupeExpUpEvent( 8, 1.2 )
	SetDailyKeepConnectEvent( 0, MIN(60), "II_SYS_SYS_SCR_BXTREASURE01", 1, 3, 2 )
	SetDailyKeepConnectEvent( 1, MIN(60), "II_SYS_SYS_SCR_BXTREASURE01", 1, 3, 2 )
	SetDailyKeepConnectEvent( 2, MIN(60), "II_SYS_SYS_SCR_BXTREASURE01", 1, 3, 2 )
	SetDailyKeepConnectEvent( 3, MIN(60), "II_SYS_SYS_SCR_BXTREASURE01", 1, 3, 2 )
	SetDailyKeepConnectEvent( 4, MIN(60), "II_SYS_SYS_SCR_BXTREASURE01", 1, 3, 2 )
	SetDailyKeepConnectEvent( 5, MIN(60), "II_SYS_SYS_SCR_BXTREASURE01", 1, 3, 2 )
	SetDailyKeepConnectEvent( 6, MIN(60), "II_SYS_SYS_SCR_BXTREASURE01", 1, 3, 2 )
--}
--]]

-------------------------------------------------------------------------
---- Begin Script -------------------------------------------------------
-------------------------------------------------------------------------



