#include "StdAfx.h"



FLApplyItemEffect::FLApplyItemEffect()
{

}

FLApplyItemEffect::~FLApplyItemEffect()
{

}

FLApplyItemEffect*	FLApplyItemEffect::GetInstance()
{
	static FLApplyItemEffect xApplyItemEffect;
	
	return & xApplyItemEffect;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BOOL	FLApplyItemEffect::ApplyParameter( CMover* pSrc, CMover* pDest, PT_ITEM_SPEC pItemProp, BOOL bSend, int nDamage )
{
	if( pSrc == NULL )		return FALSE;

	int		i;

	DWORD	dwDestParam, dwSkillTime, dwPainTime;
	int		nAdjParam, nChgParam;
	int		nDestData[3];	// TODO : ????????????

	//sun: 9, ���ο� AddSkillProp �÷� �߰� �� ����
	BOOL bPVP	= pSrc->GetType() == OT_MOVER && pSrc->IsPlayer() && pDest->IsPlayer();

	for( i = 0; i < MAX_ITEM_PARAM; i ++ )
	{
		dwDestParam = pItemProp->dwDestParam[i];
		dwSkillTime = pItemProp->dwSkillTime;
		dwPainTime  = pItemProp->tmContinuousPain;
		nAdjParam	= pItemProp->nAdjParamVal[i];
		nChgParam	= (int)pItemProp->dwChgParamVal[i];
		nDestData[0] = pItemProp->nDestData1[0];
		nDestData[1] = pItemProp->nDestData1[1];
		nDestData[2] = pItemProp->nDestData1[2];

		// destParam 1
		switch( dwDestParam )
		{
		case NULL_ID:
			break;

		case DST_CHRSTATE:		// ���� ����
			if( nAdjParam & CHS_INVISIBILITY )	// ��ũ�Ϸ�ǻ��·�...
			{
				if( pDest->SetDarkCover( TRUE, dwSkillTime ) == FALSE )
				{
					return FALSE;
				}
				pDest->m_wDarkCover = 0;		
			}
			else if( nAdjParam & CHS_STUN )	// ���ϻ��·�...
			{
				if( pDest->SetStun( TRUE, dwSkillTime ) == FALSE )
				{
					return FALSE;
				}
				pDest->m_wStunCnt = 0;		
			}
			else if( nAdjParam & CHS_POISON )	// �����·�..
			{
				if( pDest->SetPoison( TRUE, pSrc->GetId(), dwSkillTime, dwPainTime, nDestData[0] ) == FALSE )	// nDestData[0] ƽ�� ���� ������
				{
					return FALSE;
				}
				pDest->m_wPoisonCnt = 0;		// �����̶� ���� ����
			}
			else if( nAdjParam & CHS_DARK )	// �������
			{
				if( pDest->SetDark( TRUE, dwSkillTime, nDestData[1] ) == FALSE )		// nDestData1[1] : ���߷� ����ġ.
				{
					return FALSE;
				}
				pDest->m_wDarkCnt = 0;		// �����̶� ���� ����
			}
			else if( nAdjParam & CHS_BLEEDING )	// �������·�..
			{
				if( nDestData[0] < 0 )
				{
					nDestData[0]	= nDamage / 10;
				}
				if( pDest->SetBleeding( TRUE, pSrc->GetId(), dwSkillTime, dwPainTime, nDestData[0] ) == FALSE )
				{
					return FALSE;
				}
				pDest->m_wBleedingCnt = 0;		// �����̶� ���� ����
			}
			else if( nAdjParam & CHS_SLEEPING )	// �����λ��·�...
			{
				if( pDest->SetSleeping( true, dwSkillTime ) == false )
				{
					return FALSE;
				}
				pDest->SetSleepingDuration( 0 );
			}
			else
			{
				pDest->SetDestParam( dwDestParam, nAdjParam, NULL_CHGPARAM, TRUE );
			}
			break;

		case DST_CHR_STEALHP: 			// HP���.  ������ DST_�� �ϱⰡ �� ������.
			{
				int nStealHP = (int)( nDamage * (nAdjParam / 100.0f) );	// ���������� ���������� ����Ҿ��� ����.
				if( pSrc->GetType() == OT_MOVER )
				{
					pSrc->SetDestParam( DST_HP, nStealHP, NULL_CHGPARAM );		// ������ HP�� ���.
				}
			}
			break;

		case DST_CHR_STEALHP_IMM: 			// �����Ⱦ��� HP���.  
			{
				if( pDest->GetType() == OT_MOVER )
				{
					int nHP = pDest->GetHitPoint();		// ������� hp
					int nStealHP = nAdjParam;	// ����ؾ��� hp
					if( nHP < nStealHP )	// �Ǹ� �̾ƿ;��ϴµ� �����ǰ� �� ������.
					{
						nStealHP = nHP;		// �����Ǹ�ŭ �̾ƿ�
					}

					if( pSrc->GetType() == OT_MOVER )
					{
						pDest->m_pActMover->SendDamage( AF_FORCE,	((CMover*)pSrc)->GetId(), nStealHP );
						pSrc->SetDestParam( DST_HP, nStealHP, NULL_CHGPARAM, TRUE  );		// ������ HP�� ���.
					}
				}
			}
			break;

		case DST_CURECHR:		// ���� ����
			pDest->RemoveDebuff( nAdjParam );
			break;

		case DST_CLEARBUFF:		// ����� ������ �ı�.
			if( nAdjParam == NULL_ID )
			{
				pDest->RemoveBuffAll();
			}
			else
			{
				for( int i = 0; i < nAdjParam; i ++ )
					pDest->RemoveBuffOne();
			}
			break;

		case DST_CHR_CHANCESTUN:	// adj:Ȯ��		data1:��ų
		case DST_CHR_CHANCEPOISON:	// adj:Ȯ��		data1:��ų
		case DST_CHR_CHANCEDARK:	// adj:Ȯ��		data1:��ų
		case DST_CHR_CHANCEBLEEDING:// adj:Ȯ��		data1:��ų
		case DST_CHR_CHANCESTEALHP:	// adj:Ȯ��		data1:��ų
			pDest->SetDestParam( dwDestParam, nAdjParam, nChgParam, TRUE );
			break;

		default: // �׿� DST_�� �Ϲ����� ��ƾ�� ź��.
			// �Ϲ� DST_ó��
			pDest->SetDestParam( dwDestParam, nAdjParam, nChgParam, bSend );
			break;
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

