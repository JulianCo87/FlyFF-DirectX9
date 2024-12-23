--------------------------------------------------------------------
-- �Ϲ� ���� -------------------------------------------------------
--------------------------------------------------------------------

tGeneralEnchant = {}

function AddGeneralEnchant( strItemKind1, strItemKind2, strItemKind3 )
	local nIndex = table.getn( tGeneralEnchant ) + 1

	tGeneralEnchant[nIndex] = {}
	tGeneralEnchant[nIndex].strItemKind1 = strItemKind1
	tGeneralEnchant[nIndex].strItemKind2 = strItemKind2
	tGeneralEnchant[nIndex].strItemKind3 = strItemKind3
	tGeneralEnchant[nIndex].strMaterialItemKind1 = 0
	tGeneralEnchant[nIndex].strMaterialItemKind2 = 0
	tGeneralEnchant[nIndex].strMaterialItemKind3 = 0
	tGeneralEnchant[nIndex].tEnchantProb = {}
end

function SetGeneralMaterial( strMaterialItemKind1, strMaterialItemKind2, strMaterialItemKind3 )
	local nIndex 	= table.getn( tGeneralEnchant )

	tGeneralEnchant[nIndex].strMaterialItemKind1 = strMaterialItemKind1
	tGeneralEnchant[nIndex].strMaterialItemKind2 = strMaterialItemKind2
	tGeneralEnchant[nIndex].strMaterialItemKind3 = strMaterialItemKind3
end

function SetGeneralEnchantProb( dwLevel, dwSuccessProb, dwFailProb )
	local nIndex 	= table.getn( tGeneralEnchant )

	tGeneralEnchant[nIndex].tEnchantProb[dwLevel] = {}
	tGeneralEnchant[nIndex].tEnchantProb[dwLevel].dwSuccessProb = dwSuccessProb
	tGeneralEnchant[nIndex].tEnchantProb[dwLevel].dwFailProb = dwFailProb
end


--------------------------------------------------------------------
-- �Ӽ� ���� -------------------------------------------------------
--------------------------------------------------------------------

tAttributeEnchant = {}

function AddAttributeEnchant( strItemKind1, strItemKind2, strItemKind3 )
	local nIndex = table.getn( tAttributeEnchant ) + 1

	tAttributeEnchant[nIndex] = {}
	tAttributeEnchant[nIndex].strItemKind1 = strItemKind1
	tAttributeEnchant[nIndex].strItemKind2 = strItemKind2
	tAttributeEnchant[nIndex].strItemKind3 = strItemKind3
	tAttributeEnchant[nIndex].strMaterialItemKind1 = 0
	tAttributeEnchant[nIndex].strMaterialItemKind2 = 0
	tAttributeEnchant[nIndex].strMaterialItemKind3 = 0
	tAttributeEnchant[nIndex].nAttributeRemovePenya = 0
	tAttributeEnchant[nIndex].tEnchantProb = {}
end

function SetAttributeMaterial( strMaterialItemKind1, strMaterialItemKind2, strMaterialItemKind3 )
	local nIndex 	= table.getn( tAttributeEnchant )

	tAttributeEnchant[nIndex].strMaterialItemKind1 = strMaterialItemKind1
	tAttributeEnchant[nIndex].strMaterialItemKind2 = strMaterialItemKind2
	tAttributeEnchant[nIndex].strMaterialItemKind3 = strMaterialItemKind3
end

function SetAttributeRemovePenya( nAttributeRemovePenya )
	local nIndex 	= table.getn( tAttributeEnchant )

	tAttributeEnchant[nIndex].nAttributeRemovePenya = nAttributeRemovePenya
end

