#pragma once
//#include <memory>
//#include "CFBaseObj.hpp"
//#include "DPCommon.hpp"
class CDPFacade :
	public CBaseObj
{
public:
	CDPFacade() {};
	virtual ~CDPFacade() {};

	static std::shared_ptr<CDPFacade> Create();

	virtual bool Initialize() = 0;
	virtual bool GetUpdateRegionByTile(BUILDING_BLOCK_ID enBuildingBlockID, uint32_t uiPackedTileID, vector< string > &vstrUpdateRegionList) = 0;

};

