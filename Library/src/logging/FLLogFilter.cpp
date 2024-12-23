
#include "../../include/logging/FLLogFilter.h"



FLLogFilter::~FLLogFilter()
{
}

bool	FLLogFilter::IsPublish( const FLLogEvent* pEvent )
{
	return DoIsPublish( pEvent );
}
