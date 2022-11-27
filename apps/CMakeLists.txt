function(create_app ARG_NAME)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs SOURCES LIBRARIES)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(src
        ${ARG_SOURCES}
    )
    source_group("" FILES ${src})

    # sdl
    add_executable(
        ${ARG_NAME} WIN32 MACOSX_BUNDLE
    	${src}
    )
    set_crt_to_static(${ARG_NAME})
    target_link_libraries(${ARG_NAME}
        PRIVATE
            ride::options
        PUBLIC
            ${ARG_LIBRARIES}
            ride::apigl
            external::sdl2
    )
    set_target_properties(${ARG_NAME} PROPERTIES FOLDER "Applications")

    if(ride_wx)
        add_executable(${ARG_NAME}wx WIN32 MACOSX_BUNDLE ${src})
        set_crt_to_static(${ARG_NAME}wx)
        target_link_libraries(${ARG_NAME}wx
            PRIVATE
                ride::options
            PUBLIC
                ride::apiwx
                ${ARG_LIBRARIES}
        )
    endif()
endfunction()

###############################################################################
############# sdl version of ride

create_app(ride
    SOURCES main.cc
    LIBRARIES ride::libride
)

###############################################################################
############# wx version of ride




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
