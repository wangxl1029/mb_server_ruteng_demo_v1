#include	"InnerCommon.hpp"
#include	"DPCommon.hpp"
#include	"DPDBConnectionPool.hpp"
#include	"DPData.hpp"
#include	"DPDataShared.hpp"

#include	"DPProv.hpp"
#include	"DPProvShared.hpp"

#include	"DPProvFolderUpdateRegion.hpp"

RESULT CDPProvFolderUpdateRegion::Initialize(string strProductName, string strUpdateRegion, SmartPointer< CDPDBConnectionPool > spclDBConnectionPool)
{
	CCFLocker<CCFMutex>		clLock(m_clMutex);

	m_spclDBConnectionPool = spclDBConnectionPool;
	m_strProductName = strProductName;
	m_strUpdateRegion = strUpdateRegion;

	if (m_spclDPProvShared == NULL) {
		if (!m_spclDPProvShared.Create()) {
			ERR("");
			return FAILURE;
		}

		if (SUCCESS != m_spclDPProvShared->Initialize(m_strProductName, m_strUpdateRegion, m_spclDBConnectionPool)) {
			ERR("");
			return FAILURE;
		}
	}

	return SUCCESS;
}