
#pragma once

#include "WndFontColorInfo.h"

//---------------------------------------------------------------------------------------------
// �ǵ��� ����ü �̸��� ��Ʈ Į�� ���� Ŭ������ �����ϰ� ���ּ��� :)
//---------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------
// ������ ���̽�
//---------------------------------------------------------------------------------------------
typedef struct stWndBase
{
	CWndFontColorInfo	m_stDefault;					// �⺻ ��Ʈ����

	stWndBase()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stDefault.SetFontColorInfo( 0, 0, 0 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stDefault.SetFontColorInfo( 255, 255, 255 );
		}
	}

}FONTCOLOR_WNDBASE, *PFONTCOLOR_WNDBASE;


//---------------------------------------------------------------------------------------------
// ������ Ʈ�� ��Ʈ��
//---------------------------------------------------------------------------------------------
typedef struct stWndTreeCtrl
{

	CWndFontColorInfo	m_stDefault;					// �⺻ ��Ʈ����
	CWndFontColorInfo	m_stSelect;						// ���� �� Ʈ�� ������ ��Ʈ����

	stWndTreeCtrl()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stDefault.SetFontColorInfo( 64, 64, 64 );
			m_stSelect.SetFontColorInfo( 0, 0 ,255 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stDefault.SetFontColorInfo( 255, 255, 255 );
			m_stSelect.SetFontColorInfo( 150, 221, 59 );
		}
	}

}FONTCOLOR_WNDTREECTRL, *PFONTCOLOR_WNDTREECTRL;


//---------------------------------------------------------------------------------------------
// ������ �� ��Ʈ��
//---------------------------------------------------------------------------------------------
typedef struct stWndTabCtrl
{

	CWndFontColorInfo	m_stDefault;					// �⺻ ��Ʈ����
	CWndFontColorInfo	m_stSelectTab;					// ���� �� �� ��Ʈ����

	stWndTabCtrl()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stDefault.SetFontColorInfo( 255, 255, 255 );
			m_stSelectTab.SetFontColorInfo( 0, 0 ,0 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stDefault.SetFontColorInfo( 150, 137, 112 );
			m_stSelectTab.SetFontColorInfo( 255, 255 ,255 );
		}
	}

}FONTCOLOR_WNDTABCTRL, *PFONTCOLOR_WNDTABCTRL;


//---------------------------------------------------------------------------------------------
// ������ ��ư ��Ʈ��
//---------------------------------------------------------------------------------------------
typedef struct stWndButton
{

	CWndFontColorInfo	m_stMainMenuDesc;					// ���� �޴� ���� ��Ʈ����
	CWndFontColorInfo	m_stMainMenuShortcut;				// ���� �޴� ����Ű ��Ʈ����

	CWndFontColorInfo	m_stPush;							// ��ư ������ �� ��Ʈ����
	CWndFontColorInfo	m_stHighLight;						// ��ư ���̶���Ʈ �� �� ��Ʈ����
	CWndFontColorInfo	m_stDisable;						// ��ư �� Ȱ��ȭ �� �� ��Ʈ����
	CWndFontColorInfo	m_stShadow;							// ��ư �۾� �׸��� ��Ʈ ����

	stWndButton()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stMainMenuDesc.SetFontColorInfo( 0, 0, 0 );
			m_stMainMenuShortcut.SetFontColorInfo( 0, 0 ,0 );

			m_stPush.SetFontColorInfo( 255, 255, 100 );
			m_stHighLight.SetFontColorInfo( 64, 64, 255 );
			m_stDisable.SetFontColorInfo( 140, 140, 140 );
			m_stShadow.SetFontColorInfo( 0, 0, 0 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stMainMenuDesc.SetFontColorInfo( 255, 255, 255 );
			m_stMainMenuShortcut.SetFontColorInfo( 255, 255 ,255 );

			m_stPush.SetFontColorInfo( 0, 0, 0 );
			m_stHighLight.SetFontColorInfo( 116, 107, 85 );
			m_stDisable.SetFontColorInfo( 23, 23, 23 );
			m_stShadow.SetFontColorInfo( 255, 244, 204 );
		}
	}

}FONTCOLOR_WNDBUTTON, *PFONTCOLOR_WNDBUTTON;


//---------------------------------------------------------------------------------------------
// ������ �ؽ�Ʈ
//---------------------------------------------------------------------------------------------
typedef struct stWndText
{

	CWndFontColorInfo	m_stDefault;					// �⺻ ��Ʈ����

	stWndText()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stDefault.SetFontColorInfo( 0, 0, 0 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stDefault.SetFontColorInfo( 255, 255, 255 );
		}
	}

}FONTCOLOR_WNDTEXT, *PFONTCOLOR_WNDTEXT;


