#pragma once
#include <memory>
#include "CFBaseObj.hpp"
#include "DPProvProduct.hpp"
class CDPProvFolderProduct :
	public CBaseObj
{
public:
	CDPProvFolderProduct() : m_bDbSwitching(false) {}
	virtual ~CDPProvFolderProduct() {}

	//bool Initialize(std::string strProductName, std::shared_ptr< CDPDBConnectionPool > spclDBConnectionPool);

	std::shared_ptr<CDPProvProduct> GetProvProduct();
public:
	volatile bool												m_bDbSwitching;
};

