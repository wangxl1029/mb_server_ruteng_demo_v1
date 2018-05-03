#include "CFBaseObj.hpp"
#include "RPCommon.hpp"
#include "RPRouteCalcRequest.hpp"
#include "RPRCSectionDirector.hpp"
#include "RouteSession.hpp"

//	CRPWayPoint
CRPWayPoint::CRPWayPoint()
: m_enWayPointType(RPWayPointType_Invalid)
{
}

CRPWayPoint::CRPWayPoint(const CPointCoord2D &clCoord)
: m_enWayPointType(RPWayPointType_Coord), m_clCoord(clCoord)
{
}

CRPWayPoint::~CRPWayPoint()
{
}


CRouteSession::CRouteSession()
{
}

CRouteSession::~CRouteSession()
{
}

void CRouteSession::calcRoute()
{
	auto spclWayPoints = std::make_shared<CRPWayPoints>();
	spclWayPoints->m_vclWayPointList.emplace_back(CPointCoord2D(1389087203, 476456031));

	CRPRouteCalcRequest		req(spclWayPoints);
	
	CRPRouteCalcRequest		clSectionRequest(req);

	auto spclSessionDirector = std::make_shared<CRPRCSectionDirector>();
	spclSessionDirector->StartCalculateSection(clSectionRequest);
}