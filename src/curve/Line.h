//
// Created by Jens Klimke on 02.04.18.
//

#ifndef SIMMAP_CURVE_LINE_H
#define SIMMAP_CURVE_LINE_H

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
        explicit Line(double len) { length(len); }


        /**
         * Returns the type of the geo element
         * @return Type
         */
        GeoElement::Type type() const override { return GeoElement::Type::LINE; }


        /**
         * Returns the parameters of the geo element
         * @return Parameters
         */
        Eigen::RowVectorXd parameters() const override { return Eigen::RowVectorXd(0); };


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
        def::CurvePoint pos(double s) const override {
            def::CurvePoint pos(startPoint());
            return {Eigen::Vector3d{cos(pos.angle) * s, sin(pos.angle) * s, 0.0} + pos.position, pos.angle, 0.0};
        }


        /**
         * Returns a geo element which is equal to the given geo element but reversed in terms of the process variable s
         * @param geo Reversed geo element to be stored in (must be same type as original or compatible)
         */
        void reverse(GeoElement *geo) const override {

            auto line = dynamic_cast<Line*>(geo);

            // get end point and reserve angle
            auto sp = endPoint();
            sp.angle += M_PI;

            // set start point and length
            line->startPoint(sp);
            line->length(length());

        }


        /**
         * Creates a blank objects of the line
        * @return created object
        */
        virtual GeoElement* createBlank() const override {

            return new Line;

        }

    };

}
} // namespace ::simmap::curve

#endif // SIMMAP_CURVE_LINE_H
