--------------------------------------------------------------------
-- �ʱ�ȭ ----------------------------------------------------------
--------------------------------------------------------------------
dofile( ".\\LuaFunc\\ItemUpgradeBaseFunc.lua" )
--------------------------------------------------------------------


--------------------------------------------------------------------
-- �Ϲ� ���� -------------------------------------------------------
--------------------------------------------------------------------

AddGeneralEnchant( "IK1_ARMOR", "IK2_CLOTH", "IK3_CLOTH" )
--{
	SetGeneralMaterial( "IK1_GENERAL", "IK2_MATERIAL", "IK3_SMELT_ACCESSORY_MATERIAL" )

--					���ð�,	���� Ȯ��%,	����/�ı�Ȯ��%,	���̳ʽ� 1 ���ð� Ȯ��%( 10000 - ���� Ȯ�� - ���� Ȯ�� ) (%���� n/10000)
	SetGeneralEnchantProb( 		1, 	10000,		0	)	-- auto --
	SetGeneralEnchantProb( 		2, 	10000,		0	)	-- auto --
	SetGeneralEnchantProb( 		3, 	6300,		3700	)	-- auto --
	SetGeneralEnchantProb( 		4, 	4500,		5500	)	-- auto --
	SetGeneralEnchantProb(	 	5, 	3300,		6700	)	-- auto --
	SetGeneralEnchantProb(	 	6, 	2600,		7400	)	-- auto --
	SetGeneralEnchantProb( 		7, 	2100,		7900	)	-- auto --
	SetGeneralEnchantProb( 		8, 	1700,		8300	)	-- auto --
	SetGeneralEnchantProb( 		9, 	1400,		8600	)	-- auto --
	SetGeneralEnchantProb( 		10, 	1100,		8900	)	-- auto --
--}

--------------------------------------------------------------------
-- ���� ------------------------------------------------------------
--------------------------------------------------------------------

AddRandomOptionExtension( "IK1_ARMOR", "IK2_CLOTH", "IK3_CLOAK" )
--{
	SetRandomOptionMaterial( "IK1_ACTIVE", "IK2_ONCE", "IK3_COSTUME_RANDOMOPTION_GEN" )

	SetRandomOptionSizeProb( 1, 10000 )

	SetDstProb( "DST_STR", 700 )
	SetDstProb( "DST_DEX", 1400 )
	SetDstProb( "DST_INT", 2100 )
	SetDstProb( "DST_STA", 2800 )
	SetDstProb( "DST_CHR_CHANCECRITICAL", 3600 )
	SetDstProb( "DST_CRITICAL_BONUS", 4400 )
	SetDstProb( "DST_SPEED", 5100 )
	SetDstProb( "DST_ATTACKSPEED", 5800 )
	SetDstProb( "DST_ADJDEF", 6500 )
	SetDstProb( "DST_ATKPOWER", 7200 )
	SetDstProb( "DST_HP_MAX", 7900 )
	SetDstProb( "DST_MP_MAX", 8600 )
	SetDstProb( "DST_FP_MAX", 9300 )
	SetDstProb( "DST_SPELL_RATE", 10000 )
--}

AddRandomOptionExtension( "IK1_ARMOR", "IK2_CLOTHETC", "IK3_HAT" )
--{
	SetRandomOptionMaterial( "IK1_ACTIVE", "IK2_ONCE", "IK3_COSTUME_RANDOMOPTION_GEN" )

	SetRandomOptionSizeProb( 1, 10000 )

	SetDstProb( "DST_STR", 700 )
	SetDstProb( "DST_DEX", 1400 )
	SetDstProb( "DST_INT", 2100 )
	SetDstProb( "DST_STA", 2800 )
	SetDstProb( "DST_CHR_CHANCECRITICAL", 3600 )
	SetDstProb( "DST_CRITICAL_BONUS", 4400 )
	SetDstProb( "DST_SPEED", 5100 )
	SetDstProb( "DST_ATTACKSPEED", 5800 )
	SetDstProb( "DST_ADJDEF", 6500 )
	SetDstProb( "DST_ATKPOWER", 7200 )
	SetDstProb( "DST_HP_MAX", 7900 )
	SetDstProb( "DST_MP_MAX", 8600 )
	SetDstProb( "DST_FP_MAX", 9300 )
	SetDstProb( "DST_SPELL_RATE", 10000 )
--}

AddRandomOptionExtension( "IK1_ARMOR", "IK2_CLOTH", "IK3_CLOTH" )
--{
	SetRandomOptionMaterial( "IK1_ACTIVE", "IK2_ONCE", "IK3_COSTUME_RANDOMOPTION_GEN" )

	SetRandomOptionSizeProb( 1, 10000 )

	SetDstProb( "DST_STR", 700 )
	SetDstProb( "DST_DEX", 1400 )
	SetDstProb( "DST_INT", 2100 )
	SetDstProb( "DST_STA", 2800 )
	SetDstProb( "DST_CHR_CHANCECRITICAL", 3600 )
	SetDstProb( "DST_CRITICAL_BONUS", 4400 )
	SetDstProb( "DST_SPEED", 5100 )
	SetDstProb( "DST_ATTACKSPEED", 5800 )
	SetDstProb( "DST_ADJDEF", 6500 )
	SetDstProb( "DST_ATKPOWER", 7200 )
	SetDstProb( "DST_HP_MAX", 7900 )
	SetDstProb( "DST_MP_MAX", 8600 )
	SetDstProb( "DST_FP_MAX", 9300 )
	SetDstProb( "DST_SPELL_RATE", 10000 )
--}

