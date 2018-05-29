#ifndef RPROUTEDATALOADER_H
#define RPROUTEDATALOADER_H

class CRPRouteDataLoader : public CBaseObj
{
public:
	CRPRouteDataLoader();
	virtual ~CRPRouteDataLoader();

public:
	// Initialize
	RESULT Initialize(SmartPointer< CDPFacade > spclDataProvide);

	//get the shape points from database
	void MakeLinkShapePointList(CRPRCLinkID& routeLinkId, vector<CPointCoord2D>& vecShapePointList);

	//get attribute ...and so on..

private:

	SmartPointer<CDPFacade> m_spclDataProvider;
};

#endif	//	RPRCROUTECALCCOMMON_H
