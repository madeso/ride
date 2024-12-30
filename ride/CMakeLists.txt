# extra wx libs that we will probably use soon
# html propgrid xml xrc net webview


SET(RIDE_WX_COMPONENTS core aui base stc adv html)

if(${MSVC})
# on windows and visual-studio it requires scintilla
LIST(APPEND RIDE_WX_COMPONENTS scintilla)
endif()

if(${RIDE_USE_WEBVIEW})
# only try to find webview if it's required
LIST(APPEND RIDE_WX_COMPONENTS webview)
endif()

message(STATUS "wxWidgets components: ${RIDE_WX_COMPONENTS}")
set(wxWidgets_USE_STATIC ON)
find_package(wxWidgets COMPONENTS ${RIDE_WX_COMPONENTS} REQUIRED)

include( "${wxWidgets_USE_FILE}" )
message(STATUS "wxWidgets use file: ${wxWidgets_USE_FILE}")

FILE(GLOB src_glob *.cc;*.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/ride_compiler_settings.in.h"
  "${CMAKE_CURRENT_BINARY_DIR}/ride_compiler_settings.h"
)

set(src settings.proto.cc settings.proto.h resources/resources.rc ${src_glob})
set(generated_src generated/ui.cpp generated/ui.h)

set(tinyxml_src
	${CMAKE_SOURCE_DIR}/external/tinyxml2/tinyxml2.cpp
	${CMAKE_SOURCE_DIR}/external/tinyxml2/tinyxml2.h
	)
include_directories(${CMAKE_SOURCE_DIR}/external/tinyxml2/)

set(switcher_src
	switcher/ctrl.cc
	switcher/ctrl.h
	switcher/dialog.cc
	switcher/dialog.h
	switcher/group.cc
	switcher/group.h
	switcher/item.cc
	switcher/item.h
	switcher/itemlist.cc
	switcher/itemlist.h
	switcher/style.cc
	switcher/style.h
	)

set(bombs_src
	games/bombs/bombs.cpp
	games/bombs/bombs.h
	games/bombs/bombs_glue.cc
	games/bombs/bombs_glue.h
	games/bombs/bombscanvas.cpp
	games/bombs/game.cpp
	games/bombs/game.h
	)

set(forty_src
	games/forty/forty_glue.cc
	games/forty/forty_glue.h
	games/forty/canvas.cpp
	games/forty/canvas.h
	games/forty/card.cpp
	games/forty/card.h
	games/forty/forty.cpp
	games/forty/forty.h
	games/forty/game.cpp
	games/forty/game.h
	games/forty/pictures.xpm
	games/forty/pile.cpp
	games/forty/pile.h
	games/forty/scorefil.cpp
	games/forty/scorefil.h
	games/forty/symbols.xpm
	)

set(games_src ${bombs_src} ${forty_src})

source_group("" FILES ${src})
source_group("generated" FILES ${generated_src} ${SETTINGS_PROTO_SRCS} ${SETTINGS_PROTO_HDRS})
source_group("switcher" FILES ${switcher_src})
source_group("games/bombs" FILES ${bombs_src})
source_group("games/forty" FILES ${forty_src})
source_group("external/tinyxml2" FILES ${tinyxml_src})
source_group("external/pbjson" FILES ${pbjson_src})

if(APPLE)
	set(OSX_ICON_FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/application.icns)
	set_source_files_properties(${OSX_ICON_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
	source_group("resources" FILES ${OSX_ICON_FILES})
	set(generated_src ${generated_src} ${OSX_ICON_FILES})
ENDIF()

add_executable(
	${PROJECT_NAME} WIN32 MACOSX_BUNDLE
	${src} ${generated_src} ${switcher_src} ${games_src} ${tinyxml_src} ${pbjson_src}
)

install(TARGETS ${PROJECT_NAME} DESTINATION ".")

# install dependencies
if(APPLE)
  install(SCRIPT macdylibbundler.cmake)
endif()

# make apple installer look prettier
if(APPLE)
	set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_ICON_FILE "application")
	set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/bundle-info.plist")
ENDIF()

target_link_libraries(
	${PROJECT_NAME}
	${wxWidgets_LIBRARIES}
)


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
