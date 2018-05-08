#ifndef	INNERCOMMON_H
#define INNERCOMMON_H

#if defined( linux )
#else
#pragma warning( disable : 4786 )
#endif

#include	"OSWrapper.h"

#if defined( linux )
#define	 _atoi64					atoll
#define	 _snprintf					snprintf
#define	 sprintf_s					sprintf
#define	 _vsnprintf					vsnprintf
#define	 _vsnprintf					vsnprintf
#define	 _vsnprintf_s				vsnprintf
#define	 _vsnwprintf				vsnwprintf

#endif  //  #if defined( linux )

#if __ANDROID__

#include <android/log.h>
#define LOG_TAG "ZNavi"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

#endif

#endif	//	INNERCOMMON_H
