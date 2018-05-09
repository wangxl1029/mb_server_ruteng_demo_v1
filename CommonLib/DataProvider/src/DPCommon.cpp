#include "InnerCommon.hpp"
#include "DPCommon.hpp"


const char	*DP_DATA_DIR_NAME = "DATA";
const char	*DP_BACKUP_DIR_NAME = "BACKUP";
const char	*DP_DOWNLOAD_DIR_NAME = "DOWNLOAD";
const char	*DP_UNPACKED_DIR_NAME = "UNPACKED";
const char	*DP_NDS_DB_SUFFIX = ".NDS";
const char	*DP_BACKUP_DB_SUFFIX = ".bak";
const char	*DP_EXCLUDE_DB_SUFFIX = ".exclude.rid";
const char	*DP_INSUPD_DB_SUFFIX = ".insupd";
const char	*DP_DELETE_DB_SUFFIX = ".del";
const char	*DP_INSUPD_RID_DB_SUFFIX = ".insupd.rid";
const char	*DP_DELETE_RID_DB_SUFFIX = ".del.rid";
const char	*DP_DELETE_RID_DUP_DB_SUFFIX = ".del.rid.dup";


const char	*PRODUCT_NAME = "PRODUCT";



const char* DP_GetRootDirName()
{
	static const char s_acRootDirName[] = "D:\\ZNavi\\Data\\20160802/";
	return s_acRootDirName;
}
//
#define		ROUND(x)	     ((int)((double)x + 0.5001))
//NDS1348---modified by zhang-biao on 20131022 begin
#define		ROUNDDEC(x)		((int)((double)x - 0.5001 ) )
//NDS1348---modified by zhang-biao on 20131022 end
// ����Level���ź;�γ�ȼ���TileNum
RESULT CDPCommon::CoordToTileNo(int iLevelNo, int iLong, int iLat, uint& uiTileNo)
{
	if (iLevelNo < 0 || iLevelNo > 15)
	{
		return FAILURE;
	}

	//Modify by chenpeihua for ��������PackID���� on 2013-5-24 begin
	//========================
	//�����γ�ȷ�Χ����(-180~0),��Ҫ����γ��ת����(180~360)���м���
	uint uiTmpX = 0;
	uint uiTmpY = 0;
	//����
	if (iLong < 0){
		//NDS0001---Modified by LiuSq on 20130909 for Debug Unite state data begin
		//uiTmpX	=	(((uint64)1)<<32) + iLong;
		uiTmpX = (((uint64)1) << 31) + iLong;	//	zhaoyiwei
		//uiTmpX = 0xFFFFFFFF - ((uint)(-iLong)) + 1; //���Ϊ������������360�ȣ����Ϊ(0~180);
		//NDS0001---Modified by LiuSq on 20130909 for Debug Unite state data end
	}
	else{
		uiTmpX = static_cast<uint>(iLong);
	}
	//γ��
	uiTmpY = 0x1 << 30;
	uiTmpY += iLat;

	int iBitNum = 31 - iLevelNo;

	uiTmpX = (uiTmpX >> iBitNum);
	uiTmpY = (uiTmpY >> iBitNum) & (0x7FFFFFFF >> iBitNum);
	//Modify by chenpeihua for ��������PackID���� on 2013-5-24 end

	uint uiTileId = 0;
	uint uiPos = 0;
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
		return FAILURE;
	}

	uiTileNo = uiTileId;

	return SUCCESS;
}

// ȡ��PackedTileId
RESULT CDPCommon::TileNoToPackedTileID(int iLevelNo, uint uiTileNo, uint& uiPackedTileId)
{
	uiPackedTileId = 0;
	if (iLevelNo >= 0 && iLevelNo <= 15){
		if (uiTileNo < (1UL << (iLevelNo + 16))){
			uiPackedTileId = (1UL << (16 + iLevelNo)) + uiTileNo;

			return SUCCESS;
		}
	}
	return FAILURE;
}

// ����PackedTileIdȡ��Leve���ź�TileNum
RESULT CDPCommon::PackedTileIDToTileNo(uint uiPacketedTileId, int& iLevelNo, uint& uiTileNo)
{
	uint uiTmpId = uiPacketedTileId >> 16;
	iLevelNo = -1;
	while (uiTmpId > 0)
	{
		uiTmpId >>= 1;
		++iLevelNo;
	}

	if (iLevelNo < 0 || iLevelNo > 15)
	{
		return FAILURE;
	}

	uiTileNo = uiPacketedTileId - (1UL << (iLevelNo + 16));

	if (uiTileNo >= (1UL << (iLevelNo + 16)))
	{
		return FAILURE;
	}

	return SUCCESS;
}

