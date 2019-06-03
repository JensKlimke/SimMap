//
// Created by klimke on 09.05.2019.
//

#include "ODRRoad.h"
#include <curve/Curve.h>
#include <curve/Line.h>
#include <curve/Arc.h>
#include <curve/Spiral.h>
#include <curve/Poly3.h>
#include <odr/odr1_5_structure.h>



void parseCurve(ODRRoad *road, const odr1_5::t_road &r) { // ### CURVE ###

    using namespace simmap::curve;

    // set length
    road->_length = *r._length;

    // creating curve
    road->_curve = std::make_shared<Curve>();
    auto crv = road->_curve.get();

    // get number of geo elements and allocate vector
    auto n = r.sub_planView->sub_geometry.size();
    std::vector<double> lengths(n);

    // iterate over geo elements
    const auto &v = r.sub_planView->sub_geometry;
    for (size_t i = 0; i < n - 1; ++i)
        lengths[i] = *v.at(i + 1)._s - *v.at(i)._s;

    // set last element length
    lengths[n - 1] = *r._length - *v.at(n - 1)._s;

    // iterate over geo elements
    size_t i = 0;
    double ign = 0.0;
    for (auto const &geo : r.sub_planView->sub_geometry) {

        // ignore geo elements which are too small
        if (lengths[i] < def::EPS_DISTANCE) {
            ign += lengths[i++];
            continue;
        }

        // get empty geo element and calculate length (add ignored lengths)
        GeoElement *ge = nullptr;

        auto len = lengths[i] + ign;

        auto l1 = len;
        auto l2 = l1 * l1;
        auto l3 = l2 * l1;

        if (geo.sub_line)
            ge = new Line(len);
        else if (geo.sub_spiral)
            ge = new Spiral(len, *geo.sub_spiral->_curvStart, *geo.sub_spiral->_curvEnd);
        else if (geo.sub_arc)
            ge = new Arc(len, *geo.sub_arc->_curvature);
        else if (geo.sub_poly3) {

            // parameters
            double x[4] = {0.0, 0.0, 1.0 * l1, 0.0};
            double y[4] = {*geo.sub_poly3->_d * l3, *geo.sub_poly3->_c * l2,
                           *geo.sub_poly3->_b * l1, *geo.sub_poly3->_a};

            // create poly
            ge = new Poly3(len, x, y);

        } else if (geo.sub_paramPoly3) {

            // set lengths to one when normalized
            if (!geo.sub_paramPoly3->_pRange || *geo.sub_paramPoly3->_pRange != "arcLength") {
                l1 = 1.0;
                l2 = 1.0;
                l3 = 1.0;
            }

            // parameters
            double x[4] = {*geo.sub_paramPoly3->_dU * l3, *geo.sub_paramPoly3->_cU * l2,
                           *geo.sub_paramPoly3->_bU * l1, *geo.sub_paramPoly3->_aU};
            double y[4] = {*geo.sub_paramPoly3->_dV * l3, *geo.sub_paramPoly3->_cV * l2,
                           *geo.sub_paramPoly3->_bV * l1, *geo.sub_paramPoly3->_aV};

            // create poly
            ge = new Poly3(len, x, y);

        } else {

            // error
            throw std::runtime_error("element not defined or undefined type.");

        }

        // set start point and add to curve
        ge->startPoint({Eigen::Vector3d(*geo._x, *geo._y, 0.0), *geo._hdg, 0.0});
        crv->emplace(*geo._s, std::move(ge));

        i++;
        ign = 0.0;

    }

    // update length
    crv->length(*r._length);

}
