#include "stdafx.h"
#include "ActionMover.h"
#include "..\_Common\ParticleMng.h"
#ifdef __CLIENT
#include "..\_Common\TailEffectMng.h"
#include "../Neuz/DPClient.h"
#endif
#ifdef __WORLDSERVER
#include "../WorldServer/User.h"
#include "../WorldServer/DPCoreClient.h"
#endif


const float FRIC_AIR = 0.011f;		// ������ �������


#ifdef __CLIENT
extern	CDPClient	g_DPlay;
extern int			g_nFlySpeed;
extern int			g_nDrift;

void CreateFlyParticle( CMover *pMover, float fAngX, int nType )
{
	FLOAT fAngXZ, fAngH, fDist, fSpeed;
	D3DXVECTOR3 vVel;
	//D3DXVECTOR3 vPos = pMover->GetPos();
	D3DXVECTOR3 vPos = pMover->GetScrPos();
	if( nType == 0 )
		vPos.y += 0.8f;
	
	fAngXZ = pMover->GetScrAngle();
	fAngXZ += 180.0f; 
	fAngXZ = D3DXToRadian( fAngXZ );
	
	vPos.x += (xRandom(100) / 100.0f) - 0.50f;
	vPos.y += (xRandom(100) / 100.0f) - 0.50f;
	vPos.z += (xRandom(100) / 100.0f) - 0.50f;
	
	for( int i = 0; i < 2; i ++ )
	{
		fAngXZ = (float)(-45.0f + xRandomF(90));
		fAngXZ += pMover->GetScrAngle();
		fAngXZ += 180.0f;
		
		fAngXZ = D3DXToRadian( fAngXZ );
		fAngH = (float)(fAngX + xRandomF(10));
		fAngH = D3DXToRadian( fAngH );
		fSpeed = xRandomF(0.02f);
		
		fDist = cosf(-fAngH) * fSpeed;
		vVel.x = sinf(fAngXZ) * fDist;
		vVel.z = -cosf(fAngXZ) * fDist;
		vVel.y = -sinf(-fAngH) * fSpeed;
		if( nType == 1 ) // �ٵο��.
			g_ParticleMng.CreateParticle( 10 + xRandom(3), vPos, vVel, CMover::GetActiveMover()->GetPos().y + 0.5f );
		else
			g_ParticleMng.CreateParticle( 2 + xRandom(3), vPos, vVel, CMover::GetActiveMover()->GetPos().y + 0.5f );
	}
	
}

