
add_library (junior STATIC "")

target_include_directories (junior
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>)

target_sources (junior PRIVATE
	"${PROJECT_SOURCE_DIR}/include/junior.h"
	"${PROJECT_SOURCE_DIR}/include/junior-simple.h"
	"core/event.cpp"
	"core/window.cpp"
	"core/junior-simple.cpp"
	"win32/event_engine.cpp"
	"win32/window_engine.cpp"
)


set_target_properties (junior
	PROPERTIES
		DEBUG_POSTFIX "-Debug"
		OUTPUT_NAME
			"junior-${CMAKE_CXX_COMPILER_ID}$<IF:$<BOOL:MSVC>,-${MSVC_TOOLSET_VERSION},${CMAKE_CXX_COMPILER_VERSION}>")

install (
	TARGETS junior
	EXPORT junior-config
	ARCHIVE
		DESTINATION "lib/junior/static/$<$<EQUAL:8,${CMAKE_SIZEOF_VOID_P}>:x64>$<$<EQUAL:4,${CMAKE_SIZEOF_VOID_P}>:x86>")

install (
	DIRECTORY "${PROJECT_SOURCE_DIR}/include"
	DESTINATION "/")

install (
	EXPORT junior-config
	DESTINATION lib/cmake/junior)
