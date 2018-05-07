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
	return nullptr;
}