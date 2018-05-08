/****************************************************************************/
//                                                            
//   GNC System (General NDS Compiler System Platform)   	                   
//                                                            
//   All Rights Reserved,Copyright(C),2012	                   
//   Licensed Material of NEUSOFT,CO.,LTD.                    
//                                                            
/****************************************************************************/
/****************************************************************************/
/***
		GNC System
		: .

		date  : 2012/12/1
        author: ZhaoYiwei
****************************************************************************/
/***ModifyHistory                                                  
****************************************************************************/
/***--------------------------------------------------------------------------
No        |Modified by    |Modify date   |Memo
1          ZhaoYiwei       2012/12/1      New
--------------------------------------------------------------------------***/
/****************************************************************************/
#ifndef	PFERRLOG_H
#define	PFERRLOG_H

#define		ERR( format, ... )			PrintfErr( __FILE__, __LINE__, format, ##__VA_ARGS__ );assert( 0 )
#define		WARN5( format, ... )		PrintfErr( __FILE__, __LINE__, format, ##__VA_ARGS__ )
#define		WARN4( format, ... )		PrintfErr( __FILE__, __LINE__, format, ##__VA_ARGS__ )
#define		WARN3( format, ... )		PrintfErr( __FILE__, __LINE__, format, ##__VA_ARGS__ )
#define		WARN2( format, ... )		PrintfErr( __FILE__, __LINE__, format, ##__VA_ARGS__ )
#define		WARN1( format, ... )		PrintfErr( __FILE__, __LINE__, format, ##__VA_ARGS__ )
#define		LOGINFO( format, ... )		PrintfErr( __FILE__, __LINE__, format, ##__VA_ARGS__ )
#define		MESSAGE( format, ... )		PrintfErr( __FILE__, __LINE__, format, ##__VA_ARGS__ )

void PrintfErr( const char *pcFileName, int iLine, const char *a_format, ... );

#endif	//	PFERRLOG_H
