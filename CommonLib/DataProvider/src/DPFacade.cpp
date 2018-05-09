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
	virtual RESULT GetUpdateRegionByTile(BUILDING_BLOCK_ID enBuildingBlockID, uint uiPackedTileID, vector< string > &vstrUpdateRegionList);

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
