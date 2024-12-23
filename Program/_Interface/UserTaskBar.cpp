// UserTaskBar.cpp: implementation of the CUserTaskBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserTaskBar.h"
#ifdef __WORLDSERVER
#include "../WorldServer/User.h"
#endif // __WORLDSERVER

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserTaskBar::CUserTaskBar()
: m_dwActionSlotUseTick( 0 ), m_bActionSlotNoti( false )
{
	InitTaskBar();
}

CUserTaskBar::~CUserTaskBar()
{

}

void CUserTaskBar::InitTaskBar()
{
	memset( m_aSlotApplet, 0, sizeof( m_aSlotApplet ) );
	memset( m_aSlotItem  , 0, sizeof( m_aSlotItem ) );
	memset( m_aSlotQueue , 0, sizeof( m_aSlotQueue ) );
	m_nActionPoint = 0;
	m_nUsedSkillQueue = SKILL_QUEUE_NONE;
}

void CUserTaskBar::InitTaskBarShorcutKind( DWORD dwShortcutKind )
{
	for( int i = 0; i < MAX_SLOT_APPLET; i++ )
	{
		if( m_aSlotApplet[i].m_dwShortcut == dwShortcutKind )		// ���� ����ִ��� �˻�
			memset( &m_aSlotApplet[i], 0, sizeof( m_aSlotApplet[i] ) );
	}

	for( int i = 0; i < MAX_SLOT_ITEM_COUNT; i++ )
	{
		for( int j = 0; j < MAX_SLOT_ITEM; j++ )
		{
			if( m_aSlotItem[i][j].m_dwShortcut == dwShortcutKind )		// ���� ����ִ��� �˻�
			{
				memset( &m_aSlotItem[i][j], 0, sizeof( m_aSlotItem[i][j] ) );
			}
		}
	}
	for( int i = 0; i < MAX_SLOT_QUEUE; i++ )
	{
		if( m_aSlotQueue[i].m_dwShortcut == dwShortcutKind )		// ���� ����ִ��� �˻�
		{
			memset( &m_aSlotQueue[i], 0, sizeof( m_aSlotQueue[i] ) );
		}
	}
}

