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


	static int UpdateRegionName2Id(const char *pcUpdateRegionName);


};

