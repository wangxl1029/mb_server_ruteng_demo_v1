#include	"InnerCommon.hpp"
#include	"DPCommon.hpp"
#include	"DPDBConnectionPool.hpp"

#define		DP_CONNECTION_CACHE_COUNT							(20)

struct DPNDSFileNameRec_t {
	DPDB_TYPE				enDBType;
	char					*pcFileName;
};

static const DPNDSFileNameRec_t	s_astNDSFileNameList[] = {
	{ DPDB_ROOT, "ROOT.NDS" },
	{ DPDB_PRODUCT, "PRODUCT.NDS" },
	{ DPDB_SHARED, "OVERALL.NDS" },
	{ DPDB_BMD, "BMD.NDS" },
	{ DPDB_ROUTING, "ROUTING.NDS" },
	{ DPDB_NAME, "NAME.NDS" },
#ifdef NEU_DB
	{ DPDB_3DOBJ, "3D.NDS" },
#else
	{ DPDB_3DOBJ, "OBJ3D.NDS" },
#endif
	{ DPDB_DRAWPARAMETER, "STYLE.DAT" },
};
//	CDPConnRec
CDPConnRec::CDPConnRec()
{
}

CDPConnRec::~CDPConnRec()
{
	//m_spclConnctionPool->ReleaseConnection(m_spclDatabase);
}

bool CDPConnRec::Connect(std::shared_ptr< CDPDBConnectionPool > spclConnctionPool, const char *pcProductName, const char *pcUpdateRegion, DPDB_TYPE enDBType)
{
	m_spclConnctionPool = spclConnctionPool;
#if WXL_CLIENT_SPECIFIED
	return m_spclConnctionPool->GetConnection(pcProductName, pcUpdateRegion, enDBType, m_spclDatabase);
#elif defined(WXL_SERVER_SPECIFIED)
	return (SUCCESS ==  m_spclConnctionPool->GetConnection(pcProductName, pcUpdateRegion, enDBType, m_spclDatabase))
		? true 
		: false;
#else
	return false;
#endif
}

CRCPtr< CSL_Database >& CDPConnRec::Get()
{
	return m_spclDatabase;
}

//	CDPDBConnectionPool
bool CDPDBConnectionPool::Initialize(const char *pcRootPath)
{
	m_strRootPath = pcRootPath;
	return true;
}

RESULT CDPDBConnectionPool::GetConnection(const char *pcProductName, const char *pcUpdateRegion, DPDB_TYPE enDBType, CRCPtr< CSL_Database > &spDatabase)
{
	//	Need NOT lock
	string	strFileName;
	if (SUCCESS != GetDataFilePath(pcProductName, pcUpdateRegion, enDBType, strFileName)) {
		ERR("");
		return FAILURE;
	}

	if (SUCCESS != GetConnection(strFileName.c_str(), spDatabase)) {
		ERR("");
		return FAILURE;
	}

	return SUCCESS;
}

RESULT CDPDBConnectionPool::GetConnection(const char *strFileName, CRCPtr< CSL_Database > &spDatabase)
{
	//CCFMutexLocker	clLock(m_clMutex); //--> del by wxl
	for (list< ConnRec >::iterator it = m_vclConnList.begin(); it != m_vclConnList.end(); ++it) {
		if (it->m_strFileName == strFileName) {
			it->m_iRefCnt++;
			spDatabase = it->m_spclDatabase;
			//	Move to list head
			m_vclConnList.push_front(*it);
			m_vclConnList.erase(it);
			return SUCCESS;
		}
	}

	if (SUCCESS != SL_CreateDatabaseObj(&spDatabase)) {
		ERR("");
		return FAILURE;
	}
	string	strFullPath = m_strRootPath + DP_DATA_DIR_NAME + "/" + strFileName;
	if (SUCCESS != spDatabase->ConnectDB(strFullPath.c_str())) {
		ERR("");
		return FAILURE;
	}

	if (SUCCESS != AttachBackupFiles(strFileName, spDatabase)) {
		ERR("");
		return FAILURE;
	}

	if (m_bUpdating) {
		if (SUCCESS != AttachUpdateFiles(strFileName, spDatabase)) {
			ERR("");
			return FAILURE;
		}
	}

	m_vclConnList.push_front(ConnRec(strFileName, spDatabase));
	ReduceConnection();

	return SUCCESS;
}


