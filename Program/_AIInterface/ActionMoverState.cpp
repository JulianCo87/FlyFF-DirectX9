#include "stdafx.h"
#include "ActionMover.h"
#include "..\_Common\ParticleMng.h"

#ifdef __CLIENT
#include "Sfx.h"
#include "../Neuz/DPClient.h"
extern	CDPClient	g_DPlay;
#endif // CLIENT

#ifdef __WORLDSERVER
#include "../WorldServer/User.h"
#include "../WorldServer/FLEventArenaGlobal.h"
#include "..\_AIInterface\AIInterface.h"
#endif

#include "honor.h"		//sun: 13, ����

#include "party.h"
extern	CParty g_Party;


#include "../_Common/FLSkillSystem.h"

//
//	State process
//	������ �̰����� pMover->SetMotion�� ���� ����
//
void	CActionMover::ProcessState( CMover* pMover,  DWORD dwState, float fSpeed )
{
	int nParam = 0;
	FLOAT fTheta = D3DXToRadian( pMover->GetAngle() );
	CModelObject* pModel = (CModelObject*)pMover->m_pModel;

#ifdef KEYBOARD_SET
	if ( !pMover->IsMode( OLD_KEYBOARD_MODE ) && ( dwState == OBJSTA_BMOVE || dwState == OBJSTA_LEFT || dwState == OBJSTA_RIGHT || dwState == OBJSTA_LMOVE || dwState == OBJSTA_RMOVE ) )
		dwState = OBJSTA_FMOVE;
#endif

	switch( dwState )
	{
	// ���ڸ� ���
	case OBJSTA_STAND:
		if( GetState() == OBJSTA_STAND )		// ���� ���ִ� ���¿����� ���ִ� ������ ����
		{
			if( GetStateFlag() & OBJSTAF_ETC )
			{
#ifdef __CLIENT
				switch( pModel->m_nLoop )
				{
				case ANILOOP_1PLAY:
					if( pModel->IsEndFrame() )
						RemoveStateFlag( OBJSTAF_ETC );
					break;
				case ANILOOP_CONT:
				case ANILOOP_LOOP:
					if( pMover->IsActiveMover() && g_WndMng.m_pWndWorld->m_bLButtonDown )		// ���콺 Ŭ���ϸ� Ǯ��.
					{
						if( pModel->IsEndFrame() )
							RemoveStateFlag( OBJSTAF_ETC );
					}
					break;
				}
#else // client
				if( pModel->IsEndFrame() )
					RemoveStateFlag( OBJSTAF_ETC );
#endif // not client
			} 
			else
			{
				if( pModel->IsEndFrame() )
				{
					if( pMover->IsStateMode( STATE_BASEMOTION_MODE ) )
					{
						pMover->SetMotion( MTI_STAND );		// ��ȭ��� ���ֱ�
					}
					else
					{
						if( xRandom( 5000 ) < 2 )
							pMover->SetMotion( MTI_IDLE2, ANILOOP_1PLAY ); // idle2 �׼� 
						else
						if( xRandom( 2000 ) < 2 )
							pMover->SetMotion( MTI_IDLE1, ANILOOP_1PLAY ); // idle2 �׼� 
						else
							pMover->SetMotion( MTI_STAND );		// ��ȭ��� ���ֱ�
					}
				}
			}
		}

		if( m_bGround && m_vDelta.y <= 0 )		// ���� �������� STAND�� ������ ����, ������϶� Ground���°� �ƴ�.
		{
			m_vDelta.x = m_vDelta.z = 0;
		}
		break;

#ifdef __Y_INTERFACE_VER3		
		// ��������
	case OBJSTA_LMOVE:
		{
			if( pMover->m_dwFlag & MVRF_NOMOVE )	
				break;		// �̵����� ���¸� �� ����.
//sun: 10�� ���½ý���	Neuz, World, Trans
			if( pMover->GetAdjParam( DST_CHRSTATE ) & CHS_LOOT)	break;

			FLOAT fT = pMover->GetAngle();
			fT += 90.0f;
			if( fT > 360.0f )
				fT -= 360.0f;

			fT = D3DXToRadian(fT);

			if( IsStateFlag( OBJSTAF_WALK ) )		// �ȴ¸��
			{				
				m_vDelta.x	= sinf( fT ) * (fSpeed/4.0f);
				m_vDelta.z	= -cos( fT ) * (fSpeed/4.0f);

				if( GetState() != OBJSTA_LMOVE )	break;		// �������϶��� ���� ����
				if( IsStateFlag( OBJSTAF_COMBAT ) )	// �������
					pMover->SetMotion( MTI_WALK );		// �������� �ȱ�
				else
					pMover->SetMotion( MTI_WALK );		// �� �ȱ�
			}
			else
			{
				if( pMover->m_dwTypeFlag & OBJTYPE_FLYING )
				{
					FLOAT fAngXZ = pMover->GetAngle();
					FLOAT fAngH  = pMover->GetAngleX();
					AngleToVector( &m_vDelta, fAngXZ, -fAngH, fSpeed );
				}
				else
				{
					m_vDelta.x = sinf(fT) * fSpeed;
					m_vDelta.z = -cosf(fT) * fSpeed;
				}

				if( GetState() != OBJSTA_LMOVE )	break;		// �������϶��� ���� ����
				pMover->SetMotion( MTI_RUN );
			}
#ifdef __CLIENT		
			if( pMover->IsNPC() )
				if( pModel->IsAttrHit() )
					GetWorld()->m_pCamera->SetQuake( 15 );
#endif // CLIENT
		}
		break;

		// ��������
	case OBJSTA_RMOVE:
		{
			if( pMover->m_dwFlag & MVRF_NOMOVE )	
				break;		// �̵����� ���¸� �� ����.

//sun: 10�� ���½ý���	Neuz, World, Trans
			if( pMover->GetAdjParam( DST_CHRSTATE ) & CHS_LOOT)	break;

			FLOAT fT = pMover->GetAngle();
			fT -= 90.0f;
			if( fT < 0 )
				fT += 360.0f;

			fT = D3DXToRadian(fT);			
			
			if( IsStateFlag( OBJSTAF_WALK ) )		// �ȴ¸��
			{
				m_vDelta.x = sinf(fT) * (fSpeed/4.0f);		// �ȴ´�....
				m_vDelta.z = -cosf(fT) * (fSpeed/4.0f);
				
				if( GetState() != OBJSTA_RMOVE )	break;		// �������϶��� ���� ����
				if( IsStateFlag( OBJSTAF_COMBAT ) )	// �������
					pMover->SetMotion( MTI_WALK );		// �������� �ȱ�
				else
					pMover->SetMotion( MTI_WALK );		// �� �ȱ�
			}
			else
			{
				if( pMover->m_dwTypeFlag & OBJTYPE_FLYING )
				{
					FLOAT fAngXZ = pMover->GetAngle();
					FLOAT fAngH  = pMover->GetAngleX();
					AngleToVector( &m_vDelta, fAngXZ, -fAngH, fSpeed );
				}
				else
				{
					m_vDelta.x = sinf(fT) * fSpeed;
					m_vDelta.z = -cosf(fT) * fSpeed;
				}

				if( GetState() != OBJSTA_RMOVE )		break;		// �������϶��� ���� ����
				pMover->SetMotion( MTI_RUN );
			}
#ifdef __CLIENT	
			if( pMover->IsNPC() )
				if( pModel->IsAttrHit() )
					GetWorld()->m_pCamera->SetQuake( 15 );
#endif // CLIENT
		}
		break;
#endif //__Y_INTERFACE_VER3

	// ����
	case OBJSTA_FMOVE:
		{
			if( pMover->m_dwFlag & MVRF_NOMOVE )	
				break;		// �̵����� ���¸� �� ����.
//sun: 10�� ���½ý���	Neuz, World, Trans
			if( pMover->GetAdjParam( DST_CHRSTATE ) & CHS_LOOT)			break;

			if( IsStateFlag( OBJSTAF_WALK ) )		// �ȴ¸��
			{
				if( pMover->m_dwTypeFlag & OBJTYPE_FLYING )
				{
					FLOAT fAngXZ = pMover->GetAngle();
					FLOAT fAngH  = pMover->GetAngleX();
					AngleToVector( &m_vDelta, fAngXZ, -fAngH, fSpeed/4.0f );
				}
				else
				{
					m_vDelta.x = sinf(fTheta) * (fSpeed/4.0f);		// �ȴ´�....
					m_vDelta.z = -cosf(fTheta) * (fSpeed/4.0f);
				}
				if( GetState() != OBJSTA_FMOVE )		break;		// �������϶��� ���� ����
				if( IsStateFlag( OBJSTAF_COMBAT ) )	// �������
					pMover->SetMotion( MTI_WALK );		// �������� �ȱ�
				else
					pMover->SetMotion( MTI_WALK );		// �� �ȱ�
			}
			else
			{
				if( pMover->m_dwTypeFlag & OBJTYPE_FLYING )
				{
					FLOAT fAngXZ = pMover->GetAngle();
					FLOAT fAngH  = pMover->GetAngleX();
					AngleToVector( &m_vDelta, fAngXZ, -fAngH, fSpeed );
				}
				else
				{
					m_vDelta.x = sinf(fTheta) * fSpeed;
					m_vDelta.z = -cosf(fTheta) * fSpeed;
				}
				
#ifdef KEYBOARD_SET	
				if ( pMover->IsMode( OLD_KEYBOARD_MODE ) )
				{
					if( GetState() != OBJSTA_FMOVE )	break;
					
				}
				else
				{
					if ( ( GetState() != OBJSTA_FMOVE )
					&& (GetState() != OBJSTA_BMOVE) 
					&& (GetState() != OBJSTA_LEFT)
					&& (GetState() != OBJSTA_RIGHT)
					&& (GetState() != OBJSTA_LMOVE)
					&& (GetState() != OBJSTA_RMOVE) ) 
					break;
				}
#else
				if( GetState() != OBJSTA_FMOVE )	 break;		// �������϶��� ���� ����
#endif					
				
							
				pMover->SetMotion( MTI_RUN );
			}
#ifdef __CLIENT		
			if( pMover->IsNPC() )
				if( pModel->IsAttrHit() )
					GetWorld()->m_pCamera->SetQuake( 15 );
#endif // CLIENT
		}
		break;

	// ���ڸ� �ٱ�
#ifdef __CLIENT
	case OBJSTA_STOP_RUN:
		if( IsActDamage() )	break;
		m_vDelta.x = 0.0f;
		m_vDelta.z = 0.0f;
		if( GetState() != OBJSTA_STOP_RUN )		break;
		pMover->SetMotion( MTI_RUN );
		break;
#endif //__CLIENT
		
	// ����
	case OBJSTA_BMOVE:
		if( IsActDamage() )	break;
		m_vDelta.x =  sinf(fTheta) * (-fSpeed/5.0f);
		m_vDelta.z = -cosf(fTheta) * (-fSpeed/5.0f);
		if( GetState() != OBJSTA_BMOVE )		break;		// �������϶��� ���� ����
		if( IsStateFlag( OBJSTAF_COMBAT ) )		
			pMover->SetMotion( MTI_BACK );	// �������� �ȱ�
		else
			pMover->SetMotion( MTI_BACK );			// �� �ȱ�
		break;
	// �����̵�
	//case OBJSTA_LEFT:
	//case OBJSTA_RIGHT:
	//	break;
	// �ɾ��ִ� �����.
	case OBJSTA_SIT:
		if( pMover->m_dwMotion == MTI_SIT )		// �ɴ���
		{
			if( pModel->IsEndFrame() )		// �ɴ¸�� ����.
			{
				pMover->SetMotion( MTI_SITSTAND, ANILOOP_LOOP );
#ifdef __CLIENT
				if( g_Party.IsMember( pMover->m_idPlayer) && g_Party.m_dwModeTime[PARTY_STRETCHING_MODE] )
				{
					D3DXVECTOR3 v = pMover->GetPos();
					
					SkillProp* pSkillProp = prj.GetPartySkill( SKILL_INDEX( 7, ST_STRETCHING ) );
					
					CSfx *pSfx = CreateSfx( g_Neuz.m_pd3dDevice, pSkillProp->dwSfxObj, v, pMover->GetId() );	// �Ӹ����� sfx����.
					
					if( pSfx )
						pSfx->SetScale( D3DXVECTOR3(1.0f, 1.0f, 1.0f) );
				}
#endif	// __CLIENT
			}
		}
		else if( pMover->m_dwMotion == MTI_GETUP )		// �Ͼ����
		{
			if( pModel->IsEndFrame() )
			{
				ResetState( OBJSTA_MOVE_ALL );		// �ɴ»��� ��
			}
		}
		else if( pMover->m_dwMotion != MTI_SITSTAND )
		{
			pMover->SetMotion( MTI_SITSTAND, ANILOOP_LOOP );	// ����������쿡.
		}
		break;

	case OBJSTA_PICKUP:
		if( pModel->IsEndFrame() )
		{
			ResetState( OBJSTA_MOVE_ALL );
		}
		break;

	case OBJSTA_COLLECT:
		/*
		if( pMover->IsNPC() )
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "OBJSTA_COLLECT : �� ���Ͱ� ä������ ���Դ� %s" ), pMover->GetName() );
			break;
		}
 #ifdef __WORLDSERVER
		if( pMover->OnActCollecting() == TRUE || IsMove() )		// ä�������� ������ / ä���ϴ� �����̸�.
		{
			ResetState( OBJSTA_ACTION_ALL );			// ����
			g_xWSUserManager->AddHdr( (FLWSUser *)pMover, SNAPSHOTTYPE_STOP_COLLECT );	// ä������.
		}
 #elif defined(__CLIENT)
		pMover->OnActCollecting();
 #endif
		*/
		// ����� ������ ���������� ��� �ݺ���.
		break;

	case OBJSTA_STUN:
		if( pMover->m_dwMotion != MTI_GROGGY )		// �׷α� ����� ���� ���� ������
			if( (GetState() & (~OBJSTA_ACTION_ALL)) != OBJSTA_NOT_STAND )		// �ƹ��͵� ���ϰ� ����ϴ� ���¿��� �׷α� ������� �ٽ� �ǵ��� ����.
				pMover->SetMotion( MTI_GROGGY, ANILOOP_LOOP, MOP_FIXED );

		if( (pMover->GetAdjParam( DST_CHRSTATE ) & CHS_STUN) == 0 )		// ���ϻ��°� ��������
		{
			pMover->m_wStunCnt = 0;						// ����ī��Ʈ ����.
			if( ( pMover->GetAdjParam( DST_CHRSTATE ) & CHS_SLEEPING ) == 0 )
			{
				pMover->m_dwFlag &= (~MVRF_NOACTION);		// �׼Ǳ����� �����ϰ�
				ResetState( OBJSTA_ACTION_ALL );			// �׼ǻ��µ� �����Ѵ�.
			}
		}
		break;

	case OBJSTA_SLEEPING:
		if( pMover->m_dwMotion != MTI_GROGGY )
		{
			if( (GetState() & (~OBJSTA_ACTION_ALL)) != OBJSTA_NOT_STAND )
			{
				pMover->SetMotion( MTI_GROGGY, ANILOOP_LOOP, MOP_FIXED );
			}
		}
		if( (pMover->GetAdjParam( DST_CHRSTATE ) & CHS_SLEEPING) == 0 )
		{
			pMover->m_dwSleepingDuration = 0;
			if( ( pMover->GetAdjParam( DST_CHRSTATE ) & CHS_STUN ) == 0 )
			{
				pMover->m_dwFlag &= (~MVRF_NOACTION);
				ResetState( OBJSTA_ACTION_ALL );
			}
		}
		break;
		
	case OBJSTA_APPEAR:  // �����.
		if( pModel->IsEndFrame() )
		{
#ifdef __WORLDSERVER
			pMover->PostAIMsg( AIMSG_END_APPEAR );		// ����� �ִϸ��̼��� �������� �˸�.
#endif
			ResetState( OBJSTA_ACTION_ALL );
		}
		break;

		// ��/�� ����
	case OBJSTA_LTURN:
		if( IsActAttack() )		break;
		pMover->AddAngle( 4 );
//		{
//			if( pMover->m_dwFlag & MVRF_NOMOVE )	
//				break;		// �̵����� ���¸� �� ����.
////sun: 10�� ���½ý���	Neuz, World, Trans
//			if( pMover->GetAdjParam( DST_CHRSTATE ) & CHS_LOOT)	break;
//
//			FLOAT fT = pMover->GetAngle();
//			FLOAT fT2 = g_Neuz.m_camera.m_fRotx;
//
//			if(fT2>180.0f)
//			{
//				fT2 = 180.0f - (fT2-180.0f);
//			}
//			else
//			{
//				fT2 = 180.0f + (180.0f-fT2);
//			}
//			//FLOAT fT2 = pMover->GetAngle() - g_Neuz.m_camera.m_fRotx;
//			//fT = pMover->GetAngle() + fT2;
//			fT += 90.0f;
//			if( fT > 360.0f )
//				fT -= 360.0f;
//			pMover->SetAngle( fT2 );
//
//			fT = D3DXToRadian(fT2);
//
//			if( IsStateFlag( OBJSTAF_WALK ) )		// �ȴ¸��
//			{				
//				m_vDelta.x	= sinf( fT ) * (fSpeed/4.0f);
//				m_vDelta.z	= -cos( fT ) * (fSpeed/4.0f);
//
//				if( GetState() != OBJSTA_LMOVE )	break;		// �������϶��� ���� ����
//				if( IsStateFlag( OBJSTAF_COMBAT ) )	// �������
//					pMover->SetMotion( MTI_WALK );		// �������� �ȱ�
//				else
//					pMover->SetMotion( MTI_WALK );		// �� �ȱ�
//			}
//			else
//			{
//				if( pMover->m_dwTypeFlag & OBJTYPE_FLYING )
//				{
//					FLOAT fAngXZ = pMover->GetAngle();
//					FLOAT fAngH  = pMover->GetAngleX();
//					AngleToVector( &m_vDelta, fAngXZ, -fAngH, fSpeed );
//				}
//				else
//				{
//					m_vDelta.x = sinf(fT) * fSpeed;
//					m_vDelta.z = -cosf(fT) * fSpeed;
//				}
//
//				//if( GetState() != OBJSTA_LMOVE )	break;		// �������϶��� ���� ����
//				pMover->SetMotion( MTI_RUN );
//			}
//#ifdef __CLIENT		
//			if( pMover->IsNPC() )
//				if( pModel->IsAttrHit() )
//					GetWorld()->m_pCamera->SetQuake( 15 );
//#endif // CLIENT
//		}
		

		
		
		break;
	case OBJSTA_RTURN:
		if( IsActAttack() )		break;
		pMover->AddAngle( -4 );
		break;
	// ��ġ���ø���
	case OBJSTA_LOOKUP:
		if( IsActAttack() )		break;
		if( pMover->GetAngleX() > -45.0f )
			pMover->AddAngleX( -1 );
		break;

	case OBJSTA_LOOKDOWN:
		if( IsActAttack() )		break;
		if( pMover->GetAngleX() < 45.0f )
			pMover->AddAngleX( 1 );
		break;
		
	// ����-�߱���
	case OBJSTA_SJUMP1:
		if( pMover->m_dwFlag & MVRF_NOMOVE )	
			break;		// �̵����� ���¸� �� ����.
		SetJumpState( OBJSTA_SJUMP2 );
		m_vDelta.y = 0.2f + (pMover->GetParam( DST_JUMPING, 0 ) / 1000.0f);
		if( (GetState() & OBJSTA_COLLECT)== 0 )
		{
			pMover->SetMotion( MTI_JUMP2, ANILOOP_CONT );	// ��µ��� ��ȯ
		}
		pModel->SetMotionBlending( FALSE );
		break;

	// ����-���
	case OBJSTA_SJUMP2:
		if( pMover->m_dwFlag & MVRF_NOMOVE )	
			break;		// �̵����� ���¸� �� ����.
		if( (GetState() & OBJSTA_COLLECT)== 0 )
		{
			pMover->SetMotion( MTI_JUMP2, ANILOOP_CONT );	// ��µ��� ��ȯ
		}
		if( m_vDelta.y <= 0 )		// �Ʒ��� �������� ���������� �ϰ����� ��ȯ
		{
			SetJumpState( OBJSTA_SJUMP3 );
			if( (GetState() & OBJSTA_COLLECT)== 0 ) pMover->SetMotion( MTI_JUMP3, ANILOOP_CONT );
			pModel->SetMotionBlending( FALSE );
		}
		break;

	// ����-�ϰ�
	case OBJSTA_SJUMP3:
		if( pMover->m_dwFlag & MVRF_NOMOVE )	
			break;		// �̵����� ���¸� �� ����.
		if( (GetState() & OBJSTA_DMG_ALL) == 0 || (GetState() & OBJSTA_COLLECT)== 0 )			// ���������� �ƴҶ��� �ϰ����� ��ȯ.
		{
			if( TRUE == pMover->SetMotion( MTI_JUMP3, ANILOOP_CONT ) )	// �ϰ����� ��ȯ
				pModel->SetMotionBlending( FALSE );
		}
		if( m_bGround )		// �ٴڿ� ������� �������·� �Ѿ
		{
			SetJumpState( OBJSTA_SJUMP4 );	// ���� - SetAction( OBJMSG_JUMP4 ); �� ���������� �ٲ���.
			pMover->OnActLanding();			// ���� �ڵ鷯 ��.
			if( GetState() & OBJSTA_DMG_ALL)	break;		// ���������¶�� ���� ���� ���� ����.
			if( TRUE == pMover->SetMotion( MTI_JUMP4, ANILOOP_1PLAY ) )	// �������� ��ȯ
			{
				pModel->SetMotionBlending( FALSE );		// ���� ������ ������ ���� �ʴ°� �ڿ�������.
			}
		}
		break;

	// ����-����
	case OBJSTA_SJUMP4:
		//if( pMover->m_dwFlag & MVRF_NOMOVE )	break;		// �̵����� ���¸� �� ����.
		ResetState( OBJSTA_JUMP_ALL );
		if( pModel->IsEndFrame() || ( GetState() & OBJSTA_DMG_ALL ) )	// �������� �� �����ų�, ������ ���̾����� ���� ���� ������Ű�� ����.
		{
			ResetState( OBJSTA_JUMP_ALL );
#ifdef __CLIENT
			// OnActLanding() ���κ��� ���� // chipi_2012/09/12
			//if( pMover->IsActiveMover() )
			//	g_DPlay.SendPlayerBehavior();
#endif // __CLIENT
			break;
		}
//sun: 13, ����
#ifdef __WORLDSERVER
#ifndef __MAINSERVER
		if(!pMover->IsPlayer())
			FLERROR_LOG( PROGRAM_NAME, _T( "processstate1()AddHonorListAck()" ) );
#endif // __MAINSERVER
		if(pMover->IsPlayer() )
            ((FLWSUser*)pMover)->SetHonorAdd(HS_JUMP,HI_COUNT_CHECK);
#endif	// __WORLDSERVER
		break;

	//--------- ���� ---------------------------
	case OBJSTA_ATK1:
	case OBJSTA_ATK2:
	case OBJSTA_ATK3:
	case OBJSTA_ATK4:
	case OBJSTA_ATK5:
		ProcessStateAttackMelee( dwState, nParam );
		break;
		
	case OBJSTA_SP_ATK1:
	case OBJSTA_SP_ATK2:
		ProcessStateAttackSP( dwState, nParam );
		break;

	//--------- ���� ---------------------------
	case OBJSTA_ATK_MAGIC1:
		ProcessStateAttackMagic( dwState, nParam );
		break;

	//--------- ���Ÿ� �������� ------------------
	case OBJSTA_ATK_RANGE1:
		ProcessStateAttackRange( dwState, nParam );
		break;
		
	case OBJSTA_ATK_MELEESKILL:
		ProcessStateMeleeSkill( dwState, nParam );
		break;

	case OBJSTA_ATK_CASTING1:
	case OBJSTA_ATK_CASTING2:
		ProcessStateMagicCasting( dwState, nParam );
		break;

	case OBJSTA_ATK_MAGICSKILL:
		ProcessStateMagicSkill( dwState, nParam );
		break;
		
	//----------- ������ -------------------------
	case OBJSTA_DMG:
		if( pModel->IsEndFrame() )	
		{
			ResetState( OBJSTA_DMG_ALL );
		}
		break;

	case OBJSTA_DMG_FLY:
		if( m_bGround )
		{
			if( (GetState() & OBJSTA_STUN) )
			{
				ResetState( OBJSTA_DMG_FLY );			// �׼ǻ��µ� �����Ѵ�.
				break;
			}
			
			if( m_vDelta.y == 0 )	// ó�� �㶧�� m_bGround�� 1�̱ⶫ�� �ϰ��ϰ� ������ �ٴڿ� �����͸� ��ȿ!
			{
				m_vDelta.x = m_vDelta.z = 0;	// �������� �� �ӵ� ������.
				m_vDeltaE.x = m_vDelta.x;	// m_vDeltaE�� �ڵ������� ������ ���ȴ�.
				m_vDeltaE.z = m_vDelta.z;
				m_vDelta.y = 0.04f;
				SetState( OBJSTA_DMG_ALL, OBJSTA_DMG_DIE );	
				pMover->SetMotion( MTI_DMGDIE, ANILOOP_CONT );		// �н�! �������� �������ִ� ���
				break;
			}
		}
		break;

	// ���� �н� �������� ���� �̲������� ��
	case OBJSTA_DMG_DIE:
		m_vDelta.x = 0;
		m_vDelta.z = 0;
		
		if( pModel->IsEndFrame() )		// �������� ������ �׾� �κη��� ����. ����� ���� ��� �״�� �̾���.
		{
			if( pMover->m_nHitPoint > 0 )	// �ٴڿ� �������� ���� HP�� ���� ������ 
			{
				SetState( OBJSTA_DMG_ALL, OBJSTA_DMG_LIVE );	// �ٽ� �Ͼ��
				pMover->SetMotion( MTI_DMGLIVE, ANILOOP_1PLAY );	// �Ͼ�� ���
			} 
			else
			{
				SetState( OBJSTA_DMG_ALL, OBJSTA_DEAD );		// HP�� ������ �ٷ� ����.
			}
		}
		break;

	// �������� �Ͼ�� ���� - ������ ������ ó��.
	case OBJSTA_DMG_LIVE:
		if( pModel->IsEndFrame() )
		{
			pMover->m_dwFlag &= (~MVRF_NOACTION);		// �׼Ǳ����� �����ϰ�
			ResetState( OBJSTA_DMG_ALL );	// ������ ������ Ŭ����.
			ResetState( OBJSTA_ATK_ALL );
		}
		break;

	case OBJSTA_DEAD:	// �׾� �κη��� ����
		if( pModel->m_nLoop != ANILOOP_CONT )	// ��� �׾�κη����� ���󶫿� ����ó�� �ߴ� ����������
			pModel->m_nLoop = ANILOOP_CONT;

		pMover->m_dwFlag &= (~MVRF_NOACTION);		// �׼Ǳ����� �����ϰ�
 #ifdef __WORLDSERVER
		// Ŭ�󿡼� ������Ʈ ����°� ���� ���Ƿ� �̺κ��� ������ �ʿ� ����.
		if( !pMover->IsPlayer() ) 
		{
			m_nDeadCnt--;
			/*
			if( pMover->IsResourceMonster() )		// �ڿ��� �ִ� ���ͳ�
			{				
				if( pMover->m_nResource == 0 )		// �ڿ��� �� ��������
				{
					if( m_nDeadCnt > 10800 - 200 )	// ��Ÿ���� �� �ȵȰǵ� ���� �ڿ��� �������ٸ� ���� �߸��ȰŴ�.
						FLERROR_LOG( PROGRAM_NAME, _T( "OBJSTA_DEAD:%d %s" ), m_nDeadCnt, pMover->GetName() );
					m_nDeadCnt = 0;			// �ڿ��� �� ���������� ���� ����������~
				}
			}
			*/
			if( m_nDeadCnt <= 0 )
			{
				SetState( OBJSTA_DMG_ALL, OBJSTA_DISAPPEAR );	// ������� ���·� ��ȯ
				m_nCount = 0;
			}
		} 
 #endif // WORLDSERVER

#ifdef __CLIENT
		if( pMover->IsNPC() )
		{
			if( pModel->IsAttrQuake() )	// ������ ��.
			{
				if( pMover->GetProp()->dwClass == RANK_MATERIAL )
					GetWorld()->m_pCamera->SetQuake( 60, 1.0f );
				else
					GetWorld()->m_pCamera->SetQuake( 15 );
			}
		}
#endif	// __CLIENT
		break;

	case OBJSTA_DISAPPEAR:	// ������� ����
#ifndef __CLIENT
		if( m_nCount > FRAME_PER_SEC )	// 1���� �����
//		if( m_nCount++ > SEC1 )
		{
			pMover->Delete();
		}
#endif	//__CLIENT
		break;

	case OBJSTA_RESURRECTION:		// ��Ȱ �ϴ� ��.
		if( pModel->IsEndFrame() )
		{
			ResetState( OBJSTA_DMG_ALL );

#ifdef	__WORLDSERVER
			if( g_pEventArenaGlobal->IsArenaChannel() )
			{
				g_pEventArena->OnPlayerResurrection( pMover );
			}
#endif
		}
		break;
	}
}

