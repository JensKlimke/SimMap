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
