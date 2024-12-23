

#include "StdAfx.h"


#define	MAX_SLIDER		10


//---------------------------------------------------------------------------------------------
// 생성자
//---------------------------------------------------------------------------------------------
CWndTotalOption::CWndTotalOption() 
:	m_bRollBack( true )
{ 
	SaveOriginalData();
} 


//---------------------------------------------------------------------------------------------
// 파괴자
//---------------------------------------------------------------------------------------------
CWndTotalOption::~CWndTotalOption() 
{ 
	LoadOriginalData();
} 


//---------------------------------------------------------------------------------------------
// 초기화
// param	: 부모 윈도우, 생성 윈도우 아이디
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndTotalOption::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), dwWndId, 0, CPoint( 0, 0 ), pWndParent );
} 


//---------------------------------------------------------------------------------------------
// 기존 데이터들을 저정한다.
// param	: void
// return	: void
//---------------------------------------------------------------------------------------------
void CWndTotalOption::SaveOriginalData()
{
	m_kOrgOption = g_Option;
}


//---------------------------------------------------------------------------------------------
// 기존 데이터들을 불러온다.
// param	: void
// return	: void
//---------------------------------------------------------------------------------------------
void CWndTotalOption::LoadOriginalData()
{
	if( CS_VER2 == _GetContentState( CT_OPTION_17 ) )
	{
		if( m_bRollBack )
		{
			g_Option = m_kOrgOption;

			// 지형 표현
			SetLODDetail( g_Option.m_nViewArea );
			
			// 감마, 밝기, 명암
			g_Neuz.Drv_SetGamma( g_Neuz.GetSafeHwnd(), g_Option.m_fGamma, g_Option.m_nOverBright, g_Option.m_fContrast );

			// 마스크 보기
			g_DPlay.SendOptionEnableRenderMask( g_Option.m_bViewMask );

			CWndBase::m_nAlpha			= g_Option.m_nWindowAlpha;
			g_SoundMng.m_nSoundVolume	= (int)( -(1.0f-g_Option.m_fEffectVolume) * 5000 );

			SetVolume(g_Option.m_fBGMVolume);
		}
	}
}

void CWndTotalOption::UpdateDataAll()
{
	//gmpbigsun: 17차 옵션창 기능 추가
	m_OptTabVideoSnd.UpdateData( );
	m_OptTabGame.UpdateData( );
}

void CWndTotalOption::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 

	//	여기에 코딩하면 됩니다
	CWndTabCtrl* pWndTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );

	if( CS_VER2 == _GetContentState( CT_NEWUI_19 ) )
		pWndTabCtrl->SetTabTitleAlign( CWndTabCtrl::ALIGN_CENTER );

	CRect rect = GetClientRect();
	rect.left = 5;
	rect.top = 0;
	
//sun: 12, 옵션창 개선
	m_OptTabVideoSnd.Create( WBS_CHILD | WBS_NODRAWFRAME, rect, pWndTabCtrl, APP_OPTEX_AV12);
	m_OptTabGame.Create( WBS_CHILD | WBS_NODRAWFRAME, rect, pWndTabCtrl, APP_OPTEX_GAME12);

	WTCITEM tabTabItem;
	tabTabItem.mask = WTCIF_TEXT | WTCIF_PARAM;

//sun: 12, 옵션창 개선
	tabTabItem.pszText = prj.GetText(TID_TOOLTIP_OPT_VIDEOAUDIO);
	//tabTabItem.pszText = prj.GetText(TID_APP_OPTION_VIDEO);
	tabTabItem.pWndBase = &m_OptTabVideoSnd;
	pWndTabCtrl->InsertItem( 0, &tabTabItem );

	tabTabItem.pszText = prj.GetText(TID_TOOLTIP_OPT_GAME);
	//tabTabItem.pszText = prj.GetText(TID_APP_OPTION_GAME);
	tabTabItem.pWndBase = &m_OptTabGame;
	pWndTabCtrl->InsertItem( 1, &tabTabItem );

	MoveParentCenter();	
	 
} 

void CWndTotalOption::ResetInfoPang( )
{
	CWndGuideSystem* pWndGuide = (CWndGuideSystem*)GetWndBase( APP_GUIDE );

	if( pWndGuide )
	{
		pWndGuide->m_dwGuideLevel = *(g_Option.m_pGuide);
		
		if( pWndGuide->m_dwGuideLevel == 0 )
		{
			if( g_pPlayer )
			{
				if(pWndGuide->m_bVisible != false)
				{
					pWndGuide->m_bVisible = true;
					pWndGuide->ChangeModel( g_pPlayer->GetJob() );
					pWndGuide->SetAni( g_pPlayer->GetJob(), CWndGuideSystem::ANI_IDLE );
				}
			}
		}
		else
		{
			if( g_pPlayer )
			{
				pWndGuide->SetAni( g_pPlayer->GetJob(), CWndGuideSystem::ANI_BYTE );
			}
		}
	}
}

BOOL CWndTotalOption::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	switch( nID )
	{
		case 10000:
			ResetInfoPang( );
			break;

		case WIDC_BUT_OK:
			ResetInfoPang( );
			m_bRollBack = false;
			Destroy();
			break;

		case WIDC_BUT_CANCEL:
			Destroy();
			break;

		case WIDC_BUT_RESET:
			g_Option.Load( "neuzDefault.ini" );
			UpdateDataAll( );
			break;
	}
		
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 


















CWndOption::CWndOption() 
{ 
} 
CWndOption::~CWndOption() 
{ 
} 
void CWndOption::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndOption::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 

	CWndButton* pWndAlpha = (CWndButton*)GetDlgItem( WIDC_CHECK1 );
	pWndAlpha->SetCheck( g_Option.m_nWindowAlpha == 255 ? FALSE : TRUE);
	CWndButton* pWndHelp = (CWndButton*)GetDlgItem( WIDC_CHECK2 );
	pWndHelp->SetCheck( g_Option.m_nInstantHelp );
	
	CWndButton* pWndButton[ 4 ];

	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_RADIO1 );
	pWndButton[ 1 ] = (CWndButton*)GetDlgItem( WIDC_RADIO2 );
	pWndButton[ 0 ]->SetGroup( TRUE );
	pWndButton[ g_Option.m_nSlangWord ]->SetCheck( TRUE );

	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_RADIO3 );
	pWndButton[ 1 ] = (CWndButton*)GetDlgItem( WIDC_RADIO4 );
	pWndButton[ 2 ] = (CWndButton*)GetDlgItem( WIDC_RADIO5 );
	pWndButton[ 3 ] = (CWndButton*)GetDlgItem( WIDC_RADIO6 );
	pWndButton[ 0 ]->SetGroup( TRUE );
	pWndButton[ g_Option.m_nChatCommand ]->SetCheck( TRUE );

	// 인터페이스 방식(구버전이 디폴트)
	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_RADIO7 );
	pWndButton[ 1 ] = (CWndButton*)GetDlgItem( WIDC_RADIO8 );
#ifdef __Y_INTERFACE_VER3
	pWndButton[ 2 ] = (CWndButton*)GetDlgItem( WIDC_RADIO12 );
