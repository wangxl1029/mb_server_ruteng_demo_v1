﻿#ifndef	DPNDSTYPEDEF_H
#define	DPNDSTYPEDEF_H

enum AttributeTypeCode
{
	VALIDITY_RANGE		=	0,
	TIME_RANGE_OF_DAY		=	1,
	DATE_RANGE_OF_YEAR		=	2,
	DAYS_OF_WEEK	=	3,
	DAYS_OF_MONTH		=	4,
	DAY_OF_YEAR		=	5,
	MONTHS_OF_YEAR		=	6,
	TIME_DURATION		=	7,
	TRANSITION_MASK_2_4_VAL		=	8,
	TRANSITION_MASK_5_VAL		=	9,
	TRANSITION_MASK_6_8_VAL		=	10,
	TRANSITION_MASK_9_16_VAL		=	11,
	TIME_RANGE_OF_WEEK		=	12,
	SPECIAL_TRANSITION_CODE		=	13,
	RIGHT_OF_WAY_REGULATION		=	14,
	TRAFFIC_LIGHTS		=	16,
	SIGN_POSITION		=	17,
	CONSUMPTION_UP_EXCESS_SLOPE		=	18,
	CONSUMPTION_DOWN_EXCESS_SLOPE		=	19,
	CONSUMPTION_SPEED_VARIATION		=	20,
	CONSUMPTION_SPEED_DEPENDENCY		=	21,
	AVERAGE_SLOPE		=	22,
	LINK_PERCENTAGE		=	23,
	BUILDING_FLOOR_COUNT		=	24,
	PROHIBITED_PASSAGE		=	32,
	OVERTAKING_PROHIBITION		=	33,
	SPEED_LIMIT		=	34,
	MINIMUM_SPEED		=	35,
	EXPRESS_LANE		=	36,
	IS_COMPLETE_EXPRESS_ROAD		=	37,
	IS_DERIVED		=	38,
	ADVISORY_SPEED_LIMIT		=	39,
	WEIGHT_METRIC		=	40,
	WEIGHT_IMPERIAL		=	41,
	WEIGHT_PER_AXLE_METRIC		=	42,
	WEIGHT_PER_AXLE_IMPERIAL		=	43,
	VEHICLE_HEIGHT_METRIC		=	44,
	VEHICLE_HEIGHT_IMPERIAL		=	45,
	VEHICLE_WIDTH_METRIC		=	46,
	VEHICLE_WIDTH_IMPERIAL		=	47,
	LENGTH_METRIC		=	48,
	LENGTH_IMPERIAL		=	49,
	NUM_AXLES		=	50,
	FREQUENTLY_USED_VEHICLE_TYPES		=	56,
	SLOW_ROAD_USERS		=	57,
	BIG_VEHICLES		=	58,
	EQUIPMENT		=	59,
	PUBLIC_SERVICE_VEHICLES		=	60,
	AUTHORIZATION		=	61,
	LOAD		=	62,
	EMERGENCY_MILITARY_DETAILS		=	63,
	SIGNPOST_TOWARD_INFO_REFERENCE		=	64,
	SIGNPOST_DIRECTIONAL_INFO_REFERENCE		=	65,
	SIGNPOST_ICON_REFERENCE		=	66,
	SIGNPOST_EXIT_NUMBER		=	67,
	SIGNPOST_LINK_REFERENCE		=	68,
	SIGNPOST_BACKGROUND_REFERENCE		=	69,
	OCCUPANCY		=	70,
	LANE_MASK		=	71,
	EXIT_LANE_MASK		=	72,
	PARKING		=	73,
	TOURIST_ROUTE_TYPE		=	75,
	NUM_TOLL_BOOTHS		=	78,
	TOLL_BOOTH_MASK		=	79,
	TOLL_VIGNETTE		=	80,
	TOLL_ENTRY		=	81,
	TOLL_EXIT		=	82,
	TOLL_CHARGE		=	83,
	//	for windows
	NDS_CURRENCY		=	84,
	TOLL_GATEID		=	85,
	TOLL_PAYMENT_PROVIDER_ID		=	86,
	TOLL_PAYMENT_TYPE		=	87,
	NUM_LANES		=	88,
	LANE_CONNECTIVITY_ALONG_LINK		=	89,
	LANE_CONNECTIVITY_ACROSS_INTERSECTION		=	90,
	LANE_MARKING		=	91,
	LANE_SEPARATORS		=	92,
	LANE_PASSING		=	93,
	LANE_TURNING		=	94,
	LANE_USED_IN_BOTH_DIRECTIONS		=	95,
	PHYSICAL_WIDTH_METRIC		=	96,
	CURVATURE		=	97,
	TYPE_OF_PAVEMENT		=	98,
	OVERPASS		=	99,
	UNDERPASS		=	100,
	SLOPE_ARRAY		=	101,
	ELEVATION_DELTA		=	102,
	FERRY_TYPE		=	103,
	PHYSICAL_WIDTH_IMPERIAL		=	104,
	PHYSICAL_LANE_DIVIDER		=	105,
	TOLL_PAYMENT_PROVIDER_DEFINITION		=	106,
	TOLL_FEE_COLLECTION_TYPE		=	107,
	DIGITIZATION_STATUS		=	109,
	ATTRIBUTION_STATUS		=	111,
	AVERAGE_SPEED		=	114,
	URBAN		=	115,
	LINK_TYPE		=	116,
	TUNNEL		=	117,
	FERRY		=	118,
	TOLL		=	119,
	BRIDGE		=	120,
	NUM_TOLL_GATES		=	121,
	NUM_TRAFFIC_LIGHTS		=	122,
	NUM_GIVE_RIGHT_OF_WAY		=	123,
	LENGTH_ALONG_LINK		=	124,
	ICON_REFERENCE		=	125,
	ROTATED_ICON_REFERENCE		=	126,
	TURN_ON_RED_ALLOWED		=	132,
	ROAD_Z_LEVEL		=	133,
	DRAWING_ORDER		=	134,
	DAYTIME_RUNNING_LIGHT		=	135,
	ROAD_NUMBER_NAMED_OBJECT_REFERENCE		=	144,
	ROAD_NUMBER_NAMED_OBJECT_RELATIVE_REF		=	145,
	POSTAL_CODE_NAMED_OBJECT_RELATIVE_REF		=	146,
	ADMIN_HIERARCHY_NAMED_OBJECT_RELATIVE_REF		=	147,
	POSTAL_CODE_NAMED_OBJECT_REFERENCE		=	148,
	ADMIN_HIERARCHY_NAMED_OBJECT_REFERENCE		=	149,
	NAMED_OBJECT_REFERENCE		=	150,
	NAMED_OBJECT_BASE_REF		=	151,
	NAMED_OBJECT_RELATIVE_REF		=	152,
	CONTINUED_TURN_RESTRICTION		=	155,
	NAMED_OBJECT_POSITION		=	156,
	FLOOR_NUMBER		=	157,
	NAMED_OBJECT_CLASS		=	158,
	EXTENDED_POSTAL_CODE_POSITION		=	159,
	TOP_SELECTION_ENTRY		=	160,
	TOP_SELECTION_INDICATOR		=	161,
	NAMED_OBJECT_XREF_INDICATOR		=	162,
	SUPPLEMENTARY_HOUSE_NUMBER_RANGE		=	163,
	CALENDAR_WEEK		=	171,
	POPULATION		=	172,
	GATEWAY		=	173,
	KM_MI_INDICATOR		=	174,
	MULTI_DIGITIZED		=	175,
	MULTI_DIGITIZED_LINK_REFERENCE		=	176,
	METADATA_REGION_REFERENCE		=	177,
	NON_DEFAULT_TRAFFIC_SENSE		=	178,
	STUBBLE		=	179,
	DETACHED_FROM_TERRAIN		=	180,
	LOCAL_TMC_LOCATION		=	181,
	HOUSE_NUMBER_RANGE		=	182,
	SPEED_PROFILES_OR_AVG_SPEEDS_PER_WEEK		=	183,
	IS_ELEVATED_ROAD		=	184,
	WEEK_IN_MONTH		=	185,
	ADDRESSES_POINT		=	186,
	ATTRIBUTE_POINT_LIST		=	187,
	ODD_OR_EVEN_DAYS		=	188,
	WEEKDAY_IN_MONTH		=	189,
	TIME_ZONE_REFERENCE		=	190,
	IS_RACE_TRACK		=	191,
	BUILDING_HEIGHT		=	192,
	GROUND_HEIGHT		=	193,
	BASE_ELEVATION		=	194,
	ROOF_COLOR		=	195,
	WALL_COLOR		=	196,
	WARNING_SIGN		=	197,
	UNDER_CONSTRUCTION		=	198,
	RAILWAY_CROSSING		=	201,
	PEDESTRIAN_CROSSING		=	202,
	FEATURE_TYPE		=	203,
	WEATHER		=	204,
	LEGAL_SPEED_LIMIT_RANGE		=	205,
	SINGLE_OR_DUAL_CARRIAGEWAY		=	206,
	SEASONAL_CLOSED		=	207,
	FUZZY_TIME_DOMAIN		=	208,
	CONCRETE_NAMED_OBJECT		=	209,
	TRAFFIC_ZONE		=	210,
	FREQUENT_ACCIDENTS		=	211,
	NOT_USEFUL_FOR_BEGINNERS		=	213,
	GLOBAL_FEATURE_ID		=	214,
	FERRY_TRAVEL_TIME		=	215,
	SHORTCUT		=	216,
	BYPASS		=	217,
	SIDE_STREET		=	218,
	CLOTHOID_META_DATA		=	220,
	CLOTHOID		=	221,
	GRADIENT_META_DATA		=	222,
	GRADIENT		=	223,
	ENHANCED_GEOMETRY		=	224,
	ADAS_ACCURACY		=	225,
	START_OR_DESTINATION_ROAD_ONLY		=	227,
	BORDER_IS_DISPUTED_BY		=	228,
	BORDER_IS_ACCEPTED_BY		=	229,
	COMPLEX_INTERSECTION_TYPE		=	230,
	PATH_VECTOR_3D		=	239,
	SPLINE		=	240,
	TURN_GEOMETRY_CURVATURE		=	241,
	LABELING_HINT		=	242,
	POI_REFERENCE		=	243,
	PREFERRED_USAGE		=	244,
	IS_CARPOOL_LANE		=	245,
	IS_COMPLETE_CARPOOL_ROAD		=	246,
	ANTI_LABELING_HINT		=	247,
	ADDITIONAL_3D_FEATURE_CLASS		=	248,
	ORIGINAL_POINT_HEIGHTS		=	249,
	PLURAL_JUNCTION_TYPE		=	250,
	HAS_ADDITIONAL_DATA_ELEMENT_ID		=	254,
	HAS_ADDITIONAL_DATA		=	255
};