void CUserTaskBar::Serialize( CAr &ar )
{
	if( ar.IsStoring() )
	{
		int nCount			= 0;
		ptrdiff_t nCountTag	= 0;

		if( _GetContentState( CT_NEWUI_19 ) == CS_VER1 )
		{
			nCount		= 0;
			nCountTag	= ar.ReserveSpace( sizeof( nCount ) );
			for( int i = 0; i < MAX_SLOT_APPLET; i++ )
			{
				if( m_aSlotApplet[i].m_dwShortcut != SHORTCUT_NONE )		// ���� ����ִ��� �˻�
				{
					ar << i;
					ar << m_aSlotApplet[i].m_dwShortcut <<	m_aSlotApplet[i].m_dwId << m_aSlotApplet[i].m_dwType;
					ar << m_aSlotApplet[i].m_dwIndex <<	m_aSlotApplet[i].m_dwUserId << m_aSlotApplet[i].m_dwData;
					if( m_aSlotApplet[i].m_dwShortcut == SHORTCUT_CHAT)
						ar.WriteString( m_aSlotApplet[i].m_szString );
					nCount++;
				}
			}
			ar.WriteReservedSpace( nCountTag, &nCount, sizeof( nCount ) );
		}

		nCount		= 0;
		nCountTag	= ar.ReserveSpace( sizeof( nCount ) );

		for( int i = 0; i < MAX_SLOT_ITEM_COUNT; i++ )
		{
			for( int j = 0; j < MAX_SLOT_ITEM; j++ )
			{
				if( m_aSlotItem[i][j].m_dwShortcut != SHORTCUT_NONE )		// ���� ����ִ��� �˻�
				{
					ar << i << j;
					ar << m_aSlotItem[i][j].m_dwShortcut <<	m_aSlotItem[i][j].m_dwId << m_aSlotItem[i][j].m_dwType;
					ar << m_aSlotItem[i][j].m_dwIndex << m_aSlotItem[i][j].m_dwUserId << m_aSlotItem[i][j].m_dwData;
					if( m_aSlotItem[i][j].m_dwShortcut == SHORTCUT_CHAT)
						ar.WriteString( m_aSlotItem[i][j].m_szString );
					nCount++;
					
				}

				// m_aSlotItem[i][j]�� ��ȿ�� �����̶��
//				ar << i << j << m_aSlotItem[i][j]�� �ʿ��� ���� ����
//				if( m_aSlotItem[i][j].m_dwType == SHORTCUT_CHAT )
//					ar.WriteString(
			}
		}
		ar.WriteReservedSpace( nCountTag, &nCount, sizeof( nCount ) );

		nCount		= 0;
		nCountTag	= ar.ReserveSpace( sizeof( nCount ) );
		for( int i = 0; i < MAX_SLOT_QUEUE; i++ )
		{
			if( m_aSlotQueue[i].m_dwShortcut != SHORTCUT_NONE )		// ���� ����ִ��� �˻�
			{
				ar << i;
				ar << m_aSlotQueue[i].m_dwShortcut << m_aSlotQueue[i].m_dwId << m_aSlotQueue[i].m_dwType;
				ar << m_aSlotQueue[i].m_dwIndex << m_aSlotQueue[i].m_dwUserId << m_aSlotQueue[i].m_dwData;
				nCount++;
			}
			
							// m_aSlotQueue[i]�� ��ȿ�� �����̶��
//				ar << i << m_aSlotQueue[i]�� �ʿ��� ���� ����
		}
		ar << m_nActionPoint;
		ar.WriteReservedSpace( nCountTag, &nCount, sizeof( nCount ) );
	}
	else
	{
		memset( m_aSlotApplet, 0, sizeof(m_aSlotApplet) );
		memset( m_aSlotItem, 0, sizeof(m_aSlotItem) );
		memset( m_aSlotQueue, 0, sizeof(m_aSlotQueue) );

		int nCount = 0;

		if( _GetContentState( CT_NEWUI_19 ) == CS_VER1 )
		{
			ar >> nCount;	// applet count
			for( int i = 0; i < nCount; i++ )
			{
				int nIndex = 0;

				ar >> nIndex;
				ar >> m_aSlotApplet[nIndex].m_dwShortcut >>	m_aSlotApplet[nIndex].m_dwId >> m_aSlotApplet[nIndex].m_dwType;
				ar >> m_aSlotApplet[nIndex].m_dwIndex >> m_aSlotApplet[nIndex].m_dwUserId >> m_aSlotApplet[nIndex].m_dwData;
				if( m_aSlotApplet[nIndex].m_dwShortcut == SHORTCUT_CHAT)
					ar.ReadString( m_aSlotApplet[nIndex].m_szString, _countof( m_aSlotApplet[nIndex].m_szString ) );
				//			ar >>  m_aSlotApplet[nIndex]�� �Ʊ� ���� �ʿ��� ������ �ִ´�.
			}
		}

		ar >> nCount;	// slot item count
		
		for( int i = 0; i < nCount; i++ )
		{
			int nIndex	= 0;
			int nIndex2 = 0;

			ar >> nIndex >> nIndex2;	// index
			ar >> m_aSlotItem[nIndex][nIndex2].m_dwShortcut >>	m_aSlotItem[nIndex][nIndex2].m_dwId >> m_aSlotItem[nIndex][nIndex2].m_dwType;
			ar >> m_aSlotItem[nIndex][nIndex2].m_dwIndex >> m_aSlotItem[nIndex][nIndex2].m_dwUserId >> m_aSlotItem[nIndex][nIndex2].m_dwData;
			if( m_aSlotItem[nIndex][nIndex2].m_dwShortcut == SHORTCUT_CHAT)
				ar.ReadString( m_aSlotItem[nIndex][nIndex2].m_szString, _countof( m_aSlotItem[nIndex][nIndex2].m_szString ) );
		}

		ar >> nCount;
		for( int i = 0; i < nCount; i++ )
		{
			int nIndex	= 0;

			ar >> nIndex;
			ar >> m_aSlotQueue[nIndex].m_dwShortcut >> m_aSlotQueue[nIndex].m_dwId >> m_aSlotQueue[nIndex].m_dwType;
			ar >> m_aSlotQueue[nIndex].m_dwIndex >> m_aSlotQueue[nIndex].m_dwUserId >> m_aSlotQueue[nIndex].m_dwData;
//			ar >> m_aSlotQueue[nIndex]�� ������ �ִ´�.
		}
		ar >> m_nActionPoint;
	}
}

