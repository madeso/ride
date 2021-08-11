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



add_library(fmt STATIC
    # fmt-8.0.1/src/fmt.cc
    fmt-8.0.1/src/format.cc
    fmt-8.0.1/src/os.cc
)
target_include_directories(fmt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/fmt-8.0.1/include)
target_compile_features(fmt PRIVATE cxx_std_17)




add_library(stb_truetype STATIC
    stb/stb_truetype.c
    stb/stb_truetype.h
)
target_include_directories(stb_truetype
    PUBLIC
        stb
)
