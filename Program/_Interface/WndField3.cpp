#include "StdAfx.h"
#include "WndField3.h"
#include "../_CommonDefine/Packet/FLPacketItemOption.h"

//----------------------------------------------------------------------------------------------------------
//CWndWidget3
//----------------------------------------------------------------------------------------------------------
CWndWidget3::CWndWidget3( )
{
}

CWndWidget3::~CWndWidget3( )
{
}

// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndWidget3::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), dwWndId, 0, CPoint( 0, 0 ), pWndParent );
} 

void CWndWidget3::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
		
	// ������ �߾����� �ű�� �κ�.
	MoveParentCenter();
} 

BOOL CWndWidget3::Process( void )
{
	return TRUE;
}

void CWndWidget3::OnDestroyChildWnd( CWndBase* pWndChild )
{
}

BOOL CWndWidget3::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase )
{
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
}

BOOL CWndWidget3::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

void CWndWidget3::OnDraw( C2DRender* p2DRender ) 
{
}

HRESULT CWndWidget3::RestoreDeviceObjects()
{
	CWndBase::RestoreDeviceObjects();
	return S_OK;
}

HRESULT CWndWidget3::InvalidateDeviceObjects()
{
	CWndBase::InvalidateDeviceObjects();
	return S_OK;
}

HRESULT CWndWidget3::DeleteDeviceObjects()
{
	CWndBase::DeleteDeviceObjects();
	return S_OK;
}

//----------------------------------------------------------------------------------------------------------
//CWndReawakening
//----------------------------------------------------------------------------------------------------------
CWndReawakening::CWndReawakening( )
{
	m_dwSlotItem = NULL_ID;
	m_dwMaterialItem = NULL_ID;
	m_bRPushed = FALSE;

	m_fOffset = 0.0f;
	m_fKeepingTime = 0.0f;
	m_fKeepingTime2 = 0.0f;
	m_bStopForGoal = FALSE;

	m_bNoCloseButton = TRUE;
	m_bAnimationEnd = FALSE;
}

CWndReawakening::~CWndReawakening( )
{
	FLItemElem* pMainItem = g_pPlayer->m_Inventory.GetAtId( m_dwSlotItem );
	if( pMainItem )
		pMainItem->SetExtra( 0 );

	FLItemElem* pMaterialItem = g_pPlayer->m_Inventory.GetAtId( m_dwMaterialItem );
	if( pMaterialItem )
		pMaterialItem->SetExtra( 0 );
}

// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndReawakening::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_RANDOMOPTIONRETRY, 0, CPoint( 0, 0 ), pWndParent );
} 

void CWndReawakening::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
		
	// ������ �߾����� �ű�� �κ�.
	MoveParentCenter();

	Reset( );
	UpdateRadioButton( );
} 

