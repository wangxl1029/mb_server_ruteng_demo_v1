#include	"InnerCommon.hpp"
#include	"RPCommon.hpp"
//#include	"RPRSLink.h"
//#include	"RPWayPoints.hpp"
//#include	"RPSection.h"
//#include	"RPRoute.h"
#include	"RPRouteCalcRequest.hpp"
#include	"RPRCRouteCalcCommon.hpp"
#include	"RPRCCost.hpp"
//#include	"RPRCExtIF.h"
//#include	"RPRCRouteSearchCommFunc.h"

const ushort	s_usMaxShortCost = 65536;
const uint		s_uiMaxCost = 3600 * 8 * 24 * 10;

CRPRCCost::CRPRCCost()
{
}

CRPRCCost::~CRPRCCost()
{
}

RESULT CRPRCCost::Initialize()
{
	return SUCCESS;
}

ushort CRPRCCost::LinkCost(const CRPRCLinkID &clLinkID, CRPRCRoutingTileProxy &clLinkRoutingTile)
{
	//	MESSAGE( "CRPRCCost::LinkCost %d,%d,%s", clLinkID.m_clTileID.m_uiPackedTileID, clLinkID.m_usLinkNo, clLinkID.m_bPos?"pos":"neg" );

	clLinkRoutingTile.GetData(clLinkID.m_clTileID);
	if (clLinkID.m_usLinkNo >= clLinkRoutingTile->m_clRoutingTile.m_clLinks.m_vclLink.size()) {
		ERR("");
	}
	CFixedRoadAttributeSet	*pclFixedRoadAttributeSet = NULL;
	CLink	&clLink = clLinkRoutingTile->m_clRoutingTile.m_clLinks.m_vclLink[clLinkID.m_usLinkNo];
	if (clLink.m_ucAttrSource == INDEX_8 || clLink.m_ucAttrSource == INDEX_16) {
		uint	uiFixedRoadAttributeSetListIndex = clLink.m_clAttrInfo.FixedRoadAttributeSetListIndex_Ref();
		if (uiFixedRoadAttributeSetListIndex >= clLinkRoutingTile->m_clRoutingTile.m_clFixedRoadAttributeSetList.m_vclAttributeList.size()) {
			ERR("");
			return s_usMaxShortCost;
		}
		pclFixedRoadAttributeSet = &clLinkRoutingTile->m_clRoutingTile.m_clFixedRoadAttributeSetList.m_vclAttributeList[uiFixedRoadAttributeSetListIndex];
	}
	else if (clLink.m_ucAttrSource == EXPLICIT) {
		pclFixedRoadAttributeSet = &clLink.m_clAttrInfo.fixedAttributes_Ref();
	}
	else {
		ERR("");
		return s_usMaxShortCost;
	}
	switch (pclFixedRoadAttributeSet->m_clSharedAttr.m_ucTravelDirection) {
	case IN_POSITIVE_DIRECTION:
		if (!clLinkID.m_bPos) {
			return s_usMaxShortCost;
		}
		break;
	case IN_NEGATIVE_DIRECTION:
		if (clLinkID.m_bPos) {
			return s_usMaxShortCost;
		}
		break;
	case IN_NO_DIRECTION:
		return s_usMaxShortCost;
	case IN_BOTH_DIRECTIONS:
		break;
	default:
		ERR("");
		break;
	}

	uint	uiLength = clLink.m_uiLength;
	uint	uiSpeed = clLink.m_ucAverageSpeed;
	uint	uiCost = uiLength * 36 / uiSpeed / 100;

	//	MESSAGE( "CRPRCCost::LinkCost %d,%d,%s %d", clLinkID.m_clTileID.m_uiPackedTileID, clLinkID.m_usLinkNo, clLinkID.m_bPos?"pos":"neg", uiCost );

	return uiCost;
}

ushort CRPRCCost::NodeCost()
{
	return 60 * 10;
}

uint CRPRCCost::ExtractShortCost(ushort usShortCost)
{
	if (s_usMaxShortCost == usShortCost) {
		return s_uiMaxCost;
	}
	return usShortCost;
}