void CUserTaskBar::SetShortcut( int nIndex, DWORD dwShortcut, DWORD dwType, DWORD dwIndex, DWORD dwId, DWORD /*dwData*/, int nWhere )
{
	LPSHORTCUT pShortcut;
	if( nWhere == 0 )
		pShortcut = &m_aSlotApplet[ nIndex ];
	else if( nWhere == 1 )
		pShortcut = &m_aSlotItem[0][ nIndex ];
	else
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "nWhere:%d" ), nWhere ); 
		return;
	}

//	LPSHORTCUT pShortcut	= &m_aSlotApplet[ nIndex ];
	pShortcut->m_dwShortcut		= dwShortcut   ;
	pShortcut->m_dwType     = dwType;
	pShortcut->m_dwIndex    = dwIndex;
	pShortcut->m_dwId       = dwId;
	pShortcut->m_dwUserId   = 0 ;
	pShortcut->m_dwData     = nWhere;
}

#ifdef __WORLDSERVER
// ��ų�� ����� ������ ��������
void CUserTaskBar::OnEndSkillQueue( FLWSUser *pUser )
{
	m_nUsedSkillQueue = SKILL_QUEUE_NONE;
	pUser->AddHdr( GETID( pUser ), SNAPSHOTTYPE_ENDSKILLQUEUE );
	((CMover*)pUser)->ClearCmd();
	FLTRACE_LOG( PROGRAM_NAME, _T( "OnEndSkillQueue" ) );
}

bool	CUserTaskBar::SetNextSkill( FLWSUser *pUser )
{
	++m_nUsedSkillQueue;

	if( m_nUsedSkillQueue < 0 || m_nUsedSkillQueue >= MAX_SLOT_QUEUE ) {
		OnEndSkillQueue( pUser );
		return false;
	}

	if( _GetContentState( CT_NEWUI_19 ) == CS_VER1 ) {
		int nAP = m_nActionPoint;

		if( pUser->IsSMMode( SM_ACTPOINT ) == FALSE && pUser->IsSMMode( SM_ACTPOINT_30M ) == FALSE ) {
			switch( m_nUsedSkillQueue ) {
			case 1:	nAP -= 6;	break;		// �׼� ����Ʈ �Ҹ�. ���������� ����Ҽ��ִ��� �̸� �˾ƺ��� ����.
			case 2:	nAP -= 8;	break;
			case 3:	nAP -= 11;	break;
			case 4:	nAP -= 30;	break;
			}
		}

		LPSHORTCUT pShortcut = &m_aSlotQueue[ m_nUsedSkillQueue ];
		//	BOOL bResult = nAP < 0;
		if( pShortcut->IsEmpty() || nAP < 0 ) {	// ť������ ������ ���ų� || ť�� ����ų� || AP�� ���ų�.
			OnEndSkillQueue( pUser );
			return false;		// ť���� ��.
		} else {
			if( nAP < 0 )	nAP = 0;
			m_nActionPoint = nAP;
			pUser->AddSetActionPoint( nAP );		// �׼�����Ʈ Ŭ�� ����.
			OBJID idTarget = pUser->m_idSetTarget;
			FLTRACE_LOG( PROGRAM_NAME, _T( "������ų��� �õ�%d, " ), pShortcut->m_dwId );
			if( pUser->CMD_SetUseSkill( idTarget, pShortcut->m_dwId, SUT_QUEUEING ) == 0 ) {		// ������ ����� ����. �̵� + ��ų����� ������ ���.
				FLTRACE_LOG( PROGRAM_NAME, _T( "������ų��� ���� %d, " ), pShortcut->m_dwId );
				return SetNextSkill( pUser );	// ��ų��뿡 �����ߴٸ� ���� ��ų ����ϵ��� �Ѿ.
			}
			return true;
		}
	}
	else {
		LPSHORTCUT pShortcut = &m_aSlotQueue[ m_nUsedSkillQueue ];
		
		if( pShortcut->IsEmpty() ) {	// ť������ ������ ���ų� || ť�� ����ų�
			OnEndSkillQueue( pUser );
			return false;		// ť���� ��.
		}
		else {
			if( pUser->GetReuseDelay( pShortcut->m_dwId ) <= 0 ) {
				if( pUser->CMD_SetUseSkill( pUser->m_idSetTarget, pShortcut->m_dwId, SUT_QUEUEING ) == 1 ) {		// ������ ����� ����. �̵� + ��ų����� ������ ���.
					return true;
				}
				else {
					pUser->AddActionSlotSkillSkip_HardCording( SUT_QUEUEING );
					return SetNextSkill( pUser );	// ��ų��뿡 �����ߴٸ� ���� ��ų ����ϵ��� �Ѿ.
				}
			}
			else {
				pUser->AddActionSlotSkillSkip_HardCording( SUT_QUEUEING );
				return SetNextSkill( pUser );	// ��ų ��Ÿ���� ���������Ƿ� ����ĭ���� �н�
			}
		}
	}
}

