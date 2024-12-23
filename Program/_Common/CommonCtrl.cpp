#include "stdafx.h"
#include "CommonCtrl.h"
#include "CreateObj.h"

#ifdef __WORLDSERVER
#include "../WorldServer/user.h"
#include "../worldserver/LinkMap.h"
#endif	// __WORLDSERVER

#include "FLSkillSystem.h"


typedef map< string, DWORD >			CMapStrToObjId;
typedef CMapStrToObjId :: value_type	MapStrToObjIdType;

CMapStrToObjId	g_MapStrToObjId;
 
CCommonCtrl::CCommonCtrl()
{
	m_dwType = OT_CTRL;

#ifdef __WORLDSERVER
	m_fOriginalOBBCenterHeight = 0.0f;
#endif // __WORLDSERVER

	/////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory( &m_CtrlElem, sizeof( m_CtrlElem ) );
	/////////////////////////////////////////////////////////////////////////////////////////
/*
	m_CtrlElem.m_dwMinItemNum			= 1;			//-�ּ� �߻� ������ �� 
	m_CtrlElem.m_dwMaxiItemNum			= 3;			//-�ִ� �߻� ������ �� 

	m_CtrlElem.m_dwInsideItemKind[0] = II_WEA_SWO_FLOWER;
	m_CtrlElem.m_dwInsideItemPer [0] = 1500000000;
	m_CtrlElem.m_dwInsideItemKind[1] = II_WEA_CHEE_ZANNE;
	m_CtrlElem.m_dwInsideItemPer [1] = 1500000000;
	m_CtrlElem.m_dwInsideItemKind[2] = II_WEA_WAN_BLUEBOW;
	m_CtrlElem.m_dwInsideItemPer [2] = 1500000000;
	m_CtrlElem.m_dwInsideItemKind[3] = II_ARM_ARM_SHI_FURY;
	m_CtrlElem.m_dwInsideItemPer [3] = 1500000000;
	
	m_CtrlElem.m_dwMonResKind  [0]      = MI_DEMIAN2;
	m_CtrlElem.m_dwMonResNum   [0]		= 0;
	m_CtrlElem.m_dwMonActAttack[0]		= 0;
	m_CtrlElem.m_dwMonResKind  [1]      = MI_BURUDENG2;
	m_CtrlElem.m_dwMonResNum   [1]		= 0;
	m_CtrlElem.m_dwMonActAttack[1]		= 0;
	m_CtrlElem.m_dwMonResKind  [2]      = MI_MIA2;
	m_CtrlElem.m_dwMonResNum   [2]		= 0;
	m_CtrlElem.m_dwMonActAttack[2]		= 0;

	m_CtrlElem.m_dwTrapOperType         = xRandom(2);
	m_CtrlElem.m_dwTrapRandomPer        = 2000000000;
	m_CtrlElem.m_dwTrapDelay            = 1000;
	
	m_CtrlElem.m_dwTrapKind [0] = SI_GEN_STUNSINGLE;    
	//m_CtrlElem.m_dwTrapKind [0] = SI_GEN_STUNGROUP;    
	m_CtrlElem.m_dwTrapLevel[0] = 3;

	m_CtrlElem.m_dwTrapKind [1] = SI_GEN_FLASH;
	//m_CtrlElem.m_dwTrapKind [0] = SI_GEN_STUNGROUP;    
	m_CtrlElem.m_dwTrapLevel[1] = 3;
	
*/		
	/////////////////////////////////////////////////////////////////////////////////////////
	SetAngle( (float)( xRandom(360) ) );
	
	Init();	
}
CCommonCtrl::~CCommonCtrl()
{
#if !defined(__CLIENT)
	CWorld* pWorld = GetWorld();
	if( pWorld != NULL )
	{
		pWorld->m_respawner.Increment( GetRespawn(), m_nRespawnType, FALSE, GetLayer() );
	}
#endif

#ifdef __CLIENT
	SAFE_DELETE( m_pSfxModel );
	SAFE_DELETE( m_pSfxModel2 );
#endif // __CLIENT
}