#endif //__Y_INTERFACE_VER3
	pWndButton[ 0 ]->SetGroup( TRUE );
	pWndButton[ g_Option.m_nInterface ]->SetCheck( TRUE );

	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_RADIO9 );
	pWndButton[ 1 ] = (CWndButton*)GetDlgItem( WIDC_RADIO10 );
	pWndButton[ 2 ] = (CWndButton*)GetDlgItem( WIDC_RADIO11 );
	pWndButton[ 0 ]->SetGroup( TRUE );
	pWndButton[ *g_Option.m_pGuide ]->SetCheck( TRUE );

	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_CHECK4 );
	if(pWndButton[ 0 ])
	pWndButton[ 0 ]->SetCheck( g_Option.m_bCameraLock );

	// 비행시 롤링 효과.
	CWndButton* pWndRoll = (CWndButton*)GetDlgItem( WIDC_CHECK3 );		
	pWndRoll->SetCheck( g_Option.m_bRollEffect );

	//sun: 11, 줌옵션 추가
	CWndButton* pWndZoom = (CWndButton*)GetDlgItem( WIDC_CHECK5 );		
	pWndZoom->SetCheck(!g_Option.m_bZoomLimit);
	
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// 처음 이 함수를 부르면 윈도가 열린다.
BOOL CWndOption::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy에서 설정한 리소스로 윈도를 연다.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_OPTION_ETC, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  직접 윈도를 열때 사용 
BOOL CWndOption::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndOption::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndOption::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndOption::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndOption::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndOption::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	CWndButton* pWndAlpha = (CWndButton*)GetDlgItem( WIDC_CHECK1 );
	CWndButton* pWndHelp = (CWndButton*)GetDlgItem( WIDC_CHECK2 );
	CWndButton* pWndRoll = (CWndButton*)GetDlgItem( WIDC_CHECK3 );
	CWndButton* pWndCamearaLock = (CWndButton*)GetDlgItem( WIDC_CHECK4 );
	CWndButton* pWndZoomLimit   = (CWndButton*)GetDlgItem( WIDC_CHECK5 );		//sun: 11, 줌옵션 추가

	switch( nID )
	{
	case WIDC_CHECK1: // 윈도 반투명 옵션 
		if( pWndAlpha->GetCheck() )
			CWndBase::m_nAlpha = g_Option.m_nWindowAlpha = 128;
		else
			CWndBase::m_nAlpha = g_Option.m_nWindowAlpha = 255;
		break;
	case WIDC_CHECK2: // 초보자 도움말 옵션 
		if( pWndHelp->GetCheck() )
			g_Option.m_nInstantHelp = TRUE;
		else
			g_Option.m_nInstantHelp = FALSE;
		break;
	case WIDC_CHECK3:	// 비행시 롤링 효과.
		if( pWndRoll->GetCheck() )
			g_Option.m_bRollEffect = TRUE;
		else
			g_Option.m_bRollEffect = FALSE;
		break;
	case WIDC_CHECK4:
		{
			if( pWndCamearaLock->GetCheck() )
				g_Option.m_bCameraLock = TRUE;
			else
				g_Option.m_bCameraLock = FALSE;			
		}
		break;
//sun: 11, 줌옵션 추가
	case WIDC_CHECK5:
		{
			if( pWndZoomLimit->GetCheck() )
				g_Option.m_bZoomLimit = FALSE;
			else
				g_Option.m_bZoomLimit = TRUE;			
		}	
		break;

	case WIDC_RADIO1:
		g_Option.m_nSlangWord = 0;
		break;
	case WIDC_RADIO2:
		g_Option.m_nSlangWord = 1;
		break;
	case WIDC_RADIO3:
		g_Option.m_nChatCommand = 0;
		break;
	case WIDC_RADIO4:
		g_Option.m_nChatCommand = 1;
		break;
	case WIDC_RADIO5:
		g_Option.m_nChatCommand = 2;
		break;
	case WIDC_RADIO6:
		g_Option.m_nChatCommand = 3;
		break;
	case WIDC_RADIO7:	// 휠로 카메라 / 우클릭 스킬의 고전방식
		g_Option.m_nInterface = 0;
		break;
	case WIDC_RADIO8:	// 우클릭 카메라 / 좌클릭+X 스킬의 신방식.
		g_Option.m_nInterface = 1;
		break;
#ifdef __Y_INTERFACE_VER3
	case WIDC_RADIO12:
		g_Option.m_nInterface = 2;
		break;
#endif //__Y_INTERFACE_VER3
	case WIDC_RADIO9:
		*g_Option.m_pGuide = 0;
		break;
	case WIDC_RADIO10:
		*g_Option.m_pGuide = 1;
		break;
	case WIDC_RADIO11:
		*g_Option.m_pGuide = 2;
		break;
	case 10000:
		{
			CWndGuideSystem* pWndGuide = (CWndGuideSystem*)GetWndBase( APP_GUIDE );
			
			if( pWndGuide )
			{
				pWndGuide->m_dwGuideLevel = *(g_Option.m_pGuide);
				
				if( pWndGuide->m_dwGuideLevel == 0 )
				{
					if( g_pPlayer )
					{
						pWndGuide->m_bVisible = TRUE;
						pWndGuide->ChangeModel( g_pPlayer->GetJob() );
						pWndGuide->SetAni( g_pPlayer->GetJob(), CWndGuideSystem::ANI_IDLE );
					}
				}
				else
				{
					if(g_pPlayer)
					{
						pWndGuide->SetAni( g_pPlayer->GetJob(), CWndGuideSystem::ANI_BYTE );
					}
				}
			}
		}
		break;
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

CWndOptSound::CWndOptSound() 
{ 
	m_bLButtonClick = FALSE;
	m_bLButtonClick2 = FALSE;
	
	m_nStep[0] = 0;
	m_nStep[1] = 0;
} 
CWndOptSound::~CWndOptSound() 
{ 
} 
void CWndOptSound::OnDraw( C2DRender* p2DRender ) 
{ 
	CPoint pt;

	LPWNDCTRL lpWndCtrl = GetWndCtrl( WIDC_CUSTOM1 );
	CPoint pt1 = lpWndCtrl->rect.TopLeft();
	CPoint pt2 = lpWndCtrl->rect.BottomRight() - lpWndCtrl->rect.TopLeft();
	p2DRender->RenderFillRect( lpWndCtrl->rect, 0);	
	m_Texture.Render( p2DRender, pt1, pt2 );
	pt = pt1;
	pt.x += GetStepPos( m_nStep[0], lpWndCtrl->rect.right - lpWndCtrl->rect.left, 10 ).x;
	m_TexturePt.Render( p2DRender, pt, CPoint( 16, 16 ) );
	
	lpWndCtrl = GetWndCtrl( WIDC_CUSTOM2 );
	pt1 = lpWndCtrl->rect.TopLeft();
	pt2 = lpWndCtrl->rect.BottomRight() - lpWndCtrl->rect.TopLeft();
	m_Texture.Render( p2DRender, pt1, pt2 );
	pt = pt1;
	pt.x += GetStepPos( m_nStep[1], lpWndCtrl->rect.right - lpWndCtrl->rect.left, 10 ).x;
	m_TexturePt.Render( p2DRender, pt, CPoint( 16, 16 ) );

} 
void CWndOptSound::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();

	CWndButton* pWndButton[ 3 ];

	pWndButton[ 1 ] = (CWndButton*)GetDlgItem( WIDC_VOICEON );
	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_VOICEOFF );
	if( g_xFlyffConfig->GetMainLanguage() == LANG_JAP )
	{
		
		pWndButton[ 1 ]->SetGroup( TRUE );
		pWndButton[ g_Option.m_bVoice ]->SetCheck( TRUE );
	}
	else
	{
		CWndStatic* pWndVoice = (CWndStatic*)GetDlgItem(WIDC_STATIC2);
		pWndVoice->SetVisible(FALSE);
		pWndButton[ 1 ]->SetVisible(FALSE);
		pWndButton[ 0 ]->SetVisible(FALSE);
	}
	m_Texture.LoadTexture( g_Neuz.GetDevice(), MakePath( DIR_THEME,g_xFlyffConfig->GetMainLanguage(), "WndVolumeBar.tga" ), 0xffff00ff, TRUE );
	m_TexturePt.LoadTexture( g_Neuz.GetDevice(), MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(),"ButtSpin.tga" ), 0xffffffff, TRUE );		
	m_nStep[0] = (int)( g_Option.m_fEffectVolume * 10 );
	m_nStep[1] = (int)( g_Option.m_fBGMVolume * 10 );

} 
// 처음 이 함수를 부르면 윈도가 열린다.
BOOL CWndOptSound::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy에서 설정한 리소스로 윈도를 연다.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_OPTEX_SOUND, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  직접 윈도를 열때 사용 
BOOL CWndOptSound::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndOptSound::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndOptSound::OnSize( UINT nType, int cx, int cy )
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndOptSound::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
	PLAYSND(SND_NONE);
	ReleaseCapture();
	m_bLButtonClick = FALSE;
	m_bLButtonClick2 = FALSE;
} 
void CWndOptSound::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
	SetCapture();

	int nResult = 0;
	nResult = GetSliderStep( WIDC_CUSTOM1, m_nStep[0], point );	
	if( nResult == -1 )
	{
		m_bLButtonClick = TRUE;
		return;
	}
	else
	if( nResult == 1 )
	{
		g_Option.m_fEffectVolume	= m_nStep[0]*0.1f;
		g_SoundMng.m_nSoundVolume	= (int)( -(1.0f-g_Option.m_fEffectVolume)*5000 );
	}
	/////////////////////////////////////////
	nResult = GetSliderStep( WIDC_CUSTOM2, m_nStep[1], point );	
	if( nResult == -1 )
	{
		m_bLButtonClick2 = TRUE;
		return;
	}
	else
	if( nResult == 1 )
	{
		g_Option.m_fBGMVolume = m_nStep[1]*0.1f;
		SetVolume(g_Option.m_fBGMVolume);		
	}

} 

