#pragma once
#ifdef __M_USER_BOTS
#include "UserBotBehaviour.h"
#if defined(__WORLDSERVER) || defined(__DBSERVER)
#ifdef __WORLDSERVER
#include "User.h"
#endif
namespace user_bot
{
#ifdef __WORLDSERVER
	// using CUser
	class CBot : public FLWSUser{
#ifdef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
		std::size_t		m_nBehaviourIndex;
#endif
		CBotType		m_BotType;
		__time64_t		m_tStartLast;
		D3DXVECTOR3		m_vPosMoveTo;
	public:
		CBot();
		CBot(const CBotType& botType);

		~CBot();

#ifdef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
		void setBehaviourIndex(const std::size_t nIndex) {m_nBehaviourIndex = nIndex;}
		auto getBehaviourIndex() const { return m_nBehaviourIndex; }
#endif
		auto getBotType(){ return &m_BotType; }
		const auto getBotType()const  { return &m_BotType; }
#ifdef __BOT_ROUTING
		void presupposePos(D3DXVECTOR3* pv, D3DXVECTOR3* pvd, float* pf, u_long uTickCount);
#endif
		void startActivity(const std::int64_t nSeconds);
		bool endActivity();
		void setDstPosition(const D3DXVECTOR3& vPos){ m_vPosMoveTo = vPos;}
		auto getDstPosition() const { return m_vPosMoveTo; }
		bool execBehaviour(const BEHAVIOUR_DETAIL* pBehaviour);
		bool hasActivity() const;
		auto getNextActivity();

		void processBot();

		void _delete();
		void init();

#ifdef __M_USER_BOTS_INDIVIDUAL
		bool isIndividual() const {
			return m_BotType.isIndividual();
		}
#endif
	};
#endif
#ifndef __M_USER_BOTS_DB_BASE
	struct bot_name_mng {
	private:
		using _str = std::string;
		using _c_str = const _str;
	public:
		std::vector<_str>	vecUsed;
		std::vector<_str>	vecUnused;


		std::size_t isUsed(_c_str& str) {
			for (auto it = vecUsed.begin(); it != vecUsed.end(); it++) {
				if ((*it) == str) {
					return (it - vecUsed.begin());
				}
			}
			return std::size_t(-1);
		}
		std::size_t isUnused(_c_str& str) {
			for (auto it = vecUnused.begin(); it != vecUnused.end(); it++) {
				if ((*it) == str) {
					return (it - vecUnused.begin());
				}
			}
			return std::size_t(-1);
		}
		_str& pick() {
#ifdef _DEBUG
			_ASSERT(vecUnused.size());
#endif
			return vecUnused[xRandom(vecUnused.size())];
		}
		bool use(_c_str& strName) {
			std::size_t nIndex = isUnused(strName);
			if (nIndex == std::size_t(-1)) { // couldnt be used.
				return false;
			}
			vecUsed.push_back(vecUnused[nIndex]);
			vecUnused.erase(vecUnused.begin() + nIndex);
			return true;
		}
		bool recover(_c_str& strName) {
			std::size_t nIndex = isUsed(strName);
			if (nIndex == std::size_t(-1)) { // couldnt be restored.
				return false;
			}
			vecUnused.push_back(vecUsed[nIndex]);
			vecUsed.erase(vecUsed.begin()+ nIndex);
			return true;
		}
	};
#endif
	class CBotMng {
	public:
#ifdef __WORLDSERVER
#if _HAS_CXX17
		using str_to_function = mu::args::StringToFunction<void,
			const user_bot::BEHAVIOUR_DETAIL&, user_bot::CBot*>;
#else


#endif
		using						_vecBots = std::vector<CBot*>;

		// lock thread, singleton stuff
		//CMclCritSec					m_Lock;
#endif
	private:
#ifndef __M_USER_BOTS_DB_BASE
		std::size_t					m_nMaxRemoveDelay;
		std::size_t					m_nMaxSpawnDelay;
		std::size_t					m_nNextBotRemoveTick;
		std::size_t					m_nNextBotAddTick;
		bot_name_mng				m_nameMng;
#endif
#ifdef __WORLDSERVER
		__time64_t					m_tStart;
		_vecBots					m_vecBots;
		std::size_t					m_nMaxBotCount;
		std::vector<CBotType>		m_vecBotType;
		std::size_t					m_nIDStart;
#endif
#if (_HAS_CXX17== TRUE) && defined(__WORLDSERVER)
		// Behaviour commands
		str_to_function				m_Funcs;
#elif (_HAS_CXX17 == FALSE) && defined(__WORLDSERVER)
		 struct str_function {
			 using _func = std::function<void(const std::string, const user_bot::BEHAVIOUR_DETAIL&, user_bot::CBot*)>;
			std::string strName;
			// first is our command
			_func Func;

