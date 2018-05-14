// refer to RPRCRouteDirector.cpp
#include	"InnerCommon.hpp"
#include	"RPCommon.hpp"
#include	"RPRSLink.hpp"	// --> add by wxl
#include	"RPWayPoints.hpp"
//#include	"RPSection.h"
//#include	"RPRoute.h"
#include	"RPRouteCalcRequest.hpp"
//#include	"RPRCCmdBase.h"
//#include	"RPRCThreadPool.h"
#include	"RPRCRouteCalcCommon.hpp"
#include	"RPRCCost.hpp"
//#include	"RPRCExtIF.h"
//#include	"RPRCCmdCalcBase.h"
#include	"RPRCTermSearch.hpp"
#include	"RPRCUpSearch.hpp"
//#include	"RPRCCmdLinkLevelUp.h"
#include	"RPRCConnectSearch.hpp"
//#include	"RPRCCmdRouteEdit.h"
#include	"RPRCSectionDirector.hpp"

#include	"RouteSession.hpp"
#include	"DPFacade.hpp"

class CRouteSession::CPrivate
{
public:
	CPrivate() : m_lRouteID(0) {}
	static long NextRouteID() { return m_iNextRouteID++; }
	long														m_lRouteID;
	SmartPointer< CDPFacade >									m_spclDataProvider;
	SmartPointer< CRPRCLinkCostContainerSet >					m_spclLinkCostContainerSet; // refer to CRPRCMultiRouteDirector()
	static volatile long										m_iNextRouteID;
};

volatile long		CRouteSession::CPrivate::m_iNextRouteID = 0;

CRouteSession::CRouteSession()
{
}

CRouteSession::~CRouteSession()
{
}

bool CRouteSession::Initialize()
{
	if (!mp)
	{
		mp = make_shared<CPrivate>();
	}

	mp->m_spclDataProvider = CDPFacade::Create();
	if (mp->m_spclDataProvider == NULL)
	{
		ERR("");
		return false;
	}
	//	spclDPFacade->Initialize();
	if (SUCCESS != mp->m_spclDataProvider->Initialize())
	{
		ERR("");
		return false;
	}

	if (!mp->m_spclLinkCostContainerSet.Create()) {	// refer to CRPRCMultiRouteDirector::Initialize()
		ERR("");
		return false;
	}
	if (SUCCESS != mp->m_spclLinkCostContainerSet->Initialize(mp->m_spclDataProvider)) {
		ERR("");
		return false;
	}

	return true;
}

// reference pattern : 
//		CMNRP::SetDestinationByTwoCoord() and 
//		CRPRCRouteDirector::StartCalculateRoute()
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

	SmartPointer< CDPFacade > spclDPFacade = mp->m_spclDataProvider;
	spclDPFacade->Initialize();

	SmartPointer< CRPRCSectionDirector >	spclSectionDirector; // refer to CRPRCRouteDirector::Initialize()
	if (!spclSectionDirector.Create()) {
		ERR("");
		return false;
	}

	//m_spclRoute->m_uiSearchType = clRequest.m_uiSearchType;
	mp->m_lRouteID = CPrivate::NextRouteID();
	//m_spclRoute->m_iRouteID = m_lRouteID;

	// refer to CRPRCMultiRouteDirector::Initialize()
	spclSectionDirector->Initialize(spclDPFacade, mp->m_spclLinkCostContainerSet->m_aspclLinkCostContainer[0]);
	spclSectionDirector->StartCalculateSection(clRequest, mp->m_lRouteID);

	return false;
}