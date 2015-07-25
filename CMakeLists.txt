project(ride)
cmake_minimum_required(VERSION 2.8)
set_property(GLOBAL PROPERTY USE_FOLDERS OFF)

option(BUILD_USE_CLANG_FORMAT "Add clang format to the build" ON)
option(BUILD_USE_CPPLINT "Add cpplint to the build" ON)
option(BUILD_USE_DOXYGEN "Add doxygen to the build" ON)
option(BUILD_USE_DOXYGEN_GLOBAL "Add doxygen as a seperate build" ON)
option(BUILD_USE_CPPCHECK "run cppcheck on all source" ON)

##########################################################################################################################
# copied from cpptoml https://github.com/skystrife/cpptoml/blob/toml-v0.4.0/CMakeLists.txt

include(CheckCXXCompilerFlag)
include(CheckCXXSourceCompiles)
include(CheckCXXSourceRuns)

if(UNIX)
    option(USE_LIBCXX "Use libc++ for the C++ standard library" ON)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        if(USE_LIBCXX)
            message("-- Locating libc++...")
            find_library(LIBCXX_LIBRARY NAMES c++ cxx)
            if(LIBCXX_LIBRARY)
                message("-- Located libc++, using it.")
                set(LIBCXX_OPTIONS "-stdlib=libc++")
                message("--     Locating libc++'s abi...")
                find_library(LIBCXXABI_LIBRARY NAMES c++abi)
                find_library(LIBCXXRT_LIBRARY NAMES cxxrt)
                if(LIBCXXABI_LIBRARY)
                    message("--     Found libc++abi, using it.")
                    set(CXXABI_LIBRARY ${LIBCXXABI_LIBRARY})
                elseif(LIBCXXRT_LIBRARY)
                    message("--     Found libcxxrt, using it.")
                    set(CXXABI_LIBRARY ${LIBCXXRT_LIBRARY})
                else()
                    message("--     No abi library found. "
                            "Attempting to continue without one...")
                endif()
            else()
                message("-- Could not find libc++, will not use it.")
            endif()
        endif()
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LIBCXX_OPTIONS}")
    set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} ${CXXABI_LIBRARY}")

    # if we don't already set the standard for the compiler, detect the
    # best one available and use it
    if(NOT "${CMAKE_CXX_FLAGS}" MATCHES "std=c\\+\\+(0x|11|1y|14)")
        check_cxx_compiler_flag(-std=c++14 HAS_CXX14)
        if(HAS_CXX14)
            message("-- Compiler supports C++14 (using it)")
            set(STDOPT "-std=c++14")
        endif()

        if(NOT STDOPT)
            check_cxx_compiler_flag(-std=c++1y HAS_CXX1Y)
            if(HAS_CXX1Y)
                message("-- Compiler supports C++1y (using it)")
                set(STDOPT "-std=c++1y")
            endif()
        endif()

        if(NOT STDOPT)
            check_cxx_compiler_flag(-std=c++11 HAS_CXX11)
            if(HAS_CXX11)
                message("-- Compiler supports C++11 (using it)")
                set(STDOPT "-std=c++11")
            endif()
        endif()

        if(NOT STDOPT)
            check_cxx_compiler_flag(-std=c++0x HAS_CXX0X)
            if(HAS_CXXOX)
                message("-- Compiler supports C++0x (using it)")
                set(STDOPT "-std=c++0x")
            endif()
        endif()

        if(NOT STDOPT)
            message(FATAL_ERROR
                    "ride requires a compiler with at least C++0x support")
        endif()

        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${STDOPT}")
    endif()
endif()

# end copied from cpptoml
##########################################################################################################################

if(${MSVC})
	# C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include\xutility(2132): warning C4996: 'std::_Copy_impl': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS.
	add_definitions("-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS")
	message(STATUS "Adding _SCL_SECURE_NO_WARNINGS and _CRT_SECURE_NO_WARNINGS")
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
