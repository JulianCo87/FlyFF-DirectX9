--------------------------------------------------------------------
-- �ʱ�ȭ ----------------------------------------------------------
--------------------------------------------------------------------
dofile( ".\\LuaFunc\\ItemUpgradeBaseFunc.lua" )
--------------------------------------------------------------------


--------------------------------------------------------------------
-- �Ϲ� ���� -------------------------------------------------------
--------------------------------------------------------------------

AddGeneralEnchant( "IK1_WEAPON", "IK2_NONE", "IK3_NONE" )
--{
	SetGeneralMaterial( "IK1_PASSIVE", "IK2_MATERIAL", "IK3_OPERCID" )

--					���ð�,	���� Ȯ��%,	����/�ı�Ȯ��%,	���̳ʽ� 1 ���ð� Ȯ��%( 10000 - ���� Ȯ�� - ���� Ȯ�� ) (%���� n/10000)
	SetGeneralEnchantProb( 		1, 	10000,		0	)	-- auto --
	SetGeneralEnchantProb( 		2, 	10000,		0	)	-- auto --
	SetGeneralEnchantProb( 		3, 	9000,		750	)	-- auto --
	SetGeneralEnchantProb( 		4, 	8500,		1125	)	-- auto --
	SetGeneralEnchantProb( 		5, 	8000,		1500	)	-- auto --
	SetGeneralEnchantProb( 		6, 	7000,		2250	)	-- auto --
	SetGeneralEnchantProb( 		7, 	6000,		3000	)	-- auto --
	SetGeneralEnchantProb( 		8, 	5000,		3750	)	-- auto --
	SetGeneralEnchantProb( 		9, 	4000,		4500	)	-- auto --
	SetGeneralEnchantProb( 		10, 	3000,		5250	)	-- auto --
	SetGeneralEnchantProb(		11, 	2000,		6000	)	-- auto --
	SetGeneralEnchantProb(		12,	1000,		6750	)	-- auto --
	SetGeneralEnchantProb( 		13,	750,		6937	)	-- auto --
	SetGeneralEnchantProb( 		14,	500,		7125	)	-- auto --
	SetGeneralEnchantProb( 		15,	350,		7237	)	-- auto --
	SetGeneralEnchantProb( 		16, 	200,		7350	)	-- auto --
	SetGeneralEnchantProb( 		17, 	150,		7387	)	-- auto --
	SetGeneralEnchantProb( 		18, 	100,		7425	)	-- auto --
	SetGeneralEnchantProb( 		19, 	75,		7444	)	-- auto --
	SetGeneralEnchantProb( 		20, 	50,		7462	)	-- auto --
--}

AddGeneralEnchant( "IK1_ARMOR", "IK2_ARMOR", "IK3_NONE" )
--{
	SetGeneralMaterial( "IK1_PASSIVE", "IK2_MATERIAL", "IK3_OPERCID"  )

--					���ð�,	���� Ȯ��%,	����/�ı�Ȯ��%,	���̳ʽ�1 ���ð� Ȯ��%( 10000 - ���� Ȯ�� - ���� Ȯ�� )	(%���� n/10000)
	SetGeneralEnchantProb( 		1, 	10000,		0	)	-- auto --
	SetGeneralEnchantProb( 		2, 	10000,		0	)	-- auto --
	SetGeneralEnchantProb( 		3, 	9500,		375	)	-- auto --
	SetGeneralEnchantProb( 		4, 	9000,		750	)	-- auto --
	SetGeneralEnchantProb( 		5, 	8500,		1125	)	-- auto --
	SetGeneralEnchantProb( 		6, 	7750,		1687	)	-- auto --
	SetGeneralEnchantProb( 		7, 	7000,		2250	)	-- auto --
	SetGeneralEnchantProb( 		8, 	6250,		2812	)	-- auto --
	SetGeneralEnchantProb( 		9, 	5500,		3375	)	-- auto --
	SetGeneralEnchantProb( 		10, 	4750,		3937	)	-- auto --
	SetGeneralEnchantProb( 		11, 	4000,		4500	)	-- auto --
	SetGeneralEnchantProb( 		12,	3250,		5062	)	-- auto --
	SetGeneralEnchantProb( 		13,	2250,		5812	)	-- auto --
	SetGeneralEnchantProb( 		14,	1250,		6562	)	-- auto --
	SetGeneralEnchantProb( 		15,	1000,		6750	)	-- auto --
	SetGeneralEnchantProb( 		16, 	750,		6937	)	-- auto --
	SetGeneralEnchantProb( 		17, 	500,		7125	)	-- auto --
	SetGeneralEnchantProb( 		18, 	300,		7275	)	-- auto --
	SetGeneralEnchantProb( 		19, 	150,		7387	)	-- auto --
	SetGeneralEnchantProb( 		20, 	100,		7425	)	-- auto --
--}

AddGeneralEnchant( "IK1_ARMOR", "IK2_ARMORETC", "IK3_NONE" )
--{
	SetGeneralMaterial( "IK1_PASSIVE", "IK2_MATERIAL", "IK3_OPERCID"  )

--					���ð�,	���� Ȯ��%,	����/�ı�Ȯ��%,	���̳ʽ�1 ���ð� Ȯ��%( 10000 - ���� Ȯ�� - ���� Ȯ�� )	(%���� n/10000)
	SetGeneralEnchantProb( 		1, 	10000,		0	)	-- auto --
	SetGeneralEnchantProb( 		2, 	10000,		0	)	-- auto --
	SetGeneralEnchantProb( 		3, 	9500,		375	)	-- auto --
	SetGeneralEnchantProb( 		4, 	9000,		750	)	-- auto --
	SetGeneralEnchantProb( 		5, 	8500,		1125	)	-- auto --
	SetGeneralEnchantProb( 		6, 	7750,		1687	)	-- auto --
	SetGeneralEnchantProb( 		7, 	7000,		2250	)	-- auto --
	SetGeneralEnchantProb( 		8, 	6250,		2812	)	-- auto --
	SetGeneralEnchantProb( 		9, 	5500,		3375	)	-- auto --
	SetGeneralEnchantProb( 		10, 	4750,		3937	)	-- auto --
	SetGeneralEnchantProb( 		11, 	4000,		4500	)	-- auto --
	SetGeneralEnchantProb( 		12,	3250,		5062	)	-- auto --
	SetGeneralEnchantProb( 		13,	2250,		5812	)	-- auto --
	SetGeneralEnchantProb( 		14,	1250,		6562	)	-- auto --
	SetGeneralEnchantProb( 		15,	1000,		6750	)	-- auto --
	SetGeneralEnchantProb( 		16, 	750,		6937	)	-- auto --
	SetGeneralEnchantProb( 		17, 	500,		7125	)	-- auto --
	SetGeneralEnchantProb( 		18, 	300,		7275	)	-- auto --
	SetGeneralEnchantProb( 		19, 	150,		7387	)	-- auto --
	SetGeneralEnchantProb( 		20, 	100,		7425	)	-- auto --
--}
--------------------------------------------------------------------
-- �Ӽ� ���� -------------------------------------------------------
--------------------------------------------------------------------

