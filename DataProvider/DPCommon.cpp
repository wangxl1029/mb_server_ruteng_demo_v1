#include <cstdlib>
#include "CFBaseObj.hpp"
#include "DPCommon.hpp"

bool CDPCommon::CoordToTileNo(int iLevelNo, int iLong, int iLat, uint32_t& uiTileNo)
{
	if (iLevelNo < 0 || iLevelNo > 15)
	{
		return false;
	}

	//Modify by chenpeihua for ��������PackID���� on 2013-5-24 begin
	//========================
	//�����γ�ȷ�Χ����(-180~0),��Ҫ����γ��ת����(180~360)���м���
	uint32_t uiTmpX = 0;
	uint32_t uiTmpY = 0;
	//����
	if (iLong < 0){
		//NDS0001---Modified by LiuSq on 20130909 for Debug Unite state data begin
		//uiTmpX	=	(((uint64)1)<<32) + iLong;
		uiTmpX = (((uint64_t)1) << 31) + iLong;	//	zhaoyiwei
		//uiTmpX = 0xFFFFFFFF - ((uint)(-iLong)) + 1; //���Ϊ������������360�ȣ����Ϊ(0~180);
		//NDS0001---Modified by LiuSq on 20130909 for Debug Unite state data end
	}
	else{
		uiTmpX = static_cast<uint32_t>(iLong);
	}
	//γ��
	uiTmpY = 0x1 << 30;
	uiTmpY += iLat;

	int iBitNum = 31 - iLevelNo;

	uiTmpX = (uiTmpX >> iBitNum);
	uiTmpY = (uiTmpY >> iBitNum) & (0x7FFFFFFF >> iBitNum);
	//Modify by chenpeihua for ��������PackID���� on 2013-5-24 end

	uint32_t uiTileId = 0;
	uint32_t uiPos = 0;
	for (int i = iLevelNo; i >= 0; i--)
	{
		uiPos = (uiTmpX & 0x1);
		uiPos |= (uiTmpY & 0x1) << 1;

		if (1 == i)
		{
			uiPos ^= 0x2;
		}

		uiTileId |= uiPos << (2 * (iLevelNo - i));

		uiTmpX >>= 1;
		uiTmpY >>= 1;
	}

	if (uiTileId >= (1UL << (iLevelNo + 16)))
	{
		return false;
	}

	uiTileNo = uiTileId;

	return true;
}

// ȡ��ָ��Level����Tile�ľ�γ�ȿ��
bool CDPCommon::GetTileWidth(int iLevelNo, uint32_t& uiTileWidth)
{
	if (iLevelNo < 0 || iLevelNo > 15)
	{
		return false;
	}

	uiTileWidth = (1UL << (31 - iLevelNo));

	return true;
}


// ȡ��PackedTileId
bool CDPCommon::TileNoToPackedTileID(int iLevelNo, uint32_t uiTileNo, uint32_t& uiPackedTileId)
{
	uiPackedTileId = 0;
	if (iLevelNo >= 0 && iLevelNo <= 15){
		if (uiTileNo < (1UL << (iLevelNo + 16))){
			uiPackedTileId = (1UL << (16 + iLevelNo)) + uiTileNo;

			return true;
		}
	}

	return false;
}

// ȡ��ָ��TileNum�����½Ǿ�γ��
bool CDPCommon::GetLBPointCoordOfTile(int iLevelNo, uint32_t uiTileNo, int& iLong, int& iLat)
{
	if (iLevelNo < 0 || iLevelNo > 15)
	{
		return false;
	}

	if (uiTileNo >= (1UL << (iLevelNo + 16)))
	{
		return false;
	}

	uint32_t uiWidth = 0;
	uint32_t uiTmpId = uiTileNo;
	uint32_t uiTmpLong = 0;
	uint32_t uiTmpLat = 0;

	if (true == GetTileWidth(iLevelNo, uiWidth))
	{
		return false;
	}

	for (int i = iLevelNo; i >= 0; i--)
	{
		uint32_t uiPos = uiTmpId & 0x3;
		if (1 == i)
		{
			uiPos ^= 0x2;
		}

		if (uiPos & 0x1)
		{
			uiTmpLong += uiWidth;
		}

		if (uiPos & 0x2)
		{
			uiTmpLat += uiWidth;
		}

		uiTmpId >>= 2;
		uiWidth <<= 1;
	}

	iLong = (int)uiTmpLong;
	iLat = (int)uiTmpLat - (0x1 << 30);

	return true;
}

// ȡ��ָ��TileNum�����ϽǾ�γ��
bool CDPCommon::GetRTPointCoordOfTile(int iLevelNo, uint32_t uiTileNo, int& iLong, int& iLat)
{
	if (false == GetLBPointCoordOfTile(iLevelNo, uiTileNo, iLong, iLat))
	{
		return false;
	}

	uint32_t uiWidth = 0;

	if (false == GetTileWidth(iLevelNo, uiWidth))
	{
		return false;
	}

	iLong += uiWidth;
	iLat += uiWidth;

	return true;
}

// ȡ��ָ��TileNum�Ļ�׼�㾭γ��
bool CDPCommon::GetBasePointCoordOfTile(int iLevelNo, uint32_t uiTileNo, int& iLong, int& iLat)
{
	if (false == GetLBPointCoordOfTile(iLevelNo, uiTileNo, iLong, iLat))
	{
		return false;
	}

	uint32_t uiWidth = 0;

	if (false == GetTileWidth(iLevelNo, uiWidth))
	{
		return false;
	}

	iLong += uiWidth / 2;
	iLat += uiWidth / 2;

	return false;
}

int CDPCommon::UpdateRegionName2Id(const char *pcUpdateRegionName)
{
	if (pcUpdateRegionName[0] == 'C') {
		++pcUpdateRegionName;
	}
	return std::strtol(pcUpdateRegionName, NULL, 16);
}