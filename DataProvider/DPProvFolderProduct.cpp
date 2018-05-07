#include "DPProvFolderProduct.hpp"


bool CDPProvFolderProduct::Initialize(std::string strProductName, std::shared_ptr< CDPDBConnectionPool > spclDBConnectionPool)
{
	m_spclDBConnectionPool = spclDBConnectionPool;
	m_strProductName = strProductName;

	m_clDPProvFolderUpdateRegionCache.SetCapacity(10);

	return true;
}


std::shared_ptr< CDPProvProduct > CDPProvFolderProduct::GetProvProduct()
{
	if (m_bDbSwitching) {
		//ERR("");
		return nullptr;
	}

	if (nullptr == m_spclDPProvProduct) {
		auto spclDPProvProductTemp = std::make_shared<CDPProvProduct>();
		if ( ! spclDPProvProductTemp->Initialize(m_strProductName, m_spclDBConnectionPool)) {
			//ERR("");
			return nullptr;
		}
		m_spclDPProvProduct = spclDPProvProductTemp;
	}

	return m_spclDPProvProduct;
}