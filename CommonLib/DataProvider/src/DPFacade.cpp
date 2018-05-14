#include	"InnerCommon.hpp"
#include	"DPCommon.hpp"
#include	"DPDBConnectionPool.hpp"
#include	"DPData.hpp"
//#include	"DPDataRoot.hpp"
#include	"DPDataShared.hpp"
#include	"DPDataProduct.hpp"
// ignored headers
#include	"DPProv.hpp"
#include	"DPProvRoot.hpp"
#include	"DPProvShared.hpp"
#include	"DPProvProduct.hpp"
// ignored headers
#include	"DPProvRouting.hpp"
// ignored headers
#include	"DPProvFolderUpdateRegion.hpp"
#include	"DPProvFolderProduct.hpp"
//#include	"DPProvDrawParameter.h"
#include	"DPProvFolderRoot.hpp"
#include	"DPFacade.hpp"

class CDPFacadeImpl : public CDPFacade
{
public:
	CDPFacadeImpl();
	virtual ~CDPFacadeImpl();

	virtual RESULT Initialize();

	//virtual RESULT GetDBRootPath(string &str);
	//virtual RESULT GetUpperLevel(int iLevel, BUILDING_BLOCK_ID enBuildingBlockID, int &iUpperLevel);
	//virtual RESULT GetDownLevel(int iLevel, BUILDING_BLOCK_ID enBuildingBlockID, int &iDownLevel);
	virtual RESULT GetCoordShift(int iLevel, BUILDING_BLOCK_ID enBuildingBlockID, uint &uiCoordShift);
	//virtual RESULT GetLevelByScale(uint uiScale, BUILDING_BLOCK_ID enBuildingBlockID, int &iMinLevel, uint &uiSubLevel, uint &uiDetailLevel);
	virtual RESULT GetLevelList(BUILDING_BLOCK_ID enBuildingBlockID, vector<int> &viLevelList);
	//virtual RESULT GetSubLevelCount(int iLevel, BUILDING_BLOCK_ID enBuildingBlockID, uint &uiSubLevelCount);
	//virtual RESULT GetDetailLevelCount(int iLevel, BUILDING_BLOCK_ID enBuildingBlockID, uint uiSubLevel, uint &uiDetailLevelCount);
	//virtual RESULT GetBmdData(string strUpdateRegion, int iLevel, uint uiTileNo, short sVersion, SmartPointer< CDPDataBmd > &spclDataBmd);
	virtual RESULT GetRoutingTileData(string strUpdateRegion, int iLevel, uint uiTileNo, short sVersion, SmartPointer< CDPDataRoutingTile > &spclDataRoutingTile);
	//virtual RESULT GetRoutingAuxData(string strUpdateRegion, int iLevel, uint uiTileNo, short sVersion, SmartPointer< CDPDataRoutingAuxTile > &spclDataRoutingAuxTile);
	//virtual RESULT GetNamedObjectData(string strUpdateRegion, int iNamedObjectId, short sVersion, SmartPointer< CDPDataNamedObject > &spclDataNamedObject);
	//virtual RESULT GetNamedObjectDataList(string strUpdateRegion, vector< int > &viNamedObjectIdList, short sVersion, vector< SmartPointer< CDPDataNamedObject > > &vspclDataNamedObjectList);
	//virtual RESULT Get3DObjSpatialSubTree(string strUpdateRegion, int iSubTreeId, short sVersion, SmartPointer< CDP3DObjSpatialSubTree > &spclSubTree);
	//virtual RESULT Get3DObjBodyGeometry(string strUpdateRegion, int iBodyGeometryId, short sVersion, SmartPointer< CDP3DObjBodyGeometry > &spclBodyGeometry);
	//virtual RESULT Get3DObjBodyGeometryList(string strUpdateRegion, vector< int> &viBodyGeometryIdList, short sVersion, vector< SmartPointer< CDP3DObjBodyGeometry > > &vspclBodyGeometryList);
	//virtual RESULT Get3DObjTextureMap(string strUpdateRegion, int iTextureMapId, short sVersion, SmartPointer< CDP3DObjTextureMap > &spclTextureMap);
	//virtual RESULT Get3DObjTextureMapList(string strUpdateRegion, vector< int > &viTextureMapIdList, short sVersion, vector< SmartPointer< CDP3DObjTextureMap > > &vspclTextureMapList);
	virtual RESULT GetUpdateRegionByTile(BUILDING_BLOCK_ID enBuildingBlockID, uint uiPackedTileID, vector< string > &vstrUpdateRegionList);
	virtual RESULT GetGatewayByID(uint uiGatewayID, vector< CDPGatewayInfo > &vclGatewayList);

public:
public:
	string														m_strProductName;
	SmartPointer< CDPDBConnectionPool >							m_spclDBConnectionPool;
	SmartPointer< CDPProvFolderRoot >							m_spclDPProvFolderRoot;
	SmartPointer< CDPProvFolderRoot >							m_spclDPProvFolderRootOld;
	volatile bool												m_bDbSwitching;
};

