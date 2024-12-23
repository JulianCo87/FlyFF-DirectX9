#include "stdafx.h"
#include "party.h"
 
#ifdef __CLIENT
	#include "Sfx.h"
	#include "../Neuz/DPClient.h"
	#include "DialogMsg.h"
	extern	CDPClient	g_DPlay;
	#include "defineText.h"
	#include "defineSound.h"
#endif	// __CLIENT

#ifdef	__WORLDSERVER
  #include "../WorldServer/User.h"
  #include "WorldMng.h"
  extern	CWorldMng	g_WorldMng;
#endif	// __WORLDSERVER

extern	CPartyMng	g_PartyMng;


#ifdef __CLIENT

void CMover::PlayCombatMusic()
{
	//gmpbigsun: 전투음악 on off

	if( g_Option.m_bBattleBGM == FALSE )
		return;

	if( GetWorld()->GetID() == WI_WORLD_GUILDWAR )
		return;

	if( GetWorld()->GetID() == WI_DUNGEON_MUSCLE || GetWorld()->GetID() == WI_DUNGEON_KRRR || GetWorld()->GetID() == WI_DUNGEON_BEAR )
		return;
		
	if( GetWorld()->m_bIsIndoor == FALSE )
	{
		FLOAT x = GetPos().x;
		FLOAT z = GetPos().z;
		CRect rectSaintmorning( 7535, 1846, 9283, 3134 );

		if( rectSaintmorning.PtInRect( CPoint( (int)( x ), (int)( z ) ) ) )
			PlayMusic( BGM_BA_SAINTMORNING );
		else
		if( x < 4609 )
			PlayMusic( BGM_BA_DARKON );
		else
		if( x < 6309 )
			PlayMusic( BGM_BA_CRISIS );
		else
		if( x < 8138 )
			PlayMusic( BGM_BA_FLARIS );
		else
			PlayMusic( BGM_BA_SAINTMORNING );
	}
}
//
//
// 클라이언트용
void CMover::ProcessMoveArrival( CCtrl *pObj )
{
	// 클라이언트 처리
	if( IsActiveMover() )
	{
		BOOL bSendUseSkill = FALSE;
		switch( m_oaCmd )	// 목표에 도착한 후의 명령 처리.
		{
		case OBJACT_USESKILL:
			if( pObj->GetType() == OT_MOVER && ( m_SkillTimerStop || m_SkillTimer.TimeOut() ) )
			{
				CWorld *pWorld = GetWorld();
				D3DXVECTOR3 vStart = GetPos();			vStart.y += 0.5f;
				D3DXVECTOR3 vEnd   = pObj->GetPos();	vEnd.y += 0.5f;

				if( pWorld->IntersectObjLine( NULL, vStart, vEnd, FALSE, FALSE ) )
				{
					g_WndMng.PutString( prj.GetText( TID_GAME_BLOCKTARGETING ), NULL, prj.GetTextColor( TID_GAME_BLOCKTARGETING ) );
					g_WndMng.m_pWndWorld->SetNextSkill( NEXTSKILL_NONE );
					break;
				}

				PlayCombatMusic();

				int nSkillIdx = GetCmdParam(0);
				OBJID idTarget = (OBJID)GetCmdParam(1);
				SKILLUSETYPE sutType = (SKILLUSETYPE)GetCmdParam(2);
				if( (m_dwReqFlag & REQ_USESKILL) == 0 )	// 응답 요청중일땐 다시 보내선 안된다.
				{
					LPSKILL pSkill	= GetSkill( 0, nSkillIdx );		// this가 가진 스킬중 nIdx에 해당하는 스킬을 꺼낸다.
					if( pSkill == NULL )
					{
						FLERROR_LOG( PROGRAM_NAME, _T( "%s skill(%d) not found" ), m_szName, nSkillIdx );
						return;	// skill not found
					}

					if( pSkill->dwSkill == SKILL_INDEX( 107, SI_MAG_MAG_BLINKPOOL ) )
					{
						CWndWorld* pWndWorld;
						pWndWorld = (CWndWorld*)g_WndMng.m_pWndWorld;		
						{
							vStart = GetPos();		vStart.y += 1.0f;
							vEnd = pWndWorld->m_vTelePos;
							if( pWorld->IntersectObjLine( NULL, vStart, vEnd, FALSE, FALSE ) )
							{
								g_WndMng.m_pWndWorld->SetNextSkill( NEXTSKILL_NONE );
								g_WndMng.PutString( prj.GetText( TID_GAME_BLOCKTARGETING ), NULL, prj.GetTextColor( TID_GAME_BLOCKTARGETING ) );
								break;
							}
						}
						
						//if(g_pMoveMark!=NULL) g_pMoveMark->m_pSfxObj->m_nCurFrame=180;
						g_pMoveMark = NULL;

						CreateSfx(g_Neuz.m_pd3dDevice,XI_INDEX( 63, XI_GEN_MOVEMARK01 ),pWndWorld->m_vTelePos);
					}

					else if( pSkill->dwSkill == SKILL_INDEX( 412, SI_WIN_YOYO_BACKSTEP ) )
					{
						if( pWorld->IntersectObjLine( NULL, vStart, vEnd, FALSE, FALSE ) == TRUE )
						{
							g_WndMng.m_pWndWorld->SetNextSkill( NEXTSKILL_NONE );
							g_WndMng.PutString( prj.GetText( TID_GAME_BLOCKTARGETING ), NULL, prj.GetTextColor( TID_GAME_BLOCKTARGETING ) );
							break;
						}
					}

					// 뒤에서 공격가능한 스킬인지 판단한다
					// 강탈 스킬은 뒤에서 사용가능(일단 클라에서 판정하자~)
					if( pSkill->GetProp() && pSkill->GetProp()->dwAtkStyle == AS_BACK )
					{						
						D3DXVECTOR3 v3Pos;
						D3DXVECTOR3 v3PosSrc;
						D3DXVECTOR3 v3PosDest;
						
						// 방향벡터 1
						v3PosSrc = pObj->GetPos() - GetPos();
						D3DXVec3Normalize( &v3PosSrc, &v3PosSrc );
						
						// 방향벡터 2
						AngleToVectorXZ( &v3Pos, pObj->GetAngle(), 3.0f );
						v3PosDest = (pObj->GetPos()+v3Pos) - pObj->GetPos();
						D3DXVec3Normalize( &v3PosDest, &v3PosDest );
						
						FLOAT fDir = D3DXVec3Dot( &v3PosSrc, &v3PosDest );

						// 뒤가 아니면 스킬 사용 불가!
						if( fDir < 0.3f )
						{
							g_WndMng.PutString( prj.GetText(TID_GAME_NEVERKILLSTOP) );
							break;
						}
					}
					
					// 카오에게 좋은 스킬을 사용할때는 Control 키를 눌러야 함
					if( g_eLocal.GetState( EVE_PK ) )
					{
						CMover * pMover;
						pMover = prj.GetMover( idTarget );
						if( IsValidObj(pMover) && pMover != g_pPlayer && pMover->IsPlayer() && pMover->IsChaotic() )
							if( pSkill->GetProp()->IsDebuff() == FALSE ) // 좋은 스킬
								if( !(GetAsyncKeyState(VK_CONTROL) & 0x8000) )
									break;
					}
					
					FLTRACE_LOG( PROGRAM_NAME, _T( "OBJACT_USESKILL %d" ), nSkillIdx );
					
					ClearDestObj();
					g_DPlay.SendPlayerBehavior();
					SendActMsg( OBJMSG_STOP );		// 제자리 멈춤.
					
					BOOL bControl = ((GetAsyncKeyState(VK_CONTROL) & 0x8000)? TRUE:FALSE);
					g_DPlay.SendUseSkill( 0, nSkillIdx, idTarget, sutType, bControl );	// 목표지점에 도착하면 스킬쓴다고 알림.
					bSendUseSkill = TRUE;

					m_dwReqFlag |= REQ_USESKILL;	// 응답 요청중
					
				}
				if( !m_SkillTimerStop )
					m_SkillTimer.Reset();
			}
			break;
		//------------------------------------------
		case OBJACT_MELEE_ATTACK:
			if( pObj->GetType() == OT_MOVER )
			{
				PT_ITEM_SPEC pItemProp = GetActiveHandItemProp(); 
				if( pItemProp && pItemProp->dwItemKind3 == IK3_YOYO )
				{
					CWorld *pWorld = GetWorld();
					D3DXVECTOR3 vStart = GetPos();			vStart.y += 0.5f;
					D3DXVECTOR3 vEnd   = pObj->GetPos();	vEnd.y += 0.5f;
					
					if( pWorld->IntersectObjLine( NULL, vStart, vEnd, FALSE, FALSE ) )
					{
						g_WndMng.PutString( prj.GetText( TID_GAME_BLOCKTARGETING ), NULL, prj.GetTextColor( TID_GAME_BLOCKTARGETING ) );
						break;
					}
				}
				
				ClearDestObj();
				g_DPlay.SendPlayerBehavior();
				DoAttackMelee( (CMover *)pObj );		// pObj를 일반공격.
			}
			break;
		//---------------------------------------------
		case OBJACT_MAGIC_ATTACK:
			if( pObj->GetType() == OT_MOVER )
			{
				PlayCombatMusic();

				OBJID	idTarget = GetCmdParam(0);
				int		nMagicPower = GetCmdParam(1);
				CMover *pTarget = prj.GetMover( idTarget );		// 타겟의 아이디를 포인터로 읽음.
				if( IsInvalidObj(pTarget) )		break;			// 타겟이 거시기한 포인터면 취소시킴.
				
				ClearDestObj();
				g_DPlay.SendPlayerBehavior();
				DoAttackMagic( pTarget, nMagicPower );
			}
			break;
		case OBJACT_RANGE_ATTACK:
			{
				if( pObj->GetType() == OT_MOVER )
				{
					PlayCombatMusic();

					OBJID	idTarget = GetCmdParam(0);
					int		nPower = GetCmdParam(1);
					CMover *pTarget = prj.GetMover( idTarget );		// 타겟의 아이디를 포인터로 읽음.
					if( IsInvalidObj(pTarget) )		break;			// 타겟이 거시기한 포인터면 취소시킴.
					
					ClearDestObj();
					g_DPlay.SendPlayerBehavior();
					DoAttackRange( pTarget, nPower, 0 );			// nPower를 dwItemID에 넣는다.
				}				
			}
			break;
		//---------------------------------------------
		case OBJACT_USEITEM:
			ClearDestObj();	// 그외는 목표에 도착하면 멈춤.
			SendActMsg( OBJMSG_STOP );
			SetAngle( GetDegree(pObj->GetPos(), GetPos()) );		// 목표쪽으로 몸을 돌림.
			break;
		//---------------------------------------------
		case OBJACT_COLLECT:
//			ClearDestObj();	// 그외는 목표에 도착하면 멈춤.
//			SendActMsg( OBJMSG_STOP );
//			SetAngle( GetDegree(pObj->GetPos(), GetPos()) );		// 목표쪽으로 몸을 돌림.
//			g_DPlay.SendDoCollect( pObj );						// 서버로 보냄.
			break;
		//---------------------------------------------
		default:
			ClearDestObj();	// 그외는 목표에 도착하면 멈춤.
			SendActMsg( OBJMSG_STOP );
			g_DPlay.SendPlayerBehavior();
			break;
			
		}

		//gmpbigsun( 20110417, #23623 ActionSlot bug ) : 클라자체로 Actionslot 을 대기상태로 만들었으나  Send 실패시 원상태로 복귀
		if( OBJACT_USESKILL == m_oaCmd && FALSE == bSendUseSkill )
		{
			if( g_WndMng.m_pWndTaskBar )
				g_WndMng.m_pWndTaskBar->m_nExecute = 0;
		}

		SetCmd( OBJACT_NONE );
	}
	else
	{
		if( IsPlayer() )
			SendActMsg( OBJMSG_STOP );
	}
}
#else // Client
//
//
// 월드서버용
void CMover::ProcessMoveArrivalBySynchronizer()
{
	CCtrl* pObj = GetDestObj();
	if( ::IsValidObj( pObj ) == TRUE )
		ProcessMoveArrival( pObj );
}

