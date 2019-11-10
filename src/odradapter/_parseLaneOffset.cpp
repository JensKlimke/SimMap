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
// Created by Jens Klimke on 09.05.2019.
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