enum AttrMapType
{
	VALUES_TO_ONE_FEATURE 	= 0,
	VALUES_TO_MANY_FEATURES 	= 1,
	VALUES_TO_ALL_FEATURES 	= 2
};

enum ReferenceType
{
	ROUTING_SIMPLE_INTERSECTION 	= 0,
	ROUTING_LINK_DIRECTED 	= 1,
	ROUTING_LINK_BOTH_DIRECTIONS 	= 2,
	ROUTING_ROAD_GEO_LINE_DIRECTED 	= 3,
	ROUTING_ROAD_GEO_LINE_BOTH_DIRECTIONS 	= 4,
	ROUTING_SIMPLE_INTERSECTION_TRANSITION 	= 5,
	BMD_AREA_FEATURE 	= 6,
	BMD_LINE_FEATURE_DIRECTED 	= 7,
	BMD_LINE_FEATURE_BOTH_DIRECTIONS 	= 8,
	BMD_POINT_FEATURE 	= 9,
	ALL_TILE_FEATURES 	= 10
};

enum NameStringUsageType
{
	OFFICIAL_NAME 	= 0,
	ALTERNATE_NAME 	= 1,
	DEFAULT_OFFICIAL_NAME 	= 2,
	PREFERRED_ALTERNATE_NAME 	= 3
};

