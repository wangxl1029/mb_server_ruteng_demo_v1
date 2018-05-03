#pragma once
#include <stdint.h>

class CDPCommon :
	public CBaseObj
{
public:
	CDPCommon() {};
	virtual ~CDPCommon() {};
	// 根据Level番号和NDS经纬度计算TileNum
	static bool CoordToTileNo(int iLevelNo, int iLong, int iLat, uint32_t & uiTileNo);

	// 取得指定Level单个Tile的经纬度跨度
	static bool GetTileWidth(int iLevelNo, uint32_t& uiTileWidth);

	// 取得PackedTileId
	static bool TileNoToPackedTileID(int iLevelNo, uint32_t uiTileNo, uint32_t& uiPackedTileId);

	// 取得指定TileNum的左下角经纬度
	static bool GetLBPointCoordOfTile(int iLevelNo, uint32_t uiTileNo, int& iLong, int& iLat);

	// 取得指定TileNum的右上角经纬度
	static bool GetRTPointCoordOfTile(int iLevelNo, uint32_t uiTileNo, int& iLong, int& iLat);

	// 取得指定TileNum的基准点经纬度
	static bool GetBasePointCoordOfTile(int iLevelNo, uint32_t uiTileNo, int& iLong, int& iLat);
};

