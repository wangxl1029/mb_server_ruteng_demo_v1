cmake_minimum_required(VERSION 2.8)
project(DataProvider)

SET(VERSION "2.0.0")
SET(MSGHEADER "DataProvider")

IF (NOT PLATFORM_DIR)
	MESSAGE(FATAL_ERROR "${MSGHEADER}: the platform directory not defined!")
ENDIF()

IF (NOT EXTERNLIB_DIR)
	MESSAGE(FATAL_ERROR "${MSGHEADER}: the external library directory not defined!")
ENDIF()

IF (NOT GENERATED_DIR)
	MESSAGE(FATAL_ERROR "${MSGHEADER}: the generated directory not defined!")
ENDIF()

IF (NOT COMMOMLIB_DIR)
	MESSAGE(FATAL_ERROR "${MSGHEADER}: the common library directory not defined!")
ENDIF()


#including directory
include_directories(
	src
	include
	${PLATFORM_DIR}/ErrLog/include
	${PLATFORM_DIR}/OSWrapper/include
	${EXTERNLIB_DIR}/nds_sqlite3/include
#	${EXTERNLIB_DIR}/zlib-1.2.8
	${GENERATED_DIR}/IOLibs/include/IOLibCommon
	${GENERATED_DIR}/IOLibs/include/NDS
#	${GENERATED_DIR}/IOLibs/include/UPD
	${GENERATED_DIR}/SL_DAL/include
	${COMMOMLIB_DIR}/CommonFunc/include
	${COMMOMLIB_DIR}/DataProvider/include
)

# Source code files
file(GLOB SRC "src/*.cpp" "src/*/*.cpp")

# Macro definations
add_definitions(
	-DWXL_COMMENT_OUT
	-DWXL_SERVER_SPECIFIED
#	-DRS_VERSION="${VERSION}"
)

add_library(DataProvider ${SRC})