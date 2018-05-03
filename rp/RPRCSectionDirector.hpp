#pragma once
#include <memory>

class CRPRCSectionDirector :
	public CBaseObj
{
public:
	CRPRCSectionDirector();
	virtual ~CRPRCSectionDirector();
	bool StartCalculateSection();
private:
	std::shared_ptr<CBaseObj> m_termSearch;
	std::shared_ptr<CBaseObj> m_upSearch;
	std::shared_ptr<CBaseObj> m_connectSearch;
	std::shared_ptr<CBaseObj> m_routeEdit;

};