void CActionMover::ProcessFlyParticle( float fLenSq )
{
	CMover *pMover = m_pMover;

	// ��� �����嶧�� ��ƼŬ �ȳ���.
	if( (pMover->IsMode( TRANSPARENT_MODE ) ) == 0 )
	{
		PT_ITEM_SPEC pRideProp = g_xSpecManager->GetSpecItem( m_pMover->m_dwRideItemIdx );		// ���� Ÿ���ִ� Ż���� ������Ƽ.
		// ���� ��ƼŬ ó��.
		if( fLenSq > 0.01f )
		{
			if( (pMover->IsActiveMover() && g_Neuz.m_camera.m_fZoom > 2.0f) || pMover->IsActiveMover() == FALSE )		// �����ӵ� �̻��� �Ǹ� ������ ��ƼŬ�� ������ ����.
			{
				int nType = 0;
				if( pRideProp && pRideProp->dwID == ITEM_INDEX( 5800, II_RID_RID_BOR_RIDINGCLOUD ) )
					nType = 1;
				CreateFlyParticle( pMover, pMover->GetScrAngleX(), nType );
			}
		}
		
		if( fLenSq > 0.001f  )
		{
			if( (pMover->IsActiveMover() && g_Neuz.m_camera.m_fZoom > 1.0f) || pMover->IsActiveMover() == FALSE )
			{
				if( pRideProp && pRideProp->dwItemKind3 == IK3_BOARD )		// ���常 ������ ���´�.
				{
					if( m_pTail )
					{
						if( pRideProp->dwID == ITEM_INDEX( 5800, II_RID_RID_BOR_RIDINGCLOUD ) ) // �ٵο��϶�
						{
							if( m_pTail->GetType() != 2 )	// �����Ǿ��� ������ �ٵο���� �ƴϸ�
								m_pTail->ChangeTexture( D3DDEVICE, "etc_Tail2.bmp", 2 );
						} else
						{
							if( m_pTail->GetType() != 1 )	// �����Ǿ��� ������ �Ϲݺ������ �ƴϸ� 
								m_pTail->ChangeTexture( D3DDEVICE, "etc_Tail1.bmp", 1 );	// �Ϲݺ�������� �ؽ��� ��ü.
						}

							
					}
					if( m_pTail == NULL )	// ���� �Ҵ� �ȵ����� �Ҵ��ϰ�.
					{
						if( pRideProp->dwID == ITEM_INDEX( 5800, II_RID_RID_BOR_RIDINGCLOUD ) ) // �ٵο�...
						{
							m_pTail = (CTailEffectBelt*)g_TailEffectMng.AddEffect( g_Neuz.m_pd3dDevice, "etc_Tail2.bmp", 2 );
						}
						else
						{
							m_pTail = (CTailEffectBelt*)g_TailEffectMng.AddEffect( g_Neuz.m_pd3dDevice, "etc_Tail1.bmp", 1 );
						}
					}
					D3DXVECTOR3	vPos1, vPos2;
					D3DXVECTOR3	vLocal;
					FLOAT		fAngXZ = pMover->GetScrAngle();
					FLOAT		fAngH  = pMover->GetScrAngleX();
					
					AngleToVectorXZ( &vLocal, fAngXZ, -1.0f );

					fAngXZ -= 90.0f;
					if( fAngXZ < 0 )
						fAngXZ += 360.0f;
					AngleToVector( &vPos1, fAngXZ, -fAngH, 0.5f );
					vPos1 += pMover->GetScrPos();

					vPos1 += vLocal;
				
					fAngXZ = pMover->GetScrAngle();
					fAngH  = pMover->GetScrAngleX();
					
					AngleToVectorXZ( &vLocal, fAngXZ, -1.0f );

					fAngXZ += 90.0f;
					if( fAngXZ > 360.0f )
						fAngXZ -= 360.0f;
					AngleToVector( &vPos2, fAngXZ, -fAngH, 0.5f );
					vPos2 += pMover->GetScrPos();
					vPos2 += vLocal;
						
					if( m_pTail )
						m_pTail->CreateTail( vPos1, vPos2 );
				}
			}
		}
	} // �����尡 �ƴҶ�
}		

void CActionMover::ProcessFlyTracking()
{
	CMover* pMover = m_pMover;

	// �ڵ� ���� ���. g_pPlayer�� ����ȴ�.
	if( pMover->IsActiveMover() && (pMover->m_dwFlag & MVRF_TRACKING) )
	{
		static float s_fTurnAcc = 0, s_fTurnAccH = 0;
		{
			CMover *pTarget = prj.GetMover( pMover->m_idTracking );		// ������ ��ǥ.
			if( pTarget )
			{
				D3DXVECTOR3	vDist = pTarget->GetPos() - pMover->GetPos();		// ���� �������� Ÿ�ٱ����� ����.
				FLOAT fAngXZ, fAngH;
				
				xGetDegree( &fAngXZ, &fAngH, vDist );		// Ÿ�ٰ��� ���� ����.
				// ������ 0�� ����. �ð���� -180���� �ð�ݴ���� +180
				// 3�� ���ϴ� ����.
				FLOAT fMoverAng = pMover->GetAngle();
				if( fMoverAng > 180.0f )	// ����ϱ� ���� ��ǥ�踦 +,- �� �ٲ�.
					fMoverAng -= 360.0f;
				FLOAT fSubAng = fAngXZ - fMoverAng;
				if( fSubAng > 180.0f )
					fSubAng -= 360.0f;
				else if( fSubAng < -180.0f )
					fSubAng += 360.0f;
				
				if( fSubAng < -3.0f )		// ���������� ���ƾ� �Ѵ�.
					s_fTurnAcc = -2.5f;
				else 
				if( fSubAng > 3.0f )	// �������� ���ƾ� �Ѵ�.
					s_fTurnAcc = 2.5f;
				else
					s_fTurnAcc = 0;

				FLOAT fMoverAngX = pMover->GetAngleX();
				FLOAT fSubAngH = fAngH - fMoverAngX;
				if( fSubAngH > 180.0f )
					fSubAngH -= 360.0f;
				else if( fSubAngH < -180.0f )
					fSubAngH += 360.0f;

				if( fSubAngH < -3.0f )
					s_fTurnAccH = -1.5f;
				else
				if( fSubAngH > 3.0f )
					s_fTurnAccH = 1.5f;
				else
					s_fTurnAccH = 0;

				pMover->SetAngle( pMover->GetAngle() + s_fTurnAcc );
				pMover->SetAngleX( pMover->GetAngleX() + s_fTurnAccH );

				if( s_fTurnAcc || s_fTurnAccH )	// ���� �޶����� ������.
					g_DPlay.PostPlayerAngle( TRUE );
			}
		}
	}
}
#endif // __CLIENT

