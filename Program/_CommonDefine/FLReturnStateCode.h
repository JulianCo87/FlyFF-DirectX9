
#ifndef __FLERRORCODEDEFINE_H__
#define __FLERRORCODEDEFINE_H__


enum	FLYFF_STATE_CODE
{
	FSC_GENERAL_FAIL							=	0
	, FSC_GENERAL_SUCCESS						=	1

	// 극단 초대 11 ~ 30
	, FSC_PARTY_INVITE_LEADER_NOT_EXIST			=	11						// 리더가 존재하지 않음
	, FSC_PARTY_INVITE_MEMBER_NOT_EXIST			=	12 						// 멤버가 존재하지 않음
	, FSC_PARTY_INVITE_MEMBER_HAVE_PARTY		=	13						// 멤버가 이미 극단 소속임
	, FSC_PARTY_INVITE_NOT_LEADER				= 	14						// 리더가 아님
	, FSC_PARTY_INVITE_MEMBER_OVERFLOW			= 	15						// 극단 인원 풀~
	, FSC_PARTY_INVITE_CANNOT_WORLD				= 	16						// 극단 생성 및 초대 불가 지역
	, FSC_PARTY_INVITE_GUILD_COMBAT				= 	17						// 길드 쟁 중에는 초대 불가
	, FSC_PARTY_INVITE_PVP						= 	18						// 플레이어간 듀얼 중에는 초대 불가
	, FSC_PARTY_INVITE_ATTACK_MODE				= 	19						// 사냥중일때는 초대 불가
	, FSC_PARTY_INVITE_SUCCESS					= 	20						// 극단 초대 성공

	// 아이템 판매 41 ~ 60
	, FSC_SELLITEM_NOT_PLAYER					= 	41						// 판매자가 플레이어가 아님
	, FSC_SELLITEM_PLAYER_DIE					= 	42						// 판매자가 죽은 상태
	, FSC_SELLITEM_NPC_NOT_EXIST				= 	43						// 구매자가 NPC가 아님
	, FSC_SELLITEM_INVALID_ITEM					= 	44						// 판매하려는 아이템이 유효하지가 않음
	, FSC_SELLITEM_INVALID_QUANTITY				= 	45						// 판매수량이 잘못되었음
	, FSC_SELLITEM_DO_NOT_SELL_ITEM				= 	46						// 판매 불가 아이템
	, FSC_SELLITEM_USING_ITEM					= 	47						// 사용중인 아이템
	, FSC_SELLITEM_EQUIP_ITEM					= 	48						// 장착중인 아이템
	, FSC_SELLITEM_GOLD_OVERFLOW				= 	49						// 판매금액이 소지할 수 있는 최대 금액을 넘어가버림
	, FSC_SELLITEM_SELL_SUCCESS					= 	50						// 아이템 판매 성공

	// 스킬 사용 71 ~ 90
	, FSC_SKILLUSE_FAIL							=	71
	, FSC_SKILLUSE_SKIP							=	72
	, FSC_SKILLUSE_SUCCESS						=	73

	// 아이템 업그레이드 101 ~ 150
	, FSC_ITEMUPGRADE_NOT_SUPPORT								=	101
	, FSC_ITEMUPGRADE_SYSTEM_ERROR
	, FSC_ITEMUPGRADE_UPGRADE_SUCCESS
	, FSC_ITEMUPGRADE_UPGRADE_FAIL
	, FSC_ITEMUPGRADE_UPGRADE_MINUS
	, FSC_ITEMUPGRADE_CHECK_SUCCESS
	, FSC_ITEMUPGRADE_DO_NOT_UPGRADE_ITEM
	, FSC_ITEMUPGRADE_DO_NOT_UPGRADE_USER_STATE
	, FSC_ITEMUPGRADE_DO_NOT_UPGRADE_AREA
	, FSC_ITEMUPGRADE_ITEM_DATA_INVALID
	, FSC_ITEMUPGRADE_MISMATCH_MATERIAL
	, FSC_ITEMUPGRADE_ENCHANT_SIZE_OVERFLOW
	, FSC_ITEMUPGRADE_ENCHANT_SIZE_UNDERFLOW
	, FSC_ITEMUPGRADE_MISMATCH_SELECT_OPTION
	, FSC_ITEMUPGRADE_MISMATCH_ITEM_LEVEL
	, FSC_ITEMUPGRADE_EQUIP_ITEM
	, FSC_ITEMUPGRADE_BIND_ITEM
	, FSC_ITEMUPGRADE_NOT_CHANGE_SELECT_OPTION
	, FSC_ITEMUPGRADE_NOT_EXIST_OPTION
	, FSC_ITEMUPGRADE_EXIST_OPTION
	, FSC_ITEMUPGRADE_NOT_ENOUGH_MONEY


	// 로그인 결과 1001 ~
	, FSC_LOGIN_CHECK_CLIENT_INFO_SUCCESS							=	1001
	, FSC_LOGIN_CHECK_CLIENT_INFO_RESOURCE_INVALID
	, FSC_LOGIN_CHECK_CLIENT_INFO_ACCOUNT_INVALID
	, FSC_LOGIN_CHECK_CLIENT_INFO_CLIENT_NOT_FOUND
	, FSC_LOGIN_INVALID_ACCOUNT
	, FSC_LOGIN_INVALID_PASSWORD
	, FSC_LOGIN_PREVENT_LOGIN_15SEC
	, FSC_LOGIN_PREVENT_LOGIN_15MIN
};


#endif // __FLERRORCODEDEFINE_H__
