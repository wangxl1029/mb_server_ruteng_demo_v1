#include "DPDBConnectionPool.hpp"


//	CDPDBConnectionPool
bool CDPDBConnectionPool::Initialize(const char *pcRootPath)
{
	m_strRootPath = pcRootPath;
	return true;
}
