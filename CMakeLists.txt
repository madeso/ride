set(CMAKE_USER_MAKE_RULES_OVERRIDE ${CMAKE_CURRENT_SOURCE_DIR}/c_flag_overrides.cmake)
set(CMAKE_USER_MAKE_RULES_OVERRIDE_CXX ${CMAKE_CURRENT_SOURCE_DIR}/cxx_flag_overrides.cmake)

project(ride)
cmake_minimum_required(VERSION 3.0.2)
set_property(GLOBAL PROPERTY USE_FOLDERS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(RIDE_VERSION_MAJOR "0")
set(RIDE_VERSION_MINOR "0")
set(RIDE_VERSION_REVISION "0")

SET(RIDE_BUILD_COMMIT "unknown-commit" CACHE STRING "The current commit")
SET(RIDE_BUILD_NUMBER "unknown-build"  CACHE STRING "The current build number")
SET(RIDE_BUILD_BRANCH "unknown-branch" CACHE STRING "The current branch")
SET(RIDE_BUILD_REPO   "unknown-repo"   CACHE STRING "The repo")

include(cpack-config.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

message(STATUS "cmake osx status: ${CMAKE_OSX_DEPLOYMENT_TARGET}")
message(STATUS "commit: ${RIDE_BUILD_COMMIT}")
message(STATUS "build number: ${RIDE_BUILD_NUMBER}")
message(STATUS "branch: ${RIDE_BUILD_BRANCH}")
message(STATUS "repo: ${RIDE_BUILD_REPO}")


if(${MSVC})
	# C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include\xutility(2132): warning C4996: 'std::_Copy_impl': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS.
	add_definitions("-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS")
endif(${MSVC})


add_subdirectory(ride)
add_subdirectory(runner)
