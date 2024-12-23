

#include "StdAfx.h"
#include "TextureManager.h"
#include "targa.h"


//---------------------------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------------------------
CTextureMng::CTextureMng()
:	m_pDefaultTexture( NULL )
{

}


//---------------------------------------------------------------------------------------------
// �ı���
//---------------------------------------------------------------------------------------------
CTextureMng::~CTextureMng()
{
	DeleteDeviceObjects();
	m_vecLoadFailedFileName.clear();
}


//---------------------------------------------------------------------------------------------
// ��ȿȭ
// param	: void
// return	: void 
//---------------------------------------------------------------------------------------------
void CTextureMng::Invalidate()
{
	for( MapTexItor i = m_mapTexture.begin(); i != m_mapTexture.end(); ++i )
	{
		CTexture* pTexture = ((*i).second);
		FLERROR_ASSERT_LOG_DO( pTexture, continue, _T("[�ؽ��� �޴���] �ؽ��� �����Ͱ� ��ȿ���� �ʽ��ϴ� : [%s]"), (*i).first );

		pTexture->Invalidate();
	}
}


//---------------------------------------------------------------------------------------------
// ��ġ ������Ʈ ���� ( ����̽��� �������� �ؽ��� ���� )
// param	: void
// return	: BOOL 
//---------------------------------------------------------------------------------------------
BOOL CTextureMng::DeleteDeviceObjects()
{
	if( m_mapTexture.empty() )
		return TRUE;

	for( MapTexItor i = m_mapTexture.begin(); i != m_mapTexture.end(); ++i )
	{
		CTexture* pTexture = ((*i).second);
		FLERROR_ASSERT_LOG_DO( pTexture, continue, _T("[�ؽ��� �޴���] �ؽ��� �����Ͱ� ��ȿ���� �ʽ��ϴ� : [%s]"), (*i).first );

		SAFE_DELETE( pTexture );
		(*i).second = NULL;
	}

	m_mapTexture.clear();

	return TRUE;
}


//---------------------------------------------------------------------------------------------
// �⺻ �ؽ��� �ҷ�����
// param	: ...
// return	: CTexture* 
//---------------------------------------------------------------------------------------------
BOOL CTextureMng::LoadDefaultTexture()
{
	if( m_pDefaultTexture )
		return TRUE;

	string strFullPath( _T("") );

	strFullPath += DIR_THEME;
	strFullPath += DIR_DEFAULT;
	strFullPath += _T("HyperlinkUserMark.bmp");

	m_pDefaultTexture = new CTexture();
	FLERROR_ASSERT_LOG_RETURN( m_pDefaultTexture, FALSE, _T("[�ؽ��� �޴���] �⺻ �ؽ��� �Ҵ� ����.") );

	if( m_pDefaultTexture->LoadTexture( g_Neuz.m_pd3dDevice, strFullPath.c_str(), 0XFF000000, FALSE ) )
	{
		bool bResult( m_mapTexture.insert( MapTexType( strFullPath.c_str(), m_pDefaultTexture ) ).second );

		if( bResult == true )
			return TRUE;
		else
		{
			SAFE_DELETE( m_pDefaultTexture );
			FLERROR_ASSERT_LOG_RETURN( m_pDefaultTexture, FALSE, _T("[�ؽ��� �޴���] �ؽ��� ���Կ� �����Ͽ����ϴ� : [%s]"), strFullPath.c_str() );
		}
	}
	else
	{
		SAFE_DELETE( m_pDefaultTexture );
		m_vecLoadFailedFileName.push_back( strFullPath );
		FLERROR_ASSERT_LOG_RETURN( m_pDefaultTexture, FALSE, _T("[�ؽ��� �޴���] �ؽ��� �������⸦ �����Ͽ����ϴ� : [%s]"), strFullPath.c_str() );
	}

	return TRUE;
}


//---------------------------------------------------------------------------------------------
// �ε��� ������ ���� �ִ� �ؽ��� �ΰ� ?
// param	: ...
// return	: CTexture* 
//---------------------------------------------------------------------------------------------
BOOL CTextureMng::IsLoadFailedTexture( LPCTSTR pFileName )
{
	if( m_vecLoadFailedFileName.empty() )
		return FALSE;

	for( UINT nIndex = 0; nIndex < m_vecLoadFailedFileName.size(); nIndex++ )
	{
		if( m_vecLoadFailedFileName[nIndex] == pFileName )
			return TRUE;
	}

	return FALSE;
}


