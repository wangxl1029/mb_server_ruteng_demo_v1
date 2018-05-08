#ifndef	INNERCOMMON_H
#define INNERCOMMON_H

#if defined( linux )
#else
#pragma warning( disable : 4786 )
#endif

#include	"OSWrapper.h"

#if defined( linux )
#define	_atoi64					atoll
#define	_snprintf				snprintf
#define	sprintf_s				sprintf
#define	_vsnprintf				vsnprintf
#define	_vsnprintf				vsnprintf
#define	_vsnprintf_s			vsnprintf
#define	_vsnwprintf				vsnwprintf

inline void OutputDebugString( char *p )
{
	printf( "%s", p );
}

#else

#include	<direct.h>

#endif  //  #if defined( linux )

#endif	//	INNERCOMMON_H