BOOL CWndReawakening::Process( void )
{
	if( m_queVirtualSlotData.empty() )
		return TRUE;

	//frame ��� ������ ��ġ��!
	float fFrameUP = ( g_Neuz.m_fFPS / 60.0f  );
	if( fFrameUP > 1.0f )
		fFrameUP = 1.0f;
	else fFrameUP = 60.0f / g_Neuz.m_fFPS;

	if( m_fKeepingTime >= 0.0f )
		m_fKeepingTime -= ( _GetDeltaTimeSec( ) ); 

	// ���� �ӵ� ����
	if( m_bAnimationEnd )
		m_fKeepingTime = 0.0f;
	else if( m_fKeepingTime < 0.4f )
		m_fKeepingTime = 0.4f;
	
	// �귿 �ӵ��� 0�� ��������� �����ؾ� �ϴ°�� ��� ������ ���̸� ��ž
	if( TRUE == m_bStopForGoal )
	{
		// ( ��� ���̴� ) 2�� �ε����� Goal�̸� ����
		DATA& kData = m_queVirtualSlotData[MID_SLOTITEM_NUM];
		if( m_fOffset == 0.0f && kData._kData.byIndex == m_kSavedPacket.byDummyIndex )
		{
			m_fKeepingTime = 0.0f;
			m_bAnimationEnd = TRUE;
		}
	}

//	g_Console.Log( " m_fKeepingTime %f", m_fKeepingTime );

	//�귿 ���ǵ�
	//m_fOffset += ( fFrameUP * m_fKeepingTime * 0.2f ); 
	m_fOffset += ( fFrameUP * m_fKeepingTime * 1.0f ); 
	
	BOOL bFinishColor = FALSE;

	//������ ����� Ȯ���� ���� ���ʰ� �����ش�.
	if( m_fKeepingTime2 > 0.0f && TRUE == m_bAnimationEnd )
	{
		bFinishColor = TRUE;

		m_fKeepingTime2 -= _GetDeltaTimeSec( );
		if( m_fKeepingTime2 <= 0.0f )
		{
			Finish();
			m_fKeepingTime2 = 0.0f;
		}
	}
	
	//�ϳ��� ���� �Ѿ�ٸ�.
	if( m_fOffset > 20.0f )
	{
		if( m_fKeepingTime <= 0.8f && m_bStopForGoal == FALSE )
		{
			// ��÷ �����͸� �ʾ��ش�.
			CWndReawakening::DATA kData( m_kSavedPacket.vecDummyRandomOption[ m_kSavedPacket.byDummyIndex ] );
			m_queVirtualSlotData.push_front( kData );
			m_bStopForGoal = TRUE;
		}
		else 
		{
			// �����κ��� ���� �����Ϳ��� �������� �̾Ƽ� �� ���� ����
			size_t nSize = m_kSavedPacket.vecDummyRandomOption.size();
			int nR = rand() % nSize;
			CWndReawakening::DATA kData( m_kSavedPacket.vecDummyRandomOption[ nR ] );
			m_queVirtualSlotData.push_front( kData );
		}

		m_queVirtualSlotData.pop_back( );
		m_fOffset = 0.0f;
	}

	return TRUE;
}

void CWndReawakening::OnDestroyChildWnd( CWndBase* pWndChild )
{
}

BOOL CWndReawakening::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase )
{
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
}

BOOL CWndReawakening::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	switch( nID )
	{
	case WIDC_START:
		//StartAnimation( );
		//send packet

		{
			CWndButton* pWndStart = ( CWndButton* )GetDlgItem( WIDC_START );
			if( pWndStart )
			{
				// pushed START
				pWndStart->EnableWindow( FALSE );

				CWndButton* pWndClose = ( CWndButton* )GetDlgItem( WIDC_CANCEL );
				if( pWndClose )
					pWndClose->EnableWindow( FALSE );

				SendPacket_Start( );
			}

			//Radio diable
			//EnableRadioGroup( FALSE );
		}
		break;

	case WIDC_CANCEL:
		Destroy( );
		break;

	case WIDC_RADIO1:
	case WIDC_RADIO2:
	case WIDC_RADIO3:
	case WIDC_RADIO4:
	case WIDC_RADIO5:
	case WIDC_RADIO6:
		// Start��ư Ȱ��
		{
			CWndButton* pWndStart = ( CWndButton* )GetDlgItem( WIDC_START );
			if( pWndStart )
				pWndStart->EnableWindow( TRUE );
		}
		break;
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult );	
} 

BOOL CWndReawakening::OnDropIcon( LPSHORTCUT pShortcut, CPoint point )
{
	CWndBase* pWndFrame	= pShortcut->m_pFromWnd->GetFrameWnd();
	if( !pWndFrame )
	{
		FLASSERT( 0 );
		return FALSE;
	}

	if( APP_INVENTORY != pWndFrame->GetWndId( ) )
		return FALSE;

	WNDCTRL* pWndSlot1 = GetWndCtrl( WIDC_PICTURE1 );
	if( NULL != pWndSlot1 && NULL != pShortcut )
	{
		if( TRUE == pWndSlot1->rect.PtInRect( point ) )
		{
			SetSlotItem( pShortcut->m_dwId );
		}
	}
	return TRUE;
}

