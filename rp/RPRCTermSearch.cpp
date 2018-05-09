#include "InnerCommon.hpp"
#include "RPCommon.hpp"
#include "RPRouteCalcRequest.hpp"
#include "RPRCTermSearch.hpp"

CRPRCTermSearch::CRPRCTermSearch(CRPWayPoint &clWayPoint, int iLevel, std::shared_ptr< CDPFacade > spclDataProvider)
: m_clWayPoint(clWayPoint), m_iLevel(iLevel), m_spclDataProvider(spclDataProvider)
{
}

CRPRCTermSearch::~CRPRCTermSearch()
{
}

bool CRPRCTermSearch::Do(std::shared_ptr< CDPFacade > spclDataProvider)
{

	uint32_t	uiTileNo = 0;
	CDPCommon::CoordToTileNo(m_iLevel, m_clWayPoint.m_clCoord.x, m_clWayPoint.m_clCoord.y, uiTileNo);

	//CDPCommon::TileNoToPackedTileID(m_iLevel, uiTileNo, clTileID.m_uiPackedTileID);
	uint32_t uiPackedTileID = 0;
	CDPCommon::TileNoToPackedTileID(m_iLevel, uiTileNo, uiPackedTileID);

	vector< string >	vstrUpdateRegion;
	if (true != spclDataProvider->GetUpdateRegionByTile(BUILDING_BLOCK_ID_ROUTING, uiPackedTileID/*clTileID.m_uiPackedTileID*/, vstrUpdateRegion)) {
		ERR("");
		return false;
	}

	return true;
}