void	CActionMover::ProcessFlyMove( void )
{
#ifdef __CLIENT	
	g_nDrift = 0;									// �帮��Ʈ �÷� Ŭ���� 
#endif
	float fLenSq = D3DXVec3LengthSq( &m_vDelta );
	if( fLenSq == 0.0f && (GetStateFlag() & OBJSTAF_ACC ) == 0 )	  
		return;										// ���� ���¸� ���� 

	CMover* pMover = m_pMover;
	FLOAT fAccPwr = m_fAccPower;

#ifdef __CLIENT
	ProcessFlyTracking();
#endif // client
	
	// �ͺ���� ó�� 
	if( (GetStateFlag() & OBJSTAF_TURBO) && (GetStateFlag() & OBJSTAF_ACC) )		// �ͺ���� & ������
	{
	#ifdef __WORLDSERVER
		pMover->m_tmAccFuel	=  (int)( pMover->m_tmAccFuel - (1000.0f / (float)FRAME_PER_SEC) );		// 1/60��ŭ ����
	#endif
		if( pMover->m_tmAccFuel <= 0 )				// ���ӿ��ᰡ �� ��������
		{
			pMover->m_tmAccFuel = 0;
			SendActMsg( OBJMSG_MODE_TURBO_OFF );	// �ͺ���� ���� 
		#ifdef __WORLDSERVER
			g_xWSUserManager->AddSendActMsg( pMover, OBJMSG_MODE_TURBO_OFF );
		#endif
		}
		else
			fAccPwr *= 1.2f;						// ���ӿ��ᰡ �����ִٸ� �ͺ����
	}

#ifdef __CLIENT
	ProcessFlyParticle( fLenSq );
#endif

	// gmpbigsun( 20120802 ) : �ְ�ӵ����� �����¶����� ���� ���� ����
	BOOL bMaxSpeed = FALSE;

	// ����ó�� 
	if( fAccPwr > 0.0f )
	{
		// ������ ����
		FLOAT fAngX  = D3DXToRadian( pMover->GetAngleX() );		
		FLOAT fAng   = D3DXToRadian( pMover->GetAngle() );
		FLOAT fDist  = cosf(fAngX) * fAccPwr;		

		D3DXVECTOR3	vAcc;
		vAcc.x = sinf( fAng ) * fDist;
		vAcc.z = -cosf( fAng ) * fDist;
		vAcc.y = -sinf( fAngX ) * fAccPwr;

		// �������Ϳ� �����º��Ͱ� ������ 50�� ���ϸ� ������
		if( fLenSq > 0.01f )
		{
			D3DXVECTOR3 vDeltaNorm, vAccNorm;
			D3DXVec3Normalize( &vDeltaNorm, &m_vDelta );
			D3DXVec3Normalize( &vAccNorm, &vAcc );
			float fDot = D3DXVec3Dot( &vDeltaNorm, &vAccNorm );	

			if( fDot < 0.633319f )					// �����ڵ�: cosf(70.0f)  �����δ� �뷫 50�� 
			{
				vAcc *= 2.0f;						
				m_vDelta *= 0.985f;
			#ifdef __CLIENT
				g_nDrift = 1;
				
				if( ! ( pMover->IsMode( TRANSPARENT_MODE ) ) )		// ������°� �ƴҶ��� ����.
				{	
					if( (g_nProcessCnt & 3) == 0 )
						CreateSfx( g_Neuz.m_pd3dDevice, XI_INDEX( 1005, XI_NAT_DUST_RUN ), pMover->GetPos() );
				}
			#endif
			}
		}
			
		fLenSq = D3DXVec3LengthSq( &m_vDelta );		// 1/60 sec �ӵ�
		float fMaxSpeed = 0.3f;
		if( GetStateFlag() & OBJSTAF_TURBO )		// �ͺ���忡�� MAX�ӵ��� 1.1��
			fMaxSpeed *= 1.1f;
			
		if( fLenSq < (fMaxSpeed * fMaxSpeed) )		// �����̻� �ӵ��� ���� �ʰ� ����.
			m_vDelta += vAcc;						// �������� += �����º���
		else
			bMaxSpeed = TRUE;

	}


	if( FALSE == bMaxSpeed )
		m_vDelta *= (1.0f - FRIC_AIR);					// �����¿� ���� ����  

	// raiders - ��ġ�� �������� ���ؼ� ���� ��ġ�� ���Ǵ� ���� ���Ѵ�.
	fLenSq = D3DXVec3LengthSq( &m_vDelta );
	if( m_fAccPower == 0.0f && fLenSq < 0.0002f * 0.0002f )		
	{
		fLenSq = 0;
		m_vDelta = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );		
		RemoveStateFlag( OBJSTAF_ACC );				// ���ӻ��� ���� 
	}

