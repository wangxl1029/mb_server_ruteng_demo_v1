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
#include	"RPRCRouteSearchCommFunc.hpp"
#include	"RPRCRouteEdit.hpp"

//	class CRPRCCmdRouteEditResult
CRPRCRouteEditResult::CRPRCRouteEditResult()
{
}

CRPRCRouteEditResult::~CRPRCRouteEditResult()
{
}

//	class CRPRCCmdRouteEdit
CRPRCRouteEdit::CRPRCRouteEdit(//RPRCCmdSigReciever hReciever, RPRC_Cmd_PRI enCmdPRI,
	long lRouteID,// SmartPointer< CRPRCExtIF > spclExtIF,
	CRPWayPoint &clStartWayPoint, CRPWayPoint &clEndWayPoint, int iConnectLevel, vector< int > &viLevelList,
	SmartPointer< vector< CRPRSLink > > spvclStartTermSearchResultLinkList,
	SmartPointer< vector< CRPRSLink > > spvclEndTermSearchResultLinkList,
	vector< SmartPointer< RPRC_MidLinkTable > > &vspclStartTermMidLinkTable,
	vector< SmartPointer< RPRC_MidLinkTable > > &vspclEndTermMidLinkTable,
	SmartPointer< RPRCTileContainer< CRPRCMidLinkUsingTile > > spclMidLinkUsingContainer,
	SmartPointer< vector< CRPRCConnectSearchResultLink > > spvclStartTermConnectSearchResultLinkList,
	SmartPointer< vector< CRPRCConnectSearchResultLink > > spvclEndTermConnectSearchResultLinkList,
	SmartPointer< CDPFacade > spclDataProvider)
	: //CRPRCCmdCalcBase(RPRC_Cmd_Type_Route_Edit, hReciever, enCmdPRI, lRouteID, spclExtIF), 
	m_clStartWayPoint(clStartWayPoint), m_clEndWayPoint(clEndWayPoint), m_iConnectLevel(iConnectLevel), m_viLevelList(viLevelList)
	, m_spvclStartTermSearchResultLinkList(spvclStartTermSearchResultLinkList)
	, m_spvclEndTermSearchResultLinkList(spvclEndTermSearchResultLinkList)
	, m_vspclStartTermMidLinkTable(vspclStartTermMidLinkTable)
	, m_vspclEndTermMidLinkTable(vspclEndTermMidLinkTable)
	, m_spclMidLinkUsingContainer(spclMidLinkUsingContainer)
	, m_spvclStartConnectSearchResultLinkList(spvclStartTermConnectSearchResultLinkList)
	, m_spvclEndConnectSearchResultLinkList(spvclEndTermConnectSearchResultLinkList)
	, m_spclDataProvider(spclDataProvider)
{
}


CRPRCRouteEdit::~CRPRCRouteEdit()
{
}

RESULT CRPRCRouteEdit::Execute()
{
	return RouteEdit(m_clStartWayPoint, m_clEndWayPoint, m_iConnectLevel, m_viLevelList,
		m_spvclStartTermSearchResultLinkList,
		m_spvclEndTermSearchResultLinkList,
		m_vspclStartTermMidLinkTable,
		m_vspclEndTermMidLinkTable,
		m_spclMidLinkUsingContainer,
		m_spvclStartConnectSearchResultLinkList,
		m_spvclEndConnectSearchResultLinkList,
		m_spclDataProvider,
		m_spclResult);
}

