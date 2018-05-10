#include	"InnerCommon.hpp"
#include	"RPCommon.hpp"
#include	"RPRCRouteCalcCommon.hpp"

//	class CRPRCLinkID
CRPRCLinkID::CRPRCLinkID(const CRPTileID &clTileID, ushort usLinkNo, bool bPos)
: m_clTileID(clTileID), m_usLinkNo(usLinkNo), m_bPos(bPos)
{
}

CRPRCLinkID::~CRPRCLinkID()
{
}

//	class CRPRCNodeID
CRPRCNodeID::CRPRCNodeID(const CRPTileID &clTileID, const CDPRoutingTileLinkNodeNo &clNodeNo)
: m_clTileID(clTileID), m_clNodeNo(clNodeNo)
{
}

CRPRCNodeID::~CRPRCNodeID()
{
}


//	class CRPRCRoutingTileProxy
CRPRCRoutingTileProxy::CRPRCRoutingTileProxy(SmartPointer< CDPFacade > spclDataProvider)
: m_spclDataProvider(spclDataProvider)
{
}

CRPRCRoutingTileProxy::~CRPRCRoutingTileProxy()
{
}

RESULT CRPRCRoutingTileProxy::GetData(const CRPTileID &clTileID)
{
	if (clTileID == m_clLastTileID) {
		return SUCCESS;
	}

	m_clLastTileID = CRPTileID();

	int		iLevel = 0;
	uint	uiTileNo = 0;
	CDPCommon::PackedTileIDToTileNo(clTileID.m_uiPackedTileID, iLevel, uiTileNo);
	string	strUR = CDPCommon::UpdateRegionId2Name(clTileID.m_sUpdateRegionID);
	if (SUCCESS != m_spclDataProvider->GetRoutingTileData(strUR, iLevel, uiTileNo, NEW_VERSION, m_spclLastRoutingTile)) {
		ERR("");
		return FAILURE;
	}
	m_clLastTileID = clTileID;
	return SUCCESS;
}

CDPDataRoutingTile* CRPRCRoutingTileProxy::operator->()
{
	return m_spclLastRoutingTile.Get();
}

bool CRPRCRoutingTileProxy::IsEmpty()
{
	return m_spclLastRoutingTile.Get() == NULL;
}
