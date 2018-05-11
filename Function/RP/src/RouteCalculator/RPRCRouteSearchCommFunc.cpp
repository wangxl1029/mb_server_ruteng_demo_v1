#include	"InnerCommon.hpp"
#include	"RPCommon.hpp"
#include	"RPRSLink.hpp"
#include	"RPWayPoints.hpp"
//#include	"RPSection.h"
//#include	"RPRoute.h"
#include	"RPRouteCalcRequest.hpp"
#include	"RPRCRouteCalcCommon.hpp"
#include	"RPRCCost.hpp"
//#include	"RPRCExtIF.h"
#include	"RPRCRouteSearchCommFunc.hpp"


//	class CRPRCRouteSearchCommFunc
RESULT CRPRCRouteSearchCommFunc::GetNodeNoByLink(CRPRCLinkID &clLinkID,
	CRPRCRoutingTileProxy &clLinkRoutingTile,
	CRPTileID &clNodeTileID,
	CDPRoutingTileLinkNodeNo &clLinkNodeNo,
	CRPRCRoutingTileProxy &clNodeRoutingTile)
{
	if (SUCCESS != clLinkRoutingTile.GetData(clLinkID.m_clTileID)) {
		ERR("");
		return FAILURE;
	}

	CDPRoutingTileLinkInfo		&clLinkInfo = clLinkRoutingTile->m_clRoutingTileInfo.m_vclLinkList[clLinkID.m_usLinkNo];
	clLinkNodeNo = clLinkID.m_bPos ? clLinkInfo.m_clEndNode : clLinkInfo.m_clStartNode;
	if (clLinkNodeNo.m_usNodeNo != NDS_INVALID_NODE_ID) {
		clNodeTileID = clLinkID.m_clTileID;
		if (SUCCESS != clNodeRoutingTile.GetData(clNodeTileID)) {
			ERR("");
			return FAILURE;
		}
	}
	else {
		ushort	usLink2TileNo = clLinkRoutingTile->m_clRoutingTileInfo.m_vclLinkList[clLinkID.m_usLinkNo].m_usLink2TileNo;
		CLink2TileRef	&clLink2TileRef = clLinkRoutingTile->m_clRoutingTile.m_clLink2TileList.m_vclTileRef[usLink2TileNo];
		ushort	usExtTileIdx = clLink2TileRef.m_clExtTileColl.m_vusExtTileIdx.at(clLink2TileRef.m_clExtTileColl.m_vusExtTileIdx.size() - 1);
		clNodeTileID.m_uiPackedTileID = clLinkRoutingTile->m_clRoutingTile.m_clExternalTileIdList.m_viTileId[usExtTileIdx];
		clNodeTileID.m_sUpdateRegionID = clLinkID.m_clTileID.m_sUpdateRegionID;
		if (SUCCESS != clNodeRoutingTile.GetData(clNodeTileID)) {
			ERR("");
			return FAILURE;
		}
		CDPRoutingTileExtLinkID	clInExtLinkID(clLinkID.m_clTileID.m_uiPackedTileID, clLinkID.m_usLinkNo);
		map< CDPRoutingTileExtLinkID, CDPRoutingTileLinkNodeNo >::iterator	it = clNodeRoutingTile->m_clRoutingTileInfo.m_mapExtLink2Node.find(clInExtLinkID);
		if (it == clNodeRoutingTile->m_clRoutingTileInfo.m_mapExtLink2Node.end()) {
			ERR("");
			return FAILURE;
		}
		clLinkNodeNo = it->second;
	}
	return SUCCESS;
}

RESULT CRPRCRouteSearchCommFunc::GetConnectLinkID(const CRPTileID &clNodeTileID, size_t uiNodeNo, size_t uiConnectNo,
	CRPRCRoutingTileProxy &clNodeRoutingTile,
	CRPRCLinkID &clOutLinkID,
	CRPRCRoutingTileProxy &clOutLinkRoutingTile)
{
	if (SUCCESS != clNodeRoutingTile.GetData(clNodeTileID)) {
		ERR("");
		return FAILURE;
	}
	CSimpleIntersection	&clNode = clNodeRoutingTile->m_clRoutingTile.m_clSimpleIntersection.m_vclSimpleIntersection[uiNodeNo];

	CIntOrExtDirectedLinkReference		&clOutLink = clNode.m_vclConnectedLinks[uiConnectNo];
	if (clOutLink.m_ucIsExternalLinkReference) {
		CExternalDirectedLinkReference	&clExtLink = clOutLink.m_clLinkReferenceChoice.ExternalLinkReference_Ref();
		ushort	usExtTileIdx = clExtLink.m_usExtTileIdx;
		clOutLinkID.m_clTileID.m_uiPackedTileID = clNodeRoutingTile->m_clRoutingTile.m_clExternalTileIdList.m_viTileId[usExtTileIdx];
		clOutLinkID.m_clTileID.m_sUpdateRegionID = clNodeTileID.m_sUpdateRegionID;
		clOutLinkID.m_usLinkNo = clExtLink.m_clTileExternalLinkReference.m_usLinkId;
		clOutLinkID.m_bPos = clExtLink.m_clTileExternalLinkReference.m_ucPositiveLinkDirection;
		if (SUCCESS != clOutLinkRoutingTile.GetData(clOutLinkID.m_clTileID)) {
			ERR("");
			return FAILURE;
		}
	}
	else {
		CDirectedLinkReference			&clIntLink = clOutLink.m_clLinkReferenceChoice.InternalLinkReference_Ref();
		clOutLinkID.m_clTileID = clNodeTileID;
		clOutLinkID.m_usLinkNo = clIntLink.m_usLinkId;
		clOutLinkID.m_bPos = clIntLink.m_ucPositiveLinkDirection;
		clOutLinkRoutingTile = clNodeRoutingTile;
	}
	return SUCCESS;
}

