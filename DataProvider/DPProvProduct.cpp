#include "DPProvProduct.hpp"


CDPProvProduct::CDPProvProduct()
{
}


CDPProvProduct::~CDPProvProduct()
{
}

bool CDPProvProduct::Initialize(std::string strProductName, std::shared_ptr< CDPDBConnectionPool > spclDBConnectionPool)
{
	return false;
}

bool CDPProvProduct::GetUpdateRegionByTile(BUILDING_BLOCK_ID enBuildingBlockID, uint32_t uiPackedTileID, std::vector< std::string > &vstrUpdateRegionList)
{
	return false;
}