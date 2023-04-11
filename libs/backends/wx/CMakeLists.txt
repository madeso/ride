set(src
    src/backend/app.cc src/backend/app.h
    src/backend/font.cc src/backend/font.h
    src/backend/key.cc src/backend/key.h
    src/backend/main.cc src/backend/main.h
    src/backend/mouse_button.cc src/backend/mouse_button.h
    src/backend/renderer.cc src/backend/renderer.h
    src/backend/texture.cc src/backend/texture.h
    src/backend/dependency_wx.h

    src/backend/pch.public.h
    src/backend/pch.private.h
)
source_group("" FILES ${src})
add_library(backend_wx STATIC ${src})
target_include_directories(backend_wx
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
set_crt_to_static(backend_wx)
target_link_libraries(backend_wx
    PUBLIC
        external::wx
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
)
set_target_properties(backend_wx PROPERTIES FOLDER "Libraries/Backends")
target_precompile_headers(backend_wx
    PUBLIC src/backend/pch.public.h
    PRIVATE src/backend/pch.private.h
)
add_library(ride::backend_wx ALIAS wx)
