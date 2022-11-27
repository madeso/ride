add_subdirectory(apigl)

if(ride_wx)
    add_subdirectory(apiwx)
endif()



set(example_squares_src
	examples/squares.cc
)
source_group("" FILES ${example_squares_src})
add_executable(example_squares WIN32 MACOSX_BUNDLE ${example_squares_src})
set_crt_to_static(example_squares)
target_link_libraries(example_squares
    PRIVATE
        ride::options
        ride::warnings
        external::open_color
    PUBLIC
        ride::apigl
)
set_target_properties(example_squares PROPERTIES FOLDER "Examples")


set(example_images_src
	examples/images.cc
)
source_group("" FILES ${example_images_src})
add_executable(example_images WIN32 MACOSX_BUNDLE ${example_images_src})
set_crt_to_static(example_images)
target_link_libraries(example_images
    PRIVATE
        ride::options
        ride::warnings
    PUBLIC
        ride::apigl
        data::data_logo
)
set_target_properties(example_images PROPERTIES FOLDER "Examples")


set(example_font_src
	examples/font.cc
)
source_group("" FILES ${example_font_src})
add_executable(example_font WIN32 MACOSX_BUNDLE ${example_font_src})
set_crt_to_static(example_font)
target_link_libraries(example_font
    PRIVATE
        ride::options
        ride::warnings
    PUBLIC
        ride::apigl
)
set_target_properties(example_font PROPERTIES FOLDER "Examples")


set(example_scope_src
	examples/scope.cc
)
source_group("" FILES ${example_scope_src})
add_executable(example_scope WIN32 MACOSX_BUNDLE ${example_scope_src})
set_crt_to_static(example_scope)
target_link_libraries(example_scope
    PRIVATE
        ride::options
        ride::warnings
    PUBLIC
        ride::apigl
)
set_target_properties(example_scope PROPERTIES FOLDER "Examples")

