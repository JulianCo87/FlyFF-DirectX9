#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>
#include <stdio.h>
#include "xUtil3d.h"
#include "defineNeuz.h"
#include "Material.h"
#include "ModelGlobal.h"
#include "ModelObject.h"



#ifdef	_DEBUG
extern LPDIRECT3DVERTEXDECLARATION9 g_pSkinVertexDeclaration;
extern LPDIRECT3DVERTEXSHADER9	g_pSkiningVS;
#endif

BOOL	g_bUsableVS = TRUE;		// �ϵ���� ���ؽ� ���̴�(1.1)�� ���� �ִ°�.
DWORD	g_dwUsablePS_Level = 0;		

extern float g_fHairLight;
extern float s_fDiffuse[];
extern float s_fAmbient[];
extern D3DXVECTOR4 s_vLight;		// ����Ʈ ����
extern D3DXVECTOR4 s_vLightPos;		// ����Ʈ ������
extern D3DXMATRIX s_mViewProj;		// 
extern D3DXMATRIX s_mView;		// 
extern D3DXMATRIX s_mProj;		// 

extern BOOL s_bLight;


char   g_szLastFile[512];

/////////////////////////////////////////////////////////////////////////////////////
////////////
////////////
////////////
////////////
////////////
/////////////////////////////////////////////////////////////////////////////////////
CModelObject::CModelObject()
{
	Init();
}

CModelObject::~CModelObject()
{
	Destroy();
}

void CModelObject::Init( void )
{
	int		i;

	for( i = 0; i < MAX_ELEMENT; i ++ )
	{
		Init( i );
	}

	m_pBone = NULL;
	m_pForce = m_pForce2 = NULL;
	m_pPartsEffect = m_pPartsEffect2 = NULL;
	m_pMotion = m_pMotionOld = NULL;
	m_mUpdateBone = NULL;
	m_nModelType = MODELTYPE_NONE;//BIPED_MESH;
	m_bMotionBlending = FALSE;
	m_fBlendWeight = 0;
	m_bCollObj = 0;
	m_pBaseBoneInv = NULL;

	m_nFrameOld = 0;

	m_vMin.x = m_vMin.y = m_vMin.z = 65535.0f;
	m_vMax.x = m_vMax.y = m_vMax.z = -65535.0f;
	m_vForce1.x = m_vForce1.y = m_vForce1.z = 0.0f;
	m_vForce2.x = m_vForce2.y = m_vForce2.z = 0.0f;

//sun: 9�� ���ð��� (�߰� �˱� ���)
	m_vForce3.x = m_vForce3.y = m_vForce3.z = 0.0f;
	m_vForce4.x = m_vForce4.y = m_vForce4.z = 0.0f;

	m_pCloakTexture = NULL;

//sun: ���� ����Ʈ ��� ����
	m_nEffectCount  = 0;
	m_pPartsEffect1_Detail = NULL;
	m_pPartsEffect2_Detail = NULL;

#ifdef __CLIENT
	for( i = 0; i < PEF_MAX; ++i )
		m_pPartsEffectNew[i] = NULL;
#endif
}

void CModelObject::Init( int nIdx )
{
	if( nIdx < 0 )	return;
	memset( &m_Element[nIdx], 0, sizeof(m_Element[nIdx]) );
	m_Element[nIdx].m_nParentIdx = -1;
	m_Element[nIdx].m_nPartsIdx = -1;
	D3DXMatrixIdentity( &m_Element[nIdx].m_mLocalRH );
	D3DXMatrixIdentity( &m_Element[nIdx].m_mLocalLH );
	D3DXMatrixIdentity( &m_Element[nIdx].m_mLocalShield );
	D3DXMatrixIdentity( &m_Element[nIdx].m_mLocalKnuckle );
	D3DXMatrixIdentity( &m_Element[nIdx].m_mLocalTM );

	m_nFrameOld = 0;
}

//
void CModelObject::Destroy( void )
{
	int		i;

	// �޽� ����
	for( i = 0; i < MAX_ELEMENT; i ++ )
	{
		Destroy( i );
	}
	SAFE_DELETE_ARRAY( m_mUpdateBone );

	SAFE_DELETE( m_pForce );
	SAFE_DELETE( m_pForce2 );
	SAFE_DELETE( m_pPartsEffect );
	SAFE_DELETE( m_pPartsEffect2 );

//sun: ���� ����Ʈ ��� ����
	m_nEffectCount  = 0;
	SAFE_DELETE( m_pPartsEffect1_Detail );
	SAFE_DELETE( m_pPartsEffect2_Detail );
#ifdef __CLIENT
	for( i = 0; i < PEF_MAX; ++i )
		SAFE_DELETE( m_pPartsEffectNew[i] );
#endif

	Init();
	// m_pBone�� ���� �����ϸ� �ȵȴ�.
	// m_pMotion�� ���� �����ϸ� �ȵȴ�.
	// CObject3D::m_pObject3D�� ���� �����ϸ� �ȵȴ�.  CObject3DMng���� ������ �ش�.

#ifdef __ATTACH_MODEL
	m_mapAttachModel.clear();
#endif //__ATTACH_MODEL
}

#ifdef __ATTACH_MODEL
void CModelObject::SetAttachModel(int nEventIndex, CModelObject* pModelObject)
{
	if(nEventIndex < MAX_MDL_EVENT)
		m_mapAttachModel.insert(map<int, SpModelObject>::value_type(nEventIndex, SpModelObject(pModelObject)));
}

void CModelObject::SetDetachModel(int nEventIndex)
{
	map<int, SpModelObject>::iterator iter = m_mapAttachModel.find(nEventIndex);
	if(iter != m_mapAttachModel.end())
		m_mapAttachModel.erase(iter);
}

void CModelObject::InitAttachModelDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice)
{
	for(map<int, SpModelObject>::const_iterator i=m_mapAttachModel.begin(); i!=m_mapAttachModel.end(); ++i)
	{
		SpModelObject pModelObject = i->second;
		if(pModelObject.get() != NULL)
		{
			pModelObject->InitDeviceObjects(pd3dDevice);
		}
	}
}

void CModelObject::RestoreAttachModelDeviceObjects()
{
	for(map<int, SpModelObject>::const_iterator i=m_mapAttachModel.begin(); i!=m_mapAttachModel.end(); ++i)
	{
		SpModelObject pModelObject = i->second;
		if(pModelObject.get() != NULL)
		{
			pModelObject->RestoreDeviceObjects();
		}
	}
}

void CModelObject::InvalidateAttachModelDeviceObjects()
{
	for(map<int, SpModelObject>::const_iterator i=m_mapAttachModel.begin(); i!=m_mapAttachModel.end(); ++i)
	{
		SpModelObject pModelObject = i->second;
		if(pModelObject.get() != NULL)
		{
			pModelObject->InvalidateDeviceObjects();
		}
	}
}

void CModelObject::DeleteAttachModelDeviceObjects()
{
	for(map<int, SpModelObject>::const_iterator i=m_mapAttachModel.begin(); i!=m_mapAttachModel.end(); ++i)
	{
		SpModelObject pModelObject = i->second;
		if(pModelObject.get() != NULL)
		{
			pModelObject->DeleteDeviceObjects();
		}
	}
}

void CModelObject::RenderAttachModel(LPDIRECT3DDEVICE9 pd3dDevice, const D3DXMATRIX *mWorld)
{
	for(map<int, SpModelObject>::const_iterator i=m_mapAttachModel.begin(); i!=m_mapAttachModel.end(); ++i)
	{
		int nEventIndex = i->first;
		SpModelObject pModelObject = i->second;
		if(pModelObject.get() != NULL)
		{
			D3DXVECTOR3 vecAttachModel;
			GetEventPos(&vecAttachModel, nEventIndex);
			D3DXVec3TransformCoord(&vecAttachModel, &vecAttachModel, mWorld);
			
			D3DXMATRIX matRot, matAttachModel;
			D3DXMatrixIdentity(&matRot);
			D3DXMATRIX* matEventBone = GetMatrixBone(m_pBone->m_nEventParentIdx[nEventIndex]);
			matAttachModel = *matEventBone;

			D3DXQUATERNION qEventBone;
			D3DXVECTOR3 vYPW;
	
			D3DXQuaternionIdentity(&qEventBone);
			D3DXQuaternionRotationMatrix(&qEventBone, &matAttachModel);
			QuaternionRotationToYPW(qEventBone, vYPW);
			
			vYPW.x += D3DXToRadian(-90.0f);
			vYPW.y += D3DXToRadian(90.0f);
			//vYPW.z += D3DXToRadian(90.0f);
			D3DXMatrixRotationYawPitchRoll(&matRot,vYPW.y,vYPW.x,vYPW.z);
			//D3DXMatrixRotationX( &matRot, vYPW.x );
			//D3DXMatrixRotationY( &matRot, vYPW.y );
			//D3DXMatrixRotationZ( &matRot, vYPW.z );

			D3DXMATRIX matAttachModelWorld = matRot * matAttachModel * (*mWorld);
			matAttachModelWorld._41 = vecAttachModel.x;
			matAttachModelWorld._42 = vecAttachModel.y;
			matAttachModelWorld._43 = vecAttachModel.z;

			pModelObject->SetBlendFactor(m_dwBlendFactor);

			pModelObject->m_nNoEffect = m_nNoEffect;
			pModelObject->RenderAttachModelElem(pd3dDevice, &matAttachModelWorld);
			pModelObject->m_nNoEffect = 0;
		}
	}
}

int CModelObject::RenderAttachModelElem(LPDIRECT3DDEVICE9 pd3dDevice, const D3DXMATRIX *mWorld)
{
#ifdef	__WORLDSERVER
	return 1;
#else
	CObject3D	*pObject3D;
	O3D_ELEMENT	*pElem;
	int		i;
	int		nNextFrame;
	D3DXMATRIX	m1;

#ifndef __CLIENT						// ����Ŭ���̾�Ʈ�� �ƴҶ�...
	if( IsEmptyElement() == TRUE )		// ���� �ε尡 �ȵǾ� ������ �� ����
		return 1;
#endif

#ifdef	_DEBUG
	if( m_mUpdateBone && g_pSkiningVS == NULL )
		FLERROR_LOG( PROGRAM_NAME, _T( "CMyD3DApplication::RestoreDeviceObjects()�� CreateSkiningVS()�� ȣ���Ͻÿ�. CMyD3DApplication::InvalidateDeviceObjects()���� DeleteSkiningVS()�� ȣ���Ͻÿ�. Ȥ�� *.vsh������ ���� ���ߴ�." ) );
	if( m_nLoop == 0 )
		FLERROR_LOG( PROGRAM_NAME, _T( "%s : CModelObject::m_nLoop�� �������� �ʾҴ�." ), m_pMotion->m_szName );
#endif

	if( m_pMotion )		// m_pMotion�� ���̶�� m_mUpdateBone�� ���̴�.
		m_pMotion->AnimateBone( m_mUpdateBone, m_pMotionOld, m_fFrameCurrent, GetNextFrame(), m_nFrameOld, m_bMotionBlending, m_fBlendWeight );		// �ϴ� ���밡 �ִٸ� ���� �ִϸ��̼� ��Ŵ

	if( m_pBone )		// m_pBone�� �ִٸ� ���밡 �ִٴ� ���. VS�� ��� �Ѵ�.
	{
		D3DXMATRIX *pmBones;
		D3DXMATRIX mWorldTranspose;
		D3DXMATRIX *pmBonesInv = m_pBaseBoneInv ;
		BONE	*pBoneList = m_pBone->m_pBones;
		pmBones = m_mUpdateBone;

		if( m_pBone->m_bSendVS )	// ���밳���� MAX_VS_BONE���϶� �ѹ��� �� �����Ѵ�.
		{
			int		nMaxBone = m_pBone->m_nMaxBone;

			if( nMaxBone > MAX_VS_BONE )	
				FLERROR_LOG( PROGRAM_NAME, _T( "overflow bone count - %d" ), nMaxBone );

			for( i = 0; i < nMaxBone; i ++ )	// MAX_VS_BONE�� ����	
			{
				mWorldTranspose = pmBonesInv[i] * pmBones[i];				
				D3DXMatrixTranspose( &mWorldTranspose, &mWorldTranspose );		// ��Ʈ������ ��������.
				m_pd3dDevice->SetVertexShaderConstantF( i * 3, (float*)&mWorldTranspose, 3 );		// ����������Ϳ� �������.
			}
		}
		D3DXMATRIX	mView, mProj;
		D3DXMATRIX	mViewProj, mViewProjTranspose, mInvWorld;

		D3DXVECTOR4 vLight = s_vLight;
		D3DXVECTOR4 vLightPos = s_vLightPos;

		mViewProj = *mWorld * s_mView * s_mProj;
		
		D3DXMatrixTranspose( &mViewProjTranspose, &mViewProj );
		D3DXMatrixTranspose( &mWorldTranspose, mWorld );

		D3DXMatrixInverse( &mInvWorld, NULL, mWorld );
		D3DXVec4Transform( &vLight, &vLight, &mInvWorld );
		D3DXVec4Normalize( &vLight, &vLight );
		D3DXVec4Transform( &vLightPos, &vLightPos, &mInvWorld );

		m_pd3dDevice->SetVertexShaderConstantF( 84, (float*)&mViewProjTranspose, 4 );
		m_pd3dDevice->SetVertexShaderConstantF( 91, (float*)&vLightPos, 1 );
		m_pd3dDevice->SetVertexShaderConstantF( 92, (float*)&vLight,   1 );
		m_pd3dDevice->SetVertexShaderConstantF( 93, (float*)&s_fDiffuse, 1 );
		m_pd3dDevice->SetVertexShaderConstantF( 94, (float*)&s_fAmbient, 1 );
	}

	nNextFrame = GetNextFrame();
	pd3dDevice->SetMaterial( g_TextureMng.GetMaterial( pd3dDevice, 0 ) );

	D3DXVECTOR3 vec3LightBackup = D3DXVECTOR3( s_vLight[0], s_vLight[1], s_vLight[2] );
	D3DXVECTOR4 vec4Diffuse = D3DXVECTOR4( s_fDiffuse[0], s_fDiffuse[1], s_fDiffuse[2], s_fDiffuse[3] );;
	
	// ������Ʈ�� ��Ų,�Ϲ�,���� ������� ��ƼŬ������ ���Եɼ��ִ�.
	pElem = &m_Element[0];
	pObject3D = pElem->m_pObject3D;

	if(pElem != NULL && pObject3D != NULL && !(pElem->m_nEffect & XE_HIDE))
	{
		pObject3D->m_nNoTexture = 0; // m_nNoTexture;
		pObject3D->m_nNoEffect = m_nNoEffect;
		m1 = *mWorld;

		if( m_pBone )
			pObject3D->SetExternBone( m_mUpdateBone, m_pBaseBoneInv );	// ���庻�� �ִٸ� �װ��� �Ѱ��ش�.
		
		if( m_SparkInfo.m_bUsed )
		{
			SetLightVec( m_SparkInfo.m_v3SparkDir );
			
			D3DXVECTOR3 vec3Diffuse;
			D3DXVec3Lerp( &vec3Diffuse, &D3DXVECTOR3(0.0f,0.0f,0.0f), &m_SparkInfo.m_v3Color, m_SparkInfo.m_fLerp );
			SetDiffuse( vec3Diffuse.x, vec3Diffuse.y, vec3Diffuse.z );
		}
		
		DWORD dwBlendFactor = m_dwColor | ( m_dwBlendFactor << 24 );
		pObject3D->Render( pd3dDevice, pElem->m_ppd3d_VBSel, m_fFrameCurrent, nNextFrame, &m1, pElem->m_nEffect, dwBlendFactor );

		pObject3D->m_nNoEffect = 0;
	}
	
	return 1;
#endif // !__WORLDSERVER
}

