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

//	�������� float
class CPointWorld2D
{
public:
	CPointWorld2D(float px = 0.0f, float py = 0.0f) : x(px), y(py) {}

	float														x;
	float														y;
};

inline bool operator==(const CPointWorld2D &clLeft, const CPointWorld2D &clRight)
{
	return CCFUtil::FloatEqual(clLeft.x, clRight.x) && CCFUtil::FloatEqual(clLeft.y, clRight.y);
}

class CLineWorld2D
{
public:
	CLineWorld2D() {}
	CLineWorld2D(const CPointWorld2D &pp1, const CPointWorld2D &pp2) : p1(pp1), p2(pp2) {}

	CPointWorld2D												p1;
	CPointWorld2D												p2;
};

class CRectWorld2D
{
public:
	CRectWorld2D() {}
	CRectWorld2D(const CPointWorld2D &plb, const CPointWorld2D &prt) : lb(plb), rt(prt) {}

	CPointWorld2D												lb;
	CPointWorld2D												rt;
};

class CQuadWorld2D
{
public:
	CQuadWorld2D() {}
	CQuadWorld2D(const CPointWorld2D pp[4]) { for (size_t i = 0; i < 4; ++i) p[i] = pp[i]; }

	CPointWorld2D												p[4];
};

//	��γ������ int
class CPointCoord2D
{
public:
	CPointCoord2D(int px = 0, int py = 0) : x(px), y(py) {}

	int															x;
	int															y;
};

inline bool operator==(const CPointCoord2D &clLeft, const CPointCoord2D &clRight)
{
	return clLeft.x == clRight.x && clLeft.y == clRight.y;
}

class CRectCoord2D
{
public:
	CRectCoord2D() {}
	CRectCoord2D(const CPointCoord2D &plb, const CPointCoord2D &prt) : lb(plb), rt(prt) {}

	CPointCoord2D												lb;
	CPointCoord2D												rt;
};

class CQuadCoord2D
{
public:
	CQuadCoord2D() {}
	CQuadCoord2D(const CPointCoord2D pp[4]) { for (size_t i = 0; i < 4; ++i) p[i] = pp[i]; }

	CPointCoord2D												p[4];
};

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

static const BUILDING_BLOCK_ID		DP_BUILDING_BLOCK_NO_TO_ID[] = {
	BUILDING_BLOCK_ID_SHARED_DATA,
	BUILDING_BLOCK_ID_BASIC_MAP_DISPLAY,
	BUILDING_BLOCK_ID_ROUTING,
	BUILDING_BLOCK_ID_NAME,
	BUILDING_BLOCK_ID_POI,
	BUILDING_BLOCK_ID_OBJECTS_3D,
	BUILDING_BLOCK_ID_ORTHO_IMAGE,
	BUILDING_BLOCK_ID_DTM,
	BUILDING_BLOCK_ID_SPEECH,
	BUILDING_BLOCK_ID_JUNCTION_VIEW,
	BUILDING_BLOCK_ID_TRAFFIC_INFORMATION,
	BUILDING_BLOCK_ID_FTS,
	BUILDING_BLOCK_ID_ICON,
	BUILDING_BLOCK_ID_SLI,
	BUILDING_BLOCK_ID_NVC,
	BUILDING_BLOCK_ID_MAX,
	BUILDING_BLOCK_ID_EXTENSION
};

typedef size_t(*MP_BUILDING_BLOCK_ID_TO_NO_FUNCTION)(BUILDING_BLOCK_ID enBuildingBlockID);

template< MP_BUILDING_BLOCK_ID_TO_NO_FUNCTION pf >
class BuildingBlockIDToNo
{
public:
	BuildingBlockIDToNo(const BUILDING_BLOCK_ID *penBuildingBlockNoToID, size_t uiBuildingBlockCount)
		: m_penBuildingBlockNoToID(penBuildingBlockNoToID), m_uiBuildingBlockCount(uiBuildingBlockCount)
	{
	}
	size_t operator()(BUILDING_BLOCK_ID enBuildingBlockID)
	{
		static bool						s_bBuildingBlockIDToNoInit = false;
		static size_t					s_auiBuildingBlockIDToNoList[BUILDING_BLOCK_ID_MAX];

		if (!s_bBuildingBlockIDToNoInit) {
			for (size_t i = 0; i < BUILDING_BLOCK_ID_MAX; ++i) {
				for (size_t j = 0; j < m_uiBuildingBlockCount; ++j) {
					if (m_penBuildingBlockNoToID[j] == i) {
						s_auiBuildingBlockIDToNoList[i] = j;
						break;
					}
				}
			}
			s_bBuildingBlockIDToNoInit = true;
		}

		return s_auiBuildingBlockIDToNoList[enBuildingBlockID];
	}

	const BUILDING_BLOCK_ID				*m_penBuildingBlockNoToID;
	size_t								m_uiBuildingBlockCount;
};

const size_t						DP_BUILDING_BLOCK_COUNT = sizeof(DP_BUILDING_BLOCK_NO_TO_ID) / sizeof(DP_BUILDING_BLOCK_NO_TO_ID[0]);
size_t								DP_BUILDING_BLOCK_ID_TO_NO(BUILDING_BLOCK_ID enBuildingBlockID);

class CDPCommon :
	public CBaseObj
{
public:
	CDPCommon() {};
	virtual ~CDPCommon() {};
	// ����Level���ź�NDS��γ�ȼ���TileNum
	static RESULT CoordToTileNo(int iLevelNo, int iLong, int iLat, uint& uiTileNo);

	// ȡ��PackedTileId
	static RESULT TileNoToPackedTileID(int iLevelNo, uint uiTileNo, uint& uiPackedTileId);

	// ����PackedTileIdȡ��Leve���ź�TileNum
	static RESULT PackedTileIDToTileNo(uint uiPackedTileId, int& iLevelNo, uint& uiTileNo);

	// ����Level���ź�NDS��γ�ȼ���ȡ��PackedTileId
	static RESULT CoordToPackedTileID(int iLevelNo, int iNdsLong, int iNdsLat, uint& uiPackedTileId);

	static RESULT GetUpperTileID(uint uiPackedTileID, int iUpperLevel, uint &uiUpperTileID);

	// ȡ��ָ��Level����Tile�ľ�γ�ȿ��
	static RESULT GetTileWidth(int iLevelNo, uint& uiTileWidth);

	// ȡ��ָ��TileNum�����½Ǿ�γ��
	static RESULT GetLBPointCoordOfTile(int iLevelNo, uint uiTileNo, int& iLong, int& iLat);

	// ȡ��ָ��TileNum�����ϽǾ�γ��
	static RESULT GetRTPointCoordOfTile(int iLevelNo, uint uiTileNo, int& iLong, int& iLat);

	// ȡ��ָ��TileNum�Ļ�׼�㾭γ��
	static RESULT GetBasePointCoordOfTile(int iLevelNo, uint uiTileNo, int& iLong, int& iLat);

	// ���ݾ�γ��ȡ��Ī����
	static RESULT CoordToMortonCode(int iLong, int iLat, uint64& uiMortonCode);

	// ����Ī����ȡ�þ�γ��
	static RESULT MortonCodeToCoord(uint64 uiMortonCode, int& iLong, int& iLat);

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

