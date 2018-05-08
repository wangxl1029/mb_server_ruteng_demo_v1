#pragma once
#include "DPCommon.hpp"

class CDPProvProduct :
	public CBaseObj
{
public:
	CDPProvProduct() : m_bDbSwitching(false) {}
	virtual ~CDPProvProduct() {}

	bool Initialize(std::string strProductName, std::shared_ptr< CDPDBConnectionPool > spclDBConnectionPool);
	bool GetUpdateRegionByTile(BUILDING_BLOCK_ID enBuildingBlockID, uint32_t uiPackedTileID, std::vector< std::string > &vstrUpdateRegionList);

public:
	std::shared_ptr< CDPDBConnectionPool >							m_spclDBConnectionPool;
	std::string														m_strProductName;
	CDPConnRec													m_clDatabase;
	volatile bool													m_bDbSwitching;
};

