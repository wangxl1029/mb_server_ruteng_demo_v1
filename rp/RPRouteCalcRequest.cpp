#include "InnerCommon.hpp"
#include "RPCommon.hpp"
#include "RPRouteCalcRequest.hpp"

//	CRPWayPoints
CRPWayPoints::CRPWayPoints()
{
}

CRPWayPoints::~CRPWayPoints()
{
}

size_t CRPWayPoints::Size()
{
	return m_vclWayPointList.size();
}

CRPWayPoint& CRPWayPoints::At(size_t uiNo)
{
	return m_vclWayPointList[uiNo];
}

void CRPWayPoints::PushBack(const CRPWayPoint &clWayPoint)
{
	m_vclWayPointList.push_back(clWayPoint);
}
