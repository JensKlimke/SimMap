
//
// Created by klimke on 17.10.2018.
//

#ifndef SIMMAP_SERVER_MAPCOORDINATE_H
#define SIMMAP_SERVER_MAPCOORDINATE_H

#include <iostream>
#include <base/definitions.h>



namespace simmap {
namespace server {

class LaneEdge;


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
    def::CurvePoint absolutePosition() const;


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