BOOL CCommonCtrl::Read( CFileIO* pFile )
{
	CObj::Read( pFile );

	CCtrlElem* pCtrlElem;
	pCtrlElem = &m_CtrlElem;
	DWORD dwVersion;
	pFile->Read( &dwVersion, sizeof( DWORD ) );
	if( dwVersion == 0x80000000 )
	{
		pFile->Read( pCtrlElem, sizeof( CCtrlElem ) );
	}
	else
	if( dwVersion == 0x90000000 )
	{
//sun: 10�� ���½ý���	Neuz, World, Trans
		// ���� : 368
		// ���� : 432
		pFile->Read( pCtrlElem, 88 );
		pFile->Read( ((BYTE*)pCtrlElem + 152), sizeof(CCtrlElem) - 152 );	// 432 - 64 = 368

//		pFile->Read( pCtrlElem, sizeof( CCtrlElem ) );	//sun: 10�� ���½ý���	Neuz, World, Trans
	}
	else
	{
		pCtrlElem->m_dwSet = dwVersion;
		pFile->Read( &pCtrlElem->m_dwSetItem, sizeof( CCtrlElem ) - (( sizeof( DWORD ) * 10 )) );		
		// m_dwSet, m_dwSetItemCount, m_dwSetQuestNum1, m_dwSetFlagNum1
	}
	return TRUE;
}
void CCommonCtrl::DestroyWall( void )
{
#ifdef __WORLDSERVER
	// �������� ����ű�� ������������ ������.
	CMover *pAttacker = prj.GetMover( m_idAttacker );
	if( IsValidObj(pAttacker) )
	{
		if( pAttacker->IsPlayer() )
		{
			for( int i = 0; i < 2; i ++ )
			{
				if( ((FLWSUser *)pAttacker)->m_pWall[i] == this )		
					((FLWSUser *)pAttacker)->m_pWall[i] = NULL;
			}
		}
	}
	
	Delete();
#endif // WorldServer
}

