#pragma once

#if defined( linux )
#else
#pragma warning( disable: 4819 )
#endif

#define		IN
#define		OUT

#include	"OSWrapper.h"
#include	"ELErrLog.h"

#include <math.h>

#include	<vector>
#include	<list>
#include	<map>
#include	<set>
#include	<stack>
#include	<deque>
#include	<queue>
#include	<string>
#include	<algorithm>
#ifdef WXL_SERVER_SPECIFIED
#include	<memory>
#endif
using namespace std;

#include	"nds_sqlite3.h"
#include	"IOLibCommon_HeaderAll.h"
#ifdef WXL_CLIENT_SPECIFIED
#include	"IOLibNDS_HeaderAll.h"
#endif

#include	"CFHeaderAll.hpp"

#include	"SL_HeaderAll.h"

#define		NEU_DB

