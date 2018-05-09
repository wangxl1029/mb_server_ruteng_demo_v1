#include	"InnerCommon.hpp"
#include	"DPCommon.hpp"
#include	"DPDBConnectionPool.hpp"
#include	"DPData.hpp"
#include	"DPDataShared.hpp"
#include	"DPDataProduct.hpp"
// ignored headers
#include	"DPProv.hpp"
#include	"DPProvProduct.hpp"
#include	"DPProvShared.hpp"
// ignored headers
#include	"DPProvFolderUpdateRegion.hpp"
#include	"DPProvFolderProduct.hpp"

RESULT CDPProvFolderProduct::Initialize(string strProductName, SmartPointer< CDPDBConnectionPool > spclDBConnectionPool)
{
	CCFLocker<CCFMutex>		clLock(m_clMutex);

	m_spclDBConnectionPool = spclDBConnectionPool;
	m_strProductName = strProductName;

	m_clDPProvFolderUpdateRegionCache.SetCapacity(10);

	return SUCCESS;
}

RESULT CDPProvFolderProduct::GetProvProduct(SmartPointer< CDPProvProduct > &spclDPProvProduct)
{
	if (m_bDbSwitching) {
		ERR("");
		return FAILURE;
	}

	CCFLocker<CCFMutex>		clLock(m_clMutex);

	if (m_spclDPProvProduct == NULL) {
		SmartPointer< CDPProvProduct >	spclDPProvProductTemp;
		if (!spclDPProvProductTemp.Create()) {
			ERR("");
			return FAILURE;
		}

		if (SUCCESS != spclDPProvProductTemp->Initialize(m_strProductName, m_spclDBConnectionPool)) {
			ERR("");
			return FAILURE;
		}
		m_spclDPProvProduct = spclDPProvProductTemp;
	}
	spclDPProvProduct = m_spclDPProvProduct;
	return SUCCESS;
}