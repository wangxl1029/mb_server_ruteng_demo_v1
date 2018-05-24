cmake_minimum_required(VERSION 2.8)
project(rpmain)

SET(VERSION "2.0.0")

IF (NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE "Release")
ENDIF()

MESSAGE (STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

# Solution directory defination
SET (SOLUTION_DIR	${CMAKE_CURRENT_SOURCE_DIR})
SET (PLATFORM_DIR	${SOLUTION_DIR}/Platform)
SET (FUNCTION_DIR	${SOLUTION_DIR}/Function)
SET (EXTERNLIB_DIR	${SOLUTION_DIR}/ExternalLibrary)
SET (GENERATED_DIR	${SOLUTION_DIR}/Generated)
SET (COMMOMLIB_DIR	${SOLUTION_DIR}/CommonLib)
SET (LIBRARY_OUTPUT_PATH	"${SOLUTION_DIR}/lib")  
SET (EXECUTABLE_OUTPUT_PATH	"${SOLUTION_DIR}/bin")
SET (DEBUGLIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Debug)

MESSAGE(STATUS "the solution directory is ${SOLUTION_DIR}")

IF (${CMAKE_GENERATOR} STREQUAL "Visual Studio 12 Win64" OR ${CMAKE_GENERATOR} STREQUAL "Visual Studio 12 2013 Win64")
	MESSAGE (STATUS "Windows")


ELSEIF (${CMAKE_GENERATOR} STREQUAL "Unix Makefiles")
	MESSAGE (STATUS "Linux")

ENDIF ()

MESSAGE (STATUS ${VERSION})

# ---[ Build flags
set(CMAKE_C_STANDARD 99)
#set(CMAKE_CXX_STANDARD 11)

IF (NOT MSVC)
	SET (CMAKE_C_FLAGS_DEBUG "-Wall -g -fshort-wchar -fwide-exec-charset=UTF-16LE -m64 -Wno-deprecated")
	SET (CMAKE_C_FLAGS_RELEASE "-Wall -O2 -fshort-wchar -fwide-exec-charset=UTF-16LE -m64 -Wno-deprecated")

	SET (CMAKE_CXX_FLAGS_DEBUG "-Wall -g -std=c++11 -fshort-wchar -fwide-exec-charset=UTF-16LE -m64 -Wno-deprecated")
	SET (CMAKE_CXX_FLAGS_RELEASE "-Wall -O2 -std=c++11 -fshort-wchar -fwide-exec-charset=UTF-16LE -m64 -Wno-deprecated")
ENDIF()

# Including header directory
include_directories(
	${FUNCTION_DIR}
	${COMMOMLIB_DIR}/CommonFunc/include
	${COMMOMLIB_DIR}/DataProvider/include
)

file(GLOB rpmainSRC "rpmain/*.cpp")

IF (MSVC)
link_directories(${SOLUTION_DIR}/Debug)
link_directories(${LIBRARY_OUTPUT_PATH}/Debug)
ELSE()
link_directories(${SOLUTION_DIR}/Debug)
link_directories(${LIBRARY_OUTPUT_PATH})
ENDIF()

# Add library directory to build
ADD_SUBDIRECTORY(${FUNCTION_DIR}/RP)
ADD_SUBDIRECTORY(${PLATFORM_DIR}/ErrLog)
ADD_SUBDIRECTORY(${PLATFORM_DIR}/OSWrapper)
ADD_SUBDIRECTORY(${COMMOMLIB_DIR}/CommonFunc)
ADD_SUBDIRECTORY(${COMMOMLIB_DIR}/DataProvider)

add_executable(	rpmain ${rpmainSRC} )

IF (NOT MSVC)
	target_link_libraries(	rpmain
				libRP.a
				libCommFunc.a
				libDataProvider.a
				libErrLog.a
				libOSWrapper.a
				libio_lib_common.a
				libio_lib_upd.a
				libio_lib_nds.a
				libsl_dal.a
				libnds_sqlite3.a
				libzlib_linux_x64.a
				libzstd_linux_x64.a
				)

IF (0)
	add_dependencies(	rpmain
				RP
				ErrLog
				OSWrapper
				CommFunc
				DataProvider
				)
ENDIF ()
ELSE()
	target_link_libraries(	rpmain
				DataProvider.lib
				nds_sqlite3.lib
				SL_DAL.lib
				IOLibCommon.lib
				IOLibNDS.lib
				IOLibUPD.lib
				zlib.lib
				ErrLog.lib
				OSWrapper.lib
				CommFunc.lib
				RP.lib
				)
ENDIF ()
