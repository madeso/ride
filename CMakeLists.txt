cmake_minimum_required(VERSION 3.15)
set_property(GLOBAL PROPERTY USE_FOLDERS OFF)
project(ride C CXX)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(RIDE_VERSION_MAJOR "0")
set(RIDE_VERSION_MINOR "0")
set(RIDE_VERSION_REVISION "1")

include(install/cpack-config.cmake)

include(cmake/StandardProjectSettings.cmake)
include(cmake/CompilerWarnings.cmake)
include(cmake/Sanitizers.cmake)

add_library(project_options INTERFACE)
add_library(project_warnings INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)
set_project_warnings(project_warnings)
enable_sanitizers(project_options)

# https://cmake.org/cmake/help/latest/prop_tgt/MSVC_RUNTIME_LIBRARY.html
function(set_crt_to_static project_name)
    set_property(TARGET ${project_name} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endfunction()

SET(RIDE_BUILD_COMMIT "unknown-commit" CACHE STRING "The current commit")
SET(RIDE_BUILD_NUMBER "unknown-build"  CACHE STRING "The current build number")
SET(RIDE_BUILD_BRANCH "unknown-branch" CACHE STRING "The current branch")
SET(RIDE_BUILD_REPO   "unknown-repo"   CACHE STRING "The repo")

# message(STATUS "cmake osx status: ${CMAKE_OSX_DEPLOYMENT_TARGET}")
# message(STATUS "commit: ${RIDE_BUILD_COMMIT}")
# message(STATUS "build number: ${RIDE_BUILD_NUMBER}")
# message(STATUS "branch: ${RIDE_BUILD_BRANCH}")
# message(STATUS "repo: ${RIDE_BUILD_REPO}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(external)
add_subdirectory(ride)
add_subdirectory(api)
add_subdirectory(runner)
