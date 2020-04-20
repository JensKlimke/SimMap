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


    base::CurvePoint GeoElement::operator()(double s) const {

        return pos(s);

    }


    base::CurvePoint GeoElement::position(double s, double d) const {

        // get curve position
        base::CurvePoint p = pos(s);

        // translate and rotate position
        base::Vector3 y{0.0, d, 0.0};
        p.position = base::toGlobal(p, y);

        // recalculate curvature
        auto c = p.curvature;
        p.curvature = c / (1.0 - d * c);

        return p;

    }


}}