#include "StdAfx.h"
#include "FLWndCostume.h"

//-------------------------------------------------------------------------------------------------
//				�ڽ�Ƭ�޴� 
//-------------------------------------------------------------------------------------------------
CFLWndCostume_NPC::CFLWndCostume_NPC()
{
	m_pCostume_Mix = NULL;
}
//-------------------------------------------------------------------------------------------------
CFLWndCostume_NPC::~CFLWndCostume_NPC()
{
	SAFE_DELETE(m_pCostume_Mix);
}
CWndNeuz* CFLWndCostume_NPC::CreateWnd	( COSTUME_WND  eWnd)
{
	switch(eWnd)
	{
	case COSTUME_BUY:
		return NULL;
	case COSTUME_SELL:
		return NULL;
	case COSTUME_MIX:
		SAFE_DELETE(m_pCostume_Mix);
		m_pCostume_Mix = new CFLWndCostume_Mix;
		m_pCostume_Mix->Initialize();
		return m_pCostume_Mix;
	}
	return NULL;
}
void CFLWndCostume_NPC::OnRButtonDown  ( UINT nFlags, CPoint point )
{
	if(m_pCostume_Mix != NULL)
	{
		m_pCostume_Mix->OnRButtonDown( nFlags, point );
	}
}

void CFLWndCostume_NPC::ReceivedPacket( COSTUME_WND  eWnd , BOOL bResult )
{
	switch(eWnd)
	{
	case COSTUME_BUY:
		break;
	case COSTUME_SELL:
		break;
	case COSTUME_MIX:
		if(m_pCostume_Mix)
		{
			m_pCostume_Mix->ReceivedPacket(bResult);
		}
		break;
	}
}


//-------------------------------------------------------------------------------------------------
//       18�� : �ڽ�Ƭ �ռ� 
//-------------------------------------------------------------------------------------------------
CFLWndCostume_Mix::CFLWndCostume_Mix()
{
	m_pItemElem = NULL;
	memset( m_objID_Slot_A, NULL_ID, sizeof( m_objID_Slot_A ) );
	memset( m_objID_Slot_B, NULL_ID, sizeof( m_objID_Slot_B ) );
	memset( m_objID_Slot_Result, NULL_ID, sizeof( m_objID_Slot_Result ) );
	memset( m_nSlotInput, ERROR_SLOT, sizeof( m_nSlotInput ) );
}
//-------------------------------------------------------------------------------------------------
CFLWndCostume_Mix::~CFLWndCostume_Mix()
{	
}

//-------------------------------------------------------------------------------------------------
BOOL CFLWndCostume_Mix::Initialize		( CWndBase* pWndParent, DWORD nType)
{
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_COSTUME_MIXMENU, 0, CPoint( 0, 0 ), pWndParent );
}
//-------------------------------------------------------------------------------------------------
BOOL CFLWndCostume_Mix::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult )
{
	switch(nID)
	{
	case WIDC_BT_CANCEL:
		Destroy();
		break;
	case WIDC_BT_START:
		OnStart();
		break;
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
}

//-------------------------------------------------------------------------------------------------
void CFLWndCostume_Mix::OnStart()
{
	//�ڽ�Ƭ �ռ� �ı� ���� �ֹ����� ����ϰ� ���� ������ ��� �޼��� ��� 
	if(g_pPlayer->HasBuffByIk3( IK3_COSTUME_DEFENDER ) == FALSE)
	{
		//������ üũ Ȯ�� �޼���
		g_WndMng.OpenMessageBox( _T( prj.GetText(TID_MMI_COSTUME_SAFEMIXITEM01) ) ,MB_OKCANCEL, this);
	}
	else
	{
		// �ռ� ����
		SendPacket();
	}
}

//-------------------------------------------------------------------------------------------------
BOOL CFLWndCostume_Mix::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase )
{
	switch(nID)
	{
	case IDOK:// �ռ� ����
		SendPacket();
		break;
	case IDCANCEL:
		Destroy();
		break;
	}
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
}
//-------------------------------------------------------------------------------------------------
BOOL CFLWndCostume_Mix::SendPacket( )
{
	T_PACKET_ITEM_MERGE tCostume_Merge = { 0 };
	//::mem_set( &tCostume_Merge, 0, sizeof( tCostume_Merge ) );

	tCostume_Merge.dwMergeCount = N_MAX;		
	tCostume_Merge.dwMergeType	= MERGE_RANDOMOPTION;
	for( int nInfo = 0 ; nInfo < N_MAX ; nInfo++ )
	{
		tCostume_Merge.tItemMergeInfo[nInfo].dwObjIdSrc = m_objID_Slot_A[nInfo];
		tCostume_Merge.tItemMergeInfo[nInfo].dwObjIdDest = m_objID_Slot_B[nInfo];
	}

	g_DPlay.SendReqItemMerge( &tCostume_Merge );
	
	//gmpbigsun(20111208) ��Ÿ����
	EnableChildWnd( WIDC_BUTTON1, FALSE );		

	return	TRUE;
}

void CFLWndCostume_Mix::ReceivedPacket(BOOL bResult)
{
	if(bResult)//Result is TRUE
	{
		//�ռ������� ASlot�� �ִ°����� ResultSlot�� �ű��.
		for(int nSlot = 0; nSlot < N_MAX; nSlot++)
		{
			m_objID_Slot_Result[nSlot] = m_objID_Slot_A[nSlot]; 
		}
		//BSlot�� �����ش�.
		memset( m_objID_Slot_B, NULL_ID, sizeof( m_objID_Slot_B ) );
		memset( m_nSlotInput, ERROR_SLOT, sizeof( m_nSlotInput ) );
		CWndButton* pBtn = ( CWndButton* )GetDlgItem( WIDC_BUTTON1 );
		if( pBtn )
			pBtn->EnableWindow( FALSE );
	}
	else
	{
		//�����ϸ� ������ ����� 
		memset( m_objID_Slot_A, NULL_ID, sizeof( m_objID_Slot_A ) );
		memset( m_objID_Slot_B, NULL_ID, sizeof( m_objID_Slot_B ) );
		memset( m_objID_Slot_Result, NULL_ID, sizeof( m_objID_Slot_Result ) );
		memset( m_nSlotInput, ERROR_SLOT, sizeof( m_nSlotInput ) );
		CWndButton* pBtn = ( CWndButton* )GetDlgItem( WIDC_BUTTON1 );
		if( pBtn )
			pBtn->EnableWindow( FALSE );
	}

	//gmpbigsun(20111208) ��Ÿ����
	EnableChildWnd( WIDC_BUTTON1, TRUE );
}
void CFLWndCostume_Mix::EnableChildWnd( DWORD dwWndId, BOOL bEnable )
{
	CWndBase* pWnd = GetDlgItem( dwWndId );
	if( pWnd )
		pWnd->EnableWindow( bEnable );
}

void CFLWndCostume_Mix::OnDestroy( void )
{


}

