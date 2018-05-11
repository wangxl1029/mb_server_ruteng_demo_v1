#include	"InnerCommon.hpp"
#include	"RPCommon.hpp"
#include	"RPRSLink.hpp"
//#include	"RPWayPoints.h"
//#include	"RPSection.h"
//#include	"RPRoute.h"
#include	"RPRouteCalcRequest.hpp"
//#include	"RPRCCmdBase.h"
#include	"RPRCRouteCalcCommon.hpp"
#include	"RPRCCost.hpp"
//#include	"RPRCExtIF.h"
//#include	"RPRCCmdCalcBase.h"
//#include	"RPRCRouteSearchCommFunc.h"
#include	"RPRCUpSearch.h"


//	class CRPRCCmdUpSearchResult
CRPRCUpSearchResult::CRPRCUpSearchResult()
{
}

CRPRCUpSearchResult::~CRPRCUpSearchResult()
{
}


CRPRCUpSearch::CRPRCUpSearch()
{
}


CRPRCUpSearch::~CRPRCUpSearch()
{
}

RESULT CRPRCUpSearch::Execute()
{
	return UpSearch(m_enTerm, m_iFromLevel, m_iToLevel,
		m_spmapOpenTable, m_spclMidLinkTable, m_spclMidLinkUsingContainer,
		m_spclCost, m_spclLinkCostContainer, m_spclDataProvider,
		m_spclResult);
}

SmartPointer< CRPRCUpSearchResult > CRPRCUpSearch::GetResult()
{
	return m_spclResult;
}

RESULT CRPRCUpSearch::UpSearch(RP_TERM enTerm, int iFromLevel, int iToLevel,
	SmartPointer< RPRC_OpenTable > spmapOpenTable,
	SmartPointer< RPRC_MidLinkTable > spvclMidLinkTable,
	SmartPointer< RPRCTileContainer< CRPRCMidLinkUsingTile > > spclMidLinkUsingContainer,
	SmartPointer< CRPRCCost > spclCost,
	SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > > spclLinkCostContainer,
	SmartPointer< CDPFacade > spclDataProvider,
	SmartPointer< CRPRCUpSearchResult > &spclResult)
{
	//	Result Ref
	if (!spclResult.Create()) {
		ERR("");
		return FAILURE;
	}

	if (!spclResult->m_spvclResultLinkList.Create()) {
		ERR("");
		return FAILURE;
	}
	if (!spclResult->m_spvclConnectedLinkList.Create()) {
		ERR("");
		return FAILURE;
	}

	vector< CRPRCUpSearchResultLink >			&vclResultLinkList = *(spclResult->m_spvclResultLinkList);
	vector< CRPRCConnectSearchResultLink >	&vclConnectedLinkList = *(spclResult->m_spvclConnectedLinkList);

	//	Mid Data Proxy
	uint									uiCoordShift = 0;
	spclDataProvider->GetCoordShift(iFromLevel, BUILDING_BLOCK_ID_ROUTING, uiCoordShift);
	RPRC_OpenTable							&mapOpenTable = *spmapOpenTable;
	RPRC_MidLinkTable							&clMidLinkTable = *spvclMidLinkTable;
	CRPRCMidLinkUsingTableProxy				clMidLinkUsingTableProxy(spclMidLinkUsingContainer);
	CRPRCCost									&clCost = *spclCost;
	CRPRCLinkCostTableProxy					clLinkCostTableProxy(spclLinkCostContainer);
	CRPRCRoutingTileProxy						clInLinkRoutingTile(spclDataProvider);
	CRPRCRoutingTileProxy						clNodeRoutingTile(spclDataProvider);
	CRPRCRoutingTileProxy						clOutLinkRoutingTile(spclDataProvider);
	//SmartPointer< CRPRCExtIF >				spclExtIF = GetExtIF();
	size_t									uiExtendedLinkCount = 0;


	return SUCCESS;
}
