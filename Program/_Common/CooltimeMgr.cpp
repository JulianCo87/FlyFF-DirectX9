#include "stdafx.h"
#include "misc.h"
#include "CooltimeMgr.h"
#include "ProjectCmn.h"

CCooltimeMgr::CCooltimeMgr()
{
	ZeroMemory( m_times, sizeof(m_times) );
	ZeroMemory( m_bases, sizeof(m_bases) );
}
	
CCooltimeMgr::~CCooltimeMgr()
{
}

// ��Ÿ�� �������� ��Ÿ�� �׷��ȣ�� ��´�. 
// ���� �׷��� �������� �ѹ� ����ϸ� ��� ���� ��Ÿ���� ����޴´�.
// ����: �����Ϸ��� ������Ͽ� MAX_COOLTIME_TYPE�� �� ������ �� 
DWORD CCooltimeMgr::GetGroup( PT_ITEM_SPEC pItemProp )
{
	DWORD dwCooltimeItemGroup = 0;

	if( pItemProp->dwSkillReady > 0 )
	{
		switch ( pItemProp->dwItemKind2 )
		{
		case IK2_FOOD:
			dwCooltimeItemGroup = 1;
			if( pItemProp->dwItemKind3 == IK3_PILL )
			{
				dwCooltimeItemGroup = 2;
			}
			break;

		case IK2_SKILL:
			dwCooltimeItemGroup = 3;
			break;

//		case IK2_POTION:
//			dwCooltimeItemGroup = 3;
//			break;

		default:
			{
				if( pItemProp->dwID == ITEM_INDEX( 23957, II_GEN_POT_ARTIFACT_NOSFERATU ) )
				{
					dwCooltimeItemGroup	= 4;
				}
				else if( pItemProp->dwID == ITEM_INDEX( 23958, II_GEN_POT_ARTIFACT_SUFFERENCE ) )
				{
					dwCooltimeItemGroup	= 5;
				}
				else if( pItemProp->dwID == ITEM_INDEX( 23959, II_GEN_POT_ARTIFACT_VANGUARD ) )
				{
					dwCooltimeItemGroup	= 6;
				}
				else if( pItemProp->dwID == ITEM_INDEX( 23960, II_GEN_POT_ARTIFACT_VINDICATION ) )
				{
					dwCooltimeItemGroup	= 7;
				}
				else if( pItemProp->dwID == ITEM_INDEX( 23961, II_GEN_POT_ARTIFACT_LUMINESCENSE ) )
				{
					dwCooltimeItemGroup	= 8;
				}
				else if( pItemProp->dwID == ITEM_INDEX( 23962, II_GEN_POT_ARTIFACT_MISTRAL ) )
				{
					dwCooltimeItemGroup	= 9;
				}
				else if( pItemProp->dwID == ITEM_INDEX( 23963, II_GEN_POT_ARTIFACT_DECIMATION ) )
				{
					dwCooltimeItemGroup	= 10;
				}
				else if( pItemProp->dwID == ITEM_INDEX( 23964, II_GEN_POT_ARTIFACT_MALEVOLENCE ) )
				{
					dwCooltimeItemGroup	= 11;
				}
				else if( pItemProp->dwID == ITEM_INDEX( 23965, II_GEN_POT_ARTIFACT_VIGILENCE ) )
				{
					dwCooltimeItemGroup	= 12;
				}
				else if( pItemProp->dwID == ITEM_INDEX( 23966, II_GEN_POT_ARTIFACT_BELIGERENCE ) )
				{
					dwCooltimeItemGroup	= 13;
				}
				else if( pItemProp->dwID == ITEM_INDEX( 23967, II_GEN_POT_ARTIFACT_CLARITY ) )
				{
					dwCooltimeItemGroup	= 14;
				}
				else if( pItemProp->dwID == ITEM_INDEX( 23968, II_GEN_POT_ARTIFACT_BRAWN ) )
				{
					dwCooltimeItemGroup	= 15;
				}
				else if( pItemProp->dwID == ITEM_INDEX( 23969, II_GEN_POT_ARTIFACT_EXPERTISE ) )
				{
					dwCooltimeItemGroup	= 16;
				}
				else if( pItemProp->dwID == ITEM_INDEX( 23970, II_GEN_POT_ARTIFACT_PERCEPTION ) )
				{
					dwCooltimeItemGroup	= 17;
				}
			}
			break;
		}

	}

	return dwCooltimeItemGroup <= MAX_COOLTIME_TYPE ? dwCooltimeItemGroup : 0;
}


// dwGroup�� ����� �� �ִ°�?
BOOL CCooltimeMgr::CanUse( DWORD dwGroup )
{
	FLASSERT( dwGroup > 0 );
	return g_tmCurrent > GetTime( dwGroup );		// ���� �ð��� ������ ��밡�� 
}

// ����� �ð��� ����ؼ�, CanUse���� �Ǵ� �ٰŷ� ����Ѵ�.
void CCooltimeMgr::SetTime( DWORD dwGroup, DWORD dwCoolTime )
{
	FLASSERT( dwGroup > 0 && dwCoolTime > 0 );
	DWORD dwTick = g_tmCurrent;

	m_times[dwGroup - 1] = dwTick + dwCoolTime;
	m_bases[dwGroup - 1] = dwTick;
}	