//-------------------------------------------------------------------------------------------------
void CFLWndCostume_Mix::OnDraw ( C2DRender* p2DRender )
{
	for( int i = 0; i < N_MAX; ++i )
	{
		if( m_objID_Slot_A[ i ] != NULL_ID )
		{
			FLItemElem* pItem;
			pItem = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[ i ] );

			if( IsUsableItem( pItem ) )
				p2DRender->RenderTexture( m_rect_Slot_A[i].TopLeft(), pItem->GetTexture() );
		}
		if( m_objID_Slot_B[ i ] != NULL_ID )
		{
			FLItemElem* pItem;
			pItem = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_B[ i ] );

			if( IsUsableItem( pItem ) )
				p2DRender->RenderTexture( m_rect_Slot_B[i].TopLeft(), pItem->GetTexture() );
		}
		if(m_objID_Slot_Result[i] != NULL_ID)
		{
			FLItemElem* pItem;
			pItem = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_Result[ i ] );

			if( IsUsableItem( pItem ) )
				p2DRender->RenderTexture( m_rect_Slot_Result[i].TopLeft(), pItem->GetTexture() );
		}
	}
}
//-------------------------------------------------------------------------------------------------
void CFLWndCostume_Mix::OnInitialUpdate ( )
{
	CWndNeuz::OnInitialUpdate();

	if(GetWndBase( APP_BAG_EX )) 
		GetWndBase( APP_BAG_EX )->Destroy();

	//Slot Control Setting ----------------------------------------------------------
	CWndStatic* pWndCostumeHat_A	= (CWndStatic*)GetDlgItem( WIDC_ST_HAT_A );
	CWndStatic* pWndCostumeCloth_A	= (CWndStatic*)GetDlgItem( WIDC_ST_CLOTH_A );
	CWndStatic* pWndCostumeGlove_A	= (CWndStatic*)GetDlgItem( WIDC_ST_GLOVE_A );
	CWndStatic* pWndCostumeShoes_A	= (CWndStatic*)GetDlgItem( WIDC_ST_SHOES_A );

	CWndStatic* pWndCostumeHat_B	= (CWndStatic*)GetDlgItem( WIDC_ST_HAT_B );
	CWndStatic* pWndCostumeCloth_B	= (CWndStatic*)GetDlgItem( WIDC_ST_CLOTH_B );
	CWndStatic* pWndCostumeGlove_B	= (CWndStatic*)GetDlgItem( WIDC_ST_GLOVE_B );
	CWndStatic* pWndCostumeShoes_B	= (CWndStatic*)GetDlgItem( WIDC_ST_SHOES_B );

	CWndStatic* pWndCostume_Result_Hat		= (CWndStatic*)GetDlgItem( WIDC_ST_RESULT_HAT );
	CWndStatic* pWndCostume_Result_Cloth	= (CWndStatic*)GetDlgItem( WIDC_ST_RESULT_CLOTH );
	CWndStatic* pWndCostume_Result_Glove	= (CWndStatic*)GetDlgItem( WIDC_ST_RESULT_GLOVE );
	CWndStatic* pWndCostume_Result_Shoes	= (CWndStatic*)GetDlgItem( WIDC_ST_RESULT_SHOES );

	if( !( pWndCostumeHat_A && 
		pWndCostumeCloth_A && 
		pWndCostumeGlove_A && 
		pWndCostumeShoes_A &&
		pWndCostumeHat_B && 
		pWndCostumeCloth_B && 
		pWndCostumeGlove_B && 
		pWndCostumeShoes_B &&
		pWndCostume_Result_Hat &&
		pWndCostume_Result_Cloth && 
		pWndCostume_Result_Glove &&
		pWndCostume_Result_Shoes ) )
	{	
		FLERROR_LOG( PROGRAM_NAME, _T( "CFLWndCostume_Mix::OnInitialUpdate()" ) );
	}
	//Group_A
	m_rect_Slot_A[0] = pWndCostumeHat_A->GetWndRect();
	m_rect_Slot_A[1]= pWndCostumeCloth_A->GetWndRect();
	m_rect_Slot_A[2] = pWndCostumeGlove_A->GetWndRect();
	m_rect_Slot_A[3] = pWndCostumeShoes_A->GetWndRect();

	//Group_B
	m_rect_Slot_B[0] = pWndCostumeHat_B->GetWndRect();
	m_rect_Slot_B[1] = pWndCostumeCloth_B->GetWndRect();
	m_rect_Slot_B[2] = pWndCostumeGlove_B->GetWndRect();
	m_rect_Slot_B[3] = pWndCostumeShoes_B->GetWndRect();

	//Group_Result
	m_rect_Slot_Result[0] = pWndCostume_Result_Hat->GetWndRect();
	m_rect_Slot_Result[1] = pWndCostume_Result_Cloth->GetWndRect();
	m_rect_Slot_Result[2] = pWndCostume_Result_Glove->GetWndRect();
	m_rect_Slot_Result[3] = pWndCostume_Result_Shoes->GetWndRect();
	//--------------------------------------------------------------------------------

	//Window Default Position Setting  -----------------------------------------------
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
	//--------------------------------------------------------------------------------

	m_pItemElem = NULL;
	memset( m_objID_Slot_A, NULL_ID, sizeof( m_objID_Slot_A ) );
	memset( m_objID_Slot_B, NULL_ID, sizeof( m_objID_Slot_B ) );
	memset( m_objID_Slot_Result, NULL_ID, sizeof( m_objID_Slot_Result ) );
	memset( m_nSlotInput, ERROR_SLOT, sizeof( m_nSlotInput ) );

	//Button Setting -----------------------------------------------------------------
	CWndButton* pBtnOK  = ( CWndButton* )GetDlgItem( WIDC_BT_START );
	if( !pBtnOK )
		FLERROR_LOG( PROGRAM_NAME, _T( "CFLWndCostume_Mix::OnInitialUpdate()" ) );
	pBtnOK->EnableWindow( FALSE );
	//--------------------------------------------------------------------------------
}
//--------------------------------------------------------------------------------
void CFLWndCostume_Mix::OnMouseWndSurface( CPoint point )
{
	FLItemElem* pItemElem = NULL;

	//���Կ� ���� ����
	for( int i = 0; i < N_MAX; ++i )
	{
		if(m_objID_Slot_Result[i] != NULL_ID)
		{
			//�ڽ�Ƭ�ռ��� �ϼ��Ǹ� ������Կ��� ������ �����ش�.
			if(m_rect_Slot_Result[i].PtInRect(point))
			{
				CPoint pointResult = point;
				CRect m_Rect_TempResult = m_rect_Slot_Result[i];

				ClientToScreen( &pointResult );
				ClientToScreen( &m_Rect_TempResult );

				pItemElem = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_Result[i] );
				if(pItemElem != NULL)
				{
					g_WndMng.PutToolTip_Item( pItemElem , pointResult, &m_Rect_TempResult);		
				}
			}
		}
		else
		{
			if(m_rect_Slot_A[i].PtInRect(point))
			{
				CPoint pointA = point;
				CRect m_Rect_TempA = m_rect_Slot_A[i];

				ClientToScreen( &pointA );
				ClientToScreen( &m_Rect_TempA );

				pItemElem = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[i] );
				if(pItemElem != NULL)
				{
					g_WndMng.PutToolTip_Item( pItemElem , pointA, &m_Rect_TempA);		
				}
			}
			if(m_rect_Slot_B[i].PtInRect(point))
			{
				CPoint pointB = point;
				CRect m_Rect_TempB = m_rect_Slot_B[i];

				ClientToScreen( &pointB );
				ClientToScreen( &m_Rect_TempB );

				pItemElem = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_B[i] );
				if(pItemElem != NULL)
				{
					g_WndMng.PutToolTip_Item( pItemElem , pointB, &m_Rect_TempB);		
				}
			}
		}		
	}
}

