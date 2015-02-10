if(${MSVC})
	# C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include\xutility(2132): warning C4996: 'std::_Copy_impl': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS.
	add_definitions("-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS")
	message(STATUS "Adding _SCL_SECURE_NO_WARNINGS and _CRT_SECURE_NO_WARNINGS")
endif()

find_package(wxWidgets COMPONENTS core aui base stc REQUIRED)
include( "${wxWidgets_USE_FILE}" )
message(status "wxWidgets use file: ${wxWidgets_USE_FILE}")

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

FILE(GLOB src_glob *.cpp;*.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
PROTOBUF_GENERATE_CPP(SETTINGS_PROTO_SRCS SETTINGS_PROTO_HDRS settings.proto)

set(src settings.proto ${src_glob} ${SETTINGS_PROTO_SRCS} ${SETTINGS_PROTO_HDRS})

source_group("" FILES ${src})
source_group("generated" FILES ${SETTINGS_PROTO_SRCS} ${SETTINGS_PROTO_HDRS})

add_executable(
	${PROJECT_NAME} WIN32 
	${src}
)

target_link_libraries(
	${PROJECT_NAME}
	${wxWidgets_LIBRARIES}
	${PROTOBUF_LIBRARIES}
)
