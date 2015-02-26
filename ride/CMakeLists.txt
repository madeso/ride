if(${MSVC})
	# C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include\xutility(2132): warning C4996: 'std::_Copy_impl': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS.
	add_definitions("-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS")
	message(STATUS "Adding _SCL_SECURE_NO_WARNINGS and _CRT_SECURE_NO_WARNINGS")
endif()

find_package(wxWidgets COMPONENTS core aui base stc adv REQUIRED)
include( "${wxWidgets_USE_FILE}" )
message(status "wxWidgets use file: ${wxWidgets_USE_FILE}")

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

FILE(GLOB src_glob *.cc;*.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
PROTOBUF_GENERATE_CPP(SETTINGS_PROTO_SRCS SETTINGS_PROTO_HDRS settings.proto)

set(src settings.proto resources/resources.rc ${src_glob} ${SETTINGS_PROTO_SRCS} ${SETTINGS_PROTO_HDRS})
set(generated_src generated/ui.cpp generated/ui.h)

source_group("" FILES ${src})
source_group("generated" FILES ${generated_src} ${SETTINGS_PROTO_SRCS} ${SETTINGS_PROTO_HDRS})

add_executable(
	${PROJECT_NAME} WIN32 
	${src} ${generated_src}
)

target_link_libraries(
	${PROJECT_NAME}
	${wxWidgets_LIBRARIES}
	${PROTOBUF_LIBRARIES}
)

if(BUILD_USE_CLANG_FORMAT)
	add_custom_command(TARGET ${PROJECT_NAME}
				   PRE_BUILD
				   COMMAND ./call-clang-format.py ${CMAKE_CURRENT_SOURCE_DIR}/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/*.h
				   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
				   COMMENT "Running clang format" VERBATIM)
endif(BUILD_USE_CLANG_FORMAT)

if(BUILD_USE_CPPLINT)
	add_custom_command(TARGET ${PROJECT_NAME}
					   PRE_BUILD
					   COMMAND ./cpplint.py ARGS --output=vs7 ${CMAKE_CURRENT_SOURCE_DIR}/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/*.h  ${mainsrc}
					   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
					   COMMENT "Running CppLint" VERBATIM)
endif(BUILD_USE_CPPLINT)

if(BUILD_USE_CPPCHECK)
	add_custom_command(TARGET ${PROJECT_NAME}
					   PRE_BUILD
					   COMMAND cppcheck --enable=all --suppress=unusedFunction --quiet --template=vs ${CMAKE_SOURCE_DIR}/ride
					   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
					   COMMENT "Running cppcheck" VERBATIM)
endif(BUILD_USE_CPPCHECK)

if(BUILD_USE_DOXYGEN)
	add_custom_command(TARGET ${PROJECT_NAME}
					   PRE_LINK
					   COMMAND ${DOXYGEN_EXECUTABLE} ARGS ${CMAKE_CURRENT_BINARY_DIR}/../Doxyfile
					   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
					   COMMENT "Running Doxygen" VERBATIM)
endif(BUILD_USE_DOXYGEN)
