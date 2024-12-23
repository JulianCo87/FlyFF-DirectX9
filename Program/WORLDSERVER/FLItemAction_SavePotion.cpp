
#include "StdAfx.h"
#include "FLItemAction_SavePotion.h"
#include "FLItemUsing.h"


//-------------------------------------------------------------------------------------------------------------------//

FLItemAction_SavePotionStorage & FLItemAction_SavePotionStorage::GetInstance()
{
	static FLItemAction_SavePotionStorage inst;
	return inst;
}


bool FLItemAction_SavePotionStorage::Use( FLWSUser & kUser, FLItemElem & io_kUseItem, CAr & /*ar*/ )
{
	const PT_ITEM_SPEC pSpec	= io_kUseItem.GetProp();
	if( pSpec == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "[ UNKNOWN SPEC - SrcID(%u) ]" ), io_kUseItem.m_dwItemId );
		return false;
	}

	// ���� �Ǿ� �ִ��� ����.
	if( io_kUseItem.IsFlag( FLItemElem::is_using ) == FALSE || io_kUseItem.m_dwKeepTime == 0 )
	{
		kUser.AddDefinedText( TID_MMI_ELLDINPOTION_TEXT06 );		//���� �Ǿ� �ִ� ������ �Դϴ�.
		return false;
	}

	const int nRecoverPoint	= min( io_kUseItem.m_nHitPoint, kUser.GetMaxHitPoint() - kUser.GetHitPoint() );
	if( nRecoverPoint <= 0 )
	{
		if( io_kUseItem.m_nHitPoint == 0 ) 
			kUser.AddDefinedText( TID_MMI_ELLDINPOTION_TEXT01 );		// ����� ü���� ������ ġ���� �� �����ϴ�.
		else 
			kUser.AddDefinedText( TID_MMI_ELLDINPOTION_TEXT10 );		// �̹� ��� HP�� ä���� �ֽ��ϴ�.
		return false;
	}

	kUser.IncHitPoint( nRecoverPoint );
	kUser.UpdateItem( (BYTE)( io_kUseItem.m_dwObjId ), UI_HP, io_kUseItem.m_nHitPoint - nRecoverPoint );

	kUser.AddDefinedText( TID_MMI_ELLDINPOTION_TEXT05, "%d", io_kUseItem.m_nHitPoint );

	if( pSpec->bPermanence != TRUE )
	{
		const size_t uUseCount = 1;
		g_DPSrvr.PutItemLog( &kUser, "*", "remove charge potion", &io_kUseItem, uUseCount );
		kUser.RemoveItem( io_kUseItem.m_dwObjId, uUseCount );
	}

	return true;
}



//---------------------------------------------------------------------------------------------------------------------------//

FLItemAction_SavePotionCharge & FLItemAction_SavePotionCharge::GetInstance()
{
	static FLItemAction_SavePotionCharge inst;
	return inst;
}


