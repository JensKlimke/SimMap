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
            length(len);

        }


        explicit Poly3(const double *ax, const double *ay)
                : polyX(base::poly1(ax[0], ax[1], ax[2], ax[3])),
                  polyY(base::poly1(ay[0], ay[1], ay[2], ay[3])) {

            // calculate steps
            auto t = base::maxspace(0.0, 1.0, 0.001);
            auto x = polyX(t);
            auto y = polyY(t);

            // calculate diffs
            Eigen::RowVectorXd dx =
                    x.block(0, 1, x.rows(), x.cols() - 1).eval() - x.block(0, 0, x.rows(), x.cols() - 1).eval();
            Eigen::RowVectorXd dy =
                    y.block(0, 1, y.rows(), y.cols() - 1).eval() - y.block(0, 0, y.rows(), y.cols() - 1).eval();

            // save length
            length(sqrt(dx.array() * dx.array() + dy.array() * dy.array()).sum());

        }


        GeoElement::Type type() const override {

            return GeoElement::Type::PARAM_POLY_3;

        }


        Eigen::RowVectorXd parameters() const override {

            Eigen::VectorXd d(8);
            d << polyX.parameters(), polyY.parameters();

            return d;

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


        def::CurvePoint pos(double s) const override {

            double t = s / length();

            base::poly1 derX = polyX.der();
            base::poly1 derY = polyY.der();

            // rotate
            auto pos = Eigen::Vector3d(polyX(t), polyY(t), 0.0);
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
            Eigen::VectorXd pn(p.size());

            // adapt parameters
            pn(3) = p(0) + p(1) + p(2) + p(3);
            pn(2) = -3.0 * p(0) - 2.0 * p(1) - p(2);
            pn(1) = 3.0 * p(0) + p(1);
            pn(0) = -p(0);

            // set parameters
            poly->polyX.parameters(pn);


            // get parameters for y and create new parameter vector
            p = this->polyY.parameters();

            // adapt parameters
            pn(3) = p(0) + p(1) + p(2) + p(3);
            pn(2) = -3.0 * p(0) - 2.0 * p(1) - p(2);
            pn(1) = 3.0 * p(0) + p(1);
            pn(0) = -p(0);

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