AddRandomOptionExtension( "IK1_ARMOR", "IK2_CLOTHETC", "IK3_GLOVE" )
--{
	SetRandomOptionMaterial( "IK1_ACTIVE", "IK2_ONCE", "IK3_COSTUME_RANDOMOPTION_GEN" )

	SetRandomOptionSizeProb( 1, 10000 )

	SetDstProb( "DST_STR", 700 )
	SetDstProb( "DST_DEX", 1400 )
	SetDstProb( "DST_INT", 2100 )
	SetDstProb( "DST_STA", 2800 )
	SetDstProb( "DST_CHR_CHANCECRITICAL", 3600 )
	SetDstProb( "DST_CRITICAL_BONUS", 4400 )
	SetDstProb( "DST_SPEED", 5100 )
	SetDstProb( "DST_ATTACKSPEED", 5800 )
	SetDstProb( "DST_ADJDEF", 6500 )
	SetDstProb( "DST_ATKPOWER", 7200 )
	SetDstProb( "DST_HP_MAX", 7900 )
	SetDstProb( "DST_MP_MAX", 8600 )
	SetDstProb( "DST_FP_MAX", 9300 )
	SetDstProb( "DST_SPELL_RATE", 10000 )
--}

AddRandomOptionExtension( "IK1_ARMOR", "IK2_CLOTHETC", "IK3_SHOES" )
--{
	SetRandomOptionMaterial( "IK1_ACTIVE", "IK2_ONCE", "IK3_COSTUME_RANDOMOPTION_GEN" )

	SetRandomOptionSizeProb( 1, 10000 )

	SetDstProb( "DST_STR", 700 )
	SetDstProb( "DST_DEX", 1400 )
	SetDstProb( "DST_INT", 2100 )
	SetDstProb( "DST_STA", 2800 )
	SetDstProb( "DST_CHR_CHANCECRITICAL", 3600 )
	SetDstProb( "DST_CRITICAL_BONUS", 4400 )
	SetDstProb( "DST_SPEED", 5100 )
	SetDstProb( "DST_ATTACKSPEED", 5800 )
	SetDstProb( "DST_ADJDEF", 6500 )
	SetDstProb( "DST_ATKPOWER", 7200 )
	SetDstProb( "DST_HP_MAX", 7900 )
	SetDstProb( "DST_MP_MAX", 8600 )
	SetDstProb( "DST_FP_MAX", 9300 )
	SetDstProb( "DST_SPELL_RATE", 10000 )
--}


--------------------------------------------------------------------
-- �� �Ķ���͵��� ���� Ȯ�� ---------------------------------------
--------------------------------------------------------------------

-- Ȯ�� 1/10��
AddDstParameter( "DST_STR" )
--{
	SetAdjValueProb( 1, 600000000 )
	SetAdjValueProb( 4, 900000000 )
	SetAdjValueProb( 7, 1000000000 )
--}

AddDstParameter( "DST_DEX" )
--{
	SetAdjValueProb( 1, 600000000 )
	SetAdjValueProb( 4, 900000000 )
	SetAdjValueProb( 7, 1000000000 )
--}

AddDstParameter( "DST_INT" )
--{
	SetAdjValueProb( 1, 600000000 )
	SetAdjValueProb( 4, 900000000 )
	SetAdjValueProb( 7, 1000000000 )
--}

AddDstParameter( "DST_STA" )
--{
	SetAdjValueProb( 1, 600000000 )
	SetAdjValueProb( 4, 900000000 )
	SetAdjValueProb( 7, 1000000000 )
--}

AddDstParameter( "DST_CHR_CHANCECRITICAL" )
--{
	SetAdjValueProb( 1, 600000000 )
	SetAdjValueProb( 2, 900000000 )
	SetAdjValueProb( 3, 1000000000 )
--}

AddDstParameter( "DST_CRITICAL_BONUS" )
--{
	SetAdjValueProb( 1, 600000000 )
	SetAdjValueProb( 3, 900000000 )
	SetAdjValueProb( 5, 1000000000 )
--}

AddDstParameter( "DST_SPEED" )
--{
	SetAdjValueProb( 1, 600000000 )
	SetAdjValueProb( 2, 900000000 )
	SetAdjValueProb( 3, 1000000000 )
--}

AddDstParameter( "DST_ATTACKSPEED" )
--{
	SetAdjValueProb( 20, 600000000 )
	SetAdjValueProb( 60, 900000000 )
	SetAdjValueProb( 100, 1000000000 )
--}

AddDstParameter( "DST_ADJDEF" )
--{
	SetAdjValueProb( 4, 600000000 )
	SetAdjValueProb( 16, 900000000 )
	SetAdjValueProb( 28, 1000000000 )
--}

AddDstParameter( "DST_ATKPOWER" )
--{
	SetAdjValueProb( 5, 600000000 )
	SetAdjValueProb( 20, 900000000 )
	SetAdjValueProb( 35, 1000000000 )
--}

AddDstParameter( "DST_HP_MAX" )
--{
	SetAdjValueProb( 50, 600000000 )
	SetAdjValueProb( 100, 900000000 )
	SetAdjValueProb( 150, 1000000000 )
--}

AddDstParameter( "DST_MP_MAX" )
--{
	SetAdjValueProb( 50, 600000000 )
	SetAdjValueProb( 100, 900000000 )
	SetAdjValueProb( 150, 1000000000 )
--}

AddDstParameter( "DST_FP_MAX" )
--{
	SetAdjValueProb( 50, 600000000 )
	SetAdjValueProb( 100, 900000000 )
	SetAdjValueProb( 150, 1000000000 )
--}

AddDstParameter( "DST_SPELL_RATE" )
--{
	SetAdjValueProb( 1, 600000000 )
	SetAdjValueProb( 2, 900000000 )
	SetAdjValueProb( 3, 1000000000 )
--}

