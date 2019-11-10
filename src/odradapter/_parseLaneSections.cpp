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
// Created by Jens Klimke on 2019/05/09
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