//---------------------------------------------------------------------------------------------
// ������ ����Ʈ �ڽ�
//---------------------------------------------------------------------------------------------
typedef struct stWndListBox
{

	CWndFontColorInfo	m_stSelectColor;			// ���� ��Ʈ����
	CWndFontColorInfo	m_stMouseOverColor;			// ���콺 ���� ��Ʈ����
	CWndFontColorInfo	m_stInvalidColor;			// ��ȿ���� ���� ��Ʈ����

	stWndListBox()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stSelectColor.SetFontColorInfo( 64, 64, 255 );
			m_stMouseOverColor.SetFontColorInfo( 255, 128, 0 );
			m_stInvalidColor.SetFontColorInfo( 170, 170, 170 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stSelectColor.SetFontColorInfo( 64, 64, 255 );
			m_stMouseOverColor.SetFontColorInfo( 255, 128, 0 );
			m_stInvalidColor.SetFontColorInfo( 170, 170, 170 );
		}
	}

}FONTCOLOR_WNDLISTBOX, *PFONTCOLOR_WNDLISTBOX;


//---------------------------------------------------------------------------------------------
// ������ ���̾�α�
//---------------------------------------------------------------------------------------------
typedef struct stWndDialog
{
	CWndFontColorInfo	m_stGroupBoxTitle;				// �׷�ڽ� Ÿ��Ʋ ��Ʈ ����

	CWndFontColorInfo	m_stKeyButtonDetault;			// Ű ��ư �⺻ ��Ʈ ����
	CWndFontColorInfo	m_stKeyButtonSelect;			// Ű ��ư ���� ��Ʈ ����
	CWndFontColorInfo	m_stKeyButtonRollOver;			// Ű ��ư ���콺 �� ���� ��Ʈ ����

	stWndDialog()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stGroupBoxTitle.SetFontColorInfo( 128, 0, 64 );
			
			m_stKeyButtonDetault.SetFontColorInfo( 80, 80, 80 );
			m_stKeyButtonSelect.SetFontColorInfo( 128, 0, 255 );
			m_stKeyButtonRollOver.SetFontColorInfo( 16, 16, 255 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stGroupBoxTitle.SetFontColorInfo( 246, 204, 77 );

			m_stKeyButtonDetault.SetFontColorInfo( 255, 255, 255 );
			m_stKeyButtonSelect.SetFontColorInfo( 29, 252, 255 );
			m_stKeyButtonRollOver.SetFontColorInfo( 246, 204, 77 );
		}
	}

}FONTCOLOR_WNDDIALOG, *PFONTCOLOR_WNDDIALOG;


//---------------------------------------------------------------------------------------------
// ���� ���ڿ�
//---------------------------------------------------------------------------------------------
typedef struct stEditSring
{

	CWndFontColorInfo	m_stDefault;					// �⺻ ��Ʈ����

	stEditSring()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stDefault.SetFontColorInfo( 0, 0, 0 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stDefault.SetFontColorInfo( 255, 255, 255 );
		}
	}

}FONTCOLOR_EDITSTRING, *PFONTCOLOR_EDITSTRING;


//---------------------------------------------------------------------------------------------
// ���� ����
//---------------------------------------------------------------------------------------------
typedef struct stWndSelectServer
{

	CWndFontColorInfo	m_stServerListSelect;			// ���� ����Ʈ ����
	CWndFontColorInfo	m_stServerListMouseOver;		// ���� ����Ʈ ���콺 ����
 	CWndFontColorInfo	m_stServerListInvalid;			// ���� ����Ʈ ���� �Ұ�
	CWndFontColorInfo	m_stMultiListSelect;			// ä�� ����Ʈ ����
	CWndFontColorInfo	m_stMultiListMouseOver;			// ä�� ����Ʈ ���콺 ����
	CWndFontColorInfo	m_stMultiListInvalid;			// ä�� ����Ʈ ���� �Ұ�

	stWndSelectServer()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stServerListSelect.SetFontColorInfo( 0, 0, 255 );
			m_stServerListMouseOver.SetFontColorInfo( 255, 128, 0 );
			m_stServerListInvalid.SetFontColorInfo( 170, 170, 170 );
			m_stMultiListSelect.SetFontColorInfo( 0, 0, 255 );	
			m_stMultiListMouseOver.SetFontColorInfo( 255, 128, 0 );	
			m_stMultiListInvalid.SetFontColorInfo( 170, 170, 170 );

		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stServerListSelect.SetFontColorInfo( 39, 196, 203 );
			m_stServerListMouseOver.SetFontColorInfo( 253, 95, 255 );
			m_stServerListInvalid.SetFontColorInfo( 170, 170, 170 );
			m_stMultiListSelect.SetFontColorInfo( 39, 196, 203 );	
			m_stMultiListMouseOver.SetFontColorInfo( 253, 95, 255 );	
			m_stMultiListInvalid.SetFontColorInfo( 170, 170, 170 );
		}
	}

}FONTCOLOR_WNDSELECTSERVER, *PFONTCOLOR_WNDSELECTSERVER;


