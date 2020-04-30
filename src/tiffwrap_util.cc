#include "tiffwrap_util.h"

namespace tiffwrap {

size_t get_yuv_size(const TiffWrap &tif)
{
    const auto xfactor = tif.get_chroma_sampling_xfactor();
    const auto yfactor = tif.get_chroma_sampling_yfactor();
    const auto xsize = (tif.get_xsize() + xfactor - 1) / xfactor * xfactor;
    const auto ysize = (tif.get_ysize() + yfactor - 1) / yfactor * yfactor;
    const auto chroma_xsize = xsize / xfactor;
    const auto chroma_ysize = ysize / yfactor;
    return xsize * ysize + chroma_xsize * chroma_ysize * 2;
}

template<typename T>
std::vector<T> pack_yuv(
    const T *im_y,
    const T *im_u,
    const T *im_v,
    const TiffWrap &tif)
{
    const auto def_xsize = tif.get_xsize();
    const auto def_ysize = tif.get_ysize();
    const auto xfactor = tif.get_chroma_sampling_xfactor();
    const auto yfactor = tif.get_chroma_sampling_yfactor();
    const auto xsize = (def_xsize + xfactor - 1) / xfactor * xfactor;
    const auto ysize = (def_ysize + yfactor - 1) / yfactor * yfactor;
    const auto chroma_xsize = xsize / xfactor;
    const auto chroma_ysize = ysize / yfactor;

    std::vector<T> im_yuv(xsize * ysize + chroma_xsize * chroma_ysize * 2);
    auto cnt = 0u;
    for (auto y = 0u, chroma_y = 0u; y < ysize; y += yfactor, ++chroma_y) {
        for (auto x = 0u, chroma_x = 0u; x < xsize; x += xfactor, ++chroma_x) {
            for (auto ly = 0u; ly < yfactor; ++ly) {
                for (auto lx = 0u; lx < xfactor; ++lx) {
                    const auto pos_y = y + ly;
                    const auto pos_x = x + lx;
                    im_yuv[cnt++] = pos_y >= def_ysize ? 0 :
                                    pos_x >= def_xsize ? 0 :
                                    im_y[def_xsize * pos_y + pos_x] ;
                }
            }
            im_yuv[cnt++] = im_u[chroma_xsize * chroma_y + chroma_x];
            im_yuv[cnt++] = im_v[chroma_xsize * chroma_y + chroma_x];
        }
    }

    return im_yuv;
}

template std::vector<uint8_t> pack_yuv<uint8_t>(
    const uint8_t *im_y,
    const uint8_t *im_u,
    const uint8_t *im_v,
    const TiffWrap &tif);
template std::vector<uint16_t> pack_yuv<uint16_t>(
    const uint16_t *im_y,
    const uint16_t *im_u,
    const uint16_t *im_v,
    const TiffWrap &tif);

template<typename T>
std::tuple<std::vector<T>, std::vector<T>, std::vector<T>> split_packed_yuv(
    const T *im_yuv,
    const TiffWrap &tif)
{
    const auto def_xsize = tif.get_xsize();
    const auto def_ysize = tif.get_ysize();
    const auto xfactor = tif.get_chroma_sampling_xfactor();
    const auto yfactor = tif.get_chroma_sampling_yfactor();
    const auto xsize = (def_xsize + xfactor - 1) / xfactor * xfactor;
    const auto ysize = (def_ysize + yfactor - 1) / yfactor * yfactor;
    const auto chroma_xsize = xsize / xfactor;
    const auto chroma_ysize = ysize / yfactor;

    std::vector<T> im_y(xsize * ysize);
    std::vector<T> im_u(chroma_xsize * chroma_ysize);
    std::vector<T> im_v(chroma_xsize * chroma_ysize);
    auto cnt = 0u;
    for (auto y = 0u, chroma_y = 0u; y < ysize; y += yfactor, ++chroma_y) {
        for (auto x = 0u, chroma_x = 0u; x < xsize; x += xfactor, ++chroma_x) {
            for (auto ly = 0u; ly < yfactor; ++ly) {
                for (auto lx = 0u; lx < xfactor; ++lx) {
                    const auto pos_y = y + ly;
                    const auto pos_x = x + lx;
                    im_y[xsize * pos_y + pos_x] = im_yuv[cnt++] ;
                }
            }
            im_u[chroma_xsize * chroma_y + chroma_x] = im_yuv[cnt++];
            im_v[chroma_xsize * chroma_y + chroma_x] = im_yuv[cnt++];
        }
    }

#if __cplusplus >= 201703L
    return { im_y, im_u, im_v };
#else
    return std::make_tuple(std::move(im_y), std::move(im_u), std::move(im_v));
#endif
}

template std::tuple<std::vector<uint8_t>, std::vector<uint8_t>, std::vector<uint8_t>> split_packed_yuv<uint8_t>(
    const uint8_t *im_yuv,
    const TiffWrap &tif);
template std::tuple<std::vector<uint16_t>, std::vector<uint16_t>, std::vector<uint16_t>> split_packed_yuv<uint16_t>(
    const uint16_t *im_yuv,
    const TiffWrap &tif);

}  // namespace tiffwrap
