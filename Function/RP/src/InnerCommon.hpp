#ifndef	INNERCOMMON_H
#define INNERCOMMON_H

#if defined( linux )
#else
#pragma warning( disable: 4819 )
#endif

#define		_SCL_SECURE_NO_WARNINGS

#define		IN
#define		OUT

#include	"OSWrapper.h"
#include	"ELErrLog.h"

#include	"math.h"

#include	<fstream>
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
#if 0 // del by wxl
#include	<boost/bind.hpp>
#include	<boost/signals2.hpp>
#include	<boost/shared_ptr.hpp>
#include	<boost/shared_array.hpp>
#endif
//using namespace boost;
//using namespace boost::signals2;

#include	"nds_sqlite3.h"

#include	"IOLibCommon_HeaderAll.h"

#include	"IOLibNDS_HeaderAll.h"

#include	"CFHeaderAll.hpp"

#include	"SL_HeaderAll.h"

#include	"DPHeaderAll.hpp"

#endif	//	INNERCOMMON_H
