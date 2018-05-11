#pragma once


class CRPRouteCalcRequest
{
public:
	CRPRouteCalcRequest(SmartPointer< CRPWayPoints > spclWayPoints = SmartPointer< CRPWayPoints >(),
		uint uiSearchType = RP_ROUTE_SEARCH_TYPE_RECOMMEND,
		bool bQuickRoute = false)
		: m_spclWayPoints(spclWayPoints),
		m_uiSearchType(uiSearchType),
		m_bQuickRoute(bQuickRoute)
	{
	}

public:
	SmartPointer< CRPWayPoints >								m_spclWayPoints;
	uint														m_uiSearchType;
	bool														m_bQuickRoute;
};

