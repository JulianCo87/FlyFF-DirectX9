
#include "StdAfx.h"

#include "WndMadrigalGift.h"

// 18�� ������ ��¿ �� ���� ���� ��Ų��.
#include "../Neuz/DPClient.h"
#include "../_CommonDefine/Packet/FLPacketMadrigalGift.h"
extern CDPClient				g_DPlay;

#define NEXT_INCREASELEVEL		10		//  ���� ���� ���� (��ȹ �ǵ� �� ������ 10������)

//---------------------------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------------------------
CWndMadrigalGift::CWndMadrigalGift()
:	m_pWndButtonGiftBox( NULL )
,	m_pGiftBox( NULL )
,	m_pRoundGauge( NULL )
,	m_pRoundArrow( NULL )
,	m_rtGiftBox( 0, 0, 0, 0 )
,	m_rtRoundGauge( 0, 0, 0, 0 )
,	m_rtRoundArrow( 0, 0, 0, 0 )
,	m_ptRoundGaugeCenter( 0, 0 )
,	m_wRoundGaugeRadius( 0 )
,	m_eStep( eStep_None )
,	m_byPointPercent( 0 )
,	m_wLevelForCompensation( 0 )
,	m_pTexRoundArrow( NULL )
,	m_dwRoundGaugeColor( WNDCOLOR_WHITE )
,	m_bSendPacket( false )

{
	ZeroMemory( m_pTexGiftBox, sizeof( CTexture* ) * eStep_Max );
}


//---------------------------------------------------------------------------------------------
// �ı���
//---------------------------------------------------------------------------------------------
CWndMadrigalGift::~CWndMadrigalGift()
{
}


//---------------------------------------------------------------------------------------------
// �ʱ�ȭ
// param	: �θ� ������, ���� ������ ���̵�
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndMadrigalGift::Initialize( CWndBase* pWndParent, DWORD dwWndId )
{
	FLERROR_ASSERT_LOG_RETURN( pWndParent,	FALSE, _T("[���帮���� ����] �θ� ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
	FLERROR_ASSERT_LOG_RETURN( dwWndId,		FALSE, _T("[���帮���� ����] ���� ������ ���̵� ��ȿ���� �ʽ��ϴ�.") );

	AddWndStyle( WBS_NODRAWFRAME );
	DelWndStyle( WBS_CAPTION );

	m_bNoCloseButton =  TRUE;
	m_bNoHelpButton =	TRUE;

	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), dwWndId, 0, CPoint( 0, 0 ), pWndParent );
}


//---------------------------------------------------------------------------------------------
// ������Ʈ �ʱ�ȭ
// param	: ����
// return	: void
//---------------------------------------------------------------------------------------------
void CWndMadrigalGift::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 

	m_pWndButtonGiftBox = static_cast<CWndButton*>( GetDlgItem( WIDC_BUTTON1 ) );
	FLERROR_ASSERT_LOG_RETURN_VOID( m_pWndButtonGiftBox, _T("[���帮���� ����] ���� �ڽ� ��ư �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

	m_pGiftBox = GetWndCtrl( WIDC_BUTTON1 );
	FLERROR_ASSERT_LOG_RETURN_VOID( m_pGiftBox, _T("[���帮���� ����] ���� �ڽ� ��Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�."))

	m_rtGiftBox = m_pGiftBox->rect;

	m_pRoundGauge = GetWndCtrl( WIDC_CUSTOM1 );
	FLERROR_ASSERT_LOG_RETURN_VOID( m_pRoundGauge, _T("[���帮���� ����] �� ������ ��Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�."))
	
	m_rtRoundGauge = m_pRoundGauge->rect;

	m_pRoundArrow = GetWndCtrl( WIDC_CUSTOM2 );
	FLERROR_ASSERT_LOG_RETURN_VOID( m_pRoundArrow, _T("[���帮���� ����] ȭ��ǥ ��� ��Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

	m_rtRoundArrow = m_pRoundArrow->rect;

	m_pTexRoundArrow = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), _T("MadrigalGift_Bg.tga") ), 0, TRUE );
	FLERROR_ASSERT_LOG_RETURN_VOID( m_pRoundArrow, _T("[���帮���� ����] ȭ��ǥ ��� �ؽ��ĸ� �ҷ��� �� �����ϴ�.") );

	if( m_rtRoundGauge.Width() != m_rtRoundGauge.Height() )
		FLERROR_LOG( PROGRAM_NAME, _T("[���帮���� ����] ������ ��� ������ ���簢���� �ƴմϴ�. ������ ���� �� �ֽ��ϴ�. �������� Ȯ���ϼ���") );

	m_ptRoundGaugeCenter.x = m_rtRoundGauge.left + m_rtRoundGauge.Width() / 2;
	m_ptRoundGaugeCenter.y = m_rtRoundGauge.top + m_rtRoundGauge.Height() / 2;

	/// +1�� �׽�Ʈ
	// ���簢���̶�� �����Ͽ� �������� �׳� ���Ѵ�.
	m_wRoundGaugeRadius = (m_rtRoundGauge.Width() / 2) + 1;

	for( BYTE byCount = 0; byCount < eStep_Max; byCount++ )
	{
		if( 0 == byCount )
			continue;
		
		TCHAR szTextureName[32] = { NULL, };
		_sntprintf_s( szTextureName, _countof( szTextureName ), _TRUNCATE, _T( "buttMadrigalgift%02d.tga" ), byCount );
		
		m_pTexGiftBox[byCount] = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), szTextureName ), 0, TRUE );
	}

	if( eStep_None == m_eStep )
		m_pWndButtonGiftBox->EnableWindow( FALSE );
}  


