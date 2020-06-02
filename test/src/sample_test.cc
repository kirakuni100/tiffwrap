#include <vector>
#include "tiffwrap.h"
#include "tiffwrap_test.h"

int factorial(int number) { return number <= 1 ? number : factorial(number - 1) * number; }

TEST(SampleTest, SampleTest000) {
    EXPECT_EQ(factorial(0), 1);
    EXPECT_EQ(factorial(1), 1);

    tiffwrap::TiffWrap tif;
    tif.wopen("sample.tif");

    tif.set_xsize(100);
    tif.set_ysize(100);
    tif.set_depth(8);
    tif.set_comps(3);
    tif.set_color(tiffwrap::Color::RGB);
    tif.set_tags();

    const auto ysize = tif.get_ysize();
    const auto xsize = tif.get_xsize();
    const auto comps = tif.get_comps();

    std::vector<uint8_t> image(ysize * xsize * comps);
    for (auto y = 0u; y < ysize; ++y) {
        for (auto x = 0u; x < xsize; ++x) {
            for (auto c = 0u; c < comps; ++c) {
                image[xsize * comps * y + comps * x + c] =
                    xsize * comps * y + comps * x + c;
            }
        }
    }

    tif.write(image.data());
    tif.close();
}

