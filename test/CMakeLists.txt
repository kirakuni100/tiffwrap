cmake_minimum_required(VERSION 3.8)

add_library(doctest_main OBJECT src/doctest_main.cc)

set_target_properties(
    doctest_main PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)

target_compile_features(
    doctest_main PRIVATE
    cxx_std_11
)

target_include_directories(
    doctest_main PRIVATE
    thirdparty/doctest
)

add_executable(sample_test $<TARGET_OBJECTS:doctest_main> src/sample_test.cc)

set_target_properties(
    sample_test PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)

target_compile_features(
    sample_test PRIVATE
    cxx_std_11
)

target_compile_options(
    sample_test PRIVATE
    -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wconversion
    -Wdisabled-optimization -Wendif-labels -Wfloat-equal
    -Winit-self -Winline -Wlogical-op -Wmissing-include-dirs
    -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual
    -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow
    -Wswitch-default -Wswitch-enum -Wunsafe-loop-optimizations
    -Wvariadic-macros -Wwrite-strings -Wno-missing-field-initializers
    $<$<CONFIG:Release>: -O2>
    $<$<CONFIG:Debug>: -O0 -g>
    $<$<CONFIG:RElWithDebgInfo>: -O2 -g>
)

target_compile_definitions(
    sample_test PRIVATE
    $<$<NOT:$<CONFIG:Debug>>:NDEBUG>
)

target_include_directories(
    sample_test PRIVATE
    ${TIFFWRAP_INC_DIR}
    thirdparty/doctest
)

add_test(
    NAME sample_test
    COMMAND bin/sample_test
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

set_tests_properties(sample_test PROPERTIES LABELS all)

