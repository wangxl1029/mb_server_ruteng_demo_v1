#include	"InnerCommon.hpp"
#include	"DPCommon.hpp"
#include	"DPNDSTypeDef.h"
#include	"DPDBConnectionPool.hpp"
#include	"DPData.hpp"
#include	"DPDataRouting.hpp"
#include	"DPProv.hpp"
#include	"DPProvRouting.hpp"

RESULT CDPProvRouting::Initialize(string strProductName, string strUpdateRegion, SmartPointer< CDPDBConnectionPool > spclDBConnectionPool)
{
	m_spclDBConnectionPool = spclDBConnectionPool;
	m_strProductName = strProductName;
	m_strUpdateRegion = strUpdateRegion;

	if (m_spclDBConnectionPool == NULL) {
		ERR("");
		return FAILURE;
	}

	if (SUCCESS != m_clDatabase.Connect(m_spclDBConnectionPool, strProductName.c_str(), strUpdateRegion.c_str(), DPDB_ROUTING)) {
		ERR("");
		return FAILURE;
	}

	m_clRoutingTileCache.SetCapacity(20);
	m_clRoutingAuxTileCache.SetCapacity(20);

	return SUCCESS;
}
RESULT CDPProvRouting::GetRoutingTile(uint uiPackedTileID, short sVersion, SmartPointer< CDPDataRoutingTile > &spclDPDataRoutingTile, uchar ucCoordWidth)
{
	if (m_bDbSwitching) {
		ERR("");
		return FAILURE;
	}

	spclDPDataRoutingTile = m_clRoutingTileCache.GetData(CTileID(uiPackedTileID, sVersion));
	if (spclDPDataRoutingTile == NULL) {
		CRCPtr< CSL_RoutingTileTableTblIt >				spclRoutingTileTableIt;
		if (SUCCESS != SL_CreateTblIt(m_clDatabase.Get(), &spclRoutingTileTableIt)) {
			ERR("");
			return FAILURE;
		}

		bool	bSelectFromNew = true;
		using namespace slm::routingTileTable;
		if (sVersion != 1) {
			if (SUCCESS != spclRoutingTileTableIt->Select(id == uiPackedTileID, (const char*)BACKUP_DB_NAME)) {
				ERR("");
				return FAILURE;
			}
			spclRoutingTileTableIt->Begin();
			if (spclRoutingTileTableIt->IsEnd()) {
				bSelectFromNew = true;
			}
			else {
				bSelectFromNew = false;
			}
		}
		if (bSelectFromNew) {
			if (SUCCESS != spclRoutingTileTableIt->Select(id == uiPackedTileID)) {
				ERR("");
				return FAILURE;
			}
			spclRoutingTileTableIt->Begin();
			if (spclRoutingTileTableIt->IsEnd()) {
				//	should use TileContentIndexTable
				//ERR( "" );
				//return FAILURE;
				return SUCCESS;
			}
		}
		CSL_RecRoutingTileTable		clRecRoutingTileTable;
		if (SUCCESS != spclRoutingTileTableIt->CurRec(clRecRoutingTileTable)) {
			ERR("");
			return FAILURE;
		}

		if (!spclDPDataRoutingTile.Create()) {
			ERR("");
			return FAILURE;
		}

		//uint	uiTick	=	PF_GetTickCount();
		if (SUCCESS != CRoutingTileTable_Reader().ReadAll(clRecRoutingTileTable.m_clNdsData.Get(), clRecRoutingTileTable.m_clNdsData.Size(), spclDPDataRoutingTile->m_clRoutingTile, ucCoordWidth)) {
			ERR("");
			return FAILURE;
		}
		//MESSAGE( "ReadAll Time Routing: TileID = %d, Time = %d", uiPackedTileID, PF_GetTickCount() - uiTick );

		//
		spclDPDataRoutingTile->m_clRoutingTileInfo.m_vclLinkList.resize(spclDPDataRoutingTile->m_clRoutingTile.m_clLinks.m_vclLink.size());

		for (size_t i = 0; i < spclDPDataRoutingTile->m_clRoutingTile.m_clSimpleIntersection.m_vclSimpleIntersection.size(); ++i) {
			CSimpleIntersection	&clNode = spclDPDataRoutingTile->m_clRoutingTile.m_clSimpleIntersection.m_vclSimpleIntersection[i];
			for (size_t j = 0; j < clNode.m_vclConnectedLinks.size(); ++j) {
				if (clNode.m_vclConnectedLinks[j].m_ucIsExternalLinkReference) {
					if (!clNode.m_vclConnectedLinks[j].m_clLinkReferenceChoice.ExternalLinkReference_Type()) {
						ERR("");
						return FAILURE;
					}
					CExternalDirectedLinkReference	&clExtLink = clNode.m_vclConnectedLinks[j].m_clLinkReferenceChoice.ExternalLinkReference_Ref();
					CDPRoutingTileExtLinkID			clExtLinkID(spclDPDataRoutingTile->m_clRoutingTile.m_clExternalTileIdList.m_viTileId[clExtLink.m_usExtTileIdx], clExtLink.m_clTileExternalLinkReference.m_usLinkId);
					CDPRoutingTileLinkNodeNo		clLinkNodeNo(i, j);
					spclDPDataRoutingTile->m_clRoutingTileInfo.m_mapExtLink2Node.insert(pair< CDPRoutingTileExtLinkID, CDPRoutingTileLinkNodeNo >(clExtLinkID, clLinkNodeNo));
				}
				else {
					if (!clNode.m_vclConnectedLinks[j].m_clLinkReferenceChoice.InternalLinkReference_Type()) {
						ERR("");
						return FAILURE;
					}
					CDirectedLinkReference			&clIntLink = clNode.m_vclConnectedLinks[j].m_clLinkReferenceChoice.InternalLinkReference_Ref();
					CDPRoutingTileLinkNodeNo		clLinkNodeNo(i, j);
					if (clIntLink.m_ucPositiveLinkDirection) {
						spclDPDataRoutingTile->m_clRoutingTileInfo.m_vclLinkList[clIntLink.m_usLinkId].m_clStartNode = clLinkNodeNo;
					}
					else {
						spclDPDataRoutingTile->m_clRoutingTileInfo.m_vclLinkList[clIntLink.m_usLinkId].m_clEndNode = clLinkNodeNo;
					}
				}
			}
		}

		for (size_t k = 0; k < spclDPDataRoutingTile->m_clRoutingTile.m_clLink2TileList.m_vclTileRef.size(); ++k) {
			CLink2TileRef	&clLink2TileRef = spclDPDataRoutingTile->m_clRoutingTile.m_clLink2TileList.m_vclTileRef[k];
			spclDPDataRoutingTile->m_clRoutingTileInfo.m_vclLinkList[clLink2TileRef.m_usLinkId].m_usLink2TileNo = k;
		}

		for (size_t l = 0; l < spclDPDataRoutingTile->m_clRoutingTile.m_clRouteUpLinks.m_vclRouteUpLinkRef.size(); ++l) {
			CRouteUpLinkRef	&clRouteUpLinkRef = spclDPDataRoutingTile->m_clRoutingTile.m_clRouteUpLinks.m_vclRouteUpLinkRef[l];
			spclDPDataRoutingTile->m_clRoutingTileInfo.m_vclLinkList[clRouteUpLinkRef.m_clFromLink.m_usLinkId].m_ausUpLinkRefNo[LINK_DIR_NO(clRouteUpLinkRef.m_clFromLink.m_ucPositiveLinkDirection)] = l;
		}

		//
		spclDPDataRoutingTile->m_clTileID.m_uiPackedTileID = uiPackedTileID;
		spclDPDataRoutingTile->m_clTileID.m_sVersion = sVersion;

		//	Make Attribute
		CRoutingTile		&clRoutingTile = spclDPDataRoutingTile->m_clRoutingTile;
		CDPRoutingTileInfo	&clRoutingTileInfo = spclDPDataRoutingTile->m_clRoutingTileInfo;

		clRoutingTileInfo.m_vclNodeList.resize(clRoutingTile.m_clSimpleIntersection.m_usNumIntersections);

		for (size_t j = 0; j < clRoutingTile.m_clAttributeMaps.m_vclAttrMap.size(); ++j) {
			CAttributeMap	&clAttributeMap = clRoutingTile.m_clAttributeMaps.m_vclAttrMap[j];

			if (clAttributeMap.m_ucReferenceType == ROUTING_SIMPLE_INTERSECTION) {
				if (clAttributeMap.m_vucAttrCodes.size() == 1) {
					uint	height = 0;
					uchar	c = clAttributeMap.m_vucAttrCodes[0];
					if (c == GATEWAY) {
						if (clAttributeMap.m_ucAttrType == VALUES_TO_ONE_FEATURE) {
							for (size_t k = 0; k < clAttributeMap.m_vclValues4OneFeature.size(); ++k) {
								CAttrVals4OneFeature	&clAttrVals4OneFeature = clAttributeMap.m_vclValues4OneFeature[k];
								CFeatureReference		&clFeatureReference = clAttrVals4OneFeature.m_clFeature;
								for (size_t l = 0; l < clAttrVals4OneFeature.m_clAttrValList.m_vclValues.size(); ++l) {
									CAttrValueList	&clAttrValueList = clAttrVals4OneFeature.m_clAttrValList;
									CAttributeValue	&clValue = clAttrVals4OneFeature.m_clAttrValList.m_vclValues[l];
									if (!clValue.GatewayNumber_Type()) {
										ERR("");
										return FAILURE;
									}
									uint	uiGatewayNo = clValue.GatewayNumber_Ref();

									if (!clFeatureReference.IntersectionId_Type()) {
										ERR("");
										return FAILURE;
									}
									uint	uiNodeNo = clFeatureReference.IntersectionId_Ref();
									if (uiNodeNo > clRoutingTileInfo.m_vclNodeList.size()) {
										ERR("");
										return FAILURE;
									}
									clRoutingTileInfo.m_vclNodeList[uiNodeNo].m_uiGatewayID = uiGatewayNo;
									clRoutingTileInfo.m_mapGatewayID2NodeNo[uiGatewayNo] = uiNodeNo;
								}
							}
						}
						else if (clAttributeMap.m_ucAttrType == VALUES_TO_MANY_FEATURES) {
							for (size_t k = 0; k < clAttributeMap.m_vclValues4ManyFeatures.size(); ++k) {
								CAttrVals4ManyFeatures	&clAttrVals4ManyFeatures = clAttributeMap.m_vclValues4ManyFeatures[k];
								for (size_t l = 0; l < clAttrVals4ManyFeatures.m_clAttrValList.m_vclValues.size(); ++l) {
									CAttrValueList	&clAttrValueList = clAttrVals4ManyFeatures.m_clAttrValList;
									CAttributeValue	&clValue = clAttrVals4ManyFeatures.m_clAttrValList.m_vclValues[l];
									if (!clValue.GatewayNumber_Type()) {
										ERR("");
										return FAILURE;
									}
									uint	uiGatewayNo = clValue.GatewayNumber_Ref();

									for (size_t m = 0; m < clAttrVals4ManyFeatures.m_vclFeature.size(); ++m) {
										CFeatureReference		&clFeatureReference = clAttrVals4ManyFeatures.m_vclFeature[m];
										if (!clFeatureReference.IntersectionId_Type()) {
											ERR("");
											return FAILURE;
										}
										uint	uiNodeNo = clFeatureReference.IntersectionId_Ref();
										if (uiNodeNo > clRoutingTileInfo.m_vclNodeList.size()) {
											ERR("");
											return FAILURE;
										}
										clRoutingTileInfo.m_vclNodeList[uiNodeNo].m_uiGatewayID = uiGatewayNo;
										clRoutingTileInfo.m_mapGatewayID2NodeNo[uiGatewayNo] = uiNodeNo;
									}
								}
							}
						}
						else if (clAttributeMap.m_ucAttrType == VALUES_TO_ALL_FEATURES) {
							for (size_t k = 0; k < clAttributeMap.m_vclvalues4AllFeatures.size(); ++k) {
								CAttrVals4OneFeatureImplicit	& clAttrVals4OneFeatureImplicit = clAttributeMap.m_vclvalues4AllFeatures[k];
								for (size_t m = 0; m < clAttrVals4OneFeatureImplicit.m_vclMultipleAttrValueList.size(); ++m) {
									CMultipleAttrValueList	&clMultipleAttrValueList = clAttrVals4OneFeatureImplicit.m_vclMultipleAttrValueList[m];
									CAttrValueList			&clAttrValueList = clMultipleAttrValueList.m_clAttrValList;
									for (size_t n = 0; n < clAttrValueList.m_vclValues.size(); ++n) {
										CAttributeValue	&clAttributeValue = clAttrValueList.m_vclValues[n];
										if (!clAttributeValue.GatewayNumber_Type()) {
											ERR("");
											return FAILURE;
										}
										uint	uiGatewayNo = clAttributeValue.GatewayNumber_Ref();
										for (size_t l = 0; l < clRoutingTileInfo.m_vclNodeList.size(); ++l) {
											clRoutingTileInfo.m_vclNodeList[l].m_uiGatewayID = uiGatewayNo;
											clRoutingTileInfo.m_mapGatewayID2NodeNo[uiGatewayNo] = l;
										}
									}
								}
							}
						}
					}
				}
			}
		}

		CDPCommon::PackedTileIDToTileNo(uiPackedTileID, spclDPDataRoutingTile->m_iLevel,
			spclDPDataRoutingTile->m_uiTileNo);
		CDPCommon::GetBasePointCoordOfTile(spclDPDataRoutingTile->m_iLevel,
			spclDPDataRoutingTile->m_uiTileNo,
			spclDPDataRoutingTile->m_clBasePoint.x,
			spclDPDataRoutingTile->m_clBasePoint.y);

		m_clRoutingTileCache.PutData(CTileID(uiPackedTileID, sVersion), spclDPDataRoutingTile);
	}

	return SUCCESS;
}