// ����ű ��
void CCommonCtrl::_ProcessWall( void )
{
	if( m_nCount == 0 )
	{
	#ifdef __CLIENT		
		m_pSfxModel = new CSfxModel;
		m_pSfxModel2 = new CSfxModel;
		m_pSfxModel->SetSfx( "sfx_sklpsypsychicwall02" );
		m_pSfxModel2->SetSfx( "sfx_sklpsypsychicwall04" );
	#endif
	}

	D3DXVECTOR3	vPos = GetPos();

#ifndef __CLIENT
	CObj* pObj;
	BOOL bApply;
#endif //__CLIENT

	int nRange = 4;
	// �Ϲ������� fDepth�� ���� ��⶧���� �˻� ������ fDepth�� �ߴ�. 
	float fDepth = 3;
	
	if( fDepth <= 4.0f )		nRange = 4;
	else if( fDepth <= 8.0f )	nRange = 8;
	else if( fDepth <= 16.0f )	nRange = 16;
	else						nRange = 32;

#ifdef __WORLDSERVER
	CMover *pAttacker = prj.GetMover( m_idAttacker );
	
	if( IsInvalidObj( pAttacker ) )		// �ϴ� ����Ŀ�� ������� ��Ʈ�ѵ� ������� ����.
	{
		DestroyWall();
		return;
	}
	int nMin = m_pAddSkillProp->dwAbilityMin + (pAttacker->GetLevel() + (pAttacker->GetInt() / 10) * (int)m_pAddSkillProp->dwSkillLvl);
	int nMax = m_pAddSkillProp->dwAbilityMax + (pAttacker->GetLevel() + (pAttacker->GetInt() / 10) * (int)m_pAddSkillProp->dwSkillLvl);
	int nDamage = xRandom( nMin, nMax );
//sun: 9, ���ο� AddSkillProp �÷� �߰� �� ����
	int nMinPVP	= m_pAddSkillProp->dwAbilityMinPVP + ( pAttacker->GetLevel() + ( pAttacker->GetInt() / 10 ) * (int)m_pAddSkillProp->dwSkillLvl );
	int nMaxPVP		= m_pAddSkillProp->dwAbilityMaxPVP + ( pAttacker->GetLevel() + ( pAttacker->GetInt() / 10 ) * (int)m_pAddSkillProp->dwSkillLvl );
	int nDamagePVP	= xRandom( nMinPVP, nMaxPVP );

	int nHitPoint = 0;
	int nTargetHP = 0;
	
	FOR_LINKMAP( GetWorld(), vPos, pObj, nRange, CObj::linkDynamic, GetLayer() )
	{
		bApply = FALSE;
		if( pObj->GetType() == OT_MOVER )				// ����� �����϶���.
		{
			CMover *pTarget = (CMover *)pObj;
			if( pTarget->IsPeaceful() == FALSE )		// NPC�� �ƴѰ�츸 ����
				bApply = TRUE;
//sun: 8, 8.5�� ��� ������ų ����ȿ�� �Ұ��� ���� World
			if( pAttacker->IsPlayer() && pAttacker->IsChaotic() == FALSE && pTarget->GetProp()->dwClass == RANK_GUARD )
				bApply = FALSE;

			if( pTarget->m_dwMode & MATCHLESS_MODE )	//�����̸� ��ŵ
				bApply = FALSE;

			if( bApply )
			{
				if( IsValidObj( pTarget ) && pTarget->IsLive() )
				{
					if( pObj->IsRangeObj( vPos, 1.0f ) )
					{
						if( IsValidObj(pAttacker) == TRUE )	//������ �ƴϸ�
						{
							nTargetHP = pTarget->GetHitPoint();
							nHitPoint = nTargetHP - nDamage;

							if( nHitPoint > 0 )
							{
								pTarget->m_nHitPoint = nHitPoint;
								g_xWSUserManager->AddDamage( pTarget, pAttacker->GetId(), nDamage, AF_GENERIC );
							}
							else
							{
								pAttacker->SubExperience( pTarget );		// pTarget�� ���̰� ������ m_pAttacker ����ġ ó��.

								switch( _GetContentState( CT_DROP_ITEM_REFORM ) )
								{
								case CS_VER1:
									{
										pTarget->OnDie( *pAttacker );
									}
									break;
								default:
									pTarget->DropItemByDied( pAttacker );				// ���Ϳ��ٸ� ������ ���.
									break;
								}

								
								pAttacker->m_nAtkCnt = 0;					// Ÿ���� �׿����� �������� ����ī��Ʈ Ŭ����
								pTarget->DoDie( pAttacker );				// pTarget �׾��. 
								pTarget->m_nHitPoint = 0;
							}
						}
						m_nLife ++;		// �ε��������� ī��Ʈ �ö�
						if( m_nLife >= (int)(m_pAddSkillProp->dwSkillLvl / 2) )
							DestroyWall();
						
						// �ڷ� �и��� ó��.
//sun: 10, ���׿���Ŀ AI ����
						if( pTarget->IsRank( RANK_MIDBOSS ) == FALSE )
						{
							pTarget->pushedByAnotherPower( pTarget->GetAngle() + 180.0f, 0.825f );
						}
					}
				}
			}
		}
	}
	END_LINKMAP

	// �÷��̾� ��ũ���̹Ƿ� �����ڰ� �÷��̾�� PVP�̴�.
	BOOL bPVP	= pAttacker->IsPlayer();
	FOR_LINKMAP( GetWorld(), vPos, pObj, nRange, CObj::linkPlayer, GetLayer() )
	{
		if( pObj->GetType() == OT_MOVER )				// ����� �����϶���.
		{
			CMover *pTarget = (CMover *)pObj;
			if( pTarget->IsLive() && pAttacker != pTarget )
			{
				if( pTarget->m_dwMode & MATCHLESS_MODE )	//�����̸� ��ŵ
					bApply = FALSE;

				if( pObj->IsRangeObj( vPos, 1.0f ) )
				{
//sun: 9, ���ο� AddSkillProp �÷� �߰� �� ����
					int n = 0;
					if( bPVP && pAttacker->IsPVPTarget( pTarget ) )
						n	= pTarget->m_pActMover->SendDamage( AF_FORCE, pAttacker->GetId(), nDamagePVP, FALSE );	
					else if( bPVP && (m_bControl || pAttacker->IsGuildCombatTarget( pTarget )
								/*�Ʒ��� �߰�*/ || pAttacker->IsArenaTarget( pTarget )
												 || pAttacker->IsEventArenaTarget( pTarget )
						) )
						n	= pTarget->m_pActMover->SendDamage( AF_FORCE, pAttacker->GetId(), nDamage, FALSE );	

					if( n > 0 )
					{
						m_nLife ++;		// �ε��������� ī��Ʈ �ö�
						if( m_nLife >= (int)( m_pAddSkillProp->dwSkillLvl / 2 ) )
							DestroyWall();

						// �ڷ� �и��� ó��.
						pTarget->pushedByAnotherPower( pTarget->GetAngle() + 180.0f, 0.825f );
					}
				}
			}
		}
	}
	END_LINKMAP
#endif // WorldSErver

#ifdef __CLIENT				
	if( m_pSfxModel )
	{
		if( m_pSfxModel->Process() )		// ��������Ʈ�� ������ ���ư���
		{
			if( m_nEvent == 0 )
			{
				m_pSfxModel->SetSfx( "sfx_sklpsypsychicwall03" );		// ���� ����Ʈ�� �ٲ� 
				m_nEvent = 1;
			} else
			{
				m_pSfxModel->m_nCurFrame = 0;		// ���� ����Ʈ�� �ݺ�
			}
		}
	}
#endif

#ifdef __WORLDSERVER
	// �ð� �ٵǸ� ��.
	if( (int)(g_tmCurrent - m_tmStart) > (int)m_pAddSkillProp->dwSkillTime )
	{
		DestroyWall();
		return;
	}
#endif // WorldServer
}

