#include "DPFacade.hpp"
#include "DPCommon.hpp"

class CDPFacadeImpl : public CDPFacade
{
public:
	CDPFacadeImpl();
	virtual ~CDPFacadeImpl();

	virtual bool Initialize();
	virtual bool GetUpdateRegionByTile(BUILDING_BLOCK_ID enBuildingBlockID, uint32_t uiPackedTileID, std::vector< std::string > &vstrUpdateRegionList);

public:
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
	return false;
}