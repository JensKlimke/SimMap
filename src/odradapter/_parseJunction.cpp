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
// Created by Jens Klimke on 09.05.2019.
//

#include <map>
#include <odr/odr1_5_structure.h>
#include "ODRJunction.h"
#include "ODRRoad.h"

void parseJunction(ODRJunction *junc, const odr1_5::t_junction &j,
                   std::map<std::string, std::shared_ptr<ODRRoad>> &roads,
                   std::map<std::string, std::shared_ptr<ODREdge>> &edges) {

    // iterate over connections
    for (const auto &cElem : j.sub_connection) {

        // get id
        auto id = *cElem._id;
        junc->id = id;

        // get connection information (road, contact point and ID)
        auto rdIncId = *cElem._incomingRoad;
        auto rdConId = *cElem._connectingRoad;
        auto cpInc = *cElem._contactPoint == "end" ? def::ContactPoint::END : def::ContactPoint::START;

        // check if roads exist
        if (roads.find(rdIncId) == roads.end() || roads.find(rdConId) == roads.end()) {

            std::cerr << "Connection defined with non-existing road(s) ("
                      << rdIncId << ", " << rdConId << ")" << std::endl << std::flush;

            continue;

        }

        // get roads
        auto &rInc = roads.at(rdIncId);
        auto &rCon = roads.at(rdConId);

        // parse lane links
        for (auto const &lElem : cElem.sub_laneLink) {

            // read lane connections
            int from = *lElem._from;
            int to = *lElem._to;

            // get contact point of connecting road
            auto cs = rCon->lanes.crossSection(def::ContactPoint::START);
            auto cpCon = !cs.right()->empty() ? def::ContactPoint::START : def::ContactPoint::END;

            // get lanes
            try {

                // get lanes
                ODREdge *f = rInc->lanes.lane(cpInc, from);
                ODREdge *t = rCon->lanes.lane(cpCon, to);

                // save lane
                f->next(t);

            } catch (...) {

                std::cerr << "Could not create lane connection of junction " << *j._id
                          << " (road " << rCon->id() << ", lanes " << from << " ->  " << to << ")"
                          << std::endl << std::flush;
                continue;

            }

        }

    }

}