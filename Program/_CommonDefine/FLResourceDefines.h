
#ifndef	__FLRESOURCEDEFINES_H__
#define	__FLRESOURCEDEFINES_H__


//#define	INCLUDE_RESOURCE_DEFINES
#ifdef	INCLUDE_RESOURCE_DEFINES

#include <defineObj.h>
#include <defineItem.h>
#include <defineSkill.h>

#define	ITEM_INDEX_VERIFY
#define	SKILL_INDEX_VERIFY
#define	QUEST_INDEX_VERIFY
#define	XI_INDEX_VERIFY
#define	OBJECT_INDEX_VERIFY
#define	CTRL_INDEX_VERIFY
#define	REGION_INDEX_VERIFY

#endif

// ����Ʈ�� define�� ���� ������ �ʾҴ�.
//#include <definequest.h>
#include "../../resource/definequest.h"


//////////////////////////////////////////////////////////////////////////
template< int index, bool equal >
struct	FLPropIndexVerify;

template< int index >
struct	FLPropIndexVerify< index, true >
{
	enum
	{
		value = index,
	};
};

template< int index >
struct	FLPropIndexVerify< index, false >
{
};
//////////////////////////////////////////////////////////////////////////


#define	PROP_INDEX( index, define )		FLPropIndexVerify< index, index == define >::value


#ifdef	SKILL_INDEX_VERIFY
#define	SKILL_INDEX( index, define )	PROP_INDEX( index, define )
#else
#define	SKILL_INDEX( index, define )	(index)
#endif


#ifdef	ITEM_INDEX_VERIFY
#define	ITEM_INDEX( index, define )	PROP_INDEX( index, define )
#else
#define	ITEM_INDEX( index, define )	(index)
#endif


#ifdef	QUEST_INDEX_VERIFY
#define	QUEST_INDEX( index, define )	PROP_INDEX( index, define )
#else
#define	QUEST_INDEX( index, define )	(index)
#endif


#ifdef	OBJECT_INDEX_VERIFY
#define	OBJECT_INDEX( index, define )	PROP_INDEX( index, define )
#else
#define	OBJECT_INDEX( index, define )	(index)
#endif


#ifdef	XI_INDEX_VERIFY
#define	XI_INDEX( index, define )	PROP_INDEX( index, define )
#else
#define	XI_INDEX( index, define )	(index)
#endif


#ifdef	CTRL_INDEX_VERIFY
#define	CTRL_INDEX( index, define )	PROP_INDEX( index, define )
#else
#define	CTRL_INDEX( index, define )	(index)
#endif


#ifdef	REGION_INDEX_VERIFY
#define	REGION_INDEX( index, define )	PROP_INDEX( index, define )
#else
#define	REGION_INDEX( index, define )	(index)
#endif







#endif