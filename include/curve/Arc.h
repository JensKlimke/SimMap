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

            length(len);

        }


        GeoElement::Type type() const override {

            return GeoElement::Type::ARC;

        }


        Eigen::RowVectorXd parameters() const override {

            Eigen::RowVectorXd v(1);
            v << _crv;
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


        def::CurvePoint pos(double s) const override {

            // get start position
            def::CurvePoint pos0(startPoint());

            // get start angle and crv
            double phi0(pos0.angle);
            double kappa(pos0.curvature);

            // calculate start points
            double x0(sin(phi0));
            double y0(1.0 - cos(phi0));

            // calculate end angle
            double phi = phi0 + s * startCurvature();

            // return position
            return {Eigen::Vector3d{(sin(phi) - x0) / kappa, (1.0 - cos(phi) - y0) / kappa, 0.0} + pos0.position, phi, kappa};

        }


        void reverse(GeoElement *geo) const override {

            auto arc = dynamic_cast<Arc*>(geo);

            // get end point and reserve angle
            auto sp = endPoint();
            sp.angle += M_PI;

            // set start point
            arc->startPoint(sp);

            // set curvature and length
            arc->length(length());
            arc->_crv = -startCurvature();

        }


        GeoElement* createBlank() const override {

            return new Arc;

        }

    };


}
} // namespace ::simmap::curve

#endif // SIMMAP_CURVE_ARC_H
