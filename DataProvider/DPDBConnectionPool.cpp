#include	"InnerCommon.hpp"
#include	"DPCommon.hpp"
#include	"DPDBConnectionPool.hpp"

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
	//m_spclConnctionPool = spclConnctionPool;
	//return m_spclConnctionPool->GetConnection(pcProductName, pcUpdateRegion, enDBType, m_spclDatabase);
	return false;
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
