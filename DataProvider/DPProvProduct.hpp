#pragma once
#include "CFBaseObj.hpp"
#include "DPCommon.hpp"
class CDPProvProduct :
	public CBaseObj
{
public:
	CDPProvProduct();
	virtual ~CDPProvProduct();

	bool GetUpdateRegionByTile(BUILDING_BLOCK_ID enBuildingBlockID, uint32_t uiPackedTileID, std::vector< std::string > &vstrUpdateRegionList);
};

