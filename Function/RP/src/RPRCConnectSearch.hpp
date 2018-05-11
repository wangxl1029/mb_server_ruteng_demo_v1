#pragma once
class CRPRCConnectSearchResult : public CBaseObj
{
public:
	CRPRCConnectSearchResult();
	virtual ~CRPRCConnectSearchResult();

public:
	SmartPointer< vector< CRPRCConnectSearchResultLink > >		m_spvclConnectedLinkList;
};
class CRPRCConnectSearch : public CBaseObj
{
public:
	CRPRCConnectSearch();
public:
	CRPRCConnectSearch(//RPRCCmdSigReciever hReciever, RPRC_Cmd_PRI enCmdPRI,
		long lRouteID,// SmartPointer< CRPRCExtIF > spclExtIF,
		RP_TERM enTerm, int iLevel, CRPWayPoint &clStartWayPoint, CRPWayPoint &clEndWayPoint,
		SmartPointer< RPRC_OpenTable > spmapOpenTable,
		SmartPointer< RPRC_MidLinkTable > spclMidLinkTable,
		SmartPointer< RPRCTileContainer< CRPRCMidLinkUsingTile > > spclMidLinkUsingContainer,
		SmartPointer< CRPRCCost > spclCost,
		SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > > spclLinkCostContainer,
		SmartPointer< CDPFacade > spclDataProvider);
	virtual ~CRPRCConnectSearch();

	virtual RESULT Execute();

	SmartPointer< CRPRCConnectSearchResult > GetResult();

public:
	RESULT ConnectSearch(RP_TERM enTerm, int iLevel, CRPWayPoint &clStartWayPoint, CRPWayPoint &clEndWayPoint,
		SmartPointer< RPRC_OpenTable > spmapOpenTable,
		SmartPointer< RPRC_MidLinkTable > spvclMidLinkTable,
		SmartPointer< RPRCTileContainer< CRPRCMidLinkUsingTile > > spclMidLinkUsingContainer,
		SmartPointer< CRPRCCost > spclCost,
		SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > > spclLinkCostContainer,
		SmartPointer< CDPFacade > spclDataProvider,
		SmartPointer< CRPRCConnectSearchResult > &spclResult);

	class CCheckEndParam
	{
	public:
		CCheckEndParam(RP_TERM &enTerm, int &iLevel,
			RPRC_OpenTable &mapOpenTable,
			size_t &uiExtendedLinkCount,
			vector< CRPRCConnectSearchResultLink > &vclConnectedLinkList)
			: m_enTerm(enTerm),
			m_iLevel(iLevel),
			m_mapOpenTable(mapOpenTable),
			m_uiExtendedLinkCount(uiExtendedLinkCount),
			m_vclConnectedLinkList(vclConnectedLinkList)
		{
		}

		RP_TERM													&m_enTerm;
		int														&m_iLevel;
		RPRC_OpenTable											&m_mapOpenTable;
		size_t													&m_uiExtendedLinkCount;
		vector< CRPRCConnectSearchResultLink >					&m_vclConnectedLinkList;
	};

	bool CheckEnd(CCheckEndParam &p);

	class CExtendParam
	{
	public:
		CExtendParam(RP_TERM &enTerm, int &iLevel, uint &uiCoordShift,
			RPRC_OpenTable &mapOpenTable,
			RPRC_MidLinkTable &clMidLinkTable,
			CRPRCMidLinkUsingTableProxy &clMidLinkUsingTableProxy,
			CRPRCRoutingTileProxy &clInLinkRoutingTile,
			CRPRCRoutingTileProxy &clNodeRoutingTile,
			CRPRCRoutingTileProxy &clOutLinkRoutingTile,
			CRPRCCost &clCost,
			CRPRCLinkCostTableProxy &clLinkCostTableProxy,
			SmartPointer< CDPFacade > &spclDataProvider,
			vector< CRPRCConnectSearchResultLink > &vclConnectedLinkList
			/*, SmartPointer< CRPRCExtIF > &spclExtIF*/)
			: m_enTerm(enTerm),
			m_iLevel(iLevel),
			m_uiCoordShift(uiCoordShift),
			m_mapOpenTable(mapOpenTable),
			m_clMidLinkTable(clMidLinkTable),
			m_clMidLinkUsingTableProxy(clMidLinkUsingTableProxy),
			m_clInLinkRoutingTile(clInLinkRoutingTile),
			m_clNodeRoutingTile(clNodeRoutingTile),
			m_clOutLinkRoutingTile(clOutLinkRoutingTile),
			m_clCost(clCost),
			m_clLinkCostTableProxy(clLinkCostTableProxy),
			m_spclDataProvider(spclDataProvider),
			m_vclConnectedLinkList(vclConnectedLinkList)
			//,m_spclExtIF(spclExtIF)
		{
		}

		RP_TERM													&m_enTerm;
		int														&m_iLevel;
		uint													&m_uiCoordShift;
		RPRC_OpenTable											&m_mapOpenTable;
		RPRC_MidLinkTable											&m_clMidLinkTable;
		CRPRCMidLinkUsingTableProxy								&m_clMidLinkUsingTableProxy;
		CRPRCRoutingTileProxy										&m_clInLinkRoutingTile;
		CRPRCRoutingTileProxy										&m_clNodeRoutingTile;
		CRPRCRoutingTileProxy										&m_clOutLinkRoutingTile;
		CRPRCCost													&m_clCost;
		CRPRCLinkCostTableProxy									&m_clLinkCostTableProxy;
		SmartPointer< CDPFacade >								&m_spclDataProvider;
		vector< CRPRCConnectSearchResultLink >					&m_vclConnectedLinkList;
		//SmartPointer< CRPRCExtIF >								&m_spclExtIF;
	};

	RESULT Extend(CExtendParam &p);

public:
	//	input
	RP_TERM														m_enTerm;
	int															m_iLevel;
	CRPWayPoint													m_clStartWayPoint;
	CRPWayPoint													m_clEndWayPoint;

	SmartPointer< RPRC_OpenTable >								m_spmapOpenTable;
	SmartPointer< RPRC_MidLinkTable >								m_spclMidLinkTable;
	SmartPointer< RPRCTileContainer< CRPRCMidLinkUsingTile > >		m_spclMidLinkUsingContainer;
	SmartPointer< CRPRCCost >										m_spclCost;
	SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > >			m_spclLinkCostContainer;
	SmartPointer< CDPFacade >									m_spclDataProvider;

	//	output
	SmartPointer< CRPRCConnectSearchResult >					m_spclResult;
};