// ����Level���ź;�γ�ȼ���ȡ��PackedTileId
RESULT CDPCommon::CoordToPackedTileID(int iLevelNo, int iNdsLong, int iNdsLat, uint& uiPackedTileId)
{
	uint	uiTileNum = 0;
	if (FAILURE == CoordToTileNo(iLevelNo, iNdsLong, iNdsLat, uiTileNum))
	{
		return FAILURE;
	}

	if (FAILURE == TileNoToPackedTileID(iLevelNo, uiTileNum, uiPackedTileId))
	{
		return FAILURE;
	}

	return SUCCESS;
}

RESULT CDPCommon::GetUpperTileID(uint uiPackedTileID, int iUpperLevel, uint &uiUpperTileID)
{
	int		iLevel = NDS_LEVEL_MAX;
	uint	uiTileNo = 0;
	CDPCommon::PackedTileIDToTileNo(uiPackedTileID, iLevel, uiTileNo);
	int		iLon = 0;
	int		iLat = 0;
	CDPCommon::GetBasePointCoordOfTile(iLevel, uiTileNo, iLon, iLat);
	CDPCommon::CoordToPackedTileID(iUpperLevel, iLon, iLat, uiUpperTileID);

	return SUCCESS;
}
// ȡ��ָ��Level����Tile�ľ�γ�ȿ��
RESULT CDPCommon::GetTileWidth(int iLevelNo, uint& uiTileWidth)
{
	if (iLevelNo < 0 || iLevelNo > 15)
	{
		return FAILURE;
	}

	uiTileWidth = (1UL << (31 - iLevelNo));

	return SUCCESS;
}


