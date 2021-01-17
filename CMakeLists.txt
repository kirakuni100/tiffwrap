cmake_minimum_required(VERSION 3.8)

project(
    tiffwrap
    VERSION 1.0.0
    DESCRIPTION "Simple C++ Wrapper of libtiff"
    LANGUAGES CXX
)

set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

set(TIFFWRAP_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(TIFFWRAP_INC_DIR ${PROJECT_SOURCE_DIR}/include)
set(TIFFWRAP_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(TIFFWRAP_CFG_DIR ${PROJECT_SOURCE_DIR}/config)

option(TIFFWRAP_BUILD_WITH_SANITIZER "Build tiffwrap with sanitizer" OFF)
if(TIFFWRAP_BUILD_WITH_SANITIZER)
    message(STATUS "Build tiffwrap with sanitizer [${CMAKE_CXX_COMPILER_ID}]")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "-g3 -O0 -fsanitize=address -fsanitize=undefined -fsanitize=integer -fsanitize=nullability -fno-omit-frame-pointer -fno-sanitize-recover=all -fsanitize-recover=unsigned-integer-overflow")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "-g3 -O0 -fsanitize=address -fsanitize=undefined")
    endif()
endif()

option(TIFFWRAP_BUILD_WITH_COVINFO "Build tiffwrap with coverage information" OFF)
if(TIFFWRAP_BUILD_WITH_COVINFO)
    message(STATUS "Build tiffwrap with coverage information [${CMAKE_CXX_COMPILER_ID}]")
    set(CMAKE_CXX_FLAGS "--coverage")
    set(CMAKE_EXE_LINKER_FLAGS "--coverage")

    file(GLOB_RECURSE SRCS ${TIFFWRAP_SRC_DIR}/*)
    add_custom_target(lcov_html
        COMMAND lcov --directory . --capture --output-file cov.info --rc lcov_branch_coverage=1
        COMMAND lcov -e cov.info ${SRCS} --output-file cov.info.filtered --rc lcov_branch_coverage=1
        COMMAND genhtml --title "tiffwrap" --legend --demangle-cpp --output-directory html -show-details --branch-coverage cov.info.filtered
        COMMENT "Generating HTML report html/index.html"
    )
endif()

option(TIFFWRAP_BUILD_SHARED_LIBS "Build tiffwrap as a shared library" ON)
option(TIFFWRAP_BUILD_UNIT_TESTS "Build unit tests of tiffwrap" OFF)

if(TIFFWRAP_BUILD_SHARED_LIBS)
    add_library(tiffwrap SHARED)
else()
    add_library(tiffwrap STATIC)
endif()

add_library(tiffwrap::tiffwrap ALIAS tiffwrap)

set_target_properties(
    tiffwrap PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    POSITION_INDEPENDENT_CODE ON
)

target_compile_features(
    tiffwrap PRIVATE
    cxx_std_11
)

target_compile_options(
    tiffwrap PRIVATE
    $<$<CXX_COMPILER_ID:Clang>:
    -Werror -Wall -Weverything
    -Wno-padded
    -Wno-c++98-compat -Wno-c++98-compat-pedantic>
    $<$<CXX_COMPILER_ID:GNU>:
    -Werror -Wall -Wextra -pedantic
    -Wcast-align -Wcast-qual -Wconversion
    -Wdisabled-optimization -Wendif-labels -Wfloat-equal
    -Winit-self -Winline -Wlogical-op -Wmissing-include-dirs
    -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual
    -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow
    -Wswitch-default -Wswitch-enum -Wunsafe-loop-optimizations
    -Wvariadic-macros -Wwrite-strings -Wno-missing-field-initializers>
    $<$<CONFIG:Release>: -O2>
    $<$<CONFIG:Debug>: -O0 -g3>
    $<$<CONFIG:RelWithDebInfo>: -O2 -g3>
)

target_compile_definitions(
    tiffwrap PRIVATE
    $<$<NOT:$<CONFIG:Debug>>:NDEBUG>
)

configure_file(
    ${TIFFWRAP_CFG_DIR}/tiffwrap_config.h.in
    ${TIFFWRAP_INC_DIR}/tiffwrap_config.h
)

target_include_directories(
    tiffwrap PRIVATE
    $<BUILD_INTERFACE:${TIFFWRAP_INC_DIR}>
    $<INSTALL_INTERFACE:include/tiffwrap>
)

add_subdirectory(src)
add_subdirectory(include)

include(CTest)
if(TIFFWRAP_BUILD_UNIT_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

install(
    TARGETS tiffwrap
    EXPORT tiffwrap-export
    ARCHIVE DESTINATION lib/tiffwrap
    LIBRARY DESTINATION lib/tiffwrap
    INCLUDES DESTINATION include/tiffwrap
    PUBLIC_HEADER DESTINATION include/tiffwrap
)

install(
    EXPORT tiffwrap-export
    FILE tiffwrap-config.cmake
    NAMESPACE tiffwrap::
    DESTINATION share/cmake/tiffwrap
    EXPORT_LINK_INTERFACE_LIBRARIES
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${PROJECT_BINARY_DIR}/tiffwrap-config-version.cmake
    COMPATIBILITY SameMajorVersion
    VERSION ${PROJECT_VERSION}
)

install(
    FILES
    ${PROJECT_BINARY_DIR}/tiffwrap-config-version.cmake
    DESTINATION share/cmake/tiffwrap
)