AddAttributeEnchant( "IK1_WEAPON", "IK2_NONE", "IK3_NONE" )
--{
	SetAttributeMaterial( "IK1_PASSIVE", "IK2_BARUNA", "IK3_ELEORB" )

	SetAttributeRemovePenya( 100000 )

--					���ð�,	���� Ȯ��%,	���� Ȯ��%,	������������,	����������,	�Ӽ�����������  (���ð��� ������ ��簪�� n/10000)
	SetAttributeEnchantProb( 	1, 	10000,		0,		200,		200,		500	)
	SetAttributeEnchantProb( 	2, 	10000,		0,		221,		221,		522	)
	SetAttributeEnchantProb( 	3, 	9000,		750,		256,		256,		560	)
	SetAttributeEnchantProb( 	4, 	8500,		1125,		305,		305,		612	)
	SetAttributeEnchantProb( 	5, 	8000,		1500,		368,		368,		680	)
	SetAttributeEnchantProb( 	6, 	7000,		2250,		446,		446,		763	)
	SetAttributeEnchantProb( 	7, 	6000,		3000,		537,		537,		860	)
	SetAttributeEnchantProb( 	8, 	5000,		3750,		642,		642,		973	)
	SetAttributeEnchantProb( 	9, 	4000,		4500,		761,		761,		1101	)
	SetAttributeEnchantProb( 	10, 	3000,		5250,		895,		895,		1244	)
	SetAttributeEnchantProb(	11, 	2000,		6000,		1042,		1042,		1402	)
	SetAttributeEnchantProb(	12,	1000,		6750,		1203,		1203,		1575	)
	SetAttributeEnchantProb( 	13,	750,		6937,		1379,		1379,		1763	)
	SetAttributeEnchantProb( 	14,	500,		7125,		1568,		1568,		1966	)
	SetAttributeEnchantProb( 	15,	350,		7237,		1772,		1772,		2184	)
	SetAttributeEnchantProb( 	16, 	200,		7350,		1989,		1989,		2417	)
	SetAttributeEnchantProb( 	17, 	150,		7387,		2221,		2221,		2665	)
	SetAttributeEnchantProb( 	18, 	100,		7425,		2467,		2467,		2998	)
	SetAttributeEnchantProb( 	19, 	75,		7444,		2726,		2726,		3207	)
	SetAttributeEnchantProb( 	20, 	50,		7462,		3000,		3000,		3500	)
--}

AddAttributeEnchant( "IK1_ARMOR", "IK2_ARMOR", "IK3_SUIT" )
--{
	SetAttributeMaterial( "IK1_PASSIVE", "IK2_BARUNA", "IK3_ELEORB" )

	SetAttributeRemovePenya( 100000 )

--					���ð�,	���� Ȯ��%,	���� Ȯ��%,	������������,	����������,	�Ӽ�����������  (���ð��� ������ ��簪�� n/10000)
	SetAttributeEnchantProb( 	1, 	10000,		0,		200,		200,		500	)
	SetAttributeEnchantProb( 	2, 	10000,		0,		221,		221,		522	)
	SetAttributeEnchantProb( 	3, 	9500,		375,		256,		256,		560	)
	SetAttributeEnchantProb( 	4, 	9000,		750,		305,		305,		612	)
	SetAttributeEnchantProb( 	5, 	8500,		1125,		368,		368,		680	)
	SetAttributeEnchantProb( 	6, 	7750,		1687,		446,		446,		763	)
	SetAttributeEnchantProb( 	7, 	7000,		2250,		537,		537,		860	)
	SetAttributeEnchantProb( 	8, 	6250,		2812,		642,		642,		973	)
	SetAttributeEnchantProb( 	9, 	5500,		3375,		761,		761,		1101	)
	SetAttributeEnchantProb( 	10, 	4750,		3937,		895,		895,		1244	)
	SetAttributeEnchantProb( 	11, 	4000,		4500,		1042,		1042,		1402	)
	SetAttributeEnchantProb( 	12,	3250,		5062,		1203,		1203,		1575	)
	SetAttributeEnchantProb( 	13,	2250,		5812,		1379,		1379,		1763	)
	SetAttributeEnchantProb( 	14,	1250,		6562,		1568,		1568,		1966	)
	SetAttributeEnchantProb( 	15,	1000,		6750,		1772,		1772,		2184	)
	SetAttributeEnchantProb( 	16, 	750,		6937,		1989,		1989,		2417	)
	SetAttributeEnchantProb( 	17, 	500,		7125,		2221,		2221,		2665	)
	SetAttributeEnchantProb( 	18, 	300,		7275,		2467,		2467,		2998	)
	SetAttributeEnchantProb( 	19, 	150,		7387,		2726,		2726,		3207	)
	SetAttributeEnchantProb( 	20, 	100,		7425,		3000,		3000,		3500	)
--}


--------------------------------------------------------------------
-- �Ǿ�� ----------------------------------------------------------
--------------------------------------------------------------------
		
AddPiercing( "IK1_WEAPON", "IK2_NONE", "IK3_NONE" )		-- �Ǿ�� �� �������� MainCategory, SubCategory
--{
	SetPiercingMaterial( "IK1_NONE", "IK2_NONE", "IK3_NONE" )	-- �Ǿ�� ����� �ø��� ����� �������� MainCategory, SubCategory

	SetPiercedItem( "IK1_ACTIVE", "IK2_ONCE", "IK3_BARUNAWEA_PIERCE_RUNE" )	-- ���Ͽ� ���� �������� MainCategory, SubCategory

	SetPiercingSizeIncreasePenya( 0 )			-- �Ǿ�� ����� �ø��� �Һ�Ǵ� ���

	SetPiercedItemRemovePenya( 0 )			-- �Ǿ�̵� �������� �����Ҷ� �Һ�Ǵ� ���

	SetPiercingSizeProb( 1, 0 )				-- ������, ���� Ȯ��( 1/10000 )
--}