//--------------------------------------------------------------------------------
DWORD CFLWndCostume_Mix::IsCostume(PT_ITEM_SPEC pProp)
{
	switch( pProp->dwParts )
	{
	case PARTS_HAT:
		{
			if( pProp->dwItemKind1 == IK1_ARMOR && 
				pProp->dwItemKind2 == IK2_CLOTHETC &&
				pProp->dwItemKind3 == IK3_HAT )
			{
				return PARTS_HAT;
			}
			return ERROR_SLOT;
		}
	case PARTS_CLOTH:
		{
			if( pProp->dwItemKind1 == IK1_ARMOR && 
				pProp->dwItemKind2 == IK2_CLOTH &&
				pProp->dwItemKind3 == IK3_CLOTH )
			{
				return PARTS_CLOTH;
			}
			return ERROR_SLOT;
		}
	case PARTS_GLOVE:
		{
			if( pProp->dwItemKind1 == IK1_ARMOR && 
				pProp->dwItemKind2 == IK2_CLOTHETC &&
				pProp->dwItemKind3 == IK3_GLOVE )
			{
				return PARTS_GLOVE;
			}
			return ERROR_SLOT;
		}
	case PARTS_BOOTS:
		{
			if( pProp->dwItemKind1 == IK1_ARMOR && 
				pProp->dwItemKind2 == IK2_CLOTHETC &&
				pProp->dwItemKind3 == IK3_SHOES )
			{
				return PARTS_BOOTS;

			}
			return ERROR_SLOT;
		}
	}
	return ERROR_SLOT;
}
//-------------------------------------------------------------------------------------------------
void CFLWndCostume_Mix::OnRButtonDown  ( UINT nFlags, CPoint point )
{
	CWndBase* pWndFrame	= m_pWndFocus;
	if( !pWndFrame )
	{
		FLASSERT( 0 );
		return;
	}

	if( APP_INVENTORY != pWndFrame->GetWndId() )// �κ��丮 �����۸� ��ϰ����ϵ���!
	{
		return;
	}

	if( APP_BAG_EX == pWndFrame->GetWndId() )// �κ��丮 �����۸� ��ϰ����ϵ���!
	{
		g_WndMng.OpenMessageBox( prj.GetText(TID_BARUNAPEARCING_ERRORMSG) );
		return;
	}


	//check valid item
	FLItemElem* pItem;
	if(m_pItemElem == NULL) return;

	pItem = g_pPlayer->m_Inventory.GetAtId( m_pItemElem->m_dwObjId );
	for( int n = 0 ; n < N_MAX; n++ )
	{
		if( m_nSlotInput[n] == ERROR_SLOT ) 
		{
			m_nSlotInput[n] =  A_SLOT;
		}
	}
	if( !IsUsableItem( pItem ) )
		return;

	PT_ITEM_SPEC pProp = pItem->GetProp();
	if( NULL == pProp )
		return;

	DWORD dwPARTS = IsCostume(pProp);
	if(dwPARTS  == ERROR_SLOT)
	{
		//����� �� ���� �������Դϴ�.
		g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_REGISTERNOTITEM)));
		return;
	}
	FLItemElem* pItem_NextSlot = NULL;

	// ������ �ູ �ɼ��� ������ ���Ե���� ���� �ʰ� ó���� 
	if( pItem->IsSetRandomOptionExtension() == true )
	{
		FLItemElem* pItem_SlotA = NULL;

		switch(dwPARTS)
		{
		case PARTS_HAT:
			{
				if(m_nSlotInput[0] == A_SLOT)//A_Slot
				{
					if(m_objID_Slot_A[1] != NULL_ID)
					{
						pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[1] );
						//�ɼ� ���� ���� 
						if(pItem_NextSlot != NULL) 
						{
							if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )//1�� ������ �ɼ� ������ ���� 
							{
								if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
								{
									if( m_pItemElem->m_dwObjId != m_objID_Slot_B[0])// B���԰� ������ �������� �ƴ϶��
									{
										m_objID_Slot_A[0] = m_pItemElem->m_dwObjId;	//���Կ� ���
										m_nSlotInput[0] = B_SLOT;
									}
									else
									{
										g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_ITEMOVERLAP)));
									}
								}
								else
								{
									// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
									g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
									return;
								}
							}
							else
							{
								g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
							}
						}
					}
					else if(m_objID_Slot_A[2] != NULL_ID)
					{
						pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[2] );
						//�ɼ� ���� ���� 
						if(pItem_NextSlot != NULL) 
						{
							if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )//1�� ������ �ɼ� ������ ���� 
							{
								if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
								{
									if( m_pItemElem->m_dwObjId != m_objID_Slot_B[0])// B���԰� ������ �������� �ƴ϶��
									{
										m_objID_Slot_A[0] = m_pItemElem->m_dwObjId;	//���Կ� ���
										m_nSlotInput[0] = B_SLOT;
									}
									else
									{
										g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_ITEMOVERLAP)));
									}
								}
								else
								{
									// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
									g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
									return;
								}
							}
							else
							{
								g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
							}
						}
					}
					else if(m_objID_Slot_A[3] != NULL_ID)
					{
						pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[3] );
						//�ɼ� ���� ���� 
						if(pItem_NextSlot != NULL) 
						{
							if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )//1�� ������ �ɼ� ������ ���� 
							{
								if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
								{
									if( m_pItemElem->m_dwObjId != m_objID_Slot_B[0])// B���԰� ������ �������� �ƴ϶��
									{
										m_objID_Slot_A[0] = m_pItemElem->m_dwObjId;	//���Կ� ���
										m_nSlotInput[0] = B_SLOT;
									}
									else
									{
										g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_ITEMOVERLAP)));
									}
								}
								else
								{
									// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
									g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
									return;
								}
							}
							else
							{
								g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
							}
						}
					}
					else 
					{
						if( m_pItemElem->m_dwObjId != m_objID_Slot_B[0])// B���԰� ������ �������� �ƴ϶��
						{
							m_objID_Slot_A[0] = m_pItemElem->m_dwObjId;	//���Կ� ���
							m_nSlotInput[0] = B_SLOT;
						}
						else
						{
							g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_ITEMOVERLAP)));
						}
					}
				}
				else if(m_nSlotInput[0] == B_SLOT)//B_Slot
				{
					//�ɼ� ���� ���� 
					pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[0] );
					if(pItem_NextSlot == NULL) 
					{
						g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_UNCONDITIONALINCHANT02));
						return;
					}

					//�ɼ� ������ ASlot�� ���ƾ���,,, 
					if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )
					{
						if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
						{
							if( m_pItemElem->m_dwObjId != m_objID_Slot_A[0])// B���԰� ������ �������� �ƴ϶��
							{
								m_objID_Slot_B[0] = m_pItemElem->m_dwObjId;	//���Կ� ���
								m_nSlotInput[0] = A_SLOT;
							}
							else
							{
								g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_ITEMOVERLAP)));
							}
						}
						else
						{
							// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
							g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
							return;
						}
					}
					else
					{
						g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
					}
				}				
			}
			break;
		case PARTS_CLOTH:
			{
				if(m_nSlotInput[1] == A_SLOT)//A_Slot
				{
					if(m_objID_Slot_A[0] != NULL_ID)
					{
						pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[0] );
						//�ɼ� ���� ���� 
						if(pItem_NextSlot != NULL) 
						{
							if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )//1�� ������ �ɼ� ������ ���� 
							{
								if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
								{
									if( m_pItemElem->m_dwObjId != m_objID_Slot_B[1])// B���԰� ������ �������� �ƴ϶��
									{
										m_objID_Slot_A[1] = m_pItemElem->m_dwObjId;	//���Կ� ���
										m_nSlotInput[1] = B_SLOT;
									}
									else
									{
										g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_ITEMOVERLAP)));
									}
								}
								else
								{
									// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
									g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
									return;
								}
							}
							else
							{
								g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
							}
						}

					}
					else if(m_objID_Slot_A[2] != NULL_ID)
					{
						pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[2] );
						//�ɼ� ���� ���� 
						if(pItem_NextSlot != NULL) 
						{
							if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )//1�� ������ �ɼ� ������ ���� 
							{
								//gmpbigsun( 20111018, #19908 ) : pItem_SlotA => pItem_NextSlot ( �³� -_- )
								if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
								{
									if( m_pItemElem->m_dwObjId != m_objID_Slot_B[1])// B���԰� ������ �������� �ƴ϶��
									{
										m_objID_Slot_A[1] = m_pItemElem->m_dwObjId;	//���Կ� ���
										m_nSlotInput[1] = B_SLOT;
									}
									else
									{
										g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_ITEMOVERLAP)));
									}
								}
								else
								{
									// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
									g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
									return;
								}
							}
							else
							{
								g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
							}
						}

					}
					else if(m_objID_Slot_A[3] != NULL_ID)
					{
						pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[3] );
						//�ɼ� ���� ���� 
						if(pItem_NextSlot != NULL) 
						{
							if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )//1�� ������ �ɼ� ������ ���� 
							{
								if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
								{
									if( m_pItemElem->m_dwObjId != m_objID_Slot_B[1])// B���԰� ������ �������� �ƴ϶��
									{
										m_objID_Slot_A[1] = m_pItemElem->m_dwObjId;	//���Կ� ���
										m_nSlotInput[1] = B_SLOT;
									}
									else
									{
										g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_ITEMOVERLAP)));
									}
								}
								else
								{
									// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
									g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
									return;
								}
							}
							else
							{
								g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
							}
						}

					}
					else 
					{				
						if( m_pItemElem->m_dwObjId != m_objID_Slot_B[1])// B���԰� ������ �������� �ƴ϶��
						{
							m_objID_Slot_A[1] = m_pItemElem->m_dwObjId;	//���Կ� ���
							m_nSlotInput[1] = B_SLOT;
						}
						else
						{
							g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_ITEMOVERLAP)));
						}
					}
				}
				else if(m_nSlotInput[1] == B_SLOT)//B_Slot
				{
					//�ɼ� ���� ���� 
					pItem_SlotA = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[1] );
					if(pItem_SlotA == NULL) 
					{
						g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_UNCONDITIONALINCHANT02));
						return;
					}
					//�ɼ� ������ ASlot�� ���ƾ���,,, 
					if( pItem_SlotA->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )
					{
						if( pItem_SlotA->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
						{
							if( m_pItemElem->m_dwObjId != m_objID_Slot_A[1])// B���԰� ������ �������� �ƴ϶��
							{
								m_objID_Slot_B[1] = m_pItemElem->m_dwObjId;	//���Կ� ���
								m_nSlotInput[1] = A_SLOT;
							}
							else
							{
								g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_ITEMOVERLAP)));
							}
						}
						else
						{
							// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
							g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
							return;
						}
					}
					else
					{
						g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
					}
				}				
			}
			break;
		case PARTS_GLOVE:
			{
				if(m_nSlotInput[2] == A_SLOT)//A_Slot
				{
					if(m_objID_Slot_A[0] != NULL_ID)
					{
						pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[0] );
						//�ɼ� ���� ���� 
						if(pItem_NextSlot != NULL) 
						{
							if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )//1�� ������ �ɼ� ������ ���� 
							{
								if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
								{
									if( m_pItemElem->m_dwObjId != m_objID_Slot_B[2])// B���԰� ������ �������� �ƴ϶��
									{
										m_objID_Slot_A[2] = m_pItemElem->m_dwObjId;	//���Կ� ���
										m_nSlotInput[2] = B_SLOT;
									}
									else
									{
										g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_ITEMOVERLAP)));
									}
								}
								else
								{
									// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
									g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
									return;
								}
							}
							else
							{
								g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
							}
						}

					}
					else if(m_objID_Slot_A[1] != NULL_ID)
					{
						pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[1] );
						//�ɼ� ���� ���� 
						if(pItem_NextSlot != NULL) 
						{
							if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )//1�� ������ �ɼ� ������ ���� 
							{
								if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
								{
									if( m_pItemElem->m_dwObjId != m_objID_Slot_B[2])// B���԰� ������ �������� �ƴ϶��
									{
										m_objID_Slot_A[2] = m_pItemElem->m_dwObjId;	//���Կ� ���
										m_nSlotInput[2] = B_SLOT;
									}
									else
									{
										g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_ITEMOVERLAP)));
									}
								}
								else
								{
									// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..7
									g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
									return;
								}
							}
							else
							{
								g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
							}
						}

					}
					else if(m_objID_Slot_A[3] != NULL_ID)
					{
						pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[3] );
						//�ɼ� ���� ���� 
						if(pItem_NextSlot != NULL) 
						{
							if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )//1�� ������ �ɼ� ������ ���� 
							{
								if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
								{
									if( m_pItemElem->m_dwObjId != m_objID_Slot_B[2])// B���԰� ������ �������� �ƴ϶��
									{
										m_objID_Slot_A[2] = m_pItemElem->m_dwObjId;	//���Կ� ���
										m_nSlotInput[2] = B_SLOT;
									}
									else
									{
										g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_ITEMOVERLAP)));
									}
								}
								else
								{
									// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
									g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
									return;
								}
							}
							else
							{
								g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
							}
						}

					}
					else 
					{
						if( m_pItemElem->m_dwObjId != m_objID_Slot_B[2])// B���԰� ������ �������� �ƴ϶��
						{
							m_objID_Slot_A[2] = m_pItemElem->m_dwObjId;	//���Կ� ���
							m_nSlotInput[2] = B_SLOT;
						}
						else
						{
							g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_ITEMOVERLAP)));
						}
					}
				}
				else if(m_nSlotInput[2] == B_SLOT)//B_Slot
				{
					//�ɼ� ���� ���� 
					pItem_SlotA = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[2] );
					if(pItem_SlotA == NULL) 
					{
						g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_UNCONDITIONALINCHANT02));
						return;
					}
					//�ɼ� ������ ASlot�� ���ƾ���,,, 
					if( pItem_SlotA->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )
					{
						if( pItem_SlotA->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
						{
							if( m_pItemElem->m_dwObjId != m_objID_Slot_A[2])// B���԰� ������ �������� �ƴ϶��
							{
								m_objID_Slot_B[2] = m_pItemElem->m_dwObjId;	//���Կ� ���
								m_nSlotInput[2] = A_SLOT;
							}
							else
							{
								g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_ITEMOVERLAP)));
							}
						}
						else
						{
							// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
							g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
							return;
						}
					}
					else
					{
						g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
					}
				}				
			}
			break;
		case PARTS_BOOTS:
			{
				if(m_nSlotInput[3] == A_SLOT)//A_Slot
				{
					if(m_objID_Slot_A[0] != NULL_ID)
					{
						pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[0] );
						//�ɼ� ���� ���� 
						if(pItem_NextSlot != NULL) 
						{
							if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )//1�� ������ �ɼ� ������ ���� 
							{
								if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
								{
									if( m_pItemElem->m_dwObjId != m_objID_Slot_B[3])// B���԰� ������ �������� �ƴ϶��
									{
										m_objID_Slot_A[3] = m_pItemElem->m_dwObjId;	//���Կ� ���
										m_nSlotInput[3] = B_SLOT;
									}
									else
									{
										g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_ITEMOVERLAP)));
									}
								}
								else
								{
									// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
									g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
									return;
								}
							}
							else
							{
								g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
							}
						}
					}
					else if(m_objID_Slot_A[1] != NULL_ID)
					{
						pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[1] );
						//�ɼ� ���� ���� 
						if(pItem_NextSlot != NULL) 
						{
							if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )//1�� ������ �ɼ� ������ ���� 
							{
								if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
								{
									if( m_pItemElem->m_dwObjId != m_objID_Slot_B[3])// B���԰� ������ �������� �ƴ϶��
									{
										m_objID_Slot_A[3] = m_pItemElem->m_dwObjId;	//���Կ� ���
										m_nSlotInput[3] = B_SLOT;
									}
									else
									{
										g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_ITEMOVERLAP)));
									}
								}
								else
								{
									// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
									g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
									return;
								}
							}
							else
							{
								g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
							}
						}
					}
					else if(m_objID_Slot_A[2] != NULL_ID)
					{
						pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[2] );
						//�ɼ� ���� ���� 
						if(pItem_NextSlot != NULL) 
						{
							if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )//1�� ������ �ɼ� ������ ���� 
							{
								if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
								{
									if( m_pItemElem->m_dwObjId != m_objID_Slot_B[3])// B���԰� ������ �������� �ƴ϶��
									{
										m_objID_Slot_A[3] = m_pItemElem->m_dwObjId;	//���Կ� ���
										m_nSlotInput[3] = B_SLOT;
									}
									else
									{
										g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_ITEMOVERLAP)));
									}
								}
								else
								{
									// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
									g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
									return;
								}
							}
							else
							{
								g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
							}
						}
					}
					else 
					{
						if( m_pItemElem->m_dwObjId != m_objID_Slot_B[3])// B���԰� ������ �������� �ƴ϶��
						{
							m_objID_Slot_A[3] = m_pItemElem->m_dwObjId;	//���Կ� ���
							m_nSlotInput[3] = B_SLOT;
						}
						else
						{
							g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_ITEMOVERLAP)));
						}
					}
				}
				else if(m_nSlotInput[3] == B_SLOT)//B_Slot
				{
					//�ɼ� ���� ���� 
					pItem_SlotA = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[3] );
					if(pItem_SlotA == NULL) 
					{
						g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_UNCONDITIONALINCHANT02));
						return;
					}
					//�ɼ� ������ ASlot�� ���ƾ���,,, 
					if( pItem_SlotA->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )
					{
						if( pItem_SlotA->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
						{
							if( m_pItemElem->m_dwObjId != m_objID_Slot_A[3])// B���԰� ������ �������� �ƴ϶��
							{
								m_objID_Slot_B[3] = m_pItemElem->m_dwObjId;	//���Կ� ���
								m_nSlotInput[3] = A_SLOT;
							}
							else
							{
								g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_ITEMOVERLAP)));
							}
						}
						else
						{
							// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
							g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
							return;
						}
					}
					else
					{
						g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
					}
				}				
			}
			break;
		}
	}
	else
	{
		g_WndMng.OpenMessageBox( _T(prj.GetText(TID_MMI_COSTUME_UNCONDITIONALINCHANT)));
		return;
	}

	PLAYSND(SND_INF_EQUIPACCESSORY, &g_pPlayer->GetPos());
	if( m_objID_Slot_A[0] != NULL_ID && 
		m_objID_Slot_A[1] != NULL_ID &&
		m_objID_Slot_A[2] != NULL_ID &&
		m_objID_Slot_A[3] != NULL_ID &&
		m_objID_Slot_B[0] != NULL_ID && 
		m_objID_Slot_B[1] != NULL_ID &&
		m_objID_Slot_B[2] != NULL_ID &&
		m_objID_Slot_B[3] != NULL_ID
		)
	{
		CWndButton* pBtnOK  = ( CWndButton* )GetDlgItem( WIDC_BT_START );
		if( pBtnOK )
			pBtnOK->EnableWindow( TRUE );
	}
	else 
		return;
}

