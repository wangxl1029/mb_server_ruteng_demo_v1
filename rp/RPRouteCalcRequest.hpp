#pragma once
#include <memory>

class CPointCoord2D
{
public:
	CPointCoord2D(int px = 0, int py = 0) : x(px), y(py) {}

	int															x;
	int															y;
};

enum RPWayPointType
{
	RPWayPointType_Coord,
	RPWayPointType_RSLink,
	RPWayPointType_Invalid
};


class CRPWayPoint : public CBaseObj
{
public:
	CRPWayPoint();
	CRPWayPoint(const CPointCoord2D &clCoord);
public:
	RPWayPointType												m_enWayPointType;
	CPointCoord2D												m_clCoord;
};


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

