embed(files
    AS_BINARY
        Inconsolata-Medium.ttf
)

add_library(data_font STATIC ../empty.cc ${files})
target_include_directories(data_font
    PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(data_font
    PUBLIC
        embed
)
set_target_properties(data_font PROPERTIES FOLDER "Embedded files")
add_library(data::data_font ALIAS data_font)

