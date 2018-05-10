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
//	SmartPointer< RPRC_MidLinkTable >							m_spclMidLinkTable;
//	SmartPointer< vector< CRPRCConnectSearchResultLink > >		m_spvclConnectedLinkList;
};

class CRPRCTermSearch :
	public CBaseObj
{
public:
	CRPRCTermSearch(CRPWayPoint &clWayPoint, int iLevel, SmartPointer< CDPFacade > spclDataProvider);
	CRPRCTermSearch() = default;

	virtual RESULT Execute();
	virtual ~CRPRCTermSearch();

public:
	RESULT TermSearch(	int iLevel, 
						SmartPointer< CDPFacade > spclDataProvider, 
						SmartPointer< CRPRCTermSearchResult > &spclResult);

public:
	//	input
	CRPWayPoint													m_clWayPoint;
	int															m_iLevel;

	SmartPointer< CDPFacade >									m_spclDataProvider;

	//	output
	SmartPointer< CRPRCTermSearchResult >						m_spclResult;
};