AddPiercing( "IK1_ARMOR", "IK2_ARMOR", "IK3_SHIELD" )	-- �Ǿ�� �� �������� MainCategory, SubCategory
--{
	SetPiercingMaterial( "IK1_NONE", "IK2_NONE", "IK3_NONE" )	-- �Ǿ�� ����� �ø��� ����� �������� MainCategory, SubCategory

	SetPiercedItem( "IK1_ACTIVE", "IK2_ONCE", "IK3_BARUNAWEA_PIERCE_RUNE" )	-- ���Ͽ� ���� �������� MainCategory, SubCategory

	SetPiercingSizeIncreasePenya( 0 )			-- �Ǿ�� ����� �ø��� �Һ�Ǵ� ���

	SetPiercedItemRemovePenya( 0 )			-- �Ǿ�̵� �������� �����Ҷ� �Һ�Ǵ� ���

	SetPiercingSizeProb( 1, 0 )				-- ������, ���� Ȯ��( 1/10000 )
--}
AddPiercing( "IK1_ARMOR", "IK2_ARMOR", "IK3_ZEMBARUNA" )	-- �Ǿ�� �� �������� MainCategory, SubCategory
--{
	SetPiercingMaterial( "IK1_NONE", "IK2_NONE", "IK3_NONE" )	-- �Ǿ�� ����� �ø��� ����� �������� MainCategory, SubCategory

	SetPiercedItem( "IK1_ACTIVE", "IK2_ONCE", "IK3_BARUNAWEA_PIERCE_RUNE" )	-- ���Ͽ� ���� �������� MainCategory, SubCategory

	SetPiercingSizeIncreasePenya( 0 )			-- �Ǿ�� ����� �ø��� �Һ�Ǵ� ���

	SetPiercedItemRemovePenya( 0 )			-- �Ǿ�̵� �������� �����Ҷ� �Һ�Ǵ� ���

	SetPiercingSizeProb( 1, 0 )				-- ������, ���� Ȯ��( 1/10000 )
--}
AddPiercing( "IK1_ARMOR", "IK2_ARMOR", "IK3_MAGICBARUNA" )	-- �Ǿ�� �� �������� MainCategory, SubCategory
--{
	SetPiercingMaterial( "IK1_NONE", "IK2_NONE", "IK3_NONE" )	-- �Ǿ�� ����� �ø��� ����� �������� MainCategory, SubCategory

	SetPiercedItem( "IK1_ACTIVE", "IK2_ONCE", "IK3_BARUNAWEA_PIERCE_RUNE" )	-- ���Ͽ� ���� �������� MainCategory, SubCategory

	SetPiercingSizeIncreasePenya( 0 )			-- �Ǿ�� ����� �ø��� �Һ�Ǵ� ���

	SetPiercedItemRemovePenya( 0 )			-- �Ǿ�̵� �������� �����Ҷ� �Һ�Ǵ� ���

	SetPiercingSizeProb( 1, 0 )				-- ������, ���� Ȯ��( 1/10000 )
--}

AddPiercing( "IK1_ARMOR", "IK2_ARMOR", "IK3_SUIT" )	-- �Ǿ�� �� �������� MainCategory, SubCategory
--{
	SetPiercingMaterial( "IK1_NONE", "IK2_NONE", "IK3_NONE" )	-- �Ǿ�� ����� �ø��� ����� �������� MainCategory, SubCategory

	SetPiercedItem( "IK1_ACTIVE", "IK2_ONCE", "IK3_BARUNAWEA_PIERCE_RUNE" )	-- ���Ͽ� ���� �������� MainCategory, SubCategory

	SetPiercingSizeIncreasePenya( 0 )			-- �Ǿ�� ����� �ø��� �Һ�Ǵ� ���

	SetPiercedItemRemovePenya( 0 )			-- �Ǿ�̵� �������� �����Ҷ� �Һ�Ǵ� ���

	SetPiercingSizeProb( 1, 0 )				-- ������, ���� Ȯ��( 1/10000 )
--}


--------------------------------------------------------------------
-- ���� ------------------------------------------------------------
--------------------------------------------------------------------

AddRandomOptionExtension(  "IK1_WEAPON", "IK2_NONE", "IK3_NONE"  )
--{
	SetRandomOptionMaterial( "IK1_NONE", "IK2_NONE", "IK3_NONE"  )

	SetRandomOptionGeneratePenya( 100000 )

	SetRandomOptionSizeProb( 1, 10000 )
	SetRandomOptionSizeProb( 2, 10000 )
	SetRandomOptionSizeProb( 3, 10000 )
	SetRandomOptionSizeProb( 4, 10000 )
	SetRandomOptionSizeProb( 5, 10000 )

	SetDummyRandomOptionSize( 20 )

	SetDstProb( "DST_STR", 700, 440, 700 )
	SetDstProb( "DST_DEX", 1400 , 880, 1400)
	SetDstProb( "DST_INT", 2100 , 1320, 2100)
	SetDstProb( "DST_STA", 2800 , 1760, 2800)
	SetDstProb( "DST_CHR_CHANCECRITICAL", 3600 , 2200, 3600)
	SetDstProb( "DST_CRITICAL_BONUS", 4400 ,2640 , 4400)
	SetDstProb( "DST_SPEED", 5100 , 4040 , 5100)
	SetDstProb( "DST_ATTACKSPEED", 5800 , 4480, 5800)
	SetDstProb( "DST_ADJDEF", 6500 , 5880, 6500)
	SetDstProb( "DST_ATKPOWER", 7200 , 6320, 7200)
	SetDstProb( "DST_HP_MAX", 7900 , 7720, 7900)
	SetDstProb( "DST_MP_MAX", 8600 , 8160, 8600)
	SetDstProb( "DST_FP_MAX", 9300 , 9560, 9300)
	SetDstProb( "DST_SPELL_RATE", 10000 , 10000 , 10000)
--}

