#include "stdafx.h"
#ifdef __M_USER_BOTS

#include "UserBotBehaviour.h"
#ifndef __DBSERVER
#include "UserBot.h"
#else
#include "UserBotDBMng.h"
#endif
#ifdef __WORLDSERVER
#include "User.h"
#include "worldmng.h"
#include "World.h"
#include "defineItem.h"

//extern CUserMng		g_UserMng;
extern CWorldMng	g_WorldMng;


USERBOT_BEHAVIOUR_FUNC(user_bot::bot_cmd::buff) {
	pBot->startActivity(behaviour.nDuration);
	/*pBot->DoActiveSkill(46, 20, pBot, FALSE, 1337);
	pBot->DoActiveSkill(20, 20, pBot, FALSE, 1337);
	pBot->DoActiveSkill(49, 20, pBot, FALSE, 1337);
	pBot->DoActiveSkill(50, 20, pBot, FALSE, 1337);
	pBot->DoActiveSkill(52, 20, pBot, FALSE, 1337);
	pBot->DoActiveSkill(53, 20, pBot, FALSE, 1337);
	pBot->DoActiveSkill(114, 20, pBot, FALSE, 1337);
	pBot->DoActiveSkill(115, 20, pBot, FALSE, 1337);
	pBot->DoActiveSkill(116, 20, pBot, FALSE, 1337);*/
	pBot->endActivity();
}
USERBOT_BEHAVIOUR_FUNC(user_bot::bot_cmd::move) {
	CWorld* pWorld = pBot->GetWorld();
	pBot->SetDestPos(pBot->getDstPosition());
	//pBot->m_nCorr = -1;
	g_xWSUserManager->AddSetDestPos(pBot, pBot->getDstPosition(), 1);
	pBot->endActivity();
}

USERBOT_BEHAVIOUR_FUNC(user_bot::bot_cmd::collector) {
	// equip collector
	if (pBot->GetCollector() == nullptr) {
		FLItemElem elem;
		elem.m_dwItemId = II_GEN_TOO_COL_NORMAL;
		elem.m_nItemNum = 1;
		elem.m_nHitPoint = INT_MAX-1;

		DWORD byID = 0;
		if (pBot->CreateItem(&elem, &byID) == TRUE) {
			FLItemElem* pItemElem = pBot->m_Inventory.GetAtId(byID);
			if (pItemElem != nullptr) {
				FLItemElem* pWeapon = pBot->GetWeaponItem();
				if (pWeapon != nullptr) {
					pBot->EquipItem(pWeapon, FALSE, pWeapon->GetProp()->dwParts);
				}
				pBot->EquipItem(pItemElem, TRUE, pItemElem->GetProp()->dwParts);
			}
		}
	}

	if (!pBot->IsCollecting()) {
		pBot->StartCollecting();
	}
}
#ifdef __M_USER_BOTS_IDLE
USERBOT_BEHAVIOUR_FUNC(user_bot::bot_cmd::idle) {
	// do nothing.
}
#endif

#endif
void user_bot::CBotType::init() {
	setSpawnWorld(0);
	setSpawnPos(D3DXVECTOR3(0,0,0));
	setMoverID(0);
	setJob(0);
	setLevel(0);
#ifdef __M_USER_BOTS_INDIVIDUAL
	setName("");
#ifndef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
	setStartTime(0);
	setMaxOnlineTime(0);
	setLoopCount(0);
#endif
	setIndividual(false);
#endif
}


