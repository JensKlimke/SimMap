/*
 * FunctionTest.h
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

#include <gtest/gtest.h>
#include <Eigen/Core>
#include <base/functions.h>

using namespace base;


inline void ASSERT_NEAR_VEC(std::vector<double> a, const Eigen::RowVectorXd& b) {

    ASSERT_EQ(a.size(), b.size());

    for(size_t i = 0; i < a.size(); ++i)
        ASSERT_FLOAT_EQ(a[i], b(i));

}

TEST(FunctionTest, AngleDiff) {

    ASSERT_FLOAT_EQ(0.0, angleDiff(0.0, 0.0));
    ASSERT_FLOAT_EQ(0.0, angleDiff(0.001, 0.001));
    ASSERT_FLOAT_EQ(0.0, angleDiff(-0.001, -0.001));

    ASSERT_FLOAT_EQ(0.002, angleDiff(0.001, -0.001));
    ASSERT_FLOAT_EQ(0.001, angleDiff(0.0, -0.001));
    ASSERT_FLOAT_EQ(-0.001, angleDiff(0.0, 0.001));

    ASSERT_FLOAT_EQ(0.0, angleDiff(M_PI, -M_PI));
    ASSERT_FLOAT_EQ(0.0, angleDiff(M_PI_2, -1.5 * M_PI));
    ASSERT_FLOAT_EQ(0.0, angleDiff(M_PI_2, -3.5 * M_PI));
    ASSERT_FLOAT_EQ(M_PI, angleDiff(M_PI_2, 1.5 * M_PI));
    ASSERT_FLOAT_EQ(M_PI, angleDiff(M_PI_2, 3.5 * M_PI));
    ASSERT_FLOAT_EQ(-0.01, angleDiff(2.0 * M_PI, 0.01));
    ASSERT_FLOAT_EQ(-0.01, angleDiff(-2.0 * M_PI, 0.01));
    ASSERT_FLOAT_EQ(-0.01, angleDiff(-200.0 * M_PI, 0.01));
    ASSERT_FLOAT_EQ(-0.02, angleDiff(2.0 * M_PI - 0.01, 0.01));
    ASSERT_FLOAT_EQ(-0.02, angleDiff(-2.0 * M_PI - 0.01, 0.01));

    ASSERT_FLOAT_EQ(0.0, angleDiff(2.0 * M_PI, 0.0));
    ASSERT_FLOAT_EQ(0.0, angleDiff(0.0, 2.0 * M_PI));
    ASSERT_FLOAT_EQ(0.0, angleDiff(0.0, 2.0 * M_PI));

    EXPECT_DOUBLE_EQ(0.0, angleDiff(-2.0 * M_PI, 0.0));
    EXPECT_DOUBLE_EQ(0.0, angleDiff(0.0, -2.0 * M_PI));
    EXPECT_DOUBLE_EQ(0.0, angleDiff(0.0, -2.0 * M_PI));

    EXPECT_DOUBLE_EQ(0.0, angleDiff(-2.0 * M_PI, 0.0));
    EXPECT_DOUBLE_EQ(0.0, angleDiff(0.0, -2.0 * M_PI));

}

TEST(FunctionTest, MaxSpace) {

    ASSERT_NEAR_VEC({0.0}, maxspace(0.0, 0.0, 1.0));
    ASSERT_NEAR_VEC({0.0, 1.0}, maxspace(0.0, 1.0, 1.0));
    ASSERT_NEAR_VEC({0.0, 1.0, 2.0}, maxspace(0.0, 2.0, 1.0));
    ASSERT_NEAR_VEC({0.0, 0.75, 1.5}, maxspace(0.0, 1.5, 1.0));
    ASSERT_NEAR_VEC({0.0, 0.5}, maxspace(0.0, 0.5, 1.0));

}


TEST(FunctionTest, VectorManipulation) {

    // prepare vectors
    Eigen::RowVectorXd z(0);
    Eigen::RowVectorXd a = Eigen::RowVector4d(0.0, 1.0, 3.0, 0.0);
    Eigen::RowVector4d a0(0.0, 1.0, 2.0, 3.0);
    Eigen::RowVector4d a1(5.0, 0.0, 1.0, 2.0);
    Eigen::RowVector4d a2(5.0, 0.0, 1.0, 6.0);


    // add value
    base::addValueToVector(a, 2, 2.0);
    EXPECT_TRUE(a.isApprox(a0, 1e-9));

    // add value at first position
    base::addValueToVector(a, 0, 5.0);
    EXPECT_TRUE(a.isApprox(a1, 1e-9));

    // add value at last position
    base::addValueToVector(a, 3, 6.0);
    EXPECT_TRUE(a.isApprox(a2, 1e-9));

//    // out of bounds errors
//    EXPECT_EXIT(base::addValueToVector(a, 4, 6.0), ::testing::KilledBySignal(SIGABRT), ".*");
//    EXPECT_EXIT(base::addValueToVector(z, 0, 6.0), ::testing::KilledBySignal(SIGABRT), ".*");


    // prepare vectors
    Eigen::RowVectorXd b = Eigen::RowVector4d(0.0, 1.0, 2.0, 3.0);
    Eigen::RowVector3d b0(0.0, 1.0, 3.0);
    Eigen::RowVector2d b1(0.0, 1.0);
    Eigen::RowVectorXd b2(1); b2 << 1.0;
    Eigen::RowVectorXd b3(0);


//    // test invalid argument
//    EXPECT_EXIT(base::removeElementFromVector(b, 4), ::testing::KilledBySignal(SIGABRT), ".*");

    // remove value
    base::removeElementFromVector(b, 2);
    EXPECT_TRUE(b.isApprox(b0, 1e-9));

    // remove last
    base::removeElementFromVector(b, 2);
    EXPECT_TRUE(b.isApprox(b1, 1e-9));

    // remove first
    base::removeElementFromVector(b, 0);
    EXPECT_TRUE(b.isApprox(b2, 1e-9));

    base::removeElementFromVector(b, 0);
    EXPECT_TRUE(b.isApprox(b3, 1e-9));

//    EXPECT_EXIT(base::removeElementFromVector(b, 0), ::testing::KilledBySignal(SIGABRT), ".*");

}