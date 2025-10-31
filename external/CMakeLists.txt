


###################################################################################################
## cpptoml

add_library(cpptoml INTERFACE)
add_library(external::cpptoml ALIAS cpptoml)
target_include_directories(cpptoml
	INTERFACE
		${CMAKE_CURRENT_SOURCE_DIR}/cpptoml/include
)

###################################################################################################
## wxWidgets
if(WIN32)
       include(FetchContent)
       set(wxBUILD_SHARED OFF CACHE BOOL "Build shared libraries" FORCE)
       set(wxBUILD_STATIC ON CACHE BOOL "Build static libraries" FORCE)
       FetchContent_Declare(
	       wxWidgets
	       URL https://github.com/wxWidgets/wxWidgets/releases/download/v3.2.4/wxWidgets-3.2.4.tar.bz2
	       DOWNLOAD_EXTRACT_TIMESTAMP true
       )
       FetchContent_MakeAvailable(wxWidgets)

       set(wxWidgets_ROOT_DIR ${wxWidgets_SOURCE_DIR})
       set(wxWidgets_LIB_DIR ${wxWidgets_BINARY_DIR}/lib)
       set(wxWidgets_INCLUDE_DIRS ${wxWidgets_SOURCE_DIR}/include;${wxWidgets_BINARY_DIR}/lib/vc_x64_lib/mswu)
       set(wxWidgets_USE_STATIC ON)

       add_library(wxw INTERFACE)
       add_library(external::wxWidgets ALIAS wxw)
       target_link_libraries(wxw
	       INTERFACE
		       wx::core
		       wx::aui
		       wx::base
		       wx::stc
		       wx::adv
		       wx::html
       )
else()
	SET(RIDE_WX_COMPONENTS core aui base stc adv html)

	if(${MSVC})
		# on windows and visual-studio it requires scintilla
		LIST(APPEND RIDE_WX_COMPONENTS scintilla)
	endif()

	if(${RIDE_USE_WEBVIEW})
		# only try to find webview if it's required
		LIST(APPEND RIDE_WX_COMPONENTS webview)
	endif()

	message(STATUS "wxWidgets components: ${RIDE_WX_COMPONENTS}")
	set(wxWidgets_USE_STATIC ON)
	find_package(wxWidgets COMPONENTS ${RIDE_WX_COMPONENTS} REQUIRED)

	# include( "${wxWidgets_USE_FILE}" )
	# message(STATUS "wxWidgets use file: ${wxWidgets_USE_FILE}")

	add_library(wxw INTERFACE)
	add_library(external::wxWidgets ALIAS wxw)
	target_include_directories(wxw
		INTERFACE
			${wxWidgets_INCLUDE_DIRS}
	)
	target_link_libraries(wxw
		INTERFACE
			${wxWidgets_LIBRARIES}
	)
	target_compile_definitions(wxw
		INTERFACE
			${wxWidgets_DEFINITIONS}
	)
endif()

# include?
# wxWidgets_DEFINITIONS_DEBUG


###################################################################################################
## tinyxml
add_library(tinyxml STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/tinyxml2/tinyxml2.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/tinyxml2/tinyxml2.h
)
add_library(external::tinyxml ALIAS tinyxml)
target_include_directories(tinyxml
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/tinyxml2/
)
target_link_libraries(tinyxml
	PRIVATE
		common::options
)



###################################################################################################
## nlohmann json

include(FetchContent)

FetchContent_Declare(json
	URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
	DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(json)

add_library(json INTERFACE)
add_library(external::json ALIAS json)
target_link_libraries(json
	INTERFACE
		nlohmann_json::nlohmann_json
)


###################################################################################################
## 
