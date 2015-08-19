# extra wx libs that we will probably use soon
# html propgrid xml xrc net webview

if(${MSVC})
# on windows and visual-studio it requires scintilla
find_package(wxWidgets COMPONENTS core webview aui base stc adv scintilla REQUIRED)
else()
find_package(wxWidgets COMPONENTS core webview aui base stc adv REQUIRED)
# on travis we get a linker errror
# that wx_gtk2u_scintilla-3.0 is missing, but the log says it builds
#      libwxscintilla-3.0.a   ..bug? lets hope wx-config solve the problem for us since we use stc
endif()

include( "${wxWidgets_USE_FILE}" )
message(STATUS "wxWidgets use file: ${wxWidgets_USE_FILE}")

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

FILE(GLOB src_glob *.cc;*.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
PROTOBUF_GENERATE_CPP(SETTINGS_PROTO_SRCS SETTINGS_PROTO_HDRS settings.proto)


option(USE_WXWIDGETS_LEGACY_FINDTEXT "use the old wxwidgets findtext" OFF)
option(USE_CPP_AS_RUST "fallback to use c++ lexer instead of the rust lexer" OFF)

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/ride_compiler_settings.in.h"
  "${CMAKE_CURRENT_BINARY_DIR}/ride_compiler_settings.h"
)

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

if(BUILD_USE_INCLUDECHECK)
	add_custom_command(TARGET ${PROJECT_NAME}
				   PRE_BUILD
				   COMMAND ./check_includes.py ${CMAKE_CURRENT_SOURCE_DIR}/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/*.h
				   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
				   COMMENT "Checking includes" VERBATIM)
endif(BUILD_USE_INCLUDECHECK)

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

# Amend manifest to tell Windows that the application is DPI aware (needed for Windows 8.1 and up)
IF (MSVC)
	IF (CMAKE_MAJOR_VERSION LESS 3)
		MESSAGE(WARNING "CMake version 3.0 or newer is required use build variable TARGET_FILE")
	ELSE()
		ADD_CUSTOM_COMMAND(
			TARGET ${PROJECT_NAME}
			POST_BUILD
			COMMAND "mt.exe" -manifest \"${CMAKE_CURRENT_SOURCE_DIR}\\dpiaware.manifest.xml\" -inputresource:\"$<TARGET_FILE:${PROJECT_NAME}>\"\;\#1 -outputresource:\"$<TARGET_FILE:${PROJECT_NAME}>\"\;\#1
			COMMENT "Adding display aware manifest..." 
		)
	ENDIF()
ENDIF(MSVC)
