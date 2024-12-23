
#include "StdAfx.h"

#include "WndElldinsJar.h"

#define ITEMNUMBER_POS_OFFSET	11

// ���� �α׸� ����Ϸ��� Ȱ��ȭ ��Ű����.
//#define WNDELLDINSJAR_SHOW_DETAIL_LOG

//---------------------------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------------------------
CWndElldinsJar::CWndElldinsJar()
:	m_pFillTexture( NULL )
,	m_pUnSealedElldinsJar( NULL )
,	m_p1stPotion( NULL )
,	m_p2ndPotion( NULL )
,	m_dwUnSealElldinsJarID( NULL )
,	m_dw1stPostionID( NULL )
,	m_dw2ndPostionID( NULL )
,	m_pWndButtonOK( NULL )
,	m_pWndCurrentSavedHP( NULL )
,	m_pWndCanBeMaximumSaveHP( NULL )
{
	ZeroMemory( m_Rect, sizeof( CRect ) * _countof( m_Rect ) ); 

	OBJID dwID = g_pPlayer->GetRegisterElldinsJarID();
	if( NULL != dwID )
		FindRegisterUnSealElldinsJar( dwID );
	
}


//---------------------------------------------------------------------------------------------
// �ı���
//---------------------------------------------------------------------------------------------
CWndElldinsJar::~CWndElldinsJar()
{
}


//---------------------------------------------------------------------------------------------
// �ʱ�ȭ
// param	: �θ� ������, ���� ������ ���̵�
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndElldinsJar::Initialize( CWndBase* pWndParent, DWORD dwWndId )
{
	FLERROR_ASSERT_LOG_RETURN( pWndParent,	FALSE, _T("[������ �׾Ƹ�] �θ� ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
	FLERROR_ASSERT_LOG_RETURN( dwWndId,		FALSE, _T("[������ �׾Ƹ�] ���� ������ ���̵� ��ȿ���� �ʽ��ϴ�.") );

	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), dwWndId, 0, CPoint( 0, 0 ), pWndParent );
}


//---------------------------------------------------------------------------------------------
// ������Ʈ �ʱ�ȭ
// param	: ����
// return	: void
//---------------------------------------------------------------------------------------------
void CWndElldinsJar::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 

	m_pFillTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_THEME, g_xFlyffConfig->GetMainLanguage(), _T("ElldinImage01.tga") ), 0, TRUE );
	FLERROR_ASSERT_LOG_RETURN_VOID( m_pFillTexture, _T("[������ �׾Ƹ�] �׾Ƹ� ä��� �ؽ��� �ε� ����!!."))

	LPWNDCTRL pJarSlot( GetWndCtrl( WIDC_STATIC3 ) );
	FLERROR_ASSERT_LOG_RETURN_VOID( pJarSlot, _T("[������ �׾Ƹ�] �׾Ƹ� ���� ��Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�."))

	LPWNDCTRL pJarImage( GetWndCtrl( WIDC_CUSTOM1 ) );
	FLERROR_ASSERT_LOG_RETURN_VOID( pJarImage, _T("[������ �׾Ƹ�] �׾Ƹ� ���� ��Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�."))

	LPWNDCTRL p1stPotionSlot( GetWndCtrl( WIDC_STATIC4 ) );
	FLERROR_ASSERT_LOG_RETURN_VOID( p1stPotionSlot, _T("[������ �׾Ƹ�] ���� 1 ���� ��Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�."))

	LPWNDCTRL p2ndPotionSlot( GetWndCtrl( WIDC_STATIC5 ) );
	FLERROR_ASSERT_LOG_RETURN_VOID( p2ndPotionSlot, _T("[������ �׾Ƹ�] ���� 2 ���� ��Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�."))

	m_pWndButtonOK = static_cast<CWndButton*>( GetDlgItem( WIDC_BUTTON1 ) );
	FLERROR_ASSERT_LOG_RETURN_VOID( m_pWndButtonOK, _T("[������ �׾Ƹ�] OK ��ư ��Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�."))

	m_pWndCurrentSavedHP = static_cast<CWndStatic*>( GetDlgItem( WIDC_STATIC7 ) );
	FLERROR_ASSERT_LOG_RETURN_VOID( m_pWndCurrentSavedHP, _T("[������ �׾Ƹ�] ���� ���� �� ü�� ��Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�."))

	m_pWndCanBeMaximumSaveHP = static_cast<CWndStatic*>( GetDlgItem( WIDC_STATIC8 ) );
	FLERROR_ASSERT_LOG_RETURN_VOID( m_pWndCanBeMaximumSaveHP, _T("[������ �׾Ƹ�] �ִ� ���� �� �� �ִ� ü�� ��Ʈ�� �����Ͱ� ��ȿ���� �ʽ��ϴ�."))

	m_Rect[eSlot::eSlot_Jar]		= pJarSlot->rect;
	m_Rect[eSlot::eSlot_JarImage]	= pJarImage->rect;
	m_Rect[eSlot::eSlot_1stPotion]	= p1stPotionSlot->rect;
	m_Rect[eSlot::eSlot_2ndPotion]	= p2ndPotionSlot->rect;

	m_pWndButtonOK->EnableWindow( FALSE );

	MoveParentCenter();
} 


