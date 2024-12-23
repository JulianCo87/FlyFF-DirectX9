--------------------------------------------------------------------
-- �ʱ�ȭ ----------------------------------------------------------
--------------------------------------------------------------------
dofile( ".\\LuaFunc\\ItemUpgradeFunc.lua" )
--------------------------------------------------------------------

----------------------------------------------------------
-- �Ǿ�� Ȯ��( n/10000 ) --------------------------------
----------------------------------------------------------
tSuitProb = { 8000, 5000, 2000, 500 }
tWeaponProb = { 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000 }


----------------------------------------------------------
-- �Ϲ����� Ȯ�� -----------------------------------------
----------------------------------------------------------
-- �ؿܴ� +4���� �Ʒ� ������ 10% ���ҵ� ���� ����ȴ�. ( n * 0.9 )
tGeneral = { 10000, 10000, 7000, 5000, 4000, 3000, 2000, 1000, 500, 200 };

----------------------------------------------------------
-- �ڽ�Ƭ ���� Ȯ�� -----------------------------------------
----------------------------------------------------------
tCostumeEnhance = { 10000, 10000, 7000, 5000, 4000, 3000, 2000, 1000, 500, 200 };


----------------------------------------------------------
-- ������ Ʈ���� ----------------------------------------------
----------------------------------------------------------
nItemTransyLowLevel	= 500000
nItemTransyHighLevel	= 2000000


----------------------------------------------------------
-- �Ӽ����� ----------------------------------------------
----------------------------------------------------------
--		���ð�,		Ȯ��,		������������,		����������,		�Ӽ�����������  (���ð��� ������ ��簪�� n/10000)
AddAttribute(	1,		10000,		200,			200,			500		)
AddAttribute(	2,		10000,		221,			221,			522		)
AddAttribute(	3,		9200,		256,			256,			560		)
AddAttribute(	4,		7000,		305,			305,			612		)
AddAttribute(	5,		5300,		368,			368,			680		)
AddAttribute(	6,		4000,		446,			446,			763		)
AddAttribute(	7,		3000,		537,			537,			860		)
AddAttribute(	8,		2300,		642,			642,			973		)
AddAttribute(	9,		1700,		761,			761,			1101		)
AddAttribute(	10,		1300,		895,			895,			1244		)
AddAttribute(	11,		1000,		1042,			1042,			1402		)
AddAttribute(	12,		700,		1203,			1203,			1575		)
AddAttribute(	13,		600,		1379,			1379,			1763		)
AddAttribute(	14,		400,		1568,			1568,			1966		)
AddAttribute(	15,		300,		1772,			1772,			2184		)
AddAttribute(	16,		250,		1989,			1989,			2417		)
AddAttribute(	17,		200,		2221,			2221,			2665		)
AddAttribute(	18,		150,		2467,			2467,			2998		)
AddAttribute(	19,		120,		2726,			2726,			3207		)
AddAttribute(	20,		100,		3000,			3000,			3500		)

----------------------------------------------------------
-- �ٷ糪 ���� -------------------------------------------
----------------------------------------------------------
tBarunaNPCLevel = {}
function AddBarunaLevel( dwNum, dwExpRequired, dwAdditionalSuccessProp )
	tBarunaNPCLevel[dwNum] 				= {}
	tBarunaNPCLevel[dwNum].dwExpRequired		= dwExpRequired
	tBarunaNPCLevel[dwNum].dwAdditionalSuccessProp  = dwAdditionalSuccessProp
end
--			����,	���� ����ġ,	�߰� ���� ������% (%���� n/10000)
AddBarunaLevel( 	1,	100,		50	)	
AddBarunaLevel(		2,	350,		100	)
AddBarunaLevel(		3,	900,		250	)
AddBarunaLevel(		4,	2050,		400	)
AddBarunaLevel(		5,	4400,		500	)



tBarunaWeaponEnchant = {}
function AddBarunaWeaponEnchant( dwNum, dwSuccessAdditionalExp, dwFailAddtionalExp, dwMinusAdditionalExp, dwSuccessProb, dwFailProb )
	tBarunaWeaponEnchant[dwNum] = {}
	tBarunaWeaponEnchant[dwNum].dwSuccessAdditionalExp	= dwSuccessAdditionalExp
	tBarunaWeaponEnchant[dwNum].dwFailAddtionalExp		= dwFailAddtionalExp
	tBarunaWeaponEnchant[dwNum].dwMinusAdditionalExp	= dwMinusAdditionalExp
	tBarunaWeaponEnchant[dwNum].dwSuccessProb	= dwSuccessProb
	tBarunaWeaponEnchant[dwNum].dwFailProb		= dwFailProb
end

