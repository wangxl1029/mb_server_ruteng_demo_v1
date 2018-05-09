#pragma once

class CDPProvFolderRoot : public CDPProv
{
public:
	CDPProvFolderRoot() : m_bDbSwitching(false) {}
	virtual ~CDPProvFolderRoot() {}

	RESULT Initialize(SmartPointer< CDPDBConnectionPool > spclDBConnectionPool);

	RESULT SwitchDbStart();
	RESULT WaitForCanSwitchDb();
	RESULT SwitchDbEnd();

	RESULT GetProvRoot(SmartPointer< CDPProvRoot > &spclDPProvRoot);
	RESULT GetFolderProduct(string strProductName, SmartPointer< CDPProvFolderProduct > &spclFolderProduct);
//	RESULT GetProvDrawParameter(SmartPointer< CDPProvDrawParameter > &spclProvDrawParameter);

public:
	SmartPointer< CDPDBConnectionPool >							m_spclDBConnectionPool;
	CCFMutex													m_clMutex;
	SmartPointer< CDPProvRoot >									m_spclDPProvRoot;
	CCFSimpleCache< string, CDPProvFolderProduct >				m_clDPProvFolderProductCache;
//	SmartPointer< CDPProvDrawParameter >						m_spclProvDrawParameter;
	volatile bool												m_bDbSwitching;
};

