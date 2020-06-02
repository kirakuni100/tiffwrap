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
 *
 * This source code uses libtiff
 * see http://www.libtiff/org
 *
 * Copyright (c) 1988-1997 Sam Leffler
 * Copyright (c) 1991-1997 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Sam Leffler and Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Sam Leffler and Silicon Graphics.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 * IN NO EVENT SHALL SAM LEFFLER OR SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 */

#include <chrono>
#include "tiffwrap_config.h"
#include "tiffwrap.h"

namespace tiffwrap {

TiffWrap::TiffWrap() :
    m_tiff(nullptr),
    m_ftype(Ftype::SINGLE),
    m_xsize(0),
    m_ysize(0),
    m_depth(0),
    m_comps(0),
    m_color(Color::ERROR),
    m_plane(Plane::PACKED),
    m_subsampling(YUVSubSampling::YUV444),
    m_field({ 0, 0, 0, 0, 0, 0, 1, { 1, 1 }, 0 })
{}

TiffWrap::~TiffWrap()
{
    this->close();
}

TiffWrap::TiffWrap(const TiffWrap &rhs) :
    m_ftype(rhs.m_ftype),
    m_xsize(rhs.m_xsize),
    m_ysize(rhs.m_ysize),
    m_depth(rhs.m_depth),
    m_comps(rhs.m_comps),
    m_color(rhs.m_color),
    m_plane(rhs.m_plane),
    m_subsampling(rhs.m_subsampling),
    m_field(rhs.m_field)
{}

TiffWrap &TiffWrap::operator=(const TiffWrap &rhs)
{
    if (this != &rhs) {
        m_ftype = rhs.m_ftype;
        m_xsize = rhs.m_xsize;
        m_ysize = rhs.m_ysize;
        m_depth = rhs.m_depth;
        m_comps = rhs.m_comps;
        m_color = rhs.m_color;
        m_plane = rhs.m_plane;
        m_subsampling = rhs.m_subsampling;
        m_field = rhs.m_field;
    }
    return *this;
}

TiffWrap::TiffWrap(TiffWrap &&rhs) noexcept :
    m_ftype(rhs.m_ftype),
    m_xsize(rhs.m_xsize),
    m_ysize(rhs.m_ysize),
    m_depth(rhs.m_depth),
    m_comps(rhs.m_comps),
    m_color(rhs.m_color),
    m_plane(rhs.m_plane),
    m_subsampling(rhs.m_subsampling),
    m_field(rhs.m_field)
{
    this->close();
    m_tiff = rhs.m_tiff;
    rhs.m_tiff = nullptr;
}

TiffWrap &TiffWrap::operator=(TiffWrap &&rhs) noexcept
{
    if (this != &rhs) {
        this->close();
        m_tiff = rhs.m_tiff;
        rhs.m_tiff = nullptr;
        m_ftype = rhs.m_ftype;
        m_xsize = rhs.m_xsize;
        m_ysize = rhs.m_ysize;
        m_depth = rhs.m_depth;
        m_comps = rhs.m_comps;
        m_color = rhs.m_color;
        m_plane = rhs.m_plane;
        m_subsampling = rhs.m_subsampling;
        m_field = rhs.m_field;
    }
    return *this;
}

int TiffWrap::wopen(const std::string &name)
{
    if (m_tiff == nullptr) {
        return ((m_tiff = TIFFOpen(name.c_str(), "w")) == nullptr) ? -1 : 0;
    } else {
        return -1;
    }
}

int TiffWrap::ropen(const std::string &name)
{
    if (m_tiff == nullptr) {
        return ((m_tiff = TIFFOpen(name.c_str(), "r")) == nullptr) ? -1 : 0;
    } else {
        return -1;
    }
}

void TiffWrap::close()
{
    if (m_tiff) {
        TIFFClose(m_tiff);
        m_tiff = nullptr;
    }
}

int TiffWrap::set_tags()
{
    auto ret = check_params();

    if (ret == 0) {
        if (!TIFFSetField(m_tiff, TIFFTAG_SUBFILETYPE, m_field.subfiletype)) {
            ret = -1;
        }
    }

    if (ret == 0) {
        if (!TIFFSetField(m_tiff, TIFFTAG_IMAGEWIDTH, m_field.image_width)) {
            ret = -1;
        }
    }

    if (ret == 0) {
        if (!TIFFSetField(m_tiff, TIFFTAG_IMAGELENGTH, m_field.image_length)) {
            ret = -1;
        }
    }

    if (ret == 0) {
        if (!TIFFSetField(m_tiff, TIFFTAG_BITSPERSAMPLE, m_field.bits_per_sample)) {
            ret = -1;
        }
    }

    if (ret == 0) {
        if (!TIFFSetField(m_tiff, TIFFTAG_SAMPLESPERPIXEL, m_field.samples_per_pixel)) {
            ret = -1;
        }
    }

    if (ret == 0) {
        if (!TIFFSetField(m_tiff, TIFFTAG_PHOTOMETRIC, m_field.photometric)) {
            ret = -1;
        }
    }

    if (ret == 0) {
        if (!TIFFSetField(m_tiff, TIFFTAG_PLANARCONFIG, m_field.planar_config)) {
            ret = -1;
        }
    }

    if (ret == 0 && m_color == Color::YUV) {
        if (!TIFFSetField(m_tiff, TIFFTAG_YCBCRSUBSAMPLING, m_field.subsampling[0], m_field.subsampling[1])) {
            ret = -1;
        }
    }

    if (ret == 0) {
        if (!TIFFSetField(m_tiff, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT)) {
            ret = -1;
        }
    }

    if (ret == 0) {
        if (!TIFFSetField(m_tiff, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB)) {
            ret = -1;
        }
    }

    if (ret == 0) {
        if (!TIFFSetField(m_tiff, TIFFTAG_COMPRESSION, COMPRESSION_NONE)) {
            ret = -1;
        }
    }

    if (ret == 0) {
        if (!TIFFSetField(m_tiff, TIFFTAG_SOFTWARE, TIFFWRAP_SOFTWARE)) {
            ret = -1;
        }
    }

    if (ret == 0) {
        char buf[20] = { 0 };
        auto now = std::chrono::system_clock::now();
        auto tm = std::chrono::system_clock::to_time_t(now);
        std::strftime(buf, sizeof(buf), "%Y:%m:%d %H:%M:%S", std::localtime(&tm));

        if (!TIFFSetField(m_tiff, TIFFTAG_DATETIME, buf)) {
            ret = -1;
        }
    }

    if (ret == 0) {
        if (m_subsampling == YUVSubSampling::YUV420) {
            m_field.rows_per_strip = 16;
        } else if (m_subsampling == YUVSubSampling::YUV410) {
            m_field.rows_per_strip = 32;
        } else {
            m_field.rows_per_strip = TIFFDefaultStripSize(m_tiff, 0);
        }

        if (!TIFFSetField(m_tiff, TIFFTAG_ROWSPERSTRIP, m_field.rows_per_strip)) {
            ret = -1;
        }
    }

    return ret;
}

int TiffWrap::get_tags()
{
    auto ret = m_tiff == nullptr ? -1 : 0;

    if (ret == 0) {
        if (!TIFFGetField(m_tiff, TIFFTAG_SUBFILETYPE, &(m_field.subfiletype))) {
            m_field.subfiletype = 0;
        }
        m_ftype = m_field.subfiletype == 0             ? Ftype::SINGLE :
                  m_field.subfiletype == FILETYPE_PAGE ? Ftype::MULTI  :
                                                         Ftype::ERROR  ;
    }

    if (ret == 0) {
        if (!TIFFGetField(m_tiff, TIFFTAG_IMAGEWIDTH, &(m_field.image_width))) {
            ret = -1;
        }
        m_xsize = m_field.image_width;
    }

    if (ret == 0) {
        if (!TIFFGetField(m_tiff, TIFFTAG_IMAGELENGTH, &(m_field.image_length))) {
            ret = -1;
        }
        m_ysize = m_field.image_length;
    }

    if (ret == 0) {
        if (!TIFFGetField(m_tiff, TIFFTAG_BITSPERSAMPLE, &(m_field.bits_per_sample))) {
            ret = -1;
        }
        m_depth = m_field.bits_per_sample;
    }

    if (ret == 0) {
        if (!TIFFGetField(m_tiff, TIFFTAG_SAMPLESPERPIXEL, &(m_field.samples_per_pixel))) {
            ret = -1;
        }
        m_comps = m_field.samples_per_pixel;
    }

    if (ret == 0) {
        if (!TIFFGetField(m_tiff, TIFFTAG_PHOTOMETRIC, &(m_field.photometric))) {
            ret = -1;
        }
        m_color = m_field.photometric == PHOTOMETRIC_MINISBLACK ? Color::MONO :
                  m_field.photometric == PHOTOMETRIC_RGB        ? Color::RGB  :
                  m_field.photometric == PHOTOMETRIC_SEPARATED  ? Color::CMYK :
                  m_field.photometric == PHOTOMETRIC_YCBCR      ? Color::YUV  :
                                                                  Color::ERROR;
    }

    if (ret == 0) {
        if (!TIFFGetField(m_tiff, TIFFTAG_PLANARCONFIG, &(m_field.planar_config))) {
            ret = -1;
        }
        m_plane = m_field.planar_config == PLANARCONFIG_CONTIG   ? Plane::PACKED :
                  m_field.planar_config == PLANARCONFIG_SEPARATE ? Plane::PLANAR :
                                                                   Plane::ERROR  ;
    }

    if (ret == 0 && m_color == Color::YUV) {
        if (!TIFFGetField(m_tiff, TIFFTAG_YCBCRSUBSAMPLING, &(m_field.subsampling[0]), &(m_field.subsampling[1]))) {
            ret = -1;
        }
        m_subsampling = m_field.subsampling[0] == 1 &&
                        m_field.subsampling[1] == 1 ? YUVSubSampling::YUV444 :
                        m_field.subsampling[0] == 2 &&
                        m_field.subsampling[1] == 1 ? YUVSubSampling::YUV422 :
                        m_field.subsampling[0] == 2 &&
                        m_field.subsampling[1] == 2 ? YUVSubSampling::YUV420 :
                        m_field.subsampling[0] == 4 &&
                        m_field.subsampling[1] == 1 ? YUVSubSampling::YUV411 :
                        m_field.subsampling[0] == 4 &&
                        m_field.subsampling[1] == 4 ? YUVSubSampling::YUV410 :
                                                      YUVSubSampling::ERROR  ;
    }

    if (!TIFFGetField(m_tiff, TIFFTAG_ROWSPERSTRIP, &(m_field.rows_per_strip))) {
        ret = -1;
    }

    if (ret == 0) {
        ret = check_params();
    }

    return ret;
}

template<typename T>
int TiffWrap::write(T *p_data, uint16_t comp)
{
    auto ret = m_tiff == nullptr ? -1 : 0;
    auto rows_per_strip = 0u;

    if (ret == 0) {
        if (!TIFFGetFieldDefaulted(m_tiff, TIFFTAG_ROWSPERSTRIP, &rows_per_strip)) {
            ret = -1;
        }
    }

    if (ret == 0) {
        if (m_plane == Plane::PACKED) {
            const auto xfactor = m_field.subsampling[0];
            const auto yfactor = m_field.subsampling[1];
            const auto xsize = (m_xsize + xfactor - 1) / xfactor * xfactor;
            const auto ysize = (m_ysize + yfactor - 1) / yfactor * yfactor;

            for (auto y = 0u, strip = 0u; y < ysize; y += rows_per_strip, ++strip) {
                const auto nrows = y + rows_per_strip > ysize ? ysize - y : rows_per_strip;
                const auto chroma_xsize = xsize / xfactor;
                const auto chroma_ysize = nrows / yfactor;
                const auto ssize = m_color == Color::YUV ?
                                nrows * xsize + chroma_ysize * chroma_xsize * 2:
                                nrows * xsize * m_comps;

                tmsize_t rsize = 0;
                if ((rsize = TIFFWriteEncodedStrip(m_tiff, strip, p_data, ssize)) < 0) {
                    ret = -1;
                    break;
                }
                ret += static_cast<int>(rsize);
                p_data += static_cast<size_t>(rsize) / sizeof(T);
            }
        } else {
            const auto xsize = m_xsize;
            const auto ysize = m_ysize;
            const auto strips_per_comp = TIFFNumberOfStrips(m_tiff) / m_comps;

            for (auto y = 0u, st = 0u; y < ysize; y += rows_per_strip, ++st) {
                const auto nrows = y + rows_per_strip > ysize ? ysize - y : rows_per_strip;
                const auto ssize = nrows * xsize;
                const auto strip = strips_per_comp * comp + st;

                tmsize_t rsize = 0;
                if ((rsize = TIFFWriteEncodedStrip(m_tiff, strip, p_data, ssize)) < 0) {
                    ret = -1;
                    break;
                }
                ret += static_cast<int>(rsize);
                p_data += static_cast<size_t>(rsize) / sizeof(T);
            }
        }
    }

    return ret;
}

template int TiffWrap::write<uint8_t>(uint8_t *p_data, uint16_t comp);
template int TiffWrap::write<uint16_t>(uint16_t *p_data, uint16_t comp);

template<typename T>
int TiffWrap::read(T *p_data, uint16_t comp)
{
    auto ret = m_tiff == nullptr ? -1 : 0;
    auto rows_per_strip = 0u;

    if (ret == 0) {
        if (!TIFFGetFieldDefaulted(m_tiff, TIFFTAG_ROWSPERSTRIP, &rows_per_strip)) {
            ret = -1;
        }
    }

    if (ret == 0) {
        if (m_plane == Plane::PACKED) {
            const auto xfactor = m_field.subsampling[0];
            const auto yfactor = m_field.subsampling[1];
            const auto xsize = (m_xsize + xfactor - 1) / xfactor * xfactor;
            const auto ysize = (m_ysize + yfactor - 1) / yfactor * yfactor;

            for (auto y = 0u, strip = 0u; y < ysize; y += rows_per_strip, ++strip) {
                const auto nrows = y + rows_per_strip > ysize ? ysize - y : rows_per_strip;
                const auto chroma_xsize = xsize / xfactor;
                const auto chroma_ysize = nrows / yfactor;
                const auto ssize = m_color == Color::YUV ?
                                nrows * xsize + chroma_ysize * chroma_xsize * 2:
                                nrows * xsize * m_comps;

                tmsize_t rsize = 0;
                if ((rsize = TIFFReadEncodedStrip(m_tiff, strip, p_data, ssize)) < 0) {
                    ret = -1;
                    break;
                }
                ret += static_cast<int>(rsize);
                p_data += static_cast<size_t>(rsize) / sizeof(T);
            }
        } else {
            const auto xsize = m_xsize;
            const auto ysize = m_ysize;
            const auto strips_per_comp = TIFFNumberOfStrips(m_tiff) / m_comps;

            for (auto y = 0u, st = 0u; y < ysize; y += rows_per_strip, ++st) {
                const auto nrows = y + rows_per_strip > ysize ? ysize - y : rows_per_strip;
                const auto ssize = nrows * xsize;
                const auto strip = strips_per_comp * comp + st;

                tmsize_t rsize = 0;
                if ((rsize = TIFFReadEncodedStrip(m_tiff, strip, p_data, ssize)) < 0) {
                    ret = -1;
                    break;
                }
                ret += static_cast<int>(rsize);
                p_data += static_cast<size_t>(rsize) / sizeof(T);
            }
        }
    }

    return ret;
}

template int TiffWrap::read<uint8_t>(uint8_t *p_data, uint16_t comp);
template int TiffWrap::read<uint16_t>(uint16_t *p_data, uint16_t comp);

int TiffWrap::check_params()
{
    auto ret = m_tiff == nullptr ? -1 : 0;

    if (ret == 0) {
        ret = m_ftype == Ftype::ERROR ? -1 : 0;
    }

    if (ret == 0) {
        ret = m_xsize == 0 || m_ysize == 0 ? -1 : 0;
    }

    if (ret == 0) {
        ret = m_depth == 0 || m_depth > 16 ? -1 : 0;
    }

    if (ret == 0) {
        ret = m_comps == 0 ? -1 : 0;
    }

    if (ret == 0) {
        ret = m_color == Color::ERROR ? -1 : 0;
    }

    if (ret == 0) {
        ret = m_plane == Plane::ERROR ? -1 : 0;
    }

    if (ret == 0 && m_color == Color::YUV) {
        ret = m_subsampling == YUVSubSampling::ERROR ? -1 : 0;
    }

    return ret;
}

}  // namespace tiffwrap