void CWndOptSound::OnMouseMove(UINT nFlags, CPoint point)
{
	if( m_bLButtonClick )
	{
		GetRangeSlider( WIDC_CUSTOM1, m_nStep[0], point );

		g_Option.m_fEffectVolume	= m_nStep[0]*0.1f;
		g_SoundMng.m_nSoundVolume	= (int)( -(1.0f-g_Option.m_fEffectVolume)*5000 );
	}

	if( m_bLButtonClick2 )
	{
		GetRangeSlider( WIDC_CUSTOM2, m_nStep[1], point );

		g_Option.m_fBGMVolume = m_nStep[1]*0.1f;
		SetVolume(g_Option.m_fBGMVolume);
	}
}
BOOL CWndOptSound::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	switch(nID)
	{
		case WIDC_VOICEON:
			g_Option.m_bVoice = 1;
			break;
		case WIDC_VOICEOFF:
			g_Option.m_bVoice = 0;
			break;
	};
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

CPoint CWndOptSound::GetStepPos(int nStep, int nWidth, int nDivision)
{
	return nStep * (nWidth / (nDivision+1));
}

int CWndOptSound::GetSliderStep(DWORD dwWndId, int &nStep, CPoint point )
{
	LPWNDCTRL lpWndCtrl = GetWndCtrl( dwWndId );	
	CRect crect = lpWndCtrl->rect;
	
	crect.left += GetStepPos( nStep, lpWndCtrl->rect.right - lpWndCtrl->rect.left, MAX_SLIDER ).x;
	crect.right = crect.left+16;
	
	if( PtInRect( crect, point ) )
	{
		return -1;
	}
	
	crect = lpWndCtrl->rect;
	if( PtInRect( crect, point ) )
	{
		int nWidth = lpWndCtrl->rect.right - lpWndCtrl->rect.left;
		
		if( point.x > lpWndCtrl->rect.left+GetStepPos( nStep, nWidth, MAX_SLIDER ).x )
			nStep += 1;//nWidth / 10;
		else
			nStep -= 1;//nWidth / 10;

		return 1;
	}

	return 0;
}

void CWndOptSound::GetRangeSlider(DWORD dwWndId, int &nStep, CPoint point)
{
	LPWNDCTRL lpWndCtrl = GetWndCtrl( dwWndId );	
	int nWidth = lpWndCtrl->rect.right - lpWndCtrl->rect.left;
	
	if( point.x > lpWndCtrl->rect.left+GetStepPos( nStep, nWidth, MAX_SLIDER ).x )
		nStep += 1;//nWidth / 10;
	if( point.x < lpWndCtrl->rect.left+GetStepPos( nStep, nWidth, MAX_SLIDER ).x )
		nStep -= 1;//nWidth / 10;
	
	if( nStep < 0 )
		nStep = 0;
	
	if( nStep > MAX_SLIDER )
		nStep = MAX_SLIDER;
}

HRESULT CWndOptSound::InvalidateDeviceObjects()
{
	return CWndNeuz::InvalidateDeviceObjects();
}
HRESULT CWndOptSound::RestoreDeviceObjects()
{
	return CWndNeuz::RestoreDeviceObjects();
}


CWndOptVideo::CWndOptVideo() 
{ 
	m_bLButtonClick = FALSE;
	m_bLButtonClick2 = FALSE;
	
	m_nStep[0] = 0;
	m_nStep[1] = 0;

	//sun: 밝기, 감마, 명암 조절 Neuz
	m_nStep[2] = 0;
	m_nStep[3] = 0;
	m_nStep[4] = 0;

	m_bLButtonClick3 = FALSE;
	m_bLButtonClick4 = FALSE;
	m_bLButtonClick5 = FALSE;

	memset( m_nBrightTable, 0, sizeof(m_nBrightTable) );
} 
CWndOptVideo::~CWndOptVideo() 
{ 
	g_Option.Save( "neuz.ini" );	
} 
HRESULT CWndOptVideo::InvalidateDeviceObjects()
{
	return CWndNeuz::InvalidateDeviceObjects();
}
HRESULT CWndOptVideo::RestoreDeviceObjects()
{
	return CWndNeuz::RestoreDeviceObjects();
}

CPoint CWndOptVideo::GetStepPos(int nStep, int nWidth, int nDivision)
{
	if(nDivision == 1 && nStep)
		return nStep * nWidth - 16;
	else
		return nStep * (nWidth / (nDivision+1));
}

