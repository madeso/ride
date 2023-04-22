set(src
    src/backend/app.cc src/backend/app.h
    src/backend/main.cc src/backend/main.h
    
    src/backend/rencache.cc src/backend/rencache.h
    src/backend/renderer.cc src/backend/renderer.h
    
    src/backend/pch.public.h
    src/backend/pch.private.h
)
source_group("" FILES ${src})
add_library(beckend_blit STATIC ${src})
target_include_directories(beckend_blit
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
set_crt_to_static(beckend_blit)
source_group_pch(beckend_blit)
target_link_libraries(beckend_blit
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
set_target_properties(beckend_blit PROPERTIES FOLDER "Libraries/Backends")
target_precompile_headers(beckend_blit
    PUBLIC src/backend/pch.public.h
    PRIVATE src/backend/pch.private.h
)
add_library(ride::backend_blit ALIAS beckend_blit)