function SetAttributeEnchantProb( dwLevel, dwSuccessProb, dwFailProb, dwAddDamageRate, dwAddDefenseRate, dwAddAtkDmgRate )
	local nIndex 	= table.getn( tAttributeEnchant )

	tAttributeEnchant[nIndex].tEnchantProb[dwLevel] = {}
	tAttributeEnchant[nIndex].tEnchantProb[dwLevel].dwSuccessProb = dwSuccessProb
	tAttributeEnchant[nIndex].tEnchantProb[dwLevel].dwFailProb = dwFailProb
	tAttributeEnchant[nIndex].tEnchantProb[dwLevel].dwAddDamageRate = dwAddDamageRate
	tAttributeEnchant[nIndex].tEnchantProb[dwLevel].dwAddDefenseRate = dwAddDefenseRate
	tAttributeEnchant[nIndex].tEnchantProb[dwLevel].dwAddAtkDmgRate = dwAddAtkDmgRate
end


--------------------------------------------------------------------
-- �Ǿ�� ----------------------------------------------------------
--------------------------------------------------------------------

tPiercing = {}

function AddPiercing( strItemKind1, strItemKind2, strItemKind3 )
	local nIndex = table.getn( tPiercing ) + 1

	tPiercing[nIndex] = {}
	tPiercing[nIndex].strItemKind1 = strItemKind1
	tPiercing[nIndex].strItemKind2 = strItemKind2
	tPiercing[nIndex].strItemKind3 = strItemKind3
	tPiercing[nIndex].strMaterialItemKind1 = 0
	tPiercing[nIndex].strMaterialItemKind2 = 0
	tPiercing[nIndex].strMaterialItemKind3 = 0
	tPiercing[nIndex].strPiercedItemKind1 = 0
	tPiercing[nIndex].strPiercedItemKind2 = 0
	tPiercing[nIndex].strPiercedItemKind3 = 0
	tPiercing[nIndex].nSizeIncreasePenya = 0
	tPiercing[nIndex].nItemRemovePenya = 0
	tPiercing[nIndex].tPiercingSizeProb = {}
end

function SetPiercingMaterial( strMaterialItemKind1, strMaterialItemKind2, strMaterialItemKind3 )
	local nIndex 	= table.getn( tPiercing )

	tPiercing[nIndex].strMaterialItemKind1 = strMaterialItemKind1
	tPiercing[nIndex].strMaterialItemKind2 = strMaterialItemKind2
	tPiercing[nIndex].strMaterialItemKind3 = strMaterialItemKind3
end

function SetPiercedItem( strPiercedItemKind1, strPiercedItemKind2, strPiercedItemKind3 )
	local nIndex 	= table.getn( tPiercing )

	tPiercing[nIndex].strPiercedItemKind1 = strPiercedItemKind1
	tPiercing[nIndex].strPiercedItemKind2 = strPiercedItemKind2
	tPiercing[nIndex].strPiercedItemKind3 = strPiercedItemKind3
end

function SetPiercingSizeIncreasePenya( nSizeIncreasePenya )
	local nIndex 	= table.getn( tPiercing )

	tPiercing[nIndex].nSizeIncreasePenya = nSizeIncreasePenya
end

function SetPiercedItemRemovePenya( nItemRemovePenya )
	local nIndex 	= table.getn( tPiercing )

	tPiercing[nIndex].nItemRemovePenya = nItemRemovePenya
end

function SetPiercingSizeProb( dwSize, dwProb )
	local nIndex 	= table.getn( tPiercing )

	tPiercing[nIndex].tPiercingSizeProb[dwSize] = {}
	tPiercing[nIndex].tPiercingSizeProb[dwSize].dwProb = dwProb
end


--------------------------------------------------------------------
-- ���� ------------------------------------------------------------
--------------------------------------------------------------------

tRandomOptionExtension = {}

function AddRandomOptionExtension( strItemKind1, strItemKind2, strItemKind3 )
	local nIndex = table.getn( tRandomOptionExtension ) + 1

	tRandomOptionExtension[nIndex] = {}
	tRandomOptionExtension[nIndex].strItemKind1 = strItemKind1
	tRandomOptionExtension[nIndex].strItemKind2 = strItemKind2
	tRandomOptionExtension[nIndex].strItemKind3 = strItemKind3
	tRandomOptionExtension[nIndex].strMaterialItemKind1 = 0
	tRandomOptionExtension[nIndex].strMaterialItemKind2 = 0
	tRandomOptionExtension[nIndex].strMaterialItemKind3 = 0
	tRandomOptionExtension[nIndex].dwDummyRandomOptionSize = 0
	tRandomOptionExtension[nIndex].nRandomOptionGeneratePenya = 0
	tRandomOptionExtension[nIndex].tRandomOptionSizeProb = {}
	tRandomOptionExtension[nIndex].tDstProb = {}
