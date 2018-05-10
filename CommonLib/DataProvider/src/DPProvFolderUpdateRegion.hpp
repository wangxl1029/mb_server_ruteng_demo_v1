#pragma once
class CDPProvFolderUpdateRegion :
	public CDPProv
{
public:
	CDPProvFolderUpdateRegion() : m_bDbSwitching(false) {}
	virtual ~CDPProvFolderUpdateRegion() {}

	RESULT Initialize(string strProductName, string strUpdateRegion, SmartPointer< CDPDBConnectionPool > spclDBConnectionPool);
	RESULT GetProvShared(SmartPointer< CDPProvShared > &spclProvShared);
//	RESULT GetProvBmd(SmartPointer< CDPProvBmd > &spclProvBmd);
	RESULT GetProvRouting(SmartPointer< CDPProvRouting > &spclProvRouting);
//	RESULT GetProvNamedObject(SmartPointer< CDPProvNamedObject > &spclProvNamedObject);
//	RESULT GetProv3DObj(SmartPointer< CDPProv3DObj > &spclProv3DObj);

public:
	SmartPointer< CDPDBConnectionPool >							m_spclDBConnectionPool;
	string														m_strProductName;
	string														m_strUpdateRegion;
	CCFMutex													m_clMutex;
	SmartPointer< CDPProvShared >								m_spclDPProvShared;
	//SmartPointer< CDPProvBmd >								m_spclDPProvBmd;
	SmartPointer< CDPProvRouting >								m_spclDPProvRouting;
	//SmartPointer< CDPProvNamedObject >						m_spclDPProvNamedObject;
	//SmartPointer< CDPProv3DObj >								m_spclDPProv3DObj;
	volatile bool												m_bDbSwitching;
};