void CModelObject::FrameMoveAttachModel(D3DXVECTOR3 *pvSndPos, float fSpeed)
{
	for(map<int, SpModelObject>::const_iterator i=m_mapAttachModel.begin(); i!=m_mapAttachModel.end(); ++i)
	{
		SpModelObject pModelObject = i->second;
		if(pModelObject.get() != NULL)
		{
			pModelObject->FrameMove(pvSndPos, fSpeed);
		}
	}
}
#endif //__ATTACH_MODEL

CString CModelObject::GetMotionFileName( const CString& rstrMotion ) const
{
	static TCHAR s_szRoot[ MAX_OBJTYPE ][32] = { "obj", "ani", "ctrl", "sfx", "item", "mvr", "region", "obj", "path" };

	CString strBoneFileName;
	if( m_pBone == NULL || m_pModelElem == NULL )
		return strBoneFileName;

	strBoneFileName.Format( _T("%s_%s_%s.ani"), s_szRoot[ m_pModelElem->m_dwType ], m_pModelElem->getName(), rstrMotion );
	return strBoneFileName;
}

// OBB�� line�� �浹�׽�Ʈ 
// BOOL bNeedCollObject = TRUE
BOOL CModelObject::TestIntersectionOBB_Line( const Segment3& segment, const CObj* pObj, BOOL bNeedCollObject )
{ 
	CObject3D *pObject3D = m_Element[0].m_pObject3D;
	if( pObject3D->m_CollObject.m_Type == GMT_ERROR && bNeedCollObject )	// �浹�޽ð� ������ �˻����� ����.
		return FALSE;
// �浹 ���� ����
	// ���������� �ʹ� �������� ����̶� �����ս� ���鿡�� ����ó����.. ���� ������ �ʴ�.. 
	char* pString = ((CModelObject *)pObj->m_pModel)->GetObject3D()->m_szFileName;
	if( strcmp("obj_guildcombatground.o3d", pString) == 0 )
	{
		return TRUE;
	}

	#ifdef __CPU_UTILDOWN_060502	//sun!!
		return IntrSegment3Box3_Test( segment, pObj->m_OBB );
	#else
		const D3DXMATRIX& mWorld = pObj->GetMatrixWorld();

		D3DXVECTOR3 vTemp;
		D3DXVECTOR3 vStart, vEnd;
		vStart = segment.Origin - segment.Extent * segment.Direction;
		vEnd   = segment.Origin + segment.Extent * segment.Direction;
		return ::IsTouchOBB_Line( m_vMin, m_vMax, mWorld, vStart, vEnd, &vTemp );
	#endif
}

//
BOOL CModelObject::Destroy( int nIdx )
{
	if( nIdx < 0 )	return FALSE;
	O3D_ELEMENT	*pElem = &m_Element[ nIdx ];
	int		i;

	if( pElem->m_pObject3D )
	{
		if( pElem->m_ppd3d_VB )
		{
			pElem->m_pObject3D->SetLOD(0);
			for( i = 0; i < pElem->m_pObject3D->GetMaxObject(); i ++ )		// 
				SAFE_RELEASE( pElem->m_ppd3d_VB1[i] );
			pElem->m_pObject3D->SetLOD(1);
			for( i = 0; i < pElem->m_pObject3D->GetMaxObject(); i ++ )
				SAFE_RELEASE( pElem->m_ppd3d_VB2[i] );
			pElem->m_pObject3D->SetLOD(2);
			for( i = 0; i < pElem->m_pObject3D->GetMaxObject(); i ++ )
				SAFE_RELEASE( pElem->m_ppd3d_VB3[i] );
			pElem->m_pObject3D->SetLOD(0);
			
			SAFE_DELETE_ARRAY( pElem->m_ppd3d_VB );		// ��ü �޸� Ǯ�� �Ѳ����� ����.
		}
		g_Object3DMng.DeleteObject3D( pElem->m_pObject3D );	// pObject3D�� �����Ȱ� �ƴϸ� ���⼭ ���� ����.

		Init( nIdx );
		return TRUE;
	}
	Init( nIdx );
	return FALSE;
}

// �˱�Ŭ���� ����
#ifdef __CLIENT
void	CModelObject::CreateForce( int nParts )
{

	if( nParts == PARTS_RWEAPON )
	{
		if( m_pForce )	return;		// �̹� �Ҵ�Ǿ� ������ �ٽ� �Ҵ����� ����.
		m_pForce = new CSwordForce;
	} else
	if( nParts == PARTS_LWEAPON )
	{
		if( m_pForce2 )	return;		// �̹� �Ҵ�Ǿ� ������ �ٽ� �Ҵ����� ����.
		m_pForce2 = new CSwordForce;
	} else
	{
		FLASSERT(0);
	}
}
#else
void	CModelObject::CreateForce( int /*nParts*/ )
{
}
#endif


void	CModelObject::TakeOffParts( int nParts )
{
	if( nParts < 0 )
		FLERROR_LOG( PROGRAM_NAME, _T( "nParts = %d" ), nParts );


	// ������ �����ߴµ�, �Ʒ� �κ��� �����ָ� old �������� �Ÿ� �� �߻� - vampyre
	O3D_ELEMENT	*pElem;
	int nIdx = GetPartsIdx(nParts);
	pElem = GetParts(nParts);
	if( pElem && pElem->m_ppd3d_VB )
	{
		pElem->m_pObject3D->SetLOD(0);
		for( int j = 0; j < pElem->m_pObject3D->GetMaxObject(); j ++ )		// 
			SAFE_RELEASE( pElem->m_ppd3d_VB1[j] );
		pElem->m_pObject3D->SetLOD(1);
		for( int j = 0; j < pElem->m_pObject3D->GetMaxObject(); j ++ )
			SAFE_RELEASE( pElem->m_ppd3d_VB2[j] );
		pElem->m_pObject3D->SetLOD(2);
		for( int j = 0; j < pElem->m_pObject3D->GetMaxObject(); j ++ )
			SAFE_RELEASE( pElem->m_ppd3d_VB3[j] );
		pElem->m_pObject3D->SetLOD(0);
		SAFE_DELETE_ARRAY( pElem->m_ppd3d_VB );		// ��ü �޸� Ǯ�� �Ѳ����� ����.
	}

	if(nIdx != -1)
	{
	//sun: ���� ����Ʈ ��� ����
		if( nParts == PARTS_LWEAPON || nParts == PARTS_RWEAPON )
		{
			m_nEffectCount  = 0;

			SAFE_DELETE(m_pPartsEffect);
			SAFE_DELETE(m_pPartsEffect2);
			SAFE_DELETE(m_pPartsEffect1_Detail);
			SAFE_DELETE(m_pPartsEffect2_Detail);
			//m_pPartsEffect = m_pPartsEffect2 = NULL;
			// m_pPartsEffect1_Detail = NULL;
			// m_pPartsEffect2_Detail = NULL;
		}
	
		Init( nIdx );
	}
}

//
//		LoadBone
//
int		CModelObject::LoadBone( LPCTSTR szFileName )
{
	if( m_pBone )
		FLERROR_LOG( PROGRAM_NAME, _T( "�̹� ���� �ε��Ǿ� �ִ�. 0x%08x" ), (int)m_pBone );

	// Bone �����ڸ� ���� ���� �ε����� �� �����͸� �޾ƿ´�.
	m_pBone = g_BonesMng.LoadBone( szFileName );

	// ���� ���о���.
	if( m_pBone == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s : ã�� �� ����" ), szFileName );
		return FAIL;
	}

	m_bSkin = TRUE;		// ��Ű�� ������Ʈ��� �÷���
#ifdef _DEBUG
	if( m_mUpdateBone )
		FLERROR_LOG( PROGRAM_NAME, _T( "%s �̹� �о��µ� �� ������ ����." ), szFileName );
#endif

	// ������Ʈ�� ���������� �ִϸ��̼ǵǸ鼭 �ǽð����� ��ȯ�Ǵ� ��Ʈ���� �迭
	m_mUpdateBone = new D3DXMATRIX[ m_pBone->m_nMaxBone * 2 ];	// Inv�� ���� ������ * 2�� ��´�,.
	m_pBaseBoneInv = m_mUpdateBone + m_pBone->m_nMaxBone;
	
	int		i;
	for( i = 0; i < m_pBone->m_nMaxBone; i ++ )
	{
		m_mUpdateBone[i] = m_pBone->m_pBones[i].m_mTM;		// �⺻���� �̸� ī���ص�.
		m_pBaseBoneInv[i] = m_pBone->m_pBones[i].m_mInverseTM;	// Inv�� �̸� �޾Ƶ�.
	}
	return SUCCESS;
}


//
//	LoadMotion
// ������Ʈ�� �ε������ʰ� ���������� �����ؾ��Ѵ�.
// 
int		CModelObject::LoadMotion( LPCTSTR szFileName )
{
	int		i;
	// ���� ���� �Ҵ� �Ǿ� ���� ������ �ִϸ� ������ ����.
	if( m_pBone == NULL )		
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s : ���� �ε����� �ʰ� ����� ����" ), szFileName );
		return FAIL;
	}
	m_pMotionOld = m_pMotion;		// ���� ��ǹ޾Ƶ�.
	m_nFrameOld = (int)m_fFrameCurrent;
	// Motion�����ڸ� ���� ������ �ε����� �� �����͸� �޾ƿ´�.
	m_pMotion = g_MotionMng.LoadMotion( szFileName );
	if( m_pMotion == NULL )		// ����� ���о���.
	{
		InitAnimate();
		return FAIL;
	}
	if( m_pMotionOld == NULL )
		m_pMotionOld = m_pMotion;		// ������������ ������ ���� �������� Old�� ����.  NULL�� �Ǵ»�Ȳ�� ���� �Ұ�.

	InitAnimate();

	m_nFrameMax = m_pMotion->GetMaxFrame();
	m_fPerSlerp = (float)((int)(m_pMotion->m_fPerSlerp * 1000.0f)) / 1000.0f;		// �Ҽ��� 3° ���Ͽ��� ������ ����� �����Ƿ� 3°���� ���� �߶������,.

	if( m_pMotionOld->m_nMaxBone != m_pMotion->m_nMaxBone )		// ������ ���� ����� �������� �ٸ��� ������.
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s bone count(%d) - old count(%d)" ), szFileName, m_pMotion->m_nMaxBone, m_pMotionOld->m_nMaxBone );
	}

	// �̹� ��Ų ������Ʈ�� �ε��Ǿ� �ִ� ���¸� m_mUpdateBone�� �� ������Ʈ�� ��Ͻ�Ŵ.
	for( i = 0; i < MAX_ELEMENT; i ++ )
	{
		if( m_Element[i].m_pObject3D )
			m_Element[i].m_pObject3D->SetExternBone( m_mUpdateBone, m_pBaseBoneInv );	// �Լ����ο��� ��Ų�γѸ� ��Ͻ�Ų��.
	}
	return SUCCESS;
}

// ������ �ű�.
// �޼տ� ��� �����տ� �ű�µ�...
int	CModelObject::MovePart( int nDstPart, int nSrcPart )
{
	O3D_ELEMENT* pDestPart = GetParts( nDstPart );
	O3D_ELEMENT* pSrcPart = GetParts( nSrcPart );

	if( !pDestPart && pSrcPart )
	{
		LoadElement( pSrcPart->m_pObject3D->m_szFileName, nDstPart );
		memset( pSrcPart, 0, sizeof(*pSrcPart) );		// �ҽ����� Ŭ����.	
		return 1;
	}

	if( !pDestPart || !pSrcPart )
		return 0;

	if( pDestPart->m_pObject3D )
		FLERROR_LOG( PROGRAM_NAME, _T( "m_Element[%d].m_pObject3D == 0x%08x" ), nDstPart, (int)pDestPart->m_pObject3D );
	
	memcpy( pDestPart, pSrcPart, sizeof(O3D_ELEMENT) );	// ī���ϰ�
	memset( pSrcPart, 0, sizeof(*pSrcPart) );		// �ҽ����� Ŭ����.	
	return 1;
}

//
//	������Ʈ �ε�.  ����� ���� ���� ���¿����� ���������� �����Ѵ�.
//
int		CModelObject::LoadElement( LPCTSTR szFileName, int nParts )
{
	CObject3D	*pObject3D;
	O3D_ELEMENT	*pElem;

	if( nParts < 0 )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "nParts ���� - nParts = %d, %s" ), nParts, szFileName );
		return FAIL;
	}

	if( nParts == PARTS_RIDE )
		return FAIL;

	if(  nParts >= PARTS_LOWER2 && nParts <= PARTS_BULLET )
		return FAIL;

	int nIdx = GetPartsIdx(nParts);
	Destroy( nIdx );

#ifdef __CLIENT
	HRESULT hr;
	// Object3D�����ڸ� ���� �޽��� �ε��� �� �� �����͸� �޾ƿ´�.
	if( FAILED( hr = m_pd3dDevice->TestCooperativeLevel() ) )		// ����̽��� �����ϸ� ��������.
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s ����̽����� %08x" ), szFileName, (int)hr );
	}
