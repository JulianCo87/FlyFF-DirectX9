#pragma once
#ifdef __M_USER_BOTS
namespace user_bot
{
	class CBot;
	class CBotMng;
#ifdef __WORLDSERVER
#define USERBOT_BEHAVIOUR_FUNC(func) void func(std::string strCmd, const BEHAVIOUR_DETAIL& behaviour, user_bot::CBot* pBot)
#endif
	struct BEHAVIOUR_DETAIL {
		// move
		std::string str;
		// do move until time(nullptr) + nDuration > time(nullptr)
		std::int64_t nDuration;
		BEHAVIOUR_DETAIL():nDuration(0) {

		}
		BEHAVIOUR_DETAIL(const std::string _strBehaviour, const std::int64_t _nDuration) {
			str = _strBehaviour;
			nDuration = _nDuration;
		}
		BEHAVIOUR_DETAIL(const BEHAVIOUR_DETAIL& _other) {
			str = _other.str;
			nDuration = _other.nDuration;
		}

		BEHAVIOUR_DETAIL& operator=(const BEHAVIOUR_DETAIL& _other) {
			str = _other.str;
			nDuration = _other.nDuration;
			return *this;
		}

	};

#ifdef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
	struct USER_BOT_LOGIN {
		using _vecBehaviour = std::vector<user_bot::BEHAVIOUR_DETAIL>;
		__time64_t	timeStart;
		// in seconds
		int			nOnlineTime;
		_vecBehaviour vecBehaviour;
#ifdef __M_USER_BOTS_LIMIT_TIME
		int			nMaxBots;
#endif
		void Serialize(CAr& ar) {
			if (ar.IsStoring()) {
				ar << timeStart << nOnlineTime << vecBehaviour;
#ifdef __M_USER_BOTS_LIMIT_TIME
				ar << nMaxBots;
#endif
			}
			else {
				ar >> timeStart >> nOnlineTime >> vecBehaviour;
#ifdef __M_USER_BOTS_LIMIT_TIME
				ar >> nMaxBots;
#endif
			}
		}
		USER_BOT_LOGIN& operator=(const USER_BOT_LOGIN& _other) {
			timeStart = _other.timeStart;
			nOnlineTime = _other.nOnlineTime;
			vecBehaviour = _other.vecBehaviour;
#ifdef __M_USER_BOTS_LIMIT_TIME
			nMaxBots = _other.nMaxBots;
#endif
			return *this;
		}
	};
#endif

	class CBotType {
	public:
		using vec_behaviour = std::vector<BEHAVIOUR_DETAIL>;
		using vec_inventory = std::vector<FLItemElem>;
#ifdef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
		using vec_onlineTime = std::vector<USER_BOT_LOGIN>;
#endif
		CBotType() { init(); }

		// copy & assign shit
		CBotType(const CBotType& other) {
			*this = other;
		}
		CBotType& operator=(const CBotType& other) {
			setSpawnWorld(other.getSpawnWorld());
#ifndef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
			setBotBehaviour(other.getBehaviour());
#else
			this->setOnlineTimes(other.m_vecLogins);
#endif
			setSpawnPos(other.getSpawnPos());
			setBotInventory(other.getBotInventory());
			setMoverID(other.getMoverID());
			setJob(other.getJob());
			setLevel(other.getLevel());
#ifdef __M_USER_BOTS_INDIVIDUAL
			if (other.isIndividual() || isIndividual()) {
				setIndividual(other.isIndividual());
				setName(other.getName());
#ifndef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
				setStartTime(other.getStartTime());
				setMaxOnlineTime(other.getMaxOnlineTime());
				setLoopCount(other.getLoopCount());
#else
				setOnlineTimes(other.getOnlineTimes());
#endif
			}
#endif
#ifdef __M_USER_BOTS_DB_BASE
			setServerKey(other.getServerKey());
#ifdef __DBSERVER
			setPlayerID(other.getPlayerID());
#endif
#endif
			return *this;
		}
#ifdef __M_USER_BOTS_DB_BASE
		void serialize(CAr& ar);
#endif
		// SETTER
#ifdef __M_USER_BOTS_INDIVIDUAL
		void	setName(const std::string& strName) { m_strName = strName; }
		void	setIndividual(const bool bIndividual) { m_bIndividual = bIndividual; }
#ifndef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
		void	setStartTime(const __time64_t& time) { m_tStart = time; }
		void	setMaxOnlineTime(const u_int nMinutes) { m_nTimeOnline = nMinutes; }
		void	setLoopCount(const u_int nLoop) { m_nLoop = nLoop; }

#else
		void	setOnlineTimes(const vec_onlineTime& vec) { 
			m_vecLogins.clear();
			m_vecLogins.insert(m_vecLogins.begin(), vec.begin(), vec.end());
		}
#endif
#endif
#ifdef __M_USER_BOTS_DB_BASE
		void	setServerKey(const u_long uKey)  {  m_uKey = uKey; }
#ifdef __DBSERVER
		void	setPlayerID(const u_long uId) {m_uId = uId;}
#endif
#endif
#ifdef __M_USER_BOTS_SPAWN_AT_TIME
		const std::size_t checkOnlineTime() const;
#endif
#ifdef __M_USER_BOTS_LIMIT_TIME
		const std::size_t getMaxBotsAtCurrentTime() const;
#endif
		void setBotType(const CBotType& other) {
			*this = other;
		}

