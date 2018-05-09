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

	RESULT Connect(SmartPointer< CDPDBConnectionPool > spclConnctionPool, const char *pcProductName, const char *pcUpdateRegion, DPDB_TYPE enDBType);
	RESULT Connect(SmartPointer< CDPDBConnectionPool > spclConnctionPool, const char *strFileName);

	CRCPtr< CSL_Database >& Get();
	CRCPtr< CSL_Database >& operator->();
private:
	CDPConnRec(const CDPConnRec &);
	CDPConnRec& operator=(const CDPConnRec &);

public:
	CRCPtr< CSL_Database >										m_spclDatabase;
	SmartPointer< CDPDBConnectionPool >							m_spclConnctionPool;
};

class CDPDBConnectionPool :
	public CBaseObj
{
public:
	CDPDBConnectionPool() : m_bUpdating(false) {}
	virtual ~CDPDBConnectionPool() {}

	bool Initialize(const char *pcRootPath);

	RESULT GetConnection(const char *pcProductName, const char *pcUpdateRegion, DPDB_TYPE enDBType, CRCPtr< CSL_Database > &spDatabase);
	RESULT GetConnection(const char *strFileName, CRCPtr< CSL_Database > &spDatabase);
	RESULT ReleaseConnection(CRCPtr< CSL_Database > &spDatabase);
	RESULT ReduceConnection();
	RESULT CloseAll();

	RESULT GetRootPath(string &strRootPath);
	RESULT GetDataFilePath(const char *pcProductName, const char *pcUpdateRegion, DPDB_TYPE enDBType, string &strDataFilePath);
	RESULT GetDataFileFullPath(const char *pcProductName, const char *pcUpdateRegion, DPDB_TYPE enDBType, string &strDataFilePath);
	RESULT AttachUpdateFiles(const char *strFileName, CRCPtr< CSL_Database > &spDatabase);
	RESULT DetachUpdateFiles(const char *strFileName, CRCPtr< CSL_Database > &spDatabase);
	RESULT AttachBackupFiles(const char *strFileName, CRCPtr< CSL_Database > &spDatabase);
	RESULT DetachBackupFiles(const char *strFileName, CRCPtr< CSL_Database > &spDatabase);

	class ConnRec
	{
	public:
		ConnRec(string strFileName, CRCPtr< CSL_Database > spclDatabase)
			: m_iRefCnt(1), m_strFileName(strFileName), m_spclDatabase(spclDatabase) {}

		int														m_iRefCnt;
		string													m_strFileName;	//	from /ROOT/DATA/
		CRCPtr< CSL_Database >									m_spclDatabase;
	};
public:
	string														m_strRootPath;
	list< ConnRec >												m_vclConnList;	//	连接数量不会很多，使用list应该不会有性能问题
	volatile bool												m_bUpdating;
};

