#pragma once

#ifdef WXL_CLIENT_SPECIFIED
#include	"CFCommDef.h"
#endif
#include	"CFBaseObj.hpp"
#ifdef WXL_CLIENT_SPECIFIED
#include	"CFCrc.h"
#include	"CFStringConversion.h"
#include	"CFThread.h"
#elif defined(WXL_SERVER_SPECIFIED)
#include	"CFDummyFunc.hpp"
#endif
#include	"CFRefCntBase.hpp"
#ifdef WXL_CLIENT_SPECIFIED
#include	"CFFactory.h"
#include	"CFSingleton.h"
#endif
#include	"CFSimpleCache.hpp"
#ifdef WXL_CLIENT_SPECIFIED
#include	"CFUtil.h"
#include	"CFNotify.h"
#include	"CFLocalBuffer.h"
#endif
