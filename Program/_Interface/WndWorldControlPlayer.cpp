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
//sun: 10차 전승시스템	Neuz, World, Trans

//	if( g_pPlayer->GetAdjParam( DST_CHRSTATE ) & CHS_LOOT)			return 0;

	if( g_pPlayer->GetAdjParam( DST_CHRSTATE ) & CHS_SETSTONE)
		return 0;

	if( g_pPlayer->m_vtInfo.VendorIsVendor() )
		return 0;
	if( g_WndMng.m_bAllAction == FALSE )
		return 0;

	if( g_WndMng.m_pLogOutWaitting ) // 종료중이면 플레이어 컨트롤 불가
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

	if( g_pPlayer->m_dwReqFlag & REQ_USESKILL )	return 0;		// 서버로부터 useskill응답이 오기전까진 액션해선 안됨.
	
	if( g_WndMng.GetWndBase(APP_MAP_TELEPORT) )//텔레포트 지도아이템을 쓰고 있으면 컨트롤불가 
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
//		오브젝트를 움직이는데 필요한 메시지를 발생
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

	// 전진/후진/스톱
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

	//gmpbigsun : 키보드 조작중에는 마우스이동 불가 
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

	// 좌/우 회전	//05
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

//sun: 12, 몬스터 생성 Neuz, World
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
				float fDist = D3DXVec3Length( &vDist2 );			// 두좌표간의 거리
				if( 15.f < fDist )
				{
					g_WndMng.PutString( prj.GetText( TID_GAME_CREATEMON_F_15 ), NULL, prj.GetTextColor( TID_GAME_CREATEMON_F_15 ) );
					bSendCM = FALSE;
				}
				if( bSendCM )
				{
					int nAttr = g_pPlayer->GetWorld()->GetHeightAttribute( g_Neuz.m_vCursorPos.x, g_Neuz.m_vCursorPos.z );		// 이동할 위치의 속성 읽음.
					if( nAttr == HATTR_NOWALK || nAttr == HATTR_NOMOVE 
						|| g_pPlayer->IsRegionAttr( RA_SAFETY ) || g_pPlayer->GetWorld()->GetID() == WI_WORLD_GUILDWAR )		// 못 움직이는 곳이거나 안전지역이면 Pass
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

	//TODO:ata3k님 꼭 고쳐주세요. 왜 그런지 아무도 몰라!
	// 이동금지 상태가 아닐때만 클릭으로 이동할수 있다.
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
			// 이동 포인트를 얻어 목표 세팅 
			if( m_bFreeMove )
			{
	//			if( m_bLButtonDown )	// 이동금지 상태가 아닐때만 클릭으로 이동할수 있다.
				{
					{
						if( m_pWndGuideSystem  && m_pWndGuideSystem->IsVisible())
							//sun: 12, 튜토리얼 개선
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
	if( bUp || bDown || bLeft || bRight || bSpace || bLForward || bRForward )	// 이동 키조작이 들어가면 자동공격 멈춤.
#else //__Y_INTERFACE_VER3
#ifdef	KEYBOARD_SET
	if( bUp || bDown || bLeft || bRight || bSpace || bLForward || bRForward )	// 이동 키조작이 들어가면 자동공격 멈춤.
#else
	if( bUp || bDown || bLeft || bRight || bSpace )	// 이동 키조작이 들어가면 자동공격 멈춤.
#endif	// KEYBOARD_SET
#endif //__Y_INTERFACE_VER3
	{

		if( bUp || bDown )
	//sun: 12, 튜토리얼 개선
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
//	if( (bUp == FALSE && s_bUped == TRUE) || (bDown == FALSE && s_bDowned == TRUE) )	// 키를 뗀 순간에만 처리해보자..
	if( bUp == FALSE || bDown == FALSE )
	{
		if( pMover->IsEmptyDest() ) 
		{
			if( pMover->m_pActMover->IsActJump() == FALSE && (pMover->m_pActMover->IsStateFlag( OBJSTAF_SIT ) ) == FALSE )	// 앉아있을땐 실행하면 안된다.
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
	//sun: 12, 튜토리얼 개선
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
	if( bWalk && !s_bWalk2 )		// 걷기 모드 토글.
	{
		if( pMover->m_pActMover->IsStateFlag( OBJSTAF_WALK ) )
		{
			if( pMover->SendActMsg( OBJMSG_MODE_RUN ) == 1 ) {
				g_WndMng.PutString( prj.GetText( TID_GAME_RUN ), NULL, prj.GetTextColor( TID_GAME_RUN ) , CHATSTY_SYSTEM_CLIENT );
				fBehavior	= true;
			}
		} else
		{
		//sun: 12, 튜토리얼 개선
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

	if( g_pPlayer->IsStateMode( STATE_BASEMOTION_MODE ) && fCastCancel ) // 캐스트 취소
	{
		g_DPlay.SendStateModeCancel( STATE_BASEMOTION_MODE, STATEMODE_BASEMOTION_CANCEL );
	}


	// 운영자가 쓰는 키. 서버로부터 좌표받아오기.
	if( bTempKey = g_bKeyTable[ pShortcutKey_System->Key8 ] )
	{
		if( !m_bTemp3ed )
		{
			pMover->SendActMsg( OBJMSG_TEMP2 );
		}
	}
	m_bTemp3ed	= bTempKey;

//------------ 비공정 타기
	if( bBoard )
	{
		if( !s_bBoarded )		// 플레이어가 비공정에 올라타있는 상태에서. 탑승키를 누르면.
		{
			if( g_pShip == NULL )
			{
				if( g_pPlayer->GetIAObjLink() && g_pPlayer->GetIAObjLink()->GetType() == OT_SHIP && g_pPlayer->GetIAObjLink()->GetIndex() == 3 )
				{
					CShip *pShip = (CShip *)g_pPlayer->GetIAObjLink();
					if( pShip->GetMover() == NULL )		// 쥔장이 없는 배일때.
					{
						pShip->SetMover( g_pPlayer );	// 쥔장을 g_pPlayer로 설정.
						g_pShip = pShip;
							
					}
				}
			} else
			// 이미 배를 조종하고 있을때
			{
				g_pShip->SetMover( NULL );
				g_pShip = NULL;
			}
		}
	}
	s_bBoarded = bBoard;

	
#ifdef _DEBUG
	// 디버깅용 키
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
//		오브젝트를 움직이는데 필요한 메시지를 발생
//		빗자루 비행모드
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

	// 좌/우 회전
	bLeft  = g_bKeyTable[ pShortcutKey_Control->KeyLeft		];
	bRight = g_bKeyTable[ pShortcutKey_Control->KeyRight	];
	

	// 가속 상태면 전진 명령 계속 보냄
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
	if( bAcc && !s_bAccKeyed ) 		// 키 누른순간에만 토글시킴.
	{
		if( pMover->m_pActMover->IsStateFlag( OBJSTAF_ACC ) ) 		// 가속중이었다면 
		{
			pMover->SendActMsg( OBJMSG_ACC_STOP );		// 가속 멈춤
			if( pMover->m_pActMover->IsActTurn() )
			{
				fMoved	= true;
			}
		}
		else 
		{
			// 가속중이 아니었다면 가속 시킴.
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
	if( bTurbo && !s_bTurbo2 )		// 토글 방식.
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
				if( pMover->m_dwFlag & MVRF_TRACKING )	// 이미 실행중이면 해제.
				{
					pMover->m_dwFlag &= (~MVRF_TRACKING);		// 추적모드해제.
					pMover->m_idTracking = NULL_ID;
				} else
				{
					// 비행중 추적모드.
					pMover->m_dwFlag |= MVRF_TRACKING;		// 추적모드.
					pMover->m_idTracking = pFocusMover->GetId();
				}
			} else
			{	// 타겟이 없을때 Z키를 누르면 자동추적이 풀린다.
				pMover->m_dwFlag &= (~MVRF_TRACKING);		// 추적모드해제.
				pMover->m_idTracking = NULL_ID;
			}
		}
		s_bTraceKeyed = g_bKeyTable[ pShortcutKey_Control->KeyTrace ];

		// 타겟선택 키
		if( g_bKeyTable[ pShortcutKey_Control->KeySelectNearByTarget ] && !s_bSelectKeyed )
		{
			if( m_aFlyTarget.GetSize() > 0 )		// 선택된 타겟있을때.
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
						pWorld->SetObjFocus( pSelectMover );			// 이놈을 타겟으로 설정함.
						pMover->m_idTracking = pSelectMover->GetId();	// 탭으로 타겟을 바꾸면 자동추적타겟도 그놈으로 바뀐다.
					}
				}
			}
		}
		s_bSelectKeyed = g_bKeyTable[ pShortcutKey_Control->KeySelectNearByTarget ];
	}



	if( /*m_bFlyMove &&*/ m_bLButtonDown || g_bKeyTable[VK_INSERT] )	// 192 = `
	{
		CObj *pObj = pMover->GetWorld()->GetObjFocus();		// 타겟잡힌놈이 있을때만 휘두를수 있다.
		if( pObj && pObj->GetType() == OT_MOVER )
		{
			if( pMover->IsAttackAble( pObj ) )	// 공격 가능한지 검사.
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
						D3DXVec3Normalize( &vTarget, &vTarget );		// 타겟쪽으로의 벡터의 유닛벡터.
						FLOAT fDot = D3DXVec3Dot( &vFront, &vTarget );
						if( fDot >= cosf(D3DXToRadian(60.0f)) )	// 타겟이 내가 보는 방향의 +-30도 안에 있으면 발사할수 있다.
						{
							if( pMover->IsRangeObj( pObj, 64.0f ) )		// 사정거리에 들어오면 발사.
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
		FLERROR_LOG( PROGRAM_NAME, _T( "빗자루정보 읽기 실패 %d" ), g_pPlayer->GetRideItemIdx() );
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

	// 오른쪽 버튼 드래그는 빗자루 움직임
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
//		배를 움직이는데 필요한 메시지를 발생
//
int		CWndWorld::ControlShip( DWORD dwMessage, CPoint point )
{
	if( g_pShip == NULL )	return 0;
	
	g_pShip->Control();

	return 0;
}
