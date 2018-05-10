#include	"InnerCommon.hpp"
#include	"DPCommon.hpp"
#include	"DPDBConnectionPool.hpp"
#include	"DPData.hpp"
#include	"DPDataShared.hpp"
//#include	"DPDataBmd.h"
#include	"DPDataRouting.hpp"
//#include	"DPDataNamedObject.h"
//#include	"DPData3DObj.h"
#include	"DPProv.hpp"
#include	"DPProvShared.hpp"
//#include	"DPProvBmd.h"
#include	"DPProvRouting.hpp"
//#include	"DPProvNamedObject.h"
//#include	"DPProv3DObj.h"
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

RESULT CDPProvFolderUpdateRegion::GetProvShared(SmartPointer< CDPProvShared > &spclProvShared)
{
	//	Need NOT lock
	spclProvShared = m_spclDPProvShared;
	return SUCCESS;
}

RESULT CDPProvFolderUpdateRegion::GetProvRouting(SmartPointer< CDPProvRouting > &spclProvRouting)
{
	CCFLocker<CCFMutex>		clLock(m_clMutex);

	if (m_spclDPProvRouting == NULL) {
		if (!m_spclDPProvRouting.Create()) {
			ERR("");
			return FAILURE;
		}

		if (SUCCESS != m_spclDPProvRouting->Initialize(m_strProductName, m_strUpdateRegion, m_spclDBConnectionPool)) {
			ERR("");
			return FAILURE;
		}
	}
	spclProvRouting = m_spclDPProvRouting;
	return SUCCESS;
}


