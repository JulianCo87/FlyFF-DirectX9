
#if !defined(AFX_WNDMANAGER_H__A93F3186_63D6_43C1_956F_EC8691E0C7D9__INCLUDED_)
#define AFX_WNDMANAGER_H__A93F3186_63D6_43C1_956F_EC8691E0C7D9__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif 

//---------------------------------------------------------------------------------------------
// ���
//---------------------------------------------------------------------------------------------
#pragma region HeaderInclude

#include "rtmessenger.h"

#ifdef __GUILD_HOUSE_MIDDLE
	#include "WndHousing.h"
#endif 


#ifdef __ENCHANT_BARUNA16
	#include "WndEnchantBaruna.h"
#endif 

#include "WndPvp.h"						// PVP
#include "WndLord.h"					// 12�� ����
#include "WndParty.h"					// ��Ƽ
#include "WndBank.h"					// ����
#include "WndHelp.h"					// ����
#include "WndGuild.h"					// ���
#include "WndWorld.h"					// ����
#include "WndQuest.h"					// ����Ʈ
#include "WndExtra.h"			
#include "WndPetRes.h"			
#include "WndPetSys.h"			
#include "WndRankWar.h"
#include "WndSealChar.h"				// 11�� �ɸ��� ���� �ŷ� ���
#include "WndCommItem.h"
#include "WndRankInfo.h"
#include "WndPiercing.h"				// �Ǿ��
#include "WndSelectCh.h"
#include "WndRoomList.h"				// 13�� �Ͽ�¡
#include "WndQuitRoom.h"				
#include "WndMiniGame.h"				// 10�� �̴ϰ���
#include "WndRankGuild.h"				// ������
#include "WndGuildBank.h"				// �������
#include "WndAwakening.h"				// 11�� ����, �ູ
#include "WndLvReqDown.h"
#include "FLWndCostume.h"				// �ڽ�Ƭ
#include "WndQuizEvent.h"				// ���� �̺�Ʈ
#include "WndRepairItem.h"				// ����â
#include "WndCollecting.h"				// ä��
#include "WndChangeName.h"				// �̸�����
#include "WndElldinsJar.h"				// 19�� ������ �׾Ƹ�
#include "WndSecretRoom.h"				// 12�� ����� ��
#include "WndRainbowRace.h"				// 13�� ���κ��� ���̽�
#include "WndUpgradebase.h"
#include "WndSummonAngel.h"				// 8�� ���� ��ȯ
#include "WndMadrigalGift.h"			// ���帮���� ����
#include "WndQuestQuickInfo.h"			// ����Ʈ ���� ����
#include "FLWndCancellation.h"			// �ͼ�����
#include "WndBlessingCancel.h"	
#include "FLWndTeleporterMng.h"			// �ڷ���Ʈ ����
#include "FLWndTreasurechest.h"			// ��������
#include "WndGuildWarRequest.h"
#include "WndInvenRemoveItem.h"
#include "WndGuildCombat1to1.h"			// 11�� �ϴ��� ��� ����
#include "WndCommonInputCount.h"		// ���� ���� �Է� �޽��� �ڽ�
#include "WndCommonItemExchange.h"		// ���� ������ ��ȯ 
#include "FLWndInventorySlotProc.h"		// �κ��丮 Ȯ�� 
#include "WndChattingBlockingList.h"
#include "WndGuildWarPeaceConfirm.h"
#include "FLWndCharServerMovement.h" 
#ifdef PAT_LOOTOPTION
#include "WndPetLootOption.h"
#endif // PAT_LOOTOPTION

#include "../Neuz/WebBox.h"
#include "../Neuz/WorldMap.h"

#include <Guildwar.h>
#include <EveSchool.h>

#pragma endregion HeaderInclude


//---------------------------------------------------------------------------------------------
// ���漱��
//---------------------------------------------------------------------------------------------
#pragma region ForwardDeclearation

class CWndMap;
class CWndMessage;
class CWndCommand;
class CWndTextBook;
class CWndDonation;
class CWndGHUpkeep;
class CWndAddFriend;
class CWndGHMainMenu;
class CWndDuelResult;
class CWndInstantMsg;
class CWndTextLetter;
class CWndTextScroll;
class CWndScoreTitle;
class CWndReawakening;
class CWndMessageNote;
class CWndDuelConfirm;
class CWndPartyConfirm;
class CWndMessengerNote;
class CWndFriendConFirm;
class CWndSelectVillage;
class CWndDonationRanking;
class CWndPartyChangeName;
class CWndPartyLeaveConfirm;
class CWndCloseExistingConnection;
class CWndColosseumModalBox;
class CWndColosseumJoin;
class CWndColosseumRetry;
class CWndColosseumRanking;
class CWndColosseumCleared;
class CWndColosseumModelessBox;
class CWndColosseumReadyToStart;
class CFLWndCostume_NPC;
class CFLWndCancellation;
class CFLWndTeleporterMng;
class CWndEArenaTeam;
class CWndEArenaUserMessage_Summons;
class CWndEArenaReferee;
class CWndEArenaTeamMng;
class CWndEArenaWinner;
class CWndConsignmentMarket;
class CWndComposeTwohandWeapon;
class CWndCancel_ComposeWeapon;
class CWndChangeLook;
class CWndTargetInfo_HUD;
class CFLWndTreasurechest_Open;
class CFLWndTreasurechest_Result;
class CFLWndCharServerMovement_Warning;
class CFLWndCharServerMovement_Select;
class CWndPartyUpgradeScroll;

#pragma endregion ForwardDeclearation


//---------------------------------------------------------------------------------------------
// ����
//---------------------------------------------------------------------------------------------
#pragma region Definition

#define DECLAREAPPLET( AddMain_Func, AllocClass) CWndBase* AddMain_Func() { return AllocClass; }
#define REG_VERSION				1
#define MAX_SKILL				16
#define WID_MESSAGEBOX			10
#define CLIENT_WIDTH			FULLSCREEN_WIDTH
#define CLIENT_HEIGHT			FULLSCREEN_HEIGHT