//
//	������� ���� ������ų��....
//

void	CActionMover::ProcessStateMeleeSkill( DWORD /*dwState*/, int /*nParam*/ )
{
	CMover* pMover = m_pMover;
	CModelObject *pModel = (CModelObject *)pMover->m_pModel;

	int nSkill = pMover->GetActParam(0);

	if( nSkill == SKILL_INDEX( 204, SI_ACR_YOYO_COUNTER ) )	// ���̷� ��Ʈ����ũ�� ���ܴ�.
	{
	}
	else if( nSkill == SKILL_INDEX( 138, SI_BLD_DOUBLESW_SILENTSTRIKE ) )	// ���̷� ��Ʈ����ũ�� ���ܴ�.
	{
		if( pModel->IsEndFrame() )
		{
			ResetState( OBJSTA_ATK_ALL );
			return;
		}
	} 
	else
	{
		if( pModel->m_fFrameCurrent >= (pModel->m_nFrameMax * 0.65f) )	// ����ĵ�� �Ҷ� �̹������ �ٸ�������� ����!!!!!!!!!!!!!!!!!!!!!
		{
			pModel->m_bEndFrame = TRUE;		// �ִϸ� �߰��� �ߴܽ������Ƿ� �ݵ�� �̰� TRUE�� ����� �Ѵ�.
			ResetState( OBJSTA_ATK_ALL );
			return;
		}
	}

	if( (g_nProcessCnt & 3) == 0 )
	{
		if( m_nShootCnt > 0 )			// ���ӹ߻簡 ���õǸ�
		{
			g_cSkillSystem->OnMeleeSkill( pMover, 0 );	// �ٽ� �߻��Ű��
			m_nShootCnt--;				// ���� �ϳ��� ��.
		}
	}
	

	if( pModel->m_nPause > 0 )
	{
		pModel->m_nPause --;
	} 
	else
	{
		if( pModel->IsAttrHit() )	// ���� �����ӿ� Ÿ�ݼӼ��� �ִ°�?
		{
			if( nSkill == SKILL_INDEX( 138, SI_BLD_DOUBLESW_SILENTSTRIKE ) )	// ���̷� ��Ʈ����ũ�� ���ܴ�.
			{
				D3DXVECTOR3 vForce;
				AngleToVectorXZ( &vForce, pMover->GetAngle(), 0.75f );		// �������� �����ϴ� �� ����.
				SetExternalForce( vForce );
			} 
			else if( nSkill == SKILL_INDEX( 152, SI_BIL_KNU_BELIALSMESHING ) )	// ������ ���Ž̵� ������ ����.
			{
				D3DXVECTOR3 vForce;
				AngleToVectorXZ( &vForce, pMover->GetAngle(), 0.35f );		// �������� �����ϴ� �� ����.
				SetExternalForce( vForce );
			}

			g_cSkillSystem->OnMeleeSkill( pMover, 0, m_nMotionHitCount );	// ������ �ڵ鷯�� ȣ��.
			m_nMotionHitCount++;

#ifdef __CLIENT			
			pModel->m_nPause = 4;	// frame ���� - Ŭ���ϰ�츸 ������.
#endif

			if( pMover->GetAct() != OBJACT_USESKILL )	
				return;	// ��ų����� Ÿ���� �Ǳ����� �ٸ� ������� �ٲ���. �̷���Ȳ�� �ͼ� �ȵȴ�.

			int		nSkill = pMover->GetActParam(0);
			OBJID	idTarget = pMover->GetActParam(1);
			UNREFERENCED_PARAMETER( idTarget );

			DWORD	dwLevel = pMover->GetActParam(3);

			SkillProp* pSkillProp = NULL;
			AddSkillProp* pAddSkillProp = NULL;
			BOOL bRet = pMover->GetSkillProp( &pSkillProp, &pAddSkillProp, nSkill, dwLevel, "ProcessStateMeleeSkill" );
			if( bRet == FALSE )
				return;
			
			if( pAddSkillProp->nSkillCount > 0 )		// ������ų�� ��� �߻�ü������ ������.
			{
				m_nShootCnt = pAddSkillProp->nSkillCount;		// ĳ���ù߻絿�� ���� xx�����ӿ� �ѹ��� ���ʷ� �߻�.
				m_nShootCnt --;		// ���⼭ �̹� �ϳ� �߻������� �ϳ� ���ִ°� �´�.
			}
		}
	}
}

