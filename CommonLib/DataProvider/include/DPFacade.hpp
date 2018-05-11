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
	//virtual RESULT GetRoutingAuxData(string strUpdateRegion, int iLevel, uint uiTileNo, short sVersion, SmartPointer< CDPDataRoutingAuxTile > &spclDataRoutingAuxTile) = 0;
	//virtual RESULT GetNamedObjectData(string strUpdateRegion, int iNamedObjectId, short sVersion, SmartPointer< CDPDataNamedObject > &spclDataNamedObject) = 0;
	//virtual RESULT GetNamedObjectDataList(string strUpdateRegion, vector< int > &viNamedObjectIdList, short sVersion, vector< SmartPointer< CDPDataNamedObject > > &vspclDataNamedObjectList) = 0;
	//virtual RESULT Get3DObjSpatialSubTree(string strUpdateRegion, int iSubTreeId, short sVersion, SmartPointer< CDP3DObjSpatialSubTree > &spclSubTree) = 0;
	//virtual RESULT Get3DObjBodyGeometry(string strUpdateRegion, int iBodyGeometryId, short sVersion, SmartPointer< CDP3DObjBodyGeometry > &spclBodyGeometry) = 0;
	//virtual RESULT Get3DObjBodyGeometryList(string strUpdateRegion, vector< int> &viBodyGeometryIdList, short sVersion, vector< SmartPointer< CDP3DObjBodyGeometry > > &vspclBodyGeometryList) = 0;
	//virtual RESULT Get3DObjTextureMap(string strUpdateRegion, int iTextureMapId, short sVersion, SmartPointer< CDP3DObjTextureMap > &spclTextureMap) = 0;
	//virtual RESULT Get3DObjTextureMapList(string strUpdateRegion, vector< int > &viTextureMapIdList, short sVersion, vector< SmartPointer< CDP3DObjTextureMap > > &vspclTextureMapList) = 0;
	//virtual RESULT GetUpdateRegionList(vector< CDPUpdateRegionInfo > &vclUpdateRegionList) = 0;
	virtual RESULT GetUpdateRegionByTile(BUILDING_BLOCK_ID enBuildingBlockID, uint uiPackedTileID, vector< string > &vstrUpdateRegionList) = 0;
	virtual RESULT GetGatewayByID(uint uiGatewayID, vector< CDPGatewayInfo > &vclGatewayList) = 0;

};

