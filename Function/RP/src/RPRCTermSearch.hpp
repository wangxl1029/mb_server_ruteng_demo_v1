#pragma once

class CRPRCTermSearch :
	public CBaseObj
{
public:
	CRPRCTermSearch(CRPWayPoint &clWayPoint, int iLevel, SmartPointer< CDPFacade > spclDataProvider);
	CRPRCTermSearch() = default;

	virtual RESULT Execute();
	virtual ~CRPRCTermSearch();

public:
	RESULT TermSearch(int iLevel, SmartPointer< CDPFacade > spclDataProvider);

public:
	//	input
	CRPWayPoint													m_clWayPoint;
	int															m_iLevel;

	SmartPointer< CDPFacade >									m_spclDataProvider;
};

