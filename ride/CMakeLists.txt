set(src
    driver.cc driver.h
    str.cc str.h
)

add_library(libride STATIC ${src})
target_link_libraries(libride
    project_options project_warnings
)
