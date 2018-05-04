#include "DPFacade.hpp"
#include "DPCommon.hpp"
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
	return false;
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
	if (!spclProvFolderProduct)
	{
		return false;
	}

	if (!spclDPProvProduct->GetUpdateRegionByTile(enBuildingBlockID, uiPackedTileID, vstrUpdateRegionList))
	{
		return false;
	}

	return true;
}

