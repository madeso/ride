set(src
    src/assert/assert.cc src/assert/assert.h
)
source_group("" FILES ${src})
add_library(assert STATIC ${src})
set_crt_to_static(assert)
target_include_directories(assert
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(assert
    PRIVATE
        project_options
        project_warnings
)
set_target_properties(assert PROPERTIES FOLDER "Libraries")