// ä�� ��Ÿ��
#define CHATSTY_GENERAL   		0x0001
#define CHATSTY_WHISPER   		0x0002
#define CHATSTY_SHOUT     		0x0004
#define CHATSTY_PARTY     		0x0008
#define CHATSTY_GUILD     		0x0010
#define CHATSTY_ALL				0x0020

#define CHATSTY_HELP          	0x0060
#define CHATSTY_HELP_CLIENT   	0x0020
#define CHATSTY_HELP_CHAT     	0x0040
#define CHATSTY_SYSTEM        	0x0180
#define CHATSTY_SYSTEM_CLIENT 	0x0080
#define CHATSTY_SYSTEM_CHAT   	0x0100
#define CHATSTY_GAME          	0x0600
#define CHATSTY_GAME_CLIENT   	0x0200
#define CHATSTY_GAME_CHAT     	0x0400

#define PS_USE_MACRO			0x00000001
#define PS_NOT_MACRO			0x00000002
#define PS_REALADDING_CHATWND	0x00000004		//gmpbigsun: ä��â�� ������ ���� �༮

typedef CMap<DWORD, DWORD, void *, void *> CMapDWordToPtr;

#pragma endregion Definition


//---------------------------------------------------------------------------------------------
// ����ü
//---------------------------------------------------------------------------------------------
#pragma region Structure

typedef struct tagWNDREGINFO
{
	DWORD		dwWndId;
	CRect		rect;
	BOOL		bOpen;
	DWORD		dwVersion;
	DWORD		dwWndSize;
	DWORD		dwSize;
	BYTE*		lpArchive;
} WNDREGINFO,* LPWNDREGINFO;


struct AppletFunc
{
	CWndBase*	(*m_pFunc)();
	DWORD		m_dwIdApplet;
	LPCTSTR		m_pAppletName;
	LPCTSTR		m_pAppletDesc;
	LPCTSTR		m_pszIconName;
	CHAR		m_cHotkey;
};


struct ToolTipItemTextColor
{
	DWORD dwName0;					// �̸� 0�ܰ�
	DWORD dwName1;					// �̸� 1�ܰ�
	DWORD dwName2;					// �̸� 2�ܰ�
	DWORD dwName3;					// �̸� 3�ܰ�
	DWORD dwName4;					// �̸� 4�ܰ�
	DWORD dwName5;					// �̸� 5�ܰ�
	DWORD dwGeneral;				// �Ϲ�
	DWORD dwGeneralOption;			// �Ϲ� �ɼ�
	DWORD dwPiercing;				// �Ǿ��
	DWORD dwPlusOption;				// �÷��� �ɼ�
	DWORD dwResist;					// �Ӽ� �Ϲ�
	DWORD dwResistFire;				// �Ӽ� ��
	DWORD dwResistWater;			// �Ӽ� ��
	DWORD dwResistEarth;			// �Ӽ� ��
	DWORD dwResistElectricity;		// �Ӽ� ����
	DWORD dwResistWind;				// �Ӽ� �ٶ�
	DWORD dwResistSM;				// �Ӽ� ���ȭ ����
	DWORD dwResistSM1;				// �Ӽ� ���ȭ ��
	DWORD dwTime;					// �ð� �� ȿ��
	DWORD dwEffective0;				// ������ ȿ��
	DWORD dwEffective1;				// ������ ȿ��1
	DWORD dwEffective2;				// ������ ȿ��2
	DWORD dwEffective3;				// ������ ȿ��3
	DWORD dwRandomOption;			// ���� �ɼ�
	DWORD dwEnchantOption;			// ��þƮ �ɼ�
	DWORD dwSetName;				// ��Ʈ �̸�
	DWORD dwSetItem0;				// ��Ʈ ���(�������)
	DWORD dwSetItem1;				// ��Ʈ ���(�����)
	DWORD dwSetEffect;				// ��Ʈ ȿ��
	DWORD dwGold;					// ����
	DWORD dwCommand;				// ����
	DWORD dwNotUse;					// ������
	DWORD dwAddedOpt1;				//�����߰��ɼ�1
	DWORD dwAddedOpt2;				//�����߰��ɼ�2
	DWORD dwAddedOpt3;				//�����߰��ɼ�3
	DWORD dwAddedOpt4;				//�����߰��ɼ�4
	DWORD dwAddedOpt5;				//�����߰��ɼ�5
	DWORD dwAddedOpt6;				//�����߰��ɼ�6
	DWORD dwAddedOpt7;				//�����߰��ɼ�7
	DWORD dwAddedOpt8;				//�����߰��ɼ�8
	DWORD dwAddedOpt9;				//�����߰��ɼ�9
	DWORD dwAwakening;				// ������ ����
	DWORD dwBlessing;				// ������ �ູ
	DWORD dwBlessingWarning;		// ������ �ູ ���
	DWORD dwBarunaRunePiercing;		// �ٷ糪 �� �Ǿ�� 
};

#pragma endregion Structure


//---------------------------------------------------------------------------------------------
// ������ �޴���
//---------------------------------------------------------------------------------------------
#pragma region WindowManager

class CWndMgr : public CWndBase
{ 

	friend CWndApplet;


public:


	// ������
	enum 
	{
		FIRST_TC,
		SECOND_TC,
		THIRD_TC,
		MAX_TC
	};

	enum
	{ 
		BANNING_POINT							= 5,
		BANNING_2_POINT 						= 2 
	};
	
	enum
	{ 
		WARNING_MILLISECOND						= 700, 
		WARNING_2_MILLISECOND					= 1000, 
		SHORTCUT_WARNING_MILLISECOND			= 3000, 
		BANNING_MILLISECOND						= 180000
	};

