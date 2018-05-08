#pragma once
#include "DPCommon.hpp"

class CDPProvProduct :
	public CBaseObj
{
public:
	CDPProvProduct() : m_bDbSwitching(false) {}
	virtual ~CDPProvProduct() {}

	bool Initialize(string strProductName, shared_ptr< CDPDBConnectionPool > spclDBConnectionPool);
	bool GetUpdateRegionByTile(BUILDING_BLOCK_ID enBuildingBlockID, uint32_t uiPackedTileID, vector< string > &vstrUpdateRegionList);

public:
	shared_ptr< CDPDBConnectionPool >							m_spclDBConnectionPool;
	string														m_strProductName;
	CDPConnRec													m_clDatabase;
	volatile bool													m_bDbSwitching;
};

