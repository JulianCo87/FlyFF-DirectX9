

#pragma once

#include "FLColorInfo.h"


//---------------------------------------------------------------------------------------------
// �ǵ��� ����ü �̸��� ��Ʈ Į�� ���� Ŭ������ �����ϰ� ���ּ��� :)
//---------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------
// ������ ��ư ��Ʈ��
//---------------------------------------------------------------------------------------------
typedef struct stFLWndButton
{

	CFLColorInfo	m_stHighLightFill;							// ���̶���Ʈ ä�� ����
	CFLColorInfo	m_stHighLightRound;							// ���̶���Ʈ �׵θ� ����
	CFLColorInfo	m_stMenuTriangleFill;						// �޴� �ﰢ�� ä�� ����
	CFLColorInfo	m_stTopDownMenuTriangleFill;				// ž �ٿ� �޴� �ﰢ�� ä�� ����

	CFLColorInfo	m_stMainMenuCheckIcon;						// ���� �޴� üũ ������ ����

	stFLWndButton()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stHighLightFill.SetColorInfo( 200, 200, 200, 155 );
			m_stHighLightRound.SetColorInfo( 240, 240, 240, 200 );
			m_stMenuTriangleFill.SetColorInfo( 0, 160, 255 );
			m_stTopDownMenuTriangleFill.SetColorInfo( 240, 240, 240, 200 );

			m_stMainMenuCheckIcon.SetColorInfo( 0, 0, 0 );

		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stHighLightFill.SetColorInfo( 200, 200, 200, 155 );
			m_stHighLightRound.SetColorInfo( 240, 240, 240, 200 );
			m_stMenuTriangleFill.SetColorInfo( 246, 204, 77 );
			m_stTopDownMenuTriangleFill.SetColorInfo( 68, 140, 203 );

			m_stMainMenuCheckIcon.SetColorInfo( 246, 204, 77 );
		}
	}

}FLCOLOR_WNDBUTTON, *PFLCOLOR_WNDBUTTON;


//---------------------------------------------------------------------------------------------
// ������ ��Ƽ ���� ( �ش� ���� )
//---------------------------------------------------------------------------------------------
typedef struct stFLWndPartyInfo
{

	CFLColorInfo	m_stStatusBoxOutLine;					// ���� �ڽ� �׵θ� ����
	CFLColorInfo	m_stStatusBoxFill_Normal;				// ���� �ڽ� ä�� ����_����
	CFLColorInfo	m_stStatusBoxFill_Die;					// ���� �ڽ� ä�� ����_����
	CFLColorInfo	m_stStatusBoxFill_LowHP;				// ���� �ڽ� ä�� ����_����� ����
	CFLColorInfo	m_stStatusBoxFill_NotFoundFromAround;	// ���� �ڽ� ä�� ����_�������� ã�� �� ����
	CFLColorInfo	m_stStatusBoxFill_Disconnect;			// ���� �ڽ� ä�� ����_������ ����

	CFLColorInfo	m_stSelectMember;						// �ش� �� ���� ����


	stFLWndPartyInfo()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stStatusBoxOutLine.SetColorInfo( 0, 0, 0 );

			m_stStatusBoxFill_Normal.SetColorInfo( 0, 0, 255 );
			m_stStatusBoxFill_Die.SetColorInfo( 255, 0, 0 );					
			m_stStatusBoxFill_LowHP.SetColorInfo( 255, 255, 0 );				
			m_stStatusBoxFill_NotFoundFromAround.SetColorInfo( 135, 135, 135 );	
			m_stStatusBoxFill_Disconnect.SetColorInfo( 0, 0, 0 );	

			m_stSelectMember.SetColorInfo( 255, 255, 0, 96 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stStatusBoxOutLine.SetColorInfo( 255, 255, 255 );

			m_stStatusBoxFill_Normal.SetColorInfo( 0, 138, 255 );				
			m_stStatusBoxFill_Die.SetColorInfo( 115, 0, 0 );					
			m_stStatusBoxFill_LowHP.SetColorInfo( 192, 104, 1 );				
			m_stStatusBoxFill_NotFoundFromAround.SetColorInfo( 135, 135, 135 );	
			m_stStatusBoxFill_Disconnect.SetColorInfo( 15, 39, 39 );			

			m_stSelectMember.SetColorInfo( 255, 245, 140, 96 );
		}
	}

}FLCOLOR_WNDPARTYINFO, *PFLCOLOR_WNDPARTYINFO;


//---------------------------------------------------------------------------------------------
// ������ ��� �Ͽ�¡
//---------------------------------------------------------------------------------------------
typedef struct stFLWndGuildHousing
{
	CFLColorInfo	m_stSelectBoxFill;						// ���ùڽ� ä�����

	stFLWndGuildHousing()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stSelectBoxFill.SetColorInfo( 255, 0, 0, 85 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stSelectBoxFill.SetColorInfo( 255, 0, 0, 85 );
		}
	}

}FLCOLOR_WNDGUILDHOUSING, *PFLCOLOR_WNDGUILDHOUSING;



