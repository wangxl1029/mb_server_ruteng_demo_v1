#include	"InnerCommon.hpp"
#include	"DPCommon.hpp"
#include	"DPDBConnectionPool.hpp"
#include	"DPData.hpp"
#include	"DPDataProduct.hpp"
#include	"DPProv.hpp"
#include	"DPProvProduct.hpp"

RESULT CDPProvProduct::Initialize(string strProductName, SmartPointer< CDPDBConnectionPool > spclDBConnectionPool)
{
	CCFMutexLocker	clLock(m_clMutex);

	m_spclDBConnectionPool = spclDBConnectionPool;
	m_strProductName = strProductName;

	if (m_spclDBConnectionPool == NULL) {
		ERR("");
		return FAILURE;
	}

	if (SUCCESS != m_clDatabase.Connect(m_spclDBConnectionPool, strProductName.c_str(), NULL, DPDB_PRODUCT)) {
		ERR("");
		return FAILURE;
	}

	CRCPtr< CSL_TileContentIndexTableTblIt >					spclTileTableIt;
	if (SUCCESS != SL_CreateTblIt(m_clDatabase.Get(), &spclTileTableIt)) {
		ERR("");
		return FAILURE;
	}

	using namespace slm::tileContentIndexTable;

	//	按buildingBlockId 和 levelNumber排序
	if (SUCCESS != spclTileTableIt->Select(CSL_Query().OrderBy(buildingBlockId & levelNumber))) {
		ERR("");
		return FAILURE;
	}
	for (spclTileTableIt->Begin(); !spclTileTableIt->IsEnd(); spclTileTableIt->Next()) {
		CSL_RecTileContentIndexTable		clRecTileTable;
		if (SUCCESS != spclTileTableIt->CurRec(clRecTileTable)) {
			ERR("");
			return FAILURE;
		}

		uint	uiTileWidth = 0;
		CDPCommon::GetTileWidth(clRecTileTable.m_iLevelNumber, uiTileWidth);

		int		iLeft = 0;
		int		iBottom = 0;
		CDPCommon::CalcLBPointCoordOfTile(clRecTileTable.m_iSouthWestTileId, iLeft, iBottom);

		int		iRight = iLeft + uiTileWidth * clRecTileTable.m_iNumTilesPerRow;
		int		iTop = iBottom + uiTileWidth * clRecTileTable.m_iNumTilesPerColumn;

		string	strUpdateRegionId = CDPCommon::UpdateRegionId2Name(clRecTileTable.m_iUpdateRegionId);

		//	写结果
		if (m_vclTile2RegionTable.size() == 0 || m_vclTile2RegionTable.back().m_sBuildingBlockId != clRecTileTable.m_iBuildingBlockId) {
			m_vclTile2RegionTable.push_back(CDPTile2RegionBuildingBlockRect(clRecTileTable.m_iBuildingBlockId));
		}
		CDPTile2RegionBuildingBlockRect	&clTile2RegionBuildingBlockRect = m_vclTile2RegionTable.back();
		if (clTile2RegionBuildingBlockRect.m_vclTile2RegionLevelRectList.size() == 0
			|| clTile2RegionBuildingBlockRect.m_vclTile2RegionLevelRectList.back().m_ucLevelNumber != clRecTileTable.m_iLevelNumber)
		{
			clTile2RegionBuildingBlockRect.m_vclTile2RegionLevelRectList.push_back(CDPTile2RegionLevelRect(clRecTileTable.m_iLevelNumber));
		}
		CDPTile2RegionLevelRect		&clTile2RegionLevelRect = clTile2RegionBuildingBlockRect.m_vclTile2RegionLevelRectList.back();
		clTile2RegionLevelRect.m_vclTile2RegionRectList.push_back(CDPTile2RegionRect(strUpdateRegionId, iLeft, iBottom, iRight, iTop));
	}

	//	UpdateRegion List
	CRCPtr< CSL_UpdateRegionTableTblIt >					spclUpdateRegionTableIt;
	if (SUCCESS != SL_CreateTblIt(m_clDatabase.Get(), &spclUpdateRegionTableIt)) {
		ERR("");
		return FAILURE;
	}

	using namespace slm::updateRegionTable;
	if (SUCCESS != spclUpdateRegionTableIt->Select()) {
		ERR("");
		return FAILURE;
	}
	for (spclUpdateRegionTableIt->Begin(); !spclUpdateRegionTableIt->IsEnd(); spclUpdateRegionTableIt->Next()) {
		CDPUpdateRegionInfo		clRecUpdateRegionTable;
		if (SUCCESS != spclUpdateRegionTableIt->CurRec(clRecUpdateRegionTable.m_clUpdateRegionRec)) {
			ERR("");
			return FAILURE;
		}

		clRecUpdateRegionTable.m_strUpdateRegionName = CDPCommon::UpdateRegionId2Name(clRecUpdateRegionTable.m_clUpdateRegionRec.m_iUpdateRegionId);

		m_vclUpdateRegionList.push_back(clRecUpdateRegionTable);
	}

	return SUCCESS;
}

