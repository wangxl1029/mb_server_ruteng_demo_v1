#pragma once
#include "DPCommon.hpp"

class CDPTile2RegionRect
{
public:
	CDPTile2RegionRect(string strUpdateRegionId = "", int iLeft = 0, int iBottom = 0, int iRight = 0, int iTop = 0)
		: m_strUpdateRegionId(strUpdateRegionId), m_iLeft(iLeft), m_iBottom(iBottom), m_iRight(iRight), m_iTop(iTop) {}

	string														m_strUpdateRegionId;
	int															m_iLeft;
	int															m_iBottom;
	int															m_iRight;
	int															m_iTop;
};

class CDPTile2RegionLevelRect
{
public:
	CDPTile2RegionLevelRect(uchar ucLevelNumber = 0) : m_ucLevelNumber(ucLevelNumber) {}

	uchar														m_ucLevelNumber;
	vector< CDPTile2RegionRect >								m_vclTile2RegionRectList;
};

class CDPTile2RegionBuildingBlockRect
{
public:
	CDPTile2RegionBuildingBlockRect(short sBuildingBlockId = 0) : m_sBuildingBlockId(sBuildingBlockId) {}

	short														m_sBuildingBlockId;
	vector< CDPTile2RegionLevelRect >							m_vclTile2RegionLevelRectList;
};

class CDPProvProduct :
	public CBaseObj
{
public:
	CDPProvProduct() : m_bDbSwitching(false) {}
	virtual ~CDPProvProduct() {}

	bool Initialize(string strProductName, shared_ptr< CDPDBConnectionPool > spclDBConnectionPool);
	bool GetUpdateRegionByTile(BUILDING_BLOCK_ID enBuildingBlockID, uint32_t uiPackedTileID, vector< string > &vstrUpdateRegionList);

public:
	shared_ptr< CDPDBConnectionPool >							m_spclDBConnectionPool;
	string														m_strProductName;
	CDPConnRec													m_clDatabase;
	volatile bool													m_bDbSwitching;
	class CDPUpdateRegionInfo{}; // temporary codes by WXL
	vector< CDPTile2RegionBuildingBlockRect >					m_vclTile2RegionTable;
	vector< CDPUpdateRegionInfo >								m_vclUpdateRegionList;

};

