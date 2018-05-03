#pragma once
//typedef		long					RESULT;


#define	RP_ROUTE_SEARCH_TYPE_NONE								(0x0)
#define	RP_ROUTE_SEARCH_TYPE_RECOMMEND							(0x1)
#define	RP_ROUTE_SEARCH_TYPE_GENERAL							(0x2)
#define	RP_ROUTE_SEARCH_TYPE_DIST								(0x4)
#define	RP_ROUTE_SEARCH_TYPE_WIDTH								(0x8)
#define	RP_ROUTE_SEARCH_TYPE_OTHER								(0x10)
#define	RP_ROUTE_SEARCH_TYPE_ALL								(0x1F)

#include <stdint.h>
#include <vector>
#include <memory>