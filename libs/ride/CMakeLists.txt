set(src
    src/ride/str.cc src/ride/str.h
)

find_package(Filesystem REQUIRED)


add_library(libride STATIC ${src})

target_include_directories(libride
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(libride
    PRIVATE
        project_options
        project_warnings
        std::filesystem
)

set_crt_to_static(libride)
