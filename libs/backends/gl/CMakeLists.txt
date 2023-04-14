set(src
    src/backend/app.cc src/backend/app.h
    src/backend/main.cc src/backend/main.h
    
    src/backend/texture.cc src/backend/texture.h
    src/backend/shader.cc src/backend/shader.h
    src/backend/spritebatch.cc src/backend/spritebatch.h
    src/backend/vertex_layout.cc src/backend/vertex_layout.h
    src/backend/uniform.cc src/backend/uniform.h
    src/backend/dependency_opengl.cc src/backend/dependency_opengl.h
    src/backend/font.cc src/backend/font.h

    src/backend/pch.public.h
    src/backend/pch.private.h
)
source_group("" FILES ${src})
add_library(gl STATIC ${src})
target_include_directories(gl
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
set_crt_to_static(gl)
source_group_pch(gl)
target_link_libraries(gl
    PUBLIC
        external::sdl2
        embed::embed
        ride::core
        ride::api
        fmt::fmt
        ride::sdl_base
    PRIVATE
        ride::options
        ride::warnings
        external::stb_image
        external::opengl
        external::glad
        external::glm
)
set_target_properties(gl PROPERTIES FOLDER "Libraries/Backends")
target_precompile_headers(gl
    PUBLIC src/backend/pch.public.h
    PRIVATE src/backend/pch.private.h
)
add_library(ride::backend_gl ALIAS gl)