SmartPointer< CDPFacade > CDPFacade::Create()
{
	SmartPointer< CDPFacadeImpl >	spclFacadeImpl;
	if( !spclFacadeImpl.Create() ) {
		ERR( "" );
		return SmartPointer< CDPFacade >();
	}
	return SmartPointer< CDPFacade >( spclFacadeImpl );
}

CDPFacadeImpl::CDPFacadeImpl() : m_bDbSwitching(false)
{
}

CDPFacadeImpl::~CDPFacadeImpl()
{
}

RESULT CDPFacadeImpl::Initialize()
{
	m_strProductName = PRODUCT_NAME;

	if (m_spclDBConnectionPool == NULL) {
		if (!m_spclDBConnectionPool.Create()) {
			ERR("");
			return FAILURE;
		}
		if (SUCCESS != m_spclDBConnectionPool->Initialize(DP_GetRootDirName())) {
			ERR("");
			return FAILURE;
		}
	}

	if (m_spclDPProvFolderRoot == NULL) {
		if (!m_spclDPProvFolderRoot.Create()) {
			ERR("");
			return FAILURE;
		}
		if (SUCCESS != m_spclDPProvFolderRoot->Initialize(m_spclDBConnectionPool)) {
			ERR("");
			return FAILURE;
		}
	}

	return SUCCESS;
}

RESULT CDPFacadeImpl::GetCoordShift(int iLevel, BUILDING_BLOCK_ID enBuildingBlockID, uint &uiCoordShift)
{
	SmartPointer< CDPProvFolderProduct >	spclProvFolderProduct;
	if (SUCCESS != m_spclDPProvFolderRoot->GetFolderProduct(m_strProductName, spclProvFolderProduct)) {
		ERR("");
		return FAILURE;
	}

	SmartPointer< CDPProvProduct >			spclDPProvProduct;
	if (SUCCESS != spclProvFolderProduct->GetProvProduct(spclDPProvProduct)) {
		ERR("");
		return FAILURE;
	}

	vector< CDPUpdateRegionInfo >	vclUpdateRegionList;
	if (SUCCESS != spclDPProvProduct->GetUpdateRegionList(vclUpdateRegionList)) {
		ERR("");
		return FAILURE;
	}

	SmartPointer< CDPProvFolderUpdateRegion >	spclProvFolderUpdateRegion;
	if (SUCCESS != spclProvFolderProduct->GetProvFolderUpdateRegion(vclUpdateRegionList[0].m_strUpdateRegionName, spclProvFolderUpdateRegion)) {
		ERR("");
		return FAILURE;
	}

	SmartPointer< CDPProvShared >	spclProvShared;
	if (SUCCESS != spclProvFolderUpdateRegion->GetProvShared(spclProvShared)) {
		ERR("");
		return FAILURE;
	}

	if (SUCCESS != spclProvShared->GetCoordShift(iLevel, enBuildingBlockID, uiCoordShift)) {
		ERR("");
		return FAILURE;
	}

	return SUCCESS;
}

RESULT CDPFacadeImpl::GetLevelList(BUILDING_BLOCK_ID enBuildingBlockID, vector<int> &viLevelList)
{
	SmartPointer< CDPProvFolderProduct >	spclProvFolderProduct;
	if (SUCCESS != m_spclDPProvFolderRoot->GetFolderProduct(m_strProductName, spclProvFolderProduct)) {
		ERR("");
		return FAILURE;
	}

	SmartPointer< CDPProvProduct >			spclDPProvProduct;
	if (SUCCESS != spclProvFolderProduct->GetProvProduct(spclDPProvProduct)) {
		ERR("");
		return FAILURE;
	}

	vector< CDPUpdateRegionInfo >	vclUpdateRegionList;
	if (SUCCESS != spclDPProvProduct->GetUpdateRegionList(vclUpdateRegionList)) {
		ERR("");
		return FAILURE;
	}

	SmartPointer< CDPProvFolderUpdateRegion >	spclProvFolderUpdateRegion;
	if (SUCCESS != spclProvFolderProduct->GetProvFolderUpdateRegion(vclUpdateRegionList[0].m_strUpdateRegionName, spclProvFolderUpdateRegion)) {
		ERR("");
		return FAILURE;
	}

	SmartPointer< CDPProvShared >	spclProvShared;
	if (SUCCESS != spclProvFolderUpdateRegion->GetProvShared(spclProvShared)) {
		ERR("");
		return FAILURE;
	}

	if (SUCCESS != spclProvShared->GetLevelList(enBuildingBlockID, viLevelList)) {
		ERR("");
		return FAILURE;
	}

	return SUCCESS;
}