bool FLItemAction_SavePotionCharge::Apply( FLWSUser & kUser, FLItemElem & io_kUseItem, FLItemElem & io_kDestItem, CAr & /*ar*/ )
{
	const PT_ITEM_SPEC pSrcSpec	= io_kUseItem.GetProp();
	const PT_ITEM_SPEC pDestSpec= io_kDestItem.GetProp();
	if( pSrcSpec == NULL || pDestSpec == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "[ UNKNOWN SPEC - SrcID(%u), DestID(%u) ]" ), io_kUseItem.m_dwItemId, io_kDestItem.m_dwItemId );
		return false;
	}

	if( pSrcSpec->bCanSavePotion == FALSE )
	{
		kUser.AddDefinedText( TID_MMI_ELLDINPOTION_TEXT08 );			//��ĥ�� ���� ���� �Դϴ�.
		return false;
	}

	//1. ���� ������ �������ΰ�?
	if( pDestSpec->dwItemKind1 != IK1_ACTIVE || pDestSpec->dwItemKind2 != IK2_ELLDINPOTION || pDestSpec->dwItemKind3 != IK3_SAVEPOTION || pDestSpec->dwDestParam[0] != pSrcSpec->dwDestParam[0] )
	{
		kUser.AddDefinedText( TID_MMI_ELLDINPOTION_TEXT08 );
		return false;
	}

	if( io_kDestItem.IsFlag( FLItemElem::is_using ) == FALSE || io_kDestItem.m_dwKeepTime == 0 )
	{
		kUser.AddDefinedText( TID_MMI_ELLDINPOTION_TEXT06 );		//���� �Ǿ��ִ� ������ �Դϴ�
		return false;
	}
	
	if( pDestSpec->nAdjParamVal[0] <= io_kDestItem.m_nHitPoint )
	{
		kUser.AddDefinedText( TID_MMI_ELLDINPOTION_TEXT04, "%d %d", io_kDestItem.m_nHitPoint, pDestSpec->nAdjParamVal[0] );       //�̹� �׾Ƹ��� �� ���ֽ��ϴ�.
		return false;
	}

	// 2.����
	int nOneFillPoint			= 0;
	if( pSrcSpec->dwItemKind1 == IK1_GENERAL && pSrcSpec->dwItemKind2 == IK2_FOOD )
	{
		const DWORD dwFillRateMin	= 30;
		const DWORD dwFillRateMax	= 100;
		const float fFillRate		= static_cast< float >( xRandom( dwFillRateMin, dwFillRateMax ) / 100.0f );
		
		nOneFillPoint			= static_cast< int >( pSrcSpec->nAdjParamVal[0] * fFillRate );	//dwDestParam1	nAdjParamVal1
	}
	else if( pSrcSpec->dwItemKind1 == IK1_PASSIVE && pSrcSpec->dwItemKind2 == IK2_ELLDINPOTION && pSrcSpec->dwItemKind3 == IK3_FOODELLDIN )
	{
		nOneFillPoint			= pSrcSpec->nAdjParamVal[0];
		if( nOneFillPoint <= 0 )
			FLERROR_LOG( PROGRAM_NAME, "[ INVALID nAdjParamVal1 VALUE : ItemID(%u) ]", pSrcSpec->dwID );
	}

	if( nOneFillPoint <= 0  )
		return false;

	const int nSpacePoint			= max( 0, pDestSpec->nAdjParamVal[0] - io_kDestItem.m_nHitPoint );
	const size_t uConsumeCountMax	= ( nSpacePoint / nOneFillPoint ) + ( ( nSpacePoint % nOneFillPoint ) ? 1 : 0 );

	// HP ����
	const int nConsumeCount		= min( static_cast< size_t >( io_kUseItem.m_nItemNum ), uConsumeCountMax );
	const int nChargeHP			= io_kDestItem.m_nHitPoint + ( nOneFillPoint * nConsumeCount );

	const int nFillHP			= min( nChargeHP, pDestSpec->nAdjParamVal[0] );
	kUser.UpdateItem( (BYTE)( io_kDestItem.m_dwObjId ), UI_HP, nFillHP );

	kUser.AddDefinedText( TID_MMI_ELLDINPOTION_TEXT03, "%d %d", nFillHP, pDestSpec->nAdjParamVal[0] );

	// ����/ �α�		//�Ϲ�ȭ ��Ű�� ����
//	if( pSrcSpec->bPermanence != TRUE )
	{
		g_DPSrvr.PutItemLog( &kUser, "*", "charge potion", &io_kUseItem, nConsumeCount );
		kUser.RemoveItem( io_kUseItem.m_dwObjId, nConsumeCount );
	}
	return true;
}


//-------------------------------------------------------------------------------------------------------------------//


FLItemAction_SavePotionKey & FLItemAction_SavePotionKey::GetInstance()
{
	static FLItemAction_SavePotionKey inst;
	return inst;
}

bool FLItemAction_SavePotionKey::Apply( FLWSUser & kUser, FLItemElem & io_kUseItem, FLItemElem & io_kDestItem, CAr & /*ar*/ )
{
	const PT_ITEM_SPEC pSrcSpec	= io_kUseItem.GetProp();
	const PT_ITEM_SPEC pDestSpec= io_kDestItem.GetProp();
	if( pSrcSpec == NULL || pDestSpec == NULL )
	{
		FLERROR_LOG( PROGRAM_NAME, _T( "[ UNKNOWN SPEC - SrcID(%u), DestID(%u) ]" ), io_kUseItem.m_dwItemId, io_kDestItem.m_dwItemId );
		return false;
	}

	//1. ���� ������ �������ΰ�?
	if( pDestSpec->dwItemKind1 != IK1_ACTIVE || pDestSpec->dwItemKind2 != IK2_ELLDINPOTION || pDestSpec->dwItemKind3 != IK3_SAVEPOTION || pDestSpec->dwDestParam[0] != pSrcSpec->dwDestParam[0] )
		return false;

	// �ش� �������� ���� �����Ѵ�.
	if( FLItemUsing::UnbindPeriodLock( kUser, io_kDestItem ) == false )
	{
		//kUser.AddDefinedText( TID_MMI_ELLDINPOTION_TEXT07 );	//�̹� ���� �Ǿ� �ְų� ���� �Ҽ� ���� ������ �Դϴ�.
		return false;
	}

	kUser.UpdateItem( (BYTE)( io_kDestItem.m_dwObjId ), UI_HP, 0 );

	// ����/ �α�		//�Ϲ�ȭ ��Ű�� ����
	if( pSrcSpec->bPermanence != TRUE )
	{
		const size_t uUseCount = 1;
		g_DPSrvr.PutItemLog( &kUser, "*", "resetBind SavePotion", &io_kUseItem, uUseCount );
		kUser.RemoveItem( io_kUseItem.m_dwObjId, uUseCount );
	}
	return true;
}


