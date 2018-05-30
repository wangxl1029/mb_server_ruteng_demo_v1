// refer to RPRCRouteDirector.cpp
#include	"InnerCommon.hpp"
#include	"RPCommon.hpp"
#include	"RPRSLink.hpp"	// --> add by wxl
#include	"RPWayPoints.hpp"
#include	"RPSection.hpp"
//#include	"RPRoute.h"
#include	"RPRouteCalcRequest.hpp"
//#include	"RPRCCmdBase.h"
//#include	"RPRCThreadPool.h"
#include	"RPRCRouteCalcCommon.hpp"
#include	"RPRCCost.hpp"
//#include	"RPRCExtIF.h"
//#include	"RPRCCmdCalcBase.h"
#include	"RPRCTermSearch.hpp"
#include	"RPRCUpSearch.hpp"
#include	"RPRCLinkLevelUp.hpp"
#include	"RPRCConnectSearch.hpp"
#include	"RPRCRouteEdit.hpp"
#include	"RPRCSectionDirector.hpp"
#include	"RPRouteDataLoader.hpp"

#include	"RouteSession.hpp"
#include	"DPFacade.hpp"

class CRouteSession::CPrivate
{
public:
	CPrivate() : m_lRouteID(0) {}
	static long NextRouteID() { return m_iNextRouteID++; }
	long														m_lRouteID;
	SmartPointer< CDPFacade >									m_spclDataProvider;
	SmartPointer< CRPRCLinkCostContainerSet >					m_spclLinkCostContainerSet; // refer to CRPRCMultiRouteDirector()
	SmartPointer< CRPRCRoutingTileProxy	>						m_spclLinkRoutingTile;
	SmartPointer< CRPRouteDataLoader >							m_spclRouteDataLoader;
	static volatile long										m_iNextRouteID;
};

volatile long		CRouteSession::CPrivate::m_iNextRouteID = 0;

CRouteSession::CRouteSession()
{
}

CRouteSession::~CRouteSession()
{
}

bool CRouteSession::Initialize()
{
	if (!mp)
	{
		mp = make_shared<CPrivate>();
	}

	mp->m_spclDataProvider = CDPFacade::Create();
	if (mp->m_spclDataProvider == NULL)
	{
		ERR("");
		return false;
	}
	//	spclDPFacade->Initialize();
	if (SUCCESS != mp->m_spclDataProvider->Initialize())
	{
		ERR("");
		return false;
	}

	if (!mp->m_spclLinkCostContainerSet.Create()) {	// refer to CRPRCMultiRouteDirector::Initialize()
		ERR("");
		return false;
	}
	if (SUCCESS != mp->m_spclLinkCostContainerSet->Initialize(mp->m_spclDataProvider)) {
		ERR("");
		return false;
	}

	if (!mp->m_spclRouteDataLoader.Create()){
		ERR("");
		return false;
	}

	if (SUCCESS != mp->m_spclRouteDataLoader->Initialize(mp->m_spclDataProvider))
	{
		ERR("");
		return false;
	}


	if (!mp->m_spclLinkRoutingTile.Create(mp->m_spclDataProvider)){
		ERR("");
		return false;
	}

	return true;
}

inline bool equal_except_pos(const CRPRCLinkID &clLeft, const CRPRCLinkID &clRight)
{
	return clLeft.m_clTileID == clRight.m_clTileID && clLeft.m_usLinkNo == clRight.m_usLinkNo;
}

