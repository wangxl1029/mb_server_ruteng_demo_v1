#include	"InnerCommon.hpp"
#include	"CFBaseObj.hpp"
#ifdef WXL_CLIENT_SPECIFIED
#include	"CFThread.h"
#elif defined(WXL_SERVER_SPECIFIED)
#include	"CFDummyFunc.hpp"
#endif
#include	"CFRefCntBase.hpp"

//	CRefCntBase
REF_CNT_IMPLEMENTTATION_CPP( CRefCntBase )


// CCFCriticalSection
CCFCriticalSection::CCFCriticalSection()
{}

CCFCriticalSection::~CCFCriticalSection()
{}

bool CCFCriticalSection::Lock(uint)
{
	return true;
}

void CCFCriticalSection::Unlock()
{}

