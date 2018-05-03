
#include "RPCommon.hpp"
#include "RPRCTermSearch.hpp"
#include "RPRCSectionDirector.hpp"

CRPRCSectionDirector::CRPRCSectionDirector()
{
}


CRPRCSectionDirector::~CRPRCSectionDirector()
{
}


bool CRPRCSectionDirector::StartCalculateSection()
{
	auto spclTermSearch = std::make_shared<CRPRCTermSearch>();
	spclTermSearch->Do();

	return false;
}
