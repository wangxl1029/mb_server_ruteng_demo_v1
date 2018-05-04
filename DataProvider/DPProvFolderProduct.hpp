#pragma once
#include <memory>
#include "CFBaseObj.hpp"
#include "DPProvProduct.hpp"
class CDPProvFolderProduct :
	public CBaseObj
{
public:
	CDPProvFolderProduct();
	virtual ~CDPProvFolderProduct();
public:
	std::shared_ptr<CDPProvProduct> GetProvProduct();
};

