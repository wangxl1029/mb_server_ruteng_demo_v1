// navapi.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "NaviApiDll.hpp"
#include "RouteSession.hpp"

#pragma region Class 
struct CNdsNaviSession::CPrivate
{
	std::shared_ptr<CRouteSession> mRouteSess;
};

CNdsNaviSession::CNdsNaviSession()
: mp(new CPrivate())
{
}

CNdsNaviSession::~CNdsNaviSession()
{
	delete mp;
}

bool CNdsNaviSession::Initialize()
{
	if (! mp->mRouteSess)
	{
		mp->mRouteSess = std::make_shared<CRouteSession>();
	}

	return mp->mRouteSess->Initialize();
}

bool CNdsNaviSession::calcRoute(int iFromX, int iFromY, int iToX, int iToY)
{
	return mp->mRouteSess->calcRoute(iFromX, iFromY, iToX, iToY);
}

bool CNdsNaviSession::extractRouteResult()
{
	return false;
}
#pragma endregion