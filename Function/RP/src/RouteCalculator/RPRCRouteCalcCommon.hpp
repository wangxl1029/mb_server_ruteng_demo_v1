#pragma once

#define	RPRC_MAX_USHORT_COST										(0xFFFF)
#define	RPRC_INVALID_LINK_NO										((ushort)(-1))

#define	RPRC_QUEUE_UNIT_COUNT										(10000)

//	class RPRC_Queue
template < class T > class RPRC_Queue
{
public:
	RPRC_Queue(size_t uiUnitCount = RPRC_QUEUE_UNIT_COUNT) : m_uiUnitCount(uiUnitCount) {}
	~RPRC_Queue() {}

	T& New()
	{
		++m_uiLastNo;
		if (m_uiLastNo >= m_uiUnitCount || m_vvElem.size() == 0) {
			m_uiLastNo = 0;
			m_vvElem.push_back(vector< T >());
			m_vvElem.back().resize(m_uiUnitCount);
		}
		return Back();
	}

	T& Back()
	{
		return m_vvElem.back()[m_uiLastNo];
	}

	size_t Size()
	{
		return m_vvElem.size() == 0 ? 0 : (m_vvElem.size() - 1) * m_uiUnitCount + m_uiLastNo;
	}

public:
	size_t														m_uiUnitCount;
	size_t														m_uiLastNo;
	list< vector< T > >											m_vvElem;
};

//	class RPRCTileContainer
template < class TileData > class RPRCTileContainer : public CCFSimpleFileCache< CRPTileID, TileData >
{
public:
	RPRCTileContainer(SmartPointer< CDPFacade > spclDataProvider) : m_spclDataProvider(spclDataProvider) {}
	virtual ~RPRCTileContainer() {}

	virtual int ReadData(const CRPTileID &clTileID, TileData &d)
	{
		SmartPointer< CDPDataRoutingTile >	spclDPDataRoutingTile;
		if (!spclDPDataRoutingTile.Create()) {
			ERR("");
			return FAILURE;
		}
		int		iLevel = 0;
		uint	uiTileNo = 0;
		if (SUCCESS != CDPCommon::PackedTileIDToTileNo(clTileID.m_uiPackedTileID, iLevel, uiTileNo)) {
			ERR("");
			return FAILURE;
		}
		string	strUpdateRegion = CDPCommon::UpdateRegionId2Name(clTileID.m_sUpdateRegionID);
		if (SUCCESS != m_spclDataProvider->GetRoutingTileData(strUpdateRegion, iLevel, uiTileNo, NEW_VERSION, spclDPDataRoutingTile)) {
			ERR("");
			return FAILURE;
		}
		if (SUCCESS != d.Initialize(spclDPDataRoutingTile)) {
			ERR("");
			return FAILURE;
		}
		return SUCCESS;
	}

public:
	SmartPointer< CDPFacade >									m_spclDataProvider;

	CCFMutex													m_clMutex;
};
	
//	class RPRCTileContainerProxy
template < class TileData, class Container = RPRCTileContainer< TileData > > class RPRCTileContainerProxy : public CBaseObj
{
public:
	RPRCTileContainerProxy(SmartPointer< Container > spclTileContainer)
		: m_spclTileContainer(spclTileContainer) {}
	virtual ~RPRCTileContainerProxy() {}

	TileData* GetData(const CRPTileID &clTileID)
	{
		if (clTileID == m_clCurTileID) {
			return m_spclCurTileData.Get();
		}
		m_clCurTileID = clTileID;
		m_spclCurTileData = m_clTileDataCache.GetData(clTileID);
		if (m_spclCurTileData == NULL) {
			m_spclCurTileData = m_spclTileContainer->GetData(clTileID);
			if (m_spclCurTileData == NULL) {
				ERR("");
				return NULL;
			}
			m_clTileDataCache.PutData(clTileID, m_spclCurTileData);
		}
		return m_spclCurTileData.Get();
	}

	void SetCapacity(uint uiCapacity)
	{
		m_clTileDataCache.SetCapacity(uiCapacity);
	}

public:
	typedef CCFSimpleCache< CRPTileID, TileData, less< CRPTileID >, CCFSingleThreaded< int > >	SingleThreadedTileDataCacheType;

	SmartPointer< Container >									m_spclTileContainer;
	SingleThreadedTileDataCacheType								m_clTileDataCache;
	CRPTileID													m_clCurTileID;
	SmartPointer< TileData >									m_spclCurTileData;
};

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

