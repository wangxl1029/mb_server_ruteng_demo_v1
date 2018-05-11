#pragma once
//	class CRPRCRouteSearchCommFunc
class CRPRCRouteSearchCommFunc
{
public:
	CRPRCRouteSearchCommFunc();
	~CRPRCRouteSearchCommFunc();

	static RESULT GetNodeNoByLink(CRPRCLinkID &clLinkID,
		CRPRCRoutingTileProxy &clLinkRoutingTile,
		CRPTileID &clNodeTileID,
		CDPRoutingTileLinkNodeNo &clInLinkNodeNo,
		CRPRCRoutingTileProxy &clNodeRoutingTile);

	static RESULT GetConnectLinkID(const CRPTileID &clNodeTileID, size_t uiNodeNo, size_t uiConnectNo,
		CRPRCRoutingTileProxy &clNodeRoutingTile,
		CRPRCLinkID &clOutLinkID,
		CRPRCRoutingTileProxy &clOutLinkRoutingTile);

	static RESULT CalcArriveCost(RP_TERM enTerm, CRPRCLinkCostTableProxy &clLinkCostTableProxy, CRPRCCost &clCost,
		uint uiInLinkArriveCost,
		const CRPRCLinkID &clInLinkID, CRPRCRoutingTileProxy &clInLinkRoutingTile,
		const CRPTileID &clNodeTileID,
		ushort usNodeNo, ushort usInLinkConnNo, ushort usOutLinkConnNo,
		CRPRCRoutingTileProxy &clNodeRoutingTile,
		const CRPRCLinkID &clOutLinkID, CRPRCRoutingTileProxy &clOutLinkRoutingTile,
		uint &uiArriveCost, uint &uiLinkCost, uint &uiNodeCost);

	static void UpdateMidLinkCost(CRPRCMidLink *pclLink,
		uint uiCostDiff,
		RPRC_OpenTable &mapOpenTable);
};