void CCommonCtrl::Process()
{
	if( GetIndex() == CTRL_INDEX( 29, CI_PSYCHICWALL ) )		// ����ű���� ��� ���� ����ó��
	{
		_ProcessWall();
		m_nCount ++;
		return;
	}

	if( m_pModel == NULL )
		return;
	
	CtrlProp* pProp = GetProp();
	if( pProp == NULL )
		return;

#ifdef __WORLDSERVER
	if( m_dwDelete != 0xffffffff )
	{
		if( g_tmCurrent > m_dwDelete )
		{
			Delete();
			return;
		}
	}

	if( m_dwCtrlReadyTime != 0xffffffff )
	{
		if( g_tmCurrent > m_dwCtrlReadyTime )
		{
			CMover* pMover = prj.GetUserByID(m_idExpPlayer);

			if( IsValidObj( pMover ) )
			{
				FLWSUser* pUser = (FLWSUser*)pMover;

				EXPINTEGER nGapGap = m_nExpBox;
				
				if( pUser->AddExperience( nGapGap, TRUE, FALSE ) )
					pUser->LevelUpSetting();
				else
					pUser->ExpUpSetting();

				g_dpDBClient.SendLogExpBox( pUser->m_idPlayer, GetId(), m_nExpBox, TRUE );

				pUser->AddSetExperience( pUser->GetExp1(), (WORD)pUser->m_nLevel, pUser->m_nSkillPoint, pUser->m_nSkillLevel );
				g_xWSUserManager->AddCreateSfxObj((CMover *)pUser, XI_INDEX( 108, XI_SYS_EXCHAN01 ), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z);						
				  
				pUser->AddDefinedText( TID_GAME_EXPBOX_EAT, "" );						
				//pUser->AddChatText( TID_GAME_EXPBOX_EAT, "");

				m_dwCtrlReadyTime = 0xffffffff;
				pMover->m_dwCtrlReadyTime = 0xffffffff;
				pMover->m_dwCtrlReadyId   = NULL_ID;
				pUser->AddCtrlCoolTimeCancel();
					
				m_bAction = TRUE;
				m_bAlpha = TRUE;
			}
		}
	}

	for( int i = 0; i < 4; i++ )
	{
		// �Ӽ��˻��Ͽ� ������ ó��
		if( ((CModelObject*)m_pModel)->IsAttrHit() )
			_ProcessAction();

		if( m_pModel->IsEndFrame() && m_bTrap )
			m_bAlpha = TRUE;
#endif// __WORLDSERVER
		
	#ifdef __CLIENT
		m_wBlendFactor = m_nBlendFactorLocal;
	#endif 	//__CLIENT	
		
		m_pModel->FrameMove();	

#ifdef __WORLDSERVER
		if( IsDynamicBoundBoxCtrl_HardCording() == true && m_pModel->m_bEndFrame == TRUE )
		{
			ForceRestoreBoundBoxHeight_HardCording();
		}
#endif // __WORLDSERVER
		
		if( m_bAniPlay == FALSE )
			m_pModel->m_fFrameCurrent = m_fFrameCount;
		else
			m_fFrameCount = m_pModel->m_fFrameCurrent;

		// ������� ��Ʈ��
		if( pProp->dwCtrlKind2 == CK2_FADE )
		{			
			if( m_bAlpha )
			{
				m_nBlendFactorLocal-=5;
				
				if( m_nBlendFactorLocal < 0 )
				{
					m_nBlendFactorLocal = 0;
				#ifdef __WORLDSERVER
					Delete();
					return;
				#endif //__WORLDSERVER
				}
			}
		}
		else
		// ������� �ʴ� ��Ʈ���� �ٽ� �۵������ϰ� ����  (��)Ʈ����
		if( pProp->dwCtrlKind2 == CK2_KEEP )
		{
			if( m_pModel->m_bEndFrame )
			{
				Init();

				if( pProp->IsGuildHousingObj( ) )		//gmpbigsun : ��� �Ͽ콺 ������Ʈ�� �ִϸ��̼� ���� 
				{
					CModel* pModel = (CModel*)GetModel();
					FLASSERT( pModel );

					if( pModel->m_nFrameMax > 0 )
						m_bAniPlay = TRUE;
				//	pModel->SetLoop( ANILOOP_LOOP );
				}

				m_pModel->m_bEndFrame     = FALSE;
				m_pModel->m_fFrameCurrent = 0.0f;
			}
		}
		
		if( pProp->dwCtrlKind3 == CK3_HALF )
		{
			// Ǯ���ϸ��̼��� �ƴ϶��...���� �������� ��ü�� 50%�϶� �����.
			if( m_bAniPlay && m_pModel->m_fFrameCurrent == m_pModel->GetMaxFrame()/2 ) 
			{
				m_bAniPlay = FALSE;
				m_dwAniDelay = g_tmCurrent+SEC(20);  //���� �ð��ڿ� ������ ���ϸ��̼� ������
			}
			
			// ���� �ð��� �������� ������ ���ϸ��̼� ����
			if( !m_bAniPlay && m_dwAniDelay < g_tmCurrent )
			{	
				m_dwAniDelay = 0xffffffff;
				m_bAniPlay = TRUE;
			}
			
		}
#ifdef __WORLDSERVER
		_ProcessTrap();
	}
#endif// __WORLDSERVER

}