SmartPointer< CRPRCRouteEditResult > CRPRCRouteEdit::GetResult()
{
	return m_spclResult;
}
RESULT CRPRCRouteEdit::RouteEdit(CRPWayPoint &clStartWayPoint, CRPWayPoint &clEndWayPoint, int iConnectLevel, vector< int > &viLevelList,
	SmartPointer< vector< CRPRSLink > > spvclStartTermSearchResultLinkList,
	SmartPointer< vector< CRPRSLink > > spvclEndTermSearchResultLinkList,
	vector< SmartPointer< RPRC_MidLinkTable > > &vspclStartTermMidLinkTable,
	vector< SmartPointer< RPRC_MidLinkTable > > &vspclEndTermMidLinkTable,
	SmartPointer< RPRCTileContainer< CRPRCMidLinkUsingTile > > spclMidLinkUsingContainer,
	SmartPointer< vector< CRPRCConnectSearchResultLink > > spvclStartTermConnectSearchResultLinkList,
	SmartPointer< vector< CRPRCConnectSearchResultLink > > spvclEndTermConnectSearchResultLinkList,
	SmartPointer< CDPFacade > spclDataProvider,
	SmartPointer< CRPRCRouteEditResult > &spclResult)
{
	if (!m_spclResult.Create()) {
		ERR("");
		return FAILURE;
	}
	if (!m_spclResult->m_spclSection.Create()) {
		ERR("");
		return FAILURE;
	}
	m_spclResult->m_spclSection->m_clStartWayPoint = m_clStartWayPoint;
	m_spclResult->m_spclSection->m_clEndWayPoint = m_clEndWayPoint;

	CRPSection								&clSection = *(m_spclResult->m_spclSection);

	vector< CRPRSLink >						&vclStartTermSearchResultLinkList = *spvclStartTermSearchResultLinkList;
	vector< CRPRSLink >						&vclEndTermSearchResultLinkList = *spvclEndTermSearchResultLinkList;
	CRPRCMidLinkUsingTableProxy				clMidLinkUsingTableProxy(spclMidLinkUsingContainer);
	vector< CRPRCConnectSearchResultLink >	&vclStartTermConnectSearchResultLinkList = *spvclStartTermConnectSearchResultLinkList;
	vector< CRPRCConnectSearchResultLink >	&vclEndTermConnectSearchResultLinkList = *spvclEndTermConnectSearchResultLinkList;
	CRPRCRoutingTileProxy						clRoutingTile(spclDataProvider);

	//	合并始终端探索得到的接续Link
	vector< CRPRCConnectSearchResultLink >	vclConnectLinkList(vclStartTermConnectSearchResultLinkList);
	vclConnectLinkList.reserve(vclStartTermConnectSearchResultLinkList.size() + vclEndTermConnectSearchResultLinkList.size());
	vclConnectLinkList.insert(vclConnectLinkList.end(), vclStartTermConnectSearchResultLinkList.begin(), vclStartTermConnectSearchResultLinkList.end());
	vclConnectLinkList.insert(vclConnectLinkList.end(), vclEndTermConnectSearchResultLinkList.begin(), vclEndTermConnectSearchResultLinkList.end());

	RPRC_MidLinkTable							&clStartTermConnLevelMidLinkTable = *vspclStartTermMidLinkTable.back();
	RPRC_MidLinkTable							&clEndTermConnLevelMidLinkTable = *vspclEndTermMidLinkTable.back();

	//	找最小Cost的Link
	uint									uiMinArriveCost = 0xFFFFFFFF;
	CRPRCConnectSearchResultLink				clMinOutLink;
	for (size_t i = 0; i < vclConnectLinkList.size(); ++i) {
#if 0 // del by wxl --> no cancel available
		if (GetCancelFlag()) {
			return SUCCESS;
		}
#endif
		//	StartTerm时，StartTermLink的方向与clOutLink.m_clLinkID.m_bPos相同，EndTermLink的方向与clOutLink.m_clLinkID.m_bPos相反
		CRPRCConnectSearchResultLink	&clOutLink = vclConnectLinkList[i];
		CRPRCLinkID					&clOutLinkID = clOutLink.m_pclMidLink->m_clLinkID;
		bool						bStartTermPos = clOutLink.m_enTerm == RP_TERM_START ? clOutLinkID.m_bPos : !clOutLinkID.m_bPos;
		CRPRCMidLinkUsing				*pclLinkUsing = clMidLinkUsingTableProxy.GetMidLinkUsing(clOutLinkID);
		CRPRCMidLink					*pclStartTermLink = pclLinkUsing->m_apclMidLink[RP_TERM_START][LINK_DIR_NO(bStartTermPos)];
		CRPRCMidLink					*pclEndTermLink = pclLinkUsing->m_apclMidLink[RP_TERM_END][LINK_DIR_NO(!bStartTermPos)];
		uint						uiArriveCost = pclStartTermLink->m_uiArriveCost + pclEndTermLink->m_uiArriveCost - clOutLink.m_pclMidLink->m_uiLinkCost;
		if (uiArriveCost < uiMinArriveCost) {
			uiMinArriveCost = uiArriveCost;
			clMinOutLink = clOutLink;
		}
	}

	//
	list< CRPRSLink >		vclSection;

	vclSection.push_front(CRPRSLink(clMinOutLink.m_pclMidLink->m_clLinkID.m_clTileID, clMinOutLink.m_pclMidLink->m_clLinkID.m_usLinkNo,
		clMinOutLink.m_enTerm == RP_TERM_START ? clMinOutLink.m_pclMidLink->m_clLinkID.m_bPos : !clMinOutLink.m_pclMidLink->m_clLinkID.m_bPos));

	CRPRCMidLinkUsing			*pclOutLinkUsing = clMidLinkUsingTableProxy.GetMidLinkUsing(clMinOutLink.m_pclMidLink->m_clLinkID);
	bool					bStartTermPos = clMinOutLink.m_enTerm == RP_TERM_START ? clMinOutLink.m_pclMidLink->m_clLinkID.m_bPos : !clMinOutLink.m_pclMidLink->m_clLinkID.m_bPos;
	CRPRCMidLink				*pclConnStartTermLink = pclOutLinkUsing->m_apclMidLink[RP_TERM_START][LINK_DIR_NO(bStartTermPos)];
	CRPRCMidLink				*pclConnEndTermLink = pclOutLinkUsing->m_apclMidLink[RP_TERM_END][LINK_DIR_NO(!bStartTermPos)];

	CRPRCMidLink				*pclStartLink = pclConnStartTermLink;
	while ((pclStartLink = GetPrevLink(pclStartLink)) != NULL) {
#if 0 // del by wxl --> no cancel available
		if (GetCancelFlag()) {
			return SUCCESS;
		}
#endif
		vclSection.push_front(CRPRSLink(pclStartLink->m_clLinkID.m_clTileID, pclStartLink->m_clLinkID.m_usLinkNo, pclStartLink->m_clLinkID.m_bPos));
	}

	//	最后一根RSLink可能是Link的一部分，需要更新相关信息
	//	for( size_t j = 0; j < vclStartTermSearchResultLinkList.size(); ++j ) {
	//		CRPRSLink	&clRSLink				=	vclStartTermSearchResultLinkList[j];
	//		if( clRSLink.m_clTileID == pclStartLink->m_clLinkID.m_clTileID && clRSLink.m_usLinkID == pclStartLink->m_clLinkID.m_usLinkNo && clRSLink.m_bPos == pclStartLink->m_clLinkID.m_bPos ) {
	//			vclSection.push_front( clRSLink );
	//			break;
	//		}
	//	}

	CRPRCMidLink				*pclEndLink = pclConnEndTermLink;
	while ((pclEndLink = GetPrevLink(pclEndLink)) != NULL) {
#if 0 //del by wxl --> no cancel available
		if (GetCancelFlag()) {
			return SUCCESS;
		}
#endif
		vclSection.push_back(CRPRSLink(pclEndLink->m_clLinkID.m_clTileID, pclEndLink->m_clLinkID.m_usLinkNo, !pclEndLink->m_clLinkID.m_bPos));
	}

	//	最后一根RSLink可能是Link的一部分，需要更新相关信息

	//
	for (list< CRPRSLink >::iterator it = vclSection.begin(); it != vclSection.end(); ++it) {
#if 0 // del by wxl --> no cancel available
		if (GetCancelFlag()) {
			return SUCCESS;
		}
#endif
		CRPRSLink	&clRSLink = *it;

		int			iLevel = 0;
		uint		uiTileNo = 0;
		if (SUCCESS != CDPCommon::PackedTileIDToTileNo(clRSLink.m_clTileID.m_uiPackedTileID, iLevel, uiTileNo)) {
			ERR("");
			return FAILURE;
		}

		int			iAnchorX = 0;
		int			iAnchorY = 0;

		uint	uiCoordShift = 0;
		if (SUCCESS != spclDataProvider->GetCoordShift(iLevel, BUILDING_BLOCK_ID_ROUTING, uiCoordShift)) {
			ERR("");
			return FAILURE;
		}

		CRPRCLinkID					clLinkIDForStartPoint(clRSLink.m_clTileID, clRSLink.m_usLinkID, !clRSLink.m_bPos);
		CRPTileID					clNodeTileIDForStartPoint;
		CDPRoutingTileLinkNodeNo	clLinkNodeNoForStartPoint;
		if (SUCCESS != CRPRCRouteSearchCommFunc::GetNodeNoByLink(clLinkIDForStartPoint, clRoutingTile, clNodeTileIDForStartPoint, clLinkNodeNoForStartPoint, clRoutingTile)) {
			ERR("");
			return FAILURE;
		}
		if (SUCCESS != CDPCommon::PackedTileIDToTileNo(clRoutingTile->m_clTileID.m_uiPackedTileID, iLevel, uiTileNo)) {
			ERR("");
			return FAILURE;
		}
		if (SUCCESS != CDPCommon::GetBasePointCoordOfTile(iLevel, uiTileNo, iAnchorX, iAnchorY)) {
			ERR("");
			return FAILURE;
		}
		CCoordXYOffset	&clCoordForStartPoint = clRoutingTile->m_clRoutingTile.m_clSimpleIntersection.m_vclSimpleIntersection[clLinkNodeNoForStartPoint.m_usNodeNo].m_clPosition;
		clRSLink.m_clStartPoint.x = iAnchorX + (clCoordForStartPoint.m_iDx << uiCoordShift);
		clRSLink.m_clStartPoint.y = iAnchorY + (clCoordForStartPoint.m_iDy << uiCoordShift);

		CRPRCLinkID					clLinkIDForEndPoint(clRSLink.m_clTileID, clRSLink.m_usLinkID, clRSLink.m_bPos);
		CRPTileID					clNodeTileIDForEndPoint;
		CDPRoutingTileLinkNodeNo	clLinkNodeNoForEndPoint;
		if (SUCCESS != CRPRCRouteSearchCommFunc::GetNodeNoByLink(clLinkIDForEndPoint, clRoutingTile, clNodeTileIDForEndPoint, clLinkNodeNoForEndPoint, clRoutingTile)) {
			ERR("");
			return FAILURE;
		}
		if (SUCCESS != CDPCommon::PackedTileIDToTileNo(clRoutingTile->m_clTileID.m_uiPackedTileID, iLevel, uiTileNo)) {
			ERR("");
			return FAILURE;
		}
		if (SUCCESS != CDPCommon::GetBasePointCoordOfTile(iLevel, uiTileNo, iAnchorX, iAnchorY)) {
			ERR("");
			return FAILURE;
		}
		CCoordXYOffset	&clCoordForEndPoint = clRoutingTile->m_clRoutingTile.m_clSimpleIntersection.m_vclSimpleIntersection[clLinkNodeNoForEndPoint.m_usNodeNo].m_clPosition;
		clRSLink.m_clEndPoint.x = iAnchorX + (clCoordForEndPoint.m_iDx << uiCoordShift);
		clRSLink.m_clEndPoint.y = iAnchorY + (clCoordForEndPoint.m_iDy << uiCoordShift);

		clSection.m_vclRSLinkList.push_back(clRSLink);
	}

	//MESSAGE( "CRPRCCmdRouteEdit::RouteEdit Link Count = %d", clSection.m_vclRSLinkList.size() );

	return SUCCESS;
}

CRPRCMidLink* CRPRCRouteEdit::GetPrevLink(CRPRCMidLink *pclLink)
{
	while (pclLink->m_pclPrevLink == NULL) {
		pclLink = pclLink->m_pclDownLink;
		if (pclLink == NULL) {
			return NULL;
		}
	}
	return pclLink->m_pclPrevLink;
}
