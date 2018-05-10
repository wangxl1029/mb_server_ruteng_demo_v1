#pragma once
class CDPFacade :
	public CBaseObj
{
public:
	CDPFacade() {};
	virtual ~CDPFacade() {};

	static SmartPointer< CDPFacade > Create();

	virtual RESULT Initialize() = 0;

	//virtual RESULT GetDBRootPath(string &str) = 0;
	//virtual RESULT GetUpperLevel(int iLevel, BUILDING_BLOCK_ID enBuildingBlockID, int &iUpperLevel) = 0;
	//virtual RESULT GetDownLevel(int iLevel, BUILDING_BLOCK_ID enBuildingBlockID, int &iDownLevel) = 0;
	virtual RESULT GetCoordShift(int iLevel, BUILDING_BLOCK_ID enBuildingBlockID, uint &uiCoordShift) = 0;
	//virtual RESULT GetLevelByScale(uint uiScale, BUILDING_BLOCK_ID enBuildingBlockID, int &iMinLevel, uint &uiSubLevel, uint &uiDetailLevel) = 0;
	//virtual RESULT GetLevelList(BUILDING_BLOCK_ID enBuildingBlockID, vector<int> &viLevelList) = 0;
	//virtual RESULT GetSubLevelCount(int iLevel, BUILDING_BLOCK_ID enBuildingBlockID, uint &uiSubLevelCount) = 0;
	//virtual RESULT GetDetailLevelCount(int iLevel, BUILDING_BLOCK_ID enBuildingBlockID, uint uiSubLevel, uint &uiDetailLevelCount) = 0;
	//virtual RESULT GetBmdData(string strUpdateRegion, int iLevel, uint uiTileNo, short sVersion, SmartPointer< CDPDataBmd > &spclDataBmd) = 0;
	virtual RESULT GetRoutingTileData(string strUpdateRegion, int iLevel, uint uiTileNo, short sVersion, SmartPointer< CDPDataRoutingTile > &spclDataRoutingTile) = 0;

	virtual RESULT GetUpdateRegionByTile(BUILDING_BLOCK_ID enBuildingBlockID, uint uiPackedTileID, vector< string > &vstrUpdateRegionList) = 0;

};

