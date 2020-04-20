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
// Created by Jens Klimke on 2018/08/15
//

#include <base/definitions.h>
#include <base/functions.h>
#include "C3Spline.h"

namespace simmap {
namespace curve {


    C3Spline::C3Spline(const base::VectorX& s, const std::vector<base::VectorX>& p) {

        fromDefinition(s, p);

    }


    C3Spline::C3Spline(const base::VectorX& s, const base::VectorX& w) {

        fromValues(s, w);

    }


    void C3Spline::fromDefinition(const base::VectorX& s, const std::vector<base::VectorX> &p) {

        if (s.size() - 1 != p.size())
            throw std::invalid_argument("s must be +1 larger than p");

        // set length
        double s0 = -1.0 * INFINITY;

        // iterate over vector
        for(int i = 0; i < p.size(); ++i) {

            // check s
            if (s[i] <= s0)
                throw std::invalid_argument("s must increase monotonically");

            // create element such that derivatives are zero and values are as given
            insert(s[i], p[i][0], p[i][1], p[i][2], p[i][3]);

            // save last s
            s0 = s[i];

        }

        length(s[s.size() - 1] - s[0]);

    }


    void C3Spline::fromValues(const base::VectorX& s, const base::VectorX& w) {

        fromValuesAndDerivatives(s, w, base::zeros(w.size()));

    }


    void C3Spline::fromValuesAndDerivatives(const base::VectorX& s, const base::VectorX& w, const base::VectorX& der) {

        if (der.size() != w.size() || s.size() != w.size())
            throw std::invalid_argument("Vectors must have the same length");

        // iterate over vector
        for(int i = 1; i < w.size(); ++i) {

            // check s
            if (s[i - 1] >= s[i])
                throw std::invalid_argument("s must increase strict monotonically");

            // create element such that derivatives are zero and values are as given
            sequence::emplace(s[i - 1], base::poly1::order3_fromValueAndDerivative(0.0,
                    s[i] - s[i - 1], w[i - 1], der[i - 1], w[i], der[i]));

        }

        // set length
        length(s[s.size() - 1] - s[0]);

    }


    void C3Spline::insert(double s, double p0, double p1, double p2, double p3) {

        sequence::emplace(s, base::poly1(p0, p1, p2, p3));

    }


    double C3Spline::operator()(double s) const {

        // check if spline has elements
        if(empty())
            return 0.0;

        auto e = atPos(s);
        return e.element(e.position);

    }

}}