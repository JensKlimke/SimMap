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
// Created by Jens Klimke on 03.04.18.
//

#ifndef SIMMAP_CURVE_SPIRAL_H
#define SIMMAP_CURVE_SPIRAL_H

#include <Eigen/Core>
#include "GeoElement.h"

namespace simmap {
namespace curve {


    /**
     * class Spiral
     * @author Jens Klimke jens.klimke@rwth-aachen.de
     */
    class Spiral : public GeoElement {

        double _kappa0 = 0.0;
        double _kappa1 = 0.0;

    public:


        /**
         * Default constructor
         */
        Spiral() = default;



        /**
         * Constructor
         * @param len Length of the spiral
         * @param c0 Start curvature
         * @param c1 End curvature
         */
        Spiral(double len, double c0, double c1);



        /**
         * Returns the type (SPIRAL)
         * @return Spiral
         */
        GeoElement::Type type() const override;



        /**
         * Returns the parameters defining the element
         * @return Parameter vector
         */
        Eigen::RowVectorXd parameters() const override;



        /**
         * Returns the start curvature
         * @return Start curvature
         */
        double startCurvature() const override;



        /**
         * Returns the end curvature
         * @return End curvature
         */
        double endCurvature() const override;



        /**
         * Returns the derivative of the curvature
         * @return Derivative of curvature
         */
        double curvatureDerivative() const;



        /**
         * Returns the curvature at the given position
         * @param s Position
         * @return Curvature
         */
        double curvature(double s) const override;



        /**
         * Returns the curve position of the spiral at the given position
         * @param s Position
         * @return Curve position
         */
        def::CurvePoint pos(double s) const override;



        /**
         * Reverse the geo element
         * @param geo Geo element
         */
        void reverse(GeoElement *geo) const override;



        /**
         * Creates a blank element of the spiral
         * @return Spiral
         */
        GeoElement* createBlank() const override;

    };

}
} // namespace ::simmap::curve

#endif // SIMMAP_CURVE_SPIRAL_H
