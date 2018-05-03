#include "RouteSession.hpp"
#include "RPCommon.hpp"
#include "RPRouteCalcRequest.hpp"
#include "RPRCSectionDirector.hpp"


CBaseObj::CBaseObj()
{}
CBaseObj::~CBaseObj()
{}

CRouteSession::CRouteSession()
{
}

CRouteSession::~CRouteSession()
{
}

void CRouteSession::calcRoute()
{
	CRPRouteCalcRequest		req(std::make_shared<CRPWayPoints>());
	
	CRPRouteCalcRequest		clSectionRequest(req);

	auto spclSessionDirector = std::make_shared<CRPRCSectionDirector>();
	spclSessionDirector->StartCalculateSection();
}