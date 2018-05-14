#include	"InnerCommon.hpp"
#include	"RPCommon.hpp"
#include	"RPRSLink.hpp"
#include	"RPWayPoints.hpp"
//#include	"RPSection.h"
//#include	"RPRoute.h"
#include	"RPRouteCalcRequest.hpp"
//#include	"RPRCCmdBase.h"
#include	"RPRCRouteCalcCommon.hpp"
#include	"RPRCCost.hpp"
//#include	"RPRCExtIF.h"
//#include	"RPRCCmdCalcBase.h"
#include	"RPRCRouteSearchCommFunc.hpp"
#include	"RPRCConnectSearch.hpp"

//	class CRPRCCmdUpSearchResult
CRPRCConnectSearchResult::CRPRCConnectSearchResult()
{
}

CRPRCConnectSearchResult::~CRPRCConnectSearchResult()
{
}

//	class CRPRCCmdConnectSearch
CRPRCConnectSearch::CRPRCConnectSearch(//RPRCCmdSigReciever hReciever, RPRC_Cmd_PRI enCmdPRI,
	long lRouteID, //SmartPointer< CRPRCExtIF > spclExtIF,
	RP_TERM enTerm, int iLevel, CRPWayPoint &clStartWayPoint, CRPWayPoint &clEndWayPoint,
	SmartPointer< RPRC_OpenTable > spmapOpenTable,
	SmartPointer< RPRC_MidLinkTable > spclMidLinkTable,
	SmartPointer< RPRCTileContainer< CRPRCMidLinkUsingTile > > spclMidLinkUsingContainer,
	SmartPointer< CRPRCCost > spclCost,
	SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > > spclLinkCostContainer,
	SmartPointer< CDPFacade > spclDataProvider)
	: //CRPRCCmdCalcBase(RPRC_Cmd_Type_Route_ConnectSearch, hReciever, enCmdPRI, lRouteID, spclExtIF), 
	m_enTerm(enTerm), m_iLevel(iLevel)
	, m_clStartWayPoint(clStartWayPoint), m_clEndWayPoint(clEndWayPoint)
	, m_spmapOpenTable(spmapOpenTable)
	, m_spclMidLinkTable(spclMidLinkTable)
	, m_spclMidLinkUsingContainer(spclMidLinkUsingContainer)
	, m_spclCost(spclCost)
	, m_spclLinkCostContainer(spclLinkCostContainer)
	, m_spclDataProvider(spclDataProvider)
{
	if (spclDataProvider == NULL) {
		ERR("");
	}
}

RESULT CRPRCConnectSearch::Execute()
{
	return ConnectSearch(m_enTerm, m_iLevel, m_clStartWayPoint, m_clEndWayPoint,
		m_spmapOpenTable, m_spclMidLinkTable, m_spclMidLinkUsingContainer,
		m_spclCost, m_spclLinkCostContainer, m_spclDataProvider,
		m_spclResult);
}

CRPRCConnectSearch::~CRPRCConnectSearch()
{
}

SmartPointer< CRPRCConnectSearchResult > CRPRCConnectSearch::GetResult()
{
	return m_spclResult;
}

RESULT CRPRCConnectSearch::ConnectSearch(RP_TERM enTerm, int iLevel, CRPWayPoint &clStartWayPoint, CRPWayPoint &clEndWayPoint,
	SmartPointer< RPRC_OpenTable > spmapOpenTable,
	SmartPointer< RPRC_MidLinkTable > spvclMidLinkTable,
	SmartPointer< RPRCTileContainer< CRPRCMidLinkUsingTile > > spclMidLinkUsingContainer,
	SmartPointer< CRPRCCost > spclCost,
	SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > > spclLinkCostContainer,
	SmartPointer< CDPFacade > spclDataProvider,
	SmartPointer< CRPRCConnectSearchResult > &spclResult)
{
	if (spclResult == NULL || spclResult->m_spvclConnectedLinkList == NULL)
	{
		ERR("");
		return FAILURE;
	}

	vector< CRPRCConnectSearchResultLink >	&vclConnectedLinkList = *(spclResult->m_spvclConnectedLinkList);

	//	Mid Data Proxy
	uint									uiCoordShift = 0;
	spclDataProvider->GetCoordShift(iLevel, BUILDING_BLOCK_ID_ROUTING, uiCoordShift);
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
	const size_t									uiDirAlterThreshold = 3;
	RESULT									retcode = SUCCESS;

	CCheckEndParam	clCheckEndParam(	enTerm, iLevel,
										mapOpenTable,
										uiExtendedLinkCount,
										vclConnectedLinkList);

	CExtendParam	clExtendParam(	enTerm, iLevel, uiCoordShift,
									mapOpenTable,
									clMidLinkTable,
									clMidLinkUsingTableProxy,
									clInLinkRoutingTile,
									clNodeRoutingTile,
									clOutLinkRoutingTile,
									clCost,
									clLinkCostTableProxy,
									spclDataProvider,
									vclConnectedLinkList
									/*, spclExtIF*/);

	while (!CheckEnd(clCheckEndParam)) {
		if (SUCCESS != Extend(clExtendParam))
		{
			ERR("");
			retcode =  FAILURE;
			break;
		}
		++uiExtendedLinkCount;
		if (uiExtendedLinkCount >= uiDirAlterThreshold)
		{
			retcode = RP_RETURN_CONTINUE;
			break;
		}
	}

	return retcode;
}

bool CRPRCConnectSearch::CheckEnd(CRPRCConnectSearch::CCheckEndParam &p)
{
	if (p.m_mapOpenTable.size() == 0) {
		//MESSAGE( "CRPRCCmdConnectSearch p.m_mapOpenTable.size() == 0" );
		return true;
	}
	if (p.m_vclConnectedLinkList.size() > 5) {
		//MESSAGE( "CRPRCCmdConnectSearch p.m_vclConnectedLinkList.size() %d", p.m_vclConnectedLinkList.size() );
		return true;
	}
	return false;
}

RESULT CRPRCConnectSearch::Extend(CRPRCConnectSearch::CExtendParam &p)
{
	RPRC_OpenTableIterator	it = p.m_mapOpenTable.begin();
	uint					uiInLinkArriveCost = it->first;
	CRPRCMidLink				*pclInLink = it->second;
	p.m_mapOpenTable.erase(it);

	CRPRCLinkID				&clInLinkID = pclInLink->m_clLinkID;

	//	MESSAGE( "CRPRCCmdConnectSearch::Extend tid = %d, tileid = %d, linkid = %d",
	//			 pthread_self(), clInLinkID.m_clTileID.m_uiPackedTileID, clInLinkID.m_usLinkNo );

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
			if (iTileLevel != p.m_iLevel) {
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
#if 0 //del by wxl --> no cancel available
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

				if (pclOutLinkUsing->m_apclMidLink[enAnotherTerm][LINK_DIR_NO(bAnotherPos)] != NULL) {
					p.m_vclConnectedLinkList.push_back(CRPRCConnectSearchResultLink(p.m_enTerm, &clOutLink));
				}
				else {
					clOutLink.m_itOpenLink = p.m_mapOpenTable.insert(RPRC_OpenTableValueType(uiArriveCost, &clOutLink));
				}
				//				MESSAGE( "%s, Level = %d, UR = %d, Tile = %d, Link = %d, pos = %s",
				//						 p.m_enTerm?"Start":"End  ",
				//						 p.m_iLevel,
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

