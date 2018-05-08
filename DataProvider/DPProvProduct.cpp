#include "InnerCommon.hpp"
#include "DPCommon.hpp"
#include "DPDBConnectionPool.hpp"
#include "DPProvProduct.hpp"

bool CDPProvProduct::Initialize(string strProductName, shared_ptr< CDPDBConnectionPool > spclDBConnectionPool)
{
	m_spclDBConnectionPool = spclDBConnectionPool;
	m_strProductName = strProductName;

	if (m_spclDBConnectionPool == nullptr) {
		ERR("");
		return false;
	}

	if ( ! m_clDatabase.Connect(m_spclDBConnectionPool, strProductName.c_str(), NULL, DPDB_PRODUCT)) {
		ERR("");
		return false;
	}

	CRCPtr< CSL_TileContentIndexTableTblIt >					spclTileTableIt;
	if (SUCCESS != SL_CreateTblIt(m_clDatabase.Get(), &spclTileTableIt)) {
		ERR("");
		return false;
	}

	using namespace slm::tileContentIndexTable;

	//	按buildingBlockId 和 levelNumber排序
	if (SUCCESS != spclTileTableIt->Select(CSL_Query().OrderBy(buildingBlockId & levelNumber))) {
		ERR("");
		return false;
	}

	for (spclTileTableIt->Begin(); !spclTileTableIt->IsEnd(); spclTileTableIt->Next()) {
		CSL_RecTileContentIndexTable		clRecTileTable;
		if (SUCCESS != spclTileTableIt->CurRec(clRecTileTable)) {
			ERR("");
			return false;
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
		return false;
	}

	using namespace slm::updateRegionTable;
	if (SUCCESS != spclUpdateRegionTableIt->Select()) {
		ERR("");
		return false;
	}
	for (spclUpdateRegionTableIt->Begin(); !spclUpdateRegionTableIt->IsEnd(); spclUpdateRegionTableIt->Next()) {
		CDPUpdateRegionInfo		clRecUpdateRegionTable;
#if 0
		if (SUCCESS != spclUpdateRegionTableIt->CurRec(clRecUpdateRegionTable.m_clUpdateRegionRec)) {
			ERR("");
			return false;
		}

		clRecUpdateRegionTable.m_strUpdateRegionName = CDPCommon::UpdateRegionId2Name(clRecUpdateRegionTable.m_clUpdateRegionRec.m_iUpdateRegionId);
#endif
		m_vclUpdateRegionList.push_back(clRecUpdateRegionTable);
	}

	return true;
}

bool CDPProvProduct::GetUpdateRegionByTile(BUILDING_BLOCK_ID enBuildingBlockID, uint32_t uiPackedTileID, vector< string > &vstrUpdateRegionList)
{
	return false;
}