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
)
add_library(apigl STATIC ${src})
target_include_directories(apigl
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
set_crt_to_static(apigl)
target_link_libraries(apigl
    PUBLIC
        sdl2
        embed
        base
        api
        fmt::fmt
    PRIVATE
        project_options
        project_warnings
        stb_truetype
        stb_image
        data_font
        opengl
        glad
        glm
)



add_executable(
    example_squares WIN32 MACOSX_BUNDLE
	examples/squares.cc
)
set_crt_to_static(example_squares)
target_link_libraries(example_squares
    PRIVATE
        project_options
        project_warnings
        open_color
    PUBLIC
        apigl
)


add_executable(
    example_images WIN32 MACOSX_BUNDLE
	examples/images.cc
)
set_crt_to_static(example_images)
target_link_libraries(example_images
    PRIVATE
        project_options
        project_warnings
    PUBLIC
        apigl
        data_logo
)


add_executable(
    example_font WIN32 MACOSX_BUNDLE
	examples/font.cc
)
set_crt_to_static(example_font)
target_link_libraries(example_font
    PRIVATE
        project_options
        project_warnings
    PUBLIC
        apigl
)


add_executable(
    example_scope WIN32 MACOSX_BUNDLE
	examples/scope.cc
)
set_crt_to_static(example_scope)
target_link_libraries(example_scope
    PRIVATE
        project_options
        project_warnings
    PUBLIC
        apigl
)
