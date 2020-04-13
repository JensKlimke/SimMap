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
// Created by Jens Klimke on 14.04.18.
//

#ifndef SIMMAP_CURVE_C3SPLINE_H
#define SIMMAP_CURVE_C3SPLINE_H

#include <base/sequence.h>
#include <base/poly1.h>

namespace simmap {
namespace curve {

    /**
     * class C3Spline
     * @author Jens Klimke <jens.klimke@rwth-aachen.de>
     * TODO: comments
     */
    class C3Spline : public base::sequence<base::poly1> {


    private:

        void fromDefinition(const base::VectorX& s, const std::vector<base::VectorX> &p);
        void fromValues(const base::VectorX& s, const base::VectorX& w);
        void fromValuesAndDerivatives(const base::VectorX& s, const base::VectorX& w, const base::VectorX& der);

    public:

        C3Spline() = default;
        C3Spline(const C3Spline& ps) = default;
        C3Spline(const base::VectorX& s, const std::vector<base::VectorX>& p);
        C3Spline(const base::VectorX& s, const base::VectorX& w);
        explicit C3Spline(double width);

        ~C3Spline() = default;

        void insert(double s, double p0, double p1, double p2, double p3);

        base::VectorX operator() (const base::VectorX& s) const;
        double operator() (double s) const;

        base::VectorX der(const base::VectorX& s) const;
        double der(double s) const;

        base::VectorX steps(double dPhi_max, double s_max) const;

    };

}} // namespace ::simmap::curve

#endif // SIMMAP_CURVE_C3SPLINE_H