void CWndReawakening::OnRButtonUp(UINT nFlags, CPoint point)
{
	WNDCTRL* pWndSlot1 = GetWndCtrl( WIDC_PICTURE1 );
	if( pWndSlot1 )
	{
		if( TRUE == pWndSlot1->rect.PtInRect( point ) )
		{
			if( TRUE == m_bRPushed )
				SetSlotItem( NULL_ID );
		}
	}
}

void CWndReawakening::OnRButtonDown(UINT nFlags, CPoint point)
{
	WNDCTRL* pWndSlot1 = GetWndCtrl( WIDC_PICTURE1 );
	if( pWndSlot1 )
	{
		m_bRPushed = pWndSlot1->rect.PtInRect( point );
	}
}

void CWndReawakening::OnLButtonDown( UINT nFlags, CPoint point )
{
	if( NULL == m_dwSlotItem )
		return;

	WNDCTRL* pWndSlot1 = GetWndCtrl( WIDC_PICTURE1 );
	if( NULL == pWndSlot1 )
		return;
	
	if( PtInRect( &pWndSlot1->rect, point ) )
	{
		FLItemBase* pItem = g_pPlayer->GetItemId( m_dwSlotItem );
		if( NULL == pItem )
			return;

		m_GlobalShortcut.m_pFromWnd = this;
		m_GlobalShortcut.m_dwShortcut = SHORTCUT_ITEM;
		m_GlobalShortcut.m_dwType     = ITYPE_ITEM;
		m_GlobalShortcut.m_dwId       = m_dwSlotItem;
		m_GlobalShortcut.m_dwIndex    = pItem->m_dwItemId; // ->GetProp()->dwID;
		m_GlobalShortcut.m_pTexture   = pItem->GetTexture( );
		m_GlobalShortcut.m_dwData     = 0;
	}
}

void CWndReawakening::OnDraw( C2DRender* p2DRender ) 
{
	if( NULL_ID != m_dwSlotItem )
	{
		WNDCTRL* pWndSlot1 = GetWndCtrl( WIDC_PICTURE1 );
		FLItemElem* pItem = g_pPlayer->m_Inventory.GetAtId( m_dwSlotItem );

		if( pItem && pWndSlot1 )
		{
			p2DRender->RenderTexture( pWndSlot1->rect.TopLeft( ) + CPoint( 5, 5 ), pItem->GetTexture( ) );
		}
	}

	DrawSlotAnimation( p2DRender );
}

HRESULT CWndReawakening::RestoreDeviceObjects()
{
	CWndBase::RestoreDeviceObjects();
	return S_OK;
}

HRESULT CWndReawakening::InvalidateDeviceObjects()
{
	CWndBase::InvalidateDeviceObjects();
	return S_OK;
}

HRESULT CWndReawakening::DeleteDeviceObjects()
{
	CWndBase::DeleteDeviceObjects();
	return S_OK;
}

BOOL CWndReawakening::CheckItem( DWORD dwItemId )
{
	FLItemElem* pMaterialItem = g_pPlayer->m_Inventory.GetAtId( m_dwMaterialItem );
	if( NULL == pMaterialItem )
	{
		//g_WndMng.OpenMessageBox( "�η縶�� �������� ��� ���̻� �����Ҽ������ϴ�" );
		Destroy( );
		return FALSE;
	}

	FLItemElem* pItem = g_pPlayer->m_Inventory.GetAtId( dwItemId );
	if( NULL == pItem )
		return FALSE;

	PT_ITEM_SPEC pSpec = pItem->GetProp();
	if( NULL == pSpec )
		return FALSE;

	if( pSpec->dwRandomOptionMode != 1 )
	{
		g_WndMng.OpenMessageBox( prj.GetText( TID_MMI_RANDOMOPTIONRETRY_ERRORTEXT02 ) );	// ���� �Ұ��� ������
		return FALSE;
	}

	if( pItem->GetRandomOptionExtensionSize( ) < 2 )
	{
		g_WndMng.OpenMessageBox( prj.GetText( TID_MMI_RANDOMOPTIONRETRY_ERRORTEXT03 ) );	// �ɼ��� 2������
		return FALSE;
	}

	//���� ��ȣ�� �ɼ��̸� pass
	if( pItem->IsSetSafeGuardRandomOptionExtensionFlag() == true )
	{
		g_WndMng.OpenMessageBox( prj.GetText( TID_MMI_RANDOMOPTIONRETRY_ERRORTEXT04 ) );	//������ȣ�� ������
		return FALSE;
	}

	//�ڽ�Ƭ�̸� �Ұ���
	if( CFLWndCostume_Mix::ERROR_SLOT != CFLWndCostume_Mix::IsCostume( pSpec ) )
	{	
		g_WndMng.OpenMessageBox( prj.GetText( TID_MMI_RANDOMOPTIONRETRY_ERRORTEXT02 ) );	// ���� �Ұ��� ������
		return FALSE;
	}

	return TRUE;
}

