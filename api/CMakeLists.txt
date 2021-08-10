set(src
    renderer.cc renderer.h
    rencache.cc rencache.h
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
