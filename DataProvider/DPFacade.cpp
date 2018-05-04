#include "DPFacade.hpp"


class CDPFacadeImpl : public CDPFacade
{
public:
	CDPFacadeImpl();
	virtual ~CDPFacadeImpl();

	virtual bool Initialize();

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


CDPFacadeImpl::CDPFacadeImpl() //: m_bDbSwitching(false)
{
}

CDPFacadeImpl::~CDPFacadeImpl()
{
}

bool CDPFacadeImpl::Initialize()
{
	return false;
}