void CWndReawakening::EnableRadioGroup( BOOL bEnable, BOOL bResetCheck )
{
	//Radio ��Ȱ��
	DWORD dwRadioIds[] = { WIDC_RADIO1, WIDC_RADIO2, WIDC_RADIO3, WIDC_RADIO4, WIDC_RADIO5, WIDC_RADIO6 };
	CWndButton* pWndRadio = NULL;
	for( int i = 0; i < MAX_RANDOMOPTION_RETRY_SIZE; ++i )
	{
		pWndRadio = ( CWndButton* )GetDlgItem( dwRadioIds[i] );
		if( pWndRadio )
		{
			pWndRadio->EnableWindow( bEnable );
			if( bResetCheck )
				pWndRadio->SetCheck( 0 );
		}
	}
}

void CWndReawakening::SendPacket_Start( )
{
	BYTE nIndex = -1;
	DWORD dwRadioIds[] = { WIDC_RADIO1, WIDC_RADIO2, WIDC_RADIO3, WIDC_RADIO4, WIDC_RADIO5, WIDC_RADIO6 };

	CWndButton* pWndRadio = NULL;
	for( int i = 0; i < MAX_RANDOMOPTION_RETRY_SIZE; ++i )
	{
		pWndRadio = ( CWndButton* )GetDlgItem( dwRadioIds[i] );
		if( pWndRadio )
		{
			if( pWndRadio->GetCheck() )
			{
				nIndex = i;
				break;
			}
		}
	}

	if( -1 == nIndex )
	{
		FLASSERT( 0 );
		Reset( );
		return;
	}

	FLPacketItemUpgradeRandomOptionGenerateRetryReq kPacket;
	kPacket.byIndex = nIndex;
	kPacket.dwMainItemObjID = m_dwSlotItem;
	kPacket.dwMaterialItemObjID = m_dwMaterialItem;		//�η縶�� id
	kPacket.dwUpgradeNpcObjID = NULL_ID;		//�ٷ糪 ����?

	g_DPlay.SendPacket( &kPacket );

	//�Ϸ��� �޽��� �ڽ��� ( ������ �ɼ� ���ڿ� )
	m_strSelectedOption = pWndRadio->GetTitle( );
}

void CWndReawakening::SendPacket_Sync( )
{
	FLPacketItemOptionRandomOptionSyncReq kPacket;
	kPacket.dwItemObjID = m_dwSlotItem;

	g_DPlay.SendPacket( &kPacket );
}

void CWndReawakening::ReceivedPacket_Start( const FLSnapshotItemUpgradeRandomOptionGenerateRetryResultAck& kData )
{
	//�����͸� �����ϰ� ������.
	m_kSavedPacket = kData;

	//CHECK
	size_t nCheckSize = m_kSavedPacket.vecDummyRandomOption.size();
	if( nCheckSize <= m_kSavedPacket.byDummyIndex || nCheckSize < MAX_SLOTITEM_NUM )
	{
		FLASSERT( 0 );

		g_WndMng.OpenMessageBox( "Reawakening packet error" );
		Destroy( );
		return;
	}

	//Test
	RandomOptionExtensionVec& vecData = m_kSavedPacket.vecDummyRandomOption;
	size_t nSize = vecData.size();
	for( int i = 0; i < MAX_SLOTITEM_NUM; ++i )
	{
		int nR = rand() % nSize;
		m_queVirtualSlotData.push_back( CWndReawakening::DATA( vecData[nR] ) );
	}

	StartAnimation( );
	EnableWindow( FALSE );
}

