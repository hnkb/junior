
cmake_minimum_required (VERSION 3.16)

project ("junior"
	VERSION "0.0.1"
	DESCRIPTION "junior: An easy to use GUI library for starting C/C++ programmers"
	HOMEPAGE_URL "https://github.com/hnkb/junior")

if (TARGET "junior")
	return()
endif()

add_subdirectory ("src")

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	add_subdirectory ("examples")
endif()
