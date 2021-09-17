embed_name(logo/256text.png logo_256text_png)
embed(files
    AS_BINARY
        logo/256text.png
)

message(STATUS "Files are ${files}")
add_library(data_logo STATIC empty.cc ${files})
target_include_directories(data_logo
    PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(data_logo
    PUBLIC
        embed
)