#ifdef __CLIENT
	if( pMover->IsActiveMover() )
	{
		float fSpeedTemp = (sqrt(fLenSq) * 60.0f) * 60.0f * 60.0f;
	//	g_nFlySpeed = (int)( (sqrt(fLenSq) * 60.0f) * 60.0f * 60.0f );
		g_nFlySpeed	= (int)( fSpeedTemp / 200.0f );
	}
#endif
}

//
//	State process
//	������ �̰����� pMover->SetMotion�� ���� ����
//	���ڷ� ������
//
void	CActionMover::ProcessStateFly( CMover* pMover, DWORD dwState, float /*fSpeed*/ )
{
	int nParam = 0;
	float fAng = pMover->GetAngleX();
	CModelObject* pModel = (CModelObject*)pMover->m_pModel;

	PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( m_pMover->m_dwRideItemIdx );
	if( pItemProp == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s, m_dwRideItemIdx == %d" ), m_pMover->m_szName, m_pMover->m_dwRideItemIdx );
		return;
	}

	switch( dwState )
	{
	// ���ڸ� ��� / ����
	case OBJSTA_STAND:
		m_fAccPower = 0;		// ���/�������¿��� ���� ���̻� ������ �ʴ´�.
		if( GetState() & OBJSTA_ATK_ALL )	break;
		if( GetState() & OBJSTA_TURN_ALL )	break;
		if( GetState() & OBJSTA_DMG_ALL )	break;
		{
			if( pMover->SetMotion( pItemProp->dwUseMotion + MTA_FSTAND1, ANILOOP_LOOP, MOP_FIXED ) == TRUE )		// ������
				if( pMover->m_pRide )	
					pMover->m_pRide->m_fFrameCurrent = 0;
		}
		break;

	// ����
	case OBJSTA_FMOVE:
		// ������ Ÿ���ִ� �������� �ε������� �������� ������.
		m_fAccPower = pItemProp->fFlightSpeed * 0.75f;

		if( GetState() & OBJSTA_ATK_ALL )	break;
		if( GetState() & OBJSTA_TURN_ALL )	break;
		{
			if( pMover->SetMotion( pItemProp->dwUseMotion + MTA_FRUNNING1, ANILOOP_LOOP, MOP_FIXED ) == TRUE )		// ������
				if( pMover->m_pRide )		
					pMover->m_pRide->m_fFrameCurrent = 0;
		}
		break;

	// ��/�� ����
	case OBJSTA_LTURN:
		{
			float fTurnAng = m_fTurnAngle;
			if( (GetStateFlag() & OBJSTAF_ACC) == 0 )		// ���ӻ��°� �ƴҶ��� 2��� ���� ����.
				fTurnAng *= 2.5f;
			pMover->AddAngle( fTurnAng );
		}
		if( IsActAttack() )	break;
		pMover->SetMotion( pItemProp->dwUseMotion + MTA_FLTURN1, ANILOOP_LOOP, MOP_FIXED );
		break;

	case OBJSTA_RTURN:
		{
			float fTurnAng = m_fTurnAngle;
			if( (GetStateFlag() & OBJSTAF_ACC) == 0 )		// ���ӻ��°� �ƴҶ��� 2��� ���� ����.
				fTurnAng *= 2.5f;
			pMover->AddAngle( -fTurnAng );
		}
		if( IsActAttack() )	break;
		pMover->SetMotion( pItemProp->dwUseMotion + MTA_FRTURN1, ANILOOP_LOOP, MOP_FIXED );
		break;

	case OBJSTA_LOOKUP:
		if( fAng > -45.0f )
			pMover->AddAngleX( -pItemProp->fFlightTBAngle );
		break;

	case OBJSTA_LOOKDOWN:
		if( fAng < 45.0f )
			pMover->AddAngleX( pItemProp->fFlightTBAngle );
		break;

	//--------- ���� ---------------------------
	case OBJSTA_ATK1:
	case OBJSTA_ATK2:
	case OBJSTA_ATK3:
	case OBJSTA_ATK4:
	case OBJSTA_ATK5:
		ProcessStateAttackMeleeFly( dwState, nParam );
		break;

	case OBJSTA_ATK_MAGIC1:		// ���Ÿ� �ϵ� ����.
		ProcessStateAttackMagic( dwState, nParam );
		break;

	//----------- ������ -------------------------
	case OBJSTA_DMG:
		if( pModel->IsEndFrame() )	
			ResetState( OBJSTA_DMG_ALL );
		break;

	case OBJSTA_DEAD:	// �׾� �κη��� ����
#ifdef __WORLDSERVER
		if( !pMover->IsPlayer() ) 
		{
			m_nDeadCnt--;
			if( m_nDeadCnt < 0 )
			{
				SetState( OBJSTA_DMG_ALL, OBJSTA_DISAPPEAR );	// ������� ���·� ��ȯ
				m_nCount = 0;
			}
		}
#endif
		break;
	}
}