void CWndOptVideo::OnDraw( C2DRender* p2DRender ) 
{ 
	CPoint pt;
#ifdef __SFX_OPT
	LPWNDCTRL lpWndCtrl = GetWndCtrl( WIDC_CUSTOM2 );
	CPoint pt1 = lpWndCtrl->rect.TopLeft();
	CPoint pt2 = lpWndCtrl->rect.BottomRight() - lpWndCtrl->rect.TopLeft();
	p2DRender->RenderFillRect( lpWndCtrl->rect, 0);	
	m_Texture.Render( p2DRender, pt1, pt2 );
	pt = pt1;
	pt.x += GetStepPos( m_nStep[0], lpWndCtrl->rect.right - lpWndCtrl->rect.left, 5 ).x;
	m_TexturePt.Render( p2DRender, pt, CPoint( 16, 16 ) );
	
	lpWndCtrl = GetWndCtrl( WIDC_CUSTOM3 );
	pt1 = lpWndCtrl->rect.TopLeft();
	pt2 = lpWndCtrl->rect.BottomRight() - lpWndCtrl->rect.TopLeft();
	m_Texture.Render( p2DRender, pt1, pt2 );
	pt = pt1;
	pt.x += GetStepPos( m_nStep[1], lpWndCtrl->rect.right - lpWndCtrl->rect.left, 1 ).x;
	m_TexturePt.Render( p2DRender, pt, CPoint( 16, 16 ) );
#else
	LPWNDCTRL lpWndCtrl = GetWndCtrl( WIDC_CUSTOM2 );
	CPoint pt1 = lpWndCtrl->rect.TopLeft();
	CPoint pt2 = lpWndCtrl->rect.BottomRight() - lpWndCtrl->rect.TopLeft();
	p2DRender->RenderFillRect( lpWndCtrl->rect, 0);	
	m_Texture.Render( p2DRender, pt1, pt2 );
	pt = pt1;
	pt.x += GetStepPos( m_nStep[0], lpWndCtrl->rect.right - lpWndCtrl->rect.left, 10 ).x;
	m_TexturePt.Render( p2DRender, pt, CPoint( 16, 16 ) );
	
	lpWndCtrl = GetWndCtrl( WIDC_CUSTOM3 );
	pt1 = lpWndCtrl->rect.TopLeft();
	pt2 = lpWndCtrl->rect.BottomRight() - lpWndCtrl->rect.TopLeft();
	m_Texture.Render( p2DRender, pt1, pt2 );
	pt = pt1;
	pt.x += GetStepPos( m_nStep[1], lpWndCtrl->rect.right - lpWndCtrl->rect.left, 10 ).x;
	m_TexturePt.Render( p2DRender, pt, CPoint( 16, 16 ) );
#endif
	
	//gmpbigsun( 20110322 ), #0009447 : 윈도모드에서 감마, 밝기, 명암 조절버튼 no rendering
	lpWndCtrl = GetWndCtrl( WIDC_CUSTOM1 );
	pt1 = lpWndCtrl->rect.TopLeft();
	pt2 = lpWndCtrl->rect.BottomRight() - lpWndCtrl->rect.TopLeft();
	m_Texture.Render( p2DRender, pt1, pt2 );
	
	if( g_xFlyffConfig->GetMainLanguage() != LANG_KOR )
	{
		if( g_Option.IsStartFullScreenMode() )
		{
			pt = pt1;
			pt.x += GetStepPos( m_nStep[2], lpWndCtrl->rect.right - lpWndCtrl->rect.left, 10 ).x;
			m_TexturePt.Render( p2DRender, pt, CPoint( 16, 16 ) );
		}
	}
	else
	{
		pt = pt1;
		pt.x += GetStepPos( m_nStep[2], lpWndCtrl->rect.right - lpWndCtrl->rect.left, 10 ).x;
		m_TexturePt.Render( p2DRender, pt, CPoint( 16, 16 ) );
	}

	lpWndCtrl = GetWndCtrl( WIDC_CUSTOM4 );
	pt1 = lpWndCtrl->rect.TopLeft();
	pt2 = lpWndCtrl->rect.BottomRight() - lpWndCtrl->rect.TopLeft();
	m_Texture.Render( p2DRender, pt1, pt2 );
	
	if( g_xFlyffConfig->GetMainLanguage() != LANG_KOR )
	{
		if( g_Option.IsStartFullScreenMode() )
		{
			pt = pt1;
			pt.x += GetStepPos( m_nStep[3], lpWndCtrl->rect.right - lpWndCtrl->rect.left, 10 ).x;
			m_TexturePt.Render( p2DRender, pt, CPoint( 16, 16 ) );
		}
	}
	else
	{
		pt = pt1;
		pt.x += GetStepPos( m_nStep[3], lpWndCtrl->rect.right - lpWndCtrl->rect.left, 10 ).x;
		m_TexturePt.Render( p2DRender, pt, CPoint( 16, 16 ) );
	}

	lpWndCtrl = GetWndCtrl( WIDC_CUSTOM5 );
	pt1 = lpWndCtrl->rect.TopLeft();
	pt2 = lpWndCtrl->rect.BottomRight() - lpWndCtrl->rect.TopLeft();
	m_Texture.Render( p2DRender, pt1, pt2 );
	pt = pt1;
	pt.x += GetStepPos( m_nStep[4], lpWndCtrl->rect.right - lpWndCtrl->rect.left, 10 ).x;
	
	if( g_xFlyffConfig->GetMainLanguage() != LANG_KOR )
	{
		if( g_Option.IsStartFullScreenMode() )
		{
			pt = pt1;
			pt.x += GetStepPos( m_nStep[4], lpWndCtrl->rect.right - lpWndCtrl->rect.left, 10 ).x;
			m_TexturePt.Render( p2DRender, pt, CPoint( 16, 16 ) );
		}
	}
	else
	{
		pt = pt1;
		pt.x += GetStepPos( m_nStep[4], lpWndCtrl->rect.right - lpWndCtrl->rect.left, 10 ).x;
		m_TexturePt.Render( p2DRender, pt, CPoint( 16, 16 ) );
	}

} 
void CWndOptVideo::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();

	CWndButton* pWndButton[ 3 ];

	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_OBJECT_LOD_HIGH );
	pWndButton[ 1 ] = (CWndButton*)GetDlgItem( WIDC_OBJECT_LOD_MID );
	pWndButton[ 2 ] = (CWndButton*)GetDlgItem( WIDC_OBJECT_LOD_LOW );
	pWndButton[ 0 ]->SetGroup( TRUE );
	pWndButton[ g_Option.m_nObjectDetail ]->SetCheck( TRUE );

	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_OBJECT_DISTANT_HIGH );
	pWndButton[ 1 ] = (CWndButton*)GetDlgItem( WIDC_OBJECT_DISTANT_MID );
	pWndButton[ 2 ] = (CWndButton*)GetDlgItem( WIDC_OBJECT_DISTANT_LOW );
	pWndButton[ 0 ]->SetGroup( TRUE );
	pWndButton[ g_Option.m_nObjectDistant ]->SetCheck( TRUE );

	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_TERRAIN_LOD_HIGH );
	pWndButton[ 1 ] = (CWndButton*)GetDlgItem( WIDC_TERRAIN_LOD_MID );
	pWndButton[ 2 ] = (CWndButton*)GetDlgItem( WIDC_TERRAIN_LOD_LOW );
	pWndButton[ 0 ]->SetGroup( TRUE );
	pWndButton[ g_Option.m_nViewArea ]->SetCheck( TRUE );

	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_SHADOW_HIGH );
	pWndButton[ 1 ] = (CWndButton*)GetDlgItem( WIDC_SHADOW_MID );
	pWndButton[ 2 ] = (CWndButton*)GetDlgItem( WIDC_SHADOW_LOW );
	pWndButton[ 0 ]->SetGroup( TRUE );
	pWndButton[ g_Option.m_nShadow ]->SetCheck( TRUE );
	if( g_Neuz.m_d3dCaps.MaxSimultaneousTextures <= 2 ) 
	{
		pWndButton[ 0 ]->EnableWindow( FALSE );
		pWndButton[ 1 ]->EnableWindow( FALSE );
		pWndButton[ 2 ]->EnableWindow( FALSE );
	}
#ifdef __SFX_OPT
	if( g_xFlyffConfig->GetMainLanguage() == LANG_KOR || g_xFlyffConfig->GetMainLanguage() == LANG_JAP)
	{
		pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_CHECK4 );
		if(pWndButton[ 0 ])
			pWndButton[ 0 ]->SetVisible(FALSE);
	}
#else
	pWndButton[ 1 ] = (CWndButton*)GetDlgItem( WIDC_VOICEON );
	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_VOICEOFF );
	pWndButton[ 1 ]->SetGroup( TRUE );
	pWndButton[ g_Option.m_bVoice ]->SetCheck( TRUE );

	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_GLOWON );
	pWndButton[ 1 ] = (CWndButton*)GetDlgItem( WIDC_GLOWOFF );
	pWndButton[ 0 ]->SetGroup( TRUE );
	if( g_Neuz.m_d3dCaps.PixelShaderVersion >= D3DPS_VERSION(1,1) )
	{
		pWndButton[ g_Option.m_nBloom^1 ]->SetCheck( TRUE );
	} else
	{
		pWndButton[0]->EnableWindow( FALSE );
		pWndButton[1]->EnableWindow( FALSE );
	}

	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_CHECK5 );
	pWndButton[ 0 ]->SetCheck(!g_Option.m_bSFXRenderOff);

#endif
#ifdef __YENV
	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_SEPCBUMPCHECK );

	if( g_bUsableVS && g_dwUsablePS_Level == 2 )
	{
		pWndButton[0]->EnableWindow( TRUE );
	}
	else
	{
		pWndButton[0]->EnableWindow( FALSE );
	}

	if( g_Option.m_bSpecBump )
	{
		pWndButton[0]->SetCheck( TRUE );
	}
	else
	{
		pWndButton[0]->SetCheck( FALSE );
	}
#endif //__YENV	
	
	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_CHECK6 );
	pWndButton[ 0 ]->SetCheck(g_Option.m_nPlayerName);
	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_CHECK7 );
	pWndButton[ 0 ]->SetCheck(g_Option.m_nOtherPlayerName);
	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_CHECK8 );
	pWndButton[ 0 ]->SetCheck(g_Option.m_nMonName);	
	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_CHECK2 );
	pWndButton[ 0 ]->SetCheck(g_Option.m_bDamageRender);
//sun: 12, 옵션창 개선
	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_CHECK1);
	pWndButton[ 0 ]->SetCheck(!g_Option.m_bZoomLimit);
	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_VIEWMASK );
	pWndButton[ 0 ]->SetCheck(g_Option.m_bViewMask);
	if(g_pPlayer && g_pPlayer->IsOptionRenderMask() != g_Option.m_bViewMask)
		g_DPlay.SendOptionEnableRenderMask( g_Option.m_bViewMask );
	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_CHECK5 );
	if(pWndButton[ 0 ])
	pWndButton[ 0 ]->SetCheck( g_Option.m_bCameraLock );

	m_Texture.LoadTexture( g_Neuz.GetDevice(), MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(),"WndVolumeBar.tga" ), 0xffff00ff, TRUE );
	m_TexturePt.LoadTexture( g_Neuz.GetDevice(), MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(),"ButtSpin.tga" ), 0xffffffff, TRUE );		
