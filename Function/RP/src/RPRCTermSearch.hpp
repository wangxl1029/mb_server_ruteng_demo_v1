#pragma once

//	class CRPRCCmdTermSearchResult
class CRPRCTermSearchResult : public CBaseObj
{
public:
	CRPRCTermSearchResult();
	virtual ~CRPRCTermSearchResult();

public:
//	SmartPointer< vector< CRPRSLink > >							m_spvclResultLinkList;
	SmartPointer< RPRC_OpenTable >								m_spmapOpenTable;
	SmartPointer< RPRC_MidLinkTable >							m_spclMidLinkTable;
//	SmartPointer< vector< CRPRCConnectSearchResultLink > >		m_spvclConnectedLinkList;
};

class CRPRCTermSearch :
	public CBaseObj
{
public:
	CRPRCTermSearch(	RP_TERM enTerm, CRPWayPoint &clWayPoint, int iLevel, 
						SmartPointer< CRPRCCost > spclCost,
						SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > > spclLinkCostContainer,
						SmartPointer< CDPFacade > spclDataProvider);
	CRPRCTermSearch() = default;

	virtual RESULT Execute();
	virtual ~CRPRCTermSearch();

public:
	RESULT TermSearch(	RP_TERM enTerm, int iLevel,
						SmartPointer< CRPRCCost > spclCost,
						SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > > spclLinkCostContainer,
						SmartPointer< CDPFacade > spclDataProvider, 
						SmartPointer< CRPRCTermSearchResult > &spclResult);

public:
	//	input
	RP_TERM														m_enTerm;
	CRPWayPoint													m_clWayPoint;
	int															m_iLevel;

	SmartPointer< CRPRCCost >									m_spclCost;
	SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > >		m_spclLinkCostContainer;
	SmartPointer< CDPFacade >									m_spclDataProvider;

	//	output
	SmartPointer< CRPRCTermSearchResult >						m_spclResult;
};