static void _DownTo13Level(SmartPointer<CRPRCRoutingTileProxy>& spclOriTileProxy, const CRPRCLinkID& clOriLinkId, vector<CRPRCLinkID>& vcl13LevelIds)
{
	vector<CRPRCLinkID> vclDownLinkIds;
	CRPRCRoutingTileProxy clTmpTileProxy(spclOriTileProxy->m_spclDataProvider);
	CRPRCRoutingTileProxy clMidTileProxy(spclOriTileProxy->m_spclDataProvider);
	CRPRCRoutingTileProxy clExtTileProxy(spclOriTileProxy->m_spclDataProvider);
	CRPRCRoutingTileProxy clEndNodeTileProxy(spclOriTileProxy->m_spclDataProvider);

	spclOriTileProxy->GetData(clOriLinkId.m_clTileID);
	CRPRCRoutingTileProxy &clOriTileProxy = (*spclOriTileProxy.ptr);

	// 源link所在tile有下层路段数据？
	if (clOriTileProxy->m_clRoutingTile.m_clHeader.m_clContentMask.m_ucHasRouteDownLinkList)
	{
		CRouteDownLinkElement& clDownLinkElement = clOriTileProxy->m_clRoutingTile.m_clRouteDownLinks.m_vclRouteDownLinkElement[clOriLinkId.m_usLinkNo];
		// 源link下层路段数据数量大于0
		if (clDownLinkElement.m_ucNumRefs > 0)
		{
			for (size_t idxDownLink = 0; idxDownLink < clDownLinkElement.m_ucNumRefs; idxDownLink++)
			{
				CRPRCLinkID clDownLinkID;
				clDownLinkID.m_clTileID.m_uiPackedTileID = clOriTileProxy->m_clRoutingTile.m_clExternalTileIdList.m_viTileId[clDownLinkElement.m_vclToLink[idxDownLink].m_usExtTileIdx];
				// DownLinkElement中的PositiveLinkDirection是指相对于源Link的方向是正是反，此处用同或运算
				clDownLinkID.m_bPos = !(clOriLinkId.m_bPos ^ (bool)clDownLinkElement.m_vclToLink[idxDownLink].m_clTileExternalLinkReference.m_ucPositiveLinkDirection);
				clDownLinkID.m_usLinkNo = clDownLinkElement.m_vclToLink[idxDownLink].m_clTileExternalLinkReference.m_usLinkId;
				clDownLinkID.m_clTileID.m_sUpdateRegionID = clOriLinkId.m_clTileID.m_sUpdateRegionID; // Link不会跨UR
				clTmpTileProxy.GetData(clDownLinkID.m_clTileID);
				// NDS2.5.1新变更, 可以直接降到第13层
				if (clTmpTileProxy->m_iLevel == 13)
				{
					vclDownLinkIds.push_back(clDownLinkID);
				}
			}

			// 理论上此处只会有1、2两种情况
			if (vclDownLinkIds.size() == 2)
			{
				CRPRCLinkID startLinkId;
				CRPRCLinkID endLinkId;
				// 源Link是正向，取低层第一个Link为开始Link
				if (clOriLinkId.m_bPos)
				{
					startLinkId = vclDownLinkIds.at(0);
					endLinkId = vclDownLinkIds.at(1);
				}
				else // 源Link是反向，则取低层第2个Link为开始Link
				{
					startLinkId = vclDownLinkIds.at(1);
					endLinkId = vclDownLinkIds.at(0);
				}

				CRPRCLinkID midLinkId = startLinkId;
				vector<CRPRCLinkID> midLinks;

				midLinks.push_back(startLinkId);
				// 开始进行连接探索，探索策略为frc相同则取其为连接Link
//				while (midLinkId != endLinkId)
				while (!(midLinkId == endLinkId))
				{
					CDPRoutingTileLinkNodeNo endNodeId;
					vector<CIntOrExtDirectedLinkReference>* connLinks;
					clMidTileProxy.GetData(midLinkId.m_clTileID);
					// 根据Link方向取endNode
					if (midLinkId.m_bPos)
						endNodeId = clMidTileProxy->m_clRoutingTileInfo.m_vclLinkList[midLinkId.m_usLinkNo].m_clEndNode;
					else
						endNodeId = clMidTileProxy->m_clRoutingTileInfo.m_vclLinkList[midLinkId.m_usLinkNo].m_clStartNode;
					// link的endNode不在link所在的tile中
					if (endNodeId.m_usNodeNo == 65535)
					{
						CRPTileID extTileId;
						CDPRoutingTileExtLinkID eLinkId;
						// 取link关联的最后一个tile，endNode在此tile中
						// TODO: 如果link为反向，是否需要取第一个关联tile？
#if 1
						assert(false); // hard codes to mark incompatable data struct
#else
						extTileId.m_uiPackedTileID = clMidTileProxy->m_clRoutingTile.m_clExternalTileIdList.m_viTileId[clMidTileProxy->m_clRoutingTile.m_clLink2TileList.m_vclTileRef[clMidTileProxy->m_clRoutingTileInfo.m_vclLinkList[midLinkId.m_usLinkNo].m_usLink2TileNo].m_clExtTileColl.m_vusExtTileIdx[clMidTileProxy->m_clRoutingTile.m_clLink2TileList.m_vclTileRef[clMidTileProxy->m_clRoutingTileInfo.m_vclLinkList[midLinkId.m_usLinkNo].m_usLink2TileNo].m_clExtTileColl.m_uiNumExtTileIds - 1]];
#endif
						extTileId.m_sUpdateRegionID = clOriLinkId.m_clTileID.m_sUpdateRegionID;
						clEndNodeTileProxy.GetData(extTileId);
						eLinkId.m_uiTileID = midLinkId.m_clTileID.m_uiPackedTileID;
						eLinkId.m_usLinkID = midLinkId.m_usLinkNo;
						// 从endNode所在tile中查询midLink的tileId和linkNo
						endNodeId = clEndNodeTileProxy->m_clRoutingTileInfo.m_mapExtLink2Node.find(eLinkId)->second;
						connLinks = &clEndNodeTileProxy->m_clRoutingTile.m_clSimpleIntersection.m_vclSimpleIntersection[endNodeId.m_usNodeNo].m_vclConnectedLinks;
					}
					else // link的endNode在link所在的tile中
					{
						clEndNodeTileProxy = clMidTileProxy;
						connLinks = &clEndNodeTileProxy->m_clRoutingTile.m_clSimpleIntersection.m_vclSimpleIntersection[endNodeId.m_usNodeNo].m_vclConnectedLinks;
					}

					for (size_t idxConnLink = 0; idxConnLink < connLinks->size(); idxConnLink++)
					{
						// 连接link所在数据在本tile内
						if (!(*connLinks)[idxConnLink].m_ucIsExternalLinkReference)
						{
							ushort usConnLinkId = (*connLinks)[idxConnLink].m_clLinkReferenceChoice.InternalLinkReference_Ref().m_usLinkId;
							// 接续link的frc与midLink的frc相同
							if (clEndNodeTileProxy->m_clRoutingTile.m_clFixedRoadAttributeSetList.m_vclAttributeList[clEndNodeTileProxy->m_clRoutingTile.m_clLinks.m_vclLink[usConnLinkId].m_clAttrInfo.FixedRoadAttributeSetListIndex_Ref()].m_clRoutingAttr.m_ucFunctionalRoadClass ==
								clMidTileProxy->m_clRoutingTile.m_clFixedRoadAttributeSetList.m_vclAttributeList[clMidTileProxy->m_clRoutingTile.m_clLinks.m_vclLink[midLinkId.m_usLinkNo].m_clAttrInfo.FixedRoadAttributeSetListIndex_Ref()].m_clRoutingAttr.m_ucFunctionalRoadClass)
							{
								CRPRCLinkID clConnLinkID;
								clConnLinkID.m_clTileID.m_uiPackedTileID = clEndNodeTileProxy->m_clTileID.m_uiPackedTileID;
								clConnLinkID.m_clTileID.m_sUpdateRegionID = clOriLinkId.m_clTileID.m_sUpdateRegionID; // Link不会跨UR
								clConnLinkID.m_bPos = (*connLinks)[idxConnLink].m_clLinkReferenceChoice.InternalLinkReference_Ref().m_ucPositiveLinkDirection;
								clConnLinkID.m_usLinkNo = usConnLinkId;
								// 排除相同link方向不同的情况
								//if (!midLinkId.equal(clConnLinkID))
								if (!equal_except_pos(midLinkId, clConnLinkID))
								{
									midLinkId = clConnLinkID;
									midLinks.push_back(midLinkId);
									break;
								}
							}
						}
						else // 连接link所在数据在其它tile内
						{
							CRPRCLinkID clExtConnLinkID;
							clExtConnLinkID.m_clTileID.m_uiPackedTileID = clEndNodeTileProxy->m_clRoutingTile.m_clExternalTileIdList.m_viTileId[(*connLinks)[idxConnLink].m_clLinkReferenceChoice.ExternalLinkReference_Ref().m_usExtTileIdx];
							clExtConnLinkID.m_usLinkNo = (*connLinks)[idxConnLink].m_clLinkReferenceChoice.ExternalLinkReference_Ref().m_clTileExternalLinkReference.m_usLinkId;
							clExtConnLinkID.m_clTileID.m_sUpdateRegionID = clOriLinkId.m_clTileID.m_sUpdateRegionID; // Link不会跨UR
							clExtConnLinkID.m_bPos = (*connLinks)[idxConnLink].m_clLinkReferenceChoice.ExternalLinkReference_Ref().m_clTileExternalLinkReference.m_ucPositiveLinkDirection;
							clExtTileProxy.GetData(clExtConnLinkID.m_clTileID);
							// 接续link的frc与midLink的frc相同
							if (clExtTileProxy->m_clRoutingTile.m_clFixedRoadAttributeSetList.m_vclAttributeList[clExtTileProxy->m_clRoutingTile.m_clLinks.m_vclLink[clExtConnLinkID.m_usLinkNo].m_clAttrInfo.FixedRoadAttributeSetListIndex_Ref()].m_clRoutingAttr.m_ucFunctionalRoadClass ==
								clMidTileProxy->m_clRoutingTile.m_clFixedRoadAttributeSetList.m_vclAttributeList[clMidTileProxy->m_clRoutingTile.m_clLinks.m_vclLink[midLinkId.m_usLinkNo].m_clAttrInfo.FixedRoadAttributeSetListIndex_Ref()].m_clRoutingAttr.m_ucFunctionalRoadClass)
							{
								// 排除相同link方向不同的情况
								//if (!midLinkId.equal(clExtConnLinkID))
								if (!equal_except_pos(midLinkId, clExtConnLinkID))
								{
									midLinkId = clExtConnLinkID;
									midLinks.push_back(midLinkId);
									break;
								}
							}
						}
					}
				}
				vclDownLinkIds = midLinks;
			}
		}
	}

	// 无低层link
	if (vclDownLinkIds.size() == 0)
	{
		vclDownLinkIds.push_back(clOriLinkId);
	}

	vcl13LevelIds = vclDownLinkIds;
	return;
}