#ifdef __SFX_OPT
	m_nStep[0] = g_Option.m_nSfxLevel;
	m_nStep[1] = !g_Option.m_bSFXRenderOff;
#else
	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_MOUSE_HIGH );
	pWndButton[ 1 ] = (CWndButton*)GetDlgItem( WIDC_MOUSE_MID );
	pWndButton[ 2 ] = (CWndButton*)GetDlgItem( WIDC_MOUSE_LOW );
	pWndButton[ 0 ]->SetGroup( TRUE );
	pWndButton[ g_Option.m_MouseSpeed ]->SetCheck( TRUE );

	m_nStep[0] = g_Option.m_fEffectVolume * 10;
	m_nStep[1] = g_Option.m_fBGMVolume * 10;
#endif

	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_CHECK3 );
	if(pWndButton[ 0 ]) pWndButton[ 0 ]->SetCheck(g_Option.m_nWeatherEffect);

	//sun: 밝기, 감마, 명암 조절 Neuz
	m_nBrightTable[0] = -40;
	m_nBrightTable[1] = -30;
	m_nBrightTable[2] = -20;
	m_nBrightTable[3] = -10;
	m_nBrightTable[4] = 0;
	m_nBrightTable[5] = 10;
	m_nBrightTable[6] = 20;
	m_nBrightTable[7] = 30;
	m_nBrightTable[8] = 40;
	m_nBrightTable[9] = 50;
	m_nBrightTable[10] = 60;

	for( int i=0; i<11; i++ )
	{
		if( m_nBrightTable[i] == g_Option.m_nOverBright )
		{
			m_nStep[3] = i;
			break;
		}
	}

	m_nStep[2] = (int)( g_Option.m_fGamma * 5 );
	m_nStep[4] = (int)( g_Option.m_fContrast * 5 );

} 
// 처음 이 함수를 부르면 윈도가 열린다.
BOOL CWndOptVideo::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy에서 설정한 리소스로 윈도를 연다.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_OPTION_VIDEO, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  직접 윈도를 열때 사용 
BOOL CWndOptVideo::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndOptVideo::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndOptVideo::OnSize( UINT nType, int cx, int cy )
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndOptVideo::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
	PLAYSND(SND_NONE);
	ReleaseCapture();
	m_bLButtonClick = FALSE;
	m_bLButtonClick2 = FALSE;

	//sun: 밝기, 감마, 명암 조절 Neuz
	m_bLButtonClick3 = FALSE;
	m_bLButtonClick4 = FALSE;
	m_bLButtonClick5 = FALSE;
} 
void CWndOptVideo::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 

	SetCapture();

	int nResult = 0;
#ifdef __SFX_OPT
	nResult = GetSliderStep( WIDC_CUSTOM2, m_nStep[0], point, 5);	
	if( nResult == -1 )
	{
		m_bLButtonClick = TRUE;
		return;
	}
	else
	if( nResult == 1 )
	{
		g_Option.m_nSfxLevel = m_nStep[0];
	}
	/////////////////////////////////////////
	nResult = GetSliderStep( WIDC_CUSTOM3, m_nStep[1], point, 1);	
	if( nResult == -1 )
	{
		m_bLButtonClick2 = TRUE;
		return;
	}
	else
	if( nResult == 1 )
	{
		g_Option.m_bSFXRenderOff = !m_nStep[1];
	}
#else
	nResult = GetSliderStep( WIDC_CUSTOM2, m_nStep[0], point );	
	if( nResult == -1 )
	{
		m_bLButtonClick = TRUE;
		return;
	}
	else
	if( nResult == 1 )
	{
		g_Option.m_fEffectVolume = m_nStep[0]*0.1f;
		g_SoundMng.m_nSoundVolume = -(1.0f-g_Option.m_fEffectVolume)*5000;		
	}
	/////////////////////////////////////////
	nResult = GetSliderStep( WIDC_CUSTOM3, m_nStep[1], point );	
	if( nResult == -1 )
	{
		m_bLButtonClick2 = TRUE;
		return;
	}
	else
	if( nResult == 1 )
	{
		g_Option.m_fBGMVolume = m_nStep[1]*0.1f;
		SetVolume(g_Option.m_fBGMVolume);		
	}
#endif
	//sun: 밝기, 감마, 명암 조절 Neuz
	/////////////////////////////////////////
	
	if( g_xFlyffConfig->GetMainLanguage() != LANG_KOR )
	{
		if( g_Neuz.m_bStartFullscreen )
		{
			nResult = GetSliderStep( WIDC_CUSTOM1, m_nStep[2], point );	
			if( nResult == -1 )
			{
				m_bLButtonClick3 = TRUE;
				return;
			}
			else if( nResult == 1 )
			{
				g_Option.m_fGamma = (FLOAT)m_nStep[2] / (FLOAT)5.0f;
				g_Neuz.Drv_SetGamma( g_Neuz.GetSafeHwnd(), g_Option.m_fGamma, g_Option.m_nOverBright, g_Option.m_fContrast );
			}
			/////////////////////////////////////////
			nResult = GetSliderStep( WIDC_CUSTOM4, m_nStep[3], point );	
			if( nResult == -1 )
			{
					m_bLButtonClick4 = TRUE;
					return;
			}
			else if( nResult == 1 )
			{
				g_Option.m_nOverBright = m_nBrightTable[m_nStep[3]];
				g_Neuz.Drv_SetGamma( g_Neuz.GetSafeHwnd(), g_Option.m_fGamma, g_Option.m_nOverBright, g_Option.m_fContrast );
			}
			/////////////////////////////////////////
			nResult = GetSliderStep( WIDC_CUSTOM5, m_nStep[4], point );	
			if( nResult == -1 )
			{
				m_bLButtonClick5 = TRUE;
				return;
			}
			else if( nResult == 1 )
			{
				g_Option.m_fContrast = (FLOAT)m_nStep[4] / (FLOAT)5.0f;
				g_Neuz.Drv_SetGamma( g_Neuz.GetSafeHwnd(), g_Option.m_fGamma, g_Option.m_nOverBright, g_Option.m_fContrast );
			}
		}
	}
	else
	{
		nResult = GetSliderStep( WIDC_CUSTOM1, m_nStep[2], point );	
		if( nResult == -1 )
		{
			m_bLButtonClick3 = TRUE;
			return;
		}
		else if( nResult == 1 )
		{
			g_Option.m_fGamma = (FLOAT)m_nStep[2] / (FLOAT)5.0f;
			g_Neuz.Drv_SetGamma( g_Neuz.GetSafeHwnd(), g_Option.m_fGamma, g_Option.m_nOverBright, g_Option.m_fContrast );
		}
		/////////////////////////////////////////
		nResult = GetSliderStep( WIDC_CUSTOM4, m_nStep[3], point );	
		if( nResult == -1 )
		{
			m_bLButtonClick4 = TRUE;
			return;
		}
		else if( nResult == 1 )
		{
			g_Option.m_nOverBright = m_nBrightTable[m_nStep[3]];
			g_Neuz.Drv_SetGamma( g_Neuz.GetSafeHwnd(), g_Option.m_fGamma, g_Option.m_nOverBright, g_Option.m_fContrast );
		}
		/////////////////////////////////////////
		nResult = GetSliderStep( WIDC_CUSTOM5, m_nStep[4], point );	
		if( nResult == -1 )
		{
			m_bLButtonClick5 = TRUE;
			return;
		}
		else if( nResult == 1 )
		{
			g_Option.m_fContrast = (FLOAT)m_nStep[4] / (FLOAT)5.0f;
			g_Neuz.Drv_SetGamma( g_Neuz.GetSafeHwnd(), g_Option.m_fGamma, g_Option.m_nOverBright, g_Option.m_fContrast );
		}
	}
} 

