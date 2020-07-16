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
// Created by Jens Klimke on 30.03.18.
//

#ifndef SIMMAP_CURVE_GEOELEMENT_H
#define SIMMAP_CURVE_GEOELEMENT_H

#include <vector>
#include <base/definitions.h>

namespace simmap {
namespace curve {


/**
 * class GeoElement
 * @author Jens Klimke jens.klimke@rwth-aachen.de
 */
    class GeoElement {

    private:

        base::Vector3 r0 = {0.0, 0.0, 0.0};
        double phi0 = 0.0;
        double len = 0.0;

    public:

        enum class Type {
            LINE, ARC, SPIRAL, POLY_3, PARAM_POLY_3, CURVE
        };


        /**
         * Default destructor
         */
        virtual ~GeoElement() = default;


        /**
         * Returns the type of the geo element
         * @return Type
         */
        virtual Type type() const = 0;


        /**
         * Returns the parameters of the geo element
         * @return Parameters
         */
        virtual base::VectorX parameters() const = 0;


        /**
         * Sets the start point of the geo element
         * @param pos Start point
         */
        virtual void startPoint(const base::CurvePoint &pos);


        /**
         * Returns the start point of the geo element
         * @return Start point
         */
        virtual base::CurvePoint startPoint() const;


        /**
         * Returns the end point of the geo element
         * @return
         */
        virtual base::CurvePoint endPoint() const;

        /**
         * Returns the start curvature of the geo element
         * @return start curvature
         */
        virtual double startCurvature() const = 0;


        /**
         * Returns the end curvature of the geo element
         * @return end curvature
         */
        virtual double endCurvature() const = 0;


        /**
         * Returns the curvature at the given position
         * @param s Position
         * @return Curvature
         */
        virtual double curvature(double s) const = 0;


        /**
         * Sets the length of the geo element
         * @param l Length
         */
        virtual void length(double l);


        /**
         * Returns the length of the geo element
         * @return
         */
        virtual double length() const;


        /**
         * Returns the curve position at the given position
         * @param s Position
         * @return Curve position
         */
        virtual base::CurvePoint operator()(double s) const;


        /**
         * Returns the curve position at the given position
         * @param s Position
         * @return Curve position
         */
        virtual base::CurvePoint pos(double s) const = 0;


        /**
         * Returns the curve position at the given position with respect to a given offset
         * @param s Position
         * @param d Offset
         * @return Curve position
         */
        base::CurvePoint position(double s, double d) const;

    };

}} // namespace ::simmap::server

#endif // SIMMAP_SERVER_GEOELEMENT_H
