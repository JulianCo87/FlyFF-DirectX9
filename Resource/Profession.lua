nMaxProfession = 0
nMaxProfessionLevel = 10
tProfessionExp = { 100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600, 51200 }

tProfessionCraft = {}

function AddProfessionCraft( nCategorie, nLevel, nExp, strRewardId, nRewardNum, strId, strId2, strId3, nNum, nNum2, nNum3, nPercent, nPenya )
	local nIndex = #tProfessionCraft + 1
	tProfessionCraft[nIndex] = {}
	tProfessionCraft[nIndex].nCategorie = nCategorie
	tProfessionCraft[nIndex].nLevel = nLevel
	tProfessionCraft[nIndex].nExp = nExp
	tProfessionCraft[nIndex].strRewardId = strRewardId
	tProfessionCraft[nIndex].nRewardNum = nRewardNum
	tProfessionCraft[nIndex].strId = strId
	tProfessionCraft[nIndex].strId2 = strId2
	tProfessionCraft[nIndex].strId3 = strId3
	tProfessionCraft[nIndex].nNum = nNum
	tProfessionCraft[nIndex].nNum2 = nNum2
	tProfessionCraft[nIndex].nNum3 = nNum3
	tProfessionCraft[nIndex].nPercent = nPercent
	tProfessionCraft[nIndex].nPenya = nPenya
end

AddProfessionCraft( 0, 1, 10, "II_WEA_AXE_RODNEY", 1, "II_WEA_AXE_MORROW", "II_WEA_AXE_CURNING", "II_WEA_AXE_SHYADIN", 1, 1, 1, 100.00, 500000000 )
AddProfessionCraft( 0, 1, 10, "II_WEA_AXE_RODNEY", 1, "II_WEA_AXE_MORROW", "II_WEA_AXE_CURNING", "II_WEA_AXE_SHYADIN", 1, 1, 1, 100.00, 500000000 )
AddProfessionCraft( 0, 1, 10, "II_WEA_AXE_RODNEY", 1, "II_WEA_AXE_MORROW", "II_WEA_AXE_CURNING", "II_WEA_AXE_SHYADIN", 1, 1, 1, 100.00, 500000000 )











