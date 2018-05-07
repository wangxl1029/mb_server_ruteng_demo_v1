
////////////////////////////////////////////////////////////////////////////////
//
// All Rights Reserved, Copyright(C),2013
// Licenced Material of by zhaoyiwei
// This file is auto generated by IOLibMaker, Do NOT modify
//
////////////////////////////////////////////////////////////////////////////////
//
// SubSystem: NDS_UpdVersionSnapshotNDS_UpdVersionSnapshotのデータアクセス用のライブラリ
// Class Name:    NDS_UpdVersionSnapshotNDS_UpdVersionSnapshotの書き込む用のクラス
//
////////////////////////////////////////////////////////////////////////////////
#include	"InnerCommon.h"
#include	"NDS_UpdVersionSnapshot_Struct.h"
#include	"NDS_UpdVersionSnapshot_OutsideRef.h"
#include	"NDS_UpdVersionSnapshot_InsideRef.h"
#include	"NDS_UpdVersionSnapshot_FormatStructure.h"
#include	"NDS_UpdVersionSnapshot_Writer.h"


////////////////////////////////////////////////////////////////////////////////
//
// Function: NDS_UpdVersionSnapshotNDS_UpdVersionSnapshotコンストラクター
// Detail: NDS_UpdVersionSnapshotNDS_UpdVersionSnapshotクラスの初期化
// Return:None
////////////////////////////////////////////////////////////////////////////////
CNDS_UpdVersionSnapshot_Writer::CNDS_UpdVersionSnapshot_Writer()
{
}

////////////////////////////////////////////////////////////////////////////////
//
// Function: NDS_UpdVersionSnapshotNDS_UpdVersionSnapshotコンストラクター
// Detail: NDS_UpdVersionSnapshotNDS_UpdVersionSnapshotクラスの初期化
// Return:None
////////////////////////////////////////////////////////////////////////////////
CNDS_UpdVersionSnapshot_Writer::CNDS_UpdVersionSnapshot_Writer( SmartPointer<CMyOfstream> &pclStream, SmartPointer<CBaseFormatStructure> &pclFormatStructure )
	: CIOLibBaseWriter( pclStream, pclFormatStructure )
{
}


////////////////////////////////////////////////////////////////////////////////
//
// Function: NDS_UpdVersionSnapshotNDS_UpdVersionSnapshotディストラクター
// Detail: NDS_UpdVersionSnapshotNDS_UpdVersionSnapshotクラスの削除
// Return:None
////////////////////////////////////////////////////////////////////////////////
CNDS_UpdVersionSnapshot_Writer::~CNDS_UpdVersionSnapshot_Writer()
{
}


////////////////////////////////////////////////////////////////////////////////
//
// Function: CreateFormatStructure
// Detail: Create meta data class of format for WriteBack.
// Return:int 型
//               SUCCESS  : (Succeeded)             
//               FAILURE : (Failed)
////////////////////////////////////////////////////////////////////////////////
int CNDS_UpdVersionSnapshot_Writer::CreateFormatStructure( SmartPointer<CBaseFormatStructure> &spclFormatStructure )
{
	spclFormatStructure.CreateEx( Type2Type<CNDS_UpdVersionSnapshot_FormatStructure>() );
	if( NULL == m_pclFormatStructure.Get() ) {
		assert( 0 );
		return FAILURE;
	}
	return SUCCESS;
}



