#pragma once

class CDPProvFolderRoot : // class name same as znavi
	public CBaseObj
{
public:
	CDPProvFolderRoot() : m_bDbSwitching(false) {}
	virtual ~CDPProvFolderRoot() {}

	bool Initialize(std::shared_ptr< CDPDBConnectionPool > spclDBConnectionPool);
	std::shared_ptr<CDPProvFolderProduct> GetFolderProduct(string strProductName);
public:
public:
	std::shared_ptr< CDPDBConnectionPool >						m_spclDBConnectionPool;
	CCFSimpleCache<string, CDPProvFolderProduct >			m_clDPProvFolderProductCache;
	volatile bool												m_bDbSwitching;
};

