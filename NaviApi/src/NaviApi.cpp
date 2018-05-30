// navapi.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "NaviApiDll.hpp"
#include "RouteSession.hpp"

#pragma region Class 

struct CSectResultLinkProxy::CPrivate
{
	shared_ptr<nsRouteSession::CSectionLink> m_spLink;
	shared_ptr< vector< pair<int, int> > > m_spPosVec;
};

CSectResultLinkProxy::CSectResultLinkProxy() : mp(new CPrivate)
{
}
void CSectResultLinkProxy::getStartPos(int& x,  int& y)
{
	std::tie(x, y) = mp->m_spLink->getStartPos();
}

CSectResultLinkProxy::~CSectResultLinkProxy()
{}

void CSectResultLinkProxy::getEndPos(int& x, int& y)
{
	std::tie(x, y) = mp->m_spLink->getEndPos();
}

struct CSectResultProxy::CPrivate
{
	shared_ptr<nsRouteSession::CSectionResult> spSection;
};

CSectResultProxy::CSectResultProxy() : mp(new CPrivate)
{}

CSectResultProxy::~CSectResultProxy()
{
	delete mp;
}

struct CNdsNaviSession::CPrivate
{
	shared_ptr<CRouteSession> spRouteSess;
	vector< shared_ptr<CSectResultLinkProxy> > mVecLinkProxy;
};

CNdsNaviSession::CNdsNaviSession() : mp(new(CPrivate))
{}

CNdsNaviSession::~CNdsNaviSession()
{
	delete mp;
}

bool CNdsNaviSession::Initialize()
{
	if (!mp->spRouteSess)
	{
		mp->spRouteSess = std::make_shared<CRouteSession>();
	}

	return mp->spRouteSess->Initialize();
}

bool CNdsNaviSession::calcRoute(int iFromX, int iFromY, int iToX, int iToY)
{
	bool retval = mp->spRouteSess->calcRoute(iFromX, iFromY, iToX, iToY);
	return retval;
}

shared_ptr<CSectResultProxy> CNdsNaviSession::getRouteResult()
{
	auto result = make_shared<CSectResultProxy>();
	result->mp->spSection = mp->spRouteSess->getSectionRouteResult();
	return result;
}

size_t CSectResultProxy::getLinkNum()
{
	return mp->spSection->getLinkNum();
}

shared_ptr<CSectResultLinkProxy> CSectResultProxy::getLinkAt(size_t idx)
{
	auto spLnkPrxy = make_shared<CSectResultLinkProxy>();
	auto linkNum = mp->spSection->getLinkNum();
	if (idx < linkNum)
	{
		spLnkPrxy->mp->m_spLink = mp->spSection->getLinkAt(idx);
		//spLnkPrxy->mp->m_spPosVec->emplace_back(spLnkPrxy->mp->m_spLink->getStartPos());
		//spLnkPrxy->mp->m_spPosVec->emplace_back(spLnkPrxy->mp->m_spLink->getEndPos());
	}
	else
	{
		spLnkPrxy.reset();
	}

	return spLnkPrxy;
}


#pragma endregion