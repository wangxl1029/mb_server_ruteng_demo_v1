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
	shared_ptr<nsRouteSession::CSectionResult>					m_spclSingleSectResult;
	static volatile long										m_iNextRouteID;
};

volatile long		CRouteSession::CPrivate::m_iNextRouteID = 0;

struct nsRouteSession::CSectionLink::CPrivate
{
	CRPRSLink mRSLink;
};

nsRouteSession::CSectionLink::CSectionLink() : mp(make_shared<CPrivate>())
{}

shared_ptr< vector< pair<int, int> > > nsRouteSession::CSectionLink::getAllPos()
{
	auto spPosVec = make_shared< vector< pair<int, int> > >();
	auto& pos1 = mp->mRSLink.m_clStartPoint;
	spPosVec->push_back(make_pair(pos1.x, pos1.y));
	auto& pos2 = mp->mRSLink.m_clEndPoint;
	spPosVec->push_back(make_pair(pos2.x, pos2.y));

	return spPosVec;
}

pair<int, int> nsRouteSession::CSectionLink::getStartPos()
{
	auto& pos1 = mp->mRSLink.m_clStartPoint;
	return make_pair(pos1.x, pos1.y);
}

pair<int, int> nsRouteSession::CSectionLink::getEndPos()
{
	auto& pos = mp->mRSLink.m_clEndPoint;
	return make_pair(pos.x, pos.y);
}

struct nsRouteSession::CSectionResult::CPrivate
{
	SmartPointer< CRPSection >									m_spclSection;
};

nsRouteSession::CSectionResult::CSectionResult() : mp(make_shared<CPrivate>())
{}

size_t nsRouteSession::CSectionResult::getLinkNum()
{
	return mp->m_spclSection->m_vclRSLinkList.size();
}

shared_ptr<nsRouteSession::CSectionLink> nsRouteSession::CSectionResult::getLinkAt(size_t idx)
{
	auto spLink = make_shared<nsRouteSession::CSectionLink>();
	spLink->mp->mRSLink = mp->m_spclSection->m_vclRSLinkList[idx];
	return spLink;
}

CRouteSession::CRouteSession() : mp(make_shared<CPrivate>())
{}

