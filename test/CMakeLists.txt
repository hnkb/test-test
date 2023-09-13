

function (SET_COMPILER_FLAGS TARGET_NAME)
	if (MSVC)
		target_compile_options (${TARGET_NAME} PRIVATE /permissive- /W4 /WX /utf-8)
		if (NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
			target_compile_options(${TARGET_NAME} PRIVATE /Qpar)
		endif()
	else()
		target_compile_options (${TARGET_NAME} PRIVATE -Wall -Wextra -pedantic
#			-Werror
			#-Wno-class-conversion
		)
		if (WIN32)
			target_link_libraries (${TARGET_NAME} PRIVATE -static)
		endif()
	endif()
endfunction()


function (SETUP_CRIB_TEST TARGET_NAME)

	add_executable (${TARGET_NAME}
		"unit/Math/Vec_int.cpp"
#		"unit/Graphics/Camera.cpp"
#		"unit/File.cpp"
        "unit/main.cpp"
    )

	set_compiler_flags (${TARGET_NAME})

	target_link_libraries (${TARGET_NAME}
		PRIVATE
			clib
			doctest)

	add_test (
		NAME ${TARGET_NAME}
		COMMAND ${TARGET_NAME}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

endfunction()

setup_crib_test ("crib-test-cpp17")

setup_crib_test ("crib-test-cpp20")
target_compile_features ("crib-test-cpp20" PRIVATE cxx_std_20)



add_executable ("crib-benchmark"
	"benchmark/main.cpp")

set_compiler_flags ("crib-benchmark")

target_link_libraries ("crib-benchmark"
	PRIVATE
		clib)
