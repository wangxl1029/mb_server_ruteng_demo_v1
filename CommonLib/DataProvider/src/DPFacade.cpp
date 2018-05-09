#include "InnerCommon.hpp"
#include "DPCommon.hpp"
#include "DPDBConnectionPool.hpp"
#include "DPData.hpp"
#include "DPDataProduct.hpp"
#include "DPProvProduct.hpp"
#include "DPProvFolderProduct.hpp"
#include "DPProvFolderRoot.hpp"
#include "DPFacade.hpp"
class CDPFacadeImpl : public CDPFacade
{
public:
	CDPFacadeImpl();
	virtual ~CDPFacadeImpl();

	virtual bool Initialize();
	virtual bool GetUpdateRegionByTile(BUILDING_BLOCK_ID enBuildingBlockID, uint32_t uiPackedTileID, vector< string > &vstrUpdateRegionList);

public:
	string													m_strProductName;
	shared_ptr< CDPDBConnectionPool >						m_spclDBConnectionPool;
	shared_ptr< CDPProvFolderRoot >						m_spclDPProvFolderRoot;
	shared_ptr< CDPProvFolderRoot >						m_spclDPProvFolderRootOld;
	volatile bool												m_bDbSwitching;
};

shared_ptr< CDPFacade > CDPFacade::Create()
{
	static shared_ptr< CDPFacadeImpl >	spclFacadeImpl = make_shared< CDPFacadeImpl >();
#if 0
	if (!spclFacadeImpl->Create()) {
		//ERR("");
		return SmartPointer< CDPFacade >();
	}
	return SmartPointer< CDPFacade >(spclFacadeImpl);
#endif
	return spclFacadeImpl;
}


CDPFacadeImpl::CDPFacadeImpl() : m_bDbSwitching(false)
{
}

CDPFacadeImpl::~CDPFacadeImpl()
{
}

bool CDPFacadeImpl::Initialize()
{
	m_strProductName = PRODUCT_NAME;

	if (nullptr == m_spclDBConnectionPool) {
		m_spclDBConnectionPool = make_shared<CDPDBConnectionPool>();
		if (true != m_spclDBConnectionPool->Initialize(DP_GetRootDirName())) {
			//ERR("");
			return false;
		}
	}

	if (!m_spclDPProvFolderRoot)
	{
		m_spclDPProvFolderRoot = make_shared<CDPProvFolderRoot>();
		if (!m_spclDPProvFolderRoot->Initialize(m_spclDBConnectionPool))
		{
			return false;
		}
	}

	return true;
}

bool CDPFacadeImpl::GetUpdateRegionByTile(BUILDING_BLOCK_ID enBuildingBlockID, uint32_t uiPackedTileID, vector< string > &vstrUpdateRegionList)
{
	if (m_bDbSwitching) {
		ERR("");
		return false;
	}

	auto spclProvFolderProduct = m_spclDPProvFolderRoot->GetFolderProduct(m_strProductName);
	if (!spclProvFolderProduct)
	{
		return false;
	}

	auto spclDPProvProduct = spclProvFolderProduct->GetProvProduct();
	if (!spclDPProvProduct)
	{
		return false;
	}

	if (!spclDPProvProduct->GetUpdateRegionByTile(enBuildingBlockID, uiPackedTileID, vstrUpdateRegionList))
	{
		return false;
	}

	return true;
}

