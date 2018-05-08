#pragma once

extern const char	*PRODUCT_NAME;

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
	// ����Level���ź�NDS��γ�ȼ���TileNum
	static bool CoordToTileNo(int iLevelNo, int iLong, int iLat, uint32_t & uiTileNo);

	// ȡ��ָ��Level����Tile�ľ�γ�ȿ��
	static bool GetTileWidth(int iLevelNo, uint32_t& uiTileWidth);

	// ȡ��PackedTileId
	static bool TileNoToPackedTileID(int iLevelNo, uint32_t uiTileNo, uint32_t& uiPackedTileId);

	// ȡ��ָ��TileNum�����½Ǿ�γ��
	static bool GetLBPointCoordOfTile(int iLevelNo, uint32_t uiTileNo, int& iLong, int& iLat);

	// ȡ��ָ��TileNum�����ϽǾ�γ��
	static bool GetRTPointCoordOfTile(int iLevelNo, uint32_t uiTileNo, int& iLong, int& iLat);

	// ȡ��ָ��TileNum�Ļ�׼�㾭γ��
	static bool GetBasePointCoordOfTile(int iLevelNo, uint32_t uiTileNo, int& iLong, int& iLat);

	// ����Tile�Ļ�׼����������滯������㾭γ��
	static RESULT NormCoordToCoord(int		iBasePointLong,
		int		iBasePointLat,
		int		iNormCoorX,
		int		iNormCoorY,
		int		iCoordShift,
		int&	iLongitude,
		int&	iLatitude);

	// ����Tile�Ļ�׼������;�γ�ȼ������滯����
	static RESULT CoordAndBaseToNormCoord(int		iBasePointLong,
		int		iBasePointLat,
		int		iLongitude,
		int		iLatitude,
		int		iCoordShift,
		int&	iNormCoorX,
		int&	iNormCoorY);

	// NDS7009 delete by huhuan for "DBcom��������" on 20140116 begin
	// ����Tile���ź����滯������㾭γ��
	// static RESULT GetLLByTileNormCoor(int	iLevelNo, 
	// 						uint	iTileNo, 
	// 						int		iNormCoorX, 
	// 						int		iNormCoorY, 
	// 						int		iCoordShift,
	// 						int&	iLongitude,
	// 						int&	iLatitude);

	// NDS7009 delete by huhuan for "DBcom��������" on 20140116 end

	// ����Tile���ź;�γ�ȼ������滯����
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

	// 1/2560�������ϵ��NDS����ϵת��
	static RESULT Cnv2560SecCoord2NdsCoord(int i2560SecLong, int i2560SecLat, int& iNdsLong, int& iNdsLat);

	// NDS����ϵ��1/2560�������ϵת��
	static RESULT CnvNdsCoordTo2560SecCoord(int iNdsLong, int iNdsLat, int& i2560SecLong, int& i2560SecLat);

	// ����Level���ź;�γ�ȼ���ȡ��PackedTileId
	static RESULT CoordToPackedTileIDBy2560Sec(int iLevelNo, int i2560Long, int i2560Lat, uint& uiPackedTileId);

	// ����ָ��PackedTileId�Ļ�׼�㾭γ��
	static RESULT CalcBasePointCoordOfTile(uint uiPackedTileId, int& iNdsLong, int& iNdsLat);

	// ����ָ��PackedTileId�����¾�γ��
	static RESULT CalcLBPointCoordOfTile(uint uiPackedTileId, int& iLBNdsLong, int& iLBNdsLat);

	// ��NDS��γ��ת��Ϊ���滯����
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