//---------------------------------------------------------------------------------------------
// ����Ʈ
//---------------------------------------------------------------------------------------------
typedef struct stWndQuestDetail
{

	CWndFontColorInfo	m_stQuestCondition;				// ����Ʈ �Ϸ�����	
	CWndFontColorInfo	m_stQuestConditionSelect;		// ����Ʈ �Ϸ����� ����
	CWndFontColorInfo	m_stQuestConditionComplete;		// ����Ʈ �Ϸ����� �Ϸ�

	stWndQuestDetail()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stQuestCondition.SetFontColorInfo( 0, 0, 0 );
			m_stQuestConditionSelect.SetFontColorInfo( 0, 0, 255 );
			m_stQuestConditionComplete.SetFontColorInfo( 64, 64, 64 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stQuestCondition.SetFontColorInfo( 255, 255, 255 );
			m_stQuestConditionSelect.SetFontColorInfo( 39, 196, 203 );
			m_stQuestConditionComplete.SetFontColorInfo( 255, 255, 0 );
		}
	}

}FONTCOLOR_WNDQUESTDETAIL, *PFONTCOLOR_WNDQUESTDETAIL;


//---------------------------------------------------------------------------------------------
// �۾� ��
//---------------------------------------------------------------------------------------------
typedef struct stWndTaskBar
{
	CWndFontColorInfo	m_stToolTip_ShortCutApplet;		// �۾� �ٿ� ������
	CWndFontColorInfo	m_stToolTip_ShortCutHotKey;		// �۾� �ٿ� ����Ű
	CWndFontColorInfo	m_stToolTip_ShortCutMotion;		// �۾� �ٿ� �ൿ
	
	stWndTaskBar()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stToolTip_ShortCutApplet.SetFontColorInfo( 0, 0, 0 );
			m_stToolTip_ShortCutHotKey.SetFontColorInfo( 0, 0, 255 );
			m_stToolTip_ShortCutMotion.SetFontColorInfo( 0, 0, 0 );

		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stToolTip_ShortCutApplet.SetFontColorInfo( 255, 255, 255 );
			m_stToolTip_ShortCutHotKey.SetFontColorInfo( 255, 255, 0 );
			m_stToolTip_ShortCutMotion.SetFontColorInfo( 255, 255, 255 );
		}
	}

}FONTCOLOR_WNDTASKBAR, *PFONTCOLOR_WNDTASKBAR;


//---------------------------------------------------------------------------------------------
// ����
//---------------------------------------------------------------------------------------------
typedef struct stToolTip
{
	CWndFontColorInfo	m_stToolTip;					// �Ϲ����� ����
	CWndFontColorInfo	m_stPutEquipItemText;			// [ ���� ���� ������ ] �̶� ���� ��Ʈ ����

	stToolTip()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stToolTip.SetFontColorInfo( 0, 0, 0 );
			m_stPutEquipItemText.SetFontColorInfo( 0, 0, 0 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stToolTip.SetFontColorInfo( 255, 255, 255 );
			m_stPutEquipItemText.SetFontColorInfo( 255, 255, 255 );
		}
	}

}FONTCOLOR_WNDTOOLTIP, *PFONTCOLOR_WNDTOOLTIP;


//---------------------------------------------------------------------------------------------
// ������ ĳ���� ���� 
//---------------------------------------------------------------------------------------------
typedef struct stWndCharInfo
{
	CWndFontColorInfo	m_stPropertyNameColor;					// ĳ���� ���� �Ӽ� �̸� ��Ʈ����
	CWndFontColorInfo	m_stPropertyNumberColor;				// ĳ���� ���� �Ӽ� ���� ��Ʈ����
	CWndFontColorInfo	m_stIncreasePropertyNumberColor;		// ĳ���� ���� �Ӽ� ���� ���� ��Ʈ����

	CWndFontColorInfo	m_stIncreaseAbilityNumberColor;			// ĳ���� �ɷ�ġ ���� ���� ��Ʈ����
	CWndFontColorInfo	m_stDecreaseAbilityNumberColor;			// ĳ���� �ɷ�ġ ���� ���� ��Ʈ����

	stWndCharInfo()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stPropertyNameColor.SetFontColorInfo( 0, 0, 180 );
			m_stPropertyNumberColor.SetFontColorInfo( 0, 0, 0 );
			m_stIncreasePropertyNumberColor.SetFontColorInfo( 255, 0, 0 );
			m_stIncreaseAbilityNumberColor.SetFontColorInfo( 0, 0, 255 );
			m_stDecreaseAbilityNumberColor.SetFontColorInfo( 0, 0, 255 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stPropertyNameColor.SetFontColorInfo( 246, 204, 77 );
			m_stPropertyNumberColor.SetFontColorInfo( 255, 255, 255 );
			m_stIncreasePropertyNumberColor.SetFontColorInfo( 121, 80, 255 );
			m_stIncreaseAbilityNumberColor.SetFontColorInfo( 74, 123, 197 );
			m_stDecreaseAbilityNumberColor.SetFontColorInfo( 255, 0, 0 );
		}
	}

}FONTCOLOR_WNDCHARINFO, *PFONTCOLOR_WNDCHARINFO;



