
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


bool CRPRCSectionDirector::StartCalculateSection(CRPRouteCalcRequest &clRequest)
{
	auto spclTermSearch = std::make_shared<CRPRCTermSearch>();
	spclTermSearch->Do();

	return false;
}
