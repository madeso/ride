set(src
    driver.cc driver.h
    str.cc str.h
    humane_sort.cc humane_sort.h
)

find_package(Filesystem REQUIRED)


add_library(libride STATIC ${src})
target_link_libraries(libride
    PRIVATE
    project_options
    project_warnings
    std::filesystem
)