//
//	��������� �������ݵ��� ��....
//

// ĳ���� ������.
void	CActionMover::ProcessStateMagicCasting( DWORD dwState, int /*nParam*/ )
{
	CMover* pMover = m_pMover;
	CModelObject *pModel = (CModelObject *)pMover->m_pModel;
	
	if( dwState == OBJSTA_ATK_CASTING1 )	// ���� ���ø��� ��
	{
		if( pModel->IsEndFrame() )	// �ִϸ��̼��� �����ٸ� ����.
		{
			pMover->SetMotion( pMover->m_dwMotion + 1, ANILOOP_LOOP, MOP_SWDFORCE | MOP_NO_TRANS | MOP_FIXED );		// ���� ��������
//			m_nCount = 0;
			SetState( OBJSTA_ATK_ALL, OBJSTA_ATK_CASTING2 );
		}
	} 
	else if( dwState == OBJSTA_ATK_CASTING2 )	// �����ð� ��������
	{
//		if( ++m_nCount >= m_nCastingTime )
		if( m_nCount >= ( m_nCastingTime ) )
		{
			int dwMotion = pMover->m_dwMotion + 1;
		#ifdef __CLIENT
			if( pMover->HasBuffByIk3( IK3_TEXT_DISGUISE ) )
				dwMotion = MTI_ATK1;
		#endif
			pMover->SetMotion( dwMotion, ANILOOP_1PLAY, MOP_SWDFORCE | MOP_NO_TRANS | MOP_FIXED );		// ���� ��������
			SetState( OBJSTA_ATK_ALL, OBJSTA_ATK_MAGICSKILL );
		}
	}

#if defined(__CLIENT)
	pModel->m_SparkInfo.m_bUsed  = TRUE;
	pModel->m_SparkInfo.m_v3Color.x = 0.0f;
	pModel->m_SparkInfo.m_v3Color.y = 2.0f;
	pModel->m_SparkInfo.m_v3Color.z = 2.0f;
	pModel->m_SparkInfo.m_nCnt = 0;
	pModel->m_SparkInfo.m_fLerp= 1.0f;
	
	AngleToVectorXZ( &pModel->m_SparkInfo.m_v3SparkDir, pMover->GetAngle(), -1.0f );
#endif //defined(__CLIENT)
}