//-------------------------------------------------------------------------------------------------
BOOL CFLWndCostume_Mix::OnDropIcon		( LPSHORTCUT pShortcut, CPoint point )
{
	CWndBase* pWndFrame	= pShortcut->m_pFromWnd->GetFrameWnd();
	if( !pWndFrame )
	{
		FLASSERT( 0 );
		return FALSE;
	}

	// �κ����� �� �����۸� ���
	if( APP_INVENTORY != pWndFrame->GetWndId( ) )			
	{
		g_WndMng.OpenMessageBox( prj.GetText(TID_BARUNAPEARCING_ERRORMSG) );
		return FALSE;
	}

	//check valid item
	FLItemElem* pItem;
	pItem = g_pPlayer->m_Inventory.GetAtId( pShortcut->m_dwId );

	if(pItem == NULL) return FALSE;
	if( !IsUsableItem( pItem ) )
		return FALSE;

	PT_ITEM_SPEC pProp = pItem->GetProp();
	if( NULL == pProp )
		return FALSE;

	DWORD dwPARTS = IsCostume(pProp);
	if(dwPARTS == ERROR_SLOT)
	{
		//����� �� ���� �������Դϴ�.
		g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_REGISTERNOTITEM)));
		return FALSE;
	}
	FLItemElem* pItem_NextSlot = NULL;

	// ������ �ູ �ɼ��� ������ ���Ե���� ���� �ʰ� ó����
	if( pItem->IsSetRandomOptionExtension() == true )
	{
		//Group_A-----------------------------------------------------------------------------------
		if( PtInRect( &m_rect_Slot_A[0], point ) ) //����_A
		{
			if(dwPARTS == PARTS_HAT)
			{
				if(m_objID_Slot_A[1] != NULL_ID)
				{
					pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[1] );
					//�ɼ� ���� ���� 
					if(pItem_NextSlot != NULL) 
					{
						if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )//1�� ������ �ɼ� ������ ���� 
						{
							if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
							{
								if( pShortcut->m_dwId != m_objID_Slot_B[0])// B���԰� ������ �������� �ƴ϶��
								{ 
									m_objID_Slot_A[0] = pShortcut->m_dwId;	//���Կ� ���
								}
								else
								{
									//�̹� ��ϵǾ� �־� �ߺ� ����� �Ұ��� �մϴ�.
									g_WndMng.OpenMessageBox( prj.GetText(TID_COSTUME_ITEMOVERLAP));
								}
							}
							else
							{
								// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
								g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
							}
						}
						else
						{
							g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
						}
					}

				}
				else if(m_objID_Slot_A[2] != NULL_ID)
				{
					pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[2] );

					//�ɼ� ���� ���� 
					if(pItem_NextSlot != NULL) 
					{
						if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )//1�� ������ �ɼ� ������ ���� 
						{
							if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
							{
								if( pShortcut->m_dwId != m_objID_Slot_B[0])// B���԰� ������ �������� �ƴ϶��
								{ 
									m_objID_Slot_A[0] = pShortcut->m_dwId;	//���Կ� ���
								}
								else
								{
									//�̹� ��ϵǾ� �־� �ߺ� ����� �Ұ��� �մϴ�.
									g_WndMng.OpenMessageBox( prj.GetText(TID_COSTUME_ITEMOVERLAP));
								}
							}
							else
							{
								// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
								g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
							}
						}
						else
						{
							g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
						}
					}
				}
				else if(m_objID_Slot_A[3] != NULL_ID)
				{
					pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[3] );
					//�ɼ� ���� ���� 
					if(pItem_NextSlot != NULL) 
					{
						if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )//1�� ������ �ɼ� ������ ���� 
						{
							if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
							{
								if( pShortcut->m_dwId != m_objID_Slot_B[0])// B���԰� ������ �������� �ƴ϶��
								{ 
									m_objID_Slot_A[0] = pShortcut->m_dwId;	//���Կ� ���
								}
								else
								{
									//�̹� ��ϵǾ� �־� �ߺ� ����� �Ұ��� �մϴ�.
									g_WndMng.OpenMessageBox( prj.GetText(TID_COSTUME_ITEMOVERLAP));
								}
							}
							else
							{
								// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
								g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
							}
						}
						else
						{
							g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
						}
					}
				}			
				else
				{
					if( pShortcut->m_dwId != m_objID_Slot_B[0])// B���԰� ������ �������� �ƴ϶��
					{ 
						m_objID_Slot_A[0] = pShortcut->m_dwId;	//���Կ� ���
					}
					else
					{
						//�̹� ��ϵǾ� �־� �ߺ� ����� �Ұ��� �մϴ�.
						g_WndMng.OpenMessageBox( prj.GetText(TID_COSTUME_ITEMOVERLAP));
					}
				}
			}
			else 
			{
				//��� ������ ������ �ƴմϴ�.
				g_WndMng.OpenMessageBox( prj.GetText(TID_COSTUME_REGISTERNOTSLOT));
			}
		}
		else if( PtInRect( &m_rect_Slot_A[1], point ) )//�ǻ�_A
		{
			if(dwPARTS == PARTS_CLOTH)
			{
				if(m_objID_Slot_A[0] != NULL_ID)
				{
					pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[0] );
					//�ɼ� ���� ���� 
					if(pItem_NextSlot != NULL) 
					{
						if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )//1�� ������ �ɼ� ������ ���� 
						{
							if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
							{
								if( pShortcut->m_dwId != m_objID_Slot_B[1])// B���԰� ������ �������� �ƴ϶��
								{
									m_objID_Slot_A[1] = pShortcut->m_dwId; //���Կ� ���
								}
								else
								{
									//�̹� ��ϵǾ� �־� �ߺ� ����� �Ұ��� �մϴ�.
									g_WndMng.OpenMessageBox( prj.GetText(TID_COSTUME_ITEMOVERLAP));
								}
							}
							else
							{
								// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
								g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
							}
						}
						else
						{
							g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
						}
					}
				}
				else if(m_objID_Slot_A[2] != NULL_ID)
				{
					pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[2] );
					//�ɼ� ���� ���� 
					if(pItem_NextSlot != NULL) 
					{
						if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )//1�� ������ �ɼ� ������ ���� 
						{
							if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
							{
								if( pShortcut->m_dwId != m_objID_Slot_B[1])// B���԰� ������ �������� �ƴ϶��
								{
									m_objID_Slot_A[1] = pShortcut->m_dwId; //���Կ� ���
								}
								else
								{
									//�̹� ��ϵǾ� �־� �ߺ� ����� �Ұ��� �մϴ�.
									g_WndMng.OpenMessageBox( prj.GetText(TID_COSTUME_ITEMOVERLAP));
								}
							}
							else
							{
								// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
								g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
							}
						}
						else
						{
							g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
						}
					}
				}
				else if(m_objID_Slot_A[3] != NULL_ID)
				{
					pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[3] );
					//�ɼ� ���� ���� 
					if(pItem_NextSlot != NULL) 
					{
						if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )//1�� ������ �ɼ� ������ ���� 
						{
							if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
							{
								if( pShortcut->m_dwId != m_objID_Slot_B[1])// B���԰� ������ �������� �ƴ϶��
								{
									m_objID_Slot_A[1] = pShortcut->m_dwId; //���Կ� ���
								}
								else
								{
									//�̹� ��ϵǾ� �־� �ߺ� ����� �Ұ��� �մϴ�.
									g_WndMng.OpenMessageBox( prj.GetText(TID_COSTUME_ITEMOVERLAP));
								}
							}
							else
							{
								// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
								g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
							}
						}
						else
						{
							g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
						}
					}
				}
				else
				{
					if( pShortcut->m_dwId != m_objID_Slot_B[1])// B���԰� ������ �������� �ƴ϶��
					{ 
						m_objID_Slot_A[1] = pShortcut->m_dwId;	//���Կ� ���
					}
					else
					{
						//�̹� ��ϵǾ� �־� �ߺ� ����� �Ұ��� �մϴ�.
						g_WndMng.OpenMessageBox( prj.GetText(TID_COSTUME_ITEMOVERLAP));
					}
				}
			}
			else 
			{
				//��� ������ ������ �ƴմϴ�.
				g_WndMng.OpenMessageBox( prj.GetText(TID_COSTUME_REGISTERNOTSLOT));
			}
		}
		else if( PtInRect( &m_rect_Slot_A[2], point ) )//�尩_A
		{
			if(dwPARTS == PARTS_GLOVE)
			{
				if(m_objID_Slot_A[0] != NULL_ID)
				{
					pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[0] );
					//�ɼ� ���� ���� 
					if(pItem_NextSlot != NULL) 
					{
						if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )//1�� ������ �ɼ� ������ ���� 
						{
							if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
							{
								if( pShortcut->m_dwId != m_objID_Slot_B[2])// B���԰� ������ �������� �ƴ϶��
								{
									m_objID_Slot_A[2] = pShortcut->m_dwId; //���Կ� ���
								}
								else
								{
									//�̹� ��ϵǾ� �־� �ߺ� ����� �Ұ��� �մϴ�.
									g_WndMng.OpenMessageBox( prj.GetText(TID_COSTUME_ITEMOVERLAP));
								}
							}
							else
							{
								// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
								g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
							}
						}
						else
						{
							g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
						}
					}
				}
				else if(m_objID_Slot_A[1] != NULL_ID)
				{
					pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[1] );
					//�ɼ� ���� ���� 
					if(pItem_NextSlot != NULL) 
					{
						if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )//1�� ������ �ɼ� ������ ���� 
						{
							if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
							{
								if( pShortcut->m_dwId != m_objID_Slot_B[2])// B���԰� ������ �������� �ƴ϶��
								{
									m_objID_Slot_A[2] = pShortcut->m_dwId; //���Կ� ���
								}
								else
								{
									//�̹� ��ϵǾ� �־� �ߺ� ����� �Ұ��� �մϴ�.
									g_WndMng.OpenMessageBox( prj.GetText(TID_COSTUME_ITEMOVERLAP));
								}
							}
							else
							{
								// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
								g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
							}
						}
						else
						{
							g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
						}
					}
				}
				else if(m_objID_Slot_A[3] != NULL_ID)
				{
					pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[3] );
					//�ɼ� ���� ���� 
					if(pItem_NextSlot != NULL) 
					{
						if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )//1�� ������ �ɼ� ������ ���� 
						{
							if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
							{
								if( pShortcut->m_dwId != m_objID_Slot_B[2])// B���԰� ������ �������� �ƴ϶��
								{
									m_objID_Slot_A[2] = pShortcut->m_dwId; //���Կ� ���
								}
								else
								{
									//�̹� ��ϵǾ� �־� �ߺ� ����� �Ұ��� �մϴ�.
									g_WndMng.OpenMessageBox( prj.GetText(TID_COSTUME_ITEMOVERLAP));
								}
							}
							else
							{
								// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
								g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
							}
						}
						else
						{
							g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
						}
					}
				}
				else 
				{
					if( pShortcut->m_dwId != m_objID_Slot_B[2])// B���԰� ������ �������� �ƴ϶��
					{ 
						m_objID_Slot_A[2] = pShortcut->m_dwId;	//���Կ� ���
					}
					else
					{
						//�̹� ��ϵǾ� �־� �ߺ� ����� �Ұ��� �մϴ�.
						g_WndMng.OpenMessageBox( prj.GetText(TID_COSTUME_ITEMOVERLAP));
					}
				}				
			}
			else
			{
				//��� ������ ������ �ƴմϴ�.
				g_WndMng.OpenMessageBox( prj.GetText(TID_COSTUME_REGISTERNOTSLOT));
			}
		}
		else if( PtInRect( &m_rect_Slot_A[3], point ) )//�Ź�_A
		{
			if(dwPARTS == PARTS_BOOTS)
			{
				if(m_objID_Slot_A[0] != NULL_ID)
				{
					pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[0] );
					//�ɼ� ���� ���� 
					if(pItem_NextSlot != NULL) 
					{
						if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )//1�� ������ �ɼ� ������ ���� 
						{
							if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
							{
								if( pShortcut->m_dwId != m_objID_Slot_B[3])// B���԰� ������ �������� �ƴ϶��
								{
									m_objID_Slot_A[3] = pShortcut->m_dwId; //���Կ� ���
								}
								else
								{
									//�̹� ��ϵǾ� �־� �ߺ� ����� �Ұ��� �մϴ�.
									g_WndMng.OpenMessageBox( prj.GetText(TID_COSTUME_ITEMOVERLAP));
								}
							}
							else
							{
								// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
								g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
							}
						}
						else
						{
							g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
						}
					}
				}
				else if(m_objID_Slot_A[1] != NULL_ID)
				{
					pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[1] );
					//�ɼ� ���� ���� 
					if(pItem_NextSlot != NULL) 
					{
						if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )//1�� ������ �ɼ� ������ ���� 
						{
							if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
							{
								if( pShortcut->m_dwId != m_objID_Slot_B[3])// B���԰� ������ �������� �ƴ϶��
								{
									m_objID_Slot_A[3] = pShortcut->m_dwId; //���Կ� ���
								}
								else
								{
									//�̹� ��ϵǾ� �־� �ߺ� ����� �Ұ��� �մϴ�.
									g_WndMng.OpenMessageBox( prj.GetText(TID_COSTUME_ITEMOVERLAP));
								}
							}
							else
							{
								// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
								g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
							}							
						}
						else
						{
							g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
						}
					}
				}
				else if(m_objID_Slot_A[2] != NULL_ID)
				{
					pItem_NextSlot = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[2] );
					//�ɼ� ���� ���� 
					if(pItem_NextSlot != NULL) 
					{
						if( pItem_NextSlot->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )//1�� ������ �ɼ� ������ ���� 
						{
							if( pItem_NextSlot->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
							{
								if( pShortcut->m_dwId != m_objID_Slot_B[3])// B���԰� ������ �������� �ƴ϶��
								{
									m_objID_Slot_A[3] = pShortcut->m_dwId; //���Կ� ���
								}
								else
								{
									//�̹� ��ϵǾ� �־� �ߺ� ����� �Ұ��� �մϴ�.
									g_WndMng.OpenMessageBox( prj.GetText(TID_COSTUME_ITEMOVERLAP));
								}
							}
							else
							{
								// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
								g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
							}
						}
						else
						{
							g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
						}
					}
				}
				else 
				{
					if( pShortcut->m_dwId != m_objID_Slot_B[3])// B���԰� ������ �������� �ƴ϶��
					{
						m_objID_Slot_A[3] = pShortcut->m_dwId; //���Կ� ���
					}
					else
					{
						//�̹� ��ϵǾ� �־� �ߺ� ����� �Ұ��� �մϴ�.
						g_WndMng.OpenMessageBox( prj.GetText(TID_COSTUME_ITEMOVERLAP));
					}
				}				
			}
			else
			{
				//��� ������ ������ �ƴմϴ�.
				g_WndMng.OpenMessageBox( prj.GetText(TID_COSTUME_REGISTERNOTSLOT));
			}
		}
		//------------------------------------------------------------------------------------------

		FLItemElem* pItem_SlotA = NULL;
		int nSize_SlotA,nSize_SlotB;

		//Group_B-----------------------------------------------------------------------------------
		if( PtInRect( &m_rect_Slot_B[0], point ) ) //����_B
		{
			//�ɼ� ���� ���� 
			nSize_SlotA = 0;
			nSize_SlotB = 0;
			pItem_SlotA = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[0] );
			if(pItem_SlotA == NULL) 
			{
				g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_UNCONDITIONALINCHANT02));
				return FALSE;
			}
			//�ɼ� ������ ASlot�� ���ƾ���,,, 
			if( pItem_SlotA->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )
			{
				if(dwPARTS == PARTS_HAT)
				{
					if( pItem_SlotA->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
					{
						if( pItem->m_dwObjId != m_objID_Slot_A[0])// B���԰� ������ �������� �ƴ϶��
						{
							m_objID_Slot_B[0] = pItem->m_dwObjId;	//���Կ� ���
							m_nSlotInput[0] = A_SLOT;
						}
						else
						{
							//�̹� ��ϵǾ� �־� �ߺ� ����� �Ұ��� �մϴ�.
							g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_ITEMOVERLAP)));
						}
					}
					else
					{
						// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
						g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
					}
				}
				else
				{
					//��� ������ ������ �ƴմϴ�.
					g_WndMng.OpenMessageBox( prj.GetText(TID_COSTUME_REGISTERNOTSLOT));
				}
			}	
			else
			{
				g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
			}
		}
		else if( PtInRect( &m_rect_Slot_B[1], point ) )//�ǻ�_B
		{
			nSize_SlotA = 0;
			nSize_SlotB = 0;
			pItem_SlotA = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[1] );
			if(pItem_SlotA == NULL) 
			{
				g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_UNCONDITIONALINCHANT02));
				return FALSE;
			}

			//�ɼ� ������ ASlot�� ���ƾ���,,, 
			if( pItem_SlotA->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )
			{
				if(dwPARTS == PARTS_CLOTH)
				{
					if( pItem_SlotA->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
					{
						if( pItem->m_dwObjId != m_objID_Slot_A[1])// B���԰� ������ �������� �ƴ϶��
						{
							m_objID_Slot_B[1] = pItem->m_dwObjId;	//���Կ� ���
							m_nSlotInput[1] = A_SLOT;
						}
						else
						{
							//�̹� ��ϵǾ� �־� �ߺ� ����� �Ұ��� �մϴ�.
							g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_ITEMOVERLAP)));
						}
					}
					else
					{
						// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
						g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
					}
				}
				else
				{
					//��� ������ ������ �ƴմϴ�.
					g_WndMng.OpenMessageBox( prj.GetText(TID_COSTUME_REGISTERNOTSLOT));
				}
			}
			else
			{
				g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
			}
		}
		else if( PtInRect( &m_rect_Slot_B[2], point ) )//�尩_B
		{
			nSize_SlotA = 0;
			nSize_SlotB = 0;
			pItem_SlotA = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[2] );
			if(pItem_SlotA == NULL)
			{
				g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_UNCONDITIONALINCHANT02));
				return FALSE;
			}

			//�ɼ� ������ ASlot�� ���ƾ���,,, 
			if( pItem_SlotA->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )
			{
				if(dwPARTS == PARTS_GLOVE)
				{
					if( pItem_SlotA->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
					{
						if( pItem->m_dwObjId != m_objID_Slot_A[2])// B���԰� ������ �������� �ƴ϶��
						{
							m_objID_Slot_B[2] = pItem->m_dwObjId;	//���Կ� ���
							m_nSlotInput[2] = A_SLOT;
						}
						else
						{
							//�̹� ��ϵǾ� �־� �ߺ� ����� �Ұ��� �մϴ�.
							g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_ITEMOVERLAP)));
						}
					}
					else
					{
						// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
						g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
					}
				}
				else
				{
					//��� ������ ������ �ƴմϴ�.
					g_WndMng.OpenMessageBox( prj.GetText(TID_COSTUME_REGISTERNOTSLOT));
				}
			}
			else
			{
				g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
			}
		}
		else if( PtInRect( &m_rect_Slot_B[3], point ) )//�Ź�_B
		{
			nSize_SlotA = 0;
			nSize_SlotB = 0;
			pItem_SlotA = g_pPlayer->m_Inventory.GetAtId( m_objID_Slot_A[3] );
			if(pItem_SlotA == NULL) 
			{
				g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_UNCONDITIONALINCHANT02));
				return FALSE;
			}
			//�ɼ� ������ ASlot�� ���ƾ���,,, 
			if( pItem_SlotA->GetRandomOptionExtensionSize() == pItem->GetRandomOptionExtensionSize() )
			{
				if(dwPARTS == PARTS_BOOTS)
				{
					if( pItem_SlotA->GetRandomOptionExtensionSize() + pItem->GetRandomOptionExtensionSize() <= MAX_RANDOMOPTION_MERGE_SIZE )
					{
						if( pItem->m_dwObjId != m_objID_Slot_A[3])// B���԰� ������ �������� �ƴ϶��
						{
							m_objID_Slot_B[3] = pItem->m_dwObjId;	//���Կ� ���
							m_nSlotInput[3] = A_SLOT;
						}
						else
						{
							//�̹� ��ϵǾ� �־� �ߺ� ����� �Ұ��� �մϴ�.
							g_WndMng.OpenMessageBox( _T(prj.GetText(TID_COSTUME_ITEMOVERLAP)));
						}
					}
					else
					{
						// ��ó�� �����ɼ� ������ �����÷ο� �޼��� ó��..
						g_WndMng.OpenMessageBox( _T( prj.GetText( TID_MMI_COSTUME_MIXLEVELOVER ) ) );
					}
				}
				else
				{
					//��� ������ ������ �ƴմϴ�.
					g_WndMng.OpenMessageBox( prj.GetText(TID_COSTUME_REGISTERNOTSLOT));
				}
			}
			else
			{
				g_WndMng.OpenMessageBox( prj.GetText(TID_MMI_COSTUME_MIXLEVELDIFFERENT));
			}
		}
		//------------------------------------------------------------------------------------------
	}
	else
	{
		g_WndMng.OpenMessageBox( _T(prj.GetText(TID_MMI_COSTUME_UNCONDITIONALINCHANT)));
		return FALSE;
	}

	PLAYSND(SND_INF_EQUIPACCESSORY, &g_pPlayer->GetPos());
	if( m_objID_Slot_A[0] != NULL_ID && 
		m_objID_Slot_A[1] != NULL_ID &&
		m_objID_Slot_A[2] != NULL_ID &&
		m_objID_Slot_A[3] != NULL_ID &&
		m_objID_Slot_B[0] != NULL_ID && 
		m_objID_Slot_B[1] != NULL_ID &&
		m_objID_Slot_B[2] != NULL_ID &&
		m_objID_Slot_B[3] != NULL_ID
		)
	{
		CWndButton* pBtnOK  = ( CWndButton* )GetDlgItem( WIDC_BT_START );
		if( pBtnOK )
			pBtnOK->EnableWindow( TRUE );
	}
	else 
		return FALSE;

	return TRUE;
}