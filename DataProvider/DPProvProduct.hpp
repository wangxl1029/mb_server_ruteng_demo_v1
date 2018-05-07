#pragma once
#include <string>
#include <memory>
#include "CFBaseObj.hpp"
#include "DPCommon.hpp"
#include "DPDBConnectionPool.hpp"
class CDPProvProduct :
	public CBaseObj
{
public:
	CDPProvProduct();
	virtual ~CDPProvProduct();

	bool Initialize(std::string strProductName, std::shared_ptr< CDPDBConnectionPool > spclDBConnectionPool);
	bool GetUpdateRegionByTile(BUILDING_BLOCK_ID enBuildingBlockID, uint32_t uiPackedTileID, std::vector< std::string > &vstrUpdateRegionList);
};

