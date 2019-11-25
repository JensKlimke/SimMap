/*
 * PolyTest.h
 *
 * MIT License
 *
 * Copyright (c) 2019 Jens Klimke <jens.klimke@rwth-aachen.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 *         of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 *         to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *         copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 *         copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *         AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <base/poly1.h>
#include <gtest/gtest.h>

class PolyTest : public base::poly1, public testing::Test {

    void SetUp() override {

        // create polynomial
        parameters(-0.083333333333333, 0.375, -0.041666666666667, 1.0);

    }

    void TearDown() override {}

};


TEST_F(PolyTest, Calculation) {

    // calculate polynomial
    auto x = Eigen::RowVectorXd(10);
    auto y = Eigen::RowVectorXd(10);

    // set data
    x << 0.0, 0.3333, 0.6667, 1.0, 1.3333, 1.6667, 2.0, 2.3333, 2.6667, 3.0;
    y << 1.0, 1.0247, 1.1142, 1.25, 1.4136, 1.5864, 1.75, 1.8858, 1.9753, 2.0;

    // calculate results
    auto yc = this->operator()(x);

    // check results
    for(unsigned long i = 0; i < x.rows(); ++i)
        EXPECT_NEAR(y(i), yc(i), 1e-3);

}

TEST_F(PolyTest, Derivative) {

    auto d = der();
    auto dp = d.parameters();

    EXPECT_NEAR(-0.25,   dp(0), 1e-4);
    EXPECT_NEAR(0.75,    dp(1), 1e-4);
    EXPECT_NEAR(-0.0417, dp(2), 1e-4);

    // calculate polynomial
    auto x = Eigen::RowVectorXd(10);
    auto y = Eigen::RowVectorXd(10);

    // set data
    x << 0.0, 0.3333, 0.6667, 1.0, 1.3333, 1.6667, 2.0, 2.3333, 2.6667, 3.0;
    y << -0.0417, 0.1806, 0.3472, 0.4583, 0.5139, 0.5139, 0.4583, 0.3472, 0.1806, -0.0417;

    // calculate results
    auto yc = d(x);

    // check results
    for(unsigned long i = 0; i < x.rows(); ++i)
        EXPECT_NEAR(y(i), yc(i), 1e-3);

}


TEST_F(PolyTest, Shift) {

    // shift polynomial
    auto p = shift(2.0);
    auto sp = p.parameters();

    // check parameters
    EXPECT_NEAR(-0.0833, sp(0), 1e-4);
    EXPECT_NEAR( 0.875,  sp(1), 1e-4);
    EXPECT_NEAR(-2.5417, sp(2), 1e-4);
    EXPECT_NEAR(3.25,    sp(3), 1e-4);

    // calculate polynomial
    auto x = Eigen::RowVectorXd(10);
    auto y = Eigen::RowVectorXd(10);

    // set data
    x << 2.0, 2.3333, 2.6667, 3.0, 3.3333, 3.6667, 4.0, 4.3333, 4.6667, 5.0;
    y << 1.0, 1.0247, 1.1142, 1.25, 1.4136, 1.5864, 1.75, 1.8858, 1.9753, 2.0;

    // calculate results
    auto yc = p(x);

    // check results
    for(unsigned long i = 0; i < x.rows(); ++i)
        EXPECT_NEAR(y(i), yc(i), 1e-3);

    // error
    poly1 p2;
    p2.parameters(Eigen::Vector3d{0.0, 1.0, 2.0});
    EXPECT_THROW(p2.shift(10.0), std::runtime_error);

}


TEST_F(PolyTest, CreationByAlgorithms) {

    // create polynomial (equal to this one)
    auto ps= shift(2.0);
    auto pg = order3_fromValueAndDerivative(2.0, 5.0, 1.0, -0.0417, 2.0, -0.0417);

    // get parameters
    auto param0 = pg.parameters();
    auto param1 = ps.parameters();

    // check results
    for(unsigned long i = 0; i < param0.rows(); ++i)
        EXPECT_NEAR(param1(i), param0(i), 1e-3);

}