#endif
	pObject3D = g_Object3DMng.LoadObject3D( m_pd3dDevice, szFileName );
	if( pObject3D == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "%s : ã�� �� ����" ), MakePath( DIR_MODEL, szFileName ) );
		return FAIL;
	}

	pObject3D->InitDeviceObjects( m_pd3dDevice );

	// ��Ų������ ��� �ܺκ��� �о����� �˻�.
	if( pObject3D->IsUseExternBone() )		// ���庻�� ����ϴ� �������̴�.
	{
		if( m_pBone == NULL )			// ���� ���� ������ ���´�...
		{
#ifdef __CLIENT
			FLERROR_LOG( PROGRAM_NAME, _T( "%s : bone not loaded." ), szFileName );		// ���� �ε����� �ʰ� �޽��� ����
#endif // __CLIENT
			return FAIL;
		}
	}

	int i=0;
	for( ; i<MAX_ELEMENT; i++ )
	{
		if( m_Element[ i ].m_nPartsIdx == -1 )
			break;
	}

	if( i == MAX_ELEMENT )
		return FAIL;

	pElem = &m_Element[ i ];
	memset( pElem, 0, sizeof(*pElem) );
	pElem->m_nPartsIdx = nParts;
	pElem->m_nParentIdx = -1;
	D3DXMatrixIdentity( &(pElem->m_mLocalRH) );
	D3DXMatrixIdentity( &(pElem->m_mLocalLH) );
	D3DXMatrixIdentity( &(pElem->m_mLocalShield) );
	D3DXMatrixIdentity( &(pElem->m_mLocalKnuckle) );


	pElem->m_pObject3D = pObject3D;
	
	memset( g_szLastFile, 0, sizeof(g_szLastFile) );
	FLStrcpy( g_szLastFile, _countof( g_szLastFile ), szFileName );

	if( pObject3D->GetGMOBJECT() == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "ƨ�� : ���� ���ϸ� %s" ), MakePath( DIR_MODEL, szFileName ) );
		return FAIL;
	}
	
	if( pObject3D->GetGMOBJECT()->m_bOpacity )
		m_dwRenderEffect = REFF_ALPHA;
	m_bCollObj = pObject3D->IsHaveCollObj();
	if( pObject3D->m_nHavePhysique )	m_bSkin = TRUE;

	if( m_nFrameMax > 0 )	// �̹� ����̳� �������� �о maxframe�� ������ �̹��� ���� ������Ʒ maxframe�ϰ� ��. Ʋ���� ����.
	{
		if( pObject3D->GetMaxFrame() )
			if( m_nFrameMax != pObject3D->GetMaxFrame() )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "%s�� MaxFrame(%d)�� ������ ���� MaxFrame(%d)�� �ٸ���." ), szFileName, pObject3D->GetMaxFrame(), m_nFrameMax );
			}
	}
	if( m_nFrameMax == 0 )	// maxframe�� ���õ����� �������� ���� ������.
		m_nFrameMax = pObject3D->GetMaxFrame();

	// �ִϸ��̼� �ӵ�
	m_fPerSlerp = (float)((int)(pObject3D->m_fPerSlerp * 1000.0f)) / 1000.0f;		// �Ҽ��� 3° ���Ͽ��� ������ ����� �����Ƿ� 3°���� ���� �߶������,.


	// �̹� ���� �ε��Ǿ� �ִ� ���¸� m_pUpdateBone�� ������Ʈ�� �ܺλ���� �����.
	if( m_pBone )
		pObject3D->SetExternBone( m_mUpdateBone, m_pBaseBoneInv );

	// ���� ���� Mesh��  BB�� �������� CModelObject�� BB�� ����
	// �̺κа���� ���� ��Ȯ���� �ʴ�.  Min,Max��� Local�����ε��ٰ�
	// ����� â�� ��� ������� â���� �ٿ�� �ڽ��� ���Եǰ� �ִ�.
	if( pObject3D->m_vBBMin.x < m_vMin.x )	m_vMin.x = pObject3D->m_vBBMin.x;
	if( pObject3D->m_vBBMin.y < m_vMin.y )	m_vMin.y = pObject3D->m_vBBMin.y;
	if( pObject3D->m_vBBMin.z < m_vMin.z )	m_vMin.z = pObject3D->m_vBBMin.z;
	if( pObject3D->m_vBBMax.x > m_vMax.x )	m_vMax.x = pObject3D->m_vBBMax.x;
	if( pObject3D->m_vBBMax.y > m_vMax.y )	m_vMax.y = pObject3D->m_vBBMax.y;
	if( pObject3D->m_vBBMax.z > m_vMax.z )	m_vMax.z = pObject3D->m_vBBMax.z;
	SetBB( m_BB.m_vPos, m_vMin, m_vMax );

	// �о�� ������Ʈ�� this�� ���뿡 �θ������� �ȵǾ� �ְ� �Ѵ�.
	// ���� Į�� �о�Դٸ� �������� SetParent()�ϵ��� �Ѵ�.
	pElem->m_nParentIdx = -1;

	if( pObject3D->m_nHavePhysique )	// ����ũ�� ������ �ִٸ� ��Ų�� ���ؽ����۸� ���� ����
	{
		if( pElem->m_ppd3d_VB )
			FLERROR_LOG( PROGRAM_NAME, _T( "%s pElem->m_ppd3d_VB is not NULL" ), szFileName );

		int nTotal = 0;
		pObject3D->SetLOD(0);	nTotal += pObject3D->GetMaxObject();
		pObject3D->SetLOD(1);	nTotal += pObject3D->GetMaxObject();
		pObject3D->SetLOD(2);	nTotal += pObject3D->GetMaxObject();
		pObject3D->SetLOD(0);
		
		if( nTotal <= 0 )
			FLERROR_LOG( PROGRAM_NAME, _T( "%s LoadElement : nTotal=%d" ), pObject3D->m_szFileName, nTotal );

		pElem->m_ppd3d_VB = new LPDIRECT3DVERTEXBUFFER9[ nTotal ];
		memset( pElem->m_ppd3d_VB, 0, sizeof(LPDIRECT3DVERTEXBUFFER9) * nTotal );
		LPDIRECT3DVERTEXBUFFER9 *ppd3d_VB = pElem->m_ppd3d_VB;
		pObject3D->SetLOD(0);		pElem->m_ppd3d_VB1 = ppd3d_VB;		ppd3d_VB += pObject3D->GetMaxObject();
		pObject3D->SetLOD(1);		pElem->m_ppd3d_VB2 = ppd3d_VB;		ppd3d_VB += pObject3D->GetMaxObject();
		pObject3D->SetLOD(2);		pElem->m_ppd3d_VB3 = ppd3d_VB;		
		pObject3D->SetLOD(0);
		pElem->m_ppd3d_VBSel = pElem->m_ppd3d_VB1;
	}
	
	pObject3D->RestoreDeviceObjects( pElem->m_ppd3d_VB );		// �̰� ���⼭ �ϸ� �ȵǴ� ������ �־ ���Ѱǵ� �� ������ ����� �ȳ��� -_-;;;

	return SUCCESS;
}

#ifdef __CLIENT

int CModelObject::LoadClonedElement( LPCTSTR szFileName )
{
	//gmpbigsun: ���� ���� �� ����( ���� �ؽ��� ) .. ������.
	CObject3D* pObject3D = NULL;
	O3D_ELEMENT* pElem = NULL;

	pObject3D	= new CObject3D;
	pObject3D->InitDeviceObjects( m_pd3dDevice );
	if( pObject3D->LoadObject( szFileName ) == FAIL )
	{
		FLASSERT( 0 );
		SAFE_DELETE( pObject3D );
		return -1;
	}

	pObject3D->InitDeviceObjects( m_pd3dDevice );

	pElem = &m_Element[ 0 ];
	memset( pElem, 0, sizeof(*pElem) );
	pElem->m_nPartsIdx = 0;
	pElem->m_nParentIdx = -1;
	D3DXMatrixIdentity( &(pElem->m_mLocalRH) );
	D3DXMatrixIdentity( &(pElem->m_mLocalLH) );
	D3DXMatrixIdentity( &(pElem->m_mLocalShield) );
	D3DXMatrixIdentity( &(pElem->m_mLocalKnuckle) );

	pElem->m_pObject3D = pObject3D;

	m_bCollObj = pObject3D->IsHaveCollObj();

	memset( g_szLastFile, 0, sizeof(g_szLastFile) );
	FLStrcpy( g_szLastFile, _countof( g_szLastFile ), szFileName );
	if( pObject3D->m_nHavePhysique )	
		m_bSkin = TRUE;

	if( m_nFrameMax == 0 )	// maxframe�� ���õ����� �������� ���� ������.
		m_nFrameMax = pObject3D->GetMaxFrame();

	m_fPerSlerp = (float)((int)(pObject3D->m_fPerSlerp * 1000.0f)) / 1000.0f;		// �Ҽ��� 3° ���Ͽ��� ������ ����� �����Ƿ� 3°���� ���� �߶������,.

	if( m_nFrameMax > 0 )	// �̹� ����̳� �������� �о maxframe�� ������ �̹��� ���� ������Ʒ maxframe�ϰ� ��. Ʋ���� ����.
	{
		if( pObject3D->GetMaxFrame() )
			if( m_nFrameMax != pObject3D->GetMaxFrame() )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "%s�� MaxFrame(%d)�� ������ ���� MaxFrame(%d)�� �ٸ���." ), szFileName, pObject3D->GetMaxFrame(), m_nFrameMax );
			}
	}
	if( m_nFrameMax == 0 )	// maxframe�� ���õ����� �������� ���� ������.
		m_nFrameMax = pObject3D->GetMaxFrame();

	// �ִϸ��̼� �ӵ�
	m_fPerSlerp = (float)((int)(pObject3D->m_fPerSlerp * 1000.0f)) / 1000.0f;		// �Ҽ��� 3° ���Ͽ��� ������ ����� �����Ƿ� 3°���� ���� �߶������,.


	// �̹� ���� �ε��Ǿ� �ִ� ���¸� m_pUpdateBone�� ������Ʈ�� �ܺλ���� �����.
	if( m_pBone )
		pObject3D->SetExternBone( m_mUpdateBone, m_pBaseBoneInv );

	// ���� ���� Mesh��  BB�� �������� CModelObject�� BB�� ����
	// �̺κа���� ���� ��Ȯ���� �ʴ�.  Min,Max��� Local�����ε��ٰ�
	// ����� â�� ��� ������� â���� �ٿ�� �ڽ��� ���Եǰ� �ִ�.
	if( pObject3D->m_vBBMin.x < m_vMin.x )	m_vMin.x = pObject3D->m_vBBMin.x;
	if( pObject3D->m_vBBMin.y < m_vMin.y )	m_vMin.y = pObject3D->m_vBBMin.y;
	if( pObject3D->m_vBBMin.z < m_vMin.z )	m_vMin.z = pObject3D->m_vBBMin.z;
	if( pObject3D->m_vBBMax.x > m_vMax.x )	m_vMax.x = pObject3D->m_vBBMax.x;
	if( pObject3D->m_vBBMax.y > m_vMax.y )	m_vMax.y = pObject3D->m_vBBMax.y;
	if( pObject3D->m_vBBMax.z > m_vMax.z )	m_vMax.z = pObject3D->m_vBBMax.z;
	SetBB( m_BB.m_vPos, m_vMin, m_vMax );

	// �о�� ������Ʈ�� this�� ���뿡 �θ������� �ȵǾ� �ְ� �Ѵ�.
	// ���� Į�� �о�Դٸ� �������� SetParent()�ϵ��� �Ѵ�.
	pElem->m_nParentIdx = -1;

	if( pObject3D->m_nHavePhysique )	// ����ũ�� ������ �ִٸ� ��Ų�� ���ؽ����۸� ���� ����
	{
		if( pElem->m_ppd3d_VB )
			FLERROR_LOG( PROGRAM_NAME, _T( "%s pElem->m_ppd3d_VB is not NULL" ), szFileName );

		int nTotal = 0;
		pObject3D->SetLOD(0);	nTotal += pObject3D->GetMaxObject();
		pObject3D->SetLOD(1);	nTotal += pObject3D->GetMaxObject();
		pObject3D->SetLOD(2);	nTotal += pObject3D->GetMaxObject();
		pObject3D->SetLOD(0);
		
		if( nTotal <= 0 )
			FLERROR_LOG( PROGRAM_NAME, _T( "%s LoadElement : nTotal=%d" ), pObject3D->m_szFileName, nTotal );

		pElem->m_ppd3d_VB = new LPDIRECT3DVERTEXBUFFER9[ nTotal ];
		memset( pElem->m_ppd3d_VB, 0, sizeof(LPDIRECT3DVERTEXBUFFER9) * nTotal );
		LPDIRECT3DVERTEXBUFFER9 *ppd3d_VB = pElem->m_ppd3d_VB;
		pObject3D->SetLOD(0);		pElem->m_ppd3d_VB1 = ppd3d_VB;		ppd3d_VB += pObject3D->GetMaxObject();
		pObject3D->SetLOD(1);		pElem->m_ppd3d_VB2 = ppd3d_VB;		ppd3d_VB += pObject3D->GetMaxObject();
		pObject3D->SetLOD(2);		pElem->m_ppd3d_VB3 = ppd3d_VB;		
		pObject3D->SetLOD(0);
		pElem->m_ppd3d_VBSel = pElem->m_ppd3d_VB1;
	}
	
	pObject3D->RestoreDeviceObjects( pElem->m_ppd3d_VB );

	return 1;

}
#endif //__CLIENT

// SetParent( PARTS_LWEAPON, BONE_RHAND );
// nParts�� ������Ʈ�� �θ� nBoneIdx�� ����
// �θ� �ٲܶ� ����.
void	CModelObject::SetParent( int nParts, int nBoneIdx )
{
	O3D_ELEMENT	*pElem = GetParts( nParts );
	if( !pElem )
		return;


	pElem->m_nParentIdx = nBoneIdx;
	pElem->m_mLocalTM = pElem->m_pObject3D->GetGMOBJECT()->m_mLocalTM;

	// ���� ���� ������ �ٲ���ٸ� BONE_RHAND�� �ε����� �ٲ��� �Ѵ�. �ΰ������� �� ������ ���� ���Ѿ� �Ѵ�.
	// �� �ε����� ���� �˼� �ִ� ����� CModelObject->m_pMotion->m_pBoneInfo[]�� 0~32������ �Է½��Ѻ��鼭 Bip01 R Hand�� ã�´�,
	if( nBoneIdx == GetRHandIdx() )	
		pElem->m_mLocalRH = m_pBone->m_mLocalRH;

	if( nBoneIdx == GetLHandIdx() )	
		pElem->m_mLocalLH = m_pBone->m_mLocalLH;
	
	if( nBoneIdx == GetLArmIdx() )	
		pElem->m_mLocalShield = m_pBone->m_mLocalShield;

	if( nBoneIdx == GetRArmIdx() )
		pElem->m_mLocalKnuckle = m_pBone->m_mLocalKnuckle;
}

//
// nParts�� �ؽ������� szBitmap�� ��Ƽ�ؽ��ķ� ���
// ���� �ؽ��ĸ� �ı��� �ʿ�� ����.
// �ݵ�� LoadMesh()���Ŀ� �Ǿ�� �Ѵ�.
//
#if !defined(__WORLDSERVER)
void	CModelObject::SetTextureMulti( LPCTSTR szBitmap, int nParts )
{

	D3DMATERIAL9	d3dmtrl;
	MATERIAL		*mtrl;

	mtrl = g_TextureMng.AddMaterial( m_pd3dDevice, &d3dmtrl, szBitmap );
	O3D_ELEMENT* pParts = GetParts(nParts);

	if( pParts )
		pParts->m_pTextureEx = mtrl->m_pTexture;
}
#else
void	CModelObject::SetTextureMulti( LPCTSTR /*szBitmap*/, int /*nParts*/ )
{

}

#endif

//
void	CModelObject::ClearTextureEx( int nParts )
{
	O3D_ELEMENT* pParts = GetParts(nParts);
	
	if( pParts )
		pParts->m_pTextureEx = NULL;
}

//

#ifdef __WORLDSERVER
void	CModelObject::ChangeTexture( int /*nParts*/, LPCTSTR /*szSrc*/, LPCTSTR /*szDest*/ )
{
	return;
}
#else
void	CModelObject::ChangeTexture( int nParts, LPCTSTR szSrc, LPCTSTR szDest )
{
	O3D_ELEMENT* pParts = GetParts(nParts);
	if( pParts )
		pParts->m_pObject3D->ChangeTexture( szSrc, szDest );
}
#endif


#ifdef __CLIENT
//---------------------------------------------------------------------------------------------
//
// Render()�� �θ��� ���ο��� SetTransform()�� ���ְ� �ܺο��� ��Ʈ������ �ѱ�°��� ��Ģ���� ����.
// �Ѱ��ִ� ��Ʈ������ ������Ʈ�� ������ �Ѱ��ش�.
// ���� �ΰ��� ���� ���ͺκ��̴�.
//
FLOAT   g_fDiffuse[4];
FLOAT   g_fAmbient[4];
/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
���࿡ Render�ߴµ� ȭ�鿡 �ȳ��´�!!! �׷���!
static D3DXVECTOR4 vConst( 1.0f, 1.0f, 1.0f, 100.0f );
m_pd3dDevice->SetVertexShaderConstantF( 95, (float*)&vConst, 1 );
SetTransformView( matView );
SetTransformProj( matProj );
SetDiffuse( 1.0, 1.0, 1.0 );
SetAmbient( 1.0, 1.0, 1.0 );

�̰� ���� �ϱ����� �ҷ������ Ȯ���ض�!!!!!!!!!!!!!!!!!!!!! ũ�ƾƾ�!
���� ����߳�!

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
 */
