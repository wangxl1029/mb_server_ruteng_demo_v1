#pragma once

#include "RPRouteCalcRequest.hpp"
class CRPRCSectionDirector :
	public CBaseObj
{
public:
	CRPRCSectionDirector();
	virtual ~CRPRCSectionDirector();
	bool StartCalculateSection(CRPRouteCalcRequest &clRequest);
private:
	//std::shared_ptr<CRPRCTermSearch> m_termSearch;
	std::shared_ptr<CBaseObj> m_upSearch;
	std::shared_ptr<CBaseObj> m_connectSearch;
	std::shared_ptr<CBaseObj> m_routeEdit;

};

