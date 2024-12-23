#include "stdafx.h"
#ifdef __M_USER_BOTS
#include <type_traits>
#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <string>
#include <functional>
#include "UserBot.h"
#include "defineObj.h"

#include <time.h>
#include <locale>
#ifdef __WORLDSERVER
#include "worldmng.h"
#include "World.h"
#include "User.h"
#include "playerdata.h"
#include "DPCoreClient.h"

extern CDPCoreClient g_DPCoreClient;
extern CWorldMng	g_WorldMng;


user_bot::CBot::CBot() {
	init();
}
user_bot::CBot::CBot(const CBotType& botType){
	m_BotType = botType;
	init();
}
user_bot::CBot::~CBot() {

}

void user_bot::CBot::startActivity(const std::int64_t nSeconds) {
	m_tStartLast = (time(nullptr) + nSeconds);
}
bool user_bot::CBot::endActivity() {
	if (m_tStartLast <= time(nullptr)) {
		// set pos to 0
		ZeroMemory(&m_vPosMoveTo, sizeof(D3DXVECTOR3));
		return true;
	}
	return false;
}

// get current activity
bool user_bot::CBot::hasActivity() const {
	if (m_nBehaviourIndex == -1) {
		return false;
	}
#ifdef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
	return (getBotType()->getBehaviour(m_nBehaviourIndex).size() > 0
		&& m_tStartLast > time(nullptr));
#else
	return (getBotType()->getBehaviour().size() > 0
		&& m_tStartLast > time(nullptr));
#endif
}
// get next activity
auto user_bot::CBot::getNextActivity() {
#ifdef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
	return getBotType()->getNextBehaviour(m_nBehaviourIndex+1);
#else
	return getBotType()->getNextBehaviour();
#endif
}
bool user_bot::CBot::execBehaviour(const BEHAVIOUR_DETAIL* pBehaviour) {
	if (pBehaviour && 
		CBotMng::getInstance()->postMsg(*pBehaviour, this) == true) {
		return true;
	}
//	Error("%s: Activity couldnt be executed %s", __FUNCTION__, pBehaviour->str.c_str());
	return false;
}
void user_bot::CBot::processBot() {

#ifdef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
	// check if has activity
	if (!hasActivity()) {		
		// set m_nBehaviourIndex to current index
		if (getBotType()->getAllBehaviours(&m_nBehaviourIndex) == nullptr 
			|| m_nBehaviourIndex == std::size_t(-1)) {
			FLERROR_LOG(PROGRAM_NAME, "%s: No valid activity found. Bot-Type: %s", __FUNCTION__,
				getBotType()->getName().c_str());
			return;
		}
		execBehaviour(getBotType()->getNextBehaviour(m_nBehaviourIndex)); // post the next msg 
		return;
	}
	// execute
	auto pBehaviour = getBotType()->getCurrentBehaviour(m_nBehaviourIndex);
	if (pBehaviour != nullptr && 
		pBehaviour->nDuration == -1) { // -1 means loop the function, 0 means execute once
		execBehaviour(getBotType()->getCurrentBehaviour(m_nBehaviourIndex));
	}
#else // __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
	if (!hasActivity()) {
		// set m_nBehaviourIndex to current index
		execBehaviour(getBotType()->getNextBehaviour()); // post the next msg 
		return;
	}
	// execute
	auto pBehaviour = getBotType()->getCurrentBehaviour();
	if (pBehaviour != nullptr &&
		pBehaviour->nDuration == std::size_t(-1)) { // -1 means loop the function, 0 means execute once
		execBehaviour(getBotType()->getCurrentBehaviour());
	}
#endif // __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
	// process bot-type base, dont needed yet
	getBotType()->processBotType();
	// currently has an activity
	if (endActivity() == false) {
		m_tStartLast = 0;
	}
}
void user_bot::CBot::init() {
	m_tStartLast = 0;
#ifdef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
	m_nBehaviourIndex = 0;
#endif
	CMover::Init();
	FLWSUser::Init();
}

