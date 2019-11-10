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
// Created by Jens Klimke on 2019-05-08.
//

#include <cmath>
#include <odr/odr1_5_structure.h>
#include <curve/C3Spline.h>
#include <base/definitions.h>
#include <server/Track.h>
#include "LaneSection.h"
#include "ODREdge.h"
#include "ODRRoad.h"



template <typename T>
std::shared_ptr<ODREdge> parseGenericLane(LaneSection *ls, const T &def, int lid) {

    using namespace simmap::curve;
    using namespace simmap::server;

    // create edge
    auto edge = std::make_shared<ODREdge>();

    // generate id
    std::stringstream strid{};
    
    if(lid == 0)
        strid << "R" << ls->_road->id() << "-LS" << ls->_id << "-C";
    else
        strid << "R" << ls->_road->id() << "-LS" << ls->_id << (lid > 0 ? "-L" : "-R") << abs(lid);

    // set numeric ID and stringified id
    edge->_laneID = lid;
    edge->_id = strid.str();

    // parse lane type, TODO: all other types
    edge->_drivingType = (def._type && (*def._type == "driving" || *def._type == "biking"))
                         ? ODREdge::DrivingType::DRIVABLE : ODREdge::DrivingType::RESTRICTED;

    // create lane in lane section and link edge
    ls->createLane(lid, edge.get());

    // save road
    edge->_road = ls->_road;
    edge->_trackElement = Track::TrackElement{def::Orientation::BACKWARDS, edge->_road};


    // parse links

    // clear vectors
    edge->_pred.clear();
    edge->_succ.clear();

    if (def.sub_link) {

        // alloc memory
        edge->_pred.reserve(def.sub_link->sub_predecessor.size());
        edge->_succ.reserve(def.sub_link->sub_successor.size());

        // parse predecessor links
        for (auto &lnk : def.sub_link->sub_predecessor)
            edge->_pred.push_back(*lnk._id);

        // parse successor links
        for (auto &lnk : def.sub_link->sub_successor)
            edge->_succ.push_back(*lnk._id);

    }


    return edge;


}


std::shared_ptr<ODREdge> parseLane(LaneSection *ls, const odr1_5::t_road_lanes_laneSection_lr_lane &def, int lid) {

    using namespace simmap::curve;
    using namespace simmap::server;

    // create generic edge
    auto edge = parseGenericLane(ls, def, lid);

    // get width and border
    auto widthDef = def.sub_width;
    auto borderDef = def.sub_border;

    // if width is not defined, is border defined?
    if (widthDef.empty() && borderDef.empty()) {

        Eigen::RowVector2d s{0.0, INFINITY};
        edge->_width = std::make_unique<C3Spline>(s, Eigen::Matrix2d::Zero());

    } else if (!widthDef.empty()) {

        Eigen::RowVectorXd s(widthDef.size() + 1);
        s(widthDef.size()) = INFINITY;

        // create matrix
        auto m = Eigen::Matrix<double, 4, Eigen::Dynamic>(4, widthDef.size());
        size_t i = 0;
        for (const auto &w : widthDef) {

            s(i) = *w._sOffset;
            m.col(i++) = Eigen::Vector4d(*w._d, *w._c, *w._b, *w._a);

        }

        // set width
        edge->_width = std::make_unique<C3Spline>(s, m);

    } else {

        Eigen::RowVectorXd s(borderDef.size() + 1);
        s(borderDef.size()) = INFINITY;

        // create matrix
        auto m = Eigen::Matrix<double, 4, Eigen::Dynamic>(4, borderDef.size());
        size_t i = 0;
        for (const auto &w : borderDef) {

            s(i) = *w._sOffset;
            m.col(i++) = Eigen::Vector4d(*w._d, *w._c, *w._b, *w._a);

        }

        // set border
        edge->_border = std::make_unique<C3Spline>(s, m);

    }

    return edge;


}


std::shared_ptr<ODREdge> parseCenterLane(LaneSection *ls, const odr1_5::t_road_lanes_laneSection_center_lane &def, int lid) {

    // create generic edge
    return parseGenericLane(ls, def, lid);

}

