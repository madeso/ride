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

FILE(GLOB src_glob ride/*.cc;ride/*.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/ride/ride_compiler_settings.in.h"
  "${CMAKE_CURRENT_BINARY_DIR}/ride_compiler_settings.h"
)

set(src
  ride/settings.proto.cc
  ride/settings.proto.h
  ride/resources/resources.rc
  ${src_glob}
)
set(generated_src
  ride/generated/ui.cpp
  ride/generated/ui.h
)

set(tinyxml_src
	${CMAKE_SOURCE_DIR}/external/tinyxml2/tinyxml2.cpp
	${CMAKE_SOURCE_DIR}/external/tinyxml2/tinyxml2.h
	)
include_directories(${CMAKE_SOURCE_DIR}/external/tinyxml2/)

set(switcher_src
	ride/switcher/ctrl.cc
	ride/switcher/ctrl.h
	ride/switcher/dialog.cc
	ride/switcher/dialog.h
	ride/switcher/group.cc
	ride/switcher/group.h
	ride/switcher/item.cc
	ride/switcher/item.h
	ride/switcher/itemlist.cc
	ride/switcher/itemlist.h
	ride/switcher/style.cc
	ride/switcher/style.h
	)

set(bombs_src
	ride/games/bombs/bombs.cpp
	ride/games/bombs/bombs.h
	ride/games/bombs/bombs_glue.cc
	ride/games/bombs/bombs_glue.h
	ride/games/bombs/bombscanvas.cpp
	ride/games/bombs/game.cpp
	ride/games/bombs/game.h
	)

set(forty_src
	ride/games/forty/forty_glue.cc
	ride/games/forty/forty_glue.h
	ride/games/forty/canvas.cpp
	ride/games/forty/canvas.h
	ride/games/forty/card.cpp
	ride/games/forty/card.h
	ride/games/forty/forty.cpp
	ride/games/forty/forty.h
	ride/games/forty/game.cpp
	ride/games/forty/game.h
	ride/games/forty/pictures.xpm
	ride/games/forty/pile.cpp
	ride/games/forty/pile.h
	ride/games/forty/scorefil.cpp
	ride/games/forty/scorefil.h
	ride/games/forty/symbols.xpm
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
	ride WIN32 MACOSX_BUNDLE
	${src} ${generated_src} ${switcher_src} ${games_src} ${tinyxml_src} ${pbjson_src}
)

install(TARGETS ride DESTINATION ".")

# install dependencies
if(APPLE)
  install(SCRIPT macdylibbundler.cmake)
endif()

# make apple installer look prettier
if(APPLE)
	set_target_properties(ride PROPERTIES MACOSX_BUNDLE_ICON_FILE "application")
	set_target_properties(ride PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/bundle-info.plist")
ENDIF()

target_link_libraries(
	ride
	${wxWidgets_LIBRARIES}
)


# Amend manifest to tell Windows that the application is DPI aware (needed for Windows 8.1 and up)
IF (MSVC)
	IF (CMAKE_MAJOR_VERSION LESS 3)
		MESSAGE(WARNING "CMake version 3.0 or newer is required use build variable TARGET_FILE")
	ELSE()
		ADD_CUSTOM_COMMAND(
			TARGET ride
			POST_BUILD
			COMMAND "mt.exe" -manifest \"${CMAKE_CURRENT_SOURCE_DIR}\\dpiaware.manifest.xml\" -inputresource:\"$<TARGET_FILE:ride>\"\;\#1 -outputresource:\"$<TARGET_FILE:ride>\"\;\#1
			COMMENT "Adding display aware manifest..."
		)
	ENDIF()
ENDIF(MSVC)