RESULT CDPDBConnectionPool::ReleaseConnection(CRCPtr< CSL_Database > &spDatabase)
{
	//CCFMutexLocker	clLock(m_clMutex);	//---> del by wxl

	for (list< ConnRec >::iterator it = m_vclConnList.begin(); it != m_vclConnList.end(); ++it) {
		if (it->m_spclDatabase == spDatabase) {
			if (--it->m_iRefCnt == 0) {
				//	Move to list tail
				m_vclConnList.insert(m_vclConnList.end(), *it);
				m_vclConnList.erase(it);
				ReduceConnection();
			}
			return SUCCESS;
		}
	}

	ERR("");
	return FAILURE;
}

RESULT CDPDBConnectionPool::ReduceConnection()
{
	list< ConnRec >::iterator	it = m_vclConnList.begin();
	while (m_vclConnList.size() > DP_CONNECTION_CACHE_COUNT && it != m_vclConnList.end()) {
		if (it->m_iRefCnt == 0) {
			it = m_vclConnList.erase(it);
		}
		else {
			++it;
		}
	}

	return SUCCESS;
}

RESULT CDPDBConnectionPool::GetRootPath(string &strRootPath)
{
	strRootPath = m_strRootPath;
	return SUCCESS;
}

RESULT CDPDBConnectionPool::GetDataFilePath(const char *pcProductName, const char *pcUpdateRegion, DPDB_TYPE enDBType, string &strDataFilePath)
{
	strDataFilePath = "";
	if (pcProductName != NULL) {
		strDataFilePath += pcProductName;
		strDataFilePath += "/";
	}
	if (pcUpdateRegion != NULL) {
		strDataFilePath += pcUpdateRegion;
		strDataFilePath += "/";
	}
	strDataFilePath += s_astNDSFileNameList[enDBType].pcFileName;

	return SUCCESS;
}

RESULT CDPDBConnectionPool::GetDataFileFullPath(const char *pcProductName, const char *pcUpdateRegion, DPDB_TYPE enDBType, string &strDataFileFullPath)
{
	string	strDataFilePath;
	if (SUCCESS != GetDataFilePath(pcProductName, pcUpdateRegion, enDBType, strDataFilePath)) {
		ERR("");
		return FAILURE;
	}

	strDataFileFullPath = m_strRootPath + DP_DATA_DIR_NAME + "/" + strDataFilePath;

	return SUCCESS;
}


RESULT CDPDBConnectionPool::AttachUpdateFiles(const char *strFileName, CRCPtr< CSL_Database > &spDatabase)
{
	string	strInsUpdFile = m_strRootPath + DP_UNPACKED_DIR_NAME + "/" + strFileName + DP_INSUPD_DB_SUFFIX;
	string	strDeleteFile = m_strRootPath + DP_UNPACKED_DIR_NAME + "/" + strFileName + DP_DELETE_DB_SUFFIX;
	if (PF_IsFileExisting(strInsUpdFile.c_str()) || (PF_IsFileExisting(strDeleteFile.c_str()))) {
		//	todo: create dir here

		CRCPtr<CSL_Updater>	spclUpdater = spDatabase->GetUpdater();
		string	strBackupFile = m_strRootPath + DP_BACKUP_DIR_NAME + "/" + strFileName + DP_BACKUP_DB_SUFFIX;
		string	strExcludeRidFile = m_strRootPath + DP_BACKUP_DIR_NAME + "/" + strFileName + DP_EXCLUDE_DB_SUFFIX;
		if (SUCCESS != spclUpdater->AttachBackupDB(strBackupFile.c_str())) {
			ERR("");
			return FAILURE;
		}
		if (SUCCESS != spclUpdater->AttachExcludeRidDB(strExcludeRidFile.c_str())) {
			ERR("");
			return FAILURE;
		}
		if (PF_IsFileExisting(strInsUpdFile.c_str())) {
			string	strInsUpdRidFile = m_strRootPath + DP_UNPACKED_DIR_NAME + "/" + strFileName + DP_INSUPD_RID_DB_SUFFIX;
			if (SUCCESS != spclUpdater->AttachInsUpdDB(strInsUpdFile.c_str())) {
				ERR("");
				return FAILURE;
			}
			if (SUCCESS != spclUpdater->AttachInsUpdRidDB(strInsUpdRidFile.c_str())) {
				ERR("");
				return FAILURE;
			}
		}
		if (PF_IsFileExisting(strDeleteFile.c_str())) {
			string	strDeleteRidFile = m_strRootPath + DP_UNPACKED_DIR_NAME + "/" + strFileName + DP_DELETE_RID_DB_SUFFIX;
			string	strDeleteRidDupFile = m_strRootPath + DP_UNPACKED_DIR_NAME + "/" + strFileName + DP_DELETE_RID_DUP_DB_SUFFIX;
			if (SUCCESS != spclUpdater->AttachDeleteDB(strDeleteFile.c_str())) {
				ERR("");
				return FAILURE;
			}
			if (SUCCESS != spclUpdater->AttachDeleteRidDB(strDeleteRidFile.c_str())) {
				ERR("");
				return FAILURE;
			}
			if (SUCCESS != spclUpdater->AttachDeleteRidDupDB(strDeleteRidDupFile.c_str())) {
				ERR("");
				return FAILURE;
			}
		}
	}

	return SUCCESS;
}