	enum
	{ 
		TOOL_TIP_SWITCH_MAIN 					= 0, 
		TOOL_TIP_SWITCH_SUB1 					= 1, 
		TOOL_TIP_SWITCH_SUB2 					= 2
	};



public:



												CWndMgr( void );
	virtual										~CWndMgr( void );


	HRESULT 									InvalidateDeviceObjects( void );
	HRESULT 									DeleteDeviceObjects( void );
	HRESULT										RestoreDeviceObjects( void );

	// ������ �޽���
	void										AlighWindow( CRect rcOld, CRect rcNew );
	virtual	BOOL								Initialize( CWndBase* pWndParent = NULL );
	virtual	void								OnInitialUpdate( void );
	virtual void								OnDraw( C2DRender* p2DRender );
	virtual	void								OnDestroyChildWnd( CWndBase* pWndChild );
	virtual	void								OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
	virtual void								OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags );
	virtual	void								OnSize( UINT nType, int cx, int cy );
	virtual BOOL    							OnDropIcon( LPSHORTCUT pShortcut, CPoint point = 0 );
	virtual	BOOL								OnEraseBkgnd( C2DRender* p2DRender );
	virtual	BOOL								OnChildNotify( UINT message,UINT nID,LRESULT* pLResult );
	virtual	BOOL								OnCommand( UINT nID,DWORD dwMessage );
	virtual	BOOL    							Process( void );


	BOOL										IsShortcutCommand( void ) const;
	void 										SetWarningCounter( int nWarningCounter );
	void 										SetWarning2Counter( int nWarning2Counter );
	int 										GetWarningCounter( void ) const;
	int 										GetWarning2Counter( void ) const;
	CTimer& 									GetWarningTimer( void );
	CTimer& 									GetWarning2Timer( void );
	CTimer& 									GetShortcutWarningTimer( void );
	CTimer& 									GetBanningTimer( void );


	// ����Ʈ
	void 										OpenQuestItemInfo( CWndBase* pWndParent = NULL, FLItemBase* pItemBase = NULL );
	void 										ChangeQuestItemInfo( FLItemBase* pItemBase = NULL );


	BOOL										IsConsignmentMarket_Register( void );									// �߰��ŷ� �� ����� �̿����ΰ�?
	BOOL										DoModalConsignmentMarket_Register( DWORD dwItemId );					// �ش� ���������� �Է�â�� �����


	void 										OpenTextBook( CWndBase* pWndParent = NULL, FLItemBase* pItemBase = NULL);
	void 										ChangeTextBook( FLItemBase* pItemBase = NULL );
	void 										OpenTextScroll( CWndBase* pWndParent = NULL, FLItemBase* pItemBase = NULL);
	void 										ChangeTextScroll( FLItemBase* pItemBase = NULL );
	void 										OpenTextLetter( CWndBase* pWndParent = NULL, FLItemBase* pItemBase = NULL);
	void 										ChangeTextLetter( FLItemBase* pItemBase = NULL );


	// ä��
	void 										ParsingChat( CString string );
	void 										WordChange( CString& rString );
	void 										PutDefinedString( DWORD dwText, ... );
	void 										PutDestParam( DWORD dwDst1, DWORD dwDst2, DWORD dwAdj1, DWORD dwAdj2, CEditString &str );	
	void 										PutString( LPCTSTR lpszString, CObj* pObj = NULL, DWORD dwColor = 0xffffffff, DWORD dwChatStyle = CHATSTY_GAME, DWORD dwPStyle = 0x00000001 );


	BOOL										OpenMenu( void );
	void										OpenField( void );
	BOOL										OpenTaskBar( void );
	CWndMap*									OpenMap( LPCTSTR lpszMapFileName );


	void										OpenTitle( BOOL bFirstTime = FALSE );
	BOOL										IsTitle( void )				{	return m_bTitle;		}

	// ä��	
	CCollectingWnd*								OpenCollecting( void );
	BOOL										CloseCollecting(void);


	void										UpdateContentsByBuff( WORD wType, WORD wID );


	void										ObjectExecutor( DWORD dwShortcut, DWORD dwId, DWORD dwType = 0 );
	void										ObjectExecutor( LPSHORTCUT lpShortcut );
	void										UseSkillShortCut( DWORD dwType, DWORD dwSkillIdx );


	// �޽���
	CWndMessage*								GetMessage( LPCTSTR lpszFrom );
	CWndMessage*								OpenMessage( LPCTSTR lpszFrom );
	CWndInstantMsg*								GetInstantMsg( LPCTSTR lpszFrom );
	CWndInstantMsg*								OpenInstantMsg( LPCTSTR lpszFrom );
	BOOL										UpdateMessage( LPCTSTR pszOld, LPCTSTR pszNew );


	// �޽��� �ڽ�
	BOOL										OpenCustomBox( LPCTSTR strMessage, CWndMessageBox* pWndMessageBox, CWndBase* pWndParent = NULL );
	BOOL										OpenMessageBox( LPCTSTR strMessage, UINT nType = MB_OK, CWndBase* pWndParent = NULL );
	BOOL    									OpenMessageBoxUpper( LPCTSTR lpszMessage, UINT nType = MB_OK, BOOL bPostLogoutMsg = FALSE );
	BOOL										OpenMessageBoxWithTitle( LPCTSTR lpszMessage, const CString& strTitle, UINT nType = MB_OK, CWndBase* pWndParent = NULL );
	void										CloseMessageBox( void );
	CWndMessageBox*								GetMessageBox( void )		{	return 	m_pWndMessageBox;	}


	// ����
	void 										MakeToolTipText( FLItemBase* pItemBase, CEditString& strEdit, int flag = 0 );
	void 										PutToolTip_Troupe( DWORD dwSkill, CPoint point, CRect* pRect );
	void 										PutToolTip_Item( FLItemBase* pItemBase, CPoint point, CRect* pRect, int flag = 0 );
	void 										PutToolTip_Item( DWORD dwType, DWORD dwId, CPoint point, CRect* pRect, int flag = 0 );
	void 										PutToolTip_Skill( DWORD dwSkill, DWORD dwLevel, CPoint point, CRect* pRect, BOOL bActive = TRUE );
	void 										PutToolTipItemAndSubToolTip( FLItemBase* pItemBase, CPoint point, CRect* pRect, int nFlag = 0 );
	void 										PutToolTipParts( FLItemBase* pPartsItemBase, CPoint point, CRect* pRect, const int nFlag, const int nSubToolTipFlag );
	void										PutToolTip_Character( int SelectCharacter, CPoint point, CRect* pRect );
	DWORD										PutItemName( FLItemElem* pItemElem, CEditString* pEdit );
	void										PutItemAbilityPiercing( FLItemElem* pItemElem, CEditString* pEdit, DWORD dwColorBuf, int nFlag = 0 );
	void										PutItemResist( FLItemElem* pItemElem, CEditString* pEdit );
	void										PutItemSpeed( FLItemElem* pItemElem, CEditString* pEdit );
