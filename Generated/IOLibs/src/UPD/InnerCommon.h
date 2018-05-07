#ifndef	__COMMON_H_
#define	__COMMON_H_

#if defined( linux )
#else
#pragma warning( disable : 4786 )
#endif

#if defined( linux )
#else
#include	<windows.h>
#include	<tchar.h>
#endif

#include	<stdio.h>
#include	<stdlib.h>
#include	<stdarg.h>
#include	<memory.h>
#include	<assert.h>

#include	<typeinfo>
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
#include	"IOLibUPD_HeaderAll.h"

#endif	//	__COMMON_H_
