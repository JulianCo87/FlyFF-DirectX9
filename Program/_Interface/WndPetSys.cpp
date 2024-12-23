#include "stdafx.h"
#include "WndManager.h"
#include "WndPetSys.h"

//sun: 12, 펫 각성
CWndPetAwakCancel::CWndPetAwakCancel() 
{ 

	m_pItemElem  = NULL;
	m_pEItemProp = NULL;
	m_pTexture   = NULL;
	
} 
CWndPetAwakCancel::~CWndPetAwakCancel() 
{ 
} 
void CWndPetAwakCancel::OnDraw( C2DRender* p2DRender ) 
{ 
	PT_ITEM_SPEC pItemProp;
	
	if(m_pItemElem != NULL)
	{
		pItemProp = m_pItemElem->GetProp();
		LPWNDCTRL wndCtrl = GetWndCtrl( WIDC_STATIC1 );
		
		if(m_pTexture != NULL)
			m_pTexture->Render( p2DRender, CPoint( wndCtrl->rect.left, wndCtrl->rect.top ) );
	} 
} 
void CWndPetAwakCancel::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// 여기에 코딩하세요
	CWndButton* pButton = (CWndButton*)GetDlgItem(WIDC_BUTTON1);

	if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
	{
		if(g_xFlyffConfig->GetMainLanguage() == LANG_FRE)
			pButton->SetTexture(g_Neuz.m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), _T( "ButOk2.bmp" ) ), TRUE);
	}

	pButton->EnableWindow(FALSE);
	m_pText = (CWndText*)GetDlgItem( WIDC_TEXT1 );

	SetDescription();
	
	MoveParentCenter();
} 
// 처음 이 함수를 부르면 윈도가 열린다.
BOOL CWndPetAwakCancel::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy에서 설정한 리소스로 윈도를 연다.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_PET_AWAK_CANCEL, 0, CPoint( 0, 0 ), pWndParent );
} 


void CWndPetAwakCancel::OnDestroy()
{
	if(m_pItemElem != NULL)
	{
		if( !g_pPlayer->m_vtInfo.IsTrading( m_pItemElem ) )
			m_pItemElem->SetExtra(0);
	}
}

BOOL CWndPetAwakCancel::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndPetAwakCancel::OnSize( UINT nType, int cx, int cy ) 
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndPetAwakCancel::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndPetAwakCancel::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndPetAwakCancel::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_BUTTON1 )
	{
		if(m_pItemElem != NULL)
		{
			CWndButton* pButton;
			pButton = (CWndButton*)GetDlgItem( WIDC_BUTTON1 );
			pButton->EnableWindow(FALSE);

			FLPacketItemUpgradeRandomOptionInitializeReq kPacket;

			kPacket.dwMainItemObjID			= m_pItemElem->m_dwObjId;
			
			g_DPlay.SendPacket( &kPacket );

			Destroy();
		}
	}
	
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

void CWndPetAwakCancel::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	if(!m_pItemElem) return;
	CRect rect;
	LPWNDCTRL wndCtrl = GetWndCtrl( WIDC_STATIC1 );
	rect = wndCtrl->rect;
	if( rect.PtInRect( point ) )
	{
		m_pItemElem->SetExtra(0);
		CWndButton* pButton = (CWndButton*)GetDlgItem(WIDC_BUTTON1);
		pButton->EnableWindow(FALSE);
		m_pItemElem = NULL;
		m_pEItemProp = NULL;
		m_pTexture	= NULL;
	}
}

BOOL CWndPetAwakCancel::OnDropIcon( LPSHORTCUT pShortcut, CPoint point )
{
	
	FLItemElem* pTempElem= NULL;


	if(g_pPlayer != NULL) pTempElem = (FLItemElem*)g_pPlayer->GetItemId( pShortcut->m_dwId );
	if( pTempElem != NULL)
	{
		// 픽업펫 확인	// 각성 여부는 서버 검사
		if( !pTempElem->IsEatPet() )	
			return FALSE;
		// 확인 버튼 활성
		if(m_pItemElem) m_pItemElem->SetExtra(0);
		m_pItemElem = (FLItemElem*)g_pPlayer->GetItemId( pShortcut->m_dwId );
		m_pEItemProp = m_pItemElem->GetProp();
		m_pItemElem->SetExtra(m_pItemElem->GetExtra()+1);
		if(m_pEItemProp != NULL)
		{
			m_pTexture = m_pItemElem->m_pTexture;
			//m_pTexture = CWndBase::m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, m_pEItemProp->szIcon), 0xffff00ff );
		}
		CWndButton* pButton = (CWndButton*)GetDlgItem(WIDC_BUTTON1);
		pButton->EnableWindow(TRUE);
	}

	return TRUE;
}

void CWndPetAwakCancel::SetDescription()
{

	CScript scanner;
	BOOL checkflag;
	CHAR* szChar;


	checkflag = scanner.Load( MakePath( DIR_CLIENT,  _T( "PetAwakCancel.inc" ) ));
	szChar = scanner.m_pProg;
	
	if(m_pText != NULL && checkflag)
	{
		m_pText->m_string.AddParsingString( szChar );
		m_pText->ResetString();	
	}
}


//---------------------------------------------------------------------------------------------
// 펫 상태 윈도우
//---------------------------------------------------------------------------------------------

int s_nRearPetX = 0;
int s_nRearPetY = 0;

//---------------------------------------------------------------------------------------------
// 생성자
//---------------------------------------------------------------------------------------------
CWndPetStatus::CWndPetStatus()
:	m_pVBHPGauge( NULL )
,	m_pVBEXPGauge( NULL )
,	m_pPetModel( NULL )
,	m_pPetElem( NULL )
,	m_bVBHPGauge( TRUE )
,	m_bVBEXPGauge( TRUE )
,	m_bHPVisible( TRUE )
,	m_bExpVisible( TRUE )
,	m_bExtension( FALSE )
,	m_nHPWidth( -1 )
,	m_nEXPWidth( -1 )
,	m_pTexPetLvBg( NULL )
,	m_pTexPetStatusBg( NULL )
,	m_dwPropertyNameColor( NULL )
,	m_dwPropertyNameShadowColor( NULL )
,	m_dwPropertyValueColor( NULL )
,	m_dwGaugeValueColor( NULL )
,	m_nDisplay( 2 )
,	m_nChoiceLevel( -1 )
,	m_pGaugeHP( NULL )
,	m_pGaugeExp( NULL )
,	m_pGaugeBg( NULL )
{
	PFONTCOLOR_WNDPETSTATUS pFontColorWndPetStatus = g_WndFontColorManager->GetWndPetStatus();
	
	m_dwPropertyNameColor		= pFontColorWndPetStatus->m_stPropertyNameColor.GetFontColor();
	m_dwPropertyNameShadowColor = pFontColorWndPetStatus->m_stPropertyNameShadowColor.GetFontColor();
	m_dwPropertyValueColor		= pFontColorWndPetStatus->m_stPropertyValueColor.GetFontColor();
	m_dwGaugeValueColor			= pFontColorWndPetStatus->m_stGaugeValueColor.GetFontColor();

	ZeroMemory( m_aPetLv, sizeof( int ) * 6 );
	ZeroMemory( m_pTexPetLv, sizeof( CTexture* ) * 6 );

	m_nLockLevel[0]		= -1;
	m_nLockLevel[1]		= -1;
	m_nBackUpLevel[0]	= -1;
	m_nBackUpLevel[1]	= -1;
	
	m_strPathLvImage[0] = MakePath( DIR_ICON, "Icon_PetLevel1_S.dds");
	m_strPathLvImage[1] = MakePath( DIR_ICON, "Icon_PetLevel2_S.dds");
	m_strPathLvImage[2] = MakePath( DIR_ICON, "Icon_PetLevel3_S.dds");
	m_strPathLvImage[3] = MakePath( DIR_ICON, "Icon_PetLevel4_S.dds");
	m_strPathLvImage[4] = MakePath( DIR_ICON, "Icon_PetLevel5_S.dds");
	m_strPathLvImage[5] = MakePath( DIR_ICON, "Icon_PetLevel6_S.dds");
	m_strPathLvImage[6] = MakePath( DIR_ICON, "Icon_PetLevel7_S.dds");
	m_strPathLvImage[7] = MakePath( DIR_ICON, "Icon_PetLevel8_S.dds");
	m_strPathLvImage[8] = MakePath( DIR_ICON, "Icon_PetLevel9_S.dds");
	
	SetPetCamTable();
}


//---------------------------------------------------------------------------------------------
// 파괴자
//---------------------------------------------------------------------------------------------
CWndPetStatus::~CWndPetStatus()
{
	DeleteDeviceObjects();
}


//---------------------------------------------------------------------------------------------
// 펫 카메라 테이블 ( 헐... 누가 이렇게... )
//---------------------------------------------------------------------------------------------
void CWndPetStatus::SetPetCamTable()
{
	//백호
	m_PetCameTable[0].CamPos.x = 1.2f;
	m_PetCameTable[0].CamPos.y = 4.6f;
	m_PetCameTable[0].CamPos.z = -7.0f;
	m_PetCameTable[0].CamLook.x = -2.4f;
	m_PetCameTable[0].CamLook.y = 3.0f;
	m_PetCameTable[0].CamLook.z = 0.0f;
	m_PetCameTable[0].Scale = 6.0f;
	//바바리 사자
	m_PetCameTable[1].CamPos.x = 0.7f;
	m_PetCameTable[1].CamPos.y = 3.0f;
	m_PetCameTable[1].CamPos.z = -5.2f;
	m_PetCameTable[1].CamLook.x = -2.2f;
	m_PetCameTable[1].CamLook.y = 2.5f;
	m_PetCameTable[1].CamLook.z = 1.0f;
	m_PetCameTable[1].Scale = 6.0f;
	//토끼
	m_PetCameTable[2].CamPos.x = 2.0f;
	m_PetCameTable[2].CamPos.y = 2.2f;
	m_PetCameTable[2].CamPos.z = -4.0f;
	m_PetCameTable[2].CamLook.x = -2.2f;
	m_PetCameTable[2].CamLook.y = 1.0f;
	m_PetCameTable[2].CamLook.z = 3.0f;
	m_PetCameTable[2].Scale = 7.0f;
	//구미호
	m_PetCameTable[3].CamPos.x = 1.4f;
	m_PetCameTable[3].CamPos.y = 4.2f;
	m_PetCameTable[3].CamPos.z = -8.0f;
	m_PetCameTable[3].CamLook.x = -2.2f;
	m_PetCameTable[3].CamLook.y = 2.0f;
	m_PetCameTable[3].CamLook.z = 3.0f;
	m_PetCameTable[3].Scale = 6.0f;
	//새끼 드래곤
	m_PetCameTable[4].CamPos.x = 1.4f;
	m_PetCameTable[4].CamPos.y = 6.8f;
	m_PetCameTable[4].CamPos.z = -6.0f;
	m_PetCameTable[4].CamLook.x = -2.0f;
	m_PetCameTable[4].CamLook.y = 6.0f;
	m_PetCameTable[4].CamLook.z = 3.0f;
	m_PetCameTable[4].Scale = 5.5f;
	//새끼 그리핀
	m_PetCameTable[5].CamPos.x = 3.0f;
	m_PetCameTable[5].CamPos.y = 6.0f;
	m_PetCameTable[5].CamPos.z = -12.0f;
	m_PetCameTable[5].CamLook.x = -2.0f;
	m_PetCameTable[5].CamLook.y = 3.0f;
	m_PetCameTable[5].CamLook.z = 3.0f;
	m_PetCameTable[5].Scale = 4.5f;
	//유니콘
	m_PetCameTable[6].CamPos.x = 4.0f;
	m_PetCameTable[6].CamPos.y = 3.0f;
	m_PetCameTable[6].CamPos.z = -10.0f;
	m_PetCameTable[6].CamLook.x = -6.0f;
	m_PetCameTable[6].CamLook.y = 6.0f;
	m_PetCameTable[6].CamLook.z = 3.0f;
	m_PetCameTable[6].Scale = 5.0f;
}


void CWndPetStatus::MakeGaugeVertex()
{
	LPWNDCTRL lpHP   = GetWndCtrl( WIDC_CUSTOM2 );
	LPWNDCTRL lpExp  = GetWndCtrl( WIDC_CUSTOM3 );

	if( m_pPetElem != NULL && m_pPetElem->m_pPet != NULL )
	{
		CRect rect = GetClientRect();
		CRect rectTemp;
		
		int nWidthClient = lpHP->rect.Width();
		int nWidth;

		// HP
		nWidth = nWidthClient * m_pPetElem->m_pPet->GetEnergy() / m_pPetElem->m_pPet->GetMaxEnergy();
		if( m_nHPWidth != nWidth ) 
		{
			m_nHPWidth = nWidth;
			rect = lpHP->rect;
			rectTemp = rect; 
			rectTemp.right = rectTemp.left + nWidth;
			ClientToScreen( rectTemp );
			m_bVBHPGauge = m_pTheme->MakeGaugeVertex( m_pApp->m_pd3dDevice, &rectTemp, 0x64ff0000, m_pVBHPGauge, &m_texGaugeHP );
		}

		// Exp
		nWidth	= (int)( ( (float) nWidthClient / m_pPetElem->m_pPet->GetMaxExp() ) * m_pPetElem->m_pPet->GetExp() );
		if( m_nEXPWidth != nWidth ) 
		{
			m_nEXPWidth = nWidth;
			rect = lpExp->rect;
			rectTemp = rect; 
			rectTemp.right = rectTemp.left + nWidth;
			ClientToScreen( rectTemp );
			m_bVBEXPGauge = m_pTheme->MakeGaugeVertex( m_pApp->m_pd3dDevice, &rectTemp, 0x847070ff, m_pVBEXPGauge, &m_texGaugeHP );
		}
	}
}

