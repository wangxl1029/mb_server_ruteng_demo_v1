
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
	if (clRequest.m_spclWayPoints->Size() < 2)
	{
		return false;
	}

	m_clStartWayPoint = clRequest.m_spclWayPoints->At(0);
	m_clEndWayPoint = clRequest.m_spclWayPoints->At(1);

	auto spclTermSearch = std::make_shared<CRPRCTermSearch>(m_clStartWayPoint, 13, m_spclDataProvider);
	spclTermSearch->Do(m_spclDataProvider);

	return true;
}
