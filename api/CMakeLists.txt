set(src
    api/renderer.cc api/renderer.h
    api/rencache.cc api/rencache.h
    api/color.cc api/color.h
    api/scale.cc api/scale.h
)
add_library(
    api STATIC
	${src}
)
target_include_directories(
    api
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(api
    project_options
    sdl2
    stb_truetype
)