//---------------------------------------------------------------------------------------------
// �� ���� â
//---------------------------------------------------------------------------------------------
typedef struct stWndPetStatus
{
	CWndFontColorInfo	m_stPropertyNameColor;				// �� ���� �Ӽ� �̸� ��Ʈ����
	CWndFontColorInfo	m_stPropertyNameShadowColor;		// �� ���� �Ӽ� �̸� �׸��� ��Ʈ����
	CWndFontColorInfo	m_stPropertyValueColor;				// �� ���� �Ӽ� ���� ��Ʈ����
	CWndFontColorInfo	m_stGaugeValueColor;				// �� ���� ������ ���� ��Ʈ����

	stWndPetStatus()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stPropertyNameColor.SetFontColorInfo( 74, 74, 173 );
			m_stPropertyNameShadowColor.SetFontColorInfo( 160, 160, 240 );
			m_stPropertyValueColor.SetFontColorInfo( 255, 28, 174 );
			m_stGaugeValueColor.SetFontColorInfo( 255, 255, 255 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stPropertyNameColor.SetFontColorInfo( 246, 204, 77 );
			m_stPropertyNameShadowColor.SetFontColorInfo( 30, 30, 30 );
			m_stPropertyValueColor.SetFontColorInfo( 145, 203, 255 );
			m_stGaugeValueColor.SetFontColorInfo( 255, 255, 255 );
		}
	}

}FONTCOLOR_WNDPETSTATUS, *PFONTCOLOR_WNDPETSTATUS;


//---------------------------------------------------------------------------------------------
// ��Ƽ ���� â
//---------------------------------------------------------------------------------------------
typedef struct stWndPartyInfo
{
	CWndFontColorInfo	m_stLeader;						// ��Ƽ ���� ���� ��Ʈ����

	stWndPartyInfo()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stLeader.SetFontColorInfo( 31, 183, 45 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stLeader.SetFontColorInfo( 255, 95, 95 );
		}
	}

}FONTCOLOR_WNDPARTYINFO, *PFONTCOLOR_WNDPARTYINFO;


//---------------------------------------------------------------------------------------------
// ��Ƽ ��ų â
//---------------------------------------------------------------------------------------------
typedef struct stWndPartySkill
{
	CWndFontColorInfo	m_stlessPoint;						// ��ų�� ������� ����Ʈ ���� ��Ʈ����
	CWndFontColorInfo	m_stNoLearnSkill;					// ��ų�� �� ����� ��Ʈ����

	stWndPartySkill()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stlessPoint.SetFontColorInfo( 0, 0, 255 );
			m_stNoLearnSkill.SetFontColorInfo( 255, 0, 0 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stlessPoint.SetFontColorInfo( 0, 0, 255 );
			m_stNoLearnSkill.SetFontColorInfo( 255, 0, 0 );
		}
	}

}FONTCOLOR_WNDPARTYSKILL, *PFONTCOLOR_WNDPARTYSKILL;


//---------------------------------------------------------------------------------------------
// �޽���
//---------------------------------------------------------------------------------------------
typedef struct stWndMessengerEx
{
	CWndFontColorInfo	m_stName;					// �̸� ��Ʈ����
	CWndFontColorInfo	m_stNameShadow;				// �̸� �׸��� ��Ʈ����
	CWndFontColorInfo	m_stState;					// ���� ��Ʈ����
	CWndFontColorInfo	m_stCampusPoint;			// ķ�۽� ����Ʈ ��Ʈ����
	CWndFontColorInfo	m_stServer;					// ���� ��Ʈ����
	CWndFontColorInfo	m_stChannel;				// ä�� ��Ʈ����

	stWndMessengerEx()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stName.SetFontColorInfo( 96, 96, 96 );
			m_stNameShadow.SetFontColorInfo( 96, 96, 96 );
			m_stState.SetFontColorInfo( 96, 96, 96 );
			m_stCampusPoint.SetFontColorInfo( 96, 96, 96 );
			m_stServer.SetFontColorInfo( 96, 96, 96 );
			m_stChannel.SetFontColorInfo( 96, 96, 96 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stName.SetFontColorInfo( 255, 255, 255 );
			m_stNameShadow.SetFontColorInfo( 96, 96, 96 );
			m_stState.SetFontColorInfo( 54, 224, 82 );
			m_stCampusPoint.SetFontColorInfo( 246, 204, 77 );
			m_stServer.SetFontColorInfo( 246, 204, 77 );
			m_stChannel.SetFontColorInfo( 246, 204, 77 );
		}
	}

}FONTCOLOR_WNDMESSENGEREX, *PFONTCOLOR_WNDMESSENGEREX;


