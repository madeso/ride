add_subdirectory(catchy)

###################################################################################################
# SDL2
find_package(SDL2 REQUIRED)
add_library(sdl2 INTERFACE)
target_include_directories(sdl2
    INTERFACE
        SYSTEM ${SDL2_INCLUDE_DIR}
)
target_link_libraries(sdl2
    INTERFACE
        ${SDL2_LIBRARY}
        ${CMAKE_DL_LIBS}
)

add_library(stb_truetype STATIC
    stb/stb_truetype.c
    stb/stb/stb_truetype.h
)
target_include_directories(stb_truetype
    PUBLIC
        SYSTEM stb/stb
)

add_library(stb_image STATIC
    stb/stb_image.c
    stb/stb/stb_image.h
)
target_include_directories(stb_image
    PUBLIC
        SYSTEM stb/stb
)

add_subdirectory(embed)


add_library(open_color INTERFACE)
target_include_directories(open_color
    INTERFACE
        SYSTEM open-color/include
)