void CWndOptVideo::OnMouseMove(UINT nFlags, CPoint point)
{
#ifdef __SFX_OPT
	if( m_bLButtonClick )
	{
		GetRangeSlider( WIDC_CUSTOM2, m_nStep[0], point, 5);
		g_Option.m_nSfxLevel = m_nStep[0];
	}

	if( m_bLButtonClick2 )
	{
		GetRangeSlider( WIDC_CUSTOM3, m_nStep[1], point,1);
		g_Option.m_bSFXRenderOff = !m_nStep[1];
	}
#else
	if( m_bLButtonClick )
	{
		GetRangeSlider( WIDC_CUSTOM2, m_nStep[0], point );

		g_Option.m_fEffectVolume = m_nStep[0]*0.1f;
		g_SoundMng.m_nSoundVolume = -(1.0f-g_Option.m_fEffectVolume)*5000;
	}

	if( m_bLButtonClick2 )
	{
		GetRangeSlider( WIDC_CUSTOM3, m_nStep[1], point );

		g_Option.m_fBGMVolume = m_nStep[1]*0.1f;
		SetVolume(g_Option.m_fBGMVolume);
	}
#endif

//sun: 밝기, 감마, 명암 조절 Neuz
	if( g_xFlyffConfig->GetMainLanguage() != LANG_KOR )
	{
		if(g_Neuz.m_bStartFullscreen)
		{
			if( m_bLButtonClick3 )
			{
				GetRangeSlider( WIDC_CUSTOM1, m_nStep[2], point );
				g_Option.m_fGamma = (FLOAT)m_nStep[2] / (FLOAT)5.0f;
				g_Neuz.Drv_SetGamma( g_Neuz.GetSafeHwnd(), g_Option.m_fGamma, g_Option.m_nOverBright, g_Option.m_fContrast );
			}
			if( m_bLButtonClick4 )
			{
				GetRangeSlider( WIDC_CUSTOM4, m_nStep[3], point );
				g_Option.m_nOverBright = m_nBrightTable[m_nStep[3]];
				g_Neuz.Drv_SetGamma( g_Neuz.GetSafeHwnd(), g_Option.m_fGamma, g_Option.m_nOverBright, g_Option.m_fContrast );
			}
			if( m_bLButtonClick5 )
			{
				GetRangeSlider( WIDC_CUSTOM5, m_nStep[4], point );
				g_Option.m_fContrast = (FLOAT)m_nStep[4] / (FLOAT)5.0f;
				g_Neuz.Drv_SetGamma( g_Neuz.GetSafeHwnd(), g_Option.m_fGamma, g_Option.m_nOverBright, g_Option.m_fContrast );
			}
		}
	}
	else
	{
		if( m_bLButtonClick3 )
		{
			GetRangeSlider( WIDC_CUSTOM1, m_nStep[2], point );
			g_Option.m_fGamma = (FLOAT)m_nStep[2] / (FLOAT)5.0f;
			g_Neuz.Drv_SetGamma( g_Neuz.GetSafeHwnd(), g_Option.m_fGamma, g_Option.m_nOverBright, g_Option.m_fContrast );
		}
		if( m_bLButtonClick4 )
		{
			GetRangeSlider( WIDC_CUSTOM4, m_nStep[3], point );
			g_Option.m_nOverBright = m_nBrightTable[m_nStep[3]];
			g_Neuz.Drv_SetGamma( g_Neuz.GetSafeHwnd(), g_Option.m_fGamma, g_Option.m_nOverBright, g_Option.m_fContrast );
		}
		if( m_bLButtonClick5 )
		{
			GetRangeSlider( WIDC_CUSTOM5, m_nStep[4], point );
			g_Option.m_fContrast = (FLOAT)m_nStep[4] / (FLOAT)5.0f;
			g_Neuz.Drv_SetGamma( g_Neuz.GetSafeHwnd(), g_Option.m_fGamma, g_Option.m_nOverBright, g_Option.m_fContrast );
		}
	}
}

