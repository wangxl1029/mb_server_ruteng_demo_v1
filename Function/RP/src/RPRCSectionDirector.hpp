#pragma once

// class name CRPRCSectionDirector is not changed from znavi, easy to differ.
class CRPRCSectionDirector :
	public CBaseObj
{
public:
	CRPRCSectionDirector();
	virtual ~CRPRCSectionDirector();
	RESULT Initialize(SmartPointer< CDPFacade > spclDataProvider, SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > > spclLinkCostContainer);
	RESULT StartCalculateSection(CRPRouteCalcRequest &clRequest);

public:
	SmartPointer< CDPFacade >									m_spclDataProvider;
	//SmartPointer< CRPRCThreadPool >								m_spclThreadPool;
	//boost::signals2::connection									m_clThreadPoolConnection;
	SmartPointer< CRPRCCost >									m_spclCost;
	SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > >		m_spclLinkCostContainer;
	//SmartPointer< CRPRCExtIF >									m_spclExtIF;

	vector< int >												m_viLevelList;

	//	Mutex
	CCFMutex													m_clMutex;

	volatile bool												m_bCancelFlag;


	//	Request
	CRPRouteCalcRequest											m_clRequest;
	//CRPRCCalcParam												m_clParam;
	//long														m_lRouteID;
	CRPWayPoint													m_clStartWayPoint;
	CRPWayPoint													m_clEndWayPoint;

	//	Mid Data
	SmartPointer< RPRCTileContainer< CRPRCMidLinkUsingTile > >	m_spclMidLinkUsingContainer;

	// so-called command
	SmartPointer< CRPRCTermSearch >								m_spclStartTermSearch;
	SmartPointer< CRPRCTermSearch >								m_spclEndTermSearch;
private:
	//std::shared_ptr<CRPRCTermSearch> m_termSearch;
	SmartPointer<CBaseObj> m_upSearch;
	SmartPointer<CBaseObj> m_connectSearch;
	SmartPointer<CBaseObj> m_routeEdit;
};