AddRandomOptionExtension( "IK1_ARMOR", "IK2_ARMOR", "IK3_SHIELD" )
--{
	SetRandomOptionMaterial( "IK1_NONE", "IK2_NONE", "IK3_NONE" )

	SetRandomOptionGeneratePenya( 100000 )

	SetRandomOptionSizeProb( 1, 10000 )
	SetRandomOptionSizeProb( 2, 10000 )
	SetRandomOptionSizeProb( 3, 10000 )
	SetRandomOptionSizeProb( 4, 10000 )
	SetRandomOptionSizeProb( 5, 10000 )

	SetDummyRandomOptionSize( 20 )

	SetDstProb( "DST_STR", 700, 440, 700 )
	SetDstProb( "DST_DEX", 1400 , 880, 1400)
	SetDstProb( "DST_INT", 2100 , 1320, 2100)
	SetDstProb( "DST_STA", 2800 , 1760, 2800)
	SetDstProb( "DST_CHR_CHANCECRITICAL", 3600 , 2200, 3600)
	SetDstProb( "DST_CRITICAL_BONUS", 4400 ,2640 , 4400)
	SetDstProb( "DST_SPEED", 5100 , 4040 , 5100)
	SetDstProb( "DST_ATTACKSPEED", 5800 , 4480, 5800)
	SetDstProb( "DST_ADJDEF", 6500 , 5880, 6500)
	SetDstProb( "DST_ATKPOWER", 7200 , 6320, 7200)
	SetDstProb( "DST_HP_MAX", 7900 , 7720, 7900)
	SetDstProb( "DST_MP_MAX", 8600 , 8160, 8600)
	SetDstProb( "DST_FP_MAX", 9300 , 9560, 9300)
	SetDstProb( "DST_SPELL_RATE", 10000 , 10000 , 10000)
--}
AddRandomOptionExtension( "IK1_ARMOR", "IK2_ARMOR", "IK3_ZEMBARUNA" )
--{
	SetRandomOptionMaterial( "IK1_NONE", "IK2_NONE", "IK3_NONE" )

	SetRandomOptionGeneratePenya( 100000 )

	SetRandomOptionSizeProb( 1, 10000 )
	SetRandomOptionSizeProb( 2, 10000 )
	SetRandomOptionSizeProb( 3, 10000 )
	SetRandomOptionSizeProb( 4, 10000 )
	SetRandomOptionSizeProb( 5, 10000 )

	SetDummyRandomOptionSize( 20 )

	SetDstProb( "DST_STR", 700, 440, 700 )
	SetDstProb( "DST_DEX", 1400 , 880, 1400)
	SetDstProb( "DST_INT", 2100 , 1320, 2100)
	SetDstProb( "DST_STA", 2800 , 1760, 2800)
	SetDstProb( "DST_CHR_CHANCECRITICAL", 3600 , 2200, 3600)
	SetDstProb( "DST_CRITICAL_BONUS", 4400 ,2640 , 4400)
	SetDstProb( "DST_SPEED", 5100 , 4040 , 5100)
	SetDstProb( "DST_ATTACKSPEED", 5800 , 4480, 5800)
	SetDstProb( "DST_ADJDEF", 6500 , 5880, 6500)
	SetDstProb( "DST_ATKPOWER", 7200 , 6320, 7200)
	SetDstProb( "DST_HP_MAX", 7900 , 7720, 7900)
	SetDstProb( "DST_MP_MAX", 8600 , 8160, 8600)
	SetDstProb( "DST_FP_MAX", 9300 , 9560, 9300)
	SetDstProb( "DST_SPELL_RATE", 10000 , 10000 , 10000)
--}
AddRandomOptionExtension( "IK1_ARMOR", "IK2_ARMOR", "IK3_MAGICBARUNA" )
--{
	SetRandomOptionMaterial( "IK1_NONE", "IK2_NONE", "IK3_NONE" )

	SetRandomOptionGeneratePenya( 100000 )

	SetRandomOptionSizeProb( 1, 10000 )
	SetRandomOptionSizeProb( 2, 10000 )
	SetRandomOptionSizeProb( 3, 10000 )
	SetRandomOptionSizeProb( 4, 10000 )
	SetRandomOptionSizeProb( 5, 10000 )

	SetDummyRandomOptionSize( 20 )

	SetDstProb( "DST_STR", 700, 440, 700 )
	SetDstProb( "DST_DEX", 1400 , 880, 1400)
	SetDstProb( "DST_INT", 2100 , 1320, 2100)
	SetDstProb( "DST_STA", 2800 , 1760, 2800)
	SetDstProb( "DST_CHR_CHANCECRITICAL", 3600 , 2200, 3600)
	SetDstProb( "DST_CRITICAL_BONUS", 4400 ,2640 , 4400)
	SetDstProb( "DST_SPEED", 5100 , 4040 , 5100)
	SetDstProb( "DST_ATTACKSPEED", 5800 , 4480, 5800)
	SetDstProb( "DST_ADJDEF", 6500 , 5880, 6500)
	SetDstProb( "DST_ATKPOWER", 7200 , 6320, 7200)
	SetDstProb( "DST_HP_MAX", 7900 , 7720, 7900)
	SetDstProb( "DST_MP_MAX", 8600 , 8160, 8600)
	SetDstProb( "DST_FP_MAX", 9300 , 9560, 9300)
	SetDstProb( "DST_SPELL_RATE", 10000 , 10000 , 10000)
--}
AddRandomOptionExtension( "IK1_ARMOR", "IK2_ARMORETC", "IK3_HELMET" )
--{
	SetRandomOptionMaterial( "IK1_NONE", "IK2_NONE", "IK3_NONE"  )

	SetRandomOptionGeneratePenya( 100000 )

	SetRandomOptionSizeProb( 1, 10000 )
	SetRandomOptionSizeProb( 2, 10000 )
	SetRandomOptionSizeProb( 3, 10000 )
	SetRandomOptionSizeProb( 4, 10000 )
	SetRandomOptionSizeProb( 5, 10000 )

	SetDummyRandomOptionSize( 20 )

	SetDstProb( "DST_STR", 1666,1333,1666 )
	SetDstProb( "DST_DEX", 3332,2666,3332 )
	SetDstProb( "DST_INT", 4998,3999,4998)
	SetDstProb( "DST_STA", 6664,5332,6664 )
	SetDstProb( "DST_MP_MAX", 8330,6665,8330 )
	SetDstProb( "DST_FP_MAX", 10000,10000,10000 )
--}

AddRandomOptionExtension( "IK1_ARMOR", "IK2_ARMOR", "IK3_SUIT" )
--{
	SetRandomOptionMaterial( "IK1_NONE", "IK2_NONE", "IK3_NONE"  )

	SetRandomOptionGeneratePenya( 100000 )

	SetRandomOptionSizeProb( 1, 10000 )
	SetRandomOptionSizeProb( 2, 10000 )
	SetRandomOptionSizeProb( 3, 10000 )
	SetRandomOptionSizeProb( 4, 10000 )
	SetRandomOptionSizeProb( 5, 10000 )

	SetDummyRandomOptionSize( 20 )

	SetDstProb( "DST_STR", 700, 440, 700 )
	SetDstProb( "DST_DEX", 1400 , 880, 1400)
	SetDstProb( "DST_INT", 2100 , 1320, 2100)
	SetDstProb( "DST_STA", 2800 , 1760, 2800)
	SetDstProb( "DST_CHR_CHANCECRITICAL", 3600 , 2200, 3600)
	SetDstProb( "DST_CRITICAL_BONUS", 4400 ,2640 , 4400)
	SetDstProb( "DST_SPEED", 5100 , 4040 , 5100)
	SetDstProb( "DST_ATTACKSPEED", 5800 , 4480, 5800)
	SetDstProb( "DST_ADJDEF", 6500 , 5880, 6500)
	SetDstProb( "DST_ATKPOWER", 7200 , 6320, 7200)
	SetDstProb( "DST_HP_MAX", 7900 , 7720, 7900)
	SetDstProb( "DST_MP_MAX", 8600 , 8160, 8600)
	SetDstProb( "DST_FP_MAX", 9300 , 9560, 9300)
	SetDstProb( "DST_SPELL_RATE", 10000 , 10000 , 10000)
--}

