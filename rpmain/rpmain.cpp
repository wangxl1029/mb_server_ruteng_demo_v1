// rpmain.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "RouteSession.hpp"

int _tmain(int argc, _TCHAR* argv[])
{
	CRouteSession sess;

	if (sess.Initialize())
	{
		sess.calcRoute(1389087203, 476456031, 1389087203, 476456031);
	}

	return 0;
}

