#pragma once

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

class CTileContentKey
{
public:
	CTileContentKey(short sUpdateRegionId = 0, short sBuildingBlockId = 0, uchar ucLevelNumber = 0)
		: m_sUpdateRegionId(sUpdateRegionId), m_sBuildingBlockId(sBuildingBlockId), m_ucLevelNumber(ucLevelNumber) {}

	short													m_sUpdateRegionId;
	short													m_sBuildingBlockId;
	uchar													m_ucLevelNumber;
};

inline bool operator<(const CTileContentKey &l, const CTileContentKey &r)
{
	if (l.m_sUpdateRegionId < r.m_sUpdateRegionId) {
		return true;
	}
	else if (l.m_sUpdateRegionId > r.m_sUpdateRegionId) {
		return false;
	}
	else if (l.m_sBuildingBlockId < r.m_sBuildingBlockId) {
		return true;
	}
	else if (l.m_sBuildingBlockId > r.m_sBuildingBlockId) {
		return false;
	}
	else if (l.m_ucLevelNumber < r.m_ucLevelNumber) {
		return true;
	}
	else {
		return false;
	}
}

class CDPProvProduct : public CDPProv
{
public:
	CDPProvProduct() : m_bDbSwitching(false) {}
	virtual ~CDPProvProduct() {}

	RESULT Initialize(string strProductName, SmartPointer< CDPDBConnectionPool > spclDBConnectionPool);

	RESULT SwitchDbStart();
	RESULT WaitForCanSwitchDb();
	RESULT SwitchDbEnd();

	RESULT GetUpdateRegionList(vector< CDPUpdateRegionInfo > &vclUpdateRegionList);
	RESULT GetUpdateRegionByTile(BUILDING_BLOCK_ID enBuildingBlockID, uint uiPackedTileID, vector< string > &vstrUpdateRegionList);
	RESULT GetGatewayByID(uint uiGatewayID, vector< CDPGatewayInfo > &vclGatewayList);

public:
	SmartPointer< CDPDBConnectionPool >							m_spclDBConnectionPool;
	string														m_strProductName;
	CCFMutex													m_clMutex;
	CDPConnRec													m_clDatabase;
	volatile bool												m_bDbSwitching;

	vector< CDPTile2RegionBuildingBlockRect >					m_vclTile2RegionTable;
	vector< CDPUpdateRegionInfo >								m_vclUpdateRegionList;

	CCFSimpleCache< CTileContentKey, CSL_RecTileContentIndexTable >	mclTileContentIndexCache;
};
