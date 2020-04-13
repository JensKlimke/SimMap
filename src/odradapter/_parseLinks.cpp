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
#include "ODRRoad.h"
#include "ODRJunction.h"
#include <odr/odr1_5_structure.h>


void parseLinks(const odr1_5::OpenDRIVE *odr,
                std::map<std::string, std::shared_ptr<ODRRoad>> &roads,
                const std::map<std::string, std::shared_ptr<ODRJunction>> &juncs) {

    for (auto &r : odr->sub_road) {

        // get road
        auto road = roads.at(*r._id);

        // get predecessor
        auto pred = r.sub_link->sub_predecessor;
        if (pred) {

            // get id and contact point
            auto &id = *pred->_elementId;

            // get element type
            auto &et = *pred->_elementType;
            if (et == "road") {

                // get road and start lane section
                auto pr = roads.at(*pred->_elementId).get();
                auto ls = road->lanes.crossSection(base::ContactPoint::START);

                // iterate over lanes
                for (auto ln : *ls.laneSectionRight()->right()) {

                    // iterate over predecessors
                    for (auto lid : ln->_pred) {

                        // get contact point and lane
                        auto cp = *pred->_contactPoint == "end" ? base::ContactPoint::END : base::ContactPoint::START;
                        auto lsc = pr->lanes.lane(cp, lid);

                        // link lane
                        ln->link(lsc, true);

                    }

                }


                // link center lanes
                if(ls.laneSectionCenter()->center() != nullptr) {

                    // get lane
                    auto ln = ls.laneSectionCenter()->center();

                    // iterate over predecessors
                    for (auto lid : ln->_pred) {

                        // get contact point and lane
                        auto cp = *pred->_contactPoint == "end" ? base::ContactPoint::END : base::ContactPoint::START;
                        auto lsc = pr->lanes.lane(cp, lid);

                        // link lane
                        ln->prev(lsc, base::ContactPoint::START, false);

                    }

                }


                // iterate over lanes
                for (auto ln : *ls.laneSectionLeft()->left()) {

                    // iterate over predecessors
                    for (auto lid : ln->_pred) {

                        // get contact point and lane
                        auto cp = *pred->_contactPoint == "end" ? base::ContactPoint::END : base::ContactPoint::START;
                        auto lsc = pr->lanes.lane(cp, lid);

                        // link lane
                        ln->link(lsc, true);

                    }

                }

            }

        }

        // get successor
        auto succ = r.sub_link->sub_successor;
        if (succ) {

            // get id and contact point
            auto &id = *succ->_elementId;

            // get element type
            auto &et = *succ->_elementType;
            if (et == "road") {

                // get road and start lane section
                auto pr = roads.at(*succ->_elementId).get();
                auto ls = road->lanes.crossSection(base::ContactPoint::END);

                // iterate over lanes
                for (auto ln : *ls.laneSectionRight()->right()) {

                    // iterate over successors
                    for (auto lid : ln->_succ) {

                        // get contact point and lane
                        auto cp = *succ->_contactPoint == "end" ? base::ContactPoint::END : base::ContactPoint::START;
                        auto lsc = pr->lanes.lane(cp, lid);

                        // link lane
                        ln->link(lsc, false);

                    }

                }


                // link center lanes
                if(ls.laneSectionCenter()->center() != nullptr) {

                    // get lane
                    auto ln = ls.laneSectionCenter()->center();

                    // iterate over predecessors
                    for (auto lid : ln->_succ) {

                        // get contact point and lane
                        auto cp = *succ->_contactPoint == "end" ? base::ContactPoint::END : base::ContactPoint::START;
                        auto lsc = pr->lanes.lane(cp, lid);

                        // link lane
                        ln->next(lsc, base::ContactPoint::END, false);

                    }

                }


                // iterate over lanes
                for (auto ln : *ls.laneSectionLeft()->left()) {

                    // iterate over successors
                    for (auto lid : ln->_succ) {

                        // get contact point and lane
                        auto cp = *succ->_contactPoint == "end" ? base::ContactPoint::END : base::ContactPoint::START;
                        auto lsc = pr->lanes.lane(cp, lid);

                        // link lane
                        ln->link(lsc, false);

                    }

                }

            }

        }

    }

}
