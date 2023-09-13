
add_library(clib
    "test.cpp")

target_include_directories(clib PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_compile_features(clib PUBLIC cxx_std_17)