void CWndReawakening::ReceivedPacket_Sync( )
{
	//Reset & Keep item

	//make a msg
	DATA kTempData( m_kSavedPacket.vecDummyRandomOption[ m_kSavedPacket.byDummyIndex ] );
	CString strNotice;

	strNotice.Format( prj.GetText( TID_MMI_RANDOMOPTIONRETRY_SUCCESSTEXT01 ), m_strSelectedOption, kTempData._strOption );
	g_WndMng.OpenMessageBox( strNotice.GetString() ); //prj.GetText( TID_MMI_RANDOMOPTIONRETRY_SUCCESSTEXT01 ) );	//����Ϸ�

	DWORD dwKeepingSlotItem = m_dwSlotItem;
	EnableWindow( TRUE );
	Reset( );
	SetSlotItem( dwKeepingSlotItem );

	m_fKeepingTime = 0.0f;
	m_fKeepingTime2 = 0.0f;
}

void CWndReawakening::SetSlotItem( DWORD dwItemId )
{
	if( NULL_ID == dwItemId )
	{
		Reset( );

		//���� ������ ����
		UpdateRadioButton( );
	}
	else
	{
		if( FALSE == CheckItem( dwItemId ) )
			return;

		//���� ��ϵ� �������� �ִٸ�, unlock
		FLItemElem* pOldItem = g_pPlayer->m_Inventory.GetAtId( m_dwSlotItem );
		if( pOldItem )
			pOldItem->SetExtra( 0 );

		m_dwSlotItem = dwItemId;

		//������ ��ϵ� ������ lock
		FLItemElem* pNewItem = g_pPlayer->m_Inventory.GetAtId( m_dwSlotItem );
		if( pNewItem )
			pNewItem->SetExtra( pNewItem->GetExtra() - 1 );

		//���� ������ ����
		UpdateRadioButton( );

		//TODO: send packet 
		//���� �ɼ� ������ ��û
	}
}

BOOL IsDst_Rate( int nDstParam );
char* FindDstString( int nDstParam );
void CWndReawakening::UpdateRadioButton( )
{
	CWndButton* pWndRadio[MAX_RANDOMOPTION_RETRY_SIZE] = { NULL, };
	pWndRadio[0] = (CWndButton*)GetDlgItem( WIDC_RADIO1 );
	pWndRadio[1] = (CWndButton*)GetDlgItem( WIDC_RADIO2 );
	pWndRadio[2] = (CWndButton*)GetDlgItem( WIDC_RADIO3 );
	pWndRadio[3] = (CWndButton*)GetDlgItem( WIDC_RADIO4 );
	pWndRadio[4] = (CWndButton*)GetDlgItem( WIDC_RADIO5 );
	pWndRadio[5] = (CWndButton*)GetDlgItem( WIDC_RADIO6 );

	for( int i = 0; i < MAX_RANDOMOPTION_RETRY_SIZE; ++i )
	{
		if( pWndRadio[ i ] == NULL )
			return;

		pWndRadio[i]->SetTileColor( 0xff777777 );
		pWndRadio[i]->SetTitle( "" );
		pWndRadio[i]->EnableWindow( FALSE );
	}

	if( NULL_ID == m_dwSlotItem )
		return;

	FLItemElem* pItem = g_pPlayer->m_Inventory.GetAtId( m_dwSlotItem );
	if( NULL == pItem )
		return;

	//4. �����ɼ�
	PT_ITEM_SPEC pItemProp = pItem->GetProp( );
	if( NULL == pItemProp )
		return;

	if( pItem->GetRandomOptionExtensionSize() > MAX_RANDOMOPTION_RETRY_SIZE )
	{
		FLASSERT( 0 );
		g_WndMng.OpenMessageBox( "Not supported" );
		return;
	}

	CString str;
	for( size_t Nth = 0; Nth < pItem->GetRandomOptionExtensionSize(); ++Nth )
	{
		pWndRadio[Nth]->EnableWindow( TRUE );
		WORD wDstID = pItem->GetRandomOptionExtensionDstID( Nth );
		short shAdjValue = pItem->GetRandomOptionExtensionAdjValue( Nth );

	//���� ��ȣ�� �ɼ��̸� pass
	//	if( pItem->IsSetSafeGuardRandomOptionExtensionFlag() == true )
	//		continue;
		
		if( IsDst_Rate( wDstID ) == TRUE )
		{
			if( wDstID == DST_ATTACKSPEED )
			{
				str.Format( "%s %c%d%% ", FindDstString( wDstID ), ( shAdjValue > 0? '+': '-' ), ::abs( shAdjValue / 2 / 10 ) );
			}
			else
			{
				str.Format( "%s %c%d%%", FindDstString( wDstID ), ( shAdjValue > 0? '+': '-' ), ::abs( shAdjValue ) );
			}
		}
		else
		{
			str.Format( "%s %c%d", FindDstString( wDstID ), ( shAdjValue > 0? '+': '-' ), ::abs( shAdjValue ) );
		}
		
		DWORD dwColor = 0xffffffff;
		pWndRadio[Nth]->SetTileColor( dwColor );
		pWndRadio[Nth]->SetTitle( str );
	}
}

