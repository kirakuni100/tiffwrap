cmake_minimum_required(VERSION 3.8)

macro(target_public_headers TARGET)
    set_target_properties(${TARGET} PROPERTIES PUBLIC_HEADER "${ARGN}")
endmacro()

target_public_headers(
    tiffwrap
    ${TIFFWRAP_INC_DIR}/tiffwrap.h
    ${TIFFWRAP_INC_DIR}/tiffwrap_util.h
    ${TIFFWRAP_INC_DIR}/tiffwrap_config.h
)

