#pragma once

extern const char	*DP_DATA_DIR_NAME;
extern const char	*DP_BACKUP_DIR_NAME;
extern const char	*DP_DOWNLOAD_DIR_NAME;
extern const char	*DP_UNPACKED_DIR_NAME;
extern const char	*DP_NDS_DB_SUFFIX;
extern const char	*DP_BACKUP_DB_SUFFIX;
extern const char	*DP_EXCLUDE_DB_SUFFIX;
extern const char	*DP_INSUPD_DB_SUFFIX;
extern const char	*DP_DELETE_DB_SUFFIX;
extern const char	*DP_INSUPD_RID_DB_SUFFIX;
extern const char	*DP_DELETE_RID_DB_SUFFIX;
extern const char	*DP_DELETE_RID_DUP_DB_SUFFIX;

extern const char	*PRODUCT_NAME;


const uint		NDS_LEVEL_MAX = 20;
const uint		NDS_SUB_LEVEL_MAX = 3;
const uint		NDS_DETAIL_LEVEL_MAX = 100;
const uint		NDS_INVALID_TILE_ID = 0;
const uint		NDS_INVALID_UPDATE_REGION_ID = 0;
const ushort	NDS_INVALID_LINK_ID = ((ushort)-1);
const ushort	NDS_INVALID_NODE_ID = ((ushort)-1);
const uint		NDS_INVALID_GATEWAY_ID = ((uint)-1);

#define	LINK_DIR_COUNT											(2)
#define	LINK_DIR_NO(_POS_)										((_POS_)?1:0)

#define		NEW_VERSION											(1)
#define		OLD_VERSION											(0)

const char* DP_GetRootDirName();

enum BUILDING_BLOCK_ID
{
	BUILDING_BLOCK_ID_SHARED_DATA = 0x0,
	BUILDING_BLOCK_ID_BASIC_MAP_DISPLAY = 0x1,
	BUILDING_BLOCK_ID_ROUTING = 0x2,
	BUILDING_BLOCK_ID_NAME = 0x3,
	BUILDING_BLOCK_ID_POI = 0x4,
	BUILDING_BLOCK_ID_OBJECTS_3D = 0x6,
	BUILDING_BLOCK_ID_ORTHO_IMAGE = 0x7,
	BUILDING_BLOCK_ID_DTM = 0x8,
	BUILDING_BLOCK_ID_SPEECH = 0x9,
	BUILDING_BLOCK_ID_JUNCTION_VIEW = 0xA,
	BUILDING_BLOCK_ID_TRAFFIC_INFORMATION = 0xB,
	BUILDING_BLOCK_ID_FTS = 0xC,
	BUILDING_BLOCK_ID_ICON = 0xD,
	BUILDING_BLOCK_ID_SLI = 0xE,
	BUILDING_BLOCK_ID_NVC = 0xF,
	BUILDING_BLOCK_ID_MAX,
	BUILDING_BLOCK_ID_EXTENSION = 0xFF
};