//	CloseTable
typedef RPRC_Queue< CRPRCMidLink >								RPRC_MidLinkTable;

//	class CRPRCMidLinkUsing
class CRPRCMidLinkUsing
{
public:
	CRPRCMidLinkUsing();
	~CRPRCMidLinkUsing();

public:
	CRPRCMidLink									* volatile	m_apclMidLink[RP_TERM_COUNT][LINK_DIR_COUNT];
};

//	class CRPRCMidLinkUsingTile
class CRPRCMidLinkUsingTile : public CBaseObj
{
public:
	CRPRCMidLinkUsingTile();
	virtual ~CRPRCMidLinkUsingTile();

	RESULT Initialize(SmartPointer< CDPDataRoutingTile > spclDPDataRoutingTile);

public:
	vector< CRPRCMidLinkUsing >									m_vclLinkUsingList;

	SmartPointer< CDPDataRoutingTile >							m_spclDPDataRoutingTile;
};

//	class CRPRCMidLinkUsingTableProxy
class CRPRCMidLinkUsingTableProxy : public RPRCTileContainerProxy< CRPRCMidLinkUsingTile >
{
public:
	CRPRCMidLinkUsingTableProxy(SmartPointer< RPRCTileContainer< CRPRCMidLinkUsingTile > > spclRPMidLinkUsingTileContainer);
	~CRPRCMidLinkUsingTableProxy();

	CRPRCMidLinkUsing* GetMidLinkUsing(const CRPRCLinkID &clMidLinkID);
	CRPRCMidLinkUsing* GetMidLinkUsing(const CRPTileID &TileID, ushort usLinkNo);

public:
	SmartPointer< RPRCTileContainer< CRPRCMidLinkUsingTile > >		m_spclRPMidLinkUsingTileContainer;
};

//	class CRPRCLinkCostTile
class CRPRCLinkCostTile : public CBaseObj
{
public:
	CRPRCLinkCostTile();
	virtual ~CRPRCLinkCostTile();

	RESULT Initialize(SmartPointer< CDPDataRoutingTile > spclDPDataRoutingTile);

public:
	vector< vector< vector< ushort > > >						m_vvvusNodeCost;
	vector< ushort >											m_avusLinkCost[LINK_DIR_COUNT];

	SmartPointer< CDPDataRoutingTile >							m_spclDPDataRoutingTile;
};

//	class CRPRCLinkCostTableProxy
class CRPRCLinkCostTableProxy : public RPRCTileContainerProxy< CRPRCLinkCostTile >
{
public:
	CRPRCLinkCostTableProxy(SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > > spclRPLinkCostTileContainer);
	~CRPRCLinkCostTableProxy();

	ushort& GetNodeCostRef(const CRPTileID &TileID, ushort usNodeNo, ushort usInLinkNo, ushort usOutLinkNo);
	ushort& GetLinkCostRef(const CRPTileID &TileID, ushort uiLinkNo, bool bPos);

public:
};

//	class CRPRCLinkCostContainerSet
class CRPRCLinkCostContainerSet : public CBaseObj
{
public:
	CRPRCLinkCostContainerSet();
	virtual ~CRPRCLinkCostContainerSet();

	RESULT Initialize(SmartPointer< CDPFacade > spclDataProvider);

	RESULT SwitchDbStart();
	RESULT WaitForCanSwitchDb();
	RESULT SwitchDbEnd();

public:
	SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > >		m_aspclLinkCostContainer[RP_ROUTE_SEARCH_TYPE_COUNT];

	volatile bool												m_bDbSwitching;
};

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

//	class CRPRCUpSearchResultLink
class CRPRCUpSearchResultLink
{
public:
	explicit CRPRCUpSearchResultLink(CRPRCMidLink *pclMidLink = NULL);
	~CRPRCUpSearchResultLink();

public:
	CRPRCMidLink													*m_pclMidLink;
};

//	class CRPRCConnectSearchResultLink
class CRPRCConnectSearchResultLink
{
public:
	explicit CRPRCConnectSearchResultLink(const RP_TERM enTerm = RP_TERM_COUNT, CRPRCMidLink *pclMidLink = NULL);
	~CRPRCConnectSearchResultLink();

public:
	RP_TERM														m_enTerm;
	CRPRCMidLink													*m_pclMidLink;
};


