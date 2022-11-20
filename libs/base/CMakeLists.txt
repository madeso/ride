set(headeronly_src
    src/base/color.open-color.h
    src/base/compiler_warning.h
    src/base/undef_windows.h
    src/base/units.h
    src/base/pch.public.h
    src/base/pch.private.h
)

set(file_src    
    src/base/c.cc src/base/c.h
    src/base/cc.cc src/base/cc.h
    src/base/color.cc src/base/color.h
    src/base/cursor.cc src/base/cursor.h
    src/base/enumstringmap.cc src/base/enumstringmap.h
    src/base/file.cc src/base/file.h
    src/base/filesystem.cc src/base/filesystem.h
    src/base/hash.cc src/base/hash.h
    src/base/humane_sort.cc src/base/humane_sort.h
    src/base/ints.cc src/base/ints.h
    src/base/minmax.cc src/base/minmax.h
    src/base/number.cc src/base/number.h
    src/base/numeric.cc src/base/numeric.h
    src/base/rect.cc src/base/rect.h
    src/base/rng.cc src/base/rng.h
    src/base/side.cc src/base/side.h
    src/base/size.cc src/base/size.h
    src/base/string.cc src/base/string.h
    src/base/utf8.cc src/base/utf8.h
    src/base/vec2.cc src/base/vec2.h
)
set(src ${file_src} ${headeronly_src})
source_group("" FILES ${file_src})
source_group("Headers" FILES ${headeronly_src})
add_library(base STATIC ${src})
set_crt_to_static(base)
target_include_directories(base
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(base
    PUBLIC
        embed
        open_color
        assert
    PRIVATE
        project_options
        project_warnings
)
target_precompile_headers(base
    PUBLIC src/base/pch.public.h
    PRIVATE src/base/pch.private.h
)
set_target_properties(base PROPERTIES FOLDER "Libraries/base")

set(src_test
    src/base/string.test.cc
    src/base/cc.test.cc
    src/base/rect.test.cc
    src/base/vec2.test.cc
    ../../external/catch/main.cc
)
source_group("" FILES ${src_test})
add_executable(base_test ${src_test})
target_link_libraries(base_test
    PUBLIC
        catchy
        base
    PRIVATE
        project_options
        project_warnings
)
catch_discover_tests(base_test)
set_target_properties(base_test PROPERTIES FOLDER "Libraries/base")
