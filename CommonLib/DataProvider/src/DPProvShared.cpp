#include	"InnerCommon.hpp"
#include	"DPCommon.hpp"
#include	"DPDBConnectionPool.hpp"
#include	"DPData.hpp"
#include	"DPDataShared.hpp"
#include	"DPProv.hpp"
#include	"DPProvShared.hpp"

#define		DETAIL_LEVEL_COUNT		(3)
uint	s_aScaleList[][4] = {
	//	level	sublevel	detaillevel		maxscale
	{ 13, 0, 0, 10000 },	//	2
	{ 13, 1, 0, 31250 },	//	1.2
	{ 13, 2, 0, 54000 },

	{ 11, 0, 0, 93312 },	//	1.2
	{ 11, 1, 0, 161243 },
	{ 11, 2, 0, 270000 },

	{ 9, 0, 0, 432436 },	//	1.17
	{ 9, 1, 0, 692594 },
	{ 9, 2, 0, 1077000 },

	{ 7, 0, 0, 1861056 },	//	1.2
	{ 7, 1, 0, 3215905 },
	{ 7, 2, 0, 5380000 },

	{ 5, 0, 0, 9066155 },	//	1.2
	{ 5, 1, 0, 15277913 },
	{ 5, 2, 0, 25000000 },

	{ 3, 0, 0, 125000000 },	//	1.2
};

RESULT CDPProvShared::Initialize(string strProductName, string strUpdateRegion, SmartPointer< CDPDBConnectionPool > spclDBConnectionPool)
{
	m_spclDBConnectionPool = spclDBConnectionPool;
	m_strProductName = strProductName;
	m_strUpdateRegion = strUpdateRegion;

	if (m_spclDBConnectionPool == NULL) {
		ERR("");
		return FAILURE;
	}

	if (SUCCESS != m_clDatabase.Connect(m_spclDBConnectionPool, strProductName.c_str(), strUpdateRegion.c_str(), DPDB_SHARED)) {
		ERR("");
		return FAILURE;
	}

	if (SUCCESS != ReadLevelMetadata()) {
		ERR("");
		return FAILURE;
	}

	return SUCCESS;
}

RESULT CDPProvShared::GetUpperLevel(int iLevel, BUILDING_BLOCK_ID enBuildingBlockID, int &iUpperLevel)
{
	CDPMetaDataBuildingBlockLevel	&clLevelMetaData = m_avclMetadataTable[DP_BUILDING_BLOCK_ID_TO_NO(enBuildingBlockID)];
	size_t							uiLevelRecNo = clLevelMetaData.m_aucLevelIndexToLevelRec[iLevel];
	if (uiLevelRecNo + 1 >= clLevelMetaData.m_vclLevelList.size()) {
		ERR("");
		return FAILURE;
	}
	iUpperLevel = clLevelMetaData.m_vclLevelList[uiLevelRecNo + 1].m_iLevelNumber;

	return SUCCESS;
}

RESULT CDPProvShared::GetDownLevel(int iLevel, BUILDING_BLOCK_ID enBuildingBlockID, int &iDownLevel)
{
	CDPMetaDataBuildingBlockLevel	&clLevelMetaData = m_avclMetadataTable[DP_BUILDING_BLOCK_ID_TO_NO(enBuildingBlockID)];
	size_t							uiLevelRecNo = clLevelMetaData.m_aucLevelIndexToLevelRec[iLevel];
	if (uiLevelRecNo <= 0 || uiLevelRecNo >= clLevelMetaData.m_vclLevelList.size()) {
		ERR("");
		return FAILURE;
	}
	iDownLevel = clLevelMetaData.m_vclLevelList[uiLevelRecNo - 1].m_iLevelNumber;

	return SUCCESS;
}