// �Ź���. - ���� �߻絿����.
void	CActionMover::ProcessStateMagicSkill( DWORD /*dwState*/, int /*nParam*/ )
{
	CMover* pMover = m_pMover;
	CModelObject* pModel = (CModelObject *)pMover->m_pModel;

	if( pModel->IsEndFrame() )	// �ִϸ��̼��� �����ٸ� ���� ����
	{
		ResetState( OBJSTA_ATK_ALL );
		return;
	}

	if( (g_nProcessCnt & 3) == 0 )
	{
		if( m_nShootCnt > 0 )		// ���ӹ߻簡 ���õǸ�
		{
			g_cSkillSystem->OnMagicSkill( pMover, 0 );	// �ٽ� �߻��Ű��
			m_nShootCnt--;		// ���� �ϳ��� ��.
		}
	}

	if( pModel->IsAttrHit() )	// ���� �����ӿ� Ÿ���� �ִ°�?
	{
		g_cSkillSystem->OnMagicSkill( pMover, 0, m_nMotionHitCount );	// ������ �ڵ鷯�� ȣ��.
		m_nMotionHitCount++;
		
		if( pMover->GetAct() != OBJACT_USESKILL )	
			return;	// ��ų����� Ÿ���� �Ǳ����� �ٸ� ������� �ٲ���. �̷���Ȳ�� �ͼ� �ȵȴ�.

		int		nSkill = pMover->GetActParam(0);
		OBJID	idTarget = pMover->GetActParam(1);
		UNREFERENCED_PARAMETER( idTarget );

		DWORD	dwLevel = pMover->GetActParam(3);
		
		SkillProp* pSkillProp = NULL;
		AddSkillProp* pAddSkillProp = NULL;
		BOOL bRet = pMover->GetSkillProp( &pSkillProp, &pAddSkillProp, nSkill, dwLevel, "ProcessStateMagicSkill" );
		if( bRet == FALSE )
			return;
		
		if( pAddSkillProp->nSkillCount > 0 )		// ������ų�� ��� �߻�ü������ ������.
		{
			m_nShootCnt = pAddSkillProp->nSkillCount;		// ĳ���ù߻絿�� ���� xx�����ӿ� �ѹ��� ���ʷ� �߻�.
			m_nShootCnt --;		// ���⼭ �̹� �ϳ� �߻������� �ϳ� ���ִ°� �´�.
		}
		
	}
}