#ifdef COSTUME_UPGRADE_ENHANCEMENT_GEM
	void										PutItemMinMax( CMover* pMover, FLItemElem* pItemElem, CEditString* pEdit, bool bCostume = FALSE );
#else
	void										PutItemMinMax( CMover* pMover, FLItemElem* pItemElem, CEditString* pEdit );
#endif
	void										PutRandomOpt( FLItemElem* pItemElem, CEditString* pEdit, int nType = 0 );					// ��չ��� �ռ�
	void										PutAwakeningBlessing( FLItemElem* pItemElem, CEditString* pEdit );							// ����, �ູ
	void										PutBubbleTime( CEditString* pEdit );														// ���� Ÿ�� ( �޽� ����ġ )
	void 										PutPiercingOpt( FLItemElem* pItemElem, CEditString* pEdit, int nType = 0 );					// ��չ��� �ռ�
#ifdef COSTUME_UPGRADE_ENHANCEMENT_GEM
	void										PutGemOpt( FLItemElem* pItemElem, CEditString* pEdit );
#endif
	void 										PutEnchantOpt( CMover* pMover, FLItemElem* pItemElem, CEditString* pEdit, int flag );
	void 										PutSetItemOpt( CMover* pMover, FLItemElem* pItemElem, CEditString* pEdit );
	void 										PutBaseItemOpt( FLItemElem* pItemElem, CEditString* pEdit );
	void 										PutMedicine( FLItemElem* pItemElem, DWORD dwParam, LONG nParamVal, CEditString* pEdit );
	void 										PutBaseResist( FLItemElem* pItemElem, CEditString* pEdit );
	void 										PutItemGold( CMover* pMover, FLItemElem* pItemElem, CEditString* pEdit, int flag );
	void 										PutCoolTime( CMover* pMover, FLItemElem* pItemElem, CEditString* pEdit );
	void 										PutKeepTime( FLItemElem* pItemElem, CEditString* pEdit );
	void 										PutCommand( FLItemElem* pItemElem, CEditString* pEdit );
	void 										PutEndurance( FLItemElem* pItemElem, CEditString* pEdit, int flag );
	void 										PutSex( CMover* pMover, FLItemElem* pItemElem, CEditString* pEdit );
	void 										PutJob( CMover* pMover, FLItemElem* pItemElem, CEditString* pEdit );
	void 										PutLevel( CMover* pMover, FLItemElem* pItemElem, CEditString* pEdit );
	void 										PutChangedLook( FLItemElem* pItemElem, CEditString* pEdit );
	void 										PutWeapon( FLItemElem* pItemElem, CEditString* pEdit );
	void 										PutAddedOpt( FLItemElem* pItemElem, CEditString* pEdit );									// 9�� ���°��� Clienet
	void 										PutPetInfo( FLItemElem* pItemElem, CEditString* pEdit );
	void 										PutPetFeedPocket( FLItemElem* pItemElem, CEditString* pEdit );
	void 										PutSealChar( FLItemElem* pItemElem, CEditString* pEdit );									// 11�� �ɸ��� ���� �ŷ� ���
	void 										PutNeededVis( FLItemElem* pItemElem, CEditString* pEdit );
	void 										PutVisPetInfo( FLItemElem* pItemElem, CEditString* pEdit );
	void 										PutPetKind( FLItemElem* pItemElem, CEditString* pEdit );
	void 										PutBind( FLItemElem* pItemElem, CEditString* pEdit );										// �ͼ� ���� ���
	void										PutHitPoint( FLItemElem* pItemElem, CEditString* pEdit );									// 19�� ������ �׾Ƹ�
	void										PutBarunaPearcing( FLItemElem* pItemElem, CEditString* pEdit );								// �ٷ糪 �Ǿ��
	void										PutTHCombinedOption( FLItemElem* pItemElem, CEditString* pEdit );							// ��չ��� �ռ�
	void										PutItemCharge( FLItemElem* pItemElem, CEditString* pEdit );
	void										PutEquipItemText( CEditString* pEdit );


	// ���ø�
	CWndBase*									CreateApplet( DWORD dwIdApplet );
	void										DestroyApplet( void );
	CWndBase*									GetApplet( DWORD dwIdApplet );
	DWORD										GetAppletId( TCHAR* lpszAppletName );
	void										AddAllApplet( void );
	void										AddAppletFunc( LPVOID pAppletFunc, DWORD dwIdApplet, LPCTSTR lpszKeyword, LPCTSTR pszIconName, LPCTSTR lpszAppletDesc = NULL, CHAR cHotkey = 0 );
	AppletFunc*									GetAppletFunc( DWORD dwIdApplet );


	// ������ ���
	LPWNDREGINFO								GetRegInfo( DWORD dwWndId );
	BOOL										LoadRegInfo( LPCTSTR lpszFileName );
	BOOL										SaveRegInfo( LPCTSTR lpszFileName );
	BOOL 										PutRegInfo( DWORD dwWndId, CRect rect, BOOL bOpen );
	BOOL 										PutRegInfo( LPWNDREGINFO lpRegInfo );
	BOOL 										PutRegInfo( CWndNeuz* pWndNeuz, BOOL bOpen );


	// ����
	void										PreDelete( CWndBase* pBase );					// ��ϵ� ������ �̿����� �ʰ� delete�ϴ°�� �ҷ�����Ѵ�.
	void										CloseBoundWindow(void);							// �������� �ɷ� �ְų�, �������� ������ ���ɼ��� �ִ� ��� â�� ������ �ݴ´�.
	void										CloseFuncApp( void );
	void										ClearAllWnd( void );
	void										RestoreWnd( void );
	void 										Free( void );


	// ĸ��
	BOOL										ScreenCapture( void );
	BOOL										SaveJPG( LPCTSTR lpszName );	
	BOOL										SaveBitmap( LPCTSTR lpszName );

	// Ŀ���� ���ϵ�
	void 										RegisterCustomChild( CWndBase*& pWndChild );	//�ʱ�ȭ �� ���
	void 										DeleteAllCustomChild( void );	


	// HUD
	BOOL 										EnableHUD( BOOL bEnable );						// HUD window ON/OFF


	DWORD										GetDSTStringId( int nDstParam );				// �Ǿ�� ���� â ���� �� ���͸� ���� ���� â �߰�
	BOOL										IsDstRate( int nDstParam );


	void 										InitSetItemTextColor( void );
	void 										SetMessengerAutoState( void );
	void										SetPlayer( CMover* pMover );
	BOOL										CheckConfirm( FLItemBase* pItem );
	CWndBase*									GetWndVendorBase( void );
	BOOL 										IsMakingRevivalWnd( void );						// ��Ȱâ�� ����� �Ǵ°�? ��� CWndRevival�� static�Լ��� �����Ǵ����� ���ƺ�������...
	CWndCommonInputCount*						GetWndCommonInputCount( CWndBase* pParentWnd );