void CMover::ProcessMoveArrival( CCtrl *pObj )
{
	switch( m_oaCmd )	// 목표에 도착한 후의 명령 처리.
	{
	case OBJACT_USESKILL:
		if( pObj->GetType() == OT_MOVER )	// 타겟이 무버일때면 처리함.
		{
			int nSkillIdx = GetCmdParam(0);
			OBJID idTarget = (OBJID)GetCmdParam(1);
			SKILLUSETYPE sutType = (SKILLUSETYPE)GetCmdParam(2);

			LPSKILL pSkill	= GetSkill( 0, nSkillIdx );		// this가 가진 스킬중 nIdx에 해당하는 스킬을 꺼낸다.
			if( pSkill == NULL )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "mover:%s skill(%d) not found." ), m_szName, nSkillIdx );
				return;	// skill not found
			}

			if( pSkill->dwSkill == SKILL_INDEX( 107, SI_MAG_MAG_BLINKPOOL ) || pSkill->dwSkill == SKILL_INDEX( 244, SI_RIG_HERO_RETURN ) )
				return;		// 아직 서버명령으로는 블링크풀 사용못함.

			if( pSkill->dwSkill == SKILL_INDEX( 412, SI_WIN_YOYO_BACKSTEP ) )
			{
				return;
			}

			CWorld *pWorld = GetWorld();
			D3DXVECTOR3 vStart = GetPos();			vStart.y += 0.5f;
			D3DXVECTOR3 vEnd   = pObj->GetPos();	vEnd.y += 0.5f;
 
			if( pWorld->IntersectObjLine( NULL, vStart, vEnd, FALSE, FALSE ) )	// 시전자와 타겟사이 장애물을 검사.
			{
				if( IsPlayer() )
					((FLWSUser *)this)->AddDefinedText( TID_GAME_BLOCKTARGETING, "" );
				break;
			}