RESULT CDPFacadeImpl::GetRoutingTileData(string strUpdateRegion, int iLevel, uint uiTileNo, short sVersion, SmartPointer< CDPDataRoutingTile > &spclDataRoutingTile)
{
	if (m_bDbSwitching) {
		ERR("");
		return FAILURE;
	}

	SmartPointer< CDPProvFolderProduct >	spclProvFolderProduct;
	if (SUCCESS != m_spclDPProvFolderRoot->GetFolderProduct(m_strProductName, spclProvFolderProduct)) {
		ERR("");
		return FAILURE;
	}

	SmartPointer< CDPProvFolderUpdateRegion >	spclProvFolderUpdateRegion;
	if (SUCCESS != spclProvFolderProduct->GetProvFolderUpdateRegion(strUpdateRegion, spclProvFolderUpdateRegion)) {
		ERR("");
		return FAILURE;
	}

	SmartPointer< CDPProvShared >	spclProvShared;
	if (SUCCESS != spclProvFolderUpdateRegion->GetProvShared(spclProvShared)) {
		ERR("");
		return FAILURE;
	}

	uint		uiCoordShift = 0;
	if (SUCCESS != spclProvShared->GetCoordShift(iLevel, BUILDING_BLOCK_ID_BASIC_MAP_DISPLAY, uiCoordShift)) {
		ERR("");
		return FAILURE;
	}

	SmartPointer< CDPProvRouting >	spclProvRouting;
	if (SUCCESS != spclProvFolderUpdateRegion->GetProvRouting(spclProvRouting)) {
		ERR("");
		return FAILURE;
	}

	uint	uiPackedTileID = 0;
	if (SUCCESS != CDPCommon::TileNoToPackedTileID(iLevel, uiTileNo, uiPackedTileID)) {
		ERR("");
		return FAILURE;
	}

	if (SUCCESS != spclProvRouting->GetRoutingTile(uiPackedTileID, sVersion, spclDataRoutingTile, 31 - iLevel - uiCoordShift)) {
		ERR("");
		return FAILURE;
	}

	return SUCCESS;
}


RESULT CDPFacadeImpl::GetUpdateRegionByTile(BUILDING_BLOCK_ID enBuildingBlockID, uint uiPackedTileID, vector< string > &vstrUpdateRegionList)
{
	if (m_bDbSwitching) {
		ERR("");
		return FAILURE;
	}

	SmartPointer< CDPProvFolderProduct >	spclProvFolderProduct;
	if (SUCCESS != m_spclDPProvFolderRoot->GetFolderProduct(m_strProductName, spclProvFolderProduct)) {
		ERR("");
		return FAILURE;
	}

	SmartPointer< CDPProvProduct >			spclDPProvProduct;
	if (SUCCESS != spclProvFolderProduct->GetProvProduct(spclDPProvProduct)) {
		ERR("");
		return FAILURE;
	}

	if (SUCCESS != spclDPProvProduct->GetUpdateRegionByTile(enBuildingBlockID, uiPackedTileID, vstrUpdateRegionList)) {
		ERR("");
		return FAILURE;
	}

	return SUCCESS;
}

RESULT CDPFacadeImpl::GetGatewayByID(uint uiGatewayID, vector< CDPGatewayInfo > &vclGatewayList)
{
	if (m_bDbSwitching) {
		ERR("");
		return FAILURE;
	}

	SmartPointer< CDPProvFolderProduct >	spclProvFolderProduct;
	if (SUCCESS != m_spclDPProvFolderRoot->GetFolderProduct(m_strProductName, spclProvFolderProduct)) {
		ERR("");
		return FAILURE;
	}

	SmartPointer< CDPProvProduct >			spclDPProvProduct;
	if (SUCCESS != spclProvFolderProduct->GetProvProduct(spclDPProvProduct)) {
		ERR("");
		return FAILURE;
	}

	if (SUCCESS != spclDPProvProduct->GetGatewayByID(uiGatewayID, vclGatewayList)) {
		ERR("");
		return FAILURE;
	}

	return SUCCESS;
}
