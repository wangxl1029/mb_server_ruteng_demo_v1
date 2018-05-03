#pragma once
#include <stdint.h>

class CDPCommon :
	public CBaseObj
{
public:
	CDPCommon() {};
	virtual ~CDPCommon() {};
	// ����Level���ź�NDS��γ�ȼ���TileNum
	static bool CoordToTileNo(int iLevelNo, int iLong, int iLat, uint32_t & uiTileNo);

	// ȡ��PackedTileId
	static bool TileNoToPackedTileID(int iLevelNo, uint32_t uiTileNo, uint32_t& uiPackedTileId);
};

