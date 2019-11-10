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
