#include	"InnerCommon.hpp"
#include	"RPCommon.hpp"
#include	"RPRSLink.hpp"
#include	"RPWayPoints.hpp"
#include	"RPSection.hpp"
#include	"RPRouteCalcRequest.hpp"
//#include	"RPRCCmdBase.h"
//#include	"RPRCThreadPool.h"
#include	"RPRCRouteCalcCommon.hpp"
#include	"RPRCCost.hpp"
//#include	"RPRCExtIF.h"
//#include	"RPRCCmdCalcBase.h"
#include	"RPRCTermSearch.hpp"
#include	"RPRCUpSearch.hpp"
#include	"RPRCLinkLevelUp.hpp"
#include	"RPRCConnectSearch.hpp"
#include	"RPRCRouteEdit.hpp"
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
	m_spclMidLinkUsingContainer->SetCapacity(0xFFFFFFFF);	//	每次探路过程中不能释放，需要设为最大值，探路完成后会被释放

	//	Set Request
	if (SUCCESS != StepTermSearch()) {
		ERR("");
		return FAILURE;
	}

	RESULT stepCode = NextStep();
	while (RP_RETURN_CONTINUE == stepCode)
		stepCode = NextStep();

	return stepCode;
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

	m_vspclStartUpSearchList.resize(m_vspclStartUpSearchList.size() + 1);
	m_vspclEndUpSearchList.resize(m_vspclEndUpSearchList.size() + 1);

	if (!m_vspclStartUpSearchList.back().Create(/*this, RPRC_Cmd_PRI_High,*/ m_lRouteID, /*m_spclExtIF, */RP_TERM_START,
		iFromLevel, iToLevel,
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

	if (SUCCESS != m_vspclStartUpSearchList.back()->Execute())
	{
		ERR("");
		return FAILURE;
	}

	if (!m_vspclEndUpSearchList.back().Create(/*this, RPRC_Cmd_PRI_High, */m_lRouteID, /*m_spclExtIF, */RP_TERM_END,
		iFromLevel, iToLevel,
		spmapEndTermOpenTable,
		spclEndTermMidLinkTable,
		m_spclMidLinkUsingContainer,
		m_spclCost,
		m_spclLinkCostContainer,
		m_spclDataProvider)) {
		ERR("");
		return FAILURE;
	}

	if (SUCCESS != m_vspclEndUpSearchList.back()->Execute())
	{
		ERR("");
		return FAILURE;
	}

	return SUCCESS;
}

RESULT CRPRCSectionDirector::StepLinkLevelUp(int iFromLevel, int iToLevel,
	SmartPointer< vector< CRPRCUpSearchResultLink > > spvclStartInputLinkList,
	SmartPointer< vector< CRPRCUpSearchResultLink > > spvclEndInputLinkList)
{
	m_enStep = STEP_LINK_LEVEL_UP;

	m_vspclStartLinkLevelUpList.resize(m_vspclStartLinkLevelUpList.size() + 1);
	m_vspclEndLinkLevelUpList.resize(m_vspclEndLinkLevelUpList.size() + 1);

	if (!m_vspclStartLinkLevelUpList.back().Create(/*this, RPRC_Cmd_PRI_High, */m_lRouteID, /*m_spclExtIF, */RP_TERM_START,
		iFromLevel, iToLevel,
		spvclStartInputLinkList,
		m_spclMidLinkUsingContainer,
		m_spclCost,
		m_spclLinkCostContainer,
		m_spclDataProvider))
	{
		ERR("");
		return FAILURE;
	}

	if (SUCCESS != m_vspclStartLinkLevelUpList.back()->Execute())
	{
		ERR("");
		return FAILURE;
	}

	if (!m_vspclEndLinkLevelUpList.back().Create(/*this, RPRC_Cmd_PRI_High, */m_lRouteID, /*m_spclExtIF, */RP_TERM_END,
		iFromLevel, iToLevel,
		spvclEndInputLinkList,
		m_spclMidLinkUsingContainer,
		m_spclCost,
		m_spclLinkCostContainer,
		m_spclDataProvider)) {
		ERR("");
		return FAILURE;
	}

	if (SUCCESS != m_vspclEndLinkLevelUpList.back()->Execute())
	{
		ERR("");
		return FAILURE;
	}

	return SUCCESS;
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

	vector< SmartPointer< RPRC_MidLinkTable > >	vspclStartTermMidLinkTable;
	vspclStartTermMidLinkTable.push_back(m_spclStartTermSearchResult->m_spclMidLinkTable);
	for (size_t i = 0; i < m_vspclStartLinkLevelUpResult.size(); ++i) {
		vspclStartTermMidLinkTable.push_back(m_vspclStartLinkLevelUpResult[i]->m_spclMidLinkTable);
	}

	vector< SmartPointer< RPRC_MidLinkTable > >	vspclEndTermMidLinkTable;
	vspclEndTermMidLinkTable.push_back(m_spclEndTermSearchResult->m_spclMidLinkTable);
	for (size_t i = 0; i < m_vspclEndLinkLevelUpResult.size(); ++i) {
		vspclEndTermMidLinkTable.push_back(m_vspclEndLinkLevelUpResult[i]->m_spclMidLinkTable);
	}

	if (!m_spclRouteEdit.Create(/*this, RPRC_Cmd_PRI_High, */m_lRouteID, /*m_spclExtIF,*/
		m_clStartWayPoint, m_clEndWayPoint, iConnectLevel, m_viLevelList,
		m_spclStartTermSearchResult->m_spvclResultLinkList,
		m_spclEndTermSearchResult->m_spvclResultLinkList,
		vspclStartTermMidLinkTable,
		vspclEndTermMidLinkTable,
		m_spclMidLinkUsingContainer,
		spvclStartTermConnectResultLinkList,
		spvclEndTermConnectResultLinkList,
		m_spclDataProvider))
	{
		ERR("");
		return FAILURE;
	}

	return m_spclRouteEdit->Execute();
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
				ERR("step up search failed!");
				return FAILURE;
			}
		}
		else {
			ERR( "" );
			return FAILURE;
		}
	}
	else if (STEP_UP_SEARCH == m_enStep) {
		m_vspclStartUpSearchResult.push_back(m_vspclStartUpSearchList.back()->GetResult());
		m_vspclEndUpSearchResult.push_back(m_vspclEndUpSearchList.back()->GetResult());
		if ((m_vspclStartUpSearchResult.back()->m_spvclConnectedLinkList != NULL && m_vspclStartUpSearchResult.back()->m_spvclConnectedLinkList->size() > 0)
			|| (m_vspclEndUpSearchResult.back()->m_spvclConnectedLinkList != NULL && m_vspclEndUpSearchResult.back()->m_spvclConnectedLinkList->size() > 0))
		{
			if (SUCCESS != StepRouteEdit(m_viLevelList[m_vspclStartLinkLevelUpList.size()],
				m_vspclStartUpSearchResult.back()->m_spvclConnectedLinkList,
				m_vspclEndUpSearchResult.back()->m_spvclConnectedLinkList))
			{
				ERR("");
				return FAILURE;
			}
		}
		else if (m_viLevelList[m_vspclStartLinkLevelUpList.size()] <= DecideConnectLevel(m_clStartWayPoint,
			m_clEndWayPoint))
		{
			if (SUCCESS != StepConnectSearch(m_viLevelList[m_vspclStartLinkLevelUpList.size()],
				m_vspclStartLinkLevelUpResult.back()->m_spmapOpenTable,
				m_vspclStartLinkLevelUpResult.back()->m_spclMidLinkTable,
				m_vspclEndLinkLevelUpResult.back()->m_spmapOpenTable,
				m_vspclEndLinkLevelUpResult.back()->m_spclMidLinkTable))
			{
				ERR("");
				return FAILURE;
			}
		}
		else if (m_vspclStartUpSearchResult.back()->m_spvclResultLinkList->size() > 0 && m_vspclEndUpSearchResult.back()->m_spvclResultLinkList->size() > 0) {
			if (SUCCESS != StepLinkLevelUp(m_viLevelList[m_vspclStartLinkLevelUpList.size()],
				m_viLevelList[m_vspclStartLinkLevelUpList.size() + 1],
				m_vspclStartUpSearchResult.back()->m_spvclResultLinkList,
				m_vspclEndUpSearchResult.back()->m_spvclResultLinkList))
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
	else if (STEP_LINK_LEVEL_UP == m_enStep) {
		m_vspclStartLinkLevelUpResult.push_back(m_vspclStartLinkLevelUpList.back()->GetResult());
		m_vspclEndLinkLevelUpResult.push_back(m_vspclEndLinkLevelUpList.back()->GetResult());

		if (m_vspclStartLinkLevelUpResult.back()->m_spmapOpenTable->size() > 0 && m_vspclEndLinkLevelUpResult.back()->m_spmapOpenTable->size() > 0) {
			if (SUCCESS != StepUpSearch(m_viLevelList[m_vspclStartUpSearchList.size()],
				m_viLevelList[m_vspclEndUpSearchList.size() + 1],
				m_vspclStartLinkLevelUpResult.back()->m_spmapOpenTable,
				m_vspclStartLinkLevelUpResult.back()->m_spclMidLinkTable,
				m_vspclEndLinkLevelUpResult.back()->m_spmapOpenTable,
				m_vspclEndLinkLevelUpResult.back()->m_spclMidLinkTable))
			{
				ERR("");
				return FAILURE;
			}
		}
		else {
			//ERR( "" );
			return FAILURE;
		}
	} else if (STEP_CONNECT_SEARCH == m_enStep) {
		m_spclStartConnectSearchResult = m_spclStartConnectSearch->GetResult();
		m_spclEndConnectSearchResult = m_spclEndConnectSearch->GetResult();
		if ((m_spclStartConnectSearchResult->m_spvclConnectedLinkList != NULL && m_spclStartConnectSearchResult->m_spvclConnectedLinkList->size() > 0)
			|| (m_spclEndConnectSearchResult->m_spvclConnectedLinkList != NULL && m_spclEndConnectSearchResult->m_spvclConnectedLinkList->size() > 0))
		{
			//MESSAGE( "CRPRCSectionDirector:: STEP_CONNECT_SEARCH == m_enStep" );
			if (SUCCESS != StepRouteEdit(m_viLevelList[m_vspclStartLinkLevelUpList.size()],
				m_spclStartConnectSearchResult->m_spvclConnectedLinkList,
				m_spclEndConnectSearchResult->m_spvclConnectedLinkList)) {
				ERR("");
				return FAILURE;
			}
		}
		else {
			ERR( "" );
			return FAILURE;
		}
	}
	else if (STEP_ROUTE_EDIT == m_enStep) {
		m_spclRouteEditResult = m_spclRouteEdit->GetResult();
		m_spclSection = m_spclRouteEditResult->m_spclSection;
		return SUCCESS;
	}
	else {
		ERR("");
		return FAILURE;
	}

	return RP_RETURN_CONTINUE;
}
