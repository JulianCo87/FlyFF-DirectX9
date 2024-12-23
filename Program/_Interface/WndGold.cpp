
#include "StdAfx.h"

#include "WndGold.h"

//---------------------------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------------------------
CWndGold::CWndGold()
:	m_pTexture( NULL )
{
}


//---------------------------------------------------------------------------------------------
// �ı���
//---------------------------------------------------------------------------------------------
CWndGold::~CWndGold()
{
}


//---------------------------------------------------------------------------------------------
// ������Ʈ �ʱ�ȭ
// param	: ����
// return	: void
//---------------------------------------------------------------------------------------------
void CWndGold::OnInitialUpdate()
{
	m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, _T( "itm_GolGolSeed.dds" ) ), WNDCOLOR_DEFAULT_KEY );
}


//---------------------------------------------------------------------------------------------
// ���콺 ���� ��ư ������ ��
// param	: �÷���, ���콺 ��ġ
// return	: void
//---------------------------------------------------------------------------------------------
void CWndGold::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_GlobalShortcut.Empty();
	m_GlobalShortcut.m_dwShortcut	= SHORTCUT_ITEM;
	m_GlobalShortcut.m_pFromWnd		= this;
	m_GlobalShortcut.m_pTexture		= m_pTexture; 
	m_GlobalShortcut.m_dwData		= NULL;
}

