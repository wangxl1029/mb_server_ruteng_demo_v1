#include	"InnerCommon.hpp"
#include	"RPCommon.hpp"
#include	"RPRSLink.hpp"

CRPRSLink::CRPRSLink(const CRPTileID &clTileID, ushort usLinkID, bool bPos,
	bool bFirstHalf, uint uiDist)
	: m_clTileID(clTileID), m_usLinkID(usLinkID), m_bPos(bPos)
	, m_bFirstHalf(bFirstHalf), m_uiDist(uiDist)
{
}

CRPRSLink::~CRPRSLink()
{
}

bool operator==(const CRPRSLink &clLeft, const CRPRSLink &clRight)
{
	return clLeft.m_clTileID == clRight.m_clTileID && clLeft.m_usLinkID == clRight.m_usLinkID && clLeft.m_bPos == clRight.m_bPos
		&& clLeft.m_bFirstHalf == clRight.m_bFirstHalf && clLeft.m_uiDist == clRight.m_uiDist;
}

