find_package(Filesystem REQUIRED)

set(src
    src/libride/command.cc src/libride/command.h
    src/libride/document.cc  src/libride/document.h
    src/libride/document.commands.cc  src/libride/document.commands.h
)
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
)

