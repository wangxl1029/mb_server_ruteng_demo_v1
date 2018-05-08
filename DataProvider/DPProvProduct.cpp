#include "InnerCommon.hpp"
#include "DPCommon.hpp"
#include "DPDBConnectionPool.hpp"
#include "DPProvProduct.hpp"

bool CDPProvProduct::Initialize(std::string strProductName, std::shared_ptr< CDPDBConnectionPool > spclDBConnectionPool)
{
	m_spclDBConnectionPool = spclDBConnectionPool;
	m_strProductName = strProductName;

	if (m_spclDBConnectionPool == nullptr) {
		//ERR("");
		return false;
	}

	if ( ! m_clDatabase.Connect(m_spclDBConnectionPool, strProductName.c_str(), NULL, DPDB_PRODUCT)) {
		//ERR("");
		return false;
	}

	return false;
}

bool CDPProvProduct::GetUpdateRegionByTile(BUILDING_BLOCK_ID enBuildingBlockID, uint32_t uiPackedTileID, std::vector< std::string > &vstrUpdateRegionList)
{
	return false;
}