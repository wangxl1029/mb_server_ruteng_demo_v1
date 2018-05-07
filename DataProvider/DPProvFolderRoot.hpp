#pragma once
#include <string>
#include <memory>
#include "CFBaseObj.hpp"
#include "DPDBConnectionPool.hpp"
#include "DPProvFolderProduct.hpp"

class CDPProvFolderRoot : // class name same as znavi
	public CBaseObj
{
public:
	CDPProvFolderRoot() : m_bDbSwitching(false) {}
	virtual ~CDPProvFolderRoot() {}

	bool Initialize(std::shared_ptr< CDPDBConnectionPool > spclDBConnectionPool);
	std::shared_ptr<CDPProvFolderProduct> GetFolderProduct(std::string strProductName);
public:
public:
	std::shared_ptr< CDPDBConnectionPool >						m_spclDBConnectionPool;
	volatile bool												m_bDbSwitching;
};

