#pragma once

#include <windows.h> 

#ifdef NAVIAPI_EXPORTS 
#define SYMBOL_DECLSPEC __declspec(dllexport) 
#define SYMBOL_DEF 
#else 
#define SYMBOL_DECLSPEC __declspec(dllimport) 
#define SYMBOL_DEF      __declspec(dllimport) 
#endif 



#pragma region Class 

class SYMBOL_DECLSPEC  CNdsRouteSession
{
	struct CPrivate;
public:
	CNdsRouteSession();
	~CNdsRouteSession();
	bool Initialize();
	bool calcRoute(int, int, int, int);
private:
	CPrivate* mp;
};

#pragma endregion