RESULT CRPRCRouteSearchCommFunc::CalcArriveCost(RP_TERM enTerm, CRPRCLinkCostTableProxy &clLinkCostTableProxy, CRPRCCost &clCost,
	uint uiInLinkArriveCost,
	const CRPRCLinkID &clInLinkID, CRPRCRoutingTileProxy &clInLinkRoutingTile,
	const CRPTileID &clNodeTileID,
	ushort usNodeNo, ushort usInLinkConnNo, ushort usOutLinkConnNo,
	CRPRCRoutingTileProxy &clNodeRoutingTile,
	const CRPRCLinkID &clOutLinkID, CRPRCRoutingTileProxy &clOutLinkRoutingTile,
	uint &uiArriveCost, uint &uiLinkCost, uint &uiNodeCost)
{
	uiNodeCost = 0;
	if (clInLinkID.m_clTileID.m_sUpdateRegionID == clNodeTileID.m_sUpdateRegionID) {
		ushort	&usNodeCostRef = enTerm == RP_TERM_START ?
			clLinkCostTableProxy.GetNodeCostRef(clNodeTileID, usNodeNo, usInLinkConnNo, usOutLinkConnNo) :
			clLinkCostTableProxy.GetNodeCostRef(clNodeTileID, usNodeNo, usOutLinkConnNo, usInLinkConnNo);
		if (usNodeCostRef == 0) {
			usNodeCostRef = clCost.NodeCost();
		}
		uiNodeCost = clCost.ExtractShortCost(usNodeCostRef);
	}

	bool	bLinkCostPos = enTerm == RP_TERM_START ? clOutLinkID.m_bPos : !clOutLinkID.m_bPos;	//	Link的通行方向
	ushort	&usLinkCostRef = clLinkCostTableProxy.GetLinkCostRef(clOutLinkID.m_clTileID, clOutLinkID.m_usLinkNo, bLinkCostPos);
	if (usLinkCostRef == 0) {
		usLinkCostRef = clCost.LinkCost(CRPRCLinkID(clOutLinkID.m_clTileID, clOutLinkID.m_usLinkNo, bLinkCostPos), clOutLinkRoutingTile);
	}
	uiLinkCost = clCost.ExtractShortCost(usLinkCostRef);

	uiArriveCost = uiInLinkArriveCost + uiNodeCost + uiLinkCost;

	//	MESSAGE( "CalcArriveCost %s %d,%d,%s %d,%d %d,%d,%s, cost %d %d %d %d", enTerm==RP_TERM_START?"start":"end  ",
	//			 clInLinkID.m_clTileID.m_uiPackedTileID, clInLinkID.m_usLinkNo, clInLinkID.m_bPos?"pos":"neg",
	//			 clNodeTileID.m_uiPackedTileID, usNodeNo,
	//			 clOutLinkID.m_clTileID.m_uiPackedTileID, clOutLinkID.m_usLinkNo, clOutLinkID.m_bPos?"pos":"neg",
	//			 uiArriveCost, uiInLinkArriveCost, usNodeCost, usLinkCost );

	return SUCCESS;
}

void CRPRCRouteSearchCommFunc::UpdateMidLinkCost(CRPRCMidLink *pclLink,
	uint uiCostDiff,
	RPRC_OpenTable &mapOpenTable)
{
	pclLink->m_uiArriveCost -= uiCostDiff;
	CRPRCMidLink	*pclCurLink = pclLink->m_pclNextLink;
	for (ushort i = 0; i < pclLink->m_usNextLinkCnt; ++i) {
		UpdateMidLinkCost(pclCurLink, uiCostDiff, mapOpenTable);
		if (pclCurLink->m_itOpenLink != RPRC_OpenTableIterator_Null) {
			mapOpenTable.erase(pclCurLink->m_itOpenLink);
			RPRC_OpenTableIterator	itOpenLink = mapOpenTable.insert(RPRC_OpenTableValueType(pclCurLink->m_uiArriveCost, pclCurLink));
			pclCurLink->m_itOpenLink = itOpenLink;
		}
		pclCurLink = pclCurLink->m_pclNextSibling;
	}
}