int		CModelObject::Render( LPDIRECT3DDEVICE9 pd3dDevice, const D3DXMATRIX *mWorld )
{
	CObject3D	*pObject3D;
	O3D_ELEMENT	*pElem;
	int		i;
	int		nNextFrame;
	D3DXMATRIX	m1;

#ifdef	_DEBUG
	if( m_mUpdateBone && g_pSkiningVS == NULL )
		FLERROR_LOG( PROGRAM_NAME, _T( "CMyD3DApplication::RestoreDeviceObjects()�� CreateSkiningVS()�� ȣ���Ͻÿ�. CMyD3DApplication::InvalidateDeviceObjects()���� DeleteSkiningVS()�� ȣ���Ͻÿ�. Ȥ�� *.vsh������ ���� ���ߴ�." ) );
	if( m_nLoop == 0 )
		FLERROR_LOG( PROGRAM_NAME, _T( "��� : %s : CModelObject::m_nLoop�� �������� �ʾҴ�." ), m_pMotion->m_szName );
#endif

	if( m_pMotion )		// m_pMotion�� ���̶�� m_mUpdateBone�� ���̴�.
		m_pMotion->AnimateBone( m_mUpdateBone, m_pMotionOld, m_fFrameCurrent, GetNextFrame(), m_nFrameOld, m_bMotionBlending, m_fBlendWeight );		// �ϴ� ���밡 �ִٸ� ���� �ִϸ��̼� ��Ŵ

	if( m_pBone )		// m_pBone�� �ִٸ� ���밡 �ִٴ� ���. VS�� ��� �Ѵ�.
	{
		D3DXMATRIX *pmBones;
		D3DXMATRIX mWorldTranspose;
		D3DXMATRIX *pmBonesInv = m_pBaseBoneInv ;
		BONE	*pBoneList = m_pBone->m_pBones;
		pmBones = m_mUpdateBone;

		if( m_pBone->m_bSendVS )	// ���밳���� MAX_VS_BONE���϶� �ѹ��� �� �����Ѵ�.
		{
			int		nMaxBone = m_pBone->m_nMaxBone;

			if( nMaxBone > MAX_VS_BONE )	
				FLERROR_LOG( PROGRAM_NAME, _T( "overflow bone count - %d" ), nMaxBone );

			for( i = 0; i < nMaxBone; i ++ )	// MAX_VS_BONE�� ����	
			{
				mWorldTranspose = pmBonesInv[i] * pmBones[i];				
#ifdef	__YENV		
				CString str;
				str.Format( "mBoneMatrix[%d]", i );
				HRESULT hr = g_Neuz.m_pEffect->SetMatrix( str, &mWorldTranspose );
#else //__YENV
				D3DXMatrixTranspose( &mWorldTranspose, &mWorldTranspose );		// ��Ʈ������ ��������.
				m_pd3dDevice->SetVertexShaderConstantF( i * 3, (float*)&mWorldTranspose, 3 );		// ����������Ϳ� �������.
#endif //__YENV		
			}
		}
		D3DXMATRIX	mView, mProj;
		D3DXMATRIX	mViewProj, mViewProjTranspose, mInvWorld;

		D3DXVECTOR4 vLight = s_vLight;
		D3DXVECTOR4 vLightPos = s_vLightPos;

		mViewProj = *mWorld * s_mView * s_mProj;
		
		D3DXMatrixTranspose( &mViewProjTranspose, &mViewProj );
		D3DXMatrixTranspose( &mWorldTranspose, mWorld );

		D3DXMatrixInverse( &mInvWorld, NULL, mWorld );
		D3DXVec4Transform( &vLight, &vLight, &mInvWorld );
		D3DXVec4Normalize( &vLight, &vLight );
		D3DXVec4Transform( &vLightPos, &vLightPos, &mInvWorld );

#ifdef __YENV
		/*
		// ���� ����...
		g_Neuz.m_pEffect->SetMatrix( g_Neuz.m_hmWVP, &mViewProjTranspose );
		
		// ����Ʈ ��ġ ����
		D3DXVECTOR4 v;
		D3DXVECTOR4 vLight_Pos = s_vLight;
		D3DXMATRIX mLocal;
		D3DXMatrixInverse( &mLocal, NULL, &mViewProjTranspose );						
		D3DXVec4Transform( &v, &vLight_Pos, &mLocal );						// ������ǥ�� ��ȯ
		D3DXVec3Normalize( (D3DXVECTOR3*)&v, (D3DXVECTOR3*)&v );			// ����ȭ
		v.w = -0.6f;														// ȯ�汤�� ���(Ambint) Def : -0.3f
		
		// ����Ʈ ���� ����
		g_Neuz.m_pEffect->SetVector( g_Neuz.m_hvLightDir, &v );
		
		g_Neuz.m_pEffect->SetVector( g_Neuz.m_hvDiffuse, (D3DXVECTOR4*)&s_fDiffuse[0] );	
		g_Neuz.m_pEffect->SetVector( g_Neuz.m_hvAmbient, (D3DXVECTOR4*)&s_fAmbient[0] );
		*/
#else //__YENV
		m_pd3dDevice->SetVertexShaderConstantF( 84, (float*)&mViewProjTranspose, 4 );
//		m_pd3dDevice->SetVertexShaderConstantF( 88, (float*)&mWorldTranspose, 4 );
//		m_pd3dDevice->SetVertexShaderConstantF( 88, (float*)&vEyePos,  1 );		// specular use
//		m_pd3dDevice->SetVertexShaderConstantF( 89, (float*)&fSpecular, 1 );	// specular use
//		m_pd3dDevice->SetVertexShaderConstantF( 90, (float*)&fLightCol, 1 );	// specular use
		m_pd3dDevice->SetVertexShaderConstantF( 91, (float*)&vLightPos, 1 );
		m_pd3dDevice->SetVertexShaderConstantF( 92, (float*)&vLight,   1 );
		m_pd3dDevice->SetVertexShaderConstantF( 93, (float*)&s_fDiffuse, 1 );
		m_pd3dDevice->SetVertexShaderConstantF( 94, (float*)&s_fAmbient, 1 );
//		m_pd3dDevice->SetVertexShaderConstant( 95, &vConst, 1 );
#endif //__YENV
	}

	if( m_nNoEffect == 0 )
	{
		// �⺻ ���� 
		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
		pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL   );		
		pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA  );
		pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		
	}
	DWORD dwBlendFactor = m_dwColor | ( m_dwBlendFactor << 24 );
	// ������Ʈ�� ������ ȿ�� ���� 
	if( m_dwBlendFactor < 255 || m_dwColor )
	{
		pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR, dwBlendFactor );
		pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );

		pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR );
		pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
	}

	nNextFrame = GetNextFrame();
	pd3dDevice->SetMaterial( g_TextureMng.GetMaterial( pd3dDevice, 0 ) );

	D3DXVECTOR3 vec3LightBackup = D3DXVECTOR3( s_vLight[0], s_vLight[1], s_vLight[2] );
	D3DXVECTOR4 vec4Diffuse = D3DXVECTOR4( s_fDiffuse[0], s_fDiffuse[1], s_fDiffuse[2], s_fDiffuse[3] );;
	
	// ������Ʈ�� ��Ų,�Ϲ�,���� ������� ��ƼŬ������ ���Եɼ��ִ�.
	for( i = 0; i < MAX_ELEMENT; i ++ )
	{
		pElem = &m_Element[i];		

		pObject3D = pElem->m_pObject3D;
		if( pObject3D == NULL )		
			continue;

		if( pElem->m_nEffect & XE_HIDE )	
			continue;

		pObject3D->m_nNoTexture = 0; // m_nNoTexture;
		pObject3D->m_nNoEffect = m_nNoEffect;
		// ���뿡 ��ũ�Ǿ� �ִ�.
		if( pElem->m_nParentIdx != -1 )		// ���� �ε����� ���Ѵ�.
		{
			// ���� ���� ������ �ٲ���ٸ� ParentIdx�� Ʋ������ ������ �ٲ���� �Ѵ�.
			m1 = m_mUpdateBone[ pElem->m_nParentIdx ] * *mWorld;		// ���뿡 ��ũ�Ǿ� �ִٸ� ������ġ�� ���Ͱ� �ȴ�.
			if( pElem->m_nParentIdx == GetRHandIdx() )
				m1 = pElem->m_mLocalRH * m1;
			else if( pElem->m_nParentIdx == GetLHandIdx() )
				m1 = pElem->m_mLocalLH * m1;
			else if( pElem->m_nParentIdx == GetLArmIdx() )
				m1 = pElem->m_mLocalShield * m1;
			else if( pElem->m_nParentIdx == GetRArmIdx() )
				m1 = pElem->m_mLocalKnuckle * m1;
		}
		else
			m1 = *mWorld;

		if( pElem->m_nPartsIdx == PARTS_HAIR )
		{
			//gmpbigsun : 
			memcpy( g_fDiffuse, s_fDiffuse, sizeof(FLOAT)*4 );
			memcpy( g_fAmbient, s_fAmbient, sizeof(FLOAT)*4 );

#ifndef __YENV
			SetDiffuse( 0, 0, 0 );
#endif //__YENV
			
			if( s_bLight )
			{
				float fblack = (CWorld::m_light.Diffuse.r*g_fHairLight) + (CWorld::m_light.Diffuse.g*g_fHairLight) + (CWorld::m_light.Diffuse.b*g_fHairLight);
				fblack = fblack / 3.0f;
				fblack *= 0.6f;

				if( fblack < 0.3f )		
					fblack = 0.3f;

				SetAmbient( min( pObject3D->m_fAmbient[0]*fblack, 1.0f ), min( pObject3D->m_fAmbient[1]*fblack, 1.0f ), min( pObject3D->m_fAmbient[2]*fblack, 1.0f ) );
			}
			else
				SetAmbient( pObject3D->m_fAmbient[0], pObject3D->m_fAmbient[1], pObject3D->m_fAmbient[2] );
			
#ifdef __YENV
			SetDiffuse( pObject3D->m_fAmbient[0], pObject3D->m_fAmbient[1], pObject3D->m_fAmbient[2] );
#endif //__YENV
				
#ifdef __YENV
			g_Neuz.m_pEffect->SetVector( g_Neuz.m_hvDiffuse, (D3DXVECTOR4*)&s_fDiffuse[0] );	
			g_Neuz.m_pEffect->SetVector( g_Neuz.m_hvAmbient, (D3DXVECTOR4*)&s_fAmbient[0] );			
#else //__YENV
			pd3dDevice->SetVertexShaderConstantF( 93, (float*)&s_fDiffuse, 1 );
			pd3dDevice->SetVertexShaderConstantF( 94, (float*)&s_fAmbient, 1 );
#endif //__YENV
		} else
		if( pElem->m_nPartsIdx == PARTS_CLOAK )
		{
			g_ModelGlobal.SetTexture( m_pCloakTexture );		// �ܺ����� �����ؽ��İ� �ִٸ�.
		}

		if( m_pBone )
			pObject3D->SetExternBone( m_mUpdateBone, m_pBaseBoneInv );	// ���庻�� �ִٸ� �װ��� �Ѱ��ش�.
		
		if( m_SparkInfo.m_bUsed )
		{
			SetLightVec( m_SparkInfo.m_v3SparkDir );
			
			D3DXVECTOR3 vec3Diffuse;
			D3DXVec3Lerp( &vec3Diffuse, &D3DXVECTOR3(0.0f,0.0f,0.0f), &m_SparkInfo.m_v3Color, m_SparkInfo.m_fLerp );
			SetDiffuse( vec3Diffuse.x, vec3Diffuse.y, vec3Diffuse.z );
		}
		
		pObject3D->Render( pd3dDevice, pElem->m_ppd3d_VBSel, m_fFrameCurrent, nNextFrame, &m1, pElem->m_nEffect, dwBlendFactor );

		pObject3D->m_nNoEffect = 0;

		if( pElem->m_nPartsIdx == PARTS_HAIR )
		{
			memcpy( s_fDiffuse, g_fDiffuse, sizeof(FLOAT)*4 );
			memcpy( s_fAmbient, g_fAmbient, sizeof(FLOAT)*4 );
			
#ifdef __YENV
			g_Neuz.m_pEffect->SetVector( g_Neuz.m_hvDiffuse, (D3DXVECTOR4*)&s_fDiffuse[0] );	
			g_Neuz.m_pEffect->SetVector( g_Neuz.m_hvAmbient, (D3DXVECTOR4*)&s_fAmbient[0] );			
#else //__YENV		
			pd3dDevice->SetVertexShaderConstantF( 93, (float*)&s_fDiffuse, 1 );
			pd3dDevice->SetVertexShaderConstantF( 94, (float*)&s_fAmbient, 1 );
#endif //__YENV
		} else

		if( pElem->m_nPartsIdx == PARTS_CLOAK )
			g_ModelGlobal.SetTexture( NULL );
	}

#ifdef __ATTACH_MODEL
	RenderAttachModel(pd3dDevice, mWorld);
#endif //__ATTACH_MODEL

	// ���� ����
	if( m_dwBlendFactor < 255 || m_dwColor )
	{
		if( m_nNoEffect == 0 )
		{
			pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB( 255, 0, 0, 0) );
			pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
			pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
			pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
		}
	}

	SetLightVec( vec3LightBackup );
	SetDiffuse( vec4Diffuse.x, vec4Diffuse.y, vec4Diffuse.z );

	return 1;
}

