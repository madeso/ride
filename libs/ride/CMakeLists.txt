find_package(Filesystem REQUIRED)

set(src
    src/ride/app.cc src/ride/app.h
    src/ride/command.cc src/ride/command.h
    src/ride/document.cc  src/ride/document.h
    src/ride/document.commands.cc  src/ride/document.commands.h
    src/ride/keybind.cc src/ride/keybind.h
    src/ride/theme.cc src/ride/theme.h
    src/ride/view.cc src/ride/view.h
    src/ride/view.document.cc src/ride/view.document.h
    src/ride/view.filesystem.cc src/ride/view.filesystem.h

    src/ride/pch.public.h
    src/ride/pch.private.h
)
source_group("" FILES ${src})

add_library(libride STATIC ${src})

target_include_directories(libride
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(libride
    PUBLIC
        ride::core
        ride::api
        fmt::fmt

    PRIVATE
        data::data_logo
        data::data_font
        ride::options
        ride::warnings
)
set_crt_to_static(libride)
set_target_properties(libride PROPERTIES FOLDER "Libraries")
target_precompile_headers(libride
    PUBLIC src/ride/pch.public.h
    PRIVATE src/ride/pch.private.h
)
add_library(ride::libride ALIAS libride)



set(src_test
    src/ride/document.test.cc
    ../../external/catch/main.cc
)
source_group("" FILES ${src_test})
add_executable(libride_test ${src_test})
target_link_libraries(libride_test
    PUBLIC
        catchy::catchy
        ride::libride
    PRIVATE
        ride::options
        ride::warnings
        fmt::fmt
)
catch_discover_tests(libride_test)
set_target_properties(libride_test PROPERTIES FOLDER "Tests")

