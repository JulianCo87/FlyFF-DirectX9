-- remove delay: removes every 60 seconds a bot.
MAX_REMOVE_DELAY = 60;
-- spawn delay: spawns every 10 seconds a bot.
MAX_SPAWN_DELAY = 10;
-- set bot start-id
ID_BOT_START = 2147483647+1;
-- max bot count at the same time
MAX_BOT_COUNT = 135;
-- global behaviour table
g_BotType = {}

SEX_MALE   =          0
SEX_FEMALE  =         1
SEX_SEXLESS  =        2

function MIN(x)
	return (60*x)
end

function HRS(x)
	return MIN(60*x)
end

function random(from, to)
	return math.random(from,to)
end

g_BotNames = {
"Eouen",
"Oyarsa",
"Wingedd",
"xWing3d",
"BeBe",
"Perelan",
"Wateva",
"Whatever",
"Phillia",
"Zarathos",
"eViLXyKLoPz",
"ExterminAZN",
"BioHaZard",
"Usoda",
"PrincessKilla",
"Joey87",
"Melancholy",
"Henri",
"z3dddy",
"HomEr",
"ItsMarit",
"MgfonzZ",
"Raven",
"Xerusian",
"Lillyfee",
"LadyVi3t",
"nRaike",
"JohnnyAnGeLZ",
"givemepenya",
"penyaboi",
"Lettos",
"KayuzaX",
"Phuse",
"Fuzedd",
"LunicA",
"Esosleep",
"FaiTHANgeL",
"ItsReiko",
"ABUSED",
"SAVAGE",
"Draxxx",
"UchichaItachi",
"Gokuson",
"MrPK",
"SirSpeedy",
"xXAngelXx",
"TWISTER",
"xLegend19",
"xIAMLEGENDx",
"Foamless",
"Nyoko",
"ExhilarAZN",
"SnowAngel",
"SnowBird",
"WontoNSouP",
"Seph",
"Kona",
"Fyyyyre",
"Lenus",
"Lachesis",
"Kyoroji",
"PheonixPL",
"BestManPL",
"StarAngelBR",
"DofuzBR",
"GxBRage",
"Metroid89",
"GoddessJanette",
"Zaniac" ,
"Raven20" ,
"xEternity",
"xLegend17",
"xLegend18",
"NishiFR",
"GodFR" ,
"LuciFR",
"DigitalCupcake",
"Deadlarr",
"DeathDagon",
"Champiyunn",
"Eternal",
"xXDeathDemonXx",
"Aatrix",
"Zapz",
"Incoherent",
"Mate",
"Drunk",
"Lewd",
"Overwulf",
"blueeyesky",
"J0ule",
"ShadouJetkuma",
"Koolys",
"RabidFish",
"xXPENGPATOUPAUXx" ,
"Lydia",
"NOYEAHSUREGOAHEAD",
"Adob3",
"Ravencrest",
"Sylvanas",
"Ratapai",
"xSwapxy",
"Zixzu",
"PAPABLESS",
"Bearlyviable",
"PayBills",
"Misterpwns",
"theoriginalgod",
"Xotz",
"BeGoneThot",
"Zhrey",
"Tjpj",
"Mgfonz",
"Chezjr",
"Awator",
"Esprespartacus",
"Silliken",
"NekRoMaNCeR",
"OmegaLul",
"Gr3if",
"SixPax",
"PaxiFixi",
"Shufflez",
"Lasalle",
"Evilness" ,
"AcroD",
"AmazonHero",
"OverPowered",
"Teleri",
"Tinablen",
"BestNights",
"Terr0r",
"luxagirl",
"Hikailu",
"Morgannor",
"Synath",
"luxagirly",
"Vcom",
"SkyDust",
"SheRRa22",
"PauloWaBR",
"Bazdaa",
"Taypler",
"sanosuke",
"Darknass",
"SupremeAngel",
"Allux",
"Arestos",
"Perfecta",
"PsychoMantis",
"Whitish",
"Inspiration",
"Cara",
"xInsanity",
"jaoar101",
"Xyrho",
"Derek",
"Darkeh" ,
"SnowCrystal",
"Sairy",
"Darkas" ,
"Valkyra",
"TONGA",
"troyucks",
"OneZ",
"tordek",
"o0Xander0o",
"Yadira",
"Raato",
"Lunicas" ,
"OLIONO",
"xQuentin",
"NarashiFR",
"psychomatikFR",
"NarutoUzumakiFR",
"ParanoidAndroid",
"Hannibal33",
"Galathorn",
"Probonjo",
"KinkITA",
"ZeusBlade",
"BATMichael",
"Kulluah",
"Xixu",
"Heidi",
"AngelicaBellaITA",
"OnosITA",
"AcidBurn",
"Bash0r",
"Cann0nBall",
"HugeHero",
"DerErsteRitter",
"eXisted",
"GT3RS",
"GT86",
"LudaZaTobom",
"NeikiMauSi",
"SkyZzn",
"Alure",
"Keiko",
"UKIZzZzZ",
"Mimichan",
"BornToKill",
"lolos",
"BornToDie",
"rameee2",
"GIFT3D",
"LadyITA",
"AlludeITA",
"sirion1998",
"KofiKiko87",
"silabear",
"Alyyyz",
"MrTheIzlude",
"Z3ratul",
"LordNixon",
"chuykiller",
"xXDreamXx",
"Fabia",
"LoveFusion",
"ZapraS",
"sSXxXSs",
"FrozenSniper",
"GreatITA",
"Milky",
"Eur0pe",
"Clockz",
"Glockz",
"PKBKRK",
"vince",
"ThunderHeart",
"Saganaki",
"Wannabexc0n",
"xc0n",
"XxXSkyXxX",
"bloodymary",
"Swift",
"DESKJET",
"DESKJETS",
"DESKJETZ",
"NazgulPL",
"rafalziom",
"EmoDoll",
"JQUIM",
"BlackLabel",
"JackDaniels",
"Playdemo",
"lusvi",
"xXShadowXx",
"Art3mis",
"FiliaChan",
"ArmorKinG",
"Luri",
"Liyda",
"REBORN",
"REBIRTH",
"XIAOREN",
"HELLCHAOS",
"LEGIONS",
"X0din",
"Zemus",
"NightWarrior",
"Nefast",
"xOden",
"Firey",
"PeterPan",
"xDonniex",
"xLoseYourselfx",
"Eminem",
"ImAMango",
"ColdPizza",
"Sniffles",
"Snuffling",
"Scorpion",
"Rwarar",
"BuyMeWinrar",
"DreamEather",
"Mc1991",
"FR0STN0VA",
"TurkishFighter",
"PowerOfUAE",
"KillerCortiz",
"devilluck",
"xSnowGhostx",
"Dr0n",
"Dr0ne",
"Davorix",
"ghost1971",
"genkino",
"Kari22",
"YohkoChan",
"Legrande",
"PinkTurtle",
"Damage",
"EpicDamage",
"KawaiiKaki",
"gabe1",
"Suave",
"Amber",
"TempesT",
"PyschicNoob",
"Westlife",
"Player001",
"KeiziK",
"Kurryuption",
"curryishot",
"Dign1ty",
"Killala",
"Izumiii",
"Di4blo",
"DeathRipperX",
"Sakura1005",
"MGDIS4ME",
"ZoeleoZ",
"KiraKira",
"DiscordSux",
"Scwhan19",
"Kestri",
"XmurdockX",
"AznCrush",
"AznKinG",
"BALLIN",
"LazersGoPewPew",
"Ghaunkah",
"Kurbee610",
"MegaJerk",
"Lanaya",
"Atkascha",
"Cloud9",
"ImMLG",
"ChubbyYohko",
"Ylenae",
"SuperPinoy",
"LadyR1kku",
"HelloKitty99",
"Swishy",
"Dankenstein",
"flymypsych",
"ChihiroChan",
"KiwiWar",
"Biggun1",
"Americaa",
"theCRUSHER",
"ChatX",
"FlashCry",
"StormyX",
"PhantomPL",
"FlyForJohn",
"KILLAPRINCE",
"flav0racid",
"SrRich",
"Veyron",
"zarkness",
"Shimbapa",
"SoulKillerBR",
"iHopeYouDie",
"SPSILVERS",
"Belladonna",
"Star15",
"Phuongies",
"Phuong",
"NotAMethAddict",
"TaKeN",
"Arimakun95",
"Chikitica",
"CiroC",
"TranceDynasty",
"Cavativa",
"ClapzZ",
"SouleaterEl",
"Eskwyre",
"RickAstley",
"GunstarRZ",
"Dragon721",
"BigBoyyy",
"Phyrrro",
"AbuSabu",
"VuSaBu",
"poshcat",
"B3S3RK",
"Emeraude",
"Shad0wFl4re",
"Dukay",
"LordFarQad",
"Shrek",
"Romeo",
"loveyoumore",
"NinMeau",
"Saviola",
"TrippleX",
"DarkNightsAhead",
"GreenMoon",
"ImBored",
"Mhhhhhhm",
"YeahSureSure",
"Tepentine",
"WontUseAmps"
}