void CWndPetStatus::PaintFrame( C2DRender* p2DRender )
{
	if(!IsValidObj(g_pPlayer))
		return;

	CRect	rect( GetWindowRect() );
	CPoint	TitleStringPosition( CalcTitleStringPosition() );
	
	if( g_pPlayer->HasActivatedSystemPet() )
	{
		m_pPetElem = g_pPlayer->GetPetItem();
		
		if( m_pTexture && m_pPetElem != NULL && m_pPetElem->m_pPet != NULL)
		{
			RenderWnd();
			// 여기는 타이틀 바의 텍스트를 출력하는 곳 
			if( IsWndStyle( WBS_CAPTION ) )	
			{
				CD3DFont* pOldFont = p2DRender->GetFont();
				p2DRender->SetFont( CWndBase::m_Theme.m_pFontWndTitle );
				
				if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
					p2DRender->TextOut( 10, 4, m_strTitle, m_dwColor );
				if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
					p2DRender->TextOut( TitleStringPosition.x, TitleStringPosition.y, m_strTitle, m_dwColor );

				char szNameLevel[128] = {0,};

				PT_ITEM_SPEC pProp	= m_pPetElem->GetProp();
				CString strName		= pProp->szName;
				if( m_pPetElem->m_pPet->GetLevel() > PL_EGG )
				{
					MoverProp* pMoverProp	= prj.GetMoverProp( m_pPetElem->m_pPet->GetIndex() );
					if( pMoverProp )
						strName		= pMoverProp->szName;
				}
//sun: ?, __PET_1024
				if( m_pPetElem->m_pPet->HasName() )
					strName		= m_pPetElem->m_pPet->GetName();

				FLSPrintf( szNameLevel, _countof( szNameLevel ), "%s", strName );
				SetTitle( szNameLevel );
				
				p2DRender->SetFont( pOldFont );
			}				
		}
		else
		{
			m_pTheme->RenderWndBaseFrame( p2DRender, &rect );
			if( IsWndStyle( WBS_CAPTION ) )
			{
				// 타이틀 바 
				rect.bottom = 21;
				if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
					m_pTheme->RenderWndBaseTitleBar( p2DRender, &rect, 17, 7, m_strTitle, m_dwColor );
				if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
					m_pTheme->RenderWndBaseTitleBar( p2DRender, &rect, TitleStringPosition.x, TitleStringPosition.y, m_strTitle, m_dwColor );
			}
		}
	}
}

void CWndPetStatus::DrawPetInformation(C2DRender* p2DRender)
{
	p2DRender->TextOut( 74, 4,	prj.GetText(TID_GAME_PET_STATUS_LEVEL),		m_dwPropertyNameColor, m_dwPropertyNameShadowColor );
	p2DRender->TextOut( 74, 18, prj.GetText(TID_GAME_PET_STATUS_ABILITY),	m_dwPropertyNameColor, m_dwPropertyNameShadowColor );
	p2DRender->TextOut( 74, 46, prj.GetText(TID_GAME_PET_STATUS_HP),		m_dwPropertyNameColor, m_dwPropertyNameShadowColor );
	p2DRender->TextOut( 74, 61, prj.GetText(TID_GAME_PET_STATUS_EXP),		m_dwPropertyNameColor, m_dwPropertyNameShadowColor );

	DWORD dwColor = m_dwPropertyValueColor;
	
	if(m_pPetElem->m_pPet)
	{
		int nLevel = m_pPetElem->m_pPet->GetLevel();
		CString strTemp;
		DWORD dwLevelText;
		switch(nLevel) 
		{
			case PL_EGG:
				dwLevelText = TID_GAME_PETGRADE_E;
				break;
			case PL_D:
				dwLevelText = TID_GAME_PETGRADE_D;
				break;
			case PL_C:
				dwLevelText = TID_GAME_PETGRADE_C;
				break;
			case PL_B:
				dwLevelText = TID_GAME_PETGRADE_B;
				break;
			case PL_A:
				dwLevelText = TID_GAME_PETGRADE_A;
				break;
			case PL_S:
				dwLevelText = TID_GAME_PETGRADE_S;
				break;
		}
		strTemp.Format( "%s", prj.GetText(dwLevelText) );

		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
			p2DRender->TextOut( 112, 4, strTemp, dwColor);
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
			p2DRender->TextOut( 110, 4, strTemp, dwColor);
		
		DWORD dwDstParam;
		int nParam;
		DWORD dwTooltip;
		m_pPetElem->m_pPet->GetAvailDestParam(dwDstParam, nParam);
		
		switch(dwDstParam) 
		{
			case DST_STR:
				dwTooltip = TID_TOOLTIP_STR;
				break;
			case DST_DEX:
				dwTooltip = TID_TOOLTIP_DEX;
				break;
			case DST_INT:
				dwTooltip = TID_TOOLTIP_INT;
				break;
			case DST_STA:
				dwTooltip = TID_TOOLTIP_STA;
				break;
			case DST_ATKPOWER:
				dwTooltip = TID_TOOLTIP_ATKPOWER_VALUE;
				break;
			case DST_ADJDEF:
				dwTooltip = TID_TOOLTIP_DEFENCE;
				break;
			case DST_HP_MAX:
				dwTooltip = TID_TOOLTIP_HP;
				break;
		}
		
		CSize size = g_Neuz.m_2DRender.m_pFont->GetTextExtent( prj.GetText(TID_GAME_PET_STATUS_ABILITY) );

		if(dwDstParam != 0 && nParam != 0)
			strTemp.Format("%s +%d", prj.GetText(dwTooltip), nParam);
		else
			strTemp.Format("%s", prj.GetText(TID_GAME_PET_EGG_ABILITY));

		p2DRender->TextOut( 76 + size.cx, 18, strTemp, dwColor); //'Ability'자간이 넓어 겹치는 국가 발생하여 위치 조정
		
		int nLife = m_pPetElem->m_pPet->GetLife();
		strTemp.Format("%d", nLife);

		if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
			p2DRender->TextOut( 100, 32, strTemp, dwColor);
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
			p2DRender->TextOut( 110, 32, strTemp, dwColor);
		
		if(m_bExtension)
		{
			//Draw Level Background Image
			CPoint point;
			
			point = CPoint( GetWndCtrl( WIDC_CUSTOM4 )->rect.left-2, GetWndCtrl( WIDC_CUSTOM4 )->rect.top-2 );
			
			if(m_pTexPetLvBg != NULL)
				m_pTexPetLvBg->Render( p2DRender,  point );

			//Draw Level Image
			for(int i=PL_D; i<=nLevel; i++)
			{
				BYTE bLevel = m_pPetElem->m_pPet->GetAvailLevel(i);
				if(bLevel != 0)
				{
					if(m_aPetLv[i] != bLevel)
					{
						CString strPath;
						if(m_nLockLevel[0] == i)
							strPath = m_strPathLvImage[m_nBackUpLevel[0]-1];
						else if(m_nLockLevel[1] == i)
							strPath = m_strPathLvImage[m_nBackUpLevel[1]-1];
						else
							strPath = m_strPathLvImage[bLevel-1];
						
						m_pTexPetLv[i] = CWndBase::m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, strPath, 0xffff00ff );
						m_aPetLv[i] = bLevel;
					}
					if(m_pTexPetLv[i] != NULL)
					{
						if(m_nChoiceLevel == i)
							m_pTexPetLv[i]->Render( p2DRender,  point );
						else
							m_pTexPetLv[i]->Render( p2DRender,  point, 120 );
					}

					point.x += 35;					
				}
			}
		}
	}
}


//---------------------------------------------------------------------------------------------
// Draw Gauge ( 게이지 그리기 )
// param	: ...
// return	: void 
//---------------------------------------------------------------------------------------------
void CWndPetStatus::DrawGauge( C2DRender* p2DRender )
{
	if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
	{
		MakeGaugeVertex();

		if( m_bVBHPGauge )
		{
			int nMax = m_pPetElem->m_pPet->GetMaxEnergy();
			int nHitPercent = MulDiv( m_pPetElem->m_pPet->GetEnergy(), 100, nMax );

			m_bHPVisible = TRUE;
			if( m_bHPVisible )
				m_pTheme->RenderGauge( p2DRender->m_pd3dDevice, m_pVBHPGauge, &m_texGaugeHP );
		}
		if( m_bVBEXPGauge )
			m_pTheme->RenderGauge( p2DRender->m_pd3dDevice, m_pVBEXPGauge, &m_texGaugeExp );
	}

	if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
	{
		FLERROR_ASSERT_LOG_RETURN_VOID( m_pPetElem, _T("[윈도우 펫 상태] 펫 아이템 포인터가 유효하지 않습니다.") );
		FLERROR_ASSERT_LOG_RETURN_VOID( m_pPetElem->m_pPet, _T("[윈도우 펫 상태] 펫 포인터가 유효하지 않습니다.") );
		
		LPWNDCTRL	lpHP( GetWndCtrl( WIDC_CUSTOM2 ) );
		FLERROR_ASSERT_LOG_RETURN_VOID( lpHP, _T("[윈도우 펫 상태] 윈도우 컨트롤의 포인터가 유효하지 않습니다.") );

		CRect		rtRectHPGauge( lpHP->rect );
		int			nWidthHPClient( lpHP->rect.Width() );
		float		fPercentHP( static_cast<float>(m_pPetElem->m_pPet->GetEnergy()) / static_cast<float>(m_pPetElem->m_pPet->GetMaxEnergy()) );
		int			nOffsetHPRight( static_cast<int>( nWidthHPClient * fPercentHP ) );
		
		rtRectHPGauge.right = rtRectHPGauge.left + nOffsetHPRight;
		
		m_pTheme->RenderGauge( p2DRender, &lpHP->rect, WNDCOLOR_WHITE, m_pVBHPGauge, m_pGaugeBg );
		m_pTheme->RenderGauge( p2DRender, &rtRectHPGauge, WNDCOLOR_WHITE, m_pVBHPGauge, m_pGaugeHP );

		LPWNDCTRL	lpExp( GetWndCtrl( WIDC_CUSTOM3 ) );
		FLERROR_ASSERT_LOG_RETURN_VOID( lpExp, _T("[윈도우 펫 상태] 윈도우 컨트롤의 포인터가 유효하지 않습니다.") );

		CRect		rtRectExpGauge( lpExp->rect );
		int			nWidthExpClient( lpExp->rect.Width() );
		float		fPercentExp( static_cast<float>( nWidthExpClient ) /  static_cast<float>( m_pPetElem->m_pPet->GetMaxExp() ) );
		int			nOffsetExpRight( static_cast<int>( fPercentExp * m_pPetElem->m_pPet->GetExp() ) );

		rtRectExpGauge.right = rtRectExpGauge.left + nOffsetExpRight;
		m_pTheme->RenderGauge( p2DRender, &lpExp->rect, WNDCOLOR_WHITE, m_pVBEXPGauge, m_pGaugeBg );
		m_pTheme->RenderGauge( p2DRender, &rtRectExpGauge, WNDCOLOR_WHITE, m_pVBEXPGauge, m_pGaugeExp );
	}
}



void CWndPetStatus::OnDraw( C2DRender* p2DRender )
{
	if( g_pPlayer == NULL || m_pPetElem == NULL || m_pPetElem->m_pPet == NULL )
		return;

	//Draw Status Background Image
	CPoint point;
				
	point = CPoint( GetWndCtrl( WIDC_CUSTOM1 )->rect.left-12, GetWndCtrl( WIDC_CUSTOM1 )->rect.top-6 );
	
	if(m_pTexPetStatusBg != NULL)
		m_pTexPetStatusBg->Render( p2DRender,  point );
	
	CRect rect = GetClientRect();
	int nWidthClient = GetClientRect().Width() - 110;

	CRect rectTemp( 0, 0, 0, 0 );

	DrawPetInformation( p2DRender );
	DrawGauge( p2DRender );
	
	LPWNDCTRL lpFace = GetWndCtrl( WIDC_CUSTOM1 );
	LPWNDCTRL lpHP   = GetWndCtrl( WIDC_CUSTOM2 );
	LPWNDCTRL lpExp  = GetWndCtrl( WIDC_CUSTOM3 );

	if( m_nDisplay != 0 )
	{
		DWORD dwColor = m_dwGaugeValueColor;

		char cbufHp[16] = {0,};
		char cbufExp[16] = {0,};
		
		int nCharHP, nCharEXP;

		if(m_nDisplay == 1)
		{
			int nMax = m_pPetElem->m_pPet->GetMaxEnergy();
			int nHitPercent = MulDiv( m_pPetElem->m_pPet->GetEnergy(), 100, nMax );

			FLSPrintf(cbufHp, _countof( cbufHp ), "%d%%", nHitPercent);
			p2DRender->TextOut( lpHP->rect.right - 70, lpHP->rect.top - 0, cbufHp, dwColor, 0xff000000 );

			nCharHP = FLSPrintf(cbufHp, _countof( cbufHp ), "%dp", m_pPetElem->m_pPet->GetEnergy());

			int x = lpHP->rect.right-7;
			p2DRender->TextOut( x - (int)(nCharHP*5.8f) , lpHP->rect.top - 0, cbufHp, dwColor, 0xff000000 );
		}
		else
		{
			nCharHP = FLSPrintf(cbufHp, _countof( cbufHp ), "%d", m_pPetElem->m_pPet->GetEnergy());

			int x = lpHP->rect.right - 76 + 15;
			p2DRender->TextOut( x - (int)(nCharHP*2.6f), lpHP->rect.top - 0, cbufHp, dwColor, 0xff000000 );
			
			nCharHP = FLSPrintf(cbufHp, _countof( cbufHp ), "%d", m_pPetElem->m_pPet->GetMaxEnergy());
			
			x = lpHP->rect.right - 38 + 15;
			p2DRender->TextOut( x - nCharHP*2, lpHP->rect.top - 0, cbufHp, dwColor, 0xff000000 );
			
			p2DRender->TextOut( lpHP->rect.right - 42, lpHP->rect.top - 0, "/", dwColor, 0xff000000 );
		}

		EXPINTEGER	nExpResult = m_pPetElem->m_pPet->GetExp() * (EXPINTEGER)10000 / m_pPetElem->m_pPet->GetMaxExp();
		float fExp = (float)nExpResult / 100.0f;

		if( fExp >= 99.99f )
			nCharEXP = FLSPrintf( cbufExp, _countof( cbufExp ), "99.99%%" );		// sprintf함수 내부에서 반올림되어 100.00으로 표시되는 것을 막기 위해서 
		else
			nCharEXP = FLSPrintf( cbufExp, _countof( cbufExp ), "%.2f%%", fExp );

		int x = lpHP->rect.right-7; // -40
		p2DRender->TextOut( x - (int)(nCharEXP*5.8f), lpExp->rect.top - 0, cbufExp, dwColor, 0xff000000 );
	}

	LPDIRECT3DDEVICE9 pd3dDevice = p2DRender->m_pd3dDevice;
	pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0xffa08080, 1.0f, 0 ) ;

	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );		
	pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );		

	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );

	// 뷰포트 세팅 
	D3DVIEWPORT9 viewport;

	viewport.X      = p2DRender->m_ptOrigin.x + lpFace->rect.left;
	viewport.Y      = p2DRender->m_ptOrigin.y + lpFace->rect.top;
	viewport.Width  = lpFace->rect.Width();
	viewport.Height = lpFace->rect.Height();
	viewport.MinZ   = 0.0f;
	viewport.MaxZ   = 1.0f;

	pd3dDevice->SetViewport(&viewport);

	// 프로젝션 
	D3DXMATRIX matProj;
	D3DXMatrixIdentity( &matProj );
	FLOAT fAspect = ((FLOAT)viewport.Width) / (FLOAT)viewport.Height;

	FLOAT fov = D3DX_PI/4.0f;//796.0f;
	FLOAT h = cos(fov/2) / sin(fov/2);
	FLOAT w = h * fAspect;
	D3DXMatrixOrthoLH( &matProj, w, h, CWorld::m_fNearPlane - 0.01f, CWorld::m_fFarPlane );
	pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	D3DXMATRIX  matView;

	// 월드 
	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 matScale;
	D3DXMATRIXA16 matRot1, matRot2;
	D3DXMATRIXA16 matTrans;

	// 초기화 
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matRot1);
	D3DXMatrixIdentity(&matRot2);
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matWorld);

	//펫 종류에 따라 설정.
	D3DXVECTOR3 vecPos;
	D3DXVECTOR3 vecLookAt;
	float fScale = 1.0f;
		
	if( m_pPetElem->m_pPet->GetLevel() == PL_EGG )
	{
		vecPos.x = 0.0f;
		vecPos.y = 5.0f;
		vecPos.z = -4.0f;

		vecLookAt.x = 0.0f;
		vecLookAt.y = 0.0f;
		vecLookAt.z = 3.0f;
		fScale = 5.0f;
	}
	else
	{
		int petkind = m_pPetElem->m_pPet->GetKind();
		vecPos = m_PetCameTable[petkind].CamPos;
		vecLookAt = m_PetCameTable[petkind].CamLook;
		fScale = m_PetCameTable[petkind].Scale;
	}

	D3DXMatrixScaling(&matScale, fScale, fScale, fScale);
	
	D3DXMatrixLookAtLH( &matView, &vecPos, &vecLookAt, &D3DXVECTOR3(0.0f,1.0f,0.0f) );
	pd3dDevice->SetTransform( D3DTS_VIEW, &matView );
	
	D3DXMatrixMultiply(&matWorld,&matWorld,&matScale);
	D3DXMatrixMultiply(&matWorld, &matWorld,&matRot1);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matTrans );
	pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	// 랜더링 
	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	
	pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, CWorld::m_dwBgColor, 1.0f, 0 ) ;


	//CModelObject* pPetModel	=  g_pPlayer->GetPetModel();
	//if( pPetModel )
	if(m_pPetModel != NULL)
	{
		// Pet LOD가 들어갔기 때문에 LodGroup setting.
		if(g_pPlayer && g_pPlayer->m_pet.GetObj() )
		{
			float	fDist = 50.0f;
			switch( g_Option.m_nObjectDetail )
			{
				case 0 :	fDist = 20.0f;	break;
				case 1 :	fDist = 10.0f;	break;
				case 2 :	fDist = 5.0f;	break;
			}
			int nLevel = (int)( g_pPlayer->m_pet.GetObj()->m_fDistCamera / fDist );
			if( nLevel >= 2 )	nLevel = 2;
			if( nLevel < 0 )	
			{
				//FLERROR_LOG( PROGRAM_NAME, _T( "%s, lod lv=%d %f" ), m_szName, nLevel, m_fDistCamera );
				nLevel = 0;
			}
			m_pPetModel->SetGroup( nLevel );
		}
		
		::SetTransformView( matView );
		::SetTransformProj( matProj );

		int nMax = m_pPetElem->m_pPet->GetMaxEnergy();
		int nHitPercent = MulDiv( m_pPetElem->m_pPet->GetEnergy(), 100, nMax );
		{	
			SetDiffuse( 0.0f, 0.0f, 0.0f );
			SetAmbient( 1.0f, 1.0f, 1.0f );
		}			

		m_pPetModel->SetTextureEx( m_pPetModel->m_pModelElem->m_nTextureEx );
		m_pPetModel->Render(pd3dDevice, &matWorld);

		SetDiffuse( 0.0f, 0.0f, 0.0f );
		SetAmbient( 1.0f, 1.0f, 1.0f );
	}

	
	pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );
}

