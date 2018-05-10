#pragma once

//	class CDPRoutingTileLinkNodeNo
class CDPRoutingTileLinkNodeNo
{
public:
	CDPRoutingTileLinkNodeNo(ushort usNodeNo = NDS_INVALID_NODE_ID, ushort usConnNo = NDS_INVALID_NODE_ID)
		: m_usNodeNo(usNodeNo), m_usConnNo(usConnNo) {}
	~CDPRoutingTileLinkNodeNo() {}

public:
	ushort														m_usNodeNo;
	ushort														m_usConnNo;
};