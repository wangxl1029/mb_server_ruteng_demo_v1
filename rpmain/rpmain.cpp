// rpmain.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "RouteSession.hpp"
#include <iostream>
#include <cstdlib>

int _tmain(int argc, _TCHAR* argv[])
{
	CRouteSession sess;

	if (sess.Initialize())
	{
		if (sess.calcRoute(1389087203, 476456031, 1388266368, 475637856)){
			std::cout << "route OK." << std::endl;
		}
		else
		{
			std::cerr << "route FAILED!" << std::endl;
		}
	}

	std::system("pause");

	return 0;
}