AddRandomOptionExtension( "IK1_ARMOR", "IK2_ARMORETC", "IK3_GAUNTLET" )
--{
	SetRandomOptionMaterial( "IK1_NONE", "IK2_NONE", "IK3_NONE"  )

	SetRandomOptionGeneratePenya( 100000 )

	SetRandomOptionSizeProb( 1, 10000 )
	SetRandomOptionSizeProb( 2, 10000 )
	SetRandomOptionSizeProb( 3, 10000 )
	SetRandomOptionSizeProb( 4, 10000 )
	SetRandomOptionSizeProb( 5, 10000 )

	SetDummyRandomOptionSize( 20 )

	SetDstProb( "DST_CHR_CHANCECRITICAL", 1700, 1333, 1700 )
	SetDstProb( "DST_ATTACKSPEED", 3360, 2666, 3360 )
	SetDstProb( "DST_ATKPOWER", 5020 ,3999, 5020 )
	SetDstProb( "DST_MP_MAX", 6680 , 5332, 6680)
	SetDstProb( "DST_FP_MAX", 8340 , 8832, 8340)
	SetDstProb( "DST_SPELL_RATE", 10000 , 10000, 10000)
--}

AddRandomOptionExtension( "IK1_ARMOR", "IK2_ARMORETC", "IK3_BOOTS" )
--{
	SetRandomOptionMaterial( "IK1_NONE", "IK2_NONE", "IK3_NONE"  )

	SetRandomOptionGeneratePenya( 100000 )

	SetRandomOptionSizeProb( 1, 10000 )
	SetRandomOptionSizeProb( 2, 10000 )
	SetRandomOptionSizeProb( 3, 10000 )
	SetRandomOptionSizeProb( 4, 10000 )
	SetRandomOptionSizeProb( 5, 10000 )

	SetDummyRandomOptionSize( 20 )

	SetDstProb( "DST_CRITICAL_BONUS", 1700,833,1700 )
	SetDstProb( "DST_SPEED", 3360,1666,3360 )
	SetDstProb( "DST_ADJDEF", 5020,4166,5020 )
	SetDstProb( "DST_HP_MAX", 6680,6666,6680 )
	SetDstProb( "DST_MP_MAX", 8340,7499,8340 )
	SetDstProb( "DST_FP_MAX", 10000,10000,10000 )
--}


--------------------------------------------------------------------
-- �� �Ķ���͵��� ���� ��ġ�� Ȯ�� --------------------------------
--------------------------------------------------------------------

-- Ȯ�� 1/10��
AddDstParameter( "DST_STR" )
--{
	SetAdjValueProb( -28, 100000	,	230000000	,	200000000	)
	SetAdjValueProb( -25, 3038270	,	350259000	,	308549200	)
	SetAdjValueProb( -22, 9524381	,	433937800	,	386614800	)
	SetAdjValueProb( -19, 20572000	,	499326400	,	449438600	)
	SetAdjValueProb( -16, 37701630	,	553740800	,	503117400	)
	SetAdjValueProb( -13, 63346074	,	600839200	,	550699400	)
	SetAdjValueProb( -10, 101762741	,	642711800	,	593926600	)
	SetAdjValueProb( -7, 161466445	,	680665000	,	633887600	)
	SetAdjValueProb( -4, 263744223	,	715570000	,	671308400	)
	SetAdjValueProb( -1, 499999999	,	748036200	,	706697000	)
	SetAdjValueProb( 1, 736255775	,	778507200	,	740422800	)
	SetAdjValueProb( 4, 838533553	,	807315400	,	772763000	)
	SetAdjValueProb( 7, 898237257	,	834716600	,	803930800	)
	SetAdjValueProb( 10, 936653924	,	860912000	,	834093600	)
	SetAdjValueProb( 13, 962298368	,	886062200	,	863385400	)
	SetAdjValueProb( 16, 979427998	,	910297800	,	891915200	)
	SetAdjValueProb( 19, 990475617	,	933726600	,	919772400	)
	SetAdjValueProb( 22, 996961728	,	956438000	,	947032000	)
	SetAdjValueProb( 25, 999900000	,	978507800	,	973756800	)
	SetAdjValueProb( 28, 1000000000	,	1000000000	,	1000000000	)

--}

AddDstParameter( "DST_DEX" )
--{
	SetAdjValueProb( -28, 100000 	,	230000000	,	200000000	)
	SetAdjValueProb( -25, 3038270 	,	350259000	,	308549200	)
	SetAdjValueProb( -22, 9524381 	,	433937800	,	386614800	)
	SetAdjValueProb( -19, 20572000 	,	499326400	,	449438600	)
	SetAdjValueProb( -16, 37701630 	,	553740800	,	503117400	)
	SetAdjValueProb( -13, 63346074 	,	600839200	,	550699400	)
	SetAdjValueProb( -10, 101762741 	,	642711800	,	593926600	)
	SetAdjValueProb( -7, 161466445 	,	680665000	,	633887600	)
	SetAdjValueProb( -4, 263744223 	,	715570000	,	671308400	)
	SetAdjValueProb( -1, 499999999 	,	748036200	,	706697000	)
	SetAdjValueProb( 1, 736255775 	,	778507200	,	740422800	)
	SetAdjValueProb( 4, 838533553 	,	807315400	,	772763000	)
	SetAdjValueProb( 7, 898237257 	,	834716600	,	803930800	)
	SetAdjValueProb( 10, 936653924 	,	860912000	,	834093600	)
	SetAdjValueProb( 13, 962298368 	,	886062200	,	863385400	)
	SetAdjValueProb( 16, 979427998 	,	910297800	,	891915200	)
	SetAdjValueProb( 19, 990475617 	,	933726600	,	919772400	)
	SetAdjValueProb( 22, 996961728 	,	956438000	,	947032000	)
	SetAdjValueProb( 25, 999900000 	,	978507800	,	973756800	)
	SetAdjValueProb( 28, 1000000000 	,	1000000000	,	1000000000	)

--}

AddDstParameter( "DST_INT" )
--{
	SetAdjValueProb( -28, 100000 	,	230000000	,	200000000	)
	SetAdjValueProb( -25, 3038270 	,	350259000	,	308549200	)
	SetAdjValueProb( -22, 9524381 	,	433937800	,	386614800	)
	SetAdjValueProb( -19, 20572000 	,	499326400	,	449438600	)
	SetAdjValueProb( -16, 37701630 	,	553740800	,	503117400	)
	SetAdjValueProb( -13, 63346074 	,	600839200	,	550699400	)
	SetAdjValueProb( -10, 101762741 	,	642711800	,	593926600	)
	SetAdjValueProb( -7, 161466445 	,	680665000	,	633887600	)
	SetAdjValueProb( -4, 263744223 	,	715570000	,	671308400	)
	SetAdjValueProb( -1, 499999999 	,	748036200	,	706697000	)
	SetAdjValueProb( 1, 736255775 	,	778507200	,	740422800	)
	SetAdjValueProb( 4, 838533553 	,	807315400	,	772763000	)
	SetAdjValueProb( 7, 898237257 	,	834716600	,	803930800	)
	SetAdjValueProb( 10, 936653924 	,	860912000	,	834093600	)
	SetAdjValueProb( 13, 962298368 	,	886062200	,	863385400	)
	SetAdjValueProb( 16, 979427998 	,	910297800	,	891915200	)
	SetAdjValueProb( 19, 990475617 	,	933726600	,	919772400	)
	SetAdjValueProb( 22, 996961728 	,	956438000	,	947032000	)
	SetAdjValueProb( 25, 999900000 	,	978507800	,	973756800	)
	SetAdjValueProb( 28, 1000000000 	,	1000000000	,	1000000000	)
--}