void user_bot::CBot::_delete() {
	this->RemoveAllEnemies();
	this->Delete();
}
#ifdef __BOT_ROUTING
void user_bot::CBot::presupposePos(D3DXVECTOR3* pv, D3DXVECTOR3* pvd, float* pf, u_long uTickCount) {
	D3DXVECTOR3 v = this->GetPos(), v1;
	v1 = v;
	float f = this->GetAngle();
	CWorld* pWorld = this->GetWorld();
	D3DXVECTOR3 vDelta = this->m_pActMover->m_vDelta;
	float fSpeed = this->GetSpeed(this->m_pActMover->m_fSpeed);
	float fTheta;

	DWORD dwMoveState = this->m_pActMover->GetMoveState();
	DWORD dwTurnState = this->m_pActMover->GetTurnState();

	vDelta.x = vDelta.z = 0;
	for (u_long i = 0; i < uTickCount; i++)
	{
		fTheta = D3DXToRadian(f);
		switch (dwMoveState)
		{
		case OBJSTA_FMOVE:
			if (this->m_pActMover->IsStateFlag(OBJSTAF_WALK)) {
				vDelta.x = sin(fTheta) * (fSpeed / 4.0f);
				vDelta.z = -cos(fTheta) * (fSpeed / 4.0f);
			}
			else {
				vDelta.x = sin(fTheta) * fSpeed;
				vDelta.z = -cos(fTheta) * fSpeed;
			}
			break;
		case OBJSTA_BMOVE:
			vDelta.x = sinf(fTheta) * (-fSpeed / 5.0f);
			vDelta.z = -cos(fTheta) * (-fSpeed / 5.0f);
			break;
		}

		switch (dwTurnState)
		{
		case OBJSTA_LTURN:
			f += 4;
			if (f > 360)
				f -= 360;
			break;
		case OBJSTA_RTURN:
			f -= 4;
			if (f < 0)
				f += 360;
			break;
		}

		v.x += vDelta.x;
		v.z += vDelta.z;

		pWorld->ClipX(v.x);
		pWorld->ClipZ(v.z);
	}
	*pv = v;
	*pf = f;
	*pvd = vDelta;
	this->m_nCorr = (int)uTickCount;
}
#endif

