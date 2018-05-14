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

private:
	shared_ptr<CPrivate>										mp;
};