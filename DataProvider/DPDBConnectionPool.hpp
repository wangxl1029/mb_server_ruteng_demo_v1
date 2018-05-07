#pragma once
#include <string>
#include "CFBaseObj.hpp"
enum DPDB_TYPE {
	DPDB_ROOT,
	DPDB_PRODUCT,
	DPDB_SHARED,
	DPDB_BMD,
	DPDB_ROUTING,
	DPDB_NAME,
	DPDB_3DOBJ,
	DPDB_DRAWPARAMETER,
};

class CDPDBConnectionPool :
	public CBaseObj
{
public:
	CDPDBConnectionPool() : m_bUpdating(false) {}
	virtual ~CDPDBConnectionPool() {}

	bool Initialize(const char *pcRootPath);

public:
	std::string													m_strRootPath;
	volatile bool												m_bUpdating;
};

