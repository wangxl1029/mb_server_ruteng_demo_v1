#include	"InnerCommon.hpp"
#include	"DPCommon.hpp"
#include	"DPDBConnectionPool.hpp"
#include	"DPData.hpp"
#include	"DPProv.hpp"
#include	"DPProvRoot.hpp"

RESULT CDPProvRoot::Initialize(SmartPointer< CDPDBConnectionPool > spclDBConnectionPool)
{
	m_spclDBConnectionPool = spclDBConnectionPool;

	if (m_spclDBConnectionPool == NULL) {
		ERR("");
		return FAILURE;
	}

	if (SUCCESS != m_clDatabase.Connect(m_spclDBConnectionPool, NULL, NULL, DPDB_ROOT)) {
		ERR("");
		return FAILURE;
	}

	return SUCCESS;
}