HRESULT CWndPetStatus::RestoreDeviceObjects()
{
	CWndBase::RestoreDeviceObjects();
	if( m_pVBHPGauge == NULL )
	{
		m_pApp->m_pd3dDevice->CreateVertexBuffer( sizeof( TEXTUREVERTEX2 ) * 3 * 6, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_TEXTUREVERTEX2, D3DPOOL_DEFAULT, &m_pVBHPGauge, NULL );
		m_pApp->m_pd3dDevice->CreateVertexBuffer( sizeof( TEXTUREVERTEX2 ) * 3 * 6, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_TEXTUREVERTEX2, D3DPOOL_DEFAULT, &m_pVBEXPGauge, NULL );

		m_nHPWidth = -1;
		m_nEXPWidth = -1;
	}
	return S_OK;
}

HRESULT CWndPetStatus::InvalidateDeviceObjects()
{
	CWndBase::InvalidateDeviceObjects();
    SAFE_RELEASE( m_pVBHPGauge );
    SAFE_RELEASE( m_pVBEXPGauge );
	SAFE_DELETE( m_pPetModel );
	return S_OK;
}

HRESULT CWndPetStatus::DeleteDeviceObjects()
{
	CWndBase::DeleteDeviceObjects();
	return InvalidateDeviceObjects();
}


//---------------------------------------------------------------------------------------------
// 윈도우의 생성 될 위치를 계산한다.
// param	: void
// return	: void 
//---------------------------------------------------------------------------------------------
void CWndPetStatus::CalculationWindowPosition()
{
	if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
	{
		//Position Control
		CWndStatus* pWndStatus = (CWndStatus*)GetWndBase( APP_STATUS1 );

		//gmpbigsun( 20110322 ) --BEGIN
		//#0012915 [전국가_정섭_16차,17차] 버프펫과 리어펫을 동시 소환 할 경우 UI가 겹쳐지는 현상
		CWndBase* pWndBuffpet = GetWndBase( APP_BUFFPET_STATUS );
		if( NULL != pWndBuffpet )
		{
			// Correct the y position if found same wnd
			CRect rectBrother = pWndBuffpet->GetWndRect( );
			int x = rectBrother.left;
			int y = rectBrother.top + 20;

			Move( CPoint( x, y ) );
		}
		else
		// --END
		if(pWndStatus != NULL)
		{
			CRect rectRoot = pWndStatus->GetWndRect();
			CRect rect = GetWindowRect();
			int x = rectRoot.right;
			int y = rectRoot.top;	

			Move( CPoint(x, y));
		}
		else
		{
			CRect rectRoot = m_pWndRoot->GetLayoutRect();
			CPoint point( rectRoot.left+192, rectRoot.top );
			Move( point );
		}
	}

	if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
	{
		CWndBase* pWndBuffPet( GetWndBase( APP_BUFFPET_STATUS ) );
		if( pWndBuffPet )
		{
			CRect rtBuffPet( pWndBuffPet->GetWndRect() );
			Move( CPoint( rtBuffPet.right, rtBuffPet.top ) );
		}
	}
}


//---------------------------------------------------------------------------------------------
// 업데이트 전 초기화
// param	: void
// return	: void 
//---------------------------------------------------------------------------------------------
void CWndPetStatus::OnInitialUpdate()
{
	CWndNeuz::OnInitialUpdate();
	
	RestoreDeviceObjects();
	
	if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
	{
		m_texGaugeHP.LoadTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME,g_xFlyffConfig->GetMainLanguage(), _T("GauEmptyNormal.bmp") ), WNDCOLOR_DEFAULT_KEY, TRUE );
		m_texGaugeExp.LoadTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), _T("GauEmptyNormal.bmp") ), WNDCOLOR_DEFAULT_KEY, TRUE );
	}

	if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
	{
		m_pGaugeHP	= m_textureMng.AddTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME,g_xFlyffConfig->GetMainLanguage(), _T("BarRed.tga") ), WNDCOLOR_DEFAULT_KEY, TRUE );
		m_pGaugeExp	= m_textureMng.AddTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME,g_xFlyffConfig->GetMainLanguage(), _T("BarSky.tga") ), WNDCOLOR_DEFAULT_KEY, TRUE );
		m_pGaugeBg	= m_textureMng.AddTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME,g_xFlyffConfig->GetMainLanguage(), _T("BarBg.tga") ), WNDCOLOR_DEFAULT_KEY, TRUE );
	}
	
	m_pTexPetLvBg		= CWndBase::m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(),"PetLevelBg.tga"), WNDCOLOR_DEFAULT_KEY );
	m_pTexPetStatusBg	= CWndBase::m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(),"PetStatusBg.tga"), WNDCOLOR_DEFAULT_KEY, TRUE );

	// 장착, 게이지에 나올 캐릭터 오브젝트 설정
	if( g_pPlayer->HasActivatedSystemPet() )
		m_pPetElem = g_pPlayer->GetPetItem();

	//Size Control
	SetExtension(m_bExtension);

	// 윈도우 위치 계산
	CalculationWindowPosition();
}

void CWndPetStatus::OnMouseWndSurface(CPoint point)
{
	if(!m_bExtension || m_pPetElem->m_pPet == NULL)
		return;
	
	CRect baseRect = GetWndCtrl(WIDC_CUSTOM4)->rect;
	CRect testRect;
	BOOL bCheckChoice = FALSE;

	testRect = baseRect;
	testRect.right = testRect.left + 34;
	
	int nLevel = m_pPetElem->m_pPet->GetLevel();

	for(int i=PL_D; i<nLevel+1; i++)
	{
		if( testRect.PtInRect(point) )
		{
			CPoint point2 = point;
			ClientToScreen( &point2 );
			ClientToScreen( &testRect );

			DWORD dwDstParam;
			int nParam;
			DWORD dwTooltip;
			CString strTemp;
			CEditString strEdit;
			
			PPETAVAILPARAM pAvailParam = CPetProperty::GetInstance()->GetAvailParam( m_pPetElem->m_pPet->GetKind() );
			dwDstParam	= pAvailParam->dwDstParam;
			nParam = pAvailParam->m_anParam[m_pPetElem->m_pPet->GetAvailLevel(i) - 1];
			
			switch(dwDstParam) 
			{
				case DST_STR:
					dwTooltip = TID_TOOLTIP_STR;
					break;
				case DST_DEX:
					dwTooltip = TID_TOOLTIP_DEX;
					break;
				case DST_INT:
					dwTooltip = TID_TOOLTIP_INT;
					break;
				case DST_STA:
					dwTooltip = TID_TOOLTIP_STA;
					break;
				case DST_ATKPOWER:
					dwTooltip = TID_TOOLTIP_ATKPOWER_VALUE;
					break;
				case DST_ADJDEF:
					dwTooltip = TID_TOOLTIP_DEFENCE;
					break;
				case DST_HP_MAX:
					dwTooltip = TID_TOOLTIP_DST_HP_MAX;
					break;
			}
			strTemp.Format( "%s +%d", prj.GetText(dwTooltip), nParam );
			strEdit.AddString( strTemp, D3DCOLOR_XRGB(255, 0, 0) );

			g_toolTip.PutToolTip(m_pPetElem->m_dwItemId, strEdit, &testRect, point2, 0 );
			bCheckChoice = TRUE;
			m_nChoiceLevel = i;
		}
		testRect.left = testRect.right;
		testRect.right = testRect.left + 34;
	}

	if(!bCheckChoice)
		m_nChoiceLevel = -1;
}

void CWndPetStatus::SetExtension(BOOL eflag)
{
	CRect rect = GetWindowRect(TRUE);

	if(eflag)
		rect.bottom += 48;
	else
		rect.bottom -= 48;
	
	SetWndRect(rect);
}

BOOL CWndPetStatus::Initialize(CWndBase* pWndParent,DWORD dwWndId)
{
	return InitDialog( g_Neuz.GetSafeHwnd(), APP_PET_STATUS );
}

BOOL CWndPetStatus::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase )
{
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase );
}

void CWndPetStatus::OnSize(UINT nType, int cx, int cy)
{
	CWndNeuz::OnSize( nType, cx, cy );
}

void CWndPetStatus::SetWndRect( CRect rectWnd, BOOL bOnSize  )
{
	m_nHPWidth = -1;
	m_nEXPWidth = -1;
	
	CWndNeuz::SetWndRect( rectWnd, bOnSize );
}

void CWndPetStatus::OnLButtonUp(UINT nFlags, CPoint point)
{
}

void CWndPetStatus::OnLButtonDown(UINT nFlags, CPoint point)
{	
	if(++m_nDisplay > 2)
		m_nDisplay = 0;
}

BOOL CWndPetStatus::OnEraseBkgnd(C2DRender* p2DRender)
{
	return CWndBase::OnEraseBkgnd( p2DRender );
	CRect rect = GetClientRect();
	p2DRender->m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
	p2DRender->RenderFillRect( rect, D3DCOLOR_ARGB( 100, 0, 0, 0 ) );

	return TRUE;
}

BOOL CWndPetStatus::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_BUTTON1 )
	{
		m_bExtension = !m_bExtension;
		SetExtension(m_bExtension);
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
}

BOOL CWndPetStatus::Process()
{
	if(!IsValidObj(g_pPlayer))
		return FALSE;
	
	// 장착, 게이지에 나올 캐릭터 오브젝트 설정
	if( g_pPlayer->HasActivatedSystemPet() )
	{
		m_pPetElem = g_pPlayer->GetPetItem();

		if( m_pPetElem != NULL )
		{
			if( g_pPlayer->GetPetModel() == NULL && m_pPetModel != NULL )
			{
				SAFE_DELETE(m_pPetModel);
			}
			else if( m_pPetElem->m_pPet != NULL && g_pPlayer->GetPetModel() != NULL && m_pPetModel == NULL )
			{
				m_pPetModel = new CModelObject;
				m_pPetModel->InitDeviceObjects( g_Neuz.m_pd3dDevice );
				
				CString textFile;
				textFile.Format("Mvr_%s.chr", g_pPlayer->GetPetModel()->m_pModelElem->getName());
				m_pPetModel->LoadBone( textFile );
				textFile.Format("Mvr_%s.o3d", g_pPlayer->GetPetModel()->m_pModelElem->getName());
				m_pPetModel->LoadElement( textFile, 0 );

				m_pPetModel->m_pModelElem = g_pPlayer->GetPetModel()->m_pModelElem;

				if( m_pPetElem->m_pPet->GetLevel() == PL_EGG )
					textFile.Format("Mvr_%s_idle.ani", g_pPlayer->GetPetModel()->m_pModelElem->getName());
				else
					textFile.Format("Mvr_%s_stand.ani", g_pPlayer->GetPetModel()->m_pModelElem->getName());

				m_pPetModel->LoadMotion( textFile );
			}
		}
	}
	
	if(m_pPetModel != NULL)
		m_pPetModel->FrameMove();

	return TRUE;
}

