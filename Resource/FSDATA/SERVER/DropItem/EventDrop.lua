----------------------------------------------------------------------
---- �̺�Ʈ ��� ���� ��� �Լ� �ε� --------------------------------------
----------------------------------------------------------------------
dofile(".\\LuaFunc\\EventDropFunc.lua")



----------------------------------------------------------------------------------------------------------------
----  1. AddEvent( szDesc )					-- �̺�Ʈ �߰� �� ���� ���
----  2. SetLang( "LANG_KOR" )					-- ���� ����( ������ ���� ���� )
----  3. SetTime( szStartTime, szEndTime )			-- �ش� �̺�Ʈ�� ���� �ð�, ���� �ð� ���( ������ ���� ���� ) ( �ð� ���� -- "2007-05-03 17:53" )
----  4. SetLevel( dwLevelMin, dwLevelMax )			-- ���� ���� ���� ����( �ּҷ��� �̻�, �ִ뷹�� ���� )
----  5. SetItem( szItemId, limitQuantityPerOneDay, randomQuantityPerOneMonster )	-- �̺�Ʈ�� ��� ������, �Ϻ� �ִ� ��� ����, ���ʹ� ��� ����( ���� ���� ) ( ������ ���� ���� )
----------------------------------------------------------------------------------------------------------------
--[[ SAMPLE
AddEventDrop( "EVENT TEST 01" )
--{
	SetLang( "LANG_KOR" )
	SetLang( "LANG_JAP" )
	
	SetTime( "2007-06-08 14:23", "2007-06-08 16:11" )
	SetTime( "2007-06-09 14:23", "2007-06-10 00:00" )

	SetLevel( 15, 152 )
	
	SetItem( "II_SYS_SYS_EVE_HAPPYMONEY01", 12340, 1 )
	SetItem( "II_SYS_SYS_EVE_PUMPKIN01", 5678, 3 )
--}
--]]

-------------------------------------------------------------------------
---- Begin Script -------------------------------------------------------
-------------------------------------------------------------------------

AddEventDrop( "�ų��� ������ ���� �̺�Ʈ" )
--{
	SetLang( "LANG_KOR" )

	SetTime( "2013-01-23 00:01", "2013-04-30 09:59" )

	SetLevel( 30, 60 )

	SetItem( "II_SYS_SYS_SCR_UNIQCHIP_SUHO", 3000, 1 )
--}

AddEventDrop( "�ų��� ������ ���� �̺�Ʈ" )
--{
	SetLang( "LANG_KOR" )

	SetTime( "2013-01-23 00:01", "2013-04-30 09:59" )

	SetLevel( 50, 80 )

	SetItem( "II_SYS_SYS_SCR_UNIQCHIP_USU", 2500, 1 )
--}

AddEventDrop( "�ų��� ������ ���� �̺�Ʈ" )
--{
	SetLang( "LANG_KOR" )

	SetTime( "2013-01-23 00:01", "2013-04-30 09:59" )

	SetLevel( 70, 100 )

	SetItem( "II_SYS_SYS_SCR_UNIQCHIP_ANGEL", 2000, 1 )
--}

AddEventDrop( "�ų��� ������ ���� �̺�Ʈ" )
--{
	SetLang( "LANG_KOR" )

	SetTime( "2013-01-23 00:01", "2013-04-30 09:59" )

	SetLevel( 90, 120 )

	SetItem( "II_SYS_SYS_SCR_UNIQCHIP_LEAGEND", 1000, 1 )
--}


AddEventDrop( "�ų��� ������ ���� �̺�Ʈ" )
--{
	SetLang( "LANG_KOR" )

	SetTime( "2013-01-23 00:01", "2013-04-30 09:59" )

	SetLevel( 20, 150 )

	SetItem( "II_GEN_GEM_GEM_FLYFFCOUPON_FLY", 700, 1 )
	SetItem( "II_GEN_GEM_GEM_FLYFFCOUPON_FOR", 700, 1 )
	SetItem( "II_GEN_GEM_GEM_FLYFFCOUPON_FUN", 600, 1 )
--}



AddEventDrop( "Cloak event 2016 1" )
--{
	SetLang( "LANG_JAP" )
	SetLang( "LANG_KOR" )

	SetTime( "2016-03-25 00:00", "2016-04-26 11:00" )

	SetLevel( 60, 75 )
	SetLevel( 77, 84 )
	SetLevel( 86, 90 )

	SetItem( "II_SYS_SYS_EVE_CLOAKEN_60_90_2015", 3000, 1 )
--}

AddEventDrop( "Cloak event 2016 2" )
--{
	SetLang( "LANG_JAP" )
	SetLang( "LANG_KOR" )

	SetTime( "2016-03-25 00:00", "2016-04-26 11:00" )

	SetLevel( 91, 105 )
	SetLevel( 107, 114 )
	SetLevel( 116, 120 )

	SetItem( "II_SYS_SYS_EVE_CLOAKEN_91_120_2015", 3000, 1 )
--}

AddEventDrop( "Cloak event 2016 3" )
--{
	SetLang( "LANG_JAP" )
	SetLang( "LANG_KOR" )

	SetTime( "2016-03-25 00:00", "2016-04-26 11:00" )

	SetLevel( 60, 75 )
	SetLevel( 77, 84 )
	SetLevel( 86, 90 )

	SetItem( "II_SYS_SYS_EVE_CLOAKEN_60_90_2015", 3000, 1 )
--}

AddEventDrop( "Cloak event 2016 4" )
--{
	SetLang( "LANG_JAP" )
	SetLang( "LANG_KOR" )

	SetTime( "2016-03-25 00:00", "2016-04-26 11:00" )

	SetLevel( 91, 105 )
	SetLevel( 107, 114 )
	SetLevel( 116, 120 )

	SetItem( "II_SYS_SYS_EVE_CLOAKEN_91_120_2015", 3000, 1 )
--}

AddEventDrop( "Cloak event 2016 5" )
--{
	SetLang( "LANG_JAP" )
	SetLang( "LANG_KOR" )

	SetTime( "2016-03-25 00:00", "2016-04-26 11:00" )

	SetLevel( 121, 133 )
	SetLevel( 135, 150 )

	SetItem( "II_SYS_SYS_EVE_CLOAKEN_121_150_2015", 3000, 1 )
--}
