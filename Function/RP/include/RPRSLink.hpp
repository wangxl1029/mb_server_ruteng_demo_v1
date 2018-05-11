#pragma once

class CRPRSLink : public CBaseObj
{
public:
	CRPRSLink(const CRPTileID &clTileID = CRPTileID(), ushort usLinkID = NDS_INVALID_LINK_ID,
		bool m_bPos = true, bool bFirstHalf = true, uint uiDist = 0);
	virtual ~CRPRSLink();

public:
	CPointCoord2D												m_clStartPoint;
	CPointCoord2D												m_clEndPoint;
	CRPTileID													m_clTileID;
	ushort														m_usLinkID;
	bool														m_bPos;
	bool														m_bFirstHalf;
	uint														m_uiDist;
};

bool operator==(const CRPRSLink &clLeft, const CRPRSLink &clRight);