RESULT CDPProvProduct::SwitchDbStart()
{
	return SUCCESS;
}

RESULT CDPProvProduct::WaitForCanSwitchDb()
{
	m_bDbSwitching = true;
	return SUCCESS;
}

RESULT CDPProvProduct::SwitchDbEnd()
{
	m_bDbSwitching = false;
	return SUCCESS;
}

RESULT CDPProvProduct::GetUpdateRegionByTile(BUILDING_BLOCK_ID enBuildingBlockID, uint uiPackedTileID, vector< string > &vstrUpdateRegionList)
{
	if (m_bDbSwitching) {
		ERR("");
		return FAILURE;
	}

	CCFMutexLocker	clLock(m_clMutex);

	int		iNeedLevelNo = 0;
	uint	uiNeedTileNo = 0;
	CDPCommon::PackedTileIDToTileNo(uiPackedTileID, iNeedLevelNo, uiNeedTileNo);

	int		iBaseX = 0;
	int		iBaseY = 0;
	CDPCommon::GetBasePointCoordOfTile(iNeedLevelNo, uiNeedTileNo, iBaseX, iBaseY);

	uint	uiTileWidth = 0;
	CDPCommon::GetTileWidth(iNeedLevelNo, uiTileWidth);

	//	every building block
	for (size_t i = 0; i < m_vclTile2RegionTable.size(); ++i) {
		CDPTile2RegionBuildingBlockRect	&clBuildingBlockRect = m_vclTile2RegionTable[i];
		if (enBuildingBlockID == clBuildingBlockRect.m_sBuildingBlockId) {
			//	every level
			for (size_t j = 0; j < clBuildingBlockRect.m_vclTile2RegionLevelRectList.size(); ++j) {
				CDPTile2RegionLevelRect	&clLevelRect = clBuildingBlockRect.m_vclTile2RegionLevelRectList[j];
				if (iNeedLevelNo == clLevelRect.m_ucLevelNumber) {
					//	every rect
					for (size_t k = 0; k < clLevelRect.m_vclTile2RegionRectList.size(); ++k) {
						CDPTile2RegionRect	&clRect = clLevelRect.m_vclTile2RegionRectList[k];
						if (clRect.m_iLeft <= iBaseX && iBaseX <= clRect.m_iRight
							&& clRect.m_iBottom <= iBaseY && iBaseY <= clRect.m_iTop)
						{
							int		iUpdateRegion = CDPCommon::UpdateRegionName2Id(clRect.m_strUpdateRegionId.c_str());

							//	先从Cache中取得
							CTileContentKey	clKey(iUpdateRegion, enBuildingBlockID, iNeedLevelNo);
							SmartPointer< CSL_RecTileContentIndexTable >	spclRecTileContentIndexTable = mclTileContentIndexCache.GetData(clKey);
							if (spclRecTileContentIndexTable.Get() == NULL) {
								//	从Cache中取不到，从DB中读取
								CRCPtr< CSL_TileContentIndexTableTblIt >					spclTileContentIndexTableIt;
								if (SUCCESS != SL_CreateTblIt(m_clDatabase.Get(), &spclTileContentIndexTableIt)) {
									ERR("");
									return FAILURE;
								}
								using namespace slm::tileContentIndexTable;
								if (SUCCESS != spclTileContentIndexTableIt->Select(updateRegionId == iUpdateRegion && buildingBlockId == (int)enBuildingBlockID && levelNumber == iNeedLevelNo)) {
									ERR("");
									return FAILURE;
								}
								spclTileContentIndexTableIt->Begin();
								if (spclTileContentIndexTableIt->IsEnd()) {
									continue;
								}
								if (!spclRecTileContentIndexTable.Create()) {
									ERR("");
									return FAILURE;
								}

								if (SUCCESS != spclTileContentIndexTableIt->CurRec(*spclRecTileContentIndexTable.Get())) {
									ERR("");
									return FAILURE;
								}
								mclTileContentIndexCache.PutData(clKey, spclRecTileContentIndexTable);
							}

							//	左下角的Tile的左下经纬度
							int		iLBLevelNo = 0;
							uint	uiLBTileNo = 0;
							CDPCommon::PackedTileIDToTileNo(spclRecTileContentIndexTable->m_iSouthWestTileId, iLBLevelNo, uiLBTileNo);

							int		iLBLon = 0;
							int		iLBLat = 0;
							CDPCommon::GetLBPointCoordOfTile(iLBLevelNo, uiLBTileNo, iLBLon, iLBLat);

							int		iXNo = (iBaseX - iLBLon) / uiTileWidth;	//	Tile所在的列号
							int		iYNo = (iBaseY - iLBLat) / uiTileWidth;	//	Tile所在的行号

							uint	uiBitNo = iYNo * spclRecTileContentIndexTable->m_iNumTilesPerRow + iXNo;
							uint	uiByteNo = uiBitNo / 8;
							uint	uiBitRemain = uiBitNo % 8;
							uchar	*pucBuf = (uchar*)spclRecTileContentIndexTable->m_clTileContentIndex.Get();
							uchar	ucByte = pucBuf[uiByteNo];
							uchar	uiBitVal = ((ucByte >> (7 - uiBitRemain)) & 0x1);
							if (uiBitVal) {
								vstrUpdateRegionList.push_back(clRect.m_strUpdateRegionId);
							}
						}
					}
					break;	//	Level Loop
				}
			}
			break;	//	BuildingBlock Loop
		}
	}

	return SUCCESS;
}

