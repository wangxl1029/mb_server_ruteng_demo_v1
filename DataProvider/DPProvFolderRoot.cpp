#include "DPProvFolderRoot.hpp"

bool CDPProvFolderRoot::Initialize(std::shared_ptr< CDPDBConnectionPool > spclDBConnectionPool)
{
	m_spclDBConnectionPool = spclDBConnectionPool;
//	m_clDPProvFolderProductCache.SetCapacity(10);
	return true;
}

std::shared_ptr<CDPProvFolderProduct>  CDPProvFolderRoot::GetFolderProduct(std::string strProductName)
{
	return nullptr;
}

