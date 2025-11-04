FILE(GLOB src_glob ride/*.cc;ride/*.h)

set(src_glob
	ride/auix.cc ride/auix.h
	ride/autocomplete.cc ride/autocomplete.h
	ride/buildconfigurationsdlg.cc ride/buildconfigurationsdlg.h
	ride/builtinthemes.cc ride/builtinthemes.h
	ride/cargo.cc ride/cargo.h
	ride/cmdrunner.cc ride/cmdrunner.h
	ride/compilermessage.cc ride/compilermessage.h
	ride/createnewfiledlg.cc ride/createnewfiledlg.h
	ride/createnewprojectdlg.cc ride/createnewprojectdlg.h
	ride/deletefolderdlg.cc ride/deletefolderdlg.h
	ride/enabledisable.cc ride/enabledisable.h
	ride/enviroment.cc ride/enviroment.h
	ride/fileedit.cc ride/fileedit.h
	ride/filepropertiesdlg.cc ride/filepropertiesdlg.h
	ride/finddlg.cc ride/finddlg.h
	ride/form.cc ride/form.h
	ride/language.cc ride/language.h
	ride/loadresult.cc ride/loadresult.h
	ride/mainwindow.cc ride/mainwindow.h
	ride/outputcontrol.cc ride/outputcontrol.h
	ride/outputdirector.cc ride/outputdirector.h
	ride/path.cc ride/path.h
	ride/pathtester.cc ride/pathtester.h
	ride/project.cc ride/project.h
	ride/projectexplorer.cc ride/projectexplorer.h
	ride/projectsettingsdlg.cc ride/projectsettingsdlg.h
	ride/proto.cc ride/proto.h
	ride/quickopendlg.cc ride/quickopendlg.h
	ride/runconfigurationsdlg.cc ride/runconfigurationsdlg.h
	ride/runner.cc ride/runner.h
	ride/settings.cc ride/settings.h
	ride/settings.proto.cc ride/settings.proto.h
	ride/startpage.cc ride/startpage.h
	ride/stcutils.cc ride/stcutils.h
	ride/stringutils.cc ride/stringutils.h
	ride/tab.cc ride/tab.h
	ride/togui.cc ride/togui.h
	ride/variableeditor.cc ride/variableeditor.h
	ride/wxutils.cc ride/wxutils.h

	ride/guilist.h
	ride/menuevent.h
	ride/pch.private.h
	ride/pch.public.h
	ride/ride_compiler_settings.in.h
	ride/version.h
	ride/wx.h
	ride/wxid.h

	ride/main.cc
)

configure_file (
	"${CMAKE_CURRENT_SOURCE_DIR}/ride/ride_compiler_settings.in.h"
	"${CMAKE_CURRENT_BINARY_DIR}/ride_compiler_settings.h"
)

set(src
	ride/path.cc ride/path.h
	ride/settings.proto.cc
	ride/settings.proto.h
	ride/resources/resources.rc
	${src_glob}
)
set(generated_src
	ride/generated/ui.cpp
	ride/generated/ui.h
)

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

if(APPLE)
	set(OSX_ICON_FILES ${CMAKE_CURRENT_SOURCE_DIR}/ride/resources/application.icns)
	set_source_files_properties(${OSX_ICON_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
	source_group("resources" FILES ${OSX_ICON_FILES})
	set(generated_src ${generated_src} ${OSX_ICON_FILES})
ENDIF()

add_executable(ride WIN32 MACOSX_BUNDLE
	${src}
	${generated_src}
	${switcher_src}
	${games_src}
)

target_precompile_headers(ride
    PUBLIC ride/pch.public.h
    PRIVATE ride/pch.private.h
)

install(TARGETS ride DESTINATION ".")

if(APPLE)
	# install dependencies
	install(SCRIPT macdylibbundler.cmake)

	# make apple installer look prettier
	set_target_properties(ride PROPERTIES
		MACOSX_BUNDLE_ICON_FILE
			"application"
		MACOSX_BUNDLE_INFO_PLIST
			"${CMAKE_CURRENT_SOURCE_DIR}/bundle-info.plist"
	)
ENDIF()

target_include_directories(ride
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(ride
	PRIVATE
		external::wxWidgets
		external::cpptoml
		external::tinyxml
		external::json
		common::options
)


IF (MSVC)
	# Amend manifest to tell Windows that the application is DPI aware
	# (needed for Windows 8.1 and up)
	ADD_CUSTOM_COMMAND(
		TARGET ride
		POST_BUILD
		COMMAND "mt.exe" -manifest \"${CMAKE_CURRENT_SOURCE_DIR}\\ride\\dpiaware.manifest.xml\" -inputresource:\"$<TARGET_FILE:ride>\"\;\#1 -outputresource:\"$<TARGET_FILE:ride>\"\;\#1
		COMMENT "Adding display aware manifest..."
	)
ENDIF(MSVC)