void CCommonCtrl::_ProcessAction()
{
#ifdef __WORLDSERVER
	// Ʈ������ 
	if( m_bTrap )
		return;

	m_dwTrapProcessTime = g_tmCurrent + m_CtrlElem.m_dwTrapDelay;

	
	// ������, ���� ���
	//if( m_CtrlElem.m_dwMinItemNum > 0 && m_CtrlElem.m_dwMaxiItemNum > m_CtrlElem.m_dwMinItemNum )
	if( m_CtrlElem.m_dwMinItemNum > 0 && m_CtrlElem.m_dwMaxiItemNum >= m_CtrlElem.m_dwMinItemNum )
	{
		DropItem();
	}

	DropNPC();	
#endif //__WORLDSERVER	
}

void CCommonCtrl::_ProcessTrap()
{
#ifdef __WORLDSERVER

	// Ʈ���ߵ��ð��� üũ�Ͽ� �߻���
	if( m_dwTrapProcessTime < g_tmCurrent )
	{
		CtrlProp* pProp = GetProp();
		if( pProp == NULL )
			return;
		
		// ������� ��Ʈ�Ѹ� Ŭ�󿡰� ����
		if( pProp->dwCtrlKind2 == CK2_FADE )
			g_xWSUserManager->AddCommonPlace( this, COMMONPLACE_ALPHA );

		m_bTrap = TRUE;
		m_dwTrapProcessTime = 0xffffffff;

		
		// �߻� ��Ŀ� ����...
		if( m_CtrlElem.m_dwTrapOperType == TOT_RANDOM )    // ���� ���� ���
		{
			if( m_CtrlElem.m_dwTrapRandomPer >= xRandom( 3000000000 ) )
			{
				_CreateTrapSkill();
			}
		}
		else
		if( m_CtrlElem.m_dwTrapOperType == TOT_NOENDU )  // ������ 0�ϰ��
		{
			if( m_CtrlElem.m_dwSetEndu <= 0 )
			{
				_CreateTrapSkill();
			}
		}

		// ��ũ�� ������Ʈ(��Ʈ��) �ߵ�
		if( m_CtrlElem.m_strLinkCtrlKey[0] != '\0' )
		{
			map<string, DWORD>::iterator i = g_MapStrToObjId.find(m_CtrlElem.m_strLinkCtrlKey);
			if( i != g_MapStrToObjId.end() )
			{
				OBJID objId = (*i).second;
				CCtrl* pCtrl = prj.GetCtrl( objId );
				if( IsValidObj( pCtrl ) )
				{
					if( ((CCommonCtrl*)pCtrl)->m_pModel && ((CCommonCtrl*)pCtrl)->m_pModel->m_fFrameCurrent != 0.0f )
						return;		
					
					((CCommonCtrl*)pCtrl)->SetActionPlay();
				}
			}
		}
	}
	
#endif //__WORLDSERVER	
}


