#pragma once
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

class CDPDBConnectionPool;

//	CDPConnRec
class CDPConnRec : public CBaseObj
{
public:
	CDPConnRec();
	virtual ~CDPConnRec();

	bool Connect(std::shared_ptr< CDPDBConnectionPool > spclConnctionPool, const char *pcProductName, const char *pcUpdateRegion, DPDB_TYPE enDBType);
	std::shared_ptr< CDPDBConnectionPool >							m_spclConnctionPool;
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

