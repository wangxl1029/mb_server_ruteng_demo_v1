#pragma once
#include <memory>

class CRPWayPoints
{
public:
	CRPWayPoints() {}
};

class CRPRouteCalcRequest
{
public:
	CRPRouteCalcRequest(std::shared_ptr<CRPWayPoints> spclWayPoints,
		uint16_t uiSearchType = RP_ROUTE_SEARCH_TYPE_RECOMMEND,
		bool bQuickRoute = false)
		: m_spclWayPoints(spclWayPoints)
		, m_uiSearchType(uiSearchType)
		, m_bQuickRoute(bQuickRoute)
	{
	}

public:
	std::shared_ptr<CRPWayPoints>								m_spclWayPoints;
	uint16_t													m_uiSearchType;
	bool														m_bQuickRoute;
};