void CCommonCtrl::_CreateTrapSkill()
{
#ifdef __WORLDSERVER
	for( int i=0; i<MAX_TRAP; i++ )
	{
		if( m_CtrlElem.m_dwTrapLevel[i] == 0 || m_CtrlElem.m_dwTrapLevel[i] == NULL_ID )
			continue;

		SkillProp* pSkill = prj.GetSkillProp(m_CtrlElem.m_dwTrapKind[i]);
		
		if( pSkill == NULL  )
			continue;

		if( m_CtrlElem.m_dwTrapLevel[i] > pSkill->dwExpertMax )
		{
			FLERROR_LOG( PROGRAM_NAME, _T( "�ִ���� �������� ũ�� : %s, %d" ), pSkill->szName, m_CtrlElem.m_dwTrapLevel[i] );
			continue;
		}
		
		AddSkillProp* pAddSkill = prj.GetAddSkillProp( pSkill->dwSubDefine, m_CtrlElem.m_dwTrapLevel[i] );
		
		if( pAddSkill == NULL )
			continue;

		if( pSkill->dwUseChance == WUI_NOW )
		{
			g_cSkillSystem->DoApplySkill( this, this, pSkill, pAddSkill );
		}
		else
		{
			CCtrl* pCtrl = prj.GetCtrl( m_nMoverID );
			if( IsValidObj( pCtrl ) == FALSE )
				continue;

			g_cSkillSystem->DoApplySkill( this, pCtrl, pSkill, pAddSkill );
		}
		
		g_xWSUserManager->AddDoApplySkill( this, m_nMoverID, m_CtrlElem.m_dwTrapKind[i] , m_CtrlElem.m_dwTrapLevel[i] );
		
	}
#endif //__WORLDSERVER		
}

#ifdef __CLIENT
void CCommonCtrl::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	if( GetIndex() == CTRL_INDEX( 29, CI_PSYCHICWALL ) )		// ����ű���ϰ�� ���� ó�� - �ϴ� ����.
	{
		m_pSfxModel->m_vPos = GetPos();
		m_pSfxModel->m_vRotate.y = GetAngle();
		m_pSfxModel->m_vScale = GetScale();
		m_pSfxModel->m_matScale = m_matScale;
		//m_pSfxModel->Render( pd3dDevice, NULL );
		m_pSfxModel->RenderZ( pd3dDevice, NULL );
		return;
	}
		
	CtrlProp *pCtrlProp = GetProp();

	if( GetIndex() == CTRL_INDEX( 46, CI_EXPBOX ) )		// ����ű���ϰ�� ���� ó�� - �ϴ� ����.
	{
		AddAngle( 1.0f );
	}
	
	CObj::Render( pd3dDevice );
}
#else
void CCommonCtrl::Render( LPDIRECT3DDEVICE9 /*pd3dDevice*/ )
{
}

#endif// __CLIENT

#ifndef __WORLDSERVER
void CCommonCtrl::RenderName( LPDIRECT3DDEVICE9 pd3dDevice, CD3DFont* pFont, DWORD dwColor )
{
	// ���� ��ǥ�� ��ũ�� ��ǥ�� �������� �Ѵ�.
	D3DXVECTOR3 vOut, vPos = GetPos(), vPosHeight;
    D3DVIEWPORT9 vp;
	const BOUND_BOX* pBB = m_pModel->GetBBVector();
    pd3dDevice->GetViewport( &vp );
	D3DXMATRIX matTrans;
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation( &matTrans, vPos.x, vPos.y, vPos.z );
	D3DXMatrixMultiply( &matWorld, &matWorld, &m_matScale );
	D3DXMatrixMultiply( &matWorld, &matWorld, &m_matRotation );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matTrans );
	
	vPosHeight = pBB->m_vPos[0];
	vPosHeight.x = 0;
	vPosHeight.z = 0;
	
	D3DXVec3Project( &vOut, &vPosHeight, &vp, &GetWorld()->m_matProj,
		&GetWorld()->m_pCamera->m_matView, &matWorld);
	vOut.x -= pFont->GetTextExtent( GetProp()->szName ).cx / 2;
	pFont->DrawText( vOut.x + 1, vOut.y + 1, 0xff000000, GetProp()->szName );
	pFont->DrawText( vOut.x, vOut.y, dwColor, GetProp()->szName );
	return;
}
#else
void CCommonCtrl::RenderName( LPDIRECT3DDEVICE9 /*pd3dDevice*/, CD3DFont* /*pFont*/, DWORD /*dwColor*/ )
{

}
#endif	// __WORLDSERVER