//
//		������ ���� �׼� ó��
//
void	CActionMover::ProcessStateAttackMeleeFly( DWORD dwState, int /*nParam*/ )
{
	CMover* pMover = m_pMover;
	CModelObject	*pModel = (CModelObject *)pMover->m_pModel;
	
	switch( dwState )
	{
	case OBJSTA_ATK1:
		if( pModel->IsEndFrame() )
			ResetState( OBJSTA_ATK_ALL );

		if( pModel->m_nPause > 0 )
		{
			--pModel->m_nPause;
		} 
		else
		{
			if( pModel->IsAttrHit() )
			{
				CMover* pHitObj	= prj.GetMover( m_objidHit );
				if( IsInvalidObj( pHitObj ) || pHitObj->IsDie() )	// Ÿ���� �Žñ��ϰų� �׾����� ���
					return;

				BOOL bSuccess = pHitObj->m_pActMover->SendDamage( AF_GENERIC, pMover->GetId() );
				if( bSuccess == FALSE )
					return;
#ifdef __CLIENT
				PT_ITEM_SPEC pItemProp = pMover->GetActiveHandItemProp();
				if( pItemProp != NULL )
				{
					D3DXVECTOR3 v = pMover->GetPos();
					PLAYSND( pItemProp->dwSndAttack1, &v );
				}
#endif

				if( GetMover()->IsPlayer() )
				{
					pModel->m_nPause = 5;	// frame ����
				}
				else
				{
					pModel->m_nPause = 0;	// ���ʹ� ������ ����
					pHitObj->m_pModel->m_nPause = 0;
				}
					
			}
		}
		break;
	}
}
