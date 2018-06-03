#pragma once
#include "NaviApiDll.hpp"

#pragma region Class 
#include <memory>
#include <tuple>
#include <cstdint>

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
	std::tuple<
		uint16_t,	// link id
		uint32_t,	// packed tile id
		int16_t>	// region id
		getLinkId();
private:
	friend CSectResultProxy;
	CPrivate* mp;
};
#pragma endregion