/*
#ifdef __WORLDSERVER
void CCommonCtrl::SendActionError()
{
	g_xWSUserManager->AddCommonPlace( this, COMMONPLACE_ERROR );
}
#endif //__WORLDSERVER
*/
void CCommonCtrl::SetActionPlay()
{
#ifdef __WORLDSERVER
	g_xWSUserManager->AddCommonPlace( this, COMMONPLACE_ACTION );

	if( IsDynamicBoundBoxCtrl_HardCording() == true )
	{
		ForceSetBoundBoxHeight_HardCording();
	}
#endif //__WORLDSERVER
	
#ifdef __CLIENT
	if( !m_bAniPlay )
	{
		CtrlProp* pCtrlProp = GetProp();		
		
		if( pCtrlProp )
		{
			if( pCtrlProp && pCtrlProp->dwSfxCtrl != -1 )
			{
				CreateSfx( g_Neuz.m_pd3dDevice, pCtrlProp->dwSfxCtrl, GetPos() );
			}
		}
	}
#endif //__CLIENT
	
	m_pModel->SetLoop( ANILOOP_CONT );
	m_bAniPlay = TRUE;
}

void CCommonCtrl::DropItem()
{
	//int nTotalNum = (xRandom(m_CtrlElem.m_dwMaxiItemNum - m_CtrlElem.m_dwMinItemNum) + m_CtrlElem.m_dwMinItemNum) + 1;
	int nTotalNum = (xRandom(m_CtrlElem.m_dwMaxiItemNum - m_CtrlElem.m_dwMinItemNum) + m_CtrlElem.m_dwMinItemNum);
	DWORD	dwAdjRand;

	for( int i=0; i<MAX_CTRLDROPITEM; i++ )
	{
		PT_ITEM_SPEC pItemProp = g_xSpecManager->GetSpecItem( m_CtrlElem.m_dwInsideItemKind[i] );
		
		if( pItemProp == NULL )
		{
			return;
		}

		dwAdjRand	= xRandom( 3000000000 );

		if( m_CtrlElem.m_dwInsideItemPer[i] >= dwAdjRand && nTotalNum )
		{
			FLItemElem* pItemElem	= new FLItemElem;
			pItemElem->m_dwItemId	= pItemProp->dwID;
			pItemElem->m_nItemNum	= 1;

			// ������ ������ ��������ۿ뽽�� ����������� 1���� �ʰ��Ҽ� ����.
			if( i == MAX_CTRLDROPITEM-1 )
			{
				if( pItemElem->m_nItemNum > 1 )
					pItemElem->m_nItemNum = 1;
			}

			nTotalNum -= pItemElem->m_nItemNum;
			pItemElem->m_nHitPoint		= ( pItemProp->dwEndurance == -1 ) ? 0 : pItemProp->dwEndurance;//pItemProp->dwEndurance;

			pItemElem->SetAbilityOption( 0 );
			pItemElem->SetSerialNumber();
			CItem* pItem	= new CItem;

			pItem->m_pItemBase	= pItemElem;
			{
				pItem->m_idOwn			= m_nMoverID;
				pItem->m_dwDropTime		= timeGetTime();
			}
			pItem->m_bDropMob	= FALSE;
			if( pItem->m_pItemBase->m_dwItemId == 0 ) FLERROR_LOG( PROGRAM_NAME, _T( "SetIndex" ) );
			pItem->SetIndex( D3DDEVICE, pItem->m_pItemBase->m_dwItemId );
			
			D3DXVECTOR3	v3Pos = GetPos();

			FLOAT fRange = (float)xRandom(30)-15;

			fRange*=0.1f;

			v3Pos.x += fRange;
			v3Pos.z += fRange;
			pItem->SetPos( v3Pos );
			pItem->SetAngle( (float)( xRandom(360) ) );
			if( GetWorld()->ADDOBJ( pItem, TRUE, GetLayer() ) == FALSE )
			{
				SAFE_DELETE( pItem );
			}
		}
	}

	m_CtrlElem.m_dwMaxiItemNum = 0;
}

