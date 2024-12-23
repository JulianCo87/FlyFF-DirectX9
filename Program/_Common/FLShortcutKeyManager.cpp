

#include "StdAfx.h"
#include "FLShortcutKeyManager.h"

//---------------------------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------------------------
FLShortcutKeyManager::FLShortcutKeyManager()
{
	SetShortcutKey_French();
}


//---------------------------------------------------------------------------------------------
// �ı���
//---------------------------------------------------------------------------------------------
FLShortcutKeyManager::~FLShortcutKeyManager()
{
}


//---------------------------------------------------------------------------------------------
// �ν��Ͻ� ���
//---------------------------------------------------------------------------------------------
FLShortcutKeyManager* FLShortcutKeyManager::GetInstance()
{
	static	FLShortcutKeyManager xShortcutKeyManager;
	return	&xShortcutKeyManager;
}


//---------------------------------------------------------------------------------------------
// ����Ű ���� (������)
//---------------------------------------------------------------------------------------------
void FLShortcutKeyManager::SetShortcutKey_French()
{
	if( g_xFlyffConfig->GetMainLanguage() == LANG_FRE )
	{
		m_stShortcutKey_Control.KeyUp			= 'Z';
		m_stShortcutKey_Control.KeyLeft			= 'Q';
		m_stShortcutKey_Control.KeyWalk			= 'W';
		m_stShortcutKey_Control.KeyTrace		= 'F';	

		m_stShortcutKey_UI.KeyQuest				= 'L';
		m_stShortcutKey_UI.KeyCouple			= 'X';
		m_stShortcutKey_UI.KeyLordSkill			= 'A';
	}
}

