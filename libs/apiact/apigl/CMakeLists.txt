set(src
    src/apiact/app.cc src/apiact/app.h
    src/apiact/cursorcache.cc src/apiact/cursorcache.h
    src/apiact/font.cc src/apiact/font.h
    src/apiact/key.cc src/apiact/key.h
    src/apiact/log.cc src/apiact/log.h
    src/apiact/main.cc src/apiact/main.h
    src/apiact/mouse_button.cc src/apiact/mouse_button.h

    src/apiact/texture.cc src/apiact/texture.h
    src/apiact/shader.cc src/apiact/shader.h
    src/apiact/spritebatch.cc src/apiact/spritebatch.h
    src/apiact/vertex_layout.cc src/apiact/vertex_layout.h
    src/apiact/uniform.cc src/apiact/uniform.h
    src/apiact/dependency_opengl.cc src/apiact/dependency_opengl.h

    src/apiact/pch.public.h
    src/apiact/pch.private.h
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
    PUBLIC src/apiact/pch.public.h
    PRIVATE src/apiact/pch.private.h
)
add_library(ride::apigl ALIAS apigl)

