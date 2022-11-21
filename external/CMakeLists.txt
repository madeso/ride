add_subdirectory(embed)
add_subdirectory(catchy)




###################################################################################################
# glad
set(glad_src
    glad/src/glad.c
    glad/include/glad/glad.h
)
add_library(glad STATIC
    ${glad_src}
)
source_group("" FILES ${glad_src})
target_include_directories(glad SYSTEM
    PUBLIC
        glad/include
)
target_link_libraries(glad
    PUBLIC
        ${CMAKE_DL_LIBS}
)
set_target_properties(glad PROPERTIES FOLDER "External")
add_library(external::glad ALIAS glad)



###################################################################################################
# OpenGL
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
add_library(opengl INTERFACE)
target_link_libraries(opengl INTERFACE OpenGL::GL)
add_library(external::opengl ALIAS opengl)



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
add_library(external::sdl2 ALIAS sdl2)



###################################################################################################
# stb truetype
set(stb_truetype_src
    stb/stb_truetype.c
    stb/stb/stb_truetype.h
)
add_library(stb_truetype STATIC
    ${stb_truetype_src}
)
source_group("" FILES ${stb_truetype_src})
target_include_directories(stb_truetype
    PUBLIC
        SYSTEM stb/stb
)
set_target_properties(stb_truetype PROPERTIES FOLDER "External")
add_library(external::stb_truetype ALIAS stb_truetype)



###################################################################################################
# stb image
set(stb_image_src
    stb/stb_image.c
    stb/stb/stb_image.h
)
add_library(stb_image STATIC
    ${stb_image_src}
)
source_group("" FILES ${stb_image_src})
target_include_directories(stb_image
    PUBLIC
        SYSTEM stb/stb
)
set_target_properties(stb_image PROPERTIES FOLDER "External")
add_library(external::stb_image ALIAS stb_image)


###################################################################################################
# open color
add_library(open_color INTERFACE)
target_include_directories(open_color
    INTERFACE
        SYSTEM open-color/include
)
add_library(external::open_color ALIAS open_color)



###################################################################################################
# glm
add_library(glm INTERFACE)
target_include_directories(glm SYSTEM
    INTERFACE
        glm
)
add_library(external::glm ALIAS glm)




