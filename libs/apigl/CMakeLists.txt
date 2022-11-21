set(src
    src/apigl/app.cc src/apigl/app.h
    src/apigl/cursorcache.cc src/apigl/cursorcache.h
    src/apigl/font.cc src/apigl/font.h
    src/apigl/key.cc src/apigl/key.h
    src/apigl/log.cc src/apigl/log.h
    src/apigl/main.cc src/apigl/main.h
    src/apigl/mouse_button.cc src/apigl/mouse_button.h

    src/apigl/texture.cc src/apigl/texture.h
    src/apigl/shader.cc src/apigl/shader.h
    src/apigl/spritebatch.cc src/apigl/spritebatch.h
    src/apigl/vertex_layout.cc src/apigl/vertex_layout.h
    src/apigl/uniform.cc src/apigl/uniform.h
    src/apigl/dependency_opengl.cc src/apigl/dependency_opengl.h

    src/apigl/pch.public.h
    src/apigl/pch.private.h
)
source_group("" FILES ${src})
add_library(apigl STATIC ${src})
target_include_directories(apigl
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
set_crt_to_static(apigl)
target_link_libraries(apigl
    PUBLIC
        external::sdl2
        embed::embed
        ride::base
        ride::api
        fmt::fmt
    PRIVATE
        ride::options
        ride::warnings
        external::stb_truetype
        external::stb_image
        data::data_font
        external::opengl
        external::glad
        external::glm
)
set_target_properties(apigl PROPERTIES FOLDER "Libraries")
target_precompile_headers(apigl
    PUBLIC src/apigl/pch.public.h
    PRIVATE src/apigl/pch.private.h
)
add_library(ride::apigl ALIAS apigl)



set(example_squares_src
	examples/squares.cc
)
source_group("" FILES ${example_squares_src})
add_executable(example_squares WIN32 MACOSX_BUNDLE ${example_squares_src})
set_crt_to_static(example_squares)
target_link_libraries(example_squares
    PRIVATE
        ride::options
        ride::warnings
        external::open_color
    PUBLIC
        ride::apigl
)
set_target_properties(example_squares PROPERTIES FOLDER "Examples")


set(example_images_src
	examples/images.cc
)
source_group("" FILES ${example_images_src})
add_executable(example_images WIN32 MACOSX_BUNDLE ${example_images_src})
set_crt_to_static(example_images)
target_link_libraries(example_images
    PRIVATE
        ride::options
        ride::warnings
    PUBLIC
        ride::apigl
        data::data_logo
)
set_target_properties(example_images PROPERTIES FOLDER "Examples")


set(example_font_src
	examples/font.cc
)
source_group("" FILES ${example_font_src})
add_executable(example_font WIN32 MACOSX_BUNDLE ${example_font_src})
set_crt_to_static(example_font)
target_link_libraries(example_font
    PRIVATE
        ride::options
        ride::warnings
    PUBLIC
        ride::apigl
)
set_target_properties(example_font PROPERTIES FOLDER "Examples")


set(example_scope_src
	examples/scope.cc
)
source_group("" FILES ${example_scope_src})
add_executable(example_scope WIN32 MACOSX_BUNDLE ${example_scope_src})
set_crt_to_static(example_scope)
target_link_libraries(example_scope
    PRIVATE
        ride::options
        ride::warnings
    PUBLIC
        ride::apigl
)
set_target_properties(example_scope PROPERTIES FOLDER "Examples")