RESULT CDPProvRouting::GetRoutingAuxTile(uint uiPackedTileID, short sVersion, SmartPointer< CDPDataRoutingAuxTile > &spclDPDataRoutingAuxTile, uchar ucCoordWidth)
{
	if (m_bDbSwitching) {
		ERR("");
		return FAILURE;
	}

	uint	uiTick = PF_GetTickCount();

	spclDPDataRoutingAuxTile = m_clRoutingAuxTileCache.GetData(CTileID(uiPackedTileID, sVersion));
	if (spclDPDataRoutingAuxTile == NULL) {
		CRCPtr< CSL_RoutingAuxTileTableTblIt >			spclRoutingAuxTileTableIt;
		if (SUCCESS != SL_CreateTblIt(m_clDatabase.Get(), &spclRoutingAuxTileTableIt)) {
			ERR("");
			return FAILURE;
		}

		bool	bSelectFromNew = true;
		using namespace slm::routingAuxTileTable;
		if (sVersion != 1) {
			if (SUCCESS != spclRoutingAuxTileTableIt->Select(id == uiPackedTileID, (const char*)BACKUP_DB_NAME)) {
				ERR("");
				return FAILURE;
			}
			spclRoutingAuxTileTableIt->Begin();
			if (spclRoutingAuxTileTableIt->IsEnd()) {
				bSelectFromNew = true;
			}
			else {
				bSelectFromNew = false;
			}
		}
		if (bSelectFromNew) {
			if (SUCCESS != spclRoutingAuxTileTableIt->Select(id == uiPackedTileID)) {
				ERR("");
				return FAILURE;
			}
			spclRoutingAuxTileTableIt->Begin();
			if (spclRoutingAuxTileTableIt->IsEnd()) {
				//	should use TileContentIndexTable
				//ERR( "" );
				//return FAILURE;
				return SUCCESS;
			}
		}
		CSL_RecRoutingAuxTileTable		clRecRoutingAuxTileTable;
		if (SUCCESS != spclRoutingAuxTileTableIt->CurRec(clRecRoutingAuxTileTable)) {
			ERR("");
			return FAILURE;
		}

		if (!spclDPDataRoutingAuxTile.Create()) {
			ERR("");
			return FAILURE;
		}

		if (SUCCESS != CRoutingAuxTileTable_Reader().ReadAll(clRecRoutingAuxTileTable.m_clNdsData.Get(), clRecRoutingAuxTileTable.m_clNdsData.Size(), spclDPDataRoutingAuxTile->m_clRoutingAuxTile, ucCoordWidth)) {
			ERR("");
			return FAILURE;
		}

		spclDPDataRoutingAuxTile->m_clTileID.m_uiPackedTileID = uiPackedTileID;
		spclDPDataRoutingAuxTile->m_clTileID.m_sVersion = sVersion;

		m_clRoutingAuxTileCache.PutData(CTileID(uiPackedTileID, sVersion), spclDPDataRoutingAuxTile);
	}

	//MESSAGE( "GetRoutingAuxTile %d\n", PF_GetTickCount() - uiTick );

	return SUCCESS;
}