-- �׸� ���� : ���� ����ġ ( ������ �߰� NPC ����ġ ) 	���� ����ġ ( ���н� �߰� NPC ����ġ )	���̳ʽ� ����ġ ( ���̳ʽ��� �߰� NPC ����ġ )
--			���ð�,	���� ����ġ, 	���� ����ġ, 	���̳ʽ� ����ġ,	���� Ȯ��%,	����/�ı�Ȯ��%,	���̳ʽ� 1 ���ð� Ȯ��%( 10000 - ���� Ȯ�� - ���� Ȯ�� ) (%���� n/10000)
AddBarunaWeaponEnchant( 1, 	1,		0,		0,			10000,		0	)	-- auto --	
AddBarunaWeaponEnchant( 2, 	1,		0,		0,			10000,		0	)	-- auto --	
AddBarunaWeaponEnchant( 3, 	4,		1,		1,			9000,		750	)	-- auto --	
AddBarunaWeaponEnchant( 4, 	6,		1,		1,			8500,		1125	)	-- auto --	
AddBarunaWeaponEnchant( 5, 	8,		1,		1,			8000,		1500	)	-- auto --		
AddBarunaWeaponEnchant( 6, 	12,		2,		2,			7000,		2250	)	-- auto --		
AddBarunaWeaponEnchant( 7, 	16,		2,		2,			6000,		3000	)	-- auto --	
AddBarunaWeaponEnchant( 8, 	20,		2,		2,			5000,		3750	)	-- auto --		
AddBarunaWeaponEnchant( 9, 	24,		2,		2,			4000,		4500	)	-- auto --		
AddBarunaWeaponEnchant( 10, 	28,		2,		2,			3000,		5250	)	-- auto --		
AddBarunaWeaponEnchant( 11, 	32,		3,		3,			2000,		6000	)	-- auto --		
AddBarunaWeaponEnchant( 12,	36,		3,		3, 			1000,		6750	)	-- auto --		
AddBarunaWeaponEnchant( 13,	37,		3,		3, 			750,		6937	)	-- auto --		
AddBarunaWeaponEnchant( 14,	38,		3,		3, 			500,		7125	)	-- auto --		
AddBarunaWeaponEnchant( 15,	39,		3,		3, 			350,		7237	)	-- auto --		
AddBarunaWeaponEnchant( 16, 	40,		4,		4,			200,		7350	)	-- auto --		
AddBarunaWeaponEnchant( 17, 	41,		4,		4,			150,		7387	)	-- auto --		
AddBarunaWeaponEnchant( 18, 	42,		4,		4,			100,		7425	)	-- auto --		
AddBarunaWeaponEnchant( 19, 	43,		4,		4,			75,		7444	)	-- auto --		
AddBarunaWeaponEnchant( 20, 	44,		4,		4,			50,		7462	)	-- auto --		




tBarunaArmorEnchant = {}
function AddBarunaArmorEnchant( dwNum, dwSuccessAdditionalExp, dwFailAddtionalExp, dwMinusAdditionalExp, dwSuccessProb, dwFailProb )
	tBarunaArmorEnchant[dwNum] = {}
	tBarunaArmorEnchant[dwNum].dwSuccessAdditionalExp= dwSuccessAdditionalExp
	tBarunaArmorEnchant[dwNum].dwFailAddtionalExp	= dwFailAddtionalExp
	tBarunaArmorEnchant[dwNum].dwMinusAdditionalExp	= dwMinusAdditionalExp
	tBarunaArmorEnchant[dwNum].dwSuccessProb = dwSuccessProb
	tBarunaArmorEnchant[dwNum].dwFailProb 	= dwFailProb
end

-- �׸� ���� : ���� ����ġ ( ������ �߰� NPC ����ġ ) 	���� ����ġ ( ���н� �߰� NPC ����ġ )	���̳ʽ� ����ġ ( ���̳ʽ��� �߰� NPC ����ġ )
--			���ð�,	���� ����ġ, 	���� ����ġ, 	���̳ʽ� ����ġ,	���� Ȯ��%,	����/�ı�Ȯ��%,	���̳ʽ�1 ���ð� Ȯ��%( 10000 - ���� Ȯ�� - ���� Ȯ�� )	(%���� n/10000)
AddBarunaArmorEnchant( 	1, 	1,		0,		0,			10000,		0	)	-- auto --	
AddBarunaArmorEnchant( 	2, 	1,		0,		0,			10000,		0	)	-- auto --	
AddBarunaArmorEnchant( 	3, 	2,		1,		1,			9500,		375	)	-- auto --	
AddBarunaArmorEnchant( 	4, 	4,		1,		1,			9000,		750	)	-- auto --	
AddBarunaArmorEnchant( 	5, 	6,		1,		1,			8500,		1125	)	-- auto --
AddBarunaArmorEnchant( 	6, 	9,		1,		1,			7750,		1687	)	-- auto --
AddBarunaArmorEnchant( 	7, 	12,		2,		2,			7000,		2250	)	-- auto --
AddBarunaArmorEnchant( 	8, 	15,		2,		2,			6250,		2812	)	-- auto --
AddBarunaArmorEnchant( 	9, 	18,		2,		2,			5500,		3375	)	-- auto --
AddBarunaArmorEnchant( 	10, 	21,		2,		2,			4750,		3937	)	-- auto --
AddBarunaArmorEnchant( 	11, 	24,		2,		2,			4000,		4500	)	-- auto --
AddBarunaArmorEnchant( 	12,	27,		2,		2, 			3250,		5062	)	-- auto --
AddBarunaArmorEnchant( 	13,	31,		3,		3, 			2250,		5812	)	-- auto --
AddBarunaArmorEnchant( 	14,	34,		3,		3, 			1250,		6562	)	-- auto --
AddBarunaArmorEnchant( 	15,	36,		3,		3, 			1000,		6750	)	-- auto --
AddBarunaArmorEnchant( 	16, 	37,		3,		3,			750,		6937	)	-- auto --	
AddBarunaArmorEnchant( 	17, 	38,		4,		4,			500,		7125	)	-- auto --	
AddBarunaArmorEnchant( 	18, 	40,		4,		4,			300,		7275	)	-- auto --		
AddBarunaArmorEnchant( 	19, 	41,		4,		4,			150,		7387	)	-- auto --		
AddBarunaArmorEnchant( 	20, 	42,		4,		4,			100,		7425	)	-- auto --		