void CWndPetStatus::LockShowLevel(BOOL lFlag, int nLevel, int nPos)
{
	if(lFlag)
	{
		m_nLockLevel[nPos] = nLevel;
		if(m_pPetElem->m_pPet)
			m_nBackUpLevel[nPos] = m_pPetElem->m_pPet->GetAvailLevel(nLevel);
	}
	else
	{
		m_nLockLevel[nPos] = -1;
		m_nBackUpLevel[nPos] = -1;

		CString strPath;
		BYTE bLevel = m_pPetElem->m_pPet->GetAvailLevel(nLevel);
		if(bLevel != 0)
		{
			strPath = m_strPathLvImage[bLevel-1];
			m_pTexPetLv[nLevel] = CWndBase::m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, strPath, 0xffff00ff );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//	CWndFoodConfirm Class
//////////////////////////////////////////////////////////////////////////

CWndFoodConfirm::CWndFoodConfirm()
{ 
	m_pItemElem = NULL;
	m_pEdit = NULL;
	m_nParent = 0;
}

CWndFoodConfirm::CWndFoodConfirm(int nParent)
{ 
	m_pItemElem = NULL;
	m_pEdit = NULL;
	m_nParent = nParent;
} 

CWndFoodConfirm::~CWndFoodConfirm() 
{ 
} 

void CWndFoodConfirm::SetItem(DWORD dwObjId)
{
	m_pItemElem = (FLItemElem*)g_pPlayer->GetItemId( dwObjId );
}

void CWndFoodConfirm::OnDraw( C2DRender* p2DRender ) 
{ 
	LPCTSTR szNumber;
	szNumber = m_pEdit->GetString();
	int nNumber = atoi( szNumber );

	if(m_pItemElem == NULL)
		return;

	if( m_pItemElem->m_nItemNum == 1 )
	{
		m_pEdit->SetString( "1" );
	}
	else
	{
		if( nNumber > 1000 )	// 0723
		{
			char szNumberbuf[16] = {0, };
			nNumber = 1000;
			FLIntToString( 1000, szNumberbuf, _countof( szNumberbuf ), 10 );
			m_pEdit->SetString( szNumberbuf );
		}	
		else if( m_pItemElem->m_nItemNum < nNumber )
		{
			char szNumberbuf[16] = {0, };
			nNumber = m_pItemElem->m_nItemNum;
			FLIntToString( m_pItemElem->m_nItemNum, szNumberbuf, _countof( szNumberbuf ), 10 );
			m_pEdit->SetString( szNumberbuf );
		}
		
		for( int i = 0 ; i < 8 ; i++ )
		{
			char szNumberbuf[8] = {0, };
			FLStrncpy( szNumberbuf, _countof( szNumberbuf ), szNumber, 8 );
			
			// 0 : 공백, 48 : 숫자 0, 57 : 숫자 9
			if( 47 >= szNumberbuf[i] || szNumberbuf[i] >= 58 )
			{
				if( szNumberbuf[i] != 0 )
				{
					nNumber = m_pItemElem->m_nItemNum;
					FLIntToString( m_pItemElem->m_nItemNum, szNumberbuf, _countof( szNumberbuf ), 10 );
					m_pEdit->SetString( szNumberbuf );
					break;										
				}
			}
		}
	}
} 
void CWndFoodConfirm::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// 여기에 코딩하세요
	m_pEdit   = (CWndEdit  *)GetDlgItem( WIDC_EDIT1 );
	CWndButton* pWndOk = (CWndButton *)GetDlgItem( WIDC_OK );	
	pWndOk->SetDefault( TRUE );
	m_pEdit->SetFocus();

	if(m_pItemElem == NULL)
	{
		Destroy();
		return;
	}

	if( m_pItemElem->m_nItemNum == 1 )
	{
		m_pEdit->SetString( "1" );
	}
	else
	{
		TCHAR szNumber[ 64 ];
		if( m_pItemElem->m_nItemNum > 1000 )	// 0723
			FLIntToString( 1000, szNumber, _countof( szNumber ), 10 );
		else
			FLIntToString( m_pItemElem->m_nItemNum, szNumber, _countof( szNumber ), 10 );
		m_pEdit->SetString( szNumber );
	}

	CWndStatic* pStatic = (CWndStatic*)GetDlgItem(WIDC_STATIC1);
	if(m_nParent == 1)
		pStatic->SetTitle(prj.GetText(TID_GAME_PETFOOD_COUNTCONFIRM));
	else if(m_nParent == 2)
		pStatic->SetTitle(prj.GetText(TID_GAME_ITEMCOUNT_QUESTION));

	// 윈도를 중앙으로 옮기는 부분.
	MoveParentCenter();
} 
// 처음 이 함수를 부르면 윈도가 열린다.
BOOL CWndFoodConfirm::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy에서 설정한 리소스로 윈도를 연다.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_PET_ITEM, 0, CPoint( 0, 0 ), pWndParent );
} 

BOOL CWndFoodConfirm::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndFoodConfirm::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndFoodConfirm::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndFoodConfirm::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndFoodConfirm::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_OK || message == EN_RETURN )
	{
		int DropNum = 0;

		if(m_pItemElem != NULL)
		{
			if( m_pItemElem->m_nItemNum >= 1 )
			{
				DropNum = atoi( m_pEdit->GetString() );
			}
			if( DropNum != 0 && DropNum <= 1000 )	// 0723
			{
				if(m_nParent == 1) //Pet Food
				{
				}
				else if(m_nParent == 2) //Food Mill
				{
					CWndPetFoodMill* pWndPetFoodMill = (CWndPetFoodMill*)g_WndMng.GetWndBase(APP_PET_FOODMILL);
					if(pWndPetFoodMill != NULL)
						pWndPetFoodMill->SetItemForFeed(m_pItemElem, DropNum);
				}
			}
		}

		Destroy();
	}
	else if( nID == WIDC_CANCEL )
	{
		Destroy();
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
}

void CWndFoodConfirm::PaintFrame( C2DRender* p2DRender )
{
	CRect rect = GetWindowRect();
	CPoint	TitleStringPosition( CalcTitleStringPosition() );

	if( m_pTexture )
	{
		RenderWnd();
		// 여기는 타이틀 바의 텍스트를 출력하는 곳 
		if( IsWndStyle( WBS_CAPTION ) )	
		{
			int y = 4;
			CD3DFont* pOldFont = p2DRender->GetFont();
			p2DRender->SetFont( CWndBase::m_Theme.m_pFontWndTitle );
			p2DRender->TextOut( 10, y, m_strTitle, m_dwColor );
			
			char szNameLevel[128] = {0,};
			if(m_nParent == 1)
				FLSPrintf( szNameLevel, _countof( szNameLevel ), "%s", prj.GetText(TID_GAME_PETFOOD_CONFIRM) );
			else if(m_nParent == 2)
				FLSPrintf( szNameLevel, _countof( szNameLevel ), "%s", prj.GetText(TID_GAME_ITEMCOUNT_CONFIRM) );

			SetTitle( szNameLevel );
			
			p2DRender->SetFont( pOldFont );
		}				
	}
	else
	{
		m_pTheme->RenderWndBaseFrame( p2DRender, &rect );
		if( IsWndStyle( WBS_CAPTION ) )
		{
			// 타이틀 바 
			rect.bottom = 21;
			if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
				m_pTheme->RenderWndBaseTitleBar( p2DRender, &rect, 17, 7, m_strTitle, m_dwColor );
			if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
				m_pTheme->RenderWndBaseTitleBar( p2DRender, &rect, TitleStringPosition.x, TitleStringPosition.y, m_strTitle, m_dwColor );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//	CWndPetMiracle Class
//////////////////////////////////////////////////////////////////////////
CWndPetMiracle::CWndPetMiracle() 
{ 
	m_nCount[0] = 0;
	m_nDelay[0] = 1;
	m_nStatus[0] = -1;
	m_nCount[1] = 0;
	m_nDelay[1] = 1;
	m_nStatus[1] = -1;

	m_nPreLvCount = -1;
	m_nCurLvCount = -1;
	m_nResPreLevel = -1;
	m_nResCurLevel = -1;
	m_nPetLevel = -1;
	m_dwObjId = -1;

	m_bReciveResult[0] = FALSE;
	m_bReciveResult[1] = FALSE;

	m_bEnd = FALSE;
	m_pText = NULL;
	m_bLocked[0] = FALSE;
	m_bLocked[1] = FALSE;

	m_strPathLvImage[0] = MakePath( DIR_ICON, "Icon_PetLevel1_L.dds");
	m_strPathLvImage[1] = MakePath( DIR_ICON, "Icon_PetLevel2_L.dds");
	m_strPathLvImage[2] = MakePath( DIR_ICON, "Icon_PetLevel3_L.dds");
	m_strPathLvImage[3] = MakePath( DIR_ICON, "Icon_PetLevel4_L.dds");
	m_strPathLvImage[4] = MakePath( DIR_ICON, "Icon_PetLevel5_L.dds");
	m_strPathLvImage[5] = MakePath( DIR_ICON, "Icon_PetLevel6_L.dds");
	m_strPathLvImage[6] = MakePath( DIR_ICON, "Icon_PetLevel7_L.dds");
	m_strPathLvImage[7] = MakePath( DIR_ICON, "Icon_PetLevel8_L.dds");
	m_strPathLvImage[8] = MakePath( DIR_ICON, "Icon_PetLevel9_L.dds");
}
 
CWndPetMiracle::~CWndPetMiracle() 
{	
} 

void CWndPetMiracle::OnDestroy()
{
	CWndPetStatus* pWndStatus = (CWndPetStatus*)g_WndMng.GetWndBase( APP_PET_STATUS );
	if(pWndStatus != NULL && m_bLocked[0])
	{
		pWndStatus->LockShowLevel(FALSE, m_nPetLevel-1, 0);
		m_bLocked[0] = FALSE;
	}
	if(pWndStatus != NULL && m_bLocked[1])
	{
		pWndStatus->LockShowLevel(FALSE, m_nPetLevel, 1);
		m_bLocked[1] = FALSE;
	}	
}

void CWndPetMiracle::OnDraw( C2DRender* p2DRender ) 
{ 
	CTexture* pTexture;
	CString strPath;
	CEditString strEdit;
	
	//레벨 텍스트 그리기.
	strEdit.SetString(m_strPetLevel[0], D3DCOLOR_XRGB( 255, 255, 255 ), ESSTY_BOLD);
	p2DRender->TextOut_EditString(GetWndCtrl( WIDC_STATIC1 )->rect.left + 8, GetWndCtrl( WIDC_STATIC1 )->rect.top + 3, strEdit);
	strEdit.SetString(m_strPetLevel[1], D3DCOLOR_XRGB( 255, 255, 255 ), ESSTY_BOLD);
	p2DRender->TextOut_EditString(GetWndCtrl( WIDC_STATIC2 )->rect.left + 8, GetWndCtrl( WIDC_STATIC2 )->rect.top + 3, strEdit);

	//이전 레벨의 특성치 레벨 이미지 그리기.
	if(m_bReciveResult[0])
		strPath = m_strPathLvImage[m_nPreLvCount];
	else
		strPath = m_strPathLvImage[m_nMiracleLv[0] - 1];
	if(strPath.GetLength() > 0)
	{
		pTexture = CWndBase::m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, strPath, 0xffff00ff );
		if(pTexture != NULL)
			pTexture->Render( p2DRender, CPoint( GetWndCtrl( WIDC_STATIC3 )->rect.left, GetWndCtrl( WIDC_STATIC3 )->rect.top + 4 ) );	
	}
	//현재 레벨의 특성치 레벨 이미지 그리기.
	if(m_bReciveResult[1])
		strPath = m_strPathLvImage[m_nCurLvCount];
	else
		strPath = m_strPathLvImage[m_nMiracleLv[1] - 1];
	
	if(strPath.GetLength() > 0)
	{
		pTexture = CWndBase::m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, strPath, 0xffff00ff );
		if(pTexture != NULL)
			pTexture->Render( p2DRender, CPoint( GetWndCtrl( WIDC_STATIC4 )->rect.left, GetWndCtrl( WIDC_STATIC4 )->rect.top + 4 ) );
	}
}

void CWndPetMiracle::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	
	// 미국 버튼 이미지 변경
	CWndButton* pWndButton = (CWndButton*)GetDlgItem(WIDC_BUTTON2);
	if(pWndButton)
	{
		//if(g_xFlyffConfig->GetMainLanguage() == LANG_USA || g_xFlyffConfig->GetMainLanguage() == LANG_VTN)
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), "ButChance.bmp" ), 0xffff00ff );
	}

	//B/A/S 급 펫만 해당 기능을 이용할 수 있다.
	if( g_pPlayer->HasActivatedSystemPet() )
	{
		FLItemElem* m_pPetElem	= g_pPlayer->GetPetItem();
		m_nPetLevel = m_pPetElem->m_pPet->GetLevel();
		if(m_pPetElem->m_pPet != NULL && (m_nPetLevel == PL_B || m_nPetLevel == PL_A || m_nPetLevel == PL_S))
		{
			switch(m_nPetLevel) 
			{
				case PL_B:
					m_strPetLevel[0] = prj.GetText(TID_GAME_PET_GRADE_C); //"Lv. C"
					m_strPetLevel[1] = prj.GetText(TID_GAME_PET_GRADE_B); //"Lv. B"
					break;
				case PL_A:
					m_strPetLevel[0] = prj.GetText(TID_GAME_PET_GRADE_B); //"Lv. B"
					m_strPetLevel[1] = prj.GetText(TID_GAME_PET_GRADE_A); //"Lv. A"
					break;
				case PL_S:
					m_strPetLevel[0] = prj.GetText(TID_GAME_PET_GRADE_A); //"Lv. A"
					m_strPetLevel[1] = prj.GetText(TID_GAME_PET_GRADE_S); //"Lv. S"
					break;
			}

			m_nMiracleLv[0] = m_pPetElem->m_pPet->GetAvailLevel(m_nPetLevel-1);
			m_nMiracleLv[1] = m_pPetElem->m_pPet->GetAvailLevel(m_nPetLevel);
			m_nPreLvCount = m_nMiracleLv[0] - 1;
			m_nCurLvCount = m_nMiracleLv[1] - 1;

			m_pText = (CWndText *)GetDlgItem( WIDC_TEXT_DESC );
			SetDescription(NULL);

			CWndButton* pButton;
			pButton = (CWndButton*)GetDlgItem( WIDC_OK );
			pButton->EnableWindow(FALSE);
			pButton->SetVisible(FALSE);

			//만약 Status창이 활성화된 상태라면 레벨값이 변하지 않도록 고정을 요청하자.
			CWndPetStatus* pWndStatus = (CWndPetStatus*)g_WndMng.GetWndBase( APP_PET_STATUS );
			if(pWndStatus != NULL)
			{
				pWndStatus->LockShowLevel(TRUE, m_nPetLevel-1, 0);
				pWndStatus->LockShowLevel(TRUE, m_nPetLevel, 1);
				m_bLocked[0] = m_bLocked[1] = TRUE;
			}
		
			MoveParentCenter();
		}
		else
			Destroy();
	}
	else
		Destroy();
} 

BOOL CWndPetMiracle::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy에서 설정한 리소스로 윈도를 연다.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_PET_MIRACLE, 0, CPoint( 0, 0 ), pWndParent );
} 

BOOL CWndPetMiracle::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if(message == WNM_CLICKED)
	{
		if(nID == WIDC_BUTTON2) 
		{
			CWndButton* pButton;
			pButton = (CWndButton*)GetDlgItem( WIDC_BUTTON2 );
			pButton->EnableWindow(FALSE);

			//Send to Server
			if(m_dwObjId != -1)
				g_DPlay.SendPetTamerMiracle(m_dwObjId);
		}
		else if(nID == WIDC_OK || nID == WIDC_CLOSE)
			Destroy();
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
}