////////////////////////////////////////////////////////////////////////////////
//
// Function: Write_NDS_UpdVSID4
// Detail: 該当するフォーマットのNDS_UpdVSID4レコードを書き込む
// Return:long 型
//               SUCCESS  : (Succeeded)             
//               FAILURE : (Failed)
////////////////////////////////////////////////////////////////////////////////
long CNDS_UpdVersionSnapshot_Writer::WriteNDS_UpdVSID4( const CNDS_UpdVSID4 &clNDS_UpdVSID4 )
{
	CFormatStructureNotifier	clNotifier( m_pclFormatStructure, StructType_NDS_UpdVersionSnapshot_NDS_UpdVSID4 );

	NDS_UpdVersionSnapshot_CountNDS_UpdVSID4_InsideRefElem( m_pclFormatStructure );

	if( SUCCESS != m_pclStream->Put4bN( clNDS_UpdVSID4.m_uiID ) ) {
		assert( 0 );
		return FAILURE;
	}

	if( SUCCESS != m_pclStream->Put2bN( clNDS_UpdVSID4.m_usVersionId ) ) {
		assert( 0 );
		return FAILURE;
	}

	return SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function: Write_NDS_UpdVSID4List
// Detail: 該当するフォーマットのNDS_UpdVSID4Listレコードを書き込む
// Return:long 型
//               SUCCESS  : (Succeeded)             
//               FAILURE : (Failed)
////////////////////////////////////////////////////////////////////////////////
long CNDS_UpdVersionSnapshot_Writer::WriteNDS_UpdVSID4List( const CNDS_UpdVSID4List &clNDS_UpdVSID4List )
{
	CFormatStructureNotifier	clNotifier( m_pclFormatStructure, StructType_NDS_UpdVersionSnapshot_NDS_UpdVSID4List );

	CNDS_UpdVersionSnapshot_NDS_UpdVSID4List_InsideRef 	*pclInsideRef	=	NDS_UpdVersionSnapshot_GetCurNDS_UpdVSID4List_InsideRef( m_pclFormatStructure );

	pclInsideRef->m_ullIDCnt_DA	=	m_pclStream->GetDA();
	if( SUCCESS != m_pclStream->Put4bN( clNDS_UpdVSID4List.m_uiIDCnt ) ) {
		assert( 0 );
		return FAILURE;
	}

	if( SUCCESS != WriteNDS_UpdVSID4( clNDS_UpdVSID4List.m_clIDList ) ) {
		assert( 0 );
		return FAILURE;
	}

	pclInsideRef->m_uiIDList_COUNT	=	1;

	return SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function: Write_NDS_UpdVersionSnapshotDataFrame
// Detail: 該当するフォーマットのNDS_UpdVersionSnapshotDataFrameレコードを書き込む
// Return:long 型
//               SUCCESS  : (Succeeded)             
//               FAILURE : (Failed)
////////////////////////////////////////////////////////////////////////////////
long CNDS_UpdVersionSnapshot_Writer::WriteNDS_UpdVersionSnapshotDataFrame( const CNDS_UpdVersionSnapshotDataFrame &clNDS_UpdVersionSnapshotDataFrame )
{
	CFormatStructureNotifier	clNotifier( m_pclFormatStructure, StructType_NDS_UpdVersionSnapshot_NDS_UpdVersionSnapshotDataFrame );

	CNDS_UpdVersionSnapshot_NDS_UpdVersionSnapshotDataFrame_InsideRef 	*pclInsideRef	=	NDS_UpdVersionSnapshot_GetCurNDS_UpdVersionSnapshotDataFrame_InsideRef( m_pclFormatStructure );

	if( SUCCESS != m_pclStream->Put2bN( clNDS_UpdVersionSnapshotDataFrame.m_usCRC ) ) {
		assert( 0 );
		return FAILURE;
	}

	bool	bIsBmdTile_Written	=	false;
	bIsBmdTile_Written	=	true;
	if( SUCCESS != WriteNDS_UpdVSID4List( clNDS_UpdVersionSnapshotDataFrame.m_clBmdTile ) ) {
		assert( 0 );
		return FAILURE;
	}
	if( !bIsBmdTile_Written ) {
		Skip( StructType_NDS_UpdVersionSnapshot_NDS_UpdVSID4List );
	}


	bool	bIsBmdIcon_Written	=	false;
	bIsBmdIcon_Written	=	true;
	if( SUCCESS != WriteNDS_UpdVSID4List( clNDS_UpdVersionSnapshotDataFrame.m_clBmdIcon ) ) {
		assert( 0 );
		return FAILURE;
	}
	if( !bIsBmdIcon_Written ) {
		Skip( StructType_NDS_UpdVersionSnapshot_NDS_UpdVSID4List );
	}


	bool	bIsRoutingTile_Written	=	false;
	bIsRoutingTile_Written	=	true;
	if( SUCCESS != WriteNDS_UpdVSID4List( clNDS_UpdVersionSnapshotDataFrame.m_clRoutingTile ) ) {
		assert( 0 );
		return FAILURE;
	}
	if( !bIsRoutingTile_Written ) {
		Skip( StructType_NDS_UpdVersionSnapshot_NDS_UpdVSID4List );
	}


	bool	bIsRoutingIcon_Written	=	false;
	bIsRoutingIcon_Written	=	true;
	if( SUCCESS != WriteNDS_UpdVSID4List( clNDS_UpdVersionSnapshotDataFrame.m_clRoutingIcon ) ) {
		assert( 0 );
		return FAILURE;
	}
	if( !bIsRoutingIcon_Written ) {
		Skip( StructType_NDS_UpdVersionSnapshot_NDS_UpdVSID4List );
	}


	if( SUCCESS != WriteNDS_UpdVSID4List( clNDS_UpdVersionSnapshotDataFrame.m_clNamedObject ) ) {
		assert( 0 );
		return FAILURE;
	}

	return SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function: CalcSize_NDS_UpdVSID4
// Detail: NDS_UpdVSID4を書き込むSizeを計算する
// Return:long 型
//               SUCCESS  : (Succeeded)             
//               FAILURE : (Failed)
////////////////////////////////////////////////////////////////////////////////
uint64 CNDS_UpdVersionSnapshot_Writer::CalSizeNDS_UpdVSID4( const CNDS_UpdVSID4 &clNDS_UpdVSID4 )
{
	uint64	ullResultSize	=	0;
	ullResultSize	+=	CMyOfstream::CalSize4bN( clNDS_UpdVSID4.m_uiID );

	ullResultSize	+=	CMyOfstream::CalSize2bN( clNDS_UpdVSID4.m_usVersionId );

	return ullResultSize;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function: CalcSize_NDS_UpdVSID4List
// Detail: NDS_UpdVSID4Listを書き込むSizeを計算する
// Return:long 型
//               SUCCESS  : (Succeeded)             
//               FAILURE : (Failed)
////////////////////////////////////////////////////////////////////////////////
uint64 CNDS_UpdVersionSnapshot_Writer::CalSizeNDS_UpdVSID4List( const CNDS_UpdVSID4List &clNDS_UpdVSID4List )
{
	uint64	ullResultSize	=	0;
	ullResultSize	+=	CMyOfstream::CalSize4bN( clNDS_UpdVSID4List.m_uiIDCnt );

	ullResultSize	+=	CalSizeNDS_UpdVSID4( clNDS_UpdVSID4List.m_clIDList );

	return ullResultSize;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function: CalcSize_NDS_UpdVersionSnapshotDataFrame
// Detail: NDS_UpdVersionSnapshotDataFrameを書き込むSizeを計算する
// Return:long 型
//               SUCCESS  : (Succeeded)             
//               FAILURE : (Failed)
////////////////////////////////////////////////////////////////////////////////
uint64 CNDS_UpdVersionSnapshot_Writer::CalSizeNDS_UpdVersionSnapshotDataFrame( const CNDS_UpdVersionSnapshotDataFrame &clNDS_UpdVersionSnapshotDataFrame )
{
	uint64	ullResultSize	=	0;
	ullResultSize	+=	CMyOfstream::CalSize2bN( clNDS_UpdVersionSnapshotDataFrame.m_usCRC );

	ullResultSize	+=	CalSizeNDS_UpdVSID4List( clNDS_UpdVersionSnapshotDataFrame.m_clBmdTile );

	ullResultSize	+=	CalSizeNDS_UpdVSID4List( clNDS_UpdVersionSnapshotDataFrame.m_clBmdIcon );

	ullResultSize	+=	CalSizeNDS_UpdVSID4List( clNDS_UpdVersionSnapshotDataFrame.m_clRoutingTile );

	ullResultSize	+=	CalSizeNDS_UpdVSID4List( clNDS_UpdVersionSnapshotDataFrame.m_clRoutingIcon );

	ullResultSize	+=	CalSizeNDS_UpdVSID4List( clNDS_UpdVersionSnapshotDataFrame.m_clNamedObject );

	return ullResultSize;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function: SolveInsideRef
// Detail: 該当するフォーマットの中の関係(offsetと数など)ををすべてWriteBack
// Return:long 型
//               SUCCESS  : (Succeeded)             
//               FAILURE : (Failed)
////////////////////////////////////////////////////////////////////////////////

int CNDS_UpdVersionSnapshot_Writer::SolveInsideRef()
{
	CNDS_UpdVersionSnapshot_NDS_UpdVersionSnapshotDataFrame_InsideRef		&clNDS_UpdVersionSnapshotDataFrame	=	((CNDS_UpdVersionSnapshot_FormatStructure*)m_pclFormatStructure.Get())->GetInsideRefInfo();
	{
		CNDS_UpdVersionSnapshot_NDS_UpdVSID4List_InsideRef	&clNDS_UpdVersionSnapshotDataFrameBmdTile	=	clNDS_UpdVersionSnapshotDataFrame.m_clBmdTile;
		if( SUCCESS != SolveNDS_UpdVSID4ListInsideRef( clNDS_UpdVersionSnapshotDataFrameBmdTile ) ) {
			assert( 0 );
			return FAILURE;
		}
	}
	{
		CNDS_UpdVersionSnapshot_NDS_UpdVSID4List_InsideRef	&clNDS_UpdVersionSnapshotDataFrameBmdIcon	=	clNDS_UpdVersionSnapshotDataFrame.m_clBmdIcon;
		if( SUCCESS != SolveNDS_UpdVSID4ListInsideRef( clNDS_UpdVersionSnapshotDataFrameBmdIcon ) ) {
			assert( 0 );
			return FAILURE;
		}
	}
	{
		CNDS_UpdVersionSnapshot_NDS_UpdVSID4List_InsideRef	&clNDS_UpdVersionSnapshotDataFrameRoutingTile	=	clNDS_UpdVersionSnapshotDataFrame.m_clRoutingTile;
		if( SUCCESS != SolveNDS_UpdVSID4ListInsideRef( clNDS_UpdVersionSnapshotDataFrameRoutingTile ) ) {
			assert( 0 );
			return FAILURE;
		}
	}
	{
		CNDS_UpdVersionSnapshot_NDS_UpdVSID4List_InsideRef	&clNDS_UpdVersionSnapshotDataFrameRoutingIcon	=	clNDS_UpdVersionSnapshotDataFrame.m_clRoutingIcon;
		if( SUCCESS != SolveNDS_UpdVSID4ListInsideRef( clNDS_UpdVersionSnapshotDataFrameRoutingIcon ) ) {
			assert( 0 );
			return FAILURE;
		}
	}
	{
		CNDS_UpdVersionSnapshot_NDS_UpdVSID4List_InsideRef	&clNDS_UpdVersionSnapshotDataFrameNamedObject	=	clNDS_UpdVersionSnapshotDataFrame.m_clNamedObject;
		if( SUCCESS != SolveNDS_UpdVSID4ListInsideRef( clNDS_UpdVersionSnapshotDataFrameNamedObject ) ) {
			assert( 0 );
			return FAILURE;
		}
	}
	return SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function: WriteAll
// Detail: 該当するファイルをすべて読み込む
// Return:long 型
//               SUCCESS  : (Succeeded)             
//               FAILURE : (Failed)
////////////////////////////////////////////////////////////////////////////////
long CNDS_UpdVersionSnapshot_Writer::WriteAll( const tchar *ptcFileName, const CNDS_UpdVersionSnapshotDataFrame &clNDS_UpdVersionSnapshotDataFrame )
{
	if( SUCCESS != Initialize( ptcFileName ) ) {
		assert( 0 );
		return FAILURE;
	}

	if( SUCCESS != WriteNDS_UpdVersionSnapshotDataFrame( clNDS_UpdVersionSnapshotDataFrame) ) {
		assert( 0 );
		return FAILURE;
	}

	if( SUCCESS != SolveInsideRef() ) {
		assert( 0 );
		return FAILURE;
	}

	return SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function: WriteAll
// Detail: 該当するファイルをすべて読み込む
// Return:long 型
//               SUCCESS  : (Succeeded)             
//               FAILURE : (Failed)
////////////////////////////////////////////////////////////////////////////////
long CNDS_UpdVersionSnapshot_Writer::WriteAll( const CNDS_UpdVersionSnapshotDataFrame &clNDS_UpdVersionSnapshotDataFrame )
{
	if( SUCCESS != Initialize() ) {
		assert( 0 );
		return FAILURE;
	}

	if( SUCCESS != WriteNDS_UpdVersionSnapshotDataFrame( clNDS_UpdVersionSnapshotDataFrame) ) {
		assert( 0 );
		return FAILURE;
	}

	if( SUCCESS != SolveInsideRef() ) {
		assert( 0 );
		return FAILURE;
	}

	return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function: SolveNDS_UpdVSID4ListInsideRef
// Detail: NDS_UpdVSID4Listレコードの中の関係(offsetと数など)をWriteBack
// Return:long 型
//               SUCCESS  : (Succeeded)             
//               FAILURE : (Failed)
////////////////////////////////////////////////////////////////////////////////

int CNDS_UpdVersionSnapshot_Writer::SolveNDS_UpdVSID4ListInsideRef( CNDS_UpdVersionSnapshot_NDS_UpdVSID4List_InsideRef &clNDS_UpdVSID4List )
{
	if( 0xFFFFFFFFFFFFFFFF != clNDS_UpdVSID4List.m_ullIDCnt_DA ) {
		m_pclStream->SetDA( clNDS_UpdVSID4List.m_ullIDCnt_DA );
		if( SUCCESS != m_pclStream->Put4bN( clNDS_UpdVSID4List.m_uiIDList_COUNT ) ) {
			assert( 0 );
			return FAILURE;
		}
	}
	return SUCCESS;
}
