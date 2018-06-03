#pragma once
#include "NaviApiDll.hpp"
#include "navapi_route.hpp"
#pragma region Class 

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