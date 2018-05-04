#pragma once
#include "CFBaseObj.hpp"
class CDPProvFolderRoot :
	public CBaseObj
{
public:
	CDPProvFolderRoot();
	virtual ~CDPProvFolderRoot();

public:
	volatile bool												m_bDbSwitching;
};

