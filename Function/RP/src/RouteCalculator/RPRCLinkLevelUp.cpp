#include	"InnerCommon.hpp"
#include	"RPCommon.hpp"
#include	"RPRSLink.hpp"
#include	"RPWayPoints.hpp"
#include	"RPSection.hpp"
//#include	"RPRoute.h"
#include	"RPRouteCalcRequest.hpp"
//#include	"RPRCCmdBase.h"
#include	"RPRCRouteCalcCommon.hpp"
#include	"RPRCCost.hpp"
//#include	"RPRCExtIF.h"
//#include	"RPRCCmdCalcBase.h"
#include	"RPRCLinkLevelUp.hpp"

//	class CRPRCCmdLinkLevelUpResult
CRPRCLinkLevelUpResult::CRPRCLinkLevelUpResult()
{
}

CRPRCLinkLevelUpResult::~CRPRCLinkLevelUpResult()
{
}

//	class CRPRCCmdLinkLevelUp
CRPRCLinkLevelUp::CRPRCLinkLevelUp(//RPRCCmdSigReciever hReciever, RPRC_Cmd_PRI enCmdPRI,
	long lRouteID, //SmartPointer< CRPRCExtIF > spclExtIF,
	RP_TERM enTerm, int iFromLevel, int iToLevel,
	SmartPointer< vector< CRPRCUpSearchResultLink > > spvclInputLinkList,
	SmartPointer< RPRCTileContainer< CRPRCMidLinkUsingTile > > spclMidLinkUsingContainer,
	SmartPointer< CRPRCCost > spclCost,
	SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > > spclLinkCostContainer,
	SmartPointer< CDPFacade > spclDataProvider)
	: //CRPRCCmdCalcBase(RPRC_Cmd_Type_Route_LinkLevelUp, hReciever, enCmdPRI, lRouteID, spclExtIF), 
	m_enTerm(enTerm), m_iFromLevel(iFromLevel), m_iToLevel(iToLevel)
	, m_spvclInputLinkList(spvclInputLinkList), m_spclMidLinkUsingContainer(spclMidLinkUsingContainer)
	, m_spclCost(spclCost), m_spclLinkCostContainer(spclLinkCostContainer)
	, m_spclDataProvider(spclDataProvider)
{
}

CRPRCLinkLevelUp::~CRPRCLinkLevelUp()
{
}

RESULT CRPRCLinkLevelUp::Execute()
{
	return LinkLevelUp(m_enTerm, m_iFromLevel, m_iToLevel,
		m_spvclInputLinkList, m_spclMidLinkUsingContainer,
		m_spclCost, m_spclLinkCostContainer,
		m_spclDataProvider,
		m_spclResult);
}

SmartPointer< CRPRCLinkLevelUpResult > CRPRCLinkLevelUp::GetResult()
{
	return m_spclResult;
}