//---------------------------------------------------------------------------------------------
// ���� �ܰ��� ���� �ڽ� �ؽ��� �����ϱ�
// param	: �ܰ�
// return	: ����
//---------------------------------------------------------------------------------------------
void CWndMadrigalGift::SetGiftBoxTexture()
{
	switch( m_eStep )
	{
		case eStep_None:
			{
				if( m_pTexGiftBox[eStep_1st] )
					m_pWndButtonGiftBox->m_pTexture = m_pTexGiftBox[eStep_1st];

				m_pWndButtonGiftBox->EnableWindow( FALSE );
			}
			break;
		case eStep_1st:
			{
				if( m_pTexGiftBox[eStep_1st] )
					m_pWndButtonGiftBox->m_pTexture = m_pTexGiftBox[eStep_1st];

				m_pWndButtonGiftBox->EnableWindow( TRUE );
			}
			break;
		case eStep_2nd:
			{
				if( m_pTexGiftBox[eStep_2nd] )
					m_pWndButtonGiftBox->m_pTexture = m_pTexGiftBox[eStep_2nd];

				m_pWndButtonGiftBox->EnableWindow( TRUE );
			}
			break;
		case eStep_3rd:
			{
				if( m_pTexGiftBox[eStep_3rd] )
					m_pWndButtonGiftBox->m_pTexture = m_pTexGiftBox[eStep_3rd];

				m_pWndButtonGiftBox->EnableWindow( TRUE );
			}
			break;
		default:
			{
				if( m_pTexGiftBox[eStep_1st] )
					m_pWndButtonGiftBox->m_pTexture = m_pTexGiftBox[eStep_1st];

				m_pWndButtonGiftBox->EnableWindow( FALSE );

				FLERROR_ASSERT_LOG_RETURN_VOID( NULL, _T("[���帮���� ����] ���� ���� �ܰ� �Դϴ�. �⺻ �ؽ��ķ� ��ä�մϴ�.") );	
			}
			break;
	}
}


//---------------------------------------------------------------------------------------------
// ���� �ܰ��� �� ������ ������
// param	: �ܰ�
// return	: ����
//---------------------------------------------------------------------------------------------
DWORD CWndMadrigalGift::GetRoundGaugeColor()
{
	PFLCOLOR_WNDMADRIGALGIFT pColorWndMadrigalGift = g_FLColorManager->GetWndMadrigalGift();

	switch( m_eStep )
	{
		case eStep_None:
			return pColorWndMadrigalGift->m_stRoundGuage_StepNone.GetColor();
		case eStep_1st:
			return pColorWndMadrigalGift->m_stRoundGuage_Step1.GetColor();
		case eStep_2nd:
			return pColorWndMadrigalGift->m_stRoundGuage_Step2.GetColor();
		case eStep_3rd:
			return pColorWndMadrigalGift->m_stRoundGuage_Step3.GetColor();
		default:
			{
				FLERROR_ASSERT_LOG_RETURN( NULL, WNDCOLOR_BLACK, _T("[���帮���� ����] ���� ���� �ܰ� �Դϴ�.") );	
			}
			break;	
	}

	FLERROR_ASSERT_LOG_RETURN( NULL, WNDCOLOR_BLACK, _T("[���帮���� ����] ���� �����߻� ���� �� ���� �����Դϴ�.") );	
}


