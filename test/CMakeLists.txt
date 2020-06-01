cmake_minimum_required(VERSION 3.8)

include(cmake/gtest.cmake.in)

find_package(TIFF REQUIRED)

set(tests
    src/sample_test.cc
    src/test_constructor.cc
)

option(TIFFWRAP_TEST_WITH_SANITIZER "Building test suite with sanitizer" OFF)
if(TIFFWRAP_TEST_WITH_SANITIZER)
    message(STATUS "Building test suite with sanitizer")
endif()

option(TIFFWRAP_TEST_WITH_VALGRIND "Execute test suite with Valgrind" OFF)
if(TIFFWRAP_TEST_WITH_VALGRIND)
    find_program(CMAKE_MEMORYCHECK_COMMAND valgrind)
    message(STATUS "Executing test suite with Valgrind")
    set(MEMCHECK_COMMAND "${CMAKE_MEMORYCHECK_COMMAND} ${CMAKE_MEMORYCHECK_COMMAND_OPTIONS} --error-exitcode=1 --leak-check=full")
    separate_arguments(MEMCHECK_COMMAND)
endif()

foreach(test ${tests})
    get_filename_component(testcase ${test} NAME_WE)
    add_executable(${testcase} ${test})

    target_link_libraries(
        ${testcase}
        tiffwrap::tiffwrap
        ${TIFF_LIBRARIES}
        gtest::gtest
        gtest::gtest_main
    )

    set_target_properties(
        ${testcase} PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
    )

    target_compile_features(
        ${testcase} PRIVATE
        cxx_std_11
    )

    target_compile_options(
        ${testcase} PRIVATE
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
        ${testcase} PRIVATE
        $<$<NOT:$<CONFIG:Debug>>:NDEBUG>
    )

    target_include_directories(
        ${testcase} PRIVATE
        ${TIFFWRAP_INC_DIR}
        ${GTEST_INC_DIR}
        ${GMOCK_INC_DIR}
    )

    add_test(
        NAME ${testcase}
        COMMAND $<TARGET_FILE:${testcase}>
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    )
    set_tests_properties(${testcase} PROPERTIES LABELS all)

    if(TIFFWRAP_TEST_WITH_VALGRIND)
        add_test(
            NAME ${testcase}_valgrind
            COMMAND ${MEMCHECK_COMMAND} $<TARGET_FILE:${testcase}>
            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        )
        set_tests_properties(${testcase}_valgrind PROPERTIES LABELS valgrind)
    endif()
endforeach()

