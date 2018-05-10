#pragma once

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
	virtual ~CRPWayPoint();
	bool operator==(const CRPWayPoint &clRight) = delete;
public:
	RPWayPointType												m_enWayPointType;
	CPointCoord2D												m_clCoord;
};

class CRPWayPoints : public CBaseObj
{
public:
	CRPWayPoints();
	virtual ~CRPWayPoints();
public:
	size_t Size();
	CRPWayPoint& At(size_t uiNo);
	void PushBack(const CRPWayPoint &clWayPoint);
public:
	vector< CRPWayPoint >										m_vclWayPointList;
};

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

