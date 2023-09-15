cmake_minimum_required(VERSION 3.16)

project(ct)

add_subdirectory("third-party")
add_subdirectory("src")

if (${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
	enable_testing()
	add_subdirectory ("test")

	add_subdirectory ("examples")
endif()
