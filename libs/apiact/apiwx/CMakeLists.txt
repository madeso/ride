set(src
    src/apiact/app.cc src/apiact/app.h
    src/apiact/font.cc src/apiact/font.h
    src/apiact/key.cc src/apiact/key.h
    src/apiact/main.cc src/apiact/main.h
    src/apiact/mouse_button.cc src/apiact/mouse_button.h
    src/apiact/renderer.cc src/apiact/renderer.h
    src/apiact/texture.cc src/apiact/texture.h
    src/apiact/dependency_wx.h

    src/apiact/pch.public.h
    src/apiact/pch.private.h
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
        ride::base
        ride::api
        fmt::fmt
    PRIVATE
        ride::options
        ride::warnings
        external::stb_truetype
        external::stb_image
        data::data_font
)
set_target_properties(apiwx PROPERTIES FOLDER "Libraries")
target_precompile_headers(apiwx
    PUBLIC src/apiact/pch.public.h
    PRIVATE src/apiact/pch.private.h
)
add_library(ride::apiwx ALIAS apiwx)
