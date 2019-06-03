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
