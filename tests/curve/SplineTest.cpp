//
// Copyright (c) 2019 Jens Klimke <jens.klimke@rwth-aachen.de>. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Created by Jens Klimke on 20.11.2018.
//

#include <gtest/gtest.h>
#include <curve/C3Spline.h>
#include <Eigen/Core>

TEST(SplineTest, CreationAndEvaluation) {

    using namespace simmap::curve;

    Eigen::RowVector3d s(1.0, 11.0, 21.0);
    Eigen::RowVector3d w(0.0, 1.0, 1.0);

    C3Spline sp(s, w);
    EXPECT_DOUBLE_EQ(sp.length(), 20.0);

    // check results
    EXPECT_NEAR(0.0, sp(1.0),  1e-9);
    EXPECT_NEAR(0.5, sp(6.0),  1e-9);
    EXPECT_NEAR(1.0, sp(11.0), 1e-9);
    EXPECT_NEAR(1.0, sp(16.0), 1e-9);
    EXPECT_NEAR(1.0, sp(21.0), 1e-9);

    // check derivatives
    EXPECT_NEAR(0.0, sp.der(1.0),  1e-9);
    EXPECT_NEAR(0.0, sp.der(11.0), 1e-9);
    EXPECT_NEAR(0.0, sp.der(16.0), 1e-9);
    EXPECT_NEAR(0.0, sp.der(21.0), 1e-9);

}