//---------------------------------------------------------------------------------------------
// ģ�� â ( In �޽��� )
//---------------------------------------------------------------------------------------------
typedef struct stWndFriendCtrlEx
{
	CWndFontColorInfo	m_stVisitAllow;					// �̴Ϸ� ���� �㰡
	CWndFontColorInfo	m_stSelect;						// ���� ��Ʈ����
	CWndFontColorInfo	m_stBlock;						// ���� ��Ʈ����

	stWndFriendCtrlEx()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stVisitAllow.SetFontColorInfo( 0, 255, 0 );
			m_stSelect.SetFontColorInfo( 96, 96, 255 );
			m_stBlock.SetFontColorInfo( 255, 0, 0 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stVisitAllow.SetFontColorInfo( 0, 255, 0 );
			m_stSelect.SetFontColorInfo( 39, 196, 203 );
			m_stBlock.SetFontColorInfo( 255, 0, 0 );
		}
	}

}FONTCOLOR_WNDFRIENDCTRLEX, *PFONTCOLOR_WNDFRIENDCTRLEX;



//---------------------------------------------------------------------------------------------
// ��� â ( In �޽��� )
//---------------------------------------------------------------------------------------------
typedef struct stWndGuildCtrlEx
{
	CWndFontColorInfo	m_stSelect;						// ���� ��Ʈ����

	stWndGuildCtrlEx()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stSelect.SetFontColorInfo( 96, 96, 255 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stSelect.SetFontColorInfo( 39, 196, 203 );
		}
	}

}FONTCOLOR_WNDGUILDCTRLEX, *PFONTCOLOR_WNDGUILDCTRLEX;


//---------------------------------------------------------------------------------------------
// ���� â ( In �޽��� )
//---------------------------------------------------------------------------------------------
typedef struct stWndCampus
{
	CWndFontColorInfo	m_stSelect;						// ���� ��Ʈ����

	stWndCampus()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stSelect.SetFontColorInfo( 96, 96, 255 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stSelect.SetFontColorInfo( 39, 196, 203 );
		}
	}

}FONTCOLOR_WNDCAMPUS, *PFONTCOLOR_WNDCAMPUS;


//---------------------------------------------------------------------------------------------
// ������ �Ͽ�¡
//---------------------------------------------------------------------------------------------
typedef struct stWndHousing
{
	CWndFontColorInfo	m_stSelect;						// ���� ��Ʈ����

	stWndHousing()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stSelect.SetFontColorInfo( 0, 0, 255 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stSelect.SetFontColorInfo( 39, 196, 203 );
		}
	}

}FONTCOLOR_WNDHOUSING, *PFONTCOLOR_WNDHOUSING;


//---------------------------------------------------------------------------------------------
// ������ ��� �Ͽ�¡
//---------------------------------------------------------------------------------------------
typedef struct stWndGuildHousing
{
	CWndFontColorInfo	m_stDeploy;						// �����㰡 ��Ʈ����

	stWndGuildHousing()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stDeploy.SetFontColorInfo( 0, 0, 255 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stDeploy.SetFontColorInfo( 237, 20, 91 );
		}
	}

}FONTCOLOR_WNDGUILDHOUSING, *PFONTCOLOR_WNDGUILDHOUSING;


//---------------------------------------------------------------------------------------------
// ������ ��� ����â
//---------------------------------------------------------------------------------------------
typedef struct stWndGuildMetricCtrl
{
	CWndFontColorInfo	m_stItemCount;				// ������ ���� ��Ʈ ����
	CWndFontColorInfo	m_stItemCountShadow;		// ������ ������ �׸��� ��Ʈ ����

	stWndGuildMetricCtrl()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stItemCount.SetFontColorInfo( 16, 16, 255 );
			m_stItemCountShadow.SetFontColorInfo( 0, 0, 0 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stItemCount.SetFontColorInfo( 136, 255, 133 );
			m_stItemCountShadow.SetFontColorInfo( 0, 0, 0 );
		}
	}

}FONTCOLOR_WNDGUILDMETRICCTRL, *PFONTCOLOR_WNDGUILDMETRICCTRL;


