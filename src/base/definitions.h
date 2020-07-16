/*
 * definitions.h
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

#ifndef SIMMAP_BASE_DEFINITIONS_H
#define SIMMAP_BASE_DEFINITIONS_H

#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

namespace base {

    const double EPS_DOUBLE_CMP = 1e-12;
    const double EPS_DISTANCE = 1e-9;
    const double EPS_CURVATURE = 1e-9;
    const double EPS_ANGLE = 1e-9;

    enum class Unit {
        METER, KILOMETER, FEET, MILE, MPS, MPH, KPH, KILOGRAMS, TONS, PERCENT
    };
    enum class Side {
        LEFT, RIGHT, CENTER, BOTH, NONE
    };
    enum class Orientation { FORWARDS, BACKWARDS, BOTH, NONE };
    enum class ContactPoint { START, END };
    enum class Reference {INNER, OUTER, CENTER};


    /*!< Definition of dynamic vector */
    typedef std::vector<double> VectorX;


    /**
     * A simple 3D vector class
     */
    struct Vector3 {
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;
    };


    /**
     * Curve point class
     */
    struct CurvePoint {
        Vector3 position;
        double angle = 0.0;
        double curvature = 0.0;
    };

}



#endif // SIMMAP_BASE_DEFINITIONS_H
