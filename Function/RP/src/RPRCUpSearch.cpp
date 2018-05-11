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
#include	"RPRCRouteSearchCommFunc.hpp"
#include	"RPRCUpSearch.hpp"


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

	CCheckEndParam	clCheckEndParam(enTerm, iFromLevel, iToLevel,
		mapOpenTable,
		uiExtendedLinkCount,
		vclResultLinkList,
		vclConnectedLinkList);

	CExtendParam	clExtendParam(enTerm, iFromLevel, iToLevel, uiCoordShift,
		mapOpenTable,
		clMidLinkTable,
		clMidLinkUsingTableProxy,
		clInLinkRoutingTile,
		clNodeRoutingTile,
		clOutLinkRoutingTile,
		clCost,
		clLinkCostTableProxy,
		spclDataProvider,
		vclResultLinkList,
		vclConnectedLinkList
		/*, spclExtIF*/);

	while (!CheckEnd(clCheckEndParam)) {
		if (SUCCESS != Extend(clExtendParam))
		{
			ERR("");
			return FAILURE;
		}
		++uiExtendedLinkCount;
	}

	return SUCCESS;
}

bool CRPRCUpSearch::CheckEnd(CRPRCUpSearch::CCheckEndParam &p)
{
	if (p.m_mapOpenTable.size() == 0) {
		//MESSAGE( "p.m_mapOpenTable.size() == 0" );
		return true;
	}
	if (p.m_vclConnectedLinkList.size() > 5) {
		//MESSAGE( "p.m_vclConnectedLinkList.size() %d", p.m_vclConnectedLinkList.size() );
		return true;
	}
	if (p.m_vclResultLinkList.size() > 50) {
		//MESSAGE( "p.m_vclResultLinkList.size() %d", p.m_vclResultLinkList.size() );
		return true;
	}
	return false;
}

