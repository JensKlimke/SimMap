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
#include "C3Spline.h"

namespace simmap {
namespace curve {


    C3Spline::C3Spline(const base::VectorX& s, const std::vector<base::VectorX>& p) {

        // if vector is empty, set to zero
        if(p.empty())
            return;

        if(p[0].size() == 2)
            fromValuesAndDerivatives(s, p[0], p[1]);
        else if(p[0].size() == 1)
            fromValues(s, p[0]);
        else
            fromDefinition(s, p);

    }


    C3Spline::C3Spline(const base::VectorX& s, const base::VectorX& w) {

        fromValues(s, w);

    }


    C3Spline::C3Spline(double width) {

        fromValues(base::VectorX{0.0, 1.0}, base::VectorX{width, width});

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


    base::VectorX C3Spline::operator() (const base::VectorX& s) const {

        // create zeros vector
        base::VectorX res = base::zeros(s.size());

        // check if spline has elements
        if(empty())
            return res;

        // iterate over elements and calculate result
        for(size_t i = 0; i < s.size(); ++i)
            res[i] = operator()(s[i]);

        return res;

    }


    double C3Spline::operator()(double s) const {

        // check if spline has elements
        if(empty())
            return 0.0;

        auto e = atPos(s);
        return e.element(e.position);

    }


    base::VectorX C3Spline::der(const base::VectorX& s) const {

        // get elements and create result vector
        base::VectorX res(s.size());

        // iterate over elements and calculate result
        for(size_t i = 0; i < s.size(); ++i)
            res[i] = der(s[i]);

        return res;

    }


    double C3Spline::der(double s) const {

        auto e = atPos(s);
        return e.element.der()(e.position);

    }


    base::VectorX C3Spline::steps(double dPhi_max, double s_max) const {

        // create vector
        std::vector<double> stps;
        double s = 0.0; double s0 = 0.0;

        // iterate over elements
        for(auto &e : entries()) {

            // get element and position
            const base::poly1* p = e.element;
            double s1 = e.position + e.length;

            while (s < s1) {

                // add s to vector
                stps.push_back(s);

                // calculate derivative
                base::poly1 der = p->der().der();

                // get ds and calculate s
                double k0 = fabs(der(s - s0));
                double sigma = fabs(der.der()(s - s0));

                // calculate step
                double t0 = dPhi_max / k0;

                // check sigma
                if (sigma > base::EPS_CURVATURE)
                    t0 = (1.0 / sigma) * (-1.0 * k0 + sqrt(k0 * k0 + 2.0 * dPhi_max * sigma));

                // add step to s
                s += std::min(std::min(t0, s1 - s0), s_max);

            }

            // add last s
            s = std::min(s, s1);
            s0 = s1;

        }

        // add last point
        stps.push_back(s);

        // put values to vector
        base::VectorX res(stps.size());
        for(size_t i = 0; i < stps.size(); ++i)
            res[i] = stps[i];

        return res;

    }

}}