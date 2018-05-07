#include "DPFacade.hpp"
#include "DPCommon.hpp"
#include "DPDBConnectionPool.hpp"
#include "DPProvFolderRoot.hpp"

class CDPFacadeImpl : public CDPFacade
{
public:
	CDPFacadeImpl();
	virtual ~CDPFacadeImpl();

	virtual bool Initialize();
	virtual bool GetUpdateRegionByTile(BUILDING_BLOCK_ID enBuildingBlockID, uint32_t uiPackedTileID, std::vector< std::string > &vstrUpdateRegionList);

public:
	std::string													m_strProductName;
	std::shared_ptr< CDPDBConnectionPool >						m_spclDBConnectionPool;
	std::shared_ptr< CDPProvFolderRoot >						m_spclDPProvFolderRoot;
	std::shared_ptr< CDPProvFolderRoot >						m_spclDPProvFolderRootOld;
	volatile bool												m_bDbSwitching;
};

std::shared_ptr< CDPFacade > CDPFacade::Create()
{
	static std::shared_ptr< CDPFacadeImpl >	spclFacadeImpl = std::make_shared< CDPFacadeImpl >();
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
		m_spclDBConnectionPool = std::make_shared<CDPDBConnectionPool>();
		if (true != m_spclDBConnectionPool->Initialize(DP_GetRootDirName())) {
			//ERR("");
			return false;
		}
	}

	if (!m_spclDPProvFolderRoot)
	{
		m_spclDPProvFolderRoot = std::make_shared<CDPProvFolderRoot>();
		if (!m_spclDPProvFolderRoot->Initialize(m_spclDBConnectionPool))
		{
			return false;
		}
	}

	return true;
}

bool CDPFacadeImpl::GetUpdateRegionByTile(BUILDING_BLOCK_ID enBuildingBlockID, uint32_t uiPackedTileID, std::vector< std::string > &vstrUpdateRegionList)
{
	if (m_bDbSwitching) {
		//ERR("");
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

