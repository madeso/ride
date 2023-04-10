set(headeronly_src
    src/core/color.open-color.h
    src/core/compiler_warning.h
    src/core/undef_windows.h
    src/core/units.h
    src/core/pch.public.h
    src/core/pch.private.h
)

set(file_src
    src/core/c.cc src/core/c.h
    src/core/cc.cc src/core/cc.h
    src/core/color.cc src/core/color.h
    src/core/cursor.cc src/core/cursor.h
    src/core/enumstringmap.cc src/core/enumstringmap.h
    src/core/file.cc src/core/file.h
    src/core/filesystem.cc src/core/filesystem.h
    src/core/hash.cc src/core/hash.h
    src/core/humane_sort.cc src/core/humane_sort.h
    src/core/ints.cc src/core/ints.h
    src/core/minmax.cc src/core/minmax.h
    src/core/number.cc src/core/number.h
    src/core/numeric.cc src/core/numeric.h
    src/core/rect.cc src/core/rect.h
    src/core/rng.cc src/core/rng.h
    src/core/side.cc src/core/side.h
    src/core/size.cc src/core/size.h
    src/core/string.cc src/core/string.h
    src/core/utf8.cc src/core/utf8.h
    src/core/vec2.cc src/core/vec2.h
)
set(src ${file_src} ${headeronly_src})
source_group("" FILES ${file_src})
source_group("Headers" FILES ${headeronly_src})
source_group_pch(core)
add_library(core STATIC ${src})
set_crt_to_static(core)
target_include_directories(core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(core
    PUBLIC
        embed::embed
        external::open_color
        ride::assert
        fmt::fmt
    PRIVATE
        ride::options
        ride::warnings
)
target_precompile_headers(core
    PUBLIC src/core/pch.public.h
    PRIVATE src/core/pch.private.h
)
set_target_properties(core PROPERTIES FOLDER "Libraries")
add_library(ride::core ALIAS core)





set(src_test
    src/core/string.test.cc
    src/core/cc.test.cc
    src/core/rect.test.cc
    src/core/vec2.test.cc
    ../../external/catch/main.cc
)
source_group("" FILES ${src_test})
add_executable(core_test ${src_test})
target_link_libraries(core_test
    PUBLIC
        catchy::catchy
        ride::core
    PRIVATE
        ride::options
        ride::warnings
)
catch_discover_tests(core_test)
set_target_properties(core_test PROPERTIES FOLDER "Tests")