void	CModelObject::RenderEffect( LPDIRECT3DDEVICE9 pd3dDevice, const D3DXMATRIX *mWorld, DWORD dwItemKind3, int nLevelL, int nLeveR, int nType )
{

#ifdef _DEBUG
//	RenderItemEffect_TEST( mWorld );		//�ش����� ���߿Ϸ�Ǿ� �ּ�ó����, �׽�Ʈ�� ���� �ڵ�� �����
#endif

#ifdef __SFX_OPT
	if(g_Option.m_nSfxLevel <= 0 && dwItemKind3 != IK3_YOYO) return;
#endif

	O3D_ELEMENT	*pElem;
	int	i;
	BOOL bYoYoAttakLEquip = FALSE;	//sun: 10, __CSC_ENCHANT_EFFECT_2
	if( m_nNoEffect == 0 )
	{
		// �˱� ����.
		if( m_pForce && m_pForce->m_nMaxSpline > 0 )	m_pForce->Draw( pd3dDevice, mWorld );
		if( m_pForce2 && m_pForce2->m_nMaxSpline > 0 )	m_pForce2->Draw( pd3dDevice, mWorld );

		if( CObj::GetActiveObj() )
		{
			int nEffect, nLevel, nLevel2;
			nLevel2 = nLeveR;

			pElem = GetParts( PARTS_RWEAPON );

#ifdef __SFX_OPT
			if( g_Option.m_nSfxLevel > 0 && pElem && (pElem->m_nEffect & XE_HIDE) == 0 )			// ���̵尡 �ƴҶ��� ����ؾ���
#else //__SFX_OPT
			if( pElem && (pElem->m_nEffect & XE_HIDE) == 0 )			// ���̵尡 �ƴҶ��� ����ؾ���
#endif //__SFX_OPT
			{
				nEffect = pElem->m_nEffect & 0x00FFFFFF;	// ���� 24��Ʈ, �Ӽ��� ����
				nLevel = (pElem->m_nEffect >> 24);			// ���� 8��Ʈ, �ϹݼӼ� ����
				if( nEffect & XE_ITEM_FIRE )
				{
					CreateParticleAndRender( PARTS_RWEAPON, mWorld, PE_FIRE, nLevel, nLevel2, dwItemKind3, nType );
				}
				else
				if( nEffect & XE_ITEM_ELEC )
				{
					CreateParticleAndRender( PARTS_RWEAPON, mWorld, PE_ELEC, nLevel, nLevel2, dwItemKind3, nType );
					RenderItemElec_Adv( PARTS_RWEAPON, mWorld, nLevel2 );	//sun: 10, __CSC_ENCHANT_EFFECT_2
				}
				else
				if( nEffect & XE_ITEM_WATER )
				{
					CreateParticleAndRender( PARTS_RWEAPON, mWorld, PE_WATER, nLevel, nLevel2, dwItemKind3, nType );
				}
				else
				if( nEffect & XE_ITEM_WIND )
				{
					CreateParticleAndRender( PARTS_RWEAPON, mWorld, PE_WIND, nLevel, nLevel2, dwItemKind3, nType );
				}
				else
				if( nEffect & XE_ITEM_EARTH )
				{
					CreateParticleAndRender( PARTS_RWEAPON, mWorld, PE_EARTH, nLevel, nLevel2, dwItemKind3, nType );
				}
				else
				if( nEffect & XE_ITEM_GEN )
				{
					CreateParticleAndRender( PARTS_RWEAPON, mWorld, PE_GEN, nLevel, nLevel2, dwItemKind3, nType );
				}
				else
				if( nEffect & XE_ITEM_GEN_AL )
				{
					CreateParticleAndRender( PARTS_RWEAPON, mWorld, PE_GEN_AL, nLevel, nLevel2, dwItemKind3, nType );
				}
				else
				if( nEffect & XE_ITEM_FIRE_AL )
				{
					CreateParticleAndRender( PARTS_RWEAPON, mWorld, PE_FIRE_AL, nLevel, nLevel2, dwItemKind3, nType );
				}
				else
				if( nEffect & XE_ITEM_ELEC_AL )
				{
					CreateParticleAndRender( PARTS_RWEAPON, mWorld, PE_ELEC_AL, nLevel, nLevel2, dwItemKind3, nType );
					RenderItemElec_Adv( PARTS_RWEAPON, mWorld, nLevel2 );	//sun: 10, __CSC_ENCHANT_EFFECT_2
				}
				else
				if( nEffect & XE_ITEM_WATER_AL )
				{
					CreateParticleAndRender( PARTS_RWEAPON, mWorld, PE_WATER_AL, nLevel, nLevel2, dwItemKind3, nType );
				}
				else
				if( nEffect & XE_ITEM_WIND_AL )
				{
					CreateParticleAndRender( PARTS_RWEAPON, mWorld, PE_WIND_AL, nLevel, nLevel2, dwItemKind3, nType );
				}
				else
				if( nEffect & XE_ITEM_EARTH_AL )
				{
					CreateParticleAndRender( PARTS_RWEAPON, mWorld, PE_EARTH_AL, nLevel, nLevel2, dwItemKind3, nType );
				}
			}

			if( dwItemKind3 != IK3_YOYO )
				pElem = GetParts( PARTS_LWEAPON );
			else //��� �� ��� �⺻������ ������ ������ ������ ��� �� ���ݽô� �ٸ��� ����
			{				
				O3D_ELEMENT	*pRweaponElem = GetParts( PARTS_RWEAPON );
				O3D_ELEMENT	*pLweaponElem = GetParts( PARTS_LWEAPON );
				if( (pRweaponElem && (pRweaponElem->m_nEffect & XE_HIDE) == 0 ) && //���ʿ� �� �����ϰ� ���� ���
					(pLweaponElem && (pLweaponElem->m_nEffect & XE_HIDE) == 0 ) )
					pElem = GetParts( PARTS_RWEAPON );
				else if( (pRweaponElem && (pRweaponElem->m_nEffect & XE_HIDE) == 0 ) && //�����ʸ� �����ϰ� ���� ���
					(pLweaponElem && (pLweaponElem->m_nEffect & XE_HIDE) != 0 ) )
					pElem = GetParts( PARTS_LWEAPON );
				else if( (pRweaponElem && (pRweaponElem->m_nEffect & XE_HIDE) != 0 ) && //���ʸ� �����ϰ� ���� ���
					(pLweaponElem && (pLweaponElem->m_nEffect & XE_HIDE) == 0 ) )
				{
					pElem = GetParts( PARTS_RWEAPON );
					bYoYoAttakLEquip = TRUE; //������ ������ ������ �����ϵ� �Ʒ� XE_HIDE�� �н��ϱ� ���� �ӽ÷� ���
				}
			}
#ifdef __SFX_OPT
			if( g_Option.m_nSfxLevel > 0 && pElem && (bYoYoAttakLEquip || (pElem->m_nEffect & XE_HIDE) == 0) )			// ���̵尡 �ƴҶ��� ����ؾ���
#else //__SFX_OPT
			if( pElem && (bYoYoAttakLEquip || (pElem->m_nEffect & XE_HIDE) == 0) )			// ���̵尡 �ƴҶ��� ����ؾ���
#endif //__SFX_OPT
			{
				nEffect = pElem->m_nEffect & 0x00FFFFFF;	// ���� 24��Ʈ
				nLevel = (pElem->m_nEffect >> 24);			// ���� 8��Ʈ
				
			//sun: 10, __CSC_ENCHANT_EFFECT_2
				if( dwItemKind3 != IK3_YOYO ) //���� ���� ��ġ ���� ��� ����Ʈ�� �����ϰ� ����.
					nLevel2 = nLevelL;
		
				if( nEffect & XE_ITEM_FIRE )
				{
					CreateParticleAndRender( PARTS_LWEAPON, mWorld, PE_FIRE, nLevel, nLevel2, dwItemKind3, nType );
				}
				else
				if( nEffect & XE_ITEM_ELEC )
				{
					CreateParticleAndRender( PARTS_LWEAPON, mWorld, PE_ELEC, nLevel, nLevel2, dwItemKind3, nType );
					RenderItemElec_Adv( PARTS_LWEAPON, mWorld, nLevel2 );	//sun: 10, __CSC_ENCHANT_EFFECT_2			
				}
				else
				if( nEffect & XE_ITEM_WATER )
				{
					CreateParticleAndRender( PARTS_LWEAPON, mWorld, PE_WATER, nLevel, nLevel2, dwItemKind3, nType );
				}
				else
				if( nEffect & XE_ITEM_WIND )
				{
					CreateParticleAndRender( PARTS_LWEAPON, mWorld, PE_WIND, nLevel, nLevel2, dwItemKind3, nType );
				}
				else
				if( nEffect & XE_ITEM_EARTH )
				{
					CreateParticleAndRender( PARTS_LWEAPON, mWorld, PE_EARTH, nLevel, nLevel2, dwItemKind3, nType );
				}
				else
				if( nEffect & XE_ITEM_GEN )
				{
					CreateParticleAndRender( PARTS_LWEAPON, mWorld, PE_GEN, nLevel, nLevel2, dwItemKind3, nType );
				}
				else
				if( nEffect & XE_ITEM_GEN_AL )
				{
					CreateParticleAndRender( PARTS_LWEAPON, mWorld, PE_GEN_AL, nLevel, nLevel2, dwItemKind3, nType );
				}
				else
				if( nEffect & XE_ITEM_FIRE_AL )
				{
					CreateParticleAndRender( PARTS_LWEAPON, mWorld, PE_FIRE_AL, nLevel, nLevel2, dwItemKind3, nType );
				}
				else
				if( nEffect & XE_ITEM_ELEC_AL )
				{
					CreateParticleAndRender( PARTS_LWEAPON, mWorld, PE_ELEC_AL, nLevel, nLevel2, dwItemKind3, nType );
					RenderItemElec_Adv( PARTS_LWEAPON, mWorld, nLevel2 );	//sun: 10, __CSC_ENCHANT_EFFECT_2
				}
				else
				if( nEffect & XE_ITEM_WATER_AL )
				{
					CreateParticleAndRender( PARTS_LWEAPON, mWorld, PE_WATER_AL, nLevel, nLevel2, dwItemKind3, nType );
				}
				else
				if( nEffect & XE_ITEM_WIND_AL )
				{
					CreateParticleAndRender( PARTS_LWEAPON, mWorld, PE_WIND_AL, nLevel, nLevel2, dwItemKind3, nType );
				}
				else
				if( nEffect & XE_ITEM_EARTH_AL )
				{
					CreateParticleAndRender( PARTS_LWEAPON, mWorld, PE_EARTH_AL, nLevel, nLevel2, dwItemKind3, nType );
				}
			
			}
		}
		
		for( i = 0; i < MAX_ELEMENT; i ++ )
		{
			pElem = &m_Element[i];
			pElem->m_nEffect = 0;		// ������ �ʱ�ȭ ������
		}
	}
}
#endif// CLIENT


//
HRESULT CModelObject::InitDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice )
{ 
	m_pd3dDevice = pd3dDevice;

#ifdef __ATTACH_MODEL
	InitAttachModelDeviceObjects(pd3dDevice);
#endif //__ATTACH_MODEL
	return S_OK; 
}

HRESULT CModelObject::RestoreDeviceObjects()
{
	int		i;
	O3D_ELEMENT	*pElem;
	CObject3D	*pObject3D;

	SetGroup(0);
	for( i = 0; i < MAX_ELEMENT; i ++ )
	{
		pElem = &m_Element[i];
		pObject3D = pElem->m_pObject3D;
		if( pObject3D )
		{
			// pElem�� ��Ų�϶� m_ppd3d_VB[]�� ���ؽ����۰� ��ܼ� ���ƿ´�.
			pObject3D->RestoreDeviceObjects( pElem->m_ppd3d_VB );
		}
	}
	
	if( m_pPartsEffect )
		m_pPartsEffect->RestoreDeviceObjects( m_pd3dDevice );
	if( m_pPartsEffect2 )
		m_pPartsEffect2->RestoreDeviceObjects( m_pd3dDevice );

//sun: ���� ����Ʈ ��� ����
	if( m_pPartsEffect1_Detail )
		m_pPartsEffect1_Detail->RestoreDeviceObjects( m_pd3dDevice );
	if( m_pPartsEffect2_Detail )
		m_pPartsEffect2_Detail->RestoreDeviceObjects( m_pd3dDevice );

#ifdef __ATTACH_MODEL
	RestoreAttachModelDeviceObjects();
#endif //__ATTACH_MODEL

	return S_OK;
}	
HRESULT CModelObject::InvalidateDeviceObjects()
{
	DeleteDeviceObjects();

	if( m_pPartsEffect )
		m_pPartsEffect->InvalidateDeviceObjects( m_pd3dDevice );
	if( m_pPartsEffect2 )
		m_pPartsEffect2->InvalidateDeviceObjects( m_pd3dDevice );

//sun: ���� ����Ʈ ��� ����
	if( m_pPartsEffect1_Detail )
		m_pPartsEffect1_Detail->InvalidateDeviceObjects( m_pd3dDevice );
	if( m_pPartsEffect2_Detail )
		m_pPartsEffect2_Detail->InvalidateDeviceObjects( m_pd3dDevice );

	return  S_OK;
}	
HRESULT CModelObject::DeleteDeviceObjects()
{
	int		i, j;
	O3D_ELEMENT	*pElem;
	CObject3D				*pObject3D;

	for( i = 0; i < MAX_ELEMENT; i ++ )
	{
		pElem = &m_Element[i];
		pObject3D = pElem->m_pObject3D;
		if( pObject3D )
		{
			if( pElem->m_ppd3d_VB )
			{
				pElem->m_pObject3D->SetLOD(0);
				for( j = 0; j < pElem->m_pObject3D->GetMaxObject(); j ++ )		// 
					SAFE_RELEASE( pElem->m_ppd3d_VB1[j] );
				pElem->m_pObject3D->SetLOD(1);
				for( j = 0; j < pElem->m_pObject3D->GetMaxObject(); j ++ )
					SAFE_RELEASE( pElem->m_ppd3d_VB2[j] );
				pElem->m_pObject3D->SetLOD(2);
				for( j = 0; j < pElem->m_pObject3D->GetMaxObject(); j ++ )
					SAFE_RELEASE( pElem->m_ppd3d_VB3[j] );
				pElem->m_pObject3D->SetLOD(0);
	//				SAFE_DELETE_ARRAY( pElem->m_ppd3d_VB );	// ���⼭ �̰� �����ϸ� �ȵȴ�. device �ڿ��� ����.
			}
//			pObject3D->DeleteDeviceObjects();
		}
	}

#ifdef __ATTACH_MODEL
	DeleteAttachModelDeviceObjects();
#endif //__ATTACH_MODEL

	return  S_OK;
}	

void	CModelObject::FrameMove( D3DXVECTOR3 *pvSndPos, float fSpeed )
{
#ifndef __CLIENT
	UNREFERENCED_PARAMETER( pvSndPos );
#endif

#ifdef __CLIENT
	if( m_pMotion )		// CModel::FrameMove���� �������� �����Ǳ����� �˻��غ��� �Ѵ�.
	{
//sun: 9, //__AI_0509
		MOTION_ATTR* pAttr	= IsAttrSound();
		if( pAttr && pAttr->m_nSndID > 0 && m_nPause == 0 )
			PLAYSND( pAttr->m_nSndID, pvSndPos );

		int nMaxFrame = GetMaxFrame( );
		if( nMaxFrame > 0 )
		{
			int i	= ( m_nFrameOld + 1 ) % nMaxFrame;
			int nFrame	= ( static_cast<int>( m_fFrameCurrent ) + 1 ) % nMaxFrame;
			while( i != nFrame )
			{
				BOOL bAttr	= FALSE;
				/*
				MOTION_ATTR* pAttr	= IsAttrSound( i );
				if( pAttr && pAttr->m_nSndID > 0 && m_nPause == 0 )
				{
					PLAYSND( pAttr->m_nSndID, pvSndPos );
					bAttr	= TRUE;
				}
				*/
				BOOL bQuake	= IsAttrQuake( (float)( i ) );
				if( bQuake && m_nPause == 0 )
				{
					CMover *pMover	= CMover::GetActiveMover();
					if( pMover )
					{
						PT_ITEM_SPEC pItemProp	= pMover->GetActiveHandItemProp();
						if( pItemProp && pItemProp->dwItemKind3 != IK3_YOYO && !pMover->IsActiveMover() )
						{
							pMover->GetWorld()->m_pCamera->SetQuake( 15 );				
							bAttr	= TRUE;
						}
					}
				}
				if( bAttr )
					break;
				i	= ( i + 1 ) % nMaxFrame;
			}
		}

	} else
	{
		CObject3D *pObject = GetObject3D();
//sun: 9,	//__AI_0509
		if( pObject->GetMaxFrame() )
		{
			MOTION_ATTR* pAttr	= pObject->IsAttrSound( m_fFrameCurrent );
			if( pAttr && pAttr->m_nSndID > 0 && m_nPause == 0 )
				PLAYSND( pAttr->m_nSndID, pvSndPos );

			int i	= ( m_nFrameOld + 1 ) % pObject->GetMaxFrame();
			int nFrame	= ( static_cast<int>( m_fFrameCurrent ) + 1 ) % pObject->GetMaxFrame();
			while( i != nFrame )
			{
				BOOL bAttr	= FALSE;
				/*
				MOTION_ATTR* pAttr	= pObject->IsAttrSound( i );
				if( pAttr && pAttr->m_nSndID > 0 && m_nPause == 0 )
				{
					PLAYSND( pAttr->m_nSndID, pvSndPos );
					bAttr	= TRUE;
				}
				*/
				BOOL bQuake	= pObject->IsAttrQuake( (float)( i ) );
				if( bQuake && m_nPause == 0 )
				{
					CMover *pMover	= CMover::GetActiveMover();
					if( pMover && m_nPause == 0 )
					{
						pMover->GetWorld()->m_pCamera->SetQuake( 30, 1.0f );
						bAttr	= TRUE;
					}
				}
				if( bAttr )
					break;

				int nMaxFrame = GetMaxFrame( );
				if( nMaxFrame > 0 )
					i	= ( i + 1 ) % nMaxFrame;
				else 
					break;
			}
		}
	}
#endif
	
	CModel::FrameMove( NULL, fSpeed );

#if defined(__CPU_UTILDOWN_060502) && defined(__WORLDSERVER)
		return;
#else	//defined(__CPU_UTILDOWN_060502) && defined(__WORLDSERVER)	

	if( m_pForce )
	{
		if( /*!m_bEndFrame &&*/ m_pForce->m_nMaxSpline && !m_nPause )
		{
			m_pForce->m_nMaxDraw += (int)((MAX_SF_SLERP*2+2) /** fSpeed*/);
			if( m_pForce->m_nMaxDraw > m_pForce->m_nMaxSpline )
			{
				if( m_nLoop & ANILOOP_LOOP )	// ���εǴ� �ִ��ϰ��� ÷���� �ٽ� �׸���.
					m_pForce->m_nMaxDraw = 0;
				else
					m_pForce->m_nMaxDraw = m_pForce->m_nMaxSpline;
			}
				
		}

		m_pForce->Process();
	}

	if( m_pForce2 )
	{
		if( m_pForce2->m_nMaxSpline && !m_nPause )
		{
			m_pForce2->m_nMaxDraw += (int)((MAX_SF_SLERP*2+2) /** fSpeed*/);
			if( m_pForce2->m_nMaxDraw > m_pForce2->m_nMaxSpline )
			{
				if( m_nLoop & ANILOOP_LOOP )	// ���εǴ� �ִ��ϰ��� ÷���� �ٽ� �׸���.
					m_pForce2->m_nMaxDraw = 0;
				else
					m_pForce2->m_nMaxDraw = m_pForce2->m_nMaxSpline;
			}
			
		}
		
		m_pForce2->Process();
	}
	
	if( m_bMotionBlending )
	{
		m_fBlendWeight += 0.05f;
		if( m_fBlendWeight >= 1.0f )		// ������ 1.0���� �� ��������� ���� ����
			SetMotionBlending( FALSE );
	}

#ifdef __ATTACH_MODEL
	FrameMoveAttachModel();
#endif //__ATTACH_MODEL

#endif	//defined(__CPU_UTILDOWN_060502) && defined(__WORLDSERVER)	
}

