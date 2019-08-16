//
// Created by Jens Klimke on 12.11.18.
//

#ifndef SIMMAP_SERVER_LANEEDGE_H
#define SIMMAP_SERVER_LANEEDGE_H


#include <graph/Edge.h>
#include <graph/Object.h>
#include "LaneSeparation.h"
#include "Track.h"

namespace simmap {
namespace server {

class MapCoordinate;


/**
 * class LaneEdge
 * @author Jens Klimke <jens.klimke@rwth-aachen.de>
 */
struct LaneEdge : public ::graph::Edge {


    Track::TrackElement _trackElement = {def::Orientation::NONE, nullptr}; //!< The track element the edge belongs to (e.g. a road)

    base::sequence<LaneSeparation*> _separations_left{};  //!< A sequence of separation objects, which separate the edge from the neighbored left hand side edge
    base::sequence<LaneSeparation*> _separations_right{}; //!< A sequence of separation objects, which separate the edge from the neighbored right hand side edge


    /**
     * Default constructor
     */
    LaneEdge() = default;



    /**
     * Deleted copy constructor
     */
    LaneEdge(const LaneEdge&) = delete;



    /**
     * Default destructor
     */
    ~LaneEdge() override = default;



    /**
     * Returns the length of the edge
     * @return Length of the edge
     */
    double length() const override = 0;



    /**
     * Returns the start point of the lane (center point of the lane at the entrance with respect to the lane direction)
     * @return Start point
     */
    def::CurvePoint startPoint() const;



    /**
     * Returns the end point of the lane (center point of the lane at the exit with respect to the lane direction)
     * @return End point
     */
    def::CurvePoint endPoint() const;



    /**
     * Calculates the center position at the given point s
     * @param s Point within the lane with respect to direction
     * @return The position
     */
    def::CurvePoint position(double s) const;



    /**
     *
     * Calculates the position at the given point s
     * @param s Point within the lane with respect to direction
     * @param ref Reference within the lane
     * @param d An additional offset to be respected
     * @return The position
     */
    virtual def::CurvePoint position(double s, def::Reference ref, double d) const = 0;



    /**
     * Returns the width at the given position
     * @param s Point within the edge
     * @return Width of the edge
     */
    virtual double width(double s) const = 0;



    /**
     * Returns the track element
     * @return Track element
     */
    Track::TrackElement trackElement() const;



};


}
} // namespace ::simmap::server

std::ostream& operator<< (std::ostream& os, const simmap::server::LaneEdge& edge);

#endif // SIMMAP_SERVER_LANEEDGE_H
