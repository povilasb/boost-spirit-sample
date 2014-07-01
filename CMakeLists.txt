cmake_minimum_required(VERSION 2.6)

# Configurable.

if (NOT DEFINED PROJECT_NAME)
	set(PROJECT_NAME "skeleton")
endif(NOT DEFINED PROJECT_NAME)

option(SKELETON_ENABLE_TESTS "Enables or disables tests. Disabled by default."
	OFF)


project(${PROJECT_NAME} CXX)
set(PROJECT_EXEC "${PROJECT_NAME}")


message(STATUS "Build type: ${CMAKE_BUILD_TYPE}.")

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fstack-protector -s")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Werror -pedantic")

set(LIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")


# Find Boost libs.
set(Boost_NO_SYSTEM_PATHS ON)
set(BOOST_INCLUDEDIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/boost")
set(BOOST_LIBRARYDIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/boost/libs")
find_package(Boost 1.56.0)
include_directories("${Boost_INCLUDE_DIRS}")
message(STATUS "Boost include dir: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost libraries: ${Boost_LIBRARIES}")


set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cpp")

add_executable("${PROJECT_EXEC}" "${SRC_FILES}")
