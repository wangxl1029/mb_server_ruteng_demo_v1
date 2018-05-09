#pragma once


class CDPProvFolderProduct :
	public CBaseObj
{
public:
	CDPProvFolderProduct() : m_bDbSwitching(false) {}
	virtual ~CDPProvFolderProduct() {}

	RESULT Initialize(string strProductName, SmartPointer< CDPDBConnectionPool > spclDBConnectionPool);

	RESULT SwitchDbStart();
	RESULT WaitForCanSwitchDb();
	RESULT SwitchDbEnd();

	RESULT GetProvProduct(SmartPointer< CDPProvProduct > &spclDPProvProduct);
	RESULT GetProvFolderUpdateRegion(string strUpdateRegionName, SmartPointer< CDPProvFolderUpdateRegion > &spclDPProvFolderUpdateRegion);
public:
	SmartPointer< CDPDBConnectionPool >							m_spclDBConnectionPool;
	string														m_strProductName;
	CCFMutex													m_clMutex;
	SmartPointer< CDPProvProduct >								m_spclDPProvProduct;
	CCFSimpleCache< string, CDPProvFolderUpdateRegion >			m_clDPProvFolderUpdateRegionCache;
	volatile bool												m_bDbSwitching;
};

