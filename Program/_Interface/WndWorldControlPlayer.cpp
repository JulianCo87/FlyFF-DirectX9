// WndArcane.cpp: implementation of the CWndNeuz class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Sfx.h"

#include "Ship.h"


//int		__bTestLOD = 1;

int CWndWorld::ControlPlayer( DWORD dwMessage, CPoint point )
{
	if( g_pPlayer == NULL || g_pPlayer->IsDie() ) 
		return 0;
	if( g_pPlayer->m_dwMode & DONMOVE_MODE )
		return 0;
//sun: 10�� ���½ý���	Neuz, World, Trans

//	if( g_pPlayer->GetAdjParam( DST_CHRSTATE ) & CHS_LOOT)			return 0;

	if( g_pPlayer->GetAdjParam( DST_CHRSTATE ) & CHS_SETSTONE)
		return 0;

	if( g_pPlayer->m_vtInfo.VendorIsVendor() )
		return 0;
	if( g_WndMng.m_bAllAction == FALSE )
		return 0;

	if( g_WndMng.m_pLogOutWaitting ) // �������̸� �÷��̾� ��Ʈ�� �Ұ�
		return 0;

	if( g_WndMng.GetWndBase(APP_WEBBOX) )
		return 0;

//sun: 10, __EVE_MINIGAME
	if( g_WndMng.GetWndBase(APP_MINIGAME_KAWIBAWIBO) )
		return 0;
	if( g_WndMng.GetWndBase(APP_MINIGAME_DICE) )
		return 0;
	if( g_WndMng.GetWndBase(APP_MINIGAME_WORD) )
		return 0;
	if( g_WndMng.GetWndBase(APP_MINIGAME_PUZZLE) )
		return 0;

	if( g_pPlayer->m_dwReqFlag & REQ_USESKILL )	return 0;		// �����κ��� useskill������ ���������� �׼��ؼ� �ȵ�.
	
	if( g_WndMng.GetWndBase(APP_MAP_TELEPORT) )//�ڷ���Ʈ ������������ ���� ������ ��Ʈ�ѺҰ� 
		return 0;

/*	if( pWndChat && bWhisper )
	{
		pWndChat->SetFocus();
		CWndEditChat* pWndEdit = &pWndChat->m_wndEdit;
		pWndEdit->SetFocus();
	}
*/
	if( g_pShip )
	{
		return ControlShip( dwMessage, point );
	} else
	{
		if( g_pPlayer->m_pActMover->IsFly() )
			return ControlFlying( dwMessage, point );
		return ControlGround( dwMessage, point );
	}
}
//
//		������Ʈ�� �����̴µ� �ʿ��� �޽����� �߻�
//
int		CWndWorld::ControlGround( DWORD dwMessage, CPoint point )
{
	bool	fCastCancel	= false;
	int		nMsg = 0;
	BOOL	bTempKey; 

	BOOL	bUp, bDown, bLeft= FALSE, bRight= FALSE, bSpace = FALSE, bBoard, bLForward = FALSE, bRForward = FALSE;
	BOOL	bWalk;
	static BOOL	s_bWalk2 = FALSE;

	CMover* pMover = CMover::GetActiveMover();

	CWndChat* pWndChat = (CWndChat*) g_WndMng.GetApplet( APP_COMMUNICATION_CHAT );
	BOOL bWhisper = g_bKeyTable['R'];
	if( pWndChat && bWhisper )
	{
		if( 0 < strlen( g_Neuz.m_szWhisperName ) )
		{
			CString strWhisper;
			strWhisper.Format( "/whisper %s ", g_Neuz.m_szWhisperName );
			pWndChat->SetFocus();
			CWndEditChat* pWndEdit = &pWndChat->m_wndEdit;
			pWndEdit->SetString( strWhisper );
			pWndEdit->SetFocus();
			g_bKeyTable['R'] = FALSE;
		}
	}

	const PFLSHORTCUTKEY_CONTROL	pShortcutKey_Control( g_xKeyManager->GetShortcutKey_Control() );
	const PFLSHORTCUTKEY_UI			pShortcutKey_UI( g_xKeyManager->GetShortcutKey_UI() );
	const PFLSHORTCUTKEY_SYSTEM		pShortcutKey_System( g_xKeyManager->GetShortcutKey_System() );

	// ����/����/����
	CWndBase* pWndBaseFocus = (CWndBase*) g_WndMng.GetFocusWnd();
	if( g_Neuz.m_bActiveNeuz == FALSE || ( pWndChat && pWndBaseFocus && pWndBaseFocus == pWndChat ) )
	{

		g_bKeyTable[ pShortcutKey_Control->KeyUp 		] = FALSE;
		g_bKeyTable[ pShortcutKey_Control->KeyDown		] = FALSE;	
		g_bKeyTable[ pShortcutKey_Control->KeyLeft		] = FALSE;
		g_bKeyTable[ pShortcutKey_Control->KeyRight		] = FALSE;
		g_bKeyTable[ pShortcutKey_UI->KeyQuest			] = FALSE;
		g_bKeyTable[ pShortcutKey_UI->KeyMessanger		] = FALSE;
	}

	bUp	  = g_bKeyTable[ pShortcutKey_Control->KeyUp	];	
	bDown = g_bKeyTable[ pShortcutKey_Control->KeyDown	];

 	if( pMover->m_pActMover->GetState() & OBJSTA_ATK_ALL ) {
 		m_bLButtonDown = FALSE;
 	}

	//gmpbigsun : Ű���� �����߿��� ���콺�̵� �Ұ� 
	if( bUp || bDown )
		m_bLButtonDown = FALSE;

	if( bDown )
	{
		g_WndMng.m_bAutoRun = FALSE;
	}
	if( bUp )
	{
		m_timerAutoRunPush.Reset();
		if( m_nDubleUp == 2 && m_timerAutoRun.TimeOut() == FALSE )
		{
			m_nDubleUp = 3;
			g_WndMng.m_bAutoRun = TRUE;
			m_timerAutoRun.Reset();
			m_timerAutoRunBlock.Reset();
		}
		else
		{
			m_nDubleUp = 1;
			m_timerAutoRun.Reset();
		}
		if( m_timerAutoRunBlock.TimeOut() )
			g_WndMng.m_bAutoRun = FALSE;
	}
	else
	{
		if( m_timerAutoRunPush.TimeOut() == FALSE )
		{
			if( m_nDubleUp == 1 )
				m_nDubleUp = 2;
		}
		else
		{
			m_nDubleUp = 0;
		}
	}

	if( g_WndMng.m_bAutoRun )
		bUp = TRUE;

	// ��/�� ȸ��	//05
#ifdef	KEYBOARD_SET
	if ( g_pPlayer->IsMode( OLD_KEYBOARD_MODE ) )
	{
		bLeft  = g_bKeyTable[ pShortcutKey_Control->KeyLeft		];
		bRight = g_bKeyTable[ pShortcutKey_Control->KeyRight	];
		
	}
	else
	{
		bLForward = g_bKeyTable[ pShortcutKey_Control->KeyLeft		];
		bRForward = g_bKeyTable[ pShortcutKey_Control->KeyRight		];
	}
#else
	bLeft  = g_bKeyTable[ pShortcutKey_Control->KeyLeft		];
	bRight = g_bKeyTable[ pShortcutKey_Control->KeyRight	];
	
#endif // KEYBOARD_SET	

	bSpace	= g_bKeyTable[ pShortcutKey_Control->KeyJump	];

	bBoard = g_bKeyTable['B'];

	CWorld* pWorld = g_WorldMng.Get();
	CRect rect = GetClientRect();
	D3DXVECTOR3 vRayEnd;
	CObj* pFocusObj = pWorld->GetObjFocus();

	CActionMover *pAct = pMover->m_pActMover;
	pAct->m_dwCtrlMsg = 0;
		
	if( m_bLButtonDown )
		pAct->m_dwCtrlMsg |= CTRLMSG_LDOWN;

//sun: 12, ���� ���� Neuz, World
	D3DXVECTOR3 vec3Tri[3];
	pWorld->ClientPointToVector( vec3Tri, rect, point, &pWorld->m_matProj, &g_Neuz.m_camera.m_matView, &vRayEnd, TRUE );
	g_Neuz.m_vCursorPos = vRayEnd;

	if( g_Neuz.m_pCreateMonItem )
	{
		if( bUp || bDown || bLeft || bRight || bSpace || m_bLButtonDown )
		{
			BOOL bSendCM = TRUE;
			if( m_bLButtonDown )
			{
				D3DXVECTOR3 vDist2 = g_pPlayer->GetPos() - g_Neuz.m_vCursorPos;
				float fDist = D3DXVec3Length( &vDist2 );			// ����ǥ���� �Ÿ�
				if( 15.f < fDist )
				{
					g_WndMng.PutString( prj.GetText( TID_GAME_CREATEMON_F_15 ), NULL, prj.GetTextColor( TID_GAME_CREATEMON_F_15 ) );
					bSendCM = FALSE;
				}
				if( bSendCM )
				{
					int nAttr = g_pPlayer->GetWorld()->GetHeightAttribute( g_Neuz.m_vCursorPos.x, g_Neuz.m_vCursorPos.z );		// �̵��� ��ġ�� �Ӽ� ����.
					if( nAttr == HATTR_NOWALK || nAttr == HATTR_NOMOVE 
						|| g_pPlayer->IsRegionAttr( RA_SAFETY ) || g_pPlayer->GetWorld()->GetID() == WI_WORLD_GUILDWAR )		// �� �����̴� ���̰ų� ���������̸� Pass
					{
						g_WndMng.PutString( prj.GetText( TID_GAME_CREATEMON_F_AREA ), NULL, prj.GetTextColor( TID_GAME_CREATEMON_F_AREA ) );
						bSendCM = FALSE;
					}
					else
					if( g_pPlayer->GetWorld()->GetID() != WI_WORLD_MADRIGAL )
					{
						g_WndMng.PutString( prj.GetText( TID_GAME_CREATEMON_F_AREA ), NULL, prj.GetTextColor( TID_GAME_CREATEMON_F_AREA ) );
						bSendCM = FALSE;
					}
					
					if( bSendCM )
					{
						g_DPlay.SendCreateMonster( MAKELONG( ITYPE_ITEM, g_Neuz.m_pCreateMonItem->m_dwObjId ), g_Neuz.m_vCursorPos );
					}
				}			
				m_bLButtonDown = FALSE;
			}
			if( bSendCM )
				g_Neuz.m_pCreateMonItem = NULL;
		}		
	}

	//TODO:ata3k�� �� �����ּ���. �� �׷��� �ƹ��� ����!
	// �̵����� ���°� �ƴҶ��� Ŭ������ �̵��Ҽ� �ִ�.
#ifdef __Y_INTERFACE_VER3
	bool *bpButton;

	if( g_Option.m_nInterface == 2 )
		bpButton = &m_bLButtonUp;
	else
		bpButton = &m_bLButtonDown;
	
	if( *bpButton )	
#else //__Y_INTERFACE_VER3
	if( m_bLButtonDown )				
#endif //__Y_INTERFACE_VER3
	{
	#ifdef __Y_INTERFACE_VER3	
		if( g_Option.m_nInterface == 2 )
		{
			*bpButton = FALSE;

			if( m_timerLButtonDown.GetLeftTime() > 200 )
				return nMsg;		
		}
	#endif //__Y_INTERFACE_VER3

		D3DXVECTOR3 vec3Tri[3];
		if( pWorld->ClientPointToVector( vec3Tri, rect, point, &pWorld->m_matProj, &g_Neuz.m_camera.m_matView, &vRayEnd, TRUE ) ) 
		{
			// �̵� ����Ʈ�� ��� ��ǥ ���� 
			if( m_bFreeMove )
			{
	//			if( m_bLButtonDown )	// �̵����� ���°� �ƴҶ��� Ŭ������ �̵��Ҽ� �ִ�.
				{
					{
						if( m_pWndGuideSystem  && m_pWndGuideSystem->IsVisible())
							//sun: 12, Ʃ�丮�� ����
							m_pWndGuideSystem->m_Condition.bIsClickOnLand = true;

	#ifdef __IAOBJ0622					
						if( GetLastPickObj() && GetLastPickObj()->GetType() == OT_SHIP )
							pMover->SetDestPos( (CShip *)GetLastPickObj(), vRayEnd );
						else
							pMover->SetDestPos( vRayEnd );
	#else
						pMover->SetDestPos( vRayEnd );
	#endif
						m_bFreeMove		= FALSE;

						g_DPlay.SendPlayerBehavior();
						g_DPlay.SendSnapshot( TRUE );
						fCastCancel	= true;

					

						CSfx *pObj = CreateSfx(g_Neuz.m_pd3dDevice, XI_INDEX( 63, XI_GEN_MOVEMARK01 ),vRayEnd);
						
						
						D3DXVECTOR3 vVector1 = vec3Tri[2] - vec3Tri[0];
						D3DXVECTOR3 vVector2 = vec3Tri[1] - vec3Tri[0];
						D3DXVECTOR3 vNormal;
						D3DXVec3Cross( &vNormal, &vVector1, &vVector2);	
						D3DXVec3Normalize( &vNormal, &vNormal );
						
						D3DXVECTOR3 v3Up = D3DXVECTOR3( 0.0f, -1.0f, 0.0f );
						D3DXVECTOR3 v3Cross;
						FLOAT fDot;
						FLOAT fTheta;
						D3DXVec3Cross( &v3Cross, &v3Up, &vNormal );
						fDot = D3DXVec3Dot( &v3Up, &vNormal );
						fTheta = acos( fDot );
						
						D3DXQUATERNION qDirMap;
						D3DXQuaternionRotationAxis( &qDirMap, &v3Cross, fTheta );
						
						D3DXVECTOR3 vYPW;
						QuaternionRotationToYPW( qDirMap, vYPW );
						
						pObj->m_pSfxObj->m_vRotate.x = D3DXToDegree(vYPW.x);
						pObj->m_pSfxObj->m_vRotate.y = D3DXToDegree(vYPW.y);
						pObj->m_pSfxObj->m_vRotate.z = D3DXToDegree(vYPW.z);
						
						ClearTracking();
						g_pMoveMark = (CSfxGenMoveMark*) pObj;
						
					}
				}
			}
		}
	}
	
	//if( !pMover->IsEmptyDestPos() || !pMover->IsEmptyDestObj() )
	//	return nMsg;
#ifdef __Y_INTERFACE_VER3
	if( bUp || bDown || bLeft || bRight || bSpace || bLForward || bRForward )	// �̵� Ű������ ���� �ڵ����� ����.
#else //__Y_INTERFACE_VER3
#ifdef	KEYBOARD_SET
	if( bUp || bDown || bLeft || bRight || bSpace || bLForward || bRForward )	// �̵� Ű������ ���� �ڵ����� ����.
#else
	if( bUp || bDown || bLeft || bRight || bSpace )	// �̵� Ű������ ���� �ڵ����� ����.
#endif	// KEYBOARD_SET
#endif //__Y_INTERFACE_VER3
	{

		if( bUp || bDown )
	//sun: 12, Ʃ�丮�� ����
		{
			CWndGuideSystem* pWndGuide = NULL;
			pWndGuide = (CWndGuideSystem*)GetWndBase( APP_GUIDE );
			if(pWndGuide && pWndGuide->IsVisible()) pWndGuide->m_Condition.bIsKeyMove = true;
		}
	
		m_bAutoAttack = FALSE;
		g_pPlayer->ClearCmd();
		if( !bSpace )
			ClearTracking();
	}


	if( m_objidTracking != NULL_ID )
	{
		CMover* pObjTracking	= prj.GetMover( m_objidTracking );
		if( pObjTracking )
		{
			D3DXVECTOR3 vDis	= pMover->GetPos() - pObjTracking->GetPos();
			if( D3DXVec3LengthSq( &vDis ) > 4.0f * 4.0f )	{
				pMover->SetDestObj( m_objidTracking );
				static DWORD tick = GetTickCount();
				if( tick + SEC( 3 ) < GetTickCount() )	{
					g_DPlay.SendPlayerBehavior();
					tick = GetTickCount();
				}
			}
		}
		else
			ClearTracking();
	}

	bool fMoved	= false;
	bool fBehavior	= false;
	//TCHAR str[256] = { 0 };

#ifdef KEYBOARD_SET
	/*if( bLeft ) {
		if( pMover->SendActMsg( OBJMSG_LTURN ) == 1 ) {
			FLSPrintf( str, _countof( str ), _T("Left key	\r\n") );
			OutputDebugStr( str );
			fMoved	= true;
			fCastCancel	= true;
		}
	}
	else if( bRight ) {
		if( pMover->SendActMsg( OBJMSG_RTURN ) == 1 ) {
			FLSPrintf( str, _countof( str ), _T("Right key	\r\n") );
			OutputDebugStr( str );
			fMoved	= true;
			fCastCancel	= true;
		}
	}*/
	if( bLForward && bUp) {
		if( pMover->SendActMsg( OBJMSG_LFORWARD2 ) == 1 ) {
			fMoved	= true;
			fCastCancel	= true;
		}
	}
	else if( bRForward && bUp) {
		if( pMover->SendActMsg( OBJMSG_RFORWARD2 ) == 1 ) {
			fMoved	= true;
			fCastCancel	= true;
		}
	}
	else if( bLForward && bDown) {
		if( pMover->SendActMsg( OBJMSG_LBACKWARD ) == 1 ) {
			fMoved	= true;
			fCastCancel	= true;
		}
	}
	else if( bRForward && bDown) {
		if( pMover->SendActMsg( OBJMSG_RBACKWARD ) == 1 ) {
			fMoved	= true;
			fCastCancel	= true;
		}
	}
	else if( bLForward ) {
		if( pMover->SendActMsg( OBJMSG_LFORWARD ) == 1 ) {
			fMoved	= true;
			fCastCancel	= true;
		}
	}
	else if( bRForward ) {
		if( pMover->SendActMsg( OBJMSG_RFORWARD ) == 1 ) {
			fMoved	= true;
			fCastCancel	= true;
		}
	}
	else
#endif	// KEYBOARD_SET
	if( bUp ) {
		if( pMover->SendActMsg( OBJMSG_FORWARD ) == 1 ) {

		//	FLSPrintf( str, _countof( str ), _T("up key	\r\n") );
		//OutputDebugStr( str );
			fMoved	= true;
			fCastCancel	= true;
		}
	}
	else if( bDown ) {
		if( pMover->SendActMsg( OBJMSG_BACKWARD ) == 1 ) {
			
		//FLSPrintf( str, _countof( str ), _T("down key	\r\n") );
		//OutputDebugStr( str );
			fMoved	= true;
			fCastCancel	= true;
		}
	}
#ifdef __Y_INTERFACE_VER3
	else
	if( bLForward ) {
		if( pMover->SendActMsg( OBJMSG_LFORWARD ) == 1 ) {
			fMoved	= true;
			fCastCancel	= true;
		}
	}
	else if( bRForward ) {
		if( pMover->SendActMsg( OBJMSG_RFORWARD ) == 1 ) {
			fMoved	= true;
			fCastCancel	= true;
		}
	}	
#endif //__Y_INTERFACE_VER3
	else 
//	if( (bUp == FALSE && s_bUped == TRUE) || (bDown == FALSE && s_bDowned == TRUE) )	// Ű�� �� �������� ó���غ���..
	if( bUp == FALSE || bDown == FALSE )
	{
		if( pMover->IsEmptyDest() ) 
		{
			if( pMover->m_pActMover->IsActJump() == FALSE && (pMover->m_pActMover->IsStateFlag( OBJSTAF_SIT ) ) == FALSE )	// �ɾ������� �����ϸ� �ȵȴ�.
			{
				if( pMover->SendActMsg( OBJMSG_STAND ) == 1 )
				{
					fMoved	= true;
//					FLTRACE_LOG( PROGRAM_NAME, _T( "PlayerMoved, " ) );
				}
			}
		}
	}
//		s_bUped = bUp;
//		s_bDowned = bDown;

	if( bLeft ) {
		if( pMover->SendActMsg( OBJMSG_LTURN ) == 1 ) {
		//	FLSPrintf( str, _countof( str ), _T("Left key	\r\n") );
		//OutputDebugStr( str );
			fMoved	= true;
		}
	}
	else if( bRight ) {
		if( pMover->SendActMsg( OBJMSG_RTURN ) == 1 ) {
		//	FLSPrintf( str, _countof( str ), _T("Right key	\r\n") );
		//OutputDebugStr( str );
			fMoved	= true;
		}
	}
	else {

		if( pMover->SendActMsg( OBJMSG_STOP_TURN ) == 1 ) {
		//	FLSPrintf( str, _countof( str ), _T("Stop	\r\n") );
		//OutputDebugStr( str );
			fMoved	= true;
//			fBehavior	= true;
		}
	}


//	jump
	if( bSpace ) 
	{
	//sun: 12, Ʃ�丮�� ����
		if( m_pWndGuideSystem )
			m_pWndGuideSystem->SendGuideMessage(GUIDE_EVENT_KEY_JUMP);
	
		if( pMover->SendActMsg( OBJMSG_JUMP ) == 1 ) 
		{
			fBehavior	= true;
			fCastCancel	= true;
		}
	}
	if( m_bLButtonDown == TRUE && m_bRButtonDown == TRUE ) {
		if( m_timerLButtonDown.GetLeftTime() < 500 && m_timerRButtonDown.GetLeftTime() < 500 ) {
			if( g_pPlayer->SendActMsg( OBJMSG_JUMP ) == 1 ) {
				fBehavior	= true;
				fCastCancel	= true;
			}
		}
	}
#ifdef __Y_INTERFACE_VER3	
	if( g_Option.m_nInterface == 2 )
	{
		if( g_bKeyTable[VK_DIVIDE] || g_bKeyTable[191] )
		{
			bWalk = TRUE;		
		}
		else
		{
			bWalk = FALSE;			
		}	
	}
	else
	{
		bWalk = g_bKeyTable[ pShortcutKey_Control->KeyWalk ];	
	}
#else //__Y_INTERFACE_VER3	
	bWalk = g_bKeyTable[ pShortcutKey_Control->KeyWalk ];	
#endif //__Y_INTERFACE_VER3	
	if( bWalk && !s_bWalk2 )		// �ȱ� ��� ���.
	{
		if( pMover->m_pActMover->IsStateFlag( OBJSTAF_WALK ) )
		{
			if( pMover->SendActMsg( OBJMSG_MODE_RUN ) == 1 ) {
				g_WndMng.PutString( prj.GetText( TID_GAME_RUN ), NULL, prj.GetTextColor( TID_GAME_RUN ) , CHATSTY_SYSTEM_CLIENT );
				fBehavior	= true;
			}
		} else
		{
		//sun: 12, Ʃ�丮�� ����
			if(m_pWndGuideSystem)
				m_pWndGuideSystem->SendGuideMessage(GUIDE_EVENT_KEY_RUN);
		
			if( pMover->SendActMsg( OBJMSG_MODE_WALK ) == 1 ) {
				g_WndMng.PutString( prj.GetText( TID_GAME_WALK ), NULL, prj.GetTextColor( TID_GAME_WALK ) , CHATSTY_SYSTEM_CLIENT );		
				fBehavior	= true;
			}
		}
	}
	s_bWalk2 = bWalk;

	if( fMoved || fBehavior ) 
	{
		g_pPlayer->ClearDest();
		g_DPlay.SendPlayerBehavior();
	}

	if( g_pPlayer->IsStateMode( STATE_BASEMOTION_MODE ) && fCastCancel ) // ĳ��Ʈ ���
	{
		g_DPlay.SendStateModeCancel( STATE_BASEMOTION_MODE, STATEMODE_BASEMOTION_CANCEL );
	}


	// ��ڰ� ���� Ű. �����κ��� ��ǥ�޾ƿ���.
	if( bTempKey = g_bKeyTable[ pShortcutKey_System->Key8 ] )
	{
		if( !m_bTemp3ed )
		{
			pMover->SendActMsg( OBJMSG_TEMP2 );
		}
	}
	m_bTemp3ed	= bTempKey;

//------------ ����� Ÿ��
	if( bBoard )
	{
		if( !s_bBoarded )		// �÷��̾ ������� �ö�Ÿ�ִ� ���¿���. ž��Ű�� ������.
		{
			if( g_pShip == NULL )
			{
				if( g_pPlayer->GetIAObjLink() && g_pPlayer->GetIAObjLink()->GetType() == OT_SHIP && g_pPlayer->GetIAObjLink()->GetIndex() == 3 )
				{
					CShip *pShip = (CShip *)g_pPlayer->GetIAObjLink();
					if( pShip->GetMover() == NULL )		// ������ ���� ���϶�.
					{
						pShip->SetMover( g_pPlayer );	// ������ g_pPlayer�� ����.
						g_pShip = pShip;
							
					}
				}
			} else
			// �̹� �踦 �����ϰ� ������
			{
				g_pShip->SetMover( NULL );
				g_pShip = NULL;
			}
		}
	}
	s_bBoarded = bBoard;

	
#ifdef _DEBUG
	// ������ Ű
	if( bTempKey = g_bKeyTable[ pShortcutKey_System->KeyF2 ] )
	{
		if( !s_bTempKeyed )
		{
//			pMover->SendActMsg( OBJMSG_TEMP );
//			g_Option.m_nObjectDetail ++;
//			if( g_Option.m_nObjectDetail > 2 )	g_Option.m_nObjectDetail = 0;
		}
	}
	s_bTempKeyed = bTempKey;
	if( bTempKey = g_bKeyTable[ 'F' ] )
	{
		if( !m_bTemp2ed )
		{
			pMover->SendActMsg( OBJMSG_TEMP3 );
		}
	}
	m_bTemp2ed = bTempKey;
#endif
	
	return nMsg;
}
DWORD m_dwIdTarget = NULL_ID;
//
//		������Ʈ�� �����̴µ� �ʿ��� �޽����� �߻�
//		���ڷ� ������
//
extern	int	g_nDrift;
int		CWndWorld::ControlFlying( DWORD dwMessage, CPoint point )
{
 	BOOL		bUp, bDown, bLeft, bRight;
	BOOL		bTurbo;

	static		float	fTurnAngle	= 0.0f;
	static		BOOL	s_bTraceKeyed = 0, s_bSelectKeyed = 0, s_bTurbo2 = 0;
	int			nMsg( 0 );
	BOOL		bAcc( FALSE );
	BYTE		nFrame(	MAX_CORR_SIZE_150 );
	CMover*		pMover( CMover::GetActiveMover() );
	
	const PFLSHORTCUTKEY_CONTROL	pShortcutKey_Control( g_xKeyManager->GetShortcutKey_Control() );
	const PFLSHORTCUTKEY_SYSTEM		pShortcutKey_System( g_xKeyManager->GetShortcutKey_System() );

	bUp	  = g_bKeyTable[ pShortcutKey_Control->KeyUp		];
	bDown = g_bKeyTable[ pShortcutKey_Control->KeyDown		];

	// ��/�� ȸ��
	bLeft  = g_bKeyTable[ pShortcutKey_Control->KeyLeft		];
	bRight = g_bKeyTable[ pShortcutKey_Control->KeyRight	];
	

	// ���� ���¸� ���� ��� ��� ����
	bool fMoved	= false;
	bool fBehavior	= false;
	
	if( pMover->m_pActMover->IsStateFlag( OBJSTAF_ACC ) ) {
		if( pMover->SendActMsg( OBJMSG_FORWARD ) == 1 ) {
			fMoved	= true;
		}
	}
	else {
		if( pMover->SendActMsg( OBJMSG_STAND ) == 1 ) {
			fMoved	= true;
		}
	}
	//

	bAcc	= g_bKeyTable[ pShortcutKey_Control->KeyJump ];
	if( bAcc && !s_bAccKeyed ) 		// Ű ������������ ��۽�Ŵ.
	{
		if( pMover->m_pActMover->IsStateFlag( OBJSTAF_ACC ) ) 		// �������̾��ٸ� 
		{
			pMover->SendActMsg( OBJMSG_ACC_STOP );		// ���� ����
			if( pMover->m_pActMover->IsActTurn() )
			{
				fMoved	= true;
			}
		}
		else 
		{
			// �������� �ƴϾ��ٸ� ���� ��Ŵ.
			if( pMover->SendActMsg( OBJMSG_ACC_START ) == 0 )		
				g_WndMng.PutString( prj.GetText( TID_GAME_AIRFUELEMPTY ) );
			else
			{
				if( pMover->SendActMsg( OBJMSG_FORWARD ) == 1 ) 
					fMoved	= true;
			}
		}
	}
	s_bAccKeyed = bAcc;

	bTurbo = g_bKeyTable[ pShortcutKey_Control->KeyWalk ];
	if( bTurbo && !s_bTurbo2 )		// ��� ���.
	{
		if( pMover->m_pActMover->IsStateFlag( OBJSTAF_TURBO ) )
		{
			if( pMover->SendActMsg( OBJMSG_MODE_TURBO_OFF ) == 1 )
				fMoved = true;
		} else
		{
			if( pMover->SendActMsg( OBJMSG_MODE_TURBO_ON ) == 1 )
				fMoved = true;
		}
	}
	s_bTurbo2 = bTurbo;
	
	if( pMover->m_pActMover->IsFly() )
	{
		if( g_bKeyTable[ pShortcutKey_Control->KeyTrace ] && !s_bTraceKeyed )
		{
			CCtrl* pFocusObj = (CCtrl*)(pMover->GetWorld()->GetObjFocus());
			if( pFocusObj && pFocusObj->GetType() == OT_MOVER )
			{
				CMover* pFocusMover = (CMover*)pFocusObj;
				if( pMover->m_dwFlag & MVRF_TRACKING )	// �̹� �������̸� ����.
				{
					pMover->m_dwFlag &= (~MVRF_TRACKING);		// �����������.
					pMover->m_idTracking = NULL_ID;
				} else
				{
					// ������ �������.
					pMover->m_dwFlag |= MVRF_TRACKING;		// �������.
					pMover->m_idTracking = pFocusMover->GetId();
				}
			} else
			{	// Ÿ���� ������ ZŰ�� ������ �ڵ������� Ǯ����.
				pMover->m_dwFlag &= (~MVRF_TRACKING);		// �����������.
				pMover->m_idTracking = NULL_ID;
			}
		}
		s_bTraceKeyed = g_bKeyTable[ pShortcutKey_Control->KeyTrace ];

		// Ÿ�ټ��� Ű
		if( g_bKeyTable[ pShortcutKey_Control->KeySelectNearByTarget ] && !s_bSelectKeyed )
		{
			if( m_aFlyTarget.GetSize() > 0 )		// ���õ� Ÿ��������.
			{
				if( m_nSelect >= m_aFlyTarget.GetSize() )
					m_nSelect = 0;
				OBJID idSelect = m_aFlyTarget.GetAt( m_nSelect++ );
				CMover *pSelectMover = prj.GetMover( idSelect );
				if( IsValidObj(pSelectMover) )
				{
					CWorld *pWorld = pMover->GetWorld();
					if( pWorld )
					{
						pWorld->SetObjFocus( pSelectMover );			// �̳��� Ÿ������ ������.
						pMover->m_idTracking = pSelectMover->GetId();	// ������ Ÿ���� �ٲٸ� �ڵ�����Ÿ�ٵ� �׳����� �ٲ��.
					}
				}
			}
		}
		s_bSelectKeyed = g_bKeyTable[ pShortcutKey_Control->KeySelectNearByTarget ];
	}



	if( /*m_bFlyMove &&*/ m_bLButtonDown || g_bKeyTable[VK_INSERT] )	// 192 = `
	{
		CObj *pObj = pMover->GetWorld()->GetObjFocus();		// Ÿ���������� �������� �ֵθ��� �ִ�.
		if( pObj && pObj->GetType() == OT_MOVER )
		{
			if( pMover->IsAttackAble( pObj ) )	// ���� �������� �˻�.
			{
				OBJID	idTarget = ((CMover *)pObj)->GetId();
				PT_ITEM_SPEC pWeapon = pMover->GetActiveHandItemProp();
				if( pWeapon )
				{
					g_pPlayer->PlayCombatMusic();

					if( pWeapon->dwItemKind3 == IK3_WAND )
					{
						D3DXVECTOR3 vFront, vTarget;
						AngleToVector( &vFront, g_pPlayer->GetAngle(), -g_pPlayer->GetAngleX(), 1.0f );
						vTarget = pObj->GetPos() - g_pPlayer->GetPos();
						D3DXVec3Normalize( &vTarget, &vTarget );		// Ÿ���������� ������ ���ֺ���.
						FLOAT fDot = D3DXVec3Dot( &vFront, &vTarget );
						if( fDot >= cosf(D3DXToRadian(60.0f)) )	// Ÿ���� ���� ���� ������ +-30�� �ȿ� ������ �߻��Ҽ� �ִ�.
						{
							if( pMover->IsRangeObj( pObj, 64.0f ) )		// �����Ÿ��� ������ �߻�.
							{
								pMover->DoAttackMagic( pObj, 0 );
							}
						}
					}
					else
					{
						pMover->SendActMsg( OBJMSG_ATK1, idTarget );
					}
				}
			}
		}
	}

//	fTurnAngle	= 0.6f;
	PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( g_pPlayer->GetRideItemIdx() );
	if( pItemProp )
	{
		fTurnAngle = pItemProp->fFlightLRAngle;
	}
	else
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "���ڷ����� �б� ���� %d" ), g_pPlayer->GetRideItemIdx() );
		fTurnAngle = 0.6f;
	}

	if( bUp ) {
		if( g_WorldMng.Get()->GetFullHeight( pMover->GetPos() ) < pMover->GetPos().y ) {
			if( pMover->SendActMsg( OBJMSG_LOOKDOWN ) == 1 ) {
				fMoved	= true;
			}
		}
	}
	else if( bDown ) {
		if( pMover->SendActMsg( OBJMSG_LOOKUP ) == 1 ) {
			fMoved	= true;
		}
	}
	else {
		if( pMover->SendActMsg( OBJMSG_STOP_LOOK ) == 1 ) {
			fMoved	= true;
		}
	}

	if( bLeft ) {
		m_fRollAng -= 1.0f;
		if( m_fRollAng < -45.0f )	
			m_fRollAng = -45.0f;
		if( pMover->SendActMsg( OBJMSG_LTURN, (int)( fTurnAngle * 100.0f ) ) == 1 ) {
			fMoved	= true;
		}
	}
	else if( bRight ) {
		m_fRollAng += 1.0f;
		if( m_fRollAng > 45.0f )	
			m_fRollAng = 45.0f;
		if( pMover->SendActMsg( OBJMSG_RTURN, (int)( fTurnAngle * 100.0f ) ) == 1 ) {
			fMoved	= true;
		}
	}
	else {
		if( m_fRollAng < 0 )
		{
			m_fRollAng += 2.0f;
			if( m_fRollAng > 0 )	m_fRollAng = 0;
		} else
		if( m_fRollAng > 0 )
		{
			m_fRollAng -= 2.0f;
			if( m_fRollAng < 0 )	m_fRollAng = 0;
		}
		if( pMover->SendActMsg( OBJMSG_STOP_TURN ) == 1 ) {
			fMoved	= true;
//			fBehavior	= true;
		}
	}

	// ������ ��ư �巡�״� ���ڷ� ������
	if( dwMessage == WM_MOUSEMOVE /*&& m_bRButtonDown*/ )
	{
		float fAng = pMover->GetAngle();
		float fAdd = (point.x - m_ptMouseOld.x) / 2.0f;
		fAng -= fAdd;
		pMover->SetAngle( fAng );
		
		float fAngX = pMover->GetAngleX();
		float fAddX = (point.y - m_ptMouseOld.y) / 4.0f;
		fAngX += fAddX;
		if( fAddX > 0 && fAngX > 45.0f )
			fAngX = 45.0f;
		else
		if( fAddX < 0 && fAngX < -45.0f )
			fAngX = -45.0f;
			
		pMover->SetAngleX( fAngX );

		if( fAdd || fAddX )
			g_DPlay.PostPlayerAngle( TRUE );
	}
	


	BOOL bTempKey;
	if( bTempKey = g_bKeyTable[ pShortcutKey_System->Key8 ] )
	{
		if( !m_bTemp3ed )
		{
			pMover->SendActMsg( OBJMSG_TEMP2 );
			//			__bTestLOD ^= 1;
		}
	}
	m_bTemp3ed	= bTempKey;
	


	if( fBehavior )
	{
		pMover->ClearDest();
	}

	if( fMoved || fBehavior )
	{
		g_DPlay.SendPlayerBehavior();
	}
		
	return nMsg;
}

//
//		�踦 �����̴µ� �ʿ��� �޽����� �߻�
//
int		CWndWorld::ControlShip( DWORD dwMessage, CPoint point )
{
	if( g_pShip == NULL )	return 0;
	
	g_pShip->Control();

	return 0;
}
