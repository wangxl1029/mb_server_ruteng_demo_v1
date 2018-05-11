#include	"InnerCommon.hpp"
#include	"RPCommon.hpp"
#include	"RPRSLink.hpp"
#include	"RPWayPoints.hpp"

//	CRPWayPoint
CRPWayPoint::CRPWayPoint()
: m_enWayPointType(RPWayPointType_Invalid)
{
}

CRPWayPoint::CRPWayPoint(const CPointCoord2D &clCoord)
: m_enWayPointType(RPWayPointType_Coord), m_clCoord(clCoord)
{
}

CRPWayPoint::CRPWayPoint(const CRPRSLink &clRSLink)
: m_enWayPointType(RPWayPointType_RSLink), m_clLink(clRSLink)
{
}

CRPWayPoint::~CRPWayPoint()
{
}

bool CRPWayPoint::operator==(const CRPWayPoint &clRight)
{
	if (m_enWayPointType != clRight.m_enWayPointType) {
		return false;
	}
	else if (m_enWayPointType == RPWayPointType_Coord) {
		return m_clCoord == clRight.m_clCoord;
	}
	else if (m_enWayPointType == RPWayPointType_RSLink) {
		return m_clLink == clRight.m_clLink;
	}
	else {
		return false;
	}
}

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
