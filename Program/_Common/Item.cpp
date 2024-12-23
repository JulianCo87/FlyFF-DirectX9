#include "stdafx.h"
#include "CreateObj.h"


//////////////////////////////////////////////////////////////////////
// CItem - �̰��� ���� ���� �� ������ �����ϴ� ������Ʈ �������̴�. 
//////////////////////////////////////////////////////////////////////
CItem::CItem()
{
	m_dwType = OT_ITEM;
	m_pItemBase = NULL;
	m_idHolder	= 0;
	m_idOwn = NULL_ID;
	m_dwDropTime = 0;
	m_bDropMob = 0;
	m_dwDropTime = timeGetTime();
#ifdef __CLIENT
	m_fGroundY = 0;
	m_vDelta.x = m_vDelta.y = m_vDelta.z = 0;
#endif //__CLIENT
#ifdef __EVENT_MONSTER
	m_IdEventMonster = NULL_ID;
#endif // __EVENT_MONSTER
}

void CItem::SetOwner( OBJID id )
{
	m_idOwn = id;	// �� �������� ������ pAttacker(����Ŀ)������ ǥ��.
	m_dwDropTime = ::timeGetTime();	// ��� ��������� �ð��� �����.
	m_bDropMob = TRUE;		// ���� �׾ ���� ���� ǥ�ø� �ص�.
}


CItem::~CItem()
{
	SAFE_DELETE( m_pItemBase );
	if( GetWorld() )
	{
#if !defined(__CLIENT)
#ifdef __LAYER_1021
		GetWorld()->m_respawner.Increment( GetRespawn(), m_nRespawnType, FALSE, GetLayer() );
#else	// __LAYER_1021
		GetWorld()->m_respawner.Increment( GetRespawn(), m_nRespawnType, FALSE );
#endif	// __LAYER_1021
#endif
	}
}

BOOL CItem::Read( CFileIO* pFile )
{
	CObj::Read( pFile );
	return TRUE;
}

#ifdef __CLIENT
// �������� ������ų�� �̰��� �ҷ���� �߷¿� ���� ��������.
void CItem::SetDelta( float fGroundY, D3DXVECTOR3 &vDelta )
{
	m_fGroundY = fGroundY;
	m_vDelta = vDelta;
}
#endif // __CLIENT

void CItem::Process()
{
	CCtrl::Process();
#ifdef __CLIENT
	AddAngle( 0.5f );
	D3DXVECTOR3 vPos = GetPos();

	//--- ��������� ������ ��ǥ�� ��ó���� �ؼ� �ȵȴ�. 
	if( m_fGroundY )	// �̰� 0�̸� �߷�ó���� �ؼ� �ȵȴ�.
	{
		if( vPos.y > m_fGroundY )		// ���߿� ���ִ���?
		{
			m_vDelta.y -= 0.0075f;		// �̵����Ϳ� �߷� ���� ����.
		} 
		else
		{
			vPos.y = m_fGroundY;		// �ٴڿ� ���� ���¸� ������ǥ�� �����ϰ� ����.
			m_vDelta.x = m_vDelta.y = m_vDelta.z = 0;	// �̵����ʹ� ������. Ƣ��� �Ϸ��� �̷��� �ϸ� �ȵȴ�.
			SetPos( vPos );				// ���� ��ǥ ����.
		}

		// �̵����Ͱ� ������ �������ʿ� ����.
		if( m_vDelta.x == 0 && m_vDelta.y == 0 && m_vDelta.z == 0 )
		{
			// ���� ��ǥ�� ����� �ʿ� ����.
		} 
		else
		{
			m_vDelta.x = m_vDelta.z = 0;		// ���⿡ �� ��������.
			vPos += m_vDelta;			// �̵� ���͸� ����.
			SetPos( vPos );
		}
	}
#endif // __CLIENT

#ifdef __WORLDSERVER
	if( g_eLocal.GetState( EVE_SCHOOL ) )
		return;
	if( (int)g_tmCurrent - (int)m_dwDropTime > MIN( 3 ) )
		Delete();
#endif	// __WORLDSERVER
}

#ifndef __WORLDSERVER
void CItem::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{

	PT_ITEM_SPEC pItemProp = GetProp();
	if( pItemProp && pItemProp->nReflect > 0 )
	{
		// ����!!! : m_pModel�� CModelObject��°��� �����ϰ� �Ѱ��̴�. �ƴ϶�� �̷��� ���� �ȵȴ�.
		((CModelObject*)m_pModel)->SetEffect( 0, XE_REFLECT );	
	}

	CObj::Render( pd3dDevice );
	if( xRandom( 50 ) == 1 )
		CreateSfx( pd3dDevice, XI_INDEX( 64, XI_GEN_ITEM_SHINE01 ), GetPos() );
}
#else
void CItem::Render( LPDIRECT3DDEVICE9 /*pd3dDevice*/ )
{
}
#endif

#ifndef __WORLDSERVER
void CItem::RenderName( LPDIRECT3DDEVICE9 pd3dDevice, CD3DFont* pFont, DWORD dwColor )
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
	vOut.x -= pFont->GetTextExtent( m_pItemBase->GetProp()->szName ).cx / 2;
	pFont->DrawText( vOut.x + 1, vOut.y + 1, 0xff000000, m_pItemBase->GetProp()->szName	);
	pFont->DrawText( vOut.x, vOut.y, dwColor, m_pItemBase->GetProp()->szName );
	return;
}
#else
void CItem::RenderName( LPDIRECT3DDEVICE9 /*pd3dDevice*/, CD3DFont* /*pFont*/, DWORD /*dwColor*/ )
{

}
#endif	// __WORLDSERVER

#ifdef __WORLDSERVER
MEMPOOLER_IMPLEMENT( CItem, 512 );
#else	// __WORLDSERVER
MEMPOOLER_IMPLEMENT( CItem, 128 );
#endif	// __WORLDSERVER
