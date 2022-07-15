set(src
    src/api/app.cc src/api/app.h
    src/api/cursorcache.cc src/api/cursorcache.h
    src/api/font.cc src/api/font.h
    src/api/image.cc src/api/image.h
    src/api/key.cc src/api/key.h
    src/api/log.cc src/api/log.h
    src/api/main.cc src/api/main.h
    src/api/mouse_button.cc src/api/mouse_button.h

    src/api/texture.cc src/api/texture.h
    src/api/shader.cc src/api/shader.h
    src/api/spritebatch.cc src/api/spritebatch.h
    src/api/vertex_layout.cc src/api/vertex_layout.h
    src/api/uniform.cc src/api/uniform.h
    src/api/dependency_opengl.cc src/api/dependency_opengl.h
)
add_library(api STATIC ${src})
target_include_directories(api
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
set_crt_to_static(api)
target_link_libraries(api
    PUBLIC
        sdl2
        embed
        base
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
        api
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
        api
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
        api
)