// reference pattern : 
//		CMNRP::SetDestinationByTwoCoord() and 
//		CRPRCRouteDirector::StartCalculateRoute()
bool CRouteSession::calcRoute(int iFromX, int iFromY, int iToX, int iToY)
{
	CRPWayPoint		clVehicleWayPoint;
	clVehicleWayPoint.m_enWayPointType = RPWayPointType_Coord;
	clVehicleWayPoint.m_clCoord.x = iFromX;
	clVehicleWayPoint.m_clCoord.y = iFromY;

	CRPWayPoint		clDestination;
	clDestination.m_enWayPointType = RPWayPointType_Coord;
	clDestination.m_clCoord.x = iToX;
	clDestination.m_clCoord.y = iToY;

	CRPRouteCalcRequest	clRequest;
	if (!clRequest.m_spclWayPoints.Create()) {
		ERR("");
		return false;
	}
	clRequest.m_spclWayPoints->PushBack(clVehicleWayPoint);
	clRequest.m_spclWayPoints->PushBack(clDestination);

	SmartPointer< CDPFacade > spclDPFacade = mp->m_spclDataProvider;
	spclDPFacade->Initialize();

	if (mp->m_spclRouteDataLoader == NULL)
	{
		;
	}

	SmartPointer< CRPRCSectionDirector >	spclSectionDirector; // refer to CRPRCRouteDirector::Initialize()
	if (!spclSectionDirector.Create()) {
		ERR("");
		return false;
	}

	//m_spclRoute->m_uiSearchType = clRequest.m_uiSearchType;
	mp->m_lRouteID = CPrivate::NextRouteID();
	//m_spclRoute->m_iRouteID = m_lRouteID;

	// refer to CRPRCMultiRouteDirector::Initialize()
	if (SUCCESS == spclSectionDirector->Initialize(spclDPFacade, mp->m_spclLinkCostContainerSet->m_aspclLinkCostContainer[0]))
	{
		if (SUCCESS == spclSectionDirector->StartCalculateSection(clRequest, mp->m_lRouteID))
		{
			if (!mp->m_spclLinkRoutingTile.Create(mp->m_spclDataProvider)){
				ERR("");
				return false;
			}

			SmartPointer<CRPSection>& spclSection = spclSectionDirector->m_spclSection;
			for (size_t idxLink = 0; idxLink < spclSection->m_vclRSLinkList.size(); idxLink++)
			{
				CRPRSLink &link = spclSection->m_vclRSLinkList[idxLink];
				vector<CPointCoord2D> shapePoints;
				CRPRCLinkID linkID(link.m_clTileID, link.m_usLinkID, link.m_bPos);
				vector<CRPRCLinkID> vclLevel13LinkIds; 

				_DownTo13Level(mp->m_spclLinkRoutingTile, linkID, vclLevel13LinkIds);

				if (vclLevel13LinkIds.size() == 0)
					vclLevel13LinkIds.push_back(linkID);

				for (size_t idx13LevelLink = 0; idx13LevelLink < vclLevel13LinkIds.size(); idx13LevelLink++)
				{
					mp->m_spclRouteDataLoader->MakeLinkShapePointList(vclLevel13LinkIds[idx13LevelLink], shapePoints);
				}

#if 0
				link.setShapePoints(&shapePoints);
#else
				//return false;
#endif
			}

		}
	}

	return false;
}

bool CRouteSession::extrackRouteResult()
{
	//mp->
	return false;
}