set(src
    src/ride/keybind.cc src/ride/keybind.h
    src/ride/theme.cc src/ride/theme.h
    src/ride/view.cc src/ride/view.h
)

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
    PRIVATE
        project_options
        project_warnings
)

set_crt_to_static(rideapp)
