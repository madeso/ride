create_app(ride ""
    SOURCES main.cc
    LIBRARIES ride::libride
)



###############################################################################
############# todo(Gustav): fix installation
# install(TARGETS ride DESTINATION ".")
# 
# if(APPLE)
#     # install dependencies
#     install(SCRIPT macdylibbundler.cmake)
# 
#     # make apple installer look prettier
#     set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_ICON_FILE "application")
#     set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/bundle-info.plist")
# endif()
# 
# # Amend manifest to tell Windows that the application is DPI aware (needed for Windows 8.1 and up)
# IF (MSVC)
#     ADD_CUSTOM_COMMAND(
#         TARGET ride
#         POST_BUILD
#         COMMAND "mt.exe" -manifest \"${CMAKE_CURRENT_SOURCE_DIR}\\dpiaware.manifest.xml\" -inputresource:\"$<TARGET_FILE:${PROJECT_NAME}>\"\;\#1 -outputresource:\"$<TARGET_FILE:${PROJECT_NAME}>\"\;\#1
#         COMMENT "Adding display aware manifest..."
#     )
# ENDIF(MSVC)
# 