enum NameFormat
{
	NameFormat_NAME 	= 0,
	NameFormat_TELEPHONE_NUMBER 	= 1,
	NameFormat_OFFICIAL_ABBREVIATION 	= 2,
	NameFormat_ROAD_NUMBER 	= 3,
	NameFormat_HOUSE_NUMBER 	= 4,
	NameFormat_HOUSE_NUMBER_RANGE 	= 5,
	NameFormat_EXIT_NUMBER 	= 6,
	NameFormat_MOTORWAY_INTERSECTION_NAME 	= 7,
	NameFormat_ZIP_CODE_RANGE 	= 8,
	NameFormat_MOTORWAY_EXIT_NAME 	= 9,
	NameFormat_ADDRESSES_POINT 	= 10
};

enum BmdLineFeatureClass
{
	BMD_LINE 	= 0,
	LINE_ROAD 	= 1,
	LINE_BLOCK_DETAIL 	= 2,
	LINE_BUILDING_DETAIL 	= 3,
	LINE_PUBLIC_TRANSPORT 	= 4,
	LINE_RAILWAY 	= 5,
	LINE_RAILWAY_LONG_DISTANCE 	= 6,
	LINE_RAILWAY_REGIONAL 	= 7,
	LINE_RAILWAY_LOCAL 	= 8,
	LINE_SUBWAY 	= 9,
	LINE_TRAM 	= 10,
	LINE_MONORAIL 	= 11,
	LINE_BUS 	= 12,
	LINE_WATER 	= 13,
	LINE_CREEK 	= 14,
	LINE_DRAIN 	= 15,
	LINE_RIVER 	= 16,
	LINE_WADI 	= 17,
	LINE_CANAL 	= 18,
	LINE_BORDER 	= 19,
	LINE_BORDER_ORDER_0 	= 20,
	LINE_BORDER_ORDER_1 	= 22,
	LINE_BORDER_ORDER_2 	= 23,
	LINE_BORDER_ORDER_3 	= 24,
	LINE_BORDER_ORDER_4 	= 25,
	LINE_BORDER_ORDER_5 	= 26,
	LINE_BORDER_ORDER_6 	= 27,
	LINE_BORDER_ORDER_7 	= 28,
	LINE_BORDER_ORDER_8 	= 29,
	LINE_BORDER_ORDER_9 	= 30,
	LINE_BORDER_NON_ADMIN 	= 31,
	LINE_BORDER_PHONE 	= 32,
	LINE_BORDER_POSTAL 	= 33,
	LINE_BORDER_POLICE 	= 34,
	LINE_BORDER_SCHOOL 	= 35,
	LINE_TIME_ZONE 	= 36,
	LINE_CONTOUR 	= 37,
	LINE_BORDER_DISPUTED 	= 38,
	LINE_BORDER_DISPUTED_ORDER_0 	= 39,
	LINE_BORDER_DISPUTED_ORDER_1 	= 40,
	LINE_BORDER_TREATY_LINE 	= 41,
	LINE_BORDER_TREATY_LINE_ORDER_0 	= 42,
	LINE_BORDER_TREATY_LINE_ORDER_1 	= 43,
	LINE_SEA_BORDER_ORDER_0 	= 44,
	LINE_SEA_BORDER_ORDER_1 	= 45,
	LINE_BORDER_SPECIAL_ADMIN_REGION 	= 46
};

