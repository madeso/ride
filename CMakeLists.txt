project(ride)
cmake_minimum_required(VERSION 3.1.0)
set_property(GLOBAL PROPERTY USE_FOLDERS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cpack-config.cmake)

option(BUILD_USE_CLANG_FORMAT "Add clang format to the build" ON)
option(BUILD_USE_CPPLINT "Add cpplint to the build" OFF)
option(BUILD_USE_DOXYGEN "Add doxygen to the build" OFF)
option(BUILD_USE_CLANG_TIDY "Add clang-tidy to the build" OFF)
option(BUILD_USE_DOXYGEN_GLOBAL "Add doxygen as a seperate build" OFF)
option(BUILD_USE_CPPCHECK "run cppcheck on all source" OFF)
option(BUILD_USE_INCLUDECHECK "run custom script to include order problems" OFF)
option(BUILD_USE_ALL_COMPILER_WARNINGS "enable all compiler warnings" OFF)
option(BUILD_USE_CLOC "Update code stat files" OFF)
option(BUILD_USE_SIMIAN "Create simian project" OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(${APPLE})
	set(RIDE_OS_APPLE "1")
	
	if(NOT ${BUILD_USE_ALL_COMPILER_WARNINGS})
		# disabling http://stackoverflow.com/questions/32626171/xcode-7-how-to-suppress-warning-overrides-a-member-function-but-is-not-marked
		# that apple-clang spews out for wx headers
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-inconsistent-missing-override")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
	endif()
endif()

if(${MSVC})
	# C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include\xutility(2132): warning C4996: 'std::_Copy_impl': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS.
	add_definitions("-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS")
	message(STATUS "Adding _SCL_SECURE_NO_WARNINGS and _CRT_SECURE_NO_WARNINGS")
	
	if(${BUILD_USE_ALL_COMPILER_WARNINGS})
		# Force to always compile with W4
		if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
			string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
		else()
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
		endif()
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

message(STATUS "Cmake cxx flags ${CMAKE_CXX_FLAGS}")

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/external/cpptoml/include)
add_subdirectory(ride)

IF(BUILD_USE_CLOC)
	add_custom_target(codestat-everything ALL
					   run-cloc.py ride CMakeLists.txt --out code-stat-everything.txt
					   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
					   COMMENT "Running cloc on everything" VERBATIM)
					   
	add_custom_target(codestat-everything-switcher ALL
					   run-cloc.py --out code-stat-switcher.txt ride/switcher
					   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
					   COMMENT "Running cloc on switcher" VERBATIM)

	# add_custom_target(codestat-switcher-only ALL
	# 				   cloc --by-file ride/switcher/* --quiet --out code-stat-switcher.txt
	# 				   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	# 				   COMMENT "Running cloc on switcher only" VERBATIM)
ENDIF(BUILD_USE_CLOC)

if(BUILD_USE_CLANG_TIDY)
	add_custom_target(run-clang-tidy ALL
				   ./call-clang-tidy.py -build ${CMAKE_CURRENT_BINARY_DIR} ride/*.cc ride/*.h ride/switcher/*.cc ride/switcher/*.h
				   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
				   COMMENT "Running clang-tidy" VERBATIM)
endif(BUILD_USE_CLANG_TIDY)

IF(BUILD_USE_SIMIAN)
	# http://www.harukizaemon.com/simian/
	add_custom_target(simian-everything ALL
					   simian-2.4.0.exe -formatter=vs -language=cpp -threshold=4 ${CMAKE_SOURCE_DIR}/ride/*.cc
					   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
					   COMMENT "Running simian" VERBATIM)
ENDIF(BUILD_USE_SIMIAN)