		void setMoverID(const DWORD dwMoverID) {
			m_dwMoverID = dwMoverID;
		}
#ifndef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
		void					setBotBehaviour(const vec_behaviour& vecBehaviour) {
			m_vecBehaviour.clear();
			m_vecBehaviour.insert(m_vecBehaviour.begin(), vecBehaviour.begin(), vecBehaviour.end());
		}
#else
		void					setBotBehaviour(const std::size_t nOnlineIndex, const vec_behaviour& vecBehaviour) {
#ifdef _DEBUG
			_ASSERT(nOnlineIndex < m_vecLogins.size());
#endif
			m_vecLogins[nOnlineIndex].vecBehaviour.clear();
			m_vecLogins[nOnlineIndex].vecBehaviour.insert(m_vecLogins[nOnlineIndex].vecBehaviour.begin(),
				vecBehaviour.begin(), vecBehaviour.end());
		}

#endif
		void setBotInventory(const vec_inventory& inv) { m_vecInventory = inv; }
		void setSpawnPos(const D3DXVECTOR3& vPos) { m_vPos = vPos; }
		void setSpawnWorld(const DWORD dwWorldID) { m_dwWorldID = dwWorldID; }
		void setJob(const int nJob) { m_nJob = nJob; }
		void setLevel(const int nLevel) { m_nLevel = nLevel; }
		// GETTER
#ifndef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
		BEHAVIOUR_DETAIL*		findBehaviour(const std::string strBehavior) {
			for (auto it = m_vecBehaviour.begin(); it != m_vecBehaviour.end(); it++) {
				if (it->str == strBehavior) {
					return &(*it);
				}
			}
			return nullptr;
		}
#else
		BEHAVIOUR_DETAIL*		findBehaviour(const std::size_t nOnlineIndex,const std::string strBehavior) {
			for (auto it = m_vecLogins[nOnlineIndex].vecBehaviour.begin();
				it != m_vecLogins[nOnlineIndex].vecBehaviour.end(); it++) {
				if (it->str == strBehavior) {
					return &(*it);
				}
			}
			return nullptr;
		}
		USER_BOT_LOGIN::_vecBehaviour*		getAllBehaviours(std::size_t* pnCurrentIndex) {
#ifndef __M_USER_BOTS_FIX_BEHAVIOUR_INDEX
			for(std::size_t i = 0; i<m_vecLogins.size(); i++){
				if (m_vecLogins[i].timeStart >= time(nullptr))
				{
					*pnCurrentIndex = i;
					return &m_vecLogins[i].vecBehaviour;
				}
			}
			*pnCurrentIndex = std::size_t(-1);
#else
			*pnCurrentIndex = this->checkOnlineTime()-1;
			if (*pnCurrentIndex >= 0 && *pnCurrentIndex < m_vecLogins.size()) {
				return &m_vecLogins[*pnCurrentIndex].vecBehaviour;
			}
#endif
			return nullptr;
		}
		const USER_BOT_LOGIN::_vecBehaviour* getAllBehaviours(std::size_t* pnCurrentIndex) const {
#ifndef __M_USER_BOTS_FIX_BEHAVIOUR_INDEX
			for (std::size_t i = 0; i < m_vecLogins.size(); i++) {
				if (m_vecLogins[i].timeStart >= time(nullptr))
				{
					*pnCurrentIndex = i;
					return &m_vecLogins[i].vecBehaviour;
				}
			}
			*pnCurrentIndex = std::size_t(-1);
#else
			* pnCurrentIndex = this->checkOnlineTime() - 1;
			if (*pnCurrentIndex >= 0 && *pnCurrentIndex < m_vecLogins.size()) {
				return &m_vecLogins[*pnCurrentIndex].vecBehaviour;
			}
#endif
			return nullptr;
		}
#endif
		const vec_inventory&	getBotInventory() const { return m_vecInventory; }


#ifndef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
		const vec_behaviour&	getBehaviour()const { return m_vecBehaviour; }
		BEHAVIOUR_DETAIL*		getCurrentBehaviour() {
			if (m_vecBehaviour.size() <= 0) {
				return nullptr;
			}
			return &m_vecBehaviour[0];
		}
		BEHAVIOUR_DETAIL*	getNextBehaviour() {
			if (m_vecBehaviour.size() <= 1) {
				return getCurrentBehaviour();
			}
			// get next behaviour
			return &m_vecBehaviour[1];
		}

#else
		const vec_behaviour&	getBehaviour(const std::size_t nOnlineIndex)const { 
#ifdef _DEBUG
			_ASSERT(nOnlineIndex < m_vecLogins.size());
#endif
			return m_vecLogins[nOnlineIndex].vecBehaviour;
		}
		BEHAVIOUR_DETAIL*		getCurrentBehaviour(const std::size_t nOnlineIndex) {
#ifdef _DEBUG
			_ASSERT(nOnlineIndex < m_vecLogins.size());
#endif
			if (m_vecLogins[nOnlineIndex].vecBehaviour.size() <= 0) {
				return nullptr;
			}
			return &m_vecLogins[nOnlineIndex].vecBehaviour[0];
		}
		BEHAVIOUR_DETAIL*	getNextBehaviour(const std::size_t nOnlineIndex) {
#ifdef _DEBUG
			_ASSERT(nOnlineIndex < m_vecLogins.size());
#endif
			if (m_vecLogins[nOnlineIndex].vecBehaviour.size() <= 1) {
				return getCurrentBehaviour(nOnlineIndex);
			}
			// get next behaviour
			return &m_vecLogins[nOnlineIndex].vecBehaviour[1];
		}
#endif

#ifndef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
		void					eraseBehaviour(const std::size_t nIndex) {
			m_vecBehaviour.erase(m_vecBehaviour.begin() + nIndex);
		}

#else
		void					eraseBehaviour(const std::size_t nIndex, const std::size_t nIndexOnline) {
			m_vecLogins[nIndexOnline].vecBehaviour.erase(m_vecLogins[nIndexOnline].vecBehaviour.begin() + nIndex);
		}
#endif

