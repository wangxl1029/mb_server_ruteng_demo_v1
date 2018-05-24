// rpmain.cpp : Defines the entry point for the console application.
//
#pragma region Includes and Imports
#include "stdafx.h"
#include <windows.h> 

// For error handling & advanced features (e.g. Unload the delayloaded DLL) 
#include <delayimp.h> 

#include "NaviApiDll.hpp"

// headers for main entry
#include <iostream>
#include <cstdlib>

#pragma endregion


#ifdef WIN32
BOOL IsModuleLoaded(PCWSTR pszModuleName)
{ 
	// Get the module in the process according to the module name. 
	HMODULE hMod = GetModuleHandle(pszModuleName);
	return (hMod != NULL);
}

int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
	BOOL fLoaded = FALSE;

	// The name of the module to be delay-loaded. 
	PCWSTR pszModuleName = L"NaviApi";

	// Check whether or not the module is loaded. 
	fLoaded = IsModuleLoaded(pszModuleName);
	wprintf(L"Module \"%s\" is %sloaded\n", pszModuleName, fLoaded ? L"" : L"not ");
	if (fLoaded)
	{

		CNdsRouteSession sess;
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
	}

	std::system("pause");

	return 0;
}