//
//	�˱��� ����
//	m_pMotion�� ù������~�����������ӱ����� �˱��� �����Ѵ�.
#ifdef __CLIENT
void CModelObject::MakeSWDForce( int nParts, DWORD dwItemKind3, BOOL bSlow, DWORD dwColor, float fSpeed )
{

	D3DXVECTOR3		v1( 0.0f, 0.0f, 0.0f ),
					v2( 0.0f, 0.0f, 0.0f );

	O3D_ELEMENT*	pElem = GetParts( nParts );		// ������ ������ ������
	
	if( NULL == pElem )
		return ;

	D3DXMATRIX		m1;
	D3DXMatrixIdentity( &m1 );

	FLERROR_ASSERT_LOG_RETURN_VOID( pElem->m_pObject3D, _T("[�� ������Ʈ] 3D ������Ʈ �����Ͱ� ��ȿ���� �ʽ��ϴ�.") );

	if( pElem->m_pObject3D->m_vForce1.x == 0 )	
		return;

	if( pElem->m_pObject3D->m_vForce2.x == 0 )	
		return;

	// FrameCurrent�� 0����
	m_fFrameCurrent =	0.0f;
	m_nPause =			0;
	m_bEndFrame =		FALSE;
	int nLoop =			m_nLoop;
	m_nLoop =			ANILOOP_1PLAY;
	
	CreateForce( nParts );	// �˱� ������Ʈ ����
	CSwordForce* pForce = NULL;

	if( nParts == PARTS_RWEAPON )
		pForce = m_pForce;
	else
		pForce = m_pForce2;

	if( NULL == pForce )
		return;

	pForce->Clear();
	pForce->m_dwColor = dwColor;

	D3DXMATRIX *pmLocal( NULL );
	if( pElem->m_nParentIdx == GetRHandIdx() )
		pmLocal = &pElem->m_mLocalRH;
	else if( pElem->m_nParentIdx == GetLHandIdx() )
		pmLocal = &pElem->m_mLocalLH;
	else if( pElem->m_nParentIdx == GetLArmIdx() )
		pmLocal = &pElem->m_mLocalShield;
	else if( pElem->m_nParentIdx == GetRArmIdx() )
		pmLocal = &pElem->m_mLocalKnuckle;
	else
		FLERROR_ASSERT_LOG_RETURN_VOID( pmLocal, _T("[�� ������Ʈ] �θ� �ε����� ã�� �� �����ϴ�.") );

	while(1)
	{
		if( m_pMotion )
			m_pMotion->AnimateBone( m_mUpdateBone, m_pMotionOld, m_fFrameCurrent, GetNextFrame(), m_nFrameOld, m_bMotionBlending, m_fBlendWeight );		// �ϴ� ���밡 �ִٸ� ���� �ִϸ��̼� ��Ŵ

		// ���� WorldTM = ����LocalTM X ����θ�WorldTM  
		D3DXMatrixMultiply( &m1, pmLocal, &m_mUpdateBone[ pElem->m_nParentIdx ] );
		
		// Į�� ���ؽ��� ���� ��ǥ ���
		D3DXVec3TransformCoord( &v1, &(pElem->m_pObject3D->m_vForce1), &m1 );
		D3DXVec3TransformCoord( &v2, &(pElem->m_pObject3D->m_vForce2), &m1 );
		pForce->Add( v1, v2 );

		CModel::FrameMove( NULL, fSpeed );
		if( bSlow && IsAttrHit( m_fFrameCurrent ) )
			m_bSlow = TRUE;
		if( m_bEndFrame )
			break;
	}

	pForce->MakeSpline();		// ���ö��� ����
	
	// rewind
	m_fFrameCurrent = 0.0f;
	m_nPause = 0;
	m_bEndFrame = FALSE;
	m_nLoop = nLoop;
	m_bSlow = FALSE;
}
#else
void	CModelObject::MakeSWDForce( int /*nParts*/, DWORD /*dwItemKind3*/, BOOL /*bSlow*/, DWORD /*dwColor*/, float /*fSpeed*/ ) {}
#endif


#ifdef __CLIENT
void	CModelObject::CreateParticle( int nParts, const D3DXMATRIX *pmWorld, int nType, int nLevel, int nLevel2 )
{
//sun: 10, __CSC_ENCHANT_EFFECT_2
	int nEffLevel_2 = 0;
	DWORD dwSfx_2 = 0;

//sun:13, ���� Ȯ��(�Ӽ�, �Ϲ�)
	if( nLevel2 >= 10 )
		nLevel2 = 10;

	if( nLevel2 == 10 )		nEffLevel_2 = 5;
	else if( nLevel2 == 9 )	nEffLevel_2 = 4;
	else if( nLevel2 == 8 )	nEffLevel_2 = 3;
	else if( nLevel2 == 7 )	nEffLevel_2 = 2;
	else if( nLevel2 >= 4 )	nEffLevel_2 = 1;	// 4 ~ 6
	else 
		nEffLevel_2 = 0;

	if(nType == PE_GEN && (nEffLevel_2 == 0 && nLevel < 6)) return;
	if(nType <= PE_EARTH && (nEffLevel_2 == 0 && nLevel < 6)) return; 
	
	O3D_ELEMENT		*pElem = GetParts( nParts );		// ������ ������ ������

	if( !pElem )
		return;
	
	if( pElem->m_pObject3D == NULL )		return;
	
	if( pElem->m_pObject3D->m_vForce1.x == 0 )	return;
	if( pElem->m_pObject3D->m_vForce2.x == 0 )	return;

//sun: 9�� ���ð��� (�߰� �˱� ���)
	BOOL IsSecondLine = FALSE;
	D3DXVECTOR3 v2_1( 0.0F, 0.0F, 0.0F );
	D3DXVECTOR3 v2_2( 0.0F, 0.0F, 0.0F );
	D3DXVECTOR3 v2_3( 0.0F, 0.0F, 0.0F );
	if( pElem->m_pObject3D->m_vForce3.x != 0 && pElem->m_pObject3D->m_vForce4.x != 0 )
		IsSecondLine = TRUE;

	if(IsSecondLine)
	{
		v2_1 = pElem->m_pObject3D->m_vForce3;
		v2_2 = pElem->m_pObject3D->m_vForce4;
	}

	D3DXMATRIX m1;
	D3DXMatrixIdentity( &m1 );
	D3DXMATRIX m2;
	D3DXMatrixIdentity( &m2 );
	D3DXVECTOR3 v1( 0.0F, 0.0F, 0.0F );
	D3DXVECTOR3 v2( 0.0F, 0.0F, 0.0F );
	D3DXVECTOR3 v3( 0.0F, 0.0F, 0.0F );

	float		fTemp = 0.0f;		//sun:13, ���� Ȯ��(�Ӽ�, �Ϲ�)

	D3DXMATRIX *pmLocal = NULL;
	if( pElem->m_nParentIdx == GetRHandIdx() )
		pmLocal = &pElem->m_mLocalRH;
	else if( pElem->m_nParentIdx == GetLHandIdx() )
		pmLocal = &pElem->m_mLocalLH;
	else if( pElem->m_nParentIdx == GetLArmIdx() )
		pmLocal = &pElem->m_mLocalShield;
	else if( pElem->m_nParentIdx == GetRArmIdx() )
		pmLocal = &pElem->m_mLocalKnuckle;

	FLASSERT( pmLocal );
	D3DXMatrixMultiply( &m1, pmLocal, &m_mUpdateBone[ pElem->m_nParentIdx ] );
	m2 = m1 * *pmWorld;

	v1 = pElem->m_pObject3D->m_vForce1;
	v2 = pElem->m_pObject3D->m_vForce2;
	
//sun: ���� ����Ʈ ��� ����
	D3DXVECTOR3 vScale( 0.0F, 0.0F, 0.0F );
	int nEffCount = MAX_PARTS_FIRE;
#ifdef __SFX_OPT
	switch(g_Option.m_nSfxLevel)
	{
		case 1:
			nEffCount = MAX_PARTS_FIRE/3;
			break;
		case 2:
			nEffCount = MAX_PARTS_FIRE/2;
			break;
		case 3:
			nEffCount = (MAX_PARTS_FIRE/3) * 2;
			break;
		case 4:
			nEffCount = (MAX_PARTS_FIRE/5) * 4;
			break;
		case 5:
			nEffCount = MAX_PARTS_FIRE;
			break;
	};
#endif	//SFX_OPT

	if( m_nEffectCount < nEffCount )
	{
		fTemp = (FLOAT)m_nEffectCount / (FLOAT)nEffCount;

		D3DXVec3Lerp( &v3, &v1, &v2, fTemp );
//sun: 9�� ���ð��� (�߰� �˱� ���)
		if(IsSecondLine)
			D3DXVec3Lerp( &v2_3, &v2_1, &v2_2, fTemp );

		vScale = D3DXVECTOR3( 0.5f, 0.5f, 0.5f );

		if( fTemp < 0.4f || fTemp > 0.6f )
			vScale = D3DXVECTOR3( 1.1f, 1.1f, 1.1f );

		if( fTemp < 0.25f || fTemp > 0.75f )
			vScale = D3DXVECTOR3( 0.85f, 0.85f, 0.85f );

		if( fTemp < 0.1f || fTemp > 0.9f )
			vScale = D3DXVECTOR3( 0.6f, 0.6f, 0.6f );

		if( fTemp > 0.4f && fTemp < 0.6f )
			vScale = D3DXVECTOR3( 1.2f, 1.2f, 1.2f );

		m_nEffectCount++;
	}
	else
	{
		m_nEffectCount = 0;
	}

	CPartsFire *pFire = NULL;
	
	if( nParts == PARTS_LWEAPON )
	{
		if( m_pPartsEffect2 && m_pPartsEffect2->m_nType != nType )	// ����Ʈ�� �̹� �����Ǿ� �ְ�  ������ �ƴ϶��
			SAFE_DELETE( m_pPartsEffect2 );							// ����Ʈ �����ϰ� �ٽ� �Ҵ�.
		if( m_pPartsEffect2 == NULL )
			m_pPartsEffect2 = new CPartsFire( nType );
		
		pFire = (CPartsFire *)m_pPartsEffect2;
	} else
	{
		if( m_pPartsEffect && m_pPartsEffect->m_nType != nType )	// ����Ʈ�� �̹� �����Ǿ� �ְ�  ������ �ƴ϶��
			SAFE_DELETE( m_pPartsEffect );							// ����Ʈ �����ϰ� �ٽ� �Ҵ�.
		if( m_pPartsEffect == NULL )
			m_pPartsEffect = new CPartsFire( nType );
		
		pFire = (CPartsFire *)m_pPartsEffect;
	}
	if( (g_nRenderCnt & 3) == 0 )
	{
		FLOAT fScalLevel = 0.0f;

		DWORD dwSfx = 0;
		switch( nType )
		{
	//sun: ���� ����Ʈ ��� ����
			case PE_FIRE_AL : dwSfx = XI_INDEX( 1803, XI_NAT_FIRE01_ADV_AL ); break;
			case PE_ELEC_AL : dwSfx = XI_INDEX( 1843, XI_NAT_ELEC01_ADV_AL ); break;	// �̰�� �߻���...������������
			case PE_WATER_AL: dwSfx = XI_INDEX( 1813, XI_NAT_WATER01_ADV_AL ); break;
			case PE_WIND_AL : dwSfx = XI_INDEX( 1823, XI_NAT_WIND01_ADV_AL ); break;
			case PE_EARTH_AL: dwSfx = XI_INDEX( 1833, XI_NAT_EARTH01_ADV_AL ); break;
			case PE_FIRE : dwSfx = XI_INDEX( 1753, XI_NAT_FIRE01_ADV ); break;
			case PE_ELEC : dwSfx = XI_INDEX( 1793, XI_NAT_ELEC01_ADV ); break;		// �̰�� �߻���...������������
			case PE_WATER: dwSfx = XI_INDEX( 1763, XI_NAT_WATER01_ADV ); break;
			case PE_WIND : dwSfx = XI_INDEX( 1773, XI_NAT_WIND01_ADV ); break;
			case PE_EARTH: dwSfx = XI_INDEX( 1783, XI_NAT_EARTH01_ADV ); break;
			case PE_GEN_AL: dwSfx = XI_INDEX( 1853, XI_NAT_NONE01_ADV ); break;
			case PE_GEN: dwSfx = XI_INDEX( 1853, XI_NAT_NONE01_ADV ); break;
			default: dwSfx = XI_INDEX( 1753, XI_NAT_FIRE01_ADV );
		}

	//sun: ���� ����Ʈ ��� ����
		if(nLevel > 0)
		{
			switch( nType )
			{
				case PE_FIRE_AL:
				case PE_ELEC_AL:
				case PE_WATER_AL:
				case PE_WIND_AL:
				case PE_EARTH_AL:
				case PE_GEN_AL:
					fScalLevel = prj.m_fEnchantLevelScal[1][nLevel-1];
					break;
				case PE_FIRE:
				case PE_ELEC:
				case PE_WATER:
				case PE_WIND:
				case PE_EARTH:
				case PE_GEN:
					fScalLevel = prj.m_fEnchantLevelScal[0][nLevel-1];
					break;
			}
		}

		if( nLevel2 <=0 )
			nLevel2 = 1;

		dwSfx += (nLevel2-1);

//sun: ���� ����Ʈ ��� ����
//sun: 10, __CSC_ENCHANT_EFFECT_2
		switch(nType)
		{
			case PE_FIRE:
			case PE_FIRE_AL:
				dwSfx_2 = XI_INDEX( 1070, XI_NAT_FIRE01 );
				dwSfx_2 += nEffLevel_2;
				break;
			case PE_WATER:
			case PE_WATER_AL:
				dwSfx_2 = XI_INDEX( 1076, XI_NAT_WATER01 );
				dwSfx_2 += nEffLevel_2;
				break;
			case PE_WIND:
			case PE_WIND_AL:
				dwSfx_2 = XI_INDEX( 1083, XI_NAT_WIND01 );
				dwSfx_2 += nEffLevel_2;
				break;
			case PE_EARTH:
			case PE_EARTH_AL:
				dwSfx_2 = XI_INDEX( 1101, XI_NAT_EARTH01 );
				dwSfx_2 += nEffLevel_2;
				break;
		}

//sun: 10, __CSC_ENCHANT_EFFECT_2
		if(nLevel > 0)
			pFire->Create( m_pd3dDevice, v3, dwSfx, vScale * fScalLevel );	// �ش� sfx�� ��ƼŬ ������Ŵ.

		if(nEffLevel_2 > 0 && (nType == PE_FIRE || nType == PE_WATER || nType == PE_WIND || nType == PE_EARTH
			|| nType == PE_FIRE_AL || nType == PE_WATER_AL || nType == PE_WIND_AL || nType == PE_EARTH_AL)
			&& (fTemp > 0.1f && fTemp < 0.93f))
			pFire->Create( m_pd3dDevice, v3, dwSfx_2 );	// �ش� sfx�� ��ƼŬ ������Ŵ.
	
//sun: 9�� ���ð��� (�߰� �˱� ���)
		if(IsSecondLine)
		{
//sun: 10, __CSC_ENCHANT_EFFECT_2
			if(nLevel > 0)
				pFire->Create( m_pd3dDevice, v2_3, dwSfx, vScale * fScalLevel );	// �ش� sfx�� ��ƼŬ ������Ŵ.
//sun: 10, __CSC_ENCHANT_EFFECT_2
		if(nEffLevel_2 > 0 && (nType == PE_FIRE || nType == PE_WATER || nType == PE_WIND || nType == PE_EARTH
			|| nType == PE_FIRE_AL || nType == PE_WATER_AL || nType == PE_WIND_AL || nType == PE_EARTH_AL)
			&& (fTemp > 0.1f && fTemp < 0.93f))
				pFire->Create( m_pd3dDevice, v2_3, dwSfx_2 );	// �ش� sfx�� ��ƼŬ ������Ŵ.
		}

	}

	pFire->Render( m_pd3dDevice, &m2 );
}
#else
void	CModelObject::CreateParticle( int /*nParts*/, const D3DXMATRIX * /*pmWorld*/, int /*nType*/, int /*nLevel*/, int /*nLevel2*/ )
{

}

