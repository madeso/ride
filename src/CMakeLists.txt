FILE(GLOB src_glob ride/*.cc;ride/*.h)

set(src_glob
	ride/auix.cc
	ride/autocomplete.cc
	ride/buildconfigurationsdlg.cc
	ride/builtinthemes.cc
	ride/cargo.cc
	ride/cmdrunner.cc
	ride/compilermessage.cc
	ride/createnewfiledlg.cc
	ride/createnewprojectdlg.cc
	ride/deletefolderdlg.cc
	ride/enabledisable.cc
	ride/enviroment.cc
	ride/fileedit.cc
	ride/filepropertiesdlg.cc
	ride/finddlg.cc
	ride/form.cc
	ride/language.cc
	ride/loadresult.cc
	ride/main.cc
	ride/mainwindow.cc
	ride/outputcontrol.cc
	ride/outputdirector.cc
	ride/path.cc
	ride/pathtester.cc
	ride/project.cc
	ride/projectexplorer.cc
	ride/projectsettingsdlg.cc
	ride/proto.cc
	ride/quickopendlg.cc
	ride/runconfigurationsdlg.cc
	ride/runner.cc
	ride/settings.cc
	ride/settingscommon.cc
	ride/settingsdlg.cc
	ride/settingseditortab.cc
	ride/settingsenviromenttab.cc
	ride/settingsfontstab.cc
	ride/settingsindicatortab.cc
	ride/settingsmarkerstab.cc
	ride/settings.proto.cc
	ride/settingsthemestab.cc
	ride/settingswindowtab.cc
	ride/startpage.cc
	ride/stcutils.cc
	ride/stringutils.cc
	ride/tab.cc
	ride/togui.cc
	ride/variableeditor.cc
	ride/wxutils.cc

	ride/auix.h
	ride/autocomplete.h
	ride/buildconfigurationsdlg.h
	ride/builtinthemes.h
	ride/cargo.h
	ride/cmdrunner.h
	ride/compilermessage.h
	ride/createnewfiledlg.h
	ride/createnewprojectdlg.h
	ride/deletefolderdlg.h
	ride/enabledisable.h
	ride/enviroment.h
	ride/fileedit.h
	ride/filepropertiesdlg.h
	ride/finddlg.h
	ride/form.h
	ride/guilist.h
	ride/language.h
	ride/loadresult.h
	ride/mainwindow.h
	ride/menuevent.h
	ride/outputcontrol.h
	ride/outputdirector.h
	ride/path.h
	ride/pathtester.h
	ride/pch.private.h
	ride/pch.public.h
	ride/projectexplorer.h
	ride/project.h
	ride/projectsettingsdlg.h
	ride/proto.h
	ride/quickopendlg.h
	ride/ride_compiler_settings.in.h
	ride/runconfigurationsdlg.h
	ride/runner.h
	ride/settingscommon.h
	ride/settingsdlg.h
	ride/settingseditortab.h
	ride/settingsenviromenttab.h
	ride/settingsfontstab.h
	ride/settings.h
	ride/settingsindicatortab.h
	ride/settingsmarkerstab.h
	ride/settings.proto.h
	ride/settingsthemestab.h
	ride/settingswindowtab.h
	ride/startpage.h
	ride/stcutils.h
	ride/stringutils.h
	ride/tab.h
	ride/togui.h
	ride/variableeditor.h
	ride/version.h
	ride/wx.h
	ride/wxid.h
	ride/wxutils.h
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
		COMMAND "mt.exe" -manifest \"${CMAKE_CURRENT_SOURCE_DIR}\\dpiaware.manifest.xml\" -inputresource:\"$<TARGET_FILE:ride>\"\;\#1 -outputresource:\"$<TARGET_FILE:ride>\"\;\#1
		COMMENT "Adding display aware manifest..."
	)
ENDIF(MSVC)