// 			BOOL bSuccess = DoUseSkill( 0, nSkillIdx, idTarget, sutType, FALSE );		// 목표지점에 도착하면 스킬 사용시작.
// 
// 			if( bSuccess == FALSE )
// 				if( IsPlayer() )
// 					((FLWSUser *)this)->m_playTaskBar.OnEndSkillQueue( (FLWSUser *)this );

			const DWORD dwResult = DoUseSkill( 0, nSkillIdx, idTarget, sutType, FALSE );		// 목표지점에 도착하면 스킬 사용시작.

			if( IsPlayer() == TRUE && dwResult != FSC_SKILLUSE_SUCCESS )
			{
				static_cast<FLWSUser*>( this )->m_playTaskBar.OnEndSkillQueue( static_cast<FLWSUser*>( this ) );
			}

			//ClearDestObj();		// 목표에 도달하면 추적을 멈춤.
			//SendActMsg( OBJMSG_STOP );
			//g_xWSUserManager->AddMoverBehavior( this, TRUE );
		}
		break;

	default:
		SendActMsg( OBJMSG_STOP );
		OnArrive( pObj->GetId(), 0 );
		break;
	} // switch

	SetCmd( OBJACT_NONE );
	
	//if( IsPlayer() )
	//	((FLWSUser*)this)->AddQueryGetDestObj( NULL_ID );
}
#endif // not CLIENT