void CWndPetMiracle::ReceiveResult(int nPreLevel, int nCurLevel)
{
	m_nResPreLevel = nPreLevel;
	m_nResCurLevel = nCurLevel;
	m_nMiracleLv[0] = m_nResPreLevel;
	m_nMiracleLv[1] = m_nResCurLevel;
	
	m_nStatus[0] = 0;
	m_nStatus[1] = 0;
	m_bReciveResult[0] = TRUE;
	m_bReciveResult[1] = TRUE;
}

void CWndPetMiracle::SetItem(DWORD dwObjId)
{
	m_dwObjId = dwObjId;
}

BOOL CWndPetMiracle::Process()
{
	//Start버튼 누를 경우 컴퓨터의 선택이 회전하도록 함.
	PreLevelImgProcess();
	CurLevelImgProcess();
	return TRUE;
}

void CWndPetMiracle::PreLevelImgProcess()
{
	if(m_nStatus[0] == 0)
	{
		if(m_nCount[0]%4 == 0)
		{
			if(m_nCount[0] > 60)
			{
				m_nCount[0] = 0;
				m_nStatus[0] = 1;
			}
			else
			{
				PLAYSND( "InfOpen.wav" );
				int randnum = rand() % 9;
				if(randnum == m_nPreLvCount)
				{
					randnum++;
					( randnum > 8 ) ? randnum = 0 : randnum;
					
				}
				m_nPreLvCount = randnum;			
			}
		}
		m_nCount[0]++;
	}
	else if(m_nStatus[0] == 1)
	{
		if(m_nCount[0] > m_nDelay[0])
		{
			if(m_nDelay[0] < 10)
			{
				PLAYSND( "InfOpen.wav" );
				m_nDelay[0] += 1;
			}
			else
			{
				PLAYSND( "InfOpen.wav" );					
				//초기화 및 상태창이 열려있을 경우 상태창의 Level Lock을 푼다.
				m_nPreLvCount = m_nResPreLevel;
				m_nStatus[0] = -1;
				m_nDelay[0] = 1;
				m_nCount[0] = 0;
				m_bReciveResult[0] = FALSE;
				m_bEnd = TRUE;
				
				CWndPetStatus* pWndStatus = (CWndPetStatus*)g_WndMng.GetWndBase( APP_PET_STATUS );
				if(pWndStatus != NULL && m_bLocked[0])
				{
					pWndStatus->LockShowLevel(FALSE, m_nPetLevel-1, 0);
					m_bLocked[0] = FALSE;
				}
			}

			int randnum = rand() % 9;
			if(randnum == m_nPreLvCount)
			{
				randnum++;
				( randnum > 8 ) ? randnum = 0 : randnum;
			}
			m_nPreLvCount = randnum;
			m_nCount[0] = 0;
		}
		m_nCount[0]++;
	}
}

void CWndPetMiracle::CurLevelImgProcess()
{
	if(m_nStatus[1] == 0)
	{
		if(m_nCount[1]%4 == 0)
		{
			if(m_nCount[1] > 80)
			{
				m_nCount[1] = 0;
				m_nStatus[1] = 1;
			}
			else
			{
				PLAYSND( "InfOpen.wav" );
				int randnum = rand() % 9;
				if(randnum == m_nCurLvCount)
				{
					randnum++;
					( randnum > 8 ) ? randnum = 0 : randnum;
					
				}
				m_nCurLvCount = randnum;			
			}
		}
		m_nCount[1]++;
	}
	else if(m_nStatus[1] == 1)
	{
		if(m_nCount[1] > m_nDelay[1])
		{
			if(m_nDelay[1] < 20)
			{
				PLAYSND( "InfOpen.wav" );
				m_nDelay[1] += 1;
			}
			else
			{
				PLAYSND( "InfOpen.wav" );					
				//초기화 및 상태창이 열려있을 경우 상태창의 Level Lock을 푼다.
				m_nCurLvCount = m_nResCurLevel;
				m_nStatus[1] = -1;
				m_nDelay[1] = 1;
				m_nCount[1] = 0;
				m_bReciveResult[1] = FALSE;
				m_bEnd = TRUE;
				
				CWndPetStatus* pWndStatus = (CWndPetStatus*)g_WndMng.GetWndBase( APP_PET_STATUS );
				if(pWndStatus != NULL && m_bLocked[1])
				{
					pWndStatus->LockShowLevel(FALSE, m_nPetLevel, 1);
					m_bLocked[1] = FALSE;
				}
				CWndButton* pButton;
				pButton = (CWndButton*)GetDlgItem( WIDC_BUTTON2 );
				pButton->SetVisible(FALSE);

				pButton = (CWndButton*)GetDlgItem( WIDC_OK );
				pButton->SetVisible(TRUE);
				pButton->EnableWindow(TRUE);
			}
			
			int randnum = rand() % 9;
			if(randnum == m_nCurLvCount)
			{
				randnum++;
				( randnum > 8 ) ? randnum = 0 : randnum;
			}
			m_nCurLvCount = randnum;
			m_nCount[1] = 0;
		}
		m_nCount[1]++;
	}
}

void CWndPetMiracle::SetDescription( CHAR* szChar )
{
	CScript scanner;
	BOOL checkflag;
	checkflag = scanner.Load( MakePath( DIR_CLIENT,  _T( "PetMiracle.inc" ) ));
	szChar = scanner.m_pProg;

	if(m_pText != NULL && checkflag)
	{
		m_pText->m_string.AddParsingString( szChar );
		m_pText->ResetString();	
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CWndPetFoodMill Class
////////////////////////////////////////////////////////////////////////////////////////////////////////////
CWndPetFoodMill::CWndPetFoodMill()
{
	m_dwObjId = NULL_ID;
	m_pItemElem = NULL;
	m_pWndFoodConfrim = NULL;
	m_pTexture = NULL;
}

CWndPetFoodMill::~CWndPetFoodMill()
{
}

void CWndPetFoodMill::OnDestroy()
{
	if(m_pItemElem != NULL)
	{
		if( !g_pPlayer->m_vtInfo.IsTrading( m_pItemElem ) )
			m_pItemElem->SetExtra(0);
		m_pItemElem = NULL;
	}

	if(m_pWndFoodConfrim != NULL)
		m_pWndFoodConfrim->Destroy();
}

void CWndPetFoodMill::OnDraw(C2DRender* p2DRender)
{
	//Draw Food
	PT_ITEM_SPEC pItemProp;

	if(m_pItemElem != NULL)
	{
		pItemProp = m_pItemElem->GetProp();
		if(pItemProp != NULL)
		{
			CPoint point;
			point.x = GetWndCtrl( WIDC_STATIC2 )->rect.left;
			point.y = GetWndCtrl( WIDC_STATIC2 )->rect.top;
			
			if(m_pTexture != NULL)
				m_pTexture->Render( p2DRender, point );
			
			TCHAR szTemp[ 32 ];
			FLSPrintf( szTemp, _countof( szTemp ), prj.GetText(TID_GAME_KWAIBAWIBO_PRESENT_NUM), m_nItemCount );
			CSize size = m_p2DRender->m_pFont->GetTextExtent( szTemp );
			m_p2DRender->TextOut( point.x + 36 - size.cx, point.y + 48 - size.cy, szTemp, 0xff0000ff );
		}
	}	
}

void CWndPetFoodMill::OnInitialUpdate()
{
	CWndNeuz::OnInitialUpdate();
	
	CWndStatic* pStatic = (CWndStatic*)GetDlgItem(WIDC_STATIC1);
	pStatic->SetTitle(prj.GetText(TID_GAME_PETFOODMILL_DESC));

	CWndInventory* pWndInventory = (CWndInventory*)g_WndMng.CreateApplet(APP_INVENTORY);
	
	MoveParentCenter();
}

BOOL CWndPetFoodMill::Initialize(CWndBase* pWndParent,DWORD dwWndId)
{
	return InitDialog( g_Neuz.GetSafeHwnd(), APP_PET_FOODMILL );
}

BOOL CWndPetFoodMill::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if(nID == WIDC_OK)
	{
		if(m_pItemElem != NULL)
		{
			PT_ITEM_SPEC pItemProp = m_pItemElem->GetProp();
//			if( ( pItemProp->dwParts != NULL_ID || pItemProp->dwItemKind3 == IK3_GEM ) && m_pItemElem->IsCharged() == FALSE && pItemProp->dwCost > 0 )
//			if( pItemProp && (pItemProp->dwItemKind1 == IK1_WEAPON || pItemProp->dwReferStat1 == IK1_ARMOR ||
//				pItemProp->dwItemKind3 == IK3_GEM) && pItemProp->dwCost > 0)
			if( pItemProp->dwItemKind3 == IK3_GEM && m_pItemElem->IsCharged() == FALSE && pItemProp->dwCost > 0 )
			{
				//Send to Server...
				g_DPlay.SendMakePetFeed(m_pItemElem->m_dwObjId, m_nItemCount, m_dwObjId);
			}
			else
				g_WndMng.PutString( prj.GetText(TID_GAME_NOTFOOD), NULL, prj.GetTextColor(TID_GAME_NOTFOOD) );
		}
	}
	else if(nID == WIDC_CANCEL || nID == WTBID_CLOSE)
		Destroy();
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
}

void CWndPetFoodMill::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	CRect rect;
	LPWNDCTRL wndCtrl = GetWndCtrl( WIDC_STATIC2 );
	rect = wndCtrl->rect;
	if( m_pItemElem != NULL && rect.PtInRect( point ) )
	{
		m_pItemElem->SetExtra(0);
		m_pItemElem = NULL;
	}
}

BOOL CWndPetFoodMill::OnDropIcon( LPSHORTCUT pShortcut, CPoint point )
{
//sun: 11,
	CWndBase* pWndFrame = pShortcut->m_pFromWnd->GetFrameWnd();
	if( pWndFrame && pWndFrame->GetWndId() == APP_BAG_EX )
	{
		g_WndMng.OpenMessageBox( _T( prj.GetText(TID_GAME_ERROR_FOOD_MILL_POCKET) ) );
		return FALSE;
	}

	FLItemElem* pTempElem;
	pTempElem = (FLItemElem*)g_pPlayer->GetItemId( pShortcut->m_dwId );
	
	LPWNDCTRL wndCtrl = GetWndCtrl( WIDC_STATIC2 );
	if( wndCtrl->rect.PtInRect( point ) )
	{
		if(pTempElem != NULL)
		{
			if(m_pItemElem != NULL)
			{
				if( !g_pPlayer->m_vtInfo.IsTrading( m_pItemElem ) )
					m_pItemElem->SetExtra(0);
				m_pItemElem = NULL;
			}

			SAFE_DELETE( m_pWndFoodConfrim );
			
			m_pWndFoodConfrim = new CWndFoodConfirm(2);
			m_pWndFoodConfrim->m_pItemElem = pTempElem;
			m_pWndFoodConfrim->Initialize(NULL);
		}
	}

	return TRUE;
}

void CWndPetFoodMill::SetItem(DWORD dwObjId)
{
	m_dwObjId = dwObjId;
}

void CWndPetFoodMill::SetItemForFeed(FLItemElem* pItemElem, int nCount)
{
	if(pItemElem != NULL)
	{
		m_pItemElem = pItemElem;
		m_nItemCount = nCount;
		m_pItemElem->SetExtra( nCount );
			
		PT_ITEM_SPEC pItemProp = m_pItemElem->GetProp();
		
		if(pItemProp != NULL)
			m_pTexture = CWndBase::m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, pItemProp->szIcon), 0xffff00ff );
	}
}

void CWndPetFoodMill::ReceiveResult(int nResult, int nCount)
{
	switch(nResult) 
	{
		case 0:
			g_WndMng.PutString( prj.GetText(TID_GAME_LACKSPACE), NULL, prj.GetTextColor(TID_GAME_LACKSPACE) );
			break;
		case 1:
			CString strtmp;
			strtmp.Format(prj.GetText(TID_GAME_PETFEED_MAKE), nCount);
			g_WndMng.PutString( strtmp, NULL, prj.GetTextColor(TID_GAME_PETFEED_MAKE) );
			if(m_pItemElem != NULL)
			{
				if( !g_pPlayer->m_vtInfo.IsTrading( m_pItemElem ) )
					m_pItemElem->SetExtra(0);
				m_pItemElem = NULL;
			}
			if(m_pTexture != NULL)
				m_pTexture = NULL;
			break;
	}
}

//////////////////////////////////////////////////////////////////////////
//	CWndPetLifeConfirm
//////////////////////////////////////////////////////////////////////////

CWndPetLifeConfirm::CWndPetLifeConfirm() 
{
	m_nId	= -1;
}

CWndPetLifeConfirm::~CWndPetLifeConfirm() 
{ 
} 

void CWndPetLifeConfirm::OnDestroy()
{
}

void CWndPetLifeConfirm::SetItem(int nId)
{
	m_nId	= nId;
}

void CWndPetLifeConfirm::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndPetLifeConfirm::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate();

	CRect rect( GetClientRect() );
	int x = m_rectClient.Width() / 2;
	int y = m_rectClient.Height() - 30;
	CSize size = CSize( 60,25 );
	
	CRect rtYesButton( 0, 0, 0, 0 );
	CRect rtNoButton( 0, 0, 0, 0 );
	
	if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
	{
		rtYesButton =	CRect( x - size.cx - 10, y, ( x - size.cx - 10 ) + size.cx, y + size.cy );
		rtNoButton =	CRect( x + 10          , y, ( x + 10           ) + size.cx, y + size.cy );
	}

	if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
	{
		rtYesButton =	CRect( x - size.cx - 15, y, ( x - size.cx - 10 ) + size.cx, y + size.cy );
		rtNoButton =	CRect( x + 5, y, ( x + 10 ) + size.cx, y + size.cy );
	}

	rect.DeflateRect( 10, 10, 10, 35 );
	m_wndText.AddWndStyle( WBS_VSCROLL );
	m_wndText.Create( WBS_NODRAWFRAME, rect, this, 0 );
	m_strText = prj.GetText( TID_GAME_PET_USELIFE );
	m_wndText.SetString( m_strText, m_dwColor );
	m_wndText.ResetString();

	if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
	{
		m_wndButton1.Create("YES",	0, rtYesButton, this, IDYES);
		m_wndButton2.Create("NO",	0, rtNoButton, this, IDNO);
		m_wndButton1.SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME,g_xFlyffConfig->GetMainLanguage(), "ButtYes.tga" ) );
		m_wndButton2.SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME,g_xFlyffConfig->GetMainLanguage(), "ButtNo.tga" ) );
	}

	if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
	{
		m_wndButton1.Create( prj.GetText( TID_MMI_UIBUTTON_BUTTON02 ), 0, rtYesButton, this, IDYES );
		m_wndButton2.Create( prj.GetText( TID_MMI_UIBUTTON_BUTTON03 ), 0, rtNoButton, this, IDNO );
		m_wndButton1.SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME,g_xFlyffConfig->GetMainLanguage(), "ButtNormal00.tga" ) );
		m_wndButton2.SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME,g_xFlyffConfig->GetMainLanguage(), "ButtNormal00.tga" ) );
	}

	m_wndButton1.FitTextureSize();
	m_wndButton2.FitTextureSize();

	MoveParentCenter();
} 
// 처음 이 함수를 부르면 윈도가 열린다.
BOOL CWndPetLifeConfirm::Initialize( CWndBase* pWndParent, DWORD dwWndId )
{ 
	// Daisy에서 설정한 리소스로 윈도를 연다.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_MESSAGEBOX, 0, CPoint( 0, 0 ), pWndParent );
} 
BOOL CWndPetLifeConfirm::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndPetLifeConfirm::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndPetLifeConfirm::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndPetLifeConfirm::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndPetLifeConfirm::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == IDYES )
	{
		//Send to Server..
		if(	m_nId != -1 )
			g_DPlay.SendDoUseItem( MAKELONG( 0, m_nId ), NULL_ID, -1, FALSE);
	}
	else if( nID == IDNO )
	{
		//그냥 종료
	}
	
	Destroy();
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
}

