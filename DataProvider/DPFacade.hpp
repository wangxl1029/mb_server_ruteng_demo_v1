#pragma once
#include <memory>
#include "CFBaseObj.hpp"
class CDPFacade :
	public CBaseObj
{
public:
	CDPFacade() {};
	virtual ~CDPFacade() {};

	static std::shared_ptr<CDPFacade> Create();

	virtual bool Initialize() = 0;

};