RESULT CDPProvProduct::GetGatewayByID(uint uiGatewayID, vector< CDPGatewayInfo > &vclGatewayList)
{
	if (m_bDbSwitching) {
		ERR("");
		return FAILURE;
	}

	CCFMutexLocker	clLock(m_clMutex);
	CRCPtr< CSL_GlobalGatewayTableTblIt >					spclGlobalGatewayTableIt;
	if (SUCCESS != SL_CreateTblIt(m_clDatabase.Get(), &spclGlobalGatewayTableIt)) {
		ERR("");
		return FAILURE;
	}
	using namespace slm::globalGatewayTable;
	if (SUCCESS != spclGlobalGatewayTableIt->Select(gatewayId == uiGatewayID)) {
		ERR("");
		return FAILURE;
	}
	vclGatewayList.reserve(3);
	for (spclGlobalGatewayTableIt->Begin(); !spclGlobalGatewayTableIt->IsEnd(); spclGlobalGatewayTableIt->Next()) {
		CDPGatewayInfo	clGatewayInfo;
		if (SUCCESS != spclGlobalGatewayTableIt->CurRec(clGatewayInfo.m_clGatewayInfo)) {
			ERR("");
			return FAILURE;
		}
		vclGatewayList.push_back(clGatewayInfo);
	}

	return SUCCESS;
}

RESULT CDPProvProduct::GetUpdateRegionList(vector< CDPUpdateRegionInfo > &vclUpdateRegionList)
{
	vclUpdateRegionList = m_vclUpdateRegionList;
	return SUCCESS;
}
