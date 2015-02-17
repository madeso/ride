project(ride)
cmake_minimum_required(VERSION 2.8)
set_property(GLOBAL PROPERTY USE_FOLDERS OFF)

option(BUILD_USE_CLANG_FORMAT "Add clang format to the build" ON)
option(BUILD_USE_CPPLINT "Add cpplint to the build" ON)
option(BUILD_USE_DOXYGEN "Add doxygen to the build" ON)
option(BUILD_USE_CPPCHECK "run cppcheck on all source" ON)

find_package(Doxygen REQUIRED)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.ini ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(doxygen ALL
	${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	COMMENT "Running Doxygen" VERBATIM)

include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(ride)
