cmake_minimum_required(VERSION 3.8)

project(
    tiffwrap-sample
    LANGUAGES CXX
)

find_package(TIFF REQUIRED)
find_package(tiffwrap REQUIRED)

add_executable(
    sample
    ${PROJECT_SOURCE_DIR}/src/sample.cc
)

target_link_libraries(sample tiffwrap::tiffwrap ${TIFF_LIBRARIES})