enum BmdAreaFeatureClass
{
	BMD_AREA 	= 0,
	AREA_BUILDING 	= 1,
	AREA_PRIVATE_HOME_RESIDENTIAL 	= 2,
	AREA_COMMERCIAL_BUILDING 	= 3,
	AREA_CONVENTION_EXHIBITION_CENTRE 	= 4,
	AREA_RETAIL_BUILDING 	= 5,
	AREA_SHOPPING_CENTRE 	= 6,
	AREA_PETROL_STATION 	= 7,
	AREA_RESTAURANT 	= 8,
	AREA_HOTEL_OR_MOTEL 	= 9,
	AREA_DAM 	= 10,
	AREA_LIGHT_HOUSE 	= 11,
	AREA_NO_WALLS 	= 12,
	AREA_MULTI_STOREY 	= 13,
	AREA_OTHERS_FACILITY 	= 14,
	AREA_TOWER 	= 15,
	AREA_INDUSTRIAL_BUILDING 	= 16,
	AREA_TRANSPORTATION_BUILDING 	= 17,
	AREA_PUBLIC_BUILDING 	= 18,
	AREA_POLICE_OFFICE 	= 19,
	AREA_FIRE_DEPARTMENT 	= 20,
	AREA_POST_OFFICE 	= 21,
	AREA_THEATER 	= 22,
	AREA_MUSEUM 	= 23,
	AREA_LIBRARY 	= 24,
	AREA_INSTITUTION 	= 25,
	AREA_RELIGIOUS_BUILDING 	= 26,
	AREA_CHURCH 	= 27,
	AREA_SYNAGOGUE 	= 28,
	AREA_MOSQUE 	= 29,
	AREA_TEMPLE 	= 30,
	AREA_SHRINE 	= 31,
	AREA_ABBEY 	= 32,
	AREA_MONASTERY 	= 33,
	AREA_BUILDING_GROUNDS 	= 34,
	PAVED_AREA 	= 35,
	SAND_AREA 	= 36,
	AREA_GOUVERNMENT_OFFICE 	= 37,
	AREA_GOVERNMENT_BUILDING 	= 38,
	AREA_CITY_HALL 	= 39,
	AREA_COURTHOUSE 	= 40,
	AREA_PRISON 	= 41,
	AREA_SUBWAY_PLATFORM 	= 42,
	AREA_SUBWAY_STATION 	= 43,
	AREA_SCHEMATIC_BUILDING 	= 44,
	AREA_URBAN_FABRIC_BRIDGE 	= 45,
	AREA_URBAN_FABRIC_CARRIAGEWAY_DIVIDER 	= 46,
	AREA_URBAN_FABRIC_RAILWAY_BRIDGE 	= 47,
	AREA_URBAN_FABRIC_RAILWAY_CROSSING 	= 48,
	AREA_URBAN_FABRIC_TUNNEL 	= 49,
	AREA_URBAN_FABRIC_WALKWAY 	= 50,
	AREA_ARTIFICIAL 	= 51,
	AREA_URBAN_FABRIC 	= 52,
	AREA_CONTINUOUS_URBAN_FABRIC 	= 53,
	AREA_DISCONTINUOUS_URBAN_FABRIC 	= 54,
	AREA_INDUSTRIAL_COMMERCIAL_TRANSPORT 	= 55,
	AREA_INDUSTRIAL_COMMERCIAL_UNITS 	= 56,
	AREA_MILITARY_BASE 	= 57,
	AREA_TRAFFIC 	= 58,
	AREA_TRAFFIC_PEDESTRIAN 	= 59,
	AREA_TRAFFIC_ROAD 	= 60,
	AREA_TRAFFIC_RAILWAY 	= 61,
	AREA_TRAFFIC_PARKING 	= 62,
	AREA_TRAFFIC_PARKING_GARAGE 	= 63,
	AREA_TRAFFIC_PARKING_LOT 	= 64,
	AREA_CAR_RACETRACK 	= 65,
	AREA_UNDERPASS 	= 66,
	AREA_CENTER 	= 67,
	AREA_CITY_MAP_COVERAGE 	= 68,
	AREA_PERIPHERY 	= 69,
	AREA_POSTAL_DISTRICT 	= 70,
	AREA_TUNNEL_ICON 	= 71,
	AREA_GARDEN_PATH 	= 72,
	AREA_PORT_FACILITIES 	= 73,
	AREA_AIRPORT 	= 74,
	AREA_AIRPORT_RUNWAY 	= 75,
	AREA_MINE_DUMP 	= 76,
	AREA_MINERAL_EXTRACTION_SITE 	= 77,
	AREA_DUMP 	= 78,
	AREA_CONSTRUCTION 	= 79,
	AREA_VEGETATED 	= 80,
	AREA_GREEN_URBAN 	= 81,
	AREA_PARK 	= 82,
	AREA_CEMETERY 	= 83,
	AREA_SPORT_LEISURE 	= 84,
	AREA_SPORTS_COMPLEX 	= 85,
	AREA_AMUSEMENT_PARK 	= 86,
	AREA_ARTS_CENTER 	= 87,
	AREA_CAMPING_SITE 	= 88,
	AREA_GOLF_COURSE 	= 89,
	AREA_HIPPODROME 	= 90,
	AREA_HOLIDAY_AREA 	= 91,
	AREA_RECREATIONAL_AREA 	= 92,
	AREA_REST_AREA 	= 93,
	AREA_SPORTS_HALL 	= 94,
	AREA_STADIUM 	= 95,
	AREA_WALKING_TERRAIN 	= 96,
	AREA_ZOO 	= 97,
	AREA_SKI 	= 98,
	AREA_SWIMMING_POOL 	= 99,
	AREA_RAILWAY_STATION 	= 100,
	AREA_FERRY_TERMINAL 	= 101,
	AREA_BUS_STATION 	= 102,
	AREA_MARINA 	= 103,
	AREA_AGRICULTURAL 	= 104,
	AREA_ARABLE 	= 105,
	AREA_NON_IRRIGATED 	= 106,
	AREA_PERMANENTLY_IRRIGATED 	= 107,
	AREA_RICE_FIELD 	= 108,
	AREA_PERMANENT_CROP 	= 109,
	AREA_TREE_BERRY_PLANTATION 	= 110,
	AREA_VINEYARD 	= 111,
	AREA_OLIVE_GROVES 	= 112,
	AREA_HETEROGENEOUS 	= 113,
	AREA_COMPLEX_CULTIVATION 	= 114,
	AREA_ANNUAL_PERMANENT 	= 115,
	AREA_AGRO_FORRESTRY 	= 116,
	AREA_AGRICULTURAL_AND_NATURAL 	= 117,
	AREA_PASTURE_RANGELAND 	= 118,
	AREA_PASTURE 	= 119,
	AREA_RANGELAND 	= 120,
	AREA_FOREST_SEMI_NATURAL 	= 121,
	AREA_FOREST 	= 122,
	AREA_LEAVED_FOREST 	= 123,
	AREA_CONIFEROUS 	= 124,
	AREA_MIXED_FORREST 	= 125,
	AREA_SCRUB 	= 126,
	AREA_GRASSLAND 	= 127,
	AREA_MOOR_HEATH 	= 128,
	AREA_SCLEROPHYLLOUS 	= 129,
	AREA_WOODLAND 	= 130,
	AREA_OPEN_SPACE 	= 131,
	AREA_ROCK 	= 132,
	AREA_GLACIER 	= 133,
	AREA_BURNT 	= 134,
	AREA_SPARSELY_VEGETATED 	= 135,
	AREA_BEACH_DUNE 	= 136,
	AREA_WETLAND 	= 137,
	AREA_INLAND_WETLAND 	= 138,
	AREA_INLAND_MARSH 	= 139,
	AREA_PEATBOG 	= 140,
	AREA_FORESTED_WETLAND 	= 141,
	AREA_COASTAL_WETLAND 	= 142,
	AREA_SALT_MARSH 	= 143,
	AREA_SALINE 	= 144,
	AREA_INTERTIDAL_FLAT 	= 145,
	AREA_ISLAND 	= 146,
	AREA_WATER 	= 147,
	AREA_INLAND_WATER 	= 148,
	AREA_RIVER 	= 149,
	AREA_CANAL 	= 150,
	AREA_LAKE 	= 151,
	AREA_RESERVOIR 	= 152,
	AREA_BASIN 	= 153,
	AREA_MARINE_WATER 	= 154,
	AREA_SEA_OCEAN 	= 155,
	AREA_COASTAL_LAGOON 	= 156,
	AREA_ESTUARY 	= 157,
	AREA_ADMIN 	= 158,
	AREA_ORDER_0 	= 159,
	AREA_ORDER_1 	= 160,
	AREA_ORDER_2 	= 161,
	AREA_ORDER_3 	= 162,
	AREA_ORDER_4 	= 163,
	AREA_ORDER_5 	= 164,
	AREA_ORDER_6 	= 165,
	AREA_ORDER_7 	= 166,
	AREA_ORDER_8 	= 167,
	AREA_ORDER_9 	= 168,
	AREA_ADMIN_AUTONOMOUS 	= 169,
	AREA_NATIVE_TRIBE_RESERVATION 	= 170,
	AREA_CONTOUR 	= 171,
	AREA_NATIONAL_PARK 	= 172,
	AREA_ENVIRONMENTAL_ZONE 	= 173,
	AREA_TOLL_ZONE 	= 174,
	AREA_EDUCATION_BUILDING 	= 175,
	AREA_SCHOOL 	= 176,
	AREA_UNIVERSITY_OR_COLLEGE 	= 177,
	AREA_MEDICAL_BUILDING 	= 178,
	AREA_HOSPITAL 	= 179,
	AREA_EMERGENCY_SERVICE 	= 180,
	AREA_CULTURE_BUILDING 	= 181,
	AREA_HISTORICAL_BUILDING 	= 182,
	AREA_TOURIST_BUILDING 	= 183,
	AREA_CASTLE 	= 184,
	AREA_FORTRESS 	= 185,
	AREA_MONUMENT 	= 186,
	AREA_VIEW 	= 187,
	AREA_WATERMILL 	= 188,
	AREA_WINDMILL 	= 189,
	AREA_MEMORIAL_GROUND 	= 190,
	AREA_LEISURE_BUILDING 	= 191,
	AREA_SPORT_BUILDING 	= 192
};