//---------------------------------------------------------------------------------------------
// ������ ��� �Ͽ�¡���� ���� ��ġ �� 
//---------------------------------------------------------------------------------------------
typedef struct stFLWndGHousingShowOneUnit
{
	CFLColorInfo	m_stBoxFill;							// �ڽ� ä�����

	stFLWndGHousingShowOneUnit()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stBoxFill.SetColorInfo( 255, 0, 0, 85 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stBoxFill.SetColorInfo( 255, 0, 0, 85 );
		}
	}

}FLCOLOR_WNDGHOUSINGSHOWONEUNIT, *PFLCOLOR_WNDGHOUSINGSHOWONEUNIT;


//---------------------------------------------------------------------------------------------
// ������ ��� ���� ���� ����
//---------------------------------------------------------------------------------------------
typedef struct stFLWndGuildCombatRank_Class
{
	CFLColorInfo	m_stLineUp;							// �� ���� �� ä�����
	CFLColorInfo	m_stLineDown;						// �� �Ʒ� �� ä�����

	CFLColorInfo	m_stSelectBoxFill;					// ���ùڽ� ä�����

	stFLWndGuildCombatRank_Class()
	{
		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stLineUp.SetColorInfo( 0, 0, 0, 128 );
			m_stLineDown.SetColorInfo( 0, 0, 0, 128  );

			m_stSelectBoxFill.SetColorInfo( 0, 0, 0, 64 );
		}
	
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		{
			m_stLineUp.SetColorInfo( 246, 204, 77, 255 );
			m_stLineDown.SetColorInfo( 246, 204, 77, 255 );

			m_stSelectBoxFill.SetColorInfo( 255, 245, 140, 96 );
		}
	}

}FLCOLOR_WNDGUILDCOMBATRANK_CLASS, *PFLCOLOR_WNDGUILDCOMBATRANK_CLASS;


//---------------------------------------------------------------------------------------------
// ���帮���� ����
//---------------------------------------------------------------------------------------------
typedef struct stFLWndMadrigalGift
{
	CFLColorInfo	m_stRoundGuage_StepNone;			// �� ������ �ܰ� ���� �� ä�� ����
	CFLColorInfo	m_stRoundGuage_Step1;				// �� ������ 1�ܰ� ä�����
	CFLColorInfo	m_stRoundGuage_Step2;				// �� ������ 2�ܰ� ä�����
	CFLColorInfo	m_stRoundGuage_Step3;				// �� ������ 3�ܰ� ä�����

	stFLWndMadrigalGift()
	{
		m_stRoundGuage_StepNone.SetColorInfo( 0, 255, 0 );
		m_stRoundGuage_Step1.SetColorInfo( 255, 255, 0 );
		m_stRoundGuage_Step2.SetColorInfo( 255, 0, 0 );
		m_stRoundGuage_Step3.SetColorInfo( 255, 0, 0 );
	}

}FLCOLOR_WNDMADRIGALGIFT, *PFLCOLOR_WNDMADRIGALGIFT;


//---------------------------------------------------------------------------------------------
// ������ ���� �ý���
//---------------------------------------------------------------------------------------------
typedef struct stFLWndFlyffPieceSystem
{
	CFLColorInfo	m_stListBoxLineColor;				// ����Ʈ �ڽ� �� ����
	CFLColorInfo	m_stListBoxSelectRect;				// ����Ʈ ���� �׵θ� ����
	CFLColorInfo	m_stListBoxSelectFillRect;			// ����Ʈ ���� ä�� ����


	stFLWndFlyffPieceSystem()
	{
		m_stListBoxLineColor.SetColorInfo( 119, 102, 85 );
		m_stListBoxSelectRect.SetColorInfo( 0x7711ff11 );
		m_stListBoxSelectFillRect.SetColorInfo( 0x2211ff11 );
	}

}FLCOLOR_WNDFLYFFPIECESYSTEM, *PFLCOLOR_WNDFLYFFPIECESYSTEM;


//---------------------------------------------------------------------------------------------
// ���� ������ ��ȯ ������
//---------------------------------------------------------------------------------------------
typedef struct stFLWndCommonItemExchange
{
	CFLColorInfo	m_stListBoxLineColor;				// ����Ʈ �ڽ� �� ����
	CFLColorInfo	m_stListBoxSelectRect;				// ����Ʈ ���� �׵θ� ����
	CFLColorInfo	m_stListBoxSelectFillRect;			// ����Ʈ ���� ä�� ����

	stFLWndCommonItemExchange()
	{
		m_stListBoxLineColor.SetColorInfo( 119, 102, 85 );
		m_stListBoxSelectRect.SetColorInfo( 0x7711ff11 );
		m_stListBoxSelectFillRect.SetColorInfo( 0x2211ff11 );
	}

}FLCOLOR_WNDCOMMONITEMEXCHANGE, *PFLCOLOR_WNDCOMMONITEMEXCHANGE;