//sun: 12, 펫 알 변환 기능 추가

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CWndPetTransEggs Class
////////////////////////////////////////////////////////////////////////////////////////////////////////////

CWndPetTransEggs::CWndPetTransEggs() 
{
	ResetEgg();
}

CWndPetTransEggs::~CWndPetTransEggs() 
{ 
} 

void CWndPetTransEggs::OnDestroy()
{
	for(int i=0; i<MAX_TRANS_EGG; i++)
	{
		if(m_pItemElem[i] != NULL)
			m_pItemElem[i]->SetExtra(0);
	}
}

void CWndPetTransEggs::OnDraw( C2DRender* p2DRender ) 
{
	LPWNDCTRL wndCtrl;
	
	for(int i=0; i<MAX_TRANS_EGG; i++)
	{
		if(m_pItemElem[i] != NULL)
		{
			wndCtrl = GetWndCtrl( m_nCtrlId[i] );
			m_pEggTexture->Render( p2DRender, CPoint( wndCtrl->rect.left, wndCtrl->rect.top ) );
		}
	}
}

void CWndPetTransEggs::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// 여기에 코딩하세요

	CWndButton* pButton = (CWndButton*)GetDlgItem(WIDC_OK);
	pButton->EnableWindow(FALSE);

	m_pText = (CWndText*)GetDlgItem( WIDC_TEXT1 );

	m_nCtrlId[0] = WIDC_STATIC1;
	m_nCtrlId[1] = WIDC_STATIC2;
	m_nCtrlId[2] = WIDC_STATIC3;
	m_nCtrlId[3] = WIDC_STATIC4;
	m_nCtrlId[4] = WIDC_STATIC5;
	m_nCtrlId[5] = WIDC_STATIC6;
	m_nCtrlId[6] = WIDC_STATIC7;
	m_nCtrlId[7] = WIDC_STATIC8;
	m_nCtrlId[8] = WIDC_STATIC9;
	m_nCtrlId[9] = WIDC_STATIC10;

	PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( ITEM_INDEX( 21029, II_PET_EGG ) );
	if(pItemProp)
		m_pEggTexture = CWndBase::m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, pItemProp->szIcon), 0xffff00ff );

	SetDescription(NULL);

	MoveParentCenter();
} 
// 처음 이 함수를 부르면 윈도가 열린다.
BOOL CWndPetTransEggs::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy에서 설정한 리소스로 윈도를 연다.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_PET_TRANS_EGGS, 0, CPoint( 0, 0 ), pWndParent );
}

void CWndPetTransEggs::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	CRect rect;

	for(int i=0; i<MAX_TRANS_EGG; i++)
	{
		LPWNDCTRL wndCtrl = GetWndCtrl( m_nCtrlId[i] );

		if(wndCtrl)
		{
			rect = wndCtrl->rect;
			if(rect.PtInRect( point ))
			{
				if(m_pItemElem[i])
				{
					m_pItemElem[i]->SetExtra(0);
					m_pItemElem[i] = NULL;
					CWndButton* pButton = (CWndButton*)GetDlgItem(WIDC_OK);
					pButton->EnableWindow(FALSE);
				}
			}
		}
	}
}

void CWndPetTransEggs::OnMouseWndSurface(CPoint point)
{
	CRect rect;
	LPWNDCTRL wndCtrl;

	for(int i=0; i<MAX_TRANS_EGG; i++)
	{
		if(m_pItemElem[i] != NULL)
		{
			wndCtrl = GetWndCtrl( m_nCtrlId[i] );
			rect = wndCtrl->rect;
			if( rect.PtInRect( point ) )
			{
				ClientToScreen( &point );
				ClientToScreen( &rect );
				g_WndMng.PutToolTip_Item( (FLItemBase*)m_pItemElem[i], point, &rect );
			}
		}
	}
}

BOOL CWndPetTransEggs::OnDropIcon( LPSHORTCUT pShortcut, CPoint point )
{
	CRect rect;
	FLItemElem* pItemElem;
	pItemElem = (FLItemElem*)g_pPlayer->GetItemId( pShortcut->m_dwId );

	if(g_pPlayer->IsUsing(pItemElem))
	{
		g_WndMng.PutString( prj.GetText( TID_GAME_TRANS_EGGS_ERROR2 ), NULL, prj.GetTextColor( TID_GAME_TRANS_EGGS_ERROR2 ) );
	}
	else
	{

		if( (pItemElem->GetProp()->dwItemKind3 == IK3_EGG && pItemElem->m_pPet == NULL) ||
			(pItemElem->GetProp()->dwItemKind3 == IK3_EGG && pItemElem->m_pPet && pItemElem->m_pPet->GetLevel() == PL_EGG) )
		{
			if( IsUsableItem( pItemElem ) )
			{
				for(int i=0; i<MAX_TRANS_EGG; i++)
				{
					LPWNDCTRL wndCtrl = GetWndCtrl( m_nCtrlId[i] );

					if(wndCtrl)
					{
						rect = wndCtrl->rect;
						if(rect.PtInRect( point ))
						{
							if(m_pItemElem[i] == NULL)
							{
								m_pItemElem[i] = pItemElem;
								m_pItemElem[i]->SetExtra(m_pItemElem[i]->GetExtra()+1);
							}
						}
					}
				}

				CheckFull();
			}
		}
		else
			g_WndMng.PutString( prj.GetText( TID_GAME_TRANS_EGGS_ERROR1 ), NULL, prj.GetTextColor( TID_GAME_TRANS_EGGS_ERROR1 ) );
	}

	return TRUE;
}

void CWndPetTransEggs::SetItem(FLItemElem* pItemElem)
{
	int nEmpty = -1;

	for(int i=0; i<MAX_TRANS_EGG; i++)
	{
		if(m_pItemElem[i] == NULL)
		{
			nEmpty = i;
			i = MAX_TRANS_EGG;
		}
	}

	if(nEmpty != -1)
	{
		m_pItemElem[nEmpty] = pItemElem;
		m_pItemElem[nEmpty]->SetExtra(m_pItemElem[nEmpty]->GetExtra()+1);
	}

	CheckFull();
}

BOOL CWndPetTransEggs::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_OK )
	{
		CTransformStuff stuff( 0 );
		
		for(int i=0; i<MAX_TRANS_EGG; i++)
			stuff.AddComponent( m_pItemElem[i]->m_dwObjId, 1 );

		g_DPlay.SendTransformItem( stuff );
		Destroy();
	}
	else if( nID == WIDC_CANCEL )
	{
		Destroy();
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

void CWndPetTransEggs::SetDescription( CHAR* szChar )
{
	CScript scanner;
	BOOL checkflag;
	checkflag = scanner.Load( MakePath( DIR_CLIENT,  _T( "PetTransEggs.inc" ) ));
	szChar = scanner.m_pProg;
	
	if(m_pText != NULL && checkflag)
	{
		m_pText->m_string.AddParsingString( szChar );
		m_pText->ResetString();
	}
}

void CWndPetTransEggs::ResetEgg()
{
	for(int i=0; i<MAX_TRANS_EGG; i++)
		m_pItemElem[i] = NULL;
}

void CWndPetTransEggs::CheckFull()
{
	BOOL bFull = TRUE;

	for(int i=0; i<MAX_TRANS_EGG; i++)
	{
		if(m_pItemElem[i] == NULL)
		{
			bFull = FALSE;
			i = MAX_TRANS_EGG;
		}
	}

	if(bFull)
	{
		CWndButton* pButton = (CWndButton*)GetDlgItem(WIDC_OK);
		pButton->EnableWindow(TRUE);
	}
}


//---------------------------------------------------------------------------------------------
// 버프 팻 상태 윈도우
//---------------------------------------------------------------------------------------------

int s_nBuffPetX = 0;
int s_nBuffPetY = 0;

//---------------------------------------------------------------------------------------------
// 생성자
//---------------------------------------------------------------------------------------------
CWndBuffPetStatus::CWndBuffPetStatus()
:	m_pPetModel( NULL )
,	m_pWndConfirmVis( NULL )
,	m_pTexPetStatusBg( NULL )
,	m_fRadius( 0.0f )
{
	ZeroMemory( m_pItemElem,	sizeof( FLItemElem* ) * MAX_VIS );
	ZeroMemory( m_pTexture,		sizeof( CTexture* ) * MAX_VIS );
	ZeroMemory( m_nCtrlId,		sizeof( int ) * MAX_VIS );
}


//---------------------------------------------------------------------------------------------
// 파괴자
//---------------------------------------------------------------------------------------------
CWndBuffPetStatus::~CWndBuffPetStatus( )
{
	CRect rect1 = GetScreenRect();
	s_nBuffPetX = rect1.left;
	s_nBuffPetY = rect1.top;
	DeleteDeviceObjects();
}


//---------------------------------------------------------------------------------------------
// 초기화
// param	: 부모 윈도우, 생성 윈도우 아이디
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndBuffPetStatus::Initialize(CWndBase* pWndParent,DWORD dwWndId)
{
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_BUFFPET_STATUS, 0, CPoint( s_nBuffPetX, s_nBuffPetY ), pWndParent );
}


//---------------------------------------------------------------------------------------------
// 장치 오브젝트 복구
// param	: void
// return	: HRESULT
//---------------------------------------------------------------------------------------------
HRESULT CWndBuffPetStatus::RestoreDeviceObjects()
{
	CWndBase::RestoreDeviceObjects();
	return S_OK;
}


//---------------------------------------------------------------------------------------------
// 장치 오브젝트 무효화
// param	: void
// return	: HRESULT
//---------------------------------------------------------------------------------------------
HRESULT CWndBuffPetStatus::InvalidateDeviceObjects()
{
	CWndBase::InvalidateDeviceObjects();
	SAFE_DELETE( m_pWndConfirmVis );

	for( int i = 0; i < MAX_VIS; ++i )
		SAFE_DELETE( m_pItemElem[ i ] );

	return S_OK;
}


//---------------------------------------------------------------------------------------------
// 장치 오브젝트 삭제
// param	: void
// return	: HRESULT
//---------------------------------------------------------------------------------------------
HRESULT CWndBuffPetStatus::DeleteDeviceObjects()
{
	CWndBase::DeleteDeviceObjects();
	return InvalidateDeviceObjects();
}


//---------------------------------------------------------------------------------------------
// 업데이트 전 초기화
// param	: void
// return	: void
//---------------------------------------------------------------------------------------------
void CWndBuffPetStatus::OnInitialUpdate()
{
	CWndNeuz::OnInitialUpdate();
	
	RestoreDeviceObjects();
	
	m_pTexPetStatusBg = CWndBase::m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_THEME,g_xFlyffConfig->GetMainLanguage(),"BuffpetStatusBg.tga"), 0, TRUE );

	if( CS_VER1 == _GetContentState( CT_NEWUI_19 ) )
	{
		CWndStatus*			pWndStatus( (CWndStatus*)GetWndBase( APP_STATUS1 ) );
		CWndBase*			pWndPickupPet( GetWndBase( APP_PET_STATUS ) );

		if( pWndPickupPet )
		{
			CRect rectBrother = pWndPickupPet->GetWndRect( );
			int x = rectBrother.left;
			int y = rectBrother.top + 20 ;

			Move( CPoint(x, y));
		}
		else if( pWndStatus )
		{
			CRect rectRoot = pWndStatus->GetWndRect();
			CRect rect = GetWindowRect();
			int x = rectRoot.right;
			int y = rectRoot.top;	

			CRect kWRect;
			CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
			if( pWndWorld )
			{
				kWRect = pWndWorld->GetClientRect( );
				if( ( x + rect.Width() ) > kWRect.right )
					x = rectRoot.left - rect.Width();
			}

			Move( CPoint(x, y));
		}
		else
		{
			CRect rectRoot = m_pWndRoot->GetLayoutRect();
			CPoint point( rectRoot.left+192, rectRoot.top );
			Move( point );
		}
	}

	if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
	{
		CWndPetStatus* pWndPickupPet( (CWndPetStatus*)GetWndBase( APP_PET_STATUS ) );

		if( pWndPickupPet )
			pWndPickupPet->CalculationWindowPosition();	
	}

	m_nCtrlId[0] = WIDC_BUFFPET_SLOT1;
	m_nCtrlId[1] = WIDC_BUFFPET_SLOT2;
	m_nCtrlId[2] = WIDC_BUFFPET_SLOT3;
	m_nCtrlId[3] = WIDC_BUFFPET_SLOT4;
	m_nCtrlId[4] = WIDC_BUFFPET_SLOT5;
	m_nCtrlId[5] = WIDC_BUFFPET_SLOT6;
	m_nCtrlId[6] = WIDC_BUFFPET_SLOT7;
	m_nCtrlId[7] = WIDC_BUFFPET_SLOT8;
	m_nCtrlId[8] = WIDC_BUFFPET_SLOT9;
}


