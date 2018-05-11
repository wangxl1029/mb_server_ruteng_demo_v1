#pragma once

//	class CRPRCCmdUpSearchResult
class CRPRCUpSearchResult : public CBaseObj
{
public:
	CRPRCUpSearchResult();
	virtual ~CRPRCUpSearchResult();

public:
	SmartPointer< vector< CRPRCUpSearchResultLink > >			m_spvclResultLinkList;
	SmartPointer< vector< CRPRCConnectSearchResultLink > >		m_spvclConnectedLinkList;
};


class CRPRCUpSearch : public CBaseObj
{
public:
	CRPRCUpSearch();
	virtual ~CRPRCUpSearch();
	virtual RESULT Execute();

	SmartPointer< CRPRCUpSearchResult > GetResult();

public:
	RESULT UpSearch(	RP_TERM enTerm, int iFromLevel, int iToLevel,
						SmartPointer< RPRC_OpenTable > spmapOpenTable,
						SmartPointer< RPRC_MidLinkTable > spvclMidLinkTable,
						SmartPointer< RPRCTileContainer< CRPRCMidLinkUsingTile > > spclMidLinkUsingContainer,
						SmartPointer< CRPRCCost > spclCost,
						SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > > spclLinkCostContainer,
						SmartPointer< CDPFacade > spclDataProvider,
						SmartPointer< CRPRCUpSearchResult > &spclResult);

public:
	//	input
	RP_TERM														m_enTerm;
	int															m_iFromLevel;
	int															m_iToLevel;

	SmartPointer< RPRC_OpenTable >								m_spmapOpenTable;
	SmartPointer< RPRC_MidLinkTable >							m_spclMidLinkTable;
	SmartPointer< RPRCTileContainer< CRPRCMidLinkUsingTile > >	m_spclMidLinkUsingContainer;
	SmartPointer< CRPRCCost >									m_spclCost;
	SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > >		m_spclLinkCostContainer;
	SmartPointer< CDPFacade >									m_spclDataProvider;

	//	output
	SmartPointer< CRPRCUpSearchResult >						m_spclResult;
};

