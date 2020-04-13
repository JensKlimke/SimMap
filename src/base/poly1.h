/*
 * poly1.h
 *
 * MIT License
 *
 * Copyright (c) 2019 Jens Klimke <jens.klimke@rwth-aachen.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 *         of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 *         to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *         copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 *         copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *         AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef SIMMAP_BASE_POLYNOMIAL1_H
#define SIMMAP_BASE_POLYNOMIAL1_H

#include <vector>
#include "definitions.h"

namespace base {


    class poly1 {

    private:

        VectorX _params{};


    public:

        /**
         * Default constructor
         */
        poly1() = default;


        /**
         * Constructor to create polynomial of third order
         * @param p3 Parameter order 3
         * @param p2 Parameter order 2
         * @param p1 Parameter order 1
         * @param p0 Parameter order 0
         * TODO: change
         */
        poly1(double p3, double p2, double p1, double p0) {

            parameters(p3, p2, p1, p0);

        }


        /**
         * Copy constructor
         * @param p Polynomial
         */
        explicit poly1(VectorX &p) : _params(std::move(p)) {}


        /**
         * Returns the parameters of the polynomial
         * @return Parameters as vector
         */
        VectorX parameters() const {

            return _params;

        }


        /**
         * Sets the parameters of the polynomial
         * @param params Parameters
         */
        void parameters(const VectorX &params) {

            _params = params;

        }


        /**
         * Sets the parameters of the polynomial
         * @param p0 Parameter order 0
         * @param p1 Parameter order 1
         * @param p2 Parameter order 2
         * @param p3 Parameter order 3
         */
        void parameters(double p0, double p1, double p2, double p3) {

            _params = VectorX{p0, p1, p2, p3};

        }


        /**
         * Access operator to access the ith parameter
         * @param i Index of the parameter
         * @return Parameter
         */
        double operator[](size_t i) const {

            return _params.at(i);

        }


        /**
         * Operator to calculate the result at position x
         * @param x Position the polynomial will be evaluated at
         * @return Result
         */
        double operator()(double x) const {

            return this->horner(x);

        }


        /**
         * Operator to calculate the result at positions x
         * @param x Position vector the polynomial will be evaluated at
         * @return Results
         */
        VectorX operator()(const VectorX &x) const {

            // create output
            VectorX res(x.size());

            // iterate over entries
            for (unsigned int i = 0; i < x.size(); ++i)
                res[i] = this->horner(x[i]);

            return res;

        }


        /**
         * Returns the derivative of the polynomial with oder n-1
         * @return Polynomial representing the derivative
         */
        poly1 der() const {

            // create result vector
            VectorX res(_params.size() - 1);

            // iterate over elements
            long n = _params.size();
            while (--n > 0)
                res[_params.size() - 1 - n] = _params[_params.size() - 1 - n] * n;

            return poly1(res);

        }


        /**
         * Returns a polynomial with a shifted base
         * @param d The offset the base will be shifted
         * @return Shifted polynomial
         */
        poly1 shift(double d) const {

            if(_params.size() != 4)
                throw std::runtime_error("Polynomial must be of order 3 to perform this operation.");

            auto d2 = d * d;
            poly1 p = poly1(
                    _params[0],
                    -3.0 * _params[0] * d        + _params[1],
                     3.0 * _params[0] * d2 - 2.0 * _params[1] * d  + _params[2],
                    -1.0 * _params[0] * d2 * d +   _params[1] * d2 - _params[2] * d + _params[3]);

            return p;

        }


        /**
         * Calculates the polynomial fitted to the given points and derivatives at the boundaries
         * @param x0 Start position
         * @param x1 End position
         * @param y0 Start value
         * @param dy0 Start derivative
         * @param y1 End value
         * @param dy1 End derivative
         * @return Calculated polynomial
         */
        static poly1 order3_fromValueAndDerivative(double x0, double x1, double y0, double dy0,
                                                   double y1, double dy1) {

            auto a = 1.0 / (x0 - x1);
            auto a0 = -2.0 * a * y0 + 2.0 * a * y1 + dy0 + dy1;
            auto a1 = -3.0 * a * (-x0 - x1) * y0 + 3.0 * a * (-x0 - x1) * y1 - (x0 + 2.0 * x1) * dy0 - (2.0 * x0 + x1) * dy1;
            auto a2 = -6.0 * x0 * x1 * a * y0 + 6.0 * x0 * x1 * a * y1 + (x1 * x1 + 2.0 * x0 * x1) * dy0 + (x0 * x0 + 2.0 * x1 * x0) * dy1;
            auto a3 = (3.0 * x0 * x1 * x1 - x1 * x1 * x1) * a * y0 + (x0 * x0 * x0 - 3.0 * x0 * x0 * x1) * a * y1 - x0 * x1 * x1 * dy0 - x0 * x0 * x1 * dy1;

            auto det = (1.0 / ((x0 - x1) * (x0 - x1)));
            return poly1({det * a0, det * a1, det * a2, det * a3});

        }


    private:


        /**
         * Horner scheme function to calculate the result efficiently
         * @param x Position the polynomial will be evaluated at
         * @return Result
         */
        double horner(double x) const {

            // horner scheme
            double res = _params[0];
            for (int i = 1; i < _params.size(); ++i)
                res = res * x + _params[i];

            return res;

        }


    };


} // namespace

#endif // SIMMAP_BASE_POLYNOMIAL1_H
