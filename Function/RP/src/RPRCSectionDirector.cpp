#include	"InnerCommon.hpp"
#include	"RPCommon.hpp"
#include	"RPRSLink.hpp"
#include	"RPWayPoints.hpp"
//#include	"RPSection.h"
#include	"RPRouteCalcRequest.hpp"
//#include	"RPRCCmdBase.h"
//#include	"RPRCThreadPool.h"
#include	"RPRCRouteCalcCommon.hpp"
#include	"RPRCCost.hpp"
//#include	"RPRCExtIF.h"
//#include	"RPRCCmdCalcBase.h"
#include	"RPRCTermSearch.hpp"
//#include	"RPRCCmdUpSearch.h"
//#include	"RPRCCmdLinkLevelUp.h"
//#include	"RPRCCmdConnectSearch.h"
//#include	"RPRCCmdRouteEdit.h"
#include	"RPRCSectionDirector.hpp"

CRPRCSectionDirector::CRPRCSectionDirector()
{
}


CRPRCSectionDirector::~CRPRCSectionDirector()
{
}

RESULT CRPRCSectionDirector::Initialize(SmartPointer< CDPFacade > spclDataProvider)
{
	if (spclDataProvider == NULL) {
		ERR("");
		return FAILURE;
	}
	m_spclDataProvider = spclDataProvider;
	return SUCCESS;
}

RESULT CRPRCSectionDirector::StartCalculateSection(CRPRouteCalcRequest &clRequest)
{
	if (clRequest.m_spclWayPoints->Size() < 2)
	{
		ERR("");
		return FAILURE;
	}

	m_clStartWayPoint = clRequest.m_spclWayPoints->At(0);
	m_clEndWayPoint = clRequest.m_spclWayPoints->At(1);

	// refer to CRPRCSectionDirector::StepTermSearch()
	if (!m_spclStartTermSearch.Create(RP_TERM_START, m_clStartWayPoint, 13, 
		m_spclCost, m_spclLinkCostContainer, m_spclDataProvider))
	{
		ERR("");
		return FAILURE;
	}

	m_spclStartTermSearch->Execute(); 

	// refer to CRPRCSectionDirector::StepTermSearch()
	if (!m_spclEndTermSearch.Create(RP_TERM_END, m_clEndWayPoint, 13,
		m_spclCost, m_spclLinkCostContainer, m_spclDataProvider))
	{
		ERR("");
		return FAILURE;
	}

	m_spclEndTermSearch->Execute();

	return SUCCESS;
}
