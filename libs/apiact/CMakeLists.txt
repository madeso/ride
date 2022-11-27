add_subdirectory(apigl)
if(ride_wx)
    add_subdirectory(apiwx)
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
    set_target_properties(${ARG_NAME} PROPERTIES FOLDER "${FOLDER}")

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
        set_target_properties(${ARG_NAME}wx PROPERTIES FOLDER "${FOLDER}")
    endif()
endfunction()




create_app(example_squares "Examples"
    SOURCES examples/squares.cc
    LIBRARIES 
        ride::options
        ride::warnings
        external::open_color
)

create_app(example_images "Examples"
    SOURCES examples/images.cc
    LIBRARIES 
        ride::options
        ride::warnings
        data::data_logo
)

create_app(example_font "Examples"
    SOURCES examples/font.cc
    LIBRARIES 
        ride::options
        ride::warnings
        external::open_color
)

create_app(example_scope "Examples"
    SOURCES examples/scope.cc
    LIBRARIES 
        ride::options
        ride::warnings
)
