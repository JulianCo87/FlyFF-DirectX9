#pragma once
#include "StdAfx.h"
#ifdef __M_USER_BOTS
#if !defined(__M_USER_BOTS_DB_BASE)
#pragma message("__M_USER_BOTS_DB_BASE not defined......!!!!!!!!!!!!")")
#endif
#if !defined(__M_UTIL)
#pragma message("__MUTIL not defined...!!!!!!!!!!!!")
#endif
#ifdef __M_USER_BOTS_DB_BASE
#include "UserBotBehaviour.h"
#include "DPCoreSrvr.h"
#include <algorithm>
namespace user_bot {
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
			vecUsed.erase(vecUsed.begin() + nIndex);
			return true;
		}
	};
	class CBotDBMng {
		using _vec_bot = std::vector<user_bot::CBotType>;
		using _vec_ws = std::vector<std::pair<std::uint32_t, __time64_t>>;
		using _vec_bottype = std::vector<CBotType>;


	//	CMclCritSec					m_Lock;

		__time64_t					m_tStart;
		user_bot::bot_name_mng		m_nameMng;
		std::size_t					m_nMaxRemoveDelay;
		std::size_t					m_nMaxSpawnDelay;
		__time64_t					m_tNextBotRemoveTick;
		__time64_t					m_tNextBotAddTick;
		std::size_t					m_nMaxBotCount;
		std::size_t					m_nIDStart;
		_vec_ws						m_vecServerKeys;
		_vec_bottype				m_vecBotType;
		// all bots on all servers
		_vec_bot					m_vecBots;
		std::size_t					m_nCurrIndex;


		_vec_ws::iterator			findServer(const u_long uKey);
		_vec_bot::iterator			findBot(const u_long idBot);
		std::size_t					removeBotsByServerKey(const u_long uKey);
		void						removeBot(_vec_bot::iterator iter);
		std::size_t					selectBotType();
		_vec_ws::iterator			selectServer();
		u_long						selectPlayerID();

	public:

		CBotDBMng() : m_nMaxBotCount(0), m_tNextBotAddTick(0),
			m_tNextBotRemoveTick(0), m_nMaxSpawnDelay(0),
			m_nMaxRemoveDelay(0), m_nIDStart(0), m_tStart(0), m_nCurrIndex(0){

		}

		//TODO: WS-> CORE -> DB (KEYS!!!!!)

		// static pointer to this class
		static auto getInstance() {
			static CBotDBMng s;
			return &s;
		}
		// CSingleTon stuff
		//CMclCritSec* GetLock() { return &m_Lock; }


		auto			getBotNameMng() { return &m_nameMng; }
		std::size_t		getCurrentBotCount() const { return m_vecBots.size(); }
		std::size_t		getMaxBotCount() const { return m_nMaxBotCount; }



		void			addBot();
		void			process();



		void			removeServer(const u_long uKey);
		bool			isBotName(const std::string& strName);

		void			addServer(const u_long uKey) {
			m_vecServerKeys.push_back({ uKey, ::time(nullptr) });
		}
		bool			isServerKey(const u_long uKey) {
			return (findServer(uKey) != m_vecServerKeys.end());
		}

		bool loadBehaviorScript();

#ifdef __M_USER_BOTS_RELOAD_CONFIG
		void reload();
#endif
#ifdef __M_USER_BOTS_INDIVIDUAL
		bool loadIndividualScript();
#endif
	};
}


#endif
#endif