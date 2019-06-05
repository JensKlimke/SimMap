//
// Created by Jens Klimke on 15.04.18.
//

#include "Curve.h"

#include "Line.h"
#include "Arc.h"
#include "Spiral.h"
#include "Poly3.h"

#include <base/functions.h>
#include <cmath>

namespace simmap {
namespace curve {


    GeoElement::Type Curve::type() const {

        return GeoElement::Type::CURVE;

    }


    Eigen::RowVectorXd Curve::parameters() const {

        return {};

    }


    void Curve::startPoint(const def::CurvePoint &pos) {

        // check
        if (empty())
            throw std::runtime_error("no elements are defined");

        // set start point and update start points of elements
        GeoElement::startPoint(pos);

        // copy position
        auto uPos = pos;

        // update start points of underlaying curves
        for(auto const &e : *this) {

            e.element->startPoint(uPos);
            uPos = e.element->endPoint();

        }


    }


    def::CurvePoint Curve::startPoint() const {

        // get start point of first element
        return begin()->startPoint();

    }


    double Curve::startCurvature() const {

        return begin()->startCurvature();

    }


    double Curve::endCurvature() const {

        return end()->endCurvature();

    }


    double Curve::curvature(double s) const {

        auto e = atPos(s);
        return e.element->curvature(e.position);

    }


    void Curve::curvature(const Eigen::RowVectorXd &s, const Eigen::RowVectorXd &kappa) {

        // check size
        if (s.size() != kappa.size())
            throw std::invalid_argument("sizes don't match");

        // set end point
        length(s(s.cols() - 1));

        // iterate over knots to create elements
        for (int i = 0; i < s.size() - 1; ++i)
            _addElement(s(i), s(i + 1) - s(i), kappa(i), kappa(i + 1));

    }


    void Curve::curvature(const Eigen::RowVectorXd &s, const Eigen::RowVectorXd &kappa0, const Eigen::RowVectorXd &kappa1) {

        // check size
        if (s.size() - 1 != kappa0.size() || kappa0.size() != kappa1.size())
            throw std::invalid_argument("sizes invalid");

        // set end point
        length(s(s.cols() - 1));

        // iterate over knots to create elements
        for (int i = 0; i < s.size() - 1; ++i)
            _addElement(s(i), s(i + 1) - s(i), kappa0(i), kappa1(i));

    }


    void Curve::_addElement(double s, double ds, double kappa0, double kappa1) {

        // check for element type
        if(fabs(kappa1) < def::EPS_CURVATURE && fabs(kappa0) < def::EPS_CURVATURE)
            emplace(s, new curve::Line(ds));
        else if(fabs(kappa1 - kappa0) < def::EPS_CURVATURE)
            emplace(s, new curve::Arc(ds, kappa0));
        else
            emplace(s, new curve::Spiral(ds, kappa0, kappa1));

    }


    def::CurvePoint Curve::pos(double s) const {

        // get position
        auto e = atPos(s);
        return e.element->pos(e.position);

    }


    Eigen::RowVectorXd Curve::steps(double dPhi_max, double s_max) const {

        Eigen::RowVectorXd res(1); res << 0.0;
        double s0 = 0.0;

        for(auto const &ee : *this) {

            // get curve
            auto ge = ee.element;

            // get steps from element. The last element of the current vector is deleted to avoid doubled steps since the
            // end of the previous element is the start of the current element
            Eigen::RowVectorXd t = ge->steps(dPhi_max, s_max);

            // overwrite res
            Eigen::RowVectorXd tmp(res.cols() + t.cols() - 1);
            tmp << res.head(res.cols() - 1).eval(), t.array() + s0;
            res = tmp;

            // update s0
            s0 += ge->length();

        }

        return res;

    }


    double Curve::length() const {

        return GeoElement::length();

    }


    void Curve::length(double len) {

        sequence::length(len);
        GeoElement::length(len);

    }


    void Curve::reverse(GeoElement *geo) const {

        auto crv = dynamic_cast<Curve*>(geo);

        // get length
        double l = length();

        // iterate over elements
        crv->length(l);
        for(auto const &e : *this) {

            // create element and fill with reversed data
            GeoElement *rev = e.element->createBlank();
            e.element->reverse(rev);

            // insert to curve
            crv->emplace(l - (e.position + e.length), std::move(rev));

        }

        // get end point and reserve angle
        auto sp = endPoint();
        sp.angle += M_PI;

        // set start point
        crv->startPoint(sp);

    }


    GeoElement* Curve::createBlank() const {

        return new Curve;

    }

}}

std::ostream& operator<< (std::ostream& o, const simmap::curve::Curve& crv) {

    using namespace simmap::curve;

    o << "Length: " << crv.length() << ": [";

    for(const auto &ge : crv) {

        o << ge.position << ": ";

        switch(ge.element->type()) {
            case GeoElement::Type::LINE:
                o << "L";
                break;
            case GeoElement::Type::ARC:
                o << "A";
                break;
            case GeoElement::Type::SPIRAL:
                o << "S";
                break;
            case GeoElement::Type::POLY_3:
                o << "P3";
                break;
            case GeoElement::Type::PARAM_POLY_3:
                o << "PP3";
                break;
            default:
                o << "unknown";
        }

        o << " (l=" << ge.length << "), ";

    }

    return o << "]";

}
