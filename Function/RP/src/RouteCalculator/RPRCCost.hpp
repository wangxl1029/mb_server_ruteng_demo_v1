#pragma once
class CRPRCCost : public CBaseObj
{
public:
	CRPRCCost();
	virtual ~CRPRCCost();

	RESULT Initialize();

	RESULT SwitchDbStart();
	RESULT WaitForCanSwitchDb();
	RESULT SwitchDbEnd();

	ushort LinkCost(const CRPRCLinkID &clInLinkID, CRPRCRoutingTileProxy &clInLinkRoutingTile);
	ushort NodeCost();

	static uint ExtractShortCost(ushort usShortCost);

public:
	volatile bool												m_bDbSwitching;
};