end

function SetRandomOptionMaterial( strMaterialItemKind1, strMaterialItemKind2, strMaterialItemKind3 )
	local nIndex 	= table.getn( tRandomOptionExtension )

	tRandomOptionExtension[nIndex].strMaterialItemKind1 = strMaterialItemKind1
	tRandomOptionExtension[nIndex].strMaterialItemKind2 = strMaterialItemKind2
	tRandomOptionExtension[nIndex].strMaterialItemKind3 = strMaterialItemKind3
end

function SetRandomOptionGeneratePenya( nRandomOptionGeneratePenya )
	local nIndex 	= table.getn( tRandomOptionExtension )

	tRandomOptionExtension[nIndex].nRandomOptionGeneratePenya = nRandomOptionGeneratePenya
end

function SetRandomOptionSizeProb( dwSize, dwProb )
	local nIndex 	= table.getn( tRandomOptionExtension )

	tRandomOptionExtension[nIndex].tRandomOptionSizeProb[dwSize] = {}
	tRandomOptionExtension[nIndex].tRandomOptionSizeProb[dwSize].dwProb = dwProb
end

function SetDummyRandomOptionSize( dwDummyRandomOptionSize )
	local nIndex 	= table.getn( tRandomOptionExtension )

	tRandomOptionExtension[nIndex].dwDummyRandomOptionSize = dwDummyRandomOptionSize
end

function SetDstProb( strDst, dwProb, dwRetryProb, dwDummyProb )
	local nGIndex 	= table.getn( tRandomOptionExtension )
	local nDIndex	= table.getn( tRandomOptionExtension[nGIndex].tDstProb ) + 1

	tRandomOptionExtension[nGIndex].tDstProb[nDIndex] = {}
	tRandomOptionExtension[nGIndex].tDstProb[nDIndex].strDst = strDst
	tRandomOptionExtension[nGIndex].tDstProb[nDIndex].dwProb = dwProb
	tRandomOptionExtension[nGIndex].tDstProb[nDIndex].dwRetryProb = dwRetryProb
	tRandomOptionExtension[nGIndex].tDstProb[nDIndex].dwDummyProb = dwDummyProb
end


tDstParameter = {}

function AddDstParameter( strDst )
	local nIndex = table.getn( tDstParameter ) +1

	tDstParameter[nIndex] = {}
	tDstParameter[nIndex].strDst = strDst
	tDstParameter[nIndex].tAdjProb = {}
end

function SetAdjValueProb( nAdjValue, dwProb, dwRetryProb, dwDummyProb )
	local nDIndex 	= table.getn( tDstParameter )
	local nAIndex	= table.getn( tDstParameter[nDIndex].tAdjProb ) + 1

	tDstParameter[nDIndex].tAdjProb[nAIndex] = {}
	tDstParameter[nDIndex].tAdjProb[nAIndex].nAdjValue = nAdjValue
	tDstParameter[nDIndex].tAdjProb[nAIndex].dwProb = dwProb
	tDstParameter[nDIndex].tAdjProb[nAIndex].dwRetryProb = dwRetryProb
	tDstParameter[nDIndex].tAdjProb[nAIndex].dwDummyProb = dwDummyProb
end


--------------------------------------------------------------------
-- �ռ� ------------------------------------------------------------
--------------------------------------------------------------------

ENCHANT_GENERAL			= 1		-- �Ϲ� ����
PIERCING_GENERAL		= 8		-- �Ϲ� �Ǿ��
RANDOM_OPTION_ORIGIN		= 16		-- ���� �ɼ�( ex. ���� ��ø.. )
RANDOM_OPTION_EXTENSION		= 32		-- ���� �� �ູ