//---------------------------------------------------------------------------------------------
// ó��
// param	: void
// return	: ���� / ����
//---------------------------------------------------------------------------------------------
BOOL CWndMadrigalGift::Process()
{
	if( m_pGiftBox )
		m_rtGiftBox = m_pGiftBox->rect;

	if( m_pRoundGauge )
		m_rtRoundGauge = m_pRoundGauge->rect;

	if( m_pRoundArrow )
		m_rtRoundArrow = m_pRoundArrow->rect;

	SetGiftBoxTexture();
	m_dwRoundGaugeColor = GetRoundGaugeColor();

	return TRUE;
}


//---------------------------------------------------------------------------------------------
// �׸���
// param	: ������
// return	: void
//---------------------------------------------------------------------------------------------
void CWndMadrigalGift::OnDraw( C2DRender* p2DRender )
{
	if( NULL == p2DRender )
		return ;
	
	const BYTE byMaxPoint = 100;

	float fDegree = static_cast<float>( ( 360.0f * m_byPointPercent ) / byMaxPoint );

	D3DXVECTOR2		vCenterPos( static_cast<float>(m_ptRoundGaugeCenter.x), static_cast<float>(m_ptRoundGaugeCenter.y) );
	D3DXVECTOR2		vStandardPos( static_cast<float>(m_ptRoundGaugeCenter.x), static_cast<float>(m_ptRoundGaugeCenter.y - m_wRoundGaugeRadius) );

	float fRadian( D3DXToRadian( 3.6f ) );	// 360�� 1%�� 3.6 �̹Ƿ� ( Degree�� Radian���� ���� )

	for( BYTE byCount = 0; byCount < m_byPointPercent; byCount++ )
	{
		D3DXVECTOR2 vStandardAxis = vCenterPos - vStandardPos;

		D3DXVECTOR2 vRot( 0.0f, 0.0f );

		D3DXMATRIX mRotZ;
		D3DXMatrixIdentity( &mRotZ );
		D3DXMatrixRotationZ( &mRotZ, fRadian );
	
		D3DXVec2TransformCoord( &vRot, &vStandardAxis, &mRotZ );  
			
		vRot = vCenterPos - vRot; 

		p2DRender->RenderFillTriangle(	m_ptRoundGaugeCenter, 
										CPoint( (int)vStandardPos.x, (int)vStandardPos.y ), 
										CPoint( (int)vRot.x, (int)vRot.y ), 
										m_dwRoundGaugeColor, 
										m_dwRoundGaugeColor, 
										m_dwRoundGaugeColor
									 );
			
		vStandardPos = vRot;
	}

	DrawRoundArrow( p2DRender );
	DrawTooltip( p2DRender );
}


//---------------------------------------------------------------------------------------------
// ��ŷ �����ΰ�?
// param	: ���콺 ��ġ
// return	: �� / �ƴϿ�
//---------------------------------------------------------------------------------------------
BOOL CWndMadrigalGift::IsPickupSpace( CPoint point )
{
	if( m_rtGiftBox.PtInRect( point ) )
		return TRUE;

	if( m_rtRoundGauge.PtInRect( point ) )
		return TRUE;

	return TRUE;
}


