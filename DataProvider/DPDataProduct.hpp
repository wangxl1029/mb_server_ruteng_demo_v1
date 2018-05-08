#ifndef	DPDATAPRODUCT_H
#define	DPDATAPRODUCT_H

class CDPUpdateRegionInfo
{
public:
	CDPUpdateRegionInfo() {}

	string														m_strUpdateRegionName;
	CSL_RecUpdateRegionTable									m_clUpdateRegionRec;
};

class CDPGatewayInfo
{
public:
	CDPGatewayInfo() {}

	CSL_RecGlobalGatewayTable									m_clGatewayInfo;
};

class CDPDataProduct : public CDPData
{
public:
	CDPDataProduct() {}
	virtual ~CDPDataProduct() {}
};

#endif	//	DPDATAPRODUCT_H