tCombine= {}

function AddCombine( strItemKind1, strItemKind2, strItemKind3 )
	local nIndex = table.getn( tCombine ) + 1

	tCombine[nIndex] = {}
	tCombine[nIndex].strItemKind1 = strItemKind1
	tCombine[nIndex].strItemKind2 = strItemKind2
	tCombine[nIndex].strItemKind3 = strItemKind3
	tCombine[nIndex].dwCombineOption = 0
	tCombine[nIndex].dwSuccessProb = 0
	tCombine[nIndex].nInitializePenya = 0
end

function SetCombineOption( ... )
	local nIndex = table.getn( tCombine )

	local dwOption = 0
	if( arg.n > 0 ) then
		for i = 1, arg.n do
			dwOption = dwOption + arg[i]
		end
	end

	tCombine[nIndex].dwCombineOption = dwOption
end

function SetCombineSuccessProb( dwSuccessProb )
	local nIndex = table.getn( tCombine )

	tCombine[nIndex].dwSuccessProb = dwSuccessProb
end

function SetCombineInitializePenya( nInitializePenya )
	local nIndex = table.getn( tCombine )

	tCombine[nIndex].nInitializePenya = nInitializePenya
end

--------------------------------------------------------------------
-- ī���ռ� --------------------------------------------------------
--------------------------------------------------------------------

tCardCombine= {}

function AddCardCombine(dwItemIndex, dwSuccessProb)
	local nIndex = table.getn( tCardCombine ) + 1

	tCardCombine[nIndex] = {}
	tCardCombine[nIndex].dwItemIndex = dwItemIndex
	tCardCombine[nIndex].dwSuccessProb = dwSuccessProb
end




--------------------------------------------------------------------
-- ���͸� ��ȯ -----------------------------------------------------
--------------------------------------------------------------------

tUltimate = {}

function AddUltimateTrans( strItemKind1, strItemKind2, strItemKind3 )
	local nIndex = table.getn( tUltimate ) + 1

	tUltimate[nIndex] = {}
	tUltimate[nIndex].strItemKind1 = strItemKind1
	tUltimate[nIndex].strItemKind2 = strItemKind2
	tUltimate[nIndex].strItemKind3 = strItemKind3
	tUltimate[nIndex].strMaterialItemKind1_1 = 0
	tUltimate[nIndex].strMaterialItemKind1_2 = 0
	tUltimate[nIndex].strMaterialItemKind1_3 = 0
	tUltimate[nIndex].strMaterialItemKind2_1 = 0
	tUltimate[nIndex].strMaterialItemKind2_2 = 0
	tUltimate[nIndex].strMaterialItemKind2_3 = 0
	tUltimate[nIndex].nInitializePenya = 0

end

function SetUltimateMaterial1( strMaterialItemKind1_1, strMaterialItemKind1_2, strMaterialItemKind1_3 )
	local nIndex 	= table.getn( tUltimate )

	tUltimate[nIndex].strMaterialItemKind1_1 = strMaterialItemKind1_1
	tUltimate[nIndex].strMaterialItemKind1_2 = strMaterialItemKind1_2
	tUltimate[nIndex].strMaterialItemKind1_3 = strMaterialItemKind1_3
end

function SetUltimateMaterial2( strMaterialItemKind2_1, strMaterialItemKind2_2, strMaterialItemKind2_3 )
	local nIndex 	= table.getn( tUltimate )

	tUltimate[nIndex].strMaterialItemKind2_1 = strMaterialItemKind2_1
	tUltimate[nIndex].strMaterialItemKind2_2 = strMaterialItemKind2_2
	tUltimate[nIndex].strMaterialItemKind2_3 = strMaterialItemKind2_3
end

function SetUltimatePenya( nInitializePenya )
	local nIndex = table.getn( tUltimate )

	tUltimate[nIndex].nInitializePenya = nInitializePenya
end
