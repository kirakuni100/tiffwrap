cmake_minimum_required(VERSION 3.8)

target_sources(
    tiffwrap PRIVATE
    ${TIFFWRAP_SRC_DIR}/tiffwrap.cc
    ${TIFFWRAP_SRC_DIR}/tiffwrap_util.cc
)
