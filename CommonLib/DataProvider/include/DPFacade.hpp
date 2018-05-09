#pragma once
class CDPFacade :
	public CBaseObj
{
public:
	CDPFacade() {};
	virtual ~CDPFacade() {};

	static SmartPointer< CDPFacade > Create();

	virtual RESULT Initialize() = 0;

	virtual RESULT GetUpdateRegionByTile(BUILDING_BLOCK_ID enBuildingBlockID, uint uiPackedTileID, vector< string > &vstrUpdateRegionList) = 0;

};

