#ifndef	INNERCOMMON_H
#define INNERCOMMON_H

#if defined( linux )
#else
#pragma warning( disable : 4786 )
#endif

#include	"OSWrapper.h"
#include	"ELErrLog.h"

#include	<vector>
#include	<list>
#include	<map>
#include	<set>
#include	<stack>
#include	<deque>
#include	<queue>
#include	<string>
#include	<algorithm>
using namespace std;

#include	"IOLibCommon_HeaderAll.h"

#endif	//	INNERCOMMON_H
