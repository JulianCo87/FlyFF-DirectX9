----------------------------------------------------------------------
---- 이벤트 드랍 관련 루아 함수 로딩 --------------------------------------
----------------------------------------------------------------------
dofile(".\\LuaFunc\\EventDropFunc.lua")



----------------------------------------------------------------------------------------------------------------
----  1. AddEvent( szDesc )					-- 이벤트 추가 및 설명 등록
----  2. SetLang( "LANG_KOR" )					-- 국가 설정( 여러개 설정 가능 )
----  3. SetTime( szStartTime, szEndTime )			-- 해당 이벤트의 시작 시간, 종료 시간 등록( 여러개 설정 가능 ) ( 시간 형식 -- "2007-05-03 17:53" )
----  4. SetLevel( dwLevelMin, dwLevelMax )			-- 몬스터 레벨 구간 설정( 최소레벨 이상, 최대레벨 이하 )
----  5. SetItem( szItemId, limitQuantityPerOneDay, randomQuantityPerOneMonster )	-- 이벤트용 드롭 아이템, 일별 최대 드롭 개수, 몬스터당 드롭 개수( 랜덤 지급 ) ( 여러개 설정 가능 )
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

AddEventDrop( "신나는 아이템 팡팡 이벤트" )
--{
	SetLang( "LANG_KOR" )

	SetTime( "2013-01-23 00:01", "2013-04-30 09:59" )

	SetLevel( 30, 60 )

	SetItem( "II_SYS_SYS_SCR_UNIQCHIP_SUHO", 3000, 1 )
--}

AddEventDrop( "신나는 아이템 팡팡 이벤트" )
--{
	SetLang( "LANG_KOR" )

	SetTime( "2013-01-23 00:01", "2013-04-30 09:59" )

	SetLevel( 50, 80 )

	SetItem( "II_SYS_SYS_SCR_UNIQCHIP_USU", 2500, 1 )
--}

AddEventDrop( "신나는 아이템 팡팡 이벤트" )
--{
	SetLang( "LANG_KOR" )

	SetTime( "2013-01-23 00:01", "2013-04-30 09:59" )

	SetLevel( 70, 100 )

	SetItem( "II_SYS_SYS_SCR_UNIQCHIP_ANGEL", 2000, 1 )
--}

AddEventDrop( "신나는 아이템 팡팡 이벤트" )
--{
	SetLang( "LANG_KOR" )

	SetTime( "2013-01-23 00:01", "2013-04-30 09:59" )

	SetLevel( 90, 120 )

	SetItem( "II_SYS_SYS_SCR_UNIQCHIP_LEAGEND", 1000, 1 )
--}


AddEventDrop( "신나는 아이템 팡팡 이벤트" )
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
