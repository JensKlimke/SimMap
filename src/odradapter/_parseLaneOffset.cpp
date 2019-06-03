//
// Created by klimke on 09.05.2019.
//

#include "ODRRoad.h"
#include <curve/C3Spline.h>
#include <odr/odr1_5_structure.h>


void parseLaneOffset(ODRRoad *road, const odr1_5::t_road &r) {

    using namespace simmap::curve;

    // get number of elements
    const auto &lo = r.sub_lanes->sub_laneOffset;
    auto n = lo.size();

    // create s vector (set road length as last element)
    Eigen::RowVectorXd s(n + 1);
    s(0) = 0.0;
    s(n) = *r._length;

    // create matrix for parameter
    Eigen::Matrix<double, 4, Eigen::Dynamic> p(4, n);

    // iterate over all elements
    for (int i = 0; i < n; ++i) {

        // init attributes
        double S = *lo.at(i)._s;
        double P[4] = {*lo.at(i)._d, *lo.at(i)._c, *lo.at(i)._b, *lo.at(i)._a};

        // set vector and matrix
        s(i) = S;
        for (int k = 0; k < 4; k++)
            p(k, i) = P[k];

    }

    // create spline (create zero spline when offset not set)
    if (n > 0)
        road->_offset = std::make_shared<C3Spline>(s, p);
    else
        road->_offset = std::make_shared<C3Spline>(Eigen::RowVector2d(0.0, *r._length), Eigen::Matrix2d::Zero());

}
