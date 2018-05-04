#include <string>
#include "CFBaseObj.hpp"
#include "RPCommon.hpp"
#include "RPRCTermSearch.hpp"

CRPRCTermSearch::CRPRCTermSearch(CRPWayPoint &clWayPoint, int iLevel, std::shared_ptr< CDPFacade > spclDataProvider)
: m_clWayPoint(clWayPoint), m_iLevel(iLevel), m_spclDataProvider(spclDataProvider)
{
}

CRPRCTermSearch::~CRPRCTermSearch()
{
}

bool CRPRCTermSearch::Do()
{

	std::vector< std::string >	vstrUpdateRegion;

	uint32_t	uiTileNo = 0;
	CDPCommon::CoordToTileNo(m_iLevel, m_clWayPoint.m_clCoord.x, m_clWayPoint.m_clCoord.y, uiTileNo);

	return false;
}
