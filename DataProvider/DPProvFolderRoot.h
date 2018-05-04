#pragma once
#include "CFBaseObj.hpp"
class CDPProvFolderRoot : // class name same as znavi
	public CBaseObj
{
public:
	CDPProvFolderRoot();
	virtual ~CDPProvFolderRoot();

public:
	volatile bool												m_bDbSwitching;
};

