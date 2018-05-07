
////////////////////////////////////////////////////////////////////////////////
//
// All Rights Reserved, Copyright(C),2013
// Licenced Material of by zhaoyiwei
// This file is auto generated by IOLibMaker, Do NOT modify
//
////////////////////////////////////////////////////////////////////////////////
//
// SubSystem: NDS_UpdCityInfoListNDS_UpdCityInfoListのデータアクセス用のライブラリ
// Class Name:    NDS_UpdCityInfoListNDS_UpdCityInfoListの内部のWriteBack用の情報
//
////////////////////////////////////////////////////////////////////////////////
#ifndef	__NDS_UPDCITYINFOLIST_INSIDEREF_H_
#define	__NDS_UPDCITYINFOLIST_INSIDEREF_H_


class CNDS_UpdCityInfoList_NDS_UpdateInfo_InsideRef
{
public:
	CNDS_UpdCityInfoList_NDS_UpdateInfo_InsideRef()
		: m_ullCategoryCnt_DA( 0xFFFFFFFFFFFFFFFF )
		, m_uiUpdInfoList_COUNT( 0 )
	{
	}

public:
		uint64													m_ullCategoryCnt_DA;
		uint													m_uiUpdInfoList_COUNT;
};

class CNDS_UpdCityInfoList_NDS_UpdCityInfo_InsideRef
{
public:
	CNDS_UpdCityInfoList_NDS_UpdCityInfo_InsideRef()
	{
	}

public:
		CNDS_UpdCityInfoList_NDS_UpdateInfo_InsideRef			m_clUpdInfo;
};

class CNDS_UpdCityInfoList_NDS_UpdProvinceInfo_InsideRef
{
public:
	CNDS_UpdCityInfoList_NDS_UpdProvinceInfo_InsideRef()
		: m_ullCityCnt_DA( 0xFFFFFFFFFFFFFFFF )
		, m_uiCityList_COUNT( 0 )
	{
	}

public:
		uint64													m_ullCityCnt_DA;
		CNDS_UpdCityInfoList_NDS_UpdateInfo_InsideRef			m_clUpdInfo;
		CNDS_UpdCityInfoList_NDS_UpdCityInfo_InsideRef			m_clCityList;
		uint													m_uiCityList_COUNT;
};

class CNDS_UpdCityInfoList_NDS_UpdCityInfoListDataFrame_InsideRef
{
public:
	CNDS_UpdCityInfoList_NDS_UpdCityInfoListDataFrame_InsideRef()
		: m_ullProvinceCnt_DA( 0xFFFFFFFFFFFFFFFF )
		, m_uiProvinceList_COUNT( 0 )
	{
	}

public:
		uint64													m_ullProvinceCnt_DA;
		CNDS_UpdCityInfoList_NDS_UpdateInfo_InsideRef			m_clUpdInfo;
		CNDS_UpdCityInfoList_NDS_UpdProvinceInfo_InsideRef		m_clProvinceList;
		uint													m_uiProvinceList_COUNT;
};

class CNDS_UpdCityInfoList_NDS_UpdRegionInfo_InsideRef
{
public:
	CNDS_UpdCityInfoList_NDS_UpdRegionInfo_InsideRef()
	{
	}

public:
		CNDS_UpdCityInfoList_NDS_UpdateInfo_InsideRef			m_clUpdateInfo;
};

#endif	//	__NDS_UPDCITYINFOLIST_INSIDEREF_H_
