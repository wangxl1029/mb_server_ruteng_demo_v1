cmake_minimum_required(VERSION 2.8)
project(RP)

SET(VERSION "2.0.0")

MESSAGE (STATUS "RP: CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

# Guard the condition
IF (NOT PLATFORM_DIR)
	MESSAGE(FATAL_ERROR "RP: the platform directory not defined!")
#ELSE()
#	MESSAGE(STATUS "RP: the platform directory is " ${PLATFORM_DIR})
ENDIF()

IF (NOT EXTERNLIB_DIR)
	MESSAGE(FATAL_ERROR "RP: the external library directory not defined!")
#ELSE()
#	MESSAGE(STATUS "RP: the external library directory is " ${EXTERNLIB_DIR})
ENDIF()


IF (NOT GENERATED_DIR)
	MESSAGE(FATAL_ERROR "RP: the generated directory not defined!")
#ELSE()
#	MESSAGE(STATUS "RP: the generated directory is " ${GENERATED_DIR})
ENDIF()

IF (NOT COMMOMLIB_DIR)
	MESSAGE(FATAL_ERROR "RP: the common function library directory not defined!")
#ELSE()
#	MESSAGE(STATUS "RP: the common function library directory is " ${COMMOMLIB_DIR})
ENDIF()

IF (NOT FUNCTION_DIR)
	MESSAGE(FATAL_ERROR "RP: the function directory not defined!")
#ELSE()
#	MESSAGE(STATUS "RP: the function directory is " ${FUNCTION_DIR})
ENDIF()

# Source code files
file(GLOB SRC "src/*.cpp" "src/*/*.cpp")

#including directory
include_directories(
	src
	include
	src/Route
	src/RouteCalculator
	${FUNCTION_DIR}
	${PLATFORM_DIR}/ErrLog/include
	${PLATFORM_DIR}/OSWrapper/include
	${EXTERNLIB_DIR}/nds_sqlite3/include
	${EXTERNLIB_DIR}/zlib-1.2.8
	${GENERATED_DIR}/IOLibs/include/IOLibCommon
	${GENERATED_DIR}/IOLibs/include/NDS
	${GENERATED_DIR}/IOLibs/include/UPD
	${GENERATED_DIR}/SL_DAL/include
	${COMMOMLIB_DIR}/CommonFunc/include
	${COMMOMLIB_DIR}/DataProvider/include
)

# Macro definations
add_definitions(
	-DWXL_COMMENT_OUT
	-DWXL_SERVER_SPECIFIED
#	-DRS_VERSION="${VERSION}"
)

add_library(rp ${SRC})























































