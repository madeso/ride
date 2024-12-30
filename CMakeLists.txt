cmake_minimum_required(VERSION 3.10.0)

project(ride)
set_property(GLOBAL PROPERTY USE_FOLDERS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(RIDE_VERSION_MAJOR "0")
set(RIDE_VERSION_MINOR "0")
set(RIDE_VERSION_REVISION "0")

option(RIDE_USE_WEBVIEW "Use the webview of wxWidgets" OFF)

SET(RIDE_BUILD_COMMIT "unknown-commit" CACHE STRING "The current commit")
SET(RIDE_BUILD_NUMBER "unknown-build"  CACHE STRING "The current build number")
SET(RIDE_BUILD_BRANCH "unknown-branch" CACHE STRING "The current branch")
SET(RIDE_BUILD_REPO   "unknown-repo"   CACHE STRING "The repo")

message(STATUS "cmake osx status: ${CMAKE_OSX_DEPLOYMENT_TARGET}")
message(STATUS "commit: ${RIDE_BUILD_COMMIT}")
message(STATUS "build number: ${RIDE_BUILD_NUMBER}")
message(STATUS "branch: ${RIDE_BUILD_BRANCH}")
message(STATUS "repo: ${RIDE_BUILD_REPO}")

include(cpack-config.cmake)

if(${UNIX})
	set(RIDE_OS_UNIX "1")
endif()
if(${APPLE})
	set(RIDE_OS_APPLE "1")
	set(RIDE_OS_UNIX "0")
endif()

add_library(project_options INTERFACE)
add_library(common::options ALIAS project_options)
target_compile_features(project_options INTERFACE cxx_std_17)

add_subdirectory(external)
add_subdirectory(src)
