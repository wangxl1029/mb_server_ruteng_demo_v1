#pragma once
#include <memory>
#include "CFBaseObj.hpp"
#include "CFSimpleCache.hpp"
#include "DPDBConnectionPool.hpp"
#include "DPProvProduct.hpp"

class CDPProvFolderUpdateRegion{};
class CDPProvFolderProduct :
	public CBaseObj
{
public:
	CDPProvFolderProduct() : m_bDbSwitching(false) {}
	virtual ~CDPProvFolderProduct() {}

	bool Initialize(std::string strProductName, std::shared_ptr< CDPDBConnectionPool > spclDBConnectionPool);

	std::shared_ptr<CDPProvProduct> GetProvProduct();
public:
	std::shared_ptr< CDPDBConnectionPool >						m_spclDBConnectionPool;
	volatile bool												m_bDbSwitching;
	CCFSimpleCache< std::string, CDPProvFolderUpdateRegion >	m_clDPProvFolderUpdateRegionCache;
	std::string													m_strProductName;
};

