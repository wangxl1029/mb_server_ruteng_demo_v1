#pragma once
#include <memory>
using namespace std;
class CRouteSession
{
private:
	class CPrivate;
public:
	CRouteSession();
	~CRouteSession();

	bool Initialize();
	bool calcRoute(int iFromX, int iFromY, int iToX, int iToY);
	bool extrackRouteResult();
private:
	shared_ptr<CPrivate>										mp;
};