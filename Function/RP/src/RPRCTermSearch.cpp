#include	"InnerCommon.hpp"
#include	"RPCommon.hpp"
#include	"RPRouteCalcRequest.hpp"
#include	"RPRCRouteCalcCommon.hpp"
#include	"RPRCTermSearch.hpp"

//	class CRPRCCmdTermSearchResult
CRPRCTermSearchResult::CRPRCTermSearchResult()
{
}

CRPRCTermSearchResult::~CRPRCTermSearchResult()
{
}

//	class CRPRCCmdTermSearch
CRPRCTermSearch::CRPRCTermSearch(RP_TERM enTerm, CRPWayPoint &clWayPoint, int iLevel, SmartPointer< CDPFacade > spclDataProvider)
: m_clWayPoint(clWayPoint), m_iLevel(iLevel), m_spclDataProvider(spclDataProvider)
{
}

CRPRCTermSearch::~CRPRCTermSearch()
{
}

// equivalent to CRPRCCmdTermSearch::Execute() from znavi
RESULT CRPRCTermSearch::Execute()
{
#if 1
	return TermSearch(m_enTerm, m_iLevel, m_spclLinkCostContainer, m_spclDataProvider, m_spclResult);
#else
	return TermSearch(m_enTerm, m_clWayPoint, m_iLevel,
		m_spclMidLinkUsingContainer,
		m_spclCost, m_spclLinkCostContainer,
		m_spclDataProvider,
		m_spclResult);
#endif
}
// equivalent to CRPRCCmdTermSearch::TermSearch() from znavi
RESULT CRPRCTermSearch::TermSearch(	RP_TERM enTerm, int iLevel,
									SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > > spclLinkCostContainer,
									SmartPointer< CDPFacade > spclDataProvider,
									SmartPointer< CRPRCTermSearchResult > &spclResult)
{
	//	Result Ref
	if (!spclResult.Create()) {
		ERR("");
		return FAILURE;
	}
#if 0
	if (!spclResult->m_spvclResultLinkList.Create()) {
		ERR("");
		return FAILURE;
	}
	if (!spclResult->m_spmapOpenTable.Create()) {
		ERR("");
		return FAILURE;
	}
	if (!spclResult->m_spclMidLinkTable.Create()) {
		ERR("");
		return FAILURE;
	}
	if (!spclResult->m_spvclConnectedLinkList.Create()) {
		ERR("");
		return FAILURE;
	}
	vector< CRPRSLink >						&vclResultLinkList = *(spclResult->m_spvclResultLinkList);
#endif
	RPRC_OpenTable							&clOpenTable = *(spclResult->m_spmapOpenTable);
#if 0
	RPRC_MidLinkTable						&clMidLinkTable = *(spclResult->m_spclMidLinkTable);
	vector< CRPRCConnectSearchResultLink >	&vclConnectedLinkList = *(spclResult->m_spvclConnectedLinkList);
#endif
	//	Mid Data Proxy
	CRPRCLinkCostTableProxy					clLinkCostTableProxy(spclLinkCostContainer);
#if 0
	CRPRCMidLinkUsingTableProxy				clMidLinkUsingTableProxy(spclMidLinkUsingContainer);
#endif
	//
	uint	uiTileNo = 0;
	CDPCommon::CoordToTileNo(m_iLevel, m_clWayPoint.m_clCoord.x, m_clWayPoint.m_clCoord.y, uiTileNo);

	CRPTileID	clTileID;
	CDPCommon::TileNoToPackedTileID(m_iLevel, uiTileNo, clTileID.m_uiPackedTileID);

	vector< string >	vstrUpdateRegion;
	if (SUCCESS != spclDataProvider->GetUpdateRegionByTile(BUILDING_BLOCK_ID_ROUTING, clTileID.m_uiPackedTileID, vstrUpdateRegion)) {
		ERR("");
		return FAILURE;
	}

	int		iAnchorX = 0;
	int		iAnchorY = 0;
	if (SUCCESS != CDPCommon::GetBasePointCoordOfTile(iLevel, uiTileNo, iAnchorX, iAnchorY)) {
		ERR("");
		return FAILURE;
	}

	uint	uiCoordShift = 0;
	spclDataProvider->GetCoordShift(iLevel, BUILDING_BLOCK_ID_ROUTING, uiCoordShift);
	for (size_t ur = 0; ur < vstrUpdateRegion.size(); ++ur) {
		clTileID.m_sUpdateRegionID = CDPCommon::UpdateRegionName2Id(vstrUpdateRegion[ur].c_str());

		CRPRCRoutingTileProxy			clRoutingTile(spclDataProvider);
		if (SUCCESS != clRoutingTile.GetData(clTileID)) {
			ERR("");
			return FAILURE;
		}

		if (clRoutingTile.IsEmpty()) {
			continue;
		}

		double	dMinDist = -1;
		size_t	iMinInt = 0;
		for (size_t i = 0; i < clRoutingTile->m_clRoutingTile.m_clSimpleIntersection.m_vclSimpleIntersection.size(); ++i) {
#if 0 // del by wxl --> No cancel available
			if (GetCancelFlag()) {
				return SUCCESS;
			}
#endif
			CSimpleIntersection	&clInt = clRoutingTile->m_clRoutingTile.m_clSimpleIntersection.m_vclSimpleIntersection[i];
			int		x = iAnchorX + (clInt.m_clPosition.m_iDx << uiCoordShift);
			int		y = iAnchorY + (clInt.m_clPosition.m_iDy << uiCoordShift);
			double	dx = m_clWayPoint.m_clCoord.x - x;
			double	dy = m_clWayPoint.m_clCoord.y - y;
			double	dist = dx * dx + dy * dy;
			if (dMinDist < 0 || dist < dMinDist) {
				dMinDist = dist;
				iMinInt = i;
			}
		}

		CSimpleIntersection		&clMinInt = clRoutingTile->m_clRoutingTile.m_clSimpleIntersection.m_vclSimpleIntersection[iMinInt];
		for (size_t j = 0; j < clMinInt.m_vclConnectedLinks.size(); ++j) {
#if 0
			if (GetCancelFlag()) {
				return SUCCESS;
			}
#endif
			CIntOrExtDirectedLinkReference	&clLinkRef = clMinInt.m_vclConnectedLinks[j];
			CRPTileID			clCurLinkTile = clTileID;
			ushort				usCurLinkNo = 0;
			bool				bCurLinkPos = true;
			if (clLinkRef.m_ucIsExternalLinkReference) {
				CExternalDirectedLinkReference	&clExtLink = clLinkRef.m_clLinkReferenceChoice.ExternalLinkReference_Ref();
				clCurLinkTile.m_uiPackedTileID = clRoutingTile->m_clRoutingTile.m_clExternalTileIdList.m_viTileId[clExtLink.m_usExtTileIdx];
				clCurLinkTile.m_sUpdateRegionID = clTileID.m_sUpdateRegionID;
				usCurLinkNo = clExtLink.m_clTileExternalLinkReference.m_usLinkId;
				bCurLinkPos = clExtLink.m_clTileExternalLinkReference.m_ucPositiveLinkDirection;
			}
			else {
				CDirectedLinkReference	&clIntLink = clLinkRef.m_clLinkReferenceChoice.InternalLinkReference_Ref();
				clCurLinkTile = clTileID;
				usCurLinkNo = clIntLink.m_usLinkId;
				bCurLinkPos = clIntLink.m_ucPositiveLinkDirection;
			}
			bool	bLinkRunPos = enTerm == RP_TERM_START ? bCurLinkPos : !bCurLinkPos;
			ushort	&usLinkCostRef = clLinkCostTableProxy.GetLinkCostRef(clCurLinkTile, usCurLinkNo, bLinkRunPos);
#if 0
			if (usLinkCostRef == 0) {
				usLinkCostRef = spclCost->LinkCost(CRPRCLinkID(clCurLinkTile, usCurLinkNo, bLinkRunPos), clRoutingTile);
			}
			uint	uiLinkCost = spclCost->ExtractShortCost(usLinkCostRef);
			CRPRCMidLink	&clMidLink = clMidLinkTable.New();
			clMidLink.m_clLinkID.m_clTileID = clCurLinkTile;
			//	for extend
			clMidLink.m_clFromPoint.x = clRoutingTile->m_clBasePoint.x + (clMinInt.m_clPosition.m_iDx << uiCoordShift);
			clMidLink.m_clFromPoint.y = clRoutingTile->m_clBasePoint.y + (clMinInt.m_clPosition.m_iDy << uiCoordShift);
			clMidLink.m_clLinkID.m_usLinkNo = usCurLinkNo;
			clMidLink.m_clLinkID.m_bPos = bCurLinkPos;
			clMidLink.m_uiLinkCost = uiLinkCost;
			clMidLink.m_uiArriveCost = uiLinkCost;
			clMidLink.m_itOpenLink = clOpenTable.insert(RPRC_OpenTableValueType(clMidLink.m_uiArriveCost, &clMidLink));
			//			MESSAGE( "TermSearch %s %d %d %s %d", enTerm==RP_TERM_START?"start":"end  ", clMidLink.m_clLinkID.m_clTileID.m_uiPackedTileID,
			//					 clMidLink.m_clLinkID.m_usLinkNo, clMidLink.m_clLinkID.m_bPos?"pos":"neg", clMidLink.m_uiArriveCost );

			CRPRSLink	clRSLink(clCurLinkTile, usCurLinkNo, bCurLinkPos);
			vclResultLinkList.push_back(clRSLink);

			CRPRCMidLinkUsing	*pclMidLinkUsing = clMidLinkUsingTableProxy.GetMidLinkUsing(clCurLinkTile, usCurLinkNo);
			pclMidLinkUsing->m_apclMidLink[enTerm][LINK_DIR_NO(bCurLinkPos)] = &clMidLink;
			if (pclMidLinkUsing->m_apclMidLink[RP_AnotherTerm(enTerm)][LINK_DIR_NO(!bCurLinkPos)] != NULL) {
				vclConnectedLinkList.push_back(CRPRCConnectSearchResultLink(enTerm, &clMidLink));
			}
#endif
		}
	}

	if (clOpenTable.size() == 0) {
		ERR("");
		return FAILURE;
	}

	return SUCCESS;
}