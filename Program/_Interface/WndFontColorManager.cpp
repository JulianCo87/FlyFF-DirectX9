

#include "StdAfx.h"
#include "WndFontColorManager.h"


//---------------------------------------------------------------------------------------------
// ������ 
//---------------------------------------------------------------------------------------------
CWndFontColorManager::CWndFontColorManager()
{
}


//---------------------------------------------------------------------------------------------
// �ı��� 
//---------------------------------------------------------------------------------------------
CWndFontColorManager::~CWndFontColorManager()
{
}


//---------------------------------------------------------------------------------------------
// Get Instance  ( ��ü ��� )
// param	: void
// return	: CWndFontColorManager* 
//---------------------------------------------------------------------------------------------
CWndFontColorManager* CWndFontColorManager::GetInstance()
{
	static CWndFontColorManager WndFontColorManager;
	return &WndFontColorManager;
}