void CWndOptVideo::OnMouseWndSurface( CPoint point )
{
}
BOOL CWndOptVideo::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	CWndButton* pWndButton		= (CWndButton*)*pLResult;
	CWndButton* pWndZoomLimit   = (CWndButton*)GetDlgItem( WIDC_CHECK1 );
	CWndButton* pWndCamearaLock = (CWndButton*)GetDlgItem( WIDC_CHECK5 );	//sun: 12, 옵션창 개선

	switch( nID )
	{
	case WIDC_OBJECT_LOD_HIGH:
		g_Option.m_nObjectDetail = 0;
		break;
	case WIDC_OBJECT_LOD_MID:
		g_Option.m_nObjectDetail = 1;
		break;
	case WIDC_OBJECT_LOD_LOW:
		g_Option.m_nObjectDetail = 2;
		break;

	case WIDC_OBJECT_DISTANT_HIGH:
		g_Option.m_nObjectDistant = 0;
		break;
	case WIDC_OBJECT_DISTANT_MID:
		g_Option.m_nObjectDistant = 1;
		break;
	case WIDC_OBJECT_DISTANT_LOW:
		g_Option.m_nObjectDistant = 2;
		break;

	case WIDC_TERRAIN_LOD_HIGH:
		g_Option.m_nViewArea = 0;
		SetLODDetail( g_Option.m_nViewArea );
		break;
	case WIDC_TERRAIN_LOD_MID:
		g_Option.m_nViewArea = 1;
		SetLODDetail( g_Option.m_nViewArea );
		break;
	case WIDC_TERRAIN_LOD_LOW:
		g_Option.m_nViewArea = 2;
		SetLODDetail( g_Option.m_nViewArea );
		break;

	case WIDC_SHADOW_HIGH:
		g_Option.m_nShadow = 0;
		CreateShadowMap( m_pApp->m_pd3dDevice, g_Neuz.m_d3dpp.BackBufferFormat );
		break;
	case WIDC_SHADOW_MID:
		g_Option.m_nShadow = 1;
		CreateShadowMap( m_pApp->m_pd3dDevice, g_Neuz.m_d3dpp.BackBufferFormat );
		break;
	case WIDC_SHADOW_LOW:
		g_Option.m_nShadow = 2;
		break;
#ifndef __SFX_OPT
	case WIDC_GLOWON:
		if( g_Neuz.m_d3dCaps.PixelShaderVersion >= D3DPS_VERSION(1,1) )
		{
			g_Option.m_nBloom = 1; 
			g_Glare.Create( D3DDEVICE, D3DFMT_R5G6B5, g_Option.m_nResWidth, g_Option.m_nResHeight - 48 );
		}
		break;
	case WIDC_GLOWOFF:
		g_Option.m_nBloom = 0;
		break;
	case WIDC_VOICEON:
		g_Option.m_bVoice = 1;
		break;
	case WIDC_VOICEOFF:
		g_Option.m_bVoice = 0;
		break;
	case WIDC_CHECK5:
		pWndButton = (CWndButton*)GetDlgItem( WIDC_CHECK5 );
		g_Option.m_bSFXRenderOff = !pWndButton->GetCheck();
		break;
	case WIDC_MOUSE_HIGH:
		g_Option.m_MouseSpeed = 0;
		break;
	case WIDC_MOUSE_MID:
		g_Option.m_MouseSpeed = 1;
		break;
	case WIDC_MOUSE_LOW:
		g_Option.m_MouseSpeed = 2;
		break;
#endif // !__SFX_OPT
#ifdef __YENV
	case WIDC_SEPCBUMPCHECK:
		// 설정값을 바로 적용하는것이 아니라 종료할때 적용한다
		// 옵션중 다시 시작해야 적용되는것들을 위한 변수..( 범프맵... )
		pWndButton = (CWndButton*)GetDlgItem( WIDC_SEPCBUMPCHECK );
		if( pWndButton->GetCheck() != g_Option.m_bSpecBump )
		{
			g_Option.m_bExitChange = TRUE;
			g_WndMng.OpenMessageBox( prj.GetText(TID_GAME_GAME_RESETTING), MB_OK, this );
		}
		else
			g_Option.m_bExitChange = FALSE;
		break;
#endif //__YENV
	case WIDC_CHECK2:
		pWndButton = (CWndButton*)GetDlgItem( WIDC_CHECK2 );
		g_Option.m_bDamageRender = pWndButton->GetCheck();
		break;

	case WIDC_CHECK6:
		pWndButton = (CWndButton*)GetDlgItem( WIDC_CHECK6 );
		g_Option.m_nPlayerName = pWndButton->GetCheck();
		break;
	case WIDC_CHECK7:
		pWndButton = (CWndButton*)GetDlgItem( WIDC_CHECK7 );
		g_Option.m_nOtherPlayerName = pWndButton->GetCheck();
		break;
	case WIDC_CHECK8:
		pWndButton = (CWndButton*)GetDlgItem( WIDC_CHECK8 );
		g_Option.m_nMonName = pWndButton->GetCheck();
		break;
//sun: 12, 옵션창 개선
	case WIDC_CHECK1:
	{
		if( pWndZoomLimit->GetCheck() )
			g_Option.m_bZoomLimit = FALSE;
		else
			g_Option.m_bZoomLimit = TRUE;			
	}	
	break;
	case WIDC_VIEWMASK:
		pWndButton = (CWndButton*)GetDlgItem( WIDC_VIEWMASK );
		g_Option.m_bViewMask = pWndButton->GetCheck();
		g_DPlay.SendOptionEnableRenderMask( pWndButton->GetCheck() );
		break;
	case WIDC_CHECK5:
		{
			if( pWndCamearaLock->GetCheck() )
				g_Option.m_bCameraLock = TRUE;
			else
				g_Option.m_bCameraLock = FALSE;			
		}
		break;

	case WIDC_CHECK3:
		pWndButton = (CWndButton*)GetDlgItem( WIDC_CHECK3 );
		if(pWndButton) g_Option.m_nWeatherEffect = pWndButton->GetCheck();
		break;
	/*
	case WIDC_EFFECTONOFF:
		{
			CWndButton* pButton = (CWndButton*)GetDlgItem(WIDC_EFFECTONOFF);
			g_Option.m_bEffect = pButton->GetCheck();
			pButton->SetCheck( g_Option.m_bEffect );
		}
		break;
	case WIDC_BMGONOFF:
		{
			CWndButton* pButton = (CWndButton*)GetDlgItem(WIDC_BMGONOFF);
			g_Option.m_bMusic = pButton->GetCheck();
			pButton->SetCheck( g_Option.m_bMusic );

			// 음악 끄고 켜기
			SetOn( g_Option.m_bMusic );
		}
		break;
	*/
		
/*		
	case WIDC_SOUND_ON:
		g_Option.m_nSoundEffect = g_SoundMng.m_nSoundVolume = 1;
		break;
	case WIDC_SOUND_OFF:
		g_Option.m_nSoundEffect = g_SoundMng.m_nSoundVolume = 0;
		break;

	case WIDC_MUSIC_ON:
		g_Option.m_fMusicVolume = 1.0f;
		SetVolume( 1.0f );
		break;
	case WIDC_MUSIC_OFF:
		g_Option.m_fMusicVolume = 0.0f;
		SetVolume( 0.0f );
		break;
*/
	case WIDC_OK:
		Destroy();
		break;
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

void CWndOptVideo::UpdateData( )
{
	CWndButton* pWndButton[ 3 ];

	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_OBJECT_LOD_HIGH );
	pWndButton[ 1 ] = (CWndButton*)GetDlgItem( WIDC_OBJECT_LOD_MID );
	pWndButton[ 2 ] = (CWndButton*)GetDlgItem( WIDC_OBJECT_LOD_LOW );
	pWndButton[ 0 ]->SetGroup( TRUE );
	pWndButton[ 0 ]->SetCheck( FALSE );
	pWndButton[ 1 ]->SetCheck( FALSE );
	pWndButton[ 2 ]->SetCheck( FALSE );
	pWndButton[ g_Option.m_nObjectDetail ]->SetCheck( TRUE );

	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_OBJECT_DISTANT_HIGH );
	pWndButton[ 1 ] = (CWndButton*)GetDlgItem( WIDC_OBJECT_DISTANT_MID );
	pWndButton[ 2 ] = (CWndButton*)GetDlgItem( WIDC_OBJECT_DISTANT_LOW );
	pWndButton[ 0 ]->SetGroup( TRUE );
	pWndButton[ 0 ]->SetCheck( FALSE );
	pWndButton[ 1 ]->SetCheck( FALSE );
	pWndButton[ 2 ]->SetCheck( FALSE );
	pWndButton[ g_Option.m_nObjectDistant ]->SetCheck( TRUE );

	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_TERRAIN_LOD_HIGH );
	pWndButton[ 1 ] = (CWndButton*)GetDlgItem( WIDC_TERRAIN_LOD_MID );
	pWndButton[ 2 ] = (CWndButton*)GetDlgItem( WIDC_TERRAIN_LOD_LOW );
	pWndButton[ 0 ]->SetGroup( TRUE );
	pWndButton[ 0 ]->SetCheck( FALSE );
	pWndButton[ 1 ]->SetCheck( FALSE );
	pWndButton[ 2 ]->SetCheck( FALSE );
	pWndButton[ g_Option.m_nViewArea ]->SetCheck( TRUE );

	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_SHADOW_HIGH );
	pWndButton[ 1 ] = (CWndButton*)GetDlgItem( WIDC_SHADOW_MID );
	pWndButton[ 2 ] = (CWndButton*)GetDlgItem( WIDC_SHADOW_LOW );
	pWndButton[ 0 ]->SetGroup( TRUE );
	pWndButton[ 0 ]->SetCheck( FALSE );
	pWndButton[ 1 ]->SetCheck( FALSE );
	pWndButton[ 2 ]->SetCheck( FALSE );
	pWndButton[ g_Option.m_nShadow ]->SetCheck( TRUE );
	if( g_Neuz.m_d3dCaps.MaxSimultaneousTextures <= 2 ) 
	{
		pWndButton[ 0 ]->EnableWindow( FALSE );
		pWndButton[ 1 ]->EnableWindow( FALSE );
		pWndButton[ 2 ]->EnableWindow( FALSE );
	}
#ifdef __SFX_OPT
	if( g_xFlyffConfig->GetMainLanguage() == LANG_KOR || g_xFlyffConfig->GetMainLanguage() == LANG_JAP)
	{
		pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_CHECK4 );
		if(pWndButton[ 0 ])
			pWndButton[ 0 ]->SetVisible(FALSE);
	}
#else
	pWndButton[ 1 ] = (CWndButton*)GetDlgItem( WIDC_VOICEON );
	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_VOICEOFF );
	pWndButton[ 1 ]->SetGroup( TRUE );
	pWndButton[ 0 ]->SetCheck( FALSE );
	pWndButton[ 1 ]->SetCheck( FALSE );
	pWndButton[ g_Option.m_bVoice ]->SetCheck( TRUE );

	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_CHECK5 );
	pWndButton[ 0 ]->SetCheck(!g_Option.m_bSFXRenderOff);

#endif
	
	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_CHECK6 );
	pWndButton[ 0 ]->SetCheck(g_Option.m_nPlayerName);
	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_CHECK7 );
	pWndButton[ 0 ]->SetCheck(g_Option.m_nOtherPlayerName);
	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_CHECK8 );
	pWndButton[ 0 ]->SetCheck(g_Option.m_nMonName);	
	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_CHECK2 );
	pWndButton[ 0 ]->SetCheck(g_Option.m_bDamageRender);
//sun: 12, 옵션창 개선
	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_CHECK1);
	pWndButton[ 0 ]->SetCheck(!g_Option.m_bZoomLimit);
	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_VIEWMASK );
	pWndButton[ 0 ]->SetCheck(g_Option.m_bViewMask);
	if(g_pPlayer && g_pPlayer->IsOptionRenderMask() != g_Option.m_bViewMask)
		g_DPlay.SendOptionEnableRenderMask( g_Option.m_bViewMask );
	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_CHECK5 );
	if(pWndButton[ 0 ])
	pWndButton[ 0 ]->SetCheck( g_Option.m_bCameraLock );

	m_Texture.LoadTexture( g_Neuz.GetDevice(), MakePath( DIR_THEME,g_xFlyffConfig->GetMainLanguage(), "WndVolumeBar.tga" ), 0xffff00ff, TRUE );
	m_TexturePt.LoadTexture( g_Neuz.GetDevice(), MakePath( DIR_THEME,g_xFlyffConfig->GetMainLanguage(),"ButtSpin.tga" ), 0xffffffff, TRUE );		
#ifdef __SFX_OPT
	m_nStep[0] = g_Option.m_nSfxLevel;
	m_nStep[1] = !g_Option.m_bSFXRenderOff;
