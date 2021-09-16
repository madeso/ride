set(src
    src/api/app.cc src/api/app.h
    src/api/color.cc src/api/color.h
    src/api/file.cc src/api/file.h
    src/api/font.cc src/api/font.h
    src/api/image.cc src/api/image.h
    src/api/numeric.cc src/api/numeric.h
    src/api/rect.cc src/api/rect.h
    src/api/rencache.cc src/api/rencache.h
    src/api/renderer.cc src/api/renderer.h
    src/api/scale.cc src/api/scale.h
    src/api/size.cc src/api/size.h
    src/api/string.cc src/api/string.h
    src/api/utf8.cc src/api/utf8.h
    src/api/vec2.cc src/api/vec2.h
)
add_library(api STATIC ${src})
target_include_directories(api
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
set_crt_to_static(api)
target_link_libraries(api
    PRIVATE
        project_options
        sdl2
        stb_truetype
        stb_image
        embed
)
