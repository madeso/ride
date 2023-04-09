## api library
set(src
    src/api/app.cc src/api/app.h
    src/api/font.cc src/api/font.h
    src/api/image.cc src/api/image.h
    src/api/key.cc src/api/key.h
    src/api/mouse_button.cc src/api/mouse_button.h
    src/api/renderer.cc src/api/renderer.h

    src/api/pch.public.h
    src/api/pch.private.h
)
source_group("" FILES ${src})
add_library(api STATIC ${src})
target_include_directories(api
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
set_crt_to_static(api)
target_link_libraries(api
    PUBLIC
        embed::embed
        ride::base
        fmt::fmt
    PRIVATE
        ride::options
        ride::warnings
        external::stb_truetype
        external::stb_image
)
target_precompile_headers(api
    PUBLIC src/api/pch.public.h
    PRIVATE src/api/pch.private.h
)
set_target_properties(api PROPERTIES FOLDER "Libraries")
add_library(ride::api ALIAS api)



# api samples
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