RESULT CDPDBConnectionPool::DetachUpdateFiles(const char * /*strFileName*/, CRCPtr< CSL_Database > &spDatabase)
{
	CRCPtr<CSL_Updater>	spclUpdater = spDatabase->GetUpdater();
	if (spclUpdater == NULL) {
		return SUCCESS;
	}

	if (SUCCESS != spclUpdater->DetachBackupDB()) {
		ERR("");
		return FAILURE;
	}
	if (SUCCESS != spclUpdater->DetachExcludeRidDB()) {
		ERR("");
		return FAILURE;
	}
	if (SUCCESS != spclUpdater->DetachInsUpdDB()) {
		ERR("");
		return FAILURE;
	}
	if (SUCCESS != spclUpdater->DetachInsUpdRidDB()) {
		ERR("");
		return FAILURE;
	}
	if (SUCCESS != spclUpdater->DetachDeleteDB()) {
		ERR("");
		return FAILURE;
	}
	if (SUCCESS != spclUpdater->DetachDeleteRidDB()) {
		ERR("");
		return FAILURE;
	}
	if (SUCCESS != spclUpdater->DetachDeleteRidDupDB()) {
		ERR("");
		return FAILURE;
	}

	return SUCCESS;
}

RESULT CDPDBConnectionPool::AttachBackupFiles(const char *strFileName, CRCPtr< CSL_Database > &spDatabase)
{
	string	strBackupFile = m_strRootPath + DP_BACKUP_DIR_NAME + "/" + strFileName + DP_BACKUP_DB_SUFFIX;
	string	strExcludeRidFile = m_strRootPath + DP_BACKUP_DIR_NAME + "/" + strFileName + DP_EXCLUDE_DB_SUFFIX;
	if (PF_IsFileExisting(strBackupFile.c_str())) {
		CRCPtr<CSL_Updater>	spclUpdater = spDatabase->GetUpdater();
		if (SUCCESS != spclUpdater->AttachBackupDB(strBackupFile.c_str())) {
			ERR("");
			return FAILURE;
		}
		if (SUCCESS != spclUpdater->AttachExcludeRidDB(strExcludeRidFile.c_str())) {
			ERR("");
			return FAILURE;
		}
		// 		if( SUCCESS != spDatabase->AttachBackupDB( strBackupFile.c_str(), strExcludeRidFile.c_str() ) ) {
		// 			ERR( "" );
		// 			return FAILURE;
		// 		}
	}

	return SUCCESS;
}

RESULT CDPDBConnectionPool::DetachBackupFiles(const char *strFileName, CRCPtr< CSL_Database > &spDatabase)
{
	CRCPtr<CSL_Updater>	spclUpdater = spDatabase->GetUpdater();
	if (spclUpdater == NULL) {
		return SUCCESS;
	}

	if (SUCCESS != spclUpdater->DetachBackupDB()) {
		ERR("");
		return FAILURE;
	}
	if (SUCCESS != spclUpdater->DetachExcludeRidDB()) {
		ERR("");
		return FAILURE;
	}

	return SUCCESS;
}
