add_subdirectory(embed)
add_subdirectory(catchy)




###################################################################################################
# glad
add_library(glad STATIC
    glad/src/glad.c
    glad/include/glad/glad.h
)
target_include_directories(glad SYSTEM
    PUBLIC
        glad/include
)
target_link_libraries(glad
    PUBLIC
        ${CMAKE_DL_LIBS}
)



###################################################################################################
# OpenGL
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
add_library(opengl INTERFACE)
target_link_libraries(opengl INTERFACE OpenGL::GL)



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



###################################################################################################
# stb truetype
add_library(stb_truetype STATIC
    stb/stb_truetype.c
    stb/stb/stb_truetype.h
)
target_include_directories(stb_truetype
    PUBLIC
        SYSTEM stb/stb
)



###################################################################################################
# stb image
add_library(stb_image STATIC
    stb/stb_image.c
    stb/stb/stb_image.h
)
target_include_directories(stb_image
    PUBLIC
        SYSTEM stb/stb
)



###################################################################################################
# open color
add_library(open_color INTERFACE)
target_include_directories(open_color
    INTERFACE
        SYSTEM open-color/include
)

###################################################################################################
# glm
add_library(glm INTERFACE)
target_include_directories(glm SYSTEM
    INTERFACE
        glm
)