#ifdef __HYPERLINK_ITEM16
	DWORD										GetItemTitleTextColor( FLItemElem* pItemElem );
#endif 


protected:

	void										OpenMadrigalGift( void );
	void										OpenQuestQuickInfo( void );	
	
	bool										IsDontDestroy( DWORD dwWndId );
	void										RegisterAllCustomChild( void );							


	void										__HotKeyChange( DWORD dwId, char* pch );
	void										FormalizeChatString( CString & str );


public:


	// ������ ����
	CWndWorld*     								m_pWndWorld;							// ����
	CWndFaceShop*								m_pWndFaceShop;							// ��������
	CWndBeautyShop*								m_pWndBeautyShop;						// ��������
	CWndUseCouponConfirm*						m_pWndUseCouponConfirm;					// �������
	CWndSummonAngel*							m_pWndSummonAngel;						// õ���ȯ
	CWndShop*									m_pWndShop;								// ����
	CFLWndShop_Cart*							m_pWndShop_Cart;						// ���� (īƮ)
	CWndTrade*     								m_pWndTrade;							// ��ȯ
	CWndTradeGold* 								m_pWndTradeGold;						// ��ȯ (��)
	CWndConfirmTrade * 							m_pWndConfirmTrade;						// ��ȯ
	CWndTradeConfirm * 							m_pWndTradeConfirm;						// ��ȯ
	CWndBank*	   								m_pWndBank;								// ����
	CWndGuildBank*								m_pWndGuildBank;						// �������
	CWndPost*									m_pWndPost;								// ����
	CCollectingWnd*								m_pWndCollecting;						// ä��
	CWndAwakening*								m_pWndAwakening;						// ����
	CWndDropItem*  								m_pWndDropItem;							// ������ ���
	CWndDropConfirm*  							m_pWndDropConfirm;						// ������ ���
	CWndSmeltJewel*								m_pWndSmeltJewel;						// ��������
#ifdef COSTUME_UPGRADE_ENHANCEMENT_GEM
	CWndNewSmeltJewel*							m_pWndNewSmeltJewel;						// ��������(������)
#endif
	CWndMixJewel*								m_pWndSmeltMixJewel;					// ��������
#ifdef COSTUME_UPGRADE_MIX
	CWndItemMix*								m_pWndSmeltItemMix;						// �������ռ�
#endif
	CWndLordTender*								m_pWndLordTender;						// ����
	CWndLordVote*								m_pWndLordVote;							// ����
	CWndLordState*								m_pWndLordState;						// ����
	CWndLordEvent*								m_pWndLordEvent;						// ����
	CWndLordSkillConfirm*						m_pWndLordSkillConfirm;					// ����
	CWndLordInfo*								m_pWndLordInfo;							// ����
	CWndLordRPInfo*								m_pWndLordRPInfo;						// ����
	CWndSecretRoomInfoMsgBox*					m_pWndSecretRoomMsg;					// ����� ��
	CWndSecretRoomSelection*					m_pWndSecretRoomSelection;				// ����� ��
	CWndSecretRoomOffer*						m_pWndSecretRoomOffer;					// ����� ��
	CWndSecretRoomOfferState*					m_pWndSecretRoomOfferState;				// ����� ��
	CWndSecretRoomChangeTaxRate*				m_pWndSecretRoomChangeTaxRate;			// ����� ��
	CWndSecretRoomCheckTaxRate*					m_pWndSecretRoomCheckTaxRate;			// ����� ��
	CWndSecretRoomBoard*						m_pWndSecretRoomBoard;					// ����� ��
	CWndSecretRoomQuick*						m_pWndSecretRoomQuick;					// ����� ��
	CWndSmeltSafety*							m_pWndSmeltSafety;
	CWndSmeltSafetyConfirm*						m_pWndSmeltSafetyConfirm;
	CWndQuickList* 								m_pWndQuickList;
	CWndExtraction*								m_pWndExtraction;
	CWndChangeWeapon*							m_pWndChangeWeapon;
	CWndPetRes*    								m_pPetRes;
	CWndPetStatus*								m_pWndPetStatus;
	CWndPetMiracle*								m_pWndPetMiracle;
	CWndChangePetName*							m_pWndChangePetName;	
	CWndFoodConfirm*							m_pWndFoodConfirm;
	CWndPetFoodMill*							m_pWndPetFoodMill;
	CWndPetLifeConfirm*							m_pWndPetLifeConfirm;
	CWndHeroSkillUp*							m_pWndHeroSkillUp;		
	CWndRemovePiercing*							m_pWndRemovePiercing;	
	CWndRemoveJewel*							m_pWndRemoveJewel;		
	CWndPetTransEggs*							m_pWndPetTransEggs;		
	CWndHeavenTower*							m_pWndHeavenTower;	
	CWndChatLog*								m_pWndChatLog;					
	CFLWndChatNotice*							m_pWndChatNotice;
	CWndRemoveAttribute*						m_pWndRemoveAttribute;	
	CWndQueryEquip*								m_pWndQueryEquip;
	CWndSelectVillage*							m_pWndSelectVillage;
	CWndQuestItemWarning*						m_pWndQuestItemWarning;
	CWndChangeName*								m_pWndChangeName;
	CWndUpgradeBase*  							m_pWndUpgradeBase;
	CWndPiercing*	  							m_pWndPiercing;
	CWndSealChar*								m_pWndSealChar;
	CWndSealCharSelect*							m_pWndSealCharSelect;
	CWndSealCharSend*							m_pWndSealCharSend;
	CWndSealCharSet*							m_pWndSealCharSet;
	CWndInvenRemoveItem*						m_pWndInvenRemoveItem;
	CWndCommercialElem*							m_pWndCommerialElem;
	CWndRemoveElem* 							m_pRemoveElem;
	CWndRepairItem* 							m_pWndRepairItem;
	CWndConfirmBank*							m_pWndConfirmBank;
	CWndBankPassword*							m_pWndBankPassword;
	CWndPenaltyPK* 								m_pWndPenaltyPK;
	CWndRankGuild* 								m_pWndRankGuild;
	CWndRankInfo*								m_pWndRankInfo;
	CWndRankWar*								m_pWndRankWar;
	CWndLvReqDown*								m_pWndLvReqDown;
	CFLWndCancellation*							m_pWndCancellation;
	CWndFunnyCoinConfirm*						m_pFunnyCoinConfirm;
	CWndGuildNickName*							m_pWndGuildNickName;
	CWndGuildBankLog*							m_pWndGuildBankLog;		
	CWndGuildConfirm*							m_pWndGuildConfirm;
	CWndGuildWarPeaceConfirm*					m_pWndGuildWarPeaceConfirm;
	CWndGuildWarRequest*						m_pWndGuildWarRequest;
	CWndGuildCombatState*						m_pWndGuildWarState;	
	CWndGuildCombatRank_Person*					m_pWndGuildCombatRanking;				// ��� ����
	CWndGuildCombat1to1Offer*					m_pWndGuildCombatOffer;					// ��� ����
	CWndGuildCombatSelection*					m_pWndGuildCombatSelection;				// ��� ����
	CWndGuildCombatJoinSelection*				m_pWndGuildCombatJoinSelection;			// ��� ����
	CWndGuildCombatBoard*						m_pWndGuildCombatBoard;					// ��� ����
	CGuildCombatInfoMessageBox2*				m_pWndGuildCombatInfoMessageBox2;		// ��� ����
	CGuildCombatInfoMessageBox*					m_pWndGuildCombatInfoMessageBox;		// ��� ����
	CWndGuildCombatResult*						n_pWndGuildCombatResult;				// ��� ����
	CWndGuildCombat1to1Selection*				m_pWndGuildCombat1to1Selection;			// ��� ����
	CWndGuildCombat1to1Offer*					m_pWndGuildCombat1to1Offer;				// ��� ����
	CWndBlessingCancel*							m_pWndBlessingCancel;
	CWndPetAwakCancel*							m_pWndPetAwakCancel;	
	CWndSelectCh*								m_pWndSelectCh;
	CWndDiceGame*								m_pWndDiceGame;
	CWndPuzzleGame*								m_pWndPuzzleGame;
	CWndFindWordGame*							m_pWndFindWordGame;
	CWndKawiBawiBoGame*							m_pWndKawiBawiBoGame;
	CWndKawiBawiBoGameConfirm*					m_pWndKawiBawiBoGameConfirm;
	CWndRainbowRaceOffer*						m_pWndRainbowRaceOffer;					// ���κ��� ���̽�
	CWndRainbowRaceInfo*						m_pWndRainbowRaceInfo;					// ���κ��� ���̽�
	CWndRainbowRaceRule*						m_pWndRainbowRaceRule;					// ���κ��� ���̽�
	CWndRainbowRaceRanking*						m_pWndRainbowRaceRanking;				// ���κ��� ���̽�
	CWndRainbowRacePrize*						m_pWndRainbowRacePrize;					// ���κ��� ���̽�
	CWndRainbowRaceMiniGameButton*				m_pWndRainbowRaceMiniGameButton;		// ���κ��� ���̽�
	CWndRainbowRaceMiniGame*					m_pWndRainbowRaceMiniGame;				// ���κ��� ���̽�
	CWndRainbowRaceMiniGameEnd*					m_pWndRainbowRaceMiniGameEnd;			// ���κ��� ���̽�
	CWndRRMiniGameKawiBawiBo*					m_pWndRRMiniGameKawiBawiBo;				// ���κ��� ���̽� (����, ����, ��)
	CWndRRMiniGameDice*							m_pWndRRMiniGameDice;					// ���κ��� ���̽� (�ֻ���)
	CWndRRMiniGameArithmetic*					m_pWndRRMiniGameArithmetic;				// ���κ��� ���̽� (���)
	CWndRRMiniGameStopWatch*					m_pWndRRMiniGameStopWatch;				// ���κ��� ���̽� (�����ġ)
	CWndRRMiniGameTyping*						m_pWndRRMiniGameTyping;					// ���κ��� ���̽� (Ÿ����) 
	CWndRRMiniGameCard*							m_pWndRRMiniGameCard;					// ���κ��� ���̽� (ī��)
	CWndRRMiniGameLadder*						m_pWndRRMiniGameLadder;					// ���κ��� ���̽� ����
	CWndCoupleMessage*							m_pWndCoupleMessage;					// Ŀ�� �ý���
	CWndChangeAttribute*						m_pWndChangeAttribute;					// ���� Ȯ��(�Ӽ�, �Ϲ�)
	CWndReSkillWarning*							m_pWndReSkillWarning;	
	CWndFontEdit*								m_pWndFontEdit;
	CWndPartyChangeName*						m_pWndPartyChangeName;
	CWndPartyConfirm*							m_pWndPartyConfirm;
	CWndPartyLeaveConfirm*						m_pWndPartyLeaveConfirm;
	CWndMessageNote*							m_pWndMessageNote;
	CWndMessengerNote*							m_pWndMessengerNote;
	CWndFriendConFirm*							m_pWndFriendConfirm;
	CWndDuelConfirm*							m_pWndDuelConfirm;
	CWndDuelResult*								m_pWndDuelResult;
	CWndAddFriend*								m_pWndAddFriend;
	CWndStateConfirm*							m_pWndStateConfirm;
	CWndCloseExistingConnection*				m_pWndCloseExistingConnection;
	CWndRandomScrollConfirm*					m_pWndRandomScrollConfirm;
	CWndHelpInstant*							m_pWndHelpInstant;
	CWndRoomList*								m_pWndRoomList;							// �Ͽ�¡ �ý���
	CWndQuitRoom*								m_pWndQuitRoom;
	CWndEquipBindConfirm*						m_pWndEquipBindConfirm;
	CWndRestateConfirm*							m_pWndRestateConfirm;
	CWndQuizEventConfirm*						m_pWndQuizEventConfirm;					// ���� �̺�Ʈ
	CWndQuizEventQuestionOX*					m_pWndQuizEventQuestionOX;				// ���� �̺�Ʈ
	CWndQuizEventQuestion4C*					m_pWndQuizEventQuestion4C;				// ���� �̺�Ʈ
	CWndQuizEventButton*						m_pWndQuizEventButton;					// ���� �̺�Ʈ
	CWndQuestDetail*							m_pWndQuestDetail;						// ����Ʈ (�ڼ���)
	CWndQuestQuickInfo*							m_pWndQuestQuickInfo;					// ����Ʈ (������)
	CWndCampusInvitationConfirm*				m_pWndCampusInvitationConfirm;
	CWndCampusSeveranceConfirm*					m_pWndCampusSeveranceConfirm;
	CWndBuffPetStatus*							m_pWndBuffPetStatus;
	CWndConfirmVis*								m_pWndConfirmVis;
	CWndGHMainMenu*								m_pWndGHMain;
	CWndGHUpkeep*								m_pWndUpkeep;
	CWndSelectAwakeCase*						m_pWndSelectAwakeCase;
	CWndChattingBlockingList*					m_pWndChattingBlockingList;
	CWndColosseumJoin*							m_pWndColosseumJoin;					// �ݷμ���
	CWndColosseumModelessBox*					m_pWndColosseumModeless;				// �ݷμ���
	CWndColosseumModalBox*						m_pWndColosseumModal;					// �ݷμ���
	CWndColosseumCleared*						m_pWndColosseumCleared;					// �ݷμ���
	CWndColosseumReadyToStart*					m_pWndColosseumReadyToStart;			// �ݷμ���
	CWndColosseumRetry*							m_pWndColosseumRetry;					// �ݷμ���
	CWndColosseumRanking*						m_pWndColosseumRanking;					// �ݷμ���
	CWndEArenaTeam*								m_pWndEArenaTeam;						// �̺�Ʈ �Ʒ��� (����)
	CWndEArenaUserMessage_Summons*				m_pWndEArenaMessageBox;					// �̺�Ʈ �Ʒ��� (��ȯ�޽��� �ڽ�)
	CWndEArenaWinner*							m_pWndEArenaWinner;						// �̺�Ʈ �Ʒ��� (�������� ǥ��)
	CWndScoreTitle*								m_pWndScoreTitle;						// �̺�Ʈ �Ʒ��� (������)
	CWndEArenaReferee*							m_pWndEArenaReferee;					// �̺�Ʈ �Ʒ��� (������ ����)
	CWndEArenaTeamMng*							m_pWndEArenaMng;						// �̺�Ʈ �Ʒ��� (������ ������)
	CFLWndTreasurechest_Open*					m_pWndTreasurechest;					// ��������
	CFLWndTreasurechest_Result*					m_pWndTreasurechest_Result;				// ��������
	CFLWndCharServerMovement_Warning*			m_pWndCharServerMovement_Warning;		// ��������
	CFLWndCharServerMovement_Select*			m_pWndCharServerMovement_Select;		// ��������
	CWndTargetInfo_HUD*							m_pWndTargetHUD;						// Ÿ������
	CFLWndCostume_NPC*							m_pCostume_NPC;							// �ڽ�Ƭ
	CWndDonation*								m_pWndDonation;							// ���	
	CWndDonationRanking*						m_pWndDonationRanking;					// ��� ����
	CFLWndTeleporterMng*						m_pWndTeleporterMng;
	CWndConsignmentMarket*						m_pWndProxyMarket;
	CWndComposeTwohandWeapon*					m_pWndComposeTwoWeapon;
	CWndCancel_ComposeWeapon*					m_pWndCancel_CTW;
	CWndChangeLook*								m_pWndChangeLook;						
	CWndQuestItemInfo*							m_pQuestItemInfo;
	CWndTextBook*								m_pWndTextBook;
	CWndTextScroll*								m_pWndTextScroll;
	CWndTextLetter*								m_pWndTextLetter;
	CWndRevival*								m_pWndRevival;
	CWndResurrectionConfirm*					m_pWndResurrectionConfirm;
	CWndReWanted*								m_pReWanted;
	CWndWanted*									m_pWanted;
	CWndLogOutWaitting* 						m_pLogOutWaitting;
	CWndCommItemDlg*							m_pWndCommItemDlg;
	CWndChangeClass1*							m_pWndChangeClass1;
	CWndChangeClass2*							m_pWndChangeClass2;
	CWndCoupon100722*							m_pWndCoupon1000722;
	CWndReawakening*							m_pWndReawakening;						// ���� �� ����
	CWndPartyUpgradeScroll*						m_pWndPartyUpgradeScroll;				// �ش� ������ �η縶��
	CWndCommonInputCount*						m_pWndCommonInputCount;

	CWndTaskMenu*								m_pWndMenu;								// �޴�
	CWndDialog*									m_pWndDialog;							// ���̾�α�	
	CWndCommonItemExchange*						m_pWndCommonItemExchange;				// ���� ������ ��ȯ
	CWndTaskBar*								m_pWndTaskBar;							// �۾� ��
	CWndCommand*   								m_pWndCommand;							// ���
	CWndMessageBox*								m_pWndMessageBox;						// �޽��� �ڽ�
	CWndMessageBoxUpper*						m_pWndMessageBoxUpper;					// �޽��� �ڽ�
	CWndBase*      								m_pWndActiveDesktop;
	CWndStatDice*								m_pWndStatDice;