void CCommonCtrl::DropNPC()
{
	int	i = 0;
	int	j = 0;
	
	for( i=0; i<MAX_CTRLDROPMOB; i++ )
	{
		MoverProp* pMoverProp = prj.GetMoverProp( m_CtrlElem.m_dwMonResKind[i] );
		
		if( pMoverProp == NULL )
		{
			return;
		}

		// �Ϲ� �� ����
		DWORD dwBaseMob = m_CtrlElem.m_dwMonResNum[i]-m_CtrlElem.m_dwMonActAttack[i];

		for( j=0; j<(int)( dwBaseMob ); j++ )
		{
			CObj* pObj	= CreateObj( D3DDEVICE, OT_MOVER, pMoverProp->dwID );
			if( NULL == pObj )	
				return;

			D3DXVECTOR3	v3Pos = GetPos();
			v3Pos.y = GetWorld()->GetLandHeight( v3Pos );
			v3Pos.y+= 0.01f;
			
			FLOAT fRange = (float)xRandom(30)-15;
			
			fRange*=0.1f;
			
			v3Pos.x += fRange;
			v3Pos.z += fRange;
			
			pObj->SetPos( v3Pos );
			pObj->InitMotion( MTI_STAND );
			pObj->UpdateLocalMatrix();
			
			((CMover*)pObj)->m_bActiveAttack = FALSE;

			if( GetWorld()->ADDOBJ( pObj, TRUE, GetLayer() ) == FALSE )
			{
				SAFE_DELETE( pObj );
			}
		}

		// ���� �� ����
		for( j=0; j<(int)( m_CtrlElem.m_dwMonActAttack[i] ); j++ )
		{
			CObj* pObj	= CreateObj( D3DDEVICE, OT_MOVER, pMoverProp->dwID );
			if( NULL == pObj )	
				return;
			
			D3DXVECTOR3	v3Pos = GetPos();
			v3Pos.y = GetWorld()->GetLandHeight( v3Pos );
			v3Pos.y+= 0.01f;
			
			FLOAT fRange = (float)xRandom(30)-15;
			
			fRange*=0.1f;
			
			v3Pos.x += fRange;
			v3Pos.z += fRange;
			
			pObj->SetPos( v3Pos );
			pObj->InitMotion( MTI_STAND );
			pObj->UpdateLocalMatrix();
			
			((CMover*)pObj)->m_bActiveAttack = TRUE;
			if( GetWorld()->ADDOBJ( pObj, TRUE, GetLayer() ) == FALSE )
			{
				SAFE_DELETE( pObj );
			}
		}
	}
}

void CCommonCtrl::Init()
{
	m_bTrap    = FALSE;
	m_bAniPlay = FALSE;
	m_dwTrapProcessTime = 0xffffffff;
	
	m_bAlpha = FALSE;
	m_nBlendFactorLocal = 255;
	m_nMoverID = NULL_ID;

	m_pSkillProp = NULL;
	m_pAddSkillProp = NULL;
#ifdef __CLIENT
	m_pSfxModel = m_pSfxModel2 = NULL;
#endif 
	m_nCount = 0;
	m_nEvent = 0;
	m_idAttacker = NULL_ID;
	m_tmStart = timeGetTime();
	m_nLife = 0;
	m_fFrameCount = 0.0f;
	m_dwAniDelay = 0xffffffff;	
	m_bAniDelay  = FALSE;
	m_dwDelete = 0xffffffff;
	m_nExpBox  = 0;
	m_idExpPlayer = NULL_ID;
	m_bAction = FALSE;
	m_dwCtrlReadyTime = 0xffffffff;
	m_bControl	= FALSE;
}

#ifdef __WORLDSERVER

bool	CCommonCtrl::IsDynamicBoundBoxCtrl_HardCording()
{
	CtrlProp* pPropCtrl = GetProp();
	if( pPropCtrl == NULL )
	{
		return false;
	}

	if( m_bCollision == TRUE && /* m_bAnimate == TRUE && */ pPropCtrl->dwCtrlKind1 == CK1_DOOR && pPropCtrl->dwCtrlKind2 == CK2_KEEP )
	{
		return true;
	}

	return false;
}

void	CCommonCtrl::ForceSetBoundBoxHeight_HardCording()
{
	m_fOriginalOBBCenterHeight	= m_OBB.Center.y;

	m_OBB.Center.y				= m_pWorld->m_fMaxHeight;
}

void	CCommonCtrl::ForceRestoreBoundBoxHeight_HardCording()
{
	m_OBB.Center.y	= m_fOriginalOBBCenterHeight;
}

#endif // __WORLDSERVER