-- AddUserBotType("collector_only", "MI_FEMALE", "JOB_BLADE", 65)
-- base function for User-Bot-Types
function addUserBotType(strName, strMoverID, strJobName, nLevel)
	local nIndex = #( g_BotType ) + 1
	g_BotType[nIndex] = {}
	g_BotType[nIndex].strMoverID = strMoverID;
	g_BotType[nIndex].strJobName = strJobName;
	g_BotType[nIndex].nLevel = nLevel;
	g_BotType[nIndex].strWorldID ="";
	g_BotType[nIndex].x = 0;
	g_BotType[nIndex].y = 0;
	g_BotType[nIndex].z = 0;
	--g_BotType[nIndex].Behaviour = {};
	g_BotType[nIndex].Inventory = {};
	g_BotType[nIndex].nGold = 0;
	g_BotType[nIndex].strModelID =""
	g_BotType[nIndex].strName = strName;
	g_BotType[nIndex].bIndividual = 0
	g_BotType[nIndex].OnlineTimes = {}
	-- set sex automatically
	if strMoverID == "MI_MALE" then
		g_BotType[nIndex].nSex = SEX_MALE;
	elseif strMoverID == "MI_FEMALE" then
		g_BotType[nIndex].nSex = MI_FEMALE;
	else
		g_BotType[nIndex].nSex = SEX_SEXLESS;
	end
	return nIndex;
