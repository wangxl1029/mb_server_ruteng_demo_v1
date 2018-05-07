#include "CFBaseObj.hpp"
#include "RPCommon.hpp"
#include "RPRouteCalcRequest.hpp"
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

void CRouteSession::calcRoute()
{
	auto spclWayPoints = std::make_shared<CRPWayPoints>();
	spclWayPoints->m_vclWayPointList.emplace_back(CPointCoord2D(1389087203, 476456031));
	spclWayPoints->m_vclWayPointList.emplace_back(CPointCoord2D(1389087203, 476456031));

	CRPRouteCalcRequest		req(spclWayPoints);
	
	CRPRouteCalcRequest		clSectionRequest(req);
	clSectionRequest.m_spclWayPoints->PushBack(spclWayPoints->At(0));
	clSectionRequest.m_spclWayPoints->PushBack(spclWayPoints->At(1));

	auto spclDPFacade = CDPFacade::Create();
	spclDPFacade->Initialize();

	auto spclSessionDirector = std::make_shared<CRPRCSectionDirector>();
	spclSessionDirector->Initialize(spclDPFacade);
	spclSessionDirector->StartCalculateSection(clSectionRequest);
}