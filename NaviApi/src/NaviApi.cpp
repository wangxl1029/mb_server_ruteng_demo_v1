// navapi.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "NaviApiDll.hpp"
#include "RouteSession.hpp"

#pragma region Class 
struct CNdsRouteSession::CPrivate
{
	std::shared_ptr<CRouteSession> mRouteSess;
};

CNdsRouteSession::CNdsRouteSession()
: mp(new CPrivate())
{
}

CNdsRouteSession::~CNdsRouteSession()
{
	delete mp;
}

bool CNdsRouteSession::Initialize()
{
	if (! mp->mRouteSess)
	{
		mp->mRouteSess = std::make_shared<CRouteSession>();
	}

	return mp->mRouteSess->Initialize();
}

bool CNdsRouteSession::calcRoute(int iFromX, int iFromY, int iToX, int iToY)
{
	return mp->mRouteSess->calcRoute(iFromX, iFromY, iToX, iToY);
}

#pragma endregion