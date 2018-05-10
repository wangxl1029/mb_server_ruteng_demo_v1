#pragma once
class CDPProvRouting :
	public CDPProv
{
public:
	CDPProvRouting() : m_bDbSwitching(false) {}
	virtual ~CDPProvRouting() {}

	RESULT Initialize(string strProductName, string strUpdateRegion, SmartPointer< CDPDBConnectionPool > spclDBConnectionPool);

	RESULT SwitchDbStart();
	RESULT WaitForCanSwitchDb();
	RESULT SwitchDbEnd();

	RESULT GetRoutingTile(uint uiPackedTileID, short sVersion, SmartPointer< CDPDataRoutingTile > &spclDPDataRoutingTile, uchar ucCoordWidth);
	RESULT GetRoutingAuxTile(uint uiPackedTileID, short sVersion, SmartPointer< CDPDataRoutingAuxTile > &spclDPDataRoutingAuxTile, uchar ucCoordWidth);

public:
	SmartPointer< CDPDBConnectionPool >							m_spclDBConnectionPool;
	string														m_strProductName;
	string														m_strUpdateRegion;
	CCFMutex													m_clMutex;
	CDPConnRec													m_clDatabase;
	CCFSimpleCache< CTileID, CDPDataRoutingTile >				m_clRoutingTileCache;
	CCFSimpleCache< CTileID, CDPDataRoutingAuxTile >			m_clRoutingAuxTileCache;
	volatile bool												m_bDbSwitching;
};