enum AttributeSource
{
	INDEX_8 	= 0x1,
	INDEX_16 	= 0x2,
	EXPLICIT 	= 0x3
};

enum	Direction_1
{
	IN_NO_DIRECTION 	= 0,
	IN_POSITIVE_DIRECTION 	= 1,
	IN_NEGATIVE_DIRECTION 	= 2,
	IN_BOTH_DIRECTIONS 	= 3
};

enum AdminRoadClass
{
	UNKNOWN_ADMIN_ROAD_CLASS 	= 0,
	MOST_IMPORTANT_ADMIN_ROAD_CLASS 	= 1,
	SECOND_IMPORTANT_ADMIN_ROAD_CLASS 	= 2,
	THIRD_IMPORTANT_ADMIN_ROAD_CLASS 	= 3,
	FOURTH_IMPORTANT_ADMIN_ROAD_CLASS 	= 4,
	FIFTH_IMPORTANT_ADMIN_ROAD_CLASS 	= 5,
	SIXTH_IMPORTANT_ADMIN_ROAD_CLASS 	= 6,
	LEAST_IMPORTANT_ADMIN_ROAD_CLASS 	= 7
};

enum LinkType
{
	NO_SPECIAL 	= 0x0,
	RAMP 	= 0x1,
	ROUNDABOUT 	= 0x2,
	PARALLEL 	= 0x3,
	SERVICE_ROAD 	= 0x4,
	MAIN_ROAD 	= 0x5,
	SQUARE 	= 0x6,
	SERVICE_ACCESS 	= 0x7,
	PEDESTRIAN_ZONE 	= 0x8,
	ROUNDABOUT_INTERIOR 	= 0xA,
	SLIP_ROAD 	= 0xB
};

#endif	//	DPNDSTYPEDEF_H