#else
	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_MOUSE_HIGH );
	pWndButton[ 1 ] = (CWndButton*)GetDlgItem( WIDC_MOUSE_MID );
	pWndButton[ 2 ] = (CWndButton*)GetDlgItem( WIDC_MOUSE_LOW );
	pWndButton[ 0 ]->SetGroup( TRUE );
	pWndButton[ 0 ]->SetCheck( FALSE );
	pWndButton[ 1 ]->SetCheck( FALSE );
	pWndButton[ 2 ]->SetCheck( FALSE );
	pWndButton[ g_Option.m_MouseSpeed ]->SetCheck( TRUE );

	m_nStep[0] = g_Option.m_fEffectVolume * 10;
	m_nStep[1] = g_Option.m_fBGMVolume * 10;
#endif

	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_CHECK3 );
	if(pWndButton[ 0 ]) pWndButton[ 0 ]->SetCheck(g_Option.m_nWeatherEffect);

	//sun: 밝기, 감마, 명암 조절 Neuz
	m_nBrightTable[0] = -40;
	m_nBrightTable[1] = -30;
	m_nBrightTable[2] = -20;
	m_nBrightTable[3] = -10;
	m_nBrightTable[4] = 0;
	m_nBrightTable[5] = 10;
	m_nBrightTable[6] = 20;
	m_nBrightTable[7] = 30;
	m_nBrightTable[8] = 40;
	m_nBrightTable[9] = 50;
	m_nBrightTable[10] = 60;

	for( int i=0; i<11; i++ )
	{
		if( m_nBrightTable[i] == g_Option.m_nOverBright )
		{
			m_nStep[3] = i;
			break;
		}
	}

	m_nStep[2] = (int)( g_Option.m_fGamma * 5 );
	m_nStep[4] = (int)( g_Option.m_fContrast * 5 );
}


CWndOptWindow::CWndOptWindow() 
{ 
} 
CWndOptWindow::~CWndOptWindow() 
{ 
} 
void CWndOptWindow::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndOptWindow::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 

	CWndButton* pWndEffect = (CWndButton*)GetDlgItem( WIDC_CHECK );
	pWndEffect->SetCheck( g_Option.m_nWindowAlpha == 255 ? FALSE : TRUE);

	CWndButton* pWndButton[ 3 ];

	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_RADIO1 );
	pWndButton[ 1 ] = (CWndButton*)GetDlgItem( WIDC_RADIO2 );
	pWndButton[ 2 ] = (CWndButton*)GetDlgItem( WIDC_RADIO3 );
	pWndButton[ 0 ]->SetGroup( TRUE );
	pWndButton[ g_Option.m_nWindowEffect ]->SetCheck( TRUE );
/*
	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_OBJECT_DISTANT_HIGH );
	pWndButton[ 1 ] = (CWndButton*)GetDlgItem( WIDC_OBJECT_DISTANT_MID );
	pWndButton[ 2 ] = (CWndButton*)GetDlgItem( WIDC_OBJECT_DISTANT_LOW );
	pWndButton[ 0 ]->SetGroup( TRUE );
*/

	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// 처음 이 함수를 부르면 윈도가 열린다.
BOOL CWndOptWindow::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy에서 설정한 리소스로 윈도를 연다.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_OPTION_WINDOW, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  직접 윈도를 열때 사용 
BOOL CWndOptWindow::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndOptWindow::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndOptWindow::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndOptWindow::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndOptWindow::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndOptWindow::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	CWndButton* pWndAlpha = (CWndButton*)GetDlgItem( WIDC_CHECK );
	CWndButton* pWndEffect = (CWndButton*)GetDlgItem( WIDC_RADIO1 );
	switch( nID )
	{
	case WIDC_CHECK:
		if( pWndAlpha->GetCheck() )
			CWndBase::m_nAlpha = g_Option.m_nWindowAlpha = 128;
		else
			CWndBase::m_nAlpha = g_Option.m_nWindowAlpha = 255;
	case WIDC_RADIO1:
		break;
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

CWndOptMyInfo::CWndOptMyInfo() 
{ 
} 
CWndOptMyInfo::~CWndOptMyInfo() 
{ 
} 
void CWndOptMyInfo::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndOptMyInfo::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// 처음 이 함수를 부르면 윈도가 열린다.
BOOL CWndOptMyInfo::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy에서 설정한 리소스로 윈도를 연다.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_OPTION_MYINFO, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  직접 윈도를 열때 사용 
BOOL CWndOptMyInfo::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndOptMyInfo::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndOptMyInfo::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndOptMyInfo::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndOptMyInfo::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndOptMyInfo::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 



/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 테마
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//EMPTY_WNDCLASSMEMBER( CWndOptVideo, _T( "비디오 옵션" ) )

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 테마
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//EMPTY_WNDCLASSMEMBER( CWndOptSound, _T( "사운드 옵션" ) )

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 테마
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//EMPTY_WNDCLASSMEMBER( CWndOptMusic, _T( "배경음악 옵션" ) )

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 테마
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//EMPTY_WNDCLASSMEMBER( CWndOptMouse, _T( "마우스 감도 조절" ) )



int CWndOptVideo::GetSliderStep(DWORD dwWndId, int &nStep, CPoint point )
{
	LPWNDCTRL lpWndCtrl = GetWndCtrl( dwWndId );	
	CRect crect = lpWndCtrl->rect;
	
	crect.left += GetStepPos( nStep, lpWndCtrl->rect.right - lpWndCtrl->rect.left, MAX_SLIDER ).x;
	crect.right = crect.left+16;
	
	if( PtInRect( crect, point ) )
	{
		return -1;
	}
	
	crect = lpWndCtrl->rect;
	if( PtInRect( crect, point ) )
	{
		int nWidth = lpWndCtrl->rect.right - lpWndCtrl->rect.left;
		
		if( point.x > lpWndCtrl->rect.left+GetStepPos( nStep, nWidth, MAX_SLIDER ).x )
			nStep += 1;//nWidth / 10;
		else
			nStep -= 1;//nWidth / 10;

		return 1;
	}

	return 0;
}

int CWndOptVideo::GetSliderStep(DWORD dwWndId, int &nStep, CPoint point, int nDivision)
{
	LPWNDCTRL lpWndCtrl = GetWndCtrl( dwWndId );	
	CRect crect = lpWndCtrl->rect;
	
	crect.left += GetStepPos( nStep, lpWndCtrl->rect.right - lpWndCtrl->rect.left, nDivision ).x;
	crect.right = crect.left+16;
	
	if( PtInRect( crect, point ) )
	{
		return -1;
	}
	
	crect = lpWndCtrl->rect;
	if( PtInRect( crect, point ) )
	{
		int nWidth = lpWndCtrl->rect.right - lpWndCtrl->rect.left;
		
		if( point.x > lpWndCtrl->rect.left+GetStepPos( nStep, nWidth, nDivision ).x )
			nStep += 1;//nWidth / 10;
		else
			nStep -= 1;//nWidth / 10;

		return 1;
	}

	return 0;
}

void CWndOptVideo::GetRangeSlider(DWORD dwWndId, int &nStep, CPoint point, int nDivision)
{
	LPWNDCTRL lpWndCtrl = GetWndCtrl( dwWndId );	
	int nWidth = lpWndCtrl->rect.right - lpWndCtrl->rect.left;
	
	if( point.x > lpWndCtrl->rect.left+GetStepPos( nStep, nWidth, nDivision ).x )
		nStep += 1;//nWidth / 10;
	if( point.x < lpWndCtrl->rect.left+GetStepPos( nStep, nWidth, nDivision ).x )
		nStep -= 1;//nWidth / 10;
	
	if( nStep < 0 )
		nStep = 0;
	
	if( nStep > nDivision )
		nStep = nDivision;
}

void CWndOptVideo::GetRangeSlider(DWORD dwWndId, int &nStep, CPoint point)
{
	LPWNDCTRL lpWndCtrl = GetWndCtrl( dwWndId );	
	int nWidth = lpWndCtrl->rect.right - lpWndCtrl->rect.left;
	
	if( point.x > lpWndCtrl->rect.left+GetStepPos( nStep, nWidth, MAX_SLIDER ).x )
		nStep += 1;//nWidth / 10;
	if( point.x < lpWndCtrl->rect.left+GetStepPos( nStep, nWidth, MAX_SLIDER ).x )
		nStep -= 1;//nWidth / 10;
	
	if( nStep < 0 )
		nStep = 0;
	
	if( nStep > MAX_SLIDER )
		nStep = MAX_SLIDER;
}
