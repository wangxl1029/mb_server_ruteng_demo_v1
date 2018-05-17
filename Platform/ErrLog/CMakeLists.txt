cmake_minimum_required(VERSION 2.8)
project(ErrLog)

SET(VERSION "2.0.0")
SET(MSGHEADER "ErrLog")

IF (NOT COMMOMLIB_DIR)
	MESSAGE(FATAL_ERROR "${MSGHEADER}: the common function library directory not defined!")
ENDIF()

IF (NOT PLATFORM_DIR)
	MESSAGE(FATAL_ERROR "${MSGHEADER}: the platform directory not defined!")
ENDIF()

# Source code files
file(GLOB SRC "src/*.cpp")

#including directory
include_directories(
	src
	include
	${PLATFORM_DIR}/OSWrapper/include
	${COMMOMLIB_DIR}/CommonFunc/include
)

# Macro definations
add_definitions(
	-DWXL_COMMENT_OUT
	-DWXL_SERVER_SPECIFIED
#	-DRS_VERSION="${VERSION}"
)

add_library(ErrLog ${SRC})