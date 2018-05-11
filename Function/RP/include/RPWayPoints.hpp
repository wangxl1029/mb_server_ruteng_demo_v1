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
	CRPWayPoint(const CRPRSLink &clRSLink);
	virtual ~CRPWayPoint();

	bool operator==(const CRPWayPoint &clRight);

public:
	RPWayPointType												m_enWayPointType;
	CPointCoord2D												m_clCoord;
	CRPRSLink													m_clLink;
};

class CRPWayPoints : public CBaseObj
{
public:
	CRPWayPoints();
	virtual ~CRPWayPoints();

	size_t Size();
	CRPWayPoint& At(size_t uiNo);
	void PushBack(const CRPWayPoint &clWayPoint);

public:
	vector< CRPWayPoint >										m_vclWayPointList;
};
