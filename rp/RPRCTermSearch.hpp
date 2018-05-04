#pragma once
#include "DPFacade.hpp"
#include "RPRouteCalcRequest.hpp"
class CRPRCTermSearch :
	public CBaseObj
{
public:
	CRPRCTermSearch(CRPWayPoint &clWayPoint, int iLevel, std::shared_ptr< CDPFacade > spclDataProvider);
	CRPRCTermSearch() = delete;
	bool Do(std::shared_ptr< CDPFacade > spclDataProvider);
	virtual ~CRPRCTermSearch();
public:
	//	input
	CRPWayPoint													m_clWayPoint;
	int															m_iLevel;

	std::shared_ptr< CDPFacade >								m_spclDataProvider;
};