AddDstParameter( "DST_STA" )
--{
	SetAdjValueProb( -28, 100000 	,	230000000	,	200000000	)
	SetAdjValueProb( -25, 3038270 	,	350259000	,	308549200	)
	SetAdjValueProb( -22, 9524381 	,	433937800	,	386614800	)
	SetAdjValueProb( -19, 20572000 	,	499326400	,	449438600	)
	SetAdjValueProb( -16, 37701630 	,	553740800	,	503117400	)
	SetAdjValueProb( -13, 63346074 	,	600839200	,	550699400	)
	SetAdjValueProb( -10, 101762741 	,	642711800	,	593926600	)
	SetAdjValueProb( -7, 161466445 	,	680665000	,	633887600	)
	SetAdjValueProb( -4, 263744223 	,	715570000	,	671308400	)
	SetAdjValueProb( -1, 499999999 	,	748036200	,	706697000	)
	SetAdjValueProb( 1, 736255775 	,	778507200	,	740422800	)
	SetAdjValueProb( 4, 838533553 	,	807315400	,	772763000	)
	SetAdjValueProb( 7, 898237257 	,	834716600	,	803930800	)
	SetAdjValueProb( 10, 936653924 	,	860912000	,	834093600	)
	SetAdjValueProb( 13, 962298368 	,	886062200	,	863385400	)
	SetAdjValueProb( 16, 979427998 	,	910297800	,	891915200	)
	SetAdjValueProb( 19, 990475617 	,	933726600	,	919772400	)
	SetAdjValueProb( 22, 996961728 	,	956438000	,	947032000	)
	SetAdjValueProb( 25, 999900000 	,	978507800	,	973756800	)
	SetAdjValueProb( 28, 1000000000 	,	1000000000	,	1000000000	)
--}

AddDstParameter( "DST_CHR_CHANCECRITICAL" )
--{
	SetAdjValueProb( -10, 100000 	,	230000000	,	200000000	)
	SetAdjValueProb( -9, 3038270 	,	350259000	,	308549200	)
	SetAdjValueProb( -8, 9524381 	,	433937800	,	386614800	)
	SetAdjValueProb( -7, 20572000 	,	499326400	,	449438600	)
	SetAdjValueProb( -6, 37701630 	,	553740800	,	503117400	)
	SetAdjValueProb( -5, 63346074 	,	600839200	,	550699400	)
	SetAdjValueProb( -4, 101762741 	,	642711800	,	593926600	)
	SetAdjValueProb( -3, 161466445 	,	680665000	,	633887600	)
	SetAdjValueProb( -2, 263744223 	,	715570000	,	671308400	)
	SetAdjValueProb( -1, 499999999 	,	748036200	,	706697000	)
	SetAdjValueProb( 1, 736255775 	,	778507200	,	740422800	)
	SetAdjValueProb( 2, 838533553 	,	807315400	,	772763000	)
	SetAdjValueProb( 3, 898237257 	,	834716600	,	803930800	)
	SetAdjValueProb( 4, 936653924 	,	860912000	,	834093600	)
	SetAdjValueProb( 5, 962298368 	,	886062200	,	863385400	)
	SetAdjValueProb( 6, 979427998 	,	910297800	,	891915200	)
	SetAdjValueProb( 7, 990475617 	,	933726600	,	919772400	)
	SetAdjValueProb( 8, 996961728 	,	956438000	,	947032000	)
	SetAdjValueProb( 9, 999900000 	,	978507800	,	973756800	)
	SetAdjValueProb( 10, 1000000000 	,	1000000000	,	1000000000	)
--}

AddDstParameter( "DST_CRITICAL_BONUS" )
--{
	SetAdjValueProb( -19, 100000 	,	230000000	,	200000000	)
	SetAdjValueProb( -17, 3038270 	,	350259000	,	308549200	)
	SetAdjValueProb( -15, 9524381 	,	433937800	,	386614800	)
	SetAdjValueProb( -13, 20572000 	,	499326400	,	449438600	)
	SetAdjValueProb( -11, 37701630 	,	553740800	,	503117400	)
	SetAdjValueProb( -9, 63346074 	,	600839200	,	550699400	)
	SetAdjValueProb( -7, 101762741 	,	642711800	,	593926600	)
	SetAdjValueProb( -5, 161466445 	,	680665000	,	633887600	)
	SetAdjValueProb( -3, 263744223 	,	715570000	,	671308400	)
	SetAdjValueProb( -1, 499999999 	,	748036200	,	706697000	)
	SetAdjValueProb( 1, 736255775 	,	778507200	,	740422800	)
	SetAdjValueProb( 3, 838533553 	,	807315400	,	772763000	)
	SetAdjValueProb( 5, 898237257 	,	834716600	,	803930800	)
	SetAdjValueProb( 7, 936653924 	,	860912000	,	834093600	)
	SetAdjValueProb( 9, 962298368 	,	886062200	,	863385400	)
	SetAdjValueProb( 11, 979427998 	,	910297800	,	891915200	)
	SetAdjValueProb( 13, 990475617 	,	933726600	,	919772400	)
	SetAdjValueProb( 15, 996961728 	,	956438000	,	947032000	)
	SetAdjValueProb( 17, 999900000 	,	978507800	,	973756800	)
	SetAdjValueProb( 19, 1000000000 	,	1000000000	,	1000000000	)
--}

AddDstParameter( "DST_SPEED" )
--{
	SetAdjValueProb( -10, 100000 	,	230000000	,	200000000	)
	SetAdjValueProb( -9, 3038270 	,	350259000	,	308549200	)
	SetAdjValueProb( -8, 9524381 	,	433937800	,	386614800	)
	SetAdjValueProb( -7, 20572000 	,	499326400	,	449438600	)
	SetAdjValueProb( -6, 37701630 	,	553740800	,	503117400	)
	SetAdjValueProb( -5, 63346074 	,	600839200	,	550699400	)
	SetAdjValueProb( -4, 101762741 	,	642711800	,	593926600	)
	SetAdjValueProb( -3, 161466445 	,	680665000	,	633887600	)
	SetAdjValueProb( -2, 263744223 	,	715570000	,	671308400	)
	SetAdjValueProb( -1, 499999999 	,	748036200	,	706697000	)
	SetAdjValueProb( 1, 736255775 	,	778507200	,	740422800	)
	SetAdjValueProb( 2, 838533553 	,	807315400	,	772763000	)
	SetAdjValueProb( 3, 898237257 	,	834716600	,	803930800	)
	SetAdjValueProb( 4, 936653924 	,	860912000	,	834093600	)
	SetAdjValueProb( 5, 962298368 	,	886062200	,	863385400	)
	SetAdjValueProb( 6, 979427998 	,	910297800	,	891915200	)
	SetAdjValueProb( 7, 990475617 	,	933726600	,	919772400	)
	SetAdjValueProb( 8, 996961728 	,	956438000	,	947032000	)
	SetAdjValueProb( 9, 999900000 	,	978507800	,	973756800	)
	SetAdjValueProb( 10, 1000000000 	,	1000000000	,	1000000000	)
--}

