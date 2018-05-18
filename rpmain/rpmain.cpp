// rpmain.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "RouteSession.hpp"
#include <iostream>
#include <cstdlib>

#ifdef WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
	CRouteSession sess;

	if (sess.Initialize())
	{
		if (sess.calcRoute(
			1389087203, 476456031, // start pos never change
			//1388266368, 475637856	// only connective level
			1388572928, 476910336	// up level
			)){
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