#endif // client


//_SUN_ENCHANT_EFFECT17  *BEGIN=========================================================================================
#ifdef __CLIENT

int	CModelObject::GetParticleIndex( int nParts, int nAttr )
{
	if( PARTS_LWEAPON == nParts )
	{
		if( PE_GEN == nAttr || PE_GEN_AL == nAttr || PE_GEN_BR == nAttr )
			return PEF_NORMAL_L;
		else 
			return PEF_ATTR_L;
	}
	else if( PARTS_RWEAPON == nParts )
	{
		if( PE_GEN == nAttr || PE_GEN_AL == nAttr || PE_GEN_BR == nAttr )
			return PEF_NORMAL_R;
		else 
			return PEF_ATTR_R;
	}

	return -1;
}

//--------------------------------------------------------------------------------------------------
DWORD	CModelObject::GetSfxIDByAttr( int nAttr, int nLevel, DWORD ik3_kind, int nType )
{
	// define key���ڿ��� ���� value���� ã�Ƽ� ����
	int nSfx = 0;

	CString strTemp("XI_WEA_");
	switch( ik3_kind )
	{
	case IK3_HAND:				return 0;
	case IK3_SWD:				strTemp += "SWORD";			break;
	case IK3_AXE :				strTemp += "AXE";			break;
	case IK3_CHEERSTICK:		strTemp += "STICK";			break;
	case IK3_KNUCKLEHAMMER:		strTemp += "KNUCK";			break;
	case IK3_WAND:				strTemp += "WAND";			break;
	case IK3_STAFF:				strTemp += "STAFF";			break;
	case IK3_THSWD:				strTemp += "TWOSWORD";		break;
	case IK3_THAXE:				strTemp += "TWOAXE";		break;
	case IK3_VIRTUAL:			return 0;
	case IK3_YOYO:				strTemp += "YOYO";			break;
	case IK3_BOW:				strTemp += "BOW";			break;
	case IK3_YOBO:				return 0;
	case IK3_CROSSBOW:			strTemp += "CROSSBOW";		break;

	default: 
		return 0;
	}

	switch( nAttr )
	{
	case PE_GEN:
	case PE_GEN_AL:				
	case PE_GEN_BR:									break;

	case PE_FIRE:
	case PE_FIRE_AL:			strTemp += "FIRE";	break;

	case PE_ELEC:
	case PE_ELEC_AL:			strTemp += "ELECT";	break;

	case PE_WATER:
	case PE_WATER_AL:			strTemp += "WATER"; break;

	case PE_WIND:
	case PE_WIND_AL:			strTemp += "WIND"; break;

	case PE_EARTH:
	case PE_EARTH_AL:			strTemp += "EARTH"; break;

	default: 
		return 0;
	}

	//��չ��� �ռ��� ��� "_TH"
	if( nType & IET_COMPOSE_TWOHAND )
		strTemp += "_TH";

	int nEffectLevel = GetEffectLevelByOrgLevel( nAttr, nLevel );

	//0�� ����Ʈ ���� 
	if( nEffectLevel <= 0 )
		return 0;

	CString strLevel;
	strLevel.Format( "%02d", nEffectLevel );
	strTemp += strLevel;

	BOOL bFind = CScript::LookupDefine( strTemp.GetString(), nSfx );
	
	//gmpbigsun( 20120227 ) : log���ŵ�.
	//��ã�� ���! write a log
	//if( FALSE == bFind )
	//	FLERROR_LOG( PROGRAM_NAME, _T( "nAttr%d, nLevel%d, ik3_kind%d" ), nAttr, nLevel, ik3_kind ) ;

	return nSfx;
}

//--------------------------------------------------------------------------------------------------
int	CModelObject::GetEffectLevelByOrgLevel( int nAttr, int nLevel )
{
	//�Ӽ����� �������� ���� ��������� ����Ʈ ������ ����

	int nEffectLevel = 0;
	if( nAttr == PE_GEN || nAttr == PE_GEN_AL )
	{
		//�Ϲ� ������ ���  max = 10 ( 3, 5, 7, 9, 10 )
		if( nLevel < 3 )
			nEffectLevel = 0;
		else if( nLevel < 5 )
			nEffectLevel = 1;
		else if( nLevel < 7 )
			nEffectLevel = 2;
		else if( nLevel < 9 )
			nEffectLevel = 3;
		else if( nLevel < 10 )
			nEffectLevel = 4;
		else if( nLevel >= 10 )
			nEffectLevel = 5;
		else 
		{
			FLASSERT( 0 );
			FLTRACE_LOG( PROGRAM_NAME, _T( "nAttr %d, nLevel %d"), nAttr, nLevel );
		}
		
	}
	else 
	{
		//�Ӽ� �� �ٷ糪 �Ϲ� ������ ��� max = 20 ( 3, 7, 11, 16, 20 )
		if( nLevel < 3 )
			nEffectLevel = 0;
		else if( nLevel < 7 )
			nEffectLevel = 1;
		else if( nLevel < 11 )
			nEffectLevel = 2;
		else if( nLevel < 16 )
			nEffectLevel = 3;
		else if( nLevel < 20 )
			nEffectLevel = 4;
		else if( nLevel == 20 )
			nEffectLevel = 5;
		else 
		{
			FLASSERT( 0 );
			FLTRACE_LOG( PROGRAM_NAME, _T( "nAttr %d, nLevel %d"), nAttr, nLevel );
		}	
		
	}

	return nEffectLevel;
}

//--------------------------------------------------------------------------------------------------
void CModelObject::CreateParticleAndRender( int nParts, const D3DXMATRIX *pmWorld, int nAttr, int nLevel, int nLevel2, DWORD ik3_kind, int nType )
{
	//���Լ��� ���Լ� �и�
	//���Լ��� Ư¡�� nLevel, nLevel2�� �ش��ϴ� ����Ʈ�� ���ÿ� ��� �����ϹǷ�, ���Լ� ���� �����ϰ� �۵��ϰ� �Ѵ�.
	switch( _GetContentState( CT_NEW_ENCHANTEFFECT ) )
	{
	case CS_VER1 : 
		CreateParticle( nParts, pmWorld, nAttr, nLevel, nLevel2 );
		break;

	case CS_VER2:

		//�ݷ��͵� ������ �Ǹ� ����Ʈ�� �Ѿ�����, �㳪 �ֽŹ����� �ݷ��� ����Ʈ ����.
		if( IK3_COLLECTER == ik3_kind )
			break;

		//1st
		if( nLevel > 0 )
		{
			if( nType & IET_BARUNA )
				CreateParticle_New( nParts, pmWorld, PE_GEN_BR, nLevel, ik3_kind );
			else 
				CreateParticle_New( nParts, pmWorld, PE_GEN, nLevel, ik3_kind );
		}

		//2nd
		//�Ӽ����ð���, nType�� ���� ��չ����ռ����θ� �����ϹǷ� �Ӽ����� sfx�������� ���� �߰���.
		if( nLevel2 > 0 )
			CreateParticle_New( nParts, pmWorld, nAttr, nLevel2, ik3_kind, nType );

		break;

	default:
		FLASSERT( 0 );
		break;

	}
	
}

//--------------------------------------------------------------------------------------------------
void CModelObject::CreateParticle_New( int nParts, const D3DXMATRIX *pmWorld, int nAttr, int nLevel, DWORD ik3_kind, int nType )
{
	// nParts : PARTS_�� ���Ǵ� ����
	// nAttr : �Ϲ�, �Ӽ��� Ÿ��
	// nLevel : �Ϲ������� ����
	// nLevel2 : �Ӽ������� ����
	// ik3_kind : IK3Ÿ��

	CPartsWeaponEffect* pEffect = NULL;

#if 0
		pEffect = CreateParticle_Internal( nParts, nAttr, nLevel, ik3_kind, nType, "fly_wfx_elect_bow_level5" );		//test
#else
		pEffect = CreateParticle_Internal( nParts, nAttr, nLevel, ik3_kind, nType );	
#endif 
		if( pEffect )
			RenderParticle_New( pEffect, nParts, ik3_kind, pmWorld );
}

//--------------------------------------------------------------------------------------------------
CPartsWeaponEffect*	CModelObject::CreateParticle_Internal( int nParts, int nAttr, int nLevel, DWORD ik3_kind, int nType, TCHAR* filenameSFX )
{
	int nIndex = GetParticleIndex( nParts, nAttr );
	if( -1 == nIndex || nIndex > PEF_MAX )
	{
		FLASSERT( 0 );
		return NULL;
	}

	BOOL bOK = TRUE;
	if( m_pPartsEffectNew[ nIndex ] )
	{
		//�����༮�� �н�
		CPartsWeaponEffect* pEffect = m_pPartsEffectNew[ nIndex ];
		if( pEffect->m_nLevel == nLevel && pEffect->m_nType == nAttr && pEffect->m_dwIK3 == ik3_kind && pEffect->m_nTypeEX == nType )	
			bOK = FALSE;
	}

	if( bOK )
	{
		SAFE_DELETE( m_pPartsEffectNew[ nIndex ] );
	
		BOOL bRst = FALSE;
		if( filenameSFX != NULL )
		{
			//�����̸����� ���� ���� ( test )
			m_pPartsEffectNew[ nIndex ] = new CPartsWeaponEffect;
			bRst = m_pPartsEffectNew[ nIndex ]->Create( D3DDEVICE, D3DXVECTOR3( .0f, .0f, .0f ), filenameSFX, D3DXVECTOR3( 1.0f, 1.0f, 1.0f ) );
		}
		else 
		{
			//�Ӽ�, ����, ������ ������ ���� sfx id�� �� ����
			DWORD dwSfx = GetSfxIDByAttr( nAttr, nLevel, ik3_kind, nType );
			if( dwSfx > 0 )
			{
				m_pPartsEffectNew[ nIndex ] = new CPartsWeaponEffect;
				bRst = m_pPartsEffectNew[ nIndex ]->Create( D3DDEVICE, D3DXVECTOR3( .0f, .0f, .0f ), dwSfx, D3DXVECTOR3( 1.0f, 1.0f, 1.0f ) );
			}
		}
		
		if( TRUE == bRst )
		{
			m_pPartsEffectNew[ nIndex ]->m_nLevel = nLevel;
			m_pPartsEffectNew[ nIndex ]->m_nType = nAttr;
			m_pPartsEffectNew[ nIndex ]->m_dwIK3 = ik3_kind;
			m_pPartsEffectNew[ nIndex ]->m_nTypeEX = nType;
		}
		else
		{
			SAFE_DELETE( m_pPartsEffectNew[ nIndex ] );
		}
	}

	return m_pPartsEffectNew[ nIndex ];
}

//--------------------------------------------------------------------------------------------------
void CModelObject::RenderParticle_New( CPartsWeaponEffect* pEffect, int nParts, DWORD ik3_kind, const D3DXMATRIX* pmWorld )
{
	if( !pEffect )
		return;

	O3D_ELEMENT* pElem = GetParts( nParts );
	if( !pElem )
		return;

	//�� ������ ���������� �����Ǿ� ������ ���� ���� �޼տ� �����Ѱ�쿡 ������ �ֵ�. 
	//���� ������ LHAND�ӿ��� RWEAPON���� �س��� ���������� ���ʼտ� ���̵��� �ϵ��ڵ� �Ǿ��ֵ�. -_-fuck
	if( ik3_kind == IK3_BOW )
		nParts = PARTS_LWEAPON;

	//local matrix
	D3DXMATRIX* pmLocal = NULL;
	if( PARTS_LWEAPON == nParts )
		pmLocal = &pElem->m_mLocalLH;
	else if( PARTS_RWEAPON == nParts )
		pmLocal = &pElem->m_mLocalRH;

	//multiply local matrix and parent matrix
	D3DXMATRIX m1;
	D3DXMatrixMultiply( &m1, pmLocal, &m_mUpdateBone[ pElem->m_nParentIdx ] );

	//���� matrix
	D3DXMATRIX mRst;
	D3DXMatrixMultiply( &mRst, &m1, pmWorld );

	//������� 0,0,0 �� ������ǥ
	D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f );				//��� ������ ����( 0, 0, 0 )�� ���̱�� ��
	D3DXVec3TransformCoord( &vPos, &vPos, &mRst );

	pEffect->Render( m_pd3dDevice, vPos, &mRst );
}

#ifdef _DEBUG
void CModelObject::RenderItemEffect_TEST( const D3DXMATRIX* pmWorld )
{
	if( m_pPartsEffectNew_Debug[0] == NULL && m_pPartsEffectNew_Debug[1] == NULL )
		return;

	O3D_ELEMENT* pElem = GetParts( PARTS_RWEAPON );
	if( !pElem )
		return;

	//������ ���� matrix
	D3DXMATRIX* pmLocal = &pElem->m_mLocalRH;

	//������ �θ��� matrix�� multiply
	D3DXMATRIX m1;
	D3DXMatrixMultiply( &m1, pmLocal, &m_mUpdateBone[ pElem->m_nParentIdx ] );

//	D3DXMATRIX matRot;
//	D3DXMatrixRotationY( &matRot, D3DXToRadian( 90 ) );
//	m1 = m1 * matRot;

	//���� matrix
	D3DXMATRIX mRst;
	D3DXMatrixMultiply( &mRst, &m1, pmWorld );

	//������� 0,0,0 �� ������ǥ
	D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f );				//��� ������ ����( 0, 0, 0 )�� ���̱�� ��
	D3DXVec3TransformCoord( &vPos, &vPos, &mRst );

	if( m_pPartsEffectNew_Debug[0] )
		m_pPartsEffectNew_Debug[0]->Render( m_pd3dDevice, vPos, &mRst );
	if( m_pPartsEffectNew_Debug[1] )
		m_pPartsEffectNew_Debug[1]->Render( m_pd3dDevice, vPos, &mRst );
}
#endif //_DEBUG

#endif	//__CLIENT
//_SUN_ENCHANT_EFFECT17  *END=========================================================================================


