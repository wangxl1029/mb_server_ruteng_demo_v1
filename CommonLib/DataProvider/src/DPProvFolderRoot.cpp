#include	"InnerCommon.hpp"
#include	"DPCommon.hpp"
#include	"DPDBConnectionPool.hpp"
#include	"DPData.hpp"
//#include	"DPDataRoot.hpp"
#include	"DPDataShared.hpp"
#include	"DPDataProduct.hpp"

#include	"DPProv.hpp"
#include	"DPProvRoot.hpp"
#include	"DPProvShared.hpp"
#include	"DPProvProduct.hpp"

#include	"DPProvFolderUpdateRegion.hpp"
#include	"DPProvFolderProduct.hpp"
#include	"DPProvFolderRoot.hpp"


RESULT CDPProvFolderRoot::Initialize(SmartPointer< CDPDBConnectionPool > spclDBConnectionPool)
{
	CCFLocker<CCFMutex>		clLock(m_clMutex);

	m_spclDBConnectionPool = spclDBConnectionPool;

	//	if( !m_spclProvDrawParameter.Create() ) {
	//		ERR( "" );
	//		return FAILURE;
	//	}
	//	if( SUCCESS != m_spclProvDrawParameter->Initialize( spclDBConnectionPool ) ) {
	//		ERR( "" );
	//		return FAILURE;
	//	}

	m_clDPProvFolderProductCache.SetCapacity(10);

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

RESULT CDPProvFolderRoot::GetProvRoot(SmartPointer< CDPProvRoot > &spclDPProvRoot)
{
	CCFLocker<CCFMutex>		clLock(m_clMutex);

	if (m_spclDPProvRoot == NULL) {
		SmartPointer< CDPProvRoot >	spclDPProvRootTemp;
		if (!spclDPProvRootTemp.Create()) {
			ERR("");
			return FAILURE;
		}

		if (SUCCESS != spclDPProvRootTemp->Initialize(m_spclDBConnectionPool)) {
			ERR("");
			return FAILURE;
		}
		m_spclDPProvRoot = spclDPProvRootTemp;
	}
	spclDPProvRoot = m_spclDPProvRoot;
	return SUCCESS;
}

RESULT CDPProvFolderRoot::GetFolderProduct(string strProductName, SmartPointer< CDPProvFolderProduct > &spclFolderProduct)
{
	if (m_bDbSwitching) {
		ERR("");
		return FAILURE;
	}

	CCFLocker<CCFMutex>		clLock(m_clMutex);

	spclFolderProduct = m_clDPProvFolderProductCache.GetData(strProductName);
	if (spclFolderProduct == NULL) {
		SmartPointer< CDPProvFolderProduct >	spclFolderProductTemp;
		if (!spclFolderProductTemp.Create()) {
			ERR("");
			return FAILURE;
		}
		if (spclFolderProductTemp->Initialize(strProductName, m_spclDBConnectionPool)) {
			ERR("");
			return FAILURE;
		}

		m_clDPProvFolderProductCache.PutData(strProductName, spclFolderProductTemp);
		spclFolderProduct = spclFolderProductTemp;
	}
	return SUCCESS;
}

