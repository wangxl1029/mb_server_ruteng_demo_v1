#ifndef	PFSYSTEM_H
#define	PFSYSTEM_H

#if defined( linux )
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<sys/mman.h>
#include	<sys/wait.h>
#include	<errno.h>
#include	<fcntl.h>
#include	<unistd.h>
#include	<stddef.h>
#include	<pthread.h>
#include	<semaphore.h>
#include	<dirent.h>
#include	<sys/prctl.h>
#else
#include	<windows.h>
#include	<tchar.h>
#include	<io.h>
#endif

#include	<stdio.h>
#include	<stdlib.h>
#include	<stdarg.h>
#include	<assert.h>
#include	<memory.h>

#ifndef WXL_COMMENT_OUT
#include	"boost/bind.hpp"
#include	"boost/signals2.hpp"
#include	"boost/shared_ptr.hpp"
#include	"boost/shared_array.hpp"
#include	"boost/smart_ptr/detail/atomic_count.hpp"
#include	"boost/smart_ptr/detail/spinlock.hpp"
#endif

unsigned int PF_GetTickCount();

void PF_Sleep( unsigned int uiTime );
void PF_TinySleep( unsigned int uiTime );

bool PF_RemoveFile( const char *pcFileName );
bool PF_IsFileExisting( const char *pcFileName );
bool PF_CreateDirectory( const char *pcDirName );

typedef unsigned int	PF_ThreadIdType;
PF_ThreadIdType PF_GetCurrentThreadId();

#endif	//	PFSYSTEM_H
