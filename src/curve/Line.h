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
// Created by Jens Klimke on 02.04.18.
//

#ifndef SIMMAP_CURVE_LINE_H
#define SIMMAP_CURVE_LINE_H

#include <base/functions.h>
#include "GeoElement.h"

namespace simmap {
namespace curve {


    /**
     * class Line
     * @author Jens Klimke jens.klimke@rwth-aachen.de
     */
    class Line : public GeoElement {

    public:

        /**
         * Default constructor
         */
        Line() = default;


        /**
         * Constructor
         * @param len Length of the line
         */
        explicit Line(double len) {

            GeoElement::length(len);

        }


        /**
         * Returns the type of the geo element
         * @return Type
         */
        GeoElement::Type type() const override { return GeoElement::Type::LINE; }


        /**
         * Returns the parameters of the geo element
         * @return Parameters
         */
        base::VectorX parameters() const override { return base::VectorX{}; };


        /**
         * Returns the start curvature of the geo element
         * @return start curvature
         */
        double startCurvature() const override { return 0.0; }


        /**
         * Returns the end curvature of the geo element
         * @return end curvature
         */
        double endCurvature() const override { return 0.0; }


        /**
         * Returns the curvature at the given position
         * @param s Position
         * @return Curvature
         */
        double curvature(double s) const override { return 0.0; }


        /**
         * Returns the curve positions at the given positions
         * @param v Positions
         * @return Curve positions
         */
        base::CurvePoint pos(double s) const override {

            base::CurvePoint pos(startPoint());
            return {base::Vector3{cos(pos.angle) * s, sin(pos.angle) * s, 0.0} + pos.position, pos.angle, 0.0};

        }


    };

}
} // namespace ::simmap::curve

#endif // SIMMAP_CURVE_LINE_H
