#pragma once

class CDPProvFolderUpdateRegion{};
class CDPProvFolderProduct :
	public CBaseObj
{
public:
	CDPProvFolderProduct() : m_bDbSwitching(false) {}
	virtual ~CDPProvFolderProduct() {}

	bool Initialize(string strProductName, std::shared_ptr< CDPDBConnectionPool > spclDBConnectionPool);

	std::shared_ptr<CDPProvProduct> GetProvProduct();
public:
	std::shared_ptr< CDPDBConnectionPool >						m_spclDBConnectionPool;
	volatile bool												m_bDbSwitching;
	std::shared_ptr< CDPProvProduct >							m_spclDPProvProduct;
	CCFSimpleCache< string, CDPProvFolderUpdateRegion >			m_clDPProvFolderUpdateRegionCache;
	string														m_strProductName;
};

