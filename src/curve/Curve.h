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
// Created by Jens Klimke on 02.04.18.
//

#ifndef SIMMAP_CURVE_CURVE_H
#define SIMMAP_CURVE_CURVE_H


#include "GeoElement.h"
#include <base/sequence.h>
#include <base/definitions.h>


namespace simmap {
namespace curve {


class Curve : public GeoElement, public base::sequence<GeoElement*>
{

private:


    void _addElement(double s, double ds, double kappa0, double kappa1);


public:

    Curve() = default;
    ~Curve() override = default;

    GeoElement::Type type() const override;
    base::VectorX parameters() const override;

    double startCurvature() const override;
    double endCurvature() const override;
    double curvature(double s) const override;

    void curvature(const base::VectorX& s, const base::VectorX& kappa);
    void curvature(const base::VectorX& s, const base::VectorX& kappa0, const base::VectorX& kappa1);

    void startPoint(const base::CurvePoint &pos) override;
    base::CurvePoint startPoint() const override;

    base::VectorX steps(double dPhi_max, double s_max) const override;
    double length() const override;
    void length(double len) override;
    base::CurvePoint pos(double s) const override;

    void reverse(GeoElement *geo) const override;
    GeoElement* createBlank() const override;

};

}} // namespace ::simmap::server

std::ostream& operator<< (std::ostream& o, const simmap::curve::Curve& crv);

#endif // SIMMAP_CURVE_CURVE_H
