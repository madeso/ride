set(src
    src/ride/app.cc src/ride/app.h
    src/ride/keybind.cc src/ride/keybind.h
    src/ride/theme.cc src/ride/theme.h
    src/ride/view.cc src/ride/view.h
    src/ride/view.document.cc src/ride/view.document.h
    src/ride/view.filesystem.cc src/ride/view.filesystem.h
)
source_group("" FILES ${src})

add_library(rideapp STATIC ${src})

target_include_directories(rideapp
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(rideapp
    PUBLIC
        base
        api
        libride
        fmt::fmt
        
    PRIVATE
        data_logo
        data_font
        project_options
        project_warnings
)
set_crt_to_static(rideapp)
set_target_properties(rideapp PROPERTIES FOLDER "Libraries")
