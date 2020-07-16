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
// Created by Jens Klimke on 10.04.18.
//

#ifndef SIMMAP_CURVE_ARC_H
#define SIMMAP_CURVE_ARC_H

#include "GeoElement.h"

namespace simmap {
namespace curve {


    /**
     * class Arc
     * @author Jens Klimke jens.klimke@rwth-aachen.de
     */
    class Arc : public GeoElement {

        double _crv = 0.0;

    public:


        /**
         * Default constructor
         */
        Arc() = default;


        /**
         * Constructor
         * @param len Length of the element
         * @param c Curvature of the element
         */
        Arc(double len, double c) : _crv(c) {

            GeoElement::length(len);

        }


        GeoElement::Type type() const override {

            return GeoElement::Type::ARC;

        }


        base::VectorX parameters() const override {

            base::VectorX v{_crv};
            return v;

        };


        double startCurvature() const override {

            return _crv;

        }


        double endCurvature() const override {

            return _crv;

        }


        double curvature(double s) const override {

            return _crv;

        }


        base::CurvePoint pos(double s) const override {

            // get start position
            base::CurvePoint pos0(startPoint());

            // get start angle and crv
            double phi0(pos0.angle);
            double kappa(pos0.curvature);

            // calculate start points
            double x0(sin(phi0));
            double y0(1.0 - cos(phi0));

            // calculate end angle
            double phi = phi0 + s * startCurvature();

            // return position
            return {base::Vector3{(sin(phi) - x0) / kappa, (1.0 - cos(phi) - y0) / kappa, 0.0} + pos0.position, phi, kappa};

        }


    };


}
} // namespace ::simmap::curve

#endif // SIMMAP_CURVE_ARC_H
