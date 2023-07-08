
find_package (junior)

add_executable (drawing WIN32 "drawing/drawing.cpp")
target_link_libraries (drawing PRIVATE junior)

add_executable (multi-window WIN32 "multi-window/multi-window.cpp")
target_link_libraries (multi-window PRIVATE junior)

add_executable (simple "simple/simple.cpp")
target_link_libraries (simple PRIVATE junior)
