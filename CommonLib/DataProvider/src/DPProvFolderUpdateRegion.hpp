#pragma once
class CDPProvFolderUpdateRegion :
	public CDPProv
{
public:
	CDPProvFolderUpdateRegion() : m_bDbSwitching(false) {}
	virtual ~CDPProvFolderUpdateRegion() {}

	RESULT Initialize(string strProductName, string strUpdateRegion, SmartPointer< CDPDBConnectionPool > spclDBConnectionPool);
public:
	SmartPointer< CDPDBConnectionPool >							m_spclDBConnectionPool;
	string														m_strProductName;
	string														m_strUpdateRegion;
	CCFMutex													m_clMutex;
	SmartPointer< CDPProvShared >								m_spclDPProvShared;
	volatile bool												m_bDbSwitching;
};

