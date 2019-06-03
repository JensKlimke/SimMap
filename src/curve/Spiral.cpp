//
// Created by Jens Klimke on 2019-05-05.
//

#include "Spiral.h"
#include "odrSpiral.h"

namespace simmap {
namespace curve {

    Spiral::Spiral(double len, double c0, double c1) : _kappa0(c0), _kappa1(c1) {

        length(len);

    }


    GeoElement::Type Spiral::type() const {

        return GeoElement::Type::SPIRAL;

    }



    /**
     * Returns the parameters defining the element
     * @return Parameter vector
     */
    Eigen::RowVectorXd Spiral::parameters() const {

        Eigen::RowVectorXd p(2);
        p << _kappa0, _kappa1;

        return p;

    }



    double Spiral::startCurvature() const {

        return _kappa0;

    }



    double Spiral::endCurvature() const {

        return _kappa1;

    }



    double Spiral::curvatureDerivative() const {

        // calculate sigma
        return (_kappa1 - _kappa0) / std::max(def::EPS_DISTANCE, length());

    }


    double Spiral::curvature(double s) const {

        return _kappa0 + s * curvatureDerivative();

    }



    def::CurvePoint Spiral::pos(double s) const {

        // get start position
        def::CurvePoint pos0(startPoint());

        // calculate end crv
        double kappa1 = curvature(s);
        double sigma  = curvatureDerivative();

        // initialize local results
        double x0, y0, tau0;
        double x1, y1, tau1;

        // calculate start and end of spiral
        odrSpiral(_kappa0 / sigma, sigma, &x0, &y0, &tau0);
        odrSpiral(kappa1 / sigma, sigma, &x1, &y1, &tau1);

        // translate to origin
        x1 = x1 - x0;
        y1 = y1 - y0;

        // TODO: rotation by eigen
        // angle to rotate
        double dPhi = pos0.angle - tau0;

        // trigonometry
        double cs = cos(dPhi);
        double ss = sin(dPhi);

        // rotate local position and return position
        return {Eigen::Vector3d{cs * x1 - ss * y1, ss * x1 + cs * y1, 0.0} + pos0.position, tau1 + dPhi, kappa1};

    }



    void Spiral::reverse(GeoElement *geo) const {

        auto spi = dynamic_cast<Spiral*>(geo);

        // get end point and reserve angle
        auto sp = endPoint();
        sp.angle += M_PI;

        // set start point
        spi->startPoint(sp);

        // set length
        spi->length(length());

        // reserve curvatures and set
        spi->_kappa0 = -_kappa1;
        spi->_kappa1 = -_kappa0;

    }



    GeoElement* Spiral::createBlank() const {

        return new Spiral;

    }

}}