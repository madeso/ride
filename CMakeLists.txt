project(ride)
cmake_minimum_required(VERSION 3.1.0)
set_property(GLOBAL PROPERTY USE_FOLDERS OFF)

option(BUILD_USE_CLANG_FORMAT "Add clang format to the build" ON)
option(BUILD_USE_CPPLINT "Add cpplint to the build" ON)
option(BUILD_USE_DOXYGEN "Add doxygen to the build" ON)
option(BUILD_USE_DOXYGEN_GLOBAL "Add doxygen as a seperate build" ON)
option(BUILD_USE_CPPCHECK "run cppcheck on all source" ON)
option(BUILD_USE_INCLUDECHECK "run custom script to include order problems" ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(${APPLE})
	set(RIDE_OS_APPLE "1")
endif()

if(${MSVC})
	# C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include\xutility(2132): warning C4996: 'std::_Copy_impl': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS.
	add_definitions("-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS")
	message(STATUS "Adding _SCL_SECURE_NO_WARNINGS and _CRT_SECURE_NO_WARNINGS")
	
	# Force to always compile with W4
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
	
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4100 /wd4018 /wd4389 /wd4125 /wd4127 /wd4512 /wd4244 /wd4996") # unref parameter, < signed/unsigned mismatch, == signed/unsigned mismatch, decimal digit termnates octal(protobuf warning), constant conditional (protobuf), assignment couldnt be generated(protobuf), possible loss of data(protobuf), deprecated (generated/ui.cpp)
endif(${MSVC})

find_package(Doxygen REQUIRED)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.ini ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
if(BUILD_USE_DOXYGEN_GLOBAL)
	add_custom_target(doxygen ALL
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMENT "Running Doxygen" VERBATIM)
endif(BUILD_USE_DOXYGEN_GLOBAL)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/external/cpptoml/include)
add_subdirectory(ride)
