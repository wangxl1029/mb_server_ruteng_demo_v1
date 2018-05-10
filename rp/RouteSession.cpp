#include "InnerCommon.hpp"
#include "RPCommon.hpp"
#include "RPRouteCalcRequest.hpp"
#include "RPRCTermSearch.hpp"
#include "RPRCSectionDirector.hpp"
#include "RouteSession.hpp"
#include "DPFacade.hpp"
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

// reference pattern : CMNRP::SetDestinationByTwoCoord()
bool CRouteSession::calcRoute(int iFromX, int iFromY, int iToX, int iToY)
{
	CRPWayPoint		clVehicleWayPoint;
	clVehicleWayPoint.m_enWayPointType = RPWayPointType_Coord;
	clVehicleWayPoint.m_clCoord.x = iFromX;
	clVehicleWayPoint.m_clCoord.y = iFromY;

	CRPWayPoint		clDestination;
	clDestination.m_enWayPointType = RPWayPointType_Coord;
	clDestination.m_clCoord.x = iToX;
	clDestination.m_clCoord.y = iToY;

	CRPRouteCalcRequest	clRequest;
	if (!clRequest.m_spclWayPoints.Create()) {
		ERR("");
		return false;
	}
	clRequest.m_spclWayPoints->PushBack(clVehicleWayPoint);
	clRequest.m_spclWayPoints->PushBack(clDestination);

	auto spclDPFacade = CDPFacade::Create();
	spclDPFacade->Initialize();

	SmartPointer< CRPRCSectionDirector >	spclSectionDirector; // refer to CRPRCRouteDirector::Initialize()
	spclSectionDirector->Initialize(spclDPFacade);
	spclSectionDirector->StartCalculateSection(clRequest);

	return false;
}