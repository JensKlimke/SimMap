//
// Created by Jens Klimke on 15.04.18.
//

#include "GeoElement.h"

#include <cmath>
#include <algorithm>
#include <Eigen/Geometry>
#include <Eigen/Core>
#include <base/m_assert.h>
#include <base/definitions.h>


namespace simmap {
namespace curve {


    void GeoElement::startPoint(const def::CurvePoint &pos) {

        r0 = pos.position;
        phi0 = pos.angle;

    }


    def::CurvePoint GeoElement::startPoint() const {

        return {r0, phi0, startCurvature()};

    }


    def::CurvePoint GeoElement::endPoint() const {

        return pos(length());

    }

    void GeoElement::length(double l) {

        m_assert(len >= 0.0, "Length must not be negative");
        len = l;

    }


    double GeoElement::length() const {

        return len;

    }


    std::vector<def::CurvePoint> GeoElement:: positions(const Eigen::RowVectorXd &v) const {

        // create result vector
        std::vector<def::CurvePoint> r(static_cast<size_t>(v.size()));

        // iterate over knots
        for (int i = 0; i < v.size(); ++i)
            r[i] = pos(v(i));

        return r;

    }


    std::vector<def::CurvePoint> GeoElement::operator()(const Eigen::RowVectorXd &v) const {

        return positions(v);

    }


    def::CurvePoint GeoElement::operator()(double s) const {

        return pos(s);

    }


    def::CurvePoint GeoElement::pos(double s, double d) const {

        def::CurvePoint p = pos(s);

        // rotate offset
        Eigen::Transform <double, 3, Eigen::Affine> t = Eigen::Transform <double, 3, Eigen::Affine>::Identity();
        t.translate(p.position);
        t.rotate(Eigen::AngleAxisd(p.angle, Eigen::Vector3d::UnitZ()));

        // translate and rotate position
        Eigen::Vector3d y(0.0, d, 0.0);
        p.position = t * y;

        // recalculate curvature
        auto c = p.curvature;
        p.curvature = c / (1.0 - d * c);

        return p;

    }


    Eigen::RowVectorXd GeoElement::steps(double dPhi_max, double s_max) const {

        // create vector
        std::vector<double> stps;

        // calculate change of curvature
        double sigma = fabs((endCurvature() - startCurvature()) / length());

        double s = 0.0;
        while(s < length()) {

            // add s to vector
            stps.push_back(s);

            // get ds and calculate s
            auto k0 = fabs(curvature(s));

            // calculate step
            auto t0 = dPhi_max / k0;
            if(sigma > def::EPS_CURVATURE)
                t0 = (1.0 / sigma) * (-1.0 * k0 + sqrt(k0 * k0 + 2.0 * dPhi_max * sigma));

            // calculate step
            auto ds = std::min(t0, s_max);
            s += ds;

        }

        // add last s
        stps.push_back(length() - def::EPS_DISTANCE);

        // put values to vector
        Eigen::RowVectorXd res = Eigen::RowVectorXd::Map(stps.data(), stps.size());

        return res;

    }


}}