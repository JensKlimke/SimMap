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
// Created by Jens Klimke on 05.08.18.
//

#ifndef SIMMAP_CURVE_POLY3_H
#define SIMMAP_CURVE_POLY3_H

#include "GeoElement.h"
#include <base/poly1.h>
#include <base/functions.h>

namespace simmap {
namespace curve {


    /**
     * class Poly3
     * @author Jens Klimke jens.klimke@rwth-aachen.de
     * TODO: only work with normalized curves -> transform in ODR-adapter and test with Suedtangente
     */
    class Poly3 : public GeoElement {


    private:

        base::poly1 polyX{};
        base::poly1 polyY{};


    public:


        Poly3() = default;


        explicit Poly3(double len, const double *ax, const double *ay)
                : polyX(base::poly1(ax[0], ax[1], ax[2], ax[3])),
                  polyY(base::poly1(ay[0], ay[1], ay[2], ay[3])) {

            // set length
            GeoElement::length(len);

        }


        explicit Poly3(const double *ax, const double *ay)
                : polyX(base::poly1(ax[0], ax[1], ax[2], ax[3])),
                  polyY(base::poly1(ay[0], ay[1], ay[2], ay[3])) {

            // calculate steps
            auto t = base::maxspace(0.0, 1.0, 1e-6);
            auto x = polyX(t);
            auto y = polyY(t);

            // calculate diffs
            base::VectorX dx = base::diff(x);
            base::VectorX dy = base::diff(y);

            double l = 0.0;
            for(size_t i = 0; i < dx.size(); ++i)
                l += sqrt(dx[i] * dx[i] + dy[i] * dy[i]);

            // save length
            GeoElement::length(l);

        }


        GeoElement::Type type() const override {

            return GeoElement::Type::PARAM_POLY_3;

        }


        base::VectorX parameters() const override {

            auto x = polyX.parameters();
            auto y = polyY.parameters();

            return base::VectorX{x[0], x[1], x[2], x[3], y[0], y[1], y[2], y[3]};

        };


        double startCurvature() const override {

            return curvature(0.0);

        }


        double endCurvature() const override {

            return curvature(length());

        }


        double curvature(double s) const override {

            double t = s / length();

            base::poly1 derX = polyX.der();
            base::poly1 derY = polyY.der();

            double dx = derX(t);
            double dx2 = derX.der()(t);

            double dy = derY(t);
            double dy2 = derY.der()(t);

            return pow(fabs(dy2 * dy + dx2 * dx) / (dx * dx + dy * dy), 1.5);

        }


        base::CurvePoint pos(double s) const override {

            double t = s / length();

            base::poly1 derX = polyX.der();
            base::poly1 derY = polyY.der();

            // rotate
            auto pos = base::Vector3{polyX(t), polyY(t), 0.0};
            auto sp = startPoint();
            pos = base::toGlobal(sp, pos);

            return {pos, sp.angle + atan2(derY(t), derX(t)), curvature(t)};

        }


        void reverse(GeoElement *geo) const override {

            auto poly = dynamic_cast<Poly3*>(geo);

            // get end point and reserve angle
            auto sp = endPoint();
            sp.angle += M_PI;

            // set start point and length
            poly->startPoint(sp);
            poly->length(length());

            // get parameters for x and create new parameter vector
            auto p = this->polyX.parameters();
            base::VectorX pn(p.size());

            // adapt parameters
            pn[3] = p[0] + p[1] + p[2] + p[3];
            pn[2] = -3.0 * p[0] - 2.0 * p[1] - p[2];
            pn[1] = 3.0 * p[0] + p[1];
            pn[0] = -p[0];

            // set parameters
            poly->polyX.parameters(pn);


            // get parameters for y and create new parameter vector
            p = this->polyY.parameters();

            // adapt parameters
            pn[3] = p[0] + p[1] + p[2] + p[3];
            pn[2] = -3.0 * p[0] - 2.0 * p[1] - p[2];
            pn[1] = 3.0 * p[0] + p[1];
            pn[0] = -p[0];

            // set parameters
            poly->polyY.parameters(pn);

        }


        GeoElement* createBlank() const override {

            return new Poly3;

        }

    };

}
} // namespace ::simmap::curve

#endif // SIMMAP_CURVE_POLY3_H
