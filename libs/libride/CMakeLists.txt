find_package(Filesystem REQUIRED)

set(src
    src/libride/command.cc src/libride/command.h
    src/libride/document.cc  src/libride/document.h
    src/libride/document.commands.cc  src/libride/document.commands.h
)
source_group("" FILES ${src})
add_library(libride STATIC ${src})
set_crt_to_static(libride)
target_include_directories(libride
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(libride
    PUBLIC
        base
    PRIVATE
        project_options
        project_warnings
        std::filesystem
        fmt::fmt
)
set_target_properties(libride PROPERTIES FOLDER "Libraries")

set(src_test
    src/libride/document.test.cc
    ../../external/catch/main.cc
)
source_group("" FILES ${src_test})
add_executable(libride_test ${src_test})
target_link_libraries(libride_test
    PUBLIC
        catchy
        libride
    PRIVATE
        project_options
        project_warnings
        fmt::fmt
)
catch_discover_tests(libride_test)
set_target_properties(libride_test PROPERTIES FOLDER "Tests")