// ȡ��ָ��TileNum�����½Ǿ�γ��
RESULT CDPCommon::GetLBPointCoordOfTile(int iLevelNo, uint uiTileNo, int& iLong, int& iLat)
{
	if (iLevelNo < 0 || iLevelNo > 15)
	{
		return FAILURE;
	}

	if (uiTileNo >= (1UL << (iLevelNo + 16)))
	{
		return FAILURE;
	}

	uint uiWidth = 0;
	uint uiTmpId = uiTileNo;
	uint uiTmpLong = 0;
	uint uiTmpLat = 0;

	if (FAILURE == GetTileWidth(iLevelNo, uiWidth))
	{
		return FAILURE;
	}

	for (int i = iLevelNo; i >= 0; i--)
	{
		uint uiPos = uiTmpId & 0x3;
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

	return SUCCESS;
}

// ȡ��ָ��TileNum�����ϽǾ�γ��
RESULT CDPCommon::GetRTPointCoordOfTile(int iLevelNo, uint uiTileNo, int& iLong, int& iLat)
{
	if (FAILURE == GetLBPointCoordOfTile(iLevelNo, uiTileNo, iLong, iLat))
	{
		return FAILURE;
	}

	uint uiWidth = 0;

	if (FAILURE == GetTileWidth(iLevelNo, uiWidth))
	{
		return FAILURE;
	}

	iLong += uiWidth;
	iLat += uiWidth;

	return SUCCESS;
}

// ȡ��ָ��TileNum�Ļ�׼�㾭γ��
RESULT CDPCommon::GetBasePointCoordOfTile(int iLevelNo, uint uiTileNo, int& iLong, int& iLat)
{
	if (FAILURE == GetLBPointCoordOfTile(iLevelNo, uiTileNo, iLong, iLat))
	{
		return FAILURE;
	}

	uint uiWidth = 0;

	if (FAILURE == GetTileWidth(iLevelNo, uiWidth))
	{
		return FAILURE;
	}

	iLong += uiWidth / 2;
	iLat += uiWidth / 2;

	return SUCCESS;
}
// ���ݾ�γ��ȡ��Ī����
RESULT CDPCommon::CoordToMortonCode(int iLong, int iLat, uint64& uiMortonCode)
{
	uiMortonCode = 0;
	for (int i = 0; i < 32; i++)
	{
		uint64 uiTmp64 = 0;
		uiTmp64 = (iLong >> i) & 0x1;
		uiTmp64 |= ((iLat >> i) & 0x1) << 1;

		uiMortonCode |= (uiTmp64 << 2 * i);
	}
	return SUCCESS;
}

// ����Ī����ȡ�þ�γ��
RESULT CDPCommon::MortonCodeToCoord(uint64 uiMortonCode, int& iLong, int& iLat)
{
	iLong = 0;
	iLat = 0;
	for (int i = 0; i < 32; i++)
	{
		uint uiTmpCode = uiMortonCode & 0x3;

		iLong |= ((uiTmpCode & 0x1) << i);
		iLat |= (((uiTmpCode & 0x2) >> 1) << i);

		uiMortonCode >>= 2;
	}
	return SUCCESS;
}
// ����Tile�Ļ�׼����������滯������㾭γ��
RESULT CDPCommon::NormCoordToCoord(int		iBasePointLong,
	int		iBasePointLat,
	int		iNormCoorX,
	int		iNormCoorY,
	int		iCoordShift,
	int&		iLongitude,
	int&		iLatitude)
{
	iLongitude = iBasePointLong + (iNormCoorX << iCoordShift);
	iLatitude = iBasePointLat + (iNormCoorY << iCoordShift);
	return SUCCESS;
}

// ����Tile�Ļ�׼������;�γ�ȼ������滯����
RESULT CDPCommon::CoordAndBaseToNormCoord(int	iBasePointLong,
	int	iBasePointLat,
	int	iLongitude,
	int	iLatitude,
	int	iCoordShift,
	int&	iNormCoorX,
	int&	iNormCoorY)
{
	//NDS1012---Modified by LiuSq on 20130909 for 32-64 System Migration begin
	float value = (iLongitude - iBasePointLong) / pow((float)2, iCoordShift);
	//NDS1348---modified by zhang-biao on 20131022 begin 
	if (value < 0) {
		iNormCoorX = ROUNDDEC(value);
		if (iNormCoorX < (-4096)) {
			iNormCoorX = (-4096);
		}
	}
	else {
		iNormCoorX = ROUND(value);
		if (iNormCoorX > 4096) {
			iNormCoorX = 4096;
		}
	}

	value = 0.0;
	value = (iLatitude - iBasePointLat) / pow((float)2, iCoordShift);
	if (value < 0) {
		iNormCoorY = ROUNDDEC(value);
		if (iNormCoorY < (-4096)) {
			iNormCoorY = (-4096);
		}
	}
	else {
		iNormCoorY = ROUND(value);
		if (iNormCoorY > 4096) {
			iNormCoorY = 4096;
		}
	}

	//NDS1348---modified by zhang-biao on 20131022 end
	//NDS1012---Modified by LiuSq on 20130909 for 32-64 System Migration end
	return SUCCESS;
}

// NDS7009 delete by huhuan for "DBcom��������" on 20140116 begin
// ����Tile���ź����滯������㾭γ��
// RESULT CDPCommon::GetLLByTileNormCoor(int	iLevelNo, 
// 						uint	uiTileNo, 
// 						int		iNormCoorX, 
// 						int		iNormCoorY, 
// 						int		iCoordShift,
// 						int&	iLongitude,
// 						int&	iLatitude)
// {
// 	int iBasePointLong	= 0;
// 	int	iBasePointLat	= 0;
// 
// 	if (FAILURE == GetBasePointCoordOfTile(iLevelNo, uiTileNo, iBasePointLong, iBasePointLat))
// 	{
// 		return FAILURE;
// 	}
// 
// 	if (FAILURE == NormCoordToCoord(iBasePointLong, iBasePointLat, 
// 									iNormCoorX, iNormCoorY, iCoordShift, 
// 									iLongitude, iLatitude))
// 	{
// 		return FAILURE;
// 	}
// 
// 	return SUCCESS;
// }
// NDS7009 delete by huhuan for "DBcom��������" on 20140116 end

// ����Tile���ź;�γ�ȼ������滯����
RESULT CDPCommon::CoordAndTileToNormCoord(int	iLevelNo,
	uint	uiTileNo,
	int		iLongitude,
	int		iLatitude,
	int		iCoordShift,
	int&	iNormCoorX,
	int&	iNormCoorY)
{
	int iBasePointLong = 0;
	int	iBasePointLat = 0;

	if (FAILURE == GetBasePointCoordOfTile(iLevelNo, uiTileNo, iBasePointLong, iBasePointLat))
	{
		return FAILURE;
	}

	if (FAILURE == CoordAndBaseToNormCoord(iBasePointLong, iBasePointLat,
		iLongitude, iLatitude, iCoordShift,
		iNormCoorX, iNormCoorY))
	{
		return FAILURE;
	}

	return SUCCESS;
}

// 1/2560�������ϵ��NDS����ϵת��
RESULT CDPCommon::Cnv2560SecCoord2NdsCoord(int i2560SecLong, int i2560SecLat, int& iNdsLong, int& iNdsLat)
{
	int64 iLong64 = i2560SecLong;
	int64 iLat64 = i2560SecLat;

	iLong64 *= (1 << 30);
	iLat64 *= (1 << 30);

	double dLong = iLong64 / 2560.0 / 3600.0 / 90.0;
	double dLat = iLat64 / 2560.0 / 3600.0 / 90.0;

	iNdsLong = (int)floor(dLong);
	iNdsLat = (int)floor(dLat);

	return SUCCESS;
}

// NDS����ϵ��1/2560�������ϵת��
RESULT CDPCommon::CnvNdsCoordTo2560SecCoord(int iNdsLong, int iNdsLat, int& i2560SecLong, int& i2560SecLat)
{
	double dLong = iNdsLong * 3600.0 * 2560.0 * 90.0 / (1 << 30);
	double dLat = iNdsLat * 3600.0 * 2560.0 * 90.0 / (1 << 30);
	// 	
	// 	i2560SecLong	= dLong + 0.5;
	// 	i2560SecLat		= dLat  + 0.5;

	i2560SecLong = (int)floor(dLong);
	i2560SecLat = (int)floor(dLat);

	return SUCCESS;
}


// ����Level���ź;�γ�ȼ���ȡ��PackedTileId
RESULT CDPCommon::CoordToPackedTileIDBy2560Sec(int iLevelNo, int i2560Long, int i2560Lat, uint& uiPackedTileId)
{
	int iNdsLong = 0;
	int iNdsLat = 0;

	if (FAILURE == Cnv2560SecCoord2NdsCoord(i2560Long, i2560Lat, iNdsLong, iNdsLat))
	{
		return FAILURE;
	}

	if (FAILURE == CoordToPackedTileID(iLevelNo, iNdsLong, iNdsLat, uiPackedTileId))
	{
		return FAILURE;
	}

	return SUCCESS;
}

// ����ָ��PackedTileId�Ļ�׼�㾭γ��
RESULT CDPCommon::CalcBasePointCoordOfTile(uint uiPackedTileId, int& iNdsLong, int& iNdsLat)
{
	int	 iLevelNo = 0;
	uint uiTileNum = 0;
	if (FAILURE == PackedTileIDToTileNo(uiPackedTileId, iLevelNo, uiTileNum))
	{
		return FAILURE;
	}

	if (FAILURE == GetBasePointCoordOfTile(iLevelNo, uiTileNum, iNdsLong, iNdsLat))
	{
		return FAILURE;
	}

	return SUCCESS;
}

// ����ָ��PackedTileId�����¾�γ��
RESULT CDPCommon::CalcLBPointCoordOfTile(uint uiPackedTileId, int& iLBNdsLong, int& iLBNdsLat)
{
	int	 iLevelNo = 0;
	uint uiTileNum = 0;
	if (FAILURE == PackedTileIDToTileNo(uiPackedTileId, iLevelNo, uiTileNum))
	{
		return FAILURE;
	}

	if (FAILURE == GetLBPointCoordOfTile(iLevelNo, uiTileNum, iLBNdsLong, iLBNdsLat))
	{
		return FAILURE;
	}

	return SUCCESS;
}

// ��NDS��γ��ת��Ϊ���滯����
RESULT CDPCommon::CnvNdsCoord2NormCoord(uint uiPackedTileId, int iNdsLong, int iNdsLat, int iCoordShift, int& iNormCoorX, int& iNormCoorY)
{
	int		iBasePointLong = 0;
	int		iBasePointLat = 0;
	if (FAILURE == CalcBasePointCoordOfTile(uiPackedTileId, iBasePointLong, iBasePointLat))
	{
		return FAILURE;
	}

	if (FAILURE == CoordAndBaseToNormCoord(iBasePointLong, iBasePointLat,
		iNdsLong, iNdsLat, iCoordShift,
		iNormCoorX, iNormCoorY))
	{
		return FAILURE;
	}

	return SUCCESS;
}

size_t DP_BUILDING_BLOCK_ID_TO_NO(BUILDING_BLOCK_ID enBuildingBlockID)
{
	static BuildingBlockIDToNo< DP_BUILDING_BLOCK_ID_TO_NO >	s_clBuildingBlockIDToNo(DP_BUILDING_BLOCK_NO_TO_ID, DP_BUILDING_BLOCK_COUNT);
	return s_clBuildingBlockIDToNo(enBuildingBlockID);
}

string CDPCommon::UpdateRegionId2Name(int iUpdateRegionId)
{
	char	buf[10] = "";
#ifdef NEU_DB
	sprintf(buf, "%02X", iUpdateRegionId);
#else
	sprintf(buf, "C%01X", iUpdateRegionId);
#endif
	return string(buf);
}

int CDPCommon::UpdateRegionName2Id(const char *pcUpdateRegionName)
{
	if (pcUpdateRegionName[0] == 'C') {
		++pcUpdateRegionName;
	}
	return std::strtol(pcUpdateRegionName, NULL, 16);
}