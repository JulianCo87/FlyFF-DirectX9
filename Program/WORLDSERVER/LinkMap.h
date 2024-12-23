#ifndef __LINKMAP_H__20041210
#define __LINKMAP_H__20041210


#include "FLMapGrid.h"

class CObj;

//struct LINKMAP_INFO
//{
//	int			nMaxLevel;
//	int			nWidth[MAX_LINKLEVEL];
//	CObj**		apObj[MAX_LINKLEVEL];
//};

class CLinkMap
{
public:
	CLinkMap();

private:
	//LINKMAP_INFO	m_infos[MAX_LINKTYPE];	
	int			m_nLandWidth, m_nLandHeight;
	//BOOL*		m_apfMask[MAX_LINKLEVEL];
	//int			m_arrVisibilityRangeNum[MAX_LINKLEVEL];
#ifdef __LAYER_1015
private:
	BOOL	m_bInvalid;
#endif	// __LAYER_1015

	int			m_iMPU;		//MPU of the world

public:
	void		Init( int nLandWidth, int nLandHeight, int nView, int nMPU );
	//void		Release();

	//int			GetLinkWidth( DWORD dwLinkType, int dwLinkLevel );
	//CObj**		GetObj( DWORD dwLinkType, DWORD dwLinkLevel );
	//DWORD		CalcLinkLevel( CObj* pObj, float fObjWidth );

	//int			GetMaxLinkLevel( DWORD dwLinkType );
	//void		SetMaxLinkLevel( DWORD dwLinkType, int nLevel );

	//BOOL		InsertObjLink( CObj* pObj );
	//BOOL		RemoveObjLink2( CObj* pObj );
	//BOOL		RemoveObjLink( CObj* pObj );
	//CObj*		GetObjInLinkMap( const D3DXVECTOR3 & vPos, DWORD dwLinkType, int nLinkLevel );
	//BOOL		SetObjInLinkMap( const D3DXVECTOR3 & vPos, DWORD dwLinkType, int nLinkLevel, CObj* pObj );

	//void		AddItToView( CCtrl* pCtrl );
	//void		ModifyView( CCtrl* pCtrl );

#ifdef __LAYER_1015
	void	Invalidate( BOOL bInvalid )	{ m_bInvalid = bInvalid; }
	BOOL	IsInvalid()		{	return m_bInvalid;	}
#endif	// __LAYER_1015

//private:
//	int			IsOverlapped( int c, int p, int r, int w );
//	CObj**		GetObjPtr( const D3DXVECTOR3 & vPos, DWORD dwLinkType, int nLinkLevel );

public:
	FLMapGrid				m_kMapGrid;
};
//
//inline int CLinkMap::GetMaxLinkLevel( DWORD dwLinkType )
//{
//	//FLASSERT( 0 < m_infos[dwLinkType].nMaxLevel );
//	return m_infos[dwLinkType].nMaxLevel;
//}
//
//inline int	CLinkMap::GetLinkWidth( DWORD dwLinkType, int dwLinkLevel ) 
//{ 
//	return m_infos[dwLinkType].nWidth[dwLinkLevel]; 
//}	
//
//inline CObj** CLinkMap::GetObj( DWORD dwLinkType, DWORD dwLinkLevel ) 
//{ 
//	return m_infos[dwLinkType].apObj[dwLinkLevel]; 
//}		

#endif  //__LINKMAP_H__20041210 