end

function addIndividualBot(strName, strMoverID, strJobName, nLevel)
	-- call base function 
	local nIndex = addUserBotType(strName, strMoverID, strJobName, nLevel);
	-- set individual to 1
	setIndividual(1);
end

function setIndividual(bIndividual)
	local nIndex = #( g_BotType )
	g_BotType[nIndex].bIndividual = bIndividual
end


function setLevel(nLevel)
	local nIndex = #( g_BotType )
	g_BotType[nIndex].nLevel = nLevel
end

function setSex(nSex)
	local nIndex = #( g_BotType )
	if nSex==SEX_FEMALE then
		if g_BotType[nIndex].strMoverID == "MI_MALE" then
			error("Sex == SEX_FEMALE && moverID == MI_MALE. Sex setted to SEX_MALE.");
		end
		g_BotType[nIndex].nSex = SEX_MALE;
	end
	if nSex==SEX_MALE then
		if g_BotType[nIndex].strMoverID == "MI_FEMALE" then
			error("Sex == SEX_MALE && moverID == MI_FEMALE. Sex setted to SEX_FEMALE.");
		end
		g_BotType[nIndex].nSex = SEX_FEMALE;
	end		
	g_BotType[nIndex].nSex = nSex
end
-- duration -1 	= loop the behaviour (function executed everytime called Process)
-- duration 0 	= one constant behaviour
function addBehaviour(strBehaviour, nDuration) -- in seconds
--	local arg={...};
--	local buff ="";
--	for i=0, #arg do
--		if i+1 < #arg then
--			buff = buff .. arg[i] .. ":" .. tostring(arg[i+1]) .. ";";
--		end
	local nIndex = #( g_BotType )
	local nIndexLogins = #(g_BotType[nIndex].OnlineTimes)
	local nIndexBehaviour = #( g_BotType[nIndex].OnlineTimes[nIndexLogins].Behaviour )+1
	g_BotType[nIndex].OnlineTimes[nIndexLogins].Behaviour[nIndexBehaviour] = {};
	g_BotType[nIndex].OnlineTimes[nIndexLogins].Behaviour[nIndexBehaviour].strBehaviour = strBehaviour;
	g_BotType[nIndex].OnlineTimes[nIndexLogins].Behaviour[nIndexBehaviour].nDuration = nDuration;