#ifdef PAT_LOOTOPTION
	CWndPetLootOption*							m_pWndPetLootOption;				// �� ���� �ɼ� �ڽ�
#endif // PAT_LOOTOPTION
	
#ifdef CARD_UPGRADE_SYSTEM
	CWndCardUpgrade*							m_pWndCardUpgrade;					// ī�� ���׷��̵�
#endif	// CARD_UPGRADE_SYSTEM

#ifdef __GUILD_HOUSE_MIDDLE
	CWndGuildHouseBid*							m_pWndGHBid;
	CWndGuildHouseNotice*						m_pWndGHNotice;
#endif 

#ifdef __ENCHANT_BARUNA16
	CWndBarunaNPC*								m_pWndBarunaNPC;
#endif 


public:


	BOOL										m_bTitle;
	BOOL										m_bAutoRun;
	BOOL										m_bConnect;
	BOOL										m_clearFlag;
	BOOL										m_bAllAction;
	BOOL										m_bWaitRequestMail;						// ���� ��û�� ��ٸ��� �ִ°�?

	bool										m_bIsOpenLordSkill;						// ���� ��ų�� ���ȳ�?

	DWORD										m_dwSavePlayerTime;
	DWORD										m_dwSkillTime[MAX_SKILL];


	TCHAR										m_szTimerChat[128];
	ToolTipItemTextColor						dwItemColor[MAX_TC];

	CTexturePack 								m_texture;
	CTexturePack 								m_texCommand;
	CTexturePack 								m_texIcon;
	CTexturePack 								m_texWnd;

	CMapStringToPtr 							m_mapMap;
	CMapStringToPtr 							m_mapMessage;
	CMapStringToPtr 							m_mapInstantMsg;

	CMapDWordToPtr								m_mapAppletFunc;
	CMapDWordToPtr  							m_mapWndRegInfo;
	CMapDWordToPtr								m_mapWndApplet;

	list<int>									m_tempWndId;							// 8�� 'T'Ű ���, ���� ���� �Ⱥ��̱�, ȭ�� ��� â On/Off

	CUIntArray   								m_aChatColor ;
	CUIntArray   								m_aChatStyle ;
	CStringArray 								m_aChatString;

	CEditString									m_ChatString;

	CRTMessenger								m_RTMessenger;

	CTimer										m_timerChat;
	CTimer										m_timerMessenger;

	CPtrArray									m_awndShortCut;


	typedef vector< CWndBase** >				vecWndContainer;
	typedef vecWndContainer::iterator			vecWndIter;
	vecWndContainer								m_vecChildWnds;

	int											m_nMaketOpen;

