//
// Created by Jens Klimke on 05.08.18.
//

#ifndef SIMMAP_SERVER_MAP_H
#define SIMMAP_SERVER_MAP_H

#include <graph/Graph.h>
#include "LaneEdge.h"
#include "Track.h"

namespace simmap {
namespace server {


    /**
     * struct Map
     * @author Jens Klimke jens.klimke@rwth-aachen.de
     */
    struct Map {

        graph::Graph _roadNetwork{};
        graph::Graph _laneNetwork{};

        Map() = default;
        virtual ~Map() = default;


        /**
         * Returns the desired edge as LaneEdge
         * @param name Name of the edge
         * @return Edge
         */
        const LaneEdge *getEdge(const std::string &name) {

            return dynamic_cast<LaneEdge*>(_laneNetwork.at(name).get());

        }

    };

}} // namespace ::simmap::server

#endif // SIMMAP_SERVER_MAP_H