//---------------------------------------------------------------------------------------------
// ������ ���� �� ������ �׾Ƹ� ������ �ΰ�?
// param	: ������ ���̵�
// return	: ã�Ҵ�(true) / �� ã�Ҵ�(false)
//---------------------------------------------------------------------------------------------
bool CWndElldinsJar::IsUnSealElldinsJar( DWORD dwItemID )
{
	CItemContainer<FLItemElem>* pItemContainer = &g_pPlayer->m_Inventory;

	FLItemElem* pItemElem = pItemContainer->GetAtId( dwItemID );	
	FLERROR_ASSERT_LOG_RETURN( pItemElem, false, _T("[������ �׾Ƹ�] ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
	
	PT_ITEM_SPEC pItemSpec = pItemElem->GetProp();
	FLERROR_ASSERT_LOG_RETURN( pItemSpec, false, _T("[������ �׾Ƹ�] ������ ���� �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
	
	if( pItemSpec->dwItemKind2 != IK2_ELLDINPOTION ||
		pItemSpec->dwItemKind3 != IK3_SAVEPOTION )
		return false;

	if( pItemElem->m_dwKeepTime == NULL )			// ���εǾ� ����	
		return false;

	if( pItemElem->IsFlag( FLItemElem::expired ) )	// �Ⱓ ���� ��
		return false;

	m_pUnSealedElldinsJar	= pItemElem;
	m_dwUnSealElldinsJarID	= pItemElem->m_dwObjId;

	g_pPlayer->SetRegisterElldinsJarID( m_dwUnSealElldinsJarID );

#ifdef WNDELLDINSJAR_SHOW_DETAIL_LOG	
	FLINFO_LOG( PROGRAM_NAME, _T("[������ �׾Ƹ�] ���� �� �׾Ƹ� ��� : OBJECTID - [ %u ], �̸� - [ %s ]"), pItemElem->m_dwObjId, pItemSpec->szName );
#endif

	return true;
}


//---------------------------------------------------------------------------------------------
// ������ �׾Ƹ��� ��� ������ �����ΰ�?
// param	: ������ ���̵�
// return	: ����(true) / �� ����(false)
//---------------------------------------------------------------------------------------------
bool CWndElldinsJar::IsPossibleRegisterElldinsJarPotion( DWORD dwItemID, eSlot eSlotIndex )
{
	if( NULL == m_pUnSealedElldinsJar )	// �׾Ƹ��� ���� ��ϵǾ� �־�� ��
	{
		g_WndMng.PutString( prj.GetText(TID_MMI_ELLDINPOTION_TEXT12), NULL, prj.GetTextColor(TID_MMI_ELLDINPOTION_TEXT12) );
		return true;
	}

	CItemContainer<FLItemElem>* pItemContainer = &g_pPlayer->m_Inventory;

	FLItemElem* pItemElem = pItemContainer->GetAtId( dwItemID );	
	FLERROR_ASSERT_LOG_RETURN( pItemElem, false, _T("[������ �׾Ƹ�] ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
	
	PT_ITEM_SPEC pItemSpec = pItemElem->GetProp();
	FLERROR_ASSERT_LOG_RETURN( pItemSpec, false, _T("[������ �׾Ƹ�] ������ ���� �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
	
	if( FALSE == pItemSpec->bCanSavePotion )
		return false;
	
	if( eSlotIndex == eSlot::eSlot_1stPotion )
	{
		if( m_p2ndPotion )
		{
			if( m_dw2ndPostionID == pItemElem->m_dwObjId )
			{
				g_WndMng.PutString( prj.GetText(TID_MMI_ELLDINPOTION_TEXT13), NULL, prj.GetTextColor(TID_MMI_ELLDINPOTION_TEXT13) );
				return true;
			}
		}
		
		m_p1stPotion = pItemElem;
		m_dw1stPostionID = pItemElem->m_dwObjId;
	}

	if( eSlotIndex == eSlot::eSlot_2ndPotion )
	{
		if( m_p1stPotion )
		{
			if( m_dw1stPostionID == pItemElem->m_dwObjId )
			{
				g_WndMng.PutString( prj.GetText(TID_MMI_ELLDINPOTION_TEXT13), NULL, prj.GetTextColor(TID_MMI_ELLDINPOTION_TEXT13) );
				return true;
			}
		}

		m_p2ndPotion = pItemElem;
		m_dw2ndPostionID = pItemElem->m_dwObjId;
	}

#ifdef WNDELLDINSJAR_SHOW_DETAIL_LOG	
	FLINFO_LOG( PROGRAM_NAME, _T("[������ �׾Ƹ�] ���� : OBJECTID - [ %u ], �̸� - [ %s ]"), pItemElem->m_dwObjId, pItemSpec->szName );
#endif

	return true;
}


//---------------------------------------------------------------------------------------------
// �������� �κ��丮�� �����ϴ��� üũ�Ѵ�.
// param	: ���� �ε���
// return	: void
//---------------------------------------------------------------------------------------------
void CWndElldinsJar::CheckItemFromInventory( eSlot eSlotIndex )
{
	CItemContainer<FLItemElem>* pItemContainer = &g_pPlayer->m_Inventory;

	if( eSlot::eSlot_Jar == eSlotIndex )
	{
		if( NULL == m_dwUnSealElldinsJarID )
			return ;
		
		FLItemElem* pItemElem = pItemContainer->GetAtId( m_dwUnSealElldinsJarID );	
		if( NULL == pItemElem )
		{
			m_pUnSealedElldinsJar	= NULL;
			m_dwUnSealElldinsJarID	= NULL;

			g_pPlayer->SetRegisterElldinsJarID( NULL );
		}		
	}

	if( eSlot::eSlot_1stPotion == eSlotIndex )
	{
		if( NULL == m_dw1stPostionID )
			return ;
		
		FLItemElem* pItemElem = pItemContainer->GetAtId( m_dw1stPostionID );	
		if( NULL == pItemElem )
		{
			m_p1stPotion		= NULL;
			m_dw1stPostionID	= NULL;
		}		
	}

	if( eSlot::eSlot_2ndPotion == eSlotIndex )
	{
		if( NULL == m_dw2ndPostionID )
			return ;
		
		FLItemElem* pItemElem = pItemContainer->GetAtId( m_dw2ndPostionID );	
		if( NULL == pItemElem )
		{
			m_p2ndPotion		= NULL;
			m_dw2ndPostionID	= NULL;
		}		
	}
}


//---------------------------------------------------------------------------------------------
// ��� �� ������ ���� �� ������ �׾Ƹ��� ã�´�
// param	: ������ ���̵�
// return	: bool
//---------------------------------------------------------------------------------------------
bool CWndElldinsJar::FindRegisterUnSealElldinsJar( OBJID dwItemID )
{
	CItemContainer<FLItemElem>* pItemContainer = &g_pPlayer->m_Inventory;
	
	FLItemElem* pItemElem = pItemContainer->GetAtId( dwItemID );	
	FLERROR_ASSERT_LOG_RETURN( pItemElem, false, _T("[������ �׾Ƹ�] ��ϵǰ� ������ ���� �� ������ �׾Ƹ��� ã�� �� �ʽ��ϴ�.") );

	m_pUnSealedElldinsJar	= pItemElem;
	m_dwUnSealElldinsJarID	= pItemElem->m_dwObjId;

#ifdef WNDELLDINSJAR_SHOW_DETAIL_LOG	
	FLINFO_LOG( PROGRAM_NAME, _T("[������ �׾Ƹ�] ��� �� ������ ���� �� �׾Ƹ� ã�� : OBJECTID - [ %u ]"), pItemElem->m_dwObjId );
#endif
	return true;
}

//---------------------------------------------------------------------------------------------
// ��� �� ù��° ������ ã�´�
// param	: ������ ���̵�
// return	: bool
//---------------------------------------------------------------------------------------------
bool CWndElldinsJar::FindRegister1stPostion( OBJID dwItemID )
{
	CItemContainer<FLItemElem>* pItemContainer = &g_pPlayer->m_Inventory;
	
	FLItemElem* pItemElem = pItemContainer->GetAtId( dwItemID );	
	FLERROR_ASSERT_LOG_RETURN( pItemElem, false, _T("[������ �׾Ƹ�] ��ϵǰ� ù��° ���ฦ ã�� �� �ʽ��ϴ�.") );

	m_p1stPotion	= pItemElem;
	m_dw1stPostionID	= pItemElem->m_dwObjId;

#ifdef WNDELLDINSJAR_SHOW_DETAIL_LOG	
	FLINFO_LOG( PROGRAM_NAME, _T("[������ �׾Ƹ�] ��� �� ù��° ���� ã�� : OBJECTID - [ %u ]"), pItemElem->m_dwObjId );
#endif
	return true;
}

//---------------------------------------------------------------------------------------------
// ��� �� �ι�° ������ ã�´�
// param	: ������ ���̵�
// return	: bool
//---------------------------------------------------------------------------------------------
bool CWndElldinsJar::FindRegister2ndPostion( OBJID dwItemID )
{
	CItemContainer<FLItemElem>* pItemContainer = &g_pPlayer->m_Inventory;
	
	FLItemElem* pItemElem = pItemContainer->GetAtId( dwItemID );	
	FLERROR_ASSERT_LOG_RETURN( pItemElem, false, _T("[������ �׾Ƹ�] ��ϵǰ� �ι�° ���ฦ ã�� �� �ʽ��ϴ�.") );

	m_p2ndPotion	= pItemElem;
	m_dw2ndPostionID	= pItemElem->m_dwObjId;

#ifdef WNDELLDINSJAR_SHOW_DETAIL_LOG	
	FLINFO_LOG( PROGRAM_NAME, _T("[������ �׾Ƹ�] ��� �� �ι�° ���� ã�� : OBJECTID - [ %u ]"), pItemElem->m_dwObjId );
#endif
	return true;
}




//---------------------------------------------------------------------------------------------
// ���Կ� ������ �׸���
// param	: ������, ������, ���� �ε���
// return	: bool
//---------------------------------------------------------------------------------------------
bool CWndElldinsJar::DrawIconInSlot( C2DRender* p2DRender, FLItemElem* pItem, eSlot eSlotIndex )
{
	if( NULL == pItem )
		return false;

	CTexture* pTexture = pItem->GetTexture();
	FLERROR_ASSERT_LOG_RETURN( pTexture, false, _T("[������ �׾Ƹ�] �����ۿ� �ؽ��� ������ �����ϴ�.") );

	pTexture->Render( p2DRender, m_Rect[eSlotIndex].TopLeft() );

	if( pItem->m_nItemNum > 1 )
	{
		CString strNumber( _T("") );
		strNumber.Format( _T("%d"), pItem->m_nItemNum );

		
		CD3DFont* pFont = p2DRender->GetFont();
		FLERROR_ASSERT_LOG_RETURN( pFont, false, _T("[������ �׾Ƹ�] ��Ʈ�� ���� �� ����.") );
		
		SIZE StringSize;

		StringSize.cx = 0;
		StringSize.cy = 0;

		PFONTCOLOR_WNDELLDINSJAR pFontColorWndElldinsJar = g_WndFontColorManager->GetWndElldinsJar();

		pFont->GetTextExtent( strNumber, &StringSize, g_xFlyffConfig->GetCodePage() );
		p2DRender->TextOut( (m_Rect[eSlotIndex].right - StringSize.cx) ,  m_Rect[eSlotIndex].bottom - StringSize.cy , strNumber, pFontColorWndElldinsJar->m_stItemCountShadow.GetFontColor() );	
		p2DRender->TextOut( (m_Rect[eSlotIndex].right - StringSize.cx) - 1,  ( m_Rect[eSlotIndex].bottom - StringSize.cy ) - 1 , strNumber, pFontColorWndElldinsJar->m_stItemCount.GetFontColor() );	
	}

	return true;
}


//---------------------------------------------------------------------------------------------
// ������ ���� �׸���
// param	: ������, ������, ���� �ε���
// return	: bool
//---------------------------------------------------------------------------------------------
bool CWndElldinsJar::DrawTooltip( C2DRender* p2DRender, FLItemElem* pItem, eSlot eSlotIndex )
{
	if( NULL == pItem )
		return false;

	CPoint ptPoint = GetMousePoint();
	if( m_Rect[eSlotIndex].PtInRect( ptPoint ) )
	{
		CPoint	ptTemp = ptPoint;
		CRect	rtTemp = m_Rect[eSlotIndex];

		ClientToScreen( &ptTemp );
		ClientToScreen( &rtTemp );
		
		g_WndMng.PutToolTip_Item( static_cast<FLItemBase*>( pItem ), ptTemp, &rtTemp );
	}

	return true;
}


//---------------------------------------------------------------------------------------------
// HP �۾� �׸���
// param	: ������
// return	: bool
//---------------------------------------------------------------------------------------------
bool CWndElldinsJar::DrawTextHP( C2DRender* p2DRender )
{
	FLERROR_ASSERT_LOG_RETURN( m_pWndCurrentSavedHP, false, _T("[������ �׾Ƹ�] ���� ���� �� ü�� ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
	FLERROR_ASSERT_LOG_RETURN( m_pWndCanBeMaximumSaveHP, false, _T("[������ �׾Ƹ�] �ִ� ���� �� �� �ִ� ü�� ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

	if( NULL == m_pUnSealedElldinsJar )
		return false;

	CString strCurrentHP( _T("") );
	strCurrentHP.Format( _T("%d"), m_pUnSealedElldinsJar->m_nHitPoint );
	
	m_pWndCurrentSavedHP->SetTitle( strCurrentHP );

	PT_ITEM_SPEC pItemSpec = m_pUnSealedElldinsJar->GetProp();
	FLERROR_ASSERT_LOG_RETURN( pItemSpec, false, _T("[������ �׾Ƹ�] ������ �Ӽ� �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

	CString strMaximumHP( _T("") );
	strMaximumHP.Format( _T("%d"), pItemSpec->nAdjParamVal[0] );

	m_pWndCanBeMaximumSaveHP->SetTitle( strMaximumHP );

	return true;
}


//---------------------------------------------------------------------------------------------
// �׾Ƹ� ä��� �� �׸���
// param	: ������
// return	: bool
//---------------------------------------------------------------------------------------------
bool CWndElldinsJar::DrawFillTexture( C2DRender* p2DRender )
{
	FLERROR_ASSERT_LOG_RETURN( m_pFillTexture, false, _T("[������ �׾Ƹ�] ���� ���� �� ü�� ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );
	
	//-------------------------------------------------------------------------------
	// UV ��� ����
	// �׾Ƹ� �� Texture���� ���� �Ʒ� �� ������ ������ ������ ������ ����Ͽ� ( ��갪 : 63 Pixel )
	// �� ���� 100%�� �Ͽ� ���� �׾Ƹ��� �ִ밪���� ����Ǿ� �ִ� ������ ä���� ���� ����Ͽ� ( �� : 10% )
	// �׸�ŭ�� Texture�� UV�� �����Ͽ� �׸���. �� �׾Ƹ� �� �κп� ���� ������ �������� 
	// ��� �� �ؿ� �� ������ŭ�� UV�� ���ؼ� �׸���.
	// �����ϰ� ���ؼ� �ؽ��Ŀ��� �� ������ ������ ������ �׸� ���̸� ����Ͽ� �׸���.
	//-------------------------------------------------------------------------------

	#define FURE_JARIMAGE_HEIGHT_PIXEL				63.0f
	#define JARIMAGE_BOTTOM_EMPTYSPACE_HEIGHT_PIXEL	23.0f

	if( NULL == m_pUnSealedElldinsJar )
		return false;

	 if( NULL == m_pUnSealedElldinsJar->m_nHitPoint )
		 return false;

	PT_ITEM_SPEC pItemSpec = m_pUnSealedElldinsJar->GetProp();
	FLERROR_ASSERT_LOG_RETURN( pItemSpec, false, _T("[������ �׾Ƹ�] ������ �Ӽ� �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

	FLERROR_ASSERT_LOG_RETURN( pItemSpec->nAdjParamVal[0], false, _T("[������ �׾Ƹ�] �ִ� ���差�� 0 �Դϴ�. �����͸� Ȯ���ϼ���.") );

 	float fCurrentSavedHPPercent = static_cast<float>( ( m_pUnSealedElldinsJar->m_nHitPoint * 100.0f ) / pItemSpec->nAdjParamVal[0] );

	float fCalcFureJarImageHeightPixel = static_cast<float>( ( ( FURE_JARIMAGE_HEIGHT_PIXEL * fCurrentSavedHPPercent ) * 0.01f ) + JARIMAGE_BOTTOM_EMPTYSPACE_HEIGHT_PIXEL ); 

	float fTopUVCoord	= ( 1.0f - (fCalcFureJarImageHeightPixel / m_pFillTexture->m_size.cy) );
	float fTopCoord		= m_pFillTexture->m_size.cy - fCalcFureJarImageHeightPixel;

	p2DRender->RenderTextureUVEx(	CPoint( m_Rect[eSlot::eSlot_JarImage].left,	static_cast<int>( m_Rect[eSlot::eSlot_JarImage].top + fTopCoord ) ),
									CPoint( m_pFillTexture->m_size.cx,			static_cast<int>( fCalcFureJarImageHeightPixel ) ), 
									m_pFillTexture,
									0.0f,
									fTopUVCoord,
									1.0f,
									1.0f
								 );

	return true;
}



//---------------------------------------------------------------------------------------------
// OK ��ư�� Ȱ��ȭ �� ���ΰ� üũ�Ѵ�.
// param	: ������
// return	: void
//---------------------------------------------------------------------------------------------
void CWndElldinsJar::CheckEnableOKButton()
{
	FLERROR_ASSERT_LOG_RETURN_VOID( m_pWndButtonOK, _T("[������ �׾Ƹ�] OK ��ư ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

	if( NULL == m_pUnSealedElldinsJar )
	{
		m_pWndButtonOK->EnableWindow( FALSE );
		return ;
	}

	if( NULL == m_p1stPotion && NULL == m_p2ndPotion )
	{
		m_pWndButtonOK->EnableWindow( FALSE );
		return ;
	}
 
	m_pWndButtonOK->EnableWindow( TRUE );
}


//---------------------------------------------------------------------------------------------
// OK ��ư Ŭ�� �� �̺�Ʈ ó��
// param	: ������
// return	: void
//---------------------------------------------------------------------------------------------
bool CWndElldinsJar::ButtonOK_Process()
{
	FLERROR_ASSERT_LOG_RETURN( m_pUnSealedElldinsJar, false, _T("[������ �׾Ƹ�] �������� ��Ȳ�� �ƴմϴ� ( �׾Ƹ� ���� ).") );

	if( NULL == m_p1stPotion &&
		NULL == m_p2ndPotion )
		FLERROR_ASSERT_LOG_RETURN( NULL, false, _T("[������ �׾Ƹ�] �������� ��Ȳ�� �ƴմϴ� ( ������ �� �� ���� ).") );

	if( m_p1stPotion )
		g_DPlay.SendFillElldinsJarWithPotion( m_p1stPotion->m_dwObjId, m_pUnSealedElldinsJar->m_dwObjId );	

	if( m_p2ndPotion )
		g_DPlay.SendFillElldinsJarWithPotion( m_p2ndPotion->m_dwObjId, m_pUnSealedElldinsJar->m_dwObjId );	

	return true;
}


//---------------------------------------------------------------------------------------------
// �׸���
// param	: ������
// return	: void
//---------------------------------------------------------------------------------------------
void CWndElldinsJar::OnDraw(C2DRender* p2DRender)
{
	FLERROR_ASSERT_LOG_RETURN_VOID( p2DRender, _T("[������ �׾Ƹ�] ������ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

	CheckEnableOKButton();
	
	if( NULL != m_dwUnSealElldinsJarID )
	{
		FindRegisterUnSealElldinsJar( m_dwUnSealElldinsJarID );
		g_pPlayer->SetRegisterElldinsJarID( m_dwUnSealElldinsJarID );
	}
	if( NULL != m_dw1stPostionID)
		FindRegister1stPostion( m_dw1stPostionID );
	if( NULL != m_dw2ndPostionID)
		FindRegister2ndPostion( m_dw2ndPostionID );

	CheckItemFromInventory( eSlot::eSlot_Jar );
	CheckItemFromInventory( eSlot::eSlot_1stPotion );
	CheckItemFromInventory( eSlot::eSlot_2ndPotion );

	DrawFillTexture( p2DRender );
	DrawTextHP( p2DRender ); 

	DrawIconInSlot( p2DRender, m_pUnSealedElldinsJar, eSlot::eSlot_Jar );
	DrawIconInSlot( p2DRender, m_p1stPotion,eSlot::eSlot_1stPotion );
	DrawIconInSlot( p2DRender, m_p2ndPotion, eSlot::eSlot_2ndPotion );

	DrawTooltip( p2DRender, m_pUnSealedElldinsJar, eSlot::eSlot_Jar );
	DrawTooltip( p2DRender, m_p1stPotion,eSlot::eSlot_1stPotion );
	DrawTooltip( p2DRender, m_p2ndPotion, eSlot::eSlot_2ndPotion );
}


//---------------------------------------------------------------------------------------------
// ����� ���� ���� �̺�Ʈ ó��
// param	: ������ ���̵�, �޽���, �⺻ ������
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndElldinsJar::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase )
{
	if( WIDC_BUTTON1 == nID )	// OK ��ư
		ButtonOK_Process();

	return CWndBase::OnCommand( nID, dwMessage, pWndBase );
}


//---------------------------------------------------------------------------------------------
// �������� ��� ���� ���� �̺�Ʈ ó��
// param	: pShortcut, ���콺 ��ġ
// return	: BOOL
//---------------------------------------------------------------------------------------------
BOOL CWndElldinsJar::OnDropIcon( LPSHORTCUT pShortcut, CPoint point )
{
	FLERROR_ASSERT_LOG_RETURN( pShortcut, FALSE, _T("[������ �׾Ƹ�] ���� �������� �����Ͱ� ��ȿ���� �ʽ��ϴ�."))

	CWndBase* pWndFrame =  pShortcut->m_pFromWnd->GetFrameWnd();
	FLERROR_ASSERT_LOG_RETURN( pWndFrame, FALSE, _T("[������ �׾Ƹ�] ������ �������� �����Ͱ� ��ȿ���� �ʽ��ϴ�."))

	if( pShortcut->m_dwShortcut != SHORTCUT_ITEM &&
		pWndFrame->GetWndId() != APP_INVENTORY )
		return FALSE;
		
	FLItemElem* pItemElem = static_cast<FLItemElem*>( g_pPlayer->GetItemId( pShortcut->m_dwId ) );
	FLERROR_ASSERT_LOG_RETURN( pItemElem, FALSE, _T("[������ �׾Ƹ�] ���� ������ �������� �����Ͱ� ��ȿ���� �ʽ��ϴ�."))

	bool bResult( true );

	if( PtInRect(&m_Rect[eSlot::eSlot_Jar], point) )
		bResult = IsUnSealElldinsJar( pShortcut->m_dwId );

	if( false == bResult )
	{
		g_WndMng.PutString( prj.GetText(TID_MMI_ELLDINPOTION_TEXT02), NULL, prj.GetTextColor(TID_MMI_ELLDINPOTION_TEXT02) );
		CheckEnableOKButton();
		return bResult;
	}

	if( PtInRect(&m_Rect[eSlot::eSlot_1stPotion], point) )
		bResult = IsPossibleRegisterElldinsJarPotion( pShortcut->m_dwId, eSlot::eSlot_1stPotion );

	if( PtInRect(&m_Rect[eSlot::eSlot_2ndPotion], point) )
		bResult = IsPossibleRegisterElldinsJarPotion( pShortcut->m_dwId, eSlot::eSlot_2ndPotion );

	if( false == bResult )
	{
		g_WndMng.PutString( prj.GetText(TID_MMI_ELLDINPOTION_TEXT08), NULL, prj.GetTextColor(TID_MMI_ELLDINPOTION_TEXT08) );
	}

	CheckEnableOKButton();
	return TRUE;
}


#pragma warning( default : 4482 )