//---------------------------------------------------------------------------------------------
// ���� �ؽ�Ʈ ���
// param	: void
// return	: ���ڿ� ������
//---------------------------------------------------------------------------------------------
CEditString CWndMadrigalGift::GetToolTipText()
{
	PFONTCOLOR_WNDMADRIGALGIFT pFontColorWndMadrigalGift = g_WndFontColorManager->GetWndMadrigalGift();

	CString			strTemp( _T("") );
	CEditString		strResult( _T("") );
	BYTE			byLegend( g_pPlayer->GetLegendChar() );


	switch( m_eStep )
	{
		case eStep_None:
			{
				strTemp.Format( prj.GetText( TID_MMI_MADRIGALGIFT_TEXT03 ), ( BYTE )m_eStep );
				strResult.AddParsingString( strTemp, pFontColorWndMadrigalGift->m_stToolTip.GetFontColor() );
				strTemp.Empty();

				strTemp.Format( prj.GetText( TID_MMI_MADRIGALGIFT_TEXT04 ), m_byPointPercent );
				strResult.AddParsingString( strTemp, pFontColorWndMadrigalGift->m_stToolTip.GetFontColor() );
				strTemp.Empty();

				strTemp = GetCurrentLevelForCompensationToolTipText();
				strResult.AddParsingString( strTemp, pFontColorWndMadrigalGift->m_stToolTip.GetFontColor() );
				strTemp.Empty();

				strTemp.Format( prj.GetText( TID_MMI_MADRIGALGIFT_TEXT02 ), ( BYTE )(m_eStep + 1) );
				strResult.AddParsingString( strTemp );
				strTemp.Empty();

				return strResult;
			}
			break;
		case eStep_1st:
		case eStep_2nd:
			{
				strTemp = GetCurrentLevelForCompensationToolTipText();
				strResult.AddParsingString( strTemp, pFontColorWndMadrigalGift->m_stToolTip.GetFontColor() );
				strTemp.Empty();

				strTemp.Format( prj.GetText( TID_MMI_MADRIGALGIFT_TEXT04 ), m_byPointPercent );
				strResult.AddParsingString( strTemp, pFontColorWndMadrigalGift->m_stToolTip.GetFontColor() );
				strTemp.Empty();

				strTemp = GetNextLevelForCompensationToolTipText();
				strResult.AddParsingString( strTemp, pFontColorWndMadrigalGift->m_stToolTip.GetFontColor() );
				strTemp.Empty();

				strTemp.Format( prj.GetText( TID_MMI_MADRIGALGIFT_TEXT08 ), ( BYTE )m_eStep );
				strResult.AddParsingString( strTemp );
				strTemp.Empty();

				return strResult;
			}
			break;
		case eStep_3rd:
			{
				strTemp = GetCurrentLevelForCompensationToolTipText();
				strResult.AddParsingString( strTemp, pFontColorWndMadrigalGift->m_stToolTip.GetFontColor() );
				strTemp.Empty();

				strTemp.Format( prj.GetText( TID_MMI_MADRIGALGIFT_TEXT08 ), ( BYTE )m_eStep );
				strResult.AddParsingString( strTemp );
				strTemp.Empty();

				return strResult;
			}
			break;
		default:
			{
				FLERROR_ASSERT_LOG_RETURN( NULL, strResult, _T("[���帮���� ����] ���� ���� �ܰ� �Դϴ�. ���� ���ڿ��� ã�� �� �����ϴ�.") );	
			}
			break;	
	}

	FLERROR_ASSERT_LOG_RETURN( NULL, strResult, _T("[���帮���� ����] ���� �����߻� ���� �� ���� �����Դϴ�.") );	
}


//---------------------------------------------------------------------------------------------
// ���� �ܰ��� ���� ���� ���� �ؽ�Ʈ ���
// param	: ������
// return	: bool
//---------------------------------------------------------------------------------------------
CString CWndMadrigalGift::GetCurrentLevelForCompensationToolTipText()
{
	CString strToolTip( _T("") );

	switch( g_pPlayer->GetLegendChar() )
	{
		case LEGEND_CLASS_MASTER:
			strToolTip.Format( prj.GetText( TID_MMI_MADRIGALGIFT_TEXT06 ), ( BYTE )m_eStep, prj.GetText( TID_MMI_JOBGRADE_MASTER_TEXT ), m_wLevelForCompensation );
			break;
		case LEGEND_CLASS_HERO:
		case LEGEND_CLASS_LEGEND_HERO:		
			strToolTip.Format( prj.GetText( TID_MMI_MADRIGALGIFT_TEXT06 ), ( BYTE )m_eStep, prj.GetText( TID_MMI_JOBGRADE_HERO_TEXT ), m_wLevelForCompensation );
			break;

		default:
			strToolTip.Format( prj.GetText( TID_MMI_MADRIGALGIFT_TEXT06 ), ( BYTE )m_eStep, _T(""), m_wLevelForCompensation );
			break;
	}

	return strToolTip;
}