RESULT CRPRCUpSearch::Extend(CRPRCUpSearch::CExtendParam &p)
{
	RPRC_OpenTableIterator	it = p.m_mapOpenTable.begin();
	uint					uiInLinkArriveCost = it->first;
	CRPRCMidLink				*pclInLink = it->second;
	p.m_mapOpenTable.erase(it);

	CRPRCLinkID				&clInLinkID = pclInLink->m_clLinkID;

	//	link to node
	if (SUCCESS != p.m_clInLinkRoutingTile.GetData(clInLinkID.m_clTileID)) {
		ERR("");
		return FAILURE;
	}

	CRPTileID							clNodeTileID;
	CDPRoutingTileLinkNodeNo			clNodeNo;

	vector< CDPGatewayInfo >			vclGatewayList;
	uint								uiGatewayID = NDS_INVALID_GATEWAY_ID;
	short								sCurUR = clInLinkID.m_clTileID.m_sUpdateRegionID;

	do {
		if (vclGatewayList.size() == 0) {
			if (SUCCESS != CRPRCRouteSearchCommFunc::GetNodeNoByLink(clInLinkID, p.m_clInLinkRoutingTile,
				clNodeTileID, clNodeNo, p.m_clNodeRoutingTile)) {
				ERR("");
				return FAILURE;
			}
			uiGatewayID = p.m_clNodeRoutingTile->m_clRoutingTileInfo.m_vclNodeList[clNodeNo.m_usNodeNo].m_uiGatewayID;
			if (uiGatewayID != NDS_INVALID_GATEWAY_ID) {
				if (SUCCESS != p.m_spclDataProvider->GetGatewayByID(uiGatewayID, vclGatewayList)) {
					ERR("");
					return FAILURE;
				}
			}
		}
		else {
			CDPGatewayInfo	&clGateway = vclGatewayList.back();
			clNodeTileID.m_uiPackedTileID = clGateway.m_clGatewayInfo.m_iTileId;
			clNodeTileID.m_sUpdateRegionID = clGateway.m_clGatewayInfo.m_iUpdateRegionId;
			vclGatewayList.pop_back();

			if (clNodeTileID.m_sUpdateRegionID == sCurUR) {
				continue;
			}
			uint	uiTileNo = 0;
			int		iTileLevel = 0;
			CDPCommon::PackedTileIDToTileNo(clNodeTileID.m_uiPackedTileID, iTileLevel, uiTileNo);
			if (iTileLevel != p.m_iFromLevel) {
				continue;
			}

			if (SUCCESS != p.m_clNodeRoutingTile.GetData(clNodeTileID)) {
				ERR("");
				return FAILURE;
			}
			clNodeNo.m_usNodeNo = p.m_clNodeRoutingTile->m_clRoutingTileInfo.m_mapGatewayID2NodeNo[uiGatewayID];
		}

		//	loop node connected links
		CSimpleIntersection	&clNode = p.m_clNodeRoutingTile->m_clRoutingTile.m_clSimpleIntersection.m_vclSimpleIntersection[clNodeNo.m_usNodeNo];

		//	for extend
		//		pclInLink->m_clToPoint.x		=	p.m_clNodeRoutingTile->m_clBasePoint.x + ( clNode.m_clPosition.m_iDx << p.m_uiCoordShift );
		//		pclInLink->m_clToPoint.y		=	p.m_clNodeRoutingTile->m_clBasePoint.y + ( clNode.m_clPosition.m_iDy << p.m_uiCoordShift );
		//		if( pclInLink->m_clFromPoint.x != 0 ) {
		//			vector< CPointCoord3D >	vclPointList;
		//			vclPointList.push_back( CPointCoord3D( pclInLink->m_clFromPoint.x, pclInLink->m_clFromPoint.y, 0 ) );
		//			vclPointList.push_back( CPointCoord3D( pclInLink->m_clToPoint.x, pclInLink->m_clToPoint.y, 0 ) );
		//			p.m_spclExtIF->RPExtendLink( vclPointList, p.m_enTerm, p.m_clNodeRoutingTile->m_iLevel );
		//		}


		for (size_t i = 0; i < clNode.m_vclConnectedLinks.size(); ++i) {
#if 0 // del by wxl --> no cancel available
			if (GetCancelFlag()) {
				return SUCCESS;
			}
#endif
			CRPRCLinkID							clOutLinkID;

			if (SUCCESS != CRPRCRouteSearchCommFunc::GetConnectLinkID(clNodeTileID, clNodeNo.m_usNodeNo, i,
				p.m_clNodeRoutingTile, clOutLinkID, p.m_clOutLinkRoutingTile))
			{
				ERR("");
				return FAILURE;
			}

			uint	uiArriveCost = 0;
			uint	uiLinkCost = 0;
			uint	uiNodeCost = 0;
			if (SUCCESS != CRPRCRouteSearchCommFunc::CalcArriveCost(p.m_enTerm, p.m_clLinkCostTableProxy, p.m_clCost,
				uiInLinkArriveCost,
				clInLinkID, p.m_clInLinkRoutingTile,
				clNodeTileID,
				clNodeNo.m_usNodeNo, clNodeNo.m_usConnNo, i,
				p.m_clNodeRoutingTile,
				clOutLinkID, p.m_clOutLinkRoutingTile,
				uiArriveCost, uiLinkCost, uiNodeCost))
			{
				ERR("");
				return FAILURE;
			}

			//			MESSAGE( "%s %d %d %s, %d, %d %d %s , %d %d", p.m_enTerm==0?"start":"end  ", clInLinkID.m_clTileID.m_uiPackedTileID, clInLinkID.m_usLinkNo, clInLinkID.m_bPos?"pos":"neg",
			//					 clNodeNo.m_usNodeNo, clOutLinkID.m_clTileID.m_uiPackedTileID, clOutLinkID.m_usLinkNo, clOutLinkID.m_bPos?"pos":"neg", uiArriveCost, uiLinkCost );

			ushort			usUpLinkRefNo = p.m_clOutLinkRoutingTile->m_clRoutingTileInfo.m_vclLinkList[clOutLinkID.m_usLinkNo].m_ausUpLinkRefNo[LINK_DIR_NO(clOutLinkID.m_bPos)];
			bool			bHaveUpLink = usUpLinkRefNo < NDS_INVALID_NODE_ID;

			CRPRCMidLinkUsing	*pclOutLinkUsing = p.m_clMidLinkUsingTableProxy.GetMidLinkUsing(clOutLinkID);
			CRPRCMidLink		*pclOutLink = pclOutLinkUsing->m_apclMidLink[p.m_enTerm][LINK_DIR_NO(clOutLinkID.m_bPos)];
			RP_TERM			enAnotherTerm = RP_AnotherTerm(p.m_enTerm);
			bool			bAnotherPos = !clOutLinkID.m_bPos;

			if (pclOutLink == NULL) {
				CRPRCMidLink	&clOutLink = p.m_clMidLinkTable.New();

				clOutLink.m_clLinkID = clOutLinkID;

				//	for extend
				clOutLink.m_clFromPoint.x = pclInLink->m_clToPoint.x;
				clOutLink.m_clFromPoint.y = pclInLink->m_clToPoint.y;

				clOutLink.m_uiLinkCost = uiLinkCost;
				clOutLink.m_uiArriveCost = uiArriveCost;

				RPRCMidLink_ConnectLink(pclInLink, &clOutLink);

				pclOutLinkUsing->m_apclMidLink[p.m_enTerm][LINK_DIR_NO(clOutLinkID.m_bPos)] = &clOutLink;

				CRPRCMidLink	*pclAnotherMidLink = pclOutLinkUsing->m_apclMidLink[enAnotherTerm][LINK_DIR_NO(bAnotherPos)];
				if (pclAnotherMidLink != NULL) {
					p.m_vclConnectedLinkList.push_back(CRPRCConnectSearchResultLink(p.m_enTerm, &clOutLink));
				}
				if (bHaveUpLink) {
					p.m_vclResultLinkList.push_back(CRPRCUpSearchResultLink(&clOutLink));
				}
				if (pclAnotherMidLink == NULL && !bHaveUpLink) {
					clOutLink.m_itOpenLink = p.m_mapOpenTable.insert(RPRC_OpenTableValueType(uiArriveCost, &clOutLink));
				}
				//				MESSAGE( "%s, Level = %d, UR = %d, Tile = %d, Link = %d, pos = %s",
				//						 p.m_enTerm?"Start":"End  ",
				//						 p.m_iFromLevel,
				//						 clOutLinkID.m_clTileID.m_sUpdateRegionID,
				//						 clOutLinkID.m_clTileID.m_uiPackedTileID,
				//						 clOutLinkID.m_usLinkNo, clOutLinkID.m_bPos?"P":"N" );
			}
			else if (uiArriveCost < pclOutLink->m_uiArriveCost) {
				uint	uiCostDiff = pclOutLink->m_uiArriveCost - uiArriveCost;
				RPRCMidLink_DisconectLink(pclOutLink);
				RPRCMidLink_ConnectLink(pclInLink, pclOutLink);
				CRPRCRouteSearchCommFunc::UpdateMidLinkCost(pclOutLink, uiCostDiff, p.m_mapOpenTable);

				if (pclOutLinkUsing->m_apclMidLink[enAnotherTerm][LINK_DIR_NO(bAnotherPos)] != NULL) {
					p.m_vclConnectedLinkList.push_back(CRPRCConnectSearchResultLink(p.m_enTerm, pclOutLink));
				}
			}
		}
	} while (vclGatewayList.size() > 0);

	return SUCCESS;
}
