
find_package (junior)


add_executable (drawing WIN32 "drawing/drawing.cpp")
target_link_libraries (drawing PRIVATE junior)
if (MSVC)
	target_link_options (drawing PRIVATE /ENTRY:mainCRTStartup)
endif()


add_executable (multi-window WIN32 "multi-window/multi-window.cpp")
target_link_libraries (multi-window PRIVATE junior)
if (MSVC)
	target_link_options (multi-window PRIVATE /ENTRY:mainCRTStartup)
endif()


add_executable (simple "simple/simple.cpp")
target_link_libraries (simple PRIVATE junior)
