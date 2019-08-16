/*
 * functions.h
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

#ifndef SIMMAP_BASE_FUNCTIONS_H
#define SIMMAP_BASE_FUNCTIONS_H


#ifdef _WIN32
#define _USE_MATH_DEFINES
#include <math.h>
#endif

#include <cmath>
#include <fstream>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "definitions.h"


namespace base {


    /**
     * Calculates an equally spaced vector between the bounds a and b with the maximum distance ds
     * @param a Lower bound
     * @param b Upper bound
     * @param ds Maximum distance
     * @return
     */
    inline Eigen::RowVectorXd maxspace(double a, double b, double ds) {

        if ((b - a) * ds < 0.0)
            throw std::invalid_argument("(b-a)*ds must pe positive");

        // get number of elements
        unsigned long n = static_cast<unsigned long>(ceil((b - a) / ds) + 1);
        return Eigen::RowVectorXd::LinSpaced(n, a, b);

    }


    /**
     * Calculates the difference between two given angles (smallest signed difference)
     * @param p1 First angle
     * @param p0 Second angle
     * @return Difference between the angles
     */
    inline double angleDiff(double p1, double p0) {

        double d = p1 - p0;

        while (d <= M_PI)
            d += 2.0 * M_PI;

        while (d > M_PI)
            d -= 2.0 * M_PI;

        return d;

    }


    /**
     * Compares to files and returns the flag if the files are equal
     * @param p1 File name of first file
     * @param p2 File name of seconf file
     * @return Flag (true when files are equal)
     */
    inline bool compareFiles(const std::string &p1, const std::string &p2) {

        std::ifstream f1(p1, std::ifstream::binary | std::ifstream::ate);
        std::ifstream f2(p2, std::ifstream::binary | std::ifstream::ate);

        if (f1.fail() || f2.fail())
            return false; //file problem

        if (f1.tellg() != f2.tellg())
            return false; //size mismatch

        //seek back to beginning and use std::equal to compare contents
        f1.seekg(0, std::ifstream::beg);
        f2.seekg(0, std::ifstream::beg);

        return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                          std::istreambuf_iterator<char>(),
                          std::istreambuf_iterator<char>(f2.rdbuf()));
    }


    /**
     * Adds an value to the given point of the vector and moves all other elements
     * @param vec Vector to be updated
     * @param i Position of new value
     * @param val Value to be set
     */
    inline void addValueToVector(Eigen::RowVectorXd &vec, size_t i, double val) {

        if (vec.cols() == 0)
            throw std::invalid_argument("Vector must not be empty");

        if (i >= vec.cols())
            throw std::invalid_argument("i out of bounds");

        // check if i is last position
        if (i == vec.cols() - 1) {
            vec(i) = val;
            return;
        }

        // move tail by one position
        size_t n = vec.cols() - i - 1;
        vec.tail(n) = vec.segment(i, n).eval();

        // set value
        vec(i) = val;

    }


    /**
     * Removes the i-th element from the given vector
     * @param vec Vector to be updated
     * @param i Index of the element to be removed
     */
    inline void removeElementFromVector(Eigen::RowVectorXd &vec, size_t i) {

        if (vec.cols() == 0)
            throw std::invalid_argument("Vector must not be empty");

        if (i > vec.cols() - 1)
            throw std::invalid_argument("i out of bounds");

        // get head
        if (i == vec.cols() - 1) {

            vec = vec.head(vec.cols() - 1).eval();
            return;

        }

        // get tail
        if (i == 0) {

            vec = vec.tail(vec.cols() - 1).eval();
            return;

        }

        // copy vector and reallocate
        Eigen::RowVectorXd tmp = vec;
        vec = Eigen::RowVectorXd(tmp.cols() - 1);

        // save values
        vec << tmp.head(i).eval(), tmp.tail(tmp.cols() - i - 1).eval();

    }


    /**
     * Transfers the given point from the coordinate system of the curve point to the global coordinate system
     * @param curvePoint Definition of the coordinate system
     * @param point Point Point to be transfered
     * @return CurvePoint representing the point in the global coordinate system
     */
    inline Eigen::Vector3d toGlobal(const def::CurvePoint &curvePoint, const Eigen::Vector3d &point) {

        // create transformation
        Eigen::Transform <double, 3, Eigen::Affine> t = Eigen::Transform <double, 3, Eigen::Affine>::Identity();
        t.rotate(Eigen::AngleAxisd(curvePoint.angle, Eigen::Vector3d::UnitZ()));

        // create curve point and return
        return Eigen::Vector3d{t * point + curvePoint.position};

    }


    /**
     * Transfers the given point to the coordinate system of the curve point
     * @param curvePoint Definition of the coordinate system
     * @param point Point Point to be transfered
     * @return CurvePoint representing the point in the local coordinate system
     */
    inline Eigen::Vector3d toLocal(const def::CurvePoint &curvePoint, const Eigen::Vector3d &point) {

        // create transformation
        Eigen::Transform <double, 3, Eigen::Affine> t = Eigen::Transform <double, 3, Eigen::Affine>::Identity();
        t.rotate(Eigen::AngleAxisd(-curvePoint.angle, Eigen::Vector3d::UnitZ()));

        // create curve point and return
        return Eigen::Vector3d{t * (point - curvePoint.position)};

    }


    /**
     * Transfers the end point to the coordinate system of the base point
     * @param base Start point
     * @param end End point
     * @return CurvePoint representing the end point in the coordinate system of the base point
     */
    inline def::CurvePoint toLocal(const def::CurvePoint &base, const def::CurvePoint &end) {

        // create curve point and return
        return def::CurvePoint{toLocal(base, end.position), base::angleDiff(end.angle, base.angle), end.curvature};

    }


    /**
     * Flips the given orientation
     * @param ori Orientation to be flipped
     * @return Flipped orientation
     */
    inline def::Orientation flip(def::Orientation ori) {

        return ori == def::Orientation::FORWARDS ? def::Orientation::BACKWARDS :
               (ori == def::Orientation::BACKWARDS ? def::Orientation::FORWARDS :
                (ori == def::Orientation::BOTH ? def::Orientation::BOTH : def::Orientation::NONE));

    }



    /**
     * String formatter
     * @tparam Args
     * @param format Format string
     * @param args Inputs
     * @return Formatted string
     */
    template<typename ... Args>
    inline std::string string_format(const std::string &format, Args ... args) {
        auto size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
        char* buf(new char[size]);
        snprintf(buf, size, format.c_str(), args ...);
        return std::string(buf, buf + size - 1); // We don't want the '\0' inside
    }


} // namespace


#endif // SIMMAP_BASE_FUNCTIONS_H
