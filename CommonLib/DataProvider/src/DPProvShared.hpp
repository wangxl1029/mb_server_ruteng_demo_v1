#pragma once
class CDPMetaDataSubLevel
{
public:
	CDPMetaDataSubLevel() : m_uiMaxScale(0) {}

	uint														m_uiMaxScale;
	vector< uint >												m_vuiDetailLevelScale;
};

inline bool operator<(const CDPMetaDataSubLevel &l, const CDPMetaDataSubLevel &r)
{
	return l.m_uiMaxScale < r.m_uiMaxScale;
}

class CDPMetaDataLevel
{
public:
	CDPMetaDataLevel() : m_iLevelNumber(NDS_LEVEL_MAX), m_iCoordShift(0), m_uiMaxScale(0) {}

	int															m_iLevelNumber;
	int															m_iCoordShift;
	uint														m_uiMaxScale;
	vector< CDPMetaDataSubLevel >								m_vclSubLevelList;
};

inline bool operator<(const CDPMetaDataLevel &l, const CDPMetaDataLevel &r)
{
	return l.m_uiMaxScale < r.m_uiMaxScale;
}

class CDPMetaDataBuildingBlockLevel
{
public:
	CDPMetaDataBuildingBlockLevel() : m_iBuildingBlockId(BUILDING_BLOCK_ID_MAX)
	{
		for (size_t i = 0; i < sizeof(m_aucLevelIndexToLevelRec) / sizeof(m_aucLevelIndexToLevelRec[0]); ++i) {
			m_aucLevelIndexToLevelRec[i] = NDS_LEVEL_MAX;
		}
	}

	~CDPMetaDataBuildingBlockLevel() {}

	int															m_iBuildingBlockId;
	uchar														m_aucLevelIndexToLevelRec[NDS_LEVEL_MAX];	//	数组下标为Level, 数值为m_vclMetadataTable的数组下标
	vector< CDPMetaDataLevel >									m_vclLevelList;
};

class CDPProvShared :
	public CDPProv
{
public:
	CDPProvShared() : m_bDbSwitching(false) {}
	virtual ~CDPProvShared() {}

	RESULT Initialize(string strProductName, string strUpdateRegion, SmartPointer< CDPDBConnectionPool > spclDBConnectionPool);
	RESULT GetUpperLevel(int iLevel, BUILDING_BLOCK_ID enBuildingBlockID, int &iUpperLevel);
	RESULT GetDownLevel(int iLevel, BUILDING_BLOCK_ID enBuildingBlockID, int &iDownLevel);
	RESULT GetCoordShift(int iLevel, BUILDING_BLOCK_ID enBuildingBlockID, uint &uiCoordShift);
	RESULT GetLevelByScale(uint uiScale, BUILDING_BLOCK_ID enBuildingBlockID, int &iLevel, uint &uiSubLevel, uint &uiDetailLevel);
	RESULT GetLevelList(BUILDING_BLOCK_ID enBuildingBlockID, vector<int> &viLevelList);
	RESULT GetSubLevelCount(int iLevel, BUILDING_BLOCK_ID enBuildingBlockID, uint &uiSubLevelCount);
	RESULT GetDetailLevelCount(int iLevel, BUILDING_BLOCK_ID enBuildingBlockID, uint uiSubLevel, uint &uiDetailLevelCount);

	RESULT ReadLevelMetadata();

public:
	SmartPointer< CDPDBConnectionPool >							m_spclDBConnectionPool;
	string														m_strProductName;
	string														m_strUpdateRegion;
	CDPConnRec													m_clDatabase;

	CDPMetaDataBuildingBlockLevel								m_avclMetadataTable[DP_BUILDING_BLOCK_COUNT];

	volatile bool												m_bDbSwitching;
};

