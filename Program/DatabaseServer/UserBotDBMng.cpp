#include "StdAfx.h"

#ifdef __M_USER_BOTS_DB_BASE
#include "UserBotBehaviour.h"
#include "UserBotDBMng.h"
#include "defineObj.h"
#include "DPCoreSrvr.h"
extern CDPCoreSrvr		g_dpCoreSrvr;
constexpr auto g_ServerDelaySec = 30;
user_bot::CBotDBMng::_vec_bot::iterator	user_bot::CBotDBMng::findBot(const u_long idBot) {
	for (auto it = m_vecBots.begin(); it != m_vecBots.end(); it++) {
		if ((*it).getPlayerID() == idBot) {
			return it;
		}
	}
	return m_vecBots.end();
}
user_bot::CBotDBMng::_vec_ws::iterator	user_bot::CBotDBMng::findServer(const u_long uKey) {
	for (auto it = m_vecServerKeys.begin(); it != m_vecServerKeys.end(); it++) {
		if ((*it).first == uKey) {
			return it;
		}
	}
	return 	m_vecServerKeys.end();
}

std::size_t user_bot::CBotDBMng::removeBotsByServerKey(const u_long uKey) {
	std::size_t nRemoved = 0;
	for (auto itv2 = m_vecBots.begin(); itv2 != m_vecBots.end();) {
		if ((*itv2).getServerKey() == uKey) {
			m_nameMng.recover((*itv2).getName());
			itv2 = m_vecBots.erase(itv2);
			nRemoved++;
		}
		else { itv2++; }
	}
	return nRemoved;
}

u_long	user_bot::CBotDBMng::selectPlayerID() {
	for (auto idCurrent = (m_vecBots.size() + m_nIDStart);
		idCurrent < ULONG_MAX; idCurrent++) {
		auto iterBot = findBot(idCurrent);
		if (iterBot == m_vecBots.end()) {
			return idCurrent;
		}
	}
	/*if ((m_nCurrIndex + m_nIDStart) >= ULONG_MAX) {
		m_nCurrIndex = 0;
	}

	return m_nIDStart+(m_nCurrIndex++);*/
	return 0;
}
std::size_t	user_bot::CBotDBMng::selectBotType() {
	if (m_vecBotType.empty()) {
		return std::size_t(-1);
	}
#ifndef __M_USER_BOTS_SPAWN_AT_TIME
	std::size_t nIndex = xRandom(1, m_vecBotType.size() + 1) - 1;
	if (nIndex == std::size_t(-1) || m_vecBotType.size() < nIndex) {
		nIndex = 0;
	}
#else
	// getMaxBotsAtCurrentTime
	std::vector<CBotType> vecBotTypes;
	for (auto& it : m_vecBotType) {
#ifndef __M_USER_BOTS_LIMIT_TIME
		if (it.checkOnlineTime()) 
#else
		if(this->getCurrentBotCount() < it.getMaxBotsAtCurrentTime())
#endif
		{
			vecBotTypes.push_back(it);
		}
	}
	if (vecBotTypes.empty()) {
		return std::size_t(-1);
	}
	std::size_t nIndex = xRandom(1, vecBotTypes.size() + 1) - 1;
	if (nIndex == std::size_t(-1) || vecBotTypes.size() < nIndex) {
		nIndex = 0;
	}
#endif

	return nIndex;
}
user_bot::CBotDBMng::_vec_ws::iterator	user_bot::CBotDBMng::selectServer() {
	std::size_t nIndex = xRandom(1, m_vecServerKeys.size() + 1) - 1;
	if (nIndex == std::size_t(-1) || m_vecServerKeys.size() < nIndex) {
		nIndex = 0;
	}
	return (m_vecServerKeys.begin() + nIndex);
}
void	user_bot::CBotDBMng::removeServer(const u_long uKey) {
	auto it = findServer(uKey);
	if (it == m_vecServerKeys.end()) { // server not found
		return;
	}
	m_vecServerKeys.erase(it);

	g_dpCoreSrvr.sendRemoveBot(uKey, 0);
	removeBotsByServerKey(uKey);
}

bool	user_bot::CBotDBMng::isBotName(const std::string& strName) {
	return (m_nameMng.isUsed(strName) != std::size_t(-1)
		|| m_nameMng.isUnused(strName) != std::size_t(-1));
}

