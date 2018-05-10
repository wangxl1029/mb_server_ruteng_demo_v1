#pragma once

class CRPRCTermSearch :
	public CBaseObj
{
public:
	CRPRCTermSearch(CRPWayPoint &clWayPoint, int iLevel, SmartPointer< CDPFacade > spclDataProvider);
	CRPRCTermSearch() = default;
	bool Do(SmartPointer< CDPFacade > spclDataProvide);
	virtual ~CRPRCTermSearch();
public:
	//	input
	CRPWayPoint													m_clWayPoint;
	int															m_iLevel;

	SmartPointer< CDPFacade >									m_spclDataProvider;
};

