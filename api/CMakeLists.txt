set(src
    api/color.cc api/color.h
    api/file.cc api/file.h
    api/font.cc api/font.h
    api/image.cc api/image.h
    api/numeric.cc api/numeric.h
    api/rect.cc api/rect.h
    api/rencache.cc api/rencache.h
    api/renderer.cc api/renderer.h
    api/scale.cc api/scale.h
    api/size.cc api/size.h
    api/string.cc api/string.h
    api/utf8.cc api/utf8.h
    api/vec2.cc api/vec2.h
)
add_library(api STATIC ${src})
target_include_directories(api
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
set_crt_to_static(api)
target_link_libraries(api
    PRIVATE
        project_options
        sdl2
        stb_truetype
)