//
//		���� �׼� ó��
//
void	CActionMover::ProcessStateAttackMelee( DWORD dwState, int /*nParam*/ )
{
	CMover* pMover = m_pMover;
	CModelObject* pModel = (CModelObject *)pMover->m_pModel;

	switch( dwState )
	{
	case OBJSTA_ATK1:
	case OBJSTA_ATK2:
	case OBJSTA_ATK3:
	case OBJSTA_ATK4:
	case OBJSTA_ATK5:
		// ���ݵ����� ������ ���ݻ��� ����
		if( pMover->IsPlayer() )
		{
			int		nReqLevel = 0;		// ���� ��Ÿ�� �Ѿ�� ���� �ʿ䷹��.
			OBJMSG	dwNextMsg = OBJMSG_ATK2;

			switch( dwState )
			{
			case OBJSTA_ATK1:	nReqLevel = 2;		dwNextMsg = OBJMSG_ATK2;	break;
			case OBJSTA_ATK2:	nReqLevel = 8;		dwNextMsg = OBJMSG_ATK3;	break;
			case OBJSTA_ATK3:	nReqLevel = 12;		dwNextMsg = OBJMSG_ATK4;	break;
			case OBJSTA_ATK4:	nReqLevel = 15;		dwNextMsg = OBJMSG_ATK5;	break;
			case OBJSTA_ATK5:	nReqLevel = 999;	dwNextMsg = OBJMSG_ATK1;	break;
			}
			if( pModel->IsEndFrame() )		// �ִϸ��̼��� ������. ��Ÿ�� �Ѿ���� ������.
			{
#ifdef __WORLDSERVER					
				m_objidHit = NULL_ID;
#endif
				ResetState( OBJSTA_ATK_ALL );
#ifdef __CLIENT
				if( GetMover()->IsActiveMover() )
				{
					if( (pMover->GetLevel() >= nReqLevel) && ((m_dwCtrlMsg & CTRLMSG_LDOWN) || ((CWndWorld *)g_WndMng.m_pWndWorld)->m_bAutoAttack) )	// �������� X�̻��̸� ������Ÿ����
					{
						PT_ITEM_SPEC pItemProp = pMover->GetActiveHandItemProp();
						if( pItemProp && pItemProp->dwItemKind3 == IK3_YOYO )
						{
							if( dwState == OBJSTA_ATK2 || dwState == OBJSTA_ATK3 || dwState == OBJSTA_ATK4 || dwState == OBJSTA_ATK5 )
								break;
						}

						CMover* pHitObj	= prj.GetMover( m_objidHit );
						if( IsValidObj( (CObj*)pHitObj ) &&		// ���� ��ų�� ����Ǿ������� ���⼭ ������.
							g_WndMng.m_pWndWorld->GetNextSkill() == NEXTSKILL_NONE
							&& pMover->IsRangeObj( pHitObj, pMover->GetAttackRange( pItemProp != NULL ? pItemProp->dwAttackRange : AR_SHORT ) ) == TRUE )	// ���� ���� �Ÿ� ���� �ִ��� �˻�.
						{
							
							pMover->DoAttackMelee( pHitObj, dwNextMsg, 0 );
							//int nError	= SendActMsg( dwNextMsg, m_objidHit, MAKELONG( 0, 0 ), MAKELONG( 0, 0 ) );
							//if( nError > 0 )	
							//{
// 								PT_ITEM_SPEC pHandItemProp		= pMover->GetActiveHandItemProp();
// 								FLOAT fVal	= pHandItemProp? pHandItemProp->fAttackSpeed: 0.0F;
							//	g_DPlay.SendMeleeAttack ( dwNextMsg, m_objidHit, MAKELONG( 0, 0 ), MAKELONG( 0, (WORD)nError )/*, fVal*/ );
							//}
							
						}
						else
							m_objidHit = NULL_ID;
					}
				}
#endif	// CLIENT
			}
		} 
		else	
		// monster
		{
#ifdef __CLIENT
			if( pMover->m_dwIndex == OBJECT_INDEX( 164, MI_CLOCKWORK1 ) )
			{
				if( dwState == OBJSTA_ATK3 )
				{
					if( (pMover->m_dwFlag & MVRF_LASERCHARGE) == 0 )	// ���������� ����Ʈ�� ���� ���� �ȵǾ���.
					{
						// �̸����� �� ������ ����Ʈ.
						CSfx *pSfx = CreateSfx( D3DDEVICE, XI_INDEX( 1611, XI_NPCSP1DIRAMP ), pMover->GetPos(), pMover->GetId(), D3DXVECTOR3(0,0,0), NULL_ID, -1 );			
						if( pSfx )
						{
							pSfx->SetScale( D3DXVECTOR3( 2.0f, 2.0f, 2.0f ) );
							pMover->m_dwFlag |= MVRF_LASERCHARGE;
						}
					}
				}
			}
			else if( pMover->m_dwIndex == OBJECT_INDEX( 165, MI_BIGMUSCLE ) )
			{
			}
			else if( pMover->m_dwIndex == OBJECT_INDEX( 166, MI_KRRR ) || pMover->m_dwIndex == OBJECT_INDEX( 1554, MI_KRRR2 ) )
			{
			}
			else if( pMover->m_dwIndex == OBJECT_INDEX( 167, MI_MUSHMOOT ) || pMover->m_dwIndex == OBJECT_INDEX( 1552, MI_MUSHMOOT2 ) )
			{
				if( dwState == OBJSTA_ATK3 )
				{
					if( (pMover->m_dwFlag & MVRF_LASERCHARGE) == 0 )	// ���������� ����Ʈ�� ���� ���� �ȵǾ���.
					{
						// �̸����� �� ������ ����Ʈ.
						CSfxMushmootCharge *pSfx = (CSfxMushmootCharge*)CreateSfx( D3DDEVICE, XI_INDEX( 1747, XI_SKILL_MUSHMOOT_CHARGE ), pMover->GetPos(), pMover->GetId(), D3DXVECTOR3(0,0,0), NULL_ID, -1 );
						if( pSfx )
						{
							pSfx->m_nEventPos = 0;
							pSfx->SetScale( D3DXVECTOR3( 2.0f, 2.0f, 2.0f ) );
							pMover->m_dwFlag |= MVRF_LASERCHARGE;
						}
					}
				}
				else if( dwState == OBJSTA_ATK4 )
				{
					if( (pMover->m_dwFlag & MVRF_LASERCHARGE) == 0 )	// ���������� ����Ʈ�� ���� ���� �ȵǾ���.
					{
						CSfxMushmootCharge *pSfx = NULL;
						// �̸����� �� ������ ����Ʈ.
						pSfx = (CSfxMushmootCharge*)CreateSfx( D3DDEVICE, XI_INDEX( 1747, XI_SKILL_MUSHMOOT_CHARGE ), pMover->GetPos(), pMover->GetId(), D3DXVECTOR3(0,0,0), NULL_ID, -1 );			
						if( pSfx )
						{
							pSfx->m_nEventPos = 1;
							pSfx->SetScale( D3DXVECTOR3( 2.0f, 2.0f, 2.0f ) );
							pMover->m_dwFlag |= MVRF_LASERCHARGE;
						}
						pSfx = (CSfxMushmootCharge*)CreateSfx( D3DDEVICE, XI_INDEX( 1747, XI_SKILL_MUSHMOOT_CHARGE ), pMover->GetPos(), pMover->GetId(), D3DXVECTOR3(0,0,0), NULL_ID, -1 );			
						if( pSfx )
						{
							pSfx->m_nEventPos = 2;
							pSfx->SetScale( D3DXVECTOR3( 2.0f, 2.0f, 2.0f ) );
							pMover->m_dwFlag |= MVRF_LASERCHARGE;
						}
						pSfx = (CSfxMushmootCharge*)CreateSfx( D3DDEVICE, XI_INDEX( 1747, XI_SKILL_MUSHMOOT_CHARGE ), pMover->GetPos(), pMover->GetId(), D3DXVECTOR3(0,0,0), NULL_ID, -1 );			
						if( pSfx )
						{
							pSfx->m_nEventPos = 3;
							pSfx->SetScale( D3DXVECTOR3( 2.0f, 2.0f, 2.0f ) );
							pMover->m_dwFlag |= MVRF_LASERCHARGE;
						}
						pSfx = (CSfxMushmootCharge*)CreateSfx( D3DDEVICE, XI_INDEX( 1747, XI_SKILL_MUSHMOOT_CHARGE ), pMover->GetPos(), pMover->GetId(), D3DXVECTOR3(0,0,0), NULL_ID, -1 );			
						if( pSfx )
						{
							pSfx->m_nEventPos = 4;
							pSfx->SetScale( D3DXVECTOR3( 2.0f, 2.0f, 2.0f ) );
							pMover->m_dwFlag |= MVRF_LASERCHARGE;
						}
					}
				}
			}	
#endif // Client
			if( pMover->m_dwFlag & MVRF_HITCONT )
				pMover->OnAttackMeleeContinue( dwState );

			if( pModel->IsEndFrame() )
			{
				pMover->OnEndMeleeAttack( dwState );
				pMover->m_dwFlag &= (~MVRF_HITCONT);		// �ִϳ������� �÷� ����.
				pMover->m_dwFlag &= (~MVRF_LASERCHARGE);	// �ִϳ������� �÷� ����.
				m_objidHit	= NULL_ID;
				ResetState( OBJSTA_ATK_ALL );
#ifdef __CLIENT
				extern CPartsLaser g_Laser;
				g_Laser.m_bActive = FALSE;		// �ִ� ������ ������ ����(����)
#endif
				pMover->m_pActMover->m_nMotionHitCount = 0;
			}
		}
		
		if( pModel->m_nPause > 0 )
		{
#ifdef __CLIENT
			CMover* pHitObj	= prj.GetMover( m_idTarget );

			D3DXVECTOR3 vDist = pMover->GetPos() - CMover::GetActiveMover()->GetPos();
			float fDistSq = D3DXVec3LengthSq( &vDist );
			if( pHitObj && pMover->IsPlayer() && fDistSq < 32.0f * 32.0f )		// �ָ��ִ³��� ��ƼŬ ��������.
			{
				int i;
				D3DXVECTOR3	vPos = pHitObj->GetPos();

				D3DXVECTOR3 vLocal;
				AngleToVectorXZ( &vLocal, pMover->GetAngle(), 1.0f );		// ��� ���������� 1���������� ��ƼŬ ����.
				vLocal += pMover->GetPos();
				vPos = vLocal;

				vPos.y += 0.7f;
				D3DXVECTOR3 vVel;
				float	fAngXZ, fAngH;
				float	fSpeed;
				float	fDist;
				float	fMoverAngle = pMover->GetAngle();
				CParticles *pParticles = NULL;

				int nOption = 0;
				int nEffLevel = 0;
				if( pMover->IsActiveMover() )
				{
					FLItemElem *pItemElem = pMover->GetWeaponItem();
					if( pItemElem )
						nOption = pItemElem->GetAbilityOption();
				} else
				{	// Ÿ �÷��̾�� ����ũ�������̹Ƿ� ���⼭���� ���� ���´�.
					nOption = pMover->m_aEquipInfo[PARTS_RWEAPON].nOption & 0xFF;
				}

				if( nOption == 10 )
					nEffLevel = 5;
				else if( nOption == 9 )
					nEffLevel = 4;
				else if( nOption == 8 )
					nEffLevel = 3;
				else if( nOption == 7 )
					nEffLevel = 2;
				else if( nOption >= 4 )	// 4 ~ 6
					nEffLevel = 1;
				else 
					nEffLevel = 0;
					
				for( i = 0; i < 1; i ++ )
				{
					fAngXZ = xRandomF(360);
					
					fAngXZ = D3DXToRadian( fAngXZ );
					fAngH = 15.0f + xRandomF(45);
					fAngH = D3DXToRadian( fAngH );
					fSpeed = (0.1f + xRandomF(0.03f));// / 1.0f;
					
					fDist = cosf(-fAngH) * fSpeed;
					vVel.x = sinf(fAngXZ) * fDist;
					vVel.z = -cosf(fAngXZ) * fDist;
					vVel.y = -sinf(-fAngH) * fSpeed;
					if( g_Option.m_bSFXRenderOff != TRUE )
					{
						if( nEffLevel == 5 )
						{
							pParticles = g_ParticleMng.CreateParticle( nEffLevel + xRandom(6), vPos, vVel, CMover::GetActiveMover()->GetPos().y );
						} else
							pParticles = g_ParticleMng.CreateParticle( nEffLevel, vPos, vVel, CMover::GetActiveMover()->GetPos().y );
					}
					if( pParticles )
						pParticles->m_bGravity = TRUE;
				}
			}
#endif // CLIENT
		
		// ����ī��Ʈ�� �� ������ �ڷ� �и��� ����
		// 2006.09.16 raiders ���Ƶ�. ����: CActionMover�� m_fAccAng�� ������� �ʴ� ����
			--pModel->m_nPause; 
//			if( --pModel->m_nPause == 0 )		
//			{
//				if( pHitObj )
//					pHitObj->m_pActMover->m_fAccAng = pMover->GetAngle();				// �������� �����ִ� �������� �и�
//			}
		// Pause > 0 
		}
		else
		{
#if defined(__CLIENT)
			PT_ITEM_SPEC pItemProp = pMover->GetActiveHandItemProp();

			// ���� �Ϲ� �и� ��Ÿ�ʹ� �ٸ���
			// ��鸲ȿ��(Quake)�� �������� �߻�ü�� �߻��ϰ�
			// ���� ȿ��ó��(������, ����Ʈ)�� Ÿ�ݼӼ����� ó���Ѵ�.
			if( pMover->IsPlayer() && pItemProp && pItemProp->dwItemKind3 == IK3_YOYO && pModel->IsAttrQuake() )
			{
				CMover* pHitObj	= prj.GetMover( m_idTarget );
				if( IsInvalidObj(pHitObj) )
					break;

				DWORD dwSfxObj = pItemProp->dwSfxObj2;
				
				if( dwSfxObj == NULL_ID )	
					break;
				
				D3DXVECTOR3 vPos;
				D3DXVECTOR3 vLocal;
				D3DXVECTOR3 vPosSrc   = pMover->GetPos() + D3DXVECTOR3( 0.0f, 1.0f, 0.0f ); // �߻� ������ ���Ƿ� �÷��ش�. ���� 
				D3DXVECTOR3 vPosDest  = pHitObj->GetPos() + D3DXVECTOR3( 0.0f, 1.0f, 0.0f ); // ��ǥ ������ ���Ƿ� �÷��ش�. ���� 
				
				CSfx* pSfx = NULL;
				
				if( pMover->m_dwMotion == MTI_ATK1 )
				{
					pModel->GetHandPos( &vPos, PARTS_RWEAPON, pMover->GetMatrixWorld() );
					pSfx = CreateSfxYoYo( D3DDEVICE, dwSfxObj, vPos, pMover->GetId(), vPosDest );	
					((CSfxItemYoyoAtk*)pSfx)->MakePath(PARTS_RWEAPON);					
				}
				else
				if( pMover->m_dwMotion == MTI_ATK2 )
				{
					pModel->GetHandPos( &vPos, PARTS_LWEAPON, pMover->GetMatrixWorld() );
					pSfx = CreateSfxYoYo( D3DDEVICE, dwSfxObj, vPos, pMover->GetId(), vPosDest );	
					((CSfxItemYoyoAtk*)pSfx)->MakePath(PARTS_LWEAPON);
				}
			}
#endif //__CLIENT

			if( pModel->IsAttrHit() )	// ���� �����ӿ� Ÿ�ݼӼ��� �ִ°�?
			{
				pMover->m_dwFlag |= MVRF_HITCONT;
				
				CMover* pHitObj	= prj.GetMover( m_idTarget );
				if( IsValidObj( (CObj*)pHitObj ) && pHitObj->IsLive() )
				{
					// �ڵ鷯 ȣ��.
					pMover->OnAttackMelee( dwState, pHitObj );
				}
			} // is hitattr ?

#if defined(__CLIENT)
			if( pModel->IsAttrQuake() )
			{
				if( pMover->m_dwIndex == OBJECT_INDEX( 165, MI_BIGMUSCLE ) || 
					pMover->m_dwIndex == OBJECT_INDEX( 166, MI_KRRR ) || 
					pMover->m_dwIndex == OBJECT_INDEX( 1554, MI_KRRR2 ) ||
					pMover->m_dwIndex == OBJECT_INDEX( 167, MI_MUSHMOOT ) ||
					pMover->m_dwIndex == OBJECT_INDEX( 1552, MI_MUSHMOOT2 ) ||
					pMover->m_dwIndex == OBJECT_INDEX( 1565, MI_GIANTTREEK1 )
					)
					pMover->GetWorld()->m_pCamera->SetQuake( 60, 0.7f );	
			}
#endif // defined(__CLIENT)
			
		}
		break;
	}
}

