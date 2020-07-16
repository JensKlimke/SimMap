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

#include <cmath>
#include <fstream>
#include "definitions.h"


namespace base {


    /**
     * Creates a vector with n equal-sized elements between the bounds a and b
     * @param a Lower bound
     * @param b Upper bound
     * @param n Number of elements
     * @return
     */
    inline VectorX linspace(double a, double b, size_t n) {

        if(n == 0 || n == 1)
            throw std::invalid_argument("n must be > 1");

        // create vector
        VectorX v(n);
        double d = (b - a) / ((double)n - 1.0);

        // set elements
        for(size_t i = 0; i < n; ++i)
            v[i] = a + i * d;

        return v;

    }


    /**
     * Calculates an equally spaced vector between the bounds a and b with the maximum distance ds
     * @param a Lower bound
     * @param b Upper bound
     * @param ds Maximum distance
     * @return
     */
    inline VectorX maxspace(double a, double b, double ds) {

        // get number of elements
        auto n = static_cast<unsigned long>(ceil(std::abs((b - a) / ds)) + 1);

        // return start point if solution is not possible
        if(n < 2)
            return {a};

        return linspace(a, b, n);

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
     * Creates a zero vector
     * @param n Number of elements
     * @return Vector with zeros
     */
        inline VectorX zeros(size_t n) {

            VectorX vec(n);
            for(size_t i = 0; i < n; ++i)
                vec[i] = 0.0;

            return vec;

        }


    /**
     * Transfers the given point from the coordinate system of the curve point to the global coordinate system
     * @param curvePoint Definition of the coordinate system
     * @param point Point Point to be transfered
     * @return CurvePoint representing the point in the global coordinate system
     */
    inline Vector3 toGlobal(const base::CurvePoint &curvePoint, const Vector3 &point) {


        auto c = cos(curvePoint.angle);
        auto s = sin(curvePoint.angle);

        auto x = point.x;
        auto y = point.y;
        auto z = point.z;

        return Vector3 {
            x * c - y * s + curvePoint.position.x,
            x * s + y * c + curvePoint.position.y,
            z + curvePoint.position.z
        };

    }


    /**
     * Transfers the given point to the coordinate system of the curve point
     * @param curvePoint Definition of the coordinate system
     * @param point Point Point to be transferred
     * @return CurvePoint representing the point in the local coordinate system
     */
    inline Vector3 toLocal(const base::CurvePoint &curvePoint, const Vector3 &point) {


        auto c = cos(curvePoint.angle);
        auto s = sin(curvePoint.angle);

        auto x = point.x - curvePoint.position.x;
        auto y = point.y - curvePoint.position.y;
        auto z = point.z - curvePoint.position.z;

        return Vector3 {x * c + y * s, -x * s + y * c, z};

    }


    /**
     * Flips the given orientation
     * @param ori Orientation to be flipped
     * @return Flipped orientation
     */
    inline base::Orientation flip(base::Orientation ori) {

        return ori == base::Orientation::FORWARDS ? base::Orientation::BACKWARDS :
               (ori == base::Orientation::BACKWARDS ? base::Orientation::FORWARDS :
                (ori == base::Orientation::BOTH ? base::Orientation::BOTH : base::Orientation::NONE));

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


/**
 * Operator to add two vectors
 * @param a First vector
 * @param b Second vector
 * @return Sum of the tqo vectors
 */
inline base::Vector3 operator+ (const base::Vector3 &a, const base::Vector3 &b) {

    return base::Vector3{a.x + b.x, a.y + b.y, a.z + b.z};

}

/**
 * Operator to subtract two vectors
 * @param a First vector
 * @param b Second vector
 * @return Sum of the tqo vectors
 */
inline base::Vector3 operator- (const base::Vector3 &a, const base::Vector3 &b) {

    return base::Vector3{a.x - b.x, a.y - b.y, a.z - b.z};

}


#endif // SIMMAP_BASE_FUNCTIONS_H
