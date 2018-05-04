
#include "RPCommon.hpp"
#include "CFBaseObj.hpp"
#include "RPRouteCalcRequest.hpp"
#include "RPRCTermSearch.hpp"
#include "RPRCSectionDirector.hpp"

CRPRCSectionDirector::CRPRCSectionDirector()
{
}


CRPRCSectionDirector::~CRPRCSectionDirector()
{
}

bool CRPRCSectionDirector::Initialize(std::shared_ptr< CDPFacade > spclDataProvider)
{
	m_spclDataProvider = spclDataProvider;
	return true;
}

bool CRPRCSectionDirector::StartCalculateSection(CRPRouteCalcRequest &clRequest)
{
	auto spclTermSearch = std::make_shared<CRPRCTermSearch>(m_spclDataProvider);
	spclTermSearch->Do();

	return false;
}