RESULT CRPRCLinkLevelUp::LinkLevelUp(RP_TERM enTerm, int iFromLevel, int iToLevel,
	SmartPointer< vector< CRPRCUpSearchResultLink > > spvclInputLinkList,
	SmartPointer< RPRCTileContainer< CRPRCMidLinkUsingTile > > spclMidLinkUsingContainer,
	SmartPointer< CRPRCCost > spclCost,
	SmartPointer< RPRCTileContainer< CRPRCLinkCostTile > > spclLinkCostContainer,
	SmartPointer< CDPFacade > spclDataProvider,
	SmartPointer< CRPRCLinkLevelUpResult > &spclResult)
{
	//
	if (!spclResult.Create()) {
		ERR("");
		return FAILURE;
	}

	if (!spclResult->m_spmapOpenTable.Create()) {
		ERR("");
		return FAILURE;
	}
	if (!spclResult->m_spclMidLinkTable.Create()) {
		ERR("");
		return FAILURE;
	}

	RPRC_OpenTable                      &clOpenTable = *(spclResult->m_spmapOpenTable);
	RPRC_MidLinkTable					&clMidLinkTable = *(spclResult->m_spclMidLinkTable);

	vector< CRPRCUpSearchResultLink >	&vclInputLinkList = *spvclInputLinkList;
	CRPRCRoutingTileProxy				clCurRoutingTileProxy(m_spclDataProvider);
	CRPRCRoutingTileProxy				clUpRoutingTileProxy(m_spclDataProvider);
	CRPRCMidLinkUsingTableProxy         clUpLinkUsingTableProxy(spclMidLinkUsingContainer);
	CRPRCCost							&clCost = *spclCost;
	CRPRCLinkCostTableProxy             clLinkCostTableProxy(spclLinkCostContainer);

	for (size_t i = 0; i < vclInputLinkList.size(); ++i) {
#if 0 // del by wxl --> no cancel available
		if (GetCancelFlag()) {
			return SUCCESS;
		}
#endif
		CRPRCMidLink	*pclDownLink = vclInputLinkList[i].m_pclMidLink;

		CRPRCLinkID					clUpLinkID;
		if (SUCCESS != GetUpLinkID(clCurRoutingTileProxy, pclDownLink->m_clLinkID, clUpLinkID)) {
			ERR("");
			return FAILURE;
		}

		//	calc arrive cost
		bool	bUpLinkRunPos = enTerm == RP_TERM_START ? clUpLinkID.m_bPos : !clUpLinkID.m_bPos;
		ushort	&usUpLinkCostRef = clLinkCostTableProxy.GetLinkCostRef(clUpLinkID.m_clTileID, clUpLinkID.m_usLinkNo, bUpLinkRunPos);
		if (usUpLinkCostRef == 0) {
			usUpLinkCostRef = spclCost->LinkCost(CRPRCLinkID(clUpLinkID.m_clTileID, clUpLinkID.m_usLinkNo, bUpLinkRunPos), clUpRoutingTileProxy);
		}
		uint	uiUpLinkCost = spclCost->ExtractShortCost(usUpLinkCostRef);

		CRPRCMidLink	&clUpLink = clMidLinkTable.New();
		clUpLink.m_clLinkID = clUpLinkID;
		clUpLink.m_clFromPoint = pclDownLink->m_clToPoint;
		clUpLink.m_uiLinkCost = uiUpLinkCost;
		clUpLink.m_uiArriveCost = pclDownLink->m_uiArriveCost - pclDownLink->m_uiLinkCost + uiUpLinkCost;
		clUpLink.m_pclDownLink = pclDownLink;
		clUpLink.m_itOpenLink = clOpenTable.insert(RPRC_OpenTableValueType(clUpLink.m_uiArriveCost, &clUpLink));

		pclDownLink->m_pclUpLink = &clUpLink;

		//		MESSAGE( "LinkLevelUp %s ( %d, %d ) ( %d, %d )", enTerm==RP_TERM_START?"start":"end  ", pclDownLink->m_clLinkID.m_clTileID.m_uiPackedTileID, pclDownLink->m_clLinkID.m_usLinkNo,
		//				 clUpLinkID.m_clTileID.m_uiPackedTileID, clUpLinkID.m_usLinkNo );

		CRPRCMidLinkUsing	*pclUpLinkUsing = clUpLinkUsingTableProxy.GetMidLinkUsing(clUpLinkID);
		pclUpLinkUsing->m_apclMidLink[enTerm][LINK_DIR_NO(clUpLinkID.m_bPos)] = &clUpLink;
	}

	return SUCCESS;
}

RESULT CRPRCLinkLevelUp::GetUpLinkID(CRPRCRoutingTileProxy &clCurRoutingTileProxy, CRPRCLinkID &clLinkID, CRPRCLinkID &clUpLinkID)
{
	if (SUCCESS != clCurRoutingTileProxy.GetData(clLinkID.m_clTileID)) {
		ERR("");
		return FAILURE;
	}

	ushort		usUpLinkRefNo = clCurRoutingTileProxy->m_clRoutingTileInfo.m_vclLinkList[clLinkID.m_usLinkNo].m_ausUpLinkRefNo[LINK_DIR_NO(clLinkID.m_bPos)];
	if (usUpLinkRefNo >= NDS_INVALID_NODE_ID) {
		ERR("");
		return FAILURE;
	}

	CExternalDirectedLinkReference	&clUpLinkRef = clCurRoutingTileProxy->m_clRoutingTile.m_clRouteUpLinks.m_vclRouteUpLinkRef[usUpLinkRefNo].m_clToLink;
	clUpLinkID.m_clTileID.m_uiPackedTileID = clCurRoutingTileProxy->m_clRoutingTile.m_clExternalTileIdList.m_viTileId[clUpLinkRef.m_usExtTileIdx];
	clUpLinkID.m_clTileID.m_sUpdateRegionID = clLinkID.m_clTileID.m_sUpdateRegionID;
	clUpLinkID.m_usLinkNo = clUpLinkRef.m_clTileExternalLinkReference.m_usLinkId;
	clUpLinkID.m_bPos = clUpLinkRef.m_clTileExternalLinkReference.m_ucPositiveLinkDirection;

	return SUCCESS;
}
