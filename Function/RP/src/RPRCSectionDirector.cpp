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
#include	"RPRCUpSearch.hpp"
//#include	"RPRCLinkLevelUp.hpp"
#include	"RPRCConnectSearch.hpp"
//#include	"RPRCCmdRouteEdit.h"
#include	"RPRCSectionDirector.hpp"

struct RPDist2Level
{
	double														m_dMinDist;
	int															m_iLevel;
};

static RPDist2Level		s_astDist2Level[] = {
	{ -1, 13 },
	{ 1000, 11 },
	{ 10000, 9 },
	{ 100000, 7 },
	{ 1000000, 5 },
};

CRPRCSectionDirector::CRPRCSectionDirector() : m_bCancelFlag(false), m_enStep(STEP_INVALID)
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

	//
	if (SUCCESS != spclDataProvider->GetLevelList(BUILDING_BLOCK_ID_ROUTING, m_viLevelList)) {
		ERR("");
		//return SUCCESS; // replace with failure --> supposed to be a bug
		return FAILURE;
	}
	reverse(m_viLevelList.begin(), m_viLevelList.end());

#if 0 // del by wxl from znavi --> No boost
	//
	SmartPointer< boost::signals2::signal< void(SmartPointer< CRPRCCmdBase >) > >	spclSignal = spclThreadPool->CreateSignal(this);
	m_clThreadPoolConnection = spclSignal->connect(boost::bind(&CRPRCSectionDirector::CommandEnd, this, _1));
#endif
	return SUCCESS;
}

RESULT CRPRCSectionDirector::StartCalculateSection(CRPRouteCalcRequest &clRequest, long lRouteID)
{
	CCFMutexLocker	clLock(m_clMutex);

	//MESSAGE( "CRPRCSectionDirector::StartCalculateSection" );

	m_bCancelFlag = false;

	//MESSAGE( "Section Start" );

	//	Save Request
	m_clRequest = clRequest;
//	m_clParam = clParam;
	m_lRouteID = lRouteID;

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
	m_spclMidLinkUsingContainer->SetCapacity(0xFFFFFFFF);	//	ÿ��̽·�����в����ͷţ���Ҫ��Ϊ���ֵ��̽·��ɺ�ᱻ�ͷ�

	//	Set Request
	if (SUCCESS != StepTermSearch()) {
		ERR("");
		return FAILURE;
	}

	while (RP_RETURN_CONTINUE == NextStep())
	{

	}

	return SUCCESS;
}

