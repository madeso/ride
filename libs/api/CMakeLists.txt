set(src
    src/api/app.cc src/api/app.h
    src/api/font.cc src/api/font.h
    src/api/image.cc src/api/image.h
    src/api/key.cc src/api/key.h
    src/api/mouse_button.cc src/api/mouse_button.h
    src/api/rencache.cc src/api/rencache.h
    src/api/renderer.cc src/api/renderer.h
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
    PRIVATE
        project_options
        stb_truetype
        stb_image
)



add_executable(
    example_squares WIN32 MACOSX_BUNDLE
	examples/squares.cc
)
set_crt_to_static(example_squares)
target_link_libraries(example_squares
    PRIVATE
        project_options
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
    PUBLIC
        api
        data_font
)
