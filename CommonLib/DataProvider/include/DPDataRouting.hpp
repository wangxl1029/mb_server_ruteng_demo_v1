#pragma once
//	class CDPRoutingTileNodeInfo
class CDPRoutingTileNodeInfo
{
public:
	CDPRoutingTileNodeInfo(uint uiGatewayID = NDS_INVALID_GATEWAY_ID) : m_uiGatewayID(uiGatewayID) {}
	~CDPRoutingTileNodeInfo() {}

	uint														m_uiGatewayID;
};

//	class CDPRoutingTileExtLinkID
class CDPRoutingTileExtLinkID
{
public:
	CDPRoutingTileExtLinkID(uint uiTileID = NDS_INVALID_TILE_ID, ushort usLinkID = NDS_INVALID_LINK_ID)
		: m_uiTileID(uiTileID), m_usLinkID(usLinkID) {}
	~CDPRoutingTileExtLinkID() {}

	uint														m_uiTileID;
	ushort														m_usLinkID;
};

inline bool operator<(const CDPRoutingTileExtLinkID &clLeft, const CDPRoutingTileExtLinkID &clRight)
{
	if (clLeft.m_uiTileID < clRight.m_uiTileID) {
		return true;
	}
	else if (clLeft.m_uiTileID > clRight.m_uiTileID) {
		return false;
	}
	else if (clLeft.m_usLinkID < clRight.m_usLinkID) {
		return true;
	}
	else {
		return false;
	}
}

//	class CDPRoutingTileLinkNodeNo
class CDPRoutingTileLinkNodeNo
{
public:
	CDPRoutingTileLinkNodeNo(ushort usNodeNo = NDS_INVALID_NODE_ID, ushort usConnNo = NDS_INVALID_NODE_ID)
		: m_usNodeNo(usNodeNo), m_usConnNo(usConnNo) {}
	~CDPRoutingTileLinkNodeNo() {}

public:
	ushort														m_usNodeNo;
	ushort														m_usConnNo;
};

//	class CDPRoutingTileLinkInfo
class CDPRoutingTileLinkInfo
{
public:
	CDPRoutingTileLinkInfo() : m_usLink2TileNo(NDS_INVALID_NODE_ID)
	{
		for (size_t i = 0; i < sizeof(m_ausUpLinkRefNo) / sizeof(m_ausUpLinkRefNo[0]); ++i) {
			m_ausUpLinkRefNo[i] = NDS_INVALID_NODE_ID;
		}
	}
	~CDPRoutingTileLinkInfo() {}

public:
	CDPRoutingTileLinkNodeNo									m_clStartNode;
	CDPRoutingTileLinkNodeNo									m_clEndNode;
	ushort														m_usLink2TileNo;
	ushort														m_ausUpLinkRefNo[2];
};

//	class CDPRoutingTileInfo
class CDPRoutingTileInfo
{
public:
	CDPRoutingTileInfo() {}
	~CDPRoutingTileInfo() {}

public:
	vector< CDPRoutingTileNodeInfo >							m_vclNodeList;	//	与SimpleIntersectionList一一对应
	map< uint, uint >											m_mapGatewayID2NodeNo;
	vector< CDPRoutingTileLinkInfo >							m_vclLinkList;	//	与LinkList一一对应
	map< CDPRoutingTileExtLinkID, CDPRoutingTileLinkNodeNo >	m_mapExtLink2Node;
};

//	class CDPDataRoutingTile
class CDPDataRoutingTile : public CDPData
{
public:
	CDPDataRoutingTile() : m_iLevel(0), m_uiTileNo(0), m_usVersionID(0) { /*Init_RoutingTile( m_clRoutingTile );*/ }
	virtual ~CDPDataRoutingTile() { /*Release_RoutingTile( m_clRoutingTile );*/ }

public:
	CTileID								m_clTileID;
	int									m_iLevel;
	uint								m_uiTileNo;
	ushort								m_usVersionID;
	CPointCoord2D						m_clBasePoint;
	CRoutingTile						m_clRoutingTile;
	CDPRoutingTileInfo					m_clRoutingTileInfo;
};

//	class CDPDataRoutingAuxTile
class CDPDataRoutingAuxTile : public CDPData
{
public:
	CDPDataRoutingAuxTile() : m_usVersionID(0) { /*Init_RoutingGeoTile( m_clRoutingAuxTile );*/ }
	virtual ~CDPDataRoutingAuxTile() { /*Release_RoutingGeoTile( m_clRoutingAuxTile );*/ }

public:
	class CLinkID
	{
	public:
		uint							m_uiPackedTileID;
		uint							m_uiLinkID;
	};

public:
	CTileID								m_clTileID;
	ushort								m_usVersionID;
	CRoutingGeoTile						m_clRoutingAuxTile;
};