//---------------------------------------------------------------------------------------------
// ������ ��� ���� ���� ����
//---------------------------------------------------------------------------------------------
typedef struct stWndGuildCombatRank_Class
{
	CWndFontColorInfo	m_st1stPlayer;			// 1�� ���� ��Ʈ ����
	CWndFontColorInfo	m_stSelfPlayer;			// �÷���� �ڽ� ��Ʈ ����

	stWndGuildCombatRank_Class()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_st1stPlayer.SetFontColorInfo( 200, 0, 0 );
			m_stSelfPlayer.SetFontColorInfo( 0, 0, 255 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_st1stPlayer.SetFontColorInfo( 200, 0, 0 );
			m_stSelfPlayer.SetFontColorInfo( 97, 97, 255 );
		}
	}

}FONTCOLOR_WNDGUILDCOMBATRANK_CLASS, *PFONTCOLOR_WNDGUILDCOMBATRANK_CLASS;


//---------------------------------------------------------------------------------------------
// ������ ���� 
//---------------------------------------------------------------------------------------------
typedef struct stWndShop_Cart
{
	CWndFontColorInfo	m_stlessUserMoney;						// ���� ���� ���� ���� ���� ���� ��Ʈ����
	CWndFontColorInfo	m_stEnoughUserMoney;					// ���� ��� �� ���� ���� ���� ���� ��Ʈ����

	CWndFontColorInfo	m_stShoppingBagItemCount;				// ���� �鿡 ����� �ִ� ������ ������ ���� ��Ʈ ����
	CWndFontColorInfo	m_stShoppingBagItemCountShadow;			// ���� �鿡 ����� �ִ� ������ ������ ������ �׸��� ��Ʈ ����

	CWndFontColorInfo	m_stTotalShoppingBagItemCount;			// ���� �鿡 ����� �ִ� �������� �� ���� ��Ʈ ����

	stWndShop_Cart()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stlessUserMoney.SetFontColorInfo( 255, 0, 0 );
			m_stEnoughUserMoney.SetFontColorInfo( 46, 112, 169 );

			m_stShoppingBagItemCount.SetFontColorInfo( 0, 204, 0 );
			m_stShoppingBagItemCountShadow.SetFontColorInfo( 0, 0, 0 );

			m_stTotalShoppingBagItemCount.SetFontColorInfo( 255, 0, 0 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stlessUserMoney.SetFontColorInfo( 242, 7, 29 );
			m_stEnoughUserMoney.SetFontColorInfo( 150, 221, 59 );

			m_stShoppingBagItemCount.SetFontColorInfo( 136, 255, 133 );
			m_stShoppingBagItemCountShadow.SetFontColorInfo( 0, 0, 0 );

			m_stTotalShoppingBagItemCount.SetFontColorInfo( 246, 204, 77 );
		}
	}

}FONTCOLOR_WNDSHOP_CART, *PFONTCOLOR_WNDSHOP_CART;



//---------------------------------------------------------------------------------------------
// ������ ������ �׾Ƹ�
//---------------------------------------------------------------------------------------------
typedef struct stWndElldinsJar
{
	CWndFontColorInfo	m_stItemCount;							// ������ ���� ��Ʈ����
	CWndFontColorInfo	m_stItemCountShadow;					// ������ ���� �׸��� ��Ʈ����

	stWndElldinsJar()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stItemCount.SetFontColorInfo( 176, 176, 240 );
			m_stItemCountShadow.SetFontColorInfo( 0, 0, 255 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stItemCount.SetFontColorInfo( 136, 255, 133 );
			m_stItemCountShadow.SetFontColorInfo( 0, 0, 0 );
		}
	}

}FONTCOLOR_WNDELLDINSJAR, *PFONTCOLOR_WNDELLDINSJAR;


//---------------------------------------------------------------------------------------------
// ������ �κ��丮 ( �κ��丮 ���� �ִ� ����... ���̹� ���� ������ )
//---------------------------------------------------------------------------------------------
typedef struct stWndInventory
{
	CWndFontColorInfo	m_stItemCount;							// ������ ���� ��Ʈ����
	CWndFontColorInfo	m_stItemCountShadow;					// ������ ���� �׸��� ��Ʈ����

	stWndInventory()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stItemCount.SetFontColorInfo( 176, 176, 240 );
			m_stItemCountShadow.SetFontColorInfo( 0, 0, 255 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stItemCount.SetFontColorInfo( 136, 255, 133 );
			m_stItemCountShadow.SetFontColorInfo( 0, 0, 0 );
		}
	}

}FONTCOLOR_WNDINVENTORY, *PFONTCOLOR_WNDINVENTORY;


//---------------------------------------------------------------------------------------------
// ������ ������ ��Ʈ�� ( �κ��丮 �ؿ� �����۵�... ���̹� ���� ������ )
//---------------------------------------------------------------------------------------------
typedef struct stWndItemCtrl
{
	CWndFontColorInfo	m_stItemCount;							// ������ ���� ��Ʈ����
	CWndFontColorInfo	m_stItemCountShadow;					// ������ ���� �׸��� ��Ʈ����

	stWndItemCtrl()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stItemCount.SetFontColorInfo( 176, 176, 240 );
			m_stItemCountShadow.SetFontColorInfo( 0, 0, 255 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stItemCount.SetFontColorInfo( 136, 255, 133 );
			m_stItemCountShadow.SetFontColorInfo( 0, 0, 0 );
		}
	}

}FONTCOLOR_WNDITEMCTRL, *PFONTCOLOR_WNDITEMCTRL;


