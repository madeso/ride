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
add_library(apiwx STATIC ${src})
target_include_directories(apiwx
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
set_crt_to_static(apiwx)
target_link_libraries(apiwx
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
set_target_properties(apiwx PROPERTIES FOLDER "Libraries/Backends")
target_precompile_headers(apiwx
    PUBLIC src/backend/pch.public.h
    PRIVATE src/backend/pch.private.h
)
add_library(ride::apiwx ALIAS apiwx)