void CWndReawakening::StartAnimation( )
{
	//m_fKeepingTime = 6.0f;
	//m_fKeepingTime2 = 4.0f;
	m_fKeepingTime = 2.0f;
	m_fKeepingTime2 = 0.5f;
	m_fOffset = 0.0f;
	m_bStopForGoal = FALSE;
	m_bAnimationEnd = FALSE;
}

void CWndReawakening::SetItemId( DWORD dwItemId )
{
	FLASSERT( dwItemId != NULL_ID );

	m_dwMaterialItem = dwItemId;

	FLItemElem* pMaterialItem = g_pPlayer->m_Inventory.GetAtId( dwItemId );
	if( pMaterialItem )
		pMaterialItem->SetExtra( pMaterialItem->GetExtra() - 1 );
}

void CWndReawakening::Finish( )
{
	// send packet 
	// lock

	SendPacket_Sync( );
}

void CWndReawakening::DropedIcon( DWORD dwItemId )
{
	SetSlotItem( NULL_ID );
	//Reset( );
}

void CWndReawakening::Reset( )
{
	FLItemElem* pOldItem = g_pPlayer->m_Inventory.GetAtId( m_dwSlotItem );
	if( pOldItem )
		pOldItem->SetExtra( 0 );

	m_dwSlotItem = NULL_ID;

	EnableRadioGroup( FALSE, TRUE );

	CWndButton* pWndStart = ( CWndButton* )GetDlgItem( WIDC_START );
	if( pWndStart )
		pWndStart->EnableWindow( FALSE );

	CWndButton* pWndClose = ( CWndButton* )GetDlgItem( WIDC_CANCEL );
	if( pWndClose )
		pWndClose->EnableWindow( TRUE );

	m_queVirtualSlotData.clear();
	
	//m_queVirtualSlotData.push_back( CWndReawakening::DATA( 0, "11111111111111111111111" ) );
	//m_queVirtualSlotData.push_back( CWndReawakening::DATA( 1, "22222222222222222222222" ) );
	//m_queVirtualSlotData.push_back( CWndReawakening::DATA( 2, "33333333333333333333333" ) );
	//m_queVirtualSlotData.push_back( CWndReawakening::DATA( 3, "44444444444444444444444" ) );
	//m_queVirtualSlotData.push_back( CWndReawakening::DATA( 4, "55555555555555555555555" ) );
}

