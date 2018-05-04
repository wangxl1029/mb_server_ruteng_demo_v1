#pragma once
#include <string>
#include <memory>
#include "CFBaseObj.hpp"
#include "DPProvFolderProduct.hpp"

class CDPProvFolderRoot : // class name same as znavi
	public CBaseObj
{
public:
	CDPProvFolderRoot();
	virtual ~CDPProvFolderRoot();

	bool Initialize();
	std::shared_ptr<CDPProvFolderProduct> GetFolderProduct(std::string strProductName);
public:
	volatile bool												m_bDbSwitching;
};

