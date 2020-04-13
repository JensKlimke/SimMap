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
// Created by Jens Klimke on 15.04.18.
//

#include "GeoElement.h"

#include <cmath>
#include <algorithm>
#include <base/definitions.h>
#include <base/functions.h>


namespace simmap {
namespace curve {


    void GeoElement::startPoint(const base::CurvePoint &pos) {

        r0 = pos.position;
        phi0 = pos.angle;

    }


    base::CurvePoint GeoElement::startPoint() const {

        return {r0, phi0, startCurvature()};

    }


    base::CurvePoint GeoElement::endPoint() const {

        return pos(length());

    }

    void GeoElement::length(double l) {

        if (l < 0.0)
            throw std::invalid_argument("Length must not be negative");

        len = l;

    }


    double GeoElement::length() const {

        return len;

    }


    std::vector<base::CurvePoint> GeoElement:: positions(const base::VectorX &v) const {

        // create result vector
        std::vector<base::CurvePoint> r(static_cast<size_t>(v.size()));

        // iterate over grid point
        for (int i = 0; i < v.size(); ++i)
            r[i] = pos(v[i]);

        return r;

    }


    std::vector<base::CurvePoint> GeoElement::operator()(const base::VectorX &v) const {

        return positions(v);

    }


    base::CurvePoint GeoElement::operator()(double s) const {

        return pos(s);

    }


    base::CurvePoint GeoElement::pos(double s, double d) const {

        // get curve position
        base::CurvePoint p = pos(s);

        // translate and rotate position
        base::Vector3 y{0.0, d, 0.0};
        base::toGlobal(p, y);

        // recalculate curvature
        auto c = p.curvature;
        p.curvature = c / (1.0 - d * c);

        return p;

    }


    base::VectorX GeoElement::steps(double dPhi_max, double s_max) const {

        // create vector
        base::VectorX stps{};

        // calculate change of curvature
        double sigma = fabs((endCurvature() - startCurvature()) / length());

        double s = 0.0;
        while(s < length()) {

            // add s to vector
            stps.push_back(s);

            // get ds and calculate s
            auto k0 = fabs(curvature(s));

            // calculate step
            auto t0 = dPhi_max / k0;
            if(sigma > base::EPS_CURVATURE)
                t0 = (1.0 / sigma) * (-1.0 * k0 + sqrt(k0 * k0 + 2.0 * dPhi_max * sigma));

            // calculate step
            auto ds = std::min(t0, s_max);
            s += ds;

        }

        // add last s
        stps.push_back(length() - base::EPS_DISTANCE);

        return stps;

    }


}}