//---------------------------------------------------------------------------------------------
// ������ ���� ������
//---------------------------------------------------------------------------------------------
typedef struct stWndPostSend
{
	CWndFontColorInfo	m_stItemCount;							// ������ ���� ��Ʈ����
	CWndFontColorInfo	m_stItemCountShadow;					// ������ ���� �׸��� ��Ʈ����

	stWndPostSend()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stItemCount.SetFontColorInfo( 176, 176, 240 );
			m_stItemCountShadow.SetFontColorInfo( 0, 0, 255 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stItemCount.SetFontColorInfo( 136, 255, 133 );
			m_stItemCountShadow.SetFontColorInfo( 0, 0, 0 );
		}
	}

}FONTCOLOR_WNDPOSTSEND, *PFONTCOLOR_WNDPOSTSEND;


//---------------------------------------------------------------------------------------------
// ������ ���� �ޱ�
//---------------------------------------------------------------------------------------------
typedef struct stWndPostReceive
{
	CWndFontColorInfo	m_stItemCount;							// ������ ���� ��Ʈ����
	CWndFontColorInfo	m_stItemCountShadow;					// ������ ���� �׸��� ��Ʈ����

	stWndPostReceive()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stItemCount.SetFontColorInfo( 176, 176, 240 );
			m_stItemCountShadow.SetFontColorInfo( 0, 0, 255 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stItemCount.SetFontColorInfo( 136, 255, 133 );
			m_stItemCountShadow.SetFontColorInfo( 0, 0, 0 );
		}
	}

}FONTCOLOR_WNDPOSTRECEIVE, *PFONTCOLOR_WNDPOSTRECEIVE;


//---------------------------------------------------------------------------------------------
// ������ ���� �б�
//---------------------------------------------------------------------------------------------
typedef struct stWndPostRead
{
	CWndFontColorInfo	m_stItemCount;							// ������ ���� ��Ʈ����
	CWndFontColorInfo	m_stItemCountShadow;					// ������ ���� �׸��� ��Ʈ����

	stWndPostRead()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stItemCount.SetFontColorInfo( 176, 176, 240 );
			m_stItemCountShadow.SetFontColorInfo( 0, 0, 255 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stItemCount.SetFontColorInfo( 136, 255, 133 );
			m_stItemCountShadow.SetFontColorInfo( 0, 0, 0 );
		}
	}

}FONTCOLOR_WNDPOSTREAD, *PFONTCOLOR_WNDPOSTREAD;


//---------------------------------------------------------------------------------------------
// ������ ��� ��� ��
//---------------------------------------------------------------------------------------------
typedef struct stWndGuildTabMember
{
	CWndFontColorInfo	m_stSelect;								// ���� ��Ʈ����
	CWndFontColorInfo	m_stOffLine;							// �������� ��Ʈ����

	stWndGuildTabMember()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stSelect.SetFontColorInfo( 0, 0, 255 );
			m_stOffLine.SetFontColorInfo( 144, 144, 144 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stSelect.SetFontColorInfo( 39, 196, 203 );
			m_stOffLine.SetFontColorInfo( 82, 82, 82 );
		}
	}

}FONTCOLOR_WNDGUILDTABMEMBER, *PFONTCOLOR_WNDGUILDTABMEMBER;



//---------------------------------------------------------------------------------------------
// ����
//---------------------------------------------------------------------------------------------
typedef struct stWndBuffStatus
{
	CWndFontColorInfo	m_stBuffSkillTime;						// ��ų ���� ���� �ð� ��Ʈ����
	CWndFontColorInfo	m_stBuffSkillTimeShadow;				// ��ų ���� ���� �ð� �׸��� ��Ʈ����

	stWndBuffStatus()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stBuffSkillTime.SetFontColorInfo( 255, 255, 255 );
			m_stBuffSkillTimeShadow.SetFontColorInfo( 0, 0, 0 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stBuffSkillTime.SetFontColorInfo( 118, 255, 177 );
			m_stBuffSkillTimeShadow.SetFontColorInfo( 0, 0, 0 );
		}
	}

}FONTCOLOR_WNDBUFFSTATUS, *PFONTCOLOR_WNDBUFFSTATUS;