//---------------------------------------------------------------------------------------------
// �ؽ��� �߰�
// param	: ...
// return	: CTexture* 
//---------------------------------------------------------------------------------------------
CTexture* CTextureMng::AddTexture( LPDIRECT3DDEVICE9 pd3dDevice, LPCTSTR pFileName, D3DCOLOR d3dKeyColor, BOOL bMyLoader )
{
	CTexture* pTexture( NULL );

	LoadDefaultTexture();
	
	MapTexItor mapTexItor = m_mapTexture.find( pFileName );
	
	if( mapTexItor != m_mapTexture.end() )
	{
		pTexture = (*mapTexItor).second;
		FLERROR_ASSERT_LOG_RETURN( pTexture, m_pDefaultTexture, _T("[�ؽ��� �޴���] �ؽ��ĸ� ã������ �����Ͱ� ��ȿ���� �ʽ��ϴ� : [%s]"), pFileName );
		
		return pTexture;
	}
	 
	if( IsLoadFailedTexture( pFileName ) )
		return NULL;

	pTexture = new CTexture();
	FLERROR_ASSERT_LOG_RETURN( pTexture, m_pDefaultTexture, _T("[�ؽ��� �޴���] �ؽ��� �Ҵ� ����.") );

	if( pTexture->LoadTexture( pd3dDevice, pFileName, d3dKeyColor, bMyLoader ) )
	{
		bool bResult( m_mapTexture.insert( MapTexType( pFileName, pTexture ) ).second );

		if( bResult == true )
			return pTexture;
		else
		{
			SAFE_DELETE( pTexture );
			FLERROR_ASSERT_LOG_RETURN( pTexture, m_pDefaultTexture, _T("[�ؽ��� �޴���] �ؽ��� ���Կ� �����Ͽ����ϴ� : [%s]"), pFileName );
		}
	}
	else
	{
		SAFE_DELETE( pTexture );
		m_vecLoadFailedFileName.push_back( pFileName );
		FLERROR_ASSERT_LOG_RETURN( pTexture, m_pDefaultTexture, _T("[�ؽ��� �޴���] �ؽ��� �������⸦ �����Ͽ����ϴ� : [%s]"), pFileName );
	}

	return NULL;
}


//---------------------------------------------------------------------------------------------
// �ؽ��� �߰�
// param	: ...
// return	: CTexture* 
//---------------------------------------------------------------------------------------------
CTexture* CTextureMng::AddTexture( LPDIRECT3DDEVICE9 pd3dDevice, LPCTSTR pKey, CTexture* pTexture )
{
	MapTexItor mapTexItor = m_mapTexture.find( pKey );
	if( mapTexItor != m_mapTexture.end() )
		return (*mapTexItor).second;

	m_mapTexture.insert( MapTexType( pKey, pTexture ) );

	return pTexture;
}


//---------------------------------------------------------------------------------------------
// �ؽ��� ����
// param	: Ű ���ڿ�
// return	: BOOL 
//---------------------------------------------------------------------------------------------
BOOL CTextureMng::RemoveTexture( LPCTSTR pKey )
{
	if( m_mapTexture.empty() )
		return TRUE;

	MapTexItor mapTexItor = m_mapTexture.find( pKey );

	if( mapTexItor != m_mapTexture.end() )
	{
		SAFE_DELETE( (*mapTexItor).second );
		m_mapTexture.erase( pKey );
	}

	return TRUE;
}


//---------------------------------------------------------------------------------------------
// ��ȿȭ ����
// param	: �׷��� ��ġ 
// return	: BOOL 
//---------------------------------------------------------------------------------------------
BOOL CTextureMng::SetInvalidate(LPDIRECT3DDEVICE9 pd3dDevice)
{
	for( MapTexItor i = m_mapTexture.begin(); i != m_mapTexture.end(); ++i )
	{
		CTexture* pTexture = ((*i).second);
		FLERROR_ASSERT_LOG_DO( pTexture, continue, _T("[�ؽ��� �޴���] �ؽ��� �����Ͱ� ��ȿ���� �ʽ��ϴ� : [%s]"), (*i).first );

		pTexture->SetInvalidate(pd3dDevice);
	}

	return TRUE;
}


//---------------------------------------------------------------------------------------------
// �ؽ��� ���
// param	: ���� �̸�
// return	: CTexture*
//---------------------------------------------------------------------------------------------
CTexture* CTextureMng::GetAt( LPCTSTR pFileName )
{
	MapTexItor mapTexItor = m_mapTexture.find( pFileName );

	if( mapTexItor != m_mapTexture.end() )
	{
		CTexture* pTexture = (*mapTexItor).second;
		FLERROR_ASSERT_LOG_RETURN( pTexture, m_pDefaultTexture, _T("[�ؽ��� �޴���] �ؽ��� ��� �����Ͽ����ϴ� : [%s]"), pFileName );

		return (*mapTexItor).second;
	}

	return NULL;
}

