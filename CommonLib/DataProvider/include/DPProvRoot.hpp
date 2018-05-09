#pragma once
#include "DPProv.hpp"
class CDPProvRoot :
	public CDPProv
{
public:
	CDPProvRoot() : m_bDbSwitching(false) {}
	virtual ~CDPProvRoot() {}

	RESULT Initialize(SmartPointer< CDPDBConnectionPool > spclDBConnectionPool);

public:
	SmartPointer< CDPDBConnectionPool >							m_spclDBConnectionPool;
	CCFMutex													m_clMutex;
	CDPConnRec													m_clDatabase;
	volatile bool												m_bDbSwitching;
};

