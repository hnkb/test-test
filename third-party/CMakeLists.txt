
include (FetchContent)

macro (FetchContent_DownloadOnly NAME)
	FetchContent_GetProperties (${NAME})
	if (NOT ${NAME}_POPULATED)
		FetchContent_Populate (${NAME})
	endif()
endmacro()


if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

	# skip test-only dependencies when not needed

	if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doctest")
		message ("Downloading doctest library from GitHub...")
		FetchContent_Declare (doctest
			SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/doctest"
			GIT_REPOSITORY "https://github.com/onqtam/doctest.git")
		FetchContent_DownloadOnly (doctest)
	endif()

	add_library (doctest INTERFACE)
	target_include_directories (doctest INTERFACE "doctest")

endif()
