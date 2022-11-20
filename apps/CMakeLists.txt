###############################################################################
############# sdl version of ride

set(src
    main.cc
)
source_group("" FILES ${src})
add_executable (
    ride WIN32 MACOSX_BUNDLE
	${src}
)
set_crt_to_static(ride)
target_link_libraries(ride
    PRIVATE
        project_options
    PUBLIC
        libride
        apigl
        sdl2
)
set_target_properties(ride PROPERTIES FOLDER "Applications")

###############################################################################
############# wx version of ride
if(ride_wx)
    set(RIDE_WX_COMPONENTS core base stc adv)
    set(wxWidgets_USE_STATIC ON)
    find_package(wxWidgets COMPONENTS ${RIDE_WX_COMPONENTS} REQUIRED)
    include( "${wxWidgets_USE_FILE}" )
    set(srcwx main.wx.cc)
    add_executable(
        ridewx WIN32 MACOSX_BUNDLE
        ${srcwx}
    )
    set_crt_to_static(ridewx)
    target_link_libraries(ridewx
        ${wxWidgets_LIBRARIES}
        libride
        project_options
    )
endif()



###############################################################################
############# todo(Gustav): fix installation
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