RESULT CDPProvShared::GetCoordShift(int iLevel, BUILDING_BLOCK_ID enBuildingBlockID, uint &uiCoordShift)
{
	size_t				uiBuildingBlockNo = DP_BUILDING_BLOCK_ID_TO_NO(enBuildingBlockID);
	if (uiBuildingBlockNo >= DP_BUILDING_BLOCK_COUNT) {
		ERR("");
		return FAILURE;
	}
	CDPMetaDataBuildingBlockLevel	&clLevelMetaData = m_avclMetadataTable[uiBuildingBlockNo];
	if (iLevel >= NDS_LEVEL_MAX) {
		ERR("");
		return FAILURE;
	}
	size_t				uiLevelRecNo = clLevelMetaData.m_aucLevelIndexToLevelRec[iLevel];
	size_t				uiMetadataTableSize = clLevelMetaData.m_vclLevelList.size();
	if (uiLevelRecNo >= uiMetadataTableSize) {
		ERR("");
		return FAILURE;
	}
	uiCoordShift = clLevelMetaData.m_vclLevelList[uiLevelRecNo].m_iCoordShift;

	return SUCCESS;
}

RESULT CDPProvShared::GetLevelByScale(uint uiScale, BUILDING_BLOCK_ID enBuildingBlockID, int &iLevel, uint &uiSubLevel, uint &uiDetailLevel)
{
	size_t				uiBuildingBlockNo = DP_BUILDING_BLOCK_ID_TO_NO(enBuildingBlockID);
	if (uiBuildingBlockNo >= DP_BUILDING_BLOCK_COUNT) {
		ERR("");
		return FAILURE;
	}
	CDPMetaDataBuildingBlockLevel	&clMetaData = m_avclMetadataTable[uiBuildingBlockNo];

	iLevel = NDS_LEVEL_MAX;
	uiSubLevel = NDS_SUB_LEVEL_MAX;
	uiDetailLevel = NDS_DETAIL_LEVEL_MAX;

	//	find level
	CDPMetaDataLevel				clFindLevel;
	clFindLevel.m_uiMaxScale = uiScale;
	vector< CDPMetaDataLevel >::iterator	itFindLevel = lower_bound(clMetaData.m_vclLevelList.begin(),
		clMetaData.m_vclLevelList.end(),
		clFindLevel);
	if (itFindLevel == clMetaData.m_vclLevelList.end()) {
		return SUCCESS;
	}

	iLevel = itFindLevel->m_iLevelNumber;

	//	find sublevel
	CDPMetaDataLevel	&clLevel = *itFindLevel;
	CDPMetaDataSubLevel	clFindSubLevel;
	clFindSubLevel.m_uiMaxScale = uiScale;
	vector< CDPMetaDataSubLevel >::iterator	itFindSubLevel = lower_bound(clLevel.m_vclSubLevelList.begin(),
		clLevel.m_vclSubLevelList.end(),
		clFindSubLevel);
	if (itFindSubLevel == clLevel.m_vclSubLevelList.end()) {
		return SUCCESS;
	}

	uiSubLevel = itFindSubLevel - clLevel.m_vclSubLevelList.begin();

	//	find detaillevel
	CDPMetaDataSubLevel	&clSubLevel = *itFindSubLevel;
	vector< uint >::iterator	itFindDetailLevel = lower_bound(clSubLevel.m_vuiDetailLevelScale.begin(),
		clSubLevel.m_vuiDetailLevelScale.end(),
		uiScale);
	if (itFindDetailLevel == clSubLevel.m_vuiDetailLevelScale.end()) {
		return SUCCESS;
	}

	uiDetailLevel = itFindDetailLevel - clSubLevel.m_vuiDetailLevelScale.begin();

	return SUCCESS;
}