bool CRouteSession::Initialize()
{
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

	mp->m_spclSingleSectResult = make_shared<nsRouteSession::CSectionResult>();

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

	// Դlink����tile���²�·�����ݣ�
	if (clOriTileProxy->m_clRoutingTile.m_clHeader.m_clContentMask.m_ucHasRouteDownLinkList)
	{
		CRouteDownLinkElement& clDownLinkElement = clOriTileProxy->m_clRoutingTile.m_clRouteDownLinks.m_vclRouteDownLinkElement[clOriLinkId.m_usLinkNo];
		// Դlink�²�·��������������0
		if (clDownLinkElement.m_ucNumRefs > 0)
		{
			for (size_t idxDownLink = 0; idxDownLink < clDownLinkElement.m_ucNumRefs; idxDownLink++)
			{
				CRPRCLinkID clDownLinkID;
				clDownLinkID.m_clTileID.m_uiPackedTileID = clOriTileProxy->m_clRoutingTile.m_clExternalTileIdList.m_viTileId[clDownLinkElement.m_vclToLink[idxDownLink].m_usExtTileIdx];
				// DownLinkElement�е�PositiveLinkDirection��ָ�����ԴLink�ķ��������Ƿ����˴���ͬ������
				clDownLinkID.m_bPos = !(clOriLinkId.m_bPos ^ (bool)clDownLinkElement.m_vclToLink[idxDownLink].m_clTileExternalLinkReference.m_ucPositiveLinkDirection);
				clDownLinkID.m_usLinkNo = clDownLinkElement.m_vclToLink[idxDownLink].m_clTileExternalLinkReference.m_usLinkId;
				clDownLinkID.m_clTileID.m_sUpdateRegionID = clOriLinkId.m_clTileID.m_sUpdateRegionID; // Link�����UR
				clTmpTileProxy.GetData(clDownLinkID.m_clTileID);
				// NDS2.5.1�±��, ����ֱ�ӽ�����13��
				if (clTmpTileProxy->m_iLevel == 13)
				{
					vclDownLinkIds.push_back(clDownLinkID);
				}
			}

			// �����ϴ˴�ֻ����1��2�������
			if (vclDownLinkIds.size() == 2)
			{
				CRPRCLinkID startLinkId;
				CRPRCLinkID endLinkId;
				// ԴLink������ȡ�Ͳ��һ��LinkΪ��ʼLink
				if (clOriLinkId.m_bPos)
				{
					startLinkId = vclDownLinkIds.at(0);
					endLinkId = vclDownLinkIds.at(1);
				}
				else // ԴLink�Ƿ�����ȡ�Ͳ��2��LinkΪ��ʼLink
				{
					startLinkId = vclDownLinkIds.at(1);
					endLinkId = vclDownLinkIds.at(0);
				}

				CRPRCLinkID midLinkId = startLinkId;
				vector<CRPRCLinkID> midLinks;

				midLinks.push_back(startLinkId);
				// ��ʼ��������̽����̽������Ϊfrc��ͬ��ȡ��Ϊ����Link
//				while (midLinkId != endLinkId)
				while (!(midLinkId == endLinkId))
				{
					CDPRoutingTileLinkNodeNo endNodeId;
					vector<CIntOrExtDirectedLinkReference>* connLinks;
					clMidTileProxy.GetData(midLinkId.m_clTileID);
					// ����Link����ȡendNode
					if (midLinkId.m_bPos)
						endNodeId = clMidTileProxy->m_clRoutingTileInfo.m_vclLinkList[midLinkId.m_usLinkNo].m_clEndNode;
					else
						endNodeId = clMidTileProxy->m_clRoutingTileInfo.m_vclLinkList[midLinkId.m_usLinkNo].m_clStartNode;
					// link��endNode����link���ڵ�tile��
					if (endNodeId.m_usNodeNo == 65535)
					{
						CRPTileID extTileId;
						CDPRoutingTileExtLinkID eLinkId;
						// ȡlink���������һ��tile��endNode�ڴ�tile��
						// TODO: ���linkΪ�����Ƿ���Ҫȡ��һ������tile��
#if 1
						assert(false); // hard codes to mark incompatable data struct
#else
						extTileId.m_uiPackedTileID = clMidTileProxy->m_clRoutingTile.m_clExternalTileIdList.m_viTileId[clMidTileProxy->m_clRoutingTile.m_clLink2TileList.m_vclTileRef[clMidTileProxy->m_clRoutingTileInfo.m_vclLinkList[midLinkId.m_usLinkNo].m_usLink2TileNo].m_clExtTileColl.m_vusExtTileIdx[clMidTileProxy->m_clRoutingTile.m_clLink2TileList.m_vclTileRef[clMidTileProxy->m_clRoutingTileInfo.m_vclLinkList[midLinkId.m_usLinkNo].m_usLink2TileNo].m_clExtTileColl.m_uiNumExtTileIds - 1]];
#endif
						extTileId.m_sUpdateRegionID = clOriLinkId.m_clTileID.m_sUpdateRegionID;
						clEndNodeTileProxy.GetData(extTileId);
						eLinkId.m_uiTileID = midLinkId.m_clTileID.m_uiPackedTileID;
						eLinkId.m_usLinkID = midLinkId.m_usLinkNo;
						// ��endNode����tile�в�ѯmidLink��tileId��linkNo
						endNodeId = clEndNodeTileProxy->m_clRoutingTileInfo.m_mapExtLink2Node.find(eLinkId)->second;
						connLinks = &clEndNodeTileProxy->m_clRoutingTile.m_clSimpleIntersection.m_vclSimpleIntersection[endNodeId.m_usNodeNo].m_vclConnectedLinks;
					}
					else // link��endNode��link���ڵ�tile��
					{
						clEndNodeTileProxy = clMidTileProxy;
						connLinks = &clEndNodeTileProxy->m_clRoutingTile.m_clSimpleIntersection.m_vclSimpleIntersection[endNodeId.m_usNodeNo].m_vclConnectedLinks;
					}

					for (size_t idxConnLink = 0; idxConnLink < connLinks->size(); idxConnLink++)
					{
						// ����link���������ڱ�tile��
						if (!(*connLinks)[idxConnLink].m_ucIsExternalLinkReference)
						{
							ushort usConnLinkId = (*connLinks)[idxConnLink].m_clLinkReferenceChoice.InternalLinkReference_Ref().m_usLinkId;
							// ����link��frc��midLink��frc��ͬ
							if (clEndNodeTileProxy->m_clRoutingTile.m_clFixedRoadAttributeSetList.m_vclAttributeList[clEndNodeTileProxy->m_clRoutingTile.m_clLinks.m_vclLink[usConnLinkId].m_clAttrInfo.FixedRoadAttributeSetListIndex_Ref()].m_clRoutingAttr.m_ucFunctionalRoadClass ==
								clMidTileProxy->m_clRoutingTile.m_clFixedRoadAttributeSetList.m_vclAttributeList[clMidTileProxy->m_clRoutingTile.m_clLinks.m_vclLink[midLinkId.m_usLinkNo].m_clAttrInfo.FixedRoadAttributeSetListIndex_Ref()].m_clRoutingAttr.m_ucFunctionalRoadClass)
							{
								CRPRCLinkID clConnLinkID;
								clConnLinkID.m_clTileID.m_uiPackedTileID = clEndNodeTileProxy->m_clTileID.m_uiPackedTileID;
								clConnLinkID.m_clTileID.m_sUpdateRegionID = clOriLinkId.m_clTileID.m_sUpdateRegionID; // Link�����UR
								clConnLinkID.m_bPos = (*connLinks)[idxConnLink].m_clLinkReferenceChoice.InternalLinkReference_Ref().m_ucPositiveLinkDirection;
								clConnLinkID.m_usLinkNo = usConnLinkId;
								// �ų���ͬlink����ͬ�����
								//if (!midLinkId.equal(clConnLinkID))
								if (!equal_except_pos(midLinkId, clConnLinkID))
								{
									midLinkId = clConnLinkID;
									midLinks.push_back(midLinkId);
									break;
								}
							}
						}
						else // ����link��������������tile��
						{
							CRPRCLinkID clExtConnLinkID;
							clExtConnLinkID.m_clTileID.m_uiPackedTileID = clEndNodeTileProxy->m_clRoutingTile.m_clExternalTileIdList.m_viTileId[(*connLinks)[idxConnLink].m_clLinkReferenceChoice.ExternalLinkReference_Ref().m_usExtTileIdx];
							clExtConnLinkID.m_usLinkNo = (*connLinks)[idxConnLink].m_clLinkReferenceChoice.ExternalLinkReference_Ref().m_clTileExternalLinkReference.m_usLinkId;
							clExtConnLinkID.m_clTileID.m_sUpdateRegionID = clOriLinkId.m_clTileID.m_sUpdateRegionID; // Link�����UR
							clExtConnLinkID.m_bPos = (*connLinks)[idxConnLink].m_clLinkReferenceChoice.ExternalLinkReference_Ref().m_clTileExternalLinkReference.m_ucPositiveLinkDirection;
							clExtTileProxy.GetData(clExtConnLinkID.m_clTileID);
							// ����link��frc��midLink��frc��ͬ
							if (clExtTileProxy->m_clRoutingTile.m_clFixedRoadAttributeSetList.m_vclAttributeList[clExtTileProxy->m_clRoutingTile.m_clLinks.m_vclLink[clExtConnLinkID.m_usLinkNo].m_clAttrInfo.FixedRoadAttributeSetListIndex_Ref()].m_clRoutingAttr.m_ucFunctionalRoadClass ==
								clMidTileProxy->m_clRoutingTile.m_clFixedRoadAttributeSetList.m_vclAttributeList[clMidTileProxy->m_clRoutingTile.m_clLinks.m_vclLink[midLinkId.m_usLinkNo].m_clAttrInfo.FixedRoadAttributeSetListIndex_Ref()].m_clRoutingAttr.m_ucFunctionalRoadClass)
							{
								// �ų���ͬlink����ͬ�����
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

	// �޵Ͳ�link
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

	mp->m_spclSingleSectResult->mp->m_spclSection = spclSectionDirector->m_spclSection;

	return true;
}

shared_ptr<nsRouteSession::CSectionResult> CRouteSession::getSectionRouteResult()
{
	return mp->m_spclSingleSectResult;
}

