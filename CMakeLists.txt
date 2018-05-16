cmake_minimum_required(VERSION 2.8)
project(rpmain)

SET(VERSION "2.0.0")

IF (NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE "Release")
ENDIF()

MESSAGE (STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

SET (BASE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/dependency)
SET (EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib")
SET (DEBUGLIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Debug)

IF (${CMAKE_GENERATOR} STREQUAL "Visual Studio 12 Win64" OR ${CMAKE_GENERATOR} STREQUAL "Visual Studio 12 2013 Win64")
	MESSAGE (STATUS "Windows")


ELSEIF (${CMAKE_GENERATOR} STREQUAL "Unix Makefiles")
	MESSAGE (STATUS "Linux")

ENDIF ()

MESSAGE (STATUS ${VERSION})

file(GLOB rpmainSRC "rpmain/*.cpp")

link_directories(${DEBUGLIB_PATH})

add_executable(	rpmain 
		${rpmainSRC}
		)


IF (${MAPBAR_PLATFORM} STREQUAL "linux")
ELSE()

	target_link_libraries(	rpmain
				IOLibCommon.lib
				IOLibNDS.lib
				IOLibUPD.lib
				nds_sqlite3.lib
				SL_DAL.lib
				zlib.lib
				)
ENDIF ()