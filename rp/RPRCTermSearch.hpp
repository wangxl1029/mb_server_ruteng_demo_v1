#pragma once
#include "DPFacade.hpp"
class CRPRCTermSearch :
	public CBaseObj
{
public:
	CRPRCTermSearch(std::shared_ptr< CDPFacade > spclDataProvider);
	bool Do();
	virtual ~CRPRCTermSearch();
public:
	std::shared_ptr< CDPFacade >									m_spclDataProvider;
};