void user_bot::CBotMng::cleanUp() {
	m_vecBots.clear();
}
#ifndef __M_USER_BOTS_DB_BASE
void user_bot::CBotMng::addBot(const u_long idBot, const std::string& strName) {
#else
void user_bot::CBotMng::addBot(const u_long idBot, const std::string& strName, const std::size_t nBotType) {
#endif
#ifndef __M_USER_BOTS_DB_BASE
	if (m_vecBots.size() >= m_nMaxBotCount) {
		Error("%s: m_nCount >= m_MaxBotCount", __FUNCTION__);
		return;
	}
	if (m_nameMng.use(strName) == false) {
		Error("%s: Couldnt use Bot-Name %s", __FUNCTION__, strName.c_str());
		return;
	}
#endif

#ifndef __M_USER_BOTS_DB_BASE
	std::size_t nIndex = xRandom(0, m_vecBotType.size());
	CBotType* pBotType = &m_vecBotType[nIndex];
#else
	if (nBotType >= m_vecBotType.size()) {
		FLERROR_LOG(PROGRAM_NAME, "%s: Couldnt add bot: %s - BotType: %d not valid.", __FUNCTION__,
			strName.c_str(), nBotType);
		return;
	}
	CBotType* pBotType = &m_vecBotType[nBotType];
#endif
	if (pBotType == nullptr) {
		// behavior invalid
		return;
	}
	CWorld* pWorld = g_WorldMng.GetWorld(pBotType->getSpawnWorld());
	if (pWorld == nullptr) {
		// world not found
		return;
	}

	// create pointer to CBot class (coz CUser, CMover is gay)
	auto pBot = new CBot(*pBotType);
	pBot->m_idPlayer = (idBot);
	if (strlen(pBot->GetName()) < 3) {
		pBot->SetName(strName.c_str());
	}
	// create the mover
	if (createMover(pBot, pWorld) == false) {
		FLERROR_LOG(PROGRAM_NAME, "%s: Couldnt create bot-mover.", __FUNCTION__);
		return;
	}
	if (createInventory(pBot) == false) {
		// failed to add all items
		FLERROR_LOG(PROGRAM_NAME, "%s: Couldnt add all items: %s", strName.c_str());
	//	return;
	}

	// add player data 
	PlayerData data;
	data.data.nJob = static_cast<BYTE>(pBot->m_nJob);
	data.data.nLevel = static_cast<BYTE>(pBot->m_nLevel);
	data.data.nSex = pBot->GetSex();
	strcpy_s(data.szPlayer, pBot->GetName());
	CPlayerDataCenter::GetInstance()->AddPlayerData(pBot->m_idPlayer, data);

	g_DPCoreClient.SendJoin(pBot->m_idPlayer, pBot->GetName(), 
		(pBot->m_dwAuthorization >= AUTH_GAMEMASTER));

	m_vecBots.push_back(pBot);
}
bool user_bot::CBotMng::createInventory(CBot* pBot) {
	auto vecInventory = pBot->getBotType()->getBotInventory();
	std::size_t nSizeInventoryUser = 0;
	for (auto it = vecInventory.begin(); it != vecInventory.end(); it++) {
		if ((*it).IsEmpty() == FALSE) {

			auto pItemProp = it->GetProp();
			if (pItemProp == nullptr) {
				// item not found?
				continue;
			}
			DWORD byID = 0;
			if (pBot->CreateItem(&(*it), &byID) == FALSE) {
				FLERROR_LOG(PROGRAM_NAME, "%s: Could not add Item %d to Bots Inventoy. Bot Name: %s", __FUNCTION__,
					pItemProp->dwID, pBot->GetName());
			}
			else {
				nSizeInventoryUser++;
				//if ()	
				{
					auto pItemElem = pBot->m_Inventory.GetAtId(byID);
					if (pItemElem == nullptr) {
						FLERROR_LOG(PROGRAM_NAME, "%s: Could not equip Item %d. Bot Name: %s", __FUNCTION__,
							pItemProp->dwID, pBot->GetName());
					}
					else {
						//CMover::DoEquipo
						if (pBot->IsEquipAble(pItemElem) == FALSE) {
							FLERROR_LOG(PROGRAM_NAME, "%s: IsEquipAble -> ret FALSE: Could not equip Item %d. Bot Name: %s",
								__FUNCTION__, pItemProp->dwID, pBot->GetName());
							continue;
						}
						pBot->EquipItem(pItemElem, TRUE, pItemProp->dwParts);
					}
				}
			}
		}
	}

	return (vecInventory.size() == nSizeInventoryUser);
}
bool user_bot::CBotMng::createMover(CBot* pBot, CWorld* pWorld) {
	// player!
	pBot->m_bPlayer = TRUE;

	// dont spawn at the given point
	D3DXVECTOR3 vPosStart =pBot->getBotType()->getSpawnPos();
	CRect rct = {
		static_cast<LONG>(vPosStart.x - 10),
		static_cast<LONG>(vPosStart.z - 10),
		static_cast<LONG>(vPosStart.x + 10),
		static_cast<LONG>(vPosStart.z + 10)
	};

	// check spawn pos
	int nWidth = rct.right - rct.left;
	if (nWidth <= NULL)
		nWidth = 1;
	int nHeight = rct.bottom - rct.top;
	if (nHeight <= NULL)
		nHeight = 1;
	// set mover index etc.
	pBot->SetIndex(nullptr, pBot->getBotType()->getMoverID(), FALSE);
	// set motion
	pBot->SetMotion(MTI_STAND);
	// speed
	pBot->m_pActMover->m_fSpeed = pBot->GetProp()->fSpeed;

#ifndef __M_USER_BOTS_DB_BASE
	// set playerid
	pBot->m_idPlayer = m_nIDStart + m_vecBots.size();
#endif

	// init properties (propMover)
	pBot->InitProp();

	// set spawn position
	pBot->SetPos(D3DXVECTOR3(static_cast<float>(rct.left + xRandom(nWidth))
		, vPosStart.y, static_cast<float>(rct.top + xRandom(nHeight))));
	// update matrix
	pBot->UpdateLocalMatrix();

	// set level
	pBot->m_nLevel = (pBot->getBotType()->getLevel());
	// set job
	pBot->m_nJob = pBot->getBotType()->getJob();
	pBot->m_Stat.SetOriginSta(pBot->GetRemainGP());
	//pBot->m_nRemainGP = 0;
	pBot->m_Stat.SetRemainStatPoint(0);
	// set hp,mp,fp to max
	pBot->SetHitPoint(pBot->GetMaxHitPoint());
	pBot->SetManaPoint(pBot->GetMaxManaPoint());
	pBot->SetFatiguePoint(pBot->GetFatiguePoint());
	pBot->AddItToGlobalId();
	if (pWorld->ADDOBJ(pBot, FALSE, 0) == FALSE) {
		// couldnt add bot to world
		return false;
	}
	pBot->UpdateRegionAttr();
	return true;
}

void user_bot::CBotMng::process() {
#ifndef __M_USER_BOTS_DB_BASE
	if (getCurrentBotCount() < getMaxBotCount()) {
		if (m_vecBots.size() < m_nMaxBotCount
			&& m_nNextBotAddTick < time(nullptr)) {
			sendAddBot();
			m_nNextBotAddTick = time(nullptr) + xRandom(m_nMaxSpawnDelay);
		}
	}
	for (auto it = m_vecBots.begin(); it != m_vecBots.end();) {
		if (GetTickCount() > MIN(2)
			&& m_nNextBotRemoveTick < time(nullptr)) {
			it = removeBot(it);
			m_nNextBotRemoveTick = time(nullptr) + xRandom(m_nMaxRemoveDelay);
			continue;
		}
		if ((*it) != nullptr)
			(*it)->processBot();
		it++;
	}
#else
	if (m_tStart == 0) {
		m_tStart = ::time(nullptr) + 30;
	}
	if (m_tStart > ::time(nullptr)) {
		return;
	}
	for (auto& it : m_vecBots) {
		if (!it->IsDelete()) {
			it->processBot();
		}
	}
#endif
}
#ifdef __M_USER_BOTS_RELOAD_CONFIG
void user_bot::CBotMng::removeAll() {
	m_tStart = ::time(nullptr) + 30;
	for (auto& it : m_vecBots) {
		if (!it->IsDelete()) {
			it->RemoveItFromGlobalId();
			it->_delete();
		}
	}
	m_vecBots.clear();
}
#endif

user_bot::CBotMng::_vecBots::iterator user_bot::CBotMng::removeBot(_vecBots::iterator& it) {
#ifndef __M_USER_BOTS_DB_BASE
	m_nameMng.recover((*it)->GetName());
#endif
#ifndef __M_USER_BOTS_DB_BASE
	g_DPCoreClient.SendRemoveBot((*it)->m_idPlayer, std::string((*it)->GetName()));
#endif
	//(*it)->RemoveItFromView();
	(*it)->RemoveItFromGlobalId();
	//(*it)->SetDelete(TRUE);
	(*it)->_delete();
	return m_vecBots.erase(it);
}

#ifndef __M_USER_BOTS_DB_BASE
void user_bot::CBotMng::sendAddBot() {
	// do "Add" stuff here later

	if (m_nameMng.vecUnused.size() <= 0) {
		Error("%s: No more bot-names available.", __FUNCTION__);
		return;
	}
	g_DPCoreClient.SendAddBot(m_vecBots.size() + m_nIDStart, m_nameMng.pick());
}
#else
void user_bot::CBotMng::removeBot(const u_long idBot) {
#ifdef __M_USER_BOTS_RELOAD_CONFIG
	if (idBot == 0) {
		this->removeAll();
		return;
	}
#endif
	auto iter = std::find_if(m_vecBots.begin(), m_vecBots.end(), [idBot](auto& bot) {
		if (idBot == bot->m_idPlayer) {
			return true;
		}
		return false;
	});
	if (iter != m_vecBots.end()) {
		removeBot(iter);
	}
}
#endif
#endif
user_bot::CBotMng::CBotMng() {
	init();
}

user_bot::CBotMng::~CBotMng() {
#ifdef __WORLDSERVER
	cleanUp();
#endif
}
void user_bot::CBotMng::init() {
	m_tStart = 0;
	m_nMaxBotCount =
		m_nIDStart =
#ifndef __M_USER_BOTS_DB_BASE
		m_nMaxRemoveDelay =
		m_nMaxSpawnDelay =
		m_nNextBotRemoveTick =
		m_nNextBotAddTick = 0;
#else
		0;
#endif
#if (_HAS_CXX17== TRUE) && defined(__WORLDSERVER)
	// add functions
	m_Funcs.set("buff", &user_bot::bot_cmd::buff);
	m_Funcs.set("collector", &user_bot::bot_cmd::collector);
	m_Funcs.set("move", &user_bot::bot_cmd::move);
#ifdef __M_USER_BOTS_IDLE
	m_Funcs.set("idle", &user_bot::bot_cmd::idle);
#endif
#elif defined(__WORLDSERVER)
	addBotFunc("buff", &user_bot::bot_cmd::buff);
	addBotFunc("collector", &user_bot::bot_cmd::collector);
	addBotFunc("move", &user_bot::bot_cmd::move);
#ifdef __M_USER_BOTS_IDLE
	addBotFunc("idle", &user_bot::bot_cmd::idle);
#endif
#endif
}
#ifdef __WORLDSERVER
bool	user_bot::CBotMng::handleMsg(BEHAVIOUR_DETAIL behaviour, CBot* pUser) {
	if (behaviour.str.size() <= 0) {
#ifndef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
		behaviour = *pUser->getBotType()->getCurrentBehaviour();
#else
		auto nIndex = pUser->getBehaviourIndex();
		if (nIndex == std::size_t(-1)) {
			nIndex = 0;
		}
		behaviour = *pUser->getBotType()->getCurrentBehaviour(nIndex);
#endif
	}
#if (_HAS_CXX17== TRUE)
	if (m_Funcs.exec(behaviour.str, behaviour, pUser) == true) {
		if (behaviour.nDuration == 0) {
			// erase first behaviour
#ifdef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
			pUser->getBotType()->eraseBehaviour(0, pUser->getBehaviourIndex());
#else
			pUser->getBotType()->eraseBehaviour(0);
#endif
		}
		return true;
	}
	return false;
#else
	if (this->callBotFunc(behaviour.str, behaviour, pUser) == true) {
		if (behaviour.nDuration == 0) {
			// erase first behaviour
#ifdef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
			pUser->getBotType()->eraseBehaviour(0, pUser->getBehaviourIndex());
#else
			pUser->getBotType()->eraseBehaviour(0);
#endif
		}
		return true;
	}
	return false;
#endif
}

bool user_bot::CBotMng::postMsg(const BEHAVIOUR_DETAIL& behaviour, CBot* pUser) {
	if (pUser->hasActivity() == true) {
		return false;
	}
	pUser->startActivity(behaviour.nDuration);
	if (handleMsg(behaviour, pUser) == false) {
		FLERROR_LOG(PROGRAM_NAME, "%s: behaviour: %s not found.", behaviour.str.c_str());
		return false;
	}
	return true;
}
#endif

bool user_bot::CBotMng::loadBehaviorScript() {
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
	m_nNextBotAddTick = time(nullptr) + m_nMaxSpawnDelay;
	m_nMaxRemoveDelay = time(nullptr) + m_nMaxRemoveDelay + 60;

	m_nameMng.vecUnused = lua.getArray<std::string>("g_BotNames");

	if (m_nMaxBotCount >= m_nameMng.vecUnused.size()) {
		Error("%s: You need to add more Bot-Names or decrease MAX_BOT_COUNT", __FUNCTION__);
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
			std::int64_t nDuration = static_cast<std::int64_t>(lua.GetFieldToNumber(-1, "nDuration"));
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
					std::int64_t nDuration = static_cast<std::int64_t>(lua.GetFieldToNumber(-1, "nDuration"));
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
			elem.m_nAbilityOption = (static_cast<int>(lua.GetFieldToNumber(-1, "nUpgradeLevel")));
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
bool user_bot::CBotMng::loadIndividualScript() {
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
		float			x = static_cast<float>(lua.GetFieldToNumber(-1, "x"));
		float			y = static_cast<float>(lua.GetFieldToNumber(-1, "y"));
		float			z = static_cast<float>(lua.GetFieldToNumber(-1, "z"));
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
					std::int64_t nDuration = static_cast<std::int64_t>(lua.GetFieldToNumber(-1, "nDuration"));
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
			elem.m_nAbilityOption = (static_cast<int>(lua.GetFieldToNumber(-1, "nUpgradeLevel")));
			elem.SetItemResist(static_cast<BYTE>(lua.GetFieldToNumber(-1, "nElement")));
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