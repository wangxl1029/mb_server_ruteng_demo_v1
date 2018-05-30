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
#include <memory>

class CNdsNaviSession;
class CSectResultProxy;
class SYMBOL_DECLSPEC CSectResultLinkProxy
{
	struct CPrivate;
public:
	CSectResultLinkProxy();
//	CSectResultLinkProxy(const CSectResultLinkProxy&);
	~CSectResultLinkProxy();
//	CSectResultLinkProxy& operator=(const CSectResultLinkProxy&);

	// 
	void getStartPos(int& x, int& y);
	void getEndPos(int& x, int& y);
private:
	friend CSectResultProxy;
	CPrivate* mp;
};

class SYMBOL_DECLSPEC CSectResultProxy
{
	struct CPrivate;
public:
	CSectResultProxy();
	~CSectResultProxy();
	size_t getLinkNum();
	std::shared_ptr<CSectResultLinkProxy> getLinkAt(size_t);
private:
	friend CNdsNaviSession;
	CPrivate* mp;
};

class SYMBOL_DECLSPEC  CNdsNaviSession
{
	struct CPrivate;
public:
	CNdsNaviSession();
	~CNdsNaviSession();
	bool Initialize();
	bool calcRoute(int, int, int, int);
	std::shared_ptr<CSectResultProxy> getRouteResult();
private:
	CPrivate* mp;
};

#pragma endregion
