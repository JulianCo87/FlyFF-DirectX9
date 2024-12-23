#ifndef __DPSRVR_H__
#define __DPSRVR_H__

#pragma once

#include <network/FLDPMng.h>
#include <MsgHdr.h>
#include "Snapshot.h"


class CDPSrvr : public CDPMng
{
public:
	// Constructions
	CDPSrvr();
	virtual	~CDPSrvr();

	// Operations
	virtual	void SysMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom );
	virtual void UserMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom );

	USES_PFNENTRIES( CDPSrvr, ( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize ) );

	// Handlers
	void	OnAddConnection( DPID dpid );
	void	OnRemoveConnection( DPID dpid );
	void	OnAddUser( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnRemoveUser( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnChat( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnDoEquip( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnCtrlCoolTimeCancel( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );		
	void	OnMoveItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
#ifdef INVENTORY_ITEM_ALIGN
	void	OnAlignItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
#endif	// INVENTORY_ITEM_ALIGN
	void	OnDropItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnDropGold( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnTrade( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnConfirmTrade( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnConfirmTradeCancel( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnTradePut( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnTradePull( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnTradePutGold( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnTradeClearGold( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnTradeCancel( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnTradeOk( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnTradelastConfrim( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnPVendorOpen( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnPVendorClose( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnRegisterPVendorItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnUnregisterPVendorItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnQueryPVendorItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnBuyPVendorItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnEnchant( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize);

	void	OnSmeltSafety( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize);

	void	OnRemoveAttribute( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize);

	void	OnChangeAttribute( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpbuf, u_long uBufSize );

	void	OnCreateSfxObj( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize);
	void	OnPiercing( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize);
	void	OnPiercingSize( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize);

	void	OnPiercingRemove( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize);

	void	OnExpBoxInfo( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize);
	void	OnItemTransy( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnUpgradeBase( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnCommercialElem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnMotion( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnRepairItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnScriptDialogReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnReplace( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnOpenShopWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnCloseShopWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnBuyItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void	OnBuyChipItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void	OnSellItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnOpenBankWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnOpenGuildBankWnd(CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnCloseGuildBankWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnPutItemGuildBank( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnGetItemGuildBank( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnGuildBankMoveItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize);
	void	OnCloseBankWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnDoUseSkillPoint( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnBankToBank( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnPutItemBank( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnGetItemBank( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnPutGoldBank( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpbuf, u_long uBufSize );
	void	OnGetGoldBank( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpbuf, u_long uBufSize );
	void	OnMoveBankItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize);
	void	OnChangeBankPass( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnConfirmBank( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnRevivalBySkill( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnRevivalCurrentPos( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnRevivalLodestar( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnRevivalLodelight( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize ); 
	void	OnSetLodelight( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnPlayerBehavior( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnPlayerSetDestObj( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnCorrReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnCreateGuildCloak( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
// 	void	OnQueryGetPos( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
// 	void	OnGetPos( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnQueryGetDestObj( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnGetDestObj( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnSkillTaskBar( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnAddAppletTaskBar( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnRemoveAppletTaskBar( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnAddItemTaskBar( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnRemoveItemTaskBar( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnPartyRequest( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnAddFriendNameReqest( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnPartyRequestCancle( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnConfirmPartyRequest( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnPartySkillUse( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnAddFriendReqest( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnAddFriendCancel( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnGetFriendName( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnDuelRequest( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnDuelYes( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnDuelNo( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnDuelPartyRequest( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnDuelPartyYes( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnDuelPartyNo( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnMoverFocus( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnRemoveQuest( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void	OnQueryPlayerData( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnQueryPlayerData2( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void	OnAvailPocket( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnMoveItemOnPocket( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void	OnQuePetResurrection( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void	OnArenaEnter( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnArenaExit( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void	OnGuildInvite( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnIgnoreGuildInvite( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnEndSkillQueue( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnActionPoint( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnDoUseItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
//	void	OnDoApplyItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void	OnDoUseItemTarget( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
//	void	OnAwakening( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
//	void	OnRemoveItemLevelDown( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
//	void	OnBlessednessCancel( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void	OnMeleeAttack( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnMeleeAttack2( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnMagicAttack( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnRangeAttack( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnUseSkill( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnSfxID( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnSfxHit( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnSfxClear( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnSnapshot( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnSetTarget( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
//	void	OnDoCollect( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnTeleSkill( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnPlayerPos( CAr & ar, FLWSUser* pUser );
	void	OnPlayerDestPos( CAr & ar, FLWSUser* pUser );
//	void	OnPlayerDestAngle( CAr & ar, FLWSUser* pUser );
	void	OnModifyMode( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
//	void	OnSummonPlayer( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
//	void	OnTeleportPlayer( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnExpUp( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnChangeJob( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnSetHair( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnBlock( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnError( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize);
	void	OnShipActMsg( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnLocalPosFromIA( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnGuildLogo( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnGuildContribution( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnGuildNotice( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnRequestGuildRank( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnEnterChattingRoom( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnChatting( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnOpenChattingRoom( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnCloseChattingRoom( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnCommonPlace( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnSetNaviPoint( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnGameMasterWhisper( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnNWWantedGold( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnNWWantedList( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnNWWantedInfo( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnNWWantedName( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnReqLeave( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnChangeMode( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnStateMode( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnQuerySetPlayerName( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnQuerySetGuildName( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnDoEscape( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnCheering( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnQueryEquip( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void	OnOptionEnableRenderMask( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnOptionEnableRenderCostume( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void	OnQueryEquipSetting( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnFoucusObj( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnReturnScroll ( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnChangeFace( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnQueryPostMail( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnQueryRemoveMail( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnQueryGetMailItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnQueryGetMailGold( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnQueryReadMail( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnQueryMailBox( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnGCApp( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnGCCancel( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnGCRequestStatus( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnGCSelectPlayer( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnGCSelectMap( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnGCJoin( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnGCGetPenyaGuild( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnGCGetPenyaPlayer( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void	OnGCTele( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnGCPlayerPoint( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnSummonFriend( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnSummonFriendConfirm( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnSummonFriendCancel( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnSummonParty( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnSummonPartyConfirm( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void	OnRemoveInvenItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnRandomScroll( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	
	void	OnCreateMonster( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void	OnBuyingInfo( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	
	void	OnCreateAngel(CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnAngleBuff(CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	
	void OnKawibawiboStart( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnKawibawiboGetItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnReassembleStart( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnReassembleOpenWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnAlphabetOpenWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnAlphabetStart( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnFiveSystemOpenWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnFiveSystemBet( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnFiveSystemStart( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnFiveSystemDestroyWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void OnUltimateMakeItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnUltimateMakeGem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnUltimateTransWeapon( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnUltimateSetGem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnUltimateRemoveGem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnUltimateEnchantWeapon( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	
//	void OnExchange( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void	OnPetRelease( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnUsePetFeed( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnMakePetFeed( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnPetTamerMistake( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnPetTamerMiracle( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnFeedPocketInactive( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void	OnQueryStartCollecting( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnQueryStopCollecting( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void OnLegendSkillStart( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	
	void OnModifyStatus( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void OnGC1to1TenderOpenWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnGC1to1TenderView( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnGC1to1Tender( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnGC1to1CancelTender( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnGC1to1FailedTender( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnGC1to1MemberLineUpOpenWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnGC1to1MemberLineUp( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnGC1to1TeleportToNPC( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnGC1to1TeleportToStage( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void OnQueryGuildBankLogList( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void OnSealCharReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnSealCharConmReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnSealCharGetReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void OnHonorListReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnHonorChangeReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void OnNPCBuff( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void OnSecretRoomTenderOpenWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnSecretRoomLineUpOpenWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnSecretRoomTender( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnSecretRoomTenderCancelReturn( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnSecretRoomLineUpMember( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnSecretRoomEntrance( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnSecretRoomTeleportToNPC( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnSecretRoomTenderView( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void OnTeleportSecretRoomDungeon( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	BOOL IsInviteAbleGuild( FLWSUser* pUser );	//길드원 변경가능한 길드인가?(초대 및 탈퇴)

	void OnSetTaxRate( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void OnTeleportToHeavenTower( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

public:
	void	QueryDestroyPlayer( DPID dpidCache, DPID dpidUser, DWORD dwAuthKey, u_long idPlayer );
	void	SendPacket( DPID dwDPIDCache, DPID dwDPIDUser, const FLPacket* pPacket );
	void	PutCreateItemLog( FLWSUser* pUser, FLItemElem* pItemElem, const char* szAction, const char* recv ) ;
	static void	OnLogItem( LogItemInfo & info, FLItemElem* pItemElem = NULL, int nItemCount = 0 );
	void	InviteParty( u_long uLeaderid, u_long uMemberid, BOOL bTroup );
	void	InviteCompany( FLWSUser* pUser, OBJID objid );
	void	UpdateGuildBank(CGuild* p_GuildBank, int p_Mode, BYTE cbUpdate = 0, u_long idPlayer = 0, FLItemElem* pItemElem = NULL, DWORD dwPenya = 0, int nItemCount = 0 );
	BOOL	ClosePVendor( FLWSUser* pUser, OBJID objidVendor );
	BOOL	IsPiercingSize( FLWSUser* pUser, FLItemElem* pItemElem0, FLItemElem* pItemElem1, FLItemElem* pItemElem2, int& nError );

// 	BOOL	DoUseItemTarget_GenRandomOption( FLWSUser* pUser, FLItemElem* pTarget, int nKind,
// 		int nOk, int nMismatchKind, int nHasOption,
// 		const char* szOperation, const char* szReceive );
// 	BOOL	DoUseItemTarget_InitializeRandomOption( FLWSUser* pUser, FLItemElem* pTarget, int nKind,
// 		int nOk, int nError,
// 		const char* szOperation, const char* szRecv );
//	BOOL	DoUseItemTarget_ItemLevelDown( FLWSUser* pUser, FLItemElem* pMaterial, FLItemElem* pTarget );

public:
	static void	PutItemLog( FLWSUser* pUser, const char* szAction, const char* szContext, FLItemElem* pItem, int nNum = 0 );
	static void	PutPenyaLog( FLWSUser* pUser, const char* szAction, const char* szContext, int nCost );

	static void	SendPutItemLog( FLWSUser* pUser, const TCHAR* pszAction, const TCHAR* pszSendName, const TCHAR* pszRecvName, FLItemElem* pItemElem, const int nQuantity );
	static void	SendPutPenyaLog( FLWSUser* pUser, const TCHAR* pszAction, const TCHAR* pszSendName, const TCHAR* pszRecvName, const int nPenya );

private:
	void	OnElectionAddDeposit( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnElectionSetPledge( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnElectionIncVote( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnLEventCreate( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
	void	OnLordSkillUse( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );

	void	OnTransformItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );		// 알변환 핸들러
//	void	OnPickupPetAwakeningCancel( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );		// 픽업펫 각성 취소

	void	OnTutorialState( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

	void	OnDoUseItemInput( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

	void	OnClearPetName( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

	void	OnRainbowRacePrevRankingOpenWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnRainbowRaceApplicationOpenWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnRainbowRaceApplication( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnRainbowRaceMiniGamePacket( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnRainbowRaceReqFinish( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

	void	OnHousingSetupFurniture( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnHousingSetVisitAllow( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnHousingVisitRoom( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnHousingVisitableList( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnHousingGoOut( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

	void	OnReqQuestNPCPos( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

	void	OnPropose( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnRefuse( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnCouple( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnDecouple( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

	void	OnMapKey( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

	void	OnQuizEventEntrance( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnQuizEventTeleport( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

	void	OnRemoveVis( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnSwapVis( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

	void	OnBuyGuildHouse( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnGuildHousePacket( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnGuildHouseEnter( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnGuildHouseGoOut( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

	void	OnTeleporterReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

	void	OnCheckedQuest( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

	void	OnInviteCampusMember( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnAcceptCampusMember( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnRefuseCampusMember( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnRemoveCampusMember( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );


	//	mulcom	BEGIN100405	각성 보호의 두루마리
	void	OnItemSelectAwakeningValue( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	//	mulcom	END100405	각성 보호의 두루마리

#ifdef __GUILD_HOUSE_MIDDLE
	void	OnReqGuildHouseTenderMainWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqGuildHouseTenderInfoWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqGuildHouseTenderJoin( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqGuildHouseInfoWnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqGuildHouseCommentChange( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
#endif // __GUILD_HOUSE_MIDDLE


//////////////////////////////////////////////////////////////////////////
// mirchang_100723 give coupon item event

	void	OnCouponNumber( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

// mirchang_100723 give coupon item event
//////////////////////////////////////////////////////////////////////////


#ifdef __HYPERLINK_ITEM16

	void	OnReqItemLinkInfo( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

#endif // __HYPERLINK_ITEM16


#ifdef __ENCHANT_BARUNA16

	void	OnReqExtractOper( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqCreateOper( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqCreateCid( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqUpgradeCidPiece( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqCreateOperCid( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

#endif // __ENCHANT_BARUNA16

	//////////////////////////////////////////////////////////////////////////
	// shopping basket
	void	OnReqBuyItemSoppingBasket( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// shopping basket
	void	OnReqResetBind( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	//////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////
	// 협동기부
	void	OnCooperativeContributions_GetInfoReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnCooperativeContributions_ContributeReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnCooperativeContributions_GetRanking( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	//void	OnCooperativeContributions_ContributeItemReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	//void	OnCooperativeContributions_ContributeGoldReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	
	/////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////
	// 콜로세움
	void	OnColosseum_Enter( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnColosseum_Leave( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnColosseum_Retry( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnColosseum_InviteAck( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnColosseum_ForceStart( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

	void	OnColosseum_RegistAdditionalRealMonsterRateItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnColosseum_UnRegistAdditionalRealMonsterRateItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

	void	OnColosseum_RankingInfo( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	/////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// item merge
	void	OnReqItemMerge( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// item upgrade
	void	OnReqUpgradeItemGeneralEnchant( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqUpgradeItemAttributeEnchant( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqUpgradeItemPiercingInsertItem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqUpgradeItemRandomOptionGenerate( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqUpgradeItemRandomOptionGenerateRetry( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqUpgradeItemRandomOptionInitialize( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqUpgradeItemEquipLevelDecrease( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqUpgradeItemEquipLevelInitialize( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqUpgradeItemCombine( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqUpgradeItemCombineInitializeData( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqUpgradeItemCombineInitialize( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqUpgradeItemLooksChange( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqUpgradeItemLooksInitialize( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
#ifdef BARUNA_ULTIMATE_UPDATE
	void	OnReqUpgradeItemUltimateTrans( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
#endif
#ifdef CARD_UPGRADE_SYSTEM
	void	OnReqUpgradeItemCard( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
#endif	// CARD_UPGRADE_SYSTEM	
#ifdef	KEYBOARD_SET
	void	OnKeyBoardModeSetting( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
#endif	// KEYBOARD_SET 
#ifdef COSTUME_UPGRADE_ENHANCEMENT_GEM
	void	OnReqUpgradeItemSetGem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqUpgradeItemRemoveGem( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
#endif
#ifdef COSTUME_UPGRADE_MIX
	void	OnReqUpgradeItemMix( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
#endif
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// teleport map
	void	OnReqTeleportMapToPoint( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqTeleportMapAddMyPoint( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqTeleportMapChgPointName( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqTeleportMapDelPoint( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqTeleportMapDelAllPoint( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 이벤트용 아레나
	void	OnNotiEventArenaMoveBattleWait( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

	void	OnNotiEventArenaManageSetTonement( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnNotiEventArenaManageSetRedTeam( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnNotiEventArenaManageSetBlueTeam( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnNotiEventArenaManageCallPlayers( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnNotiEventArenaManageJoinPlayers( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnNotiEventArenaManageBattleStart( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnNotiEventArenaManageGameEnd( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnNotiEventArenaManageGameStop( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// charge zone ticket
	void	OnReqChargeZoneEnterance( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnReqChargeZoneLeave( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// treasure chest


	void	OnReqTreasureChestOpenByKey( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	//////////////////////////////////////////////////////////////////////////

	void	OnConsignmentSale_UserInfoReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnConsignmentSale_RegistReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnConsignmentSale_CancelRegistedReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnConsignmentSale_RegistedInfoListReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnConsignmentSale_BuyItemReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnConsignmentSale_CollectSaleGoldReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnConsignmentSale_SearchReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnConsignmentSale_SaleGoldInfoListReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnConsignmentSale_CalcTaxReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

	void	OnReqCharacterServerTransform( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

	void	OnTeleportToNPC( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

	void	OnItemUpgradeRandomOptionSyncReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

	void	OnMadrigalGiftRewardItemRecvReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

	void	OnFlyffPieceExchangeListReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
	void	OnExchangeFlyffPieceItemReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );

	void	OnItemExchangeReq( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
#ifdef PAT_LOOTOPTION
	void	OnPetLootOption( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize );
#endif // PAT_LOOTOPTION

#ifdef __APP_TELEPORTER
	void	OnTeleportByAPP(CAr& ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long);
#endif // __APP_TELEPORTER

public:
	void	SendGuildBankOutputItemReq( const CGuild* pGuild, const u_long idPlayer, const FLItemElem & kOutputItemElem );
	void	SendGuildBankOutputGoldReq( const CGuild* pGuild, const u_long idPlayer, const int nOutputPenya, const bool bUpdateContribution );

#ifdef __PROFESSION
	BOOL	DoUseItemTarget_UpgradeScroll( FLWSUser* pUser, FLItemElem* pMaterial, FLItemElem* pTarget );
	BOOL	DoUseItemTarget_ProfessionChange(FLWSUser* pUser, FLItemElem* pMaterial, FLItemElem* pTarget );
	void	OnProfessionCraft( CAr& ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long );
#endif // __PROFESSION

private:
	DPID	m_dpidCache;		// 캐쉬서버 DPID
};


#endif // __DPSRVR_H__