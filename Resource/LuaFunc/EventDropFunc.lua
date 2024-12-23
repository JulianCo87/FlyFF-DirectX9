tEventDrop = {}   -- �̺�Ʈ ���� ���̺�


-- �ð��뺰 ������ ��� ����ġ
tHour = { 505, 409, 324, 280, 220, 203, 202, 212,
	  227, 261, 302, 349, 571, 701, 764, 803,
	  790, 789, 754, 849, 936, 940, 919, 720 }


---------------------------------------------------------------------------
------ ������ �߰� �Լ� ---------------------------------------------------
---------------------------------------------------------------------------

-- ���ο� �̺�Ʈ �߰�
function AddEventDrop( strDesc )
	local nEventId 	= table.getn(tEventDrop) + 1
	
	tEventDrop[nEventId] = {}
	tEventDrop[nEventId].tLang = {}
	tEventDrop[nEventId].tItem = {}
	tEventDrop[nEventId].tPeriod = {}
	tEventDrop[nEventId].strDesc = strDesc
	tEventDrop[nEventId].dwLevelMin = 0
	tEventDrop[nEventId].dwLevelMax = 0	
end

-- ���� ���
function SetLang( nLang )
	local nEventId 	= table.getn(tEventDrop)
	local nSize 	= table.getn(tEventDrop[nEventId].tLang) + 1

	tEventDrop[nEventId].tLang[nSize] = {}
	tEventDrop[nEventId].tLang[nSize].nLang = nLang
end

-- ������
function SetItem( strItemId, limitQuantityPerOneDay, randomQuantityPerOneMonster )
	local nEventId 	= table.getn(tEventDrop) 
	local nSize 	= table.getn(tEventDrop[nEventId].tItem) + 1
	
	tEventDrop[nEventId].tItem[nSize] = {}
	tEventDrop[nEventId].tItem[nSize].strItemId = strItemId
	tEventDrop[nEventId].tItem[nSize].limitQuantityPerOneDay = limitQuantityPerOneDay
	tEventDrop[nEventId].tItem[nSize].randomQuantityPerOneMonster = randomQuantityPerOneMonster
end


-- ���۽ð�, ���ð�
function SetTime( strStart, strEnd )
	local nEventId 	= table.getn(tEventDrop) 
	local nSize 	= table.getn(tEventDrop[nEventId].tPeriod) + 1
	
	tEventDrop[nEventId].tPeriod[nSize] = {}
	tEventDrop[nEventId].tPeriod[nSize].strStart = strStart
	tEventDrop[nEventId].tPeriod[nSize].strEnd = strEnd
end

-- ���� ����
function SetLevel( dwLevelMin, dwLevelMax )
	local nEventId 	= table.getn(tEventDrop)
	
	tEventDrop[nEventId].dwLevelMin = dwLevelMin
	tEventDrop[nEventId].dwLevelMax =dwLevelMax
end
		


