#include "InnerCommon.hpp"
#include "RPCommon.hpp"

#include "RPRouteCalcRequest.hpp"
#include "RPRCTermSearch.hpp"
#include "RPRCSectionDirector.hpp"

CRPRCSectionDirector::CRPRCSectionDirector()
{
}


CRPRCSectionDirector::~CRPRCSectionDirector()
{
}

RESULT CRPRCSectionDirector::Initialize(SmartPointer< CDPFacade > spclDataProvider)
{
	if (spclDataProvider == NULL) {
		ERR("");
		return FAILURE;
	}
	m_spclDataProvider = spclDataProvider;
	return SUCCESS;
}

RESULT CRPRCSectionDirector::StartCalculateSection(CRPRouteCalcRequest &clRequest)
{
	if (clRequest.m_spclWayPoints->Size() < 2)
	{
		ERR("");
		return FAILURE;
	}

	m_clStartWayPoint = clRequest.m_spclWayPoints->At(0);
	m_clEndWayPoint = clRequest.m_spclWayPoints->At(1);

	auto spclTermSearch = std::make_shared<CRPRCTermSearch>(m_clStartWayPoint, 13, m_spclDataProvider);
	spclTermSearch->Do(m_spclDataProvider);

	return true;
}
