set(RIDE_WX_COMPONENTS core base stc adv)
set(wxWidgets_USE_STATIC ON)
find_package(wxWidgets COMPONENTS ${RIDE_WX_COMPONENTS} REQUIRED)
include( "${wxWidgets_USE_FILE}" )


set(src main.cc)

set(apple_resources)
if(APPLE)
	set(OSX_ICON_FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/application.icns)
	set_source_files_properties(${OSX_ICON_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
	source_group("apple" FILES ${OSX_ICON_FILES})
	set(apple_resources ${OSX_ICON_FILES})
endif()

add_executable (
    ride WIN32 MACOSX_BUNDLE
	${src} ${apple_resources}
)
target_link_libraries(ride
    ${wxWidgets_LIBRARIES}
    libride
    project_options
)

install(TARGETS ride DESTINATION ".")

if(APPLE)
  # install dependencies
  install(SCRIPT macdylibbundler.cmake)

  # make apple installer look prettier
	set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_ICON_FILE "application")
	set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/bundle-info.plist")
endif()

# Amend manifest to tell Windows that the application is DPI aware (needed for Windows 8.1 and up)
IF (MSVC)
    ADD_CUSTOM_COMMAND(
        TARGET ride
        POST_BUILD
        COMMAND "mt.exe" -manifest \"${CMAKE_CURRENT_SOURCE_DIR}\\dpiaware.manifest.xml\" -inputresource:\"$<TARGET_FILE:${PROJECT_NAME}>\"\;\#1 -outputresource:\"$<TARGET_FILE:${PROJECT_NAME}>\"\;\#1
        COMMENT "Adding display aware manifest..."
    )
ENDIF(MSVC)