RESULT CDPProvShared::GetLevelList(BUILDING_BLOCK_ID enBuildingBlockID, vector<int> &viLevelList)
{
	CDPMetaDataBuildingBlockLevel		&clLevelMetaData = m_avclMetadataTable[DP_BUILDING_BLOCK_ID_TO_NO(enBuildingBlockID)];

	viLevelList.resize(clLevelMetaData.m_vclLevelList.size());
	for (size_t i = 0; i < clLevelMetaData.m_vclLevelList.size(); ++i) {
		viLevelList[i] = clLevelMetaData.m_vclLevelList[clLevelMetaData.m_vclLevelList.size() - i - 1].m_iLevelNumber;
	}
	return SUCCESS;
}

RESULT CDPProvShared::GetSubLevelCount(int iLevel, BUILDING_BLOCK_ID enBuildingBlockID, uint &uiSubLevelCount)
{
	size_t				uiBuildingBlockNo = DP_BUILDING_BLOCK_ID_TO_NO(enBuildingBlockID);
	if (uiBuildingBlockNo >= DP_BUILDING_BLOCK_COUNT) {
		ERR("");
		return FAILURE;
	}
	CDPMetaDataBuildingBlockLevel	&clLevelMetaData = m_avclMetadataTable[uiBuildingBlockNo];
	if (iLevel >= NDS_LEVEL_MAX) {
		ERR("");
		return FAILURE;
	}
	size_t				uiLevelRecNo = clLevelMetaData.m_aucLevelIndexToLevelRec[iLevel];
	size_t				uiMetadataTableSize = clLevelMetaData.m_vclLevelList.size();
	if (uiLevelRecNo >= uiMetadataTableSize) {
		ERR("");
		return FAILURE;
	}
	uiSubLevelCount = clLevelMetaData.m_vclLevelList[uiLevelRecNo].m_vclSubLevelList.size();

	return SUCCESS;
}

RESULT CDPProvShared::GetDetailLevelCount(int iLevel, BUILDING_BLOCK_ID enBuildingBlockID, uint uiSubLevel, uint &uiDetailLevelCount)
{
	size_t				uiBuildingBlockNo = DP_BUILDING_BLOCK_ID_TO_NO(enBuildingBlockID);
	if (uiBuildingBlockNo >= DP_BUILDING_BLOCK_COUNT) {
		ERR("");
		return FAILURE;
	}
	CDPMetaDataBuildingBlockLevel	&clLevelMetaData = m_avclMetadataTable[uiBuildingBlockNo];
	if (iLevel >= NDS_LEVEL_MAX) {
		ERR("");
		return FAILURE;
	}
	size_t				uiLevelRecNo = clLevelMetaData.m_aucLevelIndexToLevelRec[iLevel];
	size_t				uiMetadataTableSize = clLevelMetaData.m_vclLevelList.size();
	if (uiLevelRecNo >= uiMetadataTableSize) {
		ERR("");
		return FAILURE;
	}
	CDPMetaDataLevel	&clLevel = clLevelMetaData.m_vclLevelList[uiLevelRecNo];
	if (uiSubLevel >= clLevel.m_vclSubLevelList.size()) {
		ERR("");
		return FAILURE;
	}
	uiDetailLevelCount = clLevel.m_vclSubLevelList[uiSubLevel].m_vuiDetailLevelScale.size();
	return SUCCESS;
}

