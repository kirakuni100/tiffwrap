#ifndef _TIFFWRAP_UTIL_H_
#define _TIFFWRAP_UTIL_H_

#include <tuple>
#include <vector>
#include "tiffwrap.h"

namespace tiffwrap {

size_t get_yuv_size(const TiffWrap &tif);

template<typename T>
std::vector<T> pack_yuv(
    const T *im_y,
    const T *im_u,
    const T *im_v,
    const TiffWrap &tif);

template<typename T>
std::tuple<std::vector<T>, std::vector<T>, std::vector<T>> split_packed_yuv(
    const T *im_yuv,
    const TiffWrap &tif);

}  // namespace tiffwrap

#endif  // _TIFFWRAP_UTIL_H_
