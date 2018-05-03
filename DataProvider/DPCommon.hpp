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

	// 取得PackedTileId
	static bool TileNoToPackedTileID(int iLevelNo, uint32_t uiTileNo, uint32_t& uiPackedTileId);
};

