#include	"InnerCommon.hpp"
#include	"RPCommon.hpp"
#include	"RPRCRouteCalcCommon.hpp"
#include	"RPRouteDataLoader.hpp"

CRPRouteDataLoader::CRPRouteDataLoader()
{
}

CRPRouteDataLoader::~CRPRouteDataLoader()
{
}

RESULT CRPRouteDataLoader::Initialize(SmartPointer< CDPFacade > spclDataProvide)
{
	if (spclDataProvide == NULL)
	{
		ERR("");
		return FAILURE;
	}

	m_spclDataProvider = spclDataProvide;

	return SUCCESS;
}

//get the shape points from database
void CRPRouteDataLoader::MakeLinkShapePointList(CRPRCLinkID& routeLinkId, vector<CPointCoord2D>& vecShapePointList)
{
	int		iLinkLevel = 0;
	uint	uiLinkTileNo = 0;
	CDPCommon::PackedTileIDToTileNo(routeLinkId.m_clTileID.m_uiPackedTileID, iLinkLevel, uiLinkTileNo);

	int		iLinkTileBaseX = 0;
	int		iLinkTileBaseY = 0;
	CDPCommon::GetBasePointCoordOfTile(iLinkLevel, uiLinkTileNo, iLinkTileBaseX, iLinkTileBaseY);

	uint	uiLinkTileCoordShift = 0;

#if 1
	m_spclDataProvider->GetCoordShift(iLinkLevel, BUILDING_BLOCK_ID_ROUTING, uiLinkTileCoordShift);
	string strUpdateRegion = CDPCommon::UpdateRegionId2Name(routeLinkId.m_clTileID.m_sUpdateRegionID);
#else
	m_spclDataProvider->GetCoordShift(iLinkLevel, BUILDING_BLOCK_KEY_ROUTING, uiLinkTileCoordShift);
	string strUpdateRegion;
	if (SUCCESS != m_spclDataProvider->UpdateRegionId2Name(routeLinkId.m_clTileID.m_sUpdateRegionID, strUpdateRegion)) 
	{
		ERR("");
		return;
	}
#endif

	SmartPointer< CDPDataRoutingTile >		spclRoutingTile;
	if (SUCCESS != m_spclDataProvider->GetRoutingTileData(strUpdateRegion, iLinkLevel, uiLinkTileNo, NEW_VERSION, spclRoutingTile)) {
		ERR("");
		return;
	}
	SmartPointer< CDPDataRoutingAuxTile >	spclRoutingAuxTile;
	if (SUCCESS != m_spclDataProvider->GetRoutingAuxData(strUpdateRegion, iLinkLevel, uiLinkTileNo, NEW_VERSION, spclRoutingAuxTile)) {
		ERR("");
		return;
	}

	vector< CPointCoord2D > vclShapePointPos;
	CPointCoord2D clShapePointPos;
	if (spclRoutingAuxTile->m_clRoutingAuxTile.m_clRoadShapes.m_vclShapePointInfo.size() > routeLinkId.m_usLinkNo) {
		CShapePointInfo		&clShapePointInfo = spclRoutingAuxTile->m_clRoutingAuxTile.m_clRoadShapes.m_vclShapePointInfo[routeLinkId.m_usLinkNo];
		if (clShapePointInfo.m_ucShapePointType == 0) {
			//	BASE_LINK_NO_GEOMETRY, only start and end point
			CDPRoutingTileLinkInfo	&clLinkInfo = spclRoutingTile->m_clRoutingTileInfo.m_vclLinkList[routeLinkId.m_usLinkNo];
			if (clLinkInfo.m_clStartNode.m_usNodeNo == NDS_INVALID_NODE_ID || clLinkInfo.m_clEndNode.m_usNodeNo == NDS_INVALID_NODE_ID) {
				ERR("");
			}
			CSimpleIntersection	&clStartInt = spclRoutingTile->m_clRoutingTile.m_clSimpleIntersection.m_vclSimpleIntersection[clLinkInfo.m_clStartNode.m_usNodeNo];
			clShapePointPos.x = iLinkTileBaseX + (clStartInt.m_clPosition.m_iDx << uiLinkTileCoordShift);
			clShapePointPos.y = iLinkTileBaseY + (clStartInt.m_clPosition.m_iDy << uiLinkTileCoordShift);
			vclShapePointPos.push_back(clShapePointPos);

			CSimpleIntersection	&clEndInt = spclRoutingTile->m_clRoutingTile.m_clSimpleIntersection.m_vclSimpleIntersection[clLinkInfo.m_clEndNode.m_usNodeNo];
			clShapePointPos.x = iLinkTileBaseX + (clEndInt.m_clPosition.m_iDx << uiLinkTileCoordShift);
			clShapePointPos.y = iLinkTileBaseY + (clEndInt.m_clPosition.m_iDy << uiLinkTileCoordShift);
			vclShapePointPos.push_back(clShapePointPos);
		}
		else if (clShapePointInfo.m_ucShapePointType == 1) {
			//	BASE_LINK_WITH_GEOMETRY
			CDPRoutingTileLinkInfo	&clLinkInfo = spclRoutingTile->m_clRoutingTileInfo.m_vclLinkList[routeLinkId.m_usLinkNo];
			if (clLinkInfo.m_clStartNode.m_usNodeNo == NDS_INVALID_NODE_ID || clLinkInfo.m_clEndNode.m_usNodeNo == NDS_INVALID_NODE_ID) {
				ERR("");
			}
			CSimpleIntersection	&clStartInt = spclRoutingTile->m_clRoutingTile.m_clSimpleIntersection.m_vclSimpleIntersection[clLinkInfo.m_clStartNode.m_usNodeNo];
			clShapePointPos.x = iLinkTileBaseX + (clStartInt.m_clPosition.m_iDx << uiLinkTileCoordShift);
			clShapePointPos.y = iLinkTileBaseY + (clStartInt.m_clPosition.m_iDy << uiLinkTileCoordShift);
			vclShapePointPos.push_back(clShapePointPos);

			for (size_t j = 0; j < clShapePointInfo.m_clShapePoints.m_clCoord.m_vclOffsets.size(); ++j) {
				CCoordXYOffset	&clCoord = clShapePointInfo.m_clShapePoints.m_clCoord.m_vclOffsets[j];
				clShapePointPos.x += (clCoord.m_iDx << uiLinkTileCoordShift);
				clShapePointPos.y += (clCoord.m_iDy << uiLinkTileCoordShift);
				vclShapePointPos.push_back(clShapePointPos);
			}

			CSimpleIntersection	&clEndInt = spclRoutingTile->m_clRoutingTile.m_clSimpleIntersection.m_vclSimpleIntersection[clLinkInfo.m_clEndNode.m_usNodeNo];
			clShapePointPos.x = iLinkTileBaseX + (clEndInt.m_clPosition.m_iDx << uiLinkTileCoordShift);
			clShapePointPos.y = iLinkTileBaseY + (clEndInt.m_clPosition.m_iDy << uiLinkTileCoordShift);
			vclShapePointPos.push_back(clShapePointPos);
		}
		else {
			//	ROUTE_LINK, RoadGeoLine
			// this tile
			CRoadGeoLineList	&clRoadGeoLineList = spclRoutingAuxTile->m_clRoutingAuxTile.m_clRoadGeoLine;
			size_t i = 0;
			for (; i < clRoadGeoLineList.m_vclRoadGeoLine.size(); ++i) {
				CRoadGeoLine		&clRoadGeoLine = clRoadGeoLineList.m_vclRoadGeoLine[i];
				if ((!clRoadGeoLine.m_clRouteLinkFeatureId.m_ucIsExternalLinkId &&
					clRoadGeoLine.m_clRouteLinkFeatureId.m_clLinkReferenceChoice.InternalLinkId_Ref() == routeLinkId.m_usLinkNo))
				{
					break;
				}
			}
			if (i < clRoadGeoLineList.m_vclRoadGeoLine.size()) {
				
				CRoadGeoLine		&clRoadGeoLine = clRoadGeoLineList.m_vclRoadGeoLine[i];
				clShapePointPos.x = iLinkTileBaseX + (clRoadGeoLine.m_clShapes.m_clV0.m_iDx << uiLinkTileCoordShift);
				clShapePointPos.y = iLinkTileBaseY + (clRoadGeoLine.m_clShapes.m_clV0.m_iDy << uiLinkTileCoordShift);
				vclShapePointPos.push_back(clShapePointPos);
				for (size_t k = 0; k < clRoadGeoLine.m_clShapes.m_vclOffsets.size(); ++k) {
					clShapePointPos.x += (clRoadGeoLine.m_clShapes.m_vclOffsets[k].m_iDx << uiLinkTileCoordShift);
					clShapePointPos.y += (clRoadGeoLine.m_clShapes.m_vclOffsets[k].m_iDy << uiLinkTileCoordShift);
					vclShapePointPos.push_back(clShapePointPos);
				}
			}

			// extend tile
			for (size_t l = 0; l < spclRoutingTile->m_clRoutingTile.m_clLink2TileList.m_vclTileRef.size(); ++l) {
				CLink2TileRef	&clLink2TileRef = spclRoutingTile->m_clRoutingTile.m_clLink2TileList.m_vclTileRef[l];

				if (routeLinkId.m_usLinkNo == clLink2TileRef.m_usLinkId)
				{
					for (size_t m = 0; m < clLink2TileRef.m_clExtTileColl.m_vusExtTileIdx.size(); ++m)
					{
						ushort usExtTileIdx = clLink2TileRef.m_clExtTileColl.m_vusExtTileIdx[m];

						uint uiExtPackedTileID = spclRoutingTile->m_clRoutingTile.m_clExternalTileIdList.m_viTileId[usExtTileIdx];

						int		iExtLinkLevel = 0;
						uint	uiExtLinkTileNo = 0;
						CDPCommon::PackedTileIDToTileNo(uiExtPackedTileID, iExtLinkLevel, uiExtLinkTileNo);

						SmartPointer< CDPDataRoutingAuxTile >	spclExtRoutingAuxTile;
						if (SUCCESS != m_spclDataProvider->GetRoutingAuxData(strUpdateRegion, iExtLinkLevel, uiExtLinkTileNo, NEW_VERSION, spclExtRoutingAuxTile)) {
							ERR("");
							return;
						}

						SmartPointer< CDPDataRoutingTile >	spclExtRoutingTile;
						if (SUCCESS != m_spclDataProvider->GetRoutingTileData(strUpdateRegion, iExtLinkLevel, uiExtLinkTileNo, NEW_VERSION, spclExtRoutingTile)) {
							ERR("");
							return;
						}
						vector< int >	&viExtTileIdList = spclExtRoutingTile->m_clRoutingTile.m_clExternalTileIdList.m_viTileId;

						int		iExtLinkTileBaseX = 0;
						int		iExtLinkTileBaseY = 0;
						CDPCommon::GetBasePointCoordOfTile(iExtLinkLevel, uiExtLinkTileNo, iExtLinkTileBaseX, iExtLinkTileBaseY);

						CRoadGeoLineList	&clExtRoadGeoLineList = spclExtRoutingAuxTile->m_clRoutingAuxTile.m_clRoadGeoLine;
						size_t n = 0;
						for (; n < clExtRoadGeoLineList.m_vclRoadGeoLine.size(); ++n)
						{
							CRoadGeoLine &clExtRoadGeoLine = clExtRoadGeoLineList.m_vclRoadGeoLine[n];
							if (clExtRoadGeoLine.m_clRouteLinkFeatureId.m_ucIsExternalLinkId &&
								clExtRoadGeoLine.m_clRouteLinkFeatureId.m_clLinkReferenceChoice.ExternalLinkId_Ref().m_usLinkId == routeLinkId.m_usLinkNo &&
								viExtTileIdList[clExtRoadGeoLine.m_clRouteLinkFeatureId.m_clLinkReferenceChoice.ExternalLinkId_Ref().m_usExtTileIdx] == routeLinkId.m_clTileID.m_uiPackedTileID)
							{
								clShapePointPos.x = iExtLinkTileBaseX + (clExtRoadGeoLine.m_clShapes.m_clV0.m_iDx << uiLinkTileCoordShift);
								clShapePointPos.y = iExtLinkTileBaseY + (clExtRoadGeoLine.m_clShapes.m_clV0.m_iDy << uiLinkTileCoordShift);
								vclShapePointPos.push_back(clShapePointPos);
								for (size_t z = 0; z < clExtRoadGeoLine.m_clShapes.m_vclOffsets.size(); ++z) {
									clShapePointPos.x += (clExtRoadGeoLine.m_clShapes.m_vclOffsets[z].m_iDx << uiLinkTileCoordShift);
									clShapePointPos.y += (clExtRoadGeoLine.m_clShapes.m_vclOffsets[z].m_iDy << uiLinkTileCoordShift);
									vclShapePointPos.push_back(clShapePointPos);
								}
								break;
							}
						}
					}
				}
			}
		}
	}

	if (routeLinkId.m_bPos == true) {
		for (int i = 0; i < vclShapePointPos.size(); ++i) {
			vecShapePointList.push_back(vclShapePointPos[i]);
		}
	}
	else {
		for (int i = vclShapePointPos.size() - 1; i >= 0; --i) {
			vecShapePointList.push_back(vclShapePointPos[i]);
		}
	}
}
