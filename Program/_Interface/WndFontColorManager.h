

#pragma once


#include "WndFontColor.h"


//---------------------------------------------------------------------------------------------
// ������ ��Ʈ ���� �޴��� Ŭ����
//---------------------------------------------------------------------------------------------
class CWndFontColorManager 
{

public:

												CWndFontColorManager();
												~CWndFontColorManager();

	static	CWndFontColorManager*				GetInstance( void );

	// ������ ��Ʈ��	
	PFONTCOLOR_WNDBASE							GetWndBase( void )						{	return &m_stWndBase;					}
	PFONTCOLOR_WNDTEXT							GetWndText( void )						{	return &m_stWndText;					}
	PFONTCOLOR_WNDBUTTON						GetWndButton( void )					{	return &m_stWndButton;					}
	PFONTCOLOR_WNDDIALOG						GetWndDialog( void )					{	return &m_stWndDialog;					}
	PFONTCOLOR_WNDTABCTRL						GetWndTabCtrl( void )					{	return &m_stWndTabCtrl;					}	
	PFONTCOLOR_WNDLISTBOX						GetWndListBox( void )					{	return &m_stWndListBox;					}
	PFONTCOLOR_WNDTREECTRL						GetWndTreeCtrl( void )					{	return &m_stWndTreeCtrl;				}
	PFONTCOLOR_EDITSTRING						GetEditString( void	)					{	return &m_stEditString;					}

	// ������
	PFONTCOLOR_WNDSELECTSERVER					GetWndSelectServer( void )				{	return &m_stWndSelectServer;			}
	PFONTCOLOR_WNDQUESTDETAIL					GetWndQuestDetail( void )				{	return &m_stWndQuestDetail;				}
	PFONTCOLOR_WNDTASKBAR						GetWndTaskBar( void )					{	return &m_stWndTaskBar;					}	
	PFONTCOLOR_WNDTOOLTIP						GetWndToolTip( void )					{	return &m_stWndToolTip;					}
	PFONTCOLOR_WNDCHARINFO						GetWndCharInfo( void )					{	return &m_stWndCharInfo;				}
	PFONTCOLOR_WNDPETSTATUS						GetWndPetStatus( void )					{	return &m_stWndPetStatus;				}
	PFONTCOLOR_WNDPARTYINFO						GetWndPartyInfo( void )					{	return &m_stWndPartyInfo;				}
	PFONTCOLOR_WNDPARTYSKILL					GetWndPartySkill( void )				{	return &m_stWndPartySkill;				}
	PFONTCOLOR_WNDMESSENGEREX					GetWndMessengerEx( void )				{	return &m_stWndMessengerEx;				}
	PFONTCOLOR_WNDFRIENDCTRLEX					GetWndFriendCtrlEx( void )				{	return &m_stWndFriendCtrlEx;			}
	PFONTCOLOR_WNDGUILDCTRLEX					GetWndGuildCtrlEx( void )				{	return &m_stWndGuildCtrlEx;				}
	PFONTCOLOR_WNDCAMPUS						GetWndCampus( void )					{	return &m_stWndCampus;					}
	PFONTCOLOR_WNDHOUSING						GetWndHousing( void )					{	return &m_stWndHousing;					}
	PFONTCOLOR_WNDGUILDHOUSING					GetWndGuildHousing( void )				{	return &m_stWndGuildHousing;			}
	PFONTCOLOR_WNDGUILDMETRICCTRL				GetWndGuildMetricCtrl( void )			{	return &m_stWndGuildMetricCtrl;			}
	PFONTCOLOR_WNDGUILDCOMBATRANK_CLASS			GetWndGuildCombatRank_Class( void )		{	return &m_stWndGuildCombatRank_Class;	}
	PFONTCOLOR_WNDSHOP_CART						GetWndShop_Cart( void )					{	return &m_stWndShop_Cart;				}
	PFONTCOLOR_WNDELLDINSJAR					GetWndElldinsJar( void )				{	return &m_stWndElldinsJar;				}
	PFONTCOLOR_WNDINVENTORY						GetWndInventory( void )					{	return &m_stWndInventory;				}
	PFONTCOLOR_WNDITEMCTRL						GetWndItemCtrl( void )					{	return &m_stWndItemCtrl;				}
	PFONTCOLOR_WNDPOSTSEND						GetWndPostSend( void )					{	return &m_stWndPostSend;				}
	PFONTCOLOR_WNDPOSTRECEIVE					GetWndPostReceive( void )				{	return &m_stWndPostReceive;				}
	PFONTCOLOR_WNDPOSTREAD						GetWndPostRead( void )					{	return &m_stWndPostRead;				}
	PFONTCOLOR_WNDGUILDTABMEMBER				GetWndGuildTabMember( void )			{	return &m_stWndGuildTabMember;			}
	PFONTCOLOR_WNDBUFFSTATUS					GetWndBuffStatus( void )				{	return &m_stWndBuffStatus;				}
	PFONTCOLOR_WNDMGR							GetWndMgr( void )						{	return &m_stWndMgr;						}
	PFONTCOLOR_WNDMADRIGALGIFT					GetWndMadrigalGift( void )				{	return &m_stWndMadrigalGift;			}
	PFONTCOLOR_WNDFLYFFPIECESYSTEM				GetWndFlyffPieceSystem( void )			{	return &m_stWndFlyffPieceSystem;		}	
	PFONTCOLOR_WNDWORLD							GetWndWorld( void )						{	return &m_stWndWorld;					}	
	PFONTCOLOR_WNDCOMMITEMCTRL					GetWndCommItemCtrl( void )				{	return &m_stWndCommItemCtrl;			}	
	PFONTCOLOR_WNDINSTANTMSG					GetWndInstantMsg( void )				{	return &m_stWndInstantMsg;				}	
	PFONTCOLOR_WNDCOMMONITEMEXCHANGE			GetWndCommonItemExchange( void )		{	return &m_stWndCommonItemExchange;		}


protected:


	FONTCOLOR_WNDBASE							m_stWndBase;
	FONTCOLOR_WNDTEXT							m_stWndText;
	FONTCOLOR_WNDBUTTON							m_stWndButton;
	FONTCOLOR_WNDDIALOG							m_stWndDialog;
	FONTCOLOR_WNDTABCTRL						m_stWndTabCtrl;
	FONTCOLOR_WNDLISTBOX						m_stWndListBox;
	FONTCOLOR_WNDTREECTRL						m_stWndTreeCtrl;
	FONTCOLOR_EDITSTRING						m_stEditString;		


	FONTCOLOR_WNDSELECTSERVER					m_stWndSelectServer;			// ���� ����
	FONTCOLOR_WNDQUESTDETAIL					m_stWndQuestDetail;				// ����Ʈ
	FONTCOLOR_WNDTOOLTIP						m_stWndToolTip;					// ����
	FONTCOLOR_WNDTASKBAR						m_stWndTaskBar;					// �۾� ��
	FONTCOLOR_WNDCHARINFO						m_stWndCharInfo;				// ĳ���� ����
	FONTCOLOR_WNDPETSTATUS						m_stWndPetStatus;				// �� ����
	FONTCOLOR_WNDPARTYINFO						m_stWndPartyInfo;				// �ش�
	FONTCOLOR_WNDPARTYSKILL						m_stWndPartySkill;				// �ش� ��ų
	FONTCOLOR_WNDMESSENGEREX					m_stWndMessengerEx;				// �޽���
	FONTCOLOR_WNDFRIENDCTRLEX					m_stWndFriendCtrlEx;			// �޽��� (ģ��)
	FONTCOLOR_WNDGUILDCTRLEX					m_stWndGuildCtrlEx;				// �޽��� (���)
	FONTCOLOR_WNDCAMPUS							m_stWndCampus;					// �޽��� (����)
	FONTCOLOR_WNDHOUSING						m_stWndHousing;					// �Ͽ�¡ (�̴Ϸ�)
	FONTCOLOR_WNDGUILDHOUSING					m_stWndGuildHousing;			// �Ͽ�¡ (���)
	FONTCOLOR_WNDGUILDMETRICCTRL				m_stWndGuildMetricCtrl;			// ��� ����
	FONTCOLOR_WNDGUILDCOMBATRANK_CLASS			m_stWndGuildCombatRank_Class;	// ��� ���� ���� ����
	FONTCOLOR_WNDSHOP_CART						m_stWndShop_Cart;				// ���� (��ٱ��� ����)
	FONTCOLOR_WNDELLDINSJAR						m_stWndElldinsJar;				// ������ �׾Ƹ�
	FONTCOLOR_WNDINVENTORY						m_stWndInventory;				// �κ��丮 (���)
	FONTCOLOR_WNDITEMCTRL						m_stWndItemCtrl;				// �κ��丮 (������)
	FONTCOLOR_WNDPOSTSEND						m_stWndPostSend;				// ���� (������)
	FONTCOLOR_WNDPOSTRECEIVE					m_stWndPostReceive;				// ���� (�ޱ�)
	FONTCOLOR_WNDPOSTREAD						m_stWndPostRead;				// ���� (�б�)
	FONTCOLOR_WNDGUILDTABMEMBER					m_stWndGuildTabMember;			// ��� (���)
	FONTCOLOR_WNDBUFFSTATUS						m_stWndBuffStatus;				// ���� 
	FONTCOLOR_WNDMGR							m_stWndMgr;						// ������ �޴���
	FONTCOLOR_WNDMADRIGALGIFT					m_stWndMadrigalGift;			// ���帮���� ����
	FONTCOLOR_WNDFLYFFPIECESYSTEM				m_stWndFlyffPieceSystem;		// ������ ���� �ý���
	FONTCOLOR_WNDWORLD							m_stWndWorld;					// ����
	FONTCOLOR_WNDCOMMITEMCTRL					m_stWndCommItemCtrl;			// ������ �������� ��Ʈ��
	FONTCOLOR_WNDINSTANTMSG						m_stWndInstantMsg;				// �ν���Ʈ �޽��� ( �˸� â )
	FONTCOLOR_WNDCOMMONITEMEXCHANGE				m_stWndCommonItemExchange;		// ���� ������ ��ȯ â

};

#define g_WndFontColorManager					CWndFontColorManager::GetInstance()