// refer to CRPRCSectionDirector::StepTermSearch() from znavi
RESULT CRPRCSectionDirector::StepTermSearch()
{
	m_enStep = STEP_TERM_SEARCH;

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

RESULT CRPRCSectionDirector::StepUpSearch(int iFromLevel, int iToLevel,
	SmartPointer< RPRC_OpenTable > spmapStartTermOpenTable,
	SmartPointer< RPRC_MidLinkTable > spclStartTermMidLinkTable,
	SmartPointer< RPRC_OpenTable > spmapEndTermOpenTable,
	SmartPointer< RPRC_MidLinkTable > spclEndTermMidLinkTable)
{
	m_enStep = STEP_UP_SEARCH;

	//return SUCCESS;
	return FAILURE;
}

RESULT CRPRCSectionDirector::StepLinkLevelUp(int iFromLevel, int iToLevel,
	SmartPointer< vector< CRPRCUpSearchResultLink > > spvclStartInputLinkList,
	SmartPointer< vector< CRPRCUpSearchResultLink > > spvclEndInputLinkList)
{
	m_enStep = STEP_LINK_LEVEL_UP;

	//return SUCCESS;
	return FAILURE;
}

RESULT CRPRCSectionDirector::StepConnectSearch(int iLevel,
	SmartPointer< RPRC_OpenTable > spmapStartTermOpenTable,
	SmartPointer< RPRC_MidLinkTable > spclStartTermMidLinkTable,
	SmartPointer< RPRC_OpenTable > spmapEndTermOpenTable,
	SmartPointer< RPRC_MidLinkTable > spclEndTermMidLinkTable)
{
	m_enStep = STEP_CONNECT_SEARCH;
	SmartPointer< CRPRCConnectSearchResult > spclResult;
	//	Result Ref
	if (!spclResult.Create()) {
		ERR("");
		return FAILURE;
	}

	if (!spclResult->m_spvclConnectedLinkList.Create()) {
		ERR("");
		return FAILURE;
	}


	if (!m_spclStartConnectSearch.Create(/*this, RPRC_Cmd_PRI_High, */m_lRouteID, /*m_spclExtIF, */RP_TERM_START,
		iLevel, m_clStartWayPoint, m_clEndWayPoint,
		spmapStartTermOpenTable,
		spclStartTermMidLinkTable,
		m_spclMidLinkUsingContainer,
		m_spclCost,
		m_spclLinkCostContainer,
		m_spclDataProvider))
	{
		ERR("");
		return FAILURE;
	}
	m_spclStartConnectSearch->m_spclResult = spclResult;

	if (!m_spclEndConnectSearch.Create(/*this, RPRC_Cmd_PRI_High, */m_lRouteID, /*m_spclExtIF,*/ RP_TERM_END,
		iLevel, m_clStartWayPoint, m_clEndWayPoint,
		spmapEndTermOpenTable,
		spclEndTermMidLinkTable,
		m_spclMidLinkUsingContainer,
		m_spclCost,
		m_spclLinkCostContainer,
		m_spclDataProvider)) {
		ERR("");
		return FAILURE;
	}
	m_spclEndConnectSearch->m_spclResult = spclResult;

	RESULT srchResult = RP_RETURN_CONTINUE;
	bool isForward = true;
	while (RP_RETURN_CONTINUE == srchResult)
	{
		srchResult = isForward ? m_spclStartConnectSearch->Execute() : m_spclEndConnectSearch->Execute();
		if (RP_RETURN_CONTINUE == srchResult)
		{
			isForward = !isForward;
		}
		else if(SUCCESS != srchResult)
		{
			ERR("");
			return FAILURE;
		}
	}

	return SUCCESS;
}

RESULT CRPRCSectionDirector::StepRouteEdit(int iConnectLevel,
	SmartPointer< vector< CRPRCConnectSearchResultLink > > spvclStartTermConnectResultLinkList,
	SmartPointer< vector< CRPRCConnectSearchResultLink > > spvclEndTermConnectResultLinkList)
{
	m_enStep = STEP_ROUTE_EDIT;

	//return SUCCESS;
	return FAILURE;
}

int CRPRCSectionDirector::DecideConnectLevel(CRPWayPoint &clStartWayPoint,
	CRPWayPoint &clEndWayPoint)
{
	double	dx = clStartWayPoint.m_clCoord.x - clEndWayPoint.m_clCoord.x;
	double	dy = clStartWayPoint.m_clCoord.y - clEndWayPoint.m_clCoord.y;

	double	dist = sqrt(dx * dx + dy * dy);

	size_t	uiNo = 0;
	for (size_t i = 1; i < sizeof(s_astDist2Level) / sizeof(s_astDist2Level[0]); ++i) {
		if (s_astDist2Level[i].m_dMinDist > dist) {
			uiNo = i - 1;
			break;
		}
	}
	return s_astDist2Level[uiNo].m_iLevel;
}

RESULT CRPRCSectionDirector::NextStep()
{
	if (STEP_TERM_SEARCH == m_enStep) {
		m_spclStartTermSearchResult = m_spclStartTermSearch->GetResult();
		m_spclEndTermSearchResult = m_spclEndTermSearch->GetResult();
		if ((m_spclStartTermSearchResult->m_spvclConnectedLinkList != NULL && m_spclStartTermSearchResult->m_spvclConnectedLinkList->size() > 0)
			|| (m_spclEndTermSearchResult->m_spvclConnectedLinkList != NULL && m_spclEndTermSearchResult->m_spvclConnectedLinkList->size() > 0))
		{
			if (SUCCESS != StepRouteEdit(m_viLevelList[0],
				m_spclStartTermSearchResult->m_spvclConnectedLinkList,
				m_spclEndTermSearchResult->m_spvclConnectedLinkList))
			{
				ERR("");
				return FAILURE;
			}
		}
		else if (m_viLevelList[0] <= DecideConnectLevel(m_clStartWayPoint, m_clEndWayPoint)) {
			if (SUCCESS != StepConnectSearch(m_viLevelList[0],
				m_spclStartTermSearchResult->m_spmapOpenTable,
				m_spclStartTermSearchResult->m_spclMidLinkTable,
				m_spclEndTermSearchResult->m_spmapOpenTable,
				m_spclEndTermSearchResult->m_spclMidLinkTable))
			{
				ERR("");
				return FAILURE;
			}
		}
		else if (m_spclStartTermSearchResult->m_spmapOpenTable->size() > 0 && m_spclEndTermSearchResult->m_spmapOpenTable->size() > 0) {
			if (SUCCESS != StepUpSearch(m_viLevelList[0], m_viLevelList[1],
				m_spclStartTermSearchResult->m_spmapOpenTable,
				m_spclStartTermSearchResult->m_spclMidLinkTable,
				m_spclEndTermSearchResult->m_spmapOpenTable,
				m_spclEndTermSearchResult->m_spclMidLinkTable))
			{
				ERR("");
				return FAILURE;
			}
		}
		else {
			//ERR( "" );
			return FAILURE;
		}
	}
	else if (STEP_UP_SEARCH == m_enStep) {
	}
	else if (STEP_LINK_LEVEL_UP == m_enStep) {
	}
	else if (STEP_CONNECT_SEARCH == m_enStep) {
		m_spclStartConnectSearchResult = m_spclStartConnectSearch->GetResult();
		m_spclEndConnectSearchResult = m_spclEndConnectSearch->GetResult();
		if ((m_spclStartConnectSearchResult->m_spvclConnectedLinkList != NULL && m_spclStartConnectSearchResult->m_spvclConnectedLinkList->size() > 0)
			|| (m_spclEndConnectSearchResult->m_spvclConnectedLinkList != NULL && m_spclEndConnectSearchResult->m_spvclConnectedLinkList->size() > 0))
		{
			//MESSAGE( "CRPRCSectionDirector:: STEP_CONNECT_SEARCH == m_enStep" );
			#if 0
			if (SUCCESS != StepRouteEdit(m_viLevelList[m_vspclStartLinkLevelUpList.size()],
				m_spclStartConnectSearchResult->m_spvclConnectedLinkList,
				m_spclEndConnectSearchResult->m_spvclConnectedLinkList)) {
				ERR("");
				return FAILURE;
			}
			#endif
		}
		else {
			//ERR( "" );
			return FAILURE;
		}
	}
	else if (STEP_ROUTE_EDIT == m_enStep) {
	}
	else {
		ERR("");
		return FAILURE;
	}

	return RP_RETURN_CONTINUE;
}
