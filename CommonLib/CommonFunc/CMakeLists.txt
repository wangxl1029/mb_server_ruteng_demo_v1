cmake_minimum_required(VERSION 2.8)
project(CommonFunc)

SET(VERSION "2.0.0")
SET(MSGHEADER "CommonFunc")

IF (NOT GENERATED_DIR)
	MESSAGE(FATAL_ERROR "${MSGHEADER}: the generated directory not defined!")
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
	${PLATFORM_DIR}/ErrLog/include
	${PLATFORM_DIR}/OSWrapper/include
	${GENERATED_DIR}/IOLibs/include/IOLibCommon
)

# Macro definations
add_definitions(
	-DWXL_COMMENT_OUT
	-DWXL_SERVER_SPECIFIED
#	-DRS_VERSION="${VERSION}"
)

add_library(CommFunc ${SRC})