			_func& operator()() {return Func;}
		};
	protected:
		std::vector<str_function> m_vecFunctions;
		str_function* getBotFunction(const std::string& strFunc) {
			for (auto it = m_vecFunctions.begin(); it != m_vecFunctions.end(); it++) {
				if ((*it).strName == strFunc) {
					return &(*it);
				}
			}
			return nullptr;
		}
		void addBotFunc(const std::string strFunc, str_function::_func func) {
			m_vecFunctions.push_back({ strFunc, func });
		}
		template <typename ... ArgFunc> 
		bool callBotFunc(const std::string& strFunc, ArgFunc... arg) {
			auto pFunc = this->getBotFunction(strFunc);
			if (pFunc == nullptr) {
				return false;
			}
			(*pFunc)()(strFunc, arg...);
			return true;
		}
	public:
		const str_function* findBotFunction(const std::string& strFunc) {
			return this->getBotFunction(strFunc);
		}
#endif
		void						init();
#ifdef __WORLDSERVER
		bool handleMsg(BEHAVIOUR_DETAIL behaviour, CBot* pUser);
		void cleanUp();

#ifdef __M_USER_BOTS_RELOAD_CONFIG
		void removeAll();
#endif
		_vecBots::iterator removeBot(_vecBots::iterator& it);
		void sendAddBot();
#endif
	public:

		CBotMng();
		~CBotMng();

#ifdef __WORLDSERVER
		std::size_t getCurrentBotCount() const { return m_vecBots.size(); }
		std::size_t getMaxBotCount() const { return m_nMaxBotCount; }
		bool isBot(const u_long id) { 
			return (getBot(id) != nullptr);
		}

		const CBot* getBot(const u_long id) const {
			int nRealID = static_cast<int>(id - m_nIDStart);
			if (nRealID < 0) {
				return nullptr;
			}
			return m_vecBots.size() > 0 ? m_vecBots[nRealID] : nullptr;
		}
		CBot* getBot(const u_long id) {
			int nRealID = static_cast<int>(id - m_nIDStart);
			if (nRealID < 0) {
				return nullptr;
			}
			return m_vecBots.size() > 0 ? m_vecBots[nRealID] : nullptr;
		}
		CBot* getBot(const std::string& strName)  {
			auto iter = std::find_if(m_vecBots.begin(), m_vecBots.end(), [&strName](auto& bot) {
				if (strName == bot->GetName()) {
					return true;
				}
				return false;
			});
			return (iter != m_vecBots.end() ? (*iter) : nullptr);
		}
		void process();
#ifndef __M_USER_BOTS_DB_BASE
		void addBot(const u_long idBot, const std::string& strName);
#else
		void addBot(const u_long idBot, const std::string& strName, const std::size_t nBotType);
#endif
		bool postMsg(const BEHAVIOUR_DETAIL& behaviour, CBot* pUser);
		bool createMover(CBot* pUser, CWorld* pWorld);
		bool createInventory(CBot* pUser);
#ifdef __M_USER_BOTS_DB_BASE
		void removeBot(const u_long idBot);
#endif
		// static pointer to this class
		static auto getInstance() {
			static CBotMng s;
			return &s;
		}
		// mu::CSingleTon stuff
		//::CMclCritSec* GetLock() { return &m_Lock; }

		CBotMng(const CBotMng&) = delete;
		CBotMng& operator = (const CBotMng &) = delete;
#endif

#ifdef __M_USER_BOTS_INDIVIDUAL
		bool loadIndividualScript();
#endif
		bool loadBehaviorScript();
#ifndef __M_USER_BOTS_DB_BASE
		auto getBotNameMng() { return &m_nameMng; }

		bool isBotName(const std::string& strName) {
			return (m_nameMng.isUsed(strName) != std::size_t(-1)
				|| m_nameMng.isUnused(strName) != std::size_t(-1));
		}
#endif
	};
};
#endif
#endif