private:


	CString										m_strChatBackup;

	BOOL										m_bShortcutCommand;

	int 										m_nWarningCounter;
	int 										m_nWarning2Counter;

	CTimer										m_timerDobe;
	CTimer 										m_timerBanning;
	CTimer 										m_timerWarning;
	CTimer 										m_timerWarning2;
	CTimer 										m_timerShortcutWarning;

};

#pragma endregion WindowManager


//---------------------------------------------------------------------------------------------
int												_GetHeadQuest( const int nQuest );		
int												_CalcQuestEmotionOffset( const int nHeadQuest, const int nOffset );	

extern CParty									g_Party;
extern CWndMgr									g_WndMng; 
extern CPtrArray								m_wndOrder;
extern CGuildMng								g_GuildMng;
extern CGuildWarMng								g_GuildWarMng;
extern _ERROR_STATE								g_Error_State;
extern CGuildCombat								g_GuildCombatMng;

extern DWORD									FULLSCREEN_WIDTH;
extern DWORD									FULLSCREEN_HEIGHT;

extern void										RenderEnchant( C2DRender* p2DRender, CPoint pt );
extern void										RenderRadar( C2DRender* p2DRender, CPoint pt, DWORD dwValue, DWORD dwDivisor );
//---------------------------------------------------------------------------------------------


#endif 