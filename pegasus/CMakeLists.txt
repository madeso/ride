find_package(wxWidgets COMPONENTS core base stc REQUIRED)
 
include( "${wxWidgets_USE_FILE}" )
 
add_executable(
	${PROJECT_NAME} WIN32 
	main.cpp
)
 
target_link_libraries(
	${PROJECT_NAME}
	${wxWidgets_LIBRARIES}
)
