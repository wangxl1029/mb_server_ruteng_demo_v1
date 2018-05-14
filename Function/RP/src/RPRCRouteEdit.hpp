#pragma once

//	class CRPRCCmdRouteEditResult
class CRPRCRouteEditResult : public CBaseObj
{
public:
	CRPRCRouteEditResult();
	virtual ~CRPRCRouteEditResult();

public:
	SmartPointer< CRPSection >									m_spclSection;
};


class CRPRCRouteEdit : public CBaseObj
{
public:
	CRPRCRouteEdit(//RPRCCmdSigReciever hReciever, RPRC_Cmd_PRI enCmdPRI,
		long lRouteID, //SmartPointer< CRPRCExtIF > spclExtIF,
		CRPWayPoint &clStartWayPoint, CRPWayPoint &clEndWayPoint, int iConnectLevel, vector< int > &viLevelList,
		SmartPointer< vector< CRPRSLink > > spvclStartTermSearchResultLinkList,
		SmartPointer< vector< CRPRSLink > > spvclEndTermSearchResultLinkList,
		vector< SmartPointer< RPRC_MidLinkTable > > &vspclStartTermMidLinkTable,
		vector< SmartPointer< RPRC_MidLinkTable > > &vspclEndTermMidLinkTable,
		SmartPointer< RPRCTileContainer< CRPRCMidLinkUsingTile > > spclMidLinkUsingContainer,
		SmartPointer< vector< CRPRCConnectSearchResultLink > > spvclStartTermConnectSearchResultLinkList,
		SmartPointer< vector< CRPRCConnectSearchResultLink > > spvclEndTermConnectSearchResultLinkList,
		SmartPointer< CDPFacade > spclDataProvider);
	virtual ~CRPRCRouteEdit();

	virtual RESULT Execute();

	SmartPointer< CRPRCRouteEditResult > GetResult();

public:
	RESULT RouteEdit(CRPWayPoint &clStartWayPoint, CRPWayPoint &clEndWayPoint, int iConnectLevel, vector< int > &viLevelList,
		SmartPointer< vector< CRPRSLink > > spvclStartTermSearchResultLinkList,
		SmartPointer< vector< CRPRSLink > > spvclEndTermSearchResultLinkList,
		vector< SmartPointer< RPRC_MidLinkTable > > &vspclStartTermMidLinkTable,
		vector< SmartPointer< RPRC_MidLinkTable > > &vspclEndTermMidLinkTable,
		SmartPointer< RPRCTileContainer< CRPRCMidLinkUsingTile > > spclMidLinkUsingContainer,
		SmartPointer< vector< CRPRCConnectSearchResultLink > > spvclStartTermConnectSearchResultLinkList,
		SmartPointer< vector< CRPRCConnectSearchResultLink > > spvclEndTermConnectSearchResultLinkList,
		SmartPointer< CDPFacade > spclDataProvider,
		SmartPointer< CRPRCRouteEditResult > &spclResult);

	CRPRCMidLink* GetPrevLink(CRPRCMidLink *pclLink);

public:
	//	input
	CRPWayPoint													m_clStartWayPoint;
	CRPWayPoint													m_clEndWayPoint;
	int															m_iConnectLevel;
	vector< int >												m_viLevelList;

	SmartPointer< vector< CRPRSLink > >							m_spvclStartTermSearchResultLinkList;
	SmartPointer< vector< CRPRSLink > >							m_spvclEndTermSearchResultLinkList;
	vector< SmartPointer< RPRC_MidLinkTable > >					m_vspclStartTermMidLinkTable;
	vector< SmartPointer< RPRC_MidLinkTable > >					m_vspclEndTermMidLinkTable;
	SmartPointer< RPRCTileContainer< CRPRCMidLinkUsingTile > >		m_spclMidLinkUsingContainer;
	SmartPointer< vector< CRPRCConnectSearchResultLink > >		m_spvclStartConnectSearchResultLinkList;
	SmartPointer< vector< CRPRCConnectSearchResultLink > >		m_spvclEndConnectSearchResultLinkList;
	SmartPointer< CDPFacade >									m_spclDataProvider;

	//	output
	SmartPointer< CRPRCRouteEditResult >						m_spclResult;
};

