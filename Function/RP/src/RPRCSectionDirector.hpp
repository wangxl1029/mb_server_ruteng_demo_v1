#pragma once

// class name CRPRCSectionDirector is not changed from znavi, easy to differ.
class CRPRCSectionDirector :
	public CBaseObj
{
public:
	CRPRCSectionDirector();
	virtual ~CRPRCSectionDirector();
	RESULT Initialize(SmartPointer< CDPFacade > spclDataProvider);
	RESULT StartCalculateSection(CRPRouteCalcRequest &clRequest);

public:
	SmartPointer< CDPFacade >									m_spclDataProvider;
	SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > >		m_spclLinkCostContainer;

private:
	//std::shared_ptr<CRPRCTermSearch> m_termSearch;
	SmartPointer<CBaseObj> m_upSearch;
	SmartPointer<CBaseObj> m_connectSearch;
	SmartPointer<CBaseObj> m_routeEdit;

	//	Request
	CRPRouteCalcRequest											m_clRequest;
	//CRPRCCalcParam												m_clParam;
	CRPWayPoint													m_clStartWayPoint;
	CRPWayPoint													m_clEndWayPoint;

	SmartPointer< CRPRCTermSearch >								m_spclStartTermSearch;
	SmartPointer< CRPRCTermSearch >								m_spclEndTermSearch;
};

