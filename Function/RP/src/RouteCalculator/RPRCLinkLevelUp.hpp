#pragma once
//	class CRPRCCmdLinkLevelUpResult
class CRPRCLinkLevelUpResult : public CBaseObj
{
public:
	CRPRCLinkLevelUpResult();
	virtual ~CRPRCLinkLevelUpResult();

public:
	SmartPointer< RPRC_OpenTable >								m_spmapOpenTable;
	SmartPointer< RPRC_MidLinkTable >								m_spclMidLinkTable;
};

//	class CRPRCCmdLinkLevelUp
class CRPRCLinkLevelUp : public CBaseObj
{
public:
	CRPRCLinkLevelUp(//RPRCCmdSigReciever hReciever, RPRC_Cmd_PRI enCmdPRI,
		long lRouteID, //SmartPointer< CRPRCExtIF > spclExtIF,
		RP_TERM enTerm, int iFromLevel, int iToLevel,
		SmartPointer< vector< CRPRCUpSearchResultLink > > spvclInputLinkList,
		SmartPointer< RPRCTileContainer< CRPRCMidLinkUsingTile > > spclMidLinkUsingContainer,
		SmartPointer< CRPRCCost > spclCost,
		SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > > spclLinkCostContainer,
		SmartPointer< CDPFacade > spclDataProvider);
	virtual ~CRPRCLinkLevelUp();

	virtual RESULT Execute();

	SmartPointer< CRPRCLinkLevelUpResult > GetResult();

public:
	RESULT LinkLevelUp(RP_TERM enTerm, int iFromLevel, int iToLevel,
		SmartPointer< vector< CRPRCUpSearchResultLink > > spvclInputLinkList,
		SmartPointer< RPRCTileContainer< CRPRCMidLinkUsingTile > > spclMidLinkUsingContainer,
		SmartPointer< CRPRCCost > spclCost,
		SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > > spclLinkCostContainer,
		SmartPointer< CDPFacade > spclDataProvider,
		SmartPointer< CRPRCLinkLevelUpResult > &spclResult);

	RESULT GetUpLinkID(CRPRCRoutingTileProxy &clCurRoutingTileProxy, CRPRCLinkID &clLinkID, CRPRCLinkID &clUpLinkID);

public:
	//	input
	RP_TERM														m_enTerm;
	int															m_iFromLevel;
	int															m_iToLevel;
	SmartPointer< vector< CRPRCUpSearchResultLink > >				m_spvclInputLinkList;
	SmartPointer< RPRCTileContainer< CRPRCMidLinkUsingTile > >		m_spclMidLinkUsingContainer;
	SmartPointer< CRPRCCost >										m_spclCost;
	SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > >			m_spclLinkCostContainer;
	SmartPointer< CDPFacade >									m_spclDataProvider;

	//	output
	SmartPointer< CRPRCLinkLevelUpResult >						m_spclResult;
};
