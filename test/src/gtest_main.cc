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

#include <iostream>
#include <random>
#include "gtest/gtest.h"

namespace tiffwrap {

namespace unittest {

class TiffWrapTestEnvironment : public ::testing::Environment {
 public:
    explicit TiffWrapTestEnvironment(int seed)
    {
        m_mt.seed(m_seed = seed);
    }

    explicit TiffWrapTestEnvironment()
    {
        std::random_device rnd;
        m_mt.seed(m_seed = rnd());
    }

    virtual ~TiffWrapTestEnvironment() {}

    virtual void SetUp()
    {
        std::cout << "tiffwrap test seed: " << m_seed << std::endl;
    }

    virtual void TearDown()
    {
        ;
    }

 private:
    static int m_seed;
    static std::mt19937 m_mt;
};

int TiffWrapTestEnvironment::m_seed = 0;
std::mt19937 TiffWrapTestEnvironment::m_mt(0);

}  // namespace unittest

}  // namespace tiffwrap

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(
        new tiffwrap::unittest::TiffWrapTestEnvironment(1));
    return RUN_ALL_TESTS();
}

