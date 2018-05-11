#pragma once
class CRPSection : public CBaseObj
{
public:
	CRPSection();
	virtual ~CRPSection();

public:
	long														m_lSectionID;
	CRPWayPoint													m_clStartWayPoint;
	CRPWayPoint													m_clEndWayPoint;
	vector< CRPRSLink >											m_vclRSLinkList;
};

