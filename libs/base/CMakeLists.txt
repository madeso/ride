set(src
    src/base/assert.cc src/base/assert.h
    src/base/c.cc src/base/c.h
    src/base/cc.cc src/base/cc.h
    src/base/color.cc src/base/color.h
    src/base/enumstringmap.cc src/base/enumstringmap.h
    src/base/file.cc src/base/file.h
    src/base/filesystem.cc src/base/filesystem.h
    src/base/humane_sort.cc src/base/humane_sort.h
    src/base/ints.cc src/base/ints.h
    src/base/numeric.cc src/base/numeric.h
    src/base/rect.cc src/base/rect.h
    src/base/size.cc src/base/size.h
    src/base/str.cc src/base/str.h
    src/base/string.cc src/base/string.h
    src/base/utf8.cc src/base/utf8.h
    src/base/vec2.cc src/base/vec2.h
)
add_library(base STATIC ${src})
target_include_directories(base
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
set_crt_to_static(base)
target_link_libraries(base
    PUBLIC
        embed
    PRIVATE
        project_options
)
