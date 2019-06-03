//
// Created by klimke on 09.05.2019.
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