--------------------------------------------------------------------
-- �ʱ�ȭ ----------------------------------------------------------
--------------------------------------------------------------------
dofile( ".\\LuaFunc\\SecretRoomFunc.lua" )
--------------------------------------------------------------------

--------------------------------------------------------------------
-- ���� ���� ��� ���� ---------------------------------------------
--------------------------------------------------------------------
MinGuildLevel		=	20		-- ���� ���� �ּ� ��� ����(20����)
MinGuildMemberLevel	=	30     		-- ���� ������ �ּ� ����
MinPenya		= 	100000000	-- �ּ� ���� ���(����)
MinGuildNum		=	1		-- �ּ� ���� ���� ��� ��
MaxGuildNum		=	8		-- �ִ� ���� ���� ��� ��
MinGuildMemberNum	=	1      		-- �ּ� ���� ���� ��
MaxGuildMemberNum	= 	11	    	-- �ִ� ���� ���� ��

CancelReturnRate	=	20		-- ���� ��� ��ȯ��(%)
DropoutReturnRate	=	2		-- ���� ���� ��ȯ��(%)
--------------------------------------------------------------------

--------------------------------------------------------------------
-- �ð� ���� -------------------------------------------------------
--------------------------------------------------------------------
-- ���� - Sun, Mon, Tue, Wed, Thu, Fri, Sat
OpenTime			= 	"Wed 14:55"

LineUpTime			=	MIN(60)		-- ���� ���� ��� �ð�
EntranceTime			=	SEC(0)		-- ���� ���ѽð�
WarWaitTime			=	MIN(10)		-- ���� �غ� �ð�
WarTime				=	MIN(120)	-- ���� �ð�
CloseWaitTime			=	SEC(30)		-- ���� ���� �� ��� �ð�
--------------------------------------------------------------------

--------------------------------------------------------------------
-- ���� ���� ���� ------------------------------------------------
--------------------------------------------------------------------
MonsterGenNum		= 2	-- �ѹ��� ������ ���� ����
--------------------------------------------------------------------
-- AddMonster( Type, ����ID, ��ä ��, ������, x1��ǥ, z1��ǥ, x2��ǥ, z2��ǥ, y��ǥ )
-- AddMonster( MONSTER_NORMAL,		"MI_AIBATT1",	10,	false,	500, 500, 505, 505, 10 )
-- AddMonster( MONSTER_MIDBOSS,		"MI_AIBATT1",	5,	true,	500, 500, 505, 505, 10 )
-- AddMonster( MONSTER_BOSS,		"MI_AIBATT2",	2,	true,	500, 500, 505, 505, 10 )

--1����-------------------------------------------------------------
AddMonster( MONSTER_NORMAL,		"MI_CAITSITH04_1",	14,	false,	284, 556, 304, 667, 100 )
AddMonster( MONSTER_NORMAL,		"MI_HARPY04_1",		5,	false,	284, 556, 304, 667, 100 )

--2����-------------------------------------------------------------
AddMonster( MONSTER_NORMAL,		"MI_HARPY04_1",		13,	false,	324, 703, 428, 684, 100 )
AddMonster( MONSTER_NORMAL,		"MI_POLEVIK04_1",	4,	false,	324, 703, 428, 684, 100 )

--3����-------------------------------------------------------------
AddMonster( MONSTER_NORMAL,		"MI_POLEVIK04_1",	11,	false,	444, 666, 463, 564, 100 )
AddMonster( MONSTER_NORMAL,		"MI_ABRAXAS04_1",	4,	false,	444, 666, 463, 564, 100 )

--4����-------------------------------------------------------------
AddMonster( MONSTER_NORMAL,		"MI_ABRAXAS04_1",	10,	false,	426, 545, 349, 526, 100 )
AddMonster( MONSTER_NORMAL,		"MI_HAG04_1",		3,	false,	426, 545, 349, 526, 100 )

--5����-------------------------------------------------------------
AddMonster( MONSTER_NORMAL,		"MI_HAG04_1",		8,	false,	331, 564, 313, 644, 100 )
AddMonster( MONSTER_NORMAL,		"MI_THOTH04_1",		3,	false,	331, 564, 313, 644, 100 )

--6����-------------------------------------------------------------
AddMonster( MONSTER_NORMAL,		"MI_THOTH04_1",		8,	false,	348, 677, 400, 659, 100 )
AddMonster( MONSTER_NORMAL,		"MI_KHNEMU04_1",	2,	false,	348, 677, 400, 659, 100 )

--7����-------------------------------------------------------------
AddMonster( MONSTER_NORMAL,		"MI_KHNEMU04_1",	6,	false,	418, 643, 436, 589, 100 )
AddMonster( MONSTER_NORMAL,		"MI_DANTALIAN04_1",	2,	false,	418, 643, 436, 589, 100 )

--8����-------------------------------------------------------------
AddMonster( MONSTER_NORMAL,		"MI_DANTALIAN04_1",	5,	false,	399, 571, 350, 552, 100 )
AddMonster( MONSTER_NORMAL,		"MI_GANESA04_1",	2,	false,	399, 571, 350, 552, 100 )

--9����-------------------------------------------------------------
AddMonster( MONSTER_MIDBOSS,		"MI_ASURA04_1",		1,	true,	370, 625, 375, 630, 100 )

--����ǹ�----------------------------------------------------------
AddMonster( MONSTER_BOSS,		"MI_LUCIFER01",		1,	true,	600, 360, 605, 365, 100 )