		int						getLevel() const { return m_nLevel; }
		int						getJob() const { return m_nJob; }
		DWORD					getSpawnWorld() const { return m_dwWorldID; }
		DWORD					getMoverID() const { return m_dwMoverID; }
		D3DXVECTOR3				getSpawnPos() const { return m_vPos; }
#ifdef __M_USER_BOTS_INDIVIDUAL
		const std::string&		getName() const {return m_strName;}
		bool					isIndividual() const {return m_bIndividual;}

#ifndef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
		const __time64_t&		getStartTime() const { return m_tStart; }
		u_int					getMaxOnlineTime() const { return m_nTimeOnline; }
		u_int					getLoopCount() const { return m_nLoop; }
#else
		const vec_onlineTime&	getOnlineTimes() const { return m_vecLogins; }
#endif
#endif
#ifdef __M_USER_BOTS_DB_BASE
		u_long					getServerKey() const { return m_uKey; }
#ifdef __DBSERVER
		u_long					getPlayerID() const { return m_uId; }
#endif
#endif


		// add functions
#ifndef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
		void					addBehaviour(const std::string strBehaviour, const std::int64_t nDuration) {
			m_vecBehaviour.push_back({ strBehaviour, nDuration });
		}
#endif

		void					addBotItem(FLItemElem& elem) {
			m_vecInventory.push_back(elem);
		}

#ifdef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
		void					addOnlineTime(const USER_BOT_LOGIN& login) {
			m_vecLogins.push_back(login);
		}
#endif

		// process
		void processBotType();
	protected:

		void init();
#ifdef __M_USER_BOTS_DB_BASE
		u_long						m_uKey;
#ifdef __DBSERVER
		u_long						m_uId;
#endif
#endif
#ifdef __M_USER_BOTS_INDIVIDUAL
		bool						m_bIndividual;
		// only individual stuff
		std::string					m_strName;
#ifndef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
		__time64_t					m_tStart;
		u_int						m_nTimeOnline;
		u_int						m_nLoop;
#else
		std::vector<USER_BOT_LOGIN> m_vecLogins;
#endif
#endif
		int							m_nLevel;
		int							m_nJob;
		DWORD						m_dwMoverID;
		vec_inventory				m_vecInventory;
		D3DXVECTOR3					m_vPos;
		DWORD						m_dwWorldID;
#ifndef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
		vec_behaviour				m_vecBehaviour;
#endif
	};

#if defined(__BOT_ROUTING) && defined(__WORLDSERVER)
	namespace bot_route {
		using vec_waypoints = std::vector<D3DXVECTOR3>;
		class CAutoWalk {
			D3DXVECTOR3		m_vPosStart;
			D3DXVECTOR3		m_vPosEnd;
			vec_waypoints	m_vecWaypoints;
			// constructor


			D3DXVECTOR3 createWaypoints(CMover* pUser);
		public:
			CAutoWalk(D3DXVECTOR3 const& vPosStart,
				D3DXVECTOR3 const& vPosEnd)
				: m_vPosStart(vPosStart), m_vPosEnd(vPosEnd) {
			}


			auto getEndPos() const { return m_vPosEnd; }
			auto getStartPos() const { return m_vPosStart; }

			void process(CMover* pUser);

		};
	}
#endif

#ifdef __WORLDSERVER
	namespace bot_cmd {
		USERBOT_BEHAVIOUR_FUNC(buff);
		USERBOT_BEHAVIOUR_FUNC(collector);
		USERBOT_BEHAVIOUR_FUNC(move);
#ifdef __M_USER_BOTS_IDLE
		USERBOT_BEHAVIOUR_FUNC(idle);
#endif
	}
#endif
};
#endif