#ifdef __M_USER_BOTS_DB_BASE
void user_bot::CBotType::serialize(CAr& ar) {
	if (ar.IsStoring()) {
		ar << m_dwWorldID;
#ifndef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
		ar << m_vecBehaviour;
#else
		ar << m_vecLogins;
#endif
		ar << m_vPos;
		ar << m_vecInventory;
		ar << m_dwMoverID;
		ar << m_nJob;
		ar << m_nLevel;
#ifdef __M_USER_BOTS_INDIVIDUAL
		ar << m_bIndividual;
		if (m_bIndividual) {
#ifndef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
			ar << m_strName;
			ar << m_tStart;
			ar << m_nTimeOnline;
			ar << m_nLoop;
#else
			ar << m_strName;
#endif
		}
#ifdef __M_USER_BOTS_DB_BASE
		ar << m_uKey;
#ifdef __DBSERVER
		ar << m_uId;
#endif //__DBSERVER 
#endif // __M_USER_BOTS_DB_BASE
#endif // __M_USER_BOTS_INDIVIDUAL
	}
	else {
		ar >> m_dwWorldID;
#ifndef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
		ar >> m_vecBehaviour;
#else
		ar >> m_vecLogins;
#endif
		ar >> m_vPos;
		ar >> m_vecInventory;
		ar >> m_dwMoverID;
		ar >> m_nJob;
		ar >> m_nLevel;
#ifdef __M_USER_BOTS_INDIVIDUAL
		ar >> m_bIndividual;
		if (m_bIndividual) {
#ifndef __M_USER_BOTS_MULTIPLE_LOGIN_DAILY
			ar >> m_strName
			ar	>> m_tStart
			ar	>> m_nTimeOnline
			ar	>> m_nLoop;
#else
			ar >> m_strName;
#endif
		}
#ifdef __M_USER_BOTS_DB_BASE
		ar >> m_uKey;
#ifdef __DBSERVER
		ar >> m_uId;
#endif //__DBSERVER 
#endif // __M_USER_BOTS_DB_BASE
#endif // __M_USER_BOTS_INDIVIDUAL
	}
}
#endif

#ifdef __M_USER_BOTS_SPAWN_AT_TIME
const std::size_t user_bot::CBotType::checkOnlineTime() const {
	const auto currTime = ATL::CTime::GetCurrentTime();
	for(auto i = 0U; i<m_vecLogins.size(); i++){
		CTime timeStartRes = CTime(m_vecLogins[i].timeStart);// 16:30
		CTime timeCurrentDayStart = CTime(currTime.GetYear(), currTime.GetMonth(),
			currTime.GetDay(), timeStartRes.GetHour(), timeStartRes.GetMinute(), 0);
		if(currTime >= timeCurrentDayStart && currTime < (timeCurrentDayStart + m_vecLogins[i].nOnlineTime)){
			return i+1;
		}
	}
	return 0;
}
#endif

#ifdef __M_USER_BOTS_LIMIT_TIME
const std::size_t user_bot::CBotType::getMaxBotsAtCurrentTime() const {
	const auto i = this->checkOnlineTime();
	if (i > 0) {
		return m_vecLogins[i - 1].nMaxBots;
	}
	return false;
}
#endif

void user_bot::CBotType::processBotType() {

}
#ifdef __WORLDSERVER
#ifdef __BOT_ROUTING
D3DXVECTOR3 user_bot::bot_route::CAutoWalk::createWaypoints(CMover* pMover) {
	auto vPosCurrent = pMover->GetPos();
	auto vDist = getEndPos() - vPosCurrent;
	float fDistSq = D3DXVec3LengthSq(&vDist);
	D3DXVECTOR3	vIntersect, vSlide;

	Segment3 seg3(pMover->GetPos(), getEndPos());

	CObj* pObjIter = nullptr;
	CWorld* pWorld = pMover->GetWorld();
	CModelObject* pModel = dynamic_cast<CModelObject*>(pMover->GetModel());
	// do the real stuff
	{
		FOR_LINKMAP(pWorld, vPosCurrent, pObjIter, 0, CObj::linkStatic, pMover->GetLayer()) {
			if (pModel->TestIntersectionOBB_Line(seg3, pObjIter) == TRUE) {
				if (pModel->GetObject3D()->SlideVectorXZ2(&vSlide, &vIntersect,
					vPosCurrent, getEndPos(), pObjIter->GetMatrixWorld(), 0) != TRUE) {
					continue;
				}
				auto vPosObj = pObjIter->GetPos();
				//
				float fAngle = GetDegree(vPosCurrent, vPosObj) + 45.0f;

			}
		}
	}
}



void user_bot::bot_route::CAutoWalk::process(CMover* pMover) {
}
#endif	// __WORLDSERVER
#endif
#endif