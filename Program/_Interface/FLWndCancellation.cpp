
#include "StdAfx.h"

#include "FLWndCancellation.h"

CFLWndCancellation::CFLWndCancellation()
{
	memset( &m_objID, NULL_ID, sizeof( m_objID ) );
	m_bNoCloseButton = TRUE;
}

CFLWndCancellation::~CFLWndCancellation()
{

}

void CFLWndCancellation::OnInitialUpdate()
{
	CWndNeuz::OnInitialUpdate(); 
	CWndStatic* pWndSlotItem = (CWndStatic*)GetDlgItem(WIDC_STATIC_SLOTITEM);
	if( !( pWndSlotItem ) )
		FLERROR_LOG( PROGRAM_NAME, _T( "CFLWndCancellation::OnInitialUpdate()" ) );
	
	m_rcSlot = pWndSlotItem->GetWndRect();

	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();

	CWndButton* pBtnClear  = ( CWndButton* )GetDlgItem( WIDC_BT_CLEAR );
	if( !pBtnClear )
		FLERROR_LOG( PROGRAM_NAME, _T( "CFLWndCancellation::OnInitialUpdate()" ) );

	pBtnClear->EnableWindow( FALSE );

}

BOOL CFLWndCancellation::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_CANCELLATION, 0, CPoint( 0, 0 ), pWndParent );
} 
void CFLWndCancellation::ItemClear()
{
	_T_PACKET_RESET_BIND	tResetBind;
	tResetBind.dwSrcObjid = m_pScrItem->m_dwObjId;
	tResetBind.dwDestObjid = m_objID;
	tResetBind.bResult = TRUE;
	g_DPlay.SendReqResetBind(&tResetBind);
}

void CFLWndCancellation::ReceivedPacket(PT_PACKET_RESET_BIND  ptBind)
{
	if(ptBind->bResult == TRUE)
	{
		CString str;

		FLItemElem*  pItem = g_pPlayer->m_Inventory.GetAtId(ptBind->dwDestObjid);
		if(pItem->GetProp()->dwParts == NULL_ID)
		{
			//���� �ͼ�
			g_WndMng.OpenMessageBox( _T( prj.GetText(TID_ITEM_CANCEL_CLEAR_USE) ) );		
		}
		else
		{
			//����ñͼ�
			g_WndMng.OpenMessageBox( _T( prj.GetText(TID_ITEM_CANCEL_CLEAR_EQUIP) ) );		
		}
		CWndButton* pBtn = ( CWndButton* )GetDlgItem( WIDC_BT_CLEAR );
		if( pBtn )
			pBtn->EnableWindow( FALSE );		
		
	}
	memset( &m_objID, NULL_ID, sizeof( m_objID ) );
	Destroy();
}

BOOL CFLWndCancellation::OnChildNotify( UINT message,UINT nID,LRESULT* pLResult )
{
	if( EN_RETURN == message )
	{
		ItemClear();
	}
	switch(nID)
	{
	case WIDC_BT_CLEAR:
		ItemClear();
		break;
	case WIDC_BT_CANCEL:
		Destroy();
		break;
	}
	return TRUE;
}
void CFLWndCancellation::OnDraw( C2DRender* p2DRender )
{
	if( m_objID != 0 )
	{
		FLItemElem* pItem = g_pPlayer->m_Inventory.GetAtId( m_objID );
		if( IsUsableItem( pItem ) )
			p2DRender->RenderTexture( m_rcSlot.TopLeft(), pItem->GetTexture() );		
	}
}

BOOL CFLWndCancellation::OnDropIcon( LPSHORTCUT pShortcut, CPoint point )
{	
	CWndBase* pWndFrame	= pShortcut->m_pFromWnd->GetFrameWnd();
	if(!pWndFrame)
	{
		FLASSERT( 0 );
		return FALSE;
	}

	// �κ����� �� �����۸� ���
	if( APP_INVENTORY != pWndFrame->GetWndId( ) )			
	{
		AfxMessageBox( "No inventory item" );
		return FALSE;
	}

	//check valid item
	m_pSlotItem = new FLItemElem;
	m_pSlotItem = g_pPlayer->m_Inventory.GetAtId( pShortcut->m_dwId );
		
	if( !IsUsableItem( m_pSlotItem ) )
		return FALSE;

	PT_ITEM_SPEC pProp = m_pSlotItem->GetProp();
	if( NULL == pProp )
		return FALSE;

	if(m_objID == NULL_ID)
	{
		//check slot
		if( PtInRect( &m_rcSlot, point ) )
		{
			if( m_pSlotItem->IsOwnState() )
			{
				//�ͼӻ����̸�
				CWndButton* pBtn = ( CWndButton* )GetDlgItem( WIDC_BT_CLEAR );
				if( pBtn )
					pBtn->EnableWindow( TRUE );
				m_objID = pShortcut->m_dwId;
				m_pScrItem = g_pPlayer->m_Inventory.GetAtByItemId(ITEM_INDEX( 20793, II_SYS_SYS_SCR_CANCELLATION ));
			}	
			else
			{
				//���� �ͼӻ��°� �ƴ�
				g_WndMng.OpenMessageBox( _T( prj.GetText(TID_ITEM_CANCEL_ERROR1) ) );	
			}
		}
	}
	else
	{
		//slot�� �̹� �������� ����
		g_WndMng.OpenMessageBox( _T( prj.GetText(TID_ITEM_CANCEL_ERROR3) ) );	
	}

	return TRUE;
}
