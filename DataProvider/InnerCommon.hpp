#pragma once

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
#ifdef WXL_CLIENT_SPECIFIED
#include	"IOLibCommon_HeaderAll.h"
#include	"IOLibNDS_HeaderAll.h"
#endif

#include	"CFHeaderAll.hpp"
#ifdef WXL_CLIENT_SPECIFIED
#include	"SL_HeaderAll.h"
#endif

#define		NEU_DB

