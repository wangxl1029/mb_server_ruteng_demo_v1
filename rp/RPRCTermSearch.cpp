#include "InnerCommon.hpp"
#include "RPCommon.hpp"
#include "RPRouteCalcRequest.hpp"
#include "RPRCTermSearch.hpp"

CRPRCTermSearch::CRPRCTermSearch(CRPWayPoint &clWayPoint, int iLevel, SmartPointer< CDPFacade > spclDataProvider)
: m_clWayPoint(clWayPoint), m_iLevel(iLevel), m_spclDataProvider(spclDataProvider)
{
}

CRPRCTermSearch::~CRPRCTermSearch()
{
}

bool CRPRCTermSearch::Do(SmartPointer< CDPFacade > spclDataProvider)
{

	uint32_t	uiTileNo = 0;
	CDPCommon::CoordToTileNo(m_iLevel, m_clWayPoint.m_clCoord.x, m_clWayPoint.m_clCoord.y, uiTileNo);

	CRPTileID	clTileID;
	CDPCommon::TileNoToPackedTileID(m_iLevel, uiTileNo, clTileID.m_uiPackedTileID);

	vector< string >	vstrUpdateRegion;
	if (SUCCESS != spclDataProvider->GetUpdateRegionByTile(BUILDING_BLOCK_ID_ROUTING, clTileID.m_uiPackedTileID, vstrUpdateRegion)) {
		ERR("");
		return false;
	}

	return true;
}
