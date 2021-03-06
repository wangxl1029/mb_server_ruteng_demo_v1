#pragma once
//typedef		long					RESULT;

#define	RP_RETURN_CANCEL										(100)
#define	RP_RETURN_CONTINUE										(200)

#define	RP_ROUTE_SEARCH_TYPE_COUNT								(5)
#define	RP_ROUTE_SEARCH_TYPE_NONE								(0x0)
#define	RP_ROUTE_SEARCH_TYPE_RECOMMEND							(0x1)
#define	RP_ROUTE_SEARCH_TYPE_GENERAL							(0x2)
#define	RP_ROUTE_SEARCH_TYPE_DIST								(0x4)
#define	RP_ROUTE_SEARCH_TYPE_WIDTH								(0x8)
#define	RP_ROUTE_SEARCH_TYPE_OTHER								(0x10)
#define	RP_ROUTE_SEARCH_TYPE_ALL								(0x1F)

enum RP_TERM
{
	RP_TERM_START,
	RP_TERM_END,
	RP_TERM_COUNT,
};

inline RP_TERM RP_AnotherTerm(RP_TERM enTerm)
{
	return (enTerm == RP_TERM_START) ? RP_TERM_END : RP_TERM_START;
}

class CRPTileID
{
public:
	CRPTileID(uint uiPackedTileID = NDS_INVALID_TILE_ID, short sUpdateRegionID = NDS_INVALID_UPDATE_REGION_ID)
		: m_uiPackedTileID(uiPackedTileID), m_sUpdateRegionID(sUpdateRegionID) {}

	uint														m_uiPackedTileID;
	short														m_sUpdateRegionID;
};

inline bool operator<(const CRPTileID &l, const CRPTileID &r)
{
	if (l.m_uiPackedTileID < r.m_uiPackedTileID) {
		return true;
	}
	else if (l.m_uiPackedTileID > r.m_uiPackedTileID) {
		return false;
	}
	else if (l.m_sUpdateRegionID < r.m_sUpdateRegionID) {
		return true;
	}
	else {
		return false;
	}
}

inline bool operator==(const CRPTileID &l, const CRPTileID &r)
{
	return l.m_uiPackedTileID == r.m_uiPackedTileID && l.m_sUpdateRegionID == r.m_sUpdateRegionID;
}