//---------------------------------------------------------------------------------------------
// ���� �ܰ��� ���� ���� ���� �ؽ�Ʈ ���
// param	: ������
// return	: bool
//---------------------------------------------------------------------------------------------
CString CWndMadrigalGift::GetNextLevelForCompensationToolTipText()
{
	CString strToolTip( _T("") );

	switch( g_pPlayer->GetLegendChar() )
	{
		case LEGEND_CLASS_MASTER:
			strToolTip.Format( prj.GetText( TID_MMI_MADRIGALGIFT_TEXT07 ), ( BYTE )(m_eStep + 1), prj.GetText( TID_MMI_JOBGRADE_MASTER_TEXT ), m_wLevelForCompensation + NEXT_INCREASELEVEL );
			break;
		case LEGEND_CLASS_HERO:
		case LEGEND_CLASS_LEGEND_HERO:		
			strToolTip.Format( prj.GetText( TID_MMI_MADRIGALGIFT_TEXT07 ), ( BYTE )(m_eStep + 1), prj.GetText( TID_MMI_JOBGRADE_HERO_TEXT ), m_wLevelForCompensation + NEXT_INCREASELEVEL );
			break;
		default:
			strToolTip.Format( prj.GetText( TID_MMI_MADRIGALGIFT_TEXT07 ), ( BYTE )(m_eStep + 1), _T(""), m_wLevelForCompensation + NEXT_INCREASELEVEL );
			break;
	}

	return strToolTip;
}


//---------------------------------------------------------------------------------------------
// ������ ���� �׸���
// param	: ������
// return	: bool
//---------------------------------------------------------------------------------------------
bool CWndMadrigalGift::DrawTooltip( C2DRender* p2DRender  )
{
	if( NULL == p2DRender )
		return false;

	if( m_pWndOnMouseMove != this &&
		m_pWndOnMouseMove != m_pWndButtonGiftBox )
		return false;

	CPoint	ptPoint( GetMousePoint() );
	CRect	rtClientRect( m_rtRoundArrow );		

	if( FALSE == rtClientRect.PtInRect( ptPoint ) )
		return false;

	if( m_bLButtonDown )
		return false;

	CPoint	ptTemp = ptPoint;
	CRect	rtTemp = rtClientRect;

	ClientToScreen( &ptTemp );
	ClientToScreen( &rtTemp );
		
	CEditString strToolTip( GetToolTipText() ); 
	g_toolTip.PutToolTip( (DWORD)this , strToolTip, rtTemp, ptTemp, 3 );

	return true;
}


//---------------------------------------------------------------------------------------------
// �� ����� ȭ��ǥ ��� �׸���
// param	: ������
// return	: bool
//---------------------------------------------------------------------------------------------
bool CWndMadrigalGift::DrawRoundArrow( C2DRender* p2DRender  )
{
	if( NULL == p2DRender )
		return false;

	CPoint	ptPoint( GetMousePoint() );
	CRect	rtClientRect( GetClientRect() );		

	if( rtClientRect.PtInRect( ptPoint ) && m_pTexRoundArrow )
		p2DRender->RenderTexture( m_rtRoundArrow.TopLeft(), m_pTexRoundArrow );

	return true;
}


//---------------------------------------------------------------------------------------------
// ����� ���� ���� �̺�Ʈ ó��
// param	: ������ ���̵�, �޽���, �⺻ ������
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndMadrigalGift::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase )
{
	if( WIDC_BUTTON1 == nID )			// ������ư 
	{
		if( false == m_bSendPacket &&	// ��Ŷ�� ������ �ʾҰ�
			IsGiftBoxEnable() )			// �ڽ� ��ư�� Ȱ��ȭ �Ǿ� �ֵ𸶳�
		{
			FLPacketMadrigalGiftItemReceiveReq kPacket;
			g_DPlay.SendPacket( &kPacket );
			SetGiftBoxButtonEnable( false );
			m_bSendPacket = true;
		}
	}

	return CWndBase::OnCommand( nID, dwMessage, pWndBase );
}


//---------------------------------------------------------------------------------------------
// ���� ���� ��ư Ȱ��ȭ / �� Ȱ��ȭ
// param	: Ȱ�� / �� Ȱ��
// return	: void
//---------------------------------------------------------------------------------------------
void CWndMadrigalGift::SetGiftBoxButtonEnable( bool bEnable )
{
	if( m_pWndButtonGiftBox )
		m_pWndButtonGiftBox->EnableWindow( bEnable );
}


//---------------------------------------------------------------------------------------------
// ���� ���� ��ư�� Ȱ��ȭ �Ǿ� �ֳ�? 
// param	: void
// return	: Ȱ�� / �� Ȱ��
//---------------------------------------------------------------------------------------------
bool CWndMadrigalGift::IsGiftBoxEnable()
{
	if( m_pWndButtonGiftBox )
		return m_pWndButtonGiftBox->IsWindowEnabled() ? true : false;

	return false;
}