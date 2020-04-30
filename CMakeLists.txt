cmake_minimum_required(VERSION 3.8)

project(
    tiffwrap
    VERSION 1.0.0
    DESCRIPTION "libtiff Wrapper Library"
    LANGUAGES CXX)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)

if(NOT COMPILER_SUPPORTS_CXX11)
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no c++11 support.")
endif()

if(NOT COMPILER_SUPPORTS_CXX14)
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no c++14 support.")
else()
    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if(NOT COMPILER_SUPPORTS_CXX17)
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no c++17 support.")
else()
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wconversion -Wdisabled-optimization -Wendif-labels -Wfloat-equal -Winit-self -Winline -Wlogical-op -Wmissing-include-dirs -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wswitch-default -Wswitch-enum -Wunsafe-loop-optimizations -Wvariadic-macros -Wwrite-strings -Wno-missing-field-initializers -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

message(STATUS "System       : ${CMAKE_SYSTEM_NAME}")
message(STATUS "C++ Compiler : ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Flags    : ${CMAKE_CXX_FLAGS}")

find_package(TIFF REQUIRED)
include_directories(${TIFF_INCLUDE_DIRS})

set(SRC_DIR     ${CMAKE_SOURCE_DIR}/src)
set(INC_DIR     ${CMAKE_SOURCE_DIR}/include)
set(CONF_DIR    ${CMAKE_SOURCE_DIR}/config)
set(INSTALL_DIR ${CMAKE_BINARY_DIR}/tiffwrap)

include_directories(${INC_DIR})
configure_file(${CONF_DIR}/tiffwrap_config.h.in ${INC_DIR}/tiffwrap_config.h)

file(GLOB SRCS    ${SRC_DIR}/*.cc)
file(GLOB HEADERS ${INC_DIR}/*.h)

if(BUILD_SHARED_LIBS)
    add_library(tiffwrap SHARED ${SRCS})
    set_target_properties(tiffwrap PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})
    target_link_libraries(tiffwrap ${TIFF_LIBRARIES})
    install(TARGETS tiffwrap LIBRARY DESTINATION ${INSTALL_DIR}/lib)
else()
    add_library(tiffwrap STATIC ${SRCS})
    set_target_properties(tiffwrap PROPERTIES POSITION_INDEPENDENT_CODE ON)
    target_link_libraries(tiffwrap ${CMAKE_SOURCE_DIR}/lib/libtiff.a)
    install(TARGETS tiffwrap ARCHIVE DESTINATION ${INSTALL_DIR}/lib)
endif()
install(FILES ${HEADERS} DESTINATION ${INSTALL_DIR}/include)

add_executable(sample EXCLUDE_FROM_ALL ${CMAKE_SOURCE_DIR}/sample/sample.cc)
target_link_libraries(sample tiffwrap)
