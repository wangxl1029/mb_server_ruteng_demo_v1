#include	"InnerCommon.hpp"
#include	"DPCommon.hpp"
#include	"DPDBConnectionPool.hpp"
#include	"DPData.hpp"
#include	"DPDataProduct.hpp"
#include	"DPDataShared.hpp"
//#include	"DPDataBmd.h"
#include	"DPDataRouting.hpp"
//#include	"DPDataNamedObject.h"
//#include	"DPData3DObj.h"
#include	"DPProv.hpp"
#include	"DPProvProduct.hpp"
#include	"DPProvShared.hpp"
//#include	"DPProvBmd.h"
#include	"DPProvRouting.hpp"
//#include	"DPProvNamedObject.h"
//#include	"DPProv3DObj.h"
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

RESULT CDPProvFolderProduct::GetProvFolderUpdateRegion(string strUpdateRegionName, SmartPointer< CDPProvFolderUpdateRegion > &spclDPProvFolderUpdateRegion)
{
	CCFLocker<CCFMutex>		clLock(m_clMutex);

	SmartPointer< CDPProvFolderUpdateRegion >	spclDPProvFolderUpdateRegionTemp = m_clDPProvFolderUpdateRegionCache.GetData(strUpdateRegionName);
	if (spclDPProvFolderUpdateRegionTemp == NULL) {
		if (!spclDPProvFolderUpdateRegionTemp.Create()) {
			ERR("");
			return FAILURE;
		}

		if (SUCCESS != spclDPProvFolderUpdateRegionTemp->Initialize(m_strProductName, strUpdateRegionName, m_spclDBConnectionPool)) {
			ERR("");
			return FAILURE;
		}
		m_clDPProvFolderUpdateRegionCache.PutData(strUpdateRegionName, spclDPProvFolderUpdateRegionTemp);
	}
	spclDPProvFolderUpdateRegion = spclDPProvFolderUpdateRegionTemp;

	return SUCCESS;
}
