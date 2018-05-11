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

//	class CRPRCMidLink
CRPRCMidLink::CRPRCMidLink(uint uiLinkCost, uint uiArriveCost, CRPRCMidLink *pclPrevLink, CRPRCMidLink *pclNextLink,
	CRPRCMidLink *pclPrevSibling, CRPRCMidLink *pclNextSibling, CRPRCMidLink *pclDownLink, CRPRCMidLink *pclUpLink,
	RPRC_OpenTableIterator itOpenLink)
	: m_uiLinkCost(uiLinkCost), m_uiArriveCost(uiArriveCost), m_usNextLinkCnt(0), m_pclPrevLink(pclPrevLink), m_pclNextLink(pclNextLink)
	, m_pclPrevSibling(pclPrevSibling), m_pclNextSibling(pclNextSibling), m_pclDownLink(pclDownLink), m_pclUpLink(pclUpLink),
	m_itOpenLink(itOpenLink)
{
}

CRPRCMidLink::~CRPRCMidLink()
{
}

void RPRCMidLink_ConnectLink(CRPRCMidLink *pclInLink, CRPRCMidLink *pclOutLink)
{
	if (pclInLink->m_pclNextLink == NULL) {
		pclInLink->m_pclNextLink = pclOutLink;
		pclOutLink->m_pclNextSibling = pclOutLink;
		pclOutLink->m_pclPrevSibling = pclOutLink;
	}
	else {
		CRPRCMidLink	*pclHead = pclInLink->m_pclNextLink;
		CRPRCMidLink	*pclTail = pclHead->m_pclPrevSibling;
		pclTail->m_pclNextSibling = pclOutLink;
		pclOutLink->m_pclNextSibling = pclHead;
		pclHead->m_pclPrevSibling = pclOutLink;
		pclOutLink->m_pclPrevSibling = pclTail;
	}
	++pclInLink->m_usNextLinkCnt;
	pclOutLink->m_pclPrevLink = pclInLink;
}

void RPRCMidLink_DisconectLink(CRPRCMidLink *pclOutLink)
{
	if (pclOutLink->m_pclPrevLink == NULL) {
		if (pclOutLink->m_pclDownLink == NULL) {
			ERR("");
			return;
		}
		pclOutLink->m_pclDownLink = NULL;
	}
	else {
		//	remove
		CRPRCMidLink	*pclInLink = pclOutLink->m_pclPrevLink;
		if (pclOutLink->m_pclNextSibling == pclOutLink) {
			pclInLink->m_pclNextLink = NULL;
			pclOutLink->m_pclPrevSibling = NULL;
			pclOutLink->m_pclNextSibling = NULL;
		}
		else {
			if (pclInLink->m_pclNextLink == pclOutLink) {
				pclInLink->m_pclNextLink = pclInLink->m_pclNextLink->m_pclNextSibling;
			}
			pclOutLink->m_pclPrevSibling->m_pclNextSibling = pclOutLink->m_pclNextSibling;
			pclOutLink->m_pclNextSibling->m_pclPrevSibling = pclOutLink->m_pclPrevSibling;
			pclOutLink->m_pclPrevSibling = pclOutLink;
			pclOutLink->m_pclNextSibling = pclOutLink;
		}

		--pclInLink->m_usNextLinkCnt;
		pclOutLink->m_pclPrevLink = NULL;
	}
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

//	class CRPRCLinkCostTile
CRPRCLinkCostTile::CRPRCLinkCostTile()
{
}

CRPRCLinkCostTile::~CRPRCLinkCostTile()
{
}

RESULT CRPRCLinkCostTile::Initialize(SmartPointer< CDPDataRoutingTile > spclDPDataRoutingTile)
{
	m_spclDPDataRoutingTile = spclDPDataRoutingTile;

	size_t	uiNodeCnt = spclDPDataRoutingTile->m_clRoutingTile.m_clSimpleIntersection.m_vclSimpleIntersection.size();
	m_vvvusNodeCost.resize(uiNodeCnt);
	for (size_t i = 0; i < uiNodeCnt; ++i) {
		CSimpleIntersection	&clNode = spclDPDataRoutingTile->m_clRoutingTile.m_clSimpleIntersection.m_vclSimpleIntersection[i];
		size_t	uiConnectCnt = clNode.m_vclConnectedLinks.size() * clNode.m_vclConnectedLinks.size();
		m_vvvusNodeCost[i].resize(uiConnectCnt);
		for (size_t j = 0; j < uiConnectCnt; ++j) {
			m_vvvusNodeCost[i][j].resize(uiConnectCnt, 0);
		}
	}
	size_t	uiLinkCnt = spclDPDataRoutingTile->m_clRoutingTile.m_clLinks.m_vclLink.size();
	for (size_t k = 0; k < sizeof(m_avusLinkCost) / sizeof(m_avusLinkCost[0]); ++k) {
		m_avusLinkCost[k].resize(uiLinkCnt);
	}
	return SUCCESS;
}

//	class CRPRCLinkCostTableProxy
CRPRCLinkCostTableProxy::CRPRCLinkCostTableProxy(SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > > spclRPLinkCostTileContainer)
: RPRCTileContainerProxy< CRPRCLinkCostTile >(spclRPLinkCostTileContainer)
{
}

CRPRCLinkCostTableProxy::~CRPRCLinkCostTableProxy()
{
}

ushort& CRPRCLinkCostTableProxy::GetNodeCostRef(const CRPTileID &clTileID, ushort usNodeNo, ushort usInLinkConnNo, ushort usOutLinkConnNo)
{
	CRPRCLinkCostTile	*pclLinkCostTile = GetData(clTileID);
	if (NULL == pclLinkCostTile) {
		ERR("");
	}
	if (usNodeNo >= pclLinkCostTile->m_vvvusNodeCost.size()) {
		ERR("");
	}
	if (usInLinkConnNo >= pclLinkCostTile->m_vvvusNodeCost[usNodeNo].size()) {
		ERR("");
	}
	if (usOutLinkConnNo >= pclLinkCostTile->m_vvvusNodeCost[usNodeNo][usInLinkConnNo].size()) {
		ERR("");
	}
	return pclLinkCostTile->m_vvvusNodeCost[usNodeNo][usInLinkConnNo][usOutLinkConnNo];
}

ushort& CRPRCLinkCostTableProxy::GetLinkCostRef(const CRPTileID &clTileID, ushort usLinkNo, bool bPos)
{
	CRPRCLinkCostTile		*pclLinkCostTile = GetData(clTileID);
	if (NULL == pclLinkCostTile) {
		ERR("");
	}
	if (usLinkNo >= pclLinkCostTile->m_avusLinkCost[LINK_DIR_NO(bPos)].size()) {
		ERR("");
	}
	return pclLinkCostTile->m_avusLinkCost[LINK_DIR_NO(bPos)][usLinkNo];
}
