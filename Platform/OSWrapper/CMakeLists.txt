cmake_minimum_required(VERSION 2.8)
project(OSWrapper)

SET(VERSION "2.0.0")
SET(MSGHEADER "OSWrapper")

IF (NOT COMMOMLIB_DIR)
	MESSAGE(FATAL_ERROR "${MSGHEADER}: the common function library directory not defined!")
ENDIF()

# Source code files
file(GLOB SRC "src/*.cpp")

#including directory
include_directories(
	src
	include
	${COMMOMLIB_DIR}/CommonFunc/include
)

# Macro definations
add_definitions(
	-DWXL_COMMENT_OUT
	-DWXL_SERVER_SPECIFIED
#	-DRS_VERSION="${VERSION}"
)

add_library(OSWrapper ${SRC})