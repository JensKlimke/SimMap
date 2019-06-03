//
// Created by klimke on 09.05.2019.
//

#include <map>
#include <memory>
#include <odr/odr1_5_structure.h>
#include <server/Track.h>
#include "ODREdge.h"
#include "ODRRoad.h"
#include "LaneSectionSequence.h"



// predefine lane parser
std::shared_ptr<ODREdge> parseLane(LaneSection *ls, const odr1_5::t_road_lanes_laneSection_lr_lane &def, int lid);
std::shared_ptr<ODREdge> parseCenterLane(LaneSection *ls, const odr1_5::t_road_lanes_laneSection_center_lane &def, int lid);


void parseLaneSections(std::map<std::string, std::shared_ptr<ODREdge>> &edges,
                       const odr1_5::t_road &rd,
                       const std::map<std::string, std::shared_ptr<ODRRoad>> &roads) {

    using namespace simmap::server;

    // get lane sections vector
    auto &lns = rd.sub_lanes;
    const auto &road = roads.at(*rd._id);
    auto laneSections = &road->lanes;

    // create counter and get number of elements
    unsigned int li = 0;

    // iterator over lane sections
    for (const auto &lsElem : lns->sub_laneSection) {

        // get lanes
        const auto &lftLns = lsElem.sub_left;
        const auto &ctrLns = lsElem.sub_center;
        const auto &rgtLns = lsElem.sub_right;

        // get single side flag
        bool ss = lsElem._singleSide && *lsElem._singleSide == "true";

        // get side and create lane section
        def::Side side = ss ? (rgtLns ? def::Side::RIGHT : def::Side::LEFT) : def::Side::BOTH;
        auto ls = laneSections->create(*lsElem._s, side);

        // set lane section data
        ls->_road = road.get();
        ls->_id = ++li;

        // add left lanes
        if (lftLns) {

            for (auto &i : lftLns->sub_lane) {

                // create and save edge
                auto ptr = parseLane(ls, i, *i._id);
                edges[ptr->id()] = ptr;

            }

        }


        // add center lanes
        if (ctrLns) {

            for (auto &i : ctrLns->sub_lane) {

                // create and save edge
                auto ptr = parseCenterLane(ls, i, *i._id);
                edges[ptr->id()] = ptr;

            }

        }


        // add right lanes
        if (rgtLns) {

            for (auto &i : rgtLns->sub_lane) {

                // create and save edge
                auto ptr = parseLane(ls, i, *i._id);
                edges[ptr->id()] = ptr;

            }

        }

    }

    // create index
    laneSections->length(road->length());
    laneSections->index();

}
