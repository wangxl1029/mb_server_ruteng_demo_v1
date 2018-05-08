#include	"InnerCommon.h"
#include	"OSWrapper.h"

unsigned int PF_GetTickCount()
{
#if defined( linux )
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#else
	return GetTickCount();
#endif
}

void PF_Sleep( unsigned int uiTime )
{
#if defined( linux )
	sleep( uiTime );
#else
	Sleep( uiTime );
#endif
}

void PF_TinySleep( unsigned int uiTime )
{
#if defined( linux )
	usleep( uiTime );
#else
	Sleep( uiTime < 1000 ? 1 : uiTime / 1000 );
#endif
}

bool PF_RemoveFile( const char *pcFileName )
{
#if defined( linux )
	remove( pcFileName );
#else
	::DeleteFile( pcFileName );
#endif
	return true;
}

bool PF_IsFileExisting( const char *pcFileName )
{
#if defined( linux )
	return access( pcFileName, 0 ) == 0;
#else
	return _access( pcFileName, 0 ) == 0;
#endif
}

static int PF_Inner_CreateDirectory( const char *path )
{
	int i, len;

	len = strlen(path);
	//char dir_path[len+1] = '\0';
	char	dir_path[1024] = "\0";

	strncpy(dir_path, path, len);

	for( i=0; i<len; i++ ) {
		if( dir_path[i] == '/' && i > 0 ) {
			dir_path[i]='\0';
			if( access( dir_path, /*F_OK*/0 ) < 0) {
#if defined( linux )
				if( mkdir( dir_path, 0755 ) < 0) {
#else
				if( mkdir( dir_path ) < 0) {
#endif
					return -1;
				}
			}
			dir_path[i]='/';
		}
	}

	return 0;
}

bool PF_CreateDirectory( const char *pcDirName )
{
	return PF_Inner_CreateDirectory( pcDirName ) == 0;
}

PF_ThreadIdType PF_GetCurrentThreadId()
{
#if defined( linux )
#else
	return GetCurrentThreadId();
#endif
}
