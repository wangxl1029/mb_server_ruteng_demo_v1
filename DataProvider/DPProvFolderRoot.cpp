#include "DPProvFolderRoot.hpp"


bool CDPProvFolderRoot::Initialize(std::shared_ptr< CDPDBConnectionPool > spclDBConnectionPool)
{
	m_spclDBConnectionPool = spclDBConnectionPool;
	m_clDPProvFolderProductCache.SetCapacity(10);
	return true;
}

std::shared_ptr<CDPProvFolderProduct>  CDPProvFolderRoot::GetFolderProduct(std::string strProductName)
{
	if (m_bDbSwitching) {
//		ERR("");
		return nullptr;
	}

	auto spclFolderProduct = m_clDPProvFolderProductCache.GetData(strProductName);
	if (nullptr == spclFolderProduct)
	{
		auto spclFolderProductTemp = std::make_shared< CDPProvFolderProduct >();
		if (spclFolderProductTemp->Initialize(strProductName, m_spclDBConnectionPool)) {
			//ERR("");
			return nullptr;
		}

		m_clDPProvFolderProductCache.PutData(strProductName, spclFolderProductTemp);
		spclFolderProduct = spclFolderProductTemp;
	}

	return spclFolderProduct;
}