// 이 함수.. 오브젝트 서브 클래스로 분리 예정.. 
void CMover::ProcessMove()
{
	if( m_pActMover->IsSit() )//|| m_pActMover->GetMoveState() == OBJSTA_STAND )	
		return;

	if( IsEmptyDest() )
		return;
	if( m_pActMover->IsActAttack() )
		return;
	
	D3DXVECTOR3 vPos     = GetPos(); 
	D3DXVECTOR3 vDestPos = m_vDestPos; 

	if( !IsEmptyDestPos() )	// 좌표
	{
		bool bPositiveX = ( (vPos.x - vDestPos.x) > 0.0f );
		bool bPositiveZ = ( (vPos.z - vDestPos.z) > 0.0f );

#ifdef __CLIENT		// 마우스 목적좌표 이동시 절대축과 같은경우 중간에 멈추는 현상이 있었다.
		if( ( bPositiveX != m_bPositiveX || bPositiveZ != m_bPositiveZ ) ) 
		{
			if( IsActiveMover( ) )
			{
				D3DXVECTOR3 kDir = vPos - vDestPos ;
				D3DXVec3Normalize( &kDir, &kDir );

				D3DXVECTOR3 kMyDir;
				AngleToVectorXZ( &kMyDir, GetAngle(), 1.0f );
				D3DXVec3Normalize( &kMyDir, &kMyDir );

				float fAngle = D3DXVec3Dot( &kDir, &kMyDir );
				if( fAngle > 0.0f )
				{
					OnArriveAtPos( );
					g_DPlay.SendPlayerBehavior();	// 목표점에 도착하면 동기한다. // chipi_2012-09-14
					return;
				}
			}
			else
			{
				OnArriveAtPos( );
				return;
			}
		}
#else //__CLIENT
		if( ( bPositiveX != m_bPositiveX || bPositiveZ != m_bPositiveZ ) ) 
		{
			OnArriveAtPos();									// 좌표에 도착했을 때의 처리
			return;
		}
#endif //__CLIENT
	}
	else					// 오브젝트
	{
		CCtrl* pObj = prj.GetCtrl( m_idDest );
		if( IsValidObj( pObj ) == FALSE )
		{
			SendActMsg( OBJMSG_STAND );
			return;
		}

		vDestPos = pObj->GetPos(); 
		BOOL bRangeObj = pObj->IsRangeObj( this, m_fArrivalRange );
		if( m_pActMover->IsFly() )
		{
			
			if( bRangeObj == TRUE )
			{
				ClearDestObj();									// 그외는 목표에 도착하면 멈춤.
			#ifdef __WORLDSERVER
				OnArrive( pObj->GetId(), 0 );
			#endif	// __WORLDSERVER
			}
		}
		else 
		{			
			if( bRangeObj == TRUE )		// 3D 충돌에 실패했지만
			{
				ProcessMoveArrival( pObj );
				return;		
			}
		}
	}

	// 공중 추적 
	if( m_pActMover->IsFly() )
	{
		D3DXVECTOR3 v	= vDestPos - vPos;
		m_pActMover->m_fDistance	= D3DXVec3Length( &v );
		SendActMsg( OBJMSG_TURNMOVE, (int)GetDegree( vDestPos, vPos ), (int)GetDegreeX( vDestPos, vPos ), 0 );
	}
	else
	{
		if( m_bForward ) 
			SendActMsg( OBJMSG_TURNMOVE, (int)GetDegree( vDestPos, vPos ), (int)GetDegreeX( vDestPos, vPos ) );
		else 
			SendActMsg( OBJMSG_TURNMOVE2, (int)GetDegree( vPos, vDestPos ),  (int)GetDegreeX( vPos, vDestPos ) );
	}
} 

