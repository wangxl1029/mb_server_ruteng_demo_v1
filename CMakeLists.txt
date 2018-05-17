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

# Add library directory to build
ADD_SUBDIRECTORY(${FUNCTION_DIR}/RP)
ADD_SUBDIRECTORY(${PLATFORM_DIR}/ErrLog)
ADD_SUBDIRECTORY(${PLATFORM_DIR}/OSWrapper)
ADD_SUBDIRECTORY(${COMMOMLIB_DIR}/CommonFunc)
ADD_SUBDIRECTORY(${COMMOMLIB_DIR}/DataProvider)

# Including header directory
include_directories(
	${FUNCTION_DIR}
	${COMMOMLIB_DIR}/CommonFunc/include
	${COMMOMLIB_DIR}/DataProvider/include
)

file(GLOB rpmainSRC "rpmain/*.cpp")

link_directories(${DEBUGLIB_PATH})

add_executable(	rpmain 
		${rpmainSRC}
		)


#IF (${MAPBAR_PLATFORM} STREQUAL "linux")

#ELSE()

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
#ENDIF ()