bool	CUserTaskBar::ExistCanUseSkillByActionSlot( FLWSUser* pUser ) const
{
	bool bExistCanUseSkill	= false;

	for( DWORD Nth = 0; Nth < MAX_SLOT_QUEUE; ++Nth )
	{
		const SHORTCUT* pShortCut = &m_aSlotQueue[ Nth ];
		if( pShortCut == NULL || pShortCut->IsEmpty() == TRUE )
		{
			continue;
		}

		if( pUser->GetReuseDelay( pShortCut->m_dwId ) <= 0 )
		{
			bExistCanUseSkill	= true;
		}
	}

	return bExistCanUseSkill;
}

bool	CUserTaskBar::SkipActionSlotSkill_HardCording( FLWSUser* pUser )
{
	++m_nUsedSkillQueue;

	if( m_nUsedSkillQueue < 0 || m_nUsedSkillQueue >= MAX_SLOT_QUEUE )
	{
		OnEndSkillQueue( pUser );
		return false;
	}

	const SHORTCUT* pShortCut = &m_aSlotQueue[ m_nUsedSkillQueue ];
	if( pShortCut == NULL || pShortCut->IsEmpty() == TRUE )
	{
		OnEndSkillQueue( pUser );
		return false;
	}

	return true;
}

DWORD	CUserTaskBar::GetNextActionSlotSkillIndex_HardCording( FLWSUser* pUser )
{
	if( IsValidObj( pUser ) == FALSE )
	{
		return NULL_ID;
	}

	if( SkipActionSlotSkill_HardCording( pUser ) == false )
	{
		return NULL_ID;
	}

	pUser->AddActionSlotSkillSkip_HardCording( SUT_QUEUEING );

	const SHORTCUT* pShortCut = &m_aSlotQueue[ m_nUsedSkillQueue ];
	if( pShortCut == NULL || pShortCut->IsEmpty() == TRUE )
	{
		return NULL_ID;
	}

	if( pUser->GetReuseDelay( pShortCut->m_dwId ) > 0 )
	{
		return GetNextActionSlotSkillIndex_HardCording( pUser );
	}
	else
	{
		return pShortCut->m_dwId;
	}
}

void	CUserTaskBar::SetUseSkillQueue()
{
	m_bActionSlotNoti		= true;
	m_dwActionSlotUseTick	= g_tmCurrent;
}

bool	CUserTaskBar::CanUseSkillQueue( FLWSUser* pUser ) const
{
	if( pUser->IsSMMode( SM_ACTPOINT ) == TRUE || pUser->IsSMMode( SM_ACTPOINT_30M ) == TRUE ) {
		return true;
	}
	else if( g_tmCurrent - m_dwActionSlotUseTick >= SEC( ACTIONSLOT_DEFAULT_COOLTIME ) ) {
		return true;
	}

	return false;
}

void	CUserTaskBar::Process( FLWSUser* pUser )
{
	if( m_bActionSlotNoti && !IsExecutionSkillQueue() && CanUseSkillQueue( pUser ) ) {
		m_bActionSlotNoti	= false;
		pUser->AddHdr( pUser->GetId(), SNAPSHOTTYPE_USE_ACTION_SLOT_NOTI );
	}
}
#endif // __WORLDSERVER
