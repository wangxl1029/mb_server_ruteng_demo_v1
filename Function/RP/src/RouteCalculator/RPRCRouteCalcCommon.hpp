#pragma once

#define	RPRC_MAX_USHORT_COST										(0xFFFF)
#define	RPRC_INVALID_LINK_NO										((ushort)(-1))

#define	RPRC_QUEUE_UNIT_COUNT										(10000)

//	class CRPRCLinkID
class CRPRCLinkID
{
public:
	CRPRCLinkID(const CRPTileID& clTileID = CRPTileID(), ushort usLinkNo = RPRC_INVALID_LINK_NO, bool bPos = true);
	~CRPRCLinkID();

public:
	CRPTileID													m_clTileID;
	ushort														m_usLinkNo;
	bool														m_bPos;
};

inline bool operator==(const CRPRCLinkID &clLeft, const CRPRCLinkID &clRight)
{
	return clLeft.m_clTileID == clRight.m_clTileID && clLeft.m_usLinkNo == clRight.m_usLinkNo && clLeft.m_bPos == clRight.m_bPos;
}

inline bool operator<(const CRPRCLinkID &clLeft, const CRPRCLinkID &clRight)
{
	if (clLeft.m_clTileID < clRight.m_clTileID) {
		return true;
	}
	else if (clRight.m_clTileID < clLeft.m_clTileID) {
		return false;
	}
	else if (clLeft.m_usLinkNo < clRight.m_usLinkNo) {
		return true;
	}
	else if (clLeft.m_usLinkNo > clRight.m_usLinkNo) {
		return false;
	}
	else if (!clLeft.m_bPos && clRight.m_bPos) {
		return true;
	}
	else {
		return false;
	}
}

//	class CRPRCNodeID
class CRPRCNodeID
{
public:
	CRPRCNodeID(const CRPTileID& clTileID = CRPTileID(), const CDPRoutingTileLinkNodeNo &clNodeNo = CDPRoutingTileLinkNodeNo());
	~CRPRCNodeID();

public:
	CRPTileID													m_clTileID;
	CDPRoutingTileLinkNodeNo									m_clNodeNo;
};

class CRPRCMidLink;


//	OpenTable
typedef multimap< uint, CRPRCMidLink* >							RPRC_OpenTable;
typedef pair< uint, CRPRCMidLink* >								RPRC_OpenTableValueType;
typedef RPRC_OpenTable::iterator								RPRC_OpenTableIterator;
#define	RPRC_OpenTableIterator_Null								RPRC_OpenTableIterator()

//	class CRPRCMidLink
class CRPRCMidLink
{
public:
	CRPRCMidLink(uint uiLinkCost = 0, uint uiArriveCost = 0, CRPRCMidLink *pclPrevLink = NULL, CRPRCMidLink *m_pclNextLink = NULL,
		CRPRCMidLink *pclPrevSibling = NULL, CRPRCMidLink *pclNextSibling = NULL, CRPRCMidLink *pclDownLink = NULL, CRPRCMidLink *pclUpLink = NULL,
		RPRC_OpenTableIterator itOpenLink = RPRC_OpenTableIterator_Null);
	~CRPRCMidLink();

public:
	CRPRCLinkID													m_clLinkID;
	CRPRCNodeID													m_clFromNode;
	CPointCoord2D												m_clFromPoint;		//	for extend
	CPointCoord2D												m_clToPoint;		//	for extend
	uint														m_uiLinkCost;
	uint														m_uiArriveCost;
	ushort														m_usNextLinkCnt;
	CRPRCMidLink												*m_pclPrevLink;
	CRPRCMidLink												*m_pclNextLink;
	CRPRCMidLink												*m_pclPrevSibling;
	CRPRCMidLink												*m_pclNextSibling;
	CRPRCMidLink												*m_pclDownLink;
	CRPRCMidLink												*m_pclUpLink;
	RPRC_OpenTableIterator										m_itOpenLink;
};

void RPRCMidLink_ConnectLink(CRPRCMidLink *pclInLink, CRPRCMidLink *pclOutLink);
void RPRCMidLink_DisconectLink(CRPRCMidLink *pclOutLink);

//	class CRPRCRoutingTileProxy
class CRPRCRoutingTileProxy
{
public:
	CRPRCRoutingTileProxy(SmartPointer< CDPFacade > spclDataProvider);
	~CRPRCRoutingTileProxy();

	RESULT GetData(const CRPTileID &clTileID);
	CDPDataRoutingTile* operator->();
	bool IsEmpty();

public:
	CRPTileID													m_clLastTileID;
	SmartPointer< CDPDataRoutingTile >							m_spclLastRoutingTile;
	SmartPointer< CDPFacade >									m_spclDataProvider;
};