void user_bot::CBotDBMng::addBot() {
	// check if theres no unused names left
	if (m_nameMng.vecUnused.size() <= 0) {
		FLERROR_LOG(PROGRAM_NAME, "%s: No more bot-names available.", __FUNCTION__);
		return;
	}
	auto itServer = selectServer();
	if (itServer->second + g_ServerDelaySec > ::time(nullptr)) {
		return;
	}
	// create new bot
	// select bot type index 
	std::size_t nBotType = selectBotType();
#ifdef __M_USER_BOTS_SPAWN_AT_TIME
	if (nBotType == std::size_t(-1)) { // no valid online time found.
		return;
	}
#endif
	user_bot::CBotType botType(m_vecBotType[nBotType]);
	botType.setServerKey(itServer->first);
	botType.setName(m_nameMng.pick());
	botType.setPlayerID(selectPlayerID());
	// send to ws, core
	g_dpCoreSrvr.sendAddBot(botType.getServerKey(), botType.getPlayerID(),
		botType.getName(), nBotType);
	// save bot
	m_vecBots.push_back(botType);
}

void user_bot::CBotDBMng::removeBot(_vec_bot::iterator iter) {
	m_nameMng.recover((iter)->getName());
	g_dpCoreSrvr.sendRemoveBot((iter)->getServerKey(), (iter)->getPlayerID());
	m_vecBots.erase(iter);
}


