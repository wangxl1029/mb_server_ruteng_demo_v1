#include	"InnerCommon.h"
#include	"ELErrLog.h"

void PrintfErr( const char *pcFileName, int iLine, const char *a_format, ... )
{
	const char	*p	=	NULL;
	if( pcFileName != NULL ) {
		for( p = pcFileName + strlen( pcFileName ); p > pcFileName && *(p-1) != '/'; --p ) {}
	}

	char	s_acContent[1024+10]	=	"";
	char	*q	=	s_acContent;
	strcpy( q, "znavi: " );
	q	+=	strlen( q );
	if( p != NULL ) {
		sprintf( q, "[%s,%d] ", p, iLine );
		q	+=	strlen( q );
	}

	va_list vaList;
	va_start(vaList,a_format);
#if defined( linux )
	int		nConetentLen	=	_vsnprintf_s( q, sizeof(s_acContent)-(q-s_acContent), a_format, vaList );
#else
	int		nConetentLen = _vsnprintf_s(q, sizeof(s_acContent)-(q - s_acContent), sizeof(s_acContent)-(q - s_acContent), a_format, vaList);
#endif

	va_end(vaList);
#if __ANDROID__
	LOGD( s_acContent );
#else
	printf( "%s\n", s_acContent );
	fflush( stdout );
#endif
}