class CDPCommon :
	public CBaseObj
{
public:
	CDPCommon() {};
	virtual ~CDPCommon() {};
	// 根据Level番号和NDS经纬度计算TileNum
	static RESULT CoordToTileNo(int iLevelNo, int iLong, int iLat, uint& uiTileNo);

	// 取得PackedTileId
	static RESULT TileNoToPackedTileID(int iLevelNo, uint uiTileNo, uint& uiPackedTileId);

	// 根据PackedTileId取得Leve番号和TileNum
	static RESULT PackedTileIDToTileNo(uint uiPackedTileId, int& iLevelNo, uint& uiTileNo);

	// 根据Level番号和NDS经纬度计算取得PackedTileId
	static RESULT CoordToPackedTileID(int iLevelNo, int iNdsLong, int iNdsLat, uint& uiPackedTileId);

	static RESULT GetUpperTileID(uint uiPackedTileID, int iUpperLevel, uint &uiUpperTileID);

	// 取得指定Level单个Tile的经纬度跨度
	static RESULT GetTileWidth(int iLevelNo, uint& uiTileWidth);

	// 取得指定TileNum的左下角经纬度
	static RESULT GetLBPointCoordOfTile(int iLevelNo, uint uiTileNo, int& iLong, int& iLat);

	// 取得指定TileNum的右上角经纬度
	static RESULT GetRTPointCoordOfTile(int iLevelNo, uint uiTileNo, int& iLong, int& iLat);

	// 取得指定TileNum的基准点经纬度
	static RESULT GetBasePointCoordOfTile(int iLevelNo, uint uiTileNo, int& iLong, int& iLat);

	// 根据经纬度取得莫顿码
	static RESULT CoordToMortonCode(int iLong, int iLat, uint64& uiMortonCode);

	// 根据莫顿码取得经纬度
	static RESULT MortonCodeToCoord(uint64 uiMortonCode, int& iLong, int& iLat);

	// 根据Tile的基准点坐标和正规化坐标计算经纬度
	static RESULT NormCoordToCoord(int		iBasePointLong,
		int		iBasePointLat,
		int		iNormCoorX,
		int		iNormCoorY,
		int		iCoordShift,
		int&	iLongitude,
		int&	iLatitude);

	// 根据Tile的基准点坐标和经纬度计算正规化坐标
	static RESULT CoordAndBaseToNormCoord(int		iBasePointLong,
		int		iBasePointLat,
		int		iLongitude,
		int		iLatitude,
		int		iCoordShift,
		int&	iNormCoorX,
		int&	iNormCoorY);

	// NDS7009 delete by huhuan for "DBcom代码整合" on 20140116 begin
	// 根据Tile番号和正规化坐标计算经纬度
	// static RESULT GetLLByTileNormCoor(int	iLevelNo, 
	// 						uint	iTileNo, 
	// 						int		iNormCoorX, 
	// 						int		iNormCoorY, 
	// 						int		iCoordShift,
	// 						int&	iLongitude,
	// 						int&	iLatitude);

	// NDS7009 delete by huhuan for "DBcom代码整合" on 20140116 end

	// 根据Tile番号和经纬度计算正规化坐标
	static RESULT CoordAndTileToNormCoord(int	iLevelNo,
		uint	iTileNo,
		int		iLongitude,
		int		iLatitude,
		int		iCoordShift,
		int&	iNormCoorX,
		int&	iNormCoorY);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Coordination Convert Function
	//
	//	NNMF Coordination System To NDS Coordination System
	//
	//////////////////////////////////////////////////////////////////////////

	// 1/2560秒的坐标系向NDS坐标系转换
	static RESULT Cnv2560SecCoord2NdsCoord(int i2560SecLong, int i2560SecLat, int& iNdsLong, int& iNdsLat);

	// NDS坐标系向1/2560秒的坐标系转换
	static RESULT CnvNdsCoordTo2560SecCoord(int iNdsLong, int iNdsLat, int& i2560SecLong, int& i2560SecLat);

	// 根据Level番号和经纬度计算取得PackedTileId
	static RESULT CoordToPackedTileIDBy2560Sec(int iLevelNo, int i2560Long, int i2560Lat, uint& uiPackedTileId);

	// 计算指定PackedTileId的基准点经纬度
	static RESULT CalcBasePointCoordOfTile(uint uiPackedTileId, int& iNdsLong, int& iNdsLat);

	// 计算指定PackedTileId的左下经纬度
	static RESULT CalcLBPointCoordOfTile(uint uiPackedTileId, int& iLBNdsLong, int& iLBNdsLat);

	// 将NDS经纬度转换为正规化坐标
	static RESULT CnvNdsCoord2NormCoord(uint uiPackedTileId, int iNdsLong, int iNdsLat, int iCoordShift, int& iNormCoorX, int& iNormCoorY);

	static float ConvCoord2Meters(int iCoord, int iLat);
	static float ConvMeters2Coord(float fMeters, int iLat);
	static RESULT ConvNDSCoord2DMS(int iCoord, int &d, int &m, int &s);
	static RESULT ConvDMS2NDSCoord(int d, int m, int s, int &iCoord);

	// 	static size_t BuildingBlockCount();
	// 	static BUILDING_BLOCK_ID BuildingBlockNoToID( size_t uiBuildingBlockNo );
	// 	static size_t BuildingBlockIDToNo( BUILDING_BLOCK_ID enBuildingBlockID );

	static string UpdateRegionId2Name(int iUpdateRegionId);
	static int UpdateRegionName2Id(const char *pcUpdateRegionName);


};