void user_bot::CBotDBMng::process() {
	if (m_tStart == 0) {
		m_tStart = ::time(nullptr) + g_ServerDelaySec;
	}
	if (m_vecServerKeys.size() <= 0 || m_tStart > ::time(nullptr)) {
		return;
	}
	if (getCurrentBotCount() < getMaxBotCount()) {
		if (m_vecBots.size() < m_nMaxBotCount
			&& m_tNextBotAddTick < time(nullptr)) {
			addBot();
			m_tNextBotAddTick = time(nullptr) + xRandom(m_nMaxSpawnDelay);
		}
	}
	if (m_tNextBotRemoveTick < time(nullptr)) {
		/*for (auto it = m_vecBots.begin(); it != m_vecBots.end(); it++) {
			//	it = removeBot(it);
			removeBot(&it);
			break;
		}*/
		if (!m_vecBots.empty()) {
			const auto nRandomBotIndex = xRandom(1, m_vecBots.size() + 1) - 1;
			for (auto it = m_vecBots.begin(); it != m_vecBots.end(); it++) {
				if (!it->checkOnlineTime()) {
					this->removeBot(it);
					break;
				}
			}
		//	if (auto itRmv = (m_vecBots.begin() + nRandomBotIndex); itRmv != m_vecBots.end()) {
			//	this->removeBot(itRmv);
			//}
		}
		m_tNextBotRemoveTick = time(nullptr) + xRandom(m_nMaxRemoveDelay);
	}
}
#ifdef __M_USER_BOTS_RELOAD_CONFIG
void user_bot::CBotDBMng::reload() {
	m_tStart = ::time(nullptr) + g_ServerDelaySec;
	m_tNextBotRemoveTick = ::time(nullptr) + 1000;
	m_tNextBotAddTick = ::time(nullptr) + 1000;
	// remove all bots
	for (auto& it : m_vecServerKeys) {
		g_dpCoreSrvr.sendRemoveBot(it.first, 0);
		this->removeBotsByServerKey(it.first);
	}
	this->loadBehaviorScript();
	m_tNextBotAddTick = time(nullptr) + m_nMaxSpawnDelay;
	m_tNextBotRemoveTick = time(nullptr) + m_nMaxRemoveDelay + 60;
}
#endif
bool user_bot::CBotDBMng::loadBehaviorScript() {
	CLuaBase lua;
	if (lua.RunScript("BotBehavior.lua") != 0) {
		FLERROR_LOG(PROGRAM_NAME, "%s: Couldnt load Bot-Behavior settings: BotBehavior.lua", __FUNCTION__);
		return false;
	}
#if defined(__DBSERVER) && defined(__M_USER_BOTS_DB_BASE)
	//?? value not valid 
	m_nMaxRemoveDelay = static_cast<std::size_t>(lua.GetGlobalNumber("MAX_REMOVE_DELAY"));
	m_nMaxSpawnDelay = static_cast<std::size_t>(lua.GetGlobalNumber("MAX_SPAWN_DELAY"));
#endif
	m_nMaxBotCount = static_cast<std::size_t>(lua.GetGlobalNumber("MAX_BOT_COUNT"));
	m_nIDStart = static_cast<std::size_t>(lua.GetGlobalNumber("ID_BOT_START"));

#if defined(__DBSERVER) && defined(__M_USER_BOTS_DB_BASE)
	m_tNextBotAddTick = time(nullptr) + m_nMaxSpawnDelay;
	m_tNextBotRemoveTick = time(nullptr) + m_nMaxRemoveDelay + 60;

	m_nameMng.vecUnused = lua.getArray<std::string>("g_BotNames");

	if (m_nMaxBotCount >= m_nameMng.vecUnused.size()) {
		FLERROR_LOG(PROGRAM_NAME, "%s: You need to add more Bot-Names or decrease MAX_BOT_COUNT", __FUNCTION__);
		return false;
	}
#endif
	lua.GetGloabal("g_BotType");
	lua.PushNil();
	m_vecBotType.clear();
	while (lua.TableLoop(-2)) {
		//		std::string strBehavior = (lua.GetFieldToString(-1, "strMessages"));
		DWORD dwWorldID = static_cast<DWORD>(CScript::GetDefineNum(lua.GetFieldToString(-1, "strWorldID")));
		float x = static_cast<float>(lua.GetFieldToNumber(-1, "x"));
		float y = static_cast<float>(lua.GetFieldToNumber(-1, "y"));
		float z = static_cast<float>(lua.GetFieldToNumber(-1, "z"));
		DWORD dwMoverID = static_cast<DWORD>(CScript::GetDefineNum(lua.GetFieldToString(-1, "strMoverID")));
		int nJob = static_cast<DWORD>(CScript::GetDefineNum(lua.GetFieldToString(-1, "strJobName")));
		int nLevel = static_cast<int>(lua.GetFieldToNumber(-1, "nLevel"));
		if (nLevel > MAX_3RD_LEGEND_LEVEL /*MAX_LEVEL*/ || nLevel < 1) {
			nLevel = 1;
		}
		if (nJob < JOB_VAGRANT || nJob >= MAX_JOB) {
			nJob = JOB_VAGRANT;
		}
		// DEFAULT
		if (dwMoverID == 0) {
			dwMoverID = MI_FEMALE;
		}
		if (dwWorldID == 0) {
			// madrigal
			dwWorldID = WI_WORLD_MADRIGAL;
		}
		if (x == 0 && y == 0 && z == 0) {
			// set to flaris spawn point
			x = 6983;
			y = 0;
			z = 3330;
		}
		// user behaviour base 
		// spawn position, spawn world
		CBotType type;
		type.setSpawnPos(D3DXVECTOR3(x, y, z));
		type.setSpawnWorld(dwWorldID);
		type.setMoverID(dwMoverID);
		type.setJob(nJob);
		type.setLevel(nLevel);
		// add base move 
		//type.addBehaviour("move", 0);
		// add single behaviour
#ifndef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
		lua.GetField(-1, "Behaviour");
		lua.PushNil();
		while (lua.TableLoop(-2)) {
			std::string strBehaviour = lua.GetFieldToString(-1, "strBehaviour");
			std::size_t nDuration = static_cast<std::size_t>(lua.GetFieldToNumber(-1, "nDuration"));
			type.addBehaviour(strBehaviour, nDuration);
			lua.Pop(1);
		}
		lua.Pop(1);
#else
		//if (bIndividual == true) 
		{
			lua.GetField(-1, "OnlineTimes");
			lua.PushNil();
			while (lua.TableLoop(-2)) {
#ifndef __M_USER_BOTS_LIMIT_TIME
				user_bot::USER_BOT_LOGIN login{
					mu::str2time(lua.GetFieldToString(-1, "strTimeStart"), "%H:%M"),
					static_cast<int>(lua.GetFieldToNumber(-1, "nMinutesOnline"))
				};
#else
				user_bot::USER_BOT_LOGIN login;
				login.timeStart = mu::str2time(lua.GetFieldToString(-1, "strTimeStart"), "%H:%M");
				login.nOnlineTime = static_cast<int>(lua.GetFieldToNumber(-1, "nMinutesOnline"));
				login.nMaxBots = static_cast<int>(lua.GetFieldToNumber(-1, "nMaxBots"));
#endif
				lua.GetField(-1, "Behaviour");
				lua.PushNil();
				while (lua.TableLoop(-2)) {
					std::string strBehaviour = lua.GetFieldToString(-1, "strBehaviour");
					std::size_t nDuration = static_cast<std::size_t>(lua.GetFieldToNumber(-1, "nDuration"));
					login.vecBehaviour.push_back({ strBehaviour, nDuration });
					lua.Pop(1);
				}
				type.addOnlineTime(login);
				lua.Pop(1);
				lua.Pop(1);
			}
		}
		lua.Pop(1);
#endif
		lua.GetField(-1, "Inventory");
		lua.PushNil();

		while (lua.TableLoop(-2)) {
			FLItemElem elem;
			elem.m_dwItemId = static_cast<DWORD>(CScript::GetDefineNum(lua.GetFieldToString(-1, "strItemID")));
			elem.SetAbilityOption(static_cast<int>(lua.GetFieldToNumber(-1, "nUpgradeLevel")));
			elem.SetItemResist(static_cast<BYTE>(lua.GetFieldToNumber(-1, "nElement")));
			elem.m_nResistAbilityOption = static_cast<int>(lua.GetFieldToNumber(-1, "nElementLevel"));
			elem.m_nItemNum = static_cast<short>(lua.GetFieldToNumber(-1, "nItemCount"));
			//int nCost = (static_cast<int>(lua.GetFieldToNumber(-1, "bEquip")) == TRUE ? 1337 : 0);	
			type.addBotItem(elem);
			lua.Pop(1);
		}
		lua.Pop(1);
		lua.Pop(1);
		m_vecBotType.push_back(type);
	}
#ifndef __M_USER_BOTS_INDIVIDUAL
	return m_vecBotType.size() > 0;
#else
	return loadIndividualScript();
#endif
}
#ifdef __M_USER_BOTS_INDIVIDUAL
bool user_bot::CBotDBMng::loadIndividualScript() {
	auto nSize = m_vecBotType.size();
	CLuaBase lua;
	if (lua.RunScript("BotIndividual.lua") != 0) {
		FLERROR_LOG(PROGRAM_NAME, "%s: Couldnt load Bot-Individual settings: BotIndividual.lua", __FUNCTION__);
		return false;
	}

	lua.GetGloabal("g_BotType");
	lua.PushNil();
	while (lua.TableLoop(-2)) {
		//		std::string strBehavior = (lua.GetFieldToString(-1, "strMessages"));
		DWORD		dwWorldID = static_cast<DWORD>(CScript::GetDefineNum(lua.GetFieldToString(-1, "strWorldID")));
		int			x = static_cast<int>(lua.GetFieldToNumber(-1, "x"));
		int			y = static_cast<int>(lua.GetFieldToNumber(-1, "y"));
		int			z = static_cast<int>(lua.GetFieldToNumber(-1, "z"));
		DWORD		dwMoverID = static_cast<DWORD>(CScript::GetDefineNum(lua.GetFieldToString(-1, "strMoverID")));
		int			nJob = static_cast<DWORD>(CScript::GetDefineNum(lua.GetFieldToString(-1, "strJobName")));
		int			nLevel = static_cast<int>(lua.GetFieldToNumber(-1, "nLevel"));
		std::string strName = lua.GetFieldToString(-1, "strName");
		bool		bIndividual = static_cast<BOOL>(lua.GetFieldToNumber(-1, "bIndividual")) == TRUE ? true : false;

		if (nLevel > MAX_3RD_LEGEND_LEVEL /*MAX_LEVEL*/ || nLevel < 1) {
			nLevel = 1;
		}
		if (nJob < JOB_VAGRANT || nJob >= MAX_JOB) {
			nJob = JOB_VAGRANT;
		}
		// DEFAULT
		if (dwMoverID == 0) {
			dwMoverID = MI_FEMALE;
		}
		if (dwWorldID == 0) {
			// madrigal
			dwWorldID = WI_WORLD_MADRIGAL;
		}
		if (x == 0 && y == 0 && z == 0) {
			// set to flaris spawn point
			x = 6983;
			y = 0;
			z = 3330;
		}
		// user behaviour base 
		// spawn position, spawn world
		CBotType type;
		type.setSpawnPos(D3DXVECTOR3(x, y, z));
		type.setSpawnWorld(dwWorldID);
		type.setMoverID(dwMoverID);
		type.setJob(nJob);
		type.setLevel(nLevel);
		type.setName(strName);
		type.setIndividual(bIndividual);
#ifndef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
		type.setStartTime(mu::str2time(lua.GetFieldToString(-1, "strTimeStart"), "%H:%M"));
		type.setMaxOnlineTime(lua.GetFieldToNumber(-1, "nMinutesOnline"));
		type.setLoopCount(/*lua.GetFieldToNumber(-1, "nLoop")*/ 0);
#else
		//if (bIndividual == true) 
		{
			lua.GetField(-1, "OnlineTimes");
			lua.PushNil();
			while (lua.TableLoop(-2)) {
#ifndef __M_USER_BOTS_LIMIT_TIME
				user_bot::USER_BOT_LOGIN login{
					mu::str2time(lua.GetFieldToString(-1, "strTimeStart"), "%H:%M"),
					static_cast<int>(lua.GetFieldToNumber(-1, "nMinutesOnline"))
				};
#else
				user_bot::USER_BOT_LOGIN login;
				login.timeStart = mu::str2time(lua.GetFieldToString(-1, "strTimeStart"), "%H:%M");
				login.nOnlineTime = static_cast<int>(lua.GetFieldToNumber(-1, "nMinutesOnline"));
				login.nMaxBots = static_cast<int>(lua.GetFieldToNumber(-1, "nMaxBots"));
#endif
				lua.GetField(-1, "Behaviour");
				lua.PushNil();
				while (lua.TableLoop(-2)) {
					std::string strBehaviour = lua.GetFieldToString(-1, "strBehaviour");
					std::size_t nDuration = static_cast<std::size_t>(lua.GetFieldToNumber(-1, "nDuration"));
					login.vecBehaviour.push_back({ strBehaviour, nDuration });
					lua.Pop(1);
				}
				type.addOnlineTime(login);
				lua.Pop(1);
				lua.Pop(1);
			}
		}
		lua.Pop(1);
#endif
#ifndef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
		lua.GetField(-1, "Behaviour");
		lua.PushNil();
		while (lua.TableLoop(-2)) {
			std::string strBehaviour = lua.GetFieldToString(-1, "strBehaviour");
			std::size_t nDuration = static_cast<std::size_t>(lua.GetFieldToNumber(-1, "nDuration"));
			type.addBehaviour(strBehaviour, nDuration);
			lua.Pop(1);
		}
		lua.Pop(1);

#endif
		lua.GetField(-1, "Inventory");
		lua.PushNil();

		while (lua.TableLoop(-2)) {
			FLItemElem elem;
			elem.m_dwItemId = static_cast<DWORD>(CScript::GetDefineNum(lua.GetFieldToString(-1, "strItemID")));
			elem.SetAbilityOption(static_cast<int>(lua.GetFieldToNumber(-1, "nUpgradeLevel")));
			elem.SetItemResist(static_cast<int>(lua.GetFieldToNumber(-1, "nElement")));
			elem.m_nResistAbilityOption = static_cast<int>(lua.GetFieldToNumber(-1, "nElementLevel"));
			elem.m_nItemNum = static_cast<int>(lua.GetFieldToNumber(-1, "nItemCount"));
			// not needed
			//int nCost = (static_cast<int>(lua.GetFieldToNumber(-1, "bEquip")) == TRUE ? 1337 : 0);	
			type.addBotItem(elem);
			lua.Pop(1);
		}
		lua.Pop(1);
		lua.Pop(1);
		if (bIndividual == true) {
			m_vecBotType.push_back(type);
		}
	}
	return m_vecBotType.size() >= nSize;
}
#endif


#endif