set(src
    src/backend/cursorcache.cc src/backend/cursorcache.h
    src/backend/key.cc src/backend/key.h
    src/backend/log.cc src/backend/log.h
    src/backend/mouse_button.cc src/backend/mouse_button.h
    src/backend/font.cc src/backend/font.h

    src/backend/pch.public.h
    src/backend/pch.private.h
)
source_group("" FILES ${src})
add_library(sdl_base STATIC ${src})
target_include_directories(sdl_base
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
set_crt_to_static(sdl_base)
source_group_pch(sdl_base)
target_link_libraries(sdl_base
    PUBLIC
        external::sdl2
        fmt::fmt
        ride::core
        ride::api
    PRIVATE
        ride::options
        ride::warnings
        external::stb_truetype
        data::data_font
)
set_target_properties(sdl_base PROPERTIES FOLDER "Libraries/Backends")
target_precompile_headers(sdl_base
    PUBLIC src/backend/pch.public.h
    PRIVATE src/backend/pch.private.h
)
add_library(ride::sdl_base ALIAS sdl_base)
