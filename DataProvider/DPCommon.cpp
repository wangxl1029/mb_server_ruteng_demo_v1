#include "CFBaseObj.hpp"
#include "DPCommon.hpp"

bool CDPCommon::CoordToTileNo(int iLevelNo, int iLong, int iLat, uint32_t& uiTileNo)
{
	if (iLevelNo < 0 || iLevelNo > 15)
	{
		return false;
	}

	//Modify by chenpeihua for 美国计算PackID错误 on 2013-5-24 begin
	//========================
	//如果经纬度范围从在(-180~0),需要将经纬度转换到(180~360)进行计算
	uint32_t uiTmpX = 0;
	uint32_t uiTmpY = 0;
	//经度
	if (iLong < 0){
		//NDS0001---Modified by LiuSq on 20130909 for Debug Unite state data begin
		//uiTmpX	=	(((uint64)1)<<32) + iLong;
		uiTmpX = (((uint64_t)1) << 31) + iLong;	//	zhaoyiwei
		//uiTmpX = 0xFFFFFFFF - ((uint)(-iLong)) + 1; //如果为西经，则增加360度，变更为(0~180);
		//NDS0001---Modified by LiuSq on 20130909 for Debug Unite state data end
	}
	else{
		uiTmpX = static_cast<uint32_t>(iLong);
	}
	//纬度
	uiTmpY = 0x1 << 30;
	uiTmpY += iLat;

	int iBitNum = 31 - iLevelNo;

	uiTmpX = (uiTmpX >> iBitNum);
	uiTmpY = (uiTmpY >> iBitNum) & (0x7FFFFFFF >> iBitNum);
	//Modify by chenpeihua for 美国计算PackID错误 on 2013-5-24 end

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

// 取得PackedTileId
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