//---------------------------------------------------------------------------------------------
// ����
//---------------------------------------------------------------------------------------------
typedef struct stWndMgr
{
	CWndFontColorInfo	m_stBuffSkillToolTipAbility;						// ��ų ���� ������ �ɷ�ġ �ɼ� ��Ʈ����

	stWndMgr()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stBuffSkillToolTipAbility.SetFontColorInfo( 0, 0, 255 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stBuffSkillToolTipAbility.SetFontColorInfo( 97, 97, 255 );
		}
	}

}FONTCOLOR_WNDMGR, *PFONTCOLOR_WNDMGR;


//---------------------------------------------------------------------------------------------
// ���帮���� ����
//---------------------------------------------------------------------------------------------
typedef struct stWndMadrigalGift
{
	CWndFontColorInfo	m_stToolTip;							// ���� ��Ʈ����

	stWndMadrigalGift()
	{
		m_stToolTip.SetFontColorInfo( 75, 145, 220 );					
	}

}FONTCOLOR_WNDMADRIGALGIFT, *PFONTCOLOR_WNDMADRIGALGIFT;


//---------------------------------------------------------------------------------------------
// ������ ���� �ý���
//---------------------------------------------------------------------------------------------
typedef struct stWndFlyffPieceSystem
{
	CWndFontColorInfo	m_stGuildText;							// �ȳ� ���� ��Ʈ����
	CWndFontColorInfo	m_stLoadingText;						// �ε� ���� ��Ʈ����
	CWndFontColorInfo	m_stItemListEmptyText;					// ������ ����Ʈ�� ����ִٴ� ���� ��Ʈ����


	stWndFlyffPieceSystem()
	{
		m_stGuildText.SetFontColorInfo( 255, 255, 255 );
		m_stLoadingText.SetFontColorInfo( 255, 255, 255 );
		m_stItemListEmptyText.SetFontColorInfo( 255, 255, 255 );
	}

}FONTCOLOR_WNDFLYFFPIECESYSTEM, *PFONTCOLOR_WNDFLYFFPIECESYSTEM;


//---------------------------------------------------------------------------------------------
// ������ ����
//---------------------------------------------------------------------------------------------
typedef struct stWndWorld
{
	CWndFontColorInfo	m_stExpBuffIconToolTipItemName;			// ����ġ ���� ������ ���� ������ �̸� ����

	stWndWorld()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stExpBuffIconToolTipItemName.SetFontColorInfo( 0, 0, 255 );
		}
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stExpBuffIconToolTipItemName.SetFontColorInfo( 47, 190, 109 );
		}
	}

}FONTCOLOR_WNDWORLD, *PFONTCOLOR_WNDWORLD;


//---------------------------------------------------------------------------------------------
// ������ ������ �������� ��Ʈ��
//---------------------------------------------------------------------------------------------
typedef struct stWndCommItemCtrl
{
	CWndFontColorInfo	m_stItemName;						// ������ �̸� ����
	CWndFontColorInfo	m_stToolTipItemName;				// ���� ������ �̸� ����
	
	stWndCommItemCtrl()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stItemName.SetFontColorInfo( 0, 0, 255 );
			m_stToolTipItemName.SetFontColorInfo( 0, 0, 255 );
		}

		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stItemName.SetFontColorInfo( 47, 190, 109 );
			m_stToolTipItemName.SetFontColorInfo( 47, 190, 109 );
		}
	}

}FONTCOLOR_WNDCOMMITEMCTRL, *PFONTCOLOR_WNDCOMMITEMCTRL;


//---------------------------------------------------------------------------------------------
// ������ �ν��Ͻ� �޽���
//---------------------------------------------------------------------------------------------
typedef struct stWndInstantMsg
{
	CWndFontColorInfo	m_stMessage;						// �޽��� ����
	
	stWndInstantMsg()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stMessage.SetFontColorInfo( 0, 0, 255 );
		}

		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stMessage.SetFontColorInfo( 246, 204, 77 );
		}
	}

}FONTCOLOR_WNDINSTANTMSG, *PFONTCOLOR_WNDINSTANTMSG;


//---------------------------------------------------------------------------------------------
// ���� ������ ��ȯ â
//---------------------------------------------------------------------------------------------
typedef struct stWndCommonItemExchange
{
	CWndFontColorInfo	m_stItemCount;							// ������ ���� ��Ʈ����
	CWndFontColorInfo	m_stItemCountShadow;					// ������ ���� �׸��� ��Ʈ����
	CWndFontColorInfo	m_stNotEnoughItemCount;					// ������ ���� ������ ���� ��Ʈ����

	stWndCommonItemExchange()
	{
		m_stItemCount.SetFontColorInfo( 136, 255, 133 );
		m_stItemCountShadow.SetFontColorInfo( 0, 0, 0 );
		m_stNotEnoughItemCount.SetFontColorInfo( 255, 0, 0 );
	}

}FONTCOLOR_WNDCOMMONITEMEXCHANGE, *PFONTCOLOR_WNDCOMMONITEMEXCHANGE;