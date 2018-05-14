#pragma once

// class name CRPRCSectionDirector is not changed from znavi, easy to differ.
class CRPRCSectionDirector :
	public CBaseObj
{
public:
	CRPRCSectionDirector();
	virtual ~CRPRCSectionDirector();
	RESULT Initialize(SmartPointer< CDPFacade > spclDataProvider, SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > > spclLinkCostContainer);
	RESULT StartCalculateSection(CRPRouteCalcRequest &clRequest, long lRouteID);

public:
	enum RouteCalcStep {
		STEP_TERM_SEARCH,
		STEP_UP_SEARCH,
		STEP_LINK_LEVEL_UP,
		STEP_CONNECT_SEARCH,
		STEP_ROUTE_EDIT,
		STEP_INVALID,
	};
	//void StepStart(); // del by wxl from znavi

	RESULT StepTermSearch();
	RESULT StepUpSearch(int iFromLevel, int iToLevel,
		SmartPointer< RPRC_OpenTable > spmapStartTermOpenTable,
		SmartPointer< RPRC_MidLinkTable > spclStartTermMidLinkTable,
		SmartPointer< RPRC_OpenTable > spmapEndTermOpenTable,
		SmartPointer< RPRC_MidLinkTable > spclEndTermMidLinkTable);
	RESULT StepLinkLevelUp(int iFromLevel, int iToLevel,
		SmartPointer< vector< CRPRCUpSearchResultLink > > spvclStartInputLinkList,
		SmartPointer< vector< CRPRCUpSearchResultLink > > spvclEndInputLinkList);
	RESULT StepConnectSearch(int iLevel,
		SmartPointer< RPRC_OpenTable > spmapStartTermOpenTable,
		SmartPointer< RPRC_MidLinkTable > spclStartTermMidLinkTable,
		SmartPointer< RPRC_OpenTable > spmapEndTermOpenTable,
		SmartPointer< RPRC_MidLinkTable > spclEndTermMidLinkTable);
	RESULT StepRouteEdit(int iConnectLevel,
		SmartPointer< vector< CRPRCConnectSearchResultLink > > spvclStartTermConnectResultLinkList,
		SmartPointer< vector< CRPRCConnectSearchResultLink > > spvclEndTermConnectResultLinkList);

	int DecideConnectLevel(CRPWayPoint &clStartWayPoint,
		CRPWayPoint &clEndWayPoint);

	RESULT NextStep();

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
	long														m_lRouteID;
	CRPWayPoint													m_clStartWayPoint;
	CRPWayPoint													m_clEndWayPoint;

	//	Mid Data
	SmartPointer< RPRCTileContainer< CRPRCMidLinkUsingTile > >	m_spclMidLinkUsingContainer;

	// so-called command
	SmartPointer< CRPRCTermSearch >								m_spclStartTermSearch;
	SmartPointer< CRPRCTermSearch >								m_spclEndTermSearch;
	vector< SmartPointer< CRPRCUpSearch > >						m_vspclStartUpSearchList;
	vector< SmartPointer< CRPRCUpSearch > >						m_vspclEndUpSearchList;
	vector< SmartPointer< CRPRCLinkLevelUp > >					m_vspclStartLinkLevelUpList;
	vector< SmartPointer< CRPRCLinkLevelUp > >					m_vspclEndLinkLevelUpList;
	vector< SmartPointer< CRPRCLinkLevelUpResult > >			m_vspclStartLinkLevelUpResult;
	vector< SmartPointer< CRPRCLinkLevelUpResult > >			m_vspclEndLinkLevelUpResult;
	SmartPointer< CRPRCConnectSearch >							m_spclStartConnectSearch;
	SmartPointer< CRPRCConnectSearch >							m_spclEndConnectSearch;
	SmartPointer< CRPRCRouteEdit >								m_spclRouteEdit;

	//	Executing Command
	RouteCalcStep												m_enStep;

	//	Mid Result
	SmartPointer< CRPRCTermSearchResult >						m_spclStartTermSearchResult;
	SmartPointer< CRPRCTermSearchResult >						m_spclEndTermSearchResult;
	vector< SmartPointer< CRPRCUpSearchResult > >				m_vspclStartUpSearchResult;
	vector< SmartPointer< CRPRCUpSearchResult > >				m_vspclEndUpSearchResult;
	SmartPointer< CRPRCConnectSearchResult >					m_spclStartConnectSearchResult;
	SmartPointer< CRPRCConnectSearchResult >					m_spclEndConnectSearchResult;
	SmartPointer< CRPRCRouteEditResult >						m_spclRouteEditResult;

	//	Result
	SmartPointer< CRPSection >									m_spclSection;

public:
	static volatile long										m_iNextSectionID;
};

