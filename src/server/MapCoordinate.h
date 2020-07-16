
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
// Created by Jens Klimke on 2018/10/17
//

#ifndef SIMMAP_SERVER_MAPCOORDINATE_H
#define SIMMAP_SERVER_MAPCOORDINATE_H

#include <iostream>
#include <base/definitions.h>

namespace simmap {
namespace server {

struct LaneEdge;


class MapCoordinate {

private:

    const LaneEdge *_edge = nullptr;

    double _s = 0.0;
    double _d = 0.0;


public:


    /**
     * Empty constructor
     */
    MapCoordinate() = default;


    /**
     * Copy constructors
     * @param c Map coordinate to be copied
     */
    MapCoordinate(const MapCoordinate& c) = default;


    /**
     * Constructor, tha sets the segment and the longitudinal and lateral position of the coordinate in the lane (in driving direction)
     * @param edge Edge (the lane representation)
     * @param s Longitudinal position
     * @param d Lateral position
     */
    MapCoordinate(const LaneEdge *edge, double s, double d);


    /**
     * Returns the lane of the map coordinate
     * @return The current lane
     */
    const LaneEdge* edge() const;


    /**
     * Returns the longitudinal position in the segment (in direction of driving)
     * @return Longitudinal position
     */
    double s() const;


    /**
     * Sets the position
     * @param s Position
     */
    void s(double s);


    /**
     * Returns the lateral position of the coordinate
     * @return Lateral position of the coordinate
     * @ TODO: rename this
     */
    double d() const;


    /**
     * Set the offset
     * @param d Offset
     */
    void d(double d);


    /**
     * Returns the absolute position of the map coordinate
     * @return Absolute position
     */
    base::CurvePoint absolutePosition() const;


    /**
     * Shifts the map coordinate by the given distance
     * @param ds Distance to be shifted
     * @ TODO: rename to move
     */
    void shift(double ds);


    /**
     * Returns the width of the lane at the current position
     * @return Width of the lane
     */
    double width() const;


    /**
     * Calculates and returns the parallel position on the left lane
     * @param n Number of lanes to got to the left
     * @return Parallel position on the left neighbored lane
     */
    MapCoordinate left(size_t n = 1) const;


    /**
     * Calculates and returns the parallel position on the right lane
     * @param n Number of lanes to go to the right
     * @return Parallel position on the right neighbored lane
     */
    MapCoordinate right(size_t n = 1) const;


    /**
     * Create an out-of-road map coordinate
     * @return Out
     * TODO: rename to invalid or simething like that
     */
    bool outOfRoad() const;




protected:

    /**
     * Calculates and returns the parallel position on the neighbored lane to the given side
     * @param n Number of lanes to go
     * @param toRight The side to go (true=right, false=left)
     * @return Parallel position on the neighbored lane
     */
    MapCoordinate neighbor(size_t n, bool toRight) const;


};

}} // namespace ::simmap::server

std::ostream& operator<< (std::ostream& o, const simmap::server::MapCoordinate& c);


#endif // SIMMAP_SERVER_MAPCOORDINATE_H