//
//		Ư�����ݵ�����.
//
void	CActionMover::ProcessStateAttackSP( DWORD dwState, int /*nParam*/ )
{
	CMover* pMover = m_pMover;
	CModelObject	*pModel = (CModelObject *)pMover->m_pModel;
	
	switch( dwState )
	{
	case OBJSTA_SP_ATK1:
	case OBJSTA_SP_ATK2:
		if( pModel->IsEndFrame() )
		{
			ResetState( OBJSTA_ATK_ALL );
		}
		if( pModel->IsAttrHit() )			// ���� �����ӿ� Ÿ�ݼӼ��� �ִ°�? 
		{
			pMover->OnAttackSP();		// ������ Ư������ �ڵ鷯�� ȣ��.
		}
		break;
	}
}


//
//		�ϵ���ݵ���
//
void	CActionMover::ProcessStateAttackMagic( DWORD dwState, int /*nParam*/ )
{
	CMover* pMover = m_pMover;
	CModelObject	*pModel = (CModelObject *)pMover->m_pModel;
	
	switch( dwState )
	{
	case OBJSTA_ATK_MAGIC1:
		if( pModel->IsEndFrame() )
			ResetState( OBJSTA_ATK_ALL );

		if( pModel->IsAttrHit() )			// ���� �����ӿ� Ÿ�ݼӼ��� �ִ°�? 
		{
			pMover->OnAttackMagic();		// ������ �ϵ���� �ڵ鷯�� ȣ��.
		}
		break;
	}
}


//
//		���Ÿ����ݵ���
//
void	CActionMover::ProcessStateAttackRange( DWORD dwState, int /*nParam*/ )
{
	CMover* pMover = m_pMover;
	CModelObject	*pModel = (CModelObject *)pMover->m_pModel;
	
	switch( dwState )
	{
	case OBJSTA_ATK_RANGE1:
		// ������ ������ ���� ����
		if( pModel->IsEndFrame() )
		{
			ResetState( OBJSTA_ATK_ALL );
		}
		if( pModel->IsAttrHit() )			// ���� �����ӿ� Ÿ�ݼӼ��� �ִ°�? 
		{
			pMover->m_dwFlag |= MVRF_HITCONT;	
			CMover* pHitObj	= prj.GetMover( m_idTarget );

			if( IsValidObj( (CObj*)pHitObj ) && pHitObj->IsLive() )
				pMover->OnAttackRange();
		}
		break;
	}

}

