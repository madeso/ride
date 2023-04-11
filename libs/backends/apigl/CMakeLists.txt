set(src
    src/backend/app.cc src/backend/app.h
    src/backend/cursorcache.cc src/backend/cursorcache.h
    src/backend/font.cc src/backend/font.h
    src/backend/key.cc src/backend/key.h
    src/backend/log.cc src/backend/log.h
    src/backend/main.cc src/backend/main.h
    src/backend/mouse_button.cc src/backend/mouse_button.h

    src/backend/texture.cc src/backend/texture.h
    src/backend/shader.cc src/backend/shader.h
    src/backend/spritebatch.cc src/backend/spritebatch.h
    src/backend/vertex_layout.cc src/backend/vertex_layout.h
    src/backend/uniform.cc src/backend/uniform.h
    src/backend/dependency_opengl.cc src/backend/dependency_opengl.h

    src/backend/pch.public.h
    src/backend/pch.private.h
)
source_group("" FILES ${src})
add_library(apigl STATIC ${src})
target_include_directories(apigl
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
set_crt_to_static(apigl)
source_group_pch(apigl)
target_link_libraries(apigl
    PUBLIC
        external::sdl2
        embed::embed
        ride::core
        ride::api
        fmt::fmt
    PRIVATE
        ride::options
        ride::warnings
        external::stb_truetype
        external::stb_image
        data::data_font
        external::opengl
        external::glad
        external::glm
)
set_target_properties(apigl PROPERTIES FOLDER "Libraries/Backends")
target_precompile_headers(apigl
    PUBLIC src/backend/pch.public.h
    PRIVATE src/backend/pch.private.h
)
add_library(ride::apigl ALIAS apigl)

