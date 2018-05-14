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

// plz refer to  CRPRCMultiRouteDirector::Initialize()
RESULT CRPRCSectionDirector::Initialize(	SmartPointer< CDPFacade > spclDataProvider,
											SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > > spclLinkCostContainer)

{
	if (spclDataProvider == NULL) {
		ERR("");
		return FAILURE;
	}
#if 0
	if (spclThreadPool == NULL) {
		ERR("");
		return FAILURE;
	}
#endif
	m_spclDataProvider = spclDataProvider;
//	m_spclThreadPool = spclThreadPool;
//	m_spclCost = spclCost;
	m_spclLinkCostContainer = spclLinkCostContainer;
//	m_spclExtIF = spclExtIF;

#if 0
	//
	if (SUCCESS != spclDataProvider->GetLevelList(BUILDING_BLOCK_ID_ROUTING, m_viLevelList)) {
		ERR("");
		return SUCCESS;
	}
	reverse(m_viLevelList.begin(), m_viLevelList.end());

	//
	SmartPointer< boost::signals2::signal< void(SmartPointer< CRPRCCmdBase >) > >	spclSignal = spclThreadPool->CreateSignal(this);
	m_clThreadPoolConnection = spclSignal->connect(boost::bind(&CRPRCSectionDirector::CommandEnd, this, _1));
#endif
	return SUCCESS;
}

RESULT CRPRCSectionDirector::StartCalculateSection(CRPRouteCalcRequest &clRequest)
{
	CCFMutexLocker	clLock(m_clMutex);

	//MESSAGE( "CRPRCSectionDirector::StartCalculateSection" );

	m_bCancelFlag = false;

	//MESSAGE( "Section Start" );

	//	Save Request
	m_clRequest = clRequest;
//	m_clParam = clParam;
//	m_lRouteID = lRouteID;

	if (clRequest.m_spclWayPoints->Size() < 2)
	{
		ERR("");
		return FAILURE;
	}

	m_clStartWayPoint = clRequest.m_spclWayPoints->At(0);
	m_clEndWayPoint = clRequest.m_spclWayPoints->At(1);

	//	Mid Data
	m_spclMidLinkUsingContainer.Release();
	if (!m_spclMidLinkUsingContainer.Create(m_spclDataProvider)) {
		ERR("");
		return FAILURE;
	}
	m_spclMidLinkUsingContainer->SetCapacity(0xFFFFFFFF);	//	每次探路过程中不能释放，需要设为最大值，探路完成后会被释放

	// refer to CRPRCSectionDirector::StepTermSearch()
	if (!m_spclStartTermSearch.Create(RP_TERM_START, m_clStartWayPoint, 13, 
		m_spclMidLinkUsingContainer, m_spclCost, m_spclLinkCostContainer, m_spclDataProvider))
	{
		ERR("");
		return FAILURE;
	}

	if (SUCCESS != m_spclStartTermSearch->Execute())
	{
		ERR("");
		return FAILURE;
	}

	// refer to CRPRCSectionDirector::StepTermSearch()
	if (!m_spclEndTermSearch.Create(RP_TERM_END, m_clEndWayPoint, 13,
		m_spclMidLinkUsingContainer, m_spclCost, m_spclLinkCostContainer, m_spclDataProvider))
	{
		ERR("");
		return FAILURE;
	}

	if (SUCCESS != m_spclEndTermSearch->Execute())
	{
		ERR("");
		return FAILURE;
	}

	return SUCCESS;
}