RESULT CDPProvShared::ReadLevelMetadata()
{
	CRCPtr< CSL_LevelMetadataTableTblIt >									spLevelMetadataTableTblIt;
	if (SUCCESS != SL_CreateTblIt(m_clDatabase.Get(), &spLevelMetadataTableTblIt)) {
		ERR("");
		return FAILURE;
	}

	if (SUCCESS != spLevelMetadataTableTblIt->Select()) {
		ERR("");
		return FAILURE;
	}
	for (spLevelMetadataTableTblIt->Begin(); !spLevelMetadataTableTblIt->IsEnd(); spLevelMetadataTableTblIt->Next()) {
		//	Read Database
		CSL_RecLevelMetadataTable	clSLLevelMetadataTableRec;
		if (SUCCESS != spLevelMetadataTableTblIt->CurRec(clSLLevelMetadataTableRec)) {
			ERR("");
			return FAILURE;
		}

		//	Get BuildingBlock
		uint	uiBuildingBlockNo = DP_BUILDING_BLOCK_ID_TO_NO((BUILDING_BLOCK_ID)clSLLevelMetadataTableRec.m_iBuildingBlockId);
		CDPMetaDataBuildingBlockLevel	&clBuildingBlock = m_avclMetadataTable[uiBuildingBlockNo];
		clBuildingBlock.m_iBuildingBlockId = clSLLevelMetadataTableRec.m_iBuildingBlockId;

#ifdef	NEU_DB
		if (clSLLevelMetadataTableRec.m_iLevelNumber == 14 && clSLLevelMetadataTableRec.m_iBuildingBlockId == 1) {
			continue;
		}
		//		if( clSLLevelMetadataTableRec.m_iLevelNumber == 3 && clSLLevelMetadataTableRec.m_iBuildingBlockId == 1 ) {
		//			continue;
		//		}
#endif
		//	Read Blob
		CLevelMetadataTable			clScaleDenominator;
		if (SUCCESS != CLevelMetadataTable_Reader().ReadAll(clSLLevelMetadataTableRec.m_clScaleDenominatorList.Get(),
			clSLLevelMetadataTableRec.m_clScaleDenominatorList.Size(),
			clScaleDenominator)) {
			ERR("");
			return FAILURE;
		}

		vector< uint >	&vuiSubLevelScales = clScaleDenominator.m_clScaleDenominatorList.m_vuiScaleSublevels;
		sort(vuiSubLevelScales.begin(), vuiSubLevelScales.end());

		//	NEU DB 最后一个Scale，有的与Level的Max相同，有的Level的Max小
		if (vuiSubLevelScales.size() == 0 || vuiSubLevelScales.front() != clSLLevelMetadataTableRec.m_iMinScaleDenominator) {
			vuiSubLevelScales.insert(vuiSubLevelScales.begin(), clSLLevelMetadataTableRec.m_iMinScaleDenominator);
		}
		if (vuiSubLevelScales.back() < clSLLevelMetadataTableRec.m_iMaxScaleDenominator) {
			vuiSubLevelScales.push_back(clSLLevelMetadataTableRec.m_iMaxScaleDenominator);
		}

		//	Level to push_back
		CDPMetaDataLevel	clLevel;
		clLevel.m_iLevelNumber = clSLLevelMetadataTableRec.m_iLevelNumber;
		clLevel.m_iCoordShift = clSLLevelMetadataTableRec.m_iCoordShift;
		clLevel.m_uiMaxScale = clSLLevelMetadataTableRec.m_iMaxScaleDenominator;
		for (size_t k = 1; k < vuiSubLevelScales.size(); ++k) {
			uint	uiMinScale = vuiSubLevelScales[k - 1];
			uint	uiMaxScale = vuiSubLevelScales[k];
			CDPMetaDataSubLevel	clSubLevel;
			clSubLevel.m_uiMaxScale = uiMaxScale;
			float	f = sqrt(((float)uiMaxScale) / uiMinScale);
			uint	uiScale = uiMinScale;
			for (size_t l = 0; l < DETAIL_LEVEL_COUNT; ++l) {
				uiScale *= f;
				clSubLevel.m_vuiDetailLevelScale.push_back(uiScale);
			}
			clLevel.m_vclSubLevelList.push_back(clSubLevel);
		}

#ifdef	NEU_DB
		if (clSLLevelMetadataTableRec.m_iBuildingBlockId == BUILDING_BLOCK_ID_BASIC_MAP_DISPLAY) {
			clLevel.m_vclSubLevelList.clear();
			for (size_t i = 0; i < sizeof(s_aScaleList) / sizeof(s_aScaleList[0]); ++i) {
				if (s_aScaleList[i][0] == clLevel.m_iLevelNumber) {
					if (clLevel.m_vclSubLevelList.size() == s_aScaleList[i][1]) {
						clLevel.m_vclSubLevelList.push_back(CDPMetaDataSubLevel());
					}
					clLevel.m_vclSubLevelList.back().m_vuiDetailLevelScale.push_back(s_aScaleList[i][3]);
					clLevel.m_vclSubLevelList.back().m_uiMaxScale = s_aScaleList[i][3];
				}
			}
			clLevel.m_uiMaxScale = clLevel.m_vclSubLevelList.back().m_vuiDetailLevelScale.back();
		}
#endif

		m_avclMetadataTable[uiBuildingBlockNo].m_vclLevelList.push_back(clLevel);
	}

	//	Sort and Level Index
	for (size_t i = 0; i < DP_BUILDING_BLOCK_COUNT; ++i) {
		CDPMetaDataBuildingBlockLevel		&clCurBuildingBlock = m_avclMetadataTable[i];
		sort(clCurBuildingBlock.m_vclLevelList.begin(), clCurBuildingBlock.m_vclLevelList.end());
		for (size_t j = 0; j < clCurBuildingBlock.m_vclLevelList.size(); ++j) {
			CDPMetaDataLevel	&clCurLevel = clCurBuildingBlock.m_vclLevelList[j];
			clCurBuildingBlock.m_aucLevelIndexToLevelRec[clCurLevel.m_iLevelNumber] = j;
		}
	}

#ifdef	NEU_DB
	for (size_t i = 0; i < DP_BUILDING_BLOCK_COUNT; ++i) {
		CDPMetaDataBuildingBlockLevel		&clCurBuildingBlock = m_avclMetadataTable[i];
		for (size_t j = 0; j < clCurBuildingBlock.m_vclLevelList.size(); ++j) {
			CDPMetaDataLevel	&clCurLevel = clCurBuildingBlock.m_vclLevelList[j];
			clCurLevel.m_uiMaxScale /= 3;
			for (size_t k = 0; k < clCurLevel.m_vclSubLevelList.size(); k++) {
				CDPMetaDataSubLevel	&clSubLevel = clCurLevel.m_vclSubLevelList[k];
				clSubLevel.m_uiMaxScale /= 3;
				for (size_t l = 0; l < clSubLevel.m_vuiDetailLevelScale.size(); ++l) {
					clSubLevel.m_vuiDetailLevelScale[l] /= 3;
				}
			}
		}
	}
#endif

	//	for 3DObj
	size_t	uiBuildingBlock3DObjNo = DP_BUILDING_BLOCK_ID_TO_NO(BUILDING_BLOCK_ID_OBJECTS_3D);
	CDPMetaDataBuildingBlockLevel		&cl3DOBuildingBlock = m_avclMetadataTable[uiBuildingBlock3DObjNo];
	cl3DOBuildingBlock.m_iBuildingBlockId = BUILDING_BLOCK_ID_OBJECTS_3D;
	cl3DOBuildingBlock.m_aucLevelIndexToLevelRec[14] = 0;	//	14层第0条记录

	const uint	ui3DObjMaxScale = 18000;

	CDPMetaDataLevel	cl3DOLevel;
	cl3DOLevel.m_iLevelNumber = 14;
	cl3DOLevel.m_iCoordShift = 0;
	cl3DOLevel.m_uiMaxScale = ui3DObjMaxScale;

	CDPMetaDataSubLevel	clSubLevel;
	clSubLevel.m_uiMaxScale = ui3DObjMaxScale;

	clSubLevel.m_vuiDetailLevelScale.push_back(ui3DObjMaxScale);

	cl3DOLevel.m_vclSubLevelList.push_back(clSubLevel);

	cl3DOBuildingBlock.m_vclLevelList.push_back(cl3DOLevel);

	return SUCCESS;
}
