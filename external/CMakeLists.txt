###################################################################################################
# SDL2
find_package(SDL2 REQUIRED)
add_library(sdl2 INTERFACE)
target_include_directories(sdl2
    INTERFACE
        ${SDL2_INCLUDE_DIR}
)
target_link_libraries(sdl2
    INTERFACE
        ${SDL2_LIBRARY}
        ${CMAKE_DL_LIBS}
)

add_library(stb_truetype STATIC
    stb/stb_truetype.c
    stb/stb_truetype.h
)
target_include_directories(stb_truetype
    PUBLIC
        stb
)
