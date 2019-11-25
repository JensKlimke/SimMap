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

#include <Eigen/Core>
#include <Eigen/LU>

namespace base {


    class poly1 {

    private:

        Eigen::VectorXd _params{};


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
        explicit poly1(Eigen::VectorXd p) : _params(std::move(p)) {}


        /**
         * Returns the parameters of the polynomial
         * @return Parameters as vector
         */
        Eigen::VectorXd parameters() const {

            return _params;

        }


        /**
         * Sets the parameters of the polynomial
         * @param params Parameters
         */
        void parameters(const Eigen::VectorXd &params) {

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

            _params = Eigen::Vector4d(p0, p1, p2, p3);

        }


        /**
         * Access operator to access the ith parameter
         * @param i Index of the parameter
         * @return Parameter
         */
        double operator[](size_t i) const {

            return _params(i);

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
        Eigen::MatrixXd operator()(const Eigen::MatrixXd& x) const {

            // create output
            Eigen::MatrixXd res(x.rows(), x.cols());

            // iterate over entries
            for (int i = 0; i < x.size(); ++i)
                res(i) = this->horner(x(i));

            return res;

        }


        /**
         * Returns the derivative of the polynomial with oder n-1
         * @return Polynomial representing the derivative
         */
        poly1 der() const {

            // create result vector
            Eigen::VectorXd res(_params.size() - 1);

            // iterate over elements
            long n = _params.size();
            while (--n > 0)
                res(_params.size() - 1 - n) = _params(_params.size() - 1 - n) * n;

            return poly1(res);

        }


        /**
         * Returns a polynomial with a shifted base
         * @param d The offset the base will be shifted
         * @return Shifted polynomial
         */
        poly1 shift(double d) const {

            auto d2 = d * d;

            poly1 p = poly1(
                    _params(0),
                    -3.0 * _params(0) * d + _params(1),
                    3.0 * _params(0) * d2 - 2.0 * _params(1) * d + _params(2),
                    -1.0 * _params(0) * d2 * d + _params(1) * d2 - _params(2) * d + _params(3));

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

            // calculate s
            Eigen::Vector2d t(x0, x1);

            // s^2 and s^3
            Eigen::Vector2d t2 = t.array() * t.array();
            Eigen::Vector2d t3 = t2.array() * t.array();

            // define system matrix (matrix is stored transposed)
            Eigen::Matrix4d M;
            M <<    t3(0),       t2(0),  t(0),  1.0,
                    t3(1),       t2(1),  t(1),  1.0,
                    3.0 * t2(0),  2.0 * t(0),   1.0,  0.0,
                    3.0 * t2(1),  2.0 * t(1),   1.0,  0.0;

            // define lhs and solve
            Eigen::Vector4d y(y0, y1, dy0, dy1);
            Eigen::Vector4d x(M.inverse() * y);

            return poly1(x);

        }


    private:


        /**
         * Horner scheme function to calculate the result efficiently
         * @param x Position the polynomial will be evaluated at
         * @return Result
         */
        double horner(double x) const {

            // horner scheme
            double res = _params(0);
            for (int i = 1; i < _params.size(); ++i)
                res = res * x + _params(i);

            return res;

        }


    };


} // namespace

#endif // SIMMAP_BASE_POLYNOMIAL1_H