void CWndReawakening::DrawSlotAnimation( C2DRender* p2DRender )
{
	if( m_queVirtualSlotData.empty() )
		return;

	// �ɼ��� ������ �Ʒ��� �̵��ϴ� ( ���Ըӽ� ) animation
	WNDCTRL* pWndSlot2 = GetWndCtrl( WIDC_PICTURE2 );
	if( NULL == pWndSlot2 )
		return;

	CRect kRect = pWndSlot2->rect;
	CPoint kPt = p2DRender->GetViewportOrg( );
	CRect kViewRect;
	kViewRect.left = kRect.left + kPt.x + 10;
	kViewRect.top = kRect.top + kPt.y + 10;
	kViewRect.right = kViewRect.left + kRect.Width() - 10;
	kViewRect.bottom = kViewRect.top + kRect.Height() - 20;
	p2DRender->SetViewport( kViewRect );
	
	//�귿 render
	DWORD dwColor = 0xff000000;
	if( CS_VER2 == _GetContentState( CT_REAWAKENING ) )
		dwColor = 0xffffffff;
	
	for( int i = 0; i < MAX_SLOTITEM_NUM; ++i )
	{
		p2DRender->TextOut( kRect.left + 20, kRect.top - 8 + ( i * 20 ) + (int)m_fOffset, m_queVirtualSlotData[ i ]._strOption, dwColor );
	}

	p2DRender->SetViewport( this->m_rectWindow );
}


CWndReawakening::DATA::DATA( const T_RANDOMOPTION_EXT& kData)
{
	//�ɼ� �����͸� ���ڿ��� ��ü�Ѵ�.
	_kData = kData;

	WORD wDstID = _kData.wDstID;
	short shAdjValue = _kData.shAdjValue;
	
	if( IsDst_Rate( wDstID ) == TRUE )
	{
		if( wDstID == DST_ATTACKSPEED )
		{
			_strOption.Format( "%s %c%d%% ", FindDstString( wDstID ), ( shAdjValue > 0? '+': '-' ), ::abs( shAdjValue / 2 / 10 ) );
		}
		else
		{
			_strOption.Format( "%s %c%d%%", FindDstString( wDstID ), ( shAdjValue > 0? '+': '-' ), ::abs( shAdjValue ) );
		}
	}
	else
	{
		_strOption.Format( "%s %c%d", FindDstString( wDstID ), ( shAdjValue > 0? '+': '-' ), ::abs( shAdjValue ) );
	}
}

//----------------------------------------------------------------------------------------------------------
//CWndPartyUpgradeScroll
//----------------------------------------------------------------------------------------------------------
CWndPartyUpgradeScroll::CWndPartyUpgradeScroll( )
{
}

CWndPartyUpgradeScroll::~CWndPartyUpgradeScroll( )
{
}

// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndPartyUpgradeScroll::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_PARTYCHANGENAME, 0, CPoint( 0, 0 ), pWndParent );
} 

void CWndPartyUpgradeScroll::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
		
	// ������ �߾����� �ű�� �κ�.
	MoveParentCenter();

	CWndEdit* pWndEdit = ( CWndEdit* )GetDlgItem( WIDC_EDIT1 );
	if( pWndEdit )
	{
		pWndEdit->SetFocus();
	}
} 

BOOL CWndPartyUpgradeScroll::Process( void )
{
	return TRUE;
}

void CWndPartyUpgradeScroll::OnDestroyChildWnd( CWndBase* pWndChild )
{
}

BOOL CWndPartyUpgradeScroll::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase )
{
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
}

BOOL CWndPartyUpgradeScroll::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	switch( nID )
	{
	case WIDC_OK:
		{
			CWndEdit* pWndEdit = ( CWndEdit* )GetDlgItem( WIDC_EDIT1 );
			if( pWndEdit )
			{
				CString strNewName = pWndEdit->GetString( );
				SendPacket( strNewName );
				Destroy();
			}
		}
		break;

	case WIDC_CANCEL:
		Destroy( );
		break;
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

void CWndPartyUpgradeScroll::OnDraw( C2DRender* p2DRender ) 
{
}

HRESULT CWndPartyUpgradeScroll::RestoreDeviceObjects()
{
	CWndBase::RestoreDeviceObjects();
	return S_OK;
}

HRESULT CWndPartyUpgradeScroll::InvalidateDeviceObjects()
{
	CWndBase::InvalidateDeviceObjects();
	return S_OK;
}

HRESULT CWndPartyUpgradeScroll::DeleteDeviceObjects()
{
	CWndBase::DeleteDeviceObjects();
	return S_OK;
}

void CWndPartyUpgradeScroll::SendPacket( const CString& strNewName )
{
	
}