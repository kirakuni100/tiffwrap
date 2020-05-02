#ifndef _TIFFWRAP_H_
#define _TIFFWRAP_H_

#include <cstdint>
#include <string>
#include <tiffio.h>

namespace tiffwrap {

enum class Ftype {
    ERROR = -1,
    SINGLE,
    MULTI
};

enum class Color {
    ERROR = -1,
    MONO,
    RGB,
    CMYK,
    YUV
};

enum class Plane {
    ERROR = -1,
    PACKED,
    PLANAR
};

enum class YUVSubSampling {
    ERROR = -1,
    YUV444,
    YUV422,
    YUV420,
    YUV411,
    YUV410
};

class TiffWrap {
 public:
     explicit TiffWrap();
     virtual ~TiffWrap();
     TiffWrap(const TiffWrap &rhs);
     TiffWrap &operator=(const TiffWrap &rhs);
     TiffWrap(TiffWrap &&rhs) noexcept;
     TiffWrap &operator=(TiffWrap &&rhs) noexcept;

     int wopen(const std::string &name);
     int ropen(const std::string &name);
     void close();

     void set_ftype(const Ftype ftype);
     void set_xsize(const uint32_t xsize);
     void set_ysize(const uint32_t ysize);
     void set_depth(const uint16_t depth);
     void set_comps(const uint16_t comps);
     void set_color(const Color color);
     void set_plane(const Plane plane);
     void set_subsampling(const YUVSubSampling subsampling);

     Ftype get_ftype() const;
     uint32_t get_xsize() const;
     uint32_t get_ysize() const;
     uint16_t get_depth() const;
     uint16_t get_comps() const;
     Color get_color() const;
     Plane get_plane() const;
     YUVSubSampling get_subsampling() const;
     uint16_t get_chroma_sampling_xfactor() const;
     uint16_t get_chroma_sampling_yfactor() const;

     int set_tags();
     int get_tags();

     template<typename T> int write(T *p_data, uint16_t comp = 0);
     template<typename T> int read(T *p_data, uint16_t comp = 0);

 protected:
     int check_params();

 protected:
    struct TiffTagField {
        uint32_t subfiletype;
        uint32_t image_width;
        uint32_t image_length;
        uint16_t bits_per_sample;
        uint16_t photometric;
        uint16_t samples_per_pixel;
        uint16_t planar_config;
        uint16_t subsampling[2];
        uint32_t rows_per_strip;
    };

    TIFF *m_tiff;
    Ftype m_ftype;
    uint32_t m_xsize;
    uint32_t m_ysize;
    uint16_t m_depth;
    uint16_t m_comps;
    Color m_color;
    Plane m_plane;
    YUVSubSampling m_subsampling;
    TiffTagField m_field;
};

inline void TiffWrap::set_ftype(const Ftype ftype)
{
    m_ftype = ftype;
    m_field.subfiletype =
        (m_ftype == Ftype::SINGLE) ? 0             :
        (m_ftype == Ftype::MULTI)  ? FILETYPE_PAGE : 0;
}

inline void TiffWrap::set_xsize(const uint32_t xsize)
{
    m_xsize = xsize;
    m_field.image_width = m_xsize;
}

inline void TiffWrap::set_ysize(const uint32_t ysize)
{
    m_ysize = ysize;
    m_field.image_length = m_ysize;
}

inline void TiffWrap::set_depth(const uint16_t depth)
{
    m_depth = depth;
    m_field.bits_per_sample = m_depth;
}

inline void TiffWrap::set_comps(const uint16_t comps)
{
    m_comps = comps;
    m_field.samples_per_pixel = m_comps;
}

inline void TiffWrap::set_color(const Color color)
{
    m_color = color;
    m_field.photometric =
        (m_color == Color::MONO) ? PHOTOMETRIC_MINISBLACK :
        (m_color == Color::RGB)  ? PHOTOMETRIC_RGB        :
        (m_color == Color::CMYK) ? PHOTOMETRIC_SEPARATED  :
        (m_color == Color::YUV)  ? PHOTOMETRIC_YCBCR      : 0;
}

inline void TiffWrap::set_plane(const Plane plane)
{
    m_plane = plane;
    m_field.planar_config =
        (m_plane == Plane::PACKED) ? PLANARCONFIG_CONTIG   :
        (m_plane == Plane::PLANAR) ? PLANARCONFIG_SEPARATE : 0;
}

inline void TiffWrap::set_subsampling(const YUVSubSampling subsampling)
{
    m_subsampling = subsampling;
    m_field.subsampling[0] =
        (m_subsampling == YUVSubSampling::YUV444) ? 1 :
        (m_subsampling == YUVSubSampling::YUV422) ? 2 :
        (m_subsampling == YUVSubSampling::YUV420) ? 2 :
        (m_subsampling == YUVSubSampling::YUV411) ? 4 :
        (m_subsampling == YUVSubSampling::YUV410) ? 4 : 0;
    m_field.subsampling[1] =
        (m_subsampling == YUVSubSampling::YUV444) ? 1 :
        (m_subsampling == YUVSubSampling::YUV422) ? 1 :
        (m_subsampling == YUVSubSampling::YUV420) ? 2 :
        (m_subsampling == YUVSubSampling::YUV411) ? 1 :
        (m_subsampling == YUVSubSampling::YUV410) ? 4 : 0;
}

inline Ftype TiffWrap::get_ftype() const
{
    return m_ftype;
}

inline uint32_t TiffWrap::get_xsize() const
{
    return m_xsize;
}

inline uint32_t TiffWrap::get_ysize() const
{
    return m_ysize;
}

inline uint16_t TiffWrap::get_depth() const
{
    return m_depth;
}

inline uint16_t TiffWrap::get_comps() const
{
    return m_comps;
}

inline Color TiffWrap::get_color() const
{
    return m_color;
}

inline Plane TiffWrap::get_plane() const
{
    return m_plane;
}

inline YUVSubSampling TiffWrap::get_subsampling() const
{
    return m_subsampling;
}

inline uint16_t TiffWrap::get_chroma_sampling_xfactor() const
{
    return m_field.subsampling[0];
}

inline uint16_t TiffWrap::get_chroma_sampling_yfactor() const
{
    return m_field.subsampling[1];
}

}  // namespace tiffwrap

#endif  // _TIFFWRAP_H_
