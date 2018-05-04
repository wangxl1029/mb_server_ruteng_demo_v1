#pragma once

#include "RPRouteCalcRequest.hpp"
#include "DPFacade.hpp"
// class name CRPRCSectionDirector is not changed from znavi, easy to differ.
class CRPRCSectionDirector :
	public CBaseObj
{
public:
	CRPRCSectionDirector();
	virtual ~CRPRCSectionDirector();
	bool Initialize(std::shared_ptr< CDPFacade > spclDataProvider);
	bool StartCalculateSection(CRPRouteCalcRequest &clRequest);
	std::shared_ptr< CDPFacade >									m_spclDataProvider;
private:
	//std::shared_ptr<CRPRCTermSearch> m_termSearch;
	std::shared_ptr<CBaseObj> m_upSearch;
	std::shared_ptr<CBaseObj> m_connectSearch;
	std::shared_ptr<CBaseObj> m_routeEdit;

};

