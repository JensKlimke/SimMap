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
#include <base/functions.h>

using namespace base;


inline void ASSERT_NEAR_VEC(const base::VectorX &a, const base::VectorX &b) {

    ASSERT_EQ(a.size(), b.size());

    for(size_t i = 0; i < a.size(); ++i)
        ASSERT_FLOAT_EQ(a[i], b[i]);

}

inline void ASSERT_NEAR_VEC3(const base::Vector3 &a, const base::Vector3 &b) {

    ASSERT_FLOAT_EQ(a.x, b.x);
    ASSERT_FLOAT_EQ(a.y, b.y);
    ASSERT_FLOAT_EQ(a.z, b.z);

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


TEST(FunctionTest, LinSpace) {

    // error
    ASSERT_THROW(linspace(0.0, 1.0, 1), std::invalid_argument);
    ASSERT_THROW(linspace(0.0, 1.0, 0), std::invalid_argument);

    // normal
    ASSERT_NEAR_VEC({0.0, 1.0},       linspace(0.0, 1.0, 2));
    ASSERT_NEAR_VEC({0.0, 1.0, 2.0},  linspace(0.0, 2.0, 3));
    ASSERT_NEAR_VEC({0.0, 0.75, 1.5}, linspace(0.0, 1.5, 3));
    ASSERT_NEAR_VEC({0.0, 0.5},       linspace(0.0, 0.5, 2));

    // negative
    ASSERT_NEAR_VEC({0.0, -1.0},        linspace( 0.0, -1.0, 2));
    ASSERT_NEAR_VEC({-1.0, -1.5, -2.0}, linspace(-1.0, -2.0, 3));

}


TEST(FunctionTest, MaxSpace) {

    // special case (a = b)
    ASSERT_NEAR_VEC({0.0}, maxspace(0.0, 0.0, 1.0));

    // normal
    ASSERT_NEAR_VEC({0.0, 1.0}, maxspace(0.0, 1.0, 1.0));
    ASSERT_NEAR_VEC({0.0, 1.0, 2.0}, maxspace(0.0, 2.0, 1.0));
    ASSERT_NEAR_VEC({0.0, 0.75, 1.5}, maxspace(0.0, 1.5, 1.0));
    ASSERT_NEAR_VEC({0.0, 0.5}, maxspace(0.0, 0.5, 1.0));

    // negative
    ASSERT_NEAR_VEC({1.0, 0.0, -1.0}, maxspace(1.0, -1.0, 1.0));

}


TEST(FunctionTest, VectorDiff) {

    VectorX vec{-1.0, 0.0, 1.0, 2.0, 4.0, 8.0, 16.0, 16.0, -16.0};
    ASSERT_NEAR_VEC({1.0, 1.0, 1.0, 2.0, 4.0, 8.0, 0.0, -32.0}, base::diff(vec));

}


TEST(FunctionTest, Zeros) {

    auto vec = base::zeros(5);
    ASSERT_NEAR_VEC({0.0, 0.0, 0.0, 0.0, 0.0}, vec);

    vec = base::zeros(0);
    ASSERT_NEAR_VEC({}, vec);

}


TEST(FunctionTest, Vector3Operation) {

    using namespace base;

    Vector3 a{0.0, 1.0, 2.0};
    Vector3 b{-2.0, 1.0, 0.0};

    ASSERT_NEAR_VEC3({-2.0, 2.0, 2.0}, a + b);
    ASSERT_NEAR_VEC3(b + a, a + b);
    ASSERT_NEAR_VEC3({2.0, 0.0, 2.0}, a - b);
    ASSERT_NEAR_VEC3({-2.0, 0.0, -2.0}, b - a);

}


TEST(FunctionTest, FlipOrientation) {

    using namespace base;

    ASSERT_EQ(Orientation::BACKWARDS, flip(Orientation::FORWARDS));
    ASSERT_EQ(Orientation::FORWARDS, flip(Orientation::BACKWARDS));
    ASSERT_EQ(Orientation::BOTH, flip(Orientation::BOTH));
    ASSERT_EQ(Orientation::NONE, flip(Orientation::NONE));

}


TEST(FunctionTest, TransformationToGlobal) {

    double err = 1e-9;

    // all zero
    base::CurvePoint curvePoint {{0.0, 0.0, 0.0}, 0.0, 0.0};
    base::Vector3 point {0.0, 0.0, 0.0};

    auto pnt = base::toGlobal(curvePoint, point);
    EXPECT_NEAR(0.0, pnt.x, err);
    EXPECT_NEAR(0.0, pnt.y, err);
    EXPECT_NEAR(0.0, pnt.z, err);


    // rotation in pos. direction
    curvePoint = base::CurvePoint{{0.0, 0.0, 0.0}, M_PI_2, 0.0};
    point = base::Vector3{1.0, 0.0, 0.0};

    pnt = base::toGlobal(curvePoint, point);
    EXPECT_NEAR(0.0, pnt.x, err);
    EXPECT_NEAR(1.0, pnt.y, err);
    EXPECT_NEAR(0.0, pnt.z, err);


    // rotation by pi
    curvePoint = base::CurvePoint{{0.0, 0.0, 0.0}, M_PI, 0.0};
    point = base::Vector3{1.0, 0.0, 0.0};

    pnt = base::toGlobal(curvePoint, point);
    EXPECT_NEAR(-1.0, pnt.x, err);
    EXPECT_NEAR(0.0, pnt.y, err);
    EXPECT_NEAR(0.0, pnt.z, err);


    // translation
    curvePoint = base::CurvePoint{{1.0, 2.0, 3.0}, 0.0, 5.0};
    point = base::Vector3{1.0, 0.0, 0.0};

    pnt = base::toGlobal(curvePoint, point);
    EXPECT_NEAR(2.0, pnt.x, err);
    EXPECT_NEAR(2.0, pnt.y, err);
    EXPECT_NEAR(3.0, pnt.z, err);


    // translation and rotation
    curvePoint = base::CurvePoint{{1.0, 2.0, 3.0}, 2.0 * M_PI, 5.0};
    point = base::Vector3{1.0, 0.0, 0.0};

    pnt = base::toGlobal(curvePoint, point);
    EXPECT_NEAR(2.0, pnt.x, err);
    EXPECT_NEAR(2.0, pnt.y, err);
    EXPECT_NEAR(3.0, pnt.z, err);


    // translation and neg. rotation
    curvePoint = base::CurvePoint{{1.0, 2.0, 3.0}, -M_PI_2, 5.0};
    point = base::Vector3{1.0, 0.0, 0.0};

    pnt = base::toGlobal(curvePoint, point);
    EXPECT_NEAR(1.0, pnt.x, err);
    EXPECT_NEAR(1.0, pnt.y, err);
    EXPECT_NEAR(3.0, pnt.z, err);


    // translation and rotation
    curvePoint = base::CurvePoint{{1.0, 2.0, 3.0}, M_PI_2, 5.0};
    point = base::Vector3{1.0, 1.0, 5.0};

    pnt = base::toGlobal(curvePoint, point);
    EXPECT_NEAR(0.0, pnt.x, err);
    EXPECT_NEAR(3.0, pnt.y, err);
    EXPECT_NEAR(8.0, pnt.z, err);


    // translation and neg. rotation
    curvePoint = base::CurvePoint{{1.0, 2.0, 3.0}, -M_PI_2, 5.0};
    point = base::Vector3{1.0, 1.0, 5.0};

    pnt = base::toGlobal(curvePoint, point);
    EXPECT_NEAR(2.0, pnt.x, err);
    EXPECT_NEAR(1.0, pnt.y, err);
    EXPECT_NEAR(8.0, pnt.z, err);

}


TEST(FunctionTest, TransformationToLocal) {

    double err = 1e-9;

    // all zero
    base::CurvePoint curvePoint {{0.0, 0.0, 0.0}, 0.0, 0.0};
    base::Vector3 point {0.0, 0.0, 0.0};

    auto pnt = base::toLocal(curvePoint, point);
    EXPECT_NEAR(0.0, pnt.x, err);
    EXPECT_NEAR(0.0, pnt.y, err);
    EXPECT_NEAR(0.0, pnt.z, err);


    // rotation in pos. direction
    curvePoint = base::CurvePoint{{0.0, 0.0, 0.0}, M_PI_2, 0.0};
    point = base::Vector3{0.0, 1.0, 0.0};

    pnt = base::toLocal(curvePoint, point);
    EXPECT_NEAR(1.0, pnt.x, err);
    EXPECT_NEAR(0.0, pnt.y, err);
    EXPECT_NEAR(0.0, pnt.z, err);


    // rotation by pi
    curvePoint = base::CurvePoint{{0.0, 0.0, 0.0}, M_PI, 0.0};
    point = base::Vector3{-1.0, 0.0, 0.0};

    pnt = base::toLocal(curvePoint, point);
    EXPECT_NEAR(1.0, pnt.x, err);
    EXPECT_NEAR(0.0, pnt.y, err);
    EXPECT_NEAR(0.0, pnt.z, err);


    // translation
    curvePoint = base::CurvePoint{{1.0, 2.0, 3.0}, 0.0, 5.0};
    point = base::Vector3{2.0, 2.0, 3.0};

    pnt = base::toLocal(curvePoint, point);
    EXPECT_NEAR(1.0, pnt.x, err);
    EXPECT_NEAR(0.0, pnt.y, err);
    EXPECT_NEAR(0.0, pnt.z, err);


    // translation and rotation
    curvePoint = base::CurvePoint{{1.0, 2.0, 3.0}, 2.0 * M_PI, 5.0};
    point = base::Vector3{2.0, 2.0, 3.0};

    pnt = base::toLocal(curvePoint, point);
    EXPECT_NEAR(1.0, pnt.x, err);
    EXPECT_NEAR(0.0, pnt.y, err);
    EXPECT_NEAR(0.0, pnt.z, err);


    // translation and neg. rotation
    curvePoint = base::CurvePoint{{1.0, 2.0, 3.0}, -M_PI_2, 5.0};
    point = base::Vector3{1.0, 1.0, 3.0};

    pnt = base::toLocal(curvePoint, point);
    EXPECT_NEAR(1.0, pnt.x, err);
    EXPECT_NEAR(0.0, pnt.y, err);
    EXPECT_NEAR(0.0, pnt.z, err);

}


TEST(FunctionTest, SpeedTest) {

    double err = 1e-9;

    base::Vector3 pntLocal  {0.0, 0.0, 0.0};
    base::Vector3 pntGlobal {1.0, 2.0, 3.0};

    base::CurvePoint curvePoint {{1.0, -2.0, -5.0}, 0.0, 0.0};

    // many steps
    for(unsigned int i = 0; i < 100000; ++i) {

        pntLocal = base::toLocal(curvePoint, pntGlobal);
        auto tmp = base::toGlobal(curvePoint, pntLocal);

        EXPECT_NEAR(pntGlobal.x, tmp.x, err);
        EXPECT_NEAR(pntGlobal.y, tmp.y, err);
        EXPECT_NEAR(pntGlobal.z, tmp.z, err);

        curvePoint.angle += M_PI * 0.1;

    }

}