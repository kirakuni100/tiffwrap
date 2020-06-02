/*
 * MIT License
 *
 * Copyright (c) 2020 kirakuni100
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef INCLUDE_TIFFWRAP_UTIL_H
#define INCLUDE_TIFFWRAP_UTIL_H

#include <tuple>
#include <vector>
#include "tiffwrap.h"

namespace tiffwrap {

size_t get_packed_yuv_size(const TiffWrap &tif);

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

#endif  // INCLUDE_TIFFWRAP_UTIL_H