AddDstParameter( "DST_ATTACKSPEED" )
--{
	SetAdjValueProb( -380, 100000 	,	230000000	,	200000000	)
	SetAdjValueProb( -340, 3038270 	,	350259000	,	308549200	)
	SetAdjValueProb( -300, 9524381 	,	433937800	,	386614800	)
	SetAdjValueProb( -260, 20572000 	,	499326400	,	449438600	)
	SetAdjValueProb( -220, 37701630 	,	553740800	,	503117400	)
	SetAdjValueProb( -180, 63346074 	,	600839200	,	550699400	)
	SetAdjValueProb( -140, 101762741 	,	642711800	,	593926600	)
	SetAdjValueProb( -100, 161466445 	,	680665000	,	633887600	)
	SetAdjValueProb( -60, 263744223 	,	715570000	,	671308400	)
	SetAdjValueProb( -20, 499999999 	,	748036200	,	706697000	)
	SetAdjValueProb( 20, 736255775 	,	778507200	,	740422800	)
	SetAdjValueProb( 60, 838533553 	,	807315400	,	772763000	)
	SetAdjValueProb( 100, 898237257 	,	834716600	,	803930800	)
	SetAdjValueProb( 140, 936653924 	,	860912000	,	834093600	)
	SetAdjValueProb( 180, 962298368 	,	886062200	,	863385400	)
	SetAdjValueProb( 220, 979427998 	,	910297800	,	891915200	)
	SetAdjValueProb( 260, 990475617 	,	933726600	,	919772400	)
	SetAdjValueProb( 300, 996961728 	,	956438000	,	947032000	)
	SetAdjValueProb( 340, 999900000 	,	978507800	,	973756800	)
	SetAdjValueProb( 380, 1000000000 	,	1000000000	,	1000000000	)
--}

AddDstParameter( "DST_ADJDEF" )
--{
	SetAdjValueProb( -112, 100000 	,	230000000	,	200000000	)
	SetAdjValueProb( -100, 3038270 	,	350259000	,	308549200	)
	SetAdjValueProb( -88, 9524381 	,	433937800	,	386614800	)
	SetAdjValueProb( -76, 20572000 	,	499326400	,	449438600	)
	SetAdjValueProb( -64, 37701630 	,	553740800	,	503117400	)
	SetAdjValueProb( -52, 63346074 	,	600839200	,	550699400	)
	SetAdjValueProb( -40, 101762741 	,	642711800	,	593926600	)
	SetAdjValueProb( -28, 161466445 	,	680665000	,	633887600	)
	SetAdjValueProb( -16, 263744223 	,	715570000	,	671308400	)
	SetAdjValueProb( -4, 499999999 	,	748036200	,	706697000	)
	SetAdjValueProb( 4, 736255775 	,	778507200	,	740422800	)
	SetAdjValueProb( 16, 838533553 	,	807315400	,	772763000	)
	SetAdjValueProb( 28, 898237257 	,	834716600	,	803930800	)
	SetAdjValueProb( 40, 936653924 	,	860912000	,	834093600	)
	SetAdjValueProb( 52, 962298368 	,	886062200	,	863385400	)
	SetAdjValueProb( 64, 979427998 	,	910297800	,	891915200	)
	SetAdjValueProb( 76, 990475617 	,	933726600	,	919772400	)
	SetAdjValueProb( 88, 996961728 	,	956438000	,	947032000	)
	SetAdjValueProb( 100, 999900000 	,	978507800	,	973756800	)
	SetAdjValueProb( 112, 1000000000 	,	1000000000	,	1000000000	)
--}

AddDstParameter( "DST_ATKPOWER" )
--{
	SetAdjValueProb( -140, 100000 	,	230000000	,	200000000	)
	SetAdjValueProb( -125, 3038270 	,	350259000	,	308549200	)
	SetAdjValueProb( -110, 9524381 	,	433937800	,	386614800	)
	SetAdjValueProb( -95, 20572000 	,	499326400	,	449438600	)
	SetAdjValueProb( -80, 37701630 	,	553740800	,	503117400	)
	SetAdjValueProb( -65, 63346074 	,	600839200	,	550699400	)
	SetAdjValueProb( -50, 101762741 	,	642711800	,	593926600	)
	SetAdjValueProb( -35, 161466445 	,	680665000	,	633887600	)
	SetAdjValueProb( -20, 263744223 	,	715570000	,	671308400	)
	SetAdjValueProb( -5, 499999999 	,	748036200	,	706697000	)
	SetAdjValueProb( 5, 736255775 	,	778507200	,	740422800	)
	SetAdjValueProb( 20, 838533553 	,	807315400	,	772763000	)
	SetAdjValueProb( 35, 898237257 	,	834716600	,	803930800	)
	SetAdjValueProb( 50, 936653924 	,	860912000	,	834093600	)
	SetAdjValueProb( 65, 962298368 	,	886062200	,	863385400	)
	SetAdjValueProb( 80, 979427998 	,	910297800	,	891915200	)
	SetAdjValueProb( 95, 990475617 	,	933726600	,	919772400	)
	SetAdjValueProb( 110, 996961728 	,	956438000	,	947032000	)
	SetAdjValueProb( 125, 999900000 	,	978507800	,	973756800	)
	SetAdjValueProb( 140, 1000000000 	,	1000000000	,	1000000000	)
--}

