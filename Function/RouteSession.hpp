#pragma once
#include <memory>
#include <vector>
#include <tuple>
using namespace std;

class CRouteSession;
namespace nsRouteSession
{
	class CSectionResult;
	class CSectionLink
	{
		struct CPrivate;
	public:
		CSectionLink();
		shared_ptr< vector< pair<int, int> > > getAllPos();
		pair<int, int> getStartPos();
		pair<int, int> getEndPos();
	private:
		friend CSectionResult;
		shared_ptr<CPrivate> mp;
	};

	class CSectionResult
	{
		struct CPrivate;
	public:
		CSectionResult();
		size_t getLinkNum();
		shared_ptr<CSectionLink> getLinkAt(size_t);
	private:
		friend CRouteSession;
		shared_ptr<CPrivate> mp;
	};
};

class CRouteSession
{
private:
	class CPrivate;
public:
	CRouteSession();

	bool Initialize();
	bool calcRoute(int iFromX, int iFromY, int iToX, int iToY);
	shared_ptr<nsRouteSession::CSectionResult> getSectionRouteResult();
private:
	shared_ptr<CPrivate> mp;
};