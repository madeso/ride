add_subdirectory(sdl_base)
add_subdirectory(gl)
if(ride_blit)
    add_subdirectory(blit)
endif()
if(ride_wx)
    add_subdirectory(wx)
endif()




function(create_app ARG_NAME FOLDER)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs SOURCES LIBRARIES)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(src
        ${ARG_SOURCES}
    )
    source_group("" FILES ${src})
    source_group_pch(${ARG_NAME})

    # sdl - opengl
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
            ride::backend_gl
            external::sdl2
    )
    set_target_properties(${ARG_NAME} PROPERTIES FOLDER "Apps/SDL OpenGL/${FOLDER}")


    # sdl - blit
    if(ride_blit)
        add_executable(
            ${ARG_NAME}-blit WIN32 MACOSX_BUNDLE
            ${src}
        )
        set_crt_to_static(${ARG_NAME}-blit)
        target_link_libraries(${ARG_NAME}-blit
            PRIVATE
                ride::options
            PUBLIC
                ${ARG_LIBRARIES}
                ride::backend_blit
                external::sdl2
        )
        set_target_properties(${ARG_NAME}-blit PROPERTIES FOLDER "Apps/SDL Blit/${FOLDER}")
    endif()

    if(ride_wx)
        add_executable(${ARG_NAME}wx WIN32 MACOSX_BUNDLE ${src})
        set_crt_to_static(${ARG_NAME}wx)
        target_link_libraries(${ARG_NAME}wx
            PRIVATE
                ride::options
            PUBLIC
                ride::backend_wx
                ${ARG_LIBRARIES}
        )
        set_target_properties(${ARG_NAME}wx PROPERTIES FOLDER "Apps/wxWidgets/${FOLDER}")
    endif()
endfunction()