AddDstParameter( "DST_HP_MAX" )
--{
	SetAdjValueProb( -500, 100000 	,	230000000	,	200000000	)
	SetAdjValueProb( -450, 3038270 	,	350259000	,	308549200	)
	SetAdjValueProb( -400, 9524381 	,	433937800	,	386614800	)
	SetAdjValueProb( -350, 20572000 	,	499326400	,	449438600	)
	SetAdjValueProb( -300, 37701630 	,	553740800	,	503117400	)
	SetAdjValueProb( -250, 63346074 	,	600839200	,	550699400	)
	SetAdjValueProb( -200, 101762741 	,	642711800	,	593926600	)
	SetAdjValueProb( -150, 161466445 	,	680665000	,	633887600	)
	SetAdjValueProb( -100, 263744223 	,	715570000	,	671308400	)
	SetAdjValueProb( -50, 499999999 	,	748036200	,	706697000	)
	SetAdjValueProb( 50, 736255775 	,	778507200	,	740422800	)
	SetAdjValueProb( 100, 838533553 	,	807315400	,	772763000	)
	SetAdjValueProb( 150, 898237257 	,	834716600	,	803930800	)
	SetAdjValueProb( 200, 936653924 	,	860912000	,	834093600	)
	SetAdjValueProb( 250, 962298368 	,	886062200	,	863385400	)
	SetAdjValueProb( 300, 979427998 	,	910297800	,	891915200	)
	SetAdjValueProb( 350, 990475617 	,	933726600	,	919772400	)
	SetAdjValueProb( 400, 996961728 	,	956438000	,	947032000	)
	SetAdjValueProb( 450, 999900000 	,	978507800	,	973756800	)
	SetAdjValueProb( 500, 1000000000 	,	1000000000	,	1000000000	)
--}

AddDstParameter( "DST_MP_MAX" )
--{
	SetAdjValueProb( -500, 100000 	,	230000000	,	200000000	)
	SetAdjValueProb( -450, 3038270 	,	350259000	,	308549200	)
	SetAdjValueProb( -400, 9524381 	,	433937800	,	386614800	)
	SetAdjValueProb( -350, 20572000 	,	499326400	,	449438600	)
	SetAdjValueProb( -300, 37701630 	,	553740800	,	503117400	)
	SetAdjValueProb( -250, 63346074 	,	600839200	,	550699400	)
	SetAdjValueProb( -200, 101762741 	,	642711800	,	593926600	)
	SetAdjValueProb( -150, 161466445 	,	680665000	,	633887600	)
	SetAdjValueProb( -100, 263744223 	,	715570000	,	671308400	)
	SetAdjValueProb( -50, 499999999 	,	748036200	,	706697000	)
	SetAdjValueProb( 50, 736255775 	,	778507200	,	740422800	)
	SetAdjValueProb( 100, 838533553 	,	807315400	,	772763000	)
	SetAdjValueProb( 150, 898237257 	,	834716600	,	803930800	)
	SetAdjValueProb( 200, 936653924 	,	860912000	,	834093600	)
	SetAdjValueProb( 250, 962298368 	,	886062200	,	863385400	)
	SetAdjValueProb( 300, 979427998 	,	910297800	,	891915200	)
	SetAdjValueProb( 350, 990475617 	,	933726600	,	919772400	)
	SetAdjValueProb( 400, 996961728 	,	956438000	,	947032000	)
	SetAdjValueProb( 450, 999900000 	,	978507800	,	973756800	)
	SetAdjValueProb( 500, 1000000000 	,	1000000000	,	1000000000	)
--}

AddDstParameter( "DST_FP_MAX" )
--{
	SetAdjValueProb( -500, 100000 	,	230000000	,	200000000	)
	SetAdjValueProb( -450, 3038270 	,	350259000	,	308549200	)
	SetAdjValueProb( -400, 9524381 	,	433937800	,	386614800	)
	SetAdjValueProb( -350, 20572000 	,	499326400	,	449438600	)
	SetAdjValueProb( -300, 37701630 	,	553740800	,	503117400	)
	SetAdjValueProb( -250, 63346074 	,	600839200	,	550699400	)
	SetAdjValueProb( -200, 101762741 	,	642711800	,	593926600	)
	SetAdjValueProb( -150, 161466445 	,	680665000	,	633887600	)
	SetAdjValueProb( -100, 263744223 	,	715570000	,	671308400	)
	SetAdjValueProb( -50, 499999999 	,	748036200	,	706697000	)
	SetAdjValueProb( 50, 736255775 	,	778507200	,	740422800	)
	SetAdjValueProb( 100, 838533553 	,	807315400	,	772763000	)
	SetAdjValueProb( 150, 898237257 	,	834716600	,	803930800	)
	SetAdjValueProb( 200, 936653924 	,	860912000	,	834093600	)
	SetAdjValueProb( 250, 962298368 	,	886062200	,	863385400	)
	SetAdjValueProb( 300, 979427998 	,	910297800	,	891915200	)
	SetAdjValueProb( 350, 990475617 	,	933726600	,	919772400	)
	SetAdjValueProb( 400, 996961728 	,	956438000	,	947032000	)
	SetAdjValueProb( 450, 999900000 	,	978507800	,	973756800	)
	SetAdjValueProb( 500, 1000000000 	,	1000000000	,	1000000000	)
--}

AddDstParameter( "DST_SPELL_RATE" )
--{
	SetAdjValueProb( -10, 100000 	,	230000000	,	200000000	)
	SetAdjValueProb( -9, 3038270 	,	350259000	,	308549200	)
	SetAdjValueProb( -8, 9524381 	,	433937800	,	386614800	)
	SetAdjValueProb( -7, 20572000 	,	499326400	,	449438600	)
	SetAdjValueProb( -6, 37701630 	,	553740800	,	503117400	)
	SetAdjValueProb( -5, 63346074 	,	600839200	,	550699400	)
	SetAdjValueProb( -4, 101762741 	,	642711800	,	593926600	)
	SetAdjValueProb( -3, 161466445 	,	680665000	,	633887600	)
	SetAdjValueProb( -2, 263744223 	,	715570000	,	671308400	)
	SetAdjValueProb( -1, 499999999 	,	748036200	,	706697000	)
	SetAdjValueProb( 1, 736255775 	,	778507200	,	740422800	)
	SetAdjValueProb( 2, 838533553 	,	807315400	,	772763000	)
	SetAdjValueProb( 3, 898237257 	,	834716600	,	803930800	)
	SetAdjValueProb( 4, 936653924 	,	860912000	,	834093600	)
	SetAdjValueProb( 5, 962298368 	,	886062200	,	863385400	)
	SetAdjValueProb( 6, 979427998 	,	910297800	,	891915200	)
	SetAdjValueProb( 7, 990475617 	,	933726600	,	919772400	)
	SetAdjValueProb( 8, 996961728 	,	956438000	,	947032000	)
	SetAdjValueProb( 9, 999900000 	,	978507800	,	973756800	)
	SetAdjValueProb( 10, 1000000000 	,	1000000000	,	1000000000	)
--}

--------------------------------------------------------------------
-- ���͸� ��ȯ -------------------------------------------------------
--------------------------------------------------------------------

AddUltimateTrans( "IK1_WEAPON", "IK2_NONE", "IK3_NONE" )
--{
	SetUltimateMaterial1( "IK1_PASSIVE", "IK2_MATERIAL", "IK3_OPERCID" )
	SetUltimateMaterial2( "IK1_PASSIVE", "IK2_MATERIAL", "IK3_OPERCID" )
	SetUltimatePenya ( 100000000 )
--}