//void CWndBuffPetStatus::OnDraw(C2DRender* p2DRender)
//{
//	if( g_pPlayer == NULL ) 
//		return;
//
//	//Draw slots
//	DrawSlotItems( p2DRender );
//
//	if( !m_pPetModel )
//		return;
//
//	CMover* pMoverPet = prj.GetMover( g_pPlayer->GetEatPetId( ) );
//	if( !pMoverPet )
//		return;
//	
//	CModelObject* pModel = (CModelObject *)pMoverPet->m_pModel;
//	if( !pModel )
//		return;
//	
//	LPWNDCTRL lpFace = GetWndCtrl( WIDC_CUSTOM1 );
//	if( NULL == lpFace )
//		return;
//
//	CRect rect( GetClientRect() );
//	int nWidthClient = GetClientRect().Width() - 110;
//
//	CPoint point;
//	point = CPoint( lpFace->rect.left-12, lpFace->rect.top-22 );
//	
//	if(m_pTexPetStatusBg != NULL)
//		m_pTexPetStatusBg->Render( p2DRender,  point );
//		
//
//	LPDIRECT3DDEVICE9 pd3dDevice = p2DRender->m_pd3dDevice;
//	pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0xffa08080, 1.0f, 0 ) ;
//
//	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
//	pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
//	pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
//	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
//	pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
//	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
//
//	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
//	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
//	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
//	pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );		
//	pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );		
//
//	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
//
//	// 뷰포트 세팅 
//	D3DVIEWPORT9 viewport;
//
//	viewport.X      = p2DRender->m_ptOrigin.x + lpFace->rect.left;
//	viewport.Y      = p2DRender->m_ptOrigin.y + lpFace->rect.top;
//	viewport.Width  = lpFace->rect.Width();
//	viewport.Height = lpFace->rect.Height();
//	viewport.MinZ   = 0.0f;
//	viewport.MaxZ   = 1.0f;
//
//	pd3dDevice->SetViewport(&viewport);
//
//	// 프로젝션 
//	D3DXMATRIX matProj;
//	D3DXMatrixIdentity( &matProj );
//	FLOAT fAspect = ((FLOAT)viewport.Width) / (FLOAT)viewport.Height;
//
//	m_fRadius = pModel->GetRadius( );
//
//	// 너무 작을경우 보정
//	if( m_fRadius < 0.8f )
//		m_fRadius = 0.8f;
//
//	m_fRadius *= 3.0f;
//
//	FLOAT fov = D3DX_PI/m_fRadius; //4.0f;//796.0f;
//	FLOAT h = cos(fov/2) / sin(fov/2);
//	FLOAT w = h * fAspect;
//	D3DXMatrixOrthoLH( &matProj, w, h, CWorld::m_fNearPlane - 0.01f, CWorld::m_fFarPlane );
//	pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
//
//	D3DXMATRIX  matView;
//
//	// 월드 
//	D3DXMATRIXA16 matWorld;
//	D3DXMATRIXA16 matRot1, matRot2;
//	D3DXMATRIXA16 matTrans;
//
//	// 초기화 
//	D3DXMatrixIdentity(&matRot1);
//	D3DXMatrixIdentity(&matRot2);
//	D3DXMatrixIdentity(&matTrans);
//	D3DXMatrixIdentity(&matWorld);
//
//	//펫 종류에 따라 설정.
//	D3DXVECTOR3 vecEye;
//	D3DXVECTOR3 vecLookAt;
//
//	//CModelObject에는 이벤트 좌표가 없는데 CObject3D는 있고?
//	D3DXMATRIX* pMat = pModel->GetMatrixBone( 0 );
//	vecLookAt.x = pMat->_41;
//	vecLookAt.y = pMat->_42;
//	vecLookAt.z = pMat->_43;
//
//	// mdldyna.inc에서 스케일을 조정한경우 그에 맞게 보정을 해주는데, 어떤 원리인지 내가 했지만 이상함. 나중에 다시 정확히 잡아볼까?
////	float fModelScale = pModel->m_pModelElem->m_fScale;
////	if( fModelScale < 1.0f && fModelScale > 0.001f )
////		vecPos *= ( fModelScale - fModelScale * (0.5f + ( 1.0f - fModelScale ) * 0.01f ) );	//스케일 변동치가 클수록 
////	else if ( fModelScale > 1.0f )
////		vecPos *= ( fModelScale  - fModelScale * (0.9f + fModelScale * 0.01f) );
//
//	vecEye = vecLookAt;
//	vecLookAt.z += 1.4f;
//
//	vecEye.y += 1.0f;
//	vecEye.z -= 3.0f; 
//
//	D3DXMatrixLookAtLH( &matView, &vecEye, &vecLookAt, &D3DXVECTOR3(0.0f,1.0f,0.0f) );
//	pd3dDevice->SetTransform( D3DTS_VIEW, &matView );
//	
//	D3DXMatrixMultiply(&matWorld, &matWorld,&matRot1);
//	D3DXMatrixMultiply(&matWorld, &matWorld, &matTrans );
//	pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
//
//	// 랜더링 
//	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
//	pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
//	pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, CWorld::m_dwBgColor, 1.0f, 0 ) ;
//
//	::SetTransformView( matView );
//	::SetTransformProj( matProj );
//
//	//gmpbigsun : 윈도 페이스 고정라이트
// 	::SetLight( FALSE );
// 	::SetFog( FALSE );
// 	SetDiffuse( 1.0f, 1.0f, 1.0f );
// 	SetAmbient( 1.0f, 1.0f, 1.0f );
//
//	m_pPetModel->SetTextureEx( m_pPetModel->m_pModelElem->m_nTextureEx );
//	m_pPetModel->Render(pd3dDevice, &matWorld);
//	
//	pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
//	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
//	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
//	pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
//	pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE ); 
//}


//---------------------------------------------------------------------------------------------
// 그리기
// param	: 렌더러
// return	: void
//---------------------------------------------------------------------------------------------
void CWndBuffPetStatus::OnDraw(C2DRender* p2DRender)
{
	if( p2DRender == NULL ) 
		return;

	if( g_pPlayer == NULL ) 
		return;

	//Draw slots
	DrawSlotItems( p2DRender );
	DrawBackGround( p2DRender );
	DrawPetModelObject( p2DRender );
}


//---------------------------------------------------------------------------------------------
// 모델 오브젝트 배경 그리기
// param	: 렌더러
// return	: void
//---------------------------------------------------------------------------------------------
void CWndBuffPetStatus::DrawBackGround( C2DRender* p2DRender )
{
	LPWNDCTRL lpWndBackGround = GetWndCtrl( WIDC_CUSTOM1 );
	if( NULL == lpWndBackGround )
		return;

	if( m_pTexPetStatusBg )
		m_pTexPetStatusBg->Render( p2DRender,  lpWndBackGround->rect.TopLeft() );
}


//---------------------------------------------------------------------------------------------
// 모델 오브젝트 그리기
// param	: 렌더러
// return	: void
//---------------------------------------------------------------------------------------------
void CWndBuffPetStatus::DrawPetModelObject( C2DRender* p2DRender )
{
	if( NULL == m_pPetModel )
		return;

	CMover* pMoverPet( prj.GetMover( g_pPlayer->GetEatPetId() ) );
	if( NULL == pMoverPet )
		return;
	
	CModelObject* pModel = (CModelObject *)pMoverPet->m_pModel;
	if( NULL == pModel )
		return;

	LPWNDCTRL lpWndRender = GetWndCtrl( WIDC_CUSTOM2 );
	if( NULL == lpWndRender )
		return;

	LPDIRECT3DDEVICE9 pd3dDevice = p2DRender->m_pd3dDevice;

	pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, CWorld::m_dwBgColor, 1.0f, 0 ) ;

	SetRenderState( p2DRender );
	SetViewport( p2DRender, lpWndRender->rect );
	SetProjection( p2DRender, pModel );

	D3DXMATRIX		matView;
	D3DXMATRIXA16	matWorld;

	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matWorld);

	D3DXVECTOR3		vecLookAt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3		vecEye( 0.0f, 0.0f, 0.0f );

	const BOUND_BOX* pBB = pModel->GetBBVector();
	if( pBB )
	{
		D3DXVECTOR3 vDiagonal( pBB->m_vPos[3] - pBB->m_vPos[5] );
		vDiagonal = vDiagonal * 0.5f;
		
		vecLookAt = vDiagonal + pBB->m_vPos[5];		
		vecEye = vecLookAt; 

		vecEye.z -= pModel->GetRadius() + 2.0f;
	}
	else
	{
		D3DXMATRIX* pMat = pModel->GetMatrixBone( 0 );
		if( pMat )
		{
			vecLookAt.x = pMat->_41;
			vecLookAt.y = pMat->_42;
			vecLookAt.z = pMat->_43;

			vecEye = vecLookAt;
			vecLookAt.z += 1.4f;

			vecEye.y += 1.0f;
			vecEye.z -= 3.0f; 
		}
	}

	D3DXMatrixLookAtLH( &matView, &vecEye, &vecLookAt, &D3DXVECTOR3(0.0f,1.0f,0.0f) );
	pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

	pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	::SetTransformView( matView );

	::SetLight( FALSE );
 	::SetFog( FALSE );

	SetDiffuse( 1.0f, 1.0f, 1.0f );
 	SetAmbient( 1.0f, 1.0f, 1.0f );

	m_pPetModel->SetTextureEx( m_pPetModel->m_pModelElem->m_nTextureEx );
	m_pPetModel->Render( pd3dDevice, &matWorld );

	RestoreRenderState( p2DRender );
}	


//---------------------------------------------------------------------------------------------
// 렌더링 설정 
// param	: 렌더러
// return	: void
//---------------------------------------------------------------------------------------------
void CWndBuffPetStatus::SetRenderState( C2DRender* p2DRender )
{
	LPDIRECT3DDEVICE9 pd3dDevice = p2DRender->m_pd3dDevice;

	pd3dDevice->SetRenderState( D3DRS_ZENABLE,				TRUE );
	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,			TRUE );
	pd3dDevice->SetRenderState( D3DRS_CULLMODE,				D3DCULL_CCW );
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,		FALSE );
	pd3dDevice->SetRenderState( D3DRS_FILLMODE,				D3DFILL_SOLID );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,	D3DTOP_SELECTARG1 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,	D3DTOP_SELECTARG1 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,	D3DTOP_SELECTARG1 );

	pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER,		D3DTEXF_LINEAR );		
	pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER,		D3DTEXF_LINEAR );		
}


//---------------------------------------------------------------------------------------------
// 뷰 포트 설정
// param	: 렌더러
// return	: void
//---------------------------------------------------------------------------------------------
void CWndBuffPetStatus::SetViewport( C2DRender* p2DRender, CRect rtRenderRect )
{
	LPDIRECT3DDEVICE9 pd3dDevice = p2DRender->m_pd3dDevice;

	// 뷰 포트 세팅 
	D3DVIEWPORT9	viewport;

	viewport.X      = p2DRender->m_ptOrigin.x + rtRenderRect.left;
	viewport.Y      = p2DRender->m_ptOrigin.y + rtRenderRect.top;
	viewport.Width  = rtRenderRect.Width();
	viewport.Height = rtRenderRect.Height();
	viewport.MinZ   = 0.0f;
	viewport.MaxZ   = 1.0f;

	pd3dDevice->SetViewport(&viewport);
}


//---------------------------------------------------------------------------------------------
// 투영 설정 ( 직교투영으로 변경 )
// param	: 렌더러
// return	: void
//---------------------------------------------------------------------------------------------
void CWndBuffPetStatus::SetProjection( C2DRender* p2DRender, CModelObject* pModel )
{
	LPDIRECT3DDEVICE9 pd3dDevice = p2DRender->m_pd3dDevice;

	D3DVIEWPORT9 viewport;
	pd3dDevice->GetViewport( &viewport );

	D3DXMATRIX matProj;
	D3DXMatrixIdentity( &matProj );
	
	m_fRadius = pModel->GetRadius();

	// 너무 작을경우 보정
	if( m_fRadius < 0.8f )
		m_fRadius = 0.8f;

	m_fRadius *= 3.0f;

	float fAspect = (float)viewport.Width / (float)viewport.Height;

	FLOAT fov	= D3DX_PI/m_fRadius; 
	FLOAT h		= cos(fov/2) / sin(fov/2);
	FLOAT w		= h * fAspect;
	D3DXMatrixOrthoLH( &matProj, w, h, CWorld::m_fNearPlane - 0.01f, CWorld::m_fFarPlane );

	pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	::SetTransformProj( matProj );
}


//---------------------------------------------------------------------------------------------
// 렌더링 설정 복구
// param	: 렌더러
// return	: void
//---------------------------------------------------------------------------------------------
void CWndBuffPetStatus::RestoreRenderState( C2DRender* p2DRender )
{
	LPDIRECT3DDEVICE9 pd3dDevice = p2DRender->m_pd3dDevice;

	pd3dDevice->SetRenderState( D3DRS_ZENABLE,				FALSE );
	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,			FALSE );
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,		TRUE );
	pd3dDevice->SetRenderState( D3DRS_CULLMODE,				D3DCULL_NONE );
	pd3dDevice->SetRenderState( D3DRS_FOGENABLE,			FALSE ); 
}





BOOL CWndBuffPetStatus::Process()
{
	if(!IsValidObj(g_pPlayer))
		return FALSE;

		
	// 장착, 게이지에 나올 캐릭터 오브젝트 설정
	if( g_pPlayer->HasActivatedVisPet( ) )
	{
		CMover* pMyBuffPet = NULL;

 		if( !m_pPetModel )
 		{
 			pMyBuffPet = prj.GetMover( g_pPlayer->GetEatPetId( ) );
 			if( pMyBuffPet )
			{
				FLStrcpy( pMyBuffPet->m_szCharacterKey, _countof( pMyBuffPet->m_szCharacterKey ), "MaFl_BuffPet" );
				m_pPetModel = (CModelObject*)pMyBuffPet->GetModel( );		// 버프펫이 생성됐다면 모델공유 

				//버프펫 모델이 준비되면 타이틀 (펫이름) 세팅!
				m_strTitle = pMyBuffPet->GetName( );
			}
			else
 				return TRUE;
 		}
	}

	return TRUE;
}

void CWndBuffPetStatus::UpdateVisState( )
{
	if( g_pPlayer )
		m_cVisStates = g_pPlayer->GetValidVisTable( g_pPlayer->GetVisPetItem( ) );
}


BOOL CWndBuffPetStatus::OnDropIcon( LPSHORTCUT pShortcut, CPoint point )
{
	CWndBase* pWndFrame	= pShortcut->m_pFromWnd->GetFrameWnd();
	if( !pWndFrame )
	{
		FLASSERT( 0 );
		return FALSE;
	}

	int selectedSlot = GetSlotIndexByPoint( point );
	if( selectedSlot < 0  )
		return FALSE;

	if( !IsFreeSlot( selectedSlot ) )
		return FALSE;

	if( APP_INVENTORY == pWndFrame->GetWndId( ) )			// 인벤에서 온 아이템은 조건검사 
	{
		FLItemElem* pItem;
		pItem = g_pPlayer->m_Inventory.GetAtId( pShortcut->m_dwId );	
		
		if( !IsUsableItem( pItem ) )
			return FALSE;

		if( !pItem->GetProp( )->IsVis( ) )
		return FALSE;
	}

	if( pWndFrame->GetWndId( ) != APP_INVENTORY )				
	{
		if( APP_BUFFPET_STATUS == pWndFrame->GetWndId( ) )			//같은 창 내에서 이동일 경우 
		{
			int selectedSlot = GetSlotIndexByPoint( point );
			if( selectedSlot < 0  )
				return FALSE;
			
			if( pShortcut->m_dwData == selectedSlot )			//	아이콘을 같은자리에 놓았다.
				return FALSE;

			//스왑요청 
			g_DPlay.SendSwapVis( pShortcut->m_dwData, selectedSlot );
		}
		
		return TRUE;
	}


	//확인창 띄워주고 확인창에서 OK시 SendDoUseItem
	if( APP_INVENTORY == pWndFrame->GetWndId( ) )
		DoModal_ConfirmQuestion( pShortcut->m_dwId, g_pPlayer->GetId(), ((FLItemElem*)pShortcut->m_dwData)->m_dwItemId );
	else
		DoModal_ConfirmQuestion( pShortcut->m_dwId, g_pPlayer->GetId(), pShortcut->m_dwIndex );

	return TRUE;
}