#ifdef __CLIENT
void	CModelObject::RenderItemElec_Adv( int nParts, const D3DXMATRIX *pmWorld, int nLevel )
{
	if( _GetContentState( CT_NEW_ENCHANTEFFECT ) == CS_VER2 )
		return;

	if( nLevel >= 10 )		nLevel = 5;		//sun:13, ���� Ȯ��(�Ӽ�, �Ϲ�)
	else if( nLevel == 9 )	nLevel = 4;
	else if( nLevel == 8 )	nLevel = 3;
	else if( nLevel == 7 )	nLevel = 2;
	else if( nLevel >= 4 )	nLevel = 1;	// 4 ~ 6
	else 
		nLevel = 0;

	if(nLevel <= 0 || nLevel > 5)
		return;

	O3D_ELEMENT		*pElem = GetParts( nParts );		// ������ ������ ������

	if( !pElem )
		return;
	
	if( pElem->m_pObject3D == NULL )		return;
	
	if( pElem->m_pObject3D->m_vForce1.x == 0 )	return;
	if( pElem->m_pObject3D->m_vForce2.x == 0 )	return;

//sun: 9�� ���ð��� (�߰� �˱� ���)
	BOOL IsSecondLine = FALSE;
	D3DXVECTOR3 v2_1, v2_2;
	if( pElem->m_pObject3D->m_vForce3.x != 0 && pElem->m_pObject3D->m_vForce4.x != 0 )
		IsSecondLine = TRUE;

	if(IsSecondLine)
	{
		v2_1 = pElem->m_pObject3D->m_vForce3;
		v2_2 = pElem->m_pObject3D->m_vForce4;
	}
	
	D3DXMATRIX	m1, m2;
	D3DXVECTOR3	v1, v2, v3;
	
	D3DXMATRIX *pmLocal;
	if( pElem->m_nParentIdx == GetRHandIdx() )
		pmLocal = &pElem->m_mLocalRH;
	else if( pElem->m_nParentIdx == GetLHandIdx() )
		pmLocal = &pElem->m_mLocalLH;
	else if( pElem->m_nParentIdx == GetLArmIdx() )
		pmLocal = &pElem->m_mLocalShield;
	else if( pElem->m_nParentIdx == GetRArmIdx() )
		pmLocal = &pElem->m_mLocalKnuckle;
	
	D3DXMatrixMultiply( &m1, pmLocal, &m_mUpdateBone[ pElem->m_nParentIdx ] );
	m2 = m1 * *pmWorld;

	v1 = pElem->m_pObject3D->m_vForce1;		// ���÷� �ǳ��ְ�
	v2 = pElem->m_pObject3D->m_vForce2;

	CPartsBeam *pBeam;
	if( nParts == PARTS_LWEAPON )
	{
		if( m_pPartsEffect2_Detail && m_pPartsEffect2_Detail->m_nType != PE_ELEC )
		{
			SAFE_DELETE( m_pPartsEffect2_Detail );
		}

		if( m_pPartsEffect2_Detail == NULL )
			m_pPartsEffect2_Detail = new CPartsBeam;

		pBeam = (CPartsBeam *)m_pPartsEffect2_Detail;
	} else
	{
		if( m_pPartsEffect1_Detail && m_pPartsEffect1_Detail->m_nType != PE_ELEC )
		{
			SAFE_DELETE( m_pPartsEffect1_Detail );
		}
		
		if( m_pPartsEffect1_Detail == NULL )
			m_pPartsEffect1_Detail = new CPartsBeam;
		
		pBeam = (CPartsBeam *)m_pPartsEffect1_Detail;
	}
	
	pBeam->Render( m_pd3dDevice, &m2, g_ModelGlobal.m_vCameraPos, g_ModelGlobal.m_vCameraForward, v1, v2, nLevel );
//sun: 9�� ���ð��� (�߰� �˱� ���)
	if(IsSecondLine)
		pBeam->Render( m_pd3dDevice, &m2, g_ModelGlobal.m_vCameraPos, g_ModelGlobal.m_vCameraForward, v2_1, v2_2, nLevel );
}
#else
void	CModelObject::RenderItemElec_Adv( int /*nParts*/, const D3DXMATRIX * /*pmWorld*/, int /*nLevel*/ )
{

}
#endif // CLIENT

void	CModelObject::SetMotionBlending( BOOL bFlag )
{ 
	if( bFlag == m_bMotionBlending )	return;
	m_bMotionBlending = bFlag;
	m_fBlendWeight = 0.0f;
}


void	CModelObject::GetForcePos( D3DXVECTOR3 *vOut, int nIdx, int nParts, const D3DXMATRIX &mWorld )
{
	D3DXMATRIX *pmLocal;
	O3D_ELEMENT		*pElem = GetParts( nParts );		// ������ ������ ������
	D3DXMATRIX	m1;
	D3DXVECTOR3		v1;

	if( !pElem )
		return;
	
	if( pElem->m_pObject3D == NULL )		return;
	
	if( pElem->m_pObject3D->m_vForce1.x == 0 )	return;
	if( pElem->m_pObject3D->m_vForce2.x == 0 )	return;
	
	if( pElem->m_nParentIdx == GetRHandIdx() )
		pmLocal = &pElem->m_mLocalRH;
	else if( pElem->m_nParentIdx == GetLHandIdx() )
		pmLocal = &pElem->m_mLocalLH;
	else
	{
		D3DXMatrixIdentity( &m1 );
		pmLocal = &m1;						// �ϴ� ������ �ȳ��� �̷��� �Ѵ�.
	}
		
	
//	if( m_pMotion )
//		m_pMotion->AnimateBone( m_mUpdateBone, m_pMotionOld, m_fFrameCurrent, GetNextFrame(), m_nFrameOld, m_bMotionBlending, m_fBlendWeight );		// �ϴ� ���밡 �ִٸ� ���� �ִϸ��̼� ��Ŵ
	
	D3DXMatrixMultiply( &m1, pmLocal, &m_mUpdateBone[ pElem->m_nParentIdx ] );
	if( nIdx == 0 )
		D3DXVec3TransformCoord( &v1, &(pElem->m_pObject3D->m_vForce1), &m1 );
	else
		D3DXVec3TransformCoord( &v1, &(pElem->m_pObject3D->m_vForce2), &m1 );
	
	D3DXVec3TransformCoord( &v1, &v1, &mWorld );
	*vOut = v1;
	
}


void	CModelObject::GetForcePos( D3DXVECTOR3 *vOut, int nIdx, int nParts, const D3DXMATRIX &mWorld, float fScale)
{
	D3DXMATRIX mLocal;
	O3D_ELEMENT		*pElem = GetParts( nParts );		// ������ ������ ������
	D3DXMATRIX	m1;
	D3DXVECTOR3		v1;

	if( !pElem )
		return;
	
	if( pElem->m_pObject3D == NULL )		return;
	
	if( pElem->m_pObject3D->m_vForce1.x == 0 )	return;
	if( pElem->m_pObject3D->m_vForce2.x == 0 )	return;
	
	if( pElem->m_nParentIdx == GetRHandIdx() )
	{
		mLocal = pElem->m_mLocalRH;
		D3DXMatrixScaling(&mLocal, fScale, fScale, fScale);
	}
	else if( pElem->m_nParentIdx == GetLHandIdx() )
	{
		mLocal = pElem->m_mLocalLH;
		D3DXMatrixScaling(&mLocal, fScale, fScale, fScale);
	}
	else
	{
		D3DXMatrixIdentity( &m1 );
		mLocal = m1;						// �ϴ� ������ �ȳ��� �̷��� �Ѵ�.
	}
		
	
//	if( m_pMotion )
//		m_pMotion->AnimateBone( m_mUpdateBone, m_pMotionOld, m_fFrameCurrent, GetNextFrame(), m_nFrameOld, m_bMotionBlending, m_fBlendWeight );		// �ϴ� ���밡 �ִٸ� ���� �ִϸ��̼� ��Ŵ
	
	D3DXMatrixMultiply( &m1, &mLocal, &m_mUpdateBone[ pElem->m_nParentIdx ] );
	if( nIdx == 0 )
		D3DXVec3TransformCoord( &v1, &(pElem->m_pObject3D->m_vForce1), &m1 );
	else
		D3DXVec3TransformCoord( &v1, &(pElem->m_pObject3D->m_vForce2), &m1 );
	
	D3DXVec3TransformCoord( &v1, &v1, &mWorld );
	*vOut = v1;
	
}


// �ָ� �߾��� ��ġ�� ����Ҷ�.
void	CModelObject::GetHandPos( D3DXVECTOR3 *vOut, int nParts, const D3DXMATRIX &mWorld )
{
	D3DXMATRIX *pmLocal;
	D3DXMATRIX	m1;
	D3DXVECTOR3		v1;

	int nParentIdx = 0;
	if( m_pBone == NULL )	return;
	if( nParts == PARTS_RWEAPON )
	{
		nParentIdx = GetRHandIdx();
		pmLocal = &m_pBone->m_mLocalRH;
	}
	else if( nParts == PARTS_LWEAPON  )
	{
		nParentIdx = GetLHandIdx();
		pmLocal = &m_pBone->m_mLocalLH;
	}
	else
	{
		D3DXMatrixIdentity( &m1 );
		pmLocal = &m1;						// �ϴ� ������ �ȳ��� �̷��� �Ѵ�.
	}
	
	
//	if( m_pMotion )
//		m_pMotion->AnimateBone( m_mUpdateBone, m_pMotionOld, m_fFrameCurrent, GetNextFrame(), m_nFrameOld, m_bMotionBlending, m_fBlendWeight );		// �ϴ� ���밡 �ִٸ� ���� �ִϸ��̼� ��Ŵ
	
	// �ִϸ��̼ǳ��� ���� ��Ʈ������ LocalR/LH�� ���ؼ� ���� ��Ʈ���� ����.
	D3DXMatrixMultiply( &m1, pmLocal, &m_mUpdateBone[ nParentIdx ] );
	v1.x = m1._41;		v1.y = m1._42;		v1.z = m1._43;		// ��ǥ�� �ʿ���.
	
	D3DXVec3TransformCoord( &v1, &v1, &mWorld );
	*vOut = v1;
	
}

#ifdef __BS_EFFECT_LUA
BOOL CModelObject::GetPosBone( D3DXVECTOR3* pOut, const char* bonename )
{
	//gmpbigsun : ���̸����� ����ǥ ���� 
	for( int i =0; i < m_pBone->m_nMaxBone; ++i )
	{
		BONE* pUnitBone = m_pBone->GetBone( i );
		if( !pUnitBone )
		{
			FLASSERT( 0 );
			continue;
		}

		if( strcmp( bonename, pUnitBone->m_szName ) == 0 )
		{
			D3DXMATRIX matTemp;
			if( pUnitBone->m_pParent )
				D3DXMatrixMultiply( &matTemp, &pUnitBone->m_mLocalTM, &m_mUpdateBone[ pUnitBone->m_nParentIdx ] );
			else 
				D3DXMatrixMultiply( &matTemp, &pUnitBone->m_mLocalTM, &m_mUpdateBone[ i ] );

			pOut->x = matTemp._41;
			pOut->y = matTemp._42;
			pOut->z = matTemp._43;

			return TRUE;
		}
	}

	return FALSE;
	
}

#endif //__BS_EFFECT_LUA



///////////////////////////////////////////////////////////////////////////////////
//
//
//
D3DXVECTOR3		SplineSlerp( D3DXVECTOR3 *v1, D3DXVECTOR3 *v2, D3DXVECTOR3 *v3, D3DXVECTOR3 *v4, float fSlerp )
{
	D3DXVECTOR3		vResult;

	float	t = fSlerp;
	float	t2 = t * t;
	float	t3 = t2 * t;

	D3DXVECTOR3		m0, m1;
	const float alpha = 0.0f;

	m0 = ((1 - alpha) / 2.0f) * 
		 ((*v2 - *v1) + *v3 - *v2);
	m1 = ((1 - alpha) / 2.0f) *
		 ((*v3 - *v2) + *v4 - *v3);

	vResult = (((2 * t3) - (3 * t2) + 1) * *v2) +
			   ((t3 - (2 * t2) + t) * m0) + 
			   ((t3 - t2) * m1) +
			   (((-2 * t3) + (3 * t2)) *
			   *v3 );

	return vResult;
}

void	CSwordForce::Add( D3DXVECTOR3 v1, D3DXVECTOR3 v2 )
{
	if( m_nMaxVertex >= MAX_SF_SWDFORCE )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "SWDForceAdd : ���� �ʰ� %d" ), m_nMaxVertex );
		return;
	}
	FVF_SWDFORCE*	pList = &m_aList[ m_nMaxVertex ];

	pList->position = v1;
	pList->color = 0xffffffff;

	pList = &m_aList2[ m_nMaxVertex ];
	pList->position = v2;
	pList->color = 0xffffffff;

	m_nMaxVertex ++;
}

// ��ϵ� Ű����Ʈ�� ���ö��� ������ ����Ʈ�� �ٲ۴�.
// �̷��� ������� ����Ʈ�� ���� �׸���.
void	CSwordForce::MakeSpline( void )
{
	FVF_SWDFORCE *pKey;
	int		i, j;
	D3DXVECTOR3 vSlp;
	int		a, b, c, d;
	int		nMaxVertex = m_nMaxVertex;

	m_nMaxSpline = 0;
	for( i = 0; i < nMaxVertex-1; i ++ )
	{
		// i ~ i+1���̸� �����Ѵ�.  
		a = i - 1;		if( a < 0 )	a = 0;
		b = i;
		c = i+1;
		d = i+2;		if( d >= nMaxVertex )	d = nMaxVertex - 1;
		for( j = 0; j < MAX_SF_SLERP+1; j ++ )
		{
			if( m_nMaxSpline >= MAX_SF_SPLINE )
			{
				FLERROR_LOG( PROGRAM_NAME, _T( "SWDForceAdd Spline : ���� �ʰ� %d" ), m_nMaxSpline );
				break;
			}
			pKey = m_aList;
			vSlp = SplineSlerp( &pKey[a].position, &pKey[b].position, &pKey[c].position, &pKey[d].position, (float)j / MAX_SF_SLERP );
			m_aSpline[ m_nMaxSpline ].position = vSlp;
			m_aSpline[ m_nMaxSpline ].color = 0xffffffff;
			m_nMaxSpline ++;
			pKey = m_aList2;
			vSlp = SplineSlerp( &pKey[a].position, &pKey[b].position, &pKey[c].position, &pKey[d].position, (float)j / MAX_SF_SLERP );
			m_aSpline[ m_nMaxSpline ].position = vSlp;
			m_aSpline[ m_nMaxSpline ].color = 0xffffffff;
			m_nMaxSpline ++;
		}
	}
}

void	CSwordForce::Process( void )
{
	FVF_SWDFORCE *pList = m_aSpline;
	int		i;
	int		nRed1 =  0;
	int		nGreen1 =  0;
	int		nBlue1 =   0;
	int		nRed2 = (m_dwColor >> 16) & 0xff;
	int		nGreen2 = (m_dwColor >> 8) & 0xff;
	int		nBlue2 = m_dwColor & 0xff;

	if( m_nMaxSpline == 0 )		return;

	for( i = m_nMaxDraw - 1; i >= 0; i -- )
	{
		if( i % 2 )
			pList[i].color = D3DCOLOR_ARGB( 255, nRed1, nGreen1, nBlue1 ); 
		else
			pList[i].color = D3DCOLOR_ARGB( 255, nRed2, nGreen2, nBlue2 ); 
		
		nRed1   -= 3; if( nRed1   < 0 )	nRed1   = 0;
		nGreen1 -= 3; if( nGreen1 < 0 ) nGreen1 = 0;
		nBlue1  -= 3; if( nBlue1  < 0 )	nBlue1  = 0;

		nRed2   -= 3; if( nRed2   < 0 )	nRed2   = 0;
		nGreen2 -= 3; if( nGreen2 < 0 ) nGreen2 = 0;
		nBlue2  -= 3; if( nBlue2  < 0 )	nBlue2  = 0;
	}

}

//
//
//
void	CSwordForce::Draw( LPDIRECT3DDEVICE9 pd3dDevice, const D3DXMATRIX *mWorld )
{
	pd3dDevice->SetTransform( D3DTS_WORLD, mWorld );
	pd3dDevice->SetVertexShader( NULL );
	pd3dDevice->SetFVF( D3DFVF_SWDFORCE );

    pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );

	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
	pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE   );
	pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE   );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );


	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

	{
		int		nShare, nRest;
		int		i;
		FVF_SWDFORCE *p = m_aSpline;
		int		nMaxDraw = m_nMaxDraw;

		nMaxDraw -= 2;
		nShare = nMaxDraw / 64;
		nRest  = nMaxDraw % 64;
		for( i = 0; i < nShare; i ++ )			// ������Ƽ�� ������ ������ �־ �̷��� �ߴ�.
		{
			pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 64, p, sizeof(FVF_SWDFORCE) );
			p += 64;
		}

		if( nRest > 0 )
			pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, nRest, p, sizeof(FVF_SWDFORCE) );

	}

	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE );
    pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
    pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

}


