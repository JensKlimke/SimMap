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

#include "Curve.h"
#include "Line.h"
#include "Arc.h"
#include "Spiral.h"

#include <base/functions.h>
#include <cmath>

namespace simmap {
namespace curve {


    GeoElement::Type Curve::type() const {

        return GeoElement::Type::CURVE;

    }


    base::VectorX Curve::parameters() const {

        return {};

    }


    void Curve::startPoint(const base::CurvePoint &pos) {

        // check
        if (empty())
            throw std::runtime_error("no elements are defined");

        // set start point and update start points of elements
        GeoElement::startPoint(pos);

        // copy position
        auto uPos = pos;

        // update start points of underlaying curves
        for(auto const &e : *this) {

            e.element->startPoint(uPos);
            uPos = e.element->endPoint();

        }


    }


    base::CurvePoint Curve::startPoint() const {

        // get start point of first element
        return begin()->startPoint();

    }


    double Curve::startCurvature() const {

        return begin()->startCurvature();

    }


    double Curve::endCurvature() const {

        return end()->endCurvature();

    }


    double Curve::curvature(double s) const {

        auto e = atPos(s);
        return e.element->curvature(e.position);

    }


    void Curve::curvature(const base::VectorX &s, const base::VectorX &kappa) {

        // check size
        if (s.size() != kappa.size())
            throw std::invalid_argument("sizes don't match");

        // set end point
        length(s.back());

        // iterate over grid points to create elements
        for (int i = 0; i < s.size() - 1; ++i)
            _addElement(s[i], s[i + 1] - s[i], kappa[i], kappa[i + 1]);

    }


    void Curve::curvature(const base::VectorX &s, const base::VectorX &kappa0, const base::VectorX &kappa1) {

        // check size
        if (s.size() - 1 != kappa0.size() || kappa0.size() != kappa1.size())
            throw std::invalid_argument("sizes invalid");

        // set end point
        length(s.back());

        // iterate over grid points to create elements
        for (int i = 0; i < s.size() - 1; ++i)
            _addElement(s[i], s[i + 1] - s[i], kappa0[i], kappa1[i]);

    }


    void Curve::_addElement(double s, double ds, double kappa0, double kappa1) {

        // check for element type
        if(std::abs(kappa1) < base::EPS_CURVATURE && std::abs(kappa0) < base::EPS_CURVATURE)
            emplace(s, new curve::Line(ds));
        else if(std::abs(kappa1 - kappa0) < base::EPS_CURVATURE)
            emplace(s, new curve::Arc(ds, kappa0));
        else
            emplace(s, new curve::Spiral(ds, kappa0, kappa1));

    }


    base::CurvePoint Curve::pos(double s) const {

        // get position
        auto e = atPos(s);
        return e.element->pos(e.position);

    }


    double Curve::length() const {

        return GeoElement::length();

    }


    void Curve::length(double len) {

        sequence::length(len);
        GeoElement::length(len);

    }

}}

std::ostream& operator<< (std::ostream& o, const simmap::curve::Curve& crv) {

    using namespace simmap::curve;

    o << "Length: " << crv.length() << ": [";

    for(const auto &ge : crv) {

        o << ge.position << ": ";

        switch(ge.element->type()) {
            case GeoElement::Type::LINE:
                o << "L";
                break;
            case GeoElement::Type::ARC:
                o << "A";
                break;
            case GeoElement::Type::SPIRAL:
                o << "S";
                break;
            case GeoElement::Type::POLY_3:
                o << "P3";
                break;
            case GeoElement::Type::PARAM_POLY_3:
                o << "PP3";
                break;
            default:
                o << "unknown";
        }

        o << " (l=" << ge.length << "), ";

    }

    return o << "]";

}