// 패턴이동중.
void	CMover::ProcessMovePattern( void )
{
	if( m_nMovePattern == 1 )	// 8자이동 패턴.
	{
		switch( m_nMoveEvent )
		{
		case 0:			
			m_nMoveEvent ++;
			m_nMoveEventCnt = 0;
			// break;		// break 넣지 말것.
		case 1:		// S - 1 구간 : n초간 좌회전중.
			SendActMsg( OBJMSG_FORWARD );
			if( (m_nMoveEventCnt & 3) == 0 )
				SendActMsg( OBJMSG_LTURN );
			else
				SendActMsg( OBJMSG_STOP_TURN );
			
			if( ++m_nMoveEventCnt > SEC1 * 2 )
			{
				m_nMoveEventCnt = 0;
				m_nMoveEvent ++;
//				SendActMsg( OBJMSG_STOP );
			}
			break;
		case 2:		// 1 - 2구간 : n초간 우회전하며 상승했다 하강.
			SendActMsg( OBJMSG_FORWARD );
			if( (m_nMoveEventCnt & 3) == 0 )
				SendActMsg( OBJMSG_RTURN );
			else
				SendActMsg( OBJMSG_STOP_TURN );
			if( m_nMoveEventCnt < (SEC1 * 5) / 2 )
			{
				SendActMsg( OBJMSG_LOOKUP );
			} else
			if( m_nMoveEventCnt == (SEC1 * 5) / 2 )
			{
				SendActMsg( OBJMSG_STOP_LOOK );
				SetAngleX( 0 );		// 수평으로 맞춤.
			} else
			{
				SendActMsg( OBJMSG_LOOKDOWN );
			}

			if( ++m_nMoveEventCnt > SEC1 * 5 )
			{
				m_nMoveEventCnt = 0;
				m_nMoveEvent ++;
				SendActMsg( OBJMSG_STOP_LOOK );
				SendActMsg( OBJMSG_STOP_TURN );
				SendActMsg( OBJMSG_STOP );
				SetAngleX( 0 );		// 수평으로 맞춤.
			}
			break;
		case 3:		// 2 - 3구간 : 우회전 하다가 좌회전.
			SendActMsg( OBJMSG_FORWARD );
			if( m_nMoveEventCnt < (SEC1 * 5) / 2 )		// 
			{
				if( (m_nMoveEventCnt & 3) == 0 )
					SendActMsg( OBJMSG_RTURN );
				else
					SendActMsg( OBJMSG_STOP_TURN );
			} else
			{
				if( (m_nMoveEventCnt & 3) == 0 )
					SendActMsg( OBJMSG_LTURN );
				else
					SendActMsg( OBJMSG_STOP_TURN );
			}
				
			if( ++m_nMoveEventCnt > SEC1 * 5 )
			{
				m_nMoveEventCnt = 0;
				m_nMoveEvent ++;
				SendActMsg( OBJMSG_STOP_LOOK );
				SendActMsg( OBJMSG_STOP_TURN );
				SetAngleX( 0 );		// 수평으로 맞춤.
			}
			break;
		case 4:	// 3 - S구간 : 좌회전하면서 하강하다 상승
			SendActMsg( OBJMSG_FORWARD );
			if( (m_nMoveEventCnt & 3) == 0 )
				SendActMsg( OBJMSG_LTURN );
			else
				SendActMsg( OBJMSG_STOP_TURN );
			if( m_nMoveEventCnt < (SEC1 * 5) / 2 )
			{
				SendActMsg( OBJMSG_LOOKDOWN );
			} else
			if( m_nMoveEventCnt == (SEC1 * 5) / 2 )
			{
				SendActMsg( OBJMSG_STOP_LOOK );
				SetAngleX( 0 );		// 수평으로 맞춤.
			} else
			{
				SendActMsg( OBJMSG_LOOKUP );
			}
				
			if( ++m_nMoveEventCnt > SEC1 * 5 )
			{
				m_nMoveEventCnt = 0;
				m_nMoveEvent = 99;
				SendActMsg( OBJMSG_STOP_LOOK );
				SendActMsg( OBJMSG_STOP_TURN );
				SendActMsg( OBJMSG_STOP );
				SetAngleX( 0 );		// 수평으로 맞춤.
			}
			break;
				
		case 99:
			break;
		}
	} else	// movePattern 1
	if( m_nMovePattern == 2 )		// 비행 패턴 2
	{
		switch( m_nMoveEvent )
		{
		case 0:			
			m_nMoveEvent ++;
			m_nMoveEventCnt = 0;
			// break;		// break 넣지 말것.
		case 1:		// S - 1 구간중 직진 코스
			SendActMsg( OBJMSG_FORWARD );
			
			if( ++m_nMoveEventCnt > SEC1 )		// 1초가 지나면 급 우회전.
			{
				FLOAT fAngle = GetAngle();
				SetAngle( fAngle + 135.0f );
				m_nMoveEventCnt = 0;
				m_nMoveEvent ++;
			}
			break;
		case 2:		// 1구간중 우/하로 이동
			SendActMsg( OBJMSG_FORWARD );

			if( ++m_nMoveEventCnt > SEC1 * 2 )		// 2초간 직진하다가 다시 좌로 90도 회전.
			{
				FLOAT fAngle = GetAngle();
				SetAngle( fAngle - 90.0f );
				m_nMoveEventCnt = 0;
				m_nMoveEvent ++;
				SendActMsg( OBJMSG_STOP_LOOK );
				SendActMsg( OBJMSG_STOP_TURN );
				SendActMsg( OBJMSG_STOP );
				SetAngleX( 0 );		// 수평으로 맞춤.
			}
			break;
		case 3:		// 1구간중 3번째 구간
			SendActMsg( OBJMSG_FORWARD );
				
			if( ++m_nMoveEventCnt > SEC1 * 2 )
			{
				FLOAT fAngle = GetAngle();
				SetAngle( fAngle - 45.0f );
				m_nMoveEventCnt = 0;
				m_nMoveEvent ++;
				SendActMsg( OBJMSG_STOP_LOOK );
				SendActMsg( OBJMSG_STOP_TURN );
				SetAngleX( 0 );		// 수평으로 맞춤.
			}
			break;
		case 4:	// 2-1 구간
			SendActMsg( OBJMSG_FORWARD );
				
			if( ++m_nMoveEventCnt > SEC1 * 1 )
			{
				FLOAT fAngle = GetAngle();
				SetAngle( fAngle - 45.0f );
				m_nMoveEventCnt = 0;
				m_nMoveEvent ++;
				SendActMsg( OBJMSG_STOP_LOOK );
				SendActMsg( OBJMSG_STOP_TURN );
				SendActMsg( OBJMSG_STOP );
				SetAngleX( 0 );		// 수평으로 맞춤.
			}
			break;
		case 5:	// 2-2 구간
			SendActMsg( OBJMSG_FORWARD );
			
			if( ++m_nMoveEventCnt > SEC1 * 3 )
			{
				FLOAT fAngle = GetAngle();
				SetAngle( fAngle + 45.0f );
				m_nMoveEventCnt = 0;
				m_nMoveEvent = 99;
				SendActMsg( OBJMSG_STOP_LOOK );
				SendActMsg( OBJMSG_STOP_TURN );
				SendActMsg( OBJMSG_STOP );
				SetAngleX( 0 );		// 수평으로 맞춤.
			}
			break;
			
		case 99:
			break;
		}
	} // MovePattern 2
}