--	g_BotType[nIndex].strMessages = buff;
end

--function setBehaviour(...) 
--		local arg={...};
--		local buff ="";
--		for i=0, #arg do
--			if i+1 < #arg then
--				buff = buff .. arg[i] .. ":" .. tostring(arg[i+1]) .. ";";
--			end
--		end
--	local nIndex = #( g_BotType )	
--	g_BotType[nIndex].strMessages = buff;
--end


function addItem(strItemID, nItemCount)
	local nIndex = #( g_BotType )
	local nIndexEq = #( g_BotType[nIndex].Inventory )+1
	g_BotType[nIndex].Inventory[nIndexEq] = {};
	g_BotType[nIndex].Inventory[nIndexEq].strItemID = strItemID;
	g_BotType[nIndex].Inventory[nIndexEq].nItemCount = nItemCount;
	g_BotType[nIndex].Inventory[nIndexEq].bEquip = 0;

end
function addEquipment(strItemID )
	local nIndex = #( g_BotType )
	local nIndexEq = #( g_BotType[nIndex].Inventory )+1
	g_BotType[nIndex].Inventory[nIndexEq] = {};
	g_BotType[nIndex].Inventory[nIndexEq].strItemID = strItemID;
	g_BotType[nIndex].Inventory[nIndexEq].nItemCount = 1;
	g_BotType[nIndex].Inventory[nIndexEq].bEquip = 1;
end

function addEquipmentEx(strItemID, nUpgradeLevel, nElement, nElementLevel)
	local nIndex = #( g_BotType )
	local nIndexEq = #( g_BotType[nIndex].Inventory )+1
	g_BotType[nIndex].Inventory[nIndexEq] = {};
	g_BotType[nIndex].Inventory[nIndexEq].strItemID = strItemID;
	g_BotType[nIndex].Inventory[nIndexEq].nUpgradeLevel = nUpgradeLevel;
	g_BotType[nIndex].Inventory[nIndexEq].nElement = nElement;
	g_BotType[nIndex].Inventory[nIndexEq].nElementLevel = nElementLevel;
	g_BotType[nIndex].Inventory[nIndexEq].nItemCount = 1;
	g_BotType[nIndex].Inventory[nIndexEq].bEquip = 1;
end

function addGold(nGoldCount)
	local nIndex = #( g_BotType )
	g_BotType[nIndex].nGold = nGoldCount;
end


function spawn(strWorldID, x,y,z)
	local nIndex = #( g_BotType )
	g_BotType[nIndex].strWorldID = strWorldID;
	g_BotType[nIndex].x = x;
	g_BotType[nIndex].y = y;
	g_BotType[nIndex].z = z;
end


function setName(strName)
	local nIndex = #( g_BotType )
	g_BotType[nIndex].strName = strName;
end

function setJob(strJob)
	local nIndex = #( g_BotType )
	g_BotType[nIndex].strJob = strJob;
end

-- start daily at .. 
function addOnlineTime(strTimeStart, nMinutesOnline)
	local nIndex = #( g_BotType )
	local nIndexLogins = #( g_BotType[nIndex].OnlineTimes )+1
	g_BotType[nIndex].OnlineTimes[nIndexLogins] = {}
	g_BotType[nIndex].OnlineTimes[nIndexLogins].strTimeStart = strTimeStart;
	g_BotType[nIndex].OnlineTimes[nIndexLogins].nMinutesOnline = nMinutesOnline;
	g_BotType[nIndex].OnlineTimes[nIndexLogins].Behaviour = {}
end