void CWndBuffPetStatus::OnDestroy()
{
	for(int i=0; i<MAX_VIS; i++)
		SAFE_DELETE( m_pItemElem[ i ] );
}


void CWndBuffPetStatus::DrawSlotItems( C2DRender* p2DRender )
{
	if( !g_pPlayer )
		return;

	FLItemElem* pItem = g_pPlayer->GetVisPetItem();
	if( !pItem )
		return;

	int nAvailableSlot = pItem->GetGeneralPiercingSize( );
	if( nAvailableSlot > MAX_VIS )
		return;

	UpdateVisState( );

	LPWNDCTRL wndCtrl = NULL;
	for( int i=0; i<MAX_VIS; ++i )
	{
		if( i < nAvailableSlot )
		{
			DWORD dwItemIndex = pItem->GetGeneralPiercingItemID( i );
	
			PT_ITEM_SPEC pProp = g_xSpecManager->GetSpecItem( dwItemIndex );
			if( !pProp )
				continue;

			m_pTexture[ i ] = CWndBase::m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, pProp->szIcon), 0xffff00ff );
			
			if(m_pTexture[ i ] != NULL)
			{
				
				DWORD color = ( m_cVisStates.at( i ) == SUCCSESS_NEEDVIS ? 0xffffffff : 0x4fffffff );

				wndCtrl = GetWndCtrl( m_nCtrlId[i] );
				if( wndCtrl )
				{
					CPoint pt2 = CPoint( wndCtrl->rect.left, wndCtrl->rect.top );
					m_pTexture[ i ]->Render2( p2DRender, CPoint( wndCtrl->rect.left, wndCtrl->rect.top ), color );
				}
			}
		}
		else
		{
			//사용불가능 슬롯 
			wndCtrl = GetWndCtrl( m_nCtrlId[i] );
			CTexture* pTexClosed = CWndBase::m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, "Icon_Lock.dds" ), 0xffff00ff );
			if( pTexClosed )
				pTexClosed->Render( p2DRender, CPoint( wndCtrl->rect.left, wndCtrl->rect.top ) );

		}
	}
}


BOOL CWndBuffPetStatus::DoModal_ConfirmQuestion( DWORD dwItemId, OBJID dwObjid, DWORD dwIndex, int nSlot, CWndConfirmVis::ConfirmVisSection eSection )
{
	SAFE_DELETE( m_pWndConfirmVis );
	m_pWndConfirmVis = new CWndConfirmVis;
	m_pWndConfirmVis->m_dwItemId = dwItemId;
	m_pWndConfirmVis->m_objid = dwObjid;
	m_pWndConfirmVis->m_nSlot = nSlot;
	m_pWndConfirmVis->m_eSection = eSection;
	m_pWndConfirmVis->m_dwItemIndex = dwIndex;
	
	//부모를 g_WndMng로 설정안한경우 Modal process에서 비정상적인 작동을 한다.
	//모달속성의 윈도가 뜬경우 g_WndMng로 메세지를 보내는게 유일해서 그런듯. CNeuzApp::MsgProc참고
	m_pWndConfirmVis->Initialize( &g_WndMng, APP_CONFIRM_ENTER );
		
	return TRUE;
}

int CWndBuffPetStatus::GetSlotIndexByPoint( const CPoint& point )
{
	LPWNDCTRL wndCtrl = NULL;
	CRect rect;
	for( int i = 0; i < MAX_VIS; ++i )
	{
		wndCtrl = GetWndCtrl( m_nCtrlId[i] );		// 슬롯으로 만들어진 윈도우에 대하여 
		rect  = wndCtrl->rect;

		if( rect.PtInRect( point ) )
			return i;		
	}

	return -1;
}

void CWndBuffPetStatus::OnLButtonDown( UINT nFlags, CPoint point )
{
	//해당 슬롯을 판별해서 정보를 shortcut에 넣어준다.

	LPWNDCTRL wndCtrl = NULL;
	CRect rect;

	int selectedSlot = GetSlotIndexByPoint( point );
	if( selectedSlot < 0 )
		return;

	FLItemElem* pItem = g_pPlayer->GetVisPetItem();
	if( !pItem )
		return;

	DWORD dwItemIndex = pItem->GetGeneralPiercingItemID( selectedSlot );		// 클릭을 했으나 비스가 없는 슬롯은 패스 
	if( 0 == dwItemIndex )
		return;
	
	FLASSERT( selectedSlot >= 0 && selectedSlot < MAX_VIS );
	FLASSERT( m_pTexture[ selectedSlot ] );
	
	m_GlobalShortcut.m_pFromWnd = this;
	m_GlobalShortcut.m_dwShortcut = SHORTCUT_ITEM;
	m_GlobalShortcut.m_dwType     = ITYPE_ITEM;
	m_GlobalShortcut.m_dwId       = 0;
	m_GlobalShortcut.m_dwIndex    = dwItemIndex;
	m_GlobalShortcut.m_pTexture   = m_pTexture[ selectedSlot ];
	m_GlobalShortcut.m_dwData     = selectedSlot;
}

void CWndBuffPetStatus::OnLButtonDblClk( UINT nFlags, CPoint point)
{
	//파괴 
	int selectedSlot = GetSlotIndexByPoint( point );
	if( selectedSlot < 0 )
		return;

	FLItemElem* pItem = g_pPlayer->GetVisPetItem();
	if( !pItem )
		return;

	DWORD dwItemIndex = pItem->GetGeneralPiercingItemID( selectedSlot );		// 클릭을 했으나 비스가 없는 슬롯은 패스 
	if( 0 == dwItemIndex )
		return;

	DoModal_ConfirmQuestion( 0, 0, dwItemIndex, selectedSlot, CWndConfirmVis::CVS_UNEQUIP_VIS );
}

BOOL CWndBuffPetStatus::IsVisItem( DWORD index )
{
	PT_ITEM_SPEC pProp = g_xSpecManager->GetSpecItem( index );
	if( !pProp )
		return FALSE;

	return pProp->IsVis();
}

BOOL CWndBuffPetStatus::IsFreeSlot( const int index )
{
	FLItemElem* pItem = g_pPlayer->GetVisPetItem();
	if( !pItem )
		return FALSE;

	int nAvailableSlot = pItem->GetGeneralPiercingSize( );
	return ( index < nAvailableSlot );
}

void CWndBuffPetStatus::OnMouseWndSurface( CPoint point )
{
	CRect rect;
	LPWNDCTRL wndCtrl = NULL;
	FLItemElem* pItem = g_pPlayer->GetVisPetItem( );
	FLASSERT( pItem );
	if( pItem->GetGeneralPiercingSize( ) == 0 )
		return;

	for(int i=0; i<MAX_VIS; i++)
	{
		wndCtrl = GetWndCtrl( m_nCtrlId[i] );
		rect = wndCtrl->rect;
		if( rect.PtInRect( point ) )
		{
			ClientToScreen( &point );
			ClientToScreen( &rect );

			DWORD dwIndex = pItem->GetGeneralPiercingItemID( i );
			if(  0 != dwIndex )	
			{
				if( NULL == m_pItemElem[ i ] )
					m_pItemElem[ i ] = new FLItemElem;
			
				m_pItemElem[ i ]->m_dwItemId = dwIndex;

				time_t endTime = pItem->GetVisPetPiercingItemKeepTime( i );
				//DWORD remainTime = endTime - timeGetTime( );
				m_pItemElem[ i ]->m_dwKeepTime = endTime;
			
			}else
			{
				SAFE_DELETE( m_pItemElem[ i ] );
				continue;
			}

			g_WndMng.PutToolTip_Item( (FLItemBase*)m_pItemElem[i], point, &rect );
		}

	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//CWndConfirmVis : 비스 꼽고 뺄때 확인창 
//////////////////////////////////////////////////////////////////////////////////////////////////
CWndConfirmVis::CWndConfirmVis() 
{ 
 	m_dwItemId = NULL_ID;
 	m_objid = NULL_ID;
 	m_nSlot = 0;
	m_dwItemIndex = 0;
	m_eSection = CVS_EQUIP_VIS;
} 

CWndConfirmVis::~CWndConfirmVis() 
{ 
	
} 


void CWndConfirmVis::OnDraw( C2DRender* p2DRender ) 
{ 
	
} 

void CWndConfirmVis::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 

	CWndButton* pOk( (CWndButton*)GetDlgItem( WIDC_BUTTON1 ) );
	if( pOk )
	{
		pOk->SetDefault( TRUE );

		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
			pOk->SetTitle( prj.GetText( TID_MMI_UIBUTTON_BUTTON02 ) );
	}

	CWndButton* pCancel( (CWndButton*)GetDlgItem( WIDC_BUTTON2 ) );
	if( pCancel )
	{
		if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
			pCancel->SetTitle( prj.GetText( TID_MMI_UIBUTTON_BUTTON03 ) );
	}

	AddWndStyle( WBS_MODAL );
	AddWndStyle( WBS_TOPMOST );


	if( ETC_GAMEGUARD_ERR == m_eSection )
	{
		pCancel->EnableWindow( FALSE );

		CWndButton* pBtn = m_wndTitleBar.GetTitleBarButton( WTBID_CLOSE );
		if( pBtn )
			pBtn->SetVisible( FALSE );
	}

	PT_ITEM_SPEC pProp = NULL;

	if( ETC_GAMEGUARD_ERR != m_eSection )
	{
		pProp = g_xSpecManager->GetSpecItem( m_dwItemIndex );
		FLASSERT( pProp );

		if( !pProp )
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "SkillProp is NULL, section %d" ), m_eSection );
			return;
		}
	}

	CWndText* pText = ( CWndText* )GetDlgItem( WIDC_TEXT1 );
	CString strTitle;
	switch( m_eSection )
	{
	case CVS_EQUIP_VIS : strTitle.Format( GETTEXT( TID_GAME_BUFFPET_EQUIP ), pProp->szName ); break;
	case CVS_UNEQUIP_VIS : strTitle.Format( GETTEXT( TID_GAME_BUFFPET_CANCEL ), pProp->szName ); break;
	case CVS_EQUIP_VISKEY : // 비스 슬롯 확장 키 쓸려고 할때 
		{
			MoverProp* pProp = NULL;
			if( g_pPlayer )
			{
				CMover* pPet = prj.GetMover( g_pPlayer->GetEatPetId( ) );
				if( pPet )
					pProp = pPet->GetProp( );
			}

			if( pProp )
				strTitle.Format( GETTEXT( TID_GAME_BUFFPET_KET01 ), pProp->szName );
		}break;

	case CVS_PICKUP_TO_BUFF:
		strTitle = GETTEXT( TID_GAME_PET_TRAN );	// 소환되어 있는 픽업펫 버프펫으로 변환?
		break;

	case ETC_PROTECT_AWAKE:
		strTitle = GETTEXT( TID_GAME_REGARDLESS_USE01 ); //"진짜러 각성보호 쓸래염? 의 아이디 필요";
		break;

	case ETC_GAMEGUARD_ERR:
		{
			CRect rect = this->GetWindowRect();
			this->Move( rect.left, rect.top - 200 );	//TOPMOST가 겹치는경우라 위로 올림.
			strTitle = "Client is not installed GAMEGUARD,             Did you terminate?";
		}
		break;

	case ETC_BARUNASMELT_NOTICE:
		strTitle = GETTEXT( TID_MMI_NEWSMELT_NEWSMELT07);
		break;

	case ETC_CASHITEM:
		strTitle = m_strTitleString;
		break;

	case ETC_BARUNAELEMENT_SMELT_NOTICE:
		strTitle = GETTEXT( TID_MMI_BARUNA_ELESMELT_Q_TEXT02 );
	}

	pText->SetString( strTitle );

	CWndEdit* pEdit( (CWndEdit*)GetDlgItem( WIDC_EDIT1 ) );
	if( pEdit )
	{
		//에디트창을 안보이는곳으로 보내버리고 ENTER받을준비 
		pEdit->Move( -100, -100 );
		pEdit->SetFocus( );
	}
} 

BOOL CWndConfirmVis::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	InitDialog( g_Neuz.GetSafeHwnd(), APP_CONFIRM_ENTER, WBS_KEY, 0, pWndParent );
	MoveParentCenter();
	return TRUE;
}
BOOL CWndConfirmVis::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 

void CWndConfirmVis::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndConfirmVis::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 

void CWndConfirmVis::SendEquipPacket( )
{
	switch( m_eSection )
	{
	case CVS_EQUIP_VIS:			g_DPlay.SendDoUseItem( MAKELONG( ITYPE_ITEM, m_dwItemId ), m_objid ); break;
	case CVS_UNEQUIP_VIS :		g_DPlay.SendRemoveVis( m_nSlot );	break;
	case CVS_EQUIP_VISKEY :		g_DPlay.SendDoUseItem( MAKELONG( ITYPE_ITEM, m_dwItemId ), m_objid ); break;
	case CVS_PICKUP_TO_BUFF :	g_DPlay.SendDoUseItem( MAKELONG( ITYPE_ITEM, m_dwItemId ), m_objid ); break;

	case ETC_PROTECT_AWAKE: g_DPlay.SendDoUseItem( MAKELONG( ITYPE_ITEM, m_dwItemId ), m_objid ); break;

	//게임가드 에러, 패킷송신 대신 클라를 죽인다.
	case ETC_GAMEGUARD_ERR :	::PostMessage( g_Neuz.GetSafeHwnd(), WM_CLOSE, 0, 0 );	break;

#ifdef __ENCHANT_BARUNA16
	case ETC_BARUNASMELT_NOTICE:
		((CWndEnchantBaruna*)m_pParentWnd)->SendPacket();
		break;
#endif // __ENCHANT_BARUNA16

	case ETC_CASHITEM :			g_DPlay.SendDoUseItem( MAKELONG( ITYPE_ITEM, m_dwItemId ), m_objid ); break;
	case ETC_BARUNAELEMENT_SMELT_NOTICE: ((CFLWndBaruna_Element_Smelt*)m_pParentWnd)->SetGauageBarState(TRUE); break;
	default : 
		FLASSERT( 0 ); break;
	}
}

BOOL CWndConfirmVis::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	// 엔터 입력 처리 
	if( WIDC_EDIT1 == nID && EN_RETURN == message )
	{
		SendEquipPacket( );	
		Destroy();
		return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
	}

	if( message == WNM_CLICKED  )
	{
		switch(nID)
		{
			case WIDC_BUTTON2:   
				Destroy(); 
				break;
			case WIDC_BUTTON1:       
				SendEquipPacket( );
				Destroy();
				break;
		}
	}
		
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 
